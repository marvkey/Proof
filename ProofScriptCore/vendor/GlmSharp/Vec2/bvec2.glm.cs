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
        /// Returns an object that can be used for arbitrary swizzling (e.g. swizzle.zy)
        /// </summary>
        public static swizzle_bVector2 swizzle(bVector2 v) => v.swizzle;
        
        /// <summary>
        /// Returns an array with all values
        /// </summary>
        public static bool[] Values(bVector2 v) => v.Values;
        
        /// <summary>
        /// Returns an enumerator that iterates through all components.
        /// </summary>
        public static IEnumerator<bool> GetEnumerator(bVector2 v) => v.GetEnumerator();
        
        /// <summary>
        /// Returns a string representation of this Vector using ', ' as a seperator.
        /// </summary>
        public static string ToString(bVector2 v) => v.ToString();
        
        /// <summary>
        /// Returns a string representation of this Vector using a provided seperator.
        /// </summary>
        public static string ToString(bVector2 v, string sep) => v.ToString(sep);
        
        /// <summary>
        /// Returns a string representation of this Vector using a provided seperator and a forMatrix provider for each component.
        /// </summary>
        public static string ToString(bVector2 v, string sep, IFormatProvider provider) => v.ToString(sep, provider);
        
        /// <summary>
        /// Returns the number of components (2).
        /// </summary>
        public static int Count(bVector2 v) => v.Count;
        
        /// <summary>
        /// Returns true iff this equals rhs component-wise.
        /// </summary>
        public static bool Equals(bVector2 v, bVector2 rhs) => v.Equals(rhs);
        
        /// <summary>
        /// Returns true iff this equals rhs type- and component-wise.
        /// </summary>
        public static bool Equals(bVector2 v, object obj) => v.Equals(obj);
        
        /// <summary>
        /// Returns a hash code for this instance.
        /// </summary>
        public static int GetHashCode(bVector2 v) => v.GetHashCode();
        
        /// <summary>
        /// Returns a bVector2 from component-wise application of Equal (lhs == rhs).
        /// </summary>
        public static bVector2 Equal(bVector2 lhs, bVector2 rhs) => bVector2.Equal(lhs, rhs);
        
        /// <summary>
        /// Returns a bVector2 from component-wise application of NotEqual (lhs != rhs).
        /// </summary>
        public static bVector2 NotEqual(bVector2 lhs, bVector2 rhs) => bVector2.NotEqual(lhs, rhs);
        
        /// <summary>
        /// Returns a bVector2 from component-wise application of Not (!v).
        /// </summary>
        public static bVector2 Not(bVector2 v) => bVector2.Not(v);
        
        /// <summary>
        /// Returns a bVector2 from component-wise application of And (lhs &amp;&amp; rhs).
        /// </summary>
        public static bVector2 And(bVector2 lhs, bVector2 rhs) => bVector2.And(lhs, rhs);
        
        /// <summary>
        /// Returns a bVector2 from component-wise application of Nand (!(lhs &amp;&amp; rhs)).
        /// </summary>
        public static bVector2 Nand(bVector2 lhs, bVector2 rhs) => bVector2.Nand(lhs, rhs);
        
        /// <summary>
        /// Returns a bVector2 from component-wise application of Or (lhs || rhs).
        /// </summary>
        public static bVector2 Or(bVector2 lhs, bVector2 rhs) => bVector2.Or(lhs, rhs);
        
        /// <summary>
        /// Returns a bVector2 from component-wise application of Nor (!(lhs || rhs)).
        /// </summary>
        public static bVector2 Nor(bVector2 lhs, bVector2 rhs) => bVector2.Nor(lhs, rhs);
        
        /// <summary>
        /// Returns a bVector2 from component-wise application of Xor (lhs != rhs).
        /// </summary>
        public static bVector2 Xor(bVector2 lhs, bVector2 rhs) => bVector2.Xor(lhs, rhs);
        
        /// <summary>
        /// Returns a bVector2 from component-wise application of Xnor (lhs == rhs).
        /// </summary>
        public static bVector2 Xnor(bVector2 lhs, bVector2 rhs) => bVector2.Xnor(lhs, rhs);
        
        /// <summary>
        /// Returns the minimal component of this Vector.
        /// </summary>
        public static bool MinElement(bVector2 v) => v.MinElement;
        
        /// <summary>
        /// Returns the maximal component of this Vector.
        /// </summary>
        public static bool MaxElement(bVector2 v) => v.MaxElement;
        
        /// <summary>
        /// Returns true if all component are true.
        /// </summary>
        public static bool All(bVector2 v) => v.All;
        
        /// <summary>
        /// Returns true if any component is true.
        /// </summary>
        public static bool Any(bVector2 v) => v.Any;

    }
}
