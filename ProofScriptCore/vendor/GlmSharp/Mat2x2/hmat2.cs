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
    /// A Matrix of type Half with 2 columns and 2 rows.
    /// </summary>
    [RegisterCoreClassStruct]
    
    [StructLayout(LayoutKind.Sequential)]
    public struct hMatrix2 : IReadOnlyList<Half>, IEquatable<hMatrix2>
    {

        #region Fields
        
        /// <summary>
        /// Column 0, Rows 0
        /// </summary>
        
        public Half m00;
        
        /// <summary>
        /// Column 0, Rows 1
        /// </summary>
        
        public Half m01;
        
        /// <summary>
        /// Column 1, Rows 0
        /// </summary>
        
        public Half m10;
        
        /// <summary>
        /// Column 1, Rows 1
        /// </summary>
        
        public Half m11;

        #endregion


        #region Constructors
        
        /// <summary>
        /// Component-wise constructor
        /// </summary>
        public hMatrix2(Half m00, Half m01, Half m10, Half m11)
        {
            this.m00 = m00;
            this.m01 = m01;
            this.m10 = m10;
            this.m11 = m11;
        }
        
        /// <summary>
        /// Constructs this Matrix from a hMatrix2. Non-overwritten fields are from an Identity Matrix.
        /// </summary>
        public hMatrix2(hMatrix2 m)
        {
            this.m00 = m.m00;
            this.m01 = m.m01;
            this.m10 = m.m10;
            this.m11 = m.m11;
        }
        
        /// <summary>
        /// Constructs this Matrix from a hMatrix3x2. Non-overwritten fields are from an Identity Matrix.
        /// </summary>
        public hMatrix2(hMatrix3x2 m)
        {
            this.m00 = m.m00;
            this.m01 = m.m01;
            this.m10 = m.m10;
            this.m11 = m.m11;
        }
        
        /// <summary>
        /// Constructs this Matrix from a hMatrix4x2. Non-overwritten fields are from an Identity Matrix.
        /// </summary>
        public hMatrix2(hMatrix4x2 m)
        {
            this.m00 = m.m00;
            this.m01 = m.m01;
            this.m10 = m.m10;
            this.m11 = m.m11;
        }
        
        /// <summary>
        /// Constructs this Matrix from a hMatrix2x3. Non-overwritten fields are from an Identity Matrix.
        /// </summary>
        public hMatrix2(hMatrix2x3 m)
        {
            this.m00 = m.m00;
            this.m01 = m.m01;
            this.m10 = m.m10;
            this.m11 = m.m11;
        }
        
        /// <summary>
        /// Constructs this Matrix from a hMatrix3. Non-overwritten fields are from an Identity Matrix.
        /// </summary>
        public hMatrix2(hMatrix3 m)
        {
            this.m00 = m.m00;
            this.m01 = m.m01;
            this.m10 = m.m10;
            this.m11 = m.m11;
        }
        
        /// <summary>
        /// Constructs this Matrix from a hMatrix4x3. Non-overwritten fields are from an Identity Matrix.
        /// </summary>
        public hMatrix2(hMatrix4x3 m)
        {
            this.m00 = m.m00;
            this.m01 = m.m01;
            this.m10 = m.m10;
            this.m11 = m.m11;
        }
        
        /// <summary>
        /// Constructs this Matrix from a hMatrix2x4. Non-overwritten fields are from an Identity Matrix.
        /// </summary>
        public hMatrix2(hMatrix2x4 m)
        {
            this.m00 = m.m00;
            this.m01 = m.m01;
            this.m10 = m.m10;
            this.m11 = m.m11;
        }
        
        /// <summary>
        /// Constructs this Matrix from a hMatrix3x4. Non-overwritten fields are from an Identity Matrix.
        /// </summary>
        public hMatrix2(hMatrix3x4 m)
        {
            this.m00 = m.m00;
            this.m01 = m.m01;
            this.m10 = m.m10;
            this.m11 = m.m11;
        }
        
        /// <summary>
        /// Constructs this Matrix from a hMatrix4. Non-overwritten fields are from an Identity Matrix.
        /// </summary>
        public hMatrix2(hMatrix4 m)
        {
            this.m00 = m.m00;
            this.m01 = m.m01;
            this.m10 = m.m10;
            this.m11 = m.m11;
        }
        
        /// <summary>
        /// Constructs this Matrix from a series of column Vectors. Non-overwritten fields are from an Identity Matrix.
        /// </summary>
        public hMatrix2(hVector2 c0, hVector2 c1)
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
        public Half[,] Values => new[,] { { m00, m01 }, { m10, m11 } };
        
        /// <summary>
        /// Creates a 1D array with all values (internal order)
        /// </summary>
        public Half[] Values1D => new[] { m00, m01, m10, m11 };
        
        /// <summary>
        /// Gets or sets the column nr 0
        /// </summary>
        public hVector2 Column0
        {
            get
            {
                return new hVector2(m00, m01);
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
        public hVector2 Column1
        {
            get
            {
                return new hVector2(m10, m11);
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
        public hVector2 Row0
        {
            get
            {
                return new hVector2(m00, m10);
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
        public hVector2 Row1
        {
            get
            {
                return new hVector2(m01, m11);
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
        public static hMatrix2 Zero { get; } = new hMatrix2(Half.Zero, Half.Zero, Half.Zero, Half.Zero);
        
        /// <summary>
        /// Predefined all-ones Matrix
        /// </summary>
        public static hMatrix2 Ones { get; } = new hMatrix2(Half.One, Half.One, Half.One, Half.One);
        
        /// <summary>
        /// Predefined identity Matrix
        /// </summary>
        public static hMatrix2 Identity { get; } = new hMatrix2(Half.One, Half.Zero, Half.Zero, Half.One);
        
        /// <summary>
        /// Predefined all-MaxValue Matrix
        /// </summary>
        public static hMatrix2 AllMaxValue { get; } = new hMatrix2(Half.MaxValue, Half.MaxValue, Half.MaxValue, Half.MaxValue);
        
        /// <summary>
        /// Predefined diagonal-MaxValue Matrix
        /// </summary>
        public static hMatrix2 DiagonalMaxValue { get; } = new hMatrix2(Half.MaxValue, Half.Zero, Half.Zero, Half.MaxValue);
        
        /// <summary>
        /// Predefined all-MinValue Matrix
        /// </summary>
        public static hMatrix2 AllMinValue { get; } = new hMatrix2(Half.MinValue, Half.MinValue, Half.MinValue, Half.MinValue);
        
        /// <summary>
        /// Predefined diagonal-MinValue Matrix
        /// </summary>
        public static hMatrix2 DiagonalMinValue { get; } = new hMatrix2(Half.MinValue, Half.Zero, Half.Zero, Half.MinValue);
        
        /// <summary>
        /// Predefined all-Epsilon Matrix
        /// </summary>
        public static hMatrix2 AllEpsilon { get; } = new hMatrix2(Half.Epsilon, Half.Epsilon, Half.Epsilon, Half.Epsilon);
        
        /// <summary>
        /// Predefined diagonal-Epsilon Matrix
        /// </summary>
        public static hMatrix2 DiagonalEpsilon { get; } = new hMatrix2(Half.Epsilon, Half.Zero, Half.Zero, Half.Epsilon);
        
        /// <summary>
        /// Predefined all-NaN Matrix
        /// </summary>
        public static hMatrix2 AllNaN { get; } = new hMatrix2(Half.NaN, Half.NaN, Half.NaN, Half.NaN);
        
        /// <summary>
        /// Predefined diagonal-NaN Matrix
        /// </summary>
        public static hMatrix2 DiagonalNaN { get; } = new hMatrix2(Half.NaN, Half.Zero, Half.Zero, Half.NaN);
        
        /// <summary>
        /// Predefined all-NegativeInfinity Matrix
        /// </summary>
        public static hMatrix2 AllNegativeInfinity { get; } = new hMatrix2(Half.NegativeInfinity, Half.NegativeInfinity, Half.NegativeInfinity, Half.NegativeInfinity);
        
        /// <summary>
        /// Predefined diagonal-NegativeInfinity Matrix
        /// </summary>
        public static hMatrix2 DiagonalNegativeInfinity { get; } = new hMatrix2(Half.NegativeInfinity, Half.Zero, Half.Zero, Half.NegativeInfinity);
        
        /// <summary>
        /// Predefined all-PositiveInfinity Matrix
        /// </summary>
        public static hMatrix2 AllPositiveInfinity { get; } = new hMatrix2(Half.PositiveInfinity, Half.PositiveInfinity, Half.PositiveInfinity, Half.PositiveInfinity);
        
        /// <summary>
        /// Predefined diagonal-PositiveInfinity Matrix
        /// </summary>
        public static hMatrix2 DiagonalPositiveInfinity { get; } = new hMatrix2(Half.PositiveInfinity, Half.Zero, Half.Zero, Half.PositiveInfinity);

        #endregion


        #region Functions
        
        /// <summary>
        /// Returns an enumerator that iterates through all fields.
        /// </summary>
        public IEnumerator<Half> GetEnumerator()
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
        public Half this[int fieldIndex]
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
        public Half this[int col, int row]
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
        public bool Equals(hMatrix2 rhs) => ((m00.Equals(rhs.m00) && m01.Equals(rhs.m01)) && (m10.Equals(rhs.m10) && m11.Equals(rhs.m11)));
        
        /// <summary>
        /// Returns true iff this equals rhs type- and component-wise.
        /// </summary>
        public override bool Equals(object obj)
        {
            if (ReferenceEquals(null, obj)) return false;
            return obj is hMatrix2 && Equals((hMatrix2) obj);
        }
        
        /// <summary>
        /// Returns true iff this equals rhs component-wise.
        /// </summary>
        public static bool operator ==(hMatrix2 lhs, hMatrix2 rhs) => lhs.Equals(rhs);
        
        /// <summary>
        /// Returns true iff this does not equal rhs (component-wise).
        /// </summary>
        public static bool operator !=(hMatrix2 lhs, hMatrix2 rhs) => !lhs.Equals(rhs);
        
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
        public hMatrix2 Transposed => new hMatrix2(m00, m10, m01, m11);
        
        /// <summary>
        /// Returns the minimal component of this Matrix.
        /// </summary>
        public Half MinElement => Half.Min(Half.Min(Half.Min(m00, m01), m10), m11);
        
        /// <summary>
        /// Returns the maximal component of this Matrix.
        /// </summary>
        public Half MaxElement => Half.Max(Half.Max(Half.Max(m00, m01), m10), m11);
        
        /// <summary>
        /// Returns the euclidean length of this Matrix.
        /// </summary>
        public float Length => (float)Math.Sqrt(((m00*m00 + m01*m01) + (m10*m10 + m11*m11)));
        
        /// <summary>
        /// Returns the squared euclidean length of this Matrix.
        /// </summary>
        public float LengthSqr => ((m00*m00 + m01*m01) + (m10*m10 + m11*m11));
        
        /// <summary>
        /// Returns the sum of all fields.
        /// </summary>
        public Half Sum => ((m00 + m01) + (m10 + m11));
        
        /// <summary>
        /// Returns the euclidean norm of this Matrix.
        /// </summary>
        public float Norm => (float)Math.Sqrt(((m00*m00 + m01*m01) + (m10*m10 + m11*m11)));
        
        /// <summary>
        /// Returns the one-norm of this Matrix.
        /// </summary>
        public float Norm1 => ((Half.Abs(m00) + Half.Abs(m01)) + (Half.Abs(m10) + Half.Abs(m11)));
        
        /// <summary>
        /// Returns the two-norm of this Matrix.
        /// </summary>
        public float Norm2 => (float)Math.Sqrt(((m00*m00 + m01*m01) + (m10*m10 + m11*m11)));
        
        /// <summary>
        /// Returns the max-norm of this Matrix.
        /// </summary>
        public Half NormMax => Half.Max(Half.Max(Half.Max(Half.Abs(m00), Half.Abs(m01)), Half.Abs(m10)), Half.Abs(m11));
        
        /// <summary>
        /// Returns the p-norm of this Matrix.
        /// </summary>
        public double NormP(double p) => Math.Pow(((Math.Pow((double)Half.Abs(m00), p) + Math.Pow((double)Half.Abs(m01), p)) + (Math.Pow((double)Half.Abs(m10), p) + Math.Pow((double)Half.Abs(m11), p))), 1 / p);
        
        /// <summary>
        /// Returns determinant of this Matrix.
        /// </summary>
        public Half Determinant => m00 * m11 - m10 * m01;
        
        /// <summary>
        /// Returns the adjunct of this Matrix.
        /// </summary>
        public hMatrix2 Adjugate => new hMatrix2(m11, -m01, -m10, m00);
        
        /// <summary>
        /// Returns the inverse of this Matrix (use with caution).
        /// </summary>
        public hMatrix2 Inverse => Adjugate / Determinant;
        
        /// <summary>
        /// Executes a Matrix-Matrix-multiplication hMatrix2 * hMatrix2 -> hMatrix2.
        /// </summary>
        public static hMatrix2 operator*(hMatrix2 lhs, hMatrix2 rhs) => new hMatrix2((lhs.m00 * rhs.m00 + lhs.m10 * rhs.m01), (lhs.m01 * rhs.m00 + lhs.m11 * rhs.m01), (lhs.m00 * rhs.m10 + lhs.m10 * rhs.m11), (lhs.m01 * rhs.m10 + lhs.m11 * rhs.m11));
        
        /// <summary>
        /// Executes a Matrix-Matrix-multiplication hMatrix2 * hMatrix3x2 -> hMatrix3x2.
        /// </summary>
        public static hMatrix3x2 operator*(hMatrix2 lhs, hMatrix3x2 rhs) => new hMatrix3x2((lhs.m00 * rhs.m00 + lhs.m10 * rhs.m01), (lhs.m01 * rhs.m00 + lhs.m11 * rhs.m01), (lhs.m00 * rhs.m10 + lhs.m10 * rhs.m11), (lhs.m01 * rhs.m10 + lhs.m11 * rhs.m11), (lhs.m00 * rhs.m20 + lhs.m10 * rhs.m21), (lhs.m01 * rhs.m20 + lhs.m11 * rhs.m21));
        
        /// <summary>
        /// Executes a Matrix-Matrix-multiplication hMatrix2 * hMatrix4x2 -> hMatrix4x2.
        /// </summary>
        public static hMatrix4x2 operator*(hMatrix2 lhs, hMatrix4x2 rhs) => new hMatrix4x2((lhs.m00 * rhs.m00 + lhs.m10 * rhs.m01), (lhs.m01 * rhs.m00 + lhs.m11 * rhs.m01), (lhs.m00 * rhs.m10 + lhs.m10 * rhs.m11), (lhs.m01 * rhs.m10 + lhs.m11 * rhs.m11), (lhs.m00 * rhs.m20 + lhs.m10 * rhs.m21), (lhs.m01 * rhs.m20 + lhs.m11 * rhs.m21), (lhs.m00 * rhs.m30 + lhs.m10 * rhs.m31), (lhs.m01 * rhs.m30 + lhs.m11 * rhs.m31));
        
        /// <summary>
        /// Executes a Matrix-Vector-multiplication.
        /// </summary>
        public static hVector2 operator*(hMatrix2 m, hVector2 v) => new hVector2((m.m00 * v.x + m.m10 * v.y), (m.m01 * v.x + m.m11 * v.y));
        
        /// <summary>
        /// Executes a Matrix-Matrix-divison A / B == A * B^-1 (use with caution).
        /// </summary>
        public static hMatrix2 operator/(hMatrix2 A, hMatrix2 B) => A * B.Inverse;
        
        /// <summary>
        /// Executes a component-wise * (multiply).
        /// </summary>
        public static hMatrix2 CompMul(hMatrix2 A, hMatrix2 B) => new hMatrix2(A.m00 * B.m00, A.m01 * B.m01, A.m10 * B.m10, A.m11 * B.m11);
        
        /// <summary>
        /// Executes a component-wise / (divide).
        /// </summary>
        public static hMatrix2 CompDiv(hMatrix2 A, hMatrix2 B) => new hMatrix2(A.m00 / B.m00, A.m01 / B.m01, A.m10 / B.m10, A.m11 / B.m11);
        
        /// <summary>
        /// Executes a component-wise + (add).
        /// </summary>
        public static hMatrix2 CompAdd(hMatrix2 A, hMatrix2 B) => new hMatrix2(A.m00 + B.m00, A.m01 + B.m01, A.m10 + B.m10, A.m11 + B.m11);
        
        /// <summary>
        /// Executes a component-wise - (subtract).
        /// </summary>
        public static hMatrix2 CompSub(hMatrix2 A, hMatrix2 B) => new hMatrix2(A.m00 - B.m00, A.m01 - B.m01, A.m10 - B.m10, A.m11 - B.m11);
        
        /// <summary>
        /// Executes a component-wise + (add).
        /// </summary>
        public static hMatrix2 operator+(hMatrix2 lhs, hMatrix2 rhs) => new hMatrix2(lhs.m00 + rhs.m00, lhs.m01 + rhs.m01, lhs.m10 + rhs.m10, lhs.m11 + rhs.m11);
        
        /// <summary>
        /// Executes a component-wise + (add) with a scalar.
        /// </summary>
        public static hMatrix2 operator+(hMatrix2 lhs, Half rhs) => new hMatrix2(lhs.m00 + rhs, lhs.m01 + rhs, lhs.m10 + rhs, lhs.m11 + rhs);
        
        /// <summary>
        /// Executes a component-wise + (add) with a scalar.
        /// </summary>
        public static hMatrix2 operator+(Half lhs, hMatrix2 rhs) => new hMatrix2(lhs + rhs.m00, lhs + rhs.m01, lhs + rhs.m10, lhs + rhs.m11);
        
        /// <summary>
        /// Executes a component-wise - (subtract).
        /// </summary>
        public static hMatrix2 operator-(hMatrix2 lhs, hMatrix2 rhs) => new hMatrix2(lhs.m00 - rhs.m00, lhs.m01 - rhs.m01, lhs.m10 - rhs.m10, lhs.m11 - rhs.m11);
        
        /// <summary>
        /// Executes a component-wise - (subtract) with a scalar.
        /// </summary>
        public static hMatrix2 operator-(hMatrix2 lhs, Half rhs) => new hMatrix2(lhs.m00 - rhs, lhs.m01 - rhs, lhs.m10 - rhs, lhs.m11 - rhs);
        
        /// <summary>
        /// Executes a component-wise - (subtract) with a scalar.
        /// </summary>
        public static hMatrix2 operator-(Half lhs, hMatrix2 rhs) => new hMatrix2(lhs - rhs.m00, lhs - rhs.m01, lhs - rhs.m10, lhs - rhs.m11);
        
        /// <summary>
        /// Executes a component-wise / (divide) with a scalar.
        /// </summary>
        public static hMatrix2 operator/(hMatrix2 lhs, Half rhs) => new hMatrix2(lhs.m00 / rhs, lhs.m01 / rhs, lhs.m10 / rhs, lhs.m11 / rhs);
        
        /// <summary>
        /// Executes a component-wise / (divide) with a scalar.
        /// </summary>
        public static hMatrix2 operator/(Half lhs, hMatrix2 rhs) => new hMatrix2(lhs / rhs.m00, lhs / rhs.m01, lhs / rhs.m10, lhs / rhs.m11);
        
        /// <summary>
        /// Executes a component-wise * (multiply) with a scalar.
        /// </summary>
        public static hMatrix2 operator*(hMatrix2 lhs, Half rhs) => new hMatrix2(lhs.m00 * rhs, lhs.m01 * rhs, lhs.m10 * rhs, lhs.m11 * rhs);
        
        /// <summary>
        /// Executes a component-wise * (multiply) with a scalar.
        /// </summary>
        public static hMatrix2 operator*(Half lhs, hMatrix2 rhs) => new hMatrix2(lhs * rhs.m00, lhs * rhs.m01, lhs * rhs.m10, lhs * rhs.m11);
        
        /// <summary>
        /// Executes a component-wise lesser-than comparison.
        /// </summary>
        public static bMatrix2 operator<(hMatrix2 lhs, hMatrix2 rhs) => new bMatrix2(lhs.m00 < rhs.m00, lhs.m01 < rhs.m01, lhs.m10 < rhs.m10, lhs.m11 < rhs.m11);
        
        /// <summary>
        /// Executes a component-wise lesser-than comparison with a scalar.
        /// </summary>
        public static bMatrix2 operator<(hMatrix2 lhs, Half rhs) => new bMatrix2(lhs.m00 < rhs, lhs.m01 < rhs, lhs.m10 < rhs, lhs.m11 < rhs);
        
        /// <summary>
        /// Executes a component-wise lesser-than comparison with a scalar.
        /// </summary>
        public static bMatrix2 operator<(Half lhs, hMatrix2 rhs) => new bMatrix2(lhs < rhs.m00, lhs < rhs.m01, lhs < rhs.m10, lhs < rhs.m11);
        
        /// <summary>
        /// Executes a component-wise lesser-or-equal comparison.
        /// </summary>
        public static bMatrix2 operator<=(hMatrix2 lhs, hMatrix2 rhs) => new bMatrix2(lhs.m00 <= rhs.m00, lhs.m01 <= rhs.m01, lhs.m10 <= rhs.m10, lhs.m11 <= rhs.m11);
        
        /// <summary>
        /// Executes a component-wise lesser-or-equal comparison with a scalar.
        /// </summary>
        public static bMatrix2 operator<=(hMatrix2 lhs, Half rhs) => new bMatrix2(lhs.m00 <= rhs, lhs.m01 <= rhs, lhs.m10 <= rhs, lhs.m11 <= rhs);
        
        /// <summary>
        /// Executes a component-wise lesser-or-equal comparison with a scalar.
        /// </summary>
        public static bMatrix2 operator<=(Half lhs, hMatrix2 rhs) => new bMatrix2(lhs <= rhs.m00, lhs <= rhs.m01, lhs <= rhs.m10, lhs <= rhs.m11);
        
        /// <summary>
        /// Executes a component-wise greater-than comparison.
        /// </summary>
        public static bMatrix2 operator>(hMatrix2 lhs, hMatrix2 rhs) => new bMatrix2(lhs.m00 > rhs.m00, lhs.m01 > rhs.m01, lhs.m10 > rhs.m10, lhs.m11 > rhs.m11);
        
        /// <summary>
        /// Executes a component-wise greater-than comparison with a scalar.
        /// </summary>
        public static bMatrix2 operator>(hMatrix2 lhs, Half rhs) => new bMatrix2(lhs.m00 > rhs, lhs.m01 > rhs, lhs.m10 > rhs, lhs.m11 > rhs);
        
        /// <summary>
        /// Executes a component-wise greater-than comparison with a scalar.
        /// </summary>
        public static bMatrix2 operator>(Half lhs, hMatrix2 rhs) => new bMatrix2(lhs > rhs.m00, lhs > rhs.m01, lhs > rhs.m10, lhs > rhs.m11);
        
        /// <summary>
        /// Executes a component-wise greater-or-equal comparison.
        /// </summary>
        public static bMatrix2 operator>=(hMatrix2 lhs, hMatrix2 rhs) => new bMatrix2(lhs.m00 >= rhs.m00, lhs.m01 >= rhs.m01, lhs.m10 >= rhs.m10, lhs.m11 >= rhs.m11);
        
        /// <summary>
        /// Executes a component-wise greater-or-equal comparison with a scalar.
        /// </summary>
        public static bMatrix2 operator>=(hMatrix2 lhs, Half rhs) => new bMatrix2(lhs.m00 >= rhs, lhs.m01 >= rhs, lhs.m10 >= rhs, lhs.m11 >= rhs);
        
        /// <summary>
        /// Executes a component-wise greater-or-equal comparison with a scalar.
        /// </summary>
        public static bMatrix2 operator>=(Half lhs, hMatrix2 rhs) => new bMatrix2(lhs >= rhs.m00, lhs >= rhs.m01, lhs >= rhs.m10, lhs >= rhs.m11);
    }
}
