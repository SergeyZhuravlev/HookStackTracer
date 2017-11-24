#pragma once
#include "windows.h"
#include <DbgHelp.h>
#include "..\HSTCommon\TracerDb.h"
#include "CallInfoExtended.h"
#include "..\HSTCommon\Logger.h"
#include <sstream>

#pragma comment(lib, "dbghelp.lib")


class Analysys
{
	TracerDb& _tracerDb;
	HANDLE _processHandle;
	HookStackTracerSettings _settings;
	DWORD _pid;
	Logger _results, _conclusion;

public:
	Analysys(TracerDb* const tracerDb)
		: _tracerDb(*tracerDb)
		, _processHandle()
		, _settings(GetHookStackTracerSettings())
		, _pid()
		, _results(GetHookStackTracerSettings().TracerResultsLogPath)
		, _conclusion(GetHookStackTracerSettings().TracerConclusionLogPath)
	{
		_results.Info("Starting Results Log");
		_conclusion.Info("Starting Conclusion Log");
	}

	~Analysys()
	{
		CloseHandle(_processHandle);
	}

	void Initialize()
	{
		auto pid = _tracerDb.ReadPid();
		_pid = pid;
		_conclusion.Info("Analysys for PID: " + std::to_string(pid));
		_processHandle = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pid);
		if (!_processHandle)
			throw std::runtime_error("Can't open process " + std::to_string(pid));
		auto oldOptions = SymSetOptions(0);
		SymSetOptions(oldOptions | SYMOPT_DEBUG | SYMOPT_DEFERRED_LOADS | SYMOPT_LOAD_LINES);
		if(!SymInitialize(_processHandle, _settings.SymbolSearchPaths/*NULL*/, TRUE))
			throw std::runtime_error("Can't debug process " + std::to_string(pid));
	}

	void Execute()
	{
		auto recordsAmount = _tracerDb.ReadRecordsAmount();
		_conclusion.Info("Analysys for records amount: " + std::to_string(recordsAmount));
		_tracerDb.BeginReading();
		while(recordsAmount>0)
		{
			--recordsAmount;
			CallInfoExtended info;
			_tracerDb.Read(info.callInfo);
			decode(info);
			output(info);
			analysys(info);
		}
	}

private:
	void analysys(const CallInfoExtended& info)
	{
		
	}

	std::string toString(CallType callType)
	{
		switch (callType)
		{
		case CallType::CreateEventA:
			return "CreateEventA";
		case CallType::CreateEventW:
			return "CreateEventW";
		case CallType::CreateEventExA:
			return "CreateEventExA";
		case CallType::CreateEventExW:
			return "CreateEventExW";
		case CallType::CloseHandle:
			return "CloseHandle";
		default:
			throw std::runtime_error("Unknown call type " + std::to_string((int)callType));
		}
	}

	std::string toString(std::time_t time)
	{
		char mbstr[100];
		if (std::strftime(mbstr, sizeof(mbstr), "%A %c", std::localtime(&time)))
			return mbstr;
		return std::to_string(time);
	}

	void output(const CallInfoExtended& info)
	{
		std::stringstream buf;
		buf
			<< "Call "
			<< toString(info.callInfo.callType)
			<< " for handle "
			<< (DWORD64)info.callInfo.systemHandle
			<< " with stack frames hash="
			<< info.callInfo.stackHash
			<< " with stack frames amount="
			<< info.callInfo.capturedFrames
			<< " on time "
			<< toString(info.callInfo.callTime)
			<< std::endl;
		for (auto&& stackFrame : info.decoded)
		{
			buf
				<< "		at symbol '"
				<< stackFrame.SymbolName
				<< "' line="
				<< stackFrame.LineNumber
				<< " on file '"
				<< stackFrame.SourcePath
				<< "' on column="
				<< stackFrame.ColumnNumber
				<< " on address="
				<< stackFrame.Address
				<< " on displacement="
				<< stackFrame.Displacement
				<< std::endl;
		}
		_results.Info(buf.str());
		_results.WriteLine("=============================================================");
	}

	void decode(CallInfoExtended& info) const
	{
		char buffer[sizeof(SYMBOL_INFO) + MAX_SYM_NAME * sizeof(TCHAR)];
		PSYMBOL_INFO pSymbol = (PSYMBOL_INFO)buffer;

		pSymbol->SizeOfStruct = sizeof(SYMBOL_INFO);
		pSymbol->MaxNameLen = MAX_SYM_NAME;

		auto beginIt = &*info.callInfo.stackFrames.begin();
		auto endIt = beginIt + info.callInfo.capturedFrames;
		for (auto it = beginIt; it != endIt; it++)
		{
			auto address = (DWORD64)*it;
			info.decoded.push_back({ address , (DWORD64)-1i64, "Unknown symbol", "Unknown source file", -1, -1});
			auto& frameDecoded = info.decoded.back();
			if (SymFromAddr(_processHandle, address, &frameDecoded.Displacement, pSymbol))
			{
				frameDecoded.SymbolName = std::string(pSymbol->Name, pSymbol->Name + pSymbol->NameLen);
				IMAGEHLP_LINE64 line;
				line.SizeOfStruct = sizeof(IMAGEHLP_LINE64);
				DWORD column = -1;
				if (SymGetLineFromAddr64(_processHandle, address, &column, &line))
				{
					frameDecoded.ColumnNumber = column;
					frameDecoded.LineNumber = line.LineNumber;
					frameDecoded.SourcePath = line.FileName;
				}
			}
		}
	}
};