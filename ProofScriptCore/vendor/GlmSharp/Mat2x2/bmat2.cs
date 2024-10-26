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
    /// A Matrix of type bool with 2 columns and 2 rows.
    /// </summary>
    
    
    [StructLayout(LayoutKind.Sequential)]
    public struct bMatrix2 : IReadOnlyList<bool>, IEquatable<bMatrix2>
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
        /// Column 1, Rows 0
        /// </summary>
        
        public bool m10;
        
        /// <summary>
        /// Column 1, Rows 1
        /// </summary>
        
        public bool m11;

        #endregion


        #region Constructors
        
        /// <summary>
        /// Component-wise constructor
        /// </summary>
        public bMatrix2(bool m00, bool m01, bool m10, bool m11)
        {
            this.m00 = m00;
            this.m01 = m01;
            this.m10 = m10;
            this.m11 = m11;
        }
        
        /// <summary>
        /// Constructs this Matrix from a bMatrix2. Non-overwritten fields are from an Identity Matrix.
        /// </summary>
        public bMatrix2(bMatrix2 m)
        {
            this.m00 = m.m00;
            this.m01 = m.m01;
            this.m10 = m.m10;
            this.m11 = m.m11;
        }
        
        /// <summary>
        /// Constructs this Matrix from a bMatrix3x2. Non-overwritten fields are from an Identity Matrix.
        /// </summary>
        public bMatrix2(bMatrix3x2 m)
        {
            this.m00 = m.m00;
            this.m01 = m.m01;
            this.m10 = m.m10;
            this.m11 = m.m11;
        }
        
        /// <summary>
        /// Constructs this Matrix from a bMatrix4x2. Non-overwritten fields are from an Identity Matrix.
        /// </summary>
        public bMatrix2(bMatrix4x2 m)
        {
            this.m00 = m.m00;
            this.m01 = m.m01;
            this.m10 = m.m10;
            this.m11 = m.m11;
        }
        
        /// <summary>
        /// Constructs this Matrix from a bMatrix2x3. Non-overwritten fields are from an Identity Matrix.
        /// </summary>
        public bMatrix2(bMatrix2x3 m)
        {
            this.m00 = m.m00;
            this.m01 = m.m01;
            this.m10 = m.m10;
            this.m11 = m.m11;
        }
        
        /// <summary>
        /// Constructs this Matrix from a bMatrix3. Non-overwritten fields are from an Identity Matrix.
        /// </summary>
        public bMatrix2(bMatrix3 m)
        {
            this.m00 = m.m00;
            this.m01 = m.m01;
            this.m10 = m.m10;
            this.m11 = m.m11;
        }
        
        /// <summary>
        /// Constructs this Matrix from a bMatrix4x3. Non-overwritten fields are from an Identity Matrix.
        /// </summary>
        public bMatrix2(bMatrix4x3 m)
        {
            this.m00 = m.m00;
            this.m01 = m.m01;
            this.m10 = m.m10;
            this.m11 = m.m11;
        }
        
        /// <summary>
        /// Constructs this Matrix from a bMatrix2x4. Non-overwritten fields are from an Identity Matrix.
        /// </summary>
        public bMatrix2(bMatrix2x4 m)
        {
            this.m00 = m.m00;
            this.m01 = m.m01;
            this.m10 = m.m10;
            this.m11 = m.m11;
        }
        
        /// <summary>
        /// Constructs this Matrix from a bMatrix3x4. Non-overwritten fields are from an Identity Matrix.
        /// </summary>
        public bMatrix2(bMatrix3x4 m)
        {
            this.m00 = m.m00;
            this.m01 = m.m01;
            this.m10 = m.m10;
            this.m11 = m.m11;
        }
        
        /// <summary>
        /// Constructs this Matrix from a bMatrix4. Non-overwritten fields are from an Identity Matrix.
        /// </summary>
        public bMatrix2(bMatrix4 m)
        {
            this.m00 = m.m00;
            this.m01 = m.m01;
            this.m10 = m.m10;
            this.m11 = m.m11;
        }
        
        /// <summary>
        /// Constructs this Matrix from a series of column Vectors. Non-overwritten fields are from an Identity Matrix.
        /// </summary>
        public bMatrix2(bVector2 c0, bVector2 c1)
        {
            this.m00 = c0.x;
            this.m01 = c0.y;
            this.m10 = c1.x;
            this.m11 = c1.y;
        }

        #endregion


        #region Properties
        
        /// <summary>
        /// Creates a 2D array with all values (address: Values[x, y])
        /// </summary>
        public bool[,] Values => new[,] { { m00, m01 }, { m10, m11 } };
        
        /// <summary>
        /// Creates a 1D array with all values (internal order)
        /// </summary>
        public bool[] Values1D => new[] { m00, m01, m10, m11 };
        
        /// <summary>
        /// Gets or sets the column nr 0
        /// </summary>
        public bVector2 Column0
        {
            get
            {
                return new bVector2(m00, m01);
            }
            set
            {
                m00 = value.x;
                m01 = value.y;
            }
        }
        
        /// <summary>
        /// Gets or sets the column nr 1
        /// </summary>
        public bVector2 Column1
        {
            get
            {
                return new bVector2(m10, m11);
            }
            set
            {
                m10 = value.x;
                m11 = value.y;
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

        #endregion


        #region Static Properties
        
        /// <summary>
        /// Predefined all-zero Matrix
        /// </summary>
        public static bMatrix2 Zero { get; } = new bMatrix2(false, false, false, false);
        
        /// <summary>
        /// Predefined all-ones Matrix
        /// </summary>
        public static bMatrix2 Ones { get; } = new bMatrix2(true, true, true, true);
        
        /// <summary>
        /// Predefined identity Matrix
        /// </summary>
        public static bMatrix2 Identity { get; } = new bMatrix2(true, false, false, true);

        #endregion


        #region Functions
        
        /// <summary>
        /// Returns an enumerator that iterates through all fields.
        /// </summary>
        public IEnumerator<bool> GetEnumerator()
        {
            yield return m00;
            yield return m01;
            yield return m10;
            yield return m11;
        }
        
        /// <summary>
        /// Returns an enumerator that iterates through all fields.
        /// </summary>
        IEnumerator IEnumerable.GetEnumerator() => GetEnumerator();

        #endregion

        
        /// <summary>
        /// Returns the number of Fields (2 x 2 = 4).
        /// </summary>
        public int Count => 4;
        
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
                    case 2: return m10;
                    case 3: return m11;
                    default: throw new ArgumentOutOfRangeException("fieldIndex");
                }
            }
            set
            {
                switch (fieldIndex)
                {
                    case 0: this.m00 = value; break;
                    case 1: this.m01 = value; break;
                    case 2: this.m10 = value; break;
                    case 3: this.m11 = value; break;
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
                return this[col * 2 + row];
            }
            set
            {
                this[col * 2 + row] = value;
            }
        }
        
        /// <summary>
        /// Returns true iff this equals rhs component-wise.
        /// </summary>
        public bool Equals(bMatrix2 rhs) => ((m00.Equals(rhs.m00) && m01.Equals(rhs.m01)) && (m10.Equals(rhs.m10) && m11.Equals(rhs.m11)));
        
        /// <summary>
        /// Returns true iff this equals rhs type- and component-wise.
        /// </summary>
        public override bool Equals(object obj)
        {
            if (ReferenceEquals(null, obj)) return false;
            return obj is bMatrix2 && Equals((bMatrix2) obj);
        }
        
        /// <summary>
        /// Returns true iff this equals rhs component-wise.
        /// </summary>
        public static bool operator ==(bMatrix2 lhs, bMatrix2 rhs) => lhs.Equals(rhs);
        
        /// <summary>
        /// Returns true iff this does not equal rhs (component-wise).
        /// </summary>
        public static bool operator !=(bMatrix2 lhs, bMatrix2 rhs) => !lhs.Equals(rhs);
        
        /// <summary>
        /// Returns a hash code for this instance.
        /// </summary>
        public override int GetHashCode()
        {
            unchecked
            {
                return ((((((m00.GetHashCode()) * 2) ^ m01.GetHashCode()) * 2) ^ m10.GetHashCode()) * 2) ^ m11.GetHashCode();
            }
        }
        
        /// <summary>
        /// Returns a transposed version of this Matrix.
        /// </summary>
        public bMatrix2 Transposed => new bMatrix2(m00, m10, m01, m11);
        
        /// <summary>
        /// Returns the minimal component of this Matrix.
        /// </summary>
        public bool MinElement => ((m00 && m01) && (m10 && m11));
        
        /// <summary>
        /// Returns the maximal component of this Matrix.
        /// </summary>
        public bool MaxElement => ((m00 || m01) || (m10 || m11));
        
        /// <summary>
        /// Returns true if all component are true.
        /// </summary>
        public bool All => ((m00 && m01) && (m10 && m11));
        
        /// <summary>
        /// Returns true if any component is true.
        /// </summary>
        public bool Any => ((m00 || m01) || (m10 || m11));
        
        /// <summary>
        /// Executes a component-wise &amp;&amp;. (sorry for different overload but &amp;&amp; cannot be overloaded directly)
        /// </summary>
        public static bMatrix2 operator&(bMatrix2 lhs, bMatrix2 rhs) => new bMatrix2(lhs.m00 && rhs.m00, lhs.m01 && rhs.m01, lhs.m10 && rhs.m10, lhs.m11 && rhs.m11);
        
        /// <summary>
        /// Executes a component-wise ||. (sorry for different overload but || cannot be overloaded directly)
        /// </summary>
        public static bMatrix2 operator|(bMatrix2 lhs, bMatrix2 rhs) => new bMatrix2(lhs.m00 || rhs.m00, lhs.m01 || rhs.m01, lhs.m10 || rhs.m10, lhs.m11 || rhs.m11);
    }
}
