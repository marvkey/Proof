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
        public static swizzle_decVector3 swizzle(decVector3 v) => v.swizzle;
        
        /// <summary>
        /// Returns an array with all values
        /// </summary>
        public static decimal[] Values(decVector3 v) => v.Values;
        
        /// <summary>
        /// Returns an enumerator that iterates through all components.
        /// </summary>
        public static IEnumerator<decimal> GetEnumerator(decVector3 v) => v.GetEnumerator();
        
        /// <summary>
        /// Returns a string representation of this Vector using ', ' as a seperator.
        /// </summary>
        public static string ToString(decVector3 v) => v.ToString();
        
        /// <summary>
        /// Returns a string representation of this Vector using a provided seperator.
        /// </summary>
        public static string ToString(decVector3 v, string sep) => v.ToString(sep);
        
        /// <summary>
        /// Returns a string representation of this Vector using a provided seperator and a forMatrix provider for each component.
        /// </summary>
        public static string ToString(decVector3 v, string sep, IFormatProvider provider) => v.ToString(sep, provider);
        
        /// <summary>
        /// Returns a string representation of this Vector using a provided seperator and a forMatrix for each component.
        /// </summary>
        public static string ToString(decVector3 v, string sep, string forMatrix) => v.ToString(sep, forMatrix);
        
        /// <summary>
        /// Returns a string representation of this Vector using a provided seperator and a forMatrix and forMatrix provider for each component.
        /// </summary>
        public static string ToString(decVector3 v, string sep, string forMatrix, IFormatProvider provider) => v.ToString(sep, forMatrix, provider);
        
        /// <summary>
        /// Returns the number of components (3).
        /// </summary>
        public static int Count(decVector3 v) => v.Count;
        
        /// <summary>
        /// Returns true iff this equals rhs component-wise.
        /// </summary>
        public static bool Equals(decVector3 v, decVector3 rhs) => v.Equals(rhs);
        
        /// <summary>
        /// Returns true iff this equals rhs type- and component-wise.
        /// </summary>
        public static bool Equals(decVector3 v, object obj) => v.Equals(obj);
        
        /// <summary>
        /// Returns a hash code for this instance.
        /// </summary>
        public static int GetHashCode(decVector3 v) => v.GetHashCode();
        
        /// <summary>
        /// Returns true iff distance between lhs and rhs is less than or equal to epsilon
        /// </summary>
        public static bool ApproxEqual(decVector3 lhs, decVector3 rhs, decimal eps = 0.1m) => decVector3.ApproxEqual(lhs, rhs, eps);
        
        /// <summary>
        /// Returns a bVector3 from component-wise application of Equal (lhs == rhs).
        /// </summary>
        public static bVector3 Equal(decVector3 lhs, decVector3 rhs) => decVector3.Equal(lhs, rhs);
        
        /// <summary>
        /// Returns a bVector3 from component-wise application of Equal (lhs == rhs).
        /// </summary>
        public static bool Equal(decimal lhs, decimal rhs) => lhs == rhs;
        
        /// <summary>
        /// Returns a bVector3 from component-wise application of NotEqual (lhs != rhs).
        /// </summary>
        public static bVector3 NotEqual(decVector3 lhs, decVector3 rhs) => decVector3.NotEqual(lhs, rhs);
        
        /// <summary>
        /// Returns a bVector3 from component-wise application of NotEqual (lhs != rhs).
        /// </summary>
        public static bool NotEqual(decimal lhs, decimal rhs) => lhs != rhs;
        
        /// <summary>
        /// Returns a bVector3 from component-wise application of GreaterThan (lhs &gt; rhs).
        /// </summary>
        public static bVector3 GreaterThan(decVector3 lhs, decVector3 rhs) => decVector3.GreaterThan(lhs, rhs);
        
        /// <summary>
        /// Returns a bVector3 from component-wise application of GreaterThan (lhs &gt; rhs).
        /// </summary>
        public static bool GreaterThan(decimal lhs, decimal rhs) => lhs > rhs;
        
        /// <summary>
        /// Returns a bVector3 from component-wise application of GreaterThanEqual (lhs &gt;= rhs).
        /// </summary>
        public static bVector3 GreaterThanEqual(decVector3 lhs, decVector3 rhs) => decVector3.GreaterThanEqual(lhs, rhs);
        
        /// <summary>
        /// Returns a bVector3 from component-wise application of GreaterThanEqual (lhs &gt;= rhs).
        /// </summary>
        public static bool GreaterThanEqual(decimal lhs, decimal rhs) => lhs >= rhs;
        
        /// <summary>
        /// Returns a bVector3 from component-wise application of LesserThan (lhs &lt; rhs).
        /// </summary>
        public static bVector3 LesserThan(decVector3 lhs, decVector3 rhs) => decVector3.LesserThan(lhs, rhs);
        
        /// <summary>
        /// Returns a bVector3 from component-wise application of LesserThan (lhs &lt; rhs).
        /// </summary>
        public static bool LesserThan(decimal lhs, decimal rhs) => lhs < rhs;
        
        /// <summary>
        /// Returns a bVector3 from component-wise application of LesserThanEqual (lhs &lt;= rhs).
        /// </summary>
        public static bVector3 LesserThanEqual(decVector3 lhs, decVector3 rhs) => decVector3.LesserThanEqual(lhs, rhs);
        
        /// <summary>
        /// Returns a bVector3 from component-wise application of LesserThanEqual (lhs &lt;= rhs).
        /// </summary>
        public static bool LesserThanEqual(decimal lhs, decimal rhs) => lhs <= rhs;
        
        /// <summary>
        /// Returns a decVector3 from component-wise application of Abs (Math.Abs(v)).
        /// </summary>
        public static decVector3 Abs(decVector3 v) => decVector3.Abs(v);
        
        /// <summary>
        /// Returns a decVector3 from component-wise application of Abs (Math.Abs(v)).
        /// </summary>
        public static decimal Abs(decimal v) => Math.Abs(v);
        
        /// <summary>
        /// Returns a decVector3 from component-wise application of HermiteInterpolationOrder3 ((3 - 2 * v) * v * v).
        /// </summary>
        public static decVector3 HermiteInterpolationOrder3(decVector3 v) => decVector3.HermiteInterpolationOrder3(v);
        
        /// <summary>
        /// Returns a decVector3 from component-wise application of HermiteInterpolationOrder3 ((3 - 2 * v) * v * v).
        /// </summary>
        public static decimal HermiteInterpolationOrder3(decimal v) => (3 - 2 * v) * v * v;
        
        /// <summary>
        /// Returns a decVector3 from component-wise application of HermiteInterpolationOrder5 (((6 * v - 15) * v + 10) * v * v * v).
        /// </summary>
        public static decVector3 HermiteInterpolationOrder5(decVector3 v) => decVector3.HermiteInterpolationOrder5(v);
        
        /// <summary>
        /// Returns a decVector3 from component-wise application of HermiteInterpolationOrder5 (((6 * v - 15) * v + 10) * v * v * v).
        /// </summary>
        public static decimal HermiteInterpolationOrder5(decimal v) => ((6 * v - 15) * v + 10) * v * v * v;
        
        /// <summary>
        /// Returns a decVector3 from component-wise application of Sqr (v * v).
        /// </summary>
        public static decVector3 Sqr(decVector3 v) => decVector3.Sqr(v);
        
        /// <summary>
        /// Returns a decVector3 from component-wise application of Sqr (v * v).
        /// </summary>
        public static decimal Sqr(decimal v) => v * v;
        
        /// <summary>
        /// Returns a decVector3 from component-wise application of Pow2 (v * v).
        /// </summary>
        public static decVector3 Pow2(decVector3 v) => decVector3.Pow2(v);
        
        /// <summary>
        /// Returns a decVector3 from component-wise application of Pow2 (v * v).
        /// </summary>
        public static decimal Pow2(decimal v) => v * v;
        
        /// <summary>
        /// Returns a decVector3 from component-wise application of Pow3 (v * v * v).
        /// </summary>
        public static decVector3 Pow3(decVector3 v) => decVector3.Pow3(v);
        
        /// <summary>
        /// Returns a decVector3 from component-wise application of Pow3 (v * v * v).
        /// </summary>
        public static decimal Pow3(decimal v) => v * v * v;
        
        /// <summary>
        /// Returns a decVector3 from component-wise application of Step (v &gt;= 0m ? 1m : 0m).
        /// </summary>
        public static decVector3 Step(decVector3 v) => decVector3.Step(v);
        
        /// <summary>
        /// Returns a decVector3 from component-wise application of Step (v &gt;= 0m ? 1m : 0m).
        /// </summary>
        public static decimal Step(decimal v) => v >= 0m ? 1m : 0m;
        
        /// <summary>
        /// Returns a decVector3 from component-wise application of Sqrt ((decimal)Math.Sqrt((double)v)).
        /// </summary>
        public static decVector3 Sqrt(decVector3 v) => decVector3.Sqrt(v);
        
        /// <summary>
        /// Returns a decVector3 from component-wise application of Sqrt ((decimal)Math.Sqrt((double)v)).
        /// </summary>
        public static decimal Sqrt(decimal v) => (decimal)Math.Sqrt((double)v);
        
        /// <summary>
        /// Returns a decVector3 from component-wise application of InverseSqrt ((decimal)(1.0 / Math.Sqrt((double)v))).
        /// </summary>
        public static decVector3 InverseSqrt(decVector3 v) => decVector3.InverseSqrt(v);
        
        /// <summary>
        /// Returns a decVector3 from component-wise application of InverseSqrt ((decimal)(1.0 / Math.Sqrt((double)v))).
        /// </summary>
        public static decimal InverseSqrt(decimal v) => (decimal)(1.0 / Math.Sqrt((double)v));
        
        /// <summary>
        /// Returns a iVector3 from component-wise application of Sign (Math.Sign(v)).
        /// </summary>
        public static iVector3 Sign(decVector3 v) => decVector3.Sign(v);
        
        /// <summary>
        /// Returns a iVector3 from component-wise application of Sign (Math.Sign(v)).
        /// </summary>
        public static int Sign(decimal v) => Math.Sign(v);
        
        /// <summary>
        /// Returns a decVector3 from component-wise application of Max (Math.Max(lhs, rhs)).
        /// </summary>
        public static decVector3 Max(decVector3 lhs, decVector3 rhs) => decVector3.Max(lhs, rhs);
        
        /// <summary>
        /// Returns a decVector3 from component-wise application of Max (Math.Max(lhs, rhs)).
        /// </summary>
        public static decimal Max(decimal lhs, decimal rhs) => Math.Max(lhs, rhs);
        
        /// <summary>
        /// Returns a decVector3 from component-wise application of Min (Math.Min(lhs, rhs)).
        /// </summary>
        public static decVector3 Min(decVector3 lhs, decVector3 rhs) => decVector3.Min(lhs, rhs);
        
        /// <summary>
        /// Returns a decVector3 from component-wise application of Min (Math.Min(lhs, rhs)).
        /// </summary>
        public static decimal Min(decimal lhs, decimal rhs) => Math.Min(lhs, rhs);
        
        /// <summary>
        /// Returns a decVector3 from component-wise application of Pow ((decimal)Math.Pow((double)lhs, (double)rhs)).
        /// </summary>
        public static decVector3 Pow(decVector3 lhs, decVector3 rhs) => decVector3.Pow(lhs, rhs);
        
        /// <summary>
        /// Returns a decVector3 from component-wise application of Pow ((decimal)Math.Pow((double)lhs, (double)rhs)).
        /// </summary>
        public static decimal Pow(decimal lhs, decimal rhs) => (decimal)Math.Pow((double)lhs, (double)rhs);
        
        /// <summary>
        /// Returns a decVector3 from component-wise application of Log ((decimal)Math.Log((double)lhs, (double)rhs)).
        /// </summary>
        public static decVector3 Log(decVector3 lhs, decVector3 rhs) => decVector3.Log(lhs, rhs);
        
        /// <summary>
        /// Returns a decVector3 from component-wise application of Log ((decimal)Math.Log((double)lhs, (double)rhs)).
        /// </summary>
        public static decimal Log(decimal lhs, decimal rhs) => (decimal)Math.Log((double)lhs, (double)rhs);
        
        /// <summary>
        /// Returns a decVector3 from component-wise application of Clamp (Math.Min(Math.Max(v, min), max)).
        /// </summary>
        public static decVector3 Clamp(decVector3 v, decVector3 min, decVector3 max) => decVector3.Clamp(v, min, max);
        
        /// <summary>
        /// Returns a decVector3 from component-wise application of Clamp (Math.Min(Math.Max(v, min), max)).
        /// </summary>
        public static decimal Clamp(decimal v, decimal min, decimal max) => Math.Min(Math.Max(v, min), max);
        
        /// <summary>
        /// Returns a decVector3 from component-wise application of Mix (min * (1-a) + max * a).
        /// </summary>
        public static decVector3 Mix(decVector3 min, decVector3 max, decVector3 a) => decVector3.Mix(min, max, a);
        
        /// <summary>
        /// Returns a decVector3 from component-wise application of Mix (min * (1-a) + max * a).
        /// </summary>
        public static decimal Mix(decimal min, decimal max, decimal a) => min * (1-a) + max * a;
        
        /// <summary>
        /// Returns a decVector3 from component-wise application of Lerp (min * (1-a) + max * a).
        /// </summary>
        public static decVector3 Lerp(decVector3 min, decVector3 max, decVector3 a) => decVector3.Lerp(min, max, a);
        
        /// <summary>
        /// Returns a decVector3 from component-wise application of Lerp (min * (1-a) + max * a).
        /// </summary>
        public static decimal Lerp(decimal min, decimal max, decimal a) => min * (1-a) + max * a;
        
        /// <summary>
        /// Returns a decVector3 from component-wise application of Smoothstep (((v - edge0) / (edge1 - edge0)).Clamp().HermiteInterpolationOrder3()).
        /// </summary>
        public static decVector3 Smoothstep(decVector3 edge0, decVector3 edge1, decVector3 v) => decVector3.Smoothstep(edge0, edge1, v);
        
        /// <summary>
        /// Returns a decVector3 from component-wise application of Smoothstep (((v - edge0) / (edge1 - edge0)).Clamp().HermiteInterpolationOrder3()).
        /// </summary>
        public static decimal Smoothstep(decimal edge0, decimal edge1, decimal v) => ((v - edge0) / (edge1 - edge0)).Clamp().HermiteInterpolationOrder3();
        
        /// <summary>
        /// Returns a decVector3 from component-wise application of Smootherstep (((v - edge0) / (edge1 - edge0)).Clamp().HermiteInterpolationOrder5()).
        /// </summary>
        public static decVector3 Smootherstep(decVector3 edge0, decVector3 edge1, decVector3 v) => decVector3.Smootherstep(edge0, edge1, v);
        
        /// <summary>
        /// Returns a decVector3 from component-wise application of Smootherstep (((v - edge0) / (edge1 - edge0)).Clamp().HermiteInterpolationOrder5()).
        /// </summary>
        public static decimal Smootherstep(decimal edge0, decimal edge1, decimal v) => ((v - edge0) / (edge1 - edge0)).Clamp().HermiteInterpolationOrder5();
        
        /// <summary>
        /// Returns a decVector3 from component-wise application of Fma (a * b + c).
        /// </summary>
        public static decVector3 Fma(decVector3 a, decVector3 b, decVector3 c) => decVector3.Fma(a, b, c);
        
        /// <summary>
        /// Returns a decVector3 from component-wise application of Fma (a * b + c).
        /// </summary>
        public static decimal Fma(decimal a, decimal b, decimal c) => a * b + c;
        
        /// <summary>
        /// OuterProduct treats the first parameter c as a column Vector (Matrix with one column) and the second parameter r as a row Vector (Matrix with one row) and does a linear algebraic Matrix multiply c * r, yielding a Matrix whose number of rows is the number of components in c and whose number of columns is the number of components in r.
        /// </summary>
        public static decMatrix2x3 OuterProduct(decVector3 c, decVector2 r) => decVector3.OuterProduct(c, r);
        
        /// <summary>
        /// OuterProduct treats the first parameter c as a column Vector (Matrix with one column) and the second parameter r as a row Vector (Matrix with one row) and does a linear algebraic Matrix multiply c * r, yielding a Matrix whose number of rows is the number of components in c and whose number of columns is the number of components in r.
        /// </summary>
        public static decMatrix3 OuterProduct(decVector3 c, decVector3 r) => decVector3.OuterProduct(c, r);
        
        /// <summary>
        /// OuterProduct treats the first parameter c as a column Vector (Matrix with one column) and the second parameter r as a row Vector (Matrix with one row) and does a linear algebraic Matrix multiply c * r, yielding a Matrix whose number of rows is the number of components in c and whose number of columns is the number of components in r.
        /// </summary>
        public static decMatrix4x3 OuterProduct(decVector3 c, decVector4 r) => decVector3.OuterProduct(c, r);
        
        /// <summary>
        /// Returns a decVector3 from component-wise application of Add (lhs + rhs).
        /// </summary>
        public static decVector3 Add(decVector3 lhs, decVector3 rhs) => decVector3.Add(lhs, rhs);
        
        /// <summary>
        /// Returns a decVector3 from component-wise application of Add (lhs + rhs).
        /// </summary>
        public static decimal Add(decimal lhs, decimal rhs) => lhs + rhs;
        
        /// <summary>
        /// Returns a decVector3 from component-wise application of Sub (lhs - rhs).
        /// </summary>
        public static decVector3 Sub(decVector3 lhs, decVector3 rhs) => decVector3.Sub(lhs, rhs);
        
        /// <summary>
        /// Returns a decVector3 from component-wise application of Sub (lhs - rhs).
        /// </summary>
        public static decimal Sub(decimal lhs, decimal rhs) => lhs - rhs;
        
        /// <summary>
        /// Returns a decVector3 from component-wise application of Mul (lhs * rhs).
        /// </summary>
        public static decVector3 Mul(decVector3 lhs, decVector3 rhs) => decVector3.Mul(lhs, rhs);
        
        /// <summary>
        /// Returns a decVector3 from component-wise application of Mul (lhs * rhs).
        /// </summary>
        public static decimal Mul(decimal lhs, decimal rhs) => lhs * rhs;
        
        /// <summary>
        /// Returns a decVector3 from component-wise application of Div (lhs / rhs).
        /// </summary>
        public static decVector3 Div(decVector3 lhs, decVector3 rhs) => decVector3.Div(lhs, rhs);
        
        /// <summary>
        /// Returns a decVector3 from component-wise application of Div (lhs / rhs).
        /// </summary>
        public static decimal Div(decimal lhs, decimal rhs) => lhs / rhs;
        
        /// <summary>
        /// Returns a decVector3 from component-wise application of Modulo (lhs % rhs).
        /// </summary>
        public static decVector3 Modulo(decVector3 lhs, decVector3 rhs) => decVector3.Modulo(lhs, rhs);
        
        /// <summary>
        /// Returns a decVector3 from component-wise application of Modulo (lhs % rhs).
        /// </summary>
        public static decimal Modulo(decimal lhs, decimal rhs) => lhs % rhs;
        
        /// <summary>
        /// Returns a decVector3 from component-wise application of Degrees (Radians-To-Degrees Conversion).
        /// </summary>
        public static decVector3 Degrees(decVector3 v) => decVector3.Degrees(v);
        
        /// <summary>
        /// Returns a decVector3 from component-wise application of Degrees (Radians-To-Degrees Conversion).
        /// </summary>
        public static decimal Degrees(decimal v) => (decimal)(v * 57.295779513082320876798154814105170332405472466564321m);
        
        /// <summary>
        /// Returns a decVector3 from component-wise application of Radians (Degrees-To-Radians Conversion).
        /// </summary>
        public static decVector3 Radians(decVector3 v) => decVector3.Radians(v);
        
        /// <summary>
        /// Returns a decVector3 from component-wise application of Radians (Degrees-To-Radians Conversion).
        /// </summary>
        public static decimal Radians(decimal v) => (decimal)(v * 0.0174532925199432957692369076848861271344287188854172m);
        
        /// <summary>
        /// Returns a decVector3 from component-wise application of Acos ((decimal)Math.Acos((double)v)).
        /// </summary>
        public static decVector3 Acos(decVector3 v) => decVector3.Acos(v);
        
        /// <summary>
        /// Returns a decVector3 from component-wise application of Acos ((decimal)Math.Acos((double)v)).
        /// </summary>
        public static decimal Acos(decimal v) => (decimal)Math.Acos((double)v);
        
        /// <summary>
        /// Returns a decVector3 from component-wise application of Asin ((decimal)Math.Asin((double)v)).
        /// </summary>
        public static decVector3 Asin(decVector3 v) => decVector3.Asin(v);
        
        /// <summary>
        /// Returns a decVector3 from component-wise application of Asin ((decimal)Math.Asin((double)v)).
        /// </summary>
        public static decimal Asin(decimal v) => (decimal)Math.Asin((double)v);
        
        /// <summary>
        /// Returns a decVector3 from component-wise application of Atan ((decimal)Math.Atan((double)v)).
        /// </summary>
        public static decVector3 Atan(decVector3 v) => decVector3.Atan(v);
        
        /// <summary>
        /// Returns a decVector3 from component-wise application of Atan ((decimal)Math.Atan((double)v)).
        /// </summary>
        public static decimal Atan(decimal v) => (decimal)Math.Atan((double)v);
        
        /// <summary>
        /// Returns a decVector3 from component-wise application of Cos ((decimal)Math.Cos((double)v)).
        /// </summary>
        public static decVector3 Cos(decVector3 v) => decVector3.Cos(v);
        
        /// <summary>
        /// Returns a decVector3 from component-wise application of Cos ((decimal)Math.Cos((double)v)).
        /// </summary>
        public static decimal Cos(decimal v) => (decimal)Math.Cos((double)v);
        
        /// <summary>
        /// Returns a decVector3 from component-wise application of Cosh ((decimal)Math.Cosh((double)v)).
        /// </summary>
        public static decVector3 Cosh(decVector3 v) => decVector3.Cosh(v);
        
        /// <summary>
        /// Returns a decVector3 from component-wise application of Cosh ((decimal)Math.Cosh((double)v)).
        /// </summary>
        public static decimal Cosh(decimal v) => (decimal)Math.Cosh((double)v);
        
        /// <summary>
        /// Returns a decVector3 from component-wise application of Exp ((decimal)Math.Exp((double)v)).
        /// </summary>
        public static decVector3 Exp(decVector3 v) => decVector3.Exp(v);
        
        /// <summary>
        /// Returns a decVector3 from component-wise application of Exp ((decimal)Math.Exp((double)v)).
        /// </summary>
        public static decimal Exp(decimal v) => (decimal)Math.Exp((double)v);
        
        /// <summary>
        /// Returns a decVector3 from component-wise application of Log ((decimal)Math.Log((double)v)).
        /// </summary>
        public static decVector3 Log(decVector3 v) => decVector3.Log(v);
        
        /// <summary>
        /// Returns a decVector3 from component-wise application of Log ((decimal)Math.Log((double)v)).
        /// </summary>
        public static decimal Log(decimal v) => (decimal)Math.Log((double)v);
        
        /// <summary>
        /// Returns a decVector3 from component-wise application of Log2 ((decimal)Math.Log((double)v, 2)).
        /// </summary>
        public static decVector3 Log2(decVector3 v) => decVector3.Log2(v);
        
        /// <summary>
        /// Returns a decVector3 from component-wise application of Log2 ((decimal)Math.Log((double)v, 2)).
        /// </summary>
        public static decimal Log2(decimal v) => (decimal)Math.Log((double)v, 2);
        
        /// <summary>
        /// Returns a decVector3 from component-wise application of Log10 ((decimal)Math.Log10((double)v)).
        /// </summary>
        public static decVector3 Log10(decVector3 v) => decVector3.Log10(v);
        
        /// <summary>
        /// Returns a decVector3 from component-wise application of Log10 ((decimal)Math.Log10((double)v)).
        /// </summary>
        public static decimal Log10(decimal v) => (decimal)Math.Log10((double)v);
        
        /// <summary>
        /// Returns a decVector3 from component-wise application of Floor ((decimal)Math.Floor(v)).
        /// </summary>
        public static decVector3 Floor(decVector3 v) => decVector3.Floor(v);
        
        /// <summary>
        /// Returns a decVector3 from component-wise application of Floor ((decimal)Math.Floor(v)).
        /// </summary>
        public static decimal Floor(decimal v) => (decimal)Math.Floor(v);
        
        /// <summary>
        /// Returns a decVector3 from component-wise application of Ceiling ((decimal)Math.Ceiling(v)).
        /// </summary>
        public static decVector3 Ceiling(decVector3 v) => decVector3.Ceiling(v);
        
        /// <summary>
        /// Returns a decVector3 from component-wise application of Ceiling ((decimal)Math.Ceiling(v)).
        /// </summary>
        public static decimal Ceiling(decimal v) => (decimal)Math.Ceiling(v);
        
        /// <summary>
        /// Returns a decVector3 from component-wise application of Round ((decimal)Math.Round(v)).
        /// </summary>
        public static decVector3 Round(decVector3 v) => decVector3.Round(v);
        
        /// <summary>
        /// Returns a decVector3 from component-wise application of Round ((decimal)Math.Round(v)).
        /// </summary>
        public static decimal Round(decimal v) => (decimal)Math.Round(v);
        
        /// <summary>
        /// Returns a decVector3 from component-wise application of Sin ((decimal)Math.Sin((double)v)).
        /// </summary>
        public static decVector3 Sin(decVector3 v) => decVector3.Sin(v);
        
        /// <summary>
        /// Returns a decVector3 from component-wise application of Sin ((decimal)Math.Sin((double)v)).
        /// </summary>
        public static decimal Sin(decimal v) => (decimal)Math.Sin((double)v);
        
        /// <summary>
        /// Returns a decVector3 from component-wise application of Sinh ((decimal)Math.Sinh((double)v)).
        /// </summary>
        public static decVector3 Sinh(decVector3 v) => decVector3.Sinh(v);
        
        /// <summary>
        /// Returns a decVector3 from component-wise application of Sinh ((decimal)Math.Sinh((double)v)).
        /// </summary>
        public static decimal Sinh(decimal v) => (decimal)Math.Sinh((double)v);
        
        /// <summary>
        /// Returns a decVector3 from component-wise application of Tan ((decimal)Math.Tan((double)v)).
        /// </summary>
        public static decVector3 Tan(decVector3 v) => decVector3.Tan(v);
        
        /// <summary>
        /// Returns a decVector3 from component-wise application of Tan ((decimal)Math.Tan((double)v)).
        /// </summary>
        public static decimal Tan(decimal v) => (decimal)Math.Tan((double)v);
        
        /// <summary>
        /// Returns a decVector3 from component-wise application of Tanh ((decimal)Math.Tanh((double)v)).
        /// </summary>
        public static decVector3 Tanh(decVector3 v) => decVector3.Tanh(v);
        
        /// <summary>
        /// Returns a decVector3 from component-wise application of Tanh ((decimal)Math.Tanh((double)v)).
        /// </summary>
        public static decimal Tanh(decimal v) => (decimal)Math.Tanh((double)v);
        
        /// <summary>
        /// Returns a decVector3 from component-wise application of Truncate ((decimal)Math.Truncate((double)v)).
        /// </summary>
        public static decVector3 Truncate(decVector3 v) => decVector3.Truncate(v);
        
        /// <summary>
        /// Returns a decVector3 from component-wise application of Truncate ((decimal)Math.Truncate((double)v)).
        /// </summary>
        public static decimal Truncate(decimal v) => (decimal)Math.Truncate((double)v);
        
        /// <summary>
        /// Returns a decVector3 from component-wise application of Fract ((decimal)(v - Math.Floor(v))).
        /// </summary>
        public static decVector3 Fract(decVector3 v) => decVector3.Fract(v);
        
        /// <summary>
        /// Returns a decVector3 from component-wise application of Fract ((decimal)(v - Math.Floor(v))).
        /// </summary>
        public static decimal Fract(decimal v) => (decimal)(v - Math.Floor(v));
        
        /// <summary>
        /// Returns a decVector3 from component-wise application of Trunc ((long)(v)).
        /// </summary>
        public static decVector3 Trunc(decVector3 v) => decVector3.Trunc(v);
        
        /// <summary>
        /// Returns a decVector3 from component-wise application of Trunc ((long)(v)).
        /// </summary>
        public static decimal Trunc(decimal v) => (long)(v);
        
        /// <summary>
        /// Returns the minimal component of this Vector.
        /// </summary>
        public static decimal MinElement(decVector3 v) => v.MinElement;
        
        /// <summary>
        /// Returns the maximal component of this Vector.
        /// </summary>
        public static decimal MaxElement(decVector3 v) => v.MaxElement;
        
        /// <summary>
        /// Returns the euclidean length of this Vector.
        /// </summary>
        public static decimal Length(decVector3 v) => v.Length;
        
        /// <summary>
        /// Returns the squared euclidean length of this Vector.
        /// </summary>
        public static decimal LengthSqr(decVector3 v) => v.LengthSqr;
        
        /// <summary>
        /// Returns the sum of all components.
        /// </summary>
        public static decimal Sum(decVector3 v) => v.Sum;
        
        /// <summary>
        /// Returns the euclidean norm of this Vector.
        /// </summary>
        public static decimal Norm(decVector3 v) => v.Norm;
        
        /// <summary>
        /// Returns the one-norm of this Vector.
        /// </summary>
        public static decimal Norm1(decVector3 v) => v.Norm1;
        
        /// <summary>
        /// Returns the two-norm (euclidean length) of this Vector.
        /// </summary>
        public static decimal Norm2(decVector3 v) => v.Norm2;
        
        /// <summary>
        /// Returns the max-norm of this Vector.
        /// </summary>
        public static decimal NormMax(decVector3 v) => v.NormMax;
        
        /// <summary>
        /// Returns the p-norm of this Vector.
        /// </summary>
        public static double NormP(decVector3 v, double p) => v.NormP(p);
        
        /// <summary>
        /// Returns a copy of this Vector with length one (undefined if this has zero length).
        /// </summary>
        public static decVector3 Normalized(decVector3 v) => v.Normalized;
        
        /// <summary>
        /// Returns a copy of this Vector with length one (returns zero if length is zero).
        /// </summary>
        public static decVector3 NormalizedSafe(decVector3 v) => v.NormalizedSafe;
        
        /// <summary>
        /// Returns the inner product (dot product, scalar product) of the two Vectors.
        /// </summary>
        public static decimal Dot(decVector3 lhs, decVector3 rhs) => decVector3.Dot(lhs, rhs);
        
        /// <summary>
        /// Returns the euclidean distance between the two Vectors.
        /// </summary>
        public static decimal Distance(decVector3 lhs, decVector3 rhs) => decVector3.Distance(lhs, rhs);
        
        /// <summary>
        /// Returns the squared euclidean distance between the two Vectors.
        /// </summary>
        public static decimal DistanceSqr(decVector3 lhs, decVector3 rhs) => decVector3.DistanceSqr(lhs, rhs);
        
        /// <summary>
        /// Calculate the reflection direction for an incident Vector (N should be normalized in order to achieve the desired result).
        /// </summary>
        public static decVector3 Reflect(decVector3 I, decVector3 N) => decVector3.Reflect(I, N);
        
        /// <summary>
        /// Calculate the refraction direction for an incident Vector (The input parameters I and N should be normalized in order to achieve the desired result).
        /// </summary>
        public static decVector3 Refract(decVector3 I, decVector3 N, decimal eta) => decVector3.Refract(I, N, eta);
        
        /// <summary>
        /// Returns a Vector pointing in the same direction as another (faceforward orients a Vector to point away from a surface as defined by its normal. If dot(Nref, I) is negative faceforward returns N, otherwise it returns -N).
        /// </summary>
        public static decVector3 FaceForward(decVector3 N, decVector3 I, decVector3 Nref) => decVector3.FaceForward(N, I, Nref);
        
        /// <summary>
        /// Returns the outer product (cross product, Vector product) of the two Vectors.
        /// </summary>
        public static decVector3 Cross(decVector3 l, decVector3 r) => decVector3.Cross(l, r);
        
        /// <summary>
        /// Returns a decVector3 with independent and identically distributed uniform values between 'minValue' and 'maxValue'.
        /// </summary>
        public static decVector3 Random(System.Random random, decVector3 minValue, decVector3 maxValue) => decVector3.Random(random, minValue, maxValue);
        
        /// <summary>
        /// Returns a decVector3 with independent and identically distributed uniform values between 'minValue' and 'maxValue'.
        /// </summary>
        public static decimal Random(System.Random random, decimal minValue, decimal maxValue) => (decimal)random.NextDouble() * (maxValue - minValue) + minValue;
        
        /// <summary>
        /// Returns a decVector3 with independent and identically distributed uniform values between 'minValue' and 'maxValue'.
        /// </summary>
        public static decVector3 RandomUniform(System.Random random, decVector3 minValue, decVector3 maxValue) => decVector3.RandomUniform(random, minValue, maxValue);
        
        /// <summary>
        /// Returns a decVector3 with independent and identically distributed uniform values between 'minValue' and 'maxValue'.
        /// </summary>
        public static decimal RandomUniform(System.Random random, decimal minValue, decimal maxValue) => (decimal)random.NextDouble() * (maxValue - minValue) + minValue;
        
        /// <summary>
        /// Returns a decVector3 with independent and identically distributed values according to a normal/Gaussian distribution with specified mean and variance.
        /// </summary>
        public static decVector3 RandomNormal(System.Random random, decVector3 mean, decVector3 variance) => decVector3.RandomNormal(random, mean, variance);
        
        /// <summary>
        /// Returns a decVector3 with independent and identically distributed values according to a normal/Gaussian distribution with specified mean and variance.
        /// </summary>
        public static decimal RandomNormal(System.Random random, decimal mean, decimal variance) => (decimal)(Math.Sqrt((double)variance) * Math.Cos(2 * Math.PI * random.NextDouble()) * Math.Sqrt(-2.0 * Math.Log(random.NextDouble()))) + mean;
        
        /// <summary>
        /// Returns a decVector3 with independent and identically distributed values according to a normal/Gaussian distribution with specified mean and variance.
        /// </summary>
        public static decVector3 RandomGaussian(System.Random random, decVector3 mean, decVector3 variance) => decVector3.RandomGaussian(random, mean, variance);
        
        /// <summary>
        /// Returns a decVector3 with independent and identically distributed values according to a normal/Gaussian distribution with specified mean and variance.
        /// </summary>
        public static decimal RandomGaussian(System.Random random, decimal mean, decimal variance) => (decimal)(Math.Sqrt((double)variance) * Math.Cos(2 * Math.PI * random.NextDouble()) * Math.Sqrt(-2.0 * Math.Log(random.NextDouble()))) + mean;

    }
}
