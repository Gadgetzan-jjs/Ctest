//
// Created by mikasa on 2021/10/13.
//

#include "ThreadPool.h"
#include "stdlib.h"
#include "stdio.h"
#include "string.h"
extern Thread_Pool * getInstacneThreadPool(int threadMax,int queueMax){
    Thread_Pool * threadPool=(Thread_Pool *)malloc(sizeof(Thread_Pool));
    threadPool->thread_position_num_max=threadMax;
    threadPool->task_queue=(MissionQueue *)malloc(sizeof(MissionQueue));
    threadPool->task_queue_index=threadPool->task_queue;
    threadPool->task_queue->next=NULL;
    threadPool->thread_pool_shut_down=0;
    threadPool->thread_alive_num=0;
    threadPool->queue_position_max=queueMax;
    threadPool->pool_empty=1;
    threadPool->poll_filled=0;
    threadPool->queue_index=0;
    int threadssize=sizeof(pthread_t)*threadPool->thread_position_num_max;
    threadPool->threads=(pthread_t *)malloc(threadssize);
    if (threadPool->threads==NULL){
        perror("ERROR: malloc space to threads fail!\n");
        return NULL;
    }
    memset(threadPool->threads,0, threadssize);
//    threadPool->admin_id=pthread_create()
    if (pthread_mutex_init(&threadPool->submit_lock,NULL)!=0){
        perror("ERROR: init threadpool submit lock fail!\n");
        return NULL;
    }
    //2021-10-15 写线程的阻塞等待任务唤醒的代码



    return threadPool;
}
//任务进任务队列通知空闲线程来取
void Thread_pool_submit(Thread_Pool * threadPool,void *(*function) (void *),int argnum,...){
//    pthread_cond_broadcast()
    if (threadPool->poll_filled){
        perror("the threadpool is filled\n!");
        return;
    } else if (threadPool->thread_pool_shut_down){
        perror("the threadpool is shutdown!\n");
        return;
    }
    pthread_mutex_lock(&threadPool->submit_lock);
    if (threadPool->task_queue_index==NULL) {
        MissionQueue *mission = (MissionQueue *) malloc(sizeof(MissionQueue));
        mission->mission = (Thread_Pool_task_t *) malloc(sizeof(Thread_Pool_task_t));
        threadPool->task_queue_index->next = mission;
        mission->mission->function = function;
        va_start(mission->mission->arglist, argnum);
        mission->mission->argnum = argnum;
        mission->next = NULL;
        threadPool->task_queue_index = mission;
        threadPool->task_queue=mission;
        threadPool->task_queue_index=mission;
        pthread_mutex_unlock(&threadPool->submit_lock);
        threadPool->queue_index++;
        return;
    } else if (threadPool->queue_index>=threadPool->queue_position_max){
        perror("the queue is filled\n");
        pthread_mutex_unlock(&threadPool->submit_lock);
        return;
    }
    MissionQueue * mission=(MissionQueue *)malloc(sizeof (MissionQueue));
    mission->mission = (Thread_Pool_task_t *) malloc(sizeof(Thread_Pool_task_t));
    threadPool->task_queue_index->next = mission;
    mission->mission->function = function;
    va_start(mission->mission->arglist, argnum);
    mission->mission->argnum = argnum;
    mission->next = NULL;
    threadPool->task_queue_index->next=mission;
    threadPool->task_queue_index=mission;
    pthread_mutex_unlock(&threadPool->submit_lock);
    threadPool->queue_index++;
}
void Thread_Pool_do_work(){

}
