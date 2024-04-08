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
    /// A Vector of type uint with 2 components.
    /// </summary>
    [RegisterCoreClassStruct]
    
    [StructLayout(LayoutKind.Sequential)]
    public struct uVector2 : IReadOnlyList<uint>, IEquatable<uVector2>
    {

        #region Fields
        
        /// <summary>
        /// x-component
        /// </summary>
        
        public uint x;
        
        /// <summary>
        /// y-component
        /// </summary>
        
        public uint y;

        #endregion


        #region Constructors
        
        /// <summary>
        /// Component-wise constructor
        /// </summary>
        public uVector2(uint x, uint y)
        {
            this.x = x;
            this.y = y;
        }
        
        /// <summary>
        /// all-same-value constructor
        /// </summary>
        public uVector2(uint v)
        {
            this.x = v;
            this.y = v;
        }
        
        /// <summary>
        /// from-Vector constructor
        /// </summary>
        public uVector2(uVector2 v)
        {
            this.x = v.x;
            this.y = v.y;
        }
        
        /// <summary>
        /// from-Vector constructor (additional fields are truncated)
        /// </summary>
        public uVector2(uVector3 v)
        {
            this.x = v.x;
            this.y = v.y;
        }
        
        /// <summary>
        /// from-Vector constructor (additional fields are truncated)
        /// </summary>
        public uVector2(uVector4 v)
        {
            this.x = v.x;
            this.y = v.y;
        }
        
        /// <summary>
        /// From-array/list constructor (superfluous values are ignored, missing values are zero-filled).
        /// </summary>
        public uVector2(IReadOnlyList<uint> v)
        {
            var c = v.Count;
            this.x = c < 0 ? 0u : v[0];
            this.y = c < 1 ? 0u : v[1];
        }
        
        /// <summary>
        /// Generic from-array constructor (superfluous values are ignored, missing values are zero-filled).
        /// </summary>
        public uVector2(Object[] v)
        {
            var c = v.Length;
            this.x = c < 0 ? 0u : (uint)v[0];
            this.y = c < 1 ? 0u : (uint)v[1];
        }
        
        /// <summary>
        /// From-array constructor (superfluous values are ignored, missing values are zero-filled).
        /// </summary>
        public uVector2(uint[] v)
        {
            var c = v.Length;
            this.x = c < 0 ? 0u : v[0];
            this.y = c < 1 ? 0u : v[1];
        }
        
        /// <summary>
        /// From-array constructor with base index (superfluous values are ignored, missing values are zero-filled).
        /// </summary>
        public uVector2(uint[] v, int startIndex)
        {
            var c = v.Length;
            this.x = c + startIndex < 0 ? 0u : v[0 + startIndex];
            this.y = c + startIndex < 1 ? 0u : v[1 + startIndex];
        }
        
        /// <summary>
        /// From-IEnumerable constructor (superfluous values are ignored, missing values are zero-filled).
        /// </summary>
        public uVector2(IEnumerable<uint> v)
            : this(v.ToArray())
        {
        }

        #endregion


        #region Implicit Operators
        
        /// <summary>
        /// Implicitly converts this to a lVector2.
        /// </summary>
        public static implicit operator lVector2(uVector2 v) => new lVector2((long)v.x, (long)v.y);
        
        /// <summary>
        /// Implicitly converts this to a Vector2.
        /// </summary>
        public static implicit operator Vector2(uVector2 v) => new Vector2((float)v.x, (float)v.y);
        
        /// <summary>
        /// Implicitly converts this to a hVector2.
        /// </summary>
        public static implicit operator hVector2(uVector2 v) => new hVector2((Half)v.x, (Half)v.y);
        
        /// <summary>
        /// Implicitly converts this to a dVector2.
        /// </summary>
        public static implicit operator dVector2(uVector2 v) => new dVector2((double)v.x, (double)v.y);
        
        /// <summary>
        /// Implicitly converts this to a decVector2.
        /// </summary>
        public static implicit operator decVector2(uVector2 v) => new decVector2((decimal)v.x, (decimal)v.y);
        
        /// <summary>
        /// Implicitly converts this to a cVector2.
        /// </summary>
        //public static implicit operator cVector2(uVector2 v) => new cVector2((Complex)v.x, (Complex)v.y);

        #endregion


        #region Explicit Operators
        
        /// <summary>
        /// Explicitly converts this to a iVector2.
        /// </summary>
        public static explicit operator iVector2(uVector2 v) => new iVector2((int)v.x, (int)v.y);
        
        /// <summary>
        /// Explicitly converts this to a iVector3. (Higher components are zeroed)
        /// </summary>
        public static explicit operator iVector3(uVector2 v) => new iVector3((int)v.x, (int)v.y, 0);
        
        /// <summary>
        /// Explicitly converts this to a iVector4. (Higher components are zeroed)
        /// </summary>
        public static explicit operator iVector4(uVector2 v) => new iVector4((int)v.x, (int)v.y, 0, 0);
        
        /// <summary>
        /// Explicitly converts this to a uVector3. (Higher components are zeroed)
        /// </summary>
        public static explicit operator uVector3(uVector2 v) => new uVector3((uint)v.x, (uint)v.y, 0u);
        
        /// <summary>
        /// Explicitly converts this to a uVector4. (Higher components are zeroed)
        /// </summary>
        public static explicit operator uVector4(uVector2 v) => new uVector4((uint)v.x, (uint)v.y, 0u, 0u);
        
        /// <summary>
        /// Explicitly converts this to a Vector3. (Higher components are zeroed)
        /// </summary>
        public static explicit operator Vector3(uVector2 v) => new Vector3((float)v.x, (float)v.y, 0f);
        
        /// <summary>
        /// Explicitly converts this to a Vector4. (Higher components are zeroed)
        /// </summary>
        public static explicit operator Vector4(uVector2 v) => new Vector4((float)v.x, (float)v.y, 0f, 0f);
        
        /// <summary>
        /// Explicitly converts this to a hVector3. (Higher components are zeroed)
        /// </summary>
        public static explicit operator hVector3(uVector2 v) => new hVector3((Half)v.x, (Half)v.y, Half.Zero);
        
        /// <summary>
        /// Explicitly converts this to a hVector4. (Higher components are zeroed)
        /// </summary>
        public static explicit operator hVector4(uVector2 v) => new hVector4((Half)v.x, (Half)v.y, Half.Zero, Half.Zero);
        
        /// <summary>
        /// Explicitly converts this to a dVector3. (Higher components are zeroed)
        /// </summary>
        public static explicit operator dVector3(uVector2 v) => new dVector3((double)v.x, (double)v.y, 0.0);
        
        /// <summary>
        /// Explicitly converts this to a dVector4. (Higher components are zeroed)
        /// </summary>
        public static explicit operator dVector4(uVector2 v) => new dVector4((double)v.x, (double)v.y, 0.0, 0.0);
        
        /// <summary>
        /// Explicitly converts this to a decVector3. (Higher components are zeroed)
        /// </summary>
        public static explicit operator decVector3(uVector2 v) => new decVector3((decimal)v.x, (decimal)v.y, 0m);
        
        /// <summary>
        /// Explicitly converts this to a decVector4. (Higher components are zeroed)
        /// </summary>
        public static explicit operator decVector4(uVector2 v) => new decVector4((decimal)v.x, (decimal)v.y, 0m, 0m);
        
        /// <summary>
        /// Explicitly converts this to a cVector3. (Higher components are zeroed)
        /// </summary>
        //public static explicit operator cVector3(uVector2 v) => new cVector3((Complex)v.x, (Complex)v.y, Complex.Zero);
        
        /// <summary>
        /// Explicitly converts this to a cVector4. (Higher components are zeroed)
        /// </summary>
        //public static explicit operator cVector4(uVector2 v) => new cVector4((Complex)v.x, (Complex)v.y, Complex.Zero, Complex.Zero);
        
        /// <summary>
        /// Explicitly converts this to a lVector3. (Higher components are zeroed)
        /// </summary>
        public static explicit operator lVector3(uVector2 v) => new lVector3((long)v.x, (long)v.y, 0);
        
        /// <summary>
        /// Explicitly converts this to a lVector4. (Higher components are zeroed)
        /// </summary>
        public static explicit operator lVector4(uVector2 v) => new lVector4((long)v.x, (long)v.y, 0, 0);
        
        /// <summary>
        /// Explicitly converts this to a bVector2.
        /// </summary>
        public static explicit operator bVector2(uVector2 v) => new bVector2(v.x != 0u, v.y != 0u);
        
        /// <summary>
        /// Explicitly converts this to a bVector3. (Higher components are zeroed)
        /// </summary>
        public static explicit operator bVector3(uVector2 v) => new bVector3(v.x != 0u, v.y != 0u, false);
        
        /// <summary>
        /// Explicitly converts this to a bVector4. (Higher components are zeroed)
        /// </summary>
        public static explicit operator bVector4(uVector2 v) => new bVector4(v.x != 0u, v.y != 0u, false, false);
        
        /// <summary>
        /// Explicitly converts this to a uint array.
        /// </summary>
        public static explicit operator uint[](uVector2 v) => new [] { v.x, v.y };
        
        /// <summary>
        /// Explicitly converts this to a generic object array.
        /// </summary>
        public static explicit operator Object[](uVector2 v) => new Object[] { v.x, v.y };

        #endregion


        #region Indexer
        
        /// <summary>
        /// Gets/Sets a specific indexed component (a bit slower than direct access).
        /// </summary>
        public uint this[int index]
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
        public swizzle_uVector2 swizzle => new swizzle_uVector2(x, y);
        
        /// <summary>
        /// Gets or sets the specified subset of components. For more advanced (read-only) swizzling, use the .swizzle property.
        /// </summary>
        public uVector2 xy
        {
            get
            {
                return new uVector2(x, y);
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
        public uVector2 rg
        {
            get
            {
                return new uVector2(x, y);
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
        public uint r
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
        public uint g
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
        public uint[] Values => new[] { x, y };
        
        /// <summary>
        /// Returns the number of components (2).
        /// </summary>
        public int Count => 2;
        
        /// <summary>
        /// Returns the minimal component of this Vector.
        /// </summary>
        public uint MinElement => Math.Min(x, y);
        
        /// <summary>
        /// Returns the maximal component of this Vector.
        /// </summary>
        public uint MaxElement => Math.Max(x, y);
        
        /// <summary>
        /// Returns the euclidean length of this Vector.
        /// </summary>
        public float Length => (float)Math.Sqrt((x*x + y*y));
        
        /// <summary>
        /// Returns the squared euclidean length of this Vector.
        /// </summary>
        public float LengthSqr => (x*x + y*y);
        
        /// <summary>
        /// Returns the sum of all components.
        /// </summary>
        public uint Sum => (x + y);
        
        /// <summary>
        /// Returns the euclidean norm of this Vector.
        /// </summary>
        public float Norm => (float)Math.Sqrt((x*x + y*y));
        
        /// <summary>
        /// Returns the one-norm of this Vector.
        /// </summary>
        public float Norm1 => (x + y);
        
        /// <summary>
        /// Returns the two-norm (euclidean length) of this Vector.
        /// </summary>
        public float Norm2 => (float)Math.Sqrt((x*x + y*y));
        
        /// <summary>
        /// Returns the max-norm of this Vector.
        /// </summary>
        public float NormMax => Math.Max(x, y);

        #endregion


        #region Static Properties
        
        /// <summary>
        /// Predefined all-zero Vector
        /// </summary>
        public static uVector2 Zero { get; } = new uVector2(0u, 0u);
        
        /// <summary>
        /// Predefined all-ones Vector
        /// </summary>
        public static uVector2 Ones { get; } = new uVector2(1u, 1u);
        
        /// <summary>
        /// Predefined unit-X Vector
        /// </summary>
        public static uVector2 UnitX { get; } = new uVector2(1u, 0u);
        
        /// <summary>
        /// Predefined unit-Y Vector
        /// </summary>
        public static uVector2 UnitY { get; } = new uVector2(0u, 1u);
        
        /// <summary>
        /// Predefined all-MaxValue Vector
        /// </summary>
        public static uVector2 MaxValue { get; } = new uVector2(uint.MaxValue, uint.MaxValue);
        
        /// <summary>
        /// Predefined all-MinValue Vector
        /// </summary>
        public static uVector2 MinValue { get; } = new uVector2(uint.MinValue, uint.MinValue);

        #endregion


        #region Operators
        
        /// <summary>
        /// Returns true iff this equals rhs component-wise.
        /// </summary>
        public static bool operator==(uVector2 lhs, uVector2 rhs) => lhs.Equals(rhs);
        
        /// <summary>
        /// Returns true iff this does not equal rhs (component-wise).
        /// </summary>
        public static bool operator!=(uVector2 lhs, uVector2 rhs) => !lhs.Equals(rhs);

        #endregion


        #region Functions
        
        /// <summary>
        /// Returns an enumerator that iterates through all components.
        /// </summary>
        public IEnumerator<uint> GetEnumerator()
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
        public bool Equals(uVector2 rhs) => (x.Equals(rhs.x) && y.Equals(rhs.y));
        
        /// <summary>
        /// Returns true iff this equals rhs type- and component-wise.
        /// </summary>
        public override bool Equals(object obj)
        {
            if (ReferenceEquals(null, obj)) return false;
            return obj is uVector2 && Equals((uVector2) obj);
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
        public double NormP(double p) => Math.Pow((Math.Pow((double)x, p) + Math.Pow((double)y, p)), 1 / p);

        #endregion


        #region Static Functions
        
        /// <summary>
        /// Converts the string representation of the Vector into a Vector representation (using ', ' as a separator).
        /// </summary>
        public static uVector2 Parse(string s) => Parse(s, ", ");
        
        /// <summary>
        /// Converts the string representation of the Vector into a Vector representation (using a designated separator).
        /// </summary>
        public static uVector2 Parse(string s, string sep)
        {
            var kvp = s.Split(new[] { sep }, StringSplitOptions.None);
            if (kvp.Length != 2) throw new FormatException("input has not exactly 2 parts");
            return new uVector2(uint.Parse(kvp[0].Trim()), uint.Parse(kvp[1].Trim()));
        }
        
        /// <summary>
        /// Converts the string representation of the Vector into a Vector representation (using a designated separator and a type provider).
        /// </summary>
        public static uVector2 Parse(string s, string sep, IFormatProvider provider)
        {
            var kvp = s.Split(new[] { sep }, StringSplitOptions.None);
            if (kvp.Length != 2) throw new FormatException("input has not exactly 2 parts");
            return new uVector2(uint.Parse(kvp[0].Trim(), provider), uint.Parse(kvp[1].Trim(), provider));
        }
        
        /// <summary>
        /// Converts the string representation of the Vector into a Vector representation (using a designated separator and a number style).
        /// </summary>
        public static uVector2 Parse(string s, string sep, NumberStyles style)
        {
            var kvp = s.Split(new[] { sep }, StringSplitOptions.None);
            if (kvp.Length != 2) throw new FormatException("input has not exactly 2 parts");
            return new uVector2(uint.Parse(kvp[0].Trim(), style), uint.Parse(kvp[1].Trim(), style));
        }
        
        /// <summary>
        /// Converts the string representation of the Vector into a Vector representation (using a designated separator and a number style and a forMatrix provider).
        /// </summary>
        public static uVector2 Parse(string s, string sep, NumberStyles style, IFormatProvider provider)
        {
            var kvp = s.Split(new[] { sep }, StringSplitOptions.None);
            if (kvp.Length != 2) throw new FormatException("input has not exactly 2 parts");
            return new uVector2(uint.Parse(kvp[0].Trim(), style, provider), uint.Parse(kvp[1].Trim(), style, provider));
        }
        
        /// <summary>
        /// Tries to convert the string representation of the Vector into a Vector representation (using ', ' as a separator), returns false if string was invalid.
        /// </summary>
        public static bool TryParse(string s, out uVector2 result) => TryParse(s, ", ", out result);
        
        /// <summary>
        /// Tries to convert the string representation of the Vector into a Vector representation (using a designated separator), returns false if string was invalid.
        /// </summary>
        public static bool TryParse(string s, string sep, out uVector2 result)
        {
            result = Zero;
            if (string.IsNullOrEmpty(s)) return false;
            var kvp = s.Split(new[] { sep }, StringSplitOptions.None);
            if (kvp.Length != 2) return false;
            uint x = 0u, y = 0u;
            var ok = (uint.TryParse(kvp[0].Trim(), out x) && uint.TryParse(kvp[1].Trim(), out y));
            result = ok ? new uVector2(x, y) : Zero;
            return ok;
        }
        
        /// <summary>
        /// Tries to convert the string representation of the Vector into a Vector representation (using a designated separator and a number style and a forMatrix provider), returns false if string was invalid.
        /// </summary>
        public static bool TryParse(string s, string sep, NumberStyles style, IFormatProvider provider, out uVector2 result)
        {
            result = Zero;
            if (string.IsNullOrEmpty(s)) return false;
            var kvp = s.Split(new[] { sep }, StringSplitOptions.None);
            if (kvp.Length != 2) return false;
            uint x = 0u, y = 0u;
            var ok = (uint.TryParse(kvp[0].Trim(), style, provider, out x) && uint.TryParse(kvp[1].Trim(), style, provider, out y));
            result = ok ? new uVector2(x, y) : Zero;
            return ok;
        }
        
        /// <summary>
        /// OuterProduct treats the first parameter c as a column Vector (Matrix with one column) and the second parameter r as a row Vector (Matrix with one row) and does a linear algebraic Matrix multiply c * r, yielding a Matrix whose number of rows is the number of components in c and whose number of columns is the number of components in r.
        /// </summary>
        public static uMatrix2 OuterProduct(uVector2 c, uVector2 r) => new uMatrix2(c.x * r.x, c.y * r.x, c.x * r.y, c.y * r.y);
        
        /// <summary>
        /// OuterProduct treats the first parameter c as a column Vector (Matrix with one column) and the second parameter r as a row Vector (Matrix with one row) and does a linear algebraic Matrix multiply c * r, yielding a Matrix whose number of rows is the number of components in c and whose number of columns is the number of components in r.
        /// </summary>
        public static uMatrix2x3 OuterProduct(uVector3 c, uVector2 r) => new uMatrix2x3(c.x * r.x, c.y * r.x, c.z * r.x, c.x * r.y, c.y * r.y, c.z * r.y);
        
        /// <summary>
        /// OuterProduct treats the first parameter c as a column Vector (Matrix with one column) and the second parameter r as a row Vector (Matrix with one row) and does a linear algebraic Matrix multiply c * r, yielding a Matrix whose number of rows is the number of components in c and whose number of columns is the number of components in r.
        /// </summary>
        public static uMatrix3x2 OuterProduct(uVector2 c, uVector3 r) => new uMatrix3x2(c.x * r.x, c.y * r.x, c.x * r.y, c.y * r.y, c.x * r.z, c.y * r.z);
        
        /// <summary>
        /// OuterProduct treats the first parameter c as a column Vector (Matrix with one column) and the second parameter r as a row Vector (Matrix with one row) and does a linear algebraic Matrix multiply c * r, yielding a Matrix whose number of rows is the number of components in c and whose number of columns is the number of components in r.
        /// </summary>
        public static uMatrix2x4 OuterProduct(uVector4 c, uVector2 r) => new uMatrix2x4(c.x * r.x, c.y * r.x, c.z * r.x, c.w * r.x, c.x * r.y, c.y * r.y, c.z * r.y, c.w * r.y);
        
        /// <summary>
        /// OuterProduct treats the first parameter c as a column Vector (Matrix with one column) and the second parameter r as a row Vector (Matrix with one row) and does a linear algebraic Matrix multiply c * r, yielding a Matrix whose number of rows is the number of components in c and whose number of columns is the number of components in r.
        /// </summary>
        public static uMatrix4x2 OuterProduct(uVector2 c, uVector4 r) => new uMatrix4x2(c.x * r.x, c.y * r.x, c.x * r.y, c.y * r.y, c.x * r.z, c.y * r.z, c.x * r.w, c.y * r.w);
        
        /// <summary>
        /// Returns the inner product (dot product, scalar product) of the two Vectors.
        /// </summary>
        public static uint Dot(uVector2 lhs, uVector2 rhs) => (lhs.x * rhs.x + lhs.y * rhs.y);
        
        /// <summary>
        /// Returns the euclidean distance between the two Vectors.
        /// </summary>
        public static float Distance(uVector2 lhs, uVector2 rhs) => (lhs - rhs).Length;
        
        /// <summary>
        /// Returns the squared euclidean distance between the two Vectors.
        /// </summary>
        public static float DistanceSqr(uVector2 lhs, uVector2 rhs) => (lhs - rhs).LengthSqr;
        
        /// <summary>
        /// Returns the length of the outer product (cross product, Vector product) of the two Vectors.
        /// </summary>
        public static uint Cross(uVector2 l, uVector2 r) => l.x * r.y - l.y * r.x;
        
        /// <summary>
        /// Returns a uVector2 with independent and identically distributed uniform integer values between 0 (inclusive) and int.MaxValue (exclusive).
        /// </summary>
        public static uVector2 Random(System.Random random) => new uVector2((uint)random.Next(), (uint)random.Next());

        #endregion


        #region Component-Wise Static Functions
        
        /// <summary>
        /// Returns a bVector2 from component-wise application of Equal (lhs == rhs).
        /// </summary>
        public static bVector2 Equal(uVector2 lhs, uVector2 rhs) => new bVector2(lhs.x == rhs.x, lhs.y == rhs.y);
        
        /// <summary>
        /// Returns a bVector2 from component-wise application of Equal (lhs == rhs).
        /// </summary>
        public static bVector2 Equal(uVector2 lhs, uint rhs) => new bVector2(lhs.x == rhs, lhs.y == rhs);
        
        /// <summary>
        /// Returns a bVector2 from component-wise application of Equal (lhs == rhs).
        /// </summary>
        public static bVector2 Equal(uint lhs, uVector2 rhs) => new bVector2(lhs == rhs.x, lhs == rhs.y);
        
        /// <summary>
        /// Returns a bVector from the application of Equal (lhs == rhs).
        /// </summary>
        public static bVector2 Equal(uint lhs, uint rhs) => new bVector2(lhs == rhs);
        
        /// <summary>
        /// Returns a bVector2 from component-wise application of NotEqual (lhs != rhs).
        /// </summary>
        public static bVector2 NotEqual(uVector2 lhs, uVector2 rhs) => new bVector2(lhs.x != rhs.x, lhs.y != rhs.y);
        
        /// <summary>
        /// Returns a bVector2 from component-wise application of NotEqual (lhs != rhs).
        /// </summary>
        public static bVector2 NotEqual(uVector2 lhs, uint rhs) => new bVector2(lhs.x != rhs, lhs.y != rhs);
        
        /// <summary>
        /// Returns a bVector2 from component-wise application of NotEqual (lhs != rhs).
        /// </summary>
        public static bVector2 NotEqual(uint lhs, uVector2 rhs) => new bVector2(lhs != rhs.x, lhs != rhs.y);
        
        /// <summary>
        /// Returns a bVector from the application of NotEqual (lhs != rhs).
        /// </summary>
        public static bVector2 NotEqual(uint lhs, uint rhs) => new bVector2(lhs != rhs);
        
        /// <summary>
        /// Returns a bVector2 from component-wise application of GreaterThan (lhs &gt; rhs).
        /// </summary>
        public static bVector2 GreaterThan(uVector2 lhs, uVector2 rhs) => new bVector2(lhs.x > rhs.x, lhs.y > rhs.y);
        
        /// <summary>
        /// Returns a bVector2 from component-wise application of GreaterThan (lhs &gt; rhs).
        /// </summary>
        public static bVector2 GreaterThan(uVector2 lhs, uint rhs) => new bVector2(lhs.x > rhs, lhs.y > rhs);
        
        /// <summary>
        /// Returns a bVector2 from component-wise application of GreaterThan (lhs &gt; rhs).
        /// </summary>
        public static bVector2 GreaterThan(uint lhs, uVector2 rhs) => new bVector2(lhs > rhs.x, lhs > rhs.y);
        
        /// <summary>
        /// Returns a bVector from the application of GreaterThan (lhs &gt; rhs).
        /// </summary>
        public static bVector2 GreaterThan(uint lhs, uint rhs) => new bVector2(lhs > rhs);
        
        /// <summary>
        /// Returns a bVector2 from component-wise application of GreaterThanEqual (lhs &gt;= rhs).
        /// </summary>
        public static bVector2 GreaterThanEqual(uVector2 lhs, uVector2 rhs) => new bVector2(lhs.x >= rhs.x, lhs.y >= rhs.y);
        
        /// <summary>
        /// Returns a bVector2 from component-wise application of GreaterThanEqual (lhs &gt;= rhs).
        /// </summary>
        public static bVector2 GreaterThanEqual(uVector2 lhs, uint rhs) => new bVector2(lhs.x >= rhs, lhs.y >= rhs);
        
        /// <summary>
        /// Returns a bVector2 from component-wise application of GreaterThanEqual (lhs &gt;= rhs).
        /// </summary>
        public static bVector2 GreaterThanEqual(uint lhs, uVector2 rhs) => new bVector2(lhs >= rhs.x, lhs >= rhs.y);
        
        /// <summary>
        /// Returns a bVector from the application of GreaterThanEqual (lhs &gt;= rhs).
        /// </summary>
        public static bVector2 GreaterThanEqual(uint lhs, uint rhs) => new bVector2(lhs >= rhs);
        
        /// <summary>
        /// Returns a bVector2 from component-wise application of LesserThan (lhs &lt; rhs).
        /// </summary>
        public static bVector2 LesserThan(uVector2 lhs, uVector2 rhs) => new bVector2(lhs.x < rhs.x, lhs.y < rhs.y);
        
        /// <summary>
        /// Returns a bVector2 from component-wise application of LesserThan (lhs &lt; rhs).
        /// </summary>
        public static bVector2 LesserThan(uVector2 lhs, uint rhs) => new bVector2(lhs.x < rhs, lhs.y < rhs);
        
        /// <summary>
        /// Returns a bVector2 from component-wise application of LesserThan (lhs &lt; rhs).
        /// </summary>
        public static bVector2 LesserThan(uint lhs, uVector2 rhs) => new bVector2(lhs < rhs.x, lhs < rhs.y);
        
        /// <summary>
        /// Returns a bVector from the application of LesserThan (lhs &lt; rhs).
        /// </summary>
        public static bVector2 LesserThan(uint lhs, uint rhs) => new bVector2(lhs < rhs);
        
        /// <summary>
        /// Returns a bVector2 from component-wise application of LesserThanEqual (lhs &lt;= rhs).
        /// </summary>
        public static bVector2 LesserThanEqual(uVector2 lhs, uVector2 rhs) => new bVector2(lhs.x <= rhs.x, lhs.y <= rhs.y);
        
        /// <summary>
        /// Returns a bVector2 from component-wise application of LesserThanEqual (lhs &lt;= rhs).
        /// </summary>
        public static bVector2 LesserThanEqual(uVector2 lhs, uint rhs) => new bVector2(lhs.x <= rhs, lhs.y <= rhs);
        
        /// <summary>
        /// Returns a bVector2 from component-wise application of LesserThanEqual (lhs &lt;= rhs).
        /// </summary>
        public static bVector2 LesserThanEqual(uint lhs, uVector2 rhs) => new bVector2(lhs <= rhs.x, lhs <= rhs.y);
        
        /// <summary>
        /// Returns a bVector from the application of LesserThanEqual (lhs &lt;= rhs).
        /// </summary>
        public static bVector2 LesserThanEqual(uint lhs, uint rhs) => new bVector2(lhs <= rhs);
        
        /// <summary>
        /// Returns a uVector2 from component-wise application of Abs (v).
        /// </summary>
        public static uVector2 Abs(uVector2 v) => new uVector2(v.x, v.y);
        
        /// <summary>
        /// Returns a uVector from the application of Abs (v).
        /// </summary>
        public static uVector2 Abs(uint v) => new uVector2(v);
        
        /// <summary>
        /// Returns a uVector2 from component-wise application of HermiteInterpolationOrder3 ((3 - 2 * v) * v * v).
        /// </summary>
        public static uVector2 HermiteInterpolationOrder3(uVector2 v) => new uVector2((3 - 2 * v.x) * v.x * v.x, (3 - 2 * v.y) * v.y * v.y);
        
        /// <summary>
        /// Returns a uVector from the application of HermiteInterpolationOrder3 ((3 - 2 * v) * v * v).
        /// </summary>
        public static uVector2 HermiteInterpolationOrder3(uint v) => new uVector2((3 - 2 * v) * v * v);
        
        /// <summary>
        /// Returns a uVector2 from component-wise application of HermiteInterpolationOrder5 (((6 * v - 15) * v + 10) * v * v * v).
        /// </summary>
        public static uVector2 HermiteInterpolationOrder5(uVector2 v) => new uVector2(((6 * v.x - 15) * v.x + 10) * v.x * v.x * v.x, ((6 * v.y - 15) * v.y + 10) * v.y * v.y * v.y);
        
        /// <summary>
        /// Returns a uVector from the application of HermiteInterpolationOrder5 (((6 * v - 15) * v + 10) * v * v * v).
        /// </summary>
        public static uVector2 HermiteInterpolationOrder5(uint v) => new uVector2(((6 * v - 15) * v + 10) * v * v * v);
        
        /// <summary>
        /// Returns a uVector2 from component-wise application of Sqr (v * v).
        /// </summary>
        public static uVector2 Sqr(uVector2 v) => new uVector2(v.x * v.x, v.y * v.y);
        
        /// <summary>
        /// Returns a uVector from the application of Sqr (v * v).
        /// </summary>
        public static uVector2 Sqr(uint v) => new uVector2(v * v);
        
        /// <summary>
        /// Returns a uVector2 from component-wise application of Pow2 (v * v).
        /// </summary>
        public static uVector2 Pow2(uVector2 v) => new uVector2(v.x * v.x, v.y * v.y);
        
        /// <summary>
        /// Returns a uVector from the application of Pow2 (v * v).
        /// </summary>
        public static uVector2 Pow2(uint v) => new uVector2(v * v);
        
        /// <summary>
        /// Returns a uVector2 from component-wise application of Pow3 (v * v * v).
        /// </summary>
        public static uVector2 Pow3(uVector2 v) => new uVector2(v.x * v.x * v.x, v.y * v.y * v.y);
        
        /// <summary>
        /// Returns a uVector from the application of Pow3 (v * v * v).
        /// </summary>
        public static uVector2 Pow3(uint v) => new uVector2(v * v * v);
        
        /// <summary>
        /// Returns a uVector2 from component-wise application of Step (v &gt;= 0u ? 1u : 0u).
        /// </summary>
        public static uVector2 Step(uVector2 v) => new uVector2(v.x >= 0u ? 1u : 0u, v.y >= 0u ? 1u : 0u);
        
        /// <summary>
        /// Returns a uVector from the application of Step (v &gt;= 0u ? 1u : 0u).
        /// </summary>
        public static uVector2 Step(uint v) => new uVector2(v >= 0u ? 1u : 0u);
        
        /// <summary>
        /// Returns a uVector2 from component-wise application of Sqrt ((uint)Math.Sqrt((double)v)).
        /// </summary>
        public static uVector2 Sqrt(uVector2 v) => new uVector2((uint)Math.Sqrt((double)v.x), (uint)Math.Sqrt((double)v.y));
        
        /// <summary>
        /// Returns a uVector from the application of Sqrt ((uint)Math.Sqrt((double)v)).
        /// </summary>
        public static uVector2 Sqrt(uint v) => new uVector2((uint)Math.Sqrt((double)v));
        
        /// <summary>
        /// Returns a uVector2 from component-wise application of InverseSqrt ((uint)(1.0 / Math.Sqrt((double)v))).
        /// </summary>
        public static uVector2 InverseSqrt(uVector2 v) => new uVector2((uint)(1.0 / Math.Sqrt((double)v.x)), (uint)(1.0 / Math.Sqrt((double)v.y)));
        
        /// <summary>
        /// Returns a uVector from the application of InverseSqrt ((uint)(1.0 / Math.Sqrt((double)v))).
        /// </summary>
        public static uVector2 InverseSqrt(uint v) => new uVector2((uint)(1.0 / Math.Sqrt((double)v)));
        
        /// <summary>
        /// Returns a iVector2 from component-wise application of Sign (Math.Sign(v)).
        /// </summary>
        public static iVector2 Sign(uVector2 v) => new iVector2(Math.Sign(v.x), Math.Sign(v.y));
        
        /// <summary>
        /// Returns a iVector from the application of Sign (Math.Sign(v)).
        /// </summary>
        public static iVector2 Sign(uint v) => new iVector2(Math.Sign(v));
        
        /// <summary>
        /// Returns a uVector2 from component-wise application of Max (Math.Max(lhs, rhs)).
        /// </summary>
        public static uVector2 Max(uVector2 lhs, uVector2 rhs) => new uVector2(Math.Max(lhs.x, rhs.x), Math.Max(lhs.y, rhs.y));
        
        /// <summary>
        /// Returns a uVector2 from component-wise application of Max (Math.Max(lhs, rhs)).
        /// </summary>
        public static uVector2 Max(uVector2 lhs, uint rhs) => new uVector2(Math.Max(lhs.x, rhs), Math.Max(lhs.y, rhs));
        
        /// <summary>
        /// Returns a uVector2 from component-wise application of Max (Math.Max(lhs, rhs)).
        /// </summary>
        public static uVector2 Max(uint lhs, uVector2 rhs) => new uVector2(Math.Max(lhs, rhs.x), Math.Max(lhs, rhs.y));
        
        /// <summary>
        /// Returns a uVector from the application of Max (Math.Max(lhs, rhs)).
        /// </summary>
        public static uVector2 Max(uint lhs, uint rhs) => new uVector2(Math.Max(lhs, rhs));
        
        /// <summary>
        /// Returns a uVector2 from component-wise application of Min (Math.Min(lhs, rhs)).
        /// </summary>
        public static uVector2 Min(uVector2 lhs, uVector2 rhs) => new uVector2(Math.Min(lhs.x, rhs.x), Math.Min(lhs.y, rhs.y));
        
        /// <summary>
        /// Returns a uVector2 from component-wise application of Min (Math.Min(lhs, rhs)).
        /// </summary>
        public static uVector2 Min(uVector2 lhs, uint rhs) => new uVector2(Math.Min(lhs.x, rhs), Math.Min(lhs.y, rhs));
        
        /// <summary>
        /// Returns a uVector2 from component-wise application of Min (Math.Min(lhs, rhs)).
        /// </summary>
        public static uVector2 Min(uint lhs, uVector2 rhs) => new uVector2(Math.Min(lhs, rhs.x), Math.Min(lhs, rhs.y));
        
        /// <summary>
        /// Returns a uVector from the application of Min (Math.Min(lhs, rhs)).
        /// </summary>
        public static uVector2 Min(uint lhs, uint rhs) => new uVector2(Math.Min(lhs, rhs));
        
        /// <summary>
        /// Returns a uVector2 from component-wise application of Pow ((uint)Math.Pow((double)lhs, (double)rhs)).
        /// </summary>
        public static uVector2 Pow(uVector2 lhs, uVector2 rhs) => new uVector2((uint)Math.Pow((double)lhs.x, (double)rhs.x), (uint)Math.Pow((double)lhs.y, (double)rhs.y));
        
        /// <summary>
        /// Returns a uVector2 from component-wise application of Pow ((uint)Math.Pow((double)lhs, (double)rhs)).
        /// </summary>
        public static uVector2 Pow(uVector2 lhs, uint rhs) => new uVector2((uint)Math.Pow((double)lhs.x, (double)rhs), (uint)Math.Pow((double)lhs.y, (double)rhs));
        
        /// <summary>
        /// Returns a uVector2 from component-wise application of Pow ((uint)Math.Pow((double)lhs, (double)rhs)).
        /// </summary>
        public static uVector2 Pow(uint lhs, uVector2 rhs) => new uVector2((uint)Math.Pow((double)lhs, (double)rhs.x), (uint)Math.Pow((double)lhs, (double)rhs.y));
        
        /// <summary>
        /// Returns a uVector from the application of Pow ((uint)Math.Pow((double)lhs, (double)rhs)).
        /// </summary>
        public static uVector2 Pow(uint lhs, uint rhs) => new uVector2((uint)Math.Pow((double)lhs, (double)rhs));
        
        /// <summary>
        /// Returns a uVector2 from component-wise application of Log ((uint)Math.Log((double)lhs, (double)rhs)).
        /// </summary>
        public static uVector2 Log(uVector2 lhs, uVector2 rhs) => new uVector2((uint)Math.Log((double)lhs.x, (double)rhs.x), (uint)Math.Log((double)lhs.y, (double)rhs.y));
        
        /// <summary>
        /// Returns a uVector2 from component-wise application of Log ((uint)Math.Log((double)lhs, (double)rhs)).
        /// </summary>
        public static uVector2 Log(uVector2 lhs, uint rhs) => new uVector2((uint)Math.Log((double)lhs.x, (double)rhs), (uint)Math.Log((double)lhs.y, (double)rhs));
        
        /// <summary>
        /// Returns a uVector2 from component-wise application of Log ((uint)Math.Log((double)lhs, (double)rhs)).
        /// </summary>
        public static uVector2 Log(uint lhs, uVector2 rhs) => new uVector2((uint)Math.Log((double)lhs, (double)rhs.x), (uint)Math.Log((double)lhs, (double)rhs.y));
        
        /// <summary>
        /// Returns a uVector from the application of Log ((uint)Math.Log((double)lhs, (double)rhs)).
        /// </summary>
        public static uVector2 Log(uint lhs, uint rhs) => new uVector2((uint)Math.Log((double)lhs, (double)rhs));
        
        /// <summary>
        /// Returns a uVector2 from component-wise application of Clamp (Math.Min(Math.Max(v, min), max)).
        /// </summary>
        public static uVector2 Clamp(uVector2 v, uVector2 min, uVector2 max) => new uVector2(Math.Min(Math.Max(v.x, min.x), max.x), Math.Min(Math.Max(v.y, min.y), max.y));
        
        /// <summary>
        /// Returns a uVector2 from component-wise application of Clamp (Math.Min(Math.Max(v, min), max)).
        /// </summary>
        public static uVector2 Clamp(uVector2 v, uVector2 min, uint max) => new uVector2(Math.Min(Math.Max(v.x, min.x), max), Math.Min(Math.Max(v.y, min.y), max));
        
        /// <summary>
        /// Returns a uVector2 from component-wise application of Clamp (Math.Min(Math.Max(v, min), max)).
        /// </summary>
        public static uVector2 Clamp(uVector2 v, uint min, uVector2 max) => new uVector2(Math.Min(Math.Max(v.x, min), max.x), Math.Min(Math.Max(v.y, min), max.y));
        
        /// <summary>
        /// Returns a uVector2 from component-wise application of Clamp (Math.Min(Math.Max(v, min), max)).
        /// </summary>
        public static uVector2 Clamp(uVector2 v, uint min, uint max) => new uVector2(Math.Min(Math.Max(v.x, min), max), Math.Min(Math.Max(v.y, min), max));
        
        /// <summary>
        /// Returns a uVector2 from component-wise application of Clamp (Math.Min(Math.Max(v, min), max)).
        /// </summary>
        public static uVector2 Clamp(uint v, uVector2 min, uVector2 max) => new uVector2(Math.Min(Math.Max(v, min.x), max.x), Math.Min(Math.Max(v, min.y), max.y));
        
        /// <summary>
        /// Returns a uVector2 from component-wise application of Clamp (Math.Min(Math.Max(v, min), max)).
        /// </summary>
        public static uVector2 Clamp(uint v, uVector2 min, uint max) => new uVector2(Math.Min(Math.Max(v, min.x), max), Math.Min(Math.Max(v, min.y), max));
        
        /// <summary>
        /// Returns a uVector2 from component-wise application of Clamp (Math.Min(Math.Max(v, min), max)).
        /// </summary>
        public static uVector2 Clamp(uint v, uint min, uVector2 max) => new uVector2(Math.Min(Math.Max(v, min), max.x), Math.Min(Math.Max(v, min), max.y));
        
        /// <summary>
        /// Returns a uVector from the application of Clamp (Math.Min(Math.Max(v, min), max)).
        /// </summary>
        public static uVector2 Clamp(uint v, uint min, uint max) => new uVector2(Math.Min(Math.Max(v, min), max));
        
        /// <summary>
        /// Returns a uVector2 from component-wise application of Mix (min * (1-a) + max * a).
        /// </summary>
        public static uVector2 Mix(uVector2 min, uVector2 max, uVector2 a) => new uVector2(min.x * (1-a.x) + max.x * a.x, min.y * (1-a.y) + max.y * a.y);
        
        /// <summary>
        /// Returns a uVector2 from component-wise application of Mix (min * (1-a) + max * a).
        /// </summary>
        public static uVector2 Mix(uVector2 min, uVector2 max, uint a) => new uVector2(min.x * (1-a) + max.x * a, min.y * (1-a) + max.y * a);
        
        /// <summary>
        /// Returns a uVector2 from component-wise application of Mix (min * (1-a) + max * a).
        /// </summary>
        public static uVector2 Mix(uVector2 min, uint max, uVector2 a) => new uVector2(min.x * (1-a.x) + max * a.x, min.y * (1-a.y) + max * a.y);
        
        /// <summary>
        /// Returns a uVector2 from component-wise application of Mix (min * (1-a) + max * a).
        /// </summary>
        public static uVector2 Mix(uVector2 min, uint max, uint a) => new uVector2(min.x * (1-a) + max * a, min.y * (1-a) + max * a);
        
        /// <summary>
        /// Returns a uVector2 from component-wise application of Mix (min * (1-a) + max * a).
        /// </summary>
        public static uVector2 Mix(uint min, uVector2 max, uVector2 a) => new uVector2(min * (1-a.x) + max.x * a.x, min * (1-a.y) + max.y * a.y);
        
        /// <summary>
        /// Returns a uVector2 from component-wise application of Mix (min * (1-a) + max * a).
        /// </summary>
        public static uVector2 Mix(uint min, uVector2 max, uint a) => new uVector2(min * (1-a) + max.x * a, min * (1-a) + max.y * a);
        
        /// <summary>
        /// Returns a uVector2 from component-wise application of Mix (min * (1-a) + max * a).
        /// </summary>
        public static uVector2 Mix(uint min, uint max, uVector2 a) => new uVector2(min * (1-a.x) + max * a.x, min * (1-a.y) + max * a.y);
        
        /// <summary>
        /// Returns a uVector from the application of Mix (min * (1-a) + max * a).
        /// </summary>
        public static uVector2 Mix(uint min, uint max, uint a) => new uVector2(min * (1-a) + max * a);
        
        /// <summary>
        /// Returns a uVector2 from component-wise application of Lerp (min * (1-a) + max * a).
        /// </summary>
        public static uVector2 Lerp(uVector2 min, uVector2 max, uVector2 a) => new uVector2(min.x * (1-a.x) + max.x * a.x, min.y * (1-a.y) + max.y * a.y);
        
        /// <summary>
        /// Returns a uVector2 from component-wise application of Lerp (min * (1-a) + max * a).
        /// </summary>
        public static uVector2 Lerp(uVector2 min, uVector2 max, uint a) => new uVector2(min.x * (1-a) + max.x * a, min.y * (1-a) + max.y * a);
        
        /// <summary>
        /// Returns a uVector2 from component-wise application of Lerp (min * (1-a) + max * a).
        /// </summary>
        public static uVector2 Lerp(uVector2 min, uint max, uVector2 a) => new uVector2(min.x * (1-a.x) + max * a.x, min.y * (1-a.y) + max * a.y);
        
        /// <summary>
        /// Returns a uVector2 from component-wise application of Lerp (min * (1-a) + max * a).
        /// </summary>
        public static uVector2 Lerp(uVector2 min, uint max, uint a) => new uVector2(min.x * (1-a) + max * a, min.y * (1-a) + max * a);
        
        /// <summary>
        /// Returns a uVector2 from component-wise application of Lerp (min * (1-a) + max * a).
        /// </summary>
        public static uVector2 Lerp(uint min, uVector2 max, uVector2 a) => new uVector2(min * (1-a.x) + max.x * a.x, min * (1-a.y) + max.y * a.y);
        
        /// <summary>
        /// Returns a uVector2 from component-wise application of Lerp (min * (1-a) + max * a).
        /// </summary>
        public static uVector2 Lerp(uint min, uVector2 max, uint a) => new uVector2(min * (1-a) + max.x * a, min * (1-a) + max.y * a);
        
        /// <summary>
        /// Returns a uVector2 from component-wise application of Lerp (min * (1-a) + max * a).
        /// </summary>
        public static uVector2 Lerp(uint min, uint max, uVector2 a) => new uVector2(min * (1-a.x) + max * a.x, min * (1-a.y) + max * a.y);
        
        /// <summary>
        /// Returns a uVector from the application of Lerp (min * (1-a) + max * a).
        /// </summary>
        public static uVector2 Lerp(uint min, uint max, uint a) => new uVector2(min * (1-a) + max * a);
        
        /// <summary>
        /// Returns a uVector2 from component-wise application of Smoothstep (((v - edge0) / (edge1 - edge0)).Clamp().HermiteInterpolationOrder3()).
        /// </summary>
        public static uVector2 Smoothstep(uVector2 edge0, uVector2 edge1, uVector2 v) => new uVector2(((v.x - edge0.x) / (edge1.x - edge0.x)).Clamp().HermiteInterpolationOrder3(), ((v.y - edge0.y) / (edge1.y - edge0.y)).Clamp().HermiteInterpolationOrder3());
        
        /// <summary>
        /// Returns a uVector2 from component-wise application of Smoothstep (((v - edge0) / (edge1 - edge0)).Clamp().HermiteInterpolationOrder3()).
        /// </summary>
        public static uVector2 Smoothstep(uVector2 edge0, uVector2 edge1, uint v) => new uVector2(((v - edge0.x) / (edge1.x - edge0.x)).Clamp().HermiteInterpolationOrder3(), ((v - edge0.y) / (edge1.y - edge0.y)).Clamp().HermiteInterpolationOrder3());
        
        /// <summary>
        /// Returns a uVector2 from component-wise application of Smoothstep (((v - edge0) / (edge1 - edge0)).Clamp().HermiteInterpolationOrder3()).
        /// </summary>
        public static uVector2 Smoothstep(uVector2 edge0, uint edge1, uVector2 v) => new uVector2(((v.x - edge0.x) / (edge1 - edge0.x)).Clamp().HermiteInterpolationOrder3(), ((v.y - edge0.y) / (edge1 - edge0.y)).Clamp().HermiteInterpolationOrder3());
        
        /// <summary>
        /// Returns a uVector2 from component-wise application of Smoothstep (((v - edge0) / (edge1 - edge0)).Clamp().HermiteInterpolationOrder3()).
        /// </summary>
        public static uVector2 Smoothstep(uVector2 edge0, uint edge1, uint v) => new uVector2(((v - edge0.x) / (edge1 - edge0.x)).Clamp().HermiteInterpolationOrder3(), ((v - edge0.y) / (edge1 - edge0.y)).Clamp().HermiteInterpolationOrder3());
        
        /// <summary>
        /// Returns a uVector2 from component-wise application of Smoothstep (((v - edge0) / (edge1 - edge0)).Clamp().HermiteInterpolationOrder3()).
        /// </summary>
        public static uVector2 Smoothstep(uint edge0, uVector2 edge1, uVector2 v) => new uVector2(((v.x - edge0) / (edge1.x - edge0)).Clamp().HermiteInterpolationOrder3(), ((v.y - edge0) / (edge1.y - edge0)).Clamp().HermiteInterpolationOrder3());
        
        /// <summary>
        /// Returns a uVector2 from component-wise application of Smoothstep (((v - edge0) / (edge1 - edge0)).Clamp().HermiteInterpolationOrder3()).
        /// </summary>
        public static uVector2 Smoothstep(uint edge0, uVector2 edge1, uint v) => new uVector2(((v - edge0) / (edge1.x - edge0)).Clamp().HermiteInterpolationOrder3(), ((v - edge0) / (edge1.y - edge0)).Clamp().HermiteInterpolationOrder3());
        
        /// <summary>
        /// Returns a uVector2 from component-wise application of Smoothstep (((v - edge0) / (edge1 - edge0)).Clamp().HermiteInterpolationOrder3()).
        /// </summary>
        public static uVector2 Smoothstep(uint edge0, uint edge1, uVector2 v) => new uVector2(((v.x - edge0) / (edge1 - edge0)).Clamp().HermiteInterpolationOrder3(), ((v.y - edge0) / (edge1 - edge0)).Clamp().HermiteInterpolationOrder3());
        
        /// <summary>
        /// Returns a uVector from the application of Smoothstep (((v - edge0) / (edge1 - edge0)).Clamp().HermiteInterpolationOrder3()).
        /// </summary>
        public static uVector2 Smoothstep(uint edge0, uint edge1, uint v) => new uVector2(((v - edge0) / (edge1 - edge0)).Clamp().HermiteInterpolationOrder3());
        
        /// <summary>
        /// Returns a uVector2 from component-wise application of Smootherstep (((v - edge0) / (edge1 - edge0)).Clamp().HermiteInterpolationOrder5()).
        /// </summary>
        public static uVector2 Smootherstep(uVector2 edge0, uVector2 edge1, uVector2 v) => new uVector2(((v.x - edge0.x) / (edge1.x - edge0.x)).Clamp().HermiteInterpolationOrder5(), ((v.y - edge0.y) / (edge1.y - edge0.y)).Clamp().HermiteInterpolationOrder5());
        
        /// <summary>
        /// Returns a uVector2 from component-wise application of Smootherstep (((v - edge0) / (edge1 - edge0)).Clamp().HermiteInterpolationOrder5()).
        /// </summary>
        public static uVector2 Smootherstep(uVector2 edge0, uVector2 edge1, uint v) => new uVector2(((v - edge0.x) / (edge1.x - edge0.x)).Clamp().HermiteInterpolationOrder5(), ((v - edge0.y) / (edge1.y - edge0.y)).Clamp().HermiteInterpolationOrder5());
        
        /// <summary>
        /// Returns a uVector2 from component-wise application of Smootherstep (((v - edge0) / (edge1 - edge0)).Clamp().HermiteInterpolationOrder5()).
        /// </summary>
        public static uVector2 Smootherstep(uVector2 edge0, uint edge1, uVector2 v) => new uVector2(((v.x - edge0.x) / (edge1 - edge0.x)).Clamp().HermiteInterpolationOrder5(), ((v.y - edge0.y) / (edge1 - edge0.y)).Clamp().HermiteInterpolationOrder5());
        
        /// <summary>
        /// Returns a uVector2 from component-wise application of Smootherstep (((v - edge0) / (edge1 - edge0)).Clamp().HermiteInterpolationOrder5()).
        /// </summary>
        public static uVector2 Smootherstep(uVector2 edge0, uint edge1, uint v) => new uVector2(((v - edge0.x) / (edge1 - edge0.x)).Clamp().HermiteInterpolationOrder5(), ((v - edge0.y) / (edge1 - edge0.y)).Clamp().HermiteInterpolationOrder5());
        
        /// <summary>
        /// Returns a uVector2 from component-wise application of Smootherstep (((v - edge0) / (edge1 - edge0)).Clamp().HermiteInterpolationOrder5()).
        /// </summary>
        public static uVector2 Smootherstep(uint edge0, uVector2 edge1, uVector2 v) => new uVector2(((v.x - edge0) / (edge1.x - edge0)).Clamp().HermiteInterpolationOrder5(), ((v.y - edge0) / (edge1.y - edge0)).Clamp().HermiteInterpolationOrder5());
        
        /// <summary>
        /// Returns a uVector2 from component-wise application of Smootherstep (((v - edge0) / (edge1 - edge0)).Clamp().HermiteInterpolationOrder5()).
        /// </summary>
        public static uVector2 Smootherstep(uint edge0, uVector2 edge1, uint v) => new uVector2(((v - edge0) / (edge1.x - edge0)).Clamp().HermiteInterpolationOrder5(), ((v - edge0) / (edge1.y - edge0)).Clamp().HermiteInterpolationOrder5());
        
        /// <summary>
        /// Returns a uVector2 from component-wise application of Smootherstep (((v - edge0) / (edge1 - edge0)).Clamp().HermiteInterpolationOrder5()).
        /// </summary>
        public static uVector2 Smootherstep(uint edge0, uint edge1, uVector2 v) => new uVector2(((v.x - edge0) / (edge1 - edge0)).Clamp().HermiteInterpolationOrder5(), ((v.y - edge0) / (edge1 - edge0)).Clamp().HermiteInterpolationOrder5());
        
        /// <summary>
        /// Returns a uVector from the application of Smootherstep (((v - edge0) / (edge1 - edge0)).Clamp().HermiteInterpolationOrder5()).
        /// </summary>
        public static uVector2 Smootherstep(uint edge0, uint edge1, uint v) => new uVector2(((v - edge0) / (edge1 - edge0)).Clamp().HermiteInterpolationOrder5());
        
        /// <summary>
        /// Returns a uVector2 from component-wise application of Fma (a * b + c).
        /// </summary>
        public static uVector2 Fma(uVector2 a, uVector2 b, uVector2 c) => new uVector2(a.x * b.x + c.x, a.y * b.y + c.y);
        
        /// <summary>
        /// Returns a uVector2 from component-wise application of Fma (a * b + c).
        /// </summary>
        public static uVector2 Fma(uVector2 a, uVector2 b, uint c) => new uVector2(a.x * b.x + c, a.y * b.y + c);
        
        /// <summary>
        /// Returns a uVector2 from component-wise application of Fma (a * b + c).
        /// </summary>
        public static uVector2 Fma(uVector2 a, uint b, uVector2 c) => new uVector2(a.x * b + c.x, a.y * b + c.y);
        
        /// <summary>
        /// Returns a uVector2 from component-wise application of Fma (a * b + c).
        /// </summary>
        public static uVector2 Fma(uVector2 a, uint b, uint c) => new uVector2(a.x * b + c, a.y * b + c);
        
        /// <summary>
        /// Returns a uVector2 from component-wise application of Fma (a * b + c).
        /// </summary>
        public static uVector2 Fma(uint a, uVector2 b, uVector2 c) => new uVector2(a * b.x + c.x, a * b.y + c.y);
        
        /// <summary>
        /// Returns a uVector2 from component-wise application of Fma (a * b + c).
        /// </summary>
        public static uVector2 Fma(uint a, uVector2 b, uint c) => new uVector2(a * b.x + c, a * b.y + c);
        
        /// <summary>
        /// Returns a uVector2 from component-wise application of Fma (a * b + c).
        /// </summary>
        public static uVector2 Fma(uint a, uint b, uVector2 c) => new uVector2(a * b + c.x, a * b + c.y);
        
        /// <summary>
        /// Returns a uVector from the application of Fma (a * b + c).
        /// </summary>
        public static uVector2 Fma(uint a, uint b, uint c) => new uVector2(a * b + c);
        
        /// <summary>
        /// Returns a uVector2 from component-wise application of Add (lhs + rhs).
        /// </summary>
        public static uVector2 Add(uVector2 lhs, uVector2 rhs) => new uVector2(lhs.x + rhs.x, lhs.y + rhs.y);
        
        /// <summary>
        /// Returns a uVector2 from component-wise application of Add (lhs + rhs).
        /// </summary>
        public static uVector2 Add(uVector2 lhs, uint rhs) => new uVector2(lhs.x + rhs, lhs.y + rhs);
        
        /// <summary>
        /// Returns a uVector2 from component-wise application of Add (lhs + rhs).
        /// </summary>
        public static uVector2 Add(uint lhs, uVector2 rhs) => new uVector2(lhs + rhs.x, lhs + rhs.y);
        
        /// <summary>
        /// Returns a uVector from the application of Add (lhs + rhs).
        /// </summary>
        public static uVector2 Add(uint lhs, uint rhs) => new uVector2(lhs + rhs);
        
        /// <summary>
        /// Returns a uVector2 from component-wise application of Sub (lhs - rhs).
        /// </summary>
        public static uVector2 Sub(uVector2 lhs, uVector2 rhs) => new uVector2(lhs.x - rhs.x, lhs.y - rhs.y);
        
        /// <summary>
        /// Returns a uVector2 from component-wise application of Sub (lhs - rhs).
        /// </summary>
        public static uVector2 Sub(uVector2 lhs, uint rhs) => new uVector2(lhs.x - rhs, lhs.y - rhs);
        
        /// <summary>
        /// Returns a uVector2 from component-wise application of Sub (lhs - rhs).
        /// </summary>
        public static uVector2 Sub(uint lhs, uVector2 rhs) => new uVector2(lhs - rhs.x, lhs - rhs.y);
        
        /// <summary>
        /// Returns a uVector from the application of Sub (lhs - rhs).
        /// </summary>
        public static uVector2 Sub(uint lhs, uint rhs) => new uVector2(lhs - rhs);
        
        /// <summary>
        /// Returns a uVector2 from component-wise application of Mul (lhs * rhs).
        /// </summary>
        public static uVector2 Mul(uVector2 lhs, uVector2 rhs) => new uVector2(lhs.x * rhs.x, lhs.y * rhs.y);
        
        /// <summary>
        /// Returns a uVector2 from component-wise application of Mul (lhs * rhs).
        /// </summary>
        public static uVector2 Mul(uVector2 lhs, uint rhs) => new uVector2(lhs.x * rhs, lhs.y * rhs);
        
        /// <summary>
        /// Returns a uVector2 from component-wise application of Mul (lhs * rhs).
        /// </summary>
        public static uVector2 Mul(uint lhs, uVector2 rhs) => new uVector2(lhs * rhs.x, lhs * rhs.y);
        
        /// <summary>
        /// Returns a uVector from the application of Mul (lhs * rhs).
        /// </summary>
        public static uVector2 Mul(uint lhs, uint rhs) => new uVector2(lhs * rhs);
        
        /// <summary>
        /// Returns a uVector2 from component-wise application of Div (lhs / rhs).
        /// </summary>
        public static uVector2 Div(uVector2 lhs, uVector2 rhs) => new uVector2(lhs.x / rhs.x, lhs.y / rhs.y);
        
        /// <summary>
        /// Returns a uVector2 from component-wise application of Div (lhs / rhs).
        /// </summary>
        public static uVector2 Div(uVector2 lhs, uint rhs) => new uVector2(lhs.x / rhs, lhs.y / rhs);
        
        /// <summary>
        /// Returns a uVector2 from component-wise application of Div (lhs / rhs).
        /// </summary>
        public static uVector2 Div(uint lhs, uVector2 rhs) => new uVector2(lhs / rhs.x, lhs / rhs.y);
        
        /// <summary>
        /// Returns a uVector from the application of Div (lhs / rhs).
        /// </summary>
        public static uVector2 Div(uint lhs, uint rhs) => new uVector2(lhs / rhs);
        
        /// <summary>
        /// Returns a uVector2 from component-wise application of Xor (lhs ^ rhs).
        /// </summary>
        public static uVector2 Xor(uVector2 lhs, uVector2 rhs) => new uVector2(lhs.x ^ rhs.x, lhs.y ^ rhs.y);
        
        /// <summary>
        /// Returns a uVector2 from component-wise application of Xor (lhs ^ rhs).
        /// </summary>
        public static uVector2 Xor(uVector2 lhs, uint rhs) => new uVector2(lhs.x ^ rhs, lhs.y ^ rhs);
        
        /// <summary>
        /// Returns a uVector2 from component-wise application of Xor (lhs ^ rhs).
        /// </summary>
        public static uVector2 Xor(uint lhs, uVector2 rhs) => new uVector2(lhs ^ rhs.x, lhs ^ rhs.y);
        
        /// <summary>
        /// Returns a uVector from the application of Xor (lhs ^ rhs).
        /// </summary>
        public static uVector2 Xor(uint lhs, uint rhs) => new uVector2(lhs ^ rhs);
        
        /// <summary>
        /// Returns a uVector2 from component-wise application of BitwiseOr (lhs | rhs).
        /// </summary>
        public static uVector2 BitwiseOr(uVector2 lhs, uVector2 rhs) => new uVector2(lhs.x | rhs.x, lhs.y | rhs.y);
        
        /// <summary>
        /// Returns a uVector2 from component-wise application of BitwiseOr (lhs | rhs).
        /// </summary>
        public static uVector2 BitwiseOr(uVector2 lhs, uint rhs) => new uVector2(lhs.x | rhs, lhs.y | rhs);
        
        /// <summary>
        /// Returns a uVector2 from component-wise application of BitwiseOr (lhs | rhs).
        /// </summary>
        public static uVector2 BitwiseOr(uint lhs, uVector2 rhs) => new uVector2(lhs | rhs.x, lhs | rhs.y);
        
        /// <summary>
        /// Returns a uVector from the application of BitwiseOr (lhs | rhs).
        /// </summary>
        public static uVector2 BitwiseOr(uint lhs, uint rhs) => new uVector2(lhs | rhs);
        
        /// <summary>
        /// Returns a uVector2 from component-wise application of BitwiseAnd (lhs &amp; rhs).
        /// </summary>
        public static uVector2 BitwiseAnd(uVector2 lhs, uVector2 rhs) => new uVector2(lhs.x & rhs.x, lhs.y & rhs.y);
        
        /// <summary>
        /// Returns a uVector2 from component-wise application of BitwiseAnd (lhs &amp; rhs).
        /// </summary>
        public static uVector2 BitwiseAnd(uVector2 lhs, uint rhs) => new uVector2(lhs.x & rhs, lhs.y & rhs);
        
        /// <summary>
        /// Returns a uVector2 from component-wise application of BitwiseAnd (lhs &amp; rhs).
        /// </summary>
        public static uVector2 BitwiseAnd(uint lhs, uVector2 rhs) => new uVector2(lhs & rhs.x, lhs & rhs.y);
        
        /// <summary>
        /// Returns a uVector from the application of BitwiseAnd (lhs &amp; rhs).
        /// </summary>
        public static uVector2 BitwiseAnd(uint lhs, uint rhs) => new uVector2(lhs & rhs);
        
        /// <summary>
        /// Returns a uVector2 from component-wise application of LeftShift (lhs &lt;&lt; rhs).
        /// </summary>
        public static uVector2 LeftShift(uVector2 lhs, iVector2 rhs) => new uVector2(lhs.x << rhs.x, lhs.y << rhs.y);
        
        /// <summary>
        /// Returns a uVector2 from component-wise application of LeftShift (lhs &lt;&lt; rhs).
        /// </summary>
        public static uVector2 LeftShift(uVector2 lhs, int rhs) => new uVector2(lhs.x << rhs, lhs.y << rhs);
        
        /// <summary>
        /// Returns a uVector2 from component-wise application of LeftShift (lhs &lt;&lt; rhs).
        /// </summary>
        public static uVector2 LeftShift(uint lhs, iVector2 rhs) => new uVector2(lhs << rhs.x, lhs << rhs.y);
        
        /// <summary>
        /// Returns a uVector from the application of LeftShift (lhs &lt;&lt; rhs).
        /// </summary>
        public static uVector2 LeftShift(uint lhs, int rhs) => new uVector2(lhs << rhs);
        
        /// <summary>
        /// Returns a uVector2 from component-wise application of RightShift (lhs &gt;&gt; rhs).
        /// </summary>
        public static uVector2 RightShift(uVector2 lhs, iVector2 rhs) => new uVector2(lhs.x >> rhs.x, lhs.y >> rhs.y);
        
        /// <summary>
        /// Returns a uVector2 from component-wise application of RightShift (lhs &gt;&gt; rhs).
        /// </summary>
        public static uVector2 RightShift(uVector2 lhs, int rhs) => new uVector2(lhs.x >> rhs, lhs.y >> rhs);
        
        /// <summary>
        /// Returns a uVector2 from component-wise application of RightShift (lhs &gt;&gt; rhs).
        /// </summary>
        public static uVector2 RightShift(uint lhs, iVector2 rhs) => new uVector2(lhs >> rhs.x, lhs >> rhs.y);
        
        /// <summary>
        /// Returns a uVector from the application of RightShift (lhs &gt;&gt; rhs).
        /// </summary>
        public static uVector2 RightShift(uint lhs, int rhs) => new uVector2(lhs >> rhs);
        
        /// <summary>
        /// Returns a uVector2 with independent and identically distributed uniform integer values between 0 (inclusive) and maxValue (exclusive). (A maxValue of 0 is allowed and returns 0.)
        /// </summary>
        public static uVector2 Random(System.Random random, uVector2 maxValue) => new uVector2((uint)random.Next((int)maxValue.x), (uint)random.Next((int)maxValue.y));
        
        /// <summary>
        /// Returns a uVector2 with independent and identically distributed uniform integer values between 0 (inclusive) and maxValue (exclusive). (A maxValue of 0 is allowed and returns 0.)
        /// </summary>
        public static uVector2 Random(System.Random random, uint maxValue) => new uVector2((uint)random.Next((int)maxValue));
        
        /// <summary>
        /// Returns a uVector2 with independent and identically distributed uniform integer values between minValue (inclusive) and maxValue (exclusive). (minValue == maxValue is allowed and returns minValue. Negative values are allowed.)
        /// </summary>
        public static uVector2 Random(System.Random random, uVector2 minValue, uVector2 maxValue) => new uVector2((uint)random.Next((int)minValue.x, (int)maxValue.x), (uint)random.Next((int)minValue.y, (int)maxValue.y));
        
        /// <summary>
        /// Returns a uVector2 with independent and identically distributed uniform integer values between minValue (inclusive) and maxValue (exclusive). (minValue == maxValue is allowed and returns minValue. Negative values are allowed.)
        /// </summary>
        public static uVector2 Random(System.Random random, uVector2 minValue, uint maxValue) => new uVector2((uint)random.Next((int)minValue.x, (int)maxValue), (uint)random.Next((int)minValue.y, (int)maxValue));
        
        /// <summary>
        /// Returns a uVector2 with independent and identically distributed uniform integer values between minValue (inclusive) and maxValue (exclusive). (minValue == maxValue is allowed and returns minValue. Negative values are allowed.)
        /// </summary>
        public static uVector2 Random(System.Random random, uint minValue, uVector2 maxValue) => new uVector2((uint)random.Next((int)minValue, (int)maxValue.x), (uint)random.Next((int)minValue, (int)maxValue.y));
        
        /// <summary>
        /// Returns a uVector2 with independent and identically distributed uniform integer values between minValue (inclusive) and maxValue (exclusive). (minValue == maxValue is allowed and returns minValue. Negative values are allowed.)
        /// </summary>
        public static uVector2 Random(System.Random random, uint minValue, uint maxValue) => new uVector2((uint)random.Next((int)minValue, (int)maxValue));
        
        /// <summary>
        /// Returns a uVector2 with independent and identically distributed uniform integer values between minValue (inclusive) and maxValue (exclusive). (minValue == maxValue is allowed and returns minValue. Negative values are allowed.)
        /// </summary>
        public static uVector2 RandomUniform(System.Random random, uVector2 minValue, uVector2 maxValue) => new uVector2((uint)random.Next((int)minValue.x, (int)maxValue.x), (uint)random.Next((int)minValue.y, (int)maxValue.y));
        
        /// <summary>
        /// Returns a uVector2 with independent and identically distributed uniform integer values between minValue (inclusive) and maxValue (exclusive). (minValue == maxValue is allowed and returns minValue. Negative values are allowed.)
        /// </summary>
        public static uVector2 RandomUniform(System.Random random, uVector2 minValue, uint maxValue) => new uVector2((uint)random.Next((int)minValue.x, (int)maxValue), (uint)random.Next((int)minValue.y, (int)maxValue));
        
        /// <summary>
        /// Returns a uVector2 with independent and identically distributed uniform integer values between minValue (inclusive) and maxValue (exclusive). (minValue == maxValue is allowed and returns minValue. Negative values are allowed.)
        /// </summary>
        public static uVector2 RandomUniform(System.Random random, uint minValue, uVector2 maxValue) => new uVector2((uint)random.Next((int)minValue, (int)maxValue.x), (uint)random.Next((int)minValue, (int)maxValue.y));
        
        /// <summary>
        /// Returns a uVector2 with independent and identically distributed uniform integer values between minValue (inclusive) and maxValue (exclusive). (minValue == maxValue is allowed and returns minValue. Negative values are allowed.)
        /// </summary>
        public static uVector2 RandomUniform(System.Random random, uint minValue, uint maxValue) => new uVector2((uint)random.Next((int)minValue, (int)maxValue));
        
        /// <summary>
        /// Returns a uVector2 with independent and identically distributed integer values according to a poisson distribution with given lambda parameter.
        /// </summary>
        public static uVector2 RandomPoisson(System.Random random, dVector2 lambda) => new uVector2((uint)lambda.x.GetPoisson(random), (uint)lambda.y.GetPoisson(random));
        
        /// <summary>
        /// Returns a uVector2 with independent and identically distributed integer values according to a poisson distribution with given lambda parameter.
        /// </summary>
        public static uVector2 RandomPoisson(System.Random random, double lambda) => new uVector2((uint)lambda.GetPoisson(random));

        #endregion


        #region Component-Wise Operator Overloads
        
        /// <summary>
        /// Returns a bVector2 from component-wise application of operator&lt; (lhs &lt; rhs).
        /// </summary>
        public static bVector2 operator<(uVector2 lhs, uVector2 rhs) => new bVector2(lhs.x < rhs.x, lhs.y < rhs.y);
        
        /// <summary>
        /// Returns a bVector2 from component-wise application of operator&lt; (lhs &lt; rhs).
        /// </summary>
        public static bVector2 operator<(uVector2 lhs, uint rhs) => new bVector2(lhs.x < rhs, lhs.y < rhs);
        
        /// <summary>
        /// Returns a bVector2 from component-wise application of operator&lt; (lhs &lt; rhs).
        /// </summary>
        public static bVector2 operator<(uint lhs, uVector2 rhs) => new bVector2(lhs < rhs.x, lhs < rhs.y);
        
        /// <summary>
        /// Returns a bVector2 from component-wise application of operator&lt;= (lhs &lt;= rhs).
        /// </summary>
        public static bVector2 operator<=(uVector2 lhs, uVector2 rhs) => new bVector2(lhs.x <= rhs.x, lhs.y <= rhs.y);
        
        /// <summary>
        /// Returns a bVector2 from component-wise application of operator&lt;= (lhs &lt;= rhs).
        /// </summary>
        public static bVector2 operator<=(uVector2 lhs, uint rhs) => new bVector2(lhs.x <= rhs, lhs.y <= rhs);
        
        /// <summary>
        /// Returns a bVector2 from component-wise application of operator&lt;= (lhs &lt;= rhs).
        /// </summary>
        public static bVector2 operator<=(uint lhs, uVector2 rhs) => new bVector2(lhs <= rhs.x, lhs <= rhs.y);
        
        /// <summary>
        /// Returns a bVector2 from component-wise application of operator&gt; (lhs &gt; rhs).
        /// </summary>
        public static bVector2 operator>(uVector2 lhs, uVector2 rhs) => new bVector2(lhs.x > rhs.x, lhs.y > rhs.y);
        
        /// <summary>
        /// Returns a bVector2 from component-wise application of operator&gt; (lhs &gt; rhs).
        /// </summary>
        public static bVector2 operator>(uVector2 lhs, uint rhs) => new bVector2(lhs.x > rhs, lhs.y > rhs);
        
        /// <summary>
        /// Returns a bVector2 from component-wise application of operator&gt; (lhs &gt; rhs).
        /// </summary>
        public static bVector2 operator>(uint lhs, uVector2 rhs) => new bVector2(lhs > rhs.x, lhs > rhs.y);
        
        /// <summary>
        /// Returns a bVector2 from component-wise application of operator&gt;= (lhs &gt;= rhs).
        /// </summary>
        public static bVector2 operator>=(uVector2 lhs, uVector2 rhs) => new bVector2(lhs.x >= rhs.x, lhs.y >= rhs.y);
        
        /// <summary>
        /// Returns a bVector2 from component-wise application of operator&gt;= (lhs &gt;= rhs).
        /// </summary>
        public static bVector2 operator>=(uVector2 lhs, uint rhs) => new bVector2(lhs.x >= rhs, lhs.y >= rhs);
        
        /// <summary>
        /// Returns a bVector2 from component-wise application of operator&gt;= (lhs &gt;= rhs).
        /// </summary>
        public static bVector2 operator>=(uint lhs, uVector2 rhs) => new bVector2(lhs >= rhs.x, lhs >= rhs.y);
        
        /// <summary>
        /// Returns a uVector2 from component-wise application of operator+ (lhs + rhs).
        /// </summary>
        public static uVector2 operator+(uVector2 lhs, uVector2 rhs) => new uVector2(lhs.x + rhs.x, lhs.y + rhs.y);
        
        /// <summary>
        /// Returns a uVector2 from component-wise application of operator+ (lhs + rhs).
        /// </summary>
        public static uVector2 operator+(uVector2 lhs, uint rhs) => new uVector2(lhs.x + rhs, lhs.y + rhs);
        
        /// <summary>
        /// Returns a uVector2 from component-wise application of operator+ (lhs + rhs).
        /// </summary>
        public static uVector2 operator+(uint lhs, uVector2 rhs) => new uVector2(lhs + rhs.x, lhs + rhs.y);
        
        /// <summary>
        /// Returns a uVector2 from component-wise application of operator- (lhs - rhs).
        /// </summary>
        public static uVector2 operator-(uVector2 lhs, uVector2 rhs) => new uVector2(lhs.x - rhs.x, lhs.y - rhs.y);
        
        /// <summary>
        /// Returns a uVector2 from component-wise application of operator- (lhs - rhs).
        /// </summary>
        public static uVector2 operator-(uVector2 lhs, uint rhs) => new uVector2(lhs.x - rhs, lhs.y - rhs);
        
        /// <summary>
        /// Returns a uVector2 from component-wise application of operator- (lhs - rhs).
        /// </summary>
        public static uVector2 operator-(uint lhs, uVector2 rhs) => new uVector2(lhs - rhs.x, lhs - rhs.y);
        
        /// <summary>
        /// Returns a uVector2 from component-wise application of operator* (lhs * rhs).
        /// </summary>
        public static uVector2 operator*(uVector2 lhs, uVector2 rhs) => new uVector2(lhs.x * rhs.x, lhs.y * rhs.y);
        
        /// <summary>
        /// Returns a uVector2 from component-wise application of operator* (lhs * rhs).
        /// </summary>
        public static uVector2 operator*(uVector2 lhs, uint rhs) => new uVector2(lhs.x * rhs, lhs.y * rhs);
        
        /// <summary>
        /// Returns a uVector2 from component-wise application of operator* (lhs * rhs).
        /// </summary>
        public static uVector2 operator*(uint lhs, uVector2 rhs) => new uVector2(lhs * rhs.x, lhs * rhs.y);
        
        /// <summary>
        /// Returns a uVector2 from component-wise application of operator/ (lhs / rhs).
        /// </summary>
        public static uVector2 operator/(uVector2 lhs, uVector2 rhs) => new uVector2(lhs.x / rhs.x, lhs.y / rhs.y);
        
        /// <summary>
        /// Returns a uVector2 from component-wise application of operator/ (lhs / rhs).
        /// </summary>
        public static uVector2 operator/(uVector2 lhs, uint rhs) => new uVector2(lhs.x / rhs, lhs.y / rhs);
        
        /// <summary>
        /// Returns a uVector2 from component-wise application of operator/ (lhs / rhs).
        /// </summary>
        public static uVector2 operator/(uint lhs, uVector2 rhs) => new uVector2(lhs / rhs.x, lhs / rhs.y);
        
        /// <summary>
        /// Returns a uVector2 from component-wise application of operator+ (identity).
        /// </summary>
        public static uVector2 operator+(uVector2 v) => v;
        
        /// <summary>
        /// Returns a uVector2 from component-wise application of operator~ (~v).
        /// </summary>
        public static uVector2 operator~(uVector2 v) => new uVector2(~v.x, ~v.y);
        
        /// <summary>
        /// Returns a uVector2 from component-wise application of operator^ (lhs ^ rhs).
        /// </summary>
        public static uVector2 operator^(uVector2 lhs, uVector2 rhs) => new uVector2(lhs.x ^ rhs.x, lhs.y ^ rhs.y);
        
        /// <summary>
        /// Returns a uVector2 from component-wise application of operator^ (lhs ^ rhs).
        /// </summary>
        public static uVector2 operator^(uVector2 lhs, uint rhs) => new uVector2(lhs.x ^ rhs, lhs.y ^ rhs);
        
        /// <summary>
        /// Returns a uVector2 from component-wise application of operator^ (lhs ^ rhs).
        /// </summary>
        public static uVector2 operator^(uint lhs, uVector2 rhs) => new uVector2(lhs ^ rhs.x, lhs ^ rhs.y);
        
        /// <summary>
        /// Returns a uVector2 from component-wise application of operator| (lhs | rhs).
        /// </summary>
        public static uVector2 operator|(uVector2 lhs, uVector2 rhs) => new uVector2(lhs.x | rhs.x, lhs.y | rhs.y);
        
        /// <summary>
        /// Returns a uVector2 from component-wise application of operator| (lhs | rhs).
        /// </summary>
        public static uVector2 operator|(uVector2 lhs, uint rhs) => new uVector2(lhs.x | rhs, lhs.y | rhs);
        
        /// <summary>
        /// Returns a uVector2 from component-wise application of operator| (lhs | rhs).
        /// </summary>
        public static uVector2 operator|(uint lhs, uVector2 rhs) => new uVector2(lhs | rhs.x, lhs | rhs.y);
        
        /// <summary>
        /// Returns a uVector2 from component-wise application of operator&amp; (lhs &amp; rhs).
        /// </summary>
        public static uVector2 operator&(uVector2 lhs, uVector2 rhs) => new uVector2(lhs.x & rhs.x, lhs.y & rhs.y);
        
        /// <summary>
        /// Returns a uVector2 from component-wise application of operator&amp; (lhs &amp; rhs).
        /// </summary>
        public static uVector2 operator&(uVector2 lhs, uint rhs) => new uVector2(lhs.x & rhs, lhs.y & rhs);
        
        /// <summary>
        /// Returns a uVector2 from component-wise application of operator&amp; (lhs &amp; rhs).
        /// </summary>
        public static uVector2 operator&(uint lhs, uVector2 rhs) => new uVector2(lhs & rhs.x, lhs & rhs.y);
        
        /// <summary>
        /// Returns a uVector2 from component-wise application of operator&lt;&lt; (lhs &lt;&lt; rhs).
        /// </summary>
        public static uVector2 operator<<(uVector2 lhs, int rhs) => new uVector2(lhs.x << rhs, lhs.y << rhs);
        
        /// <summary>
        /// Returns a uVector2 from component-wise application of operator&gt;&gt; (lhs &gt;&gt; rhs).
        /// </summary>
        public static uVector2 operator>>(uVector2 lhs, int rhs) => new uVector2(lhs.x >> rhs, lhs.y >> rhs);

        #endregion

    }
}
