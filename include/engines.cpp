//#include <engines.h>
/////////////////////////////
//      Core               //
/////////////////////////////
#include <Engines/Core/Stdhdr.cpp>
#include <Engines/Core/UnicodeString.cpp>
#include <Engines/Core/String.cpp>



/////////////////////////////
//      Utilities          //
/////////////////////////////

//#include <Engines/utility/wideTools.cpp>
//#include <Engines/utility/xmlTools.cpp>
//#include <Engines/utility/resourceMgr.cpp>
#include <Engines/utility/tinyxml2.cpp>

#ifdef _WIN32
#include <Engines/System/Win32/fastTiming.cpp>
#include <Engines/utility/commonTools.cpp>
#include <Engines/utility/dlfcn.cpp>
#elif __linux__
#include <Engines/System/unix/fastTiming.cpp>
#include <Engines/System/unix/lock.cpp>
#include <Engines/System/unix/commonTools.cpp>
#elif __APPLE__

#elif __unix__ // all unixes not caught above
// Unix
#elif defined(_POSIX_VERSION)
// POSIX
#else
#   error "Unknown compiler"
#endif //end if win32

#include <Engines/Utility/Console.cpp>


#ifdef UNIT_TEST
#include <Engines/Utility/unitTest.cpp>
#endif
