//
// Created by mikasa on 2021/10/7.
//

#include "MyCThreadTest.h"
#include<pthread.h>
#include<unistd.h>
#include "iostream"
#include "stdio.h"
using namespace std;
void *sayHello(void *){
//    pthread_exit((void *)pthread_self());
    cout<<"hellodje"<<endl;
    pthread_t temp=pthread_self();
    printf("this is numof:%ld\n",temp);
    return (void * )0;
}
pthread_t pthread[30];
int pthreadsize=0;
void Test(){
    if (pthreadsize>30){
        return;
    }
    int Ret=pthread_create(&pthread[pthreadsize++],NULL,sayHello,NULL);
}
void displayPthreads(){
    for (int i = 0; i < 10; ++i) {
        if (pthread[i]!=NULL){
            printf("dis %ld\n",pthread[i]);
        }
    }
}