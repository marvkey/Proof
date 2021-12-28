#include "Proof/Core/Proof.h"
#include "Proof/Core/Application.h"
#include "Editor3D/Editor3D.h"
#include <Glad/glad.h>
namespace Proof {
    class ProofEditor : public Application {
    public:
        ProofEditor() {
            PushLayer(new Editore3D());
        }
    private:
    };
    Application* CreateApplication() {
        return new ProofEditor();
    }
}