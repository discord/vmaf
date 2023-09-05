#ifndef __PTHREAD_H
#define __PTHREAD_H

#include <windows.h>
#include <process.h>

typedef CRITICAL_SECTION pthread_mutex_t;
typedef CONDITION_VARIABLE pthread_cond_t;
typedef HANDLE pthread_t;

int pthread_mutex_init(pthread_mutex_t* mutex, const void* mustBeUnused);
int pthread_mutex_lock(pthread_mutex_t* mutex);
int pthread_mutex_unlock(pthread_mutex_t* mutex);
int pthread_mutex_destroy(pthread_mutex_t* mutex);
int pthread_cond_init(pthread_cond_t* cond, const void* mustBeUnused);
int pthread_cond_wait(pthread_cond_t* cond, pthread_mutex_t* mutex);
int pthread_cond_signal(pthread_cond_t* cond);
int pthread_cond_broadcast(pthread_cond_t* cond);
int pthread_cond_destroy(pthread_cond_t* cond);
int pthread_create(pthread_t* thread, void* mustBeUnused, void* (*startFn)(void*), void* arg);
int pthread_detach(pthread_t thread);

#endif
