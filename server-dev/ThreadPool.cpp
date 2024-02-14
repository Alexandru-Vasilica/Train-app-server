//
// Created by alexv on 12/4/23.
//

#include "ThreadPool.h"

pthread_mutex_t*  ThreadPool::mlock= nullptr;
pthread_cond_t* ThreadPool::condition= nullptr;
CommandQueue* ThreadPool::commands= nullptr;

 void*ThreadPool::thread_action(void *arg) {
    int idx=*(int *)arg;
    pthread_detach(pthread_self());
    printf("Started thread %d\n",idx);
    while(true) {
        pthread_mutex_lock(mlock);
//        Command *command = commands->pop_command();
        if (commands->empty())
            pthread_cond_wait(condition,mlock);
        auto command=commands->pop_command();
        pthread_mutex_unlock(mlock);
        printf("Thread %d: Received command %d\n",idx,command->get_type());
        command->execute();
    }
}

ThreadPool::ThreadPool(size_t size) {
    this->size=size;

}

void ThreadPool::start() {
    int i;
    int* thread_indexes=new int(size);
    for( i=0;i<size;i++){
        thread_indexes[i]=i+1;
        pthread_t thread;
        pthread_create(&thread, nullptr,&ThreadPool::thread_action,(void*)&thread_indexes[i]);
    }
}



