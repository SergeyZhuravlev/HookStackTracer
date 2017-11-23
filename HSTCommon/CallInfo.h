#pragma once
#include "Windows.h"
#include <array>
#include <ctime>

enum class CallType
{
	CreateEvent_ = 0,
	CloseHandle = 1
};

const size_t MAX_STACK_FRAMES_FOR_CAPTURE = 1024;

#pragma pack (push)
#pragma pack (1)
struct CallInfo
{
	CallType callType;
	WORD capturedFrames;
	std::array<void*, MAX_STACK_FRAMES_FOR_CAPTURE> stackFrames;
	DWORD stackHash;
	HANDLE systemHandle;
	std::time_t callTime;
};
#pragma pack (pop)