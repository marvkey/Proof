using Proof;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace ProofScriptCore.src.Proof.Asset
{
    struct ParticleSystemProps
    {

    }
    public enum ParticleHandlerState
    {
        None,
		Play,
		Pause,
		End
    };
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
            get { return InternalCalls.ParticleSystemComponent_GetVisible(Entity.ID, TableIndex); }
            set
            {
                InternalCalls.ParticleSystemComponent_SetVisible(Entity.ID, TableIndex, ref value);
            }
        }

        public void Play() => InternalCalls.ParticleSystemComponent_Play(Entity.ID, TableIndex);
        public void Pause() => InternalCalls.ParticleSystemComponent_Pause(Entity.ID, TableIndex);
        public void End() => InternalCalls.ParticleSystemComponent_End(Entity.ID, TableIndex);
        public void Restart() => InternalCalls.ParticleSystemComponent_Restart(Entity.ID, TableIndex);

        public ParticleHandlerState GetState() => (ParticleHandlerState)InternalCalls.ParticleSystemComponent_GetState(Entity.ID, TableIndex); 
    }
}
