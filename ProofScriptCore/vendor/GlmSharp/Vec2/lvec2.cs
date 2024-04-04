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
    /// A Vectortor of type long with 2 components.
    /// </summary>
    [RegisterCoreClassStruct]
    
    [StructLayout(LayoutKind.Sequential)]
    public struct lVector2 : IReadOnlyList<long>, IEquatable<lVector2>
    {

        #region Fields
        
        /// <summary>
        /// x-component
        /// </summary>
        
        public long x;
        
        /// <summary>
        /// y-component
        /// </summary>
        
        public long y;

        #endregion


        #region Constructors
        
        /// <summary>
        /// Component-wise constructor
        /// </summary>
        public lVector2(long x, long y)
        {
            this.x = x;
            this.y = y;
        }
        
        /// <summary>
        /// all-same-value constructor
        /// </summary>
        public lVector2(long v)
        {
            this.x = v;
            this.y = v;
        }
        
        /// <summary>
        /// from-Vectortor constructor
        /// </summary>
        public lVector2(lVector2 v)
        {
            this.x = v.x;
            this.y = v.y;
        }
        
        /// <summary>
        /// from-Vectortor constructor (additional fields are truncated)
        /// </summary>
        public lVector2(lVector3 v)
        {
            this.x = v.x;
            this.y = v.y;
        }
        
        /// <summary>
        /// from-Vectortor constructor (additional fields are truncated)
        /// </summary>
        public lVector2(lVector4 v)
        {
            this.x = v.x;
            this.y = v.y;
        }
        
        /// <summary>
        /// From-array/list constructor (superfluous values are ignored, missing values are zero-filled).
        /// </summary>
        public lVector2(IReadOnlyList<long> v)
        {
            var c = v.Count;
            this.x = c < 0 ? 0 : v[0];
            this.y = c < 1 ? 0 : v[1];
        }
        
        /// <summary>
        /// Generic from-array constructor (superfluous values are ignored, missing values are zero-filled).
        /// </summary>
        public lVector2(Object[] v)
        {
            var c = v.Length;
            this.x = c < 0 ? 0 : (long)v[0];
            this.y = c < 1 ? 0 : (long)v[1];
        }
        
        /// <summary>
        /// From-array constructor (superfluous values are ignored, missing values are zero-filled).
        /// </summary>
        public lVector2(long[] v)
        {
            var c = v.Length;
            this.x = c < 0 ? 0 : v[0];
            this.y = c < 1 ? 0 : v[1];
        }
        
        /// <summary>
        /// From-array constructor with base index (superfluous values are ignored, missing values are zero-filled).
        /// </summary>
        public lVector2(long[] v, int startIndex)
        {
            var c = v.Length;
            this.x = c + startIndex < 0 ? 0 : v[0 + startIndex];
            this.y = c + startIndex < 1 ? 0 : v[1 + startIndex];
        }
        
        /// <summary>
        /// From-IEnumerable constructor (superfluous values are ignored, missing values are zero-filled).
        /// </summary>
        public lVector2(IEnumerable<long> v)
            : this(v.ToArray())
        {
        }

        #endregion


        #region Implicit Operators
        
        /// <summary>
        /// Implicitly converts this to a decVector2.
        /// </summary>
        public static implicit operator decVector2(lVector2 v) => new decVector2((decimal)v.x, (decimal)v.y);

        #endregion


        #region Explicit Operators
        
        /// <summary>
        /// Explicitly converts this to a iVector2.
        /// </summary>
        public static explicit operator iVector2(lVector2 v) => new iVector2((int)v.x, (int)v.y);
        
        /// <summary>
        /// Explicitly converts this to a iVector3. (Higher components are zeroed)
        /// </summary>
        public static explicit operator iVector3(lVector2 v) => new iVector3((int)v.x, (int)v.y, 0);
        
        /// <summary>
        /// Explicitly converts this to a iVector4. (Higher components are zeroed)
        /// </summary>
        public static explicit operator iVector4(lVector2 v) => new iVector4((int)v.x, (int)v.y, 0, 0);
        
        /// <summary>
        /// Explicitly converts this to a uVector2.
        /// </summary>
        public static explicit operator uVector2(lVector2 v) => new uVector2((uint)v.x, (uint)v.y);
        
        /// <summary>
        /// Explicitly converts this to a uVector3. (Higher components are zeroed)
        /// </summary>
        public static explicit operator uVector3(lVector2 v) => new uVector3((uint)v.x, (uint)v.y, 0u);
        
        /// <summary>
        /// Explicitly converts this to a uVector4. (Higher components are zeroed)
        /// </summary>
        public static explicit operator uVector4(lVector2 v) => new uVector4((uint)v.x, (uint)v.y, 0u, 0u);
        
        /// <summary>
        /// Explicitly converts this to a Vector2.
        /// </summary>
        public static explicit operator Vector2(lVector2 v) => new Vector2((float)v.x, (float)v.y);
        
        /// <summary>
        /// Explicitly converts this to a Vector3. (Higher components are zeroed)
        /// </summary>
        public static explicit operator Vector3(lVector2 v) => new Vector3((float)v.x, (float)v.y, 0f);
        
        /// <summary>
        /// Explicitly converts this to a Vector4. (Higher components are zeroed)
        /// </summary>
        public static explicit operator Vector4(lVector2 v) => new Vector4((float)v.x, (float)v.y, 0f, 0f);
        
        /// <summary>
        /// Explicitly converts this to a hVector2.
        /// </summary>
        public static explicit operator hVector2(lVector2 v) => new hVector2((Half)v.x, (Half)v.y);
        
        /// <summary>
        /// Explicitly converts this to a hVector3. (Higher components are zeroed)
        /// </summary>
        public static explicit operator hVector3(lVector2 v) => new hVector3((Half)v.x, (Half)v.y, Half.Zero);
        
        /// <summary>
        /// Explicitly converts this to a hVector4. (Higher components are zeroed)
        /// </summary>
        public static explicit operator hVector4(lVector2 v) => new hVector4((Half)v.x, (Half)v.y, Half.Zero, Half.Zero);
        
        /// <summary>
        /// Explicitly converts this to a dVector2.
        /// </summary>
        public static explicit operator dVector2(lVector2 v) => new dVector2((double)v.x, (double)v.y);
        
        /// <summary>
        /// Explicitly converts this to a dVector3. (Higher components are zeroed)
        /// </summary>
        public static explicit operator dVector3(lVector2 v) => new dVector3((double)v.x, (double)v.y, 0.0);
        
        /// <summary>
        /// Explicitly converts this to a dVector4. (Higher components are zeroed)
        /// </summary>
        public static explicit operator dVector4(lVector2 v) => new dVector4((double)v.x, (double)v.y, 0.0, 0.0);
        
        /// <summary>
        /// Explicitly converts this to a decVector3. (Higher components are zeroed)
        /// </summary>
        public static explicit operator decVector3(lVector2 v) => new decVector3((decimal)v.x, (decimal)v.y, 0m);
        
        /// <summary>
        /// Explicitly converts this to a decVector4. (Higher components are zeroed)
        /// </summary>
        public static explicit operator decVector4(lVector2 v) => new decVector4((decimal)v.x, (decimal)v.y, 0m, 0m);
        
        /// <summary>
        /// Explicitly converts this to a cVector2.
        /// </summary>
//public static explicit operator cVector2(lVector2 v) => new cVector2((Complex)v.x, (Complex)v.y);
        
        /// <summary>
        /// Explicitly converts this to a cVector3. (Higher components are zeroed)
        /// </summary>
        //public static explicit operator cVector3(lVector2 v) => new cVector3((Complex)v.x, (Complex)v.y, Complex.Zero);
        
        /// <summary>
        /// Explicitly converts this to a cVector4. (Higher components are zeroed)
        /// </summary>
        //public static explicit operator cVector4(lVector2 v) => new cVector4((Complex)v.x, (Complex)v.y, Complex.Zero, Complex.Zero);
        
        /// <summary>
        /// Explicitly converts this to a lVector3. (Higher components are zeroed)
        /// </summary>
        public static explicit operator lVector3(lVector2 v) => new lVector3((long)v.x, (long)v.y, 0);
        
        /// <summary>
        /// Explicitly converts this to a lVector4. (Higher components are zeroed)
        /// </summary>
        public static explicit operator lVector4(lVector2 v) => new lVector4((long)v.x, (long)v.y, 0, 0);
        
        /// <summary>
        /// Explicitly converts this to a bVector2.
        /// </summary>
        public static explicit operator bVector2(lVector2 v) => new bVector2(v.x != 0, v.y != 0);
        
        /// <summary>
        /// Explicitly converts this to a bVector3. (Higher components are zeroed)
        /// </summary>
        public static explicit operator bVector3(lVector2 v) => new bVector3(v.x != 0, v.y != 0, false);
        
        /// <summary>
        /// Explicitly converts this to a bVector4. (Higher components are zeroed)
        /// </summary>
        public static explicit operator bVector4(lVector2 v) => new bVector4(v.x != 0, v.y != 0, false, false);
        
        /// <summary>
        /// Explicitly converts this to a long array.
        /// </summary>
        public static explicit operator long[](lVector2 v) => new [] { v.x, v.y };
        
        /// <summary>
        /// Explicitly converts this to a generic object array.
        /// </summary>
        public static explicit operator Object[](lVector2 v) => new Object[] { v.x, v.y };

        #endregion


        #region Indexer
        
        /// <summary>
        /// Gets/Sets a specific indexed component (a bit slower than direct access).
        /// </summary>
        public long this[int index]
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
        public swizzle_lVector2 swizzle => new swizzle_lVector2(x, y);
        
        /// <summary>
        /// Gets or sets the specified subset of components. For more advanced (read-only) swizzling, use the .swizzle property.
        /// </summary>
        public lVector2 xy
        {
            get
            {
                return new lVector2(x, y);
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
        public lVector2 rg
        {
            get
            {
                return new lVector2(x, y);
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
        public long r
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
        public long g
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
        public long[] Values => new[] { x, y };
        
        /// <summary>
        /// Returns the number of components (2).
        /// </summary>
        public int Count => 2;
        
        /// <summary>
        /// Returns the minimal component of this Vectortor.
        /// </summary>
        public long MinElement => Math.Min(x, y);
        
        /// <summary>
        /// Returns the maximal component of this Vectortor.
        /// </summary>
        public long MaxElement => Math.Max(x, y);
        
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
        public long Sum => (x + y);
        
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

        #endregion


        #region Static Properties
        
        /// <summary>
        /// Predefined all-zero Vectortor
        /// </summary>
        public static lVector2 Zero { get; } = new lVector2(0, 0);
        
        /// <summary>
        /// Predefined all-ones Vectortor
        /// </summary>
        public static lVector2 Ones { get; } = new lVector2(1, 1);
        
        /// <summary>
        /// Predefined unit-X Vectortor
        /// </summary>
        public static lVector2 UnitX { get; } = new lVector2(1, 0);
        
        /// <summary>
        /// Predefined unit-Y Vectortor
        /// </summary>
        public static lVector2 UnitY { get; } = new lVector2(0, 1);
        
        /// <summary>
        /// Predefined all-MaxValue Vectortor
        /// </summary>
        public static lVector2 MaxValue { get; } = new lVector2(long.MaxValue, long.MaxValue);
        
        /// <summary>
        /// Predefined all-MinValue Vectortor
        /// </summary>
        public static lVector2 MinValue { get; } = new lVector2(long.MinValue, long.MinValue);

        #endregion


        #region Operators
        
        /// <summary>
        /// Returns true iff this equals rhs component-wise.
        /// </summary>
        public static bool operator==(lVector2 lhs, lVector2 rhs) => lhs.Equals(rhs);
        
        /// <summary>
        /// Returns true iff this does not equal rhs (component-wise).
        /// </summary>
        public static bool operator!=(lVector2 lhs, lVector2 rhs) => !lhs.Equals(rhs);

        #endregion


        #region Functions
        
        /// <summary>
        /// Returns an enumerator that iterates through all components.
        /// </summary>
        public IEnumerator<long> GetEnumerator()
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
        public bool Equals(lVector2 rhs) => (x.Equals(rhs.x) && y.Equals(rhs.y));
        
        /// <summary>
        /// Returns true iff this equals rhs type- and component-wise.
        /// </summary>
        public override bool Equals(object obj)
        {
            if (ReferenceEquals(null, obj)) return false;
            return obj is lVector2 && Equals((lVector2) obj);
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

        #endregion


        #region Static Functions
        
        /// <summary>
        /// Converts the string representation of the Vectortor into a Vectortor representation (using ', ' as a separator).
        /// </summary>
        public static lVector2 Parse(string s) => Parse(s, ", ");
        
        /// <summary>
        /// Converts the string representation of the Vectortor into a Vectortor representation (using a designated separator).
        /// </summary>
        public static lVector2 Parse(string s, string sep)
        {
            var kvp = s.Split(new[] { sep }, StringSplitOptions.None);
            if (kvp.Length != 2) throw new FormatException("input has not exactly 2 parts");
            return new lVector2(long.Parse(kvp[0].Trim()), long.Parse(kvp[1].Trim()));
        }
        
        /// <summary>
        /// Converts the string representation of the Vectortor into a Vectortor representation (using a designated separator and a type provider).
        /// </summary>
        public static lVector2 Parse(string s, string sep, IFormatProvider provider)
        {
            var kvp = s.Split(new[] { sep }, StringSplitOptions.None);
            if (kvp.Length != 2) throw new FormatException("input has not exactly 2 parts");
            return new lVector2(long.Parse(kvp[0].Trim(), provider), long.Parse(kvp[1].Trim(), provider));
        }
        
        /// <summary>
        /// Converts the string representation of the Vectortor into a Vectortor representation (using a designated separator and a number style).
        /// </summary>
        public static lVector2 Parse(string s, string sep, NumberStyles style)
        {
            var kvp = s.Split(new[] { sep }, StringSplitOptions.None);
            if (kvp.Length != 2) throw new FormatException("input has not exactly 2 parts");
            return new lVector2(long.Parse(kvp[0].Trim(), style), long.Parse(kvp[1].Trim(), style));
        }
        
        /// <summary>
        /// Converts the string representation of the Vectortor into a Vectortor representation (using a designated separator and a number style and a forMatrix provider).
        /// </summary>
        public static lVector2 Parse(string s, string sep, NumberStyles style, IFormatProvider provider)
        {
            var kvp = s.Split(new[] { sep }, StringSplitOptions.None);
            if (kvp.Length != 2) throw new FormatException("input has not exactly 2 parts");
            return new lVector2(long.Parse(kvp[0].Trim(), style, provider), long.Parse(kvp[1].Trim(), style, provider));
        }
        
        /// <summary>
        /// Tries to convert the string representation of the Vectortor into a Vectortor representation (using ', ' as a separator), returns false if string was invalid.
        /// </summary>
        public static bool TryParse(string s, out lVector2 result) => TryParse(s, ", ", out result);
        
        /// <summary>
        /// Tries to convert the string representation of the Vectortor into a Vectortor representation (using a designated separator), returns false if string was invalid.
        /// </summary>
        public static bool TryParse(string s, string sep, out lVector2 result)
        {
            result = Zero;
            if (string.IsNullOrEmpty(s)) return false;
            var kvp = s.Split(new[] { sep }, StringSplitOptions.None);
            if (kvp.Length != 2) return false;
            long x = 0, y = 0;
            var ok = (long.TryParse(kvp[0].Trim(), out x) && long.TryParse(kvp[1].Trim(), out y));
            result = ok ? new lVector2(x, y) : Zero;
            return ok;
        }
        
        /// <summary>
        /// Tries to convert the string representation of the Vectortor into a Vectortor representation (using a designated separator and a number style and a forMatrix provider), returns false if string was invalid.
        /// </summary>
        public static bool TryParse(string s, string sep, NumberStyles style, IFormatProvider provider, out lVector2 result)
        {
            result = Zero;
            if (string.IsNullOrEmpty(s)) return false;
            var kvp = s.Split(new[] { sep }, StringSplitOptions.None);
            if (kvp.Length != 2) return false;
            long x = 0, y = 0;
            var ok = (long.TryParse(kvp[0].Trim(), style, provider, out x) && long.TryParse(kvp[1].Trim(), style, provider, out y));
            result = ok ? new lVector2(x, y) : Zero;
            return ok;
        }
        
        /// <summary>
        /// OuterProduct treats the first parameter c as a column Vectortor (Matrix with one column) and the second parameter r as a row Vectortor (Matrix with one row) and does a linear algebraic Matrix multiply c * r, yielding a Matrix whose number of rows is the number of components in c and whose number of columns is the number of components in r.
        /// </summary>
        public static lMatrix2 OuterProduct(lVector2 c, lVector2 r) => new lMatrix2(c.x * r.x, c.y * r.x, c.x * r.y, c.y * r.y);
        
        /// <summary>
        /// OuterProduct treats the first parameter c as a column Vectortor (Matrix with one column) and the second parameter r as a row Vectortor (Matrix with one row) and does a linear algebraic Matrix multiply c * r, yielding a Matrix whose number of rows is the number of components in c and whose number of columns is the number of components in r.
        /// </summary>
        public static lMatrix2x3 OuterProduct(lVector3 c, lVector2 r) => new lMatrix2x3(c.x * r.x, c.y * r.x, c.z * r.x, c.x * r.y, c.y * r.y, c.z * r.y);
        
        /// <summary>
        /// OuterProduct treats the first parameter c as a column Vectortor (Matrix with one column) and the second parameter r as a row Vectortor (Matrix with one row) and does a linear algebraic Matrix multiply c * r, yielding a Matrix whose number of rows is the number of components in c and whose number of columns is the number of components in r.
        /// </summary>
        public static lMatrix3x2 OuterProduct(lVector2 c, lVector3 r) => new lMatrix3x2(c.x * r.x, c.y * r.x, c.x * r.y, c.y * r.y, c.x * r.z, c.y * r.z);
        
        /// <summary>
        /// OuterProduct treats the first parameter c as a column Vectortor (Matrix with one column) and the second parameter r as a row Vectortor (Matrix with one row) and does a linear algebraic Matrix multiply c * r, yielding a Matrix whose number of rows is the number of components in c and whose number of columns is the number of components in r.
        /// </summary>
        public static lMatrix2x4 OuterProduct(lVector4 c, lVector2 r) => new lMatrix2x4(c.x * r.x, c.y * r.x, c.z * r.x, c.w * r.x, c.x * r.y, c.y * r.y, c.z * r.y, c.w * r.y);
        
        /// <summary>
        /// OuterProduct treats the first parameter c as a column Vectortor (Matrix with one column) and the second parameter r as a row Vectortor (Matrix with one row) and does a linear algebraic Matrix multiply c * r, yielding a Matrix whose number of rows is the number of components in c and whose number of columns is the number of components in r.
        /// </summary>
        public static lMatrix4x2 OuterProduct(lVector2 c, lVector4 r) => new lMatrix4x2(c.x * r.x, c.y * r.x, c.x * r.y, c.y * r.y, c.x * r.z, c.y * r.z, c.x * r.w, c.y * r.w);
        
        /// <summary>
        /// Returns the inner product (dot product, scalar product) of the two Vectortors.
        /// </summary>
        public static long Dot(lVector2 lhs, lVector2 rhs) => (lhs.x * rhs.x + lhs.y * rhs.y);
        
        /// <summary>
        /// Returns the euclidean distance between the two Vectortors.
        /// </summary>
        public static double Distance(lVector2 lhs, lVector2 rhs) => (lhs - rhs).Length;
        
        /// <summary>
        /// Returns the squared euclidean distance between the two Vectortors.
        /// </summary>
        public static double DistanceSqr(lVector2 lhs, lVector2 rhs) => (lhs - rhs).LengthSqr;
        
        /// <summary>
        /// Calculate the reflection direction for an incident Vectortor (N should be normalized in order to achieve the desired result).
        /// </summary>
        public static lVector2 Reflect(lVector2 I, lVector2 N) => I - 2 * Dot(N, I) * N;
        
        /// <summary>
        /// Calculate the refraction direction for an incident Vectortor (The input parameters I and N should be normalized in order to achieve the desired result).
        /// </summary>
        public static lVector2 Refract(lVector2 I, lVector2 N, long eta)
        {
            var dNI = Dot(N, I);
            var k = 1 - eta * eta * (1 - dNI * dNI);
            if (k < 0) return Zero;
            return eta * I - (eta * dNI + (long)Math.Sqrt(k)) * N;
        }
        
        /// <summary>
        /// Returns a Vectortor pointing in the same direction as another (faceforward orients a Vectortor to point away from a surface as defined by its normal. If dot(Nref, I) is negative faceforward returns N, otherwise it returns -N).
        /// </summary>
        public static lVector2 FaceForward(lVector2 N, lVector2 I, lVector2 Nref) => Dot(Nref, I) < 0 ? N : -N;
        
        /// <summary>
        /// Returns the length of the outer product (cross product, Vectortor product) of the two Vectortors.
        /// </summary>
        public static long Cross(lVector2 l, lVector2 r) => l.x * r.y - l.y * r.x;
        
        /// <summary>
        /// Returns a lVector2 with independent and identically distributed uniform integer values between 0 (inclusive) and int.MaxValue (exclusive).
        /// </summary>
        public static lVector2 Random(System.Random random) => new lVector2((long)random.Next(), (long)random.Next());

        #endregion


        #region Component-Wise Static Functions
        
        /// <summary>
        /// Returns a bVector2 from component-wise application of Equal (lhs == rhs).
        /// </summary>
        public static bVector2 Equal(lVector2 lhs, lVector2 rhs) => new bVector2(lhs.x == rhs.x, lhs.y == rhs.y);
        
        /// <summary>
        /// Returns a bVector2 from component-wise application of Equal (lhs == rhs).
        /// </summary>
        public static bVector2 Equal(lVector2 lhs, long rhs) => new bVector2(lhs.x == rhs, lhs.y == rhs);
        
        /// <summary>
        /// Returns a bVector2 from component-wise application of Equal (lhs == rhs).
        /// </summary>
        public static bVector2 Equal(long lhs, lVector2 rhs) => new bVector2(lhs == rhs.x, lhs == rhs.y);
        
        /// <summary>
        /// Returns a bVector from the application of Equal (lhs == rhs).
        /// </summary>
        public static bVector2 Equal(long lhs, long rhs) => new bVector2(lhs == rhs);
        
        /// <summary>
        /// Returns a bVector2 from component-wise application of NotEqual (lhs != rhs).
        /// </summary>
        public static bVector2 NotEqual(lVector2 lhs, lVector2 rhs) => new bVector2(lhs.x != rhs.x, lhs.y != rhs.y);
        
        /// <summary>
        /// Returns a bVector2 from component-wise application of NotEqual (lhs != rhs).
        /// </summary>
        public static bVector2 NotEqual(lVector2 lhs, long rhs) => new bVector2(lhs.x != rhs, lhs.y != rhs);
        
        /// <summary>
        /// Returns a bVector2 from component-wise application of NotEqual (lhs != rhs).
        /// </summary>
        public static bVector2 NotEqual(long lhs, lVector2 rhs) => new bVector2(lhs != rhs.x, lhs != rhs.y);
        
        /// <summary>
        /// Returns a bVector from the application of NotEqual (lhs != rhs).
        /// </summary>
        public static bVector2 NotEqual(long lhs, long rhs) => new bVector2(lhs != rhs);
        
        /// <summary>
        /// Returns a bVector2 from component-wise application of GreaterThan (lhs &gt; rhs).
        /// </summary>
        public static bVector2 GreaterThan(lVector2 lhs, lVector2 rhs) => new bVector2(lhs.x > rhs.x, lhs.y > rhs.y);
        
        /// <summary>
        /// Returns a bVector2 from component-wise application of GreaterThan (lhs &gt; rhs).
        /// </summary>
        public static bVector2 GreaterThan(lVector2 lhs, long rhs) => new bVector2(lhs.x > rhs, lhs.y > rhs);
        
        /// <summary>
        /// Returns a bVector2 from component-wise application of GreaterThan (lhs &gt; rhs).
        /// </summary>
        public static bVector2 GreaterThan(long lhs, lVector2 rhs) => new bVector2(lhs > rhs.x, lhs > rhs.y);
        
        /// <summary>
        /// Returns a bVector from the application of GreaterThan (lhs &gt; rhs).
        /// </summary>
        public static bVector2 GreaterThan(long lhs, long rhs) => new bVector2(lhs > rhs);
        
        /// <summary>
        /// Returns a bVector2 from component-wise application of GreaterThanEqual (lhs &gt;= rhs).
        /// </summary>
        public static bVector2 GreaterThanEqual(lVector2 lhs, lVector2 rhs) => new bVector2(lhs.x >= rhs.x, lhs.y >= rhs.y);
        
        /// <summary>
        /// Returns a bVector2 from component-wise application of GreaterThanEqual (lhs &gt;= rhs).
        /// </summary>
        public static bVector2 GreaterThanEqual(lVector2 lhs, long rhs) => new bVector2(lhs.x >= rhs, lhs.y >= rhs);
        
        /// <summary>
        /// Returns a bVector2 from component-wise application of GreaterThanEqual (lhs &gt;= rhs).
        /// </summary>
        public static bVector2 GreaterThanEqual(long lhs, lVector2 rhs) => new bVector2(lhs >= rhs.x, lhs >= rhs.y);
        
        /// <summary>
        /// Returns a bVector from the application of GreaterThanEqual (lhs &gt;= rhs).
        /// </summary>
        public static bVector2 GreaterThanEqual(long lhs, long rhs) => new bVector2(lhs >= rhs);
        
        /// <summary>
        /// Returns a bVector2 from component-wise application of LesserThan (lhs &lt; rhs).
        /// </summary>
        public static bVector2 LesserThan(lVector2 lhs, lVector2 rhs) => new bVector2(lhs.x < rhs.x, lhs.y < rhs.y);
        
        /// <summary>
        /// Returns a bVector2 from component-wise application of LesserThan (lhs &lt; rhs).
        /// </summary>
        public static bVector2 LesserThan(lVector2 lhs, long rhs) => new bVector2(lhs.x < rhs, lhs.y < rhs);
        
        /// <summary>
        /// Returns a bVector2 from component-wise application of LesserThan (lhs &lt; rhs).
        /// </summary>
        public static bVector2 LesserThan(long lhs, lVector2 rhs) => new bVector2(lhs < rhs.x, lhs < rhs.y);
        
        /// <summary>
        /// Returns a bVector from the application of LesserThan (lhs &lt; rhs).
        /// </summary>
        public static bVector2 LesserThan(long lhs, long rhs) => new bVector2(lhs < rhs);
        
        /// <summary>
        /// Returns a bVector2 from component-wise application of LesserThanEqual (lhs &lt;= rhs).
        /// </summary>
        public static bVector2 LesserThanEqual(lVector2 lhs, lVector2 rhs) => new bVector2(lhs.x <= rhs.x, lhs.y <= rhs.y);
        
        /// <summary>
        /// Returns a bVector2 from component-wise application of LesserThanEqual (lhs &lt;= rhs).
        /// </summary>
        public static bVector2 LesserThanEqual(lVector2 lhs, long rhs) => new bVector2(lhs.x <= rhs, lhs.y <= rhs);
        
        /// <summary>
        /// Returns a bVector2 from component-wise application of LesserThanEqual (lhs &lt;= rhs).
        /// </summary>
        public static bVector2 LesserThanEqual(long lhs, lVector2 rhs) => new bVector2(lhs <= rhs.x, lhs <= rhs.y);
        
        /// <summary>
        /// Returns a bVector from the application of LesserThanEqual (lhs &lt;= rhs).
        /// </summary>
        public static bVector2 LesserThanEqual(long lhs, long rhs) => new bVector2(lhs <= rhs);
        
        /// <summary>
        /// Returns a lVector2 from component-wise application of Abs (Math.Abs(v)).
        /// </summary>
        public static lVector2 Abs(lVector2 v) => new lVector2(Math.Abs(v.x), Math.Abs(v.y));
        
        /// <summary>
        /// Returns a lVector from the application of Abs (Math.Abs(v)).
        /// </summary>
        public static lVector2 Abs(long v) => new lVector2(Math.Abs(v));
        
        /// <summary>
        /// Returns a lVector2 from component-wise application of HermiteInterpolationOrder3 ((3 - 2 * v) * v * v).
        /// </summary>
        public static lVector2 HermiteInterpolationOrder3(lVector2 v) => new lVector2((3 - 2 * v.x) * v.x * v.x, (3 - 2 * v.y) * v.y * v.y);
        
        /// <summary>
        /// Returns a lVector from the application of HermiteInterpolationOrder3 ((3 - 2 * v) * v * v).
        /// </summary>
        public static lVector2 HermiteInterpolationOrder3(long v) => new lVector2((3 - 2 * v) * v * v);
        
        /// <summary>
        /// Returns a lVector2 from component-wise application of HermiteInterpolationOrder5 (((6 * v - 15) * v + 10) * v * v * v).
        /// </summary>
        public static lVector2 HermiteInterpolationOrder5(lVector2 v) => new lVector2(((6 * v.x - 15) * v.x + 10) * v.x * v.x * v.x, ((6 * v.y - 15) * v.y + 10) * v.y * v.y * v.y);
        
        /// <summary>
        /// Returns a lVector from the application of HermiteInterpolationOrder5 (((6 * v - 15) * v + 10) * v * v * v).
        /// </summary>
        public static lVector2 HermiteInterpolationOrder5(long v) => new lVector2(((6 * v - 15) * v + 10) * v * v * v);
        
        /// <summary>
        /// Returns a lVector2 from component-wise application of Sqr (v * v).
        /// </summary>
        public static lVector2 Sqr(lVector2 v) => new lVector2(v.x * v.x, v.y * v.y);
        
        /// <summary>
        /// Returns a lVector from the application of Sqr (v * v).
        /// </summary>
        public static lVector2 Sqr(long v) => new lVector2(v * v);
        
        /// <summary>
        /// Returns a lVector2 from component-wise application of Pow2 (v * v).
        /// </summary>
        public static lVector2 Pow2(lVector2 v) => new lVector2(v.x * v.x, v.y * v.y);
        
        /// <summary>
        /// Returns a lVector from the application of Pow2 (v * v).
        /// </summary>
        public static lVector2 Pow2(long v) => new lVector2(v * v);
        
        /// <summary>
        /// Returns a lVector2 from component-wise application of Pow3 (v * v * v).
        /// </summary>
        public static lVector2 Pow3(lVector2 v) => new lVector2(v.x * v.x * v.x, v.y * v.y * v.y);
        
        /// <summary>
        /// Returns a lVector from the application of Pow3 (v * v * v).
        /// </summary>
        public static lVector2 Pow3(long v) => new lVector2(v * v * v);
        
        /// <summary>
        /// Returns a lVector2 from component-wise application of Step (v &gt;= 0 ? 1 : 0).
        /// </summary>
        public static lVector2 Step(lVector2 v) => new lVector2(v.x >= 0 ? 1 : 0, v.y >= 0 ? 1 : 0);
        
        /// <summary>
        /// Returns a lVector from the application of Step (v &gt;= 0 ? 1 : 0).
        /// </summary>
        public static lVector2 Step(long v) => new lVector2(v >= 0 ? 1 : 0);
        
        /// <summary>
        /// Returns a lVector2 from component-wise application of Sqrt ((long)Math.Sqrt((double)v)).
        /// </summary>
        public static lVector2 Sqrt(lVector2 v) => new lVector2((long)Math.Sqrt((double)v.x), (long)Math.Sqrt((double)v.y));
        
        /// <summary>
        /// Returns a lVector from the application of Sqrt ((long)Math.Sqrt((double)v)).
        /// </summary>
        public static lVector2 Sqrt(long v) => new lVector2((long)Math.Sqrt((double)v));
        
        /// <summary>
        /// Returns a lVector2 from component-wise application of InverseSqrt ((long)(1.0 / Math.Sqrt((double)v))).
        /// </summary>
        public static lVector2 InverseSqrt(lVector2 v) => new lVector2((long)(1.0 / Math.Sqrt((double)v.x)), (long)(1.0 / Math.Sqrt((double)v.y)));
        
        /// <summary>
        /// Returns a lVector from the application of InverseSqrt ((long)(1.0 / Math.Sqrt((double)v))).
        /// </summary>
        public static lVector2 InverseSqrt(long v) => new lVector2((long)(1.0 / Math.Sqrt((double)v)));
        
        /// <summary>
        /// Returns a iVector2 from component-wise application of Sign (Math.Sign(v)).
        /// </summary>
        public static iVector2 Sign(lVector2 v) => new iVector2(Math.Sign(v.x), Math.Sign(v.y));
        
        /// <summary>
        /// Returns a iVector from the application of Sign (Math.Sign(v)).
        /// </summary>
        public static iVector2 Sign(long v) => new iVector2(Math.Sign(v));
        
        /// <summary>
        /// Returns a lVector2 from component-wise application of Max (Math.Max(lhs, rhs)).
        /// </summary>
        public static lVector2 Max(lVector2 lhs, lVector2 rhs) => new lVector2(Math.Max(lhs.x, rhs.x), Math.Max(lhs.y, rhs.y));
        
        /// <summary>
        /// Returns a lVector2 from component-wise application of Max (Math.Max(lhs, rhs)).
        /// </summary>
        public static lVector2 Max(lVector2 lhs, long rhs) => new lVector2(Math.Max(lhs.x, rhs), Math.Max(lhs.y, rhs));
        
        /// <summary>
        /// Returns a lVector2 from component-wise application of Max (Math.Max(lhs, rhs)).
        /// </summary>
        public static lVector2 Max(long lhs, lVector2 rhs) => new lVector2(Math.Max(lhs, rhs.x), Math.Max(lhs, rhs.y));
        
        /// <summary>
        /// Returns a lVector from the application of Max (Math.Max(lhs, rhs)).
        /// </summary>
        public static lVector2 Max(long lhs, long rhs) => new lVector2(Math.Max(lhs, rhs));
        
        /// <summary>
        /// Returns a lVector2 from component-wise application of Min (Math.Min(lhs, rhs)).
        /// </summary>
        public static lVector2 Min(lVector2 lhs, lVector2 rhs) => new lVector2(Math.Min(lhs.x, rhs.x), Math.Min(lhs.y, rhs.y));
        
        /// <summary>
        /// Returns a lVector2 from component-wise application of Min (Math.Min(lhs, rhs)).
        /// </summary>
        public static lVector2 Min(lVector2 lhs, long rhs) => new lVector2(Math.Min(lhs.x, rhs), Math.Min(lhs.y, rhs));
        
        /// <summary>
        /// Returns a lVector2 from component-wise application of Min (Math.Min(lhs, rhs)).
        /// </summary>
        public static lVector2 Min(long lhs, lVector2 rhs) => new lVector2(Math.Min(lhs, rhs.x), Math.Min(lhs, rhs.y));
        
        /// <summary>
        /// Returns a lVector from the application of Min (Math.Min(lhs, rhs)).
        /// </summary>
        public static lVector2 Min(long lhs, long rhs) => new lVector2(Math.Min(lhs, rhs));
        
        /// <summary>
        /// Returns a lVector2 from component-wise application of Pow ((long)Math.Pow((double)lhs, (double)rhs)).
        /// </summary>
        public static lVector2 Pow(lVector2 lhs, lVector2 rhs) => new lVector2((long)Math.Pow((double)lhs.x, (double)rhs.x), (long)Math.Pow((double)lhs.y, (double)rhs.y));
        
        /// <summary>
        /// Returns a lVector2 from component-wise application of Pow ((long)Math.Pow((double)lhs, (double)rhs)).
        /// </summary>
        public static lVector2 Pow(lVector2 lhs, long rhs) => new lVector2((long)Math.Pow((double)lhs.x, (double)rhs), (long)Math.Pow((double)lhs.y, (double)rhs));
        
        /// <summary>
        /// Returns a lVector2 from component-wise application of Pow ((long)Math.Pow((double)lhs, (double)rhs)).
        /// </summary>
        public static lVector2 Pow(long lhs, lVector2 rhs) => new lVector2((long)Math.Pow((double)lhs, (double)rhs.x), (long)Math.Pow((double)lhs, (double)rhs.y));
        
        /// <summary>
        /// Returns a lVector from the application of Pow ((long)Math.Pow((double)lhs, (double)rhs)).
        /// </summary>
        public static lVector2 Pow(long lhs, long rhs) => new lVector2((long)Math.Pow((double)lhs, (double)rhs));
        
        /// <summary>
        /// Returns a lVector2 from component-wise application of Log ((long)Math.Log((double)lhs, (double)rhs)).
        /// </summary>
        public static lVector2 Log(lVector2 lhs, lVector2 rhs) => new lVector2((long)Math.Log((double)lhs.x, (double)rhs.x), (long)Math.Log((double)lhs.y, (double)rhs.y));
        
        /// <summary>
        /// Returns a lVector2 from component-wise application of Log ((long)Math.Log((double)lhs, (double)rhs)).
        /// </summary>
        public static lVector2 Log(lVector2 lhs, long rhs) => new lVector2((long)Math.Log((double)lhs.x, (double)rhs), (long)Math.Log((double)lhs.y, (double)rhs));
        
        /// <summary>
        /// Returns a lVector2 from component-wise application of Log ((long)Math.Log((double)lhs, (double)rhs)).
        /// </summary>
        public static lVector2 Log(long lhs, lVector2 rhs) => new lVector2((long)Math.Log((double)lhs, (double)rhs.x), (long)Math.Log((double)lhs, (double)rhs.y));
        
        /// <summary>
        /// Returns a lVector from the application of Log ((long)Math.Log((double)lhs, (double)rhs)).
        /// </summary>
        public static lVector2 Log(long lhs, long rhs) => new lVector2((long)Math.Log((double)lhs, (double)rhs));
        
        /// <summary>
        /// Returns a lVector2 from component-wise application of Clamp (Math.Min(Math.Max(v, min), max)).
        /// </summary>
        public static lVector2 Clamp(lVector2 v, lVector2 min, lVector2 max) => new lVector2(Math.Min(Math.Max(v.x, min.x), max.x), Math.Min(Math.Max(v.y, min.y), max.y));
        
        /// <summary>
        /// Returns a lVector2 from component-wise application of Clamp (Math.Min(Math.Max(v, min), max)).
        /// </summary>
        public static lVector2 Clamp(lVector2 v, lVector2 min, long max) => new lVector2(Math.Min(Math.Max(v.x, min.x), max), Math.Min(Math.Max(v.y, min.y), max));
        
        /// <summary>
        /// Returns a lVector2 from component-wise application of Clamp (Math.Min(Math.Max(v, min), max)).
        /// </summary>
        public static lVector2 Clamp(lVector2 v, long min, lVector2 max) => new lVector2(Math.Min(Math.Max(v.x, min), max.x), Math.Min(Math.Max(v.y, min), max.y));
        
        /// <summary>
        /// Returns a lVector2 from component-wise application of Clamp (Math.Min(Math.Max(v, min), max)).
        /// </summary>
        public static lVector2 Clamp(lVector2 v, long min, long max) => new lVector2(Math.Min(Math.Max(v.x, min), max), Math.Min(Math.Max(v.y, min), max));
        
        /// <summary>
        /// Returns a lVector2 from component-wise application of Clamp (Math.Min(Math.Max(v, min), max)).
        /// </summary>
        public static lVector2 Clamp(long v, lVector2 min, lVector2 max) => new lVector2(Math.Min(Math.Max(v, min.x), max.x), Math.Min(Math.Max(v, min.y), max.y));
        
        /// <summary>
        /// Returns a lVector2 from component-wise application of Clamp (Math.Min(Math.Max(v, min), max)).
        /// </summary>
        public static lVector2 Clamp(long v, lVector2 min, long max) => new lVector2(Math.Min(Math.Max(v, min.x), max), Math.Min(Math.Max(v, min.y), max));
        
        /// <summary>
        /// Returns a lVector2 from component-wise application of Clamp (Math.Min(Math.Max(v, min), max)).
        /// </summary>
        public static lVector2 Clamp(long v, long min, lVector2 max) => new lVector2(Math.Min(Math.Max(v, min), max.x), Math.Min(Math.Max(v, min), max.y));
        
        /// <summary>
        /// Returns a lVector from the application of Clamp (Math.Min(Math.Max(v, min), max)).
        /// </summary>
        public static lVector2 Clamp(long v, long min, long max) => new lVector2(Math.Min(Math.Max(v, min), max));
        
        /// <summary>
        /// Returns a lVector2 from component-wise application of Mix (min * (1-a) + max * a).
        /// </summary>
        public static lVector2 Mix(lVector2 min, lVector2 max, lVector2 a) => new lVector2(min.x * (1-a.x) + max.x * a.x, min.y * (1-a.y) + max.y * a.y);
        
        /// <summary>
        /// Returns a lVector2 from component-wise application of Mix (min * (1-a) + max * a).
        /// </summary>
        public static lVector2 Mix(lVector2 min, lVector2 max, long a) => new lVector2(min.x * (1-a) + max.x * a, min.y * (1-a) + max.y * a);
        
        /// <summary>
        /// Returns a lVector2 from component-wise application of Mix (min * (1-a) + max * a).
        /// </summary>
        public static lVector2 Mix(lVector2 min, long max, lVector2 a) => new lVector2(min.x * (1-a.x) + max * a.x, min.y * (1-a.y) + max * a.y);
        
        /// <summary>
        /// Returns a lVector2 from component-wise application of Mix (min * (1-a) + max * a).
        /// </summary>
        public static lVector2 Mix(lVector2 min, long max, long a) => new lVector2(min.x * (1-a) + max * a, min.y * (1-a) + max * a);
        
        /// <summary>
        /// Returns a lVector2 from component-wise application of Mix (min * (1-a) + max * a).
        /// </summary>
        public static lVector2 Mix(long min, lVector2 max, lVector2 a) => new lVector2(min * (1-a.x) + max.x * a.x, min * (1-a.y) + max.y * a.y);
        
        /// <summary>
        /// Returns a lVector2 from component-wise application of Mix (min * (1-a) + max * a).
        /// </summary>
        public static lVector2 Mix(long min, lVector2 max, long a) => new lVector2(min * (1-a) + max.x * a, min * (1-a) + max.y * a);
        
        /// <summary>
        /// Returns a lVector2 from component-wise application of Mix (min * (1-a) + max * a).
        /// </summary>
        public static lVector2 Mix(long min, long max, lVector2 a) => new lVector2(min * (1-a.x) + max * a.x, min * (1-a.y) + max * a.y);
        
        /// <summary>
        /// Returns a lVector from the application of Mix (min * (1-a) + max * a).
        /// </summary>
        public static lVector2 Mix(long min, long max, long a) => new lVector2(min * (1-a) + max * a);
        
        /// <summary>
        /// Returns a lVector2 from component-wise application of Lerp (min * (1-a) + max * a).
        /// </summary>
        public static lVector2 Lerp(lVector2 min, lVector2 max, lVector2 a) => new lVector2(min.x * (1-a.x) + max.x * a.x, min.y * (1-a.y) + max.y * a.y);
        
        /// <summary>
        /// Returns a lVector2 from component-wise application of Lerp (min * (1-a) + max * a).
        /// </summary>
        public static lVector2 Lerp(lVector2 min, lVector2 max, long a) => new lVector2(min.x * (1-a) + max.x * a, min.y * (1-a) + max.y * a);
        
        /// <summary>
        /// Returns a lVector2 from component-wise application of Lerp (min * (1-a) + max * a).
        /// </summary>
        public static lVector2 Lerp(lVector2 min, long max, lVector2 a) => new lVector2(min.x * (1-a.x) + max * a.x, min.y * (1-a.y) + max * a.y);
        
        /// <summary>
        /// Returns a lVector2 from component-wise application of Lerp (min * (1-a) + max * a).
        /// </summary>
        public static lVector2 Lerp(lVector2 min, long max, long a) => new lVector2(min.x * (1-a) + max * a, min.y * (1-a) + max * a);
        
        /// <summary>
        /// Returns a lVector2 from component-wise application of Lerp (min * (1-a) + max * a).
        /// </summary>
        public static lVector2 Lerp(long min, lVector2 max, lVector2 a) => new lVector2(min * (1-a.x) + max.x * a.x, min * (1-a.y) + max.y * a.y);
        
        /// <summary>
        /// Returns a lVector2 from component-wise application of Lerp (min * (1-a) + max * a).
        /// </summary>
        public static lVector2 Lerp(long min, lVector2 max, long a) => new lVector2(min * (1-a) + max.x * a, min * (1-a) + max.y * a);
        
        /// <summary>
        /// Returns a lVector2 from component-wise application of Lerp (min * (1-a) + max * a).
        /// </summary>
        public static lVector2 Lerp(long min, long max, lVector2 a) => new lVector2(min * (1-a.x) + max * a.x, min * (1-a.y) + max * a.y);
        
        /// <summary>
        /// Returns a lVector from the application of Lerp (min * (1-a) + max * a).
        /// </summary>
        public static lVector2 Lerp(long min, long max, long a) => new lVector2(min * (1-a) + max * a);
        
        /// <summary>
        /// Returns a lVector2 from component-wise application of Smoothstep (((v - edge0) / (edge1 - edge0)).Clamp().HermiteInterpolationOrder3()).
        /// </summary>
        public static lVector2 Smoothstep(lVector2 edge0, lVector2 edge1, lVector2 v) => new lVector2(((v.x - edge0.x) / (edge1.x - edge0.x)).Clamp().HermiteInterpolationOrder3(), ((v.y - edge0.y) / (edge1.y - edge0.y)).Clamp().HermiteInterpolationOrder3());
        
        /// <summary>
        /// Returns a lVector2 from component-wise application of Smoothstep (((v - edge0) / (edge1 - edge0)).Clamp().HermiteInterpolationOrder3()).
        /// </summary>
        public static lVector2 Smoothstep(lVector2 edge0, lVector2 edge1, long v) => new lVector2(((v - edge0.x) / (edge1.x - edge0.x)).Clamp().HermiteInterpolationOrder3(), ((v - edge0.y) / (edge1.y - edge0.y)).Clamp().HermiteInterpolationOrder3());
        
        /// <summary>
        /// Returns a lVector2 from component-wise application of Smoothstep (((v - edge0) / (edge1 - edge0)).Clamp().HermiteInterpolationOrder3()).
        /// </summary>
        public static lVector2 Smoothstep(lVector2 edge0, long edge1, lVector2 v) => new lVector2(((v.x - edge0.x) / (edge1 - edge0.x)).Clamp().HermiteInterpolationOrder3(), ((v.y - edge0.y) / (edge1 - edge0.y)).Clamp().HermiteInterpolationOrder3());
        
        /// <summary>
        /// Returns a lVector2 from component-wise application of Smoothstep (((v - edge0) / (edge1 - edge0)).Clamp().HermiteInterpolationOrder3()).
        /// </summary>
        public static lVector2 Smoothstep(lVector2 edge0, long edge1, long v) => new lVector2(((v - edge0.x) / (edge1 - edge0.x)).Clamp().HermiteInterpolationOrder3(), ((v - edge0.y) / (edge1 - edge0.y)).Clamp().HermiteInterpolationOrder3());
        
        /// <summary>
        /// Returns a lVector2 from component-wise application of Smoothstep (((v - edge0) / (edge1 - edge0)).Clamp().HermiteInterpolationOrder3()).
        /// </summary>
        public static lVector2 Smoothstep(long edge0, lVector2 edge1, lVector2 v) => new lVector2(((v.x - edge0) / (edge1.x - edge0)).Clamp().HermiteInterpolationOrder3(), ((v.y - edge0) / (edge1.y - edge0)).Clamp().HermiteInterpolationOrder3());
        
        /// <summary>
        /// Returns a lVector2 from component-wise application of Smoothstep (((v - edge0) / (edge1 - edge0)).Clamp().HermiteInterpolationOrder3()).
        /// </summary>
        public static lVector2 Smoothstep(long edge0, lVector2 edge1, long v) => new lVector2(((v - edge0) / (edge1.x - edge0)).Clamp().HermiteInterpolationOrder3(), ((v - edge0) / (edge1.y - edge0)).Clamp().HermiteInterpolationOrder3());
        
        /// <summary>
        /// Returns a lVector2 from component-wise application of Smoothstep (((v - edge0) / (edge1 - edge0)).Clamp().HermiteInterpolationOrder3()).
        /// </summary>
        public static lVector2 Smoothstep(long edge0, long edge1, lVector2 v) => new lVector2(((v.x - edge0) / (edge1 - edge0)).Clamp().HermiteInterpolationOrder3(), ((v.y - edge0) / (edge1 - edge0)).Clamp().HermiteInterpolationOrder3());
        
        /// <summary>
        /// Returns a lVector from the application of Smoothstep (((v - edge0) / (edge1 - edge0)).Clamp().HermiteInterpolationOrder3()).
        /// </summary>
        public static lVector2 Smoothstep(long edge0, long edge1, long v) => new lVector2(((v - edge0) / (edge1 - edge0)).Clamp().HermiteInterpolationOrder3());
        
        /// <summary>
        /// Returns a lVector2 from component-wise application of Smootherstep (((v - edge0) / (edge1 - edge0)).Clamp().HermiteInterpolationOrder5()).
        /// </summary>
        public static lVector2 Smootherstep(lVector2 edge0, lVector2 edge1, lVector2 v) => new lVector2(((v.x - edge0.x) / (edge1.x - edge0.x)).Clamp().HermiteInterpolationOrder5(), ((v.y - edge0.y) / (edge1.y - edge0.y)).Clamp().HermiteInterpolationOrder5());
        
        /// <summary>
        /// Returns a lVector2 from component-wise application of Smootherstep (((v - edge0) / (edge1 - edge0)).Clamp().HermiteInterpolationOrder5()).
        /// </summary>
        public static lVector2 Smootherstep(lVector2 edge0, lVector2 edge1, long v) => new lVector2(((v - edge0.x) / (edge1.x - edge0.x)).Clamp().HermiteInterpolationOrder5(), ((v - edge0.y) / (edge1.y - edge0.y)).Clamp().HermiteInterpolationOrder5());
        
        /// <summary>
        /// Returns a lVector2 from component-wise application of Smootherstep (((v - edge0) / (edge1 - edge0)).Clamp().HermiteInterpolationOrder5()).
        /// </summary>
        public static lVector2 Smootherstep(lVector2 edge0, long edge1, lVector2 v) => new lVector2(((v.x - edge0.x) / (edge1 - edge0.x)).Clamp().HermiteInterpolationOrder5(), ((v.y - edge0.y) / (edge1 - edge0.y)).Clamp().HermiteInterpolationOrder5());
        
        /// <summary>
        /// Returns a lVector2 from component-wise application of Smootherstep (((v - edge0) / (edge1 - edge0)).Clamp().HermiteInterpolationOrder5()).
        /// </summary>
        public static lVector2 Smootherstep(lVector2 edge0, long edge1, long v) => new lVector2(((v - edge0.x) / (edge1 - edge0.x)).Clamp().HermiteInterpolationOrder5(), ((v - edge0.y) / (edge1 - edge0.y)).Clamp().HermiteInterpolationOrder5());
        
        /// <summary>
        /// Returns a lVector2 from component-wise application of Smootherstep (((v - edge0) / (edge1 - edge0)).Clamp().HermiteInterpolationOrder5()).
        /// </summary>
        public static lVector2 Smootherstep(long edge0, lVector2 edge1, lVector2 v) => new lVector2(((v.x - edge0) / (edge1.x - edge0)).Clamp().HermiteInterpolationOrder5(), ((v.y - edge0) / (edge1.y - edge0)).Clamp().HermiteInterpolationOrder5());
        
        /// <summary>
        /// Returns a lVector2 from component-wise application of Smootherstep (((v - edge0) / (edge1 - edge0)).Clamp().HermiteInterpolationOrder5()).
        /// </summary>
        public static lVector2 Smootherstep(long edge0, lVector2 edge1, long v) => new lVector2(((v - edge0) / (edge1.x - edge0)).Clamp().HermiteInterpolationOrder5(), ((v - edge0) / (edge1.y - edge0)).Clamp().HermiteInterpolationOrder5());
        
        /// <summary>
        /// Returns a lVector2 from component-wise application of Smootherstep (((v - edge0) / (edge1 - edge0)).Clamp().HermiteInterpolationOrder5()).
        /// </summary>
        public static lVector2 Smootherstep(long edge0, long edge1, lVector2 v) => new lVector2(((v.x - edge0) / (edge1 - edge0)).Clamp().HermiteInterpolationOrder5(), ((v.y - edge0) / (edge1 - edge0)).Clamp().HermiteInterpolationOrder5());
        
        /// <summary>
        /// Returns a lVector from the application of Smootherstep (((v - edge0) / (edge1 - edge0)).Clamp().HermiteInterpolationOrder5()).
        /// </summary>
        public static lVector2 Smootherstep(long edge0, long edge1, long v) => new lVector2(((v - edge0) / (edge1 - edge0)).Clamp().HermiteInterpolationOrder5());
        
        /// <summary>
        /// Returns a lVector2 from component-wise application of Fma (a * b + c).
        /// </summary>
        public static lVector2 Fma(lVector2 a, lVector2 b, lVector2 c) => new lVector2(a.x * b.x + c.x, a.y * b.y + c.y);
        
        /// <summary>
        /// Returns a lVector2 from component-wise application of Fma (a * b + c).
        /// </summary>
        public static lVector2 Fma(lVector2 a, lVector2 b, long c) => new lVector2(a.x * b.x + c, a.y * b.y + c);
        
        /// <summary>
        /// Returns a lVector2 from component-wise application of Fma (a * b + c).
        /// </summary>
        public static lVector2 Fma(lVector2 a, long b, lVector2 c) => new lVector2(a.x * b + c.x, a.y * b + c.y);
        
        /// <summary>
        /// Returns a lVector2 from component-wise application of Fma (a * b + c).
        /// </summary>
        public static lVector2 Fma(lVector2 a, long b, long c) => new lVector2(a.x * b + c, a.y * b + c);
        
        /// <summary>
        /// Returns a lVector2 from component-wise application of Fma (a * b + c).
        /// </summary>
        public static lVector2 Fma(long a, lVector2 b, lVector2 c) => new lVector2(a * b.x + c.x, a * b.y + c.y);
        
        /// <summary>
        /// Returns a lVector2 from component-wise application of Fma (a * b + c).
        /// </summary>
        public static lVector2 Fma(long a, lVector2 b, long c) => new lVector2(a * b.x + c, a * b.y + c);
        
        /// <summary>
        /// Returns a lVector2 from component-wise application of Fma (a * b + c).
        /// </summary>
        public static lVector2 Fma(long a, long b, lVector2 c) => new lVector2(a * b + c.x, a * b + c.y);
        
        /// <summary>
        /// Returns a lVector from the application of Fma (a * b + c).
        /// </summary>
        public static lVector2 Fma(long a, long b, long c) => new lVector2(a * b + c);
        
        /// <summary>
        /// Returns a lVector2 from component-wise application of Add (lhs + rhs).
        /// </summary>
        public static lVector2 Add(lVector2 lhs, lVector2 rhs) => new lVector2(lhs.x + rhs.x, lhs.y + rhs.y);
        
        /// <summary>
        /// Returns a lVector2 from component-wise application of Add (lhs + rhs).
        /// </summary>
        public static lVector2 Add(lVector2 lhs, long rhs) => new lVector2(lhs.x + rhs, lhs.y + rhs);
        
        /// <summary>
        /// Returns a lVector2 from component-wise application of Add (lhs + rhs).
        /// </summary>
        public static lVector2 Add(long lhs, lVector2 rhs) => new lVector2(lhs + rhs.x, lhs + rhs.y);
        
        /// <summary>
        /// Returns a lVector from the application of Add (lhs + rhs).
        /// </summary>
        public static lVector2 Add(long lhs, long rhs) => new lVector2(lhs + rhs);
        
        /// <summary>
        /// Returns a lVector2 from component-wise application of Sub (lhs - rhs).
        /// </summary>
        public static lVector2 Sub(lVector2 lhs, lVector2 rhs) => new lVector2(lhs.x - rhs.x, lhs.y - rhs.y);
        
        /// <summary>
        /// Returns a lVector2 from component-wise application of Sub (lhs - rhs).
        /// </summary>
        public static lVector2 Sub(lVector2 lhs, long rhs) => new lVector2(lhs.x - rhs, lhs.y - rhs);
        
        /// <summary>
        /// Returns a lVector2 from component-wise application of Sub (lhs - rhs).
        /// </summary>
        public static lVector2 Sub(long lhs, lVector2 rhs) => new lVector2(lhs - rhs.x, lhs - rhs.y);
        
        /// <summary>
        /// Returns a lVector from the application of Sub (lhs - rhs).
        /// </summary>
        public static lVector2 Sub(long lhs, long rhs) => new lVector2(lhs - rhs);
        
        /// <summary>
        /// Returns a lVector2 from component-wise application of Mul (lhs * rhs).
        /// </summary>
        public static lVector2 Mul(lVector2 lhs, lVector2 rhs) => new lVector2(lhs.x * rhs.x, lhs.y * rhs.y);
        
        /// <summary>
        /// Returns a lVector2 from component-wise application of Mul (lhs * rhs).
        /// </summary>
        public static lVector2 Mul(lVector2 lhs, long rhs) => new lVector2(lhs.x * rhs, lhs.y * rhs);
        
        /// <summary>
        /// Returns a lVector2 from component-wise application of Mul (lhs * rhs).
        /// </summary>
        public static lVector2 Mul(long lhs, lVector2 rhs) => new lVector2(lhs * rhs.x, lhs * rhs.y);
        
        /// <summary>
        /// Returns a lVector from the application of Mul (lhs * rhs).
        /// </summary>
        public static lVector2 Mul(long lhs, long rhs) => new lVector2(lhs * rhs);
        
        /// <summary>
        /// Returns a lVector2 from component-wise application of Div (lhs / rhs).
        /// </summary>
        public static lVector2 Div(lVector2 lhs, lVector2 rhs) => new lVector2(lhs.x / rhs.x, lhs.y / rhs.y);
        
        /// <summary>
        /// Returns a lVector2 from component-wise application of Div (lhs / rhs).
        /// </summary>
        public static lVector2 Div(lVector2 lhs, long rhs) => new lVector2(lhs.x / rhs, lhs.y / rhs);
        
        /// <summary>
        /// Returns a lVector2 from component-wise application of Div (lhs / rhs).
        /// </summary>
        public static lVector2 Div(long lhs, lVector2 rhs) => new lVector2(lhs / rhs.x, lhs / rhs.y);
        
        /// <summary>
        /// Returns a lVector from the application of Div (lhs / rhs).
        /// </summary>
        public static lVector2 Div(long lhs, long rhs) => new lVector2(lhs / rhs);
        
        /// <summary>
        /// Returns a lVector2 from component-wise application of Xor (lhs ^ rhs).
        /// </summary>
        public static lVector2 Xor(lVector2 lhs, lVector2 rhs) => new lVector2(lhs.x ^ rhs.x, lhs.y ^ rhs.y);
        
        /// <summary>
        /// Returns a lVector2 from component-wise application of Xor (lhs ^ rhs).
        /// </summary>
        public static lVector2 Xor(lVector2 lhs, long rhs) => new lVector2(lhs.x ^ rhs, lhs.y ^ rhs);
        
        /// <summary>
        /// Returns a lVector2 from component-wise application of Xor (lhs ^ rhs).
        /// </summary>
        public static lVector2 Xor(long lhs, lVector2 rhs) => new lVector2(lhs ^ rhs.x, lhs ^ rhs.y);
        
        /// <summary>
        /// Returns a lVector from the application of Xor (lhs ^ rhs).
        /// </summary>
        public static lVector2 Xor(long lhs, long rhs) => new lVector2(lhs ^ rhs);
        
        /// <summary>
        /// Returns a lVector2 from component-wise application of BitwiseOr (lhs | rhs).
        /// </summary>
        public static lVector2 BitwiseOr(lVector2 lhs, lVector2 rhs) => new lVector2(lhs.x | rhs.x, lhs.y | rhs.y);
        
        /// <summary>
        /// Returns a lVector2 from component-wise application of BitwiseOr (lhs | rhs).
        /// </summary>
        public static lVector2 BitwiseOr(lVector2 lhs, long rhs) => new lVector2(lhs.x | rhs, lhs.y | rhs);
        
        /// <summary>
        /// Returns a lVector2 from component-wise application of BitwiseOr (lhs | rhs).
        /// </summary>
        public static lVector2 BitwiseOr(long lhs, lVector2 rhs) => new lVector2(lhs | rhs.x, lhs | rhs.y);
        
        /// <summary>
        /// Returns a lVector from the application of BitwiseOr (lhs | rhs).
        /// </summary>
        public static lVector2 BitwiseOr(long lhs, long rhs) => new lVector2(lhs | rhs);
        
        /// <summary>
        /// Returns a lVector2 from component-wise application of BitwiseAnd (lhs &amp; rhs).
        /// </summary>
        public static lVector2 BitwiseAnd(lVector2 lhs, lVector2 rhs) => new lVector2(lhs.x & rhs.x, lhs.y & rhs.y);
        
        /// <summary>
        /// Returns a lVector2 from component-wise application of BitwiseAnd (lhs &amp; rhs).
        /// </summary>
        public static lVector2 BitwiseAnd(lVector2 lhs, long rhs) => new lVector2(lhs.x & rhs, lhs.y & rhs);
        
        /// <summary>
        /// Returns a lVector2 from component-wise application of BitwiseAnd (lhs &amp; rhs).
        /// </summary>
        public static lVector2 BitwiseAnd(long lhs, lVector2 rhs) => new lVector2(lhs & rhs.x, lhs & rhs.y);
        
        /// <summary>
        /// Returns a lVector from the application of BitwiseAnd (lhs &amp; rhs).
        /// </summary>
        public static lVector2 BitwiseAnd(long lhs, long rhs) => new lVector2(lhs & rhs);
        
        /// <summary>
        /// Returns a lVector2 from component-wise application of LeftShift (lhs &lt;&lt; rhs).
        /// </summary>
        public static lVector2 LeftShift(lVector2 lhs, iVector2 rhs) => new lVector2(lhs.x << rhs.x, lhs.y << rhs.y);
        
        /// <summary>
        /// Returns a lVector2 from component-wise application of LeftShift (lhs &lt;&lt; rhs).
        /// </summary>
        public static lVector2 LeftShift(lVector2 lhs, int rhs) => new lVector2(lhs.x << rhs, lhs.y << rhs);
        
        /// <summary>
        /// Returns a lVector2 from component-wise application of LeftShift (lhs &lt;&lt; rhs).
        /// </summary>
        public static lVector2 LeftShift(long lhs, iVector2 rhs) => new lVector2(lhs << rhs.x, lhs << rhs.y);
        
        /// <summary>
        /// Returns a lVector from the application of LeftShift (lhs &lt;&lt; rhs).
        /// </summary>
        public static lVector2 LeftShift(long lhs, int rhs) => new lVector2(lhs << rhs);
        
        /// <summary>
        /// Returns a lVector2 from component-wise application of RightShift (lhs &gt;&gt; rhs).
        /// </summary>
        public static lVector2 RightShift(lVector2 lhs, iVector2 rhs) => new lVector2(lhs.x >> rhs.x, lhs.y >> rhs.y);
        
        /// <summary>
        /// Returns a lVector2 from component-wise application of RightShift (lhs &gt;&gt; rhs).
        /// </summary>
        public static lVector2 RightShift(lVector2 lhs, int rhs) => new lVector2(lhs.x >> rhs, lhs.y >> rhs);
        
        /// <summary>
        /// Returns a lVector2 from component-wise application of RightShift (lhs &gt;&gt; rhs).
        /// </summary>
        public static lVector2 RightShift(long lhs, iVector2 rhs) => new lVector2(lhs >> rhs.x, lhs >> rhs.y);
        
        /// <summary>
        /// Returns a lVector from the application of RightShift (lhs &gt;&gt; rhs).
        /// </summary>
        public static lVector2 RightShift(long lhs, int rhs) => new lVector2(lhs >> rhs);
        
        /// <summary>
        /// Returns a lVector2 with independent and identically distributed uniform integer values between 0 (inclusive) and maxValue (exclusive). (A maxValue of 0 is allowed and returns 0.)
        /// </summary>
        public static lVector2 Random(System.Random random, lVector2 maxValue) => new lVector2((long)random.Next((int)maxValue.x), (long)random.Next((int)maxValue.y));
        
        /// <summary>
        /// Returns a lVector2 with independent and identically distributed uniform integer values between 0 (inclusive) and maxValue (exclusive). (A maxValue of 0 is allowed and returns 0.)
        /// </summary>
        public static lVector2 Random(System.Random random, long maxValue) => new lVector2((long)random.Next((int)maxValue));
        
        /// <summary>
        /// Returns a lVector2 with independent and identically distributed uniform integer values between minValue (inclusive) and maxValue (exclusive). (minValue == maxValue is allowed and returns minValue. Negative values are allowed.)
        /// </summary>
        public static lVector2 Random(System.Random random, lVector2 minValue, lVector2 maxValue) => new lVector2((long)random.Next((int)minValue.x, (int)maxValue.x), (long)random.Next((int)minValue.y, (int)maxValue.y));
        
        /// <summary>
        /// Returns a lVector2 with independent and identically distributed uniform integer values between minValue (inclusive) and maxValue (exclusive). (minValue == maxValue is allowed and returns minValue. Negative values are allowed.)
        /// </summary>
        public static lVector2 Random(System.Random random, lVector2 minValue, long maxValue) => new lVector2((long)random.Next((int)minValue.x, (int)maxValue), (long)random.Next((int)minValue.y, (int)maxValue));
        
        /// <summary>
        /// Returns a lVector2 with independent and identically distributed uniform integer values between minValue (inclusive) and maxValue (exclusive). (minValue == maxValue is allowed and returns minValue. Negative values are allowed.)
        /// </summary>
        public static lVector2 Random(System.Random random, long minValue, lVector2 maxValue) => new lVector2((long)random.Next((int)minValue, (int)maxValue.x), (long)random.Next((int)minValue, (int)maxValue.y));
        
        /// <summary>
        /// Returns a lVector2 with independent and identically distributed uniform integer values between minValue (inclusive) and maxValue (exclusive). (minValue == maxValue is allowed and returns minValue. Negative values are allowed.)
        /// </summary>
        public static lVector2 Random(System.Random random, long minValue, long maxValue) => new lVector2((long)random.Next((int)minValue, (int)maxValue));
        
        /// <summary>
        /// Returns a lVector2 with independent and identically distributed uniform integer values between minValue (inclusive) and maxValue (exclusive). (minValue == maxValue is allowed and returns minValue. Negative values are allowed.)
        /// </summary>
        public static lVector2 RandomUniform(System.Random random, lVector2 minValue, lVector2 maxValue) => new lVector2((long)random.Next((int)minValue.x, (int)maxValue.x), (long)random.Next((int)minValue.y, (int)maxValue.y));
        
        /// <summary>
        /// Returns a lVector2 with independent and identically distributed uniform integer values between minValue (inclusive) and maxValue (exclusive). (minValue == maxValue is allowed and returns minValue. Negative values are allowed.)
        /// </summary>
        public static lVector2 RandomUniform(System.Random random, lVector2 minValue, long maxValue) => new lVector2((long)random.Next((int)minValue.x, (int)maxValue), (long)random.Next((int)minValue.y, (int)maxValue));
        
        /// <summary>
        /// Returns a lVector2 with independent and identically distributed uniform integer values between minValue (inclusive) and maxValue (exclusive). (minValue == maxValue is allowed and returns minValue. Negative values are allowed.)
        /// </summary>
        public static lVector2 RandomUniform(System.Random random, long minValue, lVector2 maxValue) => new lVector2((long)random.Next((int)minValue, (int)maxValue.x), (long)random.Next((int)minValue, (int)maxValue.y));
        
        /// <summary>
        /// Returns a lVector2 with independent and identically distributed uniform integer values between minValue (inclusive) and maxValue (exclusive). (minValue == maxValue is allowed and returns minValue. Negative values are allowed.)
        /// </summary>
        public static lVector2 RandomUniform(System.Random random, long minValue, long maxValue) => new lVector2((long)random.Next((int)minValue, (int)maxValue));
        
        /// <summary>
        /// Returns a lVector2 with independent and identically distributed integer values according to a poisson distribution with given lambda parameter.
        /// </summary>
        public static lVector2 RandomPoisson(System.Random random, dVector2 lambda) => new lVector2((long)lambda.x.GetPoisson(random), (long)lambda.y.GetPoisson(random));
        
        /// <summary>
        /// Returns a lVector2 with independent and identically distributed integer values according to a poisson distribution with given lambda parameter.
        /// </summary>
        public static lVector2 RandomPoisson(System.Random random, double lambda) => new lVector2((long)lambda.GetPoisson(random));

        #endregion


        #region Component-Wise Operator Overloads
        
        /// <summary>
        /// Returns a bVector2 from component-wise application of operator&lt; (lhs &lt; rhs).
        /// </summary>
        public static bVector2 operator<(lVector2 lhs, lVector2 rhs) => new bVector2(lhs.x < rhs.x, lhs.y < rhs.y);
        
        /// <summary>
        /// Returns a bVector2 from component-wise application of operator&lt; (lhs &lt; rhs).
        /// </summary>
        public static bVector2 operator<(lVector2 lhs, long rhs) => new bVector2(lhs.x < rhs, lhs.y < rhs);
        
        /// <summary>
        /// Returns a bVector2 from component-wise application of operator&lt; (lhs &lt; rhs).
        /// </summary>
        public static bVector2 operator<(long lhs, lVector2 rhs) => new bVector2(lhs < rhs.x, lhs < rhs.y);
        
        /// <summary>
        /// Returns a bVector2 from component-wise application of operator&lt;= (lhs &lt;= rhs).
        /// </summary>
        public static bVector2 operator<=(lVector2 lhs, lVector2 rhs) => new bVector2(lhs.x <= rhs.x, lhs.y <= rhs.y);
        
        /// <summary>
        /// Returns a bVector2 from component-wise application of operator&lt;= (lhs &lt;= rhs).
        /// </summary>
        public static bVector2 operator<=(lVector2 lhs, long rhs) => new bVector2(lhs.x <= rhs, lhs.y <= rhs);
        
        /// <summary>
        /// Returns a bVector2 from component-wise application of operator&lt;= (lhs &lt;= rhs).
        /// </summary>
        public static bVector2 operator<=(long lhs, lVector2 rhs) => new bVector2(lhs <= rhs.x, lhs <= rhs.y);
        
        /// <summary>
        /// Returns a bVector2 from component-wise application of operator&gt; (lhs &gt; rhs).
        /// </summary>
        public static bVector2 operator>(lVector2 lhs, lVector2 rhs) => new bVector2(lhs.x > rhs.x, lhs.y > rhs.y);
        
        /// <summary>
        /// Returns a bVector2 from component-wise application of operator&gt; (lhs &gt; rhs).
        /// </summary>
        public static bVector2 operator>(lVector2 lhs, long rhs) => new bVector2(lhs.x > rhs, lhs.y > rhs);
        
        /// <summary>
        /// Returns a bVector2 from component-wise application of operator&gt; (lhs &gt; rhs).
        /// </summary>
        public static bVector2 operator>(long lhs, lVector2 rhs) => new bVector2(lhs > rhs.x, lhs > rhs.y);
        
        /// <summary>
        /// Returns a bVector2 from component-wise application of operator&gt;= (lhs &gt;= rhs).
        /// </summary>
        public static bVector2 operator>=(lVector2 lhs, lVector2 rhs) => new bVector2(lhs.x >= rhs.x, lhs.y >= rhs.y);
        
        /// <summary>
        /// Returns a bVector2 from component-wise application of operator&gt;= (lhs &gt;= rhs).
        /// </summary>
        public static bVector2 operator>=(lVector2 lhs, long rhs) => new bVector2(lhs.x >= rhs, lhs.y >= rhs);
        
        /// <summary>
        /// Returns a bVector2 from component-wise application of operator&gt;= (lhs &gt;= rhs).
        /// </summary>
        public static bVector2 operator>=(long lhs, lVector2 rhs) => new bVector2(lhs >= rhs.x, lhs >= rhs.y);
        
        /// <summary>
        /// Returns a lVector2 from component-wise application of operator+ (lhs + rhs).
        /// </summary>
        public static lVector2 operator+(lVector2 lhs, lVector2 rhs) => new lVector2(lhs.x + rhs.x, lhs.y + rhs.y);
        
        /// <summary>
        /// Returns a lVector2 from component-wise application of operator+ (lhs + rhs).
        /// </summary>
        public static lVector2 operator+(lVector2 lhs, long rhs) => new lVector2(lhs.x + rhs, lhs.y + rhs);
        
        /// <summary>
        /// Returns a lVector2 from component-wise application of operator+ (lhs + rhs).
        /// </summary>
        public static lVector2 operator+(long lhs, lVector2 rhs) => new lVector2(lhs + rhs.x, lhs + rhs.y);
        
        /// <summary>
        /// Returns a lVector2 from component-wise application of operator- (lhs - rhs).
        /// </summary>
        public static lVector2 operator-(lVector2 lhs, lVector2 rhs) => new lVector2(lhs.x - rhs.x, lhs.y - rhs.y);
        
        /// <summary>
        /// Returns a lVector2 from component-wise application of operator- (lhs - rhs).
        /// </summary>
        public static lVector2 operator-(lVector2 lhs, long rhs) => new lVector2(lhs.x - rhs, lhs.y - rhs);
        
        /// <summary>
        /// Returns a lVector2 from component-wise application of operator- (lhs - rhs).
        /// </summary>
        public static lVector2 operator-(long lhs, lVector2 rhs) => new lVector2(lhs - rhs.x, lhs - rhs.y);
        
        /// <summary>
        /// Returns a lVector2 from component-wise application of operator* (lhs * rhs).
        /// </summary>
        public static lVector2 operator*(lVector2 lhs, lVector2 rhs) => new lVector2(lhs.x * rhs.x, lhs.y * rhs.y);
        
        /// <summary>
        /// Returns a lVector2 from component-wise application of operator* (lhs * rhs).
        /// </summary>
        public static lVector2 operator*(lVector2 lhs, long rhs) => new lVector2(lhs.x * rhs, lhs.y * rhs);
        
        /// <summary>
        /// Returns a lVector2 from component-wise application of operator* (lhs * rhs).
        /// </summary>
        public static lVector2 operator*(long lhs, lVector2 rhs) => new lVector2(lhs * rhs.x, lhs * rhs.y);
        
        /// <summary>
        /// Returns a lVector2 from component-wise application of operator/ (lhs / rhs).
        /// </summary>
        public static lVector2 operator/(lVector2 lhs, lVector2 rhs) => new lVector2(lhs.x / rhs.x, lhs.y / rhs.y);
        
        /// <summary>
        /// Returns a lVector2 from component-wise application of operator/ (lhs / rhs).
        /// </summary>
        public static lVector2 operator/(lVector2 lhs, long rhs) => new lVector2(lhs.x / rhs, lhs.y / rhs);
        
        /// <summary>
        /// Returns a lVector2 from component-wise application of operator/ (lhs / rhs).
        /// </summary>
        public static lVector2 operator/(long lhs, lVector2 rhs) => new lVector2(lhs / rhs.x, lhs / rhs.y);
        
        /// <summary>
        /// Returns a lVector2 from component-wise application of operator+ (identity).
        /// </summary>
        public static lVector2 operator+(lVector2 v) => v;
        
        /// <summary>
        /// Returns a lVector2 from component-wise application of operator- (-v).
        /// </summary>
        public static lVector2 operator-(lVector2 v) => new lVector2(-v.x, -v.y);
        
        /// <summary>
        /// Returns a lVector2 from component-wise application of operator~ (~v).
        /// </summary>
        public static lVector2 operator~(lVector2 v) => new lVector2(~v.x, ~v.y);
        
        /// <summary>
        /// Returns a lVector2 from component-wise application of operator^ (lhs ^ rhs).
        /// </summary>
        public static lVector2 operator^(lVector2 lhs, lVector2 rhs) => new lVector2(lhs.x ^ rhs.x, lhs.y ^ rhs.y);
        
        /// <summary>
        /// Returns a lVector2 from component-wise application of operator^ (lhs ^ rhs).
        /// </summary>
        public static lVector2 operator^(lVector2 lhs, long rhs) => new lVector2(lhs.x ^ rhs, lhs.y ^ rhs);
        
        /// <summary>
        /// Returns a lVector2 from component-wise application of operator^ (lhs ^ rhs).
        /// </summary>
        public static lVector2 operator^(long lhs, lVector2 rhs) => new lVector2(lhs ^ rhs.x, lhs ^ rhs.y);
        
        /// <summary>
        /// Returns a lVector2 from component-wise application of operator| (lhs | rhs).
        /// </summary>
        public static lVector2 operator|(lVector2 lhs, lVector2 rhs) => new lVector2(lhs.x | rhs.x, lhs.y | rhs.y);
        
        /// <summary>
        /// Returns a lVector2 from component-wise application of operator| (lhs | rhs).
        /// </summary>
        public static lVector2 operator|(lVector2 lhs, long rhs) => new lVector2(lhs.x | rhs, lhs.y | rhs);
        
        /// <summary>
        /// Returns a lVector2 from component-wise application of operator| (lhs | rhs).
        /// </summary>
        public static lVector2 operator|(long lhs, lVector2 rhs) => new lVector2(lhs | rhs.x, lhs | rhs.y);
        
        /// <summary>
        /// Returns a lVector2 from component-wise application of operator&amp; (lhs &amp; rhs).
        /// </summary>
        public static lVector2 operator&(lVector2 lhs, lVector2 rhs) => new lVector2(lhs.x & rhs.x, lhs.y & rhs.y);
        
        /// <summary>
        /// Returns a lVector2 from component-wise application of operator&amp; (lhs &amp; rhs).
        /// </summary>
        public static lVector2 operator&(lVector2 lhs, long rhs) => new lVector2(lhs.x & rhs, lhs.y & rhs);
        
        /// <summary>
        /// Returns a lVector2 from component-wise application of operator&amp; (lhs &amp; rhs).
        /// </summary>
        public static lVector2 operator&(long lhs, lVector2 rhs) => new lVector2(lhs & rhs.x, lhs & rhs.y);
        
        /// <summary>
        /// Returns a lVector2 from component-wise application of operator&lt;&lt; (lhs &lt;&lt; rhs).
        /// </summary>
        public static lVector2 operator<<(lVector2 lhs, int rhs) => new lVector2(lhs.x << rhs, lhs.y << rhs);
        
        /// <summary>
        /// Returns a lVector2 from component-wise application of operator&gt;&gt; (lhs &gt;&gt; rhs).
        /// </summary>
        public static lVector2 operator>>(lVector2 lhs, int rhs) => new lVector2(lhs.x >> rhs, lhs.y >> rhs);

        #endregion

    }
}
