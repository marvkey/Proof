using Proof;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Proof
{
    public enum ParticleHandlerState
    {
        None,
		Play,
		Pause,
		End
    };
    [RegisterCoreClassStruct]

    public class ParticleSystem
    {
        public uint TableIndex { get; internal set; }
        public Entity Entity { get; internal set; }

        internal ParticleSystem(uint index, Entity entity)
        {
            Entity = entity;
            TableIndex = index;
        }
        public bool Visible
        {
            get { return InternalCalls.ParticleEffectComponent_GetVisible(Entity.ID, TableIndex); }
            set
            {
                InternalCalls.ParticleEffectComponent_SetVisible(Entity.ID, TableIndex, ref value);
            }
        }

        public void Play() => InternalCalls.ParticleEffectComponent_Play(Entity.ID, TableIndex);
        public void Pause() => InternalCalls.ParticleEffectComponent_Pause(Entity.ID, TableIndex);
        public void End() => InternalCalls.ParticleEffectComponent_End(Entity.ID, TableIndex);
        public void Restart() => InternalCalls.ParticleEffectComponent_Restart(Entity.ID, TableIndex);

        public ParticleHandlerState GetState() => (ParticleHandlerState)InternalCalls.ParticleEffectComponent_GetState(Entity.ID, TableIndex); 
    }
}
