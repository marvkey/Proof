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
    public struct Vector3
    {
        public float X,Y,Z;
        public static Vector3 Zero => new Vector3(0.0f);
        public Vector3(Vector3 other)
        {
            X = other.X;
            Y = other.Y;
            Z = other.Z;
        }
        public Vector3(float x, float y, float z)
        {
            X = x;
            Y = y;  
            Z = z;  
        }
        public Vector3(float scalar)
        {
            X = scalar;
            Y = scalar;
            Z = scalar;
        }
        public static Vector3 operator +(Vector3 a) => a;
        public static Vector3 operator -(Vector3 a) => new Vector3(-a.X, -a.Y, -a.Z);
        public static Vector3 operator -(Vector3 a, Vector3 b) {

           return new Vector3(a.X - b.X, a.Y - b.Y, a.Z - b.Z);
        }
        public static Vector3 operator +(Vector3 a, Vector3 b)
        {
            return new Vector3(a.X + b.X, a.Y + b.Y, a.Z + b.Z);
        }

        public static Vector3 operator +(Vector3 a, float b)
        {
            return new Vector3(a.X + b, a.Y + b, a.Z + b);
        }

        public override string ToString()
        {
            return $"X = {X} Y = {Y} Z = {Z}";
        }
        public float GetLength()
        {
            return (float)Math.Sqrt(X*X + Y*Y + Z*Z);
        }
        public Vector3 Normalize()
        {
            float length = GetLength();
            return new Vector3(X/length, Y/length, Z/length);
        }

        public float GetDot(Vector3 vec)
        {
            return (X * vec.X + Y * vec.Y + Z * vec.Z);
        }

        public static Vector3 operator *(Vector3 a,float sclar)
        {
            return new Vector3(a.X *sclar, a.Y * sclar, a.Z * sclar);
        }
    }
    [RegisterCoreClassStruct]
    [StructLayout(LayoutKind.Sequential)]
    public struct Vector2
    {
        public float X, Y;
        public static Vector2 Zero => new Vector2(0.0f);
        public Vector2(Vector2 other)
        {
            X = other.X;
            Y = other.Y;
        }
        public Vector2(float x, float y)
        {
            X = x;
            Y = y;
        }
        public Vector2(float scalar)
        {
            X = scalar;
            Y = scalar;
        }

        public static Vector2 operator +(Vector2 a, Vector2 b)
        {
            return new Vector2(a.X + b.X, a.Y + b.Y);
        }

        public static Vector2 operator +(Vector2 a, float b)
        {
            return new Vector2(a.X + b, a.Y + b);
        }

        public override string ToString()
        {
            return $"X = {X} Y = {Y} ";
        }
        public float GetLength()
        {
            return (float)Math.Sqrt(X * X + Y * Y );
        }
        public Vector2 Normalize()
        {
            float length = GetLength();
            return new Vector2(X / length, Y / length);
        }


        public static Vector2 operator *(Vector2 a, float sclar)
        {
            return new Vector2(a.X * sclar, a.Y * sclar);
        }
    }

    [RegisterCoreClassStruct]
    [StructLayout(LayoutKind.Sequential)]
    public struct Vector4
    {
        public float X, Y,Z,W;
        public static Vector2 Zero => new Vector2(0.0f);
        public Vector4(Vector4 other)
        {
            X = other.X;
            Y = other.Y;
            Z = other.Z;
            W = other.W;
        }
        public Vector4(float x, float y,float z, float w)
        {
            X = x;
            Y = y;
            Z = z;      
            W = w;
        }
        public Vector4(float scalar)
        {
            X = scalar;
            Y = scalar;
            Z = scalar; 
            W = scalar;
        }

        public static Vector4 operator +(Vector4 a, Vector4 b)
        {
            return new Vector4(a.X + b.X, a.Y + b.Y, a.Z + b.Z, a.W +b.W);
        }

        public static Vector4 operator +(Vector4 a, float b)
        {
            return new Vector4(a.X + b, a.Y + b, a.Z + b, a.W + b);
        }

        public override string ToString()
        {
            return $"X = {X} Y = {Y} Z = {Z}  W = {W} ";
        }
        public float GetLength()
        {
            return (float)Math.Sqrt(X * X + Y * Y + Z*Z + W*W);
        }
        public Vector4 Normalize()
        {
            float length = GetLength();
            return new Vector4(X / length, Y / length,Z/length,W/length);
        }


        public static Vector4 operator *(Vector4 a, float sclar)
        {
            return new Vector4(a.X * sclar, a.Y * sclar,a.Z* sclar,a.W* sclar);
        }
    }
}
