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
        public static swizzle_dVector4 swizzle(dVector4 v) => v.swizzle;
        
        /// <summary>
        /// Returns an array with all values
        /// </summary>
        public static double[] Values(dVector4 v) => v.Values;
        
        /// <summary>
        /// Returns an enumerator that iterates through all components.
        /// </summary>
        public static IEnumerator<double> GetEnumerator(dVector4 v) => v.GetEnumerator();
        
        /// <summary>
        /// Returns a string representation of this Vectortor using ', ' as a seperator.
        /// </summary>
        public static string ToString(dVector4 v) => v.ToString();
        
        /// <summary>
        /// Returns a string representation of this Vectortor using a provided seperator.
        /// </summary>
        public static string ToString(dVector4 v, string sep) => v.ToString(sep);
        
        /// <summary>
        /// Returns a string representation of this Vectortor using a provided seperator and a forMatrix provider for each component.
        /// </summary>
        public static string ToString(dVector4 v, string sep, IFormatProvider provider) => v.ToString(sep, provider);
        
        /// <summary>
        /// Returns a string representation of this Vectortor using a provided seperator and a forMatrix for each component.
        /// </summary>
        public static string ToString(dVector4 v, string sep, string forMatrix) => v.ToString(sep, forMatrix);
        
        /// <summary>
        /// Returns a string representation of this Vectortor using a provided seperator and a forMatrix and forMatrix provider for each component.
        /// </summary>
        public static string ToString(dVector4 v, string sep, string forMatrix, IFormatProvider provider) => v.ToString(sep, forMatrix, provider);
        
        /// <summary>
        /// Returns the number of components (4).
        /// </summary>
        public static int Count(dVector4 v) => v.Count;
        
        /// <summary>
        /// Returns true iff this equals rhs component-wise.
        /// </summary>
        public static bool Equals(dVector4 v, dVector4 rhs) => v.Equals(rhs);
        
        /// <summary>
        /// Returns true iff this equals rhs type- and component-wise.
        /// </summary>
        public static bool Equals(dVector4 v, object obj) => v.Equals(obj);
        
        /// <summary>
        /// Returns a hash code for this instance.
        /// </summary>
        public static int GetHashCode(dVector4 v) => v.GetHashCode();
        
        /// <summary>
        /// Returns true iff distance between lhs and rhs is less than or equal to epsilon
        /// </summary>
        public static bool ApproxEqual(dVector4 lhs, dVector4 rhs, double eps = 0.1d) => dVector4.ApproxEqual(lhs, rhs, eps);
        
        /// <summary>
        /// Returns a bVector4 from component-wise application of Equal (lhs == rhs).
        /// </summary>
        public static bVector4 Equal(dVector4 lhs, dVector4 rhs) => dVector4.Equal(lhs, rhs);
        
        /// <summary>
        /// Returns a bVector4 from component-wise application of NotEqual (lhs != rhs).
        /// </summary>
        public static bVector4 NotEqual(dVector4 lhs, dVector4 rhs) => dVector4.NotEqual(lhs, rhs);
        
        /// <summary>
        /// Returns a bVector4 from component-wise application of GreaterThan (lhs &gt; rhs).
        /// </summary>
        public static bVector4 GreaterThan(dVector4 lhs, dVector4 rhs) => dVector4.GreaterThan(lhs, rhs);
        
        /// <summary>
        /// Returns a bVector4 from component-wise application of GreaterThanEqual (lhs &gt;= rhs).
        /// </summary>
        public static bVector4 GreaterThanEqual(dVector4 lhs, dVector4 rhs) => dVector4.GreaterThanEqual(lhs, rhs);
        
        /// <summary>
        /// Returns a bVector4 from component-wise application of LesserThan (lhs &lt; rhs).
        /// </summary>
        public static bVector4 LesserThan(dVector4 lhs, dVector4 rhs) => dVector4.LesserThan(lhs, rhs);
        
        /// <summary>
        /// Returns a bVector4 from component-wise application of LesserThanEqual (lhs &lt;= rhs).
        /// </summary>
        public static bVector4 LesserThanEqual(dVector4 lhs, dVector4 rhs) => dVector4.LesserThanEqual(lhs, rhs);
        
        /// <summary>
        /// Returns a bVector4 from component-wise application of IsInfinity (double.IsInfinity(v)).
        /// </summary>
        public static bVector4 IsInfinity(dVector4 v) => dVector4.IsInfinity(v);
        
        /// <summary>
        /// Returns a bVector4 from component-wise application of IsFinite (!double.IsNaN(v) &amp;&amp; !double.IsInfinity(v)).
        /// </summary>
        public static bVector4 IsFinite(dVector4 v) => dVector4.IsFinite(v);
        
        /// <summary>
        /// Returns a bVector4 from component-wise application of IsNaN (double.IsNaN(v)).
        /// </summary>
        public static bVector4 IsNaN(dVector4 v) => dVector4.IsNaN(v);
        
        /// <summary>
        /// Returns a bVector4 from component-wise application of IsNegativeInfinity (double.IsNegativeInfinity(v)).
        /// </summary>
        public static bVector4 IsNegativeInfinity(dVector4 v) => dVector4.IsNegativeInfinity(v);
        
        /// <summary>
        /// Returns a bVector4 from component-wise application of IsPositiveInfinity (double.IsPositiveInfinity(v)).
        /// </summary>
        public static bVector4 IsPositiveInfinity(dVector4 v) => dVector4.IsPositiveInfinity(v);
        
        /// <summary>
        /// Returns a dVector4 from component-wise application of Abs (Math.Abs(v)).
        /// </summary>
        public static dVector4 Abs(dVector4 v) => dVector4.Abs(v);
        
        /// <summary>
        /// Returns a dVector4 from component-wise application of HermiteInterpolationOrder3 ((3 - 2 * v) * v * v).
        /// </summary>
        public static dVector4 HermiteInterpolationOrder3(dVector4 v) => dVector4.HermiteInterpolationOrder3(v);
        
        /// <summary>
        /// Returns a dVector4 from component-wise application of HermiteInterpolationOrder5 (((6 * v - 15) * v + 10) * v * v * v).
        /// </summary>
        public static dVector4 HermiteInterpolationOrder5(dVector4 v) => dVector4.HermiteInterpolationOrder5(v);
        
        /// <summary>
        /// Returns a dVector4 from component-wise application of Sqr (v * v).
        /// </summary>
        public static dVector4 Sqr(dVector4 v) => dVector4.Sqr(v);
        
        /// <summary>
        /// Returns a dVector4 from component-wise application of Pow2 (v * v).
        /// </summary>
        public static dVector4 Pow2(dVector4 v) => dVector4.Pow2(v);
        
        /// <summary>
        /// Returns a dVector4 from component-wise application of Pow3 (v * v * v).
        /// </summary>
        public static dVector4 Pow3(dVector4 v) => dVector4.Pow3(v);
        
        /// <summary>
        /// Returns a dVector4 from component-wise application of Step (v &gt;= 0.0 ? 1.0 : 0.0).
        /// </summary>
        public static dVector4 Step(dVector4 v) => dVector4.Step(v);
        
        /// <summary>
        /// Returns a dVector4 from component-wise application of Sqrt ((double)Math.Sqrt((double)v)).
        /// </summary>
        public static dVector4 Sqrt(dVector4 v) => dVector4.Sqrt(v);
        
        /// <summary>
        /// Returns a dVector4 from component-wise application of InverseSqrt ((double)(1.0 / Math.Sqrt((double)v))).
        /// </summary>
        public static dVector4 InverseSqrt(dVector4 v) => dVector4.InverseSqrt(v);
        
        /// <summary>
        /// Returns a iVector4 from component-wise application of Sign (Math.Sign(v)).
        /// </summary>
        public static iVector4 Sign(dVector4 v) => dVector4.Sign(v);
        
        /// <summary>
        /// Returns a dVector4 from component-wise application of Max (Math.Max(lhs, rhs)).
        /// </summary>
        public static dVector4 Max(dVector4 lhs, dVector4 rhs) => dVector4.Max(lhs, rhs);
        
        /// <summary>
        /// Returns a dVector4 from component-wise application of Min (Math.Min(lhs, rhs)).
        /// </summary>
        public static dVector4 Min(dVector4 lhs, dVector4 rhs) => dVector4.Min(lhs, rhs);
        
        /// <summary>
        /// Returns a dVector4 from component-wise application of Pow ((double)Math.Pow((double)lhs, (double)rhs)).
        /// </summary>
        public static dVector4 Pow(dVector4 lhs, dVector4 rhs) => dVector4.Pow(lhs, rhs);
        
        /// <summary>
        /// Returns a dVector4 from component-wise application of Log ((double)Math.Log((double)lhs, (double)rhs)).
        /// </summary>
        public static dVector4 Log(dVector4 lhs, dVector4 rhs) => dVector4.Log(lhs, rhs);
        
        /// <summary>
        /// Returns a dVector4 from component-wise application of Clamp (Math.Min(Math.Max(v, min), max)).
        /// </summary>
        public static dVector4 Clamp(dVector4 v, dVector4 min, dVector4 max) => dVector4.Clamp(v, min, max);
        
        /// <summary>
        /// Returns a dVector4 from component-wise application of Mix (min * (1-a) + max * a).
        /// </summary>
        public static dVector4 Mix(dVector4 min, dVector4 max, dVector4 a) => dVector4.Mix(min, max, a);
        
        /// <summary>
        /// Returns a dVector4 from component-wise application of Lerp (min * (1-a) + max * a).
        /// </summary>
        public static dVector4 Lerp(dVector4 min, dVector4 max, dVector4 a) => dVector4.Lerp(min, max, a);
        
        /// <summary>
        /// Returns a dVector4 from component-wise application of Smoothstep (((v - edge0) / (edge1 - edge0)).Clamp().HermiteInterpolationOrder3()).
        /// </summary>
        public static dVector4 Smoothstep(dVector4 edge0, dVector4 edge1, dVector4 v) => dVector4.Smoothstep(edge0, edge1, v);
        
        /// <summary>
        /// Returns a dVector4 from component-wise application of Smootherstep (((v - edge0) / (edge1 - edge0)).Clamp().HermiteInterpolationOrder5()).
        /// </summary>
        public static dVector4 Smootherstep(dVector4 edge0, dVector4 edge1, dVector4 v) => dVector4.Smootherstep(edge0, edge1, v);
        
        /// <summary>
        /// Returns a dVector4 from component-wise application of Fma (a * b + c).
        /// </summary>
        public static dVector4 Fma(dVector4 a, dVector4 b, dVector4 c) => dVector4.Fma(a, b, c);
        
        /// <summary>
        /// OuterProduct treats the first parameter c as a column Vectortor (Matrix with one column) and the second parameter r as a row Vectortor (Matrix with one row) and does a linear algebraic Matrix multiply c * r, yielding a Matrix whose number of rows is the number of components in c and whose number of columns is the number of components in r.
        /// </summary>
        public static dMatrix2x4 OuterProduct(dVector4 c, dVector2 r) => dVector4.OuterProduct(c, r);
        
        /// <summary>
        /// OuterProduct treats the first parameter c as a column Vectortor (Matrix with one column) and the second parameter r as a row Vectortor (Matrix with one row) and does a linear algebraic Matrix multiply c * r, yielding a Matrix whose number of rows is the number of components in c and whose number of columns is the number of components in r.
        /// </summary>
        public static dMatrix3x4 OuterProduct(dVector4 c, dVector3 r) => dVector4.OuterProduct(c, r);
        
        /// <summary>
        /// OuterProduct treats the first parameter c as a column Vectortor (Matrix with one column) and the second parameter r as a row Vectortor (Matrix with one row) and does a linear algebraic Matrix multiply c * r, yielding a Matrix whose number of rows is the number of components in c and whose number of columns is the number of components in r.
        /// </summary>
        public static dMatrix4 OuterProduct(dVector4 c, dVector4 r) => dVector4.OuterProduct(c, r);
        
        /// <summary>
        /// Returns a dVector4 from component-wise application of Add (lhs + rhs).
        /// </summary>
        public static dVector4 Add(dVector4 lhs, dVector4 rhs) => dVector4.Add(lhs, rhs);
        
        /// <summary>
        /// Returns a dVector4 from component-wise application of Sub (lhs - rhs).
        /// </summary>
        public static dVector4 Sub(dVector4 lhs, dVector4 rhs) => dVector4.Sub(lhs, rhs);
        
        /// <summary>
        /// Returns a dVector4 from component-wise application of Mul (lhs * rhs).
        /// </summary>
        public static dVector4 Mul(dVector4 lhs, dVector4 rhs) => dVector4.Mul(lhs, rhs);
        
        /// <summary>
        /// Returns a dVector4 from component-wise application of Div (lhs / rhs).
        /// </summary>
        public static dVector4 Div(dVector4 lhs, dVector4 rhs) => dVector4.Div(lhs, rhs);
        
        /// <summary>
        /// Returns a dVector4 from component-wise application of Modulo (lhs % rhs).
        /// </summary>
        public static dVector4 Modulo(dVector4 lhs, dVector4 rhs) => dVector4.Modulo(lhs, rhs);
        
        /// <summary>
        /// Returns a dVector4 from component-wise application of Degrees (Radians-To-Degrees Conversion).
        /// </summary>
        public static dVector4 Degrees(dVector4 v) => dVector4.Degrees(v);
        
        /// <summary>
        /// Returns a dVector4 from component-wise application of Radians (Degrees-To-Radians Conversion).
        /// </summary>
        public static dVector4 Radians(dVector4 v) => dVector4.Radians(v);
        
        /// <summary>
        /// Returns a dVector4 from component-wise application of Acos ((double)Math.Acos((double)v)).
        /// </summary>
        public static dVector4 Acos(dVector4 v) => dVector4.Acos(v);
        
        /// <summary>
        /// Returns a dVector4 from component-wise application of Asin ((double)Math.Asin((double)v)).
        /// </summary>
        public static dVector4 Asin(dVector4 v) => dVector4.Asin(v);
        
        /// <summary>
        /// Returns a dVector4 from component-wise application of Atan ((double)Math.Atan((double)v)).
        /// </summary>
        public static dVector4 Atan(dVector4 v) => dVector4.Atan(v);
        
        /// <summary>
        /// Returns a dVector4 from component-wise application of Cos ((double)Math.Cos((double)v)).
        /// </summary>
        public static dVector4 Cos(dVector4 v) => dVector4.Cos(v);
        
        /// <summary>
        /// Returns a dVector4 from component-wise application of Cosh ((double)Math.Cosh((double)v)).
        /// </summary>
        public static dVector4 Cosh(dVector4 v) => dVector4.Cosh(v);
        
        /// <summary>
        /// Returns a dVector4 from component-wise application of Exp ((double)Math.Exp((double)v)).
        /// </summary>
        public static dVector4 Exp(dVector4 v) => dVector4.Exp(v);
        
        /// <summary>
        /// Returns a dVector4 from component-wise application of Log ((double)Math.Log((double)v)).
        /// </summary>
        public static dVector4 Log(dVector4 v) => dVector4.Log(v);
        
        /// <summary>
        /// Returns a dVector4 from component-wise application of Log2 ((double)Math.Log((double)v, 2)).
        /// </summary>
        public static dVector4 Log2(dVector4 v) => dVector4.Log2(v);
        
        /// <summary>
        /// Returns a dVector4 from component-wise application of Log10 ((double)Math.Log10((double)v)).
        /// </summary>
        public static dVector4 Log10(dVector4 v) => dVector4.Log10(v);
        
        /// <summary>
        /// Returns a dVector4 from component-wise application of Floor ((double)Math.Floor(v)).
        /// </summary>
        public static dVector4 Floor(dVector4 v) => dVector4.Floor(v);
        
        /// <summary>
        /// Returns a dVector4 from component-wise application of Ceiling ((double)Math.Ceiling(v)).
        /// </summary>
        public static dVector4 Ceiling(dVector4 v) => dVector4.Ceiling(v);
        
        /// <summary>
        /// Returns a dVector4 from component-wise application of Round ((double)Math.Round(v)).
        /// </summary>
        public static dVector4 Round(dVector4 v) => dVector4.Round(v);
        
        /// <summary>
        /// Returns a dVector4 from component-wise application of Sin ((double)Math.Sin((double)v)).
        /// </summary>
        public static dVector4 Sin(dVector4 v) => dVector4.Sin(v);
        
        /// <summary>
        /// Returns a dVector4 from component-wise application of Sinh ((double)Math.Sinh((double)v)).
        /// </summary>
        public static dVector4 Sinh(dVector4 v) => dVector4.Sinh(v);
        
        /// <summary>
        /// Returns a dVector4 from component-wise application of Tan ((double)Math.Tan((double)v)).
        /// </summary>
        public static dVector4 Tan(dVector4 v) => dVector4.Tan(v);
        
        /// <summary>
        /// Returns a dVector4 from component-wise application of Tanh ((double)Math.Tanh((double)v)).
        /// </summary>
        public static dVector4 Tanh(dVector4 v) => dVector4.Tanh(v);
        
        /// <summary>
        /// Returns a dVector4 from component-wise application of Truncate ((double)Math.Truncate((double)v)).
        /// </summary>
        public static dVector4 Truncate(dVector4 v) => dVector4.Truncate(v);
        
        /// <summary>
        /// Returns a dVector4 from component-wise application of Fract ((double)(v - Math.Floor(v))).
        /// </summary>
        public static dVector4 Fract(dVector4 v) => dVector4.Fract(v);
        
        /// <summary>
        /// Returns a dVector4 from component-wise application of Trunc ((long)(v)).
        /// </summary>
        public static dVector4 Trunc(dVector4 v) => dVector4.Trunc(v);
        
        /// <summary>
        /// Returns the minimal component of this Vectortor.
        /// </summary>
        public static double MinElement(dVector4 v) => v.MinElement;
        
        /// <summary>
        /// Returns the maximal component of this Vectortor.
        /// </summary>
        public static double MaxElement(dVector4 v) => v.MaxElement;
        
        /// <summary>
        /// Returns the euclidean length of this Vectortor.
        /// </summary>
        public static double Length(dVector4 v) => v.Length;
        
        /// <summary>
        /// Returns the squared euclidean length of this Vectortor.
        /// </summary>
        public static double LengthSqr(dVector4 v) => v.LengthSqr;
        
        /// <summary>
        /// Returns the sum of all components.
        /// </summary>
        public static double Sum(dVector4 v) => v.Sum;
        
        /// <summary>
        /// Returns the euclidean norm of this Vectortor.
        /// </summary>
        public static double Norm(dVector4 v) => v.Norm;
        
        /// <summary>
        /// Returns the one-norm of this Vectortor.
        /// </summary>
        public static double Norm1(dVector4 v) => v.Norm1;
        
        /// <summary>
        /// Returns the two-norm (euclidean length) of this Vectortor.
        /// </summary>
        public static double Norm2(dVector4 v) => v.Norm2;
        
        /// <summary>
        /// Returns the max-norm of this Vectortor.
        /// </summary>
        public static double NormMax(dVector4 v) => v.NormMax;
        
        /// <summary>
        /// Returns the p-norm of this Vectortor.
        /// </summary>
        public static double NormP(dVector4 v, double p) => v.NormP(p);
        
        /// <summary>
        /// Returns a copy of this Vectortor with length one (undefined if this has zero length).
        /// </summary>
        public static dVector4 Normalized(dVector4 v) => v.Normalized;
        
        /// <summary>
        /// Returns a copy of this Vectortor with length one (returns zero if length is zero).
        /// </summary>
        public static dVector4 NormalizedSafe(dVector4 v) => v.NormalizedSafe;
        
        /// <summary>
        /// Returns the inner product (dot product, scalar product) of the two Vectortors.
        /// </summary>
        public static double Dot(dVector4 lhs, dVector4 rhs) => dVector4.Dot(lhs, rhs);
        
        /// <summary>
        /// Returns the euclidean distance between the two Vectortors.
        /// </summary>
        public static double Distance(dVector4 lhs, dVector4 rhs) => dVector4.Distance(lhs, rhs);
        
        /// <summary>
        /// Returns the squared euclidean distance between the two Vectortors.
        /// </summary>
        public static double DistanceSqr(dVector4 lhs, dVector4 rhs) => dVector4.DistanceSqr(lhs, rhs);
        
        /// <summary>
        /// Calculate the reflection direction for an incident Vectortor (N should be normalized in order to achieve the desired result).
        /// </summary>
        public static dVector4 Reflect(dVector4 I, dVector4 N) => dVector4.Reflect(I, N);
        
        /// <summary>
        /// Calculate the refraction direction for an incident Vectortor (The input parameters I and N should be normalized in order to achieve the desired result).
        /// </summary>
        public static dVector4 Refract(dVector4 I, dVector4 N, double eta) => dVector4.Refract(I, N, eta);
        
        /// <summary>
        /// Returns a Vectortor pointing in the same direction as another (faceforward orients a Vectortor to point away from a surface as defined by its normal. If dot(Nref, I) is negative faceforward returns N, otherwise it returns -N).
        /// </summary>
        public static dVector4 FaceForward(dVector4 N, dVector4 I, dVector4 Nref) => dVector4.FaceForward(N, I, Nref);
        
        /// <summary>
        /// Returns a dVector4 with independent and identically distributed uniform values between 'minValue' and 'maxValue'.
        /// </summary>
        public static dVector4 Random(System.Random random, dVector4 minValue, dVector4 maxValue) => dVector4.Random(random, minValue, maxValue);
        
        /// <summary>
        /// Returns a dVector4 with independent and identically distributed uniform values between 'minValue' and 'maxValue'.
        /// </summary>
        public static dVector4 RandomUniform(System.Random random, dVector4 minValue, dVector4 maxValue) => dVector4.RandomUniform(random, minValue, maxValue);
        
        /// <summary>
        /// Returns a dVector4 with independent and identically distributed values according to a normal/Gaussian distribution with specified mean and variance.
        /// </summary>
        public static dVector4 RandomNormal(System.Random random, dVector4 mean, dVector4 variance) => dVector4.RandomNormal(random, mean, variance);
        
        /// <summary>
        /// Returns a dVector4 with independent and identically distributed values according to a normal/Gaussian distribution with specified mean and variance.
        /// </summary>
        public static dVector4 RandomGaussian(System.Random random, dVector4 mean, dVector4 variance) => dVector4.RandomGaussian(random, mean, variance);

    }
}
