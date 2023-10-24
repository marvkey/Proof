#pragma once
#include "Proof/Asset/Asset.h"
namespace Proof
{
	#if 0
	class ScriptFile : public Asset
	{
	public:
		ScriptFile() = default;
		ScriptFile(const std::string& classNamespace, const std::string& className)
			: m_ClassNamespace(classNamespace), m_ClassName(className) {}

		const std::string& GetClassNamespace() const { return m_ClassNamespace; }
		const std::string& GetClasssName() const { return m_ClassName; }

		std::string GetFullName() 
		{
			if(!m_ClassNamespace.empty())
				return m_ClassNamespace + "." + m_ClassName;

			return m_ClassName;
		}
		static AssetType GetStaticType() { return AssetType::ScriptFile; }
		virtual AssetType GetAssetType() const override { return GetStaticType(); }
	private:
		std::string m_ClassNamespace = "";
		std::string m_ClassName = "";
	};
	#endif
}