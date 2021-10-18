project "ProofGame"
	kind "SharedLib"
	language "C#"
    targetdir ("bin/".. OutputDirectory .. "/%{prj.name}")
	objdir ("bin-int/".. OutputDirectory .. "/%{prj.name}")
    files
	{
        "**.cs",
    }
    postbuildcommands 
    {
			
        --("{COPY} %{cfg.buildtarget.relpath} ../bin/" ..OutputDirectory.."/Proof") -- This copies dll file from Proof Into Sandbox
			
    }

    filter "configurations:Debug"
    defines "PF_DEBUG"
    symbols "on"

    filter "configurations:Release"
        defines "PF_RELEASE"
        optimize "on"

    filter "configurations:Dist"
        defines "PF_DIST"
        optimize "on"