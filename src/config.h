#pragma once
#define NOMINMAX
//
// VS 8+ includes some bounds checking even in debug mode.  This include disable this and
// enables full optimization.
//
#ifndef _DEBUG
#define _SECURE_SCL 0
#endif

//
// Enables all standard C-runtime functions without warnings
//
#define _CRT_SECURE_NO_DEPRECATE

//
// When defined, asserts are thrown on out-of-bounds vector access
//
#ifdef _DEBUG
#define VECTOR_DEBUG
#endif

//
// Enables asserts in release mode
//
//#define ALWAYS_ASSERT

#define UNIT_TEST


#include <engines.h>