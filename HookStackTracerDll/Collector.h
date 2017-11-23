#pragma once
#include "EnvStateSave.h"
#include <Windows.h>
#include <algorithm>
#include "..\HSTCommon\CallInfo.h"
#include <array>

void CollectInfo(HANDLE systemHandle, CallType callType)
{
	EnvStateSave state;
	CallInfo info{ callType };
	info.systemHandle = systemHandle;
	info.stackHash=0;
	std::fill(info.stackFrames.begin(), info.stackFrames.end(), nullptr);
	info.capturedFrames = CaptureStackBackTrace(0, info.stackFrames.size(), info.stackFrames.data(), &info.stackHash);
}

void CollectEventInfo(HANDLE createEventExResult)
{
	CollectInfo(createEventExResult, CallType::CollectEvent);
}

void CollectCloseHandleInfo(HANDLE createEventExResult)
{
	CollectInfo(createEventExResult, CallType::CloseHandle);
}
