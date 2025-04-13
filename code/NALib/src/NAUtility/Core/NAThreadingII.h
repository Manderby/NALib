
// This file contains inline implementations of the file NAThreading.h
// Do not include this file directly! It will automatically be included when
// including "NAThreading.h"



#include "../NAMemory.h"

#if NA_OS == NA_OS_WINDOWS
  #include <windows.h>
#elif NA_OS == NA_OS_MAC_OS_X
  #include <objc/objc.h>
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

NA_IDEF int naSleepN(size_t nanoSeconds) {
  return naSleepU(nanoSeconds / 1000);
}



NA_IDEF int naSleepU(size_t microSeconds) {
  #if NA_OS == NA_OS_WINDOWS
    Sleep((DWORD)(microSeconds / 1000));
    return 0;
  #elif NA_OS == NA_OS_MAC_OS_X
    return usleep((useconds_t)(microSeconds));
  #endif
}



NA_IDEF int naSleepM(size_t milliSeconds) {
  #if NA_OS == NA_OS_WINDOWS
    Sleep((DWORD)(milliSeconds));
    return 0;
  #elif NA_OS == NA_OS_MAC_OS_X
    return usleep((useconds_t)(milliSeconds * 1000LL));
  #endif
}



NA_IDEF int naSleepS(double seconds) {
  #if NA_OS == NA_OS_WINDOWS
    Sleep((DWORD)(seconds * 1000));
    return 0;
  #elif NA_OS == NA_OS_MAC_OS_X
    return usleep((useconds_t)(seconds * 1000000LL));
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

#if NA_OS == NA_OS_WINDOWS
  typedef HANDLE            NANativeThread;
#elif NA_OS == NA_OS_MAC_OS_X
  typedef dispatch_queue_t  NANativeThread;
#endif


NA_PROTOTYPE(NAThreadStruct);
struct NAThreadStruct{
  const char* name;
  #if NA_OS == NA_OS_MAC_OS_X
    dispatch_group_t dispatchGroup; // Needed to wait for threads.
  #endif
  NANativeThread nativeThread;  // If you experience an error here when working
  NAMutator function;           // with plain C files on a Mac: Turn off the
  void* arg;                    // automatic reference counting in project
};                              // settings.



NA_IDEF NAThread naMakeThread(const char* threadName, NAMutator function, void* arg) {
  NAThreadStruct* threadstruct = naAlloc(NAThreadStruct);
  threadstruct->name = threadName;
  #if NA_OS == NA_OS_WINDOWS
    threadstruct->nativeThread = NA_NULL; // Note that on windows, creating the thread would immediately start it.
  #else
    threadstruct->dispatchGroup = dispatch_group_create();
    threadstruct->nativeThread = dispatch_queue_create(threadName, DISPATCH_QUEUE_SERIAL);
  #endif
  threadstruct->function = function;
  threadstruct->arg = arg;
  return threadstruct;
}



NA_IDEF void naClearThread(NAThread thread) {
  NAThreadStruct* threadstruct = (NAThreadStruct*)thread;
  #if NA_OS == NA_OS_WINDOWS
    CloseHandle(threadstruct->nativeThread);
  #else
    #if NA_MACOS_USES_ARC
      // Thread will be released automatically when ARC is turned on.
    #else
      dispatch_release(threadstruct->nativeThread);
      dispatch_release(threadstruct->dispatchGroup);
    #endif
  #endif
  naFree(threadstruct);
}



#if NA_OS == NA_OS_WINDOWS
  // Windows has a different callback type. We need to call this function first
  // in order to call our true callback function.
  NA_HDEF static DWORD __stdcall na_RunWindowsThread(LPVOID arg) {
    NAThreadStruct* thread = (NAThreadStruct*)arg;
    thread->function(thread->arg);
    return 0;
  }
#endif



NA_IDEF void naRunThread(NAThread thread) {
  NAThreadStruct* threadstruct = (NAThreadStruct*)thread;
  #if NA_OS == NA_OS_WINDOWS
    threadstruct->nativeThread = CreateThread(NULL, 0, na_RunWindowsThread, threadstruct, 0, 0);
  #else
    dispatch_group_async_f(threadstruct->dispatchGroup, threadstruct->nativeThread, threadstruct->arg, threadstruct->function);
  #endif
}



NA_IDEF void naAwaitThread(NAThread thread) {
  NAThreadStruct* threadstruct = (NAThreadStruct*)thread;
  #if NA_OS == NA_OS_WINDOWS
    WaitForSingleObject(threadstruct->nativeThread, INFINITE);
  #else
    dispatch_group_wait(threadstruct->dispatchGroup, DISPATCH_TIME_FOREVER);
  #endif
}


// ////////////////////////////
// MUTEXES
// ////////////////////////////


#if NA_OS == NA_OS_WINDOWS

  // On windows, we need a more complex structure to really make a mutex being
  // lockable just once.
  NA_PROTOTYPE(NAWindowsMutex);

  struct NAWindowsMutex{
    #if(NA_WINDOWS_MUTEX_USE_CRITICAL_SECTION == 1)
      CRITICAL_SECTION mutex;
    #else
      HANDLE mutex;
    #endif
    NABool islockedbythisthread;
    #if NA_DEBUG
      NABool seemslocked;
    #endif
  };
  // Note that the islockedbythisthread will only be changed by the thread
  // which has locked the mutex and therefore will only evaluate to TRUE, when
  // the thread already has locked it. This is how we detect if the current
  // thread makes a double lock. The native "Threads" and "Critical sections"
  // of Windows do not do that. They allow mutliple locking as long as it is
  // the same thread. The author thinks that this is inconsistent and therefore
  // has implemented mutexes like this to be the same on all systems.

#else

  #if NA_DEBUG
    NA_PROTOTYPE(NAMacintoshMutex);
    struct NAMacintoshMutex{
      dispatch_semaphore_t mutex;
      NABool seemslocked;
    };
  #else
    typedef dispatch_semaphore_t NAMacintoshMutex;
  #endif


#endif



NA_IDEF NAMutex naMakeMutex(void) {
  #if NA_OS == NA_OS_WINDOWS
    NAWindowsMutex* windowsMutex = naAlloc(NAWindowsMutex);
    #if(NA_WINDOWS_MUTEX_USE_CRITICAL_SECTION == 1)
      InitializeCriticalSection(&windowsMutex->mutex);
    #else
      windowsMutex->mutex = CreateMutex(NULL, FALSE, NULL);
    #endif
    windowsMutex->islockedbythisthread = NA_FALSE;
    #if NA_DEBUG
      windowsMutex->seemslocked = NA_FALSE;
    #endif
    return windowsMutex;
  #else

    #if NA_DEBUG
      NAMacintoshMutex* macintoshmutex = naAlloc(NAMacintoshMutex);
      macintoshmutex->mutex = dispatch_semaphore_create(1);
      macintoshmutex->seemslocked = NA_FALSE;
      return macintoshmutex;
    #else
      NAMutex mutex = NA_COCOA_PTR_OBJC_TO_C(dispatch_semaphore_create(1));
      return mutex;
    #endif
  #endif
}



NA_IDEF void naClearMutex(NAMutex mutex) {
  #if NA_OS == NA_OS_WINDOWS
    NAWindowsMutex* windowsMutex = (NAWindowsMutex*)mutex;
    #if(NA_WINDOWS_MUTEX_USE_CRITICAL_SECTION == 1)
      DeleteCriticalSection(&windowsMutex->mutex);
    #else
      CloseHandle(windowsMutex->mutex);
    #endif
    naFree(windowsMutex);
  #else
    #if NA_DEBUG
      NAMacintoshMutex* macintoshmutex = (NAMacintoshMutex*)mutex;
      #if NA_MACOS_USES_ARC
      // Mutex will be released automatically when ARC is turned on.
      #else
        dispatch_release(macintoshmutex->mutex);
      #endif
      naFree(macintoshmutex);
    #else
      #if NA_MACOS_USES_ARC
        // Mutex will be released automatically when ARC is turned on.
        NA_UNUSED(mutex);
      #else
        dispatch_release(mutex);
      #endif
    #endif
  #endif
}



#if(NA_OS == NA_OS_WINDOWS) && (NA_WINDOWS_MUTEX_USE_CRITICAL_SECTION == 1)
  _Acquires_lock_(((NAWindowsMutex*)mutex)->mutex)
#endif
NA_IDEF void naLockMutex(NAMutex mutex) {
  #if NA_OS == NA_OS_WINDOWS
    NAWindowsMutex* windowsMutex = (NAWindowsMutex*)mutex;
    #if(NA_WINDOWS_MUTEX_USE_CRITICAL_SECTION == 1)
      EnterCriticalSection(&windowsMutex->mutex);
    #else
      WaitForSingleObject(windowsMutex->mutex, INFINITE);
    #endif
    #if NA_DEBUG
      if(windowsMutex->islockedbythisthread)
        naError("Mutex was already locked by this thread. This is not how Mutexes in NALib work.");
    #endif
    windowsMutex->islockedbythisthread = NA_TRUE;
    #if NA_DEBUG
      windowsMutex->seemslocked = NA_TRUE;
    #endif
   #else
    #if NA_DEBUG
      NAMacintoshMutex* macintoshmutex = (NAMacintoshMutex*)mutex;
      dispatch_semaphore_wait(macintoshmutex->mutex, DISPATCH_TIME_FOREVER);
      macintoshmutex->seemslocked = NA_TRUE;
    #else
      dispatch_semaphore_wait((NA_COCOA_BRIDGE dispatch_semaphore_t)mutex, DISPATCH_TIME_FOREVER);
    #endif
  #endif
}



#if(NA_OS == NA_OS_WINDOWS) && (NA_WINDOWS_MUTEX_USE_CRITICAL_SECTION == 1)
  _Releases_lock_(((NAWindowsMutex*)mutex)->mutex)
#endif
NA_IDEF void naUnlockMutex(NAMutex mutex) {
  #if NA_OS == NA_OS_WINDOWS
    NAWindowsMutex* windowsMutex = (NAWindowsMutex*)mutex;
    #if NA_DEBUG
      if(!naIsMutexLocked(mutex))
        naError("Mutex was not locked. Note: If this only happends once and very rarely, it might be because this check is unreliable!");
    #endif
    #if NA_DEBUG
      windowsMutex->seemslocked = NA_FALSE;
    #endif
    windowsMutex->islockedbythisthread = NA_FALSE;
    #if(NA_WINDOWS_MUTEX_USE_CRITICAL_SECTION == 1)
      LeaveCriticalSection(&windowsMutex->mutex);
    #else
      ReleaseMutex(windowsMutex->mutex);
    #endif
  #else
    #if NA_DEBUG
      NAMacintoshMutex* macintoshmutex = (NAMacintoshMutex*)mutex;
      macintoshmutex->seemslocked = NA_FALSE;
      dispatch_semaphore_signal(macintoshmutex->mutex);
    #else
      dispatch_semaphore_signal((NA_COCOA_BRIDGE dispatch_semaphore_t)mutex);
    #endif
  #endif
}



#if NA_DEBUG
  NA_IDEF NABool naIsMutexLocked(NAMutex mutex) {
    #if NA_OS == NA_OS_WINDOWS
      NAWindowsMutex* windowsMutex = (NAWindowsMutex*)mutex;
      return windowsMutex->seemslocked;
    #else
      NAMacintoshMutex* macintoshmutex = (NAMacintoshMutex*)mutex;
      return macintoshmutex->seemslocked;
    #endif

    // This was the old code which worked but it introduced long wait times
    // when debugging. As this function is unreliable anyway, the whole test
    // whether being locked or not has been put into a debug flag in a
    // special type only available when debugging.
    //
    //  NABool hasjustbeenlocked = naTryMutex(mutex);
    //  if(hasjustbeenlocked) {
    //    naUnlockMutex(mutex);
    //    return NA_FALSE;
    //  }else{
    //    return NA_TRUE;
    //  }
  }
#endif



#if(NA_OS == NA_OS_WINDOWS) && (NA_WINDOWS_MUTEX_USE_CRITICAL_SECTION == 1)
  _When_(return == NA_TRUE, _Acquires_lock_(((NAWindowsMutex*)mutex)->mutex))
#endif
NA_IDEF NABool naTryMutex(NAMutex mutex) {
  #if NA_OS == NA_OS_WINDOWS
    NAWindowsMutex* windowsMutex = (NAWindowsMutex*)mutex;
    #if(NA_WINDOWS_MUTEX_USE_CRITICAL_SECTION == 1)
      BOOL retValue = TryEnterCriticalSection(&windowsMutex->mutex);
      if(retValue == 0) {
        return NA_FALSE;
      }else{
        if(windowsMutex->islockedbythisthread) {
          LeaveCriticalSection(&windowsMutex->mutex);
          return NA_FALSE;
        }else{
          windowsMutex->islockedbythisthread = NA_TRUE;
          return NA_TRUE;
        }
      }
    #else
      DWORD retValue = WaitForSingleObject(windowsMutex->mutex, 0);
      if(retValue == WAIT_OBJECT_0) {
        return NA_FALSE;
      }else{
        // somehow, this does not work yet. use Critical section.
        if(windowsMutex->islockedbythisthread) {
          ReleaseMutex(windowsMutex->mutex);
          return NA_FALSE;
        }else{
          windowsMutex->islockedbythisthread = NA_TRUE;
          return NA_TRUE;
        }
      }
    #endif
  #else
    #if NA_DEBUG
      NAMacintoshMutex* macintoshmutex = (NAMacintoshMutex*)mutex;
      long retValue = dispatch_semaphore_wait(macintoshmutex->mutex, DISPATCH_TIME_NOW);
      if(retValue) {
        return NA_FALSE;
      }else{
        return NA_TRUE;
        macintoshmutex->seemslocked = NA_TRUE;
      }
    #else
      long retValue = dispatch_semaphore_wait((NA_COCOA_BRIDGE dispatch_semaphore_t)mutex, DISPATCH_TIME_NOW);
      return (retValue ? NA_FALSE : NA_TRUE);
    #endif
  #endif
}



// ////////////////////////////
// ALARMS
// ////////////////////////////


#if NA_OS == NA_OS_WINDOWS
  typedef HANDLE            NANativeAlarm;
#elif NA_OS == NA_OS_MAC_OS_X
  typedef dispatch_semaphore_t  NANativeAlarm;
#endif



NA_IDEF NAAlarm naMakeAlarm(void) {
  NANativeAlarm alarmer;
  #if NA_OS == NA_OS_WINDOWS
    alarmer = CreateEvent(NULL, FALSE, FALSE, NULL);
    return (NAAlarm)alarmer;
#else
    alarmer = dispatch_semaphore_create(0);
    return (NAAlarm)NA_COCOA_PTR_OBJC_TO_C(alarmer);
  #endif
}



NA_IDEF void naClearAlarm(NAAlarm alarmer) {
  #if NA_OS == NA_OS_WINDOWS
    CloseHandle(alarmer);
  #else
    #if NA_MACOS_USES_ARC
      NA_UNUSED(alarmer);
      // Alarm will be released automatically when ARC is turned on.
    #else
      dispatch_release(alarmer);
    #endif
  #endif
}



NA_IDEF NABool naAwaitAlarm(NAAlarm alarmer, double maxWaitTime) {
  #if NA_OS == NA_OS_WINDOWS
    DWORD result;
    #if NA_DEBUG
      if(maxWaitTime < 0.)
        naError("maxWaitTime should not be negative. Beware of the zero!");
    #endif
    ResetEvent(alarmer);
    if(maxWaitTime == 0) {
      result = WaitForSingleObject(alarmer, INFINITE);
    }else{
      result = WaitForSingleObject(alarmer, (DWORD)(1000. * maxWaitTime));
    }
    return (result == WAIT_OBJECT_0);
  #else
    long result;
    #if NA_DEBUG
      if(maxWaitTime < 0.)
        naError("maxWaitTime should not be negative. Beware of the zero!");
    #endif
    if(maxWaitTime == 0) {
      result = dispatch_semaphore_wait((NA_COCOA_BRIDGE dispatch_semaphore_t)alarmer, DISPATCH_TIME_FOREVER);
    }else{
      dispatch_time_t timeout = dispatch_time(DISPATCH_TIME_NOW, (int64_t)(1000000000 * maxWaitTime));
      result = dispatch_semaphore_wait((NA_COCOA_BRIDGE dispatch_semaphore_t)alarmer, timeout);
    }
    return (result ? NA_FALSE : NA_TRUE);
  #endif
}



NA_IDEF void naTriggerAlarm(NAAlarm alarmer) {
  #if NA_OS == NA_OS_WINDOWS
    SetEvent(alarmer);
  #else
    dispatch_semaphore_signal((NA_COCOA_BRIDGE dispatch_semaphore_t)alarmer);
  #endif
}



// This is free and unencumbered software released into the public domain.

// Anyone is free to copy, modify, publish, use, compile, sell, or
// distribute this software, either in source code form or as a compiled
// binary, for any purpose, commercial or non-commercial, and by any
// means.

// In jurisdictions that recognize copyright laws, the author or authors
// of this software dedicate any and all copyright interest in the
// software to the public domain. We make this dedication for the benefit
// of the public at large and to the detriment of our heirs and
// successors. We intend this dedication to be an overt act of
// relinquishment in perpetuity of all present and future rights to this
// software under copyright law.

// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
// EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
// MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
// IN NO EVENT SHALL THE AUTHORS BE LIABLE FOR ANY CLAIM, DAMAGES OR
// OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
// ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
// OTHER DEALINGS IN THE SOFTWARE.

// For more information, please refer to <http://unlicense.org/>
