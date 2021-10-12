--IF RECOMPILE ASSIMP WE WILL HAVE TO COPY NEW DLL AND LIB FILES INTO OUR DEFINED FILE FOLDER AND IF DLL COPY IT INTO EXECUTABLE
project "Proof-Editor"
    kind "ConsoleApp"
    language "C++"
    cppdialect "C++17"
    staticruntime "off"

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
        "%{IncludeDir.GLFW}",
        "%{IncludeDir.ImGui}",
        "%{IncludeDir.glm}",
        "%{wks.location}/SandBox/src",
        "%{IncludeDir.entt}",
        "%{IncludeDir.Glad}",
		"%{IncludeDir.Log}",
		"%{IncludeDir.Assimp}",
        "%{IncludeDir.ImGuizmo}",
		"%{IncludeDir.yaml_cpp}",
    }

    --filter { "system:windows", "configurations:Debug" }
      --  buildoptions "/MDd"        

    --filter { "system:windows", "configurations:Release" }
        --buildoptions "/MD"
    libdirs 
    {
        --"%{wks.location}/Game/Proof-Game/x64/Debug",
        "%{wks.location}/Proof/vendor/Assimp/Proof-Assimp-lib",

    }

    links
    {
        "Proof",
        "GLFW",
        "ImGui",
        "opengl32.lib",
        "Glad",
        "assimp-vc142-mt.lib",
        --"ScriptModule.lib",
        "yaml-cpp",
		--"ProofPhysics"
    }
   
    postbuildcommands{

       -- ("{COPY} %{wks.location}/Proof/vendor/Assimp/Proof-Assimp-lib"..OutputDirectory.."/Proof-Editor")
    }

    filter "system:windows"
        systemversion "latest"
        --buildoptions "/MDd"
        defines {
            "PF_PLATFORM_WINDOW64"
        }


    filter "configurations:Debug"
        defines "PF_DEBUG"
        symbols "on"
        defines{
			"PF_ENABLE_ASSERT",
			"PF_ENABLE_DEBUG"
		}

    filter "configurations:Release"
        defines "PF_RELEASE"
        optimize "on"
        defines{
			"PF_ENABLE_ASSERT",
			"PF_ENABLE_DEBUG"
		}

    filter "configurations:Dist"
        defines "PF_DIST"
        optimize "on"	