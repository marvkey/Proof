project "ProofPhysics"
    kind "StaticLib"
    language "C++"
    staticruntime "on"

    targetdir ("bin/" .. OutputDirectory .. "/%{prj.name}")
    objdir ("bin-int/" .. OutputDirectory .. "/%{prj.name}")

    files
	{
        "src/**.h",
        "src/**.cpp",
    }
    includedirs 

    {
		"%{IncludeDir.glm}",
    }
    filter "system:windows"
    systemversion "latest"
    cppdialect "C++20"

    filter "system:linux"
        pic "on"
        systemversion "latest"
        cppdialect "C++20"

    filter "configurations:Debug"
        runtime "Debug"
        symbols "on"

    filter "configurations:Release"
        runtime "Release"
        optimize "on"
		