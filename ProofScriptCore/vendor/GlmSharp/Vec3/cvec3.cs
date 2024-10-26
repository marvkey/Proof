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
    /// A Vector of type Complex with 3 components.
    /// </summary>
    /// 
    /*
    
    
    [StructLayout(LayoutKind.Sequential)]
    public struct cVector3 : IReadOnlyList<Complex>, IEquatable<cVector3>
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
        
        /// <summary>
        /// z-component
        /// </summary>
        
        public Complex z;

        #endregion


        #region Constructors
        
        /// <summary>
        /// Component-wise constructor
        /// </summary>
        public cVector3(Complex x, Complex y, Complex z)
        {
            this.x = x;
            this.y = y;
            this.z = z;
        }
        
        /// <summary>
        /// all-same-value constructor
        /// </summary>
        public cVector3(Complex v)
        {
            this.x = v;
            this.y = v;
            this.z = v;
        }
        
        /// <summary>
        /// from-Vector constructor (empty fields are zero/false)
        /// </summary>
        public cVector3(cVector2 v)
        {
            this.x = v.x;
            this.y = v.y;
            this.z = Complex.Zero;
        }
        
        /// <summary>
        /// from-Vector-and-value constructor
        /// </summary>
        public cVector3(cVector2 v, Complex z)
        {
            this.x = v.x;
            this.y = v.y;
            this.z = z;
        }
        
        /// <summary>
        /// from-Vector constructor
        /// </summary>
        public cVector3(cVector3 v)
        {
            this.x = v.x;
            this.y = v.y;
            this.z = v.z;
        }
        
        /// <summary>
        /// from-Vector constructor (additional fields are truncated)
        /// </summary>
        public cVector3(cVector4 v)
        {
            this.x = v.x;
            this.y = v.y;
            this.z = v.z;
        }
        
        /// <summary>
        /// From-array/list constructor (superfluous values are ignored, missing values are zero-filled).
        /// </summary>
        public cVector3(IReadOnlyList<Complex> v)
        {
            var c = v.Count;
            this.x = c < 0 ? Complex.Zero : v[0];
            this.y = c < 1 ? Complex.Zero : v[1];
            this.z = c < 2 ? Complex.Zero : v[2];
        }
        
        /// <summary>
        /// Generic from-array constructor (superfluous values are ignored, missing values are zero-filled).
        /// </summary>
        public cVector3(Object[] v)
        {
            var c = v.Length;
            this.x = c < 0 ? Complex.Zero : (Complex)v[0];
            this.y = c < 1 ? Complex.Zero : (Complex)v[1];
            this.z = c < 2 ? Complex.Zero : (Complex)v[2];
        }
        
        /// <summary>
        /// From-array constructor (superfluous values are ignored, missing values are zero-filled).
        /// </summary>
        public cVector3(Complex[] v)
        {
            var c = v.Length;
            this.x = c < 0 ? Complex.Zero : v[0];
            this.y = c < 1 ? Complex.Zero : v[1];
            this.z = c < 2 ? Complex.Zero : v[2];
        }
        
        /// <summary>
        /// From-array constructor with base index (superfluous values are ignored, missing values are zero-filled).
        /// </summary>
        public cVector3(Complex[] v, int startIndex)
        {
            var c = v.Length;
            this.x = c + startIndex < 0 ? Complex.Zero : v[0 + startIndex];
            this.y = c + startIndex < 1 ? Complex.Zero : v[1 + startIndex];
            this.z = c + startIndex < 2 ? Complex.Zero : v[2 + startIndex];
        }
        
        /// <summary>
        /// From-IEnumerable constructor (superfluous values are ignored, missing values are zero-filled).
        /// </summary>
        public cVector3(IEnumerable<Complex> v)
            : this(v.ToArray())
        {
        }

        #endregion


        #region Explicit Operators
        
        /// <summary>
        /// Explicitly converts this to a cVector2.
        /// </summary>
        public static explicit operator cVector2(cVector3 v) => new cVector2((Complex)v.x, (Complex)v.y);
        
        /// <summary>
        /// Explicitly converts this to a cVector4. (Higher components are zeroed)
        /// </summary>
        public static explicit operator cVector4(cVector3 v) => new cVector4((Complex)v.x, (Complex)v.y, (Complex)v.z, Complex.Zero);
        
        /// <summary>
        /// Explicitly converts this to a Complex array.
        /// </summary>
        public static explicit operator Complex[](cVector3 v) => new [] { v.x, v.y, v.z };
        
        /// <summary>
        /// Explicitly converts this to a generic object array.
        /// </summary>
        public static explicit operator Object[](cVector3 v) => new Object[] { v.x, v.y, v.z };

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
                    case 2: return z;
                    default: throw new ArgumentOutOfRangeException("index");
                }
            }
            set
            {
                switch (index)
                {
                    case 0: x = value; break;
                    case 1: y = value; break;
                    case 2: z = value; break;
                    default: throw new ArgumentOutOfRangeException("index");
                }
            }
        }

        #endregion


        #region Properties
        
        /// <summary>
        /// Returns an object that can be used for arbitrary swizzling (e.g. swizzle.zy)
        /// </summary>
        public swizzle_cVector3 swizzle => new swizzle_cVector3(x, y, z);
        
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
        public cVector2 xz
        {
            get
            {
                return new cVector2(x, z);
            }
            set
            {
                x = value.x;
                z = value.y;
            }
        }
        
        /// <summary>
        /// Gets or sets the specified subset of components. For more advanced (read-only) swizzling, use the .swizzle property.
        /// </summary>
        public cVector2 yz
        {
            get
            {
                return new cVector2(y, z);
            }
            set
            {
                y = value.x;
                z = value.y;
            }
        }
        
        /// <summary>
        /// Gets or sets the specified subset of components. For more advanced (read-only) swizzling, use the .swizzle property.
        /// </summary>
        public cVector3 xyz
        {
            get
            {
                return new cVector3(x, y, z);
            }
            set
            {
                x = value.x;
                y = value.y;
                z = value.z;
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
        /// Gets or sets the specified subset of components. For more advanced (read-only) swizzling, use the .swizzle property.
        /// </summary>
        public cVector2 rb
        {
            get
            {
                return new cVector2(x, z);
            }
            set
            {
                x = value.x;
                z = value.y;
            }
        }
        
        /// <summary>
        /// Gets or sets the specified subset of components. For more advanced (read-only) swizzling, use the .swizzle property.
        /// </summary>
        public cVector2 gb
        {
            get
            {
                return new cVector2(y, z);
            }
            set
            {
                y = value.x;
                z = value.y;
            }
        }
        
        /// <summary>
        /// Gets or sets the specified subset of components. For more advanced (read-only) swizzling, use the .swizzle property.
        /// </summary>
        public cVector3 rgb
        {
            get
            {
                return new cVector3(x, y, z);
            }
            set
            {
                x = value.x;
                y = value.y;
                z = value.z;
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
        /// Gets or sets the specified RGBA component. For more advanced (read-only) swizzling, use the .swizzle property.
        /// </summary>
        public Complex b
        {
            get
            {
                return z;
            }
            set
            {
                z = value;
            }
        }
        
        /// <summary>
        /// Returns an array with all values
        /// </summary>
        public Complex[] Values => new[] { x, y, z };
        
        /// <summary>
        /// Returns the number of components (3).
        /// </summary>
        public int Count => 3;
        
        /// <summary>
        /// Returns the euclidean length of this Vector.
        /// </summary>
        public double Length => (double)Math.Sqrt(((x.LengthSqr() + y.LengthSqr()) + z.LengthSqr()));
        
        /// <summary>
        /// Returns the squared euclidean length of this Vector.
        /// </summary>
        public double LengthSqr => ((x.LengthSqr() + y.LengthSqr()) + z.LengthSqr());
        
        /// <summary>
        /// Returns the sum of all components.
        /// </summary>
        public Complex Sum => ((x + y) + z);
        
        /// <summary>
        /// Returns the euclidean norm of this Vector.
        /// </summary>
        public double Norm => (double)Math.Sqrt(((x.LengthSqr() + y.LengthSqr()) + z.LengthSqr()));
        
        /// <summary>
        /// Returns the one-norm of this Vector.
        /// </summary>
        public double Norm1 => ((x.Magnitude + y.Magnitude) + z.Magnitude);
        
        /// <summary>
        /// Returns the two-norm (euclidean length) of this Vector.
        /// </summary>
        public double Norm2 => (double)Math.Sqrt(((x.LengthSqr() + y.LengthSqr()) + z.LengthSqr()));
        
        /// <summary>
        /// Returns the max-norm of this Vector.
        /// </summary>
        public double NormMax => Math.Max(Math.Max(x.Magnitude, y.Magnitude), z.Magnitude);
        
        /// <summary>
        /// Returns a copy of this Vector with length one (undefined if this has zero length).
        /// </summary>
        public cVector3 Normalized => this / (Complex)Length;
        
        /// <summary>
        /// Returns a copy of this Vector with length one (returns zero if length is zero).
        /// </summary>
        public cVector3 NormalizedSafe => this == Zero ? Zero : this / (Complex)Length;
        
        /// <summary>
        /// Returns a Vector containing component-wise magnitudes.
        /// </summary>
        public dVector3 Magnitude => new dVector3(x.Magnitude, y.Magnitude, z.Magnitude);
        
        /// <summary>
        /// Returns a Vector containing component-wise phases.
        /// </summary>
        public dVector3 Phase => new dVector3(x.Phase, y.Phase, z.Phase);
        
        /// <summary>
        /// Returns a Vector containing component-wise imaginary parts.
        /// </summary>
        public dVector3 Imaginary => new dVector3(x.Imaginary, y.Imaginary, z.Imaginary);
        
        /// <summary>
        /// Returns a Vector containing component-wise real parts.
        /// </summary>
        public dVector3 Real => new dVector3(x.Real, y.Real, z.Real);

        #endregion


        #region Static Properties
        
        /// <summary>
        /// Predefined all-zero Vector
        /// </summary>
        public static cVector3 Zero { get; } = new cVector3(Complex.Zero, Complex.Zero, Complex.Zero);
        
        /// <summary>
        /// Predefined all-ones Vector
        /// </summary>
        public static cVector3 Ones { get; } = new cVector3(Complex.One, Complex.One, Complex.One);
        
        /// <summary>
        /// Predefined unit-X Vector
        /// </summary>
        public static cVector3 UnitX { get; } = new cVector3(Complex.One, Complex.Zero, Complex.Zero);
        
        /// <summary>
        /// Predefined unit-Y Vector
        /// </summary>
        public static cVector3 UnitY { get; } = new cVector3(Complex.Zero, Complex.One, Complex.Zero);
        
        /// <summary>
        /// Predefined unit-Z Vector
        /// </summary>
        public static cVector3 UnitZ { get; } = new cVector3(Complex.Zero, Complex.Zero, Complex.One);
        
        /// <summary>
        /// Predefined all-imaginary-ones Vector
        /// </summary>
        public static cVector3 ImaginaryOnes { get; } = new cVector3(Complex.ImaginaryOne, Complex.ImaginaryOne, Complex.ImaginaryOne);
        
        /// <summary>
        /// Predefined unit-imaginary-X Vector
        /// </summary>
        public static cVector3 ImaginaryUnitX { get; } = new cVector3(Complex.ImaginaryOne, Complex.Zero, Complex.Zero);
        
        /// <summary>
        /// Predefined unit-imaginary-Y Vector
        /// </summary>
        public static cVector3 ImaginaryUnitY { get; } = new cVector3(Complex.Zero, Complex.ImaginaryOne, Complex.Zero);
        
        /// <summary>
        /// Predefined unit-imaginary-Z Vector
        /// </summary>
        public static cVector3 ImaginaryUnitZ { get; } = new cVector3(Complex.Zero, Complex.Zero, Complex.ImaginaryOne);

        #endregion


        #region Operators
        
        /// <summary>
        /// Returns true iff this equals rhs component-wise.
        /// </summary>
        public static bool operator==(cVector3 lhs, cVector3 rhs) => lhs.Equals(rhs);
        
        /// <summary>
        /// Returns true iff this does not equal rhs (component-wise).
        /// </summary>
        public static bool operator!=(cVector3 lhs, cVector3 rhs) => !lhs.Equals(rhs);

        #endregion


        #region Functions
        
        /// <summary>
        /// Returns an enumerator that iterates through all components.
        /// </summary>
        public IEnumerator<Complex> GetEnumerator()
        {
            yield return x;
            yield return y;
            yield return z;
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
        public string ToString(string sep) => ((x + sep + y) + sep + z);
        
        /// <summary>
        /// Returns a string representation of this Vector using a provided seperator and a forMatrix provider for each component.
        /// </summary>
        public string ToString(string sep, IFormatProvider provider) => ((x.ToString(provider) + sep + y.ToString(provider)) + sep + z.ToString(provider));
        
        /// <summary>
        /// Returns a string representation of this Vector using a provided seperator and a forMatrix for each component.
        /// </summary>
        public string ToString(string sep, string forMatrix) => ((x.ToString(forMatrix) + sep + y.ToString(forMatrix)) + sep + z.ToString(forMatrix));
        
        /// <summary>
        /// Returns a string representation of this Vector using a provided seperator and a forMatrix and forMatrix provider for each component.
        /// </summary>
        public string ToString(string sep, string forMatrix, IFormatProvider provider) => ((x.ToString(forMatrix, provider) + sep + y.ToString(forMatrix, provider)) + sep + z.ToString(forMatrix, provider));
        
        /// <summary>
        /// Returns true iff this equals rhs component-wise.
        /// </summary>
        public bool Equals(cVector3 rhs) => ((x.Equals(rhs.x) && y.Equals(rhs.y)) && z.Equals(rhs.z));
        
        /// <summary>
        /// Returns true iff this equals rhs type- and component-wise.
        /// </summary>
        public override bool Equals(object obj)
        {
            if (ReferenceEquals(null, obj)) return false;
            return obj is cVector3 && Equals((cVector3) obj);
        }
        
        /// <summary>
        /// Returns a hash code for this instance.
        /// </summary>
        public override int GetHashCode()
        {
            unchecked
            {
                return ((((x.GetHashCode()) * 397) ^ y.GetHashCode()) * 397) ^ z.GetHashCode();
            }
        }
        
        /// <summary>
        /// Returns the p-norm of this Vector.
        /// </summary>
        public double NormP(double p) => Math.Pow(((Math.Pow((double)x.Magnitude, p) + Math.Pow((double)y.Magnitude, p)) + Math.Pow((double)z.Magnitude, p)), 1 / p);

        #endregion


        #region Static Functions
        
        /// <summary>
        /// Returns true iff distance between lhs and rhs is less than or equal to epsilon
        /// </summary>
        public static bool ApproxEqual(cVector3 lhs, cVector3 rhs, double eps = 0.1d) => Distance(lhs, rhs) <= eps;
        
        /// <summary>
        /// OuterProduct treats the first parameter c as a column Vector (Matrix with one column) and the second parameter r as a row Vector (Matrix with one row) and does a linear algebraic Matrix multiply c * r, yielding a Matrix whose number of rows is the number of components in c and whose number of columns is the number of components in r.
        /// </summary>
        public static cMatrix3x2 OuterProduct(cVector2 c, cVector3 r) => new cMatrix3x2(c.x * r.x, c.y * r.x, c.x * r.y, c.y * r.y, c.x * r.z, c.y * r.z);
        
        /// <summary>
        /// OuterProduct treats the first parameter c as a column Vector (Matrix with one column) and the second parameter r as a row Vector (Matrix with one row) and does a linear algebraic Matrix multiply c * r, yielding a Matrix whose number of rows is the number of components in c and whose number of columns is the number of components in r.
        /// </summary>
        public static cMatrix2x3 OuterProduct(cVector3 c, cVector2 r) => new cMatrix2x3(c.x * r.x, c.y * r.x, c.z * r.x, c.x * r.y, c.y * r.y, c.z * r.y);
        
        /// <summary>
        /// OuterProduct treats the first parameter c as a column Vector (Matrix with one column) and the second parameter r as a row Vector (Matrix with one row) and does a linear algebraic Matrix multiply c * r, yielding a Matrix whose number of rows is the number of components in c and whose number of columns is the number of components in r.
        /// </summary>
        public static cMatrix3 OuterProduct(cVector3 c, cVector3 r) => new cMatrix3(c.x * r.x, c.y * r.x, c.z * r.x, c.x * r.y, c.y * r.y, c.z * r.y, c.x * r.z, c.y * r.z, c.z * r.z);
        
        /// <summary>
        /// OuterProduct treats the first parameter c as a column Vector (Matrix with one column) and the second parameter r as a row Vector (Matrix with one row) and does a linear algebraic Matrix multiply c * r, yielding a Matrix whose number of rows is the number of components in c and whose number of columns is the number of components in r.
        /// </summary>
        public static cMatrix3x4 OuterProduct(cVector4 c, cVector3 r) => new cMatrix3x4(c.x * r.x, c.y * r.x, c.z * r.x, c.w * r.x, c.x * r.y, c.y * r.y, c.z * r.y, c.w * r.y, c.x * r.z, c.y * r.z, c.z * r.z, c.w * r.z);
        
        /// <summary>
        /// OuterProduct treats the first parameter c as a column Vector (Matrix with one column) and the second parameter r as a row Vector (Matrix with one row) and does a linear algebraic Matrix multiply c * r, yielding a Matrix whose number of rows is the number of components in c and whose number of columns is the number of components in r.
        /// </summary>
        public static cMatrix4x3 OuterProduct(cVector3 c, cVector4 r) => new cMatrix4x3(c.x * r.x, c.y * r.x, c.z * r.x, c.x * r.y, c.y * r.y, c.z * r.y, c.x * r.z, c.y * r.z, c.z * r.z, c.x * r.w, c.y * r.w, c.z * r.w);
        
        /// <summary>
        /// Returns the inner product (dot product, scalar product) of the two Vectors.
        /// </summary>
        public static Complex Dot(cVector3 lhs, cVector3 rhs) => ((lhs.x * Complex.Conjugate(rhs.x) + lhs.y * Complex.Conjugate(rhs.y)) + lhs.z * Complex.Conjugate(rhs.z));
        
        /// <summary>
        /// Returns the euclidean distance between the two Vectors.
        /// </summary>
        public static double Distance(cVector3 lhs, cVector3 rhs) => (lhs - rhs).Length;
        
        /// <summary>
        /// Returns the squared euclidean distance between the two Vectors.
        /// </summary>
        public static double DistanceSqr(cVector3 lhs, cVector3 rhs) => (lhs - rhs).LengthSqr;
        
        /// <summary>
        /// Calculate the reflection direction for an incident Vector (N should be normalized in order to achieve the desired result).
        /// </summary>
        public static cVector3 Reflect(cVector3 I, cVector3 N) => I - 2 * Dot(N, I) * N;
        
        /// <summary>
        /// Returns the outer product (cross product, Vector product) of the two Vectors.
        /// </summary>
        public static cVector3 Cross(cVector3 l, cVector3 r) => new cVector3(l.y * r.z - l.z * r.y, l.z * r.x - l.x * r.z, l.x * r.y - l.y * r.x);

        #endregion


        #region Component-Wise Static Functions
        
        /// <summary>
        /// Returns a bVector3 from component-wise application of Equal (lhs == rhs).
        /// </summary>
        public static bVector3 Equal(cVector3 lhs, cVector3 rhs) => new bVector3(lhs.x == rhs.x, lhs.y == rhs.y, lhs.z == rhs.z);
        
        /// <summary>
        /// Returns a bVector3 from component-wise application of Equal (lhs == rhs).
        /// </summary>
        public static bVector3 Equal(cVector3 lhs, Complex rhs) => new bVector3(lhs.x == rhs, lhs.y == rhs, lhs.z == rhs);
        
        /// <summary>
        /// Returns a bVector3 from component-wise application of Equal (lhs == rhs).
        /// </summary>
        public static bVector3 Equal(Complex lhs, cVector3 rhs) => new bVector3(lhs == rhs.x, lhs == rhs.y, lhs == rhs.z);
        
        /// <summary>
        /// Returns a bVector from the application of Equal (lhs == rhs).
        /// </summary>
        public static bVector3 Equal(Complex lhs, Complex rhs) => new bVector3(lhs == rhs);
        
        /// <summary>
        /// Returns a bVector3 from component-wise application of NotEqual (lhs != rhs).
        /// </summary>
        public static bVector3 NotEqual(cVector3 lhs, cVector3 rhs) => new bVector3(lhs.x != rhs.x, lhs.y != rhs.y, lhs.z != rhs.z);
        
        /// <summary>
        /// Returns a bVector3 from component-wise application of NotEqual (lhs != rhs).
        /// </summary>
        public static bVector3 NotEqual(cVector3 lhs, Complex rhs) => new bVector3(lhs.x != rhs, lhs.y != rhs, lhs.z != rhs);
        
        /// <summary>
        /// Returns a bVector3 from component-wise application of NotEqual (lhs != rhs).
        /// </summary>
        public static bVector3 NotEqual(Complex lhs, cVector3 rhs) => new bVector3(lhs != rhs.x, lhs != rhs.y, lhs != rhs.z);
        
        /// <summary>
        /// Returns a bVector from the application of NotEqual (lhs != rhs).
        /// </summary>
        public static bVector3 NotEqual(Complex lhs, Complex rhs) => new bVector3(lhs != rhs);
        
        /// <summary>
        /// Returns a dVector3 from component-wise application of Abs (v.Magnitude).
        /// </summary>
        public static dVector3 Abs(cVector3 v) => new dVector3(v.x.Magnitude, v.y.Magnitude, v.z.Magnitude);
        
        /// <summary>
        /// Returns a dVector from the application of Abs (v.Magnitude).
        /// </summary>
        public static dVector3 Abs(Complex v) => new dVector3(v.Magnitude);
        
        /// <summary>
        /// Returns a cVector3 from component-wise application of HermiteInterpolationOrder3 ((3 - 2 * v) * v * v).
        /// </summary>
        public static cVector3 HermiteInterpolationOrder3(cVector3 v) => new cVector3((3 - 2 * v.x) * v.x * v.x, (3 - 2 * v.y) * v.y * v.y, (3 - 2 * v.z) * v.z * v.z);
        
        /// <summary>
        /// Returns a cVector from the application of HermiteInterpolationOrder3 ((3 - 2 * v) * v * v).
        /// </summary>
        public static cVector3 HermiteInterpolationOrder3(Complex v) => new cVector3((3 - 2 * v) * v * v);
        
        /// <summary>
        /// Returns a cVector3 from component-wise application of HermiteInterpolationOrder5 (((6 * v - 15) * v + 10) * v * v * v).
        /// </summary>
        public static cVector3 HermiteInterpolationOrder5(cVector3 v) => new cVector3(((6 * v.x - 15) * v.x + 10) * v.x * v.x * v.x, ((6 * v.y - 15) * v.y + 10) * v.y * v.y * v.y, ((6 * v.z - 15) * v.z + 10) * v.z * v.z * v.z);
        
        /// <summary>
        /// Returns a cVector from the application of HermiteInterpolationOrder5 (((6 * v - 15) * v + 10) * v * v * v).
        /// </summary>
        public static cVector3 HermiteInterpolationOrder5(Complex v) => new cVector3(((6 * v - 15) * v + 10) * v * v * v);
        
        /// <summary>
        /// Returns a cVector3 from component-wise application of Sqr (v * v).
        /// </summary>
        public static cVector3 Sqr(cVector3 v) => new cVector3(v.x * v.x, v.y * v.y, v.z * v.z);
        
        /// <summary>
        /// Returns a cVector from the application of Sqr (v * v).
        /// </summary>
        public static cVector3 Sqr(Complex v) => new cVector3(v * v);
        
        /// <summary>
        /// Returns a cVector3 from component-wise application of Pow2 (v * v).
        /// </summary>
        public static cVector3 Pow2(cVector3 v) => new cVector3(v.x * v.x, v.y * v.y, v.z * v.z);
        
        /// <summary>
        /// Returns a cVector from the application of Pow2 (v * v).
        /// </summary>
        public static cVector3 Pow2(Complex v) => new cVector3(v * v);
        
        /// <summary>
        /// Returns a cVector3 from component-wise application of Pow3 (v * v * v).
        /// </summary>
        public static cVector3 Pow3(cVector3 v) => new cVector3(v.x * v.x * v.x, v.y * v.y * v.y, v.z * v.z * v.z);
        
        /// <summary>
        /// Returns a cVector from the application of Pow3 (v * v * v).
        /// </summary>
        public static cVector3 Pow3(Complex v) => new cVector3(v * v * v);
        
        /// <summary>
        /// Returns a cVector3 from component-wise application of Acos (Complex.Acos(v)).
        /// </summary>
        public static cVector3 Acos(cVector3 v) => new cVector3(Complex.Acos(v.x), Complex.Acos(v.y), Complex.Acos(v.z));
        
        /// <summary>
        /// Returns a cVector from the application of Acos (Complex.Acos(v)).
        /// </summary>
        public static cVector3 Acos(Complex v) => new cVector3(Complex.Acos(v));
        
        /// <summary>
        /// Returns a cVector3 from component-wise application of Asin (Complex.Asin(v)).
        /// </summary>
        public static cVector3 Asin(cVector3 v) => new cVector3(Complex.Asin(v.x), Complex.Asin(v.y), Complex.Asin(v.z));
        
        /// <summary>
        /// Returns a cVector from the application of Asin (Complex.Asin(v)).
        /// </summary>
        public static cVector3 Asin(Complex v) => new cVector3(Complex.Asin(v));
        
        /// <summary>
        /// Returns a cVector3 from component-wise application of Atan (Complex.Atan(v)).
        /// </summary>
        public static cVector3 Atan(cVector3 v) => new cVector3(Complex.Atan(v.x), Complex.Atan(v.y), Complex.Atan(v.z));
        
        /// <summary>
        /// Returns a cVector from the application of Atan (Complex.Atan(v)).
        /// </summary>
        public static cVector3 Atan(Complex v) => new cVector3(Complex.Atan(v));
        
        /// <summary>
        /// Returns a cVector3 from component-wise application of Cos (Complex.Cos(v)).
        /// </summary>
        public static cVector3 Cos(cVector3 v) => new cVector3(Complex.Cos(v.x), Complex.Cos(v.y), Complex.Cos(v.z));
        
        /// <summary>
        /// Returns a cVector from the application of Cos (Complex.Cos(v)).
        /// </summary>
        public static cVector3 Cos(Complex v) => new cVector3(Complex.Cos(v));
        
        /// <summary>
        /// Returns a cVector3 from component-wise application of Cosh (Complex.Cosh(v)).
        /// </summary>
        public static cVector3 Cosh(cVector3 v) => new cVector3(Complex.Cosh(v.x), Complex.Cosh(v.y), Complex.Cosh(v.z));
        
        /// <summary>
        /// Returns a cVector from the application of Cosh (Complex.Cosh(v)).
        /// </summary>
        public static cVector3 Cosh(Complex v) => new cVector3(Complex.Cosh(v));
        
        /// <summary>
        /// Returns a cVector3 from component-wise application of Exp (Complex.Exp(v)).
        /// </summary>
        public static cVector3 Exp(cVector3 v) => new cVector3(Complex.Exp(v.x), Complex.Exp(v.y), Complex.Exp(v.z));
        
        /// <summary>
        /// Returns a cVector from the application of Exp (Complex.Exp(v)).
        /// </summary>
        public static cVector3 Exp(Complex v) => new cVector3(Complex.Exp(v));
        
        /// <summary>
        /// Returns a cVector3 from component-wise application of Log (Complex.Log(v)).
        /// </summary>
        public static cVector3 Log(cVector3 v) => new cVector3(Complex.Log(v.x), Complex.Log(v.y), Complex.Log(v.z));
        
        /// <summary>
        /// Returns a cVector from the application of Log (Complex.Log(v)).
        /// </summary>
        public static cVector3 Log(Complex v) => new cVector3(Complex.Log(v));
        
        /// <summary>
        /// Returns a cVector3 from component-wise application of Log2 (Complex.Log(v, 2.0)).
        /// </summary>
        public static cVector3 Log2(cVector3 v) => new cVector3(Complex.Log(v.x, 2.0), Complex.Log(v.y, 2.0), Complex.Log(v.z, 2.0));
        
        /// <summary>
        /// Returns a cVector from the application of Log2 (Complex.Log(v, 2.0)).
        /// </summary>
        public static cVector3 Log2(Complex v) => new cVector3(Complex.Log(v, 2.0));
        
        /// <summary>
        /// Returns a cVector3 from component-wise application of Log10 (Complex.Log10(v)).
        /// </summary>
        public static cVector3 Log10(cVector3 v) => new cVector3(Complex.Log10(v.x), Complex.Log10(v.y), Complex.Log10(v.z));
        
        /// <summary>
        /// Returns a cVector from the application of Log10 (Complex.Log10(v)).
        /// </summary>
        public static cVector3 Log10(Complex v) => new cVector3(Complex.Log10(v));
        
        /// <summary>
        /// Returns a cVector3 from component-wise application of Reciprocal (Complex.Reciprocal(v)).
        /// </summary>
        public static cVector3 Reciprocal(cVector3 v) => new cVector3(Complex.Reciprocal(v.x), Complex.Reciprocal(v.y), Complex.Reciprocal(v.z));
        
        /// <summary>
        /// Returns a cVector from the application of Reciprocal (Complex.Reciprocal(v)).
        /// </summary>
        public static cVector3 Reciprocal(Complex v) => new cVector3(Complex.Reciprocal(v));
        
        /// <summary>
        /// Returns a cVector3 from component-wise application of Sin (Complex.Sin(v)).
        /// </summary>
        public static cVector3 Sin(cVector3 v) => new cVector3(Complex.Sin(v.x), Complex.Sin(v.y), Complex.Sin(v.z));
        
        /// <summary>
        /// Returns a cVector from the application of Sin (Complex.Sin(v)).
        /// </summary>
        public static cVector3 Sin(Complex v) => new cVector3(Complex.Sin(v));
        
        /// <summary>
        /// Returns a cVector3 from component-wise application of Sinh (Complex.Sinh(v)).
        /// </summary>
        public static cVector3 Sinh(cVector3 v) => new cVector3(Complex.Sinh(v.x), Complex.Sinh(v.y), Complex.Sinh(v.z));
        
        /// <summary>
        /// Returns a cVector from the application of Sinh (Complex.Sinh(v)).
        /// </summary>
        public static cVector3 Sinh(Complex v) => new cVector3(Complex.Sinh(v));
        
        /// <summary>
        /// Returns a cVector3 from component-wise application of Sqrt (Complex.Sqrt(v)).
        /// </summary>
        public static cVector3 Sqrt(cVector3 v) => new cVector3(Complex.Sqrt(v.x), Complex.Sqrt(v.y), Complex.Sqrt(v.z));
        
        /// <summary>
        /// Returns a cVector from the application of Sqrt (Complex.Sqrt(v)).
        /// </summary>
        public static cVector3 Sqrt(Complex v) => new cVector3(Complex.Sqrt(v));
        
        /// <summary>
        /// Returns a cVector3 from component-wise application of InverseSqrt (Complex.One / Complex.Sqrt(v)).
        /// </summary>
        public static cVector3 InverseSqrt(cVector3 v) => new cVector3(Complex.One / Complex.Sqrt(v.x), Complex.One / Complex.Sqrt(v.y), Complex.One / Complex.Sqrt(v.z));
        
        /// <summary>
        /// Returns a cVector from the application of InverseSqrt (Complex.One / Complex.Sqrt(v)).
        /// </summary>
        public static cVector3 InverseSqrt(Complex v) => new cVector3(Complex.One / Complex.Sqrt(v));
        
        /// <summary>
        /// Returns a cVector3 from component-wise application of Tan (Complex.Tan(v)).
        /// </summary>
        public static cVector3 Tan(cVector3 v) => new cVector3(Complex.Tan(v.x), Complex.Tan(v.y), Complex.Tan(v.z));
        
        /// <summary>
        /// Returns a cVector from the application of Tan (Complex.Tan(v)).
        /// </summary>
        public static cVector3 Tan(Complex v) => new cVector3(Complex.Tan(v));
        
        /// <summary>
        /// Returns a cVector3 from component-wise application of Tanh (Complex.Tanh(v)).
        /// </summary>
        public static cVector3 Tanh(cVector3 v) => new cVector3(Complex.Tanh(v.x), Complex.Tanh(v.y), Complex.Tanh(v.z));
        
        /// <summary>
        /// Returns a cVector from the application of Tanh (Complex.Tanh(v)).
        /// </summary>
        public static cVector3 Tanh(Complex v) => new cVector3(Complex.Tanh(v));
        
        /// <summary>
        /// Returns a cVector3 from component-wise application of Conjugate (Complex.Conjugate(v)).
        /// </summary>
        public static cVector3 Conjugate(cVector3 v) => new cVector3(Complex.Conjugate(v.x), Complex.Conjugate(v.y), Complex.Conjugate(v.z));
        
        /// <summary>
        /// Returns a cVector from the application of Conjugate (Complex.Conjugate(v)).
        /// </summary>
        public static cVector3 Conjugate(Complex v) => new cVector3(Complex.Conjugate(v));
        
        /// <summary>
        /// Returns a cVector3 from component-wise application of Pow (Complex.Pow(lhs, rhs)).
        /// </summary>
        public static cVector3 Pow(cVector3 lhs, cVector3 rhs) => new cVector3(Complex.Pow(lhs.x, rhs.x), Complex.Pow(lhs.y, rhs.y), Complex.Pow(lhs.z, rhs.z));
        
        /// <summary>
        /// Returns a cVector3 from component-wise application of Pow (Complex.Pow(lhs, rhs)).
        /// </summary>
        public static cVector3 Pow(cVector3 lhs, Complex rhs) => new cVector3(Complex.Pow(lhs.x, rhs), Complex.Pow(lhs.y, rhs), Complex.Pow(lhs.z, rhs));
        
        /// <summary>
        /// Returns a cVector3 from component-wise application of Pow (Complex.Pow(lhs, rhs)).
        /// </summary>
        public static cVector3 Pow(Complex lhs, cVector3 rhs) => new cVector3(Complex.Pow(lhs, rhs.x), Complex.Pow(lhs, rhs.y), Complex.Pow(lhs, rhs.z));
        
        /// <summary>
        /// Returns a cVector from the application of Pow (Complex.Pow(lhs, rhs)).
        /// </summary>
        public static cVector3 Pow(Complex lhs, Complex rhs) => new cVector3(Complex.Pow(lhs, rhs));
        
        /// <summary>
        /// Returns a cVector3 from component-wise application of Log (Complex.Log(lhs, rhs)).
        /// </summary>
        public static cVector3 Log(cVector3 lhs, dVector3 rhs) => new cVector3(Complex.Log(lhs.x, rhs.x), Complex.Log(lhs.y, rhs.y), Complex.Log(lhs.z, rhs.z));
        
        /// <summary>
        /// Returns a cVector3 from component-wise application of Log (Complex.Log(lhs, rhs)).
        /// </summary>
        public static cVector3 Log(cVector3 lhs, double rhs) => new cVector3(Complex.Log(lhs.x, rhs), Complex.Log(lhs.y, rhs), Complex.Log(lhs.z, rhs));
        
        /// <summary>
        /// Returns a cVector3 from component-wise application of Log (Complex.Log(lhs, rhs)).
        /// </summary>
        public static cVector3 Log(Complex lhs, dVector3 rhs) => new cVector3(Complex.Log(lhs, rhs.x), Complex.Log(lhs, rhs.y), Complex.Log(lhs, rhs.z));
        
        /// <summary>
        /// Returns a cVector from the application of Log (Complex.Log(lhs, rhs)).
        /// </summary>
        public static cVector3 Log(Complex lhs, double rhs) => new cVector3(Complex.Log(lhs, rhs));
        
        /// <summary>
        /// Returns a cVector3 from component-wise application of FromPolarCoordinates (Complex.FromPolarCoordinates(magnitude, phase)).
        /// </summary>
        public static cVector3 FromPolarCoordinates(dVector3 magnitude, dVector3 phase) => new cVector3(Complex.FromPolarCoordinates(magnitude.x, phase.x), Complex.FromPolarCoordinates(magnitude.y, phase.y), Complex.FromPolarCoordinates(magnitude.z, phase.z));
        
        /// <summary>
        /// Returns a cVector3 from component-wise application of FromPolarCoordinates (Complex.FromPolarCoordinates(magnitude, phase)).
        /// </summary>
        public static cVector3 FromPolarCoordinates(dVector3 magnitude, double phase) => new cVector3(Complex.FromPolarCoordinates(magnitude.x, phase), Complex.FromPolarCoordinates(magnitude.y, phase), Complex.FromPolarCoordinates(magnitude.z, phase));
        
        /// <summary>
        /// Returns a cVector3 from component-wise application of FromPolarCoordinates (Complex.FromPolarCoordinates(magnitude, phase)).
        /// </summary>
        public static cVector3 FromPolarCoordinates(double magnitude, dVector3 phase) => new cVector3(Complex.FromPolarCoordinates(magnitude, phase.x), Complex.FromPolarCoordinates(magnitude, phase.y), Complex.FromPolarCoordinates(magnitude, phase.z));
        
        /// <summary>
        /// Returns a cVector from the application of FromPolarCoordinates (Complex.FromPolarCoordinates(magnitude, phase)).
        /// </summary>
        public static cVector3 FromPolarCoordinates(double magnitude, double phase) => new cVector3(Complex.FromPolarCoordinates(magnitude, phase));
        
        /// <summary>
        /// Returns a cVector3 from component-wise application of Fma (a * b + c).
        /// </summary>
        public static cVector3 Fma(cVector3 a, cVector3 b, cVector3 c) => new cVector3(a.x * b.x + c.x, a.y * b.y + c.y, a.z * b.z + c.z);
        
        /// <summary>
        /// Returns a cVector3 from component-wise application of Fma (a * b + c).
        /// </summary>
        public static cVector3 Fma(cVector3 a, cVector3 b, Complex c) => new cVector3(a.x * b.x + c, a.y * b.y + c, a.z * b.z + c);
        
        /// <summary>
        /// Returns a cVector3 from component-wise application of Fma (a * b + c).
        /// </summary>
        public static cVector3 Fma(cVector3 a, Complex b, cVector3 c) => new cVector3(a.x * b + c.x, a.y * b + c.y, a.z * b + c.z);
        
        /// <summary>
        /// Returns a cVector3 from component-wise application of Fma (a * b + c).
        /// </summary>
        public static cVector3 Fma(cVector3 a, Complex b, Complex c) => new cVector3(a.x * b + c, a.y * b + c, a.z * b + c);
        
        /// <summary>
        /// Returns a cVector3 from component-wise application of Fma (a * b + c).
        /// </summary>
        public static cVector3 Fma(Complex a, cVector3 b, cVector3 c) => new cVector3(a * b.x + c.x, a * b.y + c.y, a * b.z + c.z);
        
        /// <summary>
        /// Returns a cVector3 from component-wise application of Fma (a * b + c).
        /// </summary>
        public static cVector3 Fma(Complex a, cVector3 b, Complex c) => new cVector3(a * b.x + c, a * b.y + c, a * b.z + c);
        
        /// <summary>
        /// Returns a cVector3 from component-wise application of Fma (a * b + c).
        /// </summary>
        public static cVector3 Fma(Complex a, Complex b, cVector3 c) => new cVector3(a * b + c.x, a * b + c.y, a * b + c.z);
        
        /// <summary>
        /// Returns a cVector from the application of Fma (a * b + c).
        /// </summary>
        public static cVector3 Fma(Complex a, Complex b, Complex c) => new cVector3(a * b + c);
        
        /// <summary>
        /// Returns a cVector3 from component-wise application of Add (lhs + rhs).
        /// </summary>
        public static cVector3 Add(cVector3 lhs, cVector3 rhs) => new cVector3(lhs.x + rhs.x, lhs.y + rhs.y, lhs.z + rhs.z);
        
        /// <summary>
        /// Returns a cVector3 from component-wise application of Add (lhs + rhs).
        /// </summary>
        public static cVector3 Add(cVector3 lhs, Complex rhs) => new cVector3(lhs.x + rhs, lhs.y + rhs, lhs.z + rhs);
        
        /// <summary>
        /// Returns a cVector3 from component-wise application of Add (lhs + rhs).
        /// </summary>
        public static cVector3 Add(Complex lhs, cVector3 rhs) => new cVector3(lhs + rhs.x, lhs + rhs.y, lhs + rhs.z);
        
        /// <summary>
        /// Returns a cVector from the application of Add (lhs + rhs).
        /// </summary>
        public static cVector3 Add(Complex lhs, Complex rhs) => new cVector3(lhs + rhs);
        
        /// <summary>
        /// Returns a cVector3 from component-wise application of Sub (lhs - rhs).
        /// </summary>
        public static cVector3 Sub(cVector3 lhs, cVector3 rhs) => new cVector3(lhs.x - rhs.x, lhs.y - rhs.y, lhs.z - rhs.z);
        
        /// <summary>
        /// Returns a cVector3 from component-wise application of Sub (lhs - rhs).
        /// </summary>
        public static cVector3 Sub(cVector3 lhs, Complex rhs) => new cVector3(lhs.x - rhs, lhs.y - rhs, lhs.z - rhs);
        
        /// <summary>
        /// Returns a cVector3 from component-wise application of Sub (lhs - rhs).
        /// </summary>
        public static cVector3 Sub(Complex lhs, cVector3 rhs) => new cVector3(lhs - rhs.x, lhs - rhs.y, lhs - rhs.z);
        
        /// <summary>
        /// Returns a cVector from the application of Sub (lhs - rhs).
        /// </summary>
        public static cVector3 Sub(Complex lhs, Complex rhs) => new cVector3(lhs - rhs);
        
        /// <summary>
        /// Returns a cVector3 from component-wise application of Mul (lhs * rhs).
        /// </summary>
        public static cVector3 Mul(cVector3 lhs, cVector3 rhs) => new cVector3(lhs.x * rhs.x, lhs.y * rhs.y, lhs.z * rhs.z);
        
        /// <summary>
        /// Returns a cVector3 from component-wise application of Mul (lhs * rhs).
        /// </summary>
        public static cVector3 Mul(cVector3 lhs, Complex rhs) => new cVector3(lhs.x * rhs, lhs.y * rhs, lhs.z * rhs);
        
        /// <summary>
        /// Returns a cVector3 from component-wise application of Mul (lhs * rhs).
        /// </summary>
        public static cVector3 Mul(Complex lhs, cVector3 rhs) => new cVector3(lhs * rhs.x, lhs * rhs.y, lhs * rhs.z);
        
        /// <summary>
        /// Returns a cVector from the application of Mul (lhs * rhs).
        /// </summary>
        public static cVector3 Mul(Complex lhs, Complex rhs) => new cVector3(lhs * rhs);
        
        /// <summary>
        /// Returns a cVector3 from component-wise application of Div (lhs / rhs).
        /// </summary>
        public static cVector3 Div(cVector3 lhs, cVector3 rhs) => new cVector3(lhs.x / rhs.x, lhs.y / rhs.y, lhs.z / rhs.z);
        
        /// <summary>
        /// Returns a cVector3 from component-wise application of Div (lhs / rhs).
        /// </summary>
        public static cVector3 Div(cVector3 lhs, Complex rhs) => new cVector3(lhs.x / rhs, lhs.y / rhs, lhs.z / rhs);
        
        /// <summary>
        /// Returns a cVector3 from component-wise application of Div (lhs / rhs).
        /// </summary>
        public static cVector3 Div(Complex lhs, cVector3 rhs) => new cVector3(lhs / rhs.x, lhs / rhs.y, lhs / rhs.z);
        
        /// <summary>
        /// Returns a cVector from the application of Div (lhs / rhs).
        /// </summary>
        public static cVector3 Div(Complex lhs, Complex rhs) => new cVector3(lhs / rhs);

        #endregion


        #region Component-Wise Operator Overloads
        
        /// <summary>
        /// Returns a cVector3 from component-wise application of operator+ (lhs + rhs).
        /// </summary>
        public static cVector3 operator+(cVector3 lhs, cVector3 rhs) => new cVector3(lhs.x + rhs.x, lhs.y + rhs.y, lhs.z + rhs.z);
        
        /// <summary>
        /// Returns a cVector3 from component-wise application of operator+ (lhs + rhs).
        /// </summary>
        public static cVector3 operator+(cVector3 lhs, Complex rhs) => new cVector3(lhs.x + rhs, lhs.y + rhs, lhs.z + rhs);
        
        /// <summary>
        /// Returns a cVector3 from component-wise application of operator+ (lhs + rhs).
        /// </summary>
        public static cVector3 operator+(Complex lhs, cVector3 rhs) => new cVector3(lhs + rhs.x, lhs + rhs.y, lhs + rhs.z);
        
        /// <summary>
        /// Returns a cVector3 from component-wise application of operator- (lhs - rhs).
        /// </summary>
        public static cVector3 operator-(cVector3 lhs, cVector3 rhs) => new cVector3(lhs.x - rhs.x, lhs.y - rhs.y, lhs.z - rhs.z);
        
        /// <summary>
        /// Returns a cVector3 from component-wise application of operator- (lhs - rhs).
        /// </summary>
        public static cVector3 operator-(cVector3 lhs, Complex rhs) => new cVector3(lhs.x - rhs, lhs.y - rhs, lhs.z - rhs);
        
        /// <summary>
        /// Returns a cVector3 from component-wise application of operator- (lhs - rhs).
        /// </summary>
        public static cVector3 operator-(Complex lhs, cVector3 rhs) => new cVector3(lhs - rhs.x, lhs - rhs.y, lhs - rhs.z);
        
        /// <summary>
        /// Returns a cVector3 from component-wise application of operator* (lhs * rhs).
        /// </summary>
        public static cVector3 operator*(cVector3 lhs, cVector3 rhs) => new cVector3(lhs.x * rhs.x, lhs.y * rhs.y, lhs.z * rhs.z);
        
        /// <summary>
        /// Returns a cVector3 from component-wise application of operator* (lhs * rhs).
        /// </summary>
        public static cVector3 operator*(cVector3 lhs, Complex rhs) => new cVector3(lhs.x * rhs, lhs.y * rhs, lhs.z * rhs);
        
        /// <summary>
        /// Returns a cVector3 from component-wise application of operator* (lhs * rhs).
        /// </summary>
        public static cVector3 operator*(Complex lhs, cVector3 rhs) => new cVector3(lhs * rhs.x, lhs * rhs.y, lhs * rhs.z);
        
        /// <summary>
        /// Returns a cVector3 from component-wise application of operator/ (lhs / rhs).
        /// </summary>
        public static cVector3 operator/(cVector3 lhs, cVector3 rhs) => new cVector3(lhs.x / rhs.x, lhs.y / rhs.y, lhs.z / rhs.z);
        
        /// <summary>
        /// Returns a cVector3 from component-wise application of operator/ (lhs / rhs).
        /// </summary>
        public static cVector3 operator/(cVector3 lhs, Complex rhs) => new cVector3(lhs.x / rhs, lhs.y / rhs, lhs.z / rhs);
        
        /// <summary>
        /// Returns a cVector3 from component-wise application of operator/ (lhs / rhs).
        /// </summary>
        public static cVector3 operator/(Complex lhs, cVector3 rhs) => new cVector3(lhs / rhs.x, lhs / rhs.y, lhs / rhs.z);
        
        /// <summary>
        /// Returns a cVector3 from component-wise application of operator+ (identity).
        /// </summary>
        public static cVector3 operator+(cVector3 v) => v;
        
        /// <summary>
        /// Returns a cVector3 from component-wise application of operator- (-v).
        /// </summary>
        public static cVector3 operator-(cVector3 v) => new cVector3(-v.x, -v.y, -v.z);

        #endregion

    }
    */
}
