#include "stdafx.h"
#include "HookStackTracerSettings.h"

HookStackTracerSettings GetHookStackTracerSettings()
{
	return{ "e:\\TracerDb.bin", "e:\\TracerResults.log", "e:\\TracerConclusion.log", nullptr };//to do: Implement loading from settings file and caching settings.
}