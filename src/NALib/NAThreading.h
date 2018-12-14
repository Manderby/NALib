
// This file is part of NALib, a collection of C source code.
// Full license notice at the bottom.

#ifndef NA_THREADING_INCLUDED
#define NA_THREADING_INCLUDED
#ifdef __cplusplus
  extern "C"{
#endif

#include "NABase.h"

// Threading, Sleeping
//
// Note that in NALib, on Windows, the native threading functions of WINAPI are
// used. On Mac, Grand Central Dispatch (GCD) is used. In the future, there
// might be implementations planned for posix- as well as for C11-Threads. But
// so far, they are not implemented yet.

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
// S = Seconds, M = Milliseconds, U = Microseconds, N = Nanoseconds
// The return value behaves the same as the usleep function on Mac OS X, where
// a value of 0 denotes success. On Windows, the return value is always 0.
// Note that the S variant uses a double value.
NA_IAPI int naSleepN(NAUInt nsecs);
NA_IAPI int naSleepU(NAUInt usecs);
NA_IAPI int naSleepM(NAUInt msecs);
NA_IAPI int naSleepS(double secs);



// ///////////////////////////////////
// Threads
//
// Threads are simple to use parallel execution structures. You simply create
// a thread by giving him a function to execute and an argument to pass. Then,
// you start the thread and it will run parallel to your current execution.

// Creates a new thread with a start function and an argument.
// You can give a thread a name which will  be used on some systems when
// debugging later on. Note that both threadname and arg will NOT be owned by
// the thread.
NA_IAPI NAThread naMakeThread( const char* threadname,
                                   NAMutator function,
                                    void* arg);
// Clears all memory structures concerning the given thread. Will not clear
// the arg or threadname given to naMakeThread. If the thread is still running,
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
NA_IAPI NAMutex naMakeMutex(void);
NA_IAPI void naClearMutex(NAMutex mutex);
// Locks and unlocks a mutex. Waiting threads wait forever.
NA_IAPI void naLockMutex(NAMutex mutex);
NA_IAPI void naUnlockMutex(NAMutex mutex);
// Tries to lock the mutex but returns immediately even if not successful.
// Returns NA_TRUE if the lock was successful and NA_FALSE if it was not
// successful for any reason. Note that this implementation will return
// NA_FALSE even if it is the current thread which has locked a mutex. This
// is a difference to the behaviour of Mutexes in WINAPI but is now solved
// consistently along all systems.
NA_IAPI NABool naTryMutex(NAMutex mutex);

#ifndef NDEBUG
  // Returns NA_TRUE if the mutex is locked or NA_FALSE otherwise.
  // Beware!
  // This function is not guaranteed to return a meaningful result. While in
  // general, this function may return the actual state of the mutex
  // correctly, it may happen that during the time of returning the result
  // and using it, the state of the mutex might already have changed.
  // Do not use this function in release code! It is only intended for quick
  // checks.
  NA_IAPI NABool naIsMutexLocked(NAMutex mutex);
#endif


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
NA_IAPI NAAlarm naMakeAlarm(void);
NA_IAPI void naClearAlarm(NAAlarm alarm);
// Locks the current thread until some other thread calls naTriggerAlarm or
// until the timeout occurs. Returns NA_TRUE, if the alarm was triggered or
// NA_FALSE if the timeout occured. If maxwaittime is exactly 0, the function
// waits indefinitely. If maxwaittime is negative, an error is emitted when
// debugging. Make sure no accidental zero value is given!
NA_IAPI NABool naAwaitAlarm(NAAlarm alarm, double maxwaittime);
// Sends a signal to anyone waiting on that NAAlarm structure.
NA_IAPI void naTriggerAlarm(NAAlarm alarm);







// Inline implementations are in a separate file:
#include "NACore/NAThreadingII.h"



#ifdef __cplusplus
  } // extern "C"
#endif
#endif // NA_THREADING_INCLUDED


// Copyright (c) NALib, Tobias Stamm
//
// Permission is hereby granted, free of charge, to any person obtaining
// a copy of this software and associated documentation files (the
// "Software"), to deal in the Software without restriction, including
// without limitation the rights to use, copy, modify, merge, publish,
// distribute, sublicense, and/or sell copies of the Software, and to
// permit persons to whom the Software is furnished to do so, subject to
// the following conditions:
//
// The above copyright notice and this permission notice shall be included
// in all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
// EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
// MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
// IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
// CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
// TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
// SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
