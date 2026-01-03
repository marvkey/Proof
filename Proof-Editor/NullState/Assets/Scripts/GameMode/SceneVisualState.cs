using Proof;
using System;
using System.Threading;

namespace NullState
{
    public class SceneVisualState : Entity
    {
        [Seperator]
        public Material GlitchyMaterial;
        MaterialVariable m_GlitchyMaterialTimeScale;
        MaterialVariable m_GlitchyMaterialGlitchStrength;

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
            m_GlitchyMaterialTimeScale= GlitchyMaterial.GetVariable("u_MaterialUniform.TimeScalee");
            m_GlitchyMaterialGlitchStrength = GlitchyMaterial.GetVariable("u_MaterialUniform.GlitchStrength");

            m_GlitchyMaterialTimeScale.Set(0.0f);
            m_GlitchyMaterialGlitchStrength.Set(0.0f);

        }
        float bufferSize = 0;
        void UpdateGlitchyMaterial()
        {
            bufferSize += World.GetDeltaTime();

            if(bufferSize > 100)
                bufferSize = 0;

            GlitchyMaterial.SetInput("u_MaterialUniform.TimeScalee", Mathf.Sin(Application.GetTime()));
        }


        void InitilizeTaskManager()
        {

        }
    }
}
