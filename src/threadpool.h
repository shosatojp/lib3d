#include <pthread.h>
#include "array.h"

int usleep(int);

typedef void task_function(void*, int);

struct _task;
typedef struct _task task;

struct _task {
    void* arg;
    task_function* fn;

    task* prev;
    task* next;
};

typedef struct {
    task* task_head;
    task* task_tail;
    int task_count;
    int max_tasks;

    pthread_t* threads;
    int thread_count;

    pthread_mutex_t mutex;
    pthread_cond_t cond;

    bool exit;
} thread_pool;

typedef struct {
    thread_pool* pool;
    int thread_num;
} executor_info;

void pool_init(thread_pool* pool, int thread_count,int max_tasks);
void pool_push(thread_pool* pool, task_function* fn, void* arg);
void pool_start(thread_pool* pool);
void pool_exit(thread_pool* pool);
void pool_finalize(thread_pool* pool);
int threadpool_test();