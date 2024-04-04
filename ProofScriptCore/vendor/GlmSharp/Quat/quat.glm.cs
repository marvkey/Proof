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
        public static float[] Values(Quaternion q) => q.Values;
        
        /// <summary>
        /// Returns an enumerator that iterates through all components.
        /// </summary>
        public static IEnumerator<float> GetEnumerator(Quaternion q) => q.GetEnumerator();
        
        /// <summary>
        /// Returns a string representation of this Quaternionernion using ', ' as a seperator.
        /// </summary>
        public static string ToString(Quaternion q) => q.ToString();
        
        /// <summary>
        /// Returns a string representation of this Quaternionernion using a provided seperator.
        /// </summary>
        public static string ToString(Quaternion q, string sep) => q.ToString(sep);
        
        /// <summary>
        /// Returns a string representation of this Quaternionernion using a provided seperator and a forMatrix provider for each component.
        /// </summary>
        public static string ToString(Quaternion q, string sep, IFormatProvider provider) => q.ToString(sep, provider);
        
        /// <summary>
        /// Returns a string representation of this Quaternionernion using a provided seperator and a forMatrix for each component.
        /// </summary>
        public static string ToString(Quaternion q, string sep, string forMatrix) => q.ToString(sep, forMatrix);
        
        /// <summary>
        /// Returns a string representation of this Quaternionernion using a provided seperator and a forMatrix and forMatrix provider for each component.
        /// </summary>
        public static string ToString(Quaternion q, string sep, string forMatrix, IFormatProvider provider) => q.ToString(sep, forMatrix, provider);
        
        /// <summary>
        /// Returns the number of components (4).
        /// </summary>
        public static int Count(Quaternion q) => q.Count;
        
        /// <summary>
        /// Returns true iff this equals rhs component-wise.
        /// </summary>
        public static bool Equals(Quaternion q, Quaternion rhs) => q.Equals(rhs);
        
        /// <summary>
        /// Returns true iff this equals rhs type- and component-wise.
        /// </summary>
        public static bool Equals(Quaternion q, object obj) => q.Equals(obj);
        
        /// <summary>
        /// Returns a hash code for this instance.
        /// </summary>
        public static int GetHashCode(Quaternion q) => q.GetHashCode();
        
        /// <summary>
        /// Returns a bVector4 from component-wise application of IsInfinity (float.IsInfinity(v)).
        /// </summary>
        public static bVector4 IsInfinity(Quaternion v) => Quaternion.IsInfinity(v);
        
        /// <summary>
        /// Returns a bVector4 from component-wise application of IsFinite (!float.IsNaN(v) &amp;&amp; !float.IsInfinity(v)).
        /// </summary>
        public static bVector4 IsFinite(Quaternion v) => Quaternion.IsFinite(v);
        
        /// <summary>
        /// Returns a bVector4 from component-wise application of IsNaN (float.IsNaN(v)).
        /// </summary>
        public static bVector4 IsNaN(Quaternion v) => Quaternion.IsNaN(v);
        
        /// <summary>
        /// Returns a bVector4 from component-wise application of IsNegativeInfinity (float.IsNegativeInfinity(v)).
        /// </summary>
        public static bVector4 IsNegativeInfinity(Quaternion v) => Quaternion.IsNegativeInfinity(v);
        
        /// <summary>
        /// Returns a bVector4 from component-wise application of IsPositiveInfinity (float.IsPositiveInfinity(v)).
        /// </summary>
        public static bVector4 IsPositiveInfinity(Quaternion v) => Quaternion.IsPositiveInfinity(v);
        
        /// <summary>
        /// Returns a bVector4 from component-wise application of Equal (lhs == rhs).
        /// </summary>
        public static bVector4 Equal(Quaternion lhs, Quaternion rhs) => Quaternion.Equal(lhs, rhs);
        
        /// <summary>
        /// Returns a bVector4 from component-wise application of NotEqual (lhs != rhs).
        /// </summary>
        public static bVector4 NotEqual(Quaternion lhs, Quaternion rhs) => Quaternion.NotEqual(lhs, rhs);
        
        /// <summary>
        /// Returns a bVector4 from component-wise application of GreaterThan (lhs &gt; rhs).
        /// </summary>
        public static bVector4 GreaterThan(Quaternion lhs, Quaternion rhs) => Quaternion.GreaterThan(lhs, rhs);
        
        /// <summary>
        /// Returns a bVector4 from component-wise application of GreaterThanEqual (lhs &gt;= rhs).
        /// </summary>
        public static bVector4 GreaterThanEqual(Quaternion lhs, Quaternion rhs) => Quaternion.GreaterThanEqual(lhs, rhs);
        
        /// <summary>
        /// Returns a bVector4 from component-wise application of LesserThan (lhs &lt; rhs).
        /// </summary>
        public static bVector4 LesserThan(Quaternion lhs, Quaternion rhs) => Quaternion.LesserThan(lhs, rhs);
        
        /// <summary>
        /// Returns a bVector4 from component-wise application of LesserThanEqual (lhs &lt;= rhs).
        /// </summary>
        public static bVector4 LesserThanEqual(Quaternion lhs, Quaternion rhs) => Quaternion.LesserThanEqual(lhs, rhs);
        
        /// <summary>
        /// Returns the inner product (dot product, scalar product) of the two Quaternionernions.
        /// </summary>
        public static float Dot(Quaternion lhs, Quaternion rhs) => Quaternion.Dot(lhs, rhs);
        
        /// <summary>
        /// Returns the euclidean length of this Quaternionernion.
        /// </summary>
        public static float Length(Quaternion q) => q.Length;
        
        /// <summary>
        /// Returns the squared euclidean length of this Quaternionernion.
        /// </summary>
        public static float LengthSqr(Quaternion q) => q.LengthSqr;
        
        /// <summary>
        /// Returns a copy of this Quaternionernion with length one (undefined if this has zero length).
        /// </summary>
        public static Quaternion Normalized(Quaternion q) => q.Normalized;
        
        /// <summary>
        /// Returns a copy of this Quaternionernion with length one (returns zero if length is zero).
        /// </summary>
        public static Quaternion NormalizedSafe(Quaternion q) => q.NormalizedSafe;
        
        /// <summary>
        /// Returns the represented angle of this Quaternionernion.
        /// </summary>
        public static double Angle(Quaternion q) => q.Angle;
        
        /// <summary>
        /// Returns the represented axis of this Quaternionernion.
        /// </summary>
        public static Vector3 Axis(Quaternion q) => q.Axis;
        
        /// <summary>
        /// Returns the represented yaw angle of this Quaternionernion.
        /// </summary>
        public static double Yaw(Quaternion q) => q.Yaw;
        
        /// <summary>
        /// Returns the represented pitch angle of this Quaternionernion.
        /// </summary>
        public static double Pitch(Quaternion q) => q.Pitch;
        
        /// <summary>
        /// Returns the represented roll angle of this Quaternionernion.
        /// </summary>
        public static double Roll(Quaternion q) => q.Roll;
        
        /// <summary>
        /// Returns the represented euler angles (pitch, yaw, roll) of this Quaternionernion.
        /// </summary>
        public static dVector3 EulerAngles(Quaternion q) => q.EulerAngles;
        
        /// <summary>
        /// Rotates this Quaternionernion from an axis and an angle (in radians).
        /// </summary>
        public static Quaternion Rotated(Quaternion q, float angle, Vector3 v) => q.Rotated(angle, v);
        
        /// <summary>
        /// Creates a Matrix3 that realizes the rotation of this Quaternionernion
        /// </summary>
        public static Matrix3 ToMat3(Quaternion q) => q.ToMat3;
        
        /// <summary>
        /// Creates a Matrix4 that realizes the rotation of this Quaternionernion
        /// </summary>
        public static Matrix4 ToMat4(Quaternion q) => q.ToMat4;
        
        /// <summary>
        /// Returns the conjugated Quaternionernion
        /// </summary>
        public static Quaternion Conjugate(Quaternion q) => q.Conjugate;
        
        /// <summary>
        /// Returns the inverse Quaternionernion
        /// </summary>
        public static Quaternion Inverse(Quaternion q) => q.Inverse;
        
        /// <summary>
        /// Returns the cross product between two Quaternionernions.
        /// </summary>
        public static Quaternion Cross(Quaternion q1, Quaternion q2) => Quaternion.Cross(q1, q2);
        
        /// <summary>
        /// Calculates a proper spherical interpolation between two Quaternionernions (only works for normalized Quaternionernions).
        /// </summary>
        public static Quaternion Mix(Quaternion x, Quaternion y, float a) => Quaternion.Mix(x, y, a);
        
        /// <summary>
        /// Calculates a proper spherical interpolation between two Quaternionernions (only works for normalized Quaternionernions).
        /// </summary>
        public static Quaternion SLerp(Quaternion x, Quaternion y, float a) => Quaternion.SLerp(x, y, a);
        
        /// <summary>
        /// Applies squad interpolation of these Quaternionernions
        /// </summary>
        public static Quaternion Squad(Quaternion q1, Quaternion q2, Quaternion s1, Quaternion s2, float h) => Quaternion.Squad(q1, q2, s1, s2, h);
        
        /// <summary>
        /// Returns a Quaternion from component-wise application of Lerp (min * (1-a) + max * a).
        /// </summary>
        public static Quaternion Lerp(Quaternion min, Quaternion max, Quaternion a) => Quaternion.Lerp(min, max, a);

    }
}
