#pragma once
#include <stdint.h>

#define NakedFunctionProlog() \
	__asm push ebp \
	__asm mov ebp, esp \
	__asm sub esp, __LOCAL_SIZE

#define NakedFunctionEpilog() \
	__asm mov esp, ebp \
	__asm pop ebp

//__declspec(align(16)) char simd_state[512];
//Make naked function stack frame and save cpu state
#define SaveCpuState(simd_state_length, ebx_save, ecx_save, internalLoop) \
uintptr_t simd_state_length; \
uintptr_t ebx_save; \
uintptr_t ecx_save; \
	NakedFunctionProlog()\
	__asm pushfd \
	__asm push ss \
	/*pop cs*/ \
	__asm push ds \
	__asm push es \
	__asm push fs \
	__asm push gs \
	__asm pushad \
	/*calculate simd state aligned 16 length: */ \
	__asm mov simd_state_length, 512 \
	__asm mov ebx_save, ebx \
	__asm mov ecx_save, ecx \
	__asm internalLoop: \
	__asm add simd_state_length, 4 \
	__asm mov ebx, esp \
	__asm sub ebx, simd_state_length \
	__asm mov ecx, ebx \
	__asm shr ecx, 4 \
	__asm shl ecx, 4 \
	__asm cmp ecx, ebx \
	__asm jne internalLoop \
	__asm mov esp, ebx \
	/*push simd\fpu\mmx state :*/ \
	__asm fxsave byte ptr [ebx] \
	__asm mov ebx, ebx_save \
	__asm mov ecx, ecx_save


//Drop naked function stack frame and restore cpu state
#define RestoreCpuState(simd_state_length) \
	__asm mov ebx, esp \
	__asm fxrstor byte ptr [ebx] \
	__asm add ebx, simd_state_length \
	__asm mov esp, ebx \
	__asm popad \
	__asm pop gs \
	__asm pop fs \
	__asm pop es \
	__asm pop ds \
	/*pop cs*/ \
	__asm pop ss \
	__asm popfd \
	NakedFunctionEpilog()

