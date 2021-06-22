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
IncludeDir["GLEW"]="%{wks.location}/Proof/vendor/glew-2.1.0/include"
IncludeDir["ImGui"]="%{wks.location}/Proof/vendor/ImGUI"
IncludeDir["glm"]="%{wks.location}/Proof/vendor/glm"
IncludeDir["entt"]="%{wks.location}/Proof/vendor/entt"
--IncludeDir["Vulkan"]="%{wks.location}/Proof/vendor/Vulkan/Include"
IncludeDir["Glad"] = "%{wks.location}/Proof/vendor/Glad/include"
IncludeDir["Log"]= "%{wks.location}/Proof/vendor/Log/include"
IncludeDir["Assimp"]= "%{wks.location}/Proof/vendor/Assimp/include"

group "Dependencies"
	include "Proof/vendor/ImGUI"
	include "Proof/vendor/glfw-3.3.2.bin.WIN64"
	include "Proof/vendor/Glad"
group ""
include "Proof-Editor"
include "Proof"
include "SandBox"
  


