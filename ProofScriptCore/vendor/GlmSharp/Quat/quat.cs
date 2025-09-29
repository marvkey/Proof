using System;
using System.Collections;
using System.Collections.Generic;
using System.Globalization;
using System.Runtime.InteropServices;

using System.Numerics;
using System.Linq;
using Proof.Swizzle;
using System.Runtime.Remoting.Metadata.W3cXsd2001;

// ReSharper disable InconsistentNaming

namespace Proof
{
    
    /// <summary>
    /// A Quaternionernion of type float.
    /// </summary>
    
    
    [StructLayout(LayoutKind.Sequential)]
    public struct Quaternion : IReadOnlyList<float>, IEquatable<Quaternion>
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
        
        /// <summary>
        /// w-component
        /// </summary>
        
        public float w;

        #endregion


        #region Constructors
        
        /// <summary>
        /// Component-wise constructor
        /// </summary>
        public Quaternion(float x, float y, float z, float w)
        {
            this.x = x;
            this.y = y;
            this.z = z;
            this.w = w;
        }
        
        /// <summary>
        /// all-same-value constructor
        /// </summary>
        public Quaternion(float v)
        {
            this.x = v;
            this.y = v;
            this.z = v;
            this.w = v;
        }
        
        /// <summary>
        /// copy constructor
        /// </summary>
        public Quaternion(Quaternion q)
        {
            this.x = q.x;
            this.y = q.y;
            this.z = q.z;
            this.w = q.w;
        }
        
        /// <summary>
        /// Vector-and-scalar constructor (CAUTION: not angle-axis, use FromAngleAxis instead)
        /// </summary>
        public Quaternion(Vector3 v, float s)
        {
            this.x = v.x;
            this.y = v.y;
            this.z = v.z;
            this.w = s;
        }
        
        /// <summary>
        /// Create a Quaternionernion from two normalized axis (http://lolengine.net/blog/2013/09/18/beautiful-Matrixhs-Quaternionernion-from-Vectors)
        /// </summary>
        public Quaternion(Vector3 u, Vector3 v)
        {
            var localW = Vector3.Cross(u, v);
            var dot = Vector3.Dot(u, v);
            var q = new Quaternion(localW.x, localW.y, localW.z, 1f + dot).Normalized;
            this.x = q.x;
            this.y = q.y;
            this.z = q.z;
            this.w = q.w;
        }
        
        /// <summary>
        /// Create a Quaternionernion from two normalized axis (http://lolengine.net/blog/2013/09/18/beautiful-Matrixhs-Quaternionernion-from-Vectors)
        /// </summary>
        public Quaternion(Vector3 eulerAngle)
        {
            var c = Vector3.Cos(eulerAngle / 2);
            var s = Vector3.Sin(eulerAngle / 2);
            this.x = s.x * c.y * c.z - c.x * s.y * s.z;
            this.y = c.x * s.y * c.z + s.x * c.y * s.z;
            this.z = c.x * c.y * s.z - s.x * s.y * c.z;
            this.w = c.x * c.y * c.z + s.x * s.y * s.z;
        }
        
        /// <summary>
        /// Creates a Quaternionernion from the rotational part of a Matrix3.
        /// </summary>
        public Quaternion(Matrix3 m)
            : this(FromMat3(m))
        {
        }
        
        /// <summary>
        /// Creates a Quaternionernion from the rotational part of a Matrix4.
        /// </summary>
        public Quaternion(Matrix4 m)
            : this(FromMat4(m))
        {
        }

        #endregion


        #region Implicit Operators
        
        /// <summary>
        /// Implicitly converts this to a dQuaternion.
        /// </summary>
        public static implicit operator dQuaternion(Quaternion v) => new dQuaternion((double)v.x, (double)v.y, (double)v.z, (double)v.w);
        
        /// <summary>
        /// Implicitly converts this to a cQuaternion.
        /// </summary>
    //    public static implicit operator cQuaternion(Quaternion v) => new cQuaternion((Complex)v.x, (Complex)v.y, (Complex)v.z, (Complex)v.w);

        #endregion


        #region Explicit Operators
        
        /// <summary>
        /// Explicitly converts this to a iVector4.
        /// </summary>
        public static explicit operator iVector4(Quaternion v) => new iVector4((int)v.x, (int)v.y, (int)v.z, (int)v.w);
        
        /// <summary>
        /// Explicitly converts this to a iQuaternion.
        /// </summary>
        public static explicit operator iQuaternion(Quaternion v) => new iQuaternion((int)v.x, (int)v.y, (int)v.z, (int)v.w);
        
        /// <summary>
        /// Explicitly converts this to a uVector4.
        /// </summary>
        public static explicit operator uVector4(Quaternion v) => new uVector4((uint)v.x, (uint)v.y, (uint)v.z, (uint)v.w);
        
        /// <summary>
        /// Explicitly converts this to a uQuaternion.
        /// </summary>
        public static explicit operator uQuaternion(Quaternion v) => new uQuaternion((uint)v.x, (uint)v.y, (uint)v.z, (uint)v.w);
        
        /// <summary>
        /// Explicitly converts this to a Vector4.
        /// </summary>
        public static explicit operator Vector4(Quaternion v) => new Vector4((float)v.x, (float)v.y, (float)v.z, (float)v.w);
        
        /// <summary>
        /// Explicitly converts this to a hVector4.
        /// </summary>
        public static explicit operator hVector4(Quaternion v) => new hVector4((Half)v.x, (Half)v.y, (Half)v.z, (Half)v.w);
        
        /// <summary>
        /// Explicitly converts this to a hQuaternion.
        /// </summary>
        public static explicit operator hQuaternion(Quaternion v) => new hQuaternion((Half)v.x, (Half)v.y, (Half)v.z, (Half)v.w);
        
        /// <summary>
        /// Explicitly converts this to a dVector4.
        /// </summary>
        public static explicit operator dVector4(Quaternion v) => new dVector4((double)v.x, (double)v.y, (double)v.z, (double)v.w);
        
        /// <summary>
        /// Explicitly converts this to a decVector4.
        /// </summary>
        public static explicit operator decVector4(Quaternion v) => new decVector4((decimal)v.x, (decimal)v.y, (decimal)v.z, (decimal)v.w);
        
        /// <summary>
        /// Explicitly converts this to a decQuaternion.
        /// </summary>
        public static explicit operator decQuaternion(Quaternion v) => new decQuaternion((decimal)v.x, (decimal)v.y, (decimal)v.z, (decimal)v.w);
        
        /// <summary>
        /// Explicitly converts this to a cVector4.
        /// </summary>
        //public static explicit operator cVector4(Quaternion v) => new cVector4((Complex)v.x, (Complex)v.y, (Complex)v.z, (Complex)v.w);
        
        /// <summary>
        /// Explicitly converts this to a lVector4.
        /// </summary>
        public static explicit operator lVector4(Quaternion v) => new lVector4((long)v.x, (long)v.y, (long)v.z, (long)v.w);
        
        /// <summary>
        /// Explicitly converts this to a lQuaternion.
        /// </summary>
        public static explicit operator lQuaternion(Quaternion v) => new lQuaternion((long)v.x, (long)v.y, (long)v.z, (long)v.w);
        
        /// <summary>
        /// Explicitly converts this to a bVector4.
        /// </summary>
        public static explicit operator bVector4(Quaternion v) => new bVector4(v.x != 0f, v.y != 0f, v.z != 0f, v.w != 0f);
        
        /// <summary>
        /// Explicitly converts this to a bQuaternion.
        /// </summary>
        public static explicit operator bQuaternion(Quaternion v) => new bQuaternion(v.x != 0f, v.y != 0f, v.z != 0f, v.w != 0f);
        
        /// <summary>
        /// Creates a Quaternionernion from the rotational part of a Matrix3.
        /// </summary>
        public static explicit operator Quaternion(Matrix3 m) => FromMat3(m);
        
        /// <summary>
        /// Creates a Quaternionernion from the rotational part of a Matrix4.
        /// </summary>
        public static explicit operator Quaternion(Matrix4 m) => FromMat4(m);

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
        public float[] Values => new[] { x, y, z, w };
        
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
        public float LengthSqr => ((x*x + y*y) + (z*z + w*w));
        
        /// <summary>
        /// Returns a copy of this Quaternionernion with length one (undefined if this has zero length).
        /// </summary>
        public Quaternion Normalized => this / (float)Length;
        
        /// <summary>
        /// Returns a copy of this Quaternionernion with length one (returns zero if length is zero).
        /// </summary>
        public Quaternion NormalizedSafe => this == Zero ? Identity : this / (float)Length;
        
        /// <summary>
        /// Returns the represented angle of this Quaternionernion.
        /// </summary>
        public double Angle => Math.Acos((double)w) * 2.0;
        
        /// <summary>
        /// Returns the represented axis of this Quaternionernion.
        /// </summary>
        public Vector3 Axis
        {
            get
            {
                var s1 = 1 - w * w;
                if (s1 < 0) return Vector3.UnitZ;
                var s2 = 1 / Math.Sqrt(s1);
                return new Vector3((float)(x * s2), (float)(y * s2), (float)(z * s2));
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
        /// Creates a Matrix3 that realizes the rotation of this Quaternionernion
        /// </summary>
        public Matrix3 ToMat3 => new Matrix3(1 - 2 * (y*y + z*z), 2 * (x*y + w*z), 2 * (x*z - w*y), 2 * (x*y - w*z), 1 - 2 * (x*x + z*z), 2 * (y*z + w*x), 2 * (x*z + w*y), 2 * (y*z - w*x), 1 - 2 * (x*x + y*y));
        
        /// <summary>
        /// Creates a Matrix4 that realizes the rotation of this Quaternionernion
        /// </summary>
        public Matrix4 ToMat4 => new Matrix4(ToMat3);
        
        /// <summary>
        /// Returns the conjugated Quaternionernion
        /// </summary>
        public Quaternion Conjugate => new Quaternion(-x, -y, -z, w);
        
        /// <summary>
        /// Returns the inverse Quaternionernion
        /// </summary>
        public Quaternion Inverse => Conjugate / LengthSqr;

        #endregion


        #region Static Properties
        
        // added by proof

        public static Vector3 Eular(Quaternion quat)
        {
            return new Vector3((float)quat.EulerAngles.x, (float)quat.EulerAngles.y, (float)quat.EulerAngles.z);
        }

        public static Quaternion EularToQuat(Vector3 eular)
        {
            return new Quaternion(eular);
        }

        /// <summary> proof
        /// Unity-style overload: Create a quaternion from 3 float Euler.
        /// Just forwards to the Vector3 version.
        /// </summary>
        public static Quaternion EularToQuat(float x, float y, float z)
        {
            return new Quaternion(new Vector3(x,y,z));
        }

        /// <summary>
        /// Predefined all-zero Quaternionernion
        /// </summary>
        public static Quaternion Zero { get; } = new Quaternion(0f, 0f, 0f, 0f);
        
        /// <summary>
        /// Predefined all-ones Quaternionernion
        /// </summary>
        public static Quaternion Ones { get; } = new Quaternion(1f, 1f, 1f, 1f);
        
        /// <summary>
        /// Predefined identity Quaternionernion
        /// </summary>
        public static Quaternion Identity { get; } = new Quaternion(0f, 0f, 0f, 1f);
        
        /// <summary>
        /// Predefined unit-X Quaternionernion
        /// </summary>
        public static Quaternion UnitX { get; } = new Quaternion(1f, 0f, 0f, 0f);
        
        /// <summary>
        /// Predefined unit-Y Quaternionernion
        /// </summary>
        public static Quaternion UnitY { get; } = new Quaternion(0f, 1f, 0f, 0f);
        
        /// <summary>
        /// Predefined unit-Z Quaternionernion
        /// </summary>
        public static Quaternion UnitZ { get; } = new Quaternion(0f, 0f, 1f, 0f);
        
        /// <summary>
        /// Predefined unit-W Quaternionernion
        /// </summary>
        public static Quaternion UnitW { get; } = new Quaternion(0f, 0f, 0f, 1f);
        
        /// <summary>
        /// Predefined all-MaxValue Quaternionernion
        /// </summary>
        public static Quaternion MaxValue { get; } = new Quaternion(float.MaxValue, float.MaxValue, float.MaxValue, float.MaxValue);
        
        /// <summary>
        /// Predefined all-MinValue Quaternionernion
        /// </summary>
        public static Quaternion MinValue { get; } = new Quaternion(float.MinValue, float.MinValue, float.MinValue, float.MinValue);
        
        /// <summary>
        /// Predefined all-Epsilon Quaternionernion
        /// </summary>
        public static Quaternion Epsilon { get; } = new Quaternion(float.Epsilon, float.Epsilon, float.Epsilon, float.Epsilon);
        
        /// <summary>
        /// Predefined all-NaN Quaternionernion
        /// </summary>
        public static Quaternion NaN { get; } = new Quaternion(float.NaN, float.NaN, float.NaN, float.NaN);
        
        /// <summary>
        /// Predefined all-NegativeInfinity Quaternionernion
        /// </summary>
        public static Quaternion NegativeInfinity { get; } = new Quaternion(float.NegativeInfinity, float.NegativeInfinity, float.NegativeInfinity, float.NegativeInfinity);
        
        /// <summary>
        /// Predefined all-PositiveInfinity Quaternionernion
        /// </summary>
        public static Quaternion PositiveInfinity { get; } = new Quaternion(float.PositiveInfinity, float.PositiveInfinity, float.PositiveInfinity, float.PositiveInfinity);

        #endregion


        #region Operators
        
        /// <summary>
        /// Returns true iff this equals rhs component-wise.
        /// </summary>
        public static bool operator==(Quaternion lhs, Quaternion rhs) => lhs.Equals(rhs);
        
        /// <summary>
        /// Returns true iff this does not equal rhs (component-wise).
        /// </summary>
        public static bool operator!=(Quaternion lhs, Quaternion rhs) => !lhs.Equals(rhs);
        
        /// <summary>
        /// Returns proper multiplication of two Quaternionernions.
        /// </summary>
        public static Quaternion operator*(Quaternion p, Quaternion q) => new Quaternion(p.w * q.x + p.x * q.w + p.y * q.z - p.z * q.y, p.w * q.y + p.y * q.w + p.z * q.x - p.x * q.z, p.w * q.z + p.z * q.w + p.x * q.y - p.y * q.x, p.w * q.w - p.x * q.x - p.y * q.y - p.z * q.z);
        
        /// <summary>
        /// Returns a Vector rotated by the Quaternionernion.
        /// </summary>
        public static Vector3 operator*(Quaternion q, Vector3 v)
        {
            var qv = new Vector3(q.x, q.y, q.z);
            var uv = Vector3.Cross(qv, v);
            var uuv = Vector3.Cross(qv, uv);
            return v + ((uv * q.w) + uuv) * 2;
        }
        
        /// <summary>
        /// Returns a Vector rotated by the Quaternionernion (preserves v.w).
        /// </summary>
        public static Vector4 operator*(Quaternion q, Vector4 v) => new Vector4(q * new Vector3(v), v.w);
        
        /// <summary>
        /// Returns a Vector rotated by the inverted Quaternionernion.
        /// </summary>
        public static Vector3 operator*(Vector3 v, Quaternion q) => q.Inverse * v;
        
        /// <summary>
        /// Returns a Vector rotated by the inverted Quaternionernion (preserves v.w).
        /// </summary>
        public static Vector4 operator*(Vector4 v, Quaternion q) => q.Inverse * v;

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
        public bool Equals(Quaternion rhs) => ((x.Equals(rhs.x) && y.Equals(rhs.y)) && (z.Equals(rhs.z) && w.Equals(rhs.w)));
        
        /// <summary>
        /// Returns true iff this equals rhs type- and component-wise.
        /// </summary>
        public override bool Equals(object obj)
        {
            if (ReferenceEquals(null, obj)) return false;
            return obj is Quaternion && Equals((Quaternion) obj);
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
        public Quaternion Rotated(float angle, Vector3 v) => this * FromAxisAngle(angle, v);

        #endregion


        #region Static Functions
        
        /// <summary>
        /// Converts the string representation of the Quaternionernion into a Quaternionernion representation (using ', ' as a separator).
        /// </summary>
        public static Quaternion Parse(string s) => Parse(s, ", ");
        
        /// <summary>
        /// Converts the string representation of the Quaternionernion into a Quaternionernion representation (using a designated separator).
        /// </summary>
        public static Quaternion Parse(string s, string sep)
        {
            var kvp = s.Split(new[] { sep }, StringSplitOptions.None);
            if (kvp.Length != 4) throw new FormatException("input has not exactly 4 parts");
            return new Quaternion(float.Parse(kvp[0].Trim()), float.Parse(kvp[1].Trim()), float.Parse(kvp[2].Trim()), float.Parse(kvp[3].Trim()));
        }
        
        /// <summary>
        /// Converts the string representation of the Quaternionernion into a Quaternionernion representation (using a designated separator and a type provider).
        /// </summary>
        public static Quaternion Parse(string s, string sep, IFormatProvider provider)
        {
            var kvp = s.Split(new[] { sep }, StringSplitOptions.None);
            if (kvp.Length != 4) throw new FormatException("input has not exactly 4 parts");
            return new Quaternion(float.Parse(kvp[0].Trim(), provider), float.Parse(kvp[1].Trim(), provider), float.Parse(kvp[2].Trim(), provider), float.Parse(kvp[3].Trim(), provider));
        }
        
        /// <summary>
        /// Converts the string representation of the Quaternionernion into a Quaternionernion representation (using a designated separator and a number style).
        /// </summary>
        public static Quaternion Parse(string s, string sep, NumberStyles style)
        {
            var kvp = s.Split(new[] { sep }, StringSplitOptions.None);
            if (kvp.Length != 4) throw new FormatException("input has not exactly 4 parts");
            return new Quaternion(float.Parse(kvp[0].Trim(), style), float.Parse(kvp[1].Trim(), style), float.Parse(kvp[2].Trim(), style), float.Parse(kvp[3].Trim(), style));
        }
        
        /// <summary>
        /// Converts the string representation of the Quaternionernion into a Quaternionernion representation (using a designated separator and a number style and a forMatrix provider).
        /// </summary>
        public static Quaternion Parse(string s, string sep, NumberStyles style, IFormatProvider provider)
        {
            var kvp = s.Split(new[] { sep }, StringSplitOptions.None);
            if (kvp.Length != 4) throw new FormatException("input has not exactly 4 parts");
            return new Quaternion(float.Parse(kvp[0].Trim(), style, provider), float.Parse(kvp[1].Trim(), style, provider), float.Parse(kvp[2].Trim(), style, provider), float.Parse(kvp[3].Trim(), style, provider));
        }
        
        /// <summary>
        /// Tries to convert the string representation of the Quaternionernion into a Quaternionernion representation (using ', ' as a separator), returns false if string was invalid.
        /// </summary>
        public static bool TryParse(string s, out Quaternion result) => TryParse(s, ", ", out result);
        
        /// <summary>
        /// Tries to convert the string representation of the Quaternionernion into a Quaternionernion representation (using a designated separator), returns false if string was invalid.
        /// </summary>
        public static bool TryParse(string s, string sep, out Quaternion result)
        {
            result = Zero;
            if (string.IsNullOrEmpty(s)) return false;
            var kvp = s.Split(new[] { sep }, StringSplitOptions.None);
            if (kvp.Length != 4) return false;
            float x = 0f, y = 0f, z = 0f, w = 0f;
            var ok = ((float.TryParse(kvp[0].Trim(), out x) && float.TryParse(kvp[1].Trim(), out y)) && (float.TryParse(kvp[2].Trim(), out z) && float.TryParse(kvp[3].Trim(), out w)));
            result = ok ? new Quaternion(x, y, z, w) : Zero;
            return ok;
        }
        
        /// <summary>
        /// Tries to convert the string representation of the Quaternionernion into a Quaternionernion representation (using a designated separator and a number style and a forMatrix provider), returns false if string was invalid.
        /// </summary>
        public static bool TryParse(string s, string sep, NumberStyles style, IFormatProvider provider, out Quaternion result)
        {
            result = Zero;
            if (string.IsNullOrEmpty(s)) return false;
            var kvp = s.Split(new[] { sep }, StringSplitOptions.None);
            if (kvp.Length != 4) return false;
            float x = 0f, y = 0f, z = 0f, w = 0f;
            var ok = ((float.TryParse(kvp[0].Trim(), style, provider, out x) && float.TryParse(kvp[1].Trim(), style, provider, out y)) && (float.TryParse(kvp[2].Trim(), style, provider, out z) && float.TryParse(kvp[3].Trim(), style, provider, out w)));
            result = ok ? new Quaternion(x, y, z, w) : Zero;
            return ok;
        }
        
        /// <summary>
        /// Returns the inner product (dot product, scalar product) of the two Quaternionernions.
        /// </summary>
        public static float Dot(Quaternion lhs, Quaternion rhs) => ((lhs.x * rhs.x + lhs.y * rhs.y) + (lhs.z * rhs.z + lhs.w * rhs.w));
        
        /// <summary>
        /// Creates a Quaternionernion from an axis and an angle (in radians).
        /// </summary>
        public static Quaternion FromAxisAngle(float angle, Vector3 v)
        {
            var s = Math.Sin((double)angle * 0.5);
            var c = Math.Cos((double)angle * 0.5);
            return new Quaternion((float)((double)v.x * s), (float)((double)v.y * s), (float)((double)v.z * s), (float)c);
        }
        
        /// <summary>
        /// Creates a Quaternionernion from the rotational part of a Matrix4.
        /// </summary>
        public static Quaternion FromMat3(Matrix3 m)
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
                case 0: return new Quaternion((float)((double)(m.m12 - m.m21) * mult), (float)((double)(m.m20 - m.m02) * mult), (float)((double)(m.m01 - m.m10) * mult), (float)(biggestVal));
                case 1: return new Quaternion((float)(biggestVal), (float)((double)(m.m01 + m.m10) * mult), (float)((double)(m.m20 + m.m02) * mult), (float)((double)(m.m12 - m.m21) * mult));
                case 2: return new Quaternion((float)((double)(m.m01 + m.m10) * mult), (float)(biggestVal), (float)((double)(m.m12 + m.m21) * mult), (float)((double)(m.m20 - m.m02) * mult));
                default: return new Quaternion((float)((double)(m.m20 + m.m02) * mult), (float)((double)(m.m12 + m.m21) * mult), (float)(biggestVal), (float)((double)(m.m01 - m.m10) * mult));
            }
        }
        
        /// <summary>
        /// Creates a Quaternionernion from the rotational part of a Matrix3.
        /// </summary>
        public static Quaternion FromMat4(Matrix4 m) => FromMat3(new Matrix3(m));
        
        /// <summary>
        /// Returns the cross product between two Quaternionernions.
        /// </summary>
        public static Quaternion Cross(Quaternion q1, Quaternion q2) => new Quaternion(q1.w * q2.x + q1.x * q2.w + q1.y * q2.z - q1.z * q2.y, q1.w * q2.y + q1.y * q2.w + q1.z * q2.x - q1.x * q2.z, q1.w * q2.z + q1.z * q2.w + q1.x * q2.y - q1.y * q2.x, q1.w * q2.w - q1.x * q2.x - q1.y * q2.y - q1.z * q2.z);
        
        /// <summary>
        /// Calculates a proper spherical interpolation between two Quaternionernions (only works for normalized Quaternionernions).
        /// </summary>
        public static Quaternion Mix(Quaternion x, Quaternion y, float a)
        {
            var cosTheta = (double)Dot(x, y);
            if (cosTheta > 1 - float.Epsilon)
                return Lerp(x, y, a);
            else
            {
                var angle = Math.Acos((double)cosTheta);
                return (Quaternion)( (Math.Sin((1 - (double)a) * angle) * (dQuaternion)x + Math.Sin((double)a * angle) * (dQuaternion)y) / Math.Sin(angle) );
            }
        }
        
        /// <summary>
        /// Calculates a proper spherical interpolation between two Quaternionernions (only works for normalized Quaternionernions).
        /// </summary>
        public static Quaternion SLerp(Quaternion x, Quaternion y, float a)
        {
            var z = y;
            var cosTheta = (double)Dot(x, y);
            if (cosTheta < 0) { z = -y; cosTheta = -cosTheta; }
            if (cosTheta > 1 - float.Epsilon)
                return Lerp(x, z, a);
            else
            {
                var angle = Math.Acos((double)cosTheta);
                return (Quaternion)( (Math.Sin((1 - (double)a) * angle) * (dQuaternion)x + Math.Sin((double)a * angle) * (dQuaternion)z) / Math.Sin(angle) );
            }
        }
        
        /// <summary>
        /// Applies squad interpolation of these Quaternionernions
        /// </summary>
        public static Quaternion Squad(Quaternion q1, Quaternion q2, Quaternion s1, Quaternion s2, float h) => Mix(Mix(q1, q2, h), Mix(s1, s2, h), 2 * (1 - h) * h);

        #endregion


        #region Component-Wise Static Functions
        
        /// <summary>
        /// Returns a bVector4 from component-wise application of IsInfinity (float.IsInfinity(v)).
        /// </summary>
        public static bVector4 IsInfinity(Quaternion v) => new bVector4(float.IsInfinity(v.x), float.IsInfinity(v.y), float.IsInfinity(v.z), float.IsInfinity(v.w));
        
        /// <summary>
        /// Returns a bVector from the application of IsInfinity (float.IsInfinity(v)).
        /// </summary>
        public static bVector4 IsInfinity(float v) => new bVector4(float.IsInfinity(v));
        
        /// <summary>
        /// Returns a bVector4 from component-wise application of IsFinite (!float.IsNaN(v) &amp;&amp; !float.IsInfinity(v)).
        /// </summary>
        public static bVector4 IsFinite(Quaternion v) => new bVector4(!float.IsNaN(v.x) && !float.IsInfinity(v.x), !float.IsNaN(v.y) && !float.IsInfinity(v.y), !float.IsNaN(v.z) && !float.IsInfinity(v.z), !float.IsNaN(v.w) && !float.IsInfinity(v.w));
        
        /// <summary>
        /// Returns a bVector from the application of IsFinite (!float.IsNaN(v) &amp;&amp; !float.IsInfinity(v)).
        /// </summary>
        public static bVector4 IsFinite(float v) => new bVector4(!float.IsNaN(v) && !float.IsInfinity(v));
        
        /// <summary>
        /// Returns a bVector4 from component-wise application of IsNaN (float.IsNaN(v)).
        /// </summary>
        public static bVector4 IsNaN(Quaternion v) => new bVector4(float.IsNaN(v.x), float.IsNaN(v.y), float.IsNaN(v.z), float.IsNaN(v.w));
        
        /// <summary>
        /// Returns a bVector from the application of IsNaN (float.IsNaN(v)).
        /// </summary>
        public static bVector4 IsNaN(float v) => new bVector4(float.IsNaN(v));
        
        /// <summary>
        /// Returns a bVector4 from component-wise application of IsNegativeInfinity (float.IsNegativeInfinity(v)).
        /// </summary>
        public static bVector4 IsNegativeInfinity(Quaternion v) => new bVector4(float.IsNegativeInfinity(v.x), float.IsNegativeInfinity(v.y), float.IsNegativeInfinity(v.z), float.IsNegativeInfinity(v.w));
        
        /// <summary>
        /// Returns a bVector from the application of IsNegativeInfinity (float.IsNegativeInfinity(v)).
        /// </summary>
        public static bVector4 IsNegativeInfinity(float v) => new bVector4(float.IsNegativeInfinity(v));
        
        /// <summary>
        /// Returns a bVector4 from component-wise application of IsPositiveInfinity (float.IsPositiveInfinity(v)).
        /// </summary>
        public static bVector4 IsPositiveInfinity(Quaternion v) => new bVector4(float.IsPositiveInfinity(v.x), float.IsPositiveInfinity(v.y), float.IsPositiveInfinity(v.z), float.IsPositiveInfinity(v.w));
        
        /// <summary>
        /// Returns a bVector from the application of IsPositiveInfinity (float.IsPositiveInfinity(v)).
        /// </summary>
        public static bVector4 IsPositiveInfinity(float v) => new bVector4(float.IsPositiveInfinity(v));
        
        /// <summary>
        /// Returns a bVector4 from component-wise application of Equal (lhs == rhs).
        /// </summary>
        public static bVector4 Equal(Quaternion lhs, Quaternion rhs) => new bVector4(lhs.x == rhs.x, lhs.y == rhs.y, lhs.z == rhs.z, lhs.w == rhs.w);
        
        /// <summary>
        /// Returns a bVector4 from component-wise application of Equal (lhs == rhs).
        /// </summary>
        public static bVector4 Equal(Quaternion lhs, float rhs) => new bVector4(lhs.x == rhs, lhs.y == rhs, lhs.z == rhs, lhs.w == rhs);
        
        /// <summary>
        /// Returns a bVector4 from component-wise application of Equal (lhs == rhs).
        /// </summary>
        public static bVector4 Equal(float lhs, Quaternion rhs) => new bVector4(lhs == rhs.x, lhs == rhs.y, lhs == rhs.z, lhs == rhs.w);
        
        /// <summary>
        /// Returns a bVector from the application of Equal (lhs == rhs).
        /// </summary>
        public static bVector4 Equal(float lhs, float rhs) => new bVector4(lhs == rhs);
        
        /// <summary>
        /// Returns a bVector4 from component-wise application of NotEqual (lhs != rhs).
        /// </summary>
        public static bVector4 NotEqual(Quaternion lhs, Quaternion rhs) => new bVector4(lhs.x != rhs.x, lhs.y != rhs.y, lhs.z != rhs.z, lhs.w != rhs.w);
        
        /// <summary>
        /// Returns a bVector4 from component-wise application of NotEqual (lhs != rhs).
        /// </summary>
        public static bVector4 NotEqual(Quaternion lhs, float rhs) => new bVector4(lhs.x != rhs, lhs.y != rhs, lhs.z != rhs, lhs.w != rhs);
        
        /// <summary>
        /// Returns a bVector4 from component-wise application of NotEqual (lhs != rhs).
        /// </summary>
        public static bVector4 NotEqual(float lhs, Quaternion rhs) => new bVector4(lhs != rhs.x, lhs != rhs.y, lhs != rhs.z, lhs != rhs.w);
        
        /// <summary>
        /// Returns a bVector from the application of NotEqual (lhs != rhs).
        /// </summary>
        public static bVector4 NotEqual(float lhs, float rhs) => new bVector4(lhs != rhs);
        
        /// <summary>
        /// Returns a bVector4 from component-wise application of GreaterThan (lhs &gt; rhs).
        /// </summary>
        public static bVector4 GreaterThan(Quaternion lhs, Quaternion rhs) => new bVector4(lhs.x > rhs.x, lhs.y > rhs.y, lhs.z > rhs.z, lhs.w > rhs.w);
        
        /// <summary>
        /// Returns a bVector4 from component-wise application of GreaterThan (lhs &gt; rhs).
        /// </summary>
        public static bVector4 GreaterThan(Quaternion lhs, float rhs) => new bVector4(lhs.x > rhs, lhs.y > rhs, lhs.z > rhs, lhs.w > rhs);
        
        /// <summary>
        /// Returns a bVector4 from component-wise application of GreaterThan (lhs &gt; rhs).
        /// </summary>
        public static bVector4 GreaterThan(float lhs, Quaternion rhs) => new bVector4(lhs > rhs.x, lhs > rhs.y, lhs > rhs.z, lhs > rhs.w);
        
        /// <summary>
        /// Returns a bVector from the application of GreaterThan (lhs &gt; rhs).
        /// </summary>
        public static bVector4 GreaterThan(float lhs, float rhs) => new bVector4(lhs > rhs);
        
        /// <summary>
        /// Returns a bVector4 from component-wise application of GreaterThanEqual (lhs &gt;= rhs).
        /// </summary>
        public static bVector4 GreaterThanEqual(Quaternion lhs, Quaternion rhs) => new bVector4(lhs.x >= rhs.x, lhs.y >= rhs.y, lhs.z >= rhs.z, lhs.w >= rhs.w);
        
        /// <summary>
        /// Returns a bVector4 from component-wise application of GreaterThanEqual (lhs &gt;= rhs).
        /// </summary>
        public static bVector4 GreaterThanEqual(Quaternion lhs, float rhs) => new bVector4(lhs.x >= rhs, lhs.y >= rhs, lhs.z >= rhs, lhs.w >= rhs);
        
        /// <summary>
        /// Returns a bVector4 from component-wise application of GreaterThanEqual (lhs &gt;= rhs).
        /// </summary>
        public static bVector4 GreaterThanEqual(float lhs, Quaternion rhs) => new bVector4(lhs >= rhs.x, lhs >= rhs.y, lhs >= rhs.z, lhs >= rhs.w);
        
        /// <summary>
        /// Returns a bVector from the application of GreaterThanEqual (lhs &gt;= rhs).
        /// </summary>
        public static bVector4 GreaterThanEqual(float lhs, float rhs) => new bVector4(lhs >= rhs);
        
        /// <summary>
        /// Returns a bVector4 from component-wise application of LesserThan (lhs &lt; rhs).
        /// </summary>
        public static bVector4 LesserThan(Quaternion lhs, Quaternion rhs) => new bVector4(lhs.x < rhs.x, lhs.y < rhs.y, lhs.z < rhs.z, lhs.w < rhs.w);
        
        /// <summary>
        /// Returns a bVector4 from component-wise application of LesserThan (lhs &lt; rhs).
        /// </summary>
        public static bVector4 LesserThan(Quaternion lhs, float rhs) => new bVector4(lhs.x < rhs, lhs.y < rhs, lhs.z < rhs, lhs.w < rhs);
        
        /// <summary>
        /// Returns a bVector4 from component-wise application of LesserThan (lhs &lt; rhs).
        /// </summary>
        public static bVector4 LesserThan(float lhs, Quaternion rhs) => new bVector4(lhs < rhs.x, lhs < rhs.y, lhs < rhs.z, lhs < rhs.w);
        
        /// <summary>
        /// Returns a bVector from the application of LesserThan (lhs &lt; rhs).
        /// </summary>
        public static bVector4 LesserThan(float lhs, float rhs) => new bVector4(lhs < rhs);
        
        /// <summary>
        /// Returns a bVector4 from component-wise application of LesserThanEqual (lhs &lt;= rhs).
        /// </summary>
        public static bVector4 LesserThanEqual(Quaternion lhs, Quaternion rhs) => new bVector4(lhs.x <= rhs.x, lhs.y <= rhs.y, lhs.z <= rhs.z, lhs.w <= rhs.w);
        
        /// <summary>
        /// Returns a bVector4 from component-wise application of LesserThanEqual (lhs &lt;= rhs).
        /// </summary>
        public static bVector4 LesserThanEqual(Quaternion lhs, float rhs) => new bVector4(lhs.x <= rhs, lhs.y <= rhs, lhs.z <= rhs, lhs.w <= rhs);
        
        /// <summary>
        /// Returns a bVector4 from component-wise application of LesserThanEqual (lhs &lt;= rhs).
        /// </summary>
        public static bVector4 LesserThanEqual(float lhs, Quaternion rhs) => new bVector4(lhs <= rhs.x, lhs <= rhs.y, lhs <= rhs.z, lhs <= rhs.w);
        
        /// <summary>
        /// Returns a bVector from the application of LesserThanEqual (lhs &lt;= rhs).
        /// </summary>
        public static bVector4 LesserThanEqual(float lhs, float rhs) => new bVector4(lhs <= rhs);
        
        /// <summary>
        /// Returns a Quaternion from component-wise application of Lerp (min * (1-a) + max * a).
        /// </summary>
        public static Quaternion Lerp(Quaternion min, Quaternion max, Quaternion a) => new Quaternion(min.x * (1-a.x) + max.x * a.x, min.y * (1-a.y) + max.y * a.y, min.z * (1-a.z) + max.z * a.z, min.w * (1-a.w) + max.w * a.w);
        
        /// <summary>
        /// Returns a Quaternion from component-wise application of Lerp (min * (1-a) + max * a).
        /// </summary>
        public static Quaternion Lerp(Quaternion min, Quaternion max, float a) => new Quaternion(min.x * (1-a) + max.x * a, min.y * (1-a) + max.y * a, min.z * (1-a) + max.z * a, min.w * (1-a) + max.w * a);
        
        /// <summary>
        /// Returns a Quaternion from component-wise application of Lerp (min * (1-a) + max * a).
        /// </summary>
        public static Quaternion Lerp(Quaternion min, float max, Quaternion a) => new Quaternion(min.x * (1-a.x) + max * a.x, min.y * (1-a.y) + max * a.y, min.z * (1-a.z) + max * a.z, min.w * (1-a.w) + max * a.w);
        
        /// <summary>
        /// Returns a Quaternion from component-wise application of Lerp (min * (1-a) + max * a).
        /// </summary>
        public static Quaternion Lerp(Quaternion min, float max, float a) => new Quaternion(min.x * (1-a) + max * a, min.y * (1-a) + max * a, min.z * (1-a) + max * a, min.w * (1-a) + max * a);
        
        /// <summary>
        /// Returns a Quaternion from component-wise application of Lerp (min * (1-a) + max * a).
        /// </summary>
        public static Quaternion Lerp(float min, Quaternion max, Quaternion a) => new Quaternion(min * (1-a.x) + max.x * a.x, min * (1-a.y) + max.y * a.y, min * (1-a.z) + max.z * a.z, min * (1-a.w) + max.w * a.w);
        
        /// <summary>
        /// Returns a Quaternion from component-wise application of Lerp (min * (1-a) + max * a).
        /// </summary>
        public static Quaternion Lerp(float min, Quaternion max, float a) => new Quaternion(min * (1-a) + max.x * a, min * (1-a) + max.y * a, min * (1-a) + max.z * a, min * (1-a) + max.w * a);
        
        /// <summary>
        /// Returns a Quaternion from component-wise application of Lerp (min * (1-a) + max * a).
        /// </summary>
        public static Quaternion Lerp(float min, float max, Quaternion a) => new Quaternion(min * (1-a.x) + max * a.x, min * (1-a.y) + max * a.y, min * (1-a.z) + max * a.z, min * (1-a.w) + max * a.w);
        
        /// <summary>
        /// Returns a Quaternion from the application of Lerp (min * (1-a) + max * a).
        /// </summary>
        public static Quaternion Lerp(float min, float max, float a) => new Quaternion(min * (1-a) + max * a);

        #endregion


        #region Component-Wise Operator Overloads
        
        /// <summary>
        /// Returns a bVector4 from component-wise application of operator&lt; (lhs &lt; rhs).
        /// </summary>
        public static bVector4 operator<(Quaternion lhs, Quaternion rhs) => new bVector4(lhs.x < rhs.x, lhs.y < rhs.y, lhs.z < rhs.z, lhs.w < rhs.w);
        
        /// <summary>
        /// Returns a bVector4 from component-wise application of operator&lt; (lhs &lt; rhs).
        /// </summary>
        public static bVector4 operator<(Quaternion lhs, float rhs) => new bVector4(lhs.x < rhs, lhs.y < rhs, lhs.z < rhs, lhs.w < rhs);
        
        /// <summary>
        /// Returns a bVector4 from component-wise application of operator&lt; (lhs &lt; rhs).
        /// </summary>
        public static bVector4 operator<(float lhs, Quaternion rhs) => new bVector4(lhs < rhs.x, lhs < rhs.y, lhs < rhs.z, lhs < rhs.w);
        
        /// <summary>
        /// Returns a bVector4 from component-wise application of operator&lt;= (lhs &lt;= rhs).
        /// </summary>
        public static bVector4 operator<=(Quaternion lhs, Quaternion rhs) => new bVector4(lhs.x <= rhs.x, lhs.y <= rhs.y, lhs.z <= rhs.z, lhs.w <= rhs.w);
        
        /// <summary>
        /// Returns a bVector4 from component-wise application of operator&lt;= (lhs &lt;= rhs).
        /// </summary>
        public static bVector4 operator<=(Quaternion lhs, float rhs) => new bVector4(lhs.x <= rhs, lhs.y <= rhs, lhs.z <= rhs, lhs.w <= rhs);
        
        /// <summary>
        /// Returns a bVector4 from component-wise application of operator&lt;= (lhs &lt;= rhs).
        /// </summary>
        public static bVector4 operator<=(float lhs, Quaternion rhs) => new bVector4(lhs <= rhs.x, lhs <= rhs.y, lhs <= rhs.z, lhs <= rhs.w);
        
        /// <summary>
        /// Returns a bVector4 from component-wise application of operator&gt; (lhs &gt; rhs).
        /// </summary>
        public static bVector4 operator>(Quaternion lhs, Quaternion rhs) => new bVector4(lhs.x > rhs.x, lhs.y > rhs.y, lhs.z > rhs.z, lhs.w > rhs.w);
        
        /// <summary>
        /// Returns a bVector4 from component-wise application of operator&gt; (lhs &gt; rhs).
        /// </summary>
        public static bVector4 operator>(Quaternion lhs, float rhs) => new bVector4(lhs.x > rhs, lhs.y > rhs, lhs.z > rhs, lhs.w > rhs);
        
        /// <summary>
        /// Returns a bVector4 from component-wise application of operator&gt; (lhs &gt; rhs).
        /// </summary>
        public static bVector4 operator>(float lhs, Quaternion rhs) => new bVector4(lhs > rhs.x, lhs > rhs.y, lhs > rhs.z, lhs > rhs.w);
        
        /// <summary>
        /// Returns a bVector4 from component-wise application of operator&gt;= (lhs &gt;= rhs).
        /// </summary>
        public static bVector4 operator>=(Quaternion lhs, Quaternion rhs) => new bVector4(lhs.x >= rhs.x, lhs.y >= rhs.y, lhs.z >= rhs.z, lhs.w >= rhs.w);
        
        /// <summary>
        /// Returns a bVector4 from component-wise application of operator&gt;= (lhs &gt;= rhs).
        /// </summary>
        public static bVector4 operator>=(Quaternion lhs, float rhs) => new bVector4(lhs.x >= rhs, lhs.y >= rhs, lhs.z >= rhs, lhs.w >= rhs);
        
        /// <summary>
        /// Returns a bVector4 from component-wise application of operator&gt;= (lhs &gt;= rhs).
        /// </summary>
        public static bVector4 operator>=(float lhs, Quaternion rhs) => new bVector4(lhs >= rhs.x, lhs >= rhs.y, lhs >= rhs.z, lhs >= rhs.w);
        
        /// <summary>
        /// Returns a Quaternion from component-wise application of operator+ (identity).
        /// </summary>
        public static Quaternion operator+(Quaternion v) => v;
        
        /// <summary>
        /// Returns a Quaternion from component-wise application of operator- (-v).
        /// </summary>
        public static Quaternion operator-(Quaternion v) => new Quaternion(-v.x, -v.y, -v.z, -v.w);
        
        /// <summary>
        /// Returns a Quaternion from component-wise application of operator+ (lhs + rhs).
        /// </summary>
        public static Quaternion operator+(Quaternion lhs, Quaternion rhs) => new Quaternion(lhs.x + rhs.x, lhs.y + rhs.y, lhs.z + rhs.z, lhs.w + rhs.w);
        
        /// <summary>
        /// Returns a Quaternion from component-wise application of operator+ (lhs + rhs).
        /// </summary>
        public static Quaternion operator+(Quaternion lhs, float rhs) => new Quaternion(lhs.x + rhs, lhs.y + rhs, lhs.z + rhs, lhs.w + rhs);
        
        /// <summary>
        /// Returns a Quaternion from component-wise application of operator+ (lhs + rhs).
        /// </summary>
        public static Quaternion operator+(float lhs, Quaternion rhs) => new Quaternion(lhs + rhs.x, lhs + rhs.y, lhs + rhs.z, lhs + rhs.w);
        
        /// <summary>
        /// Returns a Quaternion from component-wise application of operator- (lhs - rhs).
        /// </summary>
        public static Quaternion operator-(Quaternion lhs, Quaternion rhs) => new Quaternion(lhs.x - rhs.x, lhs.y - rhs.y, lhs.z - rhs.z, lhs.w - rhs.w);
        
        /// <summary>
        /// Returns a Quaternion from component-wise application of operator- (lhs - rhs).
        /// </summary>
        public static Quaternion operator-(Quaternion lhs, float rhs) => new Quaternion(lhs.x - rhs, lhs.y - rhs, lhs.z - rhs, lhs.w - rhs);
        
        /// <summary>
        /// Returns a Quaternion from component-wise application of operator- (lhs - rhs).
        /// </summary>
        public static Quaternion operator-(float lhs, Quaternion rhs) => new Quaternion(lhs - rhs.x, lhs - rhs.y, lhs - rhs.z, lhs - rhs.w);
        
        /// <summary>
        /// Returns a Quaternion from component-wise application of operator* (lhs * rhs).
        /// </summary>
        public static Quaternion operator*(Quaternion lhs, float rhs) => new Quaternion(lhs.x * rhs, lhs.y * rhs, lhs.z * rhs, lhs.w * rhs);
        
        /// <summary>
        /// Returns a Quaternion from component-wise application of operator* (lhs * rhs).
        /// </summary>
        public static Quaternion operator*(float lhs, Quaternion rhs) => new Quaternion(lhs * rhs.x, lhs * rhs.y, lhs * rhs.z, lhs * rhs.w);
        
        /// <summary>
        /// Returns a Quaternion from component-wise application of operator/ (lhs / rhs).
        /// </summary>
        public static Quaternion operator/(Quaternion lhs, float rhs) => new Quaternion(lhs.x / rhs, lhs.y / rhs, lhs.z / rhs, lhs.w / rhs);

        #endregion

    }
}
