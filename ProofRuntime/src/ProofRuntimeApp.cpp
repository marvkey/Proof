#include "Proof/Core/Proof.h"
#include "Proof/Core/Application.h"
#include <filesystem>
#include "Runtime/RuntimeLayer.h"
#include "Proof/Project/Project.h"
#include "Proof/Utils/FileSystem.h"
namespace Proof {
    class RuntimeApplication : public Application {
    public:
        RuntimeApplication(const ApplicationConfiguration& configuration, std::string_view projectPath) :
            Application(configuration)
        {

            if (m_ApplicationConfiguration.ProjectPath.empty())
            {
                const std::string newProjectPath = "SandboxProject/SandboxProject.ProofProject";
            #if 0
                if (!std::filesystem::exists("SandboxProject"))
                {
                    std::filesystem::create_directory("SandboxProject");

                }

                if (!std::filesystem::exists(newProjectPath))
                {
                    ProjectConfig config(std::filesystem::path(newProjectPath), "SandboxProject");
                    Count<Project> sandBoxProject = Project::New(config);
                }
            #endif
                m_ApplicationConfiguration.ProjectPath = newProjectPath;
            }
            PushLayer(new class RuntimeLayer());
        }
    };
    Application* CreateApplication(int argc, char** argv) {
        std::string_view projectPath = "../Proof-Editor/Proof/Proof.ProofProject";
        //projectPath = "Driftwood/Driftwood.ProofProject";
        //projectPath = "PacMan3D/PacMan3D.ProofProject";
        projectPath = "../Proof-Editor/FlappyBird/FlappyBird.ProofProject";
       // projectPath = "../Proof-Editor/PacMan/PacMan.ProofProject";
        if (argc > 1)
            projectPath = argv[1];

        ApplicationConfiguration configuration;
        configuration.Name = "Proof Runtime";
        configuration.EnableImgui = false;
        configuration.ProjectPath = projectPath;
        configuration.WindowConfiguration.Height = 500;
        configuration.WindowConfiguration.Width = 800;
        configuration.WindowConfiguration.Vsync = true;
        configuration.WindowConfiguration.Title = FileSystem::GetFileName(projectPath);

        //when set to true
        // a bug happens when we change to another apllication while running proof
        // it crahses
        configuration.WindowConfiguration.startWindowedFullScreen = false;
        configuration.WindowConfiguration.startFullScreen = true;
        configuration.WindowConfiguration.Decorated = true;
        return pnew RuntimeApplication(configuration, projectPath);
    }
}