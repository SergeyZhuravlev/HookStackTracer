#pragma once 
#include <unordered_map>
#include "..\HSTCommon\CriticalSectionBasicLocable.h"

//to do: Implement windows heap allocator for std::unordered_map and reference counter for double AddThreadInfo for current thread, because events can be reñursive
template<class ThreadInfo>
class ThreadsInfo
{
private:
	std::unordered_map<DWORD, ThreadInfo> _infos;
	mutable CriticalSectionBasicLocable _guard;

public:

	void AddThreadInfo(const ThreadInfo& info)
	{
		decltype(_guard)::Locker locker(_guard);
		DWORD threadId = GetCurrentThreadId();
		_infos[threadId] = info;
	};

	const ThreadInfo& GetThreadInfo() const
	{
		decltype(_guard)::Locker locker(_guard);
		DWORD threadId = GetCurrentThreadId();
		auto result = _infos.find(threadId);
		if (result == _infos.cend())
			/*std::abort();*/ TerminateProcess(GetCurrentProcess(), 100500);
		return result->second;
	}

	void RemoveThreadInfo()
	{
		decltype(_guard)::Locker locker(_guard);
		DWORD threadId = GetCurrentThreadId();
		auto finded = _infos.find(threadId);
		if (finded != _infos.cend())
			_infos.erase(finded);
	}
};