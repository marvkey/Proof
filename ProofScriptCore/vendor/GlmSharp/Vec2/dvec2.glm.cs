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
        public static swizzle_dVector2 swizzle(dVector2 v) => v.swizzle;
        
        /// <summary>
        /// Returns an array with all values
        /// </summary>
        public static double[] Values(dVector2 v) => v.Values;
        
        /// <summary>
        /// Returns an enumerator that iterates through all components.
        /// </summary>
        public static IEnumerator<double> GetEnumerator(dVector2 v) => v.GetEnumerator();
        
        /// <summary>
        /// Returns a string representation of this Vector using ', ' as a seperator.
        /// </summary>
        public static string ToString(dVector2 v) => v.ToString();
        
        /// <summary>
        /// Returns a string representation of this Vector using a provided seperator.
        /// </summary>
        public static string ToString(dVector2 v, string sep) => v.ToString(sep);
        
        /// <summary>
        /// Returns a string representation of this Vector using a provided seperator and a forMatrix provider for each component.
        /// </summary>
        public static string ToString(dVector2 v, string sep, IFormatProvider provider) => v.ToString(sep, provider);
        
        /// <summary>
        /// Returns a string representation of this Vector using a provided seperator and a forMatrix for each component.
        /// </summary>
        public static string ToString(dVector2 v, string sep, string forMatrix) => v.ToString(sep, forMatrix);
        
        /// <summary>
        /// Returns a string representation of this Vector using a provided seperator and a forMatrix and forMatrix provider for each component.
        /// </summary>
        public static string ToString(dVector2 v, string sep, string forMatrix, IFormatProvider provider) => v.ToString(sep, forMatrix, provider);
        
        /// <summary>
        /// Returns the number of components (2).
        /// </summary>
        public static int Count(dVector2 v) => v.Count;
        
        /// <summary>
        /// Returns true iff this equals rhs component-wise.
        /// </summary>
        public static bool Equals(dVector2 v, dVector2 rhs) => v.Equals(rhs);
        
        /// <summary>
        /// Returns true iff this equals rhs type- and component-wise.
        /// </summary>
        public static bool Equals(dVector2 v, object obj) => v.Equals(obj);
        
        /// <summary>
        /// Returns a hash code for this instance.
        /// </summary>
        public static int GetHashCode(dVector2 v) => v.GetHashCode();
        
        /// <summary>
        /// Returns true iff distance between lhs and rhs is less than or equal to epsilon
        /// </summary>
        public static bool ApproxEqual(dVector2 lhs, dVector2 rhs, double eps = 0.1d) => dVector2.ApproxEqual(lhs, rhs, eps);
        
        /// <summary>
        /// Returns a bVector2 from component-wise application of Equal (lhs == rhs).
        /// </summary>
        public static bVector2 Equal(dVector2 lhs, dVector2 rhs) => dVector2.Equal(lhs, rhs);
        
        /// <summary>
        /// Returns a bVector2 from component-wise application of NotEqual (lhs != rhs).
        /// </summary>
        public static bVector2 NotEqual(dVector2 lhs, dVector2 rhs) => dVector2.NotEqual(lhs, rhs);
        
        /// <summary>
        /// Returns a bVector2 from component-wise application of GreaterThan (lhs &gt; rhs).
        /// </summary>
        public static bVector2 GreaterThan(dVector2 lhs, dVector2 rhs) => dVector2.GreaterThan(lhs, rhs);
        
        /// <summary>
        /// Returns a bVector2 from component-wise application of GreaterThanEqual (lhs &gt;= rhs).
        /// </summary>
        public static bVector2 GreaterThanEqual(dVector2 lhs, dVector2 rhs) => dVector2.GreaterThanEqual(lhs, rhs);
        
        /// <summary>
        /// Returns a bVector2 from component-wise application of LesserThan (lhs &lt; rhs).
        /// </summary>
        public static bVector2 LesserThan(dVector2 lhs, dVector2 rhs) => dVector2.LesserThan(lhs, rhs);
        
        /// <summary>
        /// Returns a bVector2 from component-wise application of LesserThanEqual (lhs &lt;= rhs).
        /// </summary>
        public static bVector2 LesserThanEqual(dVector2 lhs, dVector2 rhs) => dVector2.LesserThanEqual(lhs, rhs);
        
        /// <summary>
        /// Returns a bVector2 from component-wise application of IsInfinity (double.IsInfinity(v)).
        /// </summary>
        public static bVector2 IsInfinity(dVector2 v) => dVector2.IsInfinity(v);
        
        /// <summary>
        /// Returns a bVector2 from component-wise application of IsFinite (!double.IsNaN(v) &amp;&amp; !double.IsInfinity(v)).
        /// </summary>
        public static bVector2 IsFinite(dVector2 v) => dVector2.IsFinite(v);
        
        /// <summary>
        /// Returns a bVector2 from component-wise application of IsNaN (double.IsNaN(v)).
        /// </summary>
        public static bVector2 IsNaN(dVector2 v) => dVector2.IsNaN(v);
        
        /// <summary>
        /// Returns a bVector2 from component-wise application of IsNegativeInfinity (double.IsNegativeInfinity(v)).
        /// </summary>
        public static bVector2 IsNegativeInfinity(dVector2 v) => dVector2.IsNegativeInfinity(v);
        
        /// <summary>
        /// Returns a bVector2 from component-wise application of IsPositiveInfinity (double.IsPositiveInfinity(v)).
        /// </summary>
        public static bVector2 IsPositiveInfinity(dVector2 v) => dVector2.IsPositiveInfinity(v);
        
        /// <summary>
        /// Returns a dVector2 from component-wise application of Abs (Math.Abs(v)).
        /// </summary>
        public static dVector2 Abs(dVector2 v) => dVector2.Abs(v);
        
        /// <summary>
        /// Returns a dVector2 from component-wise application of HermiteInterpolationOrder3 ((3 - 2 * v) * v * v).
        /// </summary>
        public static dVector2 HermiteInterpolationOrder3(dVector2 v) => dVector2.HermiteInterpolationOrder3(v);
        
        /// <summary>
        /// Returns a dVector2 from component-wise application of HermiteInterpolationOrder5 (((6 * v - 15) * v + 10) * v * v * v).
        /// </summary>
        public static dVector2 HermiteInterpolationOrder5(dVector2 v) => dVector2.HermiteInterpolationOrder5(v);
        
        /// <summary>
        /// Returns a dVector2 from component-wise application of Sqr (v * v).
        /// </summary>
        public static dVector2 Sqr(dVector2 v) => dVector2.Sqr(v);
        
        /// <summary>
        /// Returns a dVector2 from component-wise application of Pow2 (v * v).
        /// </summary>
        public static dVector2 Pow2(dVector2 v) => dVector2.Pow2(v);
        
        /// <summary>
        /// Returns a dVector2 from component-wise application of Pow3 (v * v * v).
        /// </summary>
        public static dVector2 Pow3(dVector2 v) => dVector2.Pow3(v);
        
        /// <summary>
        /// Returns a dVector2 from component-wise application of Step (v &gt;= 0.0 ? 1.0 : 0.0).
        /// </summary>
        public static dVector2 Step(dVector2 v) => dVector2.Step(v);
        
        /// <summary>
        /// Returns a dVector2 from component-wise application of Sqrt ((double)Math.Sqrt((double)v)).
        /// </summary>
        public static dVector2 Sqrt(dVector2 v) => dVector2.Sqrt(v);
        
        /// <summary>
        /// Returns a dVector2 from component-wise application of InverseSqrt ((double)(1.0 / Math.Sqrt((double)v))).
        /// </summary>
        public static dVector2 InverseSqrt(dVector2 v) => dVector2.InverseSqrt(v);
        
        /// <summary>
        /// Returns a iVector2 from component-wise application of Sign (Math.Sign(v)).
        /// </summary>
        public static iVector2 Sign(dVector2 v) => dVector2.Sign(v);
        
        /// <summary>
        /// Returns a dVector2 from component-wise application of Max (Math.Max(lhs, rhs)).
        /// </summary>
        public static dVector2 Max(dVector2 lhs, dVector2 rhs) => dVector2.Max(lhs, rhs);
        
        /// <summary>
        /// Returns a dVector2 from component-wise application of Min (Math.Min(lhs, rhs)).
        /// </summary>
        public static dVector2 Min(dVector2 lhs, dVector2 rhs) => dVector2.Min(lhs, rhs);
        
        /// <summary>
        /// Returns a dVector2 from component-wise application of Pow ((double)Math.Pow((double)lhs, (double)rhs)).
        /// </summary>
        public static dVector2 Pow(dVector2 lhs, dVector2 rhs) => dVector2.Pow(lhs, rhs);
        
        /// <summary>
        /// Returns a dVector2 from component-wise application of Log ((double)Math.Log((double)lhs, (double)rhs)).
        /// </summary>
        public static dVector2 Log(dVector2 lhs, dVector2 rhs) => dVector2.Log(lhs, rhs);
        
        /// <summary>
        /// Returns a dVector2 from component-wise application of Clamp (Math.Min(Math.Max(v, min), max)).
        /// </summary>
        public static dVector2 Clamp(dVector2 v, dVector2 min, dVector2 max) => dVector2.Clamp(v, min, max);
        
        /// <summary>
        /// Returns a dVector2 from component-wise application of Mix (min * (1-a) + max * a).
        /// </summary>
        public static dVector2 Mix(dVector2 min, dVector2 max, dVector2 a) => dVector2.Mix(min, max, a);
        
        /// <summary>
        /// Returns a dVector2 from component-wise application of Lerp (min * (1-a) + max * a).
        /// </summary>
        public static dVector2 Lerp(dVector2 min, dVector2 max, dVector2 a) => dVector2.Lerp(min, max, a);
        
        /// <summary>
        /// Returns a dVector2 from component-wise application of Smoothstep (((v - edge0) / (edge1 - edge0)).Clamp().HermiteInterpolationOrder3()).
        /// </summary>
        public static dVector2 Smoothstep(dVector2 edge0, dVector2 edge1, dVector2 v) => dVector2.Smoothstep(edge0, edge1, v);
        
        /// <summary>
        /// Returns a dVector2 from component-wise application of Smootherstep (((v - edge0) / (edge1 - edge0)).Clamp().HermiteInterpolationOrder5()).
        /// </summary>
        public static dVector2 Smootherstep(dVector2 edge0, dVector2 edge1, dVector2 v) => dVector2.Smootherstep(edge0, edge1, v);
        
        /// <summary>
        /// Returns a dVector2 from component-wise application of Fma (a * b + c).
        /// </summary>
        public static dVector2 Fma(dVector2 a, dVector2 b, dVector2 c) => dVector2.Fma(a, b, c);
        
        /// <summary>
        /// OuterProduct treats the first parameter c as a column Vector (Matrix with one column) and the second parameter r as a row Vector (Matrix with one row) and does a linear algebraic Matrix multiply c * r, yielding a Matrix whose number of rows is the number of components in c and whose number of columns is the number of components in r.
        /// </summary>
        public static dMatrix2 OuterProduct(dVector2 c, dVector2 r) => dVector2.OuterProduct(c, r);
        
        /// <summary>
        /// OuterProduct treats the first parameter c as a column Vector (Matrix with one column) and the second parameter r as a row Vector (Matrix with one row) and does a linear algebraic Matrix multiply c * r, yielding a Matrix whose number of rows is the number of components in c and whose number of columns is the number of components in r.
        /// </summary>
        public static dMatrix3x2 OuterProduct(dVector2 c, dVector3 r) => dVector2.OuterProduct(c, r);
        
        /// <summary>
        /// OuterProduct treats the first parameter c as a column Vector (Matrix with one column) and the second parameter r as a row Vector (Matrix with one row) and does a linear algebraic Matrix multiply c * r, yielding a Matrix whose number of rows is the number of components in c and whose number of columns is the number of components in r.
        /// </summary>
        public static dMatrix4x2 OuterProduct(dVector2 c, dVector4 r) => dVector2.OuterProduct(c, r);
        
        /// <summary>
        /// Returns a dVector2 from component-wise application of Add (lhs + rhs).
        /// </summary>
        public static dVector2 Add(dVector2 lhs, dVector2 rhs) => dVector2.Add(lhs, rhs);
        
        /// <summary>
        /// Returns a dVector2 from component-wise application of Sub (lhs - rhs).
        /// </summary>
        public static dVector2 Sub(dVector2 lhs, dVector2 rhs) => dVector2.Sub(lhs, rhs);
        
        /// <summary>
        /// Returns a dVector2 from component-wise application of Mul (lhs * rhs).
        /// </summary>
        public static dVector2 Mul(dVector2 lhs, dVector2 rhs) => dVector2.Mul(lhs, rhs);
        
        /// <summary>
        /// Returns a dVector2 from component-wise application of Div (lhs / rhs).
        /// </summary>
        public static dVector2 Div(dVector2 lhs, dVector2 rhs) => dVector2.Div(lhs, rhs);
        
        /// <summary>
        /// Returns a dVector2 from component-wise application of Modulo (lhs % rhs).
        /// </summary>
        public static dVector2 Modulo(dVector2 lhs, dVector2 rhs) => dVector2.Modulo(lhs, rhs);
        
        /// <summary>
        /// Returns a dVector2 from component-wise application of Degrees (Radians-To-Degrees Conversion).
        /// </summary>
        public static dVector2 Degrees(dVector2 v) => dVector2.Degrees(v);
        
        /// <summary>
        /// Returns a dVector2 from component-wise application of Radians (Degrees-To-Radians Conversion).
        /// </summary>
        public static dVector2 Radians(dVector2 v) => dVector2.Radians(v);
        
        /// <summary>
        /// Returns a dVector2 from component-wise application of Acos ((double)Math.Acos((double)v)).
        /// </summary>
        public static dVector2 Acos(dVector2 v) => dVector2.Acos(v);
        
        /// <summary>
        /// Returns a dVector2 from component-wise application of Asin ((double)Math.Asin((double)v)).
        /// </summary>
        public static dVector2 Asin(dVector2 v) => dVector2.Asin(v);
        
        /// <summary>
        /// Returns a dVector2 from component-wise application of Atan ((double)Math.Atan((double)v)).
        /// </summary>
        public static dVector2 Atan(dVector2 v) => dVector2.Atan(v);
        
        /// <summary>
        /// Returns a dVector2 from component-wise application of Cos ((double)Math.Cos((double)v)).
        /// </summary>
        public static dVector2 Cos(dVector2 v) => dVector2.Cos(v);
        
        /// <summary>
        /// Returns a dVector2 from component-wise application of Cosh ((double)Math.Cosh((double)v)).
        /// </summary>
        public static dVector2 Cosh(dVector2 v) => dVector2.Cosh(v);
        
        /// <summary>
        /// Returns a dVector2 from component-wise application of Exp ((double)Math.Exp((double)v)).
        /// </summary>
        public static dVector2 Exp(dVector2 v) => dVector2.Exp(v);
        
        /// <summary>
        /// Returns a dVector2 from component-wise application of Log ((double)Math.Log((double)v)).
        /// </summary>
        public static dVector2 Log(dVector2 v) => dVector2.Log(v);
        
        /// <summary>
        /// Returns a dVector2 from component-wise application of Log2 ((double)Math.Log((double)v, 2)).
        /// </summary>
        public static dVector2 Log2(dVector2 v) => dVector2.Log2(v);
        
        /// <summary>
        /// Returns a dVector2 from component-wise application of Log10 ((double)Math.Log10((double)v)).
        /// </summary>
        public static dVector2 Log10(dVector2 v) => dVector2.Log10(v);
        
        /// <summary>
        /// Returns a dVector2 from component-wise application of Floor ((double)Math.Floor(v)).
        /// </summary>
        public static dVector2 Floor(dVector2 v) => dVector2.Floor(v);
        
        /// <summary>
        /// Returns a dVector2 from component-wise application of Ceiling ((double)Math.Ceiling(v)).
        /// </summary>
        public static dVector2 Ceiling(dVector2 v) => dVector2.Ceiling(v);
        
        /// <summary>
        /// Returns a dVector2 from component-wise application of Round ((double)Math.Round(v)).
        /// </summary>
        public static dVector2 Round(dVector2 v) => dVector2.Round(v);
        
        /// <summary>
        /// Returns a dVector2 from component-wise application of Sin ((double)Math.Sin((double)v)).
        /// </summary>
        public static dVector2 Sin(dVector2 v) => dVector2.Sin(v);
        
        /// <summary>
        /// Returns a dVector2 from component-wise application of Sinh ((double)Math.Sinh((double)v)).
        /// </summary>
        public static dVector2 Sinh(dVector2 v) => dVector2.Sinh(v);
        
        /// <summary>
        /// Returns a dVector2 from component-wise application of Tan ((double)Math.Tan((double)v)).
        /// </summary>
        public static dVector2 Tan(dVector2 v) => dVector2.Tan(v);
        
        /// <summary>
        /// Returns a dVector2 from component-wise application of Tanh ((double)Math.Tanh((double)v)).
        /// </summary>
        public static dVector2 Tanh(dVector2 v) => dVector2.Tanh(v);
        
        /// <summary>
        /// Returns a dVector2 from component-wise application of Truncate ((double)Math.Truncate((double)v)).
        /// </summary>
        public static dVector2 Truncate(dVector2 v) => dVector2.Truncate(v);
        
        /// <summary>
        /// Returns a dVector2 from component-wise application of Fract ((double)(v - Math.Floor(v))).
        /// </summary>
        public static dVector2 Fract(dVector2 v) => dVector2.Fract(v);
        
        /// <summary>
        /// Returns a dVector2 from component-wise application of Trunc ((long)(v)).
        /// </summary>
        public static dVector2 Trunc(dVector2 v) => dVector2.Trunc(v);
        
        /// <summary>
        /// Returns the minimal component of this Vector.
        /// </summary>
        public static double MinElement(dVector2 v) => v.MinElement;
        
        /// <summary>
        /// Returns the maximal component of this Vector.
        /// </summary>
        public static double MaxElement(dVector2 v) => v.MaxElement;
        
        /// <summary>
        /// Returns the euclidean length of this Vector.
        /// </summary>
        public static double Length(dVector2 v) => v.Length;
        
        /// <summary>
        /// Returns the squared euclidean length of this Vector.
        /// </summary>
        public static double LengthSqr(dVector2 v) => v.LengthSqr;
        
        /// <summary>
        /// Returns the sum of all components.
        /// </summary>
        public static double Sum(dVector2 v) => v.Sum;
        
        /// <summary>
        /// Returns the euclidean norm of this Vector.
        /// </summary>
        public static double Norm(dVector2 v) => v.Norm;
        
        /// <summary>
        /// Returns the one-norm of this Vector.
        /// </summary>
        public static double Norm1(dVector2 v) => v.Norm1;
        
        /// <summary>
        /// Returns the two-norm (euclidean length) of this Vector.
        /// </summary>
        public static double Norm2(dVector2 v) => v.Norm2;
        
        /// <summary>
        /// Returns the max-norm of this Vector.
        /// </summary>
        public static double NormMax(dVector2 v) => v.NormMax;
        
        /// <summary>
        /// Returns the p-norm of this Vector.
        /// </summary>
        public static double NormP(dVector2 v, double p) => v.NormP(p);
        
        /// <summary>
        /// Returns a copy of this Vector with length one (undefined if this has zero length).
        /// </summary>
        public static dVector2 Normalized(dVector2 v) => v.Normalized;
        
        /// <summary>
        /// Returns a copy of this Vector with length one (returns zero if length is zero).
        /// </summary>
        public static dVector2 NormalizedSafe(dVector2 v) => v.NormalizedSafe;
        
        /// <summary>
        /// Returns the Vector angle (atan2(y, x)) in radians.
        /// </summary>
        public static double Angle(dVector2 v) => v.Angle;
        
        /// <summary>
        /// Returns a 2D Vector that was rotated by a given angle in radians (CAUTION: result is casted and may be truncated).
        /// </summary>
        public static dVector2 Rotated(dVector2 v, double angleInRad) => v.Rotated(angleInRad);
        
        /// <summary>
        /// Returns the inner product (dot product, scalar product) of the two Vectors.
        /// </summary>
        public static double Dot(dVector2 lhs, dVector2 rhs) => dVector2.Dot(lhs, rhs);
        
        /// <summary>
        /// Returns the euclidean distance between the two Vectors.
        /// </summary>
        public static double Distance(dVector2 lhs, dVector2 rhs) => dVector2.Distance(lhs, rhs);
        
        /// <summary>
        /// Returns the squared euclidean distance between the two Vectors.
        /// </summary>
        public static double DistanceSqr(dVector2 lhs, dVector2 rhs) => dVector2.DistanceSqr(lhs, rhs);
        
        /// <summary>
        /// Calculate the reflection direction for an incident Vector (N should be normalized in order to achieve the desired result).
        /// </summary>
        public static dVector2 Reflect(dVector2 I, dVector2 N) => dVector2.Reflect(I, N);
        
        /// <summary>
        /// Calculate the refraction direction for an incident Vector (The input parameters I and N should be normalized in order to achieve the desired result).
        /// </summary>
        public static dVector2 Refract(dVector2 I, dVector2 N, double eta) => dVector2.Refract(I, N, eta);
        
        /// <summary>
        /// Returns a Vector pointing in the same direction as another (faceforward orients a Vector to point away from a surface as defined by its normal. If dot(Nref, I) is negative faceforward returns N, otherwise it returns -N).
        /// </summary>
        public static dVector2 FaceForward(dVector2 N, dVector2 I, dVector2 Nref) => dVector2.FaceForward(N, I, Nref);
        
        /// <summary>
        /// Returns the length of the outer product (cross product, Vector product) of the two Vectors.
        /// </summary>
        public static double Cross(dVector2 l, dVector2 r) => dVector2.Cross(l, r);
        
        /// <summary>
        /// Returns a dVector2 with independent and identically distributed uniform values between 'minValue' and 'maxValue'.
        /// </summary>
        public static dVector2 Random(System.Random random, dVector2 minValue, dVector2 maxValue) => dVector2.Random(random, minValue, maxValue);
        
        /// <summary>
        /// Returns a dVector2 with independent and identically distributed uniform values between 'minValue' and 'maxValue'.
        /// </summary>
        public static dVector2 RandomUniform(System.Random random, dVector2 minValue, dVector2 maxValue) => dVector2.RandomUniform(random, minValue, maxValue);
        
        /// <summary>
        /// Returns a dVector2 with independent and identically distributed values according to a normal/Gaussian distribution with specified mean and variance.
        /// </summary>
        public static dVector2 RandomNormal(System.Random random, dVector2 mean, dVector2 variance) => dVector2.RandomNormal(random, mean, variance);
        
        /// <summary>
        /// Returns a dVector2 with independent and identically distributed values according to a normal/Gaussian distribution with specified mean and variance.
        /// </summary>
        public static dVector2 RandomGaussian(System.Random random, dVector2 mean, dVector2 variance) => dVector2.RandomGaussian(random, mean, variance);

    }
}
