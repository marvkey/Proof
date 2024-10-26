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
    /// A Quaternionernion of type decimal.
    /// </summary>
    
    
    [StructLayout(LayoutKind.Sequential)]
    public struct decQuaternion : IReadOnlyList<decimal>, IEquatable<decQuaternion>
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
        
        /// <summary>
        /// z-component
        /// </summary>
        
        public decimal z;
        
        /// <summary>
        /// w-component
        /// </summary>
        
        public decimal w;

        #endregion


        #region Constructors
        
        /// <summary>
        /// Component-wise constructor
        /// </summary>
        public decQuaternion(decimal x, decimal y, decimal z, decimal w)
        {
            this.x = x;
            this.y = y;
            this.z = z;
            this.w = w;
        }
        
        /// <summary>
        /// all-same-value constructor
        /// </summary>
        public decQuaternion(decimal v)
        {
            this.x = v;
            this.y = v;
            this.z = v;
            this.w = v;
        }
        
        /// <summary>
        /// copy constructor
        /// </summary>
        public decQuaternion(decQuaternion q)
        {
            this.x = q.x;
            this.y = q.y;
            this.z = q.z;
            this.w = q.w;
        }
        
        /// <summary>
        /// Vector-and-scalar constructor (CAUTION: not angle-axis, use FromAngleAxis instead)
        /// </summary>
        public decQuaternion(decVector3 v, decimal s)
        {
            this.x = v.x;
            this.y = v.y;
            this.z = v.z;
            this.w = s;
        }
        
        /// <summary>
        /// Create a Quaternionernion from two normalized axis (http://lolengine.net/blog/2013/09/18/beautiful-Matrixhs-Quaternionernion-from-Vectors)
        /// </summary>
        public decQuaternion(decVector3 u, decVector3 v)
        {
            var localW = decVector3.Cross(u, v);
            var dot = decVector3.Dot(u, v);
            var q = new decQuaternion(localW.x, localW.y, localW.z, 1m + dot).Normalized;
            this.x = q.x;
            this.y = q.y;
            this.z = q.z;
            this.w = q.w;
        }
        
        /// <summary>
        /// Create a Quaternionernion from two normalized axis (http://lolengine.net/blog/2013/09/18/beautiful-Matrixhs-Quaternionernion-from-Vectors)
        /// </summary>
        public decQuaternion(decVector3 eulerAngle)
        {
            var c = decVector3.Cos(eulerAngle / 2);
            var s = decVector3.Sin(eulerAngle / 2);
            this.x = s.x * c.y * c.z - c.x * s.y * s.z;
            this.y = c.x * s.y * c.z + s.x * c.y * s.z;
            this.z = c.x * c.y * s.z - s.x * s.y * c.z;
            this.w = c.x * c.y * c.z + s.x * s.y * s.z;
        }
        
        /// <summary>
        /// Creates a Quaternionernion from the rotational part of a decMatrix3.
        /// </summary>
        public decQuaternion(decMatrix3 m)
            : this(FromMat3(m))
        {
        }
        
        /// <summary>
        /// Creates a Quaternionernion from the rotational part of a decMatrix4.
        /// </summary>
        public decQuaternion(decMatrix4 m)
            : this(FromMat4(m))
        {
        }

        #endregion


        #region Explicit Operators
        
        /// <summary>
        /// Explicitly converts this to a iVector4.
        /// </summary>
        public static explicit operator iVector4(decQuaternion v) => new iVector4((int)v.x, (int)v.y, (int)v.z, (int)v.w);
        
        /// <summary>
        /// Explicitly converts this to a iQuaternion.
        /// </summary>
        public static explicit operator iQuaternion(decQuaternion v) => new iQuaternion((int)v.x, (int)v.y, (int)v.z, (int)v.w);
        
        /// <summary>
        /// Explicitly converts this to a uVector4.
        /// </summary>
        public static explicit operator uVector4(decQuaternion v) => new uVector4((uint)v.x, (uint)v.y, (uint)v.z, (uint)v.w);
        
        /// <summary>
        /// Explicitly converts this to a uQuaternion.
        /// </summary>
        public static explicit operator uQuaternion(decQuaternion v) => new uQuaternion((uint)v.x, (uint)v.y, (uint)v.z, (uint)v.w);
        
        /// <summary>
        /// Explicitly converts this to a Vector4.
        /// </summary>
        public static explicit operator Vector4(decQuaternion v) => new Vector4((float)v.x, (float)v.y, (float)v.z, (float)v.w);
        
        /// <summary>
        /// Explicitly converts this to a Quaternion.
        /// </summary>
        public static explicit operator Quaternion(decQuaternion v) => new Quaternion((float)v.x, (float)v.y, (float)v.z, (float)v.w);
        
        /// <summary>
        /// Explicitly converts this to a hVector4.
        /// </summary>
        public static explicit operator hVector4(decQuaternion v) => new hVector4((Half)v.x, (Half)v.y, (Half)v.z, (Half)v.w);
        
        /// <summary>
        /// Explicitly converts this to a hQuaternion.
        /// </summary>
        public static explicit operator hQuaternion(decQuaternion v) => new hQuaternion((Half)v.x, (Half)v.y, (Half)v.z, (Half)v.w);
        
        /// <summary>
        /// Explicitly converts this to a dVector4.
        /// </summary>
        public static explicit operator dVector4(decQuaternion v) => new dVector4((double)v.x, (double)v.y, (double)v.z, (double)v.w);
        
        /// <summary>
        /// Explicitly converts this to a dQuaternion.
        /// </summary>
        public static explicit operator dQuaternion(decQuaternion v) => new dQuaternion((double)v.x, (double)v.y, (double)v.z, (double)v.w);
        
        /// <summary>
        /// Explicitly converts this to a decVector4.
        /// </summary>
        public static explicit operator decVector4(decQuaternion v) => new decVector4((decimal)v.x, (decimal)v.y, (decimal)v.z, (decimal)v.w);
        
        /// <summary>
        /// Explicitly converts this to a cVector4.
        /// </summary>
        //public static explicit operator cVector4(decQuaternion v) => new cVector4((Complex)v.x, (Complex)v.y, (Complex)v.z, (Complex)v.w);
        
        /// <summary>
        /// Explicitly converts this to a cQuaternion.
        /// </summary>
       // public static explicit operator cQuaternion(decQuaternion v) => new cQuaternion((Complex)v.x, (Complex)v.y, (Complex)v.z, (Complex)v.w);
        
        /// <summary>
        /// Explicitly converts this to a lVector4.
        /// </summary>
        public static explicit operator lVector4(decQuaternion v) => new lVector4((long)v.x, (long)v.y, (long)v.z, (long)v.w);
        
        /// <summary>
        /// Explicitly converts this to a lQuaternion.
        /// </summary>
        public static explicit operator lQuaternion(decQuaternion v) => new lQuaternion((long)v.x, (long)v.y, (long)v.z, (long)v.w);
        
        /// <summary>
        /// Explicitly converts this to a bVector4.
        /// </summary>
        public static explicit operator bVector4(decQuaternion v) => new bVector4(v.x != 0m, v.y != 0m, v.z != 0m, v.w != 0m);
        
        /// <summary>
        /// Explicitly converts this to a bQuaternion.
        /// </summary>
        public static explicit operator bQuaternion(decQuaternion v) => new bQuaternion(v.x != 0m, v.y != 0m, v.z != 0m, v.w != 0m);
        
        /// <summary>
        /// Creates a Quaternionernion from the rotational part of a decMatrix3.
        /// </summary>
        public static explicit operator decQuaternion(decMatrix3 m) => FromMat3(m);
        
        /// <summary>
        /// Creates a Quaternionernion from the rotational part of a decMatrix4.
        /// </summary>
        public static explicit operator decQuaternion(decMatrix4 m) => FromMat4(m);

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
        public decimal[] Values => new[] { x, y, z, w };
        
        /// <summary>
        /// Returns the number of components (4).
        /// </summary>
        public int Count => 4;
        
        /// <summary>
        /// Returns the euclidean length of this Quaternionernion.
        /// </summary>
        public decimal Length => (decimal)(((x*x + y*y) + (z*z + w*w))).Sqrt();
        
        /// <summary>
        /// Returns the squared euclidean length of this Quaternionernion.
        /// </summary>
        public decimal LengthSqr => ((x*x + y*y) + (z*z + w*w));
        
        /// <summary>
        /// Returns a copy of this Quaternionernion with length one (undefined if this has zero length).
        /// </summary>
        public decQuaternion Normalized => this / (decimal)Length;
        
        /// <summary>
        /// Returns a copy of this Quaternionernion with length one (returns zero if length is zero).
        /// </summary>
        public decQuaternion NormalizedSafe => this == Zero ? Identity : this / (decimal)Length;
        
        /// <summary>
        /// Returns the represented angle of this Quaternionernion.
        /// </summary>
        public double Angle => Math.Acos((double)w) * 2.0;
        
        /// <summary>
        /// Returns the represented axis of this Quaternionernion.
        /// </summary>
        public decVector3 Axis
        {
            get
            {
                var s1 = 1 - w * w;
                if (s1 < 0) return decVector3.UnitZ;
                var s2 = 1 / (s1).Sqrt();
                return new decVector3((decimal)(x * s2), (decimal)(y * s2), (decimal)(z * s2));
            }
        }
        
        /// <summary>
        /// Returns the represented yaw angle of this Quaternionernion.
        /// </summary>
        public double Yaw => Math.Asin(-2.0 * (double)(x * z - w * y));
        
        /// <summary>
        /// Returns the represented pitch angle of this Quaternionernion.
        /// </summary>
        public double Pitch => Math.Atan2(2.0 * (double)(y * z + w * x), (double)(w * w - x * x - y * y + z * z));
        
        /// <summary>
        /// Returns the represented roll angle of this Quaternionernion.
        /// </summary>
        public double Roll => Math.Atan2(2.0 * (double)(x * y + w * z), (double)(w * w + x * x - y * y - z * z));
        
        /// <summary>
        /// Returns the represented euler angles (pitch, yaw, roll) of this Quaternionernion.
        /// </summary>
        public dVector3 EulerAngles => new dVector3(Pitch, Yaw, Roll);
        
        /// <summary>
        /// Creates a decMatrix3 that realizes the rotation of this Quaternionernion
        /// </summary>
        public decMatrix3 ToMat3 => new decMatrix3(1 - 2 * (y*y + z*z), 2 * (x*y + w*z), 2 * (x*z - w*y), 2 * (x*y - w*z), 1 - 2 * (x*x + z*z), 2 * (y*z + w*x), 2 * (x*z + w*y), 2 * (y*z - w*x), 1 - 2 * (x*x + y*y));
        
        /// <summary>
        /// Creates a decMatrix4 that realizes the rotation of this Quaternionernion
        /// </summary>
        public decMatrix4 ToMat4 => new decMatrix4(ToMat3);
        
        /// <summary>
        /// Returns the conjugated Quaternionernion
        /// </summary>
        public decQuaternion Conjugate => new decQuaternion(-x, -y, -z, w);
        
        /// <summary>
        /// Returns the inverse Quaternionernion
        /// </summary>
        public decQuaternion Inverse => Conjugate / LengthSqr;

        #endregion


        #region Static Properties
        
        /// <summary>
        /// Predefined all-zero Quaternionernion
        /// </summary>
        public static decQuaternion Zero { get; } = new decQuaternion(0m, 0m, 0m, 0m);
        
        /// <summary>
        /// Predefined all-ones Quaternionernion
        /// </summary>
        public static decQuaternion Ones { get; } = new decQuaternion(1m, 1m, 1m, 1m);
        
        /// <summary>
        /// Predefined identity Quaternionernion
        /// </summary>
        public static decQuaternion Identity { get; } = new decQuaternion(0m, 0m, 0m, 1m);
        
        /// <summary>
        /// Predefined unit-X Quaternionernion
        /// </summary>
        public static decQuaternion UnitX { get; } = new decQuaternion(1m, 0m, 0m, 0m);
        
        /// <summary>
        /// Predefined unit-Y Quaternionernion
        /// </summary>
        public static decQuaternion UnitY { get; } = new decQuaternion(0m, 1m, 0m, 0m);
        
        /// <summary>
        /// Predefined unit-Z Quaternionernion
        /// </summary>
        public static decQuaternion UnitZ { get; } = new decQuaternion(0m, 0m, 1m, 0m);
        
        /// <summary>
        /// Predefined unit-W Quaternionernion
        /// </summary>
        public static decQuaternion UnitW { get; } = new decQuaternion(0m, 0m, 0m, 1m);
        
        /// <summary>
        /// Predefined all-MaxValue Quaternionernion
        /// </summary>
        public static decQuaternion MaxValue { get; } = new decQuaternion(decimal.MaxValue, decimal.MaxValue, decimal.MaxValue, decimal.MaxValue);
        
        /// <summary>
        /// Predefined all-MinValue Quaternionernion
        /// </summary>
        public static decQuaternion MinValue { get; } = new decQuaternion(decimal.MinValue, decimal.MinValue, decimal.MinValue, decimal.MinValue);
        
        /// <summary>
        /// Predefined all-MinusOne Quaternionernion
        /// </summary>
        public static decQuaternion MinusOne { get; } = new decQuaternion(decimal.MinusOne, decimal.MinusOne, decimal.MinusOne, decimal.MinusOne);

        #endregion


        #region Operators
        
        /// <summary>
        /// Returns true iff this equals rhs component-wise.
        /// </summary>
        public static bool operator==(decQuaternion lhs, decQuaternion rhs) => lhs.Equals(rhs);
        
        /// <summary>
        /// Returns true iff this does not equal rhs (component-wise).
        /// </summary>
        public static bool operator!=(decQuaternion lhs, decQuaternion rhs) => !lhs.Equals(rhs);
        
        /// <summary>
        /// Returns proper multiplication of two Quaternionernions.
        /// </summary>
        public static decQuaternion operator*(decQuaternion p, decQuaternion q) => new decQuaternion(p.w * q.x + p.x * q.w + p.y * q.z - p.z * q.y, p.w * q.y + p.y * q.w + p.z * q.x - p.x * q.z, p.w * q.z + p.z * q.w + p.x * q.y - p.y * q.x, p.w * q.w - p.x * q.x - p.y * q.y - p.z * q.z);
        
        /// <summary>
        /// Returns a Vector rotated by the Quaternionernion.
        /// </summary>
        public static decVector3 operator*(decQuaternion q, decVector3 v)
        {
            var qv = new decVector3(q.x, q.y, q.z);
            var uv = decVector3.Cross(qv, v);
            var uuv = decVector3.Cross(qv, uv);
            return v + ((uv * q.w) + uuv) * 2;
        }
        
        /// <summary>
        /// Returns a Vector rotated by the Quaternionernion (preserves v.w).
        /// </summary>
        public static decVector4 operator*(decQuaternion q, decVector4 v) => new decVector4(q * new decVector3(v), v.w);
        
        /// <summary>
        /// Returns a Vector rotated by the inverted Quaternionernion.
        /// </summary>
        public static decVector3 operator*(decVector3 v, decQuaternion q) => q.Inverse * v;
        
        /// <summary>
        /// Returns a Vector rotated by the inverted Quaternionernion (preserves v.w).
        /// </summary>
        public static decVector4 operator*(decVector4 v, decQuaternion q) => q.Inverse * v;

        #endregion


        #region Functions
        
        /// <summary>
        /// Returns an enumerator that iterates through all components.
        /// </summary>
        public IEnumerator<decimal> GetEnumerator()
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
        public bool Equals(decQuaternion rhs) => ((x.Equals(rhs.x) && y.Equals(rhs.y)) && (z.Equals(rhs.z) && w.Equals(rhs.w)));
        
        /// <summary>
        /// Returns true iff this equals rhs type- and component-wise.
        /// </summary>
        public override bool Equals(object obj)
        {
            if (ReferenceEquals(null, obj)) return false;
            return obj is decQuaternion && Equals((decQuaternion) obj);
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
        
        /// <summary>
        /// Rotates this Quaternionernion from an axis and an angle (in radians).
        /// </summary>
        public decQuaternion Rotated(decimal angle, decVector3 v) => this * FromAxisAngle(angle, v);

        #endregion


        #region Static Functions
        
        /// <summary>
        /// Converts the string representation of the Quaternionernion into a Quaternionernion representation (using ', ' as a separator).
        /// </summary>
        public static decQuaternion Parse(string s) => Parse(s, ", ");
        
        /// <summary>
        /// Converts the string representation of the Quaternionernion into a Quaternionernion representation (using a designated separator).
        /// </summary>
        public static decQuaternion Parse(string s, string sep)
        {
            var kvp = s.Split(new[] { sep }, StringSplitOptions.None);
            if (kvp.Length != 4) throw new FormatException("input has not exactly 4 parts");
            return new decQuaternion(decimal.Parse(kvp[0].Trim()), decimal.Parse(kvp[1].Trim()), decimal.Parse(kvp[2].Trim()), decimal.Parse(kvp[3].Trim()));
        }
        
        /// <summary>
        /// Converts the string representation of the Quaternionernion into a Quaternionernion representation (using a designated separator and a type provider).
        /// </summary>
        public static decQuaternion Parse(string s, string sep, IFormatProvider provider)
        {
            var kvp = s.Split(new[] { sep }, StringSplitOptions.None);
            if (kvp.Length != 4) throw new FormatException("input has not exactly 4 parts");
            return new decQuaternion(decimal.Parse(kvp[0].Trim(), provider), decimal.Parse(kvp[1].Trim(), provider), decimal.Parse(kvp[2].Trim(), provider), decimal.Parse(kvp[3].Trim(), provider));
        }
        
        /// <summary>
        /// Converts the string representation of the Quaternionernion into a Quaternionernion representation (using a designated separator and a number style).
        /// </summary>
        public static decQuaternion Parse(string s, string sep, NumberStyles style)
        {
            var kvp = s.Split(new[] { sep }, StringSplitOptions.None);
            if (kvp.Length != 4) throw new FormatException("input has not exactly 4 parts");
            return new decQuaternion(decimal.Parse(kvp[0].Trim(), style), decimal.Parse(kvp[1].Trim(), style), decimal.Parse(kvp[2].Trim(), style), decimal.Parse(kvp[3].Trim(), style));
        }
        
        /// <summary>
        /// Converts the string representation of the Quaternionernion into a Quaternionernion representation (using a designated separator and a number style and a forMatrix provider).
        /// </summary>
        public static decQuaternion Parse(string s, string sep, NumberStyles style, IFormatProvider provider)
        {
            var kvp = s.Split(new[] { sep }, StringSplitOptions.None);
            if (kvp.Length != 4) throw new FormatException("input has not exactly 4 parts");
            return new decQuaternion(decimal.Parse(kvp[0].Trim(), style, provider), decimal.Parse(kvp[1].Trim(), style, provider), decimal.Parse(kvp[2].Trim(), style, provider), decimal.Parse(kvp[3].Trim(), style, provider));
        }
        
        /// <summary>
        /// Tries to convert the string representation of the Quaternionernion into a Quaternionernion representation (using ', ' as a separator), returns false if string was invalid.
        /// </summary>
        public static bool TryParse(string s, out decQuaternion result) => TryParse(s, ", ", out result);
        
        /// <summary>
        /// Tries to convert the string representation of the Quaternionernion into a Quaternionernion representation (using a designated separator), returns false if string was invalid.
        /// </summary>
        public static bool TryParse(string s, string sep, out decQuaternion result)
        {
            result = Zero;
            if (string.IsNullOrEmpty(s)) return false;
            var kvp = s.Split(new[] { sep }, StringSplitOptions.None);
            if (kvp.Length != 4) return false;
            decimal x = 0m, y = 0m, z = 0m, w = 0m;
            var ok = ((decimal.TryParse(kvp[0].Trim(), out x) && decimal.TryParse(kvp[1].Trim(), out y)) && (decimal.TryParse(kvp[2].Trim(), out z) && decimal.TryParse(kvp[3].Trim(), out w)));
            result = ok ? new decQuaternion(x, y, z, w) : Zero;
            return ok;
        }
        
        /// <summary>
        /// Tries to convert the string representation of the Quaternionernion into a Quaternionernion representation (using a designated separator and a number style and a forMatrix provider), returns false if string was invalid.
        /// </summary>
        public static bool TryParse(string s, string sep, NumberStyles style, IFormatProvider provider, out decQuaternion result)
        {
            result = Zero;
            if (string.IsNullOrEmpty(s)) return false;
            var kvp = s.Split(new[] { sep }, StringSplitOptions.None);
            if (kvp.Length != 4) return false;
            decimal x = 0m, y = 0m, z = 0m, w = 0m;
            var ok = ((decimal.TryParse(kvp[0].Trim(), style, provider, out x) && decimal.TryParse(kvp[1].Trim(), style, provider, out y)) && (decimal.TryParse(kvp[2].Trim(), style, provider, out z) && decimal.TryParse(kvp[3].Trim(), style, provider, out w)));
            result = ok ? new decQuaternion(x, y, z, w) : Zero;
            return ok;
        }
        
        /// <summary>
        /// Returns the inner product (dot product, scalar product) of the two Quaternionernions.
        /// </summary>
        public static decimal Dot(decQuaternion lhs, decQuaternion rhs) => ((lhs.x * rhs.x + lhs.y * rhs.y) + (lhs.z * rhs.z + lhs.w * rhs.w));
        
        /// <summary>
        /// Creates a Quaternionernion from an axis and an angle (in radians).
        /// </summary>
        public static decQuaternion FromAxisAngle(decimal angle, decVector3 v)
        {
            var s = Math.Sin((double)angle * 0.5);
            var c = Math.Cos((double)angle * 0.5);
            return new decQuaternion((decimal)((double)v.x * s), (decimal)((double)v.y * s), (decimal)((double)v.z * s), (decimal)c);
        }
        
        /// <summary>
        /// Creates a Quaternionernion from the rotational part of a decMatrix4.
        /// </summary>
        public static decQuaternion FromMat3(decMatrix3 m)
        {
            var fourXSquaredMinus1 = m.m00 - m.m11 - m.m22;
            var fourYSquaredMinus1 = m.m11 - m.m00 - m.m22;
            var fourZSquaredMinus1 = m.m22 - m.m00 - m.m11;
            var fourWSquaredMinus1 = m.m00 + m.m11 + m.m22;
            var biggestIndex = 0;
            var fourBiggestSquaredMinus1 = fourWSquaredMinus1;
            if(fourXSquaredMinus1 > fourBiggestSquaredMinus1)
            {
                fourBiggestSquaredMinus1 = fourXSquaredMinus1;
                biggestIndex = 1;
            }
            if(fourYSquaredMinus1 > fourBiggestSquaredMinus1)
            {
                fourBiggestSquaredMinus1 = fourYSquaredMinus1;
                biggestIndex = 2;
            }
            if(fourZSquaredMinus1 > fourBiggestSquaredMinus1)
            {
                fourBiggestSquaredMinus1 = fourZSquaredMinus1;
                biggestIndex = 3;
            }
            var biggestVal = Math.Sqrt((double)fourBiggestSquaredMinus1 + 1.0) * 0.5;
            var mult = 0.25 / biggestVal;
            switch(biggestIndex)
            {
                case 0: return new decQuaternion((decimal)((double)(m.m12 - m.m21) * mult), (decimal)((double)(m.m20 - m.m02) * mult), (decimal)((double)(m.m01 - m.m10) * mult), (decimal)(biggestVal));
                case 1: return new decQuaternion((decimal)(biggestVal), (decimal)((double)(m.m01 + m.m10) * mult), (decimal)((double)(m.m20 + m.m02) * mult), (decimal)((double)(m.m12 - m.m21) * mult));
                case 2: return new decQuaternion((decimal)((double)(m.m01 + m.m10) * mult), (decimal)(biggestVal), (decimal)((double)(m.m12 + m.m21) * mult), (decimal)((double)(m.m20 - m.m02) * mult));
                default: return new decQuaternion((decimal)((double)(m.m20 + m.m02) * mult), (decimal)((double)(m.m12 + m.m21) * mult), (decimal)(biggestVal), (decimal)((double)(m.m01 - m.m10) * mult));
            }
        }
        
        /// <summary>
        /// Creates a Quaternionernion from the rotational part of a decMatrix3.
        /// </summary>
        public static decQuaternion FromMat4(decMatrix4 m) => FromMat3(new decMatrix3(m));
        
        /// <summary>
        /// Returns the cross product between two Quaternionernions.
        /// </summary>
        public static decQuaternion Cross(decQuaternion q1, decQuaternion q2) => new decQuaternion(q1.w * q2.x + q1.x * q2.w + q1.y * q2.z - q1.z * q2.y, q1.w * q2.y + q1.y * q2.w + q1.z * q2.x - q1.x * q2.z, q1.w * q2.z + q1.z * q2.w + q1.x * q2.y - q1.y * q2.x, q1.w * q2.w - q1.x * q2.x - q1.y * q2.y - q1.z * q2.z);
        
        /// <summary>
        /// Calculates a proper spherical interpolation between two Quaternionernions (only works for normalized Quaternionernions).
        /// </summary>
        public static decQuaternion Mix(decQuaternion x, decQuaternion y, decimal a)
        {
            var cosTheta = (double)Dot(x, y);
            if (cosTheta > 1 - float.Epsilon)
                return Lerp(x, y, a);
            else
            {
                var angle = Math.Acos((double)cosTheta);
                return (decQuaternion)( (Math.Sin((1 - (double)a) * angle) * (dQuaternion)x + Math.Sin((double)a * angle) * (dQuaternion)y) / Math.Sin(angle) );
            }
        }
        
        /// <summary>
        /// Calculates a proper spherical interpolation between two Quaternionernions (only works for normalized Quaternionernions).
        /// </summary>
        public static decQuaternion SLerp(decQuaternion x, decQuaternion y, decimal a)
        {
            var z = y;
            var cosTheta = (double)Dot(x, y);
            if (cosTheta < 0) { z = -y; cosTheta = -cosTheta; }
            if (cosTheta > 1 - float.Epsilon)
                return Lerp(x, z, a);
            else
            {
                var angle = Math.Acos((double)cosTheta);
                return (decQuaternion)( (Math.Sin((1 - (double)a) * angle) * (dQuaternion)x + Math.Sin((double)a * angle) * (dQuaternion)z) / Math.Sin(angle) );
            }
        }
        
        /// <summary>
        /// Applies squad interpolation of these Quaternionernions
        /// </summary>
        public static decQuaternion Squad(decQuaternion q1, decQuaternion q2, decQuaternion s1, decQuaternion s2, decimal h) => Mix(Mix(q1, q2, h), Mix(s1, s2, h), 2 * (1 - h) * h);

        #endregion


        #region Component-Wise Static Functions
        
        /// <summary>
        /// Returns a bVector4 from component-wise application of Equal (lhs == rhs).
        /// </summary>
        public static bVector4 Equal(decQuaternion lhs, decQuaternion rhs) => new bVector4(lhs.x == rhs.x, lhs.y == rhs.y, lhs.z == rhs.z, lhs.w == rhs.w);
        
        /// <summary>
        /// Returns a bVector4 from component-wise application of Equal (lhs == rhs).
        /// </summary>
        public static bVector4 Equal(decQuaternion lhs, decimal rhs) => new bVector4(lhs.x == rhs, lhs.y == rhs, lhs.z == rhs, lhs.w == rhs);
        
        /// <summary>
        /// Returns a bVector4 from component-wise application of Equal (lhs == rhs).
        /// </summary>
        public static bVector4 Equal(decimal lhs, decQuaternion rhs) => new bVector4(lhs == rhs.x, lhs == rhs.y, lhs == rhs.z, lhs == rhs.w);
        
        /// <summary>
        /// Returns a bVector from the application of Equal (lhs == rhs).
        /// </summary>
        public static bVector4 Equal(decimal lhs, decimal rhs) => new bVector4(lhs == rhs);
        
        /// <summary>
        /// Returns a bVector4 from component-wise application of NotEqual (lhs != rhs).
        /// </summary>
        public static bVector4 NotEqual(decQuaternion lhs, decQuaternion rhs) => new bVector4(lhs.x != rhs.x, lhs.y != rhs.y, lhs.z != rhs.z, lhs.w != rhs.w);
        
        /// <summary>
        /// Returns a bVector4 from component-wise application of NotEqual (lhs != rhs).
        /// </summary>
        public static bVector4 NotEqual(decQuaternion lhs, decimal rhs) => new bVector4(lhs.x != rhs, lhs.y != rhs, lhs.z != rhs, lhs.w != rhs);
        
        /// <summary>
        /// Returns a bVector4 from component-wise application of NotEqual (lhs != rhs).
        /// </summary>
        public static bVector4 NotEqual(decimal lhs, decQuaternion rhs) => new bVector4(lhs != rhs.x, lhs != rhs.y, lhs != rhs.z, lhs != rhs.w);
        
        /// <summary>
        /// Returns a bVector from the application of NotEqual (lhs != rhs).
        /// </summary>
        public static bVector4 NotEqual(decimal lhs, decimal rhs) => new bVector4(lhs != rhs);
        
        /// <summary>
        /// Returns a bVector4 from component-wise application of GreaterThan (lhs &gt; rhs).
        /// </summary>
        public static bVector4 GreaterThan(decQuaternion lhs, decQuaternion rhs) => new bVector4(lhs.x > rhs.x, lhs.y > rhs.y, lhs.z > rhs.z, lhs.w > rhs.w);
        
        /// <summary>
        /// Returns a bVector4 from component-wise application of GreaterThan (lhs &gt; rhs).
        /// </summary>
        public static bVector4 GreaterThan(decQuaternion lhs, decimal rhs) => new bVector4(lhs.x > rhs, lhs.y > rhs, lhs.z > rhs, lhs.w > rhs);
        
        /// <summary>
        /// Returns a bVector4 from component-wise application of GreaterThan (lhs &gt; rhs).
        /// </summary>
        public static bVector4 GreaterThan(decimal lhs, decQuaternion rhs) => new bVector4(lhs > rhs.x, lhs > rhs.y, lhs > rhs.z, lhs > rhs.w);
        
        /// <summary>
        /// Returns a bVector from the application of GreaterThan (lhs &gt; rhs).
        /// </summary>
        public static bVector4 GreaterThan(decimal lhs, decimal rhs) => new bVector4(lhs > rhs);
        
        /// <summary>
        /// Returns a bVector4 from component-wise application of GreaterThanEqual (lhs &gt;= rhs).
        /// </summary>
        public static bVector4 GreaterThanEqual(decQuaternion lhs, decQuaternion rhs) => new bVector4(lhs.x >= rhs.x, lhs.y >= rhs.y, lhs.z >= rhs.z, lhs.w >= rhs.w);
        
        /// <summary>
        /// Returns a bVector4 from component-wise application of GreaterThanEqual (lhs &gt;= rhs).
        /// </summary>
        public static bVector4 GreaterThanEqual(decQuaternion lhs, decimal rhs) => new bVector4(lhs.x >= rhs, lhs.y >= rhs, lhs.z >= rhs, lhs.w >= rhs);
        
        /// <summary>
        /// Returns a bVector4 from component-wise application of GreaterThanEqual (lhs &gt;= rhs).
        /// </summary>
        public static bVector4 GreaterThanEqual(decimal lhs, decQuaternion rhs) => new bVector4(lhs >= rhs.x, lhs >= rhs.y, lhs >= rhs.z, lhs >= rhs.w);
        
        /// <summary>
        /// Returns a bVector from the application of GreaterThanEqual (lhs &gt;= rhs).
        /// </summary>
        public static bVector4 GreaterThanEqual(decimal lhs, decimal rhs) => new bVector4(lhs >= rhs);
        
        /// <summary>
        /// Returns a bVector4 from component-wise application of LesserThan (lhs &lt; rhs).
        /// </summary>
        public static bVector4 LesserThan(decQuaternion lhs, decQuaternion rhs) => new bVector4(lhs.x < rhs.x, lhs.y < rhs.y, lhs.z < rhs.z, lhs.w < rhs.w);
        
        /// <summary>
        /// Returns a bVector4 from component-wise application of LesserThan (lhs &lt; rhs).
        /// </summary>
        public static bVector4 LesserThan(decQuaternion lhs, decimal rhs) => new bVector4(lhs.x < rhs, lhs.y < rhs, lhs.z < rhs, lhs.w < rhs);
        
        /// <summary>
        /// Returns a bVector4 from component-wise application of LesserThan (lhs &lt; rhs).
        /// </summary>
        public static bVector4 LesserThan(decimal lhs, decQuaternion rhs) => new bVector4(lhs < rhs.x, lhs < rhs.y, lhs < rhs.z, lhs < rhs.w);
        
        /// <summary>
        /// Returns a bVector from the application of LesserThan (lhs &lt; rhs).
        /// </summary>
        public static bVector4 LesserThan(decimal lhs, decimal rhs) => new bVector4(lhs < rhs);
        
        /// <summary>
        /// Returns a bVector4 from component-wise application of LesserThanEqual (lhs &lt;= rhs).
        /// </summary>
        public static bVector4 LesserThanEqual(decQuaternion lhs, decQuaternion rhs) => new bVector4(lhs.x <= rhs.x, lhs.y <= rhs.y, lhs.z <= rhs.z, lhs.w <= rhs.w);
        
        /// <summary>
        /// Returns a bVector4 from component-wise application of LesserThanEqual (lhs &lt;= rhs).
        /// </summary>
        public static bVector4 LesserThanEqual(decQuaternion lhs, decimal rhs) => new bVector4(lhs.x <= rhs, lhs.y <= rhs, lhs.z <= rhs, lhs.w <= rhs);
        
        /// <summary>
        /// Returns a bVector4 from component-wise application of LesserThanEqual (lhs &lt;= rhs).
        /// </summary>
        public static bVector4 LesserThanEqual(decimal lhs, decQuaternion rhs) => new bVector4(lhs <= rhs.x, lhs <= rhs.y, lhs <= rhs.z, lhs <= rhs.w);
        
        /// <summary>
        /// Returns a bVector from the application of LesserThanEqual (lhs &lt;= rhs).
        /// </summary>
        public static bVector4 LesserThanEqual(decimal lhs, decimal rhs) => new bVector4(lhs <= rhs);
        
        /// <summary>
        /// Returns a decQuaternion from component-wise application of Lerp (min * (1-a) + max * a).
        /// </summary>
        public static decQuaternion Lerp(decQuaternion min, decQuaternion max, decQuaternion a) => new decQuaternion(min.x * (1-a.x) + max.x * a.x, min.y * (1-a.y) + max.y * a.y, min.z * (1-a.z) + max.z * a.z, min.w * (1-a.w) + max.w * a.w);
        
        /// <summary>
        /// Returns a decQuaternion from component-wise application of Lerp (min * (1-a) + max * a).
        /// </summary>
        public static decQuaternion Lerp(decQuaternion min, decQuaternion max, decimal a) => new decQuaternion(min.x * (1-a) + max.x * a, min.y * (1-a) + max.y * a, min.z * (1-a) + max.z * a, min.w * (1-a) + max.w * a);
        
        /// <summary>
        /// Returns a decQuaternion from component-wise application of Lerp (min * (1-a) + max * a).
        /// </summary>
        public static decQuaternion Lerp(decQuaternion min, decimal max, decQuaternion a) => new decQuaternion(min.x * (1-a.x) + max * a.x, min.y * (1-a.y) + max * a.y, min.z * (1-a.z) + max * a.z, min.w * (1-a.w) + max * a.w);
        
        /// <summary>
        /// Returns a decQuaternion from component-wise application of Lerp (min * (1-a) + max * a).
        /// </summary>
        public static decQuaternion Lerp(decQuaternion min, decimal max, decimal a) => new decQuaternion(min.x * (1-a) + max * a, min.y * (1-a) + max * a, min.z * (1-a) + max * a, min.w * (1-a) + max * a);
        
        /// <summary>
        /// Returns a decQuaternion from component-wise application of Lerp (min * (1-a) + max * a).
        /// </summary>
        public static decQuaternion Lerp(decimal min, decQuaternion max, decQuaternion a) => new decQuaternion(min * (1-a.x) + max.x * a.x, min * (1-a.y) + max.y * a.y, min * (1-a.z) + max.z * a.z, min * (1-a.w) + max.w * a.w);
        
        /// <summary>
        /// Returns a decQuaternion from component-wise application of Lerp (min * (1-a) + max * a).
        /// </summary>
        public static decQuaternion Lerp(decimal min, decQuaternion max, decimal a) => new decQuaternion(min * (1-a) + max.x * a, min * (1-a) + max.y * a, min * (1-a) + max.z * a, min * (1-a) + max.w * a);
        
        /// <summary>
        /// Returns a decQuaternion from component-wise application of Lerp (min * (1-a) + max * a).
        /// </summary>
        public static decQuaternion Lerp(decimal min, decimal max, decQuaternion a) => new decQuaternion(min * (1-a.x) + max * a.x, min * (1-a.y) + max * a.y, min * (1-a.z) + max * a.z, min * (1-a.w) + max * a.w);
        
        /// <summary>
        /// Returns a decQuaternion from the application of Lerp (min * (1-a) + max * a).
        /// </summary>
        public static decQuaternion Lerp(decimal min, decimal max, decimal a) => new decQuaternion(min * (1-a) + max * a);

        #endregion


        #region Component-Wise Operator Overloads
        
        /// <summary>
        /// Returns a bVector4 from component-wise application of operator&lt; (lhs &lt; rhs).
        /// </summary>
        public static bVector4 operator<(decQuaternion lhs, decQuaternion rhs) => new bVector4(lhs.x < rhs.x, lhs.y < rhs.y, lhs.z < rhs.z, lhs.w < rhs.w);
        
        /// <summary>
        /// Returns a bVector4 from component-wise application of operator&lt; (lhs &lt; rhs).
        /// </summary>
        public static bVector4 operator<(decQuaternion lhs, decimal rhs) => new bVector4(lhs.x < rhs, lhs.y < rhs, lhs.z < rhs, lhs.w < rhs);
        
        /// <summary>
        /// Returns a bVector4 from component-wise application of operator&lt; (lhs &lt; rhs).
        /// </summary>
        public static bVector4 operator<(decimal lhs, decQuaternion rhs) => new bVector4(lhs < rhs.x, lhs < rhs.y, lhs < rhs.z, lhs < rhs.w);
        
        /// <summary>
        /// Returns a bVector4 from component-wise application of operator&lt;= (lhs &lt;= rhs).
        /// </summary>
        public static bVector4 operator<=(decQuaternion lhs, decQuaternion rhs) => new bVector4(lhs.x <= rhs.x, lhs.y <= rhs.y, lhs.z <= rhs.z, lhs.w <= rhs.w);
        
        /// <summary>
        /// Returns a bVector4 from component-wise application of operator&lt;= (lhs &lt;= rhs).
        /// </summary>
        public static bVector4 operator<=(decQuaternion lhs, decimal rhs) => new bVector4(lhs.x <= rhs, lhs.y <= rhs, lhs.z <= rhs, lhs.w <= rhs);
        
        /// <summary>
        /// Returns a bVector4 from component-wise application of operator&lt;= (lhs &lt;= rhs).
        /// </summary>
        public static bVector4 operator<=(decimal lhs, decQuaternion rhs) => new bVector4(lhs <= rhs.x, lhs <= rhs.y, lhs <= rhs.z, lhs <= rhs.w);
        
        /// <summary>
        /// Returns a bVector4 from component-wise application of operator&gt; (lhs &gt; rhs).
        /// </summary>
        public static bVector4 operator>(decQuaternion lhs, decQuaternion rhs) => new bVector4(lhs.x > rhs.x, lhs.y > rhs.y, lhs.z > rhs.z, lhs.w > rhs.w);
        
        /// <summary>
        /// Returns a bVector4 from component-wise application of operator&gt; (lhs &gt; rhs).
        /// </summary>
        public static bVector4 operator>(decQuaternion lhs, decimal rhs) => new bVector4(lhs.x > rhs, lhs.y > rhs, lhs.z > rhs, lhs.w > rhs);
        
        /// <summary>
        /// Returns a bVector4 from component-wise application of operator&gt; (lhs &gt; rhs).
        /// </summary>
        public static bVector4 operator>(decimal lhs, decQuaternion rhs) => new bVector4(lhs > rhs.x, lhs > rhs.y, lhs > rhs.z, lhs > rhs.w);
        
        /// <summary>
        /// Returns a bVector4 from component-wise application of operator&gt;= (lhs &gt;= rhs).
        /// </summary>
        public static bVector4 operator>=(decQuaternion lhs, decQuaternion rhs) => new bVector4(lhs.x >= rhs.x, lhs.y >= rhs.y, lhs.z >= rhs.z, lhs.w >= rhs.w);
        
        /// <summary>
        /// Returns a bVector4 from component-wise application of operator&gt;= (lhs &gt;= rhs).
        /// </summary>
        public static bVector4 operator>=(decQuaternion lhs, decimal rhs) => new bVector4(lhs.x >= rhs, lhs.y >= rhs, lhs.z >= rhs, lhs.w >= rhs);
        
        /// <summary>
        /// Returns a bVector4 from component-wise application of operator&gt;= (lhs &gt;= rhs).
        /// </summary>
        public static bVector4 operator>=(decimal lhs, decQuaternion rhs) => new bVector4(lhs >= rhs.x, lhs >= rhs.y, lhs >= rhs.z, lhs >= rhs.w);
        
        /// <summary>
        /// Returns a decQuaternion from component-wise application of operator+ (identity).
        /// </summary>
        public static decQuaternion operator+(decQuaternion v) => v;
        
        /// <summary>
        /// Returns a decQuaternion from component-wise application of operator- (-v).
        /// </summary>
        public static decQuaternion operator-(decQuaternion v) => new decQuaternion(-v.x, -v.y, -v.z, -v.w);
        
        /// <summary>
        /// Returns a decQuaternion from component-wise application of operator+ (lhs + rhs).
        /// </summary>
        public static decQuaternion operator+(decQuaternion lhs, decQuaternion rhs) => new decQuaternion(lhs.x + rhs.x, lhs.y + rhs.y, lhs.z + rhs.z, lhs.w + rhs.w);
        
        /// <summary>
        /// Returns a decQuaternion from component-wise application of operator+ (lhs + rhs).
        /// </summary>
        public static decQuaternion operator+(decQuaternion lhs, decimal rhs) => new decQuaternion(lhs.x + rhs, lhs.y + rhs, lhs.z + rhs, lhs.w + rhs);
        
        /// <summary>
        /// Returns a decQuaternion from component-wise application of operator+ (lhs + rhs).
        /// </summary>
        public static decQuaternion operator+(decimal lhs, decQuaternion rhs) => new decQuaternion(lhs + rhs.x, lhs + rhs.y, lhs + rhs.z, lhs + rhs.w);
        
        /// <summary>
        /// Returns a decQuaternion from component-wise application of operator- (lhs - rhs).
        /// </summary>
        public static decQuaternion operator-(decQuaternion lhs, decQuaternion rhs) => new decQuaternion(lhs.x - rhs.x, lhs.y - rhs.y, lhs.z - rhs.z, lhs.w - rhs.w);
        
        /// <summary>
        /// Returns a decQuaternion from component-wise application of operator- (lhs - rhs).
        /// </summary>
        public static decQuaternion operator-(decQuaternion lhs, decimal rhs) => new decQuaternion(lhs.x - rhs, lhs.y - rhs, lhs.z - rhs, lhs.w - rhs);
        
        /// <summary>
        /// Returns a decQuaternion from component-wise application of operator- (lhs - rhs).
        /// </summary>
        public static decQuaternion operator-(decimal lhs, decQuaternion rhs) => new decQuaternion(lhs - rhs.x, lhs - rhs.y, lhs - rhs.z, lhs - rhs.w);
        
        /// <summary>
        /// Returns a decQuaternion from component-wise application of operator* (lhs * rhs).
        /// </summary>
        public static decQuaternion operator*(decQuaternion lhs, decimal rhs) => new decQuaternion(lhs.x * rhs, lhs.y * rhs, lhs.z * rhs, lhs.w * rhs);
        
        /// <summary>
        /// Returns a decQuaternion from component-wise application of operator* (lhs * rhs).
        /// </summary>
        public static decQuaternion operator*(decimal lhs, decQuaternion rhs) => new decQuaternion(lhs * rhs.x, lhs * rhs.y, lhs * rhs.z, lhs * rhs.w);
        
        /// <summary>
        /// Returns a decQuaternion from component-wise application of operator/ (lhs / rhs).
        /// </summary>
        public static decQuaternion operator/(decQuaternion lhs, decimal rhs) => new decQuaternion(lhs.x / rhs, lhs.y / rhs, lhs.z / rhs, lhs.w / rhs);

        #endregion

    }
}
