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
    /// A Vectortor of type bool with 3 components.
    /// </summary>
    [RegisterCoreClassStruct]
    
    [StructLayout(LayoutKind.Sequential)]
    public struct bVector3 : IReadOnlyList<bool>, IEquatable<bVector3>
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

        #endregion


        #region Constructors
        
        /// <summary>
        /// Component-wise constructor
        /// </summary>
        public bVector3(bool x, bool y, bool z)
        {
            this.x = x;
            this.y = y;
            this.z = z;
        }
        
        /// <summary>
        /// all-same-value constructor
        /// </summary>
        public bVector3(bool v)
        {
            this.x = v;
            this.y = v;
            this.z = v;
        }
        
        /// <summary>
        /// from-Vectortor constructor (empty fields are zero/false)
        /// </summary>
        public bVector3(bVector2 v)
        {
            this.x = v.x;
            this.y = v.y;
            this.z = false;
        }
        
        /// <summary>
        /// from-Vectortor-and-value constructor
        /// </summary>
        public bVector3(bVector2 v, bool z)
        {
            this.x = v.x;
            this.y = v.y;
            this.z = z;
        }
        
        /// <summary>
        /// from-Vectortor constructor
        /// </summary>
        public bVector3(bVector3 v)
        {
            this.x = v.x;
            this.y = v.y;
            this.z = v.z;
        }
        
        /// <summary>
        /// from-Vectortor constructor (additional fields are truncated)
        /// </summary>
        public bVector3(bVector4 v)
        {
            this.x = v.x;
            this.y = v.y;
            this.z = v.z;
        }
        
        /// <summary>
        /// From-array/list constructor (superfluous values are ignored, missing values are zero-filled).
        /// </summary>
        public bVector3(IReadOnlyList<bool> v)
        {
            var c = v.Count;
            this.x = c < 0 ? false : v[0];
            this.y = c < 1 ? false : v[1];
            this.z = c < 2 ? false : v[2];
        }
        
        /// <summary>
        /// Generic from-array constructor (superfluous values are ignored, missing values are zero-filled).
        /// </summary>
        public bVector3(Object[] v)
        {
            var c = v.Length;
            this.x = c < 0 ? false : (bool)v[0];
            this.y = c < 1 ? false : (bool)v[1];
            this.z = c < 2 ? false : (bool)v[2];
        }
        
        /// <summary>
        /// From-array constructor (superfluous values are ignored, missing values are zero-filled).
        /// </summary>
        public bVector3(bool[] v)
        {
            var c = v.Length;
            this.x = c < 0 ? false : v[0];
            this.y = c < 1 ? false : v[1];
            this.z = c < 2 ? false : v[2];
        }
        
        /// <summary>
        /// From-array constructor with base index (superfluous values are ignored, missing values are zero-filled).
        /// </summary>
        public bVector3(bool[] v, int startIndex)
        {
            var c = v.Length;
            this.x = c + startIndex < 0 ? false : v[0 + startIndex];
            this.y = c + startIndex < 1 ? false : v[1 + startIndex];
            this.z = c + startIndex < 2 ? false : v[2 + startIndex];
        }
        
        /// <summary>
        /// From-IEnumerable constructor (superfluous values are ignored, missing values are zero-filled).
        /// </summary>
        public bVector3(IEnumerable<bool> v)
            : this(v.ToArray())
        {
        }

        #endregion


        #region Explicit Operators
        
        /// <summary>
        /// Explicitly converts this to a iVector2.
        /// </summary>
        public static explicit operator iVector2(bVector3 v) => new iVector2(v.x ? 1 : 0, v.y ? 1 : 0);
        
        /// <summary>
        /// Explicitly converts this to a iVector3.
        /// </summary>
        public static explicit operator iVector3(bVector3 v) => new iVector3(v.x ? 1 : 0, v.y ? 1 : 0, v.z ? 1 : 0);
        
        /// <summary>
        /// Explicitly converts this to a iVector4. (Higher components are zeroed)
        /// </summary>
        public static explicit operator iVector4(bVector3 v) => new iVector4(v.x ? 1 : 0, v.y ? 1 : 0, v.z ? 1 : 0, 0);
        
        /// <summary>
        /// Explicitly converts this to a uVector2.
        /// </summary>
        public static explicit operator uVector2(bVector3 v) => new uVector2(v.x ? 1u : 0u, v.y ? 1u : 0u);
        
        /// <summary>
        /// Explicitly converts this to a uVector3.
        /// </summary>
        public static explicit operator uVector3(bVector3 v) => new uVector3(v.x ? 1u : 0u, v.y ? 1u : 0u, v.z ? 1u : 0u);
        
        /// <summary>
        /// Explicitly converts this to a uVector4. (Higher components are zeroed)
        /// </summary>
        public static explicit operator uVector4(bVector3 v) => new uVector4(v.x ? 1u : 0u, v.y ? 1u : 0u, v.z ? 1u : 0u, 0u);
        
        /// <summary>
        /// Explicitly converts this to a Vector2.
        /// </summary>
        public static explicit operator Vector2(bVector3 v) => new Vector2(v.x ? 1f : 0f, v.y ? 1f : 0f);
        
        /// <summary>
        /// Explicitly converts this to a Vector3.
        /// </summary>
        public static explicit operator Vector3(bVector3 v) => new Vector3(v.x ? 1f : 0f, v.y ? 1f : 0f, v.z ? 1f : 0f);
        
        /// <summary>
        /// Explicitly converts this to a Vector4. (Higher components are zeroed)
        /// </summary>
        public static explicit operator Vector4(bVector3 v) => new Vector4(v.x ? 1f : 0f, v.y ? 1f : 0f, v.z ? 1f : 0f, 0f);
        
        /// <summary>
        /// Explicitly converts this to a hVector2.
        /// </summary>
        public static explicit operator hVector2(bVector3 v) => new hVector2(v.x ? Half.One : Half.Zero, v.y ? Half.One : Half.Zero);
        
        /// <summary>
        /// Explicitly converts this to a hVector3.
        /// </summary>
        public static explicit operator hVector3(bVector3 v) => new hVector3(v.x ? Half.One : Half.Zero, v.y ? Half.One : Half.Zero, v.z ? Half.One : Half.Zero);
        
        /// <summary>
        /// Explicitly converts this to a hVector4. (Higher components are zeroed)
        /// </summary>
        public static explicit operator hVector4(bVector3 v) => new hVector4(v.x ? Half.One : Half.Zero, v.y ? Half.One : Half.Zero, v.z ? Half.One : Half.Zero, Half.Zero);
        
        /// <summary>
        /// Explicitly converts this to a dVector2.
        /// </summary>
        public static explicit operator dVector2(bVector3 v) => new dVector2(v.x ? 1.0 : 0.0, v.y ? 1.0 : 0.0);
        
        /// <summary>
        /// Explicitly converts this to a dVector3.
        /// </summary>
        public static explicit operator dVector3(bVector3 v) => new dVector3(v.x ? 1.0 : 0.0, v.y ? 1.0 : 0.0, v.z ? 1.0 : 0.0);
        
        /// <summary>
        /// Explicitly converts this to a dVector4. (Higher components are zeroed)
        /// </summary>
        public static explicit operator dVector4(bVector3 v) => new dVector4(v.x ? 1.0 : 0.0, v.y ? 1.0 : 0.0, v.z ? 1.0 : 0.0, 0.0);
        
        /// <summary>
        /// Explicitly converts this to a decVector2.
        /// </summary>
        public static explicit operator decVector2(bVector3 v) => new decVector2(v.x ? 1m : 0m, v.y ? 1m : 0m);
        
        /// <summary>
        /// Explicitly converts this to a decVector3.
        /// </summary>
        public static explicit operator decVector3(bVector3 v) => new decVector3(v.x ? 1m : 0m, v.y ? 1m : 0m, v.z ? 1m : 0m);
        
        /// <summary>
        /// Explicitly converts this to a decVector4. (Higher components are zeroed)
        /// </summary>
        public static explicit operator decVector4(bVector3 v) => new decVector4(v.x ? 1m : 0m, v.y ? 1m : 0m, v.z ? 1m : 0m, 0m);
        
        /// <summary>
        /// Explicitly converts this to a cVector2.
        /// </summary>
        //public static explicit operator cVector2(bVector3 v) => new cVector2(v.x ? Complex.One : Complex.Zero, v.y ? Complex.One : Complex.Zero);
        
        /// <summary>
        /// Explicitly converts this to a cVector3.
        /// </summary>
        //public static explicit operator cVector3(bVector3 v) => new cVector3(v.x ? Complex.One : Complex.Zero, v.y ? Complex.One : Complex.Zero, v.z ? Complex.One : Complex.Zero);
        
        /// <summary>
        /// Explicitly converts this to a cVector4. (Higher components are zeroed)
        /// </summary>
        //public static explicit operator cVector4(bVector3 v) => new cVector4(v.x ? Complex.One : Complex.Zero, v.y ? Complex.One : Complex.Zero, v.z ? Complex.One : Complex.Zero, Complex.Zero);
        
        /// <summary>
        /// Explicitly converts this to a lVector2.
        /// </summary>
        public static explicit operator lVector2(bVector3 v) => new lVector2(v.x ? 1 : 0, v.y ? 1 : 0);
        
        /// <summary>
        /// Explicitly converts this to a lVector3.
        /// </summary>
        public static explicit operator lVector3(bVector3 v) => new lVector3(v.x ? 1 : 0, v.y ? 1 : 0, v.z ? 1 : 0);
        
        /// <summary>
        /// Explicitly converts this to a lVector4. (Higher components are zeroed)
        /// </summary>
        public static explicit operator lVector4(bVector3 v) => new lVector4(v.x ? 1 : 0, v.y ? 1 : 0, v.z ? 1 : 0, 0);
        
        /// <summary>
        /// Explicitly converts this to a bVector2.
        /// </summary>
        public static explicit operator bVector2(bVector3 v) => new bVector2((bool)v.x, (bool)v.y);
        
        /// <summary>
        /// Explicitly converts this to a bVector4. (Higher components are zeroed)
        /// </summary>
        public static explicit operator bVector4(bVector3 v) => new bVector4((bool)v.x, (bool)v.y, (bool)v.z, false);
        
        /// <summary>
        /// Explicitly converts this to a bool array.
        /// </summary>
        public static explicit operator bool[](bVector3 v) => new [] { v.x, v.y, v.z };
        
        /// <summary>
        /// Explicitly converts this to a generic object array.
        /// </summary>
        public static explicit operator Object[](bVector3 v) => new Object[] { v.x, v.y, v.z };

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
        public swizzle_bVector3 swizzle => new swizzle_bVector3(x, y, z);
        
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
        /// Returns an array with all values
        /// </summary>
        public bool[] Values => new[] { x, y, z };
        
        /// <summary>
        /// Returns the number of components (3).
        /// </summary>
        public int Count => 3;
        
        /// <summary>
        /// Returns the minimal component of this Vectortor.
        /// </summary>
        public bool MinElement => ((x && y) && z);
        
        /// <summary>
        /// Returns the maximal component of this Vectortor.
        /// </summary>
        public bool MaxElement => ((x || y) || z);
        
        /// <summary>
        /// Returns true if all component are true.
        /// </summary>
        public bool All => ((x && y) && z);
        
        /// <summary>
        /// Returns true if any component is true.
        /// </summary>
        public bool Any => ((x || y) || z);

        #endregion


        #region Static Properties
        
        /// <summary>
        /// Predefined all-zero Vectortor
        /// </summary>
        public static bVector3 Zero { get; } = new bVector3(false, false, false);
        
        /// <summary>
        /// Predefined all-ones Vectortor
        /// </summary>
        public static bVector3 Ones { get; } = new bVector3(true, true, true);
        
        /// <summary>
        /// Predefined unit-X Vectortor
        /// </summary>
        public static bVector3 UnitX { get; } = new bVector3(true, false, false);
        
        /// <summary>
        /// Predefined unit-Y Vectortor
        /// </summary>
        public static bVector3 UnitY { get; } = new bVector3(false, true, false);
        
        /// <summary>
        /// Predefined unit-Z Vectortor
        /// </summary>
        public static bVector3 UnitZ { get; } = new bVector3(false, false, true);

        #endregion


        #region Operators
        
        /// <summary>
        /// Returns true iff this equals rhs component-wise.
        /// </summary>
        public static bool operator==(bVector3 lhs, bVector3 rhs) => lhs.Equals(rhs);
        
        /// <summary>
        /// Returns true iff this does not equal rhs (component-wise).
        /// </summary>
        public static bool operator!=(bVector3 lhs, bVector3 rhs) => !lhs.Equals(rhs);

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
        /// Returns a string representation of this Vectortor using a provided seperator and a forMatrix provider for each component.
        /// </summary>
        public string ToString(string sep, IFormatProvider provider) => ((x.ToString(provider) + sep + y.ToString(provider)) + sep + z.ToString(provider));
        
        /// <summary>
        /// Returns true iff this equals rhs component-wise.
        /// </summary>
        public bool Equals(bVector3 rhs) => ((x.Equals(rhs.x) && y.Equals(rhs.y)) && z.Equals(rhs.z));
        
        /// <summary>
        /// Returns true iff this equals rhs type- and component-wise.
        /// </summary>
        public override bool Equals(object obj)
        {
            if (ReferenceEquals(null, obj)) return false;
            return obj is bVector3 && Equals((bVector3) obj);
        }
        
        /// <summary>
        /// Returns a hash code for this instance.
        /// </summary>
        public override int GetHashCode()
        {
            unchecked
            {
                return ((((x.GetHashCode()) * 2) ^ y.GetHashCode()) * 2) ^ z.GetHashCode();
            }
        }

        #endregion


        #region Static Functions
        
        /// <summary>
        /// Converts the string representation of the Vectortor into a Vectortor representation (using ', ' as a separator).
        /// </summary>
        public static bVector3 Parse(string s) => Parse(s, ", ");
        
        /// <summary>
        /// Converts the string representation of the Vectortor into a Vectortor representation (using a designated separator).
        /// </summary>
        public static bVector3 Parse(string s, string sep)
        {
            var kvp = s.Split(new[] { sep }, StringSplitOptions.None);
            if (kvp.Length != 3) throw new FormatException("input has not exactly 3 parts");
            return new bVector3(bool.Parse(kvp[0].Trim()), bool.Parse(kvp[1].Trim()), bool.Parse(kvp[2].Trim()));
        }
        
        /// <summary>
        /// Tries to convert the string representation of the Vectortor into a Vectortor representation (using ', ' as a separator), returns false if string was invalid.
        /// </summary>
        public static bool TryParse(string s, out bVector3 result) => TryParse(s, ", ", out result);
        
        /// <summary>
        /// Tries to convert the string representation of the Vectortor into a Vectortor representation (using a designated separator), returns false if string was invalid.
        /// </summary>
        public static bool TryParse(string s, string sep, out bVector3 result)
        {
            result = Zero;
            if (string.IsNullOrEmpty(s)) return false;
            var kvp = s.Split(new[] { sep }, StringSplitOptions.None);
            if (kvp.Length != 3) return false;
            bool x = false, y = false, z = false;
            var ok = ((bool.TryParse(kvp[0].Trim(), out x) && bool.TryParse(kvp[1].Trim(), out y)) && bool.TryParse(kvp[2].Trim(), out z));
            result = ok ? new bVector3(x, y, z) : Zero;
            return ok;
        }
        
        /// <summary>
        /// Returns a bVector3 with independent and identically distributed random true/false values (the probability for 'true' can be configured).
        /// </summary>
        public static bVector3 Random(System.Random random, float trueProbability = 0.5f) => new bVector3(random.NextDouble() < trueProbability, random.NextDouble() < trueProbability, random.NextDouble() < trueProbability);

        #endregion


        #region Component-Wise Static Functions
        
        /// <summary>
        /// Returns a bVector3 from component-wise application of Equal (lhs == rhs).
        /// </summary>
        public static bVector3 Equal(bVector3 lhs, bVector3 rhs) => new bVector3(lhs.x == rhs.x, lhs.y == rhs.y, lhs.z == rhs.z);
        
        /// <summary>
        /// Returns a bVector3 from component-wise application of Equal (lhs == rhs).
        /// </summary>
        public static bVector3 Equal(bVector3 lhs, bool rhs) => new bVector3(lhs.x == rhs, lhs.y == rhs, lhs.z == rhs);
        
        /// <summary>
        /// Returns a bVector3 from component-wise application of Equal (lhs == rhs).
        /// </summary>
        public static bVector3 Equal(bool lhs, bVector3 rhs) => new bVector3(lhs == rhs.x, lhs == rhs.y, lhs == rhs.z);
        
        /// <summary>
        /// Returns a bVector from the application of Equal (lhs == rhs).
        /// </summary>
        public static bVector3 Equal(bool lhs, bool rhs) => new bVector3(lhs == rhs);
        
        /// <summary>
        /// Returns a bVector3 from component-wise application of NotEqual (lhs != rhs).
        /// </summary>
        public static bVector3 NotEqual(bVector3 lhs, bVector3 rhs) => new bVector3(lhs.x != rhs.x, lhs.y != rhs.y, lhs.z != rhs.z);
        
        /// <summary>
        /// Returns a bVector3 from component-wise application of NotEqual (lhs != rhs).
        /// </summary>
        public static bVector3 NotEqual(bVector3 lhs, bool rhs) => new bVector3(lhs.x != rhs, lhs.y != rhs, lhs.z != rhs);
        
        /// <summary>
        /// Returns a bVector3 from component-wise application of NotEqual (lhs != rhs).
        /// </summary>
        public static bVector3 NotEqual(bool lhs, bVector3 rhs) => new bVector3(lhs != rhs.x, lhs != rhs.y, lhs != rhs.z);
        
        /// <summary>
        /// Returns a bVector from the application of NotEqual (lhs != rhs).
        /// </summary>
        public static bVector3 NotEqual(bool lhs, bool rhs) => new bVector3(lhs != rhs);
        
        /// <summary>
        /// Returns a bVector3 from component-wise application of Not (!v).
        /// </summary>
        public static bVector3 Not(bVector3 v) => new bVector3(!v.x, !v.y, !v.z);
        
        /// <summary>
        /// Returns a bVector from the application of Not (!v).
        /// </summary>
        public static bVector3 Not(bool v) => new bVector3(!v);
        
        /// <summary>
        /// Returns a bVector3 from component-wise application of And (lhs &amp;&amp; rhs).
        /// </summary>
        public static bVector3 And(bVector3 lhs, bVector3 rhs) => new bVector3(lhs.x && rhs.x, lhs.y && rhs.y, lhs.z && rhs.z);
        
        /// <summary>
        /// Returns a bVector3 from component-wise application of And (lhs &amp;&amp; rhs).
        /// </summary>
        public static bVector3 And(bVector3 lhs, bool rhs) => new bVector3(lhs.x && rhs, lhs.y && rhs, lhs.z && rhs);
        
        /// <summary>
        /// Returns a bVector3 from component-wise application of And (lhs &amp;&amp; rhs).
        /// </summary>
        public static bVector3 And(bool lhs, bVector3 rhs) => new bVector3(lhs && rhs.x, lhs && rhs.y, lhs && rhs.z);
        
        /// <summary>
        /// Returns a bVector from the application of And (lhs &amp;&amp; rhs).
        /// </summary>
        public static bVector3 And(bool lhs, bool rhs) => new bVector3(lhs && rhs);
        
        /// <summary>
        /// Returns a bVector3 from component-wise application of Nand (!(lhs &amp;&amp; rhs)).
        /// </summary>
        public static bVector3 Nand(bVector3 lhs, bVector3 rhs) => new bVector3(!(lhs.x && rhs.x), !(lhs.y && rhs.y), !(lhs.z && rhs.z));
        
        /// <summary>
        /// Returns a bVector3 from component-wise application of Nand (!(lhs &amp;&amp; rhs)).
        /// </summary>
        public static bVector3 Nand(bVector3 lhs, bool rhs) => new bVector3(!(lhs.x && rhs), !(lhs.y && rhs), !(lhs.z && rhs));
        
        /// <summary>
        /// Returns a bVector3 from component-wise application of Nand (!(lhs &amp;&amp; rhs)).
        /// </summary>
        public static bVector3 Nand(bool lhs, bVector3 rhs) => new bVector3(!(lhs && rhs.x), !(lhs && rhs.y), !(lhs && rhs.z));
        
        /// <summary>
        /// Returns a bVector from the application of Nand (!(lhs &amp;&amp; rhs)).
        /// </summary>
        public static bVector3 Nand(bool lhs, bool rhs) => new bVector3(!(lhs && rhs));
        
        /// <summary>
        /// Returns a bVector3 from component-wise application of Or (lhs || rhs).
        /// </summary>
        public static bVector3 Or(bVector3 lhs, bVector3 rhs) => new bVector3(lhs.x || rhs.x, lhs.y || rhs.y, lhs.z || rhs.z);
        
        /// <summary>
        /// Returns a bVector3 from component-wise application of Or (lhs || rhs).
        /// </summary>
        public static bVector3 Or(bVector3 lhs, bool rhs) => new bVector3(lhs.x || rhs, lhs.y || rhs, lhs.z || rhs);
        
        /// <summary>
        /// Returns a bVector3 from component-wise application of Or (lhs || rhs).
        /// </summary>
        public static bVector3 Or(bool lhs, bVector3 rhs) => new bVector3(lhs || rhs.x, lhs || rhs.y, lhs || rhs.z);
        
        /// <summary>
        /// Returns a bVector from the application of Or (lhs || rhs).
        /// </summary>
        public static bVector3 Or(bool lhs, bool rhs) => new bVector3(lhs || rhs);
        
        /// <summary>
        /// Returns a bVector3 from component-wise application of Nor (!(lhs || rhs)).
        /// </summary>
        public static bVector3 Nor(bVector3 lhs, bVector3 rhs) => new bVector3(!(lhs.x || rhs.x), !(lhs.y || rhs.y), !(lhs.z || rhs.z));
        
        /// <summary>
        /// Returns a bVector3 from component-wise application of Nor (!(lhs || rhs)).
        /// </summary>
        public static bVector3 Nor(bVector3 lhs, bool rhs) => new bVector3(!(lhs.x || rhs), !(lhs.y || rhs), !(lhs.z || rhs));
        
        /// <summary>
        /// Returns a bVector3 from component-wise application of Nor (!(lhs || rhs)).
        /// </summary>
        public static bVector3 Nor(bool lhs, bVector3 rhs) => new bVector3(!(lhs || rhs.x), !(lhs || rhs.y), !(lhs || rhs.z));
        
        /// <summary>
        /// Returns a bVector from the application of Nor (!(lhs || rhs)).
        /// </summary>
        public static bVector3 Nor(bool lhs, bool rhs) => new bVector3(!(lhs || rhs));
        
        /// <summary>
        /// Returns a bVector3 from component-wise application of Xor (lhs != rhs).
        /// </summary>
        public static bVector3 Xor(bVector3 lhs, bVector3 rhs) => new bVector3(lhs.x != rhs.x, lhs.y != rhs.y, lhs.z != rhs.z);
        
        /// <summary>
        /// Returns a bVector3 from component-wise application of Xor (lhs != rhs).
        /// </summary>
        public static bVector3 Xor(bVector3 lhs, bool rhs) => new bVector3(lhs.x != rhs, lhs.y != rhs, lhs.z != rhs);
        
        /// <summary>
        /// Returns a bVector3 from component-wise application of Xor (lhs != rhs).
        /// </summary>
        public static bVector3 Xor(bool lhs, bVector3 rhs) => new bVector3(lhs != rhs.x, lhs != rhs.y, lhs != rhs.z);
        
        /// <summary>
        /// Returns a bVector from the application of Xor (lhs != rhs).
        /// </summary>
        public static bVector3 Xor(bool lhs, bool rhs) => new bVector3(lhs != rhs);
        
        /// <summary>
        /// Returns a bVector3 from component-wise application of Xnor (lhs == rhs).
        /// </summary>
        public static bVector3 Xnor(bVector3 lhs, bVector3 rhs) => new bVector3(lhs.x == rhs.x, lhs.y == rhs.y, lhs.z == rhs.z);
        
        /// <summary>
        /// Returns a bVector3 from component-wise application of Xnor (lhs == rhs).
        /// </summary>
        public static bVector3 Xnor(bVector3 lhs, bool rhs) => new bVector3(lhs.x == rhs, lhs.y == rhs, lhs.z == rhs);
        
        /// <summary>
        /// Returns a bVector3 from component-wise application of Xnor (lhs == rhs).
        /// </summary>
        public static bVector3 Xnor(bool lhs, bVector3 rhs) => new bVector3(lhs == rhs.x, lhs == rhs.y, lhs == rhs.z);
        
        /// <summary>
        /// Returns a bVector from the application of Xnor (lhs == rhs).
        /// </summary>
        public static bVector3 Xnor(bool lhs, bool rhs) => new bVector3(lhs == rhs);

        #endregion


        #region Component-Wise Operator Overloads
        
        /// <summary>
        /// Returns a bVector3 from component-wise application of operator! (!v).
        /// </summary>
        public static bVector3 operator!(bVector3 v) => new bVector3(!v.x, !v.y, !v.z);
        
        /// <summary>
        /// Returns a bVector3 from component-wise application of operator&amp; (lhs &amp;&amp; rhs).
        /// </summary>
        public static bVector3 operator&(bVector3 lhs, bVector3 rhs) => new bVector3(lhs.x && rhs.x, lhs.y && rhs.y, lhs.z && rhs.z);
        
        /// <summary>
        /// Returns a bVector3 from component-wise application of operator&amp; (lhs &amp;&amp; rhs).
        /// </summary>
        public static bVector3 operator&(bVector3 lhs, bool rhs) => new bVector3(lhs.x && rhs, lhs.y && rhs, lhs.z && rhs);
        
        /// <summary>
        /// Returns a bVector3 from component-wise application of operator&amp; (lhs &amp;&amp; rhs).
        /// </summary>
        public static bVector3 operator&(bool lhs, bVector3 rhs) => new bVector3(lhs && rhs.x, lhs && rhs.y, lhs && rhs.z);
        
        /// <summary>
        /// Returns a bVector3 from component-wise application of operator| (lhs || rhs).
        /// </summary>
        public static bVector3 operator|(bVector3 lhs, bVector3 rhs) => new bVector3(lhs.x || rhs.x, lhs.y || rhs.y, lhs.z || rhs.z);
        
        /// <summary>
        /// Returns a bVector3 from component-wise application of operator| (lhs || rhs).
        /// </summary>
        public static bVector3 operator|(bVector3 lhs, bool rhs) => new bVector3(lhs.x || rhs, lhs.y || rhs, lhs.z || rhs);
        
        /// <summary>
        /// Returns a bVector3 from component-wise application of operator| (lhs || rhs).
        /// </summary>
        public static bVector3 operator|(bool lhs, bVector3 rhs) => new bVector3(lhs || rhs.x, lhs || rhs.y, lhs || rhs.z);

        #endregion

    }
}
