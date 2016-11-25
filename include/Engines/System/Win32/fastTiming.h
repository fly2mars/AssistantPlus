#pragma once

#ifndef WIN32
#error This version of fasttiming is only valid on win32 platforms
#endif


#ifndef LONGLONG
typedef __int64 LONGLONG;
#endif

#include <time.h>   // For time()


/*\class suHiResElapsedTime
* High-resolution Simple elapsed time class
*/

class suHiResElapsedTime
{
public:
  suHiResElapsedTime ();
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

