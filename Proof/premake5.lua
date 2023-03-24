--IF RECOMPILE ASSIMP WE WILL HAVE TO COPY NEW DLL AND LIB FILES INTO OUR DEFINED FILE FOLDER
project "Proof"
	kind "StaticLib"
	language "C++"
	cppdialect "C++20"
	staticruntime "on"

	targetdir ("bin/".. OutputDirectory .. "/%{prj.name}")
	objdir ("bin-int/".. OutputDirectory .. "/%{prj.name}")

	pchheader "Proofprch.h"
    pchsource "src/Proofprch.cpp"  -- only for vs studio

    files
	{
        "src/**.h",
        "src/**.cpp",
		--"vendor/ImGui/**.h",
        --"vendor/ImGui/**.cpp",
		"vendor/stb_image.h",
		"vendor/ImGuizmo/ImGuizmo.h",
		"vendor/ImGuizmo/ImGuizmo.cpp"

    }
	removefiles
	{
		"src/Platform/OpenGl/**.h",
		"src/Platform/OpenGl/**.cpp"
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
		"%{IncludeDir.physX}",
		"%{IncludeDir.physXfoundation}",
		"%{IncludeDir.optick}",
		"%{IncludeDir.magic_enum}",
		"%{IncludeDir.mono}",
		"%{IncludeDir.ktx}",
		"%{IncludeDir.msdfgen}",
		"%{IncludeDir.msdf_atlas_gen}",

    }
	libdirs{
        "%{wks.location}/Proof/vendor/VulkanSDK/Lib",
		"%{wks.location}/Proof/bin/".. OutputDirectory .. "/Proof"
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
		"ProofScriptCore",
		"msdf-atlas-gen",

		"%{wks.location}/proof/vendor/mono/lib/Debug/mono-2.0-sgen.lib",
		
		"%{Library.PhysX_static_64}",
		"%{Library.PhysXCharacterKinematic_static_64}",
		"%{Library.PhysXCommon_static_64}",
		"%{Library.PhysXCooking_static_64}",
		"%{Library.PhysXExtensions_static_64}",
		"%{Library.PhysXFoundation_static_64}",
		"%{Library.PhysXPvdSDK_static_64}",
		"%{Library.PhysXVehicle_static_64}",
		--c# dll stuff
		"OLDNAMES.lib",	
		"Ws2_32.lib",
		"Winmm.lib",
		"Version.lib",
		"Bcrypt.lib"
	}

    defines {
		"IMGUI_IMPL_OPENGL_LOADER_GLAD2",
		"GLFW_INCLUDE_NONE",
		"_CRT_SECURE_NO_WARNINGS",
	}
	filter "files:vendor/ImGui/**.cpp"
	flags { "NoPCH" }
	filter "files:vendor/ImGuizmo/**.cpp"
	flags { "NoPCH" }
	filter "files:src/Platform/Vulkan/ktx/**.cpp"
	flags { "NoPCH" }

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
			"%{Library.ShaderC_Debug}",
		}

	filter "configurations:Release"
		defines "PF_RELEASE"
		optimize "on"
		runtime "Release"

		defines{
			"PF_ENABLE_ASSERT",
			"PF_ENABLE_DEBUG",
			"NDEBUG"
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

		