using Proof;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;

namespace Proof
{
    [RegisterCoreClassStruct]
    [StructLayout(LayoutKind.Sequential)]
    public struct Transform
    {
        public static Transform Zero => new Transform(new Proof.Vector3(0.0f),new Proof.Vector3( 0.0f),new Proof.Vector3(1.0f));

        public Proof.Vector3 Location;
        public Proof.Vector3 Rotation;
        public Proof.Vector3 Scale;

        public Transform(Proof.Vector3 location, Proof.Vector3 rotation, Proof.Vector3 scale)
        {
            Location = location; Rotation = rotation; Scale = scale;
        }
        public override string ToString()
        {
            return $"Transform(" +
                   $"Location: {Location}, " +
                   $"Rotation: {Rotation}, " +
                   $"Scale: {Scale})";
        }

    }
}
