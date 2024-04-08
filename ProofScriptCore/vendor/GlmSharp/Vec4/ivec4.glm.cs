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
        public static swizzle_iVector4 swizzle(iVector4 v) => v.swizzle;
        
        /// <summary>
        /// Returns an array with all values
        /// </summary>
        public static int[] Values(iVector4 v) => v.Values;
        
        /// <summary>
        /// Returns an enumerator that iterates through all components.
        /// </summary>
        public static IEnumerator<int> GetEnumerator(iVector4 v) => v.GetEnumerator();
        
        /// <summary>
        /// Returns a string representation of this Vector using ', ' as a seperator.
        /// </summary>
        public static string ToString(iVector4 v) => v.ToString();
        
        /// <summary>
        /// Returns a string representation of this Vector using a provided seperator.
        /// </summary>
        public static string ToString(iVector4 v, string sep) => v.ToString(sep);
        
        /// <summary>
        /// Returns a string representation of this Vector using a provided seperator and a forMatrix provider for each component.
        /// </summary>
        public static string ToString(iVector4 v, string sep, IFormatProvider provider) => v.ToString(sep, provider);
        
        /// <summary>
        /// Returns a string representation of this Vector using a provided seperator and a forMatrix for each component.
        /// </summary>
        public static string ToString(iVector4 v, string sep, string forMatrix) => v.ToString(sep, forMatrix);
        
        /// <summary>
        /// Returns a string representation of this Vector using a provided seperator and a forMatrix and forMatrix provider for each component.
        /// </summary>
        public static string ToString(iVector4 v, string sep, string forMatrix, IFormatProvider provider) => v.ToString(sep, forMatrix, provider);
        
        /// <summary>
        /// Returns the number of components (4).
        /// </summary>
        public static int Count(iVector4 v) => v.Count;
        
        /// <summary>
        /// Returns true iff this equals rhs component-wise.
        /// </summary>
        public static bool Equals(iVector4 v, iVector4 rhs) => v.Equals(rhs);
        
        /// <summary>
        /// Returns true iff this equals rhs type- and component-wise.
        /// </summary>
        public static bool Equals(iVector4 v, object obj) => v.Equals(obj);
        
        /// <summary>
        /// Returns a hash code for this instance.
        /// </summary>
        public static int GetHashCode(iVector4 v) => v.GetHashCode();
        
        /// <summary>
        /// Returns a bVector4 from component-wise application of Equal (lhs == rhs).
        /// </summary>
        public static bVector4 Equal(iVector4 lhs, iVector4 rhs) => iVector4.Equal(lhs, rhs);
        
        /// <summary>
        /// Returns a bVector4 from component-wise application of NotEqual (lhs != rhs).
        /// </summary>
        public static bVector4 NotEqual(iVector4 lhs, iVector4 rhs) => iVector4.NotEqual(lhs, rhs);
        
        /// <summary>
        /// Returns a bVector4 from component-wise application of GreaterThan (lhs &gt; rhs).
        /// </summary>
        public static bVector4 GreaterThan(iVector4 lhs, iVector4 rhs) => iVector4.GreaterThan(lhs, rhs);
        
        /// <summary>
        /// Returns a bVector4 from component-wise application of GreaterThanEqual (lhs &gt;= rhs).
        /// </summary>
        public static bVector4 GreaterThanEqual(iVector4 lhs, iVector4 rhs) => iVector4.GreaterThanEqual(lhs, rhs);
        
        /// <summary>
        /// Returns a bVector4 from component-wise application of LesserThan (lhs &lt; rhs).
        /// </summary>
        public static bVector4 LesserThan(iVector4 lhs, iVector4 rhs) => iVector4.LesserThan(lhs, rhs);
        
        /// <summary>
        /// Returns a bVector4 from component-wise application of LesserThanEqual (lhs &lt;= rhs).
        /// </summary>
        public static bVector4 LesserThanEqual(iVector4 lhs, iVector4 rhs) => iVector4.LesserThanEqual(lhs, rhs);
        
        /// <summary>
        /// Returns a iVector4 from component-wise application of Abs (Math.Abs(v)).
        /// </summary>
        public static iVector4 Abs(iVector4 v) => iVector4.Abs(v);
        
        /// <summary>
        /// Returns a iVector4 from component-wise application of HermiteInterpolationOrder3 ((3 - 2 * v) * v * v).
        /// </summary>
        public static iVector4 HermiteInterpolationOrder3(iVector4 v) => iVector4.HermiteInterpolationOrder3(v);
        
        /// <summary>
        /// Returns a iVector4 from component-wise application of HermiteInterpolationOrder5 (((6 * v - 15) * v + 10) * v * v * v).
        /// </summary>
        public static iVector4 HermiteInterpolationOrder5(iVector4 v) => iVector4.HermiteInterpolationOrder5(v);
        
        /// <summary>
        /// Returns a iVector4 from component-wise application of Sqr (v * v).
        /// </summary>
        public static iVector4 Sqr(iVector4 v) => iVector4.Sqr(v);
        
        /// <summary>
        /// Returns a iVector4 from component-wise application of Pow2 (v * v).
        /// </summary>
        public static iVector4 Pow2(iVector4 v) => iVector4.Pow2(v);
        
        /// <summary>
        /// Returns a iVector4 from component-wise application of Pow3 (v * v * v).
        /// </summary>
        public static iVector4 Pow3(iVector4 v) => iVector4.Pow3(v);
        
        /// <summary>
        /// Returns a iVector4 from component-wise application of Step (v &gt;= 0 ? 1 : 0).
        /// </summary>
        public static iVector4 Step(iVector4 v) => iVector4.Step(v);
        
        /// <summary>
        /// Returns a iVector4 from component-wise application of Sqrt ((int)Math.Sqrt((double)v)).
        /// </summary>
        public static iVector4 Sqrt(iVector4 v) => iVector4.Sqrt(v);
        
        /// <summary>
        /// Returns a iVector4 from component-wise application of InverseSqrt ((int)(1.0 / Math.Sqrt((double)v))).
        /// </summary>
        public static iVector4 InverseSqrt(iVector4 v) => iVector4.InverseSqrt(v);
        
        /// <summary>
        /// Returns a iVector4 from component-wise application of Sign (Math.Sign(v)).
        /// </summary>
        public static iVector4 Sign(iVector4 v) => iVector4.Sign(v);
        
        /// <summary>
        /// Returns a iVector4 from component-wise application of Max (Math.Max(lhs, rhs)).
        /// </summary>
        public static iVector4 Max(iVector4 lhs, iVector4 rhs) => iVector4.Max(lhs, rhs);
        
        /// <summary>
        /// Returns a iVector4 from component-wise application of Min (Math.Min(lhs, rhs)).
        /// </summary>
        public static iVector4 Min(iVector4 lhs, iVector4 rhs) => iVector4.Min(lhs, rhs);
        
        /// <summary>
        /// Returns a iVector4 from component-wise application of Pow ((int)Math.Pow((double)lhs, (double)rhs)).
        /// </summary>
        public static iVector4 Pow(iVector4 lhs, iVector4 rhs) => iVector4.Pow(lhs, rhs);
        
        /// <summary>
        /// Returns a iVector4 from component-wise application of Log ((int)Math.Log((double)lhs, (double)rhs)).
        /// </summary>
        public static iVector4 Log(iVector4 lhs, iVector4 rhs) => iVector4.Log(lhs, rhs);
        
        /// <summary>
        /// Returns a iVector4 from component-wise application of Clamp (Math.Min(Math.Max(v, min), max)).
        /// </summary>
        public static iVector4 Clamp(iVector4 v, iVector4 min, iVector4 max) => iVector4.Clamp(v, min, max);
        
        /// <summary>
        /// Returns a iVector4 from component-wise application of Mix (min * (1-a) + max * a).
        /// </summary>
        public static iVector4 Mix(iVector4 min, iVector4 max, iVector4 a) => iVector4.Mix(min, max, a);
        
        /// <summary>
        /// Returns a iVector4 from component-wise application of Lerp (min * (1-a) + max * a).
        /// </summary>
        public static iVector4 Lerp(iVector4 min, iVector4 max, iVector4 a) => iVector4.Lerp(min, max, a);
        
        /// <summary>
        /// Returns a iVector4 from component-wise application of Smoothstep (((v - edge0) / (edge1 - edge0)).Clamp().HermiteInterpolationOrder3()).
        /// </summary>
        public static iVector4 Smoothstep(iVector4 edge0, iVector4 edge1, iVector4 v) => iVector4.Smoothstep(edge0, edge1, v);
        
        /// <summary>
        /// Returns a iVector4 from component-wise application of Smootherstep (((v - edge0) / (edge1 - edge0)).Clamp().HermiteInterpolationOrder5()).
        /// </summary>
        public static iVector4 Smootherstep(iVector4 edge0, iVector4 edge1, iVector4 v) => iVector4.Smootherstep(edge0, edge1, v);
        
        /// <summary>
        /// Returns a iVector4 from component-wise application of Fma (a * b + c).
        /// </summary>
        public static iVector4 Fma(iVector4 a, iVector4 b, iVector4 c) => iVector4.Fma(a, b, c);
        
        /// <summary>
        /// OuterProduct treats the first parameter c as a column Vector (Matrix with one column) and the second parameter r as a row Vector (Matrix with one row) and does a linear algebraic Matrix multiply c * r, yielding a Matrix whose number of rows is the number of components in c and whose number of columns is the number of components in r.
        /// </summary>
        public static iMatrix2x4 OuterProduct(iVector4 c, iVector2 r) => iVector4.OuterProduct(c, r);
        
        /// <summary>
        /// OuterProduct treats the first parameter c as a column Vector (Matrix with one column) and the second parameter r as a row Vector (Matrix with one row) and does a linear algebraic Matrix multiply c * r, yielding a Matrix whose number of rows is the number of components in c and whose number of columns is the number of components in r.
        /// </summary>
        public static iMatrix3x4 OuterProduct(iVector4 c, iVector3 r) => iVector4.OuterProduct(c, r);
        
        /// <summary>
        /// OuterProduct treats the first parameter c as a column Vector (Matrix with one column) and the second parameter r as a row Vector (Matrix with one row) and does a linear algebraic Matrix multiply c * r, yielding a Matrix whose number of rows is the number of components in c and whose number of columns is the number of components in r.
        /// </summary>
        public static iMatrix4 OuterProduct(iVector4 c, iVector4 r) => iVector4.OuterProduct(c, r);
        
        /// <summary>
        /// Returns a iVector4 from component-wise application of Add (lhs + rhs).
        /// </summary>
        public static iVector4 Add(iVector4 lhs, iVector4 rhs) => iVector4.Add(lhs, rhs);
        
        /// <summary>
        /// Returns a iVector4 from component-wise application of Sub (lhs - rhs).
        /// </summary>
        public static iVector4 Sub(iVector4 lhs, iVector4 rhs) => iVector4.Sub(lhs, rhs);
        
        /// <summary>
        /// Returns a iVector4 from component-wise application of Mul (lhs * rhs).
        /// </summary>
        public static iVector4 Mul(iVector4 lhs, iVector4 rhs) => iVector4.Mul(lhs, rhs);
        
        /// <summary>
        /// Returns a iVector4 from component-wise application of Div (lhs / rhs).
        /// </summary>
        public static iVector4 Div(iVector4 lhs, iVector4 rhs) => iVector4.Div(lhs, rhs);
        
        /// <summary>
        /// Returns a iVector4 from component-wise application of Xor (lhs ^ rhs).
        /// </summary>
        public static iVector4 Xor(iVector4 lhs, iVector4 rhs) => iVector4.Xor(lhs, rhs);
        
        /// <summary>
        /// Returns a iVector4 from component-wise application of BitwiseOr (lhs | rhs).
        /// </summary>
        public static iVector4 BitwiseOr(iVector4 lhs, iVector4 rhs) => iVector4.BitwiseOr(lhs, rhs);
        
        /// <summary>
        /// Returns a iVector4 from component-wise application of BitwiseAnd (lhs &amp; rhs).
        /// </summary>
        public static iVector4 BitwiseAnd(iVector4 lhs, iVector4 rhs) => iVector4.BitwiseAnd(lhs, rhs);
        
        /// <summary>
        /// Returns a iVector4 from component-wise application of LeftShift (lhs &lt;&lt; rhs).
        /// </summary>
        public static iVector4 LeftShift(iVector4 lhs, iVector4 rhs) => iVector4.LeftShift(lhs, rhs);
        
        /// <summary>
        /// Returns a iVector4 from component-wise application of RightShift (lhs &gt;&gt; rhs).
        /// </summary>
        public static iVector4 RightShift(iVector4 lhs, iVector4 rhs) => iVector4.RightShift(lhs, rhs);
        
        /// <summary>
        /// Returns the minimal component of this Vector.
        /// </summary>
        public static int MinElement(iVector4 v) => v.MinElement;
        
        /// <summary>
        /// Returns the maximal component of this Vector.
        /// </summary>
        public static int MaxElement(iVector4 v) => v.MaxElement;
        
        /// <summary>
        /// Returns the euclidean length of this Vector.
        /// </summary>
        public static float Length(iVector4 v) => v.Length;
        
        /// <summary>
        /// Returns the squared euclidean length of this Vector.
        /// </summary>
        public static float LengthSqr(iVector4 v) => v.LengthSqr;
        
        /// <summary>
        /// Returns the sum of all components.
        /// </summary>
        public static int Sum(iVector4 v) => v.Sum;
        
        /// <summary>
        /// Returns the euclidean norm of this Vector.
        /// </summary>
        public static float Norm(iVector4 v) => v.Norm;
        
        /// <summary>
        /// Returns the one-norm of this Vector.
        /// </summary>
        public static float Norm1(iVector4 v) => v.Norm1;
        
        /// <summary>
        /// Returns the two-norm (euclidean length) of this Vector.
        /// </summary>
        public static float Norm2(iVector4 v) => v.Norm2;
        
        /// <summary>
        /// Returns the max-norm of this Vector.
        /// </summary>
        public static float NormMax(iVector4 v) => v.NormMax;
        
        /// <summary>
        /// Returns the p-norm of this Vector.
        /// </summary>
        public static double NormP(iVector4 v, double p) => v.NormP(p);
        
        /// <summary>
        /// Returns the inner product (dot product, scalar product) of the two Vectors.
        /// </summary>
        public static int Dot(iVector4 lhs, iVector4 rhs) => iVector4.Dot(lhs, rhs);
        
        /// <summary>
        /// Returns the euclidean distance between the two Vectors.
        /// </summary>
        public static float Distance(iVector4 lhs, iVector4 rhs) => iVector4.Distance(lhs, rhs);
        
        /// <summary>
        /// Returns the squared euclidean distance between the two Vectors.
        /// </summary>
        public static float DistanceSqr(iVector4 lhs, iVector4 rhs) => iVector4.DistanceSqr(lhs, rhs);
        
        /// <summary>
        /// Calculate the reflection direction for an incident Vector (N should be normalized in order to achieve the desired result).
        /// </summary>
        public static iVector4 Reflect(iVector4 I, iVector4 N) => iVector4.Reflect(I, N);
        
        /// <summary>
        /// Calculate the refraction direction for an incident Vector (The input parameters I and N should be normalized in order to achieve the desired result).
        /// </summary>
        public static iVector4 Refract(iVector4 I, iVector4 N, int eta) => iVector4.Refract(I, N, eta);
        
        /// <summary>
        /// Returns a Vector pointing in the same direction as another (faceforward orients a Vector to point away from a surface as defined by its normal. If dot(Nref, I) is negative faceforward returns N, otherwise it returns -N).
        /// </summary>
        public static iVector4 FaceForward(iVector4 N, iVector4 I, iVector4 Nref) => iVector4.FaceForward(N, I, Nref);
        
        /// <summary>
        /// Returns a iVector4 with independent and identically distributed uniform integer values between 0 (inclusive) and maxValue (exclusive). (A maxValue of 0 is allowed and returns 0.)
        /// </summary>
        public static iVector4 Random(System.Random random, iVector4 maxValue) => iVector4.Random(random, maxValue);
        
        /// <summary>
        /// Returns a iVector4 with independent and identically distributed uniform integer values between minValue (inclusive) and maxValue (exclusive). (minValue == maxValue is allowed and returns minValue. Negative values are allowed.)
        /// </summary>
        public static iVector4 Random(System.Random random, iVector4 minValue, iVector4 maxValue) => iVector4.Random(random, minValue, maxValue);
        
        /// <summary>
        /// Returns a iVector4 with independent and identically distributed uniform integer values between minValue (inclusive) and maxValue (exclusive). (minValue == maxValue is allowed and returns minValue. Negative values are allowed.)
        /// </summary>
        public static iVector4 RandomUniform(System.Random random, iVector4 minValue, iVector4 maxValue) => iVector4.RandomUniform(random, minValue, maxValue);
        
        /// <summary>
        /// Returns a iVector4 with independent and identically distributed integer values according to a poisson distribution with given lambda parameter.
        /// </summary>
        public static iVector4 RandomPoisson(System.Random random, dVector4 lambda) => iVector4.RandomPoisson(random, lambda);

    }
}
