#include "Proof/Core/Proof.h"
#include "Proof/Core/Core.h"
#include "Proof/Core/Application.h"
#include <filesystem>
#include "Editor3D/Editor3D.h"
#include "Proof/Utils/FileSystem.h"
#include "Proof/Project/Project.h"
namespace Proof {
    class ProofEditor : public Application {
    public:
        ProofEditor(const ApplicationConfiguration& configuration, std::string_view projectPath):
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
            // filesyste
            {

                std::filesystem::path workingDirectory = std::filesystem::current_path();
                const std::string stemOfdire = workingDirectory.stem().string();
                //if (stemOfdire == "Proof-Editor")
                    workingDirectory = workingDirectory.parent_path();
                FileSystem::SetEnvironmentVariable("PROOF_DIR", workingDirectory.string());
                //PF_ENGINE_TRACE("     PROOF_DIR {}", (FileSystem::GetEnvironmentVariable)("PROOF_DIR"));

            }
            m_Editor =Count<Editore3D>::Create();
            PushLayer(m_Editor);
        }

    private:
        Count< class Editore3D> m_Editor = nullptr;
    };
    Application* CreateApplication(int argc, char** argv) {

        std::string_view projectPath;
        if (argc > 1)
            projectPath = argv[1];
        ApplicationConfiguration configuration;
        configuration.Name = "Proof Editor";
        configuration.EnableImgui = true;
        configuration.ProjectPath = projectPath;
        configuration.WindowConfiguration.Height = 500;
        configuration.WindowConfiguration.Width = 800;
        configuration.WindowConfiguration.Vsync = true;
        configuration.WindowConfiguration.Title = "Proof";
        
        //when set to true
        // a bug happens when we change to another apllication while running proof
        // it crahses
        configuration.WindowConfiguration.startWindowedFullScreen = false;
        configuration.WindowConfiguration.startFullScreen = true;
        configuration.WindowConfiguration.Decorated = true;
        return pnew ProofEditor(configuration, projectPath);
    }
}