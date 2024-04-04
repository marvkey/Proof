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
    /// A Matrix of type bool with 2 columns and 3 rows.
    /// </summary>
    [RegisterCoreClassStruct]
    
    [StructLayout(LayoutKind.Sequential)]
    public struct bMatrix2x3 : IReadOnlyList<bool>, IEquatable<bMatrix2x3>
    {

        #region Fields
        
        /// <summary>
        /// Column 0, Rows 0
        /// </summary>
        
        public bool m00;
        
        /// <summary>
        /// Column 0, Rows 1
        /// </summary>
        
        public bool m01;
        
        /// <summary>
        /// Column 0, Rows 2
        /// </summary>
        
        public bool m02;
        
        /// <summary>
        /// Column 1, Rows 0
        /// </summary>
        
        public bool m10;
        
        /// <summary>
        /// Column 1, Rows 1
        /// </summary>
        
        public bool m11;
        
        /// <summary>
        /// Column 1, Rows 2
        /// </summary>
        
        public bool m12;

        #endregion


        #region Constructors
        
        /// <summary>
        /// Component-wise constructor
        /// </summary>
        public bMatrix2x3(bool m00, bool m01, bool m02, bool m10, bool m11, bool m12)
        {
            this.m00 = m00;
            this.m01 = m01;
            this.m02 = m02;
            this.m10 = m10;
            this.m11 = m11;
            this.m12 = m12;
        }
        
        /// <summary>
        /// Constructs this Matrix from a bMatrix2. Non-overwritten fields are from an Identity Matrix.
        /// </summary>
        public bMatrix2x3(bMatrix2 m)
        {
            this.m00 = m.m00;
            this.m01 = m.m01;
            this.m02 = false;
            this.m10 = m.m10;
            this.m11 = m.m11;
            this.m12 = false;
        }
        
        /// <summary>
        /// Constructs this Matrix from a bMatrix3x2. Non-overwritten fields are from an Identity Matrix.
        /// </summary>
        public bMatrix2x3(bMatrix3x2 m)
        {
            this.m00 = m.m00;
            this.m01 = m.m01;
            this.m02 = false;
            this.m10 = m.m10;
            this.m11 = m.m11;
            this.m12 = false;
        }
        
        /// <summary>
        /// Constructs this Matrix from a bMatrix4x2. Non-overwritten fields are from an Identity Matrix.
        /// </summary>
        public bMatrix2x3(bMatrix4x2 m)
        {
            this.m00 = m.m00;
            this.m01 = m.m01;
            this.m02 = false;
            this.m10 = m.m10;
            this.m11 = m.m11;
            this.m12 = false;
        }
        
        /// <summary>
        /// Constructs this Matrix from a bMatrix2x3. Non-overwritten fields are from an Identity Matrix.
        /// </summary>
        public bMatrix2x3(bMatrix2x3 m)
        {
            this.m00 = m.m00;
            this.m01 = m.m01;
            this.m02 = m.m02;
            this.m10 = m.m10;
            this.m11 = m.m11;
            this.m12 = m.m12;
        }
        
        /// <summary>
        /// Constructs this Matrix from a bMatrix3. Non-overwritten fields are from an Identity Matrix.
        /// </summary>
        public bMatrix2x3(bMatrix3 m)
        {
            this.m00 = m.m00;
            this.m01 = m.m01;
            this.m02 = m.m02;
            this.m10 = m.m10;
            this.m11 = m.m11;
            this.m12 = m.m12;
        }
        
        /// <summary>
        /// Constructs this Matrix from a bMatrix4x3. Non-overwritten fields are from an Identity Matrix.
        /// </summary>
        public bMatrix2x3(bMatrix4x3 m)
        {
            this.m00 = m.m00;
            this.m01 = m.m01;
            this.m02 = m.m02;
            this.m10 = m.m10;
            this.m11 = m.m11;
            this.m12 = m.m12;
        }
        
        /// <summary>
        /// Constructs this Matrix from a bMatrix2x4. Non-overwritten fields are from an Identity Matrix.
        /// </summary>
        public bMatrix2x3(bMatrix2x4 m)
        {
            this.m00 = m.m00;
            this.m01 = m.m01;
            this.m02 = m.m02;
            this.m10 = m.m10;
            this.m11 = m.m11;
            this.m12 = m.m12;
        }
        
        /// <summary>
        /// Constructs this Matrix from a bMatrix3x4. Non-overwritten fields are from an Identity Matrix.
        /// </summary>
        public bMatrix2x3(bMatrix3x4 m)
        {
            this.m00 = m.m00;
            this.m01 = m.m01;
            this.m02 = m.m02;
            this.m10 = m.m10;
            this.m11 = m.m11;
            this.m12 = m.m12;
        }
        
        /// <summary>
        /// Constructs this Matrix from a bMatrix4. Non-overwritten fields are from an Identity Matrix.
        /// </summary>
        public bMatrix2x3(bMatrix4 m)
        {
            this.m00 = m.m00;
            this.m01 = m.m01;
            this.m02 = m.m02;
            this.m10 = m.m10;
            this.m11 = m.m11;
            this.m12 = m.m12;
        }
        
        /// <summary>
        /// Constructs this Matrix from a series of column Vectortors. Non-overwritten fields are from an Identity Matrix.
        /// </summary>
        public bMatrix2x3(bVector2 c0, bVector2 c1)
        {
            this.m00 = c0.x;
            this.m01 = c0.y;
            this.m02 = false;
            this.m10 = c1.x;
            this.m11 = c1.y;
            this.m12 = false;
        }
        
        /// <summary>
        /// Constructs this Matrix from a series of column Vectortors. Non-overwritten fields are from an Identity Matrix.
        /// </summary>
        public bMatrix2x3(bVector3 c0, bVector3 c1)
        {
            this.m00 = c0.x;
            this.m01 = c0.y;
            this.m02 = c0.z;
            this.m10 = c1.x;
            this.m11 = c1.y;
            this.m12 = c1.z;
        }

        #endregion


        #region Properties
        
        /// <summary>
        /// Creates a 2D array with all values (address: Values[x, y])
        /// </summary>
        public bool[,] Values => new[,] { { m00, m01, m02 }, { m10, m11, m12 } };
        
        /// <summary>
        /// Creates a 1D array with all values (internal order)
        /// </summary>
        public bool[] Values1D => new[] { m00, m01, m02, m10, m11, m12 };
        
        /// <summary>
        /// Gets or sets the column nr 0
        /// </summary>
        public bVector3 Column0
        {
            get
            {
                return new bVector3(m00, m01, m02);
            }
            set
            {
                m00 = value.x;
                m01 = value.y;
                m02 = value.z;
            }
        }
        
        /// <summary>
        /// Gets or sets the column nr 1
        /// </summary>
        public bVector3 Column1
        {
            get
            {
                return new bVector3(m10, m11, m12);
            }
            set
            {
                m10 = value.x;
                m11 = value.y;
                m12 = value.z;
            }
        }
        
        /// <summary>
        /// Gets or sets the row nr 0
        /// </summary>
        public bVector2 Row0
        {
            get
            {
                return new bVector2(m00, m10);
            }
            set
            {
                m00 = value.x;
                m10 = value.y;
            }
        }
        
        /// <summary>
        /// Gets or sets the row nr 1
        /// </summary>
        public bVector2 Row1
        {
            get
            {
                return new bVector2(m01, m11);
            }
            set
            {
                m01 = value.x;
                m11 = value.y;
            }
        }
        
        /// <summary>
        /// Gets or sets the row nr 2
        /// </summary>
        public bVector2 Row2
        {
            get
            {
                return new bVector2(m02, m12);
            }
            set
            {
                m02 = value.x;
                m12 = value.y;
            }
        }

        #endregion


        #region Static Properties
        
        /// <summary>
        /// Predefined all-zero Matrix
        /// </summary>
        public static bMatrix2x3 Zero { get; } = new bMatrix2x3(false, false, false, false, false, false);
        
        /// <summary>
        /// Predefined all-ones Matrix
        /// </summary>
        public static bMatrix2x3 Ones { get; } = new bMatrix2x3(true, true, true, true, true, true);
        
        /// <summary>
        /// Predefined identity Matrix
        /// </summary>
        public static bMatrix2x3 Identity { get; } = new bMatrix2x3(true, false, false, false, true, false);

        #endregion


        #region Functions
        
        /// <summary>
        /// Returns an enumerator that iterates through all fields.
        /// </summary>
        public IEnumerator<bool> GetEnumerator()
        {
            yield return m00;
            yield return m01;
            yield return m02;
            yield return m10;
            yield return m11;
            yield return m12;
        }
        
        /// <summary>
        /// Returns an enumerator that iterates through all fields.
        /// </summary>
        IEnumerator IEnumerable.GetEnumerator() => GetEnumerator();

        #endregion

        
        /// <summary>
        /// Returns the number of Fields (2 x 3 = 6).
        /// </summary>
        public int Count => 6;
        
        /// <summary>
        /// Gets/Sets a specific indexed component (a bit slower than direct access).
        /// </summary>
        public bool this[int fieldIndex]
        {
            get
            {
                switch (fieldIndex)
                {
                    case 0: return m00;
                    case 1: return m01;
                    case 2: return m02;
                    case 3: return m10;
                    case 4: return m11;
                    case 5: return m12;
                    default: throw new ArgumentOutOfRangeException("fieldIndex");
                }
            }
            set
            {
                switch (fieldIndex)
                {
                    case 0: this.m00 = value; break;
                    case 1: this.m01 = value; break;
                    case 2: this.m02 = value; break;
                    case 3: this.m10 = value; break;
                    case 4: this.m11 = value; break;
                    case 5: this.m12 = value; break;
                    default: throw new ArgumentOutOfRangeException("fieldIndex");
                }
            }
        }
        
        /// <summary>
        /// Gets/Sets a specific 2D-indexed component (a bit slower than direct access).
        /// </summary>
        public bool this[int col, int row]
        {
            get
            {
                return this[col * 3 + row];
            }
            set
            {
                this[col * 3 + row] = value;
            }
        }
        
        /// <summary>
        /// Returns true iff this equals rhs component-wise.
        /// </summary>
        public bool Equals(bMatrix2x3 rhs) => (((m00.Equals(rhs.m00) && m01.Equals(rhs.m01)) && m02.Equals(rhs.m02)) && ((m10.Equals(rhs.m10) && m11.Equals(rhs.m11)) && m12.Equals(rhs.m12)));
        
        /// <summary>
        /// Returns true iff this equals rhs type- and component-wise.
        /// </summary>
        public override bool Equals(object obj)
        {
            if (ReferenceEquals(null, obj)) return false;
            return obj is bMatrix2x3 && Equals((bMatrix2x3) obj);
        }
        
        /// <summary>
        /// Returns true iff this equals rhs component-wise.
        /// </summary>
        public static bool operator ==(bMatrix2x3 lhs, bMatrix2x3 rhs) => lhs.Equals(rhs);
        
        /// <summary>
        /// Returns true iff this does not equal rhs (component-wise).
        /// </summary>
        public static bool operator !=(bMatrix2x3 lhs, bMatrix2x3 rhs) => !lhs.Equals(rhs);
        
        /// <summary>
        /// Returns a hash code for this instance.
        /// </summary>
        public override int GetHashCode()
        {
            unchecked
            {
                return ((((((((((m00.GetHashCode()) * 2) ^ m01.GetHashCode()) * 2) ^ m02.GetHashCode()) * 2) ^ m10.GetHashCode()) * 2) ^ m11.GetHashCode()) * 2) ^ m12.GetHashCode();
            }
        }
        
        /// <summary>
        /// Returns a transposed version of this Matrix.
        /// </summary>
        public bMatrix3x2 Transposed => new bMatrix3x2(m00, m10, m01, m11, m02, m12);
        
        /// <summary>
        /// Returns the minimal component of this Matrix.
        /// </summary>
        public bool MinElement => (((m00 && m01) && m02) && ((m10 && m11) && m12));
        
        /// <summary>
        /// Returns the maximal component of this Matrix.
        /// </summary>
        public bool MaxElement => (((m00 || m01) || m02) || ((m10 || m11) || m12));
        
        /// <summary>
        /// Returns true if all component are true.
        /// </summary>
        public bool All => (((m00 && m01) && m02) && ((m10 && m11) && m12));
        
        /// <summary>
        /// Returns true if any component is true.
        /// </summary>
        public bool Any => (((m00 || m01) || m02) || ((m10 || m11) || m12));
        
        /// <summary>
        /// Executes a component-wise &amp;&amp;. (sorry for different overload but &amp;&amp; cannot be overloaded directly)
        /// </summary>
        public static bMatrix2x3 operator&(bMatrix2x3 lhs, bMatrix2x3 rhs) => new bMatrix2x3(lhs.m00 && rhs.m00, lhs.m01 && rhs.m01, lhs.m02 && rhs.m02, lhs.m10 && rhs.m10, lhs.m11 && rhs.m11, lhs.m12 && rhs.m12);
        
        /// <summary>
        /// Executes a component-wise ||. (sorry for different overload but || cannot be overloaded directly)
        /// </summary>
        public static bMatrix2x3 operator|(bMatrix2x3 lhs, bMatrix2x3 rhs) => new bMatrix2x3(lhs.m00 || rhs.m00, lhs.m01 || rhs.m01, lhs.m02 || rhs.m02, lhs.m10 || rhs.m10, lhs.m11 || rhs.m11, lhs.m12 || rhs.m12);
    }
}
