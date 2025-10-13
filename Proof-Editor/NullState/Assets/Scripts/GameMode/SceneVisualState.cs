using Proof;
using System;
using System.Threading;

namespace NullState
{
    public class SceneVisualState : Entity
    {
        [Seperator]
        public Material GlitchyMaterial;

        void OnCreate()
        {
            InitializeGlitchyMaterial();
        }

        void OnUpdate(float deltaTime)
        {
        }

        void OnPhysicsUpdate(float fixedPhysicsDeltaTime)
        {
        }

        void InitializeGlitchyMaterial()
        {
            if (GlitchyMaterial == null)
            {
                Log.Error("GlitchyMaterial is not assigned in SceneVisualState");
                return;
            }

            GlitchyMaterial.SetInput("u_MaterialUniform.TimeScalee", 0.0);
            GlitchyMaterial.SetInput("u_MaterialUniform.GlitchStrength", 0.0);

        }
        float bufferSize = 0;
        void UpdateGlitchyMaterial()
        {
            bufferSize += World.GetDeltaTime();

            if(bufferSize > 100)
                bufferSize = 0;

            GlitchyMaterial.SetInput("u_MaterialUniform.TimeScalee", Mathf.Sin(Application.GetTime()));
        }
    }
}
