using Proof;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;

namespace ProofScriptCore.src.Proof.Math
{
    [StructLayout(LayoutKind.Sequential)]
    public struct Transform
    {
        public static Transform Zero => new Transform(new Vector(0.0f),new Vector( 0.0f),new Vector(1.0f));

        public Vector Location;
        public Vector Rotation;
        public Vector Scale;

        public Transform(Vector location, Vector rotation, Vector scale)
        {
            Location = location; Rotation = rotation; Scale = scale;
        }

     
    }
}
