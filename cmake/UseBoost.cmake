##################
# UseBoost.cmake #
##################

SET(Boost_ADDITIONAL_VERSIONS "1.53" "1.53.0" "1.54" "1.54.0" "1.55" "1.55.0" "1.56" "1.56.0" "1.67")
#Change this path
SET(BOOST_ROOT E:/devLib/boost CACHE FILEPATH "The Boost root directory")
SET(Boost_USE_STATIC_LIBS ON)
set(Boost_USE_MULTITHREADED ON)
message("BOOST ROOT: " ${BOOST_ROOT})
FIND_PACKAGE(Boost REQUIRED COMPONENTS  system  regex  filesystem)
IF(Boost_FOUND)
  INCLUDE_DIRECTORIES(SYSTEM ${Boost_INCLUDE_DIRS})
  LINK_DIRECTORIES(${Boost_LIBRARY_DIRS})
  ADD_DEFINITIONS(-DBOOST_ALL_NO_LIB)
ELSE(Boost_FOUND)
  MESSAGE(FATAL_ERROR "Boost not found. Please set the directories manually using the advanced view in CMake.")
ENDIF(Boost_FOUND)
