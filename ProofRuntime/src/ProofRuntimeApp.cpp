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
    Application* CreateApplication(int argc, char** argv) 
    {
        std::string_view projectPath = "../Proof-Editor/Proof/Proof.ProofProject";

#ifdef PF_DIST
        namespace fs = std::filesystem;
        for (const auto& entry : fs::directory_iterator(fs::current_path())) {
            if (entry.is_regular_file() && entry.path().extension() == ".ProofProject") {
                projectPath = entry.path().string();
                break; // take the first one found
            }
        }
#else
        //projectPath = "Driftwood/Driftwood.ProofProject";
        //projectPath = "PacMan3D/PacMan3D.ProofProject";
        //projectPath = "../Proof-Editor/FlappyBird/FlappyBird.ProofProject";
       // projectPath = "../Proof-Editor/PacMan/PacMan.ProofProject";
        //projectPath = "../Proof-Editor/OrbitBreak/OrbitBreak.ProofProject";
        projectPath = "../Proof-Editor/NullState/NullState.ProofProject";

        if (argc > 1)
            projectPath = argv[1];
#endif

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