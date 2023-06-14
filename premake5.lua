workspace "MyGameEngine"
	architecture "x64"


	configurations
	{
		"Debug",
		"Release",
		"Dist"
	}

outputdir = "${cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

-- Include directories relative to root folder (solution directory)
IncludeDir = {}
IncludeDir["GLFW"] = "MyGameEngine/vendor/GLFW/include"

include "MyGameEngine/vendor/GLFW"

project "MyGameEngine"
	location "MyGameEngine"
	kind "SharedLib"
	language "C++"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	pchheader "MGEpch.h"
	pchsource "MyGameEngine/src/MGEpch.cpp"

	files
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp"
	}

	includedirs
	{
		"%{prj.name}/vendor/spdlog/include",
		"%{prj.name}/src",
		"%{prj.name}/src/MGE",
		"%{IncludeDir.GLFW}"
	}

	links 
	{ 
		"GLFW",
		"opengl32.lib"
	}

	filter "system:windows"
		cppdialect "C++17"
		staticruntime "On"
		systemversion "latest"

		defines
		{
			"MGE_BUILD_DLL",
			"MGE_PLATFORM_WINDOWS"
		}

		postbuildcommands
		{
			("{COPY} %{cfg.buildtarget.relpath} ../bin/" .. outputdir .. "/Sandbox")
		}

	filter "configurations:Debug"
		defines "MGE_DEBUG"
		symbols "On"
	
	filter "configurations:Release"
		defines "MGE_RELEASE"
		symbols "On"

	filter "configurations:Dist"
		defines "MGE_DIST"
		symbols "On"

project "Sandbox"		
	
	location "Sandbox"
	kind "ConsoleApp"
	language "C++"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	files
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp"
	}

	includedirs
	{
		"MyGameEngine/vendor/spdlog/include",
		"MyGameEngine/src"
	}

	links
	{
		"MyGameEngine"
	}

	filter "system:windows"
		cppdialect "C++17"
		staticruntime "On"
		systemversion "latest"

		defines
		{
			"MGE_PLATFORM_WINDOWS"
		}

	filter "configurations:Debug"
		defines "MGE_DEBUG"
		symbols "On"
	
	filter "configurations:Release"
		defines "MGE_RELEASE"
		symbols "On"

	filter "configurations:Dist"
		defines "MGE_DIST"
		symbols "On"