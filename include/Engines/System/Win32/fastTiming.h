#pragma once

#ifndef WIN32
#error This version of fasttiming is only valid on win32 platforms
#endif


#ifndef LONGLONG
typedef __int64 LONGLONG;
#endif

#include <time.h>   // For time()


/*\class suElapsedTime
* High-resolution Simple elapsed time class
*/

class suElapsedTime
{
public:
  suElapsedTime ();
  // Record the current time

  double usec () const;
  // Return elapsed time, in microseconds.

  double msec () const;
  // Return elapsed time, in milliseconds.

  double sec () const;
  // Return elapsed time, in seconds

  void reset ();
  // Reset the current time

private:
  LONGLONG starting_;     // Starting time
};

