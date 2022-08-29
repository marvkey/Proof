#include "Proof/Core/Proof.h"
#include "Proof/Core/Application.h"
#include <filesystem>
#include "Editor3D/Editor3D.h"
#include "Proof/Project/Project.h"
namespace Proof {
    class ProofEditor : public Application {
    public:
        ProofEditor(const ApplicationConfiguration& configuration, std::string_view projectPath):
        Application(configuration)
        {
            // filesyste
            {
                std::filesystem::path workingDirectory = std::filesystem::current_path();
                (FileSystem::SetAEnvironmentVariable)("PROOF_DIR", workingDirectory.string());
            }
            PushLayer(new class Editore3D());
        }
    };
    Application* CreateApplication(int argc, char** argv) {
        std::string_view projectPath;
        if (argc > 1)
            projectPath = argv[1];

        ApplicationConfiguration configuration;
        configuration.Name = "Proof Editor";
        configuration.EnableImgui = false;
        configuration.ProjectPath = projectPath;
        configuration.WindowConfiguration.Height = 500;
        configuration.WindowConfiguration.Width = 800;
        configuration.WindowConfiguration.Vsync = false;
        configuration.WindowConfiguration.Title = "Proof";
        configuration.WindowConfiguration.startMaximized = false;
        configuration.WindowConfiguration.startFullScreen = false;
        configuration.WindowConfiguration.Decorated = true; 
        return new ProofEditor(configuration, projectPath);
    }
}