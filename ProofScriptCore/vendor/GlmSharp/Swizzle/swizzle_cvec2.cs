using System;
using System.Collections;
using System.Collections.Generic;
using System.Globalization;
using System.Runtime.InteropServices;

using System.Numerics;
using System.Linq;
using Proof.Swizzle;

// ReSharper disable InconsistentNaming

namespace Proof.Swizzle
{
    
    /// <summary>
    /// Temporary Vector of type Complex with 2 components, used for implementing swizzling for cVector2.
    /// </summary>
    /// 
    /*
    
   
    [StructLayout(LayoutKind.Sequential)]
    public struct swizzle_cVector2
    {

        #region Fields
        
        /// <summary>
        /// x-component
        /// </summary>
        
        //internal readonly Complex x;
        
        /// <summary>
        /// y-component
        /// </summary>
        
        //internal readonly Complex y;

        #endregion


        #region Constructors
        
        /// <summary>
        /// Constructor for swizzle_cVector2.
        /// </summary>
        //internal swizzle_cVector2(Complex x, Complex y)
        //{
        //    this.x = x;
        //    this.y = y;
        //}

        #endregion


        #region Properties
        
        /// <summary>
        /// Returns cVector2.xx swizzling.
        /// </summary>
        public cVector2 xx => new cVector2(x, x);
        
        /// <summary>
        /// Returns cVector2.rr swizzling (equivalent to cVector2.xx).
        /// </summary>
        public cVector2 rr => new cVector2(x, x);
        
        /// <summary>
        /// Returns cVector2.xxx swizzling.
        /// </summary>
        public cVector3 xxx => new cVector3(x, x, x);
        
        /// <summary>
        /// Returns cVector2.rrr swizzling (equivalent to cVector2.xxx).
        /// </summary>
        public cVector3 rrr => new cVector3(x, x, x);
        
        /// <summary>
        /// Returns cVector2.xxxx swizzling.
        /// </summary>
        public cVector4 xxxx => new cVector4(x, x, x, x);
        
        /// <summary>
        /// Returns cVector2.rrrr swizzling (equivalent to cVector2.xxxx).
        /// </summary>
        public cVector4 rrrr => new cVector4(x, x, x, x);
        
        /// <summary>
        /// Returns cVector2.xxxy swizzling.
        /// </summary>
        public cVector4 xxxy => new cVector4(x, x, x, y);
        
        /// <summary>
        /// Returns cVector2.rrrg swizzling (equivalent to cVector2.xxxy).
        /// </summary>
        public cVector4 rrrg => new cVector4(x, x, x, y);
        
        /// <summary>
        /// Returns cVector2.xxy swizzling.
        /// </summary>
        public cVector3 xxy => new cVector3(x, x, y);
        
        /// <summary>
        /// Returns cVector2.rrg swizzling (equivalent to cVector2.xxy).
        /// </summary>
        public cVector3 rrg => new cVector3(x, x, y);
        
        /// <summary>
        /// Returns cVector2.xxyx swizzling.
        /// </summary>
        public cVector4 xxyx => new cVector4(x, x, y, x);
        
        /// <summary>
        /// Returns cVector2.rrgr swizzling (equivalent to cVector2.xxyx).
        /// </summary>
        public cVector4 rrgr => new cVector4(x, x, y, x);
        
        /// <summary>
        /// Returns cVector2.xxyy swizzling.
        /// </summary>
        public cVector4 xxyy => new cVector4(x, x, y, y);
        
        /// <summary>
        /// Returns cVector2.rrgg swizzling (equivalent to cVector2.xxyy).
        /// </summary>
        public cVector4 rrgg => new cVector4(x, x, y, y);
        
        /// <summary>
        /// Returns cVector2.xy swizzling.
        /// </summary>
        public cVector2 xy => new cVector2(x, y);
        
        /// <summary>
        /// Returns cVector2.rg swizzling (equivalent to cVector2.xy).
        /// </summary>
        public cVector2 rg => new cVector2(x, y);
        
        /// <summary>
        /// Returns cVector2.xyx swizzling.
        /// </summary>
        public cVector3 xyx => new cVector3(x, y, x);
        
        /// <summary>
        /// Returns cVector2.rgr swizzling (equivalent to cVector2.xyx).
        /// </summary>
        public cVector3 rgr => new cVector3(x, y, x);
        
        /// <summary>
        /// Returns cVector2.xyxx swizzling.
        /// </summary>
        public cVector4 xyxx => new cVector4(x, y, x, x);
        
        /// <summary>
        /// Returns cVector2.rgrr swizzling (equivalent to cVector2.xyxx).
        /// </summary>
        public cVector4 rgrr => new cVector4(x, y, x, x);
        
        /// <summary>
        /// Returns cVector2.xyxy swizzling.
        /// </summary>
        public cVector4 xyxy => new cVector4(x, y, x, y);
        
        /// <summary>
        /// Returns cVector2.rgrg swizzling (equivalent to cVector2.xyxy).
        /// </summary>
        public cVector4 rgrg => new cVector4(x, y, x, y);
        
        /// <summary>
        /// Returns cVector2.xyy swizzling.
        /// </summary>
        public cVector3 xyy => new cVector3(x, y, y);
        
        /// <summary>
        /// Returns cVector2.rgg swizzling (equivalent to cVector2.xyy).
        /// </summary>
        public cVector3 rgg => new cVector3(x, y, y);
        
        /// <summary>
        /// Returns cVector2.xyyx swizzling.
        /// </summary>
        public cVector4 xyyx => new cVector4(x, y, y, x);
        
        /// <summary>
        /// Returns cVector2.rggr swizzling (equivalent to cVector2.xyyx).
        /// </summary>
        public cVector4 rggr => new cVector4(x, y, y, x);
        
        /// <summary>
        /// Returns cVector2.xyyy swizzling.
        /// </summary>
        public cVector4 xyyy => new cVector4(x, y, y, y);
        
        /// <summary>
        /// Returns cVector2.rggg swizzling (equivalent to cVector2.xyyy).
        /// </summary>
        public cVector4 rggg => new cVector4(x, y, y, y);
        
        /// <summary>
        /// Returns cVector2.yx swizzling.
        /// </summary>
        public cVector2 yx => new cVector2(y, x);
        
        /// <summary>
        /// Returns cVector2.gr swizzling (equivalent to cVector2.yx).
        /// </summary>
        public cVector2 gr => new cVector2(y, x);
        
        /// <summary>
        /// Returns cVector2.yxx swizzling.
        /// </summary>
        public cVector3 yxx => new cVector3(y, x, x);
        
        /// <summary>
        /// Returns cVector2.grr swizzling (equivalent to cVector2.yxx).
        /// </summary>
        public cVector3 grr => new cVector3(y, x, x);
        
        /// <summary>
        /// Returns cVector2.yxxx swizzling.
        /// </summary>
        public cVector4 yxxx => new cVector4(y, x, x, x);
        
        /// <summary>
        /// Returns cVector2.grrr swizzling (equivalent to cVector2.yxxx).
        /// </summary>
        public cVector4 grrr => new cVector4(y, x, x, x);
        
        /// <summary>
        /// Returns cVector2.yxxy swizzling.
        /// </summary>
        public cVector4 yxxy => new cVector4(y, x, x, y);
        
        /// <summary>
        /// Returns cVector2.grrg swizzling (equivalent to cVector2.yxxy).
        /// </summary>
        public cVector4 grrg => new cVector4(y, x, x, y);
        
        /// <summary>
        /// Returns cVector2.yxy swizzling.
        /// </summary>
        public cVector3 yxy => new cVector3(y, x, y);
        
        /// <summary>
        /// Returns cVector2.grg swizzling (equivalent to cVector2.yxy).
        /// </summary>
        public cVector3 grg => new cVector3(y, x, y);
        
        /// <summary>
        /// Returns cVector2.yxyx swizzling.
        /// </summary>
        public cVector4 yxyx => new cVector4(y, x, y, x);
        
        /// <summary>
        /// Returns cVector2.grgr swizzling (equivalent to cVector2.yxyx).
        /// </summary>
        public cVector4 grgr => new cVector4(y, x, y, x);
        
        /// <summary>
        /// Returns cVector2.yxyy swizzling.
        /// </summary>
        public cVector4 yxyy => new cVector4(y, x, y, y);
        
        /// <summary>
        /// Returns cVector2.grgg swizzling (equivalent to cVector2.yxyy).
        /// </summary>
        public cVector4 grgg => new cVector4(y, x, y, y);
        
        /// <summary>
        /// Returns cVector2.yy swizzling.
        /// </summary>
        public cVector2 yy => new cVector2(y, y);
        
        /// <summary>
        /// Returns cVector2.gg swizzling (equivalent to cVector2.yy).
        /// </summary>
        public cVector2 gg => new cVector2(y, y);
        
        /// <summary>
        /// Returns cVector2.yyx swizzling.
        /// </summary>
        public cVector3 yyx => new cVector3(y, y, x);
        
        /// <summary>
        /// Returns cVector2.ggr swizzling (equivalent to cVector2.yyx).
        /// </summary>
        public cVector3 ggr => new cVector3(y, y, x);
        
        /// <summary>
        /// Returns cVector2.yyxx swizzling.
        /// </summary>
        public cVector4 yyxx => new cVector4(y, y, x, x);
        
        /// <summary>
        /// Returns cVector2.ggrr swizzling (equivalent to cVector2.yyxx).
        /// </summary>
        public cVector4 ggrr => new cVector4(y, y, x, x);
        
        /// <summary>
        /// Returns cVector2.yyxy swizzling.
        /// </summary>
        public cVector4 yyxy => new cVector4(y, y, x, y);
        
        /// <summary>
        /// Returns cVector2.ggrg swizzling (equivalent to cVector2.yyxy).
        /// </summary>
        public cVector4 ggrg => new cVector4(y, y, x, y);
        
        /// <summary>
        /// Returns cVector2.yyy swizzling.
        /// </summary>
        public cVector3 yyy => new cVector3(y, y, y);
        
        /// <summary>
        /// Returns cVector2.ggg swizzling (equivalent to cVector2.yyy).
        /// </summary>
        public cVector3 ggg => new cVector3(y, y, y);
        
        /// <summary>
        /// Returns cVector2.yyyx swizzling.
        /// </summary>
        public cVector4 yyyx => new cVector4(y, y, y, x);
        
        /// <summary>
        /// Returns cVector2.gggr swizzling (equivalent to cVector2.yyyx).
        /// </summary>
        public cVector4 gggr => new cVector4(y, y, y, x);
        
        /// <summary>
        /// Returns cVector2.yyyy swizzling.
        /// </summary>
        public cVector4 yyyy => new cVector4(y, y, y, y);
        
        /// <summary>
        /// Returns cVector2.gggg swizzling (equivalent to cVector2.yyyy).
        /// </summary>
        public cVector4 gggg => new cVector4(y, y, y, y);

        #endregion

    }
    */
}
