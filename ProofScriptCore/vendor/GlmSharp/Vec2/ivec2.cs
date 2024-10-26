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
    /// A Vector of type int with 2 components.
    /// </summary>
    
    
    [StructLayout(LayoutKind.Sequential)]
    public struct iVector2 : IReadOnlyList<int>, IEquatable<iVector2>
    {

        #region Fields
        
        /// <summary>
        /// x-component
        /// </summary>
        
        public int x;
        
        /// <summary>
        /// y-component
        /// </summary>
        
        public int y;

        #endregion


        #region Constructors
        
        /// <summary>
        /// Component-wise constructor
        /// </summary>
        public iVector2(int x, int y)
        {
            this.x = x;
            this.y = y;
        }
        
        /// <summary>
        /// all-same-value constructor
        /// </summary>
        public iVector2(int v)
        {
            this.x = v;
            this.y = v;
        }
        
        /// <summary>
        /// from-Vector constructor
        /// </summary>
        public iVector2(iVector2 v)
        {
            this.x = v.x;
            this.y = v.y;
        }
        
        /// <summary>
        /// from-Vector constructor (additional fields are truncated)
        /// </summary>
        public iVector2(iVector3 v)
        {
            this.x = v.x;
            this.y = v.y;
        }
        
        /// <summary>
        /// from-Vector constructor (additional fields are truncated)
        /// </summary>
        public iVector2(iVector4 v)
        {
            this.x = v.x;
            this.y = v.y;
        }
        
        /// <summary>
        /// From-array/list constructor (superfluous values are ignored, missing values are zero-filled).
        /// </summary>
        public iVector2(IReadOnlyList<int> v)
        {
            var c = v.Count;
            this.x = c < 0 ? 0 : v[0];
            this.y = c < 1 ? 0 : v[1];
        }
        
        /// <summary>
        /// Generic from-array constructor (superfluous values are ignored, missing values are zero-filled).
        /// </summary>
        public iVector2(Object[] v)
        {
            var c = v.Length;
            this.x = c < 0 ? 0 : (int)v[0];
            this.y = c < 1 ? 0 : (int)v[1];
        }
        
        /// <summary>
        /// From-array constructor (superfluous values are ignored, missing values are zero-filled).
        /// </summary>
        public iVector2(int[] v)
        {
            var c = v.Length;
            this.x = c < 0 ? 0 : v[0];
            this.y = c < 1 ? 0 : v[1];
        }
        
        /// <summary>
        /// From-array constructor with base index (superfluous values are ignored, missing values are zero-filled).
        /// </summary>
        public iVector2(int[] v, int startIndex)
        {
            var c = v.Length;
            this.x = c + startIndex < 0 ? 0 : v[0 + startIndex];
            this.y = c + startIndex < 1 ? 0 : v[1 + startIndex];
        }
        
        /// <summary>
        /// From-IEnumerable constructor (superfluous values are ignored, missing values are zero-filled).
        /// </summary>
        public iVector2(IEnumerable<int> v)
            : this(v.ToArray())
        {
        }

        #endregion


        #region Implicit Operators
        
        /// <summary>
        /// Implicitly converts this to a lVector2.
        /// </summary>
        public static implicit operator lVector2(iVector2 v) => new lVector2((long)v.x, (long)v.y);
        
        /// <summary>
        /// Implicitly converts this to a Vector2.
        /// </summary>
        public static implicit operator Vector2(iVector2 v) => new Vector2((float)v.x, (float)v.y);
        
        /// <summary>
        /// Implicitly converts this to a hVector2.
        /// </summary>
        public static implicit operator hVector2(iVector2 v) => new hVector2((Half)v.x, (Half)v.y);
        
        /// <summary>
        /// Implicitly converts this to a dVector2.
        /// </summary>
        public static implicit operator dVector2(iVector2 v) => new dVector2((double)v.x, (double)v.y);
        
        /// <summary>
        /// Implicitly converts this to a decVector2.
        /// </summary>
        public static implicit operator decVector2(iVector2 v) => new decVector2((decimal)v.x, (decimal)v.y);
        
        /// <summary>
        /// Implicitly converts this to a cVector2.
        /// </summary>
        //public static implicit operator cVector2(iVector2 v) => new cVector2((Complex)v.x, (Complex)v.y);

        #endregion


        #region Explicit Operators
        
        /// <summary>
        /// Explicitly converts this to a iVector3. (Higher components are zeroed)
        /// </summary>
        public static explicit operator iVector3(iVector2 v) => new iVector3((int)v.x, (int)v.y, 0);
        
        /// <summary>
        /// Explicitly converts this to a iVector4. (Higher components are zeroed)
        /// </summary>
        public static explicit operator iVector4(iVector2 v) => new iVector4((int)v.x, (int)v.y, 0, 0);
        
        /// <summary>
        /// Explicitly converts this to a uVector2.
        /// </summary>
        public static explicit operator uVector2(iVector2 v) => new uVector2((uint)v.x, (uint)v.y);
        
        /// <summary>
        /// Explicitly converts this to a uVector3. (Higher components are zeroed)
        /// </summary>
        public static explicit operator uVector3(iVector2 v) => new uVector3((uint)v.x, (uint)v.y, 0u);
        
        /// <summary>
        /// Explicitly converts this to a uVector4. (Higher components are zeroed)
        /// </summary>
        public static explicit operator uVector4(iVector2 v) => new uVector4((uint)v.x, (uint)v.y, 0u, 0u);
        
        /// <summary>
        /// Explicitly converts this to a Vector3. (Higher components are zeroed)
        /// </summary>
        public static explicit operator Vector3(iVector2 v) => new Vector3((float)v.x, (float)v.y, 0f);
        
        /// <summary>
        /// Explicitly converts this to a Vector4. (Higher components are zeroed)
        /// </summary>
        public static explicit operator Vector4(iVector2 v) => new Vector4((float)v.x, (float)v.y, 0f, 0f);
        
        /// <summary>
        /// Explicitly converts this to a hVector3. (Higher components are zeroed)
        /// </summary>
        public static explicit operator hVector3(iVector2 v) => new hVector3((Half)v.x, (Half)v.y, Half.Zero);
        
        /// <summary>
        /// Explicitly converts this to a hVector4. (Higher components are zeroed)
        /// </summary>
        public static explicit operator hVector4(iVector2 v) => new hVector4((Half)v.x, (Half)v.y, Half.Zero, Half.Zero);
        
        /// <summary>
        /// Explicitly converts this to a dVector3. (Higher components are zeroed)
        /// </summary>
        public static explicit operator dVector3(iVector2 v) => new dVector3((double)v.x, (double)v.y, 0.0);
        
        /// <summary>
        /// Explicitly converts this to a dVector4. (Higher components are zeroed)
        /// </summary>
        public static explicit operator dVector4(iVector2 v) => new dVector4((double)v.x, (double)v.y, 0.0, 0.0);
        
        /// <summary>
        /// Explicitly converts this to a decVector3. (Higher components are zeroed)
        /// </summary>
        public static explicit operator decVector3(iVector2 v) => new decVector3((decimal)v.x, (decimal)v.y, 0m);
        
        /// <summary>
        /// Explicitly converts this to a decVector4. (Higher components are zeroed)
        /// </summary>
        public static explicit operator decVector4(iVector2 v) => new decVector4((decimal)v.x, (decimal)v.y, 0m, 0m);
        
        /// <summary>
        /// Explicitly converts this to a cVector3. (Higher components are zeroed)
        /// </summary>
        //public static explicit operator cVector3(iVector2 v) => new cVector3((Complex)v.x, (Complex)v.y, Complex.Zero);
        
        /// <summary>
        /// Explicitly converts this to a cVector4. (Higher components are zeroed)
        /// </summary>
        //public static explicit operator cVector4(iVector2 v) => new cVector4((Complex)v.x, (Complex)v.y, Complex.Zero, Complex.Zero);
        
        /// <summary>
        /// Explicitly converts this to a lVector3. (Higher components are zeroed)
        /// </summary>
        public static explicit operator lVector3(iVector2 v) => new lVector3((long)v.x, (long)v.y, 0);
        
        /// <summary>
        /// Explicitly converts this to a lVector4. (Higher components are zeroed)
        /// </summary>
        public static explicit operator lVector4(iVector2 v) => new lVector4((long)v.x, (long)v.y, 0, 0);
        
        /// <summary>
        /// Explicitly converts this to a bVector2.
        /// </summary>
        public static explicit operator bVector2(iVector2 v) => new bVector2(v.x != 0, v.y != 0);
        
        /// <summary>
        /// Explicitly converts this to a bVector3. (Higher components are zeroed)
        /// </summary>
        public static explicit operator bVector3(iVector2 v) => new bVector3(v.x != 0, v.y != 0, false);
        
        /// <summary>
        /// Explicitly converts this to a bVector4. (Higher components are zeroed)
        /// </summary>
        public static explicit operator bVector4(iVector2 v) => new bVector4(v.x != 0, v.y != 0, false, false);
        
        /// <summary>
        /// Explicitly converts this to a int array.
        /// </summary>
        public static explicit operator int[](iVector2 v) => new [] { v.x, v.y };
        
        /// <summary>
        /// Explicitly converts this to a generic object array.
        /// </summary>
        public static explicit operator Object[](iVector2 v) => new Object[] { v.x, v.y };

        #endregion


        #region Indexer
        
        /// <summary>
        /// Gets/Sets a specific indexed component (a bit slower than direct access).
        /// </summary>
        public int this[int index]
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
        public swizzle_iVector2 swizzle => new swizzle_iVector2(x, y);
        
        /// <summary>
        /// Gets or sets the specified subset of components. For more advanced (read-only) swizzling, use the .swizzle property.
        /// </summary>
        public iVector2 xy
        {
            get
            {
                return new iVector2(x, y);
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
        public iVector2 rg
        {
            get
            {
                return new iVector2(x, y);
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
        public int r
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
        public int g
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
        public int[] Values => new[] { x, y };
        
        /// <summary>
        /// Returns the number of components (2).
        /// </summary>
        public int Count => 2;
        
        /// <summary>
        /// Returns the minimal component of this Vector.
        /// </summary>
        public int MinElement => Math.Min(x, y);
        
        /// <summary>
        /// Returns the maximal component of this Vector.
        /// </summary>
        public int MaxElement => Math.Max(x, y);
        
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
        public int Sum => (x + y);
        
        /// <summary>
        /// Returns the euclidean norm of this Vector.
        /// </summary>
        public float Norm => (float)Math.Sqrt((x*x + y*y));
        
        /// <summary>
        /// Returns the one-norm of this Vector.
        /// </summary>
        public float Norm1 => (Math.Abs(x) + Math.Abs(y));
        
        /// <summary>
        /// Returns the two-norm (euclidean length) of this Vector.
        /// </summary>
        public float Norm2 => (float)Math.Sqrt((x*x + y*y));
        
        /// <summary>
        /// Returns the max-norm of this Vector.
        /// </summary>
        public float NormMax => Math.Max(Math.Abs(x), Math.Abs(y));

        #endregion


        #region Static Properties
        
        /// <summary>
        /// Predefined all-zero Vector
        /// </summary>
        public static iVector2 Zero { get; } = new iVector2(0, 0);
        
        /// <summary>
        /// Predefined all-ones Vector
        /// </summary>
        public static iVector2 Ones { get; } = new iVector2(1, 1);
        
        /// <summary>
        /// Predefined unit-X Vector
        /// </summary>
        public static iVector2 UnitX { get; } = new iVector2(1, 0);
        
        /// <summary>
        /// Predefined unit-Y Vector
        /// </summary>
        public static iVector2 UnitY { get; } = new iVector2(0, 1);
        
        /// <summary>
        /// Predefined all-MaxValue Vector
        /// </summary>
        public static iVector2 MaxValue { get; } = new iVector2(int.MaxValue, int.MaxValue);
        
        /// <summary>
        /// Predefined all-MinValue Vector
        /// </summary>
        public static iVector2 MinValue { get; } = new iVector2(int.MinValue, int.MinValue);

        #endregion


        #region Operators
        
        /// <summary>
        /// Returns true iff this equals rhs component-wise.
        /// </summary>
        public static bool operator==(iVector2 lhs, iVector2 rhs) => lhs.Equals(rhs);
        
        /// <summary>
        /// Returns true iff this does not equal rhs (component-wise).
        /// </summary>
        public static bool operator!=(iVector2 lhs, iVector2 rhs) => !lhs.Equals(rhs);

        #endregion


        #region Functions
        
        /// <summary>
        /// Returns an enumerator that iterates through all components.
        /// </summary>
        public IEnumerator<int> GetEnumerator()
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
        public bool Equals(iVector2 rhs) => (x.Equals(rhs.x) && y.Equals(rhs.y));
        
        /// <summary>
        /// Returns true iff this equals rhs type- and component-wise.
        /// </summary>
        public override bool Equals(object obj)
        {
            if (ReferenceEquals(null, obj)) return false;
            return obj is iVector2 && Equals((iVector2) obj);
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

        #endregion


        #region Static Functions
        
        /// <summary>
        /// Converts the string representation of the Vector into a Vector representation (using ', ' as a separator).
        /// </summary>
        public static iVector2 Parse(string s) => Parse(s, ", ");
        
        /// <summary>
        /// Converts the string representation of the Vector into a Vector representation (using a designated separator).
        /// </summary>
        public static iVector2 Parse(string s, string sep)
        {
            var kvp = s.Split(new[] { sep }, StringSplitOptions.None);
            if (kvp.Length != 2) throw new FormatException("input has not exactly 2 parts");
            return new iVector2(int.Parse(kvp[0].Trim()), int.Parse(kvp[1].Trim()));
        }
        
        /// <summary>
        /// Converts the string representation of the Vector into a Vector representation (using a designated separator and a type provider).
        /// </summary>
        public static iVector2 Parse(string s, string sep, IFormatProvider provider)
        {
            var kvp = s.Split(new[] { sep }, StringSplitOptions.None);
            if (kvp.Length != 2) throw new FormatException("input has not exactly 2 parts");
            return new iVector2(int.Parse(kvp[0].Trim(), provider), int.Parse(kvp[1].Trim(), provider));
        }
        
        /// <summary>
        /// Converts the string representation of the Vector into a Vector representation (using a designated separator and a number style).
        /// </summary>
        public static iVector2 Parse(string s, string sep, NumberStyles style)
        {
            var kvp = s.Split(new[] { sep }, StringSplitOptions.None);
            if (kvp.Length != 2) throw new FormatException("input has not exactly 2 parts");
            return new iVector2(int.Parse(kvp[0].Trim(), style), int.Parse(kvp[1].Trim(), style));
        }
        
        /// <summary>
        /// Converts the string representation of the Vector into a Vector representation (using a designated separator and a number style and a forMatrix provider).
        /// </summary>
        public static iVector2 Parse(string s, string sep, NumberStyles style, IFormatProvider provider)
        {
            var kvp = s.Split(new[] { sep }, StringSplitOptions.None);
            if (kvp.Length != 2) throw new FormatException("input has not exactly 2 parts");
            return new iVector2(int.Parse(kvp[0].Trim(), style, provider), int.Parse(kvp[1].Trim(), style, provider));
        }
        
        /// <summary>
        /// Tries to convert the string representation of the Vector into a Vector representation (using ', ' as a separator), returns false if string was invalid.
        /// </summary>
        public static bool TryParse(string s, out iVector2 result) => TryParse(s, ", ", out result);
        
        /// <summary>
        /// Tries to convert the string representation of the Vector into a Vector representation (using a designated separator), returns false if string was invalid.
        /// </summary>
        public static bool TryParse(string s, string sep, out iVector2 result)
        {
            result = Zero;
            if (string.IsNullOrEmpty(s)) return false;
            var kvp = s.Split(new[] { sep }, StringSplitOptions.None);
            if (kvp.Length != 2) return false;
            int x = 0, y = 0;
            var ok = (int.TryParse(kvp[0].Trim(), out x) && int.TryParse(kvp[1].Trim(), out y));
            result = ok ? new iVector2(x, y) : Zero;
            return ok;
        }
        
        /// <summary>
        /// Tries to convert the string representation of the Vector into a Vector representation (using a designated separator and a number style and a forMatrix provider), returns false if string was invalid.
        /// </summary>
        public static bool TryParse(string s, string sep, NumberStyles style, IFormatProvider provider, out iVector2 result)
        {
            result = Zero;
            if (string.IsNullOrEmpty(s)) return false;
            var kvp = s.Split(new[] { sep }, StringSplitOptions.None);
            if (kvp.Length != 2) return false;
            int x = 0, y = 0;
            var ok = (int.TryParse(kvp[0].Trim(), style, provider, out x) && int.TryParse(kvp[1].Trim(), style, provider, out y));
            result = ok ? new iVector2(x, y) : Zero;
            return ok;
        }
        
        /// <summary>
        /// OuterProduct treats the first parameter c as a column Vector (Matrix with one column) and the second parameter r as a row Vector (Matrix with one row) and does a linear algebraic Matrix multiply c * r, yielding a Matrix whose number of rows is the number of components in c and whose number of columns is the number of components in r.
        /// </summary>
        public static iMatrix2 OuterProduct(iVector2 c, iVector2 r) => new iMatrix2(c.x * r.x, c.y * r.x, c.x * r.y, c.y * r.y);
        
        /// <summary>
        /// OuterProduct treats the first parameter c as a column Vector (Matrix with one column) and the second parameter r as a row Vector (Matrix with one row) and does a linear algebraic Matrix multiply c * r, yielding a Matrix whose number of rows is the number of components in c and whose number of columns is the number of components in r.
        /// </summary>
        public static iMatrix2x3 OuterProduct(iVector3 c, iVector2 r) => new iMatrix2x3(c.x * r.x, c.y * r.x, c.z * r.x, c.x * r.y, c.y * r.y, c.z * r.y);
        
        /// <summary>
        /// OuterProduct treats the first parameter c as a column Vector (Matrix with one column) and the second parameter r as a row Vector (Matrix with one row) and does a linear algebraic Matrix multiply c * r, yielding a Matrix whose number of rows is the number of components in c and whose number of columns is the number of components in r.
        /// </summary>
        public static iMatrix3x2 OuterProduct(iVector2 c, iVector3 r) => new iMatrix3x2(c.x * r.x, c.y * r.x, c.x * r.y, c.y * r.y, c.x * r.z, c.y * r.z);
        
        /// <summary>
        /// OuterProduct treats the first parameter c as a column Vector (Matrix with one column) and the second parameter r as a row Vector (Matrix with one row) and does a linear algebraic Matrix multiply c * r, yielding a Matrix whose number of rows is the number of components in c and whose number of columns is the number of components in r.
        /// </summary>
        public static iMatrix2x4 OuterProduct(iVector4 c, iVector2 r) => new iMatrix2x4(c.x * r.x, c.y * r.x, c.z * r.x, c.w * r.x, c.x * r.y, c.y * r.y, c.z * r.y, c.w * r.y);
        
        /// <summary>
        /// OuterProduct treats the first parameter c as a column Vector (Matrix with one column) and the second parameter r as a row Vector (Matrix with one row) and does a linear algebraic Matrix multiply c * r, yielding a Matrix whose number of rows is the number of components in c and whose number of columns is the number of components in r.
        /// </summary>
        public static iMatrix4x2 OuterProduct(iVector2 c, iVector4 r) => new iMatrix4x2(c.x * r.x, c.y * r.x, c.x * r.y, c.y * r.y, c.x * r.z, c.y * r.z, c.x * r.w, c.y * r.w);
        
        /// <summary>
        /// Returns the inner product (dot product, scalar product) of the two Vectors.
        /// </summary>
        public static int Dot(iVector2 lhs, iVector2 rhs) => (lhs.x * rhs.x + lhs.y * rhs.y);
        
        /// <summary>
        /// Returns the euclidean distance between the two Vectors.
        /// </summary>
        public static float Distance(iVector2 lhs, iVector2 rhs) => (lhs - rhs).Length;
        
        /// <summary>
        /// Returns the squared euclidean distance between the two Vectors.
        /// </summary>
        public static float DistanceSqr(iVector2 lhs, iVector2 rhs) => (lhs - rhs).LengthSqr;
        
        /// <summary>
        /// Calculate the reflection direction for an incident Vector (N should be normalized in order to achieve the desired result).
        /// </summary>
        public static iVector2 Reflect(iVector2 I, iVector2 N) => I - 2 * Dot(N, I) * N;
        
        /// <summary>
        /// Calculate the refraction direction for an incident Vector (The input parameters I and N should be normalized in order to achieve the desired result).
        /// </summary>
        public static iVector2 Refract(iVector2 I, iVector2 N, int eta)
        {
            var dNI = Dot(N, I);
            var k = 1 - eta * eta * (1 - dNI * dNI);
            if (k < 0) return Zero;
            return eta * I - (eta * dNI + (int)Math.Sqrt(k)) * N;
        }
        
        /// <summary>
        /// Returns a Vector pointing in the same direction as another (faceforward orients a Vector to point away from a surface as defined by its normal. If dot(Nref, I) is negative faceforward returns N, otherwise it returns -N).
        /// </summary>
        public static iVector2 FaceForward(iVector2 N, iVector2 I, iVector2 Nref) => Dot(Nref, I) < 0 ? N : -N;
        
        /// <summary>
        /// Returns the length of the outer product (cross product, Vector product) of the two Vectors.
        /// </summary>
        public static int Cross(iVector2 l, iVector2 r) => l.x * r.y - l.y * r.x;
        
        /// <summary>
        /// Returns a iVector2 with independent and identically distributed uniform integer values between 0 (inclusive) and int.MaxValue (exclusive).
        /// </summary>
        public static iVector2 Random(System.Random random) => new iVector2((int)random.Next(), (int)random.Next());

        #endregion


        #region Component-Wise Static Functions
        
        /// <summary>
        /// Returns a bVector2 from component-wise application of Equal (lhs == rhs).
        /// </summary>
        public static bVector2 Equal(iVector2 lhs, iVector2 rhs) => new bVector2(lhs.x == rhs.x, lhs.y == rhs.y);
        
        /// <summary>
        /// Returns a bVector2 from component-wise application of Equal (lhs == rhs).
        /// </summary>
        public static bVector2 Equal(iVector2 lhs, int rhs) => new bVector2(lhs.x == rhs, lhs.y == rhs);
        
        /// <summary>
        /// Returns a bVector2 from component-wise application of Equal (lhs == rhs).
        /// </summary>
        public static bVector2 Equal(int lhs, iVector2 rhs) => new bVector2(lhs == rhs.x, lhs == rhs.y);
        
        /// <summary>
        /// Returns a bVector from the application of Equal (lhs == rhs).
        /// </summary>
        public static bVector2 Equal(int lhs, int rhs) => new bVector2(lhs == rhs);
        
        /// <summary>
        /// Returns a bVector2 from component-wise application of NotEqual (lhs != rhs).
        /// </summary>
        public static bVector2 NotEqual(iVector2 lhs, iVector2 rhs) => new bVector2(lhs.x != rhs.x, lhs.y != rhs.y);
        
        /// <summary>
        /// Returns a bVector2 from component-wise application of NotEqual (lhs != rhs).
        /// </summary>
        public static bVector2 NotEqual(iVector2 lhs, int rhs) => new bVector2(lhs.x != rhs, lhs.y != rhs);
        
        /// <summary>
        /// Returns a bVector2 from component-wise application of NotEqual (lhs != rhs).
        /// </summary>
        public static bVector2 NotEqual(int lhs, iVector2 rhs) => new bVector2(lhs != rhs.x, lhs != rhs.y);
        
        /// <summary>
        /// Returns a bVector from the application of NotEqual (lhs != rhs).
        /// </summary>
        public static bVector2 NotEqual(int lhs, int rhs) => new bVector2(lhs != rhs);
        
        /// <summary>
        /// Returns a bVector2 from component-wise application of GreaterThan (lhs &gt; rhs).
        /// </summary>
        public static bVector2 GreaterThan(iVector2 lhs, iVector2 rhs) => new bVector2(lhs.x > rhs.x, lhs.y > rhs.y);
        
        /// <summary>
        /// Returns a bVector2 from component-wise application of GreaterThan (lhs &gt; rhs).
        /// </summary>
        public static bVector2 GreaterThan(iVector2 lhs, int rhs) => new bVector2(lhs.x > rhs, lhs.y > rhs);
        
        /// <summary>
        /// Returns a bVector2 from component-wise application of GreaterThan (lhs &gt; rhs).
        /// </summary>
        public static bVector2 GreaterThan(int lhs, iVector2 rhs) => new bVector2(lhs > rhs.x, lhs > rhs.y);
        
        /// <summary>
        /// Returns a bVector from the application of GreaterThan (lhs &gt; rhs).
        /// </summary>
        public static bVector2 GreaterThan(int lhs, int rhs) => new bVector2(lhs > rhs);
        
        /// <summary>
        /// Returns a bVector2 from component-wise application of GreaterThanEqual (lhs &gt;= rhs).
        /// </summary>
        public static bVector2 GreaterThanEqual(iVector2 lhs, iVector2 rhs) => new bVector2(lhs.x >= rhs.x, lhs.y >= rhs.y);
        
        /// <summary>
        /// Returns a bVector2 from component-wise application of GreaterThanEqual (lhs &gt;= rhs).
        /// </summary>
        public static bVector2 GreaterThanEqual(iVector2 lhs, int rhs) => new bVector2(lhs.x >= rhs, lhs.y >= rhs);
        
        /// <summary>
        /// Returns a bVector2 from component-wise application of GreaterThanEqual (lhs &gt;= rhs).
        /// </summary>
        public static bVector2 GreaterThanEqual(int lhs, iVector2 rhs) => new bVector2(lhs >= rhs.x, lhs >= rhs.y);
        
        /// <summary>
        /// Returns a bVector from the application of GreaterThanEqual (lhs &gt;= rhs).
        /// </summary>
        public static bVector2 GreaterThanEqual(int lhs, int rhs) => new bVector2(lhs >= rhs);
        
        /// <summary>
        /// Returns a bVector2 from component-wise application of LesserThan (lhs &lt; rhs).
        /// </summary>
        public static bVector2 LesserThan(iVector2 lhs, iVector2 rhs) => new bVector2(lhs.x < rhs.x, lhs.y < rhs.y);
        
        /// <summary>
        /// Returns a bVector2 from component-wise application of LesserThan (lhs &lt; rhs).
        /// </summary>
        public static bVector2 LesserThan(iVector2 lhs, int rhs) => new bVector2(lhs.x < rhs, lhs.y < rhs);
        
        /// <summary>
        /// Returns a bVector2 from component-wise application of LesserThan (lhs &lt; rhs).
        /// </summary>
        public static bVector2 LesserThan(int lhs, iVector2 rhs) => new bVector2(lhs < rhs.x, lhs < rhs.y);
        
        /// <summary>
        /// Returns a bVector from the application of LesserThan (lhs &lt; rhs).
        /// </summary>
        public static bVector2 LesserThan(int lhs, int rhs) => new bVector2(lhs < rhs);
        
        /// <summary>
        /// Returns a bVector2 from component-wise application of LesserThanEqual (lhs &lt;= rhs).
        /// </summary>
        public static bVector2 LesserThanEqual(iVector2 lhs, iVector2 rhs) => new bVector2(lhs.x <= rhs.x, lhs.y <= rhs.y);
        
        /// <summary>
        /// Returns a bVector2 from component-wise application of LesserThanEqual (lhs &lt;= rhs).
        /// </summary>
        public static bVector2 LesserThanEqual(iVector2 lhs, int rhs) => new bVector2(lhs.x <= rhs, lhs.y <= rhs);
        
        /// <summary>
        /// Returns a bVector2 from component-wise application of LesserThanEqual (lhs &lt;= rhs).
        /// </summary>
        public static bVector2 LesserThanEqual(int lhs, iVector2 rhs) => new bVector2(lhs <= rhs.x, lhs <= rhs.y);
        
        /// <summary>
        /// Returns a bVector from the application of LesserThanEqual (lhs &lt;= rhs).
        /// </summary>
        public static bVector2 LesserThanEqual(int lhs, int rhs) => new bVector2(lhs <= rhs);
        
        /// <summary>
        /// Returns a iVector2 from component-wise application of Abs (Math.Abs(v)).
        /// </summary>
        public static iVector2 Abs(iVector2 v) => new iVector2(Math.Abs(v.x), Math.Abs(v.y));
        
        /// <summary>
        /// Returns a iVector from the application of Abs (Math.Abs(v)).
        /// </summary>
        public static iVector2 Abs(int v) => new iVector2(Math.Abs(v));
        
        /// <summary>
        /// Returns a iVector2 from component-wise application of HermiteInterpolationOrder3 ((3 - 2 * v) * v * v).
        /// </summary>
        public static iVector2 HermiteInterpolationOrder3(iVector2 v) => new iVector2((3 - 2 * v.x) * v.x * v.x, (3 - 2 * v.y) * v.y * v.y);
        
        /// <summary>
        /// Returns a iVector from the application of HermiteInterpolationOrder3 ((3 - 2 * v) * v * v).
        /// </summary>
        public static iVector2 HermiteInterpolationOrder3(int v) => new iVector2((3 - 2 * v) * v * v);
        
        /// <summary>
        /// Returns a iVector2 from component-wise application of HermiteInterpolationOrder5 (((6 * v - 15) * v + 10) * v * v * v).
        /// </summary>
        public static iVector2 HermiteInterpolationOrder5(iVector2 v) => new iVector2(((6 * v.x - 15) * v.x + 10) * v.x * v.x * v.x, ((6 * v.y - 15) * v.y + 10) * v.y * v.y * v.y);
        
        /// <summary>
        /// Returns a iVector from the application of HermiteInterpolationOrder5 (((6 * v - 15) * v + 10) * v * v * v).
        /// </summary>
        public static iVector2 HermiteInterpolationOrder5(int v) => new iVector2(((6 * v - 15) * v + 10) * v * v * v);
        
        /// <summary>
        /// Returns a iVector2 from component-wise application of Sqr (v * v).
        /// </summary>
        public static iVector2 Sqr(iVector2 v) => new iVector2(v.x * v.x, v.y * v.y);
        
        /// <summary>
        /// Returns a iVector from the application of Sqr (v * v).
        /// </summary>
        public static iVector2 Sqr(int v) => new iVector2(v * v);
        
        /// <summary>
        /// Returns a iVector2 from component-wise application of Pow2 (v * v).
        /// </summary>
        public static iVector2 Pow2(iVector2 v) => new iVector2(v.x * v.x, v.y * v.y);
        
        /// <summary>
        /// Returns a iVector from the application of Pow2 (v * v).
        /// </summary>
        public static iVector2 Pow2(int v) => new iVector2(v * v);
        
        /// <summary>
        /// Returns a iVector2 from component-wise application of Pow3 (v * v * v).
        /// </summary>
        public static iVector2 Pow3(iVector2 v) => new iVector2(v.x * v.x * v.x, v.y * v.y * v.y);
        
        /// <summary>
        /// Returns a iVector from the application of Pow3 (v * v * v).
        /// </summary>
        public static iVector2 Pow3(int v) => new iVector2(v * v * v);
        
        /// <summary>
        /// Returns a iVector2 from component-wise application of Step (v &gt;= 0 ? 1 : 0).
        /// </summary>
        public static iVector2 Step(iVector2 v) => new iVector2(v.x >= 0 ? 1 : 0, v.y >= 0 ? 1 : 0);
        
        /// <summary>
        /// Returns a iVector from the application of Step (v &gt;= 0 ? 1 : 0).
        /// </summary>
        public static iVector2 Step(int v) => new iVector2(v >= 0 ? 1 : 0);
        
        /// <summary>
        /// Returns a iVector2 from component-wise application of Sqrt ((int)Math.Sqrt((double)v)).
        /// </summary>
        public static iVector2 Sqrt(iVector2 v) => new iVector2((int)Math.Sqrt((double)v.x), (int)Math.Sqrt((double)v.y));
        
        /// <summary>
        /// Returns a iVector from the application of Sqrt ((int)Math.Sqrt((double)v)).
        /// </summary>
        public static iVector2 Sqrt(int v) => new iVector2((int)Math.Sqrt((double)v));
        
        /// <summary>
        /// Returns a iVector2 from component-wise application of InverseSqrt ((int)(1.0 / Math.Sqrt((double)v))).
        /// </summary>
        public static iVector2 InverseSqrt(iVector2 v) => new iVector2((int)(1.0 / Math.Sqrt((double)v.x)), (int)(1.0 / Math.Sqrt((double)v.y)));
        
        /// <summary>
        /// Returns a iVector from the application of InverseSqrt ((int)(1.0 / Math.Sqrt((double)v))).
        /// </summary>
        public static iVector2 InverseSqrt(int v) => new iVector2((int)(1.0 / Math.Sqrt((double)v)));
        
        /// <summary>
        /// Returns a iVector2 from component-wise application of Sign (Math.Sign(v)).
        /// </summary>
        public static iVector2 Sign(iVector2 v) => new iVector2(Math.Sign(v.x), Math.Sign(v.y));
        
        /// <summary>
        /// Returns a iVector from the application of Sign (Math.Sign(v)).
        /// </summary>
        public static iVector2 Sign(int v) => new iVector2(Math.Sign(v));
        
        /// <summary>
        /// Returns a iVector2 from component-wise application of Max (Math.Max(lhs, rhs)).
        /// </summary>
        public static iVector2 Max(iVector2 lhs, iVector2 rhs) => new iVector2(Math.Max(lhs.x, rhs.x), Math.Max(lhs.y, rhs.y));
        
        /// <summary>
        /// Returns a iVector2 from component-wise application of Max (Math.Max(lhs, rhs)).
        /// </summary>
        public static iVector2 Max(iVector2 lhs, int rhs) => new iVector2(Math.Max(lhs.x, rhs), Math.Max(lhs.y, rhs));
        
        /// <summary>
        /// Returns a iVector2 from component-wise application of Max (Math.Max(lhs, rhs)).
        /// </summary>
        public static iVector2 Max(int lhs, iVector2 rhs) => new iVector2(Math.Max(lhs, rhs.x), Math.Max(lhs, rhs.y));
        
        /// <summary>
        /// Returns a iVector from the application of Max (Math.Max(lhs, rhs)).
        /// </summary>
        public static iVector2 Max(int lhs, int rhs) => new iVector2(Math.Max(lhs, rhs));
        
        /// <summary>
        /// Returns a iVector2 from component-wise application of Min (Math.Min(lhs, rhs)).
        /// </summary>
        public static iVector2 Min(iVector2 lhs, iVector2 rhs) => new iVector2(Math.Min(lhs.x, rhs.x), Math.Min(lhs.y, rhs.y));
        
        /// <summary>
        /// Returns a iVector2 from component-wise application of Min (Math.Min(lhs, rhs)).
        /// </summary>
        public static iVector2 Min(iVector2 lhs, int rhs) => new iVector2(Math.Min(lhs.x, rhs), Math.Min(lhs.y, rhs));
        
        /// <summary>
        /// Returns a iVector2 from component-wise application of Min (Math.Min(lhs, rhs)).
        /// </summary>
        public static iVector2 Min(int lhs, iVector2 rhs) => new iVector2(Math.Min(lhs, rhs.x), Math.Min(lhs, rhs.y));
        
        /// <summary>
        /// Returns a iVector from the application of Min (Math.Min(lhs, rhs)).
        /// </summary>
        public static iVector2 Min(int lhs, int rhs) => new iVector2(Math.Min(lhs, rhs));
        
        /// <summary>
        /// Returns a iVector2 from component-wise application of Pow ((int)Math.Pow((double)lhs, (double)rhs)).
        /// </summary>
        public static iVector2 Pow(iVector2 lhs, iVector2 rhs) => new iVector2((int)Math.Pow((double)lhs.x, (double)rhs.x), (int)Math.Pow((double)lhs.y, (double)rhs.y));
        
        /// <summary>
        /// Returns a iVector2 from component-wise application of Pow ((int)Math.Pow((double)lhs, (double)rhs)).
        /// </summary>
        public static iVector2 Pow(iVector2 lhs, int rhs) => new iVector2((int)Math.Pow((double)lhs.x, (double)rhs), (int)Math.Pow((double)lhs.y, (double)rhs));
        
        /// <summary>
        /// Returns a iVector2 from component-wise application of Pow ((int)Math.Pow((double)lhs, (double)rhs)).
        /// </summary>
        public static iVector2 Pow(int lhs, iVector2 rhs) => new iVector2((int)Math.Pow((double)lhs, (double)rhs.x), (int)Math.Pow((double)lhs, (double)rhs.y));
        
        /// <summary>
        /// Returns a iVector from the application of Pow ((int)Math.Pow((double)lhs, (double)rhs)).
        /// </summary>
        public static iVector2 Pow(int lhs, int rhs) => new iVector2((int)Math.Pow((double)lhs, (double)rhs));
        
        /// <summary>
        /// Returns a iVector2 from component-wise application of Log ((int)Math.Log((double)lhs, (double)rhs)).
        /// </summary>
        public static iVector2 Log(iVector2 lhs, iVector2 rhs) => new iVector2((int)Math.Log((double)lhs.x, (double)rhs.x), (int)Math.Log((double)lhs.y, (double)rhs.y));
        
        /// <summary>
        /// Returns a iVector2 from component-wise application of Log ((int)Math.Log((double)lhs, (double)rhs)).
        /// </summary>
        public static iVector2 Log(iVector2 lhs, int rhs) => new iVector2((int)Math.Log((double)lhs.x, (double)rhs), (int)Math.Log((double)lhs.y, (double)rhs));
        
        /// <summary>
        /// Returns a iVector2 from component-wise application of Log ((int)Math.Log((double)lhs, (double)rhs)).
        /// </summary>
        public static iVector2 Log(int lhs, iVector2 rhs) => new iVector2((int)Math.Log((double)lhs, (double)rhs.x), (int)Math.Log((double)lhs, (double)rhs.y));
        
        /// <summary>
        /// Returns a iVector from the application of Log ((int)Math.Log((double)lhs, (double)rhs)).
        /// </summary>
        public static iVector2 Log(int lhs, int rhs) => new iVector2((int)Math.Log((double)lhs, (double)rhs));
        
        /// <summary>
        /// Returns a iVector2 from component-wise application of Clamp (Math.Min(Math.Max(v, min), max)).
        /// </summary>
        public static iVector2 Clamp(iVector2 v, iVector2 min, iVector2 max) => new iVector2(Math.Min(Math.Max(v.x, min.x), max.x), Math.Min(Math.Max(v.y, min.y), max.y));
        
        /// <summary>
        /// Returns a iVector2 from component-wise application of Clamp (Math.Min(Math.Max(v, min), max)).
        /// </summary>
        public static iVector2 Clamp(iVector2 v, iVector2 min, int max) => new iVector2(Math.Min(Math.Max(v.x, min.x), max), Math.Min(Math.Max(v.y, min.y), max));
        
        /// <summary>
        /// Returns a iVector2 from component-wise application of Clamp (Math.Min(Math.Max(v, min), max)).
        /// </summary>
        public static iVector2 Clamp(iVector2 v, int min, iVector2 max) => new iVector2(Math.Min(Math.Max(v.x, min), max.x), Math.Min(Math.Max(v.y, min), max.y));
        
        /// <summary>
        /// Returns a iVector2 from component-wise application of Clamp (Math.Min(Math.Max(v, min), max)).
        /// </summary>
        public static iVector2 Clamp(iVector2 v, int min, int max) => new iVector2(Math.Min(Math.Max(v.x, min), max), Math.Min(Math.Max(v.y, min), max));
        
        /// <summary>
        /// Returns a iVector2 from component-wise application of Clamp (Math.Min(Math.Max(v, min), max)).
        /// </summary>
        public static iVector2 Clamp(int v, iVector2 min, iVector2 max) => new iVector2(Math.Min(Math.Max(v, min.x), max.x), Math.Min(Math.Max(v, min.y), max.y));
        
        /// <summary>
        /// Returns a iVector2 from component-wise application of Clamp (Math.Min(Math.Max(v, min), max)).
        /// </summary>
        public static iVector2 Clamp(int v, iVector2 min, int max) => new iVector2(Math.Min(Math.Max(v, min.x), max), Math.Min(Math.Max(v, min.y), max));
        
        /// <summary>
        /// Returns a iVector2 from component-wise application of Clamp (Math.Min(Math.Max(v, min), max)).
        /// </summary>
        public static iVector2 Clamp(int v, int min, iVector2 max) => new iVector2(Math.Min(Math.Max(v, min), max.x), Math.Min(Math.Max(v, min), max.y));
        
        /// <summary>
        /// Returns a iVector from the application of Clamp (Math.Min(Math.Max(v, min), max)).
        /// </summary>
        public static iVector2 Clamp(int v, int min, int max) => new iVector2(Math.Min(Math.Max(v, min), max));
        
        /// <summary>
        /// Returns a iVector2 from component-wise application of Mix (min * (1-a) + max * a).
        /// </summary>
        public static iVector2 Mix(iVector2 min, iVector2 max, iVector2 a) => new iVector2(min.x * (1-a.x) + max.x * a.x, min.y * (1-a.y) + max.y * a.y);
        
        /// <summary>
        /// Returns a iVector2 from component-wise application of Mix (min * (1-a) + max * a).
        /// </summary>
        public static iVector2 Mix(iVector2 min, iVector2 max, int a) => new iVector2(min.x * (1-a) + max.x * a, min.y * (1-a) + max.y * a);
        
        /// <summary>
        /// Returns a iVector2 from component-wise application of Mix (min * (1-a) + max * a).
        /// </summary>
        public static iVector2 Mix(iVector2 min, int max, iVector2 a) => new iVector2(min.x * (1-a.x) + max * a.x, min.y * (1-a.y) + max * a.y);
        
        /// <summary>
        /// Returns a iVector2 from component-wise application of Mix (min * (1-a) + max * a).
        /// </summary>
        public static iVector2 Mix(iVector2 min, int max, int a) => new iVector2(min.x * (1-a) + max * a, min.y * (1-a) + max * a);
        
        /// <summary>
        /// Returns a iVector2 from component-wise application of Mix (min * (1-a) + max * a).
        /// </summary>
        public static iVector2 Mix(int min, iVector2 max, iVector2 a) => new iVector2(min * (1-a.x) + max.x * a.x, min * (1-a.y) + max.y * a.y);
        
        /// <summary>
        /// Returns a iVector2 from component-wise application of Mix (min * (1-a) + max * a).
        /// </summary>
        public static iVector2 Mix(int min, iVector2 max, int a) => new iVector2(min * (1-a) + max.x * a, min * (1-a) + max.y * a);
        
        /// <summary>
        /// Returns a iVector2 from component-wise application of Mix (min * (1-a) + max * a).
        /// </summary>
        public static iVector2 Mix(int min, int max, iVector2 a) => new iVector2(min * (1-a.x) + max * a.x, min * (1-a.y) + max * a.y);
        
        /// <summary>
        /// Returns a iVector from the application of Mix (min * (1-a) + max * a).
        /// </summary>
        public static iVector2 Mix(int min, int max, int a) => new iVector2(min * (1-a) + max * a);
        
        /// <summary>
        /// Returns a iVector2 from component-wise application of Lerp (min * (1-a) + max * a).
        /// </summary>
        public static iVector2 Lerp(iVector2 min, iVector2 max, iVector2 a) => new iVector2(min.x * (1-a.x) + max.x * a.x, min.y * (1-a.y) + max.y * a.y);
        
        /// <summary>
        /// Returns a iVector2 from component-wise application of Lerp (min * (1-a) + max * a).
        /// </summary>
        public static iVector2 Lerp(iVector2 min, iVector2 max, int a) => new iVector2(min.x * (1-a) + max.x * a, min.y * (1-a) + max.y * a);
        
        /// <summary>
        /// Returns a iVector2 from component-wise application of Lerp (min * (1-a) + max * a).
        /// </summary>
        public static iVector2 Lerp(iVector2 min, int max, iVector2 a) => new iVector2(min.x * (1-a.x) + max * a.x, min.y * (1-a.y) + max * a.y);
        
        /// <summary>
        /// Returns a iVector2 from component-wise application of Lerp (min * (1-a) + max * a).
        /// </summary>
        public static iVector2 Lerp(iVector2 min, int max, int a) => new iVector2(min.x * (1-a) + max * a, min.y * (1-a) + max * a);
        
        /// <summary>
        /// Returns a iVector2 from component-wise application of Lerp (min * (1-a) + max * a).
        /// </summary>
        public static iVector2 Lerp(int min, iVector2 max, iVector2 a) => new iVector2(min * (1-a.x) + max.x * a.x, min * (1-a.y) + max.y * a.y);
        
        /// <summary>
        /// Returns a iVector2 from component-wise application of Lerp (min * (1-a) + max * a).
        /// </summary>
        public static iVector2 Lerp(int min, iVector2 max, int a) => new iVector2(min * (1-a) + max.x * a, min * (1-a) + max.y * a);
        
        /// <summary>
        /// Returns a iVector2 from component-wise application of Lerp (min * (1-a) + max * a).
        /// </summary>
        public static iVector2 Lerp(int min, int max, iVector2 a) => new iVector2(min * (1-a.x) + max * a.x, min * (1-a.y) + max * a.y);
        
        /// <summary>
        /// Returns a iVector from the application of Lerp (min * (1-a) + max * a).
        /// </summary>
        public static iVector2 Lerp(int min, int max, int a) => new iVector2(min * (1-a) + max * a);
        
        /// <summary>
        /// Returns a iVector2 from component-wise application of Smoothstep (((v - edge0) / (edge1 - edge0)).Clamp().HermiteInterpolationOrder3()).
        /// </summary>
        public static iVector2 Smoothstep(iVector2 edge0, iVector2 edge1, iVector2 v) => new iVector2(((v.x - edge0.x) / (edge1.x - edge0.x)).Clamp().HermiteInterpolationOrder3(), ((v.y - edge0.y) / (edge1.y - edge0.y)).Clamp().HermiteInterpolationOrder3());
        
        /// <summary>
        /// Returns a iVector2 from component-wise application of Smoothstep (((v - edge0) / (edge1 - edge0)).Clamp().HermiteInterpolationOrder3()).
        /// </summary>
        public static iVector2 Smoothstep(iVector2 edge0, iVector2 edge1, int v) => new iVector2(((v - edge0.x) / (edge1.x - edge0.x)).Clamp().HermiteInterpolationOrder3(), ((v - edge0.y) / (edge1.y - edge0.y)).Clamp().HermiteInterpolationOrder3());
        
        /// <summary>
        /// Returns a iVector2 from component-wise application of Smoothstep (((v - edge0) / (edge1 - edge0)).Clamp().HermiteInterpolationOrder3()).
        /// </summary>
        public static iVector2 Smoothstep(iVector2 edge0, int edge1, iVector2 v) => new iVector2(((v.x - edge0.x) / (edge1 - edge0.x)).Clamp().HermiteInterpolationOrder3(), ((v.y - edge0.y) / (edge1 - edge0.y)).Clamp().HermiteInterpolationOrder3());
        
        /// <summary>
        /// Returns a iVector2 from component-wise application of Smoothstep (((v - edge0) / (edge1 - edge0)).Clamp().HermiteInterpolationOrder3()).
        /// </summary>
        public static iVector2 Smoothstep(iVector2 edge0, int edge1, int v) => new iVector2(((v - edge0.x) / (edge1 - edge0.x)).Clamp().HermiteInterpolationOrder3(), ((v - edge0.y) / (edge1 - edge0.y)).Clamp().HermiteInterpolationOrder3());
        
        /// <summary>
        /// Returns a iVector2 from component-wise application of Smoothstep (((v - edge0) / (edge1 - edge0)).Clamp().HermiteInterpolationOrder3()).
        /// </summary>
        public static iVector2 Smoothstep(int edge0, iVector2 edge1, iVector2 v) => new iVector2(((v.x - edge0) / (edge1.x - edge0)).Clamp().HermiteInterpolationOrder3(), ((v.y - edge0) / (edge1.y - edge0)).Clamp().HermiteInterpolationOrder3());
        
        /// <summary>
        /// Returns a iVector2 from component-wise application of Smoothstep (((v - edge0) / (edge1 - edge0)).Clamp().HermiteInterpolationOrder3()).
        /// </summary>
        public static iVector2 Smoothstep(int edge0, iVector2 edge1, int v) => new iVector2(((v - edge0) / (edge1.x - edge0)).Clamp().HermiteInterpolationOrder3(), ((v - edge0) / (edge1.y - edge0)).Clamp().HermiteInterpolationOrder3());
        
        /// <summary>
        /// Returns a iVector2 from component-wise application of Smoothstep (((v - edge0) / (edge1 - edge0)).Clamp().HermiteInterpolationOrder3()).
        /// </summary>
        public static iVector2 Smoothstep(int edge0, int edge1, iVector2 v) => new iVector2(((v.x - edge0) / (edge1 - edge0)).Clamp().HermiteInterpolationOrder3(), ((v.y - edge0) / (edge1 - edge0)).Clamp().HermiteInterpolationOrder3());
        
        /// <summary>
        /// Returns a iVector from the application of Smoothstep (((v - edge0) / (edge1 - edge0)).Clamp().HermiteInterpolationOrder3()).
        /// </summary>
        public static iVector2 Smoothstep(int edge0, int edge1, int v) => new iVector2(((v - edge0) / (edge1 - edge0)).Clamp().HermiteInterpolationOrder3());
        
        /// <summary>
        /// Returns a iVector2 from component-wise application of Smootherstep (((v - edge0) / (edge1 - edge0)).Clamp().HermiteInterpolationOrder5()).
        /// </summary>
        public static iVector2 Smootherstep(iVector2 edge0, iVector2 edge1, iVector2 v) => new iVector2(((v.x - edge0.x) / (edge1.x - edge0.x)).Clamp().HermiteInterpolationOrder5(), ((v.y - edge0.y) / (edge1.y - edge0.y)).Clamp().HermiteInterpolationOrder5());
        
        /// <summary>
        /// Returns a iVector2 from component-wise application of Smootherstep (((v - edge0) / (edge1 - edge0)).Clamp().HermiteInterpolationOrder5()).
        /// </summary>
        public static iVector2 Smootherstep(iVector2 edge0, iVector2 edge1, int v) => new iVector2(((v - edge0.x) / (edge1.x - edge0.x)).Clamp().HermiteInterpolationOrder5(), ((v - edge0.y) / (edge1.y - edge0.y)).Clamp().HermiteInterpolationOrder5());
        
        /// <summary>
        /// Returns a iVector2 from component-wise application of Smootherstep (((v - edge0) / (edge1 - edge0)).Clamp().HermiteInterpolationOrder5()).
        /// </summary>
        public static iVector2 Smootherstep(iVector2 edge0, int edge1, iVector2 v) => new iVector2(((v.x - edge0.x) / (edge1 - edge0.x)).Clamp().HermiteInterpolationOrder5(), ((v.y - edge0.y) / (edge1 - edge0.y)).Clamp().HermiteInterpolationOrder5());
        
        /// <summary>
        /// Returns a iVector2 from component-wise application of Smootherstep (((v - edge0) / (edge1 - edge0)).Clamp().HermiteInterpolationOrder5()).
        /// </summary>
        public static iVector2 Smootherstep(iVector2 edge0, int edge1, int v) => new iVector2(((v - edge0.x) / (edge1 - edge0.x)).Clamp().HermiteInterpolationOrder5(), ((v - edge0.y) / (edge1 - edge0.y)).Clamp().HermiteInterpolationOrder5());
        
        /// <summary>
        /// Returns a iVector2 from component-wise application of Smootherstep (((v - edge0) / (edge1 - edge0)).Clamp().HermiteInterpolationOrder5()).
        /// </summary>
        public static iVector2 Smootherstep(int edge0, iVector2 edge1, iVector2 v) => new iVector2(((v.x - edge0) / (edge1.x - edge0)).Clamp().HermiteInterpolationOrder5(), ((v.y - edge0) / (edge1.y - edge0)).Clamp().HermiteInterpolationOrder5());
        
        /// <summary>
        /// Returns a iVector2 from component-wise application of Smootherstep (((v - edge0) / (edge1 - edge0)).Clamp().HermiteInterpolationOrder5()).
        /// </summary>
        public static iVector2 Smootherstep(int edge0, iVector2 edge1, int v) => new iVector2(((v - edge0) / (edge1.x - edge0)).Clamp().HermiteInterpolationOrder5(), ((v - edge0) / (edge1.y - edge0)).Clamp().HermiteInterpolationOrder5());
        
        /// <summary>
        /// Returns a iVector2 from component-wise application of Smootherstep (((v - edge0) / (edge1 - edge0)).Clamp().HermiteInterpolationOrder5()).
        /// </summary>
        public static iVector2 Smootherstep(int edge0, int edge1, iVector2 v) => new iVector2(((v.x - edge0) / (edge1 - edge0)).Clamp().HermiteInterpolationOrder5(), ((v.y - edge0) / (edge1 - edge0)).Clamp().HermiteInterpolationOrder5());
        
        /// <summary>
        /// Returns a iVector from the application of Smootherstep (((v - edge0) / (edge1 - edge0)).Clamp().HermiteInterpolationOrder5()).
        /// </summary>
        public static iVector2 Smootherstep(int edge0, int edge1, int v) => new iVector2(((v - edge0) / (edge1 - edge0)).Clamp().HermiteInterpolationOrder5());
        
        /// <summary>
        /// Returns a iVector2 from component-wise application of Fma (a * b + c).
        /// </summary>
        public static iVector2 Fma(iVector2 a, iVector2 b, iVector2 c) => new iVector2(a.x * b.x + c.x, a.y * b.y + c.y);
        
        /// <summary>
        /// Returns a iVector2 from component-wise application of Fma (a * b + c).
        /// </summary>
        public static iVector2 Fma(iVector2 a, iVector2 b, int c) => new iVector2(a.x * b.x + c, a.y * b.y + c);
        
        /// <summary>
        /// Returns a iVector2 from component-wise application of Fma (a * b + c).
        /// </summary>
        public static iVector2 Fma(iVector2 a, int b, iVector2 c) => new iVector2(a.x * b + c.x, a.y * b + c.y);
        
        /// <summary>
        /// Returns a iVector2 from component-wise application of Fma (a * b + c).
        /// </summary>
        public static iVector2 Fma(iVector2 a, int b, int c) => new iVector2(a.x * b + c, a.y * b + c);
        
        /// <summary>
        /// Returns a iVector2 from component-wise application of Fma (a * b + c).
        /// </summary>
        public static iVector2 Fma(int a, iVector2 b, iVector2 c) => new iVector2(a * b.x + c.x, a * b.y + c.y);
        
        /// <summary>
        /// Returns a iVector2 from component-wise application of Fma (a * b + c).
        /// </summary>
        public static iVector2 Fma(int a, iVector2 b, int c) => new iVector2(a * b.x + c, a * b.y + c);
        
        /// <summary>
        /// Returns a iVector2 from component-wise application of Fma (a * b + c).
        /// </summary>
        public static iVector2 Fma(int a, int b, iVector2 c) => new iVector2(a * b + c.x, a * b + c.y);
        
        /// <summary>
        /// Returns a iVector from the application of Fma (a * b + c).
        /// </summary>
        public static iVector2 Fma(int a, int b, int c) => new iVector2(a * b + c);
        
        /// <summary>
        /// Returns a iVector2 from component-wise application of Add (lhs + rhs).
        /// </summary>
        public static iVector2 Add(iVector2 lhs, iVector2 rhs) => new iVector2(lhs.x + rhs.x, lhs.y + rhs.y);
        
        /// <summary>
        /// Returns a iVector2 from component-wise application of Add (lhs + rhs).
        /// </summary>
        public static iVector2 Add(iVector2 lhs, int rhs) => new iVector2(lhs.x + rhs, lhs.y + rhs);
        
        /// <summary>
        /// Returns a iVector2 from component-wise application of Add (lhs + rhs).
        /// </summary>
        public static iVector2 Add(int lhs, iVector2 rhs) => new iVector2(lhs + rhs.x, lhs + rhs.y);
        
        /// <summary>
        /// Returns a iVector from the application of Add (lhs + rhs).
        /// </summary>
        public static iVector2 Add(int lhs, int rhs) => new iVector2(lhs + rhs);
        
        /// <summary>
        /// Returns a iVector2 from component-wise application of Sub (lhs - rhs).
        /// </summary>
        public static iVector2 Sub(iVector2 lhs, iVector2 rhs) => new iVector2(lhs.x - rhs.x, lhs.y - rhs.y);
        
        /// <summary>
        /// Returns a iVector2 from component-wise application of Sub (lhs - rhs).
        /// </summary>
        public static iVector2 Sub(iVector2 lhs, int rhs) => new iVector2(lhs.x - rhs, lhs.y - rhs);
        
        /// <summary>
        /// Returns a iVector2 from component-wise application of Sub (lhs - rhs).
        /// </summary>
        public static iVector2 Sub(int lhs, iVector2 rhs) => new iVector2(lhs - rhs.x, lhs - rhs.y);
        
        /// <summary>
        /// Returns a iVector from the application of Sub (lhs - rhs).
        /// </summary>
        public static iVector2 Sub(int lhs, int rhs) => new iVector2(lhs - rhs);
        
        /// <summary>
        /// Returns a iVector2 from component-wise application of Mul (lhs * rhs).
        /// </summary>
        public static iVector2 Mul(iVector2 lhs, iVector2 rhs) => new iVector2(lhs.x * rhs.x, lhs.y * rhs.y);
        
        /// <summary>
        /// Returns a iVector2 from component-wise application of Mul (lhs * rhs).
        /// </summary>
        public static iVector2 Mul(iVector2 lhs, int rhs) => new iVector2(lhs.x * rhs, lhs.y * rhs);
        
        /// <summary>
        /// Returns a iVector2 from component-wise application of Mul (lhs * rhs).
        /// </summary>
        public static iVector2 Mul(int lhs, iVector2 rhs) => new iVector2(lhs * rhs.x, lhs * rhs.y);
        
        /// <summary>
        /// Returns a iVector from the application of Mul (lhs * rhs).
        /// </summary>
        public static iVector2 Mul(int lhs, int rhs) => new iVector2(lhs * rhs);
        
        /// <summary>
        /// Returns a iVector2 from component-wise application of Div (lhs / rhs).
        /// </summary>
        public static iVector2 Div(iVector2 lhs, iVector2 rhs) => new iVector2(lhs.x / rhs.x, lhs.y / rhs.y);
        
        /// <summary>
        /// Returns a iVector2 from component-wise application of Div (lhs / rhs).
        /// </summary>
        public static iVector2 Div(iVector2 lhs, int rhs) => new iVector2(lhs.x / rhs, lhs.y / rhs);
        
        /// <summary>
        /// Returns a iVector2 from component-wise application of Div (lhs / rhs).
        /// </summary>
        public static iVector2 Div(int lhs, iVector2 rhs) => new iVector2(lhs / rhs.x, lhs / rhs.y);
        
        /// <summary>
        /// Returns a iVector from the application of Div (lhs / rhs).
        /// </summary>
        public static iVector2 Div(int lhs, int rhs) => new iVector2(lhs / rhs);
        
        /// <summary>
        /// Returns a iVector2 from component-wise application of Xor (lhs ^ rhs).
        /// </summary>
        public static iVector2 Xor(iVector2 lhs, iVector2 rhs) => new iVector2(lhs.x ^ rhs.x, lhs.y ^ rhs.y);
        
        /// <summary>
        /// Returns a iVector2 from component-wise application of Xor (lhs ^ rhs).
        /// </summary>
        public static iVector2 Xor(iVector2 lhs, int rhs) => new iVector2(lhs.x ^ rhs, lhs.y ^ rhs);
        
        /// <summary>
        /// Returns a iVector2 from component-wise application of Xor (lhs ^ rhs).
        /// </summary>
        public static iVector2 Xor(int lhs, iVector2 rhs) => new iVector2(lhs ^ rhs.x, lhs ^ rhs.y);
        
        /// <summary>
        /// Returns a iVector from the application of Xor (lhs ^ rhs).
        /// </summary>
        public static iVector2 Xor(int lhs, int rhs) => new iVector2(lhs ^ rhs);
        
        /// <summary>
        /// Returns a iVector2 from component-wise application of BitwiseOr (lhs | rhs).
        /// </summary>
        public static iVector2 BitwiseOr(iVector2 lhs, iVector2 rhs) => new iVector2(lhs.x | rhs.x, lhs.y | rhs.y);
        
        /// <summary>
        /// Returns a iVector2 from component-wise application of BitwiseOr (lhs | rhs).
        /// </summary>
        public static iVector2 BitwiseOr(iVector2 lhs, int rhs) => new iVector2(lhs.x | rhs, lhs.y | rhs);
        
        /// <summary>
        /// Returns a iVector2 from component-wise application of BitwiseOr (lhs | rhs).
        /// </summary>
        public static iVector2 BitwiseOr(int lhs, iVector2 rhs) => new iVector2(lhs | rhs.x, lhs | rhs.y);
        
        /// <summary>
        /// Returns a iVector from the application of BitwiseOr (lhs | rhs).
        /// </summary>
        public static iVector2 BitwiseOr(int lhs, int rhs) => new iVector2(lhs | rhs);
        
        /// <summary>
        /// Returns a iVector2 from component-wise application of BitwiseAnd (lhs &amp; rhs).
        /// </summary>
        public static iVector2 BitwiseAnd(iVector2 lhs, iVector2 rhs) => new iVector2(lhs.x & rhs.x, lhs.y & rhs.y);
        
        /// <summary>
        /// Returns a iVector2 from component-wise application of BitwiseAnd (lhs &amp; rhs).
        /// </summary>
        public static iVector2 BitwiseAnd(iVector2 lhs, int rhs) => new iVector2(lhs.x & rhs, lhs.y & rhs);
        
        /// <summary>
        /// Returns a iVector2 from component-wise application of BitwiseAnd (lhs &amp; rhs).
        /// </summary>
        public static iVector2 BitwiseAnd(int lhs, iVector2 rhs) => new iVector2(lhs & rhs.x, lhs & rhs.y);
        
        /// <summary>
        /// Returns a iVector from the application of BitwiseAnd (lhs &amp; rhs).
        /// </summary>
        public static iVector2 BitwiseAnd(int lhs, int rhs) => new iVector2(lhs & rhs);
        
        /// <summary>
        /// Returns a iVector2 from component-wise application of LeftShift (lhs &lt;&lt; rhs).
        /// </summary>
        public static iVector2 LeftShift(iVector2 lhs, iVector2 rhs) => new iVector2(lhs.x << rhs.x, lhs.y << rhs.y);
        
        /// <summary>
        /// Returns a iVector2 from component-wise application of LeftShift (lhs &lt;&lt; rhs).
        /// </summary>
        public static iVector2 LeftShift(iVector2 lhs, int rhs) => new iVector2(lhs.x << rhs, lhs.y << rhs);
        
        /// <summary>
        /// Returns a iVector2 from component-wise application of LeftShift (lhs &lt;&lt; rhs).
        /// </summary>
        public static iVector2 LeftShift(int lhs, iVector2 rhs) => new iVector2(lhs << rhs.x, lhs << rhs.y);
        
        /// <summary>
        /// Returns a iVector from the application of LeftShift (lhs &lt;&lt; rhs).
        /// </summary>
        public static iVector2 LeftShift(int lhs, int rhs) => new iVector2(lhs << rhs);
        
        /// <summary>
        /// Returns a iVector2 from component-wise application of RightShift (lhs &gt;&gt; rhs).
        /// </summary>
        public static iVector2 RightShift(iVector2 lhs, iVector2 rhs) => new iVector2(lhs.x >> rhs.x, lhs.y >> rhs.y);
        
        /// <summary>
        /// Returns a iVector2 from component-wise application of RightShift (lhs &gt;&gt; rhs).
        /// </summary>
        public static iVector2 RightShift(iVector2 lhs, int rhs) => new iVector2(lhs.x >> rhs, lhs.y >> rhs);
        
        /// <summary>
        /// Returns a iVector2 from component-wise application of RightShift (lhs &gt;&gt; rhs).
        /// </summary>
        public static iVector2 RightShift(int lhs, iVector2 rhs) => new iVector2(lhs >> rhs.x, lhs >> rhs.y);
        
        /// <summary>
        /// Returns a iVector from the application of RightShift (lhs &gt;&gt; rhs).
        /// </summary>
        public static iVector2 RightShift(int lhs, int rhs) => new iVector2(lhs >> rhs);
        
        /// <summary>
        /// Returns a iVector2 with independent and identically distributed uniform integer values between 0 (inclusive) and maxValue (exclusive). (A maxValue of 0 is allowed and returns 0.)
        /// </summary>
        public static iVector2 Random(System.Random random, iVector2 maxValue) => new iVector2((int)random.Next((int)maxValue.x), (int)random.Next((int)maxValue.y));
        
        /// <summary>
        /// Returns a iVector2 with independent and identically distributed uniform integer values between 0 (inclusive) and maxValue (exclusive). (A maxValue of 0 is allowed and returns 0.)
        /// </summary>
        public static iVector2 Random(System.Random random, int maxValue) => new iVector2((int)random.Next((int)maxValue));
        
        /// <summary>
        /// Returns a iVector2 with independent and identically distributed uniform integer values between minValue (inclusive) and maxValue (exclusive). (minValue == maxValue is allowed and returns minValue. Negative values are allowed.)
        /// </summary>
        public static iVector2 Random(System.Random random, iVector2 minValue, iVector2 maxValue) => new iVector2((int)random.Next((int)minValue.x, (int)maxValue.x), (int)random.Next((int)minValue.y, (int)maxValue.y));
        
        /// <summary>
        /// Returns a iVector2 with independent and identically distributed uniform integer values between minValue (inclusive) and maxValue (exclusive). (minValue == maxValue is allowed and returns minValue. Negative values are allowed.)
        /// </summary>
        public static iVector2 Random(System.Random random, iVector2 minValue, int maxValue) => new iVector2((int)random.Next((int)minValue.x, (int)maxValue), (int)random.Next((int)minValue.y, (int)maxValue));
        
        /// <summary>
        /// Returns a iVector2 with independent and identically distributed uniform integer values between minValue (inclusive) and maxValue (exclusive). (minValue == maxValue is allowed and returns minValue. Negative values are allowed.)
        /// </summary>
        public static iVector2 Random(System.Random random, int minValue, iVector2 maxValue) => new iVector2((int)random.Next((int)minValue, (int)maxValue.x), (int)random.Next((int)minValue, (int)maxValue.y));
        
        /// <summary>
        /// Returns a iVector2 with independent and identically distributed uniform integer values between minValue (inclusive) and maxValue (exclusive). (minValue == maxValue is allowed and returns minValue. Negative values are allowed.)
        /// </summary>
        public static iVector2 Random(System.Random random, int minValue, int maxValue) => new iVector2((int)random.Next((int)minValue, (int)maxValue));
        
        /// <summary>
        /// Returns a iVector2 with independent and identically distributed uniform integer values between minValue (inclusive) and maxValue (exclusive). (minValue == maxValue is allowed and returns minValue. Negative values are allowed.)
        /// </summary>
        public static iVector2 RandomUniform(System.Random random, iVector2 minValue, iVector2 maxValue) => new iVector2((int)random.Next((int)minValue.x, (int)maxValue.x), (int)random.Next((int)minValue.y, (int)maxValue.y));
        
        /// <summary>
        /// Returns a iVector2 with independent and identically distributed uniform integer values between minValue (inclusive) and maxValue (exclusive). (minValue == maxValue is allowed and returns minValue. Negative values are allowed.)
        /// </summary>
        public static iVector2 RandomUniform(System.Random random, iVector2 minValue, int maxValue) => new iVector2((int)random.Next((int)minValue.x, (int)maxValue), (int)random.Next((int)minValue.y, (int)maxValue));
        
        /// <summary>
        /// Returns a iVector2 with independent and identically distributed uniform integer values between minValue (inclusive) and maxValue (exclusive). (minValue == maxValue is allowed and returns minValue. Negative values are allowed.)
        /// </summary>
        public static iVector2 RandomUniform(System.Random random, int minValue, iVector2 maxValue) => new iVector2((int)random.Next((int)minValue, (int)maxValue.x), (int)random.Next((int)minValue, (int)maxValue.y));
        
        /// <summary>
        /// Returns a iVector2 with independent and identically distributed uniform integer values between minValue (inclusive) and maxValue (exclusive). (minValue == maxValue is allowed and returns minValue. Negative values are allowed.)
        /// </summary>
        public static iVector2 RandomUniform(System.Random random, int minValue, int maxValue) => new iVector2((int)random.Next((int)minValue, (int)maxValue));
        
        /// <summary>
        /// Returns a iVector2 with independent and identically distributed integer values according to a poisson distribution with given lambda parameter.
        /// </summary>
        public static iVector2 RandomPoisson(System.Random random, dVector2 lambda) => new iVector2((int)lambda.x.GetPoisson(random), (int)lambda.y.GetPoisson(random));
        
        /// <summary>
        /// Returns a iVector2 with independent and identically distributed integer values according to a poisson distribution with given lambda parameter.
        /// </summary>
        public static iVector2 RandomPoisson(System.Random random, double lambda) => new iVector2((int)lambda.GetPoisson(random));

        #endregion


        #region Component-Wise Operator Overloads
        
        /// <summary>
        /// Returns a bVector2 from component-wise application of operator&lt; (lhs &lt; rhs).
        /// </summary>
        public static bVector2 operator<(iVector2 lhs, iVector2 rhs) => new bVector2(lhs.x < rhs.x, lhs.y < rhs.y);
        
        /// <summary>
        /// Returns a bVector2 from component-wise application of operator&lt; (lhs &lt; rhs).
        /// </summary>
        public static bVector2 operator<(iVector2 lhs, int rhs) => new bVector2(lhs.x < rhs, lhs.y < rhs);
        
        /// <summary>
        /// Returns a bVector2 from component-wise application of operator&lt; (lhs &lt; rhs).
        /// </summary>
        public static bVector2 operator<(int lhs, iVector2 rhs) => new bVector2(lhs < rhs.x, lhs < rhs.y);
        
        /// <summary>
        /// Returns a bVector2 from component-wise application of operator&lt;= (lhs &lt;= rhs).
        /// </summary>
        public static bVector2 operator<=(iVector2 lhs, iVector2 rhs) => new bVector2(lhs.x <= rhs.x, lhs.y <= rhs.y);
        
        /// <summary>
        /// Returns a bVector2 from component-wise application of operator&lt;= (lhs &lt;= rhs).
        /// </summary>
        public static bVector2 operator<=(iVector2 lhs, int rhs) => new bVector2(lhs.x <= rhs, lhs.y <= rhs);
        
        /// <summary>
        /// Returns a bVector2 from component-wise application of operator&lt;= (lhs &lt;= rhs).
        /// </summary>
        public static bVector2 operator<=(int lhs, iVector2 rhs) => new bVector2(lhs <= rhs.x, lhs <= rhs.y);
        
        /// <summary>
        /// Returns a bVector2 from component-wise application of operator&gt; (lhs &gt; rhs).
        /// </summary>
        public static bVector2 operator>(iVector2 lhs, iVector2 rhs) => new bVector2(lhs.x > rhs.x, lhs.y > rhs.y);
        
        /// <summary>
        /// Returns a bVector2 from component-wise application of operator&gt; (lhs &gt; rhs).
        /// </summary>
        public static bVector2 operator>(iVector2 lhs, int rhs) => new bVector2(lhs.x > rhs, lhs.y > rhs);
        
        /// <summary>
        /// Returns a bVector2 from component-wise application of operator&gt; (lhs &gt; rhs).
        /// </summary>
        public static bVector2 operator>(int lhs, iVector2 rhs) => new bVector2(lhs > rhs.x, lhs > rhs.y);
        
        /// <summary>
        /// Returns a bVector2 from component-wise application of operator&gt;= (lhs &gt;= rhs).
        /// </summary>
        public static bVector2 operator>=(iVector2 lhs, iVector2 rhs) => new bVector2(lhs.x >= rhs.x, lhs.y >= rhs.y);
        
        /// <summary>
        /// Returns a bVector2 from component-wise application of operator&gt;= (lhs &gt;= rhs).
        /// </summary>
        public static bVector2 operator>=(iVector2 lhs, int rhs) => new bVector2(lhs.x >= rhs, lhs.y >= rhs);
        
        /// <summary>
        /// Returns a bVector2 from component-wise application of operator&gt;= (lhs &gt;= rhs).
        /// </summary>
        public static bVector2 operator>=(int lhs, iVector2 rhs) => new bVector2(lhs >= rhs.x, lhs >= rhs.y);
        
        /// <summary>
        /// Returns a iVector2 from component-wise application of operator+ (lhs + rhs).
        /// </summary>
        public static iVector2 operator+(iVector2 lhs, iVector2 rhs) => new iVector2(lhs.x + rhs.x, lhs.y + rhs.y);
        
        /// <summary>
        /// Returns a iVector2 from component-wise application of operator+ (lhs + rhs).
        /// </summary>
        public static iVector2 operator+(iVector2 lhs, int rhs) => new iVector2(lhs.x + rhs, lhs.y + rhs);
        
        /// <summary>
        /// Returns a iVector2 from component-wise application of operator+ (lhs + rhs).
        /// </summary>
        public static iVector2 operator+(int lhs, iVector2 rhs) => new iVector2(lhs + rhs.x, lhs + rhs.y);
        
        /// <summary>
        /// Returns a iVector2 from component-wise application of operator- (lhs - rhs).
        /// </summary>
        public static iVector2 operator-(iVector2 lhs, iVector2 rhs) => new iVector2(lhs.x - rhs.x, lhs.y - rhs.y);
        
        /// <summary>
        /// Returns a iVector2 from component-wise application of operator- (lhs - rhs).
        /// </summary>
        public static iVector2 operator-(iVector2 lhs, int rhs) => new iVector2(lhs.x - rhs, lhs.y - rhs);
        
        /// <summary>
        /// Returns a iVector2 from component-wise application of operator- (lhs - rhs).
        /// </summary>
        public static iVector2 operator-(int lhs, iVector2 rhs) => new iVector2(lhs - rhs.x, lhs - rhs.y);
        
        /// <summary>
        /// Returns a iVector2 from component-wise application of operator* (lhs * rhs).
        /// </summary>
        public static iVector2 operator*(iVector2 lhs, iVector2 rhs) => new iVector2(lhs.x * rhs.x, lhs.y * rhs.y);
        
        /// <summary>
        /// Returns a iVector2 from component-wise application of operator* (lhs * rhs).
        /// </summary>
        public static iVector2 operator*(iVector2 lhs, int rhs) => new iVector2(lhs.x * rhs, lhs.y * rhs);
        
        /// <summary>
        /// Returns a iVector2 from component-wise application of operator* (lhs * rhs).
        /// </summary>
        public static iVector2 operator*(int lhs, iVector2 rhs) => new iVector2(lhs * rhs.x, lhs * rhs.y);
        
        /// <summary>
        /// Returns a iVector2 from component-wise application of operator/ (lhs / rhs).
        /// </summary>
        public static iVector2 operator/(iVector2 lhs, iVector2 rhs) => new iVector2(lhs.x / rhs.x, lhs.y / rhs.y);
        
        /// <summary>
        /// Returns a iVector2 from component-wise application of operator/ (lhs / rhs).
        /// </summary>
        public static iVector2 operator/(iVector2 lhs, int rhs) => new iVector2(lhs.x / rhs, lhs.y / rhs);
        
        /// <summary>
        /// Returns a iVector2 from component-wise application of operator/ (lhs / rhs).
        /// </summary>
        public static iVector2 operator/(int lhs, iVector2 rhs) => new iVector2(lhs / rhs.x, lhs / rhs.y);
        
        /// <summary>
        /// Returns a iVector2 from component-wise application of operator+ (identity).
        /// </summary>
        public static iVector2 operator+(iVector2 v) => v;
        
        /// <summary>
        /// Returns a iVector2 from component-wise application of operator- (-v).
        /// </summary>
        public static iVector2 operator-(iVector2 v) => new iVector2(-v.x, -v.y);
        
        /// <summary>
        /// Returns a iVector2 from component-wise application of operator~ (~v).
        /// </summary>
        public static iVector2 operator~(iVector2 v) => new iVector2(~v.x, ~v.y);
        
        /// <summary>
        /// Returns a iVector2 from component-wise application of operator^ (lhs ^ rhs).
        /// </summary>
        public static iVector2 operator^(iVector2 lhs, iVector2 rhs) => new iVector2(lhs.x ^ rhs.x, lhs.y ^ rhs.y);
        
        /// <summary>
        /// Returns a iVector2 from component-wise application of operator^ (lhs ^ rhs).
        /// </summary>
        public static iVector2 operator^(iVector2 lhs, int rhs) => new iVector2(lhs.x ^ rhs, lhs.y ^ rhs);
        
        /// <summary>
        /// Returns a iVector2 from component-wise application of operator^ (lhs ^ rhs).
        /// </summary>
        public static iVector2 operator^(int lhs, iVector2 rhs) => new iVector2(lhs ^ rhs.x, lhs ^ rhs.y);
        
        /// <summary>
        /// Returns a iVector2 from component-wise application of operator| (lhs | rhs).
        /// </summary>
        public static iVector2 operator|(iVector2 lhs, iVector2 rhs) => new iVector2(lhs.x | rhs.x, lhs.y | rhs.y);
        
        /// <summary>
        /// Returns a iVector2 from component-wise application of operator| (lhs | rhs).
        /// </summary>
        public static iVector2 operator|(iVector2 lhs, int rhs) => new iVector2(lhs.x | rhs, lhs.y | rhs);
        
        /// <summary>
        /// Returns a iVector2 from component-wise application of operator| (lhs | rhs).
        /// </summary>
        public static iVector2 operator|(int lhs, iVector2 rhs) => new iVector2(lhs | rhs.x, lhs | rhs.y);
        
        /// <summary>
        /// Returns a iVector2 from component-wise application of operator&amp; (lhs &amp; rhs).
        /// </summary>
        public static iVector2 operator&(iVector2 lhs, iVector2 rhs) => new iVector2(lhs.x & rhs.x, lhs.y & rhs.y);
        
        /// <summary>
        /// Returns a iVector2 from component-wise application of operator&amp; (lhs &amp; rhs).
        /// </summary>
        public static iVector2 operator&(iVector2 lhs, int rhs) => new iVector2(lhs.x & rhs, lhs.y & rhs);
        
        /// <summary>
        /// Returns a iVector2 from component-wise application of operator&amp; (lhs &amp; rhs).
        /// </summary>
        public static iVector2 operator&(int lhs, iVector2 rhs) => new iVector2(lhs & rhs.x, lhs & rhs.y);
        
        /// <summary>
        /// Returns a iVector2 from component-wise application of operator&lt;&lt; (lhs &lt;&lt; rhs).
        /// </summary>
        public static iVector2 operator<<(iVector2 lhs, int rhs) => new iVector2(lhs.x << rhs, lhs.y << rhs);
        
        /// <summary>
        /// Returns a iVector2 from component-wise application of operator&gt;&gt; (lhs &gt;&gt; rhs).
        /// </summary>
        public static iVector2 operator>>(iVector2 lhs, int rhs) => new iVector2(lhs.x >> rhs, lhs.y >> rhs);

        #endregion

    }
}
