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

#ifdef HZ_ENABLE_ASSERTS
	#define HZ_ASSERT(x, ...) { if(!(x)) { HZ_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak(); } }
	#define HZ_CORE_ASSERT(x, ...) { if(!(x)) { HZ_CORE_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak(); } }
#else
	#define HZ_ASSERT(x, ...)
	#define HZ_CORE_ASSERT(x, ...)
#endif


#define BIT(x) (1 << x)