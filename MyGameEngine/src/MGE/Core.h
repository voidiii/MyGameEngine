 #pragma once

#ifdef MGE_PLATFORM_WINDOWS
#if MGE_DYNAMIC_LINK
	#ifdef MGE_BUILD_DLL
		#define MGE_API __declspec(dllexport)
    #else
		#define MGE_API __declspec(dllimport)
	#endif
#else
	#define MGE_API
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

// This creates a function object that, when called (the whole macro would be a function), will invoke x on this Application object.
#define MGE_BIND_EVENT_FUNCTION(x) std::bind(&x, this, std::placeholders::_1)