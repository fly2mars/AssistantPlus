#ifndef _unix_lock_h_
#define _unix_lock_h_

// lock.h
//
// Copyright (c) 2003 Philip Romanik, Amy Muntz
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
//  Unix mutex wrapper.


#include <pthread.h>


// **********************
// *                    *
// *  suMutexAttribute  *
// *                    *
// **********************

// Our version of suLock resuses the same pthread_mutexattr_t object
// each time. This object initializes it. The value() method returns
// a pointer that you can pass to pthread_mutex_init

class suMutexAttribute
{
public:
  static suMutexAttribute& gOnly ();

  pthread_mutexattr_t* value () { return &attr_;}

private:
  static suMutexAttribute* sOnly_;

  pthread_mutexattr_t attr_;

  suMutexAttribute ();
  ~suMutexAttribute ();
};




// ************
// *          *
// *  suLock  *
// *          *
// ************

class suLock
{
public:
  suLock  ()     
  {
    pthread_mutex_init (&lock_, suMutexAttribute::gOnly().value());
  }
  ~suLock ()     { pthread_mutex_destroy (&lock_);}

  suLock (const suLock&) 
  {
    pthread_mutex_init (&lock_, suMutexAttribute::gOnly().value());
  }

  suLock& operator= (const suLock&) { return *this;}


  // Get the lock
  bool lock () const 
  { return pthread_mutex_lock (&lock_) == 0;}

  // Release the lock
  bool unlock () const 
  { return pthread_mutex_unlock (&lock_) == 0;}

private:
  mutable pthread_mutex_t lock_;
};


#endif // _unix_lock_h_
