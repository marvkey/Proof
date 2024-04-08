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
    /// Temporary Vector of type uint with 2 components, used for implementing swizzling for uVector2.
    /// </summary>
    [RegisterCoreClassStruct]
   
    [StructLayout(LayoutKind.Sequential)]
    public struct swizzle_uVector2
    {

        #region Fields
        
        /// <summary>
        /// x-component
        /// </summary>
        
        internal readonly uint x;
        
        /// <summary>
        /// y-component
        /// </summary>
        
        internal readonly uint y;

        #endregion


        #region Constructors
        
        /// <summary>
        /// Constructor for swizzle_uVector2.
        /// </summary>
        internal swizzle_uVector2(uint x, uint y)
        {
            this.x = x;
            this.y = y;
        }

        #endregion


        #region Properties
        
        /// <summary>
        /// Returns uVector2.xx swizzling.
        /// </summary>
        public uVector2 xx => new uVector2(x, x);
        
        /// <summary>
        /// Returns uVector2.rr swizzling (equivalent to uVector2.xx).
        /// </summary>
        public uVector2 rr => new uVector2(x, x);
        
        /// <summary>
        /// Returns uVector2.xxx swizzling.
        /// </summary>
        public uVector3 xxx => new uVector3(x, x, x);
        
        /// <summary>
        /// Returns uVector2.rrr swizzling (equivalent to uVector2.xxx).
        /// </summary>
        public uVector3 rrr => new uVector3(x, x, x);
        
        /// <summary>
        /// Returns uVector2.xxxx swizzling.
        /// </summary>
        public uVector4 xxxx => new uVector4(x, x, x, x);
        
        /// <summary>
        /// Returns uVector2.rrrr swizzling (equivalent to uVector2.xxxx).
        /// </summary>
        public uVector4 rrrr => new uVector4(x, x, x, x);
        
        /// <summary>
        /// Returns uVector2.xxxy swizzling.
        /// </summary>
        public uVector4 xxxy => new uVector4(x, x, x, y);
        
        /// <summary>
        /// Returns uVector2.rrrg swizzling (equivalent to uVector2.xxxy).
        /// </summary>
        public uVector4 rrrg => new uVector4(x, x, x, y);
        
        /// <summary>
        /// Returns uVector2.xxy swizzling.
        /// </summary>
        public uVector3 xxy => new uVector3(x, x, y);
        
        /// <summary>
        /// Returns uVector2.rrg swizzling (equivalent to uVector2.xxy).
        /// </summary>
        public uVector3 rrg => new uVector3(x, x, y);
        
        /// <summary>
        /// Returns uVector2.xxyx swizzling.
        /// </summary>
        public uVector4 xxyx => new uVector4(x, x, y, x);
        
        /// <summary>
        /// Returns uVector2.rrgr swizzling (equivalent to uVector2.xxyx).
        /// </summary>
        public uVector4 rrgr => new uVector4(x, x, y, x);
        
        /// <summary>
        /// Returns uVector2.xxyy swizzling.
        /// </summary>
        public uVector4 xxyy => new uVector4(x, x, y, y);
        
        /// <summary>
        /// Returns uVector2.rrgg swizzling (equivalent to uVector2.xxyy).
        /// </summary>
        public uVector4 rrgg => new uVector4(x, x, y, y);
        
        /// <summary>
        /// Returns uVector2.xy swizzling.
        /// </summary>
        public uVector2 xy => new uVector2(x, y);
        
        /// <summary>
        /// Returns uVector2.rg swizzling (equivalent to uVector2.xy).
        /// </summary>
        public uVector2 rg => new uVector2(x, y);
        
        /// <summary>
        /// Returns uVector2.xyx swizzling.
        /// </summary>
        public uVector3 xyx => new uVector3(x, y, x);
        
        /// <summary>
        /// Returns uVector2.rgr swizzling (equivalent to uVector2.xyx).
        /// </summary>
        public uVector3 rgr => new uVector3(x, y, x);
        
        /// <summary>
        /// Returns uVector2.xyxx swizzling.
        /// </summary>
        public uVector4 xyxx => new uVector4(x, y, x, x);
        
        /// <summary>
        /// Returns uVector2.rgrr swizzling (equivalent to uVector2.xyxx).
        /// </summary>
        public uVector4 rgrr => new uVector4(x, y, x, x);
        
        /// <summary>
        /// Returns uVector2.xyxy swizzling.
        /// </summary>
        public uVector4 xyxy => new uVector4(x, y, x, y);
        
        /// <summary>
        /// Returns uVector2.rgrg swizzling (equivalent to uVector2.xyxy).
        /// </summary>
        public uVector4 rgrg => new uVector4(x, y, x, y);
        
        /// <summary>
        /// Returns uVector2.xyy swizzling.
        /// </summary>
        public uVector3 xyy => new uVector3(x, y, y);
        
        /// <summary>
        /// Returns uVector2.rgg swizzling (equivalent to uVector2.xyy).
        /// </summary>
        public uVector3 rgg => new uVector3(x, y, y);
        
        /// <summary>
        /// Returns uVector2.xyyx swizzling.
        /// </summary>
        public uVector4 xyyx => new uVector4(x, y, y, x);
        
        /// <summary>
        /// Returns uVector2.rggr swizzling (equivalent to uVector2.xyyx).
        /// </summary>
        public uVector4 rggr => new uVector4(x, y, y, x);
        
        /// <summary>
        /// Returns uVector2.xyyy swizzling.
        /// </summary>
        public uVector4 xyyy => new uVector4(x, y, y, y);
        
        /// <summary>
        /// Returns uVector2.rggg swizzling (equivalent to uVector2.xyyy).
        /// </summary>
        public uVector4 rggg => new uVector4(x, y, y, y);
        
        /// <summary>
        /// Returns uVector2.yx swizzling.
        /// </summary>
        public uVector2 yx => new uVector2(y, x);
        
        /// <summary>
        /// Returns uVector2.gr swizzling (equivalent to uVector2.yx).
        /// </summary>
        public uVector2 gr => new uVector2(y, x);
        
        /// <summary>
        /// Returns uVector2.yxx swizzling.
        /// </summary>
        public uVector3 yxx => new uVector3(y, x, x);
        
        /// <summary>
        /// Returns uVector2.grr swizzling (equivalent to uVector2.yxx).
        /// </summary>
        public uVector3 grr => new uVector3(y, x, x);
        
        /// <summary>
        /// Returns uVector2.yxxx swizzling.
        /// </summary>
        public uVector4 yxxx => new uVector4(y, x, x, x);
        
        /// <summary>
        /// Returns uVector2.grrr swizzling (equivalent to uVector2.yxxx).
        /// </summary>
        public uVector4 grrr => new uVector4(y, x, x, x);
        
        /// <summary>
        /// Returns uVector2.yxxy swizzling.
        /// </summary>
        public uVector4 yxxy => new uVector4(y, x, x, y);
        
        /// <summary>
        /// Returns uVector2.grrg swizzling (equivalent to uVector2.yxxy).
        /// </summary>
        public uVector4 grrg => new uVector4(y, x, x, y);
        
        /// <summary>
        /// Returns uVector2.yxy swizzling.
        /// </summary>
        public uVector3 yxy => new uVector3(y, x, y);
        
        /// <summary>
        /// Returns uVector2.grg swizzling (equivalent to uVector2.yxy).
        /// </summary>
        public uVector3 grg => new uVector3(y, x, y);
        
        /// <summary>
        /// Returns uVector2.yxyx swizzling.
        /// </summary>
        public uVector4 yxyx => new uVector4(y, x, y, x);
        
        /// <summary>
        /// Returns uVector2.grgr swizzling (equivalent to uVector2.yxyx).
        /// </summary>
        public uVector4 grgr => new uVector4(y, x, y, x);
        
        /// <summary>
        /// Returns uVector2.yxyy swizzling.
        /// </summary>
        public uVector4 yxyy => new uVector4(y, x, y, y);
        
        /// <summary>
        /// Returns uVector2.grgg swizzling (equivalent to uVector2.yxyy).
        /// </summary>
        public uVector4 grgg => new uVector4(y, x, y, y);
        
        /// <summary>
        /// Returns uVector2.yy swizzling.
        /// </summary>
        public uVector2 yy => new uVector2(y, y);
        
        /// <summary>
        /// Returns uVector2.gg swizzling (equivalent to uVector2.yy).
        /// </summary>
        public uVector2 gg => new uVector2(y, y);
        
        /// <summary>
        /// Returns uVector2.yyx swizzling.
        /// </summary>
        public uVector3 yyx => new uVector3(y, y, x);
        
        /// <summary>
        /// Returns uVector2.ggr swizzling (equivalent to uVector2.yyx).
        /// </summary>
        public uVector3 ggr => new uVector3(y, y, x);
        
        /// <summary>
        /// Returns uVector2.yyxx swizzling.
        /// </summary>
        public uVector4 yyxx => new uVector4(y, y, x, x);
        
        /// <summary>
        /// Returns uVector2.ggrr swizzling (equivalent to uVector2.yyxx).
        /// </summary>
        public uVector4 ggrr => new uVector4(y, y, x, x);
        
        /// <summary>
        /// Returns uVector2.yyxy swizzling.
        /// </summary>
        public uVector4 yyxy => new uVector4(y, y, x, y);
        
        /// <summary>
        /// Returns uVector2.ggrg swizzling (equivalent to uVector2.yyxy).
        /// </summary>
        public uVector4 ggrg => new uVector4(y, y, x, y);
        
        /// <summary>
        /// Returns uVector2.yyy swizzling.
        /// </summary>
        public uVector3 yyy => new uVector3(y, y, y);
        
        /// <summary>
        /// Returns uVector2.ggg swizzling (equivalent to uVector2.yyy).
        /// </summary>
        public uVector3 ggg => new uVector3(y, y, y);
        
        /// <summary>
        /// Returns uVector2.yyyx swizzling.
        /// </summary>
        public uVector4 yyyx => new uVector4(y, y, y, x);
        
        /// <summary>
        /// Returns uVector2.gggr swizzling (equivalent to uVector2.yyyx).
        /// </summary>
        public uVector4 gggr => new uVector4(y, y, y, x);
        
        /// <summary>
        /// Returns uVector2.yyyy swizzling.
        /// </summary>
        public uVector4 yyyy => new uVector4(y, y, y, y);
        
        /// <summary>
        /// Returns uVector2.gggg swizzling (equivalent to uVector2.yyyy).
        /// </summary>
        public uVector4 gggg => new uVector4(y, y, y, y);

        #endregion

    }
}
