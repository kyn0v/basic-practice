#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int x = 0, threshold = 5;
pthread_mutex_t mut = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;

void *thread_func(void *args) {
    pthread_mutex_lock(&mut);
    while (x < threshold) {  // 此处while是针对“惊群效应”，保证只有一个线程进入临界区(线程的虚假唤醒)
        pthread_cond_wait(&cond, &mut);
    }
    printf("x = %d.\n", x);
    x = 0;
    printf("x reaches threshold, and reset to %d.\n", x);
    pthread_mutex_unlock(&mut);
}

int main() {
    pthread_t mythread;
    if (pthread_create(&mythread, NULL, thread_func, NULL)) {
        printf("error creating thread");
        abort();
    }
    for (int i = 0; i < threshold * 2; i++) {
        pthread_mutex_lock(&mut);
        x++;
        if (x == threshold) {
            pthread_cond_signal(&cond);
            // pthread_cond_broadcast(&cond);
        }
        pthread_mutex_unlock(&mut);
        sleep(1);  //note: 此处有趣的现象
    }
    if (pthread_join(mythread, NULL)) {
        printf("error joining thread.");
        abort();
    }
    printf("final x = %d\n", x);
    exit(EXIT_SUCCESS);
}