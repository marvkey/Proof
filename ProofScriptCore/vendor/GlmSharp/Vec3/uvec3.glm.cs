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
        public static swizzle_uVector3 swizzle(uVector3 v) => v.swizzle;
        
        /// <summary>
        /// Returns an array with all values
        /// </summary>
        public static uint[] Values(uVector3 v) => v.Values;
        
        /// <summary>
        /// Returns an enumerator that iterates through all components.
        /// </summary>
        public static IEnumerator<uint> GetEnumerator(uVector3 v) => v.GetEnumerator();
        
        /// <summary>
        /// Returns a string representation of this Vectortor using ', ' as a seperator.
        /// </summary>
        public static string ToString(uVector3 v) => v.ToString();
        
        /// <summary>
        /// Returns a string representation of this Vectortor using a provided seperator.
        /// </summary>
        public static string ToString(uVector3 v, string sep) => v.ToString(sep);
        
        /// <summary>
        /// Returns a string representation of this Vectortor using a provided seperator and a forMatrix provider for each component.
        /// </summary>
        public static string ToString(uVector3 v, string sep, IFormatProvider provider) => v.ToString(sep, provider);
        
        /// <summary>
        /// Returns a string representation of this Vectortor using a provided seperator and a forMatrix for each component.
        /// </summary>
        public static string ToString(uVector3 v, string sep, string forMatrix) => v.ToString(sep, forMatrix);
        
        /// <summary>
        /// Returns a string representation of this Vectortor using a provided seperator and a forMatrix and forMatrix provider for each component.
        /// </summary>
        public static string ToString(uVector3 v, string sep, string forMatrix, IFormatProvider provider) => v.ToString(sep, forMatrix, provider);
        
        /// <summary>
        /// Returns the number of components (3).
        /// </summary>
        public static int Count(uVector3 v) => v.Count;
        
        /// <summary>
        /// Returns true iff this equals rhs component-wise.
        /// </summary>
        public static bool Equals(uVector3 v, uVector3 rhs) => v.Equals(rhs);
        
        /// <summary>
        /// Returns true iff this equals rhs type- and component-wise.
        /// </summary>
        public static bool Equals(uVector3 v, object obj) => v.Equals(obj);
        
        /// <summary>
        /// Returns a hash code for this instance.
        /// </summary>
        public static int GetHashCode(uVector3 v) => v.GetHashCode();
        
        /// <summary>
        /// Returns a bVector3 from component-wise application of Equal (lhs == rhs).
        /// </summary>
        public static bVector3 Equal(uVector3 lhs, uVector3 rhs) => uVector3.Equal(lhs, rhs);
        
        /// <summary>
        /// Returns a bVector3 from component-wise application of Equal (lhs == rhs).
        /// </summary>
        public static bool Equal(uint lhs, uint rhs) => lhs == rhs;
        
        /// <summary>
        /// Returns a bVector3 from component-wise application of NotEqual (lhs != rhs).
        /// </summary>
        public static bVector3 NotEqual(uVector3 lhs, uVector3 rhs) => uVector3.NotEqual(lhs, rhs);
        
        /// <summary>
        /// Returns a bVector3 from component-wise application of NotEqual (lhs != rhs).
        /// </summary>
        public static bool NotEqual(uint lhs, uint rhs) => lhs != rhs;
        
        /// <summary>
        /// Returns a bVector3 from component-wise application of GreaterThan (lhs &gt; rhs).
        /// </summary>
        public static bVector3 GreaterThan(uVector3 lhs, uVector3 rhs) => uVector3.GreaterThan(lhs, rhs);
        
        /// <summary>
        /// Returns a bVector3 from component-wise application of GreaterThan (lhs &gt; rhs).
        /// </summary>
        public static bool GreaterThan(uint lhs, uint rhs) => lhs > rhs;
        
        /// <summary>
        /// Returns a bVector3 from component-wise application of GreaterThanEqual (lhs &gt;= rhs).
        /// </summary>
        public static bVector3 GreaterThanEqual(uVector3 lhs, uVector3 rhs) => uVector3.GreaterThanEqual(lhs, rhs);
        
        /// <summary>
        /// Returns a bVector3 from component-wise application of GreaterThanEqual (lhs &gt;= rhs).
        /// </summary>
        public static bool GreaterThanEqual(uint lhs, uint rhs) => lhs >= rhs;
        
        /// <summary>
        /// Returns a bVector3 from component-wise application of LesserThan (lhs &lt; rhs).
        /// </summary>
        public static bVector3 LesserThan(uVector3 lhs, uVector3 rhs) => uVector3.LesserThan(lhs, rhs);
        
        /// <summary>
        /// Returns a bVector3 from component-wise application of LesserThan (lhs &lt; rhs).
        /// </summary>
        public static bool LesserThan(uint lhs, uint rhs) => lhs < rhs;
        
        /// <summary>
        /// Returns a bVector3 from component-wise application of LesserThanEqual (lhs &lt;= rhs).
        /// </summary>
        public static bVector3 LesserThanEqual(uVector3 lhs, uVector3 rhs) => uVector3.LesserThanEqual(lhs, rhs);
        
        /// <summary>
        /// Returns a bVector3 from component-wise application of LesserThanEqual (lhs &lt;= rhs).
        /// </summary>
        public static bool LesserThanEqual(uint lhs, uint rhs) => lhs <= rhs;
        
        /// <summary>
        /// Returns a uVector3 from component-wise application of Abs (v).
        /// </summary>
        public static uVector3 Abs(uVector3 v) => uVector3.Abs(v);
        
        /// <summary>
        /// Returns a uVector3 from component-wise application of Abs (v).
        /// </summary>
        public static uint Abs(uint v) => v;
        
        /// <summary>
        /// Returns a uVector3 from component-wise application of HermiteInterpolationOrder3 ((3 - 2 * v) * v * v).
        /// </summary>
        public static uVector3 HermiteInterpolationOrder3(uVector3 v) => uVector3.HermiteInterpolationOrder3(v);
        
        /// <summary>
        /// Returns a uVector3 from component-wise application of HermiteInterpolationOrder3 ((3 - 2 * v) * v * v).
        /// </summary>
        public static uint HermiteInterpolationOrder3(uint v) => (3 - 2 * v) * v * v;
        
        /// <summary>
        /// Returns a uVector3 from component-wise application of HermiteInterpolationOrder5 (((6 * v - 15) * v + 10) * v * v * v).
        /// </summary>
        public static uVector3 HermiteInterpolationOrder5(uVector3 v) => uVector3.HermiteInterpolationOrder5(v);
        
        /// <summary>
        /// Returns a uVector3 from component-wise application of HermiteInterpolationOrder5 (((6 * v - 15) * v + 10) * v * v * v).
        /// </summary>
        public static uint HermiteInterpolationOrder5(uint v) => ((6 * v - 15) * v + 10) * v * v * v;
        
        /// <summary>
        /// Returns a uVector3 from component-wise application of Sqr (v * v).
        /// </summary>
        public static uVector3 Sqr(uVector3 v) => uVector3.Sqr(v);
        
        /// <summary>
        /// Returns a uVector3 from component-wise application of Sqr (v * v).
        /// </summary>
        public static uint Sqr(uint v) => v * v;
        
        /// <summary>
        /// Returns a uVector3 from component-wise application of Pow2 (v * v).
        /// </summary>
        public static uVector3 Pow2(uVector3 v) => uVector3.Pow2(v);
        
        /// <summary>
        /// Returns a uVector3 from component-wise application of Pow2 (v * v).
        /// </summary>
        public static uint Pow2(uint v) => v * v;
        
        /// <summary>
        /// Returns a uVector3 from component-wise application of Pow3 (v * v * v).
        /// </summary>
        public static uVector3 Pow3(uVector3 v) => uVector3.Pow3(v);
        
        /// <summary>
        /// Returns a uVector3 from component-wise application of Pow3 (v * v * v).
        /// </summary>
        public static uint Pow3(uint v) => v * v * v;
        
        /// <summary>
        /// Returns a uVector3 from component-wise application of Step (v &gt;= 0u ? 1u : 0u).
        /// </summary>
        public static uVector3 Step(uVector3 v) => uVector3.Step(v);
        
        /// <summary>
        /// Returns a uVector3 from component-wise application of Step (v &gt;= 0u ? 1u : 0u).
        /// </summary>
        public static uint Step(uint v) => v >= 0u ? 1u : 0u;
        
        /// <summary>
        /// Returns a uVector3 from component-wise application of Sqrt ((uint)Math.Sqrt((double)v)).
        /// </summary>
        public static uVector3 Sqrt(uVector3 v) => uVector3.Sqrt(v);
        
        /// <summary>
        /// Returns a uVector3 from component-wise application of Sqrt ((uint)Math.Sqrt((double)v)).
        /// </summary>
        public static uint Sqrt(uint v) => (uint)Math.Sqrt((double)v);
        
        /// <summary>
        /// Returns a uVector3 from component-wise application of InverseSqrt ((uint)(1.0 / Math.Sqrt((double)v))).
        /// </summary>
        public static uVector3 InverseSqrt(uVector3 v) => uVector3.InverseSqrt(v);
        
        /// <summary>
        /// Returns a uVector3 from component-wise application of InverseSqrt ((uint)(1.0 / Math.Sqrt((double)v))).
        /// </summary>
        public static uint InverseSqrt(uint v) => (uint)(1.0 / Math.Sqrt((double)v));
        
        /// <summary>
        /// Returns a iVector3 from component-wise application of Sign (Math.Sign(v)).
        /// </summary>
        public static iVector3 Sign(uVector3 v) => uVector3.Sign(v);
        
        /// <summary>
        /// Returns a iVector3 from component-wise application of Sign (Math.Sign(v)).
        /// </summary>
        public static int Sign(uint v) => Math.Sign(v);
        
        /// <summary>
        /// Returns a uVector3 from component-wise application of Max (Math.Max(lhs, rhs)).
        /// </summary>
        public static uVector3 Max(uVector3 lhs, uVector3 rhs) => uVector3.Max(lhs, rhs);
        
        /// <summary>
        /// Returns a uVector3 from component-wise application of Max (Math.Max(lhs, rhs)).
        /// </summary>
        public static uint Max(uint lhs, uint rhs) => Math.Max(lhs, rhs);
        
        /// <summary>
        /// Returns a uVector3 from component-wise application of Min (Math.Min(lhs, rhs)).
        /// </summary>
        public static uVector3 Min(uVector3 lhs, uVector3 rhs) => uVector3.Min(lhs, rhs);
        
        /// <summary>
        /// Returns a uVector3 from component-wise application of Min (Math.Min(lhs, rhs)).
        /// </summary>
        public static uint Min(uint lhs, uint rhs) => Math.Min(lhs, rhs);
        
        /// <summary>
        /// Returns a uVector3 from component-wise application of Pow ((uint)Math.Pow((double)lhs, (double)rhs)).
        /// </summary>
        public static uVector3 Pow(uVector3 lhs, uVector3 rhs) => uVector3.Pow(lhs, rhs);
        
        /// <summary>
        /// Returns a uVector3 from component-wise application of Pow ((uint)Math.Pow((double)lhs, (double)rhs)).
        /// </summary>
        public static uint Pow(uint lhs, uint rhs) => (uint)Math.Pow((double)lhs, (double)rhs);
        
        /// <summary>
        /// Returns a uVector3 from component-wise application of Log ((uint)Math.Log((double)lhs, (double)rhs)).
        /// </summary>
        public static uVector3 Log(uVector3 lhs, uVector3 rhs) => uVector3.Log(lhs, rhs);
        
        /// <summary>
        /// Returns a uVector3 from component-wise application of Log ((uint)Math.Log((double)lhs, (double)rhs)).
        /// </summary>
        public static uint Log(uint lhs, uint rhs) => (uint)Math.Log((double)lhs, (double)rhs);
        
        /// <summary>
        /// Returns a uVector3 from component-wise application of Clamp (Math.Min(Math.Max(v, min), max)).
        /// </summary>
        public static uVector3 Clamp(uVector3 v, uVector3 min, uVector3 max) => uVector3.Clamp(v, min, max);
        
        /// <summary>
        /// Returns a uVector3 from component-wise application of Clamp (Math.Min(Math.Max(v, min), max)).
        /// </summary>
        public static uint Clamp(uint v, uint min, uint max) => Math.Min(Math.Max(v, min), max);
        
        /// <summary>
        /// Returns a uVector3 from component-wise application of Mix (min * (1-a) + max * a).
        /// </summary>
        public static uVector3 Mix(uVector3 min, uVector3 max, uVector3 a) => uVector3.Mix(min, max, a);
        
        /// <summary>
        /// Returns a uVector3 from component-wise application of Mix (min * (1-a) + max * a).
        /// </summary>
        public static uint Mix(uint min, uint max, uint a) => min * (1-a) + max * a;
        
        /// <summary>
        /// Returns a uVector3 from component-wise application of Lerp (min * (1-a) + max * a).
        /// </summary>
        public static uVector3 Lerp(uVector3 min, uVector3 max, uVector3 a) => uVector3.Lerp(min, max, a);
        
        /// <summary>
        /// Returns a uVector3 from component-wise application of Lerp (min * (1-a) + max * a).
        /// </summary>
        public static uint Lerp(uint min, uint max, uint a) => min * (1-a) + max * a;
        
        /// <summary>
        /// Returns a uVector3 from component-wise application of Smoothstep (((v - edge0) / (edge1 - edge0)).Clamp().HermiteInterpolationOrder3()).
        /// </summary>
        public static uVector3 Smoothstep(uVector3 edge0, uVector3 edge1, uVector3 v) => uVector3.Smoothstep(edge0, edge1, v);
        
        /// <summary>
        /// Returns a uVector3 from component-wise application of Smoothstep (((v - edge0) / (edge1 - edge0)).Clamp().HermiteInterpolationOrder3()).
        /// </summary>
        public static uint Smoothstep(uint edge0, uint edge1, uint v) => ((v - edge0) / (edge1 - edge0)).Clamp().HermiteInterpolationOrder3();
        
        /// <summary>
        /// Returns a uVector3 from component-wise application of Smootherstep (((v - edge0) / (edge1 - edge0)).Clamp().HermiteInterpolationOrder5()).
        /// </summary>
        public static uVector3 Smootherstep(uVector3 edge0, uVector3 edge1, uVector3 v) => uVector3.Smootherstep(edge0, edge1, v);
        
        /// <summary>
        /// Returns a uVector3 from component-wise application of Smootherstep (((v - edge0) / (edge1 - edge0)).Clamp().HermiteInterpolationOrder5()).
        /// </summary>
        public static uint Smootherstep(uint edge0, uint edge1, uint v) => ((v - edge0) / (edge1 - edge0)).Clamp().HermiteInterpolationOrder5();
        
        /// <summary>
        /// Returns a uVector3 from component-wise application of Fma (a * b + c).
        /// </summary>
        public static uVector3 Fma(uVector3 a, uVector3 b, uVector3 c) => uVector3.Fma(a, b, c);
        
        /// <summary>
        /// Returns a uVector3 from component-wise application of Fma (a * b + c).
        /// </summary>
        public static uint Fma(uint a, uint b, uint c) => a * b + c;
        
        /// <summary>
        /// OuterProduct treats the first parameter c as a column Vectortor (Matrix with one column) and the second parameter r as a row Vectortor (Matrix with one row) and does a linear algebraic Matrix multiply c * r, yielding a Matrix whose number of rows is the number of components in c and whose number of columns is the number of components in r.
        /// </summary>
        public static uMatrix2x3 OuterProduct(uVector3 c, uVector2 r) => uVector3.OuterProduct(c, r);
        
        /// <summary>
        /// OuterProduct treats the first parameter c as a column Vectortor (Matrix with one column) and the second parameter r as a row Vectortor (Matrix with one row) and does a linear algebraic Matrix multiply c * r, yielding a Matrix whose number of rows is the number of components in c and whose number of columns is the number of components in r.
        /// </summary>
        public static uMatrix3 OuterProduct(uVector3 c, uVector3 r) => uVector3.OuterProduct(c, r);
        
        /// <summary>
        /// OuterProduct treats the first parameter c as a column Vectortor (Matrix with one column) and the second parameter r as a row Vectortor (Matrix with one row) and does a linear algebraic Matrix multiply c * r, yielding a Matrix whose number of rows is the number of components in c and whose number of columns is the number of components in r.
        /// </summary>
        public static uMatrix4x3 OuterProduct(uVector3 c, uVector4 r) => uVector3.OuterProduct(c, r);
        
        /// <summary>
        /// Returns a uVector3 from component-wise application of Add (lhs + rhs).
        /// </summary>
        public static uVector3 Add(uVector3 lhs, uVector3 rhs) => uVector3.Add(lhs, rhs);
        
        /// <summary>
        /// Returns a uVector3 from component-wise application of Add (lhs + rhs).
        /// </summary>
        public static uint Add(uint lhs, uint rhs) => lhs + rhs;
        
        /// <summary>
        /// Returns a uVector3 from component-wise application of Sub (lhs - rhs).
        /// </summary>
        public static uVector3 Sub(uVector3 lhs, uVector3 rhs) => uVector3.Sub(lhs, rhs);
        
        /// <summary>
        /// Returns a uVector3 from component-wise application of Sub (lhs - rhs).
        /// </summary>
        public static uint Sub(uint lhs, uint rhs) => lhs - rhs;
        
        /// <summary>
        /// Returns a uVector3 from component-wise application of Mul (lhs * rhs).
        /// </summary>
        public static uVector3 Mul(uVector3 lhs, uVector3 rhs) => uVector3.Mul(lhs, rhs);
        
        /// <summary>
        /// Returns a uVector3 from component-wise application of Mul (lhs * rhs).
        /// </summary>
        public static uint Mul(uint lhs, uint rhs) => lhs * rhs;
        
        /// <summary>
        /// Returns a uVector3 from component-wise application of Div (lhs / rhs).
        /// </summary>
        public static uVector3 Div(uVector3 lhs, uVector3 rhs) => uVector3.Div(lhs, rhs);
        
        /// <summary>
        /// Returns a uVector3 from component-wise application of Div (lhs / rhs).
        /// </summary>
        public static uint Div(uint lhs, uint rhs) => lhs / rhs;
        
        /// <summary>
        /// Returns a uVector3 from component-wise application of Xor (lhs ^ rhs).
        /// </summary>
        public static uVector3 Xor(uVector3 lhs, uVector3 rhs) => uVector3.Xor(lhs, rhs);
        
        /// <summary>
        /// Returns a uVector3 from component-wise application of Xor (lhs ^ rhs).
        /// </summary>
        public static uint Xor(uint lhs, uint rhs) => lhs ^ rhs;
        
        /// <summary>
        /// Returns a uVector3 from component-wise application of BitwiseOr (lhs | rhs).
        /// </summary>
        public static uVector3 BitwiseOr(uVector3 lhs, uVector3 rhs) => uVector3.BitwiseOr(lhs, rhs);
        
        /// <summary>
        /// Returns a uVector3 from component-wise application of BitwiseOr (lhs | rhs).
        /// </summary>
        public static uint BitwiseOr(uint lhs, uint rhs) => lhs | rhs;
        
        /// <summary>
        /// Returns a uVector3 from component-wise application of BitwiseAnd (lhs &amp; rhs).
        /// </summary>
        public static uVector3 BitwiseAnd(uVector3 lhs, uVector3 rhs) => uVector3.BitwiseAnd(lhs, rhs);
        
        /// <summary>
        /// Returns a uVector3 from component-wise application of BitwiseAnd (lhs &amp; rhs).
        /// </summary>
        public static uint BitwiseAnd(uint lhs, uint rhs) => lhs & rhs;
        
        /// <summary>
        /// Returns a uVector3 from component-wise application of LeftShift (lhs &lt;&lt; rhs).
        /// </summary>
        public static uVector3 LeftShift(uVector3 lhs, iVector3 rhs) => uVector3.LeftShift(lhs, rhs);
        
        /// <summary>
        /// Returns a uVector3 from component-wise application of LeftShift (lhs &lt;&lt; rhs).
        /// </summary>
        public static uint LeftShift(uint lhs, int rhs) => lhs << rhs;
        
        /// <summary>
        /// Returns a uVector3 from component-wise application of RightShift (lhs &gt;&gt; rhs).
        /// </summary>
        public static uVector3 RightShift(uVector3 lhs, iVector3 rhs) => uVector3.RightShift(lhs, rhs);
        
        /// <summary>
        /// Returns a uVector3 from component-wise application of RightShift (lhs &gt;&gt; rhs).
        /// </summary>
        public static uint RightShift(uint lhs, int rhs) => lhs >> rhs;
        
        /// <summary>
        /// Returns the minimal component of this Vectortor.
        /// </summary>
        public static uint MinElement(uVector3 v) => v.MinElement;
        
        /// <summary>
        /// Returns the maximal component of this Vectortor.
        /// </summary>
        public static uint MaxElement(uVector3 v) => v.MaxElement;
        
        /// <summary>
        /// Returns the euclidean length of this Vectortor.
        /// </summary>
        public static float Length(uVector3 v) => v.Length;
        
        /// <summary>
        /// Returns the squared euclidean length of this Vectortor.
        /// </summary>
        public static float LengthSqr(uVector3 v) => v.LengthSqr;
        
        /// <summary>
        /// Returns the sum of all components.
        /// </summary>
        public static uint Sum(uVector3 v) => v.Sum;
        
        /// <summary>
        /// Returns the euclidean norm of this Vectortor.
        /// </summary>
        public static float Norm(uVector3 v) => v.Norm;
        
        /// <summary>
        /// Returns the one-norm of this Vectortor.
        /// </summary>
        public static float Norm1(uVector3 v) => v.Norm1;
        
        /// <summary>
        /// Returns the two-norm (euclidean length) of this Vectortor.
        /// </summary>
        public static float Norm2(uVector3 v) => v.Norm2;
        
        /// <summary>
        /// Returns the max-norm of this Vectortor.
        /// </summary>
        public static float NormMax(uVector3 v) => v.NormMax;
        
        /// <summary>
        /// Returns the p-norm of this Vectortor.
        /// </summary>
        public static double NormP(uVector3 v, double p) => v.NormP(p);
        
        /// <summary>
        /// Returns the inner product (dot product, scalar product) of the two Vectortors.
        /// </summary>
        public static uint Dot(uVector3 lhs, uVector3 rhs) => uVector3.Dot(lhs, rhs);
        
        /// <summary>
        /// Returns the euclidean distance between the two Vectortors.
        /// </summary>
        public static float Distance(uVector3 lhs, uVector3 rhs) => uVector3.Distance(lhs, rhs);
        
        /// <summary>
        /// Returns the squared euclidean distance between the two Vectortors.
        /// </summary>
        public static float DistanceSqr(uVector3 lhs, uVector3 rhs) => uVector3.DistanceSqr(lhs, rhs);
        
        /// <summary>
        /// Returns the outer product (cross product, Vectortor product) of the two Vectortors.
        /// </summary>
        public static uVector3 Cross(uVector3 l, uVector3 r) => uVector3.Cross(l, r);
        
        /// <summary>
        /// Returns a uVector3 with independent and identically distributed uniform integer values between 0 (inclusive) and maxValue (exclusive). (A maxValue of 0 is allowed and returns 0.)
        /// </summary>
        public static uVector3 Random(System.Random random, uVector3 maxValue) => uVector3.Random(random, maxValue);
        
        /// <summary>
        /// Returns a uVector3 with independent and identically distributed uniform integer values between 0 (inclusive) and maxValue (exclusive). (A maxValue of 0 is allowed and returns 0.)
        /// </summary>
        public static uint Random(System.Random random, uint maxValue) => (uint)random.Next((int)maxValue);
        
        /// <summary>
        /// Returns a uVector3 with independent and identically distributed uniform integer values between minValue (inclusive) and maxValue (exclusive). (minValue == maxValue is allowed and returns minValue. Negative values are allowed.)
        /// </summary>
        public static uVector3 Random(System.Random random, uVector3 minValue, uVector3 maxValue) => uVector3.Random(random, minValue, maxValue);
        
        /// <summary>
        /// Returns a uVector3 with independent and identically distributed uniform integer values between minValue (inclusive) and maxValue (exclusive). (minValue == maxValue is allowed and returns minValue. Negative values are allowed.)
        /// </summary>
        public static uint Random(System.Random random, uint minValue, uint maxValue) => (uint)random.Next((int)minValue, (int)maxValue);
        
        /// <summary>
        /// Returns a uVector3 with independent and identically distributed uniform integer values between minValue (inclusive) and maxValue (exclusive). (minValue == maxValue is allowed and returns minValue. Negative values are allowed.)
        /// </summary>
        public static uVector3 RandomUniform(System.Random random, uVector3 minValue, uVector3 maxValue) => uVector3.RandomUniform(random, minValue, maxValue);
        
        /// <summary>
        /// Returns a uVector3 with independent and identically distributed uniform integer values between minValue (inclusive) and maxValue (exclusive). (minValue == maxValue is allowed and returns minValue. Negative values are allowed.)
        /// </summary>
        public static uint RandomUniform(System.Random random, uint minValue, uint maxValue) => (uint)random.Next((int)minValue, (int)maxValue);

    }
}
