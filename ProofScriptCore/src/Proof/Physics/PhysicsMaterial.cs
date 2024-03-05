using Proof;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Proof
{

    public enum PhysicsMaterialCombineMode
    {
        Average = 0,
		Min = 1,
		Mutltiply = 2,
		Max = 3
	};
    [RegisterCoreClassStruct]
    public class PhysicsMaterial
    {
        internal AssetID m_Handle;
        public AssetID Handle => m_Handle;

        internal PhysicsMaterial() { m_Handle = AssetID.Invalid; }
        internal PhysicsMaterial(AssetID handle)
        {
            m_Handle = handle;
        }

        public float StaticFriction
        {
            get => InternalCalls.PhysicsMaterial_GetStaticFriction(ref m_Handle);
            set => InternalCalls.PhysicsMaterial_SetStaticFriction(ref m_Handle, value);
        }

        public float DynamicFriction
        {
            get => InternalCalls.PhysicsMaterial_GetDynamicFriction(ref m_Handle);
            set => InternalCalls.PhysicsMaterial_SetDynamicFriction(ref m_Handle, value);
        }

        public float Bounciness
        {
            get => InternalCalls.PhysicsMaterial_GetBounciness(ref m_Handle);
            set => InternalCalls.PhysicsMaterial_SetBounciness(ref m_Handle, value);
        }
        PhysicsMaterialCombineMode FrictionCombineMode
        {

            get => InternalCalls.PhysicsMaterial_GetFrictionCombineMode(ref m_Handle); 
            set => InternalCalls.PhysicsMaterial_SetFrictionCombineMode(ref m_Handle, value);
        }

        PhysicsMaterialCombineMode BouncinessCombineMode
        {
            get => InternalCalls.PhysicsMaterial_GetBouncinessCombineMode(ref m_Handle);
            set => InternalCalls.PhysicsMaterial_SetBouncinessCombineMode(ref m_Handle, value);
        }
    }
}
