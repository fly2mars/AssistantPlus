/*
Asserts.h
Written by Matthew Fisher
*/

#pragma once

#if defined(_DEBUG)

//
// Traps the debugger if Statement is false; only checked if compiled in debug mode
//
void DebugOnlyAssert(bool Statement, const char *Description);
void DebugOnlyAssert(bool Statement, const suString &Description);

#else

#define DebugOnlyAssert(x, y)

#endif

#if defined(_DEBUG) || defined(ALWAYS_ASSERT)

//
// Traps the debugger if Statement is false.
// Checked if compiled in debug mode or if ALWAYS_ASSERT is defined.
//
void Assert(bool Statement, const char *Description);
void Assert(bool Statement, const suString &Description);

//
// Signals that a fatal error has occured.
//
void SignalError(const char *Description);
void SignalError(const suString &Description);

#else

//
// Release versions
//
//https://stackoverflow.com/questions/17516855/portable-equivalent-of-debugbreak
#define BREAKPOINT asm("int $3")
//#define SignalError(x)        __debugbreak(); ExitProcess(1);}

#define SignalError(x)        {BREAKPOINT; exit(1);}

#ifndef Assert
#define Assert(x, y)
#endif

#endif

//
// These versions are called independent of debug/release mode
//
void PersistentSignalError(const char *Description);
void PersistentSignalError(const suString &Description);
void PersistentAssert(bool Statement, const char *Description);
void PersistentAssert(bool Statement, const suString &Description);
