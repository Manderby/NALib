
#include "NASystem.h"

// Threading, Sleeping
//
// Note that in NALib, on Windows, the native threading functions of WINAPI are
// used. On Mac, Grand Central Dispatch (GCD) is used. In the future, there
// might be implementations planned for posix- as well as for C11-Threads. But
// so far, they are not implemented yet.

//
// todo: Some functions start with New, but they actually use malloc. It works
// But needs to be carefully thought trough again.
//

// Threading works differently on many systems and many frameworks. The data
// structures used are also completely different. Therefore, all datatypes here
// are declared as void*.
typedef void* NAThread;
typedef void* NAMutex;
typedef void* NAAlarm;



// ///////////////////////////////////
// Sleeping
//
// The accuracy of the timing depends on the system but should be at least in
// the reign of some milli-seconds.

// Puts the current thread on hold for the specified amount of time.
// S = Seconds, M = Milliseconds, U = Microseconds.
// The return value behaves the same as the usleep function on Mac OS X, where
// a value of 0 denotes success. On Windows, the return value is always 0.
NA_IAPI int naSleepU(NAUInt usecs);
NA_IAPI int naSleepM(NAUInt msecs);
NA_IAPI int naSleepS(NAUInt  secs);



// ///////////////////////////////////
// Threads
//
// Threads are simple to use parallel execution structures. You simply create
// a thread by giving him a function to execute and an argument to pass. Then,
// you start the thread and it will run parallel to your current execution.

// Creates a new thread with a start function and an argument.
// You can give a thread a name, which is currently unused. todo. This will
// be used on some systems when debugging later on. Note that both threadname
// and arg will NOT be owned by the thread.
NA_IAPI NAThread naNewThread( const char* threadname,
                                   NAFunc function,
                                    void* arg);
// Clears all memory structures concerning the given thread. Will not clear
// the arg or threadname given to naNewThread. If the thread is still running,
// behaviour is undefined.
NA_IAPI void naClearThread(NAThread thread);
// Calling this function will execute the thread once.
NA_IAPI void naRunThread(NAThread thread);



// //////////////////////////////////
// Mutex
//
// A mutex is a simple structure which is open for precisely one thread only.
// As long as one thread has a mutex locked, any other thread is locked out and
// waits without doing anything. Threads which are locked out wait until the
// mutex is unlocked again, giving one of the waiting threads the opportunity
// to lock the mutex.

// Create and clear a mutex.
NA_IAPI NAMutex naNewMutex();
NA_IAPI void naClearMutex(NAMutex mutex);
// Locks and unlocks a mutex. Waiting threads wait forever.
NA_IAPI void naLockMutex(NAMutex mutex);
NA_IAPI void naUnlockMutex(NAMutex mutex);
// Returns NA_TRUE if the mutex is locked or NA_FALSE otherwise.
NA_IAPI NABool naIsMutexLocked(NAMutex mutex);
// Tries to lock the mutex but returns immediately even if not successful.
// Returns NA_TRUE if the lock was successful and NA_FALSE if it was not
// successful for any reason. Note that this implementation will return
// NA_FALSE even if it is the current thread which has locked a mutex. This
// is a difference to the behaviour of Mutexes in WINAPI but is now solved
// consistently along all systems.
NA_IAPI NABool naTryMutex(NAMutex mutex);



// //////////////////////////////////
// Alarm
//
// An alarm is a quick notifying structure for at least two threads. One
// thread goes to sleep using naAwaitAlarm and thus is locked for a certain
// time until another thread gives a wakeup call by calling naTriggerAlarm.
// The waiting thread can also be woken up by a timeout.
//
// Note that when a thread calls naTriggerAlarm without another thread
// awaiting it, nothing happends. If shortly afterwards, a thread starts
// awaiting the alarm, he will not detect such triggers but instead start
// to sleep until the timeout or yet another trigger occurs. If you need
// all triggers to be detected, you should consider using a reference
// counter in combination with a mutex.
//
// Creates and clears an alarm structure.
NA_IAPI NAAlarm naNewAlarm();
NA_IAPI void naClearAlarm(NAAlarm alarm);
// Locks the current thread until some other thread calls naTriggerAlarm or
// until the timeout occurs. Returns NA_TRUE, if the alarm was triggered or
// NA_FALSE if the timeout occured. If maxwaittime is exactly 0, the function
// waits indefinitely. If maxwaittime is negative, an error is emitted when
// debugging. Make sure no accidental zero value is given!
NA_IAPI NABool naAwaitAlarm(NAAlarm alarm, double maxwaittime);
// Sends a signal to anyone waiting on that NAAlarm structure.
NA_IAPI void naTriggerAlarm(NAAlarm alarm);












// ///////////////////////////////////////////////////////////////////////
// Inline Implementations: See readme file for more expanation.
// ///////////////////////////////////////////////////////////////////////




#include "NAMemory.h"

#if NA_SYSTEM == NA_SYSTEM_WINDOWS
  #include <windows.h>
#elif NA_SYSTEM == NA_SYSTEM_MAC_OS_X
  #include <unistd.h>
  #include <dispatch/dispatch.h>
  // Workaround for XCode 3 where the following macro is not defined:
  #ifndef DISPATCH_QUEUE_SERIAL
    #define DISPATCH_QUEUE_SERIAL NULL
  #endif
#endif






// ////////////////////////////
// SLEEPING
// ////////////////////////////

NA_IDEF int naSleepU(NAUInt usecs){
  #if NA_SYSTEM == NA_SYSTEM_WINDOWS
    Sleep((DWORD)(usecs / 1000));
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
    return usleep((useconds_t)(msecs * 1000LL));
  #endif
}



NA_IDEF int naSleepS(NAUInt secs){
  #if NA_SYSTEM == NA_SYSTEM_WINDOWS
    Sleep((DWORD)(secs * 1000));
    return 0;
  #elif NA_SYSTEM == NA_SYSTEM_MAC_OS_X
    return usleep((useconds_t)(secs * 1000000LL));
  #endif
}








// ////////////////////////////
// THREADS
// ////////////////////////////

// Note that traditinally, a thred only executes one function and thus in
// some implementations, creating a thread immediately runs the thread as well.
// To stay consistent and give a higher flexibility, in NALib, the function
// to call as well as the argument is stored in a structure and the thread
// will be created when needed. Also note that with this struct, the queues
// used in Grand Central Dispatch can easily be disguised as threads. This is
// of course a strong limitation to the power of timed queues but it is the
// only way of recreating someting like threads.

#if NA_SYSTEM == NA_SYSTEM_WINDOWS
  typedef HANDLE            NANativeThread;
#elif NA_SYSTEM == NA_SYSTEM_MAC_OS_X
  typedef dispatch_queue_t  NANativeThread;
#endif


typedef struct NAThreadStruct NAThreadStruct;
struct NAThreadStruct{
  const char* name;
  NANativeThread nativeThread;
  NAFunc function;
  void* arg;
};



NA_IDEF NAThread naNewThread(const char* threadname, NAFunc function, void* arg){
  NAThreadStruct* threadstruct = naAlloc(NAThreadStruct);
  threadstruct->name = threadname;  // todo
  #if NA_SYSTEM == NA_SYSTEM_WINDOWS
    threadstruct->nativeThread = NA_NULL; // Note that on windows, creating the thread would immediately start it.
  #else
    threadstruct->nativeThread = dispatch_queue_create(threadname, DISPATCH_QUEUE_SERIAL);
  #endif
  threadstruct->function = function;
  threadstruct->arg = arg;
  return threadstruct;
}



NA_IDEF void naClearThread(NAThread thread){
  NAThreadStruct* threadstruct = (NAThreadStruct*)thread;
  #if NA_SYSTEM == NA_SYSTEM_WINDOWS
    CloseHandle(threadstruct->nativeThread);
  #else
    dispatch_release(threadstruct->nativeThread);
  #endif
  naFree(threadstruct);
}



#if NA_SYSTEM == NA_SYSTEM_WINDOWS
  // Windows has a different callback type. We need to call this function first
  // in order to call our true callback function.
  NA_HDEF static DWORD __stdcall naRunWindowsThread(LPVOID arg){
    NAThreadStruct* thread = (NAThreadStruct*)arg;
    thread->function(thread->arg);
    return 0;
  }
#endif



NA_IDEF void naRunThread(NAThread thread){
  NAThreadStruct* threadstruct = (NAThreadStruct*)thread;
  #if NA_SYSTEM == NA_SYSTEM_WINDOWS
    threadstruct->nativeThread = CreateThread(NULL, 0, naRunWindowsThread, threadstruct, 0, 0);
  #else
    dispatch_async_f(threadstruct->nativeThread, threadstruct->arg, threadstruct->function);
  #endif
}







// ////////////////////////////
// MUTEXES
// ////////////////////////////


#if NA_SYSTEM == NA_SYSTEM_WINDOWS

  // There are two ways to implement mutexes on windows: "Critical section" or
  // "Mutex". But as up to now, the author had some problems with the "Mutex"
  // implementation, we just use the "Critical section" which is faster anyway.
  // todo.
  #define NA_THREAD_WINDOWS_USE_CRITICAL_SECTION

  // On windows, we need a more complex structure to really make a mutex being
  // lockable just once.
  typedef struct NAWindowsMutex NAWindowsMutex;

  struct NAWindowsMutex{
    #ifdef NA_THREAD_WINDOWS_USE_CRITICAL_SECTION
      CRITICAL_SECTION criticalsection;
    #else
      HANDLE mutex;
    #endif
    NABool islockedbythisthread;
  };
  // Note that the islockedbythisthread will only be changed by the thread
  // which has locked the mutex and therefore will only evaluate to TRUE, when
  // the thread already has locked it. This is how we detect if the current
  // thread makes a double lock. The native "Threads" and "Critical sections"
  // of Windows do not do that. They allow mutliple locking as long as it is
  // the same thread. The author thinks that this is inconsistent and therefore
  // has implemented mutexes like this to be the same on all systems.

#endif



NA_IDEF NAMutex naNewMutex(){
  #if NA_SYSTEM == NA_SYSTEM_WINDOWS
    NAWindowsMutex* windowsmutex = naAlloc(NAWindowsMutex);
    #ifdef NA_THREAD_WINDOWS_USE_CRITICAL_SECTION
      InitializeCriticalSection(&(windowsmutex->criticalsection));
    #else
      windowsmutex->mutex = CreateMutex(NULL, FALSE, NULL);
    #endif
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
    #ifdef NA_THREAD_WINDOWS_USE_CRITICAL_SECTION
      DeleteCriticalSection(&(windowsmutex->criticalsection));
    #else
      CloseHandle(windowsmutex->mutex);
    #endif
    naFree(windowsmutex);
  #else
    dispatch_release(mutex);
  #endif
}




NA_IDEF void naLockMutex(NAMutex mutex){
  #if NA_SYSTEM == NA_SYSTEM_WINDOWS
    NAWindowsMutex* windowsmutex = (NAWindowsMutex*)mutex;
    #ifdef NA_THREAD_WINDOWS_USE_CRITICAL_SECTION
      EnterCriticalSection(&(windowsmutex->criticalsection));
    #else
      WaitForSingleObject(windowsmutex->mutex, INFINITE);
    #endif
    #ifndef NDEBUG
      if(windowsmutex->islockedbythisthread)
        naError("naLockMutex", "Mutex was already locked by this thread. This is not how Mutexes in NALib work.");
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
        naError("naUnlockMutex", "Mutex was not locked locked.");
    #endif
    windowsmutex->islockedbythisthread = NA_FALSE;
    #ifdef NA_THREAD_WINDOWS_USE_CRITICAL_SECTION
      LeaveCriticalSection(&(windowsmutex->criticalsection));
    #else
      ReleaseMutex(windowsmutex->mutex);
    #endif
  #else
    dispatch_semaphore_signal(mutex);
  #endif
}



NA_IDEF NABool naIsMutexLocked(NAMutex mutex){
  NABool hasjustbeenlocked = naTryMutex(mutex);
  if(hasjustbeenlocked){
    naUnlockMutex(mutex);
    return NA_FALSE;
  }else{
    return NA_TRUE;
  }
}



NA_IDEF NABool naTryMutex(NAMutex mutex){
  #if NA_SYSTEM == NA_SYSTEM_WINDOWS
    NAWindowsMutex* windowsmutex = (NAWindowsMutex*)mutex;
    #ifdef NA_THREAD_WINDOWS_USE_CRITICAL_SECTION
      BOOL retvalue = TryEnterCriticalSection(&(windowsmutex->criticalsection));
      if(retvalue == 0){
        return NA_FALSE;
      }else{
        if(windowsmutex->islockedbythisthread){
          LeaveCriticalSection(&(windowsmutex->criticalsection));
          return NA_FALSE;
        }else{
          windowsmutex->islockedbythisthread = NA_TRUE;
          return NA_TRUE;
        }
      }
    #else
      DWORD retvalue = WaitForSingleObject(windowsmutex->mutex, 0);
      if(retvalue == WAIT_OBJECT_0){
        return NA_FALSE;
      }else{
        // somehow, this does not work yet. use Critical section.
        if(windowsmutex->islockedbythisthread){
          ReleaseMutex(windowsmutex->mutex);
          return NA_FALSE;
        }else{
          windowsmutex->islockedbythisthread = NA_TRUE;
          return NA_TRUE;
        }
      }
    #endif
  #else
    long retvalue = dispatch_semaphore_wait(mutex, DISPATCH_TIME_NOW);
    return (retvalue ? NA_FALSE : NA_TRUE);
  #endif
}







// ////////////////////////////
// ALARMS
// ////////////////////////////


#if NA_SYSTEM == NA_SYSTEM_WINDOWS
  typedef HANDLE            NANativeAlarm;
#elif NA_SYSTEM == NA_SYSTEM_MAC_OS_X
  typedef dispatch_semaphore_t  NANativeAlarm;
#endif



NA_IDEF NAAlarm naNewAlarm(){
  NANativeAlarm alarm;
  #if NA_SYSTEM == NA_SYSTEM_WINDOWS
    alarm = CreateEvent(NULL, FALSE, FALSE, NULL);
  #else
    alarm = dispatch_semaphore_create(0);
  #endif
  return alarm;
}



NA_IDEF void naClearAlarm(NAAlarm alarm){
  #if NA_SYSTEM == NA_SYSTEM_WINDOWS
    CloseHandle(alarm);
  #else
    dispatch_release(alarm);
  #endif
}



NA_IDEF NABool naAwaitAlarm(NAAlarm alarm, double maxwaittime){
  #if NA_SYSTEM == NA_SYSTEM_WINDOWS
    DWORD result;
    #ifndef NDEBUG
      if(maxwaittime < 0.)
        naError("naAwaitAlarm", "maxwaittime should not be negative. Beware of the zero!");
    #endif
    ResetEvent(alarm);
    if(maxwaittime == 0){
      result = WaitForSingleObject(alarm, INFINITE);
    }else{
      result = WaitForSingleObject(alarm, (DWORD)(1000. * maxwaittime));
    }
    return (result == WAIT_OBJECT_0);
  #else
    long result;
    #ifndef NDEBUG
      if(maxwaittime < 0.)
        naError("naAwaitAlarm", "maxwaittime should not be negative. Beware of the zero!");
    #endif
    if(maxwaittime == 0){
      result = dispatch_semaphore_wait(alarm, DISPATCH_TIME_FOREVER);
    }else{
      dispatch_time_t timeout = dispatch_time(DISPATCH_TIME_NOW, 1000000000. * maxwaittime);
      result = dispatch_semaphore_wait(alarm, timeout);
    }
    return (result ? NA_FALSE : NA_TRUE);
  #endif
}




NA_IDEF void naTriggerAlarm(NAAlarm alarm){
  #if NA_SYSTEM == NA_SYSTEM_WINDOWS
    SetEvent(alarm);
  #else
    dispatch_semaphore_signal(alarm);
  #endif
}

