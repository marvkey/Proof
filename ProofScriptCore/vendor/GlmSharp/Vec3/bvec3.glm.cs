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
        public static swizzle_bVector3 swizzle(bVector3 v) => v.swizzle;
        
        /// <summary>
        /// Returns an array with all values
        /// </summary>
        public static bool[] Values(bVector3 v) => v.Values;
        
        /// <summary>
        /// Returns an enumerator that iterates through all components.
        /// </summary>
        public static IEnumerator<bool> GetEnumerator(bVector3 v) => v.GetEnumerator();
        
        /// <summary>
        /// Returns a string representation of this Vectortor using ', ' as a seperator.
        /// </summary>
        public static string ToString(bVector3 v) => v.ToString();
        
        /// <summary>
        /// Returns a string representation of this Vectortor using a provided seperator.
        /// </summary>
        public static string ToString(bVector3 v, string sep) => v.ToString(sep);
        
        /// <summary>
        /// Returns a string representation of this Vectortor using a provided seperator and a forMatrix provider for each component.
        /// </summary>
        public static string ToString(bVector3 v, string sep, IFormatProvider provider) => v.ToString(sep, provider);
        
        /// <summary>
        /// Returns the number of components (3).
        /// </summary>
        public static int Count(bVector3 v) => v.Count;
        
        /// <summary>
        /// Returns true iff this equals rhs component-wise.
        /// </summary>
        public static bool Equals(bVector3 v, bVector3 rhs) => v.Equals(rhs);
        
        /// <summary>
        /// Returns true iff this equals rhs type- and component-wise.
        /// </summary>
        public static bool Equals(bVector3 v, object obj) => v.Equals(obj);
        
        /// <summary>
        /// Returns a hash code for this instance.
        /// </summary>
        public static int GetHashCode(bVector3 v) => v.GetHashCode();
        
        /// <summary>
        /// Returns a bVector3 from component-wise application of Equal (lhs == rhs).
        /// </summary>
        public static bVector3 Equal(bVector3 lhs, bVector3 rhs) => bVector3.Equal(lhs, rhs);
        
        /// <summary>
        /// Returns a bVector3 from component-wise application of Equal (lhs == rhs).
        /// </summary>
        public static bool Equal(bool lhs, bool rhs) => lhs == rhs;
        
        /// <summary>
        /// Returns a bVector3 from component-wise application of NotEqual (lhs != rhs).
        /// </summary>
        public static bVector3 NotEqual(bVector3 lhs, bVector3 rhs) => bVector3.NotEqual(lhs, rhs);
        
        /// <summary>
        /// Returns a bVector3 from component-wise application of NotEqual (lhs != rhs).
        /// </summary>
        public static bool NotEqual(bool lhs, bool rhs) => lhs != rhs;
        
        /// <summary>
        /// Returns a bVector3 from component-wise application of Not (!v).
        /// </summary>
        public static bVector3 Not(bVector3 v) => bVector3.Not(v);
        
        /// <summary>
        /// Returns a bVector3 from component-wise application of Not (!v).
        /// </summary>
        public static bool Not(bool v) => !v;
        
        /// <summary>
        /// Returns a bVector3 from component-wise application of And (lhs &amp;&amp; rhs).
        /// </summary>
        public static bVector3 And(bVector3 lhs, bVector3 rhs) => bVector3.And(lhs, rhs);
        
        /// <summary>
        /// Returns a bVector3 from component-wise application of And (lhs &amp;&amp; rhs).
        /// </summary>
        public static bool And(bool lhs, bool rhs) => lhs && rhs;
        
        /// <summary>
        /// Returns a bVector3 from component-wise application of Nand (!(lhs &amp;&amp; rhs)).
        /// </summary>
        public static bVector3 Nand(bVector3 lhs, bVector3 rhs) => bVector3.Nand(lhs, rhs);
        
        /// <summary>
        /// Returns a bVector3 from component-wise application of Nand (!(lhs &amp;&amp; rhs)).
        /// </summary>
        public static bool Nand(bool lhs, bool rhs) => !(lhs && rhs);
        
        /// <summary>
        /// Returns a bVector3 from component-wise application of Or (lhs || rhs).
        /// </summary>
        public static bVector3 Or(bVector3 lhs, bVector3 rhs) => bVector3.Or(lhs, rhs);
        
        /// <summary>
        /// Returns a bVector3 from component-wise application of Or (lhs || rhs).
        /// </summary>
        public static bool Or(bool lhs, bool rhs) => lhs || rhs;
        
        /// <summary>
        /// Returns a bVector3 from component-wise application of Nor (!(lhs || rhs)).
        /// </summary>
        public static bVector3 Nor(bVector3 lhs, bVector3 rhs) => bVector3.Nor(lhs, rhs);
        
        /// <summary>
        /// Returns a bVector3 from component-wise application of Nor (!(lhs || rhs)).
        /// </summary>
        public static bool Nor(bool lhs, bool rhs) => !(lhs || rhs);
        
        /// <summary>
        /// Returns a bVector3 from component-wise application of Xor (lhs != rhs).
        /// </summary>
        public static bVector3 Xor(bVector3 lhs, bVector3 rhs) => bVector3.Xor(lhs, rhs);
        
        /// <summary>
        /// Returns a bVector3 from component-wise application of Xor (lhs != rhs).
        /// </summary>
        public static bool Xor(bool lhs, bool rhs) => lhs != rhs;
        
        /// <summary>
        /// Returns a bVector3 from component-wise application of Xnor (lhs == rhs).
        /// </summary>
        public static bVector3 Xnor(bVector3 lhs, bVector3 rhs) => bVector3.Xnor(lhs, rhs);
        
        /// <summary>
        /// Returns a bVector3 from component-wise application of Xnor (lhs == rhs).
        /// </summary>
        public static bool Xnor(bool lhs, bool rhs) => lhs == rhs;
        
        /// <summary>
        /// Returns the minimal component of this Vectortor.
        /// </summary>
        public static bool MinElement(bVector3 v) => v.MinElement;
        
        /// <summary>
        /// Returns the maximal component of this Vectortor.
        /// </summary>
        public static bool MaxElement(bVector3 v) => v.MaxElement;
        
        /// <summary>
        /// Returns true if all component are true.
        /// </summary>
        public static bool All(bVector3 v) => v.All;
        
        /// <summary>
        /// Returns true if any component is true.
        /// </summary>
        public static bool Any(bVector3 v) => v.Any;

    }
}
