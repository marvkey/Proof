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
    /// A Vectortor of type Complex with 4 components.
    /// </summary>
    [RegisterCoreClassStruct]
    
    [StructLayout(LayoutKind.Sequential)]
    public struct cVector4 : IReadOnlyList<Complex>, IEquatable<cVector4>
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
        
        /// <summary>
        /// w-component
        /// </summary>
        
        public Complex w;

        #endregion


        #region Constructors
        
        /// <summary>
        /// Component-wise constructor
        /// </summary>
        public cVector4(Complex x, Complex y, Complex z, Complex w)
        {
            this.x = x;
            this.y = y;
            this.z = z;
            this.w = w;
        }
        
        /// <summary>
        /// all-same-value constructor
        /// </summary>
        public cVector4(Complex v)
        {
            this.x = v;
            this.y = v;
            this.z = v;
            this.w = v;
        }
        
        /// <summary>
        /// from-Vectortor constructor (empty fields are zero/false)
        /// </summary>
        public cVector4(cVector2 v)
        {
            this.x = v.x;
            this.y = v.y;
            this.z = Complex.Zero;
            this.w = Complex.Zero;
        }
        
        /// <summary>
        /// from-Vectortor-and-value constructor (empty fields are zero/false)
        /// </summary>
        public cVector4(cVector2 v, Complex z)
        {
            this.x = v.x;
            this.y = v.y;
            this.z = z;
            this.w = Complex.Zero;
        }
        
        /// <summary>
        /// from-Vectortor-and-value constructor
        /// </summary>
        public cVector4(cVector2 v, Complex z, Complex w)
        {
            this.x = v.x;
            this.y = v.y;
            this.z = z;
            this.w = w;
        }
        
        /// <summary>
        /// from-Vectortor constructor (empty fields are zero/false)
        /// </summary>
        public cVector4(cVector3 v)
        {
            this.x = v.x;
            this.y = v.y;
            this.z = v.z;
            this.w = Complex.Zero;
        }
        
        /// <summary>
        /// from-Vectortor-and-value constructor
        /// </summary>
        public cVector4(cVector3 v, Complex w)
        {
            this.x = v.x;
            this.y = v.y;
            this.z = v.z;
            this.w = w;
        }
        
        /// <summary>
        /// from-Vectortor constructor
        /// </summary>
        public cVector4(cVector4 v)
        {
            this.x = v.x;
            this.y = v.y;
            this.z = v.z;
            this.w = v.w;
        }
        
        /// <summary>
        /// From-array/list constructor (superfluous values are ignored, missing values are zero-filled).
        /// </summary>
        public cVector4(IReadOnlyList<Complex> v)
        {
            var c = v.Count;
            this.x = c < 0 ? Complex.Zero : v[0];
            this.y = c < 1 ? Complex.Zero : v[1];
            this.z = c < 2 ? Complex.Zero : v[2];
            this.w = c < 3 ? Complex.Zero : v[3];
        }
        
        /// <summary>
        /// Generic from-array constructor (superfluous values are ignored, missing values are zero-filled).
        /// </summary>
        public cVector4(Object[] v)
        {
            var c = v.Length;
            this.x = c < 0 ? Complex.Zero : (Complex)v[0];
            this.y = c < 1 ? Complex.Zero : (Complex)v[1];
            this.z = c < 2 ? Complex.Zero : (Complex)v[2];
            this.w = c < 3 ? Complex.Zero : (Complex)v[3];
        }
        
        /// <summary>
        /// From-array constructor (superfluous values are ignored, missing values are zero-filled).
        /// </summary>
        public cVector4(Complex[] v)
        {
            var c = v.Length;
            this.x = c < 0 ? Complex.Zero : v[0];
            this.y = c < 1 ? Complex.Zero : v[1];
            this.z = c < 2 ? Complex.Zero : v[2];
            this.w = c < 3 ? Complex.Zero : v[3];
        }
        
        /// <summary>
        /// From-array constructor with base index (superfluous values are ignored, missing values are zero-filled).
        /// </summary>
        public cVector4(Complex[] v, int startIndex)
        {
            var c = v.Length;
            this.x = c + startIndex < 0 ? Complex.Zero : v[0 + startIndex];
            this.y = c + startIndex < 1 ? Complex.Zero : v[1 + startIndex];
            this.z = c + startIndex < 2 ? Complex.Zero : v[2 + startIndex];
            this.w = c + startIndex < 3 ? Complex.Zero : v[3 + startIndex];
        }
        
        /// <summary>
        /// From-IEnumerable constructor (superfluous values are ignored, missing values are zero-filled).
        /// </summary>
        public cVector4(IEnumerable<Complex> v)
            : this(v.ToArray())
        {
        }

        #endregion


        #region Explicit Operators
        
        /// <summary>
        /// Explicitly converts this to a cVector2.
        /// </summary>
        public static explicit operator cVector2(cVector4 v) => new cVector2((Complex)v.x, (Complex)v.y);
        
        /// <summary>
        /// Explicitly converts this to a cVector3.
        /// </summary>
        public static explicit operator cVector3(cVector4 v) => new cVector3((Complex)v.x, (Complex)v.y, (Complex)v.z);
        
        /// <summary>
        /// Explicitly converts this to a Complex array.
        /// </summary>
        public static explicit operator Complex[](cVector4 v) => new [] { v.x, v.y, v.z, v.w };
        
        /// <summary>
        /// Explicitly converts this to a generic object array.
        /// </summary>
        public static explicit operator Object[](cVector4 v) => new Object[] { v.x, v.y, v.z, v.w };

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
                    case 3: return w;
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
                    case 3: w = value; break;
                    default: throw new ArgumentOutOfRangeException("index");
                }
            }
        }

        #endregion


        #region Properties
        
        /// <summary>
        /// Returns an object that can be used for arbitrary swizzling (e.g. swizzle.zy)
        /// </summary>
        public swizzle_cVector4 swizzle => new swizzle_cVector4(x, y, z, w);
        
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
        public cVector2 xw
        {
            get
            {
                return new cVector2(x, w);
            }
            set
            {
                x = value.x;
                w = value.y;
            }
        }
        
        /// <summary>
        /// Gets or sets the specified subset of components. For more advanced (read-only) swizzling, use the .swizzle property.
        /// </summary>
        public cVector2 yw
        {
            get
            {
                return new cVector2(y, w);
            }
            set
            {
                y = value.x;
                w = value.y;
            }
        }
        
        /// <summary>
        /// Gets or sets the specified subset of components. For more advanced (read-only) swizzling, use the .swizzle property.
        /// </summary>
        public cVector3 xyw
        {
            get
            {
                return new cVector3(x, y, w);
            }
            set
            {
                x = value.x;
                y = value.y;
                w = value.z;
            }
        }
        
        /// <summary>
        /// Gets or sets the specified subset of components. For more advanced (read-only) swizzling, use the .swizzle property.
        /// </summary>
        public cVector2 zw
        {
            get
            {
                return new cVector2(z, w);
            }
            set
            {
                z = value.x;
                w = value.y;
            }
        }
        
        /// <summary>
        /// Gets or sets the specified subset of components. For more advanced (read-only) swizzling, use the .swizzle property.
        /// </summary>
        public cVector3 xzw
        {
            get
            {
                return new cVector3(x, z, w);
            }
            set
            {
                x = value.x;
                z = value.y;
                w = value.z;
            }
        }
        
        /// <summary>
        /// Gets or sets the specified subset of components. For more advanced (read-only) swizzling, use the .swizzle property.
        /// </summary>
        public cVector3 yzw
        {
            get
            {
                return new cVector3(y, z, w);
            }
            set
            {
                y = value.x;
                z = value.y;
                w = value.z;
            }
        }
        
        /// <summary>
        /// Gets or sets the specified subset of components. For more advanced (read-only) swizzling, use the .swizzle property.
        /// </summary>
        public cVector4 xyzw
        {
            get
            {
                return new cVector4(x, y, z, w);
            }
            set
            {
                x = value.x;
                y = value.y;
                z = value.z;
                w = value.w;
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
        /// Gets or sets the specified subset of components. For more advanced (read-only) swizzling, use the .swizzle property.
        /// </summary>
        public cVector2 ra
        {
            get
            {
                return new cVector2(x, w);
            }
            set
            {
                x = value.x;
                w = value.y;
            }
        }
        
        /// <summary>
        /// Gets or sets the specified subset of components. For more advanced (read-only) swizzling, use the .swizzle property.
        /// </summary>
        public cVector2 ga
        {
            get
            {
                return new cVector2(y, w);
            }
            set
            {
                y = value.x;
                w = value.y;
            }
        }
        
        /// <summary>
        /// Gets or sets the specified subset of components. For more advanced (read-only) swizzling, use the .swizzle property.
        /// </summary>
        public cVector3 rga
        {
            get
            {
                return new cVector3(x, y, w);
            }
            set
            {
                x = value.x;
                y = value.y;
                w = value.z;
            }
        }
        
        /// <summary>
        /// Gets or sets the specified subset of components. For more advanced (read-only) swizzling, use the .swizzle property.
        /// </summary>
        public cVector2 ba
        {
            get
            {
                return new cVector2(z, w);
            }
            set
            {
                z = value.x;
                w = value.y;
            }
        }
        
        /// <summary>
        /// Gets or sets the specified subset of components. For more advanced (read-only) swizzling, use the .swizzle property.
        /// </summary>
        public cVector3 rba
        {
            get
            {
                return new cVector3(x, z, w);
            }
            set
            {
                x = value.x;
                z = value.y;
                w = value.z;
            }
        }
        
        /// <summary>
        /// Gets or sets the specified subset of components. For more advanced (read-only) swizzling, use the .swizzle property.
        /// </summary>
        public cVector3 gba
        {
            get
            {
                return new cVector3(y, z, w);
            }
            set
            {
                y = value.x;
                z = value.y;
                w = value.z;
            }
        }
        
        /// <summary>
        /// Gets or sets the specified subset of components. For more advanced (read-only) swizzling, use the .swizzle property.
        /// </summary>
        public cVector4 rgba
        {
            get
            {
                return new cVector4(x, y, z, w);
            }
            set
            {
                x = value.x;
                y = value.y;
                z = value.z;
                w = value.w;
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
        /// Gets or sets the specified RGBA component. For more advanced (read-only) swizzling, use the .swizzle property.
        /// </summary>
        public Complex a
        {
            get
            {
                return w;
            }
            set
            {
                w = value;
            }
        }
        
        /// <summary>
        /// Returns an array with all values
        /// </summary>
        public Complex[] Values => new[] { x, y, z, w };
        
        /// <summary>
        /// Returns the number of components (4).
        /// </summary>
        public int Count => 4;
        
        /// <summary>
        /// Returns the euclidean length of this Vectortor.
        /// </summary>
        public double Length => (double)Math.Sqrt(((x.LengthSqr() + y.LengthSqr()) + (z.LengthSqr() + w.LengthSqr())));
        
        /// <summary>
        /// Returns the squared euclidean length of this Vectortor.
        /// </summary>
        public double LengthSqr => ((x.LengthSqr() + y.LengthSqr()) + (z.LengthSqr() + w.LengthSqr()));
        
        /// <summary>
        /// Returns the sum of all components.
        /// </summary>
        public Complex Sum => ((x + y) + (z + w));
        
        /// <summary>
        /// Returns the euclidean norm of this Vectortor.
        /// </summary>
        public double Norm => (double)Math.Sqrt(((x.LengthSqr() + y.LengthSqr()) + (z.LengthSqr() + w.LengthSqr())));
        
        /// <summary>
        /// Returns the one-norm of this Vectortor.
        /// </summary>
        public double Norm1 => ((x.Magnitude + y.Magnitude) + (z.Magnitude + w.Magnitude));
        
        /// <summary>
        /// Returns the two-norm (euclidean length) of this Vectortor.
        /// </summary>
        public double Norm2 => (double)Math.Sqrt(((x.LengthSqr() + y.LengthSqr()) + (z.LengthSqr() + w.LengthSqr())));
        
        /// <summary>
        /// Returns the max-norm of this Vectortor.
        /// </summary>
        public double NormMax => Math.Max(Math.Max(x.Magnitude, y.Magnitude), Math.Max(z.Magnitude, w.Magnitude));
        
        /// <summary>
        /// Returns a copy of this Vectortor with length one (undefined if this has zero length).
        /// </summary>
        public cVector4 Normalized => this / (Complex)Length;
        
        /// <summary>
        /// Returns a copy of this Vectortor with length one (returns zero if length is zero).
        /// </summary>
        public cVector4 NormalizedSafe => this == Zero ? Zero : this / (Complex)Length;
        
        /// <summary>
        /// Returns a Vectortor containing component-wise magnitudes.
        /// </summary>
        public dVector4 Magnitude => new dVector4(x.Magnitude, y.Magnitude, z.Magnitude, w.Magnitude);
        
        /// <summary>
        /// Returns a Vectortor containing component-wise phases.
        /// </summary>
        public dVector4 Phase => new dVector4(x.Phase, y.Phase, z.Phase, w.Phase);
        
        /// <summary>
        /// Returns a Vectortor containing component-wise imaginary parts.
        /// </summary>
        public dVector4 Imaginary => new dVector4(x.Imaginary, y.Imaginary, z.Imaginary, w.Imaginary);
        
        /// <summary>
        /// Returns a Vectortor containing component-wise real parts.
        /// </summary>
        public dVector4 Real => new dVector4(x.Real, y.Real, z.Real, w.Real);

        #endregion


        #region Static Properties
        
        /// <summary>
        /// Predefined all-zero Vectortor
        /// </summary>
        public static cVector4 Zero { get; } = new cVector4(Complex.Zero, Complex.Zero, Complex.Zero, Complex.Zero);
        
        /// <summary>
        /// Predefined all-ones Vectortor
        /// </summary>
        public static cVector4 Ones { get; } = new cVector4(Complex.One, Complex.One, Complex.One, Complex.One);
        
        /// <summary>
        /// Predefined unit-X Vectortor
        /// </summary>
        public static cVector4 UnitX { get; } = new cVector4(Complex.One, Complex.Zero, Complex.Zero, Complex.Zero);
        
        /// <summary>
        /// Predefined unit-Y Vectortor
        /// </summary>
        public static cVector4 UnitY { get; } = new cVector4(Complex.Zero, Complex.One, Complex.Zero, Complex.Zero);
        
        /// <summary>
        /// Predefined unit-Z Vectortor
        /// </summary>
        public static cVector4 UnitZ { get; } = new cVector4(Complex.Zero, Complex.Zero, Complex.One, Complex.Zero);
        
        /// <summary>
        /// Predefined unit-W Vectortor
        /// </summary>
        public static cVector4 UnitW { get; } = new cVector4(Complex.Zero, Complex.Zero, Complex.Zero, Complex.One);
        
        /// <summary>
        /// Predefined all-imaginary-ones Vectortor
        /// </summary>
        public static cVector4 ImaginaryOnes { get; } = new cVector4(Complex.ImaginaryOne, Complex.ImaginaryOne, Complex.ImaginaryOne, Complex.ImaginaryOne);
        
        /// <summary>
        /// Predefined unit-imaginary-X Vectortor
        /// </summary>
        public static cVector4 ImaginaryUnitX { get; } = new cVector4(Complex.ImaginaryOne, Complex.Zero, Complex.Zero, Complex.Zero);
        
        /// <summary>
        /// Predefined unit-imaginary-Y Vectortor
        /// </summary>
        public static cVector4 ImaginaryUnitY { get; } = new cVector4(Complex.Zero, Complex.ImaginaryOne, Complex.Zero, Complex.Zero);
        
        /// <summary>
        /// Predefined unit-imaginary-Z Vectortor
        /// </summary>
        public static cVector4 ImaginaryUnitZ { get; } = new cVector4(Complex.Zero, Complex.Zero, Complex.ImaginaryOne, Complex.Zero);
        
        /// <summary>
        /// Predefined unit-imaginary-W Vectortor
        /// </summary>
        public static cVector4 ImaginaryUnitW { get; } = new cVector4(Complex.Zero, Complex.Zero, Complex.Zero, Complex.ImaginaryOne);

        #endregion


        #region Operators
        
        /// <summary>
        /// Returns true iff this equals rhs component-wise.
        /// </summary>
        public static bool operator==(cVector4 lhs, cVector4 rhs) => lhs.Equals(rhs);
        
        /// <summary>
        /// Returns true iff this does not equal rhs (component-wise).
        /// </summary>
        public static bool operator!=(cVector4 lhs, cVector4 rhs) => !lhs.Equals(rhs);

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
            yield return w;
        }
        
        /// <summary>
        /// Returns an enumerator that iterates through all components.
        /// </summary>
        IEnumerator IEnumerable.GetEnumerator() => GetEnumerator();
        
        /// <summary>
        /// Returns a string representation of this Vectortor using ', ' as a seperator.
        /// </summary>
        public override string ToString() => ToString(", ");
        
        /// <summary>
        /// Returns a string representation of this Vectortor using a provided seperator.
        /// </summary>
        public string ToString(string sep) => ((x + sep + y) + sep + (z + sep + w));
        
        /// <summary>
        /// Returns a string representation of this Vectortor using a provided seperator and a forMatrix provider for each component.
        /// </summary>
        public string ToString(string sep, IFormatProvider provider) => ((x.ToString(provider) + sep + y.ToString(provider)) + sep + (z.ToString(provider) + sep + w.ToString(provider)));
        
        /// <summary>
        /// Returns a string representation of this Vectortor using a provided seperator and a forMatrix for each component.
        /// </summary>
        public string ToString(string sep, string forMatrix) => ((x.ToString(forMatrix) + sep + y.ToString(forMatrix)) + sep + (z.ToString(forMatrix) + sep + w.ToString(forMatrix)));
        
        /// <summary>
        /// Returns a string representation of this Vectortor using a provided seperator and a forMatrix and forMatrix provider for each component.
        /// </summary>
        public string ToString(string sep, string forMatrix, IFormatProvider provider) => ((x.ToString(forMatrix, provider) + sep + y.ToString(forMatrix, provider)) + sep + (z.ToString(forMatrix, provider) + sep + w.ToString(forMatrix, provider)));
        
        /// <summary>
        /// Returns true iff this equals rhs component-wise.
        /// </summary>
        public bool Equals(cVector4 rhs) => ((x.Equals(rhs.x) && y.Equals(rhs.y)) && (z.Equals(rhs.z) && w.Equals(rhs.w)));
        
        /// <summary>
        /// Returns true iff this equals rhs type- and component-wise.
        /// </summary>
        public override bool Equals(object obj)
        {
            if (ReferenceEquals(null, obj)) return false;
            return obj is cVector4 && Equals((cVector4) obj);
        }
        
        /// <summary>
        /// Returns a hash code for this instance.
        /// </summary>
        public override int GetHashCode()
        {
            unchecked
            {
                return ((((((x.GetHashCode()) * 397) ^ y.GetHashCode()) * 397) ^ z.GetHashCode()) * 397) ^ w.GetHashCode();
            }
        }
        
        /// <summary>
        /// Returns the p-norm of this Vectortor.
        /// </summary>
        public double NormP(double p) => Math.Pow(((Math.Pow((double)x.Magnitude, p) + Math.Pow((double)y.Magnitude, p)) + (Math.Pow((double)z.Magnitude, p) + Math.Pow((double)w.Magnitude, p))), 1 / p);

        #endregion


        #region Static Functions
        
        /// <summary>
        /// Returns true iff distance between lhs and rhs is less than or equal to epsilon
        /// </summary>
        public static bool ApproxEqual(cVector4 lhs, cVector4 rhs, double eps = 0.1d) => Distance(lhs, rhs) <= eps;
        
        /// <summary>
        /// OuterProduct treats the first parameter c as a column Vectortor (Matrix with one column) and the second parameter r as a row Vectortor (Matrix with one row) and does a linear algebraic Matrix multiply c * r, yielding a Matrix whose number of rows is the number of components in c and whose number of columns is the number of components in r.
        /// </summary>
        public static cMatrix4x2 OuterProduct(cVector2 c, cVector4 r) => new cMatrix4x2(c.x * r.x, c.y * r.x, c.x * r.y, c.y * r.y, c.x * r.z, c.y * r.z, c.x * r.w, c.y * r.w);
        
        /// <summary>
        /// OuterProduct treats the first parameter c as a column Vectortor (Matrix with one column) and the second parameter r as a row Vectortor (Matrix with one row) and does a linear algebraic Matrix multiply c * r, yielding a Matrix whose number of rows is the number of components in c and whose number of columns is the number of components in r.
        /// </summary>
        public static cMatrix2x4 OuterProduct(cVector4 c, cVector2 r) => new cMatrix2x4(c.x * r.x, c.y * r.x, c.z * r.x, c.w * r.x, c.x * r.y, c.y * r.y, c.z * r.y, c.w * r.y);
        
        /// <summary>
        /// OuterProduct treats the first parameter c as a column Vectortor (Matrix with one column) and the second parameter r as a row Vectortor (Matrix with one row) and does a linear algebraic Matrix multiply c * r, yielding a Matrix whose number of rows is the number of components in c and whose number of columns is the number of components in r.
        /// </summary>
        public static cMatrix4x3 OuterProduct(cVector3 c, cVector4 r) => new cMatrix4x3(c.x * r.x, c.y * r.x, c.z * r.x, c.x * r.y, c.y * r.y, c.z * r.y, c.x * r.z, c.y * r.z, c.z * r.z, c.x * r.w, c.y * r.w, c.z * r.w);
        
        /// <summary>
        /// OuterProduct treats the first parameter c as a column Vectortor (Matrix with one column) and the second parameter r as a row Vectortor (Matrix with one row) and does a linear algebraic Matrix multiply c * r, yielding a Matrix whose number of rows is the number of components in c and whose number of columns is the number of components in r.
        /// </summary>
        public static cMatrix3x4 OuterProduct(cVector4 c, cVector3 r) => new cMatrix3x4(c.x * r.x, c.y * r.x, c.z * r.x, c.w * r.x, c.x * r.y, c.y * r.y, c.z * r.y, c.w * r.y, c.x * r.z, c.y * r.z, c.z * r.z, c.w * r.z);
        
        /// <summary>
        /// OuterProduct treats the first parameter c as a column Vectortor (Matrix with one column) and the second parameter r as a row Vectortor (Matrix with one row) and does a linear algebraic Matrix multiply c * r, yielding a Matrix whose number of rows is the number of components in c and whose number of columns is the number of components in r.
        /// </summary>
        public static cMatrix4 OuterProduct(cVector4 c, cVector4 r) => new cMatrix4(c.x * r.x, c.y * r.x, c.z * r.x, c.w * r.x, c.x * r.y, c.y * r.y, c.z * r.y, c.w * r.y, c.x * r.z, c.y * r.z, c.z * r.z, c.w * r.z, c.x * r.w, c.y * r.w, c.z * r.w, c.w * r.w);
        
        /// <summary>
        /// Returns the inner product (dot product, scalar product) of the two Vectortors.
        /// </summary>
        public static Complex Dot(cVector4 lhs, cVector4 rhs) => ((lhs.x * Complex.Conjugate(rhs.x) + lhs.y * Complex.Conjugate(rhs.y)) + (lhs.z * Complex.Conjugate(rhs.z) + lhs.w * Complex.Conjugate(rhs.w)));
        
        /// <summary>
        /// Returns the euclidean distance between the two Vectortors.
        /// </summary>
        public static double Distance(cVector4 lhs, cVector4 rhs) => (lhs - rhs).Length;
        
        /// <summary>
        /// Returns the squared euclidean distance between the two Vectortors.
        /// </summary>
        public static double DistanceSqr(cVector4 lhs, cVector4 rhs) => (lhs - rhs).LengthSqr;
        
        /// <summary>
        /// Calculate the reflection direction for an incident Vectortor (N should be normalized in order to achieve the desired result).
        /// </summary>
        public static cVector4 Reflect(cVector4 I, cVector4 N) => I - 2 * Dot(N, I) * N;

        #endregion


        #region Component-Wise Static Functions
        
        /// <summary>
        /// Returns a bVector4 from component-wise application of Equal (lhs == rhs).
        /// </summary>
        public static bVector4 Equal(cVector4 lhs, cVector4 rhs) => new bVector4(lhs.x == rhs.x, lhs.y == rhs.y, lhs.z == rhs.z, lhs.w == rhs.w);
        
        /// <summary>
        /// Returns a bVector4 from component-wise application of Equal (lhs == rhs).
        /// </summary>
        public static bVector4 Equal(cVector4 lhs, Complex rhs) => new bVector4(lhs.x == rhs, lhs.y == rhs, lhs.z == rhs, lhs.w == rhs);
        
        /// <summary>
        /// Returns a bVector4 from component-wise application of Equal (lhs == rhs).
        /// </summary>
        public static bVector4 Equal(Complex lhs, cVector4 rhs) => new bVector4(lhs == rhs.x, lhs == rhs.y, lhs == rhs.z, lhs == rhs.w);
        
        /// <summary>
        /// Returns a bVector from the application of Equal (lhs == rhs).
        /// </summary>
        public static bVector4 Equal(Complex lhs, Complex rhs) => new bVector4(lhs == rhs);
        
        /// <summary>
        /// Returns a bVector4 from component-wise application of NotEqual (lhs != rhs).
        /// </summary>
        public static bVector4 NotEqual(cVector4 lhs, cVector4 rhs) => new bVector4(lhs.x != rhs.x, lhs.y != rhs.y, lhs.z != rhs.z, lhs.w != rhs.w);
        
        /// <summary>
        /// Returns a bVector4 from component-wise application of NotEqual (lhs != rhs).
        /// </summary>
        public static bVector4 NotEqual(cVector4 lhs, Complex rhs) => new bVector4(lhs.x != rhs, lhs.y != rhs, lhs.z != rhs, lhs.w != rhs);
        
        /// <summary>
        /// Returns a bVector4 from component-wise application of NotEqual (lhs != rhs).
        /// </summary>
        public static bVector4 NotEqual(Complex lhs, cVector4 rhs) => new bVector4(lhs != rhs.x, lhs != rhs.y, lhs != rhs.z, lhs != rhs.w);
        
        /// <summary>
        /// Returns a bVector from the application of NotEqual (lhs != rhs).
        /// </summary>
        public static bVector4 NotEqual(Complex lhs, Complex rhs) => new bVector4(lhs != rhs);
        
        /// <summary>
        /// Returns a dVector4 from component-wise application of Abs (v.Magnitude).
        /// </summary>
        public static dVector4 Abs(cVector4 v) => new dVector4(v.x.Magnitude, v.y.Magnitude, v.z.Magnitude, v.w.Magnitude);
        
        /// <summary>
        /// Returns a dVector from the application of Abs (v.Magnitude).
        /// </summary>
        public static dVector4 Abs(Complex v) => new dVector4(v.Magnitude);
        
        /// <summary>
        /// Returns a cVector4 from component-wise application of HermiteInterpolationOrder3 ((3 - 2 * v) * v * v).
        /// </summary>
        public static cVector4 HermiteInterpolationOrder3(cVector4 v) => new cVector4((3 - 2 * v.x) * v.x * v.x, (3 - 2 * v.y) * v.y * v.y, (3 - 2 * v.z) * v.z * v.z, (3 - 2 * v.w) * v.w * v.w);
        
        /// <summary>
        /// Returns a cVector from the application of HermiteInterpolationOrder3 ((3 - 2 * v) * v * v).
        /// </summary>
        public static cVector4 HermiteInterpolationOrder3(Complex v) => new cVector4((3 - 2 * v) * v * v);
        
        /// <summary>
        /// Returns a cVector4 from component-wise application of HermiteInterpolationOrder5 (((6 * v - 15) * v + 10) * v * v * v).
        /// </summary>
        public static cVector4 HermiteInterpolationOrder5(cVector4 v) => new cVector4(((6 * v.x - 15) * v.x + 10) * v.x * v.x * v.x, ((6 * v.y - 15) * v.y + 10) * v.y * v.y * v.y, ((6 * v.z - 15) * v.z + 10) * v.z * v.z * v.z, ((6 * v.w - 15) * v.w + 10) * v.w * v.w * v.w);
        
        /// <summary>
        /// Returns a cVector from the application of HermiteInterpolationOrder5 (((6 * v - 15) * v + 10) * v * v * v).
        /// </summary>
        public static cVector4 HermiteInterpolationOrder5(Complex v) => new cVector4(((6 * v - 15) * v + 10) * v * v * v);
        
        /// <summary>
        /// Returns a cVector4 from component-wise application of Sqr (v * v).
        /// </summary>
        public static cVector4 Sqr(cVector4 v) => new cVector4(v.x * v.x, v.y * v.y, v.z * v.z, v.w * v.w);
        
        /// <summary>
        /// Returns a cVector from the application of Sqr (v * v).
        /// </summary>
        public static cVector4 Sqr(Complex v) => new cVector4(v * v);
        
        /// <summary>
        /// Returns a cVector4 from component-wise application of Pow2 (v * v).
        /// </summary>
        public static cVector4 Pow2(cVector4 v) => new cVector4(v.x * v.x, v.y * v.y, v.z * v.z, v.w * v.w);
        
        /// <summary>
        /// Returns a cVector from the application of Pow2 (v * v).
        /// </summary>
        public static cVector4 Pow2(Complex v) => new cVector4(v * v);
        
        /// <summary>
        /// Returns a cVector4 from component-wise application of Pow3 (v * v * v).
        /// </summary>
        public static cVector4 Pow3(cVector4 v) => new cVector4(v.x * v.x * v.x, v.y * v.y * v.y, v.z * v.z * v.z, v.w * v.w * v.w);
        
        /// <summary>
        /// Returns a cVector from the application of Pow3 (v * v * v).
        /// </summary>
        public static cVector4 Pow3(Complex v) => new cVector4(v * v * v);
        
        /// <summary>
        /// Returns a cVector4 from component-wise application of Acos (Complex.Acos(v)).
        /// </summary>
        public static cVector4 Acos(cVector4 v) => new cVector4(Complex.Acos(v.x), Complex.Acos(v.y), Complex.Acos(v.z), Complex.Acos(v.w));
        
        /// <summary>
        /// Returns a cVector from the application of Acos (Complex.Acos(v)).
        /// </summary>
        public static cVector4 Acos(Complex v) => new cVector4(Complex.Acos(v));
        
        /// <summary>
        /// Returns a cVector4 from component-wise application of Asin (Complex.Asin(v)).
        /// </summary>
        public static cVector4 Asin(cVector4 v) => new cVector4(Complex.Asin(v.x), Complex.Asin(v.y), Complex.Asin(v.z), Complex.Asin(v.w));
        
        /// <summary>
        /// Returns a cVector from the application of Asin (Complex.Asin(v)).
        /// </summary>
        public static cVector4 Asin(Complex v) => new cVector4(Complex.Asin(v));
        
        /// <summary>
        /// Returns a cVector4 from component-wise application of Atan (Complex.Atan(v)).
        /// </summary>
        public static cVector4 Atan(cVector4 v) => new cVector4(Complex.Atan(v.x), Complex.Atan(v.y), Complex.Atan(v.z), Complex.Atan(v.w));
        
        /// <summary>
        /// Returns a cVector from the application of Atan (Complex.Atan(v)).
        /// </summary>
        public static cVector4 Atan(Complex v) => new cVector4(Complex.Atan(v));
        
        /// <summary>
        /// Returns a cVector4 from component-wise application of Cos (Complex.Cos(v)).
        /// </summary>
        public static cVector4 Cos(cVector4 v) => new cVector4(Complex.Cos(v.x), Complex.Cos(v.y), Complex.Cos(v.z), Complex.Cos(v.w));
        
        /// <summary>
        /// Returns a cVector from the application of Cos (Complex.Cos(v)).
        /// </summary>
        public static cVector4 Cos(Complex v) => new cVector4(Complex.Cos(v));
        
        /// <summary>
        /// Returns a cVector4 from component-wise application of Cosh (Complex.Cosh(v)).
        /// </summary>
        public static cVector4 Cosh(cVector4 v) => new cVector4(Complex.Cosh(v.x), Complex.Cosh(v.y), Complex.Cosh(v.z), Complex.Cosh(v.w));
        
        /// <summary>
        /// Returns a cVector from the application of Cosh (Complex.Cosh(v)).
        /// </summary>
        public static cVector4 Cosh(Complex v) => new cVector4(Complex.Cosh(v));
        
        /// <summary>
        /// Returns a cVector4 from component-wise application of Exp (Complex.Exp(v)).
        /// </summary>
        public static cVector4 Exp(cVector4 v) => new cVector4(Complex.Exp(v.x), Complex.Exp(v.y), Complex.Exp(v.z), Complex.Exp(v.w));
        
        /// <summary>
        /// Returns a cVector from the application of Exp (Complex.Exp(v)).
        /// </summary>
        public static cVector4 Exp(Complex v) => new cVector4(Complex.Exp(v));
        
        /// <summary>
        /// Returns a cVector4 from component-wise application of Log (Complex.Log(v)).
        /// </summary>
        public static cVector4 Log(cVector4 v) => new cVector4(Complex.Log(v.x), Complex.Log(v.y), Complex.Log(v.z), Complex.Log(v.w));
        
        /// <summary>
        /// Returns a cVector from the application of Log (Complex.Log(v)).
        /// </summary>
        public static cVector4 Log(Complex v) => new cVector4(Complex.Log(v));
        
        /// <summary>
        /// Returns a cVector4 from component-wise application of Log2 (Complex.Log(v, 2.0)).
        /// </summary>
        public static cVector4 Log2(cVector4 v) => new cVector4(Complex.Log(v.x, 2.0), Complex.Log(v.y, 2.0), Complex.Log(v.z, 2.0), Complex.Log(v.w, 2.0));
        
        /// <summary>
        /// Returns a cVector from the application of Log2 (Complex.Log(v, 2.0)).
        /// </summary>
        public static cVector4 Log2(Complex v) => new cVector4(Complex.Log(v, 2.0));
        
        /// <summary>
        /// Returns a cVector4 from component-wise application of Log10 (Complex.Log10(v)).
        /// </summary>
        public static cVector4 Log10(cVector4 v) => new cVector4(Complex.Log10(v.x), Complex.Log10(v.y), Complex.Log10(v.z), Complex.Log10(v.w));
        
        /// <summary>
        /// Returns a cVector from the application of Log10 (Complex.Log10(v)).
        /// </summary>
        public static cVector4 Log10(Complex v) => new cVector4(Complex.Log10(v));
        
        /// <summary>
        /// Returns a cVector4 from component-wise application of Reciprocal (Complex.Reciprocal(v)).
        /// </summary>
        public static cVector4 Reciprocal(cVector4 v) => new cVector4(Complex.Reciprocal(v.x), Complex.Reciprocal(v.y), Complex.Reciprocal(v.z), Complex.Reciprocal(v.w));
        
        /// <summary>
        /// Returns a cVector from the application of Reciprocal (Complex.Reciprocal(v)).
        /// </summary>
        public static cVector4 Reciprocal(Complex v) => new cVector4(Complex.Reciprocal(v));
        
        /// <summary>
        /// Returns a cVector4 from component-wise application of Sin (Complex.Sin(v)).
        /// </summary>
        public static cVector4 Sin(cVector4 v) => new cVector4(Complex.Sin(v.x), Complex.Sin(v.y), Complex.Sin(v.z), Complex.Sin(v.w));
        
        /// <summary>
        /// Returns a cVector from the application of Sin (Complex.Sin(v)).
        /// </summary>
        public static cVector4 Sin(Complex v) => new cVector4(Complex.Sin(v));
        
        /// <summary>
        /// Returns a cVector4 from component-wise application of Sinh (Complex.Sinh(v)).
        /// </summary>
        public static cVector4 Sinh(cVector4 v) => new cVector4(Complex.Sinh(v.x), Complex.Sinh(v.y), Complex.Sinh(v.z), Complex.Sinh(v.w));
        
        /// <summary>
        /// Returns a cVector from the application of Sinh (Complex.Sinh(v)).
        /// </summary>
        public static cVector4 Sinh(Complex v) => new cVector4(Complex.Sinh(v));
        
        /// <summary>
        /// Returns a cVector4 from component-wise application of Sqrt (Complex.Sqrt(v)).
        /// </summary>
        public static cVector4 Sqrt(cVector4 v) => new cVector4(Complex.Sqrt(v.x), Complex.Sqrt(v.y), Complex.Sqrt(v.z), Complex.Sqrt(v.w));
        
        /// <summary>
        /// Returns a cVector from the application of Sqrt (Complex.Sqrt(v)).
        /// </summary>
        public static cVector4 Sqrt(Complex v) => new cVector4(Complex.Sqrt(v));
        
        /// <summary>
        /// Returns a cVector4 from component-wise application of InverseSqrt (Complex.One / Complex.Sqrt(v)).
        /// </summary>
        public static cVector4 InverseSqrt(cVector4 v) => new cVector4(Complex.One / Complex.Sqrt(v.x), Complex.One / Complex.Sqrt(v.y), Complex.One / Complex.Sqrt(v.z), Complex.One / Complex.Sqrt(v.w));
        
        /// <summary>
        /// Returns a cVector from the application of InverseSqrt (Complex.One / Complex.Sqrt(v)).
        /// </summary>
        public static cVector4 InverseSqrt(Complex v) => new cVector4(Complex.One / Complex.Sqrt(v));
        
        /// <summary>
        /// Returns a cVector4 from component-wise application of Tan (Complex.Tan(v)).
        /// </summary>
        public static cVector4 Tan(cVector4 v) => new cVector4(Complex.Tan(v.x), Complex.Tan(v.y), Complex.Tan(v.z), Complex.Tan(v.w));
        
        /// <summary>
        /// Returns a cVector from the application of Tan (Complex.Tan(v)).
        /// </summary>
        public static cVector4 Tan(Complex v) => new cVector4(Complex.Tan(v));
        
        /// <summary>
        /// Returns a cVector4 from component-wise application of Tanh (Complex.Tanh(v)).
        /// </summary>
        public static cVector4 Tanh(cVector4 v) => new cVector4(Complex.Tanh(v.x), Complex.Tanh(v.y), Complex.Tanh(v.z), Complex.Tanh(v.w));
        
        /// <summary>
        /// Returns a cVector from the application of Tanh (Complex.Tanh(v)).
        /// </summary>
        public static cVector4 Tanh(Complex v) => new cVector4(Complex.Tanh(v));
        
        /// <summary>
        /// Returns a cVector4 from component-wise application of Conjugate (Complex.Conjugate(v)).
        /// </summary>
        public static cVector4 Conjugate(cVector4 v) => new cVector4(Complex.Conjugate(v.x), Complex.Conjugate(v.y), Complex.Conjugate(v.z), Complex.Conjugate(v.w));
        
        /// <summary>
        /// Returns a cVector from the application of Conjugate (Complex.Conjugate(v)).
        /// </summary>
        public static cVector4 Conjugate(Complex v) => new cVector4(Complex.Conjugate(v));
        
        /// <summary>
        /// Returns a cVector4 from component-wise application of Pow (Complex.Pow(lhs, rhs)).
        /// </summary>
        public static cVector4 Pow(cVector4 lhs, cVector4 rhs) => new cVector4(Complex.Pow(lhs.x, rhs.x), Complex.Pow(lhs.y, rhs.y), Complex.Pow(lhs.z, rhs.z), Complex.Pow(lhs.w, rhs.w));
        
        /// <summary>
        /// Returns a cVector4 from component-wise application of Pow (Complex.Pow(lhs, rhs)).
        /// </summary>
        public static cVector4 Pow(cVector4 lhs, Complex rhs) => new cVector4(Complex.Pow(lhs.x, rhs), Complex.Pow(lhs.y, rhs), Complex.Pow(lhs.z, rhs), Complex.Pow(lhs.w, rhs));
        
        /// <summary>
        /// Returns a cVector4 from component-wise application of Pow (Complex.Pow(lhs, rhs)).
        /// </summary>
        public static cVector4 Pow(Complex lhs, cVector4 rhs) => new cVector4(Complex.Pow(lhs, rhs.x), Complex.Pow(lhs, rhs.y), Complex.Pow(lhs, rhs.z), Complex.Pow(lhs, rhs.w));
        
        /// <summary>
        /// Returns a cVector from the application of Pow (Complex.Pow(lhs, rhs)).
        /// </summary>
        public static cVector4 Pow(Complex lhs, Complex rhs) => new cVector4(Complex.Pow(lhs, rhs));
        
        /// <summary>
        /// Returns a cVector4 from component-wise application of Log (Complex.Log(lhs, rhs)).
        /// </summary>
        public static cVector4 Log(cVector4 lhs, dVector4 rhs) => new cVector4(Complex.Log(lhs.x, rhs.x), Complex.Log(lhs.y, rhs.y), Complex.Log(lhs.z, rhs.z), Complex.Log(lhs.w, rhs.w));
        
        /// <summary>
        /// Returns a cVector4 from component-wise application of Log (Complex.Log(lhs, rhs)).
        /// </summary>
        public static cVector4 Log(cVector4 lhs, double rhs) => new cVector4(Complex.Log(lhs.x, rhs), Complex.Log(lhs.y, rhs), Complex.Log(lhs.z, rhs), Complex.Log(lhs.w, rhs));
        
        /// <summary>
        /// Returns a cVector4 from component-wise application of Log (Complex.Log(lhs, rhs)).
        /// </summary>
        public static cVector4 Log(Complex lhs, dVector4 rhs) => new cVector4(Complex.Log(lhs, rhs.x), Complex.Log(lhs, rhs.y), Complex.Log(lhs, rhs.z), Complex.Log(lhs, rhs.w));
        
        /// <summary>
        /// Returns a cVector from the application of Log (Complex.Log(lhs, rhs)).
        /// </summary>
        public static cVector4 Log(Complex lhs, double rhs) => new cVector4(Complex.Log(lhs, rhs));
        
        /// <summary>
        /// Returns a cVector4 from component-wise application of FromPolarCoordinates (Complex.FromPolarCoordinates(magnitude, phase)).
        /// </summary>
        public static cVector4 FromPolarCoordinates(dVector4 magnitude, dVector4 phase) => new cVector4(Complex.FromPolarCoordinates(magnitude.x, phase.x), Complex.FromPolarCoordinates(magnitude.y, phase.y), Complex.FromPolarCoordinates(magnitude.z, phase.z), Complex.FromPolarCoordinates(magnitude.w, phase.w));
        
        /// <summary>
        /// Returns a cVector4 from component-wise application of FromPolarCoordinates (Complex.FromPolarCoordinates(magnitude, phase)).
        /// </summary>
        public static cVector4 FromPolarCoordinates(dVector4 magnitude, double phase) => new cVector4(Complex.FromPolarCoordinates(magnitude.x, phase), Complex.FromPolarCoordinates(magnitude.y, phase), Complex.FromPolarCoordinates(magnitude.z, phase), Complex.FromPolarCoordinates(magnitude.w, phase));
        
        /// <summary>
        /// Returns a cVector4 from component-wise application of FromPolarCoordinates (Complex.FromPolarCoordinates(magnitude, phase)).
        /// </summary>
        public static cVector4 FromPolarCoordinates(double magnitude, dVector4 phase) => new cVector4(Complex.FromPolarCoordinates(magnitude, phase.x), Complex.FromPolarCoordinates(magnitude, phase.y), Complex.FromPolarCoordinates(magnitude, phase.z), Complex.FromPolarCoordinates(magnitude, phase.w));
        
        /// <summary>
        /// Returns a cVector from the application of FromPolarCoordinates (Complex.FromPolarCoordinates(magnitude, phase)).
        /// </summary>
        public static cVector4 FromPolarCoordinates(double magnitude, double phase) => new cVector4(Complex.FromPolarCoordinates(magnitude, phase));
        
        /// <summary>
        /// Returns a cVector4 from component-wise application of Fma (a * b + c).
        /// </summary>
        public static cVector4 Fma(cVector4 a, cVector4 b, cVector4 c) => new cVector4(a.x * b.x + c.x, a.y * b.y + c.y, a.z * b.z + c.z, a.w * b.w + c.w);
        
        /// <summary>
        /// Returns a cVector4 from component-wise application of Fma (a * b + c).
        /// </summary>
        public static cVector4 Fma(cVector4 a, cVector4 b, Complex c) => new cVector4(a.x * b.x + c, a.y * b.y + c, a.z * b.z + c, a.w * b.w + c);
        
        /// <summary>
        /// Returns a cVector4 from component-wise application of Fma (a * b + c).
        /// </summary>
        public static cVector4 Fma(cVector4 a, Complex b, cVector4 c) => new cVector4(a.x * b + c.x, a.y * b + c.y, a.z * b + c.z, a.w * b + c.w);
        
        /// <summary>
        /// Returns a cVector4 from component-wise application of Fma (a * b + c).
        /// </summary>
        public static cVector4 Fma(cVector4 a, Complex b, Complex c) => new cVector4(a.x * b + c, a.y * b + c, a.z * b + c, a.w * b + c);
        
        /// <summary>
        /// Returns a cVector4 from component-wise application of Fma (a * b + c).
        /// </summary>
        public static cVector4 Fma(Complex a, cVector4 b, cVector4 c) => new cVector4(a * b.x + c.x, a * b.y + c.y, a * b.z + c.z, a * b.w + c.w);
        
        /// <summary>
        /// Returns a cVector4 from component-wise application of Fma (a * b + c).
        /// </summary>
        public static cVector4 Fma(Complex a, cVector4 b, Complex c) => new cVector4(a * b.x + c, a * b.y + c, a * b.z + c, a * b.w + c);
        
        /// <summary>
        /// Returns a cVector4 from component-wise application of Fma (a * b + c).
        /// </summary>
        public static cVector4 Fma(Complex a, Complex b, cVector4 c) => new cVector4(a * b + c.x, a * b + c.y, a * b + c.z, a * b + c.w);
        
        /// <summary>
        /// Returns a cVector from the application of Fma (a * b + c).
        /// </summary>
        public static cVector4 Fma(Complex a, Complex b, Complex c) => new cVector4(a * b + c);
        
        /// <summary>
        /// Returns a cVector4 from component-wise application of Add (lhs + rhs).
        /// </summary>
        public static cVector4 Add(cVector4 lhs, cVector4 rhs) => new cVector4(lhs.x + rhs.x, lhs.y + rhs.y, lhs.z + rhs.z, lhs.w + rhs.w);
        
        /// <summary>
        /// Returns a cVector4 from component-wise application of Add (lhs + rhs).
        /// </summary>
        public static cVector4 Add(cVector4 lhs, Complex rhs) => new cVector4(lhs.x + rhs, lhs.y + rhs, lhs.z + rhs, lhs.w + rhs);
        
        /// <summary>
        /// Returns a cVector4 from component-wise application of Add (lhs + rhs).
        /// </summary>
        public static cVector4 Add(Complex lhs, cVector4 rhs) => new cVector4(lhs + rhs.x, lhs + rhs.y, lhs + rhs.z, lhs + rhs.w);
        
        /// <summary>
        /// Returns a cVector from the application of Add (lhs + rhs).
        /// </summary>
        public static cVector4 Add(Complex lhs, Complex rhs) => new cVector4(lhs + rhs);
        
        /// <summary>
        /// Returns a cVector4 from component-wise application of Sub (lhs - rhs).
        /// </summary>
        public static cVector4 Sub(cVector4 lhs, cVector4 rhs) => new cVector4(lhs.x - rhs.x, lhs.y - rhs.y, lhs.z - rhs.z, lhs.w - rhs.w);
        
        /// <summary>
        /// Returns a cVector4 from component-wise application of Sub (lhs - rhs).
        /// </summary>
        public static cVector4 Sub(cVector4 lhs, Complex rhs) => new cVector4(lhs.x - rhs, lhs.y - rhs, lhs.z - rhs, lhs.w - rhs);
        
        /// <summary>
        /// Returns a cVector4 from component-wise application of Sub (lhs - rhs).
        /// </summary>
        public static cVector4 Sub(Complex lhs, cVector4 rhs) => new cVector4(lhs - rhs.x, lhs - rhs.y, lhs - rhs.z, lhs - rhs.w);
        
        /// <summary>
        /// Returns a cVector from the application of Sub (lhs - rhs).
        /// </summary>
        public static cVector4 Sub(Complex lhs, Complex rhs) => new cVector4(lhs - rhs);
        
        /// <summary>
        /// Returns a cVector4 from component-wise application of Mul (lhs * rhs).
        /// </summary>
        public static cVector4 Mul(cVector4 lhs, cVector4 rhs) => new cVector4(lhs.x * rhs.x, lhs.y * rhs.y, lhs.z * rhs.z, lhs.w * rhs.w);
        
        /// <summary>
        /// Returns a cVector4 from component-wise application of Mul (lhs * rhs).
        /// </summary>
        public static cVector4 Mul(cVector4 lhs, Complex rhs) => new cVector4(lhs.x * rhs, lhs.y * rhs, lhs.z * rhs, lhs.w * rhs);
        
        /// <summary>
        /// Returns a cVector4 from component-wise application of Mul (lhs * rhs).
        /// </summary>
        public static cVector4 Mul(Complex lhs, cVector4 rhs) => new cVector4(lhs * rhs.x, lhs * rhs.y, lhs * rhs.z, lhs * rhs.w);
        
        /// <summary>
        /// Returns a cVector from the application of Mul (lhs * rhs).
        /// </summary>
        public static cVector4 Mul(Complex lhs, Complex rhs) => new cVector4(lhs * rhs);
        
        /// <summary>
        /// Returns a cVector4 from component-wise application of Div (lhs / rhs).
        /// </summary>
        public static cVector4 Div(cVector4 lhs, cVector4 rhs) => new cVector4(lhs.x / rhs.x, lhs.y / rhs.y, lhs.z / rhs.z, lhs.w / rhs.w);
        
        /// <summary>
        /// Returns a cVector4 from component-wise application of Div (lhs / rhs).
        /// </summary>
        public static cVector4 Div(cVector4 lhs, Complex rhs) => new cVector4(lhs.x / rhs, lhs.y / rhs, lhs.z / rhs, lhs.w / rhs);
        
        /// <summary>
        /// Returns a cVector4 from component-wise application of Div (lhs / rhs).
        /// </summary>
        public static cVector4 Div(Complex lhs, cVector4 rhs) => new cVector4(lhs / rhs.x, lhs / rhs.y, lhs / rhs.z, lhs / rhs.w);
        
        /// <summary>
        /// Returns a cVector from the application of Div (lhs / rhs).
        /// </summary>
        public static cVector4 Div(Complex lhs, Complex rhs) => new cVector4(lhs / rhs);

        #endregion


        #region Component-Wise Operator Overloads
        
        /// <summary>
        /// Returns a cVector4 from component-wise application of operator+ (lhs + rhs).
        /// </summary>
        public static cVector4 operator+(cVector4 lhs, cVector4 rhs) => new cVector4(lhs.x + rhs.x, lhs.y + rhs.y, lhs.z + rhs.z, lhs.w + rhs.w);
        
        /// <summary>
        /// Returns a cVector4 from component-wise application of operator+ (lhs + rhs).
        /// </summary>
        public static cVector4 operator+(cVector4 lhs, Complex rhs) => new cVector4(lhs.x + rhs, lhs.y + rhs, lhs.z + rhs, lhs.w + rhs);
        
        /// <summary>
        /// Returns a cVector4 from component-wise application of operator+ (lhs + rhs).
        /// </summary>
        public static cVector4 operator+(Complex lhs, cVector4 rhs) => new cVector4(lhs + rhs.x, lhs + rhs.y, lhs + rhs.z, lhs + rhs.w);
        
        /// <summary>
        /// Returns a cVector4 from component-wise application of operator- (lhs - rhs).
        /// </summary>
        public static cVector4 operator-(cVector4 lhs, cVector4 rhs) => new cVector4(lhs.x - rhs.x, lhs.y - rhs.y, lhs.z - rhs.z, lhs.w - rhs.w);
        
        /// <summary>
        /// Returns a cVector4 from component-wise application of operator- (lhs - rhs).
        /// </summary>
        public static cVector4 operator-(cVector4 lhs, Complex rhs) => new cVector4(lhs.x - rhs, lhs.y - rhs, lhs.z - rhs, lhs.w - rhs);
        
        /// <summary>
        /// Returns a cVector4 from component-wise application of operator- (lhs - rhs).
        /// </summary>
        public static cVector4 operator-(Complex lhs, cVector4 rhs) => new cVector4(lhs - rhs.x, lhs - rhs.y, lhs - rhs.z, lhs - rhs.w);
        
        /// <summary>
        /// Returns a cVector4 from component-wise application of operator* (lhs * rhs).
        /// </summary>
        public static cVector4 operator*(cVector4 lhs, cVector4 rhs) => new cVector4(lhs.x * rhs.x, lhs.y * rhs.y, lhs.z * rhs.z, lhs.w * rhs.w);
        
        /// <summary>
        /// Returns a cVector4 from component-wise application of operator* (lhs * rhs).
        /// </summary>
        public static cVector4 operator*(cVector4 lhs, Complex rhs) => new cVector4(lhs.x * rhs, lhs.y * rhs, lhs.z * rhs, lhs.w * rhs);
        
        /// <summary>
        /// Returns a cVector4 from component-wise application of operator* (lhs * rhs).
        /// </summary>
        public static cVector4 operator*(Complex lhs, cVector4 rhs) => new cVector4(lhs * rhs.x, lhs * rhs.y, lhs * rhs.z, lhs * rhs.w);
        
        /// <summary>
        /// Returns a cVector4 from component-wise application of operator/ (lhs / rhs).
        /// </summary>
        public static cVector4 operator/(cVector4 lhs, cVector4 rhs) => new cVector4(lhs.x / rhs.x, lhs.y / rhs.y, lhs.z / rhs.z, lhs.w / rhs.w);
        
        /// <summary>
        /// Returns a cVector4 from component-wise application of operator/ (lhs / rhs).
        /// </summary>
        public static cVector4 operator/(cVector4 lhs, Complex rhs) => new cVector4(lhs.x / rhs, lhs.y / rhs, lhs.z / rhs, lhs.w / rhs);
        
        /// <summary>
        /// Returns a cVector4 from component-wise application of operator/ (lhs / rhs).
        /// </summary>
        public static cVector4 operator/(Complex lhs, cVector4 rhs) => new cVector4(lhs / rhs.x, lhs / rhs.y, lhs / rhs.z, lhs / rhs.w);
        
        /// <summary>
        /// Returns a cVector4 from component-wise application of operator+ (identity).
        /// </summary>
        public static cVector4 operator+(cVector4 v) => v;
        
        /// <summary>
        /// Returns a cVector4 from component-wise application of operator- (-v).
        /// </summary>
        public static cVector4 operator-(cVector4 v) => new cVector4(-v.x, -v.y, -v.z, -v.w);

        #endregion

    }
 */
}
