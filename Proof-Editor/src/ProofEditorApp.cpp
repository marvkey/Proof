#include "Proof/Core/Proof.h"
#include "Proof/Core/Application.h"
#include "Editor3D/Editor3D.h"
namespace Proof {
    class ProofEditor : public Application {
    public:
        ProofEditor() {
            PushLayer(new Editore3D());
        }
        ~ProofEditor() {
        }
    };
    Application* CreateApplication() {
        return new ProofEditor();
    }
}