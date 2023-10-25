

project "ProofScriptCore"
	kind "SharedLib"
	language "C#"
	dotnetframework "4.7.2"
	targetdir ("%{wks.location}/Proof-Editor/Resources/Scripts/Binaries/ScriptCore")
	objdir ("%{wks.location}/Proof-Editor/Resources/Scripts/Intermediates/ScriptCore")

	files 
	{
		"src/**.cs",
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