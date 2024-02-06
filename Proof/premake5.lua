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
		"src/Proof/Platform/OpenGl/**.h",
		"src/Proof/Platform/OpenGl/**.cpp",
		"src/Proof/Utils/samplerCPP/samplerBlueNoiseErrorDistribution_128x128_OptimizedFor_2d2d2d2d_2spp.cpp",
		"src/Proof/Utils/samplerCPP/samplerBlueNoiseErrorDistribution_128x128_OptimizedFor_2d2d2d2d_4spp.cpp",
		"src/Proof/Utils/samplerCPP/samplerBlueNoiseErrorDistribution_128x128_OptimizedFor_2d2d2d2d_8spp.cpp",
		"src/Proof/Utils/samplerCPP/samplerBlueNoiseErrorDistribution_128x128_OptimizedFor_2d2d2d2d_16spp.cpp",
		"src/Proof/Utils/samplerCPP/samplerBlueNoiseErrorDistribution_128x128_OptimizedFor_2d2d2d2d_32spp.cpp",
		"src/Proof/Utils/samplerCPP/samplerBlueNoiseErrorDistribution_128x128_OptimizedFor_2d2d2d2d_64spp.cpp",
		"src/Proof/Utils/samplerCPP/samplerBlueNoiseErrorDistribution_128x128_OptimizedFor_2d2d2d2d_128spp.cpp",
		"src/Proof/Utils/samplerCPP/samplerBlueNoiseErrorDistribution_128x128_OptimizedFor_2d2d2d2d_256spp.cpp",
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
		"%{IncludeDir.PhysX}",
		"%{IncludeDir.optick}",
		"%{IncludeDir.tracey}",
		"%{IncludeDir.magic_enum}",
		"%{IncludeDir.mono}",
		"%{IncludeDir.ktx}",
		"%{IncludeDir.msdfgen}",
		"%{IncludeDir.msdf_atlas_gen}",
		"%{IncludeDir.MiniAudio}",

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
		"OptickCore.lib",
		"SPIRV-Cross",
		"ProofScriptCore",
		"msdf-atlas-gen",
		"%{Library.Vulkan}",
		"%{Library.VulkanUtils}",
		"%{Library.mono}",
		"%{Library.PhysX}",
		"%{Library.PhysXCharacterKinematic}",
		"%{Library.PhysXCommon}",
		"%{Library.PhysXCooking}",
		"%{Library.PhysXExtensions}",
		"%{Library.PhysXFoundation}",
		"%{Library.PhysXPvd}",
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
		"PX_PHYSX_STATIC_LIB",
		
	}
	filter "files:vendor/ImGui/**.cpp"
	flags { "NoPCH" }
	filter "files:vendor/ImGuizmo/**.cpp"
	flags { "NoPCH" }
	filter "files:src/Platform/Vulkan/ktx/**.cpp"
	flags { "NoPCH" }
	filter "files:src/Proof/Utils/samplerCPP/**.cpp"
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
			"PF_ENABLE_DEBUG",
			"PF_TRACK_MEMORY"
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
			"NDEBUG",
			"PF_TRACK_MEMORY"
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

		