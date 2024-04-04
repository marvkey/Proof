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
    /// 
    /*
    public static partial class glm
    {
        
        /// <summary>
        /// Returns an array with all values
        /// </summary>
        public static Complex[] Values(cQuaternion q) => q.Values;
        
        /// <summary>
        /// Returns an enumerator that iterates through all components.
        /// </summary>
        public static IEnumerator<Complex> GetEnumerator(cQuaternion q) => q.GetEnumerator();
        
        /// <summary>
        /// Returns a string representation of this Quaternionernion using ', ' as a seperator.
        /// </summary>
        public static string ToString(cQuaternion q) => q.ToString();
        
        /// <summary>
        /// Returns a string representation of this Quaternionernion using a provided seperator.
        /// </summary>
        public static string ToString(cQuaternion q, string sep) => q.ToString(sep);
        
        /// <summary>
        /// Returns a string representation of this Quaternionernion using a provided seperator and a forMatrix provider for each component.
        /// </summary>
        public static string ToString(cQuaternion q, string sep, IFormatProvider provider) => q.ToString(sep, provider);
        
        /// <summary>
        /// Returns a string representation of this Quaternionernion using a provided seperator and a forMatrix for each component.
        /// </summary>
        public static string ToString(cQuaternion q, string sep, string forMatrix) => q.ToString(sep, forMatrix);
        
        /// <summary>
        /// Returns a string representation of this Quaternionernion using a provided seperator and a forMatrix and forMatrix provider for each component.
        /// </summary>
        public static string ToString(cQuaternion q, string sep, string forMatrix, IFormatProvider provider) => q.ToString(sep, forMatrix, provider);
        
        /// <summary>
        /// Returns the number of components (4).
        /// </summary>
        public static int Count(cQuaternion q) => q.Count;
        
        /// <summary>
        /// Returns true iff this equals rhs component-wise.
        /// </summary>
        public static bool Equals(cQuaternion q, cQuaternion rhs) => q.Equals(rhs);
        
        /// <summary>
        /// Returns true iff this equals rhs type- and component-wise.
        /// </summary>
        public static bool Equals(cQuaternion q, object obj) => q.Equals(obj);
        
        /// <summary>
        /// Returns a hash code for this instance.
        /// </summary>
        public static int GetHashCode(cQuaternion q) => q.GetHashCode();
        
        /// <summary>
        /// Returns a bVector4 from component-wise application of Equal (lhs == rhs).
        /// </summary>
        public static bVector4 Equal(cQuaternion lhs, cQuaternion rhs) => cQuaternion.Equal(lhs, rhs);
        
        /// <summary>
        /// Returns a bVector4 from component-wise application of NotEqual (lhs != rhs).
        /// </summary>
        public static bVector4 NotEqual(cQuaternion lhs, cQuaternion rhs) => cQuaternion.NotEqual(lhs, rhs);
        
        /// <summary>
        /// Returns the inner product (dot product, scalar product) of the two Quaternionernions.
        /// </summary>
        public static Complex Dot(cQuaternion lhs, cQuaternion rhs) => cQuaternion.Dot(lhs, rhs);
        
        /// <summary>
        /// Returns the euclidean length of this Quaternionernion.
        /// </summary>
        public static double Length(cQuaternion q) => q.Length;
        
        /// <summary>
        /// Returns the squared euclidean length of this Quaternionernion.
        /// </summary>
        public static Complex LengthSqr(cQuaternion q) => q.LengthSqr;
        
        /// <summary>
        /// Returns a copy of this Quaternionernion with length one (undefined if this has zero length).
        /// </summary>
        public static cQuaternion Normalized(cQuaternion q) => q.Normalized;
        
        /// <summary>
        /// Returns a copy of this Quaternionernion with length one (returns zero if length is zero).
        /// </summary>
        public static cQuaternion NormalizedSafe(cQuaternion q) => q.NormalizedSafe;
        
        /// <summary>
        /// Returns the conjugated Quaternionernion
        /// </summary>
        public static cQuaternion Conjugate(cQuaternion q) => q.Conjugate;
        
        /// <summary>
        /// Returns the inverse Quaternionernion
        /// </summary>
        public static cQuaternion Inverse(cQuaternion q) => q.Inverse;
        
        /// <summary>
        /// Returns the cross product between two Quaternionernions.
        /// </summary>
        public static cQuaternion Cross(cQuaternion q1, cQuaternion q2) => cQuaternion.Cross(q1, q2);
        
        /// <summary>
        /// Returns a cQuaternion from component-wise application of Lerp (min * (1-a) + max * a).
        /// </summary>
        public static cQuaternion Lerp(cQuaternion min, cQuaternion max, cQuaternion a) => cQuaternion.Lerp(min, max, a);

    }
    */
}
