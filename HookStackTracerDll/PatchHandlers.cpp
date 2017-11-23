#include "stdafx.h"
#include "PatchHandlers.h"
#include "EnvStateSave.h"
#include "CpuStateSave.h"
#include <Windows.h>
#include "ThreadsInfo.h"

ThreadsInfo inFunctionThreadInfo;

void CollectEventInfo(HANDLE createEventExResult)
{
	EnvStateSave state;
}

void CollectCloseHandleInfo(HANDLE createEventExResult)
{
	EnvStateSave state;
}

/*int Dummy()
{
	return 7;
}*/

void** CreateEventExAOldHandlerPtr = nullptr /*= Dummy*/;
void* CreateEventExAOldHandler = nullptr;
__declspec(naked) void CreateEventExAHandler()
{
	HANDLE createEventExResult;
	void** stackTopBeforeCall;//returnAddress
	SaveCpuState(simd_state1, ebx_state1, ecx_state1, loo1)
	__asm mov stackTopBeforeCall, ebp
	stackTopBeforeCall += 1;//ebp size
	inFunctionThreadInfo.AddThreadInfo({*stackTopBeforeCall});
	__asm mov ebx, stackTopBeforeCall
	__asm mov ecx, offset returnToMe
	__asm mov [ebx], ecx
	RestoreCpuState(simd_state1)
	__asm jmp CreateEventExAOldHandler
returnToMe:
	__asm call forPushNewRetAddress
forPushNewRetAddress:
	SaveCpuState(simd_state, ebx_state, ecx_state, loo)
	__asm mov createEventExResult, eax
	__asm mov stackTopBeforeCall, ebp
	stackTopBeforeCall += 1;//ebp size
	*stackTopBeforeCall = inFunctionThreadInfo.GetThreadInfo().ReturnAddress;
	inFunctionThreadInfo.RemoveThreadInfo();
	CollectEventInfo(createEventExResult);
	RestoreCpuState(simd_state)
	__asm ret
}
void* CreateEventExAHandlerTmp = CreateEventExAHandler;
void** CreateEventExAHandlerPtr = &CreateEventExAHandlerTmp;

void** CreateEventExWOldHandlerPtr = nullptr /*= Dummy*/;
void* CreateEventExWOldHandler = nullptr;
__declspec(naked) void CreateEventExWHandler()
{
	HANDLE createEventExResult;
	void** stackTopBeforeCall;//returnAddress
	SaveCpuState(simd_state1, ebx_state1, ecx_state1, loo1)
	__asm mov stackTopBeforeCall, ebp
	stackTopBeforeCall += 1;//ebp size
	inFunctionThreadInfo.AddThreadInfo({ *stackTopBeforeCall });
	__asm mov ebx, stackTopBeforeCall
	__asm mov ecx, offset returnToMe
	__asm mov[ebx], ecx
	RestoreCpuState(simd_state1)
	__asm jmp CreateEventExWOldHandler
returnToMe :
	__asm call forPushNewRetAddress
forPushNewRetAddress :
	SaveCpuState(simd_state, ebx_state, ecx_state, loo)
	__asm mov createEventExResult, eax
	__asm mov stackTopBeforeCall, ebp
	stackTopBeforeCall += 1;//ebp size
	*stackTopBeforeCall = inFunctionThreadInfo.GetThreadInfo().ReturnAddress;
	inFunctionThreadInfo.RemoveThreadInfo();
	CollectEventInfo(createEventExResult);
	RestoreCpuState(simd_state)
	__asm ret
}
void* CreateEventExWHandlerTmp = CreateEventExWHandler;
void** CreateEventExWHandlerPtr = &CreateEventExWHandlerTmp;

void** CloseHandleOldHandlerPtr = nullptr /*= Dummy*/;
void* CloseHandleOldHandler = nullptr;
__declspec(naked) void CloseHandleHandler()
{
	HANDLE closedHandle;
	SaveCpuState(simd_state, ebx_state, ecx_state, loo)
	__asm
	{
		mov ebx, ebp
		add ebx, 4 + 4
		mov ecx, dword ptr [ebx]
		mov closedHandle, ecx
	}
	CollectCloseHandleInfo(closedHandle);
	RestoreCpuState(simd_state)
	__asm jmp CloseHandleOldHandler
}
void* CloseHandleHandlerTmp = CloseHandleHandler;
void** CloseHandleHandlerPtr = &CloseHandleHandlerTmp;