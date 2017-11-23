#pragma once
#include <windows.h>
#include <type_traits>
#include <functional>
#include <stdint.h>
#include <stdexcept>

void FlushCPUCodeCache()
{
	if (!FlushInstructionCache(GetCurrentProcess(), nullptr, 0))
		throw std::runtime_error("Can't flush CPU code cache. Than try make long jump?");
}

template<class FuncPtr1, class PtrToGlobalPtrToFunc2>
void replaceFunctionByMemoryPatch(FuncPtr1 replacedFunction, PtrToGlobalPtrToFunc2 functionReplacer, std::function<void(void**)> handlerOfOldPtrToJumpAbsoluteAddressOfOldHandler = std::function<void(void**)>())
{
	static_assert(::std::is_pointer<FuncPtr1>::value, "replacedFunction must be pointer to function" );
	static_assert(::std::is_pointer<PtrToGlobalPtrToFunc2>::value, "functionReplacer must be pointer (to global pointer to function)");
	using Func1 = typename std::remove_pointer<FuncPtr1>::type;
	using FuncGlobalPtr2 = typename std::remove_pointer<PtrToGlobalPtrToFunc2>::type;
	static_assert(::std::is_pointer<FuncGlobalPtr2>::value, "functionReplacer must be pointer to global pointer (to function)");
	using Func2 = typename std::remove_pointer<FuncGlobalPtr2>::type;
	static_assert(::std::is_function<Func1>::value, "replacedFunction must be pointer to function");
	static_assert(::std::is_void<Func2>::value, "functionReplacer must be pointer to global pointer to function, but function should be represented as void*");
#pragma pack (push)
#pragma pack (1)
	/*struct Relative32Jmp
	{
		unsigned uint8_t opCode : 8;
		uintptr_t relative32AddressForJump : 32;
	};
	using Jmp32Opcode = Relative32Jmp*/
	struct Jmp32Opcode
	{
		uint8_t opCode1;
		uint8_t opCode2;
		void** ptrToJumpAbsoluteAddress;
	};
#pragma pack (pop)

	DWORD OldAccessRights = 0;
	if (!VirtualProtect(replacedFunction, sizeof(Jmp32Opcode), PAGE_EXECUTE_READWRITE, &OldAccessRights))
		throw std::runtime_error("memory protection can't update");
	Jmp32Opcode* const replacedFunctionFirstBytes = reinterpret_cast<Jmp32Opcode*>(replacedFunction);
	enum { Jump32Opcode1 = 0xff, Jump32Opcode2 = 0x25 };
	if (handlerOfOldPtrToJumpAbsoluteAddressOfOldHandler)
	{
		if (!(replacedFunctionFirstBytes->opCode1 == Jump32Opcode1 && replacedFunctionFirstBytes->opCode2 == Jump32Opcode2))
			throw std::runtime_error("Unimplemented (unsupported yet) replacedFunction first bytes for getting old fixup table address");
		handlerOfOldPtrToJumpAbsoluteAddressOfOldHandler(replacedFunctionFirstBytes->ptrToJumpAbsoluteAddress);
	}
	replacedFunctionFirstBytes->opCode1 = Jump32Opcode1;
	replacedFunctionFirstBytes->opCode2 = Jump32Opcode2;
	replacedFunctionFirstBytes->ptrToJumpAbsoluteAddress = functionReplacer;
	/*enum { Relative32JumpOpcode = 0xE9 };
	replacedFunctionFirstBytes->opCode = Relative32JumpOpcode;
	replacedFunctionFirstBytes->relative32AddressForJump = reinterpret_cast<uintptr_t>(functionReplacer) - reinterpret_cast<uintptr_t>(replacedFunctionFirstBytes) - sizeof(Jmp32Opcode);*/
	DWORD dummy;
	if (!VirtualProtect(replacedFunction, sizeof(Jmp32Opcode), OldAccessRights, &dummy))
		throw std::runtime_error("memory protection can't final update");
	FlushCPUCodeCache();
}

/*template<class FuncPtr1>
void replaceFunctionByMemoryPatch(FuncPtr1 replacedFunction, void** functionReplacerPtrToGlobalPtr)
{
	static_assert(::std::is_pointer<FuncPtr1>::value, "replacedFunction must be pointer to function");
	using Func1 = typename std::remove_pointer<FuncPtr1>::type;
	static_assert(::std::is_function<Func1>::value, "replacedFunction must be pointer to function");
	struct Jmp32Opcode
	{
		uint8_t opCode1 : 8;
		uint8_t opCode2 : 8;
		uintptr_t ptrToJumpAbsoluteAddress : 32;
	};

	DWORD OldAccessRights = 0;
	if (!VirtualProtect(replacedFunction, sizeof(Jmp32Opcode), PAGE_EXECUTE_READWRITE, &OldAccessRights))
		throw std::runtime_error("memory protection can't update");
	Jmp32Opcode* const replacedFunctionFirstBytes = reinterpret_cast<Jmp32Opcode*>(replacedFunction);
	enum { Jump32Opcode1 = 0xff, Jump32Opcode2 = 0x25 };
	replacedFunctionFirstBytes->opCode1 = Jump32Opcode1;
	replacedFunctionFirstBytes->opCode2 = Jump32Opcode2;
	replacedFunctionFirstBytes->ptrToJumpAbsoluteAddress = reinterpret_cast<uintptr_t>(functionReplacer);
	DWORD dummy;
	if (!VirtualProtect(replacedFunction, sizeof(Jmp32Opcode), OldAccessRights, &dummy))
		throw std::runtime_error("memory protection can't final update");
	FlushCPUCodeCache();
}*/
