#pragma once
//A basic codde set
//Write by Yao 

/////////////////////////////
//      Core               //
/////////////////////////////

//Forward declarations for several classes/structures
#include <Engines/Core/ClassList.h>
#include <Engines/Core/ExternalFiles.h>

//Collection of useful constants, macros, and functions
#pragma warning (disable : 4996)  //to disable warning from Microsoft, because we use a lot common "unsafe" function like "fopen"...
#include <Engines/Core/Vector.h>
#include <Engines/Core/Stdhdr.h>


//A nice std::vector like vector structure written by Matthew Fisher
//#include <Engines\Core\Vector.h>
//A std::string like string class written by Matthew Fisher, used here to make "num-string convertion" more convinient.
#include <Engines\Core\String.h>
#include <Engines\Core\UnicodeString.h>


/////////////////////////////
//      Utilities          //
/////////////////////////////
#include <Engines/utility/tstring.h>
#include <Engines/utility/exception.h>
#include <Engines/utility/lock.h>
#include <Engines/Utility/timing.h>
#include <Engines/utility/wideTools.h>
#include <Engines/utility/xmlTools.h>
#include <Engines/utility/resourceMgr.h>
#include <Engines/utility/tinyxml2.h>
#include <Engines/utility/thread.h>
#ifdef WIN32
#include <Engines/utility/commonTools.h>
#endif // WIN32

#include <Engines/utility/suCMDParser.h>
#include <Engines/Utility/Console.h>
#include <Engines/utility/dlfcn.h>

#ifdef UNIT_TEST
#include <Engines/Utility/unitTest.h>
#endif
