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
        public static swizzle_lVector2 swizzle(lVector2 v) => v.swizzle;
        
        /// <summary>
        /// Returns an array with all values
        /// </summary>
        public static long[] Values(lVector2 v) => v.Values;
        
        /// <summary>
        /// Returns an enumerator that iterates through all components.
        /// </summary>
        public static IEnumerator<long> GetEnumerator(lVector2 v) => v.GetEnumerator();
        
        /// <summary>
        /// Returns a string representation of this Vectortor using ', ' as a seperator.
        /// </summary>
        public static string ToString(lVector2 v) => v.ToString();
        
        /// <summary>
        /// Returns a string representation of this Vectortor using a provided seperator.
        /// </summary>
        public static string ToString(lVector2 v, string sep) => v.ToString(sep);
        
        /// <summary>
        /// Returns a string representation of this Vectortor using a provided seperator and a forMatrix provider for each component.
        /// </summary>
        public static string ToString(lVector2 v, string sep, IFormatProvider provider) => v.ToString(sep, provider);
        
        /// <summary>
        /// Returns a string representation of this Vectortor using a provided seperator and a forMatrix for each component.
        /// </summary>
        public static string ToString(lVector2 v, string sep, string forMatrix) => v.ToString(sep, forMatrix);
        
        /// <summary>
        /// Returns a string representation of this Vectortor using a provided seperator and a forMatrix and forMatrix provider for each component.
        /// </summary>
        public static string ToString(lVector2 v, string sep, string forMatrix, IFormatProvider provider) => v.ToString(sep, forMatrix, provider);
        
        /// <summary>
        /// Returns the number of components (2).
        /// </summary>
        public static int Count(lVector2 v) => v.Count;
        
        /// <summary>
        /// Returns true iff this equals rhs component-wise.
        /// </summary>
        public static bool Equals(lVector2 v, lVector2 rhs) => v.Equals(rhs);
        
        /// <summary>
        /// Returns true iff this equals rhs type- and component-wise.
        /// </summary>
        public static bool Equals(lVector2 v, object obj) => v.Equals(obj);
        
        /// <summary>
        /// Returns a hash code for this instance.
        /// </summary>
        public static int GetHashCode(lVector2 v) => v.GetHashCode();
        
        /// <summary>
        /// Returns a bVector2 from component-wise application of Equal (lhs == rhs).
        /// </summary>
        public static bVector2 Equal(lVector2 lhs, lVector2 rhs) => lVector2.Equal(lhs, rhs);
        
        /// <summary>
        /// Returns a bVector2 from component-wise application of NotEqual (lhs != rhs).
        /// </summary>
        public static bVector2 NotEqual(lVector2 lhs, lVector2 rhs) => lVector2.NotEqual(lhs, rhs);
        
        /// <summary>
        /// Returns a bVector2 from component-wise application of GreaterThan (lhs &gt; rhs).
        /// </summary>
        public static bVector2 GreaterThan(lVector2 lhs, lVector2 rhs) => lVector2.GreaterThan(lhs, rhs);
        
        /// <summary>
        /// Returns a bVector2 from component-wise application of GreaterThanEqual (lhs &gt;= rhs).
        /// </summary>
        public static bVector2 GreaterThanEqual(lVector2 lhs, lVector2 rhs) => lVector2.GreaterThanEqual(lhs, rhs);
        
        /// <summary>
        /// Returns a bVector2 from component-wise application of LesserThan (lhs &lt; rhs).
        /// </summary>
        public static bVector2 LesserThan(lVector2 lhs, lVector2 rhs) => lVector2.LesserThan(lhs, rhs);
        
        /// <summary>
        /// Returns a bVector2 from component-wise application of LesserThanEqual (lhs &lt;= rhs).
        /// </summary>
        public static bVector2 LesserThanEqual(lVector2 lhs, lVector2 rhs) => lVector2.LesserThanEqual(lhs, rhs);
        
        /// <summary>
        /// Returns a lVector2 from component-wise application of Abs (Math.Abs(v)).
        /// </summary>
        public static lVector2 Abs(lVector2 v) => lVector2.Abs(v);
        
        /// <summary>
        /// Returns a lVector2 from component-wise application of HermiteInterpolationOrder3 ((3 - 2 * v) * v * v).
        /// </summary>
        public static lVector2 HermiteInterpolationOrder3(lVector2 v) => lVector2.HermiteInterpolationOrder3(v);
        
        /// <summary>
        /// Returns a lVector2 from component-wise application of HermiteInterpolationOrder5 (((6 * v - 15) * v + 10) * v * v * v).
        /// </summary>
        public static lVector2 HermiteInterpolationOrder5(lVector2 v) => lVector2.HermiteInterpolationOrder5(v);
        
        /// <summary>
        /// Returns a lVector2 from component-wise application of Sqr (v * v).
        /// </summary>
        public static lVector2 Sqr(lVector2 v) => lVector2.Sqr(v);
        
        /// <summary>
        /// Returns a lVector2 from component-wise application of Pow2 (v * v).
        /// </summary>
        public static lVector2 Pow2(lVector2 v) => lVector2.Pow2(v);
        
        /// <summary>
        /// Returns a lVector2 from component-wise application of Pow3 (v * v * v).
        /// </summary>
        public static lVector2 Pow3(lVector2 v) => lVector2.Pow3(v);
        
        /// <summary>
        /// Returns a lVector2 from component-wise application of Step (v &gt;= 0 ? 1 : 0).
        /// </summary>
        public static lVector2 Step(lVector2 v) => lVector2.Step(v);
        
        /// <summary>
        /// Returns a lVector2 from component-wise application of Sqrt ((long)Math.Sqrt((double)v)).
        /// </summary>
        public static lVector2 Sqrt(lVector2 v) => lVector2.Sqrt(v);
        
        /// <summary>
        /// Returns a lVector2 from component-wise application of InverseSqrt ((long)(1.0 / Math.Sqrt((double)v))).
        /// </summary>
        public static lVector2 InverseSqrt(lVector2 v) => lVector2.InverseSqrt(v);
        
        /// <summary>
        /// Returns a iVector2 from component-wise application of Sign (Math.Sign(v)).
        /// </summary>
        public static iVector2 Sign(lVector2 v) => lVector2.Sign(v);
        
        /// <summary>
        /// Returns a lVector2 from component-wise application of Max (Math.Max(lhs, rhs)).
        /// </summary>
        public static lVector2 Max(lVector2 lhs, lVector2 rhs) => lVector2.Max(lhs, rhs);
        
        /// <summary>
        /// Returns a lVector2 from component-wise application of Min (Math.Min(lhs, rhs)).
        /// </summary>
        public static lVector2 Min(lVector2 lhs, lVector2 rhs) => lVector2.Min(lhs, rhs);
        
        /// <summary>
        /// Returns a lVector2 from component-wise application of Pow ((long)Math.Pow((double)lhs, (double)rhs)).
        /// </summary>
        public static lVector2 Pow(lVector2 lhs, lVector2 rhs) => lVector2.Pow(lhs, rhs);
        
        /// <summary>
        /// Returns a lVector2 from component-wise application of Log ((long)Math.Log((double)lhs, (double)rhs)).
        /// </summary>
        public static lVector2 Log(lVector2 lhs, lVector2 rhs) => lVector2.Log(lhs, rhs);
        
        /// <summary>
        /// Returns a lVector2 from component-wise application of Clamp (Math.Min(Math.Max(v, min), max)).
        /// </summary>
        public static lVector2 Clamp(lVector2 v, lVector2 min, lVector2 max) => lVector2.Clamp(v, min, max);
        
        /// <summary>
        /// Returns a lVector2 from component-wise application of Mix (min * (1-a) + max * a).
        /// </summary>
        public static lVector2 Mix(lVector2 min, lVector2 max, lVector2 a) => lVector2.Mix(min, max, a);
        
        /// <summary>
        /// Returns a lVector2 from component-wise application of Lerp (min * (1-a) + max * a).
        /// </summary>
        public static lVector2 Lerp(lVector2 min, lVector2 max, lVector2 a) => lVector2.Lerp(min, max, a);
        
        /// <summary>
        /// Returns a lVector2 from component-wise application of Smoothstep (((v - edge0) / (edge1 - edge0)).Clamp().HermiteInterpolationOrder3()).
        /// </summary>
        public static lVector2 Smoothstep(lVector2 edge0, lVector2 edge1, lVector2 v) => lVector2.Smoothstep(edge0, edge1, v);
        
        /// <summary>
        /// Returns a lVector2 from component-wise application of Smootherstep (((v - edge0) / (edge1 - edge0)).Clamp().HermiteInterpolationOrder5()).
        /// </summary>
        public static lVector2 Smootherstep(lVector2 edge0, lVector2 edge1, lVector2 v) => lVector2.Smootherstep(edge0, edge1, v);
        
        /// <summary>
        /// Returns a lVector2 from component-wise application of Fma (a * b + c).
        /// </summary>
        public static lVector2 Fma(lVector2 a, lVector2 b, lVector2 c) => lVector2.Fma(a, b, c);
        
        /// <summary>
        /// OuterProduct treats the first parameter c as a column Vectortor (Matrix with one column) and the second parameter r as a row Vectortor (Matrix with one row) and does a linear algebraic Matrix multiply c * r, yielding a Matrix whose number of rows is the number of components in c and whose number of columns is the number of components in r.
        /// </summary>
        public static lMatrix2 OuterProduct(lVector2 c, lVector2 r) => lVector2.OuterProduct(c, r);
        
        /// <summary>
        /// OuterProduct treats the first parameter c as a column Vectortor (Matrix with one column) and the second parameter r as a row Vectortor (Matrix with one row) and does a linear algebraic Matrix multiply c * r, yielding a Matrix whose number of rows is the number of components in c and whose number of columns is the number of components in r.
        /// </summary>
        public static lMatrix3x2 OuterProduct(lVector2 c, lVector3 r) => lVector2.OuterProduct(c, r);
        
        /// <summary>
        /// OuterProduct treats the first parameter c as a column Vectortor (Matrix with one column) and the second parameter r as a row Vectortor (Matrix with one row) and does a linear algebraic Matrix multiply c * r, yielding a Matrix whose number of rows is the number of components in c and whose number of columns is the number of components in r.
        /// </summary>
        public static lMatrix4x2 OuterProduct(lVector2 c, lVector4 r) => lVector2.OuterProduct(c, r);
        
        /// <summary>
        /// Returns a lVector2 from component-wise application of Add (lhs + rhs).
        /// </summary>
        public static lVector2 Add(lVector2 lhs, lVector2 rhs) => lVector2.Add(lhs, rhs);
        
        /// <summary>
        /// Returns a lVector2 from component-wise application of Sub (lhs - rhs).
        /// </summary>
        public static lVector2 Sub(lVector2 lhs, lVector2 rhs) => lVector2.Sub(lhs, rhs);
        
        /// <summary>
        /// Returns a lVector2 from component-wise application of Mul (lhs * rhs).
        /// </summary>
        public static lVector2 Mul(lVector2 lhs, lVector2 rhs) => lVector2.Mul(lhs, rhs);
        
        /// <summary>
        /// Returns a lVector2 from component-wise application of Div (lhs / rhs).
        /// </summary>
        public static lVector2 Div(lVector2 lhs, lVector2 rhs) => lVector2.Div(lhs, rhs);
        
        /// <summary>
        /// Returns a lVector2 from component-wise application of Xor (lhs ^ rhs).
        /// </summary>
        public static lVector2 Xor(lVector2 lhs, lVector2 rhs) => lVector2.Xor(lhs, rhs);
        
        /// <summary>
        /// Returns a lVector2 from component-wise application of BitwiseOr (lhs | rhs).
        /// </summary>
        public static lVector2 BitwiseOr(lVector2 lhs, lVector2 rhs) => lVector2.BitwiseOr(lhs, rhs);
        
        /// <summary>
        /// Returns a lVector2 from component-wise application of BitwiseAnd (lhs &amp; rhs).
        /// </summary>
        public static lVector2 BitwiseAnd(lVector2 lhs, lVector2 rhs) => lVector2.BitwiseAnd(lhs, rhs);
        
        /// <summary>
        /// Returns a lVector2 from component-wise application of LeftShift (lhs &lt;&lt; rhs).
        /// </summary>
        public static lVector2 LeftShift(lVector2 lhs, iVector2 rhs) => lVector2.LeftShift(lhs, rhs);
        
        /// <summary>
        /// Returns a lVector2 from component-wise application of RightShift (lhs &gt;&gt; rhs).
        /// </summary>
        public static lVector2 RightShift(lVector2 lhs, iVector2 rhs) => lVector2.RightShift(lhs, rhs);
        
        /// <summary>
        /// Returns the minimal component of this Vectortor.
        /// </summary>
        public static long MinElement(lVector2 v) => v.MinElement;
        
        /// <summary>
        /// Returns the maximal component of this Vectortor.
        /// </summary>
        public static long MaxElement(lVector2 v) => v.MaxElement;
        
        /// <summary>
        /// Returns the euclidean length of this Vectortor.
        /// </summary>
        public static double Length(lVector2 v) => v.Length;
        
        /// <summary>
        /// Returns the squared euclidean length of this Vectortor.
        /// </summary>
        public static double LengthSqr(lVector2 v) => v.LengthSqr;
        
        /// <summary>
        /// Returns the sum of all components.
        /// </summary>
        public static long Sum(lVector2 v) => v.Sum;
        
        /// <summary>
        /// Returns the euclidean norm of this Vectortor.
        /// </summary>
        public static double Norm(lVector2 v) => v.Norm;
        
        /// <summary>
        /// Returns the one-norm of this Vectortor.
        /// </summary>
        public static double Norm1(lVector2 v) => v.Norm1;
        
        /// <summary>
        /// Returns the two-norm (euclidean length) of this Vectortor.
        /// </summary>
        public static double Norm2(lVector2 v) => v.Norm2;
        
        /// <summary>
        /// Returns the max-norm of this Vectortor.
        /// </summary>
        public static double NormMax(lVector2 v) => v.NormMax;
        
        /// <summary>
        /// Returns the p-norm of this Vectortor.
        /// </summary>
        public static double NormP(lVector2 v, double p) => v.NormP(p);
        
        /// <summary>
        /// Returns the inner product (dot product, scalar product) of the two Vectortors.
        /// </summary>
        public static long Dot(lVector2 lhs, lVector2 rhs) => lVector2.Dot(lhs, rhs);
        
        /// <summary>
        /// Returns the euclidean distance between the two Vectortors.
        /// </summary>
        public static double Distance(lVector2 lhs, lVector2 rhs) => lVector2.Distance(lhs, rhs);
        
        /// <summary>
        /// Returns the squared euclidean distance between the two Vectortors.
        /// </summary>
        public static double DistanceSqr(lVector2 lhs, lVector2 rhs) => lVector2.DistanceSqr(lhs, rhs);
        
        /// <summary>
        /// Calculate the reflection direction for an incident Vectortor (N should be normalized in order to achieve the desired result).
        /// </summary>
        public static lVector2 Reflect(lVector2 I, lVector2 N) => lVector2.Reflect(I, N);
        
        /// <summary>
        /// Calculate the refraction direction for an incident Vectortor (The input parameters I and N should be normalized in order to achieve the desired result).
        /// </summary>
        public static lVector2 Refract(lVector2 I, lVector2 N, long eta) => lVector2.Refract(I, N, eta);
        
        /// <summary>
        /// Returns a Vectortor pointing in the same direction as another (faceforward orients a Vectortor to point away from a surface as defined by its normal. If dot(Nref, I) is negative faceforward returns N, otherwise it returns -N).
        /// </summary>
        public static lVector2 FaceForward(lVector2 N, lVector2 I, lVector2 Nref) => lVector2.FaceForward(N, I, Nref);
        
        /// <summary>
        /// Returns the length of the outer product (cross product, Vectortor product) of the two Vectortors.
        /// </summary>
        public static long Cross(lVector2 l, lVector2 r) => lVector2.Cross(l, r);
        
        /// <summary>
        /// Returns a lVector2 with independent and identically distributed uniform integer values between 0 (inclusive) and maxValue (exclusive). (A maxValue of 0 is allowed and returns 0.)
        /// </summary>
        public static lVector2 Random(System.Random random, lVector2 maxValue) => lVector2.Random(random, maxValue);
        
        /// <summary>
        /// Returns a lVector2 with independent and identically distributed uniform integer values between minValue (inclusive) and maxValue (exclusive). (minValue == maxValue is allowed and returns minValue. Negative values are allowed.)
        /// </summary>
        public static lVector2 Random(System.Random random, lVector2 minValue, lVector2 maxValue) => lVector2.Random(random, minValue, maxValue);
        
        /// <summary>
        /// Returns a lVector2 with independent and identically distributed uniform integer values between minValue (inclusive) and maxValue (exclusive). (minValue == maxValue is allowed and returns minValue. Negative values are allowed.)
        /// </summary>
        public static lVector2 RandomUniform(System.Random random, lVector2 minValue, lVector2 maxValue) => lVector2.RandomUniform(random, minValue, maxValue);

    }
}
