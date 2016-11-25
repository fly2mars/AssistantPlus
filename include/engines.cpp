//#include <engines.h>
/////////////////////////////
//      Core               //
/////////////////////////////
#include <Engines\Core\Stdhdr.cpp>
#include <Engines\Core\UnicodeString.cpp>
#include <Engines\Core\String.cpp>



/////////////////////////////
//      Utilities          //
/////////////////////////////
#include <Engines/Utility/timing.cpp>
#include <Engines/utility/wideTools.cpp>
#include <Engines/utility/xmlTools.cpp>
#include <Engines/utility/resourceMgr.cpp>
#include <Engines/utility/tinyxml2.cpp>
#ifdef WIN32
#include <Engines/utility/commonTools.cpp>
#endif // WIN32

#include <Engines/Utility/Console.cpp>
#include <Engines/utility/dlfcn.cpp>

#ifdef UNIT_TEST
#include <Engines/Utility/unitTest.cpp>
#endif
