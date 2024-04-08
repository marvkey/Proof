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
        public static swizzle_uVector4 swizzle(uVector4 v) => v.swizzle;
        
        /// <summary>
        /// Returns an array with all values
        /// </summary>
        public static uint[] Values(uVector4 v) => v.Values;
        
        /// <summary>
        /// Returns an enumerator that iterates through all components.
        /// </summary>
        public static IEnumerator<uint> GetEnumerator(uVector4 v) => v.GetEnumerator();
        
        /// <summary>
        /// Returns a string representation of this Vector using ', ' as a seperator.
        /// </summary>
        public static string ToString(uVector4 v) => v.ToString();
        
        /// <summary>
        /// Returns a string representation of this Vector using a provided seperator.
        /// </summary>
        public static string ToString(uVector4 v, string sep) => v.ToString(sep);
        
        /// <summary>
        /// Returns a string representation of this Vector using a provided seperator and a forMatrix provider for each component.
        /// </summary>
        public static string ToString(uVector4 v, string sep, IFormatProvider provider) => v.ToString(sep, provider);
        
        /// <summary>
        /// Returns a string representation of this Vector using a provided seperator and a forMatrix for each component.
        /// </summary>
        public static string ToString(uVector4 v, string sep, string forMatrix) => v.ToString(sep, forMatrix);
        
        /// <summary>
        /// Returns a string representation of this Vector using a provided seperator and a forMatrix and forMatrix provider for each component.
        /// </summary>
        public static string ToString(uVector4 v, string sep, string forMatrix, IFormatProvider provider) => v.ToString(sep, forMatrix, provider);
        
        /// <summary>
        /// Returns the number of components (4).
        /// </summary>
        public static int Count(uVector4 v) => v.Count;
        
        /// <summary>
        /// Returns true iff this equals rhs component-wise.
        /// </summary>
        public static bool Equals(uVector4 v, uVector4 rhs) => v.Equals(rhs);
        
        /// <summary>
        /// Returns true iff this equals rhs type- and component-wise.
        /// </summary>
        public static bool Equals(uVector4 v, object obj) => v.Equals(obj);
        
        /// <summary>
        /// Returns a hash code for this instance.
        /// </summary>
        public static int GetHashCode(uVector4 v) => v.GetHashCode();
        
        /// <summary>
        /// Returns a bVector4 from component-wise application of Equal (lhs == rhs).
        /// </summary>
        public static bVector4 Equal(uVector4 lhs, uVector4 rhs) => uVector4.Equal(lhs, rhs);
        
        /// <summary>
        /// Returns a bVector4 from component-wise application of NotEqual (lhs != rhs).
        /// </summary>
        public static bVector4 NotEqual(uVector4 lhs, uVector4 rhs) => uVector4.NotEqual(lhs, rhs);
        
        /// <summary>
        /// Returns a bVector4 from component-wise application of GreaterThan (lhs &gt; rhs).
        /// </summary>
        public static bVector4 GreaterThan(uVector4 lhs, uVector4 rhs) => uVector4.GreaterThan(lhs, rhs);
        
        /// <summary>
        /// Returns a bVector4 from component-wise application of GreaterThanEqual (lhs &gt;= rhs).
        /// </summary>
        public static bVector4 GreaterThanEqual(uVector4 lhs, uVector4 rhs) => uVector4.GreaterThanEqual(lhs, rhs);
        
        /// <summary>
        /// Returns a bVector4 from component-wise application of LesserThan (lhs &lt; rhs).
        /// </summary>
        public static bVector4 LesserThan(uVector4 lhs, uVector4 rhs) => uVector4.LesserThan(lhs, rhs);
        
        /// <summary>
        /// Returns a bVector4 from component-wise application of LesserThanEqual (lhs &lt;= rhs).
        /// </summary>
        public static bVector4 LesserThanEqual(uVector4 lhs, uVector4 rhs) => uVector4.LesserThanEqual(lhs, rhs);
        
        /// <summary>
        /// Returns a uVector4 from component-wise application of Abs (v).
        /// </summary>
        public static uVector4 Abs(uVector4 v) => uVector4.Abs(v);
        
        /// <summary>
        /// Returns a uVector4 from component-wise application of HermiteInterpolationOrder3 ((3 - 2 * v) * v * v).
        /// </summary>
        public static uVector4 HermiteInterpolationOrder3(uVector4 v) => uVector4.HermiteInterpolationOrder3(v);
        
        /// <summary>
        /// Returns a uVector4 from component-wise application of HermiteInterpolationOrder5 (((6 * v - 15) * v + 10) * v * v * v).
        /// </summary>
        public static uVector4 HermiteInterpolationOrder5(uVector4 v) => uVector4.HermiteInterpolationOrder5(v);
        
        /// <summary>
        /// Returns a uVector4 from component-wise application of Sqr (v * v).
        /// </summary>
        public static uVector4 Sqr(uVector4 v) => uVector4.Sqr(v);
        
        /// <summary>
        /// Returns a uVector4 from component-wise application of Pow2 (v * v).
        /// </summary>
        public static uVector4 Pow2(uVector4 v) => uVector4.Pow2(v);
        
        /// <summary>
        /// Returns a uVector4 from component-wise application of Pow3 (v * v * v).
        /// </summary>
        public static uVector4 Pow3(uVector4 v) => uVector4.Pow3(v);
        
        /// <summary>
        /// Returns a uVector4 from component-wise application of Step (v &gt;= 0u ? 1u : 0u).
        /// </summary>
        public static uVector4 Step(uVector4 v) => uVector4.Step(v);
        
        /// <summary>
        /// Returns a uVector4 from component-wise application of Sqrt ((uint)Math.Sqrt((double)v)).
        /// </summary>
        public static uVector4 Sqrt(uVector4 v) => uVector4.Sqrt(v);
        
        /// <summary>
        /// Returns a uVector4 from component-wise application of InverseSqrt ((uint)(1.0 / Math.Sqrt((double)v))).
        /// </summary>
        public static uVector4 InverseSqrt(uVector4 v) => uVector4.InverseSqrt(v);
        
        /// <summary>
        /// Returns a iVector4 from component-wise application of Sign (Math.Sign(v)).
        /// </summary>
        public static iVector4 Sign(uVector4 v) => uVector4.Sign(v);
        
        /// <summary>
        /// Returns a uVector4 from component-wise application of Max (Math.Max(lhs, rhs)).
        /// </summary>
        public static uVector4 Max(uVector4 lhs, uVector4 rhs) => uVector4.Max(lhs, rhs);
        
        /// <summary>
        /// Returns a uVector4 from component-wise application of Min (Math.Min(lhs, rhs)).
        /// </summary>
        public static uVector4 Min(uVector4 lhs, uVector4 rhs) => uVector4.Min(lhs, rhs);
        
        /// <summary>
        /// Returns a uVector4 from component-wise application of Pow ((uint)Math.Pow((double)lhs, (double)rhs)).
        /// </summary>
        public static uVector4 Pow(uVector4 lhs, uVector4 rhs) => uVector4.Pow(lhs, rhs);
        
        /// <summary>
        /// Returns a uVector4 from component-wise application of Log ((uint)Math.Log((double)lhs, (double)rhs)).
        /// </summary>
        public static uVector4 Log(uVector4 lhs, uVector4 rhs) => uVector4.Log(lhs, rhs);
        
        /// <summary>
        /// Returns a uVector4 from component-wise application of Clamp (Math.Min(Math.Max(v, min), max)).
        /// </summary>
        public static uVector4 Clamp(uVector4 v, uVector4 min, uVector4 max) => uVector4.Clamp(v, min, max);
        
        /// <summary>
        /// Returns a uVector4 from component-wise application of Mix (min * (1-a) + max * a).
        /// </summary>
        public static uVector4 Mix(uVector4 min, uVector4 max, uVector4 a) => uVector4.Mix(min, max, a);
        
        /// <summary>
        /// Returns a uVector4 from component-wise application of Lerp (min * (1-a) + max * a).
        /// </summary>
        public static uVector4 Lerp(uVector4 min, uVector4 max, uVector4 a) => uVector4.Lerp(min, max, a);
        
        /// <summary>
        /// Returns a uVector4 from component-wise application of Smoothstep (((v - edge0) / (edge1 - edge0)).Clamp().HermiteInterpolationOrder3()).
        /// </summary>
        public static uVector4 Smoothstep(uVector4 edge0, uVector4 edge1, uVector4 v) => uVector4.Smoothstep(edge0, edge1, v);
        
        /// <summary>
        /// Returns a uVector4 from component-wise application of Smootherstep (((v - edge0) / (edge1 - edge0)).Clamp().HermiteInterpolationOrder5()).
        /// </summary>
        public static uVector4 Smootherstep(uVector4 edge0, uVector4 edge1, uVector4 v) => uVector4.Smootherstep(edge0, edge1, v);
        
        /// <summary>
        /// Returns a uVector4 from component-wise application of Fma (a * b + c).
        /// </summary>
        public static uVector4 Fma(uVector4 a, uVector4 b, uVector4 c) => uVector4.Fma(a, b, c);
        
        /// <summary>
        /// OuterProduct treats the first parameter c as a column Vector (Matrix with one column) and the second parameter r as a row Vector (Matrix with one row) and does a linear algebraic Matrix multiply c * r, yielding a Matrix whose number of rows is the number of components in c and whose number of columns is the number of components in r.
        /// </summary>
        public static uMatrix2x4 OuterProduct(uVector4 c, uVector2 r) => uVector4.OuterProduct(c, r);
        
        /// <summary>
        /// OuterProduct treats the first parameter c as a column Vector (Matrix with one column) and the second parameter r as a row Vector (Matrix with one row) and does a linear algebraic Matrix multiply c * r, yielding a Matrix whose number of rows is the number of components in c and whose number of columns is the number of components in r.
        /// </summary>
        public static uMatrix3x4 OuterProduct(uVector4 c, uVector3 r) => uVector4.OuterProduct(c, r);
        
        /// <summary>
        /// OuterProduct treats the first parameter c as a column Vector (Matrix with one column) and the second parameter r as a row Vector (Matrix with one row) and does a linear algebraic Matrix multiply c * r, yielding a Matrix whose number of rows is the number of components in c and whose number of columns is the number of components in r.
        /// </summary>
        public static uMatrix4 OuterProduct(uVector4 c, uVector4 r) => uVector4.OuterProduct(c, r);
        
        /// <summary>
        /// Returns a uVector4 from component-wise application of Add (lhs + rhs).
        /// </summary>
        public static uVector4 Add(uVector4 lhs, uVector4 rhs) => uVector4.Add(lhs, rhs);
        
        /// <summary>
        /// Returns a uVector4 from component-wise application of Sub (lhs - rhs).
        /// </summary>
        public static uVector4 Sub(uVector4 lhs, uVector4 rhs) => uVector4.Sub(lhs, rhs);
        
        /// <summary>
        /// Returns a uVector4 from component-wise application of Mul (lhs * rhs).
        /// </summary>
        public static uVector4 Mul(uVector4 lhs, uVector4 rhs) => uVector4.Mul(lhs, rhs);
        
        /// <summary>
        /// Returns a uVector4 from component-wise application of Div (lhs / rhs).
        /// </summary>
        public static uVector4 Div(uVector4 lhs, uVector4 rhs) => uVector4.Div(lhs, rhs);
        
        /// <summary>
        /// Returns a uVector4 from component-wise application of Xor (lhs ^ rhs).
        /// </summary>
        public static uVector4 Xor(uVector4 lhs, uVector4 rhs) => uVector4.Xor(lhs, rhs);
        
        /// <summary>
        /// Returns a uVector4 from component-wise application of BitwiseOr (lhs | rhs).
        /// </summary>
        public static uVector4 BitwiseOr(uVector4 lhs, uVector4 rhs) => uVector4.BitwiseOr(lhs, rhs);
        
        /// <summary>
        /// Returns a uVector4 from component-wise application of BitwiseAnd (lhs &amp; rhs).
        /// </summary>
        public static uVector4 BitwiseAnd(uVector4 lhs, uVector4 rhs) => uVector4.BitwiseAnd(lhs, rhs);
        
        /// <summary>
        /// Returns a uVector4 from component-wise application of LeftShift (lhs &lt;&lt; rhs).
        /// </summary>
        public static uVector4 LeftShift(uVector4 lhs, iVector4 rhs) => uVector4.LeftShift(lhs, rhs);
        
        /// <summary>
        /// Returns a uVector4 from component-wise application of RightShift (lhs &gt;&gt; rhs).
        /// </summary>
        public static uVector4 RightShift(uVector4 lhs, iVector4 rhs) => uVector4.RightShift(lhs, rhs);
        
        /// <summary>
        /// Returns the minimal component of this Vector.
        /// </summary>
        public static uint MinElement(uVector4 v) => v.MinElement;
        
        /// <summary>
        /// Returns the maximal component of this Vector.
        /// </summary>
        public static uint MaxElement(uVector4 v) => v.MaxElement;
        
        /// <summary>
        /// Returns the euclidean length of this Vector.
        /// </summary>
        public static float Length(uVector4 v) => v.Length;
        
        /// <summary>
        /// Returns the squared euclidean length of this Vector.
        /// </summary>
        public static float LengthSqr(uVector4 v) => v.LengthSqr;
        
        /// <summary>
        /// Returns the sum of all components.
        /// </summary>
        public static uint Sum(uVector4 v) => v.Sum;
        
        /// <summary>
        /// Returns the euclidean norm of this Vector.
        /// </summary>
        public static float Norm(uVector4 v) => v.Norm;
        
        /// <summary>
        /// Returns the one-norm of this Vector.
        /// </summary>
        public static float Norm1(uVector4 v) => v.Norm1;
        
        /// <summary>
        /// Returns the two-norm (euclidean length) of this Vector.
        /// </summary>
        public static float Norm2(uVector4 v) => v.Norm2;
        
        /// <summary>
        /// Returns the max-norm of this Vector.
        /// </summary>
        public static float NormMax(uVector4 v) => v.NormMax;
        
        /// <summary>
        /// Returns the p-norm of this Vector.
        /// </summary>
        public static double NormP(uVector4 v, double p) => v.NormP(p);
        
        /// <summary>
        /// Returns the inner product (dot product, scalar product) of the two Vectors.
        /// </summary>
        public static uint Dot(uVector4 lhs, uVector4 rhs) => uVector4.Dot(lhs, rhs);
        
        /// <summary>
        /// Returns the euclidean distance between the two Vectors.
        /// </summary>
        public static float Distance(uVector4 lhs, uVector4 rhs) => uVector4.Distance(lhs, rhs);
        
        /// <summary>
        /// Returns the squared euclidean distance between the two Vectors.
        /// </summary>
        public static float DistanceSqr(uVector4 lhs, uVector4 rhs) => uVector4.DistanceSqr(lhs, rhs);
        
        /// <summary>
        /// Returns a uVector4 with independent and identically distributed uniform integer values between 0 (inclusive) and maxValue (exclusive). (A maxValue of 0 is allowed and returns 0.)
        /// </summary>
        public static uVector4 Random(System.Random random, uVector4 maxValue) => uVector4.Random(random, maxValue);
        
        /// <summary>
        /// Returns a uVector4 with independent and identically distributed uniform integer values between minValue (inclusive) and maxValue (exclusive). (minValue == maxValue is allowed and returns minValue. Negative values are allowed.)
        /// </summary>
        public static uVector4 Random(System.Random random, uVector4 minValue, uVector4 maxValue) => uVector4.Random(random, minValue, maxValue);
        
        /// <summary>
        /// Returns a uVector4 with independent and identically distributed uniform integer values between minValue (inclusive) and maxValue (exclusive). (minValue == maxValue is allowed and returns minValue. Negative values are allowed.)
        /// </summary>
        public static uVector4 RandomUniform(System.Random random, uVector4 minValue, uVector4 maxValue) => uVector4.RandomUniform(random, minValue, maxValue);

    }
}
