// HookStackTracer.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "..\HookStackTracerDll\HookStackTraceDll.h"
#include "windows.h"
#include <exception>
#include <conio.h>
#include <iostream>
#include "..\HSTCommon\TracerDb.h"
#include "Analysys.h"
#include "DebugPrivilege.h"

int _tmain(int argc, _TCHAR* argv[])
{
	try 
	{
		std::cout << "Initializing..." << std::endl;
		std::cout << "EnableDebugPrivilege..." << std::endl;
		if(!EnableDebugPrivilege())
			std::cout << "Can't enable DebugPrivilege" << std::endl;
		/*to del --------
		std::cout << "Initializing local..." << std::endl;
		InitializeHookStackCollector();
		std::cout << "Process..." << std::endl;
		auto a = CreateEventExA(nullptr, nullptr, 0, 0);
		auto b = CreateEventExW(nullptr, nullptr, 0, 0);
		for(int i = 4; i>0; --i)
			CreateEventA(nullptr, 0, 0, nullptr);
		CreateEventW(nullptr, 0, 0, nullptr);
		CloseHandle(a);
		CloseHandle(b);
		-------- to del*/
		std::cout << "TracerDb open..." << std::endl;
		TracerDb db;
		db.OpenRead();
		auto recordsAmount = db.ReadRecordsAmount();
		std::cout << "Begin analysys for nearly " << recordsAmount << " records amount " << std::endl;
		std::cout << "Initializing" << std::endl;
		Analysys as(&db);
		as.Initialize();
		std::cout << "Executing" << std::endl;
		as.Execute();
		std::cout << "MakeConclusion" << std::endl;
		as.MakeConclusion();
		std::cout << "Finished analysys" << std::endl;
		_getch();
	}
	catch (std::exception& e)
	{
		std::cout << "Work error: " << e.what() << std::endl;
		_getch();
	}

	/*to del --------
	try
	{
		DeInitializeHookStackCollector();
	}
	catch (std::exception& e)
	{
		std::cout << "Deinit error: " << e.what() << std::endl;
		_getch();
	}
	-------- to del*/
	return 0;
}

