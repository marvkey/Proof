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
    /*
    /// <summary>
    /// Static class that contains static glm functions
    /// </summary>
    public static partial class glm
    {
        
        /// <summary>
        /// Returns an object that can be used for arbitrary swizzling (e.g. swizzle.zy)
        /// </summary>
        public static swizzle_cVector3 swizzle(cVector3 v) => v.swizzle;
        
        /// <summary>
        /// Returns an array with all values
        /// </summary>
        public static Complex[] Values(cVector3 v) => v.Values;
        
        /// <summary>
        /// Returns an enumerator that iterates through all components.
        /// </summary>
        public static IEnumerator<Complex> GetEnumerator(cVector3 v) => v.GetEnumerator();
        
        /// <summary>
        /// Returns a string representation of this Vector using ', ' as a seperator.
        /// </summary>
        public static string ToString(cVector3 v) => v.ToString();
        
        /// <summary>
        /// Returns a string representation of this Vector using a provided seperator.
        /// </summary>
        public static string ToString(cVector3 v, string sep) => v.ToString(sep);
        
        /// <summary>
        /// Returns a string representation of this Vector using a provided seperator and a forMatrix provider for each component.
        /// </summary>
        public static string ToString(cVector3 v, string sep, IFormatProvider provider) => v.ToString(sep, provider);
        
        /// <summary>
        /// Returns a string representation of this Vector using a provided seperator and a forMatrix for each component.
        /// </summary>
        public static string ToString(cVector3 v, string sep, string forMatrix) => v.ToString(sep, forMatrix);
        
        /// <summary>
        /// Returns a string representation of this Vector using a provided seperator and a forMatrix and forMatrix provider for each component.
        /// </summary>
        public static string ToString(cVector3 v, string sep, string forMatrix, IFormatProvider provider) => v.ToString(sep, forMatrix, provider);
        
        /// <summary>
        /// Returns the number of components (3).
        /// </summary>
        public static int Count(cVector3 v) => v.Count;
        
        /// <summary>
        /// Returns true iff this equals rhs component-wise.
        /// </summary>
        public static bool Equals(cVector3 v, cVector3 rhs) => v.Equals(rhs);
        
        /// <summary>
        /// Returns true iff this equals rhs type- and component-wise.
        /// </summary>
        public static bool Equals(cVector3 v, object obj) => v.Equals(obj);
        
        /// <summary>
        /// Returns a hash code for this instance.
        /// </summary>
        public static int GetHashCode(cVector3 v) => v.GetHashCode();
        
        /// <summary>
        /// Returns true iff distance between lhs and rhs is less than or equal to epsilon
        /// </summary>
        public static bool ApproxEqual(cVector3 lhs, cVector3 rhs, double eps = 0.1d) => cVector3.ApproxEqual(lhs, rhs, eps);
        
        /// <summary>
        /// Returns a bVector3 from component-wise application of Equal (lhs == rhs).
        /// </summary>
        public static bVector3 Equal(cVector3 lhs, cVector3 rhs) => cVector3.Equal(lhs, rhs);
        
        /// <summary>
        /// Returns a bVector3 from component-wise application of Equal (lhs == rhs).
        /// </summary>
        public static bool Equal(Complex lhs, Complex rhs) => lhs == rhs;
        
        /// <summary>
        /// Returns a bVector3 from component-wise application of NotEqual (lhs != rhs).
        /// </summary>
        public static bVector3 NotEqual(cVector3 lhs, cVector3 rhs) => cVector3.NotEqual(lhs, rhs);
        
        /// <summary>
        /// Returns a bVector3 from component-wise application of NotEqual (lhs != rhs).
        /// </summary>
        public static bool NotEqual(Complex lhs, Complex rhs) => lhs != rhs;
        
        /// <summary>
        /// Returns a dVector3 from component-wise application of Abs (v.Magnitude).
        /// </summary>
        public static dVector3 Abs(cVector3 v) => cVector3.Abs(v);
        
        /// <summary>
        /// Returns a dVector3 from component-wise application of Abs (v.Magnitude).
        /// </summary>
        public static double Abs(Complex v) => v.Magnitude;
        
        /// <summary>
        /// Returns a cVector3 from component-wise application of HermiteInterpolationOrder3 ((3 - 2 * v) * v * v).
        /// </summary>
        public static cVector3 HermiteInterpolationOrder3(cVector3 v) => cVector3.HermiteInterpolationOrder3(v);
        
        /// <summary>
        /// Returns a cVector3 from component-wise application of HermiteInterpolationOrder3 ((3 - 2 * v) * v * v).
        /// </summary>
        public static Complex HermiteInterpolationOrder3(Complex v) => (3 - 2 * v) * v * v;
        
        /// <summary>
        /// Returns a cVector3 from component-wise application of HermiteInterpolationOrder5 (((6 * v - 15) * v + 10) * v * v * v).
        /// </summary>
        public static cVector3 HermiteInterpolationOrder5(cVector3 v) => cVector3.HermiteInterpolationOrder5(v);
        
        /// <summary>
        /// Returns a cVector3 from component-wise application of HermiteInterpolationOrder5 (((6 * v - 15) * v + 10) * v * v * v).
        /// </summary>
        public static Complex HermiteInterpolationOrder5(Complex v) => ((6 * v - 15) * v + 10) * v * v * v;
        
        /// <summary>
        /// Returns a cVector3 from component-wise application of Sqr (v * v).
        /// </summary>
        public static cVector3 Sqr(cVector3 v) => cVector3.Sqr(v);
        
        /// <summary>
        /// Returns a cVector3 from component-wise application of Sqr (v * v).
        /// </summary>
        public static Complex Sqr(Complex v) => v * v;
        
        /// <summary>
        /// Returns a cVector3 from component-wise application of Pow2 (v * v).
        /// </summary>
        public static cVector3 Pow2(cVector3 v) => cVector3.Pow2(v);
        
        /// <summary>
        /// Returns a cVector3 from component-wise application of Pow2 (v * v).
        /// </summary>
        public static Complex Pow2(Complex v) => v * v;
        
        /// <summary>
        /// Returns a cVector3 from component-wise application of Pow3 (v * v * v).
        /// </summary>
        public static cVector3 Pow3(cVector3 v) => cVector3.Pow3(v);
        
        /// <summary>
        /// Returns a cVector3 from component-wise application of Pow3 (v * v * v).
        /// </summary>
        public static Complex Pow3(Complex v) => v * v * v;
        
        /// <summary>
        /// Returns a cVector3 from component-wise application of Acos (Complex.Acos(v)).
        /// </summary>
        public static cVector3 Acos(cVector3 v) => cVector3.Acos(v);
        
        /// <summary>
        /// Returns a cVector3 from component-wise application of Acos (Complex.Acos(v)).
        /// </summary>
        public static Complex Acos(Complex v) => Complex.Acos(v);
        
        /// <summary>
        /// Returns a cVector3 from component-wise application of Asin (Complex.Asin(v)).
        /// </summary>
        public static cVector3 Asin(cVector3 v) => cVector3.Asin(v);
        
        /// <summary>
        /// Returns a cVector3 from component-wise application of Asin (Complex.Asin(v)).
        /// </summary>
        public static Complex Asin(Complex v) => Complex.Asin(v);
        
        /// <summary>
        /// Returns a cVector3 from component-wise application of Atan (Complex.Atan(v)).
        /// </summary>
        public static cVector3 Atan(cVector3 v) => cVector3.Atan(v);
        
        /// <summary>
        /// Returns a cVector3 from component-wise application of Atan (Complex.Atan(v)).
        /// </summary>
        public static Complex Atan(Complex v) => Complex.Atan(v);
        
        /// <summary>
        /// Returns a cVector3 from component-wise application of Cos (Complex.Cos(v)).
        /// </summary>
        public static cVector3 Cos(cVector3 v) => cVector3.Cos(v);
        
        /// <summary>
        /// Returns a cVector3 from component-wise application of Cos (Complex.Cos(v)).
        /// </summary>
        public static Complex Cos(Complex v) => Complex.Cos(v);
        
        /// <summary>
        /// Returns a cVector3 from component-wise application of Cosh (Complex.Cosh(v)).
        /// </summary>
        public static cVector3 Cosh(cVector3 v) => cVector3.Cosh(v);
        
        /// <summary>
        /// Returns a cVector3 from component-wise application of Cosh (Complex.Cosh(v)).
        /// </summary>
        public static Complex Cosh(Complex v) => Complex.Cosh(v);
        
        /// <summary>
        /// Returns a cVector3 from component-wise application of Exp (Complex.Exp(v)).
        /// </summary>
        public static cVector3 Exp(cVector3 v) => cVector3.Exp(v);
        
        /// <summary>
        /// Returns a cVector3 from component-wise application of Exp (Complex.Exp(v)).
        /// </summary>
        public static Complex Exp(Complex v) => Complex.Exp(v);
        
        /// <summary>
        /// Returns a cVector3 from component-wise application of Log (Complex.Log(v)).
        /// </summary>
        public static cVector3 Log(cVector3 v) => cVector3.Log(v);
        
        /// <summary>
        /// Returns a cVector3 from component-wise application of Log (Complex.Log(v)).
        /// </summary>
        public static Complex Log(Complex v) => Complex.Log(v);
        
        /// <summary>
        /// Returns a cVector3 from component-wise application of Log2 (Complex.Log(v, 2.0)).
        /// </summary>
        public static cVector3 Log2(cVector3 v) => cVector3.Log2(v);
        
        /// <summary>
        /// Returns a cVector3 from component-wise application of Log2 (Complex.Log(v, 2.0)).
        /// </summary>
        public static Complex Log2(Complex v) => Complex.Log(v, 2.0);
        
        /// <summary>
        /// Returns a cVector3 from component-wise application of Log10 (Complex.Log10(v)).
        /// </summary>
        public static cVector3 Log10(cVector3 v) => cVector3.Log10(v);
        
        /// <summary>
        /// Returns a cVector3 from component-wise application of Log10 (Complex.Log10(v)).
        /// </summary>
        public static Complex Log10(Complex v) => Complex.Log10(v);
        
        /// <summary>
        /// Returns a cVector3 from component-wise application of Reciprocal (Complex.Reciprocal(v)).
        /// </summary>
        public static cVector3 Reciprocal(cVector3 v) => cVector3.Reciprocal(v);
        
        /// <summary>
        /// Returns a cVector3 from component-wise application of Reciprocal (Complex.Reciprocal(v)).
        /// </summary>
        public static Complex Reciprocal(Complex v) => Complex.Reciprocal(v);
        
        /// <summary>
        /// Returns a cVector3 from component-wise application of Sin (Complex.Sin(v)).
        /// </summary>
        public static cVector3 Sin(cVector3 v) => cVector3.Sin(v);
        
        /// <summary>
        /// Returns a cVector3 from component-wise application of Sin (Complex.Sin(v)).
        /// </summary>
        public static Complex Sin(Complex v) => Complex.Sin(v);
        
        /// <summary>
        /// Returns a cVector3 from component-wise application of Sinh (Complex.Sinh(v)).
        /// </summary>
        public static cVector3 Sinh(cVector3 v) => cVector3.Sinh(v);
        
        /// <summary>
        /// Returns a cVector3 from component-wise application of Sinh (Complex.Sinh(v)).
        /// </summary>
        public static Complex Sinh(Complex v) => Complex.Sinh(v);
        
        /// <summary>
        /// Returns a cVector3 from component-wise application of Sqrt (Complex.Sqrt(v)).
        /// </summary>
        public static cVector3 Sqrt(cVector3 v) => cVector3.Sqrt(v);
        
        /// <summary>
        /// Returns a cVector3 from component-wise application of Sqrt (Complex.Sqrt(v)).
        /// </summary>
        public static Complex Sqrt(Complex v) => Complex.Sqrt(v);
        
        /// <summary>
        /// Returns a cVector3 from component-wise application of InverseSqrt (Complex.One / Complex.Sqrt(v)).
        /// </summary>
        public static cVector3 InverseSqrt(cVector3 v) => cVector3.InverseSqrt(v);
        
        /// <summary>
        /// Returns a cVector3 from component-wise application of InverseSqrt (Complex.One / Complex.Sqrt(v)).
        /// </summary>
        public static Complex InverseSqrt(Complex v) => Complex.One / Complex.Sqrt(v);
        
        /// <summary>
        /// Returns a cVector3 from component-wise application of Tan (Complex.Tan(v)).
        /// </summary>
        public static cVector3 Tan(cVector3 v) => cVector3.Tan(v);
        
        /// <summary>
        /// Returns a cVector3 from component-wise application of Tan (Complex.Tan(v)).
        /// </summary>
        public static Complex Tan(Complex v) => Complex.Tan(v);
        
        /// <summary>
        /// Returns a cVector3 from component-wise application of Tanh (Complex.Tanh(v)).
        /// </summary>
        public static cVector3 Tanh(cVector3 v) => cVector3.Tanh(v);
        
        /// <summary>
        /// Returns a cVector3 from component-wise application of Tanh (Complex.Tanh(v)).
        /// </summary>
        public static Complex Tanh(Complex v) => Complex.Tanh(v);
        
        /// <summary>
        /// Returns a cVector3 from component-wise application of Conjugate (Complex.Conjugate(v)).
        /// </summary>
        public static cVector3 Conjugate(cVector3 v) => cVector3.Conjugate(v);
        
        /// <summary>
        /// Returns a cVector3 from component-wise application of Conjugate (Complex.Conjugate(v)).
        /// </summary>
        public static Complex Conjugate(Complex v) => Complex.Conjugate(v);
        
        /// <summary>
        /// Returns a cVector3 from component-wise application of Pow (Complex.Pow(lhs, rhs)).
        /// </summary>
        public static cVector3 Pow(cVector3 lhs, cVector3 rhs) => cVector3.Pow(lhs, rhs);
        
        /// <summary>
        /// Returns a cVector3 from component-wise application of Pow (Complex.Pow(lhs, rhs)).
        /// </summary>
        public static Complex Pow(Complex lhs, Complex rhs) => Complex.Pow(lhs, rhs);
        
        /// <summary>
        /// Returns a cVector3 from component-wise application of Log (Complex.Log(lhs, rhs)).
        /// </summary>
        public static cVector3 Log(cVector3 lhs, dVector3 rhs) => cVector3.Log(lhs, rhs);
        
        /// <summary>
        /// Returns a cVector3 from component-wise application of Log (Complex.Log(lhs, rhs)).
        /// </summary>
        public static Complex Log(Complex lhs, double rhs) => Complex.Log(lhs, rhs);
        
        /// <summary>
        /// Returns a cVector3 from component-wise application of FromPolarCoordinates (Complex.FromPolarCoordinates(magnitude, phase)).
        /// </summary>
        public static cVector3 FromPolarCoordinates(dVector3 magnitude, dVector3 phase) => cVector3.FromPolarCoordinates(magnitude, phase);
        
        /// <summary>
        /// Returns a cVector3 from component-wise application of FromPolarCoordinates (Complex.FromPolarCoordinates(magnitude, phase)).
        /// </summary>
        public static Complex FromPolarCoordinates(double magnitude, double phase) => Complex.FromPolarCoordinates(magnitude, phase);
        
        /// <summary>
        /// Returns a cVector3 from component-wise application of Fma (a * b + c).
        /// </summary>
        public static cVector3 Fma(cVector3 a, cVector3 b, cVector3 c) => cVector3.Fma(a, b, c);
        
        /// <summary>
        /// Returns a cVector3 from component-wise application of Fma (a * b + c).
        /// </summary>
        public static Complex Fma(Complex a, Complex b, Complex c) => a * b + c;
        
        /// <summary>
        /// OuterProduct treats the first parameter c as a column Vector (Matrix with one column) and the second parameter r as a row Vector (Matrix with one row) and does a linear algebraic Matrix multiply c * r, yielding a Matrix whose number of rows is the number of components in c and whose number of columns is the number of components in r.
        /// </summary>
        public static cMatrix2x3 OuterProduct(cVector3 c, cVector2 r) => cVector3.OuterProduct(c, r);
        
        /// <summary>
        /// OuterProduct treats the first parameter c as a column Vector (Matrix with one column) and the second parameter r as a row Vector (Matrix with one row) and does a linear algebraic Matrix multiply c * r, yielding a Matrix whose number of rows is the number of components in c and whose number of columns is the number of components in r.
        /// </summary>
        public static cMatrix3 OuterProduct(cVector3 c, cVector3 r) => cVector3.OuterProduct(c, r);
        
        /// <summary>
        /// OuterProduct treats the first parameter c as a column Vector (Matrix with one column) and the second parameter r as a row Vector (Matrix with one row) and does a linear algebraic Matrix multiply c * r, yielding a Matrix whose number of rows is the number of components in c and whose number of columns is the number of components in r.
        /// </summary>
        public static cMatrix4x3 OuterProduct(cVector3 c, cVector4 r) => cVector3.OuterProduct(c, r);
        
        /// <summary>
        /// Returns a cVector3 from component-wise application of Add (lhs + rhs).
        /// </summary>
        public static cVector3 Add(cVector3 lhs, cVector3 rhs) => cVector3.Add(lhs, rhs);
        
        /// <summary>
        /// Returns a cVector3 from component-wise application of Add (lhs + rhs).
        /// </summary>
        public static Complex Add(Complex lhs, Complex rhs) => lhs + rhs;
        
        /// <summary>
        /// Returns a cVector3 from component-wise application of Sub (lhs - rhs).
        /// </summary>
        public static cVector3 Sub(cVector3 lhs, cVector3 rhs) => cVector3.Sub(lhs, rhs);
        
        /// <summary>
        /// Returns a cVector3 from component-wise application of Sub (lhs - rhs).
        /// </summary>
        public static Complex Sub(Complex lhs, Complex rhs) => lhs - rhs;
        
        /// <summary>
        /// Returns a cVector3 from component-wise application of Mul (lhs * rhs).
        /// </summary>
        public static cVector3 Mul(cVector3 lhs, cVector3 rhs) => cVector3.Mul(lhs, rhs);
        
        /// <summary>
        /// Returns a cVector3 from component-wise application of Mul (lhs * rhs).
        /// </summary>
        public static Complex Mul(Complex lhs, Complex rhs) => lhs * rhs;
        
        /// <summary>
        /// Returns a cVector3 from component-wise application of Div (lhs / rhs).
        /// </summary>
        public static cVector3 Div(cVector3 lhs, cVector3 rhs) => cVector3.Div(lhs, rhs);
        
        /// <summary>
        /// Returns a cVector3 from component-wise application of Div (lhs / rhs).
        /// </summary>
        public static Complex Div(Complex lhs, Complex rhs) => lhs / rhs;
        
        /// <summary>
        /// Returns the euclidean length of this Vector.
        /// </summary>
        public static double Length(cVector3 v) => v.Length;
        
        /// <summary>
        /// Returns the squared euclidean length of this Vector.
        /// </summary>
        public static double LengthSqr(cVector3 v) => v.LengthSqr;
        
        /// <summary>
        /// Returns the sum of all components.
        /// </summary>
        public static Complex Sum(cVector3 v) => v.Sum;
        
        /// <summary>
        /// Returns the euclidean norm of this Vector.
        /// </summary>
        public static double Norm(cVector3 v) => v.Norm;
        
        /// <summary>
        /// Returns the one-norm of this Vector.
        /// </summary>
        public static double Norm1(cVector3 v) => v.Norm1;
        
        /// <summary>
        /// Returns the two-norm (euclidean length) of this Vector.
        /// </summary>
        public static double Norm2(cVector3 v) => v.Norm2;
        
        /// <summary>
        /// Returns the max-norm of this Vector.
        /// </summary>
        public static double NormMax(cVector3 v) => v.NormMax;
        
        /// <summary>
        /// Returns the p-norm of this Vector.
        /// </summary>
        public static double NormP(cVector3 v, double p) => v.NormP(p);
        
        /// <summary>
        /// Returns a copy of this Vector with length one (undefined if this has zero length).
        /// </summary>
        public static cVector3 Normalized(cVector3 v) => v.Normalized;
        
        /// <summary>
        /// Returns a copy of this Vector with length one (returns zero if length is zero).
        /// </summary>
        public static cVector3 NormalizedSafe(cVector3 v) => v.NormalizedSafe;
        
        /// <summary>
        /// Returns a Vector containing component-wise magnitudes.
        /// </summary>
        public static dVector3 Magnitude(cVector3 v) => v.Magnitude;
        
        /// <summary>
        /// Returns a Vector containing component-wise phases.
        /// </summary>
        public static dVector3 Phase(cVector3 v) => v.Phase;
        
        /// <summary>
        /// Returns a Vector containing component-wise imaginary parts.
        /// </summary>
        public static dVector3 Imaginary(cVector3 v) => v.Imaginary;
        
        /// <summary>
        /// Returns a Vector containing component-wise real parts.
        /// </summary>
        public static dVector3 Real(cVector3 v) => v.Real;
        
        /// <summary>
        /// Returns the inner product (dot product, scalar product) of the two Vectors.
        /// </summary>
        public static Complex Dot(cVector3 lhs, cVector3 rhs) => cVector3.Dot(lhs, rhs);
        
        /// <summary>
        /// Returns the euclidean distance between the two Vectors.
        /// </summary>
        public static double Distance(cVector3 lhs, cVector3 rhs) => cVector3.Distance(lhs, rhs);
        
        /// <summary>
        /// Returns the squared euclidean distance between the two Vectors.
        /// </summary>
        public static double DistanceSqr(cVector3 lhs, cVector3 rhs) => cVector3.DistanceSqr(lhs, rhs);
        
        /// <summary>
        /// Calculate the reflection direction for an incident Vector (N should be normalized in order to achieve the desired result).
        /// </summary>
        public static cVector3 Reflect(cVector3 I, cVector3 N) => cVector3.Reflect(I, N);
        
        /// <summary>
        /// Returns the outer product (cross product, Vector product) of the two Vectors.
        /// </summary>
        public static cVector3 Cross(cVector3 l, cVector3 r) => cVector3.Cross(l, r);

    }
    */
}
