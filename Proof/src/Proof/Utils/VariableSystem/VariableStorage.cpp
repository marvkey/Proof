#include "Proofprch.h"
#include "VariableStorage.h"
#include "Proof/Asset/AssetTypes.h"
#include <glm/glm.hpp>
namespace Proof
{
	PrimitiveVariableStorage::PrimitiveVariableStorage(VariableTypes type)
		: VariableStorage(type)
	{
		switch (type)
		{
			case Proof::VariableTypes::None:
				PF_CORE_ASSERT(false, "Variable type cannot be none");
				break;
			case Proof::VariableTypes::Int:
			case Proof::VariableTypes::Float:
				m_VariableBuffer.Allocate(sizeof(float));// using float or size of float or int same size
				m_VariableBuffer.Fill(0);
				break;
			case Proof::VariableTypes::Vec2:
				m_VariableBuffer.Allocate(sizeof(glm::vec2));
				m_VariableBuffer.Fill(0);
				break;
			case Proof::VariableTypes::Vec3:
				m_VariableBuffer.Allocate(sizeof(glm::vec3));
				m_VariableBuffer.Fill(0);
				break;
			case Proof::VariableTypes::Vec4:
				m_VariableBuffer.Allocate(sizeof(glm::vec4));
				m_VariableBuffer.Fill(0);
				break;
			case Proof::VariableTypes::String:
				m_VariableBuffer.Allocate(sizeof(256 * sizeof(char)));
				m_VariableBuffer.Fill('\0');
				break;

			case Proof::VariableTypes::AssetKey:
				m_VariableBuffer.Allocate(sizeof(DynamicAssetKey));
				m_VariableBuffer.Fill(0);
			default:
				break;
		}
	}
	PrimitiveVariableStorage::PrimitiveVariableStorage(const PrimitiveVariableStorage& other)
		: VariableStorage(other.m_Type)

	{
		m_VariableBuffer.Allocate(other.m_VariableBuffer.Size);
		m_VariableBuffer.Copy(other.GetBuffer().Data, other.GetBuffer().Size);
	}

	void PrimitiveVariableStorage::SetValueBuffer(const ScopeBuffer& buffer)
	{
		m_VariableBuffer.Copy(buffer.Data,buffer.Size);
	}

}