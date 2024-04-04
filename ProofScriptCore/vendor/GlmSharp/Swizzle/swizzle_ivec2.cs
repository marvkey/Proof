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
    /// Temporary Vectortor of type int with 2 components, used for implementing swizzling for iVector2.
    /// </summary>
    [RegisterCoreClassStruct]
   
    [StructLayout(LayoutKind.Sequential)]
    public struct swizzle_iVector2
    {

        #region Fields
        
        /// <summary>
        /// x-component
        /// </summary>
        
        internal readonly int x;
        
        /// <summary>
        /// y-component
        /// </summary>
        
        internal readonly int y;

        #endregion


        #region Constructors
        
        /// <summary>
        /// Constructor for swizzle_iVector2.
        /// </summary>
        internal swizzle_iVector2(int x, int y)
        {
            this.x = x;
            this.y = y;
        }

        #endregion


        #region Properties
        
        /// <summary>
        /// Returns iVector2.xx swizzling.
        /// </summary>
        public iVector2 xx => new iVector2(x, x);
        
        /// <summary>
        /// Returns iVector2.rr swizzling (equivalent to iVector2.xx).
        /// </summary>
        public iVector2 rr => new iVector2(x, x);
        
        /// <summary>
        /// Returns iVector2.xxx swizzling.
        /// </summary>
        public iVector3 xxx => new iVector3(x, x, x);
        
        /// <summary>
        /// Returns iVector2.rrr swizzling (equivalent to iVector2.xxx).
        /// </summary>
        public iVector3 rrr => new iVector3(x, x, x);
        
        /// <summary>
        /// Returns iVector2.xxxx swizzling.
        /// </summary>
        public iVector4 xxxx => new iVector4(x, x, x, x);
        
        /// <summary>
        /// Returns iVector2.rrrr swizzling (equivalent to iVector2.xxxx).
        /// </summary>
        public iVector4 rrrr => new iVector4(x, x, x, x);
        
        /// <summary>
        /// Returns iVector2.xxxy swizzling.
        /// </summary>
        public iVector4 xxxy => new iVector4(x, x, x, y);
        
        /// <summary>
        /// Returns iVector2.rrrg swizzling (equivalent to iVector2.xxxy).
        /// </summary>
        public iVector4 rrrg => new iVector4(x, x, x, y);
        
        /// <summary>
        /// Returns iVector2.xxy swizzling.
        /// </summary>
        public iVector3 xxy => new iVector3(x, x, y);
        
        /// <summary>
        /// Returns iVector2.rrg swizzling (equivalent to iVector2.xxy).
        /// </summary>
        public iVector3 rrg => new iVector3(x, x, y);
        
        /// <summary>
        /// Returns iVector2.xxyx swizzling.
        /// </summary>
        public iVector4 xxyx => new iVector4(x, x, y, x);
        
        /// <summary>
        /// Returns iVector2.rrgr swizzling (equivalent to iVector2.xxyx).
        /// </summary>
        public iVector4 rrgr => new iVector4(x, x, y, x);
        
        /// <summary>
        /// Returns iVector2.xxyy swizzling.
        /// </summary>
        public iVector4 xxyy => new iVector4(x, x, y, y);
        
        /// <summary>
        /// Returns iVector2.rrgg swizzling (equivalent to iVector2.xxyy).
        /// </summary>
        public iVector4 rrgg => new iVector4(x, x, y, y);
        
        /// <summary>
        /// Returns iVector2.xy swizzling.
        /// </summary>
        public iVector2 xy => new iVector2(x, y);
        
        /// <summary>
        /// Returns iVector2.rg swizzling (equivalent to iVector2.xy).
        /// </summary>
        public iVector2 rg => new iVector2(x, y);
        
        /// <summary>
        /// Returns iVector2.xyx swizzling.
        /// </summary>
        public iVector3 xyx => new iVector3(x, y, x);
        
        /// <summary>
        /// Returns iVector2.rgr swizzling (equivalent to iVector2.xyx).
        /// </summary>
        public iVector3 rgr => new iVector3(x, y, x);
        
        /// <summary>
        /// Returns iVector2.xyxx swizzling.
        /// </summary>
        public iVector4 xyxx => new iVector4(x, y, x, x);
        
        /// <summary>
        /// Returns iVector2.rgrr swizzling (equivalent to iVector2.xyxx).
        /// </summary>
        public iVector4 rgrr => new iVector4(x, y, x, x);
        
        /// <summary>
        /// Returns iVector2.xyxy swizzling.
        /// </summary>
        public iVector4 xyxy => new iVector4(x, y, x, y);
        
        /// <summary>
        /// Returns iVector2.rgrg swizzling (equivalent to iVector2.xyxy).
        /// </summary>
        public iVector4 rgrg => new iVector4(x, y, x, y);
        
        /// <summary>
        /// Returns iVector2.xyy swizzling.
        /// </summary>
        public iVector3 xyy => new iVector3(x, y, y);
        
        /// <summary>
        /// Returns iVector2.rgg swizzling (equivalent to iVector2.xyy).
        /// </summary>
        public iVector3 rgg => new iVector3(x, y, y);
        
        /// <summary>
        /// Returns iVector2.xyyx swizzling.
        /// </summary>
        public iVector4 xyyx => new iVector4(x, y, y, x);
        
        /// <summary>
        /// Returns iVector2.rggr swizzling (equivalent to iVector2.xyyx).
        /// </summary>
        public iVector4 rggr => new iVector4(x, y, y, x);
        
        /// <summary>
        /// Returns iVector2.xyyy swizzling.
        /// </summary>
        public iVector4 xyyy => new iVector4(x, y, y, y);
        
        /// <summary>
        /// Returns iVector2.rggg swizzling (equivalent to iVector2.xyyy).
        /// </summary>
        public iVector4 rggg => new iVector4(x, y, y, y);
        
        /// <summary>
        /// Returns iVector2.yx swizzling.
        /// </summary>
        public iVector2 yx => new iVector2(y, x);
        
        /// <summary>
        /// Returns iVector2.gr swizzling (equivalent to iVector2.yx).
        /// </summary>
        public iVector2 gr => new iVector2(y, x);
        
        /// <summary>
        /// Returns iVector2.yxx swizzling.
        /// </summary>
        public iVector3 yxx => new iVector3(y, x, x);
        
        /// <summary>
        /// Returns iVector2.grr swizzling (equivalent to iVector2.yxx).
        /// </summary>
        public iVector3 grr => new iVector3(y, x, x);
        
        /// <summary>
        /// Returns iVector2.yxxx swizzling.
        /// </summary>
        public iVector4 yxxx => new iVector4(y, x, x, x);
        
        /// <summary>
        /// Returns iVector2.grrr swizzling (equivalent to iVector2.yxxx).
        /// </summary>
        public iVector4 grrr => new iVector4(y, x, x, x);
        
        /// <summary>
        /// Returns iVector2.yxxy swizzling.
        /// </summary>
        public iVector4 yxxy => new iVector4(y, x, x, y);
        
        /// <summary>
        /// Returns iVector2.grrg swizzling (equivalent to iVector2.yxxy).
        /// </summary>
        public iVector4 grrg => new iVector4(y, x, x, y);
        
        /// <summary>
        /// Returns iVector2.yxy swizzling.
        /// </summary>
        public iVector3 yxy => new iVector3(y, x, y);
        
        /// <summary>
        /// Returns iVector2.grg swizzling (equivalent to iVector2.yxy).
        /// </summary>
        public iVector3 grg => new iVector3(y, x, y);
        
        /// <summary>
        /// Returns iVector2.yxyx swizzling.
        /// </summary>
        public iVector4 yxyx => new iVector4(y, x, y, x);
        
        /// <summary>
        /// Returns iVector2.grgr swizzling (equivalent to iVector2.yxyx).
        /// </summary>
        public iVector4 grgr => new iVector4(y, x, y, x);
        
        /// <summary>
        /// Returns iVector2.yxyy swizzling.
        /// </summary>
        public iVector4 yxyy => new iVector4(y, x, y, y);
        
        /// <summary>
        /// Returns iVector2.grgg swizzling (equivalent to iVector2.yxyy).
        /// </summary>
        public iVector4 grgg => new iVector4(y, x, y, y);
        
        /// <summary>
        /// Returns iVector2.yy swizzling.
        /// </summary>
        public iVector2 yy => new iVector2(y, y);
        
        /// <summary>
        /// Returns iVector2.gg swizzling (equivalent to iVector2.yy).
        /// </summary>
        public iVector2 gg => new iVector2(y, y);
        
        /// <summary>
        /// Returns iVector2.yyx swizzling.
        /// </summary>
        public iVector3 yyx => new iVector3(y, y, x);
        
        /// <summary>
        /// Returns iVector2.ggr swizzling (equivalent to iVector2.yyx).
        /// </summary>
        public iVector3 ggr => new iVector3(y, y, x);
        
        /// <summary>
        /// Returns iVector2.yyxx swizzling.
        /// </summary>
        public iVector4 yyxx => new iVector4(y, y, x, x);
        
        /// <summary>
        /// Returns iVector2.ggrr swizzling (equivalent to iVector2.yyxx).
        /// </summary>
        public iVector4 ggrr => new iVector4(y, y, x, x);
        
        /// <summary>
        /// Returns iVector2.yyxy swizzling.
        /// </summary>
        public iVector4 yyxy => new iVector4(y, y, x, y);
        
        /// <summary>
        /// Returns iVector2.ggrg swizzling (equivalent to iVector2.yyxy).
        /// </summary>
        public iVector4 ggrg => new iVector4(y, y, x, y);
        
        /// <summary>
        /// Returns iVector2.yyy swizzling.
        /// </summary>
        public iVector3 yyy => new iVector3(y, y, y);
        
        /// <summary>
        /// Returns iVector2.ggg swizzling (equivalent to iVector2.yyy).
        /// </summary>
        public iVector3 ggg => new iVector3(y, y, y);
        
        /// <summary>
        /// Returns iVector2.yyyx swizzling.
        /// </summary>
        public iVector4 yyyx => new iVector4(y, y, y, x);
        
        /// <summary>
        /// Returns iVector2.gggr swizzling (equivalent to iVector2.yyyx).
        /// </summary>
        public iVector4 gggr => new iVector4(y, y, y, x);
        
        /// <summary>
        /// Returns iVector2.yyyy swizzling.
        /// </summary>
        public iVector4 yyyy => new iVector4(y, y, y, y);
        
        /// <summary>
        /// Returns iVector2.gggg swizzling (equivalent to iVector2.yyyy).
        /// </summary>
        public iVector4 gggg => new iVector4(y, y, y, y);

        #endregion

    }
}
