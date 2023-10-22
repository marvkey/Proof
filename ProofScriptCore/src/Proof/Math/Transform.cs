using Proof;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;

namespace Proof
{
    [StructLayout(LayoutKind.Sequential)]
    public struct Transform
    {
        public static Transform Zero => new Transform(new Vector3(0.0f),new Vector3( 0.0f),new Vector3(1.0f));

        public Vector3 Location;
        public Vector3 Rotation;
        public Vector3 Scale;

        public Transform(Vector3 location, Vector3 rotation, Vector3 scale)
        {
            Location = location; Rotation = rotation; Scale = scale;
        }
    }
}
