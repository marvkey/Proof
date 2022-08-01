--IF RECOMPILE ASSIMP WE WILL HAVE TO COPY NEW DLL AND LIB FILES INTO OUR DEFINED FILE FOLDER AND IF DLL COPY IT INTO EXECUTABLE
project "Proof-Editor"
    kind "ConsoleApp"
    language "C++"
    cppdialect "C++17"
    staticruntime "on"

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
		"%{IncludeDir.Bullet3}",
        "%{wks.location}/Proof/vendor/optick/src",
        "%{wks.location}/Proof/vendor/PhysX/physx/include",
        "%{wks.location}/Proof/vendor/PhysX/physx/source/physxextensions/src/**",
        "%{wks.location}/Proof/vendor/PhysX/physx/source/foundation/include/**",
        
		"%{wks.location}/Proof/vendor/PhysX/pxshared/include",
        "%{wks.location}/Proof/vendor/magic_enum/include",

		--"C:/Program Files/Mono/include/mono-2.0"
    }
    includedirs { 
        "%{wks.location}/Proof/vendor/PhysX/physx/source/**", 
        "%{wks.location}/Proof/vendor/PhysX/physx", 
        "physx/include",
        "%{wks.location}/physx/source/physx/src/**",
    }
    --filter { "system:windows", "configurations:Debug" }
      --  buildoptions "/MDd"        

    --filter { "system:windows", "configurations:Release" }
        --buildoptions "/MD"
    libdirs {
        --"%{wks.location}/Game/Proof-Game/x64/Debug",
        "%{wks.location}/Proof/vendor/Assimp/Proof-Assimp-lib",
        "%{wks.location}/Proof/vendor/VulkanSDK/1.3.204.1/Lib",
        "%{wks.location}/Proof/vendor/optick/bin/vs2017/x64/Release",
		"%{wks.location}/Proof/vendor/PhysX/physx/bin/win.x86_64.vc142.mt/debug", ----very very very important we change this up
		"%{wks.location}/Proof/vendor/bullet3/bin",
        "%{wks.location}/Proof/vendor/VulkanMemoryAllocator/build/src/Release"
        
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
		"ProofPhysics",
        "vulkan-1.lib",
		"OptickCore.lib",
        "SPIRV-Cross",
        --"VulkanMemoryAllocator.lib",
        --"physx",
        --"PhysXFoundation_64.lib",
        --"PhysXCooking_64.lib",
        --"PhysXCommon_64.lib",
        --"PhysX_64.lib",
        --"PhysXExtensions_static_64.lib",    
       -- "PhysXPvdSDK_static_64.lib",

        "LinearMath_vs2010_x64_debug.lib",
        "BulletCollision_vs2010_x64_debug.lib",
        "BulletDynamics_vs2010_x64_debug.lib",
    }
   links --physx
  {
       
    "SnippetUtils_static_64.lib",
    "LowLevel_static_64.lib",
    "LowLevelAABB_static_64.lib",
    "LowLevelDynamics_static_64.lib",
    "PhysX_64.lib",
    "PhysXCharacterKinematic_static_64.lib",
    "PhysXCommon_64.lib",
    "PhysXCooking_64.lib",
    "PhysXExtensions_static_64.lib",
    "PhysXFoundation_64.lib",
    "PhysXPvdSDK_static_64.lib",
    "PhysXTask_static_64.lib",
    "PhysXVehicle_static_64.lib",
    "SceneQuery_static_64.lib",
    "SimulationController_static_64.lib",
    "SnippetRender_static_64.lib",
    }

    filter "system:windows"
        systemversion "latest"
        defines {
            "PF_PLATFORM_WINDOW64"
        }


    filter "configurations:Debug"
        defines "PF_DEBUG"
        symbols "on"
        runtime "Debug"
        defines{
			"PF_ENABLE_ASSERT",
			"PF_ENABLE_DEBUG"
		}
        links
		{
            "%{Library.ShaderC_Release}",
		}

    filter "configurations:Release"
        defines "PF_RELEASE"
        optimize "on"
        runtime "Release"
        defines{
			"PF_ENABLE_ASSERT",
			"PF_ENABLE_DEBUG"
		}
        links
		{
			"%{Library.ShaderC_Release}",
		}

    filter "configurations:Dist"
        defines "PF_DIST"
        optimize "on"	
        runtime "Release"
        links
		{
			"%{Library.ShaderC_Release}",
		}
