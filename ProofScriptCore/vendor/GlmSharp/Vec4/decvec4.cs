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
    /// A Vectortor of type decimal with 4 components.
    /// </summary>
    [RegisterCoreClassStruct]
    
    [StructLayout(LayoutKind.Sequential)]
    public struct decVector4 : IReadOnlyList<decimal>, IEquatable<decVector4>
    {

        #region Fields
        
        /// <summary>
        /// x-component
        /// </summary>
        
        public decimal x;
        
        /// <summary>
        /// y-component
        /// </summary>
        
        public decimal y;
        
        /// <summary>
        /// z-component
        /// </summary>
        
        public decimal z;
        
        /// <summary>
        /// w-component
        /// </summary>
        
        public decimal w;

        #endregion


        #region Constructors
        
        /// <summary>
        /// Component-wise constructor
        /// </summary>
        public decVector4(decimal x, decimal y, decimal z, decimal w)
        {
            this.x = x;
            this.y = y;
            this.z = z;
            this.w = w;
        }
        
        /// <summary>
        /// all-same-value constructor
        /// </summary>
        public decVector4(decimal v)
        {
            this.x = v;
            this.y = v;
            this.z = v;
            this.w = v;
        }
        
        /// <summary>
        /// from-Vectortor constructor (empty fields are zero/false)
        /// </summary>
        public decVector4(decVector2 v)
        {
            this.x = v.x;
            this.y = v.y;
            this.z = 0m;
            this.w = 0m;
        }
        
        /// <summary>
        /// from-Vectortor-and-value constructor (empty fields are zero/false)
        /// </summary>
        public decVector4(decVector2 v, decimal z)
        {
            this.x = v.x;
            this.y = v.y;
            this.z = z;
            this.w = 0m;
        }
        
        /// <summary>
        /// from-Vectortor-and-value constructor
        /// </summary>
        public decVector4(decVector2 v, decimal z, decimal w)
        {
            this.x = v.x;
            this.y = v.y;
            this.z = z;
            this.w = w;
        }
        
        /// <summary>
        /// from-Vectortor constructor (empty fields are zero/false)
        /// </summary>
        public decVector4(decVector3 v)
        {
            this.x = v.x;
            this.y = v.y;
            this.z = v.z;
            this.w = 0m;
        }
        
        /// <summary>
        /// from-Vectortor-and-value constructor
        /// </summary>
        public decVector4(decVector3 v, decimal w)
        {
            this.x = v.x;
            this.y = v.y;
            this.z = v.z;
            this.w = w;
        }
        
        /// <summary>
        /// from-Vectortor constructor
        /// </summary>
        public decVector4(decVector4 v)
        {
            this.x = v.x;
            this.y = v.y;
            this.z = v.z;
            this.w = v.w;
        }
        
        /// <summary>
        /// From-array/list constructor (superfluous values are ignored, missing values are zero-filled).
        /// </summary>
        public decVector4(IReadOnlyList<decimal> v)
        {
            var c = v.Count;
            this.x = c < 0 ? 0m : v[0];
            this.y = c < 1 ? 0m : v[1];
            this.z = c < 2 ? 0m : v[2];
            this.w = c < 3 ? 0m : v[3];
        }
        
        /// <summary>
        /// Generic from-array constructor (superfluous values are ignored, missing values are zero-filled).
        /// </summary>
        public decVector4(Object[] v)
        {
            var c = v.Length;
            this.x = c < 0 ? 0m : (decimal)v[0];
            this.y = c < 1 ? 0m : (decimal)v[1];
            this.z = c < 2 ? 0m : (decimal)v[2];
            this.w = c < 3 ? 0m : (decimal)v[3];
        }
        
        /// <summary>
        /// From-array constructor (superfluous values are ignored, missing values are zero-filled).
        /// </summary>
        public decVector4(decimal[] v)
        {
            var c = v.Length;
            this.x = c < 0 ? 0m : v[0];
            this.y = c < 1 ? 0m : v[1];
            this.z = c < 2 ? 0m : v[2];
            this.w = c < 3 ? 0m : v[3];
        }
        
        /// <summary>
        /// From-array constructor with base index (superfluous values are ignored, missing values are zero-filled).
        /// </summary>
        public decVector4(decimal[] v, int startIndex)
        {
            var c = v.Length;
            this.x = c + startIndex < 0 ? 0m : v[0 + startIndex];
            this.y = c + startIndex < 1 ? 0m : v[1 + startIndex];
            this.z = c + startIndex < 2 ? 0m : v[2 + startIndex];
            this.w = c + startIndex < 3 ? 0m : v[3 + startIndex];
        }
        
        /// <summary>
        /// From-IEnumerable constructor (superfluous values are ignored, missing values are zero-filled).
        /// </summary>
        public decVector4(IEnumerable<decimal> v)
            : this(v.ToArray())
        {
        }

        #endregion


        #region Explicit Operators
        
        /// <summary>
        /// Explicitly converts this to a iVector2.
        /// </summary>
        public static explicit operator iVector2(decVector4 v) => new iVector2((int)v.x, (int)v.y);
        
        /// <summary>
        /// Explicitly converts this to a iVector3.
        /// </summary>
        public static explicit operator iVector3(decVector4 v) => new iVector3((int)v.x, (int)v.y, (int)v.z);
        
        /// <summary>
        /// Explicitly converts this to a iVector4.
        /// </summary>
        public static explicit operator iVector4(decVector4 v) => new iVector4((int)v.x, (int)v.y, (int)v.z, (int)v.w);
        
        /// <summary>
        /// Explicitly converts this to a uVector2.
        /// </summary>
        public static explicit operator uVector2(decVector4 v) => new uVector2((uint)v.x, (uint)v.y);
        
        /// <summary>
        /// Explicitly converts this to a uVector3.
        /// </summary>
        public static explicit operator uVector3(decVector4 v) => new uVector3((uint)v.x, (uint)v.y, (uint)v.z);
        
        /// <summary>
        /// Explicitly converts this to a uVector4.
        /// </summary>
        public static explicit operator uVector4(decVector4 v) => new uVector4((uint)v.x, (uint)v.y, (uint)v.z, (uint)v.w);
        
        /// <summary>
        /// Explicitly converts this to a Vector2.
        /// </summary>
        public static explicit operator Vector2(decVector4 v) => new Vector2((float)v.x, (float)v.y);
        
        /// <summary>
        /// Explicitly converts this to a Vector3.
        /// </summary>
        public static explicit operator Vector3(decVector4 v) => new Vector3((float)v.x, (float)v.y, (float)v.z);
        
        /// <summary>
        /// Explicitly converts this to a Vector4.
        /// </summary>
        public static explicit operator Vector4(decVector4 v) => new Vector4((float)v.x, (float)v.y, (float)v.z, (float)v.w);
        
        /// <summary>
        /// Explicitly converts this to a hVector2.
        /// </summary>
        public static explicit operator hVector2(decVector4 v) => new hVector2((Half)v.x, (Half)v.y);
        
        /// <summary>
        /// Explicitly converts this to a hVector3.
        /// </summary>
        public static explicit operator hVector3(decVector4 v) => new hVector3((Half)v.x, (Half)v.y, (Half)v.z);
        
        /// <summary>
        /// Explicitly converts this to a hVector4.
        /// </summary>
        public static explicit operator hVector4(decVector4 v) => new hVector4((Half)v.x, (Half)v.y, (Half)v.z, (Half)v.w);
        
        /// <summary>
        /// Explicitly converts this to a dVector2.
        /// </summary>
        public static explicit operator dVector2(decVector4 v) => new dVector2((double)v.x, (double)v.y);
        
        /// <summary>
        /// Explicitly converts this to a dVector3.
        /// </summary>
        public static explicit operator dVector3(decVector4 v) => new dVector3((double)v.x, (double)v.y, (double)v.z);
        
        /// <summary>
        /// Explicitly converts this to a dVector4.
        /// </summary>
        public static explicit operator dVector4(decVector4 v) => new dVector4((double)v.x, (double)v.y, (double)v.z, (double)v.w);
        
        /// <summary>
        /// Explicitly converts this to a decVector2.
        /// </summary>
        public static explicit operator decVector2(decVector4 v) => new decVector2((decimal)v.x, (decimal)v.y);
        
        /// <summary>
        /// Explicitly converts this to a decVector3.
        /// </summary>
        public static explicit operator decVector3(decVector4 v) => new decVector3((decimal)v.x, (decimal)v.y, (decimal)v.z);
        
        /// <summary>
        /// Explicitly converts this to a cVector2.
        /// </summary>
       // public static explicit operator cVector2(decVector4 v) => new cVector2((Complex)v.x, (Complex)v.y);
        
        /// <summary>
        /// Explicitly converts this to a cVector3.
        /// </summary>
       // public static explicit operator cVector3(decVector4 v) => new cVector3((Complex)v.x, (Complex)v.y, (Complex)v.z);
        
        /// <summary>
        /// Explicitly converts this to a cVector4.
        /// </summary>
       // public static explicit operator cVector4(decVector4 v) => new cVector4((Complex)v.x, (Complex)v.y, (Complex)v.z, (Complex)v.w);
        
        /// <summary>
        /// Explicitly converts this to a lVector2.
        /// </summary>
        public static explicit operator lVector2(decVector4 v) => new lVector2((long)v.x, (long)v.y);
        
        /// <summary>
        /// Explicitly converts this to a lVector3.
        /// </summary>
        public static explicit operator lVector3(decVector4 v) => new lVector3((long)v.x, (long)v.y, (long)v.z);
        
        /// <summary>
        /// Explicitly converts this to a lVector4.
        /// </summary>
        public static explicit operator lVector4(decVector4 v) => new lVector4((long)v.x, (long)v.y, (long)v.z, (long)v.w);
        
        /// <summary>
        /// Explicitly converts this to a bVector2.
        /// </summary>
        public static explicit operator bVector2(decVector4 v) => new bVector2(v.x != 0m, v.y != 0m);
        
        /// <summary>
        /// Explicitly converts this to a bVector3.
        /// </summary>
        public static explicit operator bVector3(decVector4 v) => new bVector3(v.x != 0m, v.y != 0m, v.z != 0m);
        
        /// <summary>
        /// Explicitly converts this to a bVector4.
        /// </summary>
        public static explicit operator bVector4(decVector4 v) => new bVector4(v.x != 0m, v.y != 0m, v.z != 0m, v.w != 0m);
        
        /// <summary>
        /// Explicitly converts this to a decimal array.
        /// </summary>
        public static explicit operator decimal[](decVector4 v) => new [] { v.x, v.y, v.z, v.w };
        
        /// <summary>
        /// Explicitly converts this to a generic object array.
        /// </summary>
        public static explicit operator Object[](decVector4 v) => new Object[] { v.x, v.y, v.z, v.w };

        #endregion


        #region Indexer
        
        /// <summary>
        /// Gets/Sets a specific indexed component (a bit slower than direct access).
        /// </summary>
        public decimal this[int index]
        {
            get
            {
                switch (index)
                {
                    case 0: return x;
                    case 1: return y;
                    case 2: return z;
                    case 3: return w;
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
                    case 3: w = value; break;
                    default: throw new ArgumentOutOfRangeException("index");
                }
            }
        }

        #endregion


        #region Properties
        
        /// <summary>
        /// Returns an object that can be used for arbitrary swizzling (e.g. swizzle.zy)
        /// </summary>
        public swizzle_decVector4 swizzle => new swizzle_decVector4(x, y, z, w);
        
        /// <summary>
        /// Gets or sets the specified subset of components. For more advanced (read-only) swizzling, use the .swizzle property.
        /// </summary>
        public decVector2 xy
        {
            get
            {
                return new decVector2(x, y);
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
        public decVector2 xz
        {
            get
            {
                return new decVector2(x, z);
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
        public decVector2 yz
        {
            get
            {
                return new decVector2(y, z);
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
        public decVector3 xyz
        {
            get
            {
                return new decVector3(x, y, z);
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
        public decVector2 xw
        {
            get
            {
                return new decVector2(x, w);
            }
            set
            {
                x = value.x;
                w = value.y;
            }
        }
        
        /// <summary>
        /// Gets or sets the specified subset of components. For more advanced (read-only) swizzling, use the .swizzle property.
        /// </summary>
        public decVector2 yw
        {
            get
            {
                return new decVector2(y, w);
            }
            set
            {
                y = value.x;
                w = value.y;
            }
        }
        
        /// <summary>
        /// Gets or sets the specified subset of components. For more advanced (read-only) swizzling, use the .swizzle property.
        /// </summary>
        public decVector3 xyw
        {
            get
            {
                return new decVector3(x, y, w);
            }
            set
            {
                x = value.x;
                y = value.y;
                w = value.z;
            }
        }
        
        /// <summary>
        /// Gets or sets the specified subset of components. For more advanced (read-only) swizzling, use the .swizzle property.
        /// </summary>
        public decVector2 zw
        {
            get
            {
                return new decVector2(z, w);
            }
            set
            {
                z = value.x;
                w = value.y;
            }
        }
        
        /// <summary>
        /// Gets or sets the specified subset of components. For more advanced (read-only) swizzling, use the .swizzle property.
        /// </summary>
        public decVector3 xzw
        {
            get
            {
                return new decVector3(x, z, w);
            }
            set
            {
                x = value.x;
                z = value.y;
                w = value.z;
            }
        }
        
        /// <summary>
        /// Gets or sets the specified subset of components. For more advanced (read-only) swizzling, use the .swizzle property.
        /// </summary>
        public decVector3 yzw
        {
            get
            {
                return new decVector3(y, z, w);
            }
            set
            {
                y = value.x;
                z = value.y;
                w = value.z;
            }
        }
        
        /// <summary>
        /// Gets or sets the specified subset of components. For more advanced (read-only) swizzling, use the .swizzle property.
        /// </summary>
        public decVector4 xyzw
        {
            get
            {
                return new decVector4(x, y, z, w);
            }
            set
            {
                x = value.x;
                y = value.y;
                z = value.z;
                w = value.w;
            }
        }
        
        /// <summary>
        /// Gets or sets the specified subset of components. For more advanced (read-only) swizzling, use the .swizzle property.
        /// </summary>
        public decVector2 rg
        {
            get
            {
                return new decVector2(x, y);
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
        public decVector2 rb
        {
            get
            {
                return new decVector2(x, z);
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
        public decVector2 gb
        {
            get
            {
                return new decVector2(y, z);
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
        public decVector3 rgb
        {
            get
            {
                return new decVector3(x, y, z);
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
        public decVector2 ra
        {
            get
            {
                return new decVector2(x, w);
            }
            set
            {
                x = value.x;
                w = value.y;
            }
        }
        
        /// <summary>
        /// Gets or sets the specified subset of components. For more advanced (read-only) swizzling, use the .swizzle property.
        /// </summary>
        public decVector2 ga
        {
            get
            {
                return new decVector2(y, w);
            }
            set
            {
                y = value.x;
                w = value.y;
            }
        }
        
        /// <summary>
        /// Gets or sets the specified subset of components. For more advanced (read-only) swizzling, use the .swizzle property.
        /// </summary>
        public decVector3 rga
        {
            get
            {
                return new decVector3(x, y, w);
            }
            set
            {
                x = value.x;
                y = value.y;
                w = value.z;
            }
        }
        
        /// <summary>
        /// Gets or sets the specified subset of components. For more advanced (read-only) swizzling, use the .swizzle property.
        /// </summary>
        public decVector2 ba
        {
            get
            {
                return new decVector2(z, w);
            }
            set
            {
                z = value.x;
                w = value.y;
            }
        }
        
        /// <summary>
        /// Gets or sets the specified subset of components. For more advanced (read-only) swizzling, use the .swizzle property.
        /// </summary>
        public decVector3 rba
        {
            get
            {
                return new decVector3(x, z, w);
            }
            set
            {
                x = value.x;
                z = value.y;
                w = value.z;
            }
        }
        
        /// <summary>
        /// Gets or sets the specified subset of components. For more advanced (read-only) swizzling, use the .swizzle property.
        /// </summary>
        public decVector3 gba
        {
            get
            {
                return new decVector3(y, z, w);
            }
            set
            {
                y = value.x;
                z = value.y;
                w = value.z;
            }
        }
        
        /// <summary>
        /// Gets or sets the specified subset of components. For more advanced (read-only) swizzling, use the .swizzle property.
        /// </summary>
        public decVector4 rgba
        {
            get
            {
                return new decVector4(x, y, z, w);
            }
            set
            {
                x = value.x;
                y = value.y;
                z = value.z;
                w = value.w;
            }
        }
        
        /// <summary>
        /// Gets or sets the specified RGBA component. For more advanced (read-only) swizzling, use the .swizzle property.
        /// </summary>
        public decimal r
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
        public decimal g
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
        public decimal b
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
        /// Gets or sets the specified RGBA component. For more advanced (read-only) swizzling, use the .swizzle property.
        /// </summary>
        public decimal a
        {
            get
            {
                return w;
            }
            set
            {
                w = value;
            }
        }
        
        /// <summary>
        /// Returns an array with all values
        /// </summary>
        public decimal[] Values => new[] { x, y, z, w };
        
        /// <summary>
        /// Returns the number of components (4).
        /// </summary>
        public int Count => 4;
        
        /// <summary>
        /// Returns the minimal component of this Vectortor.
        /// </summary>
        public decimal MinElement => Math.Min(Math.Min(x, y), Math.Min(z, w));
        
        /// <summary>
        /// Returns the maximal component of this Vectortor.
        /// </summary>
        public decimal MaxElement => Math.Max(Math.Max(x, y), Math.Max(z, w));
        
        /// <summary>
        /// Returns the euclidean length of this Vectortor.
        /// </summary>
        public decimal Length => (decimal)(((x*x + y*y) + (z*z + w*w))).Sqrt();
        
        /// <summary>
        /// Returns the squared euclidean length of this Vectortor.
        /// </summary>
        public decimal LengthSqr => ((x*x + y*y) + (z*z + w*w));
        
        /// <summary>
        /// Returns the sum of all components.
        /// </summary>
        public decimal Sum => ((x + y) + (z + w));
        
        /// <summary>
        /// Returns the euclidean norm of this Vectortor.
        /// </summary>
        public decimal Norm => (decimal)(((x*x + y*y) + (z*z + w*w))).Sqrt();
        
        /// <summary>
        /// Returns the one-norm of this Vectortor.
        /// </summary>
        public decimal Norm1 => ((Math.Abs(x) + Math.Abs(y)) + (Math.Abs(z) + Math.Abs(w)));
        
        /// <summary>
        /// Returns the two-norm (euclidean length) of this Vectortor.
        /// </summary>
        public decimal Norm2 => (decimal)(((x*x + y*y) + (z*z + w*w))).Sqrt();
        
        /// <summary>
        /// Returns the max-norm of this Vectortor.
        /// </summary>
        public decimal NormMax => Math.Max(Math.Max(Math.Abs(x), Math.Abs(y)), Math.Max(Math.Abs(z), Math.Abs(w)));
        
        /// <summary>
        /// Returns a copy of this Vectortor with length one (undefined if this has zero length).
        /// </summary>
        public decVector4 Normalized => this / (decimal)Length;
        
        /// <summary>
        /// Returns a copy of this Vectortor with length one (returns zero if length is zero).
        /// </summary>
        public decVector4 NormalizedSafe => this == Zero ? Zero : this / (decimal)Length;

        #endregion


        #region Static Properties
        
        /// <summary>
        /// Predefined all-zero Vectortor
        /// </summary>
        public static decVector4 Zero { get; } = new decVector4(0m, 0m, 0m, 0m);
        
        /// <summary>
        /// Predefined all-ones Vectortor
        /// </summary>
        public static decVector4 Ones { get; } = new decVector4(1m, 1m, 1m, 1m);
        
        /// <summary>
        /// Predefined unit-X Vectortor
        /// </summary>
        public static decVector4 UnitX { get; } = new decVector4(1m, 0m, 0m, 0m);
        
        /// <summary>
        /// Predefined unit-Y Vectortor
        /// </summary>
        public static decVector4 UnitY { get; } = new decVector4(0m, 1m, 0m, 0m);
        
        /// <summary>
        /// Predefined unit-Z Vectortor
        /// </summary>
        public static decVector4 UnitZ { get; } = new decVector4(0m, 0m, 1m, 0m);
        
        /// <summary>
        /// Predefined unit-W Vectortor
        /// </summary>
        public static decVector4 UnitW { get; } = new decVector4(0m, 0m, 0m, 1m);
        
        /// <summary>
        /// Predefined all-MaxValue Vectortor
        /// </summary>
        public static decVector4 MaxValue { get; } = new decVector4(decimal.MaxValue, decimal.MaxValue, decimal.MaxValue, decimal.MaxValue);
        
        /// <summary>
        /// Predefined all-MinValue Vectortor
        /// </summary>
        public static decVector4 MinValue { get; } = new decVector4(decimal.MinValue, decimal.MinValue, decimal.MinValue, decimal.MinValue);
        
        /// <summary>
        /// Predefined all-MinusOne Vectortor
        /// </summary>
        public static decVector4 MinusOne { get; } = new decVector4(decimal.MinusOne, decimal.MinusOne, decimal.MinusOne, decimal.MinusOne);

        #endregion


        #region Operators
        
        /// <summary>
        /// Returns true iff this equals rhs component-wise.
        /// </summary>
        public static bool operator==(decVector4 lhs, decVector4 rhs) => lhs.Equals(rhs);
        
        /// <summary>
        /// Returns true iff this does not equal rhs (component-wise).
        /// </summary>
        public static bool operator!=(decVector4 lhs, decVector4 rhs) => !lhs.Equals(rhs);

        #endregion


        #region Functions
        
        /// <summary>
        /// Returns an enumerator that iterates through all components.
        /// </summary>
        public IEnumerator<decimal> GetEnumerator()
        {
            yield return x;
            yield return y;
            yield return z;
            yield return w;
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
        public string ToString(string sep) => ((x + sep + y) + sep + (z + sep + w));
        
        /// <summary>
        /// Returns a string representation of this Vectortor using a provided seperator and a forMatrix provider for each component.
        /// </summary>
        public string ToString(string sep, IFormatProvider provider) => ((x.ToString(provider) + sep + y.ToString(provider)) + sep + (z.ToString(provider) + sep + w.ToString(provider)));
        
        /// <summary>
        /// Returns a string representation of this Vectortor using a provided seperator and a forMatrix for each component.
        /// </summary>
        public string ToString(string sep, string forMatrix) => ((x.ToString(forMatrix) + sep + y.ToString(forMatrix)) + sep + (z.ToString(forMatrix) + sep + w.ToString(forMatrix)));
        
        /// <summary>
        /// Returns a string representation of this Vectortor using a provided seperator and a forMatrix and forMatrix provider for each component.
        /// </summary>
        public string ToString(string sep, string forMatrix, IFormatProvider provider) => ((x.ToString(forMatrix, provider) + sep + y.ToString(forMatrix, provider)) + sep + (z.ToString(forMatrix, provider) + sep + w.ToString(forMatrix, provider)));
        
        /// <summary>
        /// Returns true iff this equals rhs component-wise.
        /// </summary>
        public bool Equals(decVector4 rhs) => ((x.Equals(rhs.x) && y.Equals(rhs.y)) && (z.Equals(rhs.z) && w.Equals(rhs.w)));
        
        /// <summary>
        /// Returns true iff this equals rhs type- and component-wise.
        /// </summary>
        public override bool Equals(object obj)
        {
            if (ReferenceEquals(null, obj)) return false;
            return obj is decVector4 && Equals((decVector4) obj);
        }
        
        /// <summary>
        /// Returns a hash code for this instance.
        /// </summary>
        public override int GetHashCode()
        {
            unchecked
            {
                return ((((((x.GetHashCode()) * 397) ^ y.GetHashCode()) * 397) ^ z.GetHashCode()) * 397) ^ w.GetHashCode();
            }
        }
        
        /// <summary>
        /// Returns the p-norm of this Vectortor.
        /// </summary>
        public double NormP(double p) => Math.Pow(((Math.Pow((double)Math.Abs(x), p) + Math.Pow((double)Math.Abs(y), p)) + (Math.Pow((double)Math.Abs(z), p) + Math.Pow((double)Math.Abs(w), p))), 1 / p);

        #endregion


        #region Static Functions
        
        /// <summary>
        /// Converts the string representation of the Vectortor into a Vectortor representation (using ', ' as a separator).
        /// </summary>
        public static decVector4 Parse(string s) => Parse(s, ", ");
        
        /// <summary>
        /// Converts the string representation of the Vectortor into a Vectortor representation (using a designated separator).
        /// </summary>
        public static decVector4 Parse(string s, string sep)
        {
            var kvp = s.Split(new[] { sep }, StringSplitOptions.None);
            if (kvp.Length != 4) throw new FormatException("input has not exactly 4 parts");
            return new decVector4(decimal.Parse(kvp[0].Trim()), decimal.Parse(kvp[1].Trim()), decimal.Parse(kvp[2].Trim()), decimal.Parse(kvp[3].Trim()));
        }
        
        /// <summary>
        /// Converts the string representation of the Vectortor into a Vectortor representation (using a designated separator and a type provider).
        /// </summary>
        public static decVector4 Parse(string s, string sep, IFormatProvider provider)
        {
            var kvp = s.Split(new[] { sep }, StringSplitOptions.None);
            if (kvp.Length != 4) throw new FormatException("input has not exactly 4 parts");
            return new decVector4(decimal.Parse(kvp[0].Trim(), provider), decimal.Parse(kvp[1].Trim(), provider), decimal.Parse(kvp[2].Trim(), provider), decimal.Parse(kvp[3].Trim(), provider));
        }
        
        /// <summary>
        /// Converts the string representation of the Vectortor into a Vectortor representation (using a designated separator and a number style).
        /// </summary>
        public static decVector4 Parse(string s, string sep, NumberStyles style)
        {
            var kvp = s.Split(new[] { sep }, StringSplitOptions.None);
            if (kvp.Length != 4) throw new FormatException("input has not exactly 4 parts");
            return new decVector4(decimal.Parse(kvp[0].Trim(), style), decimal.Parse(kvp[1].Trim(), style), decimal.Parse(kvp[2].Trim(), style), decimal.Parse(kvp[3].Trim(), style));
        }
        
        /// <summary>
        /// Converts the string representation of the Vectortor into a Vectortor representation (using a designated separator and a number style and a forMatrix provider).
        /// </summary>
        public static decVector4 Parse(string s, string sep, NumberStyles style, IFormatProvider provider)
        {
            var kvp = s.Split(new[] { sep }, StringSplitOptions.None);
            if (kvp.Length != 4) throw new FormatException("input has not exactly 4 parts");
            return new decVector4(decimal.Parse(kvp[0].Trim(), style, provider), decimal.Parse(kvp[1].Trim(), style, provider), decimal.Parse(kvp[2].Trim(), style, provider), decimal.Parse(kvp[3].Trim(), style, provider));
        }
        
        /// <summary>
        /// Tries to convert the string representation of the Vectortor into a Vectortor representation (using ', ' as a separator), returns false if string was invalid.
        /// </summary>
        public static bool TryParse(string s, out decVector4 result) => TryParse(s, ", ", out result);
        
        /// <summary>
        /// Tries to convert the string representation of the Vectortor into a Vectortor representation (using a designated separator), returns false if string was invalid.
        /// </summary>
        public static bool TryParse(string s, string sep, out decVector4 result)
        {
            result = Zero;
            if (string.IsNullOrEmpty(s)) return false;
            var kvp = s.Split(new[] { sep }, StringSplitOptions.None);
            if (kvp.Length != 4) return false;
            decimal x = 0m, y = 0m, z = 0m, w = 0m;
            var ok = ((decimal.TryParse(kvp[0].Trim(), out x) && decimal.TryParse(kvp[1].Trim(), out y)) && (decimal.TryParse(kvp[2].Trim(), out z) && decimal.TryParse(kvp[3].Trim(), out w)));
            result = ok ? new decVector4(x, y, z, w) : Zero;
            return ok;
        }
        
        /// <summary>
        /// Tries to convert the string representation of the Vectortor into a Vectortor representation (using a designated separator and a number style and a forMatrix provider), returns false if string was invalid.
        /// </summary>
        public static bool TryParse(string s, string sep, NumberStyles style, IFormatProvider provider, out decVector4 result)
        {
            result = Zero;
            if (string.IsNullOrEmpty(s)) return false;
            var kvp = s.Split(new[] { sep }, StringSplitOptions.None);
            if (kvp.Length != 4) return false;
            decimal x = 0m, y = 0m, z = 0m, w = 0m;
            var ok = ((decimal.TryParse(kvp[0].Trim(), style, provider, out x) && decimal.TryParse(kvp[1].Trim(), style, provider, out y)) && (decimal.TryParse(kvp[2].Trim(), style, provider, out z) && decimal.TryParse(kvp[3].Trim(), style, provider, out w)));
            result = ok ? new decVector4(x, y, z, w) : Zero;
            return ok;
        }
        
        /// <summary>
        /// Returns true iff distance between lhs and rhs is less than or equal to epsilon
        /// </summary>
        public static bool ApproxEqual(decVector4 lhs, decVector4 rhs, decimal eps = 0.1m) => Distance(lhs, rhs) <= eps;
        
        /// <summary>
        /// OuterProduct treats the first parameter c as a column Vectortor (Matrix with one column) and the second parameter r as a row Vectortor (Matrix with one row) and does a linear algebraic Matrix multiply c * r, yielding a Matrix whose number of rows is the number of components in c and whose number of columns is the number of components in r.
        /// </summary>
        public static decMatrix4x2 OuterProduct(decVector2 c, decVector4 r) => new decMatrix4x2(c.x * r.x, c.y * r.x, c.x * r.y, c.y * r.y, c.x * r.z, c.y * r.z, c.x * r.w, c.y * r.w);
        
        /// <summary>
        /// OuterProduct treats the first parameter c as a column Vectortor (Matrix with one column) and the second parameter r as a row Vectortor (Matrix with one row) and does a linear algebraic Matrix multiply c * r, yielding a Matrix whose number of rows is the number of components in c and whose number of columns is the number of components in r.
        /// </summary>
        public static decMatrix2x4 OuterProduct(decVector4 c, decVector2 r) => new decMatrix2x4(c.x * r.x, c.y * r.x, c.z * r.x, c.w * r.x, c.x * r.y, c.y * r.y, c.z * r.y, c.w * r.y);
        
        /// <summary>
        /// OuterProduct treats the first parameter c as a column Vectortor (Matrix with one column) and the second parameter r as a row Vectortor (Matrix with one row) and does a linear algebraic Matrix multiply c * r, yielding a Matrix whose number of rows is the number of components in c and whose number of columns is the number of components in r.
        /// </summary>
        public static decMatrix4x3 OuterProduct(decVector3 c, decVector4 r) => new decMatrix4x3(c.x * r.x, c.y * r.x, c.z * r.x, c.x * r.y, c.y * r.y, c.z * r.y, c.x * r.z, c.y * r.z, c.z * r.z, c.x * r.w, c.y * r.w, c.z * r.w);
        
        /// <summary>
        /// OuterProduct treats the first parameter c as a column Vectortor (Matrix with one column) and the second parameter r as a row Vectortor (Matrix with one row) and does a linear algebraic Matrix multiply c * r, yielding a Matrix whose number of rows is the number of components in c and whose number of columns is the number of components in r.
        /// </summary>
        public static decMatrix3x4 OuterProduct(decVector4 c, decVector3 r) => new decMatrix3x4(c.x * r.x, c.y * r.x, c.z * r.x, c.w * r.x, c.x * r.y, c.y * r.y, c.z * r.y, c.w * r.y, c.x * r.z, c.y * r.z, c.z * r.z, c.w * r.z);
        
        /// <summary>
        /// OuterProduct treats the first parameter c as a column Vectortor (Matrix with one column) and the second parameter r as a row Vectortor (Matrix with one row) and does a linear algebraic Matrix multiply c * r, yielding a Matrix whose number of rows is the number of components in c and whose number of columns is the number of components in r.
        /// </summary>
        public static decMatrix4 OuterProduct(decVector4 c, decVector4 r) => new decMatrix4(c.x * r.x, c.y * r.x, c.z * r.x, c.w * r.x, c.x * r.y, c.y * r.y, c.z * r.y, c.w * r.y, c.x * r.z, c.y * r.z, c.z * r.z, c.w * r.z, c.x * r.w, c.y * r.w, c.z * r.w, c.w * r.w);
        
        /// <summary>
        /// Returns the inner product (dot product, scalar product) of the two Vectortors.
        /// </summary>
        public static decimal Dot(decVector4 lhs, decVector4 rhs) => ((lhs.x * rhs.x + lhs.y * rhs.y) + (lhs.z * rhs.z + lhs.w * rhs.w));
        
        /// <summary>
        /// Returns the euclidean distance between the two Vectortors.
        /// </summary>
        public static decimal Distance(decVector4 lhs, decVector4 rhs) => (lhs - rhs).Length;
        
        /// <summary>
        /// Returns the squared euclidean distance between the two Vectortors.
        /// </summary>
        public static decimal DistanceSqr(decVector4 lhs, decVector4 rhs) => (lhs - rhs).LengthSqr;
        
        /// <summary>
        /// Calculate the reflection direction for an incident Vectortor (N should be normalized in order to achieve the desired result).
        /// </summary>
        public static decVector4 Reflect(decVector4 I, decVector4 N) => I - 2 * Dot(N, I) * N;
        
        /// <summary>
        /// Calculate the refraction direction for an incident Vectortor (The input parameters I and N should be normalized in order to achieve the desired result).
        /// </summary>
        public static decVector4 Refract(decVector4 I, decVector4 N, decimal eta)
        {
            var dNI = Dot(N, I);
            var k = 1 - eta * eta * (1 - dNI * dNI);
            if (k < 0) return Zero;
            return eta * I - (eta * dNI + (decimal)(k).Sqrt()) * N;
        }
        
        /// <summary>
        /// Returns a Vectortor pointing in the same direction as another (faceforward orients a Vectortor to point away from a surface as defined by its normal. If dot(Nref, I) is negative faceforward returns N, otherwise it returns -N).
        /// </summary>
        public static decVector4 FaceForward(decVector4 N, decVector4 I, decVector4 Nref) => Dot(Nref, I) < 0 ? N : -N;
        
        /// <summary>
        /// Returns a decVector4 with independent and identically distributed uniform values between 0.0 and 1.0.
        /// </summary>
        public static decVector4 Random(System.Random random) => new decVector4((decimal)random.NextDouble(), (decimal)random.NextDouble(), (decimal)random.NextDouble(), (decimal)random.NextDouble());
        
        /// <summary>
        /// Returns a decVector4 with independent and identically distributed uniform values between -1.0 and 1.0.
        /// </summary>
        public static decVector4 RandomSigned(System.Random random) => new decVector4((decimal)(random.NextDouble() * 2.0 - 1.0), (decimal)(random.NextDouble() * 2.0 - 1.0), (decimal)(random.NextDouble() * 2.0 - 1.0), (decimal)(random.NextDouble() * 2.0 - 1.0));
        
        /// <summary>
        /// Returns a decVector4 with independent and identically distributed values according to a normal distribution (zero mean, unit variance).
        /// </summary>
        public static decVector4 RandomNormal(System.Random random) => new decVector4((decimal)(Math.Cos(2 * Math.PI * random.NextDouble()) * Math.Sqrt(-2.0 * Math.Log(random.NextDouble()))), (decimal)(Math.Cos(2 * Math.PI * random.NextDouble()) * Math.Sqrt(-2.0 * Math.Log(random.NextDouble()))), (decimal)(Math.Cos(2 * Math.PI * random.NextDouble()) * Math.Sqrt(-2.0 * Math.Log(random.NextDouble()))), (decimal)(Math.Cos(2 * Math.PI * random.NextDouble()) * Math.Sqrt(-2.0 * Math.Log(random.NextDouble()))));

        #endregion


        #region Component-Wise Static Functions
        
        /// <summary>
        /// Returns a bVector4 from component-wise application of Equal (lhs == rhs).
        /// </summary>
        public static bVector4 Equal(decVector4 lhs, decVector4 rhs) => new bVector4(lhs.x == rhs.x, lhs.y == rhs.y, lhs.z == rhs.z, lhs.w == rhs.w);
        
        /// <summary>
        /// Returns a bVector4 from component-wise application of Equal (lhs == rhs).
        /// </summary>
        public static bVector4 Equal(decVector4 lhs, decimal rhs) => new bVector4(lhs.x == rhs, lhs.y == rhs, lhs.z == rhs, lhs.w == rhs);
        
        /// <summary>
        /// Returns a bVector4 from component-wise application of Equal (lhs == rhs).
        /// </summary>
        public static bVector4 Equal(decimal lhs, decVector4 rhs) => new bVector4(lhs == rhs.x, lhs == rhs.y, lhs == rhs.z, lhs == rhs.w);
        
        /// <summary>
        /// Returns a bVector from the application of Equal (lhs == rhs).
        /// </summary>
        public static bVector4 Equal(decimal lhs, decimal rhs) => new bVector4(lhs == rhs);
        
        /// <summary>
        /// Returns a bVector4 from component-wise application of NotEqual (lhs != rhs).
        /// </summary>
        public static bVector4 NotEqual(decVector4 lhs, decVector4 rhs) => new bVector4(lhs.x != rhs.x, lhs.y != rhs.y, lhs.z != rhs.z, lhs.w != rhs.w);
        
        /// <summary>
        /// Returns a bVector4 from component-wise application of NotEqual (lhs != rhs).
        /// </summary>
        public static bVector4 NotEqual(decVector4 lhs, decimal rhs) => new bVector4(lhs.x != rhs, lhs.y != rhs, lhs.z != rhs, lhs.w != rhs);
        
        /// <summary>
        /// Returns a bVector4 from component-wise application of NotEqual (lhs != rhs).
        /// </summary>
        public static bVector4 NotEqual(decimal lhs, decVector4 rhs) => new bVector4(lhs != rhs.x, lhs != rhs.y, lhs != rhs.z, lhs != rhs.w);
        
        /// <summary>
        /// Returns a bVector from the application of NotEqual (lhs != rhs).
        /// </summary>
        public static bVector4 NotEqual(decimal lhs, decimal rhs) => new bVector4(lhs != rhs);
        
        /// <summary>
        /// Returns a bVector4 from component-wise application of GreaterThan (lhs &gt; rhs).
        /// </summary>
        public static bVector4 GreaterThan(decVector4 lhs, decVector4 rhs) => new bVector4(lhs.x > rhs.x, lhs.y > rhs.y, lhs.z > rhs.z, lhs.w > rhs.w);
        
        /// <summary>
        /// Returns a bVector4 from component-wise application of GreaterThan (lhs &gt; rhs).
        /// </summary>
        public static bVector4 GreaterThan(decVector4 lhs, decimal rhs) => new bVector4(lhs.x > rhs, lhs.y > rhs, lhs.z > rhs, lhs.w > rhs);
        
        /// <summary>
        /// Returns a bVector4 from component-wise application of GreaterThan (lhs &gt; rhs).
        /// </summary>
        public static bVector4 GreaterThan(decimal lhs, decVector4 rhs) => new bVector4(lhs > rhs.x, lhs > rhs.y, lhs > rhs.z, lhs > rhs.w);
        
        /// <summary>
        /// Returns a bVector from the application of GreaterThan (lhs &gt; rhs).
        /// </summary>
        public static bVector4 GreaterThan(decimal lhs, decimal rhs) => new bVector4(lhs > rhs);
        
        /// <summary>
        /// Returns a bVector4 from component-wise application of GreaterThanEqual (lhs &gt;= rhs).
        /// </summary>
        public static bVector4 GreaterThanEqual(decVector4 lhs, decVector4 rhs) => new bVector4(lhs.x >= rhs.x, lhs.y >= rhs.y, lhs.z >= rhs.z, lhs.w >= rhs.w);
        
        /// <summary>
        /// Returns a bVector4 from component-wise application of GreaterThanEqual (lhs &gt;= rhs).
        /// </summary>
        public static bVector4 GreaterThanEqual(decVector4 lhs, decimal rhs) => new bVector4(lhs.x >= rhs, lhs.y >= rhs, lhs.z >= rhs, lhs.w >= rhs);
        
        /// <summary>
        /// Returns a bVector4 from component-wise application of GreaterThanEqual (lhs &gt;= rhs).
        /// </summary>
        public static bVector4 GreaterThanEqual(decimal lhs, decVector4 rhs) => new bVector4(lhs >= rhs.x, lhs >= rhs.y, lhs >= rhs.z, lhs >= rhs.w);
        
        /// <summary>
        /// Returns a bVector from the application of GreaterThanEqual (lhs &gt;= rhs).
        /// </summary>
        public static bVector4 GreaterThanEqual(decimal lhs, decimal rhs) => new bVector4(lhs >= rhs);
        
        /// <summary>
        /// Returns a bVector4 from component-wise application of LesserThan (lhs &lt; rhs).
        /// </summary>
        public static bVector4 LesserThan(decVector4 lhs, decVector4 rhs) => new bVector4(lhs.x < rhs.x, lhs.y < rhs.y, lhs.z < rhs.z, lhs.w < rhs.w);
        
        /// <summary>
        /// Returns a bVector4 from component-wise application of LesserThan (lhs &lt; rhs).
        /// </summary>
        public static bVector4 LesserThan(decVector4 lhs, decimal rhs) => new bVector4(lhs.x < rhs, lhs.y < rhs, lhs.z < rhs, lhs.w < rhs);
        
        /// <summary>
        /// Returns a bVector4 from component-wise application of LesserThan (lhs &lt; rhs).
        /// </summary>
        public static bVector4 LesserThan(decimal lhs, decVector4 rhs) => new bVector4(lhs < rhs.x, lhs < rhs.y, lhs < rhs.z, lhs < rhs.w);
        
        /// <summary>
        /// Returns a bVector from the application of LesserThan (lhs &lt; rhs).
        /// </summary>
        public static bVector4 LesserThan(decimal lhs, decimal rhs) => new bVector4(lhs < rhs);
        
        /// <summary>
        /// Returns a bVector4 from component-wise application of LesserThanEqual (lhs &lt;= rhs).
        /// </summary>
        public static bVector4 LesserThanEqual(decVector4 lhs, decVector4 rhs) => new bVector4(lhs.x <= rhs.x, lhs.y <= rhs.y, lhs.z <= rhs.z, lhs.w <= rhs.w);
        
        /// <summary>
        /// Returns a bVector4 from component-wise application of LesserThanEqual (lhs &lt;= rhs).
        /// </summary>
        public static bVector4 LesserThanEqual(decVector4 lhs, decimal rhs) => new bVector4(lhs.x <= rhs, lhs.y <= rhs, lhs.z <= rhs, lhs.w <= rhs);
        
        /// <summary>
        /// Returns a bVector4 from component-wise application of LesserThanEqual (lhs &lt;= rhs).
        /// </summary>
        public static bVector4 LesserThanEqual(decimal lhs, decVector4 rhs) => new bVector4(lhs <= rhs.x, lhs <= rhs.y, lhs <= rhs.z, lhs <= rhs.w);
        
        /// <summary>
        /// Returns a bVector from the application of LesserThanEqual (lhs &lt;= rhs).
        /// </summary>
        public static bVector4 LesserThanEqual(decimal lhs, decimal rhs) => new bVector4(lhs <= rhs);
        
        /// <summary>
        /// Returns a decVector4 from component-wise application of Abs (Math.Abs(v)).
        /// </summary>
        public static decVector4 Abs(decVector4 v) => new decVector4(Math.Abs(v.x), Math.Abs(v.y), Math.Abs(v.z), Math.Abs(v.w));
        
        /// <summary>
        /// Returns a decVector from the application of Abs (Math.Abs(v)).
        /// </summary>
        public static decVector4 Abs(decimal v) => new decVector4(Math.Abs(v));
        
        /// <summary>
        /// Returns a decVector4 from component-wise application of HermiteInterpolationOrder3 ((3 - 2 * v) * v * v).
        /// </summary>
        public static decVector4 HermiteInterpolationOrder3(decVector4 v) => new decVector4((3 - 2 * v.x) * v.x * v.x, (3 - 2 * v.y) * v.y * v.y, (3 - 2 * v.z) * v.z * v.z, (3 - 2 * v.w) * v.w * v.w);
        
        /// <summary>
        /// Returns a decVector from the application of HermiteInterpolationOrder3 ((3 - 2 * v) * v * v).
        /// </summary>
        public static decVector4 HermiteInterpolationOrder3(decimal v) => new decVector4((3 - 2 * v) * v * v);
        
        /// <summary>
        /// Returns a decVector4 from component-wise application of HermiteInterpolationOrder5 (((6 * v - 15) * v + 10) * v * v * v).
        /// </summary>
        public static decVector4 HermiteInterpolationOrder5(decVector4 v) => new decVector4(((6 * v.x - 15) * v.x + 10) * v.x * v.x * v.x, ((6 * v.y - 15) * v.y + 10) * v.y * v.y * v.y, ((6 * v.z - 15) * v.z + 10) * v.z * v.z * v.z, ((6 * v.w - 15) * v.w + 10) * v.w * v.w * v.w);
        
        /// <summary>
        /// Returns a decVector from the application of HermiteInterpolationOrder5 (((6 * v - 15) * v + 10) * v * v * v).
        /// </summary>
        public static decVector4 HermiteInterpolationOrder5(decimal v) => new decVector4(((6 * v - 15) * v + 10) * v * v * v);
        
        /// <summary>
        /// Returns a decVector4 from component-wise application of Sqr (v * v).
        /// </summary>
        public static decVector4 Sqr(decVector4 v) => new decVector4(v.x * v.x, v.y * v.y, v.z * v.z, v.w * v.w);
        
        /// <summary>
        /// Returns a decVector from the application of Sqr (v * v).
        /// </summary>
        public static decVector4 Sqr(decimal v) => new decVector4(v * v);
        
        /// <summary>
        /// Returns a decVector4 from component-wise application of Pow2 (v * v).
        /// </summary>
        public static decVector4 Pow2(decVector4 v) => new decVector4(v.x * v.x, v.y * v.y, v.z * v.z, v.w * v.w);
        
        /// <summary>
        /// Returns a decVector from the application of Pow2 (v * v).
        /// </summary>
        public static decVector4 Pow2(decimal v) => new decVector4(v * v);
        
        /// <summary>
        /// Returns a decVector4 from component-wise application of Pow3 (v * v * v).
        /// </summary>
        public static decVector4 Pow3(decVector4 v) => new decVector4(v.x * v.x * v.x, v.y * v.y * v.y, v.z * v.z * v.z, v.w * v.w * v.w);
        
        /// <summary>
        /// Returns a decVector from the application of Pow3 (v * v * v).
        /// </summary>
        public static decVector4 Pow3(decimal v) => new decVector4(v * v * v);
        
        /// <summary>
        /// Returns a decVector4 from component-wise application of Step (v &gt;= 0m ? 1m : 0m).
        /// </summary>
        public static decVector4 Step(decVector4 v) => new decVector4(v.x >= 0m ? 1m : 0m, v.y >= 0m ? 1m : 0m, v.z >= 0m ? 1m : 0m, v.w >= 0m ? 1m : 0m);
        
        /// <summary>
        /// Returns a decVector from the application of Step (v &gt;= 0m ? 1m : 0m).
        /// </summary>
        public static decVector4 Step(decimal v) => new decVector4(v >= 0m ? 1m : 0m);
        
        /// <summary>
        /// Returns a decVector4 from component-wise application of Sqrt ((decimal)Math.Sqrt((double)v)).
        /// </summary>
        public static decVector4 Sqrt(decVector4 v) => new decVector4((decimal)Math.Sqrt((double)v.x), (decimal)Math.Sqrt((double)v.y), (decimal)Math.Sqrt((double)v.z), (decimal)Math.Sqrt((double)v.w));
        
        /// <summary>
        /// Returns a decVector from the application of Sqrt ((decimal)Math.Sqrt((double)v)).
        /// </summary>
        public static decVector4 Sqrt(decimal v) => new decVector4((decimal)Math.Sqrt((double)v));
        
        /// <summary>
        /// Returns a decVector4 from component-wise application of InverseSqrt ((decimal)(1.0 / Math.Sqrt((double)v))).
        /// </summary>
        public static decVector4 InverseSqrt(decVector4 v) => new decVector4((decimal)(1.0 / Math.Sqrt((double)v.x)), (decimal)(1.0 / Math.Sqrt((double)v.y)), (decimal)(1.0 / Math.Sqrt((double)v.z)), (decimal)(1.0 / Math.Sqrt((double)v.w)));
        
        /// <summary>
        /// Returns a decVector from the application of InverseSqrt ((decimal)(1.0 / Math.Sqrt((double)v))).
        /// </summary>
        public static decVector4 InverseSqrt(decimal v) => new decVector4((decimal)(1.0 / Math.Sqrt((double)v)));
        
        /// <summary>
        /// Returns a iVector4 from component-wise application of Sign (Math.Sign(v)).
        /// </summary>
        public static iVector4 Sign(decVector4 v) => new iVector4(Math.Sign(v.x), Math.Sign(v.y), Math.Sign(v.z), Math.Sign(v.w));
        
        /// <summary>
        /// Returns a iVector from the application of Sign (Math.Sign(v)).
        /// </summary>
        public static iVector4 Sign(decimal v) => new iVector4(Math.Sign(v));
        
        /// <summary>
        /// Returns a decVector4 from component-wise application of Max (Math.Max(lhs, rhs)).
        /// </summary>
        public static decVector4 Max(decVector4 lhs, decVector4 rhs) => new decVector4(Math.Max(lhs.x, rhs.x), Math.Max(lhs.y, rhs.y), Math.Max(lhs.z, rhs.z), Math.Max(lhs.w, rhs.w));
        
        /// <summary>
        /// Returns a decVector4 from component-wise application of Max (Math.Max(lhs, rhs)).
        /// </summary>
        public static decVector4 Max(decVector4 lhs, decimal rhs) => new decVector4(Math.Max(lhs.x, rhs), Math.Max(lhs.y, rhs), Math.Max(lhs.z, rhs), Math.Max(lhs.w, rhs));
        
        /// <summary>
        /// Returns a decVector4 from component-wise application of Max (Math.Max(lhs, rhs)).
        /// </summary>
        public static decVector4 Max(decimal lhs, decVector4 rhs) => new decVector4(Math.Max(lhs, rhs.x), Math.Max(lhs, rhs.y), Math.Max(lhs, rhs.z), Math.Max(lhs, rhs.w));
        
        /// <summary>
        /// Returns a decVector from the application of Max (Math.Max(lhs, rhs)).
        /// </summary>
        public static decVector4 Max(decimal lhs, decimal rhs) => new decVector4(Math.Max(lhs, rhs));
        
        /// <summary>
        /// Returns a decVector4 from component-wise application of Min (Math.Min(lhs, rhs)).
        /// </summary>
        public static decVector4 Min(decVector4 lhs, decVector4 rhs) => new decVector4(Math.Min(lhs.x, rhs.x), Math.Min(lhs.y, rhs.y), Math.Min(lhs.z, rhs.z), Math.Min(lhs.w, rhs.w));
        
        /// <summary>
        /// Returns a decVector4 from component-wise application of Min (Math.Min(lhs, rhs)).
        /// </summary>
        public static decVector4 Min(decVector4 lhs, decimal rhs) => new decVector4(Math.Min(lhs.x, rhs), Math.Min(lhs.y, rhs), Math.Min(lhs.z, rhs), Math.Min(lhs.w, rhs));
        
        /// <summary>
        /// Returns a decVector4 from component-wise application of Min (Math.Min(lhs, rhs)).
        /// </summary>
        public static decVector4 Min(decimal lhs, decVector4 rhs) => new decVector4(Math.Min(lhs, rhs.x), Math.Min(lhs, rhs.y), Math.Min(lhs, rhs.z), Math.Min(lhs, rhs.w));
        
        /// <summary>
        /// Returns a decVector from the application of Min (Math.Min(lhs, rhs)).
        /// </summary>
        public static decVector4 Min(decimal lhs, decimal rhs) => new decVector4(Math.Min(lhs, rhs));
        
        /// <summary>
        /// Returns a decVector4 from component-wise application of Pow ((decimal)Math.Pow((double)lhs, (double)rhs)).
        /// </summary>
        public static decVector4 Pow(decVector4 lhs, decVector4 rhs) => new decVector4((decimal)Math.Pow((double)lhs.x, (double)rhs.x), (decimal)Math.Pow((double)lhs.y, (double)rhs.y), (decimal)Math.Pow((double)lhs.z, (double)rhs.z), (decimal)Math.Pow((double)lhs.w, (double)rhs.w));
        
        /// <summary>
        /// Returns a decVector4 from component-wise application of Pow ((decimal)Math.Pow((double)lhs, (double)rhs)).
        /// </summary>
        public static decVector4 Pow(decVector4 lhs, decimal rhs) => new decVector4((decimal)Math.Pow((double)lhs.x, (double)rhs), (decimal)Math.Pow((double)lhs.y, (double)rhs), (decimal)Math.Pow((double)lhs.z, (double)rhs), (decimal)Math.Pow((double)lhs.w, (double)rhs));
        
        /// <summary>
        /// Returns a decVector4 from component-wise application of Pow ((decimal)Math.Pow((double)lhs, (double)rhs)).
        /// </summary>
        public static decVector4 Pow(decimal lhs, decVector4 rhs) => new decVector4((decimal)Math.Pow((double)lhs, (double)rhs.x), (decimal)Math.Pow((double)lhs, (double)rhs.y), (decimal)Math.Pow((double)lhs, (double)rhs.z), (decimal)Math.Pow((double)lhs, (double)rhs.w));
        
        /// <summary>
        /// Returns a decVector from the application of Pow ((decimal)Math.Pow((double)lhs, (double)rhs)).
        /// </summary>
        public static decVector4 Pow(decimal lhs, decimal rhs) => new decVector4((decimal)Math.Pow((double)lhs, (double)rhs));
        
        /// <summary>
        /// Returns a decVector4 from component-wise application of Log ((decimal)Math.Log((double)lhs, (double)rhs)).
        /// </summary>
        public static decVector4 Log(decVector4 lhs, decVector4 rhs) => new decVector4((decimal)Math.Log((double)lhs.x, (double)rhs.x), (decimal)Math.Log((double)lhs.y, (double)rhs.y), (decimal)Math.Log((double)lhs.z, (double)rhs.z), (decimal)Math.Log((double)lhs.w, (double)rhs.w));
        
        /// <summary>
        /// Returns a decVector4 from component-wise application of Log ((decimal)Math.Log((double)lhs, (double)rhs)).
        /// </summary>
        public static decVector4 Log(decVector4 lhs, decimal rhs) => new decVector4((decimal)Math.Log((double)lhs.x, (double)rhs), (decimal)Math.Log((double)lhs.y, (double)rhs), (decimal)Math.Log((double)lhs.z, (double)rhs), (decimal)Math.Log((double)lhs.w, (double)rhs));
        
        /// <summary>
        /// Returns a decVector4 from component-wise application of Log ((decimal)Math.Log((double)lhs, (double)rhs)).
        /// </summary>
        public static decVector4 Log(decimal lhs, decVector4 rhs) => new decVector4((decimal)Math.Log((double)lhs, (double)rhs.x), (decimal)Math.Log((double)lhs, (double)rhs.y), (decimal)Math.Log((double)lhs, (double)rhs.z), (decimal)Math.Log((double)lhs, (double)rhs.w));
        
        /// <summary>
        /// Returns a decVector from the application of Log ((decimal)Math.Log((double)lhs, (double)rhs)).
        /// </summary>
        public static decVector4 Log(decimal lhs, decimal rhs) => new decVector4((decimal)Math.Log((double)lhs, (double)rhs));
        
        /// <summary>
        /// Returns a decVector4 from component-wise application of Clamp (Math.Min(Math.Max(v, min), max)).
        /// </summary>
        public static decVector4 Clamp(decVector4 v, decVector4 min, decVector4 max) => new decVector4(Math.Min(Math.Max(v.x, min.x), max.x), Math.Min(Math.Max(v.y, min.y), max.y), Math.Min(Math.Max(v.z, min.z), max.z), Math.Min(Math.Max(v.w, min.w), max.w));
        
        /// <summary>
        /// Returns a decVector4 from component-wise application of Clamp (Math.Min(Math.Max(v, min), max)).
        /// </summary>
        public static decVector4 Clamp(decVector4 v, decVector4 min, decimal max) => new decVector4(Math.Min(Math.Max(v.x, min.x), max), Math.Min(Math.Max(v.y, min.y), max), Math.Min(Math.Max(v.z, min.z), max), Math.Min(Math.Max(v.w, min.w), max));
        
        /// <summary>
        /// Returns a decVector4 from component-wise application of Clamp (Math.Min(Math.Max(v, min), max)).
        /// </summary>
        public static decVector4 Clamp(decVector4 v, decimal min, decVector4 max) => new decVector4(Math.Min(Math.Max(v.x, min), max.x), Math.Min(Math.Max(v.y, min), max.y), Math.Min(Math.Max(v.z, min), max.z), Math.Min(Math.Max(v.w, min), max.w));
        
        /// <summary>
        /// Returns a decVector4 from component-wise application of Clamp (Math.Min(Math.Max(v, min), max)).
        /// </summary>
        public static decVector4 Clamp(decVector4 v, decimal min, decimal max) => new decVector4(Math.Min(Math.Max(v.x, min), max), Math.Min(Math.Max(v.y, min), max), Math.Min(Math.Max(v.z, min), max), Math.Min(Math.Max(v.w, min), max));
        
        /// <summary>
        /// Returns a decVector4 from component-wise application of Clamp (Math.Min(Math.Max(v, min), max)).
        /// </summary>
        public static decVector4 Clamp(decimal v, decVector4 min, decVector4 max) => new decVector4(Math.Min(Math.Max(v, min.x), max.x), Math.Min(Math.Max(v, min.y), max.y), Math.Min(Math.Max(v, min.z), max.z), Math.Min(Math.Max(v, min.w), max.w));
        
        /// <summary>
        /// Returns a decVector4 from component-wise application of Clamp (Math.Min(Math.Max(v, min), max)).
        /// </summary>
        public static decVector4 Clamp(decimal v, decVector4 min, decimal max) => new decVector4(Math.Min(Math.Max(v, min.x), max), Math.Min(Math.Max(v, min.y), max), Math.Min(Math.Max(v, min.z), max), Math.Min(Math.Max(v, min.w), max));
        
        /// <summary>
        /// Returns a decVector4 from component-wise application of Clamp (Math.Min(Math.Max(v, min), max)).
        /// </summary>
        public static decVector4 Clamp(decimal v, decimal min, decVector4 max) => new decVector4(Math.Min(Math.Max(v, min), max.x), Math.Min(Math.Max(v, min), max.y), Math.Min(Math.Max(v, min), max.z), Math.Min(Math.Max(v, min), max.w));
        
        /// <summary>
        /// Returns a decVector from the application of Clamp (Math.Min(Math.Max(v, min), max)).
        /// </summary>
        public static decVector4 Clamp(decimal v, decimal min, decimal max) => new decVector4(Math.Min(Math.Max(v, min), max));
        
        /// <summary>
        /// Returns a decVector4 from component-wise application of Mix (min * (1-a) + max * a).
        /// </summary>
        public static decVector4 Mix(decVector4 min, decVector4 max, decVector4 a) => new decVector4(min.x * (1-a.x) + max.x * a.x, min.y * (1-a.y) + max.y * a.y, min.z * (1-a.z) + max.z * a.z, min.w * (1-a.w) + max.w * a.w);
        
        /// <summary>
        /// Returns a decVector4 from component-wise application of Mix (min * (1-a) + max * a).
        /// </summary>
        public static decVector4 Mix(decVector4 min, decVector4 max, decimal a) => new decVector4(min.x * (1-a) + max.x * a, min.y * (1-a) + max.y * a, min.z * (1-a) + max.z * a, min.w * (1-a) + max.w * a);
        
        /// <summary>
        /// Returns a decVector4 from component-wise application of Mix (min * (1-a) + max * a).
        /// </summary>
        public static decVector4 Mix(decVector4 min, decimal max, decVector4 a) => new decVector4(min.x * (1-a.x) + max * a.x, min.y * (1-a.y) + max * a.y, min.z * (1-a.z) + max * a.z, min.w * (1-a.w) + max * a.w);
        
        /// <summary>
        /// Returns a decVector4 from component-wise application of Mix (min * (1-a) + max * a).
        /// </summary>
        public static decVector4 Mix(decVector4 min, decimal max, decimal a) => new decVector4(min.x * (1-a) + max * a, min.y * (1-a) + max * a, min.z * (1-a) + max * a, min.w * (1-a) + max * a);
        
        /// <summary>
        /// Returns a decVector4 from component-wise application of Mix (min * (1-a) + max * a).
        /// </summary>
        public static decVector4 Mix(decimal min, decVector4 max, decVector4 a) => new decVector4(min * (1-a.x) + max.x * a.x, min * (1-a.y) + max.y * a.y, min * (1-a.z) + max.z * a.z, min * (1-a.w) + max.w * a.w);
        
        /// <summary>
        /// Returns a decVector4 from component-wise application of Mix (min * (1-a) + max * a).
        /// </summary>
        public static decVector4 Mix(decimal min, decVector4 max, decimal a) => new decVector4(min * (1-a) + max.x * a, min * (1-a) + max.y * a, min * (1-a) + max.z * a, min * (1-a) + max.w * a);
        
        /// <summary>
        /// Returns a decVector4 from component-wise application of Mix (min * (1-a) + max * a).
        /// </summary>
        public static decVector4 Mix(decimal min, decimal max, decVector4 a) => new decVector4(min * (1-a.x) + max * a.x, min * (1-a.y) + max * a.y, min * (1-a.z) + max * a.z, min * (1-a.w) + max * a.w);
        
        /// <summary>
        /// Returns a decVector from the application of Mix (min * (1-a) + max * a).
        /// </summary>
        public static decVector4 Mix(decimal min, decimal max, decimal a) => new decVector4(min * (1-a) + max * a);
        
        /// <summary>
        /// Returns a decVector4 from component-wise application of Lerp (min * (1-a) + max * a).
        /// </summary>
        public static decVector4 Lerp(decVector4 min, decVector4 max, decVector4 a) => new decVector4(min.x * (1-a.x) + max.x * a.x, min.y * (1-a.y) + max.y * a.y, min.z * (1-a.z) + max.z * a.z, min.w * (1-a.w) + max.w * a.w);
        
        /// <summary>
        /// Returns a decVector4 from component-wise application of Lerp (min * (1-a) + max * a).
        /// </summary>
        public static decVector4 Lerp(decVector4 min, decVector4 max, decimal a) => new decVector4(min.x * (1-a) + max.x * a, min.y * (1-a) + max.y * a, min.z * (1-a) + max.z * a, min.w * (1-a) + max.w * a);
        
        /// <summary>
        /// Returns a decVector4 from component-wise application of Lerp (min * (1-a) + max * a).
        /// </summary>
        public static decVector4 Lerp(decVector4 min, decimal max, decVector4 a) => new decVector4(min.x * (1-a.x) + max * a.x, min.y * (1-a.y) + max * a.y, min.z * (1-a.z) + max * a.z, min.w * (1-a.w) + max * a.w);
        
        /// <summary>
        /// Returns a decVector4 from component-wise application of Lerp (min * (1-a) + max * a).
        /// </summary>
        public static decVector4 Lerp(decVector4 min, decimal max, decimal a) => new decVector4(min.x * (1-a) + max * a, min.y * (1-a) + max * a, min.z * (1-a) + max * a, min.w * (1-a) + max * a);
        
        /// <summary>
        /// Returns a decVector4 from component-wise application of Lerp (min * (1-a) + max * a).
        /// </summary>
        public static decVector4 Lerp(decimal min, decVector4 max, decVector4 a) => new decVector4(min * (1-a.x) + max.x * a.x, min * (1-a.y) + max.y * a.y, min * (1-a.z) + max.z * a.z, min * (1-a.w) + max.w * a.w);
        
        /// <summary>
        /// Returns a decVector4 from component-wise application of Lerp (min * (1-a) + max * a).
        /// </summary>
        public static decVector4 Lerp(decimal min, decVector4 max, decimal a) => new decVector4(min * (1-a) + max.x * a, min * (1-a) + max.y * a, min * (1-a) + max.z * a, min * (1-a) + max.w * a);
        
        /// <summary>
        /// Returns a decVector4 from component-wise application of Lerp (min * (1-a) + max * a).
        /// </summary>
        public static decVector4 Lerp(decimal min, decimal max, decVector4 a) => new decVector4(min * (1-a.x) + max * a.x, min * (1-a.y) + max * a.y, min * (1-a.z) + max * a.z, min * (1-a.w) + max * a.w);
        
        /// <summary>
        /// Returns a decVector from the application of Lerp (min * (1-a) + max * a).
        /// </summary>
        public static decVector4 Lerp(decimal min, decimal max, decimal a) => new decVector4(min * (1-a) + max * a);
        
        /// <summary>
        /// Returns a decVector4 from component-wise application of Smoothstep (((v - edge0) / (edge1 - edge0)).Clamp().HermiteInterpolationOrder3()).
        /// </summary>
        public static decVector4 Smoothstep(decVector4 edge0, decVector4 edge1, decVector4 v) => new decVector4(((v.x - edge0.x) / (edge1.x - edge0.x)).Clamp().HermiteInterpolationOrder3(), ((v.y - edge0.y) / (edge1.y - edge0.y)).Clamp().HermiteInterpolationOrder3(), ((v.z - edge0.z) / (edge1.z - edge0.z)).Clamp().HermiteInterpolationOrder3(), ((v.w - edge0.w) / (edge1.w - edge0.w)).Clamp().HermiteInterpolationOrder3());
        
        /// <summary>
        /// Returns a decVector4 from component-wise application of Smoothstep (((v - edge0) / (edge1 - edge0)).Clamp().HermiteInterpolationOrder3()).
        /// </summary>
        public static decVector4 Smoothstep(decVector4 edge0, decVector4 edge1, decimal v) => new decVector4(((v - edge0.x) / (edge1.x - edge0.x)).Clamp().HermiteInterpolationOrder3(), ((v - edge0.y) / (edge1.y - edge0.y)).Clamp().HermiteInterpolationOrder3(), ((v - edge0.z) / (edge1.z - edge0.z)).Clamp().HermiteInterpolationOrder3(), ((v - edge0.w) / (edge1.w - edge0.w)).Clamp().HermiteInterpolationOrder3());
        
        /// <summary>
        /// Returns a decVector4 from component-wise application of Smoothstep (((v - edge0) / (edge1 - edge0)).Clamp().HermiteInterpolationOrder3()).
        /// </summary>
        public static decVector4 Smoothstep(decVector4 edge0, decimal edge1, decVector4 v) => new decVector4(((v.x - edge0.x) / (edge1 - edge0.x)).Clamp().HermiteInterpolationOrder3(), ((v.y - edge0.y) / (edge1 - edge0.y)).Clamp().HermiteInterpolationOrder3(), ((v.z - edge0.z) / (edge1 - edge0.z)).Clamp().HermiteInterpolationOrder3(), ((v.w - edge0.w) / (edge1 - edge0.w)).Clamp().HermiteInterpolationOrder3());
        
        /// <summary>
        /// Returns a decVector4 from component-wise application of Smoothstep (((v - edge0) / (edge1 - edge0)).Clamp().HermiteInterpolationOrder3()).
        /// </summary>
        public static decVector4 Smoothstep(decVector4 edge0, decimal edge1, decimal v) => new decVector4(((v - edge0.x) / (edge1 - edge0.x)).Clamp().HermiteInterpolationOrder3(), ((v - edge0.y) / (edge1 - edge0.y)).Clamp().HermiteInterpolationOrder3(), ((v - edge0.z) / (edge1 - edge0.z)).Clamp().HermiteInterpolationOrder3(), ((v - edge0.w) / (edge1 - edge0.w)).Clamp().HermiteInterpolationOrder3());
        
        /// <summary>
        /// Returns a decVector4 from component-wise application of Smoothstep (((v - edge0) / (edge1 - edge0)).Clamp().HermiteInterpolationOrder3()).
        /// </summary>
        public static decVector4 Smoothstep(decimal edge0, decVector4 edge1, decVector4 v) => new decVector4(((v.x - edge0) / (edge1.x - edge0)).Clamp().HermiteInterpolationOrder3(), ((v.y - edge0) / (edge1.y - edge0)).Clamp().HermiteInterpolationOrder3(), ((v.z - edge0) / (edge1.z - edge0)).Clamp().HermiteInterpolationOrder3(), ((v.w - edge0) / (edge1.w - edge0)).Clamp().HermiteInterpolationOrder3());
        
        /// <summary>
        /// Returns a decVector4 from component-wise application of Smoothstep (((v - edge0) / (edge1 - edge0)).Clamp().HermiteInterpolationOrder3()).
        /// </summary>
        public static decVector4 Smoothstep(decimal edge0, decVector4 edge1, decimal v) => new decVector4(((v - edge0) / (edge1.x - edge0)).Clamp().HermiteInterpolationOrder3(), ((v - edge0) / (edge1.y - edge0)).Clamp().HermiteInterpolationOrder3(), ((v - edge0) / (edge1.z - edge0)).Clamp().HermiteInterpolationOrder3(), ((v - edge0) / (edge1.w - edge0)).Clamp().HermiteInterpolationOrder3());
        
        /// <summary>
        /// Returns a decVector4 from component-wise application of Smoothstep (((v - edge0) / (edge1 - edge0)).Clamp().HermiteInterpolationOrder3()).
        /// </summary>
        public static decVector4 Smoothstep(decimal edge0, decimal edge1, decVector4 v) => new decVector4(((v.x - edge0) / (edge1 - edge0)).Clamp().HermiteInterpolationOrder3(), ((v.y - edge0) / (edge1 - edge0)).Clamp().HermiteInterpolationOrder3(), ((v.z - edge0) / (edge1 - edge0)).Clamp().HermiteInterpolationOrder3(), ((v.w - edge0) / (edge1 - edge0)).Clamp().HermiteInterpolationOrder3());
        
        /// <summary>
        /// Returns a decVector from the application of Smoothstep (((v - edge0) / (edge1 - edge0)).Clamp().HermiteInterpolationOrder3()).
        /// </summary>
        public static decVector4 Smoothstep(decimal edge0, decimal edge1, decimal v) => new decVector4(((v - edge0) / (edge1 - edge0)).Clamp().HermiteInterpolationOrder3());
        
        /// <summary>
        /// Returns a decVector4 from component-wise application of Smootherstep (((v - edge0) / (edge1 - edge0)).Clamp().HermiteInterpolationOrder5()).
        /// </summary>
        public static decVector4 Smootherstep(decVector4 edge0, decVector4 edge1, decVector4 v) => new decVector4(((v.x - edge0.x) / (edge1.x - edge0.x)).Clamp().HermiteInterpolationOrder5(), ((v.y - edge0.y) / (edge1.y - edge0.y)).Clamp().HermiteInterpolationOrder5(), ((v.z - edge0.z) / (edge1.z - edge0.z)).Clamp().HermiteInterpolationOrder5(), ((v.w - edge0.w) / (edge1.w - edge0.w)).Clamp().HermiteInterpolationOrder5());
        
        /// <summary>
        /// Returns a decVector4 from component-wise application of Smootherstep (((v - edge0) / (edge1 - edge0)).Clamp().HermiteInterpolationOrder5()).
        /// </summary>
        public static decVector4 Smootherstep(decVector4 edge0, decVector4 edge1, decimal v) => new decVector4(((v - edge0.x) / (edge1.x - edge0.x)).Clamp().HermiteInterpolationOrder5(), ((v - edge0.y) / (edge1.y - edge0.y)).Clamp().HermiteInterpolationOrder5(), ((v - edge0.z) / (edge1.z - edge0.z)).Clamp().HermiteInterpolationOrder5(), ((v - edge0.w) / (edge1.w - edge0.w)).Clamp().HermiteInterpolationOrder5());
        
        /// <summary>
        /// Returns a decVector4 from component-wise application of Smootherstep (((v - edge0) / (edge1 - edge0)).Clamp().HermiteInterpolationOrder5()).
        /// </summary>
        public static decVector4 Smootherstep(decVector4 edge0, decimal edge1, decVector4 v) => new decVector4(((v.x - edge0.x) / (edge1 - edge0.x)).Clamp().HermiteInterpolationOrder5(), ((v.y - edge0.y) / (edge1 - edge0.y)).Clamp().HermiteInterpolationOrder5(), ((v.z - edge0.z) / (edge1 - edge0.z)).Clamp().HermiteInterpolationOrder5(), ((v.w - edge0.w) / (edge1 - edge0.w)).Clamp().HermiteInterpolationOrder5());
        
        /// <summary>
        /// Returns a decVector4 from component-wise application of Smootherstep (((v - edge0) / (edge1 - edge0)).Clamp().HermiteInterpolationOrder5()).
        /// </summary>
        public static decVector4 Smootherstep(decVector4 edge0, decimal edge1, decimal v) => new decVector4(((v - edge0.x) / (edge1 - edge0.x)).Clamp().HermiteInterpolationOrder5(), ((v - edge0.y) / (edge1 - edge0.y)).Clamp().HermiteInterpolationOrder5(), ((v - edge0.z) / (edge1 - edge0.z)).Clamp().HermiteInterpolationOrder5(), ((v - edge0.w) / (edge1 - edge0.w)).Clamp().HermiteInterpolationOrder5());
        
        /// <summary>
        /// Returns a decVector4 from component-wise application of Smootherstep (((v - edge0) / (edge1 - edge0)).Clamp().HermiteInterpolationOrder5()).
        /// </summary>
        public static decVector4 Smootherstep(decimal edge0, decVector4 edge1, decVector4 v) => new decVector4(((v.x - edge0) / (edge1.x - edge0)).Clamp().HermiteInterpolationOrder5(), ((v.y - edge0) / (edge1.y - edge0)).Clamp().HermiteInterpolationOrder5(), ((v.z - edge0) / (edge1.z - edge0)).Clamp().HermiteInterpolationOrder5(), ((v.w - edge0) / (edge1.w - edge0)).Clamp().HermiteInterpolationOrder5());
        
        /// <summary>
        /// Returns a decVector4 from component-wise application of Smootherstep (((v - edge0) / (edge1 - edge0)).Clamp().HermiteInterpolationOrder5()).
        /// </summary>
        public static decVector4 Smootherstep(decimal edge0, decVector4 edge1, decimal v) => new decVector4(((v - edge0) / (edge1.x - edge0)).Clamp().HermiteInterpolationOrder5(), ((v - edge0) / (edge1.y - edge0)).Clamp().HermiteInterpolationOrder5(), ((v - edge0) / (edge1.z - edge0)).Clamp().HermiteInterpolationOrder5(), ((v - edge0) / (edge1.w - edge0)).Clamp().HermiteInterpolationOrder5());
        
        /// <summary>
        /// Returns a decVector4 from component-wise application of Smootherstep (((v - edge0) / (edge1 - edge0)).Clamp().HermiteInterpolationOrder5()).
        /// </summary>
        public static decVector4 Smootherstep(decimal edge0, decimal edge1, decVector4 v) => new decVector4(((v.x - edge0) / (edge1 - edge0)).Clamp().HermiteInterpolationOrder5(), ((v.y - edge0) / (edge1 - edge0)).Clamp().HermiteInterpolationOrder5(), ((v.z - edge0) / (edge1 - edge0)).Clamp().HermiteInterpolationOrder5(), ((v.w - edge0) / (edge1 - edge0)).Clamp().HermiteInterpolationOrder5());
        
        /// <summary>
        /// Returns a decVector from the application of Smootherstep (((v - edge0) / (edge1 - edge0)).Clamp().HermiteInterpolationOrder5()).
        /// </summary>
        public static decVector4 Smootherstep(decimal edge0, decimal edge1, decimal v) => new decVector4(((v - edge0) / (edge1 - edge0)).Clamp().HermiteInterpolationOrder5());
        
        /// <summary>
        /// Returns a decVector4 from component-wise application of Fma (a * b + c).
        /// </summary>
        public static decVector4 Fma(decVector4 a, decVector4 b, decVector4 c) => new decVector4(a.x * b.x + c.x, a.y * b.y + c.y, a.z * b.z + c.z, a.w * b.w + c.w);
        
        /// <summary>
        /// Returns a decVector4 from component-wise application of Fma (a * b + c).
        /// </summary>
        public static decVector4 Fma(decVector4 a, decVector4 b, decimal c) => new decVector4(a.x * b.x + c, a.y * b.y + c, a.z * b.z + c, a.w * b.w + c);
        
        /// <summary>
        /// Returns a decVector4 from component-wise application of Fma (a * b + c).
        /// </summary>
        public static decVector4 Fma(decVector4 a, decimal b, decVector4 c) => new decVector4(a.x * b + c.x, a.y * b + c.y, a.z * b + c.z, a.w * b + c.w);
        
        /// <summary>
        /// Returns a decVector4 from component-wise application of Fma (a * b + c).
        /// </summary>
        public static decVector4 Fma(decVector4 a, decimal b, decimal c) => new decVector4(a.x * b + c, a.y * b + c, a.z * b + c, a.w * b + c);
        
        /// <summary>
        /// Returns a decVector4 from component-wise application of Fma (a * b + c).
        /// </summary>
        public static decVector4 Fma(decimal a, decVector4 b, decVector4 c) => new decVector4(a * b.x + c.x, a * b.y + c.y, a * b.z + c.z, a * b.w + c.w);
        
        /// <summary>
        /// Returns a decVector4 from component-wise application of Fma (a * b + c).
        /// </summary>
        public static decVector4 Fma(decimal a, decVector4 b, decimal c) => new decVector4(a * b.x + c, a * b.y + c, a * b.z + c, a * b.w + c);
        
        /// <summary>
        /// Returns a decVector4 from component-wise application of Fma (a * b + c).
        /// </summary>
        public static decVector4 Fma(decimal a, decimal b, decVector4 c) => new decVector4(a * b + c.x, a * b + c.y, a * b + c.z, a * b + c.w);
        
        /// <summary>
        /// Returns a decVector from the application of Fma (a * b + c).
        /// </summary>
        public static decVector4 Fma(decimal a, decimal b, decimal c) => new decVector4(a * b + c);
        
        /// <summary>
        /// Returns a decVector4 from component-wise application of Add (lhs + rhs).
        /// </summary>
        public static decVector4 Add(decVector4 lhs, decVector4 rhs) => new decVector4(lhs.x + rhs.x, lhs.y + rhs.y, lhs.z + rhs.z, lhs.w + rhs.w);
        
        /// <summary>
        /// Returns a decVector4 from component-wise application of Add (lhs + rhs).
        /// </summary>
        public static decVector4 Add(decVector4 lhs, decimal rhs) => new decVector4(lhs.x + rhs, lhs.y + rhs, lhs.z + rhs, lhs.w + rhs);
        
        /// <summary>
        /// Returns a decVector4 from component-wise application of Add (lhs + rhs).
        /// </summary>
        public static decVector4 Add(decimal lhs, decVector4 rhs) => new decVector4(lhs + rhs.x, lhs + rhs.y, lhs + rhs.z, lhs + rhs.w);
        
        /// <summary>
        /// Returns a decVector from the application of Add (lhs + rhs).
        /// </summary>
        public static decVector4 Add(decimal lhs, decimal rhs) => new decVector4(lhs + rhs);
        
        /// <summary>
        /// Returns a decVector4 from component-wise application of Sub (lhs - rhs).
        /// </summary>
        public static decVector4 Sub(decVector4 lhs, decVector4 rhs) => new decVector4(lhs.x - rhs.x, lhs.y - rhs.y, lhs.z - rhs.z, lhs.w - rhs.w);
        
        /// <summary>
        /// Returns a decVector4 from component-wise application of Sub (lhs - rhs).
        /// </summary>
        public static decVector4 Sub(decVector4 lhs, decimal rhs) => new decVector4(lhs.x - rhs, lhs.y - rhs, lhs.z - rhs, lhs.w - rhs);
        
        /// <summary>
        /// Returns a decVector4 from component-wise application of Sub (lhs - rhs).
        /// </summary>
        public static decVector4 Sub(decimal lhs, decVector4 rhs) => new decVector4(lhs - rhs.x, lhs - rhs.y, lhs - rhs.z, lhs - rhs.w);
        
        /// <summary>
        /// Returns a decVector from the application of Sub (lhs - rhs).
        /// </summary>
        public static decVector4 Sub(decimal lhs, decimal rhs) => new decVector4(lhs - rhs);
        
        /// <summary>
        /// Returns a decVector4 from component-wise application of Mul (lhs * rhs).
        /// </summary>
        public static decVector4 Mul(decVector4 lhs, decVector4 rhs) => new decVector4(lhs.x * rhs.x, lhs.y * rhs.y, lhs.z * rhs.z, lhs.w * rhs.w);
        
        /// <summary>
        /// Returns a decVector4 from component-wise application of Mul (lhs * rhs).
        /// </summary>
        public static decVector4 Mul(decVector4 lhs, decimal rhs) => new decVector4(lhs.x * rhs, lhs.y * rhs, lhs.z * rhs, lhs.w * rhs);
        
        /// <summary>
        /// Returns a decVector4 from component-wise application of Mul (lhs * rhs).
        /// </summary>
        public static decVector4 Mul(decimal lhs, decVector4 rhs) => new decVector4(lhs * rhs.x, lhs * rhs.y, lhs * rhs.z, lhs * rhs.w);
        
        /// <summary>
        /// Returns a decVector from the application of Mul (lhs * rhs).
        /// </summary>
        public static decVector4 Mul(decimal lhs, decimal rhs) => new decVector4(lhs * rhs);
        
        /// <summary>
        /// Returns a decVector4 from component-wise application of Div (lhs / rhs).
        /// </summary>
        public static decVector4 Div(decVector4 lhs, decVector4 rhs) => new decVector4(lhs.x / rhs.x, lhs.y / rhs.y, lhs.z / rhs.z, lhs.w / rhs.w);
        
        /// <summary>
        /// Returns a decVector4 from component-wise application of Div (lhs / rhs).
        /// </summary>
        public static decVector4 Div(decVector4 lhs, decimal rhs) => new decVector4(lhs.x / rhs, lhs.y / rhs, lhs.z / rhs, lhs.w / rhs);
        
        /// <summary>
        /// Returns a decVector4 from component-wise application of Div (lhs / rhs).
        /// </summary>
        public static decVector4 Div(decimal lhs, decVector4 rhs) => new decVector4(lhs / rhs.x, lhs / rhs.y, lhs / rhs.z, lhs / rhs.w);
        
        /// <summary>
        /// Returns a decVector from the application of Div (lhs / rhs).
        /// </summary>
        public static decVector4 Div(decimal lhs, decimal rhs) => new decVector4(lhs / rhs);
        
        /// <summary>
        /// Returns a decVector4 from component-wise application of Modulo (lhs % rhs).
        /// </summary>
        public static decVector4 Modulo(decVector4 lhs, decVector4 rhs) => new decVector4(lhs.x % rhs.x, lhs.y % rhs.y, lhs.z % rhs.z, lhs.w % rhs.w);
        
        /// <summary>
        /// Returns a decVector4 from component-wise application of Modulo (lhs % rhs).
        /// </summary>
        public static decVector4 Modulo(decVector4 lhs, decimal rhs) => new decVector4(lhs.x % rhs, lhs.y % rhs, lhs.z % rhs, lhs.w % rhs);
        
        /// <summary>
        /// Returns a decVector4 from component-wise application of Modulo (lhs % rhs).
        /// </summary>
        public static decVector4 Modulo(decimal lhs, decVector4 rhs) => new decVector4(lhs % rhs.x, lhs % rhs.y, lhs % rhs.z, lhs % rhs.w);
        
        /// <summary>
        /// Returns a decVector from the application of Modulo (lhs % rhs).
        /// </summary>
        public static decVector4 Modulo(decimal lhs, decimal rhs) => new decVector4(lhs % rhs);
        
        /// <summary>
        /// Returns a decVector4 from component-wise application of Degrees (Radians-To-Degrees Conversion).
        /// </summary>
        public static decVector4 Degrees(decVector4 v) => new decVector4((decimal)(v.x * 57.295779513082320876798154814105170332405472466564321m), (decimal)(v.y * 57.295779513082320876798154814105170332405472466564321m), (decimal)(v.z * 57.295779513082320876798154814105170332405472466564321m), (decimal)(v.w * 57.295779513082320876798154814105170332405472466564321m));
        
        /// <summary>
        /// Returns a decVector from the application of Degrees (Radians-To-Degrees Conversion).
        /// </summary>
        public static decVector4 Degrees(decimal v) => new decVector4((decimal)(v * 57.295779513082320876798154814105170332405472466564321m));
        
        /// <summary>
        /// Returns a decVector4 from component-wise application of Radians (Degrees-To-Radians Conversion).
        /// </summary>
        public static decVector4 Radians(decVector4 v) => new decVector4((decimal)(v.x * 0.0174532925199432957692369076848861271344287188854172m), (decimal)(v.y * 0.0174532925199432957692369076848861271344287188854172m), (decimal)(v.z * 0.0174532925199432957692369076848861271344287188854172m), (decimal)(v.w * 0.0174532925199432957692369076848861271344287188854172m));
        
        /// <summary>
        /// Returns a decVector from the application of Radians (Degrees-To-Radians Conversion).
        /// </summary>
        public static decVector4 Radians(decimal v) => new decVector4((decimal)(v * 0.0174532925199432957692369076848861271344287188854172m));
        
        /// <summary>
        /// Returns a decVector4 from component-wise application of Acos ((decimal)Math.Acos((double)v)).
        /// </summary>
        public static decVector4 Acos(decVector4 v) => new decVector4((decimal)Math.Acos((double)v.x), (decimal)Math.Acos((double)v.y), (decimal)Math.Acos((double)v.z), (decimal)Math.Acos((double)v.w));
        
        /// <summary>
        /// Returns a decVector from the application of Acos ((decimal)Math.Acos((double)v)).
        /// </summary>
        public static decVector4 Acos(decimal v) => new decVector4((decimal)Math.Acos((double)v));
        
        /// <summary>
        /// Returns a decVector4 from component-wise application of Asin ((decimal)Math.Asin((double)v)).
        /// </summary>
        public static decVector4 Asin(decVector4 v) => new decVector4((decimal)Math.Asin((double)v.x), (decimal)Math.Asin((double)v.y), (decimal)Math.Asin((double)v.z), (decimal)Math.Asin((double)v.w));
        
        /// <summary>
        /// Returns a decVector from the application of Asin ((decimal)Math.Asin((double)v)).
        /// </summary>
        public static decVector4 Asin(decimal v) => new decVector4((decimal)Math.Asin((double)v));
        
        /// <summary>
        /// Returns a decVector4 from component-wise application of Atan ((decimal)Math.Atan((double)v)).
        /// </summary>
        public static decVector4 Atan(decVector4 v) => new decVector4((decimal)Math.Atan((double)v.x), (decimal)Math.Atan((double)v.y), (decimal)Math.Atan((double)v.z), (decimal)Math.Atan((double)v.w));
        
        /// <summary>
        /// Returns a decVector from the application of Atan ((decimal)Math.Atan((double)v)).
        /// </summary>
        public static decVector4 Atan(decimal v) => new decVector4((decimal)Math.Atan((double)v));
        
        /// <summary>
        /// Returns a decVector4 from component-wise application of Cos ((decimal)Math.Cos((double)v)).
        /// </summary>
        public static decVector4 Cos(decVector4 v) => new decVector4((decimal)Math.Cos((double)v.x), (decimal)Math.Cos((double)v.y), (decimal)Math.Cos((double)v.z), (decimal)Math.Cos((double)v.w));
        
        /// <summary>
        /// Returns a decVector from the application of Cos ((decimal)Math.Cos((double)v)).
        /// </summary>
        public static decVector4 Cos(decimal v) => new decVector4((decimal)Math.Cos((double)v));
        
        /// <summary>
        /// Returns a decVector4 from component-wise application of Cosh ((decimal)Math.Cosh((double)v)).
        /// </summary>
        public static decVector4 Cosh(decVector4 v) => new decVector4((decimal)Math.Cosh((double)v.x), (decimal)Math.Cosh((double)v.y), (decimal)Math.Cosh((double)v.z), (decimal)Math.Cosh((double)v.w));
        
        /// <summary>
        /// Returns a decVector from the application of Cosh ((decimal)Math.Cosh((double)v)).
        /// </summary>
        public static decVector4 Cosh(decimal v) => new decVector4((decimal)Math.Cosh((double)v));
        
        /// <summary>
        /// Returns a decVector4 from component-wise application of Exp ((decimal)Math.Exp((double)v)).
        /// </summary>
        public static decVector4 Exp(decVector4 v) => new decVector4((decimal)Math.Exp((double)v.x), (decimal)Math.Exp((double)v.y), (decimal)Math.Exp((double)v.z), (decimal)Math.Exp((double)v.w));
        
        /// <summary>
        /// Returns a decVector from the application of Exp ((decimal)Math.Exp((double)v)).
        /// </summary>
        public static decVector4 Exp(decimal v) => new decVector4((decimal)Math.Exp((double)v));
        
        /// <summary>
        /// Returns a decVector4 from component-wise application of Log ((decimal)Math.Log((double)v)).
        /// </summary>
        public static decVector4 Log(decVector4 v) => new decVector4((decimal)Math.Log((double)v.x), (decimal)Math.Log((double)v.y), (decimal)Math.Log((double)v.z), (decimal)Math.Log((double)v.w));
        
        /// <summary>
        /// Returns a decVector from the application of Log ((decimal)Math.Log((double)v)).
        /// </summary>
        public static decVector4 Log(decimal v) => new decVector4((decimal)Math.Log((double)v));
        
        /// <summary>
        /// Returns a decVector4 from component-wise application of Log2 ((decimal)Math.Log((double)v, 2)).
        /// </summary>
        public static decVector4 Log2(decVector4 v) => new decVector4((decimal)Math.Log((double)v.x, 2), (decimal)Math.Log((double)v.y, 2), (decimal)Math.Log((double)v.z, 2), (decimal)Math.Log((double)v.w, 2));
        
        /// <summary>
        /// Returns a decVector from the application of Log2 ((decimal)Math.Log((double)v, 2)).
        /// </summary>
        public static decVector4 Log2(decimal v) => new decVector4((decimal)Math.Log((double)v, 2));
        
        /// <summary>
        /// Returns a decVector4 from component-wise application of Log10 ((decimal)Math.Log10((double)v)).
        /// </summary>
        public static decVector4 Log10(decVector4 v) => new decVector4((decimal)Math.Log10((double)v.x), (decimal)Math.Log10((double)v.y), (decimal)Math.Log10((double)v.z), (decimal)Math.Log10((double)v.w));
        
        /// <summary>
        /// Returns a decVector from the application of Log10 ((decimal)Math.Log10((double)v)).
        /// </summary>
        public static decVector4 Log10(decimal v) => new decVector4((decimal)Math.Log10((double)v));
        
        /// <summary>
        /// Returns a decVector4 from component-wise application of Floor ((decimal)Math.Floor(v)).
        /// </summary>
        public static decVector4 Floor(decVector4 v) => new decVector4((decimal)Math.Floor(v.x), (decimal)Math.Floor(v.y), (decimal)Math.Floor(v.z), (decimal)Math.Floor(v.w));
        
        /// <summary>
        /// Returns a decVector from the application of Floor ((decimal)Math.Floor(v)).
        /// </summary>
        public static decVector4 Floor(decimal v) => new decVector4((decimal)Math.Floor(v));
        
        /// <summary>
        /// Returns a decVector4 from component-wise application of Ceiling ((decimal)Math.Ceiling(v)).
        /// </summary>
        public static decVector4 Ceiling(decVector4 v) => new decVector4((decimal)Math.Ceiling(v.x), (decimal)Math.Ceiling(v.y), (decimal)Math.Ceiling(v.z), (decimal)Math.Ceiling(v.w));
        
        /// <summary>
        /// Returns a decVector from the application of Ceiling ((decimal)Math.Ceiling(v)).
        /// </summary>
        public static decVector4 Ceiling(decimal v) => new decVector4((decimal)Math.Ceiling(v));
        
        /// <summary>
        /// Returns a decVector4 from component-wise application of Round ((decimal)Math.Round(v)).
        /// </summary>
        public static decVector4 Round(decVector4 v) => new decVector4((decimal)Math.Round(v.x), (decimal)Math.Round(v.y), (decimal)Math.Round(v.z), (decimal)Math.Round(v.w));
        
        /// <summary>
        /// Returns a decVector from the application of Round ((decimal)Math.Round(v)).
        /// </summary>
        public static decVector4 Round(decimal v) => new decVector4((decimal)Math.Round(v));
        
        /// <summary>
        /// Returns a decVector4 from component-wise application of Sin ((decimal)Math.Sin((double)v)).
        /// </summary>
        public static decVector4 Sin(decVector4 v) => new decVector4((decimal)Math.Sin((double)v.x), (decimal)Math.Sin((double)v.y), (decimal)Math.Sin((double)v.z), (decimal)Math.Sin((double)v.w));
        
        /// <summary>
        /// Returns a decVector from the application of Sin ((decimal)Math.Sin((double)v)).
        /// </summary>
        public static decVector4 Sin(decimal v) => new decVector4((decimal)Math.Sin((double)v));
        
        /// <summary>
        /// Returns a decVector4 from component-wise application of Sinh ((decimal)Math.Sinh((double)v)).
        /// </summary>
        public static decVector4 Sinh(decVector4 v) => new decVector4((decimal)Math.Sinh((double)v.x), (decimal)Math.Sinh((double)v.y), (decimal)Math.Sinh((double)v.z), (decimal)Math.Sinh((double)v.w));
        
        /// <summary>
        /// Returns a decVector from the application of Sinh ((decimal)Math.Sinh((double)v)).
        /// </summary>
        public static decVector4 Sinh(decimal v) => new decVector4((decimal)Math.Sinh((double)v));
        
        /// <summary>
        /// Returns a decVector4 from component-wise application of Tan ((decimal)Math.Tan((double)v)).
        /// </summary>
        public static decVector4 Tan(decVector4 v) => new decVector4((decimal)Math.Tan((double)v.x), (decimal)Math.Tan((double)v.y), (decimal)Math.Tan((double)v.z), (decimal)Math.Tan((double)v.w));
        
        /// <summary>
        /// Returns a decVector from the application of Tan ((decimal)Math.Tan((double)v)).
        /// </summary>
        public static decVector4 Tan(decimal v) => new decVector4((decimal)Math.Tan((double)v));
        
        /// <summary>
        /// Returns a decVector4 from component-wise application of Tanh ((decimal)Math.Tanh((double)v)).
        /// </summary>
        public static decVector4 Tanh(decVector4 v) => new decVector4((decimal)Math.Tanh((double)v.x), (decimal)Math.Tanh((double)v.y), (decimal)Math.Tanh((double)v.z), (decimal)Math.Tanh((double)v.w));
        
        /// <summary>
        /// Returns a decVector from the application of Tanh ((decimal)Math.Tanh((double)v)).
        /// </summary>
        public static decVector4 Tanh(decimal v) => new decVector4((decimal)Math.Tanh((double)v));
        
        /// <summary>
        /// Returns a decVector4 from component-wise application of Truncate ((decimal)Math.Truncate((double)v)).
        /// </summary>
        public static decVector4 Truncate(decVector4 v) => new decVector4((decimal)Math.Truncate((double)v.x), (decimal)Math.Truncate((double)v.y), (decimal)Math.Truncate((double)v.z), (decimal)Math.Truncate((double)v.w));
        
        /// <summary>
        /// Returns a decVector from the application of Truncate ((decimal)Math.Truncate((double)v)).
        /// </summary>
        public static decVector4 Truncate(decimal v) => new decVector4((decimal)Math.Truncate((double)v));
        
        /// <summary>
        /// Returns a decVector4 from component-wise application of Fract ((decimal)(v - Math.Floor(v))).
        /// </summary>
        public static decVector4 Fract(decVector4 v) => new decVector4((decimal)(v.x - Math.Floor(v.x)), (decimal)(v.y - Math.Floor(v.y)), (decimal)(v.z - Math.Floor(v.z)), (decimal)(v.w - Math.Floor(v.w)));
        
        /// <summary>
        /// Returns a decVector from the application of Fract ((decimal)(v - Math.Floor(v))).
        /// </summary>
        public static decVector4 Fract(decimal v) => new decVector4((decimal)(v - Math.Floor(v)));
        
        /// <summary>
        /// Returns a decVector4 from component-wise application of Trunc ((long)(v)).
        /// </summary>
        public static decVector4 Trunc(decVector4 v) => new decVector4((long)(v.x), (long)(v.y), (long)(v.z), (long)(v.w));
        
        /// <summary>
        /// Returns a decVector from the application of Trunc ((long)(v)).
        /// </summary>
        public static decVector4 Trunc(decimal v) => new decVector4((long)(v));
        
        /// <summary>
        /// Returns a decVector4 with independent and identically distributed uniform values between 'minValue' and 'maxValue'.
        /// </summary>
        public static decVector4 Random(System.Random random, decVector4 minValue, decVector4 maxValue) => new decVector4((decimal)random.NextDouble() * (maxValue.x - minValue.x) + minValue.x, (decimal)random.NextDouble() * (maxValue.y - minValue.y) + minValue.y, (decimal)random.NextDouble() * (maxValue.z - minValue.z) + minValue.z, (decimal)random.NextDouble() * (maxValue.w - minValue.w) + minValue.w);
        
        /// <summary>
        /// Returns a decVector4 with independent and identically distributed uniform values between 'minValue' and 'maxValue'.
        /// </summary>
        public static decVector4 Random(System.Random random, decVector4 minValue, decimal maxValue) => new decVector4((decimal)random.NextDouble() * (maxValue - minValue.x) + minValue.x, (decimal)random.NextDouble() * (maxValue - minValue.y) + minValue.y, (decimal)random.NextDouble() * (maxValue - minValue.z) + minValue.z, (decimal)random.NextDouble() * (maxValue - minValue.w) + minValue.w);
        
        /// <summary>
        /// Returns a decVector4 with independent and identically distributed uniform values between 'minValue' and 'maxValue'.
        /// </summary>
        public static decVector4 Random(System.Random random, decimal minValue, decVector4 maxValue) => new decVector4((decimal)random.NextDouble() * (maxValue.x - minValue) + minValue, (decimal)random.NextDouble() * (maxValue.y - minValue) + minValue, (decimal)random.NextDouble() * (maxValue.z - minValue) + minValue, (decimal)random.NextDouble() * (maxValue.w - minValue) + minValue);
        
        /// <summary>
        /// Returns a decVector4 with independent and identically distributed uniform values between 'minValue' and 'maxValue'.
        /// </summary>
        public static decVector4 Random(System.Random random, decimal minValue, decimal maxValue) => new decVector4((decimal)random.NextDouble() * (maxValue - minValue) + minValue);
        
        /// <summary>
        /// Returns a decVector4 with independent and identically distributed uniform values between 'minValue' and 'maxValue'.
        /// </summary>
        public static decVector4 RandomUniform(System.Random random, decVector4 minValue, decVector4 maxValue) => new decVector4((decimal)random.NextDouble() * (maxValue.x - minValue.x) + minValue.x, (decimal)random.NextDouble() * (maxValue.y - minValue.y) + minValue.y, (decimal)random.NextDouble() * (maxValue.z - minValue.z) + minValue.z, (decimal)random.NextDouble() * (maxValue.w - minValue.w) + minValue.w);
        
        /// <summary>
        /// Returns a decVector4 with independent and identically distributed uniform values between 'minValue' and 'maxValue'.
        /// </summary>
        public static decVector4 RandomUniform(System.Random random, decVector4 minValue, decimal maxValue) => new decVector4((decimal)random.NextDouble() * (maxValue - minValue.x) + minValue.x, (decimal)random.NextDouble() * (maxValue - minValue.y) + minValue.y, (decimal)random.NextDouble() * (maxValue - minValue.z) + minValue.z, (decimal)random.NextDouble() * (maxValue - minValue.w) + minValue.w);
        
        /// <summary>
        /// Returns a decVector4 with independent and identically distributed uniform values between 'minValue' and 'maxValue'.
        /// </summary>
        public static decVector4 RandomUniform(System.Random random, decimal minValue, decVector4 maxValue) => new decVector4((decimal)random.NextDouble() * (maxValue.x - minValue) + minValue, (decimal)random.NextDouble() * (maxValue.y - minValue) + minValue, (decimal)random.NextDouble() * (maxValue.z - minValue) + minValue, (decimal)random.NextDouble() * (maxValue.w - minValue) + minValue);
        
        /// <summary>
        /// Returns a decVector4 with independent and identically distributed uniform values between 'minValue' and 'maxValue'.
        /// </summary>
        public static decVector4 RandomUniform(System.Random random, decimal minValue, decimal maxValue) => new decVector4((decimal)random.NextDouble() * (maxValue - minValue) + minValue);
        
        /// <summary>
        /// Returns a decVector4 with independent and identically distributed values according to a normal/Gaussian distribution with specified mean and variance.
        /// </summary>
        public static decVector4 RandomNormal(System.Random random, decVector4 mean, decVector4 variance) => new decVector4((decimal)(Math.Sqrt((double)variance.x) * Math.Cos(2 * Math.PI * random.NextDouble()) * Math.Sqrt(-2.0 * Math.Log(random.NextDouble()))) + mean.x, (decimal)(Math.Sqrt((double)variance.y) * Math.Cos(2 * Math.PI * random.NextDouble()) * Math.Sqrt(-2.0 * Math.Log(random.NextDouble()))) + mean.y, (decimal)(Math.Sqrt((double)variance.z) * Math.Cos(2 * Math.PI * random.NextDouble()) * Math.Sqrt(-2.0 * Math.Log(random.NextDouble()))) + mean.z, (decimal)(Math.Sqrt((double)variance.w) * Math.Cos(2 * Math.PI * random.NextDouble()) * Math.Sqrt(-2.0 * Math.Log(random.NextDouble()))) + mean.w);
        
        /// <summary>
        /// Returns a decVector4 with independent and identically distributed values according to a normal/Gaussian distribution with specified mean and variance.
        /// </summary>
        public static decVector4 RandomNormal(System.Random random, decVector4 mean, decimal variance) => new decVector4((decimal)(Math.Sqrt((double)variance) * Math.Cos(2 * Math.PI * random.NextDouble()) * Math.Sqrt(-2.0 * Math.Log(random.NextDouble()))) + mean.x, (decimal)(Math.Sqrt((double)variance) * Math.Cos(2 * Math.PI * random.NextDouble()) * Math.Sqrt(-2.0 * Math.Log(random.NextDouble()))) + mean.y, (decimal)(Math.Sqrt((double)variance) * Math.Cos(2 * Math.PI * random.NextDouble()) * Math.Sqrt(-2.0 * Math.Log(random.NextDouble()))) + mean.z, (decimal)(Math.Sqrt((double)variance) * Math.Cos(2 * Math.PI * random.NextDouble()) * Math.Sqrt(-2.0 * Math.Log(random.NextDouble()))) + mean.w);
        
        /// <summary>
        /// Returns a decVector4 with independent and identically distributed values according to a normal/Gaussian distribution with specified mean and variance.
        /// </summary>
        public static decVector4 RandomNormal(System.Random random, decimal mean, decVector4 variance) => new decVector4((decimal)(Math.Sqrt((double)variance.x) * Math.Cos(2 * Math.PI * random.NextDouble()) * Math.Sqrt(-2.0 * Math.Log(random.NextDouble()))) + mean, (decimal)(Math.Sqrt((double)variance.y) * Math.Cos(2 * Math.PI * random.NextDouble()) * Math.Sqrt(-2.0 * Math.Log(random.NextDouble()))) + mean, (decimal)(Math.Sqrt((double)variance.z) * Math.Cos(2 * Math.PI * random.NextDouble()) * Math.Sqrt(-2.0 * Math.Log(random.NextDouble()))) + mean, (decimal)(Math.Sqrt((double)variance.w) * Math.Cos(2 * Math.PI * random.NextDouble()) * Math.Sqrt(-2.0 * Math.Log(random.NextDouble()))) + mean);
        
        /// <summary>
        /// Returns a decVector4 with independent and identically distributed values according to a normal/Gaussian distribution with specified mean and variance.
        /// </summary>
        public static decVector4 RandomNormal(System.Random random, decimal mean, decimal variance) => new decVector4((decimal)(Math.Sqrt((double)variance) * Math.Cos(2 * Math.PI * random.NextDouble()) * Math.Sqrt(-2.0 * Math.Log(random.NextDouble()))) + mean);
        
        /// <summary>
        /// Returns a decVector4 with independent and identically distributed values according to a normal/Gaussian distribution with specified mean and variance.
        /// </summary>
        public static decVector4 RandomGaussian(System.Random random, decVector4 mean, decVector4 variance) => new decVector4((decimal)(Math.Sqrt((double)variance.x) * Math.Cos(2 * Math.PI * random.NextDouble()) * Math.Sqrt(-2.0 * Math.Log(random.NextDouble()))) + mean.x, (decimal)(Math.Sqrt((double)variance.y) * Math.Cos(2 * Math.PI * random.NextDouble()) * Math.Sqrt(-2.0 * Math.Log(random.NextDouble()))) + mean.y, (decimal)(Math.Sqrt((double)variance.z) * Math.Cos(2 * Math.PI * random.NextDouble()) * Math.Sqrt(-2.0 * Math.Log(random.NextDouble()))) + mean.z, (decimal)(Math.Sqrt((double)variance.w) * Math.Cos(2 * Math.PI * random.NextDouble()) * Math.Sqrt(-2.0 * Math.Log(random.NextDouble()))) + mean.w);
        
        /// <summary>
        /// Returns a decVector4 with independent and identically distributed values according to a normal/Gaussian distribution with specified mean and variance.
        /// </summary>
        public static decVector4 RandomGaussian(System.Random random, decVector4 mean, decimal variance) => new decVector4((decimal)(Math.Sqrt((double)variance) * Math.Cos(2 * Math.PI * random.NextDouble()) * Math.Sqrt(-2.0 * Math.Log(random.NextDouble()))) + mean.x, (decimal)(Math.Sqrt((double)variance) * Math.Cos(2 * Math.PI * random.NextDouble()) * Math.Sqrt(-2.0 * Math.Log(random.NextDouble()))) + mean.y, (decimal)(Math.Sqrt((double)variance) * Math.Cos(2 * Math.PI * random.NextDouble()) * Math.Sqrt(-2.0 * Math.Log(random.NextDouble()))) + mean.z, (decimal)(Math.Sqrt((double)variance) * Math.Cos(2 * Math.PI * random.NextDouble()) * Math.Sqrt(-2.0 * Math.Log(random.NextDouble()))) + mean.w);
        
        /// <summary>
        /// Returns a decVector4 with independent and identically distributed values according to a normal/Gaussian distribution with specified mean and variance.
        /// </summary>
        public static decVector4 RandomGaussian(System.Random random, decimal mean, decVector4 variance) => new decVector4((decimal)(Math.Sqrt((double)variance.x) * Math.Cos(2 * Math.PI * random.NextDouble()) * Math.Sqrt(-2.0 * Math.Log(random.NextDouble()))) + mean, (decimal)(Math.Sqrt((double)variance.y) * Math.Cos(2 * Math.PI * random.NextDouble()) * Math.Sqrt(-2.0 * Math.Log(random.NextDouble()))) + mean, (decimal)(Math.Sqrt((double)variance.z) * Math.Cos(2 * Math.PI * random.NextDouble()) * Math.Sqrt(-2.0 * Math.Log(random.NextDouble()))) + mean, (decimal)(Math.Sqrt((double)variance.w) * Math.Cos(2 * Math.PI * random.NextDouble()) * Math.Sqrt(-2.0 * Math.Log(random.NextDouble()))) + mean);
        
        /// <summary>
        /// Returns a decVector4 with independent and identically distributed values according to a normal/Gaussian distribution with specified mean and variance.
        /// </summary>
        public static decVector4 RandomGaussian(System.Random random, decimal mean, decimal variance) => new decVector4((decimal)(Math.Sqrt((double)variance) * Math.Cos(2 * Math.PI * random.NextDouble()) * Math.Sqrt(-2.0 * Math.Log(random.NextDouble()))) + mean);

        #endregion


        #region Component-Wise Operator Overloads
        
        /// <summary>
        /// Returns a bVector4 from component-wise application of operator&lt; (lhs &lt; rhs).
        /// </summary>
        public static bVector4 operator<(decVector4 lhs, decVector4 rhs) => new bVector4(lhs.x < rhs.x, lhs.y < rhs.y, lhs.z < rhs.z, lhs.w < rhs.w);
        
        /// <summary>
        /// Returns a bVector4 from component-wise application of operator&lt; (lhs &lt; rhs).
        /// </summary>
        public static bVector4 operator<(decVector4 lhs, decimal rhs) => new bVector4(lhs.x < rhs, lhs.y < rhs, lhs.z < rhs, lhs.w < rhs);
        
        /// <summary>
        /// Returns a bVector4 from component-wise application of operator&lt; (lhs &lt; rhs).
        /// </summary>
        public static bVector4 operator<(decimal lhs, decVector4 rhs) => new bVector4(lhs < rhs.x, lhs < rhs.y, lhs < rhs.z, lhs < rhs.w);
        
        /// <summary>
        /// Returns a bVector4 from component-wise application of operator&lt;= (lhs &lt;= rhs).
        /// </summary>
        public static bVector4 operator<=(decVector4 lhs, decVector4 rhs) => new bVector4(lhs.x <= rhs.x, lhs.y <= rhs.y, lhs.z <= rhs.z, lhs.w <= rhs.w);
        
        /// <summary>
        /// Returns a bVector4 from component-wise application of operator&lt;= (lhs &lt;= rhs).
        /// </summary>
        public static bVector4 operator<=(decVector4 lhs, decimal rhs) => new bVector4(lhs.x <= rhs, lhs.y <= rhs, lhs.z <= rhs, lhs.w <= rhs);
        
        /// <summary>
        /// Returns a bVector4 from component-wise application of operator&lt;= (lhs &lt;= rhs).
        /// </summary>
        public static bVector4 operator<=(decimal lhs, decVector4 rhs) => new bVector4(lhs <= rhs.x, lhs <= rhs.y, lhs <= rhs.z, lhs <= rhs.w);
        
        /// <summary>
        /// Returns a bVector4 from component-wise application of operator&gt; (lhs &gt; rhs).
        /// </summary>
        public static bVector4 operator>(decVector4 lhs, decVector4 rhs) => new bVector4(lhs.x > rhs.x, lhs.y > rhs.y, lhs.z > rhs.z, lhs.w > rhs.w);
        
        /// <summary>
        /// Returns a bVector4 from component-wise application of operator&gt; (lhs &gt; rhs).
        /// </summary>
        public static bVector4 operator>(decVector4 lhs, decimal rhs) => new bVector4(lhs.x > rhs, lhs.y > rhs, lhs.z > rhs, lhs.w > rhs);
        
        /// <summary>
        /// Returns a bVector4 from component-wise application of operator&gt; (lhs &gt; rhs).
        /// </summary>
        public static bVector4 operator>(decimal lhs, decVector4 rhs) => new bVector4(lhs > rhs.x, lhs > rhs.y, lhs > rhs.z, lhs > rhs.w);
        
        /// <summary>
        /// Returns a bVector4 from component-wise application of operator&gt;= (lhs &gt;= rhs).
        /// </summary>
        public static bVector4 operator>=(decVector4 lhs, decVector4 rhs) => new bVector4(lhs.x >= rhs.x, lhs.y >= rhs.y, lhs.z >= rhs.z, lhs.w >= rhs.w);
        
        /// <summary>
        /// Returns a bVector4 from component-wise application of operator&gt;= (lhs &gt;= rhs).
        /// </summary>
        public static bVector4 operator>=(decVector4 lhs, decimal rhs) => new bVector4(lhs.x >= rhs, lhs.y >= rhs, lhs.z >= rhs, lhs.w >= rhs);
        
        /// <summary>
        /// Returns a bVector4 from component-wise application of operator&gt;= (lhs &gt;= rhs).
        /// </summary>
        public static bVector4 operator>=(decimal lhs, decVector4 rhs) => new bVector4(lhs >= rhs.x, lhs >= rhs.y, lhs >= rhs.z, lhs >= rhs.w);
        
        /// <summary>
        /// Returns a decVector4 from component-wise application of operator+ (lhs + rhs).
        /// </summary>
        public static decVector4 operator+(decVector4 lhs, decVector4 rhs) => new decVector4(lhs.x + rhs.x, lhs.y + rhs.y, lhs.z + rhs.z, lhs.w + rhs.w);
        
        /// <summary>
        /// Returns a decVector4 from component-wise application of operator+ (lhs + rhs).
        /// </summary>
        public static decVector4 operator+(decVector4 lhs, decimal rhs) => new decVector4(lhs.x + rhs, lhs.y + rhs, lhs.z + rhs, lhs.w + rhs);
        
        /// <summary>
        /// Returns a decVector4 from component-wise application of operator+ (lhs + rhs).
        /// </summary>
        public static decVector4 operator+(decimal lhs, decVector4 rhs) => new decVector4(lhs + rhs.x, lhs + rhs.y, lhs + rhs.z, lhs + rhs.w);
        
        /// <summary>
        /// Returns a decVector4 from component-wise application of operator- (lhs - rhs).
        /// </summary>
        public static decVector4 operator-(decVector4 lhs, decVector4 rhs) => new decVector4(lhs.x - rhs.x, lhs.y - rhs.y, lhs.z - rhs.z, lhs.w - rhs.w);
        
        /// <summary>
        /// Returns a decVector4 from component-wise application of operator- (lhs - rhs).
        /// </summary>
        public static decVector4 operator-(decVector4 lhs, decimal rhs) => new decVector4(lhs.x - rhs, lhs.y - rhs, lhs.z - rhs, lhs.w - rhs);
        
        /// <summary>
        /// Returns a decVector4 from component-wise application of operator- (lhs - rhs).
        /// </summary>
        public static decVector4 operator-(decimal lhs, decVector4 rhs) => new decVector4(lhs - rhs.x, lhs - rhs.y, lhs - rhs.z, lhs - rhs.w);
        
        /// <summary>
        /// Returns a decVector4 from component-wise application of operator* (lhs * rhs).
        /// </summary>
        public static decVector4 operator*(decVector4 lhs, decVector4 rhs) => new decVector4(lhs.x * rhs.x, lhs.y * rhs.y, lhs.z * rhs.z, lhs.w * rhs.w);
        
        /// <summary>
        /// Returns a decVector4 from component-wise application of operator* (lhs * rhs).
        /// </summary>
        public static decVector4 operator*(decVector4 lhs, decimal rhs) => new decVector4(lhs.x * rhs, lhs.y * rhs, lhs.z * rhs, lhs.w * rhs);
        
        /// <summary>
        /// Returns a decVector4 from component-wise application of operator* (lhs * rhs).
        /// </summary>
        public static decVector4 operator*(decimal lhs, decVector4 rhs) => new decVector4(lhs * rhs.x, lhs * rhs.y, lhs * rhs.z, lhs * rhs.w);
        
        /// <summary>
        /// Returns a decVector4 from component-wise application of operator/ (lhs / rhs).
        /// </summary>
        public static decVector4 operator/(decVector4 lhs, decVector4 rhs) => new decVector4(lhs.x / rhs.x, lhs.y / rhs.y, lhs.z / rhs.z, lhs.w / rhs.w);
        
        /// <summary>
        /// Returns a decVector4 from component-wise application of operator/ (lhs / rhs).
        /// </summary>
        public static decVector4 operator/(decVector4 lhs, decimal rhs) => new decVector4(lhs.x / rhs, lhs.y / rhs, lhs.z / rhs, lhs.w / rhs);
        
        /// <summary>
        /// Returns a decVector4 from component-wise application of operator/ (lhs / rhs).
        /// </summary>
        public static decVector4 operator/(decimal lhs, decVector4 rhs) => new decVector4(lhs / rhs.x, lhs / rhs.y, lhs / rhs.z, lhs / rhs.w);
        
        /// <summary>
        /// Returns a decVector4 from component-wise application of operator+ (identity).
        /// </summary>
        public static decVector4 operator+(decVector4 v) => v;
        
        /// <summary>
        /// Returns a decVector4 from component-wise application of operator- (-v).
        /// </summary>
        public static decVector4 operator-(decVector4 v) => new decVector4(-v.x, -v.y, -v.z, -v.w);
        
        /// <summary>
        /// Returns a decVector4 from component-wise application of operator% (lhs % rhs).
        /// </summary>
        public static decVector4 operator%(decVector4 lhs, decVector4 rhs) => new decVector4(lhs.x % rhs.x, lhs.y % rhs.y, lhs.z % rhs.z, lhs.w % rhs.w);
        
        /// <summary>
        /// Returns a decVector4 from component-wise application of operator% (lhs % rhs).
        /// </summary>
        public static decVector4 operator%(decVector4 lhs, decimal rhs) => new decVector4(lhs.x % rhs, lhs.y % rhs, lhs.z % rhs, lhs.w % rhs);
        
        /// <summary>
        /// Returns a decVector4 from component-wise application of operator% (lhs % rhs).
        /// </summary>
        public static decVector4 operator%(decimal lhs, decVector4 rhs) => new decVector4(lhs % rhs.x, lhs % rhs.y, lhs % rhs.z, lhs % rhs.w);

        #endregion

    }
}
