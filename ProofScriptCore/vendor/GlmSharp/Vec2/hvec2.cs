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
    /// A Vectortor of type Half with 2 components.
    /// </summary>
    [RegisterCoreClassStruct]
    
    [StructLayout(LayoutKind.Sequential)]
    public struct hVector2 : IReadOnlyList<Half>, IEquatable<hVector2>
    {

        #region Fields
        
        /// <summary>
        /// x-component
        /// </summary>
        
        public Half x;
        
        /// <summary>
        /// y-component
        /// </summary>
        
        public Half y;

        #endregion


        #region Constructors
        
        /// <summary>
        /// Component-wise constructor
        /// </summary>
        public hVector2(Half x, Half y)
        {
            this.x = x;
            this.y = y;
        }
        
        /// <summary>
        /// all-same-value constructor
        /// </summary>
        public hVector2(Half v)
        {
            this.x = v;
            this.y = v;
        }
        
        /// <summary>
        /// from-Vectortor constructor
        /// </summary>
        public hVector2(hVector2 v)
        {
            this.x = v.x;
            this.y = v.y;
        }
        
        /// <summary>
        /// from-Vectortor constructor (additional fields are truncated)
        /// </summary>
        public hVector2(hVector3 v)
        {
            this.x = v.x;
            this.y = v.y;
        }
        
        /// <summary>
        /// from-Vectortor constructor (additional fields are truncated)
        /// </summary>
        public hVector2(hVector4 v)
        {
            this.x = v.x;
            this.y = v.y;
        }
        
        /// <summary>
        /// From-array/list constructor (superfluous values are ignored, missing values are zero-filled).
        /// </summary>
        public hVector2(IReadOnlyList<Half> v)
        {
            var c = v.Count;
            this.x = c < 0 ? Half.Zero : v[0];
            this.y = c < 1 ? Half.Zero : v[1];
        }
        
        /// <summary>
        /// Generic from-array constructor (superfluous values are ignored, missing values are zero-filled).
        /// </summary>
        public hVector2(Object[] v)
        {
            var c = v.Length;
            this.x = c < 0 ? Half.Zero : (Half)v[0];
            this.y = c < 1 ? Half.Zero : (Half)v[1];
        }
        
        /// <summary>
        /// From-array constructor (superfluous values are ignored, missing values are zero-filled).
        /// </summary>
        public hVector2(Half[] v)
        {
            var c = v.Length;
            this.x = c < 0 ? Half.Zero : v[0];
            this.y = c < 1 ? Half.Zero : v[1];
        }
        
        /// <summary>
        /// From-array constructor with base index (superfluous values are ignored, missing values are zero-filled).
        /// </summary>
        public hVector2(Half[] v, int startIndex)
        {
            var c = v.Length;
            this.x = c + startIndex < 0 ? Half.Zero : v[0 + startIndex];
            this.y = c + startIndex < 1 ? Half.Zero : v[1 + startIndex];
        }
        
        /// <summary>
        /// From-IEnumerable constructor (superfluous values are ignored, missing values are zero-filled).
        /// </summary>
        public hVector2(IEnumerable<Half> v)
            : this(v.ToArray())
        {
        }

        #endregion


        #region Implicit Operators
        
        /// <summary>
        /// Implicitly converts this to a Vector2.
        /// </summary>
        public static implicit operator Vector2(hVector2 v) => new Vector2((float)v.x, (float)v.y);
        
        /// <summary>
        /// Implicitly converts this to a dVector2.
        /// </summary>
        public static implicit operator dVector2(hVector2 v) => new dVector2((double)v.x, (double)v.y);
        
        /// <summary>
        /// Implicitly converts this to a cVector2.
        /// </summary>
//public static implicit operator cVector2(hVector2 v) => new cVector2((Complex)v.x, (Complex)v.y);

        #endregion


        #region Explicit Operators
        
        /// <summary>
        /// Explicitly converts this to a iVector2.
        /// </summary>
        public static explicit operator iVector2(hVector2 v) => new iVector2((int)v.x, (int)v.y);
        
        /// <summary>
        /// Explicitly converts this to a iVector3. (Higher components are zeroed)
        /// </summary>
        public static explicit operator iVector3(hVector2 v) => new iVector3((int)v.x, (int)v.y, 0);
        
        /// <summary>
        /// Explicitly converts this to a iVector4. (Higher components are zeroed)
        /// </summary>
        public static explicit operator iVector4(hVector2 v) => new iVector4((int)v.x, (int)v.y, 0, 0);
        
        /// <summary>
        /// Explicitly converts this to a uVector2.
        /// </summary>
        public static explicit operator uVector2(hVector2 v) => new uVector2((uint)v.x, (uint)v.y);
        
        /// <summary>
        /// Explicitly converts this to a uVector3. (Higher components are zeroed)
        /// </summary>
        public static explicit operator uVector3(hVector2 v) => new uVector3((uint)v.x, (uint)v.y, 0u);
        
        /// <summary>
        /// Explicitly converts this to a uVector4. (Higher components are zeroed)
        /// </summary>
        public static explicit operator uVector4(hVector2 v) => new uVector4((uint)v.x, (uint)v.y, 0u, 0u);
        
        /// <summary>
        /// Explicitly converts this to a Vector3. (Higher components are zeroed)
        /// </summary>
        public static explicit operator Vector3(hVector2 v) => new Vector3((float)v.x, (float)v.y, 0f);
        
        /// <summary>
        /// Explicitly converts this to a Vector4. (Higher components are zeroed)
        /// </summary>
        public static explicit operator Vector4(hVector2 v) => new Vector4((float)v.x, (float)v.y, 0f, 0f);
        
        /// <summary>
        /// Explicitly converts this to a hVector3. (Higher components are zeroed)
        /// </summary>
        public static explicit operator hVector3(hVector2 v) => new hVector3((Half)v.x, (Half)v.y, Half.Zero);
        
        /// <summary>
        /// Explicitly converts this to a hVector4. (Higher components are zeroed)
        /// </summary>
        public static explicit operator hVector4(hVector2 v) => new hVector4((Half)v.x, (Half)v.y, Half.Zero, Half.Zero);
        
        /// <summary>
        /// Explicitly converts this to a dVector3. (Higher components are zeroed)
        /// </summary>
        public static explicit operator dVector3(hVector2 v) => new dVector3((double)v.x, (double)v.y, 0.0);
        
        /// <summary>
        /// Explicitly converts this to a dVector4. (Higher components are zeroed)
        /// </summary>
        public static explicit operator dVector4(hVector2 v) => new dVector4((double)v.x, (double)v.y, 0.0, 0.0);
        
        /// <summary>
        /// Explicitly converts this to a decVector2.
        /// </summary>
        public static explicit operator decVector2(hVector2 v) => new decVector2((decimal)v.x, (decimal)v.y);
        
        /// <summary>
        /// Explicitly converts this to a decVector3. (Higher components are zeroed)
        /// </summary>
        public static explicit operator decVector3(hVector2 v) => new decVector3((decimal)v.x, (decimal)v.y, 0m);
        
        /// <summary>
        /// Explicitly converts this to a decVector4. (Higher components are zeroed)
        /// </summary>
        public static explicit operator decVector4(hVector2 v) => new decVector4((decimal)v.x, (decimal)v.y, 0m, 0m);
        
        /// <summary>
        /// Explicitly converts this to a cVector3. (Higher components are zeroed)
        /// </summary>
       // public static explicit operator cVector3(hVector2 v) => new cVector3((Complex)v.x, (Complex)v.y, Complex.Zero);
        
        /// <summary>
        /// Explicitly converts this to a cVector4. (Higher components are zeroed)
        /// </summary>
        //public static explicit operator cVector4(hVector2 v) => new cVector4((Complex)v.x, (Complex)v.y, Complex.Zero, Complex.Zero);
        
        /// <summary>
        /// Explicitly converts this to a lVector2.
        /// </summary>
        public static explicit operator lVector2(hVector2 v) => new lVector2((long)v.x, (long)v.y);
        
        /// <summary>
        /// Explicitly converts this to a lVector3. (Higher components are zeroed)
        /// </summary>
        public static explicit operator lVector3(hVector2 v) => new lVector3((long)v.x, (long)v.y, 0);
        
        /// <summary>
        /// Explicitly converts this to a lVector4. (Higher components are zeroed)
        /// </summary>
        public static explicit operator lVector4(hVector2 v) => new lVector4((long)v.x, (long)v.y, 0, 0);
        
        /// <summary>
        /// Explicitly converts this to a bVector2.
        /// </summary>
        public static explicit operator bVector2(hVector2 v) => new bVector2(v.x != Half.Zero, v.y != Half.Zero);
        
        /// <summary>
        /// Explicitly converts this to a bVector3. (Higher components are zeroed)
        /// </summary>
        public static explicit operator bVector3(hVector2 v) => new bVector3(v.x != Half.Zero, v.y != Half.Zero, false);
        
        /// <summary>
        /// Explicitly converts this to a bVector4. (Higher components are zeroed)
        /// </summary>
        public static explicit operator bVector4(hVector2 v) => new bVector4(v.x != Half.Zero, v.y != Half.Zero, false, false);
        
        /// <summary>
        /// Explicitly converts this to a Half array.
        /// </summary>
        public static explicit operator Half[](hVector2 v) => new [] { v.x, v.y };
        
        /// <summary>
        /// Explicitly converts this to a generic object array.
        /// </summary>
        public static explicit operator Object[](hVector2 v) => new Object[] { v.x, v.y };

        #endregion


        #region Indexer
        
        /// <summary>
        /// Gets/Sets a specific indexed component (a bit slower than direct access).
        /// </summary>
        public Half this[int index]
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
        public swizzle_hVector2 swizzle => new swizzle_hVector2(x, y);
        
        /// <summary>
        /// Gets or sets the specified subset of components. For more advanced (read-only) swizzling, use the .swizzle property.
        /// </summary>
        public hVector2 xy
        {
            get
            {
                return new hVector2(x, y);
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
        public hVector2 rg
        {
            get
            {
                return new hVector2(x, y);
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
        public Half r
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
        public Half g
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
        public Half[] Values => new[] { x, y };
        
        /// <summary>
        /// Returns the number of components (2).
        /// </summary>
        public int Count => 2;
        
        /// <summary>
        /// Returns the minimal component of this Vectortor.
        /// </summary>
        public Half MinElement => Half.Min(x, y);
        
        /// <summary>
        /// Returns the maximal component of this Vectortor.
        /// </summary>
        public Half MaxElement => Half.Max(x, y);
        
        /// <summary>
        /// Returns the euclidean length of this Vectortor.
        /// </summary>
        public float Length => (float)Math.Sqrt((x*x + y*y));
        
        /// <summary>
        /// Returns the squared euclidean length of this Vectortor.
        /// </summary>
        public float LengthSqr => (x*x + y*y);
        
        /// <summary>
        /// Returns the sum of all components.
        /// </summary>
        public Half Sum => (x + y);
        
        /// <summary>
        /// Returns the euclidean norm of this Vectortor.
        /// </summary>
        public float Norm => (float)Math.Sqrt((x*x + y*y));
        
        /// <summary>
        /// Returns the one-norm of this Vectortor.
        /// </summary>
        public float Norm1 => (Half.Abs(x) + Half.Abs(y));
        
        /// <summary>
        /// Returns the two-norm (euclidean length) of this Vectortor.
        /// </summary>
        public float Norm2 => (float)Math.Sqrt((x*x + y*y));
        
        /// <summary>
        /// Returns the max-norm of this Vectortor.
        /// </summary>
        public float NormMax => Half.Max(Half.Abs(x), Half.Abs(y));
        
        /// <summary>
        /// Returns a copy of this Vectortor with length one (undefined if this has zero length).
        /// </summary>
        public hVector2 Normalized => this / (Half)Length;
        
        /// <summary>
        /// Returns a copy of this Vectortor with length one (returns zero if length is zero).
        /// </summary>
        public hVector2 NormalizedSafe => this == Zero ? Zero : this / (Half)Length;
        
        /// <summary>
        /// Returns the Vectortor angle (atan2(y, x)) in radians.
        /// </summary>
        public double Angle => Math.Atan2((double)y, (double)x);

        #endregion


        #region Static Properties
        
        /// <summary>
        /// Predefined all-zero Vectortor
        /// </summary>
        public static hVector2 Zero { get; } = new hVector2(Half.Zero, Half.Zero);
        
        /// <summary>
        /// Predefined all-ones Vectortor
        /// </summary>
        public static hVector2 Ones { get; } = new hVector2(Half.One, Half.One);
        
        /// <summary>
        /// Predefined unit-X Vectortor
        /// </summary>
        public static hVector2 UnitX { get; } = new hVector2(Half.One, Half.Zero);
        
        /// <summary>
        /// Predefined unit-Y Vectortor
        /// </summary>
        public static hVector2 UnitY { get; } = new hVector2(Half.Zero, Half.One);
        
        /// <summary>
        /// Predefined all-MaxValue Vectortor
        /// </summary>
        public static hVector2 MaxValue { get; } = new hVector2(Half.MaxValue, Half.MaxValue);
        
        /// <summary>
        /// Predefined all-MinValue Vectortor
        /// </summary>
        public static hVector2 MinValue { get; } = new hVector2(Half.MinValue, Half.MinValue);
        
        /// <summary>
        /// Predefined all-Epsilon Vectortor
        /// </summary>
        public static hVector2 Epsilon { get; } = new hVector2(Half.Epsilon, Half.Epsilon);
        
        /// <summary>
        /// Predefined all-NaN Vectortor
        /// </summary>
        public static hVector2 NaN { get; } = new hVector2(Half.NaN, Half.NaN);
        
        /// <summary>
        /// Predefined all-NegativeInfinity Vectortor
        /// </summary>
        public static hVector2 NegativeInfinity { get; } = new hVector2(Half.NegativeInfinity, Half.NegativeInfinity);
        
        /// <summary>
        /// Predefined all-PositiveInfinity Vectortor
        /// </summary>
        public static hVector2 PositiveInfinity { get; } = new hVector2(Half.PositiveInfinity, Half.PositiveInfinity);

        #endregion


        #region Operators
        
        /// <summary>
        /// Returns true iff this equals rhs component-wise.
        /// </summary>
        public static bool operator==(hVector2 lhs, hVector2 rhs) => lhs.Equals(rhs);
        
        /// <summary>
        /// Returns true iff this does not equal rhs (component-wise).
        /// </summary>
        public static bool operator!=(hVector2 lhs, hVector2 rhs) => !lhs.Equals(rhs);

        #endregion


        #region Functions
        
        /// <summary>
        /// Returns an enumerator that iterates through all components.
        /// </summary>
        public IEnumerator<Half> GetEnumerator()
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
        public bool Equals(hVector2 rhs) => (x.Equals(rhs.x) && y.Equals(rhs.y));
        
        /// <summary>
        /// Returns true iff this equals rhs type- and component-wise.
        /// </summary>
        public override bool Equals(object obj)
        {
            if (ReferenceEquals(null, obj)) return false;
            return obj is hVector2 && Equals((hVector2) obj);
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
        public double NormP(double p) => Math.Pow((Math.Pow((double)Half.Abs(x), p) + Math.Pow((double)Half.Abs(y), p)), 1 / p);
        
        /// <summary>
        /// Returns a 2D Vectortor that was rotated by a given angle in radians (CAUTION: result is casted and may be truncated).
        /// </summary>
        public hVector2 Rotated(double angleInRad) => (hVector2)(dVector2.FromAngle(Angle + angleInRad) * (double)Length);

        #endregion


        #region Static Functions
        
        /// <summary>
        /// Converts the string representation of the Vectortor into a Vectortor representation (using ', ' as a separator).
        /// </summary>
        public static hVector2 Parse(string s) => Parse(s, ", ");
        
        /// <summary>
        /// Converts the string representation of the Vectortor into a Vectortor representation (using a designated separator).
        /// </summary>
        public static hVector2 Parse(string s, string sep)
        {
            var kvp = s.Split(new[] { sep }, StringSplitOptions.None);
            if (kvp.Length != 2) throw new FormatException("input has not exactly 2 parts");
            return new hVector2(Half.Parse(kvp[0].Trim()), Half.Parse(kvp[1].Trim()));
        }
        
        /// <summary>
        /// Converts the string representation of the Vectortor into a Vectortor representation (using a designated separator and a type provider).
        /// </summary>
        public static hVector2 Parse(string s, string sep, IFormatProvider provider)
        {
            var kvp = s.Split(new[] { sep }, StringSplitOptions.None);
            if (kvp.Length != 2) throw new FormatException("input has not exactly 2 parts");
            return new hVector2(Half.Parse(kvp[0].Trim(), provider), Half.Parse(kvp[1].Trim(), provider));
        }
        
        /// <summary>
        /// Converts the string representation of the Vectortor into a Vectortor representation (using a designated separator and a number style).
        /// </summary>
        public static hVector2 Parse(string s, string sep, NumberStyles style)
        {
            var kvp = s.Split(new[] { sep }, StringSplitOptions.None);
            if (kvp.Length != 2) throw new FormatException("input has not exactly 2 parts");
            return new hVector2(Half.Parse(kvp[0].Trim(), style), Half.Parse(kvp[1].Trim(), style));
        }
        
        /// <summary>
        /// Converts the string representation of the Vectortor into a Vectortor representation (using a designated separator and a number style and a forMatrix provider).
        /// </summary>
        public static hVector2 Parse(string s, string sep, NumberStyles style, IFormatProvider provider)
        {
            var kvp = s.Split(new[] { sep }, StringSplitOptions.None);
            if (kvp.Length != 2) throw new FormatException("input has not exactly 2 parts");
            return new hVector2(Half.Parse(kvp[0].Trim(), style, provider), Half.Parse(kvp[1].Trim(), style, provider));
        }
        
        /// <summary>
        /// Tries to convert the string representation of the Vectortor into a Vectortor representation (using ', ' as a separator), returns false if string was invalid.
        /// </summary>
        public static bool TryParse(string s, out hVector2 result) => TryParse(s, ", ", out result);
        
        /// <summary>
        /// Tries to convert the string representation of the Vectortor into a Vectortor representation (using a designated separator), returns false if string was invalid.
        /// </summary>
        public static bool TryParse(string s, string sep, out hVector2 result)
        {
            result = Zero;
            if (string.IsNullOrEmpty(s)) return false;
            var kvp = s.Split(new[] { sep }, StringSplitOptions.None);
            if (kvp.Length != 2) return false;
            Half x = Half.Zero, y = Half.Zero;
            var ok = (Half.TryParse(kvp[0].Trim(), out x) && Half.TryParse(kvp[1].Trim(), out y));
            result = ok ? new hVector2(x, y) : Zero;
            return ok;
        }
        
        /// <summary>
        /// Tries to convert the string representation of the Vectortor into a Vectortor representation (using a designated separator and a number style and a forMatrix provider), returns false if string was invalid.
        /// </summary>
        public static bool TryParse(string s, string sep, NumberStyles style, IFormatProvider provider, out hVector2 result)
        {
            result = Zero;
            if (string.IsNullOrEmpty(s)) return false;
            var kvp = s.Split(new[] { sep }, StringSplitOptions.None);
            if (kvp.Length != 2) return false;
            Half x = Half.Zero, y = Half.Zero;
            var ok = (Half.TryParse(kvp[0].Trim(), style, provider, out x) && Half.TryParse(kvp[1].Trim(), style, provider, out y));
            result = ok ? new hVector2(x, y) : Zero;
            return ok;
        }
        
        /// <summary>
        /// Returns true iff distance between lhs and rhs is less than or equal to epsilon
        /// </summary>
        public static bool ApproxEqual(hVector2 lhs, hVector2 rhs, float eps = 0.1f) => Distance(lhs, rhs) <= eps;
        
        /// <summary>
        /// OuterProduct treats the first parameter c as a column Vectortor (Matrix with one column) and the second parameter r as a row Vectortor (Matrix with one row) and does a linear algebraic Matrix multiply c * r, yielding a Matrix whose number of rows is the number of components in c and whose number of columns is the number of components in r.
        /// </summary>
        public static hMatrix2 OuterProduct(hVector2 c, hVector2 r) => new hMatrix2(c.x * r.x, c.y * r.x, c.x * r.y, c.y * r.y);
        
        /// <summary>
        /// OuterProduct treats the first parameter c as a column Vectortor (Matrix with one column) and the second parameter r as a row Vectortor (Matrix with one row) and does a linear algebraic Matrix multiply c * r, yielding a Matrix whose number of rows is the number of components in c and whose number of columns is the number of components in r.
        /// </summary>
        public static hMatrix2x3 OuterProduct(hVector3 c, hVector2 r) => new hMatrix2x3(c.x * r.x, c.y * r.x, c.z * r.x, c.x * r.y, c.y * r.y, c.z * r.y);
        
        /// <summary>
        /// OuterProduct treats the first parameter c as a column Vectortor (Matrix with one column) and the second parameter r as a row Vectortor (Matrix with one row) and does a linear algebraic Matrix multiply c * r, yielding a Matrix whose number of rows is the number of components in c and whose number of columns is the number of components in r.
        /// </summary>
        public static hMatrix3x2 OuterProduct(hVector2 c, hVector3 r) => new hMatrix3x2(c.x * r.x, c.y * r.x, c.x * r.y, c.y * r.y, c.x * r.z, c.y * r.z);
        
        /// <summary>
        /// OuterProduct treats the first parameter c as a column Vectortor (Matrix with one column) and the second parameter r as a row Vectortor (Matrix with one row) and does a linear algebraic Matrix multiply c * r, yielding a Matrix whose number of rows is the number of components in c and whose number of columns is the number of components in r.
        /// </summary>
        public static hMatrix2x4 OuterProduct(hVector4 c, hVector2 r) => new hMatrix2x4(c.x * r.x, c.y * r.x, c.z * r.x, c.w * r.x, c.x * r.y, c.y * r.y, c.z * r.y, c.w * r.y);
        
        /// <summary>
        /// OuterProduct treats the first parameter c as a column Vectortor (Matrix with one column) and the second parameter r as a row Vectortor (Matrix with one row) and does a linear algebraic Matrix multiply c * r, yielding a Matrix whose number of rows is the number of components in c and whose number of columns is the number of components in r.
        /// </summary>
        public static hMatrix4x2 OuterProduct(hVector2 c, hVector4 r) => new hMatrix4x2(c.x * r.x, c.y * r.x, c.x * r.y, c.y * r.y, c.x * r.z, c.y * r.z, c.x * r.w, c.y * r.w);
        
        /// <summary>
        /// Returns a unit 2D Vectortor with a given angle in radians (CAUTION: result may be truncated for integer types).
        /// </summary>
        public static hVector2 FromAngle(double angleInRad) => new hVector2((Half)Math.Cos(angleInRad), (Half)Math.Sin(angleInRad));
        
        /// <summary>
        /// Returns the inner product (dot product, scalar product) of the two Vectortors.
        /// </summary>
        public static Half Dot(hVector2 lhs, hVector2 rhs) => (lhs.x * rhs.x + lhs.y * rhs.y);
        
        /// <summary>
        /// Returns the euclidean distance between the two Vectortors.
        /// </summary>
        public static float Distance(hVector2 lhs, hVector2 rhs) => (lhs - rhs).Length;
        
        /// <summary>
        /// Returns the squared euclidean distance between the two Vectortors.
        /// </summary>
        public static float DistanceSqr(hVector2 lhs, hVector2 rhs) => (lhs - rhs).LengthSqr;
        
        /// <summary>
        /// Calculate the reflection direction for an incident Vectortor (N should be normalized in order to achieve the desired result).
        /// </summary>
        public static hVector2 Reflect(hVector2 I, hVector2 N) => I - 2 * Dot(N, I) * N;
        
        /// <summary>
        /// Calculate the refraction direction for an incident Vectortor (The input parameters I and N should be normalized in order to achieve the desired result).
        /// </summary>
        public static hVector2 Refract(hVector2 I, hVector2 N, Half eta)
        {
            var dNI = Dot(N, I);
            var k = 1 - eta * eta * (1 - dNI * dNI);
            if (k < 0) return Zero;
            return eta * I - (eta * dNI + (Half)Math.Sqrt(k)) * N;
        }
        
        /// <summary>
        /// Returns a Vectortor pointing in the same direction as another (faceforward orients a Vectortor to point away from a surface as defined by its normal. If dot(Nref, I) is negative faceforward returns N, otherwise it returns -N).
        /// </summary>
        public static hVector2 FaceForward(hVector2 N, hVector2 I, hVector2 Nref) => Dot(Nref, I) < 0 ? N : -N;
        
        /// <summary>
        /// Returns the length of the outer product (cross product, Vectortor product) of the two Vectortors.
        /// </summary>
        public static Half Cross(hVector2 l, hVector2 r) => l.x * r.y - l.y * r.x;
        
        /// <summary>
        /// Returns a hVector2 with independent and identically distributed uniform values between 0.0 and 1.0.
        /// </summary>
        public static hVector2 Random(System.Random random) => new hVector2((Half)random.NextDouble(), (Half)random.NextDouble());
        
        /// <summary>
        /// Returns a hVector2 with independent and identically distributed uniform values between -1.0 and 1.0.
        /// </summary>
        public static hVector2 RandomSigned(System.Random random) => new hVector2((Half)(random.NextDouble() * 2.0 - 1.0), (Half)(random.NextDouble() * 2.0 - 1.0));
        
        /// <summary>
        /// Returns a hVector2 with independent and identically distributed values according to a normal distribution (zero mean, unit variance).
        /// </summary>
        public static hVector2 RandomNormal(System.Random random) => new hVector2((Half)(Math.Cos(2 * Math.PI * random.NextDouble()) * Math.Sqrt(-2.0 * Math.Log(random.NextDouble()))), (Half)(Math.Cos(2 * Math.PI * random.NextDouble()) * Math.Sqrt(-2.0 * Math.Log(random.NextDouble()))));

        #endregion


        #region Component-Wise Static Functions
        
        /// <summary>
        /// Returns a bVector2 from component-wise application of Equal (lhs == rhs).
        /// </summary>
        public static bVector2 Equal(hVector2 lhs, hVector2 rhs) => new bVector2(lhs.x == rhs.x, lhs.y == rhs.y);
        
        /// <summary>
        /// Returns a bVector2 from component-wise application of Equal (lhs == rhs).
        /// </summary>
        public static bVector2 Equal(hVector2 lhs, Half rhs) => new bVector2(lhs.x == rhs, lhs.y == rhs);
        
        /// <summary>
        /// Returns a bVector2 from component-wise application of Equal (lhs == rhs).
        /// </summary>
        public static bVector2 Equal(Half lhs, hVector2 rhs) => new bVector2(lhs == rhs.x, lhs == rhs.y);
        
        /// <summary>
        /// Returns a bVector from the application of Equal (lhs == rhs).
        /// </summary>
        public static bVector2 Equal(Half lhs, Half rhs) => new bVector2(lhs == rhs);
        
        /// <summary>
        /// Returns a bVector2 from component-wise application of NotEqual (lhs != rhs).
        /// </summary>
        public static bVector2 NotEqual(hVector2 lhs, hVector2 rhs) => new bVector2(lhs.x != rhs.x, lhs.y != rhs.y);
        
        /// <summary>
        /// Returns a bVector2 from component-wise application of NotEqual (lhs != rhs).
        /// </summary>
        public static bVector2 NotEqual(hVector2 lhs, Half rhs) => new bVector2(lhs.x != rhs, lhs.y != rhs);
        
        /// <summary>
        /// Returns a bVector2 from component-wise application of NotEqual (lhs != rhs).
        /// </summary>
        public static bVector2 NotEqual(Half lhs, hVector2 rhs) => new bVector2(lhs != rhs.x, lhs != rhs.y);
        
        /// <summary>
        /// Returns a bVector from the application of NotEqual (lhs != rhs).
        /// </summary>
        public static bVector2 NotEqual(Half lhs, Half rhs) => new bVector2(lhs != rhs);
        
        /// <summary>
        /// Returns a bVector2 from component-wise application of GreaterThan (lhs &gt; rhs).
        /// </summary>
        public static bVector2 GreaterThan(hVector2 lhs, hVector2 rhs) => new bVector2(lhs.x > rhs.x, lhs.y > rhs.y);
        
        /// <summary>
        /// Returns a bVector2 from component-wise application of GreaterThan (lhs &gt; rhs).
        /// </summary>
        public static bVector2 GreaterThan(hVector2 lhs, Half rhs) => new bVector2(lhs.x > rhs, lhs.y > rhs);
        
        /// <summary>
        /// Returns a bVector2 from component-wise application of GreaterThan (lhs &gt; rhs).
        /// </summary>
        public static bVector2 GreaterThan(Half lhs, hVector2 rhs) => new bVector2(lhs > rhs.x, lhs > rhs.y);
        
        /// <summary>
        /// Returns a bVector from the application of GreaterThan (lhs &gt; rhs).
        /// </summary>
        public static bVector2 GreaterThan(Half lhs, Half rhs) => new bVector2(lhs > rhs);
        
        /// <summary>
        /// Returns a bVector2 from component-wise application of GreaterThanEqual (lhs &gt;= rhs).
        /// </summary>
        public static bVector2 GreaterThanEqual(hVector2 lhs, hVector2 rhs) => new bVector2(lhs.x >= rhs.x, lhs.y >= rhs.y);
        
        /// <summary>
        /// Returns a bVector2 from component-wise application of GreaterThanEqual (lhs &gt;= rhs).
        /// </summary>
        public static bVector2 GreaterThanEqual(hVector2 lhs, Half rhs) => new bVector2(lhs.x >= rhs, lhs.y >= rhs);
        
        /// <summary>
        /// Returns a bVector2 from component-wise application of GreaterThanEqual (lhs &gt;= rhs).
        /// </summary>
        public static bVector2 GreaterThanEqual(Half lhs, hVector2 rhs) => new bVector2(lhs >= rhs.x, lhs >= rhs.y);
        
        /// <summary>
        /// Returns a bVector from the application of GreaterThanEqual (lhs &gt;= rhs).
        /// </summary>
        public static bVector2 GreaterThanEqual(Half lhs, Half rhs) => new bVector2(lhs >= rhs);
        
        /// <summary>
        /// Returns a bVector2 from component-wise application of LesserThan (lhs &lt; rhs).
        /// </summary>
        public static bVector2 LesserThan(hVector2 lhs, hVector2 rhs) => new bVector2(lhs.x < rhs.x, lhs.y < rhs.y);
        
        /// <summary>
        /// Returns a bVector2 from component-wise application of LesserThan (lhs &lt; rhs).
        /// </summary>
        public static bVector2 LesserThan(hVector2 lhs, Half rhs) => new bVector2(lhs.x < rhs, lhs.y < rhs);
        
        /// <summary>
        /// Returns a bVector2 from component-wise application of LesserThan (lhs &lt; rhs).
        /// </summary>
        public static bVector2 LesserThan(Half lhs, hVector2 rhs) => new bVector2(lhs < rhs.x, lhs < rhs.y);
        
        /// <summary>
        /// Returns a bVector from the application of LesserThan (lhs &lt; rhs).
        /// </summary>
        public static bVector2 LesserThan(Half lhs, Half rhs) => new bVector2(lhs < rhs);
        
        /// <summary>
        /// Returns a bVector2 from component-wise application of LesserThanEqual (lhs &lt;= rhs).
        /// </summary>
        public static bVector2 LesserThanEqual(hVector2 lhs, hVector2 rhs) => new bVector2(lhs.x <= rhs.x, lhs.y <= rhs.y);
        
        /// <summary>
        /// Returns a bVector2 from component-wise application of LesserThanEqual (lhs &lt;= rhs).
        /// </summary>
        public static bVector2 LesserThanEqual(hVector2 lhs, Half rhs) => new bVector2(lhs.x <= rhs, lhs.y <= rhs);
        
        /// <summary>
        /// Returns a bVector2 from component-wise application of LesserThanEqual (lhs &lt;= rhs).
        /// </summary>
        public static bVector2 LesserThanEqual(Half lhs, hVector2 rhs) => new bVector2(lhs <= rhs.x, lhs <= rhs.y);
        
        /// <summary>
        /// Returns a bVector from the application of LesserThanEqual (lhs &lt;= rhs).
        /// </summary>
        public static bVector2 LesserThanEqual(Half lhs, Half rhs) => new bVector2(lhs <= rhs);
        
        /// <summary>
        /// Returns a bVector2 from component-wise application of IsInfinity (Half.IsInfinity(v)).
        /// </summary>
        public static bVector2 IsInfinity(hVector2 v) => new bVector2(Half.IsInfinity(v.x), Half.IsInfinity(v.y));
        
        /// <summary>
        /// Returns a bVector from the application of IsInfinity (Half.IsInfinity(v)).
        /// </summary>
        public static bVector2 IsInfinity(Half v) => new bVector2(Half.IsInfinity(v));
        
        /// <summary>
        /// Returns a bVector2 from component-wise application of IsFinite (!Half.IsNaN(v) &amp;&amp; !Half.IsInfinity(v)).
        /// </summary>
        public static bVector2 IsFinite(hVector2 v) => new bVector2(!Half.IsNaN(v.x) && !Half.IsInfinity(v.x), !Half.IsNaN(v.y) && !Half.IsInfinity(v.y));
        
        /// <summary>
        /// Returns a bVector from the application of IsFinite (!Half.IsNaN(v) &amp;&amp; !Half.IsInfinity(v)).
        /// </summary>
        public static bVector2 IsFinite(Half v) => new bVector2(!Half.IsNaN(v) && !Half.IsInfinity(v));
        
        /// <summary>
        /// Returns a bVector2 from component-wise application of IsNaN (Half.IsNaN(v)).
        /// </summary>
        public static bVector2 IsNaN(hVector2 v) => new bVector2(Half.IsNaN(v.x), Half.IsNaN(v.y));
        
        /// <summary>
        /// Returns a bVector from the application of IsNaN (Half.IsNaN(v)).
        /// </summary>
        public static bVector2 IsNaN(Half v) => new bVector2(Half.IsNaN(v));
        
        /// <summary>
        /// Returns a bVector2 from component-wise application of IsNegativeInfinity (Half.IsNegativeInfinity(v)).
        /// </summary>
        public static bVector2 IsNegativeInfinity(hVector2 v) => new bVector2(Half.IsNegativeInfinity(v.x), Half.IsNegativeInfinity(v.y));
        
        /// <summary>
        /// Returns a bVector from the application of IsNegativeInfinity (Half.IsNegativeInfinity(v)).
        /// </summary>
        public static bVector2 IsNegativeInfinity(Half v) => new bVector2(Half.IsNegativeInfinity(v));
        
        /// <summary>
        /// Returns a bVector2 from component-wise application of IsPositiveInfinity (Half.IsPositiveInfinity(v)).
        /// </summary>
        public static bVector2 IsPositiveInfinity(hVector2 v) => new bVector2(Half.IsPositiveInfinity(v.x), Half.IsPositiveInfinity(v.y));
        
        /// <summary>
        /// Returns a bVector from the application of IsPositiveInfinity (Half.IsPositiveInfinity(v)).
        /// </summary>
        public static bVector2 IsPositiveInfinity(Half v) => new bVector2(Half.IsPositiveInfinity(v));
        
        /// <summary>
        /// Returns a hVector2 from component-wise application of Abs (Half.Abs(v)).
        /// </summary>
        public static hVector2 Abs(hVector2 v) => new hVector2(Half.Abs(v.x), Half.Abs(v.y));
        
        /// <summary>
        /// Returns a hVector from the application of Abs (Half.Abs(v)).
        /// </summary>
        public static hVector2 Abs(Half v) => new hVector2(Half.Abs(v));
        
        /// <summary>
        /// Returns a hVector2 from component-wise application of HermiteInterpolationOrder3 ((3 - 2 * v) * v * v).
        /// </summary>
        public static hVector2 HermiteInterpolationOrder3(hVector2 v) => new hVector2((3 - 2 * v.x) * v.x * v.x, (3 - 2 * v.y) * v.y * v.y);
        
        /// <summary>
        /// Returns a hVector from the application of HermiteInterpolationOrder3 ((3 - 2 * v) * v * v).
        /// </summary>
        public static hVector2 HermiteInterpolationOrder3(Half v) => new hVector2((3 - 2 * v) * v * v);
        
        /// <summary>
        /// Returns a hVector2 from component-wise application of HermiteInterpolationOrder5 (((6 * v - 15) * v + 10) * v * v * v).
        /// </summary>
        public static hVector2 HermiteInterpolationOrder5(hVector2 v) => new hVector2(((6 * v.x - 15) * v.x + 10) * v.x * v.x * v.x, ((6 * v.y - 15) * v.y + 10) * v.y * v.y * v.y);
        
        /// <summary>
        /// Returns a hVector from the application of HermiteInterpolationOrder5 (((6 * v - 15) * v + 10) * v * v * v).
        /// </summary>
        public static hVector2 HermiteInterpolationOrder5(Half v) => new hVector2(((6 * v - 15) * v + 10) * v * v * v);
        
        /// <summary>
        /// Returns a hVector2 from component-wise application of Sqr (v * v).
        /// </summary>
        public static hVector2 Sqr(hVector2 v) => new hVector2(v.x * v.x, v.y * v.y);
        
        /// <summary>
        /// Returns a hVector from the application of Sqr (v * v).
        /// </summary>
        public static hVector2 Sqr(Half v) => new hVector2(v * v);
        
        /// <summary>
        /// Returns a hVector2 from component-wise application of Pow2 (v * v).
        /// </summary>
        public static hVector2 Pow2(hVector2 v) => new hVector2(v.x * v.x, v.y * v.y);
        
        /// <summary>
        /// Returns a hVector from the application of Pow2 (v * v).
        /// </summary>
        public static hVector2 Pow2(Half v) => new hVector2(v * v);
        
        /// <summary>
        /// Returns a hVector2 from component-wise application of Pow3 (v * v * v).
        /// </summary>
        public static hVector2 Pow3(hVector2 v) => new hVector2(v.x * v.x * v.x, v.y * v.y * v.y);
        
        /// <summary>
        /// Returns a hVector from the application of Pow3 (v * v * v).
        /// </summary>
        public static hVector2 Pow3(Half v) => new hVector2(v * v * v);
        
        /// <summary>
        /// Returns a hVector2 from component-wise application of Step (v &gt;= Half.Zero ? Half.One : Half.Zero).
        /// </summary>
        public static hVector2 Step(hVector2 v) => new hVector2(v.x >= Half.Zero ? Half.One : Half.Zero, v.y >= Half.Zero ? Half.One : Half.Zero);
        
        /// <summary>
        /// Returns a hVector from the application of Step (v &gt;= Half.Zero ? Half.One : Half.Zero).
        /// </summary>
        public static hVector2 Step(Half v) => new hVector2(v >= Half.Zero ? Half.One : Half.Zero);
        
        /// <summary>
        /// Returns a hVector2 from component-wise application of Sqrt ((Half)Math.Sqrt((double)v)).
        /// </summary>
        public static hVector2 Sqrt(hVector2 v) => new hVector2((Half)Math.Sqrt((double)v.x), (Half)Math.Sqrt((double)v.y));
        
        /// <summary>
        /// Returns a hVector from the application of Sqrt ((Half)Math.Sqrt((double)v)).
        /// </summary>
        public static hVector2 Sqrt(Half v) => new hVector2((Half)Math.Sqrt((double)v));
        
        /// <summary>
        /// Returns a hVector2 from component-wise application of InverseSqrt ((Half)(1.0 / Math.Sqrt((double)v))).
        /// </summary>
        public static hVector2 InverseSqrt(hVector2 v) => new hVector2((Half)(1.0 / Math.Sqrt((double)v.x)), (Half)(1.0 / Math.Sqrt((double)v.y)));
        
        /// <summary>
        /// Returns a hVector from the application of InverseSqrt ((Half)(1.0 / Math.Sqrt((double)v))).
        /// </summary>
        public static hVector2 InverseSqrt(Half v) => new hVector2((Half)(1.0 / Math.Sqrt((double)v)));
        
        /// <summary>
        /// Returns a iVector2 from component-wise application of Sign (Math.Sign(v)).
        /// </summary>
        public static iVector2 Sign(hVector2 v) => new iVector2(Math.Sign(v.x), Math.Sign(v.y));
        
        /// <summary>
        /// Returns a iVector from the application of Sign (Math.Sign(v)).
        /// </summary>
        public static iVector2 Sign(Half v) => new iVector2(Math.Sign(v));
        
        /// <summary>
        /// Returns a hVector2 from component-wise application of Max (Half.Max(lhs, rhs)).
        /// </summary>
        public static hVector2 Max(hVector2 lhs, hVector2 rhs) => new hVector2(Half.Max(lhs.x, rhs.x), Half.Max(lhs.y, rhs.y));
        
        /// <summary>
        /// Returns a hVector2 from component-wise application of Max (Half.Max(lhs, rhs)).
        /// </summary>
        public static hVector2 Max(hVector2 lhs, Half rhs) => new hVector2(Half.Max(lhs.x, rhs), Half.Max(lhs.y, rhs));
        
        /// <summary>
        /// Returns a hVector2 from component-wise application of Max (Half.Max(lhs, rhs)).
        /// </summary>
        public static hVector2 Max(Half lhs, hVector2 rhs) => new hVector2(Half.Max(lhs, rhs.x), Half.Max(lhs, rhs.y));
        
        /// <summary>
        /// Returns a hVector from the application of Max (Half.Max(lhs, rhs)).
        /// </summary>
        public static hVector2 Max(Half lhs, Half rhs) => new hVector2(Half.Max(lhs, rhs));
        
        /// <summary>
        /// Returns a hVector2 from component-wise application of Min (Half.Min(lhs, rhs)).
        /// </summary>
        public static hVector2 Min(hVector2 lhs, hVector2 rhs) => new hVector2(Half.Min(lhs.x, rhs.x), Half.Min(lhs.y, rhs.y));
        
        /// <summary>
        /// Returns a hVector2 from component-wise application of Min (Half.Min(lhs, rhs)).
        /// </summary>
        public static hVector2 Min(hVector2 lhs, Half rhs) => new hVector2(Half.Min(lhs.x, rhs), Half.Min(lhs.y, rhs));
        
        /// <summary>
        /// Returns a hVector2 from component-wise application of Min (Half.Min(lhs, rhs)).
        /// </summary>
        public static hVector2 Min(Half lhs, hVector2 rhs) => new hVector2(Half.Min(lhs, rhs.x), Half.Min(lhs, rhs.y));
        
        /// <summary>
        /// Returns a hVector from the application of Min (Half.Min(lhs, rhs)).
        /// </summary>
        public static hVector2 Min(Half lhs, Half rhs) => new hVector2(Half.Min(lhs, rhs));
        
        /// <summary>
        /// Returns a hVector2 from component-wise application of Pow ((Half)Math.Pow((double)lhs, (double)rhs)).
        /// </summary>
        public static hVector2 Pow(hVector2 lhs, hVector2 rhs) => new hVector2((Half)Math.Pow((double)lhs.x, (double)rhs.x), (Half)Math.Pow((double)lhs.y, (double)rhs.y));
        
        /// <summary>
        /// Returns a hVector2 from component-wise application of Pow ((Half)Math.Pow((double)lhs, (double)rhs)).
        /// </summary>
        public static hVector2 Pow(hVector2 lhs, Half rhs) => new hVector2((Half)Math.Pow((double)lhs.x, (double)rhs), (Half)Math.Pow((double)lhs.y, (double)rhs));
        
        /// <summary>
        /// Returns a hVector2 from component-wise application of Pow ((Half)Math.Pow((double)lhs, (double)rhs)).
        /// </summary>
        public static hVector2 Pow(Half lhs, hVector2 rhs) => new hVector2((Half)Math.Pow((double)lhs, (double)rhs.x), (Half)Math.Pow((double)lhs, (double)rhs.y));
        
        /// <summary>
        /// Returns a hVector from the application of Pow ((Half)Math.Pow((double)lhs, (double)rhs)).
        /// </summary>
        public static hVector2 Pow(Half lhs, Half rhs) => new hVector2((Half)Math.Pow((double)lhs, (double)rhs));
        
        /// <summary>
        /// Returns a hVector2 from component-wise application of Log ((Half)Math.Log((double)lhs, (double)rhs)).
        /// </summary>
        public static hVector2 Log(hVector2 lhs, hVector2 rhs) => new hVector2((Half)Math.Log((double)lhs.x, (double)rhs.x), (Half)Math.Log((double)lhs.y, (double)rhs.y));
        
        /// <summary>
        /// Returns a hVector2 from component-wise application of Log ((Half)Math.Log((double)lhs, (double)rhs)).
        /// </summary>
        public static hVector2 Log(hVector2 lhs, Half rhs) => new hVector2((Half)Math.Log((double)lhs.x, (double)rhs), (Half)Math.Log((double)lhs.y, (double)rhs));
        
        /// <summary>
        /// Returns a hVector2 from component-wise application of Log ((Half)Math.Log((double)lhs, (double)rhs)).
        /// </summary>
        public static hVector2 Log(Half lhs, hVector2 rhs) => new hVector2((Half)Math.Log((double)lhs, (double)rhs.x), (Half)Math.Log((double)lhs, (double)rhs.y));
        
        /// <summary>
        /// Returns a hVector from the application of Log ((Half)Math.Log((double)lhs, (double)rhs)).
        /// </summary>
        public static hVector2 Log(Half lhs, Half rhs) => new hVector2((Half)Math.Log((double)lhs, (double)rhs));
        
        /// <summary>
        /// Returns a hVector2 from component-wise application of Clamp (Half.Min(Half.Max(v, min), max)).
        /// </summary>
        public static hVector2 Clamp(hVector2 v, hVector2 min, hVector2 max) => new hVector2(Half.Min(Half.Max(v.x, min.x), max.x), Half.Min(Half.Max(v.y, min.y), max.y));
        
        /// <summary>
        /// Returns a hVector2 from component-wise application of Clamp (Half.Min(Half.Max(v, min), max)).
        /// </summary>
        public static hVector2 Clamp(hVector2 v, hVector2 min, Half max) => new hVector2(Half.Min(Half.Max(v.x, min.x), max), Half.Min(Half.Max(v.y, min.y), max));
        
        /// <summary>
        /// Returns a hVector2 from component-wise application of Clamp (Half.Min(Half.Max(v, min), max)).
        /// </summary>
        public static hVector2 Clamp(hVector2 v, Half min, hVector2 max) => new hVector2(Half.Min(Half.Max(v.x, min), max.x), Half.Min(Half.Max(v.y, min), max.y));
        
        /// <summary>
        /// Returns a hVector2 from component-wise application of Clamp (Half.Min(Half.Max(v, min), max)).
        /// </summary>
        public static hVector2 Clamp(hVector2 v, Half min, Half max) => new hVector2(Half.Min(Half.Max(v.x, min), max), Half.Min(Half.Max(v.y, min), max));
        
        /// <summary>
        /// Returns a hVector2 from component-wise application of Clamp (Half.Min(Half.Max(v, min), max)).
        /// </summary>
        public static hVector2 Clamp(Half v, hVector2 min, hVector2 max) => new hVector2(Half.Min(Half.Max(v, min.x), max.x), Half.Min(Half.Max(v, min.y), max.y));
        
        /// <summary>
        /// Returns a hVector2 from component-wise application of Clamp (Half.Min(Half.Max(v, min), max)).
        /// </summary>
        public static hVector2 Clamp(Half v, hVector2 min, Half max) => new hVector2(Half.Min(Half.Max(v, min.x), max), Half.Min(Half.Max(v, min.y), max));
        
        /// <summary>
        /// Returns a hVector2 from component-wise application of Clamp (Half.Min(Half.Max(v, min), max)).
        /// </summary>
        public static hVector2 Clamp(Half v, Half min, hVector2 max) => new hVector2(Half.Min(Half.Max(v, min), max.x), Half.Min(Half.Max(v, min), max.y));
        
        /// <summary>
        /// Returns a hVector from the application of Clamp (Half.Min(Half.Max(v, min), max)).
        /// </summary>
        public static hVector2 Clamp(Half v, Half min, Half max) => new hVector2(Half.Min(Half.Max(v, min), max));
        
        /// <summary>
        /// Returns a hVector2 from component-wise application of Mix (min * (1-a) + max * a).
        /// </summary>
        public static hVector2 Mix(hVector2 min, hVector2 max, hVector2 a) => new hVector2(min.x * (1-a.x) + max.x * a.x, min.y * (1-a.y) + max.y * a.y);
        
        /// <summary>
        /// Returns a hVector2 from component-wise application of Mix (min * (1-a) + max * a).
        /// </summary>
        public static hVector2 Mix(hVector2 min, hVector2 max, Half a) => new hVector2(min.x * (1-a) + max.x * a, min.y * (1-a) + max.y * a);
        
        /// <summary>
        /// Returns a hVector2 from component-wise application of Mix (min * (1-a) + max * a).
        /// </summary>
        public static hVector2 Mix(hVector2 min, Half max, hVector2 a) => new hVector2(min.x * (1-a.x) + max * a.x, min.y * (1-a.y) + max * a.y);
        
        /// <summary>
        /// Returns a hVector2 from component-wise application of Mix (min * (1-a) + max * a).
        /// </summary>
        public static hVector2 Mix(hVector2 min, Half max, Half a) => new hVector2(min.x * (1-a) + max * a, min.y * (1-a) + max * a);
        
        /// <summary>
        /// Returns a hVector2 from component-wise application of Mix (min * (1-a) + max * a).
        /// </summary>
        public static hVector2 Mix(Half min, hVector2 max, hVector2 a) => new hVector2(min * (1-a.x) + max.x * a.x, min * (1-a.y) + max.y * a.y);
        
        /// <summary>
        /// Returns a hVector2 from component-wise application of Mix (min * (1-a) + max * a).
        /// </summary>
        public static hVector2 Mix(Half min, hVector2 max, Half a) => new hVector2(min * (1-a) + max.x * a, min * (1-a) + max.y * a);
        
        /// <summary>
        /// Returns a hVector2 from component-wise application of Mix (min * (1-a) + max * a).
        /// </summary>
        public static hVector2 Mix(Half min, Half max, hVector2 a) => new hVector2(min * (1-a.x) + max * a.x, min * (1-a.y) + max * a.y);
        
        /// <summary>
        /// Returns a hVector from the application of Mix (min * (1-a) + max * a).
        /// </summary>
        public static hVector2 Mix(Half min, Half max, Half a) => new hVector2(min * (1-a) + max * a);
        
        /// <summary>
        /// Returns a hVector2 from component-wise application of Lerp (min * (1-a) + max * a).
        /// </summary>
        public static hVector2 Lerp(hVector2 min, hVector2 max, hVector2 a) => new hVector2(min.x * (1-a.x) + max.x * a.x, min.y * (1-a.y) + max.y * a.y);
        
        /// <summary>
        /// Returns a hVector2 from component-wise application of Lerp (min * (1-a) + max * a).
        /// </summary>
        public static hVector2 Lerp(hVector2 min, hVector2 max, Half a) => new hVector2(min.x * (1-a) + max.x * a, min.y * (1-a) + max.y * a);
        
        /// <summary>
        /// Returns a hVector2 from component-wise application of Lerp (min * (1-a) + max * a).
        /// </summary>
        public static hVector2 Lerp(hVector2 min, Half max, hVector2 a) => new hVector2(min.x * (1-a.x) + max * a.x, min.y * (1-a.y) + max * a.y);
        
        /// <summary>
        /// Returns a hVector2 from component-wise application of Lerp (min * (1-a) + max * a).
        /// </summary>
        public static hVector2 Lerp(hVector2 min, Half max, Half a) => new hVector2(min.x * (1-a) + max * a, min.y * (1-a) + max * a);
        
        /// <summary>
        /// Returns a hVector2 from component-wise application of Lerp (min * (1-a) + max * a).
        /// </summary>
        public static hVector2 Lerp(Half min, hVector2 max, hVector2 a) => new hVector2(min * (1-a.x) + max.x * a.x, min * (1-a.y) + max.y * a.y);
        
        /// <summary>
        /// Returns a hVector2 from component-wise application of Lerp (min * (1-a) + max * a).
        /// </summary>
        public static hVector2 Lerp(Half min, hVector2 max, Half a) => new hVector2(min * (1-a) + max.x * a, min * (1-a) + max.y * a);
        
        /// <summary>
        /// Returns a hVector2 from component-wise application of Lerp (min * (1-a) + max * a).
        /// </summary>
        public static hVector2 Lerp(Half min, Half max, hVector2 a) => new hVector2(min * (1-a.x) + max * a.x, min * (1-a.y) + max * a.y);
        
        /// <summary>
        /// Returns a hVector from the application of Lerp (min * (1-a) + max * a).
        /// </summary>
        public static hVector2 Lerp(Half min, Half max, Half a) => new hVector2(min * (1-a) + max * a);
        
        /// <summary>
        /// Returns a hVector2 from component-wise application of Smoothstep (((v - edge0) / (edge1 - edge0)).Clamp().HermiteInterpolationOrder3()).
        /// </summary>
        public static hVector2 Smoothstep(hVector2 edge0, hVector2 edge1, hVector2 v) => new hVector2(((v.x - edge0.x) / (edge1.x - edge0.x)).Clamp().HermiteInterpolationOrder3(), ((v.y - edge0.y) / (edge1.y - edge0.y)).Clamp().HermiteInterpolationOrder3());
        
        /// <summary>
        /// Returns a hVector2 from component-wise application of Smoothstep (((v - edge0) / (edge1 - edge0)).Clamp().HermiteInterpolationOrder3()).
        /// </summary>
        public static hVector2 Smoothstep(hVector2 edge0, hVector2 edge1, Half v) => new hVector2(((v - edge0.x) / (edge1.x - edge0.x)).Clamp().HermiteInterpolationOrder3(), ((v - edge0.y) / (edge1.y - edge0.y)).Clamp().HermiteInterpolationOrder3());
        
        /// <summary>
        /// Returns a hVector2 from component-wise application of Smoothstep (((v - edge0) / (edge1 - edge0)).Clamp().HermiteInterpolationOrder3()).
        /// </summary>
        public static hVector2 Smoothstep(hVector2 edge0, Half edge1, hVector2 v) => new hVector2(((v.x - edge0.x) / (edge1 - edge0.x)).Clamp().HermiteInterpolationOrder3(), ((v.y - edge0.y) / (edge1 - edge0.y)).Clamp().HermiteInterpolationOrder3());
        
        /// <summary>
        /// Returns a hVector2 from component-wise application of Smoothstep (((v - edge0) / (edge1 - edge0)).Clamp().HermiteInterpolationOrder3()).
        /// </summary>
        public static hVector2 Smoothstep(hVector2 edge0, Half edge1, Half v) => new hVector2(((v - edge0.x) / (edge1 - edge0.x)).Clamp().HermiteInterpolationOrder3(), ((v - edge0.y) / (edge1 - edge0.y)).Clamp().HermiteInterpolationOrder3());
        
        /// <summary>
        /// Returns a hVector2 from component-wise application of Smoothstep (((v - edge0) / (edge1 - edge0)).Clamp().HermiteInterpolationOrder3()).
        /// </summary>
        public static hVector2 Smoothstep(Half edge0, hVector2 edge1, hVector2 v) => new hVector2(((v.x - edge0) / (edge1.x - edge0)).Clamp().HermiteInterpolationOrder3(), ((v.y - edge0) / (edge1.y - edge0)).Clamp().HermiteInterpolationOrder3());
        
        /// <summary>
        /// Returns a hVector2 from component-wise application of Smoothstep (((v - edge0) / (edge1 - edge0)).Clamp().HermiteInterpolationOrder3()).
        /// </summary>
        public static hVector2 Smoothstep(Half edge0, hVector2 edge1, Half v) => new hVector2(((v - edge0) / (edge1.x - edge0)).Clamp().HermiteInterpolationOrder3(), ((v - edge0) / (edge1.y - edge0)).Clamp().HermiteInterpolationOrder3());
        
        /// <summary>
        /// Returns a hVector2 from component-wise application of Smoothstep (((v - edge0) / (edge1 - edge0)).Clamp().HermiteInterpolationOrder3()).
        /// </summary>
        public static hVector2 Smoothstep(Half edge0, Half edge1, hVector2 v) => new hVector2(((v.x - edge0) / (edge1 - edge0)).Clamp().HermiteInterpolationOrder3(), ((v.y - edge0) / (edge1 - edge0)).Clamp().HermiteInterpolationOrder3());
        
        /// <summary>
        /// Returns a hVector from the application of Smoothstep (((v - edge0) / (edge1 - edge0)).Clamp().HermiteInterpolationOrder3()).
        /// </summary>
        public static hVector2 Smoothstep(Half edge0, Half edge1, Half v) => new hVector2(((v - edge0) / (edge1 - edge0)).Clamp().HermiteInterpolationOrder3());
        
        /// <summary>
        /// Returns a hVector2 from component-wise application of Smootherstep (((v - edge0) / (edge1 - edge0)).Clamp().HermiteInterpolationOrder5()).
        /// </summary>
        public static hVector2 Smootherstep(hVector2 edge0, hVector2 edge1, hVector2 v) => new hVector2(((v.x - edge0.x) / (edge1.x - edge0.x)).Clamp().HermiteInterpolationOrder5(), ((v.y - edge0.y) / (edge1.y - edge0.y)).Clamp().HermiteInterpolationOrder5());
        
        /// <summary>
        /// Returns a hVector2 from component-wise application of Smootherstep (((v - edge0) / (edge1 - edge0)).Clamp().HermiteInterpolationOrder5()).
        /// </summary>
        public static hVector2 Smootherstep(hVector2 edge0, hVector2 edge1, Half v) => new hVector2(((v - edge0.x) / (edge1.x - edge0.x)).Clamp().HermiteInterpolationOrder5(), ((v - edge0.y) / (edge1.y - edge0.y)).Clamp().HermiteInterpolationOrder5());
        
        /// <summary>
        /// Returns a hVector2 from component-wise application of Smootherstep (((v - edge0) / (edge1 - edge0)).Clamp().HermiteInterpolationOrder5()).
        /// </summary>
        public static hVector2 Smootherstep(hVector2 edge0, Half edge1, hVector2 v) => new hVector2(((v.x - edge0.x) / (edge1 - edge0.x)).Clamp().HermiteInterpolationOrder5(), ((v.y - edge0.y) / (edge1 - edge0.y)).Clamp().HermiteInterpolationOrder5());
        
        /// <summary>
        /// Returns a hVector2 from component-wise application of Smootherstep (((v - edge0) / (edge1 - edge0)).Clamp().HermiteInterpolationOrder5()).
        /// </summary>
        public static hVector2 Smootherstep(hVector2 edge0, Half edge1, Half v) => new hVector2(((v - edge0.x) / (edge1 - edge0.x)).Clamp().HermiteInterpolationOrder5(), ((v - edge0.y) / (edge1 - edge0.y)).Clamp().HermiteInterpolationOrder5());
        
        /// <summary>
        /// Returns a hVector2 from component-wise application of Smootherstep (((v - edge0) / (edge1 - edge0)).Clamp().HermiteInterpolationOrder5()).
        /// </summary>
        public static hVector2 Smootherstep(Half edge0, hVector2 edge1, hVector2 v) => new hVector2(((v.x - edge0) / (edge1.x - edge0)).Clamp().HermiteInterpolationOrder5(), ((v.y - edge0) / (edge1.y - edge0)).Clamp().HermiteInterpolationOrder5());
        
        /// <summary>
        /// Returns a hVector2 from component-wise application of Smootherstep (((v - edge0) / (edge1 - edge0)).Clamp().HermiteInterpolationOrder5()).
        /// </summary>
        public static hVector2 Smootherstep(Half edge0, hVector2 edge1, Half v) => new hVector2(((v - edge0) / (edge1.x - edge0)).Clamp().HermiteInterpolationOrder5(), ((v - edge0) / (edge1.y - edge0)).Clamp().HermiteInterpolationOrder5());
        
        /// <summary>
        /// Returns a hVector2 from component-wise application of Smootherstep (((v - edge0) / (edge1 - edge0)).Clamp().HermiteInterpolationOrder5()).
        /// </summary>
        public static hVector2 Smootherstep(Half edge0, Half edge1, hVector2 v) => new hVector2(((v.x - edge0) / (edge1 - edge0)).Clamp().HermiteInterpolationOrder5(), ((v.y - edge0) / (edge1 - edge0)).Clamp().HermiteInterpolationOrder5());
        
        /// <summary>
        /// Returns a hVector from the application of Smootherstep (((v - edge0) / (edge1 - edge0)).Clamp().HermiteInterpolationOrder5()).
        /// </summary>
        public static hVector2 Smootherstep(Half edge0, Half edge1, Half v) => new hVector2(((v - edge0) / (edge1 - edge0)).Clamp().HermiteInterpolationOrder5());
        
        /// <summary>
        /// Returns a hVector2 from component-wise application of Fma (a * b + c).
        /// </summary>
        public static hVector2 Fma(hVector2 a, hVector2 b, hVector2 c) => new hVector2(a.x * b.x + c.x, a.y * b.y + c.y);
        
        /// <summary>
        /// Returns a hVector2 from component-wise application of Fma (a * b + c).
        /// </summary>
        public static hVector2 Fma(hVector2 a, hVector2 b, Half c) => new hVector2(a.x * b.x + c, a.y * b.y + c);
        
        /// <summary>
        /// Returns a hVector2 from component-wise application of Fma (a * b + c).
        /// </summary>
        public static hVector2 Fma(hVector2 a, Half b, hVector2 c) => new hVector2(a.x * b + c.x, a.y * b + c.y);
        
        /// <summary>
        /// Returns a hVector2 from component-wise application of Fma (a * b + c).
        /// </summary>
        public static hVector2 Fma(hVector2 a, Half b, Half c) => new hVector2(a.x * b + c, a.y * b + c);
        
        /// <summary>
        /// Returns a hVector2 from component-wise application of Fma (a * b + c).
        /// </summary>
        public static hVector2 Fma(Half a, hVector2 b, hVector2 c) => new hVector2(a * b.x + c.x, a * b.y + c.y);
        
        /// <summary>
        /// Returns a hVector2 from component-wise application of Fma (a * b + c).
        /// </summary>
        public static hVector2 Fma(Half a, hVector2 b, Half c) => new hVector2(a * b.x + c, a * b.y + c);
        
        /// <summary>
        /// Returns a hVector2 from component-wise application of Fma (a * b + c).
        /// </summary>
        public static hVector2 Fma(Half a, Half b, hVector2 c) => new hVector2(a * b + c.x, a * b + c.y);
        
        /// <summary>
        /// Returns a hVector from the application of Fma (a * b + c).
        /// </summary>
        public static hVector2 Fma(Half a, Half b, Half c) => new hVector2(a * b + c);
        
        /// <summary>
        /// Returns a hVector2 from component-wise application of Add (lhs + rhs).
        /// </summary>
        public static hVector2 Add(hVector2 lhs, hVector2 rhs) => new hVector2(lhs.x + rhs.x, lhs.y + rhs.y);
        
        /// <summary>
        /// Returns a hVector2 from component-wise application of Add (lhs + rhs).
        /// </summary>
        public static hVector2 Add(hVector2 lhs, Half rhs) => new hVector2(lhs.x + rhs, lhs.y + rhs);
        
        /// <summary>
        /// Returns a hVector2 from component-wise application of Add (lhs + rhs).
        /// </summary>
        public static hVector2 Add(Half lhs, hVector2 rhs) => new hVector2(lhs + rhs.x, lhs + rhs.y);
        
        /// <summary>
        /// Returns a hVector from the application of Add (lhs + rhs).
        /// </summary>
        public static hVector2 Add(Half lhs, Half rhs) => new hVector2(lhs + rhs);
        
        /// <summary>
        /// Returns a hVector2 from component-wise application of Sub (lhs - rhs).
        /// </summary>
        public static hVector2 Sub(hVector2 lhs, hVector2 rhs) => new hVector2(lhs.x - rhs.x, lhs.y - rhs.y);
        
        /// <summary>
        /// Returns a hVector2 from component-wise application of Sub (lhs - rhs).
        /// </summary>
        public static hVector2 Sub(hVector2 lhs, Half rhs) => new hVector2(lhs.x - rhs, lhs.y - rhs);
        
        /// <summary>
        /// Returns a hVector2 from component-wise application of Sub (lhs - rhs).
        /// </summary>
        public static hVector2 Sub(Half lhs, hVector2 rhs) => new hVector2(lhs - rhs.x, lhs - rhs.y);
        
        /// <summary>
        /// Returns a hVector from the application of Sub (lhs - rhs).
        /// </summary>
        public static hVector2 Sub(Half lhs, Half rhs) => new hVector2(lhs - rhs);
        
        /// <summary>
        /// Returns a hVector2 from component-wise application of Mul (lhs * rhs).
        /// </summary>
        public static hVector2 Mul(hVector2 lhs, hVector2 rhs) => new hVector2(lhs.x * rhs.x, lhs.y * rhs.y);
        
        /// <summary>
        /// Returns a hVector2 from component-wise application of Mul (lhs * rhs).
        /// </summary>
        public static hVector2 Mul(hVector2 lhs, Half rhs) => new hVector2(lhs.x * rhs, lhs.y * rhs);
        
        /// <summary>
        /// Returns a hVector2 from component-wise application of Mul (lhs * rhs).
        /// </summary>
        public static hVector2 Mul(Half lhs, hVector2 rhs) => new hVector2(lhs * rhs.x, lhs * rhs.y);
        
        /// <summary>
        /// Returns a hVector from the application of Mul (lhs * rhs).
        /// </summary>
        public static hVector2 Mul(Half lhs, Half rhs) => new hVector2(lhs * rhs);
        
        /// <summary>
        /// Returns a hVector2 from component-wise application of Div (lhs / rhs).
        /// </summary>
        public static hVector2 Div(hVector2 lhs, hVector2 rhs) => new hVector2(lhs.x / rhs.x, lhs.y / rhs.y);
        
        /// <summary>
        /// Returns a hVector2 from component-wise application of Div (lhs / rhs).
        /// </summary>
        public static hVector2 Div(hVector2 lhs, Half rhs) => new hVector2(lhs.x / rhs, lhs.y / rhs);
        
        /// <summary>
        /// Returns a hVector2 from component-wise application of Div (lhs / rhs).
        /// </summary>
        public static hVector2 Div(Half lhs, hVector2 rhs) => new hVector2(lhs / rhs.x, lhs / rhs.y);
        
        /// <summary>
        /// Returns a hVector from the application of Div (lhs / rhs).
        /// </summary>
        public static hVector2 Div(Half lhs, Half rhs) => new hVector2(lhs / rhs);
        
        /// <summary>
        /// Returns a hVector2 from component-wise application of Modulo (lhs % rhs).
        /// </summary>
        public static hVector2 Modulo(hVector2 lhs, hVector2 rhs) => new hVector2(lhs.x % rhs.x, lhs.y % rhs.y);
        
        /// <summary>
        /// Returns a hVector2 from component-wise application of Modulo (lhs % rhs).
        /// </summary>
        public static hVector2 Modulo(hVector2 lhs, Half rhs) => new hVector2(lhs.x % rhs, lhs.y % rhs);
        
        /// <summary>
        /// Returns a hVector2 from component-wise application of Modulo (lhs % rhs).
        /// </summary>
        public static hVector2 Modulo(Half lhs, hVector2 rhs) => new hVector2(lhs % rhs.x, lhs % rhs.y);
        
        /// <summary>
        /// Returns a hVector from the application of Modulo (lhs % rhs).
        /// </summary>
        public static hVector2 Modulo(Half lhs, Half rhs) => new hVector2(lhs % rhs);
        
        /// <summary>
        /// Returns a hVector2 from component-wise application of Degrees (Radians-To-Degrees Conversion).
        /// </summary>
        public static hVector2 Degrees(hVector2 v) => new hVector2((Half)(v.x * new Half(57.295779513082320876798154814105170332405472466564321)), (Half)(v.y * new Half(57.295779513082320876798154814105170332405472466564321)));
        
        /// <summary>
        /// Returns a hVector from the application of Degrees (Radians-To-Degrees Conversion).
        /// </summary>
        public static hVector2 Degrees(Half v) => new hVector2((Half)(v * new Half(57.295779513082320876798154814105170332405472466564321)));
        
        /// <summary>
        /// Returns a hVector2 from component-wise application of Radians (Degrees-To-Radians Conversion).
        /// </summary>
        public static hVector2 Radians(hVector2 v) => new hVector2((Half)(v.x * new Half(0.0174532925199432957692369076848861271344287188854172)), (Half)(v.y * new Half(0.0174532925199432957692369076848861271344287188854172)));
        
        /// <summary>
        /// Returns a hVector from the application of Radians (Degrees-To-Radians Conversion).
        /// </summary>
        public static hVector2 Radians(Half v) => new hVector2((Half)(v * new Half(0.0174532925199432957692369076848861271344287188854172)));
        
        /// <summary>
        /// Returns a hVector2 from component-wise application of Acos ((Half)Math.Acos((double)v)).
        /// </summary>
        public static hVector2 Acos(hVector2 v) => new hVector2((Half)Math.Acos((double)v.x), (Half)Math.Acos((double)v.y));
        
        /// <summary>
        /// Returns a hVector from the application of Acos ((Half)Math.Acos((double)v)).
        /// </summary>
        public static hVector2 Acos(Half v) => new hVector2((Half)Math.Acos((double)v));
        
        /// <summary>
        /// Returns a hVector2 from component-wise application of Asin ((Half)Math.Asin((double)v)).
        /// </summary>
        public static hVector2 Asin(hVector2 v) => new hVector2((Half)Math.Asin((double)v.x), (Half)Math.Asin((double)v.y));
        
        /// <summary>
        /// Returns a hVector from the application of Asin ((Half)Math.Asin((double)v)).
        /// </summary>
        public static hVector2 Asin(Half v) => new hVector2((Half)Math.Asin((double)v));
        
        /// <summary>
        /// Returns a hVector2 from component-wise application of Atan ((Half)Math.Atan((double)v)).
        /// </summary>
        public static hVector2 Atan(hVector2 v) => new hVector2((Half)Math.Atan((double)v.x), (Half)Math.Atan((double)v.y));
        
        /// <summary>
        /// Returns a hVector from the application of Atan ((Half)Math.Atan((double)v)).
        /// </summary>
        public static hVector2 Atan(Half v) => new hVector2((Half)Math.Atan((double)v));
        
        /// <summary>
        /// Returns a hVector2 from component-wise application of Cos ((Half)Math.Cos((double)v)).
        /// </summary>
        public static hVector2 Cos(hVector2 v) => new hVector2((Half)Math.Cos((double)v.x), (Half)Math.Cos((double)v.y));
        
        /// <summary>
        /// Returns a hVector from the application of Cos ((Half)Math.Cos((double)v)).
        /// </summary>
        public static hVector2 Cos(Half v) => new hVector2((Half)Math.Cos((double)v));
        
        /// <summary>
        /// Returns a hVector2 from component-wise application of Cosh ((Half)Math.Cosh((double)v)).
        /// </summary>
        public static hVector2 Cosh(hVector2 v) => new hVector2((Half)Math.Cosh((double)v.x), (Half)Math.Cosh((double)v.y));
        
        /// <summary>
        /// Returns a hVector from the application of Cosh ((Half)Math.Cosh((double)v)).
        /// </summary>
        public static hVector2 Cosh(Half v) => new hVector2((Half)Math.Cosh((double)v));
        
        /// <summary>
        /// Returns a hVector2 from component-wise application of Exp ((Half)Math.Exp((double)v)).
        /// </summary>
        public static hVector2 Exp(hVector2 v) => new hVector2((Half)Math.Exp((double)v.x), (Half)Math.Exp((double)v.y));
        
        /// <summary>
        /// Returns a hVector from the application of Exp ((Half)Math.Exp((double)v)).
        /// </summary>
        public static hVector2 Exp(Half v) => new hVector2((Half)Math.Exp((double)v));
        
        /// <summary>
        /// Returns a hVector2 from component-wise application of Log ((Half)Math.Log((double)v)).
        /// </summary>
        public static hVector2 Log(hVector2 v) => new hVector2((Half)Math.Log((double)v.x), (Half)Math.Log((double)v.y));
        
        /// <summary>
        /// Returns a hVector from the application of Log ((Half)Math.Log((double)v)).
        /// </summary>
        public static hVector2 Log(Half v) => new hVector2((Half)Math.Log((double)v));
        
        /// <summary>
        /// Returns a hVector2 from component-wise application of Log2 ((Half)Math.Log((double)v, 2)).
        /// </summary>
        public static hVector2 Log2(hVector2 v) => new hVector2((Half)Math.Log((double)v.x, 2), (Half)Math.Log((double)v.y, 2));
        
        /// <summary>
        /// Returns a hVector from the application of Log2 ((Half)Math.Log((double)v, 2)).
        /// </summary>
        public static hVector2 Log2(Half v) => new hVector2((Half)Math.Log((double)v, 2));
        
        /// <summary>
        /// Returns a hVector2 from component-wise application of Log10 ((Half)Math.Log10((double)v)).
        /// </summary>
        public static hVector2 Log10(hVector2 v) => new hVector2((Half)Math.Log10((double)v.x), (Half)Math.Log10((double)v.y));
        
        /// <summary>
        /// Returns a hVector from the application of Log10 ((Half)Math.Log10((double)v)).
        /// </summary>
        public static hVector2 Log10(Half v) => new hVector2((Half)Math.Log10((double)v));
        
        /// <summary>
        /// Returns a hVector2 from component-wise application of Floor ((Half)Math.Floor(v)).
        /// </summary>
        public static hVector2 Floor(hVector2 v) => new hVector2((Half)Math.Floor(v.x), (Half)Math.Floor(v.y));
        
        /// <summary>
        /// Returns a hVector from the application of Floor ((Half)Math.Floor(v)).
        /// </summary>
        public static hVector2 Floor(Half v) => new hVector2((Half)Math.Floor(v));
        
        /// <summary>
        /// Returns a hVector2 from component-wise application of Ceiling ((Half)Math.Ceiling(v)).
        /// </summary>
        public static hVector2 Ceiling(hVector2 v) => new hVector2((Half)Math.Ceiling(v.x), (Half)Math.Ceiling(v.y));
        
        /// <summary>
        /// Returns a hVector from the application of Ceiling ((Half)Math.Ceiling(v)).
        /// </summary>
        public static hVector2 Ceiling(Half v) => new hVector2((Half)Math.Ceiling(v));
        
        /// <summary>
        /// Returns a hVector2 from component-wise application of Round ((Half)Math.Round(v)).
        /// </summary>
        public static hVector2 Round(hVector2 v) => new hVector2((Half)Math.Round(v.x), (Half)Math.Round(v.y));
        
        /// <summary>
        /// Returns a hVector from the application of Round ((Half)Math.Round(v)).
        /// </summary>
        public static hVector2 Round(Half v) => new hVector2((Half)Math.Round(v));
        
        /// <summary>
        /// Returns a hVector2 from component-wise application of Sin ((Half)Math.Sin((double)v)).
        /// </summary>
        public static hVector2 Sin(hVector2 v) => new hVector2((Half)Math.Sin((double)v.x), (Half)Math.Sin((double)v.y));
        
        /// <summary>
        /// Returns a hVector from the application of Sin ((Half)Math.Sin((double)v)).
        /// </summary>
        public static hVector2 Sin(Half v) => new hVector2((Half)Math.Sin((double)v));
        
        /// <summary>
        /// Returns a hVector2 from component-wise application of Sinh ((Half)Math.Sinh((double)v)).
        /// </summary>
        public static hVector2 Sinh(hVector2 v) => new hVector2((Half)Math.Sinh((double)v.x), (Half)Math.Sinh((double)v.y));
        
        /// <summary>
        /// Returns a hVector from the application of Sinh ((Half)Math.Sinh((double)v)).
        /// </summary>
        public static hVector2 Sinh(Half v) => new hVector2((Half)Math.Sinh((double)v));
        
        /// <summary>
        /// Returns a hVector2 from component-wise application of Tan ((Half)Math.Tan((double)v)).
        /// </summary>
        public static hVector2 Tan(hVector2 v) => new hVector2((Half)Math.Tan((double)v.x), (Half)Math.Tan((double)v.y));
        
        /// <summary>
        /// Returns a hVector from the application of Tan ((Half)Math.Tan((double)v)).
        /// </summary>
        public static hVector2 Tan(Half v) => new hVector2((Half)Math.Tan((double)v));
        
        /// <summary>
        /// Returns a hVector2 from component-wise application of Tanh ((Half)Math.Tanh((double)v)).
        /// </summary>
        public static hVector2 Tanh(hVector2 v) => new hVector2((Half)Math.Tanh((double)v.x), (Half)Math.Tanh((double)v.y));
        
        /// <summary>
        /// Returns a hVector from the application of Tanh ((Half)Math.Tanh((double)v)).
        /// </summary>
        public static hVector2 Tanh(Half v) => new hVector2((Half)Math.Tanh((double)v));
        
        /// <summary>
        /// Returns a hVector2 from component-wise application of Truncate ((Half)Math.Truncate((double)v)).
        /// </summary>
        public static hVector2 Truncate(hVector2 v) => new hVector2((Half)Math.Truncate((double)v.x), (Half)Math.Truncate((double)v.y));
        
        /// <summary>
        /// Returns a hVector from the application of Truncate ((Half)Math.Truncate((double)v)).
        /// </summary>
        public static hVector2 Truncate(Half v) => new hVector2((Half)Math.Truncate((double)v));
        
        /// <summary>
        /// Returns a hVector2 from component-wise application of Fract ((Half)(v - Math.Floor(v))).
        /// </summary>
        public static hVector2 Fract(hVector2 v) => new hVector2((Half)(v.x - Math.Floor(v.x)), (Half)(v.y - Math.Floor(v.y)));
        
        /// <summary>
        /// Returns a hVector from the application of Fract ((Half)(v - Math.Floor(v))).
        /// </summary>
        public static hVector2 Fract(Half v) => new hVector2((Half)(v - Math.Floor(v)));
        
        /// <summary>
        /// Returns a hVector2 from component-wise application of Trunc ((long)(v)).
        /// </summary>
        public static hVector2 Trunc(hVector2 v) => new hVector2((long)(v.x), (long)(v.y));
        
        /// <summary>
        /// Returns a hVector from the application of Trunc ((long)(v)).
        /// </summary>
        public static hVector2 Trunc(Half v) => new hVector2((long)(v));
        
        /// <summary>
        /// Returns a hVector2 with independent and identically distributed uniform values between 'minValue' and 'maxValue'.
        /// </summary>
        public static hVector2 Random(System.Random random, hVector2 minValue, hVector2 maxValue) => new hVector2((Half)random.NextDouble() * (maxValue.x - minValue.x) + minValue.x, (Half)random.NextDouble() * (maxValue.y - minValue.y) + minValue.y);
        
        /// <summary>
        /// Returns a hVector2 with independent and identically distributed uniform values between 'minValue' and 'maxValue'.
        /// </summary>
        public static hVector2 Random(System.Random random, hVector2 minValue, Half maxValue) => new hVector2((Half)random.NextDouble() * (maxValue - minValue.x) + minValue.x, (Half)random.NextDouble() * (maxValue - minValue.y) + minValue.y);
        
        /// <summary>
        /// Returns a hVector2 with independent and identically distributed uniform values between 'minValue' and 'maxValue'.
        /// </summary>
        public static hVector2 Random(System.Random random, Half minValue, hVector2 maxValue) => new hVector2((Half)random.NextDouble() * (maxValue.x - minValue) + minValue, (Half)random.NextDouble() * (maxValue.y - minValue) + minValue);
        
        /// <summary>
        /// Returns a hVector2 with independent and identically distributed uniform values between 'minValue' and 'maxValue'.
        /// </summary>
        public static hVector2 Random(System.Random random, Half minValue, Half maxValue) => new hVector2((Half)random.NextDouble() * (maxValue - minValue) + minValue);
        
        /// <summary>
        /// Returns a hVector2 with independent and identically distributed uniform values between 'minValue' and 'maxValue'.
        /// </summary>
        public static hVector2 RandomUniform(System.Random random, hVector2 minValue, hVector2 maxValue) => new hVector2((Half)random.NextDouble() * (maxValue.x - minValue.x) + minValue.x, (Half)random.NextDouble() * (maxValue.y - minValue.y) + minValue.y);
        
        /// <summary>
        /// Returns a hVector2 with independent and identically distributed uniform values between 'minValue' and 'maxValue'.
        /// </summary>
        public static hVector2 RandomUniform(System.Random random, hVector2 minValue, Half maxValue) => new hVector2((Half)random.NextDouble() * (maxValue - minValue.x) + minValue.x, (Half)random.NextDouble() * (maxValue - minValue.y) + minValue.y);
        
        /// <summary>
        /// Returns a hVector2 with independent and identically distributed uniform values between 'minValue' and 'maxValue'.
        /// </summary>
        public static hVector2 RandomUniform(System.Random random, Half minValue, hVector2 maxValue) => new hVector2((Half)random.NextDouble() * (maxValue.x - minValue) + minValue, (Half)random.NextDouble() * (maxValue.y - minValue) + minValue);
        
        /// <summary>
        /// Returns a hVector2 with independent and identically distributed uniform values between 'minValue' and 'maxValue'.
        /// </summary>
        public static hVector2 RandomUniform(System.Random random, Half minValue, Half maxValue) => new hVector2((Half)random.NextDouble() * (maxValue - minValue) + minValue);
        
        /// <summary>
        /// Returns a hVector2 with independent and identically distributed values according to a normal/Gaussian distribution with specified mean and variance.
        /// </summary>
        public static hVector2 RandomNormal(System.Random random, hVector2 mean, hVector2 variance) => new hVector2((Half)(Math.Sqrt((double)variance.x) * Math.Cos(2 * Math.PI * random.NextDouble()) * Math.Sqrt(-2.0 * Math.Log(random.NextDouble()))) + mean.x, (Half)(Math.Sqrt((double)variance.y) * Math.Cos(2 * Math.PI * random.NextDouble()) * Math.Sqrt(-2.0 * Math.Log(random.NextDouble()))) + mean.y);
        
        /// <summary>
        /// Returns a hVector2 with independent and identically distributed values according to a normal/Gaussian distribution with specified mean and variance.
        /// </summary>
        public static hVector2 RandomNormal(System.Random random, hVector2 mean, Half variance) => new hVector2((Half)(Math.Sqrt((double)variance) * Math.Cos(2 * Math.PI * random.NextDouble()) * Math.Sqrt(-2.0 * Math.Log(random.NextDouble()))) + mean.x, (Half)(Math.Sqrt((double)variance) * Math.Cos(2 * Math.PI * random.NextDouble()) * Math.Sqrt(-2.0 * Math.Log(random.NextDouble()))) + mean.y);
        
        /// <summary>
        /// Returns a hVector2 with independent and identically distributed values according to a normal/Gaussian distribution with specified mean and variance.
        /// </summary>
        public static hVector2 RandomNormal(System.Random random, Half mean, hVector2 variance) => new hVector2((Half)(Math.Sqrt((double)variance.x) * Math.Cos(2 * Math.PI * random.NextDouble()) * Math.Sqrt(-2.0 * Math.Log(random.NextDouble()))) + mean, (Half)(Math.Sqrt((double)variance.y) * Math.Cos(2 * Math.PI * random.NextDouble()) * Math.Sqrt(-2.0 * Math.Log(random.NextDouble()))) + mean);
        
        /// <summary>
        /// Returns a hVector2 with independent and identically distributed values according to a normal/Gaussian distribution with specified mean and variance.
        /// </summary>
        public static hVector2 RandomNormal(System.Random random, Half mean, Half variance) => new hVector2((Half)(Math.Sqrt((double)variance) * Math.Cos(2 * Math.PI * random.NextDouble()) * Math.Sqrt(-2.0 * Math.Log(random.NextDouble()))) + mean);
        
        /// <summary>
        /// Returns a hVector2 with independent and identically distributed values according to a normal/Gaussian distribution with specified mean and variance.
        /// </summary>
        public static hVector2 RandomGaussian(System.Random random, hVector2 mean, hVector2 variance) => new hVector2((Half)(Math.Sqrt((double)variance.x) * Math.Cos(2 * Math.PI * random.NextDouble()) * Math.Sqrt(-2.0 * Math.Log(random.NextDouble()))) + mean.x, (Half)(Math.Sqrt((double)variance.y) * Math.Cos(2 * Math.PI * random.NextDouble()) * Math.Sqrt(-2.0 * Math.Log(random.NextDouble()))) + mean.y);
        
        /// <summary>
        /// Returns a hVector2 with independent and identically distributed values according to a normal/Gaussian distribution with specified mean and variance.
        /// </summary>
        public static hVector2 RandomGaussian(System.Random random, hVector2 mean, Half variance) => new hVector2((Half)(Math.Sqrt((double)variance) * Math.Cos(2 * Math.PI * random.NextDouble()) * Math.Sqrt(-2.0 * Math.Log(random.NextDouble()))) + mean.x, (Half)(Math.Sqrt((double)variance) * Math.Cos(2 * Math.PI * random.NextDouble()) * Math.Sqrt(-2.0 * Math.Log(random.NextDouble()))) + mean.y);
        
        /// <summary>
        /// Returns a hVector2 with independent and identically distributed values according to a normal/Gaussian distribution with specified mean and variance.
        /// </summary>
        public static hVector2 RandomGaussian(System.Random random, Half mean, hVector2 variance) => new hVector2((Half)(Math.Sqrt((double)variance.x) * Math.Cos(2 * Math.PI * random.NextDouble()) * Math.Sqrt(-2.0 * Math.Log(random.NextDouble()))) + mean, (Half)(Math.Sqrt((double)variance.y) * Math.Cos(2 * Math.PI * random.NextDouble()) * Math.Sqrt(-2.0 * Math.Log(random.NextDouble()))) + mean);
        
        /// <summary>
        /// Returns a hVector2 with independent and identically distributed values according to a normal/Gaussian distribution with specified mean and variance.
        /// </summary>
        public static hVector2 RandomGaussian(System.Random random, Half mean, Half variance) => new hVector2((Half)(Math.Sqrt((double)variance) * Math.Cos(2 * Math.PI * random.NextDouble()) * Math.Sqrt(-2.0 * Math.Log(random.NextDouble()))) + mean);

        #endregion


        #region Component-Wise Operator Overloads
        
        /// <summary>
        /// Returns a bVector2 from component-wise application of operator&lt; (lhs &lt; rhs).
        /// </summary>
        public static bVector2 operator<(hVector2 lhs, hVector2 rhs) => new bVector2(lhs.x < rhs.x, lhs.y < rhs.y);
        
        /// <summary>
        /// Returns a bVector2 from component-wise application of operator&lt; (lhs &lt; rhs).
        /// </summary>
        public static bVector2 operator<(hVector2 lhs, Half rhs) => new bVector2(lhs.x < rhs, lhs.y < rhs);
        
        /// <summary>
        /// Returns a bVector2 from component-wise application of operator&lt; (lhs &lt; rhs).
        /// </summary>
        public static bVector2 operator<(Half lhs, hVector2 rhs) => new bVector2(lhs < rhs.x, lhs < rhs.y);
        
        /// <summary>
        /// Returns a bVector2 from component-wise application of operator&lt;= (lhs &lt;= rhs).
        /// </summary>
        public static bVector2 operator<=(hVector2 lhs, hVector2 rhs) => new bVector2(lhs.x <= rhs.x, lhs.y <= rhs.y);
        
        /// <summary>
        /// Returns a bVector2 from component-wise application of operator&lt;= (lhs &lt;= rhs).
        /// </summary>
        public static bVector2 operator<=(hVector2 lhs, Half rhs) => new bVector2(lhs.x <= rhs, lhs.y <= rhs);
        
        /// <summary>
        /// Returns a bVector2 from component-wise application of operator&lt;= (lhs &lt;= rhs).
        /// </summary>
        public static bVector2 operator<=(Half lhs, hVector2 rhs) => new bVector2(lhs <= rhs.x, lhs <= rhs.y);
        
        /// <summary>
        /// Returns a bVector2 from component-wise application of operator&gt; (lhs &gt; rhs).
        /// </summary>
        public static bVector2 operator>(hVector2 lhs, hVector2 rhs) => new bVector2(lhs.x > rhs.x, lhs.y > rhs.y);
        
        /// <summary>
        /// Returns a bVector2 from component-wise application of operator&gt; (lhs &gt; rhs).
        /// </summary>
        public static bVector2 operator>(hVector2 lhs, Half rhs) => new bVector2(lhs.x > rhs, lhs.y > rhs);
        
        /// <summary>
        /// Returns a bVector2 from component-wise application of operator&gt; (lhs &gt; rhs).
        /// </summary>
        public static bVector2 operator>(Half lhs, hVector2 rhs) => new bVector2(lhs > rhs.x, lhs > rhs.y);
        
        /// <summary>
        /// Returns a bVector2 from component-wise application of operator&gt;= (lhs &gt;= rhs).
        /// </summary>
        public static bVector2 operator>=(hVector2 lhs, hVector2 rhs) => new bVector2(lhs.x >= rhs.x, lhs.y >= rhs.y);
        
        /// <summary>
        /// Returns a bVector2 from component-wise application of operator&gt;= (lhs &gt;= rhs).
        /// </summary>
        public static bVector2 operator>=(hVector2 lhs, Half rhs) => new bVector2(lhs.x >= rhs, lhs.y >= rhs);
        
        /// <summary>
        /// Returns a bVector2 from component-wise application of operator&gt;= (lhs &gt;= rhs).
        /// </summary>
        public static bVector2 operator>=(Half lhs, hVector2 rhs) => new bVector2(lhs >= rhs.x, lhs >= rhs.y);
        
        /// <summary>
        /// Returns a hVector2 from component-wise application of operator+ (lhs + rhs).
        /// </summary>
        public static hVector2 operator+(hVector2 lhs, hVector2 rhs) => new hVector2(lhs.x + rhs.x, lhs.y + rhs.y);
        
        /// <summary>
        /// Returns a hVector2 from component-wise application of operator+ (lhs + rhs).
        /// </summary>
        public static hVector2 operator+(hVector2 lhs, Half rhs) => new hVector2(lhs.x + rhs, lhs.y + rhs);
        
        /// <summary>
        /// Returns a hVector2 from component-wise application of operator+ (lhs + rhs).
        /// </summary>
        public static hVector2 operator+(Half lhs, hVector2 rhs) => new hVector2(lhs + rhs.x, lhs + rhs.y);
        
        /// <summary>
        /// Returns a hVector2 from component-wise application of operator- (lhs - rhs).
        /// </summary>
        public static hVector2 operator-(hVector2 lhs, hVector2 rhs) => new hVector2(lhs.x - rhs.x, lhs.y - rhs.y);
        
        /// <summary>
        /// Returns a hVector2 from component-wise application of operator- (lhs - rhs).
        /// </summary>
        public static hVector2 operator-(hVector2 lhs, Half rhs) => new hVector2(lhs.x - rhs, lhs.y - rhs);
        
        /// <summary>
        /// Returns a hVector2 from component-wise application of operator- (lhs - rhs).
        /// </summary>
        public static hVector2 operator-(Half lhs, hVector2 rhs) => new hVector2(lhs - rhs.x, lhs - rhs.y);
        
        /// <summary>
        /// Returns a hVector2 from component-wise application of operator* (lhs * rhs).
        /// </summary>
        public static hVector2 operator*(hVector2 lhs, hVector2 rhs) => new hVector2(lhs.x * rhs.x, lhs.y * rhs.y);
        
        /// <summary>
        /// Returns a hVector2 from component-wise application of operator* (lhs * rhs).
        /// </summary>
        public static hVector2 operator*(hVector2 lhs, Half rhs) => new hVector2(lhs.x * rhs, lhs.y * rhs);
        
        /// <summary>
        /// Returns a hVector2 from component-wise application of operator* (lhs * rhs).
        /// </summary>
        public static hVector2 operator*(Half lhs, hVector2 rhs) => new hVector2(lhs * rhs.x, lhs * rhs.y);
        
        /// <summary>
        /// Returns a hVector2 from component-wise application of operator/ (lhs / rhs).
        /// </summary>
        public static hVector2 operator/(hVector2 lhs, hVector2 rhs) => new hVector2(lhs.x / rhs.x, lhs.y / rhs.y);
        
        /// <summary>
        /// Returns a hVector2 from component-wise application of operator/ (lhs / rhs).
        /// </summary>
        public static hVector2 operator/(hVector2 lhs, Half rhs) => new hVector2(lhs.x / rhs, lhs.y / rhs);
        
        /// <summary>
        /// Returns a hVector2 from component-wise application of operator/ (lhs / rhs).
        /// </summary>
        public static hVector2 operator/(Half lhs, hVector2 rhs) => new hVector2(lhs / rhs.x, lhs / rhs.y);
        
        /// <summary>
        /// Returns a hVector2 from component-wise application of operator+ (identity).
        /// </summary>
        public static hVector2 operator+(hVector2 v) => v;
        
        /// <summary>
        /// Returns a hVector2 from component-wise application of operator- (-v).
        /// </summary>
        public static hVector2 operator-(hVector2 v) => new hVector2(-v.x, -v.y);
        
        /// <summary>
        /// Returns a hVector2 from component-wise application of operator% (lhs % rhs).
        /// </summary>
        public static hVector2 operator%(hVector2 lhs, hVector2 rhs) => new hVector2(lhs.x % rhs.x, lhs.y % rhs.y);
        
        /// <summary>
        /// Returns a hVector2 from component-wise application of operator% (lhs % rhs).
        /// </summary>
        public static hVector2 operator%(hVector2 lhs, Half rhs) => new hVector2(lhs.x % rhs, lhs.y % rhs);
        
        /// <summary>
        /// Returns a hVector2 from component-wise application of operator% (lhs % rhs).
        /// </summary>
        public static hVector2 operator%(Half lhs, hVector2 rhs) => new hVector2(lhs % rhs.x, lhs % rhs.y);

        #endregion

    }
}
