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
        public static swizzle_hVector4 swizzle(hVector4 v) => v.swizzle;
        
        /// <summary>
        /// Returns an array with all values
        /// </summary>
        public static Half[] Values(hVector4 v) => v.Values;
        
        /// <summary>
        /// Returns an enumerator that iterates through all components.
        /// </summary>
        public static IEnumerator<Half> GetEnumerator(hVector4 v) => v.GetEnumerator();
        
        /// <summary>
        /// Returns a string representation of this Vectortor using ', ' as a seperator.
        /// </summary>
        public static string ToString(hVector4 v) => v.ToString();
        
        /// <summary>
        /// Returns a string representation of this Vectortor using a provided seperator.
        /// </summary>
        public static string ToString(hVector4 v, string sep) => v.ToString(sep);
        
        /// <summary>
        /// Returns a string representation of this Vectortor using a provided seperator and a forMatrix provider for each component.
        /// </summary>
        public static string ToString(hVector4 v, string sep, IFormatProvider provider) => v.ToString(sep, provider);
        
        /// <summary>
        /// Returns a string representation of this Vectortor using a provided seperator and a forMatrix for each component.
        /// </summary>
        public static string ToString(hVector4 v, string sep, string forMatrix) => v.ToString(sep, forMatrix);
        
        /// <summary>
        /// Returns a string representation of this Vectortor using a provided seperator and a forMatrix and forMatrix provider for each component.
        /// </summary>
        public static string ToString(hVector4 v, string sep, string forMatrix, IFormatProvider provider) => v.ToString(sep, forMatrix, provider);
        
        /// <summary>
        /// Returns the number of components (4).
        /// </summary>
        public static int Count(hVector4 v) => v.Count;
        
        /// <summary>
        /// Returns true iff this equals rhs component-wise.
        /// </summary>
        public static bool Equals(hVector4 v, hVector4 rhs) => v.Equals(rhs);
        
        /// <summary>
        /// Returns true iff this equals rhs type- and component-wise.
        /// </summary>
        public static bool Equals(hVector4 v, object obj) => v.Equals(obj);
        
        /// <summary>
        /// Returns a hash code for this instance.
        /// </summary>
        public static int GetHashCode(hVector4 v) => v.GetHashCode();
        
        /// <summary>
        /// Returns true iff distance between lhs and rhs is less than or equal to epsilon
        /// </summary>
        public static bool ApproxEqual(hVector4 lhs, hVector4 rhs, float eps = 0.1f) => hVector4.ApproxEqual(lhs, rhs, eps);
        
        /// <summary>
        /// Returns a bVector4 from component-wise application of Equal (lhs == rhs).
        /// </summary>
        public static bVector4 Equal(hVector4 lhs, hVector4 rhs) => hVector4.Equal(lhs, rhs);
        
        /// <summary>
        /// Returns a bVector4 from component-wise application of NotEqual (lhs != rhs).
        /// </summary>
        public static bVector4 NotEqual(hVector4 lhs, hVector4 rhs) => hVector4.NotEqual(lhs, rhs);
        
        /// <summary>
        /// Returns a bVector4 from component-wise application of GreaterThan (lhs &gt; rhs).
        /// </summary>
        public static bVector4 GreaterThan(hVector4 lhs, hVector4 rhs) => hVector4.GreaterThan(lhs, rhs);
        
        /// <summary>
        /// Returns a bVector4 from component-wise application of GreaterThanEqual (lhs &gt;= rhs).
        /// </summary>
        public static bVector4 GreaterThanEqual(hVector4 lhs, hVector4 rhs) => hVector4.GreaterThanEqual(lhs, rhs);
        
        /// <summary>
        /// Returns a bVector4 from component-wise application of LesserThan (lhs &lt; rhs).
        /// </summary>
        public static bVector4 LesserThan(hVector4 lhs, hVector4 rhs) => hVector4.LesserThan(lhs, rhs);
        
        /// <summary>
        /// Returns a bVector4 from component-wise application of LesserThanEqual (lhs &lt;= rhs).
        /// </summary>
        public static bVector4 LesserThanEqual(hVector4 lhs, hVector4 rhs) => hVector4.LesserThanEqual(lhs, rhs);
        
        /// <summary>
        /// Returns a bVector4 from component-wise application of IsInfinity (Half.IsInfinity(v)).
        /// </summary>
        public static bVector4 IsInfinity(hVector4 v) => hVector4.IsInfinity(v);
        
        /// <summary>
        /// Returns a bVector4 from component-wise application of IsFinite (!Half.IsNaN(v) &amp;&amp; !Half.IsInfinity(v)).
        /// </summary>
        public static bVector4 IsFinite(hVector4 v) => hVector4.IsFinite(v);
        
        /// <summary>
        /// Returns a bVector4 from component-wise application of IsNaN (Half.IsNaN(v)).
        /// </summary>
        public static bVector4 IsNaN(hVector4 v) => hVector4.IsNaN(v);
        
        /// <summary>
        /// Returns a bVector4 from component-wise application of IsNegativeInfinity (Half.IsNegativeInfinity(v)).
        /// </summary>
        public static bVector4 IsNegativeInfinity(hVector4 v) => hVector4.IsNegativeInfinity(v);
        
        /// <summary>
        /// Returns a bVector4 from component-wise application of IsPositiveInfinity (Half.IsPositiveInfinity(v)).
        /// </summary>
        public static bVector4 IsPositiveInfinity(hVector4 v) => hVector4.IsPositiveInfinity(v);
        
        /// <summary>
        /// Returns a hVector4 from component-wise application of Abs (Half.Abs(v)).
        /// </summary>
        public static hVector4 Abs(hVector4 v) => hVector4.Abs(v);
        
        /// <summary>
        /// Returns a hVector4 from component-wise application of HermiteInterpolationOrder3 ((3 - 2 * v) * v * v).
        /// </summary>
        public static hVector4 HermiteInterpolationOrder3(hVector4 v) => hVector4.HermiteInterpolationOrder3(v);
        
        /// <summary>
        /// Returns a hVector4 from component-wise application of HermiteInterpolationOrder5 (((6 * v - 15) * v + 10) * v * v * v).
        /// </summary>
        public static hVector4 HermiteInterpolationOrder5(hVector4 v) => hVector4.HermiteInterpolationOrder5(v);
        
        /// <summary>
        /// Returns a hVector4 from component-wise application of Sqr (v * v).
        /// </summary>
        public static hVector4 Sqr(hVector4 v) => hVector4.Sqr(v);
        
        /// <summary>
        /// Returns a hVector4 from component-wise application of Pow2 (v * v).
        /// </summary>
        public static hVector4 Pow2(hVector4 v) => hVector4.Pow2(v);
        
        /// <summary>
        /// Returns a hVector4 from component-wise application of Pow3 (v * v * v).
        /// </summary>
        public static hVector4 Pow3(hVector4 v) => hVector4.Pow3(v);
        
        /// <summary>
        /// Returns a hVector4 from component-wise application of Step (v &gt;= Half.Zero ? Half.One : Half.Zero).
        /// </summary>
        public static hVector4 Step(hVector4 v) => hVector4.Step(v);
        
        /// <summary>
        /// Returns a hVector4 from component-wise application of Sqrt ((Half)Math.Sqrt((double)v)).
        /// </summary>
        public static hVector4 Sqrt(hVector4 v) => hVector4.Sqrt(v);
        
        /// <summary>
        /// Returns a hVector4 from component-wise application of InverseSqrt ((Half)(1.0 / Math.Sqrt((double)v))).
        /// </summary>
        public static hVector4 InverseSqrt(hVector4 v) => hVector4.InverseSqrt(v);
        
        /// <summary>
        /// Returns a iVector4 from component-wise application of Sign (Math.Sign(v)).
        /// </summary>
        public static iVector4 Sign(hVector4 v) => hVector4.Sign(v);
        
        /// <summary>
        /// Returns a hVector4 from component-wise application of Max (Half.Max(lhs, rhs)).
        /// </summary>
        public static hVector4 Max(hVector4 lhs, hVector4 rhs) => hVector4.Max(lhs, rhs);
        
        /// <summary>
        /// Returns a hVector4 from component-wise application of Min (Half.Min(lhs, rhs)).
        /// </summary>
        public static hVector4 Min(hVector4 lhs, hVector4 rhs) => hVector4.Min(lhs, rhs);
        
        /// <summary>
        /// Returns a hVector4 from component-wise application of Pow ((Half)Math.Pow((double)lhs, (double)rhs)).
        /// </summary>
        public static hVector4 Pow(hVector4 lhs, hVector4 rhs) => hVector4.Pow(lhs, rhs);
        
        /// <summary>
        /// Returns a hVector4 from component-wise application of Log ((Half)Math.Log((double)lhs, (double)rhs)).
        /// </summary>
        public static hVector4 Log(hVector4 lhs, hVector4 rhs) => hVector4.Log(lhs, rhs);
        
        /// <summary>
        /// Returns a hVector4 from component-wise application of Clamp (Half.Min(Half.Max(v, min), max)).
        /// </summary>
        public static hVector4 Clamp(hVector4 v, hVector4 min, hVector4 max) => hVector4.Clamp(v, min, max);
        
        /// <summary>
        /// Returns a hVector4 from component-wise application of Mix (min * (1-a) + max * a).
        /// </summary>
        public static hVector4 Mix(hVector4 min, hVector4 max, hVector4 a) => hVector4.Mix(min, max, a);
        
        /// <summary>
        /// Returns a hVector4 from component-wise application of Lerp (min * (1-a) + max * a).
        /// </summary>
        public static hVector4 Lerp(hVector4 min, hVector4 max, hVector4 a) => hVector4.Lerp(min, max, a);
        
        /// <summary>
        /// Returns a hVector4 from component-wise application of Smoothstep (((v - edge0) / (edge1 - edge0)).Clamp().HermiteInterpolationOrder3()).
        /// </summary>
        public static hVector4 Smoothstep(hVector4 edge0, hVector4 edge1, hVector4 v) => hVector4.Smoothstep(edge0, edge1, v);
        
        /// <summary>
        /// Returns a hVector4 from component-wise application of Smootherstep (((v - edge0) / (edge1 - edge0)).Clamp().HermiteInterpolationOrder5()).
        /// </summary>
        public static hVector4 Smootherstep(hVector4 edge0, hVector4 edge1, hVector4 v) => hVector4.Smootherstep(edge0, edge1, v);
        
        /// <summary>
        /// Returns a hVector4 from component-wise application of Fma (a * b + c).
        /// </summary>
        public static hVector4 Fma(hVector4 a, hVector4 b, hVector4 c) => hVector4.Fma(a, b, c);
        
        /// <summary>
        /// OuterProduct treats the first parameter c as a column Vectortor (Matrix with one column) and the second parameter r as a row Vectortor (Matrix with one row) and does a linear algebraic Matrix multiply c * r, yielding a Matrix whose number of rows is the number of components in c and whose number of columns is the number of components in r.
        /// </summary>
        public static hMatrix2x4 OuterProduct(hVector4 c, hVector2 r) => hVector4.OuterProduct(c, r);
        
        /// <summary>
        /// OuterProduct treats the first parameter c as a column Vectortor (Matrix with one column) and the second parameter r as a row Vectortor (Matrix with one row) and does a linear algebraic Matrix multiply c * r, yielding a Matrix whose number of rows is the number of components in c and whose number of columns is the number of components in r.
        /// </summary>
        public static hMatrix3x4 OuterProduct(hVector4 c, hVector3 r) => hVector4.OuterProduct(c, r);
        
        /// <summary>
        /// OuterProduct treats the first parameter c as a column Vectortor (Matrix with one column) and the second parameter r as a row Vectortor (Matrix with one row) and does a linear algebraic Matrix multiply c * r, yielding a Matrix whose number of rows is the number of components in c and whose number of columns is the number of components in r.
        /// </summary>
        public static hMatrix4 OuterProduct(hVector4 c, hVector4 r) => hVector4.OuterProduct(c, r);
        
        /// <summary>
        /// Returns a hVector4 from component-wise application of Add (lhs + rhs).
        /// </summary>
        public static hVector4 Add(hVector4 lhs, hVector4 rhs) => hVector4.Add(lhs, rhs);
        
        /// <summary>
        /// Returns a hVector4 from component-wise application of Sub (lhs - rhs).
        /// </summary>
        public static hVector4 Sub(hVector4 lhs, hVector4 rhs) => hVector4.Sub(lhs, rhs);
        
        /// <summary>
        /// Returns a hVector4 from component-wise application of Mul (lhs * rhs).
        /// </summary>
        public static hVector4 Mul(hVector4 lhs, hVector4 rhs) => hVector4.Mul(lhs, rhs);
        
        /// <summary>
        /// Returns a hVector4 from component-wise application of Div (lhs / rhs).
        /// </summary>
        public static hVector4 Div(hVector4 lhs, hVector4 rhs) => hVector4.Div(lhs, rhs);
        
        /// <summary>
        /// Returns a hVector4 from component-wise application of Modulo (lhs % rhs).
        /// </summary>
        public static hVector4 Modulo(hVector4 lhs, hVector4 rhs) => hVector4.Modulo(lhs, rhs);
        
        /// <summary>
        /// Returns a hVector4 from component-wise application of Degrees (Radians-To-Degrees Conversion).
        /// </summary>
        public static hVector4 Degrees(hVector4 v) => hVector4.Degrees(v);
        
        /// <summary>
        /// Returns a hVector4 from component-wise application of Radians (Degrees-To-Radians Conversion).
        /// </summary>
        public static hVector4 Radians(hVector4 v) => hVector4.Radians(v);
        
        /// <summary>
        /// Returns a hVector4 from component-wise application of Acos ((Half)Math.Acos((double)v)).
        /// </summary>
        public static hVector4 Acos(hVector4 v) => hVector4.Acos(v);
        
        /// <summary>
        /// Returns a hVector4 from component-wise application of Asin ((Half)Math.Asin((double)v)).
        /// </summary>
        public static hVector4 Asin(hVector4 v) => hVector4.Asin(v);
        
        /// <summary>
        /// Returns a hVector4 from component-wise application of Atan ((Half)Math.Atan((double)v)).
        /// </summary>
        public static hVector4 Atan(hVector4 v) => hVector4.Atan(v);
        
        /// <summary>
        /// Returns a hVector4 from component-wise application of Cos ((Half)Math.Cos((double)v)).
        /// </summary>
        public static hVector4 Cos(hVector4 v) => hVector4.Cos(v);
        
        /// <summary>
        /// Returns a hVector4 from component-wise application of Cosh ((Half)Math.Cosh((double)v)).
        /// </summary>
        public static hVector4 Cosh(hVector4 v) => hVector4.Cosh(v);
        
        /// <summary>
        /// Returns a hVector4 from component-wise application of Exp ((Half)Math.Exp((double)v)).
        /// </summary>
        public static hVector4 Exp(hVector4 v) => hVector4.Exp(v);
        
        /// <summary>
        /// Returns a hVector4 from component-wise application of Log ((Half)Math.Log((double)v)).
        /// </summary>
        public static hVector4 Log(hVector4 v) => hVector4.Log(v);
        
        /// <summary>
        /// Returns a hVector4 from component-wise application of Log2 ((Half)Math.Log((double)v, 2)).
        /// </summary>
        public static hVector4 Log2(hVector4 v) => hVector4.Log2(v);
        
        /// <summary>
        /// Returns a hVector4 from component-wise application of Log10 ((Half)Math.Log10((double)v)).
        /// </summary>
        public static hVector4 Log10(hVector4 v) => hVector4.Log10(v);
        
        /// <summary>
        /// Returns a hVector4 from component-wise application of Floor ((Half)Math.Floor(v)).
        /// </summary>
        public static hVector4 Floor(hVector4 v) => hVector4.Floor(v);
        
        /// <summary>
        /// Returns a hVector4 from component-wise application of Ceiling ((Half)Math.Ceiling(v)).
        /// </summary>
        public static hVector4 Ceiling(hVector4 v) => hVector4.Ceiling(v);
        
        /// <summary>
        /// Returns a hVector4 from component-wise application of Round ((Half)Math.Round(v)).
        /// </summary>
        public static hVector4 Round(hVector4 v) => hVector4.Round(v);
        
        /// <summary>
        /// Returns a hVector4 from component-wise application of Sin ((Half)Math.Sin((double)v)).
        /// </summary>
        public static hVector4 Sin(hVector4 v) => hVector4.Sin(v);
        
        /// <summary>
        /// Returns a hVector4 from component-wise application of Sinh ((Half)Math.Sinh((double)v)).
        /// </summary>
        public static hVector4 Sinh(hVector4 v) => hVector4.Sinh(v);
        
        /// <summary>
        /// Returns a hVector4 from component-wise application of Tan ((Half)Math.Tan((double)v)).
        /// </summary>
        public static hVector4 Tan(hVector4 v) => hVector4.Tan(v);
        
        /// <summary>
        /// Returns a hVector4 from component-wise application of Tanh ((Half)Math.Tanh((double)v)).
        /// </summary>
        public static hVector4 Tanh(hVector4 v) => hVector4.Tanh(v);
        
        /// <summary>
        /// Returns a hVector4 from component-wise application of Truncate ((Half)Math.Truncate((double)v)).
        /// </summary>
        public static hVector4 Truncate(hVector4 v) => hVector4.Truncate(v);
        
        /// <summary>
        /// Returns a hVector4 from component-wise application of Fract ((Half)(v - Math.Floor(v))).
        /// </summary>
        public static hVector4 Fract(hVector4 v) => hVector4.Fract(v);
        
        /// <summary>
        /// Returns a hVector4 from component-wise application of Trunc ((long)(v)).
        /// </summary>
        public static hVector4 Trunc(hVector4 v) => hVector4.Trunc(v);
        
        /// <summary>
        /// Returns the minimal component of this Vectortor.
        /// </summary>
        public static Half MinElement(hVector4 v) => v.MinElement;
        
        /// <summary>
        /// Returns the maximal component of this Vectortor.
        /// </summary>
        public static Half MaxElement(hVector4 v) => v.MaxElement;
        
        /// <summary>
        /// Returns the euclidean length of this Vectortor.
        /// </summary>
        public static float Length(hVector4 v) => v.Length;
        
        /// <summary>
        /// Returns the squared euclidean length of this Vectortor.
        /// </summary>
        public static float LengthSqr(hVector4 v) => v.LengthSqr;
        
        /// <summary>
        /// Returns the sum of all components.
        /// </summary>
        public static Half Sum(hVector4 v) => v.Sum;
        
        /// <summary>
        /// Returns the euclidean norm of this Vectortor.
        /// </summary>
        public static float Norm(hVector4 v) => v.Norm;
        
        /// <summary>
        /// Returns the one-norm of this Vectortor.
        /// </summary>
        public static float Norm1(hVector4 v) => v.Norm1;
        
        /// <summary>
        /// Returns the two-norm (euclidean length) of this Vectortor.
        /// </summary>
        public static float Norm2(hVector4 v) => v.Norm2;
        
        /// <summary>
        /// Returns the max-norm of this Vectortor.
        /// </summary>
        public static float NormMax(hVector4 v) => v.NormMax;
        
        /// <summary>
        /// Returns the p-norm of this Vectortor.
        /// </summary>
        public static double NormP(hVector4 v, double p) => v.NormP(p);
        
        /// <summary>
        /// Returns a copy of this Vectortor with length one (undefined if this has zero length).
        /// </summary>
        public static hVector4 Normalized(hVector4 v) => v.Normalized;
        
        /// <summary>
        /// Returns a copy of this Vectortor with length one (returns zero if length is zero).
        /// </summary>
        public static hVector4 NormalizedSafe(hVector4 v) => v.NormalizedSafe;
        
        /// <summary>
        /// Returns the inner product (dot product, scalar product) of the two Vectortors.
        /// </summary>
        public static Half Dot(hVector4 lhs, hVector4 rhs) => hVector4.Dot(lhs, rhs);
        
        /// <summary>
        /// Returns the euclidean distance between the two Vectortors.
        /// </summary>
        public static float Distance(hVector4 lhs, hVector4 rhs) => hVector4.Distance(lhs, rhs);
        
        /// <summary>
        /// Returns the squared euclidean distance between the two Vectortors.
        /// </summary>
        public static float DistanceSqr(hVector4 lhs, hVector4 rhs) => hVector4.DistanceSqr(lhs, rhs);
        
        /// <summary>
        /// Calculate the reflection direction for an incident Vectortor (N should be normalized in order to achieve the desired result).
        /// </summary>
        public static hVector4 Reflect(hVector4 I, hVector4 N) => hVector4.Reflect(I, N);
        
        /// <summary>
        /// Calculate the refraction direction for an incident Vectortor (The input parameters I and N should be normalized in order to achieve the desired result).
        /// </summary>
        public static hVector4 Refract(hVector4 I, hVector4 N, Half eta) => hVector4.Refract(I, N, eta);
        
        /// <summary>
        /// Returns a Vectortor pointing in the same direction as another (faceforward orients a Vectortor to point away from a surface as defined by its normal. If dot(Nref, I) is negative faceforward returns N, otherwise it returns -N).
        /// </summary>
        public static hVector4 FaceForward(hVector4 N, hVector4 I, hVector4 Nref) => hVector4.FaceForward(N, I, Nref);
        
        /// <summary>
        /// Returns a hVector4 with independent and identically distributed uniform values between 'minValue' and 'maxValue'.
        /// </summary>
        public static hVector4 Random(System.Random random, hVector4 minValue, hVector4 maxValue) => hVector4.Random(random, minValue, maxValue);
        
        /// <summary>
        /// Returns a hVector4 with independent and identically distributed uniform values between 'minValue' and 'maxValue'.
        /// </summary>
        public static hVector4 RandomUniform(System.Random random, hVector4 minValue, hVector4 maxValue) => hVector4.RandomUniform(random, minValue, maxValue);
        
        /// <summary>
        /// Returns a hVector4 with independent and identically distributed values according to a normal/Gaussian distribution with specified mean and variance.
        /// </summary>
        public static hVector4 RandomNormal(System.Random random, hVector4 mean, hVector4 variance) => hVector4.RandomNormal(random, mean, variance);
        
        /// <summary>
        /// Returns a hVector4 with independent and identically distributed values according to a normal/Gaussian distribution with specified mean and variance.
        /// </summary>
        public static hVector4 RandomGaussian(System.Random random, hVector4 mean, hVector4 variance) => hVector4.RandomGaussian(random, mean, variance);

    }
}
