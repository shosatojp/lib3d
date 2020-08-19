#include "pool.h"
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void pool_init(thread_pool* pool, int thread_count, int max_tasks) {
    memset(pool, 0, sizeof(thread_pool));
    pool->threads = calloc(sizeof(pthread_t), thread_count);
    pool->thread_count = thread_count;
    pool->max_tasks = max_tasks;
    pthread_mutex_init(&pool->mutex, NULL);
    pthread_cond_init(&pool->cond, NULL);
}

void pool_submit(thread_pool* pool, void (*fn)(void*, int), void* arg) {
    while (pool->task_count >= pool->max_tasks - 1)
        usleep(10000);

    pthread_mutex_lock(&pool->mutex);
    task* t = malloc(sizeof(task));
    t->fn = fn;
    t->arg = arg;
    t->next = NULL;
    t->prev = NULL;

    if (pool->task_head) {
        t->prev = pool->task_tail;
        pool->task_tail->next = t;
        pool->task_tail = t;
    } else {
        pool->task_head = pool->task_tail = t;
    }
    pool->task_count++;

    pthread_cond_signal(&pool->cond);
    pthread_mutex_unlock(&pool->mutex);
}

static void pool_executor(executor_info* info) {
    thread_pool* pool = info->pool;
    while (1) {
        pthread_mutex_lock(&pool->mutex);
        if (pool->task_head) {
            task t = *pool->task_head;

            free(pool->task_head);
            pool->task_head = t.next;
            pool->task_count--;

            pthread_mutex_unlock(&pool->mutex);

            t.fn(t.arg, info->thread_num);
        } else {
            if (pool->exit) {
                pthread_mutex_unlock(&pool->mutex);
                break;
            } else {
                pthread_cond_wait(&pool->cond, &pool->mutex);
                pthread_mutex_unlock(&pool->mutex);
            }
        }
    }
    free(info);
}

void pool_start(thread_pool* pool) {
    for (int i = 0; i < pool->thread_count; i++) {
        executor_info* info = malloc(sizeof(executor_info));
        info->pool = pool;
        info->thread_num = i;
        pthread_create(&pool->threads[i], NULL, (void* (*)(void*))pool_executor, (void*)info);
    }
}

void pool_exit(thread_pool* pool) {
    pool->exit = true;
}

void pool_finalize(thread_pool* pool) {
    pthread_cond_broadcast(&pool->cond);
    for (int i = 0; i < pool->thread_count; i++)
        pthread_join(pool->threads[i], NULL);
    while (pool->task_head) {
        free(pool->task_head);
        pool->task_head = pool->task_head->next;
    }
    if (pool->threads)
        free(pool->threads);
    pthread_mutex_destroy(&pool->mutex);
    pthread_cond_destroy(&pool->cond);
}

void fn(void* arg, int thread_n) {
    usleep(100000);
    printf("hoge  %d  %ld\n", thread_n, (long)arg);
    fflush(stdout);
    return;
}

int threadpool_test() {
    thread_pool pool;
    pool_init(&pool, 10, 100);
    pool_start(&pool);

    for (int i = 0; i < 1000; i++) {
        pool_submit(&pool, fn, (void*)(long)i);
    }

    pool_exit(&pool);
    pool_finalize(&pool);

    return 0;
}