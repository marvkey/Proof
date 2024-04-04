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
        public static swizzle_cVector4 swizzle(cVector4 v) => v.swizzle;
        
        /// <summary>
        /// Returns an array with all values
        /// </summary>
        public static Complex[] Values(cVector4 v) => v.Values;
        
        /// <summary>
        /// Returns an enumerator that iterates through all components.
        /// </summary>
        public static IEnumerator<Complex> GetEnumerator(cVector4 v) => v.GetEnumerator();
        
        /// <summary>
        /// Returns a string representation of this Vectortor using ', ' as a seperator.
        /// </summary>
        public static string ToString(cVector4 v) => v.ToString();
        
        /// <summary>
        /// Returns a string representation of this Vectortor using a provided seperator.
        /// </summary>
        public static string ToString(cVector4 v, string sep) => v.ToString(sep);
        
        /// <summary>
        /// Returns a string representation of this Vectortor using a provided seperator and a forMatrix provider for each component.
        /// </summary>
        public static string ToString(cVector4 v, string sep, IFormatProvider provider) => v.ToString(sep, provider);
        
        /// <summary>
        /// Returns a string representation of this Vectortor using a provided seperator and a forMatrix for each component.
        /// </summary>
        public static string ToString(cVector4 v, string sep, string forMatrix) => v.ToString(sep, forMatrix);
        
        /// <summary>
        /// Returns a string representation of this Vectortor using a provided seperator and a forMatrix and forMatrix provider for each component.
        /// </summary>
        public static string ToString(cVector4 v, string sep, string forMatrix, IFormatProvider provider) => v.ToString(sep, forMatrix, provider);
        
        /// <summary>
        /// Returns the number of components (4).
        /// </summary>
        public static int Count(cVector4 v) => v.Count;
        
        /// <summary>
        /// Returns true iff this equals rhs component-wise.
        /// </summary>
        public static bool Equals(cVector4 v, cVector4 rhs) => v.Equals(rhs);
        
        /// <summary>
        /// Returns true iff this equals rhs type- and component-wise.
        /// </summary>
        public static bool Equals(cVector4 v, object obj) => v.Equals(obj);
        
        /// <summary>
        /// Returns a hash code for this instance.
        /// </summary>
        public static int GetHashCode(cVector4 v) => v.GetHashCode();
        
        /// <summary>
        /// Returns true iff distance between lhs and rhs is less than or equal to epsilon
        /// </summary>
        public static bool ApproxEqual(cVector4 lhs, cVector4 rhs, double eps = 0.1d) => cVector4.ApproxEqual(lhs, rhs, eps);
        
        /// <summary>
        /// Returns a bVector4 from component-wise application of Equal (lhs == rhs).
        /// </summary>
        public static bVector4 Equal(cVector4 lhs, cVector4 rhs) => cVector4.Equal(lhs, rhs);
        
        /// <summary>
        /// Returns a bVector4 from component-wise application of NotEqual (lhs != rhs).
        /// </summary>
        public static bVector4 NotEqual(cVector4 lhs, cVector4 rhs) => cVector4.NotEqual(lhs, rhs);
        
        /// <summary>
        /// Returns a dVector4 from component-wise application of Abs (v.Magnitude).
        /// </summary>
        public static dVector4 Abs(cVector4 v) => cVector4.Abs(v);
        
        /// <summary>
        /// Returns a cVector4 from component-wise application of HermiteInterpolationOrder3 ((3 - 2 * v) * v * v).
        /// </summary>
        public static cVector4 HermiteInterpolationOrder3(cVector4 v) => cVector4.HermiteInterpolationOrder3(v);
        
        /// <summary>
        /// Returns a cVector4 from component-wise application of HermiteInterpolationOrder5 (((6 * v - 15) * v + 10) * v * v * v).
        /// </summary>
        public static cVector4 HermiteInterpolationOrder5(cVector4 v) => cVector4.HermiteInterpolationOrder5(v);
        
        /// <summary>
        /// Returns a cVector4 from component-wise application of Sqr (v * v).
        /// </summary>
        public static cVector4 Sqr(cVector4 v) => cVector4.Sqr(v);
        
        /// <summary>
        /// Returns a cVector4 from component-wise application of Pow2 (v * v).
        /// </summary>
        public static cVector4 Pow2(cVector4 v) => cVector4.Pow2(v);
        
        /// <summary>
        /// Returns a cVector4 from component-wise application of Pow3 (v * v * v).
        /// </summary>
        public static cVector4 Pow3(cVector4 v) => cVector4.Pow3(v);
        
        /// <summary>
        /// Returns a cVector4 from component-wise application of Acos (Complex.Acos(v)).
        /// </summary>
        public static cVector4 Acos(cVector4 v) => cVector4.Acos(v);
        
        /// <summary>
        /// Returns a cVector4 from component-wise application of Asin (Complex.Asin(v)).
        /// </summary>
        public static cVector4 Asin(cVector4 v) => cVector4.Asin(v);
        
        /// <summary>
        /// Returns a cVector4 from component-wise application of Atan (Complex.Atan(v)).
        /// </summary>
        public static cVector4 Atan(cVector4 v) => cVector4.Atan(v);
        
        /// <summary>
        /// Returns a cVector4 from component-wise application of Cos (Complex.Cos(v)).
        /// </summary>
        public static cVector4 Cos(cVector4 v) => cVector4.Cos(v);
        
        /// <summary>
        /// Returns a cVector4 from component-wise application of Cosh (Complex.Cosh(v)).
        /// </summary>
        public static cVector4 Cosh(cVector4 v) => cVector4.Cosh(v);
        
        /// <summary>
        /// Returns a cVector4 from component-wise application of Exp (Complex.Exp(v)).
        /// </summary>
        public static cVector4 Exp(cVector4 v) => cVector4.Exp(v);
        
        /// <summary>
        /// Returns a cVector4 from component-wise application of Log (Complex.Log(v)).
        /// </summary>
        public static cVector4 Log(cVector4 v) => cVector4.Log(v);
        
        /// <summary>
        /// Returns a cVector4 from component-wise application of Log2 (Complex.Log(v, 2.0)).
        /// </summary>
        public static cVector4 Log2(cVector4 v) => cVector4.Log2(v);
        
        /// <summary>
        /// Returns a cVector4 from component-wise application of Log10 (Complex.Log10(v)).
        /// </summary>
        public static cVector4 Log10(cVector4 v) => cVector4.Log10(v);
        
        /// <summary>
        /// Returns a cVector4 from component-wise application of Reciprocal (Complex.Reciprocal(v)).
        /// </summary>
        public static cVector4 Reciprocal(cVector4 v) => cVector4.Reciprocal(v);
        
        /// <summary>
        /// Returns a cVector4 from component-wise application of Sin (Complex.Sin(v)).
        /// </summary>
        public static cVector4 Sin(cVector4 v) => cVector4.Sin(v);
        
        /// <summary>
        /// Returns a cVector4 from component-wise application of Sinh (Complex.Sinh(v)).
        /// </summary>
        public static cVector4 Sinh(cVector4 v) => cVector4.Sinh(v);
        
        /// <summary>
        /// Returns a cVector4 from component-wise application of Sqrt (Complex.Sqrt(v)).
        /// </summary>
        public static cVector4 Sqrt(cVector4 v) => cVector4.Sqrt(v);
        
        /// <summary>
        /// Returns a cVector4 from component-wise application of InverseSqrt (Complex.One / Complex.Sqrt(v)).
        /// </summary>
        public static cVector4 InverseSqrt(cVector4 v) => cVector4.InverseSqrt(v);
        
        /// <summary>
        /// Returns a cVector4 from component-wise application of Tan (Complex.Tan(v)).
        /// </summary>
        public static cVector4 Tan(cVector4 v) => cVector4.Tan(v);
        
        /// <summary>
        /// Returns a cVector4 from component-wise application of Tanh (Complex.Tanh(v)).
        /// </summary>
        public static cVector4 Tanh(cVector4 v) => cVector4.Tanh(v);
        
        /// <summary>
        /// Returns a cVector4 from component-wise application of Conjugate (Complex.Conjugate(v)).
        /// </summary>
        public static cVector4 Conjugate(cVector4 v) => cVector4.Conjugate(v);
        
        /// <summary>
        /// Returns a cVector4 from component-wise application of Pow (Complex.Pow(lhs, rhs)).
        /// </summary>
        public static cVector4 Pow(cVector4 lhs, cVector4 rhs) => cVector4.Pow(lhs, rhs);
        
        /// <summary>
        /// Returns a cVector4 from component-wise application of Log (Complex.Log(lhs, rhs)).
        /// </summary>
        public static cVector4 Log(cVector4 lhs, dVector4 rhs) => cVector4.Log(lhs, rhs);
        
        /// <summary>
        /// Returns a cVector4 from component-wise application of FromPolarCoordinates (Complex.FromPolarCoordinates(magnitude, phase)).
        /// </summary>
        public static cVector4 FromPolarCoordinates(dVector4 magnitude, dVector4 phase) => cVector4.FromPolarCoordinates(magnitude, phase);
        
        /// <summary>
        /// Returns a cVector4 from component-wise application of Fma (a * b + c).
        /// </summary>
        public static cVector4 Fma(cVector4 a, cVector4 b, cVector4 c) => cVector4.Fma(a, b, c);
        
        /// <summary>
        /// OuterProduct treats the first parameter c as a column Vectortor (Matrix with one column) and the second parameter r as a row Vectortor (Matrix with one row) and does a linear algebraic Matrix multiply c * r, yielding a Matrix whose number of rows is the number of components in c and whose number of columns is the number of components in r.
        /// </summary>
        public static cMatrix2x4 OuterProduct(cVector4 c, cVector2 r) => cVector4.OuterProduct(c, r);
        
        /// <summary>
        /// OuterProduct treats the first parameter c as a column Vectortor (Matrix with one column) and the second parameter r as a row Vectortor (Matrix with one row) and does a linear algebraic Matrix multiply c * r, yielding a Matrix whose number of rows is the number of components in c and whose number of columns is the number of components in r.
        /// </summary>
        public static cMatrix3x4 OuterProduct(cVector4 c, cVector3 r) => cVector4.OuterProduct(c, r);
        
        /// <summary>
        /// OuterProduct treats the first parameter c as a column Vectortor (Matrix with one column) and the second parameter r as a row Vectortor (Matrix with one row) and does a linear algebraic Matrix multiply c * r, yielding a Matrix whose number of rows is the number of components in c and whose number of columns is the number of components in r.
        /// </summary>
        public static cMatrix4 OuterProduct(cVector4 c, cVector4 r) => cVector4.OuterProduct(c, r);
        
        /// <summary>
        /// Returns a cVector4 from component-wise application of Add (lhs + rhs).
        /// </summary>
        public static cVector4 Add(cVector4 lhs, cVector4 rhs) => cVector4.Add(lhs, rhs);
        
        /// <summary>
        /// Returns a cVector4 from component-wise application of Sub (lhs - rhs).
        /// </summary>
        public static cVector4 Sub(cVector4 lhs, cVector4 rhs) => cVector4.Sub(lhs, rhs);
        
        /// <summary>
        /// Returns a cVector4 from component-wise application of Mul (lhs * rhs).
        /// </summary>
        public static cVector4 Mul(cVector4 lhs, cVector4 rhs) => cVector4.Mul(lhs, rhs);
        
        /// <summary>
        /// Returns a cVector4 from component-wise application of Div (lhs / rhs).
        /// </summary>
        public static cVector4 Div(cVector4 lhs, cVector4 rhs) => cVector4.Div(lhs, rhs);
        
        /// <summary>
        /// Returns the euclidean length of this Vectortor.
        /// </summary>
        public static double Length(cVector4 v) => v.Length;
        
        /// <summary>
        /// Returns the squared euclidean length of this Vectortor.
        /// </summary>
        public static double LengthSqr(cVector4 v) => v.LengthSqr;
        
        /// <summary>
        /// Returns the sum of all components.
        /// </summary>
        public static Complex Sum(cVector4 v) => v.Sum;
        
        /// <summary>
        /// Returns the euclidean norm of this Vectortor.
        /// </summary>
        public static double Norm(cVector4 v) => v.Norm;
        
        /// <summary>
        /// Returns the one-norm of this Vectortor.
        /// </summary>
        public static double Norm1(cVector4 v) => v.Norm1;
        
        /// <summary>
        /// Returns the two-norm (euclidean length) of this Vectortor.
        /// </summary>
        public static double Norm2(cVector4 v) => v.Norm2;
        
        /// <summary>
        /// Returns the max-norm of this Vectortor.
        /// </summary>
        public static double NormMax(cVector4 v) => v.NormMax;
        
        /// <summary>
        /// Returns the p-norm of this Vectortor.
        /// </summary>
        public static double NormP(cVector4 v, double p) => v.NormP(p);
        
        /// <summary>
        /// Returns a copy of this Vectortor with length one (undefined if this has zero length).
        /// </summary>
        public static cVector4 Normalized(cVector4 v) => v.Normalized;
        
        /// <summary>
        /// Returns a copy of this Vectortor with length one (returns zero if length is zero).
        /// </summary>
        public static cVector4 NormalizedSafe(cVector4 v) => v.NormalizedSafe;
        
        /// <summary>
        /// Returns a Vectortor containing component-wise magnitudes.
        /// </summary>
        public static dVector4 Magnitude(cVector4 v) => v.Magnitude;
        
        /// <summary>
        /// Returns a Vectortor containing component-wise phases.
        /// </summary>
        public static dVector4 Phase(cVector4 v) => v.Phase;
        
        /// <summary>
        /// Returns a Vectortor containing component-wise imaginary parts.
        /// </summary>
        public static dVector4 Imaginary(cVector4 v) => v.Imaginary;
        
        /// <summary>
        /// Returns a Vectortor containing component-wise real parts.
        /// </summary>
        public static dVector4 Real(cVector4 v) => v.Real;
        
        /// <summary>
        /// Returns the inner product (dot product, scalar product) of the two Vectortors.
        /// </summary>
        public static Complex Dot(cVector4 lhs, cVector4 rhs) => cVector4.Dot(lhs, rhs);
        
        /// <summary>
        /// Returns the euclidean distance between the two Vectortors.
        /// </summary>
        public static double Distance(cVector4 lhs, cVector4 rhs) => cVector4.Distance(lhs, rhs);
        
        /// <summary>
        /// Returns the squared euclidean distance between the two Vectortors.
        /// </summary>
        public static double DistanceSqr(cVector4 lhs, cVector4 rhs) => cVector4.DistanceSqr(lhs, rhs);
        
        /// <summary>
        /// Calculate the reflection direction for an incident Vectortor (N should be normalized in order to achieve the desired result).
        /// </summary>
        public static cVector4 Reflect(cVector4 I, cVector4 N) => cVector4.Reflect(I, N);

    }
    */
}
