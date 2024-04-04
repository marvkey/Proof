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
        public static swizzle_dVector3 swizzle(dVector3 v) => v.swizzle;
        
        /// <summary>
        /// Returns an array with all values
        /// </summary>
        public static double[] Values(dVector3 v) => v.Values;
        
        /// <summary>
        /// Returns an enumerator that iterates through all components.
        /// </summary>
        public static IEnumerator<double> GetEnumerator(dVector3 v) => v.GetEnumerator();
        
        /// <summary>
        /// Returns a string representation of this Vectortor using ', ' as a seperator.
        /// </summary>
        public static string ToString(dVector3 v) => v.ToString();
        
        /// <summary>
        /// Returns a string representation of this Vectortor using a provided seperator.
        /// </summary>
        public static string ToString(dVector3 v, string sep) => v.ToString(sep);
        
        /// <summary>
        /// Returns a string representation of this Vectortor using a provided seperator and a forMatrix provider for each component.
        /// </summary>
        public static string ToString(dVector3 v, string sep, IFormatProvider provider) => v.ToString(sep, provider);
        
        /// <summary>
        /// Returns a string representation of this Vectortor using a provided seperator and a forMatrix for each component.
        /// </summary>
        public static string ToString(dVector3 v, string sep, string forMatrix) => v.ToString(sep, forMatrix);
        
        /// <summary>
        /// Returns a string representation of this Vectortor using a provided seperator and a forMatrix and forMatrix provider for each component.
        /// </summary>
        public static string ToString(dVector3 v, string sep, string forMatrix, IFormatProvider provider) => v.ToString(sep, forMatrix, provider);
        
        /// <summary>
        /// Returns the number of components (3).
        /// </summary>
        public static int Count(dVector3 v) => v.Count;
        
        /// <summary>
        /// Returns true iff this equals rhs component-wise.
        /// </summary>
        public static bool Equals(dVector3 v, dVector3 rhs) => v.Equals(rhs);
        
        /// <summary>
        /// Returns true iff this equals rhs type- and component-wise.
        /// </summary>
        public static bool Equals(dVector3 v, object obj) => v.Equals(obj);
        
        /// <summary>
        /// Returns a hash code for this instance.
        /// </summary>
        public static int GetHashCode(dVector3 v) => v.GetHashCode();
        
        /// <summary>
        /// Returns true iff distance between lhs and rhs is less than or equal to epsilon
        /// </summary>
        public static bool ApproxEqual(dVector3 lhs, dVector3 rhs, double eps = 0.1d) => dVector3.ApproxEqual(lhs, rhs, eps);
        
        /// <summary>
        /// Returns a bVector3 from component-wise application of Equal (lhs == rhs).
        /// </summary>
        public static bVector3 Equal(dVector3 lhs, dVector3 rhs) => dVector3.Equal(lhs, rhs);
        
        /// <summary>
        /// Returns a bVector3 from component-wise application of Equal (lhs == rhs).
        /// </summary>
        public static bool Equal(double lhs, double rhs) => lhs == rhs;
        
        /// <summary>
        /// Returns a bVector3 from component-wise application of NotEqual (lhs != rhs).
        /// </summary>
        public static bVector3 NotEqual(dVector3 lhs, dVector3 rhs) => dVector3.NotEqual(lhs, rhs);
        
        /// <summary>
        /// Returns a bVector3 from component-wise application of NotEqual (lhs != rhs).
        /// </summary>
        public static bool NotEqual(double lhs, double rhs) => lhs != rhs;
        
        /// <summary>
        /// Returns a bVector3 from component-wise application of GreaterThan (lhs &gt; rhs).
        /// </summary>
        public static bVector3 GreaterThan(dVector3 lhs, dVector3 rhs) => dVector3.GreaterThan(lhs, rhs);
        
        /// <summary>
        /// Returns a bVector3 from component-wise application of GreaterThan (lhs &gt; rhs).
        /// </summary>
        public static bool GreaterThan(double lhs, double rhs) => lhs > rhs;
        
        /// <summary>
        /// Returns a bVector3 from component-wise application of GreaterThanEqual (lhs &gt;= rhs).
        /// </summary>
        public static bVector3 GreaterThanEqual(dVector3 lhs, dVector3 rhs) => dVector3.GreaterThanEqual(lhs, rhs);
        
        /// <summary>
        /// Returns a bVector3 from component-wise application of GreaterThanEqual (lhs &gt;= rhs).
        /// </summary>
        public static bool GreaterThanEqual(double lhs, double rhs) => lhs >= rhs;
        
        /// <summary>
        /// Returns a bVector3 from component-wise application of LesserThan (lhs &lt; rhs).
        /// </summary>
        public static bVector3 LesserThan(dVector3 lhs, dVector3 rhs) => dVector3.LesserThan(lhs, rhs);
        
        /// <summary>
        /// Returns a bVector3 from component-wise application of LesserThan (lhs &lt; rhs).
        /// </summary>
        public static bool LesserThan(double lhs, double rhs) => lhs < rhs;
        
        /// <summary>
        /// Returns a bVector3 from component-wise application of LesserThanEqual (lhs &lt;= rhs).
        /// </summary>
        public static bVector3 LesserThanEqual(dVector3 lhs, dVector3 rhs) => dVector3.LesserThanEqual(lhs, rhs);
        
        /// <summary>
        /// Returns a bVector3 from component-wise application of LesserThanEqual (lhs &lt;= rhs).
        /// </summary>
        public static bool LesserThanEqual(double lhs, double rhs) => lhs <= rhs;
        
        /// <summary>
        /// Returns a bVector3 from component-wise application of IsInfinity (double.IsInfinity(v)).
        /// </summary>
        public static bVector3 IsInfinity(dVector3 v) => dVector3.IsInfinity(v);
        
        /// <summary>
        /// Returns a bVector3 from component-wise application of IsInfinity (double.IsInfinity(v)).
        /// </summary>
        public static bool IsInfinity(double v) => double.IsInfinity(v);
        
        /// <summary>
        /// Returns a bVector3 from component-wise application of IsFinite (!double.IsNaN(v) &amp;&amp; !double.IsInfinity(v)).
        /// </summary>
        public static bVector3 IsFinite(dVector3 v) => dVector3.IsFinite(v);
        
        /// <summary>
        /// Returns a bVector3 from component-wise application of IsFinite (!double.IsNaN(v) &amp;&amp; !double.IsInfinity(v)).
        /// </summary>
        public static bool IsFinite(double v) => !double.IsNaN(v) && !double.IsInfinity(v);
        
        /// <summary>
        /// Returns a bVector3 from component-wise application of IsNaN (double.IsNaN(v)).
        /// </summary>
        public static bVector3 IsNaN(dVector3 v) => dVector3.IsNaN(v);
        
        /// <summary>
        /// Returns a bVector3 from component-wise application of IsNaN (double.IsNaN(v)).
        /// </summary>
        public static bool IsNaN(double v) => double.IsNaN(v);
        
        /// <summary>
        /// Returns a bVector3 from component-wise application of IsNegativeInfinity (double.IsNegativeInfinity(v)).
        /// </summary>
        public static bVector3 IsNegativeInfinity(dVector3 v) => dVector3.IsNegativeInfinity(v);
        
        /// <summary>
        /// Returns a bVector3 from component-wise application of IsNegativeInfinity (double.IsNegativeInfinity(v)).
        /// </summary>
        public static bool IsNegativeInfinity(double v) => double.IsNegativeInfinity(v);
        
        /// <summary>
        /// Returns a bVector3 from component-wise application of IsPositiveInfinity (double.IsPositiveInfinity(v)).
        /// </summary>
        public static bVector3 IsPositiveInfinity(dVector3 v) => dVector3.IsPositiveInfinity(v);
        
        /// <summary>
        /// Returns a bVector3 from component-wise application of IsPositiveInfinity (double.IsPositiveInfinity(v)).
        /// </summary>
        public static bool IsPositiveInfinity(double v) => double.IsPositiveInfinity(v);
        
        /// <summary>
        /// Returns a dVector3 from component-wise application of Abs (Math.Abs(v)).
        /// </summary>
        public static dVector3 Abs(dVector3 v) => dVector3.Abs(v);
        
        /// <summary>
        /// Returns a dVector3 from component-wise application of Abs (Math.Abs(v)).
        /// </summary>
        public static double Abs(double v) => Math.Abs(v);
        
        /// <summary>
        /// Returns a dVector3 from component-wise application of HermiteInterpolationOrder3 ((3 - 2 * v) * v * v).
        /// </summary>
        public static dVector3 HermiteInterpolationOrder3(dVector3 v) => dVector3.HermiteInterpolationOrder3(v);
        
        /// <summary>
        /// Returns a dVector3 from component-wise application of HermiteInterpolationOrder3 ((3 - 2 * v) * v * v).
        /// </summary>
        public static double HermiteInterpolationOrder3(double v) => (3 - 2 * v) * v * v;
        
        /// <summary>
        /// Returns a dVector3 from component-wise application of HermiteInterpolationOrder5 (((6 * v - 15) * v + 10) * v * v * v).
        /// </summary>
        public static dVector3 HermiteInterpolationOrder5(dVector3 v) => dVector3.HermiteInterpolationOrder5(v);
        
        /// <summary>
        /// Returns a dVector3 from component-wise application of HermiteInterpolationOrder5 (((6 * v - 15) * v + 10) * v * v * v).
        /// </summary>
        public static double HermiteInterpolationOrder5(double v) => ((6 * v - 15) * v + 10) * v * v * v;
        
        /// <summary>
        /// Returns a dVector3 from component-wise application of Sqr (v * v).
        /// </summary>
        public static dVector3 Sqr(dVector3 v) => dVector3.Sqr(v);
        
        /// <summary>
        /// Returns a dVector3 from component-wise application of Sqr (v * v).
        /// </summary>
        public static double Sqr(double v) => v * v;
        
        /// <summary>
        /// Returns a dVector3 from component-wise application of Pow2 (v * v).
        /// </summary>
        public static dVector3 Pow2(dVector3 v) => dVector3.Pow2(v);
        
        /// <summary>
        /// Returns a dVector3 from component-wise application of Pow2 (v * v).
        /// </summary>
        public static double Pow2(double v) => v * v;
        
        /// <summary>
        /// Returns a dVector3 from component-wise application of Pow3 (v * v * v).
        /// </summary>
        public static dVector3 Pow3(dVector3 v) => dVector3.Pow3(v);
        
        /// <summary>
        /// Returns a dVector3 from component-wise application of Pow3 (v * v * v).
        /// </summary>
        public static double Pow3(double v) => v * v * v;
        
        /// <summary>
        /// Returns a dVector3 from component-wise application of Step (v &gt;= 0.0 ? 1.0 : 0.0).
        /// </summary>
        public static dVector3 Step(dVector3 v) => dVector3.Step(v);
        
        /// <summary>
        /// Returns a dVector3 from component-wise application of Step (v &gt;= 0.0 ? 1.0 : 0.0).
        /// </summary>
        public static double Step(double v) => v >= 0.0 ? 1.0 : 0.0;
        
        /// <summary>
        /// Returns a dVector3 from component-wise application of Sqrt ((double)Math.Sqrt((double)v)).
        /// </summary>
        public static dVector3 Sqrt(dVector3 v) => dVector3.Sqrt(v);
        
        /// <summary>
        /// Returns a dVector3 from component-wise application of Sqrt ((double)Math.Sqrt((double)v)).
        /// </summary>
        public static double Sqrt(double v) => (double)Math.Sqrt((double)v);
        
        /// <summary>
        /// Returns a dVector3 from component-wise application of InverseSqrt ((double)(1.0 / Math.Sqrt((double)v))).
        /// </summary>
        public static dVector3 InverseSqrt(dVector3 v) => dVector3.InverseSqrt(v);
        
        /// <summary>
        /// Returns a dVector3 from component-wise application of InverseSqrt ((double)(1.0 / Math.Sqrt((double)v))).
        /// </summary>
        public static double InverseSqrt(double v) => (double)(1.0 / Math.Sqrt((double)v));
        
        /// <summary>
        /// Returns a iVector3 from component-wise application of Sign (Math.Sign(v)).
        /// </summary>
        public static iVector3 Sign(dVector3 v) => dVector3.Sign(v);
        
        /// <summary>
        /// Returns a iVector3 from component-wise application of Sign (Math.Sign(v)).
        /// </summary>
        public static int Sign(double v) => Math.Sign(v);
        
        /// <summary>
        /// Returns a dVector3 from component-wise application of Max (Math.Max(lhs, rhs)).
        /// </summary>
        public static dVector3 Max(dVector3 lhs, dVector3 rhs) => dVector3.Max(lhs, rhs);
        
        /// <summary>
        /// Returns a dVector3 from component-wise application of Max (Math.Max(lhs, rhs)).
        /// </summary>
        public static double Max(double lhs, double rhs) => Math.Max(lhs, rhs);
        
        /// <summary>
        /// Returns a dVector3 from component-wise application of Min (Math.Min(lhs, rhs)).
        /// </summary>
        public static dVector3 Min(dVector3 lhs, dVector3 rhs) => dVector3.Min(lhs, rhs);
        
        /// <summary>
        /// Returns a dVector3 from component-wise application of Min (Math.Min(lhs, rhs)).
        /// </summary>
        public static double Min(double lhs, double rhs) => Math.Min(lhs, rhs);
        
        /// <summary>
        /// Returns a dVector3 from component-wise application of Pow ((double)Math.Pow((double)lhs, (double)rhs)).
        /// </summary>
        public static dVector3 Pow(dVector3 lhs, dVector3 rhs) => dVector3.Pow(lhs, rhs);
        
        /// <summary>
        /// Returns a dVector3 from component-wise application of Pow ((double)Math.Pow((double)lhs, (double)rhs)).
        /// </summary>
        public static double Pow(double lhs, double rhs) => (double)Math.Pow((double)lhs, (double)rhs);
        
        /// <summary>
        /// Returns a dVector3 from component-wise application of Log ((double)Math.Log((double)lhs, (double)rhs)).
        /// </summary>
        public static dVector3 Log(dVector3 lhs, dVector3 rhs) => dVector3.Log(lhs, rhs);
        
        /// <summary>
        /// Returns a dVector3 from component-wise application of Log ((double)Math.Log((double)lhs, (double)rhs)).
        /// </summary>
        public static double Log(double lhs, double rhs) => (double)Math.Log((double)lhs, (double)rhs);
        
        /// <summary>
        /// Returns a dVector3 from component-wise application of Clamp (Math.Min(Math.Max(v, min), max)).
        /// </summary>
        public static dVector3 Clamp(dVector3 v, dVector3 min, dVector3 max) => dVector3.Clamp(v, min, max);
        
        /// <summary>
        /// Returns a dVector3 from component-wise application of Clamp (Math.Min(Math.Max(v, min), max)).
        /// </summary>
        public static double Clamp(double v, double min, double max) => Math.Min(Math.Max(v, min), max);
        
        /// <summary>
        /// Returns a dVector3 from component-wise application of Mix (min * (1-a) + max * a).
        /// </summary>
        public static dVector3 Mix(dVector3 min, dVector3 max, dVector3 a) => dVector3.Mix(min, max, a);
        
        /// <summary>
        /// Returns a dVector3 from component-wise application of Mix (min * (1-a) + max * a).
        /// </summary>
        public static double Mix(double min, double max, double a) => min * (1-a) + max * a;
        
        /// <summary>
        /// Returns a dVector3 from component-wise application of Lerp (min * (1-a) + max * a).
        /// </summary>
        public static dVector3 Lerp(dVector3 min, dVector3 max, dVector3 a) => dVector3.Lerp(min, max, a);
        
        /// <summary>
        /// Returns a dVector3 from component-wise application of Lerp (min * (1-a) + max * a).
        /// </summary>
        public static double Lerp(double min, double max, double a) => min * (1-a) + max * a;
        
        /// <summary>
        /// Returns a dVector3 from component-wise application of Smoothstep (((v - edge0) / (edge1 - edge0)).Clamp().HermiteInterpolationOrder3()).
        /// </summary>
        public static dVector3 Smoothstep(dVector3 edge0, dVector3 edge1, dVector3 v) => dVector3.Smoothstep(edge0, edge1, v);
        
        /// <summary>
        /// Returns a dVector3 from component-wise application of Smoothstep (((v - edge0) / (edge1 - edge0)).Clamp().HermiteInterpolationOrder3()).
        /// </summary>
        public static double Smoothstep(double edge0, double edge1, double v) => ((v - edge0) / (edge1 - edge0)).Clamp().HermiteInterpolationOrder3();
        
        /// <summary>
        /// Returns a dVector3 from component-wise application of Smootherstep (((v - edge0) / (edge1 - edge0)).Clamp().HermiteInterpolationOrder5()).
        /// </summary>
        public static dVector3 Smootherstep(dVector3 edge0, dVector3 edge1, dVector3 v) => dVector3.Smootherstep(edge0, edge1, v);
        
        /// <summary>
        /// Returns a dVector3 from component-wise application of Smootherstep (((v - edge0) / (edge1 - edge0)).Clamp().HermiteInterpolationOrder5()).
        /// </summary>
        public static double Smootherstep(double edge0, double edge1, double v) => ((v - edge0) / (edge1 - edge0)).Clamp().HermiteInterpolationOrder5();
        
        /// <summary>
        /// Returns a dVector3 from component-wise application of Fma (a * b + c).
        /// </summary>
        public static dVector3 Fma(dVector3 a, dVector3 b, dVector3 c) => dVector3.Fma(a, b, c);
        
        /// <summary>
        /// Returns a dVector3 from component-wise application of Fma (a * b + c).
        /// </summary>
        public static double Fma(double a, double b, double c) => a * b + c;
        
        /// <summary>
        /// OuterProduct treats the first parameter c as a column Vectortor (Matrix with one column) and the second parameter r as a row Vectortor (Matrix with one row) and does a linear algebraic Matrix multiply c * r, yielding a Matrix whose number of rows is the number of components in c and whose number of columns is the number of components in r.
        /// </summary>
        public static dMatrix2x3 OuterProduct(dVector3 c, dVector2 r) => dVector3.OuterProduct(c, r);
        
        /// <summary>
        /// OuterProduct treats the first parameter c as a column Vectortor (Matrix with one column) and the second parameter r as a row Vectortor (Matrix with one row) and does a linear algebraic Matrix multiply c * r, yielding a Matrix whose number of rows is the number of components in c and whose number of columns is the number of components in r.
        /// </summary>
        public static dMatrix3 OuterProduct(dVector3 c, dVector3 r) => dVector3.OuterProduct(c, r);
        
        /// <summary>
        /// OuterProduct treats the first parameter c as a column Vectortor (Matrix with one column) and the second parameter r as a row Vectortor (Matrix with one row) and does a linear algebraic Matrix multiply c * r, yielding a Matrix whose number of rows is the number of components in c and whose number of columns is the number of components in r.
        /// </summary>
        public static dMatrix4x3 OuterProduct(dVector3 c, dVector4 r) => dVector3.OuterProduct(c, r);
        
        /// <summary>
        /// Returns a dVector3 from component-wise application of Add (lhs + rhs).
        /// </summary>
        public static dVector3 Add(dVector3 lhs, dVector3 rhs) => dVector3.Add(lhs, rhs);
        
        /// <summary>
        /// Returns a dVector3 from component-wise application of Add (lhs + rhs).
        /// </summary>
        public static double Add(double lhs, double rhs) => lhs + rhs;
        
        /// <summary>
        /// Returns a dVector3 from component-wise application of Sub (lhs - rhs).
        /// </summary>
        public static dVector3 Sub(dVector3 lhs, dVector3 rhs) => dVector3.Sub(lhs, rhs);
        
        /// <summary>
        /// Returns a dVector3 from component-wise application of Sub (lhs - rhs).
        /// </summary>
        public static double Sub(double lhs, double rhs) => lhs - rhs;
        
        /// <summary>
        /// Returns a dVector3 from component-wise application of Mul (lhs * rhs).
        /// </summary>
        public static dVector3 Mul(dVector3 lhs, dVector3 rhs) => dVector3.Mul(lhs, rhs);
        
        /// <summary>
        /// Returns a dVector3 from component-wise application of Mul (lhs * rhs).
        /// </summary>
        public static double Mul(double lhs, double rhs) => lhs * rhs;
        
        /// <summary>
        /// Returns a dVector3 from component-wise application of Div (lhs / rhs).
        /// </summary>
        public static dVector3 Div(dVector3 lhs, dVector3 rhs) => dVector3.Div(lhs, rhs);
        
        /// <summary>
        /// Returns a dVector3 from component-wise application of Div (lhs / rhs).
        /// </summary>
        public static double Div(double lhs, double rhs) => lhs / rhs;
        
        /// <summary>
        /// Returns a dVector3 from component-wise application of Modulo (lhs % rhs).
        /// </summary>
        public static dVector3 Modulo(dVector3 lhs, dVector3 rhs) => dVector3.Modulo(lhs, rhs);
        
        /// <summary>
        /// Returns a dVector3 from component-wise application of Modulo (lhs % rhs).
        /// </summary>
        public static double Modulo(double lhs, double rhs) => lhs % rhs;
        
        /// <summary>
        /// Returns a dVector3 from component-wise application of Degrees (Radians-To-Degrees Conversion).
        /// </summary>
        public static dVector3 Degrees(dVector3 v) => dVector3.Degrees(v);
        
        /// <summary>
        /// Returns a dVector3 from component-wise application of Degrees (Radians-To-Degrees Conversion).
        /// </summary>
        public static double Degrees(double v) => (double)(v * 57.295779513082320876798154814105170332405472466564321d);
        
        /// <summary>
        /// Returns a dVector3 from component-wise application of Radians (Degrees-To-Radians Conversion).
        /// </summary>
        public static dVector3 Radians(dVector3 v) => dVector3.Radians(v);
        
        /// <summary>
        /// Returns a dVector3 from component-wise application of Radians (Degrees-To-Radians Conversion).
        /// </summary>
        public static double Radians(double v) => (double)(v * 0.0174532925199432957692369076848861271344287188854172d);
        
        /// <summary>
        /// Returns a dVector3 from component-wise application of Acos ((double)Math.Acos((double)v)).
        /// </summary>
        public static dVector3 Acos(dVector3 v) => dVector3.Acos(v);
        
        /// <summary>
        /// Returns a dVector3 from component-wise application of Acos ((double)Math.Acos((double)v)).
        /// </summary>
        public static double Acos(double v) => (double)Math.Acos((double)v);
        
        /// <summary>
        /// Returns a dVector3 from component-wise application of Asin ((double)Math.Asin((double)v)).
        /// </summary>
        public static dVector3 Asin(dVector3 v) => dVector3.Asin(v);
        
        /// <summary>
        /// Returns a dVector3 from component-wise application of Asin ((double)Math.Asin((double)v)).
        /// </summary>
        public static double Asin(double v) => (double)Math.Asin((double)v);
        
        /// <summary>
        /// Returns a dVector3 from component-wise application of Atan ((double)Math.Atan((double)v)).
        /// </summary>
        public static dVector3 Atan(dVector3 v) => dVector3.Atan(v);
        
        /// <summary>
        /// Returns a dVector3 from component-wise application of Atan ((double)Math.Atan((double)v)).
        /// </summary>
        public static double Atan(double v) => (double)Math.Atan((double)v);
        
        /// <summary>
        /// Returns a dVector3 from component-wise application of Cos ((double)Math.Cos((double)v)).
        /// </summary>
        public static dVector3 Cos(dVector3 v) => dVector3.Cos(v);
        
        /// <summary>
        /// Returns a dVector3 from component-wise application of Cos ((double)Math.Cos((double)v)).
        /// </summary>
        public static double Cos(double v) => (double)Math.Cos((double)v);
        
        /// <summary>
        /// Returns a dVector3 from component-wise application of Cosh ((double)Math.Cosh((double)v)).
        /// </summary>
        public static dVector3 Cosh(dVector3 v) => dVector3.Cosh(v);
        
        /// <summary>
        /// Returns a dVector3 from component-wise application of Cosh ((double)Math.Cosh((double)v)).
        /// </summary>
        public static double Cosh(double v) => (double)Math.Cosh((double)v);
        
        /// <summary>
        /// Returns a dVector3 from component-wise application of Exp ((double)Math.Exp((double)v)).
        /// </summary>
        public static dVector3 Exp(dVector3 v) => dVector3.Exp(v);
        
        /// <summary>
        /// Returns a dVector3 from component-wise application of Exp ((double)Math.Exp((double)v)).
        /// </summary>
        public static double Exp(double v) => (double)Math.Exp((double)v);
        
        /// <summary>
        /// Returns a dVector3 from component-wise application of Log ((double)Math.Log((double)v)).
        /// </summary>
        public static dVector3 Log(dVector3 v) => dVector3.Log(v);
        
        /// <summary>
        /// Returns a dVector3 from component-wise application of Log ((double)Math.Log((double)v)).
        /// </summary>
        public static double Log(double v) => (double)Math.Log((double)v);
        
        /// <summary>
        /// Returns a dVector3 from component-wise application of Log2 ((double)Math.Log((double)v, 2)).
        /// </summary>
        public static dVector3 Log2(dVector3 v) => dVector3.Log2(v);
        
        /// <summary>
        /// Returns a dVector3 from component-wise application of Log2 ((double)Math.Log((double)v, 2)).
        /// </summary>
        public static double Log2(double v) => (double)Math.Log((double)v, 2);
        
        /// <summary>
        /// Returns a dVector3 from component-wise application of Log10 ((double)Math.Log10((double)v)).
        /// </summary>
        public static dVector3 Log10(dVector3 v) => dVector3.Log10(v);
        
        /// <summary>
        /// Returns a dVector3 from component-wise application of Log10 ((double)Math.Log10((double)v)).
        /// </summary>
        public static double Log10(double v) => (double)Math.Log10((double)v);
        
        /// <summary>
        /// Returns a dVector3 from component-wise application of Floor ((double)Math.Floor(v)).
        /// </summary>
        public static dVector3 Floor(dVector3 v) => dVector3.Floor(v);
        
        /// <summary>
        /// Returns a dVector3 from component-wise application of Floor ((double)Math.Floor(v)).
        /// </summary>
        public static double Floor(double v) => (double)Math.Floor(v);
        
        /// <summary>
        /// Returns a dVector3 from component-wise application of Ceiling ((double)Math.Ceiling(v)).
        /// </summary>
        public static dVector3 Ceiling(dVector3 v) => dVector3.Ceiling(v);
        
        /// <summary>
        /// Returns a dVector3 from component-wise application of Ceiling ((double)Math.Ceiling(v)).
        /// </summary>
        public static double Ceiling(double v) => (double)Math.Ceiling(v);
        
        /// <summary>
        /// Returns a dVector3 from component-wise application of Round ((double)Math.Round(v)).
        /// </summary>
        public static dVector3 Round(dVector3 v) => dVector3.Round(v);
        
        /// <summary>
        /// Returns a dVector3 from component-wise application of Round ((double)Math.Round(v)).
        /// </summary>
        public static double Round(double v) => (double)Math.Round(v);
        
        /// <summary>
        /// Returns a dVector3 from component-wise application of Sin ((double)Math.Sin((double)v)).
        /// </summary>
        public static dVector3 Sin(dVector3 v) => dVector3.Sin(v);
        
        /// <summary>
        /// Returns a dVector3 from component-wise application of Sin ((double)Math.Sin((double)v)).
        /// </summary>
        public static double Sin(double v) => (double)Math.Sin((double)v);
        
        /// <summary>
        /// Returns a dVector3 from component-wise application of Sinh ((double)Math.Sinh((double)v)).
        /// </summary>
        public static dVector3 Sinh(dVector3 v) => dVector3.Sinh(v);
        
        /// <summary>
        /// Returns a dVector3 from component-wise application of Sinh ((double)Math.Sinh((double)v)).
        /// </summary>
        public static double Sinh(double v) => (double)Math.Sinh((double)v);
        
        /// <summary>
        /// Returns a dVector3 from component-wise application of Tan ((double)Math.Tan((double)v)).
        /// </summary>
        public static dVector3 Tan(dVector3 v) => dVector3.Tan(v);
        
        /// <summary>
        /// Returns a dVector3 from component-wise application of Tan ((double)Math.Tan((double)v)).
        /// </summary>
        public static double Tan(double v) => (double)Math.Tan((double)v);
        
        /// <summary>
        /// Returns a dVector3 from component-wise application of Tanh ((double)Math.Tanh((double)v)).
        /// </summary>
        public static dVector3 Tanh(dVector3 v) => dVector3.Tanh(v);
        
        /// <summary>
        /// Returns a dVector3 from component-wise application of Tanh ((double)Math.Tanh((double)v)).
        /// </summary>
        public static double Tanh(double v) => (double)Math.Tanh((double)v);
        
        /// <summary>
        /// Returns a dVector3 from component-wise application of Truncate ((double)Math.Truncate((double)v)).
        /// </summary>
        public static dVector3 Truncate(dVector3 v) => dVector3.Truncate(v);
        
        /// <summary>
        /// Returns a dVector3 from component-wise application of Truncate ((double)Math.Truncate((double)v)).
        /// </summary>
        public static double Truncate(double v) => (double)Math.Truncate((double)v);
        
        /// <summary>
        /// Returns a dVector3 from component-wise application of Fract ((double)(v - Math.Floor(v))).
        /// </summary>
        public static dVector3 Fract(dVector3 v) => dVector3.Fract(v);
        
        /// <summary>
        /// Returns a dVector3 from component-wise application of Fract ((double)(v - Math.Floor(v))).
        /// </summary>
        public static double Fract(double v) => (double)(v - Math.Floor(v));
        
        /// <summary>
        /// Returns a dVector3 from component-wise application of Trunc ((long)(v)).
        /// </summary>
        public static dVector3 Trunc(dVector3 v) => dVector3.Trunc(v);
        
        /// <summary>
        /// Returns a dVector3 from component-wise application of Trunc ((long)(v)).
        /// </summary>
        public static double Trunc(double v) => (long)(v);
        
        /// <summary>
        /// Returns the minimal component of this Vectortor.
        /// </summary>
        public static double MinElement(dVector3 v) => v.MinElement;
        
        /// <summary>
        /// Returns the maximal component of this Vectortor.
        /// </summary>
        public static double MaxElement(dVector3 v) => v.MaxElement;
        
        /// <summary>
        /// Returns the euclidean length of this Vectortor.
        /// </summary>
        public static double Length(dVector3 v) => v.Length;
        
        /// <summary>
        /// Returns the squared euclidean length of this Vectortor.
        /// </summary>
        public static double LengthSqr(dVector3 v) => v.LengthSqr;
        
        /// <summary>
        /// Returns the sum of all components.
        /// </summary>
        public static double Sum(dVector3 v) => v.Sum;
        
        /// <summary>
        /// Returns the euclidean norm of this Vectortor.
        /// </summary>
        public static double Norm(dVector3 v) => v.Norm;
        
        /// <summary>
        /// Returns the one-norm of this Vectortor.
        /// </summary>
        public static double Norm1(dVector3 v) => v.Norm1;
        
        /// <summary>
        /// Returns the two-norm (euclidean length) of this Vectortor.
        /// </summary>
        public static double Norm2(dVector3 v) => v.Norm2;
        
        /// <summary>
        /// Returns the max-norm of this Vectortor.
        /// </summary>
        public static double NormMax(dVector3 v) => v.NormMax;
        
        /// <summary>
        /// Returns the p-norm of this Vectortor.
        /// </summary>
        public static double NormP(dVector3 v, double p) => v.NormP(p);
        
        /// <summary>
        /// Returns a copy of this Vectortor with length one (undefined if this has zero length).
        /// </summary>
        public static dVector3 Normalized(dVector3 v) => v.Normalized;
        
        /// <summary>
        /// Returns a copy of this Vectortor with length one (returns zero if length is zero).
        /// </summary>
        public static dVector3 NormalizedSafe(dVector3 v) => v.NormalizedSafe;
        
        /// <summary>
        /// Returns the inner product (dot product, scalar product) of the two Vectortors.
        /// </summary>
        public static double Dot(dVector3 lhs, dVector3 rhs) => dVector3.Dot(lhs, rhs);
        
        /// <summary>
        /// Returns the euclidean distance between the two Vectortors.
        /// </summary>
        public static double Distance(dVector3 lhs, dVector3 rhs) => dVector3.Distance(lhs, rhs);
        
        /// <summary>
        /// Returns the squared euclidean distance between the two Vectortors.
        /// </summary>
        public static double DistanceSqr(dVector3 lhs, dVector3 rhs) => dVector3.DistanceSqr(lhs, rhs);
        
        /// <summary>
        /// Calculate the reflection direction for an incident Vectortor (N should be normalized in order to achieve the desired result).
        /// </summary>
        public static dVector3 Reflect(dVector3 I, dVector3 N) => dVector3.Reflect(I, N);
        
        /// <summary>
        /// Calculate the refraction direction for an incident Vectortor (The input parameters I and N should be normalized in order to achieve the desired result).
        /// </summary>
        public static dVector3 Refract(dVector3 I, dVector3 N, double eta) => dVector3.Refract(I, N, eta);
        
        /// <summary>
        /// Returns a Vectortor pointing in the same direction as another (faceforward orients a Vectortor to point away from a surface as defined by its normal. If dot(Nref, I) is negative faceforward returns N, otherwise it returns -N).
        /// </summary>
        public static dVector3 FaceForward(dVector3 N, dVector3 I, dVector3 Nref) => dVector3.FaceForward(N, I, Nref);
        
        /// <summary>
        /// Returns the outer product (cross product, Vectortor product) of the two Vectortors.
        /// </summary>
        public static dVector3 Cross(dVector3 l, dVector3 r) => dVector3.Cross(l, r);
        
        /// <summary>
        /// Returns a dVector3 with independent and identically distributed uniform values between 'minValue' and 'maxValue'.
        /// </summary>
        public static dVector3 Random(System.Random random, dVector3 minValue, dVector3 maxValue) => dVector3.Random(random, minValue, maxValue);
        
        /// <summary>
        /// Returns a dVector3 with independent and identically distributed uniform values between 'minValue' and 'maxValue'.
        /// </summary>
        public static double Random(System.Random random, double minValue, double maxValue) => (double)random.NextDouble() * (maxValue - minValue) + minValue;
        
        /// <summary>
        /// Returns a dVector3 with independent and identically distributed uniform values between 'minValue' and 'maxValue'.
        /// </summary>
        public static dVector3 RandomUniform(System.Random random, dVector3 minValue, dVector3 maxValue) => dVector3.RandomUniform(random, minValue, maxValue);
        
        /// <summary>
        /// Returns a dVector3 with independent and identically distributed uniform values between 'minValue' and 'maxValue'.
        /// </summary>
        public static double RandomUniform(System.Random random, double minValue, double maxValue) => (double)random.NextDouble() * (maxValue - minValue) + minValue;
        
        /// <summary>
        /// Returns a dVector3 with independent and identically distributed values according to a normal/Gaussian distribution with specified mean and variance.
        /// </summary>
        public static dVector3 RandomNormal(System.Random random, dVector3 mean, dVector3 variance) => dVector3.RandomNormal(random, mean, variance);
        
        /// <summary>
        /// Returns a dVector3 with independent and identically distributed values according to a normal/Gaussian distribution with specified mean and variance.
        /// </summary>
        public static double RandomNormal(System.Random random, double mean, double variance) => (double)(Math.Sqrt((double)variance) * Math.Cos(2 * Math.PI * random.NextDouble()) * Math.Sqrt(-2.0 * Math.Log(random.NextDouble()))) + mean;
        
        /// <summary>
        /// Returns a dVector3 with independent and identically distributed values according to a normal/Gaussian distribution with specified mean and variance.
        /// </summary>
        public static dVector3 RandomGaussian(System.Random random, dVector3 mean, dVector3 variance) => dVector3.RandomGaussian(random, mean, variance);
        
        /// <summary>
        /// Returns a dVector3 with independent and identically distributed values according to a normal/Gaussian distribution with specified mean and variance.
        /// </summary>
        public static double RandomGaussian(System.Random random, double mean, double variance) => (double)(Math.Sqrt((double)variance) * Math.Cos(2 * Math.PI * random.NextDouble()) * Math.Sqrt(-2.0 * Math.Log(random.NextDouble()))) + mean;

    }
}
