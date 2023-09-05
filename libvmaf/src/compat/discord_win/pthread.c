#include "pthread.h"

#include <stdlib.h>

typedef struct {
    void* (*startFn)(void*);
    void *args;
} ThreadState;

#define EINVAL 22

int pthread_mutex_init(pthread_mutex_t *mutex, const void* mustBeUnused) {
    if (mustBeUnused) {
        return EINVAL;
    }

    InitializeCriticalSection(mutex);
    return 0;
}

int pthread_mutex_lock(pthread_mutex_t* mutex) {
    EnterCriticalSection(mutex);
    return 0;
}

int pthread_mutex_unlock(pthread_mutex_t* mutex) {
    LeaveCriticalSection(mutex);
    return 0;
}

int pthread_mutex_destroy(pthread_mutex_t* mutex) {
    DeleteCriticalSection(mutex);
    return 0;
}

int pthread_cond_init(pthread_cond_t* cond, const void* mustBeUnused) {
    if (mustBeUnused) {
        return EINVAL;
    }

    InitializeConditionVariable(cond);
    return 0;
}

int pthread_cond_wait(pthread_cond_t* cond, pthread_mutex_t* mutex) {
    BOOL ret = SleepConditionVariableCS(cond, mutex, INFINITE);
    if (ret == 0) {
        // If the function succeeds, the return value is nonzero.
        return EINVAL;
    }

    return 0;
}

int pthread_cond_signal(pthread_cond_t* cond) {
    WakeConditionVariable(cond);
    return 0;
}

int pthread_cond_broadcast(pthread_cond_t* cond) {
    WakeAllConditionVariable(cond);
    return 0;
}

int pthread_cond_destroy(pthread_cond_t* cond) {
    (void)cond; // Not needed with *ConditionVariable API
    return 0;
}

static unsigned __stdcall begin_thread_to_pthread(void* in) {
    ThreadState* threadState = (ThreadState*)(in);

    threadState->startFn(threadState->args);

    free(threadState);
    return 0;
}

int pthread_create(pthread_t* thread, void* mustBeUnused, void* (*startFn)(void*), void* arg) {
    if (mustBeUnused || !thread) {
        return EINVAL;
    }

    ThreadState* threadState = (ThreadState*)(malloc(sizeof(ThreadState)));
    if (threadState == NULL) {
        return EINVAL;
    }

    threadState->args = arg;
    threadState->startFn = startFn;

    // Begin suspended to handle a case where _beginthreadex can execute the thread before returning
    uintptr_t ret = _beginthreadex(NULL, 0, begin_thread_to_pthread, threadState, CREATE_SUSPENDED, NULL);
    if (ret == 0) {
        // "On an error, _beginthreadex returns 0, and errno and _doserrno are set"
        free(threadState);
        return EINVAL;
    }
    HANDLE handle = (HANDLE)(ret);
    // If the return value of ResumeThread is 1, the specified thread was suspended but was restarted. 
    if (ResumeThread(handle) != 1) {
        // ... so, since the thread was started suspended, it should be started, and any other return value is a failure
        // TODO: intentionally leaking the malloc as the documentation of ResumeThread is ambiguous if the thread can still run in the presence of failures to ResumeThread
        return EINVAL;
    }
    *thread = handle;
    return 0;
}

int pthread_detach(pthread_t thread) {
    // Note that CloseHandle of a thread can race process destruction--you can end up with the thread exiting after main returns/when parts of the runtime have already been torn down, and crash.
    BOOL ret = CloseHandle(thread);
    if (ret == 0) {
        return EINVAL;
    }

    return 0;
}
