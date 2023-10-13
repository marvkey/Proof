#pragma once
#include "Proof/Core/Core.h"

#include <string>
#include <unordered_map>
#include <filesystem>

extern "C" {
	typedef struct _MonoAssembly MonoAssembly;
	typedef struct _MonoImage MonoImage;
	typedef struct _MonoClassField MonoClassField;
	typedef struct _MonoProperty MonoProperty;
	typedef struct _MonoMethod MonoMethod;
	typedef struct _MonoClass MonoClass;
	typedef struct _MonoObject MonoObject;
	typedef struct _MonoType MonoType;
}
namespace Proof
{
	namespace Utils {

		// NOTE: Stolen from Boost
		template<typename T>
		inline void HashCombine(size_t& result, const T& value)
		{
			std::hash<T> h;
			result ^= h(value) + 0x9e3779b9 + (result << 6) + (result >> 2);
		}

	}
	struct ManagedMethod
	{
		uint64_t ID = 0;
		std::string FullName = "";
		bool IsVirtual = false;
		bool IsStatic = false;
		uint32_t ParameterCount = 0;

		MonoMethod* Method = nullptr;

		~ManagedMethod()
		{
			Method = nullptr;
		}
	};

	struct ManagedClass
	{
		uint64_t ID = 0;
		std::string FullName = "";
		std::vector<uint64_t> Fields;
		std::vector<uint64_t> Methods;
		uint32_t Size = 0;

		// Will also be true if class is static
		bool IsAbstract = false;
		bool IsStruct = false;

		uint64_t ParentID = 0;

		MonoClass* Class = nullptr;

		~ManagedClass()
		{
			Class = nullptr;
		}
	};

	struct AssemblyMetadata
	{
		std::string Name;
		uint32_t MajorVersion;
		uint32_t MinorVersion;
		uint32_t BuildVersion;
		uint32_t RevisionVersion;

		bool operator==(const AssemblyMetadata& other) const
		{
			return Name == other.Name && MajorVersion == other.MajorVersion && MinorVersion == other.MinorVersion && BuildVersion == other.BuildVersion && RevisionVersion == other.RevisionVersion;
		}

		bool operator!=(const AssemblyMetadata& other) const { return !(*this == other); }
	};

	struct AssemblyInfo : public RefCounted
	{
		std::filesystem::path FilePath = "";
		MonoAssembly* Assembly = nullptr;
		MonoImage* AssemblyImage = nullptr;
		std::vector<uint32_t> Classes;
		bool IsCoreAssembly = false;
		AssemblyMetadata Metadata;
		std::vector<AssemblyMetadata> ReferencedAssemblies;
	};
}

namespace std {

	template<>
	struct hash<Proof::AssemblyMetadata>
	{
		size_t operator()(const Proof::AssemblyMetadata& metadata) const
		{
			size_t result = 0;
			Proof::Utils::HashCombine(result, metadata.Name);
			Proof::Utils::HashCombine(result, metadata.MajorVersion);
			Proof::Utils::HashCombine(result, metadata.MinorVersion);
			Proof::Utils::HashCombine(result, metadata.BuildVersion);
			Proof::Utils::HashCombine(result, metadata.RevisionVersion);
			return result;
		}
	};

}