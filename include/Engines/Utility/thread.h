#pragma once

// Decide which implementation to use
// Defining SU_NOTHREADS will use the default implementation in
// applications where threading isn't an issue.

#if !defined(SU_NOTHREADS) && defined(WIN32)
#include <Engines/System/Win32/thread.h>
#elif !defined(SU_NOTHREADS) && defined(HasPTHREADS)
#include <Engines/System/unix/thread.h>
#else


// **************
// *            *
// *  suThread  *
// *            *
// **************
//
// suThread is a simple wrapper around a threading object to
// start and execute a thread. This default version does nothing and
// is suitable for platforms that do not support threading, or
// applications that do not require it.

class suThread
{
public:
  suThread  ()     {}
  ~suThread ()     {}

  int threadid () const { return 0;}
  // Return thread id

  bool start () { return false;}
  // Start the thread running. return false on error

  bool stop  () { return false;}
  // Stop the thread. Ungraceful and may result in locking/resource problems.

  bool wait (unsigned int seconds = 0) { return false;}
  // Wait for thread to complete. Return true if thread is finished

  void sleep (unsigned int msec) {}
  // Sleep for the specified amount of time.

protected:
  virtual void thread () {}        // Thread function, never returns
};

#endif



