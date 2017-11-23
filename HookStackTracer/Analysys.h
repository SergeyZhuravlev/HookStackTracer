#pragma once
#include "..\HSTCommon\TracerDb.h"
#include "CallInfoExtended.h"

class Analysys
{
	TracerDb& _tracerDb;

public:
	Analysys(TracerDb* const tracerDb)
		:_tracerDb(*tracerDb)
	{}

	void Execute()
	{
		auto recordsAmount = _tracerDb.ReadRecordsAmount();
		_tracerDb.BeginReading();
		while(recordsAmount>0)
		{
			--recordsAmount;
			CallInfoExtended info;
			_tracerDb.Read(info.callInfo);
			decode(info)
			analysys(info);
		}
	}

private:
	void analysys(const CallInfoExtended& info)
	{
		
	}

	void decode(CallInfoExtended& info) const
	{
		
	}
};