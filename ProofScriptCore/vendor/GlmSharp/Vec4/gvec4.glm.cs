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
        public static swizzle_gVector4<T> swizzle<T>(gVector4<T> v) => v.swizzle;
        
        /// <summary>
        /// Returns an array with all values
        /// </summary>
        public static T[] Values<T>(gVector4<T> v) => v.Values;
        
        /// <summary>
        /// Returns an enumerator that iterates through all components.
        /// </summary>
        public static IEnumerator<T> GetEnumerator<T>(gVector4<T> v) => v.GetEnumerator();
        
        /// <summary>
        /// Returns a string representation of this Vectortor using ', ' as a seperator.
        /// </summary>
        public static string ToString<T>(gVector4<T> v) => v.ToString();
        
        /// <summary>
        /// Returns a string representation of this Vectortor using a provided seperator.
        /// </summary>
        public static string ToString<T>(gVector4<T> v, string sep) => v.ToString(sep);
        
        /// <summary>
        /// Returns the number of components (4).
        /// </summary>
        public static int Count<T>(gVector4<T> v) => v.Count;
        
        /// <summary>
        /// Returns true iff this equals rhs component-wise.
        /// </summary>
        public static bool Equals<T>(gVector4<T> v, gVector4<T> rhs) => v.Equals(rhs);
        
        /// <summary>
        /// Returns true iff this equals rhs type- and component-wise.
        /// </summary>
        public static bool Equals<T>(gVector4<T> v, object obj) => v.Equals(obj);
        
        /// <summary>
        /// Returns a hash code for this instance.
        /// </summary>
        public static int GetHashCode<T>(gVector4<T> v) => v.GetHashCode();
        
        /// <summary>
        /// Returns a bVector4 from component-wise application of Equal (EqualityComparer&lt;T&gt;.Default.Equals(lhs, rhs)).
        /// </summary>
        public static bVector4 Equal<T>(gVector4<T> lhs, gVector4<T> rhs) => gVector4<T>.Equal(lhs, rhs);
        
        /// <summary>
        /// Returns a bVector4 from component-wise application of NotEqual (!EqualityComparer&lt;T&gt;.Default.Equals(lhs, rhs)).
        /// </summary>
        public static bVector4 NotEqual<T>(gVector4<T> lhs, gVector4<T> rhs) => gVector4<T>.NotEqual(lhs, rhs);

    }
}
