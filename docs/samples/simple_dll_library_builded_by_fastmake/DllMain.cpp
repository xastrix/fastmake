#include <iostream>
#include <windows.h>

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
		MessageBoxA(NULL, "Hello!", "Dll-library", NULL);
		break;
	case DLL_PROCESS_DETACH:
		break;
	}
	return TRUE;
}