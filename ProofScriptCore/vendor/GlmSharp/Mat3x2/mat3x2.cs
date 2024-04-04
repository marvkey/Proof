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
    /// A Matrix of type float with 3 columns and 2 rows.
    /// </summary>
    [RegisterCoreClassStruct]
    
    [StructLayout(LayoutKind.Sequential)]
    public struct Matrix3x2 : IReadOnlyList<float>, IEquatable<Matrix3x2>
    {

        #region Fields
        
        /// <summary>
        /// Column 0, Rows 0
        /// </summary>
        
        public float m00;
        
        /// <summary>
        /// Column 0, Rows 1
        /// </summary>
        
        public float m01;
        
        /// <summary>
        /// Column 1, Rows 0
        /// </summary>
        
        public float m10;
        
        /// <summary>
        /// Column 1, Rows 1
        /// </summary>
        
        public float m11;
        
        /// <summary>
        /// Column 2, Rows 0
        /// </summary>
        
        public float m20;
        
        /// <summary>
        /// Column 2, Rows 1
        /// </summary>
        
        public float m21;

        #endregion


        #region Constructors
        
        /// <summary>
        /// Component-wise constructor
        /// </summary>
        public Matrix3x2(float m00, float m01, float m10, float m11, float m20, float m21)
        {
            this.m00 = m00;
            this.m01 = m01;
            this.m10 = m10;
            this.m11 = m11;
            this.m20 = m20;
            this.m21 = m21;
        }
        
        /// <summary>
        /// Constructs this Matrix from a Matrix2. Non-overwritten fields are from an Identity Matrix.
        /// </summary>
        public Matrix3x2(Matrix2 m)
        {
            this.m00 = m.m00;
            this.m01 = m.m01;
            this.m10 = m.m10;
            this.m11 = m.m11;
            this.m20 = 0f;
            this.m21 = 0f;
        }
        
        /// <summary>
        /// Constructs this Matrix from a Matrix3x2. Non-overwritten fields are from an Identity Matrix.
        /// </summary>
        public Matrix3x2(Matrix3x2 m)
        {
            this.m00 = m.m00;
            this.m01 = m.m01;
            this.m10 = m.m10;
            this.m11 = m.m11;
            this.m20 = m.m20;
            this.m21 = m.m21;
        }
        
        /// <summary>
        /// Constructs this Matrix from a Matrix4x2. Non-overwritten fields are from an Identity Matrix.
        /// </summary>
        public Matrix3x2(Matrix4x2 m)
        {
            this.m00 = m.m00;
            this.m01 = m.m01;
            this.m10 = m.m10;
            this.m11 = m.m11;
            this.m20 = m.m20;
            this.m21 = m.m21;
        }
        
        /// <summary>
        /// Constructs this Matrix from a Matrix2x3. Non-overwritten fields are from an Identity Matrix.
        /// </summary>
        public Matrix3x2(Matrix2x3 m)
        {
            this.m00 = m.m00;
            this.m01 = m.m01;
            this.m10 = m.m10;
            this.m11 = m.m11;
            this.m20 = 0f;
            this.m21 = 0f;
        }
        
        /// <summary>
        /// Constructs this Matrix from a Matrix3. Non-overwritten fields are from an Identity Matrix.
        /// </summary>
        public Matrix3x2(Matrix3 m)
        {
            this.m00 = m.m00;
            this.m01 = m.m01;
            this.m10 = m.m10;
            this.m11 = m.m11;
            this.m20 = m.m20;
            this.m21 = m.m21;
        }
        
        /// <summary>
        /// Constructs this Matrix from a Matrix4x3. Non-overwritten fields are from an Identity Matrix.
        /// </summary>
        public Matrix3x2(Matrix4x3 m)
        {
            this.m00 = m.m00;
            this.m01 = m.m01;
            this.m10 = m.m10;
            this.m11 = m.m11;
            this.m20 = m.m20;
            this.m21 = m.m21;
        }
        
        /// <summary>
        /// Constructs this Matrix from a Matrix2x4. Non-overwritten fields are from an Identity Matrix.
        /// </summary>
        public Matrix3x2(Matrix2x4 m)
        {
            this.m00 = m.m00;
            this.m01 = m.m01;
            this.m10 = m.m10;
            this.m11 = m.m11;
            this.m20 = 0f;
            this.m21 = 0f;
        }
        
        /// <summary>
        /// Constructs this Matrix from a Matrix3x4. Non-overwritten fields are from an Identity Matrix.
        /// </summary>
        public Matrix3x2(Matrix3x4 m)
        {
            this.m00 = m.m00;
            this.m01 = m.m01;
            this.m10 = m.m10;
            this.m11 = m.m11;
            this.m20 = m.m20;
            this.m21 = m.m21;
        }
        
        /// <summary>
        /// Constructs this Matrix from a Matrix4. Non-overwritten fields are from an Identity Matrix.
        /// </summary>
        public Matrix3x2(Matrix4 m)
        {
            this.m00 = m.m00;
            this.m01 = m.m01;
            this.m10 = m.m10;
            this.m11 = m.m11;
            this.m20 = m.m20;
            this.m21 = m.m21;
        }
        
        /// <summary>
        /// Constructs this Matrix from a series of column Vectortors. Non-overwritten fields are from an Identity Matrix.
        /// </summary>
        public Matrix3x2(Vector2 c0, Vector2 c1)
        {
            this.m00 = c0.x;
            this.m01 = c0.y;
            this.m10 = c1.x;
            this.m11 = c1.y;
            this.m20 = 0f;
            this.m21 = 0f;
        }
        
        /// <summary>
        /// Constructs this Matrix from a series of column Vectortors. Non-overwritten fields are from an Identity Matrix.
        /// </summary>
        public Matrix3x2(Vector2 c0, Vector2 c1, Vector2 c2)
        {
            this.m00 = c0.x;
            this.m01 = c0.y;
            this.m10 = c1.x;
            this.m11 = c1.y;
            this.m20 = c2.x;
            this.m21 = c2.y;
        }

        #endregion


        #region Properties
        
        /// <summary>
        /// Creates a 2D array with all values (address: Values[x, y])
        /// </summary>
        public float[,] Values => new[,] { { m00, m01 }, { m10, m11 }, { m20, m21 } };
        
        /// <summary>
        /// Creates a 1D array with all values (internal order)
        /// </summary>
        public float[] Values1D => new[] { m00, m01, m10, m11, m20, m21 };
        
        /// <summary>
        /// Gets or sets the column nr 0
        /// </summary>
        public Vector2 Column0
        {
            get
            {
                return new Vector2(m00, m01);
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
        public Vector2 Column1
        {
            get
            {
                return new Vector2(m10, m11);
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
        public Vector2 Column2
        {
            get
            {
                return new Vector2(m20, m21);
            }
            set
            {
                m20 = value.x;
                m21 = value.y;
            }
        }
        
        /// <summary>
        /// Gets or sets the row nr 0
        /// </summary>
        public Vector3 Row0
        {
            get
            {
                return new Vector3(m00, m10, m20);
            }
            set
            {
                m00 = value.x;
                m10 = value.y;
                m20 = value.z;
            }
        }
        
        /// <summary>
        /// Gets or sets the row nr 1
        /// </summary>
        public Vector3 Row1
        {
            get
            {
                return new Vector3(m01, m11, m21);
            }
            set
            {
                m01 = value.x;
                m11 = value.y;
                m21 = value.z;
            }
        }

        #endregion


        #region Static Properties
        
        /// <summary>
        /// Predefined all-zero Matrix
        /// </summary>
        public static Matrix3x2 Zero { get; } = new Matrix3x2(0f, 0f, 0f, 0f, 0f, 0f);
        
        /// <summary>
        /// Predefined all-ones Matrix
        /// </summary>
        public static Matrix3x2 Ones { get; } = new Matrix3x2(1f, 1f, 1f, 1f, 1f, 1f);
        
        /// <summary>
        /// Predefined identity Matrix
        /// </summary>
        public static Matrix3x2 Identity { get; } = new Matrix3x2(1f, 0f, 0f, 1f, 0f, 0f);
        
        /// <summary>
        /// Predefined all-MaxValue Matrix
        /// </summary>
        public static Matrix3x2 AllMaxValue { get; } = new Matrix3x2(float.MaxValue, float.MaxValue, float.MaxValue, float.MaxValue, float.MaxValue, float.MaxValue);
        
        /// <summary>
        /// Predefined diagonal-MaxValue Matrix
        /// </summary>
        public static Matrix3x2 DiagonalMaxValue { get; } = new Matrix3x2(float.MaxValue, 0f, 0f, float.MaxValue, 0f, 0f);
        
        /// <summary>
        /// Predefined all-MinValue Matrix
        /// </summary>
        public static Matrix3x2 AllMinValue { get; } = new Matrix3x2(float.MinValue, float.MinValue, float.MinValue, float.MinValue, float.MinValue, float.MinValue);
        
        /// <summary>
        /// Predefined diagonal-MinValue Matrix
        /// </summary>
        public static Matrix3x2 DiagonalMinValue { get; } = new Matrix3x2(float.MinValue, 0f, 0f, float.MinValue, 0f, 0f);
        
        /// <summary>
        /// Predefined all-Epsilon Matrix
        /// </summary>
        public static Matrix3x2 AllEpsilon { get; } = new Matrix3x2(float.Epsilon, float.Epsilon, float.Epsilon, float.Epsilon, float.Epsilon, float.Epsilon);
        
        /// <summary>
        /// Predefined diagonal-Epsilon Matrix
        /// </summary>
        public static Matrix3x2 DiagonalEpsilon { get; } = new Matrix3x2(float.Epsilon, 0f, 0f, float.Epsilon, 0f, 0f);
        
        /// <summary>
        /// Predefined all-NaN Matrix
        /// </summary>
        public static Matrix3x2 AllNaN { get; } = new Matrix3x2(float.NaN, float.NaN, float.NaN, float.NaN, float.NaN, float.NaN);
        
        /// <summary>
        /// Predefined diagonal-NaN Matrix
        /// </summary>
        public static Matrix3x2 DiagonalNaN { get; } = new Matrix3x2(float.NaN, 0f, 0f, float.NaN, 0f, 0f);
        
        /// <summary>
        /// Predefined all-NegativeInfinity Matrix
        /// </summary>
        public static Matrix3x2 AllNegativeInfinity { get; } = new Matrix3x2(float.NegativeInfinity, float.NegativeInfinity, float.NegativeInfinity, float.NegativeInfinity, float.NegativeInfinity, float.NegativeInfinity);
        
        /// <summary>
        /// Predefined diagonal-NegativeInfinity Matrix
        /// </summary>
        public static Matrix3x2 DiagonalNegativeInfinity { get; } = new Matrix3x2(float.NegativeInfinity, 0f, 0f, float.NegativeInfinity, 0f, 0f);
        
        /// <summary>
        /// Predefined all-PositiveInfinity Matrix
        /// </summary>
        public static Matrix3x2 AllPositiveInfinity { get; } = new Matrix3x2(float.PositiveInfinity, float.PositiveInfinity, float.PositiveInfinity, float.PositiveInfinity, float.PositiveInfinity, float.PositiveInfinity);
        
        /// <summary>
        /// Predefined diagonal-PositiveInfinity Matrix
        /// </summary>
        public static Matrix3x2 DiagonalPositiveInfinity { get; } = new Matrix3x2(float.PositiveInfinity, 0f, 0f, float.PositiveInfinity, 0f, 0f);

        #endregion


        #region Functions
        
        /// <summary>
        /// Returns an enumerator that iterates through all fields.
        /// </summary>
        public IEnumerator<float> GetEnumerator()
        {
            yield return m00;
            yield return m01;
            yield return m10;
            yield return m11;
            yield return m20;
            yield return m21;
        }
        
        /// <summary>
        /// Returns an enumerator that iterates through all fields.
        /// </summary>
        IEnumerator IEnumerable.GetEnumerator() => GetEnumerator();

        #endregion

        
        /// <summary>
        /// Returns the number of Fields (3 x 2 = 6).
        /// </summary>
        public int Count => 6;
        
        /// <summary>
        /// Gets/Sets a specific indexed component (a bit slower than direct access).
        /// </summary>
        public float this[int fieldIndex]
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
                    default: throw new ArgumentOutOfRangeException("fieldIndex");
                }
            }
        }
        
        /// <summary>
        /// Gets/Sets a specific 2D-indexed component (a bit slower than direct access).
        /// </summary>
        public float this[int col, int row]
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
        public bool Equals(Matrix3x2 rhs) => (((m00.Equals(rhs.m00) && m01.Equals(rhs.m01)) && m10.Equals(rhs.m10)) && ((m11.Equals(rhs.m11) && m20.Equals(rhs.m20)) && m21.Equals(rhs.m21)));
        
        /// <summary>
        /// Returns true iff this equals rhs type- and component-wise.
        /// </summary>
        public override bool Equals(object obj)
        {
            if (ReferenceEquals(null, obj)) return false;
            return obj is Matrix3x2 && Equals((Matrix3x2) obj);
        }
        
        /// <summary>
        /// Returns true iff this equals rhs component-wise.
        /// </summary>
        public static bool operator ==(Matrix3x2 lhs, Matrix3x2 rhs) => lhs.Equals(rhs);
        
        /// <summary>
        /// Returns true iff this does not equal rhs (component-wise).
        /// </summary>
        public static bool operator !=(Matrix3x2 lhs, Matrix3x2 rhs) => !lhs.Equals(rhs);
        
        /// <summary>
        /// Returns a hash code for this instance.
        /// </summary>
        public override int GetHashCode()
        {
            unchecked
            {
                return ((((((((((m00.GetHashCode()) * 397) ^ m01.GetHashCode()) * 397) ^ m10.GetHashCode()) * 397) ^ m11.GetHashCode()) * 397) ^ m20.GetHashCode()) * 397) ^ m21.GetHashCode();
            }
        }
        
        /// <summary>
        /// Returns a transposed version of this Matrix.
        /// </summary>
        public Matrix2x3 Transposed => new Matrix2x3(m00, m10, m20, m01, m11, m21);
        
        /// <summary>
        /// Returns the minimal component of this Matrix.
        /// </summary>
        public float MinElement => Math.Min(Math.Min(Math.Min(Math.Min(Math.Min(m00, m01), m10), m11), m20), m21);
        
        /// <summary>
        /// Returns the maximal component of this Matrix.
        /// </summary>
        public float MaxElement => Math.Max(Math.Max(Math.Max(Math.Max(Math.Max(m00, m01), m10), m11), m20), m21);
        
        /// <summary>
        /// Returns the euclidean length of this Matrix.
        /// </summary>
        public float Length => (float)Math.Sqrt((((m00*m00 + m01*m01) + m10*m10) + ((m11*m11 + m20*m20) + m21*m21)));
        
        /// <summary>
        /// Returns the squared euclidean length of this Matrix.
        /// </summary>
        public float LengthSqr => (((m00*m00 + m01*m01) + m10*m10) + ((m11*m11 + m20*m20) + m21*m21));
        
        /// <summary>
        /// Returns the sum of all fields.
        /// </summary>
        public float Sum => (((m00 + m01) + m10) + ((m11 + m20) + m21));
        
        /// <summary>
        /// Returns the euclidean norm of this Matrix.
        /// </summary>
        public float Norm => (float)Math.Sqrt((((m00*m00 + m01*m01) + m10*m10) + ((m11*m11 + m20*m20) + m21*m21)));
        
        /// <summary>
        /// Returns the one-norm of this Matrix.
        /// </summary>
        public float Norm1 => (((Math.Abs(m00) + Math.Abs(m01)) + Math.Abs(m10)) + ((Math.Abs(m11) + Math.Abs(m20)) + Math.Abs(m21)));
        
        /// <summary>
        /// Returns the two-norm of this Matrix.
        /// </summary>
        public float Norm2 => (float)Math.Sqrt((((m00*m00 + m01*m01) + m10*m10) + ((m11*m11 + m20*m20) + m21*m21)));
        
        /// <summary>
        /// Returns the max-norm of this Matrix.
        /// </summary>
        public float NormMax => Math.Max(Math.Max(Math.Max(Math.Max(Math.Max(Math.Abs(m00), Math.Abs(m01)), Math.Abs(m10)), Math.Abs(m11)), Math.Abs(m20)), Math.Abs(m21));
        
        /// <summary>
        /// Returns the p-norm of this Matrix.
        /// </summary>
        public double NormP(double p) => Math.Pow((((Math.Pow((double)Math.Abs(m00), p) + Math.Pow((double)Math.Abs(m01), p)) + Math.Pow((double)Math.Abs(m10), p)) + ((Math.Pow((double)Math.Abs(m11), p) + Math.Pow((double)Math.Abs(m20), p)) + Math.Pow((double)Math.Abs(m21), p))), 1 / p);
        
        /// <summary>
        /// Executes a Matrix-Matrix-multiplication Matrix3x2 * Matrix2x3 -> Matrix2.
        /// </summary>
        public static Matrix2 operator*(Matrix3x2 lhs, Matrix2x3 rhs) => new Matrix2(((lhs.m00 * rhs.m00 + lhs.m10 * rhs.m01) + lhs.m20 * rhs.m02), ((lhs.m01 * rhs.m00 + lhs.m11 * rhs.m01) + lhs.m21 * rhs.m02), ((lhs.m00 * rhs.m10 + lhs.m10 * rhs.m11) + lhs.m20 * rhs.m12), ((lhs.m01 * rhs.m10 + lhs.m11 * rhs.m11) + lhs.m21 * rhs.m12));
        
        /// <summary>
        /// Executes a Matrix-Matrix-multiplication Matrix3x2 * Matrix3 -> Matrix3x2.
        /// </summary>
        public static Matrix3x2 operator*(Matrix3x2 lhs, Matrix3 rhs) => new Matrix3x2(((lhs.m00 * rhs.m00 + lhs.m10 * rhs.m01) + lhs.m20 * rhs.m02), ((lhs.m01 * rhs.m00 + lhs.m11 * rhs.m01) + lhs.m21 * rhs.m02), ((lhs.m00 * rhs.m10 + lhs.m10 * rhs.m11) + lhs.m20 * rhs.m12), ((lhs.m01 * rhs.m10 + lhs.m11 * rhs.m11) + lhs.m21 * rhs.m12), ((lhs.m00 * rhs.m20 + lhs.m10 * rhs.m21) + lhs.m20 * rhs.m22), ((lhs.m01 * rhs.m20 + lhs.m11 * rhs.m21) + lhs.m21 * rhs.m22));
        
        /// <summary>
        /// Executes a Matrix-Matrix-multiplication Matrix3x2 * Matrix4x3 -> Matrix4x2.
        /// </summary>
        public static Matrix4x2 operator*(Matrix3x2 lhs, Matrix4x3 rhs) => new Matrix4x2(((lhs.m00 * rhs.m00 + lhs.m10 * rhs.m01) + lhs.m20 * rhs.m02), ((lhs.m01 * rhs.m00 + lhs.m11 * rhs.m01) + lhs.m21 * rhs.m02), ((lhs.m00 * rhs.m10 + lhs.m10 * rhs.m11) + lhs.m20 * rhs.m12), ((lhs.m01 * rhs.m10 + lhs.m11 * rhs.m11) + lhs.m21 * rhs.m12), ((lhs.m00 * rhs.m20 + lhs.m10 * rhs.m21) + lhs.m20 * rhs.m22), ((lhs.m01 * rhs.m20 + lhs.m11 * rhs.m21) + lhs.m21 * rhs.m22), ((lhs.m00 * rhs.m30 + lhs.m10 * rhs.m31) + lhs.m20 * rhs.m32), ((lhs.m01 * rhs.m30 + lhs.m11 * rhs.m31) + lhs.m21 * rhs.m32));
        
        /// <summary>
        /// Executes a Matrix-Vectortor-multiplication.
        /// </summary>
        public static Vector2 operator*(Matrix3x2 m, Vector3 v) => new Vector2(((m.m00 * v.x + m.m10 * v.y) + m.m20 * v.z), ((m.m01 * v.x + m.m11 * v.y) + m.m21 * v.z));
        
        /// <summary>
        /// Executes a component-wise * (multiply).
        /// </summary>
        public static Matrix3x2 CompMul(Matrix3x2 A, Matrix3x2 B) => new Matrix3x2(A.m00 * B.m00, A.m01 * B.m01, A.m10 * B.m10, A.m11 * B.m11, A.m20 * B.m20, A.m21 * B.m21);
        
        /// <summary>
        /// Executes a component-wise / (divide).
        /// </summary>
        public static Matrix3x2 CompDiv(Matrix3x2 A, Matrix3x2 B) => new Matrix3x2(A.m00 / B.m00, A.m01 / B.m01, A.m10 / B.m10, A.m11 / B.m11, A.m20 / B.m20, A.m21 / B.m21);
        
        /// <summary>
        /// Executes a component-wise + (add).
        /// </summary>
        public static Matrix3x2 CompAdd(Matrix3x2 A, Matrix3x2 B) => new Matrix3x2(A.m00 + B.m00, A.m01 + B.m01, A.m10 + B.m10, A.m11 + B.m11, A.m20 + B.m20, A.m21 + B.m21);
        
        /// <summary>
        /// Executes a component-wise - (subtract).
        /// </summary>
        public static Matrix3x2 CompSub(Matrix3x2 A, Matrix3x2 B) => new Matrix3x2(A.m00 - B.m00, A.m01 - B.m01, A.m10 - B.m10, A.m11 - B.m11, A.m20 - B.m20, A.m21 - B.m21);
        
        /// <summary>
        /// Executes a component-wise + (add).
        /// </summary>
        public static Matrix3x2 operator+(Matrix3x2 lhs, Matrix3x2 rhs) => new Matrix3x2(lhs.m00 + rhs.m00, lhs.m01 + rhs.m01, lhs.m10 + rhs.m10, lhs.m11 + rhs.m11, lhs.m20 + rhs.m20, lhs.m21 + rhs.m21);
        
        /// <summary>
        /// Executes a component-wise + (add) with a scalar.
        /// </summary>
        public static Matrix3x2 operator+(Matrix3x2 lhs, float rhs) => new Matrix3x2(lhs.m00 + rhs, lhs.m01 + rhs, lhs.m10 + rhs, lhs.m11 + rhs, lhs.m20 + rhs, lhs.m21 + rhs);
        
        /// <summary>
        /// Executes a component-wise + (add) with a scalar.
        /// </summary>
        public static Matrix3x2 operator+(float lhs, Matrix3x2 rhs) => new Matrix3x2(lhs + rhs.m00, lhs + rhs.m01, lhs + rhs.m10, lhs + rhs.m11, lhs + rhs.m20, lhs + rhs.m21);
        
        /// <summary>
        /// Executes a component-wise - (subtract).
        /// </summary>
        public static Matrix3x2 operator-(Matrix3x2 lhs, Matrix3x2 rhs) => new Matrix3x2(lhs.m00 - rhs.m00, lhs.m01 - rhs.m01, lhs.m10 - rhs.m10, lhs.m11 - rhs.m11, lhs.m20 - rhs.m20, lhs.m21 - rhs.m21);
        
        /// <summary>
        /// Executes a component-wise - (subtract) with a scalar.
        /// </summary>
        public static Matrix3x2 operator-(Matrix3x2 lhs, float rhs) => new Matrix3x2(lhs.m00 - rhs, lhs.m01 - rhs, lhs.m10 - rhs, lhs.m11 - rhs, lhs.m20 - rhs, lhs.m21 - rhs);
        
        /// <summary>
        /// Executes a component-wise - (subtract) with a scalar.
        /// </summary>
        public static Matrix3x2 operator-(float lhs, Matrix3x2 rhs) => new Matrix3x2(lhs - rhs.m00, lhs - rhs.m01, lhs - rhs.m10, lhs - rhs.m11, lhs - rhs.m20, lhs - rhs.m21);
        
        /// <summary>
        /// Executes a component-wise / (divide) with a scalar.
        /// </summary>
        public static Matrix3x2 operator/(Matrix3x2 lhs, float rhs) => new Matrix3x2(lhs.m00 / rhs, lhs.m01 / rhs, lhs.m10 / rhs, lhs.m11 / rhs, lhs.m20 / rhs, lhs.m21 / rhs);
        
        /// <summary>
        /// Executes a component-wise / (divide) with a scalar.
        /// </summary>
        public static Matrix3x2 operator/(float lhs, Matrix3x2 rhs) => new Matrix3x2(lhs / rhs.m00, lhs / rhs.m01, lhs / rhs.m10, lhs / rhs.m11, lhs / rhs.m20, lhs / rhs.m21);
        
        /// <summary>
        /// Executes a component-wise * (multiply) with a scalar.
        /// </summary>
        public static Matrix3x2 operator*(Matrix3x2 lhs, float rhs) => new Matrix3x2(lhs.m00 * rhs, lhs.m01 * rhs, lhs.m10 * rhs, lhs.m11 * rhs, lhs.m20 * rhs, lhs.m21 * rhs);
        
        /// <summary>
        /// Executes a component-wise * (multiply) with a scalar.
        /// </summary>
        public static Matrix3x2 operator*(float lhs, Matrix3x2 rhs) => new Matrix3x2(lhs * rhs.m00, lhs * rhs.m01, lhs * rhs.m10, lhs * rhs.m11, lhs * rhs.m20, lhs * rhs.m21);
        
        /// <summary>
        /// Executes a component-wise lesser-than comparison.
        /// </summary>
        public static bMatrix3x2 operator<(Matrix3x2 lhs, Matrix3x2 rhs) => new bMatrix3x2(lhs.m00 < rhs.m00, lhs.m01 < rhs.m01, lhs.m10 < rhs.m10, lhs.m11 < rhs.m11, lhs.m20 < rhs.m20, lhs.m21 < rhs.m21);
        
        /// <summary>
        /// Executes a component-wise lesser-than comparison with a scalar.
        /// </summary>
        public static bMatrix3x2 operator<(Matrix3x2 lhs, float rhs) => new bMatrix3x2(lhs.m00 < rhs, lhs.m01 < rhs, lhs.m10 < rhs, lhs.m11 < rhs, lhs.m20 < rhs, lhs.m21 < rhs);
        
        /// <summary>
        /// Executes a component-wise lesser-than comparison with a scalar.
        /// </summary>
        public static bMatrix3x2 operator<(float lhs, Matrix3x2 rhs) => new bMatrix3x2(lhs < rhs.m00, lhs < rhs.m01, lhs < rhs.m10, lhs < rhs.m11, lhs < rhs.m20, lhs < rhs.m21);
        
        /// <summary>
        /// Executes a component-wise lesser-or-equal comparison.
        /// </summary>
        public static bMatrix3x2 operator<=(Matrix3x2 lhs, Matrix3x2 rhs) => new bMatrix3x2(lhs.m00 <= rhs.m00, lhs.m01 <= rhs.m01, lhs.m10 <= rhs.m10, lhs.m11 <= rhs.m11, lhs.m20 <= rhs.m20, lhs.m21 <= rhs.m21);
        
        /// <summary>
        /// Executes a component-wise lesser-or-equal comparison with a scalar.
        /// </summary>
        public static bMatrix3x2 operator<=(Matrix3x2 lhs, float rhs) => new bMatrix3x2(lhs.m00 <= rhs, lhs.m01 <= rhs, lhs.m10 <= rhs, lhs.m11 <= rhs, lhs.m20 <= rhs, lhs.m21 <= rhs);
        
        /// <summary>
        /// Executes a component-wise lesser-or-equal comparison with a scalar.
        /// </summary>
        public static bMatrix3x2 operator<=(float lhs, Matrix3x2 rhs) => new bMatrix3x2(lhs <= rhs.m00, lhs <= rhs.m01, lhs <= rhs.m10, lhs <= rhs.m11, lhs <= rhs.m20, lhs <= rhs.m21);
        
        /// <summary>
        /// Executes a component-wise greater-than comparison.
        /// </summary>
        public static bMatrix3x2 operator>(Matrix3x2 lhs, Matrix3x2 rhs) => new bMatrix3x2(lhs.m00 > rhs.m00, lhs.m01 > rhs.m01, lhs.m10 > rhs.m10, lhs.m11 > rhs.m11, lhs.m20 > rhs.m20, lhs.m21 > rhs.m21);
        
        /// <summary>
        /// Executes a component-wise greater-than comparison with a scalar.
        /// </summary>
        public static bMatrix3x2 operator>(Matrix3x2 lhs, float rhs) => new bMatrix3x2(lhs.m00 > rhs, lhs.m01 > rhs, lhs.m10 > rhs, lhs.m11 > rhs, lhs.m20 > rhs, lhs.m21 > rhs);
        
        /// <summary>
        /// Executes a component-wise greater-than comparison with a scalar.
        /// </summary>
        public static bMatrix3x2 operator>(float lhs, Matrix3x2 rhs) => new bMatrix3x2(lhs > rhs.m00, lhs > rhs.m01, lhs > rhs.m10, lhs > rhs.m11, lhs > rhs.m20, lhs > rhs.m21);
        
        /// <summary>
        /// Executes a component-wise greater-or-equal comparison.
        /// </summary>
        public static bMatrix3x2 operator>=(Matrix3x2 lhs, Matrix3x2 rhs) => new bMatrix3x2(lhs.m00 >= rhs.m00, lhs.m01 >= rhs.m01, lhs.m10 >= rhs.m10, lhs.m11 >= rhs.m11, lhs.m20 >= rhs.m20, lhs.m21 >= rhs.m21);
        
        /// <summary>
        /// Executes a component-wise greater-or-equal comparison with a scalar.
        /// </summary>
        public static bMatrix3x2 operator>=(Matrix3x2 lhs, float rhs) => new bMatrix3x2(lhs.m00 >= rhs, lhs.m01 >= rhs, lhs.m10 >= rhs, lhs.m11 >= rhs, lhs.m20 >= rhs, lhs.m21 >= rhs);
        
        /// <summary>
        /// Executes a component-wise greater-or-equal comparison with a scalar.
        /// </summary>
        public static bMatrix3x2 operator>=(float lhs, Matrix3x2 rhs) => new bMatrix3x2(lhs >= rhs.m00, lhs >= rhs.m01, lhs >= rhs.m10, lhs >= rhs.m11, lhs >= rhs.m20, lhs >= rhs.m21);
    }
}
