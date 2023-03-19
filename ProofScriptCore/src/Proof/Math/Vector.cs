using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;

namespace Proof
{
    [StructLayout(LayoutKind.Sequential)]
    public struct Vector
    {
        public float X,Y,Z;
        public static Vector Zero => new Vector(0.0f);
        public Vector(float x, float y, float z)
        {
            X = x;
            Y = y;  
            Z = z;  
        }
        public Vector(float scalar)
        {
            X = scalar;
            Y = scalar;
            Z = scalar;
        }

        public static Vector operator +(Vector a, Vector b)
        {
            return new Vector(a.X + b.X, a.Y + b.Y, a.Z + b.Z);
        }

        public override string ToString()
        {
            return $"X = {X} Y = {Y} Z = {Z}";
        }
        public float GetLength()
        {
            return (float)Math.Sqrt(X*X + Y*Y + Z*Z);
        }
        public Vector Normalize()
        {
            float length = GetLength();
            return new Vector(X/length, Y/length, Z/length);
        }

        public float GetDot(Vector vec)
        {
            return (X * vec.X + Y * vec.Y + Z * vec.Z);
        }
    }


}
