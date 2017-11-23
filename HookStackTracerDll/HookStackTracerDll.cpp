// HookStackTracerDll.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include "PatchHandlers.h"
#include "Patcher.h"
#include "HookStackTraceDll.h"
#include "Windows.h"

void InitializeHookStackCollector()
{
	replaceFunctionByMemoryPatch(CreateEventExA, CreateEventExAHandlerPtr, [](void** oldHandler)
	{
		CreateEventExAOldHandlerPtr = oldHandler;
		CreateEventExAOldHandler = *oldHandler;
	});
	replaceFunctionByMemoryPatch(CreateEventExW, CreateEventExWHandlerPtr, [](void** oldHandler)
	{
		CreateEventExWOldHandlerPtr = oldHandler;
		CreateEventExWOldHandler = *oldHandler;
	});
	replaceFunctionByMemoryPatch(CloseHandle, CloseHandleHandlerPtr, [](void** oldHandler)
	{
		CloseHandleOldHandlerPtr = oldHandler;
		CloseHandleOldHandler = *oldHandler;
	});
}

void DeInitializeHookStackCollector()
{
	replaceFunctionByMemoryPatch(CreateEventExA, CreateEventExAOldHandlerPtr);
	replaceFunctionByMemoryPatch(CreateEventExW, CreateEventExWOldHandlerPtr);
	replaceFunctionByMemoryPatch(CloseHandle, CloseHandleOldHandlerPtr);
}

/*void TestHookStackCollector()
{
	CreateEventExAHandler();
	CreateEventExWHandler();
	CloseHandleHandler();
}*/
