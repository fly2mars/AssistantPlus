#pragma once

// lock.h
//
// Copyright (c) 2005 Yao Yuan
//
// Permission to use, copy, modify, distribute, and sell this software and
// its documentation for any purpose is hereby granted without fee, provided
// that (i) the above copyright notices and this permission notice appear in
// all copies of the software and related documentation, and (ii) the names
// of Philip Romanik and Amy Muntz may not be used in any advertising or
// publicity relating to the software without the specific, prior written
// permission of Philip Romanik and Amy Muntz.
//
// Use of this software and/or its documentation will be deemed to be
// acceptance of these terms.
//
// THE SOFTWARE IS PROVIDED "AS-IS" AND WITHOUT WARRANTY OF ANY KIND,
// EXPRESS, IMPLIED OR OTHERWISE, INCLUDING WITHOUT LIMITATION, ANY
// WARRANTY OF MERCHANTABILITY OR FITNESS FOR A PARTICULAR PURPOSE.
//
// IN NO EVENT SHALL PHILIP ROMANIK OR AMY MUNTZ BE LIABLE FOR
// ANY SPECIAL, INCIDENTAL, INDIRECT OR CONSEQUENTIAL DAMAGES OF ANY KIND,
// OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS,
// WHETHER OR NOT ADVISED OF THE POSSIBILITY OF DAMAGE, AND ON ANY THEORY OF
// LIABILITY, ARISING OUT OF OR IN CONNECTION WITH THE USE OR PERFORMANCE
// OF THIS SOFTWARE.
//
//
//  win32 critical section wrapper.

#ifndef WIN32
#error This version of suLock is only valid on win32 platforms
#endif


// We want this to be compatible with MFC and non-MFC apps
#ifdef _AFXDLL
#include <afx.h>
#else
#include <windows.h>
#endif



/*\class suLock
* brief On win32 platforms, the critical section object is the fastest
  thread locking object available.
*/

class suLock
{
public:
  suLock  ()     { InitializeCriticalSection (&lock_); }
  ~suLock ()     { DeleteCriticalSection (&lock_);}

  suLock (const suLock&)
  { InitializeCriticalSection (&lock_);}

  suLock& operator= (const suLock&)
  {}


  // Get the lock
  bool lock () const
  { EnterCriticalSection (&lock_); return true;}

  // Release the lock
  bool unlock () const
  { LeaveCriticalSection (&lock_); return true;}

private:
  mutable CRITICAL_SECTION lock_;
};


