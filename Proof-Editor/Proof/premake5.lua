
FileVersion = 1.2
require "vstudio"

--just need t work//https://github.com/premake/premake-core/issues/1963
function platformsElement(cfg)
	_p(2,'<Platforms>x64</Platforms>')
end

premake.override(premake.vstudio.cs2005.elements, "projectProperties", function (oldfn, cfg)
	return table.join(oldfn(cfg), {
	platformsElement,
	})
end)

ProofRoootDirectory = os.getenv("PROOF_DIR")
include (path.join(ProofRoootDirectory, "Proof-Editor", "Resources", "LUA", "Proof.lua"))
workspace "Game"
	targetdir "build"
	startproject "Game"
	
	configurations 
	{ 
		"Debug", 
		"Release",
		"Dist"
	}

group "Proof"
project "ProofScriptCore"
	location "%{ProofRoootDirectory}/ProofScriptCore"
	kind "SharedLib"
	language "C#"
	dotnetframework "4.7.2"

	targetdir ("%{ProofRoootDirectory}/Proof-Editor/Resources/Scripts/Binaries/ScriptCore")
	objdir ("%{ProofRoootDirectory}/Proof-Editor/Resources/Scripts/Intermediates/ScriptCore")

	files
	{
		"%{ProofRoootDirectory}/ProofScriptCore/Src/**.cs",
		--"%{ProofRoootDirectory}/ProofScriptCore/Properties/**.cs",
	}

	filter "configurations:Debug"
		optimize "Off"
		symbols "Default"

	filter "configurations:Release"
		optimize "On"
		symbols "Default"

	filter "configurations:Dist"
		optimize "Full"
		symbols "Off"

group ""

project "Game"
	location "Source/ScriptApp"
	kind "SharedLib"
	language "C#"
	dotnetframework "4.7.2"
    
	targetname "Game"
	targetdir ("%{wks.location}/Binaries/ScriptApp")
	objdir ("%{wks.location}/Intermediates/ScriptApp")

	files 
	{
		"Assets/**.cs", 
	}
    links
	{
		"ProofScriptCore"
	}
	linkAppReferences()

	print("\nDone linking app references")    
	filter "configurations:Debug"
		optimize "Off"
		symbols "Default"

	filter "configurations:Release"
		optimize "On"
		symbols "Default"

	filter "configurations:Dist"
		optimize "Full"
		symbols "Off"
