#pragma once
#include "../HSTCommon/CallInfo.h"
#include <vector>

struct DecodedStackFrame
{
	DWORD64 Address;
	DWORD64 Displacement;
	std::string SymbolName;
	std::string SourcePath;
	int LineNumber;
	int ColumnNumber;
};

using DecodedStackFrames = std::vector<DecodedStackFrame>;

struct CallInfoExtended
{
	CallInfo callInfo;
	/*DWORD pid;
	HANDLE hProcess;*/
	DecodedStackFrames decoded;
};