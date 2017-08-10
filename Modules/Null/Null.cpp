// Null.cpp : Defines the entry point for the DLL application.
//

#include "../config.h"
#include "Null.h"
#include "simpleDll.h"

#ifdef WIN32
BOOL __stdcall DllMain( HANDLE hModule, 
                       DWORD  ul_reason_for_call, 
                       LPVOID lpReserved
					 )
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
	case DLL_PROCESS_DETACH:
		break;
	}
    return TRUE;
}
#endif

//external method which creates an instance of the module when called within Simulation
extern "C" suService* create() {
	return new SimpleDll();
}
extern "C" suService* createEx(suStatus* pStatus) {
	return new SimpleDll(pStatus);
}