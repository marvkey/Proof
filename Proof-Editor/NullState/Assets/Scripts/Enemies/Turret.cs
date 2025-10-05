using System;
using System.Runtime.InteropServices;
using Proof;

namespace NullState
{
    public class Turret : Entity
    {
        Entity m_Target;

        public Entity Muzzle;
        public float Range = 15.0f;

        public bool HasTarget => m_Target != null;  

        void OnCreate()
        {
            InvokeRepeat(SphereCheck, 1.0f);
        }

        void OnUpdate(float deltaTime)
        {

        }

        void SphereCheck()
        {
           Entity[] entities = Physics.OverlapSphere(Transform.Location, Range);

            if (entities.Length == 0)
            {
                if(m_Target != null)
                    Log.Trace("Target Lost: " + m_Target.Name);

                m_Target = null;
                return;
            }

            foreach (Entity entity in entities)
            {
                if(entity.HasScript<Character>())
                {
                    if(m_Target == entity)
                        return;
                    m_Target = entity;
                    Log.Trace("Target Acquired: " + m_Target.Name);
                    return;
                }
            }

            if (m_Target != null)
                Log.Trace("Target Lost: " + m_Target.Name);

            m_Target = null;
        }
    }
}
