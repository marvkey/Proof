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
        public static swizzle_hVector3 swizzle(hVector3 v) => v.swizzle;
        
        /// <summary>
        /// Returns an array with all values
        /// </summary>
        public static Half[] Values(hVector3 v) => v.Values;
        
        /// <summary>
        /// Returns an enumerator that iterates through all components.
        /// </summary>
        public static IEnumerator<Half> GetEnumerator(hVector3 v) => v.GetEnumerator();
        
        /// <summary>
        /// Returns a string representation of this Vector using ', ' as a seperator.
        /// </summary>
        public static string ToString(hVector3 v) => v.ToString();
        
        /// <summary>
        /// Returns a string representation of this Vector using a provided seperator.
        /// </summary>
        public static string ToString(hVector3 v, string sep) => v.ToString(sep);
        
        /// <summary>
        /// Returns a string representation of this Vector using a provided seperator and a forMatrix provider for each component.
        /// </summary>
        public static string ToString(hVector3 v, string sep, IFormatProvider provider) => v.ToString(sep, provider);
        
        /// <summary>
        /// Returns a string representation of this Vector using a provided seperator and a forMatrix for each component.
        /// </summary>
        public static string ToString(hVector3 v, string sep, string forMatrix) => v.ToString(sep, forMatrix);
        
        /// <summary>
        /// Returns a string representation of this Vector using a provided seperator and a forMatrix and forMatrix provider for each component.
        /// </summary>
        public static string ToString(hVector3 v, string sep, string forMatrix, IFormatProvider provider) => v.ToString(sep, forMatrix, provider);
        
        /// <summary>
        /// Returns the number of components (3).
        /// </summary>
        public static int Count(hVector3 v) => v.Count;
        
        /// <summary>
        /// Returns true iff this equals rhs component-wise.
        /// </summary>
        public static bool Equals(hVector3 v, hVector3 rhs) => v.Equals(rhs);
        
        /// <summary>
        /// Returns true iff this equals rhs type- and component-wise.
        /// </summary>
        public static bool Equals(hVector3 v, object obj) => v.Equals(obj);
        
        /// <summary>
        /// Returns a hash code for this instance.
        /// </summary>
        public static int GetHashCode(hVector3 v) => v.GetHashCode();
        
        /// <summary>
        /// Returns true iff distance between lhs and rhs is less than or equal to epsilon
        /// </summary>
        public static bool ApproxEqual(hVector3 lhs, hVector3 rhs, float eps = 0.1f) => hVector3.ApproxEqual(lhs, rhs, eps);
        
        /// <summary>
        /// Returns a bVector3 from component-wise application of Equal (lhs == rhs).
        /// </summary>
        public static bVector3 Equal(hVector3 lhs, hVector3 rhs) => hVector3.Equal(lhs, rhs);
        
        /// <summary>
        /// Returns a bVector3 from component-wise application of Equal (lhs == rhs).
        /// </summary>
        public static bool Equal(Half lhs, Half rhs) => lhs == rhs;
        
        /// <summary>
        /// Returns a bVector3 from component-wise application of NotEqual (lhs != rhs).
        /// </summary>
        public static bVector3 NotEqual(hVector3 lhs, hVector3 rhs) => hVector3.NotEqual(lhs, rhs);
        
        /// <summary>
        /// Returns a bVector3 from component-wise application of NotEqual (lhs != rhs).
        /// </summary>
        public static bool NotEqual(Half lhs, Half rhs) => lhs != rhs;
        
        /// <summary>
        /// Returns a bVector3 from component-wise application of GreaterThan (lhs &gt; rhs).
        /// </summary>
        public static bVector3 GreaterThan(hVector3 lhs, hVector3 rhs) => hVector3.GreaterThan(lhs, rhs);
        
        /// <summary>
        /// Returns a bVector3 from component-wise application of GreaterThan (lhs &gt; rhs).
        /// </summary>
        public static bool GreaterThan(Half lhs, Half rhs) => lhs > rhs;
        
        /// <summary>
        /// Returns a bVector3 from component-wise application of GreaterThanEqual (lhs &gt;= rhs).
        /// </summary>
        public static bVector3 GreaterThanEqual(hVector3 lhs, hVector3 rhs) => hVector3.GreaterThanEqual(lhs, rhs);
        
        /// <summary>
        /// Returns a bVector3 from component-wise application of GreaterThanEqual (lhs &gt;= rhs).
        /// </summary>
        public static bool GreaterThanEqual(Half lhs, Half rhs) => lhs >= rhs;
        
        /// <summary>
        /// Returns a bVector3 from component-wise application of LesserThan (lhs &lt; rhs).
        /// </summary>
        public static bVector3 LesserThan(hVector3 lhs, hVector3 rhs) => hVector3.LesserThan(lhs, rhs);
        
        /// <summary>
        /// Returns a bVector3 from component-wise application of LesserThan (lhs &lt; rhs).
        /// </summary>
        public static bool LesserThan(Half lhs, Half rhs) => lhs < rhs;
        
        /// <summary>
        /// Returns a bVector3 from component-wise application of LesserThanEqual (lhs &lt;= rhs).
        /// </summary>
        public static bVector3 LesserThanEqual(hVector3 lhs, hVector3 rhs) => hVector3.LesserThanEqual(lhs, rhs);
        
        /// <summary>
        /// Returns a bVector3 from component-wise application of LesserThanEqual (lhs &lt;= rhs).
        /// </summary>
        public static bool LesserThanEqual(Half lhs, Half rhs) => lhs <= rhs;
        
        /// <summary>
        /// Returns a bVector3 from component-wise application of IsInfinity (Half.IsInfinity(v)).
        /// </summary>
        public static bVector3 IsInfinity(hVector3 v) => hVector3.IsInfinity(v);
        
        /// <summary>
        /// Returns a bVector3 from component-wise application of IsInfinity (Half.IsInfinity(v)).
        /// </summary>
        public static bool IsInfinity(Half v) => Half.IsInfinity(v);
        
        /// <summary>
        /// Returns a bVector3 from component-wise application of IsFinite (!Half.IsNaN(v) &amp;&amp; !Half.IsInfinity(v)).
        /// </summary>
        public static bVector3 IsFinite(hVector3 v) => hVector3.IsFinite(v);
        
        /// <summary>
        /// Returns a bVector3 from component-wise application of IsFinite (!Half.IsNaN(v) &amp;&amp; !Half.IsInfinity(v)).
        /// </summary>
        public static bool IsFinite(Half v) => !Half.IsNaN(v) && !Half.IsInfinity(v);
        
        /// <summary>
        /// Returns a bVector3 from component-wise application of IsNaN (Half.IsNaN(v)).
        /// </summary>
        public static bVector3 IsNaN(hVector3 v) => hVector3.IsNaN(v);
        
        /// <summary>
        /// Returns a bVector3 from component-wise application of IsNaN (Half.IsNaN(v)).
        /// </summary>
        public static bool IsNaN(Half v) => Half.IsNaN(v);
        
        /// <summary>
        /// Returns a bVector3 from component-wise application of IsNegativeInfinity (Half.IsNegativeInfinity(v)).
        /// </summary>
        public static bVector3 IsNegativeInfinity(hVector3 v) => hVector3.IsNegativeInfinity(v);
        
        /// <summary>
        /// Returns a bVector3 from component-wise application of IsNegativeInfinity (Half.IsNegativeInfinity(v)).
        /// </summary>
        public static bool IsNegativeInfinity(Half v) => Half.IsNegativeInfinity(v);
        
        /// <summary>
        /// Returns a bVector3 from component-wise application of IsPositiveInfinity (Half.IsPositiveInfinity(v)).
        /// </summary>
        public static bVector3 IsPositiveInfinity(hVector3 v) => hVector3.IsPositiveInfinity(v);
        
        /// <summary>
        /// Returns a bVector3 from component-wise application of IsPositiveInfinity (Half.IsPositiveInfinity(v)).
        /// </summary>
        public static bool IsPositiveInfinity(Half v) => Half.IsPositiveInfinity(v);
        
        /// <summary>
        /// Returns a hVector3 from component-wise application of Abs (Half.Abs(v)).
        /// </summary>
        public static hVector3 Abs(hVector3 v) => hVector3.Abs(v);
        
        /// <summary>
        /// Returns a hVector3 from component-wise application of Abs (Half.Abs(v)).
        /// </summary>
        public static Half Abs(Half v) => Half.Abs(v);
        
        /// <summary>
        /// Returns a hVector3 from component-wise application of HermiteInterpolationOrder3 ((3 - 2 * v) * v * v).
        /// </summary>
        public static hVector3 HermiteInterpolationOrder3(hVector3 v) => hVector3.HermiteInterpolationOrder3(v);
        
        /// <summary>
        /// Returns a hVector3 from component-wise application of HermiteInterpolationOrder3 ((3 - 2 * v) * v * v).
        /// </summary>
        public static Half HermiteInterpolationOrder3(Half v) => (3 - 2 * v) * v * v;
        
        /// <summary>
        /// Returns a hVector3 from component-wise application of HermiteInterpolationOrder5 (((6 * v - 15) * v + 10) * v * v * v).
        /// </summary>
        public static hVector3 HermiteInterpolationOrder5(hVector3 v) => hVector3.HermiteInterpolationOrder5(v);
        
        /// <summary>
        /// Returns a hVector3 from component-wise application of HermiteInterpolationOrder5 (((6 * v - 15) * v + 10) * v * v * v).
        /// </summary>
        public static Half HermiteInterpolationOrder5(Half v) => ((6 * v - 15) * v + 10) * v * v * v;
        
        /// <summary>
        /// Returns a hVector3 from component-wise application of Sqr (v * v).
        /// </summary>
        public static hVector3 Sqr(hVector3 v) => hVector3.Sqr(v);
        
        /// <summary>
        /// Returns a hVector3 from component-wise application of Sqr (v * v).
        /// </summary>
        public static Half Sqr(Half v) => v * v;
        
        /// <summary>
        /// Returns a hVector3 from component-wise application of Pow2 (v * v).
        /// </summary>
        public static hVector3 Pow2(hVector3 v) => hVector3.Pow2(v);
        
        /// <summary>
        /// Returns a hVector3 from component-wise application of Pow2 (v * v).
        /// </summary>
        public static Half Pow2(Half v) => v * v;
        
        /// <summary>
        /// Returns a hVector3 from component-wise application of Pow3 (v * v * v).
        /// </summary>
        public static hVector3 Pow3(hVector3 v) => hVector3.Pow3(v);
        
        /// <summary>
        /// Returns a hVector3 from component-wise application of Pow3 (v * v * v).
        /// </summary>
        public static Half Pow3(Half v) => v * v * v;
        
        /// <summary>
        /// Returns a hVector3 from component-wise application of Step (v &gt;= Half.Zero ? Half.One : Half.Zero).
        /// </summary>
        public static hVector3 Step(hVector3 v) => hVector3.Step(v);
        
        /// <summary>
        /// Returns a hVector3 from component-wise application of Step (v &gt;= Half.Zero ? Half.One : Half.Zero).
        /// </summary>
        public static Half Step(Half v) => v >= Half.Zero ? Half.One : Half.Zero;
        
        /// <summary>
        /// Returns a hVector3 from component-wise application of Sqrt ((Half)Math.Sqrt((double)v)).
        /// </summary>
        public static hVector3 Sqrt(hVector3 v) => hVector3.Sqrt(v);
        
        /// <summary>
        /// Returns a hVector3 from component-wise application of Sqrt ((Half)Math.Sqrt((double)v)).
        /// </summary>
        public static Half Sqrt(Half v) => (Half)Math.Sqrt((double)v);
        
        /// <summary>
        /// Returns a hVector3 from component-wise application of InverseSqrt ((Half)(1.0 / Math.Sqrt((double)v))).
        /// </summary>
        public static hVector3 InverseSqrt(hVector3 v) => hVector3.InverseSqrt(v);
        
        /// <summary>
        /// Returns a hVector3 from component-wise application of InverseSqrt ((Half)(1.0 / Math.Sqrt((double)v))).
        /// </summary>
        public static Half InverseSqrt(Half v) => (Half)(1.0 / Math.Sqrt((double)v));
        
        /// <summary>
        /// Returns a iVector3 from component-wise application of Sign (Math.Sign(v)).
        /// </summary>
        public static iVector3 Sign(hVector3 v) => hVector3.Sign(v);
        
        /// <summary>
        /// Returns a iVector3 from component-wise application of Sign (Math.Sign(v)).
        /// </summary>
        public static int Sign(Half v) => Math.Sign(v);
        
        /// <summary>
        /// Returns a hVector3 from component-wise application of Max (Half.Max(lhs, rhs)).
        /// </summary>
        public static hVector3 Max(hVector3 lhs, hVector3 rhs) => hVector3.Max(lhs, rhs);
        
        /// <summary>
        /// Returns a hVector3 from component-wise application of Max (Half.Max(lhs, rhs)).
        /// </summary>
        public static Half Max(Half lhs, Half rhs) => Half.Max(lhs, rhs);
        
        /// <summary>
        /// Returns a hVector3 from component-wise application of Min (Half.Min(lhs, rhs)).
        /// </summary>
        public static hVector3 Min(hVector3 lhs, hVector3 rhs) => hVector3.Min(lhs, rhs);
        
        /// <summary>
        /// Returns a hVector3 from component-wise application of Min (Half.Min(lhs, rhs)).
        /// </summary>
        public static Half Min(Half lhs, Half rhs) => Half.Min(lhs, rhs);
        
        /// <summary>
        /// Returns a hVector3 from component-wise application of Pow ((Half)Math.Pow((double)lhs, (double)rhs)).
        /// </summary>
        public static hVector3 Pow(hVector3 lhs, hVector3 rhs) => hVector3.Pow(lhs, rhs);
        
        /// <summary>
        /// Returns a hVector3 from component-wise application of Pow ((Half)Math.Pow((double)lhs, (double)rhs)).
        /// </summary>
        public static Half Pow(Half lhs, Half rhs) => (Half)Math.Pow((double)lhs, (double)rhs);
        
        /// <summary>
        /// Returns a hVector3 from component-wise application of Log ((Half)Math.Log((double)lhs, (double)rhs)).
        /// </summary>
        public static hVector3 Log(hVector3 lhs, hVector3 rhs) => hVector3.Log(lhs, rhs);
        
        /// <summary>
        /// Returns a hVector3 from component-wise application of Log ((Half)Math.Log((double)lhs, (double)rhs)).
        /// </summary>
        public static Half Log(Half lhs, Half rhs) => (Half)Math.Log((double)lhs, (double)rhs);
        
        /// <summary>
        /// Returns a hVector3 from component-wise application of Clamp (Half.Min(Half.Max(v, min), max)).
        /// </summary>
        public static hVector3 Clamp(hVector3 v, hVector3 min, hVector3 max) => hVector3.Clamp(v, min, max);
        
        /// <summary>
        /// Returns a hVector3 from component-wise application of Clamp (Half.Min(Half.Max(v, min), max)).
        /// </summary>
        public static Half Clamp(Half v, Half min, Half max) => Half.Min(Half.Max(v, min), max);
        
        /// <summary>
        /// Returns a hVector3 from component-wise application of Mix (min * (1-a) + max * a).
        /// </summary>
        public static hVector3 Mix(hVector3 min, hVector3 max, hVector3 a) => hVector3.Mix(min, max, a);
        
        /// <summary>
        /// Returns a hVector3 from component-wise application of Mix (min * (1-a) + max * a).
        /// </summary>
        public static Half Mix(Half min, Half max, Half a) => min * (1-a) + max * a;
        
        /// <summary>
        /// Returns a hVector3 from component-wise application of Lerp (min * (1-a) + max * a).
        /// </summary>
        public static hVector3 Lerp(hVector3 min, hVector3 max, hVector3 a) => hVector3.Lerp(min, max, a);
        
        /// <summary>
        /// Returns a hVector3 from component-wise application of Lerp (min * (1-a) + max * a).
        /// </summary>
        public static Half Lerp(Half min, Half max, Half a) => min * (1-a) + max * a;
        
        /// <summary>
        /// Returns a hVector3 from component-wise application of Smoothstep (((v - edge0) / (edge1 - edge0)).Clamp().HermiteInterpolationOrder3()).
        /// </summary>
        public static hVector3 Smoothstep(hVector3 edge0, hVector3 edge1, hVector3 v) => hVector3.Smoothstep(edge0, edge1, v);
        
        /// <summary>
        /// Returns a hVector3 from component-wise application of Smoothstep (((v - edge0) / (edge1 - edge0)).Clamp().HermiteInterpolationOrder3()).
        /// </summary>
        public static Half Smoothstep(Half edge0, Half edge1, Half v) => ((v - edge0) / (edge1 - edge0)).Clamp().HermiteInterpolationOrder3();
        
        /// <summary>
        /// Returns a hVector3 from component-wise application of Smootherstep (((v - edge0) / (edge1 - edge0)).Clamp().HermiteInterpolationOrder5()).
        /// </summary>
        public static hVector3 Smootherstep(hVector3 edge0, hVector3 edge1, hVector3 v) => hVector3.Smootherstep(edge0, edge1, v);
        
        /// <summary>
        /// Returns a hVector3 from component-wise application of Smootherstep (((v - edge0) / (edge1 - edge0)).Clamp().HermiteInterpolationOrder5()).
        /// </summary>
        public static Half Smootherstep(Half edge0, Half edge1, Half v) => ((v - edge0) / (edge1 - edge0)).Clamp().HermiteInterpolationOrder5();
        
        /// <summary>
        /// Returns a hVector3 from component-wise application of Fma (a * b + c).
        /// </summary>
        public static hVector3 Fma(hVector3 a, hVector3 b, hVector3 c) => hVector3.Fma(a, b, c);
        
        /// <summary>
        /// Returns a hVector3 from component-wise application of Fma (a * b + c).
        /// </summary>
        public static Half Fma(Half a, Half b, Half c) => a * b + c;
        
        /// <summary>
        /// OuterProduct treats the first parameter c as a column Vector (Matrix with one column) and the second parameter r as a row Vector (Matrix with one row) and does a linear algebraic Matrix multiply c * r, yielding a Matrix whose number of rows is the number of components in c and whose number of columns is the number of components in r.
        /// </summary>
        public static hMatrix2x3 OuterProduct(hVector3 c, hVector2 r) => hVector3.OuterProduct(c, r);
        
        /// <summary>
        /// OuterProduct treats the first parameter c as a column Vector (Matrix with one column) and the second parameter r as a row Vector (Matrix with one row) and does a linear algebraic Matrix multiply c * r, yielding a Matrix whose number of rows is the number of components in c and whose number of columns is the number of components in r.
        /// </summary>
        public static hMatrix3 OuterProduct(hVector3 c, hVector3 r) => hVector3.OuterProduct(c, r);
        
        /// <summary>
        /// OuterProduct treats the first parameter c as a column Vector (Matrix with one column) and the second parameter r as a row Vector (Matrix with one row) and does a linear algebraic Matrix multiply c * r, yielding a Matrix whose number of rows is the number of components in c and whose number of columns is the number of components in r.
        /// </summary>
        public static hMatrix4x3 OuterProduct(hVector3 c, hVector4 r) => hVector3.OuterProduct(c, r);
        
        /// <summary>
        /// Returns a hVector3 from component-wise application of Add (lhs + rhs).
        /// </summary>
        public static hVector3 Add(hVector3 lhs, hVector3 rhs) => hVector3.Add(lhs, rhs);
        
        /// <summary>
        /// Returns a hVector3 from component-wise application of Add (lhs + rhs).
        /// </summary>
        public static Half Add(Half lhs, Half rhs) => lhs + rhs;
        
        /// <summary>
        /// Returns a hVector3 from component-wise application of Sub (lhs - rhs).
        /// </summary>
        public static hVector3 Sub(hVector3 lhs, hVector3 rhs) => hVector3.Sub(lhs, rhs);
        
        /// <summary>
        /// Returns a hVector3 from component-wise application of Sub (lhs - rhs).
        /// </summary>
        public static Half Sub(Half lhs, Half rhs) => lhs - rhs;
        
        /// <summary>
        /// Returns a hVector3 from component-wise application of Mul (lhs * rhs).
        /// </summary>
        public static hVector3 Mul(hVector3 lhs, hVector3 rhs) => hVector3.Mul(lhs, rhs);
        
        /// <summary>
        /// Returns a hVector3 from component-wise application of Mul (lhs * rhs).
        /// </summary>
        public static Half Mul(Half lhs, Half rhs) => lhs * rhs;
        
        /// <summary>
        /// Returns a hVector3 from component-wise application of Div (lhs / rhs).
        /// </summary>
        public static hVector3 Div(hVector3 lhs, hVector3 rhs) => hVector3.Div(lhs, rhs);
        
        /// <summary>
        /// Returns a hVector3 from component-wise application of Div (lhs / rhs).
        /// </summary>
        public static Half Div(Half lhs, Half rhs) => lhs / rhs;
        
        /// <summary>
        /// Returns a hVector3 from component-wise application of Modulo (lhs % rhs).
        /// </summary>
        public static hVector3 Modulo(hVector3 lhs, hVector3 rhs) => hVector3.Modulo(lhs, rhs);
        
        /// <summary>
        /// Returns a hVector3 from component-wise application of Modulo (lhs % rhs).
        /// </summary>
        public static Half Modulo(Half lhs, Half rhs) => lhs % rhs;
        
        /// <summary>
        /// Returns a hVector3 from component-wise application of Degrees (Radians-To-Degrees Conversion).
        /// </summary>
        public static hVector3 Degrees(hVector3 v) => hVector3.Degrees(v);
        
        /// <summary>
        /// Returns a hVector3 from component-wise application of Degrees (Radians-To-Degrees Conversion).
        /// </summary>
        public static Half Degrees(Half v) => (Half)(v * new Half(57.295779513082320876798154814105170332405472466564321));
        
        /// <summary>
        /// Returns a hVector3 from component-wise application of Radians (Degrees-To-Radians Conversion).
        /// </summary>
        public static hVector3 Radians(hVector3 v) => hVector3.Radians(v);
        
        /// <summary>
        /// Returns a hVector3 from component-wise application of Radians (Degrees-To-Radians Conversion).
        /// </summary>
        public static Half Radians(Half v) => (Half)(v * new Half(0.0174532925199432957692369076848861271344287188854172));
        
        /// <summary>
        /// Returns a hVector3 from component-wise application of Acos ((Half)Math.Acos((double)v)).
        /// </summary>
        public static hVector3 Acos(hVector3 v) => hVector3.Acos(v);
        
        /// <summary>
        /// Returns a hVector3 from component-wise application of Acos ((Half)Math.Acos((double)v)).
        /// </summary>
        public static Half Acos(Half v) => (Half)Math.Acos((double)v);
        
        /// <summary>
        /// Returns a hVector3 from component-wise application of Asin ((Half)Math.Asin((double)v)).
        /// </summary>
        public static hVector3 Asin(hVector3 v) => hVector3.Asin(v);
        
        /// <summary>
        /// Returns a hVector3 from component-wise application of Asin ((Half)Math.Asin((double)v)).
        /// </summary>
        public static Half Asin(Half v) => (Half)Math.Asin((double)v);
        
        /// <summary>
        /// Returns a hVector3 from component-wise application of Atan ((Half)Math.Atan((double)v)).
        /// </summary>
        public static hVector3 Atan(hVector3 v) => hVector3.Atan(v);
        
        /// <summary>
        /// Returns a hVector3 from component-wise application of Atan ((Half)Math.Atan((double)v)).
        /// </summary>
        public static Half Atan(Half v) => (Half)Math.Atan((double)v);
        
        /// <summary>
        /// Returns a hVector3 from component-wise application of Cos ((Half)Math.Cos((double)v)).
        /// </summary>
        public static hVector3 Cos(hVector3 v) => hVector3.Cos(v);
        
        /// <summary>
        /// Returns a hVector3 from component-wise application of Cos ((Half)Math.Cos((double)v)).
        /// </summary>
        public static Half Cos(Half v) => (Half)Math.Cos((double)v);
        
        /// <summary>
        /// Returns a hVector3 from component-wise application of Cosh ((Half)Math.Cosh((double)v)).
        /// </summary>
        public static hVector3 Cosh(hVector3 v) => hVector3.Cosh(v);
        
        /// <summary>
        /// Returns a hVector3 from component-wise application of Cosh ((Half)Math.Cosh((double)v)).
        /// </summary>
        public static Half Cosh(Half v) => (Half)Math.Cosh((double)v);
        
        /// <summary>
        /// Returns a hVector3 from component-wise application of Exp ((Half)Math.Exp((double)v)).
        /// </summary>
        public static hVector3 Exp(hVector3 v) => hVector3.Exp(v);
        
        /// <summary>
        /// Returns a hVector3 from component-wise application of Exp ((Half)Math.Exp((double)v)).
        /// </summary>
        public static Half Exp(Half v) => (Half)Math.Exp((double)v);
        
        /// <summary>
        /// Returns a hVector3 from component-wise application of Log ((Half)Math.Log((double)v)).
        /// </summary>
        public static hVector3 Log(hVector3 v) => hVector3.Log(v);
        
        /// <summary>
        /// Returns a hVector3 from component-wise application of Log ((Half)Math.Log((double)v)).
        /// </summary>
        public static Half Log(Half v) => (Half)Math.Log((double)v);
        
        /// <summary>
        /// Returns a hVector3 from component-wise application of Log2 ((Half)Math.Log((double)v, 2)).
        /// </summary>
        public static hVector3 Log2(hVector3 v) => hVector3.Log2(v);
        
        /// <summary>
        /// Returns a hVector3 from component-wise application of Log2 ((Half)Math.Log((double)v, 2)).
        /// </summary>
        public static Half Log2(Half v) => (Half)Math.Log((double)v, 2);
        
        /// <summary>
        /// Returns a hVector3 from component-wise application of Log10 ((Half)Math.Log10((double)v)).
        /// </summary>
        public static hVector3 Log10(hVector3 v) => hVector3.Log10(v);
        
        /// <summary>
        /// Returns a hVector3 from component-wise application of Log10 ((Half)Math.Log10((double)v)).
        /// </summary>
        public static Half Log10(Half v) => (Half)Math.Log10((double)v);
        
        /// <summary>
        /// Returns a hVector3 from component-wise application of Floor ((Half)Math.Floor(v)).
        /// </summary>
        public static hVector3 Floor(hVector3 v) => hVector3.Floor(v);
        
        /// <summary>
        /// Returns a hVector3 from component-wise application of Floor ((Half)Math.Floor(v)).
        /// </summary>
        public static Half Floor(Half v) => (Half)Math.Floor(v);
        
        /// <summary>
        /// Returns a hVector3 from component-wise application of Ceiling ((Half)Math.Ceiling(v)).
        /// </summary>
        public static hVector3 Ceiling(hVector3 v) => hVector3.Ceiling(v);
        
        /// <summary>
        /// Returns a hVector3 from component-wise application of Ceiling ((Half)Math.Ceiling(v)).
        /// </summary>
        public static Half Ceiling(Half v) => (Half)Math.Ceiling(v);
        
        /// <summary>
        /// Returns a hVector3 from component-wise application of Round ((Half)Math.Round(v)).
        /// </summary>
        public static hVector3 Round(hVector3 v) => hVector3.Round(v);
        
        /// <summary>
        /// Returns a hVector3 from component-wise application of Round ((Half)Math.Round(v)).
        /// </summary>
        public static Half Round(Half v) => (Half)Math.Round(v);
        
        /// <summary>
        /// Returns a hVector3 from component-wise application of Sin ((Half)Math.Sin((double)v)).
        /// </summary>
        public static hVector3 Sin(hVector3 v) => hVector3.Sin(v);
        
        /// <summary>
        /// Returns a hVector3 from component-wise application of Sin ((Half)Math.Sin((double)v)).
        /// </summary>
        public static Half Sin(Half v) => (Half)Math.Sin((double)v);
        
        /// <summary>
        /// Returns a hVector3 from component-wise application of Sinh ((Half)Math.Sinh((double)v)).
        /// </summary>
        public static hVector3 Sinh(hVector3 v) => hVector3.Sinh(v);
        
        /// <summary>
        /// Returns a hVector3 from component-wise application of Sinh ((Half)Math.Sinh((double)v)).
        /// </summary>
        public static Half Sinh(Half v) => (Half)Math.Sinh((double)v);
        
        /// <summary>
        /// Returns a hVector3 from component-wise application of Tan ((Half)Math.Tan((double)v)).
        /// </summary>
        public static hVector3 Tan(hVector3 v) => hVector3.Tan(v);
        
        /// <summary>
        /// Returns a hVector3 from component-wise application of Tan ((Half)Math.Tan((double)v)).
        /// </summary>
        public static Half Tan(Half v) => (Half)Math.Tan((double)v);
        
        /// <summary>
        /// Returns a hVector3 from component-wise application of Tanh ((Half)Math.Tanh((double)v)).
        /// </summary>
        public static hVector3 Tanh(hVector3 v) => hVector3.Tanh(v);
        
        /// <summary>
        /// Returns a hVector3 from component-wise application of Tanh ((Half)Math.Tanh((double)v)).
        /// </summary>
        public static Half Tanh(Half v) => (Half)Math.Tanh((double)v);
        
        /// <summary>
        /// Returns a hVector3 from component-wise application of Truncate ((Half)Math.Truncate((double)v)).
        /// </summary>
        public static hVector3 Truncate(hVector3 v) => hVector3.Truncate(v);
        
        /// <summary>
        /// Returns a hVector3 from component-wise application of Truncate ((Half)Math.Truncate((double)v)).
        /// </summary>
        public static Half Truncate(Half v) => (Half)Math.Truncate((double)v);
        
        /// <summary>
        /// Returns a hVector3 from component-wise application of Fract ((Half)(v - Math.Floor(v))).
        /// </summary>
        public static hVector3 Fract(hVector3 v) => hVector3.Fract(v);
        
        /// <summary>
        /// Returns a hVector3 from component-wise application of Fract ((Half)(v - Math.Floor(v))).
        /// </summary>
        public static Half Fract(Half v) => (Half)(v - Math.Floor(v));
        
        /// <summary>
        /// Returns a hVector3 from component-wise application of Trunc ((long)(v)).
        /// </summary>
        public static hVector3 Trunc(hVector3 v) => hVector3.Trunc(v);
        
        /// <summary>
        /// Returns a hVector3 from component-wise application of Trunc ((long)(v)).
        /// </summary>
        public static Half Trunc(Half v) => (long)(v);
        
        /// <summary>
        /// Returns the minimal component of this Vector.
        /// </summary>
        public static Half MinElement(hVector3 v) => v.MinElement;
        
        /// <summary>
        /// Returns the maximal component of this Vector.
        /// </summary>
        public static Half MaxElement(hVector3 v) => v.MaxElement;
        
        /// <summary>
        /// Returns the euclidean length of this Vector.
        /// </summary>
        public static float Length(hVector3 v) => v.Length;
        
        /// <summary>
        /// Returns the squared euclidean length of this Vector.
        /// </summary>
        public static float LengthSqr(hVector3 v) => v.LengthSqr;
        
        /// <summary>
        /// Returns the sum of all components.
        /// </summary>
        public static Half Sum(hVector3 v) => v.Sum;
        
        /// <summary>
        /// Returns the euclidean norm of this Vector.
        /// </summary>
        public static float Norm(hVector3 v) => v.Norm;
        
        /// <summary>
        /// Returns the one-norm of this Vector.
        /// </summary>
        public static float Norm1(hVector3 v) => v.Norm1;
        
        /// <summary>
        /// Returns the two-norm (euclidean length) of this Vector.
        /// </summary>
        public static float Norm2(hVector3 v) => v.Norm2;
        
        /// <summary>
        /// Returns the max-norm of this Vector.
        /// </summary>
        public static float NormMax(hVector3 v) => v.NormMax;
        
        /// <summary>
        /// Returns the p-norm of this Vector.
        /// </summary>
        public static double NormP(hVector3 v, double p) => v.NormP(p);
        
        /// <summary>
        /// Returns a copy of this Vector with length one (undefined if this has zero length).
        /// </summary>
        public static hVector3 Normalized(hVector3 v) => v.Normalized;
        
        /// <summary>
        /// Returns a copy of this Vector with length one (returns zero if length is zero).
        /// </summary>
        public static hVector3 NormalizedSafe(hVector3 v) => v.NormalizedSafe;
        
        /// <summary>
        /// Returns the inner product (dot product, scalar product) of the two Vectors.
        /// </summary>
        public static Half Dot(hVector3 lhs, hVector3 rhs) => hVector3.Dot(lhs, rhs);
        
        /// <summary>
        /// Returns the euclidean distance between the two Vectors.
        /// </summary>
        public static float Distance(hVector3 lhs, hVector3 rhs) => hVector3.Distance(lhs, rhs);
        
        /// <summary>
        /// Returns the squared euclidean distance between the two Vectors.
        /// </summary>
        public static float DistanceSqr(hVector3 lhs, hVector3 rhs) => hVector3.DistanceSqr(lhs, rhs);
        
        /// <summary>
        /// Calculate the reflection direction for an incident Vector (N should be normalized in order to achieve the desired result).
        /// </summary>
        public static hVector3 Reflect(hVector3 I, hVector3 N) => hVector3.Reflect(I, N);
        
        /// <summary>
        /// Calculate the refraction direction for an incident Vector (The input parameters I and N should be normalized in order to achieve the desired result).
        /// </summary>
        public static hVector3 Refract(hVector3 I, hVector3 N, Half eta) => hVector3.Refract(I, N, eta);
        
        /// <summary>
        /// Returns a Vector pointing in the same direction as another (faceforward orients a Vector to point away from a surface as defined by its normal. If dot(Nref, I) is negative faceforward returns N, otherwise it returns -N).
        /// </summary>
        public static hVector3 FaceForward(hVector3 N, hVector3 I, hVector3 Nref) => hVector3.FaceForward(N, I, Nref);
        
        /// <summary>
        /// Returns the outer product (cross product, Vector product) of the two Vectors.
        /// </summary>
        public static hVector3 Cross(hVector3 l, hVector3 r) => hVector3.Cross(l, r);
        
        /// <summary>
        /// Returns a hVector3 with independent and identically distributed uniform values between 'minValue' and 'maxValue'.
        /// </summary>
        public static hVector3 Random(System.Random random, hVector3 minValue, hVector3 maxValue) => hVector3.Random(random, minValue, maxValue);
        
        /// <summary>
        /// Returns a hVector3 with independent and identically distributed uniform values between 'minValue' and 'maxValue'.
        /// </summary>
        public static Half Random(System.Random random, Half minValue, Half maxValue) => (Half)random.NextDouble() * (maxValue - minValue) + minValue;
        
        /// <summary>
        /// Returns a hVector3 with independent and identically distributed uniform values between 'minValue' and 'maxValue'.
        /// </summary>
        public static hVector3 RandomUniform(System.Random random, hVector3 minValue, hVector3 maxValue) => hVector3.RandomUniform(random, minValue, maxValue);
        
        /// <summary>
        /// Returns a hVector3 with independent and identically distributed uniform values between 'minValue' and 'maxValue'.
        /// </summary>
        public static Half RandomUniform(System.Random random, Half minValue, Half maxValue) => (Half)random.NextDouble() * (maxValue - minValue) + minValue;
        
        /// <summary>
        /// Returns a hVector3 with independent and identically distributed values according to a normal/Gaussian distribution with specified mean and variance.
        /// </summary>
        public static hVector3 RandomNormal(System.Random random, hVector3 mean, hVector3 variance) => hVector3.RandomNormal(random, mean, variance);
        
        /// <summary>
        /// Returns a hVector3 with independent and identically distributed values according to a normal/Gaussian distribution with specified mean and variance.
        /// </summary>
        public static Half RandomNormal(System.Random random, Half mean, Half variance) => (Half)(Math.Sqrt((double)variance) * Math.Cos(2 * Math.PI * random.NextDouble()) * Math.Sqrt(-2.0 * Math.Log(random.NextDouble()))) + mean;
        
        /// <summary>
        /// Returns a hVector3 with independent and identically distributed values according to a normal/Gaussian distribution with specified mean and variance.
        /// </summary>
        public static hVector3 RandomGaussian(System.Random random, hVector3 mean, hVector3 variance) => hVector3.RandomGaussian(random, mean, variance);
        
        /// <summary>
        /// Returns a hVector3 with independent and identically distributed values according to a normal/Gaussian distribution with specified mean and variance.
        /// </summary>
        public static Half RandomGaussian(System.Random random, Half mean, Half variance) => (Half)(Math.Sqrt((double)variance) * Math.Cos(2 * Math.PI * random.NextDouble()) * Math.Sqrt(-2.0 * Math.Log(random.NextDouble()))) + mean;

    }
}
