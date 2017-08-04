cmake_minimum_required(VERSION 3.0 FATAL_ERROR)
# add our macro directory to cmake search path
set (CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_SOURCE_DIR}/cmake)

project(Assitant+)
set(APP_NAME Assistant+)

FILE(GLOB SRC_SOURCES src/*.cpp)
FILE(GLOB SRC_HEADERS src/*.h )

set(SRC_LIST ${SRC_SOURCES} 
             ${SRC_HEADERS}               
              )


set(PROJECT_INCLUDE_DIRS ${CMAKE_SOURCE_DIR}/include) 
set(PROJECT_LIBRARY_DIRS ${CMAKE_SOURCE_DIR}/lib) 

set(PROJECT_BINARY_DIR ${CMAKE_SOURCE_DIR}/bin/)
set(PROJECT_DLL_DIR ${CMAKE_SOURCE_DIR}/bin/app)

set(PROJECT_LIBRARYS)

Message("Project include: " ${PROJECT_INCLUDE_DIRS})

####################Compiler###################################
# C++ compile flags
# -std=c++11 -> for c++ standarts
# -Wall for WARNING
# -pedantic -> for warning between c and c++
# -pthread for portability
# -g -O0 -fprofile-arcs -ftest-coverage for code coverage
# https://github.com/codecov/example-cpp11-cmake/blob/master/run_build.sh
if(NOT CMAKE_VERSION VERSION_LESS 3.1)
    set(CMAKE_CXX_STANDARD 11)
else()
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
endif()

####################Find Lib###################################

## C++ Boost
set(Boost_DEBUG OFF)
set(Boost_USE_STATIC_LIBS ON) 
#set(Boost_USE_STATIC_RUNTIME ON) 
set(Boost_USE_MULTITHREADED ON)  

Message("Finding  " "Boost lib...")  
if(WIN32)
  set(Boost_INCLUDE_DIR e:/devLib/boost)
  set(Boost_LIBRARY_DIRS e:/devLib/boost/lib32-msvc-14.0)
  find_package(Boost COMPONENTS REQUIRED system  regex  filesystem) 
else()
  find_package(Boost COMPONENTS REQUIRED system  regex filesystem)
endif(WIN32)

if(!Boost_FOUND)
   Message(STATUS "Boost lib not found")  
else()
   Message("Boost Path: " ${Boost_INCLUDE_DIRS})
   Message("Boostlib Path: " ${Boost_LIBRARIES} )
endif()


## Threads
if(UNIX) 
   Message("Find " "Thread lib")
   set(THREADS_PREFER_PTHREAD_FLAG ON)
   find_package(Threads REQUIRED)
endif()

## Project related paths
include_directories(
  ${PROJECT_INCLUDE_DIRS}
  ${Boost_INCLUDE_DIRS}
  )
  
link_directories( 
  ${PROJECT_LIBRARY_DIRS}
  ${Boost_LIBRARY_DIRS} 
  )

###########################DLLs########################################
####Add Dlls to solution
include( cmake/CMakeInclude.cmake )
set(_CommonFiles "Modules/suVirtualDevice.cpp"  "Modules/suVirtualDevice.cpp" "include/Engines.h" "include/Engines.cpp")

## install_App_Dll("GetVar"  ${PROJECT_DLL_DIR} ${_CommonFiles})
## install_App_Dll("SetVar"  ${PROJECT_DLL_DIR} ${_CommonFiles})
## install_App_Dll("Quote"  ${PROJECT_DLL_DIR} ${_CommonFiles})
## install_App_Dll("Convert" ${PROJECT_DLL_DIR} ${_CommonFiles})
## install_App_Dll("AgentMonitor" ${PROJECT_DLL_DIR} ${_CommonFiles})
## install_App_Dll("Translator" ${PROJECT_DLL_DIR} ${_CommonFiles})
## install_App_Dll("RuleEngine" ${PROJECT_DLL_DIR} ${_CommonFiles})
## install_App_Dll("TaskSubmit" ${PROJECT_DLL_DIR} ${_CommonFiles})
## install_App_Dll("TaskQuery" ${PROJECT_DLL_DIR} ${_CommonFiles})
## install_App_Dll("ServerStatus" ${PROJECT_DLL_DIR} ${_CommonFiles})
## install_App_Dll("Executor" ${PROJECT_DLL_DIR} ${_CommonFiles})
## install_App_Dll("QueryOrder" ${PROJECT_DLL_DIR} ${_CommonFiles})
## install_App_Dll("OrderListener" ${PROJECT_DLL_DIR} ${_CommonFiles})		      
######################################################################			      
####Asistant+ Server
set(CMAKE_BUILD_TYPE Release)
#set(CMAKE_BUILD_TYPE Debug)

add_executable (${APP_NAME} ${SRC_LIST} )
target_link_libraries( ${APP_NAME}  
           ${Boost_LIBRARIES}
		   ${CMAKE_THREAD_LIBS_INIT}
		   ${CMAKE_DL_LIBS}  )   #dlopen lib

# copy bin to binary folder
ADD_CUSTOM_COMMAND(TARGET ${APP_NAME}
          POST_BUILD         
          COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:${APP_NAME}> ${PROJECT_BINARY_DIR}
)

IF(WIN32)
# set working directory 
## replace setting and copy setting file
SET(USER_FILE ${APP_NAME}.vcproj.user)
SET(OUTPUT_PATH ${CMAKE_CURRENT_BINARY_DIR}/)

SET(WORKING_COMMAND ${PROJECT_BINARY_DIR}${APP_NAME}.exe)
SET(USERFILE_WORKING_DIRECTORY_DEBUG ${PROJECT_BINARY_DIR})
CONFIGURE_FILE(vc/${APP_NAME}.vcxproj.user ${OUTPUT_PATH}${APP_NAME}.vcxproj.user @ONLY)
message(STATUS "编译后在bin目录生成文件，并使用模板文件和变量替换visual studio的调试设置")


######################################################################				      
####Assistant+ windows tray

FILE(GLOB TRAY_SRC src/Config.cpp src/sysTray/*.h src/sysTray/*.cpp src/sysTray/*.rc)
SET(tray_sources ${SRC_LIST})
LIST(REMOVE_ITEM tray_sources ${CMAKE_CURRENT_SOURCE_DIR}/src/ServerMain.cpp)
add_executable (${APP_NAME}Tray 
	  ${tray_sources}
	${TRAY_SRC}		  
	  )
  
# target_link_libraries (${APP_NAME}Tray Win32Tools)
if(MSVC) # Check if we are using the Visual Studio compiler
	set_target_properties(${APP_NAME}Tray PROPERTIES LINK_FLAGS_RELEASE "/SUBSYSTEM:WINDOWS")
	set_target_properties(${APP_NAME}Tray PROPERTIES LINK_FLAGS_DEBUG "/SUBSYSTEM:WINDOWS")
	Message(${APP_NAME}Tray " is a Windows program")
endif(MSVC)
 
# copy binary to running director
ADD_CUSTOM_COMMAND(TARGET ${APP_NAME}Tray 
   POST_BUILD          
   COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:${APP_NAME}Tray > ${PROJECT_BINARY_DIR}
)




message(STATUS "公共代码文件目录: " ${CMAKE_SOURCE_DIR}/include)
message(STATUS "公共库文件目录: " ${CMAKE_SOURCE_DIR}/lib)
message(STATUS "3)CMake生成VC工程后，在suAgent工程的属性页->调试->将[工作目录]设为<你的代码目录>/Bin")
message(STATUS "4)suAgent工程的属性页->调试->将[命令]设为<你的代码目录>/Bin/${APP_NAME}.exe，以便利用IDE进行调试。")

ENDIF(WIN32) 

# Test Unit
FILE(GLOB UNITTEST_SRC unit_test/*.cpp unit_test/*.h)
add_executable (UnitTest ${UNITTEST_SRC} src/config.h src/config.cpp src/ServerSet.h 
                src/ServerSet.cpp  src/Option.cpp
				src/redispp.cpp src/suMemory.cpp)
target_link_libraries( UnitTest  
           ${Boost_LIBRARIES}
		   ${CMAKE_THREAD_LIBS_INIT} 
		   ${CMAKE_THREAD_LIBS_INIT}
		   ${CMAKE_DL_LIBS}  )   #dlopen lib
message("Boost Libs: " ${Boost_LIBRARIES} )
