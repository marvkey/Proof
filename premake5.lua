
workspace "Proof"
	architecture "x64"
	startproject "SandBox"
	configurations
	{

		"Debug",
		"Release",
		"Dist"
	}
	
	flags
	{
		"MultiProcessorCompile"
	}


OutputDirectory ="%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"
IncludeDir={}
IncludeDir["GLFW"]="%{wks.location}/Proof/vendor/glfw-3.3.2.bin.WIN64/include"
IncludeDir["GLEW"]="%{wks.location}/Proof/vendor/glew-2.1.0/include"
IncludeDir["ImGui"]="%{wks.location}/Proof/vendor/ImGUI"
IncludeDir["glm"]="%{wks.location}/Proof/vendor/glm"


project "Proof"
	location "Proof"
	kind "StaticLib"
	language "C++"
	cppdialect "C++17"

	targetdir ("bin/".. OutputDirectory .. "/%{prj.name}")
	objdir ("bin-int/".. OutputDirectory .. "/%{prj.name}")

    pchheader "Proofprch.h"
    pchsource "Proof/src/Proofprch.cpp"  -- only for vs studio

    files
	{
        "%{prj.name}/src/**.h",
        "%{prj.name}/src/**.cpp",
    }
    includedirs 
	{
		"%{wks.location}/Proof/src",
		"%{wks.location}/Proof/vendor/spdlog/include",
		"%{IncludeDir.GLEW}",
		"%{IncludeDir.GLFW}",
		"%{IncludeDir.ImGui}",
		"%{IncludeDir.glm}"
    }
	libdirs
	{
		"Proof/vendor/glfw-3.3.2.bin.WIN64/lib-vc2019",
		"Proof/vendor/glew-2.1.0/lib/Release/x64"
	}
	links
	{
		"glew32s.lib",
		"glfw3.lib",
		"opengl32.lib"
	}
    defines 
	{
		"PF_BUILD_DLL", -- checks if it is Dll
		"GLEW_STATIC",-- to use Glew
		"PF_PLATFORM_WINDOW64"
	}
	filter "system:windows"
		systemversion "latest"
		buildoptions "/MDd"
		
		postbuildcommands 
		{
			
			--("{COPY} %{cfg.buildtarget.relpath} ../bin/" ..OutputDirectory.."/SandBox") -- This copies dll file from Proof Into Sandbox
		}

	filter "configurations:Debug"
		defines "PF_DEBUG"
		symbols "on"

	filter "configurations:Release"
		defines "PF_RELEASE"
		optimize "on"

	filter "configurations:Dist"
		defines "PF_DIST"
		optimize "on"
			

        
    
project "SandBox"
	location "SandBox"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++17"

	targetdir ("bin/".. OutputDirectory .. "/%{prj.name}")
	objdir ("bin-int/".. OutputDirectory .. "/%{prj.name}")

	files 
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp",
	}

	includedirs
	{
		"%{wks.location}/Proof/src",
		"%{wks.location}/Proof/vendor/spdlog/include",
		"%{IncludeDir.GLEW}",
		"%{IncludeDir.GLFW}",
		"%{IncludeDir.ImGui}",
		"%{IncludeDir.glm}"

	}

	libdirs 
	{
		"%{wks.location}/Proof/vendor/glfw-3.3.2.bin.WIN64/lib-vc2019",
		"%{wks.location}/Proof/vendor/glew-2.1.0/lib/Release/x64",
	
	}
	
	links
	{

		"Proof",
		"glew32s.lib",
		"glfw3.lib",
		"opengl32.lib"
	}
	defines 
	{
		"GLEW_STATIC",
		"PF_PLATFORM_WINDOW64"
	}

	filter "system:windows"
		systemversion "latest"
		buildoptions "/MDd"

	filter "configurations:Debug"
		defines "PF_DEBUG"
		symbols "on"

	filter "configurations:Release"
		defines "PF_RELEASE"
		optimize "on"

	filter "configurations:Dist"
		defines "PF_DIST"
		optimize "on"		


