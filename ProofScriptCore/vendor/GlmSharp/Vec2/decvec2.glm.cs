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
        public static swizzle_decVector2 swizzle(decVector2 v) => v.swizzle;
        
        /// <summary>
        /// Returns an array with all values
        /// </summary>
        public static decimal[] Values(decVector2 v) => v.Values;
        
        /// <summary>
        /// Returns an enumerator that iterates through all components.
        /// </summary>
        public static IEnumerator<decimal> GetEnumerator(decVector2 v) => v.GetEnumerator();
        
        /// <summary>
        /// Returns a string representation of this Vectortor using ', ' as a seperator.
        /// </summary>
        public static string ToString(decVector2 v) => v.ToString();
        
        /// <summary>
        /// Returns a string representation of this Vectortor using a provided seperator.
        /// </summary>
        public static string ToString(decVector2 v, string sep) => v.ToString(sep);
        
        /// <summary>
        /// Returns a string representation of this Vectortor using a provided seperator and a forMatrix provider for each component.
        /// </summary>
        public static string ToString(decVector2 v, string sep, IFormatProvider provider) => v.ToString(sep, provider);
        
        /// <summary>
        /// Returns a string representation of this Vectortor using a provided seperator and a forMatrix for each component.
        /// </summary>
        public static string ToString(decVector2 v, string sep, string forMatrix) => v.ToString(sep, forMatrix);
        
        /// <summary>
        /// Returns a string representation of this Vectortor using a provided seperator and a forMatrix and forMatrix provider for each component.
        /// </summary>
        public static string ToString(decVector2 v, string sep, string forMatrix, IFormatProvider provider) => v.ToString(sep, forMatrix, provider);
        
        /// <summary>
        /// Returns the number of components (2).
        /// </summary>
        public static int Count(decVector2 v) => v.Count;
        
        /// <summary>
        /// Returns true iff this equals rhs component-wise.
        /// </summary>
        public static bool Equals(decVector2 v, decVector2 rhs) => v.Equals(rhs);
        
        /// <summary>
        /// Returns true iff this equals rhs type- and component-wise.
        /// </summary>
        public static bool Equals(decVector2 v, object obj) => v.Equals(obj);
        
        /// <summary>
        /// Returns a hash code for this instance.
        /// </summary>
        public static int GetHashCode(decVector2 v) => v.GetHashCode();
        
        /// <summary>
        /// Returns true iff distance between lhs and rhs is less than or equal to epsilon
        /// </summary>
        public static bool ApproxEqual(decVector2 lhs, decVector2 rhs, decimal eps = 0.1m) => decVector2.ApproxEqual(lhs, rhs, eps);
        
        /// <summary>
        /// Returns a bVector2 from component-wise application of Equal (lhs == rhs).
        /// </summary>
        public static bVector2 Equal(decVector2 lhs, decVector2 rhs) => decVector2.Equal(lhs, rhs);
        
        /// <summary>
        /// Returns a bVector2 from component-wise application of NotEqual (lhs != rhs).
        /// </summary>
        public static bVector2 NotEqual(decVector2 lhs, decVector2 rhs) => decVector2.NotEqual(lhs, rhs);
        
        /// <summary>
        /// Returns a bVector2 from component-wise application of GreaterThan (lhs &gt; rhs).
        /// </summary>
        public static bVector2 GreaterThan(decVector2 lhs, decVector2 rhs) => decVector2.GreaterThan(lhs, rhs);
        
        /// <summary>
        /// Returns a bVector2 from component-wise application of GreaterThanEqual (lhs &gt;= rhs).
        /// </summary>
        public static bVector2 GreaterThanEqual(decVector2 lhs, decVector2 rhs) => decVector2.GreaterThanEqual(lhs, rhs);
        
        /// <summary>
        /// Returns a bVector2 from component-wise application of LesserThan (lhs &lt; rhs).
        /// </summary>
        public static bVector2 LesserThan(decVector2 lhs, decVector2 rhs) => decVector2.LesserThan(lhs, rhs);
        
        /// <summary>
        /// Returns a bVector2 from component-wise application of LesserThanEqual (lhs &lt;= rhs).
        /// </summary>
        public static bVector2 LesserThanEqual(decVector2 lhs, decVector2 rhs) => decVector2.LesserThanEqual(lhs, rhs);
        
        /// <summary>
        /// Returns a decVector2 from component-wise application of Abs (Math.Abs(v)).
        /// </summary>
        public static decVector2 Abs(decVector2 v) => decVector2.Abs(v);
        
        /// <summary>
        /// Returns a decVector2 from component-wise application of HermiteInterpolationOrder3 ((3 - 2 * v) * v * v).
        /// </summary>
        public static decVector2 HermiteInterpolationOrder3(decVector2 v) => decVector2.HermiteInterpolationOrder3(v);
        
        /// <summary>
        /// Returns a decVector2 from component-wise application of HermiteInterpolationOrder5 (((6 * v - 15) * v + 10) * v * v * v).
        /// </summary>
        public static decVector2 HermiteInterpolationOrder5(decVector2 v) => decVector2.HermiteInterpolationOrder5(v);
        
        /// <summary>
        /// Returns a decVector2 from component-wise application of Sqr (v * v).
        /// </summary>
        public static decVector2 Sqr(decVector2 v) => decVector2.Sqr(v);
        
        /// <summary>
        /// Returns a decVector2 from component-wise application of Pow2 (v * v).
        /// </summary>
        public static decVector2 Pow2(decVector2 v) => decVector2.Pow2(v);
        
        /// <summary>
        /// Returns a decVector2 from component-wise application of Pow3 (v * v * v).
        /// </summary>
        public static decVector2 Pow3(decVector2 v) => decVector2.Pow3(v);
        
        /// <summary>
        /// Returns a decVector2 from component-wise application of Step (v &gt;= 0m ? 1m : 0m).
        /// </summary>
        public static decVector2 Step(decVector2 v) => decVector2.Step(v);
        
        /// <summary>
        /// Returns a decVector2 from component-wise application of Sqrt ((decimal)Math.Sqrt((double)v)).
        /// </summary>
        public static decVector2 Sqrt(decVector2 v) => decVector2.Sqrt(v);
        
        /// <summary>
        /// Returns a decVector2 from component-wise application of InverseSqrt ((decimal)(1.0 / Math.Sqrt((double)v))).
        /// </summary>
        public static decVector2 InverseSqrt(decVector2 v) => decVector2.InverseSqrt(v);
        
        /// <summary>
        /// Returns a iVector2 from component-wise application of Sign (Math.Sign(v)).
        /// </summary>
        public static iVector2 Sign(decVector2 v) => decVector2.Sign(v);
        
        /// <summary>
        /// Returns a decVector2 from component-wise application of Max (Math.Max(lhs, rhs)).
        /// </summary>
        public static decVector2 Max(decVector2 lhs, decVector2 rhs) => decVector2.Max(lhs, rhs);
        
        /// <summary>
        /// Returns a decVector2 from component-wise application of Min (Math.Min(lhs, rhs)).
        /// </summary>
        public static decVector2 Min(decVector2 lhs, decVector2 rhs) => decVector2.Min(lhs, rhs);
        
        /// <summary>
        /// Returns a decVector2 from component-wise application of Pow ((decimal)Math.Pow((double)lhs, (double)rhs)).
        /// </summary>
        public static decVector2 Pow(decVector2 lhs, decVector2 rhs) => decVector2.Pow(lhs, rhs);
        
        /// <summary>
        /// Returns a decVector2 from component-wise application of Log ((decimal)Math.Log((double)lhs, (double)rhs)).
        /// </summary>
        public static decVector2 Log(decVector2 lhs, decVector2 rhs) => decVector2.Log(lhs, rhs);
        
        /// <summary>
        /// Returns a decVector2 from component-wise application of Clamp (Math.Min(Math.Max(v, min), max)).
        /// </summary>
        public static decVector2 Clamp(decVector2 v, decVector2 min, decVector2 max) => decVector2.Clamp(v, min, max);
        
        /// <summary>
        /// Returns a decVector2 from component-wise application of Mix (min * (1-a) + max * a).
        /// </summary>
        public static decVector2 Mix(decVector2 min, decVector2 max, decVector2 a) => decVector2.Mix(min, max, a);
        
        /// <summary>
        /// Returns a decVector2 from component-wise application of Lerp (min * (1-a) + max * a).
        /// </summary>
        public static decVector2 Lerp(decVector2 min, decVector2 max, decVector2 a) => decVector2.Lerp(min, max, a);
        
        /// <summary>
        /// Returns a decVector2 from component-wise application of Smoothstep (((v - edge0) / (edge1 - edge0)).Clamp().HermiteInterpolationOrder3()).
        /// </summary>
        public static decVector2 Smoothstep(decVector2 edge0, decVector2 edge1, decVector2 v) => decVector2.Smoothstep(edge0, edge1, v);
        
        /// <summary>
        /// Returns a decVector2 from component-wise application of Smootherstep (((v - edge0) / (edge1 - edge0)).Clamp().HermiteInterpolationOrder5()).
        /// </summary>
        public static decVector2 Smootherstep(decVector2 edge0, decVector2 edge1, decVector2 v) => decVector2.Smootherstep(edge0, edge1, v);
        
        /// <summary>
        /// Returns a decVector2 from component-wise application of Fma (a * b + c).
        /// </summary>
        public static decVector2 Fma(decVector2 a, decVector2 b, decVector2 c) => decVector2.Fma(a, b, c);
        
        /// <summary>
        /// OuterProduct treats the first parameter c as a column Vectortor (Matrix with one column) and the second parameter r as a row Vectortor (Matrix with one row) and does a linear algebraic Matrix multiply c * r, yielding a Matrix whose number of rows is the number of components in c and whose number of columns is the number of components in r.
        /// </summary>
        public static decMatrix2 OuterProduct(decVector2 c, decVector2 r) => decVector2.OuterProduct(c, r);
        
        /// <summary>
        /// OuterProduct treats the first parameter c as a column Vectortor (Matrix with one column) and the second parameter r as a row Vectortor (Matrix with one row) and does a linear algebraic Matrix multiply c * r, yielding a Matrix whose number of rows is the number of components in c and whose number of columns is the number of components in r.
        /// </summary>
        public static decMatrix3x2 OuterProduct(decVector2 c, decVector3 r) => decVector2.OuterProduct(c, r);
        
        /// <summary>
        /// OuterProduct treats the first parameter c as a column Vectortor (Matrix with one column) and the second parameter r as a row Vectortor (Matrix with one row) and does a linear algebraic Matrix multiply c * r, yielding a Matrix whose number of rows is the number of components in c and whose number of columns is the number of components in r.
        /// </summary>
        public static decMatrix4x2 OuterProduct(decVector2 c, decVector4 r) => decVector2.OuterProduct(c, r);
        
        /// <summary>
        /// Returns a decVector2 from component-wise application of Add (lhs + rhs).
        /// </summary>
        public static decVector2 Add(decVector2 lhs, decVector2 rhs) => decVector2.Add(lhs, rhs);
        
        /// <summary>
        /// Returns a decVector2 from component-wise application of Sub (lhs - rhs).
        /// </summary>
        public static decVector2 Sub(decVector2 lhs, decVector2 rhs) => decVector2.Sub(lhs, rhs);
        
        /// <summary>
        /// Returns a decVector2 from component-wise application of Mul (lhs * rhs).
        /// </summary>
        public static decVector2 Mul(decVector2 lhs, decVector2 rhs) => decVector2.Mul(lhs, rhs);
        
        /// <summary>
        /// Returns a decVector2 from component-wise application of Div (lhs / rhs).
        /// </summary>
        public static decVector2 Div(decVector2 lhs, decVector2 rhs) => decVector2.Div(lhs, rhs);
        
        /// <summary>
        /// Returns a decVector2 from component-wise application of Modulo (lhs % rhs).
        /// </summary>
        public static decVector2 Modulo(decVector2 lhs, decVector2 rhs) => decVector2.Modulo(lhs, rhs);
        
        /// <summary>
        /// Returns a decVector2 from component-wise application of Degrees (Radians-To-Degrees Conversion).
        /// </summary>
        public static decVector2 Degrees(decVector2 v) => decVector2.Degrees(v);
        
        /// <summary>
        /// Returns a decVector2 from component-wise application of Radians (Degrees-To-Radians Conversion).
        /// </summary>
        public static decVector2 Radians(decVector2 v) => decVector2.Radians(v);
        
        /// <summary>
        /// Returns a decVector2 from component-wise application of Acos ((decimal)Math.Acos((double)v)).
        /// </summary>
        public static decVector2 Acos(decVector2 v) => decVector2.Acos(v);
        
        /// <summary>
        /// Returns a decVector2 from component-wise application of Asin ((decimal)Math.Asin((double)v)).
        /// </summary>
        public static decVector2 Asin(decVector2 v) => decVector2.Asin(v);
        
        /// <summary>
        /// Returns a decVector2 from component-wise application of Atan ((decimal)Math.Atan((double)v)).
        /// </summary>
        public static decVector2 Atan(decVector2 v) => decVector2.Atan(v);
        
        /// <summary>
        /// Returns a decVector2 from component-wise application of Cos ((decimal)Math.Cos((double)v)).
        /// </summary>
        public static decVector2 Cos(decVector2 v) => decVector2.Cos(v);
        
        /// <summary>
        /// Returns a decVector2 from component-wise application of Cosh ((decimal)Math.Cosh((double)v)).
        /// </summary>
        public static decVector2 Cosh(decVector2 v) => decVector2.Cosh(v);
        
        /// <summary>
        /// Returns a decVector2 from component-wise application of Exp ((decimal)Math.Exp((double)v)).
        /// </summary>
        public static decVector2 Exp(decVector2 v) => decVector2.Exp(v);
        
        /// <summary>
        /// Returns a decVector2 from component-wise application of Log ((decimal)Math.Log((double)v)).
        /// </summary>
        public static decVector2 Log(decVector2 v) => decVector2.Log(v);
        
        /// <summary>
        /// Returns a decVector2 from component-wise application of Log2 ((decimal)Math.Log((double)v, 2)).
        /// </summary>
        public static decVector2 Log2(decVector2 v) => decVector2.Log2(v);
        
        /// <summary>
        /// Returns a decVector2 from component-wise application of Log10 ((decimal)Math.Log10((double)v)).
        /// </summary>
        public static decVector2 Log10(decVector2 v) => decVector2.Log10(v);
        
        /// <summary>
        /// Returns a decVector2 from component-wise application of Floor ((decimal)Math.Floor(v)).
        /// </summary>
        public static decVector2 Floor(decVector2 v) => decVector2.Floor(v);
        
        /// <summary>
        /// Returns a decVector2 from component-wise application of Ceiling ((decimal)Math.Ceiling(v)).
        /// </summary>
        public static decVector2 Ceiling(decVector2 v) => decVector2.Ceiling(v);
        
        /// <summary>
        /// Returns a decVector2 from component-wise application of Round ((decimal)Math.Round(v)).
        /// </summary>
        public static decVector2 Round(decVector2 v) => decVector2.Round(v);
        
        /// <summary>
        /// Returns a decVector2 from component-wise application of Sin ((decimal)Math.Sin((double)v)).
        /// </summary>
        public static decVector2 Sin(decVector2 v) => decVector2.Sin(v);
        
        /// <summary>
        /// Returns a decVector2 from component-wise application of Sinh ((decimal)Math.Sinh((double)v)).
        /// </summary>
        public static decVector2 Sinh(decVector2 v) => decVector2.Sinh(v);
        
        /// <summary>
        /// Returns a decVector2 from component-wise application of Tan ((decimal)Math.Tan((double)v)).
        /// </summary>
        public static decVector2 Tan(decVector2 v) => decVector2.Tan(v);
        
        /// <summary>
        /// Returns a decVector2 from component-wise application of Tanh ((decimal)Math.Tanh((double)v)).
        /// </summary>
        public static decVector2 Tanh(decVector2 v) => decVector2.Tanh(v);
        
        /// <summary>
        /// Returns a decVector2 from component-wise application of Truncate ((decimal)Math.Truncate((double)v)).
        /// </summary>
        public static decVector2 Truncate(decVector2 v) => decVector2.Truncate(v);
        
        /// <summary>
        /// Returns a decVector2 from component-wise application of Fract ((decimal)(v - Math.Floor(v))).
        /// </summary>
        public static decVector2 Fract(decVector2 v) => decVector2.Fract(v);
        
        /// <summary>
        /// Returns a decVector2 from component-wise application of Trunc ((long)(v)).
        /// </summary>
        public static decVector2 Trunc(decVector2 v) => decVector2.Trunc(v);
        
        /// <summary>
        /// Returns the minimal component of this Vectortor.
        /// </summary>
        public static decimal MinElement(decVector2 v) => v.MinElement;
        
        /// <summary>
        /// Returns the maximal component of this Vectortor.
        /// </summary>
        public static decimal MaxElement(decVector2 v) => v.MaxElement;
        
        /// <summary>
        /// Returns the euclidean length of this Vectortor.
        /// </summary>
        public static decimal Length(decVector2 v) => v.Length;
        
        /// <summary>
        /// Returns the squared euclidean length of this Vectortor.
        /// </summary>
        public static decimal LengthSqr(decVector2 v) => v.LengthSqr;
        
        /// <summary>
        /// Returns the sum of all components.
        /// </summary>
        public static decimal Sum(decVector2 v) => v.Sum;
        
        /// <summary>
        /// Returns the euclidean norm of this Vectortor.
        /// </summary>
        public static decimal Norm(decVector2 v) => v.Norm;
        
        /// <summary>
        /// Returns the one-norm of this Vectortor.
        /// </summary>
        public static decimal Norm1(decVector2 v) => v.Norm1;
        
        /// <summary>
        /// Returns the two-norm (euclidean length) of this Vectortor.
        /// </summary>
        public static decimal Norm2(decVector2 v) => v.Norm2;
        
        /// <summary>
        /// Returns the max-norm of this Vectortor.
        /// </summary>
        public static decimal NormMax(decVector2 v) => v.NormMax;
        
        /// <summary>
        /// Returns the p-norm of this Vectortor.
        /// </summary>
        public static double NormP(decVector2 v, double p) => v.NormP(p);
        
        /// <summary>
        /// Returns a copy of this Vectortor with length one (undefined if this has zero length).
        /// </summary>
        public static decVector2 Normalized(decVector2 v) => v.Normalized;
        
        /// <summary>
        /// Returns a copy of this Vectortor with length one (returns zero if length is zero).
        /// </summary>
        public static decVector2 NormalizedSafe(decVector2 v) => v.NormalizedSafe;
        
        /// <summary>
        /// Returns the Vectortor angle (atan2(y, x)) in radians.
        /// </summary>
        public static double Angle(decVector2 v) => v.Angle;
        
        /// <summary>
        /// Returns a 2D Vectortor that was rotated by a given angle in radians (CAUTION: result is casted and may be truncated).
        /// </summary>
        public static decVector2 Rotated(decVector2 v, double angleInRad) => v.Rotated(angleInRad);
        
        /// <summary>
        /// Returns the inner product (dot product, scalar product) of the two Vectortors.
        /// </summary>
        public static decimal Dot(decVector2 lhs, decVector2 rhs) => decVector2.Dot(lhs, rhs);
        
        /// <summary>
        /// Returns the euclidean distance between the two Vectortors.
        /// </summary>
        public static decimal Distance(decVector2 lhs, decVector2 rhs) => decVector2.Distance(lhs, rhs);
        
        /// <summary>
        /// Returns the squared euclidean distance between the two Vectortors.
        /// </summary>
        public static decimal DistanceSqr(decVector2 lhs, decVector2 rhs) => decVector2.DistanceSqr(lhs, rhs);
        
        /// <summary>
        /// Calculate the reflection direction for an incident Vectortor (N should be normalized in order to achieve the desired result).
        /// </summary>
        public static decVector2 Reflect(decVector2 I, decVector2 N) => decVector2.Reflect(I, N);
        
        /// <summary>
        /// Calculate the refraction direction for an incident Vectortor (The input parameters I and N should be normalized in order to achieve the desired result).
        /// </summary>
        public static decVector2 Refract(decVector2 I, decVector2 N, decimal eta) => decVector2.Refract(I, N, eta);
        
        /// <summary>
        /// Returns a Vectortor pointing in the same direction as another (faceforward orients a Vectortor to point away from a surface as defined by its normal. If dot(Nref, I) is negative faceforward returns N, otherwise it returns -N).
        /// </summary>
        public static decVector2 FaceForward(decVector2 N, decVector2 I, decVector2 Nref) => decVector2.FaceForward(N, I, Nref);
        
        /// <summary>
        /// Returns the length of the outer product (cross product, Vectortor product) of the two Vectortors.
        /// </summary>
        public static decimal Cross(decVector2 l, decVector2 r) => decVector2.Cross(l, r);
        
        /// <summary>
        /// Returns a decVector2 with independent and identically distributed uniform values between 'minValue' and 'maxValue'.
        /// </summary>
        public static decVector2 Random(System.Random random, decVector2 minValue, decVector2 maxValue) => decVector2.Random(random, minValue, maxValue);
        
        /// <summary>
        /// Returns a decVector2 with independent and identically distributed uniform values between 'minValue' and 'maxValue'.
        /// </summary>
        public static decVector2 RandomUniform(System.Random random, decVector2 minValue, decVector2 maxValue) => decVector2.RandomUniform(random, minValue, maxValue);
        
        /// <summary>
        /// Returns a decVector2 with independent and identically distributed values according to a normal/Gaussian distribution with specified mean and variance.
        /// </summary>
        public static decVector2 RandomNormal(System.Random random, decVector2 mean, decVector2 variance) => decVector2.RandomNormal(random, mean, variance);
        
        /// <summary>
        /// Returns a decVector2 with independent and identically distributed values according to a normal/Gaussian distribution with specified mean and variance.
        /// </summary>
        public static decVector2 RandomGaussian(System.Random random, decVector2 mean, decVector2 variance) => decVector2.RandomGaussian(random, mean, variance);

    }
}
