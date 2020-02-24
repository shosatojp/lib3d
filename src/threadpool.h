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

    bool exit;
} thread_pool;

typedef struct {
    thread_pool* pool;
    int thread_num;
} executor_info;

void init_pool(thread_pool* pool, int thread_count,int max_tasks);
void add_task(thread_pool* pool, task_function* fn, void* arg);
void executor(executor_info* info);
void start_pool(thread_pool* pool);
void exit_pool(thread_pool* pool);
void finalize_pool(thread_pool* pool);
int threadpool_test();