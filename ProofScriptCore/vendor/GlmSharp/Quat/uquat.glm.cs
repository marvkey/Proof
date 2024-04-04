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
        public static uint[] Values(uQuaternion q) => q.Values;
        
        /// <summary>
        /// Returns an enumerator that iterates through all components.
        /// </summary>
        public static IEnumerator<uint> GetEnumerator(uQuaternion q) => q.GetEnumerator();
        
        /// <summary>
        /// Returns a string representation of this Quaternionernion using ', ' as a seperator.
        /// </summary>
        public static string ToString(uQuaternion q) => q.ToString();
        
        /// <summary>
        /// Returns a string representation of this Quaternionernion using a provided seperator.
        /// </summary>
        public static string ToString(uQuaternion q, string sep) => q.ToString(sep);
        
        /// <summary>
        /// Returns a string representation of this Quaternionernion using a provided seperator and a forMatrix provider for each component.
        /// </summary>
        public static string ToString(uQuaternion q, string sep, IFormatProvider provider) => q.ToString(sep, provider);
        
        /// <summary>
        /// Returns a string representation of this Quaternionernion using a provided seperator and a forMatrix for each component.
        /// </summary>
        public static string ToString(uQuaternion q, string sep, string forMatrix) => q.ToString(sep, forMatrix);
        
        /// <summary>
        /// Returns a string representation of this Quaternionernion using a provided seperator and a forMatrix and forMatrix provider for each component.
        /// </summary>
        public static string ToString(uQuaternion q, string sep, string forMatrix, IFormatProvider provider) => q.ToString(sep, forMatrix, provider);
        
        /// <summary>
        /// Returns the number of components (4).
        /// </summary>
        public static int Count(uQuaternion q) => q.Count;
        
        /// <summary>
        /// Returns true iff this equals rhs component-wise.
        /// </summary>
        public static bool Equals(uQuaternion q, uQuaternion rhs) => q.Equals(rhs);
        
        /// <summary>
        /// Returns true iff this equals rhs type- and component-wise.
        /// </summary>
        public static bool Equals(uQuaternion q, object obj) => q.Equals(obj);
        
        /// <summary>
        /// Returns a hash code for this instance.
        /// </summary>
        public static int GetHashCode(uQuaternion q) => q.GetHashCode();
        
        /// <summary>
        /// Returns a bVector4 from component-wise application of Equal (lhs == rhs).
        /// </summary>
        public static bVector4 Equal(uQuaternion lhs, uQuaternion rhs) => uQuaternion.Equal(lhs, rhs);
        
        /// <summary>
        /// Returns a bVector4 from component-wise application of NotEqual (lhs != rhs).
        /// </summary>
        public static bVector4 NotEqual(uQuaternion lhs, uQuaternion rhs) => uQuaternion.NotEqual(lhs, rhs);
        
        /// <summary>
        /// Returns a bVector4 from component-wise application of GreaterThan (lhs &gt; rhs).
        /// </summary>
        public static bVector4 GreaterThan(uQuaternion lhs, uQuaternion rhs) => uQuaternion.GreaterThan(lhs, rhs);
        
        /// <summary>
        /// Returns a bVector4 from component-wise application of GreaterThanEqual (lhs &gt;= rhs).
        /// </summary>
        public static bVector4 GreaterThanEqual(uQuaternion lhs, uQuaternion rhs) => uQuaternion.GreaterThanEqual(lhs, rhs);
        
        /// <summary>
        /// Returns a bVector4 from component-wise application of LesserThan (lhs &lt; rhs).
        /// </summary>
        public static bVector4 LesserThan(uQuaternion lhs, uQuaternion rhs) => uQuaternion.LesserThan(lhs, rhs);
        
        /// <summary>
        /// Returns a bVector4 from component-wise application of LesserThanEqual (lhs &lt;= rhs).
        /// </summary>
        public static bVector4 LesserThanEqual(uQuaternion lhs, uQuaternion rhs) => uQuaternion.LesserThanEqual(lhs, rhs);
        
        /// <summary>
        /// Returns the inner product (dot product, scalar product) of the two Quaternionernions.
        /// </summary>
        public static uint Dot(uQuaternion lhs, uQuaternion rhs) => uQuaternion.Dot(lhs, rhs);
        
        /// <summary>
        /// Returns the euclidean length of this Quaternionernion.
        /// </summary>
        public static float Length(uQuaternion q) => q.Length;
        
        /// <summary>
        /// Returns the squared euclidean length of this Quaternionernion.
        /// </summary>
        public static uint LengthSqr(uQuaternion q) => q.LengthSqr;
        
        /// <summary>
        /// Returns the cross product between two Quaternionernions.
        /// </summary>
        public static uQuaternion Cross(uQuaternion q1, uQuaternion q2) => uQuaternion.Cross(q1, q2);
        
        /// <summary>
        /// Returns a uQuaternion from component-wise application of Lerp (min * (1-a) + max * a).
        /// </summary>
        public static uQuaternion Lerp(uQuaternion min, uQuaternion max, uQuaternion a) => uQuaternion.Lerp(min, max, a);

    }
}
