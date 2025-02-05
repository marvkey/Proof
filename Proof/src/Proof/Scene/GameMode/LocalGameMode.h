#pragma once
#include "GameMode.h"
#include <unordered_map>
#include "Proof/Core/UUID.h"
#include "Proof/Input/KeyCodes.h"
namespace Proof
{
    struct LocalMultiplayerPlayer
    {
        UUID PlayerUUID;
        Count<class WorldRenderer> Renderer;
    };

    enum class LocalGameModeState
    {
        SinglePlayer,
        MultiPlayer
    };
    class LocalGameMode : public GameMode
    {
    public:
        // Constructor: Initializes the game mode type to Local Multiplayer
        LocalGameMode(Count<World> world)
            : GameMode(GameModeTypes::LocalGameMode, world)
        {
        }

        virtual void Start();
        virtual void End();
        virtual void RenderRuntime(Count<class WorldRenderer> renderer, FrameTime time);
        
        void CreatePlayer(UUID enittyID,Players player = Players::None);
    private:
        virtual void RenderRuntimeSinglePlayer(Count<class WorldRenderer> renderer, FrameTime time);
        virtual void RenderRuntimeMultiPlayer(Count<class WorldRenderer> renderer, FrameTime time);

        bool RenderPlayer(LocalMultiplayerPlayer& player, FrameTime time);
        void GenerateMultiplayerPipeLines();

        void RenderWorldHUD(Count<class WorldRenderer> renderer);
    private:
        LocalGameModeState m_LocalGameModeSate;
        Count<class RenderPass> m_PlayerGridPass;
        Count<class RenderCommandBuffer> m_CommandBuffer;
        std::vector<LocalMultiplayerPlayer> m_Players; // entityID
    };
}