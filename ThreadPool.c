//
// Created by mikasa on 2021/10/13.
//

#include "ThreadPool.h"
#include "stdlib.h"
#include "stdio.h"
#include "string.h"
#include <unistd.h>

void * action_thread(Threadpool * threadPool);
void * admin_work(Threadpool * threadPool);
void freeThreadPool(Threadpool * threadPool);
extern Threadpool * getInstacneThreadPool(int threadMax,int queueMax){
    Threadpool * threadPool=(Threadpool *)malloc(sizeof(Threadpool));
    threadPool->thread_position_num_max=threadMax;
    threadPool->task_queue=(Missionqueue *)malloc(sizeof(Missionqueue));
    threadPool->task_queue_index=threadPool->task_queue;
    threadPool->task_queue->next=NULL;
    threadPool->thread_pool_shut_down=0;
    threadPool->thread_alive_num=0;
    threadPool->queue_position_max=queueMax;
    threadPool->pool_empty=1;
    threadPool->poll_filled=0;
    threadPool->queue_index=0;
    threadPool->working_thread_num=0;
    threadPool->waiting_thread_num=0;
    pthread_create(&threadPool->admin_id,NULL,admin_work,threadPool);



    pthread_cond_init(&threadPool->empty_queue_wait,NULL);
    int threadssize=sizeof(pthread_t)*threadPool->thread_position_num_max;
    threadPool->threads=(pthread_t *)malloc(threadssize);
    if (threadPool->threads==NULL){
        perror("[ERROR]: malloc space to threads fail!\n");
        return NULL;
    }
    memset(threadPool->threads,0, threadssize);
//    threadPool->admin_id=pthread_create()
    if (pthread_mutex_init(&threadPool->submit_lock,NULL)!=0){
        perror("[ERROR]: init threadpool submit lock fail!\n");
        return NULL;
    }
    if (pthread_mutex_init(&threadPool->wait_for_lock,NULL)!=0){
        perror("[ERROR]: init wait for lock fail!\n");
        return NULL;
    }
    //2021-10-15 写线程的阻塞等待任务唤醒的代码
    for (int i = 0; i < threadMax; ++i) {
        pthread_create(&threadPool->threads[i], NULL, action_thread, threadPool);
    }


    return threadPool;
}
//任务进任务队列通知空闲线程来取
void Thread_pool_submit(Threadpool * threadPool,void *(*function) (void *),int argnum,...){
//    pthread_cond_broadcast()
    if (threadPool->poll_filled){
        perror("[ERROR]: the threadpool is filled\n!");
        return;
    } else if (threadPool->thread_pool_shut_down){
        perror("[ERROR]: the threadpool is shutdown!\n");
        return;
    }
    pthread_mutex_lock(&threadPool->submit_lock);
    if (threadPool->queue_index==0) {
        Missionqueue *mission = (Missionqueue *) malloc(sizeof(Missionqueue));
        mission->mission = (Thread_Pool_task_t *) malloc(sizeof(Thread_Pool_task_t));
        threadPool->task_queue_index->next = mission;
        mission->mission->function = function;
        va_start(mission->mission->arglist, argnum);
//        void * arg1=va_arg(threadPool->task_queue->mission->arglist,void *);
//        void * arg2=va_arg(threadPool->task_queue->mission->arglist,void *);
//        mission->mission->function((arg1,arg2));
        mission->mission->argnum = argnum;
        mission->next = NULL;
        threadPool->task_queue_index = mission;
        threadPool->task_queue=mission;
        threadPool->task_queue_index=mission;
        pthread_cond_signal(&threadPool->wait_for_lock);
        threadPool->queue_index++;
        pthread_mutex_unlock(&threadPool->submit_lock);
        return;
    } else if (threadPool->queue_index>=threadPool->queue_position_max){
        perror("[ERROR]: the queue is filled\n");
        pthread_mutex_unlock(&threadPool->submit_lock);
        return;
    }
    Missionqueue * mission=(Missionqueue *)malloc(sizeof (Missionqueue));
    mission->mission = (Thread_Pool_task_t *) malloc(sizeof(Thread_Pool_task_t));
    threadPool->task_queue_index->next = mission;
    mission->mission->function = function;
    va_start(mission->mission->arglist, argnum);
    mission->mission->argnum = argnum;
    mission->next = NULL;
    threadPool->task_queue_index->next=mission;
    threadPool->task_queue_index=mission;
    pthread_cond_signal(&threadPool->wait_for_lock);
    threadPool->queue_index++;
    pthread_mutex_unlock(&threadPool->submit_lock);
}
void * action_thread(Threadpool * threadPool){// 线程创建之后或处理完上一个任务之后来访问任务队列，如果任务队列为空或者线程池关闭则等待
    while (1){
        pthread_mutex_lock(&threadPool->wait_for_lock);

        while (threadPool->queue_index==0&&!threadPool->thread_pool_shut_down){
            printf("[INFO]: queue is empty and 0x%x thread is wait!\n",(unsigned int)pthread_self());
//            pthread_mutex_lock(&threadPool->waiting_thread_num);
//            threadPool->waiting_thread_num++;
//            pthread_mutex_unlock(&threadPool->waiting_thread_num);
            pthread_cond_wait(&threadPool->empty_queue_wait,&threadPool->wait_for_lock);
        }
        printf("[INFO]: 0x%x thread is wakeup!\n",(unsigned int)pthread_self());
//        pthread_mutex_lock(&threadPool->waiting_thread_num);
//        threadPool->waiting_thread_num--;
//        pthread_mutex_unlock(&threadPool->waiting_thread_num);

//        printf("task queue index is%d\n",threadPool->queue_index);
//        void * arg1=va_arg(threadPool->task_queue->mission->arglist,void *);
//        void * arg2=va_arg(threadPool->task_queue->mission->arglist,void *);
//
        if (threadPool->thread_pool_shut_down){
            perror("[ERROR]: the threadpool is shutdown!\n");
            return NULL;
        }
        pthread_mutex_lock(&threadPool->working_thread_num);
        threadPool->working_thread_num++;
        pthread_mutex_unlock(&threadPool->working_thread_num);
        threadPool->task_queue->mission->function(threadPool->task_queue->mission->arglist);
        threadPool->queue_index--;
        pthread_mutex_lock(&threadPool->working_thread_num);
        threadPool->working_thread_num--;
        pthread_mutex_unlock(&threadPool->working_thread_num);
        pthread_mutex_unlock(&threadPool->wait_for_lock);
    }
}
void * admin_work(Threadpool * threadpool){//写管理线程的功能
    while (1){
        if (threadpool->thread_pool_shut_down){
            freeThreadPool(threadpool);
            return NULL;
        }
        sleep(2);

    }
}
void freeThreadPool(Threadpool * threadpool){
    if (threadpool==NULL){
        return;
    }
    if (threadpool->threads!=NULL){
        free(threadpool->threads);
    }else if (threadpool->task_queue!=NULL){
        free(threadpool->task_queue);
    } else if (threadpool->task_queue_index!=NULL){
        free(threadpool->task_queue_index);
    }
    free(threadpool);
}
