using OrbitBreak;
using Proof;
using System;
using System.IO;
using System.Security.Cryptography;
namespace Proof
{
    class GameManager :Entity
    {
        public Entity Player;

        public Texture2D Lazer;
        public Texture2D Shield;
        public Texture2D Speed;

        public UIPanel PowerupPanel;
        public UIPanel RestartPanel;
        public Prefab ExplosionLazer;

        float RestartCountdown = 3.0f;

        int playerLastScore = 0;
        int HighScore = 0;

        private string filePath = "HighScore.txt";

        

        // OnCreate is called once when the Entity that this script is attached to
        // is instantiated in the world at runtime
        void OnCreate()
        {
            GetComponent<PlayerHUDComponent>().PushPanel("HUD", PowerupPanel, false);
            GetComponent<PlayerHUDComponent>().PushPanel("HUD", RestartPanel, false);

            LoadHighScore();
        }

        // OnUpdate is called once every frame while this script is active in the world
        void OnUpdate(float deltaTime)
        {

            UpdateUI();

           
        }

        void SaveScore()
        {
            if (playerLastScore > HighScore)
            {
                HighScore = playerLastScore;
                File.WriteAllText(filePath, HighScore.ToString());
                Log.Info("High score saved: " + HighScore);
            }
        }

        public void LoadHighScore()
        {
            if (File.Exists(filePath))
            {
                string text = File.ReadAllText(filePath);
                if (int.TryParse(text, out int savedScore))
                {
                    HighScore = savedScore;
                    Log.Info("High score loaded: " + HighScore);
                }
                else
                {
                    Log.Warn("Failed to parse high score from file.");
                    HighScore = 0;
                }
            }
            else
            {
                Log.Warn("No high score file found. Starting fresh.");
                HighScore = 0;
            }
        }


        // OnPhysicsUpdate is called at a fixed timestep for physics-related logic
        void OnPhysicsUpdate(float fixedPhysicsDeltaTime)
        {
        }

        void UpdateUI()
        {
            if (Entity.IsValid(Player) == false)
            {
                SaveScore();
                GetComponent<PlayerHUDComponent>().GetRegistryVariableByPanelIndex(3, 0, "Health")?.SetData((0.0f));
                GetComponent<PlayerHUDComponent>().SetPanelVisible(3, PowerupPanel,false);

                GetComponent<PlayerHUDComponent>().SetPanelVisible(3, RestartPanel, true);

                if(RestartCountdown <= 0.0f)
                {
                World.Restart();

                }
                var s = string.Format("{0:0.00}", RestartCountdown);
                 GetComponent<PlayerHUDComponent>().GetRegistryVariableByPanelIndex(3, 2, "RestartTime")?.SetData(s);
                 RestartCountdown -= World.GetDeltaTime();

                return;
            }


            float health = Player.GetScript<Player>().Health;

            GetComponent<PlayerHUDComponent>().GetRegistryVariableByPanelIndex(3,0,"Health")?.SetData((float)health /100.0f);

            int score = (int)Player.GetScript<Player>().Score;

            playerLastScore = score;
            GetComponent<PlayerHUDComponent>().GetRegistryVariableByPanelIndex(3,0,"Score")?.SetData<string>(score.ToString());
            GetComponent<PlayerHUDComponent>().GetRegistryVariableByPanelIndex(3,0,"HighScore")?.SetData<string>(HighScore.ToString());


            if (Player.GetScript<Player>().PowerUpState != PowerUp.State.None)
            {
                GetComponent<PlayerHUDComponent>().SetPanelVisible(3, PowerupPanel, true);

                Texture2D icon = null;
                float time = 0.0f;
                switch (Player.GetScript<Player>().PowerUpState)
                {
                    case PowerUp.State.Gun:
                        icon = Lazer;
                        time = Player.GetScript<PowerUp>().PercentLeft;
                        break;
                    case PowerUp.State.Shield:
                        icon = Shield;
                        time = Player.GetScript<PowerUp>().PercentLeft;
                        break;
                    case PowerUp.State.Speed:
                        icon = Speed;
                        time = Player.GetScript<PowerUp>().PercentLeft;
                        break;
                }
                GetComponent<PlayerHUDComponent>().GetRegistryVariableByPanelIndex(3, 1, "Icon")?.SetData<Texture2D>(icon);
                GetComponent<PlayerHUDComponent>().GetRegistryVariableByPanelIndex(3, 1, "Time")?.SetData<float>(time );
            }
            else
            {
                GetComponent<PlayerHUDComponent>().SetPanelVisible(3, PowerupPanel, false);
            }
        }
    }
}
