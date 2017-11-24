// HookStackTracerDll.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include "PatchHandlers.h"
#include "Patcher.h"
#include "HookStackTraceDll.h"
#include "Windows.h"
#include "..\HSTCommon\TracerDb.h"
#include "TracerDbInstance.h"

TracerDb tracerDb;

void InitializeHookStackCollector()
{
	tracerDb.OpenWrite();
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
	replaceFunctionByMemoryPatch(CreateEventA, CreateEventAHandlerPtr, [](void** oldHandler)
	{
		CreateEventAOldHandlerPtr = oldHandler;
		CreateEventAOldHandler = *oldHandler;
	});
	replaceFunctionByMemoryPatch(CreateEventW, CreateEventWHandlerPtr, [](void** oldHandler)
	{
		CreateEventWOldHandlerPtr = oldHandler;
		CreateEventWOldHandler = *oldHandler;
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
	replaceFunctionByMemoryPatch(CreateEventA, CreateEventAOldHandlerPtr);
	replaceFunctionByMemoryPatch(CreateEventW, CreateEventWOldHandlerPtr);
	replaceFunctionByMemoryPatch(CloseHandle, CloseHandleOldHandlerPtr);
	tracerDb.Close();
}

/*void TestHookStackCollector()
{
	CreateEventExAHandler();
	CreateEventExWHandler();
	CloseHandleHandler();
}*/
