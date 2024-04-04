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
        public static Half[] Values(hQuaternion q) => q.Values;
        
        /// <summary>
        /// Returns an enumerator that iterates through all components.
        /// </summary>
        public static IEnumerator<Half> GetEnumerator(hQuaternion q) => q.GetEnumerator();
        
        /// <summary>
        /// Returns a string representation of this Quaternionernion using ', ' as a seperator.
        /// </summary>
        public static string ToString(hQuaternion q) => q.ToString();
        
        /// <summary>
        /// Returns a string representation of this Quaternionernion using a provided seperator.
        /// </summary>
        public static string ToString(hQuaternion q, string sep) => q.ToString(sep);
        
        /// <summary>
        /// Returns a string representation of this Quaternionernion using a provided seperator and a forMatrix provider for each component.
        /// </summary>
        public static string ToString(hQuaternion q, string sep, IFormatProvider provider) => q.ToString(sep, provider);
        
        /// <summary>
        /// Returns a string representation of this Quaternionernion using a provided seperator and a forMatrix for each component.
        /// </summary>
        public static string ToString(hQuaternion q, string sep, string forMatrix) => q.ToString(sep, forMatrix);
        
        /// <summary>
        /// Returns a string representation of this Quaternionernion using a provided seperator and a forMatrix and forMatrix provider for each component.
        /// </summary>
        public static string ToString(hQuaternion q, string sep, string forMatrix, IFormatProvider provider) => q.ToString(sep, forMatrix, provider);
        
        /// <summary>
        /// Returns the number of components (4).
        /// </summary>
        public static int Count(hQuaternion q) => q.Count;
        
        /// <summary>
        /// Returns true iff this equals rhs component-wise.
        /// </summary>
        public static bool Equals(hQuaternion q, hQuaternion rhs) => q.Equals(rhs);
        
        /// <summary>
        /// Returns true iff this equals rhs type- and component-wise.
        /// </summary>
        public static bool Equals(hQuaternion q, object obj) => q.Equals(obj);
        
        /// <summary>
        /// Returns a hash code for this instance.
        /// </summary>
        public static int GetHashCode(hQuaternion q) => q.GetHashCode();
        
        /// <summary>
        /// Returns a bVector4 from component-wise application of IsInfinity (Half.IsInfinity(v)).
        /// </summary>
        public static bVector4 IsInfinity(hQuaternion v) => hQuaternion.IsInfinity(v);
        
        /// <summary>
        /// Returns a bVector4 from component-wise application of IsFinite (!Half.IsNaN(v) &amp;&amp; !Half.IsInfinity(v)).
        /// </summary>
        public static bVector4 IsFinite(hQuaternion v) => hQuaternion.IsFinite(v);
        
        /// <summary>
        /// Returns a bVector4 from component-wise application of IsNaN (Half.IsNaN(v)).
        /// </summary>
        public static bVector4 IsNaN(hQuaternion v) => hQuaternion.IsNaN(v);
        
        /// <summary>
        /// Returns a bVector4 from component-wise application of IsNegativeInfinity (Half.IsNegativeInfinity(v)).
        /// </summary>
        public static bVector4 IsNegativeInfinity(hQuaternion v) => hQuaternion.IsNegativeInfinity(v);
        
        /// <summary>
        /// Returns a bVector4 from component-wise application of IsPositiveInfinity (Half.IsPositiveInfinity(v)).
        /// </summary>
        public static bVector4 IsPositiveInfinity(hQuaternion v) => hQuaternion.IsPositiveInfinity(v);
        
        /// <summary>
        /// Returns a bVector4 from component-wise application of Equal (lhs == rhs).
        /// </summary>
        public static bVector4 Equal(hQuaternion lhs, hQuaternion rhs) => hQuaternion.Equal(lhs, rhs);
        
        /// <summary>
        /// Returns a bVector4 from component-wise application of NotEqual (lhs != rhs).
        /// </summary>
        public static bVector4 NotEqual(hQuaternion lhs, hQuaternion rhs) => hQuaternion.NotEqual(lhs, rhs);
        
        /// <summary>
        /// Returns a bVector4 from component-wise application of GreaterThan (lhs &gt; rhs).
        /// </summary>
        public static bVector4 GreaterThan(hQuaternion lhs, hQuaternion rhs) => hQuaternion.GreaterThan(lhs, rhs);
        
        /// <summary>
        /// Returns a bVector4 from component-wise application of GreaterThanEqual (lhs &gt;= rhs).
        /// </summary>
        public static bVector4 GreaterThanEqual(hQuaternion lhs, hQuaternion rhs) => hQuaternion.GreaterThanEqual(lhs, rhs);
        
        /// <summary>
        /// Returns a bVector4 from component-wise application of LesserThan (lhs &lt; rhs).
        /// </summary>
        public static bVector4 LesserThan(hQuaternion lhs, hQuaternion rhs) => hQuaternion.LesserThan(lhs, rhs);
        
        /// <summary>
        /// Returns a bVector4 from component-wise application of LesserThanEqual (lhs &lt;= rhs).
        /// </summary>
        public static bVector4 LesserThanEqual(hQuaternion lhs, hQuaternion rhs) => hQuaternion.LesserThanEqual(lhs, rhs);
        
        /// <summary>
        /// Returns the inner product (dot product, scalar product) of the two Quaternionernions.
        /// </summary>
        public static Half Dot(hQuaternion lhs, hQuaternion rhs) => hQuaternion.Dot(lhs, rhs);
        
        /// <summary>
        /// Returns the euclidean length of this Quaternionernion.
        /// </summary>
        public static float Length(hQuaternion q) => q.Length;
        
        /// <summary>
        /// Returns the squared euclidean length of this Quaternionernion.
        /// </summary>
        public static Half LengthSqr(hQuaternion q) => q.LengthSqr;
        
        /// <summary>
        /// Returns a copy of this Quaternionernion with length one (undefined if this has zero length).
        /// </summary>
        public static hQuaternion Normalized(hQuaternion q) => q.Normalized;
        
        /// <summary>
        /// Returns a copy of this Quaternionernion with length one (returns zero if length is zero).
        /// </summary>
        public static hQuaternion NormalizedSafe(hQuaternion q) => q.NormalizedSafe;
        
        /// <summary>
        /// Returns the represented angle of this Quaternionernion.
        /// </summary>
        public static double Angle(hQuaternion q) => q.Angle;
        
        /// <summary>
        /// Returns the represented axis of this Quaternionernion.
        /// </summary>
        public static hVector3 Axis(hQuaternion q) => q.Axis;
        
        /// <summary>
        /// Returns the represented yaw angle of this Quaternionernion.
        /// </summary>
        public static double Yaw(hQuaternion q) => q.Yaw;
        
        /// <summary>
        /// Returns the represented pitch angle of this Quaternionernion.
        /// </summary>
        public static double Pitch(hQuaternion q) => q.Pitch;
        
        /// <summary>
        /// Returns the represented roll angle of this Quaternionernion.
        /// </summary>
        public static double Roll(hQuaternion q) => q.Roll;
        
        /// <summary>
        /// Returns the represented euler angles (pitch, yaw, roll) of this Quaternionernion.
        /// </summary>
        public static dVector3 EulerAngles(hQuaternion q) => q.EulerAngles;
        
        /// <summary>
        /// Rotates this Quaternionernion from an axis and an angle (in radians).
        /// </summary>
        public static hQuaternion Rotated(hQuaternion q, Half angle, hVector3 v) => q.Rotated(angle, v);
        
        /// <summary>
        /// Creates a hMatrix3 that realizes the rotation of this Quaternionernion
        /// </summary>
        public static hMatrix3 ToMat3(hQuaternion q) => q.ToMat3;
        
        /// <summary>
        /// Creates a hMatrix4 that realizes the rotation of this Quaternionernion
        /// </summary>
        public static hMatrix4 ToMat4(hQuaternion q) => q.ToMat4;
        
        /// <summary>
        /// Returns the conjugated Quaternionernion
        /// </summary>
        public static hQuaternion Conjugate(hQuaternion q) => q.Conjugate;
        
        /// <summary>
        /// Returns the inverse Quaternionernion
        /// </summary>
        public static hQuaternion Inverse(hQuaternion q) => q.Inverse;
        
        /// <summary>
        /// Returns the cross product between two Quaternionernions.
        /// </summary>
        public static hQuaternion Cross(hQuaternion q1, hQuaternion q2) => hQuaternion.Cross(q1, q2);
        
        /// <summary>
        /// Calculates a proper spherical interpolation between two Quaternionernions (only works for normalized Quaternionernions).
        /// </summary>
        public static hQuaternion Mix(hQuaternion x, hQuaternion y, Half a) => hQuaternion.Mix(x, y, a);
        
        /// <summary>
        /// Calculates a proper spherical interpolation between two Quaternionernions (only works for normalized Quaternionernions).
        /// </summary>
        public static hQuaternion SLerp(hQuaternion x, hQuaternion y, Half a) => hQuaternion.SLerp(x, y, a);
        
        /// <summary>
        /// Applies squad interpolation of these Quaternionernions
        /// </summary>
        public static hQuaternion Squad(hQuaternion q1, hQuaternion q2, hQuaternion s1, hQuaternion s2, Half h) => hQuaternion.Squad(q1, q2, s1, s2, h);
        
        /// <summary>
        /// Returns a hQuaternion from component-wise application of Lerp (min * (1-a) + max * a).
        /// </summary>
        public static hQuaternion Lerp(hQuaternion min, hQuaternion max, hQuaternion a) => hQuaternion.Lerp(min, max, a);

    }
}
