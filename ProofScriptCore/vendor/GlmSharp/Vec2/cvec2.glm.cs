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
    /// 
    /*
    public static partial class glm
    {
        
        /// <summary>
        /// Returns an object that can be used for arbitrary swizzling (e.g. swizzle.zy)
        /// </summary>
        public static swizzle_cVector2 swizzle(cVector2 v) => v.swizzle;
        
        /// <summary>
        /// Returns an array with all values
        /// </summary>
        public static Complex[] Values(cVector2 v) => v.Values;
        
        /// <summary>
        /// Returns an enumerator that iterates through all components.
        /// </summary>
        public static IEnumerator<Complex> GetEnumerator(cVector2 v) => v.GetEnumerator();
        
        /// <summary>
        /// Returns a string representation of this Vector using ', ' as a seperator.
        /// </summary>
        public static string ToString(cVector2 v) => v.ToString();
        
        /// <summary>
        /// Returns a string representation of this Vector using a provided seperator.
        /// </summary>
        public static string ToString(cVector2 v, string sep) => v.ToString(sep);
        
        /// <summary>
        /// Returns a string representation of this Vector using a provided seperator and a forMatrix provider for each component.
        /// </summary>
        public static string ToString(cVector2 v, string sep, IFormatProvider provider) => v.ToString(sep, provider);
        
        /// <summary>
        /// Returns a string representation of this Vector using a provided seperator and a forMatrix for each component.
        /// </summary>
        public static string ToString(cVector2 v, string sep, string forMatrix) => v.ToString(sep, forMatrix);
        
        /// <summary>
        /// Returns a string representation of this Vector using a provided seperator and a forMatrix and forMatrix provider for each component.
        /// </summary>
        public static string ToString(cVector2 v, string sep, string forMatrix, IFormatProvider provider) => v.ToString(sep, forMatrix, provider);
        
        /// <summary>
        /// Returns the number of components (2).
        /// </summary>
        public static int Count(cVector2 v) => v.Count;
        
        /// <summary>
        /// Returns true iff this equals rhs component-wise.
        /// </summary>
        public static bool Equals(cVector2 v, cVector2 rhs) => v.Equals(rhs);
        
        /// <summary>
        /// Returns true iff this equals rhs type- and component-wise.
        /// </summary>
        public static bool Equals(cVector2 v, object obj) => v.Equals(obj);
        
        /// <summary>
        /// Returns a hash code for this instance.
        /// </summary>
        public static int GetHashCode(cVector2 v) => v.GetHashCode();
        
        /// <summary>
        /// Returns true iff distance between lhs and rhs is less than or equal to epsilon
        /// </summary>
        public static bool ApproxEqual(cVector2 lhs, cVector2 rhs, double eps = 0.1d) => cVector2.ApproxEqual(lhs, rhs, eps);
        
        /// <summary>
        /// Returns a bVector2 from component-wise application of Equal (lhs == rhs).
        /// </summary>
        public static bVector2 Equal(cVector2 lhs, cVector2 rhs) => cVector2.Equal(lhs, rhs);
        
        /// <summary>
        /// Returns a bVector2 from component-wise application of NotEqual (lhs != rhs).
        /// </summary>
        public static bVector2 NotEqual(cVector2 lhs, cVector2 rhs) => cVector2.NotEqual(lhs, rhs);
        
        /// <summary>
        /// Returns a dVector2 from component-wise application of Abs (v.Magnitude).
        /// </summary>
        public static dVector2 Abs(cVector2 v) => cVector2.Abs(v);
        
        /// <summary>
        /// Returns a cVector2 from component-wise application of HermiteInterpolationOrder3 ((3 - 2 * v) * v * v).
        /// </summary>
        public static cVector2 HermiteInterpolationOrder3(cVector2 v) => cVector2.HermiteInterpolationOrder3(v);
        
        /// <summary>
        /// Returns a cVector2 from component-wise application of HermiteInterpolationOrder5 (((6 * v - 15) * v + 10) * v * v * v).
        /// </summary>
        public static cVector2 HermiteInterpolationOrder5(cVector2 v) => cVector2.HermiteInterpolationOrder5(v);
        
        /// <summary>
        /// Returns a cVector2 from component-wise application of Sqr (v * v).
        /// </summary>
        public static cVector2 Sqr(cVector2 v) => cVector2.Sqr(v);
        
        /// <summary>
        /// Returns a cVector2 from component-wise application of Pow2 (v * v).
        /// </summary>
        public static cVector2 Pow2(cVector2 v) => cVector2.Pow2(v);
        
        /// <summary>
        /// Returns a cVector2 from component-wise application of Pow3 (v * v * v).
        /// </summary>
        public static cVector2 Pow3(cVector2 v) => cVector2.Pow3(v);
        
        /// <summary>
        /// Returns a cVector2 from component-wise application of Acos (Complex.Acos(v)).
        /// </summary>
        public static cVector2 Acos(cVector2 v) => cVector2.Acos(v);
        
        /// <summary>
        /// Returns a cVector2 from component-wise application of Asin (Complex.Asin(v)).
        /// </summary>
        public static cVector2 Asin(cVector2 v) => cVector2.Asin(v);
        
        /// <summary>
        /// Returns a cVector2 from component-wise application of Atan (Complex.Atan(v)).
        /// </summary>
        public static cVector2 Atan(cVector2 v) => cVector2.Atan(v);
        
        /// <summary>
        /// Returns a cVector2 from component-wise application of Cos (Complex.Cos(v)).
        /// </summary>
        public static cVector2 Cos(cVector2 v) => cVector2.Cos(v);
        
        /// <summary>
        /// Returns a cVector2 from component-wise application of Cosh (Complex.Cosh(v)).
        /// </summary>
        public static cVector2 Cosh(cVector2 v) => cVector2.Cosh(v);
        
        /// <summary>
        /// Returns a cVector2 from component-wise application of Exp (Complex.Exp(v)).
        /// </summary>
        public static cVector2 Exp(cVector2 v) => cVector2.Exp(v);
        
        /// <summary>
        /// Returns a cVector2 from component-wise application of Log (Complex.Log(v)).
        /// </summary>
        public static cVector2 Log(cVector2 v) => cVector2.Log(v);
        
        /// <summary>
        /// Returns a cVector2 from component-wise application of Log2 (Complex.Log(v, 2.0)).
        /// </summary>
        public static cVector2 Log2(cVector2 v) => cVector2.Log2(v);
        
        /// <summary>
        /// Returns a cVector2 from component-wise application of Log10 (Complex.Log10(v)).
        /// </summary>
        public static cVector2 Log10(cVector2 v) => cVector2.Log10(v);
        
        /// <summary>
        /// Returns a cVector2 from component-wise application of Reciprocal (Complex.Reciprocal(v)).
        /// </summary>
        public static cVector2 Reciprocal(cVector2 v) => cVector2.Reciprocal(v);
        
        /// <summary>
        /// Returns a cVector2 from component-wise application of Sin (Complex.Sin(v)).
        /// </summary>
        public static cVector2 Sin(cVector2 v) => cVector2.Sin(v);
        
        /// <summary>
        /// Returns a cVector2 from component-wise application of Sinh (Complex.Sinh(v)).
        /// </summary>
        public static cVector2 Sinh(cVector2 v) => cVector2.Sinh(v);
        
        /// <summary>
        /// Returns a cVector2 from component-wise application of Sqrt (Complex.Sqrt(v)).
        /// </summary>
        public static cVector2 Sqrt(cVector2 v) => cVector2.Sqrt(v);
        
        /// <summary>
        /// Returns a cVector2 from component-wise application of InverseSqrt (Complex.One / Complex.Sqrt(v)).
        /// </summary>
        public static cVector2 InverseSqrt(cVector2 v) => cVector2.InverseSqrt(v);
        
        /// <summary>
        /// Returns a cVector2 from component-wise application of Tan (Complex.Tan(v)).
        /// </summary>
        public static cVector2 Tan(cVector2 v) => cVector2.Tan(v);
        
        /// <summary>
        /// Returns a cVector2 from component-wise application of Tanh (Complex.Tanh(v)).
        /// </summary>
        public static cVector2 Tanh(cVector2 v) => cVector2.Tanh(v);
        
        /// <summary>
        /// Returns a cVector2 from component-wise application of Conjugate (Complex.Conjugate(v)).
        /// </summary>
        public static cVector2 Conjugate(cVector2 v) => cVector2.Conjugate(v);
        
        /// <summary>
        /// Returns a cVector2 from component-wise application of Pow (Complex.Pow(lhs, rhs)).
        /// </summary>
        public static cVector2 Pow(cVector2 lhs, cVector2 rhs) => cVector2.Pow(lhs, rhs);
        
        /// <summary>
        /// Returns a cVector2 from component-wise application of Log (Complex.Log(lhs, rhs)).
        /// </summary>
        public static cVector2 Log(cVector2 lhs, dVector2 rhs) => cVector2.Log(lhs, rhs);
        
        /// <summary>
        /// Returns a cVector2 from component-wise application of FromPolarCoordinates (Complex.FromPolarCoordinates(magnitude, phase)).
        /// </summary>
        public static cVector2 FromPolarCoordinates(dVector2 magnitude, dVector2 phase) => cVector2.FromPolarCoordinates(magnitude, phase);
        
        /// <summary>
        /// Returns a cVector2 from component-wise application of Fma (a * b + c).
        /// </summary>
        public static cVector2 Fma(cVector2 a, cVector2 b, cVector2 c) => cVector2.Fma(a, b, c);
        
        /// <summary>
        /// OuterProduct treats the first parameter c as a column Vector (Matrix with one column) and the second parameter r as a row Vector (Matrix with one row) and does a linear algebraic Matrix multiply c * r, yielding a Matrix whose number of rows is the number of components in c and whose number of columns is the number of components in r.
        /// </summary>
        public static cMatrix2 OuterProduct(cVector2 c, cVector2 r) => cVector2.OuterProduct(c, r);
        
        /// <summary>
        /// OuterProduct treats the first parameter c as a column Vector (Matrix with one column) and the second parameter r as a row Vector (Matrix with one row) and does a linear algebraic Matrix multiply c * r, yielding a Matrix whose number of rows is the number of components in c and whose number of columns is the number of components in r.
        /// </summary>
        public static cMatrix3x2 OuterProduct(cVector2 c, cVector3 r) => cVector2.OuterProduct(c, r);
        
        /// <summary>
        /// OuterProduct treats the first parameter c as a column Vector (Matrix with one column) and the second parameter r as a row Vector (Matrix with one row) and does a linear algebraic Matrix multiply c * r, yielding a Matrix whose number of rows is the number of components in c and whose number of columns is the number of components in r.
        /// </summary>
        public static cMatrix4x2 OuterProduct(cVector2 c, cVector4 r) => cVector2.OuterProduct(c, r);
        
        /// <summary>
        /// Returns a cVector2 from component-wise application of Add (lhs + rhs).
        /// </summary>
        public static cVector2 Add(cVector2 lhs, cVector2 rhs) => cVector2.Add(lhs, rhs);
        
        /// <summary>
        /// Returns a cVector2 from component-wise application of Sub (lhs - rhs).
        /// </summary>
        public static cVector2 Sub(cVector2 lhs, cVector2 rhs) => cVector2.Sub(lhs, rhs);
        
        /// <summary>
        /// Returns a cVector2 from component-wise application of Mul (lhs * rhs).
        /// </summary>
        public static cVector2 Mul(cVector2 lhs, cVector2 rhs) => cVector2.Mul(lhs, rhs);
        
        /// <summary>
        /// Returns a cVector2 from component-wise application of Div (lhs / rhs).
        /// </summary>
        public static cVector2 Div(cVector2 lhs, cVector2 rhs) => cVector2.Div(lhs, rhs);
        
        /// <summary>
        /// Returns the euclidean length of this Vector.
        /// </summary>
        public static double Length(cVector2 v) => v.Length;
        
        /// <summary>
        /// Returns the squared euclidean length of this Vector.
        /// </summary>
        public static double LengthSqr(cVector2 v) => v.LengthSqr;
        
        /// <summary>
        /// Returns the sum of all components.
        /// </summary>
        public static Complex Sum(cVector2 v) => v.Sum;
        
        /// <summary>
        /// Returns the euclidean norm of this Vector.
        /// </summary>
        public static double Norm(cVector2 v) => v.Norm;
        
        /// <summary>
        /// Returns the one-norm of this Vector.
        /// </summary>
        public static double Norm1(cVector2 v) => v.Norm1;
        
        /// <summary>
        /// Returns the two-norm (euclidean length) of this Vector.
        /// </summary>
        public static double Norm2(cVector2 v) => v.Norm2;
        
        /// <summary>
        /// Returns the max-norm of this Vector.
        /// </summary>
        public static double NormMax(cVector2 v) => v.NormMax;
        
        /// <summary>
        /// Returns the p-norm of this Vector.
        /// </summary>
        public static double NormP(cVector2 v, double p) => v.NormP(p);
        
        /// <summary>
        /// Returns a copy of this Vector with length one (undefined if this has zero length).
        /// </summary>
        public static cVector2 Normalized(cVector2 v) => v.Normalized;
        
        /// <summary>
        /// Returns a copy of this Vector with length one (returns zero if length is zero).
        /// </summary>
        public static cVector2 NormalizedSafe(cVector2 v) => v.NormalizedSafe;
        
        /// <summary>
        /// Returns a Vector containing component-wise magnitudes.
        /// </summary>
        public static dVector2 Magnitude(cVector2 v) => v.Magnitude;
        
        /// <summary>
        /// Returns a Vector containing component-wise phases.
        /// </summary>
        public static dVector2 Phase(cVector2 v) => v.Phase;
        
        /// <summary>
        /// Returns a Vector containing component-wise imaginary parts.
        /// </summary>
        public static dVector2 Imaginary(cVector2 v) => v.Imaginary;
        
        /// <summary>
        /// Returns a Vector containing component-wise real parts.
        /// </summary>
        public static dVector2 Real(cVector2 v) => v.Real;
        
        /// <summary>
        /// Returns the inner product (dot product, scalar product) of the two Vectors.
        /// </summary>
        public static Complex Dot(cVector2 lhs, cVector2 rhs) => cVector2.Dot(lhs, rhs);
        
        /// <summary>
        /// Returns the euclidean distance between the two Vectors.
        /// </summary>
        public static double Distance(cVector2 lhs, cVector2 rhs) => cVector2.Distance(lhs, rhs);
        
        /// <summary>
        /// Returns the squared euclidean distance between the two Vectors.
        /// </summary>
        public static double DistanceSqr(cVector2 lhs, cVector2 rhs) => cVector2.DistanceSqr(lhs, rhs);
        
        /// <summary>
        /// Calculate the reflection direction for an incident Vector (N should be normalized in order to achieve the desired result).
        /// </summary>
        public static cVector2 Reflect(cVector2 I, cVector2 N) => cVector2.Reflect(I, N);
        
        /// <summary>
        /// Returns the length of the outer product (cross product, Vector product) of the two Vectors.
        /// </summary>
        public static Complex Cross(cVector2 l, cVector2 r) => cVector2.Cross(l, r);

    }
    */
}
