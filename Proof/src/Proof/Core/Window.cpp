#include "Proofprch.h"
#include "Proof/Events/Event.h"
#include "Proof/Input/Controller.h"
#include "Platform/Window/WindowsWindow.h"
namespace Proof
{
    Special <Window> Window::Create(const WindowConfiguration& configuration) {
        return CreateSpecial<WindowsWindow>(configuration);
    }
    Controller& Window::GetController(int index)
    {
        PF_CORE_ASSERT(HasController(index), "Does not contain controller");
        return s_Controllers.at(index);
    }
}