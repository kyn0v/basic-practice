#ifndef MUTEX_H
#define MUTEX_H

#include <pthread.h>
#include <stdio.h>
#include <string.h>

#include <cstdlib>

class Mutex {
 public:
  Mutex();
  ~Mutex();
  void Lock();
  void Unlock();

 private:
  pthread_mutex_t mutex_;
  // No copying
  Mutex(const Mutex&);
  void operator=(const Mutex&);
};

#endif
