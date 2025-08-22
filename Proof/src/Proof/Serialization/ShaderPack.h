#pragma once
#include "Proof/Core/Core.h"
#include <filesystem>
#include "StreamReadAndWrite.h"
namespace Proof
{
	/*
		 ============================
		 Shader Pack File Layout (on disk)
		 ============================
		 [FileHeader]                          // Signature + version + counts
		 [ShaderModuleInfo x N]                // All individual SPIR-V modules
		 [ShaderProgramInfo x M]               // Each program = list of module indices
		 [ReflectionData x M]                  // Reflection info per program (offset from above)
		 [SPIR-V Binary Data x N]              // Raw SPIR-V binary blobs (indexed by ShaderModuleInfo)
	 */

	struct ShaderPackFile
	{
		// ====== Metadata extracted from SPIR-V reflection (optional use) ======
		struct ShaderReflectionInfo
		{
			// To be populated by SPIRV-Cross or similar tool
			// e.g. lists of:
			// - Uniform buffer bindings
			// - Storage buffer bindings
			// - Push constants
			// - Sampled images
			// - Storage images
			// etc.
		};

		// ====== Runtime representation of a shader ======
		struct ShaderData
		{
			uint8_t Stage = 0;                         // Vertex, Fragment, Compute, etc.
			ShaderReflectionInfo ReflectionInfo;       // Reflection metadata (not serialized here)
			void* Data = nullptr;                      // Pointer to SPIR-V binary in memory
		};

		// ====== Serialized metadata for an individual SPIR-V shader ======
		struct ShaderModuleInfo
		{
			uint64_t PackedOffset;     // Offset in file where SPIR-V binary starts
			uint64_t PackedSize;       // Size of SPIR-V data
			uint8_t Version;           // Shader version or compiler ID
			uint8_t Stage;             // Shader stage enum
			uint32_t Flags = 0;        // Bitfield: optimization flags, debug, etc.

			static void Serialize(StreamWriter* writer, const ShaderModuleInfo& info) { writer->WriteRaw(info); }
			static void Deserialize(StreamReader* reader, ShaderModuleInfo& info) { reader->ReadRaw(info); }
		};

		// ====== A shader program is made up of multiple stages (e.g. vert + frag) ======
		struct ShaderProgramInfo
		{
			uint64_t ReflectionDataOffset = 0;       // Offset in file to program reflection
			std::vector<uint32_t> ModuleIndices;     // Indices into ShaderModules array
		};

		// ====== Shader Index holds all modules and links programs to modules ======
		struct ShaderIndex
		{
			std::map<uint32_t, ShaderProgramInfo> ShaderPrograms;  // Hashed name/path → info
			std::vector<ShaderModuleInfo> ShaderModules;            // All shader modules

			// Estimate size needed for saving index (excluding reflection data)
			static uint64_t CalculateSizeRequirements(uint32_t programCount, uint32_t moduleCount)
			{
				return (sizeof(uint32_t) + sizeof(ShaderProgramInfo)) * programCount
					+ sizeof(ShaderModuleInfo) * moduleCount;
			}
		};

		// ====== Header appears at the very beginning of the file ======
		struct FileHeader
		{
			char HEADER[4] = { 'P','F','S','P' };     // Signature to identify shader pack
			uint32_t Version = 1;                    // File format version
			uint32_t ShaderProgramCount = 0;
			uint32_t ShaderModuleCount = 0;
		};

		// ====== Top-level structure for loading/packing shader packs ======
		FileHeader Header;
		ShaderIndex Index;
		ShaderData* Data = nullptr; // Optional if loading binaries into memory
	};

    class ShaderPack : public RefCounted
    {
    public:
        ShaderPack() = default;

        ShaderPack(const std::filesystem::path& path);

        bool IsLoaded() const { return m_Loaded; }
        bool Contains(std::string_view name) const;

        Count<class Shader> LoadShader(std::string_view name);


        static Count<ShaderPack> CreateFromLibrary(Count<class ShaderLibrary> shaderLibrary, const std::filesystem::path& path);
    private:
        bool m_Loaded = false;
        ShaderPackFile m_File;
        std::filesystem::path m_Path;
    };
}