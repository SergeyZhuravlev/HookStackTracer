#pragma once
#include <Windows.h>
#include <mutex>

class CriticalSectionBasicLocable
{
private:
	mutable CRITICAL_SECTION _protector;

public:
	using Locker = std::lock_guard<CriticalSectionBasicLocable>;

	CriticalSectionBasicLocable(CriticalSectionBasicLocable&) = delete;
	CriticalSectionBasicLocable& operator=(CriticalSectionBasicLocable&) = delete;
	CriticalSectionBasicLocable& operator=(CriticalSectionBasicLocable&&) = delete;

	CriticalSectionBasicLocable()
	{
		InitializeCriticalSectionAndSpinCount(&_protector, 0);
	}

	virtual ~CriticalSectionBasicLocable()
	{
		DeleteCriticalSection(&_protector);
	}

	void lock()
	{
		EnterCriticalSection(&_protector);
	}

	void unlock()
	{
		LeaveCriticalSection(&_protector);
	}
};
