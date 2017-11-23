#pragma once 
#include "CriticalSectionBasicLocable.h"
#include <fstream>
#include <share.h>
#include "HookStackTracerSettings.h"
#include "CallInfo.h"

class TracerDb
{
public:
	using RecordPosition = std::streamsize;

private:
	mutable CriticalSectionBasicLocable _protector;
	HookStackTracerSettings _settings;
	std::fstream _db;
	const std::string _headerSign = "TracerDb 1.0";

	void writeRecordsAmount(RecordPosition amount)
	{
		decltype(_protector)::Locker lock(_protector);
		_db.seekp(_headerSign.size(), std::ios::beg);
		_db.write((char*)&amount, sizeof(amount));
		_db.flush();
	}

	void increaseRecordsAmount()
	{
		decltype(_protector)::Locker lock(_protector);
		writeRecordsAmount(ReadRecordsAmount() + 1);
	}

public:
	void OpenWrite()
	{
		decltype(_protector)::Locker lock(_protector);
		_settings = GetHookStackTracerSettings();
		_db.open(_settings.TracerDbPath, std::ios::binary | std::ios::out | std::ios::in | std::ios::trunc, _SH_DENYWR);
		if (!_db.is_open())
			throw std::runtime_error("Can't create TracerDb file '" + _settings.TracerDbPath + "'");
		_db.exceptions(std::ifstream::failbit | std::ifstream::badbit);
		_db.write((char*)_headerSign.data(), _headerSign.size());
		writeRecordsAmount(0);
	}

	void OpenRead()
	{
		decltype(_protector)::Locker lock(_protector);
		_settings = GetHookStackTracerSettings();
		_db.open(_settings.TracerDbPath, std::ios::binary | std::ios::in, _SH_DENYNO);
		if (!_db.is_open())
			throw std::runtime_error("Can't create OutfstreamTrace file " + _settings.TracerDbPath);
		_db.exceptions(std::ifstream::failbit | std::ifstream::badbit);
		BeginReading();
	}

	void BeginReading()
	{
		decltype(_protector)::Locker lock(_protector);
		_db.seekg(0, std::ios::beg);
		std::string sign(_headerSign.size(), ' ');
		_db.read((char*)sign.data(), sign.size());
		if (sign != _headerSign)
			throw std::runtime_error("Db version unsupported");
		ReadRecordsAmount();
	}

	void Close()
	{
		decltype(_protector)::Locker lock(_protector);
		_db.close();
	}

	void Write(const CallInfo& callInfo)
	{
		decltype(_protector)::Locker lock(_protector);
		_db.seekp(0, std::ios::end);
		_db.write((char*)&callInfo, sizeof(callInfo));
		increaseRecordsAmount();
		_db.flush();
	}

	void Read(CallInfo& callInfo)
	{
		decltype(_protector)::Locker lock(_protector);
		_db.read((char*)&callInfo, sizeof(callInfo));
	}

	RecordPosition ReadRecordsAmount()
	{
		decltype(_protector)::Locker lock(_protector);
		_db.seekg(_headerSign.size(), std::ios::beg);
		RecordPosition result;
		_db.read((char*)&result, sizeof(result));
		return result;
	}
};