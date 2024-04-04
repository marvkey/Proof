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
        public static bool[] Values(bQuaternion q) => q.Values;
        
        /// <summary>
        /// Returns an enumerator that iterates through all components.
        /// </summary>
        public static IEnumerator<bool> GetEnumerator(bQuaternion q) => q.GetEnumerator();
        
        /// <summary>
        /// Returns a string representation of this Quaternionernion using ', ' as a seperator.
        /// </summary>
        public static string ToString(bQuaternion q) => q.ToString();
        
        /// <summary>
        /// Returns a string representation of this Quaternionernion using a provided seperator.
        /// </summary>
        public static string ToString(bQuaternion q, string sep) => q.ToString(sep);
        
        /// <summary>
        /// Returns a string representation of this Quaternionernion using a provided seperator and a forMatrix provider for each component.
        /// </summary>
        public static string ToString(bQuaternion q, string sep, IFormatProvider provider) => q.ToString(sep, provider);
        
        /// <summary>
        /// Returns the number of components (4).
        /// </summary>
        public static int Count(bQuaternion q) => q.Count;
        
        /// <summary>
        /// Returns true iff this equals rhs component-wise.
        /// </summary>
        public static bool Equals(bQuaternion q, bQuaternion rhs) => q.Equals(rhs);
        
        /// <summary>
        /// Returns true iff this equals rhs type- and component-wise.
        /// </summary>
        public static bool Equals(bQuaternion q, object obj) => q.Equals(obj);
        
        /// <summary>
        /// Returns a hash code for this instance.
        /// </summary>
        public static int GetHashCode(bQuaternion q) => q.GetHashCode();
        
        /// <summary>
        /// Returns a bVector4 from component-wise application of Equal (lhs == rhs).
        /// </summary>
        public static bVector4 Equal(bQuaternion lhs, bQuaternion rhs) => bQuaternion.Equal(lhs, rhs);
        
        /// <summary>
        /// Returns a bVector4 from component-wise application of NotEqual (lhs != rhs).
        /// </summary>
        public static bVector4 NotEqual(bQuaternion lhs, bQuaternion rhs) => bQuaternion.NotEqual(lhs, rhs);
        
        /// <summary>
        /// Returns a bVector4 from component-wise application of Not (!v).
        /// </summary>
        public static bVector4 Not(bQuaternion v) => bQuaternion.Not(v);
        
        /// <summary>
        /// Returns a bQuaternion from component-wise application of And (lhs &amp;&amp; rhs).
        /// </summary>
        public static bQuaternion And(bQuaternion lhs, bQuaternion rhs) => bQuaternion.And(lhs, rhs);
        
        /// <summary>
        /// Returns a bQuaternion from component-wise application of Nand (!(lhs &amp;&amp; rhs)).
        /// </summary>
        public static bQuaternion Nand(bQuaternion lhs, bQuaternion rhs) => bQuaternion.Nand(lhs, rhs);
        
        /// <summary>
        /// Returns a bQuaternion from component-wise application of Or (lhs || rhs).
        /// </summary>
        public static bQuaternion Or(bQuaternion lhs, bQuaternion rhs) => bQuaternion.Or(lhs, rhs);
        
        /// <summary>
        /// Returns a bQuaternion from component-wise application of Nor (!(lhs || rhs)).
        /// </summary>
        public static bQuaternion Nor(bQuaternion lhs, bQuaternion rhs) => bQuaternion.Nor(lhs, rhs);
        
        /// <summary>
        /// Returns a bQuaternion from component-wise application of Xor (lhs != rhs).
        /// </summary>
        public static bQuaternion Xor(bQuaternion lhs, bQuaternion rhs) => bQuaternion.Xor(lhs, rhs);
        
        /// <summary>
        /// Returns a bQuaternion from component-wise application of Xnor (lhs == rhs).
        /// </summary>
        public static bQuaternion Xnor(bQuaternion lhs, bQuaternion rhs) => bQuaternion.Xnor(lhs, rhs);
        
        /// <summary>
        /// Returns the minimal component of this Quaternionernion.
        /// </summary>
        public static bool MinElement(bQuaternion q) => q.MinElement;
        
        /// <summary>
        /// Returns the maximal component of this Quaternionernion.
        /// </summary>
        public static bool MaxElement(bQuaternion q) => q.MaxElement;
        
        /// <summary>
        /// Returns true if all component are true.
        /// </summary>
        public static bool All(bQuaternion q) => q.All;
        
        /// <summary>
        /// Returns true if any component is true.
        /// </summary>
        public static bool Any(bQuaternion q) => q.Any;

    }
}
