#include "stdafx.h"
#include "PatchHandlers.h"
#include "CpuStateSave.h"
#include <Windows.h>
#include "ThreadsInfo.h"
#include "InHookThreadInfo.h"
#include "Collector.h"

ThreadsInfo<InHookThreadInfo> inHookThreadInfo;

void** CreateEventExAOldHandlerPtr = nullptr;
void* CreateEventExAOldHandler = nullptr;
__declspec(naked) void CreateEventExAHandler()
{
	HANDLE createEventResult;
	void** stackTopBeforeCall;//returnAddress
	SaveCpuState(simd_state1, ebx_state1, ecx_state1, loo1)
	__asm mov stackTopBeforeCall, ebp
	stackTopBeforeCall += 1;//ebp size
	inHookThreadInfo.AddThreadInfo({*stackTopBeforeCall});
	__asm mov ebx, stackTopBeforeCall
	__asm mov ecx, offset returnToMe
	__asm mov [ebx], ecx
	RestoreCpuState(simd_state1)
	__asm jmp CreateEventExAOldHandler
returnToMe:
	__asm call forPushNewRetAddress
forPushNewRetAddress:
	SaveCpuState(simd_state, ebx_state, ecx_state, loo)
	__asm mov createEventResult, eax
	__asm mov stackTopBeforeCall, ebp
	stackTopBeforeCall += 1;//ebp size
	*stackTopBeforeCall = inHookThreadInfo.GetThreadInfo().ReturnAddress;
	inHookThreadInfo.RemoveThreadInfo();
	CollectHandleInfo(createEventResult,CallType::CreateEventExA);
	RestoreCpuState(simd_state)
	__asm ret
}
void* CreateEventExAHandlerTmp = CreateEventExAHandler;
void** CreateEventExAHandlerPtr = &CreateEventExAHandlerTmp;

void** CreateEventExWOldHandlerPtr = nullptr;
void* CreateEventExWOldHandler = nullptr;
__declspec(naked) void CreateEventExWHandler()
{
	HANDLE createEventResult;
	void** stackTopBeforeCall;//returnAddress
	SaveCpuState(simd_state1, ebx_state1, ecx_state1, loo1)
	__asm mov stackTopBeforeCall, ebp
	stackTopBeforeCall += 1;//ebp size
	inHookThreadInfo.AddThreadInfo({ *stackTopBeforeCall });
	__asm mov ebx, stackTopBeforeCall
	__asm mov ecx, offset returnToMe
	__asm mov[ebx], ecx
	RestoreCpuState(simd_state1)
	__asm jmp CreateEventExWOldHandler
returnToMe :
	__asm call forPushNewRetAddress
forPushNewRetAddress :
	SaveCpuState(simd_state, ebx_state, ecx_state, loo)
	__asm mov createEventResult, eax
	__asm mov stackTopBeforeCall, ebp
	stackTopBeforeCall += 1;//ebp size
	*stackTopBeforeCall = inHookThreadInfo.GetThreadInfo().ReturnAddress;
	inHookThreadInfo.RemoveThreadInfo();
	CollectHandleInfo(createEventResult, CallType::CreateEventExW);
	RestoreCpuState(simd_state)
	__asm ret
}
void* CreateEventExWHandlerTmp = CreateEventExWHandler;
void** CreateEventExWHandlerPtr = &CreateEventExWHandlerTmp;

//====================================================================
void** CreateEventAOldHandlerPtr = nullptr;
void* CreateEventAOldHandler = nullptr;
__declspec(naked) void CreateEventAHandler()
{
	HANDLE createEventResult;
	void** stackTopBeforeCall;//returnAddress
	SaveCpuState(simd_state1, ebx_state1, ecx_state1, loo1)
	__asm mov stackTopBeforeCall, ebp
	stackTopBeforeCall += 1;//ebp size
	inHookThreadInfo.AddThreadInfo({ *stackTopBeforeCall });
	__asm mov ebx, stackTopBeforeCall
	__asm mov ecx, offset returnToMe
	__asm mov[ebx], ecx
	RestoreCpuState(simd_state1)
	__asm jmp CreateEventAOldHandler
returnToMe :
	__asm call forPushNewRetAddress
forPushNewRetAddress :
	SaveCpuState(simd_state, ebx_state, ecx_state, loo)
	__asm mov createEventResult, eax
	__asm mov stackTopBeforeCall, ebp
	stackTopBeforeCall += 1;//ebp size
	*stackTopBeforeCall = inHookThreadInfo.GetThreadInfo().ReturnAddress;
	inHookThreadInfo.RemoveThreadInfo();
	CollectHandleInfo(createEventResult, CallType::CreateEventA);
	RestoreCpuState(simd_state)
	__asm ret
}
void* CreateEventAHandlerTmp = CreateEventAHandler;
void** CreateEventAHandlerPtr = &CreateEventAHandlerTmp;

void** CreateEventWOldHandlerPtr = nullptr;
void* CreateEventWOldHandler = nullptr;
__declspec(naked) void CreateEventWHandler()
{
	HANDLE createEventResult;
	void** stackTopBeforeCall;//returnAddress
	SaveCpuState(simd_state1, ebx_state1, ecx_state1, loo1)
	__asm mov stackTopBeforeCall, ebp
	stackTopBeforeCall += 1;//ebp size
	inHookThreadInfo.AddThreadInfo({ *stackTopBeforeCall });
	__asm mov ebx, stackTopBeforeCall
	__asm mov ecx, offset returnToMe
	__asm mov[ebx], ecx
	RestoreCpuState(simd_state1)
	__asm jmp CreateEventWOldHandler
returnToMe :
	__asm call forPushNewRetAddress
forPushNewRetAddress :
	SaveCpuState(simd_state, ebx_state, ecx_state, loo)
	__asm mov createEventResult, eax
	__asm mov stackTopBeforeCall, ebp
	stackTopBeforeCall += 1;//ebp size
	*stackTopBeforeCall = inHookThreadInfo.GetThreadInfo().ReturnAddress;
	inHookThreadInfo.RemoveThreadInfo();
	CollectHandleInfo(createEventResult, CallType::CreateEventW);
	RestoreCpuState(simd_state)
	__asm ret
}
void* CreateEventWHandlerTmp = CreateEventWHandler;
void** CreateEventWHandlerPtr = &CreateEventWHandlerTmp;

//====================================================================
void** CloseHandleOldHandlerPtr = nullptr;
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
	CollectHandleInfo(closedHandle, CallType::CloseHandle);
	RestoreCpuState(simd_state)
	__asm jmp CloseHandleOldHandler
}
void* CloseHandleHandlerTmp = CloseHandleHandler;
void** CloseHandleHandlerPtr = &CloseHandleHandlerTmp;