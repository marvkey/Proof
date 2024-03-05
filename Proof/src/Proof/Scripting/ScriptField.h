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
		IsEnum = BIT(7),
	};

	enum class ScriptFieldType
	{
		None = 0,
		Void,
		Float, Double,
		Bool, Char,String,
		Int8, Int16, Int32, Int64,
		UInt8, UInt16, UInt32, UInt64,
		Vector2, Vector3, Vector4, Vector2Bool, Vector3Bool, Vector4Bool,
		Entity,
		//everything after entity should be an asset Type
		Prefab,Texture2D, AssetID,Mesh,DynamicMesh,Material,PhysicsMaterial,//asset
		
	};
	inline bool IsScriptFieldAssetType(ScriptFieldType type)
	{
		if ((int)type > (int)ScriptFieldType::Entity)
			return true;
		return false;
	}

	
	struct ScriptFieldRangeAttribute
	{
		enum RangeSet
		{
			None =-1,
			Min = BIT(0),
			Max = BIT(1),
			All = Min | Max
		};

		int RangeFlags = 0;
		float MinValue, MaxValue;
		bool IsSet()
		{
			return !(RangeFlags &  RangeSet::None);
		}

		bool IsAllSet()const { return RangeFlags & RangeSet::All; }
		bool IsMinSet()const { return RangeFlags & RangeSet::Min; }
		bool IsMaxSet()const {	return RangeFlags & RangeSet::Max; }
	};
	struct ScriptField
	{

		//uint64_t ID = 0;
		std::string Name;
		std::string FullName;
		ScriptFieldType Type;
		uint32_t Size;
		bool IsProperty;
		Buffer DefaultValueBuffer;

		uint64_t Flags = 0;
		std::string DisplayName = "";
		std::string ToolTip ="";
		bool HasFlag(FieldFlag flag) const { return Flags & (uint64_t)flag; }

		bool IsWritable() const
		{
			return !HasFlag(FieldFlag::ReadOnly) && HasFlag(FieldFlag::Public);
		}

		bool IsArray() const { return HasFlag(FieldFlag::IsArray); }
		bool IsEnum()const { return HasFlag(FieldFlag::IsEnum); };
		std::string RegistryClassName =	"";// 

		ScriptFieldRangeAttribute FieldRangeAttribute;
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
			case ScriptFieldType::Vector2Bool: return sizeof(glm::bvec2);
			case ScriptFieldType::Vector3Bool: return sizeof(glm::bvec3);
			case ScriptFieldType::Vector4Bool: return sizeof(glm::bvec4);
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
	inline bool IsPrimitiveType(ScriptFieldType type)
	{
		switch (type)
		{

			case ScriptFieldType::Bool:
			case ScriptFieldType::Int8:
			case ScriptFieldType::Int16:
			case ScriptFieldType::Int32:
			case ScriptFieldType::Int64:
			case ScriptFieldType::UInt8:
			case ScriptFieldType::UInt16:
			case ScriptFieldType::UInt32:
			case ScriptFieldType::UInt64:
			case ScriptFieldType::Float:
			case ScriptFieldType::Double:
			case ScriptFieldType::Char:
			case ScriptFieldType::Vector2:
			case ScriptFieldType::Vector3:
			case ScriptFieldType::Vector4:
			case ScriptFieldType::Vector2Bool: 
			case ScriptFieldType::Vector3Bool: 
			case ScriptFieldType::Vector4Bool: 
				return true;
		}

		return false;
	}
	class FieldStorageBase : public RefCounted
	{
	public:
		FieldStorageBase(ScriptField* fieldInfo)
			: m_FieldInfo(fieldInfo) {}

		virtual void SetRuntimeInstance(ScriptGCHandle instance) = 0;
		virtual void CopyFrom(const Count<FieldStorageBase>& other) = 0;

		virtual Buffer GetValueBuffer() const = 0;
		virtual void SetValueBuffer(const Buffer& buffer) = 0;

		const ScriptField* GetFieldInfo() const { return m_FieldInfo; }

	protected:
		ScriptField* m_FieldInfo = nullptr;
	};

	class FieldStorage : public FieldStorageBase
	{
	public:
		FieldStorage(ScriptField* fieldInfo)
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

		template<typename T>
		void SetValue(const T& value)
		{
			PF_CORE_ASSERT(sizeof(T) == m_FieldInfo->Size);

			if (m_RuntimeInstance != nullptr)
			{
				SetValueRuntime(&value);
			}
			else
			{
				if (!m_DataBuffer)
					m_DataBuffer.Allocate(m_FieldInfo->Size);
				m_DataBuffer.Write(&value, sizeof(T));
			}
		}

		template<>
		void SetValue<std::string>(const std::string& value)
		{
			if (m_RuntimeInstance != nullptr)
			{
				SetValueRuntime(&value);
			}
			else
			{
				if (m_DataBuffer.Size <= value.length() * sizeof(char))
				{
					m_DataBuffer.Release();
					m_DataBuffer.Allocate((value.length() * 2) * sizeof(char));
				}

				m_DataBuffer.ZeroInitialize();
				memcpy(m_DataBuffer.Data, value.c_str(), value.length() * sizeof(char));
			}
		}

		virtual void SetRuntimeInstance(ScriptGCHandle instance) override
		{
			m_RuntimeInstance = instance;

			if (m_RuntimeInstance)
			{
				if (m_FieldInfo->Type == ScriptFieldType::String)
				{
					std::string str((char*)m_DataBuffer.Data, m_DataBuffer.Size / sizeof(char));
					SetValueRuntime(&str);
				}
				else
				{
					SetValueRuntime(m_DataBuffer.Data);
				}
			}
		}

		virtual void CopyFrom(const Count<FieldStorageBase>& other)
		{
			Count<FieldStorage> fieldStorage = other.As<FieldStorage>();

			if (m_RuntimeInstance != nullptr)
			{
				Buffer valueBuffer;
				if (fieldStorage->GetValueRuntime(valueBuffer))
				{
					SetValueRuntime(valueBuffer.Data);
					valueBuffer.Release();
				}
			}
			else
			{
				m_DataBuffer.Release();
				m_DataBuffer = Buffer::Copy(fieldStorage->m_DataBuffer);
			}
		}

		virtual Buffer GetValueBuffer() const override
		{
			if (m_RuntimeInstance == nullptr)
				return m_DataBuffer;

			Buffer result;
			GetValueRuntime(result);
			return result;
		}

		virtual void SetValueBuffer(const Buffer& buffer)
		{
			if (m_RuntimeInstance != nullptr)
				SetValueRuntime(buffer.Data);
			else
				m_DataBuffer = Buffer::Copy(buffer);
		}
	private:
		bool GetValueRuntime(Buffer& outBuffer) const;
		void SetValueRuntime(const void* data);
	private:
		Buffer m_DataBuffer;
		ScriptGCHandle m_RuntimeInstance = nullptr;
	};

	class EnumFieldStorage : public FieldStorageBase
	{
	public:
		EnumFieldStorage(ScriptField* fieldInfo)
			: FieldStorageBase(fieldInfo)
		{
			m_DataBuffer = Buffer::Copy(fieldInfo->DefaultValueBuffer);
			m_EnumType = fieldInfo->Type;
		}
		ScriptFieldType GetType() 
		{
			return m_EnumType;
		}

		struct ManageEnumClass* ManageEnum()const;
		template<typename T>
		T GetValue() const
		{
			PF_CORE_ASSERT(sizeof(T) == m_FieldInfo->Size);

			if (IsRuntime())
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

		template<typename T>
		void SetValue(const T& value)
		{
			PF_CORE_ASSERT(sizeof(T) == m_FieldInfo->Size);

			if (m_RuntimeInstance != nullptr)
			{
				SetValueRuntime(&value);
			}
			else
			{
				if (!m_DataBuffer)
					m_DataBuffer.Allocate(m_FieldInfo->Size);
				m_DataBuffer.Write(&value, sizeof(T));
			}
		}

		virtual void CopyFrom(const Count<FieldStorageBase>& other)
		{
			Count<EnumFieldStorage> fieldStorage = other.As<EnumFieldStorage>();

			if (m_RuntimeInstance != nullptr)
			{
				Buffer valueBuffer;
				if (fieldStorage->GetValueRuntime(valueBuffer))
				{	
					SetValueRuntime(valueBuffer.Data);
					valueBuffer.Release();
				}
			}
			else
			{
				m_DataBuffer.Release();
				m_DataBuffer = Buffer::Copy(fieldStorage->m_DataBuffer);
			}
		}

		virtual Buffer GetValueBuffer() const override
		{
			if (m_RuntimeInstance == nullptr)
				return m_DataBuffer;

			Buffer result;
			GetValueRuntime(result);
			return result;
		}

		virtual void SetValueBuffer(const Buffer& buffer)
		{
			if (m_RuntimeInstance != nullptr)
				SetValueRuntime(buffer.Data);
			else
				m_DataBuffer = Buffer::Copy(buffer);
		}

		virtual void SetRuntimeInstance(ScriptGCHandle instance) override
		{
			m_RuntimeInstance = instance;

			if (m_RuntimeInstance)
				SetValueRuntime(m_DataBuffer.Data);
		}
	private:
		bool GetValueRuntime(Buffer& outBuffer) const;
		void SetValueRuntime(const void* data);
		bool IsRuntime()const { return m_RuntimeInstance != nullptr; }
	private:
		Buffer m_DataBuffer;
		ScriptGCHandle m_RuntimeInstance = nullptr;
		ScriptFieldType m_EnumType;
	};

	enum class ArrayFieldStorageStorage
	{
		None =0,
		ScriptFieldType, //
		Enum,
	};
	class ArrayFieldStorage : public FieldStorageBase
	{
	public:
		ArrayFieldStorage(ScriptField* fieldInfo)
			: FieldStorageBase(fieldInfo)
		{
			m_DataBuffer = Buffer::Copy(fieldInfo->DefaultValueBuffer);
			m_Length = (uint32_t)(m_DataBuffer.Size / m_FieldInfo->Size);
		}

		template<typename T>
		T GetValue(uint32_t index) const
		{
			if (m_RuntimeInstance != nullptr)
			{
				T value = T();
				GetValueRuntime(index, &value);
				return value;
			}

			if (!m_DataBuffer)
				return T();

			uint32_t offset = index * sizeof(T);
			return m_DataBuffer.Read<T>(offset);
		}

		template<>
		std::string GetValue(uint32_t index) const
		{
			if (m_RuntimeInstance != nullptr)
			{
				// TODO
				return std::string();
			}

			if (!m_DataBuffer)
				return std::string();

			const Buffer* buffer = (const Buffer*)m_DataBuffer.Data;
			buffer += index;
			return std::string((char*)buffer->Data, buffer->Size);
		}

		template<typename T>
		void SetValue(uint32_t index, const T& value)
		{
			PF_CORE_ASSERT(sizeof(T) == m_FieldInfo->Size);

			if (m_RuntimeInstance != nullptr)
			{
				SetValueRuntime(index, &value);
			}
			else
			{
				uint32_t offset = index * sizeof(T);
				m_DataBuffer.Write(&value, sizeof(T), offset);
			}
		}

		template<>
		void SetValue<std::string>(uint32_t index, const std::string& value)
		{
			if (m_RuntimeInstance != nullptr)
			{
				SetValueRuntime(index, &value);
			}
			else
			{
				Buffer& stringBuffer = (Buffer&)m_DataBuffer[index * sizeof(Buffer)];
				if (stringBuffer.Size != value.size())
				{
					stringBuffer.Release();
					stringBuffer.Allocate((value.length() * 2) * sizeof(char));
				}
				memcpy(stringBuffer.Data, value.c_str(), value.length() * sizeof(char));
			}
		}

		virtual void SetRuntimeInstance(ScriptGCHandle instance) override
		{
			m_RuntimeInstance = instance;

			if (m_RuntimeInstance)
				SetRuntimeArray(m_DataBuffer);
		}

		virtual void CopyFrom(const Count<FieldStorageBase>& other)
		{
			Count<ArrayFieldStorage> fieldStorage = other.As<ArrayFieldStorage>();

			if (m_RuntimeInstance != nullptr)
			{
				Buffer valueBuffer;
				if (fieldStorage->GetRuntimeArray(valueBuffer))
				{
					SetRuntimeArray(valueBuffer);
					valueBuffer.Release();
				}
			}
			else
			{
				m_DataBuffer.Release();
				m_DataBuffer = Buffer::Copy(fieldStorage->m_DataBuffer);
			}
		}

		void Resize(uint32_t newLength);
		void RemoveAt(uint32_t index);

		uint32_t GetLength() const { return m_RuntimeInstance != nullptr ? GetLengthRuntime() : m_Length; }

		virtual Buffer GetValueBuffer() const override
		{
			if (m_RuntimeInstance == nullptr)
				return m_DataBuffer;

			Buffer result;
			GetRuntimeArray(result);
			return result;
		}

		virtual void SetValueBuffer(const Buffer& buffer)
		{
			if (m_RuntimeInstance != nullptr)
			{
				SetRuntimeArray(buffer);
				m_Length = (uint32_t)(buffer.Size / m_FieldInfo->Size);
			}
			else
			{
				m_DataBuffer = Buffer::Copy(buffer);
				m_Length = (uint32_t)(m_DataBuffer.Size / m_FieldInfo->Size);
			}
		}
		ArrayFieldStorageStorage GetStorageType();
	private:
		bool GetRuntimeArray(Buffer& outData) const;
		void SetRuntimeArray(const Buffer& data);
		void GetValueRuntime(uint32_t index, void* data) const;
		void SetValueRuntime(uint32_t index, const void* data);
		uint32_t GetLengthRuntime() const;

	private:
		Buffer m_DataBuffer;
		uint32_t m_Length = 0;
		ScriptGCHandle m_RuntimeInstance = nullptr;
	};
}