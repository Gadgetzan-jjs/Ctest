//
// Created by mikasa on 2021/10/13.
//

#ifndef UNTITLED_THREADPOOL_H
#define UNTITLED_THREADPOOL_H
#endif //UNTITLED_THREADPOOL_H
#include "pthread.h"
#include "stdarg.h"
typedef struct {
    void *(*function)(void * arg,...);
    va_list arglist;
    int argnum;
    void * args[10];
    void * arg;

}Thread_Pool_task_t;

typedef struct MissionQueue{
    Thread_Pool_task_t * mission;
    struct MissionQueue * next;
}Missionqueue;



typedef struct Thread_Pool{
    int thread_pool_shut_down;
    int thread_alive_num;
    int thread_position_num_max;

    Missionqueue *task_queue;//任务队列
    Missionqueue *task_queue_index;


    pthread_t *threads;
    pthread_t admin_id;

    int queue_position_max;
    int pool_empty;
    int poll_filled;
    int queue_index;
    int working_thread_num;
    int waiting_thread_num;

    pthread_mutex_t submit_lock;
    pthread_mutex_t wait_for_lock;
    pthread_mutex_t ops_waiting_thread_num;
    pthread_mutex_t ops_working_thread_num;
    pthread_cond_t empty_queue_wait;
}Threadpool;
extern Threadpool * getInstacneThreadPool(int threadMax,int queueMax);
extern void Thread_pool_submit(Threadpool * threadPool,void *(*function)(void * arg),int argnum,...);
