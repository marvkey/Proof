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
        public static swizzle_uVector2 swizzle(uVector2 v) => v.swizzle;
        
        /// <summary>
        /// Returns an array with all values
        /// </summary>
        public static uint[] Values(uVector2 v) => v.Values;
        
        /// <summary>
        /// Returns an enumerator that iterates through all components.
        /// </summary>
        public static IEnumerator<uint> GetEnumerator(uVector2 v) => v.GetEnumerator();
        
        /// <summary>
        /// Returns a string representation of this Vectortor using ', ' as a seperator.
        /// </summary>
        public static string ToString(uVector2 v) => v.ToString();
        
        /// <summary>
        /// Returns a string representation of this Vectortor using a provided seperator.
        /// </summary>
        public static string ToString(uVector2 v, string sep) => v.ToString(sep);
        
        /// <summary>
        /// Returns a string representation of this Vectortor using a provided seperator and a forMatrix provider for each component.
        /// </summary>
        public static string ToString(uVector2 v, string sep, IFormatProvider provider) => v.ToString(sep, provider);
        
        /// <summary>
        /// Returns a string representation of this Vectortor using a provided seperator and a forMatrix for each component.
        /// </summary>
        public static string ToString(uVector2 v, string sep, string forMatrix) => v.ToString(sep, forMatrix);
        
        /// <summary>
        /// Returns a string representation of this Vectortor using a provided seperator and a forMatrix and forMatrix provider for each component.
        /// </summary>
        public static string ToString(uVector2 v, string sep, string forMatrix, IFormatProvider provider) => v.ToString(sep, forMatrix, provider);
        
        /// <summary>
        /// Returns the number of components (2).
        /// </summary>
        public static int Count(uVector2 v) => v.Count;
        
        /// <summary>
        /// Returns true iff this equals rhs component-wise.
        /// </summary>
        public static bool Equals(uVector2 v, uVector2 rhs) => v.Equals(rhs);
        
        /// <summary>
        /// Returns true iff this equals rhs type- and component-wise.
        /// </summary>
        public static bool Equals(uVector2 v, object obj) => v.Equals(obj);
        
        /// <summary>
        /// Returns a hash code for this instance.
        /// </summary>
        public static int GetHashCode(uVector2 v) => v.GetHashCode();
        
        /// <summary>
        /// Returns a bVector2 from component-wise application of Equal (lhs == rhs).
        /// </summary>
        public static bVector2 Equal(uVector2 lhs, uVector2 rhs) => uVector2.Equal(lhs, rhs);
        
        /// <summary>
        /// Returns a bVector2 from component-wise application of NotEqual (lhs != rhs).
        /// </summary>
        public static bVector2 NotEqual(uVector2 lhs, uVector2 rhs) => uVector2.NotEqual(lhs, rhs);
        
        /// <summary>
        /// Returns a bVector2 from component-wise application of GreaterThan (lhs &gt; rhs).
        /// </summary>
        public static bVector2 GreaterThan(uVector2 lhs, uVector2 rhs) => uVector2.GreaterThan(lhs, rhs);
        
        /// <summary>
        /// Returns a bVector2 from component-wise application of GreaterThanEqual (lhs &gt;= rhs).
        /// </summary>
        public static bVector2 GreaterThanEqual(uVector2 lhs, uVector2 rhs) => uVector2.GreaterThanEqual(lhs, rhs);
        
        /// <summary>
        /// Returns a bVector2 from component-wise application of LesserThan (lhs &lt; rhs).
        /// </summary>
        public static bVector2 LesserThan(uVector2 lhs, uVector2 rhs) => uVector2.LesserThan(lhs, rhs);
        
        /// <summary>
        /// Returns a bVector2 from component-wise application of LesserThanEqual (lhs &lt;= rhs).
        /// </summary>
        public static bVector2 LesserThanEqual(uVector2 lhs, uVector2 rhs) => uVector2.LesserThanEqual(lhs, rhs);
        
        /// <summary>
        /// Returns a uVector2 from component-wise application of Abs (v).
        /// </summary>
        public static uVector2 Abs(uVector2 v) => uVector2.Abs(v);
        
        /// <summary>
        /// Returns a uVector2 from component-wise application of HermiteInterpolationOrder3 ((3 - 2 * v) * v * v).
        /// </summary>
        public static uVector2 HermiteInterpolationOrder3(uVector2 v) => uVector2.HermiteInterpolationOrder3(v);
        
        /// <summary>
        /// Returns a uVector2 from component-wise application of HermiteInterpolationOrder5 (((6 * v - 15) * v + 10) * v * v * v).
        /// </summary>
        public static uVector2 HermiteInterpolationOrder5(uVector2 v) => uVector2.HermiteInterpolationOrder5(v);
        
        /// <summary>
        /// Returns a uVector2 from component-wise application of Sqr (v * v).
        /// </summary>
        public static uVector2 Sqr(uVector2 v) => uVector2.Sqr(v);
        
        /// <summary>
        /// Returns a uVector2 from component-wise application of Pow2 (v * v).
        /// </summary>
        public static uVector2 Pow2(uVector2 v) => uVector2.Pow2(v);
        
        /// <summary>
        /// Returns a uVector2 from component-wise application of Pow3 (v * v * v).
        /// </summary>
        public static uVector2 Pow3(uVector2 v) => uVector2.Pow3(v);
        
        /// <summary>
        /// Returns a uVector2 from component-wise application of Step (v &gt;= 0u ? 1u : 0u).
        /// </summary>
        public static uVector2 Step(uVector2 v) => uVector2.Step(v);
        
        /// <summary>
        /// Returns a uVector2 from component-wise application of Sqrt ((uint)Math.Sqrt((double)v)).
        /// </summary>
        public static uVector2 Sqrt(uVector2 v) => uVector2.Sqrt(v);
        
        /// <summary>
        /// Returns a uVector2 from component-wise application of InverseSqrt ((uint)(1.0 / Math.Sqrt((double)v))).
        /// </summary>
        public static uVector2 InverseSqrt(uVector2 v) => uVector2.InverseSqrt(v);
        
        /// <summary>
        /// Returns a iVector2 from component-wise application of Sign (Math.Sign(v)).
        /// </summary>
        public static iVector2 Sign(uVector2 v) => uVector2.Sign(v);
        
        /// <summary>
        /// Returns a uVector2 from component-wise application of Max (Math.Max(lhs, rhs)).
        /// </summary>
        public static uVector2 Max(uVector2 lhs, uVector2 rhs) => uVector2.Max(lhs, rhs);
        
        /// <summary>
        /// Returns a uVector2 from component-wise application of Min (Math.Min(lhs, rhs)).
        /// </summary>
        public static uVector2 Min(uVector2 lhs, uVector2 rhs) => uVector2.Min(lhs, rhs);
        
        /// <summary>
        /// Returns a uVector2 from component-wise application of Pow ((uint)Math.Pow((double)lhs, (double)rhs)).
        /// </summary>
        public static uVector2 Pow(uVector2 lhs, uVector2 rhs) => uVector2.Pow(lhs, rhs);
        
        /// <summary>
        /// Returns a uVector2 from component-wise application of Log ((uint)Math.Log((double)lhs, (double)rhs)).
        /// </summary>
        public static uVector2 Log(uVector2 lhs, uVector2 rhs) => uVector2.Log(lhs, rhs);
        
        /// <summary>
        /// Returns a uVector2 from component-wise application of Clamp (Math.Min(Math.Max(v, min), max)).
        /// </summary>
        public static uVector2 Clamp(uVector2 v, uVector2 min, uVector2 max) => uVector2.Clamp(v, min, max);
        
        /// <summary>
        /// Returns a uVector2 from component-wise application of Mix (min * (1-a) + max * a).
        /// </summary>
        public static uVector2 Mix(uVector2 min, uVector2 max, uVector2 a) => uVector2.Mix(min, max, a);
        
        /// <summary>
        /// Returns a uVector2 from component-wise application of Lerp (min * (1-a) + max * a).
        /// </summary>
        public static uVector2 Lerp(uVector2 min, uVector2 max, uVector2 a) => uVector2.Lerp(min, max, a);
        
        /// <summary>
        /// Returns a uVector2 from component-wise application of Smoothstep (((v - edge0) / (edge1 - edge0)).Clamp().HermiteInterpolationOrder3()).
        /// </summary>
        public static uVector2 Smoothstep(uVector2 edge0, uVector2 edge1, uVector2 v) => uVector2.Smoothstep(edge0, edge1, v);
        
        /// <summary>
        /// Returns a uVector2 from component-wise application of Smootherstep (((v - edge0) / (edge1 - edge0)).Clamp().HermiteInterpolationOrder5()).
        /// </summary>
        public static uVector2 Smootherstep(uVector2 edge0, uVector2 edge1, uVector2 v) => uVector2.Smootherstep(edge0, edge1, v);
        
        /// <summary>
        /// Returns a uVector2 from component-wise application of Fma (a * b + c).
        /// </summary>
        public static uVector2 Fma(uVector2 a, uVector2 b, uVector2 c) => uVector2.Fma(a, b, c);
        
        /// <summary>
        /// OuterProduct treats the first parameter c as a column Vectortor (Matrix with one column) and the second parameter r as a row Vectortor (Matrix with one row) and does a linear algebraic Matrix multiply c * r, yielding a Matrix whose number of rows is the number of components in c and whose number of columns is the number of components in r.
        /// </summary>
        public static uMatrix2 OuterProduct(uVector2 c, uVector2 r) => uVector2.OuterProduct(c, r);
        
        /// <summary>
        /// OuterProduct treats the first parameter c as a column Vectortor (Matrix with one column) and the second parameter r as a row Vectortor (Matrix with one row) and does a linear algebraic Matrix multiply c * r, yielding a Matrix whose number of rows is the number of components in c and whose number of columns is the number of components in r.
        /// </summary>
        public static uMatrix3x2 OuterProduct(uVector2 c, uVector3 r) => uVector2.OuterProduct(c, r);
        
        /// <summary>
        /// OuterProduct treats the first parameter c as a column Vectortor (Matrix with one column) and the second parameter r as a row Vectortor (Matrix with one row) and does a linear algebraic Matrix multiply c * r, yielding a Matrix whose number of rows is the number of components in c and whose number of columns is the number of components in r.
        /// </summary>
        public static uMatrix4x2 OuterProduct(uVector2 c, uVector4 r) => uVector2.OuterProduct(c, r);
        
        /// <summary>
        /// Returns a uVector2 from component-wise application of Add (lhs + rhs).
        /// </summary>
        public static uVector2 Add(uVector2 lhs, uVector2 rhs) => uVector2.Add(lhs, rhs);
        
        /// <summary>
        /// Returns a uVector2 from component-wise application of Sub (lhs - rhs).
        /// </summary>
        public static uVector2 Sub(uVector2 lhs, uVector2 rhs) => uVector2.Sub(lhs, rhs);
        
        /// <summary>
        /// Returns a uVector2 from component-wise application of Mul (lhs * rhs).
        /// </summary>
        public static uVector2 Mul(uVector2 lhs, uVector2 rhs) => uVector2.Mul(lhs, rhs);
        
        /// <summary>
        /// Returns a uVector2 from component-wise application of Div (lhs / rhs).
        /// </summary>
        public static uVector2 Div(uVector2 lhs, uVector2 rhs) => uVector2.Div(lhs, rhs);
        
        /// <summary>
        /// Returns a uVector2 from component-wise application of Xor (lhs ^ rhs).
        /// </summary>
        public static uVector2 Xor(uVector2 lhs, uVector2 rhs) => uVector2.Xor(lhs, rhs);
        
        /// <summary>
        /// Returns a uVector2 from component-wise application of BitwiseOr (lhs | rhs).
        /// </summary>
        public static uVector2 BitwiseOr(uVector2 lhs, uVector2 rhs) => uVector2.BitwiseOr(lhs, rhs);
        
        /// <summary>
        /// Returns a uVector2 from component-wise application of BitwiseAnd (lhs &amp; rhs).
        /// </summary>
        public static uVector2 BitwiseAnd(uVector2 lhs, uVector2 rhs) => uVector2.BitwiseAnd(lhs, rhs);
        
        /// <summary>
        /// Returns a uVector2 from component-wise application of LeftShift (lhs &lt;&lt; rhs).
        /// </summary>
        public static uVector2 LeftShift(uVector2 lhs, iVector2 rhs) => uVector2.LeftShift(lhs, rhs);
        
        /// <summary>
        /// Returns a uVector2 from component-wise application of RightShift (lhs &gt;&gt; rhs).
        /// </summary>
        public static uVector2 RightShift(uVector2 lhs, iVector2 rhs) => uVector2.RightShift(lhs, rhs);
        
        /// <summary>
        /// Returns the minimal component of this Vectortor.
        /// </summary>
        public static uint MinElement(uVector2 v) => v.MinElement;
        
        /// <summary>
        /// Returns the maximal component of this Vectortor.
        /// </summary>
        public static uint MaxElement(uVector2 v) => v.MaxElement;
        
        /// <summary>
        /// Returns the euclidean length of this Vectortor.
        /// </summary>
        public static float Length(uVector2 v) => v.Length;
        
        /// <summary>
        /// Returns the squared euclidean length of this Vectortor.
        /// </summary>
        public static float LengthSqr(uVector2 v) => v.LengthSqr;
        
        /// <summary>
        /// Returns the sum of all components.
        /// </summary>
        public static uint Sum(uVector2 v) => v.Sum;
        
        /// <summary>
        /// Returns the euclidean norm of this Vectortor.
        /// </summary>
        public static float Norm(uVector2 v) => v.Norm;
        
        /// <summary>
        /// Returns the one-norm of this Vectortor.
        /// </summary>
        public static float Norm1(uVector2 v) => v.Norm1;
        
        /// <summary>
        /// Returns the two-norm (euclidean length) of this Vectortor.
        /// </summary>
        public static float Norm2(uVector2 v) => v.Norm2;
        
        /// <summary>
        /// Returns the max-norm of this Vectortor.
        /// </summary>
        public static float NormMax(uVector2 v) => v.NormMax;
        
        /// <summary>
        /// Returns the p-norm of this Vectortor.
        /// </summary>
        public static double NormP(uVector2 v, double p) => v.NormP(p);
        
        /// <summary>
        /// Returns the inner product (dot product, scalar product) of the two Vectortors.
        /// </summary>
        public static uint Dot(uVector2 lhs, uVector2 rhs) => uVector2.Dot(lhs, rhs);
        
        /// <summary>
        /// Returns the euclidean distance between the two Vectortors.
        /// </summary>
        public static float Distance(uVector2 lhs, uVector2 rhs) => uVector2.Distance(lhs, rhs);
        
        /// <summary>
        /// Returns the squared euclidean distance between the two Vectortors.
        /// </summary>
        public static float DistanceSqr(uVector2 lhs, uVector2 rhs) => uVector2.DistanceSqr(lhs, rhs);
        
        /// <summary>
        /// Returns the length of the outer product (cross product, Vectortor product) of the two Vectortors.
        /// </summary>
        public static uint Cross(uVector2 l, uVector2 r) => uVector2.Cross(l, r);
        
        /// <summary>
        /// Returns a uVector2 with independent and identically distributed uniform integer values between 0 (inclusive) and maxValue (exclusive). (A maxValue of 0 is allowed and returns 0.)
        /// </summary>
        public static uVector2 Random(System.Random random, uVector2 maxValue) => uVector2.Random(random, maxValue);
        
        /// <summary>
        /// Returns a uVector2 with independent and identically distributed uniform integer values between minValue (inclusive) and maxValue (exclusive). (minValue == maxValue is allowed and returns minValue. Negative values are allowed.)
        /// </summary>
        public static uVector2 Random(System.Random random, uVector2 minValue, uVector2 maxValue) => uVector2.Random(random, minValue, maxValue);
        
        /// <summary>
        /// Returns a uVector2 with independent and identically distributed uniform integer values between minValue (inclusive) and maxValue (exclusive). (minValue == maxValue is allowed and returns minValue. Negative values are allowed.)
        /// </summary>
        public static uVector2 RandomUniform(System.Random random, uVector2 minValue, uVector2 maxValue) => uVector2.RandomUniform(random, minValue, maxValue);

    }
}
