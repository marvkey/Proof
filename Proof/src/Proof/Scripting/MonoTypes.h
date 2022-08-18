#pragma once
#include "Proof/Core/Core.h"
#include <any>
namespace Proof{
	/*Mono Types that are supported*/
	enum class ProofMonoType {
		None = 0,
		Bool = 0x02,
		Char = 0x03,
		String = 0x0e,

		Uint8_t = 0x05,
		Uint16_t = 0x07,
		Uint32_t = 0x09,
		Uint64_t = 0x0b,

		Int8_t = 0x04,
		Int16_t = 0x06,
		Int32_t = 0x08,
		Int64_t = 0x0a,

		Float = 0x0c,
		Double = 0x0d,
		// only support sublcass of components and Entity classes 
		Class = 0x12,
		// only supported for types we have here 
		Array = 0x14,
		Enum = 0x11,
		Entity =100,
	};

	enum class ProofMonoAccessibility : uint8_t {
		None = 0,
		Private = (1 << 0),
		Internal = (1 << 1),
		Protected = (1 << 2),
		Public = (1 << 3)
	};

		//Rename ScriptClassFields
	struct ScriptField {
		std::any Data;
		std::string Name;
		ProofMonoType Type = ProofMonoType::None;
	};
	//Rename ScriptClassData

	struct ScriptData {
	public:
		// GONNA CHANGE TO ID instead of string
		ScriptData(const std::string& className) {
			ClassName = className;
		}

		bool HasField(const std::string& name) {
			for (auto& field : Fields) {
				if (name == field.Name)
					return true;
			}
			return false;
		}
		std::string ClassName;
		std::vector<ScriptField> Fields;
	};

	
}
