#pragma once
#include "windows.h"
#include "Errno.h"

class EnvStateSave
{
	DWORD _win32stateLastError;
	int _errnoLastError;

	static int get_errno()
	{
		int errnoValue = 0;
		_get_errno(&errnoValue);
		return errnoValue;
	}

public:
	EnvStateSave() 
		: _win32stateLastError(GetLastError())
		, _errnoLastError(get_errno())
	{
		SetLastError(0);
		_set_errno(0);
	}

	~EnvStateSave()
	{
		_set_errno(_errnoLastError);
		SetLastError(_win32stateLastError);
	}
};