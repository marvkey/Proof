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
    /// A Vector of type float with 3 components.
    /// </summary>

    [RegisterCoreClassStruct]
    [StructLayout(LayoutKind.Sequential)]
    public struct Vector3 : IReadOnlyList<float>, IEquatable<Vector3>
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
        
        /// <summary>
        /// z-component
        /// </summary>
        
        public float z;

        #endregion


        #region Constructors
        
        /// <summary>
        /// Component-wise constructor
        /// </summary>
        public Vector3(float x, float y, float z)
        {
            this.x = x;
            this.y = y;
            this.z = z;
        }
        
        /// <summary>
        /// all-same-value constructor
        /// </summary>
        public Vector3(float v)
        {
            this.x = v;
            this.y = v;
            this.z = v;
        }
        
        /// <summary>
        /// from-Vector constructor (empty fields are zero/false)
        /// </summary>
        public Vector3(Vector2 v)
        {
            this.x = v.x;
            this.y = v.y;
            this.z = 0f;
        }
        
        /// <summary>
        /// from-Vector-and-value constructor
        /// </summary>
        public Vector3(Vector2 v, float z)
        {
            this.x = v.x;
            this.y = v.y;
            this.z = z;
        }
        
        /// <summary>
        /// from-Vector constructor
        /// </summary>
        public Vector3(Vector3 v)
        {
            this.x = v.x;
            this.y = v.y;
            this.z = v.z;
        }
        
        /// <summary>
        /// from-Vector constructor (additional fields are truncated)
        /// </summary>
        public Vector3(Vector4 v)
        {
            this.x = v.x;
            this.y = v.y;
            this.z = v.z;
        }
        
        /// <summary>
        /// From-array/list constructor (superfluous values are ignored, missing values are zero-filled).
        /// </summary>
        public Vector3(IReadOnlyList<float> v)
        {
            var c = v.Count;
            this.x = c < 0 ? 0f : v[0];
            this.y = c < 1 ? 0f : v[1];
            this.z = c < 2 ? 0f : v[2];
        }
        
        /// <summary>
        /// Generic from-array constructor (superfluous values are ignored, missing values are zero-filled).
        /// </summary>
        public Vector3(Object[] v)
        {
            var c = v.Length;
            this.x = c < 0 ? 0f : (float)v[0];
            this.y = c < 1 ? 0f : (float)v[1];
            this.z = c < 2 ? 0f : (float)v[2];
        }
        
        /// <summary>
        /// From-array constructor (superfluous values are ignored, missing values are zero-filled).
        /// </summary>
        public Vector3(float[] v)
        {
            var c = v.Length;
            this.x = c < 0 ? 0f : v[0];
            this.y = c < 1 ? 0f : v[1];
            this.z = c < 2 ? 0f : v[2];
        }
        
        /// <summary>
        /// From-array constructor with base index (superfluous values are ignored, missing values are zero-filled).
        /// </summary>
        public Vector3(float[] v, int startIndex)
        {
            var c = v.Length;
            this.x = c + startIndex < 0 ? 0f : v[0 + startIndex];
            this.y = c + startIndex < 1 ? 0f : v[1 + startIndex];
            this.z = c + startIndex < 2 ? 0f : v[2 + startIndex];
        }
        
        /// <summary>
        /// From-IEnumerable constructor (superfluous values are ignored, missing values are zero-filled).
        /// </summary>
        public Vector3(IEnumerable<float> v)
            : this(v.ToArray())
        {
        }

        #endregion


        #region Implicit Operators
        
        /// <summary>
        /// Implicitly converts this to a dVector3.
        /// </summary>
        public static implicit operator dVector3(Vector3 v) => new dVector3((double)v.x, (double)v.y, (double)v.z);
        
        /// <summary>
        /// Implicitly converts this to a cVector3.
        /// </summary>
       // public static implicit operator cVector3(Vector3 v) => new cVector3((Complex)v.x, (Complex)v.y, (Complex)v.z);

        #endregion


        #region Explicit Operators
        
        /// <summary>
        /// Explicitly converts this to a iVector2.
        /// </summary>
        public static explicit operator iVector2(Vector3 v) => new iVector2((int)v.x, (int)v.y);
        
        /// <summary>
        /// Explicitly converts this to a iVector3.
        /// </summary>
        public static explicit operator iVector3(Vector3 v) => new iVector3((int)v.x, (int)v.y, (int)v.z);
        
        /// <summary>
        /// Explicitly converts this to a iVector4. (Higher components are zeroed)
        /// </summary>
        public static explicit operator iVector4(Vector3 v) => new iVector4((int)v.x, (int)v.y, (int)v.z, 0);
        
        /// <summary>
        /// Explicitly converts this to a uVector2.
        /// </summary>
        public static explicit operator uVector2(Vector3 v) => new uVector2((uint)v.x, (uint)v.y);
        
        /// <summary>
        /// Explicitly converts this to a uVector3.
        /// </summary>
        public static explicit operator uVector3(Vector3 v) => new uVector3((uint)v.x, (uint)v.y, (uint)v.z);
        
        /// <summary>
        /// Explicitly converts this to a uVector4. (Higher components are zeroed)
        /// </summary>
        public static explicit operator uVector4(Vector3 v) => new uVector4((uint)v.x, (uint)v.y, (uint)v.z, 0u);
        
        /// <summary>
        /// Explicitly converts this to a Vector2.
        /// </summary>
        public static explicit operator Vector2(Vector3 v) => new Vector2((float)v.x, (float)v.y);
        
        /// <summary>
        /// Explicitly converts this to a Vector4. (Higher components are zeroed)
        /// </summary>
        public static explicit operator Vector4(Vector3 v) => new Vector4((float)v.x, (float)v.y, (float)v.z, 0f);
        
        /// <summary>
        /// Explicitly converts this to a hVector2.
        /// </summary>
        public static explicit operator hVector2(Vector3 v) => new hVector2((Half)v.x, (Half)v.y);
        
        /// <summary>
        /// Explicitly converts this to a hVector3.
        /// </summary>
        public static explicit operator hVector3(Vector3 v) => new hVector3((Half)v.x, (Half)v.y, (Half)v.z);
        
        /// <summary>
        /// Explicitly converts this to a hVector4. (Higher components are zeroed)
        /// </summary>
        public static explicit operator hVector4(Vector3 v) => new hVector4((Half)v.x, (Half)v.y, (Half)v.z, Half.Zero);
        
        /// <summary>
        /// Explicitly converts this to a dVector2.
        /// </summary>
        public static explicit operator dVector2(Vector3 v) => new dVector2((double)v.x, (double)v.y);
        
        /// <summary>
        /// Explicitly converts this to a dVector4. (Higher components are zeroed)
        /// </summary>
        public static explicit operator dVector4(Vector3 v) => new dVector4((double)v.x, (double)v.y, (double)v.z, 0.0);
        
        /// <summary>
        /// Explicitly converts this to a decVector2.
        /// </summary>
        public static explicit operator decVector2(Vector3 v) => new decVector2((decimal)v.x, (decimal)v.y);
        
        /// <summary>
        /// Explicitly converts this to a decVector3.
        /// </summary>
        public static explicit operator decVector3(Vector3 v) => new decVector3((decimal)v.x, (decimal)v.y, (decimal)v.z);
        
        /// <summary>
        /// Explicitly converts this to a decVector4. (Higher components are zeroed)
        /// </summary>
        public static explicit operator decVector4(Vector3 v) => new decVector4((decimal)v.x, (decimal)v.y, (decimal)v.z, 0m);
        
        /// <summary>
        /// Explicitly converts this to a cVector2.
        /// </summary>
        //public static explicit operator cVector2(Vector3 v) => new cVector2((Complex)v.x, (Complex)v.y);
        
        /// <summary>
        /// Explicitly converts this to a cVector4. (Higher components are zeroed)
        /// </summary>
        //public static explicit operator cVector4(Vector3 v) => new cVector4((Complex)v.x, (Complex)v.y, (Complex)v.z, Complex.Zero);
        
        /// <summary>
        /// Explicitly converts this to a lVector2.
        /// </summary>
        //public static explicit operator lVector2(Vector3 v) => new lVector2((long)v.x, (long)v.y);
        
        /// <summary>
        /// Explicitly converts this to a lVector3.
        /// </summary>
        public static explicit operator lVector3(Vector3 v) => new lVector3((long)v.x, (long)v.y, (long)v.z);
        
        /// <summary>
        /// Explicitly converts this to a lVector4. (Higher components are zeroed)
        /// </summary>
        public static explicit operator lVector4(Vector3 v) => new lVector4((long)v.x, (long)v.y, (long)v.z, 0);
        
        /// <summary>
        /// Explicitly converts this to a bVector2.
        /// </summary>
        public static explicit operator bVector2(Vector3 v) => new bVector2(v.x != 0f, v.y != 0f);
        
        /// <summary>
        /// Explicitly converts this to a bVector3.
        /// </summary>
        public static explicit operator bVector3(Vector3 v) => new bVector3(v.x != 0f, v.y != 0f, v.z != 0f);
        
        /// <summary>
        /// Explicitly converts this to a bVector4. (Higher components are zeroed)
        /// </summary>
        public static explicit operator bVector4(Vector3 v) => new bVector4(v.x != 0f, v.y != 0f, v.z != 0f, false);
        
        /// <summary>
        /// Explicitly converts this to a float array.
        /// </summary>
        public static explicit operator float[](Vector3 v) => new [] { v.x, v.y, v.z };
        
        /// <summary>
        /// Explicitly converts this to a generic object array.
        /// </summary>
        public static explicit operator Object[](Vector3 v) => new Object[] { v.x, v.y, v.z };

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
        public swizzle_Vector3 swizzle => new swizzle_Vector3(x, y, z);
        
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
        public Vector2 xz
        {
            get
            {
                return new Vector2(x, z);
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
        public Vector2 yz
        {
            get
            {
                return new Vector2(y, z);
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
        public Vector3 xyz
        {
            get
            {
                return new Vector3(x, y, z);
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
        /// Gets or sets the specified subset of components. For more advanced (read-only) swizzling, use the .swizzle property.
        /// </summary>
        public Vector2 rb
        {
            get
            {
                return new Vector2(x, z);
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
        public Vector2 gb
        {
            get
            {
                return new Vector2(y, z);
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
        public Vector3 rgb
        {
            get
            {
                return new Vector3(x, y, z);
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
        /// Gets or sets the specified RGBA component. For more advanced (read-only) swizzling, use the .swizzle property.
        /// </summary>
        public float b
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
        public float[] Values => new[] { x, y, z };
        
        /// <summary>
        /// Returns the number of components (3).
        /// </summary>
        public int Count => 3;
        
        /// <summary>
        /// Returns the minimal component of this Vector.
        /// </summary>
        public float MinElement => Math.Min(Math.Min(x, y), z);
        
        /// <summary>
        /// Returns the maximal component of this Vector.
        /// </summary>
        public float MaxElement => Math.Max(Math.Max(x, y), z);
        
        /// <summary>
        /// Returns the euclidean length of this Vector.
        /// </summary>
        public float Length => (float)Math.Sqrt(((x*x + y*y) + z*z));

        // added by proof
        public float Magnitude => (float)Math.Sqrt(((x * x + y * y) + z * z));

        /// <summary>
        /// Returns the squared euclidean length of this Vector.
        /// </summary>
        public float LengthSqr => ((x*x + y*y) + z*z);

        public float MagnitudeSqr => LengthSqr;
         
        /// <summary>
        /// Returns the sum of all components.
        /// </summary>
        public float Sum => ((x + y) + z);
        
        /// <summary>
        /// Returns the euclidean norm of this Vector.
        /// </summary>
        public float Norm => (float)Math.Sqrt(((x*x + y*y) + z*z));
        
        /// <summary>
        /// Returns the one-norm of this Vector.
        /// </summary>
        public float Norm1 => ((Math.Abs(x) + Math.Abs(y)) + Math.Abs(z));
        
        /// <summary>
        /// Returns the two-norm (euclidean length) of this Vector.
        /// </summary>
        public float Norm2 => (float)Math.Sqrt(((x*x + y*y) + z*z));
        
        /// <summary>
        /// Returns the max-norm of this Vector.
        /// </summary>
        public float NormMax => Math.Max(Math.Max(Math.Abs(x), Math.Abs(y)), Math.Abs(z));
        
        /// <summary>
        /// Returns a copy of this Vector with length one (undefined if this has zero length).
        /// </summary>
        public Vector3 Normalized => this / (float)Length;
        
        /// <summary>
        /// Returns a copy of this Vector with length one (returns zero if length is zero).
        /// </summary>
        public Vector3 NormalizedSafe => this == Zero ? Zero : this / (float)Length;

        #endregion


        #region Static Properties
        
        /// <summary>
        /// Predefined all-zero Vector
        /// </summary>
        public static Vector3 Zero { get; } = new Vector3(0f, 0f, 0f);
        
        /// <summary>
        /// Predefined all-ones Vector
        /// </summary>
        public static Vector3 Ones { get; } = new Vector3(1f, 1f, 1f);
        
        /// <summary>
        /// Predefined unit-X Vector
        /// </summary>
        public static Vector3 UnitX { get; } = new Vector3(1f, 0f, 0f);
        
        /// <summary>
        /// Predefined unit-Y Vector
        /// </summary>
        public static Vector3 UnitY { get; } = new Vector3(0f, 1f, 0f);
        
        /// <summary>
        /// Predefined unit-Z Vector
        /// </summary>
        public static Vector3 UnitZ { get; } = new Vector3(0f, 0f, 1f);
        
        /// <summary>
        /// Predefined all-MaxValue Vector
        /// </summary>
        public static Vector3 MaxValue { get; } = new Vector3(float.MaxValue, float.MaxValue, float.MaxValue);
        
        /// <summary>
        /// Predefined all-MinValue Vector
        /// </summary>
        public static Vector3 MinValue { get; } = new Vector3(float.MinValue, float.MinValue, float.MinValue);
        
        /// <summary>
        /// Predefined all-Epsilon Vector
        /// </summary>
        public static Vector3 Epsilon { get; } = new Vector3(float.Epsilon, float.Epsilon, float.Epsilon);
        
        /// <summary>
        /// Predefined all-NaN Vector
        /// </summary>
        public static Vector3 NaN { get; } = new Vector3(float.NaN, float.NaN, float.NaN);
        
        /// <summary>
        /// Predefined all-NegativeInfinity Vector
        /// </summary>
        public static Vector3 NegativeInfinity { get; } = new Vector3(float.NegativeInfinity, float.NegativeInfinity, float.NegativeInfinity);
        
        /// <summary>
        /// Predefined all-PositiveInfinity Vector
        /// </summary>
        public static Vector3 PositiveInfinity { get; } = new Vector3(float.PositiveInfinity, float.PositiveInfinity, float.PositiveInfinity);

        #endregion


        #region Operators
        
        /// <summary>
        /// Returns true iff this equals rhs component-wise.
        /// </summary>
        public static bool operator==(Vector3 lhs, Vector3 rhs) => lhs.Equals(rhs);
        
        /// <summary>
        /// Returns true iff this does not equal rhs (component-wise).
        /// </summary>
        public static bool operator!=(Vector3 lhs, Vector3 rhs) => !lhs.Equals(rhs);

        #endregion


        #region Functions
        
        /// <summary>
        /// Returns an enumerator that iterates through all components.
        /// </summary>
        public IEnumerator<float> GetEnumerator()
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
        public bool Equals(Vector3 rhs) => ((x.Equals(rhs.x) && y.Equals(rhs.y)) && z.Equals(rhs.z));
        
        /// <summary>
        /// Returns true iff this equals rhs type- and component-wise.
        /// </summary>
        public override bool Equals(object obj)
        {
            if (ReferenceEquals(null, obj)) return false;
            return obj is Vector3 && Equals((Vector3) obj);
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
        public double NormP(double p) => Math.Pow(((Math.Pow((double)Math.Abs(x), p) + Math.Pow((double)Math.Abs(y), p)) + Math.Pow((double)Math.Abs(z), p)), 1 / p);

        #endregion


        #region Static Functions
        
        /// <summary>
        /// Converts the string representation of the Vector into a Vector representation (using ', ' as a separator).
        /// </summary>
        public static Vector3 Parse(string s) => Parse(s, ", ");
        
        /// <summary>
        /// Converts the string representation of the Vector into a Vector representation (using a designated separator).
        /// </summary>
        public static Vector3 Parse(string s, string sep)
        {
            var kvp = s.Split(new[] { sep }, StringSplitOptions.None);
            if (kvp.Length != 3) throw new FormatException("input has not exactly 3 parts");
            return new Vector3(float.Parse(kvp[0].Trim()), float.Parse(kvp[1].Trim()), float.Parse(kvp[2].Trim()));
        }
        
        /// <summary>
        /// Converts the string representation of the Vector into a Vector representation (using a designated separator and a type provider).
        /// </summary>
        public static Vector3 Parse(string s, string sep, IFormatProvider provider)
        {
            var kvp = s.Split(new[] { sep }, StringSplitOptions.None);
            if (kvp.Length != 3) throw new FormatException("input has not exactly 3 parts");
            return new Vector3(float.Parse(kvp[0].Trim(), provider), float.Parse(kvp[1].Trim(), provider), float.Parse(kvp[2].Trim(), provider));
        }
        
        /// <summary>
        /// Converts the string representation of the Vector into a Vector representation (using a designated separator and a number style).
        /// </summary>
        public static Vector3 Parse(string s, string sep, NumberStyles style)
        {
            var kvp = s.Split(new[] { sep }, StringSplitOptions.None);
            if (kvp.Length != 3) throw new FormatException("input has not exactly 3 parts");
            return new Vector3(float.Parse(kvp[0].Trim(), style), float.Parse(kvp[1].Trim(), style), float.Parse(kvp[2].Trim(), style));
        }
        
        /// <summary>
        /// Converts the string representation of the Vector into a Vector representation (using a designated separator and a number style and a forMatrix provider).
        /// </summary>
        public static Vector3 Parse(string s, string sep, NumberStyles style, IFormatProvider provider)
        {
            var kvp = s.Split(new[] { sep }, StringSplitOptions.None);
            if (kvp.Length != 3) throw new FormatException("input has not exactly 3 parts");
            return new Vector3(float.Parse(kvp[0].Trim(), style, provider), float.Parse(kvp[1].Trim(), style, provider), float.Parse(kvp[2].Trim(), style, provider));
        }
        
        /// <summary>
        /// Tries to convert the string representation of the Vector into a Vector representation (using ', ' as a separator), returns false if string was invalid.
        /// </summary>
        public static bool TryParse(string s, out Vector3 result) => TryParse(s, ", ", out result);
        
        /// <summary>
        /// Tries to convert the string representation of the Vector into a Vector representation (using a designated separator), returns false if string was invalid.
        /// </summary>
        public static bool TryParse(string s, string sep, out Vector3 result)
        {
            result = Zero;
            if (string.IsNullOrEmpty(s)) return false;
            var kvp = s.Split(new[] { sep }, StringSplitOptions.None);
            if (kvp.Length != 3) return false;
            float x = 0f, y = 0f, z = 0f;
            var ok = ((float.TryParse(kvp[0].Trim(), out x) && float.TryParse(kvp[1].Trim(), out y)) && float.TryParse(kvp[2].Trim(), out z));
            result = ok ? new Vector3(x, y, z) : Zero;
            return ok;
        }
        
        /// <summary>
        /// Tries to convert the string representation of the Vector into a Vector representation (using a designated separator and a number style and a forMatrix provider), returns false if string was invalid.
        /// </summary>
        public static bool TryParse(string s, string sep, NumberStyles style, IFormatProvider provider, out Vector3 result)
        {
            result = Zero;
            if (string.IsNullOrEmpty(s)) return false;
            var kvp = s.Split(new[] { sep }, StringSplitOptions.None);
            if (kvp.Length != 3) return false;
            float x = 0f, y = 0f, z = 0f;
            var ok = ((float.TryParse(kvp[0].Trim(), style, provider, out x) && float.TryParse(kvp[1].Trim(), style, provider, out y)) && float.TryParse(kvp[2].Trim(), style, provider, out z));
            result = ok ? new Vector3(x, y, z) : Zero;
            return ok;
        }
        
        /// <summary>
        /// Returns true iff distance between lhs and rhs is less than or equal to epsilon
        /// </summary>
        public static bool ApproxEqual(Vector3 lhs, Vector3 rhs, float eps = 0.1f) => Distance(lhs, rhs) <= eps;
        
        /// <summary>
        /// OuterProduct treats the first parameter c as a column Vector (Matrix with one column) and the second parameter r as a row Vector (Matrix with one row) and does a linear algebraic Matrix multiply c * r, yielding a Matrix whose number of rows is the number of components in c and whose number of columns is the number of components in r.
        /// </summary>
        public static Matrix3x2 OuterProduct(Vector2 c, Vector3 r) => new Matrix3x2(c.x * r.x, c.y * r.x, c.x * r.y, c.y * r.y, c.x * r.z, c.y * r.z);
        
        /// <summary>
        /// OuterProduct treats the first parameter c as a column Vector (Matrix with one column) and the second parameter r as a row Vector (Matrix with one row) and does a linear algebraic Matrix multiply c * r, yielding a Matrix whose number of rows is the number of components in c and whose number of columns is the number of components in r.
        /// </summary>
        public static Matrix2x3 OuterProduct(Vector3 c, Vector2 r) => new Matrix2x3(c.x * r.x, c.y * r.x, c.z * r.x, c.x * r.y, c.y * r.y, c.z * r.y);
        
        /// <summary>
        /// OuterProduct treats the first parameter c as a column Vector (Matrix with one column) and the second parameter r as a row Vector (Matrix with one row) and does a linear algebraic Matrix multiply c * r, yielding a Matrix whose number of rows is the number of components in c and whose number of columns is the number of components in r.
        /// </summary>
        public static Matrix3 OuterProduct(Vector3 c, Vector3 r) => new Matrix3(c.x * r.x, c.y * r.x, c.z * r.x, c.x * r.y, c.y * r.y, c.z * r.y, c.x * r.z, c.y * r.z, c.z * r.z);
        
        /// <summary>
        /// OuterProduct treats the first parameter c as a column Vector (Matrix with one column) and the second parameter r as a row Vector (Matrix with one row) and does a linear algebraic Matrix multiply c * r, yielding a Matrix whose number of rows is the number of components in c and whose number of columns is the number of components in r.
        /// </summary>
        public static Matrix3x4 OuterProduct(Vector4 c, Vector3 r) => new Matrix3x4(c.x * r.x, c.y * r.x, c.z * r.x, c.w * r.x, c.x * r.y, c.y * r.y, c.z * r.y, c.w * r.y, c.x * r.z, c.y * r.z, c.z * r.z, c.w * r.z);
        
        /// <summary>
        /// OuterProduct treats the first parameter c as a column Vector (Matrix with one column) and the second parameter r as a row Vector (Matrix with one row) and does a linear algebraic Matrix multiply c * r, yielding a Matrix whose number of rows is the number of components in c and whose number of columns is the number of components in r.
        /// </summary>
        public static Matrix4x3 OuterProduct(Vector3 c, Vector4 r) => new Matrix4x3(c.x * r.x, c.y * r.x, c.z * r.x, c.x * r.y, c.y * r.y, c.z * r.y, c.x * r.z, c.y * r.z, c.z * r.z, c.x * r.w, c.y * r.w, c.z * r.w);
        
        /// <summary>
        /// Returns the inner product (dot product, scalar product) of the two Vectors.
        /// </summary>
        public static float Dot(Vector3 lhs, Vector3 rhs) => ((lhs.x * rhs.x + lhs.y * rhs.y) + lhs.z * rhs.z);
        
        /// <summary>
        /// Returns the euclidean distance between the two Vectors.
        /// </summary>
        public static float Distance(Vector3 lhs, Vector3 rhs) => (lhs - rhs).Length;
        
        /// <summary>
        /// Returns the squared euclidean distance between the two Vectors.
        /// </summary>
        public static float DistanceSqr(Vector3 lhs, Vector3 rhs) => (lhs - rhs).LengthSqr;
        
        /// <summary>
        /// Calculate the reflection direction for an incident Vector (N should be normalized in order to achieve the desired result).
        /// </summary>
        public static Vector3 Reflect(Vector3 I, Vector3 N) => I - 2 * Dot(N, I) * N;
        
        /// <summary>
        /// Calculate the refraction direction for an incident Vector (The input parameters I and N should be normalized in order to achieve the desired result).
        /// </summary>
        public static Vector3 Refract(Vector3 I, Vector3 N, float eta)
        {
            var dNI = Dot(N, I);
            var k = 1 - eta * eta * (1 - dNI * dNI);
            if (k < 0) return Zero;
            return eta * I - (eta * dNI + (float)Math.Sqrt(k)) * N;
        }
        
        /// <summary>
        /// Returns a Vector pointing in the same direction as another (faceforward orients a Vector to point away from a surface as defined by its normal. If dot(Nref, I) is negative faceforward returns N, otherwise it returns -N).
        /// </summary>
        public static Vector3 FaceForward(Vector3 N, Vector3 I, Vector3 Nref) => Dot(Nref, I) < 0 ? N : -N;
        
        /// <summary>
        /// Returns the outer product (cross product, Vector product) of the two Vectors.
        /// </summary>
        public static Vector3 Cross(Vector3 l, Vector3 r) => new Vector3(l.y * r.z - l.z * r.y, l.z * r.x - l.x * r.z, l.x * r.y - l.y * r.x);
        
        /// <summary>
        /// Returns a Vector3 with independent and identically distributed uniform values between 0.0 and 1.0.
        /// </summary>
        public static Vector3 Random(System.Random random) => new Vector3((float)random.NextDouble(), (float)random.NextDouble(), (float)random.NextDouble());
        
        /// <summary>
        /// Returns a Vector3 with independent and identically distributed uniform values between -1.0 and 1.0.
        /// </summary>
        public static Vector3 RandomSigned(System.Random random) => new Vector3((float)(random.NextDouble() * 2.0 - 1.0), (float)(random.NextDouble() * 2.0 - 1.0), (float)(random.NextDouble() * 2.0 - 1.0));
        
        /// <summary>
        /// Returns a Vector3 with independent and identically distributed values according to a normal distribution (zero mean, unit variance).
        /// </summary>
        public static Vector3 RandomNormal(System.Random random) => new Vector3((float)(Math.Cos(2 * Math.PI * random.NextDouble()) * Math.Sqrt(-2.0 * Math.Log(random.NextDouble()))), (float)(Math.Cos(2 * Math.PI * random.NextDouble()) * Math.Sqrt(-2.0 * Math.Log(random.NextDouble()))), (float)(Math.Cos(2 * Math.PI * random.NextDouble()) * Math.Sqrt(-2.0 * Math.Log(random.NextDouble()))));


        // createed by proof
        /// <summary>
        /// Returns the angle in radians between two vectors.
        /// </summary>
        /// 
        public const float kEpsilonNormalSqrt = 1e-15F;

        public static float Angle(Vector3 a, Vector3 b)
        {
            /*
            float dot = Dot(a, b);
            float cosTheta = dot / (a.Magnitude * b.Magnitude);

            return (float)Math.Acos(cosTheta);
            */

            float denominator = (float)Mathf.Sqrt(a.MagnitudeSqr * b.MagnitudeSqr);
            if (denominator < kEpsilonNormalSqrt)
                return 0F;
            
            float dot = Mathf.Clamp(Dot(a, b) / denominator, -1F, 1F);
            return ((float)Math.Acos(dot));
        }

        // createed by proof
        /// <summary>
        /// Returns the angle in degrees between two vectors.
        /// </summary>
        public static float AngleDegrees(Vector3 a, Vector3 b)
        {
            return Angle(a, b) * (180f / (float)Math.PI);
        }


        // added by proof
        public static Vector3 ProjectOnPlane(Vector3 vector, Vector3 planeNormal)
        {
            float sqrMag = Dot(planeNormal, planeNormal);
            if (sqrMag < Mathf.Epsilon)
                return vector;
            else
            {
                var dot = Dot(vector, planeNormal);
                return new Vector3(vector.x - planeNormal.x * dot / sqrMag,
                    vector.y - planeNormal.y * dot / sqrMag,
                    vector.z - planeNormal.z * dot / sqrMag);
            }
        }
        #endregion


        #region Component-Wise Static Functions

        /// <summary>
        /// Returns a bVector3 from component-wise application of Equal (lhs == rhs).
        /// </summary>
        public static bVector3 Equal(Vector3 lhs, Vector3 rhs) => new bVector3(lhs.x == rhs.x, lhs.y == rhs.y, lhs.z == rhs.z);
        
        /// <summary>
        /// Returns a bVector3 from component-wise application of Equal (lhs == rhs).
        /// </summary>
        public static bVector3 Equal(Vector3 lhs, float rhs) => new bVector3(lhs.x == rhs, lhs.y == rhs, lhs.z == rhs);
        
        /// <summary>
        /// Returns a bVector3 from component-wise application of Equal (lhs == rhs).
        /// </summary>
        public static bVector3 Equal(float lhs, Vector3 rhs) => new bVector3(lhs == rhs.x, lhs == rhs.y, lhs == rhs.z);
        
        /// <summary>
        /// Returns a bVector from the application of Equal (lhs == rhs).
        /// </summary>
        public static bVector3 Equal(float lhs, float rhs) => new bVector3(lhs == rhs);
        
        /// <summary>
        /// Returns a bVector3 from component-wise application of NotEqual (lhs != rhs).
        /// </summary>
        public static bVector3 NotEqual(Vector3 lhs, Vector3 rhs) => new bVector3(lhs.x != rhs.x, lhs.y != rhs.y, lhs.z != rhs.z);
        
        /// <summary>
        /// Returns a bVector3 from component-wise application of NotEqual (lhs != rhs).
        /// </summary>
        public static bVector3 NotEqual(Vector3 lhs, float rhs) => new bVector3(lhs.x != rhs, lhs.y != rhs, lhs.z != rhs);
        
        /// <summary>
        /// Returns a bVector3 from component-wise application of NotEqual (lhs != rhs).
        /// </summary>
        public static bVector3 NotEqual(float lhs, Vector3 rhs) => new bVector3(lhs != rhs.x, lhs != rhs.y, lhs != rhs.z);
        
        /// <summary>
        /// Returns a bVector from the application of NotEqual (lhs != rhs).
        /// </summary>
        public static bVector3 NotEqual(float lhs, float rhs) => new bVector3(lhs != rhs);
        
        /// <summary>
        /// Returns a bVector3 from component-wise application of GreaterThan (lhs &gt; rhs).
        /// </summary>
        public static bVector3 GreaterThan(Vector3 lhs, Vector3 rhs) => new bVector3(lhs.x > rhs.x, lhs.y > rhs.y, lhs.z > rhs.z);
        
        /// <summary>
        /// Returns a bVector3 from component-wise application of GreaterThan (lhs &gt; rhs).
        /// </summary>
        public static bVector3 GreaterThan(Vector3 lhs, float rhs) => new bVector3(lhs.x > rhs, lhs.y > rhs, lhs.z > rhs);
        
        /// <summary>
        /// Returns a bVector3 from component-wise application of GreaterThan (lhs &gt; rhs).
        /// </summary>
        public static bVector3 GreaterThan(float lhs, Vector3 rhs) => new bVector3(lhs > rhs.x, lhs > rhs.y, lhs > rhs.z);
        
        /// <summary>
        /// Returns a bVector from the application of GreaterThan (lhs &gt; rhs).
        /// </summary>
        public static bVector3 GreaterThan(float lhs, float rhs) => new bVector3(lhs > rhs);
        
        /// <summary>
        /// Returns a bVector3 from component-wise application of GreaterThanEqual (lhs &gt;= rhs).
        /// </summary>
        public static bVector3 GreaterThanEqual(Vector3 lhs, Vector3 rhs) => new bVector3(lhs.x >= rhs.x, lhs.y >= rhs.y, lhs.z >= rhs.z);
        
        /// <summary>
        /// Returns a bVector3 from component-wise application of GreaterThanEqual (lhs &gt;= rhs).
        /// </summary>
        public static bVector3 GreaterThanEqual(Vector3 lhs, float rhs) => new bVector3(lhs.x >= rhs, lhs.y >= rhs, lhs.z >= rhs);
        
        /// <summary>
        /// Returns a bVector3 from component-wise application of GreaterThanEqual (lhs &gt;= rhs).
        /// </summary>
        public static bVector3 GreaterThanEqual(float lhs, Vector3 rhs) => new bVector3(lhs >= rhs.x, lhs >= rhs.y, lhs >= rhs.z);
        
        /// <summary>
        /// Returns a bVector from the application of GreaterThanEqual (lhs &gt;= rhs).
        /// </summary>
        public static bVector3 GreaterThanEqual(float lhs, float rhs) => new bVector3(lhs >= rhs);
        
        /// <summary>
        /// Returns a bVector3 from component-wise application of LesserThan (lhs &lt; rhs).
        /// </summary>
        public static bVector3 LesserThan(Vector3 lhs, Vector3 rhs) => new bVector3(lhs.x < rhs.x, lhs.y < rhs.y, lhs.z < rhs.z);
        
        /// <summary>
        /// Returns a bVector3 from component-wise application of LesserThan (lhs &lt; rhs).
        /// </summary>
        public static bVector3 LesserThan(Vector3 lhs, float rhs) => new bVector3(lhs.x < rhs, lhs.y < rhs, lhs.z < rhs);
        
        /// <summary>
        /// Returns a bVector3 from component-wise application of LesserThan (lhs &lt; rhs).
        /// </summary>
        public static bVector3 LesserThan(float lhs, Vector3 rhs) => new bVector3(lhs < rhs.x, lhs < rhs.y, lhs < rhs.z);
        
        /// <summary>
        /// Returns a bVector from the application of LesserThan (lhs &lt; rhs).
        /// </summary>
        public static bVector3 LesserThan(float lhs, float rhs) => new bVector3(lhs < rhs);
        
        /// <summary>
        /// Returns a bVector3 from component-wise application of LesserThanEqual (lhs &lt;= rhs).
        /// </summary>
        public static bVector3 LesserThanEqual(Vector3 lhs, Vector3 rhs) => new bVector3(lhs.x <= rhs.x, lhs.y <= rhs.y, lhs.z <= rhs.z);
        
        /// <summary>
        /// Returns a bVector3 from component-wise application of LesserThanEqual (lhs &lt;= rhs).
        /// </summary>
        public static bVector3 LesserThanEqual(Vector3 lhs, float rhs) => new bVector3(lhs.x <= rhs, lhs.y <= rhs, lhs.z <= rhs);
        
        /// <summary>
        /// Returns a bVector3 from component-wise application of LesserThanEqual (lhs &lt;= rhs).
        /// </summary>
        public static bVector3 LesserThanEqual(float lhs, Vector3 rhs) => new bVector3(lhs <= rhs.x, lhs <= rhs.y, lhs <= rhs.z);
        
        /// <summary>
        /// Returns a bVector from the application of LesserThanEqual (lhs &lt;= rhs).
        /// </summary>
        public static bVector3 LesserThanEqual(float lhs, float rhs) => new bVector3(lhs <= rhs);
        
        /// <summary>
        /// Returns a bVector3 from component-wise application of IsInfinity (float.IsInfinity(v)).
        /// </summary>
        public static bVector3 IsInfinity(Vector3 v) => new bVector3(float.IsInfinity(v.x), float.IsInfinity(v.y), float.IsInfinity(v.z));
        
        /// <summary>
        /// Returns a bVector from the application of IsInfinity (float.IsInfinity(v)).
        /// </summary>
        public static bVector3 IsInfinity(float v) => new bVector3(float.IsInfinity(v));
        
        /// <summary>
        /// Returns a bVector3 from component-wise application of IsFinite (!float.IsNaN(v) &amp;&amp; !float.IsInfinity(v)).
        /// </summary>
        public static bVector3 IsFinite(Vector3 v) => new bVector3(!float.IsNaN(v.x) && !float.IsInfinity(v.x), !float.IsNaN(v.y) && !float.IsInfinity(v.y), !float.IsNaN(v.z) && !float.IsInfinity(v.z));
        
        /// <summary>
        /// Returns a bVector from the application of IsFinite (!float.IsNaN(v) &amp;&amp; !float.IsInfinity(v)).
        /// </summary>
        public static bVector3 IsFinite(float v) => new bVector3(!float.IsNaN(v) && !float.IsInfinity(v));
        
        /// <summary>
        /// Returns a bVector3 from component-wise application of IsNaN (float.IsNaN(v)).
        /// </summary>
        public static bVector3 IsNaN(Vector3 v) => new bVector3(float.IsNaN(v.x), float.IsNaN(v.y), float.IsNaN(v.z));
        
        /// <summary>
        /// Returns a bVector from the application of IsNaN (float.IsNaN(v)).
        /// </summary>
        public static bVector3 IsNaN(float v) => new bVector3(float.IsNaN(v));
        
        /// <summary>
        /// Returns a bVector3 from component-wise application of IsNegativeInfinity (float.IsNegativeInfinity(v)).
        /// </summary>
        public static bVector3 IsNegativeInfinity(Vector3 v) => new bVector3(float.IsNegativeInfinity(v.x), float.IsNegativeInfinity(v.y), float.IsNegativeInfinity(v.z));
        
        /// <summary>
        /// Returns a bVector from the application of IsNegativeInfinity (float.IsNegativeInfinity(v)).
        /// </summary>
        public static bVector3 IsNegativeInfinity(float v) => new bVector3(float.IsNegativeInfinity(v));
        
        /// <summary>
        /// Returns a bVector3 from component-wise application of IsPositiveInfinity (float.IsPositiveInfinity(v)).
        /// </summary>
        public static bVector3 IsPositiveInfinity(Vector3 v) => new bVector3(float.IsPositiveInfinity(v.x), float.IsPositiveInfinity(v.y), float.IsPositiveInfinity(v.z));
        
        /// <summary>
        /// Returns a bVector from the application of IsPositiveInfinity (float.IsPositiveInfinity(v)).
        /// </summary>
        public static bVector3 IsPositiveInfinity(float v) => new bVector3(float.IsPositiveInfinity(v));
        
        /// <summary>
        /// Returns a Vector3 from component-wise application of Abs (Math.Abs(v)).
        /// </summary>
        public static Vector3 Abs(Vector3 v) => new Vector3(Math.Abs(v.x), Math.Abs(v.y), Math.Abs(v.z));
        
        /// <summary>
        /// Returns a Vector from the application of Abs (Math.Abs(v)).
        /// </summary>
        public static Vector3 Abs(float v) => new Vector3(Math.Abs(v));
        
        /// <summary>
        /// Returns a Vector3 from component-wise application of HermiteInterpolationOrder3 ((3 - 2 * v) * v * v).
        /// </summary>
        public static Vector3 HermiteInterpolationOrder3(Vector3 v) => new Vector3((3 - 2 * v.x) * v.x * v.x, (3 - 2 * v.y) * v.y * v.y, (3 - 2 * v.z) * v.z * v.z);
        
        /// <summary>
        /// Returns a Vector from the application of HermiteInterpolationOrder3 ((3 - 2 * v) * v * v).
        /// </summary>
        public static Vector3 HermiteInterpolationOrder3(float v) => new Vector3((3 - 2 * v) * v * v);
        
        /// <summary>
        /// Returns a Vector3 from component-wise application of HermiteInterpolationOrder5 (((6 * v - 15) * v + 10) * v * v * v).
        /// </summary>
        public static Vector3 HermiteInterpolationOrder5(Vector3 v) => new Vector3(((6 * v.x - 15) * v.x + 10) * v.x * v.x * v.x, ((6 * v.y - 15) * v.y + 10) * v.y * v.y * v.y, ((6 * v.z - 15) * v.z + 10) * v.z * v.z * v.z);
        
        /// <summary>
        /// Returns a Vector from the application of HermiteInterpolationOrder5 (((6 * v - 15) * v + 10) * v * v * v).
        /// </summary>
        public static Vector3 HermiteInterpolationOrder5(float v) => new Vector3(((6 * v - 15) * v + 10) * v * v * v);
        
        /// <summary>
        /// Returns a Vector3 from component-wise application of Sqr (v * v).
        /// </summary>
        public static Vector3 Sqr(Vector3 v) => new Vector3(v.x * v.x, v.y * v.y, v.z * v.z);
        
        /// <summary>
        /// Returns a Vector from the application of Sqr (v * v).
        /// </summary>
        public static Vector3 Sqr(float v) => new Vector3(v * v);
        
        /// <summary>
        /// Returns a Vector3 from component-wise application of Pow2 (v * v).
        /// </summary>
        public static Vector3 Pow2(Vector3 v) => new Vector3(v.x * v.x, v.y * v.y, v.z * v.z);
        
        /// <summary>
        /// Returns a Vector from the application of Pow2 (v * v).
        /// </summary>
        public static Vector3 Pow2(float v) => new Vector3(v * v);
        
        /// <summary>
        /// Returns a Vector3 from component-wise application of Pow3 (v * v * v).
        /// </summary>
        public static Vector3 Pow3(Vector3 v) => new Vector3(v.x * v.x * v.x, v.y * v.y * v.y, v.z * v.z * v.z);
        
        /// <summary>
        /// Returns a Vector from the application of Pow3 (v * v * v).
        /// </summary>
        public static Vector3 Pow3(float v) => new Vector3(v * v * v);
        
        /// <summary>
        /// Returns a Vector3 from component-wise application of Step (v &gt;= 0f ? 1f : 0f).
        /// </summary>
        public static Vector3 Step(Vector3 v) => new Vector3(v.x >= 0f ? 1f : 0f, v.y >= 0f ? 1f : 0f, v.z >= 0f ? 1f : 0f);
        
        /// <summary>
        /// Returns a Vector from the application of Step (v &gt;= 0f ? 1f : 0f).
        /// </summary>
        public static Vector3 Step(float v) => new Vector3(v >= 0f ? 1f : 0f);
        
        /// <summary>
        /// Returns a Vector3 from component-wise application of Sqrt ((float)Math.Sqrt((double)v)).
        /// </summary>
        public static Vector3 Sqrt(Vector3 v) => new Vector3((float)Math.Sqrt((double)v.x), (float)Math.Sqrt((double)v.y), (float)Math.Sqrt((double)v.z));
        
        /// <summary>
        /// Returns a Vector from the application of Sqrt ((float)Math.Sqrt((double)v)).
        /// </summary>
        public static Vector3 Sqrt(float v) => new Vector3((float)Math.Sqrt((double)v));
        
        /// <summary>
        /// Returns a Vector3 from component-wise application of InverseSqrt ((float)(1.0 / Math.Sqrt((double)v))).
        /// </summary>
        public static Vector3 InverseSqrt(Vector3 v) => new Vector3((float)(1.0 / Math.Sqrt((double)v.x)), (float)(1.0 / Math.Sqrt((double)v.y)), (float)(1.0 / Math.Sqrt((double)v.z)));
        
        /// <summary>
        /// Returns a Vector from the application of InverseSqrt ((float)(1.0 / Math.Sqrt((double)v))).
        /// </summary>
        public static Vector3 InverseSqrt(float v) => new Vector3((float)(1.0 / Math.Sqrt((double)v)));
        
        /// <summary>
        /// Returns a iVector3 from component-wise application of Sign (Math.Sign(v)).
        /// </summary>
        public static iVector3 Sign(Vector3 v) => new iVector3(Math.Sign(v.x), Math.Sign(v.y), Math.Sign(v.z));
        
        /// <summary>
        /// Returns a iVector from the application of Sign (Math.Sign(v)).
        /// </summary>
        public static iVector3 Sign(float v) => new iVector3(Math.Sign(v));
        
        /// <summary>
        /// Returns a Vector3 from component-wise application of Max (Math.Max(lhs, rhs)).
        /// </summary>
        public static Vector3 Max(Vector3 lhs, Vector3 rhs) => new Vector3(Math.Max(lhs.x, rhs.x), Math.Max(lhs.y, rhs.y), Math.Max(lhs.z, rhs.z));
        
        /// <summary>
        /// Returns a Vector3 from component-wise application of Max (Math.Max(lhs, rhs)).
        /// </summary>
        public static Vector3 Max(Vector3 lhs, float rhs) => new Vector3(Math.Max(lhs.x, rhs), Math.Max(lhs.y, rhs), Math.Max(lhs.z, rhs));
        
        /// <summary>
        /// Returns a Vector3 from component-wise application of Max (Math.Max(lhs, rhs)).
        /// </summary>
        public static Vector3 Max(float lhs, Vector3 rhs) => new Vector3(Math.Max(lhs, rhs.x), Math.Max(lhs, rhs.y), Math.Max(lhs, rhs.z));
        
        /// <summary>
        /// Returns a Vector from the application of Max (Math.Max(lhs, rhs)).
        /// </summary>
        public static Vector3 Max(float lhs, float rhs) => new Vector3(Math.Max(lhs, rhs));
        
        /// <summary>
        /// Returns a Vector3 from component-wise application of Min (Math.Min(lhs, rhs)).
        /// </summary>
        public static Vector3 Min(Vector3 lhs, Vector3 rhs) => new Vector3(Math.Min(lhs.x, rhs.x), Math.Min(lhs.y, rhs.y), Math.Min(lhs.z, rhs.z));
        
        /// <summary>
        /// Returns a Vector3 from component-wise application of Min (Math.Min(lhs, rhs)).
        /// </summary>
        public static Vector3 Min(Vector3 lhs, float rhs) => new Vector3(Math.Min(lhs.x, rhs), Math.Min(lhs.y, rhs), Math.Min(lhs.z, rhs));
        
        /// <summary>
        /// Returns a Vector3 from component-wise application of Min (Math.Min(lhs, rhs)).
        /// </summary>
        public static Vector3 Min(float lhs, Vector3 rhs) => new Vector3(Math.Min(lhs, rhs.x), Math.Min(lhs, rhs.y), Math.Min(lhs, rhs.z));
        
        /// <summary>
        /// Returns a Vector from the application of Min (Math.Min(lhs, rhs)).
        /// </summary>
        public static Vector3 Min(float lhs, float rhs) => new Vector3(Math.Min(lhs, rhs));
        
        /// <summary>
        /// Returns a Vector3 from component-wise application of Pow ((float)Math.Pow((double)lhs, (double)rhs)).
        /// </summary>
        public static Vector3 Pow(Vector3 lhs, Vector3 rhs) => new Vector3((float)Math.Pow((double)lhs.x, (double)rhs.x), (float)Math.Pow((double)lhs.y, (double)rhs.y), (float)Math.Pow((double)lhs.z, (double)rhs.z));
        
        /// <summary>
        /// Returns a Vector3 from component-wise application of Pow ((float)Math.Pow((double)lhs, (double)rhs)).
        /// </summary>
        public static Vector3 Pow(Vector3 lhs, float rhs) => new Vector3((float)Math.Pow((double)lhs.x, (double)rhs), (float)Math.Pow((double)lhs.y, (double)rhs), (float)Math.Pow((double)lhs.z, (double)rhs));
        
        /// <summary>
        /// Returns a Vector3 from component-wise application of Pow ((float)Math.Pow((double)lhs, (double)rhs)).
        /// </summary>
        public static Vector3 Pow(float lhs, Vector3 rhs) => new Vector3((float)Math.Pow((double)lhs, (double)rhs.x), (float)Math.Pow((double)lhs, (double)rhs.y), (float)Math.Pow((double)lhs, (double)rhs.z));
        
        /// <summary>
        /// Returns a Vector from the application of Pow ((float)Math.Pow((double)lhs, (double)rhs)).
        /// </summary>
        public static Vector3 Pow(float lhs, float rhs) => new Vector3((float)Math.Pow((double)lhs, (double)rhs));
        
        /// <summary>
        /// Returns a Vector3 from component-wise application of Log ((float)Math.Log((double)lhs, (double)rhs)).
        /// </summary>
        public static Vector3 Log(Vector3 lhs, Vector3 rhs) => new Vector3((float)Math.Log((double)lhs.x, (double)rhs.x), (float)Math.Log((double)lhs.y, (double)rhs.y), (float)Math.Log((double)lhs.z, (double)rhs.z));
        
        /// <summary>
        /// Returns a Vector3 from component-wise application of Log ((float)Math.Log((double)lhs, (double)rhs)).
        /// </summary>
        public static Vector3 Log(Vector3 lhs, float rhs) => new Vector3((float)Math.Log((double)lhs.x, (double)rhs), (float)Math.Log((double)lhs.y, (double)rhs), (float)Math.Log((double)lhs.z, (double)rhs));
        
        /// <summary>
        /// Returns a Vector3 from component-wise application of Log ((float)Math.Log((double)lhs, (double)rhs)).
        /// </summary>
        public static Vector3 Log(float lhs, Vector3 rhs) => new Vector3((float)Math.Log((double)lhs, (double)rhs.x), (float)Math.Log((double)lhs, (double)rhs.y), (float)Math.Log((double)lhs, (double)rhs.z));
        
        /// <summary>
        /// Returns a Vector from the application of Log ((float)Math.Log((double)lhs, (double)rhs)).
        /// </summary>
        public static Vector3 Log(float lhs, float rhs) => new Vector3((float)Math.Log((double)lhs, (double)rhs));
        
        /// <summary>
        /// Returns a Vector3 from component-wise application of Clamp (Math.Min(Math.Max(v, min), max)).
        /// </summary>
        public static Vector3 Clamp(Vector3 v, Vector3 min, Vector3 max) => new Vector3(Math.Min(Math.Max(v.x, min.x), max.x), Math.Min(Math.Max(v.y, min.y), max.y), Math.Min(Math.Max(v.z, min.z), max.z));
        
        /// <summary>
        /// Returns a Vector3 from component-wise application of Clamp (Math.Min(Math.Max(v, min), max)).
        /// </summary>
        public static Vector3 Clamp(Vector3 v, Vector3 min, float max) => new Vector3(Math.Min(Math.Max(v.x, min.x), max), Math.Min(Math.Max(v.y, min.y), max), Math.Min(Math.Max(v.z, min.z), max));
        
        /// <summary>
        /// Returns a Vector3 from component-wise application of Clamp (Math.Min(Math.Max(v, min), max)).
        /// </summary>
        public static Vector3 Clamp(Vector3 v, float min, Vector3 max) => new Vector3(Math.Min(Math.Max(v.x, min), max.x), Math.Min(Math.Max(v.y, min), max.y), Math.Min(Math.Max(v.z, min), max.z));
        
        /// <summary>
        /// Returns a Vector3 from component-wise application of Clamp (Math.Min(Math.Max(v, min), max)).
        /// </summary>
        public static Vector3 Clamp(Vector3 v, float min, float max) => new Vector3(Math.Min(Math.Max(v.x, min), max), Math.Min(Math.Max(v.y, min), max), Math.Min(Math.Max(v.z, min), max));
        
        /// <summary>
        /// Returns a Vector3 from component-wise application of Clamp (Math.Min(Math.Max(v, min), max)).
        /// </summary>
        public static Vector3 Clamp(float v, Vector3 min, Vector3 max) => new Vector3(Math.Min(Math.Max(v, min.x), max.x), Math.Min(Math.Max(v, min.y), max.y), Math.Min(Math.Max(v, min.z), max.z));
        
        /// <summary>
        /// Returns a Vector3 from component-wise application of Clamp (Math.Min(Math.Max(v, min), max)).
        /// </summary>
        public static Vector3 Clamp(float v, Vector3 min, float max) => new Vector3(Math.Min(Math.Max(v, min.x), max), Math.Min(Math.Max(v, min.y), max), Math.Min(Math.Max(v, min.z), max));
        
        /// <summary>
        /// Returns a Vector3 from component-wise application of Clamp (Math.Min(Math.Max(v, min), max)).
        /// </summary>
        public static Vector3 Clamp(float v, float min, Vector3 max) => new Vector3(Math.Min(Math.Max(v, min), max.x), Math.Min(Math.Max(v, min), max.y), Math.Min(Math.Max(v, min), max.z));
        
        /// <summary>
        /// Returns a Vector from the application of Clamp (Math.Min(Math.Max(v, min), max)).
        /// </summary>
        public static Vector3 Clamp(float v, float min, float max) => new Vector3(Math.Min(Math.Max(v, min), max));
        
        /// <summary>
        /// Returns a Vector3 from component-wise application of Mix (min * (1-a) + max * a).
        /// </summary>
        public static Vector3 Mix(Vector3 min, Vector3 max, Vector3 a) => new Vector3(min.x * (1-a.x) + max.x * a.x, min.y * (1-a.y) + max.y * a.y, min.z * (1-a.z) + max.z * a.z);
        
        /// <summary>
        /// Returns a Vector3 from component-wise application of Mix (min * (1-a) + max * a).
        /// </summary>
        public static Vector3 Mix(Vector3 min, Vector3 max, float a) => new Vector3(min.x * (1-a) + max.x * a, min.y * (1-a) + max.y * a, min.z * (1-a) + max.z * a);
        
        /// <summary>
        /// Returns a Vector3 from component-wise application of Mix (min * (1-a) + max * a).
        /// </summary>
        public static Vector3 Mix(Vector3 min, float max, Vector3 a) => new Vector3(min.x * (1-a.x) + max * a.x, min.y * (1-a.y) + max * a.y, min.z * (1-a.z) + max * a.z);
        
        /// <summary>
        /// Returns a Vector3 from component-wise application of Mix (min * (1-a) + max * a).
        /// </summary>
        public static Vector3 Mix(Vector3 min, float max, float a) => new Vector3(min.x * (1-a) + max * a, min.y * (1-a) + max * a, min.z * (1-a) + max * a);
        
        /// <summary>
        /// Returns a Vector3 from component-wise application of Mix (min * (1-a) + max * a).
        /// </summary>
        public static Vector3 Mix(float min, Vector3 max, Vector3 a) => new Vector3(min * (1-a.x) + max.x * a.x, min * (1-a.y) + max.y * a.y, min * (1-a.z) + max.z * a.z);
        
        /// <summary>
        /// Returns a Vector3 from component-wise application of Mix (min * (1-a) + max * a).
        /// </summary>
        public static Vector3 Mix(float min, Vector3 max, float a) => new Vector3(min * (1-a) + max.x * a, min * (1-a) + max.y * a, min * (1-a) + max.z * a);
        
        /// <summary>
        /// Returns a Vector3 from component-wise application of Mix (min * (1-a) + max * a).
        /// </summary>
        public static Vector3 Mix(float min, float max, Vector3 a) => new Vector3(min * (1-a.x) + max * a.x, min * (1-a.y) + max * a.y, min * (1-a.z) + max * a.z);
        
        /// <summary>
        /// Returns a Vector from the application of Mix (min * (1-a) + max * a).
        /// </summary>
        public static Vector3 Mix(float min, float max, float a) => new Vector3(min * (1-a) + max * a);
        
        /// <summary>
        /// Returns a Vector3 from component-wise application of Lerp (min * (1-a) + max * a).
        /// </summary>
        public static Vector3 Lerp(Vector3 min, Vector3 max, Vector3 a) => new Vector3(min.x * (1-a.x) + max.x * a.x, min.y * (1-a.y) + max.y * a.y, min.z * (1-a.z) + max.z * a.z);
        
        /// <summary>
        /// Returns a Vector3 from component-wise application of Lerp (min * (1-a) + max * a).
        /// </summary>
        public static Vector3 Lerp(Vector3 min, Vector3 max, float a) => new Vector3(min.x * (1-a) + max.x * a, min.y * (1-a) + max.y * a, min.z * (1-a) + max.z * a);
        
        /// <summary>
        /// Returns a Vector3 from component-wise application of Lerp (min * (1-a) + max * a).
        /// </summary>
        public static Vector3 Lerp(Vector3 min, float max, Vector3 a) => new Vector3(min.x * (1-a.x) + max * a.x, min.y * (1-a.y) + max * a.y, min.z * (1-a.z) + max * a.z);
        
        /// <summary>
        /// Returns a Vector3 from component-wise application of Lerp (min * (1-a) + max * a).
        /// </summary>
        public static Vector3 Lerp(Vector3 min, float max, float a) => new Vector3(min.x * (1-a) + max * a, min.y * (1-a) + max * a, min.z * (1-a) + max * a);
        
        /// <summary>
        /// Returns a Vector3 from component-wise application of Lerp (min * (1-a) + max * a).
        /// </summary>
        public static Vector3 Lerp(float min, Vector3 max, Vector3 a) => new Vector3(min * (1-a.x) + max.x * a.x, min * (1-a.y) + max.y * a.y, min * (1-a.z) + max.z * a.z);
        
        /// <summary>
        /// Returns a Vector3 from component-wise application of Lerp (min * (1-a) + max * a).
        /// </summary>
        public static Vector3 Lerp(float min, Vector3 max, float a) => new Vector3(min * (1-a) + max.x * a, min * (1-a) + max.y * a, min * (1-a) + max.z * a);
        
        /// <summary>
        /// Returns a Vector3 from component-wise application of Lerp (min * (1-a) + max * a).
        /// </summary>
        public static Vector3 Lerp(float min, float max, Vector3 a) => new Vector3(min * (1-a.x) + max * a.x, min * (1-a.y) + max * a.y, min * (1-a.z) + max * a.z);
        
        /// <summary>
        /// Returns a Vector from the application of Lerp (min * (1-a) + max * a).
        /// </summary>
        public static Vector3 Lerp(float min, float max, float a) => new Vector3(min * (1-a) + max * a);
        
        /// <summary>
        /// Returns a Vector3 from component-wise application of Smoothstep (((v - edge0) / (edge1 - edge0)).Clamp().HermiteInterpolationOrder3()).
        /// </summary>
        public static Vector3 Smoothstep(Vector3 edge0, Vector3 edge1, Vector3 v) => new Vector3(((v.x - edge0.x) / (edge1.x - edge0.x)).Clamp().HermiteInterpolationOrder3(), ((v.y - edge0.y) / (edge1.y - edge0.y)).Clamp().HermiteInterpolationOrder3(), ((v.z - edge0.z) / (edge1.z - edge0.z)).Clamp().HermiteInterpolationOrder3());
        
        /// <summary>
        /// Returns a Vector3 from component-wise application of Smoothstep (((v - edge0) / (edge1 - edge0)).Clamp().HermiteInterpolationOrder3()).
        /// </summary>
        public static Vector3 Smoothstep(Vector3 edge0, Vector3 edge1, float v) => new Vector3(((v - edge0.x) / (edge1.x - edge0.x)).Clamp().HermiteInterpolationOrder3(), ((v - edge0.y) / (edge1.y - edge0.y)).Clamp().HermiteInterpolationOrder3(), ((v - edge0.z) / (edge1.z - edge0.z)).Clamp().HermiteInterpolationOrder3());
        
        /// <summary>
        /// Returns a Vector3 from component-wise application of Smoothstep (((v - edge0) / (edge1 - edge0)).Clamp().HermiteInterpolationOrder3()).
        /// </summary>
        public static Vector3 Smoothstep(Vector3 edge0, float edge1, Vector3 v) => new Vector3(((v.x - edge0.x) / (edge1 - edge0.x)).Clamp().HermiteInterpolationOrder3(), ((v.y - edge0.y) / (edge1 - edge0.y)).Clamp().HermiteInterpolationOrder3(), ((v.z - edge0.z) / (edge1 - edge0.z)).Clamp().HermiteInterpolationOrder3());
        
        /// <summary>
        /// Returns a Vector3 from component-wise application of Smoothstep (((v - edge0) / (edge1 - edge0)).Clamp().HermiteInterpolationOrder3()).
        /// </summary>
        public static Vector3 Smoothstep(Vector3 edge0, float edge1, float v) => new Vector3(((v - edge0.x) / (edge1 - edge0.x)).Clamp().HermiteInterpolationOrder3(), ((v - edge0.y) / (edge1 - edge0.y)).Clamp().HermiteInterpolationOrder3(), ((v - edge0.z) / (edge1 - edge0.z)).Clamp().HermiteInterpolationOrder3());
        
        /// <summary>
        /// Returns a Vector3 from component-wise application of Smoothstep (((v - edge0) / (edge1 - edge0)).Clamp().HermiteInterpolationOrder3()).
        /// </summary>
        public static Vector3 Smoothstep(float edge0, Vector3 edge1, Vector3 v) => new Vector3(((v.x - edge0) / (edge1.x - edge0)).Clamp().HermiteInterpolationOrder3(), ((v.y - edge0) / (edge1.y - edge0)).Clamp().HermiteInterpolationOrder3(), ((v.z - edge0) / (edge1.z - edge0)).Clamp().HermiteInterpolationOrder3());
        
        /// <summary>
        /// Returns a Vector3 from component-wise application of Smoothstep (((v - edge0) / (edge1 - edge0)).Clamp().HermiteInterpolationOrder3()).
        /// </summary>
        public static Vector3 Smoothstep(float edge0, Vector3 edge1, float v) => new Vector3(((v - edge0) / (edge1.x - edge0)).Clamp().HermiteInterpolationOrder3(), ((v - edge0) / (edge1.y - edge0)).Clamp().HermiteInterpolationOrder3(), ((v - edge0) / (edge1.z - edge0)).Clamp().HermiteInterpolationOrder3());
        
        /// <summary>
        /// Returns a Vector3 from component-wise application of Smoothstep (((v - edge0) / (edge1 - edge0)).Clamp().HermiteInterpolationOrder3()).
        /// </summary>
        public static Vector3 Smoothstep(float edge0, float edge1, Vector3 v) => new Vector3(((v.x - edge0) / (edge1 - edge0)).Clamp().HermiteInterpolationOrder3(), ((v.y - edge0) / (edge1 - edge0)).Clamp().HermiteInterpolationOrder3(), ((v.z - edge0) / (edge1 - edge0)).Clamp().HermiteInterpolationOrder3());
        
        /// <summary>
        /// Returns a Vector from the application of Smoothstep (((v - edge0) / (edge1 - edge0)).Clamp().HermiteInterpolationOrder3()).
        /// </summary>
        public static Vector3 Smoothstep(float edge0, float edge1, float v) => new Vector3(((v - edge0) / (edge1 - edge0)).Clamp().HermiteInterpolationOrder3());
        
        /// <summary>
        /// Returns a Vector3 from component-wise application of Smootherstep (((v - edge0) / (edge1 - edge0)).Clamp().HermiteInterpolationOrder5()).
        /// </summary>
        public static Vector3 Smootherstep(Vector3 edge0, Vector3 edge1, Vector3 v) => new Vector3(((v.x - edge0.x) / (edge1.x - edge0.x)).Clamp().HermiteInterpolationOrder5(), ((v.y - edge0.y) / (edge1.y - edge0.y)).Clamp().HermiteInterpolationOrder5(), ((v.z - edge0.z) / (edge1.z - edge0.z)).Clamp().HermiteInterpolationOrder5());
        
        /// <summary>
        /// Returns a Vector3 from component-wise application of Smootherstep (((v - edge0) / (edge1 - edge0)).Clamp().HermiteInterpolationOrder5()).
        /// </summary>
        public static Vector3 Smootherstep(Vector3 edge0, Vector3 edge1, float v) => new Vector3(((v - edge0.x) / (edge1.x - edge0.x)).Clamp().HermiteInterpolationOrder5(), ((v - edge0.y) / (edge1.y - edge0.y)).Clamp().HermiteInterpolationOrder5(), ((v - edge0.z) / (edge1.z - edge0.z)).Clamp().HermiteInterpolationOrder5());
        
        /// <summary>
        /// Returns a Vector3 from component-wise application of Smootherstep (((v - edge0) / (edge1 - edge0)).Clamp().HermiteInterpolationOrder5()).
        /// </summary>
        public static Vector3 Smootherstep(Vector3 edge0, float edge1, Vector3 v) => new Vector3(((v.x - edge0.x) / (edge1 - edge0.x)).Clamp().HermiteInterpolationOrder5(), ((v.y - edge0.y) / (edge1 - edge0.y)).Clamp().HermiteInterpolationOrder5(), ((v.z - edge0.z) / (edge1 - edge0.z)).Clamp().HermiteInterpolationOrder5());
        
        /// <summary>
        /// Returns a Vector3 from component-wise application of Smootherstep (((v - edge0) / (edge1 - edge0)).Clamp().HermiteInterpolationOrder5()).
        /// </summary>
        public static Vector3 Smootherstep(Vector3 edge0, float edge1, float v) => new Vector3(((v - edge0.x) / (edge1 - edge0.x)).Clamp().HermiteInterpolationOrder5(), ((v - edge0.y) / (edge1 - edge0.y)).Clamp().HermiteInterpolationOrder5(), ((v - edge0.z) / (edge1 - edge0.z)).Clamp().HermiteInterpolationOrder5());
        
        /// <summary>
        /// Returns a Vector3 from component-wise application of Smootherstep (((v - edge0) / (edge1 - edge0)).Clamp().HermiteInterpolationOrder5()).
        /// </summary>
        public static Vector3 Smootherstep(float edge0, Vector3 edge1, Vector3 v) => new Vector3(((v.x - edge0) / (edge1.x - edge0)).Clamp().HermiteInterpolationOrder5(), ((v.y - edge0) / (edge1.y - edge0)).Clamp().HermiteInterpolationOrder5(), ((v.z - edge0) / (edge1.z - edge0)).Clamp().HermiteInterpolationOrder5());
        
        /// <summary>
        /// Returns a Vector3 from component-wise application of Smootherstep (((v - edge0) / (edge1 - edge0)).Clamp().HermiteInterpolationOrder5()).
        /// </summary>
        public static Vector3 Smootherstep(float edge0, Vector3 edge1, float v) => new Vector3(((v - edge0) / (edge1.x - edge0)).Clamp().HermiteInterpolationOrder5(), ((v - edge0) / (edge1.y - edge0)).Clamp().HermiteInterpolationOrder5(), ((v - edge0) / (edge1.z - edge0)).Clamp().HermiteInterpolationOrder5());
        
        /// <summary>
        /// Returns a Vector3 from component-wise application of Smootherstep (((v - edge0) / (edge1 - edge0)).Clamp().HermiteInterpolationOrder5()).
        /// </summary>
        public static Vector3 Smootherstep(float edge0, float edge1, Vector3 v) => new Vector3(((v.x - edge0) / (edge1 - edge0)).Clamp().HermiteInterpolationOrder5(), ((v.y - edge0) / (edge1 - edge0)).Clamp().HermiteInterpolationOrder5(), ((v.z - edge0) / (edge1 - edge0)).Clamp().HermiteInterpolationOrder5());
        
        /// <summary>
        /// Returns a Vector from the application of Smootherstep (((v - edge0) / (edge1 - edge0)).Clamp().HermiteInterpolationOrder5()).
        /// </summary>
        public static Vector3 Smootherstep(float edge0, float edge1, float v) => new Vector3(((v - edge0) / (edge1 - edge0)).Clamp().HermiteInterpolationOrder5());
        
        /// <summary>
        /// Returns a Vector3 from component-wise application of Fma (a * b + c).
        /// </summary>
        public static Vector3 Fma(Vector3 a, Vector3 b, Vector3 c) => new Vector3(a.x * b.x + c.x, a.y * b.y + c.y, a.z * b.z + c.z);
        
        /// <summary>
        /// Returns a Vector3 from component-wise application of Fma (a * b + c).
        /// </summary>
        public static Vector3 Fma(Vector3 a, Vector3 b, float c) => new Vector3(a.x * b.x + c, a.y * b.y + c, a.z * b.z + c);
        
        /// <summary>
        /// Returns a Vector3 from component-wise application of Fma (a * b + c).
        /// </summary>
        public static Vector3 Fma(Vector3 a, float b, Vector3 c) => new Vector3(a.x * b + c.x, a.y * b + c.y, a.z * b + c.z);
        
        /// <summary>
        /// Returns a Vector3 from component-wise application of Fma (a * b + c).
        /// </summary>
        public static Vector3 Fma(Vector3 a, float b, float c) => new Vector3(a.x * b + c, a.y * b + c, a.z * b + c);
        
        /// <summary>
        /// Returns a Vector3 from component-wise application of Fma (a * b + c).
        /// </summary>
        public static Vector3 Fma(float a, Vector3 b, Vector3 c) => new Vector3(a * b.x + c.x, a * b.y + c.y, a * b.z + c.z);
        
        /// <summary>
        /// Returns a Vector3 from component-wise application of Fma (a * b + c).
        /// </summary>
        public static Vector3 Fma(float a, Vector3 b, float c) => new Vector3(a * b.x + c, a * b.y + c, a * b.z + c);
        
        /// <summary>
        /// Returns a Vector3 from component-wise application of Fma (a * b + c).
        /// </summary>
        public static Vector3 Fma(float a, float b, Vector3 c) => new Vector3(a * b + c.x, a * b + c.y, a * b + c.z);
        
        /// <summary>
        /// Returns a Vector from the application of Fma (a * b + c).
        /// </summary>
        public static Vector3 Fma(float a, float b, float c) => new Vector3(a * b + c);
        
        /// <summary>
        /// Returns a Vector3 from component-wise application of Add (lhs + rhs).
        /// </summary>
        public static Vector3 Add(Vector3 lhs, Vector3 rhs) => new Vector3(lhs.x + rhs.x, lhs.y + rhs.y, lhs.z + rhs.z);
        
        /// <summary>
        /// Returns a Vector3 from component-wise application of Add (lhs + rhs).
        /// </summary>
        public static Vector3 Add(Vector3 lhs, float rhs) => new Vector3(lhs.x + rhs, lhs.y + rhs, lhs.z + rhs);
        
        /// <summary>
        /// Returns a Vector3 from component-wise application of Add (lhs + rhs).
        /// </summary>
        public static Vector3 Add(float lhs, Vector3 rhs) => new Vector3(lhs + rhs.x, lhs + rhs.y, lhs + rhs.z);
        
        /// <summary>
        /// Returns a Vector from the application of Add (lhs + rhs).
        /// </summary>
        public static Vector3 Add(float lhs, float rhs) => new Vector3(lhs + rhs);
        
        /// <summary>
        /// Returns a Vector3 from component-wise application of Sub (lhs - rhs).
        /// </summary>
        public static Vector3 Sub(Vector3 lhs, Vector3 rhs) => new Vector3(lhs.x - rhs.x, lhs.y - rhs.y, lhs.z - rhs.z);
        
        /// <summary>
        /// Returns a Vector3 from component-wise application of Sub (lhs - rhs).
        /// </summary>
        public static Vector3 Sub(Vector3 lhs, float rhs) => new Vector3(lhs.x - rhs, lhs.y - rhs, lhs.z - rhs);
        
        /// <summary>
        /// Returns a Vector3 from component-wise application of Sub (lhs - rhs).
        /// </summary>
        public static Vector3 Sub(float lhs, Vector3 rhs) => new Vector3(lhs - rhs.x, lhs - rhs.y, lhs - rhs.z);
        
        /// <summary>
        /// Returns a Vector from the application of Sub (lhs - rhs).
        /// </summary>
        public static Vector3 Sub(float lhs, float rhs) => new Vector3(lhs - rhs);
        
        /// <summary>
        /// Returns a Vector3 from component-wise application of Mul (lhs * rhs).
        /// </summary>
        public static Vector3 Mul(Vector3 lhs, Vector3 rhs) => new Vector3(lhs.x * rhs.x, lhs.y * rhs.y, lhs.z * rhs.z);
        
        /// <summary>
        /// Returns a Vector3 from component-wise application of Mul (lhs * rhs).
        /// </summary>
        public static Vector3 Mul(Vector3 lhs, float rhs) => new Vector3(lhs.x * rhs, lhs.y * rhs, lhs.z * rhs);
        
        /// <summary>
        /// Returns a Vector3 from component-wise application of Mul (lhs * rhs).
        /// </summary>
        public static Vector3 Mul(float lhs, Vector3 rhs) => new Vector3(lhs * rhs.x, lhs * rhs.y, lhs * rhs.z);
        
        /// <summary>
        /// Returns a Vector from the application of Mul (lhs * rhs).
        /// </summary>
        public static Vector3 Mul(float lhs, float rhs) => new Vector3(lhs * rhs);
        
        /// <summary>
        /// Returns a Vector3 from component-wise application of Div (lhs / rhs).
        /// </summary>
        public static Vector3 Div(Vector3 lhs, Vector3 rhs) => new Vector3(lhs.x / rhs.x, lhs.y / rhs.y, lhs.z / rhs.z);
        
        /// <summary>
        /// Returns a Vector3 from component-wise application of Div (lhs / rhs).
        /// </summary>
        public static Vector3 Div(Vector3 lhs, float rhs) => new Vector3(lhs.x / rhs, lhs.y / rhs, lhs.z / rhs);
        
        /// <summary>
        /// Returns a Vector3 from component-wise application of Div (lhs / rhs).
        /// </summary>
        public static Vector3 Div(float lhs, Vector3 rhs) => new Vector3(lhs / rhs.x, lhs / rhs.y, lhs / rhs.z);
        
        /// <summary>
        /// Returns a Vector from the application of Div (lhs / rhs).
        /// </summary>
        public static Vector3 Div(float lhs, float rhs) => new Vector3(lhs / rhs);
        
        /// <summary>
        /// Returns a Vector3 from component-wise application of Modulo (lhs % rhs).
        /// </summary>
        public static Vector3 Modulo(Vector3 lhs, Vector3 rhs) => new Vector3(lhs.x % rhs.x, lhs.y % rhs.y, lhs.z % rhs.z);
        
        /// <summary>
        /// Returns a Vector3 from component-wise application of Modulo (lhs % rhs).
        /// </summary>
        public static Vector3 Modulo(Vector3 lhs, float rhs) => new Vector3(lhs.x % rhs, lhs.y % rhs, lhs.z % rhs);
        
        /// <summary>
        /// Returns a Vector3 from component-wise application of Modulo (lhs % rhs).
        /// </summary>
        public static Vector3 Modulo(float lhs, Vector3 rhs) => new Vector3(lhs % rhs.x, lhs % rhs.y, lhs % rhs.z);
        
        /// <summary>
        /// Returns a Vector from the application of Modulo (lhs % rhs).
        /// </summary>
        public static Vector3 Modulo(float lhs, float rhs) => new Vector3(lhs % rhs);
        
        /// <summary>
        /// Returns a Vector3 from component-wise application of Degrees (Radians-To-Degrees Conversion).
        /// </summary>
        public static Vector3 Degrees(Vector3 v) => new Vector3((float)(v.x * 57.295779513082320876798154814105170332405472466564321f), (float)(v.y * 57.295779513082320876798154814105170332405472466564321f), (float)(v.z * 57.295779513082320876798154814105170332405472466564321f));
        
        /// <summary>
        /// Returns a Vector from the application of Degrees (Radians-To-Degrees Conversion).
        /// </summary>
        public static Vector3 Degrees(float v) => new Vector3((float)(v * 57.295779513082320876798154814105170332405472466564321f));
        
        /// <summary>
        /// Returns a Vector3 from component-wise application of Radians (Degrees-To-Radians Conversion).
        /// </summary>
        public static Vector3 Radians(Vector3 v) => new Vector3((float)(v.x * 0.0174532925199432957692369076848861271344287188854172f), (float)(v.y * 0.0174532925199432957692369076848861271344287188854172f), (float)(v.z * 0.0174532925199432957692369076848861271344287188854172f));
        
        /// <summary>
        /// Returns a Vector from the application of Radians (Degrees-To-Radians Conversion).
        /// </summary>
        public static Vector3 Radians(float v) => new Vector3((float)(v * 0.0174532925199432957692369076848861271344287188854172f));
        
        /// <summary>
        /// Returns a Vector3 from component-wise application of Acos ((float)Math.Acos((double)v)).
        /// </summary>
        public static Vector3 Acos(Vector3 v) => new Vector3((float)Math.Acos((double)v.x), (float)Math.Acos((double)v.y), (float)Math.Acos((double)v.z));
        
        /// <summary>
        /// Returns a Vector from the application of Acos ((float)Math.Acos((double)v)).
        /// </summary>
        public static Vector3 Acos(float v) => new Vector3((float)Math.Acos((double)v));
        
        /// <summary>
        /// Returns a Vector3 from component-wise application of Asin ((float)Math.Asin((double)v)).
        /// </summary>
        public static Vector3 Asin(Vector3 v) => new Vector3((float)Math.Asin((double)v.x), (float)Math.Asin((double)v.y), (float)Math.Asin((double)v.z));
        
        /// <summary>
        /// Returns a Vector from the application of Asin ((float)Math.Asin((double)v)).
        /// </summary>
        public static Vector3 Asin(float v) => new Vector3((float)Math.Asin((double)v));
        
        /// <summary>
        /// Returns a Vector3 from component-wise application of Atan ((float)Math.Atan((double)v)).
        /// </summary>
        public static Vector3 Atan(Vector3 v) => new Vector3((float)Math.Atan((double)v.x), (float)Math.Atan((double)v.y), (float)Math.Atan((double)v.z));
        
        /// <summary>
        /// Returns a Vector from the application of Atan ((float)Math.Atan((double)v)).
        /// </summary>
        public static Vector3 Atan(float v) => new Vector3((float)Math.Atan((double)v));
        
        /// <summary>
        /// Returns a Vector3 from component-wise application of Cos ((float)Math.Cos((double)v)).
        /// </summary>
        public static Vector3 Cos(Vector3 v) => new Vector3((float)Math.Cos((double)v.x), (float)Math.Cos((double)v.y), (float)Math.Cos((double)v.z));
        
        /// <summary>
        /// Returns a Vector from the application of Cos ((float)Math.Cos((double)v)).
        /// </summary>
        public static Vector3 Cos(float v) => new Vector3((float)Math.Cos((double)v));
        
        /// <summary>
        /// Returns a Vector3 from component-wise application of Cosh ((float)Math.Cosh((double)v)).
        /// </summary>
        public static Vector3 Cosh(Vector3 v) => new Vector3((float)Math.Cosh((double)v.x), (float)Math.Cosh((double)v.y), (float)Math.Cosh((double)v.z));
        
        /// <summary>
        /// Returns a Vector from the application of Cosh ((float)Math.Cosh((double)v)).
        /// </summary>
        public static Vector3 Cosh(float v) => new Vector3((float)Math.Cosh((double)v));
        
        /// <summary>
        /// Returns a Vector3 from component-wise application of Exp ((float)Math.Exp((double)v)).
        /// </summary>
        public static Vector3 Exp(Vector3 v) => new Vector3((float)Math.Exp((double)v.x), (float)Math.Exp((double)v.y), (float)Math.Exp((double)v.z));
        
        /// <summary>
        /// Returns a Vector from the application of Exp ((float)Math.Exp((double)v)).
        /// </summary>
        public static Vector3 Exp(float v) => new Vector3((float)Math.Exp((double)v));
        
        /// <summary>
        /// Returns a Vector3 from component-wise application of Log ((float)Math.Log((double)v)).
        /// </summary>
        public static Vector3 Log(Vector3 v) => new Vector3((float)Math.Log((double)v.x), (float)Math.Log((double)v.y), (float)Math.Log((double)v.z));
        
        /// <summary>
        /// Returns a Vector from the application of Log ((float)Math.Log((double)v)).
        /// </summary>
        public static Vector3 Log(float v) => new Vector3((float)Math.Log((double)v));
        
        /// <summary>
        /// Returns a Vector3 from component-wise application of Log2 ((float)Math.Log((double)v, 2)).
        /// </summary>
        public static Vector3 Log2(Vector3 v) => new Vector3((float)Math.Log((double)v.x, 2), (float)Math.Log((double)v.y, 2), (float)Math.Log((double)v.z, 2));
        
        /// <summary>
        /// Returns a Vector from the application of Log2 ((float)Math.Log((double)v, 2)).
        /// </summary>
        public static Vector3 Log2(float v) => new Vector3((float)Math.Log((double)v, 2));
        
        /// <summary>
        /// Returns a Vector3 from component-wise application of Log10 ((float)Math.Log10((double)v)).
        /// </summary>
        public static Vector3 Log10(Vector3 v) => new Vector3((float)Math.Log10((double)v.x), (float)Math.Log10((double)v.y), (float)Math.Log10((double)v.z));
        
        /// <summary>
        /// Returns a Vector from the application of Log10 ((float)Math.Log10((double)v)).
        /// </summary>
        public static Vector3 Log10(float v) => new Vector3((float)Math.Log10((double)v));
        
        /// <summary>
        /// Returns a Vector3 from component-wise application of Floor ((float)Math.Floor(v)).
        /// </summary>
        public static Vector3 Floor(Vector3 v) => new Vector3((float)Math.Floor(v.x), (float)Math.Floor(v.y), (float)Math.Floor(v.z));
        
        /// <summary>
        /// Returns a Vector from the application of Floor ((float)Math.Floor(v)).
        /// </summary>
        public static Vector3 Floor(float v) => new Vector3((float)Math.Floor(v));
        
        /// <summary>
        /// Returns a Vector3 from component-wise application of Ceiling ((float)Math.Ceiling(v)).
        /// </summary>
        public static Vector3 Ceiling(Vector3 v) => new Vector3((float)Math.Ceiling(v.x), (float)Math.Ceiling(v.y), (float)Math.Ceiling(v.z));
        
        /// <summary>
        /// Returns a Vector from the application of Ceiling ((float)Math.Ceiling(v)).
        /// </summary>
        public static Vector3 Ceiling(float v) => new Vector3((float)Math.Ceiling(v));
        
        /// <summary>
        /// Returns a Vector3 from component-wise application of Round ((float)Math.Round(v)).
        /// </summary>
        public static Vector3 Round(Vector3 v) => new Vector3((float)Math.Round(v.x), (float)Math.Round(v.y), (float)Math.Round(v.z));
        
        /// <summary>
        /// Returns a Vector from the application of Round ((float)Math.Round(v)).
        /// </summary>
        public static Vector3 Round(float v) => new Vector3((float)Math.Round(v));
        
        /// <summary>
        /// Returns a Vector3 from component-wise application of Sin ((float)Math.Sin((double)v)).
        /// </summary>
        public static Vector3 Sin(Vector3 v) => new Vector3((float)Math.Sin((double)v.x), (float)Math.Sin((double)v.y), (float)Math.Sin((double)v.z));
        
        /// <summary>
        /// Returns a Vector from the application of Sin ((float)Math.Sin((double)v)).
        /// </summary>
        public static Vector3 Sin(float v) => new Vector3((float)Math.Sin((double)v));
        
        /// <summary>
        /// Returns a Vector3 from component-wise application of Sinh ((float)Math.Sinh((double)v)).
        /// </summary>
        public static Vector3 Sinh(Vector3 v) => new Vector3((float)Math.Sinh((double)v.x), (float)Math.Sinh((double)v.y), (float)Math.Sinh((double)v.z));
        
        /// <summary>
        /// Returns a Vector from the application of Sinh ((float)Math.Sinh((double)v)).
        /// </summary>
        public static Vector3 Sinh(float v) => new Vector3((float)Math.Sinh((double)v));
        
        /// <summary>
        /// Returns a Vector3 from component-wise application of Tan ((float)Math.Tan((double)v)).
        /// </summary>
        public static Vector3 Tan(Vector3 v) => new Vector3((float)Math.Tan((double)v.x), (float)Math.Tan((double)v.y), (float)Math.Tan((double)v.z));
        
        /// <summary>
        /// Returns a Vector from the application of Tan ((float)Math.Tan((double)v)).
        /// </summary>
        public static Vector3 Tan(float v) => new Vector3((float)Math.Tan((double)v));
        
        /// <summary>
        /// Returns a Vector3 from component-wise application of Tanh ((float)Math.Tanh((double)v)).
        /// </summary>
        public static Vector3 Tanh(Vector3 v) => new Vector3((float)Math.Tanh((double)v.x), (float)Math.Tanh((double)v.y), (float)Math.Tanh((double)v.z));
        
        /// <summary>
        /// Returns a Vector from the application of Tanh ((float)Math.Tanh((double)v)).
        /// </summary>
        public static Vector3 Tanh(float v) => new Vector3((float)Math.Tanh((double)v));
        
        /// <summary>
        /// Returns a Vector3 from component-wise application of Truncate ((float)Math.Truncate((double)v)).
        /// </summary>
        public static Vector3 Truncate(Vector3 v) => new Vector3((float)Math.Truncate((double)v.x), (float)Math.Truncate((double)v.y), (float)Math.Truncate((double)v.z));
        
        /// <summary>
        /// Returns a Vector from the application of Truncate ((float)Math.Truncate((double)v)).
        /// </summary>
        public static Vector3 Truncate(float v) => new Vector3((float)Math.Truncate((double)v));
        
        /// <summary>
        /// Returns a Vector3 from component-wise application of Fract ((float)(v - Math.Floor(v))).
        /// </summary>
        public static Vector3 Fract(Vector3 v) => new Vector3((float)(v.x - Math.Floor(v.x)), (float)(v.y - Math.Floor(v.y)), (float)(v.z - Math.Floor(v.z)));
        
        /// <summary>
        /// Returns a Vector from the application of Fract ((float)(v - Math.Floor(v))).
        /// </summary>
        public static Vector3 Fract(float v) => new Vector3((float)(v - Math.Floor(v)));
        
        /// <summary>
        /// Returns a Vector3 from component-wise application of Trunc ((long)(v)).
        /// </summary>
        public static Vector3 Trunc(Vector3 v) => new Vector3((long)(v.x), (long)(v.y), (long)(v.z));
        
        /// <summary>
        /// Returns a Vector from the application of Trunc ((long)(v)).
        /// </summary>
        public static Vector3 Trunc(float v) => new Vector3((long)(v));
        
        /// <summary>
        /// Returns a Vector3 with independent and identically distributed uniform values between 'minValue' and 'maxValue'.
        /// </summary>
        public static Vector3 Random(System.Random random, Vector3 minValue, Vector3 maxValue) => new Vector3((float)random.NextDouble() * (maxValue.x - minValue.x) + minValue.x, (float)random.NextDouble() * (maxValue.y - minValue.y) + minValue.y, (float)random.NextDouble() * (maxValue.z - minValue.z) + minValue.z);
        
        /// <summary>
        /// Returns a Vector3 with independent and identically distributed uniform values between 'minValue' and 'maxValue'.
        /// </summary>
        public static Vector3 Random(System.Random random, Vector3 minValue, float maxValue) => new Vector3((float)random.NextDouble() * (maxValue - minValue.x) + minValue.x, (float)random.NextDouble() * (maxValue - minValue.y) + minValue.y, (float)random.NextDouble() * (maxValue - minValue.z) + minValue.z);
        
        /// <summary>
        /// Returns a Vector3 with independent and identically distributed uniform values between 'minValue' and 'maxValue'.
        /// </summary>
        public static Vector3 Random(System.Random random, float minValue, Vector3 maxValue) => new Vector3((float)random.NextDouble() * (maxValue.x - minValue) + minValue, (float)random.NextDouble() * (maxValue.y - minValue) + minValue, (float)random.NextDouble() * (maxValue.z - minValue) + minValue);
        
        /// <summary>
        /// Returns a Vector3 with independent and identically distributed uniform values between 'minValue' and 'maxValue'.
        /// </summary>
        public static Vector3 Random(System.Random random, float minValue, float maxValue) => new Vector3((float)random.NextDouble() * (maxValue - minValue) + minValue);
        
        /// <summary>
        /// Returns a Vector3 with independent and identically distributed uniform values between 'minValue' and 'maxValue'.
        /// </summary>
        public static Vector3 RandomUniform(System.Random random, Vector3 minValue, Vector3 maxValue) => new Vector3((float)random.NextDouble() * (maxValue.x - minValue.x) + minValue.x, (float)random.NextDouble() * (maxValue.y - minValue.y) + minValue.y, (float)random.NextDouble() * (maxValue.z - minValue.z) + minValue.z);
        
        /// <summary>
        /// Returns a Vector3 with independent and identically distributed uniform values between 'minValue' and 'maxValue'.
        /// </summary>
        public static Vector3 RandomUniform(System.Random random, Vector3 minValue, float maxValue) => new Vector3((float)random.NextDouble() * (maxValue - minValue.x) + minValue.x, (float)random.NextDouble() * (maxValue - minValue.y) + minValue.y, (float)random.NextDouble() * (maxValue - minValue.z) + minValue.z);
        
        /// <summary>
        /// Returns a Vector3 with independent and identically distributed uniform values between 'minValue' and 'maxValue'.
        /// </summary>
        public static Vector3 RandomUniform(System.Random random, float minValue, Vector3 maxValue) => new Vector3((float)random.NextDouble() * (maxValue.x - minValue) + minValue, (float)random.NextDouble() * (maxValue.y - minValue) + minValue, (float)random.NextDouble() * (maxValue.z - minValue) + minValue);
        
        /// <summary>
        /// Returns a Vector3 with independent and identically distributed uniform values between 'minValue' and 'maxValue'.
        /// </summary>
        public static Vector3 RandomUniform(System.Random random, float minValue, float maxValue) => new Vector3((float)random.NextDouble() * (maxValue - minValue) + minValue);
        
        /// <summary>
        /// Returns a Vector3 with independent and identically distributed values according to a normal/Gaussian distribution with specified mean and variance.
        /// </summary>
        public static Vector3 RandomNormal(System.Random random, Vector3 mean, Vector3 variance) => new Vector3((float)(Math.Sqrt((double)variance.x) * Math.Cos(2 * Math.PI * random.NextDouble()) * Math.Sqrt(-2.0 * Math.Log(random.NextDouble()))) + mean.x, (float)(Math.Sqrt((double)variance.y) * Math.Cos(2 * Math.PI * random.NextDouble()) * Math.Sqrt(-2.0 * Math.Log(random.NextDouble()))) + mean.y, (float)(Math.Sqrt((double)variance.z) * Math.Cos(2 * Math.PI * random.NextDouble()) * Math.Sqrt(-2.0 * Math.Log(random.NextDouble()))) + mean.z);
        
        /// <summary>
        /// Returns a Vector3 with independent and identically distributed values according to a normal/Gaussian distribution with specified mean and variance.
        /// </summary>
        public static Vector3 RandomNormal(System.Random random, Vector3 mean, float variance) => new Vector3((float)(Math.Sqrt((double)variance) * Math.Cos(2 * Math.PI * random.NextDouble()) * Math.Sqrt(-2.0 * Math.Log(random.NextDouble()))) + mean.x, (float)(Math.Sqrt((double)variance) * Math.Cos(2 * Math.PI * random.NextDouble()) * Math.Sqrt(-2.0 * Math.Log(random.NextDouble()))) + mean.y, (float)(Math.Sqrt((double)variance) * Math.Cos(2 * Math.PI * random.NextDouble()) * Math.Sqrt(-2.0 * Math.Log(random.NextDouble()))) + mean.z);
        
        /// <summary>
        /// Returns a Vector3 with independent and identically distributed values according to a normal/Gaussian distribution with specified mean and variance.
        /// </summary>
        public static Vector3 RandomNormal(System.Random random, float mean, Vector3 variance) => new Vector3((float)(Math.Sqrt((double)variance.x) * Math.Cos(2 * Math.PI * random.NextDouble()) * Math.Sqrt(-2.0 * Math.Log(random.NextDouble()))) + mean, (float)(Math.Sqrt((double)variance.y) * Math.Cos(2 * Math.PI * random.NextDouble()) * Math.Sqrt(-2.0 * Math.Log(random.NextDouble()))) + mean, (float)(Math.Sqrt((double)variance.z) * Math.Cos(2 * Math.PI * random.NextDouble()) * Math.Sqrt(-2.0 * Math.Log(random.NextDouble()))) + mean);
        
        /// <summary>
        /// Returns a Vector3 with independent and identically distributed values according to a normal/Gaussian distribution with specified mean and variance.
        /// </summary>
        public static Vector3 RandomNormal(System.Random random, float mean, float variance) => new Vector3((float)(Math.Sqrt((double)variance) * Math.Cos(2 * Math.PI * random.NextDouble()) * Math.Sqrt(-2.0 * Math.Log(random.NextDouble()))) + mean);
        
        /// <summary>
        /// Returns a Vector3 with independent and identically distributed values according to a normal/Gaussian distribution with specified mean and variance.
        /// </summary>
        public static Vector3 RandomGaussian(System.Random random, Vector3 mean, Vector3 variance) => new Vector3((float)(Math.Sqrt((double)variance.x) * Math.Cos(2 * Math.PI * random.NextDouble()) * Math.Sqrt(-2.0 * Math.Log(random.NextDouble()))) + mean.x, (float)(Math.Sqrt((double)variance.y) * Math.Cos(2 * Math.PI * random.NextDouble()) * Math.Sqrt(-2.0 * Math.Log(random.NextDouble()))) + mean.y, (float)(Math.Sqrt((double)variance.z) * Math.Cos(2 * Math.PI * random.NextDouble()) * Math.Sqrt(-2.0 * Math.Log(random.NextDouble()))) + mean.z);
        
        /// <summary>
        /// Returns a Vector3 with independent and identically distributed values according to a normal/Gaussian distribution with specified mean and variance.
        /// </summary>
        public static Vector3 RandomGaussian(System.Random random, Vector3 mean, float variance) => new Vector3((float)(Math.Sqrt((double)variance) * Math.Cos(2 * Math.PI * random.NextDouble()) * Math.Sqrt(-2.0 * Math.Log(random.NextDouble()))) + mean.x, (float)(Math.Sqrt((double)variance) * Math.Cos(2 * Math.PI * random.NextDouble()) * Math.Sqrt(-2.0 * Math.Log(random.NextDouble()))) + mean.y, (float)(Math.Sqrt((double)variance) * Math.Cos(2 * Math.PI * random.NextDouble()) * Math.Sqrt(-2.0 * Math.Log(random.NextDouble()))) + mean.z);
        
        /// <summary>
        /// Returns a Vector3 with independent and identically distributed values according to a normal/Gaussian distribution with specified mean and variance.
        /// </summary>
        public static Vector3 RandomGaussian(System.Random random, float mean, Vector3 variance) => new Vector3((float)(Math.Sqrt((double)variance.x) * Math.Cos(2 * Math.PI * random.NextDouble()) * Math.Sqrt(-2.0 * Math.Log(random.NextDouble()))) + mean, (float)(Math.Sqrt((double)variance.y) * Math.Cos(2 * Math.PI * random.NextDouble()) * Math.Sqrt(-2.0 * Math.Log(random.NextDouble()))) + mean, (float)(Math.Sqrt((double)variance.z) * Math.Cos(2 * Math.PI * random.NextDouble()) * Math.Sqrt(-2.0 * Math.Log(random.NextDouble()))) + mean);
        
        /// <summary>
        /// Returns a Vector3 with independent and identically distributed values according to a normal/Gaussian distribution with specified mean and variance.
        /// </summary>
        public static Vector3 RandomGaussian(System.Random random, float mean, float variance) => new Vector3((float)(Math.Sqrt((double)variance) * Math.Cos(2 * Math.PI * random.NextDouble()) * Math.Sqrt(-2.0 * Math.Log(random.NextDouble()))) + mean);

        #endregion


        #region Component-Wise Operator Overloads
        
        /// <summary>
        /// Returns a bVector3 from component-wise application of operator&lt; (lhs &lt; rhs).
        /// </summary>
        public static bVector3 operator<(Vector3 lhs, Vector3 rhs) => new bVector3(lhs.x < rhs.x, lhs.y < rhs.y, lhs.z < rhs.z);
        
        /// <summary>
        /// Returns a bVector3 from component-wise application of operator&lt; (lhs &lt; rhs).
        /// </summary>
        public static bVector3 operator<(Vector3 lhs, float rhs) => new bVector3(lhs.x < rhs, lhs.y < rhs, lhs.z < rhs);
        
        /// <summary>
        /// Returns a bVector3 from component-wise application of operator&lt; (lhs &lt; rhs).
        /// </summary>
        public static bVector3 operator<(float lhs, Vector3 rhs) => new bVector3(lhs < rhs.x, lhs < rhs.y, lhs < rhs.z);
        
        /// <summary>
        /// Returns a bVector3 from component-wise application of operator&lt;= (lhs &lt;= rhs).
        /// </summary>
        public static bVector3 operator<=(Vector3 lhs, Vector3 rhs) => new bVector3(lhs.x <= rhs.x, lhs.y <= rhs.y, lhs.z <= rhs.z);
        
        /// <summary>
        /// Returns a bVector3 from component-wise application of operator&lt;= (lhs &lt;= rhs).
        /// </summary>
        public static bVector3 operator<=(Vector3 lhs, float rhs) => new bVector3(lhs.x <= rhs, lhs.y <= rhs, lhs.z <= rhs);
        
        /// <summary>
        /// Returns a bVector3 from component-wise application of operator&lt;= (lhs &lt;= rhs).
        /// </summary>
        public static bVector3 operator<=(float lhs, Vector3 rhs) => new bVector3(lhs <= rhs.x, lhs <= rhs.y, lhs <= rhs.z);
        
        /// <summary>
        /// Returns a bVector3 from component-wise application of operator&gt; (lhs &gt; rhs).
        /// </summary>
        public static bVector3 operator>(Vector3 lhs, Vector3 rhs) => new bVector3(lhs.x > rhs.x, lhs.y > rhs.y, lhs.z > rhs.z);
        
        /// <summary>
        /// Returns a bVector3 from component-wise application of operator&gt; (lhs &gt; rhs).
        /// </summary>
        public static bVector3 operator>(Vector3 lhs, float rhs) => new bVector3(lhs.x > rhs, lhs.y > rhs, lhs.z > rhs);
        
        /// <summary>
        /// Returns a bVector3 from component-wise application of operator&gt; (lhs &gt; rhs).
        /// </summary>
        public static bVector3 operator>(float lhs, Vector3 rhs) => new bVector3(lhs > rhs.x, lhs > rhs.y, lhs > rhs.z);
        
        /// <summary>
        /// Returns a bVector3 from component-wise application of operator&gt;= (lhs &gt;= rhs).
        /// </summary>
        public static bVector3 operator>=(Vector3 lhs, Vector3 rhs) => new bVector3(lhs.x >= rhs.x, lhs.y >= rhs.y, lhs.z >= rhs.z);
        
        /// <summary>
        /// Returns a bVector3 from component-wise application of operator&gt;= (lhs &gt;= rhs).
        /// </summary>
        public static bVector3 operator>=(Vector3 lhs, float rhs) => new bVector3(lhs.x >= rhs, lhs.y >= rhs, lhs.z >= rhs);
        
        /// <summary>
        /// Returns a bVector3 from component-wise application of operator&gt;= (lhs &gt;= rhs).
        /// </summary>
        public static bVector3 operator>=(float lhs, Vector3 rhs) => new bVector3(lhs >= rhs.x, lhs >= rhs.y, lhs >= rhs.z);
        
        /// <summary>
        /// Returns a Vector3 from component-wise application of operator+ (lhs + rhs).
        /// </summary>
        public static Vector3 operator+(Vector3 lhs, Vector3 rhs) => new Vector3(lhs.x + rhs.x, lhs.y + rhs.y, lhs.z + rhs.z);
        
        /// <summary>
        /// Returns a Vector3 from component-wise application of operator+ (lhs + rhs).
        /// </summary>
        public static Vector3 operator+(Vector3 lhs, float rhs) => new Vector3(lhs.x + rhs, lhs.y + rhs, lhs.z + rhs);
        
        /// <summary>
        /// Returns a Vector3 from component-wise application of operator+ (lhs + rhs).
        /// </summary>
        public static Vector3 operator+(float lhs, Vector3 rhs) => new Vector3(lhs + rhs.x, lhs + rhs.y, lhs + rhs.z);
        
        /// <summary>
        /// Returns a Vector3 from component-wise application of operator- (lhs - rhs).
        /// </summary>
        public static Vector3 operator-(Vector3 lhs, Vector3 rhs) => new Vector3(lhs.x - rhs.x, lhs.y - rhs.y, lhs.z - rhs.z);
        
        /// <summary>
        /// Returns a Vector3 from component-wise application of operator- (lhs - rhs).
        /// </summary>
        public static Vector3 operator-(Vector3 lhs, float rhs) => new Vector3(lhs.x - rhs, lhs.y - rhs, lhs.z - rhs);
        
        /// <summary>
        /// Returns a Vector3 from component-wise application of operator- (lhs - rhs).
        /// </summary>
        public static Vector3 operator-(float lhs, Vector3 rhs) => new Vector3(lhs - rhs.x, lhs - rhs.y, lhs - rhs.z);
        
        /// <summary>
        /// Returns a Vector3 from component-wise application of operator* (lhs * rhs).
        /// </summary>
        public static Vector3 operator*(Vector3 lhs, Vector3 rhs) => new Vector3(lhs.x * rhs.x, lhs.y * rhs.y, lhs.z * rhs.z);
        
        /// <summary>
        /// Returns a Vector3 from component-wise application of operator* (lhs * rhs).
        /// </summary>
        public static Vector3 operator*(Vector3 lhs, float rhs) => new Vector3(lhs.x * rhs, lhs.y * rhs, lhs.z * rhs);
        
        /// <summary>
        /// Returns a Vector3 from component-wise application of operator* (lhs * rhs).
        /// </summary>
        public static Vector3 operator*(float lhs, Vector3 rhs) => new Vector3(lhs * rhs.x, lhs * rhs.y, lhs * rhs.z);
        
        /// <summary>
        /// Returns a Vector3 from component-wise application of operator/ (lhs / rhs).
        /// </summary>
        public static Vector3 operator/(Vector3 lhs, Vector3 rhs) => new Vector3(lhs.x / rhs.x, lhs.y / rhs.y, lhs.z / rhs.z);
        
        /// <summary>
        /// Returns a Vector3 from component-wise application of operator/ (lhs / rhs).
        /// </summary>
        public static Vector3 operator/(Vector3 lhs, float rhs) => new Vector3(lhs.x / rhs, lhs.y / rhs, lhs.z / rhs);
        
        /// <summary>
        /// Returns a Vector3 from component-wise application of operator/ (lhs / rhs).
        /// </summary>
        public static Vector3 operator/(float lhs, Vector3 rhs) => new Vector3(lhs / rhs.x, lhs / rhs.y, lhs / rhs.z);
        
        /// <summary>
        /// Returns a Vector3 from component-wise application of operator+ (identity).
        /// </summary>
        public static Vector3 operator+(Vector3 v) => v;
        
        /// <summary>
        /// Returns a Vector3 from component-wise application of operator- (-v).
        /// </summary>
        public static Vector3 operator-(Vector3 v) => new Vector3(-v.x, -v.y, -v.z);
        
        /// <summary>
        /// Returns a Vector3 from component-wise application of operator% (lhs % rhs).
        /// </summary>
        public static Vector3 operator%(Vector3 lhs, Vector3 rhs) => new Vector3(lhs.x % rhs.x, lhs.y % rhs.y, lhs.z % rhs.z);
        
        /// <summary>
        /// Returns a Vector3 from component-wise application of operator% (lhs % rhs).
        /// </summary>
        public static Vector3 operator%(Vector3 lhs, float rhs) => new Vector3(lhs.x % rhs, lhs.y % rhs, lhs.z % rhs);
        
        /// <summary>
        /// Returns a Vector3 from component-wise application of operator% (lhs % rhs).
        /// </summary>
        public static Vector3 operator%(float lhs, Vector3 rhs) => new Vector3(lhs % rhs.x, lhs % rhs.y, lhs % rhs.z);

        #endregion

    }
}
