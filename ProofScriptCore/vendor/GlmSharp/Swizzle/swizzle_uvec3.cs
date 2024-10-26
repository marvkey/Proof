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
    /// Temporary Vector of type uint with 3 components, used for implementing swizzling for uVector3.
    /// </summary>
    
   
    [StructLayout(LayoutKind.Sequential)]
    public struct swizzle_uVector3
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
        
        /// <summary>
        /// z-component
        /// </summary>
        
        internal readonly uint z;

        #endregion


        #region Constructors
        
        /// <summary>
        /// Constructor for swizzle_uVector3.
        /// </summary>
        internal swizzle_uVector3(uint x, uint y, uint z)
        {
            this.x = x;
            this.y = y;
            this.z = z;
        }

        #endregion


        #region Properties
        
        /// <summary>
        /// Returns uVector3.xx swizzling.
        /// </summary>
        public uVector2 xx => new uVector2(x, x);
        
        /// <summary>
        /// Returns uVector3.rr swizzling (equivalent to uVector3.xx).
        /// </summary>
        public uVector2 rr => new uVector2(x, x);
        
        /// <summary>
        /// Returns uVector3.xxx swizzling.
        /// </summary>
        public uVector3 xxx => new uVector3(x, x, x);
        
        /// <summary>
        /// Returns uVector3.rrr swizzling (equivalent to uVector3.xxx).
        /// </summary>
        public uVector3 rrr => new uVector3(x, x, x);
        
        /// <summary>
        /// Returns uVector3.xxxx swizzling.
        /// </summary>
        public uVector4 xxxx => new uVector4(x, x, x, x);
        
        /// <summary>
        /// Returns uVector3.rrrr swizzling (equivalent to uVector3.xxxx).
        /// </summary>
        public uVector4 rrrr => new uVector4(x, x, x, x);
        
        /// <summary>
        /// Returns uVector3.xxxy swizzling.
        /// </summary>
        public uVector4 xxxy => new uVector4(x, x, x, y);
        
        /// <summary>
        /// Returns uVector3.rrrg swizzling (equivalent to uVector3.xxxy).
        /// </summary>
        public uVector4 rrrg => new uVector4(x, x, x, y);
        
        /// <summary>
        /// Returns uVector3.xxxz swizzling.
        /// </summary>
        public uVector4 xxxz => new uVector4(x, x, x, z);
        
        /// <summary>
        /// Returns uVector3.rrrb swizzling (equivalent to uVector3.xxxz).
        /// </summary>
        public uVector4 rrrb => new uVector4(x, x, x, z);
        
        /// <summary>
        /// Returns uVector3.xxy swizzling.
        /// </summary>
        public uVector3 xxy => new uVector3(x, x, y);
        
        /// <summary>
        /// Returns uVector3.rrg swizzling (equivalent to uVector3.xxy).
        /// </summary>
        public uVector3 rrg => new uVector3(x, x, y);
        
        /// <summary>
        /// Returns uVector3.xxyx swizzling.
        /// </summary>
        public uVector4 xxyx => new uVector4(x, x, y, x);
        
        /// <summary>
        /// Returns uVector3.rrgr swizzling (equivalent to uVector3.xxyx).
        /// </summary>
        public uVector4 rrgr => new uVector4(x, x, y, x);
        
        /// <summary>
        /// Returns uVector3.xxyy swizzling.
        /// </summary>
        public uVector4 xxyy => new uVector4(x, x, y, y);
        
        /// <summary>
        /// Returns uVector3.rrgg swizzling (equivalent to uVector3.xxyy).
        /// </summary>
        public uVector4 rrgg => new uVector4(x, x, y, y);
        
        /// <summary>
        /// Returns uVector3.xxyz swizzling.
        /// </summary>
        public uVector4 xxyz => new uVector4(x, x, y, z);
        
        /// <summary>
        /// Returns uVector3.rrgb swizzling (equivalent to uVector3.xxyz).
        /// </summary>
        public uVector4 rrgb => new uVector4(x, x, y, z);
        
        /// <summary>
        /// Returns uVector3.xxz swizzling.
        /// </summary>
        public uVector3 xxz => new uVector3(x, x, z);
        
        /// <summary>
        /// Returns uVector3.rrb swizzling (equivalent to uVector3.xxz).
        /// </summary>
        public uVector3 rrb => new uVector3(x, x, z);
        
        /// <summary>
        /// Returns uVector3.xxzx swizzling.
        /// </summary>
        public uVector4 xxzx => new uVector4(x, x, z, x);
        
        /// <summary>
        /// Returns uVector3.rrbr swizzling (equivalent to uVector3.xxzx).
        /// </summary>
        public uVector4 rrbr => new uVector4(x, x, z, x);
        
        /// <summary>
        /// Returns uVector3.xxzy swizzling.
        /// </summary>
        public uVector4 xxzy => new uVector4(x, x, z, y);
        
        /// <summary>
        /// Returns uVector3.rrbg swizzling (equivalent to uVector3.xxzy).
        /// </summary>
        public uVector4 rrbg => new uVector4(x, x, z, y);
        
        /// <summary>
        /// Returns uVector3.xxzz swizzling.
        /// </summary>
        public uVector4 xxzz => new uVector4(x, x, z, z);
        
        /// <summary>
        /// Returns uVector3.rrbb swizzling (equivalent to uVector3.xxzz).
        /// </summary>
        public uVector4 rrbb => new uVector4(x, x, z, z);
        
        /// <summary>
        /// Returns uVector3.xy swizzling.
        /// </summary>
        public uVector2 xy => new uVector2(x, y);
        
        /// <summary>
        /// Returns uVector3.rg swizzling (equivalent to uVector3.xy).
        /// </summary>
        public uVector2 rg => new uVector2(x, y);
        
        /// <summary>
        /// Returns uVector3.xyx swizzling.
        /// </summary>
        public uVector3 xyx => new uVector3(x, y, x);
        
        /// <summary>
        /// Returns uVector3.rgr swizzling (equivalent to uVector3.xyx).
        /// </summary>
        public uVector3 rgr => new uVector3(x, y, x);
        
        /// <summary>
        /// Returns uVector3.xyxx swizzling.
        /// </summary>
        public uVector4 xyxx => new uVector4(x, y, x, x);
        
        /// <summary>
        /// Returns uVector3.rgrr swizzling (equivalent to uVector3.xyxx).
        /// </summary>
        public uVector4 rgrr => new uVector4(x, y, x, x);
        
        /// <summary>
        /// Returns uVector3.xyxy swizzling.
        /// </summary>
        public uVector4 xyxy => new uVector4(x, y, x, y);
        
        /// <summary>
        /// Returns uVector3.rgrg swizzling (equivalent to uVector3.xyxy).
        /// </summary>
        public uVector4 rgrg => new uVector4(x, y, x, y);
        
        /// <summary>
        /// Returns uVector3.xyxz swizzling.
        /// </summary>
        public uVector4 xyxz => new uVector4(x, y, x, z);
        
        /// <summary>
        /// Returns uVector3.rgrb swizzling (equivalent to uVector3.xyxz).
        /// </summary>
        public uVector4 rgrb => new uVector4(x, y, x, z);
        
        /// <summary>
        /// Returns uVector3.xyy swizzling.
        /// </summary>
        public uVector3 xyy => new uVector3(x, y, y);
        
        /// <summary>
        /// Returns uVector3.rgg swizzling (equivalent to uVector3.xyy).
        /// </summary>
        public uVector3 rgg => new uVector3(x, y, y);
        
        /// <summary>
        /// Returns uVector3.xyyx swizzling.
        /// </summary>
        public uVector4 xyyx => new uVector4(x, y, y, x);
        
        /// <summary>
        /// Returns uVector3.rggr swizzling (equivalent to uVector3.xyyx).
        /// </summary>
        public uVector4 rggr => new uVector4(x, y, y, x);
        
        /// <summary>
        /// Returns uVector3.xyyy swizzling.
        /// </summary>
        public uVector4 xyyy => new uVector4(x, y, y, y);
        
        /// <summary>
        /// Returns uVector3.rggg swizzling (equivalent to uVector3.xyyy).
        /// </summary>
        public uVector4 rggg => new uVector4(x, y, y, y);
        
        /// <summary>
        /// Returns uVector3.xyyz swizzling.
        /// </summary>
        public uVector4 xyyz => new uVector4(x, y, y, z);
        
        /// <summary>
        /// Returns uVector3.rggb swizzling (equivalent to uVector3.xyyz).
        /// </summary>
        public uVector4 rggb => new uVector4(x, y, y, z);
        
        /// <summary>
        /// Returns uVector3.xyz swizzling.
        /// </summary>
        public uVector3 xyz => new uVector3(x, y, z);
        
        /// <summary>
        /// Returns uVector3.rgb swizzling (equivalent to uVector3.xyz).
        /// </summary>
        public uVector3 rgb => new uVector3(x, y, z);
        
        /// <summary>
        /// Returns uVector3.xyzx swizzling.
        /// </summary>
        public uVector4 xyzx => new uVector4(x, y, z, x);
        
        /// <summary>
        /// Returns uVector3.rgbr swizzling (equivalent to uVector3.xyzx).
        /// </summary>
        public uVector4 rgbr => new uVector4(x, y, z, x);
        
        /// <summary>
        /// Returns uVector3.xyzy swizzling.
        /// </summary>
        public uVector4 xyzy => new uVector4(x, y, z, y);
        
        /// <summary>
        /// Returns uVector3.rgbg swizzling (equivalent to uVector3.xyzy).
        /// </summary>
        public uVector4 rgbg => new uVector4(x, y, z, y);
        
        /// <summary>
        /// Returns uVector3.xyzz swizzling.
        /// </summary>
        public uVector4 xyzz => new uVector4(x, y, z, z);
        
        /// <summary>
        /// Returns uVector3.rgbb swizzling (equivalent to uVector3.xyzz).
        /// </summary>
        public uVector4 rgbb => new uVector4(x, y, z, z);
        
        /// <summary>
        /// Returns uVector3.xz swizzling.
        /// </summary>
        public uVector2 xz => new uVector2(x, z);
        
        /// <summary>
        /// Returns uVector3.rb swizzling (equivalent to uVector3.xz).
        /// </summary>
        public uVector2 rb => new uVector2(x, z);
        
        /// <summary>
        /// Returns uVector3.xzx swizzling.
        /// </summary>
        public uVector3 xzx => new uVector3(x, z, x);
        
        /// <summary>
        /// Returns uVector3.rbr swizzling (equivalent to uVector3.xzx).
        /// </summary>
        public uVector3 rbr => new uVector3(x, z, x);
        
        /// <summary>
        /// Returns uVector3.xzxx swizzling.
        /// </summary>
        public uVector4 xzxx => new uVector4(x, z, x, x);
        
        /// <summary>
        /// Returns uVector3.rbrr swizzling (equivalent to uVector3.xzxx).
        /// </summary>
        public uVector4 rbrr => new uVector4(x, z, x, x);
        
        /// <summary>
        /// Returns uVector3.xzxy swizzling.
        /// </summary>
        public uVector4 xzxy => new uVector4(x, z, x, y);
        
        /// <summary>
        /// Returns uVector3.rbrg swizzling (equivalent to uVector3.xzxy).
        /// </summary>
        public uVector4 rbrg => new uVector4(x, z, x, y);
        
        /// <summary>
        /// Returns uVector3.xzxz swizzling.
        /// </summary>
        public uVector4 xzxz => new uVector4(x, z, x, z);
        
        /// <summary>
        /// Returns uVector3.rbrb swizzling (equivalent to uVector3.xzxz).
        /// </summary>
        public uVector4 rbrb => new uVector4(x, z, x, z);
        
        /// <summary>
        /// Returns uVector3.xzy swizzling.
        /// </summary>
        public uVector3 xzy => new uVector3(x, z, y);
        
        /// <summary>
        /// Returns uVector3.rbg swizzling (equivalent to uVector3.xzy).
        /// </summary>
        public uVector3 rbg => new uVector3(x, z, y);
        
        /// <summary>
        /// Returns uVector3.xzyx swizzling.
        /// </summary>
        public uVector4 xzyx => new uVector4(x, z, y, x);
        
        /// <summary>
        /// Returns uVector3.rbgr swizzling (equivalent to uVector3.xzyx).
        /// </summary>
        public uVector4 rbgr => new uVector4(x, z, y, x);
        
        /// <summary>
        /// Returns uVector3.xzyy swizzling.
        /// </summary>
        public uVector4 xzyy => new uVector4(x, z, y, y);
        
        /// <summary>
        /// Returns uVector3.rbgg swizzling (equivalent to uVector3.xzyy).
        /// </summary>
        public uVector4 rbgg => new uVector4(x, z, y, y);
        
        /// <summary>
        /// Returns uVector3.xzyz swizzling.
        /// </summary>
        public uVector4 xzyz => new uVector4(x, z, y, z);
        
        /// <summary>
        /// Returns uVector3.rbgb swizzling (equivalent to uVector3.xzyz).
        /// </summary>
        public uVector4 rbgb => new uVector4(x, z, y, z);
        
        /// <summary>
        /// Returns uVector3.xzz swizzling.
        /// </summary>
        public uVector3 xzz => new uVector3(x, z, z);
        
        /// <summary>
        /// Returns uVector3.rbb swizzling (equivalent to uVector3.xzz).
        /// </summary>
        public uVector3 rbb => new uVector3(x, z, z);
        
        /// <summary>
        /// Returns uVector3.xzzx swizzling.
        /// </summary>
        public uVector4 xzzx => new uVector4(x, z, z, x);
        
        /// <summary>
        /// Returns uVector3.rbbr swizzling (equivalent to uVector3.xzzx).
        /// </summary>
        public uVector4 rbbr => new uVector4(x, z, z, x);
        
        /// <summary>
        /// Returns uVector3.xzzy swizzling.
        /// </summary>
        public uVector4 xzzy => new uVector4(x, z, z, y);
        
        /// <summary>
        /// Returns uVector3.rbbg swizzling (equivalent to uVector3.xzzy).
        /// </summary>
        public uVector4 rbbg => new uVector4(x, z, z, y);
        
        /// <summary>
        /// Returns uVector3.xzzz swizzling.
        /// </summary>
        public uVector4 xzzz => new uVector4(x, z, z, z);
        
        /// <summary>
        /// Returns uVector3.rbbb swizzling (equivalent to uVector3.xzzz).
        /// </summary>
        public uVector4 rbbb => new uVector4(x, z, z, z);
        
        /// <summary>
        /// Returns uVector3.yx swizzling.
        /// </summary>
        public uVector2 yx => new uVector2(y, x);
        
        /// <summary>
        /// Returns uVector3.gr swizzling (equivalent to uVector3.yx).
        /// </summary>
        public uVector2 gr => new uVector2(y, x);
        
        /// <summary>
        /// Returns uVector3.yxx swizzling.
        /// </summary>
        public uVector3 yxx => new uVector3(y, x, x);
        
        /// <summary>
        /// Returns uVector3.grr swizzling (equivalent to uVector3.yxx).
        /// </summary>
        public uVector3 grr => new uVector3(y, x, x);
        
        /// <summary>
        /// Returns uVector3.yxxx swizzling.
        /// </summary>
        public uVector4 yxxx => new uVector4(y, x, x, x);
        
        /// <summary>
        /// Returns uVector3.grrr swizzling (equivalent to uVector3.yxxx).
        /// </summary>
        public uVector4 grrr => new uVector4(y, x, x, x);
        
        /// <summary>
        /// Returns uVector3.yxxy swizzling.
        /// </summary>
        public uVector4 yxxy => new uVector4(y, x, x, y);
        
        /// <summary>
        /// Returns uVector3.grrg swizzling (equivalent to uVector3.yxxy).
        /// </summary>
        public uVector4 grrg => new uVector4(y, x, x, y);
        
        /// <summary>
        /// Returns uVector3.yxxz swizzling.
        /// </summary>
        public uVector4 yxxz => new uVector4(y, x, x, z);
        
        /// <summary>
        /// Returns uVector3.grrb swizzling (equivalent to uVector3.yxxz).
        /// </summary>
        public uVector4 grrb => new uVector4(y, x, x, z);
        
        /// <summary>
        /// Returns uVector3.yxy swizzling.
        /// </summary>
        public uVector3 yxy => new uVector3(y, x, y);
        
        /// <summary>
        /// Returns uVector3.grg swizzling (equivalent to uVector3.yxy).
        /// </summary>
        public uVector3 grg => new uVector3(y, x, y);
        
        /// <summary>
        /// Returns uVector3.yxyx swizzling.
        /// </summary>
        public uVector4 yxyx => new uVector4(y, x, y, x);
        
        /// <summary>
        /// Returns uVector3.grgr swizzling (equivalent to uVector3.yxyx).
        /// </summary>
        public uVector4 grgr => new uVector4(y, x, y, x);
        
        /// <summary>
        /// Returns uVector3.yxyy swizzling.
        /// </summary>
        public uVector4 yxyy => new uVector4(y, x, y, y);
        
        /// <summary>
        /// Returns uVector3.grgg swizzling (equivalent to uVector3.yxyy).
        /// </summary>
        public uVector4 grgg => new uVector4(y, x, y, y);
        
        /// <summary>
        /// Returns uVector3.yxyz swizzling.
        /// </summary>
        public uVector4 yxyz => new uVector4(y, x, y, z);
        
        /// <summary>
        /// Returns uVector3.grgb swizzling (equivalent to uVector3.yxyz).
        /// </summary>
        public uVector4 grgb => new uVector4(y, x, y, z);
        
        /// <summary>
        /// Returns uVector3.yxz swizzling.
        /// </summary>
        public uVector3 yxz => new uVector3(y, x, z);
        
        /// <summary>
        /// Returns uVector3.grb swizzling (equivalent to uVector3.yxz).
        /// </summary>
        public uVector3 grb => new uVector3(y, x, z);
        
        /// <summary>
        /// Returns uVector3.yxzx swizzling.
        /// </summary>
        public uVector4 yxzx => new uVector4(y, x, z, x);
        
        /// <summary>
        /// Returns uVector3.grbr swizzling (equivalent to uVector3.yxzx).
        /// </summary>
        public uVector4 grbr => new uVector4(y, x, z, x);
        
        /// <summary>
        /// Returns uVector3.yxzy swizzling.
        /// </summary>
        public uVector4 yxzy => new uVector4(y, x, z, y);
        
        /// <summary>
        /// Returns uVector3.grbg swizzling (equivalent to uVector3.yxzy).
        /// </summary>
        public uVector4 grbg => new uVector4(y, x, z, y);
        
        /// <summary>
        /// Returns uVector3.yxzz swizzling.
        /// </summary>
        public uVector4 yxzz => new uVector4(y, x, z, z);
        
        /// <summary>
        /// Returns uVector3.grbb swizzling (equivalent to uVector3.yxzz).
        /// </summary>
        public uVector4 grbb => new uVector4(y, x, z, z);
        
        /// <summary>
        /// Returns uVector3.yy swizzling.
        /// </summary>
        public uVector2 yy => new uVector2(y, y);
        
        /// <summary>
        /// Returns uVector3.gg swizzling (equivalent to uVector3.yy).
        /// </summary>
        public uVector2 gg => new uVector2(y, y);
        
        /// <summary>
        /// Returns uVector3.yyx swizzling.
        /// </summary>
        public uVector3 yyx => new uVector3(y, y, x);
        
        /// <summary>
        /// Returns uVector3.ggr swizzling (equivalent to uVector3.yyx).
        /// </summary>
        public uVector3 ggr => new uVector3(y, y, x);
        
        /// <summary>
        /// Returns uVector3.yyxx swizzling.
        /// </summary>
        public uVector4 yyxx => new uVector4(y, y, x, x);
        
        /// <summary>
        /// Returns uVector3.ggrr swizzling (equivalent to uVector3.yyxx).
        /// </summary>
        public uVector4 ggrr => new uVector4(y, y, x, x);
        
        /// <summary>
        /// Returns uVector3.yyxy swizzling.
        /// </summary>
        public uVector4 yyxy => new uVector4(y, y, x, y);
        
        /// <summary>
        /// Returns uVector3.ggrg swizzling (equivalent to uVector3.yyxy).
        /// </summary>
        public uVector4 ggrg => new uVector4(y, y, x, y);
        
        /// <summary>
        /// Returns uVector3.yyxz swizzling.
        /// </summary>
        public uVector4 yyxz => new uVector4(y, y, x, z);
        
        /// <summary>
        /// Returns uVector3.ggrb swizzling (equivalent to uVector3.yyxz).
        /// </summary>
        public uVector4 ggrb => new uVector4(y, y, x, z);
        
        /// <summary>
        /// Returns uVector3.yyy swizzling.
        /// </summary>
        public uVector3 yyy => new uVector3(y, y, y);
        
        /// <summary>
        /// Returns uVector3.ggg swizzling (equivalent to uVector3.yyy).
        /// </summary>
        public uVector3 ggg => new uVector3(y, y, y);
        
        /// <summary>
        /// Returns uVector3.yyyx swizzling.
        /// </summary>
        public uVector4 yyyx => new uVector4(y, y, y, x);
        
        /// <summary>
        /// Returns uVector3.gggr swizzling (equivalent to uVector3.yyyx).
        /// </summary>
        public uVector4 gggr => new uVector4(y, y, y, x);
        
        /// <summary>
        /// Returns uVector3.yyyy swizzling.
        /// </summary>
        public uVector4 yyyy => new uVector4(y, y, y, y);
        
        /// <summary>
        /// Returns uVector3.gggg swizzling (equivalent to uVector3.yyyy).
        /// </summary>
        public uVector4 gggg => new uVector4(y, y, y, y);
        
        /// <summary>
        /// Returns uVector3.yyyz swizzling.
        /// </summary>
        public uVector4 yyyz => new uVector4(y, y, y, z);
        
        /// <summary>
        /// Returns uVector3.gggb swizzling (equivalent to uVector3.yyyz).
        /// </summary>
        public uVector4 gggb => new uVector4(y, y, y, z);
        
        /// <summary>
        /// Returns uVector3.yyz swizzling.
        /// </summary>
        public uVector3 yyz => new uVector3(y, y, z);
        
        /// <summary>
        /// Returns uVector3.ggb swizzling (equivalent to uVector3.yyz).
        /// </summary>
        public uVector3 ggb => new uVector3(y, y, z);
        
        /// <summary>
        /// Returns uVector3.yyzx swizzling.
        /// </summary>
        public uVector4 yyzx => new uVector4(y, y, z, x);
        
        /// <summary>
        /// Returns uVector3.ggbr swizzling (equivalent to uVector3.yyzx).
        /// </summary>
        public uVector4 ggbr => new uVector4(y, y, z, x);
        
        /// <summary>
        /// Returns uVector3.yyzy swizzling.
        /// </summary>
        public uVector4 yyzy => new uVector4(y, y, z, y);
        
        /// <summary>
        /// Returns uVector3.ggbg swizzling (equivalent to uVector3.yyzy).
        /// </summary>
        public uVector4 ggbg => new uVector4(y, y, z, y);
        
        /// <summary>
        /// Returns uVector3.yyzz swizzling.
        /// </summary>
        public uVector4 yyzz => new uVector4(y, y, z, z);
        
        /// <summary>
        /// Returns uVector3.ggbb swizzling (equivalent to uVector3.yyzz).
        /// </summary>
        public uVector4 ggbb => new uVector4(y, y, z, z);
        
        /// <summary>
        /// Returns uVector3.yz swizzling.
        /// </summary>
        public uVector2 yz => new uVector2(y, z);
        
        /// <summary>
        /// Returns uVector3.gb swizzling (equivalent to uVector3.yz).
        /// </summary>
        public uVector2 gb => new uVector2(y, z);
        
        /// <summary>
        /// Returns uVector3.yzx swizzling.
        /// </summary>
        public uVector3 yzx => new uVector3(y, z, x);
        
        /// <summary>
        /// Returns uVector3.gbr swizzling (equivalent to uVector3.yzx).
        /// </summary>
        public uVector3 gbr => new uVector3(y, z, x);
        
        /// <summary>
        /// Returns uVector3.yzxx swizzling.
        /// </summary>
        public uVector4 yzxx => new uVector4(y, z, x, x);
        
        /// <summary>
        /// Returns uVector3.gbrr swizzling (equivalent to uVector3.yzxx).
        /// </summary>
        public uVector4 gbrr => new uVector4(y, z, x, x);
        
        /// <summary>
        /// Returns uVector3.yzxy swizzling.
        /// </summary>
        public uVector4 yzxy => new uVector4(y, z, x, y);
        
        /// <summary>
        /// Returns uVector3.gbrg swizzling (equivalent to uVector3.yzxy).
        /// </summary>
        public uVector4 gbrg => new uVector4(y, z, x, y);
        
        /// <summary>
        /// Returns uVector3.yzxz swizzling.
        /// </summary>
        public uVector4 yzxz => new uVector4(y, z, x, z);
        
        /// <summary>
        /// Returns uVector3.gbrb swizzling (equivalent to uVector3.yzxz).
        /// </summary>
        public uVector4 gbrb => new uVector4(y, z, x, z);
        
        /// <summary>
        /// Returns uVector3.yzy swizzling.
        /// </summary>
        public uVector3 yzy => new uVector3(y, z, y);
        
        /// <summary>
        /// Returns uVector3.gbg swizzling (equivalent to uVector3.yzy).
        /// </summary>
        public uVector3 gbg => new uVector3(y, z, y);
        
        /// <summary>
        /// Returns uVector3.yzyx swizzling.
        /// </summary>
        public uVector4 yzyx => new uVector4(y, z, y, x);
        
        /// <summary>
        /// Returns uVector3.gbgr swizzling (equivalent to uVector3.yzyx).
        /// </summary>
        public uVector4 gbgr => new uVector4(y, z, y, x);
        
        /// <summary>
        /// Returns uVector3.yzyy swizzling.
        /// </summary>
        public uVector4 yzyy => new uVector4(y, z, y, y);
        
        /// <summary>
        /// Returns uVector3.gbgg swizzling (equivalent to uVector3.yzyy).
        /// </summary>
        public uVector4 gbgg => new uVector4(y, z, y, y);
        
        /// <summary>
        /// Returns uVector3.yzyz swizzling.
        /// </summary>
        public uVector4 yzyz => new uVector4(y, z, y, z);
        
        /// <summary>
        /// Returns uVector3.gbgb swizzling (equivalent to uVector3.yzyz).
        /// </summary>
        public uVector4 gbgb => new uVector4(y, z, y, z);
        
        /// <summary>
        /// Returns uVector3.yzz swizzling.
        /// </summary>
        public uVector3 yzz => new uVector3(y, z, z);
        
        /// <summary>
        /// Returns uVector3.gbb swizzling (equivalent to uVector3.yzz).
        /// </summary>
        public uVector3 gbb => new uVector3(y, z, z);
        
        /// <summary>
        /// Returns uVector3.yzzx swizzling.
        /// </summary>
        public uVector4 yzzx => new uVector4(y, z, z, x);
        
        /// <summary>
        /// Returns uVector3.gbbr swizzling (equivalent to uVector3.yzzx).
        /// </summary>
        public uVector4 gbbr => new uVector4(y, z, z, x);
        
        /// <summary>
        /// Returns uVector3.yzzy swizzling.
        /// </summary>
        public uVector4 yzzy => new uVector4(y, z, z, y);
        
        /// <summary>
        /// Returns uVector3.gbbg swizzling (equivalent to uVector3.yzzy).
        /// </summary>
        public uVector4 gbbg => new uVector4(y, z, z, y);
        
        /// <summary>
        /// Returns uVector3.yzzz swizzling.
        /// </summary>
        public uVector4 yzzz => new uVector4(y, z, z, z);
        
        /// <summary>
        /// Returns uVector3.gbbb swizzling (equivalent to uVector3.yzzz).
        /// </summary>
        public uVector4 gbbb => new uVector4(y, z, z, z);
        
        /// <summary>
        /// Returns uVector3.zx swizzling.
        /// </summary>
        public uVector2 zx => new uVector2(z, x);
        
        /// <summary>
        /// Returns uVector3.br swizzling (equivalent to uVector3.zx).
        /// </summary>
        public uVector2 br => new uVector2(z, x);
        
        /// <summary>
        /// Returns uVector3.zxx swizzling.
        /// </summary>
        public uVector3 zxx => new uVector3(z, x, x);
        
        /// <summary>
        /// Returns uVector3.brr swizzling (equivalent to uVector3.zxx).
        /// </summary>
        public uVector3 brr => new uVector3(z, x, x);
        
        /// <summary>
        /// Returns uVector3.zxxx swizzling.
        /// </summary>
        public uVector4 zxxx => new uVector4(z, x, x, x);
        
        /// <summary>
        /// Returns uVector3.brrr swizzling (equivalent to uVector3.zxxx).
        /// </summary>
        public uVector4 brrr => new uVector4(z, x, x, x);
        
        /// <summary>
        /// Returns uVector3.zxxy swizzling.
        /// </summary>
        public uVector4 zxxy => new uVector4(z, x, x, y);
        
        /// <summary>
        /// Returns uVector3.brrg swizzling (equivalent to uVector3.zxxy).
        /// </summary>
        public uVector4 brrg => new uVector4(z, x, x, y);
        
        /// <summary>
        /// Returns uVector3.zxxz swizzling.
        /// </summary>
        public uVector4 zxxz => new uVector4(z, x, x, z);
        
        /// <summary>
        /// Returns uVector3.brrb swizzling (equivalent to uVector3.zxxz).
        /// </summary>
        public uVector4 brrb => new uVector4(z, x, x, z);
        
        /// <summary>
        /// Returns uVector3.zxy swizzling.
        /// </summary>
        public uVector3 zxy => new uVector3(z, x, y);
        
        /// <summary>
        /// Returns uVector3.brg swizzling (equivalent to uVector3.zxy).
        /// </summary>
        public uVector3 brg => new uVector3(z, x, y);
        
        /// <summary>
        /// Returns uVector3.zxyx swizzling.
        /// </summary>
        public uVector4 zxyx => new uVector4(z, x, y, x);
        
        /// <summary>
        /// Returns uVector3.brgr swizzling (equivalent to uVector3.zxyx).
        /// </summary>
        public uVector4 brgr => new uVector4(z, x, y, x);
        
        /// <summary>
        /// Returns uVector3.zxyy swizzling.
        /// </summary>
        public uVector4 zxyy => new uVector4(z, x, y, y);
        
        /// <summary>
        /// Returns uVector3.brgg swizzling (equivalent to uVector3.zxyy).
        /// </summary>
        public uVector4 brgg => new uVector4(z, x, y, y);
        
        /// <summary>
        /// Returns uVector3.zxyz swizzling.
        /// </summary>
        public uVector4 zxyz => new uVector4(z, x, y, z);
        
        /// <summary>
        /// Returns uVector3.brgb swizzling (equivalent to uVector3.zxyz).
        /// </summary>
        public uVector4 brgb => new uVector4(z, x, y, z);
        
        /// <summary>
        /// Returns uVector3.zxz swizzling.
        /// </summary>
        public uVector3 zxz => new uVector3(z, x, z);
        
        /// <summary>
        /// Returns uVector3.brb swizzling (equivalent to uVector3.zxz).
        /// </summary>
        public uVector3 brb => new uVector3(z, x, z);
        
        /// <summary>
        /// Returns uVector3.zxzx swizzling.
        /// </summary>
        public uVector4 zxzx => new uVector4(z, x, z, x);
        
        /// <summary>
        /// Returns uVector3.brbr swizzling (equivalent to uVector3.zxzx).
        /// </summary>
        public uVector4 brbr => new uVector4(z, x, z, x);
        
        /// <summary>
        /// Returns uVector3.zxzy swizzling.
        /// </summary>
        public uVector4 zxzy => new uVector4(z, x, z, y);
        
        /// <summary>
        /// Returns uVector3.brbg swizzling (equivalent to uVector3.zxzy).
        /// </summary>
        public uVector4 brbg => new uVector4(z, x, z, y);
        
        /// <summary>
        /// Returns uVector3.zxzz swizzling.
        /// </summary>
        public uVector4 zxzz => new uVector4(z, x, z, z);
        
        /// <summary>
        /// Returns uVector3.brbb swizzling (equivalent to uVector3.zxzz).
        /// </summary>
        public uVector4 brbb => new uVector4(z, x, z, z);
        
        /// <summary>
        /// Returns uVector3.zy swizzling.
        /// </summary>
        public uVector2 zy => new uVector2(z, y);
        
        /// <summary>
        /// Returns uVector3.bg swizzling (equivalent to uVector3.zy).
        /// </summary>
        public uVector2 bg => new uVector2(z, y);
        
        /// <summary>
        /// Returns uVector3.zyx swizzling.
        /// </summary>
        public uVector3 zyx => new uVector3(z, y, x);
        
        /// <summary>
        /// Returns uVector3.bgr swizzling (equivalent to uVector3.zyx).
        /// </summary>
        public uVector3 bgr => new uVector3(z, y, x);
        
        /// <summary>
        /// Returns uVector3.zyxx swizzling.
        /// </summary>
        public uVector4 zyxx => new uVector4(z, y, x, x);
        
        /// <summary>
        /// Returns uVector3.bgrr swizzling (equivalent to uVector3.zyxx).
        /// </summary>
        public uVector4 bgrr => new uVector4(z, y, x, x);
        
        /// <summary>
        /// Returns uVector3.zyxy swizzling.
        /// </summary>
        public uVector4 zyxy => new uVector4(z, y, x, y);
        
        /// <summary>
        /// Returns uVector3.bgrg swizzling (equivalent to uVector3.zyxy).
        /// </summary>
        public uVector4 bgrg => new uVector4(z, y, x, y);
        
        /// <summary>
        /// Returns uVector3.zyxz swizzling.
        /// </summary>
        public uVector4 zyxz => new uVector4(z, y, x, z);
        
        /// <summary>
        /// Returns uVector3.bgrb swizzling (equivalent to uVector3.zyxz).
        /// </summary>
        public uVector4 bgrb => new uVector4(z, y, x, z);
        
        /// <summary>
        /// Returns uVector3.zyy swizzling.
        /// </summary>
        public uVector3 zyy => new uVector3(z, y, y);
        
        /// <summary>
        /// Returns uVector3.bgg swizzling (equivalent to uVector3.zyy).
        /// </summary>
        public uVector3 bgg => new uVector3(z, y, y);
        
        /// <summary>
        /// Returns uVector3.zyyx swizzling.
        /// </summary>
        public uVector4 zyyx => new uVector4(z, y, y, x);
        
        /// <summary>
        /// Returns uVector3.bggr swizzling (equivalent to uVector3.zyyx).
        /// </summary>
        public uVector4 bggr => new uVector4(z, y, y, x);
        
        /// <summary>
        /// Returns uVector3.zyyy swizzling.
        /// </summary>
        public uVector4 zyyy => new uVector4(z, y, y, y);
        
        /// <summary>
        /// Returns uVector3.bggg swizzling (equivalent to uVector3.zyyy).
        /// </summary>
        public uVector4 bggg => new uVector4(z, y, y, y);
        
        /// <summary>
        /// Returns uVector3.zyyz swizzling.
        /// </summary>
        public uVector4 zyyz => new uVector4(z, y, y, z);
        
        /// <summary>
        /// Returns uVector3.bggb swizzling (equivalent to uVector3.zyyz).
        /// </summary>
        public uVector4 bggb => new uVector4(z, y, y, z);
        
        /// <summary>
        /// Returns uVector3.zyz swizzling.
        /// </summary>
        public uVector3 zyz => new uVector3(z, y, z);
        
        /// <summary>
        /// Returns uVector3.bgb swizzling (equivalent to uVector3.zyz).
        /// </summary>
        public uVector3 bgb => new uVector3(z, y, z);
        
        /// <summary>
        /// Returns uVector3.zyzx swizzling.
        /// </summary>
        public uVector4 zyzx => new uVector4(z, y, z, x);
        
        /// <summary>
        /// Returns uVector3.bgbr swizzling (equivalent to uVector3.zyzx).
        /// </summary>
        public uVector4 bgbr => new uVector4(z, y, z, x);
        
        /// <summary>
        /// Returns uVector3.zyzy swizzling.
        /// </summary>
        public uVector4 zyzy => new uVector4(z, y, z, y);
        
        /// <summary>
        /// Returns uVector3.bgbg swizzling (equivalent to uVector3.zyzy).
        /// </summary>
        public uVector4 bgbg => new uVector4(z, y, z, y);
        
        /// <summary>
        /// Returns uVector3.zyzz swizzling.
        /// </summary>
        public uVector4 zyzz => new uVector4(z, y, z, z);
        
        /// <summary>
        /// Returns uVector3.bgbb swizzling (equivalent to uVector3.zyzz).
        /// </summary>
        public uVector4 bgbb => new uVector4(z, y, z, z);
        
        /// <summary>
        /// Returns uVector3.zz swizzling.
        /// </summary>
        public uVector2 zz => new uVector2(z, z);
        
        /// <summary>
        /// Returns uVector3.bb swizzling (equivalent to uVector3.zz).
        /// </summary>
        public uVector2 bb => new uVector2(z, z);
        
        /// <summary>
        /// Returns uVector3.zzx swizzling.
        /// </summary>
        public uVector3 zzx => new uVector3(z, z, x);
        
        /// <summary>
        /// Returns uVector3.bbr swizzling (equivalent to uVector3.zzx).
        /// </summary>
        public uVector3 bbr => new uVector3(z, z, x);
        
        /// <summary>
        /// Returns uVector3.zzxx swizzling.
        /// </summary>
        public uVector4 zzxx => new uVector4(z, z, x, x);
        
        /// <summary>
        /// Returns uVector3.bbrr swizzling (equivalent to uVector3.zzxx).
        /// </summary>
        public uVector4 bbrr => new uVector4(z, z, x, x);
        
        /// <summary>
        /// Returns uVector3.zzxy swizzling.
        /// </summary>
        public uVector4 zzxy => new uVector4(z, z, x, y);
        
        /// <summary>
        /// Returns uVector3.bbrg swizzling (equivalent to uVector3.zzxy).
        /// </summary>
        public uVector4 bbrg => new uVector4(z, z, x, y);
        
        /// <summary>
        /// Returns uVector3.zzxz swizzling.
        /// </summary>
        public uVector4 zzxz => new uVector4(z, z, x, z);
        
        /// <summary>
        /// Returns uVector3.bbrb swizzling (equivalent to uVector3.zzxz).
        /// </summary>
        public uVector4 bbrb => new uVector4(z, z, x, z);
        
        /// <summary>
        /// Returns uVector3.zzy swizzling.
        /// </summary>
        public uVector3 zzy => new uVector3(z, z, y);
        
        /// <summary>
        /// Returns uVector3.bbg swizzling (equivalent to uVector3.zzy).
        /// </summary>
        public uVector3 bbg => new uVector3(z, z, y);
        
        /// <summary>
        /// Returns uVector3.zzyx swizzling.
        /// </summary>
        public uVector4 zzyx => new uVector4(z, z, y, x);
        
        /// <summary>
        /// Returns uVector3.bbgr swizzling (equivalent to uVector3.zzyx).
        /// </summary>
        public uVector4 bbgr => new uVector4(z, z, y, x);
        
        /// <summary>
        /// Returns uVector3.zzyy swizzling.
        /// </summary>
        public uVector4 zzyy => new uVector4(z, z, y, y);
        
        /// <summary>
        /// Returns uVector3.bbgg swizzling (equivalent to uVector3.zzyy).
        /// </summary>
        public uVector4 bbgg => new uVector4(z, z, y, y);
        
        /// <summary>
        /// Returns uVector3.zzyz swizzling.
        /// </summary>
        public uVector4 zzyz => new uVector4(z, z, y, z);
        
        /// <summary>
        /// Returns uVector3.bbgb swizzling (equivalent to uVector3.zzyz).
        /// </summary>
        public uVector4 bbgb => new uVector4(z, z, y, z);
        
        /// <summary>
        /// Returns uVector3.zzz swizzling.
        /// </summary>
        public uVector3 zzz => new uVector3(z, z, z);
        
        /// <summary>
        /// Returns uVector3.bbb swizzling (equivalent to uVector3.zzz).
        /// </summary>
        public uVector3 bbb => new uVector3(z, z, z);
        
        /// <summary>
        /// Returns uVector3.zzzx swizzling.
        /// </summary>
        public uVector4 zzzx => new uVector4(z, z, z, x);
        
        /// <summary>
        /// Returns uVector3.bbbr swizzling (equivalent to uVector3.zzzx).
        /// </summary>
        public uVector4 bbbr => new uVector4(z, z, z, x);
        
        /// <summary>
        /// Returns uVector3.zzzy swizzling.
        /// </summary>
        public uVector4 zzzy => new uVector4(z, z, z, y);
        
        /// <summary>
        /// Returns uVector3.bbbg swizzling (equivalent to uVector3.zzzy).
        /// </summary>
        public uVector4 bbbg => new uVector4(z, z, z, y);
        
        /// <summary>
        /// Returns uVector3.zzzz swizzling.
        /// </summary>
        public uVector4 zzzz => new uVector4(z, z, z, z);
        
        /// <summary>
        /// Returns uVector3.bbbb swizzling (equivalent to uVector3.zzzz).
        /// </summary>
        public uVector4 bbbb => new uVector4(z, z, z, z);

        #endregion

    }
}
