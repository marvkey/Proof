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
    /// Temporary Vector of type Half with 2 components, used for implementing swizzling for hVector2.
    /// </summary>
    
   
    [StructLayout(LayoutKind.Sequential)]
    public struct swizzle_hVector2
    {

        #region Fields
        
        /// <summary>
        /// x-component
        /// </summary>
        
        internal readonly Half x;
        
        /// <summary>
        /// y-component
        /// </summary>
        
        internal readonly Half y;

        #endregion


        #region Constructors
        
        /// <summary>
        /// Constructor for swizzle_hVector2.
        /// </summary>
        internal swizzle_hVector2(Half x, Half y)
        {
            this.x = x;
            this.y = y;
        }

        #endregion


        #region Properties
        
        /// <summary>
        /// Returns hVector2.xx swizzling.
        /// </summary>
        public hVector2 xx => new hVector2(x, x);
        
        /// <summary>
        /// Returns hVector2.rr swizzling (equivalent to hVector2.xx).
        /// </summary>
        public hVector2 rr => new hVector2(x, x);
        
        /// <summary>
        /// Returns hVector2.xxx swizzling.
        /// </summary>
        public hVector3 xxx => new hVector3(x, x, x);
        
        /// <summary>
        /// Returns hVector2.rrr swizzling (equivalent to hVector2.xxx).
        /// </summary>
        public hVector3 rrr => new hVector3(x, x, x);
        
        /// <summary>
        /// Returns hVector2.xxxx swizzling.
        /// </summary>
        public hVector4 xxxx => new hVector4(x, x, x, x);
        
        /// <summary>
        /// Returns hVector2.rrrr swizzling (equivalent to hVector2.xxxx).
        /// </summary>
        public hVector4 rrrr => new hVector4(x, x, x, x);
        
        /// <summary>
        /// Returns hVector2.xxxy swizzling.
        /// </summary>
        public hVector4 xxxy => new hVector4(x, x, x, y);
        
        /// <summary>
        /// Returns hVector2.rrrg swizzling (equivalent to hVector2.xxxy).
        /// </summary>
        public hVector4 rrrg => new hVector4(x, x, x, y);
        
        /// <summary>
        /// Returns hVector2.xxy swizzling.
        /// </summary>
        public hVector3 xxy => new hVector3(x, x, y);
        
        /// <summary>
        /// Returns hVector2.rrg swizzling (equivalent to hVector2.xxy).
        /// </summary>
        public hVector3 rrg => new hVector3(x, x, y);
        
        /// <summary>
        /// Returns hVector2.xxyx swizzling.
        /// </summary>
        public hVector4 xxyx => new hVector4(x, x, y, x);
        
        /// <summary>
        /// Returns hVector2.rrgr swizzling (equivalent to hVector2.xxyx).
        /// </summary>
        public hVector4 rrgr => new hVector4(x, x, y, x);
        
        /// <summary>
        /// Returns hVector2.xxyy swizzling.
        /// </summary>
        public hVector4 xxyy => new hVector4(x, x, y, y);
        
        /// <summary>
        /// Returns hVector2.rrgg swizzling (equivalent to hVector2.xxyy).
        /// </summary>
        public hVector4 rrgg => new hVector4(x, x, y, y);
        
        /// <summary>
        /// Returns hVector2.xy swizzling.
        /// </summary>
        public hVector2 xy => new hVector2(x, y);
        
        /// <summary>
        /// Returns hVector2.rg swizzling (equivalent to hVector2.xy).
        /// </summary>
        public hVector2 rg => new hVector2(x, y);
        
        /// <summary>
        /// Returns hVector2.xyx swizzling.
        /// </summary>
        public hVector3 xyx => new hVector3(x, y, x);
        
        /// <summary>
        /// Returns hVector2.rgr swizzling (equivalent to hVector2.xyx).
        /// </summary>
        public hVector3 rgr => new hVector3(x, y, x);
        
        /// <summary>
        /// Returns hVector2.xyxx swizzling.
        /// </summary>
        public hVector4 xyxx => new hVector4(x, y, x, x);
        
        /// <summary>
        /// Returns hVector2.rgrr swizzling (equivalent to hVector2.xyxx).
        /// </summary>
        public hVector4 rgrr => new hVector4(x, y, x, x);
        
        /// <summary>
        /// Returns hVector2.xyxy swizzling.
        /// </summary>
        public hVector4 xyxy => new hVector4(x, y, x, y);
        
        /// <summary>
        /// Returns hVector2.rgrg swizzling (equivalent to hVector2.xyxy).
        /// </summary>
        public hVector4 rgrg => new hVector4(x, y, x, y);
        
        /// <summary>
        /// Returns hVector2.xyy swizzling.
        /// </summary>
        public hVector3 xyy => new hVector3(x, y, y);
        
        /// <summary>
        /// Returns hVector2.rgg swizzling (equivalent to hVector2.xyy).
        /// </summary>
        public hVector3 rgg => new hVector3(x, y, y);
        
        /// <summary>
        /// Returns hVector2.xyyx swizzling.
        /// </summary>
        public hVector4 xyyx => new hVector4(x, y, y, x);
        
        /// <summary>
        /// Returns hVector2.rggr swizzling (equivalent to hVector2.xyyx).
        /// </summary>
        public hVector4 rggr => new hVector4(x, y, y, x);
        
        /// <summary>
        /// Returns hVector2.xyyy swizzling.
        /// </summary>
        public hVector4 xyyy => new hVector4(x, y, y, y);
        
        /// <summary>
        /// Returns hVector2.rggg swizzling (equivalent to hVector2.xyyy).
        /// </summary>
        public hVector4 rggg => new hVector4(x, y, y, y);
        
        /// <summary>
        /// Returns hVector2.yx swizzling.
        /// </summary>
        public hVector2 yx => new hVector2(y, x);
        
        /// <summary>
        /// Returns hVector2.gr swizzling (equivalent to hVector2.yx).
        /// </summary>
        public hVector2 gr => new hVector2(y, x);
        
        /// <summary>
        /// Returns hVector2.yxx swizzling.
        /// </summary>
        public hVector3 yxx => new hVector3(y, x, x);
        
        /// <summary>
        /// Returns hVector2.grr swizzling (equivalent to hVector2.yxx).
        /// </summary>
        public hVector3 grr => new hVector3(y, x, x);
        
        /// <summary>
        /// Returns hVector2.yxxx swizzling.
        /// </summary>
        public hVector4 yxxx => new hVector4(y, x, x, x);
        
        /// <summary>
        /// Returns hVector2.grrr swizzling (equivalent to hVector2.yxxx).
        /// </summary>
        public hVector4 grrr => new hVector4(y, x, x, x);
        
        /// <summary>
        /// Returns hVector2.yxxy swizzling.
        /// </summary>
        public hVector4 yxxy => new hVector4(y, x, x, y);
        
        /// <summary>
        /// Returns hVector2.grrg swizzling (equivalent to hVector2.yxxy).
        /// </summary>
        public hVector4 grrg => new hVector4(y, x, x, y);
        
        /// <summary>
        /// Returns hVector2.yxy swizzling.
        /// </summary>
        public hVector3 yxy => new hVector3(y, x, y);
        
        /// <summary>
        /// Returns hVector2.grg swizzling (equivalent to hVector2.yxy).
        /// </summary>
        public hVector3 grg => new hVector3(y, x, y);
        
        /// <summary>
        /// Returns hVector2.yxyx swizzling.
        /// </summary>
        public hVector4 yxyx => new hVector4(y, x, y, x);
        
        /// <summary>
        /// Returns hVector2.grgr swizzling (equivalent to hVector2.yxyx).
        /// </summary>
        public hVector4 grgr => new hVector4(y, x, y, x);
        
        /// <summary>
        /// Returns hVector2.yxyy swizzling.
        /// </summary>
        public hVector4 yxyy => new hVector4(y, x, y, y);
        
        /// <summary>
        /// Returns hVector2.grgg swizzling (equivalent to hVector2.yxyy).
        /// </summary>
        public hVector4 grgg => new hVector4(y, x, y, y);
        
        /// <summary>
        /// Returns hVector2.yy swizzling.
        /// </summary>
        public hVector2 yy => new hVector2(y, y);
        
        /// <summary>
        /// Returns hVector2.gg swizzling (equivalent to hVector2.yy).
        /// </summary>
        public hVector2 gg => new hVector2(y, y);
        
        /// <summary>
        /// Returns hVector2.yyx swizzling.
        /// </summary>
        public hVector3 yyx => new hVector3(y, y, x);
        
        /// <summary>
        /// Returns hVector2.ggr swizzling (equivalent to hVector2.yyx).
        /// </summary>
        public hVector3 ggr => new hVector3(y, y, x);
        
        /// <summary>
        /// Returns hVector2.yyxx swizzling.
        /// </summary>
        public hVector4 yyxx => new hVector4(y, y, x, x);
        
        /// <summary>
        /// Returns hVector2.ggrr swizzling (equivalent to hVector2.yyxx).
        /// </summary>
        public hVector4 ggrr => new hVector4(y, y, x, x);
        
        /// <summary>
        /// Returns hVector2.yyxy swizzling.
        /// </summary>
        public hVector4 yyxy => new hVector4(y, y, x, y);
        
        /// <summary>
        /// Returns hVector2.ggrg swizzling (equivalent to hVector2.yyxy).
        /// </summary>
        public hVector4 ggrg => new hVector4(y, y, x, y);
        
        /// <summary>
        /// Returns hVector2.yyy swizzling.
        /// </summary>
        public hVector3 yyy => new hVector3(y, y, y);
        
        /// <summary>
        /// Returns hVector2.ggg swizzling (equivalent to hVector2.yyy).
        /// </summary>
        public hVector3 ggg => new hVector3(y, y, y);
        
        /// <summary>
        /// Returns hVector2.yyyx swizzling.
        /// </summary>
        public hVector4 yyyx => new hVector4(y, y, y, x);
        
        /// <summary>
        /// Returns hVector2.gggr swizzling (equivalent to hVector2.yyyx).
        /// </summary>
        public hVector4 gggr => new hVector4(y, y, y, x);
        
        /// <summary>
        /// Returns hVector2.yyyy swizzling.
        /// </summary>
        public hVector4 yyyy => new hVector4(y, y, y, y);
        
        /// <summary>
        /// Returns hVector2.gggg swizzling (equivalent to hVector2.yyyy).
        /// </summary>
        public hVector4 gggg => new hVector4(y, y, y, y);

        #endregion

    }
}
