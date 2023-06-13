 #pragma once

#ifdef MGE_PLATFORM_WINDOWS
	#ifdef MGE_BUILD_DLL
		#define MGE_API __declspec(dllexport)
    #else
		#define MGE_API __declspec(dllimport)
	#endif
#else
	#error GME only supoorts windows
#endif

#define BIT(x) (1 << x)