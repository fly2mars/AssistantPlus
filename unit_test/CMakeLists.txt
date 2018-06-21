
##################
# Test Unit                    #
##################

add_executable (UnitTest main.cpp 
                ../src/config.h ../src/config.cpp ../src/ServerSet.h 
                ../src/ServerSet.cpp  ../src/Option.cpp
	../src/redispp.cpp ../src/suMemory.cpp)
target_link_libraries( UnitTest  
           ${Boost_LIBRARIES}
		   ${CMAKE_THREAD_LIBS_INIT}
		   ${CMAKE_DL_LIBS}  )   #dlopen lib

##################
# Test LIB error #
##################
add_executable(test 
                test.cpp 
                ../src/config.h ../src/config.cpp ../src/ServerSet.h 
                ../src/ServerSet.cpp  ../src/Option.cpp
	../src/redispp.cpp ../src/suMemory.cpp
)
target_link_libraries( test  
           ${Boost_LIBRARIES}
		   ${CMAKE_THREAD_LIBS_INIT}
		   ${CMAKE_DL_LIBS}  )   #dlopen lib

if(WIN32)
    set_property(TARGET UnitTest  PROPERTY FOLDER "Tests")
    set_property(TARGET test PROPERTY FOLDER "Tests")
    # Properties->General->Output Directory
    set_property(TARGET UnitTest test PROPERTY RUNTIME_OUTPUT_DIRECTORY_RELEASE ${PROJECT_BINARY_DIR}/$(Platform)/$(Configuration))
    set_property(TARGET UnitTest test PROPERTY RUNTIME_OUTPUT_DIRECTORY_DEBUG ${PROJECT_BINARY_DIR}/$(Platform)/$(Configuration))
endif (WIN32)		   
