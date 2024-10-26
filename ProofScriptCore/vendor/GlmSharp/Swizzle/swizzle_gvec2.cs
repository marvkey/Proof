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
    /// Temporary Vector of type T with 2 components, used for implementing swizzling for gVector2.
    /// </summary>
    
   
    [StructLayout(LayoutKind.Sequential)]
    public struct swizzle_gVector2<T>
    {

        #region Fields
        
        /// <summary>
        /// x-component
        /// </summary>
        
        internal readonly T x;
        
        /// <summary>
        /// y-component
        /// </summary>
        
        internal readonly T y;

        #endregion


        #region Constructors
        
        /// <summary>
        /// Constructor for swizzle_gVector2.
        /// </summary>
        internal swizzle_gVector2(T x, T y)
        {
            this.x = x;
            this.y = y;
        }

        #endregion


        #region Properties
        
        /// <summary>
        /// Returns gVector2.xx swizzling.
        /// </summary>
        public gVector2<T> xx => new gVector2<T>(x, x);
        
        /// <summary>
        /// Returns gVector2.rr swizzling (equivalent to gVector2.xx).
        /// </summary>
        public gVector2<T> rr => new gVector2<T>(x, x);
        
        /// <summary>
        /// Returns gVector2.xxx swizzling.
        /// </summary>
        public gVector3<T> xxx => new gVector3<T>(x, x, x);
        
        /// <summary>
        /// Returns gVector2.rrr swizzling (equivalent to gVector2.xxx).
        /// </summary>
        public gVector3<T> rrr => new gVector3<T>(x, x, x);
        
        /// <summary>
        /// Returns gVector2.xxxx swizzling.
        /// </summary>
        public gVector4<T> xxxx => new gVector4<T>(x, x, x, x);
        
        /// <summary>
        /// Returns gVector2.rrrr swizzling (equivalent to gVector2.xxxx).
        /// </summary>
        public gVector4<T> rrrr => new gVector4<T>(x, x, x, x);
        
        /// <summary>
        /// Returns gVector2.xxxy swizzling.
        /// </summary>
        public gVector4<T> xxxy => new gVector4<T>(x, x, x, y);
        
        /// <summary>
        /// Returns gVector2.rrrg swizzling (equivalent to gVector2.xxxy).
        /// </summary>
        public gVector4<T> rrrg => new gVector4<T>(x, x, x, y);
        
        /// <summary>
        /// Returns gVector2.xxy swizzling.
        /// </summary>
        public gVector3<T> xxy => new gVector3<T>(x, x, y);
        
        /// <summary>
        /// Returns gVector2.rrg swizzling (equivalent to gVector2.xxy).
        /// </summary>
        public gVector3<T> rrg => new gVector3<T>(x, x, y);
        
        /// <summary>
        /// Returns gVector2.xxyx swizzling.
        /// </summary>
        public gVector4<T> xxyx => new gVector4<T>(x, x, y, x);
        
        /// <summary>
        /// Returns gVector2.rrgr swizzling (equivalent to gVector2.xxyx).
        /// </summary>
        public gVector4<T> rrgr => new gVector4<T>(x, x, y, x);
        
        /// <summary>
        /// Returns gVector2.xxyy swizzling.
        /// </summary>
        public gVector4<T> xxyy => new gVector4<T>(x, x, y, y);
        
        /// <summary>
        /// Returns gVector2.rrgg swizzling (equivalent to gVector2.xxyy).
        /// </summary>
        public gVector4<T> rrgg => new gVector4<T>(x, x, y, y);
        
        /// <summary>
        /// Returns gVector2.xy swizzling.
        /// </summary>
        public gVector2<T> xy => new gVector2<T>(x, y);
        
        /// <summary>
        /// Returns gVector2.rg swizzling (equivalent to gVector2.xy).
        /// </summary>
        public gVector2<T> rg => new gVector2<T>(x, y);
        
        /// <summary>
        /// Returns gVector2.xyx swizzling.
        /// </summary>
        public gVector3<T> xyx => new gVector3<T>(x, y, x);
        
        /// <summary>
        /// Returns gVector2.rgr swizzling (equivalent to gVector2.xyx).
        /// </summary>
        public gVector3<T> rgr => new gVector3<T>(x, y, x);
        
        /// <summary>
        /// Returns gVector2.xyxx swizzling.
        /// </summary>
        public gVector4<T> xyxx => new gVector4<T>(x, y, x, x);
        
        /// <summary>
        /// Returns gVector2.rgrr swizzling (equivalent to gVector2.xyxx).
        /// </summary>
        public gVector4<T> rgrr => new gVector4<T>(x, y, x, x);
        
        /// <summary>
        /// Returns gVector2.xyxy swizzling.
        /// </summary>
        public gVector4<T> xyxy => new gVector4<T>(x, y, x, y);
        
        /// <summary>
        /// Returns gVector2.rgrg swizzling (equivalent to gVector2.xyxy).
        /// </summary>
        public gVector4<T> rgrg => new gVector4<T>(x, y, x, y);
        
        /// <summary>
        /// Returns gVector2.xyy swizzling.
        /// </summary>
        public gVector3<T> xyy => new gVector3<T>(x, y, y);
        
        /// <summary>
        /// Returns gVector2.rgg swizzling (equivalent to gVector2.xyy).
        /// </summary>
        public gVector3<T> rgg => new gVector3<T>(x, y, y);
        
        /// <summary>
        /// Returns gVector2.xyyx swizzling.
        /// </summary>
        public gVector4<T> xyyx => new gVector4<T>(x, y, y, x);
        
        /// <summary>
        /// Returns gVector2.rggr swizzling (equivalent to gVector2.xyyx).
        /// </summary>
        public gVector4<T> rggr => new gVector4<T>(x, y, y, x);
        
        /// <summary>
        /// Returns gVector2.xyyy swizzling.
        /// </summary>
        public gVector4<T> xyyy => new gVector4<T>(x, y, y, y);
        
        /// <summary>
        /// Returns gVector2.rggg swizzling (equivalent to gVector2.xyyy).
        /// </summary>
        public gVector4<T> rggg => new gVector4<T>(x, y, y, y);
        
        /// <summary>
        /// Returns gVector2.yx swizzling.
        /// </summary>
        public gVector2<T> yx => new gVector2<T>(y, x);
        
        /// <summary>
        /// Returns gVector2.gr swizzling (equivalent to gVector2.yx).
        /// </summary>
        public gVector2<T> gr => new gVector2<T>(y, x);
        
        /// <summary>
        /// Returns gVector2.yxx swizzling.
        /// </summary>
        public gVector3<T> yxx => new gVector3<T>(y, x, x);
        
        /// <summary>
        /// Returns gVector2.grr swizzling (equivalent to gVector2.yxx).
        /// </summary>
        public gVector3<T> grr => new gVector3<T>(y, x, x);
        
        /// <summary>
        /// Returns gVector2.yxxx swizzling.
        /// </summary>
        public gVector4<T> yxxx => new gVector4<T>(y, x, x, x);
        
        /// <summary>
        /// Returns gVector2.grrr swizzling (equivalent to gVector2.yxxx).
        /// </summary>
        public gVector4<T> grrr => new gVector4<T>(y, x, x, x);
        
        /// <summary>
        /// Returns gVector2.yxxy swizzling.
        /// </summary>
        public gVector4<T> yxxy => new gVector4<T>(y, x, x, y);
        
        /// <summary>
        /// Returns gVector2.grrg swizzling (equivalent to gVector2.yxxy).
        /// </summary>
        public gVector4<T> grrg => new gVector4<T>(y, x, x, y);
        
        /// <summary>
        /// Returns gVector2.yxy swizzling.
        /// </summary>
        public gVector3<T> yxy => new gVector3<T>(y, x, y);
        
        /// <summary>
        /// Returns gVector2.grg swizzling (equivalent to gVector2.yxy).
        /// </summary>
        public gVector3<T> grg => new gVector3<T>(y, x, y);
        
        /// <summary>
        /// Returns gVector2.yxyx swizzling.
        /// </summary>
        public gVector4<T> yxyx => new gVector4<T>(y, x, y, x);
        
        /// <summary>
        /// Returns gVector2.grgr swizzling (equivalent to gVector2.yxyx).
        /// </summary>
        public gVector4<T> grgr => new gVector4<T>(y, x, y, x);
        
        /// <summary>
        /// Returns gVector2.yxyy swizzling.
        /// </summary>
        public gVector4<T> yxyy => new gVector4<T>(y, x, y, y);
        
        /// <summary>
        /// Returns gVector2.grgg swizzling (equivalent to gVector2.yxyy).
        /// </summary>
        public gVector4<T> grgg => new gVector4<T>(y, x, y, y);
        
        /// <summary>
        /// Returns gVector2.yy swizzling.
        /// </summary>
        public gVector2<T> yy => new gVector2<T>(y, y);
        
        /// <summary>
        /// Returns gVector2.gg swizzling (equivalent to gVector2.yy).
        /// </summary>
        public gVector2<T> gg => new gVector2<T>(y, y);
        
        /// <summary>
        /// Returns gVector2.yyx swizzling.
        /// </summary>
        public gVector3<T> yyx => new gVector3<T>(y, y, x);
        
        /// <summary>
        /// Returns gVector2.ggr swizzling (equivalent to gVector2.yyx).
        /// </summary>
        public gVector3<T> ggr => new gVector3<T>(y, y, x);
        
        /// <summary>
        /// Returns gVector2.yyxx swizzling.
        /// </summary>
        public gVector4<T> yyxx => new gVector4<T>(y, y, x, x);
        
        /// <summary>
        /// Returns gVector2.ggrr swizzling (equivalent to gVector2.yyxx).
        /// </summary>
        public gVector4<T> ggrr => new gVector4<T>(y, y, x, x);
        
        /// <summary>
        /// Returns gVector2.yyxy swizzling.
        /// </summary>
        public gVector4<T> yyxy => new gVector4<T>(y, y, x, y);
        
        /// <summary>
        /// Returns gVector2.ggrg swizzling (equivalent to gVector2.yyxy).
        /// </summary>
        public gVector4<T> ggrg => new gVector4<T>(y, y, x, y);
        
        /// <summary>
        /// Returns gVector2.yyy swizzling.
        /// </summary>
        public gVector3<T> yyy => new gVector3<T>(y, y, y);
        
        /// <summary>
        /// Returns gVector2.ggg swizzling (equivalent to gVector2.yyy).
        /// </summary>
        public gVector3<T> ggg => new gVector3<T>(y, y, y);
        
        /// <summary>
        /// Returns gVector2.yyyx swizzling.
        /// </summary>
        public gVector4<T> yyyx => new gVector4<T>(y, y, y, x);
        
        /// <summary>
        /// Returns gVector2.gggr swizzling (equivalent to gVector2.yyyx).
        /// </summary>
        public gVector4<T> gggr => new gVector4<T>(y, y, y, x);
        
        /// <summary>
        /// Returns gVector2.yyyy swizzling.
        /// </summary>
        public gVector4<T> yyyy => new gVector4<T>(y, y, y, y);
        
        /// <summary>
        /// Returns gVector2.gggg swizzling (equivalent to gVector2.yyyy).
        /// </summary>
        public gVector4<T> gggg => new gVector4<T>(y, y, y, y);

        #endregion

    }
}
