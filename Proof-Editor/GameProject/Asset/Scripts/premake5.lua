local ProofRootDir = '../../../..'
include (ProofRootDir .. "/vendor/premake/premake_customization/solution_items.lua")

workspace "Game"
	architecture "x86_64"
	startproject "Game"

	configurations
	{
		"Debug",
		"Release",
		"Dist"
	}

	flags
	{
		"MultiProcessorCompile"
	}

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

project "Game"
	kind "SharedLib"
	language "C#"
	dotnetframework "4.7.2"

	targetdir ("Binaries")
	objdir ("Intermediates")

	files 
	{
		"src/**.cs",
		"Properties/**.cs"
	}

	links
	{
		"ProofScriptCore"
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

group "Proof"
	include (ProofRootDir .. "/ProofScriptCore")
group ""