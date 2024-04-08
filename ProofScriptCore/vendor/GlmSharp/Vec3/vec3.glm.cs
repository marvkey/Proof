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
        public static swizzle_Vector3 swizzle(Vector3 v) => v.swizzle;
        
        /// <summary>
        /// Returns an array with all values
        /// </summary>
        public static float[] Values(Vector3 v) => v.Values;
        
        /// <summary>
        /// Returns an enumerator that iterates through all components.
        /// </summary>
        public static IEnumerator<float> GetEnumerator(Vector3 v) => v.GetEnumerator();
        
        /// <summary>
        /// Returns a string representation of this Vector using ', ' as a seperator.
        /// </summary>
        public static string ToString(Vector3 v) => v.ToString();
        
        /// <summary>
        /// Returns a string representation of this Vector using a provided seperator.
        /// </summary>
        public static string ToString(Vector3 v, string sep) => v.ToString(sep);
        
        /// <summary>
        /// Returns a string representation of this Vector using a provided seperator and a forMatrix provider for each component.
        /// </summary>
        public static string ToString(Vector3 v, string sep, IFormatProvider provider) => v.ToString(sep, provider);
        
        /// <summary>
        /// Returns a string representation of this Vector using a provided seperator and a forMatrix for each component.
        /// </summary>
        public static string ToString(Vector3 v, string sep, string forMatrix) => v.ToString(sep, forMatrix);
        
        /// <summary>
        /// Returns a string representation of this Vector using a provided seperator and a forMatrix and forMatrix provider for each component.
        /// </summary>
        public static string ToString(Vector3 v, string sep, string forMatrix, IFormatProvider provider) => v.ToString(sep, forMatrix, provider);
        
        /// <summary>
        /// Returns the number of components (3).
        /// </summary>
        public static int Count(Vector3 v) => v.Count;
        
        /// <summary>
        /// Returns true iff this equals rhs component-wise.
        /// </summary>
        public static bool Equals(Vector3 v, Vector3 rhs) => v.Equals(rhs);
        
        /// <summary>
        /// Returns true iff this equals rhs type- and component-wise.
        /// </summary>
        public static bool Equals(Vector3 v, object obj) => v.Equals(obj);
        
        /// <summary>
        /// Returns a hash code for this instance.
        /// </summary>
        public static int GetHashCode(Vector3 v) => v.GetHashCode();
        
        /// <summary>
        /// Returns true iff distance between lhs and rhs is less than or equal to epsilon
        /// </summary>
        public static bool ApproxEqual(Vector3 lhs, Vector3 rhs, float eps = 0.1f) => Vector3.ApproxEqual(lhs, rhs, eps);
        
        /// <summary>
        /// Returns a bVector3 from component-wise application of Equal (lhs == rhs).
        /// </summary>
        public static bVector3 Equal(Vector3 lhs, Vector3 rhs) => Vector3.Equal(lhs, rhs);
        
        /// <summary>
        /// Returns a bVector3 from component-wise application of Equal (lhs == rhs).
        /// </summary>
        public static bool Equal(float lhs, float rhs) => lhs == rhs;
        
        /// <summary>
        /// Returns a bVector3 from component-wise application of NotEqual (lhs != rhs).
        /// </summary>
        public static bVector3 NotEqual(Vector3 lhs, Vector3 rhs) => Vector3.NotEqual(lhs, rhs);
        
        /// <summary>
        /// Returns a bVector3 from component-wise application of NotEqual (lhs != rhs).
        /// </summary>
        public static bool NotEqual(float lhs, float rhs) => lhs != rhs;
        
        /// <summary>
        /// Returns a bVector3 from component-wise application of GreaterThan (lhs &gt; rhs).
        /// </summary>
        public static bVector3 GreaterThan(Vector3 lhs, Vector3 rhs) => Vector3.GreaterThan(lhs, rhs);
        
        /// <summary>
        /// Returns a bVector3 from component-wise application of GreaterThan (lhs &gt; rhs).
        /// </summary>
        public static bool GreaterThan(float lhs, float rhs) => lhs > rhs;
        
        /// <summary>
        /// Returns a bVector3 from component-wise application of GreaterThanEqual (lhs &gt;= rhs).
        /// </summary>
        public static bVector3 GreaterThanEqual(Vector3 lhs, Vector3 rhs) => Vector3.GreaterThanEqual(lhs, rhs);
        
        /// <summary>
        /// Returns a bVector3 from component-wise application of GreaterThanEqual (lhs &gt;= rhs).
        /// </summary>
        public static bool GreaterThanEqual(float lhs, float rhs) => lhs >= rhs;
        
        /// <summary>
        /// Returns a bVector3 from component-wise application of LesserThan (lhs &lt; rhs).
        /// </summary>
        public static bVector3 LesserThan(Vector3 lhs, Vector3 rhs) => Vector3.LesserThan(lhs, rhs);
        
        /// <summary>
        /// Returns a bVector3 from component-wise application of LesserThan (lhs &lt; rhs).
        /// </summary>
        public static bool LesserThan(float lhs, float rhs) => lhs < rhs;
        
        /// <summary>
        /// Returns a bVector3 from component-wise application of LesserThanEqual (lhs &lt;= rhs).
        /// </summary>
        public static bVector3 LesserThanEqual(Vector3 lhs, Vector3 rhs) => Vector3.LesserThanEqual(lhs, rhs);
        
        /// <summary>
        /// Returns a bVector3 from component-wise application of LesserThanEqual (lhs &lt;= rhs).
        /// </summary>
        public static bool LesserThanEqual(float lhs, float rhs) => lhs <= rhs;
        
        /// <summary>
        /// Returns a bVector3 from component-wise application of IsInfinity (float.IsInfinity(v)).
        /// </summary>
        public static bVector3 IsInfinity(Vector3 v) => Vector3.IsInfinity(v);
        
        /// <summary>
        /// Returns a bVector3 from component-wise application of IsInfinity (float.IsInfinity(v)).
        /// </summary>
        public static bool IsInfinity(float v) => float.IsInfinity(v);
        
        /// <summary>
        /// Returns a bVector3 from component-wise application of IsFinite (!float.IsNaN(v) &amp;&amp; !float.IsInfinity(v)).
        /// </summary>
        public static bVector3 IsFinite(Vector3 v) => Vector3.IsFinite(v);
        
        /// <summary>
        /// Returns a bVector3 from component-wise application of IsFinite (!float.IsNaN(v) &amp;&amp; !float.IsInfinity(v)).
        /// </summary>
        public static bool IsFinite(float v) => !float.IsNaN(v) && !float.IsInfinity(v);
        
        /// <summary>
        /// Returns a bVector3 from component-wise application of IsNaN (float.IsNaN(v)).
        /// </summary>
        public static bVector3 IsNaN(Vector3 v) => Vector3.IsNaN(v);
        
        /// <summary>
        /// Returns a bVector3 from component-wise application of IsNaN (float.IsNaN(v)).
        /// </summary>
        public static bool IsNaN(float v) => float.IsNaN(v);
        
        /// <summary>
        /// Returns a bVector3 from component-wise application of IsNegativeInfinity (float.IsNegativeInfinity(v)).
        /// </summary>
        public static bVector3 IsNegativeInfinity(Vector3 v) => Vector3.IsNegativeInfinity(v);
        
        /// <summary>
        /// Returns a bVector3 from component-wise application of IsNegativeInfinity (float.IsNegativeInfinity(v)).
        /// </summary>
        public static bool IsNegativeInfinity(float v) => float.IsNegativeInfinity(v);
        
        /// <summary>
        /// Returns a bVector3 from component-wise application of IsPositiveInfinity (float.IsPositiveInfinity(v)).
        /// </summary>
        public static bVector3 IsPositiveInfinity(Vector3 v) => Vector3.IsPositiveInfinity(v);
        
        /// <summary>
        /// Returns a bVector3 from component-wise application of IsPositiveInfinity (float.IsPositiveInfinity(v)).
        /// </summary>
        public static bool IsPositiveInfinity(float v) => float.IsPositiveInfinity(v);
        
        /// <summary>
        /// Returns a Vector3 from component-wise application of Abs (Math.Abs(v)).
        /// </summary>
        public static Vector3 Abs(Vector3 v) => Vector3.Abs(v);
        
        /// <summary>
        /// Returns a Vector3 from component-wise application of Abs (Math.Abs(v)).
        /// </summary>
        public static float Abs(float v) => Math.Abs(v);
        
        /// <summary>
        /// Returns a Vector3 from component-wise application of HermiteInterpolationOrder3 ((3 - 2 * v) * v * v).
        /// </summary>
        public static Vector3 HermiteInterpolationOrder3(Vector3 v) => Vector3.HermiteInterpolationOrder3(v);
        
        /// <summary>
        /// Returns a Vector3 from component-wise application of HermiteInterpolationOrder3 ((3 - 2 * v) * v * v).
        /// </summary>
        public static float HermiteInterpolationOrder3(float v) => (3 - 2 * v) * v * v;
        
        /// <summary>
        /// Returns a Vector3 from component-wise application of HermiteInterpolationOrder5 (((6 * v - 15) * v + 10) * v * v * v).
        /// </summary>
        public static Vector3 HermiteInterpolationOrder5(Vector3 v) => Vector3.HermiteInterpolationOrder5(v);
        
        /// <summary>
        /// Returns a Vector3 from component-wise application of HermiteInterpolationOrder5 (((6 * v - 15) * v + 10) * v * v * v).
        /// </summary>
        public static float HermiteInterpolationOrder5(float v) => ((6 * v - 15) * v + 10) * v * v * v;
        
        /// <summary>
        /// Returns a Vector3 from component-wise application of Sqr (v * v).
        /// </summary>
        public static Vector3 Sqr(Vector3 v) => Vector3.Sqr(v);
        
        /// <summary>
        /// Returns a Vector3 from component-wise application of Sqr (v * v).
        /// </summary>
        public static float Sqr(float v) => v * v;
        
        /// <summary>
        /// Returns a Vector3 from component-wise application of Pow2 (v * v).
        /// </summary>
        public static Vector3 Pow2(Vector3 v) => Vector3.Pow2(v);
        
        /// <summary>
        /// Returns a Vector3 from component-wise application of Pow2 (v * v).
        /// </summary>
        public static float Pow2(float v) => v * v;
        
        /// <summary>
        /// Returns a Vector3 from component-wise application of Pow3 (v * v * v).
        /// </summary>
        public static Vector3 Pow3(Vector3 v) => Vector3.Pow3(v);
        
        /// <summary>
        /// Returns a Vector3 from component-wise application of Pow3 (v * v * v).
        /// </summary>
        public static float Pow3(float v) => v * v * v;
        
        /// <summary>
        /// Returns a Vector3 from component-wise application of Step (v &gt;= 0f ? 1f : 0f).
        /// </summary>
        public static Vector3 Step(Vector3 v) => Vector3.Step(v);
        
        /// <summary>
        /// Returns a Vector3 from component-wise application of Step (v &gt;= 0f ? 1f : 0f).
        /// </summary>
        public static float Step(float v) => v >= 0f ? 1f : 0f;
        
        /// <summary>
        /// Returns a Vector3 from component-wise application of Sqrt ((float)Math.Sqrt((double)v)).
        /// </summary>
        public static Vector3 Sqrt(Vector3 v) => Vector3.Sqrt(v);
        
        /// <summary>
        /// Returns a Vector3 from component-wise application of Sqrt ((float)Math.Sqrt((double)v)).
        /// </summary>
        public static float Sqrt(float v) => (float)Math.Sqrt((double)v);
        
        /// <summary>
        /// Returns a Vector3 from component-wise application of InverseSqrt ((float)(1.0 / Math.Sqrt((double)v))).
        /// </summary>
        public static Vector3 InverseSqrt(Vector3 v) => Vector3.InverseSqrt(v);
        
        /// <summary>
        /// Returns a Vector3 from component-wise application of InverseSqrt ((float)(1.0 / Math.Sqrt((double)v))).
        /// </summary>
        public static float InverseSqrt(float v) => (float)(1.0 / Math.Sqrt((double)v));
        
        /// <summary>
        /// Returns a iVector3 from component-wise application of Sign (Math.Sign(v)).
        /// </summary>
        public static iVector3 Sign(Vector3 v) => Vector3.Sign(v);
        
        /// <summary>
        /// Returns a iVector3 from component-wise application of Sign (Math.Sign(v)).
        /// </summary>
        public static int Sign(float v) => Math.Sign(v);
        
        /// <summary>
        /// Returns a Vector3 from component-wise application of Max (Math.Max(lhs, rhs)).
        /// </summary>
        public static Vector3 Max(Vector3 lhs, Vector3 rhs) => Vector3.Max(lhs, rhs);
        
        /// <summary>
        /// Returns a Vector3 from component-wise application of Max (Math.Max(lhs, rhs)).
        /// </summary>
        public static float Max(float lhs, float rhs) => Math.Max(lhs, rhs);
        
        /// <summary>
        /// Returns a Vector3 from component-wise application of Min (Math.Min(lhs, rhs)).
        /// </summary>
        public static Vector3 Min(Vector3 lhs, Vector3 rhs) => Vector3.Min(lhs, rhs);
        
        /// <summary>
        /// Returns a Vector3 from component-wise application of Min (Math.Min(lhs, rhs)).
        /// </summary>
        public static float Min(float lhs, float rhs) => Math.Min(lhs, rhs);
        
        /// <summary>
        /// Returns a Vector3 from component-wise application of Pow ((float)Math.Pow((double)lhs, (double)rhs)).
        /// </summary>
        public static Vector3 Pow(Vector3 lhs, Vector3 rhs) => Vector3.Pow(lhs, rhs);
        
        /// <summary>
        /// Returns a Vector3 from component-wise application of Pow ((float)Math.Pow((double)lhs, (double)rhs)).
        /// </summary>
        public static float Pow(float lhs, float rhs) => (float)Math.Pow((double)lhs, (double)rhs);
        
        /// <summary>
        /// Returns a Vector3 from component-wise application of Log ((float)Math.Log((double)lhs, (double)rhs)).
        /// </summary>
        public static Vector3 Log(Vector3 lhs, Vector3 rhs) => Vector3.Log(lhs, rhs);
        
        /// <summary>
        /// Returns a Vector3 from component-wise application of Log ((float)Math.Log((double)lhs, (double)rhs)).
        /// </summary>
        public static float Log(float lhs, float rhs) => (float)Math.Log((double)lhs, (double)rhs);
        
        /// <summary>
        /// Returns a Vector3 from component-wise application of Clamp (Math.Min(Math.Max(v, min), max)).
        /// </summary>
        public static Vector3 Clamp(Vector3 v, Vector3 min, Vector3 max) => Vector3.Clamp(v, min, max);
        
        /// <summary>
        /// Returns a Vector3 from component-wise application of Clamp (Math.Min(Math.Max(v, min), max)).
        /// </summary>
        public static float Clamp(float v, float min, float max) => Math.Min(Math.Max(v, min), max);
        
        /// <summary>
        /// Returns a Vector3 from component-wise application of Mix (min * (1-a) + max * a).
        /// </summary>
        public static Vector3 Mix(Vector3 min, Vector3 max, Vector3 a) => Vector3.Mix(min, max, a);
        
        /// <summary>
        /// Returns a Vector3 from component-wise application of Mix (min * (1-a) + max * a).
        /// </summary>
        public static float Mix(float min, float max, float a) => min * (1-a) + max * a;
        
        /// <summary>
        /// Returns a Vector3 from component-wise application of Lerp (min * (1-a) + max * a).
        /// </summary>
        public static Vector3 Lerp(Vector3 min, Vector3 max, Vector3 a) => Vector3.Lerp(min, max, a);
        
        /// <summary>
        /// Returns a Vector3 from component-wise application of Lerp (min * (1-a) + max * a).
        /// </summary>
        public static float Lerp(float min, float max, float a) => min * (1-a) + max * a;
        
        /// <summary>
        /// Returns a Vector3 from component-wise application of Smoothstep (((v - edge0) / (edge1 - edge0)).Clamp().HermiteInterpolationOrder3()).
        /// </summary>
        public static Vector3 Smoothstep(Vector3 edge0, Vector3 edge1, Vector3 v) => Vector3.Smoothstep(edge0, edge1, v);
        
        /// <summary>
        /// Returns a Vector3 from component-wise application of Smoothstep (((v - edge0) / (edge1 - edge0)).Clamp().HermiteInterpolationOrder3()).
        /// </summary>
        public static float Smoothstep(float edge0, float edge1, float v) => ((v - edge0) / (edge1 - edge0)).Clamp().HermiteInterpolationOrder3();
        
        /// <summary>
        /// Returns a Vector3 from component-wise application of Smootherstep (((v - edge0) / (edge1 - edge0)).Clamp().HermiteInterpolationOrder5()).
        /// </summary>
        public static Vector3 Smootherstep(Vector3 edge0, Vector3 edge1, Vector3 v) => Vector3.Smootherstep(edge0, edge1, v);
        
        /// <summary>
        /// Returns a Vector3 from component-wise application of Smootherstep (((v - edge0) / (edge1 - edge0)).Clamp().HermiteInterpolationOrder5()).
        /// </summary>
        public static float Smootherstep(float edge0, float edge1, float v) => ((v - edge0) / (edge1 - edge0)).Clamp().HermiteInterpolationOrder5();
        
        /// <summary>
        /// Returns a Vector3 from component-wise application of Fma (a * b + c).
        /// </summary>
        public static Vector3 Fma(Vector3 a, Vector3 b, Vector3 c) => Vector3.Fma(a, b, c);
        
        /// <summary>
        /// Returns a Vector3 from component-wise application of Fma (a * b + c).
        /// </summary>
        public static float Fma(float a, float b, float c) => a * b + c;
        
        /// <summary>
        /// OuterProduct treats the first parameter c as a column Vector (Matrix with one column) and the second parameter r as a row Vector (Matrix with one row) and does a linear algebraic Matrix multiply c * r, yielding a Matrix whose number of rows is the number of components in c and whose number of columns is the number of components in r.
        /// </summary>
        public static Matrix2x3 OuterProduct(Vector3 c, Vector2 r) => Vector3.OuterProduct(c, r);
        
        /// <summary>
        /// OuterProduct treats the first parameter c as a column Vector (Matrix with one column) and the second parameter r as a row Vector (Matrix with one row) and does a linear algebraic Matrix multiply c * r, yielding a Matrix whose number of rows is the number of components in c and whose number of columns is the number of components in r.
        /// </summary>
        public static Matrix3 OuterProduct(Vector3 c, Vector3 r) => Vector3.OuterProduct(c, r);
        
        /// <summary>
        /// OuterProduct treats the first parameter c as a column Vector (Matrix with one column) and the second parameter r as a row Vector (Matrix with one row) and does a linear algebraic Matrix multiply c * r, yielding a Matrix whose number of rows is the number of components in c and whose number of columns is the number of components in r.
        /// </summary>
        public static Matrix4x3 OuterProduct(Vector3 c, Vector4 r) => Vector3.OuterProduct(c, r);
        
        /// <summary>
        /// Returns a Vector3 from component-wise application of Add (lhs + rhs).
        /// </summary>
        public static Vector3 Add(Vector3 lhs, Vector3 rhs) => Vector3.Add(lhs, rhs);
        
        /// <summary>
        /// Returns a Vector3 from component-wise application of Add (lhs + rhs).
        /// </summary>
        public static float Add(float lhs, float rhs) => lhs + rhs;
        
        /// <summary>
        /// Returns a Vector3 from component-wise application of Sub (lhs - rhs).
        /// </summary>
        public static Vector3 Sub(Vector3 lhs, Vector3 rhs) => Vector3.Sub(lhs, rhs);
        
        /// <summary>
        /// Returns a Vector3 from component-wise application of Sub (lhs - rhs).
        /// </summary>
        public static float Sub(float lhs, float rhs) => lhs - rhs;
        
        /// <summary>
        /// Returns a Vector3 from component-wise application of Mul (lhs * rhs).
        /// </summary>
        public static Vector3 Mul(Vector3 lhs, Vector3 rhs) => Vector3.Mul(lhs, rhs);
        
        /// <summary>
        /// Returns a Vector3 from component-wise application of Mul (lhs * rhs).
        /// </summary>
        public static float Mul(float lhs, float rhs) => lhs * rhs;
        
        /// <summary>
        /// Returns a Vector3 from component-wise application of Div (lhs / rhs).
        /// </summary>
        public static Vector3 Div(Vector3 lhs, Vector3 rhs) => Vector3.Div(lhs, rhs);
        
        /// <summary>
        /// Returns a Vector3 from component-wise application of Div (lhs / rhs).
        /// </summary>
        public static float Div(float lhs, float rhs) => lhs / rhs;
        
        /// <summary>
        /// Returns a Vector3 from component-wise application of Modulo (lhs % rhs).
        /// </summary>
        public static Vector3 Modulo(Vector3 lhs, Vector3 rhs) => Vector3.Modulo(lhs, rhs);
        
        /// <summary>
        /// Returns a Vector3 from component-wise application of Modulo (lhs % rhs).
        /// </summary>
        public static float Modulo(float lhs, float rhs) => lhs % rhs;
        
        /// <summary>
        /// Returns a Vector3 from component-wise application of Degrees (Radians-To-Degrees Conversion).
        /// </summary>
        public static Vector3 Degrees(Vector3 v) => Vector3.Degrees(v);
        
        /// <summary>
        /// Returns a Vector3 from component-wise application of Degrees (Radians-To-Degrees Conversion).
        /// </summary>
        public static float Degrees(float v) => (float)(v * 57.295779513082320876798154814105170332405472466564321f);
        
        /// <summary>
        /// Returns a Vector3 from component-wise application of Radians (Degrees-To-Radians Conversion).
        /// </summary>
        public static Vector3 Radians(Vector3 v) => Vector3.Radians(v);
        
        /// <summary>
        /// Returns a Vector3 from component-wise application of Radians (Degrees-To-Radians Conversion).
        /// </summary>
        public static float Radians(float v) => (float)(v * 0.0174532925199432957692369076848861271344287188854172f);
        
        /// <summary>
        /// Returns a Vector3 from component-wise application of Acos ((float)Math.Acos((double)v)).
        /// </summary>
        public static Vector3 Acos(Vector3 v) => Vector3.Acos(v);
        
        /// <summary>
        /// Returns a Vector3 from component-wise application of Acos ((float)Math.Acos((double)v)).
        /// </summary>
        public static float Acos(float v) => (float)Math.Acos((double)v);
        
        /// <summary>
        /// Returns a Vector3 from component-wise application of Asin ((float)Math.Asin((double)v)).
        /// </summary>
        public static Vector3 Asin(Vector3 v) => Vector3.Asin(v);
        
        /// <summary>
        /// Returns a Vector3 from component-wise application of Asin ((float)Math.Asin((double)v)).
        /// </summary>
        public static float Asin(float v) => (float)Math.Asin((double)v);
        
        /// <summary>
        /// Returns a Vector3 from component-wise application of Atan ((float)Math.Atan((double)v)).
        /// </summary>
        public static Vector3 Atan(Vector3 v) => Vector3.Atan(v);
        
        /// <summary>
        /// Returns a Vector3 from component-wise application of Atan ((float)Math.Atan((double)v)).
        /// </summary>
        public static float Atan(float v) => (float)Math.Atan((double)v);
        
        /// <summary>
        /// Returns a Vector3 from component-wise application of Cos ((float)Math.Cos((double)v)).
        /// </summary>
        public static Vector3 Cos(Vector3 v) => Vector3.Cos(v);
        
        /// <summary>
        /// Returns a Vector3 from component-wise application of Cos ((float)Math.Cos((double)v)).
        /// </summary>
        public static float Cos(float v) => (float)Math.Cos((double)v);
        
        /// <summary>
        /// Returns a Vector3 from component-wise application of Cosh ((float)Math.Cosh((double)v)).
        /// </summary>
        public static Vector3 Cosh(Vector3 v) => Vector3.Cosh(v);
        
        /// <summary>
        /// Returns a Vector3 from component-wise application of Cosh ((float)Math.Cosh((double)v)).
        /// </summary>
        public static float Cosh(float v) => (float)Math.Cosh((double)v);
        
        /// <summary>
        /// Returns a Vector3 from component-wise application of Exp ((float)Math.Exp((double)v)).
        /// </summary>
        public static Vector3 Exp(Vector3 v) => Vector3.Exp(v);
        
        /// <summary>
        /// Returns a Vector3 from component-wise application of Exp ((float)Math.Exp((double)v)).
        /// </summary>
        public static float Exp(float v) => (float)Math.Exp((double)v);
        
        /// <summary>
        /// Returns a Vector3 from component-wise application of Log ((float)Math.Log((double)v)).
        /// </summary>
        public static Vector3 Log(Vector3 v) => Vector3.Log(v);
        
        /// <summary>
        /// Returns a Vector3 from component-wise application of Log ((float)Math.Log((double)v)).
        /// </summary>
        public static float Log(float v) => (float)Math.Log((double)v);
        
        /// <summary>
        /// Returns a Vector3 from component-wise application of Log2 ((float)Math.Log((double)v, 2)).
        /// </summary>
        public static Vector3 Log2(Vector3 v) => Vector3.Log2(v);
        
        /// <summary>
        /// Returns a Vector3 from component-wise application of Log2 ((float)Math.Log((double)v, 2)).
        /// </summary>
        public static float Log2(float v) => (float)Math.Log((double)v, 2);
        
        /// <summary>
        /// Returns a Vector3 from component-wise application of Log10 ((float)Math.Log10((double)v)).
        /// </summary>
        public static Vector3 Log10(Vector3 v) => Vector3.Log10(v);
        
        /// <summary>
        /// Returns a Vector3 from component-wise application of Log10 ((float)Math.Log10((double)v)).
        /// </summary>
        public static float Log10(float v) => (float)Math.Log10((double)v);
        
        /// <summary>
        /// Returns a Vector3 from component-wise application of Floor ((float)Math.Floor(v)).
        /// </summary>
        public static Vector3 Floor(Vector3 v) => Vector3.Floor(v);
        
        /// <summary>
        /// Returns a Vector3 from component-wise application of Floor ((float)Math.Floor(v)).
        /// </summary>
        public static float Floor(float v) => (float)Math.Floor(v);
        
        /// <summary>
        /// Returns a Vector3 from component-wise application of Ceiling ((float)Math.Ceiling(v)).
        /// </summary>
        public static Vector3 Ceiling(Vector3 v) => Vector3.Ceiling(v);
        
        /// <summary>
        /// Returns a Vector3 from component-wise application of Ceiling ((float)Math.Ceiling(v)).
        /// </summary>
        public static float Ceiling(float v) => (float)Math.Ceiling(v);
        
        /// <summary>
        /// Returns a Vector3 from component-wise application of Round ((float)Math.Round(v)).
        /// </summary>
        public static Vector3 Round(Vector3 v) => Vector3.Round(v);
        
        /// <summary>
        /// Returns a Vector3 from component-wise application of Round ((float)Math.Round(v)).
        /// </summary>
        public static float Round(float v) => (float)Math.Round(v);
        
        /// <summary>
        /// Returns a Vector3 from component-wise application of Sin ((float)Math.Sin((double)v)).
        /// </summary>
        public static Vector3 Sin(Vector3 v) => Vector3.Sin(v);
        
        /// <summary>
        /// Returns a Vector3 from component-wise application of Sin ((float)Math.Sin((double)v)).
        /// </summary>
        public static float Sin(float v) => (float)Math.Sin((double)v);
        
        /// <summary>
        /// Returns a Vector3 from component-wise application of Sinh ((float)Math.Sinh((double)v)).
        /// </summary>
        public static Vector3 Sinh(Vector3 v) => Vector3.Sinh(v);
        
        /// <summary>
        /// Returns a Vector3 from component-wise application of Sinh ((float)Math.Sinh((double)v)).
        /// </summary>
        public static float Sinh(float v) => (float)Math.Sinh((double)v);
        
        /// <summary>
        /// Returns a Vector3 from component-wise application of Tan ((float)Math.Tan((double)v)).
        /// </summary>
        public static Vector3 Tan(Vector3 v) => Vector3.Tan(v);
        
        /// <summary>
        /// Returns a Vector3 from component-wise application of Tan ((float)Math.Tan((double)v)).
        /// </summary>
        public static float Tan(float v) => (float)Math.Tan((double)v);
        
        /// <summary>
        /// Returns a Vector3 from component-wise application of Tanh ((float)Math.Tanh((double)v)).
        /// </summary>
        public static Vector3 Tanh(Vector3 v) => Vector3.Tanh(v);
        
        /// <summary>
        /// Returns a Vector3 from component-wise application of Tanh ((float)Math.Tanh((double)v)).
        /// </summary>
        public static float Tanh(float v) => (float)Math.Tanh((double)v);
        
        /// <summary>
        /// Returns a Vector3 from component-wise application of Truncate ((float)Math.Truncate((double)v)).
        /// </summary>
        public static Vector3 Truncate(Vector3 v) => Vector3.Truncate(v);
        
        /// <summary>
        /// Returns a Vector3 from component-wise application of Truncate ((float)Math.Truncate((double)v)).
        /// </summary>
        public static float Truncate(float v) => (float)Math.Truncate((double)v);
        
        /// <summary>
        /// Returns a Vector3 from component-wise application of Fract ((float)(v - Math.Floor(v))).
        /// </summary>
        public static Vector3 Fract(Vector3 v) => Vector3.Fract(v);
        
        /// <summary>
        /// Returns a Vector3 from component-wise application of Fract ((float)(v - Math.Floor(v))).
        /// </summary>
        public static float Fract(float v) => (float)(v - Math.Floor(v));
        
        /// <summary>
        /// Returns a Vector3 from component-wise application of Trunc ((long)(v)).
        /// </summary>
        public static Vector3 Trunc(Vector3 v) => Vector3.Trunc(v);
        
        /// <summary>
        /// Returns a Vector3 from component-wise application of Trunc ((long)(v)).
        /// </summary>
        public static float Trunc(float v) => (long)(v);
        
        /// <summary>
        /// Returns the minimal component of this Vector.
        /// </summary>
        public static float MinElement(Vector3 v) => v.MinElement;
        
        /// <summary>
        /// Returns the maximal component of this Vector.
        /// </summary>
        public static float MaxElement(Vector3 v) => v.MaxElement;
        
        /// <summary>
        /// Returns the euclidean length of this Vector.
        /// </summary>
        public static float Length(Vector3 v) => v.Length;
        
        /// <summary>
        /// Returns the squared euclidean length of this Vector.
        /// </summary>
        public static float LengthSqr(Vector3 v) => v.LengthSqr;
        
        /// <summary>
        /// Returns the sum of all components.
        /// </summary>
        public static float Sum(Vector3 v) => v.Sum;
        
        /// <summary>
        /// Returns the euclidean norm of this Vector.
        /// </summary>
        public static float Norm(Vector3 v) => v.Norm;
        
        /// <summary>
        /// Returns the one-norm of this Vector.
        /// </summary>
        public static float Norm1(Vector3 v) => v.Norm1;
        
        /// <summary>
        /// Returns the two-norm (euclidean length) of this Vector.
        /// </summary>
        public static float Norm2(Vector3 v) => v.Norm2;
        
        /// <summary>
        /// Returns the max-norm of this Vector.
        /// </summary>
        public static float NormMax(Vector3 v) => v.NormMax;
        
        /// <summary>
        /// Returns the p-norm of this Vector.
        /// </summary>
        public static double NormP(Vector3 v, double p) => v.NormP(p);
        
        /// <summary>
        /// Returns a copy of this Vector with length one (undefined if this has zero length).
        /// </summary>
        public static Vector3 Normalized(Vector3 v) => v.Normalized;
        
        /// <summary>
        /// Returns a copy of this Vector with length one (returns zero if length is zero).
        /// </summary>
        public static Vector3 NormalizedSafe(Vector3 v) => v.NormalizedSafe;
        
        /// <summary>
        /// Returns the inner product (dot product, scalar product) of the two Vectors.
        /// </summary>
        public static float Dot(Vector3 lhs, Vector3 rhs) => Vector3.Dot(lhs, rhs);
        
        /// <summary>
        /// Returns the euclidean distance between the two Vectors.
        /// </summary>
        public static float Distance(Vector3 lhs, Vector3 rhs) => Vector3.Distance(lhs, rhs);
        
        /// <summary>
        /// Returns the squared euclidean distance between the two Vectors.
        /// </summary>
        public static float DistanceSqr(Vector3 lhs, Vector3 rhs) => Vector3.DistanceSqr(lhs, rhs);
        
        /// <summary>
        /// Calculate the reflection direction for an incident Vector (N should be normalized in order to achieve the desired result).
        /// </summary>
        public static Vector3 Reflect(Vector3 I, Vector3 N) => Vector3.Reflect(I, N);
        
        /// <summary>
        /// Calculate the refraction direction for an incident Vector (The input parameters I and N should be normalized in order to achieve the desired result).
        /// </summary>
        public static Vector3 Refract(Vector3 I, Vector3 N, float eta) => Vector3.Refract(I, N, eta);
        
        /// <summary>
        /// Returns a Vector pointing in the same direction as another (faceforward orients a Vector to point away from a surface as defined by its normal. If dot(Nref, I) is negative faceforward returns N, otherwise it returns -N).
        /// </summary>
        public static Vector3 FaceForward(Vector3 N, Vector3 I, Vector3 Nref) => Vector3.FaceForward(N, I, Nref);
        
        /// <summary>
        /// Returns the outer product (cross product, Vector product) of the two Vectors.
        /// </summary>
        public static Vector3 Cross(Vector3 l, Vector3 r) => Vector3.Cross(l, r);
        
        /// <summary>
        /// Returns a Vector3 with independent and identically distributed uniform values between 'minValue' and 'maxValue'.
        /// </summary>
        public static Vector3 Random(System.Random random, Vector3 minValue, Vector3 maxValue) => Vector3.Random(random, minValue, maxValue);
        
        /// <summary>
        /// Returns a Vector3 with independent and identically distributed uniform values between 'minValue' and 'maxValue'.
        /// </summary>
        public static float Random(System.Random random, float minValue, float maxValue) => (float)random.NextDouble() * (maxValue - minValue) + minValue;
        
        /// <summary>
        /// Returns a Vector3 with independent and identically distributed uniform values between 'minValue' and 'maxValue'.
        /// </summary>
        public static Vector3 RandomUniform(System.Random random, Vector3 minValue, Vector3 maxValue) => Vector3.RandomUniform(random, minValue, maxValue);
        
        /// <summary>
        /// Returns a Vector3 with independent and identically distributed uniform values between 'minValue' and 'maxValue'.
        /// </summary>
        public static float RandomUniform(System.Random random, float minValue, float maxValue) => (float)random.NextDouble() * (maxValue - minValue) + minValue;
        
        /// <summary>
        /// Returns a Vector3 with independent and identically distributed values according to a normal/Gaussian distribution with specified mean and variance.
        /// </summary>
        public static Vector3 RandomNormal(System.Random random, Vector3 mean, Vector3 variance) => Vector3.RandomNormal(random, mean, variance);
        
        /// <summary>
        /// Returns a Vector3 with independent and identically distributed values according to a normal/Gaussian distribution with specified mean and variance.
        /// </summary>
        public static float RandomNormal(System.Random random, float mean, float variance) => (float)(Math.Sqrt((double)variance) * Math.Cos(2 * Math.PI * random.NextDouble()) * Math.Sqrt(-2.0 * Math.Log(random.NextDouble()))) + mean;
        
        /// <summary>
        /// Returns a Vector3 with independent and identically distributed values according to a normal/Gaussian distribution with specified mean and variance.
        /// </summary>
        public static Vector3 RandomGaussian(System.Random random, Vector3 mean, Vector3 variance) => Vector3.RandomGaussian(random, mean, variance);
        
        /// <summary>
        /// Returns a Vector3 with independent and identically distributed values according to a normal/Gaussian distribution with specified mean and variance.
        /// </summary>
        public static float RandomGaussian(System.Random random, float mean, float variance) => (float)(Math.Sqrt((double)variance) * Math.Cos(2 * Math.PI * random.NextDouble()) * Math.Sqrt(-2.0 * Math.Log(random.NextDouble()))) + mean;

    }
}
