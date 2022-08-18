workspace "Proof"
	architecture "x64"
	startproject "Proof-Editor"
	configurations
	{

		"Debug",
		"Release",
		"Dist"
	}
	



OutputDirectory ="%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"
IncludeDir={}
IncludeDir["GLFW"]="%{wks.location}/Proof/vendor/glfw-3.3.2.bin.WIN64/include"
IncludeDir["ImGui"]="%{wks.location}/Proof/vendor/ImGUI"
IncludeDir["glm"]="%{wks.location}/Proof/vendor/glm"
IncludeDir["Vulkan"]="%{wks.location}/Proof/vendor/VulkanSDK/1.3.204.1/Include"
IncludeDir["Glad"] = "%{wks.location}/Proof/vendor/Glad/include"
IncludeDir["Log"]= "%{wks.location}/Proof/vendor/Log/include"
IncludeDir["Assimp"]= "%{wks.location}/Proof/vendor/Assimp/include"
IncludeDir["ImGuizmo"] = "%{wks.location}/Proof/vendor/ImGuizmo"
IncludeDir["yaml_cpp"] = "%{wks.location}/Proof/vendor/yaml-cpp/include"
IncludeDir["fmt"] = "%{wks.location}/Proof/vendor/fmt/include"
IncludeDir["SPIRV-Cross"] = "%{wks.location}/Proof/vendor/SPIRV-Cross"
IncludeDir["shaderc"] = "%{wks.location}/proof/vendor/shaderc/include"
IncludeDir["PhysX"] = "%{wks.location}/proof/vendor/PhysX/include"
IncludeDir["Bullet3"] = "%{wks.location}/proof/vendor/Bullet3/src"
IncludeDir["mono"] = "%{wks.location}/Proof/vendor/mono/include"


LibraryDir = {}
LibraryDir["VulkanSDK"] = "%{wks.location}/Proof/vendor/VulkanSDK/1.3.204.1/Lib"

Library = {}
Library["Vulkan"] = "%{LibraryDir.VulkanSDK}/vulkan-1.lib"
Library["VulkanUtils"] = "%{LibraryDir.VulkanSDK}/VkLayer_utils.lib"


Library["ShaderC_Debug"] = "%{LibraryDir.VulkanSDK}/shaderc_shared.lib"
Library["SPIRV_Cross_Debug"] = "%{LibraryDir.VulkanSDK}/spirv-cross-core.lib"
Library["SPIRV_Cross_GLSL_Debug"] = "%{LibraryDir.VulkanSDK}/spirv-cross-glsl.lib"
Library["SPIRV_Tools_Debug"] = "%{LibraryDir.VulkanSDK}/SPIRV-Tools.lib"

Library["ShaderC_Release"] = "%{LibraryDir.VulkanSDK}/shaderc_shared.lib"
Library["SPIRV_Cross_Release"] = "%{LibraryDir.VulkanSDK}/spirv-cross-core.lib"
Library["SPIRV_Cross_GLSL_Release"] = "%{LibraryDir.VulkanSDK}/spirv-cross-glsl.lib"


Library["mono"] = "%{wks.location}/proof/vendor/mono/lib/%{cfg.buildcfg}"
-- Windows (THIS ARE FOR MONO)
Library["WinSock"] = "Ws2_32.lib"
Library["WinMM"] = "Winmm.lib"
Library["WinVersion"] = "Version.lib"
Library["BCrypt"] = "Bcrypt.lib"
group "ExternalDependencies"
	include "Proof/vendor/ImGUI"
	include "Proof/vendor/glfw-3.3.2.bin.WIN64"
	include "Proof/vendor/Glad"
	include "Proof/vendor/yaml-cpp"
	include "Proof/vendor/fmt"
	include "Proof/vendor/SPIRV-Cross"
	include "Proof/vendor/shaderc"
	include "Proof/vendor/PhysX"
	include "Proof/vendor/bullet3"
group ""

group "Dependencies"
	include "ProofPhysics"  
group ""

include "Proof-Editor"
include "Proof"
include "SandBox"
include "ProofScriptCore"  



