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

defines { "IMGUI_API=__declspec(dllexport)" }
IncludeDir = {}
IncludeDir["GLFW"] = "MyGameEngine/vendor/GLFW/include"
IncludeDir["Glad"] = "MyGameEngine/vendor/Glad/include"
IncludeDir["ImGui"] = "MyGameEngine/vendor/ImGui"
IncludeDir["Mathter"] = "MyGameEngine/vendor/Math/Mather/include/Mathter" -- this is what I added for Mather
IncludeDir["stb_image"] = "MyGameEngine/vendor/stb_image"

include "MyGameEngine/vendor/GLFW"
include "MyGameEngine/vendor/Glad"
include "MyGameEngine/vendor/ImGui"

project "MyGameEngine"
	location "MyGameEngine"
	kind "StaticLib"
	language "C++"
	cppdialect "C++17"
	staticruntime "on"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	pchheader "MGEpch.h"
	pchsource "MyGameEngine/src/MGEpch.cpp"

	files
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp",
		"%{prj.name}/vendor/Math/Mather/include/**.hpp", -- this is what I added for Mather
		"%{prj.name}/vendor/stb_image/**.h",
		"%{prj.name}/vendor/stb_image/**.cpp"
	}

	includedirs
	{
		"%{prj.name}/vendor/spdlog/include",
		"%{prj.name}/src",
		"%{prj.name}/src/MGE",
		"%{IncludeDir.GLFW}",
		"%{IncludeDir.Glad}",
		"%{IncludeDir.ImGui}",
		"%{IncludeDir.Mathter}", -- this is what I added for Mather
		"%{IncludeDir.stb_image}"
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
	cppdialect "C++17"
	staticruntime "on"

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
		"%{IncludeDir.Mathter}",
		"%{IncludeDir.ImGui}"
	}

	links
	{
		"MyGameEngine",
		--"ImGui" -- I added this 
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