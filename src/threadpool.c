#include "threadpool.h"
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "array.h"

void init_pool(thread_pool* pool, int thread_count, int max_tasks) {
    memset(pool, 0, sizeof(thread_pool));
    pool->threads = calloc(sizeof(pthread_t), thread_count);
    pool->thread_count = thread_count;
    pool->max_tasks = max_tasks;
}

void add_task(thread_pool* pool, task_function* fn, void* arg) {
    while (pool->task_count > pool->max_tasks) {
        usleep(10000);
    }
    pthread_mutex_lock(&pool->mutex);
    task* t = calloc(sizeof(task), 1);
    t->fn = fn;
    t->arg = arg;

    if (pool->task_tail) {
        pool->task_tail->next = t;
        t->prev = pool->task_tail;
    }
    if (!pool->task_head) {
        pool->task_head = t;
    }
    pool->task_tail = t;
    pool->task_count++;

    pthread_mutex_unlock(&pool->mutex);
}

void executor(executor_info* info) {
    thread_pool* pool = info->pool;
    while (1) {
        fflush(stdout);
        pthread_mutex_lock(&pool->mutex);
        if (pool->task_head) {
            task t = *pool->task_head;

            free(pool->task_head);
            pool->task_head = t.next;
            pool->task_count--;

            pthread_mutex_unlock(&pool->mutex);

            t.fn(t.arg, info->thread_num);
        } else {
            pthread_mutex_unlock(&pool->mutex);
            if (pool->exit) break;
        }
        usleep(10000);
    }
    free(info);
}

void start_pool(thread_pool* pool) {
    for (int i = 0; i < pool->thread_count; i++) {
        executor_info* info = calloc(sizeof(executor_info), 1);
        info->pool = pool;
        info->thread_num = i;
        pthread_create(&pool->threads[i], NULL, (void* (*)(void*))executor, (void*)info);
    }
}

void exit_pool(thread_pool* pool) {
    pool->exit = true;
}

void finalize_pool(thread_pool* pool) {
    for (int i = 0; i < pool->thread_count; i++)
        pthread_join(pool->threads[i], NULL);
    while (pool->task_head) {
        free(pool->task_head);
        pool->task_head = pool->task_head->next;
    }
    if (pool->threads)
        free(pool->threads);
}

void fn(void* arg, int thread_n) {
    sleep(1);
    printf("hoge\n");
    return;
}

int threadpool_test() {
    thread_pool pool;
    init_pool(&pool, 4, 5);
    start_pool(&pool);

    for (int i = 0; i < 10; i++) {
        add_task(&pool, fn, NULL);
    }

    pool.exit = true;
    finalize_pool(&pool);

    return 0;
}