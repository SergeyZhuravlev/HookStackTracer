#pragma once

#ifdef HOOKSTACKTRACERDLL_EXPORTS
	#define HOOKSTACKTRACERDLL_API __declspec(dllexport)
#else
	#define HOOKSTACKTRACERDLL_API __declspec(dllimport)
#endif