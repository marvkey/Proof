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
        public static swizzle_iVector3 swizzle(iVector3 v) => v.swizzle;
        
        /// <summary>
        /// Returns an array with all values
        /// </summary>
        public static int[] Values(iVector3 v) => v.Values;
        
        /// <summary>
        /// Returns an enumerator that iterates through all components.
        /// </summary>
        public static IEnumerator<int> GetEnumerator(iVector3 v) => v.GetEnumerator();
        
        /// <summary>
        /// Returns a string representation of this Vectortor using ', ' as a seperator.
        /// </summary>
        public static string ToString(iVector3 v) => v.ToString();
        
        /// <summary>
        /// Returns a string representation of this Vectortor using a provided seperator.
        /// </summary>
        public static string ToString(iVector3 v, string sep) => v.ToString(sep);
        
        /// <summary>
        /// Returns a string representation of this Vectortor using a provided seperator and a forMatrix provider for each component.
        /// </summary>
        public static string ToString(iVector3 v, string sep, IFormatProvider provider) => v.ToString(sep, provider);
        
        /// <summary>
        /// Returns a string representation of this Vectortor using a provided seperator and a forMatrix for each component.
        /// </summary>
        public static string ToString(iVector3 v, string sep, string forMatrix) => v.ToString(sep, forMatrix);
        
        /// <summary>
        /// Returns a string representation of this Vectortor using a provided seperator and a forMatrix and forMatrix provider for each component.
        /// </summary>
        public static string ToString(iVector3 v, string sep, string forMatrix, IFormatProvider provider) => v.ToString(sep, forMatrix, provider);
        
        /// <summary>
        /// Returns the number of components (3).
        /// </summary>
        public static int Count(iVector3 v) => v.Count;
        
        /// <summary>
        /// Returns true iff this equals rhs component-wise.
        /// </summary>
        public static bool Equals(iVector3 v, iVector3 rhs) => v.Equals(rhs);
        
        /// <summary>
        /// Returns true iff this equals rhs type- and component-wise.
        /// </summary>
        public static bool Equals(iVector3 v, object obj) => v.Equals(obj);
        
        /// <summary>
        /// Returns a hash code for this instance.
        /// </summary>
        public static int GetHashCode(iVector3 v) => v.GetHashCode();
        
        /// <summary>
        /// Returns a bVector3 from component-wise application of Equal (lhs == rhs).
        /// </summary>
        public static bVector3 Equal(iVector3 lhs, iVector3 rhs) => iVector3.Equal(lhs, rhs);
        
        /// <summary>
        /// Returns a bVector3 from component-wise application of Equal (lhs == rhs).
        /// </summary>
        public static bool Equal(int lhs, int rhs) => lhs == rhs;
        
        /// <summary>
        /// Returns a bVector3 from component-wise application of NotEqual (lhs != rhs).
        /// </summary>
        public static bVector3 NotEqual(iVector3 lhs, iVector3 rhs) => iVector3.NotEqual(lhs, rhs);
        
        /// <summary>
        /// Returns a bVector3 from component-wise application of NotEqual (lhs != rhs).
        /// </summary>
        public static bool NotEqual(int lhs, int rhs) => lhs != rhs;
        
        /// <summary>
        /// Returns a bVector3 from component-wise application of GreaterThan (lhs &gt; rhs).
        /// </summary>
        public static bVector3 GreaterThan(iVector3 lhs, iVector3 rhs) => iVector3.GreaterThan(lhs, rhs);
        
        /// <summary>
        /// Returns a bVector3 from component-wise application of GreaterThan (lhs &gt; rhs).
        /// </summary>
        public static bool GreaterThan(int lhs, int rhs) => lhs > rhs;
        
        /// <summary>
        /// Returns a bVector3 from component-wise application of GreaterThanEqual (lhs &gt;= rhs).
        /// </summary>
        public static bVector3 GreaterThanEqual(iVector3 lhs, iVector3 rhs) => iVector3.GreaterThanEqual(lhs, rhs);
        
        /// <summary>
        /// Returns a bVector3 from component-wise application of GreaterThanEqual (lhs &gt;= rhs).
        /// </summary>
        public static bool GreaterThanEqual(int lhs, int rhs) => lhs >= rhs;
        
        /// <summary>
        /// Returns a bVector3 from component-wise application of LesserThan (lhs &lt; rhs).
        /// </summary>
        public static bVector3 LesserThan(iVector3 lhs, iVector3 rhs) => iVector3.LesserThan(lhs, rhs);
        
        /// <summary>
        /// Returns a bVector3 from component-wise application of LesserThan (lhs &lt; rhs).
        /// </summary>
        public static bool LesserThan(int lhs, int rhs) => lhs < rhs;
        
        /// <summary>
        /// Returns a bVector3 from component-wise application of LesserThanEqual (lhs &lt;= rhs).
        /// </summary>
        public static bVector3 LesserThanEqual(iVector3 lhs, iVector3 rhs) => iVector3.LesserThanEqual(lhs, rhs);
        
        /// <summary>
        /// Returns a bVector3 from component-wise application of LesserThanEqual (lhs &lt;= rhs).
        /// </summary>
        public static bool LesserThanEqual(int lhs, int rhs) => lhs <= rhs;
        
        /// <summary>
        /// Returns a iVector3 from component-wise application of Abs (Math.Abs(v)).
        /// </summary>
        public static iVector3 Abs(iVector3 v) => iVector3.Abs(v);
        
        /// <summary>
        /// Returns a iVector3 from component-wise application of Abs (Math.Abs(v)).
        /// </summary>
        public static int Abs(int v) => Math.Abs(v);
        
        /// <summary>
        /// Returns a iVector3 from component-wise application of HermiteInterpolationOrder3 ((3 - 2 * v) * v * v).
        /// </summary>
        public static iVector3 HermiteInterpolationOrder3(iVector3 v) => iVector3.HermiteInterpolationOrder3(v);
        
        /// <summary>
        /// Returns a iVector3 from component-wise application of HermiteInterpolationOrder3 ((3 - 2 * v) * v * v).
        /// </summary>
        public static int HermiteInterpolationOrder3(int v) => (3 - 2 * v) * v * v;
        
        /// <summary>
        /// Returns a iVector3 from component-wise application of HermiteInterpolationOrder5 (((6 * v - 15) * v + 10) * v * v * v).
        /// </summary>
        public static iVector3 HermiteInterpolationOrder5(iVector3 v) => iVector3.HermiteInterpolationOrder5(v);
        
        /// <summary>
        /// Returns a iVector3 from component-wise application of HermiteInterpolationOrder5 (((6 * v - 15) * v + 10) * v * v * v).
        /// </summary>
        public static int HermiteInterpolationOrder5(int v) => ((6 * v - 15) * v + 10) * v * v * v;
        
        /// <summary>
        /// Returns a iVector3 from component-wise application of Sqr (v * v).
        /// </summary>
        public static iVector3 Sqr(iVector3 v) => iVector3.Sqr(v);
        
        /// <summary>
        /// Returns a iVector3 from component-wise application of Sqr (v * v).
        /// </summary>
        public static int Sqr(int v) => v * v;
        
        /// <summary>
        /// Returns a iVector3 from component-wise application of Pow2 (v * v).
        /// </summary>
        public static iVector3 Pow2(iVector3 v) => iVector3.Pow2(v);
        
        /// <summary>
        /// Returns a iVector3 from component-wise application of Pow2 (v * v).
        /// </summary>
        public static int Pow2(int v) => v * v;
        
        /// <summary>
        /// Returns a iVector3 from component-wise application of Pow3 (v * v * v).
        /// </summary>
        public static iVector3 Pow3(iVector3 v) => iVector3.Pow3(v);
        
        /// <summary>
        /// Returns a iVector3 from component-wise application of Pow3 (v * v * v).
        /// </summary>
        public static int Pow3(int v) => v * v * v;
        
        /// <summary>
        /// Returns a iVector3 from component-wise application of Step (v &gt;= 0 ? 1 : 0).
        /// </summary>
        public static iVector3 Step(iVector3 v) => iVector3.Step(v);
        
        /// <summary>
        /// Returns a iVector3 from component-wise application of Step (v &gt;= 0 ? 1 : 0).
        /// </summary>
        public static int Step(int v) => v >= 0 ? 1 : 0;
        
        /// <summary>
        /// Returns a iVector3 from component-wise application of Sqrt ((int)Math.Sqrt((double)v)).
        /// </summary>
        public static iVector3 Sqrt(iVector3 v) => iVector3.Sqrt(v);
        
        /// <summary>
        /// Returns a iVector3 from component-wise application of Sqrt ((int)Math.Sqrt((double)v)).
        /// </summary>
        public static int Sqrt(int v) => (int)Math.Sqrt((double)v);
        
        /// <summary>
        /// Returns a iVector3 from component-wise application of InverseSqrt ((int)(1.0 / Math.Sqrt((double)v))).
        /// </summary>
        public static iVector3 InverseSqrt(iVector3 v) => iVector3.InverseSqrt(v);
        
        /// <summary>
        /// Returns a iVector3 from component-wise application of InverseSqrt ((int)(1.0 / Math.Sqrt((double)v))).
        /// </summary>
        public static int InverseSqrt(int v) => (int)(1.0 / Math.Sqrt((double)v));
        
        /// <summary>
        /// Returns a iVector3 from component-wise application of Sign (Math.Sign(v)).
        /// </summary>
        public static iVector3 Sign(iVector3 v) => iVector3.Sign(v);
        
        /// <summary>
        /// Returns a iVector3 from component-wise application of Sign (Math.Sign(v)).
        /// </summary>
        public static int Sign(int v) => Math.Sign(v);
        
        /// <summary>
        /// Returns a iVector3 from component-wise application of Max (Math.Max(lhs, rhs)).
        /// </summary>
        public static iVector3 Max(iVector3 lhs, iVector3 rhs) => iVector3.Max(lhs, rhs);
        
        /// <summary>
        /// Returns a iVector3 from component-wise application of Max (Math.Max(lhs, rhs)).
        /// </summary>
        public static int Max(int lhs, int rhs) => Math.Max(lhs, rhs);
        
        /// <summary>
        /// Returns a iVector3 from component-wise application of Min (Math.Min(lhs, rhs)).
        /// </summary>
        public static iVector3 Min(iVector3 lhs, iVector3 rhs) => iVector3.Min(lhs, rhs);
        
        /// <summary>
        /// Returns a iVector3 from component-wise application of Min (Math.Min(lhs, rhs)).
        /// </summary>
        public static int Min(int lhs, int rhs) => Math.Min(lhs, rhs);
        
        /// <summary>
        /// Returns a iVector3 from component-wise application of Pow ((int)Math.Pow((double)lhs, (double)rhs)).
        /// </summary>
        public static iVector3 Pow(iVector3 lhs, iVector3 rhs) => iVector3.Pow(lhs, rhs);
        
        /// <summary>
        /// Returns a iVector3 from component-wise application of Pow ((int)Math.Pow((double)lhs, (double)rhs)).
        /// </summary>
        public static int Pow(int lhs, int rhs) => (int)Math.Pow((double)lhs, (double)rhs);
        
        /// <summary>
        /// Returns a iVector3 from component-wise application of Log ((int)Math.Log((double)lhs, (double)rhs)).
        /// </summary>
        public static iVector3 Log(iVector3 lhs, iVector3 rhs) => iVector3.Log(lhs, rhs);
        
        /// <summary>
        /// Returns a iVector3 from component-wise application of Log ((int)Math.Log((double)lhs, (double)rhs)).
        /// </summary>
        public static int Log(int lhs, int rhs) => (int)Math.Log((double)lhs, (double)rhs);
        
        /// <summary>
        /// Returns a iVector3 from component-wise application of Clamp (Math.Min(Math.Max(v, min), max)).
        /// </summary>
        public static iVector3 Clamp(iVector3 v, iVector3 min, iVector3 max) => iVector3.Clamp(v, min, max);
        
        /// <summary>
        /// Returns a iVector3 from component-wise application of Clamp (Math.Min(Math.Max(v, min), max)).
        /// </summary>
        public static int Clamp(int v, int min, int max) => Math.Min(Math.Max(v, min), max);
        
        /// <summary>
        /// Returns a iVector3 from component-wise application of Mix (min * (1-a) + max * a).
        /// </summary>
        public static iVector3 Mix(iVector3 min, iVector3 max, iVector3 a) => iVector3.Mix(min, max, a);
        
        /// <summary>
        /// Returns a iVector3 from component-wise application of Mix (min * (1-a) + max * a).
        /// </summary>
        public static int Mix(int min, int max, int a) => min * (1-a) + max * a;
        
        /// <summary>
        /// Returns a iVector3 from component-wise application of Lerp (min * (1-a) + max * a).
        /// </summary>
        public static iVector3 Lerp(iVector3 min, iVector3 max, iVector3 a) => iVector3.Lerp(min, max, a);
        
        /// <summary>
        /// Returns a iVector3 from component-wise application of Lerp (min * (1-a) + max * a).
        /// </summary>
        public static int Lerp(int min, int max, int a) => min * (1-a) + max * a;
        
        /// <summary>
        /// Returns a iVector3 from component-wise application of Smoothstep (((v - edge0) / (edge1 - edge0)).Clamp().HermiteInterpolationOrder3()).
        /// </summary>
        public static iVector3 Smoothstep(iVector3 edge0, iVector3 edge1, iVector3 v) => iVector3.Smoothstep(edge0, edge1, v);
        
        /// <summary>
        /// Returns a iVector3 from component-wise application of Smoothstep (((v - edge0) / (edge1 - edge0)).Clamp().HermiteInterpolationOrder3()).
        /// </summary>
        public static int Smoothstep(int edge0, int edge1, int v) => ((v - edge0) / (edge1 - edge0)).Clamp().HermiteInterpolationOrder3();
        
        /// <summary>
        /// Returns a iVector3 from component-wise application of Smootherstep (((v - edge0) / (edge1 - edge0)).Clamp().HermiteInterpolationOrder5()).
        /// </summary>
        public static iVector3 Smootherstep(iVector3 edge0, iVector3 edge1, iVector3 v) => iVector3.Smootherstep(edge0, edge1, v);
        
        /// <summary>
        /// Returns a iVector3 from component-wise application of Smootherstep (((v - edge0) / (edge1 - edge0)).Clamp().HermiteInterpolationOrder5()).
        /// </summary>
        public static int Smootherstep(int edge0, int edge1, int v) => ((v - edge0) / (edge1 - edge0)).Clamp().HermiteInterpolationOrder5();
        
        /// <summary>
        /// Returns a iVector3 from component-wise application of Fma (a * b + c).
        /// </summary>
        public static iVector3 Fma(iVector3 a, iVector3 b, iVector3 c) => iVector3.Fma(a, b, c);
        
        /// <summary>
        /// Returns a iVector3 from component-wise application of Fma (a * b + c).
        /// </summary>
        public static int Fma(int a, int b, int c) => a * b + c;
        
        /// <summary>
        /// OuterProduct treats the first parameter c as a column Vectortor (Matrix with one column) and the second parameter r as a row Vectortor (Matrix with one row) and does a linear algebraic Matrix multiply c * r, yielding a Matrix whose number of rows is the number of components in c and whose number of columns is the number of components in r.
        /// </summary>
        public static iMatrix2x3 OuterProduct(iVector3 c, iVector2 r) => iVector3.OuterProduct(c, r);
        
        /// <summary>
        /// OuterProduct treats the first parameter c as a column Vectortor (Matrix with one column) and the second parameter r as a row Vectortor (Matrix with one row) and does a linear algebraic Matrix multiply c * r, yielding a Matrix whose number of rows is the number of components in c and whose number of columns is the number of components in r.
        /// </summary>
        public static iMatrix3 OuterProduct(iVector3 c, iVector3 r) => iVector3.OuterProduct(c, r);
        
        /// <summary>
        /// OuterProduct treats the first parameter c as a column Vectortor (Matrix with one column) and the second parameter r as a row Vectortor (Matrix with one row) and does a linear algebraic Matrix multiply c * r, yielding a Matrix whose number of rows is the number of components in c and whose number of columns is the number of components in r.
        /// </summary>
        public static iMatrix4x3 OuterProduct(iVector3 c, iVector4 r) => iVector3.OuterProduct(c, r);
        
        /// <summary>
        /// Returns a iVector3 from component-wise application of Add (lhs + rhs).
        /// </summary>
        public static iVector3 Add(iVector3 lhs, iVector3 rhs) => iVector3.Add(lhs, rhs);
        
        /// <summary>
        /// Returns a iVector3 from component-wise application of Add (lhs + rhs).
        /// </summary>
        public static int Add(int lhs, int rhs) => lhs + rhs;
        
        /// <summary>
        /// Returns a iVector3 from component-wise application of Sub (lhs - rhs).
        /// </summary>
        public static iVector3 Sub(iVector3 lhs, iVector3 rhs) => iVector3.Sub(lhs, rhs);
        
        /// <summary>
        /// Returns a iVector3 from component-wise application of Sub (lhs - rhs).
        /// </summary>
        public static int Sub(int lhs, int rhs) => lhs - rhs;
        
        /// <summary>
        /// Returns a iVector3 from component-wise application of Mul (lhs * rhs).
        /// </summary>
        public static iVector3 Mul(iVector3 lhs, iVector3 rhs) => iVector3.Mul(lhs, rhs);
        
        /// <summary>
        /// Returns a iVector3 from component-wise application of Mul (lhs * rhs).
        /// </summary>
        public static int Mul(int lhs, int rhs) => lhs * rhs;
        
        /// <summary>
        /// Returns a iVector3 from component-wise application of Div (lhs / rhs).
        /// </summary>
        public static iVector3 Div(iVector3 lhs, iVector3 rhs) => iVector3.Div(lhs, rhs);
        
        /// <summary>
        /// Returns a iVector3 from component-wise application of Div (lhs / rhs).
        /// </summary>
        public static int Div(int lhs, int rhs) => lhs / rhs;
        
        /// <summary>
        /// Returns a iVector3 from component-wise application of Xor (lhs ^ rhs).
        /// </summary>
        public static iVector3 Xor(iVector3 lhs, iVector3 rhs) => iVector3.Xor(lhs, rhs);
        
        /// <summary>
        /// Returns a iVector3 from component-wise application of Xor (lhs ^ rhs).
        /// </summary>
        public static int Xor(int lhs, int rhs) => lhs ^ rhs;
        
        /// <summary>
        /// Returns a iVector3 from component-wise application of BitwiseOr (lhs | rhs).
        /// </summary>
        public static iVector3 BitwiseOr(iVector3 lhs, iVector3 rhs) => iVector3.BitwiseOr(lhs, rhs);
        
        /// <summary>
        /// Returns a iVector3 from component-wise application of BitwiseOr (lhs | rhs).
        /// </summary>
        public static int BitwiseOr(int lhs, int rhs) => lhs | rhs;
        
        /// <summary>
        /// Returns a iVector3 from component-wise application of BitwiseAnd (lhs &amp; rhs).
        /// </summary>
        public static iVector3 BitwiseAnd(iVector3 lhs, iVector3 rhs) => iVector3.BitwiseAnd(lhs, rhs);
        
        /// <summary>
        /// Returns a iVector3 from component-wise application of BitwiseAnd (lhs &amp; rhs).
        /// </summary>
        public static int BitwiseAnd(int lhs, int rhs) => lhs & rhs;
        
        /// <summary>
        /// Returns a iVector3 from component-wise application of LeftShift (lhs &lt;&lt; rhs).
        /// </summary>
        public static iVector3 LeftShift(iVector3 lhs, iVector3 rhs) => iVector3.LeftShift(lhs, rhs);
        
        /// <summary>
        /// Returns a iVector3 from component-wise application of LeftShift (lhs &lt;&lt; rhs).
        /// </summary>
        public static int LeftShift(int lhs, int rhs) => lhs << rhs;
        
        /// <summary>
        /// Returns a iVector3 from component-wise application of RightShift (lhs &gt;&gt; rhs).
        /// </summary>
        public static iVector3 RightShift(iVector3 lhs, iVector3 rhs) => iVector3.RightShift(lhs, rhs);
        
        /// <summary>
        /// Returns a iVector3 from component-wise application of RightShift (lhs &gt;&gt; rhs).
        /// </summary>
        public static int RightShift(int lhs, int rhs) => lhs >> rhs;
        
        /// <summary>
        /// Returns the minimal component of this Vectortor.
        /// </summary>
        public static int MinElement(iVector3 v) => v.MinElement;
        
        /// <summary>
        /// Returns the maximal component of this Vectortor.
        /// </summary>
        public static int MaxElement(iVector3 v) => v.MaxElement;
        
        /// <summary>
        /// Returns the euclidean length of this Vectortor.
        /// </summary>
        public static float Length(iVector3 v) => v.Length;
        
        /// <summary>
        /// Returns the squared euclidean length of this Vectortor.
        /// </summary>
        public static float LengthSqr(iVector3 v) => v.LengthSqr;
        
        /// <summary>
        /// Returns the sum of all components.
        /// </summary>
        public static int Sum(iVector3 v) => v.Sum;
        
        /// <summary>
        /// Returns the euclidean norm of this Vectortor.
        /// </summary>
        public static float Norm(iVector3 v) => v.Norm;
        
        /// <summary>
        /// Returns the one-norm of this Vectortor.
        /// </summary>
        public static float Norm1(iVector3 v) => v.Norm1;
        
        /// <summary>
        /// Returns the two-norm (euclidean length) of this Vectortor.
        /// </summary>
        public static float Norm2(iVector3 v) => v.Norm2;
        
        /// <summary>
        /// Returns the max-norm of this Vectortor.
        /// </summary>
        public static float NormMax(iVector3 v) => v.NormMax;
        
        /// <summary>
        /// Returns the p-norm of this Vectortor.
        /// </summary>
        public static double NormP(iVector3 v, double p) => v.NormP(p);
        
        /// <summary>
        /// Returns the inner product (dot product, scalar product) of the two Vectortors.
        /// </summary>
        public static int Dot(iVector3 lhs, iVector3 rhs) => iVector3.Dot(lhs, rhs);
        
        /// <summary>
        /// Returns the euclidean distance between the two Vectortors.
        /// </summary>
        public static float Distance(iVector3 lhs, iVector3 rhs) => iVector3.Distance(lhs, rhs);
        
        /// <summary>
        /// Returns the squared euclidean distance between the two Vectortors.
        /// </summary>
        public static float DistanceSqr(iVector3 lhs, iVector3 rhs) => iVector3.DistanceSqr(lhs, rhs);
        
        /// <summary>
        /// Calculate the reflection direction for an incident Vectortor (N should be normalized in order to achieve the desired result).
        /// </summary>
        public static iVector3 Reflect(iVector3 I, iVector3 N) => iVector3.Reflect(I, N);
        
        /// <summary>
        /// Calculate the refraction direction for an incident Vectortor (The input parameters I and N should be normalized in order to achieve the desired result).
        /// </summary>
        public static iVector3 Refract(iVector3 I, iVector3 N, int eta) => iVector3.Refract(I, N, eta);
        
        /// <summary>
        /// Returns a Vectortor pointing in the same direction as another (faceforward orients a Vectortor to point away from a surface as defined by its normal. If dot(Nref, I) is negative faceforward returns N, otherwise it returns -N).
        /// </summary>
        public static iVector3 FaceForward(iVector3 N, iVector3 I, iVector3 Nref) => iVector3.FaceForward(N, I, Nref);
        
        /// <summary>
        /// Returns the outer product (cross product, Vectortor product) of the two Vectortors.
        /// </summary>
        public static iVector3 Cross(iVector3 l, iVector3 r) => iVector3.Cross(l, r);
        
        /// <summary>
        /// Returns a iVector3 with independent and identically distributed uniform integer values between 0 (inclusive) and maxValue (exclusive). (A maxValue of 0 is allowed and returns 0.)
        /// </summary>
        public static iVector3 Random(System.Random random, iVector3 maxValue) => iVector3.Random(random, maxValue);
        
        /// <summary>
        /// Returns a iVector3 with independent and identically distributed uniform integer values between 0 (inclusive) and maxValue (exclusive). (A maxValue of 0 is allowed and returns 0.)
        /// </summary>
        public static int Random(System.Random random, int maxValue) => (int)random.Next((int)maxValue);
        
        /// <summary>
        /// Returns a iVector3 with independent and identically distributed uniform integer values between minValue (inclusive) and maxValue (exclusive). (minValue == maxValue is allowed and returns minValue. Negative values are allowed.)
        /// </summary>
        public static iVector3 Random(System.Random random, iVector3 minValue, iVector3 maxValue) => iVector3.Random(random, minValue, maxValue);
        
        /// <summary>
        /// Returns a iVector3 with independent and identically distributed uniform integer values between minValue (inclusive) and maxValue (exclusive). (minValue == maxValue is allowed and returns minValue. Negative values are allowed.)
        /// </summary>
        public static int Random(System.Random random, int minValue, int maxValue) => (int)random.Next((int)minValue, (int)maxValue);
        
        /// <summary>
        /// Returns a iVector3 with independent and identically distributed uniform integer values between minValue (inclusive) and maxValue (exclusive). (minValue == maxValue is allowed and returns minValue. Negative values are allowed.)
        /// </summary>
        public static iVector3 RandomUniform(System.Random random, iVector3 minValue, iVector3 maxValue) => iVector3.RandomUniform(random, minValue, maxValue);
        
        /// <summary>
        /// Returns a iVector3 with independent and identically distributed uniform integer values between minValue (inclusive) and maxValue (exclusive). (minValue == maxValue is allowed and returns minValue. Negative values are allowed.)
        /// </summary>
        public static int RandomUniform(System.Random random, int minValue, int maxValue) => (int)random.Next((int)minValue, (int)maxValue);
        
        /// <summary>
        /// Returns a iVector3 with independent and identically distributed integer values according to a poisson distribution with given lambda parameter.
        /// </summary>
        public static iVector3 RandomPoisson(System.Random random, dVector3 lambda) => iVector3.RandomPoisson(random, lambda);
        
        /// <summary>
        /// Returns a iVector3 with independent and identically distributed integer values according to a poisson distribution with given lambda parameter.
        /// </summary>
        public static int RandomPoisson(System.Random random, double lambda) => (int)lambda.GetPoisson(random);

    }
}
