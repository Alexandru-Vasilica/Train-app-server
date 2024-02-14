//
// Created by alexv on 12/4/23.
//

#ifndef TEMA2_THREADPOOL_H
#define TEMA2_THREADPOOL_H

#include "CommandQueue.h"
#include <iostream>
#include <pthread.h>
#include <vector>

#define THREAD_POOL_SIZE 25

using namespace std;

//class Thread{
//private:
//    int idx;
//    pthread_mutex_t* mlock;
//    pthread_cond_t* condition;
//    CommandQueue* commands;
//    static void* thread_action(void* arg);
//    pthread_t thread;
//public:
//    Thread(int idx, pthread_mutex_t *mlock, pthread_cond_t *condition, CommandQueue *commands, pthread_t thread);
//};

class ThreadPool {
private:
    size_t size;

    static void* thread_action(void* arg);

public:
    static pthread_mutex_t* mlock;
    static pthread_cond_t* condition;
    static CommandQueue* commands;
    ThreadPool(size_t size);
    void start();
};


#endif //TEMA2_THREADPOOL_H
