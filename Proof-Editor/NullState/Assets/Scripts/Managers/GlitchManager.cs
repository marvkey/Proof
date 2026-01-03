using System;
using Proof;
using System.Collections.Generic;
using System.Reflection;

namespace NullState
{
    public struct GlitchInstance
    {
        public Entity Target;       // Who is being glitched
        public Material GlitchMat;  // Which material to use
        public float Duration;      // Total time to apply glitch
        public float Elapsed;       // How long it has been active
        public bool IncludeChildren;

        public GlitchInstance(Entity target, Material mat, float duration, bool includeChildren)
        {
            Target = target;
            GlitchMat = mat;
            Duration = duration;
            Elapsed = 0f;
            IncludeChildren = includeChildren;
        }

        public bool IsFinished => Elapsed >= Duration;
    }
    public class GlitchManager  : Entity
    {
        [ShowInEditorAttribute("Glitches")]
        Material[] m_GlitchMaterials;
        List<GlitchInstance> m_GlitchInstances;

        void OnCreate()
        {
            m_GlitchInstances = new List<GlitchInstance>();
        }

        void OnUpdate(float deltaTime)
        {
            ProcessGlitches(deltaTime);
        }

        public Material GetGlitch()
        {
            if(m_GlitchMaterials.Length == 0)
                return null;
            return m_GlitchMaterials[Proof.Random.Int(0,m_GlitchMaterials.Length-1)];
        }

        public void QueueGlitch(Entity e, float time = 5.0f,bool includeChildren = true)
        {
            
             if(!Entity.IsValid(e))
                 return;
              
             if(m_GlitchMaterials.Length == 0)
             {
                 Log.Warn("Glitch material List is empty");
                 return;
             }

             Material mat = GetGlitch();
             if(mat == null)
             {
                 Log.Error("Glitch material has an index with a null material");
                 return;
             }

             GlitchInstance instance = new GlitchInstance(e, mat,time, includeChildren);
             m_GlitchInstances.Add(instance);
             Log.Trace("Glitch instance added: " + instance.Target.Name);
             
        }

        void ProcessGlitches(float deltaTime)
        {
            if (m_GlitchInstances == null)
                return;

            for (int i = m_GlitchInstances.Count - 1; i >= 0; i--)
            {
                var glitch = m_GlitchInstances[i];
                glitch.Elapsed += deltaTime;

                if (!Entity.IsValid(glitch.Target))
                {
                    m_GlitchInstances.RemoveAt(i);
                    continue;
                }

                ApplyGlitchRecursive(glitch.Target, glitch);

                if (glitch.IsFinished)
                    m_GlitchInstances.RemoveAt(i);
                else
                    m_GlitchInstances[i] = glitch; // struct write-back
            }
        }

        void ApplyGlitchRecursive(Entity entity, GlitchInstance glitch)
        {
            if (!Entity.IsValid(entity))
                return;

            if (entity.HasComponent<DynamicMeshComponent>())
                ImmediateRenderer.SubmitDynamicMeshComponent(
                    entity.GetComponent<DynamicMeshComponent>(),
                    glitch.GlitchMat
                );

            if (entity.HasComponent<MeshComponent>())
            {
                
                ImmediateRenderer.SubmitMeshComponent(
                    entity.GetComponent<MeshComponent>(),
                    glitch.GlitchMat
                );

            }

           // Log.Info(entity.WorldTransform.ToString());

            if (!glitch.IncludeChildren)
                return;

            var children = entity.GetChildren();
            if (children == null)
                return;

            foreach (var child in children)
                ApplyGlitchRecursive(child, glitch);
        }


    }
}