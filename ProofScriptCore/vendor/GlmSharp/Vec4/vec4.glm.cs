using System;
using System.Collections;
using System.Collections.Generic;
using System.Globalization;
using System.Runtime.InteropServices;

using System.Numerics;
using System.Linq;
using Proof.Swizzle;

// ReSharper disable InconsistentNaming

namespace Proof
{
    /// <summary>
    /// Static class that contains static glm functions
    /// </summary>
    public static partial class glm
    {
        
        /// <summary>
        /// Returns an object that can be used for arbitrary swizzling (e.g. swizzle.zy)
        /// </summary>
        public static swizzle_Vector4 swizzle(Vector4 v) => v.swizzle;
        
        /// <summary>
        /// Returns an array with all values
        /// </summary>
        public static float[] Values(Vector4 v) => v.Values;
        
        /// <summary>
        /// Returns an enumerator that iterates through all components.
        /// </summary>
        public static IEnumerator<float> GetEnumerator(Vector4 v) => v.GetEnumerator();
        
        /// <summary>
        /// Returns a string representation of this Vector using ', ' as a seperator.
        /// </summary>
        public static string ToString(Vector4 v) => v.ToString();
        
        /// <summary>
        /// Returns a string representation of this Vector using a provided seperator.
        /// </summary>
        public static string ToString(Vector4 v, string sep) => v.ToString(sep);
        
        /// <summary>
        /// Returns a string representation of this Vector using a provided seperator and a forMatrix provider for each component.
        /// </summary>
        public static string ToString(Vector4 v, string sep, IFormatProvider provider) => v.ToString(sep, provider);
        
        /// <summary>
        /// Returns a string representation of this Vector using a provided seperator and a forMatrix for each component.
        /// </summary>
        public static string ToString(Vector4 v, string sep, string forMatrix) => v.ToString(sep, forMatrix);
        
        /// <summary>
        /// Returns a string representation of this Vector using a provided seperator and a forMatrix and forMatrix provider for each component.
        /// </summary>
        public static string ToString(Vector4 v, string sep, string forMatrix, IFormatProvider provider) => v.ToString(sep, forMatrix, provider);
        
        /// <summary>
        /// Returns the number of components (4).
        /// </summary>
        public static int Count(Vector4 v) => v.Count;
        
        /// <summary>
        /// Returns true iff this equals rhs component-wise.
        /// </summary>
        public static bool Equals(Vector4 v, Vector4 rhs) => v.Equals(rhs);
        
        /// <summary>
        /// Returns true iff this equals rhs type- and component-wise.
        /// </summary>
        public static bool Equals(Vector4 v, object obj) => v.Equals(obj);
        
        /// <summary>
        /// Returns a hash code for this instance.
        /// </summary>
        public static int GetHashCode(Vector4 v) => v.GetHashCode();
        
        /// <summary>
        /// Returns true iff distance between lhs and rhs is less than or equal to epsilon
        /// </summary>
        public static bool ApproxEqual(Vector4 lhs, Vector4 rhs, float eps = 0.1f) => Vector4.ApproxEqual(lhs, rhs, eps);
        
        /// <summary>
        /// Returns a bVector4 from component-wise application of Equal (lhs == rhs).
        /// </summary>
        public static bVector4 Equal(Vector4 lhs, Vector4 rhs) => Vector4.Equal(lhs, rhs);
        
        /// <summary>
        /// Returns a bVector4 from component-wise application of NotEqual (lhs != rhs).
        /// </summary>
        public static bVector4 NotEqual(Vector4 lhs, Vector4 rhs) => Vector4.NotEqual(lhs, rhs);
        
        /// <summary>
        /// Returns a bVector4 from component-wise application of GreaterThan (lhs &gt; rhs).
        /// </summary>
        public static bVector4 GreaterThan(Vector4 lhs, Vector4 rhs) => Vector4.GreaterThan(lhs, rhs);
        
        /// <summary>
        /// Returns a bVector4 from component-wise application of GreaterThanEqual (lhs &gt;= rhs).
        /// </summary>
        public static bVector4 GreaterThanEqual(Vector4 lhs, Vector4 rhs) => Vector4.GreaterThanEqual(lhs, rhs);
        
        /// <summary>
        /// Returns a bVector4 from component-wise application of LesserThan (lhs &lt; rhs).
        /// </summary>
        public static bVector4 LesserThan(Vector4 lhs, Vector4 rhs) => Vector4.LesserThan(lhs, rhs);
        
        /// <summary>
        /// Returns a bVector4 from component-wise application of LesserThanEqual (lhs &lt;= rhs).
        /// </summary>
        public static bVector4 LesserThanEqual(Vector4 lhs, Vector4 rhs) => Vector4.LesserThanEqual(lhs, rhs);
        
        /// <summary>
        /// Returns a bVector4 from component-wise application of IsInfinity (float.IsInfinity(v)).
        /// </summary>
        public static bVector4 IsInfinity(Vector4 v) => Vector4.IsInfinity(v);
        
        /// <summary>
        /// Returns a bVector4 from component-wise application of IsFinite (!float.IsNaN(v) &amp;&amp; !float.IsInfinity(v)).
        /// </summary>
        public static bVector4 IsFinite(Vector4 v) => Vector4.IsFinite(v);
        
        /// <summary>
        /// Returns a bVector4 from component-wise application of IsNaN (float.IsNaN(v)).
        /// </summary>
        public static bVector4 IsNaN(Vector4 v) => Vector4.IsNaN(v);
        
        /// <summary>
        /// Returns a bVector4 from component-wise application of IsNegativeInfinity (float.IsNegativeInfinity(v)).
        /// </summary>
        public static bVector4 IsNegativeInfinity(Vector4 v) => Vector4.IsNegativeInfinity(v);
        
        /// <summary>
        /// Returns a bVector4 from component-wise application of IsPositiveInfinity (float.IsPositiveInfinity(v)).
        /// </summary>
        public static bVector4 IsPositiveInfinity(Vector4 v) => Vector4.IsPositiveInfinity(v);
        
        /// <summary>
        /// Returns a Vector4 from component-wise application of Abs (Math.Abs(v)).
        /// </summary>
        public static Vector4 Abs(Vector4 v) => Vector4.Abs(v);
        
        /// <summary>
        /// Returns a Vector4 from component-wise application of HermiteInterpolationOrder3 ((3 - 2 * v) * v * v).
        /// </summary>
        public static Vector4 HermiteInterpolationOrder3(Vector4 v) => Vector4.HermiteInterpolationOrder3(v);
        
        /// <summary>
        /// Returns a Vector4 from component-wise application of HermiteInterpolationOrder5 (((6 * v - 15) * v + 10) * v * v * v).
        /// </summary>
        public static Vector4 HermiteInterpolationOrder5(Vector4 v) => Vector4.HermiteInterpolationOrder5(v);
        
        /// <summary>
        /// Returns a Vector4 from component-wise application of Sqr (v * v).
        /// </summary>
        public static Vector4 Sqr(Vector4 v) => Vector4.Sqr(v);
        
        /// <summary>
        /// Returns a Vector4 from component-wise application of Pow2 (v * v).
        /// </summary>
        public static Vector4 Pow2(Vector4 v) => Vector4.Pow2(v);
        
        /// <summary>
        /// Returns a Vector4 from component-wise application of Pow3 (v * v * v).
        /// </summary>
        public static Vector4 Pow3(Vector4 v) => Vector4.Pow3(v);
        
        /// <summary>
        /// Returns a Vector4 from component-wise application of Step (v &gt;= 0f ? 1f : 0f).
        /// </summary>
        public static Vector4 Step(Vector4 v) => Vector4.Step(v);
        
        /// <summary>
        /// Returns a Vector4 from component-wise application of Sqrt ((float)Math.Sqrt((double)v)).
        /// </summary>
        public static Vector4 Sqrt(Vector4 v) => Vector4.Sqrt(v);
        
        /// <summary>
        /// Returns a Vector4 from component-wise application of InverseSqrt ((float)(1.0 / Math.Sqrt((double)v))).
        /// </summary>
        public static Vector4 InverseSqrt(Vector4 v) => Vector4.InverseSqrt(v);
        
        /// <summary>
        /// Returns a iVector4 from component-wise application of Sign (Math.Sign(v)).
        /// </summary>
        public static iVector4 Sign(Vector4 v) => Vector4.Sign(v);
        
        /// <summary>
        /// Returns a Vector4 from component-wise application of Max (Math.Max(lhs, rhs)).
        /// </summary>
        public static Vector4 Max(Vector4 lhs, Vector4 rhs) => Vector4.Max(lhs, rhs);
        
        /// <summary>
        /// Returns a Vector4 from component-wise application of Min (Math.Min(lhs, rhs)).
        /// </summary>
        public static Vector4 Min(Vector4 lhs, Vector4 rhs) => Vector4.Min(lhs, rhs);
        
        /// <summary>
        /// Returns a Vector4 from component-wise application of Pow ((float)Math.Pow((double)lhs, (double)rhs)).
        /// </summary>
        public static Vector4 Pow(Vector4 lhs, Vector4 rhs) => Vector4.Pow(lhs, rhs);
        
        /// <summary>
        /// Returns a Vector4 from component-wise application of Log ((float)Math.Log((double)lhs, (double)rhs)).
        /// </summary>
        public static Vector4 Log(Vector4 lhs, Vector4 rhs) => Vector4.Log(lhs, rhs);
        
        /// <summary>
        /// Returns a Vector4 from component-wise application of Clamp (Math.Min(Math.Max(v, min), max)).
        /// </summary>
        public static Vector4 Clamp(Vector4 v, Vector4 min, Vector4 max) => Vector4.Clamp(v, min, max);
        
        /// <summary>
        /// Returns a Vector4 from component-wise application of Mix (min * (1-a) + max * a).
        /// </summary>
        public static Vector4 Mix(Vector4 min, Vector4 max, Vector4 a) => Vector4.Mix(min, max, a);
        
        /// <summary>
        /// Returns a Vector4 from component-wise application of Lerp (min * (1-a) + max * a).
        /// </summary>
        public static Vector4 Lerp(Vector4 min, Vector4 max, Vector4 a) => Vector4.Lerp(min, max, a);
        
        /// <summary>
        /// Returns a Vector4 from component-wise application of Smoothstep (((v - edge0) / (edge1 - edge0)).Clamp().HermiteInterpolationOrder3()).
        /// </summary>
        public static Vector4 Smoothstep(Vector4 edge0, Vector4 edge1, Vector4 v) => Vector4.Smoothstep(edge0, edge1, v);
        
        /// <summary>
        /// Returns a Vector4 from component-wise application of Smootherstep (((v - edge0) / (edge1 - edge0)).Clamp().HermiteInterpolationOrder5()).
        /// </summary>
        public static Vector4 Smootherstep(Vector4 edge0, Vector4 edge1, Vector4 v) => Vector4.Smootherstep(edge0, edge1, v);
        
        /// <summary>
        /// Returns a Vector4 from component-wise application of Fma (a * b + c).
        /// </summary>
        public static Vector4 Fma(Vector4 a, Vector4 b, Vector4 c) => Vector4.Fma(a, b, c);
        
        /// <summary>
        /// OuterProduct treats the first parameter c as a column Vector (Matrix with one column) and the second parameter r as a row Vector (Matrix with one row) and does a linear algebraic Matrix multiply c * r, yielding a Matrix whose number of rows is the number of components in c and whose number of columns is the number of components in r.
        /// </summary>
        public static Matrix2x4 OuterProduct(Vector4 c, Vector2 r) => Vector4.OuterProduct(c, r);
        
        /// <summary>
        /// OuterProduct treats the first parameter c as a column Vector (Matrix with one column) and the second parameter r as a row Vector (Matrix with one row) and does a linear algebraic Matrix multiply c * r, yielding a Matrix whose number of rows is the number of components in c and whose number of columns is the number of components in r.
        /// </summary>
        public static Matrix3x4 OuterProduct(Vector4 c, Vector3 r) => Vector4.OuterProduct(c, r);
        
        /// <summary>
        /// OuterProduct treats the first parameter c as a column Vector (Matrix with one column) and the second parameter r as a row Vector (Matrix with one row) and does a linear algebraic Matrix multiply c * r, yielding a Matrix whose number of rows is the number of components in c and whose number of columns is the number of components in r.
        /// </summary>
        public static Matrix4 OuterProduct(Vector4 c, Vector4 r) => Vector4.OuterProduct(c, r);
        
        /// <summary>
        /// Returns a Vector4 from component-wise application of Add (lhs + rhs).
        /// </summary>
        public static Vector4 Add(Vector4 lhs, Vector4 rhs) => Vector4.Add(lhs, rhs);
        
        /// <summary>
        /// Returns a Vector4 from component-wise application of Sub (lhs - rhs).
        /// </summary>
        public static Vector4 Sub(Vector4 lhs, Vector4 rhs) => Vector4.Sub(lhs, rhs);
        
        /// <summary>
        /// Returns a Vector4 from component-wise application of Mul (lhs * rhs).
        /// </summary>
        public static Vector4 Mul(Vector4 lhs, Vector4 rhs) => Vector4.Mul(lhs, rhs);
        
        /// <summary>
        /// Returns a Vector4 from component-wise application of Div (lhs / rhs).
        /// </summary>
        public static Vector4 Div(Vector4 lhs, Vector4 rhs) => Vector4.Div(lhs, rhs);
        
        /// <summary>
        /// Returns a Vector4 from component-wise application of Modulo (lhs % rhs).
        /// </summary>
        public static Vector4 Modulo(Vector4 lhs, Vector4 rhs) => Vector4.Modulo(lhs, rhs);
        
        /// <summary>
        /// Returns a Vector4 from component-wise application of Degrees (Radians-To-Degrees Conversion).
        /// </summary>
        public static Vector4 Degrees(Vector4 v) => Vector4.Degrees(v);
        
        /// <summary>
        /// Returns a Vector4 from component-wise application of Radians (Degrees-To-Radians Conversion).
        /// </summary>
        public static Vector4 Radians(Vector4 v) => Vector4.Radians(v);
        
        /// <summary>
        /// Returns a Vector4 from component-wise application of Acos ((float)Math.Acos((double)v)).
        /// </summary>
        public static Vector4 Acos(Vector4 v) => Vector4.Acos(v);
        
        /// <summary>
        /// Returns a Vector4 from component-wise application of Asin ((float)Math.Asin((double)v)).
        /// </summary>
        public static Vector4 Asin(Vector4 v) => Vector4.Asin(v);
        
        /// <summary>
        /// Returns a Vector4 from component-wise application of Atan ((float)Math.Atan((double)v)).
        /// </summary>
        public static Vector4 Atan(Vector4 v) => Vector4.Atan(v);
        
        /// <summary>
        /// Returns a Vector4 from component-wise application of Cos ((float)Math.Cos((double)v)).
        /// </summary>
        public static Vector4 Cos(Vector4 v) => Vector4.Cos(v);
        
        /// <summary>
        /// Returns a Vector4 from component-wise application of Cosh ((float)Math.Cosh((double)v)).
        /// </summary>
        public static Vector4 Cosh(Vector4 v) => Vector4.Cosh(v);
        
        /// <summary>
        /// Returns a Vector4 from component-wise application of Exp ((float)Math.Exp((double)v)).
        /// </summary>
        public static Vector4 Exp(Vector4 v) => Vector4.Exp(v);
        
        /// <summary>
        /// Returns a Vector4 from component-wise application of Log ((float)Math.Log((double)v)).
        /// </summary>
        public static Vector4 Log(Vector4 v) => Vector4.Log(v);
        
        /// <summary>
        /// Returns a Vector4 from component-wise application of Log2 ((float)Math.Log((double)v, 2)).
        /// </summary>
        public static Vector4 Log2(Vector4 v) => Vector4.Log2(v);
        
        /// <summary>
        /// Returns a Vector4 from component-wise application of Log10 ((float)Math.Log10((double)v)).
        /// </summary>
        public static Vector4 Log10(Vector4 v) => Vector4.Log10(v);
        
        /// <summary>
        /// Returns a Vector4 from component-wise application of Floor ((float)Math.Floor(v)).
        /// </summary>
        public static Vector4 Floor(Vector4 v) => Vector4.Floor(v);
        
        /// <summary>
        /// Returns a Vector4 from component-wise application of Ceiling ((float)Math.Ceiling(v)).
        /// </summary>
        public static Vector4 Ceiling(Vector4 v) => Vector4.Ceiling(v);
        
        /// <summary>
        /// Returns a Vector4 from component-wise application of Round ((float)Math.Round(v)).
        /// </summary>
        public static Vector4 Round(Vector4 v) => Vector4.Round(v);
        
        /// <summary>
        /// Returns a Vector4 from component-wise application of Sin ((float)Math.Sin((double)v)).
        /// </summary>
        public static Vector4 Sin(Vector4 v) => Vector4.Sin(v);
        
        /// <summary>
        /// Returns a Vector4 from component-wise application of Sinh ((float)Math.Sinh((double)v)).
        /// </summary>
        public static Vector4 Sinh(Vector4 v) => Vector4.Sinh(v);
        
        /// <summary>
        /// Returns a Vector4 from component-wise application of Tan ((float)Math.Tan((double)v)).
        /// </summary>
        public static Vector4 Tan(Vector4 v) => Vector4.Tan(v);
        
        /// <summary>
        /// Returns a Vector4 from component-wise application of Tanh ((float)Math.Tanh((double)v)).
        /// </summary>
        public static Vector4 Tanh(Vector4 v) => Vector4.Tanh(v);
        
        /// <summary>
        /// Returns a Vector4 from component-wise application of Truncate ((float)Math.Truncate((double)v)).
        /// </summary>
        public static Vector4 Truncate(Vector4 v) => Vector4.Truncate(v);
        
        /// <summary>
        /// Returns a Vector4 from component-wise application of Fract ((float)(v - Math.Floor(v))).
        /// </summary>
        public static Vector4 Fract(Vector4 v) => Vector4.Fract(v);
        
        /// <summary>
        /// Returns a Vector4 from component-wise application of Trunc ((long)(v)).
        /// </summary>
        public static Vector4 Trunc(Vector4 v) => Vector4.Trunc(v);
        
        /// <summary>
        /// Returns the minimal component of this Vector.
        /// </summary>
        public static float MinElement(Vector4 v) => v.MinElement;
        
        /// <summary>
        /// Returns the maximal component of this Vector.
        /// </summary>
        public static float MaxElement(Vector4 v) => v.MaxElement;
        
        /// <summary>
        /// Returns the euclidean length of this Vector.
        /// </summary>
        public static float Length(Vector4 v) => v.Length;
        
        /// <summary>
        /// Returns the squared euclidean length of this Vector.
        /// </summary>
        public static float LengthSqr(Vector4 v) => v.LengthSqr;
        
        /// <summary>
        /// Returns the sum of all components.
        /// </summary>
        public static float Sum(Vector4 v) => v.Sum;
        
        /// <summary>
        /// Returns the euclidean norm of this Vector.
        /// </summary>
        public static float Norm(Vector4 v) => v.Norm;
        
        /// <summary>
        /// Returns the one-norm of this Vector.
        /// </summary>
        public static float Norm1(Vector4 v) => v.Norm1;
        
        /// <summary>
        /// Returns the two-norm (euclidean length) of this Vector.
        /// </summary>
        public static float Norm2(Vector4 v) => v.Norm2;
        
        /// <summary>
        /// Returns the max-norm of this Vector.
        /// </summary>
        public static float NormMax(Vector4 v) => v.NormMax;
        
        /// <summary>
        /// Returns the p-norm of this Vector.
        /// </summary>
        public static double NormP(Vector4 v, double p) => v.NormP(p);
        
        /// <summary>
        /// Returns a copy of this Vector with length one (undefined if this has zero length).
        /// </summary>
        public static Vector4 Normalized(Vector4 v) => v.Normalized;
        
        /// <summary>
        /// Returns a copy of this Vector with length one (returns zero if length is zero).
        /// </summary>
        public static Vector4 NormalizedSafe(Vector4 v) => v.NormalizedSafe;
        
        /// <summary>
        /// Returns the inner product (dot product, scalar product) of the two Vectors.
        /// </summary>
        public static float Dot(Vector4 lhs, Vector4 rhs) => Vector4.Dot(lhs, rhs);
        
        /// <summary>
        /// Returns the euclidean distance between the two Vectors.
        /// </summary>
        public static float Distance(Vector4 lhs, Vector4 rhs) => Vector4.Distance(lhs, rhs);
        
        /// <summary>
        /// Returns the squared euclidean distance between the two Vectors.
        /// </summary>
        public static float DistanceSqr(Vector4 lhs, Vector4 rhs) => Vector4.DistanceSqr(lhs, rhs);
        
        /// <summary>
        /// Calculate the reflection direction for an incident Vector (N should be normalized in order to achieve the desired result).
        /// </summary>
        public static Vector4 Reflect(Vector4 I, Vector4 N) => Vector4.Reflect(I, N);
        
        /// <summary>
        /// Calculate the refraction direction for an incident Vector (The input parameters I and N should be normalized in order to achieve the desired result).
        /// </summary>
        public static Vector4 Refract(Vector4 I, Vector4 N, float eta) => Vector4.Refract(I, N, eta);
        
        /// <summary>
        /// Returns a Vector pointing in the same direction as another (faceforward orients a Vector to point away from a surface as defined by its normal. If dot(Nref, I) is negative faceforward returns N, otherwise it returns -N).
        /// </summary>
        public static Vector4 FaceForward(Vector4 N, Vector4 I, Vector4 Nref) => Vector4.FaceForward(N, I, Nref);
        
        /// <summary>
        /// Returns a Vector4 with independent and identically distributed uniform values between 'minValue' and 'maxValue'.
        /// </summary>
        public static Vector4 Random(System.Random random, Vector4 minValue, Vector4 maxValue) => Vector4.Random(random, minValue, maxValue);
        
        /// <summary>
        /// Returns a Vector4 with independent and identically distributed uniform values between 'minValue' and 'maxValue'.
        /// </summary>
        public static Vector4 RandomUniform(System.Random random, Vector4 minValue, Vector4 maxValue) => Vector4.RandomUniform(random, minValue, maxValue);
        
        /// <summary>
        /// Returns a Vector4 with independent and identically distributed values according to a normal/Gaussian distribution with specified mean and variance.
        /// </summary>
        public static Vector4 RandomNormal(System.Random random, Vector4 mean, Vector4 variance) => Vector4.RandomNormal(random, mean, variance);
        
        /// <summary>
        /// Returns a Vector4 with independent and identically distributed values according to a normal/Gaussian distribution with specified mean and variance.
        /// </summary>
        public static Vector4 RandomGaussian(System.Random random, Vector4 mean, Vector4 variance) => Vector4.RandomGaussian(random, mean, variance);

    }
}
