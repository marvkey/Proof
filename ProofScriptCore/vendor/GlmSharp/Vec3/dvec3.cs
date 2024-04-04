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
    /// A Vectortor of type double with 3 components.
    /// </summary>
    [RegisterCoreClassStruct]
    
    [StructLayout(LayoutKind.Sequential)]
    public struct dVector3 : IReadOnlyList<double>, IEquatable<dVector3>
    {

        #region Fields
        
        /// <summary>
        /// x-component
        /// </summary>
        
        public double x;
        
        /// <summary>
        /// y-component
        /// </summary>
        
        public double y;
        
        /// <summary>
        /// z-component
        /// </summary>
        
        public double z;

        #endregion


        #region Constructors
        
        /// <summary>
        /// Component-wise constructor
        /// </summary>
        public dVector3(double x, double y, double z)
        {
            this.x = x;
            this.y = y;
            this.z = z;
        }
        
        /// <summary>
        /// all-same-value constructor
        /// </summary>
        public dVector3(double v)
        {
            this.x = v;
            this.y = v;
            this.z = v;
        }
        
        /// <summary>
        /// from-Vectortor constructor (empty fields are zero/false)
        /// </summary>
        public dVector3(dVector2 v)
        {
            this.x = v.x;
            this.y = v.y;
            this.z = 0.0;
        }
        
        /// <summary>
        /// from-Vectortor-and-value constructor
        /// </summary>
        public dVector3(dVector2 v, double z)
        {
            this.x = v.x;
            this.y = v.y;
            this.z = z;
        }
        
        /// <summary>
        /// from-Vectortor constructor
        /// </summary>
        public dVector3(dVector3 v)
        {
            this.x = v.x;
            this.y = v.y;
            this.z = v.z;
        }
        
        /// <summary>
        /// from-Vectortor constructor (additional fields are truncated)
        /// </summary>
        public dVector3(dVector4 v)
        {
            this.x = v.x;
            this.y = v.y;
            this.z = v.z;
        }
        
        /// <summary>
        /// From-array/list constructor (superfluous values are ignored, missing values are zero-filled).
        /// </summary>
        public dVector3(IReadOnlyList<double> v)
        {
            var c = v.Count;
            this.x = c < 0 ? 0.0 : v[0];
            this.y = c < 1 ? 0.0 : v[1];
            this.z = c < 2 ? 0.0 : v[2];
        }
        
        /// <summary>
        /// Generic from-array constructor (superfluous values are ignored, missing values are zero-filled).
        /// </summary>
        public dVector3(Object[] v)
        {
            var c = v.Length;
            this.x = c < 0 ? 0.0 : (double)v[0];
            this.y = c < 1 ? 0.0 : (double)v[1];
            this.z = c < 2 ? 0.0 : (double)v[2];
        }
        
        /// <summary>
        /// From-array constructor (superfluous values are ignored, missing values are zero-filled).
        /// </summary>
        public dVector3(double[] v)
        {
            var c = v.Length;
            this.x = c < 0 ? 0.0 : v[0];
            this.y = c < 1 ? 0.0 : v[1];
            this.z = c < 2 ? 0.0 : v[2];
        }
        
        /// <summary>
        /// From-array constructor with base index (superfluous values are ignored, missing values are zero-filled).
        /// </summary>
        public dVector3(double[] v, int startIndex)
        {
            var c = v.Length;
            this.x = c + startIndex < 0 ? 0.0 : v[0 + startIndex];
            this.y = c + startIndex < 1 ? 0.0 : v[1 + startIndex];
            this.z = c + startIndex < 2 ? 0.0 : v[2 + startIndex];
        }
        
        /// <summary>
        /// From-IEnumerable constructor (superfluous values are ignored, missing values are zero-filled).
        /// </summary>
        public dVector3(IEnumerable<double> v)
            : this(v.ToArray())
        {
        }

        #endregion


        #region Implicit Operators
        
        /// <summary>
        /// Implicitly converts this to a cVector3.
        /// </summary>
        //public static implicit operator cVector3(dVector3 v) => new cVector3((Complex)v.x, (Complex)v.y, (Complex)v.z);

        #endregion


        #region Explicit Operators
        
        /// <summary>
        /// Explicitly converts this to a iVector2.
        /// </summary>
        public static explicit operator iVector2(dVector3 v) => new iVector2((int)v.x, (int)v.y);
        
        /// <summary>
        /// Explicitly converts this to a iVector3.
        /// </summary>
        public static explicit operator iVector3(dVector3 v) => new iVector3((int)v.x, (int)v.y, (int)v.z);
        
        /// <summary>
        /// Explicitly converts this to a iVector4. (Higher components are zeroed)
        /// </summary>
        public static explicit operator iVector4(dVector3 v) => new iVector4((int)v.x, (int)v.y, (int)v.z, 0);
        
        /// <summary>
        /// Explicitly converts this to a uVector2.
        /// </summary>
        public static explicit operator uVector2(dVector3 v) => new uVector2((uint)v.x, (uint)v.y);
        
        /// <summary>
        /// Explicitly converts this to a uVector3.
        /// </summary>
        public static explicit operator uVector3(dVector3 v) => new uVector3((uint)v.x, (uint)v.y, (uint)v.z);
        
        /// <summary>
        /// Explicitly converts this to a uVector4. (Higher components are zeroed)
        /// </summary>
        public static explicit operator uVector4(dVector3 v) => new uVector4((uint)v.x, (uint)v.y, (uint)v.z, 0u);
        
        /// <summary>
        /// Explicitly converts this to a Vector2.
        /// </summary>
        public static explicit operator Vector2(dVector3 v) => new Vector2((float)v.x, (float)v.y);
        
        /// <summary>
        /// Explicitly converts this to a Vector3.
        /// </summary>
        public static explicit operator Vector3(dVector3 v) => new Vector3((float)v.x, (float)v.y, (float)v.z);
        
        /// <summary>
        /// Explicitly converts this to a Vector4. (Higher components are zeroed)
        /// </summary>
        public static explicit operator Vector4(dVector3 v) => new Vector4((float)v.x, (float)v.y, (float)v.z, 0f);
        
        /// <summary>
        /// Explicitly converts this to a hVector2.
        /// </summary>
        public static explicit operator hVector2(dVector3 v) => new hVector2((Half)v.x, (Half)v.y);
        
        /// <summary>
        /// Explicitly converts this to a hVector3.
        /// </summary>
        public static explicit operator hVector3(dVector3 v) => new hVector3((Half)v.x, (Half)v.y, (Half)v.z);
        
        /// <summary>
        /// Explicitly converts this to a hVector4. (Higher components are zeroed)
        /// </summary>
        public static explicit operator hVector4(dVector3 v) => new hVector4((Half)v.x, (Half)v.y, (Half)v.z, Half.Zero);
        
        /// <summary>
        /// Explicitly converts this to a dVector2.
        /// </summary>
        public static explicit operator dVector2(dVector3 v) => new dVector2((double)v.x, (double)v.y);
        
        /// <summary>
        /// Explicitly converts this to a dVector4. (Higher components are zeroed)
        /// </summary>
        public static explicit operator dVector4(dVector3 v) => new dVector4((double)v.x, (double)v.y, (double)v.z, 0.0);
        
        /// <summary>
        /// Explicitly converts this to a decVector2.
        /// </summary>
        public static explicit operator decVector2(dVector3 v) => new decVector2((decimal)v.x, (decimal)v.y);
        
        /// <summary>
        /// Explicitly converts this to a decVector3.
        /// </summary>
        public static explicit operator decVector3(dVector3 v) => new decVector3((decimal)v.x, (decimal)v.y, (decimal)v.z);
        
        /// <summary>
        /// Explicitly converts this to a decVector4. (Higher components are zeroed)
        /// </summary>
        public static explicit operator decVector4(dVector3 v) => new decVector4((decimal)v.x, (decimal)v.y, (decimal)v.z, 0m);
        
        /// <summary>
        /// Explicitly converts this to a cVector2.
        /// </summary>
      //  public static explicit operator cVector2(dVector3 v) => new cVector2((Complex)v.x, (Complex)v.y);
        
        /// <summary>
        /// Explicitly converts this to a cVector4. (Higher components are zeroed)
        /// </summary>
        //public static explicit operator cVector4(dVector3 v) => new cVector4((Complex)v.x, (Complex)v.y, (Complex)v.z, Complex.Zero);
        
        /// <summary>
        /// Explicitly converts this to a lVector2.
        /// </summary>
        public static explicit operator lVector2(dVector3 v) => new lVector2((long)v.x, (long)v.y);
        
        /// <summary>
        /// Explicitly converts this to a lVector3.
        /// </summary>
        public static explicit operator lVector3(dVector3 v) => new lVector3((long)v.x, (long)v.y, (long)v.z);
        
        /// <summary>
        /// Explicitly converts this to a lVector4. (Higher components are zeroed)
        /// </summary>
        public static explicit operator lVector4(dVector3 v) => new lVector4((long)v.x, (long)v.y, (long)v.z, 0);
        
        /// <summary>
        /// Explicitly converts this to a bVector2.
        /// </summary>
        public static explicit operator bVector2(dVector3 v) => new bVector2(v.x != 0.0, v.y != 0.0);
        
        /// <summary>
        /// Explicitly converts this to a bVector3.
        /// </summary>
        public static explicit operator bVector3(dVector3 v) => new bVector3(v.x != 0.0, v.y != 0.0, v.z != 0.0);
        
        /// <summary>
        /// Explicitly converts this to a bVector4. (Higher components are zeroed)
        /// </summary>
        public static explicit operator bVector4(dVector3 v) => new bVector4(v.x != 0.0, v.y != 0.0, v.z != 0.0, false);
        
        /// <summary>
        /// Explicitly converts this to a double array.
        /// </summary>
        public static explicit operator double[](dVector3 v) => new [] { v.x, v.y, v.z };
        
        /// <summary>
        /// Explicitly converts this to a generic object array.
        /// </summary>
        public static explicit operator Object[](dVector3 v) => new Object[] { v.x, v.y, v.z };

        #endregion


        #region Indexer
        
        /// <summary>
        /// Gets/Sets a specific indexed component (a bit slower than direct access).
        /// </summary>
        public double this[int index]
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
        public swizzle_dVector3 swizzle => new swizzle_dVector3(x, y, z);
        
        /// <summary>
        /// Gets or sets the specified subset of components. For more advanced (read-only) swizzling, use the .swizzle property.
        /// </summary>
        public dVector2 xy
        {
            get
            {
                return new dVector2(x, y);
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
        public dVector2 xz
        {
            get
            {
                return new dVector2(x, z);
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
        public dVector2 yz
        {
            get
            {
                return new dVector2(y, z);
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
        public dVector3 xyz
        {
            get
            {
                return new dVector3(x, y, z);
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
        public dVector2 rg
        {
            get
            {
                return new dVector2(x, y);
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
        public dVector2 rb
        {
            get
            {
                return new dVector2(x, z);
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
        public dVector2 gb
        {
            get
            {
                return new dVector2(y, z);
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
        public dVector3 rgb
        {
            get
            {
                return new dVector3(x, y, z);
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
        public double r
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
        public double g
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
        public double b
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
        public double[] Values => new[] { x, y, z };
        
        /// <summary>
        /// Returns the number of components (3).
        /// </summary>
        public int Count => 3;
        
        /// <summary>
        /// Returns the minimal component of this Vectortor.
        /// </summary>
        public double MinElement => Math.Min(Math.Min(x, y), z);
        
        /// <summary>
        /// Returns the maximal component of this Vectortor.
        /// </summary>
        public double MaxElement => Math.Max(Math.Max(x, y), z);
        
        /// <summary>
        /// Returns the euclidean length of this Vectortor.
        /// </summary>
        public double Length => (double)Math.Sqrt(((x*x + y*y) + z*z));
        
        /// <summary>
        /// Returns the squared euclidean length of this Vectortor.
        /// </summary>
        public double LengthSqr => ((x*x + y*y) + z*z);
        
        /// <summary>
        /// Returns the sum of all components.
        /// </summary>
        public double Sum => ((x + y) + z);
        
        /// <summary>
        /// Returns the euclidean norm of this Vectortor.
        /// </summary>
        public double Norm => (double)Math.Sqrt(((x*x + y*y) + z*z));
        
        /// <summary>
        /// Returns the one-norm of this Vectortor.
        /// </summary>
        public double Norm1 => ((Math.Abs(x) + Math.Abs(y)) + Math.Abs(z));
        
        /// <summary>
        /// Returns the two-norm (euclidean length) of this Vectortor.
        /// </summary>
        public double Norm2 => (double)Math.Sqrt(((x*x + y*y) + z*z));
        
        /// <summary>
        /// Returns the max-norm of this Vectortor.
        /// </summary>
        public double NormMax => Math.Max(Math.Max(Math.Abs(x), Math.Abs(y)), Math.Abs(z));
        
        /// <summary>
        /// Returns a copy of this Vectortor with length one (undefined if this has zero length).
        /// </summary>
        public dVector3 Normalized => this / (double)Length;
        
        /// <summary>
        /// Returns a copy of this Vectortor with length one (returns zero if length is zero).
        /// </summary>
        public dVector3 NormalizedSafe => this == Zero ? Zero : this / (double)Length;

        #endregion


        #region Static Properties
        
        /// <summary>
        /// Predefined all-zero Vectortor
        /// </summary>
        public static dVector3 Zero { get; } = new dVector3(0.0, 0.0, 0.0);
        
        /// <summary>
        /// Predefined all-ones Vectortor
        /// </summary>
        public static dVector3 Ones { get; } = new dVector3(1.0, 1.0, 1.0);
        
        /// <summary>
        /// Predefined unit-X Vectortor
        /// </summary>
        public static dVector3 UnitX { get; } = new dVector3(1.0, 0.0, 0.0);
        
        /// <summary>
        /// Predefined unit-Y Vectortor
        /// </summary>
        public static dVector3 UnitY { get; } = new dVector3(0.0, 1.0, 0.0);
        
        /// <summary>
        /// Predefined unit-Z Vectortor
        /// </summary>
        public static dVector3 UnitZ { get; } = new dVector3(0.0, 0.0, 1.0);
        
        /// <summary>
        /// Predefined all-MaxValue Vectortor
        /// </summary>
        public static dVector3 MaxValue { get; } = new dVector3(double.MaxValue, double.MaxValue, double.MaxValue);
        
        /// <summary>
        /// Predefined all-MinValue Vectortor
        /// </summary>
        public static dVector3 MinValue { get; } = new dVector3(double.MinValue, double.MinValue, double.MinValue);
        
        /// <summary>
        /// Predefined all-Epsilon Vectortor
        /// </summary>
        public static dVector3 Epsilon { get; } = new dVector3(double.Epsilon, double.Epsilon, double.Epsilon);
        
        /// <summary>
        /// Predefined all-NaN Vectortor
        /// </summary>
        public static dVector3 NaN { get; } = new dVector3(double.NaN, double.NaN, double.NaN);
        
        /// <summary>
        /// Predefined all-NegativeInfinity Vectortor
        /// </summary>
        public static dVector3 NegativeInfinity { get; } = new dVector3(double.NegativeInfinity, double.NegativeInfinity, double.NegativeInfinity);
        
        /// <summary>
        /// Predefined all-PositiveInfinity Vectortor
        /// </summary>
        public static dVector3 PositiveInfinity { get; } = new dVector3(double.PositiveInfinity, double.PositiveInfinity, double.PositiveInfinity);

        #endregion


        #region Operators
        
        /// <summary>
        /// Returns true iff this equals rhs component-wise.
        /// </summary>
        public static bool operator==(dVector3 lhs, dVector3 rhs) => lhs.Equals(rhs);
        
        /// <summary>
        /// Returns true iff this does not equal rhs (component-wise).
        /// </summary>
        public static bool operator!=(dVector3 lhs, dVector3 rhs) => !lhs.Equals(rhs);

        #endregion


        #region Functions
        
        /// <summary>
        /// Returns an enumerator that iterates through all components.
        /// </summary>
        public IEnumerator<double> GetEnumerator()
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
        /// Returns a string representation of this Vectortor using ', ' as a seperator.
        /// </summary>
        public override string ToString() => ToString(", ");
        
        /// <summary>
        /// Returns a string representation of this Vectortor using a provided seperator.
        /// </summary>
        public string ToString(string sep) => ((x + sep + y) + sep + z);
        
        /// <summary>
        /// Returns a string representation of this Vectortor using a provided seperator and a forMatrix provider for each component.
        /// </summary>
        public string ToString(string sep, IFormatProvider provider) => ((x.ToString(provider) + sep + y.ToString(provider)) + sep + z.ToString(provider));
        
        /// <summary>
        /// Returns a string representation of this Vectortor using a provided seperator and a forMatrix for each component.
        /// </summary>
        public string ToString(string sep, string forMatrix) => ((x.ToString(forMatrix) + sep + y.ToString(forMatrix)) + sep + z.ToString(forMatrix));
        
        /// <summary>
        /// Returns a string representation of this Vectortor using a provided seperator and a forMatrix and forMatrix provider for each component.
        /// </summary>
        public string ToString(string sep, string forMatrix, IFormatProvider provider) => ((x.ToString(forMatrix, provider) + sep + y.ToString(forMatrix, provider)) + sep + z.ToString(forMatrix, provider));
        
        /// <summary>
        /// Returns true iff this equals rhs component-wise.
        /// </summary>
        public bool Equals(dVector3 rhs) => ((x.Equals(rhs.x) && y.Equals(rhs.y)) && z.Equals(rhs.z));
        
        /// <summary>
        /// Returns true iff this equals rhs type- and component-wise.
        /// </summary>
        public override bool Equals(object obj)
        {
            if (ReferenceEquals(null, obj)) return false;
            return obj is dVector3 && Equals((dVector3) obj);
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
        /// Returns the p-norm of this Vectortor.
        /// </summary>
        public double NormP(double p) => Math.Pow(((Math.Pow((double)Math.Abs(x), p) + Math.Pow((double)Math.Abs(y), p)) + Math.Pow((double)Math.Abs(z), p)), 1 / p);

        #endregion


        #region Static Functions
        
        /// <summary>
        /// Converts the string representation of the Vectortor into a Vectortor representation (using ', ' as a separator).
        /// </summary>
        public static dVector3 Parse(string s) => Parse(s, ", ");
        
        /// <summary>
        /// Converts the string representation of the Vectortor into a Vectortor representation (using a designated separator).
        /// </summary>
        public static dVector3 Parse(string s, string sep)
        {
            var kvp = s.Split(new[] { sep }, StringSplitOptions.None);
            if (kvp.Length != 3) throw new FormatException("input has not exactly 3 parts");
            return new dVector3(double.Parse(kvp[0].Trim()), double.Parse(kvp[1].Trim()), double.Parse(kvp[2].Trim()));
        }
        
        /// <summary>
        /// Converts the string representation of the Vectortor into a Vectortor representation (using a designated separator and a type provider).
        /// </summary>
        public static dVector3 Parse(string s, string sep, IFormatProvider provider)
        {
            var kvp = s.Split(new[] { sep }, StringSplitOptions.None);
            if (kvp.Length != 3) throw new FormatException("input has not exactly 3 parts");
            return new dVector3(double.Parse(kvp[0].Trim(), provider), double.Parse(kvp[1].Trim(), provider), double.Parse(kvp[2].Trim(), provider));
        }
        
        /// <summary>
        /// Converts the string representation of the Vectortor into a Vectortor representation (using a designated separator and a number style).
        /// </summary>
        public static dVector3 Parse(string s, string sep, NumberStyles style)
        {
            var kvp = s.Split(new[] { sep }, StringSplitOptions.None);
            if (kvp.Length != 3) throw new FormatException("input has not exactly 3 parts");
            return new dVector3(double.Parse(kvp[0].Trim(), style), double.Parse(kvp[1].Trim(), style), double.Parse(kvp[2].Trim(), style));
        }
        
        /// <summary>
        /// Converts the string representation of the Vectortor into a Vectortor representation (using a designated separator and a number style and a forMatrix provider).
        /// </summary>
        public static dVector3 Parse(string s, string sep, NumberStyles style, IFormatProvider provider)
        {
            var kvp = s.Split(new[] { sep }, StringSplitOptions.None);
            if (kvp.Length != 3) throw new FormatException("input has not exactly 3 parts");
            return new dVector3(double.Parse(kvp[0].Trim(), style, provider), double.Parse(kvp[1].Trim(), style, provider), double.Parse(kvp[2].Trim(), style, provider));
        }
        
        /// <summary>
        /// Tries to convert the string representation of the Vectortor into a Vectortor representation (using ', ' as a separator), returns false if string was invalid.
        /// </summary>
        public static bool TryParse(string s, out dVector3 result) => TryParse(s, ", ", out result);
        
        /// <summary>
        /// Tries to convert the string representation of the Vectortor into a Vectortor representation (using a designated separator), returns false if string was invalid.
        /// </summary>
        public static bool TryParse(string s, string sep, out dVector3 result)
        {
            result = Zero;
            if (string.IsNullOrEmpty(s)) return false;
            var kvp = s.Split(new[] { sep }, StringSplitOptions.None);
            if (kvp.Length != 3) return false;
            double x = 0.0, y = 0.0, z = 0.0;
            var ok = ((double.TryParse(kvp[0].Trim(), out x) && double.TryParse(kvp[1].Trim(), out y)) && double.TryParse(kvp[2].Trim(), out z));
            result = ok ? new dVector3(x, y, z) : Zero;
            return ok;
        }
        
        /// <summary>
        /// Tries to convert the string representation of the Vectortor into a Vectortor representation (using a designated separator and a number style and a forMatrix provider), returns false if string was invalid.
        /// </summary>
        public static bool TryParse(string s, string sep, NumberStyles style, IFormatProvider provider, out dVector3 result)
        {
            result = Zero;
            if (string.IsNullOrEmpty(s)) return false;
            var kvp = s.Split(new[] { sep }, StringSplitOptions.None);
            if (kvp.Length != 3) return false;
            double x = 0.0, y = 0.0, z = 0.0;
            var ok = ((double.TryParse(kvp[0].Trim(), style, provider, out x) && double.TryParse(kvp[1].Trim(), style, provider, out y)) && double.TryParse(kvp[2].Trim(), style, provider, out z));
            result = ok ? new dVector3(x, y, z) : Zero;
            return ok;
        }
        
        /// <summary>
        /// Returns true iff distance between lhs and rhs is less than or equal to epsilon
        /// </summary>
        public static bool ApproxEqual(dVector3 lhs, dVector3 rhs, double eps = 0.1d) => Distance(lhs, rhs) <= eps;
        
        /// <summary>
        /// OuterProduct treats the first parameter c as a column Vectortor (Matrix with one column) and the second parameter r as a row Vectortor (Matrix with one row) and does a linear algebraic Matrix multiply c * r, yielding a Matrix whose number of rows is the number of components in c and whose number of columns is the number of components in r.
        /// </summary>
        public static dMatrix3x2 OuterProduct(dVector2 c, dVector3 r) => new dMatrix3x2(c.x * r.x, c.y * r.x, c.x * r.y, c.y * r.y, c.x * r.z, c.y * r.z);
        
        /// <summary>
        /// OuterProduct treats the first parameter c as a column Vectortor (Matrix with one column) and the second parameter r as a row Vectortor (Matrix with one row) and does a linear algebraic Matrix multiply c * r, yielding a Matrix whose number of rows is the number of components in c and whose number of columns is the number of components in r.
        /// </summary>
        public static dMatrix2x3 OuterProduct(dVector3 c, dVector2 r) => new dMatrix2x3(c.x * r.x, c.y * r.x, c.z * r.x, c.x * r.y, c.y * r.y, c.z * r.y);
        
        /// <summary>
        /// OuterProduct treats the first parameter c as a column Vectortor (Matrix with one column) and the second parameter r as a row Vectortor (Matrix with one row) and does a linear algebraic Matrix multiply c * r, yielding a Matrix whose number of rows is the number of components in c and whose number of columns is the number of components in r.
        /// </summary>
        public static dMatrix3 OuterProduct(dVector3 c, dVector3 r) => new dMatrix3(c.x * r.x, c.y * r.x, c.z * r.x, c.x * r.y, c.y * r.y, c.z * r.y, c.x * r.z, c.y * r.z, c.z * r.z);
        
        /// <summary>
        /// OuterProduct treats the first parameter c as a column Vectortor (Matrix with one column) and the second parameter r as a row Vectortor (Matrix with one row) and does a linear algebraic Matrix multiply c * r, yielding a Matrix whose number of rows is the number of components in c and whose number of columns is the number of components in r.
        /// </summary>
        public static dMatrix3x4 OuterProduct(dVector4 c, dVector3 r) => new dMatrix3x4(c.x * r.x, c.y * r.x, c.z * r.x, c.w * r.x, c.x * r.y, c.y * r.y, c.z * r.y, c.w * r.y, c.x * r.z, c.y * r.z, c.z * r.z, c.w * r.z);
        
        /// <summary>
        /// OuterProduct treats the first parameter c as a column Vectortor (Matrix with one column) and the second parameter r as a row Vectortor (Matrix with one row) and does a linear algebraic Matrix multiply c * r, yielding a Matrix whose number of rows is the number of components in c and whose number of columns is the number of components in r.
        /// </summary>
        public static dMatrix4x3 OuterProduct(dVector3 c, dVector4 r) => new dMatrix4x3(c.x * r.x, c.y * r.x, c.z * r.x, c.x * r.y, c.y * r.y, c.z * r.y, c.x * r.z, c.y * r.z, c.z * r.z, c.x * r.w, c.y * r.w, c.z * r.w);
        
        /// <summary>
        /// Returns the inner product (dot product, scalar product) of the two Vectortors.
        /// </summary>
        public static double Dot(dVector3 lhs, dVector3 rhs) => ((lhs.x * rhs.x + lhs.y * rhs.y) + lhs.z * rhs.z);
        
        /// <summary>
        /// Returns the euclidean distance between the two Vectortors.
        /// </summary>
        public static double Distance(dVector3 lhs, dVector3 rhs) => (lhs - rhs).Length;
        
        /// <summary>
        /// Returns the squared euclidean distance between the two Vectortors.
        /// </summary>
        public static double DistanceSqr(dVector3 lhs, dVector3 rhs) => (lhs - rhs).LengthSqr;
        
        /// <summary>
        /// Calculate the reflection direction for an incident Vectortor (N should be normalized in order to achieve the desired result).
        /// </summary>
        public static dVector3 Reflect(dVector3 I, dVector3 N) => I - 2 * Dot(N, I) * N;
        
        /// <summary>
        /// Calculate the refraction direction for an incident Vectortor (The input parameters I and N should be normalized in order to achieve the desired result).
        /// </summary>
        public static dVector3 Refract(dVector3 I, dVector3 N, double eta)
        {
            var dNI = Dot(N, I);
            var k = 1 - eta * eta * (1 - dNI * dNI);
            if (k < 0) return Zero;
            return eta * I - (eta * dNI + (double)Math.Sqrt(k)) * N;
        }
        
        /// <summary>
        /// Returns a Vectortor pointing in the same direction as another (faceforward orients a Vectortor to point away from a surface as defined by its normal. If dot(Nref, I) is negative faceforward returns N, otherwise it returns -N).
        /// </summary>
        public static dVector3 FaceForward(dVector3 N, dVector3 I, dVector3 Nref) => Dot(Nref, I) < 0 ? N : -N;
        
        /// <summary>
        /// Returns the outer product (cross product, Vectortor product) of the two Vectortors.
        /// </summary>
        public static dVector3 Cross(dVector3 l, dVector3 r) => new dVector3(l.y * r.z - l.z * r.y, l.z * r.x - l.x * r.z, l.x * r.y - l.y * r.x);
        
        /// <summary>
        /// Returns a dVector3 with independent and identically distributed uniform values between 0.0 and 1.0.
        /// </summary>
        public static dVector3 Random(System.Random random) => new dVector3((double)random.NextDouble(), (double)random.NextDouble(), (double)random.NextDouble());
        
        /// <summary>
        /// Returns a dVector3 with independent and identically distributed uniform values between -1.0 and 1.0.
        /// </summary>
        public static dVector3 RandomSigned(System.Random random) => new dVector3((double)(random.NextDouble() * 2.0 - 1.0), (double)(random.NextDouble() * 2.0 - 1.0), (double)(random.NextDouble() * 2.0 - 1.0));
        
        /// <summary>
        /// Returns a dVector3 with independent and identically distributed values according to a normal distribution (zero mean, unit variance).
        /// </summary>
        public static dVector3 RandomNormal(System.Random random) => new dVector3((double)(Math.Cos(2 * Math.PI * random.NextDouble()) * Math.Sqrt(-2.0 * Math.Log(random.NextDouble()))), (double)(Math.Cos(2 * Math.PI * random.NextDouble()) * Math.Sqrt(-2.0 * Math.Log(random.NextDouble()))), (double)(Math.Cos(2 * Math.PI * random.NextDouble()) * Math.Sqrt(-2.0 * Math.Log(random.NextDouble()))));

        #endregion


        #region Component-Wise Static Functions
        
        /// <summary>
        /// Returns a bVector3 from component-wise application of Equal (lhs == rhs).
        /// </summary>
        public static bVector3 Equal(dVector3 lhs, dVector3 rhs) => new bVector3(lhs.x == rhs.x, lhs.y == rhs.y, lhs.z == rhs.z);
        
        /// <summary>
        /// Returns a bVector3 from component-wise application of Equal (lhs == rhs).
        /// </summary>
        public static bVector3 Equal(dVector3 lhs, double rhs) => new bVector3(lhs.x == rhs, lhs.y == rhs, lhs.z == rhs);
        
        /// <summary>
        /// Returns a bVector3 from component-wise application of Equal (lhs == rhs).
        /// </summary>
        public static bVector3 Equal(double lhs, dVector3 rhs) => new bVector3(lhs == rhs.x, lhs == rhs.y, lhs == rhs.z);
        
        /// <summary>
        /// Returns a bVector from the application of Equal (lhs == rhs).
        /// </summary>
        public static bVector3 Equal(double lhs, double rhs) => new bVector3(lhs == rhs);
        
        /// <summary>
        /// Returns a bVector3 from component-wise application of NotEqual (lhs != rhs).
        /// </summary>
        public static bVector3 NotEqual(dVector3 lhs, dVector3 rhs) => new bVector3(lhs.x != rhs.x, lhs.y != rhs.y, lhs.z != rhs.z);
        
        /// <summary>
        /// Returns a bVector3 from component-wise application of NotEqual (lhs != rhs).
        /// </summary>
        public static bVector3 NotEqual(dVector3 lhs, double rhs) => new bVector3(lhs.x != rhs, lhs.y != rhs, lhs.z != rhs);
        
        /// <summary>
        /// Returns a bVector3 from component-wise application of NotEqual (lhs != rhs).
        /// </summary>
        public static bVector3 NotEqual(double lhs, dVector3 rhs) => new bVector3(lhs != rhs.x, lhs != rhs.y, lhs != rhs.z);
        
        /// <summary>
        /// Returns a bVector from the application of NotEqual (lhs != rhs).
        /// </summary>
        public static bVector3 NotEqual(double lhs, double rhs) => new bVector3(lhs != rhs);
        
        /// <summary>
        /// Returns a bVector3 from component-wise application of GreaterThan (lhs &gt; rhs).
        /// </summary>
        public static bVector3 GreaterThan(dVector3 lhs, dVector3 rhs) => new bVector3(lhs.x > rhs.x, lhs.y > rhs.y, lhs.z > rhs.z);
        
        /// <summary>
        /// Returns a bVector3 from component-wise application of GreaterThan (lhs &gt; rhs).
        /// </summary>
        public static bVector3 GreaterThan(dVector3 lhs, double rhs) => new bVector3(lhs.x > rhs, lhs.y > rhs, lhs.z > rhs);
        
        /// <summary>
        /// Returns a bVector3 from component-wise application of GreaterThan (lhs &gt; rhs).
        /// </summary>
        public static bVector3 GreaterThan(double lhs, dVector3 rhs) => new bVector3(lhs > rhs.x, lhs > rhs.y, lhs > rhs.z);
        
        /// <summary>
        /// Returns a bVector from the application of GreaterThan (lhs &gt; rhs).
        /// </summary>
        public static bVector3 GreaterThan(double lhs, double rhs) => new bVector3(lhs > rhs);
        
        /// <summary>
        /// Returns a bVector3 from component-wise application of GreaterThanEqual (lhs &gt;= rhs).
        /// </summary>
        public static bVector3 GreaterThanEqual(dVector3 lhs, dVector3 rhs) => new bVector3(lhs.x >= rhs.x, lhs.y >= rhs.y, lhs.z >= rhs.z);
        
        /// <summary>
        /// Returns a bVector3 from component-wise application of GreaterThanEqual (lhs &gt;= rhs).
        /// </summary>
        public static bVector3 GreaterThanEqual(dVector3 lhs, double rhs) => new bVector3(lhs.x >= rhs, lhs.y >= rhs, lhs.z >= rhs);
        
        /// <summary>
        /// Returns a bVector3 from component-wise application of GreaterThanEqual (lhs &gt;= rhs).
        /// </summary>
        public static bVector3 GreaterThanEqual(double lhs, dVector3 rhs) => new bVector3(lhs >= rhs.x, lhs >= rhs.y, lhs >= rhs.z);
        
        /// <summary>
        /// Returns a bVector from the application of GreaterThanEqual (lhs &gt;= rhs).
        /// </summary>
        public static bVector3 GreaterThanEqual(double lhs, double rhs) => new bVector3(lhs >= rhs);
        
        /// <summary>
        /// Returns a bVector3 from component-wise application of LesserThan (lhs &lt; rhs).
        /// </summary>
        public static bVector3 LesserThan(dVector3 lhs, dVector3 rhs) => new bVector3(lhs.x < rhs.x, lhs.y < rhs.y, lhs.z < rhs.z);
        
        /// <summary>
        /// Returns a bVector3 from component-wise application of LesserThan (lhs &lt; rhs).
        /// </summary>
        public static bVector3 LesserThan(dVector3 lhs, double rhs) => new bVector3(lhs.x < rhs, lhs.y < rhs, lhs.z < rhs);
        
        /// <summary>
        /// Returns a bVector3 from component-wise application of LesserThan (lhs &lt; rhs).
        /// </summary>
        public static bVector3 LesserThan(double lhs, dVector3 rhs) => new bVector3(lhs < rhs.x, lhs < rhs.y, lhs < rhs.z);
        
        /// <summary>
        /// Returns a bVector from the application of LesserThan (lhs &lt; rhs).
        /// </summary>
        public static bVector3 LesserThan(double lhs, double rhs) => new bVector3(lhs < rhs);
        
        /// <summary>
        /// Returns a bVector3 from component-wise application of LesserThanEqual (lhs &lt;= rhs).
        /// </summary>
        public static bVector3 LesserThanEqual(dVector3 lhs, dVector3 rhs) => new bVector3(lhs.x <= rhs.x, lhs.y <= rhs.y, lhs.z <= rhs.z);
        
        /// <summary>
        /// Returns a bVector3 from component-wise application of LesserThanEqual (lhs &lt;= rhs).
        /// </summary>
        public static bVector3 LesserThanEqual(dVector3 lhs, double rhs) => new bVector3(lhs.x <= rhs, lhs.y <= rhs, lhs.z <= rhs);
        
        /// <summary>
        /// Returns a bVector3 from component-wise application of LesserThanEqual (lhs &lt;= rhs).
        /// </summary>
        public static bVector3 LesserThanEqual(double lhs, dVector3 rhs) => new bVector3(lhs <= rhs.x, lhs <= rhs.y, lhs <= rhs.z);
        
        /// <summary>
        /// Returns a bVector from the application of LesserThanEqual (lhs &lt;= rhs).
        /// </summary>
        public static bVector3 LesserThanEqual(double lhs, double rhs) => new bVector3(lhs <= rhs);
        
        /// <summary>
        /// Returns a bVector3 from component-wise application of IsInfinity (double.IsInfinity(v)).
        /// </summary>
        public static bVector3 IsInfinity(dVector3 v) => new bVector3(double.IsInfinity(v.x), double.IsInfinity(v.y), double.IsInfinity(v.z));
        
        /// <summary>
        /// Returns a bVector from the application of IsInfinity (double.IsInfinity(v)).
        /// </summary>
        public static bVector3 IsInfinity(double v) => new bVector3(double.IsInfinity(v));
        
        /// <summary>
        /// Returns a bVector3 from component-wise application of IsFinite (!double.IsNaN(v) &amp;&amp; !double.IsInfinity(v)).
        /// </summary>
        public static bVector3 IsFinite(dVector3 v) => new bVector3(!double.IsNaN(v.x) && !double.IsInfinity(v.x), !double.IsNaN(v.y) && !double.IsInfinity(v.y), !double.IsNaN(v.z) && !double.IsInfinity(v.z));
        
        /// <summary>
        /// Returns a bVector from the application of IsFinite (!double.IsNaN(v) &amp;&amp; !double.IsInfinity(v)).
        /// </summary>
        public static bVector3 IsFinite(double v) => new bVector3(!double.IsNaN(v) && !double.IsInfinity(v));
        
        /// <summary>
        /// Returns a bVector3 from component-wise application of IsNaN (double.IsNaN(v)).
        /// </summary>
        public static bVector3 IsNaN(dVector3 v) => new bVector3(double.IsNaN(v.x), double.IsNaN(v.y), double.IsNaN(v.z));
        
        /// <summary>
        /// Returns a bVector from the application of IsNaN (double.IsNaN(v)).
        /// </summary>
        public static bVector3 IsNaN(double v) => new bVector3(double.IsNaN(v));
        
        /// <summary>
        /// Returns a bVector3 from component-wise application of IsNegativeInfinity (double.IsNegativeInfinity(v)).
        /// </summary>
        public static bVector3 IsNegativeInfinity(dVector3 v) => new bVector3(double.IsNegativeInfinity(v.x), double.IsNegativeInfinity(v.y), double.IsNegativeInfinity(v.z));
        
        /// <summary>
        /// Returns a bVector from the application of IsNegativeInfinity (double.IsNegativeInfinity(v)).
        /// </summary>
        public static bVector3 IsNegativeInfinity(double v) => new bVector3(double.IsNegativeInfinity(v));
        
        /// <summary>
        /// Returns a bVector3 from component-wise application of IsPositiveInfinity (double.IsPositiveInfinity(v)).
        /// </summary>
        public static bVector3 IsPositiveInfinity(dVector3 v) => new bVector3(double.IsPositiveInfinity(v.x), double.IsPositiveInfinity(v.y), double.IsPositiveInfinity(v.z));
        
        /// <summary>
        /// Returns a bVector from the application of IsPositiveInfinity (double.IsPositiveInfinity(v)).
        /// </summary>
        public static bVector3 IsPositiveInfinity(double v) => new bVector3(double.IsPositiveInfinity(v));
        
        /// <summary>
        /// Returns a dVector3 from component-wise application of Abs (Math.Abs(v)).
        /// </summary>
        public static dVector3 Abs(dVector3 v) => new dVector3(Math.Abs(v.x), Math.Abs(v.y), Math.Abs(v.z));
        
        /// <summary>
        /// Returns a dVector from the application of Abs (Math.Abs(v)).
        /// </summary>
        public static dVector3 Abs(double v) => new dVector3(Math.Abs(v));
        
        /// <summary>
        /// Returns a dVector3 from component-wise application of HermiteInterpolationOrder3 ((3 - 2 * v) * v * v).
        /// </summary>
        public static dVector3 HermiteInterpolationOrder3(dVector3 v) => new dVector3((3 - 2 * v.x) * v.x * v.x, (3 - 2 * v.y) * v.y * v.y, (3 - 2 * v.z) * v.z * v.z);
        
        /// <summary>
        /// Returns a dVector from the application of HermiteInterpolationOrder3 ((3 - 2 * v) * v * v).
        /// </summary>
        public static dVector3 HermiteInterpolationOrder3(double v) => new dVector3((3 - 2 * v) * v * v);
        
        /// <summary>
        /// Returns a dVector3 from component-wise application of HermiteInterpolationOrder5 (((6 * v - 15) * v + 10) * v * v * v).
        /// </summary>
        public static dVector3 HermiteInterpolationOrder5(dVector3 v) => new dVector3(((6 * v.x - 15) * v.x + 10) * v.x * v.x * v.x, ((6 * v.y - 15) * v.y + 10) * v.y * v.y * v.y, ((6 * v.z - 15) * v.z + 10) * v.z * v.z * v.z);
        
        /// <summary>
        /// Returns a dVector from the application of HermiteInterpolationOrder5 (((6 * v - 15) * v + 10) * v * v * v).
        /// </summary>
        public static dVector3 HermiteInterpolationOrder5(double v) => new dVector3(((6 * v - 15) * v + 10) * v * v * v);
        
        /// <summary>
        /// Returns a dVector3 from component-wise application of Sqr (v * v).
        /// </summary>
        public static dVector3 Sqr(dVector3 v) => new dVector3(v.x * v.x, v.y * v.y, v.z * v.z);
        
        /// <summary>
        /// Returns a dVector from the application of Sqr (v * v).
        /// </summary>
        public static dVector3 Sqr(double v) => new dVector3(v * v);
        
        /// <summary>
        /// Returns a dVector3 from component-wise application of Pow2 (v * v).
        /// </summary>
        public static dVector3 Pow2(dVector3 v) => new dVector3(v.x * v.x, v.y * v.y, v.z * v.z);
        
        /// <summary>
        /// Returns a dVector from the application of Pow2 (v * v).
        /// </summary>
        public static dVector3 Pow2(double v) => new dVector3(v * v);
        
        /// <summary>
        /// Returns a dVector3 from component-wise application of Pow3 (v * v * v).
        /// </summary>
        public static dVector3 Pow3(dVector3 v) => new dVector3(v.x * v.x * v.x, v.y * v.y * v.y, v.z * v.z * v.z);
        
        /// <summary>
        /// Returns a dVector from the application of Pow3 (v * v * v).
        /// </summary>
        public static dVector3 Pow3(double v) => new dVector3(v * v * v);
        
        /// <summary>
        /// Returns a dVector3 from component-wise application of Step (v &gt;= 0.0 ? 1.0 : 0.0).
        /// </summary>
        public static dVector3 Step(dVector3 v) => new dVector3(v.x >= 0.0 ? 1.0 : 0.0, v.y >= 0.0 ? 1.0 : 0.0, v.z >= 0.0 ? 1.0 : 0.0);
        
        /// <summary>
        /// Returns a dVector from the application of Step (v &gt;= 0.0 ? 1.0 : 0.0).
        /// </summary>
        public static dVector3 Step(double v) => new dVector3(v >= 0.0 ? 1.0 : 0.0);
        
        /// <summary>
        /// Returns a dVector3 from component-wise application of Sqrt ((double)Math.Sqrt((double)v)).
        /// </summary>
        public static dVector3 Sqrt(dVector3 v) => new dVector3((double)Math.Sqrt((double)v.x), (double)Math.Sqrt((double)v.y), (double)Math.Sqrt((double)v.z));
        
        /// <summary>
        /// Returns a dVector from the application of Sqrt ((double)Math.Sqrt((double)v)).
        /// </summary>
        public static dVector3 Sqrt(double v) => new dVector3((double)Math.Sqrt((double)v));
        
        /// <summary>
        /// Returns a dVector3 from component-wise application of InverseSqrt ((double)(1.0 / Math.Sqrt((double)v))).
        /// </summary>
        public static dVector3 InverseSqrt(dVector3 v) => new dVector3((double)(1.0 / Math.Sqrt((double)v.x)), (double)(1.0 / Math.Sqrt((double)v.y)), (double)(1.0 / Math.Sqrt((double)v.z)));
        
        /// <summary>
        /// Returns a dVector from the application of InverseSqrt ((double)(1.0 / Math.Sqrt((double)v))).
        /// </summary>
        public static dVector3 InverseSqrt(double v) => new dVector3((double)(1.0 / Math.Sqrt((double)v)));
        
        /// <summary>
        /// Returns a iVector3 from component-wise application of Sign (Math.Sign(v)).
        /// </summary>
        public static iVector3 Sign(dVector3 v) => new iVector3(Math.Sign(v.x), Math.Sign(v.y), Math.Sign(v.z));
        
        /// <summary>
        /// Returns a iVector from the application of Sign (Math.Sign(v)).
        /// </summary>
        public static iVector3 Sign(double v) => new iVector3(Math.Sign(v));
        
        /// <summary>
        /// Returns a dVector3 from component-wise application of Max (Math.Max(lhs, rhs)).
        /// </summary>
        public static dVector3 Max(dVector3 lhs, dVector3 rhs) => new dVector3(Math.Max(lhs.x, rhs.x), Math.Max(lhs.y, rhs.y), Math.Max(lhs.z, rhs.z));
        
        /// <summary>
        /// Returns a dVector3 from component-wise application of Max (Math.Max(lhs, rhs)).
        /// </summary>
        public static dVector3 Max(dVector3 lhs, double rhs) => new dVector3(Math.Max(lhs.x, rhs), Math.Max(lhs.y, rhs), Math.Max(lhs.z, rhs));
        
        /// <summary>
        /// Returns a dVector3 from component-wise application of Max (Math.Max(lhs, rhs)).
        /// </summary>
        public static dVector3 Max(double lhs, dVector3 rhs) => new dVector3(Math.Max(lhs, rhs.x), Math.Max(lhs, rhs.y), Math.Max(lhs, rhs.z));
        
        /// <summary>
        /// Returns a dVector from the application of Max (Math.Max(lhs, rhs)).
        /// </summary>
        public static dVector3 Max(double lhs, double rhs) => new dVector3(Math.Max(lhs, rhs));
        
        /// <summary>
        /// Returns a dVector3 from component-wise application of Min (Math.Min(lhs, rhs)).
        /// </summary>
        public static dVector3 Min(dVector3 lhs, dVector3 rhs) => new dVector3(Math.Min(lhs.x, rhs.x), Math.Min(lhs.y, rhs.y), Math.Min(lhs.z, rhs.z));
        
        /// <summary>
        /// Returns a dVector3 from component-wise application of Min (Math.Min(lhs, rhs)).
        /// </summary>
        public static dVector3 Min(dVector3 lhs, double rhs) => new dVector3(Math.Min(lhs.x, rhs), Math.Min(lhs.y, rhs), Math.Min(lhs.z, rhs));
        
        /// <summary>
        /// Returns a dVector3 from component-wise application of Min (Math.Min(lhs, rhs)).
        /// </summary>
        public static dVector3 Min(double lhs, dVector3 rhs) => new dVector3(Math.Min(lhs, rhs.x), Math.Min(lhs, rhs.y), Math.Min(lhs, rhs.z));
        
        /// <summary>
        /// Returns a dVector from the application of Min (Math.Min(lhs, rhs)).
        /// </summary>
        public static dVector3 Min(double lhs, double rhs) => new dVector3(Math.Min(lhs, rhs));
        
        /// <summary>
        /// Returns a dVector3 from component-wise application of Pow ((double)Math.Pow((double)lhs, (double)rhs)).
        /// </summary>
        public static dVector3 Pow(dVector3 lhs, dVector3 rhs) => new dVector3((double)Math.Pow((double)lhs.x, (double)rhs.x), (double)Math.Pow((double)lhs.y, (double)rhs.y), (double)Math.Pow((double)lhs.z, (double)rhs.z));
        
        /// <summary>
        /// Returns a dVector3 from component-wise application of Pow ((double)Math.Pow((double)lhs, (double)rhs)).
        /// </summary>
        public static dVector3 Pow(dVector3 lhs, double rhs) => new dVector3((double)Math.Pow((double)lhs.x, (double)rhs), (double)Math.Pow((double)lhs.y, (double)rhs), (double)Math.Pow((double)lhs.z, (double)rhs));
        
        /// <summary>
        /// Returns a dVector3 from component-wise application of Pow ((double)Math.Pow((double)lhs, (double)rhs)).
        /// </summary>
        public static dVector3 Pow(double lhs, dVector3 rhs) => new dVector3((double)Math.Pow((double)lhs, (double)rhs.x), (double)Math.Pow((double)lhs, (double)rhs.y), (double)Math.Pow((double)lhs, (double)rhs.z));
        
        /// <summary>
        /// Returns a dVector from the application of Pow ((double)Math.Pow((double)lhs, (double)rhs)).
        /// </summary>
        public static dVector3 Pow(double lhs, double rhs) => new dVector3((double)Math.Pow((double)lhs, (double)rhs));
        
        /// <summary>
        /// Returns a dVector3 from component-wise application of Log ((double)Math.Log((double)lhs, (double)rhs)).
        /// </summary>
        public static dVector3 Log(dVector3 lhs, dVector3 rhs) => new dVector3((double)Math.Log((double)lhs.x, (double)rhs.x), (double)Math.Log((double)lhs.y, (double)rhs.y), (double)Math.Log((double)lhs.z, (double)rhs.z));
        
        /// <summary>
        /// Returns a dVector3 from component-wise application of Log ((double)Math.Log((double)lhs, (double)rhs)).
        /// </summary>
        public static dVector3 Log(dVector3 lhs, double rhs) => new dVector3((double)Math.Log((double)lhs.x, (double)rhs), (double)Math.Log((double)lhs.y, (double)rhs), (double)Math.Log((double)lhs.z, (double)rhs));
        
        /// <summary>
        /// Returns a dVector3 from component-wise application of Log ((double)Math.Log((double)lhs, (double)rhs)).
        /// </summary>
        public static dVector3 Log(double lhs, dVector3 rhs) => new dVector3((double)Math.Log((double)lhs, (double)rhs.x), (double)Math.Log((double)lhs, (double)rhs.y), (double)Math.Log((double)lhs, (double)rhs.z));
        
        /// <summary>
        /// Returns a dVector from the application of Log ((double)Math.Log((double)lhs, (double)rhs)).
        /// </summary>
        public static dVector3 Log(double lhs, double rhs) => new dVector3((double)Math.Log((double)lhs, (double)rhs));
        
        /// <summary>
        /// Returns a dVector3 from component-wise application of Clamp (Math.Min(Math.Max(v, min), max)).
        /// </summary>
        public static dVector3 Clamp(dVector3 v, dVector3 min, dVector3 max) => new dVector3(Math.Min(Math.Max(v.x, min.x), max.x), Math.Min(Math.Max(v.y, min.y), max.y), Math.Min(Math.Max(v.z, min.z), max.z));
        
        /// <summary>
        /// Returns a dVector3 from component-wise application of Clamp (Math.Min(Math.Max(v, min), max)).
        /// </summary>
        public static dVector3 Clamp(dVector3 v, dVector3 min, double max) => new dVector3(Math.Min(Math.Max(v.x, min.x), max), Math.Min(Math.Max(v.y, min.y), max), Math.Min(Math.Max(v.z, min.z), max));
        
        /// <summary>
        /// Returns a dVector3 from component-wise application of Clamp (Math.Min(Math.Max(v, min), max)).
        /// </summary>
        public static dVector3 Clamp(dVector3 v, double min, dVector3 max) => new dVector3(Math.Min(Math.Max(v.x, min), max.x), Math.Min(Math.Max(v.y, min), max.y), Math.Min(Math.Max(v.z, min), max.z));
        
        /// <summary>
        /// Returns a dVector3 from component-wise application of Clamp (Math.Min(Math.Max(v, min), max)).
        /// </summary>
        public static dVector3 Clamp(dVector3 v, double min, double max) => new dVector3(Math.Min(Math.Max(v.x, min), max), Math.Min(Math.Max(v.y, min), max), Math.Min(Math.Max(v.z, min), max));
        
        /// <summary>
        /// Returns a dVector3 from component-wise application of Clamp (Math.Min(Math.Max(v, min), max)).
        /// </summary>
        public static dVector3 Clamp(double v, dVector3 min, dVector3 max) => new dVector3(Math.Min(Math.Max(v, min.x), max.x), Math.Min(Math.Max(v, min.y), max.y), Math.Min(Math.Max(v, min.z), max.z));
        
        /// <summary>
        /// Returns a dVector3 from component-wise application of Clamp (Math.Min(Math.Max(v, min), max)).
        /// </summary>
        public static dVector3 Clamp(double v, dVector3 min, double max) => new dVector3(Math.Min(Math.Max(v, min.x), max), Math.Min(Math.Max(v, min.y), max), Math.Min(Math.Max(v, min.z), max));
        
        /// <summary>
        /// Returns a dVector3 from component-wise application of Clamp (Math.Min(Math.Max(v, min), max)).
        /// </summary>
        public static dVector3 Clamp(double v, double min, dVector3 max) => new dVector3(Math.Min(Math.Max(v, min), max.x), Math.Min(Math.Max(v, min), max.y), Math.Min(Math.Max(v, min), max.z));
        
        /// <summary>
        /// Returns a dVector from the application of Clamp (Math.Min(Math.Max(v, min), max)).
        /// </summary>
        public static dVector3 Clamp(double v, double min, double max) => new dVector3(Math.Min(Math.Max(v, min), max));
        
        /// <summary>
        /// Returns a dVector3 from component-wise application of Mix (min * (1-a) + max * a).
        /// </summary>
        public static dVector3 Mix(dVector3 min, dVector3 max, dVector3 a) => new dVector3(min.x * (1-a.x) + max.x * a.x, min.y * (1-a.y) + max.y * a.y, min.z * (1-a.z) + max.z * a.z);
        
        /// <summary>
        /// Returns a dVector3 from component-wise application of Mix (min * (1-a) + max * a).
        /// </summary>
        public static dVector3 Mix(dVector3 min, dVector3 max, double a) => new dVector3(min.x * (1-a) + max.x * a, min.y * (1-a) + max.y * a, min.z * (1-a) + max.z * a);
        
        /// <summary>
        /// Returns a dVector3 from component-wise application of Mix (min * (1-a) + max * a).
        /// </summary>
        public static dVector3 Mix(dVector3 min, double max, dVector3 a) => new dVector3(min.x * (1-a.x) + max * a.x, min.y * (1-a.y) + max * a.y, min.z * (1-a.z) + max * a.z);
        
        /// <summary>
        /// Returns a dVector3 from component-wise application of Mix (min * (1-a) + max * a).
        /// </summary>
        public static dVector3 Mix(dVector3 min, double max, double a) => new dVector3(min.x * (1-a) + max * a, min.y * (1-a) + max * a, min.z * (1-a) + max * a);
        
        /// <summary>
        /// Returns a dVector3 from component-wise application of Mix (min * (1-a) + max * a).
        /// </summary>
        public static dVector3 Mix(double min, dVector3 max, dVector3 a) => new dVector3(min * (1-a.x) + max.x * a.x, min * (1-a.y) + max.y * a.y, min * (1-a.z) + max.z * a.z);
        
        /// <summary>
        /// Returns a dVector3 from component-wise application of Mix (min * (1-a) + max * a).
        /// </summary>
        public static dVector3 Mix(double min, dVector3 max, double a) => new dVector3(min * (1-a) + max.x * a, min * (1-a) + max.y * a, min * (1-a) + max.z * a);
        
        /// <summary>
        /// Returns a dVector3 from component-wise application of Mix (min * (1-a) + max * a).
        /// </summary>
        public static dVector3 Mix(double min, double max, dVector3 a) => new dVector3(min * (1-a.x) + max * a.x, min * (1-a.y) + max * a.y, min * (1-a.z) + max * a.z);
        
        /// <summary>
        /// Returns a dVector from the application of Mix (min * (1-a) + max * a).
        /// </summary>
        public static dVector3 Mix(double min, double max, double a) => new dVector3(min * (1-a) + max * a);
        
        /// <summary>
        /// Returns a dVector3 from component-wise application of Lerp (min * (1-a) + max * a).
        /// </summary>
        public static dVector3 Lerp(dVector3 min, dVector3 max, dVector3 a) => new dVector3(min.x * (1-a.x) + max.x * a.x, min.y * (1-a.y) + max.y * a.y, min.z * (1-a.z) + max.z * a.z);
        
        /// <summary>
        /// Returns a dVector3 from component-wise application of Lerp (min * (1-a) + max * a).
        /// </summary>
        public static dVector3 Lerp(dVector3 min, dVector3 max, double a) => new dVector3(min.x * (1-a) + max.x * a, min.y * (1-a) + max.y * a, min.z * (1-a) + max.z * a);
        
        /// <summary>
        /// Returns a dVector3 from component-wise application of Lerp (min * (1-a) + max * a).
        /// </summary>
        public static dVector3 Lerp(dVector3 min, double max, dVector3 a) => new dVector3(min.x * (1-a.x) + max * a.x, min.y * (1-a.y) + max * a.y, min.z * (1-a.z) + max * a.z);
        
        /// <summary>
        /// Returns a dVector3 from component-wise application of Lerp (min * (1-a) + max * a).
        /// </summary>
        public static dVector3 Lerp(dVector3 min, double max, double a) => new dVector3(min.x * (1-a) + max * a, min.y * (1-a) + max * a, min.z * (1-a) + max * a);
        
        /// <summary>
        /// Returns a dVector3 from component-wise application of Lerp (min * (1-a) + max * a).
        /// </summary>
        public static dVector3 Lerp(double min, dVector3 max, dVector3 a) => new dVector3(min * (1-a.x) + max.x * a.x, min * (1-a.y) + max.y * a.y, min * (1-a.z) + max.z * a.z);
        
        /// <summary>
        /// Returns a dVector3 from component-wise application of Lerp (min * (1-a) + max * a).
        /// </summary>
        public static dVector3 Lerp(double min, dVector3 max, double a) => new dVector3(min * (1-a) + max.x * a, min * (1-a) + max.y * a, min * (1-a) + max.z * a);
        
        /// <summary>
        /// Returns a dVector3 from component-wise application of Lerp (min * (1-a) + max * a).
        /// </summary>
        public static dVector3 Lerp(double min, double max, dVector3 a) => new dVector3(min * (1-a.x) + max * a.x, min * (1-a.y) + max * a.y, min * (1-a.z) + max * a.z);
        
        /// <summary>
        /// Returns a dVector from the application of Lerp (min * (1-a) + max * a).
        /// </summary>
        public static dVector3 Lerp(double min, double max, double a) => new dVector3(min * (1-a) + max * a);
        
        /// <summary>
        /// Returns a dVector3 from component-wise application of Smoothstep (((v - edge0) / (edge1 - edge0)).Clamp().HermiteInterpolationOrder3()).
        /// </summary>
        public static dVector3 Smoothstep(dVector3 edge0, dVector3 edge1, dVector3 v) => new dVector3(((v.x - edge0.x) / (edge1.x - edge0.x)).Clamp().HermiteInterpolationOrder3(), ((v.y - edge0.y) / (edge1.y - edge0.y)).Clamp().HermiteInterpolationOrder3(), ((v.z - edge0.z) / (edge1.z - edge0.z)).Clamp().HermiteInterpolationOrder3());
        
        /// <summary>
        /// Returns a dVector3 from component-wise application of Smoothstep (((v - edge0) / (edge1 - edge0)).Clamp().HermiteInterpolationOrder3()).
        /// </summary>
        public static dVector3 Smoothstep(dVector3 edge0, dVector3 edge1, double v) => new dVector3(((v - edge0.x) / (edge1.x - edge0.x)).Clamp().HermiteInterpolationOrder3(), ((v - edge0.y) / (edge1.y - edge0.y)).Clamp().HermiteInterpolationOrder3(), ((v - edge0.z) / (edge1.z - edge0.z)).Clamp().HermiteInterpolationOrder3());
        
        /// <summary>
        /// Returns a dVector3 from component-wise application of Smoothstep (((v - edge0) / (edge1 - edge0)).Clamp().HermiteInterpolationOrder3()).
        /// </summary>
        public static dVector3 Smoothstep(dVector3 edge0, double edge1, dVector3 v) => new dVector3(((v.x - edge0.x) / (edge1 - edge0.x)).Clamp().HermiteInterpolationOrder3(), ((v.y - edge0.y) / (edge1 - edge0.y)).Clamp().HermiteInterpolationOrder3(), ((v.z - edge0.z) / (edge1 - edge0.z)).Clamp().HermiteInterpolationOrder3());
        
        /// <summary>
        /// Returns a dVector3 from component-wise application of Smoothstep (((v - edge0) / (edge1 - edge0)).Clamp().HermiteInterpolationOrder3()).
        /// </summary>
        public static dVector3 Smoothstep(dVector3 edge0, double edge1, double v) => new dVector3(((v - edge0.x) / (edge1 - edge0.x)).Clamp().HermiteInterpolationOrder3(), ((v - edge0.y) / (edge1 - edge0.y)).Clamp().HermiteInterpolationOrder3(), ((v - edge0.z) / (edge1 - edge0.z)).Clamp().HermiteInterpolationOrder3());
        
        /// <summary>
        /// Returns a dVector3 from component-wise application of Smoothstep (((v - edge0) / (edge1 - edge0)).Clamp().HermiteInterpolationOrder3()).
        /// </summary>
        public static dVector3 Smoothstep(double edge0, dVector3 edge1, dVector3 v) => new dVector3(((v.x - edge0) / (edge1.x - edge0)).Clamp().HermiteInterpolationOrder3(), ((v.y - edge0) / (edge1.y - edge0)).Clamp().HermiteInterpolationOrder3(), ((v.z - edge0) / (edge1.z - edge0)).Clamp().HermiteInterpolationOrder3());
        
        /// <summary>
        /// Returns a dVector3 from component-wise application of Smoothstep (((v - edge0) / (edge1 - edge0)).Clamp().HermiteInterpolationOrder3()).
        /// </summary>
        public static dVector3 Smoothstep(double edge0, dVector3 edge1, double v) => new dVector3(((v - edge0) / (edge1.x - edge0)).Clamp().HermiteInterpolationOrder3(), ((v - edge0) / (edge1.y - edge0)).Clamp().HermiteInterpolationOrder3(), ((v - edge0) / (edge1.z - edge0)).Clamp().HermiteInterpolationOrder3());
        
        /// <summary>
        /// Returns a dVector3 from component-wise application of Smoothstep (((v - edge0) / (edge1 - edge0)).Clamp().HermiteInterpolationOrder3()).
        /// </summary>
        public static dVector3 Smoothstep(double edge0, double edge1, dVector3 v) => new dVector3(((v.x - edge0) / (edge1 - edge0)).Clamp().HermiteInterpolationOrder3(), ((v.y - edge0) / (edge1 - edge0)).Clamp().HermiteInterpolationOrder3(), ((v.z - edge0) / (edge1 - edge0)).Clamp().HermiteInterpolationOrder3());
        
        /// <summary>
        /// Returns a dVector from the application of Smoothstep (((v - edge0) / (edge1 - edge0)).Clamp().HermiteInterpolationOrder3()).
        /// </summary>
        public static dVector3 Smoothstep(double edge0, double edge1, double v) => new dVector3(((v - edge0) / (edge1 - edge0)).Clamp().HermiteInterpolationOrder3());
        
        /// <summary>
        /// Returns a dVector3 from component-wise application of Smootherstep (((v - edge0) / (edge1 - edge0)).Clamp().HermiteInterpolationOrder5()).
        /// </summary>
        public static dVector3 Smootherstep(dVector3 edge0, dVector3 edge1, dVector3 v) => new dVector3(((v.x - edge0.x) / (edge1.x - edge0.x)).Clamp().HermiteInterpolationOrder5(), ((v.y - edge0.y) / (edge1.y - edge0.y)).Clamp().HermiteInterpolationOrder5(), ((v.z - edge0.z) / (edge1.z - edge0.z)).Clamp().HermiteInterpolationOrder5());
        
        /// <summary>
        /// Returns a dVector3 from component-wise application of Smootherstep (((v - edge0) / (edge1 - edge0)).Clamp().HermiteInterpolationOrder5()).
        /// </summary>
        public static dVector3 Smootherstep(dVector3 edge0, dVector3 edge1, double v) => new dVector3(((v - edge0.x) / (edge1.x - edge0.x)).Clamp().HermiteInterpolationOrder5(), ((v - edge0.y) / (edge1.y - edge0.y)).Clamp().HermiteInterpolationOrder5(), ((v - edge0.z) / (edge1.z - edge0.z)).Clamp().HermiteInterpolationOrder5());
        
        /// <summary>
        /// Returns a dVector3 from component-wise application of Smootherstep (((v - edge0) / (edge1 - edge0)).Clamp().HermiteInterpolationOrder5()).
        /// </summary>
        public static dVector3 Smootherstep(dVector3 edge0, double edge1, dVector3 v) => new dVector3(((v.x - edge0.x) / (edge1 - edge0.x)).Clamp().HermiteInterpolationOrder5(), ((v.y - edge0.y) / (edge1 - edge0.y)).Clamp().HermiteInterpolationOrder5(), ((v.z - edge0.z) / (edge1 - edge0.z)).Clamp().HermiteInterpolationOrder5());
        
        /// <summary>
        /// Returns a dVector3 from component-wise application of Smootherstep (((v - edge0) / (edge1 - edge0)).Clamp().HermiteInterpolationOrder5()).
        /// </summary>
        public static dVector3 Smootherstep(dVector3 edge0, double edge1, double v) => new dVector3(((v - edge0.x) / (edge1 - edge0.x)).Clamp().HermiteInterpolationOrder5(), ((v - edge0.y) / (edge1 - edge0.y)).Clamp().HermiteInterpolationOrder5(), ((v - edge0.z) / (edge1 - edge0.z)).Clamp().HermiteInterpolationOrder5());
        
        /// <summary>
        /// Returns a dVector3 from component-wise application of Smootherstep (((v - edge0) / (edge1 - edge0)).Clamp().HermiteInterpolationOrder5()).
        /// </summary>
        public static dVector3 Smootherstep(double edge0, dVector3 edge1, dVector3 v) => new dVector3(((v.x - edge0) / (edge1.x - edge0)).Clamp().HermiteInterpolationOrder5(), ((v.y - edge0) / (edge1.y - edge0)).Clamp().HermiteInterpolationOrder5(), ((v.z - edge0) / (edge1.z - edge0)).Clamp().HermiteInterpolationOrder5());
        
        /// <summary>
        /// Returns a dVector3 from component-wise application of Smootherstep (((v - edge0) / (edge1 - edge0)).Clamp().HermiteInterpolationOrder5()).
        /// </summary>
        public static dVector3 Smootherstep(double edge0, dVector3 edge1, double v) => new dVector3(((v - edge0) / (edge1.x - edge0)).Clamp().HermiteInterpolationOrder5(), ((v - edge0) / (edge1.y - edge0)).Clamp().HermiteInterpolationOrder5(), ((v - edge0) / (edge1.z - edge0)).Clamp().HermiteInterpolationOrder5());
        
        /// <summary>
        /// Returns a dVector3 from component-wise application of Smootherstep (((v - edge0) / (edge1 - edge0)).Clamp().HermiteInterpolationOrder5()).
        /// </summary>
        public static dVector3 Smootherstep(double edge0, double edge1, dVector3 v) => new dVector3(((v.x - edge0) / (edge1 - edge0)).Clamp().HermiteInterpolationOrder5(), ((v.y - edge0) / (edge1 - edge0)).Clamp().HermiteInterpolationOrder5(), ((v.z - edge0) / (edge1 - edge0)).Clamp().HermiteInterpolationOrder5());
        
        /// <summary>
        /// Returns a dVector from the application of Smootherstep (((v - edge0) / (edge1 - edge0)).Clamp().HermiteInterpolationOrder5()).
        /// </summary>
        public static dVector3 Smootherstep(double edge0, double edge1, double v) => new dVector3(((v - edge0) / (edge1 - edge0)).Clamp().HermiteInterpolationOrder5());
        
        /// <summary>
        /// Returns a dVector3 from component-wise application of Fma (a * b + c).
        /// </summary>
        public static dVector3 Fma(dVector3 a, dVector3 b, dVector3 c) => new dVector3(a.x * b.x + c.x, a.y * b.y + c.y, a.z * b.z + c.z);
        
        /// <summary>
        /// Returns a dVector3 from component-wise application of Fma (a * b + c).
        /// </summary>
        public static dVector3 Fma(dVector3 a, dVector3 b, double c) => new dVector3(a.x * b.x + c, a.y * b.y + c, a.z * b.z + c);
        
        /// <summary>
        /// Returns a dVector3 from component-wise application of Fma (a * b + c).
        /// </summary>
        public static dVector3 Fma(dVector3 a, double b, dVector3 c) => new dVector3(a.x * b + c.x, a.y * b + c.y, a.z * b + c.z);
        
        /// <summary>
        /// Returns a dVector3 from component-wise application of Fma (a * b + c).
        /// </summary>
        public static dVector3 Fma(dVector3 a, double b, double c) => new dVector3(a.x * b + c, a.y * b + c, a.z * b + c);
        
        /// <summary>
        /// Returns a dVector3 from component-wise application of Fma (a * b + c).
        /// </summary>
        public static dVector3 Fma(double a, dVector3 b, dVector3 c) => new dVector3(a * b.x + c.x, a * b.y + c.y, a * b.z + c.z);
        
        /// <summary>
        /// Returns a dVector3 from component-wise application of Fma (a * b + c).
        /// </summary>
        public static dVector3 Fma(double a, dVector3 b, double c) => new dVector3(a * b.x + c, a * b.y + c, a * b.z + c);
        
        /// <summary>
        /// Returns a dVector3 from component-wise application of Fma (a * b + c).
        /// </summary>
        public static dVector3 Fma(double a, double b, dVector3 c) => new dVector3(a * b + c.x, a * b + c.y, a * b + c.z);
        
        /// <summary>
        /// Returns a dVector from the application of Fma (a * b + c).
        /// </summary>
        public static dVector3 Fma(double a, double b, double c) => new dVector3(a * b + c);
        
        /// <summary>
        /// Returns a dVector3 from component-wise application of Add (lhs + rhs).
        /// </summary>
        public static dVector3 Add(dVector3 lhs, dVector3 rhs) => new dVector3(lhs.x + rhs.x, lhs.y + rhs.y, lhs.z + rhs.z);
        
        /// <summary>
        /// Returns a dVector3 from component-wise application of Add (lhs + rhs).
        /// </summary>
        public static dVector3 Add(dVector3 lhs, double rhs) => new dVector3(lhs.x + rhs, lhs.y + rhs, lhs.z + rhs);
        
        /// <summary>
        /// Returns a dVector3 from component-wise application of Add (lhs + rhs).
        /// </summary>
        public static dVector3 Add(double lhs, dVector3 rhs) => new dVector3(lhs + rhs.x, lhs + rhs.y, lhs + rhs.z);
        
        /// <summary>
        /// Returns a dVector from the application of Add (lhs + rhs).
        /// </summary>
        public static dVector3 Add(double lhs, double rhs) => new dVector3(lhs + rhs);
        
        /// <summary>
        /// Returns a dVector3 from component-wise application of Sub (lhs - rhs).
        /// </summary>
        public static dVector3 Sub(dVector3 lhs, dVector3 rhs) => new dVector3(lhs.x - rhs.x, lhs.y - rhs.y, lhs.z - rhs.z);
        
        /// <summary>
        /// Returns a dVector3 from component-wise application of Sub (lhs - rhs).
        /// </summary>
        public static dVector3 Sub(dVector3 lhs, double rhs) => new dVector3(lhs.x - rhs, lhs.y - rhs, lhs.z - rhs);
        
        /// <summary>
        /// Returns a dVector3 from component-wise application of Sub (lhs - rhs).
        /// </summary>
        public static dVector3 Sub(double lhs, dVector3 rhs) => new dVector3(lhs - rhs.x, lhs - rhs.y, lhs - rhs.z);
        
        /// <summary>
        /// Returns a dVector from the application of Sub (lhs - rhs).
        /// </summary>
        public static dVector3 Sub(double lhs, double rhs) => new dVector3(lhs - rhs);
        
        /// <summary>
        /// Returns a dVector3 from component-wise application of Mul (lhs * rhs).
        /// </summary>
        public static dVector3 Mul(dVector3 lhs, dVector3 rhs) => new dVector3(lhs.x * rhs.x, lhs.y * rhs.y, lhs.z * rhs.z);
        
        /// <summary>
        /// Returns a dVector3 from component-wise application of Mul (lhs * rhs).
        /// </summary>
        public static dVector3 Mul(dVector3 lhs, double rhs) => new dVector3(lhs.x * rhs, lhs.y * rhs, lhs.z * rhs);
        
        /// <summary>
        /// Returns a dVector3 from component-wise application of Mul (lhs * rhs).
        /// </summary>
        public static dVector3 Mul(double lhs, dVector3 rhs) => new dVector3(lhs * rhs.x, lhs * rhs.y, lhs * rhs.z);
        
        /// <summary>
        /// Returns a dVector from the application of Mul (lhs * rhs).
        /// </summary>
        public static dVector3 Mul(double lhs, double rhs) => new dVector3(lhs * rhs);
        
        /// <summary>
        /// Returns a dVector3 from component-wise application of Div (lhs / rhs).
        /// </summary>
        public static dVector3 Div(dVector3 lhs, dVector3 rhs) => new dVector3(lhs.x / rhs.x, lhs.y / rhs.y, lhs.z / rhs.z);
        
        /// <summary>
        /// Returns a dVector3 from component-wise application of Div (lhs / rhs).
        /// </summary>
        public static dVector3 Div(dVector3 lhs, double rhs) => new dVector3(lhs.x / rhs, lhs.y / rhs, lhs.z / rhs);
        
        /// <summary>
        /// Returns a dVector3 from component-wise application of Div (lhs / rhs).
        /// </summary>
        public static dVector3 Div(double lhs, dVector3 rhs) => new dVector3(lhs / rhs.x, lhs / rhs.y, lhs / rhs.z);
        
        /// <summary>
        /// Returns a dVector from the application of Div (lhs / rhs).
        /// </summary>
        public static dVector3 Div(double lhs, double rhs) => new dVector3(lhs / rhs);
        
        /// <summary>
        /// Returns a dVector3 from component-wise application of Modulo (lhs % rhs).
        /// </summary>
        public static dVector3 Modulo(dVector3 lhs, dVector3 rhs) => new dVector3(lhs.x % rhs.x, lhs.y % rhs.y, lhs.z % rhs.z);
        
        /// <summary>
        /// Returns a dVector3 from component-wise application of Modulo (lhs % rhs).
        /// </summary>
        public static dVector3 Modulo(dVector3 lhs, double rhs) => new dVector3(lhs.x % rhs, lhs.y % rhs, lhs.z % rhs);
        
        /// <summary>
        /// Returns a dVector3 from component-wise application of Modulo (lhs % rhs).
        /// </summary>
        public static dVector3 Modulo(double lhs, dVector3 rhs) => new dVector3(lhs % rhs.x, lhs % rhs.y, lhs % rhs.z);
        
        /// <summary>
        /// Returns a dVector from the application of Modulo (lhs % rhs).
        /// </summary>
        public static dVector3 Modulo(double lhs, double rhs) => new dVector3(lhs % rhs);
        
        /// <summary>
        /// Returns a dVector3 from component-wise application of Degrees (Radians-To-Degrees Conversion).
        /// </summary>
        public static dVector3 Degrees(dVector3 v) => new dVector3((double)(v.x * 57.295779513082320876798154814105170332405472466564321d), (double)(v.y * 57.295779513082320876798154814105170332405472466564321d), (double)(v.z * 57.295779513082320876798154814105170332405472466564321d));
        
        /// <summary>
        /// Returns a dVector from the application of Degrees (Radians-To-Degrees Conversion).
        /// </summary>
        public static dVector3 Degrees(double v) => new dVector3((double)(v * 57.295779513082320876798154814105170332405472466564321d));
        
        /// <summary>
        /// Returns a dVector3 from component-wise application of Radians (Degrees-To-Radians Conversion).
        /// </summary>
        public static dVector3 Radians(dVector3 v) => new dVector3((double)(v.x * 0.0174532925199432957692369076848861271344287188854172d), (double)(v.y * 0.0174532925199432957692369076848861271344287188854172d), (double)(v.z * 0.0174532925199432957692369076848861271344287188854172d));
        
        /// <summary>
        /// Returns a dVector from the application of Radians (Degrees-To-Radians Conversion).
        /// </summary>
        public static dVector3 Radians(double v) => new dVector3((double)(v * 0.0174532925199432957692369076848861271344287188854172d));
        
        /// <summary>
        /// Returns a dVector3 from component-wise application of Acos ((double)Math.Acos((double)v)).
        /// </summary>
        public static dVector3 Acos(dVector3 v) => new dVector3((double)Math.Acos((double)v.x), (double)Math.Acos((double)v.y), (double)Math.Acos((double)v.z));
        
        /// <summary>
        /// Returns a dVector from the application of Acos ((double)Math.Acos((double)v)).
        /// </summary>
        public static dVector3 Acos(double v) => new dVector3((double)Math.Acos((double)v));
        
        /// <summary>
        /// Returns a dVector3 from component-wise application of Asin ((double)Math.Asin((double)v)).
        /// </summary>
        public static dVector3 Asin(dVector3 v) => new dVector3((double)Math.Asin((double)v.x), (double)Math.Asin((double)v.y), (double)Math.Asin((double)v.z));
        
        /// <summary>
        /// Returns a dVector from the application of Asin ((double)Math.Asin((double)v)).
        /// </summary>
        public static dVector3 Asin(double v) => new dVector3((double)Math.Asin((double)v));
        
        /// <summary>
        /// Returns a dVector3 from component-wise application of Atan ((double)Math.Atan((double)v)).
        /// </summary>
        public static dVector3 Atan(dVector3 v) => new dVector3((double)Math.Atan((double)v.x), (double)Math.Atan((double)v.y), (double)Math.Atan((double)v.z));
        
        /// <summary>
        /// Returns a dVector from the application of Atan ((double)Math.Atan((double)v)).
        /// </summary>
        public static dVector3 Atan(double v) => new dVector3((double)Math.Atan((double)v));
        
        /// <summary>
        /// Returns a dVector3 from component-wise application of Cos ((double)Math.Cos((double)v)).
        /// </summary>
        public static dVector3 Cos(dVector3 v) => new dVector3((double)Math.Cos((double)v.x), (double)Math.Cos((double)v.y), (double)Math.Cos((double)v.z));
        
        /// <summary>
        /// Returns a dVector from the application of Cos ((double)Math.Cos((double)v)).
        /// </summary>
        public static dVector3 Cos(double v) => new dVector3((double)Math.Cos((double)v));
        
        /// <summary>
        /// Returns a dVector3 from component-wise application of Cosh ((double)Math.Cosh((double)v)).
        /// </summary>
        public static dVector3 Cosh(dVector3 v) => new dVector3((double)Math.Cosh((double)v.x), (double)Math.Cosh((double)v.y), (double)Math.Cosh((double)v.z));
        
        /// <summary>
        /// Returns a dVector from the application of Cosh ((double)Math.Cosh((double)v)).
        /// </summary>
        public static dVector3 Cosh(double v) => new dVector3((double)Math.Cosh((double)v));
        
        /// <summary>
        /// Returns a dVector3 from component-wise application of Exp ((double)Math.Exp((double)v)).
        /// </summary>
        public static dVector3 Exp(dVector3 v) => new dVector3((double)Math.Exp((double)v.x), (double)Math.Exp((double)v.y), (double)Math.Exp((double)v.z));
        
        /// <summary>
        /// Returns a dVector from the application of Exp ((double)Math.Exp((double)v)).
        /// </summary>
        public static dVector3 Exp(double v) => new dVector3((double)Math.Exp((double)v));
        
        /// <summary>
        /// Returns a dVector3 from component-wise application of Log ((double)Math.Log((double)v)).
        /// </summary>
        public static dVector3 Log(dVector3 v) => new dVector3((double)Math.Log((double)v.x), (double)Math.Log((double)v.y), (double)Math.Log((double)v.z));
        
        /// <summary>
        /// Returns a dVector from the application of Log ((double)Math.Log((double)v)).
        /// </summary>
        public static dVector3 Log(double v) => new dVector3((double)Math.Log((double)v));
        
        /// <summary>
        /// Returns a dVector3 from component-wise application of Log2 ((double)Math.Log((double)v, 2)).
        /// </summary>
        public static dVector3 Log2(dVector3 v) => new dVector3((double)Math.Log((double)v.x, 2), (double)Math.Log((double)v.y, 2), (double)Math.Log((double)v.z, 2));
        
        /// <summary>
        /// Returns a dVector from the application of Log2 ((double)Math.Log((double)v, 2)).
        /// </summary>
        public static dVector3 Log2(double v) => new dVector3((double)Math.Log((double)v, 2));
        
        /// <summary>
        /// Returns a dVector3 from component-wise application of Log10 ((double)Math.Log10((double)v)).
        /// </summary>
        public static dVector3 Log10(dVector3 v) => new dVector3((double)Math.Log10((double)v.x), (double)Math.Log10((double)v.y), (double)Math.Log10((double)v.z));
        
        /// <summary>
        /// Returns a dVector from the application of Log10 ((double)Math.Log10((double)v)).
        /// </summary>
        public static dVector3 Log10(double v) => new dVector3((double)Math.Log10((double)v));
        
        /// <summary>
        /// Returns a dVector3 from component-wise application of Floor ((double)Math.Floor(v)).
        /// </summary>
        public static dVector3 Floor(dVector3 v) => new dVector3((double)Math.Floor(v.x), (double)Math.Floor(v.y), (double)Math.Floor(v.z));
        
        /// <summary>
        /// Returns a dVector from the application of Floor ((double)Math.Floor(v)).
        /// </summary>
        public static dVector3 Floor(double v) => new dVector3((double)Math.Floor(v));
        
        /// <summary>
        /// Returns a dVector3 from component-wise application of Ceiling ((double)Math.Ceiling(v)).
        /// </summary>
        public static dVector3 Ceiling(dVector3 v) => new dVector3((double)Math.Ceiling(v.x), (double)Math.Ceiling(v.y), (double)Math.Ceiling(v.z));
        
        /// <summary>
        /// Returns a dVector from the application of Ceiling ((double)Math.Ceiling(v)).
        /// </summary>
        public static dVector3 Ceiling(double v) => new dVector3((double)Math.Ceiling(v));
        
        /// <summary>
        /// Returns a dVector3 from component-wise application of Round ((double)Math.Round(v)).
        /// </summary>
        public static dVector3 Round(dVector3 v) => new dVector3((double)Math.Round(v.x), (double)Math.Round(v.y), (double)Math.Round(v.z));
        
        /// <summary>
        /// Returns a dVector from the application of Round ((double)Math.Round(v)).
        /// </summary>
        public static dVector3 Round(double v) => new dVector3((double)Math.Round(v));
        
        /// <summary>
        /// Returns a dVector3 from component-wise application of Sin ((double)Math.Sin((double)v)).
        /// </summary>
        public static dVector3 Sin(dVector3 v) => new dVector3((double)Math.Sin((double)v.x), (double)Math.Sin((double)v.y), (double)Math.Sin((double)v.z));
        
        /// <summary>
        /// Returns a dVector from the application of Sin ((double)Math.Sin((double)v)).
        /// </summary>
        public static dVector3 Sin(double v) => new dVector3((double)Math.Sin((double)v));
        
        /// <summary>
        /// Returns a dVector3 from component-wise application of Sinh ((double)Math.Sinh((double)v)).
        /// </summary>
        public static dVector3 Sinh(dVector3 v) => new dVector3((double)Math.Sinh((double)v.x), (double)Math.Sinh((double)v.y), (double)Math.Sinh((double)v.z));
        
        /// <summary>
        /// Returns a dVector from the application of Sinh ((double)Math.Sinh((double)v)).
        /// </summary>
        public static dVector3 Sinh(double v) => new dVector3((double)Math.Sinh((double)v));
        
        /// <summary>
        /// Returns a dVector3 from component-wise application of Tan ((double)Math.Tan((double)v)).
        /// </summary>
        public static dVector3 Tan(dVector3 v) => new dVector3((double)Math.Tan((double)v.x), (double)Math.Tan((double)v.y), (double)Math.Tan((double)v.z));
        
        /// <summary>
        /// Returns a dVector from the application of Tan ((double)Math.Tan((double)v)).
        /// </summary>
        public static dVector3 Tan(double v) => new dVector3((double)Math.Tan((double)v));
        
        /// <summary>
        /// Returns a dVector3 from component-wise application of Tanh ((double)Math.Tanh((double)v)).
        /// </summary>
        public static dVector3 Tanh(dVector3 v) => new dVector3((double)Math.Tanh((double)v.x), (double)Math.Tanh((double)v.y), (double)Math.Tanh((double)v.z));
        
        /// <summary>
        /// Returns a dVector from the application of Tanh ((double)Math.Tanh((double)v)).
        /// </summary>
        public static dVector3 Tanh(double v) => new dVector3((double)Math.Tanh((double)v));
        
        /// <summary>
        /// Returns a dVector3 from component-wise application of Truncate ((double)Math.Truncate((double)v)).
        /// </summary>
        public static dVector3 Truncate(dVector3 v) => new dVector3((double)Math.Truncate((double)v.x), (double)Math.Truncate((double)v.y), (double)Math.Truncate((double)v.z));
        
        /// <summary>
        /// Returns a dVector from the application of Truncate ((double)Math.Truncate((double)v)).
        /// </summary>
        public static dVector3 Truncate(double v) => new dVector3((double)Math.Truncate((double)v));
        
        /// <summary>
        /// Returns a dVector3 from component-wise application of Fract ((double)(v - Math.Floor(v))).
        /// </summary>
        public static dVector3 Fract(dVector3 v) => new dVector3((double)(v.x - Math.Floor(v.x)), (double)(v.y - Math.Floor(v.y)), (double)(v.z - Math.Floor(v.z)));
        
        /// <summary>
        /// Returns a dVector from the application of Fract ((double)(v - Math.Floor(v))).
        /// </summary>
        public static dVector3 Fract(double v) => new dVector3((double)(v - Math.Floor(v)));
        
        /// <summary>
        /// Returns a dVector3 from component-wise application of Trunc ((long)(v)).
        /// </summary>
        public static dVector3 Trunc(dVector3 v) => new dVector3((long)(v.x), (long)(v.y), (long)(v.z));
        
        /// <summary>
        /// Returns a dVector from the application of Trunc ((long)(v)).
        /// </summary>
        public static dVector3 Trunc(double v) => new dVector3((long)(v));
        
        /// <summary>
        /// Returns a dVector3 with independent and identically distributed uniform values between 'minValue' and 'maxValue'.
        /// </summary>
        public static dVector3 Random(System.Random random, dVector3 minValue, dVector3 maxValue) => new dVector3((double)random.NextDouble() * (maxValue.x - minValue.x) + minValue.x, (double)random.NextDouble() * (maxValue.y - minValue.y) + minValue.y, (double)random.NextDouble() * (maxValue.z - minValue.z) + minValue.z);
        
        /// <summary>
        /// Returns a dVector3 with independent and identically distributed uniform values between 'minValue' and 'maxValue'.
        /// </summary>
        public static dVector3 Random(System.Random random, dVector3 minValue, double maxValue) => new dVector3((double)random.NextDouble() * (maxValue - minValue.x) + minValue.x, (double)random.NextDouble() * (maxValue - minValue.y) + minValue.y, (double)random.NextDouble() * (maxValue - minValue.z) + minValue.z);
        
        /// <summary>
        /// Returns a dVector3 with independent and identically distributed uniform values between 'minValue' and 'maxValue'.
        /// </summary>
        public static dVector3 Random(System.Random random, double minValue, dVector3 maxValue) => new dVector3((double)random.NextDouble() * (maxValue.x - minValue) + minValue, (double)random.NextDouble() * (maxValue.y - minValue) + minValue, (double)random.NextDouble() * (maxValue.z - minValue) + minValue);
        
        /// <summary>
        /// Returns a dVector3 with independent and identically distributed uniform values between 'minValue' and 'maxValue'.
        /// </summary>
        public static dVector3 Random(System.Random random, double minValue, double maxValue) => new dVector3((double)random.NextDouble() * (maxValue - minValue) + minValue);
        
        /// <summary>
        /// Returns a dVector3 with independent and identically distributed uniform values between 'minValue' and 'maxValue'.
        /// </summary>
        public static dVector3 RandomUniform(System.Random random, dVector3 minValue, dVector3 maxValue) => new dVector3((double)random.NextDouble() * (maxValue.x - minValue.x) + minValue.x, (double)random.NextDouble() * (maxValue.y - minValue.y) + minValue.y, (double)random.NextDouble() * (maxValue.z - minValue.z) + minValue.z);
        
        /// <summary>
        /// Returns a dVector3 with independent and identically distributed uniform values between 'minValue' and 'maxValue'.
        /// </summary>
        public static dVector3 RandomUniform(System.Random random, dVector3 minValue, double maxValue) => new dVector3((double)random.NextDouble() * (maxValue - minValue.x) + minValue.x, (double)random.NextDouble() * (maxValue - minValue.y) + minValue.y, (double)random.NextDouble() * (maxValue - minValue.z) + minValue.z);
        
        /// <summary>
        /// Returns a dVector3 with independent and identically distributed uniform values between 'minValue' and 'maxValue'.
        /// </summary>
        public static dVector3 RandomUniform(System.Random random, double minValue, dVector3 maxValue) => new dVector3((double)random.NextDouble() * (maxValue.x - minValue) + minValue, (double)random.NextDouble() * (maxValue.y - minValue) + minValue, (double)random.NextDouble() * (maxValue.z - minValue) + minValue);
        
        /// <summary>
        /// Returns a dVector3 with independent and identically distributed uniform values between 'minValue' and 'maxValue'.
        /// </summary>
        public static dVector3 RandomUniform(System.Random random, double minValue, double maxValue) => new dVector3((double)random.NextDouble() * (maxValue - minValue) + minValue);
        
        /// <summary>
        /// Returns a dVector3 with independent and identically distributed values according to a normal/Gaussian distribution with specified mean and variance.
        /// </summary>
        public static dVector3 RandomNormal(System.Random random, dVector3 mean, dVector3 variance) => new dVector3((double)(Math.Sqrt((double)variance.x) * Math.Cos(2 * Math.PI * random.NextDouble()) * Math.Sqrt(-2.0 * Math.Log(random.NextDouble()))) + mean.x, (double)(Math.Sqrt((double)variance.y) * Math.Cos(2 * Math.PI * random.NextDouble()) * Math.Sqrt(-2.0 * Math.Log(random.NextDouble()))) + mean.y, (double)(Math.Sqrt((double)variance.z) * Math.Cos(2 * Math.PI * random.NextDouble()) * Math.Sqrt(-2.0 * Math.Log(random.NextDouble()))) + mean.z);
        
        /// <summary>
        /// Returns a dVector3 with independent and identically distributed values according to a normal/Gaussian distribution with specified mean and variance.
        /// </summary>
        public static dVector3 RandomNormal(System.Random random, dVector3 mean, double variance) => new dVector3((double)(Math.Sqrt((double)variance) * Math.Cos(2 * Math.PI * random.NextDouble()) * Math.Sqrt(-2.0 * Math.Log(random.NextDouble()))) + mean.x, (double)(Math.Sqrt((double)variance) * Math.Cos(2 * Math.PI * random.NextDouble()) * Math.Sqrt(-2.0 * Math.Log(random.NextDouble()))) + mean.y, (double)(Math.Sqrt((double)variance) * Math.Cos(2 * Math.PI * random.NextDouble()) * Math.Sqrt(-2.0 * Math.Log(random.NextDouble()))) + mean.z);
        
        /// <summary>
        /// Returns a dVector3 with independent and identically distributed values according to a normal/Gaussian distribution with specified mean and variance.
        /// </summary>
        public static dVector3 RandomNormal(System.Random random, double mean, dVector3 variance) => new dVector3((double)(Math.Sqrt((double)variance.x) * Math.Cos(2 * Math.PI * random.NextDouble()) * Math.Sqrt(-2.0 * Math.Log(random.NextDouble()))) + mean, (double)(Math.Sqrt((double)variance.y) * Math.Cos(2 * Math.PI * random.NextDouble()) * Math.Sqrt(-2.0 * Math.Log(random.NextDouble()))) + mean, (double)(Math.Sqrt((double)variance.z) * Math.Cos(2 * Math.PI * random.NextDouble()) * Math.Sqrt(-2.0 * Math.Log(random.NextDouble()))) + mean);
        
        /// <summary>
        /// Returns a dVector3 with independent and identically distributed values according to a normal/Gaussian distribution with specified mean and variance.
        /// </summary>
        public static dVector3 RandomNormal(System.Random random, double mean, double variance) => new dVector3((double)(Math.Sqrt((double)variance) * Math.Cos(2 * Math.PI * random.NextDouble()) * Math.Sqrt(-2.0 * Math.Log(random.NextDouble()))) + mean);
        
        /// <summary>
        /// Returns a dVector3 with independent and identically distributed values according to a normal/Gaussian distribution with specified mean and variance.
        /// </summary>
        public static dVector3 RandomGaussian(System.Random random, dVector3 mean, dVector3 variance) => new dVector3((double)(Math.Sqrt((double)variance.x) * Math.Cos(2 * Math.PI * random.NextDouble()) * Math.Sqrt(-2.0 * Math.Log(random.NextDouble()))) + mean.x, (double)(Math.Sqrt((double)variance.y) * Math.Cos(2 * Math.PI * random.NextDouble()) * Math.Sqrt(-2.0 * Math.Log(random.NextDouble()))) + mean.y, (double)(Math.Sqrt((double)variance.z) * Math.Cos(2 * Math.PI * random.NextDouble()) * Math.Sqrt(-2.0 * Math.Log(random.NextDouble()))) + mean.z);
        
        /// <summary>
        /// Returns a dVector3 with independent and identically distributed values according to a normal/Gaussian distribution with specified mean and variance.
        /// </summary>
        public static dVector3 RandomGaussian(System.Random random, dVector3 mean, double variance) => new dVector3((double)(Math.Sqrt((double)variance) * Math.Cos(2 * Math.PI * random.NextDouble()) * Math.Sqrt(-2.0 * Math.Log(random.NextDouble()))) + mean.x, (double)(Math.Sqrt((double)variance) * Math.Cos(2 * Math.PI * random.NextDouble()) * Math.Sqrt(-2.0 * Math.Log(random.NextDouble()))) + mean.y, (double)(Math.Sqrt((double)variance) * Math.Cos(2 * Math.PI * random.NextDouble()) * Math.Sqrt(-2.0 * Math.Log(random.NextDouble()))) + mean.z);
        
        /// <summary>
        /// Returns a dVector3 with independent and identically distributed values according to a normal/Gaussian distribution with specified mean and variance.
        /// </summary>
        public static dVector3 RandomGaussian(System.Random random, double mean, dVector3 variance) => new dVector3((double)(Math.Sqrt((double)variance.x) * Math.Cos(2 * Math.PI * random.NextDouble()) * Math.Sqrt(-2.0 * Math.Log(random.NextDouble()))) + mean, (double)(Math.Sqrt((double)variance.y) * Math.Cos(2 * Math.PI * random.NextDouble()) * Math.Sqrt(-2.0 * Math.Log(random.NextDouble()))) + mean, (double)(Math.Sqrt((double)variance.z) * Math.Cos(2 * Math.PI * random.NextDouble()) * Math.Sqrt(-2.0 * Math.Log(random.NextDouble()))) + mean);
        
        /// <summary>
        /// Returns a dVector3 with independent and identically distributed values according to a normal/Gaussian distribution with specified mean and variance.
        /// </summary>
        public static dVector3 RandomGaussian(System.Random random, double mean, double variance) => new dVector3((double)(Math.Sqrt((double)variance) * Math.Cos(2 * Math.PI * random.NextDouble()) * Math.Sqrt(-2.0 * Math.Log(random.NextDouble()))) + mean);

        #endregion


        #region Component-Wise Operator Overloads
        
        /// <summary>
        /// Returns a bVector3 from component-wise application of operator&lt; (lhs &lt; rhs).
        /// </summary>
        public static bVector3 operator<(dVector3 lhs, dVector3 rhs) => new bVector3(lhs.x < rhs.x, lhs.y < rhs.y, lhs.z < rhs.z);
        
        /// <summary>
        /// Returns a bVector3 from component-wise application of operator&lt; (lhs &lt; rhs).
        /// </summary>
        public static bVector3 operator<(dVector3 lhs, double rhs) => new bVector3(lhs.x < rhs, lhs.y < rhs, lhs.z < rhs);
        
        /// <summary>
        /// Returns a bVector3 from component-wise application of operator&lt; (lhs &lt; rhs).
        /// </summary>
        public static bVector3 operator<(double lhs, dVector3 rhs) => new bVector3(lhs < rhs.x, lhs < rhs.y, lhs < rhs.z);
        
        /// <summary>
        /// Returns a bVector3 from component-wise application of operator&lt;= (lhs &lt;= rhs).
        /// </summary>
        public static bVector3 operator<=(dVector3 lhs, dVector3 rhs) => new bVector3(lhs.x <= rhs.x, lhs.y <= rhs.y, lhs.z <= rhs.z);
        
        /// <summary>
        /// Returns a bVector3 from component-wise application of operator&lt;= (lhs &lt;= rhs).
        /// </summary>
        public static bVector3 operator<=(dVector3 lhs, double rhs) => new bVector3(lhs.x <= rhs, lhs.y <= rhs, lhs.z <= rhs);
        
        /// <summary>
        /// Returns a bVector3 from component-wise application of operator&lt;= (lhs &lt;= rhs).
        /// </summary>
        public static bVector3 operator<=(double lhs, dVector3 rhs) => new bVector3(lhs <= rhs.x, lhs <= rhs.y, lhs <= rhs.z);
        
        /// <summary>
        /// Returns a bVector3 from component-wise application of operator&gt; (lhs &gt; rhs).
        /// </summary>
        public static bVector3 operator>(dVector3 lhs, dVector3 rhs) => new bVector3(lhs.x > rhs.x, lhs.y > rhs.y, lhs.z > rhs.z);
        
        /// <summary>
        /// Returns a bVector3 from component-wise application of operator&gt; (lhs &gt; rhs).
        /// </summary>
        public static bVector3 operator>(dVector3 lhs, double rhs) => new bVector3(lhs.x > rhs, lhs.y > rhs, lhs.z > rhs);
        
        /// <summary>
        /// Returns a bVector3 from component-wise application of operator&gt; (lhs &gt; rhs).
        /// </summary>
        public static bVector3 operator>(double lhs, dVector3 rhs) => new bVector3(lhs > rhs.x, lhs > rhs.y, lhs > rhs.z);
        
        /// <summary>
        /// Returns a bVector3 from component-wise application of operator&gt;= (lhs &gt;= rhs).
        /// </summary>
        public static bVector3 operator>=(dVector3 lhs, dVector3 rhs) => new bVector3(lhs.x >= rhs.x, lhs.y >= rhs.y, lhs.z >= rhs.z);
        
        /// <summary>
        /// Returns a bVector3 from component-wise application of operator&gt;= (lhs &gt;= rhs).
        /// </summary>
        public static bVector3 operator>=(dVector3 lhs, double rhs) => new bVector3(lhs.x >= rhs, lhs.y >= rhs, lhs.z >= rhs);
        
        /// <summary>
        /// Returns a bVector3 from component-wise application of operator&gt;= (lhs &gt;= rhs).
        /// </summary>
        public static bVector3 operator>=(double lhs, dVector3 rhs) => new bVector3(lhs >= rhs.x, lhs >= rhs.y, lhs >= rhs.z);
        
        /// <summary>
        /// Returns a dVector3 from component-wise application of operator+ (lhs + rhs).
        /// </summary>
        public static dVector3 operator+(dVector3 lhs, dVector3 rhs) => new dVector3(lhs.x + rhs.x, lhs.y + rhs.y, lhs.z + rhs.z);
        
        /// <summary>
        /// Returns a dVector3 from component-wise application of operator+ (lhs + rhs).
        /// </summary>
        public static dVector3 operator+(dVector3 lhs, double rhs) => new dVector3(lhs.x + rhs, lhs.y + rhs, lhs.z + rhs);
        
        /// <summary>
        /// Returns a dVector3 from component-wise application of operator+ (lhs + rhs).
        /// </summary>
        public static dVector3 operator+(double lhs, dVector3 rhs) => new dVector3(lhs + rhs.x, lhs + rhs.y, lhs + rhs.z);
        
        /// <summary>
        /// Returns a dVector3 from component-wise application of operator- (lhs - rhs).
        /// </summary>
        public static dVector3 operator-(dVector3 lhs, dVector3 rhs) => new dVector3(lhs.x - rhs.x, lhs.y - rhs.y, lhs.z - rhs.z);
        
        /// <summary>
        /// Returns a dVector3 from component-wise application of operator- (lhs - rhs).
        /// </summary>
        public static dVector3 operator-(dVector3 lhs, double rhs) => new dVector3(lhs.x - rhs, lhs.y - rhs, lhs.z - rhs);
        
        /// <summary>
        /// Returns a dVector3 from component-wise application of operator- (lhs - rhs).
        /// </summary>
        public static dVector3 operator-(double lhs, dVector3 rhs) => new dVector3(lhs - rhs.x, lhs - rhs.y, lhs - rhs.z);
        
        /// <summary>
        /// Returns a dVector3 from component-wise application of operator* (lhs * rhs).
        /// </summary>
        public static dVector3 operator*(dVector3 lhs, dVector3 rhs) => new dVector3(lhs.x * rhs.x, lhs.y * rhs.y, lhs.z * rhs.z);
        
        /// <summary>
        /// Returns a dVector3 from component-wise application of operator* (lhs * rhs).
        /// </summary>
        public static dVector3 operator*(dVector3 lhs, double rhs) => new dVector3(lhs.x * rhs, lhs.y * rhs, lhs.z * rhs);
        
        /// <summary>
        /// Returns a dVector3 from component-wise application of operator* (lhs * rhs).
        /// </summary>
        public static dVector3 operator*(double lhs, dVector3 rhs) => new dVector3(lhs * rhs.x, lhs * rhs.y, lhs * rhs.z);
        
        /// <summary>
        /// Returns a dVector3 from component-wise application of operator/ (lhs / rhs).
        /// </summary>
        public static dVector3 operator/(dVector3 lhs, dVector3 rhs) => new dVector3(lhs.x / rhs.x, lhs.y / rhs.y, lhs.z / rhs.z);
        
        /// <summary>
        /// Returns a dVector3 from component-wise application of operator/ (lhs / rhs).
        /// </summary>
        public static dVector3 operator/(dVector3 lhs, double rhs) => new dVector3(lhs.x / rhs, lhs.y / rhs, lhs.z / rhs);
        
        /// <summary>
        /// Returns a dVector3 from component-wise application of operator/ (lhs / rhs).
        /// </summary>
        public static dVector3 operator/(double lhs, dVector3 rhs) => new dVector3(lhs / rhs.x, lhs / rhs.y, lhs / rhs.z);
        
        /// <summary>
        /// Returns a dVector3 from component-wise application of operator+ (identity).
        /// </summary>
        public static dVector3 operator+(dVector3 v) => v;
        
        /// <summary>
        /// Returns a dVector3 from component-wise application of operator- (-v).
        /// </summary>
        public static dVector3 operator-(dVector3 v) => new dVector3(-v.x, -v.y, -v.z);
        
        /// <summary>
        /// Returns a dVector3 from component-wise application of operator% (lhs % rhs).
        /// </summary>
        public static dVector3 operator%(dVector3 lhs, dVector3 rhs) => new dVector3(lhs.x % rhs.x, lhs.y % rhs.y, lhs.z % rhs.z);
        
        /// <summary>
        /// Returns a dVector3 from component-wise application of operator% (lhs % rhs).
        /// </summary>
        public static dVector3 operator%(dVector3 lhs, double rhs) => new dVector3(lhs.x % rhs, lhs.y % rhs, lhs.z % rhs);
        
        /// <summary>
        /// Returns a dVector3 from component-wise application of operator% (lhs % rhs).
        /// </summary>
        public static dVector3 operator%(double lhs, dVector3 rhs) => new dVector3(lhs % rhs.x, lhs % rhs.y, lhs % rhs.z);

        #endregion

    }
}
