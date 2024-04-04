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
        public static swizzle_Vector2 swizzle(Vector2 v) => v.swizzle;
        
        /// <summary>
        /// Returns an array with all values
        /// </summary>
        public static float[] Values(Vector2 v) => v.Values;
        
        /// <summary>
        /// Returns an enumerator that iterates through all components.
        /// </summary>
        public static IEnumerator<float> GetEnumerator(Vector2 v) => v.GetEnumerator();
        
        /// <summary>
        /// Returns a string representation of this Vectortor using ', ' as a seperator.
        /// </summary>
        public static string ToString(Vector2 v) => v.ToString();
        
        /// <summary>
        /// Returns a string representation of this Vectortor using a provided seperator.
        /// </summary>
        public static string ToString(Vector2 v, string sep) => v.ToString(sep);
        
        /// <summary>
        /// Returns a string representation of this Vectortor using a provided seperator and a forMatrix provider for each component.
        /// </summary>
        public static string ToString(Vector2 v, string sep, IFormatProvider provider) => v.ToString(sep, provider);
        
        /// <summary>
        /// Returns a string representation of this Vectortor using a provided seperator and a forMatrix for each component.
        /// </summary>
        public static string ToString(Vector2 v, string sep, string forMatrix) => v.ToString(sep, forMatrix);
        
        /// <summary>
        /// Returns a string representation of this Vectortor using a provided seperator and a forMatrix and forMatrix provider for each component.
        /// </summary>
        public static string ToString(Vector2 v, string sep, string forMatrix, IFormatProvider provider) => v.ToString(sep, forMatrix, provider);
        
        /// <summary>
        /// Returns the number of components (2).
        /// </summary>
        public static int Count(Vector2 v) => v.Count;
        
        /// <summary>
        /// Returns true iff this equals rhs component-wise.
        /// </summary>
        public static bool Equals(Vector2 v, Vector2 rhs) => v.Equals(rhs);
        
        /// <summary>
        /// Returns true iff this equals rhs type- and component-wise.
        /// </summary>
        public static bool Equals(Vector2 v, object obj) => v.Equals(obj);
        
        /// <summary>
        /// Returns a hash code for this instance.
        /// </summary>
        public static int GetHashCode(Vector2 v) => v.GetHashCode();
        
        /// <summary>
        /// Returns true iff distance between lhs and rhs is less than or equal to epsilon
        /// </summary>
        public static bool ApproxEqual(Vector2 lhs, Vector2 rhs, float eps = 0.1f) => Vector2.ApproxEqual(lhs, rhs, eps);
        
        /// <summary>
        /// Returns a bVector2 from component-wise application of Equal (lhs == rhs).
        /// </summary>
        public static bVector2 Equal(Vector2 lhs, Vector2 rhs) => Vector2.Equal(lhs, rhs);
        
        /// <summary>
        /// Returns a bVector2 from component-wise application of NotEqual (lhs != rhs).
        /// </summary>
        public static bVector2 NotEqual(Vector2 lhs, Vector2 rhs) => Vector2.NotEqual(lhs, rhs);
        
        /// <summary>
        /// Returns a bVector2 from component-wise application of GreaterThan (lhs &gt; rhs).
        /// </summary>
        public static bVector2 GreaterThan(Vector2 lhs, Vector2 rhs) => Vector2.GreaterThan(lhs, rhs);
        
        /// <summary>
        /// Returns a bVector2 from component-wise application of GreaterThanEqual (lhs &gt;= rhs).
        /// </summary>
        public static bVector2 GreaterThanEqual(Vector2 lhs, Vector2 rhs) => Vector2.GreaterThanEqual(lhs, rhs);
        
        /// <summary>
        /// Returns a bVector2 from component-wise application of LesserThan (lhs &lt; rhs).
        /// </summary>
        public static bVector2 LesserThan(Vector2 lhs, Vector2 rhs) => Vector2.LesserThan(lhs, rhs);
        
        /// <summary>
        /// Returns a bVector2 from component-wise application of LesserThanEqual (lhs &lt;= rhs).
        /// </summary>
        public static bVector2 LesserThanEqual(Vector2 lhs, Vector2 rhs) => Vector2.LesserThanEqual(lhs, rhs);
        
        /// <summary>
        /// Returns a bVector2 from component-wise application of IsInfinity (float.IsInfinity(v)).
        /// </summary>
        public static bVector2 IsInfinity(Vector2 v) => Vector2.IsInfinity(v);
        
        /// <summary>
        /// Returns a bVector2 from component-wise application of IsFinite (!float.IsNaN(v) &amp;&amp; !float.IsInfinity(v)).
        /// </summary>
        public static bVector2 IsFinite(Vector2 v) => Vector2.IsFinite(v);
        
        /// <summary>
        /// Returns a bVector2 from component-wise application of IsNaN (float.IsNaN(v)).
        /// </summary>
        public static bVector2 IsNaN(Vector2 v) => Vector2.IsNaN(v);
        
        /// <summary>
        /// Returns a bVector2 from component-wise application of IsNegativeInfinity (float.IsNegativeInfinity(v)).
        /// </summary>
        public static bVector2 IsNegativeInfinity(Vector2 v) => Vector2.IsNegativeInfinity(v);
        
        /// <summary>
        /// Returns a bVector2 from component-wise application of IsPositiveInfinity (float.IsPositiveInfinity(v)).
        /// </summary>
        public static bVector2 IsPositiveInfinity(Vector2 v) => Vector2.IsPositiveInfinity(v);
        
        /// <summary>
        /// Returns a Vector2 from component-wise application of Abs (Math.Abs(v)).
        /// </summary>
        public static Vector2 Abs(Vector2 v) => Vector2.Abs(v);
        
        /// <summary>
        /// Returns a Vector2 from component-wise application of HermiteInterpolationOrder3 ((3 - 2 * v) * v * v).
        /// </summary>
        public static Vector2 HermiteInterpolationOrder3(Vector2 v) => Vector2.HermiteInterpolationOrder3(v);
        
        /// <summary>
        /// Returns a Vector2 from component-wise application of HermiteInterpolationOrder5 (((6 * v - 15) * v + 10) * v * v * v).
        /// </summary>
        public static Vector2 HermiteInterpolationOrder5(Vector2 v) => Vector2.HermiteInterpolationOrder5(v);
        
        /// <summary>
        /// Returns a Vector2 from component-wise application of Sqr (v * v).
        /// </summary>
        public static Vector2 Sqr(Vector2 v) => Vector2.Sqr(v);
        
        /// <summary>
        /// Returns a Vector2 from component-wise application of Pow2 (v * v).
        /// </summary>
        public static Vector2 Pow2(Vector2 v) => Vector2.Pow2(v);
        
        /// <summary>
        /// Returns a Vector2 from component-wise application of Pow3 (v * v * v).
        /// </summary>
        public static Vector2 Pow3(Vector2 v) => Vector2.Pow3(v);
        
        /// <summary>
        /// Returns a Vector2 from component-wise application of Step (v &gt;= 0f ? 1f : 0f).
        /// </summary>
        public static Vector2 Step(Vector2 v) => Vector2.Step(v);
        
        /// <summary>
        /// Returns a Vector2 from component-wise application of Sqrt ((float)Math.Sqrt((double)v)).
        /// </summary>
        public static Vector2 Sqrt(Vector2 v) => Vector2.Sqrt(v);
        
        /// <summary>
        /// Returns a Vector2 from component-wise application of InverseSqrt ((float)(1.0 / Math.Sqrt((double)v))).
        /// </summary>
        public static Vector2 InverseSqrt(Vector2 v) => Vector2.InverseSqrt(v);
        
        /// <summary>
        /// Returns a iVector2 from component-wise application of Sign (Math.Sign(v)).
        /// </summary>
        public static iVector2 Sign(Vector2 v) => Vector2.Sign(v);
        
        /// <summary>
        /// Returns a Vector2 from component-wise application of Max (Math.Max(lhs, rhs)).
        /// </summary>
        public static Vector2 Max(Vector2 lhs, Vector2 rhs) => Vector2.Max(lhs, rhs);
        
        /// <summary>
        /// Returns a Vector2 from component-wise application of Min (Math.Min(lhs, rhs)).
        /// </summary>
        public static Vector2 Min(Vector2 lhs, Vector2 rhs) => Vector2.Min(lhs, rhs);
        
        /// <summary>
        /// Returns a Vector2 from component-wise application of Pow ((float)Math.Pow((double)lhs, (double)rhs)).
        /// </summary>
        public static Vector2 Pow(Vector2 lhs, Vector2 rhs) => Vector2.Pow(lhs, rhs);
        
        /// <summary>
        /// Returns a Vector2 from component-wise application of Log ((float)Math.Log((double)lhs, (double)rhs)).
        /// </summary>
        public static Vector2 Log(Vector2 lhs, Vector2 rhs) => Vector2.Log(lhs, rhs);
        
        /// <summary>
        /// Returns a Vector2 from component-wise application of Clamp (Math.Min(Math.Max(v, min), max)).
        /// </summary>
        public static Vector2 Clamp(Vector2 v, Vector2 min, Vector2 max) => Vector2.Clamp(v, min, max);
        
        /// <summary>
        /// Returns a Vector2 from component-wise application of Mix (min * (1-a) + max * a).
        /// </summary>
        public static Vector2 Mix(Vector2 min, Vector2 max, Vector2 a) => Vector2.Mix(min, max, a);
        
        /// <summary>
        /// Returns a Vector2 from component-wise application of Lerp (min * (1-a) + max * a).
        /// </summary>
        public static Vector2 Lerp(Vector2 min, Vector2 max, Vector2 a) => Vector2.Lerp(min, max, a);
        
        /// <summary>
        /// Returns a Vector2 from component-wise application of Smoothstep (((v - edge0) / (edge1 - edge0)).Clamp().HermiteInterpolationOrder3()).
        /// </summary>
        public static Vector2 Smoothstep(Vector2 edge0, Vector2 edge1, Vector2 v) => Vector2.Smoothstep(edge0, edge1, v);
        
        /// <summary>
        /// Returns a Vector2 from component-wise application of Smootherstep (((v - edge0) / (edge1 - edge0)).Clamp().HermiteInterpolationOrder5()).
        /// </summary>
        public static Vector2 Smootherstep(Vector2 edge0, Vector2 edge1, Vector2 v) => Vector2.Smootherstep(edge0, edge1, v);
        
        /// <summary>
        /// Returns a Vector2 from component-wise application of Fma (a * b + c).
        /// </summary>
        public static Vector2 Fma(Vector2 a, Vector2 b, Vector2 c) => Vector2.Fma(a, b, c);
        
        /// <summary>
        /// OuterProduct treats the first parameter c as a column Vectortor (Matrix with one column) and the second parameter r as a row Vectortor (Matrix with one row) and does a linear algebraic Matrix multiply c * r, yielding a Matrix whose number of rows is the number of components in c and whose number of columns is the number of components in r.
        /// </summary>
        public static Matrix2 OuterProduct(Vector2 c, Vector2 r) => Vector2.OuterProduct(c, r);
        
        /// <summary>
        /// OuterProduct treats the first parameter c as a column Vectortor (Matrix with one column) and the second parameter r as a row Vectortor (Matrix with one row) and does a linear algebraic Matrix multiply c * r, yielding a Matrix whose number of rows is the number of components in c and whose number of columns is the number of components in r.
        /// </summary>
        public static Matrix3x2 OuterProduct(Vector2 c, Vector3 r) => Vector2.OuterProduct(c, r);
        
        /// <summary>
        /// OuterProduct treats the first parameter c as a column Vectortor (Matrix with one column) and the second parameter r as a row Vectortor (Matrix with one row) and does a linear algebraic Matrix multiply c * r, yielding a Matrix whose number of rows is the number of components in c and whose number of columns is the number of components in r.
        /// </summary>
        public static Matrix4x2 OuterProduct(Vector2 c, Vector4 r) => Vector2.OuterProduct(c, r);
        
        /// <summary>
        /// Returns a Vector2 from component-wise application of Add (lhs + rhs).
        /// </summary>
        public static Vector2 Add(Vector2 lhs, Vector2 rhs) => Vector2.Add(lhs, rhs);
        
        /// <summary>
        /// Returns a Vector2 from component-wise application of Sub (lhs - rhs).
        /// </summary>
        public static Vector2 Sub(Vector2 lhs, Vector2 rhs) => Vector2.Sub(lhs, rhs);
        
        /// <summary>
        /// Returns a Vector2 from component-wise application of Mul (lhs * rhs).
        /// </summary>
        public static Vector2 Mul(Vector2 lhs, Vector2 rhs) => Vector2.Mul(lhs, rhs);
        
        /// <summary>
        /// Returns a Vector2 from component-wise application of Div (lhs / rhs).
        /// </summary>
        public static Vector2 Div(Vector2 lhs, Vector2 rhs) => Vector2.Div(lhs, rhs);
        
        /// <summary>
        /// Returns a Vector2 from component-wise application of Modulo (lhs % rhs).
        /// </summary>
        public static Vector2 Modulo(Vector2 lhs, Vector2 rhs) => Vector2.Modulo(lhs, rhs);
        
        /// <summary>
        /// Returns a Vector2 from component-wise application of Degrees (Radians-To-Degrees Conversion).
        /// </summary>
        public static Vector2 Degrees(Vector2 v) => Vector2.Degrees(v);
        
        /// <summary>
        /// Returns a Vector2 from component-wise application of Radians (Degrees-To-Radians Conversion).
        /// </summary>
        public static Vector2 Radians(Vector2 v) => Vector2.Radians(v);
        
        /// <summary>
        /// Returns a Vector2 from component-wise application of Acos ((float)Math.Acos((double)v)).
        /// </summary>
        public static Vector2 Acos(Vector2 v) => Vector2.Acos(v);
        
        /// <summary>
        /// Returns a Vector2 from component-wise application of Asin ((float)Math.Asin((double)v)).
        /// </summary>
        public static Vector2 Asin(Vector2 v) => Vector2.Asin(v);
        
        /// <summary>
        /// Returns a Vector2 from component-wise application of Atan ((float)Math.Atan((double)v)).
        /// </summary>
        public static Vector2 Atan(Vector2 v) => Vector2.Atan(v);
        
        /// <summary>
        /// Returns a Vector2 from component-wise application of Cos ((float)Math.Cos((double)v)).
        /// </summary>
        public static Vector2 Cos(Vector2 v) => Vector2.Cos(v);
        
        /// <summary>
        /// Returns a Vector2 from component-wise application of Cosh ((float)Math.Cosh((double)v)).
        /// </summary>
        public static Vector2 Cosh(Vector2 v) => Vector2.Cosh(v);
        
        /// <summary>
        /// Returns a Vector2 from component-wise application of Exp ((float)Math.Exp((double)v)).
        /// </summary>
        public static Vector2 Exp(Vector2 v) => Vector2.Exp(v);
        
        /// <summary>
        /// Returns a Vector2 from component-wise application of Log ((float)Math.Log((double)v)).
        /// </summary>
        public static Vector2 Log(Vector2 v) => Vector2.Log(v);
        
        /// <summary>
        /// Returns a Vector2 from component-wise application of Log2 ((float)Math.Log((double)v, 2)).
        /// </summary>
        public static Vector2 Log2(Vector2 v) => Vector2.Log2(v);
        
        /// <summary>
        /// Returns a Vector2 from component-wise application of Log10 ((float)Math.Log10((double)v)).
        /// </summary>
        public static Vector2 Log10(Vector2 v) => Vector2.Log10(v);
        
        /// <summary>
        /// Returns a Vector2 from component-wise application of Floor ((float)Math.Floor(v)).
        /// </summary>
        public static Vector2 Floor(Vector2 v) => Vector2.Floor(v);
        
        /// <summary>
        /// Returns a Vector2 from component-wise application of Ceiling ((float)Math.Ceiling(v)).
        /// </summary>
        public static Vector2 Ceiling(Vector2 v) => Vector2.Ceiling(v);
        
        /// <summary>
        /// Returns a Vector2 from component-wise application of Round ((float)Math.Round(v)).
        /// </summary>
        public static Vector2 Round(Vector2 v) => Vector2.Round(v);
        
        /// <summary>
        /// Returns a Vector2 from component-wise application of Sin ((float)Math.Sin((double)v)).
        /// </summary>
        public static Vector2 Sin(Vector2 v) => Vector2.Sin(v);
        
        /// <summary>
        /// Returns a Vector2 from component-wise application of Sinh ((float)Math.Sinh((double)v)).
        /// </summary>
        public static Vector2 Sinh(Vector2 v) => Vector2.Sinh(v);
        
        /// <summary>
        /// Returns a Vector2 from component-wise application of Tan ((float)Math.Tan((double)v)).
        /// </summary>
        public static Vector2 Tan(Vector2 v) => Vector2.Tan(v);
        
        /// <summary>
        /// Returns a Vector2 from component-wise application of Tanh ((float)Math.Tanh((double)v)).
        /// </summary>
        public static Vector2 Tanh(Vector2 v) => Vector2.Tanh(v);
        
        /// <summary>
        /// Returns a Vector2 from component-wise application of Truncate ((float)Math.Truncate((double)v)).
        /// </summary>
        public static Vector2 Truncate(Vector2 v) => Vector2.Truncate(v);
        
        /// <summary>
        /// Returns a Vector2 from component-wise application of Fract ((float)(v - Math.Floor(v))).
        /// </summary>
        public static Vector2 Fract(Vector2 v) => Vector2.Fract(v);
        
        /// <summary>
        /// Returns a Vector2 from component-wise application of Trunc ((long)(v)).
        /// </summary>
        public static Vector2 Trunc(Vector2 v) => Vector2.Trunc(v);
        
        /// <summary>
        /// Returns the minimal component of this Vectortor.
        /// </summary>
        public static float MinElement(Vector2 v) => v.MinElement;
        
        /// <summary>
        /// Returns the maximal component of this Vectortor.
        /// </summary>
        public static float MaxElement(Vector2 v) => v.MaxElement;
        
        /// <summary>
        /// Returns the euclidean length of this Vectortor.
        /// </summary>
        public static float Length(Vector2 v) => v.Length;
        
        /// <summary>
        /// Returns the squared euclidean length of this Vectortor.
        /// </summary>
        public static float LengthSqr(Vector2 v) => v.LengthSqr;
        
        /// <summary>
        /// Returns the sum of all components.
        /// </summary>
        public static float Sum(Vector2 v) => v.Sum;
        
        /// <summary>
        /// Returns the euclidean norm of this Vectortor.
        /// </summary>
        public static float Norm(Vector2 v) => v.Norm;
        
        /// <summary>
        /// Returns the one-norm of this Vectortor.
        /// </summary>
        public static float Norm1(Vector2 v) => v.Norm1;
        
        /// <summary>
        /// Returns the two-norm (euclidean length) of this Vectortor.
        /// </summary>
        public static float Norm2(Vector2 v) => v.Norm2;
        
        /// <summary>
        /// Returns the max-norm of this Vectortor.
        /// </summary>
        public static float NormMax(Vector2 v) => v.NormMax;
        
        /// <summary>
        /// Returns the p-norm of this Vectortor.
        /// </summary>
        public static double NormP(Vector2 v, double p) => v.NormP(p);
        
        /// <summary>
        /// Returns a copy of this Vectortor with length one (undefined if this has zero length).
        /// </summary>
        public static Vector2 Normalized(Vector2 v) => v.Normalized;
        
        /// <summary>
        /// Returns a copy of this Vectortor with length one (returns zero if length is zero).
        /// </summary>
        public static Vector2 NormalizedSafe(Vector2 v) => v.NormalizedSafe;
        
        /// <summary>
        /// Returns the Vectortor angle (atan2(y, x)) in radians.
        /// </summary>
        public static double Angle(Vector2 v) => v.Angle;
        
        /// <summary>
        /// Returns a 2D Vectortor that was rotated by a given angle in radians (CAUTION: result is casted and may be truncated).
        /// </summary>
        public static Vector2 Rotated(Vector2 v, double angleInRad) => v.Rotated(angleInRad);
        
        /// <summary>
        /// Returns the inner product (dot product, scalar product) of the two Vectortors.
        /// </summary>
        public static float Dot(Vector2 lhs, Vector2 rhs) => Vector2.Dot(lhs, rhs);
        
        /// <summary>
        /// Returns the euclidean distance between the two Vectortors.
        /// </summary>
        public static float Distance(Vector2 lhs, Vector2 rhs) => Vector2.Distance(lhs, rhs);
        
        /// <summary>
        /// Returns the squared euclidean distance between the two Vectortors.
        /// </summary>
        public static float DistanceSqr(Vector2 lhs, Vector2 rhs) => Vector2.DistanceSqr(lhs, rhs);
        
        /// <summary>
        /// Calculate the reflection direction for an incident Vectortor (N should be normalized in order to achieve the desired result).
        /// </summary>
        public static Vector2 Reflect(Vector2 I, Vector2 N) => Vector2.Reflect(I, N);
        
        /// <summary>
        /// Calculate the refraction direction for an incident Vectortor (The input parameters I and N should be normalized in order to achieve the desired result).
        /// </summary>
        public static Vector2 Refract(Vector2 I, Vector2 N, float eta) => Vector2.Refract(I, N, eta);
        
        /// <summary>
        /// Returns a Vectortor pointing in the same direction as another (faceforward orients a Vectortor to point away from a surface as defined by its normal. If dot(Nref, I) is negative faceforward returns N, otherwise it returns -N).
        /// </summary>
        public static Vector2 FaceForward(Vector2 N, Vector2 I, Vector2 Nref) => Vector2.FaceForward(N, I, Nref);
        
        /// <summary>
        /// Returns the length of the outer product (cross product, Vectortor product) of the two Vectortors.
        /// </summary>
        public static float Cross(Vector2 l, Vector2 r) => Vector2.Cross(l, r);
        
        /// <summary>
        /// Returns a Vector2 with independent and identically distributed uniform values between 'minValue' and 'maxValue'.
        /// </summary>
        public static Vector2 Random(System.Random random, Vector2 minValue, Vector2 maxValue) => Vector2.Random(random, minValue, maxValue);
        
        /// <summary>
        /// Returns a Vector2 with independent and identically distributed uniform values between 'minValue' and 'maxValue'.
        /// </summary>
        public static Vector2 RandomUniform(System.Random random, Vector2 minValue, Vector2 maxValue) => Vector2.RandomUniform(random, minValue, maxValue);
        
        /// <summary>
        /// Returns a Vector2 with independent and identically distributed values according to a normal/Gaussian distribution with specified mean and variance.
        /// </summary>
        public static Vector2 RandomNormal(System.Random random, Vector2 mean, Vector2 variance) => Vector2.RandomNormal(random, mean, variance);
        
        /// <summary>
        /// Returns a Vector2 with independent and identically distributed values according to a normal/Gaussian distribution with specified mean and variance.
        /// </summary>
        public static Vector2 RandomGaussian(System.Random random, Vector2 mean, Vector2 variance) => Vector2.RandomGaussian(random, mean, variance);

    }
}
