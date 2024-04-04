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
    /// Temporary Vectortor of type decimal with 3 components, used for implementing swizzling for decVector3.
    /// </summary>
    [RegisterCoreClassStruct]
   
    [StructLayout(LayoutKind.Sequential)]
    public struct swizzle_decVector3
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
        
        /// <summary>
        /// z-component
        /// </summary>
        
        internal readonly decimal z;

        #endregion


        #region Constructors
        
        /// <summary>
        /// Constructor for swizzle_decVector3.
        /// </summary>
        internal swizzle_decVector3(decimal x, decimal y, decimal z)
        {
            this.x = x;
            this.y = y;
            this.z = z;
        }

        #endregion


        #region Properties
        
        /// <summary>
        /// Returns decVector3.xx swizzling.
        /// </summary>
        public decVector2 xx => new decVector2(x, x);
        
        /// <summary>
        /// Returns decVector3.rr swizzling (equivalent to decVector3.xx).
        /// </summary>
        public decVector2 rr => new decVector2(x, x);
        
        /// <summary>
        /// Returns decVector3.xxx swizzling.
        /// </summary>
        public decVector3 xxx => new decVector3(x, x, x);
        
        /// <summary>
        /// Returns decVector3.rrr swizzling (equivalent to decVector3.xxx).
        /// </summary>
        public decVector3 rrr => new decVector3(x, x, x);
        
        /// <summary>
        /// Returns decVector3.xxxx swizzling.
        /// </summary>
        public decVector4 xxxx => new decVector4(x, x, x, x);
        
        /// <summary>
        /// Returns decVector3.rrrr swizzling (equivalent to decVector3.xxxx).
        /// </summary>
        public decVector4 rrrr => new decVector4(x, x, x, x);
        
        /// <summary>
        /// Returns decVector3.xxxy swizzling.
        /// </summary>
        public decVector4 xxxy => new decVector4(x, x, x, y);
        
        /// <summary>
        /// Returns decVector3.rrrg swizzling (equivalent to decVector3.xxxy).
        /// </summary>
        public decVector4 rrrg => new decVector4(x, x, x, y);
        
        /// <summary>
        /// Returns decVector3.xxxz swizzling.
        /// </summary>
        public decVector4 xxxz => new decVector4(x, x, x, z);
        
        /// <summary>
        /// Returns decVector3.rrrb swizzling (equivalent to decVector3.xxxz).
        /// </summary>
        public decVector4 rrrb => new decVector4(x, x, x, z);
        
        /// <summary>
        /// Returns decVector3.xxy swizzling.
        /// </summary>
        public decVector3 xxy => new decVector3(x, x, y);
        
        /// <summary>
        /// Returns decVector3.rrg swizzling (equivalent to decVector3.xxy).
        /// </summary>
        public decVector3 rrg => new decVector3(x, x, y);
        
        /// <summary>
        /// Returns decVector3.xxyx swizzling.
        /// </summary>
        public decVector4 xxyx => new decVector4(x, x, y, x);
        
        /// <summary>
        /// Returns decVector3.rrgr swizzling (equivalent to decVector3.xxyx).
        /// </summary>
        public decVector4 rrgr => new decVector4(x, x, y, x);
        
        /// <summary>
        /// Returns decVector3.xxyy swizzling.
        /// </summary>
        public decVector4 xxyy => new decVector4(x, x, y, y);
        
        /// <summary>
        /// Returns decVector3.rrgg swizzling (equivalent to decVector3.xxyy).
        /// </summary>
        public decVector4 rrgg => new decVector4(x, x, y, y);
        
        /// <summary>
        /// Returns decVector3.xxyz swizzling.
        /// </summary>
        public decVector4 xxyz => new decVector4(x, x, y, z);
        
        /// <summary>
        /// Returns decVector3.rrgb swizzling (equivalent to decVector3.xxyz).
        /// </summary>
        public decVector4 rrgb => new decVector4(x, x, y, z);
        
        /// <summary>
        /// Returns decVector3.xxz swizzling.
        /// </summary>
        public decVector3 xxz => new decVector3(x, x, z);
        
        /// <summary>
        /// Returns decVector3.rrb swizzling (equivalent to decVector3.xxz).
        /// </summary>
        public decVector3 rrb => new decVector3(x, x, z);
        
        /// <summary>
        /// Returns decVector3.xxzx swizzling.
        /// </summary>
        public decVector4 xxzx => new decVector4(x, x, z, x);
        
        /// <summary>
        /// Returns decVector3.rrbr swizzling (equivalent to decVector3.xxzx).
        /// </summary>
        public decVector4 rrbr => new decVector4(x, x, z, x);
        
        /// <summary>
        /// Returns decVector3.xxzy swizzling.
        /// </summary>
        public decVector4 xxzy => new decVector4(x, x, z, y);
        
        /// <summary>
        /// Returns decVector3.rrbg swizzling (equivalent to decVector3.xxzy).
        /// </summary>
        public decVector4 rrbg => new decVector4(x, x, z, y);
        
        /// <summary>
        /// Returns decVector3.xxzz swizzling.
        /// </summary>
        public decVector4 xxzz => new decVector4(x, x, z, z);
        
        /// <summary>
        /// Returns decVector3.rrbb swizzling (equivalent to decVector3.xxzz).
        /// </summary>
        public decVector4 rrbb => new decVector4(x, x, z, z);
        
        /// <summary>
        /// Returns decVector3.xy swizzling.
        /// </summary>
        public decVector2 xy => new decVector2(x, y);
        
        /// <summary>
        /// Returns decVector3.rg swizzling (equivalent to decVector3.xy).
        /// </summary>
        public decVector2 rg => new decVector2(x, y);
        
        /// <summary>
        /// Returns decVector3.xyx swizzling.
        /// </summary>
        public decVector3 xyx => new decVector3(x, y, x);
        
        /// <summary>
        /// Returns decVector3.rgr swizzling (equivalent to decVector3.xyx).
        /// </summary>
        public decVector3 rgr => new decVector3(x, y, x);
        
        /// <summary>
        /// Returns decVector3.xyxx swizzling.
        /// </summary>
        public decVector4 xyxx => new decVector4(x, y, x, x);
        
        /// <summary>
        /// Returns decVector3.rgrr swizzling (equivalent to decVector3.xyxx).
        /// </summary>
        public decVector4 rgrr => new decVector4(x, y, x, x);
        
        /// <summary>
        /// Returns decVector3.xyxy swizzling.
        /// </summary>
        public decVector4 xyxy => new decVector4(x, y, x, y);
        
        /// <summary>
        /// Returns decVector3.rgrg swizzling (equivalent to decVector3.xyxy).
        /// </summary>
        public decVector4 rgrg => new decVector4(x, y, x, y);
        
        /// <summary>
        /// Returns decVector3.xyxz swizzling.
        /// </summary>
        public decVector4 xyxz => new decVector4(x, y, x, z);
        
        /// <summary>
        /// Returns decVector3.rgrb swizzling (equivalent to decVector3.xyxz).
        /// </summary>
        public decVector4 rgrb => new decVector4(x, y, x, z);
        
        /// <summary>
        /// Returns decVector3.xyy swizzling.
        /// </summary>
        public decVector3 xyy => new decVector3(x, y, y);
        
        /// <summary>
        /// Returns decVector3.rgg swizzling (equivalent to decVector3.xyy).
        /// </summary>
        public decVector3 rgg => new decVector3(x, y, y);
        
        /// <summary>
        /// Returns decVector3.xyyx swizzling.
        /// </summary>
        public decVector4 xyyx => new decVector4(x, y, y, x);
        
        /// <summary>
        /// Returns decVector3.rggr swizzling (equivalent to decVector3.xyyx).
        /// </summary>
        public decVector4 rggr => new decVector4(x, y, y, x);
        
        /// <summary>
        /// Returns decVector3.xyyy swizzling.
        /// </summary>
        public decVector4 xyyy => new decVector4(x, y, y, y);
        
        /// <summary>
        /// Returns decVector3.rggg swizzling (equivalent to decVector3.xyyy).
        /// </summary>
        public decVector4 rggg => new decVector4(x, y, y, y);
        
        /// <summary>
        /// Returns decVector3.xyyz swizzling.
        /// </summary>
        public decVector4 xyyz => new decVector4(x, y, y, z);
        
        /// <summary>
        /// Returns decVector3.rggb swizzling (equivalent to decVector3.xyyz).
        /// </summary>
        public decVector4 rggb => new decVector4(x, y, y, z);
        
        /// <summary>
        /// Returns decVector3.xyz swizzling.
        /// </summary>
        public decVector3 xyz => new decVector3(x, y, z);
        
        /// <summary>
        /// Returns decVector3.rgb swizzling (equivalent to decVector3.xyz).
        /// </summary>
        public decVector3 rgb => new decVector3(x, y, z);
        
        /// <summary>
        /// Returns decVector3.xyzx swizzling.
        /// </summary>
        public decVector4 xyzx => new decVector4(x, y, z, x);
        
        /// <summary>
        /// Returns decVector3.rgbr swizzling (equivalent to decVector3.xyzx).
        /// </summary>
        public decVector4 rgbr => new decVector4(x, y, z, x);
        
        /// <summary>
        /// Returns decVector3.xyzy swizzling.
        /// </summary>
        public decVector4 xyzy => new decVector4(x, y, z, y);
        
        /// <summary>
        /// Returns decVector3.rgbg swizzling (equivalent to decVector3.xyzy).
        /// </summary>
        public decVector4 rgbg => new decVector4(x, y, z, y);
        
        /// <summary>
        /// Returns decVector3.xyzz swizzling.
        /// </summary>
        public decVector4 xyzz => new decVector4(x, y, z, z);
        
        /// <summary>
        /// Returns decVector3.rgbb swizzling (equivalent to decVector3.xyzz).
        /// </summary>
        public decVector4 rgbb => new decVector4(x, y, z, z);
        
        /// <summary>
        /// Returns decVector3.xz swizzling.
        /// </summary>
        public decVector2 xz => new decVector2(x, z);
        
        /// <summary>
        /// Returns decVector3.rb swizzling (equivalent to decVector3.xz).
        /// </summary>
        public decVector2 rb => new decVector2(x, z);
        
        /// <summary>
        /// Returns decVector3.xzx swizzling.
        /// </summary>
        public decVector3 xzx => new decVector3(x, z, x);
        
        /// <summary>
        /// Returns decVector3.rbr swizzling (equivalent to decVector3.xzx).
        /// </summary>
        public decVector3 rbr => new decVector3(x, z, x);
        
        /// <summary>
        /// Returns decVector3.xzxx swizzling.
        /// </summary>
        public decVector4 xzxx => new decVector4(x, z, x, x);
        
        /// <summary>
        /// Returns decVector3.rbrr swizzling (equivalent to decVector3.xzxx).
        /// </summary>
        public decVector4 rbrr => new decVector4(x, z, x, x);
        
        /// <summary>
        /// Returns decVector3.xzxy swizzling.
        /// </summary>
        public decVector4 xzxy => new decVector4(x, z, x, y);
        
        /// <summary>
        /// Returns decVector3.rbrg swizzling (equivalent to decVector3.xzxy).
        /// </summary>
        public decVector4 rbrg => new decVector4(x, z, x, y);
        
        /// <summary>
        /// Returns decVector3.xzxz swizzling.
        /// </summary>
        public decVector4 xzxz => new decVector4(x, z, x, z);
        
        /// <summary>
        /// Returns decVector3.rbrb swizzling (equivalent to decVector3.xzxz).
        /// </summary>
        public decVector4 rbrb => new decVector4(x, z, x, z);
        
        /// <summary>
        /// Returns decVector3.xzy swizzling.
        /// </summary>
        public decVector3 xzy => new decVector3(x, z, y);
        
        /// <summary>
        /// Returns decVector3.rbg swizzling (equivalent to decVector3.xzy).
        /// </summary>
        public decVector3 rbg => new decVector3(x, z, y);
        
        /// <summary>
        /// Returns decVector3.xzyx swizzling.
        /// </summary>
        public decVector4 xzyx => new decVector4(x, z, y, x);
        
        /// <summary>
        /// Returns decVector3.rbgr swizzling (equivalent to decVector3.xzyx).
        /// </summary>
        public decVector4 rbgr => new decVector4(x, z, y, x);
        
        /// <summary>
        /// Returns decVector3.xzyy swizzling.
        /// </summary>
        public decVector4 xzyy => new decVector4(x, z, y, y);
        
        /// <summary>
        /// Returns decVector3.rbgg swizzling (equivalent to decVector3.xzyy).
        /// </summary>
        public decVector4 rbgg => new decVector4(x, z, y, y);
        
        /// <summary>
        /// Returns decVector3.xzyz swizzling.
        /// </summary>
        public decVector4 xzyz => new decVector4(x, z, y, z);
        
        /// <summary>
        /// Returns decVector3.rbgb swizzling (equivalent to decVector3.xzyz).
        /// </summary>
        public decVector4 rbgb => new decVector4(x, z, y, z);
        
        /// <summary>
        /// Returns decVector3.xzz swizzling.
        /// </summary>
        public decVector3 xzz => new decVector3(x, z, z);
        
        /// <summary>
        /// Returns decVector3.rbb swizzling (equivalent to decVector3.xzz).
        /// </summary>
        public decVector3 rbb => new decVector3(x, z, z);
        
        /// <summary>
        /// Returns decVector3.xzzx swizzling.
        /// </summary>
        public decVector4 xzzx => new decVector4(x, z, z, x);
        
        /// <summary>
        /// Returns decVector3.rbbr swizzling (equivalent to decVector3.xzzx).
        /// </summary>
        public decVector4 rbbr => new decVector4(x, z, z, x);
        
        /// <summary>
        /// Returns decVector3.xzzy swizzling.
        /// </summary>
        public decVector4 xzzy => new decVector4(x, z, z, y);
        
        /// <summary>
        /// Returns decVector3.rbbg swizzling (equivalent to decVector3.xzzy).
        /// </summary>
        public decVector4 rbbg => new decVector4(x, z, z, y);
        
        /// <summary>
        /// Returns decVector3.xzzz swizzling.
        /// </summary>
        public decVector4 xzzz => new decVector4(x, z, z, z);
        
        /// <summary>
        /// Returns decVector3.rbbb swizzling (equivalent to decVector3.xzzz).
        /// </summary>
        public decVector4 rbbb => new decVector4(x, z, z, z);
        
        /// <summary>
        /// Returns decVector3.yx swizzling.
        /// </summary>
        public decVector2 yx => new decVector2(y, x);
        
        /// <summary>
        /// Returns decVector3.gr swizzling (equivalent to decVector3.yx).
        /// </summary>
        public decVector2 gr => new decVector2(y, x);
        
        /// <summary>
        /// Returns decVector3.yxx swizzling.
        /// </summary>
        public decVector3 yxx => new decVector3(y, x, x);
        
        /// <summary>
        /// Returns decVector3.grr swizzling (equivalent to decVector3.yxx).
        /// </summary>
        public decVector3 grr => new decVector3(y, x, x);
        
        /// <summary>
        /// Returns decVector3.yxxx swizzling.
        /// </summary>
        public decVector4 yxxx => new decVector4(y, x, x, x);
        
        /// <summary>
        /// Returns decVector3.grrr swizzling (equivalent to decVector3.yxxx).
        /// </summary>
        public decVector4 grrr => new decVector4(y, x, x, x);
        
        /// <summary>
        /// Returns decVector3.yxxy swizzling.
        /// </summary>
        public decVector4 yxxy => new decVector4(y, x, x, y);
        
        /// <summary>
        /// Returns decVector3.grrg swizzling (equivalent to decVector3.yxxy).
        /// </summary>
        public decVector4 grrg => new decVector4(y, x, x, y);
        
        /// <summary>
        /// Returns decVector3.yxxz swizzling.
        /// </summary>
        public decVector4 yxxz => new decVector4(y, x, x, z);
        
        /// <summary>
        /// Returns decVector3.grrb swizzling (equivalent to decVector3.yxxz).
        /// </summary>
        public decVector4 grrb => new decVector4(y, x, x, z);
        
        /// <summary>
        /// Returns decVector3.yxy swizzling.
        /// </summary>
        public decVector3 yxy => new decVector3(y, x, y);
        
        /// <summary>
        /// Returns decVector3.grg swizzling (equivalent to decVector3.yxy).
        /// </summary>
        public decVector3 grg => new decVector3(y, x, y);
        
        /// <summary>
        /// Returns decVector3.yxyx swizzling.
        /// </summary>
        public decVector4 yxyx => new decVector4(y, x, y, x);
        
        /// <summary>
        /// Returns decVector3.grgr swizzling (equivalent to decVector3.yxyx).
        /// </summary>
        public decVector4 grgr => new decVector4(y, x, y, x);
        
        /// <summary>
        /// Returns decVector3.yxyy swizzling.
        /// </summary>
        public decVector4 yxyy => new decVector4(y, x, y, y);
        
        /// <summary>
        /// Returns decVector3.grgg swizzling (equivalent to decVector3.yxyy).
        /// </summary>
        public decVector4 grgg => new decVector4(y, x, y, y);
        
        /// <summary>
        /// Returns decVector3.yxyz swizzling.
        /// </summary>
        public decVector4 yxyz => new decVector4(y, x, y, z);
        
        /// <summary>
        /// Returns decVector3.grgb swizzling (equivalent to decVector3.yxyz).
        /// </summary>
        public decVector4 grgb => new decVector4(y, x, y, z);
        
        /// <summary>
        /// Returns decVector3.yxz swizzling.
        /// </summary>
        public decVector3 yxz => new decVector3(y, x, z);
        
        /// <summary>
        /// Returns decVector3.grb swizzling (equivalent to decVector3.yxz).
        /// </summary>
        public decVector3 grb => new decVector3(y, x, z);
        
        /// <summary>
        /// Returns decVector3.yxzx swizzling.
        /// </summary>
        public decVector4 yxzx => new decVector4(y, x, z, x);
        
        /// <summary>
        /// Returns decVector3.grbr swizzling (equivalent to decVector3.yxzx).
        /// </summary>
        public decVector4 grbr => new decVector4(y, x, z, x);
        
        /// <summary>
        /// Returns decVector3.yxzy swizzling.
        /// </summary>
        public decVector4 yxzy => new decVector4(y, x, z, y);
        
        /// <summary>
        /// Returns decVector3.grbg swizzling (equivalent to decVector3.yxzy).
        /// </summary>
        public decVector4 grbg => new decVector4(y, x, z, y);
        
        /// <summary>
        /// Returns decVector3.yxzz swizzling.
        /// </summary>
        public decVector4 yxzz => new decVector4(y, x, z, z);
        
        /// <summary>
        /// Returns decVector3.grbb swizzling (equivalent to decVector3.yxzz).
        /// </summary>
        public decVector4 grbb => new decVector4(y, x, z, z);
        
        /// <summary>
        /// Returns decVector3.yy swizzling.
        /// </summary>
        public decVector2 yy => new decVector2(y, y);
        
        /// <summary>
        /// Returns decVector3.gg swizzling (equivalent to decVector3.yy).
        /// </summary>
        public decVector2 gg => new decVector2(y, y);
        
        /// <summary>
        /// Returns decVector3.yyx swizzling.
        /// </summary>
        public decVector3 yyx => new decVector3(y, y, x);
        
        /// <summary>
        /// Returns decVector3.ggr swizzling (equivalent to decVector3.yyx).
        /// </summary>
        public decVector3 ggr => new decVector3(y, y, x);
        
        /// <summary>
        /// Returns decVector3.yyxx swizzling.
        /// </summary>
        public decVector4 yyxx => new decVector4(y, y, x, x);
        
        /// <summary>
        /// Returns decVector3.ggrr swizzling (equivalent to decVector3.yyxx).
        /// </summary>
        public decVector4 ggrr => new decVector4(y, y, x, x);
        
        /// <summary>
        /// Returns decVector3.yyxy swizzling.
        /// </summary>
        public decVector4 yyxy => new decVector4(y, y, x, y);
        
        /// <summary>
        /// Returns decVector3.ggrg swizzling (equivalent to decVector3.yyxy).
        /// </summary>
        public decVector4 ggrg => new decVector4(y, y, x, y);
        
        /// <summary>
        /// Returns decVector3.yyxz swizzling.
        /// </summary>
        public decVector4 yyxz => new decVector4(y, y, x, z);
        
        /// <summary>
        /// Returns decVector3.ggrb swizzling (equivalent to decVector3.yyxz).
        /// </summary>
        public decVector4 ggrb => new decVector4(y, y, x, z);
        
        /// <summary>
        /// Returns decVector3.yyy swizzling.
        /// </summary>
        public decVector3 yyy => new decVector3(y, y, y);
        
        /// <summary>
        /// Returns decVector3.ggg swizzling (equivalent to decVector3.yyy).
        /// </summary>
        public decVector3 ggg => new decVector3(y, y, y);
        
        /// <summary>
        /// Returns decVector3.yyyx swizzling.
        /// </summary>
        public decVector4 yyyx => new decVector4(y, y, y, x);
        
        /// <summary>
        /// Returns decVector3.gggr swizzling (equivalent to decVector3.yyyx).
        /// </summary>
        public decVector4 gggr => new decVector4(y, y, y, x);
        
        /// <summary>
        /// Returns decVector3.yyyy swizzling.
        /// </summary>
        public decVector4 yyyy => new decVector4(y, y, y, y);
        
        /// <summary>
        /// Returns decVector3.gggg swizzling (equivalent to decVector3.yyyy).
        /// </summary>
        public decVector4 gggg => new decVector4(y, y, y, y);
        
        /// <summary>
        /// Returns decVector3.yyyz swizzling.
        /// </summary>
        public decVector4 yyyz => new decVector4(y, y, y, z);
        
        /// <summary>
        /// Returns decVector3.gggb swizzling (equivalent to decVector3.yyyz).
        /// </summary>
        public decVector4 gggb => new decVector4(y, y, y, z);
        
        /// <summary>
        /// Returns decVector3.yyz swizzling.
        /// </summary>
        public decVector3 yyz => new decVector3(y, y, z);
        
        /// <summary>
        /// Returns decVector3.ggb swizzling (equivalent to decVector3.yyz).
        /// </summary>
        public decVector3 ggb => new decVector3(y, y, z);
        
        /// <summary>
        /// Returns decVector3.yyzx swizzling.
        /// </summary>
        public decVector4 yyzx => new decVector4(y, y, z, x);
        
        /// <summary>
        /// Returns decVector3.ggbr swizzling (equivalent to decVector3.yyzx).
        /// </summary>
        public decVector4 ggbr => new decVector4(y, y, z, x);
        
        /// <summary>
        /// Returns decVector3.yyzy swizzling.
        /// </summary>
        public decVector4 yyzy => new decVector4(y, y, z, y);
        
        /// <summary>
        /// Returns decVector3.ggbg swizzling (equivalent to decVector3.yyzy).
        /// </summary>
        public decVector4 ggbg => new decVector4(y, y, z, y);
        
        /// <summary>
        /// Returns decVector3.yyzz swizzling.
        /// </summary>
        public decVector4 yyzz => new decVector4(y, y, z, z);
        
        /// <summary>
        /// Returns decVector3.ggbb swizzling (equivalent to decVector3.yyzz).
        /// </summary>
        public decVector4 ggbb => new decVector4(y, y, z, z);
        
        /// <summary>
        /// Returns decVector3.yz swizzling.
        /// </summary>
        public decVector2 yz => new decVector2(y, z);
        
        /// <summary>
        /// Returns decVector3.gb swizzling (equivalent to decVector3.yz).
        /// </summary>
        public decVector2 gb => new decVector2(y, z);
        
        /// <summary>
        /// Returns decVector3.yzx swizzling.
        /// </summary>
        public decVector3 yzx => new decVector3(y, z, x);
        
        /// <summary>
        /// Returns decVector3.gbr swizzling (equivalent to decVector3.yzx).
        /// </summary>
        public decVector3 gbr => new decVector3(y, z, x);
        
        /// <summary>
        /// Returns decVector3.yzxx swizzling.
        /// </summary>
        public decVector4 yzxx => new decVector4(y, z, x, x);
        
        /// <summary>
        /// Returns decVector3.gbrr swizzling (equivalent to decVector3.yzxx).
        /// </summary>
        public decVector4 gbrr => new decVector4(y, z, x, x);
        
        /// <summary>
        /// Returns decVector3.yzxy swizzling.
        /// </summary>
        public decVector4 yzxy => new decVector4(y, z, x, y);
        
        /// <summary>
        /// Returns decVector3.gbrg swizzling (equivalent to decVector3.yzxy).
        /// </summary>
        public decVector4 gbrg => new decVector4(y, z, x, y);
        
        /// <summary>
        /// Returns decVector3.yzxz swizzling.
        /// </summary>
        public decVector4 yzxz => new decVector4(y, z, x, z);
        
        /// <summary>
        /// Returns decVector3.gbrb swizzling (equivalent to decVector3.yzxz).
        /// </summary>
        public decVector4 gbrb => new decVector4(y, z, x, z);
        
        /// <summary>
        /// Returns decVector3.yzy swizzling.
        /// </summary>
        public decVector3 yzy => new decVector3(y, z, y);
        
        /// <summary>
        /// Returns decVector3.gbg swizzling (equivalent to decVector3.yzy).
        /// </summary>
        public decVector3 gbg => new decVector3(y, z, y);
        
        /// <summary>
        /// Returns decVector3.yzyx swizzling.
        /// </summary>
        public decVector4 yzyx => new decVector4(y, z, y, x);
        
        /// <summary>
        /// Returns decVector3.gbgr swizzling (equivalent to decVector3.yzyx).
        /// </summary>
        public decVector4 gbgr => new decVector4(y, z, y, x);
        
        /// <summary>
        /// Returns decVector3.yzyy swizzling.
        /// </summary>
        public decVector4 yzyy => new decVector4(y, z, y, y);
        
        /// <summary>
        /// Returns decVector3.gbgg swizzling (equivalent to decVector3.yzyy).
        /// </summary>
        public decVector4 gbgg => new decVector4(y, z, y, y);
        
        /// <summary>
        /// Returns decVector3.yzyz swizzling.
        /// </summary>
        public decVector4 yzyz => new decVector4(y, z, y, z);
        
        /// <summary>
        /// Returns decVector3.gbgb swizzling (equivalent to decVector3.yzyz).
        /// </summary>
        public decVector4 gbgb => new decVector4(y, z, y, z);
        
        /// <summary>
        /// Returns decVector3.yzz swizzling.
        /// </summary>
        public decVector3 yzz => new decVector3(y, z, z);
        
        /// <summary>
        /// Returns decVector3.gbb swizzling (equivalent to decVector3.yzz).
        /// </summary>
        public decVector3 gbb => new decVector3(y, z, z);
        
        /// <summary>
        /// Returns decVector3.yzzx swizzling.
        /// </summary>
        public decVector4 yzzx => new decVector4(y, z, z, x);
        
        /// <summary>
        /// Returns decVector3.gbbr swizzling (equivalent to decVector3.yzzx).
        /// </summary>
        public decVector4 gbbr => new decVector4(y, z, z, x);
        
        /// <summary>
        /// Returns decVector3.yzzy swizzling.
        /// </summary>
        public decVector4 yzzy => new decVector4(y, z, z, y);
        
        /// <summary>
        /// Returns decVector3.gbbg swizzling (equivalent to decVector3.yzzy).
        /// </summary>
        public decVector4 gbbg => new decVector4(y, z, z, y);
        
        /// <summary>
        /// Returns decVector3.yzzz swizzling.
        /// </summary>
        public decVector4 yzzz => new decVector4(y, z, z, z);
        
        /// <summary>
        /// Returns decVector3.gbbb swizzling (equivalent to decVector3.yzzz).
        /// </summary>
        public decVector4 gbbb => new decVector4(y, z, z, z);
        
        /// <summary>
        /// Returns decVector3.zx swizzling.
        /// </summary>
        public decVector2 zx => new decVector2(z, x);
        
        /// <summary>
        /// Returns decVector3.br swizzling (equivalent to decVector3.zx).
        /// </summary>
        public decVector2 br => new decVector2(z, x);
        
        /// <summary>
        /// Returns decVector3.zxx swizzling.
        /// </summary>
        public decVector3 zxx => new decVector3(z, x, x);
        
        /// <summary>
        /// Returns decVector3.brr swizzling (equivalent to decVector3.zxx).
        /// </summary>
        public decVector3 brr => new decVector3(z, x, x);
        
        /// <summary>
        /// Returns decVector3.zxxx swizzling.
        /// </summary>
        public decVector4 zxxx => new decVector4(z, x, x, x);
        
        /// <summary>
        /// Returns decVector3.brrr swizzling (equivalent to decVector3.zxxx).
        /// </summary>
        public decVector4 brrr => new decVector4(z, x, x, x);
        
        /// <summary>
        /// Returns decVector3.zxxy swizzling.
        /// </summary>
        public decVector4 zxxy => new decVector4(z, x, x, y);
        
        /// <summary>
        /// Returns decVector3.brrg swizzling (equivalent to decVector3.zxxy).
        /// </summary>
        public decVector4 brrg => new decVector4(z, x, x, y);
        
        /// <summary>
        /// Returns decVector3.zxxz swizzling.
        /// </summary>
        public decVector4 zxxz => new decVector4(z, x, x, z);
        
        /// <summary>
        /// Returns decVector3.brrb swizzling (equivalent to decVector3.zxxz).
        /// </summary>
        public decVector4 brrb => new decVector4(z, x, x, z);
        
        /// <summary>
        /// Returns decVector3.zxy swizzling.
        /// </summary>
        public decVector3 zxy => new decVector3(z, x, y);
        
        /// <summary>
        /// Returns decVector3.brg swizzling (equivalent to decVector3.zxy).
        /// </summary>
        public decVector3 brg => new decVector3(z, x, y);
        
        /// <summary>
        /// Returns decVector3.zxyx swizzling.
        /// </summary>
        public decVector4 zxyx => new decVector4(z, x, y, x);
        
        /// <summary>
        /// Returns decVector3.brgr swizzling (equivalent to decVector3.zxyx).
        /// </summary>
        public decVector4 brgr => new decVector4(z, x, y, x);
        
        /// <summary>
        /// Returns decVector3.zxyy swizzling.
        /// </summary>
        public decVector4 zxyy => new decVector4(z, x, y, y);
        
        /// <summary>
        /// Returns decVector3.brgg swizzling (equivalent to decVector3.zxyy).
        /// </summary>
        public decVector4 brgg => new decVector4(z, x, y, y);
        
        /// <summary>
        /// Returns decVector3.zxyz swizzling.
        /// </summary>
        public decVector4 zxyz => new decVector4(z, x, y, z);
        
        /// <summary>
        /// Returns decVector3.brgb swizzling (equivalent to decVector3.zxyz).
        /// </summary>
        public decVector4 brgb => new decVector4(z, x, y, z);
        
        /// <summary>
        /// Returns decVector3.zxz swizzling.
        /// </summary>
        public decVector3 zxz => new decVector3(z, x, z);
        
        /// <summary>
        /// Returns decVector3.brb swizzling (equivalent to decVector3.zxz).
        /// </summary>
        public decVector3 brb => new decVector3(z, x, z);
        
        /// <summary>
        /// Returns decVector3.zxzx swizzling.
        /// </summary>
        public decVector4 zxzx => new decVector4(z, x, z, x);
        
        /// <summary>
        /// Returns decVector3.brbr swizzling (equivalent to decVector3.zxzx).
        /// </summary>
        public decVector4 brbr => new decVector4(z, x, z, x);
        
        /// <summary>
        /// Returns decVector3.zxzy swizzling.
        /// </summary>
        public decVector4 zxzy => new decVector4(z, x, z, y);
        
        /// <summary>
        /// Returns decVector3.brbg swizzling (equivalent to decVector3.zxzy).
        /// </summary>
        public decVector4 brbg => new decVector4(z, x, z, y);
        
        /// <summary>
        /// Returns decVector3.zxzz swizzling.
        /// </summary>
        public decVector4 zxzz => new decVector4(z, x, z, z);
        
        /// <summary>
        /// Returns decVector3.brbb swizzling (equivalent to decVector3.zxzz).
        /// </summary>
        public decVector4 brbb => new decVector4(z, x, z, z);
        
        /// <summary>
        /// Returns decVector3.zy swizzling.
        /// </summary>
        public decVector2 zy => new decVector2(z, y);
        
        /// <summary>
        /// Returns decVector3.bg swizzling (equivalent to decVector3.zy).
        /// </summary>
        public decVector2 bg => new decVector2(z, y);
        
        /// <summary>
        /// Returns decVector3.zyx swizzling.
        /// </summary>
        public decVector3 zyx => new decVector3(z, y, x);
        
        /// <summary>
        /// Returns decVector3.bgr swizzling (equivalent to decVector3.zyx).
        /// </summary>
        public decVector3 bgr => new decVector3(z, y, x);
        
        /// <summary>
        /// Returns decVector3.zyxx swizzling.
        /// </summary>
        public decVector4 zyxx => new decVector4(z, y, x, x);
        
        /// <summary>
        /// Returns decVector3.bgrr swizzling (equivalent to decVector3.zyxx).
        /// </summary>
        public decVector4 bgrr => new decVector4(z, y, x, x);
        
        /// <summary>
        /// Returns decVector3.zyxy swizzling.
        /// </summary>
        public decVector4 zyxy => new decVector4(z, y, x, y);
        
        /// <summary>
        /// Returns decVector3.bgrg swizzling (equivalent to decVector3.zyxy).
        /// </summary>
        public decVector4 bgrg => new decVector4(z, y, x, y);
        
        /// <summary>
        /// Returns decVector3.zyxz swizzling.
        /// </summary>
        public decVector4 zyxz => new decVector4(z, y, x, z);
        
        /// <summary>
        /// Returns decVector3.bgrb swizzling (equivalent to decVector3.zyxz).
        /// </summary>
        public decVector4 bgrb => new decVector4(z, y, x, z);
        
        /// <summary>
        /// Returns decVector3.zyy swizzling.
        /// </summary>
        public decVector3 zyy => new decVector3(z, y, y);
        
        /// <summary>
        /// Returns decVector3.bgg swizzling (equivalent to decVector3.zyy).
        /// </summary>
        public decVector3 bgg => new decVector3(z, y, y);
        
        /// <summary>
        /// Returns decVector3.zyyx swizzling.
        /// </summary>
        public decVector4 zyyx => new decVector4(z, y, y, x);
        
        /// <summary>
        /// Returns decVector3.bggr swizzling (equivalent to decVector3.zyyx).
        /// </summary>
        public decVector4 bggr => new decVector4(z, y, y, x);
        
        /// <summary>
        /// Returns decVector3.zyyy swizzling.
        /// </summary>
        public decVector4 zyyy => new decVector4(z, y, y, y);
        
        /// <summary>
        /// Returns decVector3.bggg swizzling (equivalent to decVector3.zyyy).
        /// </summary>
        public decVector4 bggg => new decVector4(z, y, y, y);
        
        /// <summary>
        /// Returns decVector3.zyyz swizzling.
        /// </summary>
        public decVector4 zyyz => new decVector4(z, y, y, z);
        
        /// <summary>
        /// Returns decVector3.bggb swizzling (equivalent to decVector3.zyyz).
        /// </summary>
        public decVector4 bggb => new decVector4(z, y, y, z);
        
        /// <summary>
        /// Returns decVector3.zyz swizzling.
        /// </summary>
        public decVector3 zyz => new decVector3(z, y, z);
        
        /// <summary>
        /// Returns decVector3.bgb swizzling (equivalent to decVector3.zyz).
        /// </summary>
        public decVector3 bgb => new decVector3(z, y, z);
        
        /// <summary>
        /// Returns decVector3.zyzx swizzling.
        /// </summary>
        public decVector4 zyzx => new decVector4(z, y, z, x);
        
        /// <summary>
        /// Returns decVector3.bgbr swizzling (equivalent to decVector3.zyzx).
        /// </summary>
        public decVector4 bgbr => new decVector4(z, y, z, x);
        
        /// <summary>
        /// Returns decVector3.zyzy swizzling.
        /// </summary>
        public decVector4 zyzy => new decVector4(z, y, z, y);
        
        /// <summary>
        /// Returns decVector3.bgbg swizzling (equivalent to decVector3.zyzy).
        /// </summary>
        public decVector4 bgbg => new decVector4(z, y, z, y);
        
        /// <summary>
        /// Returns decVector3.zyzz swizzling.
        /// </summary>
        public decVector4 zyzz => new decVector4(z, y, z, z);
        
        /// <summary>
        /// Returns decVector3.bgbb swizzling (equivalent to decVector3.zyzz).
        /// </summary>
        public decVector4 bgbb => new decVector4(z, y, z, z);
        
        /// <summary>
        /// Returns decVector3.zz swizzling.
        /// </summary>
        public decVector2 zz => new decVector2(z, z);
        
        /// <summary>
        /// Returns decVector3.bb swizzling (equivalent to decVector3.zz).
        /// </summary>
        public decVector2 bb => new decVector2(z, z);
        
        /// <summary>
        /// Returns decVector3.zzx swizzling.
        /// </summary>
        public decVector3 zzx => new decVector3(z, z, x);
        
        /// <summary>
        /// Returns decVector3.bbr swizzling (equivalent to decVector3.zzx).
        /// </summary>
        public decVector3 bbr => new decVector3(z, z, x);
        
        /// <summary>
        /// Returns decVector3.zzxx swizzling.
        /// </summary>
        public decVector4 zzxx => new decVector4(z, z, x, x);
        
        /// <summary>
        /// Returns decVector3.bbrr swizzling (equivalent to decVector3.zzxx).
        /// </summary>
        public decVector4 bbrr => new decVector4(z, z, x, x);
        
        /// <summary>
        /// Returns decVector3.zzxy swizzling.
        /// </summary>
        public decVector4 zzxy => new decVector4(z, z, x, y);
        
        /// <summary>
        /// Returns decVector3.bbrg swizzling (equivalent to decVector3.zzxy).
        /// </summary>
        public decVector4 bbrg => new decVector4(z, z, x, y);
        
        /// <summary>
        /// Returns decVector3.zzxz swizzling.
        /// </summary>
        public decVector4 zzxz => new decVector4(z, z, x, z);
        
        /// <summary>
        /// Returns decVector3.bbrb swizzling (equivalent to decVector3.zzxz).
        /// </summary>
        public decVector4 bbrb => new decVector4(z, z, x, z);
        
        /// <summary>
        /// Returns decVector3.zzy swizzling.
        /// </summary>
        public decVector3 zzy => new decVector3(z, z, y);
        
        /// <summary>
        /// Returns decVector3.bbg swizzling (equivalent to decVector3.zzy).
        /// </summary>
        public decVector3 bbg => new decVector3(z, z, y);
        
        /// <summary>
        /// Returns decVector3.zzyx swizzling.
        /// </summary>
        public decVector4 zzyx => new decVector4(z, z, y, x);
        
        /// <summary>
        /// Returns decVector3.bbgr swizzling (equivalent to decVector3.zzyx).
        /// </summary>
        public decVector4 bbgr => new decVector4(z, z, y, x);
        
        /// <summary>
        /// Returns decVector3.zzyy swizzling.
        /// </summary>
        public decVector4 zzyy => new decVector4(z, z, y, y);
        
        /// <summary>
        /// Returns decVector3.bbgg swizzling (equivalent to decVector3.zzyy).
        /// </summary>
        public decVector4 bbgg => new decVector4(z, z, y, y);
        
        /// <summary>
        /// Returns decVector3.zzyz swizzling.
        /// </summary>
        public decVector4 zzyz => new decVector4(z, z, y, z);
        
        /// <summary>
        /// Returns decVector3.bbgb swizzling (equivalent to decVector3.zzyz).
        /// </summary>
        public decVector4 bbgb => new decVector4(z, z, y, z);
        
        /// <summary>
        /// Returns decVector3.zzz swizzling.
        /// </summary>
        public decVector3 zzz => new decVector3(z, z, z);
        
        /// <summary>
        /// Returns decVector3.bbb swizzling (equivalent to decVector3.zzz).
        /// </summary>
        public decVector3 bbb => new decVector3(z, z, z);
        
        /// <summary>
        /// Returns decVector3.zzzx swizzling.
        /// </summary>
        public decVector4 zzzx => new decVector4(z, z, z, x);
        
        /// <summary>
        /// Returns decVector3.bbbr swizzling (equivalent to decVector3.zzzx).
        /// </summary>
        public decVector4 bbbr => new decVector4(z, z, z, x);
        
        /// <summary>
        /// Returns decVector3.zzzy swizzling.
        /// </summary>
        public decVector4 zzzy => new decVector4(z, z, z, y);
        
        /// <summary>
        /// Returns decVector3.bbbg swizzling (equivalent to decVector3.zzzy).
        /// </summary>
        public decVector4 bbbg => new decVector4(z, z, z, y);
        
        /// <summary>
        /// Returns decVector3.zzzz swizzling.
        /// </summary>
        public decVector4 zzzz => new decVector4(z, z, z, z);
        
        /// <summary>
        /// Returns decVector3.bbbb swizzling (equivalent to decVector3.zzzz).
        /// </summary>
        public decVector4 bbbb => new decVector4(z, z, z, z);

        #endregion

    }
}
