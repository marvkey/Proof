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
    /// A Matrix of type Complex with 2 columns and 3 rows.
    /// </summary>
    /*
    [RegisterCoreClassStruct]
    
    [StructLayout(LayoutKind.Sequential)]
    public struct cMatrix2x3 : IReadOnlyList<Complex>, IEquatable<cMatrix2x3>
    {

        #region Fields
        
        /// <summary>
        /// Column 0, Rows 0
        /// </summary>
        
        public Complex m00;
        
        /// <summary>
        /// Column 0, Rows 1
        /// </summary>
        
        public Complex m01;
        
        /// <summary>
        /// Column 0, Rows 2
        /// </summary>
        
        public Complex m02;
        
        /// <summary>
        /// Column 1, Rows 0
        /// </summary>
        
        public Complex m10;
        
        /// <summary>
        /// Column 1, Rows 1
        /// </summary>
        
        public Complex m11;
        
        /// <summary>
        /// Column 1, Rows 2
        /// </summary>
        
        public Complex m12;

        #endregion


        #region Constructors
        
        /// <summary>
        /// Component-wise constructor
        /// </summary>
        public cMatrix2x3(Complex m00, Complex m01, Complex m02, Complex m10, Complex m11, Complex m12)
        {
            this.m00 = m00;
            this.m01 = m01;
            this.m02 = m02;
            this.m10 = m10;
            this.m11 = m11;
            this.m12 = m12;
        }
        
        /// <summary>
        /// Constructs this Matrix from a cMatrix2. Non-overwritten fields are from an Identity Matrix.
        /// </summary>
        public cMatrix2x3(cMatrix2 m)
        {
            this.m00 = m.m00;
            this.m01 = m.m01;
            this.m02 = Complex.Zero;
            this.m10 = m.m10;
            this.m11 = m.m11;
            this.m12 = Complex.Zero;
        }
        
        /// <summary>
        /// Constructs this Matrix from a cMatrix3x2. Non-overwritten fields are from an Identity Matrix.
        /// </summary>
        public cMatrix2x3(cMatrix3x2 m)
        {
            this.m00 = m.m00;
            this.m01 = m.m01;
            this.m02 = Complex.Zero;
            this.m10 = m.m10;
            this.m11 = m.m11;
            this.m12 = Complex.Zero;
        }
        
        /// <summary>
        /// Constructs this Matrix from a cMatrix4x2. Non-overwritten fields are from an Identity Matrix.
        /// </summary>
        public cMatrix2x3(cMatrix4x2 m)
        {
            this.m00 = m.m00;
            this.m01 = m.m01;
            this.m02 = Complex.Zero;
            this.m10 = m.m10;
            this.m11 = m.m11;
            this.m12 = Complex.Zero;
        }
        
        /// <summary>
        /// Constructs this Matrix from a cMatrix2x3. Non-overwritten fields are from an Identity Matrix.
        /// </summary>
        public cMatrix2x3(cMatrix2x3 m)
        {
            this.m00 = m.m00;
            this.m01 = m.m01;
            this.m02 = m.m02;
            this.m10 = m.m10;
            this.m11 = m.m11;
            this.m12 = m.m12;
        }
        
        /// <summary>
        /// Constructs this Matrix from a cMatrix3. Non-overwritten fields are from an Identity Matrix.
        /// </summary>
        public cMatrix2x3(cMatrix3 m)
        {
            this.m00 = m.m00;
            this.m01 = m.m01;
            this.m02 = m.m02;
            this.m10 = m.m10;
            this.m11 = m.m11;
            this.m12 = m.m12;
        }
        
        /// <summary>
        /// Constructs this Matrix from a cMatrix4x3. Non-overwritten fields are from an Identity Matrix.
        /// </summary>
        public cMatrix2x3(cMatrix4x3 m)
        {
            this.m00 = m.m00;
            this.m01 = m.m01;
            this.m02 = m.m02;
            this.m10 = m.m10;
            this.m11 = m.m11;
            this.m12 = m.m12;
        }
        
        /// <summary>
        /// Constructs this Matrix from a cMatrix2x4. Non-overwritten fields are from an Identity Matrix.
        /// </summary>
        public cMatrix2x3(cMatrix2x4 m)
        {
            this.m00 = m.m00;
            this.m01 = m.m01;
            this.m02 = m.m02;
            this.m10 = m.m10;
            this.m11 = m.m11;
            this.m12 = m.m12;
        }
        
        /// <summary>
        /// Constructs this Matrix from a cMatrix3x4. Non-overwritten fields are from an Identity Matrix.
        /// </summary>
        public cMatrix2x3(cMatrix3x4 m)
        {
            this.m00 = m.m00;
            this.m01 = m.m01;
            this.m02 = m.m02;
            this.m10 = m.m10;
            this.m11 = m.m11;
            this.m12 = m.m12;
        }
        
        /// <summary>
        /// Constructs this Matrix from a cMatrix4. Non-overwritten fields are from an Identity Matrix.
        /// </summary>
        public cMatrix2x3(cMatrix4 m)
        {
            this.m00 = m.m00;
            this.m01 = m.m01;
            this.m02 = m.m02;
            this.m10 = m.m10;
            this.m11 = m.m11;
            this.m12 = m.m12;
        }
        
        /// <summary>
        /// Constructs this Matrix from a series of column Vectors. Non-overwritten fields are from an Identity Matrix.
        /// </summary>
        public cMatrix2x3(cVector2 c0, cVector2 c1)
        {
            this.m00 = c0.x;
            this.m01 = c0.y;
            this.m02 = Complex.Zero;
            this.m10 = c1.x;
            this.m11 = c1.y;
            this.m12 = Complex.Zero;
        }
        
        /// <summary>
        /// Constructs this Matrix from a series of column Vectors. Non-overwritten fields are from an Identity Matrix.
        /// </summary>
        public cMatrix2x3(cVector3 c0, cVector3 c1)
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
        public Complex[,] Values => new[,] { { m00, m01, m02 }, { m10, m11, m12 } };
        
        /// <summary>
        /// Creates a 1D array with all values (internal order)
        /// </summary>
        public Complex[] Values1D => new[] { m00, m01, m02, m10, m11, m12 };
        
        /// <summary>
        /// Gets or sets the column nr 0
        /// </summary>
        public cVector3 Column0
        {
            get
            {
                return new cVector3(m00, m01, m02);
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
        public cVector3 Column1
        {
            get
            {
                return new cVector3(m10, m11, m12);
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
        public cVector2 Row0
        {
            get
            {
                return new cVector2(m00, m10);
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
        public cVector2 Row1
        {
            get
            {
                return new cVector2(m01, m11);
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
        public cVector2 Row2
        {
            get
            {
                return new cVector2(m02, m12);
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
        public static cMatrix2x3 Zero { get; } = new cMatrix2x3(Complex.Zero, Complex.Zero, Complex.Zero, Complex.Zero, Complex.Zero, Complex.Zero);
        
        /// <summary>
        /// Predefined all-ones Matrix
        /// </summary>
        public static cMatrix2x3 Ones { get; } = new cMatrix2x3(Complex.One, Complex.One, Complex.One, Complex.One, Complex.One, Complex.One);
        
        /// <summary>
        /// Predefined identity Matrix
        /// </summary>
        public static cMatrix2x3 Identity { get; } = new cMatrix2x3(Complex.One, Complex.Zero, Complex.Zero, Complex.Zero, Complex.One, Complex.Zero);
        
        /// <summary>
        /// Predefined all-imaginary-ones Matrix
        /// </summary>
        public static cMatrix2x3 ImaginaryOnes { get; } = new cMatrix2x3(Complex.ImaginaryOne, Complex.ImaginaryOne, Complex.ImaginaryOne, Complex.ImaginaryOne, Complex.ImaginaryOne, Complex.ImaginaryOne);
        
        /// <summary>
        /// Predefined diagonal-imaginary-one Matrix
        /// </summary>
        public static cMatrix2x3 ImaginaryIdentity { get; } = new cMatrix2x3(Complex.ImaginaryOne, Complex.Zero, Complex.Zero, Complex.Zero, Complex.ImaginaryOne, Complex.Zero);

        #endregion


        #region Functions
        
        /// <summary>
        /// Returns an enumerator that iterates through all fields.
        /// </summary>
        public IEnumerator<Complex> GetEnumerator()
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
        public Complex this[int fieldIndex]
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
        public Complex this[int col, int row]
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
        public bool Equals(cMatrix2x3 rhs) => (((m00.Equals(rhs.m00) && m01.Equals(rhs.m01)) && m02.Equals(rhs.m02)) && ((m10.Equals(rhs.m10) && m11.Equals(rhs.m11)) && m12.Equals(rhs.m12)));
        
        /// <summary>
        /// Returns true iff this equals rhs type- and component-wise.
        /// </summary>
        public override bool Equals(object obj)
        {
            if (ReferenceEquals(null, obj)) return false;
            return obj is cMatrix2x3 && Equals((cMatrix2x3) obj);
        }
        
        /// <summary>
        /// Returns true iff this equals rhs component-wise.
        /// </summary>
        public static bool operator ==(cMatrix2x3 lhs, cMatrix2x3 rhs) => lhs.Equals(rhs);
        
        /// <summary>
        /// Returns true iff this does not equal rhs (component-wise).
        /// </summary>
        public static bool operator !=(cMatrix2x3 lhs, cMatrix2x3 rhs) => !lhs.Equals(rhs);
        
        /// <summary>
        /// Returns a hash code for this instance.
        /// </summary>
        public override int GetHashCode()
        {
            unchecked
            {
                return ((((((((((m00.GetHashCode()) * 397) ^ m01.GetHashCode()) * 397) ^ m02.GetHashCode()) * 397) ^ m10.GetHashCode()) * 397) ^ m11.GetHashCode()) * 397) ^ m12.GetHashCode();
            }
        }
        
        /// <summary>
        /// Returns a transposed version of this Matrix.
        /// </summary>
        public cMatrix3x2 Transposed => new cMatrix3x2(m00, m10, m01, m11, m02, m12);
        
        /// <summary>
        /// Returns the euclidean length of this Matrix.
        /// </summary>
        public double Length => (double)Math.Sqrt((((m00.LengthSqr() + m01.LengthSqr()) + m02.LengthSqr()) + ((m10.LengthSqr() + m11.LengthSqr()) + m12.LengthSqr())));
        
        /// <summary>
        /// Returns the squared euclidean length of this Matrix.
        /// </summary>
        public double LengthSqr => (((m00.LengthSqr() + m01.LengthSqr()) + m02.LengthSqr()) + ((m10.LengthSqr() + m11.LengthSqr()) + m12.LengthSqr()));
        
        /// <summary>
        /// Returns the sum of all fields.
        /// </summary>
        public Complex Sum => (((m00 + m01) + m02) + ((m10 + m11) + m12));
        
        /// <summary>
        /// Returns the euclidean norm of this Matrix.
        /// </summary>
        public double Norm => (double)Math.Sqrt((((m00.LengthSqr() + m01.LengthSqr()) + m02.LengthSqr()) + ((m10.LengthSqr() + m11.LengthSqr()) + m12.LengthSqr())));
        
        /// <summary>
        /// Returns the one-norm of this Matrix.
        /// </summary>
        public double Norm1 => (((m00.Magnitude + m01.Magnitude) + m02.Magnitude) + ((m10.Magnitude + m11.Magnitude) + m12.Magnitude));
        
        /// <summary>
        /// Returns the two-norm of this Matrix.
        /// </summary>
        public double Norm2 => (double)Math.Sqrt((((m00.LengthSqr() + m01.LengthSqr()) + m02.LengthSqr()) + ((m10.LengthSqr() + m11.LengthSqr()) + m12.LengthSqr())));
        
        /// <summary>
        /// Returns the max-norm of this Matrix.
        /// </summary>
        public Complex NormMax => Math.Max(Math.Max(Math.Max(Math.Max(Math.Max(m00.Magnitude, m01.Magnitude), m02.Magnitude), m10.Magnitude), m11.Magnitude), m12.Magnitude);
        
        /// <summary>
        /// Returns the p-norm of this Matrix.
        /// </summary>
        public double NormP(double p) => Math.Pow((((Math.Pow((double)m00.Magnitude, p) + Math.Pow((double)m01.Magnitude, p)) + Math.Pow((double)m02.Magnitude, p)) + ((Math.Pow((double)m10.Magnitude, p) + Math.Pow((double)m11.Magnitude, p)) + Math.Pow((double)m12.Magnitude, p))), 1 / p);
        
        /// <summary>
        /// Executes a Matrix-Matrix-multiplication cMatrix2x3 * cMatrix2 -> cMatrix2x3.
        /// </summary>
        public static cMatrix2x3 operator*(cMatrix2x3 lhs, cMatrix2 rhs) => new cMatrix2x3((lhs.m00 * rhs.m00 + lhs.m10 * rhs.m01), (lhs.m01 * rhs.m00 + lhs.m11 * rhs.m01), (lhs.m02 * rhs.m00 + lhs.m12 * rhs.m01), (lhs.m00 * rhs.m10 + lhs.m10 * rhs.m11), (lhs.m01 * rhs.m10 + lhs.m11 * rhs.m11), (lhs.m02 * rhs.m10 + lhs.m12 * rhs.m11));
        
        /// <summary>
        /// Executes a Matrix-Matrix-multiplication cMatrix2x3 * cMatrix3x2 -> cMatrix3.
        /// </summary>
        public static cMatrix3 operator*(cMatrix2x3 lhs, cMatrix3x2 rhs) => new cMatrix3((lhs.m00 * rhs.m00 + lhs.m10 * rhs.m01), (lhs.m01 * rhs.m00 + lhs.m11 * rhs.m01), (lhs.m02 * rhs.m00 + lhs.m12 * rhs.m01), (lhs.m00 * rhs.m10 + lhs.m10 * rhs.m11), (lhs.m01 * rhs.m10 + lhs.m11 * rhs.m11), (lhs.m02 * rhs.m10 + lhs.m12 * rhs.m11), (lhs.m00 * rhs.m20 + lhs.m10 * rhs.m21), (lhs.m01 * rhs.m20 + lhs.m11 * rhs.m21), (lhs.m02 * rhs.m20 + lhs.m12 * rhs.m21));
        
        /// <summary>
        /// Executes a Matrix-Matrix-multiplication cMatrix2x3 * cMatrix4x2 -> cMatrix4x3.
        /// </summary>
        public static cMatrix4x3 operator*(cMatrix2x3 lhs, cMatrix4x2 rhs) => new cMatrix4x3((lhs.m00 * rhs.m00 + lhs.m10 * rhs.m01), (lhs.m01 * rhs.m00 + lhs.m11 * rhs.m01), (lhs.m02 * rhs.m00 + lhs.m12 * rhs.m01), (lhs.m00 * rhs.m10 + lhs.m10 * rhs.m11), (lhs.m01 * rhs.m10 + lhs.m11 * rhs.m11), (lhs.m02 * rhs.m10 + lhs.m12 * rhs.m11), (lhs.m00 * rhs.m20 + lhs.m10 * rhs.m21), (lhs.m01 * rhs.m20 + lhs.m11 * rhs.m21), (lhs.m02 * rhs.m20 + lhs.m12 * rhs.m21), (lhs.m00 * rhs.m30 + lhs.m10 * rhs.m31), (lhs.m01 * rhs.m30 + lhs.m11 * rhs.m31), (lhs.m02 * rhs.m30 + lhs.m12 * rhs.m31));
        
        /// <summary>
        /// Executes a Matrix-Vector-multiplication.
        /// </summary>
        public static cVector3 operator*(cMatrix2x3 m, cVector2 v) => new cVector3((m.m00 * v.x + m.m10 * v.y), (m.m01 * v.x + m.m11 * v.y), (m.m02 * v.x + m.m12 * v.y));
        
        /// <summary>
        /// Executes a component-wise * (multiply).
        /// </summary>
        public static cMatrix2x3 CompMul(cMatrix2x3 A, cMatrix2x3 B) => new cMatrix2x3(A.m00 * B.m00, A.m01 * B.m01, A.m02 * B.m02, A.m10 * B.m10, A.m11 * B.m11, A.m12 * B.m12);
        
        /// <summary>
        /// Executes a component-wise / (divide).
        /// </summary>
        public static cMatrix2x3 CompDiv(cMatrix2x3 A, cMatrix2x3 B) => new cMatrix2x3(A.m00 / B.m00, A.m01 / B.m01, A.m02 / B.m02, A.m10 / B.m10, A.m11 / B.m11, A.m12 / B.m12);
        
        /// <summary>
        /// Executes a component-wise + (add).
        /// </summary>
        public static cMatrix2x3 CompAdd(cMatrix2x3 A, cMatrix2x3 B) => new cMatrix2x3(A.m00 + B.m00, A.m01 + B.m01, A.m02 + B.m02, A.m10 + B.m10, A.m11 + B.m11, A.m12 + B.m12);
        
        /// <summary>
        /// Executes a component-wise - (subtract).
        /// </summary>
        public static cMatrix2x3 CompSub(cMatrix2x3 A, cMatrix2x3 B) => new cMatrix2x3(A.m00 - B.m00, A.m01 - B.m01, A.m02 - B.m02, A.m10 - B.m10, A.m11 - B.m11, A.m12 - B.m12);
        
        /// <summary>
        /// Executes a component-wise + (add).
        /// </summary>
        public static cMatrix2x3 operator+(cMatrix2x3 lhs, cMatrix2x3 rhs) => new cMatrix2x3(lhs.m00 + rhs.m00, lhs.m01 + rhs.m01, lhs.m02 + rhs.m02, lhs.m10 + rhs.m10, lhs.m11 + rhs.m11, lhs.m12 + rhs.m12);
        
        /// <summary>
        /// Executes a component-wise + (add) with a scalar.
        /// </summary>
        public static cMatrix2x3 operator+(cMatrix2x3 lhs, Complex rhs) => new cMatrix2x3(lhs.m00 + rhs, lhs.m01 + rhs, lhs.m02 + rhs, lhs.m10 + rhs, lhs.m11 + rhs, lhs.m12 + rhs);
        
        /// <summary>
        /// Executes a component-wise + (add) with a scalar.
        /// </summary>
        public static cMatrix2x3 operator+(Complex lhs, cMatrix2x3 rhs) => new cMatrix2x3(lhs + rhs.m00, lhs + rhs.m01, lhs + rhs.m02, lhs + rhs.m10, lhs + rhs.m11, lhs + rhs.m12);
        
        /// <summary>
        /// Executes a component-wise - (subtract).
        /// </summary>
        public static cMatrix2x3 operator-(cMatrix2x3 lhs, cMatrix2x3 rhs) => new cMatrix2x3(lhs.m00 - rhs.m00, lhs.m01 - rhs.m01, lhs.m02 - rhs.m02, lhs.m10 - rhs.m10, lhs.m11 - rhs.m11, lhs.m12 - rhs.m12);
        
        /// <summary>
        /// Executes a component-wise - (subtract) with a scalar.
        /// </summary>
        public static cMatrix2x3 operator-(cMatrix2x3 lhs, Complex rhs) => new cMatrix2x3(lhs.m00 - rhs, lhs.m01 - rhs, lhs.m02 - rhs, lhs.m10 - rhs, lhs.m11 - rhs, lhs.m12 - rhs);
        
        /// <summary>
        /// Executes a component-wise - (subtract) with a scalar.
        /// </summary>
        public static cMatrix2x3 operator-(Complex lhs, cMatrix2x3 rhs) => new cMatrix2x3(lhs - rhs.m00, lhs - rhs.m01, lhs - rhs.m02, lhs - rhs.m10, lhs - rhs.m11, lhs - rhs.m12);
        
        /// <summary>
        /// Executes a component-wise / (divide) with a scalar.
        /// </summary>
        public static cMatrix2x3 operator/(cMatrix2x3 lhs, Complex rhs) => new cMatrix2x3(lhs.m00 / rhs, lhs.m01 / rhs, lhs.m02 / rhs, lhs.m10 / rhs, lhs.m11 / rhs, lhs.m12 / rhs);
        
        /// <summary>
        /// Executes a component-wise / (divide) with a scalar.
        /// </summary>
        public static cMatrix2x3 operator/(Complex lhs, cMatrix2x3 rhs) => new cMatrix2x3(lhs / rhs.m00, lhs / rhs.m01, lhs / rhs.m02, lhs / rhs.m10, lhs / rhs.m11, lhs / rhs.m12);
        
        /// <summary>
        /// Executes a component-wise * (multiply) with a scalar.
        /// </summary>
        public static cMatrix2x3 operator*(cMatrix2x3 lhs, Complex rhs) => new cMatrix2x3(lhs.m00 * rhs, lhs.m01 * rhs, lhs.m02 * rhs, lhs.m10 * rhs, lhs.m11 * rhs, lhs.m12 * rhs);
        
        /// <summary>
        /// Executes a component-wise * (multiply) with a scalar.
        /// </summary>
        public static cMatrix2x3 operator*(Complex lhs, cMatrix2x3 rhs) => new cMatrix2x3(lhs * rhs.m00, lhs * rhs.m01, lhs * rhs.m02, lhs * rhs.m10, lhs * rhs.m11, lhs * rhs.m12);
    }
    */
}
