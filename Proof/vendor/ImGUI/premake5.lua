project "ImGui"
	kind "StaticLib"
	language "C++"
	staticruntime "on"
	targetdir ("bin/" .. OutputDirectory .. "/%{prj.name}")
	objdir ("bin-int/" .. OutputDirectory .. "/%{prj.name}")

	files
	{
		"ImGui/**.h",
		"ImGui/**.cpp",
		"misc/cpp/**.h",
		"misc/cpp/**.cpp",
	}
    includedirs 
	{
		"%{IncludeDir.GLFW}",
		"%{IncludeDir.Vulkan}",
	}
	filter "system:windows"
		systemversion "latest"
		cppdialect "C++17"

	filter "system:linux"
		pic "On"
		systemversion "latest"
		cppdialect "C++17"
		staticruntime "On"

	filter "configurations:Debug"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		runtime "Release"
		optimize "on"
