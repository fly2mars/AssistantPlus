#pragma once

// \file timing.h
//
#if !defined(SU_NOTHREADS) && defined(WIN32)
#include <Engines/System/Win32/fastTiming.h>
#elif defined(__linux__)
#include <Engines/System/unix/fastTiming.h>
#else

/*\class suElapsedTime
\biref Elapsed time computation.
*/

class suElapsedTime
{
public:
  suElapsedTime ();
  // Record the current time

  double msec () const { return sec() * 1000;}
  double sec  () const;
  // Return elapsed time, in milliseconds or sec

  void reset ();
  // Reset the current time

private:
  clock_t starting_;  // Starting time
};

#endif

