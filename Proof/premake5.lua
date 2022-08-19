--IF RECOMPILE ASSIMP WE WILL HAVE TO COPY NEW DLL AND LIB FILES INTO OUR DEFINED FILE FOLDER
project "Proof"
	kind "StaticLib"
	language "C++"
	cppdialect "C++17"
	staticruntime "on"

	targetdir ("bin/".. OutputDirectory .. "/%{prj.name}")
	objdir ("bin-int/".. OutputDirectory .. "/%{prj.name}")

	pchheader "Proofprch.h"
    pchsource "src/Proofprch.cpp"  -- only for vs studio


    files
	{
        "src/**.h",
        "src/**.cpp",
		"vendor/ImGui/ImGui/**.h",
        "vendor/ImGui/ImGui/**.cpp",
		"vendor/stb_image.h",
		"vendor/ImGuizmo/ImGuizmo.h",
		"vendor/ImGuizmo/ImGuizmo.cpp"

    }
    includedirs 
	{
		"%{wks.location}/Proof/src",
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
		"%{IncludeDir.Bullet3}",
		"%{wks.location}/Proof/vendor/PhysX/physx/include",
		"%{wks.location}/Proof/vendor/PhysX/pxshared/include",
        "%{wks.location}/Proof/vendor/PhysX/physx/**",
		"%{wks.location}/Proof/vendor/PhysX/physx/source/physxextensions/src/**",
        "%{wks.location}/Proof/vendor/PhysX/physx/source/foundation/include/**",
        "%{wks.location}/Proof/vendor/entt",
        "%{wks.location}/Proof/vendor/optick/src",
        "%{wks.location}/Proof/vendor/magic_enum/include",
        "%{wks.location}/Proof/vendor/mono/include",
		"{IncludeDir.mono}"

    }
	includedirs { 
        "%{wks.location}/Proof/vendor/PhysX/physx/source/**", 
        "%{wks.location}/Proof/vendor/PhysX/physx", 
        "physx/include",
        "%{wks.location}/physx/source/physx/src/**",
    }
	libdirs{
		"vendor/Assimp/Proof-Assimp-lib",
        "%{wks.location}/Proof/vendor/VulkanSDK/1.3.204.1/Lib",
		"%{wks.location}/Proof/vendor/optick/bin/vs2017/x64/Release",
		"%{wks.location}/Proof/vendor/PhysX/physx/bin/win.x86_64.vc142.mt/debug",
		"%{wks.location}/Proof/vendor/bullet3/bin"
	}
	links
	{	
		"GLFW",
		"opengl32.lib",
		"ImGui",
		"Glad",
        "assimp-vc143-mt.lib",
		"yaml-cpp",
		"ProofPhysics",
        "vulkan-1.lib",
		"OptickCore.lib",
		"SPIRV-Cross",
		"LinearMath_vs2010_x64_debug.lib",
        "BulletCollision_vs2010_x64_debug.lib",
        "BulletDynamics_vs2010_x64_debug.lib",
		"%{wks.location}/proof/vendor/mono/lib/Debug/mono-2.0-sgen.lib",
		"ProofScriptCore",
		"OLDNAMES.lib",	
		"Ws2_32.lib",
		"Winmm.lib",
		"Version.lib",
		"Bcrypt.lib"
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
    defines {
		"STB_IMAGE_IMPLEMENTATION",
		"IMGUI_IMPL_OPENGL_LOADER_GLAD2",
		"GLFW_INCLUDE_NONE",
		"_CRT_SECURE_NO_WARNINGS",
	}
	filter "files:vendor/ImGui/**.cpp"
	flags { "NoPCH" }
	filter "files:vendor/ImGuizmo/**.cpp"
	flags { "NoPCH" }

	filter "system:windows"
		systemversion "latest"
		defines {
			"PF_PLATFORM_WINDOW64"
		}
		postbuildcommands 
		{
			
			--("{COPY} %{cfg.buildtarget.relpath} ../bin/" ..OutputDirectory.."/SandBox") -- This copies dll file from Proof Into Sandbox
			
		}

	filter "configurations:Debug"
		defines "PF_DEBUG"
		symbols "on"
		--runtime "Debug"
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

		