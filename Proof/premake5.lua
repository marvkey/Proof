--IF RECOMPILE ASSIMP WE WILL HAVE TO COPY NEW DLL AND LIB FILES INTO OUR DEFINED FILE FOLDER
project "Proof"
	kind "StaticLib"
	language "C++"
	cppdialect "C++17"

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
		--"%{IncludeDir.Vulkan}",
		"%{IncludeDir.Glad}",
		"%{IncludeDir.Log}",
		"%{IncludeDir.Assimp}",
        "%{IncludeDir.ImGuizmo}",
		"%{IncludeDir.yaml_cpp}",
        "%{wks.location}/Proof/vendor/entt"

		--"C:/Program Files/Mono/include/mono-2.0"
    }
	libdirs{
		"vendor/Assimp/Proof-Assimp-lib",
		--"C:/Program Files/Mono/lib"
	}
	links
	{	
		"GLFW",
		"opengl32.lib",
		"ImGui",
		"Glad",
        "assimp-vc143-mt.lib",
		"yaml-cpp",
		"ProofPhysics"
		--"mono-2.0-sgen.lib",

		--"ProofPhysics"
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
		--buildoptions "/MDd"
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
		