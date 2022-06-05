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
        "%{wks.location}/Proof/vendor/entt",
		"%{IncludeDir.Vulkan}",
		"%{IncludeDir.fmt}",
        "%{wks.location}/Proof/vendor/optick/src"



		--"C:/Program Files/Mono/include/mono-2.0"
    }

    --filter { "system:windows", "configurations:Debug" }
      --  buildoptions "/MDd"        

    --filter { "system:windows", "configurations:Release" }
        --buildoptions "/MD"
    libdirs 
    {
        --"%{wks.location}/Game/Proof-Game/x64/Debug",
        "%{wks.location}/Proof/vendor/Assimp/Proof-Assimp-lib",
        "%{wks.location}/Proof/vendor/VulkanSDK/1.3.204.1/Lib",
        "%{wks.location}/Proof/vendor/optick/bin/vs2017/x64/Release"
		--"C:/Program Files/Mono/lib"

    }

    links
    {
        "Proof",
        "GLFW",
        "ImGui",
        "opengl32.lib",
        "Glad",
        "assimp-vc143-mt.lib",
        "yaml-cpp",
		--"mono-2.0-sgen.lib",
        --"ScriptModule.lib",
		"ProofPhysics",
        "vulkan-1.lib",
        "fmt",
		"OptickCore.lib"

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