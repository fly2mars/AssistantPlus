cmake_minimum_required(VERSION 3.0 FATAL_ERROR)
set (CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_SOURCE_DIR}/cmake)

set(APP_NAME Assistant+)
project(${APP_NAME})

option(OPTION_TEST_UNIT              "Generate UnitTest"  OFF)
option(OPTION_MODULES                "Generate plugin modules" OFF)
option(OPTION_WINDOWS_TRAY           "Generate windows tray app" OFF)
################
# Properties   #
################
if(NOT CMAKE_VERSION VERSION_LESS 3.1)
    set(CMAKE_CXX_STANDARD 11)
else()
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
endif()
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

###############
#  Find Lib   #
###############

## C++ Boost
INCLUDE(${PROJECT_SOURCE_DIR}/cmake/UseBoost.cmake)

################
# Project path #
################
set(PROJECT_INCLUDE_DIRS ${CMAKE_SOURCE_DIR}/include ${CMAKE_SOURCE_DIR}/src) 
set(PROJECT_LIBRARY_DIRS ${CMAKE_SOURCE_DIR}/lib) 

set(DEPLOY_DIR ${CMAKE_SOURCE_DIR}/bin/)
set(PROJECT_DLL_DIR ${CMAKE_SOURCE_DIR}/bin/App)

set(PROJECT_LIBRARYS)

Message("Project include: " ${PROJECT_INCLUDE_DIRS})

## Project related paths
include_directories(
  ${PROJECT_INCLUDE_DIRS}
  ${Boost_INCLUDE_DIRS}
  )
  
link_directories( 
  ${PROJECT_LIBRARY_DIRS}
  ${Boost_LIBRARY_DIRS} 
  )

# ========================================
# === common, build static lib to speed up
# ========================================
file(GLOB COMMON_SOURCES common/*.cpp)
add_library(common STATIC ${COMMON_SOURCES})


## Threads
if(UNIX) 
   Message("Find " "Thread lib")
   set(THREADS_PREFER_PTHREAD_FLAG ON)
   find_package(Threads REQUIRED)
endif()     

####################
# Asistant+ Server #
####################
set(CMAKE_BUILD_TYPE Release)  #set(CMAKE_BUILD_TYPE Debug)

FILE(GLOB SRC_SOURCES src/*.cpp)
FILE(GLOB SRC_HEADERS src/*.h )

set(SRC_LIST ${SRC_SOURCES} 
             ${SRC_HEADERS}               
              )

add_executable (${APP_NAME} ${SRC_LIST} )


target_link_libraries( ${APP_NAME}  
           common
           Boost::log_setup 
           Boost::log
           ${Boost_LIBRARIES}
		   ${CMAKE_THREAD_LIBS_INIT}
		   ${CMAKE_DL_LIBS}  )   #dlopen lib
#INCLUDE(${PROJECT_SOURCE_DIR}/cmake/LinkBoost.cmake)

# copy bin to binary folder
ADD_CUSTOM_COMMAND(TARGET ${APP_NAME}
          POST_BUILD         
          COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:${APP_NAME}> ${DEPLOY_DIR}
)



IF(MSVC) 
	# set working directory 
	## replace setting and copy setting file
	SET(USER_FILE ${APP_NAME}.vcproj.user)
	SET(OUTPUT_PATH ${CMAKE_CURRENT_BINARY_DIR}/)

	SET(WORKING_COMMAND ${DEPLOY_DIR}${APP_NAME}.exe)
	SET(USERFILE_WORKING_DIRECTORY_DEBUG ${DEPLOY_DIR})
	CONFIGURE_FILE(vc/${APP_NAME}.vcxproj.user ${OUTPUT_PATH}${APP_NAME}.vcxproj.user @ONLY)
	message(STATUS "编译后在bin目录生成文件，并使用模板文件和变量替换visual studio的调试设置")
ENDIF(MSVC)

#################################
# Assistant+ windows tray       #
#################################	
IF(OPTION_WINDOWS_TRAY)		      
if(WIN32)
	FILE(GLOB TRAY_SRC src/Config.cpp src/sysTray/*.h src/sysTray/*.cpp src/sysTray/*.rc)
	SET(tray_sources ${SRC_LIST})
	LIST(REMOVE_ITEM tray_sources ${CMAKE_CURRENT_SOURCE_DIR}/src/ServerMain.cpp)
	add_executable (${APP_NAME}Tray 
		  ${tray_sources}
		${TRAY_SRC}		  
		  )
  
	target_link_libraries(${APP_NAME}Tray  
			   ${Boost_LIBRARIES}
			   ${CMAKE_THREAD_LIBS_INIT}
			   ${CMAKE_DL_LIBS}  )   #dlopen lib

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

endif(WIN32)
ENDIF(OPTION_WINDOWS_TRAY) 


###############
#  Add DLLs   #
###############
IF(OPTION_MODULES)
include( cmake/CMakeInclude.cmake )
set(_CommonFiles "Modules/config.h"  "Modules/config.cpp"  "Modules/suVirtualDevice.cpp"  "Modules/suVirtualDevice.cpp" )

install_App_Dll("Null"  ${PROJECT_DLL_DIR} ${_CommonFiles})
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
ENDIF(OPTION_MODULES)	 

##############
# Test Units #
##############
IF(OPTION_TEST_UNIT)
  add_subdirectory("unit_test")
ENDIF(OPTION_TEST_UNIT)

set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT ${APP_NAME})
