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
    /// A Quaternionernion of type Half.
    /// </summary>
    [RegisterCoreClassStruct]
    
    [StructLayout(LayoutKind.Sequential)]
    public struct hQuaternion : IReadOnlyList<Half>, IEquatable<hQuaternion>
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
        
        /// <summary>
        /// z-component
        /// </summary>
        
        public Half z;
        
        /// <summary>
        /// w-component
        /// </summary>
        
        public Half w;

        #endregion


        #region Constructors
        
        /// <summary>
        /// Component-wise constructor
        /// </summary>
        public hQuaternion(Half x, Half y, Half z, Half w)
        {
            this.x = x;
            this.y = y;
            this.z = z;
            this.w = w;
        }
        
        /// <summary>
        /// all-same-value constructor
        /// </summary>
        public hQuaternion(Half v)
        {
            this.x = v;
            this.y = v;
            this.z = v;
            this.w = v;
        }
        
        /// <summary>
        /// copy constructor
        /// </summary>
        public hQuaternion(hQuaternion q)
        {
            this.x = q.x;
            this.y = q.y;
            this.z = q.z;
            this.w = q.w;
        }
        
        /// <summary>
        /// Vectortor-and-scalar constructor (CAUTION: not angle-axis, use FromAngleAxis instead)
        /// </summary>
        public hQuaternion(hVector3 v, Half s)
        {
            this.x = v.x;
            this.y = v.y;
            this.z = v.z;
            this.w = s;
        }
        
        /// <summary>
        /// Create a Quaternionernion from two normalized axis (http://lolengine.net/blog/2013/09/18/beautiful-Matrixhs-Quaternionernion-from-Vectortors)
        /// </summary>
        public hQuaternion(hVector3 u, hVector3 v)
        {
            var localW = hVector3.Cross(u, v);
            var dot = hVector3.Dot(u, v);
            var q = new hQuaternion(localW.x, localW.y, localW.z, Half.One + dot).Normalized;
            this.x = q.x;
            this.y = q.y;
            this.z = q.z;
            this.w = q.w;
        }
        
        /// <summary>
        /// Create a Quaternionernion from two normalized axis (http://lolengine.net/blog/2013/09/18/beautiful-Matrixhs-Quaternionernion-from-Vectortors)
        /// </summary>
        public hQuaternion(hVector3 eulerAngle)
        {
            var c = hVector3.Cos(eulerAngle / 2);
            var s = hVector3.Sin(eulerAngle / 2);
            this.x = s.x * c.y * c.z - c.x * s.y * s.z;
            this.y = c.x * s.y * c.z + s.x * c.y * s.z;
            this.z = c.x * c.y * s.z - s.x * s.y * c.z;
            this.w = c.x * c.y * c.z + s.x * s.y * s.z;
        }
        
        /// <summary>
        /// Creates a Quaternionernion from the rotational part of a hMatrix3.
        /// </summary>
        public hQuaternion(hMatrix3 m)
            : this(FromMat3(m))
        {
        }
        
        /// <summary>
        /// Creates a Quaternionernion from the rotational part of a hMatrix4.
        /// </summary>
        public hQuaternion(hMatrix4 m)
            : this(FromMat4(m))
        {
        }

        #endregion


        #region Implicit Operators
        
        /// <summary>
        /// Implicitly converts this to a Quaternion.
        /// </summary>
        public static implicit operator Quaternion(hQuaternion v) => new Quaternion((float)v.x, (float)v.y, (float)v.z, (float)v.w);
        
        /// <summary>
        /// Implicitly converts this to a dQuaternion.
        /// </summary>
        public static implicit operator dQuaternion(hQuaternion v) => new dQuaternion((double)v.x, (double)v.y, (double)v.z, (double)v.w);
        
        /// <summary>
        /// Implicitly converts this to a cQuaternion.
        /// </summary>
        //public static implicit operator cQuaternion(hQuaternion v) => new cQuaternion((Complex)v.x, (Complex)v.y, (Complex)v.z, (Complex)v.w);

        #endregion


        #region Explicit Operators
        
        /// <summary>
        /// Explicitly converts this to a iVector4.
        /// </summary>
        public static explicit operator iVector4(hQuaternion v) => new iVector4((int)v.x, (int)v.y, (int)v.z, (int)v.w);
        
        /// <summary>
        /// Explicitly converts this to a iQuaternion.
        /// </summary>
        public static explicit operator iQuaternion(hQuaternion v) => new iQuaternion((int)v.x, (int)v.y, (int)v.z, (int)v.w);
        
        /// <summary>
        /// Explicitly converts this to a uVector4.
        /// </summary>
        public static explicit operator uVector4(hQuaternion v) => new uVector4((uint)v.x, (uint)v.y, (uint)v.z, (uint)v.w);
        
        /// <summary>
        /// Explicitly converts this to a uQuaternion.
        /// </summary>
        public static explicit operator uQuaternion(hQuaternion v) => new uQuaternion((uint)v.x, (uint)v.y, (uint)v.z, (uint)v.w);
        
        /// <summary>
        /// Explicitly converts this to a Vector4.
        /// </summary>
        public static explicit operator Vector4(hQuaternion v) => new Vector4((float)v.x, (float)v.y, (float)v.z, (float)v.w);
        
        /// <summary>
        /// Explicitly converts this to a hVector4.
        /// </summary>
        public static explicit operator hVector4(hQuaternion v) => new hVector4((Half)v.x, (Half)v.y, (Half)v.z, (Half)v.w);
        
        /// <summary>
        /// Explicitly converts this to a dVector4.
        /// </summary>
        public static explicit operator dVector4(hQuaternion v) => new dVector4((double)v.x, (double)v.y, (double)v.z, (double)v.w);
        
        /// <summary>
        /// Explicitly converts this to a decVector4.
        /// </summary>
        public static explicit operator decVector4(hQuaternion v) => new decVector4((decimal)v.x, (decimal)v.y, (decimal)v.z, (decimal)v.w);
        
        /// <summary>
        /// Explicitly converts this to a decQuaternion.
        /// </summary>
        public static explicit operator decQuaternion(hQuaternion v) => new decQuaternion((decimal)v.x, (decimal)v.y, (decimal)v.z, (decimal)v.w);
        
        /// <summary>
        /// Explicitly converts this to a cVector4.
        /// </summary>
       // public static explicit operator cVector4(hQuaternion v) => new cVector4((Complex)v.x, (Complex)v.y, (Complex)v.z, (Complex)v.w);
        
        /// <summary>
        /// Explicitly converts this to a lVector4.
        /// </summary>
        public static explicit operator lVector4(hQuaternion v) => new lVector4((long)v.x, (long)v.y, (long)v.z, (long)v.w);
        
        /// <summary>
        /// Explicitly converts this to a lQuaternion.
        /// </summary>
        public static explicit operator lQuaternion(hQuaternion v) => new lQuaternion((long)v.x, (long)v.y, (long)v.z, (long)v.w);
        
        /// <summary>
        /// Explicitly converts this to a bVector4.
        /// </summary>
        public static explicit operator bVector4(hQuaternion v) => new bVector4(v.x != Half.Zero, v.y != Half.Zero, v.z != Half.Zero, v.w != Half.Zero);
        
        /// <summary>
        /// Explicitly converts this to a bQuaternion.
        /// </summary>
        public static explicit operator bQuaternion(hQuaternion v) => new bQuaternion(v.x != Half.Zero, v.y != Half.Zero, v.z != Half.Zero, v.w != Half.Zero);
        
        /// <summary>
        /// Creates a Quaternionernion from the rotational part of a hMatrix3.
        /// </summary>
        public static explicit operator hQuaternion(hMatrix3 m) => FromMat3(m);
        
        /// <summary>
        /// Creates a Quaternionernion from the rotational part of a hMatrix4.
        /// </summary>
        public static explicit operator hQuaternion(hMatrix4 m) => FromMat4(m);

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
        public Half[] Values => new[] { x, y, z, w };
        
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
        public Half LengthSqr => ((x*x + y*y) + (z*z + w*w));
        
        /// <summary>
        /// Returns a copy of this Quaternionernion with length one (undefined if this has zero length).
        /// </summary>
        public hQuaternion Normalized => this / (Half)Length;
        
        /// <summary>
        /// Returns a copy of this Quaternionernion with length one (returns zero if length is zero).
        /// </summary>
        public hQuaternion NormalizedSafe => this == Zero ? Identity : this / (Half)Length;
        
        /// <summary>
        /// Returns the represented angle of this Quaternionernion.
        /// </summary>
        public double Angle => Math.Acos((double)w) * 2.0;
        
        /// <summary>
        /// Returns the represented axis of this Quaternionernion.
        /// </summary>
        public hVector3 Axis
        {
            get
            {
                var s1 = 1 - w * w;
                if (s1 < 0) return hVector3.UnitZ;
                var s2 = 1 / Math.Sqrt(s1);
                return new hVector3((Half)(x * s2), (Half)(y * s2), (Half)(z * s2));
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
        /// Creates a hMatrix3 that realizes the rotation of this Quaternionernion
        /// </summary>
        public hMatrix3 ToMat3 => new hMatrix3(1 - 2 * (y*y + z*z), 2 * (x*y + w*z), 2 * (x*z - w*y), 2 * (x*y - w*z), 1 - 2 * (x*x + z*z), 2 * (y*z + w*x), 2 * (x*z + w*y), 2 * (y*z - w*x), 1 - 2 * (x*x + y*y));
        
        /// <summary>
        /// Creates a hMatrix4 that realizes the rotation of this Quaternionernion
        /// </summary>
        public hMatrix4 ToMat4 => new hMatrix4(ToMat3);
        
        /// <summary>
        /// Returns the conjugated Quaternionernion
        /// </summary>
        public hQuaternion Conjugate => new hQuaternion(-x, -y, -z, w);
        
        /// <summary>
        /// Returns the inverse Quaternionernion
        /// </summary>
        public hQuaternion Inverse => Conjugate / LengthSqr;

        #endregion


        #region Static Properties
        
        /// <summary>
        /// Predefined all-zero Quaternionernion
        /// </summary>
        public static hQuaternion Zero { get; } = new hQuaternion(Half.Zero, Half.Zero, Half.Zero, Half.Zero);
        
        /// <summary>
        /// Predefined all-ones Quaternionernion
        /// </summary>
        public static hQuaternion Ones { get; } = new hQuaternion(Half.One, Half.One, Half.One, Half.One);
        
        /// <summary>
        /// Predefined identity Quaternionernion
        /// </summary>
        public static hQuaternion Identity { get; } = new hQuaternion(Half.Zero, Half.Zero, Half.Zero, Half.One);
        
        /// <summary>
        /// Predefined unit-X Quaternionernion
        /// </summary>
        public static hQuaternion UnitX { get; } = new hQuaternion(Half.One, Half.Zero, Half.Zero, Half.Zero);
        
        /// <summary>
        /// Predefined unit-Y Quaternionernion
        /// </summary>
        public static hQuaternion UnitY { get; } = new hQuaternion(Half.Zero, Half.One, Half.Zero, Half.Zero);
        
        /// <summary>
        /// Predefined unit-Z Quaternionernion
        /// </summary>
        public static hQuaternion UnitZ { get; } = new hQuaternion(Half.Zero, Half.Zero, Half.One, Half.Zero);
        
        /// <summary>
        /// Predefined unit-W Quaternionernion
        /// </summary>
        public static hQuaternion UnitW { get; } = new hQuaternion(Half.Zero, Half.Zero, Half.Zero, Half.One);
        
        /// <summary>
        /// Predefined all-MaxValue Quaternionernion
        /// </summary>
        public static hQuaternion MaxValue { get; } = new hQuaternion(Half.MaxValue, Half.MaxValue, Half.MaxValue, Half.MaxValue);
        
        /// <summary>
        /// Predefined all-MinValue Quaternionernion
        /// </summary>
        public static hQuaternion MinValue { get; } = new hQuaternion(Half.MinValue, Half.MinValue, Half.MinValue, Half.MinValue);
        
        /// <summary>
        /// Predefined all-Epsilon Quaternionernion
        /// </summary>
        public static hQuaternion Epsilon { get; } = new hQuaternion(Half.Epsilon, Half.Epsilon, Half.Epsilon, Half.Epsilon);
        
        /// <summary>
        /// Predefined all-NaN Quaternionernion
        /// </summary>
        public static hQuaternion NaN { get; } = new hQuaternion(Half.NaN, Half.NaN, Half.NaN, Half.NaN);
        
        /// <summary>
        /// Predefined all-NegativeInfinity Quaternionernion
        /// </summary>
        public static hQuaternion NegativeInfinity { get; } = new hQuaternion(Half.NegativeInfinity, Half.NegativeInfinity, Half.NegativeInfinity, Half.NegativeInfinity);
        
        /// <summary>
        /// Predefined all-PositiveInfinity Quaternionernion
        /// </summary>
        public static hQuaternion PositiveInfinity { get; } = new hQuaternion(Half.PositiveInfinity, Half.PositiveInfinity, Half.PositiveInfinity, Half.PositiveInfinity);

        #endregion


        #region Operators
        
        /// <summary>
        /// Returns true iff this equals rhs component-wise.
        /// </summary>
        public static bool operator==(hQuaternion lhs, hQuaternion rhs) => lhs.Equals(rhs);
        
        /// <summary>
        /// Returns true iff this does not equal rhs (component-wise).
        /// </summary>
        public static bool operator!=(hQuaternion lhs, hQuaternion rhs) => !lhs.Equals(rhs);
        
        /// <summary>
        /// Returns proper multiplication of two Quaternionernions.
        /// </summary>
        public static hQuaternion operator*(hQuaternion p, hQuaternion q) => new hQuaternion(p.w * q.x + p.x * q.w + p.y * q.z - p.z * q.y, p.w * q.y + p.y * q.w + p.z * q.x - p.x * q.z, p.w * q.z + p.z * q.w + p.x * q.y - p.y * q.x, p.w * q.w - p.x * q.x - p.y * q.y - p.z * q.z);
        
        /// <summary>
        /// Returns a Vectortor rotated by the Quaternionernion.
        /// </summary>
        public static hVector3 operator*(hQuaternion q, hVector3 v)
        {
            var qv = new hVector3(q.x, q.y, q.z);
            var uv = hVector3.Cross(qv, v);
            var uuv = hVector3.Cross(qv, uv);
            return v + ((uv * q.w) + uuv) * 2;
        }
        
        /// <summary>
        /// Returns a Vectortor rotated by the Quaternionernion (preserves v.w).
        /// </summary>
        public static hVector4 operator*(hQuaternion q, hVector4 v) => new hVector4(q * new hVector3(v), v.w);
        
        /// <summary>
        /// Returns a Vectortor rotated by the inverted Quaternionernion.
        /// </summary>
        public static hVector3 operator*(hVector3 v, hQuaternion q) => q.Inverse * v;
        
        /// <summary>
        /// Returns a Vectortor rotated by the inverted Quaternionernion (preserves v.w).
        /// </summary>
        public static hVector4 operator*(hVector4 v, hQuaternion q) => q.Inverse * v;

        #endregion


        #region Functions
        
        /// <summary>
        /// Returns an enumerator that iterates through all components.
        /// </summary>
        public IEnumerator<Half> GetEnumerator()
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
        public bool Equals(hQuaternion rhs) => ((x.Equals(rhs.x) && y.Equals(rhs.y)) && (z.Equals(rhs.z) && w.Equals(rhs.w)));
        
        /// <summary>
        /// Returns true iff this equals rhs type- and component-wise.
        /// </summary>
        public override bool Equals(object obj)
        {
            if (ReferenceEquals(null, obj)) return false;
            return obj is hQuaternion && Equals((hQuaternion) obj);
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
        public hQuaternion Rotated(Half angle, hVector3 v) => this * FromAxisAngle(angle, v);

        #endregion


        #region Static Functions
        
        /// <summary>
        /// Converts the string representation of the Quaternionernion into a Quaternionernion representation (using ', ' as a separator).
        /// </summary>
        public static hQuaternion Parse(string s) => Parse(s, ", ");
        
        /// <summary>
        /// Converts the string representation of the Quaternionernion into a Quaternionernion representation (using a designated separator).
        /// </summary>
        public static hQuaternion Parse(string s, string sep)
        {
            var kvp = s.Split(new[] { sep }, StringSplitOptions.None);
            if (kvp.Length != 4) throw new FormatException("input has not exactly 4 parts");
            return new hQuaternion(Half.Parse(kvp[0].Trim()), Half.Parse(kvp[1].Trim()), Half.Parse(kvp[2].Trim()), Half.Parse(kvp[3].Trim()));
        }
        
        /// <summary>
        /// Converts the string representation of the Quaternionernion into a Quaternionernion representation (using a designated separator and a type provider).
        /// </summary>
        public static hQuaternion Parse(string s, string sep, IFormatProvider provider)
        {
            var kvp = s.Split(new[] { sep }, StringSplitOptions.None);
            if (kvp.Length != 4) throw new FormatException("input has not exactly 4 parts");
            return new hQuaternion(Half.Parse(kvp[0].Trim(), provider), Half.Parse(kvp[1].Trim(), provider), Half.Parse(kvp[2].Trim(), provider), Half.Parse(kvp[3].Trim(), provider));
        }
        
        /// <summary>
        /// Converts the string representation of the Quaternionernion into a Quaternionernion representation (using a designated separator and a number style).
        /// </summary>
        public static hQuaternion Parse(string s, string sep, NumberStyles style)
        {
            var kvp = s.Split(new[] { sep }, StringSplitOptions.None);
            if (kvp.Length != 4) throw new FormatException("input has not exactly 4 parts");
            return new hQuaternion(Half.Parse(kvp[0].Trim(), style), Half.Parse(kvp[1].Trim(), style), Half.Parse(kvp[2].Trim(), style), Half.Parse(kvp[3].Trim(), style));
        }
        
        /// <summary>
        /// Converts the string representation of the Quaternionernion into a Quaternionernion representation (using a designated separator and a number style and a forMatrix provider).
        /// </summary>
        public static hQuaternion Parse(string s, string sep, NumberStyles style, IFormatProvider provider)
        {
            var kvp = s.Split(new[] { sep }, StringSplitOptions.None);
            if (kvp.Length != 4) throw new FormatException("input has not exactly 4 parts");
            return new hQuaternion(Half.Parse(kvp[0].Trim(), style, provider), Half.Parse(kvp[1].Trim(), style, provider), Half.Parse(kvp[2].Trim(), style, provider), Half.Parse(kvp[3].Trim(), style, provider));
        }
        
        /// <summary>
        /// Tries to convert the string representation of the Quaternionernion into a Quaternionernion representation (using ', ' as a separator), returns false if string was invalid.
        /// </summary>
        public static bool TryParse(string s, out hQuaternion result) => TryParse(s, ", ", out result);
        
        /// <summary>
        /// Tries to convert the string representation of the Quaternionernion into a Quaternionernion representation (using a designated separator), returns false if string was invalid.
        /// </summary>
        public static bool TryParse(string s, string sep, out hQuaternion result)
        {
            result = Zero;
            if (string.IsNullOrEmpty(s)) return false;
            var kvp = s.Split(new[] { sep }, StringSplitOptions.None);
            if (kvp.Length != 4) return false;
            Half x = Half.Zero, y = Half.Zero, z = Half.Zero, w = Half.Zero;
            var ok = ((Half.TryParse(kvp[0].Trim(), out x) && Half.TryParse(kvp[1].Trim(), out y)) && (Half.TryParse(kvp[2].Trim(), out z) && Half.TryParse(kvp[3].Trim(), out w)));
            result = ok ? new hQuaternion(x, y, z, w) : Zero;
            return ok;
        }
        
        /// <summary>
        /// Tries to convert the string representation of the Quaternionernion into a Quaternionernion representation (using a designated separator and a number style and a forMatrix provider), returns false if string was invalid.
        /// </summary>
        public static bool TryParse(string s, string sep, NumberStyles style, IFormatProvider provider, out hQuaternion result)
        {
            result = Zero;
            if (string.IsNullOrEmpty(s)) return false;
            var kvp = s.Split(new[] { sep }, StringSplitOptions.None);
            if (kvp.Length != 4) return false;
            Half x = Half.Zero, y = Half.Zero, z = Half.Zero, w = Half.Zero;
            var ok = ((Half.TryParse(kvp[0].Trim(), style, provider, out x) && Half.TryParse(kvp[1].Trim(), style, provider, out y)) && (Half.TryParse(kvp[2].Trim(), style, provider, out z) && Half.TryParse(kvp[3].Trim(), style, provider, out w)));
            result = ok ? new hQuaternion(x, y, z, w) : Zero;
            return ok;
        }
        
        /// <summary>
        /// Returns the inner product (dot product, scalar product) of the two Quaternionernions.
        /// </summary>
        public static Half Dot(hQuaternion lhs, hQuaternion rhs) => ((lhs.x * rhs.x + lhs.y * rhs.y) + (lhs.z * rhs.z + lhs.w * rhs.w));
        
        /// <summary>
        /// Creates a Quaternionernion from an axis and an angle (in radians).
        /// </summary>
        public static hQuaternion FromAxisAngle(Half angle, hVector3 v)
        {
            var s = Math.Sin((double)angle * 0.5);
            var c = Math.Cos((double)angle * 0.5);
            return new hQuaternion((Half)((double)v.x * s), (Half)((double)v.y * s), (Half)((double)v.z * s), (Half)c);
        }
        
        /// <summary>
        /// Creates a Quaternionernion from the rotational part of a hMatrix4.
        /// </summary>
        public static hQuaternion FromMat3(hMatrix3 m)
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
                case 0: return new hQuaternion((Half)((double)(m.m12 - m.m21) * mult), (Half)((double)(m.m20 - m.m02) * mult), (Half)((double)(m.m01 - m.m10) * mult), (Half)(biggestVal));
                case 1: return new hQuaternion((Half)(biggestVal), (Half)((double)(m.m01 + m.m10) * mult), (Half)((double)(m.m20 + m.m02) * mult), (Half)((double)(m.m12 - m.m21) * mult));
                case 2: return new hQuaternion((Half)((double)(m.m01 + m.m10) * mult), (Half)(biggestVal), (Half)((double)(m.m12 + m.m21) * mult), (Half)((double)(m.m20 - m.m02) * mult));
                default: return new hQuaternion((Half)((double)(m.m20 + m.m02) * mult), (Half)((double)(m.m12 + m.m21) * mult), (Half)(biggestVal), (Half)((double)(m.m01 - m.m10) * mult));
            }
        }
        
        /// <summary>
        /// Creates a Quaternionernion from the rotational part of a hMatrix3.
        /// </summary>
        public static hQuaternion FromMat4(hMatrix4 m) => FromMat3(new hMatrix3(m));
        
        /// <summary>
        /// Returns the cross product between two Quaternionernions.
        /// </summary>
        public static hQuaternion Cross(hQuaternion q1, hQuaternion q2) => new hQuaternion(q1.w * q2.x + q1.x * q2.w + q1.y * q2.z - q1.z * q2.y, q1.w * q2.y + q1.y * q2.w + q1.z * q2.x - q1.x * q2.z, q1.w * q2.z + q1.z * q2.w + q1.x * q2.y - q1.y * q2.x, q1.w * q2.w - q1.x * q2.x - q1.y * q2.y - q1.z * q2.z);
        
        /// <summary>
        /// Calculates a proper spherical interpolation between two Quaternionernions (only works for normalized Quaternionernions).
        /// </summary>
        public static hQuaternion Mix(hQuaternion x, hQuaternion y, Half a)
        {
            var cosTheta = (double)Dot(x, y);
            if (cosTheta > 1 - float.Epsilon)
                return Lerp(x, y, a);
            else
            {
                var angle = Math.Acos((double)cosTheta);
                return (hQuaternion)( (Math.Sin((1 - (double)a) * angle) * (dQuaternion)x + Math.Sin((double)a * angle) * (dQuaternion)y) / Math.Sin(angle) );
            }
        }
        
        /// <summary>
        /// Calculates a proper spherical interpolation between two Quaternionernions (only works for normalized Quaternionernions).
        /// </summary>
        public static hQuaternion SLerp(hQuaternion x, hQuaternion y, Half a)
        {
            var z = y;
            var cosTheta = (double)Dot(x, y);
            if (cosTheta < 0) { z = -y; cosTheta = -cosTheta; }
            if (cosTheta > 1 - float.Epsilon)
                return Lerp(x, z, a);
            else
            {
                var angle = Math.Acos((double)cosTheta);
                return (hQuaternion)( (Math.Sin((1 - (double)a) * angle) * (dQuaternion)x + Math.Sin((double)a * angle) * (dQuaternion)z) / Math.Sin(angle) );
            }
        }
        
        /// <summary>
        /// Applies squad interpolation of these Quaternionernions
        /// </summary>
        public static hQuaternion Squad(hQuaternion q1, hQuaternion q2, hQuaternion s1, hQuaternion s2, Half h) => Mix(Mix(q1, q2, h), Mix(s1, s2, h), 2 * (1 - h) * h);

        #endregion


        #region Component-Wise Static Functions
        
        /// <summary>
        /// Returns a bVector4 from component-wise application of IsInfinity (Half.IsInfinity(v)).
        /// </summary>
        public static bVector4 IsInfinity(hQuaternion v) => new bVector4(Half.IsInfinity(v.x), Half.IsInfinity(v.y), Half.IsInfinity(v.z), Half.IsInfinity(v.w));
        
        /// <summary>
        /// Returns a bVector from the application of IsInfinity (Half.IsInfinity(v)).
        /// </summary>
        public static bVector4 IsInfinity(Half v) => new bVector4(Half.IsInfinity(v));
        
        /// <summary>
        /// Returns a bVector4 from component-wise application of IsFinite (!Half.IsNaN(v) &amp;&amp; !Half.IsInfinity(v)).
        /// </summary>
        public static bVector4 IsFinite(hQuaternion v) => new bVector4(!Half.IsNaN(v.x) && !Half.IsInfinity(v.x), !Half.IsNaN(v.y) && !Half.IsInfinity(v.y), !Half.IsNaN(v.z) && !Half.IsInfinity(v.z), !Half.IsNaN(v.w) && !Half.IsInfinity(v.w));
        
        /// <summary>
        /// Returns a bVector from the application of IsFinite (!Half.IsNaN(v) &amp;&amp; !Half.IsInfinity(v)).
        /// </summary>
        public static bVector4 IsFinite(Half v) => new bVector4(!Half.IsNaN(v) && !Half.IsInfinity(v));
        
        /// <summary>
        /// Returns a bVector4 from component-wise application of IsNaN (Half.IsNaN(v)).
        /// </summary>
        public static bVector4 IsNaN(hQuaternion v) => new bVector4(Half.IsNaN(v.x), Half.IsNaN(v.y), Half.IsNaN(v.z), Half.IsNaN(v.w));
        
        /// <summary>
        /// Returns a bVector from the application of IsNaN (Half.IsNaN(v)).
        /// </summary>
        public static bVector4 IsNaN(Half v) => new bVector4(Half.IsNaN(v));
        
        /// <summary>
        /// Returns a bVector4 from component-wise application of IsNegativeInfinity (Half.IsNegativeInfinity(v)).
        /// </summary>
        public static bVector4 IsNegativeInfinity(hQuaternion v) => new bVector4(Half.IsNegativeInfinity(v.x), Half.IsNegativeInfinity(v.y), Half.IsNegativeInfinity(v.z), Half.IsNegativeInfinity(v.w));
        
        /// <summary>
        /// Returns a bVector from the application of IsNegativeInfinity (Half.IsNegativeInfinity(v)).
        /// </summary>
        public static bVector4 IsNegativeInfinity(Half v) => new bVector4(Half.IsNegativeInfinity(v));
        
        /// <summary>
        /// Returns a bVector4 from component-wise application of IsPositiveInfinity (Half.IsPositiveInfinity(v)).
        /// </summary>
        public static bVector4 IsPositiveInfinity(hQuaternion v) => new bVector4(Half.IsPositiveInfinity(v.x), Half.IsPositiveInfinity(v.y), Half.IsPositiveInfinity(v.z), Half.IsPositiveInfinity(v.w));
        
        /// <summary>
        /// Returns a bVector from the application of IsPositiveInfinity (Half.IsPositiveInfinity(v)).
        /// </summary>
        public static bVector4 IsPositiveInfinity(Half v) => new bVector4(Half.IsPositiveInfinity(v));
        
        /// <summary>
        /// Returns a bVector4 from component-wise application of Equal (lhs == rhs).
        /// </summary>
        public static bVector4 Equal(hQuaternion lhs, hQuaternion rhs) => new bVector4(lhs.x == rhs.x, lhs.y == rhs.y, lhs.z == rhs.z, lhs.w == rhs.w);
        
        /// <summary>
        /// Returns a bVector4 from component-wise application of Equal (lhs == rhs).
        /// </summary>
        public static bVector4 Equal(hQuaternion lhs, Half rhs) => new bVector4(lhs.x == rhs, lhs.y == rhs, lhs.z == rhs, lhs.w == rhs);
        
        /// <summary>
        /// Returns a bVector4 from component-wise application of Equal (lhs == rhs).
        /// </summary>
        public static bVector4 Equal(Half lhs, hQuaternion rhs) => new bVector4(lhs == rhs.x, lhs == rhs.y, lhs == rhs.z, lhs == rhs.w);
        
        /// <summary>
        /// Returns a bVector from the application of Equal (lhs == rhs).
        /// </summary>
        public static bVector4 Equal(Half lhs, Half rhs) => new bVector4(lhs == rhs);
        
        /// <summary>
        /// Returns a bVector4 from component-wise application of NotEqual (lhs != rhs).
        /// </summary>
        public static bVector4 NotEqual(hQuaternion lhs, hQuaternion rhs) => new bVector4(lhs.x != rhs.x, lhs.y != rhs.y, lhs.z != rhs.z, lhs.w != rhs.w);
        
        /// <summary>
        /// Returns a bVector4 from component-wise application of NotEqual (lhs != rhs).
        /// </summary>
        public static bVector4 NotEqual(hQuaternion lhs, Half rhs) => new bVector4(lhs.x != rhs, lhs.y != rhs, lhs.z != rhs, lhs.w != rhs);
        
        /// <summary>
        /// Returns a bVector4 from component-wise application of NotEqual (lhs != rhs).
        /// </summary>
        public static bVector4 NotEqual(Half lhs, hQuaternion rhs) => new bVector4(lhs != rhs.x, lhs != rhs.y, lhs != rhs.z, lhs != rhs.w);
        
        /// <summary>
        /// Returns a bVector from the application of NotEqual (lhs != rhs).
        /// </summary>
        public static bVector4 NotEqual(Half lhs, Half rhs) => new bVector4(lhs != rhs);
        
        /// <summary>
        /// Returns a bVector4 from component-wise application of GreaterThan (lhs &gt; rhs).
        /// </summary>
        public static bVector4 GreaterThan(hQuaternion lhs, hQuaternion rhs) => new bVector4(lhs.x > rhs.x, lhs.y > rhs.y, lhs.z > rhs.z, lhs.w > rhs.w);
        
        /// <summary>
        /// Returns a bVector4 from component-wise application of GreaterThan (lhs &gt; rhs).
        /// </summary>
        public static bVector4 GreaterThan(hQuaternion lhs, Half rhs) => new bVector4(lhs.x > rhs, lhs.y > rhs, lhs.z > rhs, lhs.w > rhs);
        
        /// <summary>
        /// Returns a bVector4 from component-wise application of GreaterThan (lhs &gt; rhs).
        /// </summary>
        public static bVector4 GreaterThan(Half lhs, hQuaternion rhs) => new bVector4(lhs > rhs.x, lhs > rhs.y, lhs > rhs.z, lhs > rhs.w);
        
        /// <summary>
        /// Returns a bVector from the application of GreaterThan (lhs &gt; rhs).
        /// </summary>
        public static bVector4 GreaterThan(Half lhs, Half rhs) => new bVector4(lhs > rhs);
        
        /// <summary>
        /// Returns a bVector4 from component-wise application of GreaterThanEqual (lhs &gt;= rhs).
        /// </summary>
        public static bVector4 GreaterThanEqual(hQuaternion lhs, hQuaternion rhs) => new bVector4(lhs.x >= rhs.x, lhs.y >= rhs.y, lhs.z >= rhs.z, lhs.w >= rhs.w);
        
        /// <summary>
        /// Returns a bVector4 from component-wise application of GreaterThanEqual (lhs &gt;= rhs).
        /// </summary>
        public static bVector4 GreaterThanEqual(hQuaternion lhs, Half rhs) => new bVector4(lhs.x >= rhs, lhs.y >= rhs, lhs.z >= rhs, lhs.w >= rhs);
        
        /// <summary>
        /// Returns a bVector4 from component-wise application of GreaterThanEqual (lhs &gt;= rhs).
        /// </summary>
        public static bVector4 GreaterThanEqual(Half lhs, hQuaternion rhs) => new bVector4(lhs >= rhs.x, lhs >= rhs.y, lhs >= rhs.z, lhs >= rhs.w);
        
        /// <summary>
        /// Returns a bVector from the application of GreaterThanEqual (lhs &gt;= rhs).
        /// </summary>
        public static bVector4 GreaterThanEqual(Half lhs, Half rhs) => new bVector4(lhs >= rhs);
        
        /// <summary>
        /// Returns a bVector4 from component-wise application of LesserThan (lhs &lt; rhs).
        /// </summary>
        public static bVector4 LesserThan(hQuaternion lhs, hQuaternion rhs) => new bVector4(lhs.x < rhs.x, lhs.y < rhs.y, lhs.z < rhs.z, lhs.w < rhs.w);
        
        /// <summary>
        /// Returns a bVector4 from component-wise application of LesserThan (lhs &lt; rhs).
        /// </summary>
        public static bVector4 LesserThan(hQuaternion lhs, Half rhs) => new bVector4(lhs.x < rhs, lhs.y < rhs, lhs.z < rhs, lhs.w < rhs);
        
        /// <summary>
        /// Returns a bVector4 from component-wise application of LesserThan (lhs &lt; rhs).
        /// </summary>
        public static bVector4 LesserThan(Half lhs, hQuaternion rhs) => new bVector4(lhs < rhs.x, lhs < rhs.y, lhs < rhs.z, lhs < rhs.w);
        
        /// <summary>
        /// Returns a bVector from the application of LesserThan (lhs &lt; rhs).
        /// </summary>
        public static bVector4 LesserThan(Half lhs, Half rhs) => new bVector4(lhs < rhs);
        
        /// <summary>
        /// Returns a bVector4 from component-wise application of LesserThanEqual (lhs &lt;= rhs).
        /// </summary>
        public static bVector4 LesserThanEqual(hQuaternion lhs, hQuaternion rhs) => new bVector4(lhs.x <= rhs.x, lhs.y <= rhs.y, lhs.z <= rhs.z, lhs.w <= rhs.w);
        
        /// <summary>
        /// Returns a bVector4 from component-wise application of LesserThanEqual (lhs &lt;= rhs).
        /// </summary>
        public static bVector4 LesserThanEqual(hQuaternion lhs, Half rhs) => new bVector4(lhs.x <= rhs, lhs.y <= rhs, lhs.z <= rhs, lhs.w <= rhs);
        
        /// <summary>
        /// Returns a bVector4 from component-wise application of LesserThanEqual (lhs &lt;= rhs).
        /// </summary>
        public static bVector4 LesserThanEqual(Half lhs, hQuaternion rhs) => new bVector4(lhs <= rhs.x, lhs <= rhs.y, lhs <= rhs.z, lhs <= rhs.w);
        
        /// <summary>
        /// Returns a bVector from the application of LesserThanEqual (lhs &lt;= rhs).
        /// </summary>
        public static bVector4 LesserThanEqual(Half lhs, Half rhs) => new bVector4(lhs <= rhs);
        
        /// <summary>
        /// Returns a hQuaternion from component-wise application of Lerp (min * (1-a) + max * a).
        /// </summary>
        public static hQuaternion Lerp(hQuaternion min, hQuaternion max, hQuaternion a) => new hQuaternion(min.x * (1-a.x) + max.x * a.x, min.y * (1-a.y) + max.y * a.y, min.z * (1-a.z) + max.z * a.z, min.w * (1-a.w) + max.w * a.w);
        
        /// <summary>
        /// Returns a hQuaternion from component-wise application of Lerp (min * (1-a) + max * a).
        /// </summary>
        public static hQuaternion Lerp(hQuaternion min, hQuaternion max, Half a) => new hQuaternion(min.x * (1-a) + max.x * a, min.y * (1-a) + max.y * a, min.z * (1-a) + max.z * a, min.w * (1-a) + max.w * a);
        
        /// <summary>
        /// Returns a hQuaternion from component-wise application of Lerp (min * (1-a) + max * a).
        /// </summary>
        public static hQuaternion Lerp(hQuaternion min, Half max, hQuaternion a) => new hQuaternion(min.x * (1-a.x) + max * a.x, min.y * (1-a.y) + max * a.y, min.z * (1-a.z) + max * a.z, min.w * (1-a.w) + max * a.w);
        
        /// <summary>
        /// Returns a hQuaternion from component-wise application of Lerp (min * (1-a) + max * a).
        /// </summary>
        public static hQuaternion Lerp(hQuaternion min, Half max, Half a) => new hQuaternion(min.x * (1-a) + max * a, min.y * (1-a) + max * a, min.z * (1-a) + max * a, min.w * (1-a) + max * a);
        
        /// <summary>
        /// Returns a hQuaternion from component-wise application of Lerp (min * (1-a) + max * a).
        /// </summary>
        public static hQuaternion Lerp(Half min, hQuaternion max, hQuaternion a) => new hQuaternion(min * (1-a.x) + max.x * a.x, min * (1-a.y) + max.y * a.y, min * (1-a.z) + max.z * a.z, min * (1-a.w) + max.w * a.w);
        
        /// <summary>
        /// Returns a hQuaternion from component-wise application of Lerp (min * (1-a) + max * a).
        /// </summary>
        public static hQuaternion Lerp(Half min, hQuaternion max, Half a) => new hQuaternion(min * (1-a) + max.x * a, min * (1-a) + max.y * a, min * (1-a) + max.z * a, min * (1-a) + max.w * a);
        
        /// <summary>
        /// Returns a hQuaternion from component-wise application of Lerp (min * (1-a) + max * a).
        /// </summary>
        public static hQuaternion Lerp(Half min, Half max, hQuaternion a) => new hQuaternion(min * (1-a.x) + max * a.x, min * (1-a.y) + max * a.y, min * (1-a.z) + max * a.z, min * (1-a.w) + max * a.w);
        
        /// <summary>
        /// Returns a hQuaternion from the application of Lerp (min * (1-a) + max * a).
        /// </summary>
        public static hQuaternion Lerp(Half min, Half max, Half a) => new hQuaternion(min * (1-a) + max * a);

        #endregion


        #region Component-Wise Operator Overloads
        
        /// <summary>
        /// Returns a bVector4 from component-wise application of operator&lt; (lhs &lt; rhs).
        /// </summary>
        public static bVector4 operator<(hQuaternion lhs, hQuaternion rhs) => new bVector4(lhs.x < rhs.x, lhs.y < rhs.y, lhs.z < rhs.z, lhs.w < rhs.w);
        
        /// <summary>
        /// Returns a bVector4 from component-wise application of operator&lt; (lhs &lt; rhs).
        /// </summary>
        public static bVector4 operator<(hQuaternion lhs, Half rhs) => new bVector4(lhs.x < rhs, lhs.y < rhs, lhs.z < rhs, lhs.w < rhs);
        
        /// <summary>
        /// Returns a bVector4 from component-wise application of operator&lt; (lhs &lt; rhs).
        /// </summary>
        public static bVector4 operator<(Half lhs, hQuaternion rhs) => new bVector4(lhs < rhs.x, lhs < rhs.y, lhs < rhs.z, lhs < rhs.w);
        
        /// <summary>
        /// Returns a bVector4 from component-wise application of operator&lt;= (lhs &lt;= rhs).
        /// </summary>
        public static bVector4 operator<=(hQuaternion lhs, hQuaternion rhs) => new bVector4(lhs.x <= rhs.x, lhs.y <= rhs.y, lhs.z <= rhs.z, lhs.w <= rhs.w);
        
        /// <summary>
        /// Returns a bVector4 from component-wise application of operator&lt;= (lhs &lt;= rhs).
        /// </summary>
        public static bVector4 operator<=(hQuaternion lhs, Half rhs) => new bVector4(lhs.x <= rhs, lhs.y <= rhs, lhs.z <= rhs, lhs.w <= rhs);
        
        /// <summary>
        /// Returns a bVector4 from component-wise application of operator&lt;= (lhs &lt;= rhs).
        /// </summary>
        public static bVector4 operator<=(Half lhs, hQuaternion rhs) => new bVector4(lhs <= rhs.x, lhs <= rhs.y, lhs <= rhs.z, lhs <= rhs.w);
        
        /// <summary>
        /// Returns a bVector4 from component-wise application of operator&gt; (lhs &gt; rhs).
        /// </summary>
        public static bVector4 operator>(hQuaternion lhs, hQuaternion rhs) => new bVector4(lhs.x > rhs.x, lhs.y > rhs.y, lhs.z > rhs.z, lhs.w > rhs.w);
        
        /// <summary>
        /// Returns a bVector4 from component-wise application of operator&gt; (lhs &gt; rhs).
        /// </summary>
        public static bVector4 operator>(hQuaternion lhs, Half rhs) => new bVector4(lhs.x > rhs, lhs.y > rhs, lhs.z > rhs, lhs.w > rhs);
        
        /// <summary>
        /// Returns a bVector4 from component-wise application of operator&gt; (lhs &gt; rhs).
        /// </summary>
        public static bVector4 operator>(Half lhs, hQuaternion rhs) => new bVector4(lhs > rhs.x, lhs > rhs.y, lhs > rhs.z, lhs > rhs.w);
        
        /// <summary>
        /// Returns a bVector4 from component-wise application of operator&gt;= (lhs &gt;= rhs).
        /// </summary>
        public static bVector4 operator>=(hQuaternion lhs, hQuaternion rhs) => new bVector4(lhs.x >= rhs.x, lhs.y >= rhs.y, lhs.z >= rhs.z, lhs.w >= rhs.w);
        
        /// <summary>
        /// Returns a bVector4 from component-wise application of operator&gt;= (lhs &gt;= rhs).
        /// </summary>
        public static bVector4 operator>=(hQuaternion lhs, Half rhs) => new bVector4(lhs.x >= rhs, lhs.y >= rhs, lhs.z >= rhs, lhs.w >= rhs);
        
        /// <summary>
        /// Returns a bVector4 from component-wise application of operator&gt;= (lhs &gt;= rhs).
        /// </summary>
        public static bVector4 operator>=(Half lhs, hQuaternion rhs) => new bVector4(lhs >= rhs.x, lhs >= rhs.y, lhs >= rhs.z, lhs >= rhs.w);
        
        /// <summary>
        /// Returns a hQuaternion from component-wise application of operator+ (identity).
        /// </summary>
        public static hQuaternion operator+(hQuaternion v) => v;
        
        /// <summary>
        /// Returns a hQuaternion from component-wise application of operator- (-v).
        /// </summary>
        public static hQuaternion operator-(hQuaternion v) => new hQuaternion(-v.x, -v.y, -v.z, -v.w);
        
        /// <summary>
        /// Returns a hQuaternion from component-wise application of operator+ (lhs + rhs).
        /// </summary>
        public static hQuaternion operator+(hQuaternion lhs, hQuaternion rhs) => new hQuaternion(lhs.x + rhs.x, lhs.y + rhs.y, lhs.z + rhs.z, lhs.w + rhs.w);
        
        /// <summary>
        /// Returns a hQuaternion from component-wise application of operator+ (lhs + rhs).
        /// </summary>
        public static hQuaternion operator+(hQuaternion lhs, Half rhs) => new hQuaternion(lhs.x + rhs, lhs.y + rhs, lhs.z + rhs, lhs.w + rhs);
        
        /// <summary>
        /// Returns a hQuaternion from component-wise application of operator+ (lhs + rhs).
        /// </summary>
        public static hQuaternion operator+(Half lhs, hQuaternion rhs) => new hQuaternion(lhs + rhs.x, lhs + rhs.y, lhs + rhs.z, lhs + rhs.w);
        
        /// <summary>
        /// Returns a hQuaternion from component-wise application of operator- (lhs - rhs).
        /// </summary>
        public static hQuaternion operator-(hQuaternion lhs, hQuaternion rhs) => new hQuaternion(lhs.x - rhs.x, lhs.y - rhs.y, lhs.z - rhs.z, lhs.w - rhs.w);
        
        /// <summary>
        /// Returns a hQuaternion from component-wise application of operator- (lhs - rhs).
        /// </summary>
        public static hQuaternion operator-(hQuaternion lhs, Half rhs) => new hQuaternion(lhs.x - rhs, lhs.y - rhs, lhs.z - rhs, lhs.w - rhs);
        
        /// <summary>
        /// Returns a hQuaternion from component-wise application of operator- (lhs - rhs).
        /// </summary>
        public static hQuaternion operator-(Half lhs, hQuaternion rhs) => new hQuaternion(lhs - rhs.x, lhs - rhs.y, lhs - rhs.z, lhs - rhs.w);
        
        /// <summary>
        /// Returns a hQuaternion from component-wise application of operator* (lhs * rhs).
        /// </summary>
        public static hQuaternion operator*(hQuaternion lhs, Half rhs) => new hQuaternion(lhs.x * rhs, lhs.y * rhs, lhs.z * rhs, lhs.w * rhs);
        
        /// <summary>
        /// Returns a hQuaternion from component-wise application of operator* (lhs * rhs).
        /// </summary>
        public static hQuaternion operator*(Half lhs, hQuaternion rhs) => new hQuaternion(lhs * rhs.x, lhs * rhs.y, lhs * rhs.z, lhs * rhs.w);
        
        /// <summary>
        /// Returns a hQuaternion from component-wise application of operator/ (lhs / rhs).
        /// </summary>
        public static hQuaternion operator/(hQuaternion lhs, Half rhs) => new hQuaternion(lhs.x / rhs, lhs.y / rhs, lhs.z / rhs, lhs.w / rhs);

        #endregion

    }
}
