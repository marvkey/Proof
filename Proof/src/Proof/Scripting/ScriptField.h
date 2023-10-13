#pragma once
#include "Proof/Core/Core.h"
#include "Proof/Core/Buffer.h"
#include <glm/glm.hpp>
#include "Proof/Core/UUID.h"
#include "Proof/Core/Assert.h"
#include "ScriptGCManager.h"

#include "ScriptGCManager.h"
namespace Proof
{
	enum class FieldFlag
	{
		None = -1,
		ReadOnly = BIT(0),
		Static = BIT(1),
		Public = BIT(2),
		Private = BIT(3),
		Protected = BIT(4),
		Internal = BIT(5),
		IsArray = BIT(6),
	};

	enum class ScriptFieldType
	{
		None = 0,
		Void,
		Float, Double,
		Bool, Char,
		Int8, Int16, Int32, Int64, Enum, //suports only integer types, they are basically integers,
		UInt8, UInt16, UInt32, UInt64,
		Vector2, Vector3, Vector4,
		Entity,
		Prefab,Texture2D, Mesh,AssetID,DynamicMesh,Material,PhysicsMaterial,//asset
	};

	struct FieldInfo
	{
		uint64_t ID = 0;
		std::string Name;
		ScriptFieldType Type;
		uint32_t Size;
		bool IsProperty;
		Buffer DefaultValueBuffer;

		uint64_t Flags = 0;
		std::string DisplayName = "";
		bool HasFlag(FieldFlag flag) const { return Flags & (uint64_t)flag; }

		bool IsWritable() const
		{
			return !HasFlag(FieldFlag::ReadOnly) && HasFlag(FieldFlag::Public);
		}

		bool IsArray() const { return HasFlag(FieldFlag::IsArray); }
	};

	inline uint32_t GetFieldTypeSize(ScriptFieldType type)
	{
		switch (type)
		{
			case ScriptFieldType::Bool: return sizeof(bool);
			case ScriptFieldType::Int8: return sizeof(int8_t);
			case ScriptFieldType::Int16: return sizeof(int16_t);
			case ScriptFieldType::Int32: return sizeof(int32_t);
			case ScriptFieldType::Int64: return sizeof(int64_t);
			case ScriptFieldType::UInt8: return sizeof(uint8_t);
			case ScriptFieldType::UInt16: return sizeof(uint16_t);
			case ScriptFieldType::UInt32: return sizeof(uint32_t);
			case ScriptFieldType::UInt64: return sizeof(uint64_t);
			case ScriptFieldType::Float: return sizeof(float);
			case ScriptFieldType::Double: return sizeof(double);
			case ScriptFieldType::Char: return sizeof(char);
			case ScriptFieldType::Vector2: return sizeof(glm::vec2);
			case ScriptFieldType::Vector3: return sizeof(glm::vec3);
			case ScriptFieldType::Vector4: return sizeof(glm::vec4);
			case ScriptFieldType::AssetID:
			case ScriptFieldType::Entity:
			case ScriptFieldType::Prefab:
			case ScriptFieldType::Mesh:
			case ScriptFieldType::DynamicMesh:
			case ScriptFieldType::Material:
			case ScriptFieldType::PhysicsMaterial:
			case ScriptFieldType::Texture2D:
				return sizeof(UUID);
		}

		PF_CORE_ASSERT(false);
		return 0;
	}

	class FieldStorageBase : public RefCounted
	{
	public:
		FieldStorageBase(FieldInfo* fieldInfo)
			: m_FieldInfo(fieldInfo) {}

		virtual void SetRuntimeInstance(ScriptGCHandle instance) = 0;
		virtual void CopyFrom(const Count<FieldStorageBase>& other) = 0;

		virtual Buffer GetValueBuffer() const = 0;
		virtual void SetValueBuffer(const Buffer& buffer) = 0;

		const FieldInfo* GetFieldInfo() const { return m_FieldInfo; }

	protected:
		FieldInfo* m_FieldInfo = nullptr;
	};

	class FieldStorage : public FieldStorageBase
	{
	public:
		FieldStorage(FieldInfo* fieldInfo)
			: FieldStorageBase(fieldInfo)
		{
			m_DataBuffer = Buffer::Copy(fieldInfo->DefaultValueBuffer);
		}

		template<typename T>
		T GetValue() const
		{
			if (m_RuntimeInstance != nullptr)
			{
				Buffer valueBuffer;
				bool success = GetValueRuntime(valueBuffer);

				if (!success)
					return T();

				T value = T();
				memcpy(&value, valueBuffer.Data, valueBuffer.Size);
				valueBuffer.Release();
				return value;
			}

			if (!m_DataBuffer)
				return T();

			return *(m_DataBuffer.As<T>());
		}

		template<>
		std::string GetValue() const
		{
			if (m_RuntimeInstance != nullptr)
			{
				Buffer valueBuffer;
				bool success = GetValueRuntime(valueBuffer);

				if (!success)
					return std::string();

				std::string value((char*)valueBuffer.Data, valueBuffer.Size / sizeof(char));
				valueBuffer.Release();
				return value;
			}

			if (!m_DataBuffer)
				return std::string();

			return std::string((char*)m_DataBuffer.Data, m_DataBuffer.Size / sizeof(char));
		}

	private:
		bool GetValueRuntime(Buffer& outBuffer) const;
		void SetValueRuntime(const void* data);
	private:
		Buffer m_DataBuffer;
		ScriptGCHandle m_RuntimeInstance = nullptr;
	};
}