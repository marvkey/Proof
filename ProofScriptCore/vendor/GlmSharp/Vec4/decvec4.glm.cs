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
        public static swizzle_decVector4 swizzle(decVector4 v) => v.swizzle;
        
        /// <summary>
        /// Returns an array with all values
        /// </summary>
        public static decimal[] Values(decVector4 v) => v.Values;
        
        /// <summary>
        /// Returns an enumerator that iterates through all components.
        /// </summary>
        public static IEnumerator<decimal> GetEnumerator(decVector4 v) => v.GetEnumerator();
        
        /// <summary>
        /// Returns a string representation of this Vector using ', ' as a seperator.
        /// </summary>
        public static string ToString(decVector4 v) => v.ToString();
        
        /// <summary>
        /// Returns a string representation of this Vector using a provided seperator.
        /// </summary>
        public static string ToString(decVector4 v, string sep) => v.ToString(sep);
        
        /// <summary>
        /// Returns a string representation of this Vector using a provided seperator and a forMatrix provider for each component.
        /// </summary>
        public static string ToString(decVector4 v, string sep, IFormatProvider provider) => v.ToString(sep, provider);
        
        /// <summary>
        /// Returns a string representation of this Vector using a provided seperator and a forMatrix for each component.
        /// </summary>
        public static string ToString(decVector4 v, string sep, string forMatrix) => v.ToString(sep, forMatrix);
        
        /// <summary>
        /// Returns a string representation of this Vector using a provided seperator and a forMatrix and forMatrix provider for each component.
        /// </summary>
        public static string ToString(decVector4 v, string sep, string forMatrix, IFormatProvider provider) => v.ToString(sep, forMatrix, provider);
        
        /// <summary>
        /// Returns the number of components (4).
        /// </summary>
        public static int Count(decVector4 v) => v.Count;
        
        /// <summary>
        /// Returns true iff this equals rhs component-wise.
        /// </summary>
        public static bool Equals(decVector4 v, decVector4 rhs) => v.Equals(rhs);
        
        /// <summary>
        /// Returns true iff this equals rhs type- and component-wise.
        /// </summary>
        public static bool Equals(decVector4 v, object obj) => v.Equals(obj);
        
        /// <summary>
        /// Returns a hash code for this instance.
        /// </summary>
        public static int GetHashCode(decVector4 v) => v.GetHashCode();
        
        /// <summary>
        /// Returns true iff distance between lhs and rhs is less than or equal to epsilon
        /// </summary>
        public static bool ApproxEqual(decVector4 lhs, decVector4 rhs, decimal eps = 0.1m) => decVector4.ApproxEqual(lhs, rhs, eps);
        
        /// <summary>
        /// Returns a bVector4 from component-wise application of Equal (lhs == rhs).
        /// </summary>
        public static bVector4 Equal(decVector4 lhs, decVector4 rhs) => decVector4.Equal(lhs, rhs);
        
        /// <summary>
        /// Returns a bVector4 from component-wise application of NotEqual (lhs != rhs).
        /// </summary>
        public static bVector4 NotEqual(decVector4 lhs, decVector4 rhs) => decVector4.NotEqual(lhs, rhs);
        
        /// <summary>
        /// Returns a bVector4 from component-wise application of GreaterThan (lhs &gt; rhs).
        /// </summary>
        public static bVector4 GreaterThan(decVector4 lhs, decVector4 rhs) => decVector4.GreaterThan(lhs, rhs);
        
        /// <summary>
        /// Returns a bVector4 from component-wise application of GreaterThanEqual (lhs &gt;= rhs).
        /// </summary>
        public static bVector4 GreaterThanEqual(decVector4 lhs, decVector4 rhs) => decVector4.GreaterThanEqual(lhs, rhs);
        
        /// <summary>
        /// Returns a bVector4 from component-wise application of LesserThan (lhs &lt; rhs).
        /// </summary>
        public static bVector4 LesserThan(decVector4 lhs, decVector4 rhs) => decVector4.LesserThan(lhs, rhs);
        
        /// <summary>
        /// Returns a bVector4 from component-wise application of LesserThanEqual (lhs &lt;= rhs).
        /// </summary>
        public static bVector4 LesserThanEqual(decVector4 lhs, decVector4 rhs) => decVector4.LesserThanEqual(lhs, rhs);
        
        /// <summary>
        /// Returns a decVector4 from component-wise application of Abs (Math.Abs(v)).
        /// </summary>
        public static decVector4 Abs(decVector4 v) => decVector4.Abs(v);
        
        /// <summary>
        /// Returns a decVector4 from component-wise application of HermiteInterpolationOrder3 ((3 - 2 * v) * v * v).
        /// </summary>
        public static decVector4 HermiteInterpolationOrder3(decVector4 v) => decVector4.HermiteInterpolationOrder3(v);
        
        /// <summary>
        /// Returns a decVector4 from component-wise application of HermiteInterpolationOrder5 (((6 * v - 15) * v + 10) * v * v * v).
        /// </summary>
        public static decVector4 HermiteInterpolationOrder5(decVector4 v) => decVector4.HermiteInterpolationOrder5(v);
        
        /// <summary>
        /// Returns a decVector4 from component-wise application of Sqr (v * v).
        /// </summary>
        public static decVector4 Sqr(decVector4 v) => decVector4.Sqr(v);
        
        /// <summary>
        /// Returns a decVector4 from component-wise application of Pow2 (v * v).
        /// </summary>
        public static decVector4 Pow2(decVector4 v) => decVector4.Pow2(v);
        
        /// <summary>
        /// Returns a decVector4 from component-wise application of Pow3 (v * v * v).
        /// </summary>
        public static decVector4 Pow3(decVector4 v) => decVector4.Pow3(v);
        
        /// <summary>
        /// Returns a decVector4 from component-wise application of Step (v &gt;= 0m ? 1m : 0m).
        /// </summary>
        public static decVector4 Step(decVector4 v) => decVector4.Step(v);
        
        /// <summary>
        /// Returns a decVector4 from component-wise application of Sqrt ((decimal)Math.Sqrt((double)v)).
        /// </summary>
        public static decVector4 Sqrt(decVector4 v) => decVector4.Sqrt(v);
        
        /// <summary>
        /// Returns a decVector4 from component-wise application of InverseSqrt ((decimal)(1.0 / Math.Sqrt((double)v))).
        /// </summary>
        public static decVector4 InverseSqrt(decVector4 v) => decVector4.InverseSqrt(v);
        
        /// <summary>
        /// Returns a iVector4 from component-wise application of Sign (Math.Sign(v)).
        /// </summary>
        public static iVector4 Sign(decVector4 v) => decVector4.Sign(v);
        
        /// <summary>
        /// Returns a decVector4 from component-wise application of Max (Math.Max(lhs, rhs)).
        /// </summary>
        public static decVector4 Max(decVector4 lhs, decVector4 rhs) => decVector4.Max(lhs, rhs);
        
        /// <summary>
        /// Returns a decVector4 from component-wise application of Min (Math.Min(lhs, rhs)).
        /// </summary>
        public static decVector4 Min(decVector4 lhs, decVector4 rhs) => decVector4.Min(lhs, rhs);
        
        /// <summary>
        /// Returns a decVector4 from component-wise application of Pow ((decimal)Math.Pow((double)lhs, (double)rhs)).
        /// </summary>
        public static decVector4 Pow(decVector4 lhs, decVector4 rhs) => decVector4.Pow(lhs, rhs);
        
        /// <summary>
        /// Returns a decVector4 from component-wise application of Log ((decimal)Math.Log((double)lhs, (double)rhs)).
        /// </summary>
        public static decVector4 Log(decVector4 lhs, decVector4 rhs) => decVector4.Log(lhs, rhs);
        
        /// <summary>
        /// Returns a decVector4 from component-wise application of Clamp (Math.Min(Math.Max(v, min), max)).
        /// </summary>
        public static decVector4 Clamp(decVector4 v, decVector4 min, decVector4 max) => decVector4.Clamp(v, min, max);
        
        /// <summary>
        /// Returns a decVector4 from component-wise application of Mix (min * (1-a) + max * a).
        /// </summary>
        public static decVector4 Mix(decVector4 min, decVector4 max, decVector4 a) => decVector4.Mix(min, max, a);
        
        /// <summary>
        /// Returns a decVector4 from component-wise application of Lerp (min * (1-a) + max * a).
        /// </summary>
        public static decVector4 Lerp(decVector4 min, decVector4 max, decVector4 a) => decVector4.Lerp(min, max, a);
        
        /// <summary>
        /// Returns a decVector4 from component-wise application of Smoothstep (((v - edge0) / (edge1 - edge0)).Clamp().HermiteInterpolationOrder3()).
        /// </summary>
        public static decVector4 Smoothstep(decVector4 edge0, decVector4 edge1, decVector4 v) => decVector4.Smoothstep(edge0, edge1, v);
        
        /// <summary>
        /// Returns a decVector4 from component-wise application of Smootherstep (((v - edge0) / (edge1 - edge0)).Clamp().HermiteInterpolationOrder5()).
        /// </summary>
        public static decVector4 Smootherstep(decVector4 edge0, decVector4 edge1, decVector4 v) => decVector4.Smootherstep(edge0, edge1, v);
        
        /// <summary>
        /// Returns a decVector4 from component-wise application of Fma (a * b + c).
        /// </summary>
        public static decVector4 Fma(decVector4 a, decVector4 b, decVector4 c) => decVector4.Fma(a, b, c);
        
        /// <summary>
        /// OuterProduct treats the first parameter c as a column Vector (Matrix with one column) and the second parameter r as a row Vector (Matrix with one row) and does a linear algebraic Matrix multiply c * r, yielding a Matrix whose number of rows is the number of components in c and whose number of columns is the number of components in r.
        /// </summary>
        public static decMatrix2x4 OuterProduct(decVector4 c, decVector2 r) => decVector4.OuterProduct(c, r);
        
        /// <summary>
        /// OuterProduct treats the first parameter c as a column Vector (Matrix with one column) and the second parameter r as a row Vector (Matrix with one row) and does a linear algebraic Matrix multiply c * r, yielding a Matrix whose number of rows is the number of components in c and whose number of columns is the number of components in r.
        /// </summary>
        public static decMatrix3x4 OuterProduct(decVector4 c, decVector3 r) => decVector4.OuterProduct(c, r);
        
        /// <summary>
        /// OuterProduct treats the first parameter c as a column Vector (Matrix with one column) and the second parameter r as a row Vector (Matrix with one row) and does a linear algebraic Matrix multiply c * r, yielding a Matrix whose number of rows is the number of components in c and whose number of columns is the number of components in r.
        /// </summary>
        public static decMatrix4 OuterProduct(decVector4 c, decVector4 r) => decVector4.OuterProduct(c, r);
        
        /// <summary>
        /// Returns a decVector4 from component-wise application of Add (lhs + rhs).
        /// </summary>
        public static decVector4 Add(decVector4 lhs, decVector4 rhs) => decVector4.Add(lhs, rhs);
        
        /// <summary>
        /// Returns a decVector4 from component-wise application of Sub (lhs - rhs).
        /// </summary>
        public static decVector4 Sub(decVector4 lhs, decVector4 rhs) => decVector4.Sub(lhs, rhs);
        
        /// <summary>
        /// Returns a decVector4 from component-wise application of Mul (lhs * rhs).
        /// </summary>
        public static decVector4 Mul(decVector4 lhs, decVector4 rhs) => decVector4.Mul(lhs, rhs);
        
        /// <summary>
        /// Returns a decVector4 from component-wise application of Div (lhs / rhs).
        /// </summary>
        public static decVector4 Div(decVector4 lhs, decVector4 rhs) => decVector4.Div(lhs, rhs);
        
        /// <summary>
        /// Returns a decVector4 from component-wise application of Modulo (lhs % rhs).
        /// </summary>
        public static decVector4 Modulo(decVector4 lhs, decVector4 rhs) => decVector4.Modulo(lhs, rhs);
        
        /// <summary>
        /// Returns a decVector4 from component-wise application of Degrees (Radians-To-Degrees Conversion).
        /// </summary>
        public static decVector4 Degrees(decVector4 v) => decVector4.Degrees(v);
        
        /// <summary>
        /// Returns a decVector4 from component-wise application of Radians (Degrees-To-Radians Conversion).
        /// </summary>
        public static decVector4 Radians(decVector4 v) => decVector4.Radians(v);
        
        /// <summary>
        /// Returns a decVector4 from component-wise application of Acos ((decimal)Math.Acos((double)v)).
        /// </summary>
        public static decVector4 Acos(decVector4 v) => decVector4.Acos(v);
        
        /// <summary>
        /// Returns a decVector4 from component-wise application of Asin ((decimal)Math.Asin((double)v)).
        /// </summary>
        public static decVector4 Asin(decVector4 v) => decVector4.Asin(v);
        
        /// <summary>
        /// Returns a decVector4 from component-wise application of Atan ((decimal)Math.Atan((double)v)).
        /// </summary>
        public static decVector4 Atan(decVector4 v) => decVector4.Atan(v);
        
        /// <summary>
        /// Returns a decVector4 from component-wise application of Cos ((decimal)Math.Cos((double)v)).
        /// </summary>
        public static decVector4 Cos(decVector4 v) => decVector4.Cos(v);
        
        /// <summary>
        /// Returns a decVector4 from component-wise application of Cosh ((decimal)Math.Cosh((double)v)).
        /// </summary>
        public static decVector4 Cosh(decVector4 v) => decVector4.Cosh(v);
        
        /// <summary>
        /// Returns a decVector4 from component-wise application of Exp ((decimal)Math.Exp((double)v)).
        /// </summary>
        public static decVector4 Exp(decVector4 v) => decVector4.Exp(v);
        
        /// <summary>
        /// Returns a decVector4 from component-wise application of Log ((decimal)Math.Log((double)v)).
        /// </summary>
        public static decVector4 Log(decVector4 v) => decVector4.Log(v);
        
        /// <summary>
        /// Returns a decVector4 from component-wise application of Log2 ((decimal)Math.Log((double)v, 2)).
        /// </summary>
        public static decVector4 Log2(decVector4 v) => decVector4.Log2(v);
        
        /// <summary>
        /// Returns a decVector4 from component-wise application of Log10 ((decimal)Math.Log10((double)v)).
        /// </summary>
        public static decVector4 Log10(decVector4 v) => decVector4.Log10(v);
        
        /// <summary>
        /// Returns a decVector4 from component-wise application of Floor ((decimal)Math.Floor(v)).
        /// </summary>
        public static decVector4 Floor(decVector4 v) => decVector4.Floor(v);
        
        /// <summary>
        /// Returns a decVector4 from component-wise application of Ceiling ((decimal)Math.Ceiling(v)).
        /// </summary>
        public static decVector4 Ceiling(decVector4 v) => decVector4.Ceiling(v);
        
        /// <summary>
        /// Returns a decVector4 from component-wise application of Round ((decimal)Math.Round(v)).
        /// </summary>
        public static decVector4 Round(decVector4 v) => decVector4.Round(v);
        
        /// <summary>
        /// Returns a decVector4 from component-wise application of Sin ((decimal)Math.Sin((double)v)).
        /// </summary>
        public static decVector4 Sin(decVector4 v) => decVector4.Sin(v);
        
        /// <summary>
        /// Returns a decVector4 from component-wise application of Sinh ((decimal)Math.Sinh((double)v)).
        /// </summary>
        public static decVector4 Sinh(decVector4 v) => decVector4.Sinh(v);
        
        /// <summary>
        /// Returns a decVector4 from component-wise application of Tan ((decimal)Math.Tan((double)v)).
        /// </summary>
        public static decVector4 Tan(decVector4 v) => decVector4.Tan(v);
        
        /// <summary>
        /// Returns a decVector4 from component-wise application of Tanh ((decimal)Math.Tanh((double)v)).
        /// </summary>
        public static decVector4 Tanh(decVector4 v) => decVector4.Tanh(v);
        
        /// <summary>
        /// Returns a decVector4 from component-wise application of Truncate ((decimal)Math.Truncate((double)v)).
        /// </summary>
        public static decVector4 Truncate(decVector4 v) => decVector4.Truncate(v);
        
        /// <summary>
        /// Returns a decVector4 from component-wise application of Fract ((decimal)(v - Math.Floor(v))).
        /// </summary>
        public static decVector4 Fract(decVector4 v) => decVector4.Fract(v);
        
        /// <summary>
        /// Returns a decVector4 from component-wise application of Trunc ((long)(v)).
        /// </summary>
        public static decVector4 Trunc(decVector4 v) => decVector4.Trunc(v);
        
        /// <summary>
        /// Returns the minimal component of this Vector.
        /// </summary>
        public static decimal MinElement(decVector4 v) => v.MinElement;
        
        /// <summary>
        /// Returns the maximal component of this Vector.
        /// </summary>
        public static decimal MaxElement(decVector4 v) => v.MaxElement;
        
        /// <summary>
        /// Returns the euclidean length of this Vector.
        /// </summary>
        public static decimal Length(decVector4 v) => v.Length;
        
        /// <summary>
        /// Returns the squared euclidean length of this Vector.
        /// </summary>
        public static decimal LengthSqr(decVector4 v) => v.LengthSqr;
        
        /// <summary>
        /// Returns the sum of all components.
        /// </summary>
        public static decimal Sum(decVector4 v) => v.Sum;
        
        /// <summary>
        /// Returns the euclidean norm of this Vector.
        /// </summary>
        public static decimal Norm(decVector4 v) => v.Norm;
        
        /// <summary>
        /// Returns the one-norm of this Vector.
        /// </summary>
        public static decimal Norm1(decVector4 v) => v.Norm1;
        
        /// <summary>
        /// Returns the two-norm (euclidean length) of this Vector.
        /// </summary>
        public static decimal Norm2(decVector4 v) => v.Norm2;
        
        /// <summary>
        /// Returns the max-norm of this Vector.
        /// </summary>
        public static decimal NormMax(decVector4 v) => v.NormMax;
        
        /// <summary>
        /// Returns the p-norm of this Vector.
        /// </summary>
        public static double NormP(decVector4 v, double p) => v.NormP(p);
        
        /// <summary>
        /// Returns a copy of this Vector with length one (undefined if this has zero length).
        /// </summary>
        public static decVector4 Normalized(decVector4 v) => v.Normalized;
        
        /// <summary>
        /// Returns a copy of this Vector with length one (returns zero if length is zero).
        /// </summary>
        public static decVector4 NormalizedSafe(decVector4 v) => v.NormalizedSafe;
        
        /// <summary>
        /// Returns the inner product (dot product, scalar product) of the two Vectors.
        /// </summary>
        public static decimal Dot(decVector4 lhs, decVector4 rhs) => decVector4.Dot(lhs, rhs);
        
        /// <summary>
        /// Returns the euclidean distance between the two Vectors.
        /// </summary>
        public static decimal Distance(decVector4 lhs, decVector4 rhs) => decVector4.Distance(lhs, rhs);
        
        /// <summary>
        /// Returns the squared euclidean distance between the two Vectors.
        /// </summary>
        public static decimal DistanceSqr(decVector4 lhs, decVector4 rhs) => decVector4.DistanceSqr(lhs, rhs);
        
        /// <summary>
        /// Calculate the reflection direction for an incident Vector (N should be normalized in order to achieve the desired result).
        /// </summary>
        public static decVector4 Reflect(decVector4 I, decVector4 N) => decVector4.Reflect(I, N);
        
        /// <summary>
        /// Calculate the refraction direction for an incident Vector (The input parameters I and N should be normalized in order to achieve the desired result).
        /// </summary>
        public static decVector4 Refract(decVector4 I, decVector4 N, decimal eta) => decVector4.Refract(I, N, eta);
        
        /// <summary>
        /// Returns a Vector pointing in the same direction as another (faceforward orients a Vector to point away from a surface as defined by its normal. If dot(Nref, I) is negative faceforward returns N, otherwise it returns -N).
        /// </summary>
        public static decVector4 FaceForward(decVector4 N, decVector4 I, decVector4 Nref) => decVector4.FaceForward(N, I, Nref);
        
        /// <summary>
        /// Returns a decVector4 with independent and identically distributed uniform values between 'minValue' and 'maxValue'.
        /// </summary>
        public static decVector4 Random(System.Random random, decVector4 minValue, decVector4 maxValue) => decVector4.Random(random, minValue, maxValue);
        
        /// <summary>
        /// Returns a decVector4 with independent and identically distributed uniform values between 'minValue' and 'maxValue'.
        /// </summary>
        public static decVector4 RandomUniform(System.Random random, decVector4 minValue, decVector4 maxValue) => decVector4.RandomUniform(random, minValue, maxValue);
        
        /// <summary>
        /// Returns a decVector4 with independent and identically distributed values according to a normal/Gaussian distribution with specified mean and variance.
        /// </summary>
        public static decVector4 RandomNormal(System.Random random, decVector4 mean, decVector4 variance) => decVector4.RandomNormal(random, mean, variance);
        
        /// <summary>
        /// Returns a decVector4 with independent and identically distributed values according to a normal/Gaussian distribution with specified mean and variance.
        /// </summary>
        public static decVector4 RandomGaussian(System.Random random, decVector4 mean, decVector4 variance) => decVector4.RandomGaussian(random, mean, variance);

    }
}
