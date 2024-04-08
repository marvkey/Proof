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
        public static swizzle_lVector4 swizzle(lVector4 v) => v.swizzle;
        
        /// <summary>
        /// Returns an array with all values
        /// </summary>
        public static long[] Values(lVector4 v) => v.Values;
        
        /// <summary>
        /// Returns an enumerator that iterates through all components.
        /// </summary>
        public static IEnumerator<long> GetEnumerator(lVector4 v) => v.GetEnumerator();
        
        /// <summary>
        /// Returns a string representation of this Vector using ', ' as a seperator.
        /// </summary>
        public static string ToString(lVector4 v) => v.ToString();
        
        /// <summary>
        /// Returns a string representation of this Vector using a provided seperator.
        /// </summary>
        public static string ToString(lVector4 v, string sep) => v.ToString(sep);
        
        /// <summary>
        /// Returns a string representation of this Vector using a provided seperator and a forMatrix provider for each component.
        /// </summary>
        public static string ToString(lVector4 v, string sep, IFormatProvider provider) => v.ToString(sep, provider);
        
        /// <summary>
        /// Returns a string representation of this Vector using a provided seperator and a forMatrix for each component.
        /// </summary>
        public static string ToString(lVector4 v, string sep, string forMatrix) => v.ToString(sep, forMatrix);
        
        /// <summary>
        /// Returns a string representation of this Vector using a provided seperator and a forMatrix and forMatrix provider for each component.
        /// </summary>
        public static string ToString(lVector4 v, string sep, string forMatrix, IFormatProvider provider) => v.ToString(sep, forMatrix, provider);
        
        /// <summary>
        /// Returns the number of components (4).
        /// </summary>
        public static int Count(lVector4 v) => v.Count;
        
        /// <summary>
        /// Returns true iff this equals rhs component-wise.
        /// </summary>
        public static bool Equals(lVector4 v, lVector4 rhs) => v.Equals(rhs);
        
        /// <summary>
        /// Returns true iff this equals rhs type- and component-wise.
        /// </summary>
        public static bool Equals(lVector4 v, object obj) => v.Equals(obj);
        
        /// <summary>
        /// Returns a hash code for this instance.
        /// </summary>
        public static int GetHashCode(lVector4 v) => v.GetHashCode();
        
        /// <summary>
        /// Returns a bVector4 from component-wise application of Equal (lhs == rhs).
        /// </summary>
        public static bVector4 Equal(lVector4 lhs, lVector4 rhs) => lVector4.Equal(lhs, rhs);
        
        /// <summary>
        /// Returns a bVector4 from component-wise application of NotEqual (lhs != rhs).
        /// </summary>
        public static bVector4 NotEqual(lVector4 lhs, lVector4 rhs) => lVector4.NotEqual(lhs, rhs);
        
        /// <summary>
        /// Returns a bVector4 from component-wise application of GreaterThan (lhs &gt; rhs).
        /// </summary>
        public static bVector4 GreaterThan(lVector4 lhs, lVector4 rhs) => lVector4.GreaterThan(lhs, rhs);
        
        /// <summary>
        /// Returns a bVector4 from component-wise application of GreaterThanEqual (lhs &gt;= rhs).
        /// </summary>
        public static bVector4 GreaterThanEqual(lVector4 lhs, lVector4 rhs) => lVector4.GreaterThanEqual(lhs, rhs);
        
        /// <summary>
        /// Returns a bVector4 from component-wise application of LesserThan (lhs &lt; rhs).
        /// </summary>
        public static bVector4 LesserThan(lVector4 lhs, lVector4 rhs) => lVector4.LesserThan(lhs, rhs);
        
        /// <summary>
        /// Returns a bVector4 from component-wise application of LesserThanEqual (lhs &lt;= rhs).
        /// </summary>
        public static bVector4 LesserThanEqual(lVector4 lhs, lVector4 rhs) => lVector4.LesserThanEqual(lhs, rhs);
        
        /// <summary>
        /// Returns a lVector4 from component-wise application of Abs (Math.Abs(v)).
        /// </summary>
        public static lVector4 Abs(lVector4 v) => lVector4.Abs(v);
        
        /// <summary>
        /// Returns a lVector4 from component-wise application of HermiteInterpolationOrder3 ((3 - 2 * v) * v * v).
        /// </summary>
        public static lVector4 HermiteInterpolationOrder3(lVector4 v) => lVector4.HermiteInterpolationOrder3(v);
        
        /// <summary>
        /// Returns a lVector4 from component-wise application of HermiteInterpolationOrder5 (((6 * v - 15) * v + 10) * v * v * v).
        /// </summary>
        public static lVector4 HermiteInterpolationOrder5(lVector4 v) => lVector4.HermiteInterpolationOrder5(v);
        
        /// <summary>
        /// Returns a lVector4 from component-wise application of Sqr (v * v).
        /// </summary>
        public static lVector4 Sqr(lVector4 v) => lVector4.Sqr(v);
        
        /// <summary>
        /// Returns a lVector4 from component-wise application of Pow2 (v * v).
        /// </summary>
        public static lVector4 Pow2(lVector4 v) => lVector4.Pow2(v);
        
        /// <summary>
        /// Returns a lVector4 from component-wise application of Pow3 (v * v * v).
        /// </summary>
        public static lVector4 Pow3(lVector4 v) => lVector4.Pow3(v);
        
        /// <summary>
        /// Returns a lVector4 from component-wise application of Step (v &gt;= 0 ? 1 : 0).
        /// </summary>
        public static lVector4 Step(lVector4 v) => lVector4.Step(v);
        
        /// <summary>
        /// Returns a lVector4 from component-wise application of Sqrt ((long)Math.Sqrt((double)v)).
        /// </summary>
        public static lVector4 Sqrt(lVector4 v) => lVector4.Sqrt(v);
        
        /// <summary>
        /// Returns a lVector4 from component-wise application of InverseSqrt ((long)(1.0 / Math.Sqrt((double)v))).
        /// </summary>
        public static lVector4 InverseSqrt(lVector4 v) => lVector4.InverseSqrt(v);
        
        /// <summary>
        /// Returns a iVector4 from component-wise application of Sign (Math.Sign(v)).
        /// </summary>
        public static iVector4 Sign(lVector4 v) => lVector4.Sign(v);
        
        /// <summary>
        /// Returns a lVector4 from component-wise application of Max (Math.Max(lhs, rhs)).
        /// </summary>
        public static lVector4 Max(lVector4 lhs, lVector4 rhs) => lVector4.Max(lhs, rhs);
        
        /// <summary>
        /// Returns a lVector4 from component-wise application of Min (Math.Min(lhs, rhs)).
        /// </summary>
        public static lVector4 Min(lVector4 lhs, lVector4 rhs) => lVector4.Min(lhs, rhs);
        
        /// <summary>
        /// Returns a lVector4 from component-wise application of Pow ((long)Math.Pow((double)lhs, (double)rhs)).
        /// </summary>
        public static lVector4 Pow(lVector4 lhs, lVector4 rhs) => lVector4.Pow(lhs, rhs);
        
        /// <summary>
        /// Returns a lVector4 from component-wise application of Log ((long)Math.Log((double)lhs, (double)rhs)).
        /// </summary>
        public static lVector4 Log(lVector4 lhs, lVector4 rhs) => lVector4.Log(lhs, rhs);
        
        /// <summary>
        /// Returns a lVector4 from component-wise application of Clamp (Math.Min(Math.Max(v, min), max)).
        /// </summary>
        public static lVector4 Clamp(lVector4 v, lVector4 min, lVector4 max) => lVector4.Clamp(v, min, max);
        
        /// <summary>
        /// Returns a lVector4 from component-wise application of Mix (min * (1-a) + max * a).
        /// </summary>
        public static lVector4 Mix(lVector4 min, lVector4 max, lVector4 a) => lVector4.Mix(min, max, a);
        
        /// <summary>
        /// Returns a lVector4 from component-wise application of Lerp (min * (1-a) + max * a).
        /// </summary>
        public static lVector4 Lerp(lVector4 min, lVector4 max, lVector4 a) => lVector4.Lerp(min, max, a);
        
        /// <summary>
        /// Returns a lVector4 from component-wise application of Smoothstep (((v - edge0) / (edge1 - edge0)).Clamp().HermiteInterpolationOrder3()).
        /// </summary>
        public static lVector4 Smoothstep(lVector4 edge0, lVector4 edge1, lVector4 v) => lVector4.Smoothstep(edge0, edge1, v);
        
        /// <summary>
        /// Returns a lVector4 from component-wise application of Smootherstep (((v - edge0) / (edge1 - edge0)).Clamp().HermiteInterpolationOrder5()).
        /// </summary>
        public static lVector4 Smootherstep(lVector4 edge0, lVector4 edge1, lVector4 v) => lVector4.Smootherstep(edge0, edge1, v);
        
        /// <summary>
        /// Returns a lVector4 from component-wise application of Fma (a * b + c).
        /// </summary>
        public static lVector4 Fma(lVector4 a, lVector4 b, lVector4 c) => lVector4.Fma(a, b, c);
        
        /// <summary>
        /// OuterProduct treats the first parameter c as a column Vector (Matrix with one column) and the second parameter r as a row Vector (Matrix with one row) and does a linear algebraic Matrix multiply c * r, yielding a Matrix whose number of rows is the number of components in c and whose number of columns is the number of components in r.
        /// </summary>
        public static lMatrix2x4 OuterProduct(lVector4 c, lVector2 r) => lVector4.OuterProduct(c, r);
        
        /// <summary>
        /// OuterProduct treats the first parameter c as a column Vector (Matrix with one column) and the second parameter r as a row Vector (Matrix with one row) and does a linear algebraic Matrix multiply c * r, yielding a Matrix whose number of rows is the number of components in c and whose number of columns is the number of components in r.
        /// </summary>
        public static lMatrix3x4 OuterProduct(lVector4 c, lVector3 r) => lVector4.OuterProduct(c, r);
        
        /// <summary>
        /// OuterProduct treats the first parameter c as a column Vector (Matrix with one column) and the second parameter r as a row Vector (Matrix with one row) and does a linear algebraic Matrix multiply c * r, yielding a Matrix whose number of rows is the number of components in c and whose number of columns is the number of components in r.
        /// </summary>
        public static lMatrix4 OuterProduct(lVector4 c, lVector4 r) => lVector4.OuterProduct(c, r);
        
        /// <summary>
        /// Returns a lVector4 from component-wise application of Add (lhs + rhs).
        /// </summary>
        public static lVector4 Add(lVector4 lhs, lVector4 rhs) => lVector4.Add(lhs, rhs);
        
        /// <summary>
        /// Returns a lVector4 from component-wise application of Sub (lhs - rhs).
        /// </summary>
        public static lVector4 Sub(lVector4 lhs, lVector4 rhs) => lVector4.Sub(lhs, rhs);
        
        /// <summary>
        /// Returns a lVector4 from component-wise application of Mul (lhs * rhs).
        /// </summary>
        public static lVector4 Mul(lVector4 lhs, lVector4 rhs) => lVector4.Mul(lhs, rhs);
        
        /// <summary>
        /// Returns a lVector4 from component-wise application of Div (lhs / rhs).
        /// </summary>
        public static lVector4 Div(lVector4 lhs, lVector4 rhs) => lVector4.Div(lhs, rhs);
        
        /// <summary>
        /// Returns a lVector4 from component-wise application of Xor (lhs ^ rhs).
        /// </summary>
        public static lVector4 Xor(lVector4 lhs, lVector4 rhs) => lVector4.Xor(lhs, rhs);
        
        /// <summary>
        /// Returns a lVector4 from component-wise application of BitwiseOr (lhs | rhs).
        /// </summary>
        public static lVector4 BitwiseOr(lVector4 lhs, lVector4 rhs) => lVector4.BitwiseOr(lhs, rhs);
        
        /// <summary>
        /// Returns a lVector4 from component-wise application of BitwiseAnd (lhs &amp; rhs).
        /// </summary>
        public static lVector4 BitwiseAnd(lVector4 lhs, lVector4 rhs) => lVector4.BitwiseAnd(lhs, rhs);
        
        /// <summary>
        /// Returns a lVector4 from component-wise application of LeftShift (lhs &lt;&lt; rhs).
        /// </summary>
        public static lVector4 LeftShift(lVector4 lhs, iVector4 rhs) => lVector4.LeftShift(lhs, rhs);
        
        /// <summary>
        /// Returns a lVector4 from component-wise application of RightShift (lhs &gt;&gt; rhs).
        /// </summary>
        public static lVector4 RightShift(lVector4 lhs, iVector4 rhs) => lVector4.RightShift(lhs, rhs);
        
        /// <summary>
        /// Returns the minimal component of this Vector.
        /// </summary>
        public static long MinElement(lVector4 v) => v.MinElement;
        
        /// <summary>
        /// Returns the maximal component of this Vector.
        /// </summary>
        public static long MaxElement(lVector4 v) => v.MaxElement;
        
        /// <summary>
        /// Returns the euclidean length of this Vector.
        /// </summary>
        public static double Length(lVector4 v) => v.Length;
        
        /// <summary>
        /// Returns the squared euclidean length of this Vector.
        /// </summary>
        public static double LengthSqr(lVector4 v) => v.LengthSqr;
        
        /// <summary>
        /// Returns the sum of all components.
        /// </summary>
        public static long Sum(lVector4 v) => v.Sum;
        
        /// <summary>
        /// Returns the euclidean norm of this Vector.
        /// </summary>
        public static double Norm(lVector4 v) => v.Norm;
        
        /// <summary>
        /// Returns the one-norm of this Vector.
        /// </summary>
        public static double Norm1(lVector4 v) => v.Norm1;
        
        /// <summary>
        /// Returns the two-norm (euclidean length) of this Vector.
        /// </summary>
        public static double Norm2(lVector4 v) => v.Norm2;
        
        /// <summary>
        /// Returns the max-norm of this Vector.
        /// </summary>
        public static double NormMax(lVector4 v) => v.NormMax;
        
        /// <summary>
        /// Returns the p-norm of this Vector.
        /// </summary>
        public static double NormP(lVector4 v, double p) => v.NormP(p);
        
        /// <summary>
        /// Returns the inner product (dot product, scalar product) of the two Vectors.
        /// </summary>
        public static long Dot(lVector4 lhs, lVector4 rhs) => lVector4.Dot(lhs, rhs);
        
        /// <summary>
        /// Returns the euclidean distance between the two Vectors.
        /// </summary>
        public static double Distance(lVector4 lhs, lVector4 rhs) => lVector4.Distance(lhs, rhs);
        
        /// <summary>
        /// Returns the squared euclidean distance between the two Vectors.
        /// </summary>
        public static double DistanceSqr(lVector4 lhs, lVector4 rhs) => lVector4.DistanceSqr(lhs, rhs);
        
        /// <summary>
        /// Calculate the reflection direction for an incident Vector (N should be normalized in order to achieve the desired result).
        /// </summary>
        public static lVector4 Reflect(lVector4 I, lVector4 N) => lVector4.Reflect(I, N);
        
        /// <summary>
        /// Calculate the refraction direction for an incident Vector (The input parameters I and N should be normalized in order to achieve the desired result).
        /// </summary>
        public static lVector4 Refract(lVector4 I, lVector4 N, long eta) => lVector4.Refract(I, N, eta);
        
        /// <summary>
        /// Returns a Vector pointing in the same direction as another (faceforward orients a Vector to point away from a surface as defined by its normal. If dot(Nref, I) is negative faceforward returns N, otherwise it returns -N).
        /// </summary>
        public static lVector4 FaceForward(lVector4 N, lVector4 I, lVector4 Nref) => lVector4.FaceForward(N, I, Nref);
        
        /// <summary>
        /// Returns a lVector4 with independent and identically distributed uniform integer values between 0 (inclusive) and maxValue (exclusive). (A maxValue of 0 is allowed and returns 0.)
        /// </summary>
        public static lVector4 Random(System.Random random, lVector4 maxValue) => lVector4.Random(random, maxValue);
        
        /// <summary>
        /// Returns a lVector4 with independent and identically distributed uniform integer values between minValue (inclusive) and maxValue (exclusive). (minValue == maxValue is allowed and returns minValue. Negative values are allowed.)
        /// </summary>
        public static lVector4 Random(System.Random random, lVector4 minValue, lVector4 maxValue) => lVector4.Random(random, minValue, maxValue);
        
        /// <summary>
        /// Returns a lVector4 with independent and identically distributed uniform integer values between minValue (inclusive) and maxValue (exclusive). (minValue == maxValue is allowed and returns minValue. Negative values are allowed.)
        /// </summary>
        public static lVector4 RandomUniform(System.Random random, lVector4 minValue, lVector4 maxValue) => lVector4.RandomUniform(random, minValue, maxValue);

    }
}
