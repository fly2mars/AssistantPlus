# ------------------------------------------------------------------------------
# helpers
# ------------------------------------------------------------------------------
# Add service Dlls into solution
function( install_App_Dll dllName destDir) # 1+ arguments: ARGV0 = Name of Dll / ARGV1 = dll dir

if( ${ARGC} LESS 1 )
	message( SEND_ERROR "function install_App_Dlls: invalid number of arguments! (need Dll name and some additional common files)" )
else()
	####Gen DLL dllName	
	FILE(GLOB DLL_${dllName}_SRC_SOURCES  Modules/${dllName}/*.cpp Modules/${dllName}/*.def)
	FILE(GLOB DLL_${dllName}_SRC_HEADERS  Modules/${dllName}/*.h)
	
	set(_args ${ARGN})  
	
	add_library(${dllName} SHARED ${DLL_${dllName}_SRC_SOURCES} 
	                        ${DLL_${dllName}_SRC_HEADERS}                               				
				${_args}			
				      )
        ##Copy dll to 
        ADD_CUSTOM_COMMAND(TARGET ${dllName}
		  POST_BUILD          
	          COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:${dllName}> ${destDir}
	)
	
endif()

endfunction()
