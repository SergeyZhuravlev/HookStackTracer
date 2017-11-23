#pragma once
#include <fstream>
#include <share.h>

class Logger
{
private:
	std::ofstream _file;

public:
	Logger(Logger& path) = delete;
	Logger(Logger&& path) = delete;
	Logger() = delete;
	Logger& operator=(Logger&) = delete;
	Logger& operator=(Logger&&) = delete;

	Logger(const std::string& path)
		: _file(path, std::ios::out | std::ios::trunc, _SH_DENYWR)
	{
		if (!_file.is_open())
			throw std::runtime_error("Can't open log '" + path + "'");
		_file.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	}

	void Write(const std::string& buf)
	{
		_file << buf;
		_file.flush();
	}

	void WriteLine(const std::string& buf)
	{
		_file << buf << std::endl;
		//_file.flush();
	}

	void Info(const std::string& buf)
	{
		WriteLine("Info: " + buf);
	}

	void Error(const std::string& buf)
	{
		WriteLine("Error: " + buf);
	}
};