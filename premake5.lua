workspace "AudioVisualizer"
	architecture "x64"
	startproject "Visualizer"

	configurations { "Debug", "Release" }

	flags {
		"MultiProcessorCompile"
	}
	
	outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

group "Dependencies"
    include "Visualizer/vendor/Engine"

group ""

project "Visualizer"
	location "Visualizer"
	-- kind "WindowedApp"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++17"
	-- entrypoint "mainCRTStartup"


	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	files {
		"%{prj.name}/src/**.hpp",
		"%{prj.name}/src/**.cpp"
	}

	includedirs {
		"%{prj.name}/src",
		"Engine/src",
		"Engine/vendor",
		"Engine/vendor/glm",
		"Engine/vendor/Glad/include",
		"Engine/vendor/GLFW/include",
		"Engine/vendor/spdlog/include",
		"Engine/vendor/SPSCQueue/include",
		"Engine/vendor/imgui"
	}

	links {
		"Engine"
	}

	defines {
		"PROJ_NAME=\"%{string.upper(prj.name)}\""
	}

	filter "system:windows"
		systemversion "latest"

	filter "configurations:Debug"
		defines "DEBUG"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		defines "RELEASE"
		runtime "Release"
		optimize "on"