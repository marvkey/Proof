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
    /// A Matrix of type Complex with 3 columns and 2 rows.
    /// </summary>
    /// 
    /*
    [RegisterCoreClassStruct]
    
    [StructLayout(LayoutKind.Sequential)]
    public struct cMatrix3x2 : IReadOnlyList<Complex>, IEquatable<cMatrix3x2>
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
        /// Column 1, Rows 0
        /// </summary>
        
        public Complex m10;
        
        /// <summary>
        /// Column 1, Rows 1
        /// </summary>
        
        public Complex m11;
        
        /// <summary>
        /// Column 2, Rows 0
        /// </summary>
        
        public Complex m20;
        
        /// <summary>
        /// Column 2, Rows 1
        /// </summary>
        
        public Complex m21;

        #endregion


        #region Constructors
        
        /// <summary>
        /// Component-wise constructor
        /// </summary>
        public cMatrix3x2(Complex m00, Complex m01, Complex m10, Complex m11, Complex m20, Complex m21)
        {
            this.m00 = m00;
            this.m01 = m01;
            this.m10 = m10;
            this.m11 = m11;
            this.m20 = m20;
            this.m21 = m21;
        }
        
        /// <summary>
        /// Constructs this Matrix from a cMatrix2. Non-overwritten fields are from an Identity Matrix.
        /// </summary>
        public cMatrix3x2(cMatrix2 m)
        {
            this.m00 = m.m00;
            this.m01 = m.m01;
            this.m10 = m.m10;
            this.m11 = m.m11;
            this.m20 = Complex.Zero;
            this.m21 = Complex.Zero;
        }
        
        /// <summary>
        /// Constructs this Matrix from a cMatrix3x2. Non-overwritten fields are from an Identity Matrix.
        /// </summary>
        public cMatrix3x2(cMatrix3x2 m)
        {
            this.m00 = m.m00;
            this.m01 = m.m01;
            this.m10 = m.m10;
            this.m11 = m.m11;
            this.m20 = m.m20;
            this.m21 = m.m21;
        }
        
        /// <summary>
        /// Constructs this Matrix from a cMatrix4x2. Non-overwritten fields are from an Identity Matrix.
        /// </summary>
        public cMatrix3x2(cMatrix4x2 m)
        {
            this.m00 = m.m00;
            this.m01 = m.m01;
            this.m10 = m.m10;
            this.m11 = m.m11;
            this.m20 = m.m20;
            this.m21 = m.m21;
        }
        
        /// <summary>
        /// Constructs this Matrix from a cMatrix2x3. Non-overwritten fields are from an Identity Matrix.
        /// </summary>
        public cMatrix3x2(cMatrix2x3 m)
        {
            this.m00 = m.m00;
            this.m01 = m.m01;
            this.m10 = m.m10;
            this.m11 = m.m11;
            this.m20 = Complex.Zero;
            this.m21 = Complex.Zero;
        }
        
        /// <summary>
        /// Constructs this Matrix from a cMatrix3. Non-overwritten fields are from an Identity Matrix.
        /// </summary>
        public cMatrix3x2(cMatrix3 m)
        {
            this.m00 = m.m00;
            this.m01 = m.m01;
            this.m10 = m.m10;
            this.m11 = m.m11;
            this.m20 = m.m20;
            this.m21 = m.m21;
        }
        
        /// <summary>
        /// Constructs this Matrix from a cMatrix4x3. Non-overwritten fields are from an Identity Matrix.
        /// </summary>
        public cMatrix3x2(cMatrix4x3 m)
        {
            this.m00 = m.m00;
            this.m01 = m.m01;
            this.m10 = m.m10;
            this.m11 = m.m11;
            this.m20 = m.m20;
            this.m21 = m.m21;
        }
        
        /// <summary>
        /// Constructs this Matrix from a cMatrix2x4. Non-overwritten fields are from an Identity Matrix.
        /// </summary>
        public cMatrix3x2(cMatrix2x4 m)
        {
            this.m00 = m.m00;
            this.m01 = m.m01;
            this.m10 = m.m10;
            this.m11 = m.m11;
            this.m20 = Complex.Zero;
            this.m21 = Complex.Zero;
        }
        
        /// <summary>
        /// Constructs this Matrix from a cMatrix3x4. Non-overwritten fields are from an Identity Matrix.
        /// </summary>
        public cMatrix3x2(cMatrix3x4 m)
        {
            this.m00 = m.m00;
            this.m01 = m.m01;
            this.m10 = m.m10;
            this.m11 = m.m11;
            this.m20 = m.m20;
            this.m21 = m.m21;
        }
        
        /// <summary>
        /// Constructs this Matrix from a cMatrix4. Non-overwritten fields are from an Identity Matrix.
        /// </summary>
        public cMatrix3x2(cMatrix4 m)
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
        public cMatrix3x2(cVector2 c0, cVector2 c1)
        {
            this.m00 = c0.x;
            this.m01 = c0.y;
            this.m10 = c1.x;
            this.m11 = c1.y;
            this.m20 = Complex.Zero;
            this.m21 = Complex.Zero;
        }
        
        /// <summary>
        /// Constructs this Matrix from a series of column Vectortors. Non-overwritten fields are from an Identity Matrix.
        /// </summary>
        public cMatrix3x2(cVector2 c0, cVector2 c1, cVector2 c2)
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
        public Complex[,] Values => new[,] { { m00, m01 }, { m10, m11 }, { m20, m21 } };
        
        /// <summary>
        /// Creates a 1D array with all values (internal order)
        /// </summary>
        public Complex[] Values1D => new[] { m00, m01, m10, m11, m20, m21 };
        
        /// <summary>
        /// Gets or sets the column nr 0
        /// </summary>
        public cVector2 Column0
        {
            get
            {
                return new cVector2(m00, m01);
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
        public cVector2 Column1
        {
            get
            {
                return new cVector2(m10, m11);
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
        public cVector2 Column2
        {
            get
            {
                return new cVector2(m20, m21);
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
        public cVector3 Row0
        {
            get
            {
                return new cVector3(m00, m10, m20);
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
        public cVector3 Row1
        {
            get
            {
                return new cVector3(m01, m11, m21);
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
        public static cMatrix3x2 Zero { get; } = new cMatrix3x2(Complex.Zero, Complex.Zero, Complex.Zero, Complex.Zero, Complex.Zero, Complex.Zero);
        
        /// <summary>
        /// Predefined all-ones Matrix
        /// </summary>
        public static cMatrix3x2 Ones { get; } = new cMatrix3x2(Complex.One, Complex.One, Complex.One, Complex.One, Complex.One, Complex.One);
        
        /// <summary>
        /// Predefined identity Matrix
        /// </summary>
        public static cMatrix3x2 Identity { get; } = new cMatrix3x2(Complex.One, Complex.Zero, Complex.Zero, Complex.One, Complex.Zero, Complex.Zero);
        
        /// <summary>
        /// Predefined all-imaginary-ones Matrix
        /// </summary>
        public static cMatrix3x2 ImaginaryOnes { get; } = new cMatrix3x2(Complex.ImaginaryOne, Complex.ImaginaryOne, Complex.ImaginaryOne, Complex.ImaginaryOne, Complex.ImaginaryOne, Complex.ImaginaryOne);
        
        /// <summary>
        /// Predefined diagonal-imaginary-one Matrix
        /// </summary>
        public static cMatrix3x2 ImaginaryIdentity { get; } = new cMatrix3x2(Complex.ImaginaryOne, Complex.Zero, Complex.Zero, Complex.ImaginaryOne, Complex.Zero, Complex.Zero);

        #endregion


        #region Functions
        
        /// <summary>
        /// Returns an enumerator that iterates through all fields.
        /// </summary>
        public IEnumerator<Complex> GetEnumerator()
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
        public Complex this[int fieldIndex]
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
        public Complex this[int col, int row]
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
        public bool Equals(cMatrix3x2 rhs) => (((m00.Equals(rhs.m00) && m01.Equals(rhs.m01)) && m10.Equals(rhs.m10)) && ((m11.Equals(rhs.m11) && m20.Equals(rhs.m20)) && m21.Equals(rhs.m21)));
        
        /// <summary>
        /// Returns true iff this equals rhs type- and component-wise.
        /// </summary>
        public override bool Equals(object obj)
        {
            if (ReferenceEquals(null, obj)) return false;
            return obj is cMatrix3x2 && Equals((cMatrix3x2) obj);
        }
        
        /// <summary>
        /// Returns true iff this equals rhs component-wise.
        /// </summary>
        public static bool operator ==(cMatrix3x2 lhs, cMatrix3x2 rhs) => lhs.Equals(rhs);
        
        /// <summary>
        /// Returns true iff this does not equal rhs (component-wise).
        /// </summary>
        public static bool operator !=(cMatrix3x2 lhs, cMatrix3x2 rhs) => !lhs.Equals(rhs);
        
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
        public cMatrix2x3 Transposed => new cMatrix2x3(m00, m10, m20, m01, m11, m21);
        
        /// <summary>
        /// Returns the euclidean length of this Matrix.
        /// </summary>
        public double Length => (double)Math.Sqrt((((m00.LengthSqr() + m01.LengthSqr()) + m10.LengthSqr()) + ((m11.LengthSqr() + m20.LengthSqr()) + m21.LengthSqr())));
        
        /// <summary>
        /// Returns the squared euclidean length of this Matrix.
        /// </summary>
        public double LengthSqr => (((m00.LengthSqr() + m01.LengthSqr()) + m10.LengthSqr()) + ((m11.LengthSqr() + m20.LengthSqr()) + m21.LengthSqr()));
        
        /// <summary>
        /// Returns the sum of all fields.
        /// </summary>
        public Complex Sum => (((m00 + m01) + m10) + ((m11 + m20) + m21));
        
        /// <summary>
        /// Returns the euclidean norm of this Matrix.
        /// </summary>
        public double Norm => (double)Math.Sqrt((((m00.LengthSqr() + m01.LengthSqr()) + m10.LengthSqr()) + ((m11.LengthSqr() + m20.LengthSqr()) + m21.LengthSqr())));
        
        /// <summary>
        /// Returns the one-norm of this Matrix.
        /// </summary>
        public double Norm1 => (((m00.Magnitude + m01.Magnitude) + m10.Magnitude) + ((m11.Magnitude + m20.Magnitude) + m21.Magnitude));
        
        /// <summary>
        /// Returns the two-norm of this Matrix.
        /// </summary>
        public double Norm2 => (double)Math.Sqrt((((m00.LengthSqr() + m01.LengthSqr()) + m10.LengthSqr()) + ((m11.LengthSqr() + m20.LengthSqr()) + m21.LengthSqr())));
        
        /// <summary>
        /// Returns the max-norm of this Matrix.
        /// </summary>
        public Complex NormMax => Math.Max(Math.Max(Math.Max(Math.Max(Math.Max(m00.Magnitude, m01.Magnitude), m10.Magnitude), m11.Magnitude), m20.Magnitude), m21.Magnitude);
        
        /// <summary>
        /// Returns the p-norm of this Matrix.
        /// </summary>
        public double NormP(double p) => Math.Pow((((Math.Pow((double)m00.Magnitude, p) + Math.Pow((double)m01.Magnitude, p)) + Math.Pow((double)m10.Magnitude, p)) + ((Math.Pow((double)m11.Magnitude, p) + Math.Pow((double)m20.Magnitude, p)) + Math.Pow((double)m21.Magnitude, p))), 1 / p);
        
        /// <summary>
        /// Executes a Matrix-Matrix-multiplication cMatrix3x2 * cMatrix2x3 -> cMatrix2.
        /// </summary>
        public static cMatrix2 operator*(cMatrix3x2 lhs, cMatrix2x3 rhs) => new cMatrix2(((lhs.m00 * rhs.m00 + lhs.m10 * rhs.m01) + lhs.m20 * rhs.m02), ((lhs.m01 * rhs.m00 + lhs.m11 * rhs.m01) + lhs.m21 * rhs.m02), ((lhs.m00 * rhs.m10 + lhs.m10 * rhs.m11) + lhs.m20 * rhs.m12), ((lhs.m01 * rhs.m10 + lhs.m11 * rhs.m11) + lhs.m21 * rhs.m12));
        
        /// <summary>
        /// Executes a Matrix-Matrix-multiplication cMatrix3x2 * cMatrix3 -> cMatrix3x2.
        /// </summary>
        public static cMatrix3x2 operator*(cMatrix3x2 lhs, cMatrix3 rhs) => new cMatrix3x2(((lhs.m00 * rhs.m00 + lhs.m10 * rhs.m01) + lhs.m20 * rhs.m02), ((lhs.m01 * rhs.m00 + lhs.m11 * rhs.m01) + lhs.m21 * rhs.m02), ((lhs.m00 * rhs.m10 + lhs.m10 * rhs.m11) + lhs.m20 * rhs.m12), ((lhs.m01 * rhs.m10 + lhs.m11 * rhs.m11) + lhs.m21 * rhs.m12), ((lhs.m00 * rhs.m20 + lhs.m10 * rhs.m21) + lhs.m20 * rhs.m22), ((lhs.m01 * rhs.m20 + lhs.m11 * rhs.m21) + lhs.m21 * rhs.m22));
        
        /// <summary>
        /// Executes a Matrix-Matrix-multiplication cMatrix3x2 * cMatrix4x3 -> cMatrix4x2.
        /// </summary>
        public static cMatrix4x2 operator*(cMatrix3x2 lhs, cMatrix4x3 rhs) => new cMatrix4x2(((lhs.m00 * rhs.m00 + lhs.m10 * rhs.m01) + lhs.m20 * rhs.m02), ((lhs.m01 * rhs.m00 + lhs.m11 * rhs.m01) + lhs.m21 * rhs.m02), ((lhs.m00 * rhs.m10 + lhs.m10 * rhs.m11) + lhs.m20 * rhs.m12), ((lhs.m01 * rhs.m10 + lhs.m11 * rhs.m11) + lhs.m21 * rhs.m12), ((lhs.m00 * rhs.m20 + lhs.m10 * rhs.m21) + lhs.m20 * rhs.m22), ((lhs.m01 * rhs.m20 + lhs.m11 * rhs.m21) + lhs.m21 * rhs.m22), ((lhs.m00 * rhs.m30 + lhs.m10 * rhs.m31) + lhs.m20 * rhs.m32), ((lhs.m01 * rhs.m30 + lhs.m11 * rhs.m31) + lhs.m21 * rhs.m32));
        
        /// <summary>
        /// Executes a Matrix-Vectortor-multiplication.
        /// </summary>
        public static cVector2 operator*(cMatrix3x2 m, cVector3 v) => new cVector2(((m.m00 * v.x + m.m10 * v.y) + m.m20 * v.z), ((m.m01 * v.x + m.m11 * v.y) + m.m21 * v.z));
        
        /// <summary>
        /// Executes a component-wise * (multiply).
        /// </summary>
        public static cMatrix3x2 CompMul(cMatrix3x2 A, cMatrix3x2 B) => new cMatrix3x2(A.m00 * B.m00, A.m01 * B.m01, A.m10 * B.m10, A.m11 * B.m11, A.m20 * B.m20, A.m21 * B.m21);
        
        /// <summary>
        /// Executes a component-wise / (divide).
        /// </summary>
        public static cMatrix3x2 CompDiv(cMatrix3x2 A, cMatrix3x2 B) => new cMatrix3x2(A.m00 / B.m00, A.m01 / B.m01, A.m10 / B.m10, A.m11 / B.m11, A.m20 / B.m20, A.m21 / B.m21);
        
        /// <summary>
        /// Executes a component-wise + (add).
        /// </summary>
        public static cMatrix3x2 CompAdd(cMatrix3x2 A, cMatrix3x2 B) => new cMatrix3x2(A.m00 + B.m00, A.m01 + B.m01, A.m10 + B.m10, A.m11 + B.m11, A.m20 + B.m20, A.m21 + B.m21);
        
        /// <summary>
        /// Executes a component-wise - (subtract).
        /// </summary>
        public static cMatrix3x2 CompSub(cMatrix3x2 A, cMatrix3x2 B) => new cMatrix3x2(A.m00 - B.m00, A.m01 - B.m01, A.m10 - B.m10, A.m11 - B.m11, A.m20 - B.m20, A.m21 - B.m21);
        
        /// <summary>
        /// Executes a component-wise + (add).
        /// </summary>
        public static cMatrix3x2 operator+(cMatrix3x2 lhs, cMatrix3x2 rhs) => new cMatrix3x2(lhs.m00 + rhs.m00, lhs.m01 + rhs.m01, lhs.m10 + rhs.m10, lhs.m11 + rhs.m11, lhs.m20 + rhs.m20, lhs.m21 + rhs.m21);
        
        /// <summary>
        /// Executes a component-wise + (add) with a scalar.
        /// </summary>
        public static cMatrix3x2 operator+(cMatrix3x2 lhs, Complex rhs) => new cMatrix3x2(lhs.m00 + rhs, lhs.m01 + rhs, lhs.m10 + rhs, lhs.m11 + rhs, lhs.m20 + rhs, lhs.m21 + rhs);
        
        /// <summary>
        /// Executes a component-wise + (add) with a scalar.
        /// </summary>
        public static cMatrix3x2 operator+(Complex lhs, cMatrix3x2 rhs) => new cMatrix3x2(lhs + rhs.m00, lhs + rhs.m01, lhs + rhs.m10, lhs + rhs.m11, lhs + rhs.m20, lhs + rhs.m21);
        
        /// <summary>
        /// Executes a component-wise - (subtract).
        /// </summary>
        public static cMatrix3x2 operator-(cMatrix3x2 lhs, cMatrix3x2 rhs) => new cMatrix3x2(lhs.m00 - rhs.m00, lhs.m01 - rhs.m01, lhs.m10 - rhs.m10, lhs.m11 - rhs.m11, lhs.m20 - rhs.m20, lhs.m21 - rhs.m21);
        
        /// <summary>
        /// Executes a component-wise - (subtract) with a scalar.
        /// </summary>
        public static cMatrix3x2 operator-(cMatrix3x2 lhs, Complex rhs) => new cMatrix3x2(lhs.m00 - rhs, lhs.m01 - rhs, lhs.m10 - rhs, lhs.m11 - rhs, lhs.m20 - rhs, lhs.m21 - rhs);
        
        /// <summary>
        /// Executes a component-wise - (subtract) with a scalar.
        /// </summary>
        public static cMatrix3x2 operator-(Complex lhs, cMatrix3x2 rhs) => new cMatrix3x2(lhs - rhs.m00, lhs - rhs.m01, lhs - rhs.m10, lhs - rhs.m11, lhs - rhs.m20, lhs - rhs.m21);
        
        /// <summary>
        /// Executes a component-wise / (divide) with a scalar.
        /// </summary>
        public static cMatrix3x2 operator/(cMatrix3x2 lhs, Complex rhs) => new cMatrix3x2(lhs.m00 / rhs, lhs.m01 / rhs, lhs.m10 / rhs, lhs.m11 / rhs, lhs.m20 / rhs, lhs.m21 / rhs);
        
        /// <summary>
        /// Executes a component-wise / (divide) with a scalar.
        /// </summary>
        public static cMatrix3x2 operator/(Complex lhs, cMatrix3x2 rhs) => new cMatrix3x2(lhs / rhs.m00, lhs / rhs.m01, lhs / rhs.m10, lhs / rhs.m11, lhs / rhs.m20, lhs / rhs.m21);
        
        /// <summary>
        /// Executes a component-wise * (multiply) with a scalar.
        /// </summary>
        public static cMatrix3x2 operator*(cMatrix3x2 lhs, Complex rhs) => new cMatrix3x2(lhs.m00 * rhs, lhs.m01 * rhs, lhs.m10 * rhs, lhs.m11 * rhs, lhs.m20 * rhs, lhs.m21 * rhs);
        
        /// <summary>
        /// Executes a component-wise * (multiply) with a scalar.
        /// </summary>
        public static cMatrix3x2 operator*(Complex lhs, cMatrix3x2 rhs) => new cMatrix3x2(lhs * rhs.m00, lhs * rhs.m01, lhs * rhs.m10, lhs * rhs.m11, lhs * rhs.m20, lhs * rhs.m21);
    }
    */
}
