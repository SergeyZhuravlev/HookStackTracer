#pragma once
#include "EnvStateSave.h"
#include <Windows.h>
#include <algorithm>
#include "..\HSTCommon\CallInfo.h"
#include "TracerDbInstance.h"
#include <array>

void CollectInfo(HANDLE systemHandle, CallType callType)
{
	EnvStateSave state;
	CallInfo info{ callType };
	info.systemHandle = systemHandle;
	info.stackHash=0;
	info.callTime = std::time(nullptr);
	std::fill(info.stackFrames.begin(), info.stackFrames.end(), nullptr);
	info.capturedFrames = CaptureStackBackTrace(0, info.stackFrames.size(), info.stackFrames.data(), &info.stackHash);
	tracerDb.Write(info);
}

void CollectEventInfo(HANDLE createEventExResult)
{
	CollectInfo(createEventExResult, CallType::CreateEvent_);
}

void CollectCloseHandleInfo(HANDLE createEventExResult)
{
	CollectInfo(createEventExResult, CallType::CloseHandle);
}
