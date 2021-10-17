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
    Thread_Pool_task_t * task;
    struct MissionQueue * next;
}Missionqueue;



typedef struct Thread_Pool{
    int thread_pool_shut_down; //线程池是否关闭了
    int thread_alive_num; //线程池存活的线程数
    int thread_position_num_max; //线程的最大容量

    Missionqueue *task_queue;//任务队列
    Missionqueue *task_queue_index; //任务队列当前的头
    Missionqueue *task_queue_for_free; //用于释放内存的指针

    pthread_t *threads; //线程池的线程
    pthread_t admin_id; //管理者线程的id

    int queue_position_max; //队列的最大容量
    int pool_empty; //线程池是否为空
    int poll_filled; //线程池是否为满
    int queue_index; //当前队列的长度
    int working_thread_num; //正在工作线程的数量
    int waiting_thread_num; //等待工作线程的数量

    pthread_mutex_t submit_lock; //提交任务的令牌
    pthread_mutex_t wait_for_lock; //等待唤醒的令牌
    pthread_mutex_t ops_waiting_thread_num; //操作等待线程数的令牌
    pthread_mutex_t ops_working_thread_num; //操作工作线程数的令牌
    pthread_mutex_t ops_queue_index; //操作队列当前数量的令牌
    pthread_cond_t empty_queue_wait; //空队列等待的条件

}Threadpool;
extern Threadpool * getInstacneThreadPool(int threadMax,int queueMax);
extern void Thread_pool_submit(Threadpool * threadPool,void *(*function)(void * arg),int argnum,...);
