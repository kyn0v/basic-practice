#include <unistd.h>

#include <iostream>

#include "mutexlock.hpp"

#define NUM_THREADS 10

using namespace std;

int num = 0;
Mutex mutex;

void *count_NonRAII(void *args) {
  mutex.Lock();
  for (int i = 0; i < 10000; i++) {
    num++;
    cout << num << endl;
  }
  mutex.Unlock();
}

void *count_RAII(void *args) {
  MutexLock lock(&mutex);
  for (int i = 0; i < 10000; i++) {
    num++;
    cout << num << endl;
  }
}

int main() {
  int t;
  pthread_t thread[NUM_THREADS];

  for (t = 0; t < NUM_THREADS; t++) {
    int ret = pthread_create(&thread[t], NULL, count_RAII, NULL);
    if (ret) {
      return -1;
    }
  }

  for (t = 0; t < NUM_THREADS; t++) {
    pthread_join(thread[t], NULL);
  }
  std::cout << num << std::endl;
  return 0;
}

/*
RAII四步骤：
  1.设计一个类封装资源
  2.在构造函数中初始化
  3.在析沟函数中执行销毁操作
  4.使用时声明一个该对象的类
  
ref: zhuanlan.zhihu.com/p/34660259
*/