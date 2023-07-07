#ifndef __PTHREAD_H
#define __PTHREAD_H

// TODO: these are very quick and dirty. Notably, _beginthreadex has troubling semantics for its return value

#include <windows.h>
#include <process.h>

// Mutexes
typedef CRITICAL_SECTION pthread_mutex_t;
#define pthread_mutex_init(a, b) 0; InitializeCriticalSection(a)
#define pthread_mutex_lock(a) EnterCriticalSection(a)
#define pthread_mutex_unlock(a) LeaveCriticalSection(a)
#define pthread_mutex_destroy(a) DeleteCriticalSection(a)

// Condition Variables
typedef CONDITION_VARIABLE pthread_cond_t;
#define pthread_cond_init(a, b) 0; InitializeConditionVariable(a)
#define pthread_cond_wait(a, b) SleepConditionVariableCS(a, b, INFINITE)
#define pthread_cond_signal(a) WakeConditionVariable(a)
#define pthread_cond_broadcast(a) WakeAllConditionVariable(a)
#define pthread_cond_destroy(a) /*no-op in windows*/

// Thread Creation
typedef HANDLE pthread_t;
#define pthread_create(a, b, c, d) *a = (HANDLE) _beginthreadex(NULL, 0, c, d, 0, NULL)
#define pthread_join(a, b) WaitForSingleObject(a, INFINITE)
#define pthread_detach(a) CloseHandle(a)

/*
typedef HANDLE pthread_t;
#define pthread_create(a, b, c, d) *a = 0
#define pthread_join(a, b) 
#define pthread_detach(a) 
*/

#endif
