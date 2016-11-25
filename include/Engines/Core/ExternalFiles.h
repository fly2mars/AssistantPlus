#pragma once
/*
 * ExternalFiles.h
 *
 * All #includes that are part of C++ core, STL, or other libraries
*/

// Windows
#include <winsock2.h>   //to aoid sock type error http://stackoverflow.com/questions/9750344/boostasio-winsock-and-winsock-2-compatibility-issue
#include <windows.h>
#include <winuser.h>

#ifdef USE_OPENGL
// OpenGL
#include <gl\gl.h>
#include <gl\glu.h>
#endif

#ifdef USE_EIGEN
#include <Eigen/Core>
#endif


// C/C++
#include <math.h>
#include <memory.h>
#include <time.h>
#include <io.h>
#include <stdio.h>
#include <stdlib.h>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <fstream>
#include <limits>
#include <exception>

// STL
#include <map>
#include <queue>
#include <set>
#include <vector>

#ifdef USE_ZLIB
// ZLib (compression)
#include "zlib.h"

// PNG saving and loading (requires zlib)
#ifdef USE_PNG
#include "png.h"
#endif

#endif

#ifdef USE_SDL
#include "SDL\SDL_image.h"
#endif
