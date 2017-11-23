// HookStackTracer.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "..\HookStackTracerDll\HookStackTraceDll.h"
#include "windows.h"

int _tmain(int argc, _TCHAR* argv[])
{
	//auto c = CreateEventExA(nullptr, nullptr, 0, 0);
	InitializeHookStackCollector();
	//TestHookStackCollector();

	auto a = CreateEventExA(nullptr, nullptr, 0, 0);
	auto b = CreateEventExW(nullptr, nullptr, 0, 0);
	CloseHandle(a);
	CloseHandle(b);

	DeInitializeHookStackCollector();
	return 0;
}

