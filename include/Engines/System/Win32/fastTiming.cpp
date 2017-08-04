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


// ************************
// *                      *
// *  suElapsedTime  *
// *                      *
// ************************

suElapsedTime::suElapsedTime () : starting_ (0)
{
  reset ();
}


double suElapsedTime::usec () const
{
  return sec() * 1000000.;
}

double suElapsedTime::msec () const
{
  return sec() * 1000;
}

double suElapsedTime::sec () const
{
  LARGE_INTEGER t, freq;
  QueryPerformanceCounter (&t);
  QueryPerformanceFrequency (&freq);

  return (double(t.QuadPart - starting_)) / freq.QuadPart;
}


void suElapsedTime::reset ()
{
  LARGE_INTEGER t;
  QueryPerformanceCounter (&t);

  starting_ = t.QuadPart;
}
