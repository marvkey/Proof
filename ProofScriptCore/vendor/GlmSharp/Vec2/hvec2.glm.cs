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
        public static swizzle_hVector2 swizzle(hVector2 v) => v.swizzle;
        
        /// <summary>
        /// Returns an array with all values
        /// </summary>
        public static Half[] Values(hVector2 v) => v.Values;
        
        /// <summary>
        /// Returns an enumerator that iterates through all components.
        /// </summary>
        public static IEnumerator<Half> GetEnumerator(hVector2 v) => v.GetEnumerator();
        
        /// <summary>
        /// Returns a string representation of this Vectortor using ', ' as a seperator.
        /// </summary>
        public static string ToString(hVector2 v) => v.ToString();
        
        /// <summary>
        /// Returns a string representation of this Vectortor using a provided seperator.
        /// </summary>
        public static string ToString(hVector2 v, string sep) => v.ToString(sep);
        
        /// <summary>
        /// Returns a string representation of this Vectortor using a provided seperator and a forMatrix provider for each component.
        /// </summary>
        public static string ToString(hVector2 v, string sep, IFormatProvider provider) => v.ToString(sep, provider);
        
        /// <summary>
        /// Returns a string representation of this Vectortor using a provided seperator and a forMatrix for each component.
        /// </summary>
        public static string ToString(hVector2 v, string sep, string forMatrix) => v.ToString(sep, forMatrix);
        
        /// <summary>
        /// Returns a string representation of this Vectortor using a provided seperator and a forMatrix and forMatrix provider for each component.
        /// </summary>
        public static string ToString(hVector2 v, string sep, string forMatrix, IFormatProvider provider) => v.ToString(sep, forMatrix, provider);
        
        /// <summary>
        /// Returns the number of components (2).
        /// </summary>
        public static int Count(hVector2 v) => v.Count;
        
        /// <summary>
        /// Returns true iff this equals rhs component-wise.
        /// </summary>
        public static bool Equals(hVector2 v, hVector2 rhs) => v.Equals(rhs);
        
        /// <summary>
        /// Returns true iff this equals rhs type- and component-wise.
        /// </summary>
        public static bool Equals(hVector2 v, object obj) => v.Equals(obj);
        
        /// <summary>
        /// Returns a hash code for this instance.
        /// </summary>
        public static int GetHashCode(hVector2 v) => v.GetHashCode();
        
        /// <summary>
        /// Returns true iff distance between lhs and rhs is less than or equal to epsilon
        /// </summary>
        public static bool ApproxEqual(hVector2 lhs, hVector2 rhs, float eps = 0.1f) => hVector2.ApproxEqual(lhs, rhs, eps);
        
        /// <summary>
        /// Returns a bVector2 from component-wise application of Equal (lhs == rhs).
        /// </summary>
        public static bVector2 Equal(hVector2 lhs, hVector2 rhs) => hVector2.Equal(lhs, rhs);
        
        /// <summary>
        /// Returns a bVector2 from component-wise application of NotEqual (lhs != rhs).
        /// </summary>
        public static bVector2 NotEqual(hVector2 lhs, hVector2 rhs) => hVector2.NotEqual(lhs, rhs);
        
        /// <summary>
        /// Returns a bVector2 from component-wise application of GreaterThan (lhs &gt; rhs).
        /// </summary>
        public static bVector2 GreaterThan(hVector2 lhs, hVector2 rhs) => hVector2.GreaterThan(lhs, rhs);
        
        /// <summary>
        /// Returns a bVector2 from component-wise application of GreaterThanEqual (lhs &gt;= rhs).
        /// </summary>
        public static bVector2 GreaterThanEqual(hVector2 lhs, hVector2 rhs) => hVector2.GreaterThanEqual(lhs, rhs);
        
        /// <summary>
        /// Returns a bVector2 from component-wise application of LesserThan (lhs &lt; rhs).
        /// </summary>
        public static bVector2 LesserThan(hVector2 lhs, hVector2 rhs) => hVector2.LesserThan(lhs, rhs);
        
        /// <summary>
        /// Returns a bVector2 from component-wise application of LesserThanEqual (lhs &lt;= rhs).
        /// </summary>
        public static bVector2 LesserThanEqual(hVector2 lhs, hVector2 rhs) => hVector2.LesserThanEqual(lhs, rhs);
        
        /// <summary>
        /// Returns a bVector2 from component-wise application of IsInfinity (Half.IsInfinity(v)).
        /// </summary>
        public static bVector2 IsInfinity(hVector2 v) => hVector2.IsInfinity(v);
        
        /// <summary>
        /// Returns a bVector2 from component-wise application of IsFinite (!Half.IsNaN(v) &amp;&amp; !Half.IsInfinity(v)).
        /// </summary>
        public static bVector2 IsFinite(hVector2 v) => hVector2.IsFinite(v);
        
        /// <summary>
        /// Returns a bVector2 from component-wise application of IsNaN (Half.IsNaN(v)).
        /// </summary>
        public static bVector2 IsNaN(hVector2 v) => hVector2.IsNaN(v);
        
        /// <summary>
        /// Returns a bVector2 from component-wise application of IsNegativeInfinity (Half.IsNegativeInfinity(v)).
        /// </summary>
        public static bVector2 IsNegativeInfinity(hVector2 v) => hVector2.IsNegativeInfinity(v);
        
        /// <summary>
        /// Returns a bVector2 from component-wise application of IsPositiveInfinity (Half.IsPositiveInfinity(v)).
        /// </summary>
        public static bVector2 IsPositiveInfinity(hVector2 v) => hVector2.IsPositiveInfinity(v);
        
        /// <summary>
        /// Returns a hVector2 from component-wise application of Abs (Half.Abs(v)).
        /// </summary>
        public static hVector2 Abs(hVector2 v) => hVector2.Abs(v);
        
        /// <summary>
        /// Returns a hVector2 from component-wise application of HermiteInterpolationOrder3 ((3 - 2 * v) * v * v).
        /// </summary>
        public static hVector2 HermiteInterpolationOrder3(hVector2 v) => hVector2.HermiteInterpolationOrder3(v);
        
        /// <summary>
        /// Returns a hVector2 from component-wise application of HermiteInterpolationOrder5 (((6 * v - 15) * v + 10) * v * v * v).
        /// </summary>
        public static hVector2 HermiteInterpolationOrder5(hVector2 v) => hVector2.HermiteInterpolationOrder5(v);
        
        /// <summary>
        /// Returns a hVector2 from component-wise application of Sqr (v * v).
        /// </summary>
        public static hVector2 Sqr(hVector2 v) => hVector2.Sqr(v);
        
        /// <summary>
        /// Returns a hVector2 from component-wise application of Pow2 (v * v).
        /// </summary>
        public static hVector2 Pow2(hVector2 v) => hVector2.Pow2(v);
        
        /// <summary>
        /// Returns a hVector2 from component-wise application of Pow3 (v * v * v).
        /// </summary>
        public static hVector2 Pow3(hVector2 v) => hVector2.Pow3(v);
        
        /// <summary>
        /// Returns a hVector2 from component-wise application of Step (v &gt;= Half.Zero ? Half.One : Half.Zero).
        /// </summary>
        public static hVector2 Step(hVector2 v) => hVector2.Step(v);
        
        /// <summary>
        /// Returns a hVector2 from component-wise application of Sqrt ((Half)Math.Sqrt((double)v)).
        /// </summary>
        public static hVector2 Sqrt(hVector2 v) => hVector2.Sqrt(v);
        
        /// <summary>
        /// Returns a hVector2 from component-wise application of InverseSqrt ((Half)(1.0 / Math.Sqrt((double)v))).
        /// </summary>
        public static hVector2 InverseSqrt(hVector2 v) => hVector2.InverseSqrt(v);
        
        /// <summary>
        /// Returns a iVector2 from component-wise application of Sign (Math.Sign(v)).
        /// </summary>
        public static iVector2 Sign(hVector2 v) => hVector2.Sign(v);
        
        /// <summary>
        /// Returns a hVector2 from component-wise application of Max (Half.Max(lhs, rhs)).
        /// </summary>
        public static hVector2 Max(hVector2 lhs, hVector2 rhs) => hVector2.Max(lhs, rhs);
        
        /// <summary>
        /// Returns a hVector2 from component-wise application of Min (Half.Min(lhs, rhs)).
        /// </summary>
        public static hVector2 Min(hVector2 lhs, hVector2 rhs) => hVector2.Min(lhs, rhs);
        
        /// <summary>
        /// Returns a hVector2 from component-wise application of Pow ((Half)Math.Pow((double)lhs, (double)rhs)).
        /// </summary>
        public static hVector2 Pow(hVector2 lhs, hVector2 rhs) => hVector2.Pow(lhs, rhs);
        
        /// <summary>
        /// Returns a hVector2 from component-wise application of Log ((Half)Math.Log((double)lhs, (double)rhs)).
        /// </summary>
        public static hVector2 Log(hVector2 lhs, hVector2 rhs) => hVector2.Log(lhs, rhs);
        
        /// <summary>
        /// Returns a hVector2 from component-wise application of Clamp (Half.Min(Half.Max(v, min), max)).
        /// </summary>
        public static hVector2 Clamp(hVector2 v, hVector2 min, hVector2 max) => hVector2.Clamp(v, min, max);
        
        /// <summary>
        /// Returns a hVector2 from component-wise application of Mix (min * (1-a) + max * a).
        /// </summary>
        public static hVector2 Mix(hVector2 min, hVector2 max, hVector2 a) => hVector2.Mix(min, max, a);
        
        /// <summary>
        /// Returns a hVector2 from component-wise application of Lerp (min * (1-a) + max * a).
        /// </summary>
        public static hVector2 Lerp(hVector2 min, hVector2 max, hVector2 a) => hVector2.Lerp(min, max, a);
        
        /// <summary>
        /// Returns a hVector2 from component-wise application of Smoothstep (((v - edge0) / (edge1 - edge0)).Clamp().HermiteInterpolationOrder3()).
        /// </summary>
        public static hVector2 Smoothstep(hVector2 edge0, hVector2 edge1, hVector2 v) => hVector2.Smoothstep(edge0, edge1, v);
        
        /// <summary>
        /// Returns a hVector2 from component-wise application of Smootherstep (((v - edge0) / (edge1 - edge0)).Clamp().HermiteInterpolationOrder5()).
        /// </summary>
        public static hVector2 Smootherstep(hVector2 edge0, hVector2 edge1, hVector2 v) => hVector2.Smootherstep(edge0, edge1, v);
        
        /// <summary>
        /// Returns a hVector2 from component-wise application of Fma (a * b + c).
        /// </summary>
        public static hVector2 Fma(hVector2 a, hVector2 b, hVector2 c) => hVector2.Fma(a, b, c);
        
        /// <summary>
        /// OuterProduct treats the first parameter c as a column Vectortor (Matrix with one column) and the second parameter r as a row Vectortor (Matrix with one row) and does a linear algebraic Matrix multiply c * r, yielding a Matrix whose number of rows is the number of components in c and whose number of columns is the number of components in r.
        /// </summary>
        public static hMatrix2 OuterProduct(hVector2 c, hVector2 r) => hVector2.OuterProduct(c, r);
        
        /// <summary>
        /// OuterProduct treats the first parameter c as a column Vectortor (Matrix with one column) and the second parameter r as a row Vectortor (Matrix with one row) and does a linear algebraic Matrix multiply c * r, yielding a Matrix whose number of rows is the number of components in c and whose number of columns is the number of components in r.
        /// </summary>
        public static hMatrix3x2 OuterProduct(hVector2 c, hVector3 r) => hVector2.OuterProduct(c, r);
        
        /// <summary>
        /// OuterProduct treats the first parameter c as a column Vectortor (Matrix with one column) and the second parameter r as a row Vectortor (Matrix with one row) and does a linear algebraic Matrix multiply c * r, yielding a Matrix whose number of rows is the number of components in c and whose number of columns is the number of components in r.
        /// </summary>
        public static hMatrix4x2 OuterProduct(hVector2 c, hVector4 r) => hVector2.OuterProduct(c, r);
        
        /// <summary>
        /// Returns a hVector2 from component-wise application of Add (lhs + rhs).
        /// </summary>
        public static hVector2 Add(hVector2 lhs, hVector2 rhs) => hVector2.Add(lhs, rhs);
        
        /// <summary>
        /// Returns a hVector2 from component-wise application of Sub (lhs - rhs).
        /// </summary>
        public static hVector2 Sub(hVector2 lhs, hVector2 rhs) => hVector2.Sub(lhs, rhs);
        
        /// <summary>
        /// Returns a hVector2 from component-wise application of Mul (lhs * rhs).
        /// </summary>
        public static hVector2 Mul(hVector2 lhs, hVector2 rhs) => hVector2.Mul(lhs, rhs);
        
        /// <summary>
        /// Returns a hVector2 from component-wise application of Div (lhs / rhs).
        /// </summary>
        public static hVector2 Div(hVector2 lhs, hVector2 rhs) => hVector2.Div(lhs, rhs);
        
        /// <summary>
        /// Returns a hVector2 from component-wise application of Modulo (lhs % rhs).
        /// </summary>
        public static hVector2 Modulo(hVector2 lhs, hVector2 rhs) => hVector2.Modulo(lhs, rhs);
        
        /// <summary>
        /// Returns a hVector2 from component-wise application of Degrees (Radians-To-Degrees Conversion).
        /// </summary>
        public static hVector2 Degrees(hVector2 v) => hVector2.Degrees(v);
        
        /// <summary>
        /// Returns a hVector2 from component-wise application of Radians (Degrees-To-Radians Conversion).
        /// </summary>
        public static hVector2 Radians(hVector2 v) => hVector2.Radians(v);
        
        /// <summary>
        /// Returns a hVector2 from component-wise application of Acos ((Half)Math.Acos((double)v)).
        /// </summary>
        public static hVector2 Acos(hVector2 v) => hVector2.Acos(v);
        
        /// <summary>
        /// Returns a hVector2 from component-wise application of Asin ((Half)Math.Asin((double)v)).
        /// </summary>
        public static hVector2 Asin(hVector2 v) => hVector2.Asin(v);
        
        /// <summary>
        /// Returns a hVector2 from component-wise application of Atan ((Half)Math.Atan((double)v)).
        /// </summary>
        public static hVector2 Atan(hVector2 v) => hVector2.Atan(v);
        
        /// <summary>
        /// Returns a hVector2 from component-wise application of Cos ((Half)Math.Cos((double)v)).
        /// </summary>
        public static hVector2 Cos(hVector2 v) => hVector2.Cos(v);
        
        /// <summary>
        /// Returns a hVector2 from component-wise application of Cosh ((Half)Math.Cosh((double)v)).
        /// </summary>
        public static hVector2 Cosh(hVector2 v) => hVector2.Cosh(v);
        
        /// <summary>
        /// Returns a hVector2 from component-wise application of Exp ((Half)Math.Exp((double)v)).
        /// </summary>
        public static hVector2 Exp(hVector2 v) => hVector2.Exp(v);
        
        /// <summary>
        /// Returns a hVector2 from component-wise application of Log ((Half)Math.Log((double)v)).
        /// </summary>
        public static hVector2 Log(hVector2 v) => hVector2.Log(v);
        
        /// <summary>
        /// Returns a hVector2 from component-wise application of Log2 ((Half)Math.Log((double)v, 2)).
        /// </summary>
        public static hVector2 Log2(hVector2 v) => hVector2.Log2(v);
        
        /// <summary>
        /// Returns a hVector2 from component-wise application of Log10 ((Half)Math.Log10((double)v)).
        /// </summary>
        public static hVector2 Log10(hVector2 v) => hVector2.Log10(v);
        
        /// <summary>
        /// Returns a hVector2 from component-wise application of Floor ((Half)Math.Floor(v)).
        /// </summary>
        public static hVector2 Floor(hVector2 v) => hVector2.Floor(v);
        
        /// <summary>
        /// Returns a hVector2 from component-wise application of Ceiling ((Half)Math.Ceiling(v)).
        /// </summary>
        public static hVector2 Ceiling(hVector2 v) => hVector2.Ceiling(v);
        
        /// <summary>
        /// Returns a hVector2 from component-wise application of Round ((Half)Math.Round(v)).
        /// </summary>
        public static hVector2 Round(hVector2 v) => hVector2.Round(v);
        
        /// <summary>
        /// Returns a hVector2 from component-wise application of Sin ((Half)Math.Sin((double)v)).
        /// </summary>
        public static hVector2 Sin(hVector2 v) => hVector2.Sin(v);
        
        /// <summary>
        /// Returns a hVector2 from component-wise application of Sinh ((Half)Math.Sinh((double)v)).
        /// </summary>
        public static hVector2 Sinh(hVector2 v) => hVector2.Sinh(v);
        
        /// <summary>
        /// Returns a hVector2 from component-wise application of Tan ((Half)Math.Tan((double)v)).
        /// </summary>
        public static hVector2 Tan(hVector2 v) => hVector2.Tan(v);
        
        /// <summary>
        /// Returns a hVector2 from component-wise application of Tanh ((Half)Math.Tanh((double)v)).
        /// </summary>
        public static hVector2 Tanh(hVector2 v) => hVector2.Tanh(v);
        
        /// <summary>
        /// Returns a hVector2 from component-wise application of Truncate ((Half)Math.Truncate((double)v)).
        /// </summary>
        public static hVector2 Truncate(hVector2 v) => hVector2.Truncate(v);
        
        /// <summary>
        /// Returns a hVector2 from component-wise application of Fract ((Half)(v - Math.Floor(v))).
        /// </summary>
        public static hVector2 Fract(hVector2 v) => hVector2.Fract(v);
        
        /// <summary>
        /// Returns a hVector2 from component-wise application of Trunc ((long)(v)).
        /// </summary>
        public static hVector2 Trunc(hVector2 v) => hVector2.Trunc(v);
        
        /// <summary>
        /// Returns the minimal component of this Vectortor.
        /// </summary>
        public static Half MinElement(hVector2 v) => v.MinElement;
        
        /// <summary>
        /// Returns the maximal component of this Vectortor.
        /// </summary>
        public static Half MaxElement(hVector2 v) => v.MaxElement;
        
        /// <summary>
        /// Returns the euclidean length of this Vectortor.
        /// </summary>
        public static float Length(hVector2 v) => v.Length;
        
        /// <summary>
        /// Returns the squared euclidean length of this Vectortor.
        /// </summary>
        public static float LengthSqr(hVector2 v) => v.LengthSqr;
        
        /// <summary>
        /// Returns the sum of all components.
        /// </summary>
        public static Half Sum(hVector2 v) => v.Sum;
        
        /// <summary>
        /// Returns the euclidean norm of this Vectortor.
        /// </summary>
        public static float Norm(hVector2 v) => v.Norm;
        
        /// <summary>
        /// Returns the one-norm of this Vectortor.
        /// </summary>
        public static float Norm1(hVector2 v) => v.Norm1;
        
        /// <summary>
        /// Returns the two-norm (euclidean length) of this Vectortor.
        /// </summary>
        public static float Norm2(hVector2 v) => v.Norm2;
        
        /// <summary>
        /// Returns the max-norm of this Vectortor.
        /// </summary>
        public static float NormMax(hVector2 v) => v.NormMax;
        
        /// <summary>
        /// Returns the p-norm of this Vectortor.
        /// </summary>
        public static double NormP(hVector2 v, double p) => v.NormP(p);
        
        /// <summary>
        /// Returns a copy of this Vectortor with length one (undefined if this has zero length).
        /// </summary>
        public static hVector2 Normalized(hVector2 v) => v.Normalized;
        
        /// <summary>
        /// Returns a copy of this Vectortor with length one (returns zero if length is zero).
        /// </summary>
        public static hVector2 NormalizedSafe(hVector2 v) => v.NormalizedSafe;
        
        /// <summary>
        /// Returns the Vectortor angle (atan2(y, x)) in radians.
        /// </summary>
        public static double Angle(hVector2 v) => v.Angle;
        
        /// <summary>
        /// Returns a 2D Vectortor that was rotated by a given angle in radians (CAUTION: result is casted and may be truncated).
        /// </summary>
        public static hVector2 Rotated(hVector2 v, double angleInRad) => v.Rotated(angleInRad);
        
        /// <summary>
        /// Returns the inner product (dot product, scalar product) of the two Vectortors.
        /// </summary>
        public static Half Dot(hVector2 lhs, hVector2 rhs) => hVector2.Dot(lhs, rhs);
        
        /// <summary>
        /// Returns the euclidean distance between the two Vectortors.
        /// </summary>
        public static float Distance(hVector2 lhs, hVector2 rhs) => hVector2.Distance(lhs, rhs);
        
        /// <summary>
        /// Returns the squared euclidean distance between the two Vectortors.
        /// </summary>
        public static float DistanceSqr(hVector2 lhs, hVector2 rhs) => hVector2.DistanceSqr(lhs, rhs);
        
        /// <summary>
        /// Calculate the reflection direction for an incident Vectortor (N should be normalized in order to achieve the desired result).
        /// </summary>
        public static hVector2 Reflect(hVector2 I, hVector2 N) => hVector2.Reflect(I, N);
        
        /// <summary>
        /// Calculate the refraction direction for an incident Vectortor (The input parameters I and N should be normalized in order to achieve the desired result).
        /// </summary>
        public static hVector2 Refract(hVector2 I, hVector2 N, Half eta) => hVector2.Refract(I, N, eta);
        
        /// <summary>
        /// Returns a Vectortor pointing in the same direction as another (faceforward orients a Vectortor to point away from a surface as defined by its normal. If dot(Nref, I) is negative faceforward returns N, otherwise it returns -N).
        /// </summary>
        public static hVector2 FaceForward(hVector2 N, hVector2 I, hVector2 Nref) => hVector2.FaceForward(N, I, Nref);
        
        /// <summary>
        /// Returns the length of the outer product (cross product, Vectortor product) of the two Vectortors.
        /// </summary>
        public static Half Cross(hVector2 l, hVector2 r) => hVector2.Cross(l, r);
        
        /// <summary>
        /// Returns a hVector2 with independent and identically distributed uniform values between 'minValue' and 'maxValue'.
        /// </summary>
        public static hVector2 Random(System.Random random, hVector2 minValue, hVector2 maxValue) => hVector2.Random(random, minValue, maxValue);
        
        /// <summary>
        /// Returns a hVector2 with independent and identically distributed uniform values between 'minValue' and 'maxValue'.
        /// </summary>
        public static hVector2 RandomUniform(System.Random random, hVector2 minValue, hVector2 maxValue) => hVector2.RandomUniform(random, minValue, maxValue);
        
        /// <summary>
        /// Returns a hVector2 with independent and identically distributed values according to a normal/Gaussian distribution with specified mean and variance.
        /// </summary>
        public static hVector2 RandomNormal(System.Random random, hVector2 mean, hVector2 variance) => hVector2.RandomNormal(random, mean, variance);
        
        /// <summary>
        /// Returns a hVector2 with independent and identically distributed values according to a normal/Gaussian distribution with specified mean and variance.
        /// </summary>
        public static hVector2 RandomGaussian(System.Random random, hVector2 mean, hVector2 variance) => hVector2.RandomGaussian(random, mean, variance);

    }
}
