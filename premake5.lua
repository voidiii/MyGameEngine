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
IncludeDir["Glad"] = "MyGameEngine/vendor/Glad/include"
IncludeDir["ImGui"] = "MyGameEngine/vendor/ImGui"
IncludeDir["Mather"] = "MyGameEngine/vendor/Math/Mather/include" -- this is what I added for Mather

include "MyGameEngine/vendor/GLFW"
include "MyGameEngine/vendor/Glad"
include "MyGameEngine/vendor/ImGui"

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
		"%{prj.name}/src/**.cpp",
		"%{prj.name}/vendor/Math/Mather/include/**.hpp" -- this is what I added for Mather
	}

	includedirs
	{
		"%{prj.name}/vendor/spdlog/include",
		"%{prj.name}/src",
		"%{prj.name}/src/MGE",
		"%{IncludeDir.GLFW}",
		"%{IncludeDir.Glad}",
		"%{IncludeDir.ImGui}",
		"%{IncludeDir.Mather}" -- this is what I added for Mather
	}

	links 
	{ 
		"GLFW",
		"Glad",
		"opengl32.lib",
		"ImGui"
	}

	filter "system:windows"
		cppdialect "C++17"
		staticruntime "On"
		systemversion "latest"

		defines
		{
			"MGE_BUILD_DLL",
			"MGE_PLATFORM_WINDOWS",
			"GLFW_INCLUDE_NONE" -- This is to prevent GLFW from including OpenGL headers which we already include in Glad so it might cause include conflicts
		}

		postbuildcommands
		{
			("{COPY} %{cfg.buildtarget.relpath} ../bin/" .. outputdir .. "/Sandbox")
		}

	filter "configurations:Debug"
		defines "MGE_DEBUG"
		buildoptions "/MDd"
		symbols "On"
	
	filter "configurations:Release"
		defines "MGE_RELEASE"
		buildoptions "/MD"
		symbols "On"

	filter "configurations:Dist"
		defines "MGE_DIST"
		buildoptions "/MD"
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
		"MyGameEngine/src",
		"%{IncludeDir.Mather}"
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
		buildoptions "/MDd"
		symbols "On"
	
	filter "configurations:Release"
		defines "MGE_RELEASE"
		buildoptions "/MD"
		symbols "On"

	filter "configurations:Dist"
		defines "MGE_DIST"
		buildoptions "/MD"
		symbols "On"