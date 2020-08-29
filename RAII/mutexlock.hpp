#ifndef MUTEXLOCK_H
#define MUTEXLOCK_H

#include "mutex.h"

class MutexLock {
 public:
  explicit MutexLock(Mutex *mutex) : mutex_(mutex) { this->mutex_->Lock(); }
  ~MutexLock() { this->mutex_->Unlock(); }

 private:
  Mutex *const mutex_;
  // No copying allowed
  MutexLock(const MutexLock &);
  void operator=(const MutexLock &);
};

#endif