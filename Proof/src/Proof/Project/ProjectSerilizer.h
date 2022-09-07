#pragma once
#include "Proof/Core/Core.h"
namespace Proof{
	class ProjectSerilizer {
	public:
		ProjectSerilizer(class Project* project);
		void SerilizeText(const std::string& filePath);
		bool DeSerilizeText(const std::string& filePath);
	private:
		class Project* m_Project;
	};
}
