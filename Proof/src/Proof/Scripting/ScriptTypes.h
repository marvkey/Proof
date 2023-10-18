#pragma once
#include "Proof/Core/Core.h"
#include "Proof/Core/Buffer.h"

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
		//uint64_t ID = 0;
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
		//uint64_t ID = 0;
		std::string FullName = "";
		//std::vector<uint64_t> Fields;
		//std::vector<uint64_t> Methods;
		std::vector<std::string> Fields;
		std::vector<std::string> Methods;
		uint32_t Size = 0;

		// Will also be true if class is static
		bool IsAbstract = false;
		bool IsStruct = false;

		//uint64_t ParentID = 0;
		std::string ParentName = "";
		MonoClass* Class = nullptr;

		~ManagedClass()
		{
			Class = nullptr;
		}
	};
	
	struct ManageEnumFieldData
	{
		std::string Name;
		ManageEnumFieldData()
		{
			m_Buffer = Buffer(8);//maximum bit of an integer uint64_t or int64_t
		}
		template<typename T>
		T GetValue()const
		{
			static_assert(sizeof(T) <= 8, "Type too large!");
			return *(m_Buffer.As<T>());
		}

		template<typename T>
		void SetValue(const T& value)
		{
			static_assert(sizeof(T) <= 8, "Type too large!");
			m_Buffer.Write(&value, sizeof(T));
		}

		virtual void SetValueBuffer(const Buffer& buffer)
		{
			m_Buffer = Buffer::Copy(buffer);
		}
		virtual Buffer GetValueBuffer() const 
		{
			return m_Buffer;
		}
	private:
		Buffer m_Buffer;

	};
	struct ManageEnumClass
	{
		std::string FullName = "";
		std::vector<ManageEnumFieldData> EnumFields;
		MonoClass* Class = nullptr;

		~ManageEnumClass()
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

	class AssemblyInfo : public RefCounted
	{
	public:
		std::filesystem::path FilePath = "";
		MonoAssembly* Assembly = nullptr;
		MonoImage* AssemblyImage = nullptr;
		//std::vector<uint32_t> Classes;
		std::vector<std::string> Classes;
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