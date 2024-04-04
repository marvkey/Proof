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
    /// A Vectortor of type float with 2 components.
    /// </summary>
    [RegisterCoreClassStruct]
    
    [StructLayout(LayoutKind.Sequential)]
    public struct Vector2 : IReadOnlyList<float>, IEquatable<Vector2>
    {

        #region Fields
        
        /// <summary>
        /// x-component
        /// </summary>
        
        public float x;
        
        /// <summary>
        /// y-component
        /// </summary>
        
        public float y;

        #endregion


        #region Constructors
        
        /// <summary>
        /// Component-wise constructor
        /// </summary>
        public Vector2(float x, float y)
        {
            this.x = x;
            this.y = y;
        }
        
        /// <summary>
        /// all-same-value constructor
        /// </summary>
        public Vector2(float v)
        {
            this.x = v;
            this.y = v;
        }
        
        /// <summary>
        /// from-Vectortor constructor
        /// </summary>
        public Vector2(Vector2 v)
        {
            this.x = v.x;
            this.y = v.y;
        }
        
        /// <summary>
        /// from-Vectortor constructor (additional fields are truncated)
        /// </summary>
        public Vector2(Vector3 v)
        {
            this.x = v.x;
            this.y = v.y;
        }
        
        /// <summary>
        /// from-Vectortor constructor (additional fields are truncated)
        /// </summary>
        public Vector2(Vector4 v)
        {
            this.x = v.x;
            this.y = v.y;
        }
        
        /// <summary>
        /// From-array/list constructor (superfluous values are ignored, missing values are zero-filled).
        /// </summary>
        public Vector2(IReadOnlyList<float> v)
        {
            var c = v.Count;
            this.x = c < 0 ? 0f : v[0];
            this.y = c < 1 ? 0f : v[1];
        }
        
        /// <summary>
        /// Generic from-array constructor (superfluous values are ignored, missing values are zero-filled).
        /// </summary>
        public Vector2(Object[] v)
        {
            var c = v.Length;
            this.x = c < 0 ? 0f : (float)v[0];
            this.y = c < 1 ? 0f : (float)v[1];
        }
        
        /// <summary>
        /// From-array constructor (superfluous values are ignored, missing values are zero-filled).
        /// </summary>
        public Vector2(float[] v)
        {
            var c = v.Length;
            this.x = c < 0 ? 0f : v[0];
            this.y = c < 1 ? 0f : v[1];
        }
        
        /// <summary>
        /// From-array constructor with base index (superfluous values are ignored, missing values are zero-filled).
        /// </summary>
        public Vector2(float[] v, int startIndex)
        {
            var c = v.Length;
            this.x = c + startIndex < 0 ? 0f : v[0 + startIndex];
            this.y = c + startIndex < 1 ? 0f : v[1 + startIndex];
        }
        
        /// <summary>
        /// From-IEnumerable constructor (superfluous values are ignored, missing values are zero-filled).
        /// </summary>
        public Vector2(IEnumerable<float> v)
            : this(v.ToArray())
        {
        }

        #endregion


        #region Implicit Operators
        
        /// <summary>
        /// Implicitly converts this to a dVector2.
        /// </summary>
        public static implicit operator dVector2(Vector2 v) => new dVector2((double)v.x, (double)v.y);
        
        /// <summary>
        /// Implicitly converts this to a cVector2.
        /// </summary>
        //public static implicit operator cVector2(Vector2 v) => new cVector2((Complex)v.x, (Complex)v.y);

        #endregion


        #region Explicit Operators
        
        /// <summary>
        /// Explicitly converts this to a iVector2.
        /// </summary>
        public static explicit operator iVector2(Vector2 v) => new iVector2((int)v.x, (int)v.y);
        
        /// <summary>
        /// Explicitly converts this to a iVector3. (Higher components are zeroed)
        /// </summary>
        public static explicit operator iVector3(Vector2 v) => new iVector3((int)v.x, (int)v.y, 0);
        
        /// <summary>
        /// Explicitly converts this to a iVector4. (Higher components are zeroed)
        /// </summary>
        public static explicit operator iVector4(Vector2 v) => new iVector4((int)v.x, (int)v.y, 0, 0);
        
        /// <summary>
        /// Explicitly converts this to a uVector2.
        /// </summary>
        public static explicit operator uVector2(Vector2 v) => new uVector2((uint)v.x, (uint)v.y);
        
        /// <summary>
        /// Explicitly converts this to a uVector3. (Higher components are zeroed)
        /// </summary>
        public static explicit operator uVector3(Vector2 v) => new uVector3((uint)v.x, (uint)v.y, 0u);
        
        /// <summary>
        /// Explicitly converts this to a uVector4. (Higher components are zeroed)
        /// </summary>
        public static explicit operator uVector4(Vector2 v) => new uVector4((uint)v.x, (uint)v.y, 0u, 0u);
        
        /// <summary>
        /// Explicitly converts this to a Vector3. (Higher components are zeroed)
        /// </summary>
        public static explicit operator Vector3(Vector2 v) => new Vector3((float)v.x, (float)v.y, 0f);
        
        /// <summary>
        /// Explicitly converts this to a Vector4. (Higher components are zeroed)
        /// </summary>
        public static explicit operator Vector4(Vector2 v) => new Vector4((float)v.x, (float)v.y, 0f, 0f);
        
        /// <summary>
        /// Explicitly converts this to a hVector2.
        /// </summary>
        public static explicit operator hVector2(Vector2 v) => new hVector2((Half)v.x, (Half)v.y);
        
        /// <summary>
        /// Explicitly converts this to a hVector3. (Higher components are zeroed)
        /// </summary>
        public static explicit operator hVector3(Vector2 v) => new hVector3((Half)v.x, (Half)v.y, Half.Zero);
        
        /// <summary>
        /// Explicitly converts this to a hVector4. (Higher components are zeroed)
        /// </summary>
        public static explicit operator hVector4(Vector2 v) => new hVector4((Half)v.x, (Half)v.y, Half.Zero, Half.Zero);
        
        /// <summary>
        /// Explicitly converts this to a dVector3. (Higher components are zeroed)
        /// </summary>
        public static explicit operator dVector3(Vector2 v) => new dVector3((double)v.x, (double)v.y, 0.0);
        
        /// <summary>
        /// Explicitly converts this to a dVector4. (Higher components are zeroed)
        /// </summary>
        public static explicit operator dVector4(Vector2 v) => new dVector4((double)v.x, (double)v.y, 0.0, 0.0);
        
        /// <summary>
        /// Explicitly converts this to a decVector2.
        /// </summary>
        public static explicit operator decVector2(Vector2 v) => new decVector2((decimal)v.x, (decimal)v.y);
        
        /// <summary>
        /// Explicitly converts this to a decVector3. (Higher components are zeroed)
        /// </summary>
        public static explicit operator decVector3(Vector2 v) => new decVector3((decimal)v.x, (decimal)v.y, 0m);
        
        /// <summary>
        /// Explicitly converts this to a decVector4. (Higher components are zeroed)
        /// </summary>
        public static explicit operator decVector4(Vector2 v) => new decVector4((decimal)v.x, (decimal)v.y, 0m, 0m);
        
        /// <summary>
        /// Explicitly converts this to a cVector3. (Higher components are zeroed)
        /// </summary>
        //public static explicit operator cVector3(Vector2 v) => new cVector3((Complex)v.x, (Complex)v.y, Complex.Zero);
        
        /// <summary>
        /// Explicitly converts this to a cVector4. (Higher components are zeroed)
        /// </summary>
        //public static explicit operator cVector4(Vector2 v) => new cVector4((Complex)v.x, (Complex)v.y, Complex.Zero, Complex.Zero);
        
        /// <summary>
        /// Explicitly converts this to a lVector2.
        /// </summary>
        public static explicit operator lVector2(Vector2 v) => new lVector2((long)v.x, (long)v.y);
        
        /// <summary>
        /// Explicitly converts this to a lVector3. (Higher components are zeroed)
        /// </summary>
        public static explicit operator lVector3(Vector2 v) => new lVector3((long)v.x, (long)v.y, 0);
        
        /// <summary>
        /// Explicitly converts this to a lVector4. (Higher components are zeroed)
        /// </summary>
        public static explicit operator lVector4(Vector2 v) => new lVector4((long)v.x, (long)v.y, 0, 0);
        
        /// <summary>
        /// Explicitly converts this to a bVector2.
        /// </summary>
        public static explicit operator bVector2(Vector2 v) => new bVector2(v.x != 0f, v.y != 0f);
        
        /// <summary>
        /// Explicitly converts this to a bVector3. (Higher components are zeroed)
        /// </summary>
        public static explicit operator bVector3(Vector2 v) => new bVector3(v.x != 0f, v.y != 0f, false);
        
        /// <summary>
        /// Explicitly converts this to a bVector4. (Higher components are zeroed)
        /// </summary>
        public static explicit operator bVector4(Vector2 v) => new bVector4(v.x != 0f, v.y != 0f, false, false);
        
        /// <summary>
        /// Explicitly converts this to a float array.
        /// </summary>
        public static explicit operator float[](Vector2 v) => new [] { v.x, v.y };
        
        /// <summary>
        /// Explicitly converts this to a generic object array.
        /// </summary>
        public static explicit operator Object[](Vector2 v) => new Object[] { v.x, v.y };

        #endregion


        #region Indexer
        
        /// <summary>
        /// Gets/Sets a specific indexed component (a bit slower than direct access).
        /// </summary>
        public float this[int index]
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
        public swizzle_Vector2 swizzle => new swizzle_Vector2(x, y);
        
        /// <summary>
        /// Gets or sets the specified subset of components. For more advanced (read-only) swizzling, use the .swizzle property.
        /// </summary>
        public Vector2 xy
        {
            get
            {
                return new Vector2(x, y);
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
        public Vector2 rg
        {
            get
            {
                return new Vector2(x, y);
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
        public float r
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
        public float g
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
        public float[] Values => new[] { x, y };
        
        /// <summary>
        /// Returns the number of components (2).
        /// </summary>
        public int Count => 2;
        
        /// <summary>
        /// Returns the minimal component of this Vectortor.
        /// </summary>
        public float MinElement => Math.Min(x, y);
        
        /// <summary>
        /// Returns the maximal component of this Vectortor.
        /// </summary>
        public float MaxElement => Math.Max(x, y);
        
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
        public float Sum => (x + y);
        
        /// <summary>
        /// Returns the euclidean norm of this Vectortor.
        /// </summary>
        public float Norm => (float)Math.Sqrt((x*x + y*y));
        
        /// <summary>
        /// Returns the one-norm of this Vectortor.
        /// </summary>
        public float Norm1 => (Math.Abs(x) + Math.Abs(y));
        
        /// <summary>
        /// Returns the two-norm (euclidean length) of this Vectortor.
        /// </summary>
        public float Norm2 => (float)Math.Sqrt((x*x + y*y));
        
        /// <summary>
        /// Returns the max-norm of this Vectortor.
        /// </summary>
        public float NormMax => Math.Max(Math.Abs(x), Math.Abs(y));
        
        /// <summary>
        /// Returns a copy of this Vectortor with length one (undefined if this has zero length).
        /// </summary>
        public Vector2 Normalized => this / (float)Length;
        
        /// <summary>
        /// Returns a copy of this Vectortor with length one (returns zero if length is zero).
        /// </summary>
        public Vector2 NormalizedSafe => this == Zero ? Zero : this / (float)Length;
        
        /// <summary>
        /// Returns the Vectortor angle (atan2(y, x)) in radians.
        /// </summary>
        public double Angle => Math.Atan2((double)y, (double)x);

        #endregion


        #region Static Properties
        
        /// <summary>
        /// Predefined all-zero Vectortor
        /// </summary>
        public static Vector2 Zero { get; } = new Vector2(0f, 0f);
        
        /// <summary>
        /// Predefined all-ones Vectortor
        /// </summary>
        public static Vector2 Ones { get; } = new Vector2(1f, 1f);
        
        /// <summary>
        /// Predefined unit-X Vectortor
        /// </summary>
        public static Vector2 UnitX { get; } = new Vector2(1f, 0f);
        
        /// <summary>
        /// Predefined unit-Y Vectortor
        /// </summary>
        public static Vector2 UnitY { get; } = new Vector2(0f, 1f);
        
        /// <summary>
        /// Predefined all-MaxValue Vectortor
        /// </summary>
        public static Vector2 MaxValue { get; } = new Vector2(float.MaxValue, float.MaxValue);
        
        /// <summary>
        /// Predefined all-MinValue Vectortor
        /// </summary>
        public static Vector2 MinValue { get; } = new Vector2(float.MinValue, float.MinValue);
        
        /// <summary>
        /// Predefined all-Epsilon Vectortor
        /// </summary>
        public static Vector2 Epsilon { get; } = new Vector2(float.Epsilon, float.Epsilon);
        
        /// <summary>
        /// Predefined all-NaN Vectortor
        /// </summary>
        public static Vector2 NaN { get; } = new Vector2(float.NaN, float.NaN);
        
        /// <summary>
        /// Predefined all-NegativeInfinity Vectortor
        /// </summary>
        public static Vector2 NegativeInfinity { get; } = new Vector2(float.NegativeInfinity, float.NegativeInfinity);
        
        /// <summary>
        /// Predefined all-PositiveInfinity Vectortor
        /// </summary>
        public static Vector2 PositiveInfinity { get; } = new Vector2(float.PositiveInfinity, float.PositiveInfinity);

        #endregion


        #region Operators
        
        /// <summary>
        /// Returns true iff this equals rhs component-wise.
        /// </summary>
        public static bool operator==(Vector2 lhs, Vector2 rhs) => lhs.Equals(rhs);
        
        /// <summary>
        /// Returns true iff this does not equal rhs (component-wise).
        /// </summary>
        public static bool operator!=(Vector2 lhs, Vector2 rhs) => !lhs.Equals(rhs);

        #endregion


        #region Functions
        
        /// <summary>
        /// Returns an enumerator that iterates through all components.
        /// </summary>
        public IEnumerator<float> GetEnumerator()
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
        public bool Equals(Vector2 rhs) => (x.Equals(rhs.x) && y.Equals(rhs.y));
        
        /// <summary>
        /// Returns true iff this equals rhs type- and component-wise.
        /// </summary>
        public override bool Equals(object obj)
        {
            if (ReferenceEquals(null, obj)) return false;
            return obj is Vector2 && Equals((Vector2) obj);
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
        public Vector2 Rotated(double angleInRad) => (Vector2)(dVector2.FromAngle(Angle + angleInRad) * (double)Length);

        #endregion


        #region Static Functions
        
        /// <summary>
        /// Converts the string representation of the Vectortor into a Vectortor representation (using ', ' as a separator).
        /// </summary>
        public static Vector2 Parse(string s) => Parse(s, ", ");
        
        /// <summary>
        /// Converts the string representation of the Vectortor into a Vectortor representation (using a designated separator).
        /// </summary>
        public static Vector2 Parse(string s, string sep)
        {
            var kvp = s.Split(new[] { sep }, StringSplitOptions.None);
            if (kvp.Length != 2) throw new FormatException("input has not exactly 2 parts");
            return new Vector2(float.Parse(kvp[0].Trim()), float.Parse(kvp[1].Trim()));
        }
        
        /// <summary>
        /// Converts the string representation of the Vectortor into a Vectortor representation (using a designated separator and a type provider).
        /// </summary>
        public static Vector2 Parse(string s, string sep, IFormatProvider provider)
        {
            var kvp = s.Split(new[] { sep }, StringSplitOptions.None);
            if (kvp.Length != 2) throw new FormatException("input has not exactly 2 parts");
            return new Vector2(float.Parse(kvp[0].Trim(), provider), float.Parse(kvp[1].Trim(), provider));
        }
        
        /// <summary>
        /// Converts the string representation of the Vectortor into a Vectortor representation (using a designated separator and a number style).
        /// </summary>
        public static Vector2 Parse(string s, string sep, NumberStyles style)
        {
            var kvp = s.Split(new[] { sep }, StringSplitOptions.None);
            if (kvp.Length != 2) throw new FormatException("input has not exactly 2 parts");
            return new Vector2(float.Parse(kvp[0].Trim(), style), float.Parse(kvp[1].Trim(), style));
        }
        
        /// <summary>
        /// Converts the string representation of the Vectortor into a Vectortor representation (using a designated separator and a number style and a forMatrix provider).
        /// </summary>
        public static Vector2 Parse(string s, string sep, NumberStyles style, IFormatProvider provider)
        {
            var kvp = s.Split(new[] { sep }, StringSplitOptions.None);
            if (kvp.Length != 2) throw new FormatException("input has not exactly 2 parts");
            return new Vector2(float.Parse(kvp[0].Trim(), style, provider), float.Parse(kvp[1].Trim(), style, provider));
        }
        
        /// <summary>
        /// Tries to convert the string representation of the Vectortor into a Vectortor representation (using ', ' as a separator), returns false if string was invalid.
        /// </summary>
        public static bool TryParse(string s, out Vector2 result) => TryParse(s, ", ", out result);
        
        /// <summary>
        /// Tries to convert the string representation of the Vectortor into a Vectortor representation (using a designated separator), returns false if string was invalid.
        /// </summary>
        public static bool TryParse(string s, string sep, out Vector2 result)
        {
            result = Zero;
            if (string.IsNullOrEmpty(s)) return false;
            var kvp = s.Split(new[] { sep }, StringSplitOptions.None);
            if (kvp.Length != 2) return false;
            float x = 0f, y = 0f;
            var ok = (float.TryParse(kvp[0].Trim(), out x) && float.TryParse(kvp[1].Trim(), out y));
            result = ok ? new Vector2(x, y) : Zero;
            return ok;
        }
        
        /// <summary>
        /// Tries to convert the string representation of the Vectortor into a Vectortor representation (using a designated separator and a number style and a forMatrix provider), returns false if string was invalid.
        /// </summary>
        public static bool TryParse(string s, string sep, NumberStyles style, IFormatProvider provider, out Vector2 result)
        {
            result = Zero;
            if (string.IsNullOrEmpty(s)) return false;
            var kvp = s.Split(new[] { sep }, StringSplitOptions.None);
            if (kvp.Length != 2) return false;
            float x = 0f, y = 0f;
            var ok = (float.TryParse(kvp[0].Trim(), style, provider, out x) && float.TryParse(kvp[1].Trim(), style, provider, out y));
            result = ok ? new Vector2(x, y) : Zero;
            return ok;
        }
        
        /// <summary>
        /// Returns true iff distance between lhs and rhs is less than or equal to epsilon
        /// </summary>
        public static bool ApproxEqual(Vector2 lhs, Vector2 rhs, float eps = 0.1f) => Distance(lhs, rhs) <= eps;
        
        /// <summary>
        /// OuterProduct treats the first parameter c as a column Vectortor (Matrix with one column) and the second parameter r as a row Vectortor (Matrix with one row) and does a linear algebraic Matrix multiply c * r, yielding a Matrix whose number of rows is the number of components in c and whose number of columns is the number of components in r.
        /// </summary>
        public static Matrix2 OuterProduct(Vector2 c, Vector2 r) => new Matrix2(c.x * r.x, c.y * r.x, c.x * r.y, c.y * r.y);
        
        /// <summary>
        /// OuterProduct treats the first parameter c as a column Vectortor (Matrix with one column) and the second parameter r as a row Vectortor (Matrix with one row) and does a linear algebraic Matrix multiply c * r, yielding a Matrix whose number of rows is the number of components in c and whose number of columns is the number of components in r.
        /// </summary>
        public static Matrix2x3 OuterProduct(Vector3 c, Vector2 r) => new Matrix2x3(c.x * r.x, c.y * r.x, c.z * r.x, c.x * r.y, c.y * r.y, c.z * r.y);
        
        /// <summary>
        /// OuterProduct treats the first parameter c as a column Vectortor (Matrix with one column) and the second parameter r as a row Vectortor (Matrix with one row) and does a linear algebraic Matrix multiply c * r, yielding a Matrix whose number of rows is the number of components in c and whose number of columns is the number of components in r.
        /// </summary>
        public static Matrix3x2 OuterProduct(Vector2 c, Vector3 r) => new Matrix3x2(c.x * r.x, c.y * r.x, c.x * r.y, c.y * r.y, c.x * r.z, c.y * r.z);
        
        /// <summary>
        /// OuterProduct treats the first parameter c as a column Vectortor (Matrix with one column) and the second parameter r as a row Vectortor (Matrix with one row) and does a linear algebraic Matrix multiply c * r, yielding a Matrix whose number of rows is the number of components in c and whose number of columns is the number of components in r.
        /// </summary>
        public static Matrix2x4 OuterProduct(Vector4 c, Vector2 r) => new Matrix2x4(c.x * r.x, c.y * r.x, c.z * r.x, c.w * r.x, c.x * r.y, c.y * r.y, c.z * r.y, c.w * r.y);
        
        /// <summary>
        /// OuterProduct treats the first parameter c as a column Vectortor (Matrix with one column) and the second parameter r as a row Vectortor (Matrix with one row) and does a linear algebraic Matrix multiply c * r, yielding a Matrix whose number of rows is the number of components in c and whose number of columns is the number of components in r.
        /// </summary>
        public static Matrix4x2 OuterProduct(Vector2 c, Vector4 r) => new Matrix4x2(c.x * r.x, c.y * r.x, c.x * r.y, c.y * r.y, c.x * r.z, c.y * r.z, c.x * r.w, c.y * r.w);
        
        /// <summary>
        /// Returns a unit 2D Vectortor with a given angle in radians (CAUTION: result may be truncated for integer types).
        /// </summary>
        public static Vector2 FromAngle(double angleInRad) => new Vector2((float)Math.Cos(angleInRad), (float)Math.Sin(angleInRad));
        
        /// <summary>
        /// Returns the inner product (dot product, scalar product) of the two Vectortors.
        /// </summary>
        public static float Dot(Vector2 lhs, Vector2 rhs) => (lhs.x * rhs.x + lhs.y * rhs.y);
        
        /// <summary>
        /// Returns the euclidean distance between the two Vectortors.
        /// </summary>
        public static float Distance(Vector2 lhs, Vector2 rhs) => (lhs - rhs).Length;
        
        /// <summary>
        /// Returns the squared euclidean distance between the two Vectortors.
        /// </summary>
        public static float DistanceSqr(Vector2 lhs, Vector2 rhs) => (lhs - rhs).LengthSqr;
        
        /// <summary>
        /// Calculate the reflection direction for an incident Vectortor (N should be normalized in order to achieve the desired result).
        /// </summary>
        public static Vector2 Reflect(Vector2 I, Vector2 N) => I - 2 * Dot(N, I) * N;
        
        /// <summary>
        /// Calculate the refraction direction for an incident Vectortor (The input parameters I and N should be normalized in order to achieve the desired result).
        /// </summary>
        public static Vector2 Refract(Vector2 I, Vector2 N, float eta)
        {
            var dNI = Dot(N, I);
            var k = 1 - eta * eta * (1 - dNI * dNI);
            if (k < 0) return Zero;
            return eta * I - (eta * dNI + (float)Math.Sqrt(k)) * N;
        }
        
        /// <summary>
        /// Returns a Vectortor pointing in the same direction as another (faceforward orients a Vectortor to point away from a surface as defined by its normal. If dot(Nref, I) is negative faceforward returns N, otherwise it returns -N).
        /// </summary>
        public static Vector2 FaceForward(Vector2 N, Vector2 I, Vector2 Nref) => Dot(Nref, I) < 0 ? N : -N;
        
        /// <summary>
        /// Returns the length of the outer product (cross product, Vectortor product) of the two Vectortors.
        /// </summary>
        public static float Cross(Vector2 l, Vector2 r) => l.x * r.y - l.y * r.x;
        
        /// <summary>
        /// Returns a Vector2 with independent and identically distributed uniform values between 0.0 and 1.0.
        /// </summary>
        public static Vector2 Random(System.Random random) => new Vector2((float)random.NextDouble(), (float)random.NextDouble());
        
        /// <summary>
        /// Returns a Vector2 with independent and identically distributed uniform values between -1.0 and 1.0.
        /// </summary>
        public static Vector2 RandomSigned(System.Random random) => new Vector2((float)(random.NextDouble() * 2.0 - 1.0), (float)(random.NextDouble() * 2.0 - 1.0));
        
        /// <summary>
        /// Returns a Vector2 with independent and identically distributed values according to a normal distribution (zero mean, unit variance).
        /// </summary>
        public static Vector2 RandomNormal(System.Random random) => new Vector2((float)(Math.Cos(2 * Math.PI * random.NextDouble()) * Math.Sqrt(-2.0 * Math.Log(random.NextDouble()))), (float)(Math.Cos(2 * Math.PI * random.NextDouble()) * Math.Sqrt(-2.0 * Math.Log(random.NextDouble()))));

        #endregion


        #region Component-Wise Static Functions
        
        /// <summary>
        /// Returns a bVector2 from component-wise application of Equal (lhs == rhs).
        /// </summary>
        public static bVector2 Equal(Vector2 lhs, Vector2 rhs) => new bVector2(lhs.x == rhs.x, lhs.y == rhs.y);
        
        /// <summary>
        /// Returns a bVector2 from component-wise application of Equal (lhs == rhs).
        /// </summary>
        public static bVector2 Equal(Vector2 lhs, float rhs) => new bVector2(lhs.x == rhs, lhs.y == rhs);
        
        /// <summary>
        /// Returns a bVector2 from component-wise application of Equal (lhs == rhs).
        /// </summary>
        public static bVector2 Equal(float lhs, Vector2 rhs) => new bVector2(lhs == rhs.x, lhs == rhs.y);
        
        /// <summary>
        /// Returns a bVector from the application of Equal (lhs == rhs).
        /// </summary>
        public static bVector2 Equal(float lhs, float rhs) => new bVector2(lhs == rhs);
        
        /// <summary>
        /// Returns a bVector2 from component-wise application of NotEqual (lhs != rhs).
        /// </summary>
        public static bVector2 NotEqual(Vector2 lhs, Vector2 rhs) => new bVector2(lhs.x != rhs.x, lhs.y != rhs.y);
        
        /// <summary>
        /// Returns a bVector2 from component-wise application of NotEqual (lhs != rhs).
        /// </summary>
        public static bVector2 NotEqual(Vector2 lhs, float rhs) => new bVector2(lhs.x != rhs, lhs.y != rhs);
        
        /// <summary>
        /// Returns a bVector2 from component-wise application of NotEqual (lhs != rhs).
        /// </summary>
        public static bVector2 NotEqual(float lhs, Vector2 rhs) => new bVector2(lhs != rhs.x, lhs != rhs.y);
        
        /// <summary>
        /// Returns a bVector from the application of NotEqual (lhs != rhs).
        /// </summary>
        public static bVector2 NotEqual(float lhs, float rhs) => new bVector2(lhs != rhs);
        
        /// <summary>
        /// Returns a bVector2 from component-wise application of GreaterThan (lhs &gt; rhs).
        /// </summary>
        public static bVector2 GreaterThan(Vector2 lhs, Vector2 rhs) => new bVector2(lhs.x > rhs.x, lhs.y > rhs.y);
        
        /// <summary>
        /// Returns a bVector2 from component-wise application of GreaterThan (lhs &gt; rhs).
        /// </summary>
        public static bVector2 GreaterThan(Vector2 lhs, float rhs) => new bVector2(lhs.x > rhs, lhs.y > rhs);
        
        /// <summary>
        /// Returns a bVector2 from component-wise application of GreaterThan (lhs &gt; rhs).
        /// </summary>
        public static bVector2 GreaterThan(float lhs, Vector2 rhs) => new bVector2(lhs > rhs.x, lhs > rhs.y);
        
        /// <summary>
        /// Returns a bVector from the application of GreaterThan (lhs &gt; rhs).
        /// </summary>
        public static bVector2 GreaterThan(float lhs, float rhs) => new bVector2(lhs > rhs);
        
        /// <summary>
        /// Returns a bVector2 from component-wise application of GreaterThanEqual (lhs &gt;= rhs).
        /// </summary>
        public static bVector2 GreaterThanEqual(Vector2 lhs, Vector2 rhs) => new bVector2(lhs.x >= rhs.x, lhs.y >= rhs.y);
        
        /// <summary>
        /// Returns a bVector2 from component-wise application of GreaterThanEqual (lhs &gt;= rhs).
        /// </summary>
        public static bVector2 GreaterThanEqual(Vector2 lhs, float rhs) => new bVector2(lhs.x >= rhs, lhs.y >= rhs);
        
        /// <summary>
        /// Returns a bVector2 from component-wise application of GreaterThanEqual (lhs &gt;= rhs).
        /// </summary>
        public static bVector2 GreaterThanEqual(float lhs, Vector2 rhs) => new bVector2(lhs >= rhs.x, lhs >= rhs.y);
        
        /// <summary>
        /// Returns a bVector from the application of GreaterThanEqual (lhs &gt;= rhs).
        /// </summary>
        public static bVector2 GreaterThanEqual(float lhs, float rhs) => new bVector2(lhs >= rhs);
        
        /// <summary>
        /// Returns a bVector2 from component-wise application of LesserThan (lhs &lt; rhs).
        /// </summary>
        public static bVector2 LesserThan(Vector2 lhs, Vector2 rhs) => new bVector2(lhs.x < rhs.x, lhs.y < rhs.y);
        
        /// <summary>
        /// Returns a bVector2 from component-wise application of LesserThan (lhs &lt; rhs).
        /// </summary>
        public static bVector2 LesserThan(Vector2 lhs, float rhs) => new bVector2(lhs.x < rhs, lhs.y < rhs);
        
        /// <summary>
        /// Returns a bVector2 from component-wise application of LesserThan (lhs &lt; rhs).
        /// </summary>
        public static bVector2 LesserThan(float lhs, Vector2 rhs) => new bVector2(lhs < rhs.x, lhs < rhs.y);
        
        /// <summary>
        /// Returns a bVector from the application of LesserThan (lhs &lt; rhs).
        /// </summary>
        public static bVector2 LesserThan(float lhs, float rhs) => new bVector2(lhs < rhs);
        
        /// <summary>
        /// Returns a bVector2 from component-wise application of LesserThanEqual (lhs &lt;= rhs).
        /// </summary>
        public static bVector2 LesserThanEqual(Vector2 lhs, Vector2 rhs) => new bVector2(lhs.x <= rhs.x, lhs.y <= rhs.y);
        
        /// <summary>
        /// Returns a bVector2 from component-wise application of LesserThanEqual (lhs &lt;= rhs).
        /// </summary>
        public static bVector2 LesserThanEqual(Vector2 lhs, float rhs) => new bVector2(lhs.x <= rhs, lhs.y <= rhs);
        
        /// <summary>
        /// Returns a bVector2 from component-wise application of LesserThanEqual (lhs &lt;= rhs).
        /// </summary>
        public static bVector2 LesserThanEqual(float lhs, Vector2 rhs) => new bVector2(lhs <= rhs.x, lhs <= rhs.y);
        
        /// <summary>
        /// Returns a bVector from the application of LesserThanEqual (lhs &lt;= rhs).
        /// </summary>
        public static bVector2 LesserThanEqual(float lhs, float rhs) => new bVector2(lhs <= rhs);
        
        /// <summary>
        /// Returns a bVector2 from component-wise application of IsInfinity (float.IsInfinity(v)).
        /// </summary>
        public static bVector2 IsInfinity(Vector2 v) => new bVector2(float.IsInfinity(v.x), float.IsInfinity(v.y));
        
        /// <summary>
        /// Returns a bVector from the application of IsInfinity (float.IsInfinity(v)).
        /// </summary>
        public static bVector2 IsInfinity(float v) => new bVector2(float.IsInfinity(v));
        
        /// <summary>
        /// Returns a bVector2 from component-wise application of IsFinite (!float.IsNaN(v) &amp;&amp; !float.IsInfinity(v)).
        /// </summary>
        public static bVector2 IsFinite(Vector2 v) => new bVector2(!float.IsNaN(v.x) && !float.IsInfinity(v.x), !float.IsNaN(v.y) && !float.IsInfinity(v.y));
        
        /// <summary>
        /// Returns a bVector from the application of IsFinite (!float.IsNaN(v) &amp;&amp; !float.IsInfinity(v)).
        /// </summary>
        public static bVector2 IsFinite(float v) => new bVector2(!float.IsNaN(v) && !float.IsInfinity(v));
        
        /// <summary>
        /// Returns a bVector2 from component-wise application of IsNaN (float.IsNaN(v)).
        /// </summary>
        public static bVector2 IsNaN(Vector2 v) => new bVector2(float.IsNaN(v.x), float.IsNaN(v.y));
        
        /// <summary>
        /// Returns a bVector from the application of IsNaN (float.IsNaN(v)).
        /// </summary>
        public static bVector2 IsNaN(float v) => new bVector2(float.IsNaN(v));
        
        /// <summary>
        /// Returns a bVector2 from component-wise application of IsNegativeInfinity (float.IsNegativeInfinity(v)).
        /// </summary>
        public static bVector2 IsNegativeInfinity(Vector2 v) => new bVector2(float.IsNegativeInfinity(v.x), float.IsNegativeInfinity(v.y));
        
        /// <summary>
        /// Returns a bVector from the application of IsNegativeInfinity (float.IsNegativeInfinity(v)).
        /// </summary>
        public static bVector2 IsNegativeInfinity(float v) => new bVector2(float.IsNegativeInfinity(v));
        
        /// <summary>
        /// Returns a bVector2 from component-wise application of IsPositiveInfinity (float.IsPositiveInfinity(v)).
        /// </summary>
        public static bVector2 IsPositiveInfinity(Vector2 v) => new bVector2(float.IsPositiveInfinity(v.x), float.IsPositiveInfinity(v.y));
        
        /// <summary>
        /// Returns a bVector from the application of IsPositiveInfinity (float.IsPositiveInfinity(v)).
        /// </summary>
        public static bVector2 IsPositiveInfinity(float v) => new bVector2(float.IsPositiveInfinity(v));
        
        /// <summary>
        /// Returns a Vector2 from component-wise application of Abs (Math.Abs(v)).
        /// </summary>
        public static Vector2 Abs(Vector2 v) => new Vector2(Math.Abs(v.x), Math.Abs(v.y));
        
        /// <summary>
        /// Returns a Vectortor from the application of Abs (Math.Abs(v)).
        /// </summary>
        public static Vector2 Abs(float v) => new Vector2(Math.Abs(v));
        
        /// <summary>
        /// Returns a Vector2 from component-wise application of HermiteInterpolationOrder3 ((3 - 2 * v) * v * v).
        /// </summary>
        public static Vector2 HermiteInterpolationOrder3(Vector2 v) => new Vector2((3 - 2 * v.x) * v.x * v.x, (3 - 2 * v.y) * v.y * v.y);
        
        /// <summary>
        /// Returns a Vectortor from the application of HermiteInterpolationOrder3 ((3 - 2 * v) * v * v).
        /// </summary>
        public static Vector2 HermiteInterpolationOrder3(float v) => new Vector2((3 - 2 * v) * v * v);
        
        /// <summary>
        /// Returns a Vector2 from component-wise application of HermiteInterpolationOrder5 (((6 * v - 15) * v + 10) * v * v * v).
        /// </summary>
        public static Vector2 HermiteInterpolationOrder5(Vector2 v) => new Vector2(((6 * v.x - 15) * v.x + 10) * v.x * v.x * v.x, ((6 * v.y - 15) * v.y + 10) * v.y * v.y * v.y);
        
        /// <summary>
        /// Returns a Vectortor from the application of HermiteInterpolationOrder5 (((6 * v - 15) * v + 10) * v * v * v).
        /// </summary>
        public static Vector2 HermiteInterpolationOrder5(float v) => new Vector2(((6 * v - 15) * v + 10) * v * v * v);
        
        /// <summary>
        /// Returns a Vector2 from component-wise application of Sqr (v * v).
        /// </summary>
        public static Vector2 Sqr(Vector2 v) => new Vector2(v.x * v.x, v.y * v.y);
        
        /// <summary>
        /// Returns a Vectortor from the application of Sqr (v * v).
        /// </summary>
        public static Vector2 Sqr(float v) => new Vector2(v * v);
        
        /// <summary>
        /// Returns a Vector2 from component-wise application of Pow2 (v * v).
        /// </summary>
        public static Vector2 Pow2(Vector2 v) => new Vector2(v.x * v.x, v.y * v.y);
        
        /// <summary>
        /// Returns a Vectortor from the application of Pow2 (v * v).
        /// </summary>
        public static Vector2 Pow2(float v) => new Vector2(v * v);
        
        /// <summary>
        /// Returns a Vector2 from component-wise application of Pow3 (v * v * v).
        /// </summary>
        public static Vector2 Pow3(Vector2 v) => new Vector2(v.x * v.x * v.x, v.y * v.y * v.y);
        
        /// <summary>
        /// Returns a Vectortor from the application of Pow3 (v * v * v).
        /// </summary>
        public static Vector2 Pow3(float v) => new Vector2(v * v * v);
        
        /// <summary>
        /// Returns a Vector2 from component-wise application of Step (v &gt;= 0f ? 1f : 0f).
        /// </summary>
        public static Vector2 Step(Vector2 v) => new Vector2(v.x >= 0f ? 1f : 0f, v.y >= 0f ? 1f : 0f);
        
        /// <summary>
        /// Returns a Vectortor from the application of Step (v &gt;= 0f ? 1f : 0f).
        /// </summary>
        public static Vector2 Step(float v) => new Vector2(v >= 0f ? 1f : 0f);
        
        /// <summary>
        /// Returns a Vector2 from component-wise application of Sqrt ((float)Math.Sqrt((double)v)).
        /// </summary>
        public static Vector2 Sqrt(Vector2 v) => new Vector2((float)Math.Sqrt((double)v.x), (float)Math.Sqrt((double)v.y));
        
        /// <summary>
        /// Returns a Vectortor from the application of Sqrt ((float)Math.Sqrt((double)v)).
        /// </summary>
        public static Vector2 Sqrt(float v) => new Vector2((float)Math.Sqrt((double)v));
        
        /// <summary>
        /// Returns a Vector2 from component-wise application of InverseSqrt ((float)(1.0 / Math.Sqrt((double)v))).
        /// </summary>
        public static Vector2 InverseSqrt(Vector2 v) => new Vector2((float)(1.0 / Math.Sqrt((double)v.x)), (float)(1.0 / Math.Sqrt((double)v.y)));
        
        /// <summary>
        /// Returns a Vectortor from the application of InverseSqrt ((float)(1.0 / Math.Sqrt((double)v))).
        /// </summary>
        public static Vector2 InverseSqrt(float v) => new Vector2((float)(1.0 / Math.Sqrt((double)v)));
        
        /// <summary>
        /// Returns a iVector2 from component-wise application of Sign (Math.Sign(v)).
        /// </summary>
        public static iVector2 Sign(Vector2 v) => new iVector2(Math.Sign(v.x), Math.Sign(v.y));
        
        /// <summary>
        /// Returns a iVector from the application of Sign (Math.Sign(v)).
        /// </summary>
        public static iVector2 Sign(float v) => new iVector2(Math.Sign(v));
        
        /// <summary>
        /// Returns a Vector2 from component-wise application of Max (Math.Max(lhs, rhs)).
        /// </summary>
        public static Vector2 Max(Vector2 lhs, Vector2 rhs) => new Vector2(Math.Max(lhs.x, rhs.x), Math.Max(lhs.y, rhs.y));
        
        /// <summary>
        /// Returns a Vector2 from component-wise application of Max (Math.Max(lhs, rhs)).
        /// </summary>
        public static Vector2 Max(Vector2 lhs, float rhs) => new Vector2(Math.Max(lhs.x, rhs), Math.Max(lhs.y, rhs));
        
        /// <summary>
        /// Returns a Vector2 from component-wise application of Max (Math.Max(lhs, rhs)).
        /// </summary>
        public static Vector2 Max(float lhs, Vector2 rhs) => new Vector2(Math.Max(lhs, rhs.x), Math.Max(lhs, rhs.y));
        
        /// <summary>
        /// Returns a Vectortor from the application of Max (Math.Max(lhs, rhs)).
        /// </summary>
        public static Vector2 Max(float lhs, float rhs) => new Vector2(Math.Max(lhs, rhs));
        
        /// <summary>
        /// Returns a Vector2 from component-wise application of Min (Math.Min(lhs, rhs)).
        /// </summary>
        public static Vector2 Min(Vector2 lhs, Vector2 rhs) => new Vector2(Math.Min(lhs.x, rhs.x), Math.Min(lhs.y, rhs.y));
        
        /// <summary>
        /// Returns a Vector2 from component-wise application of Min (Math.Min(lhs, rhs)).
        /// </summary>
        public static Vector2 Min(Vector2 lhs, float rhs) => new Vector2(Math.Min(lhs.x, rhs), Math.Min(lhs.y, rhs));
        
        /// <summary>
        /// Returns a Vector2 from component-wise application of Min (Math.Min(lhs, rhs)).
        /// </summary>
        public static Vector2 Min(float lhs, Vector2 rhs) => new Vector2(Math.Min(lhs, rhs.x), Math.Min(lhs, rhs.y));
        
        /// <summary>
        /// Returns a Vectortor from the application of Min (Math.Min(lhs, rhs)).
        /// </summary>
        public static Vector2 Min(float lhs, float rhs) => new Vector2(Math.Min(lhs, rhs));
        
        /// <summary>
        /// Returns a Vector2 from component-wise application of Pow ((float)Math.Pow((double)lhs, (double)rhs)).
        /// </summary>
        public static Vector2 Pow(Vector2 lhs, Vector2 rhs) => new Vector2((float)Math.Pow((double)lhs.x, (double)rhs.x), (float)Math.Pow((double)lhs.y, (double)rhs.y));
        
        /// <summary>
        /// Returns a Vector2 from component-wise application of Pow ((float)Math.Pow((double)lhs, (double)rhs)).
        /// </summary>
        public static Vector2 Pow(Vector2 lhs, float rhs) => new Vector2((float)Math.Pow((double)lhs.x, (double)rhs), (float)Math.Pow((double)lhs.y, (double)rhs));
        
        /// <summary>
        /// Returns a Vector2 from component-wise application of Pow ((float)Math.Pow((double)lhs, (double)rhs)).
        /// </summary>
        public static Vector2 Pow(float lhs, Vector2 rhs) => new Vector2((float)Math.Pow((double)lhs, (double)rhs.x), (float)Math.Pow((double)lhs, (double)rhs.y));
        
        /// <summary>
        /// Returns a Vectortor from the application of Pow ((float)Math.Pow((double)lhs, (double)rhs)).
        /// </summary>
        public static Vector2 Pow(float lhs, float rhs) => new Vector2((float)Math.Pow((double)lhs, (double)rhs));
        
        /// <summary>
        /// Returns a Vector2 from component-wise application of Log ((float)Math.Log((double)lhs, (double)rhs)).
        /// </summary>
        public static Vector2 Log(Vector2 lhs, Vector2 rhs) => new Vector2((float)Math.Log((double)lhs.x, (double)rhs.x), (float)Math.Log((double)lhs.y, (double)rhs.y));
        
        /// <summary>
        /// Returns a Vector2 from component-wise application of Log ((float)Math.Log((double)lhs, (double)rhs)).
        /// </summary>
        public static Vector2 Log(Vector2 lhs, float rhs) => new Vector2((float)Math.Log((double)lhs.x, (double)rhs), (float)Math.Log((double)lhs.y, (double)rhs));
        
        /// <summary>
        /// Returns a Vector2 from component-wise application of Log ((float)Math.Log((double)lhs, (double)rhs)).
        /// </summary>
        public static Vector2 Log(float lhs, Vector2 rhs) => new Vector2((float)Math.Log((double)lhs, (double)rhs.x), (float)Math.Log((double)lhs, (double)rhs.y));
        
        /// <summary>
        /// Returns a Vectortor from the application of Log ((float)Math.Log((double)lhs, (double)rhs)).
        /// </summary>
        public static Vector2 Log(float lhs, float rhs) => new Vector2((float)Math.Log((double)lhs, (double)rhs));
        
        /// <summary>
        /// Returns a Vector2 from component-wise application of Clamp (Math.Min(Math.Max(v, min), max)).
        /// </summary>
        public static Vector2 Clamp(Vector2 v, Vector2 min, Vector2 max) => new Vector2(Math.Min(Math.Max(v.x, min.x), max.x), Math.Min(Math.Max(v.y, min.y), max.y));
        
        /// <summary>
        /// Returns a Vector2 from component-wise application of Clamp (Math.Min(Math.Max(v, min), max)).
        /// </summary>
        public static Vector2 Clamp(Vector2 v, Vector2 min, float max) => new Vector2(Math.Min(Math.Max(v.x, min.x), max), Math.Min(Math.Max(v.y, min.y), max));
        
        /// <summary>
        /// Returns a Vector2 from component-wise application of Clamp (Math.Min(Math.Max(v, min), max)).
        /// </summary>
        public static Vector2 Clamp(Vector2 v, float min, Vector2 max) => new Vector2(Math.Min(Math.Max(v.x, min), max.x), Math.Min(Math.Max(v.y, min), max.y));
        
        /// <summary>
        /// Returns a Vector2 from component-wise application of Clamp (Math.Min(Math.Max(v, min), max)).
        /// </summary>
        public static Vector2 Clamp(Vector2 v, float min, float max) => new Vector2(Math.Min(Math.Max(v.x, min), max), Math.Min(Math.Max(v.y, min), max));
        
        /// <summary>
        /// Returns a Vector2 from component-wise application of Clamp (Math.Min(Math.Max(v, min), max)).
        /// </summary>
        public static Vector2 Clamp(float v, Vector2 min, Vector2 max) => new Vector2(Math.Min(Math.Max(v, min.x), max.x), Math.Min(Math.Max(v, min.y), max.y));
        
        /// <summary>
        /// Returns a Vector2 from component-wise application of Clamp (Math.Min(Math.Max(v, min), max)).
        /// </summary>
        public static Vector2 Clamp(float v, Vector2 min, float max) => new Vector2(Math.Min(Math.Max(v, min.x), max), Math.Min(Math.Max(v, min.y), max));
        
        /// <summary>
        /// Returns a Vector2 from component-wise application of Clamp (Math.Min(Math.Max(v, min), max)).
        /// </summary>
        public static Vector2 Clamp(float v, float min, Vector2 max) => new Vector2(Math.Min(Math.Max(v, min), max.x), Math.Min(Math.Max(v, min), max.y));
        
        /// <summary>
        /// Returns a Vectortor from the application of Clamp (Math.Min(Math.Max(v, min), max)).
        /// </summary>
        public static Vector2 Clamp(float v, float min, float max) => new Vector2(Math.Min(Math.Max(v, min), max));
        
        /// <summary>
        /// Returns a Vector2 from component-wise application of Mix (min * (1-a) + max * a).
        /// </summary>
        public static Vector2 Mix(Vector2 min, Vector2 max, Vector2 a) => new Vector2(min.x * (1-a.x) + max.x * a.x, min.y * (1-a.y) + max.y * a.y);
        
        /// <summary>
        /// Returns a Vector2 from component-wise application of Mix (min * (1-a) + max * a).
        /// </summary>
        public static Vector2 Mix(Vector2 min, Vector2 max, float a) => new Vector2(min.x * (1-a) + max.x * a, min.y * (1-a) + max.y * a);
        
        /// <summary>
        /// Returns a Vector2 from component-wise application of Mix (min * (1-a) + max * a).
        /// </summary>
        public static Vector2 Mix(Vector2 min, float max, Vector2 a) => new Vector2(min.x * (1-a.x) + max * a.x, min.y * (1-a.y) + max * a.y);
        
        /// <summary>
        /// Returns a Vector2 from component-wise application of Mix (min * (1-a) + max * a).
        /// </summary>
        public static Vector2 Mix(Vector2 min, float max, float a) => new Vector2(min.x * (1-a) + max * a, min.y * (1-a) + max * a);
        
        /// <summary>
        /// Returns a Vector2 from component-wise application of Mix (min * (1-a) + max * a).
        /// </summary>
        public static Vector2 Mix(float min, Vector2 max, Vector2 a) => new Vector2(min * (1-a.x) + max.x * a.x, min * (1-a.y) + max.y * a.y);
        
        /// <summary>
        /// Returns a Vector2 from component-wise application of Mix (min * (1-a) + max * a).
        /// </summary>
        public static Vector2 Mix(float min, Vector2 max, float a) => new Vector2(min * (1-a) + max.x * a, min * (1-a) + max.y * a);
        
        /// <summary>
        /// Returns a Vector2 from component-wise application of Mix (min * (1-a) + max * a).
        /// </summary>
        public static Vector2 Mix(float min, float max, Vector2 a) => new Vector2(min * (1-a.x) + max * a.x, min * (1-a.y) + max * a.y);
        
        /// <summary>
        /// Returns a Vectortor from the application of Mix (min * (1-a) + max * a).
        /// </summary>
        public static Vector2 Mix(float min, float max, float a) => new Vector2(min * (1-a) + max * a);
        
        /// <summary>
        /// Returns a Vector2 from component-wise application of Lerp (min * (1-a) + max * a).
        /// </summary>
        public static Vector2 Lerp(Vector2 min, Vector2 max, Vector2 a) => new Vector2(min.x * (1-a.x) + max.x * a.x, min.y * (1-a.y) + max.y * a.y);
        
        /// <summary>
        /// Returns a Vector2 from component-wise application of Lerp (min * (1-a) + max * a).
        /// </summary>
        public static Vector2 Lerp(Vector2 min, Vector2 max, float a) => new Vector2(min.x * (1-a) + max.x * a, min.y * (1-a) + max.y * a);
        
        /// <summary>
        /// Returns a Vector2 from component-wise application of Lerp (min * (1-a) + max * a).
        /// </summary>
        public static Vector2 Lerp(Vector2 min, float max, Vector2 a) => new Vector2(min.x * (1-a.x) + max * a.x, min.y * (1-a.y) + max * a.y);
        
        /// <summary>
        /// Returns a Vector2 from component-wise application of Lerp (min * (1-a) + max * a).
        /// </summary>
        public static Vector2 Lerp(Vector2 min, float max, float a) => new Vector2(min.x * (1-a) + max * a, min.y * (1-a) + max * a);
        
        /// <summary>
        /// Returns a Vector2 from component-wise application of Lerp (min * (1-a) + max * a).
        /// </summary>
        public static Vector2 Lerp(float min, Vector2 max, Vector2 a) => new Vector2(min * (1-a.x) + max.x * a.x, min * (1-a.y) + max.y * a.y);
        
        /// <summary>
        /// Returns a Vector2 from component-wise application of Lerp (min * (1-a) + max * a).
        /// </summary>
        public static Vector2 Lerp(float min, Vector2 max, float a) => new Vector2(min * (1-a) + max.x * a, min * (1-a) + max.y * a);
        
        /// <summary>
        /// Returns a Vector2 from component-wise application of Lerp (min * (1-a) + max * a).
        /// </summary>
        public static Vector2 Lerp(float min, float max, Vector2 a) => new Vector2(min * (1-a.x) + max * a.x, min * (1-a.y) + max * a.y);
        
        /// <summary>
        /// Returns a Vectortor from the application of Lerp (min * (1-a) + max * a).
        /// </summary>
        public static Vector2 Lerp(float min, float max, float a) => new Vector2(min * (1-a) + max * a);
        
        /// <summary>
        /// Returns a Vector2 from component-wise application of Smoothstep (((v - edge0) / (edge1 - edge0)).Clamp().HermiteInterpolationOrder3()).
        /// </summary>
        public static Vector2 Smoothstep(Vector2 edge0, Vector2 edge1, Vector2 v) => new Vector2(((v.x - edge0.x) / (edge1.x - edge0.x)).Clamp().HermiteInterpolationOrder3(), ((v.y - edge0.y) / (edge1.y - edge0.y)).Clamp().HermiteInterpolationOrder3());
        
        /// <summary>
        /// Returns a Vector2 from component-wise application of Smoothstep (((v - edge0) / (edge1 - edge0)).Clamp().HermiteInterpolationOrder3()).
        /// </summary>
        public static Vector2 Smoothstep(Vector2 edge0, Vector2 edge1, float v) => new Vector2(((v - edge0.x) / (edge1.x - edge0.x)).Clamp().HermiteInterpolationOrder3(), ((v - edge0.y) / (edge1.y - edge0.y)).Clamp().HermiteInterpolationOrder3());
        
        /// <summary>
        /// Returns a Vector2 from component-wise application of Smoothstep (((v - edge0) / (edge1 - edge0)).Clamp().HermiteInterpolationOrder3()).
        /// </summary>
        public static Vector2 Smoothstep(Vector2 edge0, float edge1, Vector2 v) => new Vector2(((v.x - edge0.x) / (edge1 - edge0.x)).Clamp().HermiteInterpolationOrder3(), ((v.y - edge0.y) / (edge1 - edge0.y)).Clamp().HermiteInterpolationOrder3());
        
        /// <summary>
        /// Returns a Vector2 from component-wise application of Smoothstep (((v - edge0) / (edge1 - edge0)).Clamp().HermiteInterpolationOrder3()).
        /// </summary>
        public static Vector2 Smoothstep(Vector2 edge0, float edge1, float v) => new Vector2(((v - edge0.x) / (edge1 - edge0.x)).Clamp().HermiteInterpolationOrder3(), ((v - edge0.y) / (edge1 - edge0.y)).Clamp().HermiteInterpolationOrder3());
        
        /// <summary>
        /// Returns a Vector2 from component-wise application of Smoothstep (((v - edge0) / (edge1 - edge0)).Clamp().HermiteInterpolationOrder3()).
        /// </summary>
        public static Vector2 Smoothstep(float edge0, Vector2 edge1, Vector2 v) => new Vector2(((v.x - edge0) / (edge1.x - edge0)).Clamp().HermiteInterpolationOrder3(), ((v.y - edge0) / (edge1.y - edge0)).Clamp().HermiteInterpolationOrder3());
        
        /// <summary>
        /// Returns a Vector2 from component-wise application of Smoothstep (((v - edge0) / (edge1 - edge0)).Clamp().HermiteInterpolationOrder3()).
        /// </summary>
        public static Vector2 Smoothstep(float edge0, Vector2 edge1, float v) => new Vector2(((v - edge0) / (edge1.x - edge0)).Clamp().HermiteInterpolationOrder3(), ((v - edge0) / (edge1.y - edge0)).Clamp().HermiteInterpolationOrder3());
        
        /// <summary>
        /// Returns a Vector2 from component-wise application of Smoothstep (((v - edge0) / (edge1 - edge0)).Clamp().HermiteInterpolationOrder3()).
        /// </summary>
        public static Vector2 Smoothstep(float edge0, float edge1, Vector2 v) => new Vector2(((v.x - edge0) / (edge1 - edge0)).Clamp().HermiteInterpolationOrder3(), ((v.y - edge0) / (edge1 - edge0)).Clamp().HermiteInterpolationOrder3());
        
        /// <summary>
        /// Returns a Vectortor from the application of Smoothstep (((v - edge0) / (edge1 - edge0)).Clamp().HermiteInterpolationOrder3()).
        /// </summary>
        public static Vector2 Smoothstep(float edge0, float edge1, float v) => new Vector2(((v - edge0) / (edge1 - edge0)).Clamp().HermiteInterpolationOrder3());
        
        /// <summary>
        /// Returns a Vector2 from component-wise application of Smootherstep (((v - edge0) / (edge1 - edge0)).Clamp().HermiteInterpolationOrder5()).
        /// </summary>
        public static Vector2 Smootherstep(Vector2 edge0, Vector2 edge1, Vector2 v) => new Vector2(((v.x - edge0.x) / (edge1.x - edge0.x)).Clamp().HermiteInterpolationOrder5(), ((v.y - edge0.y) / (edge1.y - edge0.y)).Clamp().HermiteInterpolationOrder5());
        
        /// <summary>
        /// Returns a Vector2 from component-wise application of Smootherstep (((v - edge0) / (edge1 - edge0)).Clamp().HermiteInterpolationOrder5()).
        /// </summary>
        public static Vector2 Smootherstep(Vector2 edge0, Vector2 edge1, float v) => new Vector2(((v - edge0.x) / (edge1.x - edge0.x)).Clamp().HermiteInterpolationOrder5(), ((v - edge0.y) / (edge1.y - edge0.y)).Clamp().HermiteInterpolationOrder5());
        
        /// <summary>
        /// Returns a Vector2 from component-wise application of Smootherstep (((v - edge0) / (edge1 - edge0)).Clamp().HermiteInterpolationOrder5()).
        /// </summary>
        public static Vector2 Smootherstep(Vector2 edge0, float edge1, Vector2 v) => new Vector2(((v.x - edge0.x) / (edge1 - edge0.x)).Clamp().HermiteInterpolationOrder5(), ((v.y - edge0.y) / (edge1 - edge0.y)).Clamp().HermiteInterpolationOrder5());
        
        /// <summary>
        /// Returns a Vector2 from component-wise application of Smootherstep (((v - edge0) / (edge1 - edge0)).Clamp().HermiteInterpolationOrder5()).
        /// </summary>
        public static Vector2 Smootherstep(Vector2 edge0, float edge1, float v) => new Vector2(((v - edge0.x) / (edge1 - edge0.x)).Clamp().HermiteInterpolationOrder5(), ((v - edge0.y) / (edge1 - edge0.y)).Clamp().HermiteInterpolationOrder5());
        
        /// <summary>
        /// Returns a Vector2 from component-wise application of Smootherstep (((v - edge0) / (edge1 - edge0)).Clamp().HermiteInterpolationOrder5()).
        /// </summary>
        public static Vector2 Smootherstep(float edge0, Vector2 edge1, Vector2 v) => new Vector2(((v.x - edge0) / (edge1.x - edge0)).Clamp().HermiteInterpolationOrder5(), ((v.y - edge0) / (edge1.y - edge0)).Clamp().HermiteInterpolationOrder5());
        
        /// <summary>
        /// Returns a Vector2 from component-wise application of Smootherstep (((v - edge0) / (edge1 - edge0)).Clamp().HermiteInterpolationOrder5()).
        /// </summary>
        public static Vector2 Smootherstep(float edge0, Vector2 edge1, float v) => new Vector2(((v - edge0) / (edge1.x - edge0)).Clamp().HermiteInterpolationOrder5(), ((v - edge0) / (edge1.y - edge0)).Clamp().HermiteInterpolationOrder5());
        
        /// <summary>
        /// Returns a Vector2 from component-wise application of Smootherstep (((v - edge0) / (edge1 - edge0)).Clamp().HermiteInterpolationOrder5()).
        /// </summary>
        public static Vector2 Smootherstep(float edge0, float edge1, Vector2 v) => new Vector2(((v.x - edge0) / (edge1 - edge0)).Clamp().HermiteInterpolationOrder5(), ((v.y - edge0) / (edge1 - edge0)).Clamp().HermiteInterpolationOrder5());
        
        /// <summary>
        /// Returns a Vectortor from the application of Smootherstep (((v - edge0) / (edge1 - edge0)).Clamp().HermiteInterpolationOrder5()).
        /// </summary>
        public static Vector2 Smootherstep(float edge0, float edge1, float v) => new Vector2(((v - edge0) / (edge1 - edge0)).Clamp().HermiteInterpolationOrder5());
        
        /// <summary>
        /// Returns a Vector2 from component-wise application of Fma (a * b + c).
        /// </summary>
        public static Vector2 Fma(Vector2 a, Vector2 b, Vector2 c) => new Vector2(a.x * b.x + c.x, a.y * b.y + c.y);
        
        /// <summary>
        /// Returns a Vector2 from component-wise application of Fma (a * b + c).
        /// </summary>
        public static Vector2 Fma(Vector2 a, Vector2 b, float c) => new Vector2(a.x * b.x + c, a.y * b.y + c);
        
        /// <summary>
        /// Returns a Vector2 from component-wise application of Fma (a * b + c).
        /// </summary>
        public static Vector2 Fma(Vector2 a, float b, Vector2 c) => new Vector2(a.x * b + c.x, a.y * b + c.y);
        
        /// <summary>
        /// Returns a Vector2 from component-wise application of Fma (a * b + c).
        /// </summary>
        public static Vector2 Fma(Vector2 a, float b, float c) => new Vector2(a.x * b + c, a.y * b + c);
        
        /// <summary>
        /// Returns a Vector2 from component-wise application of Fma (a * b + c).
        /// </summary>
        public static Vector2 Fma(float a, Vector2 b, Vector2 c) => new Vector2(a * b.x + c.x, a * b.y + c.y);
        
        /// <summary>
        /// Returns a Vector2 from component-wise application of Fma (a * b + c).
        /// </summary>
        public static Vector2 Fma(float a, Vector2 b, float c) => new Vector2(a * b.x + c, a * b.y + c);
        
        /// <summary>
        /// Returns a Vector2 from component-wise application of Fma (a * b + c).
        /// </summary>
        public static Vector2 Fma(float a, float b, Vector2 c) => new Vector2(a * b + c.x, a * b + c.y);
        
        /// <summary>
        /// Returns a Vectortor from the application of Fma (a * b + c).
        /// </summary>
        public static Vector2 Fma(float a, float b, float c) => new Vector2(a * b + c);
        
        /// <summary>
        /// Returns a Vector2 from component-wise application of Add (lhs + rhs).
        /// </summary>
        public static Vector2 Add(Vector2 lhs, Vector2 rhs) => new Vector2(lhs.x + rhs.x, lhs.y + rhs.y);
        
        /// <summary>
        /// Returns a Vector2 from component-wise application of Add (lhs + rhs).
        /// </summary>
        public static Vector2 Add(Vector2 lhs, float rhs) => new Vector2(lhs.x + rhs, lhs.y + rhs);
        
        /// <summary>
        /// Returns a Vector2 from component-wise application of Add (lhs + rhs).
        /// </summary>
        public static Vector2 Add(float lhs, Vector2 rhs) => new Vector2(lhs + rhs.x, lhs + rhs.y);
        
        /// <summary>
        /// Returns a Vectortor from the application of Add (lhs + rhs).
        /// </summary>
        public static Vector2 Add(float lhs, float rhs) => new Vector2(lhs + rhs);
        
        /// <summary>
        /// Returns a Vector2 from component-wise application of Sub (lhs - rhs).
        /// </summary>
        public static Vector2 Sub(Vector2 lhs, Vector2 rhs) => new Vector2(lhs.x - rhs.x, lhs.y - rhs.y);
        
        /// <summary>
        /// Returns a Vector2 from component-wise application of Sub (lhs - rhs).
        /// </summary>
        public static Vector2 Sub(Vector2 lhs, float rhs) => new Vector2(lhs.x - rhs, lhs.y - rhs);
        
        /// <summary>
        /// Returns a Vector2 from component-wise application of Sub (lhs - rhs).
        /// </summary>
        public static Vector2 Sub(float lhs, Vector2 rhs) => new Vector2(lhs - rhs.x, lhs - rhs.y);
        
        /// <summary>
        /// Returns a Vectortor from the application of Sub (lhs - rhs).
        /// </summary>
        public static Vector2 Sub(float lhs, float rhs) => new Vector2(lhs - rhs);
        
        /// <summary>
        /// Returns a Vector2 from component-wise application of Mul (lhs * rhs).
        /// </summary>
        public static Vector2 Mul(Vector2 lhs, Vector2 rhs) => new Vector2(lhs.x * rhs.x, lhs.y * rhs.y);
        
        /// <summary>
        /// Returns a Vector2 from component-wise application of Mul (lhs * rhs).
        /// </summary>
        public static Vector2 Mul(Vector2 lhs, float rhs) => new Vector2(lhs.x * rhs, lhs.y * rhs);
        
        /// <summary>
        /// Returns a Vector2 from component-wise application of Mul (lhs * rhs).
        /// </summary>
        public static Vector2 Mul(float lhs, Vector2 rhs) => new Vector2(lhs * rhs.x, lhs * rhs.y);
        
        /// <summary>
        /// Returns a Vectortor from the application of Mul (lhs * rhs).
        /// </summary>
        public static Vector2 Mul(float lhs, float rhs) => new Vector2(lhs * rhs);
        
        /// <summary>
        /// Returns a Vector2 from component-wise application of Div (lhs / rhs).
        /// </summary>
        public static Vector2 Div(Vector2 lhs, Vector2 rhs) => new Vector2(lhs.x / rhs.x, lhs.y / rhs.y);
        
        /// <summary>
        /// Returns a Vector2 from component-wise application of Div (lhs / rhs).
        /// </summary>
        public static Vector2 Div(Vector2 lhs, float rhs) => new Vector2(lhs.x / rhs, lhs.y / rhs);
        
        /// <summary>
        /// Returns a Vector2 from component-wise application of Div (lhs / rhs).
        /// </summary>
        public static Vector2 Div(float lhs, Vector2 rhs) => new Vector2(lhs / rhs.x, lhs / rhs.y);
        
        /// <summary>
        /// Returns a Vectortor from the application of Div (lhs / rhs).
        /// </summary>
        public static Vector2 Div(float lhs, float rhs) => new Vector2(lhs / rhs);
        
        /// <summary>
        /// Returns a Vector2 from component-wise application of Modulo (lhs % rhs).
        /// </summary>
        public static Vector2 Modulo(Vector2 lhs, Vector2 rhs) => new Vector2(lhs.x % rhs.x, lhs.y % rhs.y);
        
        /// <summary>
        /// Returns a Vector2 from component-wise application of Modulo (lhs % rhs).
        /// </summary>
        public static Vector2 Modulo(Vector2 lhs, float rhs) => new Vector2(lhs.x % rhs, lhs.y % rhs);
        
        /// <summary>
        /// Returns a Vector2 from component-wise application of Modulo (lhs % rhs).
        /// </summary>
        public static Vector2 Modulo(float lhs, Vector2 rhs) => new Vector2(lhs % rhs.x, lhs % rhs.y);
        
        /// <summary>
        /// Returns a Vectortor from the application of Modulo (lhs % rhs).
        /// </summary>
        public static Vector2 Modulo(float lhs, float rhs) => new Vector2(lhs % rhs);
        
        /// <summary>
        /// Returns a Vector2 from component-wise application of Degrees (Radians-To-Degrees Conversion).
        /// </summary>
        public static Vector2 Degrees(Vector2 v) => new Vector2((float)(v.x * 57.295779513082320876798154814105170332405472466564321f), (float)(v.y * 57.295779513082320876798154814105170332405472466564321f));
        
        /// <summary>
        /// Returns a Vectortor from the application of Degrees (Radians-To-Degrees Conversion).
        /// </summary>
        public static Vector2 Degrees(float v) => new Vector2((float)(v * 57.295779513082320876798154814105170332405472466564321f));
        
        /// <summary>
        /// Returns a Vector2 from component-wise application of Radians (Degrees-To-Radians Conversion).
        /// </summary>
        public static Vector2 Radians(Vector2 v) => new Vector2((float)(v.x * 0.0174532925199432957692369076848861271344287188854172f), (float)(v.y * 0.0174532925199432957692369076848861271344287188854172f));
        
        /// <summary>
        /// Returns a Vectortor from the application of Radians (Degrees-To-Radians Conversion).
        /// </summary>
        public static Vector2 Radians(float v) => new Vector2((float)(v * 0.0174532925199432957692369076848861271344287188854172f));
        
        /// <summary>
        /// Returns a Vector2 from component-wise application of Acos ((float)Math.Acos((double)v)).
        /// </summary>
        public static Vector2 Acos(Vector2 v) => new Vector2((float)Math.Acos((double)v.x), (float)Math.Acos((double)v.y));
        
        /// <summary>
        /// Returns a Vectortor from the application of Acos ((float)Math.Acos((double)v)).
        /// </summary>
        public static Vector2 Acos(float v) => new Vector2((float)Math.Acos((double)v));
        
        /// <summary>
        /// Returns a Vector2 from component-wise application of Asin ((float)Math.Asin((double)v)).
        /// </summary>
        public static Vector2 Asin(Vector2 v) => new Vector2((float)Math.Asin((double)v.x), (float)Math.Asin((double)v.y));
        
        /// <summary>
        /// Returns a Vectortor from the application of Asin ((float)Math.Asin((double)v)).
        /// </summary>
        public static Vector2 Asin(float v) => new Vector2((float)Math.Asin((double)v));
        
        /// <summary>
        /// Returns a Vector2 from component-wise application of Atan ((float)Math.Atan((double)v)).
        /// </summary>
        public static Vector2 Atan(Vector2 v) => new Vector2((float)Math.Atan((double)v.x), (float)Math.Atan((double)v.y));
        
        /// <summary>
        /// Returns a Vectortor from the application of Atan ((float)Math.Atan((double)v)).
        /// </summary>
        public static Vector2 Atan(float v) => new Vector2((float)Math.Atan((double)v));
        
        /// <summary>
        /// Returns a Vector2 from component-wise application of Cos ((float)Math.Cos((double)v)).
        /// </summary>
        public static Vector2 Cos(Vector2 v) => new Vector2((float)Math.Cos((double)v.x), (float)Math.Cos((double)v.y));
        
        /// <summary>
        /// Returns a Vectortor from the application of Cos ((float)Math.Cos((double)v)).
        /// </summary>
        public static Vector2 Cos(float v) => new Vector2((float)Math.Cos((double)v));
        
        /// <summary>
        /// Returns a Vector2 from component-wise application of Cosh ((float)Math.Cosh((double)v)).
        /// </summary>
        public static Vector2 Cosh(Vector2 v) => new Vector2((float)Math.Cosh((double)v.x), (float)Math.Cosh((double)v.y));
        
        /// <summary>
        /// Returns a Vectortor from the application of Cosh ((float)Math.Cosh((double)v)).
        /// </summary>
        public static Vector2 Cosh(float v) => new Vector2((float)Math.Cosh((double)v));
        
        /// <summary>
        /// Returns a Vector2 from component-wise application of Exp ((float)Math.Exp((double)v)).
        /// </summary>
        public static Vector2 Exp(Vector2 v) => new Vector2((float)Math.Exp((double)v.x), (float)Math.Exp((double)v.y));
        
        /// <summary>
        /// Returns a Vectortor from the application of Exp ((float)Math.Exp((double)v)).
        /// </summary>
        public static Vector2 Exp(float v) => new Vector2((float)Math.Exp((double)v));
        
        /// <summary>
        /// Returns a Vector2 from component-wise application of Log ((float)Math.Log((double)v)).
        /// </summary>
        public static Vector2 Log(Vector2 v) => new Vector2((float)Math.Log((double)v.x), (float)Math.Log((double)v.y));
        
        /// <summary>
        /// Returns a Vectortor from the application of Log ((float)Math.Log((double)v)).
        /// </summary>
        public static Vector2 Log(float v) => new Vector2((float)Math.Log((double)v));
        
        /// <summary>
        /// Returns a Vector2 from component-wise application of Log2 ((float)Math.Log((double)v, 2)).
        /// </summary>
        public static Vector2 Log2(Vector2 v) => new Vector2((float)Math.Log((double)v.x, 2), (float)Math.Log((double)v.y, 2));
        
        /// <summary>
        /// Returns a Vectortor from the application of Log2 ((float)Math.Log((double)v, 2)).
        /// </summary>
        public static Vector2 Log2(float v) => new Vector2((float)Math.Log((double)v, 2));
        
        /// <summary>
        /// Returns a Vector2 from component-wise application of Log10 ((float)Math.Log10((double)v)).
        /// </summary>
        public static Vector2 Log10(Vector2 v) => new Vector2((float)Math.Log10((double)v.x), (float)Math.Log10((double)v.y));
        
        /// <summary>
        /// Returns a Vectortor from the application of Log10 ((float)Math.Log10((double)v)).
        /// </summary>
        public static Vector2 Log10(float v) => new Vector2((float)Math.Log10((double)v));
        
        /// <summary>
        /// Returns a Vector2 from component-wise application of Floor ((float)Math.Floor(v)).
        /// </summary>
        public static Vector2 Floor(Vector2 v) => new Vector2((float)Math.Floor(v.x), (float)Math.Floor(v.y));
        
        /// <summary>
        /// Returns a Vectortor from the application of Floor ((float)Math.Floor(v)).
        /// </summary>
        public static Vector2 Floor(float v) => new Vector2((float)Math.Floor(v));
        
        /// <summary>
        /// Returns a Vector2 from component-wise application of Ceiling ((float)Math.Ceiling(v)).
        /// </summary>
        public static Vector2 Ceiling(Vector2 v) => new Vector2((float)Math.Ceiling(v.x), (float)Math.Ceiling(v.y));
        
        /// <summary>
        /// Returns a Vectortor from the application of Ceiling ((float)Math.Ceiling(v)).
        /// </summary>
        public static Vector2 Ceiling(float v) => new Vector2((float)Math.Ceiling(v));
        
        /// <summary>
        /// Returns a Vector2 from component-wise application of Round ((float)Math.Round(v)).
        /// </summary>
        public static Vector2 Round(Vector2 v) => new Vector2((float)Math.Round(v.x), (float)Math.Round(v.y));
        
        /// <summary>
        /// Returns a Vectortor from the application of Round ((float)Math.Round(v)).
        /// </summary>
        public static Vector2 Round(float v) => new Vector2((float)Math.Round(v));
        
        /// <summary>
        /// Returns a Vector2 from component-wise application of Sin ((float)Math.Sin((double)v)).
        /// </summary>
        public static Vector2 Sin(Vector2 v) => new Vector2((float)Math.Sin((double)v.x), (float)Math.Sin((double)v.y));
        
        /// <summary>
        /// Returns a Vectortor from the application of Sin ((float)Math.Sin((double)v)).
        /// </summary>
        public static Vector2 Sin(float v) => new Vector2((float)Math.Sin((double)v));
        
        /// <summary>
        /// Returns a Vector2 from component-wise application of Sinh ((float)Math.Sinh((double)v)).
        /// </summary>
        public static Vector2 Sinh(Vector2 v) => new Vector2((float)Math.Sinh((double)v.x), (float)Math.Sinh((double)v.y));
        
        /// <summary>
        /// Returns a Vectortor from the application of Sinh ((float)Math.Sinh((double)v)).
        /// </summary>
        public static Vector2 Sinh(float v) => new Vector2((float)Math.Sinh((double)v));
        
        /// <summary>
        /// Returns a Vector2 from component-wise application of Tan ((float)Math.Tan((double)v)).
        /// </summary>
        public static Vector2 Tan(Vector2 v) => new Vector2((float)Math.Tan((double)v.x), (float)Math.Tan((double)v.y));
        
        /// <summary>
        /// Returns a Vectortor from the application of Tan ((float)Math.Tan((double)v)).
        /// </summary>
        public static Vector2 Tan(float v) => new Vector2((float)Math.Tan((double)v));
        
        /// <summary>
        /// Returns a Vector2 from component-wise application of Tanh ((float)Math.Tanh((double)v)).
        /// </summary>
        public static Vector2 Tanh(Vector2 v) => new Vector2((float)Math.Tanh((double)v.x), (float)Math.Tanh((double)v.y));
        
        /// <summary>
        /// Returns a Vectortor from the application of Tanh ((float)Math.Tanh((double)v)).
        /// </summary>
        public static Vector2 Tanh(float v) => new Vector2((float)Math.Tanh((double)v));
        
        /// <summary>
        /// Returns a Vector2 from component-wise application of Truncate ((float)Math.Truncate((double)v)).
        /// </summary>
        public static Vector2 Truncate(Vector2 v) => new Vector2((float)Math.Truncate((double)v.x), (float)Math.Truncate((double)v.y));
        
        /// <summary>
        /// Returns a Vectortor from the application of Truncate ((float)Math.Truncate((double)v)).
        /// </summary>
        public static Vector2 Truncate(float v) => new Vector2((float)Math.Truncate((double)v));
        
        /// <summary>
        /// Returns a Vector2 from component-wise application of Fract ((float)(v - Math.Floor(v))).
        /// </summary>
        public static Vector2 Fract(Vector2 v) => new Vector2((float)(v.x - Math.Floor(v.x)), (float)(v.y - Math.Floor(v.y)));
        
        /// <summary>
        /// Returns a Vectortor from the application of Fract ((float)(v - Math.Floor(v))).
        /// </summary>
        public static Vector2 Fract(float v) => new Vector2((float)(v - Math.Floor(v)));
        
        /// <summary>
        /// Returns a Vector2 from component-wise application of Trunc ((long)(v)).
        /// </summary>
        public static Vector2 Trunc(Vector2 v) => new Vector2((long)(v.x), (long)(v.y));
        
        /// <summary>
        /// Returns a Vectortor from the application of Trunc ((long)(v)).
        /// </summary>
        public static Vector2 Trunc(float v) => new Vector2((long)(v));
        
        /// <summary>
        /// Returns a Vector2 with independent and identically distributed uniform values between 'minValue' and 'maxValue'.
        /// </summary>
        public static Vector2 Random(System.Random random, Vector2 minValue, Vector2 maxValue) => new Vector2((float)random.NextDouble() * (maxValue.x - minValue.x) + minValue.x, (float)random.NextDouble() * (maxValue.y - minValue.y) + minValue.y);
        
        /// <summary>
        /// Returns a Vector2 with independent and identically distributed uniform values between 'minValue' and 'maxValue'.
        /// </summary>
        public static Vector2 Random(System.Random random, Vector2 minValue, float maxValue) => new Vector2((float)random.NextDouble() * (maxValue - minValue.x) + minValue.x, (float)random.NextDouble() * (maxValue - minValue.y) + minValue.y);
        
        /// <summary>
        /// Returns a Vector2 with independent and identically distributed uniform values between 'minValue' and 'maxValue'.
        /// </summary>
        public static Vector2 Random(System.Random random, float minValue, Vector2 maxValue) => new Vector2((float)random.NextDouble() * (maxValue.x - minValue) + minValue, (float)random.NextDouble() * (maxValue.y - minValue) + minValue);
        
        /// <summary>
        /// Returns a Vector2 with independent and identically distributed uniform values between 'minValue' and 'maxValue'.
        /// </summary>
        public static Vector2 Random(System.Random random, float minValue, float maxValue) => new Vector2((float)random.NextDouble() * (maxValue - minValue) + minValue);
        
        /// <summary>
        /// Returns a Vector2 with independent and identically distributed uniform values between 'minValue' and 'maxValue'.
        /// </summary>
        public static Vector2 RandomUniform(System.Random random, Vector2 minValue, Vector2 maxValue) => new Vector2((float)random.NextDouble() * (maxValue.x - minValue.x) + minValue.x, (float)random.NextDouble() * (maxValue.y - minValue.y) + minValue.y);
        
        /// <summary>
        /// Returns a Vector2 with independent and identically distributed uniform values between 'minValue' and 'maxValue'.
        /// </summary>
        public static Vector2 RandomUniform(System.Random random, Vector2 minValue, float maxValue) => new Vector2((float)random.NextDouble() * (maxValue - minValue.x) + minValue.x, (float)random.NextDouble() * (maxValue - minValue.y) + minValue.y);
        
        /// <summary>
        /// Returns a Vector2 with independent and identically distributed uniform values between 'minValue' and 'maxValue'.
        /// </summary>
        public static Vector2 RandomUniform(System.Random random, float minValue, Vector2 maxValue) => new Vector2((float)random.NextDouble() * (maxValue.x - minValue) + minValue, (float)random.NextDouble() * (maxValue.y - minValue) + minValue);
        
        /// <summary>
        /// Returns a Vector2 with independent and identically distributed uniform values between 'minValue' and 'maxValue'.
        /// </summary>
        public static Vector2 RandomUniform(System.Random random, float minValue, float maxValue) => new Vector2((float)random.NextDouble() * (maxValue - minValue) + minValue);
        
        /// <summary>
        /// Returns a Vector2 with independent and identically distributed values according to a normal/Gaussian distribution with specified mean and variance.
        /// </summary>
        public static Vector2 RandomNormal(System.Random random, Vector2 mean, Vector2 variance) => new Vector2((float)(Math.Sqrt((double)variance.x) * Math.Cos(2 * Math.PI * random.NextDouble()) * Math.Sqrt(-2.0 * Math.Log(random.NextDouble()))) + mean.x, (float)(Math.Sqrt((double)variance.y) * Math.Cos(2 * Math.PI * random.NextDouble()) * Math.Sqrt(-2.0 * Math.Log(random.NextDouble()))) + mean.y);
        
        /// <summary>
        /// Returns a Vector2 with independent and identically distributed values according to a normal/Gaussian distribution with specified mean and variance.
        /// </summary>
        public static Vector2 RandomNormal(System.Random random, Vector2 mean, float variance) => new Vector2((float)(Math.Sqrt((double)variance) * Math.Cos(2 * Math.PI * random.NextDouble()) * Math.Sqrt(-2.0 * Math.Log(random.NextDouble()))) + mean.x, (float)(Math.Sqrt((double)variance) * Math.Cos(2 * Math.PI * random.NextDouble()) * Math.Sqrt(-2.0 * Math.Log(random.NextDouble()))) + mean.y);
        
        /// <summary>
        /// Returns a Vector2 with independent and identically distributed values according to a normal/Gaussian distribution with specified mean and variance.
        /// </summary>
        public static Vector2 RandomNormal(System.Random random, float mean, Vector2 variance) => new Vector2((float)(Math.Sqrt((double)variance.x) * Math.Cos(2 * Math.PI * random.NextDouble()) * Math.Sqrt(-2.0 * Math.Log(random.NextDouble()))) + mean, (float)(Math.Sqrt((double)variance.y) * Math.Cos(2 * Math.PI * random.NextDouble()) * Math.Sqrt(-2.0 * Math.Log(random.NextDouble()))) + mean);
        
        /// <summary>
        /// Returns a Vector2 with independent and identically distributed values according to a normal/Gaussian distribution with specified mean and variance.
        /// </summary>
        public static Vector2 RandomNormal(System.Random random, float mean, float variance) => new Vector2((float)(Math.Sqrt((double)variance) * Math.Cos(2 * Math.PI * random.NextDouble()) * Math.Sqrt(-2.0 * Math.Log(random.NextDouble()))) + mean);
        
        /// <summary>
        /// Returns a Vector2 with independent and identically distributed values according to a normal/Gaussian distribution with specified mean and variance.
        /// </summary>
        public static Vector2 RandomGaussian(System.Random random, Vector2 mean, Vector2 variance) => new Vector2((float)(Math.Sqrt((double)variance.x) * Math.Cos(2 * Math.PI * random.NextDouble()) * Math.Sqrt(-2.0 * Math.Log(random.NextDouble()))) + mean.x, (float)(Math.Sqrt((double)variance.y) * Math.Cos(2 * Math.PI * random.NextDouble()) * Math.Sqrt(-2.0 * Math.Log(random.NextDouble()))) + mean.y);
        
        /// <summary>
        /// Returns a Vector2 with independent and identically distributed values according to a normal/Gaussian distribution with specified mean and variance.
        /// </summary>
        public static Vector2 RandomGaussian(System.Random random, Vector2 mean, float variance) => new Vector2((float)(Math.Sqrt((double)variance) * Math.Cos(2 * Math.PI * random.NextDouble()) * Math.Sqrt(-2.0 * Math.Log(random.NextDouble()))) + mean.x, (float)(Math.Sqrt((double)variance) * Math.Cos(2 * Math.PI * random.NextDouble()) * Math.Sqrt(-2.0 * Math.Log(random.NextDouble()))) + mean.y);
        
        /// <summary>
        /// Returns a Vector2 with independent and identically distributed values according to a normal/Gaussian distribution with specified mean and variance.
        /// </summary>
        public static Vector2 RandomGaussian(System.Random random, float mean, Vector2 variance) => new Vector2((float)(Math.Sqrt((double)variance.x) * Math.Cos(2 * Math.PI * random.NextDouble()) * Math.Sqrt(-2.0 * Math.Log(random.NextDouble()))) + mean, (float)(Math.Sqrt((double)variance.y) * Math.Cos(2 * Math.PI * random.NextDouble()) * Math.Sqrt(-2.0 * Math.Log(random.NextDouble()))) + mean);
        
        /// <summary>
        /// Returns a Vector2 with independent and identically distributed values according to a normal/Gaussian distribution with specified mean and variance.
        /// </summary>
        public static Vector2 RandomGaussian(System.Random random, float mean, float variance) => new Vector2((float)(Math.Sqrt((double)variance) * Math.Cos(2 * Math.PI * random.NextDouble()) * Math.Sqrt(-2.0 * Math.Log(random.NextDouble()))) + mean);

        #endregion


        #region Component-Wise Operator Overloads
        
        /// <summary>
        /// Returns a bVector2 from component-wise application of operator&lt; (lhs &lt; rhs).
        /// </summary>
        public static bVector2 operator<(Vector2 lhs, Vector2 rhs) => new bVector2(lhs.x < rhs.x, lhs.y < rhs.y);
        
        /// <summary>
        /// Returns a bVector2 from component-wise application of operator&lt; (lhs &lt; rhs).
        /// </summary>
        public static bVector2 operator<(Vector2 lhs, float rhs) => new bVector2(lhs.x < rhs, lhs.y < rhs);
        
        /// <summary>
        /// Returns a bVector2 from component-wise application of operator&lt; (lhs &lt; rhs).
        /// </summary>
        public static bVector2 operator<(float lhs, Vector2 rhs) => new bVector2(lhs < rhs.x, lhs < rhs.y);
        
        /// <summary>
        /// Returns a bVector2 from component-wise application of operator&lt;= (lhs &lt;= rhs).
        /// </summary>
        public static bVector2 operator<=(Vector2 lhs, Vector2 rhs) => new bVector2(lhs.x <= rhs.x, lhs.y <= rhs.y);
        
        /// <summary>
        /// Returns a bVector2 from component-wise application of operator&lt;= (lhs &lt;= rhs).
        /// </summary>
        public static bVector2 operator<=(Vector2 lhs, float rhs) => new bVector2(lhs.x <= rhs, lhs.y <= rhs);
        
        /// <summary>
        /// Returns a bVector2 from component-wise application of operator&lt;= (lhs &lt;= rhs).
        /// </summary>
        public static bVector2 operator<=(float lhs, Vector2 rhs) => new bVector2(lhs <= rhs.x, lhs <= rhs.y);
        
        /// <summary>
        /// Returns a bVector2 from component-wise application of operator&gt; (lhs &gt; rhs).
        /// </summary>
        public static bVector2 operator>(Vector2 lhs, Vector2 rhs) => new bVector2(lhs.x > rhs.x, lhs.y > rhs.y);
        
        /// <summary>
        /// Returns a bVector2 from component-wise application of operator&gt; (lhs &gt; rhs).
        /// </summary>
        public static bVector2 operator>(Vector2 lhs, float rhs) => new bVector2(lhs.x > rhs, lhs.y > rhs);
        
        /// <summary>
        /// Returns a bVector2 from component-wise application of operator&gt; (lhs &gt; rhs).
        /// </summary>
        public static bVector2 operator>(float lhs, Vector2 rhs) => new bVector2(lhs > rhs.x, lhs > rhs.y);
        
        /// <summary>
        /// Returns a bVector2 from component-wise application of operator&gt;= (lhs &gt;= rhs).
        /// </summary>
        public static bVector2 operator>=(Vector2 lhs, Vector2 rhs) => new bVector2(lhs.x >= rhs.x, lhs.y >= rhs.y);
        
        /// <summary>
        /// Returns a bVector2 from component-wise application of operator&gt;= (lhs &gt;= rhs).
        /// </summary>
        public static bVector2 operator>=(Vector2 lhs, float rhs) => new bVector2(lhs.x >= rhs, lhs.y >= rhs);
        
        /// <summary>
        /// Returns a bVector2 from component-wise application of operator&gt;= (lhs &gt;= rhs).
        /// </summary>
        public static bVector2 operator>=(float lhs, Vector2 rhs) => new bVector2(lhs >= rhs.x, lhs >= rhs.y);
        
        /// <summary>
        /// Returns a Vector2 from component-wise application of operator+ (lhs + rhs).
        /// </summary>
        public static Vector2 operator+(Vector2 lhs, Vector2 rhs) => new Vector2(lhs.x + rhs.x, lhs.y + rhs.y);
        
        /// <summary>
        /// Returns a Vector2 from component-wise application of operator+ (lhs + rhs).
        /// </summary>
        public static Vector2 operator+(Vector2 lhs, float rhs) => new Vector2(lhs.x + rhs, lhs.y + rhs);
        
        /// <summary>
        /// Returns a Vector2 from component-wise application of operator+ (lhs + rhs).
        /// </summary>
        public static Vector2 operator+(float lhs, Vector2 rhs) => new Vector2(lhs + rhs.x, lhs + rhs.y);
        
        /// <summary>
        /// Returns a Vector2 from component-wise application of operator- (lhs - rhs).
        /// </summary>
        public static Vector2 operator-(Vector2 lhs, Vector2 rhs) => new Vector2(lhs.x - rhs.x, lhs.y - rhs.y);
        
        /// <summary>
        /// Returns a Vector2 from component-wise application of operator- (lhs - rhs).
        /// </summary>
        public static Vector2 operator-(Vector2 lhs, float rhs) => new Vector2(lhs.x - rhs, lhs.y - rhs);
        
        /// <summary>
        /// Returns a Vector2 from component-wise application of operator- (lhs - rhs).
        /// </summary>
        public static Vector2 operator-(float lhs, Vector2 rhs) => new Vector2(lhs - rhs.x, lhs - rhs.y);
        
        /// <summary>
        /// Returns a Vector2 from component-wise application of operator* (lhs * rhs).
        /// </summary>
        public static Vector2 operator*(Vector2 lhs, Vector2 rhs) => new Vector2(lhs.x * rhs.x, lhs.y * rhs.y);
        
        /// <summary>
        /// Returns a Vector2 from component-wise application of operator* (lhs * rhs).
        /// </summary>
        public static Vector2 operator*(Vector2 lhs, float rhs) => new Vector2(lhs.x * rhs, lhs.y * rhs);
        
        /// <summary>
        /// Returns a Vector2 from component-wise application of operator* (lhs * rhs).
        /// </summary>
        public static Vector2 operator*(float lhs, Vector2 rhs) => new Vector2(lhs * rhs.x, lhs * rhs.y);
        
        /// <summary>
        /// Returns a Vector2 from component-wise application of operator/ (lhs / rhs).
        /// </summary>
        public static Vector2 operator/(Vector2 lhs, Vector2 rhs) => new Vector2(lhs.x / rhs.x, lhs.y / rhs.y);
        
        /// <summary>
        /// Returns a Vector2 from component-wise application of operator/ (lhs / rhs).
        /// </summary>
        public static Vector2 operator/(Vector2 lhs, float rhs) => new Vector2(lhs.x / rhs, lhs.y / rhs);
        
        /// <summary>
        /// Returns a Vector2 from component-wise application of operator/ (lhs / rhs).
        /// </summary>
        public static Vector2 operator/(float lhs, Vector2 rhs) => new Vector2(lhs / rhs.x, lhs / rhs.y);
        
        /// <summary>
        /// Returns a Vector2 from component-wise application of operator+ (identity).
        /// </summary>
        public static Vector2 operator+(Vector2 v) => v;
        
        /// <summary>
        /// Returns a Vector2 from component-wise application of operator- (-v).
        /// </summary>
        public static Vector2 operator-(Vector2 v) => new Vector2(-v.x, -v.y);
        
        /// <summary>
        /// Returns a Vector2 from component-wise application of operator% (lhs % rhs).
        /// </summary>
        public static Vector2 operator%(Vector2 lhs, Vector2 rhs) => new Vector2(lhs.x % rhs.x, lhs.y % rhs.y);
        
        /// <summary>
        /// Returns a Vector2 from component-wise application of operator% (lhs % rhs).
        /// </summary>
        public static Vector2 operator%(Vector2 lhs, float rhs) => new Vector2(lhs.x % rhs, lhs.y % rhs);
        
        /// <summary>
        /// Returns a Vector2 from component-wise application of operator% (lhs % rhs).
        /// </summary>
        public static Vector2 operator%(float lhs, Vector2 rhs) => new Vector2(lhs % rhs.x, lhs % rhs.y);

        #endregion

    }
}
