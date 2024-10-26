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
    /// A Matrix of type Half with 4 columns and 2 rows.
    /// </summary>
    
    
    [StructLayout(LayoutKind.Sequential)]
    public struct hMatrix4x2 : IReadOnlyList<Half>, IEquatable<hMatrix4x2>
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
        
        /// <summary>
        /// Column 2, Rows 0
        /// </summary>
        
        public Half m20;
        
        /// <summary>
        /// Column 2, Rows 1
        /// </summary>
        
        public Half m21;
        
        /// <summary>
        /// Column 3, Rows 0
        /// </summary>
        
        public Half m30;
        
        /// <summary>
        /// Column 3, Rows 1
        /// </summary>
        
        public Half m31;

        #endregion


        #region Constructors
        
        /// <summary>
        /// Component-wise constructor
        /// </summary>
        public hMatrix4x2(Half m00, Half m01, Half m10, Half m11, Half m20, Half m21, Half m30, Half m31)
        {
            this.m00 = m00;
            this.m01 = m01;
            this.m10 = m10;
            this.m11 = m11;
            this.m20 = m20;
            this.m21 = m21;
            this.m30 = m30;
            this.m31 = m31;
        }
        
        /// <summary>
        /// Constructs this Matrix from a hMatrix2. Non-overwritten fields are from an Identity Matrix.
        /// </summary>
        public hMatrix4x2(hMatrix2 m)
        {
            this.m00 = m.m00;
            this.m01 = m.m01;
            this.m10 = m.m10;
            this.m11 = m.m11;
            this.m20 = Half.Zero;
            this.m21 = Half.Zero;
            this.m30 = Half.Zero;
            this.m31 = Half.Zero;
        }
        
        /// <summary>
        /// Constructs this Matrix from a hMatrix3x2. Non-overwritten fields are from an Identity Matrix.
        /// </summary>
        public hMatrix4x2(hMatrix3x2 m)
        {
            this.m00 = m.m00;
            this.m01 = m.m01;
            this.m10 = m.m10;
            this.m11 = m.m11;
            this.m20 = m.m20;
            this.m21 = m.m21;
            this.m30 = Half.Zero;
            this.m31 = Half.Zero;
        }
        
        /// <summary>
        /// Constructs this Matrix from a hMatrix4x2. Non-overwritten fields are from an Identity Matrix.
        /// </summary>
        public hMatrix4x2(hMatrix4x2 m)
        {
            this.m00 = m.m00;
            this.m01 = m.m01;
            this.m10 = m.m10;
            this.m11 = m.m11;
            this.m20 = m.m20;
            this.m21 = m.m21;
            this.m30 = m.m30;
            this.m31 = m.m31;
        }
        
        /// <summary>
        /// Constructs this Matrix from a hMatrix2x3. Non-overwritten fields are from an Identity Matrix.
        /// </summary>
        public hMatrix4x2(hMatrix2x3 m)
        {
            this.m00 = m.m00;
            this.m01 = m.m01;
            this.m10 = m.m10;
            this.m11 = m.m11;
            this.m20 = Half.Zero;
            this.m21 = Half.Zero;
            this.m30 = Half.Zero;
            this.m31 = Half.Zero;
        }
        
        /// <summary>
        /// Constructs this Matrix from a hMatrix3. Non-overwritten fields are from an Identity Matrix.
        /// </summary>
        public hMatrix4x2(hMatrix3 m)
        {
            this.m00 = m.m00;
            this.m01 = m.m01;
            this.m10 = m.m10;
            this.m11 = m.m11;
            this.m20 = m.m20;
            this.m21 = m.m21;
            this.m30 = Half.Zero;
            this.m31 = Half.Zero;
        }
        
        /// <summary>
        /// Constructs this Matrix from a hMatrix4x3. Non-overwritten fields are from an Identity Matrix.
        /// </summary>
        public hMatrix4x2(hMatrix4x3 m)
        {
            this.m00 = m.m00;
            this.m01 = m.m01;
            this.m10 = m.m10;
            this.m11 = m.m11;
            this.m20 = m.m20;
            this.m21 = m.m21;
            this.m30 = m.m30;
            this.m31 = m.m31;
        }
        
        /// <summary>
        /// Constructs this Matrix from a hMatrix2x4. Non-overwritten fields are from an Identity Matrix.
        /// </summary>
        public hMatrix4x2(hMatrix2x4 m)
        {
            this.m00 = m.m00;
            this.m01 = m.m01;
            this.m10 = m.m10;
            this.m11 = m.m11;
            this.m20 = Half.Zero;
            this.m21 = Half.Zero;
            this.m30 = Half.Zero;
            this.m31 = Half.Zero;
        }
        
        /// <summary>
        /// Constructs this Matrix from a hMatrix3x4. Non-overwritten fields are from an Identity Matrix.
        /// </summary>
        public hMatrix4x2(hMatrix3x4 m)
        {
            this.m00 = m.m00;
            this.m01 = m.m01;
            this.m10 = m.m10;
            this.m11 = m.m11;
            this.m20 = m.m20;
            this.m21 = m.m21;
            this.m30 = Half.Zero;
            this.m31 = Half.Zero;
        }
        
        /// <summary>
        /// Constructs this Matrix from a hMatrix4. Non-overwritten fields are from an Identity Matrix.
        /// </summary>
        public hMatrix4x2(hMatrix4 m)
        {
            this.m00 = m.m00;
            this.m01 = m.m01;
            this.m10 = m.m10;
            this.m11 = m.m11;
            this.m20 = m.m20;
            this.m21 = m.m21;
            this.m30 = m.m30;
            this.m31 = m.m31;
        }
        
        /// <summary>
        /// Constructs this Matrix from a series of column Vectors. Non-overwritten fields are from an Identity Matrix.
        /// </summary>
        public hMatrix4x2(hVector2 c0, hVector2 c1)
        {
            this.m00 = c0.x;
            this.m01 = c0.y;
            this.m10 = c1.x;
            this.m11 = c1.y;
            this.m20 = Half.Zero;
            this.m21 = Half.Zero;
            this.m30 = Half.Zero;
            this.m31 = Half.Zero;
        }
        
        /// <summary>
        /// Constructs this Matrix from a series of column Vectors. Non-overwritten fields are from an Identity Matrix.
        /// </summary>
        public hMatrix4x2(hVector2 c0, hVector2 c1, hVector2 c2)
        {
            this.m00 = c0.x;
            this.m01 = c0.y;
            this.m10 = c1.x;
            this.m11 = c1.y;
            this.m20 = c2.x;
            this.m21 = c2.y;
            this.m30 = Half.Zero;
            this.m31 = Half.Zero;
        }
        
        /// <summary>
        /// Constructs this Matrix from a series of column Vectors. Non-overwritten fields are from an Identity Matrix.
        /// </summary>
        public hMatrix4x2(hVector2 c0, hVector2 c1, hVector2 c2, hVector2 c3)
        {
            this.m00 = c0.x;
            this.m01 = c0.y;
            this.m10 = c1.x;
            this.m11 = c1.y;
            this.m20 = c2.x;
            this.m21 = c2.y;
            this.m30 = c3.x;
            this.m31 = c3.y;
        }

        #endregion


        #region Properties
        
        /// <summary>
        /// Creates a 2D array with all values (address: Values[x, y])
        /// </summary>
        public Half[,] Values => new[,] { { m00, m01 }, { m10, m11 }, { m20, m21 }, { m30, m31 } };
        
        /// <summary>
        /// Creates a 1D array with all values (internal order)
        /// </summary>
        public Half[] Values1D => new[] { m00, m01, m10, m11, m20, m21, m30, m31 };
        
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
        /// Gets or sets the column nr 2
        /// </summary>
        public hVector2 Column2
        {
            get
            {
                return new hVector2(m20, m21);
            }
            set
            {
                m20 = value.x;
                m21 = value.y;
            }
        }
        
        /// <summary>
        /// Gets or sets the column nr 3
        /// </summary>
        public hVector2 Column3
        {
            get
            {
                return new hVector2(m30, m31);
            }
            set
            {
                m30 = value.x;
                m31 = value.y;
            }
        }
        
        /// <summary>
        /// Gets or sets the row nr 0
        /// </summary>
        public hVector4 Row0
        {
            get
            {
                return new hVector4(m00, m10, m20, m30);
            }
            set
            {
                m00 = value.x;
                m10 = value.y;
                m20 = value.z;
                m30 = value.w;
            }
        }
        
        /// <summary>
        /// Gets or sets the row nr 1
        /// </summary>
        public hVector4 Row1
        {
            get
            {
                return new hVector4(m01, m11, m21, m31);
            }
            set
            {
                m01 = value.x;
                m11 = value.y;
                m21 = value.z;
                m31 = value.w;
            }
        }

        #endregion


        #region Static Properties
        
        /// <summary>
        /// Predefined all-zero Matrix
        /// </summary>
        public static hMatrix4x2 Zero { get; } = new hMatrix4x2(Half.Zero, Half.Zero, Half.Zero, Half.Zero, Half.Zero, Half.Zero, Half.Zero, Half.Zero);
        
        /// <summary>
        /// Predefined all-ones Matrix
        /// </summary>
        public static hMatrix4x2 Ones { get; } = new hMatrix4x2(Half.One, Half.One, Half.One, Half.One, Half.One, Half.One, Half.One, Half.One);
        
        /// <summary>
        /// Predefined identity Matrix
        /// </summary>
        public static hMatrix4x2 Identity { get; } = new hMatrix4x2(Half.One, Half.Zero, Half.Zero, Half.One, Half.Zero, Half.Zero, Half.Zero, Half.Zero);
        
        /// <summary>
        /// Predefined all-MaxValue Matrix
        /// </summary>
        public static hMatrix4x2 AllMaxValue { get; } = new hMatrix4x2(Half.MaxValue, Half.MaxValue, Half.MaxValue, Half.MaxValue, Half.MaxValue, Half.MaxValue, Half.MaxValue, Half.MaxValue);
        
        /// <summary>
        /// Predefined diagonal-MaxValue Matrix
        /// </summary>
        public static hMatrix4x2 DiagonalMaxValue { get; } = new hMatrix4x2(Half.MaxValue, Half.Zero, Half.Zero, Half.MaxValue, Half.Zero, Half.Zero, Half.Zero, Half.Zero);
        
        /// <summary>
        /// Predefined all-MinValue Matrix
        /// </summary>
        public static hMatrix4x2 AllMinValue { get; } = new hMatrix4x2(Half.MinValue, Half.MinValue, Half.MinValue, Half.MinValue, Half.MinValue, Half.MinValue, Half.MinValue, Half.MinValue);
        
        /// <summary>
        /// Predefined diagonal-MinValue Matrix
        /// </summary>
        public static hMatrix4x2 DiagonalMinValue { get; } = new hMatrix4x2(Half.MinValue, Half.Zero, Half.Zero, Half.MinValue, Half.Zero, Half.Zero, Half.Zero, Half.Zero);
        
        /// <summary>
        /// Predefined all-Epsilon Matrix
        /// </summary>
        public static hMatrix4x2 AllEpsilon { get; } = new hMatrix4x2(Half.Epsilon, Half.Epsilon, Half.Epsilon, Half.Epsilon, Half.Epsilon, Half.Epsilon, Half.Epsilon, Half.Epsilon);
        
        /// <summary>
        /// Predefined diagonal-Epsilon Matrix
        /// </summary>
        public static hMatrix4x2 DiagonalEpsilon { get; } = new hMatrix4x2(Half.Epsilon, Half.Zero, Half.Zero, Half.Epsilon, Half.Zero, Half.Zero, Half.Zero, Half.Zero);
        
        /// <summary>
        /// Predefined all-NaN Matrix
        /// </summary>
        public static hMatrix4x2 AllNaN { get; } = new hMatrix4x2(Half.NaN, Half.NaN, Half.NaN, Half.NaN, Half.NaN, Half.NaN, Half.NaN, Half.NaN);
        
        /// <summary>
        /// Predefined diagonal-NaN Matrix
        /// </summary>
        public static hMatrix4x2 DiagonalNaN { get; } = new hMatrix4x2(Half.NaN, Half.Zero, Half.Zero, Half.NaN, Half.Zero, Half.Zero, Half.Zero, Half.Zero);
        
        /// <summary>
        /// Predefined all-NegativeInfinity Matrix
        /// </summary>
        public static hMatrix4x2 AllNegativeInfinity { get; } = new hMatrix4x2(Half.NegativeInfinity, Half.NegativeInfinity, Half.NegativeInfinity, Half.NegativeInfinity, Half.NegativeInfinity, Half.NegativeInfinity, Half.NegativeInfinity, Half.NegativeInfinity);
        
        /// <summary>
        /// Predefined diagonal-NegativeInfinity Matrix
        /// </summary>
        public static hMatrix4x2 DiagonalNegativeInfinity { get; } = new hMatrix4x2(Half.NegativeInfinity, Half.Zero, Half.Zero, Half.NegativeInfinity, Half.Zero, Half.Zero, Half.Zero, Half.Zero);
        
        /// <summary>
        /// Predefined all-PositiveInfinity Matrix
        /// </summary>
        public static hMatrix4x2 AllPositiveInfinity { get; } = new hMatrix4x2(Half.PositiveInfinity, Half.PositiveInfinity, Half.PositiveInfinity, Half.PositiveInfinity, Half.PositiveInfinity, Half.PositiveInfinity, Half.PositiveInfinity, Half.PositiveInfinity);
        
        /// <summary>
        /// Predefined diagonal-PositiveInfinity Matrix
        /// </summary>
        public static hMatrix4x2 DiagonalPositiveInfinity { get; } = new hMatrix4x2(Half.PositiveInfinity, Half.Zero, Half.Zero, Half.PositiveInfinity, Half.Zero, Half.Zero, Half.Zero, Half.Zero);

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
            yield return m20;
            yield return m21;
            yield return m30;
            yield return m31;
        }
        
        /// <summary>
        /// Returns an enumerator that iterates through all fields.
        /// </summary>
        IEnumerator IEnumerable.GetEnumerator() => GetEnumerator();

        #endregion

        
        /// <summary>
        /// Returns the number of Fields (4 x 2 = 8).
        /// </summary>
        public int Count => 8;
        
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
                    case 4: return m20;
                    case 5: return m21;
                    case 6: return m30;
                    case 7: return m31;
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
                    case 4: this.m20 = value; break;
                    case 5: this.m21 = value; break;
                    case 6: this.m30 = value; break;
                    case 7: this.m31 = value; break;
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
        public bool Equals(hMatrix4x2 rhs) => (((m00.Equals(rhs.m00) && m01.Equals(rhs.m01)) && (m10.Equals(rhs.m10) && m11.Equals(rhs.m11))) && ((m20.Equals(rhs.m20) && m21.Equals(rhs.m21)) && (m30.Equals(rhs.m30) && m31.Equals(rhs.m31))));
        
        /// <summary>
        /// Returns true iff this equals rhs type- and component-wise.
        /// </summary>
        public override bool Equals(object obj)
        {
            if (ReferenceEquals(null, obj)) return false;
            return obj is hMatrix4x2 && Equals((hMatrix4x2) obj);
        }
        
        /// <summary>
        /// Returns true iff this equals rhs component-wise.
        /// </summary>
        public static bool operator ==(hMatrix4x2 lhs, hMatrix4x2 rhs) => lhs.Equals(rhs);
        
        /// <summary>
        /// Returns true iff this does not equal rhs (component-wise).
        /// </summary>
        public static bool operator !=(hMatrix4x2 lhs, hMatrix4x2 rhs) => !lhs.Equals(rhs);
        
        /// <summary>
        /// Returns a hash code for this instance.
        /// </summary>
        public override int GetHashCode()
        {
            unchecked
            {
                return ((((((((((((((m00.GetHashCode()) * 397) ^ m01.GetHashCode()) * 397) ^ m10.GetHashCode()) * 397) ^ m11.GetHashCode()) * 397) ^ m20.GetHashCode()) * 397) ^ m21.GetHashCode()) * 397) ^ m30.GetHashCode()) * 397) ^ m31.GetHashCode();
            }
        }
        
        /// <summary>
        /// Returns a transposed version of this Matrix.
        /// </summary>
        public hMatrix2x4 Transposed => new hMatrix2x4(m00, m10, m20, m30, m01, m11, m21, m31);
        
        /// <summary>
        /// Returns the minimal component of this Matrix.
        /// </summary>
        public Half MinElement => Half.Min(Half.Min(Half.Min(Half.Min(Half.Min(Half.Min(Half.Min(m00, m01), m10), m11), m20), m21), m30), m31);
        
        /// <summary>
        /// Returns the maximal component of this Matrix.
        /// </summary>
        public Half MaxElement => Half.Max(Half.Max(Half.Max(Half.Max(Half.Max(Half.Max(Half.Max(m00, m01), m10), m11), m20), m21), m30), m31);
        
        /// <summary>
        /// Returns the euclidean length of this Matrix.
        /// </summary>
        public float Length => (float)Math.Sqrt((((m00*m00 + m01*m01) + (m10*m10 + m11*m11)) + ((m20*m20 + m21*m21) + (m30*m30 + m31*m31))));
        
        /// <summary>
        /// Returns the squared euclidean length of this Matrix.
        /// </summary>
        public float LengthSqr => (((m00*m00 + m01*m01) + (m10*m10 + m11*m11)) + ((m20*m20 + m21*m21) + (m30*m30 + m31*m31)));
        
        /// <summary>
        /// Returns the sum of all fields.
        /// </summary>
        public Half Sum => (((m00 + m01) + (m10 + m11)) + ((m20 + m21) + (m30 + m31)));
        
        /// <summary>
        /// Returns the euclidean norm of this Matrix.
        /// </summary>
        public float Norm => (float)Math.Sqrt((((m00*m00 + m01*m01) + (m10*m10 + m11*m11)) + ((m20*m20 + m21*m21) + (m30*m30 + m31*m31))));
        
        /// <summary>
        /// Returns the one-norm of this Matrix.
        /// </summary>
        public float Norm1 => (((Half.Abs(m00) + Half.Abs(m01)) + (Half.Abs(m10) + Half.Abs(m11))) + ((Half.Abs(m20) + Half.Abs(m21)) + (Half.Abs(m30) + Half.Abs(m31))));
        
        /// <summary>
        /// Returns the two-norm of this Matrix.
        /// </summary>
        public float Norm2 => (float)Math.Sqrt((((m00*m00 + m01*m01) + (m10*m10 + m11*m11)) + ((m20*m20 + m21*m21) + (m30*m30 + m31*m31))));
        
        /// <summary>
        /// Returns the max-norm of this Matrix.
        /// </summary>
        public Half NormMax => Half.Max(Half.Max(Half.Max(Half.Max(Half.Max(Half.Max(Half.Max(Half.Abs(m00), Half.Abs(m01)), Half.Abs(m10)), Half.Abs(m11)), Half.Abs(m20)), Half.Abs(m21)), Half.Abs(m30)), Half.Abs(m31));
        
        /// <summary>
        /// Returns the p-norm of this Matrix.
        /// </summary>
        public double NormP(double p) => Math.Pow((((Math.Pow((double)Half.Abs(m00), p) + Math.Pow((double)Half.Abs(m01), p)) + (Math.Pow((double)Half.Abs(m10), p) + Math.Pow((double)Half.Abs(m11), p))) + ((Math.Pow((double)Half.Abs(m20), p) + Math.Pow((double)Half.Abs(m21), p)) + (Math.Pow((double)Half.Abs(m30), p) + Math.Pow((double)Half.Abs(m31), p)))), 1 / p);
        
        /// <summary>
        /// Executes a Matrix-Matrix-multiplication hMatrix4x2 * hMatrix2x4 -> hMatrix2.
        /// </summary>
        public static hMatrix2 operator*(hMatrix4x2 lhs, hMatrix2x4 rhs) => new hMatrix2(((lhs.m00 * rhs.m00 + lhs.m10 * rhs.m01) + (lhs.m20 * rhs.m02 + lhs.m30 * rhs.m03)), ((lhs.m01 * rhs.m00 + lhs.m11 * rhs.m01) + (lhs.m21 * rhs.m02 + lhs.m31 * rhs.m03)), ((lhs.m00 * rhs.m10 + lhs.m10 * rhs.m11) + (lhs.m20 * rhs.m12 + lhs.m30 * rhs.m13)), ((lhs.m01 * rhs.m10 + lhs.m11 * rhs.m11) + (lhs.m21 * rhs.m12 + lhs.m31 * rhs.m13)));
        
        /// <summary>
        /// Executes a Matrix-Matrix-multiplication hMatrix4x2 * hMatrix3x4 -> hMatrix3x2.
        /// </summary>
        public static hMatrix3x2 operator*(hMatrix4x2 lhs, hMatrix3x4 rhs) => new hMatrix3x2(((lhs.m00 * rhs.m00 + lhs.m10 * rhs.m01) + (lhs.m20 * rhs.m02 + lhs.m30 * rhs.m03)), ((lhs.m01 * rhs.m00 + lhs.m11 * rhs.m01) + (lhs.m21 * rhs.m02 + lhs.m31 * rhs.m03)), ((lhs.m00 * rhs.m10 + lhs.m10 * rhs.m11) + (lhs.m20 * rhs.m12 + lhs.m30 * rhs.m13)), ((lhs.m01 * rhs.m10 + lhs.m11 * rhs.m11) + (lhs.m21 * rhs.m12 + lhs.m31 * rhs.m13)), ((lhs.m00 * rhs.m20 + lhs.m10 * rhs.m21) + (lhs.m20 * rhs.m22 + lhs.m30 * rhs.m23)), ((lhs.m01 * rhs.m20 + lhs.m11 * rhs.m21) + (lhs.m21 * rhs.m22 + lhs.m31 * rhs.m23)));
        
        /// <summary>
        /// Executes a Matrix-Matrix-multiplication hMatrix4x2 * hMatrix4 -> hMatrix4x2.
        /// </summary>
        public static hMatrix4x2 operator*(hMatrix4x2 lhs, hMatrix4 rhs) => new hMatrix4x2(((lhs.m00 * rhs.m00 + lhs.m10 * rhs.m01) + (lhs.m20 * rhs.m02 + lhs.m30 * rhs.m03)), ((lhs.m01 * rhs.m00 + lhs.m11 * rhs.m01) + (lhs.m21 * rhs.m02 + lhs.m31 * rhs.m03)), ((lhs.m00 * rhs.m10 + lhs.m10 * rhs.m11) + (lhs.m20 * rhs.m12 + lhs.m30 * rhs.m13)), ((lhs.m01 * rhs.m10 + lhs.m11 * rhs.m11) + (lhs.m21 * rhs.m12 + lhs.m31 * rhs.m13)), ((lhs.m00 * rhs.m20 + lhs.m10 * rhs.m21) + (lhs.m20 * rhs.m22 + lhs.m30 * rhs.m23)), ((lhs.m01 * rhs.m20 + lhs.m11 * rhs.m21) + (lhs.m21 * rhs.m22 + lhs.m31 * rhs.m23)), ((lhs.m00 * rhs.m30 + lhs.m10 * rhs.m31) + (lhs.m20 * rhs.m32 + lhs.m30 * rhs.m33)), ((lhs.m01 * rhs.m30 + lhs.m11 * rhs.m31) + (lhs.m21 * rhs.m32 + lhs.m31 * rhs.m33)));
        
        /// <summary>
        /// Executes a Matrix-Vector-multiplication.
        /// </summary>
        public static hVector2 operator*(hMatrix4x2 m, hVector4 v) => new hVector2(((m.m00 * v.x + m.m10 * v.y) + (m.m20 * v.z + m.m30 * v.w)), ((m.m01 * v.x + m.m11 * v.y) + (m.m21 * v.z + m.m31 * v.w)));
        
        /// <summary>
        /// Executes a component-wise * (multiply).
        /// </summary>
        public static hMatrix4x2 CompMul(hMatrix4x2 A, hMatrix4x2 B) => new hMatrix4x2(A.m00 * B.m00, A.m01 * B.m01, A.m10 * B.m10, A.m11 * B.m11, A.m20 * B.m20, A.m21 * B.m21, A.m30 * B.m30, A.m31 * B.m31);
        
        /// <summary>
        /// Executes a component-wise / (divide).
        /// </summary>
        public static hMatrix4x2 CompDiv(hMatrix4x2 A, hMatrix4x2 B) => new hMatrix4x2(A.m00 / B.m00, A.m01 / B.m01, A.m10 / B.m10, A.m11 / B.m11, A.m20 / B.m20, A.m21 / B.m21, A.m30 / B.m30, A.m31 / B.m31);
        
        /// <summary>
        /// Executes a component-wise + (add).
        /// </summary>
        public static hMatrix4x2 CompAdd(hMatrix4x2 A, hMatrix4x2 B) => new hMatrix4x2(A.m00 + B.m00, A.m01 + B.m01, A.m10 + B.m10, A.m11 + B.m11, A.m20 + B.m20, A.m21 + B.m21, A.m30 + B.m30, A.m31 + B.m31);
        
        /// <summary>
        /// Executes a component-wise - (subtract).
        /// </summary>
        public static hMatrix4x2 CompSub(hMatrix4x2 A, hMatrix4x2 B) => new hMatrix4x2(A.m00 - B.m00, A.m01 - B.m01, A.m10 - B.m10, A.m11 - B.m11, A.m20 - B.m20, A.m21 - B.m21, A.m30 - B.m30, A.m31 - B.m31);
        
        /// <summary>
        /// Executes a component-wise + (add).
        /// </summary>
        public static hMatrix4x2 operator+(hMatrix4x2 lhs, hMatrix4x2 rhs) => new hMatrix4x2(lhs.m00 + rhs.m00, lhs.m01 + rhs.m01, lhs.m10 + rhs.m10, lhs.m11 + rhs.m11, lhs.m20 + rhs.m20, lhs.m21 + rhs.m21, lhs.m30 + rhs.m30, lhs.m31 + rhs.m31);
        
        /// <summary>
        /// Executes a component-wise + (add) with a scalar.
        /// </summary>
        public static hMatrix4x2 operator+(hMatrix4x2 lhs, Half rhs) => new hMatrix4x2(lhs.m00 + rhs, lhs.m01 + rhs, lhs.m10 + rhs, lhs.m11 + rhs, lhs.m20 + rhs, lhs.m21 + rhs, lhs.m30 + rhs, lhs.m31 + rhs);
        
        /// <summary>
        /// Executes a component-wise + (add) with a scalar.
        /// </summary>
        public static hMatrix4x2 operator+(Half lhs, hMatrix4x2 rhs) => new hMatrix4x2(lhs + rhs.m00, lhs + rhs.m01, lhs + rhs.m10, lhs + rhs.m11, lhs + rhs.m20, lhs + rhs.m21, lhs + rhs.m30, lhs + rhs.m31);
        
        /// <summary>
        /// Executes a component-wise - (subtract).
        /// </summary>
        public static hMatrix4x2 operator-(hMatrix4x2 lhs, hMatrix4x2 rhs) => new hMatrix4x2(lhs.m00 - rhs.m00, lhs.m01 - rhs.m01, lhs.m10 - rhs.m10, lhs.m11 - rhs.m11, lhs.m20 - rhs.m20, lhs.m21 - rhs.m21, lhs.m30 - rhs.m30, lhs.m31 - rhs.m31);
        
        /// <summary>
        /// Executes a component-wise - (subtract) with a scalar.
        /// </summary>
        public static hMatrix4x2 operator-(hMatrix4x2 lhs, Half rhs) => new hMatrix4x2(lhs.m00 - rhs, lhs.m01 - rhs, lhs.m10 - rhs, lhs.m11 - rhs, lhs.m20 - rhs, lhs.m21 - rhs, lhs.m30 - rhs, lhs.m31 - rhs);
        
        /// <summary>
        /// Executes a component-wise - (subtract) with a scalar.
        /// </summary>
        public static hMatrix4x2 operator-(Half lhs, hMatrix4x2 rhs) => new hMatrix4x2(lhs - rhs.m00, lhs - rhs.m01, lhs - rhs.m10, lhs - rhs.m11, lhs - rhs.m20, lhs - rhs.m21, lhs - rhs.m30, lhs - rhs.m31);
        
        /// <summary>
        /// Executes a component-wise / (divide) with a scalar.
        /// </summary>
        public static hMatrix4x2 operator/(hMatrix4x2 lhs, Half rhs) => new hMatrix4x2(lhs.m00 / rhs, lhs.m01 / rhs, lhs.m10 / rhs, lhs.m11 / rhs, lhs.m20 / rhs, lhs.m21 / rhs, lhs.m30 / rhs, lhs.m31 / rhs);
        
        /// <summary>
        /// Executes a component-wise / (divide) with a scalar.
        /// </summary>
        public static hMatrix4x2 operator/(Half lhs, hMatrix4x2 rhs) => new hMatrix4x2(lhs / rhs.m00, lhs / rhs.m01, lhs / rhs.m10, lhs / rhs.m11, lhs / rhs.m20, lhs / rhs.m21, lhs / rhs.m30, lhs / rhs.m31);
        
        /// <summary>
        /// Executes a component-wise * (multiply) with a scalar.
        /// </summary>
        public static hMatrix4x2 operator*(hMatrix4x2 lhs, Half rhs) => new hMatrix4x2(lhs.m00 * rhs, lhs.m01 * rhs, lhs.m10 * rhs, lhs.m11 * rhs, lhs.m20 * rhs, lhs.m21 * rhs, lhs.m30 * rhs, lhs.m31 * rhs);
        
        /// <summary>
        /// Executes a component-wise * (multiply) with a scalar.
        /// </summary>
        public static hMatrix4x2 operator*(Half lhs, hMatrix4x2 rhs) => new hMatrix4x2(lhs * rhs.m00, lhs * rhs.m01, lhs * rhs.m10, lhs * rhs.m11, lhs * rhs.m20, lhs * rhs.m21, lhs * rhs.m30, lhs * rhs.m31);
        
        /// <summary>
        /// Executes a component-wise lesser-than comparison.
        /// </summary>
        public static bMatrix4x2 operator<(hMatrix4x2 lhs, hMatrix4x2 rhs) => new bMatrix4x2(lhs.m00 < rhs.m00, lhs.m01 < rhs.m01, lhs.m10 < rhs.m10, lhs.m11 < rhs.m11, lhs.m20 < rhs.m20, lhs.m21 < rhs.m21, lhs.m30 < rhs.m30, lhs.m31 < rhs.m31);
        
        /// <summary>
        /// Executes a component-wise lesser-than comparison with a scalar.
        /// </summary>
        public static bMatrix4x2 operator<(hMatrix4x2 lhs, Half rhs) => new bMatrix4x2(lhs.m00 < rhs, lhs.m01 < rhs, lhs.m10 < rhs, lhs.m11 < rhs, lhs.m20 < rhs, lhs.m21 < rhs, lhs.m30 < rhs, lhs.m31 < rhs);
        
        /// <summary>
        /// Executes a component-wise lesser-than comparison with a scalar.
        /// </summary>
        public static bMatrix4x2 operator<(Half lhs, hMatrix4x2 rhs) => new bMatrix4x2(lhs < rhs.m00, lhs < rhs.m01, lhs < rhs.m10, lhs < rhs.m11, lhs < rhs.m20, lhs < rhs.m21, lhs < rhs.m30, lhs < rhs.m31);
        
        /// <summary>
        /// Executes a component-wise lesser-or-equal comparison.
        /// </summary>
        public static bMatrix4x2 operator<=(hMatrix4x2 lhs, hMatrix4x2 rhs) => new bMatrix4x2(lhs.m00 <= rhs.m00, lhs.m01 <= rhs.m01, lhs.m10 <= rhs.m10, lhs.m11 <= rhs.m11, lhs.m20 <= rhs.m20, lhs.m21 <= rhs.m21, lhs.m30 <= rhs.m30, lhs.m31 <= rhs.m31);
        
        /// <summary>
        /// Executes a component-wise lesser-or-equal comparison with a scalar.
        /// </summary>
        public static bMatrix4x2 operator<=(hMatrix4x2 lhs, Half rhs) => new bMatrix4x2(lhs.m00 <= rhs, lhs.m01 <= rhs, lhs.m10 <= rhs, lhs.m11 <= rhs, lhs.m20 <= rhs, lhs.m21 <= rhs, lhs.m30 <= rhs, lhs.m31 <= rhs);
        
        /// <summary>
        /// Executes a component-wise lesser-or-equal comparison with a scalar.
        /// </summary>
        public static bMatrix4x2 operator<=(Half lhs, hMatrix4x2 rhs) => new bMatrix4x2(lhs <= rhs.m00, lhs <= rhs.m01, lhs <= rhs.m10, lhs <= rhs.m11, lhs <= rhs.m20, lhs <= rhs.m21, lhs <= rhs.m30, lhs <= rhs.m31);
        
        /// <summary>
        /// Executes a component-wise greater-than comparison.
        /// </summary>
        public static bMatrix4x2 operator>(hMatrix4x2 lhs, hMatrix4x2 rhs) => new bMatrix4x2(lhs.m00 > rhs.m00, lhs.m01 > rhs.m01, lhs.m10 > rhs.m10, lhs.m11 > rhs.m11, lhs.m20 > rhs.m20, lhs.m21 > rhs.m21, lhs.m30 > rhs.m30, lhs.m31 > rhs.m31);
        
        /// <summary>
        /// Executes a component-wise greater-than comparison with a scalar.
        /// </summary>
        public static bMatrix4x2 operator>(hMatrix4x2 lhs, Half rhs) => new bMatrix4x2(lhs.m00 > rhs, lhs.m01 > rhs, lhs.m10 > rhs, lhs.m11 > rhs, lhs.m20 > rhs, lhs.m21 > rhs, lhs.m30 > rhs, lhs.m31 > rhs);
        
        /// <summary>
        /// Executes a component-wise greater-than comparison with a scalar.
        /// </summary>
        public static bMatrix4x2 operator>(Half lhs, hMatrix4x2 rhs) => new bMatrix4x2(lhs > rhs.m00, lhs > rhs.m01, lhs > rhs.m10, lhs > rhs.m11, lhs > rhs.m20, lhs > rhs.m21, lhs > rhs.m30, lhs > rhs.m31);
        
        /// <summary>
        /// Executes a component-wise greater-or-equal comparison.
        /// </summary>
        public static bMatrix4x2 operator>=(hMatrix4x2 lhs, hMatrix4x2 rhs) => new bMatrix4x2(lhs.m00 >= rhs.m00, lhs.m01 >= rhs.m01, lhs.m10 >= rhs.m10, lhs.m11 >= rhs.m11, lhs.m20 >= rhs.m20, lhs.m21 >= rhs.m21, lhs.m30 >= rhs.m30, lhs.m31 >= rhs.m31);
        
        /// <summary>
        /// Executes a component-wise greater-or-equal comparison with a scalar.
        /// </summary>
        public static bMatrix4x2 operator>=(hMatrix4x2 lhs, Half rhs) => new bMatrix4x2(lhs.m00 >= rhs, lhs.m01 >= rhs, lhs.m10 >= rhs, lhs.m11 >= rhs, lhs.m20 >= rhs, lhs.m21 >= rhs, lhs.m30 >= rhs, lhs.m31 >= rhs);
        
        /// <summary>
        /// Executes a component-wise greater-or-equal comparison with a scalar.
        /// </summary>
        public static bMatrix4x2 operator>=(Half lhs, hMatrix4x2 rhs) => new bMatrix4x2(lhs >= rhs.m00, lhs >= rhs.m01, lhs >= rhs.m10, lhs >= rhs.m11, lhs >= rhs.m20, lhs >= rhs.m21, lhs >= rhs.m30, lhs >= rhs.m31);
    }
}
