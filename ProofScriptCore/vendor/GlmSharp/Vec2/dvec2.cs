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
    /// A Vectortor of type double with 2 components.
    /// </summary>
    [RegisterCoreClassStruct]
    
    [StructLayout(LayoutKind.Sequential)]
    public struct dVector2 : IReadOnlyList<double>, IEquatable<dVector2>
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

        #endregion


        #region Constructors
        
        /// <summary>
        /// Component-wise constructor
        /// </summary>
        public dVector2(double x, double y)
        {
            this.x = x;
            this.y = y;
        }
        
        /// <summary>
        /// all-same-value constructor
        /// </summary>
        public dVector2(double v)
        {
            this.x = v;
            this.y = v;
        }
        
        /// <summary>
        /// from-Vectortor constructor
        /// </summary>
        public dVector2(dVector2 v)
        {
            this.x = v.x;
            this.y = v.y;
        }
        
        /// <summary>
        /// from-Vectortor constructor (additional fields are truncated)
        /// </summary>
        public dVector2(dVector3 v)
        {
            this.x = v.x;
            this.y = v.y;
        }
        
        /// <summary>
        /// from-Vectortor constructor (additional fields are truncated)
        /// </summary>
        public dVector2(dVector4 v)
        {
            this.x = v.x;
            this.y = v.y;
        }
        
        /// <summary>
        /// From-array/list constructor (superfluous values are ignored, missing values are zero-filled).
        /// </summary>
        public dVector2(IReadOnlyList<double> v)
        {
            var c = v.Count;
            this.x = c < 0 ? 0.0 : v[0];
            this.y = c < 1 ? 0.0 : v[1];
        }
        
        /// <summary>
        /// Generic from-array constructor (superfluous values are ignored, missing values are zero-filled).
        /// </summary>
        public dVector2(Object[] v)
        {
            var c = v.Length;
            this.x = c < 0 ? 0.0 : (double)v[0];
            this.y = c < 1 ? 0.0 : (double)v[1];
        }
        
        /// <summary>
        /// From-array constructor (superfluous values are ignored, missing values are zero-filled).
        /// </summary>
        public dVector2(double[] v)
        {
            var c = v.Length;
            this.x = c < 0 ? 0.0 : v[0];
            this.y = c < 1 ? 0.0 : v[1];
        }
        
        /// <summary>
        /// From-array constructor with base index (superfluous values are ignored, missing values are zero-filled).
        /// </summary>
        public dVector2(double[] v, int startIndex)
        {
            var c = v.Length;
            this.x = c + startIndex < 0 ? 0.0 : v[0 + startIndex];
            this.y = c + startIndex < 1 ? 0.0 : v[1 + startIndex];
        }
        
        /// <summary>
        /// From-IEnumerable constructor (superfluous values are ignored, missing values are zero-filled).
        /// </summary>
        public dVector2(IEnumerable<double> v)
            : this(v.ToArray())
        {
        }

        #endregion


        #region Implicit Operators
        
        /// <summary>
        /// Implicitly converts this to a cVector2.
        /// </summary>
        //public static implicit operator cVector2(dVector2 v) => new cVector2((Complex)v.x, (Complex)v.y);

        #endregion


        #region Explicit Operators
        
        /// <summary>
        /// Explicitly converts this to a iVector2.
        /// </summary>
        public static explicit operator iVector2(dVector2 v) => new iVector2((int)v.x, (int)v.y);
        
        /// <summary>
        /// Explicitly converts this to a iVector3. (Higher components are zeroed)
        /// </summary>
        public static explicit operator iVector3(dVector2 v) => new iVector3((int)v.x, (int)v.y, 0);
        
        /// <summary>
        /// Explicitly converts this to a iVector4. (Higher components are zeroed)
        /// </summary>
        public static explicit operator iVector4(dVector2 v) => new iVector4((int)v.x, (int)v.y, 0, 0);
        
        /// <summary>
        /// Explicitly converts this to a uVector2.
        /// </summary>
        public static explicit operator uVector2(dVector2 v) => new uVector2((uint)v.x, (uint)v.y);
        
        /// <summary>
        /// Explicitly converts this to a uVector3. (Higher components are zeroed)
        /// </summary>
        public static explicit operator uVector3(dVector2 v) => new uVector3((uint)v.x, (uint)v.y, 0u);
        
        /// <summary>
        /// Explicitly converts this to a uVector4. (Higher components are zeroed)
        /// </summary>
        public static explicit operator uVector4(dVector2 v) => new uVector4((uint)v.x, (uint)v.y, 0u, 0u);
        
        /// <summary>
        /// Explicitly converts this to a Vector2.
        /// </summary>
        public static explicit operator Vector2(dVector2 v) => new Vector2((float)v.x, (float)v.y);
        
        /// <summary>
        /// Explicitly converts this to a Vector3. (Higher components are zeroed)
        /// </summary>
        public static explicit operator Vector3(dVector2 v) => new Vector3((float)v.x, (float)v.y, 0f);
        
        /// <summary>
        /// Explicitly converts this to a Vector4. (Higher components are zeroed)
        /// </summary>
        public static explicit operator Vector4(dVector2 v) => new Vector4((float)v.x, (float)v.y, 0f, 0f);
        
        /// <summary>
        /// Explicitly converts this to a hVector2.
        /// </summary>
        public static explicit operator hVector2(dVector2 v) => new hVector2((Half)v.x, (Half)v.y);
        
        /// <summary>
        /// Explicitly converts this to a hVector3. (Higher components are zeroed)
        /// </summary>
        public static explicit operator hVector3(dVector2 v) => new hVector3((Half)v.x, (Half)v.y, Half.Zero);
        
        /// <summary>
        /// Explicitly converts this to a hVector4. (Higher components are zeroed)
        /// </summary>
        public static explicit operator hVector4(dVector2 v) => new hVector4((Half)v.x, (Half)v.y, Half.Zero, Half.Zero);
        
        /// <summary>
        /// Explicitly converts this to a dVector3. (Higher components are zeroed)
        /// </summary>
        public static explicit operator dVector3(dVector2 v) => new dVector3((double)v.x, (double)v.y, 0.0);
        
        /// <summary>
        /// Explicitly converts this to a dVector4. (Higher components are zeroed)
        /// </summary>
        public static explicit operator dVector4(dVector2 v) => new dVector4((double)v.x, (double)v.y, 0.0, 0.0);
        
        /// <summary>
        /// Explicitly converts this to a decVector2.
        /// </summary>
        public static explicit operator decVector2(dVector2 v) => new decVector2((decimal)v.x, (decimal)v.y);
        
        /// <summary>
        /// Explicitly converts this to a decVector3. (Higher components are zeroed)
        /// </summary>
        public static explicit operator decVector3(dVector2 v) => new decVector3((decimal)v.x, (decimal)v.y, 0m);
        
        /// <summary>
        /// Explicitly converts this to a decVector4. (Higher components are zeroed)
        /// </summary>
        public static explicit operator decVector4(dVector2 v) => new decVector4((decimal)v.x, (decimal)v.y, 0m, 0m);
        
        /// <summary>
        /// Explicitly converts this to a cVector3. (Higher components are zeroed)
        /// </summary>
        //public static explicit operator cVector3(dVector2 v) => new cVector3((Complex)v.x, (Complex)v.y, Complex.Zero);
        
        /// <summary>
        /// Explicitly converts this to a cVector4. (Higher components are zeroed)
        /// </summary>
        //public static explicit operator cVector4(dVector2 v) => new cVector4((Complex)v.x, (Complex)v.y, Complex.Zero, Complex.Zero);
        
        /// <summary>
        /// Explicitly converts this to a lVector2.
        /// </summary>
        public static explicit operator lVector2(dVector2 v) => new lVector2((long)v.x, (long)v.y);
        
        /// <summary>
        /// Explicitly converts this to a lVector3. (Higher components are zeroed)
        /// </summary>
        public static explicit operator lVector3(dVector2 v) => new lVector3((long)v.x, (long)v.y, 0);
        
        /// <summary>
        /// Explicitly converts this to a lVector4. (Higher components are zeroed)
        /// </summary>
        public static explicit operator lVector4(dVector2 v) => new lVector4((long)v.x, (long)v.y, 0, 0);
        
        /// <summary>
        /// Explicitly converts this to a bVector2.
        /// </summary>
        public static explicit operator bVector2(dVector2 v) => new bVector2(v.x != 0.0, v.y != 0.0);
        
        /// <summary>
        /// Explicitly converts this to a bVector3. (Higher components are zeroed)
        /// </summary>
        public static explicit operator bVector3(dVector2 v) => new bVector3(v.x != 0.0, v.y != 0.0, false);
        
        /// <summary>
        /// Explicitly converts this to a bVector4. (Higher components are zeroed)
        /// </summary>
        public static explicit operator bVector4(dVector2 v) => new bVector4(v.x != 0.0, v.y != 0.0, false, false);
        
        /// <summary>
        /// Explicitly converts this to a double array.
        /// </summary>
        public static explicit operator double[](dVector2 v) => new [] { v.x, v.y };
        
        /// <summary>
        /// Explicitly converts this to a generic object array.
        /// </summary>
        public static explicit operator Object[](dVector2 v) => new Object[] { v.x, v.y };

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
        public swizzle_dVector2 swizzle => new swizzle_dVector2(x, y);
        
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
        /// Returns an array with all values
        /// </summary>
        public double[] Values => new[] { x, y };
        
        /// <summary>
        /// Returns the number of components (2).
        /// </summary>
        public int Count => 2;
        
        /// <summary>
        /// Returns the minimal component of this Vectortor.
        /// </summary>
        public double MinElement => Math.Min(x, y);
        
        /// <summary>
        /// Returns the maximal component of this Vectortor.
        /// </summary>
        public double MaxElement => Math.Max(x, y);
        
        /// <summary>
        /// Returns the euclidean length of this Vectortor.
        /// </summary>
        public double Length => (double)Math.Sqrt((x*x + y*y));
        
        /// <summary>
        /// Returns the squared euclidean length of this Vectortor.
        /// </summary>
        public double LengthSqr => (x*x + y*y);
        
        /// <summary>
        /// Returns the sum of all components.
        /// </summary>
        public double Sum => (x + y);
        
        /// <summary>
        /// Returns the euclidean norm of this Vectortor.
        /// </summary>
        public double Norm => (double)Math.Sqrt((x*x + y*y));
        
        /// <summary>
        /// Returns the one-norm of this Vectortor.
        /// </summary>
        public double Norm1 => (Math.Abs(x) + Math.Abs(y));
        
        /// <summary>
        /// Returns the two-norm (euclidean length) of this Vectortor.
        /// </summary>
        public double Norm2 => (double)Math.Sqrt((x*x + y*y));
        
        /// <summary>
        /// Returns the max-norm of this Vectortor.
        /// </summary>
        public double NormMax => Math.Max(Math.Abs(x), Math.Abs(y));
        
        /// <summary>
        /// Returns a copy of this Vectortor with length one (undefined if this has zero length).
        /// </summary>
        public dVector2 Normalized => this / (double)Length;
        
        /// <summary>
        /// Returns a copy of this Vectortor with length one (returns zero if length is zero).
        /// </summary>
        public dVector2 NormalizedSafe => this == Zero ? Zero : this / (double)Length;
        
        /// <summary>
        /// Returns the Vectortor angle (atan2(y, x)) in radians.
        /// </summary>
        public double Angle => Math.Atan2((double)y, (double)x);

        #endregion


        #region Static Properties
        
        /// <summary>
        /// Predefined all-zero Vectortor
        /// </summary>
        public static dVector2 Zero { get; } = new dVector2(0.0, 0.0);
        
        /// <summary>
        /// Predefined all-ones Vectortor
        /// </summary>
        public static dVector2 Ones { get; } = new dVector2(1.0, 1.0);
        
        /// <summary>
        /// Predefined unit-X Vectortor
        /// </summary>
        public static dVector2 UnitX { get; } = new dVector2(1.0, 0.0);
        
        /// <summary>
        /// Predefined unit-Y Vectortor
        /// </summary>
        public static dVector2 UnitY { get; } = new dVector2(0.0, 1.0);
        
        /// <summary>
        /// Predefined all-MaxValue Vectortor
        /// </summary>
        public static dVector2 MaxValue { get; } = new dVector2(double.MaxValue, double.MaxValue);
        
        /// <summary>
        /// Predefined all-MinValue Vectortor
        /// </summary>
        public static dVector2 MinValue { get; } = new dVector2(double.MinValue, double.MinValue);
        
        /// <summary>
        /// Predefined all-Epsilon Vectortor
        /// </summary>
        public static dVector2 Epsilon { get; } = new dVector2(double.Epsilon, double.Epsilon);
        
        /// <summary>
        /// Predefined all-NaN Vectortor
        /// </summary>
        public static dVector2 NaN { get; } = new dVector2(double.NaN, double.NaN);
        
        /// <summary>
        /// Predefined all-NegativeInfinity Vectortor
        /// </summary>
        public static dVector2 NegativeInfinity { get; } = new dVector2(double.NegativeInfinity, double.NegativeInfinity);
        
        /// <summary>
        /// Predefined all-PositiveInfinity Vectortor
        /// </summary>
        public static dVector2 PositiveInfinity { get; } = new dVector2(double.PositiveInfinity, double.PositiveInfinity);

        #endregion


        #region Operators
        
        /// <summary>
        /// Returns true iff this equals rhs component-wise.
        /// </summary>
        public static bool operator==(dVector2 lhs, dVector2 rhs) => lhs.Equals(rhs);
        
        /// <summary>
        /// Returns true iff this does not equal rhs (component-wise).
        /// </summary>
        public static bool operator!=(dVector2 lhs, dVector2 rhs) => !lhs.Equals(rhs);

        #endregion


        #region Functions
        
        /// <summary>
        /// Returns an enumerator that iterates through all components.
        /// </summary>
        public IEnumerator<double> GetEnumerator()
        {
            yield return x;
            yield return y;
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
        public string ToString(string sep) => (x + sep + y);
        
        /// <summary>
        /// Returns a string representation of this Vectortor using a provided seperator and a forMatrix provider for each component.
        /// </summary>
        public string ToString(string sep, IFormatProvider provider) => (x.ToString(provider) + sep + y.ToString(provider));
        
        /// <summary>
        /// Returns a string representation of this Vectortor using a provided seperator and a forMatrix for each component.
        /// </summary>
        public string ToString(string sep, string forMatrix) => (x.ToString(forMatrix) + sep + y.ToString(forMatrix));
        
        /// <summary>
        /// Returns a string representation of this Vectortor using a provided seperator and a forMatrix and forMatrix provider for each component.
        /// </summary>
        public string ToString(string sep, string forMatrix, IFormatProvider provider) => (x.ToString(forMatrix, provider) + sep + y.ToString(forMatrix, provider));
        
        /// <summary>
        /// Returns true iff this equals rhs component-wise.
        /// </summary>
        public bool Equals(dVector2 rhs) => (x.Equals(rhs.x) && y.Equals(rhs.y));
        
        /// <summary>
        /// Returns true iff this equals rhs type- and component-wise.
        /// </summary>
        public override bool Equals(object obj)
        {
            if (ReferenceEquals(null, obj)) return false;
            return obj is dVector2 && Equals((dVector2) obj);
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
        /// Returns the p-norm of this Vectortor.
        /// </summary>
        public double NormP(double p) => Math.Pow((Math.Pow((double)Math.Abs(x), p) + Math.Pow((double)Math.Abs(y), p)), 1 / p);
        
        /// <summary>
        /// Returns a 2D Vectortor that was rotated by a given angle in radians (CAUTION: result is casted and may be truncated).
        /// </summary>
        public dVector2 Rotated(double angleInRad) => (dVector2)(dVector2.FromAngle(Angle + angleInRad) * (double)Length);

        #endregion


        #region Static Functions
        
        /// <summary>
        /// Converts the string representation of the Vectortor into a Vectortor representation (using ', ' as a separator).
        /// </summary>
        public static dVector2 Parse(string s) => Parse(s, ", ");
        
        /// <summary>
        /// Converts the string representation of the Vectortor into a Vectortor representation (using a designated separator).
        /// </summary>
        public static dVector2 Parse(string s, string sep)
        {
            var kvp = s.Split(new[] { sep }, StringSplitOptions.None);
            if (kvp.Length != 2) throw new FormatException("input has not exactly 2 parts");
            return new dVector2(double.Parse(kvp[0].Trim()), double.Parse(kvp[1].Trim()));
        }
        
        /// <summary>
        /// Converts the string representation of the Vectortor into a Vectortor representation (using a designated separator and a type provider).
        /// </summary>
        public static dVector2 Parse(string s, string sep, IFormatProvider provider)
        {
            var kvp = s.Split(new[] { sep }, StringSplitOptions.None);
            if (kvp.Length != 2) throw new FormatException("input has not exactly 2 parts");
            return new dVector2(double.Parse(kvp[0].Trim(), provider), double.Parse(kvp[1].Trim(), provider));
        }
        
        /// <summary>
        /// Converts the string representation of the Vectortor into a Vectortor representation (using a designated separator and a number style).
        /// </summary>
        public static dVector2 Parse(string s, string sep, NumberStyles style)
        {
            var kvp = s.Split(new[] { sep }, StringSplitOptions.None);
            if (kvp.Length != 2) throw new FormatException("input has not exactly 2 parts");
            return new dVector2(double.Parse(kvp[0].Trim(), style), double.Parse(kvp[1].Trim(), style));
        }
        
        /// <summary>
        /// Converts the string representation of the Vectortor into a Vectortor representation (using a designated separator and a number style and a forMatrix provider).
        /// </summary>
        public static dVector2 Parse(string s, string sep, NumberStyles style, IFormatProvider provider)
        {
            var kvp = s.Split(new[] { sep }, StringSplitOptions.None);
            if (kvp.Length != 2) throw new FormatException("input has not exactly 2 parts");
            return new dVector2(double.Parse(kvp[0].Trim(), style, provider), double.Parse(kvp[1].Trim(), style, provider));
        }
        
        /// <summary>
        /// Tries to convert the string representation of the Vectortor into a Vectortor representation (using ', ' as a separator), returns false if string was invalid.
        /// </summary>
        public static bool TryParse(string s, out dVector2 result) => TryParse(s, ", ", out result);
        
        /// <summary>
        /// Tries to convert the string representation of the Vectortor into a Vectortor representation (using a designated separator), returns false if string was invalid.
        /// </summary>
        public static bool TryParse(string s, string sep, out dVector2 result)
        {
            result = Zero;
            if (string.IsNullOrEmpty(s)) return false;
            var kvp = s.Split(new[] { sep }, StringSplitOptions.None);
            if (kvp.Length != 2) return false;
            double x = 0.0, y = 0.0;
            var ok = (double.TryParse(kvp[0].Trim(), out x) && double.TryParse(kvp[1].Trim(), out y));
            result = ok ? new dVector2(x, y) : Zero;
            return ok;
        }
        
        /// <summary>
        /// Tries to convert the string representation of the Vectortor into a Vectortor representation (using a designated separator and a number style and a forMatrix provider), returns false if string was invalid.
        /// </summary>
        public static bool TryParse(string s, string sep, NumberStyles style, IFormatProvider provider, out dVector2 result)
        {
            result = Zero;
            if (string.IsNullOrEmpty(s)) return false;
            var kvp = s.Split(new[] { sep }, StringSplitOptions.None);
            if (kvp.Length != 2) return false;
            double x = 0.0, y = 0.0;
            var ok = (double.TryParse(kvp[0].Trim(), style, provider, out x) && double.TryParse(kvp[1].Trim(), style, provider, out y));
            result = ok ? new dVector2(x, y) : Zero;
            return ok;
        }
        
        /// <summary>
        /// Returns true iff distance between lhs and rhs is less than or equal to epsilon
        /// </summary>
        public static bool ApproxEqual(dVector2 lhs, dVector2 rhs, double eps = 0.1d) => Distance(lhs, rhs) <= eps;
        
        /// <summary>
        /// OuterProduct treats the first parameter c as a column Vectortor (Matrix with one column) and the second parameter r as a row Vectortor (Matrix with one row) and does a linear algebraic Matrix multiply c * r, yielding a Matrix whose number of rows is the number of components in c and whose number of columns is the number of components in r.
        /// </summary>
        public static dMatrix2 OuterProduct(dVector2 c, dVector2 r) => new dMatrix2(c.x * r.x, c.y * r.x, c.x * r.y, c.y * r.y);
        
        /// <summary>
        /// OuterProduct treats the first parameter c as a column Vectortor (Matrix with one column) and the second parameter r as a row Vectortor (Matrix with one row) and does a linear algebraic Matrix multiply c * r, yielding a Matrix whose number of rows is the number of components in c and whose number of columns is the number of components in r.
        /// </summary>
        public static dMatrix2x3 OuterProduct(dVector3 c, dVector2 r) => new dMatrix2x3(c.x * r.x, c.y * r.x, c.z * r.x, c.x * r.y, c.y * r.y, c.z * r.y);
        
        /// <summary>
        /// OuterProduct treats the first parameter c as a column Vectortor (Matrix with one column) and the second parameter r as a row Vectortor (Matrix with one row) and does a linear algebraic Matrix multiply c * r, yielding a Matrix whose number of rows is the number of components in c and whose number of columns is the number of components in r.
        /// </summary>
        public static dMatrix3x2 OuterProduct(dVector2 c, dVector3 r) => new dMatrix3x2(c.x * r.x, c.y * r.x, c.x * r.y, c.y * r.y, c.x * r.z, c.y * r.z);
        
        /// <summary>
        /// OuterProduct treats the first parameter c as a column Vectortor (Matrix with one column) and the second parameter r as a row Vectortor (Matrix with one row) and does a linear algebraic Matrix multiply c * r, yielding a Matrix whose number of rows is the number of components in c and whose number of columns is the number of components in r.
        /// </summary>
        public static dMatrix2x4 OuterProduct(dVector4 c, dVector2 r) => new dMatrix2x4(c.x * r.x, c.y * r.x, c.z * r.x, c.w * r.x, c.x * r.y, c.y * r.y, c.z * r.y, c.w * r.y);
        
        /// <summary>
        /// OuterProduct treats the first parameter c as a column Vectortor (Matrix with one column) and the second parameter r as a row Vectortor (Matrix with one row) and does a linear algebraic Matrix multiply c * r, yielding a Matrix whose number of rows is the number of components in c and whose number of columns is the number of components in r.
        /// </summary>
        public static dMatrix4x2 OuterProduct(dVector2 c, dVector4 r) => new dMatrix4x2(c.x * r.x, c.y * r.x, c.x * r.y, c.y * r.y, c.x * r.z, c.y * r.z, c.x * r.w, c.y * r.w);
        
        /// <summary>
        /// Returns a unit 2D Vectortor with a given angle in radians (CAUTION: result may be truncated for integer types).
        /// </summary>
        public static dVector2 FromAngle(double angleInRad) => new dVector2((double)Math.Cos(angleInRad), (double)Math.Sin(angleInRad));
        
        /// <summary>
        /// Returns the inner product (dot product, scalar product) of the two Vectortors.
        /// </summary>
        public static double Dot(dVector2 lhs, dVector2 rhs) => (lhs.x * rhs.x + lhs.y * rhs.y);
        
        /// <summary>
        /// Returns the euclidean distance between the two Vectortors.
        /// </summary>
        public static double Distance(dVector2 lhs, dVector2 rhs) => (lhs - rhs).Length;
        
        /// <summary>
        /// Returns the squared euclidean distance between the two Vectortors.
        /// </summary>
        public static double DistanceSqr(dVector2 lhs, dVector2 rhs) => (lhs - rhs).LengthSqr;
        
        /// <summary>
        /// Calculate the reflection direction for an incident Vectortor (N should be normalized in order to achieve the desired result).
        /// </summary>
        public static dVector2 Reflect(dVector2 I, dVector2 N) => I - 2 * Dot(N, I) * N;
        
        /// <summary>
        /// Calculate the refraction direction for an incident Vectortor (The input parameters I and N should be normalized in order to achieve the desired result).
        /// </summary>
        public static dVector2 Refract(dVector2 I, dVector2 N, double eta)
        {
            var dNI = Dot(N, I);
            var k = 1 - eta * eta * (1 - dNI * dNI);
            if (k < 0) return Zero;
            return eta * I - (eta * dNI + (double)Math.Sqrt(k)) * N;
        }
        
        /// <summary>
        /// Returns a Vectortor pointing in the same direction as another (faceforward orients a Vectortor to point away from a surface as defined by its normal. If dot(Nref, I) is negative faceforward returns N, otherwise it returns -N).
        /// </summary>
        public static dVector2 FaceForward(dVector2 N, dVector2 I, dVector2 Nref) => Dot(Nref, I) < 0 ? N : -N;
        
        /// <summary>
        /// Returns the length of the outer product (cross product, Vectortor product) of the two Vectortors.
        /// </summary>
        public static double Cross(dVector2 l, dVector2 r) => l.x * r.y - l.y * r.x;
        
        /// <summary>
        /// Returns a dVector2 with independent and identically distributed uniform values between 0.0 and 1.0.
        /// </summary>
        public static dVector2 Random(System.Random random) => new dVector2((double)random.NextDouble(), (double)random.NextDouble());
        
        /// <summary>
        /// Returns a dVector2 with independent and identically distributed uniform values between -1.0 and 1.0.
        /// </summary>
        public static dVector2 RandomSigned(System.Random random) => new dVector2((double)(random.NextDouble() * 2.0 - 1.0), (double)(random.NextDouble() * 2.0 - 1.0));
        
        /// <summary>
        /// Returns a dVector2 with independent and identically distributed values according to a normal distribution (zero mean, unit variance).
        /// </summary>
        public static dVector2 RandomNormal(System.Random random) => new dVector2((double)(Math.Cos(2 * Math.PI * random.NextDouble()) * Math.Sqrt(-2.0 * Math.Log(random.NextDouble()))), (double)(Math.Cos(2 * Math.PI * random.NextDouble()) * Math.Sqrt(-2.0 * Math.Log(random.NextDouble()))));

        #endregion


        #region Component-Wise Static Functions
        
        /// <summary>
        /// Returns a bVector2 from component-wise application of Equal (lhs == rhs).
        /// </summary>
        public static bVector2 Equal(dVector2 lhs, dVector2 rhs) => new bVector2(lhs.x == rhs.x, lhs.y == rhs.y);
        
        /// <summary>
        /// Returns a bVector2 from component-wise application of Equal (lhs == rhs).
        /// </summary>
        public static bVector2 Equal(dVector2 lhs, double rhs) => new bVector2(lhs.x == rhs, lhs.y == rhs);
        
        /// <summary>
        /// Returns a bVector2 from component-wise application of Equal (lhs == rhs).
        /// </summary>
        public static bVector2 Equal(double lhs, dVector2 rhs) => new bVector2(lhs == rhs.x, lhs == rhs.y);
        
        /// <summary>
        /// Returns a bVector from the application of Equal (lhs == rhs).
        /// </summary>
        public static bVector2 Equal(double lhs, double rhs) => new bVector2(lhs == rhs);
        
        /// <summary>
        /// Returns a bVector2 from component-wise application of NotEqual (lhs != rhs).
        /// </summary>
        public static bVector2 NotEqual(dVector2 lhs, dVector2 rhs) => new bVector2(lhs.x != rhs.x, lhs.y != rhs.y);
        
        /// <summary>
        /// Returns a bVector2 from component-wise application of NotEqual (lhs != rhs).
        /// </summary>
        public static bVector2 NotEqual(dVector2 lhs, double rhs) => new bVector2(lhs.x != rhs, lhs.y != rhs);
        
        /// <summary>
        /// Returns a bVector2 from component-wise application of NotEqual (lhs != rhs).
        /// </summary>
        public static bVector2 NotEqual(double lhs, dVector2 rhs) => new bVector2(lhs != rhs.x, lhs != rhs.y);
        
        /// <summary>
        /// Returns a bVector from the application of NotEqual (lhs != rhs).
        /// </summary>
        public static bVector2 NotEqual(double lhs, double rhs) => new bVector2(lhs != rhs);
        
        /// <summary>
        /// Returns a bVector2 from component-wise application of GreaterThan (lhs &gt; rhs).
        /// </summary>
        public static bVector2 GreaterThan(dVector2 lhs, dVector2 rhs) => new bVector2(lhs.x > rhs.x, lhs.y > rhs.y);
        
        /// <summary>
        /// Returns a bVector2 from component-wise application of GreaterThan (lhs &gt; rhs).
        /// </summary>
        public static bVector2 GreaterThan(dVector2 lhs, double rhs) => new bVector2(lhs.x > rhs, lhs.y > rhs);
        
        /// <summary>
        /// Returns a bVector2 from component-wise application of GreaterThan (lhs &gt; rhs).
        /// </summary>
        public static bVector2 GreaterThan(double lhs, dVector2 rhs) => new bVector2(lhs > rhs.x, lhs > rhs.y);
        
        /// <summary>
        /// Returns a bVector from the application of GreaterThan (lhs &gt; rhs).
        /// </summary>
        public static bVector2 GreaterThan(double lhs, double rhs) => new bVector2(lhs > rhs);
        
        /// <summary>
        /// Returns a bVector2 from component-wise application of GreaterThanEqual (lhs &gt;= rhs).
        /// </summary>
        public static bVector2 GreaterThanEqual(dVector2 lhs, dVector2 rhs) => new bVector2(lhs.x >= rhs.x, lhs.y >= rhs.y);
        
        /// <summary>
        /// Returns a bVector2 from component-wise application of GreaterThanEqual (lhs &gt;= rhs).
        /// </summary>
        public static bVector2 GreaterThanEqual(dVector2 lhs, double rhs) => new bVector2(lhs.x >= rhs, lhs.y >= rhs);
        
        /// <summary>
        /// Returns a bVector2 from component-wise application of GreaterThanEqual (lhs &gt;= rhs).
        /// </summary>
        public static bVector2 GreaterThanEqual(double lhs, dVector2 rhs) => new bVector2(lhs >= rhs.x, lhs >= rhs.y);
        
        /// <summary>
        /// Returns a bVector from the application of GreaterThanEqual (lhs &gt;= rhs).
        /// </summary>
        public static bVector2 GreaterThanEqual(double lhs, double rhs) => new bVector2(lhs >= rhs);
        
        /// <summary>
        /// Returns a bVector2 from component-wise application of LesserThan (lhs &lt; rhs).
        /// </summary>
        public static bVector2 LesserThan(dVector2 lhs, dVector2 rhs) => new bVector2(lhs.x < rhs.x, lhs.y < rhs.y);
        
        /// <summary>
        /// Returns a bVector2 from component-wise application of LesserThan (lhs &lt; rhs).
        /// </summary>
        public static bVector2 LesserThan(dVector2 lhs, double rhs) => new bVector2(lhs.x < rhs, lhs.y < rhs);
        
        /// <summary>
        /// Returns a bVector2 from component-wise application of LesserThan (lhs &lt; rhs).
        /// </summary>
        public static bVector2 LesserThan(double lhs, dVector2 rhs) => new bVector2(lhs < rhs.x, lhs < rhs.y);
        
        /// <summary>
        /// Returns a bVector from the application of LesserThan (lhs &lt; rhs).
        /// </summary>
        public static bVector2 LesserThan(double lhs, double rhs) => new bVector2(lhs < rhs);
        
        /// <summary>
        /// Returns a bVector2 from component-wise application of LesserThanEqual (lhs &lt;= rhs).
        /// </summary>
        public static bVector2 LesserThanEqual(dVector2 lhs, dVector2 rhs) => new bVector2(lhs.x <= rhs.x, lhs.y <= rhs.y);
        
        /// <summary>
        /// Returns a bVector2 from component-wise application of LesserThanEqual (lhs &lt;= rhs).
        /// </summary>
        public static bVector2 LesserThanEqual(dVector2 lhs, double rhs) => new bVector2(lhs.x <= rhs, lhs.y <= rhs);
        
        /// <summary>
        /// Returns a bVector2 from component-wise application of LesserThanEqual (lhs &lt;= rhs).
        /// </summary>
        public static bVector2 LesserThanEqual(double lhs, dVector2 rhs) => new bVector2(lhs <= rhs.x, lhs <= rhs.y);
        
        /// <summary>
        /// Returns a bVector from the application of LesserThanEqual (lhs &lt;= rhs).
        /// </summary>
        public static bVector2 LesserThanEqual(double lhs, double rhs) => new bVector2(lhs <= rhs);
        
        /// <summary>
        /// Returns a bVector2 from component-wise application of IsInfinity (double.IsInfinity(v)).
        /// </summary>
        public static bVector2 IsInfinity(dVector2 v) => new bVector2(double.IsInfinity(v.x), double.IsInfinity(v.y));
        
        /// <summary>
        /// Returns a bVector from the application of IsInfinity (double.IsInfinity(v)).
        /// </summary>
        public static bVector2 IsInfinity(double v) => new bVector2(double.IsInfinity(v));
        
        /// <summary>
        /// Returns a bVector2 from component-wise application of IsFinite (!double.IsNaN(v) &amp;&amp; !double.IsInfinity(v)).
        /// </summary>
        public static bVector2 IsFinite(dVector2 v) => new bVector2(!double.IsNaN(v.x) && !double.IsInfinity(v.x), !double.IsNaN(v.y) && !double.IsInfinity(v.y));
        
        /// <summary>
        /// Returns a bVector from the application of IsFinite (!double.IsNaN(v) &amp;&amp; !double.IsInfinity(v)).
        /// </summary>
        public static bVector2 IsFinite(double v) => new bVector2(!double.IsNaN(v) && !double.IsInfinity(v));
        
        /// <summary>
        /// Returns a bVector2 from component-wise application of IsNaN (double.IsNaN(v)).
        /// </summary>
        public static bVector2 IsNaN(dVector2 v) => new bVector2(double.IsNaN(v.x), double.IsNaN(v.y));
        
        /// <summary>
        /// Returns a bVector from the application of IsNaN (double.IsNaN(v)).
        /// </summary>
        public static bVector2 IsNaN(double v) => new bVector2(double.IsNaN(v));
        
        /// <summary>
        /// Returns a bVector2 from component-wise application of IsNegativeInfinity (double.IsNegativeInfinity(v)).
        /// </summary>
        public static bVector2 IsNegativeInfinity(dVector2 v) => new bVector2(double.IsNegativeInfinity(v.x), double.IsNegativeInfinity(v.y));
        
        /// <summary>
        /// Returns a bVector from the application of IsNegativeInfinity (double.IsNegativeInfinity(v)).
        /// </summary>
        public static bVector2 IsNegativeInfinity(double v) => new bVector2(double.IsNegativeInfinity(v));
        
        /// <summary>
        /// Returns a bVector2 from component-wise application of IsPositiveInfinity (double.IsPositiveInfinity(v)).
        /// </summary>
        public static bVector2 IsPositiveInfinity(dVector2 v) => new bVector2(double.IsPositiveInfinity(v.x), double.IsPositiveInfinity(v.y));
        
        /// <summary>
        /// Returns a bVector from the application of IsPositiveInfinity (double.IsPositiveInfinity(v)).
        /// </summary>
        public static bVector2 IsPositiveInfinity(double v) => new bVector2(double.IsPositiveInfinity(v));
        
        /// <summary>
        /// Returns a dVector2 from component-wise application of Abs (Math.Abs(v)).
        /// </summary>
        public static dVector2 Abs(dVector2 v) => new dVector2(Math.Abs(v.x), Math.Abs(v.y));
        
        /// <summary>
        /// Returns a dVector from the application of Abs (Math.Abs(v)).
        /// </summary>
        public static dVector2 Abs(double v) => new dVector2(Math.Abs(v));
        
        /// <summary>
        /// Returns a dVector2 from component-wise application of HermiteInterpolationOrder3 ((3 - 2 * v) * v * v).
        /// </summary>
        public static dVector2 HermiteInterpolationOrder3(dVector2 v) => new dVector2((3 - 2 * v.x) * v.x * v.x, (3 - 2 * v.y) * v.y * v.y);
        
        /// <summary>
        /// Returns a dVector from the application of HermiteInterpolationOrder3 ((3 - 2 * v) * v * v).
        /// </summary>
        public static dVector2 HermiteInterpolationOrder3(double v) => new dVector2((3 - 2 * v) * v * v);
        
        /// <summary>
        /// Returns a dVector2 from component-wise application of HermiteInterpolationOrder5 (((6 * v - 15) * v + 10) * v * v * v).
        /// </summary>
        public static dVector2 HermiteInterpolationOrder5(dVector2 v) => new dVector2(((6 * v.x - 15) * v.x + 10) * v.x * v.x * v.x, ((6 * v.y - 15) * v.y + 10) * v.y * v.y * v.y);
        
        /// <summary>
        /// Returns a dVector from the application of HermiteInterpolationOrder5 (((6 * v - 15) * v + 10) * v * v * v).
        /// </summary>
        public static dVector2 HermiteInterpolationOrder5(double v) => new dVector2(((6 * v - 15) * v + 10) * v * v * v);
        
        /// <summary>
        /// Returns a dVector2 from component-wise application of Sqr (v * v).
        /// </summary>
        public static dVector2 Sqr(dVector2 v) => new dVector2(v.x * v.x, v.y * v.y);
        
        /// <summary>
        /// Returns a dVector from the application of Sqr (v * v).
        /// </summary>
        public static dVector2 Sqr(double v) => new dVector2(v * v);
        
        /// <summary>
        /// Returns a dVector2 from component-wise application of Pow2 (v * v).
        /// </summary>
        public static dVector2 Pow2(dVector2 v) => new dVector2(v.x * v.x, v.y * v.y);
        
        /// <summary>
        /// Returns a dVector from the application of Pow2 (v * v).
        /// </summary>
        public static dVector2 Pow2(double v) => new dVector2(v * v);
        
        /// <summary>
        /// Returns a dVector2 from component-wise application of Pow3 (v * v * v).
        /// </summary>
        public static dVector2 Pow3(dVector2 v) => new dVector2(v.x * v.x * v.x, v.y * v.y * v.y);
        
        /// <summary>
        /// Returns a dVector from the application of Pow3 (v * v * v).
        /// </summary>
        public static dVector2 Pow3(double v) => new dVector2(v * v * v);
        
        /// <summary>
        /// Returns a dVector2 from component-wise application of Step (v &gt;= 0.0 ? 1.0 : 0.0).
        /// </summary>
        public static dVector2 Step(dVector2 v) => new dVector2(v.x >= 0.0 ? 1.0 : 0.0, v.y >= 0.0 ? 1.0 : 0.0);
        
        /// <summary>
        /// Returns a dVector from the application of Step (v &gt;= 0.0 ? 1.0 : 0.0).
        /// </summary>
        public static dVector2 Step(double v) => new dVector2(v >= 0.0 ? 1.0 : 0.0);
        
        /// <summary>
        /// Returns a dVector2 from component-wise application of Sqrt ((double)Math.Sqrt((double)v)).
        /// </summary>
        public static dVector2 Sqrt(dVector2 v) => new dVector2((double)Math.Sqrt((double)v.x), (double)Math.Sqrt((double)v.y));
        
        /// <summary>
        /// Returns a dVector from the application of Sqrt ((double)Math.Sqrt((double)v)).
        /// </summary>
        public static dVector2 Sqrt(double v) => new dVector2((double)Math.Sqrt((double)v));
        
        /// <summary>
        /// Returns a dVector2 from component-wise application of InverseSqrt ((double)(1.0 / Math.Sqrt((double)v))).
        /// </summary>
        public static dVector2 InverseSqrt(dVector2 v) => new dVector2((double)(1.0 / Math.Sqrt((double)v.x)), (double)(1.0 / Math.Sqrt((double)v.y)));
        
        /// <summary>
        /// Returns a dVector from the application of InverseSqrt ((double)(1.0 / Math.Sqrt((double)v))).
        /// </summary>
        public static dVector2 InverseSqrt(double v) => new dVector2((double)(1.0 / Math.Sqrt((double)v)));
        
        /// <summary>
        /// Returns a iVector2 from component-wise application of Sign (Math.Sign(v)).
        /// </summary>
        public static iVector2 Sign(dVector2 v) => new iVector2(Math.Sign(v.x), Math.Sign(v.y));
        
        /// <summary>
        /// Returns a iVector from the application of Sign (Math.Sign(v)).
        /// </summary>
        public static iVector2 Sign(double v) => new iVector2(Math.Sign(v));
        
        /// <summary>
        /// Returns a dVector2 from component-wise application of Max (Math.Max(lhs, rhs)).
        /// </summary>
        public static dVector2 Max(dVector2 lhs, dVector2 rhs) => new dVector2(Math.Max(lhs.x, rhs.x), Math.Max(lhs.y, rhs.y));
        
        /// <summary>
        /// Returns a dVector2 from component-wise application of Max (Math.Max(lhs, rhs)).
        /// </summary>
        public static dVector2 Max(dVector2 lhs, double rhs) => new dVector2(Math.Max(lhs.x, rhs), Math.Max(lhs.y, rhs));
        
        /// <summary>
        /// Returns a dVector2 from component-wise application of Max (Math.Max(lhs, rhs)).
        /// </summary>
        public static dVector2 Max(double lhs, dVector2 rhs) => new dVector2(Math.Max(lhs, rhs.x), Math.Max(lhs, rhs.y));
        
        /// <summary>
        /// Returns a dVector from the application of Max (Math.Max(lhs, rhs)).
        /// </summary>
        public static dVector2 Max(double lhs, double rhs) => new dVector2(Math.Max(lhs, rhs));
        
        /// <summary>
        /// Returns a dVector2 from component-wise application of Min (Math.Min(lhs, rhs)).
        /// </summary>
        public static dVector2 Min(dVector2 lhs, dVector2 rhs) => new dVector2(Math.Min(lhs.x, rhs.x), Math.Min(lhs.y, rhs.y));
        
        /// <summary>
        /// Returns a dVector2 from component-wise application of Min (Math.Min(lhs, rhs)).
        /// </summary>
        public static dVector2 Min(dVector2 lhs, double rhs) => new dVector2(Math.Min(lhs.x, rhs), Math.Min(lhs.y, rhs));
        
        /// <summary>
        /// Returns a dVector2 from component-wise application of Min (Math.Min(lhs, rhs)).
        /// </summary>
        public static dVector2 Min(double lhs, dVector2 rhs) => new dVector2(Math.Min(lhs, rhs.x), Math.Min(lhs, rhs.y));
        
        /// <summary>
        /// Returns a dVector from the application of Min (Math.Min(lhs, rhs)).
        /// </summary>
        public static dVector2 Min(double lhs, double rhs) => new dVector2(Math.Min(lhs, rhs));
        
        /// <summary>
        /// Returns a dVector2 from component-wise application of Pow ((double)Math.Pow((double)lhs, (double)rhs)).
        /// </summary>
        public static dVector2 Pow(dVector2 lhs, dVector2 rhs) => new dVector2((double)Math.Pow((double)lhs.x, (double)rhs.x), (double)Math.Pow((double)lhs.y, (double)rhs.y));
        
        /// <summary>
        /// Returns a dVector2 from component-wise application of Pow ((double)Math.Pow((double)lhs, (double)rhs)).
        /// </summary>
        public static dVector2 Pow(dVector2 lhs, double rhs) => new dVector2((double)Math.Pow((double)lhs.x, (double)rhs), (double)Math.Pow((double)lhs.y, (double)rhs));
        
        /// <summary>
        /// Returns a dVector2 from component-wise application of Pow ((double)Math.Pow((double)lhs, (double)rhs)).
        /// </summary>
        public static dVector2 Pow(double lhs, dVector2 rhs) => new dVector2((double)Math.Pow((double)lhs, (double)rhs.x), (double)Math.Pow((double)lhs, (double)rhs.y));
        
        /// <summary>
        /// Returns a dVector from the application of Pow ((double)Math.Pow((double)lhs, (double)rhs)).
        /// </summary>
        public static dVector2 Pow(double lhs, double rhs) => new dVector2((double)Math.Pow((double)lhs, (double)rhs));
        
        /// <summary>
        /// Returns a dVector2 from component-wise application of Log ((double)Math.Log((double)lhs, (double)rhs)).
        /// </summary>
        public static dVector2 Log(dVector2 lhs, dVector2 rhs) => new dVector2((double)Math.Log((double)lhs.x, (double)rhs.x), (double)Math.Log((double)lhs.y, (double)rhs.y));
        
        /// <summary>
        /// Returns a dVector2 from component-wise application of Log ((double)Math.Log((double)lhs, (double)rhs)).
        /// </summary>
        public static dVector2 Log(dVector2 lhs, double rhs) => new dVector2((double)Math.Log((double)lhs.x, (double)rhs), (double)Math.Log((double)lhs.y, (double)rhs));
        
        /// <summary>
        /// Returns a dVector2 from component-wise application of Log ((double)Math.Log((double)lhs, (double)rhs)).
        /// </summary>
        public static dVector2 Log(double lhs, dVector2 rhs) => new dVector2((double)Math.Log((double)lhs, (double)rhs.x), (double)Math.Log((double)lhs, (double)rhs.y));
        
        /// <summary>
        /// Returns a dVector from the application of Log ((double)Math.Log((double)lhs, (double)rhs)).
        /// </summary>
        public static dVector2 Log(double lhs, double rhs) => new dVector2((double)Math.Log((double)lhs, (double)rhs));
        
        /// <summary>
        /// Returns a dVector2 from component-wise application of Clamp (Math.Min(Math.Max(v, min), max)).
        /// </summary>
        public static dVector2 Clamp(dVector2 v, dVector2 min, dVector2 max) => new dVector2(Math.Min(Math.Max(v.x, min.x), max.x), Math.Min(Math.Max(v.y, min.y), max.y));
        
        /// <summary>
        /// Returns a dVector2 from component-wise application of Clamp (Math.Min(Math.Max(v, min), max)).
        /// </summary>
        public static dVector2 Clamp(dVector2 v, dVector2 min, double max) => new dVector2(Math.Min(Math.Max(v.x, min.x), max), Math.Min(Math.Max(v.y, min.y), max));
        
        /// <summary>
        /// Returns a dVector2 from component-wise application of Clamp (Math.Min(Math.Max(v, min), max)).
        /// </summary>
        public static dVector2 Clamp(dVector2 v, double min, dVector2 max) => new dVector2(Math.Min(Math.Max(v.x, min), max.x), Math.Min(Math.Max(v.y, min), max.y));
        
        /// <summary>
        /// Returns a dVector2 from component-wise application of Clamp (Math.Min(Math.Max(v, min), max)).
        /// </summary>
        public static dVector2 Clamp(dVector2 v, double min, double max) => new dVector2(Math.Min(Math.Max(v.x, min), max), Math.Min(Math.Max(v.y, min), max));
        
        /// <summary>
        /// Returns a dVector2 from component-wise application of Clamp (Math.Min(Math.Max(v, min), max)).
        /// </summary>
        public static dVector2 Clamp(double v, dVector2 min, dVector2 max) => new dVector2(Math.Min(Math.Max(v, min.x), max.x), Math.Min(Math.Max(v, min.y), max.y));
        
        /// <summary>
        /// Returns a dVector2 from component-wise application of Clamp (Math.Min(Math.Max(v, min), max)).
        /// </summary>
        public static dVector2 Clamp(double v, dVector2 min, double max) => new dVector2(Math.Min(Math.Max(v, min.x), max), Math.Min(Math.Max(v, min.y), max));
        
        /// <summary>
        /// Returns a dVector2 from component-wise application of Clamp (Math.Min(Math.Max(v, min), max)).
        /// </summary>
        public static dVector2 Clamp(double v, double min, dVector2 max) => new dVector2(Math.Min(Math.Max(v, min), max.x), Math.Min(Math.Max(v, min), max.y));
        
        /// <summary>
        /// Returns a dVector from the application of Clamp (Math.Min(Math.Max(v, min), max)).
        /// </summary>
        public static dVector2 Clamp(double v, double min, double max) => new dVector2(Math.Min(Math.Max(v, min), max));
        
        /// <summary>
        /// Returns a dVector2 from component-wise application of Mix (min * (1-a) + max * a).
        /// </summary>
        public static dVector2 Mix(dVector2 min, dVector2 max, dVector2 a) => new dVector2(min.x * (1-a.x) + max.x * a.x, min.y * (1-a.y) + max.y * a.y);
        
        /// <summary>
        /// Returns a dVector2 from component-wise application of Mix (min * (1-a) + max * a).
        /// </summary>
        public static dVector2 Mix(dVector2 min, dVector2 max, double a) => new dVector2(min.x * (1-a) + max.x * a, min.y * (1-a) + max.y * a);
        
        /// <summary>
        /// Returns a dVector2 from component-wise application of Mix (min * (1-a) + max * a).
        /// </summary>
        public static dVector2 Mix(dVector2 min, double max, dVector2 a) => new dVector2(min.x * (1-a.x) + max * a.x, min.y * (1-a.y) + max * a.y);
        
        /// <summary>
        /// Returns a dVector2 from component-wise application of Mix (min * (1-a) + max * a).
        /// </summary>
        public static dVector2 Mix(dVector2 min, double max, double a) => new dVector2(min.x * (1-a) + max * a, min.y * (1-a) + max * a);
        
        /// <summary>
        /// Returns a dVector2 from component-wise application of Mix (min * (1-a) + max * a).
        /// </summary>
        public static dVector2 Mix(double min, dVector2 max, dVector2 a) => new dVector2(min * (1-a.x) + max.x * a.x, min * (1-a.y) + max.y * a.y);
        
        /// <summary>
        /// Returns a dVector2 from component-wise application of Mix (min * (1-a) + max * a).
        /// </summary>
        public static dVector2 Mix(double min, dVector2 max, double a) => new dVector2(min * (1-a) + max.x * a, min * (1-a) + max.y * a);
        
        /// <summary>
        /// Returns a dVector2 from component-wise application of Mix (min * (1-a) + max * a).
        /// </summary>
        public static dVector2 Mix(double min, double max, dVector2 a) => new dVector2(min * (1-a.x) + max * a.x, min * (1-a.y) + max * a.y);
        
        /// <summary>
        /// Returns a dVector from the application of Mix (min * (1-a) + max * a).
        /// </summary>
        public static dVector2 Mix(double min, double max, double a) => new dVector2(min * (1-a) + max * a);
        
        /// <summary>
        /// Returns a dVector2 from component-wise application of Lerp (min * (1-a) + max * a).
        /// </summary>
        public static dVector2 Lerp(dVector2 min, dVector2 max, dVector2 a) => new dVector2(min.x * (1-a.x) + max.x * a.x, min.y * (1-a.y) + max.y * a.y);
        
        /// <summary>
        /// Returns a dVector2 from component-wise application of Lerp (min * (1-a) + max * a).
        /// </summary>
        public static dVector2 Lerp(dVector2 min, dVector2 max, double a) => new dVector2(min.x * (1-a) + max.x * a, min.y * (1-a) + max.y * a);
        
        /// <summary>
        /// Returns a dVector2 from component-wise application of Lerp (min * (1-a) + max * a).
        /// </summary>
        public static dVector2 Lerp(dVector2 min, double max, dVector2 a) => new dVector2(min.x * (1-a.x) + max * a.x, min.y * (1-a.y) + max * a.y);
        
        /// <summary>
        /// Returns a dVector2 from component-wise application of Lerp (min * (1-a) + max * a).
        /// </summary>
        public static dVector2 Lerp(dVector2 min, double max, double a) => new dVector2(min.x * (1-a) + max * a, min.y * (1-a) + max * a);
        
        /// <summary>
        /// Returns a dVector2 from component-wise application of Lerp (min * (1-a) + max * a).
        /// </summary>
        public static dVector2 Lerp(double min, dVector2 max, dVector2 a) => new dVector2(min * (1-a.x) + max.x * a.x, min * (1-a.y) + max.y * a.y);
        
        /// <summary>
        /// Returns a dVector2 from component-wise application of Lerp (min * (1-a) + max * a).
        /// </summary>
        public static dVector2 Lerp(double min, dVector2 max, double a) => new dVector2(min * (1-a) + max.x * a, min * (1-a) + max.y * a);
        
        /// <summary>
        /// Returns a dVector2 from component-wise application of Lerp (min * (1-a) + max * a).
        /// </summary>
        public static dVector2 Lerp(double min, double max, dVector2 a) => new dVector2(min * (1-a.x) + max * a.x, min * (1-a.y) + max * a.y);
        
        /// <summary>
        /// Returns a dVector from the application of Lerp (min * (1-a) + max * a).
        /// </summary>
        public static dVector2 Lerp(double min, double max, double a) => new dVector2(min * (1-a) + max * a);
        
        /// <summary>
        /// Returns a dVector2 from component-wise application of Smoothstep (((v - edge0) / (edge1 - edge0)).Clamp().HermiteInterpolationOrder3()).
        /// </summary>
        public static dVector2 Smoothstep(dVector2 edge0, dVector2 edge1, dVector2 v) => new dVector2(((v.x - edge0.x) / (edge1.x - edge0.x)).Clamp().HermiteInterpolationOrder3(), ((v.y - edge0.y) / (edge1.y - edge0.y)).Clamp().HermiteInterpolationOrder3());
        
        /// <summary>
        /// Returns a dVector2 from component-wise application of Smoothstep (((v - edge0) / (edge1 - edge0)).Clamp().HermiteInterpolationOrder3()).
        /// </summary>
        public static dVector2 Smoothstep(dVector2 edge0, dVector2 edge1, double v) => new dVector2(((v - edge0.x) / (edge1.x - edge0.x)).Clamp().HermiteInterpolationOrder3(), ((v - edge0.y) / (edge1.y - edge0.y)).Clamp().HermiteInterpolationOrder3());
        
        /// <summary>
        /// Returns a dVector2 from component-wise application of Smoothstep (((v - edge0) / (edge1 - edge0)).Clamp().HermiteInterpolationOrder3()).
        /// </summary>
        public static dVector2 Smoothstep(dVector2 edge0, double edge1, dVector2 v) => new dVector2(((v.x - edge0.x) / (edge1 - edge0.x)).Clamp().HermiteInterpolationOrder3(), ((v.y - edge0.y) / (edge1 - edge0.y)).Clamp().HermiteInterpolationOrder3());
        
        /// <summary>
        /// Returns a dVector2 from component-wise application of Smoothstep (((v - edge0) / (edge1 - edge0)).Clamp().HermiteInterpolationOrder3()).
        /// </summary>
        public static dVector2 Smoothstep(dVector2 edge0, double edge1, double v) => new dVector2(((v - edge0.x) / (edge1 - edge0.x)).Clamp().HermiteInterpolationOrder3(), ((v - edge0.y) / (edge1 - edge0.y)).Clamp().HermiteInterpolationOrder3());
        
        /// <summary>
        /// Returns a dVector2 from component-wise application of Smoothstep (((v - edge0) / (edge1 - edge0)).Clamp().HermiteInterpolationOrder3()).
        /// </summary>
        public static dVector2 Smoothstep(double edge0, dVector2 edge1, dVector2 v) => new dVector2(((v.x - edge0) / (edge1.x - edge0)).Clamp().HermiteInterpolationOrder3(), ((v.y - edge0) / (edge1.y - edge0)).Clamp().HermiteInterpolationOrder3());
        
        /// <summary>
        /// Returns a dVector2 from component-wise application of Smoothstep (((v - edge0) / (edge1 - edge0)).Clamp().HermiteInterpolationOrder3()).
        /// </summary>
        public static dVector2 Smoothstep(double edge0, dVector2 edge1, double v) => new dVector2(((v - edge0) / (edge1.x - edge0)).Clamp().HermiteInterpolationOrder3(), ((v - edge0) / (edge1.y - edge0)).Clamp().HermiteInterpolationOrder3());
        
        /// <summary>
        /// Returns a dVector2 from component-wise application of Smoothstep (((v - edge0) / (edge1 - edge0)).Clamp().HermiteInterpolationOrder3()).
        /// </summary>
        public static dVector2 Smoothstep(double edge0, double edge1, dVector2 v) => new dVector2(((v.x - edge0) / (edge1 - edge0)).Clamp().HermiteInterpolationOrder3(), ((v.y - edge0) / (edge1 - edge0)).Clamp().HermiteInterpolationOrder3());
        
        /// <summary>
        /// Returns a dVector from the application of Smoothstep (((v - edge0) / (edge1 - edge0)).Clamp().HermiteInterpolationOrder3()).
        /// </summary>
        public static dVector2 Smoothstep(double edge0, double edge1, double v) => new dVector2(((v - edge0) / (edge1 - edge0)).Clamp().HermiteInterpolationOrder3());
        
        /// <summary>
        /// Returns a dVector2 from component-wise application of Smootherstep (((v - edge0) / (edge1 - edge0)).Clamp().HermiteInterpolationOrder5()).
        /// </summary>
        public static dVector2 Smootherstep(dVector2 edge0, dVector2 edge1, dVector2 v) => new dVector2(((v.x - edge0.x) / (edge1.x - edge0.x)).Clamp().HermiteInterpolationOrder5(), ((v.y - edge0.y) / (edge1.y - edge0.y)).Clamp().HermiteInterpolationOrder5());
        
        /// <summary>
        /// Returns a dVector2 from component-wise application of Smootherstep (((v - edge0) / (edge1 - edge0)).Clamp().HermiteInterpolationOrder5()).
        /// </summary>
        public static dVector2 Smootherstep(dVector2 edge0, dVector2 edge1, double v) => new dVector2(((v - edge0.x) / (edge1.x - edge0.x)).Clamp().HermiteInterpolationOrder5(), ((v - edge0.y) / (edge1.y - edge0.y)).Clamp().HermiteInterpolationOrder5());
        
        /// <summary>
        /// Returns a dVector2 from component-wise application of Smootherstep (((v - edge0) / (edge1 - edge0)).Clamp().HermiteInterpolationOrder5()).
        /// </summary>
        public static dVector2 Smootherstep(dVector2 edge0, double edge1, dVector2 v) => new dVector2(((v.x - edge0.x) / (edge1 - edge0.x)).Clamp().HermiteInterpolationOrder5(), ((v.y - edge0.y) / (edge1 - edge0.y)).Clamp().HermiteInterpolationOrder5());
        
        /// <summary>
        /// Returns a dVector2 from component-wise application of Smootherstep (((v - edge0) / (edge1 - edge0)).Clamp().HermiteInterpolationOrder5()).
        /// </summary>
        public static dVector2 Smootherstep(dVector2 edge0, double edge1, double v) => new dVector2(((v - edge0.x) / (edge1 - edge0.x)).Clamp().HermiteInterpolationOrder5(), ((v - edge0.y) / (edge1 - edge0.y)).Clamp().HermiteInterpolationOrder5());
        
        /// <summary>
        /// Returns a dVector2 from component-wise application of Smootherstep (((v - edge0) / (edge1 - edge0)).Clamp().HermiteInterpolationOrder5()).
        /// </summary>
        public static dVector2 Smootherstep(double edge0, dVector2 edge1, dVector2 v) => new dVector2(((v.x - edge0) / (edge1.x - edge0)).Clamp().HermiteInterpolationOrder5(), ((v.y - edge0) / (edge1.y - edge0)).Clamp().HermiteInterpolationOrder5());
        
        /// <summary>
        /// Returns a dVector2 from component-wise application of Smootherstep (((v - edge0) / (edge1 - edge0)).Clamp().HermiteInterpolationOrder5()).
        /// </summary>
        public static dVector2 Smootherstep(double edge0, dVector2 edge1, double v) => new dVector2(((v - edge0) / (edge1.x - edge0)).Clamp().HermiteInterpolationOrder5(), ((v - edge0) / (edge1.y - edge0)).Clamp().HermiteInterpolationOrder5());
        
        /// <summary>
        /// Returns a dVector2 from component-wise application of Smootherstep (((v - edge0) / (edge1 - edge0)).Clamp().HermiteInterpolationOrder5()).
        /// </summary>
        public static dVector2 Smootherstep(double edge0, double edge1, dVector2 v) => new dVector2(((v.x - edge0) / (edge1 - edge0)).Clamp().HermiteInterpolationOrder5(), ((v.y - edge0) / (edge1 - edge0)).Clamp().HermiteInterpolationOrder5());
        
        /// <summary>
        /// Returns a dVector from the application of Smootherstep (((v - edge0) / (edge1 - edge0)).Clamp().HermiteInterpolationOrder5()).
        /// </summary>
        public static dVector2 Smootherstep(double edge0, double edge1, double v) => new dVector2(((v - edge0) / (edge1 - edge0)).Clamp().HermiteInterpolationOrder5());
        
        /// <summary>
        /// Returns a dVector2 from component-wise application of Fma (a * b + c).
        /// </summary>
        public static dVector2 Fma(dVector2 a, dVector2 b, dVector2 c) => new dVector2(a.x * b.x + c.x, a.y * b.y + c.y);
        
        /// <summary>
        /// Returns a dVector2 from component-wise application of Fma (a * b + c).
        /// </summary>
        public static dVector2 Fma(dVector2 a, dVector2 b, double c) => new dVector2(a.x * b.x + c, a.y * b.y + c);
        
        /// <summary>
        /// Returns a dVector2 from component-wise application of Fma (a * b + c).
        /// </summary>
        public static dVector2 Fma(dVector2 a, double b, dVector2 c) => new dVector2(a.x * b + c.x, a.y * b + c.y);
        
        /// <summary>
        /// Returns a dVector2 from component-wise application of Fma (a * b + c).
        /// </summary>
        public static dVector2 Fma(dVector2 a, double b, double c) => new dVector2(a.x * b + c, a.y * b + c);
        
        /// <summary>
        /// Returns a dVector2 from component-wise application of Fma (a * b + c).
        /// </summary>
        public static dVector2 Fma(double a, dVector2 b, dVector2 c) => new dVector2(a * b.x + c.x, a * b.y + c.y);
        
        /// <summary>
        /// Returns a dVector2 from component-wise application of Fma (a * b + c).
        /// </summary>
        public static dVector2 Fma(double a, dVector2 b, double c) => new dVector2(a * b.x + c, a * b.y + c);
        
        /// <summary>
        /// Returns a dVector2 from component-wise application of Fma (a * b + c).
        /// </summary>
        public static dVector2 Fma(double a, double b, dVector2 c) => new dVector2(a * b + c.x, a * b + c.y);
        
        /// <summary>
        /// Returns a dVector from the application of Fma (a * b + c).
        /// </summary>
        public static dVector2 Fma(double a, double b, double c) => new dVector2(a * b + c);
        
        /// <summary>
        /// Returns a dVector2 from component-wise application of Add (lhs + rhs).
        /// </summary>
        public static dVector2 Add(dVector2 lhs, dVector2 rhs) => new dVector2(lhs.x + rhs.x, lhs.y + rhs.y);
        
        /// <summary>
        /// Returns a dVector2 from component-wise application of Add (lhs + rhs).
        /// </summary>
        public static dVector2 Add(dVector2 lhs, double rhs) => new dVector2(lhs.x + rhs, lhs.y + rhs);
        
        /// <summary>
        /// Returns a dVector2 from component-wise application of Add (lhs + rhs).
        /// </summary>
        public static dVector2 Add(double lhs, dVector2 rhs) => new dVector2(lhs + rhs.x, lhs + rhs.y);
        
        /// <summary>
        /// Returns a dVector from the application of Add (lhs + rhs).
        /// </summary>
        public static dVector2 Add(double lhs, double rhs) => new dVector2(lhs + rhs);
        
        /// <summary>
        /// Returns a dVector2 from component-wise application of Sub (lhs - rhs).
        /// </summary>
        public static dVector2 Sub(dVector2 lhs, dVector2 rhs) => new dVector2(lhs.x - rhs.x, lhs.y - rhs.y);
        
        /// <summary>
        /// Returns a dVector2 from component-wise application of Sub (lhs - rhs).
        /// </summary>
        public static dVector2 Sub(dVector2 lhs, double rhs) => new dVector2(lhs.x - rhs, lhs.y - rhs);
        
        /// <summary>
        /// Returns a dVector2 from component-wise application of Sub (lhs - rhs).
        /// </summary>
        public static dVector2 Sub(double lhs, dVector2 rhs) => new dVector2(lhs - rhs.x, lhs - rhs.y);
        
        /// <summary>
        /// Returns a dVector from the application of Sub (lhs - rhs).
        /// </summary>
        public static dVector2 Sub(double lhs, double rhs) => new dVector2(lhs - rhs);
        
        /// <summary>
        /// Returns a dVector2 from component-wise application of Mul (lhs * rhs).
        /// </summary>
        public static dVector2 Mul(dVector2 lhs, dVector2 rhs) => new dVector2(lhs.x * rhs.x, lhs.y * rhs.y);
        
        /// <summary>
        /// Returns a dVector2 from component-wise application of Mul (lhs * rhs).
        /// </summary>
        public static dVector2 Mul(dVector2 lhs, double rhs) => new dVector2(lhs.x * rhs, lhs.y * rhs);
        
        /// <summary>
        /// Returns a dVector2 from component-wise application of Mul (lhs * rhs).
        /// </summary>
        public static dVector2 Mul(double lhs, dVector2 rhs) => new dVector2(lhs * rhs.x, lhs * rhs.y);
        
        /// <summary>
        /// Returns a dVector from the application of Mul (lhs * rhs).
        /// </summary>
        public static dVector2 Mul(double lhs, double rhs) => new dVector2(lhs * rhs);
        
        /// <summary>
        /// Returns a dVector2 from component-wise application of Div (lhs / rhs).
        /// </summary>
        public static dVector2 Div(dVector2 lhs, dVector2 rhs) => new dVector2(lhs.x / rhs.x, lhs.y / rhs.y);
        
        /// <summary>
        /// Returns a dVector2 from component-wise application of Div (lhs / rhs).
        /// </summary>
        public static dVector2 Div(dVector2 lhs, double rhs) => new dVector2(lhs.x / rhs, lhs.y / rhs);
        
        /// <summary>
        /// Returns a dVector2 from component-wise application of Div (lhs / rhs).
        /// </summary>
        public static dVector2 Div(double lhs, dVector2 rhs) => new dVector2(lhs / rhs.x, lhs / rhs.y);
        
        /// <summary>
        /// Returns a dVector from the application of Div (lhs / rhs).
        /// </summary>
        public static dVector2 Div(double lhs, double rhs) => new dVector2(lhs / rhs);
        
        /// <summary>
        /// Returns a dVector2 from component-wise application of Modulo (lhs % rhs).
        /// </summary>
        public static dVector2 Modulo(dVector2 lhs, dVector2 rhs) => new dVector2(lhs.x % rhs.x, lhs.y % rhs.y);
        
        /// <summary>
        /// Returns a dVector2 from component-wise application of Modulo (lhs % rhs).
        /// </summary>
        public static dVector2 Modulo(dVector2 lhs, double rhs) => new dVector2(lhs.x % rhs, lhs.y % rhs);
        
        /// <summary>
        /// Returns a dVector2 from component-wise application of Modulo (lhs % rhs).
        /// </summary>
        public static dVector2 Modulo(double lhs, dVector2 rhs) => new dVector2(lhs % rhs.x, lhs % rhs.y);
        
        /// <summary>
        /// Returns a dVector from the application of Modulo (lhs % rhs).
        /// </summary>
        public static dVector2 Modulo(double lhs, double rhs) => new dVector2(lhs % rhs);
        
        /// <summary>
        /// Returns a dVector2 from component-wise application of Degrees (Radians-To-Degrees Conversion).
        /// </summary>
        public static dVector2 Degrees(dVector2 v) => new dVector2((double)(v.x * 57.295779513082320876798154814105170332405472466564321d), (double)(v.y * 57.295779513082320876798154814105170332405472466564321d));
        
        /// <summary>
        /// Returns a dVector from the application of Degrees (Radians-To-Degrees Conversion).
        /// </summary>
        public static dVector2 Degrees(double v) => new dVector2((double)(v * 57.295779513082320876798154814105170332405472466564321d));
        
        /// <summary>
        /// Returns a dVector2 from component-wise application of Radians (Degrees-To-Radians Conversion).
        /// </summary>
        public static dVector2 Radians(dVector2 v) => new dVector2((double)(v.x * 0.0174532925199432957692369076848861271344287188854172d), (double)(v.y * 0.0174532925199432957692369076848861271344287188854172d));
        
        /// <summary>
        /// Returns a dVector from the application of Radians (Degrees-To-Radians Conversion).
        /// </summary>
        public static dVector2 Radians(double v) => new dVector2((double)(v * 0.0174532925199432957692369076848861271344287188854172d));
        
        /// <summary>
        /// Returns a dVector2 from component-wise application of Acos ((double)Math.Acos((double)v)).
        /// </summary>
        public static dVector2 Acos(dVector2 v) => new dVector2((double)Math.Acos((double)v.x), (double)Math.Acos((double)v.y));
        
        /// <summary>
        /// Returns a dVector from the application of Acos ((double)Math.Acos((double)v)).
        /// </summary>
        public static dVector2 Acos(double v) => new dVector2((double)Math.Acos((double)v));
        
        /// <summary>
        /// Returns a dVector2 from component-wise application of Asin ((double)Math.Asin((double)v)).
        /// </summary>
        public static dVector2 Asin(dVector2 v) => new dVector2((double)Math.Asin((double)v.x), (double)Math.Asin((double)v.y));
        
        /// <summary>
        /// Returns a dVector from the application of Asin ((double)Math.Asin((double)v)).
        /// </summary>
        public static dVector2 Asin(double v) => new dVector2((double)Math.Asin((double)v));
        
        /// <summary>
        /// Returns a dVector2 from component-wise application of Atan ((double)Math.Atan((double)v)).
        /// </summary>
        public static dVector2 Atan(dVector2 v) => new dVector2((double)Math.Atan((double)v.x), (double)Math.Atan((double)v.y));
        
        /// <summary>
        /// Returns a dVector from the application of Atan ((double)Math.Atan((double)v)).
        /// </summary>
        public static dVector2 Atan(double v) => new dVector2((double)Math.Atan((double)v));
        
        /// <summary>
        /// Returns a dVector2 from component-wise application of Cos ((double)Math.Cos((double)v)).
        /// </summary>
        public static dVector2 Cos(dVector2 v) => new dVector2((double)Math.Cos((double)v.x), (double)Math.Cos((double)v.y));
        
        /// <summary>
        /// Returns a dVector from the application of Cos ((double)Math.Cos((double)v)).
        /// </summary>
        public static dVector2 Cos(double v) => new dVector2((double)Math.Cos((double)v));
        
        /// <summary>
        /// Returns a dVector2 from component-wise application of Cosh ((double)Math.Cosh((double)v)).
        /// </summary>
        public static dVector2 Cosh(dVector2 v) => new dVector2((double)Math.Cosh((double)v.x), (double)Math.Cosh((double)v.y));
        
        /// <summary>
        /// Returns a dVector from the application of Cosh ((double)Math.Cosh((double)v)).
        /// </summary>
        public static dVector2 Cosh(double v) => new dVector2((double)Math.Cosh((double)v));
        
        /// <summary>
        /// Returns a dVector2 from component-wise application of Exp ((double)Math.Exp((double)v)).
        /// </summary>
        public static dVector2 Exp(dVector2 v) => new dVector2((double)Math.Exp((double)v.x), (double)Math.Exp((double)v.y));
        
        /// <summary>
        /// Returns a dVector from the application of Exp ((double)Math.Exp((double)v)).
        /// </summary>
        public static dVector2 Exp(double v) => new dVector2((double)Math.Exp((double)v));
        
        /// <summary>
        /// Returns a dVector2 from component-wise application of Log ((double)Math.Log((double)v)).
        /// </summary>
        public static dVector2 Log(dVector2 v) => new dVector2((double)Math.Log((double)v.x), (double)Math.Log((double)v.y));
        
        /// <summary>
        /// Returns a dVector from the application of Log ((double)Math.Log((double)v)).
        /// </summary>
        public static dVector2 Log(double v) => new dVector2((double)Math.Log((double)v));
        
        /// <summary>
        /// Returns a dVector2 from component-wise application of Log2 ((double)Math.Log((double)v, 2)).
        /// </summary>
        public static dVector2 Log2(dVector2 v) => new dVector2((double)Math.Log((double)v.x, 2), (double)Math.Log((double)v.y, 2));
        
        /// <summary>
        /// Returns a dVector from the application of Log2 ((double)Math.Log((double)v, 2)).
        /// </summary>
        public static dVector2 Log2(double v) => new dVector2((double)Math.Log((double)v, 2));
        
        /// <summary>
        /// Returns a dVector2 from component-wise application of Log10 ((double)Math.Log10((double)v)).
        /// </summary>
        public static dVector2 Log10(dVector2 v) => new dVector2((double)Math.Log10((double)v.x), (double)Math.Log10((double)v.y));
        
        /// <summary>
        /// Returns a dVector from the application of Log10 ((double)Math.Log10((double)v)).
        /// </summary>
        public static dVector2 Log10(double v) => new dVector2((double)Math.Log10((double)v));
        
        /// <summary>
        /// Returns a dVector2 from component-wise application of Floor ((double)Math.Floor(v)).
        /// </summary>
        public static dVector2 Floor(dVector2 v) => new dVector2((double)Math.Floor(v.x), (double)Math.Floor(v.y));
        
        /// <summary>
        /// Returns a dVector from the application of Floor ((double)Math.Floor(v)).
        /// </summary>
        public static dVector2 Floor(double v) => new dVector2((double)Math.Floor(v));
        
        /// <summary>
        /// Returns a dVector2 from component-wise application of Ceiling ((double)Math.Ceiling(v)).
        /// </summary>
        public static dVector2 Ceiling(dVector2 v) => new dVector2((double)Math.Ceiling(v.x), (double)Math.Ceiling(v.y));
        
        /// <summary>
        /// Returns a dVector from the application of Ceiling ((double)Math.Ceiling(v)).
        /// </summary>
        public static dVector2 Ceiling(double v) => new dVector2((double)Math.Ceiling(v));
        
        /// <summary>
        /// Returns a dVector2 from component-wise application of Round ((double)Math.Round(v)).
        /// </summary>
        public static dVector2 Round(dVector2 v) => new dVector2((double)Math.Round(v.x), (double)Math.Round(v.y));
        
        /// <summary>
        /// Returns a dVector from the application of Round ((double)Math.Round(v)).
        /// </summary>
        public static dVector2 Round(double v) => new dVector2((double)Math.Round(v));
        
        /// <summary>
        /// Returns a dVector2 from component-wise application of Sin ((double)Math.Sin((double)v)).
        /// </summary>
        public static dVector2 Sin(dVector2 v) => new dVector2((double)Math.Sin((double)v.x), (double)Math.Sin((double)v.y));
        
        /// <summary>
        /// Returns a dVector from the application of Sin ((double)Math.Sin((double)v)).
        /// </summary>
        public static dVector2 Sin(double v) => new dVector2((double)Math.Sin((double)v));
        
        /// <summary>
        /// Returns a dVector2 from component-wise application of Sinh ((double)Math.Sinh((double)v)).
        /// </summary>
        public static dVector2 Sinh(dVector2 v) => new dVector2((double)Math.Sinh((double)v.x), (double)Math.Sinh((double)v.y));
        
        /// <summary>
        /// Returns a dVector from the application of Sinh ((double)Math.Sinh((double)v)).
        /// </summary>
        public static dVector2 Sinh(double v) => new dVector2((double)Math.Sinh((double)v));
        
        /// <summary>
        /// Returns a dVector2 from component-wise application of Tan ((double)Math.Tan((double)v)).
        /// </summary>
        public static dVector2 Tan(dVector2 v) => new dVector2((double)Math.Tan((double)v.x), (double)Math.Tan((double)v.y));
        
        /// <summary>
        /// Returns a dVector from the application of Tan ((double)Math.Tan((double)v)).
        /// </summary>
        public static dVector2 Tan(double v) => new dVector2((double)Math.Tan((double)v));
        
        /// <summary>
        /// Returns a dVector2 from component-wise application of Tanh ((double)Math.Tanh((double)v)).
        /// </summary>
        public static dVector2 Tanh(dVector2 v) => new dVector2((double)Math.Tanh((double)v.x), (double)Math.Tanh((double)v.y));
        
        /// <summary>
        /// Returns a dVector from the application of Tanh ((double)Math.Tanh((double)v)).
        /// </summary>
        public static dVector2 Tanh(double v) => new dVector2((double)Math.Tanh((double)v));
        
        /// <summary>
        /// Returns a dVector2 from component-wise application of Truncate ((double)Math.Truncate((double)v)).
        /// </summary>
        public static dVector2 Truncate(dVector2 v) => new dVector2((double)Math.Truncate((double)v.x), (double)Math.Truncate((double)v.y));
        
        /// <summary>
        /// Returns a dVector from the application of Truncate ((double)Math.Truncate((double)v)).
        /// </summary>
        public static dVector2 Truncate(double v) => new dVector2((double)Math.Truncate((double)v));
        
        /// <summary>
        /// Returns a dVector2 from component-wise application of Fract ((double)(v - Math.Floor(v))).
        /// </summary>
        public static dVector2 Fract(dVector2 v) => new dVector2((double)(v.x - Math.Floor(v.x)), (double)(v.y - Math.Floor(v.y)));
        
        /// <summary>
        /// Returns a dVector from the application of Fract ((double)(v - Math.Floor(v))).
        /// </summary>
        public static dVector2 Fract(double v) => new dVector2((double)(v - Math.Floor(v)));
        
        /// <summary>
        /// Returns a dVector2 from component-wise application of Trunc ((long)(v)).
        /// </summary>
        public static dVector2 Trunc(dVector2 v) => new dVector2((long)(v.x), (long)(v.y));
        
        /// <summary>
        /// Returns a dVector from the application of Trunc ((long)(v)).
        /// </summary>
        public static dVector2 Trunc(double v) => new dVector2((long)(v));
        
        /// <summary>
        /// Returns a dVector2 with independent and identically distributed uniform values between 'minValue' and 'maxValue'.
        /// </summary>
        public static dVector2 Random(System.Random random, dVector2 minValue, dVector2 maxValue) => new dVector2((double)random.NextDouble() * (maxValue.x - minValue.x) + minValue.x, (double)random.NextDouble() * (maxValue.y - minValue.y) + minValue.y);
        
        /// <summary>
        /// Returns a dVector2 with independent and identically distributed uniform values between 'minValue' and 'maxValue'.
        /// </summary>
        public static dVector2 Random(System.Random random, dVector2 minValue, double maxValue) => new dVector2((double)random.NextDouble() * (maxValue - minValue.x) + minValue.x, (double)random.NextDouble() * (maxValue - minValue.y) + minValue.y);
        
        /// <summary>
        /// Returns a dVector2 with independent and identically distributed uniform values between 'minValue' and 'maxValue'.
        /// </summary>
        public static dVector2 Random(System.Random random, double minValue, dVector2 maxValue) => new dVector2((double)random.NextDouble() * (maxValue.x - minValue) + minValue, (double)random.NextDouble() * (maxValue.y - minValue) + minValue);
        
        /// <summary>
        /// Returns a dVector2 with independent and identically distributed uniform values between 'minValue' and 'maxValue'.
        /// </summary>
        public static dVector2 Random(System.Random random, double minValue, double maxValue) => new dVector2((double)random.NextDouble() * (maxValue - minValue) + minValue);
        
        /// <summary>
        /// Returns a dVector2 with independent and identically distributed uniform values between 'minValue' and 'maxValue'.
        /// </summary>
        public static dVector2 RandomUniform(System.Random random, dVector2 minValue, dVector2 maxValue) => new dVector2((double)random.NextDouble() * (maxValue.x - minValue.x) + minValue.x, (double)random.NextDouble() * (maxValue.y - minValue.y) + minValue.y);
        
        /// <summary>
        /// Returns a dVector2 with independent and identically distributed uniform values between 'minValue' and 'maxValue'.
        /// </summary>
        public static dVector2 RandomUniform(System.Random random, dVector2 minValue, double maxValue) => new dVector2((double)random.NextDouble() * (maxValue - minValue.x) + minValue.x, (double)random.NextDouble() * (maxValue - minValue.y) + minValue.y);
        
        /// <summary>
        /// Returns a dVector2 with independent and identically distributed uniform values between 'minValue' and 'maxValue'.
        /// </summary>
        public static dVector2 RandomUniform(System.Random random, double minValue, dVector2 maxValue) => new dVector2((double)random.NextDouble() * (maxValue.x - minValue) + minValue, (double)random.NextDouble() * (maxValue.y - minValue) + minValue);
        
        /// <summary>
        /// Returns a dVector2 with independent and identically distributed uniform values between 'minValue' and 'maxValue'.
        /// </summary>
        public static dVector2 RandomUniform(System.Random random, double minValue, double maxValue) => new dVector2((double)random.NextDouble() * (maxValue - minValue) + minValue);
        
        /// <summary>
        /// Returns a dVector2 with independent and identically distributed values according to a normal/Gaussian distribution with specified mean and variance.
        /// </summary>
        public static dVector2 RandomNormal(System.Random random, dVector2 mean, dVector2 variance) => new dVector2((double)(Math.Sqrt((double)variance.x) * Math.Cos(2 * Math.PI * random.NextDouble()) * Math.Sqrt(-2.0 * Math.Log(random.NextDouble()))) + mean.x, (double)(Math.Sqrt((double)variance.y) * Math.Cos(2 * Math.PI * random.NextDouble()) * Math.Sqrt(-2.0 * Math.Log(random.NextDouble()))) + mean.y);
        
        /// <summary>
        /// Returns a dVector2 with independent and identically distributed values according to a normal/Gaussian distribution with specified mean and variance.
        /// </summary>
        public static dVector2 RandomNormal(System.Random random, dVector2 mean, double variance) => new dVector2((double)(Math.Sqrt((double)variance) * Math.Cos(2 * Math.PI * random.NextDouble()) * Math.Sqrt(-2.0 * Math.Log(random.NextDouble()))) + mean.x, (double)(Math.Sqrt((double)variance) * Math.Cos(2 * Math.PI * random.NextDouble()) * Math.Sqrt(-2.0 * Math.Log(random.NextDouble()))) + mean.y);
        
        /// <summary>
        /// Returns a dVector2 with independent and identically distributed values according to a normal/Gaussian distribution with specified mean and variance.
        /// </summary>
        public static dVector2 RandomNormal(System.Random random, double mean, dVector2 variance) => new dVector2((double)(Math.Sqrt((double)variance.x) * Math.Cos(2 * Math.PI * random.NextDouble()) * Math.Sqrt(-2.0 * Math.Log(random.NextDouble()))) + mean, (double)(Math.Sqrt((double)variance.y) * Math.Cos(2 * Math.PI * random.NextDouble()) * Math.Sqrt(-2.0 * Math.Log(random.NextDouble()))) + mean);
        
        /// <summary>
        /// Returns a dVector2 with independent and identically distributed values according to a normal/Gaussian distribution with specified mean and variance.
        /// </summary>
        public static dVector2 RandomNormal(System.Random random, double mean, double variance) => new dVector2((double)(Math.Sqrt((double)variance) * Math.Cos(2 * Math.PI * random.NextDouble()) * Math.Sqrt(-2.0 * Math.Log(random.NextDouble()))) + mean);
        
        /// <summary>
        /// Returns a dVector2 with independent and identically distributed values according to a normal/Gaussian distribution with specified mean and variance.
        /// </summary>
        public static dVector2 RandomGaussian(System.Random random, dVector2 mean, dVector2 variance) => new dVector2((double)(Math.Sqrt((double)variance.x) * Math.Cos(2 * Math.PI * random.NextDouble()) * Math.Sqrt(-2.0 * Math.Log(random.NextDouble()))) + mean.x, (double)(Math.Sqrt((double)variance.y) * Math.Cos(2 * Math.PI * random.NextDouble()) * Math.Sqrt(-2.0 * Math.Log(random.NextDouble()))) + mean.y);
        
        /// <summary>
        /// Returns a dVector2 with independent and identically distributed values according to a normal/Gaussian distribution with specified mean and variance.
        /// </summary>
        public static dVector2 RandomGaussian(System.Random random, dVector2 mean, double variance) => new dVector2((double)(Math.Sqrt((double)variance) * Math.Cos(2 * Math.PI * random.NextDouble()) * Math.Sqrt(-2.0 * Math.Log(random.NextDouble()))) + mean.x, (double)(Math.Sqrt((double)variance) * Math.Cos(2 * Math.PI * random.NextDouble()) * Math.Sqrt(-2.0 * Math.Log(random.NextDouble()))) + mean.y);
        
        /// <summary>
        /// Returns a dVector2 with independent and identically distributed values according to a normal/Gaussian distribution with specified mean and variance.
        /// </summary>
        public static dVector2 RandomGaussian(System.Random random, double mean, dVector2 variance) => new dVector2((double)(Math.Sqrt((double)variance.x) * Math.Cos(2 * Math.PI * random.NextDouble()) * Math.Sqrt(-2.0 * Math.Log(random.NextDouble()))) + mean, (double)(Math.Sqrt((double)variance.y) * Math.Cos(2 * Math.PI * random.NextDouble()) * Math.Sqrt(-2.0 * Math.Log(random.NextDouble()))) + mean);
        
        /// <summary>
        /// Returns a dVector2 with independent and identically distributed values according to a normal/Gaussian distribution with specified mean and variance.
        /// </summary>
        public static dVector2 RandomGaussian(System.Random random, double mean, double variance) => new dVector2((double)(Math.Sqrt((double)variance) * Math.Cos(2 * Math.PI * random.NextDouble()) * Math.Sqrt(-2.0 * Math.Log(random.NextDouble()))) + mean);

        #endregion


        #region Component-Wise Operator Overloads
        
        /// <summary>
        /// Returns a bVector2 from component-wise application of operator&lt; (lhs &lt; rhs).
        /// </summary>
        public static bVector2 operator<(dVector2 lhs, dVector2 rhs) => new bVector2(lhs.x < rhs.x, lhs.y < rhs.y);
        
        /// <summary>
        /// Returns a bVector2 from component-wise application of operator&lt; (lhs &lt; rhs).
        /// </summary>
        public static bVector2 operator<(dVector2 lhs, double rhs) => new bVector2(lhs.x < rhs, lhs.y < rhs);
        
        /// <summary>
        /// Returns a bVector2 from component-wise application of operator&lt; (lhs &lt; rhs).
        /// </summary>
        public static bVector2 operator<(double lhs, dVector2 rhs) => new bVector2(lhs < rhs.x, lhs < rhs.y);
        
        /// <summary>
        /// Returns a bVector2 from component-wise application of operator&lt;= (lhs &lt;= rhs).
        /// </summary>
        public static bVector2 operator<=(dVector2 lhs, dVector2 rhs) => new bVector2(lhs.x <= rhs.x, lhs.y <= rhs.y);
        
        /// <summary>
        /// Returns a bVector2 from component-wise application of operator&lt;= (lhs &lt;= rhs).
        /// </summary>
        public static bVector2 operator<=(dVector2 lhs, double rhs) => new bVector2(lhs.x <= rhs, lhs.y <= rhs);
        
        /// <summary>
        /// Returns a bVector2 from component-wise application of operator&lt;= (lhs &lt;= rhs).
        /// </summary>
        public static bVector2 operator<=(double lhs, dVector2 rhs) => new bVector2(lhs <= rhs.x, lhs <= rhs.y);
        
        /// <summary>
        /// Returns a bVector2 from component-wise application of operator&gt; (lhs &gt; rhs).
        /// </summary>
        public static bVector2 operator>(dVector2 lhs, dVector2 rhs) => new bVector2(lhs.x > rhs.x, lhs.y > rhs.y);
        
        /// <summary>
        /// Returns a bVector2 from component-wise application of operator&gt; (lhs &gt; rhs).
        /// </summary>
        public static bVector2 operator>(dVector2 lhs, double rhs) => new bVector2(lhs.x > rhs, lhs.y > rhs);
        
        /// <summary>
        /// Returns a bVector2 from component-wise application of operator&gt; (lhs &gt; rhs).
        /// </summary>
        public static bVector2 operator>(double lhs, dVector2 rhs) => new bVector2(lhs > rhs.x, lhs > rhs.y);
        
        /// <summary>
        /// Returns a bVector2 from component-wise application of operator&gt;= (lhs &gt;= rhs).
        /// </summary>
        public static bVector2 operator>=(dVector2 lhs, dVector2 rhs) => new bVector2(lhs.x >= rhs.x, lhs.y >= rhs.y);
        
        /// <summary>
        /// Returns a bVector2 from component-wise application of operator&gt;= (lhs &gt;= rhs).
        /// </summary>
        public static bVector2 operator>=(dVector2 lhs, double rhs) => new bVector2(lhs.x >= rhs, lhs.y >= rhs);
        
        /// <summary>
        /// Returns a bVector2 from component-wise application of operator&gt;= (lhs &gt;= rhs).
        /// </summary>
        public static bVector2 operator>=(double lhs, dVector2 rhs) => new bVector2(lhs >= rhs.x, lhs >= rhs.y);
        
        /// <summary>
        /// Returns a dVector2 from component-wise application of operator+ (lhs + rhs).
        /// </summary>
        public static dVector2 operator+(dVector2 lhs, dVector2 rhs) => new dVector2(lhs.x + rhs.x, lhs.y + rhs.y);
        
        /// <summary>
        /// Returns a dVector2 from component-wise application of operator+ (lhs + rhs).
        /// </summary>
        public static dVector2 operator+(dVector2 lhs, double rhs) => new dVector2(lhs.x + rhs, lhs.y + rhs);
        
        /// <summary>
        /// Returns a dVector2 from component-wise application of operator+ (lhs + rhs).
        /// </summary>
        public static dVector2 operator+(double lhs, dVector2 rhs) => new dVector2(lhs + rhs.x, lhs + rhs.y);
        
        /// <summary>
        /// Returns a dVector2 from component-wise application of operator- (lhs - rhs).
        /// </summary>
        public static dVector2 operator-(dVector2 lhs, dVector2 rhs) => new dVector2(lhs.x - rhs.x, lhs.y - rhs.y);
        
        /// <summary>
        /// Returns a dVector2 from component-wise application of operator- (lhs - rhs).
        /// </summary>
        public static dVector2 operator-(dVector2 lhs, double rhs) => new dVector2(lhs.x - rhs, lhs.y - rhs);
        
        /// <summary>
        /// Returns a dVector2 from component-wise application of operator- (lhs - rhs).
        /// </summary>
        public static dVector2 operator-(double lhs, dVector2 rhs) => new dVector2(lhs - rhs.x, lhs - rhs.y);
        
        /// <summary>
        /// Returns a dVector2 from component-wise application of operator* (lhs * rhs).
        /// </summary>
        public static dVector2 operator*(dVector2 lhs, dVector2 rhs) => new dVector2(lhs.x * rhs.x, lhs.y * rhs.y);
        
        /// <summary>
        /// Returns a dVector2 from component-wise application of operator* (lhs * rhs).
        /// </summary>
        public static dVector2 operator*(dVector2 lhs, double rhs) => new dVector2(lhs.x * rhs, lhs.y * rhs);
        
        /// <summary>
        /// Returns a dVector2 from component-wise application of operator* (lhs * rhs).
        /// </summary>
        public static dVector2 operator*(double lhs, dVector2 rhs) => new dVector2(lhs * rhs.x, lhs * rhs.y);
        
        /// <summary>
        /// Returns a dVector2 from component-wise application of operator/ (lhs / rhs).
        /// </summary>
        public static dVector2 operator/(dVector2 lhs, dVector2 rhs) => new dVector2(lhs.x / rhs.x, lhs.y / rhs.y);
        
        /// <summary>
        /// Returns a dVector2 from component-wise application of operator/ (lhs / rhs).
        /// </summary>
        public static dVector2 operator/(dVector2 lhs, double rhs) => new dVector2(lhs.x / rhs, lhs.y / rhs);
        
        /// <summary>
        /// Returns a dVector2 from component-wise application of operator/ (lhs / rhs).
        /// </summary>
        public static dVector2 operator/(double lhs, dVector2 rhs) => new dVector2(lhs / rhs.x, lhs / rhs.y);
        
        /// <summary>
        /// Returns a dVector2 from component-wise application of operator+ (identity).
        /// </summary>
        public static dVector2 operator+(dVector2 v) => v;
        
        /// <summary>
        /// Returns a dVector2 from component-wise application of operator- (-v).
        /// </summary>
        public static dVector2 operator-(dVector2 v) => new dVector2(-v.x, -v.y);
        
        /// <summary>
        /// Returns a dVector2 from component-wise application of operator% (lhs % rhs).
        /// </summary>
        public static dVector2 operator%(dVector2 lhs, dVector2 rhs) => new dVector2(lhs.x % rhs.x, lhs.y % rhs.y);
        
        /// <summary>
        /// Returns a dVector2 from component-wise application of operator% (lhs % rhs).
        /// </summary>
        public static dVector2 operator%(dVector2 lhs, double rhs) => new dVector2(lhs.x % rhs, lhs.y % rhs);
        
        /// <summary>
        /// Returns a dVector2 from component-wise application of operator% (lhs % rhs).
        /// </summary>
        public static dVector2 operator%(double lhs, dVector2 rhs) => new dVector2(lhs % rhs.x, lhs % rhs.y);

        #endregion

    }
}
