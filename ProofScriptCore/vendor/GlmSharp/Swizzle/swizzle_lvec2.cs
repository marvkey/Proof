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
    /// Temporary Vectortor of type long with 2 components, used for implementing swizzling for lVector2.
    /// </summary>
    [RegisterCoreClassStruct]
   
    [StructLayout(LayoutKind.Sequential)]
    public struct swizzle_lVector2
    {

        #region Fields
        
        /// <summary>
        /// x-component
        /// </summary>
        
        internal readonly long x;
        
        /// <summary>
        /// y-component
        /// </summary>
        
        internal readonly long y;

        #endregion


        #region Constructors
        
        /// <summary>
        /// Constructor for swizzle_lVector2.
        /// </summary>
        internal swizzle_lVector2(long x, long y)
        {
            this.x = x;
            this.y = y;
        }

        #endregion


        #region Properties
        
        /// <summary>
        /// Returns lVector2.xx swizzling.
        /// </summary>
        public lVector2 xx => new lVector2(x, x);
        
        /// <summary>
        /// Returns lVector2.rr swizzling (equivalent to lVector2.xx).
        /// </summary>
        public lVector2 rr => new lVector2(x, x);
        
        /// <summary>
        /// Returns lVector2.xxx swizzling.
        /// </summary>
        public lVector3 xxx => new lVector3(x, x, x);
        
        /// <summary>
        /// Returns lVector2.rrr swizzling (equivalent to lVector2.xxx).
        /// </summary>
        public lVector3 rrr => new lVector3(x, x, x);
        
        /// <summary>
        /// Returns lVector2.xxxx swizzling.
        /// </summary>
        public lVector4 xxxx => new lVector4(x, x, x, x);
        
        /// <summary>
        /// Returns lVector2.rrrr swizzling (equivalent to lVector2.xxxx).
        /// </summary>
        public lVector4 rrrr => new lVector4(x, x, x, x);
        
        /// <summary>
        /// Returns lVector2.xxxy swizzling.
        /// </summary>
        public lVector4 xxxy => new lVector4(x, x, x, y);
        
        /// <summary>
        /// Returns lVector2.rrrg swizzling (equivalent to lVector2.xxxy).
        /// </summary>
        public lVector4 rrrg => new lVector4(x, x, x, y);
        
        /// <summary>
        /// Returns lVector2.xxy swizzling.
        /// </summary>
        public lVector3 xxy => new lVector3(x, x, y);
        
        /// <summary>
        /// Returns lVector2.rrg swizzling (equivalent to lVector2.xxy).
        /// </summary>
        public lVector3 rrg => new lVector3(x, x, y);
        
        /// <summary>
        /// Returns lVector2.xxyx swizzling.
        /// </summary>
        public lVector4 xxyx => new lVector4(x, x, y, x);
        
        /// <summary>
        /// Returns lVector2.rrgr swizzling (equivalent to lVector2.xxyx).
        /// </summary>
        public lVector4 rrgr => new lVector4(x, x, y, x);
        
        /// <summary>
        /// Returns lVector2.xxyy swizzling.
        /// </summary>
        public lVector4 xxyy => new lVector4(x, x, y, y);
        
        /// <summary>
        /// Returns lVector2.rrgg swizzling (equivalent to lVector2.xxyy).
        /// </summary>
        public lVector4 rrgg => new lVector4(x, x, y, y);
        
        /// <summary>
        /// Returns lVector2.xy swizzling.
        /// </summary>
        public lVector2 xy => new lVector2(x, y);
        
        /// <summary>
        /// Returns lVector2.rg swizzling (equivalent to lVector2.xy).
        /// </summary>
        public lVector2 rg => new lVector2(x, y);
        
        /// <summary>
        /// Returns lVector2.xyx swizzling.
        /// </summary>
        public lVector3 xyx => new lVector3(x, y, x);
        
        /// <summary>
        /// Returns lVector2.rgr swizzling (equivalent to lVector2.xyx).
        /// </summary>
        public lVector3 rgr => new lVector3(x, y, x);
        
        /// <summary>
        /// Returns lVector2.xyxx swizzling.
        /// </summary>
        public lVector4 xyxx => new lVector4(x, y, x, x);
        
        /// <summary>
        /// Returns lVector2.rgrr swizzling (equivalent to lVector2.xyxx).
        /// </summary>
        public lVector4 rgrr => new lVector4(x, y, x, x);
        
        /// <summary>
        /// Returns lVector2.xyxy swizzling.
        /// </summary>
        public lVector4 xyxy => new lVector4(x, y, x, y);
        
        /// <summary>
        /// Returns lVector2.rgrg swizzling (equivalent to lVector2.xyxy).
        /// </summary>
        public lVector4 rgrg => new lVector4(x, y, x, y);
        
        /// <summary>
        /// Returns lVector2.xyy swizzling.
        /// </summary>
        public lVector3 xyy => new lVector3(x, y, y);
        
        /// <summary>
        /// Returns lVector2.rgg swizzling (equivalent to lVector2.xyy).
        /// </summary>
        public lVector3 rgg => new lVector3(x, y, y);
        
        /// <summary>
        /// Returns lVector2.xyyx swizzling.
        /// </summary>
        public lVector4 xyyx => new lVector4(x, y, y, x);
        
        /// <summary>
        /// Returns lVector2.rggr swizzling (equivalent to lVector2.xyyx).
        /// </summary>
        public lVector4 rggr => new lVector4(x, y, y, x);
        
        /// <summary>
        /// Returns lVector2.xyyy swizzling.
        /// </summary>
        public lVector4 xyyy => new lVector4(x, y, y, y);
        
        /// <summary>
        /// Returns lVector2.rggg swizzling (equivalent to lVector2.xyyy).
        /// </summary>
        public lVector4 rggg => new lVector4(x, y, y, y);
        
        /// <summary>
        /// Returns lVector2.yx swizzling.
        /// </summary>
        public lVector2 yx => new lVector2(y, x);
        
        /// <summary>
        /// Returns lVector2.gr swizzling (equivalent to lVector2.yx).
        /// </summary>
        public lVector2 gr => new lVector2(y, x);
        
        /// <summary>
        /// Returns lVector2.yxx swizzling.
        /// </summary>
        public lVector3 yxx => new lVector3(y, x, x);
        
        /// <summary>
        /// Returns lVector2.grr swizzling (equivalent to lVector2.yxx).
        /// </summary>
        public lVector3 grr => new lVector3(y, x, x);
        
        /// <summary>
        /// Returns lVector2.yxxx swizzling.
        /// </summary>
        public lVector4 yxxx => new lVector4(y, x, x, x);
        
        /// <summary>
        /// Returns lVector2.grrr swizzling (equivalent to lVector2.yxxx).
        /// </summary>
        public lVector4 grrr => new lVector4(y, x, x, x);
        
        /// <summary>
        /// Returns lVector2.yxxy swizzling.
        /// </summary>
        public lVector4 yxxy => new lVector4(y, x, x, y);
        
        /// <summary>
        /// Returns lVector2.grrg swizzling (equivalent to lVector2.yxxy).
        /// </summary>
        public lVector4 grrg => new lVector4(y, x, x, y);
        
        /// <summary>
        /// Returns lVector2.yxy swizzling.
        /// </summary>
        public lVector3 yxy => new lVector3(y, x, y);
        
        /// <summary>
        /// Returns lVector2.grg swizzling (equivalent to lVector2.yxy).
        /// </summary>
        public lVector3 grg => new lVector3(y, x, y);
        
        /// <summary>
        /// Returns lVector2.yxyx swizzling.
        /// </summary>
        public lVector4 yxyx => new lVector4(y, x, y, x);
        
        /// <summary>
        /// Returns lVector2.grgr swizzling (equivalent to lVector2.yxyx).
        /// </summary>
        public lVector4 grgr => new lVector4(y, x, y, x);
        
        /// <summary>
        /// Returns lVector2.yxyy swizzling.
        /// </summary>
        public lVector4 yxyy => new lVector4(y, x, y, y);
        
        /// <summary>
        /// Returns lVector2.grgg swizzling (equivalent to lVector2.yxyy).
        /// </summary>
        public lVector4 grgg => new lVector4(y, x, y, y);
        
        /// <summary>
        /// Returns lVector2.yy swizzling.
        /// </summary>
        public lVector2 yy => new lVector2(y, y);
        
        /// <summary>
        /// Returns lVector2.gg swizzling (equivalent to lVector2.yy).
        /// </summary>
        public lVector2 gg => new lVector2(y, y);
        
        /// <summary>
        /// Returns lVector2.yyx swizzling.
        /// </summary>
        public lVector3 yyx => new lVector3(y, y, x);
        
        /// <summary>
        /// Returns lVector2.ggr swizzling (equivalent to lVector2.yyx).
        /// </summary>
        public lVector3 ggr => new lVector3(y, y, x);
        
        /// <summary>
        /// Returns lVector2.yyxx swizzling.
        /// </summary>
        public lVector4 yyxx => new lVector4(y, y, x, x);
        
        /// <summary>
        /// Returns lVector2.ggrr swizzling (equivalent to lVector2.yyxx).
        /// </summary>
        public lVector4 ggrr => new lVector4(y, y, x, x);
        
        /// <summary>
        /// Returns lVector2.yyxy swizzling.
        /// </summary>
        public lVector4 yyxy => new lVector4(y, y, x, y);
        
        /// <summary>
        /// Returns lVector2.ggrg swizzling (equivalent to lVector2.yyxy).
        /// </summary>
        public lVector4 ggrg => new lVector4(y, y, x, y);
        
        /// <summary>
        /// Returns lVector2.yyy swizzling.
        /// </summary>
        public lVector3 yyy => new lVector3(y, y, y);
        
        /// <summary>
        /// Returns lVector2.ggg swizzling (equivalent to lVector2.yyy).
        /// </summary>
        public lVector3 ggg => new lVector3(y, y, y);
        
        /// <summary>
        /// Returns lVector2.yyyx swizzling.
        /// </summary>
        public lVector4 yyyx => new lVector4(y, y, y, x);
        
        /// <summary>
        /// Returns lVector2.gggr swizzling (equivalent to lVector2.yyyx).
        /// </summary>
        public lVector4 gggr => new lVector4(y, y, y, x);
        
        /// <summary>
        /// Returns lVector2.yyyy swizzling.
        /// </summary>
        public lVector4 yyyy => new lVector4(y, y, y, y);
        
        /// <summary>
        /// Returns lVector2.gggg swizzling (equivalent to lVector2.yyyy).
        /// </summary>
        public lVector4 gggg => new lVector4(y, y, y, y);

        #endregion

    }
}
