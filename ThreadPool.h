//
// Created by mikasa on 2021/10/13.
//

#ifndef UNTITLED_THREADPOOL_H
#define UNTITLED_THREADPOOL_H
#endif //UNTITLED_THREADPOOL_H
#include "pthread.h"
#include "stdarg.h"
typedef struct {
    void *(*function)(void * args);
    va_list arglist;
    int argnum;
}Thread_Pool_task_t;

typedef struct MissionQueue{
    Thread_Pool_task_t * mission;
    struct MissionQueue * next;
};



typedef struct Thread_Pool{
    int thread_pool_shut_down;
    int thread_alive_num;
    int thread_position_num_max;

    MissionQueue *task_queue;//任务队列
    MissionQueue *task_queue_index;


    pthread_t *threads;
    pthread_t admin_id;

    int queue_position_max;
    int pool_empty;
    int poll_filled;
    int queue_index;

    pthread_mutex_t submit_lock;

};
extern Thread_Pool * getInstacneThreadPool(int threadMax,int queueMax);
extern void Thread_pool_submit(Thread_Pool * threadPool,void *(*function)(void * arg),int argnum,...);
