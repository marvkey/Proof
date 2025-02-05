#pragma once
#include "Proof/Core/Core.h"
#include "Proof/Core/FrameTime.h"
namespace Proof
{
    class World;
	enum class GameModeTypes
	{
		None = 0,
		LocalGameMode,
		ServerGameMode, 
	};

    class GameMode : public RefCounted
    {
    public:
        GameMode(GameModeTypes mode, Count<World> world)
            : m_CurrentMode(mode), m_World(world) {}

        // Get the current game mode
        GameModeTypes GetGameMode() const
        {
            return m_CurrentMode;
        }

        virtual void Start() = 0;
        virtual void End() = 0;
        virtual void RenderRuntime(Count<class WorldRenderer> renderer, FrameTime time) = 0;
        virtual void Update(float deltaTime) {}; // Update logic (called every frame)

        virtual void HandlePlayerJoin(UUID entityID) {}; // Handle a player joining the game
        virtual void HandlePlayerLeave(UUID entityID) {}; // Handle a player leaving the game
    protected:
        GameModeTypes m_CurrentMode;  
        Count< World> m_World;
    };
}