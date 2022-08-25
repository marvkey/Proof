project "SandBox"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++20"

	targetdir ("bin/".. OutputDirectory .. "/%{prj.name}")
	objdir ("bin-int/".. OutputDirectory .. "/%{prj.name}")

	files 
	{
		"src/**.h",
		"src/**.cpp",
		
	}

	includedirs
	{
		"%{wks.location}/Proof/src",
		"%{wks.location}/Proof/vendor/spdlog/include",
		--"%{IncludeDir.GLEW}",
		"%{IncludeDir.GLFW}",
		"%{IncludeDir.ImGui}",
		"%{IncludeDir.glm}",
		"%{wks.location}/SandBox/src",
        "%{IncludeDir.Glad}"
	}

	libdirs 
	{
		"%{wks.location}/Proof/vendor/glfw-3.3.2.bin.WIN64/lib-vc2019",
		"%{wks.location}/Proof/vendor/glew-2.1.0/lib/Release/x64",
		"%{wks.location}/Proof/vendor/Vulkan/Lib"
	
	}
	
	links
	{

	    "Proof",
		"glfw3.lib",
		"opengl32.lib",
		--"vulkan-1.lib"
	}
	defines 
	{
		--"GLEW_STATIC",
		"PF_PLATFORM_WINDOW64",
	}

	filter "system:windows"
		systemversion "latest"
		buildoptions "/MDd"
        defines 
        {
            "PF_PLATFORM_WINDOW64",
        }
    
	filter "configurations:Debug"
		defines "PF_DEBUG"
		symbols "on"
		defines {
			"PF_ENABLE_ASSERT"
		}

	filter "configurations:Release"
		defines "PF_RELEASE"
		optimize "on"

	filter "configurations:Dist"
		defines "PF_DIST"
		optimize "on"		
		defines {
			"PF_ENABLE_ASSERT"
		}
