project "ProofScriptCore"
	kind "SharedLib"
	language "C#"
	dotnetframework "4.7.2"
	targetdir ("../Proof-Editor/Proof/Resources/Scripts/Binaries")
	objdir ("../Proof-Editor/Resources/Scripts/Intermediates")

	files 
	{
		"src/**.cs",
		"Properties/**.cs"
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