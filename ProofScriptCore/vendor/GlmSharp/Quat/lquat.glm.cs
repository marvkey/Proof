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
        public static long[] Values(lQuaternion q) => q.Values;
        
        /// <summary>
        /// Returns an enumerator that iterates through all components.
        /// </summary>
        public static IEnumerator<long> GetEnumerator(lQuaternion q) => q.GetEnumerator();
        
        /// <summary>
        /// Returns a string representation of this Quaternionernion using ', ' as a seperator.
        /// </summary>
        public static string ToString(lQuaternion q) => q.ToString();
        
        /// <summary>
        /// Returns a string representation of this Quaternionernion using a provided seperator.
        /// </summary>
        public static string ToString(lQuaternion q, string sep) => q.ToString(sep);
        
        /// <summary>
        /// Returns a string representation of this Quaternionernion using a provided seperator and a forMatrix provider for each component.
        /// </summary>
        public static string ToString(lQuaternion q, string sep, IFormatProvider provider) => q.ToString(sep, provider);
        
        /// <summary>
        /// Returns a string representation of this Quaternionernion using a provided seperator and a forMatrix for each component.
        /// </summary>
        public static string ToString(lQuaternion q, string sep, string forMatrix) => q.ToString(sep, forMatrix);
        
        /// <summary>
        /// Returns a string representation of this Quaternionernion using a provided seperator and a forMatrix and forMatrix provider for each component.
        /// </summary>
        public static string ToString(lQuaternion q, string sep, string forMatrix, IFormatProvider provider) => q.ToString(sep, forMatrix, provider);
        
        /// <summary>
        /// Returns the number of components (4).
        /// </summary>
        public static int Count(lQuaternion q) => q.Count;
        
        /// <summary>
        /// Returns true iff this equals rhs component-wise.
        /// </summary>
        public static bool Equals(lQuaternion q, lQuaternion rhs) => q.Equals(rhs);
        
        /// <summary>
        /// Returns true iff this equals rhs type- and component-wise.
        /// </summary>
        public static bool Equals(lQuaternion q, object obj) => q.Equals(obj);
        
        /// <summary>
        /// Returns a hash code for this instance.
        /// </summary>
        public static int GetHashCode(lQuaternion q) => q.GetHashCode();
        
        /// <summary>
        /// Returns a bVector4 from component-wise application of Equal (lhs == rhs).
        /// </summary>
        public static bVector4 Equal(lQuaternion lhs, lQuaternion rhs) => lQuaternion.Equal(lhs, rhs);
        
        /// <summary>
        /// Returns a bVector4 from component-wise application of NotEqual (lhs != rhs).
        /// </summary>
        public static bVector4 NotEqual(lQuaternion lhs, lQuaternion rhs) => lQuaternion.NotEqual(lhs, rhs);
        
        /// <summary>
        /// Returns a bVector4 from component-wise application of GreaterThan (lhs &gt; rhs).
        /// </summary>
        public static bVector4 GreaterThan(lQuaternion lhs, lQuaternion rhs) => lQuaternion.GreaterThan(lhs, rhs);
        
        /// <summary>
        /// Returns a bVector4 from component-wise application of GreaterThanEqual (lhs &gt;= rhs).
        /// </summary>
        public static bVector4 GreaterThanEqual(lQuaternion lhs, lQuaternion rhs) => lQuaternion.GreaterThanEqual(lhs, rhs);
        
        /// <summary>
        /// Returns a bVector4 from component-wise application of LesserThan (lhs &lt; rhs).
        /// </summary>
        public static bVector4 LesserThan(lQuaternion lhs, lQuaternion rhs) => lQuaternion.LesserThan(lhs, rhs);
        
        /// <summary>
        /// Returns a bVector4 from component-wise application of LesserThanEqual (lhs &lt;= rhs).
        /// </summary>
        public static bVector4 LesserThanEqual(lQuaternion lhs, lQuaternion rhs) => lQuaternion.LesserThanEqual(lhs, rhs);
        
        /// <summary>
        /// Returns the inner product (dot product, scalar product) of the two Quaternionernions.
        /// </summary>
        public static long Dot(lQuaternion lhs, lQuaternion rhs) => lQuaternion.Dot(lhs, rhs);
        
        /// <summary>
        /// Returns the euclidean length of this Quaternionernion.
        /// </summary>
        public static double Length(lQuaternion q) => q.Length;
        
        /// <summary>
        /// Returns the squared euclidean length of this Quaternionernion.
        /// </summary>
        public static long LengthSqr(lQuaternion q) => q.LengthSqr;
        
        /// <summary>
        /// Returns the conjugated Quaternionernion
        /// </summary>
        public static lQuaternion Conjugate(lQuaternion q) => q.Conjugate;
        
        /// <summary>
        /// Returns the inverse Quaternionernion
        /// </summary>
        public static lQuaternion Inverse(lQuaternion q) => q.Inverse;
        
        /// <summary>
        /// Returns the cross product between two Quaternionernions.
        /// </summary>
        public static lQuaternion Cross(lQuaternion q1, lQuaternion q2) => lQuaternion.Cross(q1, q2);
        
        /// <summary>
        /// Returns a lQuaternion from component-wise application of Lerp (min * (1-a) + max * a).
        /// </summary>
        public static lQuaternion Lerp(lQuaternion min, lQuaternion max, lQuaternion a) => lQuaternion.Lerp(min, max, a);

    }
}
