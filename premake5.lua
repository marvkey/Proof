workspace "Proof"
	architecture "x64"
	startproject "Proof-Editor"
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


OutputDirectory ="%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"
IncludeDir={}
IncludeDir["GLFW"]="%{wks.location}/Proof/vendor/glfw-3.3.2.bin.WIN64/include"
IncludeDir["ImGui"]="%{wks.location}/Proof/vendor/ImGUI"
IncludeDir["glm"]="%{wks.location}/Proof/vendor/glm"
--IncludeDir["Vulkan"]="%{wks.location}/Proof/vendor/Vulkan/Include"
IncludeDir["Glad"] = "%{wks.location}/Proof/vendor/Glad/include"
IncludeDir["Log"]= "%{wks.location}/Proof/vendor/Log/include"
IncludeDir["Assimp"]= "%{wks.location}/Proof/vendor/Assimp/include"
IncludeDir["ImGuizmo"] = "%{wks.location}/Proof/vendor/ImGuizmo"
IncludeDir["yaml_cpp"] = "%{wks.location}/Proof/vendor/yaml-cpp/include"

group "Dependencies"
	include "Proof/vendor/ImGUI"
	include "Proof/vendor/glfw-3.3.2.bin.WIN64"
	include "Proof/vendor/Glad"
	include "Proof/vendor/yaml-cpp"
group ""
include "Proof-Editor"
include "Proof"
include "SandBox"
  


