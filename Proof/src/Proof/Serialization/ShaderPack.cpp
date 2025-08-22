#include "Proofprch.h"
#include "ShaderPack.h"
#include "Proof/Renderer/Shader.h"
#include "FileStream.h"
#include "Proof/Platform/Vulkan/VulkanShader.h"
#include "Proof/Core/Hash.h"

namespace Proof
{
	ShaderPack::ShaderPack(const std::filesystem::path& path)
		:m_Path(path)
	{
		// Read index
		FileStreamReader serializer(path);
		if (!serializer)
			return;

		serializer.ReadRaw(m_File.Header);
		if (memcmp(m_File.Header.HEADER, "PFSP", 4) != 0)
			return;

		m_Loaded = true;
		for (uint32_t i = 0; i < m_File.Header.ShaderProgramCount; i++)
		{
			uint32_t key;
			serializer.ReadRaw(key);
			auto& shaderProgramInfo = m_File.Index.ShaderPrograms[key];
			serializer.ReadRaw(shaderProgramInfo.ReflectionDataOffset);
			serializer.ReadArrayAndSize(shaderProgramInfo.ModuleIndices);
		}

		auto sp = serializer.GetStreamPosition();
		serializer.ReadArrayWithSize(m_File.Index.ShaderModules, m_File.Header.ShaderModuleCount);
	}
	bool ShaderPack::Contains(std::string_view name) const
	{
		return m_File.Index.ShaderPrograms.contains(Hash::GenerateFNVHash32(name));
	}
	Count<class Shader> ShaderPack::LoadShader(std::string_view name)
	{
		uint32_t nameHash = Hash::GenerateFNVHash32(name);
		if(!Contains(name))
			PF_CORE_ASSERT(false);

		const auto& shaderProgramInfo = m_File.Index.ShaderPrograms.at(nameHash);

		FileStreamReader serializer(m_Path);

		serializer.SetStreamPosition(shaderProgramInfo.ReflectionDataOffset);

		// Debug only
		std::string shaderName;
		{
			std::string path(name);
			size_t found = path.find_last_of("/\\");
			shaderName = found != std::string::npos ? path.substr(found + 1) : path;
			found = shaderName.find_last_of('.');
			shaderName = found != std::string::npos ? shaderName.substr(0, found) : name;
		}

		//vulkanShader->TryReadReflectionData(&serializer);
		// vulkanShader->m_DisableOptimization =

		std::unordered_map<ShaderStage, std::vector<uint32_t>> shaderModules;
		for (uint32_t index : shaderProgramInfo.ModuleIndices)
		{
			const auto& info = m_File.Index.ShaderModules[index];
			auto& moduleData = shaderModules[(ShaderStage)info.Stage];

			serializer.SetStreamPosition(info.PackedOffset);
			serializer.ReadArrayWithSize(moduleData, (uint32_t)info.PackedSize);
		}

		serializer.SetStreamPosition(shaderProgramInfo.ReflectionDataOffset);
		//vulkanShader->TryReadReflectionData(&serializer);

		//vulkanShader->LoadAndCreateShaders(shaderModules);
		//vulkanShader->CreateDescriptors();

		//Renderer::AcknowledgeParsedGlobalMacros(compiler->GetAcknowledgedMacros(), vulkanShader);
		//Renderer::OnShaderReloaded(vulkanShader->GetHash());

		Count<VulkanShader> vulkanShader = Count<VulkanShader>::Create(std::string(name.data()), shaderModules);

		return vulkanShader;
	}

	Count<ShaderPack> ShaderPack::CreateFromLibrary(Count<class ShaderLibrary> shaderLibrary, const std::filesystem::path& path)
    {
		PF_EC_WARN("SHADER PACK STILL SHIPS WITH SPIRV NEED TO ADD REFLECTION OF SHADER INFORMATION TO STOP RECOMPILING IN DIST");
		PF_EC_WARN("SHADER PACK STILL SHIPS WITH SPIRV NEED TO ADD REFLECTION OF SHADER INFORMATION TO STOP RECOMPILING IN DIST");
		PF_EC_WARN("SHADER PACK STILL SHIPS WITH SPIRV NEED TO ADD REFLECTION OF SHADER INFORMATION TO STOP RECOMPILING IN DIST");
		PF_EC_WARN("SHADER PACK STILL SHIPS WITH SPIRV NEED TO ADD REFLECTION OF SHADER INFORMATION TO STOP RECOMPILING IN DIST");
		PF_EC_WARN("SHADER PACK STILL SHIPS WITH SPIRV NEED TO ADD REFLECTION OF SHADER INFORMATION TO STOP RECOMPILING IN DIST");
		PF_EC_WARN("SHADER PACK STILL SHIPS WITH SPIRV NEED TO ADD REFLECTION OF SHADER INFORMATION TO STOP RECOMPILING IN DIST");
		PF_EC_WARN("SHADER PACK STILL SHIPS WITH SPIRV NEED TO ADD REFLECTION OF SHADER INFORMATION TO STOP RECOMPILING IN DIST");

		Count<ShaderPack> shaderPack = Count<ShaderPack>::Create();

		const auto& shaderMap = shaderLibrary->GetShaderMap();
		auto& shaderPackFile = shaderPack->m_File;

		shaderPackFile.Header.Version = 1;
		shaderPackFile.Header.ShaderProgramCount = (uint32_t)shaderMap.size();
		shaderPackFile.Header.ShaderModuleCount = 0;

		// Determine number of modules (per shader)
		// NOTE: this currently doesn't care about duplicated modules, but it should (eventually, not that important atm)
		uint32_t shaderModuleIndex = 0;
		uint32_t shaderModuleIndexArraySize = 0;
		for (const auto& [name, shader] : shaderMap)
		{
			//	for (auto shader : shaders)
			{
				Count<VulkanShader> vulkanShader = shader.As<VulkanShader>();
				const auto& shaderData = vulkanShader->m_VulkanSPIRV;

				shaderPackFile.Header.ShaderModuleCount += (uint32_t)shaderData.size();
				auto& shaderProgramInfo = shaderPackFile.Index.ShaderPrograms[Hash::GenerateFNVHash32(name)];

				for (int i = 0; i < (int)shaderData.size(); i++)
					shaderProgramInfo.ModuleIndices.emplace_back(shaderModuleIndex++);

				shaderModuleIndexArraySize += sizeof(uint32_t); // size
				shaderModuleIndexArraySize += (uint32_t)shaderData.size() * sizeof(uint32_t); // indices
			}
		}

		uint32_t shaderProgramIndexSize = shaderPackFile.Header.ShaderProgramCount *
			(sizeof(std::map<uint32_t, ShaderPackFile::ShaderProgramInfo>::key_type) + sizeof(ShaderPackFile::ShaderProgramInfo::ReflectionDataOffset))
			+ shaderModuleIndexArraySize;

		FileStreamWriter serializer(path);

		// Write header
		serializer.WriteRaw<ShaderPackFile::FileHeader>(shaderPackFile.Header);

		// ===============
		// Write index
		// ===============
		// Write dummy data for shader programs
		uint64_t shaderProgramIndexPos = serializer.GetStreamPosition();
		serializer.WriteZero(shaderProgramIndexSize);

		// Write dummy data for shader modules
		uint64_t shaderModuleIndexPos = serializer.GetStreamPosition();
		serializer.WriteZero(shaderPackFile.Header.ShaderModuleCount * sizeof(ShaderPackFile::ShaderModuleInfo));
		for (const auto& [name, shader] : shaderMap)
		{
			//for (auto shader : shaders)
			{
				Count<VulkanShader> vulkanShader = shader.As<VulkanShader>();

				// Serialize reflection data
				shaderPackFile.Index.ShaderPrograms[Hash::GenerateFNVHash32(name)].ReflectionDataOffset = serializer.GetStreamPosition();
				//vulkanShader->SerializeReflectionData(&serializer);

				// Serialize SPIR-V data
				const auto& shaderData = vulkanShader->m_VulkanSPIRV;
				for (const auto& [stage, data] : shaderData)
				{
					auto& indexShaderModule = shaderPackFile.Index.ShaderModules.emplace_back();
					indexShaderModule.PackedOffset = serializer.GetStreamPosition();
					indexShaderModule.PackedSize = data.size();
					indexShaderModule.Stage = (uint8_t)stage;

					serializer.WriteArray(data, false);
				}
			}
		}

		// Write program index
		serializer.SetStreamPosition(shaderProgramIndexPos);
		uint64_t begin = shaderProgramIndexPos;
		for (const auto& [name, programInfo] : shaderPackFile.Index.ShaderPrograms)
		{
			serializer.WriteRaw(name);
			serializer.WriteRaw(programInfo.ReflectionDataOffset);
			serializer.WriteArray(programInfo.ModuleIndices);
		}
		uint64_t end = serializer.GetStreamPosition();
		uint64_t s = end - begin;

		// Write module index
		serializer.SetStreamPosition(shaderModuleIndexPos);
		serializer.WriteArray(shaderPackFile.Index.ShaderModules, false);

		return shaderPack;
    }
}
