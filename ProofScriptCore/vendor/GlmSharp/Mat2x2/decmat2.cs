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
    /// A Matrix of type decimal with 2 columns and 2 rows.
    /// </summary>
    [RegisterCoreClassStruct]
    
    [StructLayout(LayoutKind.Sequential)]
    public struct decMatrix2 : IReadOnlyList<decimal>, IEquatable<decMatrix2>
    {

        #region Fields
        
        /// <summary>
        /// Column 0, Rows 0
        /// </summary>
        
        public decimal m00;
        
        /// <summary>
        /// Column 0, Rows 1
        /// </summary>
        
        public decimal m01;
        
        /// <summary>
        /// Column 1, Rows 0
        /// </summary>
        
        public decimal m10;
        
        /// <summary>
        /// Column 1, Rows 1
        /// </summary>
        
        public decimal m11;

        #endregion


        #region Constructors
        
        /// <summary>
        /// Component-wise constructor
        /// </summary>
        public decMatrix2(decimal m00, decimal m01, decimal m10, decimal m11)
        {
            this.m00 = m00;
            this.m01 = m01;
            this.m10 = m10;
            this.m11 = m11;
        }
        
        /// <summary>
        /// Constructs this Matrix from a decMatrix2. Non-overwritten fields are from an Identity Matrix.
        /// </summary>
        public decMatrix2(decMatrix2 m)
        {
            this.m00 = m.m00;
            this.m01 = m.m01;
            this.m10 = m.m10;
            this.m11 = m.m11;
        }
        
        /// <summary>
        /// Constructs this Matrix from a decMatrix3x2. Non-overwritten fields are from an Identity Matrix.
        /// </summary>
        public decMatrix2(decMatrix3x2 m)
        {
            this.m00 = m.m00;
            this.m01 = m.m01;
            this.m10 = m.m10;
            this.m11 = m.m11;
        }
        
        /// <summary>
        /// Constructs this Matrix from a decMatrix4x2. Non-overwritten fields are from an Identity Matrix.
        /// </summary>
        public decMatrix2(decMatrix4x2 m)
        {
            this.m00 = m.m00;
            this.m01 = m.m01;
            this.m10 = m.m10;
            this.m11 = m.m11;
        }
        
        /// <summary>
        /// Constructs this Matrix from a decMatrix2x3. Non-overwritten fields are from an Identity Matrix.
        /// </summary>
        public decMatrix2(decMatrix2x3 m)
        {
            this.m00 = m.m00;
            this.m01 = m.m01;
            this.m10 = m.m10;
            this.m11 = m.m11;
        }
        
        /// <summary>
        /// Constructs this Matrix from a decMatrix3. Non-overwritten fields are from an Identity Matrix.
        /// </summary>
        public decMatrix2(decMatrix3 m)
        {
            this.m00 = m.m00;
            this.m01 = m.m01;
            this.m10 = m.m10;
            this.m11 = m.m11;
        }
        
        /// <summary>
        /// Constructs this Matrix from a decMatrix4x3. Non-overwritten fields are from an Identity Matrix.
        /// </summary>
        public decMatrix2(decMatrix4x3 m)
        {
            this.m00 = m.m00;
            this.m01 = m.m01;
            this.m10 = m.m10;
            this.m11 = m.m11;
        }
        
        /// <summary>
        /// Constructs this Matrix from a decMatrix2x4. Non-overwritten fields are from an Identity Matrix.
        /// </summary>
        public decMatrix2(decMatrix2x4 m)
        {
            this.m00 = m.m00;
            this.m01 = m.m01;
            this.m10 = m.m10;
            this.m11 = m.m11;
        }
        
        /// <summary>
        /// Constructs this Matrix from a decMatrix3x4. Non-overwritten fields are from an Identity Matrix.
        /// </summary>
        public decMatrix2(decMatrix3x4 m)
        {
            this.m00 = m.m00;
            this.m01 = m.m01;
            this.m10 = m.m10;
            this.m11 = m.m11;
        }
        
        /// <summary>
        /// Constructs this Matrix from a decMatrix4. Non-overwritten fields are from an Identity Matrix.
        /// </summary>
        public decMatrix2(decMatrix4 m)
        {
            this.m00 = m.m00;
            this.m01 = m.m01;
            this.m10 = m.m10;
            this.m11 = m.m11;
        }
        
        /// <summary>
        /// Constructs this Matrix from a series of column Vectortors. Non-overwritten fields are from an Identity Matrix.
        /// </summary>
        public decMatrix2(decVector2 c0, decVector2 c1)
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
        public decimal[,] Values => new[,] { { m00, m01 }, { m10, m11 } };
        
        /// <summary>
        /// Creates a 1D array with all values (internal order)
        /// </summary>
        public decimal[] Values1D => new[] { m00, m01, m10, m11 };
        
        /// <summary>
        /// Gets or sets the column nr 0
        /// </summary>
        public decVector2 Column0
        {
            get
            {
                return new decVector2(m00, m01);
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
        public decVector2 Column1
        {
            get
            {
                return new decVector2(m10, m11);
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
        public decVector2 Row0
        {
            get
            {
                return new decVector2(m00, m10);
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
        public decVector2 Row1
        {
            get
            {
                return new decVector2(m01, m11);
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
        public static decMatrix2 Zero { get; } = new decMatrix2(0m, 0m, 0m, 0m);
        
        /// <summary>
        /// Predefined all-ones Matrix
        /// </summary>
        public static decMatrix2 Ones { get; } = new decMatrix2(1m, 1m, 1m, 1m);
        
        /// <summary>
        /// Predefined identity Matrix
        /// </summary>
        public static decMatrix2 Identity { get; } = new decMatrix2(1m, 0m, 0m, 1m);
        
        /// <summary>
        /// Predefined all-MaxValue Matrix
        /// </summary>
        public static decMatrix2 AllMaxValue { get; } = new decMatrix2(decimal.MaxValue, decimal.MaxValue, decimal.MaxValue, decimal.MaxValue);
        
        /// <summary>
        /// Predefined diagonal-MaxValue Matrix
        /// </summary>
        public static decMatrix2 DiagonalMaxValue { get; } = new decMatrix2(decimal.MaxValue, 0m, 0m, decimal.MaxValue);
        
        /// <summary>
        /// Predefined all-MinValue Matrix
        /// </summary>
        public static decMatrix2 AllMinValue { get; } = new decMatrix2(decimal.MinValue, decimal.MinValue, decimal.MinValue, decimal.MinValue);
        
        /// <summary>
        /// Predefined diagonal-MinValue Matrix
        /// </summary>
        public static decMatrix2 DiagonalMinValue { get; } = new decMatrix2(decimal.MinValue, 0m, 0m, decimal.MinValue);
        
        /// <summary>
        /// Predefined all-MinusOne Matrix
        /// </summary>
        public static decMatrix2 AllMinusOne { get; } = new decMatrix2(decimal.MinusOne, decimal.MinusOne, decimal.MinusOne, decimal.MinusOne);
        
        /// <summary>
        /// Predefined diagonal-MinusOne Matrix
        /// </summary>
        public static decMatrix2 DiagonalMinusOne { get; } = new decMatrix2(decimal.MinusOne, 0m, 0m, decimal.MinusOne);

        #endregion


        #region Functions
        
        /// <summary>
        /// Returns an enumerator that iterates through all fields.
        /// </summary>
        public IEnumerator<decimal> GetEnumerator()
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
        public decimal this[int fieldIndex]
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
        public decimal this[int col, int row]
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
        public bool Equals(decMatrix2 rhs) => ((m00.Equals(rhs.m00) && m01.Equals(rhs.m01)) && (m10.Equals(rhs.m10) && m11.Equals(rhs.m11)));
        
        /// <summary>
        /// Returns true iff this equals rhs type- and component-wise.
        /// </summary>
        public override bool Equals(object obj)
        {
            if (ReferenceEquals(null, obj)) return false;
            return obj is decMatrix2 && Equals((decMatrix2) obj);
        }
        
        /// <summary>
        /// Returns true iff this equals rhs component-wise.
        /// </summary>
        public static bool operator ==(decMatrix2 lhs, decMatrix2 rhs) => lhs.Equals(rhs);
        
        /// <summary>
        /// Returns true iff this does not equal rhs (component-wise).
        /// </summary>
        public static bool operator !=(decMatrix2 lhs, decMatrix2 rhs) => !lhs.Equals(rhs);
        
        /// <summary>
        /// Returns a hash code for this instance.
        /// </summary>
        public override int GetHashCode()
        {
            unchecked
            {
                return ((((((m00.GetHashCode()) * 397) ^ m01.GetHashCode()) * 397) ^ m10.GetHashCode()) * 397) ^ m11.GetHashCode();
            }
        }
        
        /// <summary>
        /// Returns a transposed version of this Matrix.
        /// </summary>
        public decMatrix2 Transposed => new decMatrix2(m00, m10, m01, m11);
        
        /// <summary>
        /// Returns the minimal component of this Matrix.
        /// </summary>
        public decimal MinElement => Math.Min(Math.Min(Math.Min(m00, m01), m10), m11);
        
        /// <summary>
        /// Returns the maximal component of this Matrix.
        /// </summary>
        public decimal MaxElement => Math.Max(Math.Max(Math.Max(m00, m01), m10), m11);
        
        /// <summary>
        /// Returns the euclidean length of this Matrix.
        /// </summary>
        public decimal Length => (decimal)(((m00*m00 + m01*m01) + (m10*m10 + m11*m11))).Sqrt();
        
        /// <summary>
        /// Returns the squared euclidean length of this Matrix.
        /// </summary>
        public decimal LengthSqr => ((m00*m00 + m01*m01) + (m10*m10 + m11*m11));
        
        /// <summary>
        /// Returns the sum of all fields.
        /// </summary>
        public decimal Sum => ((m00 + m01) + (m10 + m11));
        
        /// <summary>
        /// Returns the euclidean norm of this Matrix.
        /// </summary>
        public decimal Norm => (decimal)(((m00*m00 + m01*m01) + (m10*m10 + m11*m11))).Sqrt();
        
        /// <summary>
        /// Returns the one-norm of this Matrix.
        /// </summary>
        public decimal Norm1 => ((Math.Abs(m00) + Math.Abs(m01)) + (Math.Abs(m10) + Math.Abs(m11)));
        
        /// <summary>
        /// Returns the two-norm of this Matrix.
        /// </summary>
        public decimal Norm2 => (decimal)(((m00*m00 + m01*m01) + (m10*m10 + m11*m11))).Sqrt();
        
        /// <summary>
        /// Returns the max-norm of this Matrix.
        /// </summary>
        public decimal NormMax => Math.Max(Math.Max(Math.Max(Math.Abs(m00), Math.Abs(m01)), Math.Abs(m10)), Math.Abs(m11));
        
        /// <summary>
        /// Returns the p-norm of this Matrix.
        /// </summary>
        public double NormP(double p) => Math.Pow(((Math.Pow((double)Math.Abs(m00), p) + Math.Pow((double)Math.Abs(m01), p)) + (Math.Pow((double)Math.Abs(m10), p) + Math.Pow((double)Math.Abs(m11), p))), 1 / p);
        
        /// <summary>
        /// Returns determinant of this Matrix.
        /// </summary>
        public decimal Determinant => m00 * m11 - m10 * m01;
        
        /// <summary>
        /// Returns the adjunct of this Matrix.
        /// </summary>
        public decMatrix2 Adjugate => new decMatrix2(m11, -m01, -m10, m00);
        
        /// <summary>
        /// Returns the inverse of this Matrix (use with caution).
        /// </summary>
        public decMatrix2 Inverse => Adjugate / Determinant;
        
        /// <summary>
        /// Executes a Matrix-Matrix-multiplication decMatrix2 * decMatrix2 -> decMatrix2.
        /// </summary>
        public static decMatrix2 operator*(decMatrix2 lhs, decMatrix2 rhs) => new decMatrix2((lhs.m00 * rhs.m00 + lhs.m10 * rhs.m01), (lhs.m01 * rhs.m00 + lhs.m11 * rhs.m01), (lhs.m00 * rhs.m10 + lhs.m10 * rhs.m11), (lhs.m01 * rhs.m10 + lhs.m11 * rhs.m11));
        
        /// <summary>
        /// Executes a Matrix-Matrix-multiplication decMatrix2 * decMatrix3x2 -> decMatrix3x2.
        /// </summary>
        public static decMatrix3x2 operator*(decMatrix2 lhs, decMatrix3x2 rhs) => new decMatrix3x2((lhs.m00 * rhs.m00 + lhs.m10 * rhs.m01), (lhs.m01 * rhs.m00 + lhs.m11 * rhs.m01), (lhs.m00 * rhs.m10 + lhs.m10 * rhs.m11), (lhs.m01 * rhs.m10 + lhs.m11 * rhs.m11), (lhs.m00 * rhs.m20 + lhs.m10 * rhs.m21), (lhs.m01 * rhs.m20 + lhs.m11 * rhs.m21));
        
        /// <summary>
        /// Executes a Matrix-Matrix-multiplication decMatrix2 * decMatrix4x2 -> decMatrix4x2.
        /// </summary>
        public static decMatrix4x2 operator*(decMatrix2 lhs, decMatrix4x2 rhs) => new decMatrix4x2((lhs.m00 * rhs.m00 + lhs.m10 * rhs.m01), (lhs.m01 * rhs.m00 + lhs.m11 * rhs.m01), (lhs.m00 * rhs.m10 + lhs.m10 * rhs.m11), (lhs.m01 * rhs.m10 + lhs.m11 * rhs.m11), (lhs.m00 * rhs.m20 + lhs.m10 * rhs.m21), (lhs.m01 * rhs.m20 + lhs.m11 * rhs.m21), (lhs.m00 * rhs.m30 + lhs.m10 * rhs.m31), (lhs.m01 * rhs.m30 + lhs.m11 * rhs.m31));
        
        /// <summary>
        /// Executes a Matrix-Vectortor-multiplication.
        /// </summary>
        public static decVector2 operator*(decMatrix2 m, decVector2 v) => new decVector2((m.m00 * v.x + m.m10 * v.y), (m.m01 * v.x + m.m11 * v.y));
        
        /// <summary>
        /// Executes a Matrix-Matrix-divison A / B == A * B^-1 (use with caution).
        /// </summary>
        public static decMatrix2 operator/(decMatrix2 A, decMatrix2 B) => A * B.Inverse;
        
        /// <summary>
        /// Executes a component-wise * (multiply).
        /// </summary>
        public static decMatrix2 CompMul(decMatrix2 A, decMatrix2 B) => new decMatrix2(A.m00 * B.m00, A.m01 * B.m01, A.m10 * B.m10, A.m11 * B.m11);
        
        /// <summary>
        /// Executes a component-wise / (divide).
        /// </summary>
        public static decMatrix2 CompDiv(decMatrix2 A, decMatrix2 B) => new decMatrix2(A.m00 / B.m00, A.m01 / B.m01, A.m10 / B.m10, A.m11 / B.m11);
        
        /// <summary>
        /// Executes a component-wise + (add).
        /// </summary>
        public static decMatrix2 CompAdd(decMatrix2 A, decMatrix2 B) => new decMatrix2(A.m00 + B.m00, A.m01 + B.m01, A.m10 + B.m10, A.m11 + B.m11);
        
        /// <summary>
        /// Executes a component-wise - (subtract).
        /// </summary>
        public static decMatrix2 CompSub(decMatrix2 A, decMatrix2 B) => new decMatrix2(A.m00 - B.m00, A.m01 - B.m01, A.m10 - B.m10, A.m11 - B.m11);
        
        /// <summary>
        /// Executes a component-wise + (add).
        /// </summary>
        public static decMatrix2 operator+(decMatrix2 lhs, decMatrix2 rhs) => new decMatrix2(lhs.m00 + rhs.m00, lhs.m01 + rhs.m01, lhs.m10 + rhs.m10, lhs.m11 + rhs.m11);
        
        /// <summary>
        /// Executes a component-wise + (add) with a scalar.
        /// </summary>
        public static decMatrix2 operator+(decMatrix2 lhs, decimal rhs) => new decMatrix2(lhs.m00 + rhs, lhs.m01 + rhs, lhs.m10 + rhs, lhs.m11 + rhs);
        
        /// <summary>
        /// Executes a component-wise + (add) with a scalar.
        /// </summary>
        public static decMatrix2 operator+(decimal lhs, decMatrix2 rhs) => new decMatrix2(lhs + rhs.m00, lhs + rhs.m01, lhs + rhs.m10, lhs + rhs.m11);
        
        /// <summary>
        /// Executes a component-wise - (subtract).
        /// </summary>
        public static decMatrix2 operator-(decMatrix2 lhs, decMatrix2 rhs) => new decMatrix2(lhs.m00 - rhs.m00, lhs.m01 - rhs.m01, lhs.m10 - rhs.m10, lhs.m11 - rhs.m11);
        
        /// <summary>
        /// Executes a component-wise - (subtract) with a scalar.
        /// </summary>
        public static decMatrix2 operator-(decMatrix2 lhs, decimal rhs) => new decMatrix2(lhs.m00 - rhs, lhs.m01 - rhs, lhs.m10 - rhs, lhs.m11 - rhs);
        
        /// <summary>
        /// Executes a component-wise - (subtract) with a scalar.
        /// </summary>
        public static decMatrix2 operator-(decimal lhs, decMatrix2 rhs) => new decMatrix2(lhs - rhs.m00, lhs - rhs.m01, lhs - rhs.m10, lhs - rhs.m11);
        
        /// <summary>
        /// Executes a component-wise / (divide) with a scalar.
        /// </summary>
        public static decMatrix2 operator/(decMatrix2 lhs, decimal rhs) => new decMatrix2(lhs.m00 / rhs, lhs.m01 / rhs, lhs.m10 / rhs, lhs.m11 / rhs);
        
        /// <summary>
        /// Executes a component-wise / (divide) with a scalar.
        /// </summary>
        public static decMatrix2 operator/(decimal lhs, decMatrix2 rhs) => new decMatrix2(lhs / rhs.m00, lhs / rhs.m01, lhs / rhs.m10, lhs / rhs.m11);
        
        /// <summary>
        /// Executes a component-wise * (multiply) with a scalar.
        /// </summary>
        public static decMatrix2 operator*(decMatrix2 lhs, decimal rhs) => new decMatrix2(lhs.m00 * rhs, lhs.m01 * rhs, lhs.m10 * rhs, lhs.m11 * rhs);
        
        /// <summary>
        /// Executes a component-wise * (multiply) with a scalar.
        /// </summary>
        public static decMatrix2 operator*(decimal lhs, decMatrix2 rhs) => new decMatrix2(lhs * rhs.m00, lhs * rhs.m01, lhs * rhs.m10, lhs * rhs.m11);
        
        /// <summary>
        /// Executes a component-wise lesser-than comparison.
        /// </summary>
        public static bMatrix2 operator<(decMatrix2 lhs, decMatrix2 rhs) => new bMatrix2(lhs.m00 < rhs.m00, lhs.m01 < rhs.m01, lhs.m10 < rhs.m10, lhs.m11 < rhs.m11);
        
        /// <summary>
        /// Executes a component-wise lesser-than comparison with a scalar.
        /// </summary>
        public static bMatrix2 operator<(decMatrix2 lhs, decimal rhs) => new bMatrix2(lhs.m00 < rhs, lhs.m01 < rhs, lhs.m10 < rhs, lhs.m11 < rhs);
        
        /// <summary>
        /// Executes a component-wise lesser-than comparison with a scalar.
        /// </summary>
        public static bMatrix2 operator<(decimal lhs, decMatrix2 rhs) => new bMatrix2(lhs < rhs.m00, lhs < rhs.m01, lhs < rhs.m10, lhs < rhs.m11);
        
        /// <summary>
        /// Executes a component-wise lesser-or-equal comparison.
        /// </summary>
        public static bMatrix2 operator<=(decMatrix2 lhs, decMatrix2 rhs) => new bMatrix2(lhs.m00 <= rhs.m00, lhs.m01 <= rhs.m01, lhs.m10 <= rhs.m10, lhs.m11 <= rhs.m11);
        
        /// <summary>
        /// Executes a component-wise lesser-or-equal comparison with a scalar.
        /// </summary>
        public static bMatrix2 operator<=(decMatrix2 lhs, decimal rhs) => new bMatrix2(lhs.m00 <= rhs, lhs.m01 <= rhs, lhs.m10 <= rhs, lhs.m11 <= rhs);
        
        /// <summary>
        /// Executes a component-wise lesser-or-equal comparison with a scalar.
        /// </summary>
        public static bMatrix2 operator<=(decimal lhs, decMatrix2 rhs) => new bMatrix2(lhs <= rhs.m00, lhs <= rhs.m01, lhs <= rhs.m10, lhs <= rhs.m11);
        
        /// <summary>
        /// Executes a component-wise greater-than comparison.
        /// </summary>
        public static bMatrix2 operator>(decMatrix2 lhs, decMatrix2 rhs) => new bMatrix2(lhs.m00 > rhs.m00, lhs.m01 > rhs.m01, lhs.m10 > rhs.m10, lhs.m11 > rhs.m11);
        
        /// <summary>
        /// Executes a component-wise greater-than comparison with a scalar.
        /// </summary>
        public static bMatrix2 operator>(decMatrix2 lhs, decimal rhs) => new bMatrix2(lhs.m00 > rhs, lhs.m01 > rhs, lhs.m10 > rhs, lhs.m11 > rhs);
        
        /// <summary>
        /// Executes a component-wise greater-than comparison with a scalar.
        /// </summary>
        public static bMatrix2 operator>(decimal lhs, decMatrix2 rhs) => new bMatrix2(lhs > rhs.m00, lhs > rhs.m01, lhs > rhs.m10, lhs > rhs.m11);
        
        /// <summary>
        /// Executes a component-wise greater-or-equal comparison.
        /// </summary>
        public static bMatrix2 operator>=(decMatrix2 lhs, decMatrix2 rhs) => new bMatrix2(lhs.m00 >= rhs.m00, lhs.m01 >= rhs.m01, lhs.m10 >= rhs.m10, lhs.m11 >= rhs.m11);
        
        /// <summary>
        /// Executes a component-wise greater-or-equal comparison with a scalar.
        /// </summary>
        public static bMatrix2 operator>=(decMatrix2 lhs, decimal rhs) => new bMatrix2(lhs.m00 >= rhs, lhs.m01 >= rhs, lhs.m10 >= rhs, lhs.m11 >= rhs);
        
        /// <summary>
        /// Executes a component-wise greater-or-equal comparison with a scalar.
        /// </summary>
        public static bMatrix2 operator>=(decimal lhs, decMatrix2 rhs) => new bMatrix2(lhs >= rhs.m00, lhs >= rhs.m01, lhs >= rhs.m10, lhs >= rhs.m11);
    }
}
