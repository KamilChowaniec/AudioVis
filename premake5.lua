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
		"%{prj.name}/vendor/Engine/Engine/src",
		"%{prj.name}/vendor/Engine/Engine/vendor",
		"%{prj.name}/vendor/Engine/Engine/vendor/glm",
		"%{prj.name}/vendor/Engine/Engine/vendor/Glad/include",
		"%{prj.name}/vendor/Engine/Engine/vendor/GLFW/include",
		"%{prj.name}/vendor/Engine/Engine/vendor/spdlog/include",
		"%{prj.name}/vendor/Engine/Engine/vendor/SPSCQueue/include",
		"%{prj.name}/vendor/Engine/Engine/vendor/imgui",
		"%{prj.name}/vendor/BASS/c"
	}

	libdirs{
		"%{prj.name}/vendor/BASS/c/x64"
	}

	links {
		"Engine",
		"bass.lib"
	}

	defines {
		"PROJ_NAME=\"%{string.upper(prj.name)}\""
	}

	postbuildcommands {
		"{COPY} \"%{prj.location}vendor/BASS/x64/bass.dll\" \"%{cfg.targetdir}/\""
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