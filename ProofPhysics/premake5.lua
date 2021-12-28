project "ProofPhysics"
    kind "StaticLib"
    language "C++"

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
		--"C:/Program Files/Mono/include/mono-2.0"
    }
    filter "system:windows"
    systemversion "latest"
    cppdialect "C++17"
    staticruntime "On"

    filter "system:linux"
        pic "on"
        systemversion "latest"
        cppdialect "C++17"
        staticruntime "On"

    filter "configurations:Debug"
        runtime "Debug"
        symbols "on"

    filter "configurations:Release"
        runtime "Release"
        optimize "on"
		