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
    /// Temporary Vectortor of type double with 2 components, used for implementing swizzling for dVector2.
    /// </summary>
    [RegisterCoreClassStruct]
   
    [StructLayout(LayoutKind.Sequential)]
    public struct swizzle_dVector2
    {

        #region Fields
        
        /// <summary>
        /// x-component
        /// </summary>
        
        internal readonly double x;
        
        /// <summary>
        /// y-component
        /// </summary>
        
        internal readonly double y;

        #endregion


        #region Constructors
        
        /// <summary>
        /// Constructor for swizzle_dVector2.
        /// </summary>
        internal swizzle_dVector2(double x, double y)
        {
            this.x = x;
            this.y = y;
        }

        #endregion


        #region Properties
        
        /// <summary>
        /// Returns dVector2.xx swizzling.
        /// </summary>
        public dVector2 xx => new dVector2(x, x);
        
        /// <summary>
        /// Returns dVector2.rr swizzling (equivalent to dVector2.xx).
        /// </summary>
        public dVector2 rr => new dVector2(x, x);
        
        /// <summary>
        /// Returns dVector2.xxx swizzling.
        /// </summary>
        public dVector3 xxx => new dVector3(x, x, x);
        
        /// <summary>
        /// Returns dVector2.rrr swizzling (equivalent to dVector2.xxx).
        /// </summary>
        public dVector3 rrr => new dVector3(x, x, x);
        
        /// <summary>
        /// Returns dVector2.xxxx swizzling.
        /// </summary>
        public dVector4 xxxx => new dVector4(x, x, x, x);
        
        /// <summary>
        /// Returns dVector2.rrrr swizzling (equivalent to dVector2.xxxx).
        /// </summary>
        public dVector4 rrrr => new dVector4(x, x, x, x);
        
        /// <summary>
        /// Returns dVector2.xxxy swizzling.
        /// </summary>
        public dVector4 xxxy => new dVector4(x, x, x, y);
        
        /// <summary>
        /// Returns dVector2.rrrg swizzling (equivalent to dVector2.xxxy).
        /// </summary>
        public dVector4 rrrg => new dVector4(x, x, x, y);
        
        /// <summary>
        /// Returns dVector2.xxy swizzling.
        /// </summary>
        public dVector3 xxy => new dVector3(x, x, y);
        
        /// <summary>
        /// Returns dVector2.rrg swizzling (equivalent to dVector2.xxy).
        /// </summary>
        public dVector3 rrg => new dVector3(x, x, y);
        
        /// <summary>
        /// Returns dVector2.xxyx swizzling.
        /// </summary>
        public dVector4 xxyx => new dVector4(x, x, y, x);
        
        /// <summary>
        /// Returns dVector2.rrgr swizzling (equivalent to dVector2.xxyx).
        /// </summary>
        public dVector4 rrgr => new dVector4(x, x, y, x);
        
        /// <summary>
        /// Returns dVector2.xxyy swizzling.
        /// </summary>
        public dVector4 xxyy => new dVector4(x, x, y, y);
        
        /// <summary>
        /// Returns dVector2.rrgg swizzling (equivalent to dVector2.xxyy).
        /// </summary>
        public dVector4 rrgg => new dVector4(x, x, y, y);
        
        /// <summary>
        /// Returns dVector2.xy swizzling.
        /// </summary>
        public dVector2 xy => new dVector2(x, y);
        
        /// <summary>
        /// Returns dVector2.rg swizzling (equivalent to dVector2.xy).
        /// </summary>
        public dVector2 rg => new dVector2(x, y);
        
        /// <summary>
        /// Returns dVector2.xyx swizzling.
        /// </summary>
        public dVector3 xyx => new dVector3(x, y, x);
        
        /// <summary>
        /// Returns dVector2.rgr swizzling (equivalent to dVector2.xyx).
        /// </summary>
        public dVector3 rgr => new dVector3(x, y, x);
        
        /// <summary>
        /// Returns dVector2.xyxx swizzling.
        /// </summary>
        public dVector4 xyxx => new dVector4(x, y, x, x);
        
        /// <summary>
        /// Returns dVector2.rgrr swizzling (equivalent to dVector2.xyxx).
        /// </summary>
        public dVector4 rgrr => new dVector4(x, y, x, x);
        
        /// <summary>
        /// Returns dVector2.xyxy swizzling.
        /// </summary>
        public dVector4 xyxy => new dVector4(x, y, x, y);
        
        /// <summary>
        /// Returns dVector2.rgrg swizzling (equivalent to dVector2.xyxy).
        /// </summary>
        public dVector4 rgrg => new dVector4(x, y, x, y);
        
        /// <summary>
        /// Returns dVector2.xyy swizzling.
        /// </summary>
        public dVector3 xyy => new dVector3(x, y, y);
        
        /// <summary>
        /// Returns dVector2.rgg swizzling (equivalent to dVector2.xyy).
        /// </summary>
        public dVector3 rgg => new dVector3(x, y, y);
        
        /// <summary>
        /// Returns dVector2.xyyx swizzling.
        /// </summary>
        public dVector4 xyyx => new dVector4(x, y, y, x);
        
        /// <summary>
        /// Returns dVector2.rggr swizzling (equivalent to dVector2.xyyx).
        /// </summary>
        public dVector4 rggr => new dVector4(x, y, y, x);
        
        /// <summary>
        /// Returns dVector2.xyyy swizzling.
        /// </summary>
        public dVector4 xyyy => new dVector4(x, y, y, y);
        
        /// <summary>
        /// Returns dVector2.rggg swizzling (equivalent to dVector2.xyyy).
        /// </summary>
        public dVector4 rggg => new dVector4(x, y, y, y);
        
        /// <summary>
        /// Returns dVector2.yx swizzling.
        /// </summary>
        public dVector2 yx => new dVector2(y, x);
        
        /// <summary>
        /// Returns dVector2.gr swizzling (equivalent to dVector2.yx).
        /// </summary>
        public dVector2 gr => new dVector2(y, x);
        
        /// <summary>
        /// Returns dVector2.yxx swizzling.
        /// </summary>
        public dVector3 yxx => new dVector3(y, x, x);
        
        /// <summary>
        /// Returns dVector2.grr swizzling (equivalent to dVector2.yxx).
        /// </summary>
        public dVector3 grr => new dVector3(y, x, x);
        
        /// <summary>
        /// Returns dVector2.yxxx swizzling.
        /// </summary>
        public dVector4 yxxx => new dVector4(y, x, x, x);
        
        /// <summary>
        /// Returns dVector2.grrr swizzling (equivalent to dVector2.yxxx).
        /// </summary>
        public dVector4 grrr => new dVector4(y, x, x, x);
        
        /// <summary>
        /// Returns dVector2.yxxy swizzling.
        /// </summary>
        public dVector4 yxxy => new dVector4(y, x, x, y);
        
        /// <summary>
        /// Returns dVector2.grrg swizzling (equivalent to dVector2.yxxy).
        /// </summary>
        public dVector4 grrg => new dVector4(y, x, x, y);
        
        /// <summary>
        /// Returns dVector2.yxy swizzling.
        /// </summary>
        public dVector3 yxy => new dVector3(y, x, y);
        
        /// <summary>
        /// Returns dVector2.grg swizzling (equivalent to dVector2.yxy).
        /// </summary>
        public dVector3 grg => new dVector3(y, x, y);
        
        /// <summary>
        /// Returns dVector2.yxyx swizzling.
        /// </summary>
        public dVector4 yxyx => new dVector4(y, x, y, x);
        
        /// <summary>
        /// Returns dVector2.grgr swizzling (equivalent to dVector2.yxyx).
        /// </summary>
        public dVector4 grgr => new dVector4(y, x, y, x);
        
        /// <summary>
        /// Returns dVector2.yxyy swizzling.
        /// </summary>
        public dVector4 yxyy => new dVector4(y, x, y, y);
        
        /// <summary>
        /// Returns dVector2.grgg swizzling (equivalent to dVector2.yxyy).
        /// </summary>
        public dVector4 grgg => new dVector4(y, x, y, y);
        
        /// <summary>
        /// Returns dVector2.yy swizzling.
        /// </summary>
        public dVector2 yy => new dVector2(y, y);
        
        /// <summary>
        /// Returns dVector2.gg swizzling (equivalent to dVector2.yy).
        /// </summary>
        public dVector2 gg => new dVector2(y, y);
        
        /// <summary>
        /// Returns dVector2.yyx swizzling.
        /// </summary>
        public dVector3 yyx => new dVector3(y, y, x);
        
        /// <summary>
        /// Returns dVector2.ggr swizzling (equivalent to dVector2.yyx).
        /// </summary>
        public dVector3 ggr => new dVector3(y, y, x);
        
        /// <summary>
        /// Returns dVector2.yyxx swizzling.
        /// </summary>
        public dVector4 yyxx => new dVector4(y, y, x, x);
        
        /// <summary>
        /// Returns dVector2.ggrr swizzling (equivalent to dVector2.yyxx).
        /// </summary>
        public dVector4 ggrr => new dVector4(y, y, x, x);
        
        /// <summary>
        /// Returns dVector2.yyxy swizzling.
        /// </summary>
        public dVector4 yyxy => new dVector4(y, y, x, y);
        
        /// <summary>
        /// Returns dVector2.ggrg swizzling (equivalent to dVector2.yyxy).
        /// </summary>
        public dVector4 ggrg => new dVector4(y, y, x, y);
        
        /// <summary>
        /// Returns dVector2.yyy swizzling.
        /// </summary>
        public dVector3 yyy => new dVector3(y, y, y);
        
        /// <summary>
        /// Returns dVector2.ggg swizzling (equivalent to dVector2.yyy).
        /// </summary>
        public dVector3 ggg => new dVector3(y, y, y);
        
        /// <summary>
        /// Returns dVector2.yyyx swizzling.
        /// </summary>
        public dVector4 yyyx => new dVector4(y, y, y, x);
        
        /// <summary>
        /// Returns dVector2.gggr swizzling (equivalent to dVector2.yyyx).
        /// </summary>
        public dVector4 gggr => new dVector4(y, y, y, x);
        
        /// <summary>
        /// Returns dVector2.yyyy swizzling.
        /// </summary>
        public dVector4 yyyy => new dVector4(y, y, y, y);
        
        /// <summary>
        /// Returns dVector2.gggg swizzling (equivalent to dVector2.yyyy).
        /// </summary>
        public dVector4 gggg => new dVector4(y, y, y, y);

        #endregion

    }
}
