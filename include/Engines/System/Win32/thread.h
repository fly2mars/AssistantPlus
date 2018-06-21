#pragma once


#ifndef WIN32
#error This version of suThread is only valid on win32 platforms
#endif


// We want this to be compatible with MFC and non-MFC apps
#ifdef _AFXDLL
#include <afx.h>
#else
#include <windows.h>
#endif

#include <process.h>    // _beginthread(), _endthread()


#include <iostream>

/*\class suThread
\usage Just reload thread() function
<pre>
   class thread:public suThread
   {
     void thread()
     {
       while(true)
       {
         ...
       }
     }
   };    
</pre>

*/


class suThread
{
public:
  suThread  () : threadid_ (-1) {}
  ~suThread () {if (threadid_ != -1) stop();}

  int threadid () const { return threadid_;}

  bool start () 
  {
    threadid_ = (int)_beginthreadex (0, 0, thread_, this,
                                CREATE_SUSPENDED, (unsigned int*) &threadid_);
    if (threadid_ != 0)
      ResumeThread ((HANDLE)threadid_);
    return (threadid_ != 0);
  }
  // Start the thread running

  bool stop  () 
  {
    TerminateThread ((HANDLE) threadid_, -1);
    return true;
  }
  // Stop the thread. Ungraceful and may result in locking/resource problems.

  bool wait (unsigned int seconds = 0)
  {
    DWORD wait = seconds * 1000;
    if (wait == 0) wait = INFINITE;
    DWORD status = WaitForSingleObject ((HANDLE) threadid_, wait);
    return (status != WAIT_TIMEOUT);
  }
  // Wait for thread to complete

  void sleep (unsigned int msec) { Sleep (msec);}
  // Sleep for the specified amount of time.

protected:
  int threadid_;

  static unsigned int __stdcall thread_ (void* obj)
  {
    // Call the overriden thread function
    suThread* t = reinterpret_cast<suThread*>(obj);
    t->thread ();
    return 0;
  }

  virtual void thread () {
    _endthreadex (0);
    CloseHandle ((HANDLE) threadid_);
  }
  // Thread function, Override this in derived classes.
};



