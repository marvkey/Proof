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
    /// A Quaternionernion of type double.
    /// </summary>
    
    
    [StructLayout(LayoutKind.Sequential)]
    public struct dQuaternion : IReadOnlyList<double>, IEquatable<dQuaternion>
    {

        #region Fields
        
        /// <summary>
        /// x-component
        /// </summary>
        
        public double x;
        
        /// <summary>
        /// y-component
        /// </summary>
        
        public double y;
        
        /// <summary>
        /// z-component
        /// </summary>
        
        public double z;
        
        /// <summary>
        /// w-component
        /// </summary>
        
        public double w;

        #endregion


        #region Constructors
        
        /// <summary>
        /// Component-wise constructor
        /// </summary>
        public dQuaternion(double x, double y, double z, double w)
        {
            this.x = x;
            this.y = y;
            this.z = z;
            this.w = w;
        }
        
        /// <summary>
        /// all-same-value constructor
        /// </summary>
        public dQuaternion(double v)
        {
            this.x = v;
            this.y = v;
            this.z = v;
            this.w = v;
        }
        
        /// <summary>
        /// copy constructor
        /// </summary>
        public dQuaternion(dQuaternion q)
        {
            this.x = q.x;
            this.y = q.y;
            this.z = q.z;
            this.w = q.w;
        }
        
        /// <summary>
        /// Vector-and-scalar constructor (CAUTION: not angle-axis, use FromAngleAxis instead)
        /// </summary>
        public dQuaternion(dVector3 v, double s)
        {
            this.x = v.x;
            this.y = v.y;
            this.z = v.z;
            this.w = s;
        }
        
        /// <summary>
        /// Create a Quaternionernion from two normalized axis (http://lolengine.net/blog/2013/09/18/beautiful-Matrixhs-Quaternionernion-from-Vectors)
        /// </summary>
        public dQuaternion(dVector3 u, dVector3 v)
        {
            var localW = dVector3.Cross(u, v);
            var dot = dVector3.Dot(u, v);
            var q = new dQuaternion(localW.x, localW.y, localW.z, 1.0 + dot).Normalized;
            this.x = q.x;
            this.y = q.y;
            this.z = q.z;
            this.w = q.w;
        }
        
        /// <summary>
        /// Create a Quaternionernion from two normalized axis (http://lolengine.net/blog/2013/09/18/beautiful-Matrixhs-Quaternionernion-from-Vectors)
        /// </summary>
        public dQuaternion(dVector3 eulerAngle)
        {
            var c = dVector3.Cos(eulerAngle / 2);
            var s = dVector3.Sin(eulerAngle / 2);
            this.x = s.x * c.y * c.z - c.x * s.y * s.z;
            this.y = c.x * s.y * c.z + s.x * c.y * s.z;
            this.z = c.x * c.y * s.z - s.x * s.y * c.z;
            this.w = c.x * c.y * c.z + s.x * s.y * s.z;
        }
        
        /// <summary>
        /// Creates a Quaternionernion from the rotational part of a dMatrix3.
        /// </summary>
        public dQuaternion(dMatrix3 m)
            : this(FromMat3(m))
        {
        }
        
        /// <summary>
        /// Creates a Quaternionernion from the rotational part of a dMatrix4.
        /// </summary>
        public dQuaternion(dMatrix4 m)
            : this(FromMat4(m))
        {
        }

        #endregion


        #region Implicit Operators
        
        /// <summary>
        /// Implicitly converts this to a cQuaternion.
        /// </summary>
        //public static implicit operator cQuaternion(dQuaternion v) => new cQuaternion((Complex)v.x, (Complex)v.y, (Complex)v.z, (Complex)v.w);

        #endregion


        #region Explicit Operators
        
        /// <summary>
        /// Explicitly converts this to a iVector4.
        /// </summary>
        public static explicit operator iVector4(dQuaternion v) => new iVector4((int)v.x, (int)v.y, (int)v.z, (int)v.w);
        
        /// <summary>
        /// Explicitly converts this to a iQuaternion.
        /// </summary>
        public static explicit operator iQuaternion(dQuaternion v) => new iQuaternion((int)v.x, (int)v.y, (int)v.z, (int)v.w);
        
        /// <summary>
        /// Explicitly converts this to a uVector4.
        /// </summary>
        public static explicit operator uVector4(dQuaternion v) => new uVector4((uint)v.x, (uint)v.y, (uint)v.z, (uint)v.w);
        
        /// <summary>
        /// Explicitly converts this to a uQuaternion.
        /// </summary>
        public static explicit operator uQuaternion(dQuaternion v) => new uQuaternion((uint)v.x, (uint)v.y, (uint)v.z, (uint)v.w);
        
        /// <summary>
        /// Explicitly converts this to a Vector4.
        /// </summary>
        public static explicit operator Vector4(dQuaternion v) => new Vector4((float)v.x, (float)v.y, (float)v.z, (float)v.w);
        
        /// <summary>
        /// Explicitly converts this to a Quaternion.
        /// </summary>
        public static explicit operator Quaternion(dQuaternion v) => new Quaternion((float)v.x, (float)v.y, (float)v.z, (float)v.w);
        
        /// <summary>
        /// Explicitly converts this to a hVector4.
        /// </summary>
        public static explicit operator hVector4(dQuaternion v) => new hVector4((Half)v.x, (Half)v.y, (Half)v.z, (Half)v.w);
        
        /// <summary>
        /// Explicitly converts this to a hQuaternion.
        /// </summary>
        public static explicit operator hQuaternion(dQuaternion v) => new hQuaternion((Half)v.x, (Half)v.y, (Half)v.z, (Half)v.w);
        
        /// <summary>
        /// Explicitly converts this to a dVector4.
        /// </summary>
        public static explicit operator dVector4(dQuaternion v) => new dVector4((double)v.x, (double)v.y, (double)v.z, (double)v.w);
        
        /// <summary>
        /// Explicitly converts this to a decVector4.
        /// </summary>
        public static explicit operator decVector4(dQuaternion v) => new decVector4((decimal)v.x, (decimal)v.y, (decimal)v.z, (decimal)v.w);
        
        /// <summary>
        /// Explicitly converts this to a decQuaternion.
        /// </summary>
        public static explicit operator decQuaternion(dQuaternion v) => new decQuaternion((decimal)v.x, (decimal)v.y, (decimal)v.z, (decimal)v.w);
        
        /// <summary>
        /// Explicitly converts this to a cVector4.
        /// </summary>
        //public static explicit operator cVector4(dQuaternion v) => new cVector4((Complex)v.x, (Complex)v.y, (Complex)v.z, (Complex)v.w);
        
        /// <summary>
        /// Explicitly converts this to a lVector4.
        /// </summary>
        public static explicit operator lVector4(dQuaternion v) => new lVector4((long)v.x, (long)v.y, (long)v.z, (long)v.w);
        
        /// <summary>
        /// Explicitly converts this to a lQuaternion.
        /// </summary>
        public static explicit operator lQuaternion(dQuaternion v) => new lQuaternion((long)v.x, (long)v.y, (long)v.z, (long)v.w);
        
        /// <summary>
        /// Explicitly converts this to a bVector4.
        /// </summary>
        public static explicit operator bVector4(dQuaternion v) => new bVector4(v.x != 0.0, v.y != 0.0, v.z != 0.0, v.w != 0.0);
        
        /// <summary>
        /// Explicitly converts this to a bQuaternion.
        /// </summary>
        public static explicit operator bQuaternion(dQuaternion v) => new bQuaternion(v.x != 0.0, v.y != 0.0, v.z != 0.0, v.w != 0.0);
        
        /// <summary>
        /// Creates a Quaternionernion from the rotational part of a dMatrix3.
        /// </summary>
        public static explicit operator dQuaternion(dMatrix3 m) => FromMat3(m);
        
        /// <summary>
        /// Creates a Quaternionernion from the rotational part of a dMatrix4.
        /// </summary>
        public static explicit operator dQuaternion(dMatrix4 m) => FromMat4(m);

        #endregion


        #region Indexer
        
        /// <summary>
        /// Gets/Sets a specific indexed component (a bit slower than direct access).
        /// </summary>
        public double this[int index]
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
        public double[] Values => new[] { x, y, z, w };
        
        /// <summary>
        /// Returns the number of components (4).
        /// </summary>
        public int Count => 4;
        
        /// <summary>
        /// Returns the euclidean length of this Quaternionernion.
        /// </summary>
        public double Length => (double)Math.Sqrt(((x*x + y*y) + (z*z + w*w)));
        
        /// <summary>
        /// Returns the squared euclidean length of this Quaternionernion.
        /// </summary>
        public double LengthSqr => ((x*x + y*y) + (z*z + w*w));
        
        /// <summary>
        /// Returns a copy of this Quaternionernion with length one (undefined if this has zero length).
        /// </summary>
        public dQuaternion Normalized => this / (double)Length;
        
        /// <summary>
        /// Returns a copy of this Quaternionernion with length one (returns zero if length is zero).
        /// </summary>
        public dQuaternion NormalizedSafe => this == Zero ? Identity : this / (double)Length;
        
        /// <summary>
        /// Returns the represented angle of this Quaternionernion.
        /// </summary>
        public double Angle => Math.Acos((double)w) * 2.0;
        
        /// <summary>
        /// Returns the represented axis of this Quaternionernion.
        /// </summary>
        public dVector3 Axis
        {
            get
            {
                var s1 = 1 - w * w;
                if (s1 < 0) return dVector3.UnitZ;
                var s2 = 1 / Math.Sqrt(s1);
                return new dVector3((double)(x * s2), (double)(y * s2), (double)(z * s2));
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
        /// Creates a dMatrix3 that realizes the rotation of this Quaternionernion
        /// </summary>
        public dMatrix3 ToMat3 => new dMatrix3(1 - 2 * (y*y + z*z), 2 * (x*y + w*z), 2 * (x*z - w*y), 2 * (x*y - w*z), 1 - 2 * (x*x + z*z), 2 * (y*z + w*x), 2 * (x*z + w*y), 2 * (y*z - w*x), 1 - 2 * (x*x + y*y));
        
        /// <summary>
        /// Creates a dMatrix4 that realizes the rotation of this Quaternionernion
        /// </summary>
        public dMatrix4 ToMat4 => new dMatrix4(ToMat3);
        
        /// <summary>
        /// Returns the conjugated Quaternionernion
        /// </summary>
        public dQuaternion Conjugate => new dQuaternion(-x, -y, -z, w);
        
        /// <summary>
        /// Returns the inverse Quaternionernion
        /// </summary>
        public dQuaternion Inverse => Conjugate / LengthSqr;

        #endregion


        #region Static Properties
        
        /// <summary>
        /// Predefined all-zero Quaternionernion
        /// </summary>
        public static dQuaternion Zero { get; } = new dQuaternion(0.0, 0.0, 0.0, 0.0);
        
        /// <summary>
        /// Predefined all-ones Quaternionernion
        /// </summary>
        public static dQuaternion Ones { get; } = new dQuaternion(1.0, 1.0, 1.0, 1.0);
        
        /// <summary>
        /// Predefined identity Quaternionernion
        /// </summary>
        public static dQuaternion Identity { get; } = new dQuaternion(0.0, 0.0, 0.0, 1.0);
        
        /// <summary>
        /// Predefined unit-X Quaternionernion
        /// </summary>
        public static dQuaternion UnitX { get; } = new dQuaternion(1.0, 0.0, 0.0, 0.0);
        
        /// <summary>
        /// Predefined unit-Y Quaternionernion
        /// </summary>
        public static dQuaternion UnitY { get; } = new dQuaternion(0.0, 1.0, 0.0, 0.0);
        
        /// <summary>
        /// Predefined unit-Z Quaternionernion
        /// </summary>
        public static dQuaternion UnitZ { get; } = new dQuaternion(0.0, 0.0, 1.0, 0.0);
        
        /// <summary>
        /// Predefined unit-W Quaternionernion
        /// </summary>
        public static dQuaternion UnitW { get; } = new dQuaternion(0.0, 0.0, 0.0, 1.0);
        
        /// <summary>
        /// Predefined all-MaxValue Quaternionernion
        /// </summary>
        public static dQuaternion MaxValue { get; } = new dQuaternion(double.MaxValue, double.MaxValue, double.MaxValue, double.MaxValue);
        
        /// <summary>
        /// Predefined all-MinValue Quaternionernion
        /// </summary>
        public static dQuaternion MinValue { get; } = new dQuaternion(double.MinValue, double.MinValue, double.MinValue, double.MinValue);
        
        /// <summary>
        /// Predefined all-Epsilon Quaternionernion
        /// </summary>
        public static dQuaternion Epsilon { get; } = new dQuaternion(double.Epsilon, double.Epsilon, double.Epsilon, double.Epsilon);
        
        /// <summary>
        /// Predefined all-NaN Quaternionernion
        /// </summary>
        public static dQuaternion NaN { get; } = new dQuaternion(double.NaN, double.NaN, double.NaN, double.NaN);
        
        /// <summary>
        /// Predefined all-NegativeInfinity Quaternionernion
        /// </summary>
        public static dQuaternion NegativeInfinity { get; } = new dQuaternion(double.NegativeInfinity, double.NegativeInfinity, double.NegativeInfinity, double.NegativeInfinity);
        
        /// <summary>
        /// Predefined all-PositiveInfinity Quaternionernion
        /// </summary>
        public static dQuaternion PositiveInfinity { get; } = new dQuaternion(double.PositiveInfinity, double.PositiveInfinity, double.PositiveInfinity, double.PositiveInfinity);

        #endregion


        #region Operators
        
        /// <summary>
        /// Returns true iff this equals rhs component-wise.
        /// </summary>
        public static bool operator==(dQuaternion lhs, dQuaternion rhs) => lhs.Equals(rhs);
        
        /// <summary>
        /// Returns true iff this does not equal rhs (component-wise).
        /// </summary>
        public static bool operator!=(dQuaternion lhs, dQuaternion rhs) => !lhs.Equals(rhs);
        
        /// <summary>
        /// Returns proper multiplication of two Quaternionernions.
        /// </summary>
        public static dQuaternion operator*(dQuaternion p, dQuaternion q) => new dQuaternion(p.w * q.x + p.x * q.w + p.y * q.z - p.z * q.y, p.w * q.y + p.y * q.w + p.z * q.x - p.x * q.z, p.w * q.z + p.z * q.w + p.x * q.y - p.y * q.x, p.w * q.w - p.x * q.x - p.y * q.y - p.z * q.z);
        
        /// <summary>
        /// Returns a Vector rotated by the Quaternionernion.
        /// </summary>
        public static dVector3 operator*(dQuaternion q, dVector3 v)
        {
            var qv = new dVector3(q.x, q.y, q.z);
            var uv = dVector3.Cross(qv, v);
            var uuv = dVector3.Cross(qv, uv);
            return v + ((uv * q.w) + uuv) * 2;
        }
        
        /// <summary>
        /// Returns a Vector rotated by the Quaternionernion (preserves v.w).
        /// </summary>
        public static dVector4 operator*(dQuaternion q, dVector4 v) => new dVector4(q * new dVector3(v), v.w);
        
        /// <summary>
        /// Returns a Vector rotated by the inverted Quaternionernion.
        /// </summary>
        public static dVector3 operator*(dVector3 v, dQuaternion q) => q.Inverse * v;
        
        /// <summary>
        /// Returns a Vector rotated by the inverted Quaternionernion (preserves v.w).
        /// </summary>
        public static dVector4 operator*(dVector4 v, dQuaternion q) => q.Inverse * v;

        #endregion


        #region Functions
        
        /// <summary>
        /// Returns an enumerator that iterates through all components.
        /// </summary>
        public IEnumerator<double> GetEnumerator()
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
        public bool Equals(dQuaternion rhs) => ((x.Equals(rhs.x) && y.Equals(rhs.y)) && (z.Equals(rhs.z) && w.Equals(rhs.w)));
        
        /// <summary>
        /// Returns true iff this equals rhs type- and component-wise.
        /// </summary>
        public override bool Equals(object obj)
        {
            if (ReferenceEquals(null, obj)) return false;
            return obj is dQuaternion && Equals((dQuaternion) obj);
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
        public dQuaternion Rotated(double angle, dVector3 v) => this * FromAxisAngle(angle, v);

        #endregion


        #region Static Functions
        
        /// <summary>
        /// Converts the string representation of the Quaternionernion into a Quaternionernion representation (using ', ' as a separator).
        /// </summary>
        public static dQuaternion Parse(string s) => Parse(s, ", ");
        
        /// <summary>
        /// Converts the string representation of the Quaternionernion into a Quaternionernion representation (using a designated separator).
        /// </summary>
        public static dQuaternion Parse(string s, string sep)
        {
            var kvp = s.Split(new[] { sep }, StringSplitOptions.None);
            if (kvp.Length != 4) throw new FormatException("input has not exactly 4 parts");
            return new dQuaternion(double.Parse(kvp[0].Trim()), double.Parse(kvp[1].Trim()), double.Parse(kvp[2].Trim()), double.Parse(kvp[3].Trim()));
        }
        
        /// <summary>
        /// Converts the string representation of the Quaternionernion into a Quaternionernion representation (using a designated separator and a type provider).
        /// </summary>
        public static dQuaternion Parse(string s, string sep, IFormatProvider provider)
        {
            var kvp = s.Split(new[] { sep }, StringSplitOptions.None);
            if (kvp.Length != 4) throw new FormatException("input has not exactly 4 parts");
            return new dQuaternion(double.Parse(kvp[0].Trim(), provider), double.Parse(kvp[1].Trim(), provider), double.Parse(kvp[2].Trim(), provider), double.Parse(kvp[3].Trim(), provider));
        }
        
        /// <summary>
        /// Converts the string representation of the Quaternionernion into a Quaternionernion representation (using a designated separator and a number style).
        /// </summary>
        public static dQuaternion Parse(string s, string sep, NumberStyles style)
        {
            var kvp = s.Split(new[] { sep }, StringSplitOptions.None);
            if (kvp.Length != 4) throw new FormatException("input has not exactly 4 parts");
            return new dQuaternion(double.Parse(kvp[0].Trim(), style), double.Parse(kvp[1].Trim(), style), double.Parse(kvp[2].Trim(), style), double.Parse(kvp[3].Trim(), style));
        }
        
        /// <summary>
        /// Converts the string representation of the Quaternionernion into a Quaternionernion representation (using a designated separator and a number style and a forMatrix provider).
        /// </summary>
        public static dQuaternion Parse(string s, string sep, NumberStyles style, IFormatProvider provider)
        {
            var kvp = s.Split(new[] { sep }, StringSplitOptions.None);
            if (kvp.Length != 4) throw new FormatException("input has not exactly 4 parts");
            return new dQuaternion(double.Parse(kvp[0].Trim(), style, provider), double.Parse(kvp[1].Trim(), style, provider), double.Parse(kvp[2].Trim(), style, provider), double.Parse(kvp[3].Trim(), style, provider));
        }
        
        /// <summary>
        /// Tries to convert the string representation of the Quaternionernion into a Quaternionernion representation (using ', ' as a separator), returns false if string was invalid.
        /// </summary>
        public static bool TryParse(string s, out dQuaternion result) => TryParse(s, ", ", out result);
        
        /// <summary>
        /// Tries to convert the string representation of the Quaternionernion into a Quaternionernion representation (using a designated separator), returns false if string was invalid.
        /// </summary>
        public static bool TryParse(string s, string sep, out dQuaternion result)
        {
            result = Zero;
            if (string.IsNullOrEmpty(s)) return false;
            var kvp = s.Split(new[] { sep }, StringSplitOptions.None);
            if (kvp.Length != 4) return false;
            double x = 0.0, y = 0.0, z = 0.0, w = 0.0;
            var ok = ((double.TryParse(kvp[0].Trim(), out x) && double.TryParse(kvp[1].Trim(), out y)) && (double.TryParse(kvp[2].Trim(), out z) && double.TryParse(kvp[3].Trim(), out w)));
            result = ok ? new dQuaternion(x, y, z, w) : Zero;
            return ok;
        }
        
        /// <summary>
        /// Tries to convert the string representation of the Quaternionernion into a Quaternionernion representation (using a designated separator and a number style and a forMatrix provider), returns false if string was invalid.
        /// </summary>
        public static bool TryParse(string s, string sep, NumberStyles style, IFormatProvider provider, out dQuaternion result)
        {
            result = Zero;
            if (string.IsNullOrEmpty(s)) return false;
            var kvp = s.Split(new[] { sep }, StringSplitOptions.None);
            if (kvp.Length != 4) return false;
            double x = 0.0, y = 0.0, z = 0.0, w = 0.0;
            var ok = ((double.TryParse(kvp[0].Trim(), style, provider, out x) && double.TryParse(kvp[1].Trim(), style, provider, out y)) && (double.TryParse(kvp[2].Trim(), style, provider, out z) && double.TryParse(kvp[3].Trim(), style, provider, out w)));
            result = ok ? new dQuaternion(x, y, z, w) : Zero;
            return ok;
        }
        
        /// <summary>
        /// Returns the inner product (dot product, scalar product) of the two Quaternionernions.
        /// </summary>
        public static double Dot(dQuaternion lhs, dQuaternion rhs) => ((lhs.x * rhs.x + lhs.y * rhs.y) + (lhs.z * rhs.z + lhs.w * rhs.w));
        
        /// <summary>
        /// Creates a Quaternionernion from an axis and an angle (in radians).
        /// </summary>
        public static dQuaternion FromAxisAngle(double angle, dVector3 v)
        {
            var s = Math.Sin((double)angle * 0.5);
            var c = Math.Cos((double)angle * 0.5);
            return new dQuaternion((double)((double)v.x * s), (double)((double)v.y * s), (double)((double)v.z * s), (double)c);
        }
        
        /// <summary>
        /// Creates a Quaternionernion from the rotational part of a dMatrix4.
        /// </summary>
        public static dQuaternion FromMat3(dMatrix3 m)
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
                case 0: return new dQuaternion((double)((double)(m.m12 - m.m21) * mult), (double)((double)(m.m20 - m.m02) * mult), (double)((double)(m.m01 - m.m10) * mult), (double)(biggestVal));
                case 1: return new dQuaternion((double)(biggestVal), (double)((double)(m.m01 + m.m10) * mult), (double)((double)(m.m20 + m.m02) * mult), (double)((double)(m.m12 - m.m21) * mult));
                case 2: return new dQuaternion((double)((double)(m.m01 + m.m10) * mult), (double)(biggestVal), (double)((double)(m.m12 + m.m21) * mult), (double)((double)(m.m20 - m.m02) * mult));
                default: return new dQuaternion((double)((double)(m.m20 + m.m02) * mult), (double)((double)(m.m12 + m.m21) * mult), (double)(biggestVal), (double)((double)(m.m01 - m.m10) * mult));
            }
        }
        
        /// <summary>
        /// Creates a Quaternionernion from the rotational part of a dMatrix3.
        /// </summary>
        public static dQuaternion FromMat4(dMatrix4 m) => FromMat3(new dMatrix3(m));
        
        /// <summary>
        /// Returns the cross product between two Quaternionernions.
        /// </summary>
        public static dQuaternion Cross(dQuaternion q1, dQuaternion q2) => new dQuaternion(q1.w * q2.x + q1.x * q2.w + q1.y * q2.z - q1.z * q2.y, q1.w * q2.y + q1.y * q2.w + q1.z * q2.x - q1.x * q2.z, q1.w * q2.z + q1.z * q2.w + q1.x * q2.y - q1.y * q2.x, q1.w * q2.w - q1.x * q2.x - q1.y * q2.y - q1.z * q2.z);
        
        /// <summary>
        /// Calculates a proper spherical interpolation between two Quaternionernions (only works for normalized Quaternionernions).
        /// </summary>
        public static dQuaternion Mix(dQuaternion x, dQuaternion y, double a)
        {
            var cosTheta = (double)Dot(x, y);
            if (cosTheta > 1 - float.Epsilon)
                return Lerp(x, y, a);
            else
            {
                var angle = Math.Acos((double)cosTheta);
                return ( (Math.Sin((1 - (double)a) * angle) * x + Math.Sin((double)a * angle) * y) / Math.Sin(angle) );
            }
        }
        
        /// <summary>
        /// Calculates a proper spherical interpolation between two Quaternionernions (only works for normalized Quaternionernions).
        /// </summary>
        public static dQuaternion SLerp(dQuaternion x, dQuaternion y, double a)
        {
            var z = y;
            var cosTheta = (double)Dot(x, y);
            if (cosTheta < 0) { z = -y; cosTheta = -cosTheta; }
            if (cosTheta > 1 - float.Epsilon)
                return Lerp(x, z, a);
            else
            {
                var angle = Math.Acos((double)cosTheta);
                return ( (Math.Sin((1 - (double)a) * angle) * x + Math.Sin((double)a * angle) * z) / Math.Sin(angle) );
            }
        }
        
        /// <summary>
        /// Applies squad interpolation of these Quaternionernions
        /// </summary>
        public static dQuaternion Squad(dQuaternion q1, dQuaternion q2, dQuaternion s1, dQuaternion s2, double h) => Mix(Mix(q1, q2, h), Mix(s1, s2, h), 2 * (1 - h) * h);

        #endregion


        #region Component-Wise Static Functions
        
        /// <summary>
        /// Returns a bVector4 from component-wise application of IsInfinity (double.IsInfinity(v)).
        /// </summary>
        public static bVector4 IsInfinity(dQuaternion v) => new bVector4(double.IsInfinity(v.x), double.IsInfinity(v.y), double.IsInfinity(v.z), double.IsInfinity(v.w));
        
        /// <summary>
        /// Returns a bVector from the application of IsInfinity (double.IsInfinity(v)).
        /// </summary>
        public static bVector4 IsInfinity(double v) => new bVector4(double.IsInfinity(v));
        
        /// <summary>
        /// Returns a bVector4 from component-wise application of IsFinite (!double.IsNaN(v) &amp;&amp; !double.IsInfinity(v)).
        /// </summary>
        public static bVector4 IsFinite(dQuaternion v) => new bVector4(!double.IsNaN(v.x) && !double.IsInfinity(v.x), !double.IsNaN(v.y) && !double.IsInfinity(v.y), !double.IsNaN(v.z) && !double.IsInfinity(v.z), !double.IsNaN(v.w) && !double.IsInfinity(v.w));
        
        /// <summary>
        /// Returns a bVector from the application of IsFinite (!double.IsNaN(v) &amp;&amp; !double.IsInfinity(v)).
        /// </summary>
        public static bVector4 IsFinite(double v) => new bVector4(!double.IsNaN(v) && !double.IsInfinity(v));
        
        /// <summary>
        /// Returns a bVector4 from component-wise application of IsNaN (double.IsNaN(v)).
        /// </summary>
        public static bVector4 IsNaN(dQuaternion v) => new bVector4(double.IsNaN(v.x), double.IsNaN(v.y), double.IsNaN(v.z), double.IsNaN(v.w));
        
        /// <summary>
        /// Returns a bVector from the application of IsNaN (double.IsNaN(v)).
        /// </summary>
        public static bVector4 IsNaN(double v) => new bVector4(double.IsNaN(v));
        
        /// <summary>
        /// Returns a bVector4 from component-wise application of IsNegativeInfinity (double.IsNegativeInfinity(v)).
        /// </summary>
        public static bVector4 IsNegativeInfinity(dQuaternion v) => new bVector4(double.IsNegativeInfinity(v.x), double.IsNegativeInfinity(v.y), double.IsNegativeInfinity(v.z), double.IsNegativeInfinity(v.w));
        
        /// <summary>
        /// Returns a bVector from the application of IsNegativeInfinity (double.IsNegativeInfinity(v)).
        /// </summary>
        public static bVector4 IsNegativeInfinity(double v) => new bVector4(double.IsNegativeInfinity(v));
        
        /// <summary>
        /// Returns a bVector4 from component-wise application of IsPositiveInfinity (double.IsPositiveInfinity(v)).
        /// </summary>
        public static bVector4 IsPositiveInfinity(dQuaternion v) => new bVector4(double.IsPositiveInfinity(v.x), double.IsPositiveInfinity(v.y), double.IsPositiveInfinity(v.z), double.IsPositiveInfinity(v.w));
        
        /// <summary>
        /// Returns a bVector from the application of IsPositiveInfinity (double.IsPositiveInfinity(v)).
        /// </summary>
        public static bVector4 IsPositiveInfinity(double v) => new bVector4(double.IsPositiveInfinity(v));
        
        /// <summary>
        /// Returns a bVector4 from component-wise application of Equal (lhs == rhs).
        /// </summary>
        public static bVector4 Equal(dQuaternion lhs, dQuaternion rhs) => new bVector4(lhs.x == rhs.x, lhs.y == rhs.y, lhs.z == rhs.z, lhs.w == rhs.w);
        
        /// <summary>
        /// Returns a bVector4 from component-wise application of Equal (lhs == rhs).
        /// </summary>
        public static bVector4 Equal(dQuaternion lhs, double rhs) => new bVector4(lhs.x == rhs, lhs.y == rhs, lhs.z == rhs, lhs.w == rhs);
        
        /// <summary>
        /// Returns a bVector4 from component-wise application of Equal (lhs == rhs).
        /// </summary>
        public static bVector4 Equal(double lhs, dQuaternion rhs) => new bVector4(lhs == rhs.x, lhs == rhs.y, lhs == rhs.z, lhs == rhs.w);
        
        /// <summary>
        /// Returns a bVector from the application of Equal (lhs == rhs).
        /// </summary>
        public static bVector4 Equal(double lhs, double rhs) => new bVector4(lhs == rhs);
        
        /// <summary>
        /// Returns a bVector4 from component-wise application of NotEqual (lhs != rhs).
        /// </summary>
        public static bVector4 NotEqual(dQuaternion lhs, dQuaternion rhs) => new bVector4(lhs.x != rhs.x, lhs.y != rhs.y, lhs.z != rhs.z, lhs.w != rhs.w);
        
        /// <summary>
        /// Returns a bVector4 from component-wise application of NotEqual (lhs != rhs).
        /// </summary>
        public static bVector4 NotEqual(dQuaternion lhs, double rhs) => new bVector4(lhs.x != rhs, lhs.y != rhs, lhs.z != rhs, lhs.w != rhs);
        
        /// <summary>
        /// Returns a bVector4 from component-wise application of NotEqual (lhs != rhs).
        /// </summary>
        public static bVector4 NotEqual(double lhs, dQuaternion rhs) => new bVector4(lhs != rhs.x, lhs != rhs.y, lhs != rhs.z, lhs != rhs.w);
        
        /// <summary>
        /// Returns a bVector from the application of NotEqual (lhs != rhs).
        /// </summary>
        public static bVector4 NotEqual(double lhs, double rhs) => new bVector4(lhs != rhs);
        
        /// <summary>
        /// Returns a bVector4 from component-wise application of GreaterThan (lhs &gt; rhs).
        /// </summary>
        public static bVector4 GreaterThan(dQuaternion lhs, dQuaternion rhs) => new bVector4(lhs.x > rhs.x, lhs.y > rhs.y, lhs.z > rhs.z, lhs.w > rhs.w);
        
        /// <summary>
        /// Returns a bVector4 from component-wise application of GreaterThan (lhs &gt; rhs).
        /// </summary>
        public static bVector4 GreaterThan(dQuaternion lhs, double rhs) => new bVector4(lhs.x > rhs, lhs.y > rhs, lhs.z > rhs, lhs.w > rhs);
        
        /// <summary>
        /// Returns a bVector4 from component-wise application of GreaterThan (lhs &gt; rhs).
        /// </summary>
        public static bVector4 GreaterThan(double lhs, dQuaternion rhs) => new bVector4(lhs > rhs.x, lhs > rhs.y, lhs > rhs.z, lhs > rhs.w);
        
        /// <summary>
        /// Returns a bVector from the application of GreaterThan (lhs &gt; rhs).
        /// </summary>
        public static bVector4 GreaterThan(double lhs, double rhs) => new bVector4(lhs > rhs);
        
        /// <summary>
        /// Returns a bVector4 from component-wise application of GreaterThanEqual (lhs &gt;= rhs).
        /// </summary>
        public static bVector4 GreaterThanEqual(dQuaternion lhs, dQuaternion rhs) => new bVector4(lhs.x >= rhs.x, lhs.y >= rhs.y, lhs.z >= rhs.z, lhs.w >= rhs.w);
        
        /// <summary>
        /// Returns a bVector4 from component-wise application of GreaterThanEqual (lhs &gt;= rhs).
        /// </summary>
        public static bVector4 GreaterThanEqual(dQuaternion lhs, double rhs) => new bVector4(lhs.x >= rhs, lhs.y >= rhs, lhs.z >= rhs, lhs.w >= rhs);
        
        /// <summary>
        /// Returns a bVector4 from component-wise application of GreaterThanEqual (lhs &gt;= rhs).
        /// </summary>
        public static bVector4 GreaterThanEqual(double lhs, dQuaternion rhs) => new bVector4(lhs >= rhs.x, lhs >= rhs.y, lhs >= rhs.z, lhs >= rhs.w);
        
        /// <summary>
        /// Returns a bVector from the application of GreaterThanEqual (lhs &gt;= rhs).
        /// </summary>
        public static bVector4 GreaterThanEqual(double lhs, double rhs) => new bVector4(lhs >= rhs);
        
        /// <summary>
        /// Returns a bVector4 from component-wise application of LesserThan (lhs &lt; rhs).
        /// </summary>
        public static bVector4 LesserThan(dQuaternion lhs, dQuaternion rhs) => new bVector4(lhs.x < rhs.x, lhs.y < rhs.y, lhs.z < rhs.z, lhs.w < rhs.w);
        
        /// <summary>
        /// Returns a bVector4 from component-wise application of LesserThan (lhs &lt; rhs).
        /// </summary>
        public static bVector4 LesserThan(dQuaternion lhs, double rhs) => new bVector4(lhs.x < rhs, lhs.y < rhs, lhs.z < rhs, lhs.w < rhs);
        
        /// <summary>
        /// Returns a bVector4 from component-wise application of LesserThan (lhs &lt; rhs).
        /// </summary>
        public static bVector4 LesserThan(double lhs, dQuaternion rhs) => new bVector4(lhs < rhs.x, lhs < rhs.y, lhs < rhs.z, lhs < rhs.w);
        
        /// <summary>
        /// Returns a bVector from the application of LesserThan (lhs &lt; rhs).
        /// </summary>
        public static bVector4 LesserThan(double lhs, double rhs) => new bVector4(lhs < rhs);
        
        /// <summary>
        /// Returns a bVector4 from component-wise application of LesserThanEqual (lhs &lt;= rhs).
        /// </summary>
        public static bVector4 LesserThanEqual(dQuaternion lhs, dQuaternion rhs) => new bVector4(lhs.x <= rhs.x, lhs.y <= rhs.y, lhs.z <= rhs.z, lhs.w <= rhs.w);
        
        /// <summary>
        /// Returns a bVector4 from component-wise application of LesserThanEqual (lhs &lt;= rhs).
        /// </summary>
        public static bVector4 LesserThanEqual(dQuaternion lhs, double rhs) => new bVector4(lhs.x <= rhs, lhs.y <= rhs, lhs.z <= rhs, lhs.w <= rhs);
        
        /// <summary>
        /// Returns a bVector4 from component-wise application of LesserThanEqual (lhs &lt;= rhs).
        /// </summary>
        public static bVector4 LesserThanEqual(double lhs, dQuaternion rhs) => new bVector4(lhs <= rhs.x, lhs <= rhs.y, lhs <= rhs.z, lhs <= rhs.w);
        
        /// <summary>
        /// Returns a bVector from the application of LesserThanEqual (lhs &lt;= rhs).
        /// </summary>
        public static bVector4 LesserThanEqual(double lhs, double rhs) => new bVector4(lhs <= rhs);
        
        /// <summary>
        /// Returns a dQuaternion from component-wise application of Lerp (min * (1-a) + max * a).
        /// </summary>
        public static dQuaternion Lerp(dQuaternion min, dQuaternion max, dQuaternion a) => new dQuaternion(min.x * (1-a.x) + max.x * a.x, min.y * (1-a.y) + max.y * a.y, min.z * (1-a.z) + max.z * a.z, min.w * (1-a.w) + max.w * a.w);
        
        /// <summary>
        /// Returns a dQuaternion from component-wise application of Lerp (min * (1-a) + max * a).
        /// </summary>
        public static dQuaternion Lerp(dQuaternion min, dQuaternion max, double a) => new dQuaternion(min.x * (1-a) + max.x * a, min.y * (1-a) + max.y * a, min.z * (1-a) + max.z * a, min.w * (1-a) + max.w * a);
        
        /// <summary>
        /// Returns a dQuaternion from component-wise application of Lerp (min * (1-a) + max * a).
        /// </summary>
        public static dQuaternion Lerp(dQuaternion min, double max, dQuaternion a) => new dQuaternion(min.x * (1-a.x) + max * a.x, min.y * (1-a.y) + max * a.y, min.z * (1-a.z) + max * a.z, min.w * (1-a.w) + max * a.w);
        
        /// <summary>
        /// Returns a dQuaternion from component-wise application of Lerp (min * (1-a) + max * a).
        /// </summary>
        public static dQuaternion Lerp(dQuaternion min, double max, double a) => new dQuaternion(min.x * (1-a) + max * a, min.y * (1-a) + max * a, min.z * (1-a) + max * a, min.w * (1-a) + max * a);
        
        /// <summary>
        /// Returns a dQuaternion from component-wise application of Lerp (min * (1-a) + max * a).
        /// </summary>
        public static dQuaternion Lerp(double min, dQuaternion max, dQuaternion a) => new dQuaternion(min * (1-a.x) + max.x * a.x, min * (1-a.y) + max.y * a.y, min * (1-a.z) + max.z * a.z, min * (1-a.w) + max.w * a.w);
        
        /// <summary>
        /// Returns a dQuaternion from component-wise application of Lerp (min * (1-a) + max * a).
        /// </summary>
        public static dQuaternion Lerp(double min, dQuaternion max, double a) => new dQuaternion(min * (1-a) + max.x * a, min * (1-a) + max.y * a, min * (1-a) + max.z * a, min * (1-a) + max.w * a);
        
        /// <summary>
        /// Returns a dQuaternion from component-wise application of Lerp (min * (1-a) + max * a).
        /// </summary>
        public static dQuaternion Lerp(double min, double max, dQuaternion a) => new dQuaternion(min * (1-a.x) + max * a.x, min * (1-a.y) + max * a.y, min * (1-a.z) + max * a.z, min * (1-a.w) + max * a.w);
        
        /// <summary>
        /// Returns a dQuaternion from the application of Lerp (min * (1-a) + max * a).
        /// </summary>
        public static dQuaternion Lerp(double min, double max, double a) => new dQuaternion(min * (1-a) + max * a);

        #endregion


        #region Component-Wise Operator Overloads
        
        /// <summary>
        /// Returns a bVector4 from component-wise application of operator&lt; (lhs &lt; rhs).
        /// </summary>
        public static bVector4 operator<(dQuaternion lhs, dQuaternion rhs) => new bVector4(lhs.x < rhs.x, lhs.y < rhs.y, lhs.z < rhs.z, lhs.w < rhs.w);
        
        /// <summary>
        /// Returns a bVector4 from component-wise application of operator&lt; (lhs &lt; rhs).
        /// </summary>
        public static bVector4 operator<(dQuaternion lhs, double rhs) => new bVector4(lhs.x < rhs, lhs.y < rhs, lhs.z < rhs, lhs.w < rhs);
        
        /// <summary>
        /// Returns a bVector4 from component-wise application of operator&lt; (lhs &lt; rhs).
        /// </summary>
        public static bVector4 operator<(double lhs, dQuaternion rhs) => new bVector4(lhs < rhs.x, lhs < rhs.y, lhs < rhs.z, lhs < rhs.w);
        
        /// <summary>
        /// Returns a bVector4 from component-wise application of operator&lt;= (lhs &lt;= rhs).
        /// </summary>
        public static bVector4 operator<=(dQuaternion lhs, dQuaternion rhs) => new bVector4(lhs.x <= rhs.x, lhs.y <= rhs.y, lhs.z <= rhs.z, lhs.w <= rhs.w);
        
        /// <summary>
        /// Returns a bVector4 from component-wise application of operator&lt;= (lhs &lt;= rhs).
        /// </summary>
        public static bVector4 operator<=(dQuaternion lhs, double rhs) => new bVector4(lhs.x <= rhs, lhs.y <= rhs, lhs.z <= rhs, lhs.w <= rhs);
        
        /// <summary>
        /// Returns a bVector4 from component-wise application of operator&lt;= (lhs &lt;= rhs).
        /// </summary>
        public static bVector4 operator<=(double lhs, dQuaternion rhs) => new bVector4(lhs <= rhs.x, lhs <= rhs.y, lhs <= rhs.z, lhs <= rhs.w);
        
        /// <summary>
        /// Returns a bVector4 from component-wise application of operator&gt; (lhs &gt; rhs).
        /// </summary>
        public static bVector4 operator>(dQuaternion lhs, dQuaternion rhs) => new bVector4(lhs.x > rhs.x, lhs.y > rhs.y, lhs.z > rhs.z, lhs.w > rhs.w);
        
        /// <summary>
        /// Returns a bVector4 from component-wise application of operator&gt; (lhs &gt; rhs).
        /// </summary>
        public static bVector4 operator>(dQuaternion lhs, double rhs) => new bVector4(lhs.x > rhs, lhs.y > rhs, lhs.z > rhs, lhs.w > rhs);
        
        /// <summary>
        /// Returns a bVector4 from component-wise application of operator&gt; (lhs &gt; rhs).
        /// </summary>
        public static bVector4 operator>(double lhs, dQuaternion rhs) => new bVector4(lhs > rhs.x, lhs > rhs.y, lhs > rhs.z, lhs > rhs.w);
        
        /// <summary>
        /// Returns a bVector4 from component-wise application of operator&gt;= (lhs &gt;= rhs).
        /// </summary>
        public static bVector4 operator>=(dQuaternion lhs, dQuaternion rhs) => new bVector4(lhs.x >= rhs.x, lhs.y >= rhs.y, lhs.z >= rhs.z, lhs.w >= rhs.w);
        
        /// <summary>
        /// Returns a bVector4 from component-wise application of operator&gt;= (lhs &gt;= rhs).
        /// </summary>
        public static bVector4 operator>=(dQuaternion lhs, double rhs) => new bVector4(lhs.x >= rhs, lhs.y >= rhs, lhs.z >= rhs, lhs.w >= rhs);
        
        /// <summary>
        /// Returns a bVector4 from component-wise application of operator&gt;= (lhs &gt;= rhs).
        /// </summary>
        public static bVector4 operator>=(double lhs, dQuaternion rhs) => new bVector4(lhs >= rhs.x, lhs >= rhs.y, lhs >= rhs.z, lhs >= rhs.w);
        
        /// <summary>
        /// Returns a dQuaternion from component-wise application of operator+ (identity).
        /// </summary>
        public static dQuaternion operator+(dQuaternion v) => v;
        
        /// <summary>
        /// Returns a dQuaternion from component-wise application of operator- (-v).
        /// </summary>
        public static dQuaternion operator-(dQuaternion v) => new dQuaternion(-v.x, -v.y, -v.z, -v.w);
        
        /// <summary>
        /// Returns a dQuaternion from component-wise application of operator+ (lhs + rhs).
        /// </summary>
        public static dQuaternion operator+(dQuaternion lhs, dQuaternion rhs) => new dQuaternion(lhs.x + rhs.x, lhs.y + rhs.y, lhs.z + rhs.z, lhs.w + rhs.w);
        
        /// <summary>
        /// Returns a dQuaternion from component-wise application of operator+ (lhs + rhs).
        /// </summary>
        public static dQuaternion operator+(dQuaternion lhs, double rhs) => new dQuaternion(lhs.x + rhs, lhs.y + rhs, lhs.z + rhs, lhs.w + rhs);
        
        /// <summary>
        /// Returns a dQuaternion from component-wise application of operator+ (lhs + rhs).
        /// </summary>
        public static dQuaternion operator+(double lhs, dQuaternion rhs) => new dQuaternion(lhs + rhs.x, lhs + rhs.y, lhs + rhs.z, lhs + rhs.w);
        
        /// <summary>
        /// Returns a dQuaternion from component-wise application of operator- (lhs - rhs).
        /// </summary>
        public static dQuaternion operator-(dQuaternion lhs, dQuaternion rhs) => new dQuaternion(lhs.x - rhs.x, lhs.y - rhs.y, lhs.z - rhs.z, lhs.w - rhs.w);
        
        /// <summary>
        /// Returns a dQuaternion from component-wise application of operator- (lhs - rhs).
        /// </summary>
        public static dQuaternion operator-(dQuaternion lhs, double rhs) => new dQuaternion(lhs.x - rhs, lhs.y - rhs, lhs.z - rhs, lhs.w - rhs);
        
        /// <summary>
        /// Returns a dQuaternion from component-wise application of operator- (lhs - rhs).
        /// </summary>
        public static dQuaternion operator-(double lhs, dQuaternion rhs) => new dQuaternion(lhs - rhs.x, lhs - rhs.y, lhs - rhs.z, lhs - rhs.w);
        
        /// <summary>
        /// Returns a dQuaternion from component-wise application of operator* (lhs * rhs).
        /// </summary>
        public static dQuaternion operator*(dQuaternion lhs, double rhs) => new dQuaternion(lhs.x * rhs, lhs.y * rhs, lhs.z * rhs, lhs.w * rhs);
        
        /// <summary>
        /// Returns a dQuaternion from component-wise application of operator* (lhs * rhs).
        /// </summary>
        public static dQuaternion operator*(double lhs, dQuaternion rhs) => new dQuaternion(lhs * rhs.x, lhs * rhs.y, lhs * rhs.z, lhs * rhs.w);
        
        /// <summary>
        /// Returns a dQuaternion from component-wise application of operator/ (lhs / rhs).
        /// </summary>
        public static dQuaternion operator/(dQuaternion lhs, double rhs) => new dQuaternion(lhs.x / rhs, lhs.y / rhs, lhs.z / rhs, lhs.w / rhs);

        #endregion

    }
}
