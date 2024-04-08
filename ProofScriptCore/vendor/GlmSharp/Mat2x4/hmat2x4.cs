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
    /// A Matrix of type Half with 2 columns and 4 rows.
    /// </summary>
    [RegisterCoreClassStruct]
    
    [StructLayout(LayoutKind.Sequential)]
    public struct hMatrix2x4 : IReadOnlyList<Half>, IEquatable<hMatrix2x4>
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
        /// Column 0, Rows 2
        /// </summary>
        
        public Half m02;
        
        /// <summary>
        /// Column 0, Rows 3
        /// </summary>
        
        public Half m03;
        
        /// <summary>
        /// Column 1, Rows 0
        /// </summary>
        
        public Half m10;
        
        /// <summary>
        /// Column 1, Rows 1
        /// </summary>
        
        public Half m11;
        
        /// <summary>
        /// Column 1, Rows 2
        /// </summary>
        
        public Half m12;
        
        /// <summary>
        /// Column 1, Rows 3
        /// </summary>
        
        public Half m13;

        #endregion


        #region Constructors
        
        /// <summary>
        /// Component-wise constructor
        /// </summary>
        public hMatrix2x4(Half m00, Half m01, Half m02, Half m03, Half m10, Half m11, Half m12, Half m13)
        {
            this.m00 = m00;
            this.m01 = m01;
            this.m02 = m02;
            this.m03 = m03;
            this.m10 = m10;
            this.m11 = m11;
            this.m12 = m12;
            this.m13 = m13;
        }
        
        /// <summary>
        /// Constructs this Matrix from a hMatrix2. Non-overwritten fields are from an Identity Matrix.
        /// </summary>
        public hMatrix2x4(hMatrix2 m)
        {
            this.m00 = m.m00;
            this.m01 = m.m01;
            this.m02 = Half.Zero;
            this.m03 = Half.Zero;
            this.m10 = m.m10;
            this.m11 = m.m11;
            this.m12 = Half.Zero;
            this.m13 = Half.Zero;
        }
        
        /// <summary>
        /// Constructs this Matrix from a hMatrix3x2. Non-overwritten fields are from an Identity Matrix.
        /// </summary>
        public hMatrix2x4(hMatrix3x2 m)
        {
            this.m00 = m.m00;
            this.m01 = m.m01;
            this.m02 = Half.Zero;
            this.m03 = Half.Zero;
            this.m10 = m.m10;
            this.m11 = m.m11;
            this.m12 = Half.Zero;
            this.m13 = Half.Zero;
        }
        
        /// <summary>
        /// Constructs this Matrix from a hMatrix4x2. Non-overwritten fields are from an Identity Matrix.
        /// </summary>
        public hMatrix2x4(hMatrix4x2 m)
        {
            this.m00 = m.m00;
            this.m01 = m.m01;
            this.m02 = Half.Zero;
            this.m03 = Half.Zero;
            this.m10 = m.m10;
            this.m11 = m.m11;
            this.m12 = Half.Zero;
            this.m13 = Half.Zero;
        }
        
        /// <summary>
        /// Constructs this Matrix from a hMatrix2x3. Non-overwritten fields are from an Identity Matrix.
        /// </summary>
        public hMatrix2x4(hMatrix2x3 m)
        {
            this.m00 = m.m00;
            this.m01 = m.m01;
            this.m02 = m.m02;
            this.m03 = Half.Zero;
            this.m10 = m.m10;
            this.m11 = m.m11;
            this.m12 = m.m12;
            this.m13 = Half.Zero;
        }
        
        /// <summary>
        /// Constructs this Matrix from a hMatrix3. Non-overwritten fields are from an Identity Matrix.
        /// </summary>
        public hMatrix2x4(hMatrix3 m)
        {
            this.m00 = m.m00;
            this.m01 = m.m01;
            this.m02 = m.m02;
            this.m03 = Half.Zero;
            this.m10 = m.m10;
            this.m11 = m.m11;
            this.m12 = m.m12;
            this.m13 = Half.Zero;
        }
        
        /// <summary>
        /// Constructs this Matrix from a hMatrix4x3. Non-overwritten fields are from an Identity Matrix.
        /// </summary>
        public hMatrix2x4(hMatrix4x3 m)
        {
            this.m00 = m.m00;
            this.m01 = m.m01;
            this.m02 = m.m02;
            this.m03 = Half.Zero;
            this.m10 = m.m10;
            this.m11 = m.m11;
            this.m12 = m.m12;
            this.m13 = Half.Zero;
        }
        
        /// <summary>
        /// Constructs this Matrix from a hMatrix2x4. Non-overwritten fields are from an Identity Matrix.
        /// </summary>
        public hMatrix2x4(hMatrix2x4 m)
        {
            this.m00 = m.m00;
            this.m01 = m.m01;
            this.m02 = m.m02;
            this.m03 = m.m03;
            this.m10 = m.m10;
            this.m11 = m.m11;
            this.m12 = m.m12;
            this.m13 = m.m13;
        }
        
        /// <summary>
        /// Constructs this Matrix from a hMatrix3x4. Non-overwritten fields are from an Identity Matrix.
        /// </summary>
        public hMatrix2x4(hMatrix3x4 m)
        {
            this.m00 = m.m00;
            this.m01 = m.m01;
            this.m02 = m.m02;
            this.m03 = m.m03;
            this.m10 = m.m10;
            this.m11 = m.m11;
            this.m12 = m.m12;
            this.m13 = m.m13;
        }
        
        /// <summary>
        /// Constructs this Matrix from a hMatrix4. Non-overwritten fields are from an Identity Matrix.
        /// </summary>
        public hMatrix2x4(hMatrix4 m)
        {
            this.m00 = m.m00;
            this.m01 = m.m01;
            this.m02 = m.m02;
            this.m03 = m.m03;
            this.m10 = m.m10;
            this.m11 = m.m11;
            this.m12 = m.m12;
            this.m13 = m.m13;
        }
        
        /// <summary>
        /// Constructs this Matrix from a series of column Vectors. Non-overwritten fields are from an Identity Matrix.
        /// </summary>
        public hMatrix2x4(hVector2 c0, hVector2 c1)
        {
            this.m00 = c0.x;
            this.m01 = c0.y;
            this.m02 = Half.Zero;
            this.m03 = Half.Zero;
            this.m10 = c1.x;
            this.m11 = c1.y;
            this.m12 = Half.Zero;
            this.m13 = Half.Zero;
        }
        
        /// <summary>
        /// Constructs this Matrix from a series of column Vectors. Non-overwritten fields are from an Identity Matrix.
        /// </summary>
        public hMatrix2x4(hVector3 c0, hVector3 c1)
        {
            this.m00 = c0.x;
            this.m01 = c0.y;
            this.m02 = c0.z;
            this.m03 = Half.Zero;
            this.m10 = c1.x;
            this.m11 = c1.y;
            this.m12 = c1.z;
            this.m13 = Half.Zero;
        }
        
        /// <summary>
        /// Constructs this Matrix from a series of column Vectors. Non-overwritten fields are from an Identity Matrix.
        /// </summary>
        public hMatrix2x4(hVector4 c0, hVector4 c1)
        {
            this.m00 = c0.x;
            this.m01 = c0.y;
            this.m02 = c0.z;
            this.m03 = c0.w;
            this.m10 = c1.x;
            this.m11 = c1.y;
            this.m12 = c1.z;
            this.m13 = c1.w;
        }

        #endregion


        #region Properties
        
        /// <summary>
        /// Creates a 2D array with all values (address: Values[x, y])
        /// </summary>
        public Half[,] Values => new[,] { { m00, m01, m02, m03 }, { m10, m11, m12, m13 } };
        
        /// <summary>
        /// Creates a 1D array with all values (internal order)
        /// </summary>
        public Half[] Values1D => new[] { m00, m01, m02, m03, m10, m11, m12, m13 };
        
        /// <summary>
        /// Gets or sets the column nr 0
        /// </summary>
        public hVector4 Column0
        {
            get
            {
                return new hVector4(m00, m01, m02, m03);
            }
            set
            {
                m00 = value.x;
                m01 = value.y;
                m02 = value.z;
                m03 = value.w;
            }
        }
        
        /// <summary>
        /// Gets or sets the column nr 1
        /// </summary>
        public hVector4 Column1
        {
            get
            {
                return new hVector4(m10, m11, m12, m13);
            }
            set
            {
                m10 = value.x;
                m11 = value.y;
                m12 = value.z;
                m13 = value.w;
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
        
        /// <summary>
        /// Gets or sets the row nr 2
        /// </summary>
        public hVector2 Row2
        {
            get
            {
                return new hVector2(m02, m12);
            }
            set
            {
                m02 = value.x;
                m12 = value.y;
            }
        }
        
        /// <summary>
        /// Gets or sets the row nr 3
        /// </summary>
        public hVector2 Row3
        {
            get
            {
                return new hVector2(m03, m13);
            }
            set
            {
                m03 = value.x;
                m13 = value.y;
            }
        }

        #endregion


        #region Static Properties
        
        /// <summary>
        /// Predefined all-zero Matrix
        /// </summary>
        public static hMatrix2x4 Zero { get; } = new hMatrix2x4(Half.Zero, Half.Zero, Half.Zero, Half.Zero, Half.Zero, Half.Zero, Half.Zero, Half.Zero);
        
        /// <summary>
        /// Predefined all-ones Matrix
        /// </summary>
        public static hMatrix2x4 Ones { get; } = new hMatrix2x4(Half.One, Half.One, Half.One, Half.One, Half.One, Half.One, Half.One, Half.One);
        
        /// <summary>
        /// Predefined identity Matrix
        /// </summary>
        public static hMatrix2x4 Identity { get; } = new hMatrix2x4(Half.One, Half.Zero, Half.Zero, Half.Zero, Half.Zero, Half.One, Half.Zero, Half.Zero);
        
        /// <summary>
        /// Predefined all-MaxValue Matrix
        /// </summary>
        public static hMatrix2x4 AllMaxValue { get; } = new hMatrix2x4(Half.MaxValue, Half.MaxValue, Half.MaxValue, Half.MaxValue, Half.MaxValue, Half.MaxValue, Half.MaxValue, Half.MaxValue);
        
        /// <summary>
        /// Predefined diagonal-MaxValue Matrix
        /// </summary>
        public static hMatrix2x4 DiagonalMaxValue { get; } = new hMatrix2x4(Half.MaxValue, Half.Zero, Half.Zero, Half.Zero, Half.Zero, Half.MaxValue, Half.Zero, Half.Zero);
        
        /// <summary>
        /// Predefined all-MinValue Matrix
        /// </summary>
        public static hMatrix2x4 AllMinValue { get; } = new hMatrix2x4(Half.MinValue, Half.MinValue, Half.MinValue, Half.MinValue, Half.MinValue, Half.MinValue, Half.MinValue, Half.MinValue);
        
        /// <summary>
        /// Predefined diagonal-MinValue Matrix
        /// </summary>
        public static hMatrix2x4 DiagonalMinValue { get; } = new hMatrix2x4(Half.MinValue, Half.Zero, Half.Zero, Half.Zero, Half.Zero, Half.MinValue, Half.Zero, Half.Zero);
        
        /// <summary>
        /// Predefined all-Epsilon Matrix
        /// </summary>
        public static hMatrix2x4 AllEpsilon { get; } = new hMatrix2x4(Half.Epsilon, Half.Epsilon, Half.Epsilon, Half.Epsilon, Half.Epsilon, Half.Epsilon, Half.Epsilon, Half.Epsilon);
        
        /// <summary>
        /// Predefined diagonal-Epsilon Matrix
        /// </summary>
        public static hMatrix2x4 DiagonalEpsilon { get; } = new hMatrix2x4(Half.Epsilon, Half.Zero, Half.Zero, Half.Zero, Half.Zero, Half.Epsilon, Half.Zero, Half.Zero);
        
        /// <summary>
        /// Predefined all-NaN Matrix
        /// </summary>
        public static hMatrix2x4 AllNaN { get; } = new hMatrix2x4(Half.NaN, Half.NaN, Half.NaN, Half.NaN, Half.NaN, Half.NaN, Half.NaN, Half.NaN);
        
        /// <summary>
        /// Predefined diagonal-NaN Matrix
        /// </summary>
        public static hMatrix2x4 DiagonalNaN { get; } = new hMatrix2x4(Half.NaN, Half.Zero, Half.Zero, Half.Zero, Half.Zero, Half.NaN, Half.Zero, Half.Zero);
        
        /// <summary>
        /// Predefined all-NegativeInfinity Matrix
        /// </summary>
        public static hMatrix2x4 AllNegativeInfinity { get; } = new hMatrix2x4(Half.NegativeInfinity, Half.NegativeInfinity, Half.NegativeInfinity, Half.NegativeInfinity, Half.NegativeInfinity, Half.NegativeInfinity, Half.NegativeInfinity, Half.NegativeInfinity);
        
        /// <summary>
        /// Predefined diagonal-NegativeInfinity Matrix
        /// </summary>
        public static hMatrix2x4 DiagonalNegativeInfinity { get; } = new hMatrix2x4(Half.NegativeInfinity, Half.Zero, Half.Zero, Half.Zero, Half.Zero, Half.NegativeInfinity, Half.Zero, Half.Zero);
        
        /// <summary>
        /// Predefined all-PositiveInfinity Matrix
        /// </summary>
        public static hMatrix2x4 AllPositiveInfinity { get; } = new hMatrix2x4(Half.PositiveInfinity, Half.PositiveInfinity, Half.PositiveInfinity, Half.PositiveInfinity, Half.PositiveInfinity, Half.PositiveInfinity, Half.PositiveInfinity, Half.PositiveInfinity);
        
        /// <summary>
        /// Predefined diagonal-PositiveInfinity Matrix
        /// </summary>
        public static hMatrix2x4 DiagonalPositiveInfinity { get; } = new hMatrix2x4(Half.PositiveInfinity, Half.Zero, Half.Zero, Half.Zero, Half.Zero, Half.PositiveInfinity, Half.Zero, Half.Zero);

        #endregion


        #region Functions
        
        /// <summary>
        /// Returns an enumerator that iterates through all fields.
        /// </summary>
        public IEnumerator<Half> GetEnumerator()
        {
            yield return m00;
            yield return m01;
            yield return m02;
            yield return m03;
            yield return m10;
            yield return m11;
            yield return m12;
            yield return m13;
        }
        
        /// <summary>
        /// Returns an enumerator that iterates through all fields.
        /// </summary>
        IEnumerator IEnumerable.GetEnumerator() => GetEnumerator();

        #endregion

        
        /// <summary>
        /// Returns the number of Fields (2 x 4 = 8).
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
                    case 2: return m02;
                    case 3: return m03;
                    case 4: return m10;
                    case 5: return m11;
                    case 6: return m12;
                    case 7: return m13;
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
                    case 3: this.m03 = value; break;
                    case 4: this.m10 = value; break;
                    case 5: this.m11 = value; break;
                    case 6: this.m12 = value; break;
                    case 7: this.m13 = value; break;
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
                return this[col * 4 + row];
            }
            set
            {
                this[col * 4 + row] = value;
            }
        }
        
        /// <summary>
        /// Returns true iff this equals rhs component-wise.
        /// </summary>
        public bool Equals(hMatrix2x4 rhs) => (((m00.Equals(rhs.m00) && m01.Equals(rhs.m01)) && (m02.Equals(rhs.m02) && m03.Equals(rhs.m03))) && ((m10.Equals(rhs.m10) && m11.Equals(rhs.m11)) && (m12.Equals(rhs.m12) && m13.Equals(rhs.m13))));
        
        /// <summary>
        /// Returns true iff this equals rhs type- and component-wise.
        /// </summary>
        public override bool Equals(object obj)
        {
            if (ReferenceEquals(null, obj)) return false;
            return obj is hMatrix2x4 && Equals((hMatrix2x4) obj);
        }
        
        /// <summary>
        /// Returns true iff this equals rhs component-wise.
        /// </summary>
        public static bool operator ==(hMatrix2x4 lhs, hMatrix2x4 rhs) => lhs.Equals(rhs);
        
        /// <summary>
        /// Returns true iff this does not equal rhs (component-wise).
        /// </summary>
        public static bool operator !=(hMatrix2x4 lhs, hMatrix2x4 rhs) => !lhs.Equals(rhs);
        
        /// <summary>
        /// Returns a hash code for this instance.
        /// </summary>
        public override int GetHashCode()
        {
            unchecked
            {
                return ((((((((((((((m00.GetHashCode()) * 397) ^ m01.GetHashCode()) * 397) ^ m02.GetHashCode()) * 397) ^ m03.GetHashCode()) * 397) ^ m10.GetHashCode()) * 397) ^ m11.GetHashCode()) * 397) ^ m12.GetHashCode()) * 397) ^ m13.GetHashCode();
            }
        }
        
        /// <summary>
        /// Returns a transposed version of this Matrix.
        /// </summary>
        public hMatrix4x2 Transposed => new hMatrix4x2(m00, m10, m01, m11, m02, m12, m03, m13);
        
        /// <summary>
        /// Returns the minimal component of this Matrix.
        /// </summary>
        public Half MinElement => Half.Min(Half.Min(Half.Min(Half.Min(Half.Min(Half.Min(Half.Min(m00, m01), m02), m03), m10), m11), m12), m13);
        
        /// <summary>
        /// Returns the maximal component of this Matrix.
        /// </summary>
        public Half MaxElement => Half.Max(Half.Max(Half.Max(Half.Max(Half.Max(Half.Max(Half.Max(m00, m01), m02), m03), m10), m11), m12), m13);
        
        /// <summary>
        /// Returns the euclidean length of this Matrix.
        /// </summary>
        public float Length => (float)Math.Sqrt((((m00*m00 + m01*m01) + (m02*m02 + m03*m03)) + ((m10*m10 + m11*m11) + (m12*m12 + m13*m13))));
        
        /// <summary>
        /// Returns the squared euclidean length of this Matrix.
        /// </summary>
        public float LengthSqr => (((m00*m00 + m01*m01) + (m02*m02 + m03*m03)) + ((m10*m10 + m11*m11) + (m12*m12 + m13*m13)));
        
        /// <summary>
        /// Returns the sum of all fields.
        /// </summary>
        public Half Sum => (((m00 + m01) + (m02 + m03)) + ((m10 + m11) + (m12 + m13)));
        
        /// <summary>
        /// Returns the euclidean norm of this Matrix.
        /// </summary>
        public float Norm => (float)Math.Sqrt((((m00*m00 + m01*m01) + (m02*m02 + m03*m03)) + ((m10*m10 + m11*m11) + (m12*m12 + m13*m13))));
        
        /// <summary>
        /// Returns the one-norm of this Matrix.
        /// </summary>
        public float Norm1 => (((Half.Abs(m00) + Half.Abs(m01)) + (Half.Abs(m02) + Half.Abs(m03))) + ((Half.Abs(m10) + Half.Abs(m11)) + (Half.Abs(m12) + Half.Abs(m13))));
        
        /// <summary>
        /// Returns the two-norm of this Matrix.
        /// </summary>
        public float Norm2 => (float)Math.Sqrt((((m00*m00 + m01*m01) + (m02*m02 + m03*m03)) + ((m10*m10 + m11*m11) + (m12*m12 + m13*m13))));
        
        /// <summary>
        /// Returns the max-norm of this Matrix.
        /// </summary>
        public Half NormMax => Half.Max(Half.Max(Half.Max(Half.Max(Half.Max(Half.Max(Half.Max(Half.Abs(m00), Half.Abs(m01)), Half.Abs(m02)), Half.Abs(m03)), Half.Abs(m10)), Half.Abs(m11)), Half.Abs(m12)), Half.Abs(m13));
        
        /// <summary>
        /// Returns the p-norm of this Matrix.
        /// </summary>
        public double NormP(double p) => Math.Pow((((Math.Pow((double)Half.Abs(m00), p) + Math.Pow((double)Half.Abs(m01), p)) + (Math.Pow((double)Half.Abs(m02), p) + Math.Pow((double)Half.Abs(m03), p))) + ((Math.Pow((double)Half.Abs(m10), p) + Math.Pow((double)Half.Abs(m11), p)) + (Math.Pow((double)Half.Abs(m12), p) + Math.Pow((double)Half.Abs(m13), p)))), 1 / p);
        
        /// <summary>
        /// Executes a Matrix-Matrix-multiplication hMatrix2x4 * hMatrix2 -> hMatrix2x4.
        /// </summary>
        public static hMatrix2x4 operator*(hMatrix2x4 lhs, hMatrix2 rhs) => new hMatrix2x4((lhs.m00 * rhs.m00 + lhs.m10 * rhs.m01), (lhs.m01 * rhs.m00 + lhs.m11 * rhs.m01), (lhs.m02 * rhs.m00 + lhs.m12 * rhs.m01), (lhs.m03 * rhs.m00 + lhs.m13 * rhs.m01), (lhs.m00 * rhs.m10 + lhs.m10 * rhs.m11), (lhs.m01 * rhs.m10 + lhs.m11 * rhs.m11), (lhs.m02 * rhs.m10 + lhs.m12 * rhs.m11), (lhs.m03 * rhs.m10 + lhs.m13 * rhs.m11));
        
        /// <summary>
        /// Executes a Matrix-Matrix-multiplication hMatrix2x4 * hMatrix3x2 -> hMatrix3x4.
        /// </summary>
        public static hMatrix3x4 operator*(hMatrix2x4 lhs, hMatrix3x2 rhs) => new hMatrix3x4((lhs.m00 * rhs.m00 + lhs.m10 * rhs.m01), (lhs.m01 * rhs.m00 + lhs.m11 * rhs.m01), (lhs.m02 * rhs.m00 + lhs.m12 * rhs.m01), (lhs.m03 * rhs.m00 + lhs.m13 * rhs.m01), (lhs.m00 * rhs.m10 + lhs.m10 * rhs.m11), (lhs.m01 * rhs.m10 + lhs.m11 * rhs.m11), (lhs.m02 * rhs.m10 + lhs.m12 * rhs.m11), (lhs.m03 * rhs.m10 + lhs.m13 * rhs.m11), (lhs.m00 * rhs.m20 + lhs.m10 * rhs.m21), (lhs.m01 * rhs.m20 + lhs.m11 * rhs.m21), (lhs.m02 * rhs.m20 + lhs.m12 * rhs.m21), (lhs.m03 * rhs.m20 + lhs.m13 * rhs.m21));
        
        /// <summary>
        /// Executes a Matrix-Matrix-multiplication hMatrix2x4 * hMatrix4x2 -> hMatrix4.
        /// </summary>
        public static hMatrix4 operator*(hMatrix2x4 lhs, hMatrix4x2 rhs) => new hMatrix4((lhs.m00 * rhs.m00 + lhs.m10 * rhs.m01), (lhs.m01 * rhs.m00 + lhs.m11 * rhs.m01), (lhs.m02 * rhs.m00 + lhs.m12 * rhs.m01), (lhs.m03 * rhs.m00 + lhs.m13 * rhs.m01), (lhs.m00 * rhs.m10 + lhs.m10 * rhs.m11), (lhs.m01 * rhs.m10 + lhs.m11 * rhs.m11), (lhs.m02 * rhs.m10 + lhs.m12 * rhs.m11), (lhs.m03 * rhs.m10 + lhs.m13 * rhs.m11), (lhs.m00 * rhs.m20 + lhs.m10 * rhs.m21), (lhs.m01 * rhs.m20 + lhs.m11 * rhs.m21), (lhs.m02 * rhs.m20 + lhs.m12 * rhs.m21), (lhs.m03 * rhs.m20 + lhs.m13 * rhs.m21), (lhs.m00 * rhs.m30 + lhs.m10 * rhs.m31), (lhs.m01 * rhs.m30 + lhs.m11 * rhs.m31), (lhs.m02 * rhs.m30 + lhs.m12 * rhs.m31), (lhs.m03 * rhs.m30 + lhs.m13 * rhs.m31));
        
        /// <summary>
        /// Executes a Matrix-Vector-multiplication.
        /// </summary>
        public static hVector4 operator*(hMatrix2x4 m, hVector2 v) => new hVector4((m.m00 * v.x + m.m10 * v.y), (m.m01 * v.x + m.m11 * v.y), (m.m02 * v.x + m.m12 * v.y), (m.m03 * v.x + m.m13 * v.y));
        
        /// <summary>
        /// Executes a component-wise * (multiply).
        /// </summary>
        public static hMatrix2x4 CompMul(hMatrix2x4 A, hMatrix2x4 B) => new hMatrix2x4(A.m00 * B.m00, A.m01 * B.m01, A.m02 * B.m02, A.m03 * B.m03, A.m10 * B.m10, A.m11 * B.m11, A.m12 * B.m12, A.m13 * B.m13);
        
        /// <summary>
        /// Executes a component-wise / (divide).
        /// </summary>
        public static hMatrix2x4 CompDiv(hMatrix2x4 A, hMatrix2x4 B) => new hMatrix2x4(A.m00 / B.m00, A.m01 / B.m01, A.m02 / B.m02, A.m03 / B.m03, A.m10 / B.m10, A.m11 / B.m11, A.m12 / B.m12, A.m13 / B.m13);
        
        /// <summary>
        /// Executes a component-wise + (add).
        /// </summary>
        public static hMatrix2x4 CompAdd(hMatrix2x4 A, hMatrix2x4 B) => new hMatrix2x4(A.m00 + B.m00, A.m01 + B.m01, A.m02 + B.m02, A.m03 + B.m03, A.m10 + B.m10, A.m11 + B.m11, A.m12 + B.m12, A.m13 + B.m13);
        
        /// <summary>
        /// Executes a component-wise - (subtract).
        /// </summary>
        public static hMatrix2x4 CompSub(hMatrix2x4 A, hMatrix2x4 B) => new hMatrix2x4(A.m00 - B.m00, A.m01 - B.m01, A.m02 - B.m02, A.m03 - B.m03, A.m10 - B.m10, A.m11 - B.m11, A.m12 - B.m12, A.m13 - B.m13);
        
        /// <summary>
        /// Executes a component-wise + (add).
        /// </summary>
        public static hMatrix2x4 operator+(hMatrix2x4 lhs, hMatrix2x4 rhs) => new hMatrix2x4(lhs.m00 + rhs.m00, lhs.m01 + rhs.m01, lhs.m02 + rhs.m02, lhs.m03 + rhs.m03, lhs.m10 + rhs.m10, lhs.m11 + rhs.m11, lhs.m12 + rhs.m12, lhs.m13 + rhs.m13);
        
        /// <summary>
        /// Executes a component-wise + (add) with a scalar.
        /// </summary>
        public static hMatrix2x4 operator+(hMatrix2x4 lhs, Half rhs) => new hMatrix2x4(lhs.m00 + rhs, lhs.m01 + rhs, lhs.m02 + rhs, lhs.m03 + rhs, lhs.m10 + rhs, lhs.m11 + rhs, lhs.m12 + rhs, lhs.m13 + rhs);
        
        /// <summary>
        /// Executes a component-wise + (add) with a scalar.
        /// </summary>
        public static hMatrix2x4 operator+(Half lhs, hMatrix2x4 rhs) => new hMatrix2x4(lhs + rhs.m00, lhs + rhs.m01, lhs + rhs.m02, lhs + rhs.m03, lhs + rhs.m10, lhs + rhs.m11, lhs + rhs.m12, lhs + rhs.m13);
        
        /// <summary>
        /// Executes a component-wise - (subtract).
        /// </summary>
        public static hMatrix2x4 operator-(hMatrix2x4 lhs, hMatrix2x4 rhs) => new hMatrix2x4(lhs.m00 - rhs.m00, lhs.m01 - rhs.m01, lhs.m02 - rhs.m02, lhs.m03 - rhs.m03, lhs.m10 - rhs.m10, lhs.m11 - rhs.m11, lhs.m12 - rhs.m12, lhs.m13 - rhs.m13);
        
        /// <summary>
        /// Executes a component-wise - (subtract) with a scalar.
        /// </summary>
        public static hMatrix2x4 operator-(hMatrix2x4 lhs, Half rhs) => new hMatrix2x4(lhs.m00 - rhs, lhs.m01 - rhs, lhs.m02 - rhs, lhs.m03 - rhs, lhs.m10 - rhs, lhs.m11 - rhs, lhs.m12 - rhs, lhs.m13 - rhs);
        
        /// <summary>
        /// Executes a component-wise - (subtract) with a scalar.
        /// </summary>
        public static hMatrix2x4 operator-(Half lhs, hMatrix2x4 rhs) => new hMatrix2x4(lhs - rhs.m00, lhs - rhs.m01, lhs - rhs.m02, lhs - rhs.m03, lhs - rhs.m10, lhs - rhs.m11, lhs - rhs.m12, lhs - rhs.m13);
        
        /// <summary>
        /// Executes a component-wise / (divide) with a scalar.
        /// </summary>
        public static hMatrix2x4 operator/(hMatrix2x4 lhs, Half rhs) => new hMatrix2x4(lhs.m00 / rhs, lhs.m01 / rhs, lhs.m02 / rhs, lhs.m03 / rhs, lhs.m10 / rhs, lhs.m11 / rhs, lhs.m12 / rhs, lhs.m13 / rhs);
        
        /// <summary>
        /// Executes a component-wise / (divide) with a scalar.
        /// </summary>
        public static hMatrix2x4 operator/(Half lhs, hMatrix2x4 rhs) => new hMatrix2x4(lhs / rhs.m00, lhs / rhs.m01, lhs / rhs.m02, lhs / rhs.m03, lhs / rhs.m10, lhs / rhs.m11, lhs / rhs.m12, lhs / rhs.m13);
        
        /// <summary>
        /// Executes a component-wise * (multiply) with a scalar.
        /// </summary>
        public static hMatrix2x4 operator*(hMatrix2x4 lhs, Half rhs) => new hMatrix2x4(lhs.m00 * rhs, lhs.m01 * rhs, lhs.m02 * rhs, lhs.m03 * rhs, lhs.m10 * rhs, lhs.m11 * rhs, lhs.m12 * rhs, lhs.m13 * rhs);
        
        /// <summary>
        /// Executes a component-wise * (multiply) with a scalar.
        /// </summary>
        public static hMatrix2x4 operator*(Half lhs, hMatrix2x4 rhs) => new hMatrix2x4(lhs * rhs.m00, lhs * rhs.m01, lhs * rhs.m02, lhs * rhs.m03, lhs * rhs.m10, lhs * rhs.m11, lhs * rhs.m12, lhs * rhs.m13);
        
        /// <summary>
        /// Executes a component-wise lesser-than comparison.
        /// </summary>
        public static bMatrix2x4 operator<(hMatrix2x4 lhs, hMatrix2x4 rhs) => new bMatrix2x4(lhs.m00 < rhs.m00, lhs.m01 < rhs.m01, lhs.m02 < rhs.m02, lhs.m03 < rhs.m03, lhs.m10 < rhs.m10, lhs.m11 < rhs.m11, lhs.m12 < rhs.m12, lhs.m13 < rhs.m13);
        
        /// <summary>
        /// Executes a component-wise lesser-than comparison with a scalar.
        /// </summary>
        public static bMatrix2x4 operator<(hMatrix2x4 lhs, Half rhs) => new bMatrix2x4(lhs.m00 < rhs, lhs.m01 < rhs, lhs.m02 < rhs, lhs.m03 < rhs, lhs.m10 < rhs, lhs.m11 < rhs, lhs.m12 < rhs, lhs.m13 < rhs);
        
        /// <summary>
        /// Executes a component-wise lesser-than comparison with a scalar.
        /// </summary>
        public static bMatrix2x4 operator<(Half lhs, hMatrix2x4 rhs) => new bMatrix2x4(lhs < rhs.m00, lhs < rhs.m01, lhs < rhs.m02, lhs < rhs.m03, lhs < rhs.m10, lhs < rhs.m11, lhs < rhs.m12, lhs < rhs.m13);
        
        /// <summary>
        /// Executes a component-wise lesser-or-equal comparison.
        /// </summary>
        public static bMatrix2x4 operator<=(hMatrix2x4 lhs, hMatrix2x4 rhs) => new bMatrix2x4(lhs.m00 <= rhs.m00, lhs.m01 <= rhs.m01, lhs.m02 <= rhs.m02, lhs.m03 <= rhs.m03, lhs.m10 <= rhs.m10, lhs.m11 <= rhs.m11, lhs.m12 <= rhs.m12, lhs.m13 <= rhs.m13);
        
        /// <summary>
        /// Executes a component-wise lesser-or-equal comparison with a scalar.
        /// </summary>
        public static bMatrix2x4 operator<=(hMatrix2x4 lhs, Half rhs) => new bMatrix2x4(lhs.m00 <= rhs, lhs.m01 <= rhs, lhs.m02 <= rhs, lhs.m03 <= rhs, lhs.m10 <= rhs, lhs.m11 <= rhs, lhs.m12 <= rhs, lhs.m13 <= rhs);
        
        /// <summary>
        /// Executes a component-wise lesser-or-equal comparison with a scalar.
        /// </summary>
        public static bMatrix2x4 operator<=(Half lhs, hMatrix2x4 rhs) => new bMatrix2x4(lhs <= rhs.m00, lhs <= rhs.m01, lhs <= rhs.m02, lhs <= rhs.m03, lhs <= rhs.m10, lhs <= rhs.m11, lhs <= rhs.m12, lhs <= rhs.m13);
        
        /// <summary>
        /// Executes a component-wise greater-than comparison.
        /// </summary>
        public static bMatrix2x4 operator>(hMatrix2x4 lhs, hMatrix2x4 rhs) => new bMatrix2x4(lhs.m00 > rhs.m00, lhs.m01 > rhs.m01, lhs.m02 > rhs.m02, lhs.m03 > rhs.m03, lhs.m10 > rhs.m10, lhs.m11 > rhs.m11, lhs.m12 > rhs.m12, lhs.m13 > rhs.m13);
        
        /// <summary>
        /// Executes a component-wise greater-than comparison with a scalar.
        /// </summary>
        public static bMatrix2x4 operator>(hMatrix2x4 lhs, Half rhs) => new bMatrix2x4(lhs.m00 > rhs, lhs.m01 > rhs, lhs.m02 > rhs, lhs.m03 > rhs, lhs.m10 > rhs, lhs.m11 > rhs, lhs.m12 > rhs, lhs.m13 > rhs);
        
        /// <summary>
        /// Executes a component-wise greater-than comparison with a scalar.
        /// </summary>
        public static bMatrix2x4 operator>(Half lhs, hMatrix2x4 rhs) => new bMatrix2x4(lhs > rhs.m00, lhs > rhs.m01, lhs > rhs.m02, lhs > rhs.m03, lhs > rhs.m10, lhs > rhs.m11, lhs > rhs.m12, lhs > rhs.m13);
        
        /// <summary>
        /// Executes a component-wise greater-or-equal comparison.
        /// </summary>
        public static bMatrix2x4 operator>=(hMatrix2x4 lhs, hMatrix2x4 rhs) => new bMatrix2x4(lhs.m00 >= rhs.m00, lhs.m01 >= rhs.m01, lhs.m02 >= rhs.m02, lhs.m03 >= rhs.m03, lhs.m10 >= rhs.m10, lhs.m11 >= rhs.m11, lhs.m12 >= rhs.m12, lhs.m13 >= rhs.m13);
        
        /// <summary>
        /// Executes a component-wise greater-or-equal comparison with a scalar.
        /// </summary>
        public static bMatrix2x4 operator>=(hMatrix2x4 lhs, Half rhs) => new bMatrix2x4(lhs.m00 >= rhs, lhs.m01 >= rhs, lhs.m02 >= rhs, lhs.m03 >= rhs, lhs.m10 >= rhs, lhs.m11 >= rhs, lhs.m12 >= rhs, lhs.m13 >= rhs);
        
        /// <summary>
        /// Executes a component-wise greater-or-equal comparison with a scalar.
        /// </summary>
        public static bMatrix2x4 operator>=(Half lhs, hMatrix2x4 rhs) => new bMatrix2x4(lhs >= rhs.m00, lhs >= rhs.m01, lhs >= rhs.m02, lhs >= rhs.m03, lhs >= rhs.m10, lhs >= rhs.m11, lhs >= rhs.m12, lhs >= rhs.m13);
    }
}
