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
    /// Temporary Vector of type decimal with 2 components, used for implementing swizzling for decVector2.
    /// </summary>
    
   
    [StructLayout(LayoutKind.Sequential)]
    public struct swizzle_decVector2
    {

        #region Fields
        
        /// <summary>
        /// x-component
        /// </summary>
        
        internal readonly decimal x;
        
        /// <summary>
        /// y-component
        /// </summary>
        
        internal readonly decimal y;

        #endregion


        #region Constructors
        
        /// <summary>
        /// Constructor for swizzle_decVector2.
        /// </summary>
        internal swizzle_decVector2(decimal x, decimal y)
        {
            this.x = x;
            this.y = y;
        }

        #endregion


        #region Properties
        
        /// <summary>
        /// Returns decVector2.xx swizzling.
        /// </summary>
        public decVector2 xx => new decVector2(x, x);
        
        /// <summary>
        /// Returns decVector2.rr swizzling (equivalent to decVector2.xx).
        /// </summary>
        public decVector2 rr => new decVector2(x, x);
        
        /// <summary>
        /// Returns decVector2.xxx swizzling.
        /// </summary>
        public decVector3 xxx => new decVector3(x, x, x);
        
        /// <summary>
        /// Returns decVector2.rrr swizzling (equivalent to decVector2.xxx).
        /// </summary>
        public decVector3 rrr => new decVector3(x, x, x);
        
        /// <summary>
        /// Returns decVector2.xxxx swizzling.
        /// </summary>
        public decVector4 xxxx => new decVector4(x, x, x, x);
        
        /// <summary>
        /// Returns decVector2.rrrr swizzling (equivalent to decVector2.xxxx).
        /// </summary>
        public decVector4 rrrr => new decVector4(x, x, x, x);
        
        /// <summary>
        /// Returns decVector2.xxxy swizzling.
        /// </summary>
        public decVector4 xxxy => new decVector4(x, x, x, y);
        
        /// <summary>
        /// Returns decVector2.rrrg swizzling (equivalent to decVector2.xxxy).
        /// </summary>
        public decVector4 rrrg => new decVector4(x, x, x, y);
        
        /// <summary>
        /// Returns decVector2.xxy swizzling.
        /// </summary>
        public decVector3 xxy => new decVector3(x, x, y);
        
        /// <summary>
        /// Returns decVector2.rrg swizzling (equivalent to decVector2.xxy).
        /// </summary>
        public decVector3 rrg => new decVector3(x, x, y);
        
        /// <summary>
        /// Returns decVector2.xxyx swizzling.
        /// </summary>
        public decVector4 xxyx => new decVector4(x, x, y, x);
        
        /// <summary>
        /// Returns decVector2.rrgr swizzling (equivalent to decVector2.xxyx).
        /// </summary>
        public decVector4 rrgr => new decVector4(x, x, y, x);
        
        /// <summary>
        /// Returns decVector2.xxyy swizzling.
        /// </summary>
        public decVector4 xxyy => new decVector4(x, x, y, y);
        
        /// <summary>
        /// Returns decVector2.rrgg swizzling (equivalent to decVector2.xxyy).
        /// </summary>
        public decVector4 rrgg => new decVector4(x, x, y, y);
        
        /// <summary>
        /// Returns decVector2.xy swizzling.
        /// </summary>
        public decVector2 xy => new decVector2(x, y);
        
        /// <summary>
        /// Returns decVector2.rg swizzling (equivalent to decVector2.xy).
        /// </summary>
        public decVector2 rg => new decVector2(x, y);
        
        /// <summary>
        /// Returns decVector2.xyx swizzling.
        /// </summary>
        public decVector3 xyx => new decVector3(x, y, x);
        
        /// <summary>
        /// Returns decVector2.rgr swizzling (equivalent to decVector2.xyx).
        /// </summary>
        public decVector3 rgr => new decVector3(x, y, x);
        
        /// <summary>
        /// Returns decVector2.xyxx swizzling.
        /// </summary>
        public decVector4 xyxx => new decVector4(x, y, x, x);
        
        /// <summary>
        /// Returns decVector2.rgrr swizzling (equivalent to decVector2.xyxx).
        /// </summary>
        public decVector4 rgrr => new decVector4(x, y, x, x);
        
        /// <summary>
        /// Returns decVector2.xyxy swizzling.
        /// </summary>
        public decVector4 xyxy => new decVector4(x, y, x, y);
        
        /// <summary>
        /// Returns decVector2.rgrg swizzling (equivalent to decVector2.xyxy).
        /// </summary>
        public decVector4 rgrg => new decVector4(x, y, x, y);
        
        /// <summary>
        /// Returns decVector2.xyy swizzling.
        /// </summary>
        public decVector3 xyy => new decVector3(x, y, y);
        
        /// <summary>
        /// Returns decVector2.rgg swizzling (equivalent to decVector2.xyy).
        /// </summary>
        public decVector3 rgg => new decVector3(x, y, y);
        
        /// <summary>
        /// Returns decVector2.xyyx swizzling.
        /// </summary>
        public decVector4 xyyx => new decVector4(x, y, y, x);
        
        /// <summary>
        /// Returns decVector2.rggr swizzling (equivalent to decVector2.xyyx).
        /// </summary>
        public decVector4 rggr => new decVector4(x, y, y, x);
        
        /// <summary>
        /// Returns decVector2.xyyy swizzling.
        /// </summary>
        public decVector4 xyyy => new decVector4(x, y, y, y);
        
        /// <summary>
        /// Returns decVector2.rggg swizzling (equivalent to decVector2.xyyy).
        /// </summary>
        public decVector4 rggg => new decVector4(x, y, y, y);
        
        /// <summary>
        /// Returns decVector2.yx swizzling.
        /// </summary>
        public decVector2 yx => new decVector2(y, x);
        
        /// <summary>
        /// Returns decVector2.gr swizzling (equivalent to decVector2.yx).
        /// </summary>
        public decVector2 gr => new decVector2(y, x);
        
        /// <summary>
        /// Returns decVector2.yxx swizzling.
        /// </summary>
        public decVector3 yxx => new decVector3(y, x, x);
        
        /// <summary>
        /// Returns decVector2.grr swizzling (equivalent to decVector2.yxx).
        /// </summary>
        public decVector3 grr => new decVector3(y, x, x);
        
        /// <summary>
        /// Returns decVector2.yxxx swizzling.
        /// </summary>
        public decVector4 yxxx => new decVector4(y, x, x, x);
        
        /// <summary>
        /// Returns decVector2.grrr swizzling (equivalent to decVector2.yxxx).
        /// </summary>
        public decVector4 grrr => new decVector4(y, x, x, x);
        
        /// <summary>
        /// Returns decVector2.yxxy swizzling.
        /// </summary>
        public decVector4 yxxy => new decVector4(y, x, x, y);
        
        /// <summary>
        /// Returns decVector2.grrg swizzling (equivalent to decVector2.yxxy).
        /// </summary>
        public decVector4 grrg => new decVector4(y, x, x, y);
        
        /// <summary>
        /// Returns decVector2.yxy swizzling.
        /// </summary>
        public decVector3 yxy => new decVector3(y, x, y);
        
        /// <summary>
        /// Returns decVector2.grg swizzling (equivalent to decVector2.yxy).
        /// </summary>
        public decVector3 grg => new decVector3(y, x, y);
        
        /// <summary>
        /// Returns decVector2.yxyx swizzling.
        /// </summary>
        public decVector4 yxyx => new decVector4(y, x, y, x);
        
        /// <summary>
        /// Returns decVector2.grgr swizzling (equivalent to decVector2.yxyx).
        /// </summary>
        public decVector4 grgr => new decVector4(y, x, y, x);
        
        /// <summary>
        /// Returns decVector2.yxyy swizzling.
        /// </summary>
        public decVector4 yxyy => new decVector4(y, x, y, y);
        
        /// <summary>
        /// Returns decVector2.grgg swizzling (equivalent to decVector2.yxyy).
        /// </summary>
        public decVector4 grgg => new decVector4(y, x, y, y);
        
        /// <summary>
        /// Returns decVector2.yy swizzling.
        /// </summary>
        public decVector2 yy => new decVector2(y, y);
        
        /// <summary>
        /// Returns decVector2.gg swizzling (equivalent to decVector2.yy).
        /// </summary>
        public decVector2 gg => new decVector2(y, y);
        
        /// <summary>
        /// Returns decVector2.yyx swizzling.
        /// </summary>
        public decVector3 yyx => new decVector3(y, y, x);
        
        /// <summary>
        /// Returns decVector2.ggr swizzling (equivalent to decVector2.yyx).
        /// </summary>
        public decVector3 ggr => new decVector3(y, y, x);
        
        /// <summary>
        /// Returns decVector2.yyxx swizzling.
        /// </summary>
        public decVector4 yyxx => new decVector4(y, y, x, x);
        
        /// <summary>
        /// Returns decVector2.ggrr swizzling (equivalent to decVector2.yyxx).
        /// </summary>
        public decVector4 ggrr => new decVector4(y, y, x, x);
        
        /// <summary>
        /// Returns decVector2.yyxy swizzling.
        /// </summary>
        public decVector4 yyxy => new decVector4(y, y, x, y);
        
        /// <summary>
        /// Returns decVector2.ggrg swizzling (equivalent to decVector2.yyxy).
        /// </summary>
        public decVector4 ggrg => new decVector4(y, y, x, y);
        
        /// <summary>
        /// Returns decVector2.yyy swizzling.
        /// </summary>
        public decVector3 yyy => new decVector3(y, y, y);
        
        /// <summary>
        /// Returns decVector2.ggg swizzling (equivalent to decVector2.yyy).
        /// </summary>
        public decVector3 ggg => new decVector3(y, y, y);
        
        /// <summary>
        /// Returns decVector2.yyyx swizzling.
        /// </summary>
        public decVector4 yyyx => new decVector4(y, y, y, x);
        
        /// <summary>
        /// Returns decVector2.gggr swizzling (equivalent to decVector2.yyyx).
        /// </summary>
        public decVector4 gggr => new decVector4(y, y, y, x);
        
        /// <summary>
        /// Returns decVector2.yyyy swizzling.
        /// </summary>
        public decVector4 yyyy => new decVector4(y, y, y, y);
        
        /// <summary>
        /// Returns decVector2.gggg swizzling (equivalent to decVector2.yyyy).
        /// </summary>
        public decVector4 gggg => new decVector4(y, y, y, y);

        #endregion

    }
}
