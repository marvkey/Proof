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
    /// A Vector of type decimal with 2 components.
    /// </summary> 
    
    
    [StructLayout(LayoutKind.Sequential)]
    public struct decVector2 : IReadOnlyList<decimal>, IEquatable<decVector2>
    {

        #region Fields
        
        /// <summary>
        /// x-component
        /// </summary>
        
        public decimal x;
        
        /// <summary>
        /// y-component
        /// </summary>
        
        public decimal y;

        #endregion


        #region Constructors
        
        /// <summary>
        /// Component-wise constructor
        /// </summary>
        public decVector2(decimal x, decimal y)
        {
            this.x = x;
            this.y = y;
        }
        
        /// <summary>
        /// all-same-value constructor
        /// </summary>
        public decVector2(decimal v)
        {
            this.x = v;
            this.y = v;
        }
        
        /// <summary>
        /// from-Vector constructor
        /// </summary>
        public decVector2(decVector2 v)
        {
            this.x = v.x;
            this.y = v.y;
        }
        
        /// <summary>
        /// from-Vector constructor (additional fields are truncated)
        /// </summary>
        public decVector2(decVector3 v)
        {
            this.x = v.x;
            this.y = v.y;
        }
        
        /// <summary>
        /// from-Vector constructor (additional fields are truncated)
        /// </summary>
        public decVector2(decVector4 v)
        {
            this.x = v.x;
            this.y = v.y;
        }
        
        /// <summary>
        /// From-array/list constructor (superfluous values are ignored, missing values are zero-filled).
        /// </summary>
        public decVector2(IReadOnlyList<decimal> v)
        {
            var c = v.Count;
            this.x = c < 0 ? 0m : v[0];
            this.y = c < 1 ? 0m : v[1];
        }
        
        /// <summary>
        /// Generic from-array constructor (superfluous values are ignored, missing values are zero-filled).
        /// </summary>
        public decVector2(Object[] v)
        {
            var c = v.Length;
            this.x = c < 0 ? 0m : (decimal)v[0];
            this.y = c < 1 ? 0m : (decimal)v[1];
        }
        
        /// <summary>
        /// From-array constructor (superfluous values are ignored, missing values are zero-filled).
        /// </summary>
        public decVector2(decimal[] v)
        {
            var c = v.Length;
            this.x = c < 0 ? 0m : v[0];
            this.y = c < 1 ? 0m : v[1];
        }
        
        /// <summary>
        /// From-array constructor with base index (superfluous values are ignored, missing values are zero-filled).
        /// </summary>
        public decVector2(decimal[] v, int startIndex)
        {
            var c = v.Length;
            this.x = c + startIndex < 0 ? 0m : v[0 + startIndex];
            this.y = c + startIndex < 1 ? 0m : v[1 + startIndex];
        }
        
        /// <summary>
        /// From-IEnumerable constructor (superfluous values are ignored, missing values are zero-filled).
        /// </summary>
        public decVector2(IEnumerable<decimal> v)
            : this(v.ToArray())
        {
        }

        #endregion


        #region Explicit Operators
        
        /// <summary>
        /// Explicitly converts this to a iVector2.
        /// </summary>
        public static explicit operator iVector2(decVector2 v) => new iVector2((int)v.x, (int)v.y);
        
        /// <summary>
        /// Explicitly converts this to a iVector3. (Higher components are zeroed)
        /// </summary>
        public static explicit operator iVector3(decVector2 v) => new iVector3((int)v.x, (int)v.y, 0);
        
        /// <summary>
        /// Explicitly converts this to a iVector4. (Higher components are zeroed)
        /// </summary>
        public static explicit operator iVector4(decVector2 v) => new iVector4((int)v.x, (int)v.y, 0, 0);
        
        /// <summary>
        /// Explicitly converts this to a uVector2.
        /// </summary>
        public static explicit operator uVector2(decVector2 v) => new uVector2((uint)v.x, (uint)v.y);
        
        /// <summary>
        /// Explicitly converts this to a uVector3. (Higher components are zeroed)
        /// </summary>
        public static explicit operator uVector3(decVector2 v) => new uVector3((uint)v.x, (uint)v.y, 0u);
        
        /// <summary>
        /// Explicitly converts this to a uVector4. (Higher components are zeroed)
        /// </summary>
        public static explicit operator uVector4(decVector2 v) => new uVector4((uint)v.x, (uint)v.y, 0u, 0u);
        
        /// <summary>
        /// Explicitly converts this to a Vector2.
        /// </summary>
        public static explicit operator Vector2(decVector2 v) => new Vector2((float)v.x, (float)v.y);
        
        /// <summary>
        /// Explicitly converts this to a Vector3. (Higher components are zeroed)
        /// </summary>
        public static explicit operator Vector3(decVector2 v) => new Vector3((float)v.x, (float)v.y, 0f);
        
        /// <summary>
        /// Explicitly converts this to a Vector4. (Higher components are zeroed)
        /// </summary>
        public static explicit operator Vector4(decVector2 v) => new Vector4((float)v.x, (float)v.y, 0f, 0f);
        
        /// <summary>
        /// Explicitly converts this to a hVector2.
        /// </summary>
        public static explicit operator hVector2(decVector2 v) => new hVector2((Half)v.x, (Half)v.y);
        
        /// <summary>
        /// Explicitly converts this to a hVector3. (Higher components are zeroed)
        /// </summary>
        public static explicit operator hVector3(decVector2 v) => new hVector3((Half)v.x, (Half)v.y, Half.Zero);
        
        /// <summary>
        /// Explicitly converts this to a hVector4. (Higher components are zeroed)
        /// </summary>
        public static explicit operator hVector4(decVector2 v) => new hVector4((Half)v.x, (Half)v.y, Half.Zero, Half.Zero);
        
        /// <summary>
        /// Explicitly converts this to a dVector2.
        /// </summary>
        public static explicit operator dVector2(decVector2 v) => new dVector2((double)v.x, (double)v.y);
        
        /// <summary>
        /// Explicitly converts this to a dVector3. (Higher components are zeroed)
        /// </summary>
        public static explicit operator dVector3(decVector2 v) => new dVector3((double)v.x, (double)v.y, 0.0);
        
        /// <summary>
        /// Explicitly converts this to a dVector4. (Higher components are zeroed)
        /// </summary>
        public static explicit operator dVector4(decVector2 v) => new dVector4((double)v.x, (double)v.y, 0.0, 0.0);
        
        /// <summary>
        /// Explicitly converts this to a decVector3. (Higher components are zeroed)
        /// </summary>
        public static explicit operator decVector3(decVector2 v) => new decVector3((decimal)v.x, (decimal)v.y, 0m);
        
        /// <summary>
        /// Explicitly converts this to a decVector4. (Higher components are zeroed)
        /// </summary>
        public static explicit operator decVector4(decVector2 v) => new decVector4((decimal)v.x, (decimal)v.y, 0m, 0m);
        
        /// <summary>
        /// Explicitly converts this to a cVector2.
        /// </summary>
        //public static explicit operator cVector2(decVector2 v) => new cVector2((Complex)v.x, (Complex)v.y);
        
        /// <summary>
        /// Explicitly converts this to a cVector3. (Higher components are zeroed)
        /// </summary>
        //public static explicit operator cVector3(decVector2 v) => new cVector3((Complex)v.x, (Complex)v.y, Complex.Zero);
        
        /// <summary>
        /// Explicitly converts this to a cVector4. (Higher components are zeroed)
        /// </summary>
        //public static explicit operator cVector4(decVector2 v) => new cVector4((Complex)v.x, (Complex)v.y, Complex.Zero, Complex.Zero);
        
        /// <summary>
        /// Explicitly converts this to a lVector2.
        /// </summary>
        public static explicit operator lVector2(decVector2 v) => new lVector2((long)v.x, (long)v.y);
        
        /// <summary>
        /// Explicitly converts this to a lVector3. (Higher components are zeroed)
        /// </summary>
        public static explicit operator lVector3(decVector2 v) => new lVector3((long)v.x, (long)v.y, 0);
        
        /// <summary>
        /// Explicitly converts this to a lVector4. (Higher components are zeroed)
        /// </summary>
        public static explicit operator lVector4(decVector2 v) => new lVector4((long)v.x, (long)v.y, 0, 0);
        
        /// <summary>
        /// Explicitly converts this to a bVector2.
        /// </summary>
        public static explicit operator bVector2(decVector2 v) => new bVector2(v.x != 0m, v.y != 0m);
        
        /// <summary>
        /// Explicitly converts this to a bVector3. (Higher components are zeroed)
        /// </summary>
        public static explicit operator bVector3(decVector2 v) => new bVector3(v.x != 0m, v.y != 0m, false);
        
        /// <summary>
        /// Explicitly converts this to a bVector4. (Higher components are zeroed)
        /// </summary>
        public static explicit operator bVector4(decVector2 v) => new bVector4(v.x != 0m, v.y != 0m, false, false);
        
        /// <summary>
        /// Explicitly converts this to a decimal array.
        /// </summary>
        public static explicit operator decimal[](decVector2 v) => new [] { v.x, v.y };
        
        /// <summary>
        /// Explicitly converts this to a generic object array.
        /// </summary>
        public static explicit operator Object[](decVector2 v) => new Object[] { v.x, v.y };

        #endregion


        #region Indexer
        
        /// <summary>
        /// Gets/Sets a specific indexed component (a bit slower than direct access).
        /// </summary>
        public decimal this[int index]
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
        public swizzle_decVector2 swizzle => new swizzle_decVector2(x, y);
        
        /// <summary>
        /// Gets or sets the specified subset of components. For more advanced (read-only) swizzling, use the .swizzle property.
        /// </summary>
        public decVector2 xy
        {
            get
            {
                return new decVector2(x, y);
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
        public decVector2 rg
        {
            get
            {
                return new decVector2(x, y);
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
        public decimal r
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
        public decimal g
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
        public decimal[] Values => new[] { x, y };
        
        /// <summary>
        /// Returns the number of components (2).
        /// </summary>
        public int Count => 2;
        
        /// <summary>
        /// Returns the minimal component of this Vector.
        /// </summary>
        public decimal MinElement => Math.Min(x, y);
        
        /// <summary>
        /// Returns the maximal component of this Vector.
        /// </summary>
        public decimal MaxElement => Math.Max(x, y);
        
        /// <summary>
        /// Returns the euclidean length of this Vector.
        /// </summary>
        public decimal Length => (decimal)((x*x + y*y)).Sqrt();
        
        /// <summary>
        /// Returns the squared euclidean length of this Vector.
        /// </summary>
        public decimal LengthSqr => (x*x + y*y);
        
        /// <summary>
        /// Returns the sum of all components.
        /// </summary>
        public decimal Sum => (x + y);
        
        /// <summary>
        /// Returns the euclidean norm of this Vector.
        /// </summary>
        public decimal Norm => (decimal)((x*x + y*y)).Sqrt();
        
        /// <summary>
        /// Returns the one-norm of this Vector.
        /// </summary>
        public decimal Norm1 => (Math.Abs(x) + Math.Abs(y));
        
        /// <summary>
        /// Returns the two-norm (euclidean length) of this Vector.
        /// </summary>
        public decimal Norm2 => (decimal)((x*x + y*y)).Sqrt();
        
        /// <summary>
        /// Returns the max-norm of this Vector.
        /// </summary>
        public decimal NormMax => Math.Max(Math.Abs(x), Math.Abs(y));
        
        /// <summary>
        /// Returns a copy of this Vector with length one (undefined if this has zero length).
        /// </summary>
        public decVector2 Normalized => this / (decimal)Length;
        
        /// <summary>
        /// Returns a copy of this Vector with length one (returns zero if length is zero).
        /// </summary>
        public decVector2 NormalizedSafe => this == Zero ? Zero : this / (decimal)Length;
        
        /// <summary>
        /// Returns the Vector angle (atan2(y, x)) in radians.
        /// </summary>
        public double Angle => Math.Atan2((double)y, (double)x);

        #endregion


        #region Static Properties
        
        /// <summary>
        /// Predefined all-zero Vector
        /// </summary>
        public static decVector2 Zero { get; } = new decVector2(0m, 0m);
        
        /// <summary>
        /// Predefined all-ones Vector
        /// </summary>
        public static decVector2 Ones { get; } = new decVector2(1m, 1m);
        
        /// <summary>
        /// Predefined unit-X Vector
        /// </summary>
        public static decVector2 UnitX { get; } = new decVector2(1m, 0m);
        
        /// <summary>
        /// Predefined unit-Y Vector
        /// </summary>
        public static decVector2 UnitY { get; } = new decVector2(0m, 1m);
        
        /// <summary>
        /// Predefined all-MaxValue Vector
        /// </summary>
        public static decVector2 MaxValue { get; } = new decVector2(decimal.MaxValue, decimal.MaxValue);
        
        /// <summary>
        /// Predefined all-MinValue Vector
        /// </summary>
        public static decVector2 MinValue { get; } = new decVector2(decimal.MinValue, decimal.MinValue);
        
        /// <summary>
        /// Predefined all-MinusOne Vector
        /// </summary>
        public static decVector2 MinusOne { get; } = new decVector2(decimal.MinusOne, decimal.MinusOne);

        #endregion


        #region Operators
        
        /// <summary>
        /// Returns true iff this equals rhs component-wise.
        /// </summary>
        public static bool operator==(decVector2 lhs, decVector2 rhs) => lhs.Equals(rhs);
        
        /// <summary>
        /// Returns true iff this does not equal rhs (component-wise).
        /// </summary>
        public static bool operator!=(decVector2 lhs, decVector2 rhs) => !lhs.Equals(rhs);

        #endregion


        #region Functions
        
        /// <summary>
        /// Returns an enumerator that iterates through all components.
        /// </summary>
        public IEnumerator<decimal> GetEnumerator()
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
        public bool Equals(decVector2 rhs) => (x.Equals(rhs.x) && y.Equals(rhs.y));
        
        /// <summary>
        /// Returns true iff this equals rhs type- and component-wise.
        /// </summary>
        public override bool Equals(object obj)
        {
            if (ReferenceEquals(null, obj)) return false;
            return obj is decVector2 && Equals((decVector2) obj);
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
        public double NormP(double p) => Math.Pow((Math.Pow((double)Math.Abs(x), p) + Math.Pow((double)Math.Abs(y), p)), 1 / p);
        
        /// <summary>
        /// Returns a 2D Vector that was rotated by a given angle in radians (CAUTION: result is casted and may be truncated).
        /// </summary>
        public decVector2 Rotated(double angleInRad) => (decVector2)(dVector2.FromAngle(Angle + angleInRad) * (double)Length);

        #endregion


        #region Static Functions
        
        /// <summary>
        /// Converts the string representation of the Vector into a Vector representation (using ', ' as a separator).
        /// </summary>
        public static decVector2 Parse(string s) => Parse(s, ", ");
        
        /// <summary>
        /// Converts the string representation of the Vector into a Vector representation (using a designated separator).
        /// </summary>
        public static decVector2 Parse(string s, string sep)
        {
            var kvp = s.Split(new[] { sep }, StringSplitOptions.None);
            if (kvp.Length != 2) throw new FormatException("input has not exactly 2 parts");
            return new decVector2(decimal.Parse(kvp[0].Trim()), decimal.Parse(kvp[1].Trim()));
        }
        
        /// <summary>
        /// Converts the string representation of the Vector into a Vector representation (using a designated separator and a type provider).
        /// </summary>
        public static decVector2 Parse(string s, string sep, IFormatProvider provider)
        {
            var kvp = s.Split(new[] { sep }, StringSplitOptions.None);
            if (kvp.Length != 2) throw new FormatException("input has not exactly 2 parts");
            return new decVector2(decimal.Parse(kvp[0].Trim(), provider), decimal.Parse(kvp[1].Trim(), provider));
        }
        
        /// <summary>
        /// Converts the string representation of the Vector into a Vector representation (using a designated separator and a number style).
        /// </summary>
        public static decVector2 Parse(string s, string sep, NumberStyles style)
        {
            var kvp = s.Split(new[] { sep }, StringSplitOptions.None);
            if (kvp.Length != 2) throw new FormatException("input has not exactly 2 parts");
            return new decVector2(decimal.Parse(kvp[0].Trim(), style), decimal.Parse(kvp[1].Trim(), style));
        }
        
        /// <summary>
        /// Converts the string representation of the Vector into a Vector representation (using a designated separator and a number style and a forMatrix provider).
        /// </summary>
        public static decVector2 Parse(string s, string sep, NumberStyles style, IFormatProvider provider)
        {
            var kvp = s.Split(new[] { sep }, StringSplitOptions.None);
            if (kvp.Length != 2) throw new FormatException("input has not exactly 2 parts");
            return new decVector2(decimal.Parse(kvp[0].Trim(), style, provider), decimal.Parse(kvp[1].Trim(), style, provider));
        }
        
        /// <summary>
        /// Tries to convert the string representation of the Vector into a Vector representation (using ', ' as a separator), returns false if string was invalid.
        /// </summary>
        public static bool TryParse(string s, out decVector2 result) => TryParse(s, ", ", out result);
        
        /// <summary>
        /// Tries to convert the string representation of the Vector into a Vector representation (using a designated separator), returns false if string was invalid.
        /// </summary>
        public static bool TryParse(string s, string sep, out decVector2 result)
        {
            result = Zero;
            if (string.IsNullOrEmpty(s)) return false;
            var kvp = s.Split(new[] { sep }, StringSplitOptions.None);
            if (kvp.Length != 2) return false;
            decimal x = 0m, y = 0m;
            var ok = (decimal.TryParse(kvp[0].Trim(), out x) && decimal.TryParse(kvp[1].Trim(), out y));
            result = ok ? new decVector2(x, y) : Zero;
            return ok;
        }
        
        /// <summary>
        /// Tries to convert the string representation of the Vector into a Vector representation (using a designated separator and a number style and a forMatrix provider), returns false if string was invalid.
        /// </summary>
        public static bool TryParse(string s, string sep, NumberStyles style, IFormatProvider provider, out decVector2 result)
        {
            result = Zero;
            if (string.IsNullOrEmpty(s)) return false;
            var kvp = s.Split(new[] { sep }, StringSplitOptions.None);
            if (kvp.Length != 2) return false;
            decimal x = 0m, y = 0m;
            var ok = (decimal.TryParse(kvp[0].Trim(), style, provider, out x) && decimal.TryParse(kvp[1].Trim(), style, provider, out y));
            result = ok ? new decVector2(x, y) : Zero;
            return ok;
        }
        
        /// <summary>
        /// Returns true iff distance between lhs and rhs is less than or equal to epsilon
        /// </summary>
        public static bool ApproxEqual(decVector2 lhs, decVector2 rhs, decimal eps = 0.1m) => Distance(lhs, rhs) <= eps;
        
        /// <summary>
        /// OuterProduct treats the first parameter c as a column Vector (Matrix with one column) and the second parameter r as a row Vector (Matrix with one row) and does a linear algebraic Matrix multiply c * r, yielding a Matrix whose number of rows is the number of components in c and whose number of columns is the number of components in r.
        /// </summary>
        public static decMatrix2 OuterProduct(decVector2 c, decVector2 r) => new decMatrix2(c.x * r.x, c.y * r.x, c.x * r.y, c.y * r.y);
        
        /// <summary>
        /// OuterProduct treats the first parameter c as a column Vector (Matrix with one column) and the second parameter r as a row Vector (Matrix with one row) and does a linear algebraic Matrix multiply c * r, yielding a Matrix whose number of rows is the number of components in c and whose number of columns is the number of components in r.
        /// </summary>
        public static decMatrix2x3 OuterProduct(decVector3 c, decVector2 r) => new decMatrix2x3(c.x * r.x, c.y * r.x, c.z * r.x, c.x * r.y, c.y * r.y, c.z * r.y);
        
        /// <summary>
        /// OuterProduct treats the first parameter c as a column Vector (Matrix with one column) and the second parameter r as a row Vector (Matrix with one row) and does a linear algebraic Matrix multiply c * r, yielding a Matrix whose number of rows is the number of components in c and whose number of columns is the number of components in r.
        /// </summary>
        public static decMatrix3x2 OuterProduct(decVector2 c, decVector3 r) => new decMatrix3x2(c.x * r.x, c.y * r.x, c.x * r.y, c.y * r.y, c.x * r.z, c.y * r.z);
        
        /// <summary>
        /// OuterProduct treats the first parameter c as a column Vector (Matrix with one column) and the second parameter r as a row Vector (Matrix with one row) and does a linear algebraic Matrix multiply c * r, yielding a Matrix whose number of rows is the number of components in c and whose number of columns is the number of components in r.
        /// </summary>
        public static decMatrix2x4 OuterProduct(decVector4 c, decVector2 r) => new decMatrix2x4(c.x * r.x, c.y * r.x, c.z * r.x, c.w * r.x, c.x * r.y, c.y * r.y, c.z * r.y, c.w * r.y);
        
        /// <summary>
        /// OuterProduct treats the first parameter c as a column Vector (Matrix with one column) and the second parameter r as a row Vector (Matrix with one row) and does a linear algebraic Matrix multiply c * r, yielding a Matrix whose number of rows is the number of components in c and whose number of columns is the number of components in r.
        /// </summary>
        public static decMatrix4x2 OuterProduct(decVector2 c, decVector4 r) => new decMatrix4x2(c.x * r.x, c.y * r.x, c.x * r.y, c.y * r.y, c.x * r.z, c.y * r.z, c.x * r.w, c.y * r.w);
        
        /// <summary>
        /// Returns a unit 2D Vector with a given angle in radians (CAUTION: result may be truncated for integer types).
        /// </summary>
        public static decVector2 FromAngle(double angleInRad) => new decVector2((decimal)Math.Cos(angleInRad), (decimal)Math.Sin(angleInRad));
        
        /// <summary>
        /// Returns the inner product (dot product, scalar product) of the two Vectors.
        /// </summary>
        public static decimal Dot(decVector2 lhs, decVector2 rhs) => (lhs.x * rhs.x + lhs.y * rhs.y);
        
        /// <summary>
        /// Returns the euclidean distance between the two Vectors.
        /// </summary>
        public static decimal Distance(decVector2 lhs, decVector2 rhs) => (lhs - rhs).Length;
        
        /// <summary>
        /// Returns the squared euclidean distance between the two Vectors.
        /// </summary>
        public static decimal DistanceSqr(decVector2 lhs, decVector2 rhs) => (lhs - rhs).LengthSqr;
        
        /// <summary>
        /// Calculate the reflection direction for an incident Vector (N should be normalized in order to achieve the desired result).
        /// </summary>
        public static decVector2 Reflect(decVector2 I, decVector2 N) => I - 2 * Dot(N, I) * N;
        
        /// <summary>
        /// Calculate the refraction direction for an incident Vector (The input parameters I and N should be normalized in order to achieve the desired result).
        /// </summary>
        public static decVector2 Refract(decVector2 I, decVector2 N, decimal eta)
        {
            var dNI = Dot(N, I);
            var k = 1 - eta * eta * (1 - dNI * dNI);
            if (k < 0) return Zero;
            return eta * I - (eta * dNI + (decimal)(k).Sqrt()) * N;
        }
        
        /// <summary>
        /// Returns a Vector pointing in the same direction as another (faceforward orients a Vector to point away from a surface as defined by its normal. If dot(Nref, I) is negative faceforward returns N, otherwise it returns -N).
        /// </summary>
        public static decVector2 FaceForward(decVector2 N, decVector2 I, decVector2 Nref) => Dot(Nref, I) < 0 ? N : -N;
        
        /// <summary>
        /// Returns the length of the outer product (cross product, Vector product) of the two Vectors.
        /// </summary>
        public static decimal Cross(decVector2 l, decVector2 r) => l.x * r.y - l.y * r.x;
        
        /// <summary>
        /// Returns a decVector2 with independent and identically distributed uniform values between 0.0 and 1.0.
        /// </summary>
        public static decVector2 Random(System.Random random) => new decVector2((decimal)random.NextDouble(), (decimal)random.NextDouble());
        
        /// <summary>
        /// Returns a decVector2 with independent and identically distributed uniform values between -1.0 and 1.0.
        /// </summary>
        public static decVector2 RandomSigned(System.Random random) => new decVector2((decimal)(random.NextDouble() * 2.0 - 1.0), (decimal)(random.NextDouble() * 2.0 - 1.0));
        
        /// <summary>
        /// Returns a decVector2 with independent and identically distributed values according to a normal distribution (zero mean, unit variance).
        /// </summary>
        public static decVector2 RandomNormal(System.Random random) => new decVector2((decimal)(Math.Cos(2 * Math.PI * random.NextDouble()) * Math.Sqrt(-2.0 * Math.Log(random.NextDouble()))), (decimal)(Math.Cos(2 * Math.PI * random.NextDouble()) * Math.Sqrt(-2.0 * Math.Log(random.NextDouble()))));

        #endregion


        #region Component-Wise Static Functions
        
        /// <summary>
        /// Returns a bVector2 from component-wise application of Equal (lhs == rhs).
        /// </summary>
        public static bVector2 Equal(decVector2 lhs, decVector2 rhs) => new bVector2(lhs.x == rhs.x, lhs.y == rhs.y);
        
        /// <summary>
        /// Returns a bVector2 from component-wise application of Equal (lhs == rhs).
        /// </summary>
        public static bVector2 Equal(decVector2 lhs, decimal rhs) => new bVector2(lhs.x == rhs, lhs.y == rhs);
        
        /// <summary>
        /// Returns a bVector2 from component-wise application of Equal (lhs == rhs).
        /// </summary>
        public static bVector2 Equal(decimal lhs, decVector2 rhs) => new bVector2(lhs == rhs.x, lhs == rhs.y);
        
        /// <summary>
        /// Returns a bVector from the application of Equal (lhs == rhs).
        /// </summary>
        public static bVector2 Equal(decimal lhs, decimal rhs) => new bVector2(lhs == rhs);
        
        /// <summary>
        /// Returns a bVector2 from component-wise application of NotEqual (lhs != rhs).
        /// </summary>
        public static bVector2 NotEqual(decVector2 lhs, decVector2 rhs) => new bVector2(lhs.x != rhs.x, lhs.y != rhs.y);
        
        /// <summary>
        /// Returns a bVector2 from component-wise application of NotEqual (lhs != rhs).
        /// </summary>
        public static bVector2 NotEqual(decVector2 lhs, decimal rhs) => new bVector2(lhs.x != rhs, lhs.y != rhs);
        
        /// <summary>
        /// Returns a bVector2 from component-wise application of NotEqual (lhs != rhs).
        /// </summary>
        public static bVector2 NotEqual(decimal lhs, decVector2 rhs) => new bVector2(lhs != rhs.x, lhs != rhs.y);
        
        /// <summary>
        /// Returns a bVector from the application of NotEqual (lhs != rhs).
        /// </summary>
        public static bVector2 NotEqual(decimal lhs, decimal rhs) => new bVector2(lhs != rhs);
        
        /// <summary>
        /// Returns a bVector2 from component-wise application of GreaterThan (lhs &gt; rhs).
        /// </summary>
        public static bVector2 GreaterThan(decVector2 lhs, decVector2 rhs) => new bVector2(lhs.x > rhs.x, lhs.y > rhs.y);
        
        /// <summary>
        /// Returns a bVector2 from component-wise application of GreaterThan (lhs &gt; rhs).
        /// </summary>
        public static bVector2 GreaterThan(decVector2 lhs, decimal rhs) => new bVector2(lhs.x > rhs, lhs.y > rhs);
        
        /// <summary>
        /// Returns a bVector2 from component-wise application of GreaterThan (lhs &gt; rhs).
        /// </summary>
        public static bVector2 GreaterThan(decimal lhs, decVector2 rhs) => new bVector2(lhs > rhs.x, lhs > rhs.y);
        
        /// <summary>
        /// Returns a bVector from the application of GreaterThan (lhs &gt; rhs).
        /// </summary>
        public static bVector2 GreaterThan(decimal lhs, decimal rhs) => new bVector2(lhs > rhs);
        
        /// <summary>
        /// Returns a bVector2 from component-wise application of GreaterThanEqual (lhs &gt;= rhs).
        /// </summary>
        public static bVector2 GreaterThanEqual(decVector2 lhs, decVector2 rhs) => new bVector2(lhs.x >= rhs.x, lhs.y >= rhs.y);
        
        /// <summary>
        /// Returns a bVector2 from component-wise application of GreaterThanEqual (lhs &gt;= rhs).
        /// </summary>
        public static bVector2 GreaterThanEqual(decVector2 lhs, decimal rhs) => new bVector2(lhs.x >= rhs, lhs.y >= rhs);
        
        /// <summary>
        /// Returns a bVector2 from component-wise application of GreaterThanEqual (lhs &gt;= rhs).
        /// </summary>
        public static bVector2 GreaterThanEqual(decimal lhs, decVector2 rhs) => new bVector2(lhs >= rhs.x, lhs >= rhs.y);
        
        /// <summary>
        /// Returns a bVector from the application of GreaterThanEqual (lhs &gt;= rhs).
        /// </summary>
        public static bVector2 GreaterThanEqual(decimal lhs, decimal rhs) => new bVector2(lhs >= rhs);
        
        /// <summary>
        /// Returns a bVector2 from component-wise application of LesserThan (lhs &lt; rhs).
        /// </summary>
        public static bVector2 LesserThan(decVector2 lhs, decVector2 rhs) => new bVector2(lhs.x < rhs.x, lhs.y < rhs.y);
        
        /// <summary>
        /// Returns a bVector2 from component-wise application of LesserThan (lhs &lt; rhs).
        /// </summary>
        public static bVector2 LesserThan(decVector2 lhs, decimal rhs) => new bVector2(lhs.x < rhs, lhs.y < rhs);
        
        /// <summary>
        /// Returns a bVector2 from component-wise application of LesserThan (lhs &lt; rhs).
        /// </summary>
        public static bVector2 LesserThan(decimal lhs, decVector2 rhs) => new bVector2(lhs < rhs.x, lhs < rhs.y);
        
        /// <summary>
        /// Returns a bVector from the application of LesserThan (lhs &lt; rhs).
        /// </summary>
        public static bVector2 LesserThan(decimal lhs, decimal rhs) => new bVector2(lhs < rhs);
        
        /// <summary>
        /// Returns a bVector2 from component-wise application of LesserThanEqual (lhs &lt;= rhs).
        /// </summary>
        public static bVector2 LesserThanEqual(decVector2 lhs, decVector2 rhs) => new bVector2(lhs.x <= rhs.x, lhs.y <= rhs.y);
        
        /// <summary>
        /// Returns a bVector2 from component-wise application of LesserThanEqual (lhs &lt;= rhs).
        /// </summary>
        public static bVector2 LesserThanEqual(decVector2 lhs, decimal rhs) => new bVector2(lhs.x <= rhs, lhs.y <= rhs);
        
        /// <summary>
        /// Returns a bVector2 from component-wise application of LesserThanEqual (lhs &lt;= rhs).
        /// </summary>
        public static bVector2 LesserThanEqual(decimal lhs, decVector2 rhs) => new bVector2(lhs <= rhs.x, lhs <= rhs.y);
        
        /// <summary>
        /// Returns a bVector from the application of LesserThanEqual (lhs &lt;= rhs).
        /// </summary>
        public static bVector2 LesserThanEqual(decimal lhs, decimal rhs) => new bVector2(lhs <= rhs);
        
        /// <summary>
        /// Returns a decVector2 from component-wise application of Abs (Math.Abs(v)).
        /// </summary>
        public static decVector2 Abs(decVector2 v) => new decVector2(Math.Abs(v.x), Math.Abs(v.y));
        
        /// <summary>
        /// Returns a decVector from the application of Abs (Math.Abs(v)).
        /// </summary>
        public static decVector2 Abs(decimal v) => new decVector2(Math.Abs(v));
        
        /// <summary>
        /// Returns a decVector2 from component-wise application of HermiteInterpolationOrder3 ((3 - 2 * v) * v * v).
        /// </summary>
        public static decVector2 HermiteInterpolationOrder3(decVector2 v) => new decVector2((3 - 2 * v.x) * v.x * v.x, (3 - 2 * v.y) * v.y * v.y);
        
        /// <summary>
        /// Returns a decVector from the application of HermiteInterpolationOrder3 ((3 - 2 * v) * v * v).
        /// </summary>
        public static decVector2 HermiteInterpolationOrder3(decimal v) => new decVector2((3 - 2 * v) * v * v);
        
        /// <summary>
        /// Returns a decVector2 from component-wise application of HermiteInterpolationOrder5 (((6 * v - 15) * v + 10) * v * v * v).
        /// </summary>
        public static decVector2 HermiteInterpolationOrder5(decVector2 v) => new decVector2(((6 * v.x - 15) * v.x + 10) * v.x * v.x * v.x, ((6 * v.y - 15) * v.y + 10) * v.y * v.y * v.y);
        
        /// <summary>
        /// Returns a decVector from the application of HermiteInterpolationOrder5 (((6 * v - 15) * v + 10) * v * v * v).
        /// </summary>
        public static decVector2 HermiteInterpolationOrder5(decimal v) => new decVector2(((6 * v - 15) * v + 10) * v * v * v);
        
        /// <summary>
        /// Returns a decVector2 from component-wise application of Sqr (v * v).
        /// </summary>
        public static decVector2 Sqr(decVector2 v) => new decVector2(v.x * v.x, v.y * v.y);
        
        /// <summary>
        /// Returns a decVector from the application of Sqr (v * v).
        /// </summary>
        public static decVector2 Sqr(decimal v) => new decVector2(v * v);
        
        /// <summary>
        /// Returns a decVector2 from component-wise application of Pow2 (v * v).
        /// </summary>
        public static decVector2 Pow2(decVector2 v) => new decVector2(v.x * v.x, v.y * v.y);
        
        /// <summary>
        /// Returns a decVector from the application of Pow2 (v * v).
        /// </summary>
        public static decVector2 Pow2(decimal v) => new decVector2(v * v);
        
        /// <summary>
        /// Returns a decVector2 from component-wise application of Pow3 (v * v * v).
        /// </summary>
        public static decVector2 Pow3(decVector2 v) => new decVector2(v.x * v.x * v.x, v.y * v.y * v.y);
        
        /// <summary>
        /// Returns a decVector from the application of Pow3 (v * v * v).
        /// </summary>
        public static decVector2 Pow3(decimal v) => new decVector2(v * v * v);
        
        /// <summary>
        /// Returns a decVector2 from component-wise application of Step (v &gt;= 0m ? 1m : 0m).
        /// </summary>
        public static decVector2 Step(decVector2 v) => new decVector2(v.x >= 0m ? 1m : 0m, v.y >= 0m ? 1m : 0m);
        
        /// <summary>
        /// Returns a decVector from the application of Step (v &gt;= 0m ? 1m : 0m).
        /// </summary>
        public static decVector2 Step(decimal v) => new decVector2(v >= 0m ? 1m : 0m);
        
        /// <summary>
        /// Returns a decVector2 from component-wise application of Sqrt ((decimal)Math.Sqrt((double)v)).
        /// </summary>
        public static decVector2 Sqrt(decVector2 v) => new decVector2((decimal)Math.Sqrt((double)v.x), (decimal)Math.Sqrt((double)v.y));
        
        /// <summary>
        /// Returns a decVector from the application of Sqrt ((decimal)Math.Sqrt((double)v)).
        /// </summary>
        public static decVector2 Sqrt(decimal v) => new decVector2((decimal)Math.Sqrt((double)v));
        
        /// <summary>
        /// Returns a decVector2 from component-wise application of InverseSqrt ((decimal)(1.0 / Math.Sqrt((double)v))).
        /// </summary>
        public static decVector2 InverseSqrt(decVector2 v) => new decVector2((decimal)(1.0 / Math.Sqrt((double)v.x)), (decimal)(1.0 / Math.Sqrt((double)v.y)));
        
        /// <summary>
        /// Returns a decVector from the application of InverseSqrt ((decimal)(1.0 / Math.Sqrt((double)v))).
        /// </summary>
        public static decVector2 InverseSqrt(decimal v) => new decVector2((decimal)(1.0 / Math.Sqrt((double)v)));
        
        /// <summary>
        /// Returns a iVector2 from component-wise application of Sign (Math.Sign(v)).
        /// </summary>
        public static iVector2 Sign(decVector2 v) => new iVector2(Math.Sign(v.x), Math.Sign(v.y));
        
        /// <summary>
        /// Returns a iVector from the application of Sign (Math.Sign(v)).
        /// </summary>
        public static iVector2 Sign(decimal v) => new iVector2(Math.Sign(v));
        
        /// <summary>
        /// Returns a decVector2 from component-wise application of Max (Math.Max(lhs, rhs)).
        /// </summary>
        public static decVector2 Max(decVector2 lhs, decVector2 rhs) => new decVector2(Math.Max(lhs.x, rhs.x), Math.Max(lhs.y, rhs.y));
        
        /// <summary>
        /// Returns a decVector2 from component-wise application of Max (Math.Max(lhs, rhs)).
        /// </summary>
        public static decVector2 Max(decVector2 lhs, decimal rhs) => new decVector2(Math.Max(lhs.x, rhs), Math.Max(lhs.y, rhs));
        
        /// <summary>
        /// Returns a decVector2 from component-wise application of Max (Math.Max(lhs, rhs)).
        /// </summary>
        public static decVector2 Max(decimal lhs, decVector2 rhs) => new decVector2(Math.Max(lhs, rhs.x), Math.Max(lhs, rhs.y));
        
        /// <summary>
        /// Returns a decVector from the application of Max (Math.Max(lhs, rhs)).
        /// </summary>
        public static decVector2 Max(decimal lhs, decimal rhs) => new decVector2(Math.Max(lhs, rhs));
        
        /// <summary>
        /// Returns a decVector2 from component-wise application of Min (Math.Min(lhs, rhs)).
        /// </summary>
        public static decVector2 Min(decVector2 lhs, decVector2 rhs) => new decVector2(Math.Min(lhs.x, rhs.x), Math.Min(lhs.y, rhs.y));
        
        /// <summary>
        /// Returns a decVector2 from component-wise application of Min (Math.Min(lhs, rhs)).
        /// </summary>
        public static decVector2 Min(decVector2 lhs, decimal rhs) => new decVector2(Math.Min(lhs.x, rhs), Math.Min(lhs.y, rhs));
        
        /// <summary>
        /// Returns a decVector2 from component-wise application of Min (Math.Min(lhs, rhs)).
        /// </summary>
        public static decVector2 Min(decimal lhs, decVector2 rhs) => new decVector2(Math.Min(lhs, rhs.x), Math.Min(lhs, rhs.y));
        
        /// <summary>
        /// Returns a decVector from the application of Min (Math.Min(lhs, rhs)).
        /// </summary>
        public static decVector2 Min(decimal lhs, decimal rhs) => new decVector2(Math.Min(lhs, rhs));
        
        /// <summary>
        /// Returns a decVector2 from component-wise application of Pow ((decimal)Math.Pow((double)lhs, (double)rhs)).
        /// </summary>
        public static decVector2 Pow(decVector2 lhs, decVector2 rhs) => new decVector2((decimal)Math.Pow((double)lhs.x, (double)rhs.x), (decimal)Math.Pow((double)lhs.y, (double)rhs.y));
        
        /// <summary>
        /// Returns a decVector2 from component-wise application of Pow ((decimal)Math.Pow((double)lhs, (double)rhs)).
        /// </summary>
        public static decVector2 Pow(decVector2 lhs, decimal rhs) => new decVector2((decimal)Math.Pow((double)lhs.x, (double)rhs), (decimal)Math.Pow((double)lhs.y, (double)rhs));
        
        /// <summary>
        /// Returns a decVector2 from component-wise application of Pow ((decimal)Math.Pow((double)lhs, (double)rhs)).
        /// </summary>
        public static decVector2 Pow(decimal lhs, decVector2 rhs) => new decVector2((decimal)Math.Pow((double)lhs, (double)rhs.x), (decimal)Math.Pow((double)lhs, (double)rhs.y));
        
        /// <summary>
        /// Returns a decVector from the application of Pow ((decimal)Math.Pow((double)lhs, (double)rhs)).
        /// </summary>
        public static decVector2 Pow(decimal lhs, decimal rhs) => new decVector2((decimal)Math.Pow((double)lhs, (double)rhs));
        
        /// <summary>
        /// Returns a decVector2 from component-wise application of Log ((decimal)Math.Log((double)lhs, (double)rhs)).
        /// </summary>
        public static decVector2 Log(decVector2 lhs, decVector2 rhs) => new decVector2((decimal)Math.Log((double)lhs.x, (double)rhs.x), (decimal)Math.Log((double)lhs.y, (double)rhs.y));
        
        /// <summary>
        /// Returns a decVector2 from component-wise application of Log ((decimal)Math.Log((double)lhs, (double)rhs)).
        /// </summary>
        public static decVector2 Log(decVector2 lhs, decimal rhs) => new decVector2((decimal)Math.Log((double)lhs.x, (double)rhs), (decimal)Math.Log((double)lhs.y, (double)rhs));
        
        /// <summary>
        /// Returns a decVector2 from component-wise application of Log ((decimal)Math.Log((double)lhs, (double)rhs)).
        /// </summary>
        public static decVector2 Log(decimal lhs, decVector2 rhs) => new decVector2((decimal)Math.Log((double)lhs, (double)rhs.x), (decimal)Math.Log((double)lhs, (double)rhs.y));
        
        /// <summary>
        /// Returns a decVector from the application of Log ((decimal)Math.Log((double)lhs, (double)rhs)).
        /// </summary>
        public static decVector2 Log(decimal lhs, decimal rhs) => new decVector2((decimal)Math.Log((double)lhs, (double)rhs));
        
        /// <summary>
        /// Returns a decVector2 from component-wise application of Clamp (Math.Min(Math.Max(v, min), max)).
        /// </summary>
        public static decVector2 Clamp(decVector2 v, decVector2 min, decVector2 max) => new decVector2(Math.Min(Math.Max(v.x, min.x), max.x), Math.Min(Math.Max(v.y, min.y), max.y));
        
        /// <summary>
        /// Returns a decVector2 from component-wise application of Clamp (Math.Min(Math.Max(v, min), max)).
        /// </summary>
        public static decVector2 Clamp(decVector2 v, decVector2 min, decimal max) => new decVector2(Math.Min(Math.Max(v.x, min.x), max), Math.Min(Math.Max(v.y, min.y), max));
        
        /// <summary>
        /// Returns a decVector2 from component-wise application of Clamp (Math.Min(Math.Max(v, min), max)).
        /// </summary>
        public static decVector2 Clamp(decVector2 v, decimal min, decVector2 max) => new decVector2(Math.Min(Math.Max(v.x, min), max.x), Math.Min(Math.Max(v.y, min), max.y));
        
        /// <summary>
        /// Returns a decVector2 from component-wise application of Clamp (Math.Min(Math.Max(v, min), max)).
        /// </summary>
        public static decVector2 Clamp(decVector2 v, decimal min, decimal max) => new decVector2(Math.Min(Math.Max(v.x, min), max), Math.Min(Math.Max(v.y, min), max));
        
        /// <summary>
        /// Returns a decVector2 from component-wise application of Clamp (Math.Min(Math.Max(v, min), max)).
        /// </summary>
        public static decVector2 Clamp(decimal v, decVector2 min, decVector2 max) => new decVector2(Math.Min(Math.Max(v, min.x), max.x), Math.Min(Math.Max(v, min.y), max.y));
        
        /// <summary>
        /// Returns a decVector2 from component-wise application of Clamp (Math.Min(Math.Max(v, min), max)).
        /// </summary>
        public static decVector2 Clamp(decimal v, decVector2 min, decimal max) => new decVector2(Math.Min(Math.Max(v, min.x), max), Math.Min(Math.Max(v, min.y), max));
        
        /// <summary>
        /// Returns a decVector2 from component-wise application of Clamp (Math.Min(Math.Max(v, min), max)).
        /// </summary>
        public static decVector2 Clamp(decimal v, decimal min, decVector2 max) => new decVector2(Math.Min(Math.Max(v, min), max.x), Math.Min(Math.Max(v, min), max.y));
        
        /// <summary>
        /// Returns a decVector from the application of Clamp (Math.Min(Math.Max(v, min), max)).
        /// </summary>
        public static decVector2 Clamp(decimal v, decimal min, decimal max) => new decVector2(Math.Min(Math.Max(v, min), max));
        
        /// <summary>
        /// Returns a decVector2 from component-wise application of Mix (min * (1-a) + max * a).
        /// </summary>
        public static decVector2 Mix(decVector2 min, decVector2 max, decVector2 a) => new decVector2(min.x * (1-a.x) + max.x * a.x, min.y * (1-a.y) + max.y * a.y);
        
        /// <summary>
        /// Returns a decVector2 from component-wise application of Mix (min * (1-a) + max * a).
        /// </summary>
        public static decVector2 Mix(decVector2 min, decVector2 max, decimal a) => new decVector2(min.x * (1-a) + max.x * a, min.y * (1-a) + max.y * a);
        
        /// <summary>
        /// Returns a decVector2 from component-wise application of Mix (min * (1-a) + max * a).
        /// </summary>
        public static decVector2 Mix(decVector2 min, decimal max, decVector2 a) => new decVector2(min.x * (1-a.x) + max * a.x, min.y * (1-a.y) + max * a.y);
        
        /// <summary>
        /// Returns a decVector2 from component-wise application of Mix (min * (1-a) + max * a).
        /// </summary>
        public static decVector2 Mix(decVector2 min, decimal max, decimal a) => new decVector2(min.x * (1-a) + max * a, min.y * (1-a) + max * a);
        
        /// <summary>
        /// Returns a decVector2 from component-wise application of Mix (min * (1-a) + max * a).
        /// </summary>
        public static decVector2 Mix(decimal min, decVector2 max, decVector2 a) => new decVector2(min * (1-a.x) + max.x * a.x, min * (1-a.y) + max.y * a.y);
        
        /// <summary>
        /// Returns a decVector2 from component-wise application of Mix (min * (1-a) + max * a).
        /// </summary>
        public static decVector2 Mix(decimal min, decVector2 max, decimal a) => new decVector2(min * (1-a) + max.x * a, min * (1-a) + max.y * a);
        
        /// <summary>
        /// Returns a decVector2 from component-wise application of Mix (min * (1-a) + max * a).
        /// </summary>
        public static decVector2 Mix(decimal min, decimal max, decVector2 a) => new decVector2(min * (1-a.x) + max * a.x, min * (1-a.y) + max * a.y);
        
        /// <summary>
        /// Returns a decVector from the application of Mix (min * (1-a) + max * a).
        /// </summary>
        public static decVector2 Mix(decimal min, decimal max, decimal a) => new decVector2(min * (1-a) + max * a);
        
        /// <summary>
        /// Returns a decVector2 from component-wise application of Lerp (min * (1-a) + max * a).
        /// </summary>
        public static decVector2 Lerp(decVector2 min, decVector2 max, decVector2 a) => new decVector2(min.x * (1-a.x) + max.x * a.x, min.y * (1-a.y) + max.y * a.y);
        
        /// <summary>
        /// Returns a decVector2 from component-wise application of Lerp (min * (1-a) + max * a).
        /// </summary>
        public static decVector2 Lerp(decVector2 min, decVector2 max, decimal a) => new decVector2(min.x * (1-a) + max.x * a, min.y * (1-a) + max.y * a);
        
        /// <summary>
        /// Returns a decVector2 from component-wise application of Lerp (min * (1-a) + max * a).
        /// </summary>
        public static decVector2 Lerp(decVector2 min, decimal max, decVector2 a) => new decVector2(min.x * (1-a.x) + max * a.x, min.y * (1-a.y) + max * a.y);
        
        /// <summary>
        /// Returns a decVector2 from component-wise application of Lerp (min * (1-a) + max * a).
        /// </summary>
        public static decVector2 Lerp(decVector2 min, decimal max, decimal a) => new decVector2(min.x * (1-a) + max * a, min.y * (1-a) + max * a);
        
        /// <summary>
        /// Returns a decVector2 from component-wise application of Lerp (min * (1-a) + max * a).
        /// </summary>
        public static decVector2 Lerp(decimal min, decVector2 max, decVector2 a) => new decVector2(min * (1-a.x) + max.x * a.x, min * (1-a.y) + max.y * a.y);
        
        /// <summary>
        /// Returns a decVector2 from component-wise application of Lerp (min * (1-a) + max * a).
        /// </summary>
        public static decVector2 Lerp(decimal min, decVector2 max, decimal a) => new decVector2(min * (1-a) + max.x * a, min * (1-a) + max.y * a);
        
        /// <summary>
        /// Returns a decVector2 from component-wise application of Lerp (min * (1-a) + max * a).
        /// </summary>
        public static decVector2 Lerp(decimal min, decimal max, decVector2 a) => new decVector2(min * (1-a.x) + max * a.x, min * (1-a.y) + max * a.y);
        
        /// <summary>
        /// Returns a decVector from the application of Lerp (min * (1-a) + max * a).
        /// </summary>
        public static decVector2 Lerp(decimal min, decimal max, decimal a) => new decVector2(min * (1-a) + max * a);
        
        /// <summary>
        /// Returns a decVector2 from component-wise application of Smoothstep (((v - edge0) / (edge1 - edge0)).Clamp().HermiteInterpolationOrder3()).
        /// </summary>
        public static decVector2 Smoothstep(decVector2 edge0, decVector2 edge1, decVector2 v) => new decVector2(((v.x - edge0.x) / (edge1.x - edge0.x)).Clamp().HermiteInterpolationOrder3(), ((v.y - edge0.y) / (edge1.y - edge0.y)).Clamp().HermiteInterpolationOrder3());
        
        /// <summary>
        /// Returns a decVector2 from component-wise application of Smoothstep (((v - edge0) / (edge1 - edge0)).Clamp().HermiteInterpolationOrder3()).
        /// </summary>
        public static decVector2 Smoothstep(decVector2 edge0, decVector2 edge1, decimal v) => new decVector2(((v - edge0.x) / (edge1.x - edge0.x)).Clamp().HermiteInterpolationOrder3(), ((v - edge0.y) / (edge1.y - edge0.y)).Clamp().HermiteInterpolationOrder3());
        
        /// <summary>
        /// Returns a decVector2 from component-wise application of Smoothstep (((v - edge0) / (edge1 - edge0)).Clamp().HermiteInterpolationOrder3()).
        /// </summary>
        public static decVector2 Smoothstep(decVector2 edge0, decimal edge1, decVector2 v) => new decVector2(((v.x - edge0.x) / (edge1 - edge0.x)).Clamp().HermiteInterpolationOrder3(), ((v.y - edge0.y) / (edge1 - edge0.y)).Clamp().HermiteInterpolationOrder3());
        
        /// <summary>
        /// Returns a decVector2 from component-wise application of Smoothstep (((v - edge0) / (edge1 - edge0)).Clamp().HermiteInterpolationOrder3()).
        /// </summary>
        public static decVector2 Smoothstep(decVector2 edge0, decimal edge1, decimal v) => new decVector2(((v - edge0.x) / (edge1 - edge0.x)).Clamp().HermiteInterpolationOrder3(), ((v - edge0.y) / (edge1 - edge0.y)).Clamp().HermiteInterpolationOrder3());
        
        /// <summary>
        /// Returns a decVector2 from component-wise application of Smoothstep (((v - edge0) / (edge1 - edge0)).Clamp().HermiteInterpolationOrder3()).
        /// </summary>
        public static decVector2 Smoothstep(decimal edge0, decVector2 edge1, decVector2 v) => new decVector2(((v.x - edge0) / (edge1.x - edge0)).Clamp().HermiteInterpolationOrder3(), ((v.y - edge0) / (edge1.y - edge0)).Clamp().HermiteInterpolationOrder3());
        
        /// <summary>
        /// Returns a decVector2 from component-wise application of Smoothstep (((v - edge0) / (edge1 - edge0)).Clamp().HermiteInterpolationOrder3()).
        /// </summary>
        public static decVector2 Smoothstep(decimal edge0, decVector2 edge1, decimal v) => new decVector2(((v - edge0) / (edge1.x - edge0)).Clamp().HermiteInterpolationOrder3(), ((v - edge0) / (edge1.y - edge0)).Clamp().HermiteInterpolationOrder3());
        
        /// <summary>
        /// Returns a decVector2 from component-wise application of Smoothstep (((v - edge0) / (edge1 - edge0)).Clamp().HermiteInterpolationOrder3()).
        /// </summary>
        public static decVector2 Smoothstep(decimal edge0, decimal edge1, decVector2 v) => new decVector2(((v.x - edge0) / (edge1 - edge0)).Clamp().HermiteInterpolationOrder3(), ((v.y - edge0) / (edge1 - edge0)).Clamp().HermiteInterpolationOrder3());
        
        /// <summary>
        /// Returns a decVector from the application of Smoothstep (((v - edge0) / (edge1 - edge0)).Clamp().HermiteInterpolationOrder3()).
        /// </summary>
        public static decVector2 Smoothstep(decimal edge0, decimal edge1, decimal v) => new decVector2(((v - edge0) / (edge1 - edge0)).Clamp().HermiteInterpolationOrder3());
        
        /// <summary>
        /// Returns a decVector2 from component-wise application of Smootherstep (((v - edge0) / (edge1 - edge0)).Clamp().HermiteInterpolationOrder5()).
        /// </summary>
        public static decVector2 Smootherstep(decVector2 edge0, decVector2 edge1, decVector2 v) => new decVector2(((v.x - edge0.x) / (edge1.x - edge0.x)).Clamp().HermiteInterpolationOrder5(), ((v.y - edge0.y) / (edge1.y - edge0.y)).Clamp().HermiteInterpolationOrder5());
        
        /// <summary>
        /// Returns a decVector2 from component-wise application of Smootherstep (((v - edge0) / (edge1 - edge0)).Clamp().HermiteInterpolationOrder5()).
        /// </summary>
        public static decVector2 Smootherstep(decVector2 edge0, decVector2 edge1, decimal v) => new decVector2(((v - edge0.x) / (edge1.x - edge0.x)).Clamp().HermiteInterpolationOrder5(), ((v - edge0.y) / (edge1.y - edge0.y)).Clamp().HermiteInterpolationOrder5());
        
        /// <summary>
        /// Returns a decVector2 from component-wise application of Smootherstep (((v - edge0) / (edge1 - edge0)).Clamp().HermiteInterpolationOrder5()).
        /// </summary>
        public static decVector2 Smootherstep(decVector2 edge0, decimal edge1, decVector2 v) => new decVector2(((v.x - edge0.x) / (edge1 - edge0.x)).Clamp().HermiteInterpolationOrder5(), ((v.y - edge0.y) / (edge1 - edge0.y)).Clamp().HermiteInterpolationOrder5());
        
        /// <summary>
        /// Returns a decVector2 from component-wise application of Smootherstep (((v - edge0) / (edge1 - edge0)).Clamp().HermiteInterpolationOrder5()).
        /// </summary>
        public static decVector2 Smootherstep(decVector2 edge0, decimal edge1, decimal v) => new decVector2(((v - edge0.x) / (edge1 - edge0.x)).Clamp().HermiteInterpolationOrder5(), ((v - edge0.y) / (edge1 - edge0.y)).Clamp().HermiteInterpolationOrder5());
        
        /// <summary>
        /// Returns a decVector2 from component-wise application of Smootherstep (((v - edge0) / (edge1 - edge0)).Clamp().HermiteInterpolationOrder5()).
        /// </summary>
        public static decVector2 Smootherstep(decimal edge0, decVector2 edge1, decVector2 v) => new decVector2(((v.x - edge0) / (edge1.x - edge0)).Clamp().HermiteInterpolationOrder5(), ((v.y - edge0) / (edge1.y - edge0)).Clamp().HermiteInterpolationOrder5());
        
        /// <summary>
        /// Returns a decVector2 from component-wise application of Smootherstep (((v - edge0) / (edge1 - edge0)).Clamp().HermiteInterpolationOrder5()).
        /// </summary>
        public static decVector2 Smootherstep(decimal edge0, decVector2 edge1, decimal v) => new decVector2(((v - edge0) / (edge1.x - edge0)).Clamp().HermiteInterpolationOrder5(), ((v - edge0) / (edge1.y - edge0)).Clamp().HermiteInterpolationOrder5());
        
        /// <summary>
        /// Returns a decVector2 from component-wise application of Smootherstep (((v - edge0) / (edge1 - edge0)).Clamp().HermiteInterpolationOrder5()).
        /// </summary>
        public static decVector2 Smootherstep(decimal edge0, decimal edge1, decVector2 v) => new decVector2(((v.x - edge0) / (edge1 - edge0)).Clamp().HermiteInterpolationOrder5(), ((v.y - edge0) / (edge1 - edge0)).Clamp().HermiteInterpolationOrder5());
        
        /// <summary>
        /// Returns a decVector from the application of Smootherstep (((v - edge0) / (edge1 - edge0)).Clamp().HermiteInterpolationOrder5()).
        /// </summary>
        public static decVector2 Smootherstep(decimal edge0, decimal edge1, decimal v) => new decVector2(((v - edge0) / (edge1 - edge0)).Clamp().HermiteInterpolationOrder5());
        
        /// <summary>
        /// Returns a decVector2 from component-wise application of Fma (a * b + c).
        /// </summary>
        public static decVector2 Fma(decVector2 a, decVector2 b, decVector2 c) => new decVector2(a.x * b.x + c.x, a.y * b.y + c.y);
        
        /// <summary>
        /// Returns a decVector2 from component-wise application of Fma (a * b + c).
        /// </summary>
        public static decVector2 Fma(decVector2 a, decVector2 b, decimal c) => new decVector2(a.x * b.x + c, a.y * b.y + c);
        
        /// <summary>
        /// Returns a decVector2 from component-wise application of Fma (a * b + c).
        /// </summary>
        public static decVector2 Fma(decVector2 a, decimal b, decVector2 c) => new decVector2(a.x * b + c.x, a.y * b + c.y);
        
        /// <summary>
        /// Returns a decVector2 from component-wise application of Fma (a * b + c).
        /// </summary>
        public static decVector2 Fma(decVector2 a, decimal b, decimal c) => new decVector2(a.x * b + c, a.y * b + c);
        
        /// <summary>
        /// Returns a decVector2 from component-wise application of Fma (a * b + c).
        /// </summary>
        public static decVector2 Fma(decimal a, decVector2 b, decVector2 c) => new decVector2(a * b.x + c.x, a * b.y + c.y);
        
        /// <summary>
        /// Returns a decVector2 from component-wise application of Fma (a * b + c).
        /// </summary>
        public static decVector2 Fma(decimal a, decVector2 b, decimal c) => new decVector2(a * b.x + c, a * b.y + c);
        
        /// <summary>
        /// Returns a decVector2 from component-wise application of Fma (a * b + c).
        /// </summary>
        public static decVector2 Fma(decimal a, decimal b, decVector2 c) => new decVector2(a * b + c.x, a * b + c.y);
        
        /// <summary>
        /// Returns a decVector from the application of Fma (a * b + c).
        /// </summary>
        public static decVector2 Fma(decimal a, decimal b, decimal c) => new decVector2(a * b + c);
        
        /// <summary>
        /// Returns a decVector2 from component-wise application of Add (lhs + rhs).
        /// </summary>
        public static decVector2 Add(decVector2 lhs, decVector2 rhs) => new decVector2(lhs.x + rhs.x, lhs.y + rhs.y);
        
        /// <summary>
        /// Returns a decVector2 from component-wise application of Add (lhs + rhs).
        /// </summary>
        public static decVector2 Add(decVector2 lhs, decimal rhs) => new decVector2(lhs.x + rhs, lhs.y + rhs);
        
        /// <summary>
        /// Returns a decVector2 from component-wise application of Add (lhs + rhs).
        /// </summary>
        public static decVector2 Add(decimal lhs, decVector2 rhs) => new decVector2(lhs + rhs.x, lhs + rhs.y);
        
        /// <summary>
        /// Returns a decVector from the application of Add (lhs + rhs).
        /// </summary>
        public static decVector2 Add(decimal lhs, decimal rhs) => new decVector2(lhs + rhs);
        
        /// <summary>
        /// Returns a decVector2 from component-wise application of Sub (lhs - rhs).
        /// </summary>
        public static decVector2 Sub(decVector2 lhs, decVector2 rhs) => new decVector2(lhs.x - rhs.x, lhs.y - rhs.y);
        
        /// <summary>
        /// Returns a decVector2 from component-wise application of Sub (lhs - rhs).
        /// </summary>
        public static decVector2 Sub(decVector2 lhs, decimal rhs) => new decVector2(lhs.x - rhs, lhs.y - rhs);
        
        /// <summary>
        /// Returns a decVector2 from component-wise application of Sub (lhs - rhs).
        /// </summary>
        public static decVector2 Sub(decimal lhs, decVector2 rhs) => new decVector2(lhs - rhs.x, lhs - rhs.y);
        
        /// <summary>
        /// Returns a decVector from the application of Sub (lhs - rhs).
        /// </summary>
        public static decVector2 Sub(decimal lhs, decimal rhs) => new decVector2(lhs - rhs);
        
        /// <summary>
        /// Returns a decVector2 from component-wise application of Mul (lhs * rhs).
        /// </summary>
        public static decVector2 Mul(decVector2 lhs, decVector2 rhs) => new decVector2(lhs.x * rhs.x, lhs.y * rhs.y);
        
        /// <summary>
        /// Returns a decVector2 from component-wise application of Mul (lhs * rhs).
        /// </summary>
        public static decVector2 Mul(decVector2 lhs, decimal rhs) => new decVector2(lhs.x * rhs, lhs.y * rhs);
        
        /// <summary>
        /// Returns a decVector2 from component-wise application of Mul (lhs * rhs).
        /// </summary>
        public static decVector2 Mul(decimal lhs, decVector2 rhs) => new decVector2(lhs * rhs.x, lhs * rhs.y);
        
        /// <summary>
        /// Returns a decVector from the application of Mul (lhs * rhs).
        /// </summary>
        public static decVector2 Mul(decimal lhs, decimal rhs) => new decVector2(lhs * rhs);
        
        /// <summary>
        /// Returns a decVector2 from component-wise application of Div (lhs / rhs).
        /// </summary>
        public static decVector2 Div(decVector2 lhs, decVector2 rhs) => new decVector2(lhs.x / rhs.x, lhs.y / rhs.y);
        
        /// <summary>
        /// Returns a decVector2 from component-wise application of Div (lhs / rhs).
        /// </summary>
        public static decVector2 Div(decVector2 lhs, decimal rhs) => new decVector2(lhs.x / rhs, lhs.y / rhs);
        
        /// <summary>
        /// Returns a decVector2 from component-wise application of Div (lhs / rhs).
        /// </summary>
        public static decVector2 Div(decimal lhs, decVector2 rhs) => new decVector2(lhs / rhs.x, lhs / rhs.y);
        
        /// <summary>
        /// Returns a decVector from the application of Div (lhs / rhs).
        /// </summary>
        public static decVector2 Div(decimal lhs, decimal rhs) => new decVector2(lhs / rhs);
        
        /// <summary>
        /// Returns a decVector2 from component-wise application of Modulo (lhs % rhs).
        /// </summary>
        public static decVector2 Modulo(decVector2 lhs, decVector2 rhs) => new decVector2(lhs.x % rhs.x, lhs.y % rhs.y);
        
        /// <summary>
        /// Returns a decVector2 from component-wise application of Modulo (lhs % rhs).
        /// </summary>
        public static decVector2 Modulo(decVector2 lhs, decimal rhs) => new decVector2(lhs.x % rhs, lhs.y % rhs);
        
        /// <summary>
        /// Returns a decVector2 from component-wise application of Modulo (lhs % rhs).
        /// </summary>
        public static decVector2 Modulo(decimal lhs, decVector2 rhs) => new decVector2(lhs % rhs.x, lhs % rhs.y);
        
        /// <summary>
        /// Returns a decVector from the application of Modulo (lhs % rhs).
        /// </summary>
        public static decVector2 Modulo(decimal lhs, decimal rhs) => new decVector2(lhs % rhs);
        
        /// <summary>
        /// Returns a decVector2 from component-wise application of Degrees (Radians-To-Degrees Conversion).
        /// </summary>
        public static decVector2 Degrees(decVector2 v) => new decVector2((decimal)(v.x * 57.295779513082320876798154814105170332405472466564321m), (decimal)(v.y * 57.295779513082320876798154814105170332405472466564321m));
        
        /// <summary>
        /// Returns a decVector from the application of Degrees (Radians-To-Degrees Conversion).
        /// </summary>
        public static decVector2 Degrees(decimal v) => new decVector2((decimal)(v * 57.295779513082320876798154814105170332405472466564321m));
        
        /// <summary>
        /// Returns a decVector2 from component-wise application of Radians (Degrees-To-Radians Conversion).
        /// </summary>
        public static decVector2 Radians(decVector2 v) => new decVector2((decimal)(v.x * 0.0174532925199432957692369076848861271344287188854172m), (decimal)(v.y * 0.0174532925199432957692369076848861271344287188854172m));
        
        /// <summary>
        /// Returns a decVector from the application of Radians (Degrees-To-Radians Conversion).
        /// </summary>
        public static decVector2 Radians(decimal v) => new decVector2((decimal)(v * 0.0174532925199432957692369076848861271344287188854172m));
        
        /// <summary>
        /// Returns a decVector2 from component-wise application of Acos ((decimal)Math.Acos((double)v)).
        /// </summary>
        public static decVector2 Acos(decVector2 v) => new decVector2((decimal)Math.Acos((double)v.x), (decimal)Math.Acos((double)v.y));
        
        /// <summary>
        /// Returns a decVector from the application of Acos ((decimal)Math.Acos((double)v)).
        /// </summary>
        public static decVector2 Acos(decimal v) => new decVector2((decimal)Math.Acos((double)v));
        
        /// <summary>
        /// Returns a decVector2 from component-wise application of Asin ((decimal)Math.Asin((double)v)).
        /// </summary>
        public static decVector2 Asin(decVector2 v) => new decVector2((decimal)Math.Asin((double)v.x), (decimal)Math.Asin((double)v.y));
        
        /// <summary>
        /// Returns a decVector from the application of Asin ((decimal)Math.Asin((double)v)).
        /// </summary>
        public static decVector2 Asin(decimal v) => new decVector2((decimal)Math.Asin((double)v));
        
        /// <summary>
        /// Returns a decVector2 from component-wise application of Atan ((decimal)Math.Atan((double)v)).
        /// </summary>
        public static decVector2 Atan(decVector2 v) => new decVector2((decimal)Math.Atan((double)v.x), (decimal)Math.Atan((double)v.y));
        
        /// <summary>
        /// Returns a decVector from the application of Atan ((decimal)Math.Atan((double)v)).
        /// </summary>
        public static decVector2 Atan(decimal v) => new decVector2((decimal)Math.Atan((double)v));
        
        /// <summary>
        /// Returns a decVector2 from component-wise application of Cos ((decimal)Math.Cos((double)v)).
        /// </summary>
        public static decVector2 Cos(decVector2 v) => new decVector2((decimal)Math.Cos((double)v.x), (decimal)Math.Cos((double)v.y));
        
        /// <summary>
        /// Returns a decVector from the application of Cos ((decimal)Math.Cos((double)v)).
        /// </summary>
        public static decVector2 Cos(decimal v) => new decVector2((decimal)Math.Cos((double)v));
        
        /// <summary>
        /// Returns a decVector2 from component-wise application of Cosh ((decimal)Math.Cosh((double)v)).
        /// </summary>
        public static decVector2 Cosh(decVector2 v) => new decVector2((decimal)Math.Cosh((double)v.x), (decimal)Math.Cosh((double)v.y));
        
        /// <summary>
        /// Returns a decVector from the application of Cosh ((decimal)Math.Cosh((double)v)).
        /// </summary>
        public static decVector2 Cosh(decimal v) => new decVector2((decimal)Math.Cosh((double)v));
        
        /// <summary>
        /// Returns a decVector2 from component-wise application of Exp ((decimal)Math.Exp((double)v)).
        /// </summary>
        public static decVector2 Exp(decVector2 v) => new decVector2((decimal)Math.Exp((double)v.x), (decimal)Math.Exp((double)v.y));
        
        /// <summary>
        /// Returns a decVector from the application of Exp ((decimal)Math.Exp((double)v)).
        /// </summary>
        public static decVector2 Exp(decimal v) => new decVector2((decimal)Math.Exp((double)v));
        
        /// <summary>
        /// Returns a decVector2 from component-wise application of Log ((decimal)Math.Log((double)v)).
        /// </summary>
        public static decVector2 Log(decVector2 v) => new decVector2((decimal)Math.Log((double)v.x), (decimal)Math.Log((double)v.y));
        
        /// <summary>
        /// Returns a decVector from the application of Log ((decimal)Math.Log((double)v)).
        /// </summary>
        public static decVector2 Log(decimal v) => new decVector2((decimal)Math.Log((double)v));
        
        /// <summary>
        /// Returns a decVector2 from component-wise application of Log2 ((decimal)Math.Log((double)v, 2)).
        /// </summary>
        public static decVector2 Log2(decVector2 v) => new decVector2((decimal)Math.Log((double)v.x, 2), (decimal)Math.Log((double)v.y, 2));
        
        /// <summary>
        /// Returns a decVector from the application of Log2 ((decimal)Math.Log((double)v, 2)).
        /// </summary>
        public static decVector2 Log2(decimal v) => new decVector2((decimal)Math.Log((double)v, 2));
        
        /// <summary>
        /// Returns a decVector2 from component-wise application of Log10 ((decimal)Math.Log10((double)v)).
        /// </summary>
        public static decVector2 Log10(decVector2 v) => new decVector2((decimal)Math.Log10((double)v.x), (decimal)Math.Log10((double)v.y));
        
        /// <summary>
        /// Returns a decVector from the application of Log10 ((decimal)Math.Log10((double)v)).
        /// </summary>
        public static decVector2 Log10(decimal v) => new decVector2((decimal)Math.Log10((double)v));
        
        /// <summary>
        /// Returns a decVector2 from component-wise application of Floor ((decimal)Math.Floor(v)).
        /// </summary>
        public static decVector2 Floor(decVector2 v) => new decVector2((decimal)Math.Floor(v.x), (decimal)Math.Floor(v.y));
        
        /// <summary>
        /// Returns a decVector from the application of Floor ((decimal)Math.Floor(v)).
        /// </summary>
        public static decVector2 Floor(decimal v) => new decVector2((decimal)Math.Floor(v));
        
        /// <summary>
        /// Returns a decVector2 from component-wise application of Ceiling ((decimal)Math.Ceiling(v)).
        /// </summary>
        public static decVector2 Ceiling(decVector2 v) => new decVector2((decimal)Math.Ceiling(v.x), (decimal)Math.Ceiling(v.y));
        
        /// <summary>
        /// Returns a decVector from the application of Ceiling ((decimal)Math.Ceiling(v)).
        /// </summary>
        public static decVector2 Ceiling(decimal v) => new decVector2((decimal)Math.Ceiling(v));
        
        /// <summary>
        /// Returns a decVector2 from component-wise application of Round ((decimal)Math.Round(v)).
        /// </summary>
        public static decVector2 Round(decVector2 v) => new decVector2((decimal)Math.Round(v.x), (decimal)Math.Round(v.y));
        
        /// <summary>
        /// Returns a decVector from the application of Round ((decimal)Math.Round(v)).
        /// </summary>
        public static decVector2 Round(decimal v) => new decVector2((decimal)Math.Round(v));
        
        /// <summary>
        /// Returns a decVector2 from component-wise application of Sin ((decimal)Math.Sin((double)v)).
        /// </summary>
        public static decVector2 Sin(decVector2 v) => new decVector2((decimal)Math.Sin((double)v.x), (decimal)Math.Sin((double)v.y));
        
        /// <summary>
        /// Returns a decVector from the application of Sin ((decimal)Math.Sin((double)v)).
        /// </summary>
        public static decVector2 Sin(decimal v) => new decVector2((decimal)Math.Sin((double)v));
        
        /// <summary>
        /// Returns a decVector2 from component-wise application of Sinh ((decimal)Math.Sinh((double)v)).
        /// </summary>
        public static decVector2 Sinh(decVector2 v) => new decVector2((decimal)Math.Sinh((double)v.x), (decimal)Math.Sinh((double)v.y));
        
        /// <summary>
        /// Returns a decVector from the application of Sinh ((decimal)Math.Sinh((double)v)).
        /// </summary>
        public static decVector2 Sinh(decimal v) => new decVector2((decimal)Math.Sinh((double)v));
        
        /// <summary>
        /// Returns a decVector2 from component-wise application of Tan ((decimal)Math.Tan((double)v)).
        /// </summary>
        public static decVector2 Tan(decVector2 v) => new decVector2((decimal)Math.Tan((double)v.x), (decimal)Math.Tan((double)v.y));
        
        /// <summary>
        /// Returns a decVector from the application of Tan ((decimal)Math.Tan((double)v)).
        /// </summary>
        public static decVector2 Tan(decimal v) => new decVector2((decimal)Math.Tan((double)v));
        
        /// <summary>
        /// Returns a decVector2 from component-wise application of Tanh ((decimal)Math.Tanh((double)v)).
        /// </summary>
        public static decVector2 Tanh(decVector2 v) => new decVector2((decimal)Math.Tanh((double)v.x), (decimal)Math.Tanh((double)v.y));
        
        /// <summary>
        /// Returns a decVector from the application of Tanh ((decimal)Math.Tanh((double)v)).
        /// </summary>
        public static decVector2 Tanh(decimal v) => new decVector2((decimal)Math.Tanh((double)v));
        
        /// <summary>
        /// Returns a decVector2 from component-wise application of Truncate ((decimal)Math.Truncate((double)v)).
        /// </summary>
        public static decVector2 Truncate(decVector2 v) => new decVector2((decimal)Math.Truncate((double)v.x), (decimal)Math.Truncate((double)v.y));
        
        /// <summary>
        /// Returns a decVector from the application of Truncate ((decimal)Math.Truncate((double)v)).
        /// </summary>
        public static decVector2 Truncate(decimal v) => new decVector2((decimal)Math.Truncate((double)v));
        
        /// <summary>
        /// Returns a decVector2 from component-wise application of Fract ((decimal)(v - Math.Floor(v))).
        /// </summary>
        public static decVector2 Fract(decVector2 v) => new decVector2((decimal)(v.x - Math.Floor(v.x)), (decimal)(v.y - Math.Floor(v.y)));
        
        /// <summary>
        /// Returns a decVector from the application of Fract ((decimal)(v - Math.Floor(v))).
        /// </summary>
        public static decVector2 Fract(decimal v) => new decVector2((decimal)(v - Math.Floor(v)));
        
        /// <summary>
        /// Returns a decVector2 from component-wise application of Trunc ((long)(v)).
        /// </summary>
        public static decVector2 Trunc(decVector2 v) => new decVector2((long)(v.x), (long)(v.y));
        
        /// <summary>
        /// Returns a decVector from the application of Trunc ((long)(v)).
        /// </summary>
        public static decVector2 Trunc(decimal v) => new decVector2((long)(v));
        
        /// <summary>
        /// Returns a decVector2 with independent and identically distributed uniform values between 'minValue' and 'maxValue'.
        /// </summary>
        public static decVector2 Random(System.Random random, decVector2 minValue, decVector2 maxValue) => new decVector2((decimal)random.NextDouble() * (maxValue.x - minValue.x) + minValue.x, (decimal)random.NextDouble() * (maxValue.y - minValue.y) + minValue.y);
        
        /// <summary>
        /// Returns a decVector2 with independent and identically distributed uniform values between 'minValue' and 'maxValue'.
        /// </summary>
        public static decVector2 Random(System.Random random, decVector2 minValue, decimal maxValue) => new decVector2((decimal)random.NextDouble() * (maxValue - minValue.x) + minValue.x, (decimal)random.NextDouble() * (maxValue - minValue.y) + minValue.y);
        
        /// <summary>
        /// Returns a decVector2 with independent and identically distributed uniform values between 'minValue' and 'maxValue'.
        /// </summary>
        public static decVector2 Random(System.Random random, decimal minValue, decVector2 maxValue) => new decVector2((decimal)random.NextDouble() * (maxValue.x - minValue) + minValue, (decimal)random.NextDouble() * (maxValue.y - minValue) + minValue);
        
        /// <summary>
        /// Returns a decVector2 with independent and identically distributed uniform values between 'minValue' and 'maxValue'.
        /// </summary>
        public static decVector2 Random(System.Random random, decimal minValue, decimal maxValue) => new decVector2((decimal)random.NextDouble() * (maxValue - minValue) + minValue);
        
        /// <summary>
        /// Returns a decVector2 with independent and identically distributed uniform values between 'minValue' and 'maxValue'.
        /// </summary>
        public static decVector2 RandomUniform(System.Random random, decVector2 minValue, decVector2 maxValue) => new decVector2((decimal)random.NextDouble() * (maxValue.x - minValue.x) + minValue.x, (decimal)random.NextDouble() * (maxValue.y - minValue.y) + minValue.y);
        
        /// <summary>
        /// Returns a decVector2 with independent and identically distributed uniform values between 'minValue' and 'maxValue'.
        /// </summary>
        public static decVector2 RandomUniform(System.Random random, decVector2 minValue, decimal maxValue) => new decVector2((decimal)random.NextDouble() * (maxValue - minValue.x) + minValue.x, (decimal)random.NextDouble() * (maxValue - minValue.y) + minValue.y);
        
        /// <summary>
        /// Returns a decVector2 with independent and identically distributed uniform values between 'minValue' and 'maxValue'.
        /// </summary>
        public static decVector2 RandomUniform(System.Random random, decimal minValue, decVector2 maxValue) => new decVector2((decimal)random.NextDouble() * (maxValue.x - minValue) + minValue, (decimal)random.NextDouble() * (maxValue.y - minValue) + minValue);
        
        /// <summary>
        /// Returns a decVector2 with independent and identically distributed uniform values between 'minValue' and 'maxValue'.
        /// </summary>
        public static decVector2 RandomUniform(System.Random random, decimal minValue, decimal maxValue) => new decVector2((decimal)random.NextDouble() * (maxValue - minValue) + minValue);
        
        /// <summary>
        /// Returns a decVector2 with independent and identically distributed values according to a normal/Gaussian distribution with specified mean and variance.
        /// </summary>
        public static decVector2 RandomNormal(System.Random random, decVector2 mean, decVector2 variance) => new decVector2((decimal)(Math.Sqrt((double)variance.x) * Math.Cos(2 * Math.PI * random.NextDouble()) * Math.Sqrt(-2.0 * Math.Log(random.NextDouble()))) + mean.x, (decimal)(Math.Sqrt((double)variance.y) * Math.Cos(2 * Math.PI * random.NextDouble()) * Math.Sqrt(-2.0 * Math.Log(random.NextDouble()))) + mean.y);
        
        /// <summary>
        /// Returns a decVector2 with independent and identically distributed values according to a normal/Gaussian distribution with specified mean and variance.
        /// </summary>
        public static decVector2 RandomNormal(System.Random random, decVector2 mean, decimal variance) => new decVector2((decimal)(Math.Sqrt((double)variance) * Math.Cos(2 * Math.PI * random.NextDouble()) * Math.Sqrt(-2.0 * Math.Log(random.NextDouble()))) + mean.x, (decimal)(Math.Sqrt((double)variance) * Math.Cos(2 * Math.PI * random.NextDouble()) * Math.Sqrt(-2.0 * Math.Log(random.NextDouble()))) + mean.y);
        
        /// <summary>
        /// Returns a decVector2 with independent and identically distributed values according to a normal/Gaussian distribution with specified mean and variance.
        /// </summary>
        public static decVector2 RandomNormal(System.Random random, decimal mean, decVector2 variance) => new decVector2((decimal)(Math.Sqrt((double)variance.x) * Math.Cos(2 * Math.PI * random.NextDouble()) * Math.Sqrt(-2.0 * Math.Log(random.NextDouble()))) + mean, (decimal)(Math.Sqrt((double)variance.y) * Math.Cos(2 * Math.PI * random.NextDouble()) * Math.Sqrt(-2.0 * Math.Log(random.NextDouble()))) + mean);
        
        /// <summary>
        /// Returns a decVector2 with independent and identically distributed values according to a normal/Gaussian distribution with specified mean and variance.
        /// </summary>
        public static decVector2 RandomNormal(System.Random random, decimal mean, decimal variance) => new decVector2((decimal)(Math.Sqrt((double)variance) * Math.Cos(2 * Math.PI * random.NextDouble()) * Math.Sqrt(-2.0 * Math.Log(random.NextDouble()))) + mean);
        
        /// <summary>
        /// Returns a decVector2 with independent and identically distributed values according to a normal/Gaussian distribution with specified mean and variance.
        /// </summary>
        public static decVector2 RandomGaussian(System.Random random, decVector2 mean, decVector2 variance) => new decVector2((decimal)(Math.Sqrt((double)variance.x) * Math.Cos(2 * Math.PI * random.NextDouble()) * Math.Sqrt(-2.0 * Math.Log(random.NextDouble()))) + mean.x, (decimal)(Math.Sqrt((double)variance.y) * Math.Cos(2 * Math.PI * random.NextDouble()) * Math.Sqrt(-2.0 * Math.Log(random.NextDouble()))) + mean.y);
        
        /// <summary>
        /// Returns a decVector2 with independent and identically distributed values according to a normal/Gaussian distribution with specified mean and variance.
        /// </summary>
        public static decVector2 RandomGaussian(System.Random random, decVector2 mean, decimal variance) => new decVector2((decimal)(Math.Sqrt((double)variance) * Math.Cos(2 * Math.PI * random.NextDouble()) * Math.Sqrt(-2.0 * Math.Log(random.NextDouble()))) + mean.x, (decimal)(Math.Sqrt((double)variance) * Math.Cos(2 * Math.PI * random.NextDouble()) * Math.Sqrt(-2.0 * Math.Log(random.NextDouble()))) + mean.y);
        
        /// <summary>
        /// Returns a decVector2 with independent and identically distributed values according to a normal/Gaussian distribution with specified mean and variance.
        /// </summary>
        public static decVector2 RandomGaussian(System.Random random, decimal mean, decVector2 variance) => new decVector2((decimal)(Math.Sqrt((double)variance.x) * Math.Cos(2 * Math.PI * random.NextDouble()) * Math.Sqrt(-2.0 * Math.Log(random.NextDouble()))) + mean, (decimal)(Math.Sqrt((double)variance.y) * Math.Cos(2 * Math.PI * random.NextDouble()) * Math.Sqrt(-2.0 * Math.Log(random.NextDouble()))) + mean);
        
        /// <summary>
        /// Returns a decVector2 with independent and identically distributed values according to a normal/Gaussian distribution with specified mean and variance.
        /// </summary>
        public static decVector2 RandomGaussian(System.Random random, decimal mean, decimal variance) => new decVector2((decimal)(Math.Sqrt((double)variance) * Math.Cos(2 * Math.PI * random.NextDouble()) * Math.Sqrt(-2.0 * Math.Log(random.NextDouble()))) + mean);

        #endregion


        #region Component-Wise Operator Overloads
        
        /// <summary>
        /// Returns a bVector2 from component-wise application of operator&lt; (lhs &lt; rhs).
        /// </summary>
        public static bVector2 operator<(decVector2 lhs, decVector2 rhs) => new bVector2(lhs.x < rhs.x, lhs.y < rhs.y);
        
        /// <summary>
        /// Returns a bVector2 from component-wise application of operator&lt; (lhs &lt; rhs).
        /// </summary>
        public static bVector2 operator<(decVector2 lhs, decimal rhs) => new bVector2(lhs.x < rhs, lhs.y < rhs);
        
        /// <summary>
        /// Returns a bVector2 from component-wise application of operator&lt; (lhs &lt; rhs).
        /// </summary>
        public static bVector2 operator<(decimal lhs, decVector2 rhs) => new bVector2(lhs < rhs.x, lhs < rhs.y);
        
        /// <summary>
        /// Returns a bVector2 from component-wise application of operator&lt;= (lhs &lt;= rhs).
        /// </summary>
        public static bVector2 operator<=(decVector2 lhs, decVector2 rhs) => new bVector2(lhs.x <= rhs.x, lhs.y <= rhs.y);
        
        /// <summary>
        /// Returns a bVector2 from component-wise application of operator&lt;= (lhs &lt;= rhs).
        /// </summary>
        public static bVector2 operator<=(decVector2 lhs, decimal rhs) => new bVector2(lhs.x <= rhs, lhs.y <= rhs);
        
        /// <summary>
        /// Returns a bVector2 from component-wise application of operator&lt;= (lhs &lt;= rhs).
        /// </summary>
        public static bVector2 operator<=(decimal lhs, decVector2 rhs) => new bVector2(lhs <= rhs.x, lhs <= rhs.y);
        
        /// <summary>
        /// Returns a bVector2 from component-wise application of operator&gt; (lhs &gt; rhs).
        /// </summary>
        public static bVector2 operator>(decVector2 lhs, decVector2 rhs) => new bVector2(lhs.x > rhs.x, lhs.y > rhs.y);
        
        /// <summary>
        /// Returns a bVector2 from component-wise application of operator&gt; (lhs &gt; rhs).
        /// </summary>
        public static bVector2 operator>(decVector2 lhs, decimal rhs) => new bVector2(lhs.x > rhs, lhs.y > rhs);
        
        /// <summary>
        /// Returns a bVector2 from component-wise application of operator&gt; (lhs &gt; rhs).
        /// </summary>
        public static bVector2 operator>(decimal lhs, decVector2 rhs) => new bVector2(lhs > rhs.x, lhs > rhs.y);
        
        /// <summary>
        /// Returns a bVector2 from component-wise application of operator&gt;= (lhs &gt;= rhs).
        /// </summary>
        public static bVector2 operator>=(decVector2 lhs, decVector2 rhs) => new bVector2(lhs.x >= rhs.x, lhs.y >= rhs.y);
        
        /// <summary>
        /// Returns a bVector2 from component-wise application of operator&gt;= (lhs &gt;= rhs).
        /// </summary>
        public static bVector2 operator>=(decVector2 lhs, decimal rhs) => new bVector2(lhs.x >= rhs, lhs.y >= rhs);
        
        /// <summary>
        /// Returns a bVector2 from component-wise application of operator&gt;= (lhs &gt;= rhs).
        /// </summary>
        public static bVector2 operator>=(decimal lhs, decVector2 rhs) => new bVector2(lhs >= rhs.x, lhs >= rhs.y);
        
        /// <summary>
        /// Returns a decVector2 from component-wise application of operator+ (lhs + rhs).
        /// </summary>
        public static decVector2 operator+(decVector2 lhs, decVector2 rhs) => new decVector2(lhs.x + rhs.x, lhs.y + rhs.y);
        
        /// <summary>
        /// Returns a decVector2 from component-wise application of operator+ (lhs + rhs).
        /// </summary>
        public static decVector2 operator+(decVector2 lhs, decimal rhs) => new decVector2(lhs.x + rhs, lhs.y + rhs);
        
        /// <summary>
        /// Returns a decVector2 from component-wise application of operator+ (lhs + rhs).
        /// </summary>
        public static decVector2 operator+(decimal lhs, decVector2 rhs) => new decVector2(lhs + rhs.x, lhs + rhs.y);
        
        /// <summary>
        /// Returns a decVector2 from component-wise application of operator- (lhs - rhs).
        /// </summary>
        public static decVector2 operator-(decVector2 lhs, decVector2 rhs) => new decVector2(lhs.x - rhs.x, lhs.y - rhs.y);
        
        /// <summary>
        /// Returns a decVector2 from component-wise application of operator- (lhs - rhs).
        /// </summary>
        public static decVector2 operator-(decVector2 lhs, decimal rhs) => new decVector2(lhs.x - rhs, lhs.y - rhs);
        
        /// <summary>
        /// Returns a decVector2 from component-wise application of operator- (lhs - rhs).
        /// </summary>
        public static decVector2 operator-(decimal lhs, decVector2 rhs) => new decVector2(lhs - rhs.x, lhs - rhs.y);
        
        /// <summary>
        /// Returns a decVector2 from component-wise application of operator* (lhs * rhs).
        /// </summary>
        public static decVector2 operator*(decVector2 lhs, decVector2 rhs) => new decVector2(lhs.x * rhs.x, lhs.y * rhs.y);
        
        /// <summary>
        /// Returns a decVector2 from component-wise application of operator* (lhs * rhs).
        /// </summary>
        public static decVector2 operator*(decVector2 lhs, decimal rhs) => new decVector2(lhs.x * rhs, lhs.y * rhs);
        
        /// <summary>
        /// Returns a decVector2 from component-wise application of operator* (lhs * rhs).
        /// </summary>
        public static decVector2 operator*(decimal lhs, decVector2 rhs) => new decVector2(lhs * rhs.x, lhs * rhs.y);
        
        /// <summary>
        /// Returns a decVector2 from component-wise application of operator/ (lhs / rhs).
        /// </summary>
        public static decVector2 operator/(decVector2 lhs, decVector2 rhs) => new decVector2(lhs.x / rhs.x, lhs.y / rhs.y);
        
        /// <summary>
        /// Returns a decVector2 from component-wise application of operator/ (lhs / rhs).
        /// </summary>
        public static decVector2 operator/(decVector2 lhs, decimal rhs) => new decVector2(lhs.x / rhs, lhs.y / rhs);
        
        /// <summary>
        /// Returns a decVector2 from component-wise application of operator/ (lhs / rhs).
        /// </summary>
        public static decVector2 operator/(decimal lhs, decVector2 rhs) => new decVector2(lhs / rhs.x, lhs / rhs.y);
        
        /// <summary>
        /// Returns a decVector2 from component-wise application of operator+ (identity).
        /// </summary>
        public static decVector2 operator+(decVector2 v) => v;
        
        /// <summary>
        /// Returns a decVector2 from component-wise application of operator- (-v).
        /// </summary>
        public static decVector2 operator-(decVector2 v) => new decVector2(-v.x, -v.y);
        
        /// <summary>
        /// Returns a decVector2 from component-wise application of operator% (lhs % rhs).
        /// </summary>
        public static decVector2 operator%(decVector2 lhs, decVector2 rhs) => new decVector2(lhs.x % rhs.x, lhs.y % rhs.y);
        
        /// <summary>
        /// Returns a decVector2 from component-wise application of operator% (lhs % rhs).
        /// </summary>
        public static decVector2 operator%(decVector2 lhs, decimal rhs) => new decVector2(lhs.x % rhs, lhs.y % rhs);
        
        /// <summary>
        /// Returns a decVector2 from component-wise application of operator% (lhs % rhs).
        /// </summary>
        public static decVector2 operator%(decimal lhs, decVector2 rhs) => new decVector2(lhs % rhs.x, lhs % rhs.y);

        #endregion

    }
}
