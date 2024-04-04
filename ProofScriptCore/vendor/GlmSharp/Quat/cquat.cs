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
    /// A Quaternionernion of type Complex.
    /// </summary>
    /// 
    /*
    [RegisterCoreClassStruct]
    
    [StructLayout(LayoutKind.Sequential)]
    public struct cQuaternion : IReadOnlyList<Complex>, IEquatable<cQuaternion>
    {

        #region Fields
        
        /// <summary>
        /// x-component
        /// </summary>
        
        public Complex x;
        
        /// <summary>
        /// y-component
        /// </summary>
        
        public Complex y;
        
        /// <summary>
        /// z-component
        /// </summary>
        
        public Complex z;
        
        /// <summary>
        /// w-component
        /// </summary>
        
        public Complex w;

        #endregion


        #region Constructors
        
        /// <summary>
        /// Component-wise constructor
        /// </summary>
        public cQuaternion(Complex x, Complex y, Complex z, Complex w)
        {
            this.x = x;
            this.y = y;
            this.z = z;
            this.w = w;
        }
        
        /// <summary>
        /// all-same-value constructor
        /// </summary>
        public cQuaternion(Complex v)
        {
            this.x = v;
            this.y = v;
            this.z = v;
            this.w = v;
        }
        
        /// <summary>
        /// copy constructor
        /// </summary>
        public cQuaternion(cQuaternion q)
        {
            this.x = q.x;
            this.y = q.y;
            this.z = q.z;
            this.w = q.w;
        }
        
        /// <summary>
        /// Vectortor-and-scalar constructor (CAUTION: not angle-axis, use FromAngleAxis instead)
        /// </summary>
        public cQuaternion(cVector3 v, Complex s)
        {
            this.x = v.x;
            this.y = v.y;
            this.z = v.z;
            this.w = s;
        }

        #endregion


        #region Explicit Operators
        
        /// <summary>
        /// Explicitly converts this to a cVector4.
        /// </summary>
        public static explicit operator cVector4(cQuaternion v) => new cVector4((Complex)v.x, (Complex)v.y, (Complex)v.z, (Complex)v.w);

        #endregion


        #region Indexer
        
        /// <summary>
        /// Gets/Sets a specific indexed component (a bit slower than direct access).
        /// </summary>
        public Complex this[int index]
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
        public Complex[] Values => new[] { x, y, z, w };
        
        /// <summary>
        /// Returns the number of components (4).
        /// </summary>
        public int Count => 4;
        
        /// <summary>
        /// Returns the euclidean length of this Quaternionernion.
        /// </summary>
        public double Length => (double)Math.Sqrt(((x.LengthSqr() + y.LengthSqr()) + (z.LengthSqr() + w.LengthSqr())));
        
        /// <summary>
        /// Returns the squared euclidean length of this Quaternionernion.
        /// </summary>
        public Complex LengthSqr => ((x.LengthSqr() + y.LengthSqr()) + (z.LengthSqr() + w.LengthSqr()));
        
        /// <summary>
        /// Returns a copy of this Quaternionernion with length one (undefined if this has zero length).
        /// </summary>
        public cQuaternion Normalized => this / (Complex)Length;
        
        /// <summary>
        /// Returns a copy of this Quaternionernion with length one (returns zero if length is zero).
        /// </summary>
        public cQuaternion NormalizedSafe => this == Zero ? Identity : this / (Complex)Length;
        
        /// <summary>
        /// Returns the conjugated Quaternionernion
        /// </summary>
        public cQuaternion Conjugate => new cQuaternion(-x, -y, -z, w);
        
        /// <summary>
        /// Returns the inverse Quaternionernion
        /// </summary>
        public cQuaternion Inverse => Conjugate / LengthSqr;

        #endregion


        #region Static Properties
        
        /// <summary>
        /// Predefined all-zero Quaternionernion
        /// </summary>
        public static cQuaternion Zero { get; } = new cQuaternion(Complex.Zero, Complex.Zero, Complex.Zero, Complex.Zero);
        
        /// <summary>
        /// Predefined all-ones Quaternionernion
        /// </summary>
        public static cQuaternion Ones { get; } = new cQuaternion(Complex.One, Complex.One, Complex.One, Complex.One);
        
        /// <summary>
        /// Predefined identity Quaternionernion
        /// </summary>
        public static cQuaternion Identity { get; } = new cQuaternion(Complex.Zero, Complex.Zero, Complex.Zero, Complex.One);
        
        /// <summary>
        /// Predefined unit-X Quaternionernion
        /// </summary>
        public static cQuaternion UnitX { get; } = new cQuaternion(Complex.One, Complex.Zero, Complex.Zero, Complex.Zero);
        
        /// <summary>
        /// Predefined unit-Y Quaternionernion
        /// </summary>
        public static cQuaternion UnitY { get; } = new cQuaternion(Complex.Zero, Complex.One, Complex.Zero, Complex.Zero);
        
        /// <summary>
        /// Predefined unit-Z Quaternionernion
        /// </summary>
        public static cQuaternion UnitZ { get; } = new cQuaternion(Complex.Zero, Complex.Zero, Complex.One, Complex.Zero);
        
        /// <summary>
        /// Predefined unit-W Quaternionernion
        /// </summary>
        public static cQuaternion UnitW { get; } = new cQuaternion(Complex.Zero, Complex.Zero, Complex.Zero, Complex.One);
        
        /// <summary>
        /// Predefined all-imaginary-ones Quaternionernion
        /// </summary>
        public static cQuaternion ImaginaryOnes { get; } = new cQuaternion(Complex.ImaginaryOne, Complex.ImaginaryOne, Complex.ImaginaryOne, Complex.ImaginaryOne);
        
        /// <summary>
        /// Predefined unit-imaginary-X Quaternionernion
        /// </summary>
        public static cQuaternion ImaginaryUnitX { get; } = new cQuaternion(Complex.ImaginaryOne, Complex.Zero, Complex.Zero, Complex.Zero);
        
        /// <summary>
        /// Predefined unit-imaginary-Y Quaternionernion
        /// </summary>
        public static cQuaternion ImaginaryUnitY { get; } = new cQuaternion(Complex.Zero, Complex.ImaginaryOne, Complex.Zero, Complex.Zero);
        
        /// <summary>
        /// Predefined unit-imaginary-Z Quaternionernion
        /// </summary>
        public static cQuaternion ImaginaryUnitZ { get; } = new cQuaternion(Complex.Zero, Complex.Zero, Complex.ImaginaryOne, Complex.Zero);
        
        /// <summary>
        /// Predefined unit-imaginary-W Quaternionernion
        /// </summary>
        public static cQuaternion ImaginaryUnitW { get; } = new cQuaternion(Complex.Zero, Complex.Zero, Complex.Zero, Complex.ImaginaryOne);

        #endregion


        #region Operators
        
        /// <summary>
        /// Returns true iff this equals rhs component-wise.
        /// </summary>
        public static bool operator==(cQuaternion lhs, cQuaternion rhs) => lhs.Equals(rhs);
        
        /// <summary>
        /// Returns true iff this does not equal rhs (component-wise).
        /// </summary>
        public static bool operator!=(cQuaternion lhs, cQuaternion rhs) => !lhs.Equals(rhs);
        
        /// <summary>
        /// Returns proper multiplication of two Quaternionernions.
        /// </summary>
        public static cQuaternion operator*(cQuaternion p, cQuaternion q) => new cQuaternion(p.w * q.x + p.x * q.w + p.y * q.z - p.z * q.y, p.w * q.y + p.y * q.w + p.z * q.x - p.x * q.z, p.w * q.z + p.z * q.w + p.x * q.y - p.y * q.x, p.w * q.w - p.x * q.x - p.y * q.y - p.z * q.z);
        
        /// <summary>
        /// Returns a Vectortor rotated by the Quaternionernion.
        /// </summary>
        public static cVector3 operator*(cQuaternion q, cVector3 v)
        {
            var qv = new cVector3(q.x, q.y, q.z);
            var uv = cVector3.Cross(qv, v);
            var uuv = cVector3.Cross(qv, uv);
            return v + ((uv * q.w) + uuv) * 2;
        }
        
        /// <summary>
        /// Returns a Vectortor rotated by the Quaternionernion (preserves v.w).
        /// </summary>
        public static cVector4 operator*(cQuaternion q, cVector4 v) => new cVector4(q * new cVector3(v), v.w);
        
        /// <summary>
        /// Returns a Vectortor rotated by the inverted Quaternionernion.
        /// </summary>
        public static cVector3 operator*(cVector3 v, cQuaternion q) => q.Inverse * v;
        
        /// <summary>
        /// Returns a Vectortor rotated by the inverted Quaternionernion (preserves v.w).
        /// </summary>
        public static cVector4 operator*(cVector4 v, cQuaternion q) => q.Inverse * v;

        #endregion


        #region Functions
        
        /// <summary>
        /// Returns an enumerator that iterates through all components.
        /// </summary>
        public IEnumerator<Complex> GetEnumerator()
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
        public bool Equals(cQuaternion rhs) => ((x.Equals(rhs.x) && y.Equals(rhs.y)) && (z.Equals(rhs.z) && w.Equals(rhs.w)));
        
        /// <summary>
        /// Returns true iff this equals rhs type- and component-wise.
        /// </summary>
        public override bool Equals(object obj)
        {
            if (ReferenceEquals(null, obj)) return false;
            return obj is cQuaternion && Equals((cQuaternion) obj);
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
        /// Returns the inner product (dot product, scalar product) of the two Quaternionernions.
        /// </summary>
        public static Complex Dot(cQuaternion lhs, cQuaternion rhs) => ((lhs.x * Complex.Conjugate(rhs.x) + lhs.y * Complex.Conjugate(rhs.y)) + (lhs.z * Complex.Conjugate(rhs.z) + lhs.w * Complex.Conjugate(rhs.w)));
        
        /// <summary>
        /// Returns the cross product between two Quaternionernions.
        /// </summary>
        public static cQuaternion Cross(cQuaternion q1, cQuaternion q2) => new cQuaternion(q1.w * q2.x + q1.x * q2.w + q1.y * q2.z - q1.z * q2.y, q1.w * q2.y + q1.y * q2.w + q1.z * q2.x - q1.x * q2.z, q1.w * q2.z + q1.z * q2.w + q1.x * q2.y - q1.y * q2.x, q1.w * q2.w - q1.x * q2.x - q1.y * q2.y - q1.z * q2.z);

        #endregion


        #region Component-Wise Static Functions
        
        /// <summary>
        /// Returns a bVector4 from component-wise application of Equal (lhs == rhs).
        /// </summary>
        public static bVector4 Equal(cQuaternion lhs, cQuaternion rhs) => new bVector4(lhs.x == rhs.x, lhs.y == rhs.y, lhs.z == rhs.z, lhs.w == rhs.w);
        
        /// <summary>
        /// Returns a bVector4 from component-wise application of Equal (lhs == rhs).
        /// </summary>
        public static bVector4 Equal(cQuaternion lhs, Complex rhs) => new bVector4(lhs.x == rhs, lhs.y == rhs, lhs.z == rhs, lhs.w == rhs);
        
        /// <summary>
        /// Returns a bVector4 from component-wise application of Equal (lhs == rhs).
        /// </summary>
        public static bVector4 Equal(Complex lhs, cQuaternion rhs) => new bVector4(lhs == rhs.x, lhs == rhs.y, lhs == rhs.z, lhs == rhs.w);
        
        /// <summary>
        /// Returns a bVector from the application of Equal (lhs == rhs).
        /// </summary>
        public static bVector4 Equal(Complex lhs, Complex rhs) => new bVector4(lhs == rhs);
        
        /// <summary>
        /// Returns a bVector4 from component-wise application of NotEqual (lhs != rhs).
        /// </summary>
        public static bVector4 NotEqual(cQuaternion lhs, cQuaternion rhs) => new bVector4(lhs.x != rhs.x, lhs.y != rhs.y, lhs.z != rhs.z, lhs.w != rhs.w);
        
        /// <summary>
        /// Returns a bVector4 from component-wise application of NotEqual (lhs != rhs).
        /// </summary>
        public static bVector4 NotEqual(cQuaternion lhs, Complex rhs) => new bVector4(lhs.x != rhs, lhs.y != rhs, lhs.z != rhs, lhs.w != rhs);
        
        /// <summary>
        /// Returns a bVector4 from component-wise application of NotEqual (lhs != rhs).
        /// </summary>
        public static bVector4 NotEqual(Complex lhs, cQuaternion rhs) => new bVector4(lhs != rhs.x, lhs != rhs.y, lhs != rhs.z, lhs != rhs.w);
        
        /// <summary>
        /// Returns a bVector from the application of NotEqual (lhs != rhs).
        /// </summary>
        public static bVector4 NotEqual(Complex lhs, Complex rhs) => new bVector4(lhs != rhs);
        
        /// <summary>
        /// Returns a cQuaternion from component-wise application of Lerp (min * (1-a) + max * a).
        /// </summary>
        public static cQuaternion Lerp(cQuaternion min, cQuaternion max, cQuaternion a) => new cQuaternion(min.x * (1-a.x) + max.x * a.x, min.y * (1-a.y) + max.y * a.y, min.z * (1-a.z) + max.z * a.z, min.w * (1-a.w) + max.w * a.w);
        
        /// <summary>
        /// Returns a cQuaternion from component-wise application of Lerp (min * (1-a) + max * a).
        /// </summary>
        public static cQuaternion Lerp(cQuaternion min, cQuaternion max, Complex a) => new cQuaternion(min.x * (1-a) + max.x * a, min.y * (1-a) + max.y * a, min.z * (1-a) + max.z * a, min.w * (1-a) + max.w * a);
        
        /// <summary>
        /// Returns a cQuaternion from component-wise application of Lerp (min * (1-a) + max * a).
        /// </summary>
        public static cQuaternion Lerp(cQuaternion min, Complex max, cQuaternion a) => new cQuaternion(min.x * (1-a.x) + max * a.x, min.y * (1-a.y) + max * a.y, min.z * (1-a.z) + max * a.z, min.w * (1-a.w) + max * a.w);
        
        /// <summary>
        /// Returns a cQuaternion from component-wise application of Lerp (min * (1-a) + max * a).
        /// </summary>
        public static cQuaternion Lerp(cQuaternion min, Complex max, Complex a) => new cQuaternion(min.x * (1-a) + max * a, min.y * (1-a) + max * a, min.z * (1-a) + max * a, min.w * (1-a) + max * a);
        
        /// <summary>
        /// Returns a cQuaternion from component-wise application of Lerp (min * (1-a) + max * a).
        /// </summary>
        public static cQuaternion Lerp(Complex min, cQuaternion max, cQuaternion a) => new cQuaternion(min * (1-a.x) + max.x * a.x, min * (1-a.y) + max.y * a.y, min * (1-a.z) + max.z * a.z, min * (1-a.w) + max.w * a.w);
        
        /// <summary>
        /// Returns a cQuaternion from component-wise application of Lerp (min * (1-a) + max * a).
        /// </summary>
        public static cQuaternion Lerp(Complex min, cQuaternion max, Complex a) => new cQuaternion(min * (1-a) + max.x * a, min * (1-a) + max.y * a, min * (1-a) + max.z * a, min * (1-a) + max.w * a);
        
        /// <summary>
        /// Returns a cQuaternion from component-wise application of Lerp (min * (1-a) + max * a).
        /// </summary>
        public static cQuaternion Lerp(Complex min, Complex max, cQuaternion a) => new cQuaternion(min * (1-a.x) + max * a.x, min * (1-a.y) + max * a.y, min * (1-a.z) + max * a.z, min * (1-a.w) + max * a.w);
        
        /// <summary>
        /// Returns a cQuaternion from the application of Lerp (min * (1-a) + max * a).
        /// </summary>
        public static cQuaternion Lerp(Complex min, Complex max, Complex a) => new cQuaternion(min * (1-a) + max * a);

        #endregion


        #region Component-Wise Operator Overloads
        
        /// <summary>
        /// Returns a cQuaternion from component-wise application of operator+ (identity).
        /// </summary>
        public static cQuaternion operator+(cQuaternion v) => v;
        
        /// <summary>
        /// Returns a cQuaternion from component-wise application of operator- (-v).
        /// </summary>
        public static cQuaternion operator-(cQuaternion v) => new cQuaternion(-v.x, -v.y, -v.z, -v.w);
        
        /// <summary>
        /// Returns a cQuaternion from component-wise application of operator+ (lhs + rhs).
        /// </summary>
        public static cQuaternion operator+(cQuaternion lhs, cQuaternion rhs) => new cQuaternion(lhs.x + rhs.x, lhs.y + rhs.y, lhs.z + rhs.z, lhs.w + rhs.w);
        
        /// <summary>
        /// Returns a cQuaternion from component-wise application of operator+ (lhs + rhs).
        /// </summary>
        public static cQuaternion operator+(cQuaternion lhs, Complex rhs) => new cQuaternion(lhs.x + rhs, lhs.y + rhs, lhs.z + rhs, lhs.w + rhs);
        
        /// <summary>
        /// Returns a cQuaternion from component-wise application of operator+ (lhs + rhs).
        /// </summary>
        public static cQuaternion operator+(Complex lhs, cQuaternion rhs) => new cQuaternion(lhs + rhs.x, lhs + rhs.y, lhs + rhs.z, lhs + rhs.w);
        
        /// <summary>
        /// Returns a cQuaternion from component-wise application of operator- (lhs - rhs).
        /// </summary>
        public static cQuaternion operator-(cQuaternion lhs, cQuaternion rhs) => new cQuaternion(lhs.x - rhs.x, lhs.y - rhs.y, lhs.z - rhs.z, lhs.w - rhs.w);
        
        /// <summary>
        /// Returns a cQuaternion from component-wise application of operator- (lhs - rhs).
        /// </summary>
        public static cQuaternion operator-(cQuaternion lhs, Complex rhs) => new cQuaternion(lhs.x - rhs, lhs.y - rhs, lhs.z - rhs, lhs.w - rhs);
        
        /// <summary>
        /// Returns a cQuaternion from component-wise application of operator- (lhs - rhs).
        /// </summary>
        public static cQuaternion operator-(Complex lhs, cQuaternion rhs) => new cQuaternion(lhs - rhs.x, lhs - rhs.y, lhs - rhs.z, lhs - rhs.w);
        
        /// <summary>
        /// Returns a cQuaternion from component-wise application of operator* (lhs * rhs).
        /// </summary>
        public static cQuaternion operator*(cQuaternion lhs, Complex rhs) => new cQuaternion(lhs.x * rhs, lhs.y * rhs, lhs.z * rhs, lhs.w * rhs);
        
        /// <summary>
        /// Returns a cQuaternion from component-wise application of operator* (lhs * rhs).
        /// </summary>
        public static cQuaternion operator*(Complex lhs, cQuaternion rhs) => new cQuaternion(lhs * rhs.x, lhs * rhs.y, lhs * rhs.z, lhs * rhs.w);
        
        /// <summary>
        /// Returns a cQuaternion from component-wise application of operator/ (lhs / rhs).
        /// </summary>
        public static cQuaternion operator/(cQuaternion lhs, Complex rhs) => new cQuaternion(lhs.x / rhs, lhs.y / rhs, lhs.z / rhs, lhs.w / rhs);

        #endregion

    }
    */
}
