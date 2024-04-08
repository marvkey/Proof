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
    /// A Vector of type T with 2 components.
    /// </summary>
    [RegisterCoreClassStruct]
    
    [StructLayout(LayoutKind.Sequential)]
    public struct gVector2<T> : IReadOnlyList<T>, IEquatable<gVector2<T>>
    {

        #region Fields
        
        /// <summary>
        /// x-component
        /// </summary>
        
        public T x;
        
        /// <summary>
        /// y-component
        /// </summary>
        
        public T y;

        #endregion


        #region Constructors
        
        /// <summary>
        /// Component-wise constructor
        /// </summary>
        public gVector2(T x, T y)
        {
            this.x = x;
            this.y = y;
        }
        
        /// <summary>
        /// all-same-value constructor
        /// </summary>
        public gVector2(T v)
        {
            this.x = v;
            this.y = v;
        }
        
        /// <summary>
        /// from-Vector constructor
        /// </summary>
        public gVector2(gVector2<T> v)
        {
            this.x = v.x;
            this.y = v.y;
        }
        
        /// <summary>
        /// from-Vector constructor (additional fields are truncated)
        /// </summary>
        public gVector2(gVector3<T> v)
        {
            this.x = v.x;
            this.y = v.y;
        }
        
        /// <summary>
        /// from-Vector constructor (additional fields are truncated)
        /// </summary>
        public gVector2(gVector4<T> v)
        {
            this.x = v.x;
            this.y = v.y;
        }
        
        /// <summary>
        /// From-array/list constructor (superfluous values are ignored, missing values are zero-filled).
        /// </summary>
        public gVector2(IReadOnlyList<T> v)
        {
            var c = v.Count;
            this.x = c < 0 ? default(T) : v[0];
            this.y = c < 1 ? default(T) : v[1];
        }
        
        /// <summary>
        /// Generic from-array constructor (superfluous values are ignored, missing values are zero-filled).
        /// </summary>
        public gVector2(Object[] v)
        {
            var c = v.Length;
            this.x = c < 0 ? default(T) : (T)v[0];
            this.y = c < 1 ? default(T) : (T)v[1];
        }
        
        /// <summary>
        /// From-array constructor (superfluous values are ignored, missing values are zero-filled).
        /// </summary>
        public gVector2(T[] v)
        {
            var c = v.Length;
            this.x = c < 0 ? default(T) : v[0];
            this.y = c < 1 ? default(T) : v[1];
        }
        
        /// <summary>
        /// From-array constructor with base index (superfluous values are ignored, missing values are zero-filled).
        /// </summary>
        public gVector2(T[] v, int startIndex)
        {
            var c = v.Length;
            this.x = c + startIndex < 0 ? default(T) : v[0 + startIndex];
            this.y = c + startIndex < 1 ? default(T) : v[1 + startIndex];
        }
        
        /// <summary>
        /// From-IEnumerable constructor (superfluous values are ignored, missing values are zero-filled).
        /// </summary>
        public gVector2(IEnumerable<T> v)
            : this(v.ToArray())
        {
        }

        #endregion


        #region Explicit Operators
        
        /// <summary>
        /// Explicitly converts this to a gVector3. (Higher components are zeroed)
        /// </summary>
        public static explicit operator gVector3<T>(gVector2<T> v) => new gVector3<T>((T)v.x, (T)v.y, default(T));
        
        /// <summary>
        /// Explicitly converts this to a gVector4. (Higher components are zeroed)
        /// </summary>
        public static explicit operator gVector4<T>(gVector2<T> v) => new gVector4<T>((T)v.x, (T)v.y, default(T), default(T));
        
        /// <summary>
        /// Explicitly converts this to a T array.
        /// </summary>
        public static explicit operator T[](gVector2<T> v) => new [] { v.x, v.y };
        
        /// <summary>
        /// Explicitly converts this to a generic object array.
        /// </summary>
        public static explicit operator Object[](gVector2<T> v) => new Object[] { v.x, v.y };

        #endregion


        #region Indexer
        
        /// <summary>
        /// Gets/Sets a specific indexed component (a bit slower than direct access).
        /// </summary>
        public T this[int index]
        {
            get
            {
                switch (index)
                {
                    case 0: return x;
                    case 1: return y;
                    default: throw new ArgumentOutOfRangeException("index");
                }
            }
            set
            {
                switch (index)
                {
                    case 0: x = value; break;
                    case 1: y = value; break;
                    default: throw new ArgumentOutOfRangeException("index");
                }
            }
        }

        #endregion


        #region Properties
        
        /// <summary>
        /// Returns an object that can be used for arbitrary swizzling (e.g. swizzle.zy)
        /// </summary>
        public swizzle_gVector2<T> swizzle => new swizzle_gVector2<T>(x, y);
        
        /// <summary>
        /// Gets or sets the specified subset of components. For more advanced (read-only) swizzling, use the .swizzle property.
        /// </summary>
        public gVector2<T> xy
        {
            get
            {
                return new gVector2<T>(x, y);
            }
            set
            {
                x = value.x;
                y = value.y;
            }
        }
        
        /// <summary>
        /// Gets or sets the specified subset of components. For more advanced (read-only) swizzling, use the .swizzle property.
        /// </summary>
        public gVector2<T> rg
        {
            get
            {
                return new gVector2<T>(x, y);
            }
            set
            {
                x = value.x;
                y = value.y;
            }
        }
        
        /// <summary>
        /// Gets or sets the specified RGBA component. For more advanced (read-only) swizzling, use the .swizzle property.
        /// </summary>
        public T r
        {
            get
            {
                return x;
            }
            set
            {
                x = value;
            }
        }
        
        /// <summary>
        /// Gets or sets the specified RGBA component. For more advanced (read-only) swizzling, use the .swizzle property.
        /// </summary>
        public T g
        {
            get
            {
                return y;
            }
            set
            {
                y = value;
            }
        }
        
        /// <summary>
        /// Returns an array with all values
        /// </summary>
        public T[] Values => new[] { x, y };
        
        /// <summary>
        /// Returns the number of components (2).
        /// </summary>
        public int Count => 2;

        #endregion


        #region Static Properties
        
        /// <summary>
        /// Predefined all-zero Vector
        /// </summary>
        public static gVector2<T> Zero { get; } = new gVector2<T>(default(T), default(T));

        #endregion


        #region Operators
        
        /// <summary>
        /// Returns true iff this equals rhs component-wise.
        /// </summary>
        public static bool operator==(gVector2<T> lhs, gVector2<T> rhs) => lhs.Equals(rhs);
        
        /// <summary>
        /// Returns true iff this does not equal rhs (component-wise).
        /// </summary>
        public static bool operator!=(gVector2<T> lhs, gVector2<T> rhs) => !lhs.Equals(rhs);

        #endregion


        #region Functions
        
        /// <summary>
        /// Returns an enumerator that iterates through all components.
        /// </summary>
        public IEnumerator<T> GetEnumerator()
        {
            yield return x;
            yield return y;
        }
        
        /// <summary>
        /// Returns an enumerator that iterates through all components.
        /// </summary>
        IEnumerator IEnumerable.GetEnumerator() => GetEnumerator();
        
        /// <summary>
        /// Returns a string representation of this Vector using ', ' as a seperator.
        /// </summary>
        public override string ToString() => ToString(", ");
        
        /// <summary>
        /// Returns a string representation of this Vector using a provided seperator.
        /// </summary>
        public string ToString(string sep) => (x + sep + y);
        
        /// <summary>
        /// Returns true iff this equals rhs component-wise.
        /// </summary>
        public bool Equals(gVector2<T> rhs) => (EqualityComparer<T>.Default.Equals(x, rhs.x) && EqualityComparer<T>.Default.Equals(y, rhs.y));
        
        /// <summary>
        /// Returns true iff this equals rhs type- and component-wise.
        /// </summary>
        public override bool Equals(object obj)
        {
            if (ReferenceEquals(null, obj)) return false;
            return obj is gVector2<T> && Equals((gVector2<T>) obj);
        }
        
        /// <summary>
        /// Returns a hash code for this instance.
        /// </summary>
        public override int GetHashCode()
        {
            unchecked
            {
                return ((EqualityComparer<T>.Default.GetHashCode(x)) * 397) ^ EqualityComparer<T>.Default.GetHashCode(y);
            }
        }

        #endregion


        #region Component-Wise Static Functions
        
        /// <summary>
        /// Returns a bVector2 from component-wise application of Equal (EqualityComparer&lt;T&gt;.Default.Equals(lhs, rhs)).
        /// </summary>
        public static bVector2 Equal(gVector2<T> lhs, gVector2<T> rhs) => new bVector2(EqualityComparer<T>.Default.Equals(lhs.x, rhs.x), EqualityComparer<T>.Default.Equals(lhs.y, rhs.y));
        
        /// <summary>
        /// Returns a bVector2 from component-wise application of Equal (EqualityComparer&lt;T&gt;.Default.Equals(lhs, rhs)).
        /// </summary>
        public static bVector2 Equal(gVector2<T> lhs, T rhs) => new bVector2(EqualityComparer<T>.Default.Equals(lhs.x, rhs), EqualityComparer<T>.Default.Equals(lhs.y, rhs));
        
        /// <summary>
        /// Returns a bVector2 from component-wise application of Equal (EqualityComparer&lt;T&gt;.Default.Equals(lhs, rhs)).
        /// </summary>
        public static bVector2 Equal(T lhs, gVector2<T> rhs) => new bVector2(EqualityComparer<T>.Default.Equals(lhs, rhs.x), EqualityComparer<T>.Default.Equals(lhs, rhs.y));
        
        /// <summary>
        /// Returns a bVector from the application of Equal (EqualityComparer&lt;T&gt;.Default.Equals(lhs, rhs)).
        /// </summary>
        public static bVector2 Equal(T lhs, T rhs) => new bVector2(EqualityComparer<T>.Default.Equals(lhs, rhs));
        
        /// <summary>
        /// Returns a bVector2 from component-wise application of NotEqual (!EqualityComparer&lt;T&gt;.Default.Equals(lhs, rhs)).
        /// </summary>
        public static bVector2 NotEqual(gVector2<T> lhs, gVector2<T> rhs) => new bVector2(!EqualityComparer<T>.Default.Equals(lhs.x, rhs.x), !EqualityComparer<T>.Default.Equals(lhs.y, rhs.y));
        
        /// <summary>
        /// Returns a bVector2 from component-wise application of NotEqual (!EqualityComparer&lt;T&gt;.Default.Equals(lhs, rhs)).
        /// </summary>
        public static bVector2 NotEqual(gVector2<T> lhs, T rhs) => new bVector2(!EqualityComparer<T>.Default.Equals(lhs.x, rhs), !EqualityComparer<T>.Default.Equals(lhs.y, rhs));
        
        /// <summary>
        /// Returns a bVector2 from component-wise application of NotEqual (!EqualityComparer&lt;T&gt;.Default.Equals(lhs, rhs)).
        /// </summary>
        public static bVector2 NotEqual(T lhs, gVector2<T> rhs) => new bVector2(!EqualityComparer<T>.Default.Equals(lhs, rhs.x), !EqualityComparer<T>.Default.Equals(lhs, rhs.y));
        
        /// <summary>
        /// Returns a bVector from the application of NotEqual (!EqualityComparer&lt;T&gt;.Default.Equals(lhs, rhs)).
        /// </summary>
        public static bVector2 NotEqual(T lhs, T rhs) => new bVector2(!EqualityComparer<T>.Default.Equals(lhs, rhs));

        #endregion

    }
}
