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
    /// Temporary Vector of type float with 3 components, used for implementing swizzling for Vector3.
    /// </summary>
    
   
    [StructLayout(LayoutKind.Sequential)]
    public struct swizzle_Vector3
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
        
        /// <summary>
        /// z-component
        /// </summary>
        
        internal readonly float z;

        #endregion


        #region Constructors
        
        /// <summary>
        /// Constructor for swizzle_Vector3.
        /// </summary>
        internal swizzle_Vector3(float x, float y, float z)
        {
            this.x = x;
            this.y = y;
            this.z = z;
        }

        #endregion


        #region Properties
        
        /// <summary>
        /// Returns Vector3.xx swizzling.
        /// </summary>
        public Vector2 xx => new Vector2(x, x);
        
        /// <summary>
        /// Returns Vector3.rr swizzling (equivalent to Vector3.xx).
        /// </summary>
        public Vector2 rr => new Vector2(x, x);
        
        /// <summary>
        /// Returns Vector3.xxx swizzling.
        /// </summary>
        public Vector3 xxx => new Vector3(x, x, x);
        
        /// <summary>
        /// Returns Vector3.rrr swizzling (equivalent to Vector3.xxx).
        /// </summary>
        public Vector3 rrr => new Vector3(x, x, x);
        
        /// <summary>
        /// Returns Vector3.xxxx swizzling.
        /// </summary>
        public Vector4 xxxx => new Vector4(x, x, x, x);
        
        /// <summary>
        /// Returns Vector3.rrrr swizzling (equivalent to Vector3.xxxx).
        /// </summary>
        public Vector4 rrrr => new Vector4(x, x, x, x);
        
        /// <summary>
        /// Returns Vector3.xxxy swizzling.
        /// </summary>
        public Vector4 xxxy => new Vector4(x, x, x, y);
        
        /// <summary>
        /// Returns Vector3.rrrg swizzling (equivalent to Vector3.xxxy).
        /// </summary>
        public Vector4 rrrg => new Vector4(x, x, x, y);
        
        /// <summary>
        /// Returns Vector3.xxxz swizzling.
        /// </summary>
        public Vector4 xxxz => new Vector4(x, x, x, z);
        
        /// <summary>
        /// Returns Vector3.rrrb swizzling (equivalent to Vector3.xxxz).
        /// </summary>
        public Vector4 rrrb => new Vector4(x, x, x, z);
        
        /// <summary>
        /// Returns Vector3.xxy swizzling.
        /// </summary>
        public Vector3 xxy => new Vector3(x, x, y);
        
        /// <summary>
        /// Returns Vector3.rrg swizzling (equivalent to Vector3.xxy).
        /// </summary>
        public Vector3 rrg => new Vector3(x, x, y);
        
        /// <summary>
        /// Returns Vector3.xxyx swizzling.
        /// </summary>
        public Vector4 xxyx => new Vector4(x, x, y, x);
        
        /// <summary>
        /// Returns Vector3.rrgr swizzling (equivalent to Vector3.xxyx).
        /// </summary>
        public Vector4 rrgr => new Vector4(x, x, y, x);
        
        /// <summary>
        /// Returns Vector3.xxyy swizzling.
        /// </summary>
        public Vector4 xxyy => new Vector4(x, x, y, y);
        
        /// <summary>
        /// Returns Vector3.rrgg swizzling (equivalent to Vector3.xxyy).
        /// </summary>
        public Vector4 rrgg => new Vector4(x, x, y, y);
        
        /// <summary>
        /// Returns Vector3.xxyz swizzling.
        /// </summary>
        public Vector4 xxyz => new Vector4(x, x, y, z);
        
        /// <summary>
        /// Returns Vector3.rrgb swizzling (equivalent to Vector3.xxyz).
        /// </summary>
        public Vector4 rrgb => new Vector4(x, x, y, z);
        
        /// <summary>
        /// Returns Vector3.xxz swizzling.
        /// </summary>
        public Vector3 xxz => new Vector3(x, x, z);
        
        /// <summary>
        /// Returns Vector3.rrb swizzling (equivalent to Vector3.xxz).
        /// </summary>
        public Vector3 rrb => new Vector3(x, x, z);
        
        /// <summary>
        /// Returns Vector3.xxzx swizzling.
        /// </summary>
        public Vector4 xxzx => new Vector4(x, x, z, x);
        
        /// <summary>
        /// Returns Vector3.rrbr swizzling (equivalent to Vector3.xxzx).
        /// </summary>
        public Vector4 rrbr => new Vector4(x, x, z, x);
        
        /// <summary>
        /// Returns Vector3.xxzy swizzling.
        /// </summary>
        public Vector4 xxzy => new Vector4(x, x, z, y);
        
        /// <summary>
        /// Returns Vector3.rrbg swizzling (equivalent to Vector3.xxzy).
        /// </summary>
        public Vector4 rrbg => new Vector4(x, x, z, y);
        
        /// <summary>
        /// Returns Vector3.xxzz swizzling.
        /// </summary>
        public Vector4 xxzz => new Vector4(x, x, z, z);
        
        /// <summary>
        /// Returns Vector3.rrbb swizzling (equivalent to Vector3.xxzz).
        /// </summary>
        public Vector4 rrbb => new Vector4(x, x, z, z);
        
        /// <summary>
        /// Returns Vector3.xy swizzling.
        /// </summary>
        public Vector2 xy => new Vector2(x, y);
        
        /// <summary>
        /// Returns Vector3.rg swizzling (equivalent to Vector3.xy).
        /// </summary>
        public Vector2 rg => new Vector2(x, y);
        
        /// <summary>
        /// Returns Vector3.xyx swizzling.
        /// </summary>
        public Vector3 xyx => new Vector3(x, y, x);
        
        /// <summary>
        /// Returns Vector3.rgr swizzling (equivalent to Vector3.xyx).
        /// </summary>
        public Vector3 rgr => new Vector3(x, y, x);
        
        /// <summary>
        /// Returns Vector3.xyxx swizzling.
        /// </summary>
        public Vector4 xyxx => new Vector4(x, y, x, x);
        
        /// <summary>
        /// Returns Vector3.rgrr swizzling (equivalent to Vector3.xyxx).
        /// </summary>
        public Vector4 rgrr => new Vector4(x, y, x, x);
        
        /// <summary>
        /// Returns Vector3.xyxy swizzling.
        /// </summary>
        public Vector4 xyxy => new Vector4(x, y, x, y);
        
        /// <summary>
        /// Returns Vector3.rgrg swizzling (equivalent to Vector3.xyxy).
        /// </summary>
        public Vector4 rgrg => new Vector4(x, y, x, y);
        
        /// <summary>
        /// Returns Vector3.xyxz swizzling.
        /// </summary>
        public Vector4 xyxz => new Vector4(x, y, x, z);
        
        /// <summary>
        /// Returns Vector3.rgrb swizzling (equivalent to Vector3.xyxz).
        /// </summary>
        public Vector4 rgrb => new Vector4(x, y, x, z);
        
        /// <summary>
        /// Returns Vector3.xyy swizzling.
        /// </summary>
        public Vector3 xyy => new Vector3(x, y, y);
        
        /// <summary>
        /// Returns Vector3.rgg swizzling (equivalent to Vector3.xyy).
        /// </summary>
        public Vector3 rgg => new Vector3(x, y, y);
        
        /// <summary>
        /// Returns Vector3.xyyx swizzling.
        /// </summary>
        public Vector4 xyyx => new Vector4(x, y, y, x);
        
        /// <summary>
        /// Returns Vector3.rggr swizzling (equivalent to Vector3.xyyx).
        /// </summary>
        public Vector4 rggr => new Vector4(x, y, y, x);
        
        /// <summary>
        /// Returns Vector3.xyyy swizzling.
        /// </summary>
        public Vector4 xyyy => new Vector4(x, y, y, y);
        
        /// <summary>
        /// Returns Vector3.rggg swizzling (equivalent to Vector3.xyyy).
        /// </summary>
        public Vector4 rggg => new Vector4(x, y, y, y);
        
        /// <summary>
        /// Returns Vector3.xyyz swizzling.
        /// </summary>
        public Vector4 xyyz => new Vector4(x, y, y, z);
        
        /// <summary>
        /// Returns Vector3.rggb swizzling (equivalent to Vector3.xyyz).
        /// </summary>
        public Vector4 rggb => new Vector4(x, y, y, z);
        
        /// <summary>
        /// Returns Vector3.xyz swizzling.
        /// </summary>
        public Vector3 xyz => new Vector3(x, y, z);
        
        /// <summary>
        /// Returns Vector3.rgb swizzling (equivalent to Vector3.xyz).
        /// </summary>
        public Vector3 rgb => new Vector3(x, y, z);
        
        /// <summary>
        /// Returns Vector3.xyzx swizzling.
        /// </summary>
        public Vector4 xyzx => new Vector4(x, y, z, x);
        
        /// <summary>
        /// Returns Vector3.rgbr swizzling (equivalent to Vector3.xyzx).
        /// </summary>
        public Vector4 rgbr => new Vector4(x, y, z, x);
        
        /// <summary>
        /// Returns Vector3.xyzy swizzling.
        /// </summary>
        public Vector4 xyzy => new Vector4(x, y, z, y);
        
        /// <summary>
        /// Returns Vector3.rgbg swizzling (equivalent to Vector3.xyzy).
        /// </summary>
        public Vector4 rgbg => new Vector4(x, y, z, y);
        
        /// <summary>
        /// Returns Vector3.xyzz swizzling.
        /// </summary>
        public Vector4 xyzz => new Vector4(x, y, z, z);
        
        /// <summary>
        /// Returns Vector3.rgbb swizzling (equivalent to Vector3.xyzz).
        /// </summary>
        public Vector4 rgbb => new Vector4(x, y, z, z);
        
        /// <summary>
        /// Returns Vector3.xz swizzling.
        /// </summary>
        public Vector2 xz => new Vector2(x, z);
        
        /// <summary>
        /// Returns Vector3.rb swizzling (equivalent to Vector3.xz).
        /// </summary>
        public Vector2 rb => new Vector2(x, z);
        
        /// <summary>
        /// Returns Vector3.xzx swizzling.
        /// </summary>
        public Vector3 xzx => new Vector3(x, z, x);
        
        /// <summary>
        /// Returns Vector3.rbr swizzling (equivalent to Vector3.xzx).
        /// </summary>
        public Vector3 rbr => new Vector3(x, z, x);
        
        /// <summary>
        /// Returns Vector3.xzxx swizzling.
        /// </summary>
        public Vector4 xzxx => new Vector4(x, z, x, x);
        
        /// <summary>
        /// Returns Vector3.rbrr swizzling (equivalent to Vector3.xzxx).
        /// </summary>
        public Vector4 rbrr => new Vector4(x, z, x, x);
        
        /// <summary>
        /// Returns Vector3.xzxy swizzling.
        /// </summary>
        public Vector4 xzxy => new Vector4(x, z, x, y);
        
        /// <summary>
        /// Returns Vector3.rbrg swizzling (equivalent to Vector3.xzxy).
        /// </summary>
        public Vector4 rbrg => new Vector4(x, z, x, y);
        
        /// <summary>
        /// Returns Vector3.xzxz swizzling.
        /// </summary>
        public Vector4 xzxz => new Vector4(x, z, x, z);
        
        /// <summary>
        /// Returns Vector3.rbrb swizzling (equivalent to Vector3.xzxz).
        /// </summary>
        public Vector4 rbrb => new Vector4(x, z, x, z);
        
        /// <summary>
        /// Returns Vector3.xzy swizzling.
        /// </summary>
        public Vector3 xzy => new Vector3(x, z, y);
        
        /// <summary>
        /// Returns Vector3.rbg swizzling (equivalent to Vector3.xzy).
        /// </summary>
        public Vector3 rbg => new Vector3(x, z, y);
        
        /// <summary>
        /// Returns Vector3.xzyx swizzling.
        /// </summary>
        public Vector4 xzyx => new Vector4(x, z, y, x);
        
        /// <summary>
        /// Returns Vector3.rbgr swizzling (equivalent to Vector3.xzyx).
        /// </summary>
        public Vector4 rbgr => new Vector4(x, z, y, x);
        
        /// <summary>
        /// Returns Vector3.xzyy swizzling.
        /// </summary>
        public Vector4 xzyy => new Vector4(x, z, y, y);
        
        /// <summary>
        /// Returns Vector3.rbgg swizzling (equivalent to Vector3.xzyy).
        /// </summary>
        public Vector4 rbgg => new Vector4(x, z, y, y);
        
        /// <summary>
        /// Returns Vector3.xzyz swizzling.
        /// </summary>
        public Vector4 xzyz => new Vector4(x, z, y, z);
        
        /// <summary>
        /// Returns Vector3.rbgb swizzling (equivalent to Vector3.xzyz).
        /// </summary>
        public Vector4 rbgb => new Vector4(x, z, y, z);
        
        /// <summary>
        /// Returns Vector3.xzz swizzling.
        /// </summary>
        public Vector3 xzz => new Vector3(x, z, z);
        
        /// <summary>
        /// Returns Vector3.rbb swizzling (equivalent to Vector3.xzz).
        /// </summary>
        public Vector3 rbb => new Vector3(x, z, z);
        
        /// <summary>
        /// Returns Vector3.xzzx swizzling.
        /// </summary>
        public Vector4 xzzx => new Vector4(x, z, z, x);
        
        /// <summary>
        /// Returns Vector3.rbbr swizzling (equivalent to Vector3.xzzx).
        /// </summary>
        public Vector4 rbbr => new Vector4(x, z, z, x);
        
        /// <summary>
        /// Returns Vector3.xzzy swizzling.
        /// </summary>
        public Vector4 xzzy => new Vector4(x, z, z, y);
        
        /// <summary>
        /// Returns Vector3.rbbg swizzling (equivalent to Vector3.xzzy).
        /// </summary>
        public Vector4 rbbg => new Vector4(x, z, z, y);
        
        /// <summary>
        /// Returns Vector3.xzzz swizzling.
        /// </summary>
        public Vector4 xzzz => new Vector4(x, z, z, z);
        
        /// <summary>
        /// Returns Vector3.rbbb swizzling (equivalent to Vector3.xzzz).
        /// </summary>
        public Vector4 rbbb => new Vector4(x, z, z, z);
        
        /// <summary>
        /// Returns Vector3.yx swizzling.
        /// </summary>
        public Vector2 yx => new Vector2(y, x);
        
        /// <summary>
        /// Returns Vector3.gr swizzling (equivalent to Vector3.yx).
        /// </summary>
        public Vector2 gr => new Vector2(y, x);
        
        /// <summary>
        /// Returns Vector3.yxx swizzling.
        /// </summary>
        public Vector3 yxx => new Vector3(y, x, x);
        
        /// <summary>
        /// Returns Vector3.grr swizzling (equivalent to Vector3.yxx).
        /// </summary>
        public Vector3 grr => new Vector3(y, x, x);
        
        /// <summary>
        /// Returns Vector3.yxxx swizzling.
        /// </summary>
        public Vector4 yxxx => new Vector4(y, x, x, x);
        
        /// <summary>
        /// Returns Vector3.grrr swizzling (equivalent to Vector3.yxxx).
        /// </summary>
        public Vector4 grrr => new Vector4(y, x, x, x);
        
        /// <summary>
        /// Returns Vector3.yxxy swizzling.
        /// </summary>
        public Vector4 yxxy => new Vector4(y, x, x, y);
        
        /// <summary>
        /// Returns Vector3.grrg swizzling (equivalent to Vector3.yxxy).
        /// </summary>
        public Vector4 grrg => new Vector4(y, x, x, y);
        
        /// <summary>
        /// Returns Vector3.yxxz swizzling.
        /// </summary>
        public Vector4 yxxz => new Vector4(y, x, x, z);
        
        /// <summary>
        /// Returns Vector3.grrb swizzling (equivalent to Vector3.yxxz).
        /// </summary>
        public Vector4 grrb => new Vector4(y, x, x, z);
        
        /// <summary>
        /// Returns Vector3.yxy swizzling.
        /// </summary>
        public Vector3 yxy => new Vector3(y, x, y);
        
        /// <summary>
        /// Returns Vector3.grg swizzling (equivalent to Vector3.yxy).
        /// </summary>
        public Vector3 grg => new Vector3(y, x, y);
        
        /// <summary>
        /// Returns Vector3.yxyx swizzling.
        /// </summary>
        public Vector4 yxyx => new Vector4(y, x, y, x);
        
        /// <summary>
        /// Returns Vector3.grgr swizzling (equivalent to Vector3.yxyx).
        /// </summary>
        public Vector4 grgr => new Vector4(y, x, y, x);
        
        /// <summary>
        /// Returns Vector3.yxyy swizzling.
        /// </summary>
        public Vector4 yxyy => new Vector4(y, x, y, y);
        
        /// <summary>
        /// Returns Vector3.grgg swizzling (equivalent to Vector3.yxyy).
        /// </summary>
        public Vector4 grgg => new Vector4(y, x, y, y);
        
        /// <summary>
        /// Returns Vector3.yxyz swizzling.
        /// </summary>
        public Vector4 yxyz => new Vector4(y, x, y, z);
        
        /// <summary>
        /// Returns Vector3.grgb swizzling (equivalent to Vector3.yxyz).
        /// </summary>
        public Vector4 grgb => new Vector4(y, x, y, z);
        
        /// <summary>
        /// Returns Vector3.yxz swizzling.
        /// </summary>
        public Vector3 yxz => new Vector3(y, x, z);
        
        /// <summary>
        /// Returns Vector3.grb swizzling (equivalent to Vector3.yxz).
        /// </summary>
        public Vector3 grb => new Vector3(y, x, z);
        
        /// <summary>
        /// Returns Vector3.yxzx swizzling.
        /// </summary>
        public Vector4 yxzx => new Vector4(y, x, z, x);
        
        /// <summary>
        /// Returns Vector3.grbr swizzling (equivalent to Vector3.yxzx).
        /// </summary>
        public Vector4 grbr => new Vector4(y, x, z, x);
        
        /// <summary>
        /// Returns Vector3.yxzy swizzling.
        /// </summary>
        public Vector4 yxzy => new Vector4(y, x, z, y);
        
        /// <summary>
        /// Returns Vector3.grbg swizzling (equivalent to Vector3.yxzy).
        /// </summary>
        public Vector4 grbg => new Vector4(y, x, z, y);
        
        /// <summary>
        /// Returns Vector3.yxzz swizzling.
        /// </summary>
        public Vector4 yxzz => new Vector4(y, x, z, z);
        
        /// <summary>
        /// Returns Vector3.grbb swizzling (equivalent to Vector3.yxzz).
        /// </summary>
        public Vector4 grbb => new Vector4(y, x, z, z);
        
        /// <summary>
        /// Returns Vector3.yy swizzling.
        /// </summary>
        public Vector2 yy => new Vector2(y, y);
        
        /// <summary>
        /// Returns Vector3.gg swizzling (equivalent to Vector3.yy).
        /// </summary>
        public Vector2 gg => new Vector2(y, y);
        
        /// <summary>
        /// Returns Vector3.yyx swizzling.
        /// </summary>
        public Vector3 yyx => new Vector3(y, y, x);
        
        /// <summary>
        /// Returns Vector3.ggr swizzling (equivalent to Vector3.yyx).
        /// </summary>
        public Vector3 ggr => new Vector3(y, y, x);
        
        /// <summary>
        /// Returns Vector3.yyxx swizzling.
        /// </summary>
        public Vector4 yyxx => new Vector4(y, y, x, x);
        
        /// <summary>
        /// Returns Vector3.ggrr swizzling (equivalent to Vector3.yyxx).
        /// </summary>
        public Vector4 ggrr => new Vector4(y, y, x, x);
        
        /// <summary>
        /// Returns Vector3.yyxy swizzling.
        /// </summary>
        public Vector4 yyxy => new Vector4(y, y, x, y);
        
        /// <summary>
        /// Returns Vector3.ggrg swizzling (equivalent to Vector3.yyxy).
        /// </summary>
        public Vector4 ggrg => new Vector4(y, y, x, y);
        
        /// <summary>
        /// Returns Vector3.yyxz swizzling.
        /// </summary>
        public Vector4 yyxz => new Vector4(y, y, x, z);
        
        /// <summary>
        /// Returns Vector3.ggrb swizzling (equivalent to Vector3.yyxz).
        /// </summary>
        public Vector4 ggrb => new Vector4(y, y, x, z);
        
        /// <summary>
        /// Returns Vector3.yyy swizzling.
        /// </summary>
        public Vector3 yyy => new Vector3(y, y, y);
        
        /// <summary>
        /// Returns Vector3.ggg swizzling (equivalent to Vector3.yyy).
        /// </summary>
        public Vector3 ggg => new Vector3(y, y, y);
        
        /// <summary>
        /// Returns Vector3.yyyx swizzling.
        /// </summary>
        public Vector4 yyyx => new Vector4(y, y, y, x);
        
        /// <summary>
        /// Returns Vector3.gggr swizzling (equivalent to Vector3.yyyx).
        /// </summary>
        public Vector4 gggr => new Vector4(y, y, y, x);
        
        /// <summary>
        /// Returns Vector3.yyyy swizzling.
        /// </summary>
        public Vector4 yyyy => new Vector4(y, y, y, y);
        
        /// <summary>
        /// Returns Vector3.gggg swizzling (equivalent to Vector3.yyyy).
        /// </summary>
        public Vector4 gggg => new Vector4(y, y, y, y);
        
        /// <summary>
        /// Returns Vector3.yyyz swizzling.
        /// </summary>
        public Vector4 yyyz => new Vector4(y, y, y, z);
        
        /// <summary>
        /// Returns Vector3.gggb swizzling (equivalent to Vector3.yyyz).
        /// </summary>
        public Vector4 gggb => new Vector4(y, y, y, z);
        
        /// <summary>
        /// Returns Vector3.yyz swizzling.
        /// </summary>
        public Vector3 yyz => new Vector3(y, y, z);
        
        /// <summary>
        /// Returns Vector3.ggb swizzling (equivalent to Vector3.yyz).
        /// </summary>
        public Vector3 ggb => new Vector3(y, y, z);
        
        /// <summary>
        /// Returns Vector3.yyzx swizzling.
        /// </summary>
        public Vector4 yyzx => new Vector4(y, y, z, x);
        
        /// <summary>
        /// Returns Vector3.ggbr swizzling (equivalent to Vector3.yyzx).
        /// </summary>
        public Vector4 ggbr => new Vector4(y, y, z, x);
        
        /// <summary>
        /// Returns Vector3.yyzy swizzling.
        /// </summary>
        public Vector4 yyzy => new Vector4(y, y, z, y);
        
        /// <summary>
        /// Returns Vector3.ggbg swizzling (equivalent to Vector3.yyzy).
        /// </summary>
        public Vector4 ggbg => new Vector4(y, y, z, y);
        
        /// <summary>
        /// Returns Vector3.yyzz swizzling.
        /// </summary>
        public Vector4 yyzz => new Vector4(y, y, z, z);
        
        /// <summary>
        /// Returns Vector3.ggbb swizzling (equivalent to Vector3.yyzz).
        /// </summary>
        public Vector4 ggbb => new Vector4(y, y, z, z);
        
        /// <summary>
        /// Returns Vector3.yz swizzling.
        /// </summary>
        public Vector2 yz => new Vector2(y, z);
        
        /// <summary>
        /// Returns Vector3.gb swizzling (equivalent to Vector3.yz).
        /// </summary>
        public Vector2 gb => new Vector2(y, z);
        
        /// <summary>
        /// Returns Vector3.yzx swizzling.
        /// </summary>
        public Vector3 yzx => new Vector3(y, z, x);
        
        /// <summary>
        /// Returns Vector3.gbr swizzling (equivalent to Vector3.yzx).
        /// </summary>
        public Vector3 gbr => new Vector3(y, z, x);
        
        /// <summary>
        /// Returns Vector3.yzxx swizzling.
        /// </summary>
        public Vector4 yzxx => new Vector4(y, z, x, x);
        
        /// <summary>
        /// Returns Vector3.gbrr swizzling (equivalent to Vector3.yzxx).
        /// </summary>
        public Vector4 gbrr => new Vector4(y, z, x, x);
        
        /// <summary>
        /// Returns Vector3.yzxy swizzling.
        /// </summary>
        public Vector4 yzxy => new Vector4(y, z, x, y);
        
        /// <summary>
        /// Returns Vector3.gbrg swizzling (equivalent to Vector3.yzxy).
        /// </summary>
        public Vector4 gbrg => new Vector4(y, z, x, y);
        
        /// <summary>
        /// Returns Vector3.yzxz swizzling.
        /// </summary>
        public Vector4 yzxz => new Vector4(y, z, x, z);
        
        /// <summary>
        /// Returns Vector3.gbrb swizzling (equivalent to Vector3.yzxz).
        /// </summary>
        public Vector4 gbrb => new Vector4(y, z, x, z);
        
        /// <summary>
        /// Returns Vector3.yzy swizzling.
        /// </summary>
        public Vector3 yzy => new Vector3(y, z, y);
        
        /// <summary>
        /// Returns Vector3.gbg swizzling (equivalent to Vector3.yzy).
        /// </summary>
        public Vector3 gbg => new Vector3(y, z, y);
        
        /// <summary>
        /// Returns Vector3.yzyx swizzling.
        /// </summary>
        public Vector4 yzyx => new Vector4(y, z, y, x);
        
        /// <summary>
        /// Returns Vector3.gbgr swizzling (equivalent to Vector3.yzyx).
        /// </summary>
        public Vector4 gbgr => new Vector4(y, z, y, x);
        
        /// <summary>
        /// Returns Vector3.yzyy swizzling.
        /// </summary>
        public Vector4 yzyy => new Vector4(y, z, y, y);
        
        /// <summary>
        /// Returns Vector3.gbgg swizzling (equivalent to Vector3.yzyy).
        /// </summary>
        public Vector4 gbgg => new Vector4(y, z, y, y);
        
        /// <summary>
        /// Returns Vector3.yzyz swizzling.
        /// </summary>
        public Vector4 yzyz => new Vector4(y, z, y, z);
        
        /// <summary>
        /// Returns Vector3.gbgb swizzling (equivalent to Vector3.yzyz).
        /// </summary>
        public Vector4 gbgb => new Vector4(y, z, y, z);
        
        /// <summary>
        /// Returns Vector3.yzz swizzling.
        /// </summary>
        public Vector3 yzz => new Vector3(y, z, z);
        
        /// <summary>
        /// Returns Vector3.gbb swizzling (equivalent to Vector3.yzz).
        /// </summary>
        public Vector3 gbb => new Vector3(y, z, z);
        
        /// <summary>
        /// Returns Vector3.yzzx swizzling.
        /// </summary>
        public Vector4 yzzx => new Vector4(y, z, z, x);
        
        /// <summary>
        /// Returns Vector3.gbbr swizzling (equivalent to Vector3.yzzx).
        /// </summary>
        public Vector4 gbbr => new Vector4(y, z, z, x);
        
        /// <summary>
        /// Returns Vector3.yzzy swizzling.
        /// </summary>
        public Vector4 yzzy => new Vector4(y, z, z, y);
        
        /// <summary>
        /// Returns Vector3.gbbg swizzling (equivalent to Vector3.yzzy).
        /// </summary>
        public Vector4 gbbg => new Vector4(y, z, z, y);
        
        /// <summary>
        /// Returns Vector3.yzzz swizzling.
        /// </summary>
        public Vector4 yzzz => new Vector4(y, z, z, z);
        
        /// <summary>
        /// Returns Vector3.gbbb swizzling (equivalent to Vector3.yzzz).
        /// </summary>
        public Vector4 gbbb => new Vector4(y, z, z, z);
        
        /// <summary>
        /// Returns Vector3.zx swizzling.
        /// </summary>
        public Vector2 zx => new Vector2(z, x);
        
        /// <summary>
        /// Returns Vector3.br swizzling (equivalent to Vector3.zx).
        /// </summary>
        public Vector2 br => new Vector2(z, x);
        
        /// <summary>
        /// Returns Vector3.zxx swizzling.
        /// </summary>
        public Vector3 zxx => new Vector3(z, x, x);
        
        /// <summary>
        /// Returns Vector3.brr swizzling (equivalent to Vector3.zxx).
        /// </summary>
        public Vector3 brr => new Vector3(z, x, x);
        
        /// <summary>
        /// Returns Vector3.zxxx swizzling.
        /// </summary>
        public Vector4 zxxx => new Vector4(z, x, x, x);
        
        /// <summary>
        /// Returns Vector3.brrr swizzling (equivalent to Vector3.zxxx).
        /// </summary>
        public Vector4 brrr => new Vector4(z, x, x, x);
        
        /// <summary>
        /// Returns Vector3.zxxy swizzling.
        /// </summary>
        public Vector4 zxxy => new Vector4(z, x, x, y);
        
        /// <summary>
        /// Returns Vector3.brrg swizzling (equivalent to Vector3.zxxy).
        /// </summary>
        public Vector4 brrg => new Vector4(z, x, x, y);
        
        /// <summary>
        /// Returns Vector3.zxxz swizzling.
        /// </summary>
        public Vector4 zxxz => new Vector4(z, x, x, z);
        
        /// <summary>
        /// Returns Vector3.brrb swizzling (equivalent to Vector3.zxxz).
        /// </summary>
        public Vector4 brrb => new Vector4(z, x, x, z);
        
        /// <summary>
        /// Returns Vector3.zxy swizzling.
        /// </summary>
        public Vector3 zxy => new Vector3(z, x, y);
        
        /// <summary>
        /// Returns Vector3.brg swizzling (equivalent to Vector3.zxy).
        /// </summary>
        public Vector3 brg => new Vector3(z, x, y);
        
        /// <summary>
        /// Returns Vector3.zxyx swizzling.
        /// </summary>
        public Vector4 zxyx => new Vector4(z, x, y, x);
        
        /// <summary>
        /// Returns Vector3.brgr swizzling (equivalent to Vector3.zxyx).
        /// </summary>
        public Vector4 brgr => new Vector4(z, x, y, x);
        
        /// <summary>
        /// Returns Vector3.zxyy swizzling.
        /// </summary>
        public Vector4 zxyy => new Vector4(z, x, y, y);
        
        /// <summary>
        /// Returns Vector3.brgg swizzling (equivalent to Vector3.zxyy).
        /// </summary>
        public Vector4 brgg => new Vector4(z, x, y, y);
        
        /// <summary>
        /// Returns Vector3.zxyz swizzling.
        /// </summary>
        public Vector4 zxyz => new Vector4(z, x, y, z);
        
        /// <summary>
        /// Returns Vector3.brgb swizzling (equivalent to Vector3.zxyz).
        /// </summary>
        public Vector4 brgb => new Vector4(z, x, y, z);
        
        /// <summary>
        /// Returns Vector3.zxz swizzling.
        /// </summary>
        public Vector3 zxz => new Vector3(z, x, z);
        
        /// <summary>
        /// Returns Vector3.brb swizzling (equivalent to Vector3.zxz).
        /// </summary>
        public Vector3 brb => new Vector3(z, x, z);
        
        /// <summary>
        /// Returns Vector3.zxzx swizzling.
        /// </summary>
        public Vector4 zxzx => new Vector4(z, x, z, x);
        
        /// <summary>
        /// Returns Vector3.brbr swizzling (equivalent to Vector3.zxzx).
        /// </summary>
        public Vector4 brbr => new Vector4(z, x, z, x);
        
        /// <summary>
        /// Returns Vector3.zxzy swizzling.
        /// </summary>
        public Vector4 zxzy => new Vector4(z, x, z, y);
        
        /// <summary>
        /// Returns Vector3.brbg swizzling (equivalent to Vector3.zxzy).
        /// </summary>
        public Vector4 brbg => new Vector4(z, x, z, y);
        
        /// <summary>
        /// Returns Vector3.zxzz swizzling.
        /// </summary>
        public Vector4 zxzz => new Vector4(z, x, z, z);
        
        /// <summary>
        /// Returns Vector3.brbb swizzling (equivalent to Vector3.zxzz).
        /// </summary>
        public Vector4 brbb => new Vector4(z, x, z, z);
        
        /// <summary>
        /// Returns Vector3.zy swizzling.
        /// </summary>
        public Vector2 zy => new Vector2(z, y);
        
        /// <summary>
        /// Returns Vector3.bg swizzling (equivalent to Vector3.zy).
        /// </summary>
        public Vector2 bg => new Vector2(z, y);
        
        /// <summary>
        /// Returns Vector3.zyx swizzling.
        /// </summary>
        public Vector3 zyx => new Vector3(z, y, x);
        
        /// <summary>
        /// Returns Vector3.bgr swizzling (equivalent to Vector3.zyx).
        /// </summary>
        public Vector3 bgr => new Vector3(z, y, x);
        
        /// <summary>
        /// Returns Vector3.zyxx swizzling.
        /// </summary>
        public Vector4 zyxx => new Vector4(z, y, x, x);
        
        /// <summary>
        /// Returns Vector3.bgrr swizzling (equivalent to Vector3.zyxx).
        /// </summary>
        public Vector4 bgrr => new Vector4(z, y, x, x);
        
        /// <summary>
        /// Returns Vector3.zyxy swizzling.
        /// </summary>
        public Vector4 zyxy => new Vector4(z, y, x, y);
        
        /// <summary>
        /// Returns Vector3.bgrg swizzling (equivalent to Vector3.zyxy).
        /// </summary>
        public Vector4 bgrg => new Vector4(z, y, x, y);
        
        /// <summary>
        /// Returns Vector3.zyxz swizzling.
        /// </summary>
        public Vector4 zyxz => new Vector4(z, y, x, z);
        
        /// <summary>
        /// Returns Vector3.bgrb swizzling (equivalent to Vector3.zyxz).
        /// </summary>
        public Vector4 bgrb => new Vector4(z, y, x, z);
        
        /// <summary>
        /// Returns Vector3.zyy swizzling.
        /// </summary>
        public Vector3 zyy => new Vector3(z, y, y);
        
        /// <summary>
        /// Returns Vector3.bgg swizzling (equivalent to Vector3.zyy).
        /// </summary>
        public Vector3 bgg => new Vector3(z, y, y);
        
        /// <summary>
        /// Returns Vector3.zyyx swizzling.
        /// </summary>
        public Vector4 zyyx => new Vector4(z, y, y, x);
        
        /// <summary>
        /// Returns Vector3.bggr swizzling (equivalent to Vector3.zyyx).
        /// </summary>
        public Vector4 bggr => new Vector4(z, y, y, x);
        
        /// <summary>
        /// Returns Vector3.zyyy swizzling.
        /// </summary>
        public Vector4 zyyy => new Vector4(z, y, y, y);
        
        /// <summary>
        /// Returns Vector3.bggg swizzling (equivalent to Vector3.zyyy).
        /// </summary>
        public Vector4 bggg => new Vector4(z, y, y, y);
        
        /// <summary>
        /// Returns Vector3.zyyz swizzling.
        /// </summary>
        public Vector4 zyyz => new Vector4(z, y, y, z);
        
        /// <summary>
        /// Returns Vector3.bggb swizzling (equivalent to Vector3.zyyz).
        /// </summary>
        public Vector4 bggb => new Vector4(z, y, y, z);
        
        /// <summary>
        /// Returns Vector3.zyz swizzling.
        /// </summary>
        public Vector3 zyz => new Vector3(z, y, z);
        
        /// <summary>
        /// Returns Vector3.bgb swizzling (equivalent to Vector3.zyz).
        /// </summary>
        public Vector3 bgb => new Vector3(z, y, z);
        
        /// <summary>
        /// Returns Vector3.zyzx swizzling.
        /// </summary>
        public Vector4 zyzx => new Vector4(z, y, z, x);
        
        /// <summary>
        /// Returns Vector3.bgbr swizzling (equivalent to Vector3.zyzx).
        /// </summary>
        public Vector4 bgbr => new Vector4(z, y, z, x);
        
        /// <summary>
        /// Returns Vector3.zyzy swizzling.
        /// </summary>
        public Vector4 zyzy => new Vector4(z, y, z, y);
        
        /// <summary>
        /// Returns Vector3.bgbg swizzling (equivalent to Vector3.zyzy).
        /// </summary>
        public Vector4 bgbg => new Vector4(z, y, z, y);
        
        /// <summary>
        /// Returns Vector3.zyzz swizzling.
        /// </summary>
        public Vector4 zyzz => new Vector4(z, y, z, z);
        
        /// <summary>
        /// Returns Vector3.bgbb swizzling (equivalent to Vector3.zyzz).
        /// </summary>
        public Vector4 bgbb => new Vector4(z, y, z, z);
        
        /// <summary>
        /// Returns Vector3.zz swizzling.
        /// </summary>
        public Vector2 zz => new Vector2(z, z);
        
        /// <summary>
        /// Returns Vector3.bb swizzling (equivalent to Vector3.zz).
        /// </summary>
        public Vector2 bb => new Vector2(z, z);
        
        /// <summary>
        /// Returns Vector3.zzx swizzling.
        /// </summary>
        public Vector3 zzx => new Vector3(z, z, x);
        
        /// <summary>
        /// Returns Vector3.bbr swizzling (equivalent to Vector3.zzx).
        /// </summary>
        public Vector3 bbr => new Vector3(z, z, x);
        
        /// <summary>
        /// Returns Vector3.zzxx swizzling.
        /// </summary>
        public Vector4 zzxx => new Vector4(z, z, x, x);
        
        /// <summary>
        /// Returns Vector3.bbrr swizzling (equivalent to Vector3.zzxx).
        /// </summary>
        public Vector4 bbrr => new Vector4(z, z, x, x);
        
        /// <summary>
        /// Returns Vector3.zzxy swizzling.
        /// </summary>
        public Vector4 zzxy => new Vector4(z, z, x, y);
        
        /// <summary>
        /// Returns Vector3.bbrg swizzling (equivalent to Vector3.zzxy).
        /// </summary>
        public Vector4 bbrg => new Vector4(z, z, x, y);
        
        /// <summary>
        /// Returns Vector3.zzxz swizzling.
        /// </summary>
        public Vector4 zzxz => new Vector4(z, z, x, z);
        
        /// <summary>
        /// Returns Vector3.bbrb swizzling (equivalent to Vector3.zzxz).
        /// </summary>
        public Vector4 bbrb => new Vector4(z, z, x, z);
        
        /// <summary>
        /// Returns Vector3.zzy swizzling.
        /// </summary>
        public Vector3 zzy => new Vector3(z, z, y);
        
        /// <summary>
        /// Returns Vector3.bbg swizzling (equivalent to Vector3.zzy).
        /// </summary>
        public Vector3 bbg => new Vector3(z, z, y);
        
        /// <summary>
        /// Returns Vector3.zzyx swizzling.
        /// </summary>
        public Vector4 zzyx => new Vector4(z, z, y, x);
        
        /// <summary>
        /// Returns Vector3.bbgr swizzling (equivalent to Vector3.zzyx).
        /// </summary>
        public Vector4 bbgr => new Vector4(z, z, y, x);
        
        /// <summary>
        /// Returns Vector3.zzyy swizzling.
        /// </summary>
        public Vector4 zzyy => new Vector4(z, z, y, y);
        
        /// <summary>
        /// Returns Vector3.bbgg swizzling (equivalent to Vector3.zzyy).
        /// </summary>
        public Vector4 bbgg => new Vector4(z, z, y, y);
        
        /// <summary>
        /// Returns Vector3.zzyz swizzling.
        /// </summary>
        public Vector4 zzyz => new Vector4(z, z, y, z);
        
        /// <summary>
        /// Returns Vector3.bbgb swizzling (equivalent to Vector3.zzyz).
        /// </summary>
        public Vector4 bbgb => new Vector4(z, z, y, z);
        
        /// <summary>
        /// Returns Vector3.zzz swizzling.
        /// </summary>
        public Vector3 zzz => new Vector3(z, z, z);
        
        /// <summary>
        /// Returns Vector3.bbb swizzling (equivalent to Vector3.zzz).
        /// </summary>
        public Vector3 bbb => new Vector3(z, z, z);
        
        /// <summary>
        /// Returns Vector3.zzzx swizzling.
        /// </summary>
        public Vector4 zzzx => new Vector4(z, z, z, x);
        
        /// <summary>
        /// Returns Vector3.bbbr swizzling (equivalent to Vector3.zzzx).
        /// </summary>
        public Vector4 bbbr => new Vector4(z, z, z, x);
        
        /// <summary>
        /// Returns Vector3.zzzy swizzling.
        /// </summary>
        public Vector4 zzzy => new Vector4(z, z, z, y);
        
        /// <summary>
        /// Returns Vector3.bbbg swizzling (equivalent to Vector3.zzzy).
        /// </summary>
        public Vector4 bbbg => new Vector4(z, z, z, y);
        
        /// <summary>
        /// Returns Vector3.zzzz swizzling.
        /// </summary>
        public Vector4 zzzz => new Vector4(z, z, z, z);
        
        /// <summary>
        /// Returns Vector3.bbbb swizzling (equivalent to Vector3.zzzz).
        /// </summary>
        public Vector4 bbbb => new Vector4(z, z, z, z);

        #endregion

    }
}
