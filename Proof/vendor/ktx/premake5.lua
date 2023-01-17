project "ktx"
    kind "StaticLib"
    language "C"
    staticruntime "on"
    
    targetdir ("bin/" .. OutputDirectory .. "/%{prj.name}")
    objdir ("bin-int/" .. OutputDirectory .. "/%{prj.name}")

    files
    {
        "ktx/ **.h",
        "ktx/**.c",
        "ktx/**.cxx",
        "ktx/**.cpp",
    
    }
	removefiles
    {
        --"ktx/lib/glloader.c",
        --"ktx/lib/writer_v1.c",  
        --"ktx/lib/etcunpack.cxx",
    }
    includedirs
    {
        "ktx/include",
        "ktx/other_include",
        "ktx/lib",
		"%{IncludeDir.Vulkan}",
    }
    defines "KHRONOS_STATIC"
    defines "KTX_OPENGL_ES1"
    filter "system:windows"
        systemversion "latest"
		staticruntime "off"

    filter "configurations:Debug"
        runtime "Debug"
        symbols "on"

    filter "configurations:Release"
        runtime "Release"
        optimize "on"