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
        /// Creates a 2D array with all values (address: Values[x, y])
        /// </summary>
        public static Half[,] Values(hMatrix4 m) => m.Values;
        
        /// <summary>
        /// Creates a 1D array with all values (internal order)
        /// </summary>
        public static Half[] Values1D(hMatrix4 m) => m.Values1D;
        
        /// <summary>
        /// Creates a Quaternionernion from the rotational part of this Matrix.
        /// </summary>
        public static hQuaternion ToQuaternion(hMatrix4 m) => m.ToQuaternion;
        
        /// <summary>
        /// Returns an enumerator that iterates through all fields.
        /// </summary>
        public static IEnumerator<Half> GetEnumerator(hMatrix4 m) => m.GetEnumerator();

    }
}
