#pragma once
//A basic code set
//Write by Yao 

/////////////////////////////
//      Core               //
/////////////////////////////

#ifdef __linux__
#include <Engines/System/unix/win2linux_types.h>
#elif __APPLE__

#elif __unix__ // all unixes not caught above
// Unix
#elif defined(_POSIX_VERSION)
// POSIX
#endif

//Forward declarations for several classes/structures
#include <Engines/Core/ClassList.h>
#include <Engines/Core/ExternalFiles.h>

//Collection of useful constants, macros, and functions
#ifdef WIN32
#pragma warning (disable : 4996)  //to disable warning from Microsoft, because we use a lot common "unsafe" function like "fopen"...
#endif

#include <Engines/Core/Vector.h>
#include <Engines/Core/Stdhdr.h>

//A nice std::vector like vector structure written by Matthew Fisher
//#include <Engines\Core\Vector.h>
//A std::string like string class written by Matthew Fisher, used here to make "num-string convertion" more convinient.
#include <Engines/Core/String.h>
#include <Engines/Core/UnicodeString.h>


/////////////////////////////
//      Utilities          //
/////////////////////////////
#include <Engines/utility/tstring.h>
#include <Engines/utility/exception.h>
//#include <Engines/utility/wideTools.h>
//#include <Engines/utility/xmlTools.h>
//#include <Engines/utility/resourceMgr.h>
#include <Engines/utility/tinyxml2.h>
#include <Engines/utility/thread.h>
#include <Engines/utility/lock.h>
#include <Engines/Utility/timing.h>

#if defined(WIN32)
#include <Engines/utility/dlfcn.h>
#include <Engines/utility/commonTools.h>
#elif  defined(__linux)
#include <dlfcn.h>
#include <Engines/System/unix/commonTools.h>
#endif 

#include <Engines/utility/suCMDParser.h>
#include <Engines/Utility/Console.h>


#ifdef UNIT_TEST
#include <Engines/Utility/unitTest.h>
#endif
