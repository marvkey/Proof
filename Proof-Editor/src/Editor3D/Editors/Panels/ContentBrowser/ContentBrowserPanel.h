#pragma once
#include "Editor3D/Editors/Panels/Panel.h"

#define MAX_INPUT_BUFFER_LENGTH 216
namespace Proof
{
	class ContentBrowserPanel : public Panel
	{
	public:
		ContentBrowserPanel();

		void OnImGuiRender(const char* dsiplayName, bool& isOpen);

	};
}