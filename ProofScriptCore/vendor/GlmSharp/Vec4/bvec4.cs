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
    /// A Vectortor of type bool with 4 components.
    /// </summary>
    [RegisterCoreClassStruct]
    
    [StructLayout(LayoutKind.Sequential)]
    public struct bVector4 : IReadOnlyList<bool>, IEquatable<bVector4>
    {

        #region Fields
        
        /// <summary>
        /// x-component
        /// </summary>
        
        public bool x;
        
        /// <summary>
        /// y-component
        /// </summary>
        
        public bool y;
        
        /// <summary>
        /// z-component
        /// </summary>
        
        public bool z;
        
        /// <summary>
        /// w-component
        /// </summary>
        
        public bool w;

        #endregion


        #region Constructors
        
        /// <summary>
        /// Component-wise constructor
        /// </summary>
        public bVector4(bool x, bool y, bool z, bool w)
        {
            this.x = x;
            this.y = y;
            this.z = z;
            this.w = w;
        }
        
        /// <summary>
        /// all-same-value constructor
        /// </summary>
        public bVector4(bool v)
        {
            this.x = v;
            this.y = v;
            this.z = v;
            this.w = v;
        }
        
        /// <summary>
        /// from-Vectortor constructor (empty fields are zero/false)
        /// </summary>
        public bVector4(bVector2 v)
        {
            this.x = v.x;
            this.y = v.y;
            this.z = false;
            this.w = false;
        }
        
        /// <summary>
        /// from-Vectortor-and-value constructor (empty fields are zero/false)
        /// </summary>
        public bVector4(bVector2 v, bool z)
        {
            this.x = v.x;
            this.y = v.y;
            this.z = z;
            this.w = false;
        }
        
        /// <summary>
        /// from-Vectortor-and-value constructor
        /// </summary>
        public bVector4(bVector2 v, bool z, bool w)
        {
            this.x = v.x;
            this.y = v.y;
            this.z = z;
            this.w = w;
        }
        
        /// <summary>
        /// from-Vectortor constructor (empty fields are zero/false)
        /// </summary>
        public bVector4(bVector3 v)
        {
            this.x = v.x;
            this.y = v.y;
            this.z = v.z;
            this.w = false;
        }
        
        /// <summary>
        /// from-Vectortor-and-value constructor
        /// </summary>
        public bVector4(bVector3 v, bool w)
        {
            this.x = v.x;
            this.y = v.y;
            this.z = v.z;
            this.w = w;
        }
        
        /// <summary>
        /// from-Vectortor constructor
        /// </summary>
        public bVector4(bVector4 v)
        {
            this.x = v.x;
            this.y = v.y;
            this.z = v.z;
            this.w = v.w;
        }
        
        /// <summary>
        /// From-array/list constructor (superfluous values are ignored, missing values are zero-filled).
        /// </summary>
        public bVector4(IReadOnlyList<bool> v)
        {
            var c = v.Count;
            this.x = c < 0 ? false : v[0];
            this.y = c < 1 ? false : v[1];
            this.z = c < 2 ? false : v[2];
            this.w = c < 3 ? false : v[3];
        }
        
        /// <summary>
        /// Generic from-array constructor (superfluous values are ignored, missing values are zero-filled).
        /// </summary>
        public bVector4(Object[] v)
        {
            var c = v.Length;
            this.x = c < 0 ? false : (bool)v[0];
            this.y = c < 1 ? false : (bool)v[1];
            this.z = c < 2 ? false : (bool)v[2];
            this.w = c < 3 ? false : (bool)v[3];
        }
        
        /// <summary>
        /// From-array constructor (superfluous values are ignored, missing values are zero-filled).
        /// </summary>
        public bVector4(bool[] v)
        {
            var c = v.Length;
            this.x = c < 0 ? false : v[0];
            this.y = c < 1 ? false : v[1];
            this.z = c < 2 ? false : v[2];
            this.w = c < 3 ? false : v[3];
        }
        
        /// <summary>
        /// From-array constructor with base index (superfluous values are ignored, missing values are zero-filled).
        /// </summary>
        public bVector4(bool[] v, int startIndex)
        {
            var c = v.Length;
            this.x = c + startIndex < 0 ? false : v[0 + startIndex];
            this.y = c + startIndex < 1 ? false : v[1 + startIndex];
            this.z = c + startIndex < 2 ? false : v[2 + startIndex];
            this.w = c + startIndex < 3 ? false : v[3 + startIndex];
        }
        
        /// <summary>
        /// From-IEnumerable constructor (superfluous values are ignored, missing values are zero-filled).
        /// </summary>
        public bVector4(IEnumerable<bool> v)
            : this(v.ToArray())
        {
        }

        #endregion


        #region Explicit Operators
        
        /// <summary>
        /// Explicitly converts this to a iVector2.
        /// </summary>
        public static explicit operator iVector2(bVector4 v) => new iVector2(v.x ? 1 : 0, v.y ? 1 : 0);
        
        /// <summary>
        /// Explicitly converts this to a iVector3.
        /// </summary>
        public static explicit operator iVector3(bVector4 v) => new iVector3(v.x ? 1 : 0, v.y ? 1 : 0, v.z ? 1 : 0);
        
        /// <summary>
        /// Explicitly converts this to a iVector4.
        /// </summary>
        public static explicit operator iVector4(bVector4 v) => new iVector4(v.x ? 1 : 0, v.y ? 1 : 0, v.z ? 1 : 0, v.w ? 1 : 0);
        
        /// <summary>
        /// Explicitly converts this to a uVector2.
        /// </summary>
        public static explicit operator uVector2(bVector4 v) => new uVector2(v.x ? 1u : 0u, v.y ? 1u : 0u);
        
        /// <summary>
        /// Explicitly converts this to a uVector3.
        /// </summary>
        public static explicit operator uVector3(bVector4 v) => new uVector3(v.x ? 1u : 0u, v.y ? 1u : 0u, v.z ? 1u : 0u);
        
        /// <summary>
        /// Explicitly converts this to a uVector4.
        /// </summary>
        public static explicit operator uVector4(bVector4 v) => new uVector4(v.x ? 1u : 0u, v.y ? 1u : 0u, v.z ? 1u : 0u, v.w ? 1u : 0u);
        
        /// <summary>
        /// Explicitly converts this to a Vector2.
        /// </summary>
        public static explicit operator Vector2(bVector4 v) => new Vector2(v.x ? 1f : 0f, v.y ? 1f : 0f);
        
        /// <summary>
        /// Explicitly converts this to a Vector3.
        /// </summary>
        public static explicit operator Vector3(bVector4 v) => new Vector3(v.x ? 1f : 0f, v.y ? 1f : 0f, v.z ? 1f : 0f);
        
        /// <summary>
        /// Explicitly converts this to a Vector4.
        /// </summary>
        public static explicit operator Vector4(bVector4 v) => new Vector4(v.x ? 1f : 0f, v.y ? 1f : 0f, v.z ? 1f : 0f, v.w ? 1f : 0f);
        
        /// <summary>
        /// Explicitly converts this to a hVector2.
        /// </summary>
        public static explicit operator hVector2(bVector4 v) => new hVector2(v.x ? Half.One : Half.Zero, v.y ? Half.One : Half.Zero);
        
        /// <summary>
        /// Explicitly converts this to a hVector3.
        /// </summary>
        public static explicit operator hVector3(bVector4 v) => new hVector3(v.x ? Half.One : Half.Zero, v.y ? Half.One : Half.Zero, v.z ? Half.One : Half.Zero);
        
        /// <summary>
        /// Explicitly converts this to a hVector4.
        /// </summary>
        public static explicit operator hVector4(bVector4 v) => new hVector4(v.x ? Half.One : Half.Zero, v.y ? Half.One : Half.Zero, v.z ? Half.One : Half.Zero, v.w ? Half.One : Half.Zero);
        
        /// <summary>
        /// Explicitly converts this to a dVector2.
        /// </summary>
        public static explicit operator dVector2(bVector4 v) => new dVector2(v.x ? 1.0 : 0.0, v.y ? 1.0 : 0.0);
        
        /// <summary>
        /// Explicitly converts this to a dVector3.
        /// </summary>
        public static explicit operator dVector3(bVector4 v) => new dVector3(v.x ? 1.0 : 0.0, v.y ? 1.0 : 0.0, v.z ? 1.0 : 0.0);
        
        /// <summary>
        /// Explicitly converts this to a dVector4.
        /// </summary>
        public static explicit operator dVector4(bVector4 v) => new dVector4(v.x ? 1.0 : 0.0, v.y ? 1.0 : 0.0, v.z ? 1.0 : 0.0, v.w ? 1.0 : 0.0);
        
        /// <summary>
        /// Explicitly converts this to a decVector2.
        /// </summary>
        public static explicit operator decVector2(bVector4 v) => new decVector2(v.x ? 1m : 0m, v.y ? 1m : 0m);
        
        /// <summary>
        /// Explicitly converts this to a decVector3.
        /// </summary>
        public static explicit operator decVector3(bVector4 v) => new decVector3(v.x ? 1m : 0m, v.y ? 1m : 0m, v.z ? 1m : 0m);
        
        /// <summary>
        /// Explicitly converts this to a decVector4.
        /// </summary>
        public static explicit operator decVector4(bVector4 v) => new decVector4(v.x ? 1m : 0m, v.y ? 1m : 0m, v.z ? 1m : 0m, v.w ? 1m : 0m);
        
        /// <summary>
        /// Explicitly converts this to a cVector2.
        /// </summary>
        //public static explicit operator cVector2(bVector4 v) => new cVector2(v.x ? Complex.One : Complex.Zero, v.y ? Complex.One : Complex.Zero);
        
        /// <summary>
        /// Explicitly converts this to a cVector3.
        /// </summary>
        //public static explicit operator cVector3(bVector4 v) => new cVector3(v.x ? Complex.One : Complex.Zero, v.y ? Complex.One : Complex.Zero, v.z ? Complex.One : Complex.Zero);
        
        /// <summary>
        /// Explicitly converts this to a cVector4.
        /// </summary>
        //public static explicit operator cVector4(bVector4 v) => new cVector4(v.x ? Complex.One : Complex.Zero, v.y ? Complex.One : Complex.Zero, v.z ? Complex.One : Complex.Zero, v.w ? Complex.One : Complex.Zero);
        
        /// <summary>
        /// Explicitly converts this to a lVector2.
        /// </summary>
        public static explicit operator lVector2(bVector4 v) => new lVector2(v.x ? 1 : 0, v.y ? 1 : 0);
        
        /// <summary>
        /// Explicitly converts this to a lVector3.
        /// </summary>
        public static explicit operator lVector3(bVector4 v) => new lVector3(v.x ? 1 : 0, v.y ? 1 : 0, v.z ? 1 : 0);
        
        /// <summary>
        /// Explicitly converts this to a lVector4.
        /// </summary>
        public static explicit operator lVector4(bVector4 v) => new lVector4(v.x ? 1 : 0, v.y ? 1 : 0, v.z ? 1 : 0, v.w ? 1 : 0);
        
        /// <summary>
        /// Explicitly converts this to a bVector2.
        /// </summary>
        public static explicit operator bVector2(bVector4 v) => new bVector2((bool)v.x, (bool)v.y);
        
        /// <summary>
        /// Explicitly converts this to a bVector3.
        /// </summary>
        public static explicit operator bVector3(bVector4 v) => new bVector3((bool)v.x, (bool)v.y, (bool)v.z);
        
        /// <summary>
        /// Explicitly converts this to a bool array.
        /// </summary>
        public static explicit operator bool[](bVector4 v) => new [] { v.x, v.y, v.z, v.w };
        
        /// <summary>
        /// Explicitly converts this to a generic object array.
        /// </summary>
        public static explicit operator Object[](bVector4 v) => new Object[] { v.x, v.y, v.z, v.w };

        #endregion


        #region Indexer
        
        /// <summary>
        /// Gets/Sets a specific indexed component (a bit slower than direct access).
        /// </summary>
        public bool this[int index]
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
        public swizzle_bVector4 swizzle => new swizzle_bVector4(x, y, z, w);
        
        /// <summary>
        /// Gets or sets the specified subset of components. For more advanced (read-only) swizzling, use the .swizzle property.
        /// </summary>
        public bVector2 xy
        {
            get
            {
                return new bVector2(x, y);
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
        public bVector2 xz
        {
            get
            {
                return new bVector2(x, z);
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
        public bVector2 yz
        {
            get
            {
                return new bVector2(y, z);
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
        public bVector3 xyz
        {
            get
            {
                return new bVector3(x, y, z);
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
        public bVector2 xw
        {
            get
            {
                return new bVector2(x, w);
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
        public bVector2 yw
        {
            get
            {
                return new bVector2(y, w);
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
        public bVector3 xyw
        {
            get
            {
                return new bVector3(x, y, w);
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
        public bVector2 zw
        {
            get
            {
                return new bVector2(z, w);
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
        public bVector3 xzw
        {
            get
            {
                return new bVector3(x, z, w);
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
        public bVector3 yzw
        {
            get
            {
                return new bVector3(y, z, w);
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
        public bVector4 xyzw
        {
            get
            {
                return new bVector4(x, y, z, w);
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
        public bVector2 rg
        {
            get
            {
                return new bVector2(x, y);
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
        public bVector2 rb
        {
            get
            {
                return new bVector2(x, z);
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
        public bVector2 gb
        {
            get
            {
                return new bVector2(y, z);
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
        public bVector3 rgb
        {
            get
            {
                return new bVector3(x, y, z);
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
        public bVector2 ra
        {
            get
            {
                return new bVector2(x, w);
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
        public bVector2 ga
        {
            get
            {
                return new bVector2(y, w);
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
        public bVector3 rga
        {
            get
            {
                return new bVector3(x, y, w);
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
        public bVector2 ba
        {
            get
            {
                return new bVector2(z, w);
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
        public bVector3 rba
        {
            get
            {
                return new bVector3(x, z, w);
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
        public bVector3 gba
        {
            get
            {
                return new bVector3(y, z, w);
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
        public bVector4 rgba
        {
            get
            {
                return new bVector4(x, y, z, w);
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
        public bool r
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
        public bool g
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
        public bool b
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
        public bool a
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
        public bool[] Values => new[] { x, y, z, w };
        
        /// <summary>
        /// Returns the number of components (4).
        /// </summary>
        public int Count => 4;
        
        /// <summary>
        /// Returns the minimal component of this Vectortor.
        /// </summary>
        public bool MinElement => ((x && y) && (z && w));
        
        /// <summary>
        /// Returns the maximal component of this Vectortor.
        /// </summary>
        public bool MaxElement => ((x || y) || (z || w));
        
        /// <summary>
        /// Returns true if all component are true.
        /// </summary>
        public bool All => ((x && y) && (z && w));
        
        /// <summary>
        /// Returns true if any component is true.
        /// </summary>
        public bool Any => ((x || y) || (z || w));

        #endregion


        #region Static Properties
        
        /// <summary>
        /// Predefined all-zero Vectortor
        /// </summary>
        public static bVector4 Zero { get; } = new bVector4(false, false, false, false);
        
        /// <summary>
        /// Predefined all-ones Vectortor
        /// </summary>
        public static bVector4 Ones { get; } = new bVector4(true, true, true, true);
        
        /// <summary>
        /// Predefined unit-X Vectortor
        /// </summary>
        public static bVector4 UnitX { get; } = new bVector4(true, false, false, false);
        
        /// <summary>
        /// Predefined unit-Y Vectortor
        /// </summary>
        public static bVector4 UnitY { get; } = new bVector4(false, true, false, false);
        
        /// <summary>
        /// Predefined unit-Z Vectortor
        /// </summary>
        public static bVector4 UnitZ { get; } = new bVector4(false, false, true, false);
        
        /// <summary>
        /// Predefined unit-W Vectortor
        /// </summary>
        public static bVector4 UnitW { get; } = new bVector4(false, false, false, true);

        #endregion


        #region Operators
        
        /// <summary>
        /// Returns true iff this equals rhs component-wise.
        /// </summary>
        public static bool operator==(bVector4 lhs, bVector4 rhs) => lhs.Equals(rhs);
        
        /// <summary>
        /// Returns true iff this does not equal rhs (component-wise).
        /// </summary>
        public static bool operator!=(bVector4 lhs, bVector4 rhs) => !lhs.Equals(rhs);

        #endregion


        #region Functions
        
        /// <summary>
        /// Returns an enumerator that iterates through all components.
        /// </summary>
        public IEnumerator<bool> GetEnumerator()
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
        /// Returns true iff this equals rhs component-wise.
        /// </summary>
        public bool Equals(bVector4 rhs) => ((x.Equals(rhs.x) && y.Equals(rhs.y)) && (z.Equals(rhs.z) && w.Equals(rhs.w)));
        
        /// <summary>
        /// Returns true iff this equals rhs type- and component-wise.
        /// </summary>
        public override bool Equals(object obj)
        {
            if (ReferenceEquals(null, obj)) return false;
            return obj is bVector4 && Equals((bVector4) obj);
        }
        
        /// <summary>
        /// Returns a hash code for this instance.
        /// </summary>
        public override int GetHashCode()
        {
            unchecked
            {
                return ((((((x.GetHashCode()) * 2) ^ y.GetHashCode()) * 2) ^ z.GetHashCode()) * 2) ^ w.GetHashCode();
            }
        }

        #endregion


        #region Static Functions
        
        /// <summary>
        /// Converts the string representation of the Vectortor into a Vectortor representation (using ', ' as a separator).
        /// </summary>
        public static bVector4 Parse(string s) => Parse(s, ", ");
        
        /// <summary>
        /// Converts the string representation of the Vectortor into a Vectortor representation (using a designated separator).
        /// </summary>
        public static bVector4 Parse(string s, string sep)
        {
            var kvp = s.Split(new[] { sep }, StringSplitOptions.None);
            if (kvp.Length != 4) throw new FormatException("input has not exactly 4 parts");
            return new bVector4(bool.Parse(kvp[0].Trim()), bool.Parse(kvp[1].Trim()), bool.Parse(kvp[2].Trim()), bool.Parse(kvp[3].Trim()));
        }
        
        /// <summary>
        /// Tries to convert the string representation of the Vectortor into a Vectortor representation (using ', ' as a separator), returns false if string was invalid.
        /// </summary>
        public static bool TryParse(string s, out bVector4 result) => TryParse(s, ", ", out result);
        
        /// <summary>
        /// Tries to convert the string representation of the Vectortor into a Vectortor representation (using a designated separator), returns false if string was invalid.
        /// </summary>
        public static bool TryParse(string s, string sep, out bVector4 result)
        {
            result = Zero;
            if (string.IsNullOrEmpty(s)) return false;
            var kvp = s.Split(new[] { sep }, StringSplitOptions.None);
            if (kvp.Length != 4) return false;
            bool x = false, y = false, z = false, w = false;
            var ok = ((bool.TryParse(kvp[0].Trim(), out x) && bool.TryParse(kvp[1].Trim(), out y)) && (bool.TryParse(kvp[2].Trim(), out z) && bool.TryParse(kvp[3].Trim(), out w)));
            result = ok ? new bVector4(x, y, z, w) : Zero;
            return ok;
        }
        
        /// <summary>
        /// Returns a bVector4 with independent and identically distributed random true/false values (the probability for 'true' can be configured).
        /// </summary>
        public static bVector4 Random(System.Random random, float trueProbability = 0.5f) => new bVector4(random.NextDouble() < trueProbability, random.NextDouble() < trueProbability, random.NextDouble() < trueProbability, random.NextDouble() < trueProbability);

        #endregion


        #region Component-Wise Static Functions
        
        /// <summary>
        /// Returns a bVector4 from component-wise application of Equal (lhs == rhs).
        /// </summary>
        public static bVector4 Equal(bVector4 lhs, bVector4 rhs) => new bVector4(lhs.x == rhs.x, lhs.y == rhs.y, lhs.z == rhs.z, lhs.w == rhs.w);
        
        /// <summary>
        /// Returns a bVector4 from component-wise application of Equal (lhs == rhs).
        /// </summary>
        public static bVector4 Equal(bVector4 lhs, bool rhs) => new bVector4(lhs.x == rhs, lhs.y == rhs, lhs.z == rhs, lhs.w == rhs);
        
        /// <summary>
        /// Returns a bVector4 from component-wise application of Equal (lhs == rhs).
        /// </summary>
        public static bVector4 Equal(bool lhs, bVector4 rhs) => new bVector4(lhs == rhs.x, lhs == rhs.y, lhs == rhs.z, lhs == rhs.w);
        
        /// <summary>
        /// Returns a bVector from the application of Equal (lhs == rhs).
        /// </summary>
        public static bVector4 Equal(bool lhs, bool rhs) => new bVector4(lhs == rhs);
        
        /// <summary>
        /// Returns a bVector4 from component-wise application of NotEqual (lhs != rhs).
        /// </summary>
        public static bVector4 NotEqual(bVector4 lhs, bVector4 rhs) => new bVector4(lhs.x != rhs.x, lhs.y != rhs.y, lhs.z != rhs.z, lhs.w != rhs.w);
        
        /// <summary>
        /// Returns a bVector4 from component-wise application of NotEqual (lhs != rhs).
        /// </summary>
        public static bVector4 NotEqual(bVector4 lhs, bool rhs) => new bVector4(lhs.x != rhs, lhs.y != rhs, lhs.z != rhs, lhs.w != rhs);
        
        /// <summary>
        /// Returns a bVector4 from component-wise application of NotEqual (lhs != rhs).
        /// </summary>
        public static bVector4 NotEqual(bool lhs, bVector4 rhs) => new bVector4(lhs != rhs.x, lhs != rhs.y, lhs != rhs.z, lhs != rhs.w);
        
        /// <summary>
        /// Returns a bVector from the application of NotEqual (lhs != rhs).
        /// </summary>
        public static bVector4 NotEqual(bool lhs, bool rhs) => new bVector4(lhs != rhs);
        
        /// <summary>
        /// Returns a bVector4 from component-wise application of Not (!v).
        /// </summary>
        public static bVector4 Not(bVector4 v) => new bVector4(!v.x, !v.y, !v.z, !v.w);
        
        /// <summary>
        /// Returns a bVector from the application of Not (!v).
        /// </summary>
        public static bVector4 Not(bool v) => new bVector4(!v);
        
        /// <summary>
        /// Returns a bVector4 from component-wise application of And (lhs &amp;&amp; rhs).
        /// </summary>
        public static bVector4 And(bVector4 lhs, bVector4 rhs) => new bVector4(lhs.x && rhs.x, lhs.y && rhs.y, lhs.z && rhs.z, lhs.w && rhs.w);
        
        /// <summary>
        /// Returns a bVector4 from component-wise application of And (lhs &amp;&amp; rhs).
        /// </summary>
        public static bVector4 And(bVector4 lhs, bool rhs) => new bVector4(lhs.x && rhs, lhs.y && rhs, lhs.z && rhs, lhs.w && rhs);
        
        /// <summary>
        /// Returns a bVector4 from component-wise application of And (lhs &amp;&amp; rhs).
        /// </summary>
        public static bVector4 And(bool lhs, bVector4 rhs) => new bVector4(lhs && rhs.x, lhs && rhs.y, lhs && rhs.z, lhs && rhs.w);
        
        /// <summary>
        /// Returns a bVector from the application of And (lhs &amp;&amp; rhs).
        /// </summary>
        public static bVector4 And(bool lhs, bool rhs) => new bVector4(lhs && rhs);
        
        /// <summary>
        /// Returns a bVector4 from component-wise application of Nand (!(lhs &amp;&amp; rhs)).
        /// </summary>
        public static bVector4 Nand(bVector4 lhs, bVector4 rhs) => new bVector4(!(lhs.x && rhs.x), !(lhs.y && rhs.y), !(lhs.z && rhs.z), !(lhs.w && rhs.w));
        
        /// <summary>
        /// Returns a bVector4 from component-wise application of Nand (!(lhs &amp;&amp; rhs)).
        /// </summary>
        public static bVector4 Nand(bVector4 lhs, bool rhs) => new bVector4(!(lhs.x && rhs), !(lhs.y && rhs), !(lhs.z && rhs), !(lhs.w && rhs));
        
        /// <summary>
        /// Returns a bVector4 from component-wise application of Nand (!(lhs &amp;&amp; rhs)).
        /// </summary>
        public static bVector4 Nand(bool lhs, bVector4 rhs) => new bVector4(!(lhs && rhs.x), !(lhs && rhs.y), !(lhs && rhs.z), !(lhs && rhs.w));
        
        /// <summary>
        /// Returns a bVector from the application of Nand (!(lhs &amp;&amp; rhs)).
        /// </summary>
        public static bVector4 Nand(bool lhs, bool rhs) => new bVector4(!(lhs && rhs));
        
        /// <summary>
        /// Returns a bVector4 from component-wise application of Or (lhs || rhs).
        /// </summary>
        public static bVector4 Or(bVector4 lhs, bVector4 rhs) => new bVector4(lhs.x || rhs.x, lhs.y || rhs.y, lhs.z || rhs.z, lhs.w || rhs.w);
        
        /// <summary>
        /// Returns a bVector4 from component-wise application of Or (lhs || rhs).
        /// </summary>
        public static bVector4 Or(bVector4 lhs, bool rhs) => new bVector4(lhs.x || rhs, lhs.y || rhs, lhs.z || rhs, lhs.w || rhs);
        
        /// <summary>
        /// Returns a bVector4 from component-wise application of Or (lhs || rhs).
        /// </summary>
        public static bVector4 Or(bool lhs, bVector4 rhs) => new bVector4(lhs || rhs.x, lhs || rhs.y, lhs || rhs.z, lhs || rhs.w);
        
        /// <summary>
        /// Returns a bVector from the application of Or (lhs || rhs).
        /// </summary>
        public static bVector4 Or(bool lhs, bool rhs) => new bVector4(lhs || rhs);
        
        /// <summary>
        /// Returns a bVector4 from component-wise application of Nor (!(lhs || rhs)).
        /// </summary>
        public static bVector4 Nor(bVector4 lhs, bVector4 rhs) => new bVector4(!(lhs.x || rhs.x), !(lhs.y || rhs.y), !(lhs.z || rhs.z), !(lhs.w || rhs.w));
        
        /// <summary>
        /// Returns a bVector4 from component-wise application of Nor (!(lhs || rhs)).
        /// </summary>
        public static bVector4 Nor(bVector4 lhs, bool rhs) => new bVector4(!(lhs.x || rhs), !(lhs.y || rhs), !(lhs.z || rhs), !(lhs.w || rhs));
        
        /// <summary>
        /// Returns a bVector4 from component-wise application of Nor (!(lhs || rhs)).
        /// </summary>
        public static bVector4 Nor(bool lhs, bVector4 rhs) => new bVector4(!(lhs || rhs.x), !(lhs || rhs.y), !(lhs || rhs.z), !(lhs || rhs.w));
        
        /// <summary>
        /// Returns a bVector from the application of Nor (!(lhs || rhs)).
        /// </summary>
        public static bVector4 Nor(bool lhs, bool rhs) => new bVector4(!(lhs || rhs));
        
        /// <summary>
        /// Returns a bVector4 from component-wise application of Xor (lhs != rhs).
        /// </summary>
        public static bVector4 Xor(bVector4 lhs, bVector4 rhs) => new bVector4(lhs.x != rhs.x, lhs.y != rhs.y, lhs.z != rhs.z, lhs.w != rhs.w);
        
        /// <summary>
        /// Returns a bVector4 from component-wise application of Xor (lhs != rhs).
        /// </summary>
        public static bVector4 Xor(bVector4 lhs, bool rhs) => new bVector4(lhs.x != rhs, lhs.y != rhs, lhs.z != rhs, lhs.w != rhs);
        
        /// <summary>
        /// Returns a bVector4 from component-wise application of Xor (lhs != rhs).
        /// </summary>
        public static bVector4 Xor(bool lhs, bVector4 rhs) => new bVector4(lhs != rhs.x, lhs != rhs.y, lhs != rhs.z, lhs != rhs.w);
        
        /// <summary>
        /// Returns a bVector from the application of Xor (lhs != rhs).
        /// </summary>
        public static bVector4 Xor(bool lhs, bool rhs) => new bVector4(lhs != rhs);
        
        /// <summary>
        /// Returns a bVector4 from component-wise application of Xnor (lhs == rhs).
        /// </summary>
        public static bVector4 Xnor(bVector4 lhs, bVector4 rhs) => new bVector4(lhs.x == rhs.x, lhs.y == rhs.y, lhs.z == rhs.z, lhs.w == rhs.w);
        
        /// <summary>
        /// Returns a bVector4 from component-wise application of Xnor (lhs == rhs).
        /// </summary>
        public static bVector4 Xnor(bVector4 lhs, bool rhs) => new bVector4(lhs.x == rhs, lhs.y == rhs, lhs.z == rhs, lhs.w == rhs);
        
        /// <summary>
        /// Returns a bVector4 from component-wise application of Xnor (lhs == rhs).
        /// </summary>
        public static bVector4 Xnor(bool lhs, bVector4 rhs) => new bVector4(lhs == rhs.x, lhs == rhs.y, lhs == rhs.z, lhs == rhs.w);
        
        /// <summary>
        /// Returns a bVector from the application of Xnor (lhs == rhs).
        /// </summary>
        public static bVector4 Xnor(bool lhs, bool rhs) => new bVector4(lhs == rhs);

        #endregion


        #region Component-Wise Operator Overloads
        
        /// <summary>
        /// Returns a bVector4 from component-wise application of operator! (!v).
        /// </summary>
        public static bVector4 operator!(bVector4 v) => new bVector4(!v.x, !v.y, !v.z, !v.w);
        
        /// <summary>
        /// Returns a bVector4 from component-wise application of operator&amp; (lhs &amp;&amp; rhs).
        /// </summary>
        public static bVector4 operator&(bVector4 lhs, bVector4 rhs) => new bVector4(lhs.x && rhs.x, lhs.y && rhs.y, lhs.z && rhs.z, lhs.w && rhs.w);
        
        /// <summary>
        /// Returns a bVector4 from component-wise application of operator&amp; (lhs &amp;&amp; rhs).
        /// </summary>
        public static bVector4 operator&(bVector4 lhs, bool rhs) => new bVector4(lhs.x && rhs, lhs.y && rhs, lhs.z && rhs, lhs.w && rhs);
        
        /// <summary>
        /// Returns a bVector4 from component-wise application of operator&amp; (lhs &amp;&amp; rhs).
        /// </summary>
        public static bVector4 operator&(bool lhs, bVector4 rhs) => new bVector4(lhs && rhs.x, lhs && rhs.y, lhs && rhs.z, lhs && rhs.w);
        
        /// <summary>
        /// Returns a bVector4 from component-wise application of operator| (lhs || rhs).
        /// </summary>
        public static bVector4 operator|(bVector4 lhs, bVector4 rhs) => new bVector4(lhs.x || rhs.x, lhs.y || rhs.y, lhs.z || rhs.z, lhs.w || rhs.w);
        
        /// <summary>
        /// Returns a bVector4 from component-wise application of operator| (lhs || rhs).
        /// </summary>
        public static bVector4 operator|(bVector4 lhs, bool rhs) => new bVector4(lhs.x || rhs, lhs.y || rhs, lhs.z || rhs, lhs.w || rhs);
        
        /// <summary>
        /// Returns a bVector4 from component-wise application of operator| (lhs || rhs).
        /// </summary>
        public static bVector4 operator|(bool lhs, bVector4 rhs) => new bVector4(lhs || rhs.x, lhs || rhs.y, lhs || rhs.z, lhs || rhs.w);

        #endregion

    }
}
