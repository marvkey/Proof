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
    /// Temporary Vector of type long with 3 components, used for implementing swizzling for lVector3.
    /// </summary>
    
   
    [StructLayout(LayoutKind.Sequential)]
    public struct swizzle_lVector3
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
        
        /// <summary>
        /// z-component
        /// </summary>
        
        internal readonly long z;

        #endregion


        #region Constructors
        
        /// <summary>
        /// Constructor for swizzle_lVector3.
        /// </summary>
        internal swizzle_lVector3(long x, long y, long z)
        {
            this.x = x;
            this.y = y;
            this.z = z;
        }

        #endregion


        #region Properties
        
        /// <summary>
        /// Returns lVector3.xx swizzling.
        /// </summary>
        public lVector2 xx => new lVector2(x, x);
        
        /// <summary>
        /// Returns lVector3.rr swizzling (equivalent to lVector3.xx).
        /// </summary>
        public lVector2 rr => new lVector2(x, x);
        
        /// <summary>
        /// Returns lVector3.xxx swizzling.
        /// </summary>
        public lVector3 xxx => new lVector3(x, x, x);
        
        /// <summary>
        /// Returns lVector3.rrr swizzling (equivalent to lVector3.xxx).
        /// </summary>
        public lVector3 rrr => new lVector3(x, x, x);
        
        /// <summary>
        /// Returns lVector3.xxxx swizzling.
        /// </summary>
        public lVector4 xxxx => new lVector4(x, x, x, x);
        
        /// <summary>
        /// Returns lVector3.rrrr swizzling (equivalent to lVector3.xxxx).
        /// </summary>
        public lVector4 rrrr => new lVector4(x, x, x, x);
        
        /// <summary>
        /// Returns lVector3.xxxy swizzling.
        /// </summary>
        public lVector4 xxxy => new lVector4(x, x, x, y);
        
        /// <summary>
        /// Returns lVector3.rrrg swizzling (equivalent to lVector3.xxxy).
        /// </summary>
        public lVector4 rrrg => new lVector4(x, x, x, y);
        
        /// <summary>
        /// Returns lVector3.xxxz swizzling.
        /// </summary>
        public lVector4 xxxz => new lVector4(x, x, x, z);
        
        /// <summary>
        /// Returns lVector3.rrrb swizzling (equivalent to lVector3.xxxz).
        /// </summary>
        public lVector4 rrrb => new lVector4(x, x, x, z);
        
        /// <summary>
        /// Returns lVector3.xxy swizzling.
        /// </summary>
        public lVector3 xxy => new lVector3(x, x, y);
        
        /// <summary>
        /// Returns lVector3.rrg swizzling (equivalent to lVector3.xxy).
        /// </summary>
        public lVector3 rrg => new lVector3(x, x, y);
        
        /// <summary>
        /// Returns lVector3.xxyx swizzling.
        /// </summary>
        public lVector4 xxyx => new lVector4(x, x, y, x);
        
        /// <summary>
        /// Returns lVector3.rrgr swizzling (equivalent to lVector3.xxyx).
        /// </summary>
        public lVector4 rrgr => new lVector4(x, x, y, x);
        
        /// <summary>
        /// Returns lVector3.xxyy swizzling.
        /// </summary>
        public lVector4 xxyy => new lVector4(x, x, y, y);
        
        /// <summary>
        /// Returns lVector3.rrgg swizzling (equivalent to lVector3.xxyy).
        /// </summary>
        public lVector4 rrgg => new lVector4(x, x, y, y);
        
        /// <summary>
        /// Returns lVector3.xxyz swizzling.
        /// </summary>
        public lVector4 xxyz => new lVector4(x, x, y, z);
        
        /// <summary>
        /// Returns lVector3.rrgb swizzling (equivalent to lVector3.xxyz).
        /// </summary>
        public lVector4 rrgb => new lVector4(x, x, y, z);
        
        /// <summary>
        /// Returns lVector3.xxz swizzling.
        /// </summary>
        public lVector3 xxz => new lVector3(x, x, z);
        
        /// <summary>
        /// Returns lVector3.rrb swizzling (equivalent to lVector3.xxz).
        /// </summary>
        public lVector3 rrb => new lVector3(x, x, z);
        
        /// <summary>
        /// Returns lVector3.xxzx swizzling.
        /// </summary>
        public lVector4 xxzx => new lVector4(x, x, z, x);
        
        /// <summary>
        /// Returns lVector3.rrbr swizzling (equivalent to lVector3.xxzx).
        /// </summary>
        public lVector4 rrbr => new lVector4(x, x, z, x);
        
        /// <summary>
        /// Returns lVector3.xxzy swizzling.
        /// </summary>
        public lVector4 xxzy => new lVector4(x, x, z, y);
        
        /// <summary>
        /// Returns lVector3.rrbg swizzling (equivalent to lVector3.xxzy).
        /// </summary>
        public lVector4 rrbg => new lVector4(x, x, z, y);
        
        /// <summary>
        /// Returns lVector3.xxzz swizzling.
        /// </summary>
        public lVector4 xxzz => new lVector4(x, x, z, z);
        
        /// <summary>
        /// Returns lVector3.rrbb swizzling (equivalent to lVector3.xxzz).
        /// </summary>
        public lVector4 rrbb => new lVector4(x, x, z, z);
        
        /// <summary>
        /// Returns lVector3.xy swizzling.
        /// </summary>
        public lVector2 xy => new lVector2(x, y);
        
        /// <summary>
        /// Returns lVector3.rg swizzling (equivalent to lVector3.xy).
        /// </summary>
        public lVector2 rg => new lVector2(x, y);
        
        /// <summary>
        /// Returns lVector3.xyx swizzling.
        /// </summary>
        public lVector3 xyx => new lVector3(x, y, x);
        
        /// <summary>
        /// Returns lVector3.rgr swizzling (equivalent to lVector3.xyx).
        /// </summary>
        public lVector3 rgr => new lVector3(x, y, x);
        
        /// <summary>
        /// Returns lVector3.xyxx swizzling.
        /// </summary>
        public lVector4 xyxx => new lVector4(x, y, x, x);
        
        /// <summary>
        /// Returns lVector3.rgrr swizzling (equivalent to lVector3.xyxx).
        /// </summary>
        public lVector4 rgrr => new lVector4(x, y, x, x);
        
        /// <summary>
        /// Returns lVector3.xyxy swizzling.
        /// </summary>
        public lVector4 xyxy => new lVector4(x, y, x, y);
        
        /// <summary>
        /// Returns lVector3.rgrg swizzling (equivalent to lVector3.xyxy).
        /// </summary>
        public lVector4 rgrg => new lVector4(x, y, x, y);
        
        /// <summary>
        /// Returns lVector3.xyxz swizzling.
        /// </summary>
        public lVector4 xyxz => new lVector4(x, y, x, z);
        
        /// <summary>
        /// Returns lVector3.rgrb swizzling (equivalent to lVector3.xyxz).
        /// </summary>
        public lVector4 rgrb => new lVector4(x, y, x, z);
        
        /// <summary>
        /// Returns lVector3.xyy swizzling.
        /// </summary>
        public lVector3 xyy => new lVector3(x, y, y);
        
        /// <summary>
        /// Returns lVector3.rgg swizzling (equivalent to lVector3.xyy).
        /// </summary>
        public lVector3 rgg => new lVector3(x, y, y);
        
        /// <summary>
        /// Returns lVector3.xyyx swizzling.
        /// </summary>
        public lVector4 xyyx => new lVector4(x, y, y, x);
        
        /// <summary>
        /// Returns lVector3.rggr swizzling (equivalent to lVector3.xyyx).
        /// </summary>
        public lVector4 rggr => new lVector4(x, y, y, x);
        
        /// <summary>
        /// Returns lVector3.xyyy swizzling.
        /// </summary>
        public lVector4 xyyy => new lVector4(x, y, y, y);
        
        /// <summary>
        /// Returns lVector3.rggg swizzling (equivalent to lVector3.xyyy).
        /// </summary>
        public lVector4 rggg => new lVector4(x, y, y, y);
        
        /// <summary>
        /// Returns lVector3.xyyz swizzling.
        /// </summary>
        public lVector4 xyyz => new lVector4(x, y, y, z);
        
        /// <summary>
        /// Returns lVector3.rggb swizzling (equivalent to lVector3.xyyz).
        /// </summary>
        public lVector4 rggb => new lVector4(x, y, y, z);
        
        /// <summary>
        /// Returns lVector3.xyz swizzling.
        /// </summary>
        public lVector3 xyz => new lVector3(x, y, z);
        
        /// <summary>
        /// Returns lVector3.rgb swizzling (equivalent to lVector3.xyz).
        /// </summary>
        public lVector3 rgb => new lVector3(x, y, z);
        
        /// <summary>
        /// Returns lVector3.xyzx swizzling.
        /// </summary>
        public lVector4 xyzx => new lVector4(x, y, z, x);
        
        /// <summary>
        /// Returns lVector3.rgbr swizzling (equivalent to lVector3.xyzx).
        /// </summary>
        public lVector4 rgbr => new lVector4(x, y, z, x);
        
        /// <summary>
        /// Returns lVector3.xyzy swizzling.
        /// </summary>
        public lVector4 xyzy => new lVector4(x, y, z, y);
        
        /// <summary>
        /// Returns lVector3.rgbg swizzling (equivalent to lVector3.xyzy).
        /// </summary>
        public lVector4 rgbg => new lVector4(x, y, z, y);
        
        /// <summary>
        /// Returns lVector3.xyzz swizzling.
        /// </summary>
        public lVector4 xyzz => new lVector4(x, y, z, z);
        
        /// <summary>
        /// Returns lVector3.rgbb swizzling (equivalent to lVector3.xyzz).
        /// </summary>
        public lVector4 rgbb => new lVector4(x, y, z, z);
        
        /// <summary>
        /// Returns lVector3.xz swizzling.
        /// </summary>
        public lVector2 xz => new lVector2(x, z);
        
        /// <summary>
        /// Returns lVector3.rb swizzling (equivalent to lVector3.xz).
        /// </summary>
        public lVector2 rb => new lVector2(x, z);
        
        /// <summary>
        /// Returns lVector3.xzx swizzling.
        /// </summary>
        public lVector3 xzx => new lVector3(x, z, x);
        
        /// <summary>
        /// Returns lVector3.rbr swizzling (equivalent to lVector3.xzx).
        /// </summary>
        public lVector3 rbr => new lVector3(x, z, x);
        
        /// <summary>
        /// Returns lVector3.xzxx swizzling.
        /// </summary>
        public lVector4 xzxx => new lVector4(x, z, x, x);
        
        /// <summary>
        /// Returns lVector3.rbrr swizzling (equivalent to lVector3.xzxx).
        /// </summary>
        public lVector4 rbrr => new lVector4(x, z, x, x);
        
        /// <summary>
        /// Returns lVector3.xzxy swizzling.
        /// </summary>
        public lVector4 xzxy => new lVector4(x, z, x, y);
        
        /// <summary>
        /// Returns lVector3.rbrg swizzling (equivalent to lVector3.xzxy).
        /// </summary>
        public lVector4 rbrg => new lVector4(x, z, x, y);
        
        /// <summary>
        /// Returns lVector3.xzxz swizzling.
        /// </summary>
        public lVector4 xzxz => new lVector4(x, z, x, z);
        
        /// <summary>
        /// Returns lVector3.rbrb swizzling (equivalent to lVector3.xzxz).
        /// </summary>
        public lVector4 rbrb => new lVector4(x, z, x, z);
        
        /// <summary>
        /// Returns lVector3.xzy swizzling.
        /// </summary>
        public lVector3 xzy => new lVector3(x, z, y);
        
        /// <summary>
        /// Returns lVector3.rbg swizzling (equivalent to lVector3.xzy).
        /// </summary>
        public lVector3 rbg => new lVector3(x, z, y);
        
        /// <summary>
        /// Returns lVector3.xzyx swizzling.
        /// </summary>
        public lVector4 xzyx => new lVector4(x, z, y, x);
        
        /// <summary>
        /// Returns lVector3.rbgr swizzling (equivalent to lVector3.xzyx).
        /// </summary>
        public lVector4 rbgr => new lVector4(x, z, y, x);
        
        /// <summary>
        /// Returns lVector3.xzyy swizzling.
        /// </summary>
        public lVector4 xzyy => new lVector4(x, z, y, y);
        
        /// <summary>
        /// Returns lVector3.rbgg swizzling (equivalent to lVector3.xzyy).
        /// </summary>
        public lVector4 rbgg => new lVector4(x, z, y, y);
        
        /// <summary>
        /// Returns lVector3.xzyz swizzling.
        /// </summary>
        public lVector4 xzyz => new lVector4(x, z, y, z);
        
        /// <summary>
        /// Returns lVector3.rbgb swizzling (equivalent to lVector3.xzyz).
        /// </summary>
        public lVector4 rbgb => new lVector4(x, z, y, z);
        
        /// <summary>
        /// Returns lVector3.xzz swizzling.
        /// </summary>
        public lVector3 xzz => new lVector3(x, z, z);
        
        /// <summary>
        /// Returns lVector3.rbb swizzling (equivalent to lVector3.xzz).
        /// </summary>
        public lVector3 rbb => new lVector3(x, z, z);
        
        /// <summary>
        /// Returns lVector3.xzzx swizzling.
        /// </summary>
        public lVector4 xzzx => new lVector4(x, z, z, x);
        
        /// <summary>
        /// Returns lVector3.rbbr swizzling (equivalent to lVector3.xzzx).
        /// </summary>
        public lVector4 rbbr => new lVector4(x, z, z, x);
        
        /// <summary>
        /// Returns lVector3.xzzy swizzling.
        /// </summary>
        public lVector4 xzzy => new lVector4(x, z, z, y);
        
        /// <summary>
        /// Returns lVector3.rbbg swizzling (equivalent to lVector3.xzzy).
        /// </summary>
        public lVector4 rbbg => new lVector4(x, z, z, y);
        
        /// <summary>
        /// Returns lVector3.xzzz swizzling.
        /// </summary>
        public lVector4 xzzz => new lVector4(x, z, z, z);
        
        /// <summary>
        /// Returns lVector3.rbbb swizzling (equivalent to lVector3.xzzz).
        /// </summary>
        public lVector4 rbbb => new lVector4(x, z, z, z);
        
        /// <summary>
        /// Returns lVector3.yx swizzling.
        /// </summary>
        public lVector2 yx => new lVector2(y, x);
        
        /// <summary>
        /// Returns lVector3.gr swizzling (equivalent to lVector3.yx).
        /// </summary>
        public lVector2 gr => new lVector2(y, x);
        
        /// <summary>
        /// Returns lVector3.yxx swizzling.
        /// </summary>
        public lVector3 yxx => new lVector3(y, x, x);
        
        /// <summary>
        /// Returns lVector3.grr swizzling (equivalent to lVector3.yxx).
        /// </summary>
        public lVector3 grr => new lVector3(y, x, x);
        
        /// <summary>
        /// Returns lVector3.yxxx swizzling.
        /// </summary>
        public lVector4 yxxx => new lVector4(y, x, x, x);
        
        /// <summary>
        /// Returns lVector3.grrr swizzling (equivalent to lVector3.yxxx).
        /// </summary>
        public lVector4 grrr => new lVector4(y, x, x, x);
        
        /// <summary>
        /// Returns lVector3.yxxy swizzling.
        /// </summary>
        public lVector4 yxxy => new lVector4(y, x, x, y);
        
        /// <summary>
        /// Returns lVector3.grrg swizzling (equivalent to lVector3.yxxy).
        /// </summary>
        public lVector4 grrg => new lVector4(y, x, x, y);
        
        /// <summary>
        /// Returns lVector3.yxxz swizzling.
        /// </summary>
        public lVector4 yxxz => new lVector4(y, x, x, z);
        
        /// <summary>
        /// Returns lVector3.grrb swizzling (equivalent to lVector3.yxxz).
        /// </summary>
        public lVector4 grrb => new lVector4(y, x, x, z);
        
        /// <summary>
        /// Returns lVector3.yxy swizzling.
        /// </summary>
        public lVector3 yxy => new lVector3(y, x, y);
        
        /// <summary>
        /// Returns lVector3.grg swizzling (equivalent to lVector3.yxy).
        /// </summary>
        public lVector3 grg => new lVector3(y, x, y);
        
        /// <summary>
        /// Returns lVector3.yxyx swizzling.
        /// </summary>
        public lVector4 yxyx => new lVector4(y, x, y, x);
        
        /// <summary>
        /// Returns lVector3.grgr swizzling (equivalent to lVector3.yxyx).
        /// </summary>
        public lVector4 grgr => new lVector4(y, x, y, x);
        
        /// <summary>
        /// Returns lVector3.yxyy swizzling.
        /// </summary>
        public lVector4 yxyy => new lVector4(y, x, y, y);
        
        /// <summary>
        /// Returns lVector3.grgg swizzling (equivalent to lVector3.yxyy).
        /// </summary>
        public lVector4 grgg => new lVector4(y, x, y, y);
        
        /// <summary>
        /// Returns lVector3.yxyz swizzling.
        /// </summary>
        public lVector4 yxyz => new lVector4(y, x, y, z);
        
        /// <summary>
        /// Returns lVector3.grgb swizzling (equivalent to lVector3.yxyz).
        /// </summary>
        public lVector4 grgb => new lVector4(y, x, y, z);
        
        /// <summary>
        /// Returns lVector3.yxz swizzling.
        /// </summary>
        public lVector3 yxz => new lVector3(y, x, z);
        
        /// <summary>
        /// Returns lVector3.grb swizzling (equivalent to lVector3.yxz).
        /// </summary>
        public lVector3 grb => new lVector3(y, x, z);
        
        /// <summary>
        /// Returns lVector3.yxzx swizzling.
        /// </summary>
        public lVector4 yxzx => new lVector4(y, x, z, x);
        
        /// <summary>
        /// Returns lVector3.grbr swizzling (equivalent to lVector3.yxzx).
        /// </summary>
        public lVector4 grbr => new lVector4(y, x, z, x);
        
        /// <summary>
        /// Returns lVector3.yxzy swizzling.
        /// </summary>
        public lVector4 yxzy => new lVector4(y, x, z, y);
        
        /// <summary>
        /// Returns lVector3.grbg swizzling (equivalent to lVector3.yxzy).
        /// </summary>
        public lVector4 grbg => new lVector4(y, x, z, y);
        
        /// <summary>
        /// Returns lVector3.yxzz swizzling.
        /// </summary>
        public lVector4 yxzz => new lVector4(y, x, z, z);
        
        /// <summary>
        /// Returns lVector3.grbb swizzling (equivalent to lVector3.yxzz).
        /// </summary>
        public lVector4 grbb => new lVector4(y, x, z, z);
        
        /// <summary>
        /// Returns lVector3.yy swizzling.
        /// </summary>
        public lVector2 yy => new lVector2(y, y);
        
        /// <summary>
        /// Returns lVector3.gg swizzling (equivalent to lVector3.yy).
        /// </summary>
        public lVector2 gg => new lVector2(y, y);
        
        /// <summary>
        /// Returns lVector3.yyx swizzling.
        /// </summary>
        public lVector3 yyx => new lVector3(y, y, x);
        
        /// <summary>
        /// Returns lVector3.ggr swizzling (equivalent to lVector3.yyx).
        /// </summary>
        public lVector3 ggr => new lVector3(y, y, x);
        
        /// <summary>
        /// Returns lVector3.yyxx swizzling.
        /// </summary>
        public lVector4 yyxx => new lVector4(y, y, x, x);
        
        /// <summary>
        /// Returns lVector3.ggrr swizzling (equivalent to lVector3.yyxx).
        /// </summary>
        public lVector4 ggrr => new lVector4(y, y, x, x);
        
        /// <summary>
        /// Returns lVector3.yyxy swizzling.
        /// </summary>
        public lVector4 yyxy => new lVector4(y, y, x, y);
        
        /// <summary>
        /// Returns lVector3.ggrg swizzling (equivalent to lVector3.yyxy).
        /// </summary>
        public lVector4 ggrg => new lVector4(y, y, x, y);
        
        /// <summary>
        /// Returns lVector3.yyxz swizzling.
        /// </summary>
        public lVector4 yyxz => new lVector4(y, y, x, z);
        
        /// <summary>
        /// Returns lVector3.ggrb swizzling (equivalent to lVector3.yyxz).
        /// </summary>
        public lVector4 ggrb => new lVector4(y, y, x, z);
        
        /// <summary>
        /// Returns lVector3.yyy swizzling.
        /// </summary>
        public lVector3 yyy => new lVector3(y, y, y);
        
        /// <summary>
        /// Returns lVector3.ggg swizzling (equivalent to lVector3.yyy).
        /// </summary>
        public lVector3 ggg => new lVector3(y, y, y);
        
        /// <summary>
        /// Returns lVector3.yyyx swizzling.
        /// </summary>
        public lVector4 yyyx => new lVector4(y, y, y, x);
        
        /// <summary>
        /// Returns lVector3.gggr swizzling (equivalent to lVector3.yyyx).
        /// </summary>
        public lVector4 gggr => new lVector4(y, y, y, x);
        
        /// <summary>
        /// Returns lVector3.yyyy swizzling.
        /// </summary>
        public lVector4 yyyy => new lVector4(y, y, y, y);
        
        /// <summary>
        /// Returns lVector3.gggg swizzling (equivalent to lVector3.yyyy).
        /// </summary>
        public lVector4 gggg => new lVector4(y, y, y, y);
        
        /// <summary>
        /// Returns lVector3.yyyz swizzling.
        /// </summary>
        public lVector4 yyyz => new lVector4(y, y, y, z);
        
        /// <summary>
        /// Returns lVector3.gggb swizzling (equivalent to lVector3.yyyz).
        /// </summary>
        public lVector4 gggb => new lVector4(y, y, y, z);
        
        /// <summary>
        /// Returns lVector3.yyz swizzling.
        /// </summary>
        public lVector3 yyz => new lVector3(y, y, z);
        
        /// <summary>
        /// Returns lVector3.ggb swizzling (equivalent to lVector3.yyz).
        /// </summary>
        public lVector3 ggb => new lVector3(y, y, z);
        
        /// <summary>
        /// Returns lVector3.yyzx swizzling.
        /// </summary>
        public lVector4 yyzx => new lVector4(y, y, z, x);
        
        /// <summary>
        /// Returns lVector3.ggbr swizzling (equivalent to lVector3.yyzx).
        /// </summary>
        public lVector4 ggbr => new lVector4(y, y, z, x);
        
        /// <summary>
        /// Returns lVector3.yyzy swizzling.
        /// </summary>
        public lVector4 yyzy => new lVector4(y, y, z, y);
        
        /// <summary>
        /// Returns lVector3.ggbg swizzling (equivalent to lVector3.yyzy).
        /// </summary>
        public lVector4 ggbg => new lVector4(y, y, z, y);
        
        /// <summary>
        /// Returns lVector3.yyzz swizzling.
        /// </summary>
        public lVector4 yyzz => new lVector4(y, y, z, z);
        
        /// <summary>
        /// Returns lVector3.ggbb swizzling (equivalent to lVector3.yyzz).
        /// </summary>
        public lVector4 ggbb => new lVector4(y, y, z, z);
        
        /// <summary>
        /// Returns lVector3.yz swizzling.
        /// </summary>
        public lVector2 yz => new lVector2(y, z);
        
        /// <summary>
        /// Returns lVector3.gb swizzling (equivalent to lVector3.yz).
        /// </summary>
        public lVector2 gb => new lVector2(y, z);
        
        /// <summary>
        /// Returns lVector3.yzx swizzling.
        /// </summary>
        public lVector3 yzx => new lVector3(y, z, x);
        
        /// <summary>
        /// Returns lVector3.gbr swizzling (equivalent to lVector3.yzx).
        /// </summary>
        public lVector3 gbr => new lVector3(y, z, x);
        
        /// <summary>
        /// Returns lVector3.yzxx swizzling.
        /// </summary>
        public lVector4 yzxx => new lVector4(y, z, x, x);
        
        /// <summary>
        /// Returns lVector3.gbrr swizzling (equivalent to lVector3.yzxx).
        /// </summary>
        public lVector4 gbrr => new lVector4(y, z, x, x);
        
        /// <summary>
        /// Returns lVector3.yzxy swizzling.
        /// </summary>
        public lVector4 yzxy => new lVector4(y, z, x, y);
        
        /// <summary>
        /// Returns lVector3.gbrg swizzling (equivalent to lVector3.yzxy).
        /// </summary>
        public lVector4 gbrg => new lVector4(y, z, x, y);
        
        /// <summary>
        /// Returns lVector3.yzxz swizzling.
        /// </summary>
        public lVector4 yzxz => new lVector4(y, z, x, z);
        
        /// <summary>
        /// Returns lVector3.gbrb swizzling (equivalent to lVector3.yzxz).
        /// </summary>
        public lVector4 gbrb => new lVector4(y, z, x, z);
        
        /// <summary>
        /// Returns lVector3.yzy swizzling.
        /// </summary>
        public lVector3 yzy => new lVector3(y, z, y);
        
        /// <summary>
        /// Returns lVector3.gbg swizzling (equivalent to lVector3.yzy).
        /// </summary>
        public lVector3 gbg => new lVector3(y, z, y);
        
        /// <summary>
        /// Returns lVector3.yzyx swizzling.
        /// </summary>
        public lVector4 yzyx => new lVector4(y, z, y, x);
        
        /// <summary>
        /// Returns lVector3.gbgr swizzling (equivalent to lVector3.yzyx).
        /// </summary>
        public lVector4 gbgr => new lVector4(y, z, y, x);
        
        /// <summary>
        /// Returns lVector3.yzyy swizzling.
        /// </summary>
        public lVector4 yzyy => new lVector4(y, z, y, y);
        
        /// <summary>
        /// Returns lVector3.gbgg swizzling (equivalent to lVector3.yzyy).
        /// </summary>
        public lVector4 gbgg => new lVector4(y, z, y, y);
        
        /// <summary>
        /// Returns lVector3.yzyz swizzling.
        /// </summary>
        public lVector4 yzyz => new lVector4(y, z, y, z);
        
        /// <summary>
        /// Returns lVector3.gbgb swizzling (equivalent to lVector3.yzyz).
        /// </summary>
        public lVector4 gbgb => new lVector4(y, z, y, z);
        
        /// <summary>
        /// Returns lVector3.yzz swizzling.
        /// </summary>
        public lVector3 yzz => new lVector3(y, z, z);
        
        /// <summary>
        /// Returns lVector3.gbb swizzling (equivalent to lVector3.yzz).
        /// </summary>
        public lVector3 gbb => new lVector3(y, z, z);
        
        /// <summary>
        /// Returns lVector3.yzzx swizzling.
        /// </summary>
        public lVector4 yzzx => new lVector4(y, z, z, x);
        
        /// <summary>
        /// Returns lVector3.gbbr swizzling (equivalent to lVector3.yzzx).
        /// </summary>
        public lVector4 gbbr => new lVector4(y, z, z, x);
        
        /// <summary>
        /// Returns lVector3.yzzy swizzling.
        /// </summary>
        public lVector4 yzzy => new lVector4(y, z, z, y);
        
        /// <summary>
        /// Returns lVector3.gbbg swizzling (equivalent to lVector3.yzzy).
        /// </summary>
        public lVector4 gbbg => new lVector4(y, z, z, y);
        
        /// <summary>
        /// Returns lVector3.yzzz swizzling.
        /// </summary>
        public lVector4 yzzz => new lVector4(y, z, z, z);
        
        /// <summary>
        /// Returns lVector3.gbbb swizzling (equivalent to lVector3.yzzz).
        /// </summary>
        public lVector4 gbbb => new lVector4(y, z, z, z);
        
        /// <summary>
        /// Returns lVector3.zx swizzling.
        /// </summary>
        public lVector2 zx => new lVector2(z, x);
        
        /// <summary>
        /// Returns lVector3.br swizzling (equivalent to lVector3.zx).
        /// </summary>
        public lVector2 br => new lVector2(z, x);
        
        /// <summary>
        /// Returns lVector3.zxx swizzling.
        /// </summary>
        public lVector3 zxx => new lVector3(z, x, x);
        
        /// <summary>
        /// Returns lVector3.brr swizzling (equivalent to lVector3.zxx).
        /// </summary>
        public lVector3 brr => new lVector3(z, x, x);
        
        /// <summary>
        /// Returns lVector3.zxxx swizzling.
        /// </summary>
        public lVector4 zxxx => new lVector4(z, x, x, x);
        
        /// <summary>
        /// Returns lVector3.brrr swizzling (equivalent to lVector3.zxxx).
        /// </summary>
        public lVector4 brrr => new lVector4(z, x, x, x);
        
        /// <summary>
        /// Returns lVector3.zxxy swizzling.
        /// </summary>
        public lVector4 zxxy => new lVector4(z, x, x, y);
        
        /// <summary>
        /// Returns lVector3.brrg swizzling (equivalent to lVector3.zxxy).
        /// </summary>
        public lVector4 brrg => new lVector4(z, x, x, y);
        
        /// <summary>
        /// Returns lVector3.zxxz swizzling.
        /// </summary>
        public lVector4 zxxz => new lVector4(z, x, x, z);
        
        /// <summary>
        /// Returns lVector3.brrb swizzling (equivalent to lVector3.zxxz).
        /// </summary>
        public lVector4 brrb => new lVector4(z, x, x, z);
        
        /// <summary>
        /// Returns lVector3.zxy swizzling.
        /// </summary>
        public lVector3 zxy => new lVector3(z, x, y);
        
        /// <summary>
        /// Returns lVector3.brg swizzling (equivalent to lVector3.zxy).
        /// </summary>
        public lVector3 brg => new lVector3(z, x, y);
        
        /// <summary>
        /// Returns lVector3.zxyx swizzling.
        /// </summary>
        public lVector4 zxyx => new lVector4(z, x, y, x);
        
        /// <summary>
        /// Returns lVector3.brgr swizzling (equivalent to lVector3.zxyx).
        /// </summary>
        public lVector4 brgr => new lVector4(z, x, y, x);
        
        /// <summary>
        /// Returns lVector3.zxyy swizzling.
        /// </summary>
        public lVector4 zxyy => new lVector4(z, x, y, y);
        
        /// <summary>
        /// Returns lVector3.brgg swizzling (equivalent to lVector3.zxyy).
        /// </summary>
        public lVector4 brgg => new lVector4(z, x, y, y);
        
        /// <summary>
        /// Returns lVector3.zxyz swizzling.
        /// </summary>
        public lVector4 zxyz => new lVector4(z, x, y, z);
        
        /// <summary>
        /// Returns lVector3.brgb swizzling (equivalent to lVector3.zxyz).
        /// </summary>
        public lVector4 brgb => new lVector4(z, x, y, z);
        
        /// <summary>
        /// Returns lVector3.zxz swizzling.
        /// </summary>
        public lVector3 zxz => new lVector3(z, x, z);
        
        /// <summary>
        /// Returns lVector3.brb swizzling (equivalent to lVector3.zxz).
        /// </summary>
        public lVector3 brb => new lVector3(z, x, z);
        
        /// <summary>
        /// Returns lVector3.zxzx swizzling.
        /// </summary>
        public lVector4 zxzx => new lVector4(z, x, z, x);
        
        /// <summary>
        /// Returns lVector3.brbr swizzling (equivalent to lVector3.zxzx).
        /// </summary>
        public lVector4 brbr => new lVector4(z, x, z, x);
        
        /// <summary>
        /// Returns lVector3.zxzy swizzling.
        /// </summary>
        public lVector4 zxzy => new lVector4(z, x, z, y);
        
        /// <summary>
        /// Returns lVector3.brbg swizzling (equivalent to lVector3.zxzy).
        /// </summary>
        public lVector4 brbg => new lVector4(z, x, z, y);
        
        /// <summary>
        /// Returns lVector3.zxzz swizzling.
        /// </summary>
        public lVector4 zxzz => new lVector4(z, x, z, z);
        
        /// <summary>
        /// Returns lVector3.brbb swizzling (equivalent to lVector3.zxzz).
        /// </summary>
        public lVector4 brbb => new lVector4(z, x, z, z);
        
        /// <summary>
        /// Returns lVector3.zy swizzling.
        /// </summary>
        public lVector2 zy => new lVector2(z, y);
        
        /// <summary>
        /// Returns lVector3.bg swizzling (equivalent to lVector3.zy).
        /// </summary>
        public lVector2 bg => new lVector2(z, y);
        
        /// <summary>
        /// Returns lVector3.zyx swizzling.
        /// </summary>
        public lVector3 zyx => new lVector3(z, y, x);
        
        /// <summary>
        /// Returns lVector3.bgr swizzling (equivalent to lVector3.zyx).
        /// </summary>
        public lVector3 bgr => new lVector3(z, y, x);
        
        /// <summary>
        /// Returns lVector3.zyxx swizzling.
        /// </summary>
        public lVector4 zyxx => new lVector4(z, y, x, x);
        
        /// <summary>
        /// Returns lVector3.bgrr swizzling (equivalent to lVector3.zyxx).
        /// </summary>
        public lVector4 bgrr => new lVector4(z, y, x, x);
        
        /// <summary>
        /// Returns lVector3.zyxy swizzling.
        /// </summary>
        public lVector4 zyxy => new lVector4(z, y, x, y);
        
        /// <summary>
        /// Returns lVector3.bgrg swizzling (equivalent to lVector3.zyxy).
        /// </summary>
        public lVector4 bgrg => new lVector4(z, y, x, y);
        
        /// <summary>
        /// Returns lVector3.zyxz swizzling.
        /// </summary>
        public lVector4 zyxz => new lVector4(z, y, x, z);
        
        /// <summary>
        /// Returns lVector3.bgrb swizzling (equivalent to lVector3.zyxz).
        /// </summary>
        public lVector4 bgrb => new lVector4(z, y, x, z);
        
        /// <summary>
        /// Returns lVector3.zyy swizzling.
        /// </summary>
        public lVector3 zyy => new lVector3(z, y, y);
        
        /// <summary>
        /// Returns lVector3.bgg swizzling (equivalent to lVector3.zyy).
        /// </summary>
        public lVector3 bgg => new lVector3(z, y, y);
        
        /// <summary>
        /// Returns lVector3.zyyx swizzling.
        /// </summary>
        public lVector4 zyyx => new lVector4(z, y, y, x);
        
        /// <summary>
        /// Returns lVector3.bggr swizzling (equivalent to lVector3.zyyx).
        /// </summary>
        public lVector4 bggr => new lVector4(z, y, y, x);
        
        /// <summary>
        /// Returns lVector3.zyyy swizzling.
        /// </summary>
        public lVector4 zyyy => new lVector4(z, y, y, y);
        
        /// <summary>
        /// Returns lVector3.bggg swizzling (equivalent to lVector3.zyyy).
        /// </summary>
        public lVector4 bggg => new lVector4(z, y, y, y);
        
        /// <summary>
        /// Returns lVector3.zyyz swizzling.
        /// </summary>
        public lVector4 zyyz => new lVector4(z, y, y, z);
        
        /// <summary>
        /// Returns lVector3.bggb swizzling (equivalent to lVector3.zyyz).
        /// </summary>
        public lVector4 bggb => new lVector4(z, y, y, z);
        
        /// <summary>
        /// Returns lVector3.zyz swizzling.
        /// </summary>
        public lVector3 zyz => new lVector3(z, y, z);
        
        /// <summary>
        /// Returns lVector3.bgb swizzling (equivalent to lVector3.zyz).
        /// </summary>
        public lVector3 bgb => new lVector3(z, y, z);
        
        /// <summary>
        /// Returns lVector3.zyzx swizzling.
        /// </summary>
        public lVector4 zyzx => new lVector4(z, y, z, x);
        
        /// <summary>
        /// Returns lVector3.bgbr swizzling (equivalent to lVector3.zyzx).
        /// </summary>
        public lVector4 bgbr => new lVector4(z, y, z, x);
        
        /// <summary>
        /// Returns lVector3.zyzy swizzling.
        /// </summary>
        public lVector4 zyzy => new lVector4(z, y, z, y);
        
        /// <summary>
        /// Returns lVector3.bgbg swizzling (equivalent to lVector3.zyzy).
        /// </summary>
        public lVector4 bgbg => new lVector4(z, y, z, y);
        
        /// <summary>
        /// Returns lVector3.zyzz swizzling.
        /// </summary>
        public lVector4 zyzz => new lVector4(z, y, z, z);
        
        /// <summary>
        /// Returns lVector3.bgbb swizzling (equivalent to lVector3.zyzz).
        /// </summary>
        public lVector4 bgbb => new lVector4(z, y, z, z);
        
        /// <summary>
        /// Returns lVector3.zz swizzling.
        /// </summary>
        public lVector2 zz => new lVector2(z, z);
        
        /// <summary>
        /// Returns lVector3.bb swizzling (equivalent to lVector3.zz).
        /// </summary>
        public lVector2 bb => new lVector2(z, z);
        
        /// <summary>
        /// Returns lVector3.zzx swizzling.
        /// </summary>
        public lVector3 zzx => new lVector3(z, z, x);
        
        /// <summary>
        /// Returns lVector3.bbr swizzling (equivalent to lVector3.zzx).
        /// </summary>
        public lVector3 bbr => new lVector3(z, z, x);
        
        /// <summary>
        /// Returns lVector3.zzxx swizzling.
        /// </summary>
        public lVector4 zzxx => new lVector4(z, z, x, x);
        
        /// <summary>
        /// Returns lVector3.bbrr swizzling (equivalent to lVector3.zzxx).
        /// </summary>
        public lVector4 bbrr => new lVector4(z, z, x, x);
        
        /// <summary>
        /// Returns lVector3.zzxy swizzling.
        /// </summary>
        public lVector4 zzxy => new lVector4(z, z, x, y);
        
        /// <summary>
        /// Returns lVector3.bbrg swizzling (equivalent to lVector3.zzxy).
        /// </summary>
        public lVector4 bbrg => new lVector4(z, z, x, y);
        
        /// <summary>
        /// Returns lVector3.zzxz swizzling.
        /// </summary>
        public lVector4 zzxz => new lVector4(z, z, x, z);
        
        /// <summary>
        /// Returns lVector3.bbrb swizzling (equivalent to lVector3.zzxz).
        /// </summary>
        public lVector4 bbrb => new lVector4(z, z, x, z);
        
        /// <summary>
        /// Returns lVector3.zzy swizzling.
        /// </summary>
        public lVector3 zzy => new lVector3(z, z, y);
        
        /// <summary>
        /// Returns lVector3.bbg swizzling (equivalent to lVector3.zzy).
        /// </summary>
        public lVector3 bbg => new lVector3(z, z, y);
        
        /// <summary>
        /// Returns lVector3.zzyx swizzling.
        /// </summary>
        public lVector4 zzyx => new lVector4(z, z, y, x);
        
        /// <summary>
        /// Returns lVector3.bbgr swizzling (equivalent to lVector3.zzyx).
        /// </summary>
        public lVector4 bbgr => new lVector4(z, z, y, x);
        
        /// <summary>
        /// Returns lVector3.zzyy swizzling.
        /// </summary>
        public lVector4 zzyy => new lVector4(z, z, y, y);
        
        /// <summary>
        /// Returns lVector3.bbgg swizzling (equivalent to lVector3.zzyy).
        /// </summary>
        public lVector4 bbgg => new lVector4(z, z, y, y);
        
        /// <summary>
        /// Returns lVector3.zzyz swizzling.
        /// </summary>
        public lVector4 zzyz => new lVector4(z, z, y, z);
        
        /// <summary>
        /// Returns lVector3.bbgb swizzling (equivalent to lVector3.zzyz).
        /// </summary>
        public lVector4 bbgb => new lVector4(z, z, y, z);
        
        /// <summary>
        /// Returns lVector3.zzz swizzling.
        /// </summary>
        public lVector3 zzz => new lVector3(z, z, z);
        
        /// <summary>
        /// Returns lVector3.bbb swizzling (equivalent to lVector3.zzz).
        /// </summary>
        public lVector3 bbb => new lVector3(z, z, z);
        
        /// <summary>
        /// Returns lVector3.zzzx swizzling.
        /// </summary>
        public lVector4 zzzx => new lVector4(z, z, z, x);
        
        /// <summary>
        /// Returns lVector3.bbbr swizzling (equivalent to lVector3.zzzx).
        /// </summary>
        public lVector4 bbbr => new lVector4(z, z, z, x);
        
        /// <summary>
        /// Returns lVector3.zzzy swizzling.
        /// </summary>
        public lVector4 zzzy => new lVector4(z, z, z, y);
        
        /// <summary>
        /// Returns lVector3.bbbg swizzling (equivalent to lVector3.zzzy).
        /// </summary>
        public lVector4 bbbg => new lVector4(z, z, z, y);
        
        /// <summary>
        /// Returns lVector3.zzzz swizzling.
        /// </summary>
        public lVector4 zzzz => new lVector4(z, z, z, z);
        
        /// <summary>
        /// Returns lVector3.bbbb swizzling (equivalent to lVector3.zzzz).
        /// </summary>
        public lVector4 bbbb => new lVector4(z, z, z, z);

        #endregion

    }
}
