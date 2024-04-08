using System;
using System.Collections;
using System.Collections.Generic;
using System.Globalization;
using System.Runtime.InteropServices;

using System.Linq;
using Proof.Swizzle;

// ReSharper disable InconsistentNaming

namespace Proof
{
    
    /// <summary>
    /// A Quaternionernion of type bool.
    /// </summary>
    [RegisterCoreClassStruct]
    
    [StructLayout(LayoutKind.Sequential)]
    public struct bQuaternion : IReadOnlyList<bool>, IEquatable<bQuaternion>
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
        public bQuaternion(bool x, bool y, bool z, bool w)
        {
            this.x = x;
            this.y = y;
            this.z = z;
            this.w = w;
        }
        
        /// <summary>
        /// all-same-value constructor
        /// </summary>
        public bQuaternion(bool v)
        {
            this.x = v;
            this.y = v;
            this.z = v;
            this.w = v;
        }
        
        /// <summary>
        /// copy constructor
        /// </summary>
        public bQuaternion(bQuaternion q)
        {
            this.x = q.x;
            this.y = q.y;
            this.z = q.z;
            this.w = q.w;
        }
        
        /// <summary>
        /// Vector-and-scalar constructor (CAUTION: not angle-axis, use FromAngleAxis instead)
        /// </summary>
        public bQuaternion(bVector3 v, bool s)
        {
            this.x = v.x;
            this.y = v.y;
            this.z = v.z;
            this.w = s;
        }

        #endregion


        #region Explicit Operators
        
        /// <summary>
        /// Explicitly converts this to a iVector4.
        /// </summary>
        public static explicit operator iVector4(bQuaternion v) => new iVector4(v.x ? 1 : 0, v.y ? 1 : 0, v.z ? 1 : 0, v.w ? 1 : 0);
        
        /// <summary>
        /// Explicitly converts this to a iQuaternion.
        /// </summary>
        public static explicit operator iQuaternion(bQuaternion v) => new iQuaternion(v.x ? 1 : 0, v.y ? 1 : 0, v.z ? 1 : 0, v.w ? 1 : 0);
        
        /// <summary>
        /// Explicitly converts this to a uVector4.
        /// </summary>
        public static explicit operator uVector4(bQuaternion v) => new uVector4(v.x ? 1u : 0u, v.y ? 1u : 0u, v.z ? 1u : 0u, v.w ? 1u : 0u);
        
        /// <summary>
        /// Explicitly converts this to a uQuaternion.
        /// </summary>
        public static explicit operator uQuaternion(bQuaternion v) => new uQuaternion(v.x ? 1u : 0u, v.y ? 1u : 0u, v.z ? 1u : 0u, v.w ? 1u : 0u);
        
        /// <summary>
        /// Explicitly converts this to a Vector4.
        /// </summary>
        public static explicit operator Vector4(bQuaternion v) => new Vector4(v.x ? 1f : 0f, v.y ? 1f : 0f, v.z ? 1f : 0f, v.w ? 1f : 0f);
        
        /// <summary>
        /// Explicitly converts this to a Quaternion.
        /// </summary>
        public static explicit operator Quaternion(bQuaternion v) => new Quaternion(v.x ? 1f : 0f, v.y ? 1f : 0f, v.z ? 1f : 0f, v.w ? 1f : 0f);
        
        /// <summary>
        /// Explicitly converts this to a hVector4.
        /// </summary>
        public static explicit operator hVector4(bQuaternion v) => new hVector4(v.x ? Half.One : Half.Zero, v.y ? Half.One : Half.Zero, v.z ? Half.One : Half.Zero, v.w ? Half.One : Half.Zero);
        
        /// <summary>
        /// Explicitly converts this to a hQuaternion.
        /// </summary>
        public static explicit operator hQuaternion(bQuaternion v) => new hQuaternion(v.x ? Half.One : Half.Zero, v.y ? Half.One : Half.Zero, v.z ? Half.One : Half.Zero, v.w ? Half.One : Half.Zero);
        
        /// <summary>
        /// Explicitly converts this to a dVector4.
        /// </summary>
        public static explicit operator dVector4(bQuaternion v) => new dVector4(v.x ? 1.0 : 0.0, v.y ? 1.0 : 0.0, v.z ? 1.0 : 0.0, v.w ? 1.0 : 0.0);
        
        /// <summary>
        /// Explicitly converts this to a dQuaternion.
        /// </summary>
        public static explicit operator dQuaternion(bQuaternion v) => new dQuaternion(v.x ? 1.0 : 0.0, v.y ? 1.0 : 0.0, v.z ? 1.0 : 0.0, v.w ? 1.0 : 0.0);
        
        /// <summary>
        /// Explicitly converts this to a decVector4.
        /// </summary>
        public static explicit operator decVector4(bQuaternion v) => new decVector4(v.x ? 1m : 0m, v.y ? 1m : 0m, v.z ? 1m : 0m, v.w ? 1m : 0m);
        
        /// <summary>
        /// Explicitly converts this to a decQuaternion.
        /// </summary>
        public static explicit operator decQuaternion(bQuaternion v) => new decQuaternion(v.x ? 1m : 0m, v.y ? 1m : 0m, v.z ? 1m : 0m, v.w ? 1m : 0m);
        
        /// <summary>
        /// Explicitly converts this to a cVector4.
        /// </summary>
        //public static explicit operator cVector4(bQuaternion v) => new cVector4(v.x ? Complex.One : Complex.Zero, v.y ? Complex.One : Complex.Zero, v.z ? Complex.One : Complex.Zero, v.w ? Complex.One : Complex.Zero);
        
        /// <summary>
        /// Explicitly converts this to a cQuaternion.
        /// </summary>
        //public static explicit operator cQuaternion(bQuaternion v) => new cQuaternion(v.x ? Complex.One : Complex.Zero, v.y ? Complex.One : Complex.Zero, v.z ? Complex.One : Complex.Zero, v.w ? Complex.One : Complex.Zero);
        
        /// <summary>
        /// Explicitly converts this to a lVector4.
        /// </summary>
        public static explicit operator lVector4(bQuaternion v) => new lVector4(v.x ? 1 : 0, v.y ? 1 : 0, v.z ? 1 : 0, v.w ? 1 : 0);
        
        /// <summary>
        /// Explicitly converts this to a lQuaternion.
        /// </summary>
        public static explicit operator lQuaternion(bQuaternion v) => new lQuaternion(v.x ? 1 : 0, v.y ? 1 : 0, v.z ? 1 : 0, v.w ? 1 : 0);
        
        /// <summary>
        /// Explicitly converts this to a bVector4.
        /// </summary>
        public static explicit operator bVector4(bQuaternion v) => new bVector4((bool)v.x, (bool)v.y, (bool)v.z, (bool)v.w);

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
        /// Returns an array with all values
        /// </summary>
        public bool[] Values => new[] { x, y, z, w };
        
        /// <summary>
        /// Returns the number of components (4).
        /// </summary>
        public int Count => 4;
        
        /// <summary>
        /// Returns the minimal component of this Quaternionernion.
        /// </summary>
        public bool MinElement => ((x && y) && (z && w));
        
        /// <summary>
        /// Returns the maximal component of this Quaternionernion.
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
        /// Predefined all-zero Quaternionernion
        /// </summary>
        public static bQuaternion Zero { get; } = new bQuaternion(false, false, false, false);
        
        /// <summary>
        /// Predefined all-ones Quaternionernion
        /// </summary>
        public static bQuaternion Ones { get; } = new bQuaternion(true, true, true, true);
        
        /// <summary>
        /// Predefined identity Quaternionernion
        /// </summary>
        public static bQuaternion Identity { get; } = new bQuaternion(false, false, false, true);
        
        /// <summary>
        /// Predefined unit-X Quaternionernion
        /// </summary>
        public static bQuaternion UnitX { get; } = new bQuaternion(true, false, false, false);
        
        /// <summary>
        /// Predefined unit-Y Quaternionernion
        /// </summary>
        public static bQuaternion UnitY { get; } = new bQuaternion(false, true, false, false);
        
        /// <summary>
        /// Predefined unit-Z Quaternionernion
        /// </summary>
        public static bQuaternion UnitZ { get; } = new bQuaternion(false, false, true, false);
        
        /// <summary>
        /// Predefined unit-W Quaternionernion
        /// </summary>
        public static bQuaternion UnitW { get; } = new bQuaternion(false, false, false, true);

        #endregion


        #region Operators
        
        /// <summary>
        /// Returns true iff this equals rhs component-wise.
        /// </summary>
        public static bool operator==(bQuaternion lhs, bQuaternion rhs) => lhs.Equals(rhs);
        
        /// <summary>
        /// Returns true iff this does not equal rhs (component-wise).
        /// </summary>
        public static bool operator!=(bQuaternion lhs, bQuaternion rhs) => !lhs.Equals(rhs);

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
        /// Returns a string representation of this Quaternionernion using ', ' as a seperator.
        /// </summary>
        public override string ToString() => ToString(", ");
        
        /// <summary>
        /// Returns a string representation of this Quaternionernion using a provided seperator.
        /// </summary>
        public string ToString(string sep) => ((x + sep + y) + sep + (z + sep + w));
        
        /// <summary>
        /// Returns a string representation of this Quaternionernion using a provided seperator and a forMatrix provider for each component.
        /// </summary>
        public string ToString(string sep, IFormatProvider provider) => ((x.ToString(provider) + sep + y.ToString(provider)) + sep + (z.ToString(provider) + sep + w.ToString(provider)));
        
        /// <summary>
        /// Returns true iff this equals rhs component-wise.
        /// </summary>
        public bool Equals(bQuaternion rhs) => ((x.Equals(rhs.x) && y.Equals(rhs.y)) && (z.Equals(rhs.z) && w.Equals(rhs.w)));
        
        /// <summary>
        /// Returns true iff this equals rhs type- and component-wise.
        /// </summary>
        public override bool Equals(object obj)
        {
            if (ReferenceEquals(null, obj)) return false;
            return obj is bQuaternion && Equals((bQuaternion) obj);
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
        /// Converts the string representation of the Quaternionernion into a Quaternionernion representation (using ', ' as a separator).
        /// </summary>
        public static bQuaternion Parse(string s) => Parse(s, ", ");
        
        /// <summary>
        /// Converts the string representation of the Quaternionernion into a Quaternionernion representation (using a designated separator).
        /// </summary>
        public static bQuaternion Parse(string s, string sep)
        {
            var kvp = s.Split(new[] { sep }, StringSplitOptions.None);
            if (kvp.Length != 4) throw new FormatException("input has not exactly 4 parts");
            return new bQuaternion(bool.Parse(kvp[0].Trim()), bool.Parse(kvp[1].Trim()), bool.Parse(kvp[2].Trim()), bool.Parse(kvp[3].Trim()));
        }
        
        /// <summary>
        /// Tries to convert the string representation of the Quaternionernion into a Quaternionernion representation (using ', ' as a separator), returns false if string was invalid.
        /// </summary>
        public static bool TryParse(string s, out bQuaternion result) => TryParse(s, ", ", out result);
        
        /// <summary>
        /// Tries to convert the string representation of the Quaternionernion into a Quaternionernion representation (using a designated separator), returns false if string was invalid.
        /// </summary>
        public static bool TryParse(string s, string sep, out bQuaternion result)
        {
            result = Zero;
            if (string.IsNullOrEmpty(s)) return false;
            var kvp = s.Split(new[] { sep }, StringSplitOptions.None);
            if (kvp.Length != 4) return false;
            bool x = false, y = false, z = false, w = false;
            var ok = ((bool.TryParse(kvp[0].Trim(), out x) && bool.TryParse(kvp[1].Trim(), out y)) && (bool.TryParse(kvp[2].Trim(), out z) && bool.TryParse(kvp[3].Trim(), out w)));
            result = ok ? new bQuaternion(x, y, z, w) : Zero;
            return ok;
        }

        #endregion


        #region Component-Wise Static Functions
        
        /// <summary>
        /// Returns a bVector4 from component-wise application of Equal (lhs == rhs).
        /// </summary>
        public static bVector4 Equal(bQuaternion lhs, bQuaternion rhs) => new bVector4(lhs.x == rhs.x, lhs.y == rhs.y, lhs.z == rhs.z, lhs.w == rhs.w);
        
        /// <summary>
        /// Returns a bVector4 from component-wise application of Equal (lhs == rhs).
        /// </summary>
        public static bVector4 Equal(bQuaternion lhs, bool rhs) => new bVector4(lhs.x == rhs, lhs.y == rhs, lhs.z == rhs, lhs.w == rhs);
        
        /// <summary>
        /// Returns a bVector4 from component-wise application of Equal (lhs == rhs).
        /// </summary>
        public static bVector4 Equal(bool lhs, bQuaternion rhs) => new bVector4(lhs == rhs.x, lhs == rhs.y, lhs == rhs.z, lhs == rhs.w);
        
        /// <summary>
        /// Returns a bVector from the application of Equal (lhs == rhs).
        /// </summary>
        public static bVector4 Equal(bool lhs, bool rhs) => new bVector4(lhs == rhs);
        
        /// <summary>
        /// Returns a bVector4 from component-wise application of NotEqual (lhs != rhs).
        /// </summary>
        public static bVector4 NotEqual(bQuaternion lhs, bQuaternion rhs) => new bVector4(lhs.x != rhs.x, lhs.y != rhs.y, lhs.z != rhs.z, lhs.w != rhs.w);
        
        /// <summary>
        /// Returns a bVector4 from component-wise application of NotEqual (lhs != rhs).
        /// </summary>
        public static bVector4 NotEqual(bQuaternion lhs, bool rhs) => new bVector4(lhs.x != rhs, lhs.y != rhs, lhs.z != rhs, lhs.w != rhs);
        
        /// <summary>
        /// Returns a bVector4 from component-wise application of NotEqual (lhs != rhs).
        /// </summary>
        public static bVector4 NotEqual(bool lhs, bQuaternion rhs) => new bVector4(lhs != rhs.x, lhs != rhs.y, lhs != rhs.z, lhs != rhs.w);
        
        /// <summary>
        /// Returns a bVector from the application of NotEqual (lhs != rhs).
        /// </summary>
        public static bVector4 NotEqual(bool lhs, bool rhs) => new bVector4(lhs != rhs);
        
        /// <summary>
        /// Returns a bVector4 from component-wise application of Not (!v).
        /// </summary>
        public static bVector4 Not(bQuaternion v) => new bVector4(!v.x, !v.y, !v.z, !v.w);
        
        /// <summary>
        /// Returns a bVector from the application of Not (!v).
        /// </summary>
        public static bVector4 Not(bool v) => new bVector4(!v);
        
        /// <summary>
        /// Returns a bQuaternion from component-wise application of And (lhs &amp;&amp; rhs).
        /// </summary>
        public static bQuaternion And(bQuaternion lhs, bQuaternion rhs) => new bQuaternion(lhs.x && rhs.x, lhs.y && rhs.y, lhs.z && rhs.z, lhs.w && rhs.w);
        
        /// <summary>
        /// Returns a bQuaternion from component-wise application of And (lhs &amp;&amp; rhs).
        /// </summary>
        public static bQuaternion And(bQuaternion lhs, bool rhs) => new bQuaternion(lhs.x && rhs, lhs.y && rhs, lhs.z && rhs, lhs.w && rhs);
        
        /// <summary>
        /// Returns a bQuaternion from component-wise application of And (lhs &amp;&amp; rhs).
        /// </summary>
        public static bQuaternion And(bool lhs, bQuaternion rhs) => new bQuaternion(lhs && rhs.x, lhs && rhs.y, lhs && rhs.z, lhs && rhs.w);
        
        /// <summary>
        /// Returns a bQuaternion from the application of And (lhs &amp;&amp; rhs).
        /// </summary>
        public static bQuaternion And(bool lhs, bool rhs) => new bQuaternion(lhs && rhs);
        
        /// <summary>
        /// Returns a bQuaternion from component-wise application of Nand (!(lhs &amp;&amp; rhs)).
        /// </summary>
        public static bQuaternion Nand(bQuaternion lhs, bQuaternion rhs) => new bQuaternion(!(lhs.x && rhs.x), !(lhs.y && rhs.y), !(lhs.z && rhs.z), !(lhs.w && rhs.w));
        
        /// <summary>
        /// Returns a bQuaternion from component-wise application of Nand (!(lhs &amp;&amp; rhs)).
        /// </summary>
        public static bQuaternion Nand(bQuaternion lhs, bool rhs) => new bQuaternion(!(lhs.x && rhs), !(lhs.y && rhs), !(lhs.z && rhs), !(lhs.w && rhs));
        
        /// <summary>
        /// Returns a bQuaternion from component-wise application of Nand (!(lhs &amp;&amp; rhs)).
        /// </summary>
        public static bQuaternion Nand(bool lhs, bQuaternion rhs) => new bQuaternion(!(lhs && rhs.x), !(lhs && rhs.y), !(lhs && rhs.z), !(lhs && rhs.w));
        
        /// <summary>
        /// Returns a bQuaternion from the application of Nand (!(lhs &amp;&amp; rhs)).
        /// </summary>
        public static bQuaternion Nand(bool lhs, bool rhs) => new bQuaternion(!(lhs && rhs));
        
        /// <summary>
        /// Returns a bQuaternion from component-wise application of Or (lhs || rhs).
        /// </summary>
        public static bQuaternion Or(bQuaternion lhs, bQuaternion rhs) => new bQuaternion(lhs.x || rhs.x, lhs.y || rhs.y, lhs.z || rhs.z, lhs.w || rhs.w);
        
        /// <summary>
        /// Returns a bQuaternion from component-wise application of Or (lhs || rhs).
        /// </summary>
        public static bQuaternion Or(bQuaternion lhs, bool rhs) => new bQuaternion(lhs.x || rhs, lhs.y || rhs, lhs.z || rhs, lhs.w || rhs);
        
        /// <summary>
        /// Returns a bQuaternion from component-wise application of Or (lhs || rhs).
        /// </summary>
        public static bQuaternion Or(bool lhs, bQuaternion rhs) => new bQuaternion(lhs || rhs.x, lhs || rhs.y, lhs || rhs.z, lhs || rhs.w);
        
        /// <summary>
        /// Returns a bQuaternion from the application of Or (lhs || rhs).
        /// </summary>
        public static bQuaternion Or(bool lhs, bool rhs) => new bQuaternion(lhs || rhs);
        
        /// <summary>
        /// Returns a bQuaternion from component-wise application of Nor (!(lhs || rhs)).
        /// </summary>
        public static bQuaternion Nor(bQuaternion lhs, bQuaternion rhs) => new bQuaternion(!(lhs.x || rhs.x), !(lhs.y || rhs.y), !(lhs.z || rhs.z), !(lhs.w || rhs.w));
        
        /// <summary>
        /// Returns a bQuaternion from component-wise application of Nor (!(lhs || rhs)).
        /// </summary>
        public static bQuaternion Nor(bQuaternion lhs, bool rhs) => new bQuaternion(!(lhs.x || rhs), !(lhs.y || rhs), !(lhs.z || rhs), !(lhs.w || rhs));
        
        /// <summary>
        /// Returns a bQuaternion from component-wise application of Nor (!(lhs || rhs)).
        /// </summary>
        public static bQuaternion Nor(bool lhs, bQuaternion rhs) => new bQuaternion(!(lhs || rhs.x), !(lhs || rhs.y), !(lhs || rhs.z), !(lhs || rhs.w));
        
        /// <summary>
        /// Returns a bQuaternion from the application of Nor (!(lhs || rhs)).
        /// </summary>
        public static bQuaternion Nor(bool lhs, bool rhs) => new bQuaternion(!(lhs || rhs));
        
        /// <summary>
        /// Returns a bQuaternion from component-wise application of Xor (lhs != rhs).
        /// </summary>
        public static bQuaternion Xor(bQuaternion lhs, bQuaternion rhs) => new bQuaternion(lhs.x != rhs.x, lhs.y != rhs.y, lhs.z != rhs.z, lhs.w != rhs.w);
        
        /// <summary>
        /// Returns a bQuaternion from component-wise application of Xor (lhs != rhs).
        /// </summary>
        public static bQuaternion Xor(bQuaternion lhs, bool rhs) => new bQuaternion(lhs.x != rhs, lhs.y != rhs, lhs.z != rhs, lhs.w != rhs);
        
        /// <summary>
        /// Returns a bQuaternion from component-wise application of Xor (lhs != rhs).
        /// </summary>
        public static bQuaternion Xor(bool lhs, bQuaternion rhs) => new bQuaternion(lhs != rhs.x, lhs != rhs.y, lhs != rhs.z, lhs != rhs.w);
        
        /// <summary>
        /// Returns a bQuaternion from the application of Xor (lhs != rhs).
        /// </summary>
        public static bQuaternion Xor(bool lhs, bool rhs) => new bQuaternion(lhs != rhs);
        
        /// <summary>
        /// Returns a bQuaternion from component-wise application of Xnor (lhs == rhs).
        /// </summary>
        public static bQuaternion Xnor(bQuaternion lhs, bQuaternion rhs) => new bQuaternion(lhs.x == rhs.x, lhs.y == rhs.y, lhs.z == rhs.z, lhs.w == rhs.w);
        
        /// <summary>
        /// Returns a bQuaternion from component-wise application of Xnor (lhs == rhs).
        /// </summary>
        public static bQuaternion Xnor(bQuaternion lhs, bool rhs) => new bQuaternion(lhs.x == rhs, lhs.y == rhs, lhs.z == rhs, lhs.w == rhs);
        
        /// <summary>
        /// Returns a bQuaternion from component-wise application of Xnor (lhs == rhs).
        /// </summary>
        public static bQuaternion Xnor(bool lhs, bQuaternion rhs) => new bQuaternion(lhs == rhs.x, lhs == rhs.y, lhs == rhs.z, lhs == rhs.w);
        
        /// <summary>
        /// Returns a bQuaternion from the application of Xnor (lhs == rhs).
        /// </summary>
        public static bQuaternion Xnor(bool lhs, bool rhs) => new bQuaternion(lhs == rhs);

        #endregion


        #region Component-Wise Operator Overloads
        
        /// <summary>
        /// Returns a bQuaternion from component-wise application of operator! (!v).
        /// </summary>
        public static bQuaternion operator!(bQuaternion v) => new bQuaternion(!v.x, !v.y, !v.z, !v.w);
        
        /// <summary>
        /// Returns a bQuaternion from component-wise application of operator&amp; (lhs &amp;&amp; rhs).
        /// </summary>
        public static bQuaternion operator&(bQuaternion lhs, bQuaternion rhs) => new bQuaternion(lhs.x && rhs.x, lhs.y && rhs.y, lhs.z && rhs.z, lhs.w && rhs.w);
        
        /// <summary>
        /// Returns a bQuaternion from component-wise application of operator&amp; (lhs &amp;&amp; rhs).
        /// </summary>
        public static bQuaternion operator&(bQuaternion lhs, bool rhs) => new bQuaternion(lhs.x && rhs, lhs.y && rhs, lhs.z && rhs, lhs.w && rhs);
        
        /// <summary>
        /// Returns a bQuaternion from component-wise application of operator&amp; (lhs &amp;&amp; rhs).
        /// </summary>
        public static bQuaternion operator&(bool lhs, bQuaternion rhs) => new bQuaternion(lhs && rhs.x, lhs && rhs.y, lhs && rhs.z, lhs && rhs.w);
        
        /// <summary>
        /// Returns a bQuaternion from component-wise application of operator| (lhs || rhs).
        /// </summary>
        public static bQuaternion operator|(bQuaternion lhs, bQuaternion rhs) => new bQuaternion(lhs.x || rhs.x, lhs.y || rhs.y, lhs.z || rhs.z, lhs.w || rhs.w);
        
        /// <summary>
        /// Returns a bQuaternion from component-wise application of operator| (lhs || rhs).
        /// </summary>
        public static bQuaternion operator|(bQuaternion lhs, bool rhs) => new bQuaternion(lhs.x || rhs, lhs.y || rhs, lhs.z || rhs, lhs.w || rhs);
        
        /// <summary>
        /// Returns a bQuaternion from component-wise application of operator| (lhs || rhs).
        /// </summary>
        public static bQuaternion operator|(bool lhs, bQuaternion rhs) => new bQuaternion(lhs || rhs.x, lhs || rhs.y, lhs || rhs.z, lhs || rhs.w);

        #endregion

    }
}
