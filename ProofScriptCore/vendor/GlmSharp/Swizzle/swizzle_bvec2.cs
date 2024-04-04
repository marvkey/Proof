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
    /// Temporary Vectortor of type bool with 2 components, used for implementing swizzling for bVector2.
    /// </summary>
    [RegisterCoreClassStruct]
   
    [StructLayout(LayoutKind.Sequential)]
    public struct swizzle_bVector2
    {

        #region Fields
        
        /// <summary>
        /// x-component
        /// </summary>
        
        internal readonly bool x;
        
        /// <summary>
        /// y-component
        /// </summary>
        
        internal readonly bool y;

        #endregion


        #region Constructors
        
        /// <summary>
        /// Constructor for swizzle_bVector2.
        /// </summary>
        internal swizzle_bVector2(bool x, bool y)
        {
            this.x = x;
            this.y = y;
        }

        #endregion


        #region Properties
        
        /// <summary>
        /// Returns bVector2.xx swizzling.
        /// </summary>
        public bVector2 xx => new bVector2(x, x);
        
        /// <summary>
        /// Returns bVector2.rr swizzling (equivalent to bVector2.xx).
        /// </summary>
        public bVector2 rr => new bVector2(x, x);
        
        /// <summary>
        /// Returns bVector2.xxx swizzling.
        /// </summary>
        public bVector3 xxx => new bVector3(x, x, x);
        
        /// <summary>
        /// Returns bVector2.rrr swizzling (equivalent to bVector2.xxx).
        /// </summary>
        public bVector3 rrr => new bVector3(x, x, x);
        
        /// <summary>
        /// Returns bVector2.xxxx swizzling.
        /// </summary>
        public bVector4 xxxx => new bVector4(x, x, x, x);
        
        /// <summary>
        /// Returns bVector2.rrrr swizzling (equivalent to bVector2.xxxx).
        /// </summary>
        public bVector4 rrrr => new bVector4(x, x, x, x);
        
        /// <summary>
        /// Returns bVector2.xxxy swizzling.
        /// </summary>
        public bVector4 xxxy => new bVector4(x, x, x, y);
        
        /// <summary>
        /// Returns bVector2.rrrg swizzling (equivalent to bVector2.xxxy).
        /// </summary>
        public bVector4 rrrg => new bVector4(x, x, x, y);
        
        /// <summary>
        /// Returns bVector2.xxy swizzling.
        /// </summary>
        public bVector3 xxy => new bVector3(x, x, y);
        
        /// <summary>
        /// Returns bVector2.rrg swizzling (equivalent to bVector2.xxy).
        /// </summary>
        public bVector3 rrg => new bVector3(x, x, y);
        
        /// <summary>
        /// Returns bVector2.xxyx swizzling.
        /// </summary>
        public bVector4 xxyx => new bVector4(x, x, y, x);
        
        /// <summary>
        /// Returns bVector2.rrgr swizzling (equivalent to bVector2.xxyx).
        /// </summary>
        public bVector4 rrgr => new bVector4(x, x, y, x);
        
        /// <summary>
        /// Returns bVector2.xxyy swizzling.
        /// </summary>
        public bVector4 xxyy => new bVector4(x, x, y, y);
        
        /// <summary>
        /// Returns bVector2.rrgg swizzling (equivalent to bVector2.xxyy).
        /// </summary>
        public bVector4 rrgg => new bVector4(x, x, y, y);
        
        /// <summary>
        /// Returns bVector2.xy swizzling.
        /// </summary>
        public bVector2 xy => new bVector2(x, y);
        
        /// <summary>
        /// Returns bVector2.rg swizzling (equivalent to bVector2.xy).
        /// </summary>
        public bVector2 rg => new bVector2(x, y);
        
        /// <summary>
        /// Returns bVector2.xyx swizzling.
        /// </summary>
        public bVector3 xyx => new bVector3(x, y, x);
        
        /// <summary>
        /// Returns bVector2.rgr swizzling (equivalent to bVector2.xyx).
        /// </summary>
        public bVector3 rgr => new bVector3(x, y, x);
        
        /// <summary>
        /// Returns bVector2.xyxx swizzling.
        /// </summary>
        public bVector4 xyxx => new bVector4(x, y, x, x);
        
        /// <summary>
        /// Returns bVector2.rgrr swizzling (equivalent to bVector2.xyxx).
        /// </summary>
        public bVector4 rgrr => new bVector4(x, y, x, x);
        
        /// <summary>
        /// Returns bVector2.xyxy swizzling.
        /// </summary>
        public bVector4 xyxy => new bVector4(x, y, x, y);
        
        /// <summary>
        /// Returns bVector2.rgrg swizzling (equivalent to bVector2.xyxy).
        /// </summary>
        public bVector4 rgrg => new bVector4(x, y, x, y);
        
        /// <summary>
        /// Returns bVector2.xyy swizzling.
        /// </summary>
        public bVector3 xyy => new bVector3(x, y, y);
        
        /// <summary>
        /// Returns bVector2.rgg swizzling (equivalent to bVector2.xyy).
        /// </summary>
        public bVector3 rgg => new bVector3(x, y, y);
        
        /// <summary>
        /// Returns bVector2.xyyx swizzling.
        /// </summary>
        public bVector4 xyyx => new bVector4(x, y, y, x);
        
        /// <summary>
        /// Returns bVector2.rggr swizzling (equivalent to bVector2.xyyx).
        /// </summary>
        public bVector4 rggr => new bVector4(x, y, y, x);
        
        /// <summary>
        /// Returns bVector2.xyyy swizzling.
        /// </summary>
        public bVector4 xyyy => new bVector4(x, y, y, y);
        
        /// <summary>
        /// Returns bVector2.rggg swizzling (equivalent to bVector2.xyyy).
        /// </summary>
        public bVector4 rggg => new bVector4(x, y, y, y);
        
        /// <summary>
        /// Returns bVector2.yx swizzling.
        /// </summary>
        public bVector2 yx => new bVector2(y, x);
        
        /// <summary>
        /// Returns bVector2.gr swizzling (equivalent to bVector2.yx).
        /// </summary>
        public bVector2 gr => new bVector2(y, x);
        
        /// <summary>
        /// Returns bVector2.yxx swizzling.
        /// </summary>
        public bVector3 yxx => new bVector3(y, x, x);
        
        /// <summary>
        /// Returns bVector2.grr swizzling (equivalent to bVector2.yxx).
        /// </summary>
        public bVector3 grr => new bVector3(y, x, x);
        
        /// <summary>
        /// Returns bVector2.yxxx swizzling.
        /// </summary>
        public bVector4 yxxx => new bVector4(y, x, x, x);
        
        /// <summary>
        /// Returns bVector2.grrr swizzling (equivalent to bVector2.yxxx).
        /// </summary>
        public bVector4 grrr => new bVector4(y, x, x, x);
        
        /// <summary>
        /// Returns bVector2.yxxy swizzling.
        /// </summary>
        public bVector4 yxxy => new bVector4(y, x, x, y);
        
        /// <summary>
        /// Returns bVector2.grrg swizzling (equivalent to bVector2.yxxy).
        /// </summary>
        public bVector4 grrg => new bVector4(y, x, x, y);
        
        /// <summary>
        /// Returns bVector2.yxy swizzling.
        /// </summary>
        public bVector3 yxy => new bVector3(y, x, y);
        
        /// <summary>
        /// Returns bVector2.grg swizzling (equivalent to bVector2.yxy).
        /// </summary>
        public bVector3 grg => new bVector3(y, x, y);
        
        /// <summary>
        /// Returns bVector2.yxyx swizzling.
        /// </summary>
        public bVector4 yxyx => new bVector4(y, x, y, x);
        
        /// <summary>
        /// Returns bVector2.grgr swizzling (equivalent to bVector2.yxyx).
        /// </summary>
        public bVector4 grgr => new bVector4(y, x, y, x);
        
        /// <summary>
        /// Returns bVector2.yxyy swizzling.
        /// </summary>
        public bVector4 yxyy => new bVector4(y, x, y, y);
        
        /// <summary>
        /// Returns bVector2.grgg swizzling (equivalent to bVector2.yxyy).
        /// </summary>
        public bVector4 grgg => new bVector4(y, x, y, y);
        
        /// <summary>
        /// Returns bVector2.yy swizzling.
        /// </summary>
        public bVector2 yy => new bVector2(y, y);
        
        /// <summary>
        /// Returns bVector2.gg swizzling (equivalent to bVector2.yy).
        /// </summary>
        public bVector2 gg => new bVector2(y, y);
        
        /// <summary>
        /// Returns bVector2.yyx swizzling.
        /// </summary>
        public bVector3 yyx => new bVector3(y, y, x);
        
        /// <summary>
        /// Returns bVector2.ggr swizzling (equivalent to bVector2.yyx).
        /// </summary>
        public bVector3 ggr => new bVector3(y, y, x);
        
        /// <summary>
        /// Returns bVector2.yyxx swizzling.
        /// </summary>
        public bVector4 yyxx => new bVector4(y, y, x, x);
        
        /// <summary>
        /// Returns bVector2.ggrr swizzling (equivalent to bVector2.yyxx).
        /// </summary>
        public bVector4 ggrr => new bVector4(y, y, x, x);
        
        /// <summary>
        /// Returns bVector2.yyxy swizzling.
        /// </summary>
        public bVector4 yyxy => new bVector4(y, y, x, y);
        
        /// <summary>
        /// Returns bVector2.ggrg swizzling (equivalent to bVector2.yyxy).
        /// </summary>
        public bVector4 ggrg => new bVector4(y, y, x, y);
        
        /// <summary>
        /// Returns bVector2.yyy swizzling.
        /// </summary>
        public bVector3 yyy => new bVector3(y, y, y);
        
        /// <summary>
        /// Returns bVector2.ggg swizzling (equivalent to bVector2.yyy).
        /// </summary>
        public bVector3 ggg => new bVector3(y, y, y);
        
        /// <summary>
        /// Returns bVector2.yyyx swizzling.
        /// </summary>
        public bVector4 yyyx => new bVector4(y, y, y, x);
        
        /// <summary>
        /// Returns bVector2.gggr swizzling (equivalent to bVector2.yyyx).
        /// </summary>
        public bVector4 gggr => new bVector4(y, y, y, x);
        
        /// <summary>
        /// Returns bVector2.yyyy swizzling.
        /// </summary>
        public bVector4 yyyy => new bVector4(y, y, y, y);
        
        /// <summary>
        /// Returns bVector2.gggg swizzling (equivalent to bVector2.yyyy).
        /// </summary>
        public bVector4 gggg => new bVector4(y, y, y, y);

        #endregion

    }
}
