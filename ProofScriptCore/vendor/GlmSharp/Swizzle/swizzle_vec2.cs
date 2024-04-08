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
    /// Temporary Vector of type float with 2 components, used for implementing swizzling for Vector2.
    /// </summary>
    [RegisterCoreClassStruct]
   
    [StructLayout(LayoutKind.Sequential)]
    public struct swizzle_Vector2
    {

        #region Fields
        
        /// <summary>
        /// x-component
        /// </summary>
        
        internal readonly float x;
        
        /// <summary>
        /// y-component
        /// </summary>
        
        internal readonly float y;

        #endregion


        #region Constructors
        
        /// <summary>
        /// Constructor for swizzle_Vector2.
        /// </summary>
        internal swizzle_Vector2(float x, float y)
        {
            this.x = x;
            this.y = y;
        }

        #endregion


        #region Properties
        
        /// <summary>
        /// Returns Vector2.xx swizzling.
        /// </summary>
        public Vector2 xx => new Vector2(x, x);
        
        /// <summary>
        /// Returns Vector2.rr swizzling (equivalent to Vector2.xx).
        /// </summary>
        public Vector2 rr => new Vector2(x, x);
        
        /// <summary>
        /// Returns Vector2.xxx swizzling.
        /// </summary>
        public Vector3 xxx => new Vector3(x, x, x);
        
        /// <summary>
        /// Returns Vector2.rrr swizzling (equivalent to Vector2.xxx).
        /// </summary>
        public Vector3 rrr => new Vector3(x, x, x);
        
        /// <summary>
        /// Returns Vector2.xxxx swizzling.
        /// </summary>
        public Vector4 xxxx => new Vector4(x, x, x, x);
        
        /// <summary>
        /// Returns Vector2.rrrr swizzling (equivalent to Vector2.xxxx).
        /// </summary>
        public Vector4 rrrr => new Vector4(x, x, x, x);
        
        /// <summary>
        /// Returns Vector2.xxxy swizzling.
        /// </summary>
        public Vector4 xxxy => new Vector4(x, x, x, y);
        
        /// <summary>
        /// Returns Vector2.rrrg swizzling (equivalent to Vector2.xxxy).
        /// </summary>
        public Vector4 rrrg => new Vector4(x, x, x, y);
        
        /// <summary>
        /// Returns Vector2.xxy swizzling.
        /// </summary>
        public Vector3 xxy => new Vector3(x, x, y);
        
        /// <summary>
        /// Returns Vector2.rrg swizzling (equivalent to Vector2.xxy).
        /// </summary>
        public Vector3 rrg => new Vector3(x, x, y);
        
        /// <summary>
        /// Returns Vector2.xxyx swizzling.
        /// </summary>
        public Vector4 xxyx => new Vector4(x, x, y, x);
        
        /// <summary>
        /// Returns Vector2.rrgr swizzling (equivalent to Vector2.xxyx).
        /// </summary>
        public Vector4 rrgr => new Vector4(x, x, y, x);
        
        /// <summary>
        /// Returns Vector2.xxyy swizzling.
        /// </summary>
        public Vector4 xxyy => new Vector4(x, x, y, y);
        
        /// <summary>
        /// Returns Vector2.rrgg swizzling (equivalent to Vector2.xxyy).
        /// </summary>
        public Vector4 rrgg => new Vector4(x, x, y, y);
        
        /// <summary>
        /// Returns Vector2.xy swizzling.
        /// </summary>
        public Vector2 xy => new Vector2(x, y);
        
        /// <summary>
        /// Returns Vector2.rg swizzling (equivalent to Vector2.xy).
        /// </summary>
        public Vector2 rg => new Vector2(x, y);
        
        /// <summary>
        /// Returns Vector2.xyx swizzling.
        /// </summary>
        public Vector3 xyx => new Vector3(x, y, x);
        
        /// <summary>
        /// Returns Vector2.rgr swizzling (equivalent to Vector2.xyx).
        /// </summary>
        public Vector3 rgr => new Vector3(x, y, x);
        
        /// <summary>
        /// Returns Vector2.xyxx swizzling.
        /// </summary>
        public Vector4 xyxx => new Vector4(x, y, x, x);
        
        /// <summary>
        /// Returns Vector2.rgrr swizzling (equivalent to Vector2.xyxx).
        /// </summary>
        public Vector4 rgrr => new Vector4(x, y, x, x);
        
        /// <summary>
        /// Returns Vector2.xyxy swizzling.
        /// </summary>
        public Vector4 xyxy => new Vector4(x, y, x, y);
        
        /// <summary>
        /// Returns Vector2.rgrg swizzling (equivalent to Vector2.xyxy).
        /// </summary>
        public Vector4 rgrg => new Vector4(x, y, x, y);
        
        /// <summary>
        /// Returns Vector2.xyy swizzling.
        /// </summary>
        public Vector3 xyy => new Vector3(x, y, y);
        
        /// <summary>
        /// Returns Vector2.rgg swizzling (equivalent to Vector2.xyy).
        /// </summary>
        public Vector3 rgg => new Vector3(x, y, y);
        
        /// <summary>
        /// Returns Vector2.xyyx swizzling.
        /// </summary>
        public Vector4 xyyx => new Vector4(x, y, y, x);
        
        /// <summary>
        /// Returns Vector2.rggr swizzling (equivalent to Vector2.xyyx).
        /// </summary>
        public Vector4 rggr => new Vector4(x, y, y, x);
        
        /// <summary>
        /// Returns Vector2.xyyy swizzling.
        /// </summary>
        public Vector4 xyyy => new Vector4(x, y, y, y);
        
        /// <summary>
        /// Returns Vector2.rggg swizzling (equivalent to Vector2.xyyy).
        /// </summary>
        public Vector4 rggg => new Vector4(x, y, y, y);
        
        /// <summary>
        /// Returns Vector2.yx swizzling.
        /// </summary>
        public Vector2 yx => new Vector2(y, x);
        
        /// <summary>
        /// Returns Vector2.gr swizzling (equivalent to Vector2.yx).
        /// </summary>
        public Vector2 gr => new Vector2(y, x);
        
        /// <summary>
        /// Returns Vector2.yxx swizzling.
        /// </summary>
        public Vector3 yxx => new Vector3(y, x, x);
        
        /// <summary>
        /// Returns Vector2.grr swizzling (equivalent to Vector2.yxx).
        /// </summary>
        public Vector3 grr => new Vector3(y, x, x);
        
        /// <summary>
        /// Returns Vector2.yxxx swizzling.
        /// </summary>
        public Vector4 yxxx => new Vector4(y, x, x, x);
        
        /// <summary>
        /// Returns Vector2.grrr swizzling (equivalent to Vector2.yxxx).
        /// </summary>
        public Vector4 grrr => new Vector4(y, x, x, x);
        
        /// <summary>
        /// Returns Vector2.yxxy swizzling.
        /// </summary>
        public Vector4 yxxy => new Vector4(y, x, x, y);
        
        /// <summary>
        /// Returns Vector2.grrg swizzling (equivalent to Vector2.yxxy).
        /// </summary>
        public Vector4 grrg => new Vector4(y, x, x, y);
        
        /// <summary>
        /// Returns Vector2.yxy swizzling.
        /// </summary>
        public Vector3 yxy => new Vector3(y, x, y);
        
        /// <summary>
        /// Returns Vector2.grg swizzling (equivalent to Vector2.yxy).
        /// </summary>
        public Vector3 grg => new Vector3(y, x, y);
        
        /// <summary>
        /// Returns Vector2.yxyx swizzling.
        /// </summary>
        public Vector4 yxyx => new Vector4(y, x, y, x);
        
        /// <summary>
        /// Returns Vector2.grgr swizzling (equivalent to Vector2.yxyx).
        /// </summary>
        public Vector4 grgr => new Vector4(y, x, y, x);
        
        /// <summary>
        /// Returns Vector2.yxyy swizzling.
        /// </summary>
        public Vector4 yxyy => new Vector4(y, x, y, y);
        
        /// <summary>
        /// Returns Vector2.grgg swizzling (equivalent to Vector2.yxyy).
        /// </summary>
        public Vector4 grgg => new Vector4(y, x, y, y);
        
        /// <summary>
        /// Returns Vector2.yy swizzling.
        /// </summary>
        public Vector2 yy => new Vector2(y, y);
        
        /// <summary>
        /// Returns Vector2.gg swizzling (equivalent to Vector2.yy).
        /// </summary>
        public Vector2 gg => new Vector2(y, y);
        
        /// <summary>
        /// Returns Vector2.yyx swizzling.
        /// </summary>
        public Vector3 yyx => new Vector3(y, y, x);
        
        /// <summary>
        /// Returns Vector2.ggr swizzling (equivalent to Vector2.yyx).
        /// </summary>
        public Vector3 ggr => new Vector3(y, y, x);
        
        /// <summary>
        /// Returns Vector2.yyxx swizzling.
        /// </summary>
        public Vector4 yyxx => new Vector4(y, y, x, x);
        
        /// <summary>
        /// Returns Vector2.ggrr swizzling (equivalent to Vector2.yyxx).
        /// </summary>
        public Vector4 ggrr => new Vector4(y, y, x, x);
        
        /// <summary>
        /// Returns Vector2.yyxy swizzling.
        /// </summary>
        public Vector4 yyxy => new Vector4(y, y, x, y);
        
        /// <summary>
        /// Returns Vector2.ggrg swizzling (equivalent to Vector2.yyxy).
        /// </summary>
        public Vector4 ggrg => new Vector4(y, y, x, y);
        
        /// <summary>
        /// Returns Vector2.yyy swizzling.
        /// </summary>
        public Vector3 yyy => new Vector3(y, y, y);
        
        /// <summary>
        /// Returns Vector2.ggg swizzling (equivalent to Vector2.yyy).
        /// </summary>
        public Vector3 ggg => new Vector3(y, y, y);
        
        /// <summary>
        /// Returns Vector2.yyyx swizzling.
        /// </summary>
        public Vector4 yyyx => new Vector4(y, y, y, x);
        
        /// <summary>
        /// Returns Vector2.gggr swizzling (equivalent to Vector2.yyyx).
        /// </summary>
        public Vector4 gggr => new Vector4(y, y, y, x);
        
        /// <summary>
        /// Returns Vector2.yyyy swizzling.
        /// </summary>
        public Vector4 yyyy => new Vector4(y, y, y, y);
        
        /// <summary>
        /// Returns Vector2.gggg swizzling (equivalent to Vector2.yyyy).
        /// </summary>
        public Vector4 gggg => new Vector4(y, y, y, y);

        #endregion

    }
}
