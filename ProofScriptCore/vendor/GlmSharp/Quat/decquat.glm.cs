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
    /// Static class that contains static glm functions
    /// </summary>
    public static partial class glm
    {
        
        /// <summary>
        /// Returns an array with all values
        /// </summary>
        public static decimal[] Values(decQuaternion q) => q.Values;
        
        /// <summary>
        /// Returns an enumerator that iterates through all components.
        /// </summary>
        public static IEnumerator<decimal> GetEnumerator(decQuaternion q) => q.GetEnumerator();
        
        /// <summary>
        /// Returns a string representation of this Quaternionernion using ', ' as a seperator.
        /// </summary>
        public static string ToString(decQuaternion q) => q.ToString();
        
        /// <summary>
        /// Returns a string representation of this Quaternionernion using a provided seperator.
        /// </summary>
        public static string ToString(decQuaternion q, string sep) => q.ToString(sep);
        
        /// <summary>
        /// Returns a string representation of this Quaternionernion using a provided seperator and a forMatrix provider for each component.
        /// </summary>
        public static string ToString(decQuaternion q, string sep, IFormatProvider provider) => q.ToString(sep, provider);
        
        /// <summary>
        /// Returns a string representation of this Quaternionernion using a provided seperator and a forMatrix for each component.
        /// </summary>
        public static string ToString(decQuaternion q, string sep, string forMatrix) => q.ToString(sep, forMatrix);
        
        /// <summary>
        /// Returns a string representation of this Quaternionernion using a provided seperator and a forMatrix and forMatrix provider for each component.
        /// </summary>
        public static string ToString(decQuaternion q, string sep, string forMatrix, IFormatProvider provider) => q.ToString(sep, forMatrix, provider);
        
        /// <summary>
        /// Returns the number of components (4).
        /// </summary>
        public static int Count(decQuaternion q) => q.Count;
        
        /// <summary>
        /// Returns true iff this equals rhs component-wise.
        /// </summary>
        public static bool Equals(decQuaternion q, decQuaternion rhs) => q.Equals(rhs);
        
        /// <summary>
        /// Returns true iff this equals rhs type- and component-wise.
        /// </summary>
        public static bool Equals(decQuaternion q, object obj) => q.Equals(obj);
        
        /// <summary>
        /// Returns a hash code for this instance.
        /// </summary>
        public static int GetHashCode(decQuaternion q) => q.GetHashCode();
        
        /// <summary>
        /// Returns a bVector4 from component-wise application of Equal (lhs == rhs).
        /// </summary>
        public static bVector4 Equal(decQuaternion lhs, decQuaternion rhs) => decQuaternion.Equal(lhs, rhs);
        
        /// <summary>
        /// Returns a bVector4 from component-wise application of NotEqual (lhs != rhs).
        /// </summary>
        public static bVector4 NotEqual(decQuaternion lhs, decQuaternion rhs) => decQuaternion.NotEqual(lhs, rhs);
        
        /// <summary>
        /// Returns a bVector4 from component-wise application of GreaterThan (lhs &gt; rhs).
        /// </summary>
        public static bVector4 GreaterThan(decQuaternion lhs, decQuaternion rhs) => decQuaternion.GreaterThan(lhs, rhs);
        
        /// <summary>
        /// Returns a bVector4 from component-wise application of GreaterThanEqual (lhs &gt;= rhs).
        /// </summary>
        public static bVector4 GreaterThanEqual(decQuaternion lhs, decQuaternion rhs) => decQuaternion.GreaterThanEqual(lhs, rhs);
        
        /// <summary>
        /// Returns a bVector4 from component-wise application of LesserThan (lhs &lt; rhs).
        /// </summary>
        public static bVector4 LesserThan(decQuaternion lhs, decQuaternion rhs) => decQuaternion.LesserThan(lhs, rhs);
        
        /// <summary>
        /// Returns a bVector4 from component-wise application of LesserThanEqual (lhs &lt;= rhs).
        /// </summary>
        public static bVector4 LesserThanEqual(decQuaternion lhs, decQuaternion rhs) => decQuaternion.LesserThanEqual(lhs, rhs);
        
        /// <summary>
        /// Returns the inner product (dot product, scalar product) of the two Quaternionernions.
        /// </summary>
        public static decimal Dot(decQuaternion lhs, decQuaternion rhs) => decQuaternion.Dot(lhs, rhs);
        
        /// <summary>
        /// Returns the euclidean length of this Quaternionernion.
        /// </summary>
        public static decimal Length(decQuaternion q) => q.Length;
        
        /// <summary>
        /// Returns the squared euclidean length of this Quaternionernion.
        /// </summary>
        public static decimal LengthSqr(decQuaternion q) => q.LengthSqr;
        
        /// <summary>
        /// Returns a copy of this Quaternionernion with length one (undefined if this has zero length).
        /// </summary>
        public static decQuaternion Normalized(decQuaternion q) => q.Normalized;
        
        /// <summary>
        /// Returns a copy of this Quaternionernion with length one (returns zero if length is zero).
        /// </summary>
        public static decQuaternion NormalizedSafe(decQuaternion q) => q.NormalizedSafe;
        
        /// <summary>
        /// Returns the represented angle of this Quaternionernion.
        /// </summary>
        public static double Angle(decQuaternion q) => q.Angle;
        
        /// <summary>
        /// Returns the represented axis of this Quaternionernion.
        /// </summary>
        public static decVector3 Axis(decQuaternion q) => q.Axis;
        
        /// <summary>
        /// Returns the represented yaw angle of this Quaternionernion.
        /// </summary>
        public static double Yaw(decQuaternion q) => q.Yaw;
        
        /// <summary>
        /// Returns the represented pitch angle of this Quaternionernion.
        /// </summary>
        public static double Pitch(decQuaternion q) => q.Pitch;
        
        /// <summary>
        /// Returns the represented roll angle of this Quaternionernion.
        /// </summary>
        public static double Roll(decQuaternion q) => q.Roll;
        
        /// <summary>
        /// Returns the represented euler angles (pitch, yaw, roll) of this Quaternionernion.
        /// </summary>
        public static dVector3 EulerAngles(decQuaternion q) => q.EulerAngles;
        
        /// <summary>
        /// Rotates this Quaternionernion from an axis and an angle (in radians).
        /// </summary>
        public static decQuaternion Rotated(decQuaternion q, decimal angle, decVector3 v) => q.Rotated(angle, v);
        
        /// <summary>
        /// Creates a decMatrix3 that realizes the rotation of this Quaternionernion
        /// </summary>
        public static decMatrix3 ToMat3(decQuaternion q) => q.ToMat3;
        
        /// <summary>
        /// Creates a decMatrix4 that realizes the rotation of this Quaternionernion
        /// </summary>
        public static decMatrix4 ToMat4(decQuaternion q) => q.ToMat4;
        
        /// <summary>
        /// Returns the conjugated Quaternionernion
        /// </summary>
        public static decQuaternion Conjugate(decQuaternion q) => q.Conjugate;
        
        /// <summary>
        /// Returns the inverse Quaternionernion
        /// </summary>
        public static decQuaternion Inverse(decQuaternion q) => q.Inverse;
        
        /// <summary>
        /// Returns the cross product between two Quaternionernions.
        /// </summary>
        public static decQuaternion Cross(decQuaternion q1, decQuaternion q2) => decQuaternion.Cross(q1, q2);
        
        /// <summary>
        /// Calculates a proper spherical interpolation between two Quaternionernions (only works for normalized Quaternionernions).
        /// </summary>
        public static decQuaternion Mix(decQuaternion x, decQuaternion y, decimal a) => decQuaternion.Mix(x, y, a);
        
        /// <summary>
        /// Calculates a proper spherical interpolation between two Quaternionernions (only works for normalized Quaternionernions).
        /// </summary>
        public static decQuaternion SLerp(decQuaternion x, decQuaternion y, decimal a) => decQuaternion.SLerp(x, y, a);
        
        /// <summary>
        /// Applies squad interpolation of these Quaternionernions
        /// </summary>
        public static decQuaternion Squad(decQuaternion q1, decQuaternion q2, decQuaternion s1, decQuaternion s2, decimal h) => decQuaternion.Squad(q1, q2, s1, s2, h);
        
        /// <summary>
        /// Returns a decQuaternion from component-wise application of Lerp (min * (1-a) + max * a).
        /// </summary>
        public static decQuaternion Lerp(decQuaternion min, decQuaternion max, decQuaternion a) => decQuaternion.Lerp(min, max, a);

    }
}
