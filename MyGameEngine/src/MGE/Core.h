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

#ifdef MGE_ENABLE_ASSERTS
	#define MGE_ASSERT(x, ...) { if(!(x)) { MGE_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak(); } }
	#define MGE_CORE_ASSERT(x, ...) { if(!(x)) { MGE_CORE_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak(); } }
#else
	#define MGE_ASSERT(x, ...)
	#define MGE_CORE_ASSERT(x, ...)
#endif


#define BIT(x) (1 << x)