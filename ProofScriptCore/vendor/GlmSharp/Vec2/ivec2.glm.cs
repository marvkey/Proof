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
        public static swizzle_iVector2 swizzle(iVector2 v) => v.swizzle;
        
        /// <summary>
        /// Returns an array with all values
        /// </summary>
        public static int[] Values(iVector2 v) => v.Values;
        
        /// <summary>
        /// Returns an enumerator that iterates through all components.
        /// </summary>
        public static IEnumerator<int> GetEnumerator(iVector2 v) => v.GetEnumerator();
        
        /// <summary>
        /// Returns a string representation of this Vectortor using ', ' as a seperator.
        /// </summary>
        public static string ToString(iVector2 v) => v.ToString();
        
        /// <summary>
        /// Returns a string representation of this Vectortor using a provided seperator.
        /// </summary>
        public static string ToString(iVector2 v, string sep) => v.ToString(sep);
        
        /// <summary>
        /// Returns a string representation of this Vectortor using a provided seperator and a forMatrix provider for each component.
        /// </summary>
        public static string ToString(iVector2 v, string sep, IFormatProvider provider) => v.ToString(sep, provider);
        
        /// <summary>
        /// Returns a string representation of this Vectortor using a provided seperator and a forMatrix for each component.
        /// </summary>
        public static string ToString(iVector2 v, string sep, string forMatrix) => v.ToString(sep, forMatrix);
        
        /// <summary>
        /// Returns a string representation of this Vectortor using a provided seperator and a forMatrix and forMatrix provider for each component.
        /// </summary>
        public static string ToString(iVector2 v, string sep, string forMatrix, IFormatProvider provider) => v.ToString(sep, forMatrix, provider);
        
        /// <summary>
        /// Returns the number of components (2).
        /// </summary>
        public static int Count(iVector2 v) => v.Count;
        
        /// <summary>
        /// Returns true iff this equals rhs component-wise.
        /// </summary>
        public static bool Equals(iVector2 v, iVector2 rhs) => v.Equals(rhs);
        
        /// <summary>
        /// Returns true iff this equals rhs type- and component-wise.
        /// </summary>
        public static bool Equals(iVector2 v, object obj) => v.Equals(obj);
        
        /// <summary>
        /// Returns a hash code for this instance.
        /// </summary>
        public static int GetHashCode(iVector2 v) => v.GetHashCode();
        
        /// <summary>
        /// Returns a bVector2 from component-wise application of Equal (lhs == rhs).
        /// </summary>
        public static bVector2 Equal(iVector2 lhs, iVector2 rhs) => iVector2.Equal(lhs, rhs);
        
        /// <summary>
        /// Returns a bVector2 from component-wise application of NotEqual (lhs != rhs).
        /// </summary>
        public static bVector2 NotEqual(iVector2 lhs, iVector2 rhs) => iVector2.NotEqual(lhs, rhs);
        
        /// <summary>
        /// Returns a bVector2 from component-wise application of GreaterThan (lhs &gt; rhs).
        /// </summary>
        public static bVector2 GreaterThan(iVector2 lhs, iVector2 rhs) => iVector2.GreaterThan(lhs, rhs);
        
        /// <summary>
        /// Returns a bVector2 from component-wise application of GreaterThanEqual (lhs &gt;= rhs).
        /// </summary>
        public static bVector2 GreaterThanEqual(iVector2 lhs, iVector2 rhs) => iVector2.GreaterThanEqual(lhs, rhs);
        
        /// <summary>
        /// Returns a bVector2 from component-wise application of LesserThan (lhs &lt; rhs).
        /// </summary>
        public static bVector2 LesserThan(iVector2 lhs, iVector2 rhs) => iVector2.LesserThan(lhs, rhs);
        
        /// <summary>
        /// Returns a bVector2 from component-wise application of LesserThanEqual (lhs &lt;= rhs).
        /// </summary>
        public static bVector2 LesserThanEqual(iVector2 lhs, iVector2 rhs) => iVector2.LesserThanEqual(lhs, rhs);
        
        /// <summary>
        /// Returns a iVector2 from component-wise application of Abs (Math.Abs(v)).
        /// </summary>
        public static iVector2 Abs(iVector2 v) => iVector2.Abs(v);
        
        /// <summary>
        /// Returns a iVector2 from component-wise application of HermiteInterpolationOrder3 ((3 - 2 * v) * v * v).
        /// </summary>
        public static iVector2 HermiteInterpolationOrder3(iVector2 v) => iVector2.HermiteInterpolationOrder3(v);
        
        /// <summary>
        /// Returns a iVector2 from component-wise application of HermiteInterpolationOrder5 (((6 * v - 15) * v + 10) * v * v * v).
        /// </summary>
        public static iVector2 HermiteInterpolationOrder5(iVector2 v) => iVector2.HermiteInterpolationOrder5(v);
        
        /// <summary>
        /// Returns a iVector2 from component-wise application of Sqr (v * v).
        /// </summary>
        public static iVector2 Sqr(iVector2 v) => iVector2.Sqr(v);
        
        /// <summary>
        /// Returns a iVector2 from component-wise application of Pow2 (v * v).
        /// </summary>
        public static iVector2 Pow2(iVector2 v) => iVector2.Pow2(v);
        
        /// <summary>
        /// Returns a iVector2 from component-wise application of Pow3 (v * v * v).
        /// </summary>
        public static iVector2 Pow3(iVector2 v) => iVector2.Pow3(v);
        
        /// <summary>
        /// Returns a iVector2 from component-wise application of Step (v &gt;= 0 ? 1 : 0).
        /// </summary>
        public static iVector2 Step(iVector2 v) => iVector2.Step(v);
        
        /// <summary>
        /// Returns a iVector2 from component-wise application of Sqrt ((int)Math.Sqrt((double)v)).
        /// </summary>
        public static iVector2 Sqrt(iVector2 v) => iVector2.Sqrt(v);
        
        /// <summary>
        /// Returns a iVector2 from component-wise application of InverseSqrt ((int)(1.0 / Math.Sqrt((double)v))).
        /// </summary>
        public static iVector2 InverseSqrt(iVector2 v) => iVector2.InverseSqrt(v);
        
        /// <summary>
        /// Returns a iVector2 from component-wise application of Sign (Math.Sign(v)).
        /// </summary>
        public static iVector2 Sign(iVector2 v) => iVector2.Sign(v);
        
        /// <summary>
        /// Returns a iVector2 from component-wise application of Max (Math.Max(lhs, rhs)).
        /// </summary>
        public static iVector2 Max(iVector2 lhs, iVector2 rhs) => iVector2.Max(lhs, rhs);
        
        /// <summary>
        /// Returns a iVector2 from component-wise application of Min (Math.Min(lhs, rhs)).
        /// </summary>
        public static iVector2 Min(iVector2 lhs, iVector2 rhs) => iVector2.Min(lhs, rhs);
        
        /// <summary>
        /// Returns a iVector2 from component-wise application of Pow ((int)Math.Pow((double)lhs, (double)rhs)).
        /// </summary>
        public static iVector2 Pow(iVector2 lhs, iVector2 rhs) => iVector2.Pow(lhs, rhs);
        
        /// <summary>
        /// Returns a iVector2 from component-wise application of Log ((int)Math.Log((double)lhs, (double)rhs)).
        /// </summary>
        public static iVector2 Log(iVector2 lhs, iVector2 rhs) => iVector2.Log(lhs, rhs);
        
        /// <summary>
        /// Returns a iVector2 from component-wise application of Clamp (Math.Min(Math.Max(v, min), max)).
        /// </summary>
        public static iVector2 Clamp(iVector2 v, iVector2 min, iVector2 max) => iVector2.Clamp(v, min, max);
        
        /// <summary>
        /// Returns a iVector2 from component-wise application of Mix (min * (1-a) + max * a).
        /// </summary>
        public static iVector2 Mix(iVector2 min, iVector2 max, iVector2 a) => iVector2.Mix(min, max, a);
        
        /// <summary>
        /// Returns a iVector2 from component-wise application of Lerp (min * (1-a) + max * a).
        /// </summary>
        public static iVector2 Lerp(iVector2 min, iVector2 max, iVector2 a) => iVector2.Lerp(min, max, a);
        
        /// <summary>
        /// Returns a iVector2 from component-wise application of Smoothstep (((v - edge0) / (edge1 - edge0)).Clamp().HermiteInterpolationOrder3()).
        /// </summary>
        public static iVector2 Smoothstep(iVector2 edge0, iVector2 edge1, iVector2 v) => iVector2.Smoothstep(edge0, edge1, v);
        
        /// <summary>
        /// Returns a iVector2 from component-wise application of Smootherstep (((v - edge0) / (edge1 - edge0)).Clamp().HermiteInterpolationOrder5()).
        /// </summary>
        public static iVector2 Smootherstep(iVector2 edge0, iVector2 edge1, iVector2 v) => iVector2.Smootherstep(edge0, edge1, v);
        
        /// <summary>
        /// Returns a iVector2 from component-wise application of Fma (a * b + c).
        /// </summary>
        public static iVector2 Fma(iVector2 a, iVector2 b, iVector2 c) => iVector2.Fma(a, b, c);
        
        /// <summary>
        /// OuterProduct treats the first parameter c as a column Vectortor (Matrix with one column) and the second parameter r as a row Vectortor (Matrix with one row) and does a linear algebraic Matrix multiply c * r, yielding a Matrix whose number of rows is the number of components in c and whose number of columns is the number of components in r.
        /// </summary>
        public static iMatrix2 OuterProduct(iVector2 c, iVector2 r) => iVector2.OuterProduct(c, r);
        
        /// <summary>
        /// OuterProduct treats the first parameter c as a column Vectortor (Matrix with one column) and the second parameter r as a row Vectortor (Matrix with one row) and does a linear algebraic Matrix multiply c * r, yielding a Matrix whose number of rows is the number of components in c and whose number of columns is the number of components in r.
        /// </summary>
        public static iMatrix3x2 OuterProduct(iVector2 c, iVector3 r) => iVector2.OuterProduct(c, r);
        
        /// <summary>
        /// OuterProduct treats the first parameter c as a column Vectortor (Matrix with one column) and the second parameter r as a row Vectortor (Matrix with one row) and does a linear algebraic Matrix multiply c * r, yielding a Matrix whose number of rows is the number of components in c and whose number of columns is the number of components in r.
        /// </summary>
        public static iMatrix4x2 OuterProduct(iVector2 c, iVector4 r) => iVector2.OuterProduct(c, r);
        
        /// <summary>
        /// Returns a iVector2 from component-wise application of Add (lhs + rhs).
        /// </summary>
        public static iVector2 Add(iVector2 lhs, iVector2 rhs) => iVector2.Add(lhs, rhs);
        
        /// <summary>
        /// Returns a iVector2 from component-wise application of Sub (lhs - rhs).
        /// </summary>
        public static iVector2 Sub(iVector2 lhs, iVector2 rhs) => iVector2.Sub(lhs, rhs);
        
        /// <summary>
        /// Returns a iVector2 from component-wise application of Mul (lhs * rhs).
        /// </summary>
        public static iVector2 Mul(iVector2 lhs, iVector2 rhs) => iVector2.Mul(lhs, rhs);
        
        /// <summary>
        /// Returns a iVector2 from component-wise application of Div (lhs / rhs).
        /// </summary>
        public static iVector2 Div(iVector2 lhs, iVector2 rhs) => iVector2.Div(lhs, rhs);
        
        /// <summary>
        /// Returns a iVector2 from component-wise application of Xor (lhs ^ rhs).
        /// </summary>
        public static iVector2 Xor(iVector2 lhs, iVector2 rhs) => iVector2.Xor(lhs, rhs);
        
        /// <summary>
        /// Returns a iVector2 from component-wise application of BitwiseOr (lhs | rhs).
        /// </summary>
        public static iVector2 BitwiseOr(iVector2 lhs, iVector2 rhs) => iVector2.BitwiseOr(lhs, rhs);
        
        /// <summary>
        /// Returns a iVector2 from component-wise application of BitwiseAnd (lhs &amp; rhs).
        /// </summary>
        public static iVector2 BitwiseAnd(iVector2 lhs, iVector2 rhs) => iVector2.BitwiseAnd(lhs, rhs);
        
        /// <summary>
        /// Returns a iVector2 from component-wise application of LeftShift (lhs &lt;&lt; rhs).
        /// </summary>
        public static iVector2 LeftShift(iVector2 lhs, iVector2 rhs) => iVector2.LeftShift(lhs, rhs);
        
        /// <summary>
        /// Returns a iVector2 from component-wise application of RightShift (lhs &gt;&gt; rhs).
        /// </summary>
        public static iVector2 RightShift(iVector2 lhs, iVector2 rhs) => iVector2.RightShift(lhs, rhs);
        
        /// <summary>
        /// Returns the minimal component of this Vectortor.
        /// </summary>
        public static int MinElement(iVector2 v) => v.MinElement;
        
        /// <summary>
        /// Returns the maximal component of this Vectortor.
        /// </summary>
        public static int MaxElement(iVector2 v) => v.MaxElement;
        
        /// <summary>
        /// Returns the euclidean length of this Vectortor.
        /// </summary>
        public static float Length(iVector2 v) => v.Length;
        
        /// <summary>
        /// Returns the squared euclidean length of this Vectortor.
        /// </summary>
        public static float LengthSqr(iVector2 v) => v.LengthSqr;
        
        /// <summary>
        /// Returns the sum of all components.
        /// </summary>
        public static int Sum(iVector2 v) => v.Sum;
        
        /// <summary>
        /// Returns the euclidean norm of this Vectortor.
        /// </summary>
        public static float Norm(iVector2 v) => v.Norm;
        
        /// <summary>
        /// Returns the one-norm of this Vectortor.
        /// </summary>
        public static float Norm1(iVector2 v) => v.Norm1;
        
        /// <summary>
        /// Returns the two-norm (euclidean length) of this Vectortor.
        /// </summary>
        public static float Norm2(iVector2 v) => v.Norm2;
        
        /// <summary>
        /// Returns the max-norm of this Vectortor.
        /// </summary>
        public static float NormMax(iVector2 v) => v.NormMax;
        
        /// <summary>
        /// Returns the p-norm of this Vectortor.
        /// </summary>
        public static double NormP(iVector2 v, double p) => v.NormP(p);
        
        /// <summary>
        /// Returns the inner product (dot product, scalar product) of the two Vectortors.
        /// </summary>
        public static int Dot(iVector2 lhs, iVector2 rhs) => iVector2.Dot(lhs, rhs);
        
        /// <summary>
        /// Returns the euclidean distance between the two Vectortors.
        /// </summary>
        public static float Distance(iVector2 lhs, iVector2 rhs) => iVector2.Distance(lhs, rhs);
        
        /// <summary>
        /// Returns the squared euclidean distance between the two Vectortors.
        /// </summary>
        public static float DistanceSqr(iVector2 lhs, iVector2 rhs) => iVector2.DistanceSqr(lhs, rhs);
        
        /// <summary>
        /// Calculate the reflection direction for an incident Vectortor (N should be normalized in order to achieve the desired result).
        /// </summary>
        public static iVector2 Reflect(iVector2 I, iVector2 N) => iVector2.Reflect(I, N);
        
        /// <summary>
        /// Calculate the refraction direction for an incident Vectortor (The input parameters I and N should be normalized in order to achieve the desired result).
        /// </summary>
        public static iVector2 Refract(iVector2 I, iVector2 N, int eta) => iVector2.Refract(I, N, eta);
        
        /// <summary>
        /// Returns a Vectortor pointing in the same direction as another (faceforward orients a Vectortor to point away from a surface as defined by its normal. If dot(Nref, I) is negative faceforward returns N, otherwise it returns -N).
        /// </summary>
        public static iVector2 FaceForward(iVector2 N, iVector2 I, iVector2 Nref) => iVector2.FaceForward(N, I, Nref);
        
        /// <summary>
        /// Returns the length of the outer product (cross product, Vectortor product) of the two Vectortors.
        /// </summary>
        public static int Cross(iVector2 l, iVector2 r) => iVector2.Cross(l, r);
        
        /// <summary>
        /// Returns a iVector2 with independent and identically distributed uniform integer values between 0 (inclusive) and maxValue (exclusive). (A maxValue of 0 is allowed and returns 0.)
        /// </summary>
        public static iVector2 Random(System.Random random, iVector2 maxValue) => iVector2.Random(random, maxValue);
        
        /// <summary>
        /// Returns a iVector2 with independent and identically distributed uniform integer values between minValue (inclusive) and maxValue (exclusive). (minValue == maxValue is allowed and returns minValue. Negative values are allowed.)
        /// </summary>
        public static iVector2 Random(System.Random random, iVector2 minValue, iVector2 maxValue) => iVector2.Random(random, minValue, maxValue);
        
        /// <summary>
        /// Returns a iVector2 with independent and identically distributed uniform integer values between minValue (inclusive) and maxValue (exclusive). (minValue == maxValue is allowed and returns minValue. Negative values are allowed.)
        /// </summary>
        public static iVector2 RandomUniform(System.Random random, iVector2 minValue, iVector2 maxValue) => iVector2.RandomUniform(random, minValue, maxValue);
        
        /// <summary>
        /// Returns a iVector2 with independent and identically distributed integer values according to a poisson distribution with given lambda parameter.
        /// </summary>
        public static iVector2 RandomPoisson(System.Random random, dVector2 lambda) => iVector2.RandomPoisson(random, lambda);

    }
}
