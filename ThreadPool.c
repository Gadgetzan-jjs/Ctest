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
    if (pthread_mutex_init(&threadPool->ops_working_thread_num,NULL)!=0){
        perror("[ERROR]: init ops working thread num fail\n");
        return NULL;
    }
    if (pthread_mutex_init(&threadPool->ops_waiting_thread_num,NULL)!=0){
        perror("[ERROR]: init ops waiting thread num fail\n");
        return NULL;
    }
    if (pthread_mutex_init(&threadPool->ops_queue_index,NULL)!=0){
        perror("[ERROR]: init ops queue index\n");
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
        Missionqueue *mission = (Missionqueue *) malloc(sizeof(Missionqueue));
        mission->task = (Thread_Pool_task_t *) malloc(sizeof(Thread_Pool_task_t));
        mission->task->function = function;
        va_start(mission->task->arglist, argnum);
        mission->task->argnum = argnum;
        mission->next = NULL;
        pthread_mutex_lock(&threadPool->ops_queue_index);
            if (threadPool->queue_index>=threadPool->queue_position_max){
            perror("[ERROR]: the queue is filled\n");
            pthread_mutex_unlock(&threadPool->submit_lock);
            return;
            }
            if (threadPool->queue_index==0) {
                threadPool->task_queue=mission;// 任务队列的头
                threadPool->task_queue_for_free=threadPool->task_queue;
                threadPool->task_queue_index = threadPool->task_queue; //队列指针当前所在位置
                threadPool->queue_index++;
            } else{
                threadPool->task_queue_index->next=mission; //当前队列指针的next指向新来的任务
                threadPool->task_queue_index=mission; //队列指针指向新来的任务，即指针后移
                threadPool->queue_index++;
            }
            pthread_cond_signal(&threadPool->wait_for_lock);
        pthread_mutex_unlock(&threadPool->ops_queue_index);
    pthread_mutex_unlock(&threadPool->submit_lock);
}
void * action_thread(Threadpool * threadPool){// 线程创建之后或处理完上一个任务之后来访问任务队列，如果任务队列为空或者线程池关闭则等待
//    int afterwork=0;
    while (1){
        pthread_mutex_lock(&threadPool->wait_for_lock);
        while (threadPool->queue_index==0&&!threadPool->thread_pool_shut_down){
            printf("[INFO]: queue is empty and 0x%x thread is wait!\n",(unsigned int)pthread_self());
            pthread_mutex_lock(&threadPool->ops_waiting_thread_num);
            threadPool->waiting_thread_num++;
            pthread_mutex_unlock(&threadPool->ops_waiting_thread_num);
            pthread_cond_wait(&threadPool->empty_queue_wait,&threadPool->wait_for_lock);

//            afterwork=0;
        }
//        if (afterwork){
//            printf("[INFO]: 0x%x thread is afterwork now wait!\n",(unsigned int)pthread_self());
//            afterwork=0;
//            pthread_mutex_lock(&threadPool->ops_waiting_thread_num);
//            threadPool->waiting_thread_num++;
//            pthread_mutex_unlock(&threadPool->ops_waiting_thread_num);
//            pthread_cond_wait(&threadPool->empty_queue_wait,&threadPool->wait_for_lock);
//        }
        printf("[INFO]: 0x%x thread is wakeup!\n",(unsigned int)pthread_self());
        pthread_mutex_lock(&threadPool->ops_waiting_thread_num);
        threadPool->waiting_thread_num--;
        pthread_mutex_unlock(&threadPool->ops_waiting_thread_num);

//        printf("task queue index is%d\n",threadPool->queue_index);
//        void * arg1=va_arg(threadPool->task_queue->task->arglist,void *);
//        void * arg2=va_arg(threadPool->task_queue->task->arglist,void *);
//
        if (threadPool->thread_pool_shut_down){
            perror("[ERROR]: the threadpool is shutdown!\n");
            pthread_exit(NULL);
        }
        pthread_mutex_lock(&threadPool->ops_working_thread_num);
        threadPool->working_thread_num++;
        pthread_mutex_unlock(&threadPool->ops_working_thread_num);
        pthread_mutex_lock(&threadPool->ops_queue_index);
        Thread_Pool_task_t * task=threadPool->task_queue->task;
        threadPool->task_queue=threadPool->task_queue->next;
        threadPool->queue_index--;
        pthread_mutex_unlock(&threadPool->ops_queue_index);

        task->function(task->arglist);

        pthread_mutex_unlock(&threadPool->wait_for_lock);
        pthread_mutex_lock(&threadPool->ops_working_thread_num);
        threadPool->working_thread_num--;
        pthread_mutex_unlock(&threadPool->ops_working_thread_num);
//        afterwork=1;
    }
    pthread_exit(NULL);
}
void * admin_work(Threadpool * threadpool){//写管理线程的功能
    while (1){


        printf("[INFO]: the admin is working\n");
        if (threadpool->thread_pool_shut_down){
            pthread_cond_broadcast(&threadpool->wait_for_lock);
            freeThreadPool(threadpool);
            return NULL;
        }
//        pthread_mutex_lock(&threadpool->ops_waiting_thread_num);
        printf("[INFO]: the waiting thread num is %d\n",threadpool->waiting_thread_num);
//        pthread_mutex_unlock(&threadpool->ops_waiting_thread_num);
//        pthread_mutex_lock(&threadpool->ops_working_thread_num);
        printf("[INFO]: the working thread num is %d\n",threadpool->working_thread_num);
//        pthread_mutex_unlock(&threadpool->ops_working_thread_num);
//        pthread_mutex_lock(&threadpool->ops_queue_index);
        printf("[INFO]: the queue length is %d\n",threadpool->queue_index);
//        pthread_mutex_unlock(&threadpool->ops_queue_index);
        sleep(2);
    }
}
void freeThreadPool(Threadpool * threadpool){
    if (threadpool==NULL){
        return;
    }
    if (threadpool->threads!=NULL) {
        free(threadpool->threads);
    }
    if (threadpool->task_queue_for_free!=NULL){
        while (threadpool->task_queue_for_free!=NULL){
            Missionqueue * temp=threadpool->task_queue_for_free->next;
            free(threadpool->task_queue_for_free);
            threadpool->task_queue_for_free=temp;
        }
    }
//    }else if (threadpool->task_queue!=NULL){
//        free(threadpool->task_queue);
//    } else if (threadpool->task_queue_index!=NULL){
//        free(threadpool->task_queue_index);
//    }
    free(threadpool);
}
