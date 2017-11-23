#pragma once 
#include <string>

struct HookStackTracerSettings
{
	std::string TracerDbPath;
	std::string TracerResultsLogPath;
	std::string TracerConclusionLogPath;
	const char* SymbolSearchPaths;
};

HookStackTracerSettings GetHookStackTracerSettings();