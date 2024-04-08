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
    /// A Vector of type Complex with 2 components.
    /// </summary>
    /// 
    /*
    [RegisterCoreClassStruct]
    
    [StructLayout(LayoutKind.Sequential)]
    public struct cVector2 : IReadOnlyList<Complex>, IEquatable<cVector2>
    {

        #region Fields
        
        /// <summary>
        /// x-component
        /// </summary>
        
        public Complex x;
        
        /// <summary>
        /// y-component
        /// </summary>
        
        public Complex y;

        #endregion


        #region Constructors
        
        /// <summary>
        /// Component-wise constructor
        /// </summary>
        public cVector2(Complex x, Complex y)
        {
            this.x = x;
            this.y = y;
        }
        
        /// <summary>
        /// all-same-value constructor
        /// </summary>
        public cVector2(Complex v)
        {
            this.x = v;
            this.y = v;
        }
        
        /// <summary>
        /// from-Vector constructor
        /// </summary>
        public cVector2(cVector2 v)
        {
            this.x = v.x;
            this.y = v.y;
        }
        
        /// <summary>
        /// from-Vector constructor (additional fields are truncated)
        /// </summary>
        public cVector2(cVector3 v)
        {
            this.x = v.x;
            this.y = v.y;
        }
        
        /// <summary>
        /// from-Vector constructor (additional fields are truncated)
        /// </summary>
        public cVector2(cVector4 v)
        {
            this.x = v.x;
            this.y = v.y;
        }
        
        /// <summary>
        /// From-array/list constructor (superfluous values are ignored, missing values are zero-filled).
        /// </summary>
        public cVector2(IReadOnlyList<Complex> v)
        {
            var c = v.Count;
            this.x = c < 0 ? Complex.Zero : v[0];
            this.y = c < 1 ? Complex.Zero : v[1];
        }
        
        /// <summary>
        /// Generic from-array constructor (superfluous values are ignored, missing values are zero-filled).
        /// </summary>
        public cVector2(Object[] v)
        {
            var c = v.Length;
            this.x = c < 0 ? Complex.Zero : (Complex)v[0];
            this.y = c < 1 ? Complex.Zero : (Complex)v[1];
        }
        
        /// <summary>
        /// From-array constructor (superfluous values are ignored, missing values are zero-filled).
        /// </summary>
        public cVector2(Complex[] v)
        {
            var c = v.Length;
            this.x = c < 0 ? Complex.Zero : v[0];
            this.y = c < 1 ? Complex.Zero : v[1];
        }
        
        /// <summary>
        /// From-array constructor with base index (superfluous values are ignored, missing values are zero-filled).
        /// </summary>
        public cVector2(Complex[] v, int startIndex)
        {
            var c = v.Length;
            this.x = c + startIndex < 0 ? Complex.Zero : v[0 + startIndex];
            this.y = c + startIndex < 1 ? Complex.Zero : v[1 + startIndex];
        }
        
        /// <summary>
        /// From-IEnumerable constructor (superfluous values are ignored, missing values are zero-filled).
        /// </summary>
        public cVector2(IEnumerable<Complex> v)
            : this(v.ToArray())
        {
        }

        #endregion


        #region Explicit Operators
        
        /// <summary>
        /// Explicitly converts this to a cVector3. (Higher components are zeroed)
        /// </summary>
        public static explicit operator cVector3(cVector2 v) => new cVector3((Complex)v.x, (Complex)v.y, Complex.Zero);
        
        /// <summary>
        /// Explicitly converts this to a cVector4. (Higher components are zeroed)
        /// </summary>
        public static explicit operator cVector4(cVector2 v) => new cVector4((Complex)v.x, (Complex)v.y, Complex.Zero, Complex.Zero);
        
        /// <summary>
        /// Explicitly converts this to a Complex array.
        /// </summary>
        public static explicit operator Complex[](cVector2 v) => new [] { v.x, v.y };
        
        /// <summary>
        /// Explicitly converts this to a generic object array.
        /// </summary>
        public static explicit operator Object[](cVector2 v) => new Object[] { v.x, v.y };

        #endregion


        #region Indexer
        
        /// <summary>
        /// Gets/Sets a specific indexed component (a bit slower than direct access).
        /// </summary>
        public Complex this[int index]
        {
            get
            {
                switch (index)
                {
                    case 0: return x;
                    case 1: return y;
                    default: throw new ArgumentOutOfRangeException("index");
                }
            }
            set
            {
                switch (index)
                {
                    case 0: x = value; break;
                    case 1: y = value; break;
                    default: throw new ArgumentOutOfRangeException("index");
                }
            }
        }

        #endregion


        #region Properties
        
        /// <summary>
        /// Returns an object that can be used for arbitrary swizzling (e.g. swizzle.zy)
        /// </summary>
        public swizzle_cVector2 swizzle => new swizzle_cVector2(x, y);
        
        /// <summary>
        /// Gets or sets the specified subset of components. For more advanced (read-only) swizzling, use the .swizzle property.
        /// </summary>
        public cVector2 xy
        {
            get
            {
                return new cVector2(x, y);
            }
            set
            {
                x = value.x;
                y = value.y;
            }
        }
        
        /// <summary>
        /// Gets or sets the specified subset of components. For more advanced (read-only) swizzling, use the .swizzle property.
        /// </summary>
        public cVector2 rg
        {
            get
            {
                return new cVector2(x, y);
            }
            set
            {
                x = value.x;
                y = value.y;
            }
        }
        
        /// <summary>
        /// Gets or sets the specified RGBA component. For more advanced (read-only) swizzling, use the .swizzle property.
        /// </summary>
        public Complex r
        {
            get
            {
                return x;
            }
            set
            {
                x = value;
            }
        }
        
        /// <summary>
        /// Gets or sets the specified RGBA component. For more advanced (read-only) swizzling, use the .swizzle property.
        /// </summary>
        public Complex g
        {
            get
            {
                return y;
            }
            set
            {
                y = value;
            }
        }
        
        /// <summary>
        /// Returns an array with all values
        /// </summary>
        public Complex[] Values => new[] { x, y };
        
        /// <summary>
        /// Returns the number of components (2).
        /// </summary>
        public int Count => 2;
        
        /// <summary>
        /// Returns the euclidean length of this Vector.
        /// </summary>
        public double Length => (double)Math.Sqrt((x.LengthSqr() + y.LengthSqr()));
        
        /// <summary>
        /// Returns the squared euclidean length of this Vector.
        /// </summary>
        public double LengthSqr => (x.LengthSqr() + y.LengthSqr());
        
        /// <summary>
        /// Returns the sum of all components.
        /// </summary>
        public Complex Sum => (x + y);
        
        /// <summary>
        /// Returns the euclidean norm of this Vector.
        /// </summary>
        public double Norm => (double)Math.Sqrt((x.LengthSqr() + y.LengthSqr()));
        
        /// <summary>
        /// Returns the one-norm of this Vector.
        /// </summary>
        public double Norm1 => (x.Magnitude + y.Magnitude);
        
        /// <summary>
        /// Returns the two-norm (euclidean length) of this Vector.
        /// </summary>
        public double Norm2 => (double)Math.Sqrt((x.LengthSqr() + y.LengthSqr()));
        
        /// <summary>
        /// Returns the max-norm of this Vector.
        /// </summary>
        public double NormMax => Math.Max(x.Magnitude, y.Magnitude);
        
        /// <summary>
        /// Returns a copy of this Vector with length one (undefined if this has zero length).
        /// </summary>
        public cVector2 Normalized => this / (Complex)Length;
        
        /// <summary>
        /// Returns a copy of this Vector with length one (returns zero if length is zero).
        /// </summary>
        public cVector2 NormalizedSafe => this == Zero ? Zero : this / (Complex)Length;
        
        /// <summary>
        /// Returns a Vector containing component-wise magnitudes.
        /// </summary>
        public dVector2 Magnitude => new dVector2(x.Magnitude, y.Magnitude);
        
        /// <summary>
        /// Returns a Vector containing component-wise phases.
        /// </summary>
        public dVector2 Phase => new dVector2(x.Phase, y.Phase);
        
        /// <summary>
        /// Returns a Vector containing component-wise imaginary parts.
        /// </summary>
        public dVector2 Imaginary => new dVector2(x.Imaginary, y.Imaginary);
        
        /// <summary>
        /// Returns a Vector containing component-wise real parts.
        /// </summary>
        public dVector2 Real => new dVector2(x.Real, y.Real);

        #endregion


        #region Static Properties
        
        /// <summary>
        /// Predefined all-zero Vector
        /// </summary>
        public static cVector2 Zero { get; } = new cVector2(Complex.Zero, Complex.Zero);
        
        /// <summary>
        /// Predefined all-ones Vector
        /// </summary>
        public static cVector2 Ones { get; } = new cVector2(Complex.One, Complex.One);
        
        /// <summary>
        /// Predefined unit-X Vector
        /// </summary>
        public static cVector2 UnitX { get; } = new cVector2(Complex.One, Complex.Zero);
        
        /// <summary>
        /// Predefined unit-Y Vector
        /// </summary>
        public static cVector2 UnitY { get; } = new cVector2(Complex.Zero, Complex.One);
        
        /// <summary>
        /// Predefined all-imaginary-ones Vector
        /// </summary>
        public static cVector2 ImaginaryOnes { get; } = new cVector2(Complex.ImaginaryOne, Complex.ImaginaryOne);
        
        /// <summary>
        /// Predefined unit-imaginary-X Vector
        /// </summary>
        public static cVector2 ImaginaryUnitX { get; } = new cVector2(Complex.ImaginaryOne, Complex.Zero);
        
        /// <summary>
        /// Predefined unit-imaginary-Y Vector
        /// </summary>
        public static cVector2 ImaginaryUnitY { get; } = new cVector2(Complex.Zero, Complex.ImaginaryOne);

        #endregion


        #region Operators
        
        /// <summary>
        /// Returns true iff this equals rhs component-wise.
        /// </summary>
        public static bool operator==(cVector2 lhs, cVector2 rhs) => lhs.Equals(rhs);
        
        /// <summary>
        /// Returns true iff this does not equal rhs (component-wise).
        /// </summary>
        public static bool operator!=(cVector2 lhs, cVector2 rhs) => !lhs.Equals(rhs);

        #endregion


        #region Functions
        
        /// <summary>
        /// Returns an enumerator that iterates through all components.
        /// </summary>
        public IEnumerator<Complex> GetEnumerator()
        {
            yield return x;
            yield return y;
        }
        
        /// <summary>
        /// Returns an enumerator that iterates through all components.
        /// </summary>
        IEnumerator IEnumerable.GetEnumerator() => GetEnumerator();
        
        /// <summary>
        /// Returns a string representation of this Vector using ', ' as a seperator.
        /// </summary>
        public override string ToString() => ToString(", ");
        
        /// <summary>
        /// Returns a string representation of this Vector using a provided seperator.
        /// </summary>
        public string ToString(string sep) => (x + sep + y);
        
        /// <summary>
        /// Returns a string representation of this Vector using a provided seperator and a forMatrix provider for each component.
        /// </summary>
        public string ToString(string sep, IFormatProvider provider) => (x.ToString(provider) + sep + y.ToString(provider));
        
        /// <summary>
        /// Returns a string representation of this Vector using a provided seperator and a forMatrix for each component.
        /// </summary>
        public string ToString(string sep, string forMatrix) => (x.ToString(forMatrix) + sep + y.ToString(forMatrix));
        
        /// <summary>
        /// Returns a string representation of this Vector using a provided seperator and a forMatrix and forMatrix provider for each component.
        /// </summary>
        public string ToString(string sep, string forMatrix, IFormatProvider provider) => (x.ToString(forMatrix, provider) + sep + y.ToString(forMatrix, provider));
        
        /// <summary>
        /// Returns true iff this equals rhs component-wise.
        /// </summary>
        public bool Equals(cVector2 rhs) => (x.Equals(rhs.x) && y.Equals(rhs.y));
        
        /// <summary>
        /// Returns true iff this equals rhs type- and component-wise.
        /// </summary>
        public override bool Equals(object obj)
        {
            if (ReferenceEquals(null, obj)) return false;
            return obj is cVector2 && Equals((cVector2) obj);
        }
        
        /// <summary>
        /// Returns a hash code for this instance.
        /// </summary>
        public override int GetHashCode()
        {
            unchecked
            {
                return ((x.GetHashCode()) * 397) ^ y.GetHashCode();
            }
        }
        
        /// <summary>
        /// Returns the p-norm of this Vector.
        /// </summary>
        public double NormP(double p) => Math.Pow((Math.Pow((double)x.Magnitude, p) + Math.Pow((double)y.Magnitude, p)), 1 / p);

        #endregion


        #region Static Functions
        
        /// <summary>
        /// Returns true iff distance between lhs and rhs is less than or equal to epsilon
        /// </summary>
        public static bool ApproxEqual(cVector2 lhs, cVector2 rhs, double eps = 0.1d) => Distance(lhs, rhs) <= eps;
        
        /// <summary>
        /// OuterProduct treats the first parameter c as a column Vector (Matrix with one column) and the second parameter r as a row Vector (Matrix with one row) and does a linear algebraic Matrix multiply c * r, yielding a Matrix whose number of rows is the number of components in c and whose number of columns is the number of components in r.
        /// </summary>
        public static cMatrix2 OuterProduct(cVector2 c, cVector2 r) => new cMatrix2(c.x * r.x, c.y * r.x, c.x * r.y, c.y * r.y);
        
        /// <summary>
        /// OuterProduct treats the first parameter c as a column Vector (Matrix with one column) and the second parameter r as a row Vector (Matrix with one row) and does a linear algebraic Matrix multiply c * r, yielding a Matrix whose number of rows is the number of components in c and whose number of columns is the number of components in r.
        /// </summary>
        public static cMatrix2x3 OuterProduct(cVector3 c, cVector2 r) => new cMatrix2x3(c.x * r.x, c.y * r.x, c.z * r.x, c.x * r.y, c.y * r.y, c.z * r.y);
        
        /// <summary>
        /// OuterProduct treats the first parameter c as a column Vector (Matrix with one column) and the second parameter r as a row Vector (Matrix with one row) and does a linear algebraic Matrix multiply c * r, yielding a Matrix whose number of rows is the number of components in c and whose number of columns is the number of components in r.
        /// </summary>
        public static cMatrix3x2 OuterProduct(cVector2 c, cVector3 r) => new cMatrix3x2(c.x * r.x, c.y * r.x, c.x * r.y, c.y * r.y, c.x * r.z, c.y * r.z);
        
        /// <summary>
        /// OuterProduct treats the first parameter c as a column Vector (Matrix with one column) and the second parameter r as a row Vector (Matrix with one row) and does a linear algebraic Matrix multiply c * r, yielding a Matrix whose number of rows is the number of components in c and whose number of columns is the number of components in r.
        /// </summary>
        public static cMatrix2x4 OuterProduct(cVector4 c, cVector2 r) => new cMatrix2x4(c.x * r.x, c.y * r.x, c.z * r.x, c.w * r.x, c.x * r.y, c.y * r.y, c.z * r.y, c.w * r.y);
        
        /// <summary>
        /// OuterProduct treats the first parameter c as a column Vector (Matrix with one column) and the second parameter r as a row Vector (Matrix with one row) and does a linear algebraic Matrix multiply c * r, yielding a Matrix whose number of rows is the number of components in c and whose number of columns is the number of components in r.
        /// </summary>
        public static cMatrix4x2 OuterProduct(cVector2 c, cVector4 r) => new cMatrix4x2(c.x * r.x, c.y * r.x, c.x * r.y, c.y * r.y, c.x * r.z, c.y * r.z, c.x * r.w, c.y * r.w);
        
        /// <summary>
        /// Returns the inner product (dot product, scalar product) of the two Vectors.
        /// </summary>
        public static Complex Dot(cVector2 lhs, cVector2 rhs) => (lhs.x * Complex.Conjugate(rhs.x) + lhs.y * Complex.Conjugate(rhs.y));
        
        /// <summary>
        /// Returns the euclidean distance between the two Vectors.
        /// </summary>
        public static double Distance(cVector2 lhs, cVector2 rhs) => (lhs - rhs).Length;
        
        /// <summary>
        /// Returns the squared euclidean distance between the two Vectors.
        /// </summary>
        public static double DistanceSqr(cVector2 lhs, cVector2 rhs) => (lhs - rhs).LengthSqr;
        
        /// <summary>
        /// Calculate the reflection direction for an incident Vector (N should be normalized in order to achieve the desired result).
        /// </summary>
        public static cVector2 Reflect(cVector2 I, cVector2 N) => I - 2 * Dot(N, I) * N;
        
        /// <summary>
        /// Returns the length of the outer product (cross product, Vector product) of the two Vectors.
        /// </summary>
        public static Complex Cross(cVector2 l, cVector2 r) => l.x * r.y - l.y * r.x;

        #endregion


        #region Component-Wise Static Functions
        
        /// <summary>
        /// Returns a bVector2 from component-wise application of Equal (lhs == rhs).
        /// </summary>
        public static bVector2 Equal(cVector2 lhs, cVector2 rhs) => new bVector2(lhs.x == rhs.x, lhs.y == rhs.y);
        
        /// <summary>
        /// Returns a bVector2 from component-wise application of Equal (lhs == rhs).
        /// </summary>
        public static bVector2 Equal(cVector2 lhs, Complex rhs) => new bVector2(lhs.x == rhs, lhs.y == rhs);
        
        /// <summary>
        /// Returns a bVector2 from component-wise application of Equal (lhs == rhs).
        /// </summary>
        public static bVector2 Equal(Complex lhs, cVector2 rhs) => new bVector2(lhs == rhs.x, lhs == rhs.y);
        
        /// <summary>
        /// Returns a bVector from the application of Equal (lhs == rhs).
        /// </summary>
        public static bVector2 Equal(Complex lhs, Complex rhs) => new bVector2(lhs == rhs);
        
        /// <summary>
        /// Returns a bVector2 from component-wise application of NotEqual (lhs != rhs).
        /// </summary>
        public static bVector2 NotEqual(cVector2 lhs, cVector2 rhs) => new bVector2(lhs.x != rhs.x, lhs.y != rhs.y);
        
        /// <summary>
        /// Returns a bVector2 from component-wise application of NotEqual (lhs != rhs).
        /// </summary>
        public static bVector2 NotEqual(cVector2 lhs, Complex rhs) => new bVector2(lhs.x != rhs, lhs.y != rhs);
        
        /// <summary>
        /// Returns a bVector2 from component-wise application of NotEqual (lhs != rhs).
        /// </summary>
        public static bVector2 NotEqual(Complex lhs, cVector2 rhs) => new bVector2(lhs != rhs.x, lhs != rhs.y);
        
        /// <summary>
        /// Returns a bVector from the application of NotEqual (lhs != rhs).
        /// </summary>
        public static bVector2 NotEqual(Complex lhs, Complex rhs) => new bVector2(lhs != rhs);
        
        /// <summary>
        /// Returns a dVector2 from component-wise application of Abs (v.Magnitude).
        /// </summary>
        public static dVector2 Abs(cVector2 v) => new dVector2(v.x.Magnitude, v.y.Magnitude);
        
        /// <summary>
        /// Returns a dVector from the application of Abs (v.Magnitude).
        /// </summary>
        public static dVector2 Abs(Complex v) => new dVector2(v.Magnitude);
        
        /// <summary>
        /// Returns a cVector2 from component-wise application of HermiteInterpolationOrder3 ((3 - 2 * v) * v * v).
        /// </summary>
        public static cVector2 HermiteInterpolationOrder3(cVector2 v) => new cVector2((3 - 2 * v.x) * v.x * v.x, (3 - 2 * v.y) * v.y * v.y);
        
        /// <summary>
        /// Returns a cVector from the application of HermiteInterpolationOrder3 ((3 - 2 * v) * v * v).
        /// </summary>
        public static cVector2 HermiteInterpolationOrder3(Complex v) => new cVector2((3 - 2 * v) * v * v);
        
        /// <summary>
        /// Returns a cVector2 from component-wise application of HermiteInterpolationOrder5 (((6 * v - 15) * v + 10) * v * v * v).
        /// </summary>
        public static cVector2 HermiteInterpolationOrder5(cVector2 v) => new cVector2(((6 * v.x - 15) * v.x + 10) * v.x * v.x * v.x, ((6 * v.y - 15) * v.y + 10) * v.y * v.y * v.y);
        
        /// <summary>
        /// Returns a cVector from the application of HermiteInterpolationOrder5 (((6 * v - 15) * v + 10) * v * v * v).
        /// </summary>
        public static cVector2 HermiteInterpolationOrder5(Complex v) => new cVector2(((6 * v - 15) * v + 10) * v * v * v);
        
        /// <summary>
        /// Returns a cVector2 from component-wise application of Sqr (v * v).
        /// </summary>
        public static cVector2 Sqr(cVector2 v) => new cVector2(v.x * v.x, v.y * v.y);
        
        /// <summary>
        /// Returns a cVector from the application of Sqr (v * v).
        /// </summary>
        public static cVector2 Sqr(Complex v) => new cVector2(v * v);
        
        /// <summary>
        /// Returns a cVector2 from component-wise application of Pow2 (v * v).
        /// </summary>
        public static cVector2 Pow2(cVector2 v) => new cVector2(v.x * v.x, v.y * v.y);
        
        /// <summary>
        /// Returns a cVector from the application of Pow2 (v * v).
        /// </summary>
        public static cVector2 Pow2(Complex v) => new cVector2(v * v);
        
        /// <summary>
        /// Returns a cVector2 from component-wise application of Pow3 (v * v * v).
        /// </summary>
        public static cVector2 Pow3(cVector2 v) => new cVector2(v.x * v.x * v.x, v.y * v.y * v.y);
        
        /// <summary>
        /// Returns a cVector from the application of Pow3 (v * v * v).
        /// </summary>
        public static cVector2 Pow3(Complex v) => new cVector2(v * v * v);
        
        /// <summary>
        /// Returns a cVector2 from component-wise application of Acos (Complex.Acos(v)).
        /// </summary>
        public static cVector2 Acos(cVector2 v) => new cVector2(Complex.Acos(v.x), Complex.Acos(v.y));
        
        /// <summary>
        /// Returns a cVector from the application of Acos (Complex.Acos(v)).
        /// </summary>
        public static cVector2 Acos(Complex v) => new cVector2(Complex.Acos(v));
        
        /// <summary>
        /// Returns a cVector2 from component-wise application of Asin (Complex.Asin(v)).
        /// </summary>
        public static cVector2 Asin(cVector2 v) => new cVector2(Complex.Asin(v.x), Complex.Asin(v.y));
        
        /// <summary>
        /// Returns a cVector from the application of Asin (Complex.Asin(v)).
        /// </summary>
        public static cVector2 Asin(Complex v) => new cVector2(Complex.Asin(v));
        
        /// <summary>
        /// Returns a cVector2 from component-wise application of Atan (Complex.Atan(v)).
        /// </summary>
        public static cVector2 Atan(cVector2 v) => new cVector2(Complex.Atan(v.x), Complex.Atan(v.y));
        
        /// <summary>
        /// Returns a cVector from the application of Atan (Complex.Atan(v)).
        /// </summary>
        public static cVector2 Atan(Complex v) => new cVector2(Complex.Atan(v));
        
        /// <summary>
        /// Returns a cVector2 from component-wise application of Cos (Complex.Cos(v)).
        /// </summary>
        public static cVector2 Cos(cVector2 v) => new cVector2(Complex.Cos(v.x), Complex.Cos(v.y));
        
        /// <summary>
        /// Returns a cVector from the application of Cos (Complex.Cos(v)).
        /// </summary>
        public static cVector2 Cos(Complex v) => new cVector2(Complex.Cos(v));
        
        /// <summary>
        /// Returns a cVector2 from component-wise application of Cosh (Complex.Cosh(v)).
        /// </summary>
        public static cVector2 Cosh(cVector2 v) => new cVector2(Complex.Cosh(v.x), Complex.Cosh(v.y));
        
        /// <summary>
        /// Returns a cVector from the application of Cosh (Complex.Cosh(v)).
        /// </summary>
        public static cVector2 Cosh(Complex v) => new cVector2(Complex.Cosh(v));
        
        /// <summary>
        /// Returns a cVector2 from component-wise application of Exp (Complex.Exp(v)).
        /// </summary>
        public static cVector2 Exp(cVector2 v) => new cVector2(Complex.Exp(v.x), Complex.Exp(v.y));
        
        /// <summary>
        /// Returns a cVector from the application of Exp (Complex.Exp(v)).
        /// </summary>
        public static cVector2 Exp(Complex v) => new cVector2(Complex.Exp(v));
        
        /// <summary>
        /// Returns a cVector2 from component-wise application of Log (Complex.Log(v)).
        /// </summary>
        public static cVector2 Log(cVector2 v) => new cVector2(Complex.Log(v.x), Complex.Log(v.y));
        
        /// <summary>
        /// Returns a cVector from the application of Log (Complex.Log(v)).
        /// </summary>
        public static cVector2 Log(Complex v) => new cVector2(Complex.Log(v));
        
        /// <summary>
        /// Returns a cVector2 from component-wise application of Log2 (Complex.Log(v, 2.0)).
        /// </summary>
        public static cVector2 Log2(cVector2 v) => new cVector2(Complex.Log(v.x, 2.0), Complex.Log(v.y, 2.0));
        
        /// <summary>
        /// Returns a cVector from the application of Log2 (Complex.Log(v, 2.0)).
        /// </summary>
        public static cVector2 Log2(Complex v) => new cVector2(Complex.Log(v, 2.0));
        
        /// <summary>
        /// Returns a cVector2 from component-wise application of Log10 (Complex.Log10(v)).
        /// </summary>
        public static cVector2 Log10(cVector2 v) => new cVector2(Complex.Log10(v.x), Complex.Log10(v.y));
        
        /// <summary>
        /// Returns a cVector from the application of Log10 (Complex.Log10(v)).
        /// </summary>
        public static cVector2 Log10(Complex v) => new cVector2(Complex.Log10(v));
        
        /// <summary>
        /// Returns a cVector2 from component-wise application of Reciprocal (Complex.Reciprocal(v)).
        /// </summary>
        public static cVector2 Reciprocal(cVector2 v) => new cVector2(Complex.Reciprocal(v.x), Complex.Reciprocal(v.y));
        
        /// <summary>
        /// Returns a cVector from the application of Reciprocal (Complex.Reciprocal(v)).
        /// </summary>
        public static cVector2 Reciprocal(Complex v) => new cVector2(Complex.Reciprocal(v));
        
        /// <summary>
        /// Returns a cVector2 from component-wise application of Sin (Complex.Sin(v)).
        /// </summary>
        public static cVector2 Sin(cVector2 v) => new cVector2(Complex.Sin(v.x), Complex.Sin(v.y));
        
        /// <summary>
        /// Returns a cVector from the application of Sin (Complex.Sin(v)).
        /// </summary>
        public static cVector2 Sin(Complex v) => new cVector2(Complex.Sin(v));
        
        /// <summary>
        /// Returns a cVector2 from component-wise application of Sinh (Complex.Sinh(v)).
        /// </summary>
        public static cVector2 Sinh(cVector2 v) => new cVector2(Complex.Sinh(v.x), Complex.Sinh(v.y));
        
        /// <summary>
        /// Returns a cVector from the application of Sinh (Complex.Sinh(v)).
        /// </summary>
        public static cVector2 Sinh(Complex v) => new cVector2(Complex.Sinh(v));
        
        /// <summary>
        /// Returns a cVector2 from component-wise application of Sqrt (Complex.Sqrt(v)).
        /// </summary>
        public static cVector2 Sqrt(cVector2 v) => new cVector2(Complex.Sqrt(v.x), Complex.Sqrt(v.y));
        
        /// <summary>
        /// Returns a cVector from the application of Sqrt (Complex.Sqrt(v)).
        /// </summary>
        public static cVector2 Sqrt(Complex v) => new cVector2(Complex.Sqrt(v));
        
        /// <summary>
        /// Returns a cVector2 from component-wise application of InverseSqrt (Complex.One / Complex.Sqrt(v)).
        /// </summary>
        public static cVector2 InverseSqrt(cVector2 v) => new cVector2(Complex.One / Complex.Sqrt(v.x), Complex.One / Complex.Sqrt(v.y));
        
        /// <summary>
        /// Returns a cVector from the application of InverseSqrt (Complex.One / Complex.Sqrt(v)).
        /// </summary>
        public static cVector2 InverseSqrt(Complex v) => new cVector2(Complex.One / Complex.Sqrt(v));
        
        /// <summary>
        /// Returns a cVector2 from component-wise application of Tan (Complex.Tan(v)).
        /// </summary>
        public static cVector2 Tan(cVector2 v) => new cVector2(Complex.Tan(v.x), Complex.Tan(v.y));
        
        /// <summary>
        /// Returns a cVector from the application of Tan (Complex.Tan(v)).
        /// </summary>
        public static cVector2 Tan(Complex v) => new cVector2(Complex.Tan(v));
        
        /// <summary>
        /// Returns a cVector2 from component-wise application of Tanh (Complex.Tanh(v)).
        /// </summary>
        public static cVector2 Tanh(cVector2 v) => new cVector2(Complex.Tanh(v.x), Complex.Tanh(v.y));
        
        /// <summary>
        /// Returns a cVector from the application of Tanh (Complex.Tanh(v)).
        /// </summary>
        public static cVector2 Tanh(Complex v) => new cVector2(Complex.Tanh(v));
        
        /// <summary>
        /// Returns a cVector2 from component-wise application of Conjugate (Complex.Conjugate(v)).
        /// </summary>
        public static cVector2 Conjugate(cVector2 v) => new cVector2(Complex.Conjugate(v.x), Complex.Conjugate(v.y));
        
        /// <summary>
        /// Returns a cVector from the application of Conjugate (Complex.Conjugate(v)).
        /// </summary>
        public static cVector2 Conjugate(Complex v) => new cVector2(Complex.Conjugate(v));
        
        /// <summary>
        /// Returns a cVector2 from component-wise application of Pow (Complex.Pow(lhs, rhs)).
        /// </summary>
        public static cVector2 Pow(cVector2 lhs, cVector2 rhs) => new cVector2(Complex.Pow(lhs.x, rhs.x), Complex.Pow(lhs.y, rhs.y));
        
        /// <summary>
        /// Returns a cVector2 from component-wise application of Pow (Complex.Pow(lhs, rhs)).
        /// </summary>
        public static cVector2 Pow(cVector2 lhs, Complex rhs) => new cVector2(Complex.Pow(lhs.x, rhs), Complex.Pow(lhs.y, rhs));
        
        /// <summary>
        /// Returns a cVector2 from component-wise application of Pow (Complex.Pow(lhs, rhs)).
        /// </summary>
        public static cVector2 Pow(Complex lhs, cVector2 rhs) => new cVector2(Complex.Pow(lhs, rhs.x), Complex.Pow(lhs, rhs.y));
        
        /// <summary>
        /// Returns a cVector from the application of Pow (Complex.Pow(lhs, rhs)).
        /// </summary>
        public static cVector2 Pow(Complex lhs, Complex rhs) => new cVector2(Complex.Pow(lhs, rhs));
        
        /// <summary>
        /// Returns a cVector2 from component-wise application of Log (Complex.Log(lhs, rhs)).
        /// </summary>
        public static cVector2 Log(cVector2 lhs, dVector2 rhs) => new cVector2(Complex.Log(lhs.x, rhs.x), Complex.Log(lhs.y, rhs.y));
        
        /// <summary>
        /// Returns a cVector2 from component-wise application of Log (Complex.Log(lhs, rhs)).
        /// </summary>
        public static cVector2 Log(cVector2 lhs, double rhs) => new cVector2(Complex.Log(lhs.x, rhs), Complex.Log(lhs.y, rhs));
        
        /// <summary>
        /// Returns a cVector2 from component-wise application of Log (Complex.Log(lhs, rhs)).
        /// </summary>
        public static cVector2 Log(Complex lhs, dVector2 rhs) => new cVector2(Complex.Log(lhs, rhs.x), Complex.Log(lhs, rhs.y));
        
        /// <summary>
        /// Returns a cVector from the application of Log (Complex.Log(lhs, rhs)).
        /// </summary>
        public static cVector2 Log(Complex lhs, double rhs) => new cVector2(Complex.Log(lhs, rhs));
        
        /// <summary>
        /// Returns a cVector2 from component-wise application of FromPolarCoordinates (Complex.FromPolarCoordinates(magnitude, phase)).
        /// </summary>
        public static cVector2 FromPolarCoordinates(dVector2 magnitude, dVector2 phase) => new cVector2(Complex.FromPolarCoordinates(magnitude.x, phase.x), Complex.FromPolarCoordinates(magnitude.y, phase.y));
        
        /// <summary>
        /// Returns a cVector2 from component-wise application of FromPolarCoordinates (Complex.FromPolarCoordinates(magnitude, phase)).
        /// </summary>
        public static cVector2 FromPolarCoordinates(dVector2 magnitude, double phase) => new cVector2(Complex.FromPolarCoordinates(magnitude.x, phase), Complex.FromPolarCoordinates(magnitude.y, phase));
        
        /// <summary>
        /// Returns a cVector2 from component-wise application of FromPolarCoordinates (Complex.FromPolarCoordinates(magnitude, phase)).
        /// </summary>
        public static cVector2 FromPolarCoordinates(double magnitude, dVector2 phase) => new cVector2(Complex.FromPolarCoordinates(magnitude, phase.x), Complex.FromPolarCoordinates(magnitude, phase.y));
        
        /// <summary>
        /// Returns a cVector from the application of FromPolarCoordinates (Complex.FromPolarCoordinates(magnitude, phase)).
        /// </summary>
        public static cVector2 FromPolarCoordinates(double magnitude, double phase) => new cVector2(Complex.FromPolarCoordinates(magnitude, phase));
        
        /// <summary>
        /// Returns a cVector2 from component-wise application of Fma (a * b + c).
        /// </summary>
        public static cVector2 Fma(cVector2 a, cVector2 b, cVector2 c) => new cVector2(a.x * b.x + c.x, a.y * b.y + c.y);
        
        /// <summary>
        /// Returns a cVector2 from component-wise application of Fma (a * b + c).
        /// </summary>
        public static cVector2 Fma(cVector2 a, cVector2 b, Complex c) => new cVector2(a.x * b.x + c, a.y * b.y + c);
        
        /// <summary>
        /// Returns a cVector2 from component-wise application of Fma (a * b + c).
        /// </summary>
        public static cVector2 Fma(cVector2 a, Complex b, cVector2 c) => new cVector2(a.x * b + c.x, a.y * b + c.y);
        
        /// <summary>
        /// Returns a cVector2 from component-wise application of Fma (a * b + c).
        /// </summary>
        public static cVector2 Fma(cVector2 a, Complex b, Complex c) => new cVector2(a.x * b + c, a.y * b + c);
        
        /// <summary>
        /// Returns a cVector2 from component-wise application of Fma (a * b + c).
        /// </summary>
        public static cVector2 Fma(Complex a, cVector2 b, cVector2 c) => new cVector2(a * b.x + c.x, a * b.y + c.y);
        
        /// <summary>
        /// Returns a cVector2 from component-wise application of Fma (a * b + c).
        /// </summary>
        public static cVector2 Fma(Complex a, cVector2 b, Complex c) => new cVector2(a * b.x + c, a * b.y + c);
        
        /// <summary>
        /// Returns a cVector2 from component-wise application of Fma (a * b + c).
        /// </summary>
        public static cVector2 Fma(Complex a, Complex b, cVector2 c) => new cVector2(a * b + c.x, a * b + c.y);
        
        /// <summary>
        /// Returns a cVector from the application of Fma (a * b + c).
        /// </summary>
        public static cVector2 Fma(Complex a, Complex b, Complex c) => new cVector2(a * b + c);
        
        /// <summary>
        /// Returns a cVector2 from component-wise application of Add (lhs + rhs).
        /// </summary>
        public static cVector2 Add(cVector2 lhs, cVector2 rhs) => new cVector2(lhs.x + rhs.x, lhs.y + rhs.y);
        
        /// <summary>
        /// Returns a cVector2 from component-wise application of Add (lhs + rhs).
        /// </summary>
        public static cVector2 Add(cVector2 lhs, Complex rhs) => new cVector2(lhs.x + rhs, lhs.y + rhs);
        
        /// <summary>
        /// Returns a cVector2 from component-wise application of Add (lhs + rhs).
        /// </summary>
        public static cVector2 Add(Complex lhs, cVector2 rhs) => new cVector2(lhs + rhs.x, lhs + rhs.y);
        
        /// <summary>
        /// Returns a cVector from the application of Add (lhs + rhs).
        /// </summary>
        public static cVector2 Add(Complex lhs, Complex rhs) => new cVector2(lhs + rhs);
        
        /// <summary>
        /// Returns a cVector2 from component-wise application of Sub (lhs - rhs).
        /// </summary>
        public static cVector2 Sub(cVector2 lhs, cVector2 rhs) => new cVector2(lhs.x - rhs.x, lhs.y - rhs.y);
        
        /// <summary>
        /// Returns a cVector2 from component-wise application of Sub (lhs - rhs).
        /// </summary>
        public static cVector2 Sub(cVector2 lhs, Complex rhs) => new cVector2(lhs.x - rhs, lhs.y - rhs);
        
        /// <summary>
        /// Returns a cVector2 from component-wise application of Sub (lhs - rhs).
        /// </summary>
        public static cVector2 Sub(Complex lhs, cVector2 rhs) => new cVector2(lhs - rhs.x, lhs - rhs.y);
        
        /// <summary>
        /// Returns a cVector from the application of Sub (lhs - rhs).
        /// </summary>
        public static cVector2 Sub(Complex lhs, Complex rhs) => new cVector2(lhs - rhs);
        
        /// <summary>
        /// Returns a cVector2 from component-wise application of Mul (lhs * rhs).
        /// </summary>
        public static cVector2 Mul(cVector2 lhs, cVector2 rhs) => new cVector2(lhs.x * rhs.x, lhs.y * rhs.y);
        
        /// <summary>
        /// Returns a cVector2 from component-wise application of Mul (lhs * rhs).
        /// </summary>
        public static cVector2 Mul(cVector2 lhs, Complex rhs) => new cVector2(lhs.x * rhs, lhs.y * rhs);
        
        /// <summary>
        /// Returns a cVector2 from component-wise application of Mul (lhs * rhs).
        /// </summary>
        public static cVector2 Mul(Complex lhs, cVector2 rhs) => new cVector2(lhs * rhs.x, lhs * rhs.y);
        
        /// <summary>
        /// Returns a cVector from the application of Mul (lhs * rhs).
        /// </summary>
        public static cVector2 Mul(Complex lhs, Complex rhs) => new cVector2(lhs * rhs);
        
        /// <summary>
        /// Returns a cVector2 from component-wise application of Div (lhs / rhs).
        /// </summary>
        public static cVector2 Div(cVector2 lhs, cVector2 rhs) => new cVector2(lhs.x / rhs.x, lhs.y / rhs.y);
        
        /// <summary>
        /// Returns a cVector2 from component-wise application of Div (lhs / rhs).
        /// </summary>
        public static cVector2 Div(cVector2 lhs, Complex rhs) => new cVector2(lhs.x / rhs, lhs.y / rhs);
        
        /// <summary>
        /// Returns a cVector2 from component-wise application of Div (lhs / rhs).
        /// </summary>
        public static cVector2 Div(Complex lhs, cVector2 rhs) => new cVector2(lhs / rhs.x, lhs / rhs.y);
        
        /// <summary>
        /// Returns a cVector from the application of Div (lhs / rhs).
        /// </summary>
        public static cVector2 Div(Complex lhs, Complex rhs) => new cVector2(lhs / rhs);

        #endregion


        #region Component-Wise Operator Overloads
        
        /// <summary>
        /// Returns a cVector2 from component-wise application of operator+ (lhs + rhs).
        /// </summary>
        public static cVector2 operator+(cVector2 lhs, cVector2 rhs) => new cVector2(lhs.x + rhs.x, lhs.y + rhs.y);
        
        /// <summary>
        /// Returns a cVector2 from component-wise application of operator+ (lhs + rhs).
        /// </summary>
        public static cVector2 operator+(cVector2 lhs, Complex rhs) => new cVector2(lhs.x + rhs, lhs.y + rhs);
        
        /// <summary>
        /// Returns a cVector2 from component-wise application of operator+ (lhs + rhs).
        /// </summary>
        public static cVector2 operator+(Complex lhs, cVector2 rhs) => new cVector2(lhs + rhs.x, lhs + rhs.y);
        
        /// <summary>
        /// Returns a cVector2 from component-wise application of operator- (lhs - rhs).
        /// </summary>
        public static cVector2 operator-(cVector2 lhs, cVector2 rhs) => new cVector2(lhs.x - rhs.x, lhs.y - rhs.y);
        
        /// <summary>
        /// Returns a cVector2 from component-wise application of operator- (lhs - rhs).
        /// </summary>
        public static cVector2 operator-(cVector2 lhs, Complex rhs) => new cVector2(lhs.x - rhs, lhs.y - rhs);
        
        /// <summary>
        /// Returns a cVector2 from component-wise application of operator- (lhs - rhs).
        /// </summary>
        public static cVector2 operator-(Complex lhs, cVector2 rhs) => new cVector2(lhs - rhs.x, lhs - rhs.y);
        
        /// <summary>
        /// Returns a cVector2 from component-wise application of operator* (lhs * rhs).
        /// </summary>
        public static cVector2 operator*(cVector2 lhs, cVector2 rhs) => new cVector2(lhs.x * rhs.x, lhs.y * rhs.y);
        
        /// <summary>
        /// Returns a cVector2 from component-wise application of operator* (lhs * rhs).
        /// </summary>
        public static cVector2 operator*(cVector2 lhs, Complex rhs) => new cVector2(lhs.x * rhs, lhs.y * rhs);
        
        /// <summary>
        /// Returns a cVector2 from component-wise application of operator* (lhs * rhs).
        /// </summary>
        public static cVector2 operator*(Complex lhs, cVector2 rhs) => new cVector2(lhs * rhs.x, lhs * rhs.y);
        
        /// <summary>
        /// Returns a cVector2 from component-wise application of operator/ (lhs / rhs).
        /// </summary>
        public static cVector2 operator/(cVector2 lhs, cVector2 rhs) => new cVector2(lhs.x / rhs.x, lhs.y / rhs.y);
        
        /// <summary>
        /// Returns a cVector2 from component-wise application of operator/ (lhs / rhs).
        /// </summary>
        public static cVector2 operator/(cVector2 lhs, Complex rhs) => new cVector2(lhs.x / rhs, lhs.y / rhs);
        
        /// <summary>
        /// Returns a cVector2 from component-wise application of operator/ (lhs / rhs).
        /// </summary>
        public static cVector2 operator/(Complex lhs, cVector2 rhs) => new cVector2(lhs / rhs.x, lhs / rhs.y);
        
        /// <summary>
        /// Returns a cVector2 from component-wise application of operator+ (identity).
        /// </summary>
        public static cVector2 operator+(cVector2 v) => v;
        
        /// <summary>
        /// Returns a cVector2 from component-wise application of operator- (-v).
        /// </summary>
        public static cVector2 operator-(cVector2 v) => new cVector2(-v.x, -v.y);

        #endregion

    }
    */
}
