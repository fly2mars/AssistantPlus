// fasttiming.cpp
//


#ifndef WIN32
#error This version of fasttiming is only valid on win32 platforms
#endif

// We want this to be compatible with MFC and non-MFC apps
#ifdef _AFXDLL
#include <afx.h>
#else
#include <windows.h>
#endif


#include "win32/fastTiming.h"

// ************************
// *                      *
// *  suHiResElapsedTime  *
// *                      *
// ************************

suHiResElapsedTime::suHiResElapsedTime () : starting_ (0)
{
  reset ();
}


double suHiResElapsedTime::usec () const
{
  return sec() * 1000000.;
}

double suHiResElapsedTime::msec () const
{
  return sec() * 1000;
}

double suHiResElapsedTime::sec () const
{
  LARGE_INTEGER t, freq;
  QueryPerformanceCounter (&t);
  QueryPerformanceFrequency (&freq);

  return (double(t.QuadPart - starting_)) / freq.QuadPart;
}


void suHiResElapsedTime::reset ()
{
  LARGE_INTEGER t;
  QueryPerformanceCounter (&t);

  starting_ = t.QuadPart;
}
