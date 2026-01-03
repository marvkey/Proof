using System;
using System.Collections.Generic;
using System.Linq;
using System.Numerics;
using System.Runtime.Remoting.Messaging;
using System.Text;
using System.Threading.Tasks;
using System.Runtime.InteropServices;
namespace Proof
{
    public static class Mathf
    {

        public const float Epsilon = 0.00001f;
        public const float PI = (float)Math.PI;
        public const float PIonTwo = (float)(Math.PI / 2.0f);
        public const float TwoPI = (float)(Math.PI * 2.0);
        public const float Deg2Rad = PI / 180.0f;
        public const float Rad2Deg = 180.0f / PI;
        public static float DegreesToRadians(float degrees)
        {
            return degrees * Mathf.PI / 180f;
        }

        // Converts radians to degrees
        public static float RadiansToDegrees(float radians)
        {
            return radians * 180f / Mathf.PI;
        }

        public static Vector3 RadiansToDegrees(Vector3 radians)
        {
            return radians * (180f / Mathf.PI);
        }

        public static float Sin(float value) => (float)Math.Sin(value);
        public static float Cos(float value) => (float)Math.Cos(value);
        public static float Acos(float value) => (float)Math.Acos(value);

        public static float Tan(float value) => (float)Math.Tan(value);

        public static float Clamp(float value, float min, float max)
        {
            if (value < min)
                return min;
            return value > max ? max : value;
        }
      
        public static float Lerp(float start, float end, float t)
        {
            // Clamp t to the range [0, 1]
            t = Mathf.Clamp(t,0,1);

            // Perform the linear interpolation
            return start + (end - start) * t;
        }

        public static float InverseLerp(float a, float b, float value)
        {
            if (Mathf.Abs(b - a) < 1e-6f)
                return 0f; // Prevent divide-by-zero

            return Mathf.Clamp((value - a) / (b - a),0.0f,1.0f);
        }
        public static float Asin(float x) => (float)Math.Asin(x);
        public static float Atan(float x) => (float)Math.Atan(x);
        public static float Atan2(float y, float x) => (float)Math.Atan2(y, x);

        public static float Min(float v0, float v1) => v0 < v1 ? v0 : v1;
        public static float Max(float v0, float v1) => v0 > v1 ? v0 : v1;

        public static float Sqrt(float value) => (float)Math.Sqrt(value);

        public static float Abs(float value) => Math.Abs(value);
        public static int Abs(int value) => Math.Abs(value);

        public static float SmoothStep(float edge0, float edge1, float x)
        {
            // Scale, bias and saturate x to 0..1 range
            x = Mathf.Clamp((x - edge0) / (edge1 - edge0),0,1);
            // Evaluate polynomial
            return x * x * (3 - 2 * x);
        }

        public static Proof.Vector3 Abs(Proof.Vector3 value)
        {
            return new Proof.Vector3(Math.Abs(value.x), Math.Abs(value.y), Math.Abs(value.z));
        }

        public static readonly Vector3 Forward = new Vector3(0F, 0F, -1F);
        public static readonly Vector3 Back = -Forward;
        public static readonly Vector3 Up = new Vector3(0F, 1F, 0F);
        public static readonly Vector3 Down = -Up;
        public static readonly Vector3 Right = new Vector3(1F, 0F, 0F);
        public static readonly Vector3 Left = -Right;

    // public static float Lerp(float p1, float p2, float t) => Interpolate.Linear(p1, p2, t);
    // public static Proof.Vector3 Lerp(Proof.Vector3 p1, Proof.Vector3 p2, float t) => Interpolate.Linear(p1, p2, t);

    public static float Floor(float value) => (float)Math.Floor(value);

        // not the same as a%b
        public static float Modulo(float a, float b) => a - b * (float)Math.Floor(a / b);

        public static float Distance(float p1, float p2) => Abs(p1 - p2);

        //public static float PerlinNoise(float x, float y) => Noise.Perlin(x, y);

        public static int CeilToInt(float value) => (int)Math.Ceiling(value);
        public static int FloorToInt(float value) => (int)Math.Floor(value);
    }


    [StructLayout(LayoutKind.Explicit)]
    public struct TransferMatrix4
    {
        [FieldOffset( 0)] public float D00;
        [FieldOffset( 4)] public float D10;
        [FieldOffset( 8)] public float D20;
        [FieldOffset(12)] public float D30;
        [FieldOffset(16)] public float D01;
        [FieldOffset(20)] public float D11;
        [FieldOffset(24)] public float D21;
        [FieldOffset(28)] public float D31;
        [FieldOffset(32)] public float D02;
        [FieldOffset(36)] public float D12;
        [FieldOffset(40)] public float D22;
        [FieldOffset(44)] public float D32;
        [FieldOffset(48)] public float D03;
        [FieldOffset(52)] public float D13;
        [FieldOffset(56)] public float D23;
        [FieldOffset(60)] public float D33;

        public TransferMatrix4(float value)
        {
            D00 = value; D10 = 0.0f; D20 = 0.0f; D30 = 0.0f;
            D01 = 0.0f; D11 = value; D21 = 0.0f; D31 = 0.0f;
            D02 = 0.0f; D12 = 0.0f; D22 = value; D32 = 0.0f;
            D03 = 0.0f; D13 = 0.0f; D23 = 0.0f; D33 = value;
        }

        public void DebugPrint()
        {
            Console.WriteLine($"{D00:0.00}  {D10:0.00}  {D20:0.00}  {D30:0.00}");
            Console.WriteLine($"{D01:0.00}  {D11:0.00}  {D21:0.00}  {D31:0.00}");
            Console.WriteLine($"{D02:0.00}  {D12:0.00}  {D22:0.00}  {D32:0.00}");
            Console.WriteLine($"{D03:0.00}  {D13:0.00}  {D23:0.00}  {D33:0.00}");
        }


        public static Matrix4 ToProof(TransferMatrix4 src)
        {
            Matrix4 dst = new Matrix4();

            // Column 0
            dst.m00 = src.D00;
            dst.m01 = src.D10;
            dst.m02 = src.D20;
            dst.m03 = src.D30;

            // Column 1
            dst.m10 = src.D01;
            dst.m11 = src.D11;
            dst.m12 = src.D21;
            dst.m13 = src.D31;

            // Column 2
            dst.m20 = src.D02;
            dst.m21 = src.D12;
            dst.m22 = src.D22;
            dst.m23 = src.D32;

            // Column 3
            dst.m30 = src.D03;
            dst.m31 = src.D13;
            dst.m32 = src.D23;
            dst.m33 = src.D33;


            return dst;
        }

        public static TransferMatrix4 ToTransfer(Matrix4 src)
        {
            TransferMatrix4 dst = new TransferMatrix4();

            // Column 0
            dst.D00 = src.m00;
            dst.D10 = src.m01;
            dst.D20 = src.m02;
            dst.D30 = src.m03;

            // Column 1
            dst.D01 = src.m10;
            dst.D11 = src.m11;
            dst.D21 = src.m12;
            dst.D31 = src.m13;

            // Column 2
            dst.D02 = src.m20;
            dst.D12 = src.m21;
            dst.D22 = src.m22;
            dst.D32 = src.m23;

            // Column 3
            dst.D03 = src.m30;
            dst.D13 = src.m31;
            dst.D23 = src.m32;
            dst.D33 = src.m33;

            return dst;
        }
    }
}
