
#include "NASystem.h"

// Threading, Sleeping
//
// Note that for now, on Windows, the native threading functions of WINAPI are
// used. On Mac, Grand Central Dispatch is used. There are implementations
// planned for posix- as well as for C11-Threads. But so far, they are not
// implemented yet here.


// Threading works differently on many systems and many frameworks. The data
// structures used are also completely different. Therefore, all datatypes here
// are declared as void*. In the implementation file, these void* are casted
// to their explicit type.
typedef void* NAThread;
typedef void* NAMutex;
typedef void* NAWait;


// The function prototype for any threaded or otherwise called function.
typedef void (*NACallingFunction)(void*);



// ///////////////////////////////////
// Sleeping and timed calling of functions
//
// The accuracy of the timing depends on the system but should be at least in
// the reign of some milli-seconds.

// Puts the current thread on hold for the specified amount of time.
// S = Seconds, M = Milliseconds, U = Microseconds.
NA_IAPI int naSleepU(NAUInt usecs);
NA_IAPI int naSleepM(NAUInt msecs);
NA_IAPI int naSleepS(NAUInt  secs);
// Executes the given function in the given number of seconds with the given
// arg. Note that the timediff is given as a double.
NA_IAPI void naCallUIElementInSeconds(     NACallingFunction function,
                                                      void* uielement,
                                                     double timediff);


// ///////////////////////////////////
// Threads

// Creates a new thread with a start function and an argument.
// You can give a thread a name. This will be used on some systems when
// debugging. Note that neither threadname nor arg will be owned by the
// thread. You have to take care of deletion of any memory after naClearThread
// by yourself.
NA_IAPI NAThread naNewThread(         const char* threadname,
                                NACallingFunction function,
                                            void* arg);
// Clears all memory structures concerning the given thread. Will not clear
// the arg or threadname given to naNewThread. If the thread is still running,
// behaviour is undefined.
NA_IAPI void naClearThread(NAThread thread);
// Calling this function will execute the thread once.
NA_IAPI void naRunThread(NAThread thread);



// //////////////////////////////////
// Mutex

// Create and clear a mutex.
NA_IAPI NAMutex naNewMutex();
NA_IAPI void naClearMutex(NAMutex mutex);
// Locks and unlocks a mutex. The locking waits forever.
NA_IAPI void naLockMutex(NAMutex mutex);
NA_IAPI void naUnlockMutex(NAMutex mutex);
// Tries to lock the mutex but returns after the given timeout if not
// successful. Returns NA_TRUE if the lock was successful and NA_FALSE if the
// timeout occured or if there was any kind of error.
NA_IAPI NABool naTryMutex(NAMutex mutex, double timeout);
// Returns NA_TRUE if the mutex is locked or NA_FALSE otherwise.
NA_IAPI NABool naIsMutexLocked(NAMutex mutex);



// //////////////////////////////////
// Wait

// An NAWait is a structure for two threads. One goes to sleep using
// naStartWait and thus is locked for a certain time until another thread gives
// a wakeup call by calling naSignalWait. The waiting thread can also be woken
// up by a timeout. When a thread calls naSignalWait without another thread
// waiting, nothing happends.
//
// Creates and clears a wait structure.
NA_IAPI NAWait naNewWait();
NA_IAPI void naClearWait(NAWait wait);
// Locks the current thread until some other thread calls naSignalWait or
// until the timeout occurs.
// Returns NA_TRUE, if the wait was signaled or NA_FALSE if the timeout occured.
NA_IAPI NABool naStartWait(NAWait wait, double maxwaittime);
// Sends a signal to anyone waiting on that NAWait structure.
NA_IAPI void naSignalWait(NAWait wait);












// ///////////////////////////////////////////////////////////////////////
// Inline Implementations: See readme file for more expanation.
// ///////////////////////////////////////////////////////////////////////


// Note that traditinally, a thred only executes one function and thus in
// some implementations, creating a thread immediately runs the thread as well.
// To stay consistent and give a higher flexibility, in NALib, the function
// to call as well as the argument is stored in a structure and the thread
// will be created when needed. Also note that with this struct, the queues
// used in Grand Central Dispatch can easily be disguised as threads. This is
// of course a strong limitation to the power of timed queues but it is the
// only way of recreating someting like threads.
typedef struct NAThreadStruct NAThreadStruct;


#include "NAMemory.h"

#if NA_SYSTEM == NA_SYSTEM_WINDOWS
  #include <windows.h>
  struct NAThreadStruct{
    HANDLE thread;
    NACallingFunction function;
    void* arg;
  };
#elif NA_SYSTEM == NA_SYSTEM_MAC_OS_X
  #include <unistd.h>
  #include <dispatch/dispatch.h>
  struct NAThreadStruct{
    const char* name;
    dispatch_queue_t queue;
    NACallingFunction function;
    void* arg;
  };
  // Workaround for XCode 3 where the following macro is not defined:
  #ifndef DISPATCH_QUEUE_SERIAL
    #define DISPATCH_QUEUE_SERIAL NULL
  #endif
#endif



NA_IDEF int naSleepU(NAUInt usecs){
  #if NA_SYSTEM == NA_SYSTEM_WINDOWS
    Sleep((DWORD)(usecs/1000));
    return 0;
  #elif NA_SYSTEM == NA_SYSTEM_MAC_OS_X
    return usleep((useconds_t)(usecs));
  #endif
}



NA_IDEF int naSleepM(NAUInt msecs){
  #if NA_SYSTEM == NA_SYSTEM_WINDOWS
    Sleep((DWORD)(msecs));
    return 0;
  #elif NA_SYSTEM == NA_SYSTEM_MAC_OS_X
    return usleep((useconds_t)(msecs*1000LL));
  #endif
}



NA_IDEF int naSleepS(NAUInt secs){
  #if NA_SYSTEM == NA_SYSTEM_WINDOWS
    Sleep((DWORD)(secs*1000));
    return 0;
  #elif NA_SYSTEM == NA_SYSTEM_MAC_OS_X
    return usleep((useconds_t)(secs*1000000LL));
  #endif
}



// On windows, we need to reroute a timer function using a specific callback.
#if NA_SYSTEM == NA_SYSTEM_WINDOWS
  NA_HDEF static VOID CALLBACK naCallUIElementInSecondsCallback(HWND hwnd, UINT uMsg, UINT_PTR idEvent, DWORD dwTime){
    PostMessage(hwnd, WM_PAINT, (WPARAM)NA_NULL, (LPARAM)NA_NULL);
  }
#endif


NA_IDEF void naCallUIElementInSeconds(NACallingFunction function, void* nativeID, double timediff){
  #if NA_SYSTEM == NA_SYSTEM_WINDOWS
    UINT_PTR test;
    test = SetTimer(nativeID, (UINT_PTR)NA_NULL, (UINT)(1000 * timediff), naCallUIElementInSecondsCallback);
  #else
    dispatch_time_t nexttime = dispatch_time(DISPATCH_TIME_NOW, 1000000000 * timediff);
    dispatch_queue_t queue = dispatch_get_current_queue();
    dispatch_after_f(nexttime, queue, nativeID, function);
  #endif
}







NA_IDEF NAThread naNewThread(const char* threadname, NACallingFunction function, void* arg){
  #if NA_SYSTEM == NA_SYSTEM_WINDOWS
    NAThreadStruct* threadstruct = naAlloc(NAThreadStruct);
    threadstruct->function = function;
    threadstruct->arg = arg;
    threadstruct->thread = NA_NULL;
    return threadstruct;
  #else
    NAThreadStruct* threadstruct = naAlloc(NAThreadStruct);
    threadstruct->name = threadname;
    threadstruct->queue = dispatch_queue_create(threadname, DISPATCH_QUEUE_SERIAL);
    threadstruct->function = function;
    threadstruct->arg = arg;
    return threadstruct;
  #endif
}



NA_IDEF void naClearThread(NAThread thread){
  #if NA_SYSTEM == NA_SYSTEM_WINDOWS
    NAThreadStruct* threadstruct = (NAThreadStruct*)thread;
    naFree(threadstruct);
  #else
    NAThreadStruct* threadstruct = (NAThreadStruct*)thread;
    dispatch_release(threadstruct->queue);
    naFree(threadstruct);
  #endif
}


#if NA_SYSTEM == NA_SYSTEM_WINDOWS
  NA_HDEF static DWORD __stdcall naRunWindowsThread(LPVOID arg){
    NAThreadStruct* thread = (NAThreadStruct*)arg;
    thread->function(thread->arg);
    return 0;
  }
#endif


NA_IDEF void naRunThread(NAThread thread){
  #if NA_SYSTEM == NA_SYSTEM_WINDOWS
    NAThreadStruct* threadstruct = (NAThreadStruct*)thread;
    threadstruct->thread = CreateThread(NULL, 0, naRunWindowsThread, threadstruct, 0, 0);
  #else
    NAThreadStruct* threadstruct = (NAThreadStruct*)thread;
    dispatch_async_f(threadstruct->queue, threadstruct->arg, threadstruct->function);
  #endif
}





#if NA_SYSTEM == NA_SYSTEM_WINDOWS
  typedef struct NAWindowsMutex NAWindowsMutex;
  struct NAWindowsMutex{
//    CRITICAL_SECTION criticalsection;
    HANDLE mutex;
    NABool islockedbythisthread;
  };
#endif


NA_IDEF NAMutex naNewMutex(){
  #if NA_SYSTEM == NA_SYSTEM_WINDOWS
    NAWindowsMutex* windowsmutex = naAlloc(NAWindowsMutex);
    windowsmutex->mutex = CreateMutex(NULL, FALSE, NULL);
    //InitializeCriticalSection(&(windowsmutex->criticalsection));
    windowsmutex->islockedbythisthread = NA_FALSE;
    return windowsmutex;
  #else
    NAMutex mutex = dispatch_semaphore_create(1);
    return mutex;
  #endif
}



NA_IDEF void naClearMutex(NAMutex mutex){
  #if NA_SYSTEM == NA_SYSTEM_WINDOWS
    NAWindowsMutex* windowsmutex = (NAWindowsMutex*)mutex;
    CloseHandle(windowsmutex->mutex);
    //DeleteCriticalSection(&(windowsmutex->criticalsection));
    naFree(windowsmutex);
  #else
    dispatch_release(mutex);
  #endif
}




NA_IDEF void naLockMutex(NAMutex mutex){
  #if NA_SYSTEM == NA_SYSTEM_WINDOWS
    NAWindowsMutex* windowsmutex = (NAWindowsMutex*)mutex;
    WaitForSingleObject(windowsmutex->mutex, INFINITE);
    //EnterCriticalSection(&(windowsmutex->criticalsection));
    #ifndef NDEBUG
      if(windowsmutex->islockedbythisthread)
        naError("naLockMutex", "Mutex was already locked by this thread.");
    #endif
    windowsmutex->islockedbythisthread = NA_TRUE;
   #else
    dispatch_semaphore_wait(mutex, DISPATCH_TIME_FOREVER);
  #endif
}



NA_IDEF void naUnlockMutex(NAMutex mutex){
  #if NA_SYSTEM == NA_SYSTEM_WINDOWS
    NAWindowsMutex* windowsmutex = (NAWindowsMutex*)mutex;
    #ifndef NDEBUG
      if(!naIsMutexLocked(mutex))
        naError("naUnlockMutex", "Mutex was not locked locked by this thread.");
    #endif
    windowsmutex->islockedbythisthread = NA_FALSE;
    ReleaseMutex(windowsmutex->mutex);
    //LeaveCriticalSection(&(windowsmutex->criticalsection));
  #else
    dispatch_semaphore_signal(mutex);
  #endif
}



NA_IDEF NABool naTryMutex(NAMutex mutex, double timeout){
  #if NA_SYSTEM == NA_SYSTEM_WINDOWS
    NAWindowsMutex* windowsmutex = (NAWindowsMutex*)mutex;
    DWORD retvalue = WaitForSingleObject(mutex, (DWORD)(1000. * timeout));
    if(retvalue == WAIT_OBJECT_0){
      return NA_FALSE;
    }else{
      return !(windowsmutex->islockedbythisthread);
    }
    //BOOL retvalue = TryEnterCriticalSection(&(windowsmutex->criticalsection));
    //if(retvalue == 0){
    //  return NA_FALSE;
    //}else{
    //  return !(windowsmutex->islockedbythisthread);
    //}
  #else
    long retvalue = dispatch_semaphore_wait(mutex, dispatch_time(DISPATCH_TIME_NOW, 1000000000. * timeout));
    return (retvalue?NA_FALSE:NA_TRUE);
  #endif
}




NA_IDEF NABool naIsMutexLocked(NAMutex mutex){
  NABool hasjustbeenlocked = naTryMutex(mutex, 0);
  if(hasjustbeenlocked){
    naUnlockMutex(mutex);
    return NA_FALSE;
  }else{
    return NA_TRUE;
  }
}





NA_IDEF NAWait naNewWait(){
  #if NA_SYSTEM == NA_SYSTEM_WINDOWS
    HANDLE wait = CreateSemaphore(NULL, 0, 1, NULL);
    return wait;
  #else
    NAWait wait = dispatch_semaphore_create(0);
    return wait;
  #endif
}



NA_IDEF void naClearWait(NAWait wait){
  #if NA_SYSTEM == NA_SYSTEM_WINDOWS
    CloseHandle(wait);
  #else
    dispatch_release(wait);
  #endif
}



NA_IDEF NABool naStartWait(NAWait wait, double maxwaittime){
  #if NA_SYSTEM == NA_SYSTEM_WINDOWS
    DWORD result;
    if(maxwaittime == 0){
      result = WaitForSingleObject(wait, INFINITE);
    }else{
      result = WaitForSingleObject(wait, (DWORD)(1000. * maxwaittime));
    }
    if(result == WAIT_OBJECT_0){
      // The wait has been signaled. Reenable the wait.
      WaitForSingleObject(wait, 0);
      return NA_TRUE;
    }else{
      return NA_FALSE;
    }
  #else
    long result;
    if(maxwaittime == 0){
      result = dispatch_semaphore_wait(wait, DISPATCH_TIME_FOREVER);
    }else{
      dispatch_time_t timeout = dispatch_time(DISPATCH_TIME_NOW, 1000000000. * maxwaittime);
      result = dispatch_semaphore_wait(wait, timeout);
    }
    return(result?NA_FALSE:NA_TRUE);
  #endif
}




NA_IDEF void naSignalWait(NAWait wait){
  #if NA_SYSTEM == NA_SYSTEM_WINDOWS
    ReleaseSemaphore(wait, 1, NULL);
  #else
    dispatch_semaphore_signal(wait);
  #endif
}

