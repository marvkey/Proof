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
        public static double[] Values(dQuaternion q) => q.Values;
        
        /// <summary>
        /// Returns an enumerator that iterates through all components.
        /// </summary>
        public static IEnumerator<double> GetEnumerator(dQuaternion q) => q.GetEnumerator();
        
        /// <summary>
        /// Returns a string representation of this Quaternionernion using ', ' as a seperator.
        /// </summary>
        public static string ToString(dQuaternion q) => q.ToString();
        
        /// <summary>
        /// Returns a string representation of this Quaternionernion using a provided seperator.
        /// </summary>
        public static string ToString(dQuaternion q, string sep) => q.ToString(sep);
        
        /// <summary>
        /// Returns a string representation of this Quaternionernion using a provided seperator and a forMatrix provider for each component.
        /// </summary>
        public static string ToString(dQuaternion q, string sep, IFormatProvider provider) => q.ToString(sep, provider);
        
        /// <summary>
        /// Returns a string representation of this Quaternionernion using a provided seperator and a forMatrix for each component.
        /// </summary>
        public static string ToString(dQuaternion q, string sep, string forMatrix) => q.ToString(sep, forMatrix);
        
        /// <summary>
        /// Returns a string representation of this Quaternionernion using a provided seperator and a forMatrix and forMatrix provider for each component.
        /// </summary>
        public static string ToString(dQuaternion q, string sep, string forMatrix, IFormatProvider provider) => q.ToString(sep, forMatrix, provider);
        
        /// <summary>
        /// Returns the number of components (4).
        /// </summary>
        public static int Count(dQuaternion q) => q.Count;
        
        /// <summary>
        /// Returns true iff this equals rhs component-wise.
        /// </summary>
        public static bool Equals(dQuaternion q, dQuaternion rhs) => q.Equals(rhs);
        
        /// <summary>
        /// Returns true iff this equals rhs type- and component-wise.
        /// </summary>
        public static bool Equals(dQuaternion q, object obj) => q.Equals(obj);
        
        /// <summary>
        /// Returns a hash code for this instance.
        /// </summary>
        public static int GetHashCode(dQuaternion q) => q.GetHashCode();
        
        /// <summary>
        /// Returns a bVector4 from component-wise application of IsInfinity (double.IsInfinity(v)).
        /// </summary>
        public static bVector4 IsInfinity(dQuaternion v) => dQuaternion.IsInfinity(v);
        
        /// <summary>
        /// Returns a bVector4 from component-wise application of IsFinite (!double.IsNaN(v) &amp;&amp; !double.IsInfinity(v)).
        /// </summary>
        public static bVector4 IsFinite(dQuaternion v) => dQuaternion.IsFinite(v);
        
        /// <summary>
        /// Returns a bVector4 from component-wise application of IsNaN (double.IsNaN(v)).
        /// </summary>
        public static bVector4 IsNaN(dQuaternion v) => dQuaternion.IsNaN(v);
        
        /// <summary>
        /// Returns a bVector4 from component-wise application of IsNegativeInfinity (double.IsNegativeInfinity(v)).
        /// </summary>
        public static bVector4 IsNegativeInfinity(dQuaternion v) => dQuaternion.IsNegativeInfinity(v);
        
        /// <summary>
        /// Returns a bVector4 from component-wise application of IsPositiveInfinity (double.IsPositiveInfinity(v)).
        /// </summary>
        public static bVector4 IsPositiveInfinity(dQuaternion v) => dQuaternion.IsPositiveInfinity(v);
        
        /// <summary>
        /// Returns a bVector4 from component-wise application of Equal (lhs == rhs).
        /// </summary>
        public static bVector4 Equal(dQuaternion lhs, dQuaternion rhs) => dQuaternion.Equal(lhs, rhs);
        
        /// <summary>
        /// Returns a bVector4 from component-wise application of NotEqual (lhs != rhs).
        /// </summary>
        public static bVector4 NotEqual(dQuaternion lhs, dQuaternion rhs) => dQuaternion.NotEqual(lhs, rhs);
        
        /// <summary>
        /// Returns a bVector4 from component-wise application of GreaterThan (lhs &gt; rhs).
        /// </summary>
        public static bVector4 GreaterThan(dQuaternion lhs, dQuaternion rhs) => dQuaternion.GreaterThan(lhs, rhs);
        
        /// <summary>
        /// Returns a bVector4 from component-wise application of GreaterThanEqual (lhs &gt;= rhs).
        /// </summary>
        public static bVector4 GreaterThanEqual(dQuaternion lhs, dQuaternion rhs) => dQuaternion.GreaterThanEqual(lhs, rhs);
        
        /// <summary>
        /// Returns a bVector4 from component-wise application of LesserThan (lhs &lt; rhs).
        /// </summary>
        public static bVector4 LesserThan(dQuaternion lhs, dQuaternion rhs) => dQuaternion.LesserThan(lhs, rhs);
        
        /// <summary>
        /// Returns a bVector4 from component-wise application of LesserThanEqual (lhs &lt;= rhs).
        /// </summary>
        public static bVector4 LesserThanEqual(dQuaternion lhs, dQuaternion rhs) => dQuaternion.LesserThanEqual(lhs, rhs);
        
        /// <summary>
        /// Returns the inner product (dot product, scalar product) of the two Quaternionernions.
        /// </summary>
        public static double Dot(dQuaternion lhs, dQuaternion rhs) => dQuaternion.Dot(lhs, rhs);
        
        /// <summary>
        /// Returns the euclidean length of this Quaternionernion.
        /// </summary>
        public static double Length(dQuaternion q) => q.Length;
        
        /// <summary>
        /// Returns the squared euclidean length of this Quaternionernion.
        /// </summary>
        public static double LengthSqr(dQuaternion q) => q.LengthSqr;
        
        /// <summary>
        /// Returns a copy of this Quaternionernion with length one (undefined if this has zero length).
        /// </summary>
        public static dQuaternion Normalized(dQuaternion q) => q.Normalized;
        
        /// <summary>
        /// Returns a copy of this Quaternionernion with length one (returns zero if length is zero).
        /// </summary>
        public static dQuaternion NormalizedSafe(dQuaternion q) => q.NormalizedSafe;
        
        /// <summary>
        /// Returns the represented angle of this Quaternionernion.
        /// </summary>
        public static double Angle(dQuaternion q) => q.Angle;
        
        /// <summary>
        /// Returns the represented axis of this Quaternionernion.
        /// </summary>
        public static dVector3 Axis(dQuaternion q) => q.Axis;
        
        /// <summary>
        /// Returns the represented yaw angle of this Quaternionernion.
        /// </summary>
        public static double Yaw(dQuaternion q) => q.Yaw;
        
        /// <summary>
        /// Returns the represented pitch angle of this Quaternionernion.
        /// </summary>
        public static double Pitch(dQuaternion q) => q.Pitch;
        
        /// <summary>
        /// Returns the represented roll angle of this Quaternionernion.
        /// </summary>
        public static double Roll(dQuaternion q) => q.Roll;
        
        /// <summary>
        /// Returns the represented euler angles (pitch, yaw, roll) of this Quaternionernion.
        /// </summary>
        public static dVector3 EulerAngles(dQuaternion q) => q.EulerAngles;
        
        /// <summary>
        /// Rotates this Quaternionernion from an axis and an angle (in radians).
        /// </summary>
        public static dQuaternion Rotated(dQuaternion q, double angle, dVector3 v) => q.Rotated(angle, v);
        
        /// <summary>
        /// Creates a dMatrix3 that realizes the rotation of this Quaternionernion
        /// </summary>
        public static dMatrix3 ToMat3(dQuaternion q) => q.ToMat3;
        
        /// <summary>
        /// Creates a dMatrix4 that realizes the rotation of this Quaternionernion
        /// </summary>
        public static dMatrix4 ToMat4(dQuaternion q) => q.ToMat4;
        
        /// <summary>
        /// Returns the conjugated Quaternionernion
        /// </summary>
        public static dQuaternion Conjugate(dQuaternion q) => q.Conjugate;
        
        /// <summary>
        /// Returns the inverse Quaternionernion
        /// </summary>
        public static dQuaternion Inverse(dQuaternion q) => q.Inverse;
        
        /// <summary>
        /// Returns the cross product between two Quaternionernions.
        /// </summary>
        public static dQuaternion Cross(dQuaternion q1, dQuaternion q2) => dQuaternion.Cross(q1, q2);
        
        /// <summary>
        /// Calculates a proper spherical interpolation between two Quaternionernions (only works for normalized Quaternionernions).
        /// </summary>
        public static dQuaternion Mix(dQuaternion x, dQuaternion y, double a) => dQuaternion.Mix(x, y, a);
        
        /// <summary>
        /// Calculates a proper spherical interpolation between two Quaternionernions (only works for normalized Quaternionernions).
        /// </summary>
        public static dQuaternion SLerp(dQuaternion x, dQuaternion y, double a) => dQuaternion.SLerp(x, y, a);
        
        /// <summary>
        /// Applies squad interpolation of these Quaternionernions
        /// </summary>
        public static dQuaternion Squad(dQuaternion q1, dQuaternion q2, dQuaternion s1, dQuaternion s2, double h) => dQuaternion.Squad(q1, q2, s1, s2, h);
        
        /// <summary>
        /// Returns a dQuaternion from component-wise application of Lerp (min * (1-a) + max * a).
        /// </summary>
        public static dQuaternion Lerp(dQuaternion min, dQuaternion max, dQuaternion a) => dQuaternion.Lerp(min, max, a);

    }
}
