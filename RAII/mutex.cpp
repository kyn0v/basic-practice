#include "mutex.h"

static void PthreadCall(const char* label, int result) {
  if (result != 0) {
    fprintf(stderr, "pthread %s: %s\n", label, strerror(result));
  }
}

Mutex::Mutex() { PthreadCall("init mutex", pthread_mutex_init(&mutex_, NULL)); }
Mutex::~Mutex() { PthreadCall("destroy mutex", pthread_mutex_destroy(&mutex_)); }
void Mutex::Lock() { PthreadCall("lock", pthread_mutex_lock(&mutex_)); }
void Mutex::Unlock() { PthreadCall("unlock", pthread_mutex_unlock(&mutex_)); }