#pragma once 
#include <string>

struct HookStackTracerSettings
{
	std::string TracerDbPath;
	std::string TracerResultsLogPath;
	std::string TracerConclusionLogPath;
};

HookStackTracerSettings GetHookStackTracerSettings();