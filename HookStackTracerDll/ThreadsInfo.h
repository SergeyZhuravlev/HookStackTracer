#pragma once 
#include <Windows.h>
#include <unordered_map>

//to do: Implement with windows heap allocator and reference counter for double AddThreadInfo for current thread, because events can be reñursive

struct ThreadInfo
{
	void* ReturnAddress;
};

class ThreadsInfo
{
private:
	mutable CRITICAL_SECTION _selfProtector;
	std::unordered_map<DWORD, ThreadInfo> _infos;

public:
	ThreadsInfo(ThreadsInfo&) = delete;
	ThreadsInfo& operator=(ThreadsInfo&) = delete;

	ThreadsInfo()
	{
		InitializeCriticalSectionAndSpinCount(&_selfProtector, 0);
	}

	~ThreadsInfo()
	{
		DeleteCriticalSection(&_selfProtector);
	}

	void AddThreadInfo(const ThreadInfo& info)
	{
		EnterCriticalSection(&_selfProtector);
		_infos[GetCurrentThreadId()] = info;
		LeaveCriticalSection(&_selfProtector);
	};

	const ThreadInfo& GetThreadInfo() const
	{
		EnterCriticalSection(&_selfProtector);
		DWORD threadId = GetCurrentThreadId();
		auto result = _infos.find(threadId);
		if (result == _infos.cend())
			TerminateProcess(GetCurrentProcess(), 100500);
		LeaveCriticalSection(&_selfProtector);
		return result->second;
	}

	void RemoveThreadInfo()
	{
		EnterCriticalSection(&_selfProtector);
		DWORD threadId = GetCurrentThreadId();
		auto finded = _infos.find(threadId);
		if (finded != _infos.cend())
			_infos.erase(finded);
		LeaveCriticalSection(&_selfProtector);
	}
};