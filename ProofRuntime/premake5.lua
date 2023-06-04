--IF RECOMPILE ASSIMP WE WILL HAVE TO COPY NEW DLL AND LIB FILES INTO OUR DEFINED FILE FOLDER AND IF DLL COPY IT INTO EXECUTABLE
project "ProofRuntime"
    kind "ConsoleApp"
    language "C++"
    cppdialect "C++20"
    staticruntime "on"
    defines "PF_ENABLE_RUNTIME"

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
		"%{wks.location}/ProofRuntime/src",
		"%{IncludeDir.GLFW}",
		"%{IncludeDir.ImGui}",
		"%{IncludeDir.glm}",
		"%{IncludeDir.entt}",
		"%{IncludeDir.Vulkan}",
		"%{IncludeDir.Glad}",
		"%{IncludeDir.Log}",
		"%{IncludeDir.Assimp}",
        "%{IncludeDir.ImGuizmo}",
		"%{IncludeDir.yaml_cpp}",
		"%{IncludeDir.fmt}",
		"%{IncludeDir.physX}",
		"%{IncludeDir.physXfoundation}",
		"%{IncludeDir.optick}",
		"%{IncludeDir.magic_enum}",
		"%{IncludeDir.mono}",
    }

    links
    {
        "Proof",
        
    }
   
    filter "system:windows"
        systemversion "latest"
        defines "PF_PLATFORM_WINDOW64"
       

    filter "configurations:Debug"
        defines "PF_DEBUG"
        symbols "on"
        runtime "Debug"
        defines{
			"PF_ENABLE_ASSERT",
			"PF_ENABLE_DEBUG",
		}

    filter "configurations:Release"
        defines "PF_RELEASE"
        optimize "on"
        runtime "Release"
        defines{
			"PF_ENABLE_ASSERT",
			"PF_ENABLE_DEBUG",
		}

    filter "configurations:Dist"
        defines "PF_DIST"
        optimize "on"	
        runtime "Release"
