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
    /// A Vectortor of type T with 3 components.
    /// </summary>
    [RegisterCoreClassStruct]
    
    [StructLayout(LayoutKind.Sequential)]
    public struct gVector3<T> : IReadOnlyList<T>, IEquatable<gVector3<T>>
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
        
        /// <summary>
        /// z-component
        /// </summary>
        
        public T z;

        #endregion


        #region Constructors
        
        /// <summary>
        /// Component-wise constructor
        /// </summary>
        public gVector3(T x, T y, T z)
        {
            this.x = x;
            this.y = y;
            this.z = z;
        }
        
        /// <summary>
        /// all-same-value constructor
        /// </summary>
        public gVector3(T v)
        {
            this.x = v;
            this.y = v;
            this.z = v;
        }
        
        /// <summary>
        /// from-Vectortor constructor (empty fields are zero/false)
        /// </summary>
        public gVector3(gVector2<T> v)
        {
            this.x = v.x;
            this.y = v.y;
            this.z = default(T);
        }
        
        /// <summary>
        /// from-Vectortor-and-value constructor
        /// </summary>
        public gVector3(gVector2<T> v, T z)
        {
            this.x = v.x;
            this.y = v.y;
            this.z = z;
        }
        
        /// <summary>
        /// from-Vectortor constructor
        /// </summary>
        public gVector3(gVector3<T> v)
        {
            this.x = v.x;
            this.y = v.y;
            this.z = v.z;
        }
        
        /// <summary>
        /// from-Vectortor constructor (additional fields are truncated)
        /// </summary>
        public gVector3(gVector4<T> v)
        {
            this.x = v.x;
            this.y = v.y;
            this.z = v.z;
        }
        
        /// <summary>
        /// From-array/list constructor (superfluous values are ignored, missing values are zero-filled).
        /// </summary>
        public gVector3(IReadOnlyList<T> v)
        {
            var c = v.Count;
            this.x = c < 0 ? default(T) : v[0];
            this.y = c < 1 ? default(T) : v[1];
            this.z = c < 2 ? default(T) : v[2];
        }
        
        /// <summary>
        /// Generic from-array constructor (superfluous values are ignored, missing values are zero-filled).
        /// </summary>
        public gVector3(Object[] v)
        {
            var c = v.Length;
            this.x = c < 0 ? default(T) : (T)v[0];
            this.y = c < 1 ? default(T) : (T)v[1];
            this.z = c < 2 ? default(T) : (T)v[2];
        }
        
        /// <summary>
        /// From-array constructor (superfluous values are ignored, missing values are zero-filled).
        /// </summary>
        public gVector3(T[] v)
        {
            var c = v.Length;
            this.x = c < 0 ? default(T) : v[0];
            this.y = c < 1 ? default(T) : v[1];
            this.z = c < 2 ? default(T) : v[2];
        }
        
        /// <summary>
        /// From-array constructor with base index (superfluous values are ignored, missing values are zero-filled).
        /// </summary>
        public gVector3(T[] v, int startIndex)
        {
            var c = v.Length;
            this.x = c + startIndex < 0 ? default(T) : v[0 + startIndex];
            this.y = c + startIndex < 1 ? default(T) : v[1 + startIndex];
            this.z = c + startIndex < 2 ? default(T) : v[2 + startIndex];
        }
        
        /// <summary>
        /// From-IEnumerable constructor (superfluous values are ignored, missing values are zero-filled).
        /// </summary>
        public gVector3(IEnumerable<T> v)
            : this(v.ToArray())
        {
        }

        #endregion


        #region Explicit Operators
        
        /// <summary>
        /// Explicitly converts this to a gVector2.
        /// </summary>
        public static explicit operator gVector2<T>(gVector3<T> v) => new gVector2<T>((T)v.x, (T)v.y);
        
        /// <summary>
        /// Explicitly converts this to a gVector4. (Higher components are zeroed)
        /// </summary>
        public static explicit operator gVector4<T>(gVector3<T> v) => new gVector4<T>((T)v.x, (T)v.y, (T)v.z, default(T));
        
        /// <summary>
        /// Explicitly converts this to a T array.
        /// </summary>
        public static explicit operator T[](gVector3<T> v) => new [] { v.x, v.y, v.z };
        
        /// <summary>
        /// Explicitly converts this to a generic object array.
        /// </summary>
        public static explicit operator Object[](gVector3<T> v) => new Object[] { v.x, v.y, v.z };

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
                    case 2: return z;
                    default: throw new ArgumentOutOfRangeException("index");
                }
            }
            set
            {
                switch (index)
                {
                    case 0: x = value; break;
                    case 1: y = value; break;
                    case 2: z = value; break;
                    default: throw new ArgumentOutOfRangeException("index");
                }
            }
        }

        #endregion


        #region Properties
        
        /// <summary>
        /// Returns an object that can be used for arbitrary swizzling (e.g. swizzle.zy)
        /// </summary>
        public swizzle_gVector3<T> swizzle => new swizzle_gVector3<T>(x, y, z);
        
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
        public gVector2<T> xz
        {
            get
            {
                return new gVector2<T>(x, z);
            }
            set
            {
                x = value.x;
                z = value.y;
            }
        }
        
        /// <summary>
        /// Gets or sets the specified subset of components. For more advanced (read-only) swizzling, use the .swizzle property.
        /// </summary>
        public gVector2<T> yz
        {
            get
            {
                return new gVector2<T>(y, z);
            }
            set
            {
                y = value.x;
                z = value.y;
            }
        }
        
        /// <summary>
        /// Gets or sets the specified subset of components. For more advanced (read-only) swizzling, use the .swizzle property.
        /// </summary>
        public gVector3<T> xyz
        {
            get
            {
                return new gVector3<T>(x, y, z);
            }
            set
            {
                x = value.x;
                y = value.y;
                z = value.z;
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
        /// Gets or sets the specified subset of components. For more advanced (read-only) swizzling, use the .swizzle property.
        /// </summary>
        public gVector2<T> rb
        {
            get
            {
                return new gVector2<T>(x, z);
            }
            set
            {
                x = value.x;
                z = value.y;
            }
        }
        
        /// <summary>
        /// Gets or sets the specified subset of components. For more advanced (read-only) swizzling, use the .swizzle property.
        /// </summary>
        public gVector2<T> gb
        {
            get
            {
                return new gVector2<T>(y, z);
            }
            set
            {
                y = value.x;
                z = value.y;
            }
        }
        
        /// <summary>
        /// Gets or sets the specified subset of components. For more advanced (read-only) swizzling, use the .swizzle property.
        /// </summary>
        public gVector3<T> rgb
        {
            get
            {
                return new gVector3<T>(x, y, z);
            }
            set
            {
                x = value.x;
                y = value.y;
                z = value.z;
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
        /// Gets or sets the specified RGBA component. For more advanced (read-only) swizzling, use the .swizzle property.
        /// </summary>
        public T b
        {
            get
            {
                return z;
            }
            set
            {
                z = value;
            }
        }
        
        /// <summary>
        /// Returns an array with all values
        /// </summary>
        public T[] Values => new[] { x, y, z };
        
        /// <summary>
        /// Returns the number of components (3).
        /// </summary>
        public int Count => 3;

        #endregion


        #region Static Properties
        
        /// <summary>
        /// Predefined all-zero Vectortor
        /// </summary>
        public static gVector3<T> Zero { get; } = new gVector3<T>(default(T), default(T), default(T));

        #endregion


        #region Operators
        
        /// <summary>
        /// Returns true iff this equals rhs component-wise.
        /// </summary>
        public static bool operator==(gVector3<T> lhs, gVector3<T> rhs) => lhs.Equals(rhs);
        
        /// <summary>
        /// Returns true iff this does not equal rhs (component-wise).
        /// </summary>
        public static bool operator!=(gVector3<T> lhs, gVector3<T> rhs) => !lhs.Equals(rhs);

        #endregion


        #region Functions
        
        /// <summary>
        /// Returns an enumerator that iterates through all components.
        /// </summary>
        public IEnumerator<T> GetEnumerator()
        {
            yield return x;
            yield return y;
            yield return z;
        }
        
        /// <summary>
        /// Returns an enumerator that iterates through all components.
        /// </summary>
        IEnumerator IEnumerable.GetEnumerator() => GetEnumerator();
        
        /// <summary>
        /// Returns a string representation of this Vectortor using ', ' as a seperator.
        /// </summary>
        public override string ToString() => ToString(", ");
        
        /// <summary>
        /// Returns a string representation of this Vectortor using a provided seperator.
        /// </summary>
        public string ToString(string sep) => ((x + sep + y) + sep + z);
        
        /// <summary>
        /// Returns true iff this equals rhs component-wise.
        /// </summary>
        public bool Equals(gVector3<T> rhs) => ((EqualityComparer<T>.Default.Equals(x, rhs.x) && EqualityComparer<T>.Default.Equals(y, rhs.y)) && EqualityComparer<T>.Default.Equals(z, rhs.z));
        
        /// <summary>
        /// Returns true iff this equals rhs type- and component-wise.
        /// </summary>
        public override bool Equals(object obj)
        {
            if (ReferenceEquals(null, obj)) return false;
            return obj is gVector3<T> && Equals((gVector3<T>) obj);
        }
        
        /// <summary>
        /// Returns a hash code for this instance.
        /// </summary>
        public override int GetHashCode()
        {
            unchecked
            {
                return ((((EqualityComparer<T>.Default.GetHashCode(x)) * 397) ^ EqualityComparer<T>.Default.GetHashCode(y)) * 397) ^ EqualityComparer<T>.Default.GetHashCode(z);
            }
        }

        #endregion


        #region Component-Wise Static Functions
        
        /// <summary>
        /// Returns a bVector3 from component-wise application of Equal (EqualityComparer&lt;T&gt;.Default.Equals(lhs, rhs)).
        /// </summary>
        public static bVector3 Equal(gVector3<T> lhs, gVector3<T> rhs) => new bVector3(EqualityComparer<T>.Default.Equals(lhs.x, rhs.x), EqualityComparer<T>.Default.Equals(lhs.y, rhs.y), EqualityComparer<T>.Default.Equals(lhs.z, rhs.z));
        
        /// <summary>
        /// Returns a bVector3 from component-wise application of Equal (EqualityComparer&lt;T&gt;.Default.Equals(lhs, rhs)).
        /// </summary>
        public static bVector3 Equal(gVector3<T> lhs, T rhs) => new bVector3(EqualityComparer<T>.Default.Equals(lhs.x, rhs), EqualityComparer<T>.Default.Equals(lhs.y, rhs), EqualityComparer<T>.Default.Equals(lhs.z, rhs));
        
        /// <summary>
        /// Returns a bVector3 from component-wise application of Equal (EqualityComparer&lt;T&gt;.Default.Equals(lhs, rhs)).
        /// </summary>
        public static bVector3 Equal(T lhs, gVector3<T> rhs) => new bVector3(EqualityComparer<T>.Default.Equals(lhs, rhs.x), EqualityComparer<T>.Default.Equals(lhs, rhs.y), EqualityComparer<T>.Default.Equals(lhs, rhs.z));
        
        /// <summary>
        /// Returns a bVector from the application of Equal (EqualityComparer&lt;T&gt;.Default.Equals(lhs, rhs)).
        /// </summary>
        public static bVector3 Equal(T lhs, T rhs) => new bVector3(EqualityComparer<T>.Default.Equals(lhs, rhs));
        
        /// <summary>
        /// Returns a bVector3 from component-wise application of NotEqual (!EqualityComparer&lt;T&gt;.Default.Equals(lhs, rhs)).
        /// </summary>
        public static bVector3 NotEqual(gVector3<T> lhs, gVector3<T> rhs) => new bVector3(!EqualityComparer<T>.Default.Equals(lhs.x, rhs.x), !EqualityComparer<T>.Default.Equals(lhs.y, rhs.y), !EqualityComparer<T>.Default.Equals(lhs.z, rhs.z));
        
        /// <summary>
        /// Returns a bVector3 from component-wise application of NotEqual (!EqualityComparer&lt;T&gt;.Default.Equals(lhs, rhs)).
        /// </summary>
        public static bVector3 NotEqual(gVector3<T> lhs, T rhs) => new bVector3(!EqualityComparer<T>.Default.Equals(lhs.x, rhs), !EqualityComparer<T>.Default.Equals(lhs.y, rhs), !EqualityComparer<T>.Default.Equals(lhs.z, rhs));
        
        /// <summary>
        /// Returns a bVector3 from component-wise application of NotEqual (!EqualityComparer&lt;T&gt;.Default.Equals(lhs, rhs)).
        /// </summary>
        public static bVector3 NotEqual(T lhs, gVector3<T> rhs) => new bVector3(!EqualityComparer<T>.Default.Equals(lhs, rhs.x), !EqualityComparer<T>.Default.Equals(lhs, rhs.y), !EqualityComparer<T>.Default.Equals(lhs, rhs.z));
        
        /// <summary>
        /// Returns a bVector from the application of NotEqual (!EqualityComparer&lt;T&gt;.Default.Equals(lhs, rhs)).
        /// </summary>
        public static bVector3 NotEqual(T lhs, T rhs) => new bVector3(!EqualityComparer<T>.Default.Equals(lhs, rhs));

        #endregion

    }
}
