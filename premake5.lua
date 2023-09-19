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
IncludeDir["Vulkan"]="%{wks.location}/Proof/vendor/VulkanSDK/Include"
IncludeDir["Glad"] = "%{wks.location}/Proof/vendor/Glad/include"
IncludeDir["Log"]= "%{wks.location}/Proof/vendor/Log/include"
IncludeDir["Assimp"]= "%{wks.location}/Proof/vendor/Assimp/include"
IncludeDir["ImGuizmo"] = "%{wks.location}/Proof/vendor/ImGuizmo"
IncludeDir["yaml_cpp"] = "%{wks.location}/Proof/vendor/yaml-cpp/include"
IncludeDir["fmt"] = "%{wks.location}/Proof/vendor/fmt/include"
IncludeDir["SPIRV-Cross"] = "%{wks.location}/Proof/vendor/SPIRV-Cross"
IncludeDir["shaderc"] = "%{wks.location}/Proof/vendor/shaderc/include"
IncludeDir["physX"] = "%{wks.location}/Proof/vendor/PhysX/physx/include"
IncludeDir["physXfoundation"] = "%{wks.location}/Proof/vendor/PhysX/pxshared/include"
IncludeDir["mono"] = "%{wks.location}/Proof/vendor/mono/include"
IncludeDir["magic_enum"] = "%{wks.location}/Proof/vendor/magic_enum/include"
IncludeDir["optick"] = "%{wks.location}/Proof/vendor/optick/src"
IncludeDir["entt"] = "%{wks.location}/Proof/vendor/entt"
IncludeDir["ktx"] = "%{wks.location}/Proof/vendor/ktx"
IncludeDir["msdfgen"] = "%{wks.location}/Proof/vendor/msdf-atlas-gen/msdfgen"
IncludeDir["msdf_atlas_gen"] = "%{wks.location}/Proof/vendor/msdf-atlas-gen"
IncludeDir["MiniAudio"] = "%{wks.location}/Proof/vendor/MiniAudio"

LibraryDir = {}
LibraryDir["ProofOuputDir"] =  "%{wks.location}/Proof/bin/".. OutputDirectory .. "/Proof"
LibraryDir["VulkanSDK"] = "%{wks.location}/Proof/vendor/VulkanSDK/Lib"

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


Library["mono"] = "%{wks.location}/Proof/vendor/mono/lib/%{cfg.buildcfg}"
-- Windows (THIS ARE FOR MONO)
Library["WinSock"] = "Ws2_32.lib"
Library["WinMM"] = "Winmm.lib"
Library["WinVersion"] = "Version.lib"
Library["BCrypt"] = "Bcrypt.lib"

Library["PhysX_static_64"] ="%{LibraryDir.ProofOuputDir}/physx/PhysX_static_64.lib"
Library["PhysXCharacterKinematic_static_64"] ="%{LibraryDir.ProofOuputDir}/physx/PhysXCharacterKinematic_static_64.lib"
Library["PhysXCommon_static_64"] = "%{LibraryDir.ProofOuputDir}/physx/PhysXCommon_static_64.lib"
Library["PhysXCooking_static_64"] = "%{LibraryDir.ProofOuputDir}/physx/PhysXCooking_static_64.lib"
Library["PhysXExtensions_static_64"] = "%{LibraryDir.ProofOuputDir}/physx/PhysXExtensions_static_64.lib"
Library["PhysXFoundation_static_64"]= "%{LibraryDir.ProofOuputDir}/physx/PhysXFoundation_static_64.lib"
Library["PhysXPvdSDK_static_64"]= "%{LibraryDir.ProofOuputDir}/physx/PhysXPvdSDK_static_64.lib"
Library["PhysXVehicle_static_64"]= "%{LibraryDir.ProofOuputDir}/physx/PhysXVehicle_static_64.lib"

group "ExternalDependencies"
	include "Proof/vendor/ImGUI"
	include "Proof/vendor/glfw-3.3.2.bin.WIN64"
	include "Proof/vendor/Glad"
	include "Proof/vendor/yaml-cpp"
	include "Proof/vendor/SPIRV-Cross"
	include "Proof/vendor/shaderc"
	include "Proof/vendor/msdf-atlas-gen"
group ""

group "Dependencies"
	include "ProofPhysics"  
group ""

include "Proof-Editor"
include "Proof"
include "SandBox"
include "ProofScriptCore"  
include "ProofRuntime"  



