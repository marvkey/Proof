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
        public static swizzle_lVector3 swizzle(lVector3 v) => v.swizzle;
        
        /// <summary>
        /// Returns an array with all values
        /// </summary>
        public static long[] Values(lVector3 v) => v.Values;
        
        /// <summary>
        /// Returns an enumerator that iterates through all components.
        /// </summary>
        public static IEnumerator<long> GetEnumerator(lVector3 v) => v.GetEnumerator();
        
        /// <summary>
        /// Returns a string representation of this Vector using ', ' as a seperator.
        /// </summary>
        public static string ToString(lVector3 v) => v.ToString();
        
        /// <summary>
        /// Returns a string representation of this Vector using a provided seperator.
        /// </summary>
        public static string ToString(lVector3 v, string sep) => v.ToString(sep);
        
        /// <summary>
        /// Returns a string representation of this Vector using a provided seperator and a forMatrix provider for each component.
        /// </summary>
        public static string ToString(lVector3 v, string sep, IFormatProvider provider) => v.ToString(sep, provider);
        
        /// <summary>
        /// Returns a string representation of this Vector using a provided seperator and a forMatrix for each component.
        /// </summary>
        public static string ToString(lVector3 v, string sep, string forMatrix) => v.ToString(sep, forMatrix);
        
        /// <summary>
        /// Returns a string representation of this Vector using a provided seperator and a forMatrix and forMatrix provider for each component.
        /// </summary>
        public static string ToString(lVector3 v, string sep, string forMatrix, IFormatProvider provider) => v.ToString(sep, forMatrix, provider);
        
        /// <summary>
        /// Returns the number of components (3).
        /// </summary>
        public static int Count(lVector3 v) => v.Count;
        
        /// <summary>
        /// Returns true iff this equals rhs component-wise.
        /// </summary>
        public static bool Equals(lVector3 v, lVector3 rhs) => v.Equals(rhs);
        
        /// <summary>
        /// Returns true iff this equals rhs type- and component-wise.
        /// </summary>
        public static bool Equals(lVector3 v, object obj) => v.Equals(obj);
        
        /// <summary>
        /// Returns a hash code for this instance.
        /// </summary>
        public static int GetHashCode(lVector3 v) => v.GetHashCode();
        
        /// <summary>
        /// Returns a bVector3 from component-wise application of Equal (lhs == rhs).
        /// </summary>
        public static bVector3 Equal(lVector3 lhs, lVector3 rhs) => lVector3.Equal(lhs, rhs);
        
        /// <summary>
        /// Returns a bVector3 from component-wise application of Equal (lhs == rhs).
        /// </summary>
        public static bool Equal(long lhs, long rhs) => lhs == rhs;
        
        /// <summary>
        /// Returns a bVector3 from component-wise application of NotEqual (lhs != rhs).
        /// </summary>
        public static bVector3 NotEqual(lVector3 lhs, lVector3 rhs) => lVector3.NotEqual(lhs, rhs);
        
        /// <summary>
        /// Returns a bVector3 from component-wise application of NotEqual (lhs != rhs).
        /// </summary>
        public static bool NotEqual(long lhs, long rhs) => lhs != rhs;
        
        /// <summary>
        /// Returns a bVector3 from component-wise application of GreaterThan (lhs &gt; rhs).
        /// </summary>
        public static bVector3 GreaterThan(lVector3 lhs, lVector3 rhs) => lVector3.GreaterThan(lhs, rhs);
        
        /// <summary>
        /// Returns a bVector3 from component-wise application of GreaterThan (lhs &gt; rhs).
        /// </summary>
        public static bool GreaterThan(long lhs, long rhs) => lhs > rhs;
        
        /// <summary>
        /// Returns a bVector3 from component-wise application of GreaterThanEqual (lhs &gt;= rhs).
        /// </summary>
        public static bVector3 GreaterThanEqual(lVector3 lhs, lVector3 rhs) => lVector3.GreaterThanEqual(lhs, rhs);
        
        /// <summary>
        /// Returns a bVector3 from component-wise application of GreaterThanEqual (lhs &gt;= rhs).
        /// </summary>
        public static bool GreaterThanEqual(long lhs, long rhs) => lhs >= rhs;
        
        /// <summary>
        /// Returns a bVector3 from component-wise application of LesserThan (lhs &lt; rhs).
        /// </summary>
        public static bVector3 LesserThan(lVector3 lhs, lVector3 rhs) => lVector3.LesserThan(lhs, rhs);
        
        /// <summary>
        /// Returns a bVector3 from component-wise application of LesserThan (lhs &lt; rhs).
        /// </summary>
        public static bool LesserThan(long lhs, long rhs) => lhs < rhs;
        
        /// <summary>
        /// Returns a bVector3 from component-wise application of LesserThanEqual (lhs &lt;= rhs).
        /// </summary>
        public static bVector3 LesserThanEqual(lVector3 lhs, lVector3 rhs) => lVector3.LesserThanEqual(lhs, rhs);
        
        /// <summary>
        /// Returns a bVector3 from component-wise application of LesserThanEqual (lhs &lt;= rhs).
        /// </summary>
        public static bool LesserThanEqual(long lhs, long rhs) => lhs <= rhs;
        
        /// <summary>
        /// Returns a lVector3 from component-wise application of Abs (Math.Abs(v)).
        /// </summary>
        public static lVector3 Abs(lVector3 v) => lVector3.Abs(v);
        
        /// <summary>
        /// Returns a lVector3 from component-wise application of Abs (Math.Abs(v)).
        /// </summary>
        public static long Abs(long v) => Math.Abs(v);
        
        /// <summary>
        /// Returns a lVector3 from component-wise application of HermiteInterpolationOrder3 ((3 - 2 * v) * v * v).
        /// </summary>
        public static lVector3 HermiteInterpolationOrder3(lVector3 v) => lVector3.HermiteInterpolationOrder3(v);
        
        /// <summary>
        /// Returns a lVector3 from component-wise application of HermiteInterpolationOrder3 ((3 - 2 * v) * v * v).
        /// </summary>
        public static long HermiteInterpolationOrder3(long v) => (3 - 2 * v) * v * v;
        
        /// <summary>
        /// Returns a lVector3 from component-wise application of HermiteInterpolationOrder5 (((6 * v - 15) * v + 10) * v * v * v).
        /// </summary>
        public static lVector3 HermiteInterpolationOrder5(lVector3 v) => lVector3.HermiteInterpolationOrder5(v);
        
        /// <summary>
        /// Returns a lVector3 from component-wise application of HermiteInterpolationOrder5 (((6 * v - 15) * v + 10) * v * v * v).
        /// </summary>
        public static long HermiteInterpolationOrder5(long v) => ((6 * v - 15) * v + 10) * v * v * v;
        
        /// <summary>
        /// Returns a lVector3 from component-wise application of Sqr (v * v).
        /// </summary>
        public static lVector3 Sqr(lVector3 v) => lVector3.Sqr(v);
        
        /// <summary>
        /// Returns a lVector3 from component-wise application of Sqr (v * v).
        /// </summary>
        public static long Sqr(long v) => v * v;
        
        /// <summary>
        /// Returns a lVector3 from component-wise application of Pow2 (v * v).
        /// </summary>
        public static lVector3 Pow2(lVector3 v) => lVector3.Pow2(v);
        
        /// <summary>
        /// Returns a lVector3 from component-wise application of Pow2 (v * v).
        /// </summary>
        public static long Pow2(long v) => v * v;
        
        /// <summary>
        /// Returns a lVector3 from component-wise application of Pow3 (v * v * v).
        /// </summary>
        public static lVector3 Pow3(lVector3 v) => lVector3.Pow3(v);
        
        /// <summary>
        /// Returns a lVector3 from component-wise application of Pow3 (v * v * v).
        /// </summary>
        public static long Pow3(long v) => v * v * v;
        
        /// <summary>
        /// Returns a lVector3 from component-wise application of Step (v &gt;= 0 ? 1 : 0).
        /// </summary>
        public static lVector3 Step(lVector3 v) => lVector3.Step(v);
        
        /// <summary>
        /// Returns a lVector3 from component-wise application of Step (v &gt;= 0 ? 1 : 0).
        /// </summary>
        public static long Step(long v) => v >= 0 ? 1 : 0;
        
        /// <summary>
        /// Returns a lVector3 from component-wise application of Sqrt ((long)Math.Sqrt((double)v)).
        /// </summary>
        public static lVector3 Sqrt(lVector3 v) => lVector3.Sqrt(v);
        
        /// <summary>
        /// Returns a lVector3 from component-wise application of Sqrt ((long)Math.Sqrt((double)v)).
        /// </summary>
        public static long Sqrt(long v) => (long)Math.Sqrt((double)v);
        
        /// <summary>
        /// Returns a lVector3 from component-wise application of InverseSqrt ((long)(1.0 / Math.Sqrt((double)v))).
        /// </summary>
        public static lVector3 InverseSqrt(lVector3 v) => lVector3.InverseSqrt(v);
        
        /// <summary>
        /// Returns a lVector3 from component-wise application of InverseSqrt ((long)(1.0 / Math.Sqrt((double)v))).
        /// </summary>
        public static long InverseSqrt(long v) => (long)(1.0 / Math.Sqrt((double)v));
        
        /// <summary>
        /// Returns a iVector3 from component-wise application of Sign (Math.Sign(v)).
        /// </summary>
        public static iVector3 Sign(lVector3 v) => lVector3.Sign(v);
        
        /// <summary>
        /// Returns a iVector3 from component-wise application of Sign (Math.Sign(v)).
        /// </summary>
        public static int Sign(long v) => Math.Sign(v);
        
        /// <summary>
        /// Returns a lVector3 from component-wise application of Max (Math.Max(lhs, rhs)).
        /// </summary>
        public static lVector3 Max(lVector3 lhs, lVector3 rhs) => lVector3.Max(lhs, rhs);
        
        /// <summary>
        /// Returns a lVector3 from component-wise application of Max (Math.Max(lhs, rhs)).
        /// </summary>
        public static long Max(long lhs, long rhs) => Math.Max(lhs, rhs);
        
        /// <summary>
        /// Returns a lVector3 from component-wise application of Min (Math.Min(lhs, rhs)).
        /// </summary>
        public static lVector3 Min(lVector3 lhs, lVector3 rhs) => lVector3.Min(lhs, rhs);
        
        /// <summary>
        /// Returns a lVector3 from component-wise application of Min (Math.Min(lhs, rhs)).
        /// </summary>
        public static long Min(long lhs, long rhs) => Math.Min(lhs, rhs);
        
        /// <summary>
        /// Returns a lVector3 from component-wise application of Pow ((long)Math.Pow((double)lhs, (double)rhs)).
        /// </summary>
        public static lVector3 Pow(lVector3 lhs, lVector3 rhs) => lVector3.Pow(lhs, rhs);
        
        /// <summary>
        /// Returns a lVector3 from component-wise application of Pow ((long)Math.Pow((double)lhs, (double)rhs)).
        /// </summary>
        public static long Pow(long lhs, long rhs) => (long)Math.Pow((double)lhs, (double)rhs);
        
        /// <summary>
        /// Returns a lVector3 from component-wise application of Log ((long)Math.Log((double)lhs, (double)rhs)).
        /// </summary>
        public static lVector3 Log(lVector3 lhs, lVector3 rhs) => lVector3.Log(lhs, rhs);
        
        /// <summary>
        /// Returns a lVector3 from component-wise application of Log ((long)Math.Log((double)lhs, (double)rhs)).
        /// </summary>
        public static long Log(long lhs, long rhs) => (long)Math.Log((double)lhs, (double)rhs);
        
        /// <summary>
        /// Returns a lVector3 from component-wise application of Clamp (Math.Min(Math.Max(v, min), max)).
        /// </summary>
        public static lVector3 Clamp(lVector3 v, lVector3 min, lVector3 max) => lVector3.Clamp(v, min, max);
        
        /// <summary>
        /// Returns a lVector3 from component-wise application of Clamp (Math.Min(Math.Max(v, min), max)).
        /// </summary>
        public static long Clamp(long v, long min, long max) => Math.Min(Math.Max(v, min), max);
        
        /// <summary>
        /// Returns a lVector3 from component-wise application of Mix (min * (1-a) + max * a).
        /// </summary>
        public static lVector3 Mix(lVector3 min, lVector3 max, lVector3 a) => lVector3.Mix(min, max, a);
        
        /// <summary>
        /// Returns a lVector3 from component-wise application of Mix (min * (1-a) + max * a).
        /// </summary>
        public static long Mix(long min, long max, long a) => min * (1-a) + max * a;
        
        /// <summary>
        /// Returns a lVector3 from component-wise application of Lerp (min * (1-a) + max * a).
        /// </summary>
        public static lVector3 Lerp(lVector3 min, lVector3 max, lVector3 a) => lVector3.Lerp(min, max, a);
        
        /// <summary>
        /// Returns a lVector3 from component-wise application of Lerp (min * (1-a) + max * a).
        /// </summary>
        public static long Lerp(long min, long max, long a) => min * (1-a) + max * a;
        
        /// <summary>
        /// Returns a lVector3 from component-wise application of Smoothstep (((v - edge0) / (edge1 - edge0)).Clamp().HermiteInterpolationOrder3()).
        /// </summary>
        public static lVector3 Smoothstep(lVector3 edge0, lVector3 edge1, lVector3 v) => lVector3.Smoothstep(edge0, edge1, v);
        
        /// <summary>
        /// Returns a lVector3 from component-wise application of Smoothstep (((v - edge0) / (edge1 - edge0)).Clamp().HermiteInterpolationOrder3()).
        /// </summary>
        public static long Smoothstep(long edge0, long edge1, long v) => ((v - edge0) / (edge1 - edge0)).Clamp().HermiteInterpolationOrder3();
        
        /// <summary>
        /// Returns a lVector3 from component-wise application of Smootherstep (((v - edge0) / (edge1 - edge0)).Clamp().HermiteInterpolationOrder5()).
        /// </summary>
        public static lVector3 Smootherstep(lVector3 edge0, lVector3 edge1, lVector3 v) => lVector3.Smootherstep(edge0, edge1, v);
        
        /// <summary>
        /// Returns a lVector3 from component-wise application of Smootherstep (((v - edge0) / (edge1 - edge0)).Clamp().HermiteInterpolationOrder5()).
        /// </summary>
        public static long Smootherstep(long edge0, long edge1, long v) => ((v - edge0) / (edge1 - edge0)).Clamp().HermiteInterpolationOrder5();
        
        /// <summary>
        /// Returns a lVector3 from component-wise application of Fma (a * b + c).
        /// </summary>
        public static lVector3 Fma(lVector3 a, lVector3 b, lVector3 c) => lVector3.Fma(a, b, c);
        
        /// <summary>
        /// Returns a lVector3 from component-wise application of Fma (a * b + c).
        /// </summary>
        public static long Fma(long a, long b, long c) => a * b + c;
        
        /// <summary>
        /// OuterProduct treats the first parameter c as a column Vector (Matrix with one column) and the second parameter r as a row Vector (Matrix with one row) and does a linear algebraic Matrix multiply c * r, yielding a Matrix whose number of rows is the number of components in c and whose number of columns is the number of components in r.
        /// </summary>
        public static lMatrix2x3 OuterProduct(lVector3 c, lVector2 r) => lVector3.OuterProduct(c, r);
        
        /// <summary>
        /// OuterProduct treats the first parameter c as a column Vector (Matrix with one column) and the second parameter r as a row Vector (Matrix with one row) and does a linear algebraic Matrix multiply c * r, yielding a Matrix whose number of rows is the number of components in c and whose number of columns is the number of components in r.
        /// </summary>
        public static lMatrix3 OuterProduct(lVector3 c, lVector3 r) => lVector3.OuterProduct(c, r);
        
        /// <summary>
        /// OuterProduct treats the first parameter c as a column Vector (Matrix with one column) and the second parameter r as a row Vector (Matrix with one row) and does a linear algebraic Matrix multiply c * r, yielding a Matrix whose number of rows is the number of components in c and whose number of columns is the number of components in r.
        /// </summary>
        public static lMatrix4x3 OuterProduct(lVector3 c, lVector4 r) => lVector3.OuterProduct(c, r);
        
        /// <summary>
        /// Returns a lVector3 from component-wise application of Add (lhs + rhs).
        /// </summary>
        public static lVector3 Add(lVector3 lhs, lVector3 rhs) => lVector3.Add(lhs, rhs);
        
        /// <summary>
        /// Returns a lVector3 from component-wise application of Add (lhs + rhs).
        /// </summary>
        public static long Add(long lhs, long rhs) => lhs + rhs;
        
        /// <summary>
        /// Returns a lVector3 from component-wise application of Sub (lhs - rhs).
        /// </summary>
        public static lVector3 Sub(lVector3 lhs, lVector3 rhs) => lVector3.Sub(lhs, rhs);
        
        /// <summary>
        /// Returns a lVector3 from component-wise application of Sub (lhs - rhs).
        /// </summary>
        public static long Sub(long lhs, long rhs) => lhs - rhs;
        
        /// <summary>
        /// Returns a lVector3 from component-wise application of Mul (lhs * rhs).
        /// </summary>
        public static lVector3 Mul(lVector3 lhs, lVector3 rhs) => lVector3.Mul(lhs, rhs);
        
        /// <summary>
        /// Returns a lVector3 from component-wise application of Mul (lhs * rhs).
        /// </summary>
        public static long Mul(long lhs, long rhs) => lhs * rhs;
        
        /// <summary>
        /// Returns a lVector3 from component-wise application of Div (lhs / rhs).
        /// </summary>
        public static lVector3 Div(lVector3 lhs, lVector3 rhs) => lVector3.Div(lhs, rhs);
        
        /// <summary>
        /// Returns a lVector3 from component-wise application of Div (lhs / rhs).
        /// </summary>
        public static long Div(long lhs, long rhs) => lhs / rhs;
        
        /// <summary>
        /// Returns a lVector3 from component-wise application of Xor (lhs ^ rhs).
        /// </summary>
        public static lVector3 Xor(lVector3 lhs, lVector3 rhs) => lVector3.Xor(lhs, rhs);
        
        /// <summary>
        /// Returns a lVector3 from component-wise application of Xor (lhs ^ rhs).
        /// </summary>
        public static long Xor(long lhs, long rhs) => lhs ^ rhs;
        
        /// <summary>
        /// Returns a lVector3 from component-wise application of BitwiseOr (lhs | rhs).
        /// </summary>
        public static lVector3 BitwiseOr(lVector3 lhs, lVector3 rhs) => lVector3.BitwiseOr(lhs, rhs);
        
        /// <summary>
        /// Returns a lVector3 from component-wise application of BitwiseOr (lhs | rhs).
        /// </summary>
        public static long BitwiseOr(long lhs, long rhs) => lhs | rhs;
        
        /// <summary>
        /// Returns a lVector3 from component-wise application of BitwiseAnd (lhs &amp; rhs).
        /// </summary>
        public static lVector3 BitwiseAnd(lVector3 lhs, lVector3 rhs) => lVector3.BitwiseAnd(lhs, rhs);
        
        /// <summary>
        /// Returns a lVector3 from component-wise application of BitwiseAnd (lhs &amp; rhs).
        /// </summary>
        public static long BitwiseAnd(long lhs, long rhs) => lhs & rhs;
        
        /// <summary>
        /// Returns a lVector3 from component-wise application of LeftShift (lhs &lt;&lt; rhs).
        /// </summary>
        public static lVector3 LeftShift(lVector3 lhs, iVector3 rhs) => lVector3.LeftShift(lhs, rhs);
        
        /// <summary>
        /// Returns a lVector3 from component-wise application of LeftShift (lhs &lt;&lt; rhs).
        /// </summary>
        public static long LeftShift(long lhs, int rhs) => lhs << rhs;
        
        /// <summary>
        /// Returns a lVector3 from component-wise application of RightShift (lhs &gt;&gt; rhs).
        /// </summary>
        public static lVector3 RightShift(lVector3 lhs, iVector3 rhs) => lVector3.RightShift(lhs, rhs);
        
        /// <summary>
        /// Returns a lVector3 from component-wise application of RightShift (lhs &gt;&gt; rhs).
        /// </summary>
        public static long RightShift(long lhs, int rhs) => lhs >> rhs;
        
        /// <summary>
        /// Returns the minimal component of this Vector.
        /// </summary>
        public static long MinElement(lVector3 v) => v.MinElement;
        
        /// <summary>
        /// Returns the maximal component of this Vector.
        /// </summary>
        public static long MaxElement(lVector3 v) => v.MaxElement;
        
        /// <summary>
        /// Returns the euclidean length of this Vector.
        /// </summary>
        public static double Length(lVector3 v) => v.Length;
        
        /// <summary>
        /// Returns the squared euclidean length of this Vector.
        /// </summary>
        public static double LengthSqr(lVector3 v) => v.LengthSqr;
        
        /// <summary>
        /// Returns the sum of all components.
        /// </summary>
        public static long Sum(lVector3 v) => v.Sum;
        
        /// <summary>
        /// Returns the euclidean norm of this Vector.
        /// </summary>
        public static double Norm(lVector3 v) => v.Norm;
        
        /// <summary>
        /// Returns the one-norm of this Vector.
        /// </summary>
        public static double Norm1(lVector3 v) => v.Norm1;
        
        /// <summary>
        /// Returns the two-norm (euclidean length) of this Vector.
        /// </summary>
        public static double Norm2(lVector3 v) => v.Norm2;
        
        /// <summary>
        /// Returns the max-norm of this Vector.
        /// </summary>
        public static double NormMax(lVector3 v) => v.NormMax;
        
        /// <summary>
        /// Returns the p-norm of this Vector.
        /// </summary>
        public static double NormP(lVector3 v, double p) => v.NormP(p);
        
        /// <summary>
        /// Returns the inner product (dot product, scalar product) of the two Vectors.
        /// </summary>
        public static long Dot(lVector3 lhs, lVector3 rhs) => lVector3.Dot(lhs, rhs);
        
        /// <summary>
        /// Returns the euclidean distance between the two Vectors.
        /// </summary>
        public static double Distance(lVector3 lhs, lVector3 rhs) => lVector3.Distance(lhs, rhs);
        
        /// <summary>
        /// Returns the squared euclidean distance between the two Vectors.
        /// </summary>
        public static double DistanceSqr(lVector3 lhs, lVector3 rhs) => lVector3.DistanceSqr(lhs, rhs);
        
        /// <summary>
        /// Calculate the reflection direction for an incident Vector (N should be normalized in order to achieve the desired result).
        /// </summary>
        public static lVector3 Reflect(lVector3 I, lVector3 N) => lVector3.Reflect(I, N);
        
        /// <summary>
        /// Calculate the refraction direction for an incident Vector (The input parameters I and N should be normalized in order to achieve the desired result).
        /// </summary>
        public static lVector3 Refract(lVector3 I, lVector3 N, long eta) => lVector3.Refract(I, N, eta);
        
        /// <summary>
        /// Returns a Vector pointing in the same direction as another (faceforward orients a Vector to point away from a surface as defined by its normal. If dot(Nref, I) is negative faceforward returns N, otherwise it returns -N).
        /// </summary>
        public static lVector3 FaceForward(lVector3 N, lVector3 I, lVector3 Nref) => lVector3.FaceForward(N, I, Nref);
        
        /// <summary>
        /// Returns the outer product (cross product, Vector product) of the two Vectors.
        /// </summary>
        public static lVector3 Cross(lVector3 l, lVector3 r) => lVector3.Cross(l, r);
        
        /// <summary>
        /// Returns a lVector3 with independent and identically distributed uniform integer values between 0 (inclusive) and maxValue (exclusive). (A maxValue of 0 is allowed and returns 0.)
        /// </summary>
        public static lVector3 Random(System.Random random, lVector3 maxValue) => lVector3.Random(random, maxValue);
        
        /// <summary>
        /// Returns a lVector3 with independent and identically distributed uniform integer values between 0 (inclusive) and maxValue (exclusive). (A maxValue of 0 is allowed and returns 0.)
        /// </summary>
        public static long Random(System.Random random, long maxValue) => (long)random.Next((int)maxValue);
        
        /// <summary>
        /// Returns a lVector3 with independent and identically distributed uniform integer values between minValue (inclusive) and maxValue (exclusive). (minValue == maxValue is allowed and returns minValue. Negative values are allowed.)
        /// </summary>
        public static lVector3 Random(System.Random random, lVector3 minValue, lVector3 maxValue) => lVector3.Random(random, minValue, maxValue);
        
        /// <summary>
        /// Returns a lVector3 with independent and identically distributed uniform integer values between minValue (inclusive) and maxValue (exclusive). (minValue == maxValue is allowed and returns minValue. Negative values are allowed.)
        /// </summary>
        public static long Random(System.Random random, long minValue, long maxValue) => (long)random.Next((int)minValue, (int)maxValue);
        
        /// <summary>
        /// Returns a lVector3 with independent and identically distributed uniform integer values between minValue (inclusive) and maxValue (exclusive). (minValue == maxValue is allowed and returns minValue. Negative values are allowed.)
        /// </summary>
        public static lVector3 RandomUniform(System.Random random, lVector3 minValue, lVector3 maxValue) => lVector3.RandomUniform(random, minValue, maxValue);
        
        /// <summary>
        /// Returns a lVector3 with independent and identically distributed uniform integer values between minValue (inclusive) and maxValue (exclusive). (minValue == maxValue is allowed and returns minValue. Negative values are allowed.)
        /// </summary>
        public static long RandomUniform(System.Random random, long minValue, long maxValue) => (long)random.Next((int)minValue, (int)maxValue);

    }
}
