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
    /// A Quaternionernion of type int.
    /// </summary>
    
    
    [StructLayout(LayoutKind.Sequential)]
    public struct iQuaternion : IReadOnlyList<int>, IEquatable<iQuaternion>
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
        
        /// <summary>
        /// z-component
        /// </summary>
        
        public int z;
        
        /// <summary>
        /// w-component
        /// </summary>
        
        public int w;

        #endregion


        #region Constructors
        
        /// <summary>
        /// Component-wise constructor
        /// </summary>
        public iQuaternion(int x, int y, int z, int w)
        {
            this.x = x;
            this.y = y;
            this.z = z;
            this.w = w;
        }
        
        /// <summary>
        /// all-same-value constructor
        /// </summary>
        public iQuaternion(int v)
        {
            this.x = v;
            this.y = v;
            this.z = v;
            this.w = v;
        }
        
        /// <summary>
        /// copy constructor
        /// </summary>
        public iQuaternion(iQuaternion q)
        {
            this.x = q.x;
            this.y = q.y;
            this.z = q.z;
            this.w = q.w;
        }
        
        /// <summary>
        /// Vector-and-scalar constructor (CAUTION: not angle-axis, use FromAngleAxis instead)
        /// </summary>
        public iQuaternion(iVector3 v, int s)
        {
            this.x = v.x;
            this.y = v.y;
            this.z = v.z;
            this.w = s;
        }

        #endregion


        #region Implicit Operators
        
        /// <summary>
        /// Implicitly converts this to a lQuaternion.
        /// </summary>
        public static implicit operator lQuaternion(iQuaternion v) => new lQuaternion((long)v.x, (long)v.y, (long)v.z, (long)v.w);
        
        /// <summary>
        /// Implicitly converts this to a Quaternion.
        /// </summary>
        public static implicit operator Quaternion(iQuaternion v) => new Quaternion((float)v.x, (float)v.y, (float)v.z, (float)v.w);
        
        /// <summary>
        /// Implicitly converts this to a hQuaternion.
        /// </summary>
        public static implicit operator hQuaternion(iQuaternion v) => new hQuaternion((Half)v.x, (Half)v.y, (Half)v.z, (Half)v.w);
        
        /// <summary>
        /// Implicitly converts this to a dQuaternion.
        /// </summary>
        public static implicit operator dQuaternion(iQuaternion v) => new dQuaternion((double)v.x, (double)v.y, (double)v.z, (double)v.w);
        
        /// <summary>
        /// Implicitly converts this to a decQuaternion.
        /// </summary>
        public static implicit operator decQuaternion(iQuaternion v) => new decQuaternion((decimal)v.x, (decimal)v.y, (decimal)v.z, (decimal)v.w);
        
        /// <summary>
        /// Implicitly converts this to a cQuaternion.
        /// </summary>
       // public static implicit operator cQuaternion(iQuaternion v) => new cQuaternion((Complex)v.x, (Complex)v.y, (Complex)v.z, (Complex)v.w);

        #endregion


        #region Explicit Operators
        
        /// <summary>
        /// Explicitly converts this to a iVector4.
        /// </summary>
        public static explicit operator iVector4(iQuaternion v) => new iVector4((int)v.x, (int)v.y, (int)v.z, (int)v.w);
        
        /// <summary>
        /// Explicitly converts this to a uVector4.
        /// </summary>
        public static explicit operator uVector4(iQuaternion v) => new uVector4((uint)v.x, (uint)v.y, (uint)v.z, (uint)v.w);
        
        /// <summary>
        /// Explicitly converts this to a uQuaternion.
        /// </summary>
        public static explicit operator uQuaternion(iQuaternion v) => new uQuaternion((uint)v.x, (uint)v.y, (uint)v.z, (uint)v.w);
        
        /// <summary>
        /// Explicitly converts this to a Vector4.
        /// </summary>
        public static explicit operator Vector4(iQuaternion v) => new Vector4((float)v.x, (float)v.y, (float)v.z, (float)v.w);
        
        /// <summary>
        /// Explicitly converts this to a hVector4.
        /// </summary>
        public static explicit operator hVector4(iQuaternion v) => new hVector4((Half)v.x, (Half)v.y, (Half)v.z, (Half)v.w);
        
        /// <summary>
        /// Explicitly converts this to a dVector4.
        /// </summary>
        public static explicit operator dVector4(iQuaternion v) => new dVector4((double)v.x, (double)v.y, (double)v.z, (double)v.w);
        
        /// <summary>
        /// Explicitly converts this to a decVector4.
        /// </summary>
        public static explicit operator decVector4(iQuaternion v) => new decVector4((decimal)v.x, (decimal)v.y, (decimal)v.z, (decimal)v.w);
        
        /// <summary>
        /// Explicitly converts this to a cVector4.
        /// </summary>
        //public static explicit operator cVector4(iQuaternion v) => new cVector4((Complex)v.x, (Complex)v.y, (Complex)v.z, (Complex)v.w);
        
        /// <summary>
        /// Explicitly converts this to a lVector4.
        /// </summary>
        public static explicit operator lVector4(iQuaternion v) => new lVector4((long)v.x, (long)v.y, (long)v.z, (long)v.w);
        
        /// <summary>
        /// Explicitly converts this to a bVector4.
        /// </summary>
        public static explicit operator bVector4(iQuaternion v) => new bVector4(v.x != 0, v.y != 0, v.z != 0, v.w != 0);
        
        /// <summary>
        /// Explicitly converts this to a bQuaternion.
        /// </summary>
        public static explicit operator bQuaternion(iQuaternion v) => new bQuaternion(v.x != 0, v.y != 0, v.z != 0, v.w != 0);

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
        /// Returns an array with all values
        /// </summary>
        public int[] Values => new[] { x, y, z, w };
        
        /// <summary>
        /// Returns the number of components (4).
        /// </summary>
        public int Count => 4;
        
        /// <summary>
        /// Returns the euclidean length of this Quaternionernion.
        /// </summary>
        public float Length => (float)Math.Sqrt(((x*x + y*y) + (z*z + w*w)));
        
        /// <summary>
        /// Returns the squared euclidean length of this Quaternionernion.
        /// </summary>
        public int LengthSqr => ((x*x + y*y) + (z*z + w*w));
        
        /// <summary>
        /// Returns the conjugated Quaternionernion
        /// </summary>
        public iQuaternion Conjugate => new iQuaternion(-x, -y, -z, w);
        
        /// <summary>
        /// Returns the inverse Quaternionernion
        /// </summary>
        public iQuaternion Inverse => Conjugate / LengthSqr;

        #endregion


        #region Static Properties
        
        /// <summary>
        /// Predefined all-zero Quaternionernion
        /// </summary>
        public static iQuaternion Zero { get; } = new iQuaternion(0, 0, 0, 0);
        
        /// <summary>
        /// Predefined all-ones Quaternionernion
        /// </summary>
        public static iQuaternion Ones { get; } = new iQuaternion(1, 1, 1, 1);
        
        /// <summary>
        /// Predefined identity Quaternionernion
        /// </summary>
        public static iQuaternion Identity { get; } = new iQuaternion(0, 0, 0, 1);
        
        /// <summary>
        /// Predefined unit-X Quaternionernion
        /// </summary>
        public static iQuaternion UnitX { get; } = new iQuaternion(1, 0, 0, 0);
        
        /// <summary>
        /// Predefined unit-Y Quaternionernion
        /// </summary>
        public static iQuaternion UnitY { get; } = new iQuaternion(0, 1, 0, 0);
        
        /// <summary>
        /// Predefined unit-Z Quaternionernion
        /// </summary>
        public static iQuaternion UnitZ { get; } = new iQuaternion(0, 0, 1, 0);
        
        /// <summary>
        /// Predefined unit-W Quaternionernion
        /// </summary>
        public static iQuaternion UnitW { get; } = new iQuaternion(0, 0, 0, 1);
        
        /// <summary>
        /// Predefined all-MaxValue Quaternionernion
        /// </summary>
        public static iQuaternion MaxValue { get; } = new iQuaternion(int.MaxValue, int.MaxValue, int.MaxValue, int.MaxValue);
        
        /// <summary>
        /// Predefined all-MinValue Quaternionernion
        /// </summary>
        public static iQuaternion MinValue { get; } = new iQuaternion(int.MinValue, int.MinValue, int.MinValue, int.MinValue);

        #endregion


        #region Operators
        
        /// <summary>
        /// Returns true iff this equals rhs component-wise.
        /// </summary>
        public static bool operator==(iQuaternion lhs, iQuaternion rhs) => lhs.Equals(rhs);
        
        /// <summary>
        /// Returns true iff this does not equal rhs (component-wise).
        /// </summary>
        public static bool operator!=(iQuaternion lhs, iQuaternion rhs) => !lhs.Equals(rhs);
        
        /// <summary>
        /// Returns proper multiplication of two Quaternionernions.
        /// </summary>
        public static iQuaternion operator*(iQuaternion p, iQuaternion q) => new iQuaternion(p.w * q.x + p.x * q.w + p.y * q.z - p.z * q.y, p.w * q.y + p.y * q.w + p.z * q.x - p.x * q.z, p.w * q.z + p.z * q.w + p.x * q.y - p.y * q.x, p.w * q.w - p.x * q.x - p.y * q.y - p.z * q.z);
        
        /// <summary>
        /// Returns a Vector rotated by the Quaternionernion.
        /// </summary>
        public static iVector3 operator*(iQuaternion q, iVector3 v)
        {
            var qv = new iVector3(q.x, q.y, q.z);
            var uv = iVector3.Cross(qv, v);
            var uuv = iVector3.Cross(qv, uv);
            return v + ((uv * q.w) + uuv) * 2;
        }
        
        /// <summary>
        /// Returns a Vector rotated by the Quaternionernion (preserves v.w).
        /// </summary>
        public static iVector4 operator*(iQuaternion q, iVector4 v) => new iVector4(q * new iVector3(v), v.w);
        
        /// <summary>
        /// Returns a Vector rotated by the inverted Quaternionernion.
        /// </summary>
        public static iVector3 operator*(iVector3 v, iQuaternion q) => q.Inverse * v;
        
        /// <summary>
        /// Returns a Vector rotated by the inverted Quaternionernion (preserves v.w).
        /// </summary>
        public static iVector4 operator*(iVector4 v, iQuaternion q) => q.Inverse * v;

        #endregion


        #region Functions
        
        /// <summary>
        /// Returns an enumerator that iterates through all components.
        /// </summary>
        public IEnumerator<int> GetEnumerator()
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
        /// Returns a string representation of this Quaternionernion using ', ' as a seperator.
        /// </summary>
        public override string ToString() => ToString(", ");
        
        /// <summary>
        /// Returns a string representation of this Quaternionernion using a provided seperator.
        /// </summary>
        public string ToString(string sep) => ((x + sep + y) + sep + (z + sep + w));
        
        /// <summary>
        /// Returns a string representation of this Quaternionernion using a provided seperator and a forMatrix provider for each component.
        /// </summary>
        public string ToString(string sep, IFormatProvider provider) => ((x.ToString(provider) + sep + y.ToString(provider)) + sep + (z.ToString(provider) + sep + w.ToString(provider)));
        
        /// <summary>
        /// Returns a string representation of this Quaternionernion using a provided seperator and a forMatrix for each component.
        /// </summary>
        public string ToString(string sep, string forMatrix) => ((x.ToString(forMatrix) + sep + y.ToString(forMatrix)) + sep + (z.ToString(forMatrix) + sep + w.ToString(forMatrix)));
        
        /// <summary>
        /// Returns a string representation of this Quaternionernion using a provided seperator and a forMatrix and forMatrix provider for each component.
        /// </summary>
        public string ToString(string sep, string forMatrix, IFormatProvider provider) => ((x.ToString(forMatrix, provider) + sep + y.ToString(forMatrix, provider)) + sep + (z.ToString(forMatrix, provider) + sep + w.ToString(forMatrix, provider)));
        
        /// <summary>
        /// Returns true iff this equals rhs component-wise.
        /// </summary>
        public bool Equals(iQuaternion rhs) => ((x.Equals(rhs.x) && y.Equals(rhs.y)) && (z.Equals(rhs.z) && w.Equals(rhs.w)));
        
        /// <summary>
        /// Returns true iff this equals rhs type- and component-wise.
        /// </summary>
        public override bool Equals(object obj)
        {
            if (ReferenceEquals(null, obj)) return false;
            return obj is iQuaternion && Equals((iQuaternion) obj);
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

        #endregion


        #region Static Functions
        
        /// <summary>
        /// Converts the string representation of the Quaternionernion into a Quaternionernion representation (using ', ' as a separator).
        /// </summary>
        public static iQuaternion Parse(string s) => Parse(s, ", ");
        
        /// <summary>
        /// Converts the string representation of the Quaternionernion into a Quaternionernion representation (using a designated separator).
        /// </summary>
        public static iQuaternion Parse(string s, string sep)
        {
            var kvp = s.Split(new[] { sep }, StringSplitOptions.None);
            if (kvp.Length != 4) throw new FormatException("input has not exactly 4 parts");
            return new iQuaternion(int.Parse(kvp[0].Trim()), int.Parse(kvp[1].Trim()), int.Parse(kvp[2].Trim()), int.Parse(kvp[3].Trim()));
        }
        
        /// <summary>
        /// Converts the string representation of the Quaternionernion into a Quaternionernion representation (using a designated separator and a type provider).
        /// </summary>
        public static iQuaternion Parse(string s, string sep, IFormatProvider provider)
        {
            var kvp = s.Split(new[] { sep }, StringSplitOptions.None);
            if (kvp.Length != 4) throw new FormatException("input has not exactly 4 parts");
            return new iQuaternion(int.Parse(kvp[0].Trim(), provider), int.Parse(kvp[1].Trim(), provider), int.Parse(kvp[2].Trim(), provider), int.Parse(kvp[3].Trim(), provider));
        }
        
        /// <summary>
        /// Converts the string representation of the Quaternionernion into a Quaternionernion representation (using a designated separator and a number style).
        /// </summary>
        public static iQuaternion Parse(string s, string sep, NumberStyles style)
        {
            var kvp = s.Split(new[] { sep }, StringSplitOptions.None);
            if (kvp.Length != 4) throw new FormatException("input has not exactly 4 parts");
            return new iQuaternion(int.Parse(kvp[0].Trim(), style), int.Parse(kvp[1].Trim(), style), int.Parse(kvp[2].Trim(), style), int.Parse(kvp[3].Trim(), style));
        }
        
        /// <summary>
        /// Converts the string representation of the Quaternionernion into a Quaternionernion representation (using a designated separator and a number style and a forMatrix provider).
        /// </summary>
        public static iQuaternion Parse(string s, string sep, NumberStyles style, IFormatProvider provider)
        {
            var kvp = s.Split(new[] { sep }, StringSplitOptions.None);
            if (kvp.Length != 4) throw new FormatException("input has not exactly 4 parts");
            return new iQuaternion(int.Parse(kvp[0].Trim(), style, provider), int.Parse(kvp[1].Trim(), style, provider), int.Parse(kvp[2].Trim(), style, provider), int.Parse(kvp[3].Trim(), style, provider));
        }
        
        /// <summary>
        /// Tries to convert the string representation of the Quaternionernion into a Quaternionernion representation (using ', ' as a separator), returns false if string was invalid.
        /// </summary>
        public static bool TryParse(string s, out iQuaternion result) => TryParse(s, ", ", out result);
        
        /// <summary>
        /// Tries to convert the string representation of the Quaternionernion into a Quaternionernion representation (using a designated separator), returns false if string was invalid.
        /// </summary>
        public static bool TryParse(string s, string sep, out iQuaternion result)
        {
            result = Zero;
            if (string.IsNullOrEmpty(s)) return false;
            var kvp = s.Split(new[] { sep }, StringSplitOptions.None);
            if (kvp.Length != 4) return false;
            int x = 0, y = 0, z = 0, w = 0;
            var ok = ((int.TryParse(kvp[0].Trim(), out x) && int.TryParse(kvp[1].Trim(), out y)) && (int.TryParse(kvp[2].Trim(), out z) && int.TryParse(kvp[3].Trim(), out w)));
            result = ok ? new iQuaternion(x, y, z, w) : Zero;
            return ok;
        }
        
        /// <summary>
        /// Tries to convert the string representation of the Quaternionernion into a Quaternionernion representation (using a designated separator and a number style and a forMatrix provider), returns false if string was invalid.
        /// </summary>
        public static bool TryParse(string s, string sep, NumberStyles style, IFormatProvider provider, out iQuaternion result)
        {
            result = Zero;
            if (string.IsNullOrEmpty(s)) return false;
            var kvp = s.Split(new[] { sep }, StringSplitOptions.None);
            if (kvp.Length != 4) return false;
            int x = 0, y = 0, z = 0, w = 0;
            var ok = ((int.TryParse(kvp[0].Trim(), style, provider, out x) && int.TryParse(kvp[1].Trim(), style, provider, out y)) && (int.TryParse(kvp[2].Trim(), style, provider, out z) && int.TryParse(kvp[3].Trim(), style, provider, out w)));
            result = ok ? new iQuaternion(x, y, z, w) : Zero;
            return ok;
        }
        
        /// <summary>
        /// Returns the inner product (dot product, scalar product) of the two Quaternionernions.
        /// </summary>
        public static int Dot(iQuaternion lhs, iQuaternion rhs) => ((lhs.x * rhs.x + lhs.y * rhs.y) + (lhs.z * rhs.z + lhs.w * rhs.w));
        
        /// <summary>
        /// Returns the cross product between two Quaternionernions.
        /// </summary>
        public static iQuaternion Cross(iQuaternion q1, iQuaternion q2) => new iQuaternion(q1.w * q2.x + q1.x * q2.w + q1.y * q2.z - q1.z * q2.y, q1.w * q2.y + q1.y * q2.w + q1.z * q2.x - q1.x * q2.z, q1.w * q2.z + q1.z * q2.w + q1.x * q2.y - q1.y * q2.x, q1.w * q2.w - q1.x * q2.x - q1.y * q2.y - q1.z * q2.z);

        #endregion


        #region Component-Wise Static Functions
        
        /// <summary>
        /// Returns a bVector4 from component-wise application of Equal (lhs == rhs).
        /// </summary>
        public static bVector4 Equal(iQuaternion lhs, iQuaternion rhs) => new bVector4(lhs.x == rhs.x, lhs.y == rhs.y, lhs.z == rhs.z, lhs.w == rhs.w);
        
        /// <summary>
        /// Returns a bVector4 from component-wise application of Equal (lhs == rhs).
        /// </summary>
        public static bVector4 Equal(iQuaternion lhs, int rhs) => new bVector4(lhs.x == rhs, lhs.y == rhs, lhs.z == rhs, lhs.w == rhs);
        
        /// <summary>
        /// Returns a bVector4 from component-wise application of Equal (lhs == rhs).
        /// </summary>
        public static bVector4 Equal(int lhs, iQuaternion rhs) => new bVector4(lhs == rhs.x, lhs == rhs.y, lhs == rhs.z, lhs == rhs.w);
        
        /// <summary>
        /// Returns a bVector from the application of Equal (lhs == rhs).
        /// </summary>
        public static bVector4 Equal(int lhs, int rhs) => new bVector4(lhs == rhs);
        
        /// <summary>
        /// Returns a bVector4 from component-wise application of NotEqual (lhs != rhs).
        /// </summary>
        public static bVector4 NotEqual(iQuaternion lhs, iQuaternion rhs) => new bVector4(lhs.x != rhs.x, lhs.y != rhs.y, lhs.z != rhs.z, lhs.w != rhs.w);
        
        /// <summary>
        /// Returns a bVector4 from component-wise application of NotEqual (lhs != rhs).
        /// </summary>
        public static bVector4 NotEqual(iQuaternion lhs, int rhs) => new bVector4(lhs.x != rhs, lhs.y != rhs, lhs.z != rhs, lhs.w != rhs);
        
        /// <summary>
        /// Returns a bVector4 from component-wise application of NotEqual (lhs != rhs).
        /// </summary>
        public static bVector4 NotEqual(int lhs, iQuaternion rhs) => new bVector4(lhs != rhs.x, lhs != rhs.y, lhs != rhs.z, lhs != rhs.w);
        
        /// <summary>
        /// Returns a bVector from the application of NotEqual (lhs != rhs).
        /// </summary>
        public static bVector4 NotEqual(int lhs, int rhs) => new bVector4(lhs != rhs);
        
        /// <summary>
        /// Returns a bVector4 from component-wise application of GreaterThan (lhs &gt; rhs).
        /// </summary>
        public static bVector4 GreaterThan(iQuaternion lhs, iQuaternion rhs) => new bVector4(lhs.x > rhs.x, lhs.y > rhs.y, lhs.z > rhs.z, lhs.w > rhs.w);
        
        /// <summary>
        /// Returns a bVector4 from component-wise application of GreaterThan (lhs &gt; rhs).
        /// </summary>
        public static bVector4 GreaterThan(iQuaternion lhs, int rhs) => new bVector4(lhs.x > rhs, lhs.y > rhs, lhs.z > rhs, lhs.w > rhs);
        
        /// <summary>
        /// Returns a bVector4 from component-wise application of GreaterThan (lhs &gt; rhs).
        /// </summary>
        public static bVector4 GreaterThan(int lhs, iQuaternion rhs) => new bVector4(lhs > rhs.x, lhs > rhs.y, lhs > rhs.z, lhs > rhs.w);
        
        /// <summary>
        /// Returns a bVector from the application of GreaterThan (lhs &gt; rhs).
        /// </summary>
        public static bVector4 GreaterThan(int lhs, int rhs) => new bVector4(lhs > rhs);
        
        /// <summary>
        /// Returns a bVector4 from component-wise application of GreaterThanEqual (lhs &gt;= rhs).
        /// </summary>
        public static bVector4 GreaterThanEqual(iQuaternion lhs, iQuaternion rhs) => new bVector4(lhs.x >= rhs.x, lhs.y >= rhs.y, lhs.z >= rhs.z, lhs.w >= rhs.w);
        
        /// <summary>
        /// Returns a bVector4 from component-wise application of GreaterThanEqual (lhs &gt;= rhs).
        /// </summary>
        public static bVector4 GreaterThanEqual(iQuaternion lhs, int rhs) => new bVector4(lhs.x >= rhs, lhs.y >= rhs, lhs.z >= rhs, lhs.w >= rhs);
        
        /// <summary>
        /// Returns a bVector4 from component-wise application of GreaterThanEqual (lhs &gt;= rhs).
        /// </summary>
        public static bVector4 GreaterThanEqual(int lhs, iQuaternion rhs) => new bVector4(lhs >= rhs.x, lhs >= rhs.y, lhs >= rhs.z, lhs >= rhs.w);
        
        /// <summary>
        /// Returns a bVector from the application of GreaterThanEqual (lhs &gt;= rhs).
        /// </summary>
        public static bVector4 GreaterThanEqual(int lhs, int rhs) => new bVector4(lhs >= rhs);
        
        /// <summary>
        /// Returns a bVector4 from component-wise application of LesserThan (lhs &lt; rhs).
        /// </summary>
        public static bVector4 LesserThan(iQuaternion lhs, iQuaternion rhs) => new bVector4(lhs.x < rhs.x, lhs.y < rhs.y, lhs.z < rhs.z, lhs.w < rhs.w);
        
        /// <summary>
        /// Returns a bVector4 from component-wise application of LesserThan (lhs &lt; rhs).
        /// </summary>
        public static bVector4 LesserThan(iQuaternion lhs, int rhs) => new bVector4(lhs.x < rhs, lhs.y < rhs, lhs.z < rhs, lhs.w < rhs);
        
        /// <summary>
        /// Returns a bVector4 from component-wise application of LesserThan (lhs &lt; rhs).
        /// </summary>
        public static bVector4 LesserThan(int lhs, iQuaternion rhs) => new bVector4(lhs < rhs.x, lhs < rhs.y, lhs < rhs.z, lhs < rhs.w);
        
        /// <summary>
        /// Returns a bVector from the application of LesserThan (lhs &lt; rhs).
        /// </summary>
        public static bVector4 LesserThan(int lhs, int rhs) => new bVector4(lhs < rhs);
        
        /// <summary>
        /// Returns a bVector4 from component-wise application of LesserThanEqual (lhs &lt;= rhs).
        /// </summary>
        public static bVector4 LesserThanEqual(iQuaternion lhs, iQuaternion rhs) => new bVector4(lhs.x <= rhs.x, lhs.y <= rhs.y, lhs.z <= rhs.z, lhs.w <= rhs.w);
        
        /// <summary>
        /// Returns a bVector4 from component-wise application of LesserThanEqual (lhs &lt;= rhs).
        /// </summary>
        public static bVector4 LesserThanEqual(iQuaternion lhs, int rhs) => new bVector4(lhs.x <= rhs, lhs.y <= rhs, lhs.z <= rhs, lhs.w <= rhs);
        
        /// <summary>
        /// Returns a bVector4 from component-wise application of LesserThanEqual (lhs &lt;= rhs).
        /// </summary>
        public static bVector4 LesserThanEqual(int lhs, iQuaternion rhs) => new bVector4(lhs <= rhs.x, lhs <= rhs.y, lhs <= rhs.z, lhs <= rhs.w);
        
        /// <summary>
        /// Returns a bVector from the application of LesserThanEqual (lhs &lt;= rhs).
        /// </summary>
        public static bVector4 LesserThanEqual(int lhs, int rhs) => new bVector4(lhs <= rhs);
        
        /// <summary>
        /// Returns a iQuaternion from component-wise application of Lerp (min * (1-a) + max * a).
        /// </summary>
        public static iQuaternion Lerp(iQuaternion min, iQuaternion max, iQuaternion a) => new iQuaternion(min.x * (1-a.x) + max.x * a.x, min.y * (1-a.y) + max.y * a.y, min.z * (1-a.z) + max.z * a.z, min.w * (1-a.w) + max.w * a.w);
        
        /// <summary>
        /// Returns a iQuaternion from component-wise application of Lerp (min * (1-a) + max * a).
        /// </summary>
        public static iQuaternion Lerp(iQuaternion min, iQuaternion max, int a) => new iQuaternion(min.x * (1-a) + max.x * a, min.y * (1-a) + max.y * a, min.z * (1-a) + max.z * a, min.w * (1-a) + max.w * a);
        
        /// <summary>
        /// Returns a iQuaternion from component-wise application of Lerp (min * (1-a) + max * a).
        /// </summary>
        public static iQuaternion Lerp(iQuaternion min, int max, iQuaternion a) => new iQuaternion(min.x * (1-a.x) + max * a.x, min.y * (1-a.y) + max * a.y, min.z * (1-a.z) + max * a.z, min.w * (1-a.w) + max * a.w);
        
        /// <summary>
        /// Returns a iQuaternion from component-wise application of Lerp (min * (1-a) + max * a).
        /// </summary>
        public static iQuaternion Lerp(iQuaternion min, int max, int a) => new iQuaternion(min.x * (1-a) + max * a, min.y * (1-a) + max * a, min.z * (1-a) + max * a, min.w * (1-a) + max * a);
        
        /// <summary>
        /// Returns a iQuaternion from component-wise application of Lerp (min * (1-a) + max * a).
        /// </summary>
        public static iQuaternion Lerp(int min, iQuaternion max, iQuaternion a) => new iQuaternion(min * (1-a.x) + max.x * a.x, min * (1-a.y) + max.y * a.y, min * (1-a.z) + max.z * a.z, min * (1-a.w) + max.w * a.w);
        
        /// <summary>
        /// Returns a iQuaternion from component-wise application of Lerp (min * (1-a) + max * a).
        /// </summary>
        public static iQuaternion Lerp(int min, iQuaternion max, int a) => new iQuaternion(min * (1-a) + max.x * a, min * (1-a) + max.y * a, min * (1-a) + max.z * a, min * (1-a) + max.w * a);
        
        /// <summary>
        /// Returns a iQuaternion from component-wise application of Lerp (min * (1-a) + max * a).
        /// </summary>
        public static iQuaternion Lerp(int min, int max, iQuaternion a) => new iQuaternion(min * (1-a.x) + max * a.x, min * (1-a.y) + max * a.y, min * (1-a.z) + max * a.z, min * (1-a.w) + max * a.w);
        
        /// <summary>
        /// Returns a iQuaternion from the application of Lerp (min * (1-a) + max * a).
        /// </summary>
        public static iQuaternion Lerp(int min, int max, int a) => new iQuaternion(min * (1-a) + max * a);

        #endregion


        #region Component-Wise Operator Overloads
        
        /// <summary>
        /// Returns a bVector4 from component-wise application of operator&lt; (lhs &lt; rhs).
        /// </summary>
        public static bVector4 operator<(iQuaternion lhs, iQuaternion rhs) => new bVector4(lhs.x < rhs.x, lhs.y < rhs.y, lhs.z < rhs.z, lhs.w < rhs.w);
        
        /// <summary>
        /// Returns a bVector4 from component-wise application of operator&lt; (lhs &lt; rhs).
        /// </summary>
        public static bVector4 operator<(iQuaternion lhs, int rhs) => new bVector4(lhs.x < rhs, lhs.y < rhs, lhs.z < rhs, lhs.w < rhs);
        
        /// <summary>
        /// Returns a bVector4 from component-wise application of operator&lt; (lhs &lt; rhs).
        /// </summary>
        public static bVector4 operator<(int lhs, iQuaternion rhs) => new bVector4(lhs < rhs.x, lhs < rhs.y, lhs < rhs.z, lhs < rhs.w);
        
        /// <summary>
        /// Returns a bVector4 from component-wise application of operator&lt;= (lhs &lt;= rhs).
        /// </summary>
        public static bVector4 operator<=(iQuaternion lhs, iQuaternion rhs) => new bVector4(lhs.x <= rhs.x, lhs.y <= rhs.y, lhs.z <= rhs.z, lhs.w <= rhs.w);
        
        /// <summary>
        /// Returns a bVector4 from component-wise application of operator&lt;= (lhs &lt;= rhs).
        /// </summary>
        public static bVector4 operator<=(iQuaternion lhs, int rhs) => new bVector4(lhs.x <= rhs, lhs.y <= rhs, lhs.z <= rhs, lhs.w <= rhs);
        
        /// <summary>
        /// Returns a bVector4 from component-wise application of operator&lt;= (lhs &lt;= rhs).
        /// </summary>
        public static bVector4 operator<=(int lhs, iQuaternion rhs) => new bVector4(lhs <= rhs.x, lhs <= rhs.y, lhs <= rhs.z, lhs <= rhs.w);
        
        /// <summary>
        /// Returns a bVector4 from component-wise application of operator&gt; (lhs &gt; rhs).
        /// </summary>
        public static bVector4 operator>(iQuaternion lhs, iQuaternion rhs) => new bVector4(lhs.x > rhs.x, lhs.y > rhs.y, lhs.z > rhs.z, lhs.w > rhs.w);
        
        /// <summary>
        /// Returns a bVector4 from component-wise application of operator&gt; (lhs &gt; rhs).
        /// </summary>
        public static bVector4 operator>(iQuaternion lhs, int rhs) => new bVector4(lhs.x > rhs, lhs.y > rhs, lhs.z > rhs, lhs.w > rhs);
        
        /// <summary>
        /// Returns a bVector4 from component-wise application of operator&gt; (lhs &gt; rhs).
        /// </summary>
        public static bVector4 operator>(int lhs, iQuaternion rhs) => new bVector4(lhs > rhs.x, lhs > rhs.y, lhs > rhs.z, lhs > rhs.w);
        
        /// <summary>
        /// Returns a bVector4 from component-wise application of operator&gt;= (lhs &gt;= rhs).
        /// </summary>
        public static bVector4 operator>=(iQuaternion lhs, iQuaternion rhs) => new bVector4(lhs.x >= rhs.x, lhs.y >= rhs.y, lhs.z >= rhs.z, lhs.w >= rhs.w);
        
        /// <summary>
        /// Returns a bVector4 from component-wise application of operator&gt;= (lhs &gt;= rhs).
        /// </summary>
        public static bVector4 operator>=(iQuaternion lhs, int rhs) => new bVector4(lhs.x >= rhs, lhs.y >= rhs, lhs.z >= rhs, lhs.w >= rhs);
        
        /// <summary>
        /// Returns a bVector4 from component-wise application of operator&gt;= (lhs &gt;= rhs).
        /// </summary>
        public static bVector4 operator>=(int lhs, iQuaternion rhs) => new bVector4(lhs >= rhs.x, lhs >= rhs.y, lhs >= rhs.z, lhs >= rhs.w);
        
        /// <summary>
        /// Returns a iQuaternion from component-wise application of operator+ (identity).
        /// </summary>
        public static iQuaternion operator+(iQuaternion v) => v;
        
        /// <summary>
        /// Returns a iQuaternion from component-wise application of operator- (-v).
        /// </summary>
        public static iQuaternion operator-(iQuaternion v) => new iQuaternion(-v.x, -v.y, -v.z, -v.w);
        
        /// <summary>
        /// Returns a iQuaternion from component-wise application of operator+ (lhs + rhs).
        /// </summary>
        public static iQuaternion operator+(iQuaternion lhs, iQuaternion rhs) => new iQuaternion(lhs.x + rhs.x, lhs.y + rhs.y, lhs.z + rhs.z, lhs.w + rhs.w);
        
        /// <summary>
        /// Returns a iQuaternion from component-wise application of operator+ (lhs + rhs).
        /// </summary>
        public static iQuaternion operator+(iQuaternion lhs, int rhs) => new iQuaternion(lhs.x + rhs, lhs.y + rhs, lhs.z + rhs, lhs.w + rhs);
        
        /// <summary>
        /// Returns a iQuaternion from component-wise application of operator+ (lhs + rhs).
        /// </summary>
        public static iQuaternion operator+(int lhs, iQuaternion rhs) => new iQuaternion(lhs + rhs.x, lhs + rhs.y, lhs + rhs.z, lhs + rhs.w);
        
        /// <summary>
        /// Returns a iQuaternion from component-wise application of operator- (lhs - rhs).
        /// </summary>
        public static iQuaternion operator-(iQuaternion lhs, iQuaternion rhs) => new iQuaternion(lhs.x - rhs.x, lhs.y - rhs.y, lhs.z - rhs.z, lhs.w - rhs.w);
        
        /// <summary>
        /// Returns a iQuaternion from component-wise application of operator- (lhs - rhs).
        /// </summary>
        public static iQuaternion operator-(iQuaternion lhs, int rhs) => new iQuaternion(lhs.x - rhs, lhs.y - rhs, lhs.z - rhs, lhs.w - rhs);
        
        /// <summary>
        /// Returns a iQuaternion from component-wise application of operator- (lhs - rhs).
        /// </summary>
        public static iQuaternion operator-(int lhs, iQuaternion rhs) => new iQuaternion(lhs - rhs.x, lhs - rhs.y, lhs - rhs.z, lhs - rhs.w);
        
        /// <summary>
        /// Returns a iQuaternion from component-wise application of operator* (lhs * rhs).
        /// </summary>
        public static iQuaternion operator*(iQuaternion lhs, int rhs) => new iQuaternion(lhs.x * rhs, lhs.y * rhs, lhs.z * rhs, lhs.w * rhs);
        
        /// <summary>
        /// Returns a iQuaternion from component-wise application of operator* (lhs * rhs).
        /// </summary>
        public static iQuaternion operator*(int lhs, iQuaternion rhs) => new iQuaternion(lhs * rhs.x, lhs * rhs.y, lhs * rhs.z, lhs * rhs.w);
        
        /// <summary>
        /// Returns a iQuaternion from component-wise application of operator/ (lhs / rhs).
        /// </summary>
        public static iQuaternion operator/(iQuaternion lhs, int rhs) => new iQuaternion(lhs.x / rhs, lhs.y / rhs, lhs.z / rhs, lhs.w / rhs);

        #endregion

    }
}
