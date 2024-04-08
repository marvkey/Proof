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
    /// Temporary Vector of type Half with 3 components, used for implementing swizzling for hVector3.
    /// </summary>
    [RegisterCoreClassStruct]
   
    [StructLayout(LayoutKind.Sequential)]
    public struct swizzle_hVector3
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
        
        /// <summary>
        /// z-component
        /// </summary>
        
        internal readonly Half z;

        #endregion


        #region Constructors
        
        /// <summary>
        /// Constructor for swizzle_hVector3.
        /// </summary>
        internal swizzle_hVector3(Half x, Half y, Half z)
        {
            this.x = x;
            this.y = y;
            this.z = z;
        }

        #endregion


        #region Properties
        
        /// <summary>
        /// Returns hVector3.xx swizzling.
        /// </summary>
        public hVector2 xx => new hVector2(x, x);
        
        /// <summary>
        /// Returns hVector3.rr swizzling (equivalent to hVector3.xx).
        /// </summary>
        public hVector2 rr => new hVector2(x, x);
        
        /// <summary>
        /// Returns hVector3.xxx swizzling.
        /// </summary>
        public hVector3 xxx => new hVector3(x, x, x);
        
        /// <summary>
        /// Returns hVector3.rrr swizzling (equivalent to hVector3.xxx).
        /// </summary>
        public hVector3 rrr => new hVector3(x, x, x);
        
        /// <summary>
        /// Returns hVector3.xxxx swizzling.
        /// </summary>
        public hVector4 xxxx => new hVector4(x, x, x, x);
        
        /// <summary>
        /// Returns hVector3.rrrr swizzling (equivalent to hVector3.xxxx).
        /// </summary>
        public hVector4 rrrr => new hVector4(x, x, x, x);
        
        /// <summary>
        /// Returns hVector3.xxxy swizzling.
        /// </summary>
        public hVector4 xxxy => new hVector4(x, x, x, y);
        
        /// <summary>
        /// Returns hVector3.rrrg swizzling (equivalent to hVector3.xxxy).
        /// </summary>
        public hVector4 rrrg => new hVector4(x, x, x, y);
        
        /// <summary>
        /// Returns hVector3.xxxz swizzling.
        /// </summary>
        public hVector4 xxxz => new hVector4(x, x, x, z);
        
        /// <summary>
        /// Returns hVector3.rrrb swizzling (equivalent to hVector3.xxxz).
        /// </summary>
        public hVector4 rrrb => new hVector4(x, x, x, z);
        
        /// <summary>
        /// Returns hVector3.xxy swizzling.
        /// </summary>
        public hVector3 xxy => new hVector3(x, x, y);
        
        /// <summary>
        /// Returns hVector3.rrg swizzling (equivalent to hVector3.xxy).
        /// </summary>
        public hVector3 rrg => new hVector3(x, x, y);
        
        /// <summary>
        /// Returns hVector3.xxyx swizzling.
        /// </summary>
        public hVector4 xxyx => new hVector4(x, x, y, x);
        
        /// <summary>
        /// Returns hVector3.rrgr swizzling (equivalent to hVector3.xxyx).
        /// </summary>
        public hVector4 rrgr => new hVector4(x, x, y, x);
        
        /// <summary>
        /// Returns hVector3.xxyy swizzling.
        /// </summary>
        public hVector4 xxyy => new hVector4(x, x, y, y);
        
        /// <summary>
        /// Returns hVector3.rrgg swizzling (equivalent to hVector3.xxyy).
        /// </summary>
        public hVector4 rrgg => new hVector4(x, x, y, y);
        
        /// <summary>
        /// Returns hVector3.xxyz swizzling.
        /// </summary>
        public hVector4 xxyz => new hVector4(x, x, y, z);
        
        /// <summary>
        /// Returns hVector3.rrgb swizzling (equivalent to hVector3.xxyz).
        /// </summary>
        public hVector4 rrgb => new hVector4(x, x, y, z);
        
        /// <summary>
        /// Returns hVector3.xxz swizzling.
        /// </summary>
        public hVector3 xxz => new hVector3(x, x, z);
        
        /// <summary>
        /// Returns hVector3.rrb swizzling (equivalent to hVector3.xxz).
        /// </summary>
        public hVector3 rrb => new hVector3(x, x, z);
        
        /// <summary>
        /// Returns hVector3.xxzx swizzling.
        /// </summary>
        public hVector4 xxzx => new hVector4(x, x, z, x);
        
        /// <summary>
        /// Returns hVector3.rrbr swizzling (equivalent to hVector3.xxzx).
        /// </summary>
        public hVector4 rrbr => new hVector4(x, x, z, x);
        
        /// <summary>
        /// Returns hVector3.xxzy swizzling.
        /// </summary>
        public hVector4 xxzy => new hVector4(x, x, z, y);
        
        /// <summary>
        /// Returns hVector3.rrbg swizzling (equivalent to hVector3.xxzy).
        /// </summary>
        public hVector4 rrbg => new hVector4(x, x, z, y);
        
        /// <summary>
        /// Returns hVector3.xxzz swizzling.
        /// </summary>
        public hVector4 xxzz => new hVector4(x, x, z, z);
        
        /// <summary>
        /// Returns hVector3.rrbb swizzling (equivalent to hVector3.xxzz).
        /// </summary>
        public hVector4 rrbb => new hVector4(x, x, z, z);
        
        /// <summary>
        /// Returns hVector3.xy swizzling.
        /// </summary>
        public hVector2 xy => new hVector2(x, y);
        
        /// <summary>
        /// Returns hVector3.rg swizzling (equivalent to hVector3.xy).
        /// </summary>
        public hVector2 rg => new hVector2(x, y);
        
        /// <summary>
        /// Returns hVector3.xyx swizzling.
        /// </summary>
        public hVector3 xyx => new hVector3(x, y, x);
        
        /// <summary>
        /// Returns hVector3.rgr swizzling (equivalent to hVector3.xyx).
        /// </summary>
        public hVector3 rgr => new hVector3(x, y, x);
        
        /// <summary>
        /// Returns hVector3.xyxx swizzling.
        /// </summary>
        public hVector4 xyxx => new hVector4(x, y, x, x);
        
        /// <summary>
        /// Returns hVector3.rgrr swizzling (equivalent to hVector3.xyxx).
        /// </summary>
        public hVector4 rgrr => new hVector4(x, y, x, x);
        
        /// <summary>
        /// Returns hVector3.xyxy swizzling.
        /// </summary>
        public hVector4 xyxy => new hVector4(x, y, x, y);
        
        /// <summary>
        /// Returns hVector3.rgrg swizzling (equivalent to hVector3.xyxy).
        /// </summary>
        public hVector4 rgrg => new hVector4(x, y, x, y);
        
        /// <summary>
        /// Returns hVector3.xyxz swizzling.
        /// </summary>
        public hVector4 xyxz => new hVector4(x, y, x, z);
        
        /// <summary>
        /// Returns hVector3.rgrb swizzling (equivalent to hVector3.xyxz).
        /// </summary>
        public hVector4 rgrb => new hVector4(x, y, x, z);
        
        /// <summary>
        /// Returns hVector3.xyy swizzling.
        /// </summary>
        public hVector3 xyy => new hVector3(x, y, y);
        
        /// <summary>
        /// Returns hVector3.rgg swizzling (equivalent to hVector3.xyy).
        /// </summary>
        public hVector3 rgg => new hVector3(x, y, y);
        
        /// <summary>
        /// Returns hVector3.xyyx swizzling.
        /// </summary>
        public hVector4 xyyx => new hVector4(x, y, y, x);
        
        /// <summary>
        /// Returns hVector3.rggr swizzling (equivalent to hVector3.xyyx).
        /// </summary>
        public hVector4 rggr => new hVector4(x, y, y, x);
        
        /// <summary>
        /// Returns hVector3.xyyy swizzling.
        /// </summary>
        public hVector4 xyyy => new hVector4(x, y, y, y);
        
        /// <summary>
        /// Returns hVector3.rggg swizzling (equivalent to hVector3.xyyy).
        /// </summary>
        public hVector4 rggg => new hVector4(x, y, y, y);
        
        /// <summary>
        /// Returns hVector3.xyyz swizzling.
        /// </summary>
        public hVector4 xyyz => new hVector4(x, y, y, z);
        
        /// <summary>
        /// Returns hVector3.rggb swizzling (equivalent to hVector3.xyyz).
        /// </summary>
        public hVector4 rggb => new hVector4(x, y, y, z);
        
        /// <summary>
        /// Returns hVector3.xyz swizzling.
        /// </summary>
        public hVector3 xyz => new hVector3(x, y, z);
        
        /// <summary>
        /// Returns hVector3.rgb swizzling (equivalent to hVector3.xyz).
        /// </summary>
        public hVector3 rgb => new hVector3(x, y, z);
        
        /// <summary>
        /// Returns hVector3.xyzx swizzling.
        /// </summary>
        public hVector4 xyzx => new hVector4(x, y, z, x);
        
        /// <summary>
        /// Returns hVector3.rgbr swizzling (equivalent to hVector3.xyzx).
        /// </summary>
        public hVector4 rgbr => new hVector4(x, y, z, x);
        
        /// <summary>
        /// Returns hVector3.xyzy swizzling.
        /// </summary>
        public hVector4 xyzy => new hVector4(x, y, z, y);
        
        /// <summary>
        /// Returns hVector3.rgbg swizzling (equivalent to hVector3.xyzy).
        /// </summary>
        public hVector4 rgbg => new hVector4(x, y, z, y);
        
        /// <summary>
        /// Returns hVector3.xyzz swizzling.
        /// </summary>
        public hVector4 xyzz => new hVector4(x, y, z, z);
        
        /// <summary>
        /// Returns hVector3.rgbb swizzling (equivalent to hVector3.xyzz).
        /// </summary>
        public hVector4 rgbb => new hVector4(x, y, z, z);
        
        /// <summary>
        /// Returns hVector3.xz swizzling.
        /// </summary>
        public hVector2 xz => new hVector2(x, z);
        
        /// <summary>
        /// Returns hVector3.rb swizzling (equivalent to hVector3.xz).
        /// </summary>
        public hVector2 rb => new hVector2(x, z);
        
        /// <summary>
        /// Returns hVector3.xzx swizzling.
        /// </summary>
        public hVector3 xzx => new hVector3(x, z, x);
        
        /// <summary>
        /// Returns hVector3.rbr swizzling (equivalent to hVector3.xzx).
        /// </summary>
        public hVector3 rbr => new hVector3(x, z, x);
        
        /// <summary>
        /// Returns hVector3.xzxx swizzling.
        /// </summary>
        public hVector4 xzxx => new hVector4(x, z, x, x);
        
        /// <summary>
        /// Returns hVector3.rbrr swizzling (equivalent to hVector3.xzxx).
        /// </summary>
        public hVector4 rbrr => new hVector4(x, z, x, x);
        
        /// <summary>
        /// Returns hVector3.xzxy swizzling.
        /// </summary>
        public hVector4 xzxy => new hVector4(x, z, x, y);
        
        /// <summary>
        /// Returns hVector3.rbrg swizzling (equivalent to hVector3.xzxy).
        /// </summary>
        public hVector4 rbrg => new hVector4(x, z, x, y);
        
        /// <summary>
        /// Returns hVector3.xzxz swizzling.
        /// </summary>
        public hVector4 xzxz => new hVector4(x, z, x, z);
        
        /// <summary>
        /// Returns hVector3.rbrb swizzling (equivalent to hVector3.xzxz).
        /// </summary>
        public hVector4 rbrb => new hVector4(x, z, x, z);
        
        /// <summary>
        /// Returns hVector3.xzy swizzling.
        /// </summary>
        public hVector3 xzy => new hVector3(x, z, y);
        
        /// <summary>
        /// Returns hVector3.rbg swizzling (equivalent to hVector3.xzy).
        /// </summary>
        public hVector3 rbg => new hVector3(x, z, y);
        
        /// <summary>
        /// Returns hVector3.xzyx swizzling.
        /// </summary>
        public hVector4 xzyx => new hVector4(x, z, y, x);
        
        /// <summary>
        /// Returns hVector3.rbgr swizzling (equivalent to hVector3.xzyx).
        /// </summary>
        public hVector4 rbgr => new hVector4(x, z, y, x);
        
        /// <summary>
        /// Returns hVector3.xzyy swizzling.
        /// </summary>
        public hVector4 xzyy => new hVector4(x, z, y, y);
        
        /// <summary>
        /// Returns hVector3.rbgg swizzling (equivalent to hVector3.xzyy).
        /// </summary>
        public hVector4 rbgg => new hVector4(x, z, y, y);
        
        /// <summary>
        /// Returns hVector3.xzyz swizzling.
        /// </summary>
        public hVector4 xzyz => new hVector4(x, z, y, z);
        
        /// <summary>
        /// Returns hVector3.rbgb swizzling (equivalent to hVector3.xzyz).
        /// </summary>
        public hVector4 rbgb => new hVector4(x, z, y, z);
        
        /// <summary>
        /// Returns hVector3.xzz swizzling.
        /// </summary>
        public hVector3 xzz => new hVector3(x, z, z);
        
        /// <summary>
        /// Returns hVector3.rbb swizzling (equivalent to hVector3.xzz).
        /// </summary>
        public hVector3 rbb => new hVector3(x, z, z);
        
        /// <summary>
        /// Returns hVector3.xzzx swizzling.
        /// </summary>
        public hVector4 xzzx => new hVector4(x, z, z, x);
        
        /// <summary>
        /// Returns hVector3.rbbr swizzling (equivalent to hVector3.xzzx).
        /// </summary>
        public hVector4 rbbr => new hVector4(x, z, z, x);
        
        /// <summary>
        /// Returns hVector3.xzzy swizzling.
        /// </summary>
        public hVector4 xzzy => new hVector4(x, z, z, y);
        
        /// <summary>
        /// Returns hVector3.rbbg swizzling (equivalent to hVector3.xzzy).
        /// </summary>
        public hVector4 rbbg => new hVector4(x, z, z, y);
        
        /// <summary>
        /// Returns hVector3.xzzz swizzling.
        /// </summary>
        public hVector4 xzzz => new hVector4(x, z, z, z);
        
        /// <summary>
        /// Returns hVector3.rbbb swizzling (equivalent to hVector3.xzzz).
        /// </summary>
        public hVector4 rbbb => new hVector4(x, z, z, z);
        
        /// <summary>
        /// Returns hVector3.yx swizzling.
        /// </summary>
        public hVector2 yx => new hVector2(y, x);
        
        /// <summary>
        /// Returns hVector3.gr swizzling (equivalent to hVector3.yx).
        /// </summary>
        public hVector2 gr => new hVector2(y, x);
        
        /// <summary>
        /// Returns hVector3.yxx swizzling.
        /// </summary>
        public hVector3 yxx => new hVector3(y, x, x);
        
        /// <summary>
        /// Returns hVector3.grr swizzling (equivalent to hVector3.yxx).
        /// </summary>
        public hVector3 grr => new hVector3(y, x, x);
        
        /// <summary>
        /// Returns hVector3.yxxx swizzling.
        /// </summary>
        public hVector4 yxxx => new hVector4(y, x, x, x);
        
        /// <summary>
        /// Returns hVector3.grrr swizzling (equivalent to hVector3.yxxx).
        /// </summary>
        public hVector4 grrr => new hVector4(y, x, x, x);
        
        /// <summary>
        /// Returns hVector3.yxxy swizzling.
        /// </summary>
        public hVector4 yxxy => new hVector4(y, x, x, y);
        
        /// <summary>
        /// Returns hVector3.grrg swizzling (equivalent to hVector3.yxxy).
        /// </summary>
        public hVector4 grrg => new hVector4(y, x, x, y);
        
        /// <summary>
        /// Returns hVector3.yxxz swizzling.
        /// </summary>
        public hVector4 yxxz => new hVector4(y, x, x, z);
        
        /// <summary>
        /// Returns hVector3.grrb swizzling (equivalent to hVector3.yxxz).
        /// </summary>
        public hVector4 grrb => new hVector4(y, x, x, z);
        
        /// <summary>
        /// Returns hVector3.yxy swizzling.
        /// </summary>
        public hVector3 yxy => new hVector3(y, x, y);
        
        /// <summary>
        /// Returns hVector3.grg swizzling (equivalent to hVector3.yxy).
        /// </summary>
        public hVector3 grg => new hVector3(y, x, y);
        
        /// <summary>
        /// Returns hVector3.yxyx swizzling.
        /// </summary>
        public hVector4 yxyx => new hVector4(y, x, y, x);
        
        /// <summary>
        /// Returns hVector3.grgr swizzling (equivalent to hVector3.yxyx).
        /// </summary>
        public hVector4 grgr => new hVector4(y, x, y, x);
        
        /// <summary>
        /// Returns hVector3.yxyy swizzling.
        /// </summary>
        public hVector4 yxyy => new hVector4(y, x, y, y);
        
        /// <summary>
        /// Returns hVector3.grgg swizzling (equivalent to hVector3.yxyy).
        /// </summary>
        public hVector4 grgg => new hVector4(y, x, y, y);
        
        /// <summary>
        /// Returns hVector3.yxyz swizzling.
        /// </summary>
        public hVector4 yxyz => new hVector4(y, x, y, z);
        
        /// <summary>
        /// Returns hVector3.grgb swizzling (equivalent to hVector3.yxyz).
        /// </summary>
        public hVector4 grgb => new hVector4(y, x, y, z);
        
        /// <summary>
        /// Returns hVector3.yxz swizzling.
        /// </summary>
        public hVector3 yxz => new hVector3(y, x, z);
        
        /// <summary>
        /// Returns hVector3.grb swizzling (equivalent to hVector3.yxz).
        /// </summary>
        public hVector3 grb => new hVector3(y, x, z);
        
        /// <summary>
        /// Returns hVector3.yxzx swizzling.
        /// </summary>
        public hVector4 yxzx => new hVector4(y, x, z, x);
        
        /// <summary>
        /// Returns hVector3.grbr swizzling (equivalent to hVector3.yxzx).
        /// </summary>
        public hVector4 grbr => new hVector4(y, x, z, x);
        
        /// <summary>
        /// Returns hVector3.yxzy swizzling.
        /// </summary>
        public hVector4 yxzy => new hVector4(y, x, z, y);
        
        /// <summary>
        /// Returns hVector3.grbg swizzling (equivalent to hVector3.yxzy).
        /// </summary>
        public hVector4 grbg => new hVector4(y, x, z, y);
        
        /// <summary>
        /// Returns hVector3.yxzz swizzling.
        /// </summary>
        public hVector4 yxzz => new hVector4(y, x, z, z);
        
        /// <summary>
        /// Returns hVector3.grbb swizzling (equivalent to hVector3.yxzz).
        /// </summary>
        public hVector4 grbb => new hVector4(y, x, z, z);
        
        /// <summary>
        /// Returns hVector3.yy swizzling.
        /// </summary>
        public hVector2 yy => new hVector2(y, y);
        
        /// <summary>
        /// Returns hVector3.gg swizzling (equivalent to hVector3.yy).
        /// </summary>
        public hVector2 gg => new hVector2(y, y);
        
        /// <summary>
        /// Returns hVector3.yyx swizzling.
        /// </summary>
        public hVector3 yyx => new hVector3(y, y, x);
        
        /// <summary>
        /// Returns hVector3.ggr swizzling (equivalent to hVector3.yyx).
        /// </summary>
        public hVector3 ggr => new hVector3(y, y, x);
        
        /// <summary>
        /// Returns hVector3.yyxx swizzling.
        /// </summary>
        public hVector4 yyxx => new hVector4(y, y, x, x);
        
        /// <summary>
        /// Returns hVector3.ggrr swizzling (equivalent to hVector3.yyxx).
        /// </summary>
        public hVector4 ggrr => new hVector4(y, y, x, x);
        
        /// <summary>
        /// Returns hVector3.yyxy swizzling.
        /// </summary>
        public hVector4 yyxy => new hVector4(y, y, x, y);
        
        /// <summary>
        /// Returns hVector3.ggrg swizzling (equivalent to hVector3.yyxy).
        /// </summary>
        public hVector4 ggrg => new hVector4(y, y, x, y);
        
        /// <summary>
        /// Returns hVector3.yyxz swizzling.
        /// </summary>
        public hVector4 yyxz => new hVector4(y, y, x, z);
        
        /// <summary>
        /// Returns hVector3.ggrb swizzling (equivalent to hVector3.yyxz).
        /// </summary>
        public hVector4 ggrb => new hVector4(y, y, x, z);
        
        /// <summary>
        /// Returns hVector3.yyy swizzling.
        /// </summary>
        public hVector3 yyy => new hVector3(y, y, y);
        
        /// <summary>
        /// Returns hVector3.ggg swizzling (equivalent to hVector3.yyy).
        /// </summary>
        public hVector3 ggg => new hVector3(y, y, y);
        
        /// <summary>
        /// Returns hVector3.yyyx swizzling.
        /// </summary>
        public hVector4 yyyx => new hVector4(y, y, y, x);
        
        /// <summary>
        /// Returns hVector3.gggr swizzling (equivalent to hVector3.yyyx).
        /// </summary>
        public hVector4 gggr => new hVector4(y, y, y, x);
        
        /// <summary>
        /// Returns hVector3.yyyy swizzling.
        /// </summary>
        public hVector4 yyyy => new hVector4(y, y, y, y);
        
        /// <summary>
        /// Returns hVector3.gggg swizzling (equivalent to hVector3.yyyy).
        /// </summary>
        public hVector4 gggg => new hVector4(y, y, y, y);
        
        /// <summary>
        /// Returns hVector3.yyyz swizzling.
        /// </summary>
        public hVector4 yyyz => new hVector4(y, y, y, z);
        
        /// <summary>
        /// Returns hVector3.gggb swizzling (equivalent to hVector3.yyyz).
        /// </summary>
        public hVector4 gggb => new hVector4(y, y, y, z);
        
        /// <summary>
        /// Returns hVector3.yyz swizzling.
        /// </summary>
        public hVector3 yyz => new hVector3(y, y, z);
        
        /// <summary>
        /// Returns hVector3.ggb swizzling (equivalent to hVector3.yyz).
        /// </summary>
        public hVector3 ggb => new hVector3(y, y, z);
        
        /// <summary>
        /// Returns hVector3.yyzx swizzling.
        /// </summary>
        public hVector4 yyzx => new hVector4(y, y, z, x);
        
        /// <summary>
        /// Returns hVector3.ggbr swizzling (equivalent to hVector3.yyzx).
        /// </summary>
        public hVector4 ggbr => new hVector4(y, y, z, x);
        
        /// <summary>
        /// Returns hVector3.yyzy swizzling.
        /// </summary>
        public hVector4 yyzy => new hVector4(y, y, z, y);
        
        /// <summary>
        /// Returns hVector3.ggbg swizzling (equivalent to hVector3.yyzy).
        /// </summary>
        public hVector4 ggbg => new hVector4(y, y, z, y);
        
        /// <summary>
        /// Returns hVector3.yyzz swizzling.
        /// </summary>
        public hVector4 yyzz => new hVector4(y, y, z, z);
        
        /// <summary>
        /// Returns hVector3.ggbb swizzling (equivalent to hVector3.yyzz).
        /// </summary>
        public hVector4 ggbb => new hVector4(y, y, z, z);
        
        /// <summary>
        /// Returns hVector3.yz swizzling.
        /// </summary>
        public hVector2 yz => new hVector2(y, z);
        
        /// <summary>
        /// Returns hVector3.gb swizzling (equivalent to hVector3.yz).
        /// </summary>
        public hVector2 gb => new hVector2(y, z);
        
        /// <summary>
        /// Returns hVector3.yzx swizzling.
        /// </summary>
        public hVector3 yzx => new hVector3(y, z, x);
        
        /// <summary>
        /// Returns hVector3.gbr swizzling (equivalent to hVector3.yzx).
        /// </summary>
        public hVector3 gbr => new hVector3(y, z, x);
        
        /// <summary>
        /// Returns hVector3.yzxx swizzling.
        /// </summary>
        public hVector4 yzxx => new hVector4(y, z, x, x);
        
        /// <summary>
        /// Returns hVector3.gbrr swizzling (equivalent to hVector3.yzxx).
        /// </summary>
        public hVector4 gbrr => new hVector4(y, z, x, x);
        
        /// <summary>
        /// Returns hVector3.yzxy swizzling.
        /// </summary>
        public hVector4 yzxy => new hVector4(y, z, x, y);
        
        /// <summary>
        /// Returns hVector3.gbrg swizzling (equivalent to hVector3.yzxy).
        /// </summary>
        public hVector4 gbrg => new hVector4(y, z, x, y);
        
        /// <summary>
        /// Returns hVector3.yzxz swizzling.
        /// </summary>
        public hVector4 yzxz => new hVector4(y, z, x, z);
        
        /// <summary>
        /// Returns hVector3.gbrb swizzling (equivalent to hVector3.yzxz).
        /// </summary>
        public hVector4 gbrb => new hVector4(y, z, x, z);
        
        /// <summary>
        /// Returns hVector3.yzy swizzling.
        /// </summary>
        public hVector3 yzy => new hVector3(y, z, y);
        
        /// <summary>
        /// Returns hVector3.gbg swizzling (equivalent to hVector3.yzy).
        /// </summary>
        public hVector3 gbg => new hVector3(y, z, y);
        
        /// <summary>
        /// Returns hVector3.yzyx swizzling.
        /// </summary>
        public hVector4 yzyx => new hVector4(y, z, y, x);
        
        /// <summary>
        /// Returns hVector3.gbgr swizzling (equivalent to hVector3.yzyx).
        /// </summary>
        public hVector4 gbgr => new hVector4(y, z, y, x);
        
        /// <summary>
        /// Returns hVector3.yzyy swizzling.
        /// </summary>
        public hVector4 yzyy => new hVector4(y, z, y, y);
        
        /// <summary>
        /// Returns hVector3.gbgg swizzling (equivalent to hVector3.yzyy).
        /// </summary>
        public hVector4 gbgg => new hVector4(y, z, y, y);
        
        /// <summary>
        /// Returns hVector3.yzyz swizzling.
        /// </summary>
        public hVector4 yzyz => new hVector4(y, z, y, z);
        
        /// <summary>
        /// Returns hVector3.gbgb swizzling (equivalent to hVector3.yzyz).
        /// </summary>
        public hVector4 gbgb => new hVector4(y, z, y, z);
        
        /// <summary>
        /// Returns hVector3.yzz swizzling.
        /// </summary>
        public hVector3 yzz => new hVector3(y, z, z);
        
        /// <summary>
        /// Returns hVector3.gbb swizzling (equivalent to hVector3.yzz).
        /// </summary>
        public hVector3 gbb => new hVector3(y, z, z);
        
        /// <summary>
        /// Returns hVector3.yzzx swizzling.
        /// </summary>
        public hVector4 yzzx => new hVector4(y, z, z, x);
        
        /// <summary>
        /// Returns hVector3.gbbr swizzling (equivalent to hVector3.yzzx).
        /// </summary>
        public hVector4 gbbr => new hVector4(y, z, z, x);
        
        /// <summary>
        /// Returns hVector3.yzzy swizzling.
        /// </summary>
        public hVector4 yzzy => new hVector4(y, z, z, y);
        
        /// <summary>
        /// Returns hVector3.gbbg swizzling (equivalent to hVector3.yzzy).
        /// </summary>
        public hVector4 gbbg => new hVector4(y, z, z, y);
        
        /// <summary>
        /// Returns hVector3.yzzz swizzling.
        /// </summary>
        public hVector4 yzzz => new hVector4(y, z, z, z);
        
        /// <summary>
        /// Returns hVector3.gbbb swizzling (equivalent to hVector3.yzzz).
        /// </summary>
        public hVector4 gbbb => new hVector4(y, z, z, z);
        
        /// <summary>
        /// Returns hVector3.zx swizzling.
        /// </summary>
        public hVector2 zx => new hVector2(z, x);
        
        /// <summary>
        /// Returns hVector3.br swizzling (equivalent to hVector3.zx).
        /// </summary>
        public hVector2 br => new hVector2(z, x);
        
        /// <summary>
        /// Returns hVector3.zxx swizzling.
        /// </summary>
        public hVector3 zxx => new hVector3(z, x, x);
        
        /// <summary>
        /// Returns hVector3.brr swizzling (equivalent to hVector3.zxx).
        /// </summary>
        public hVector3 brr => new hVector3(z, x, x);
        
        /// <summary>
        /// Returns hVector3.zxxx swizzling.
        /// </summary>
        public hVector4 zxxx => new hVector4(z, x, x, x);
        
        /// <summary>
        /// Returns hVector3.brrr swizzling (equivalent to hVector3.zxxx).
        /// </summary>
        public hVector4 brrr => new hVector4(z, x, x, x);
        
        /// <summary>
        /// Returns hVector3.zxxy swizzling.
        /// </summary>
        public hVector4 zxxy => new hVector4(z, x, x, y);
        
        /// <summary>
        /// Returns hVector3.brrg swizzling (equivalent to hVector3.zxxy).
        /// </summary>
        public hVector4 brrg => new hVector4(z, x, x, y);
        
        /// <summary>
        /// Returns hVector3.zxxz swizzling.
        /// </summary>
        public hVector4 zxxz => new hVector4(z, x, x, z);
        
        /// <summary>
        /// Returns hVector3.brrb swizzling (equivalent to hVector3.zxxz).
        /// </summary>
        public hVector4 brrb => new hVector4(z, x, x, z);
        
        /// <summary>
        /// Returns hVector3.zxy swizzling.
        /// </summary>
        public hVector3 zxy => new hVector3(z, x, y);
        
        /// <summary>
        /// Returns hVector3.brg swizzling (equivalent to hVector3.zxy).
        /// </summary>
        public hVector3 brg => new hVector3(z, x, y);
        
        /// <summary>
        /// Returns hVector3.zxyx swizzling.
        /// </summary>
        public hVector4 zxyx => new hVector4(z, x, y, x);
        
        /// <summary>
        /// Returns hVector3.brgr swizzling (equivalent to hVector3.zxyx).
        /// </summary>
        public hVector4 brgr => new hVector4(z, x, y, x);
        
        /// <summary>
        /// Returns hVector3.zxyy swizzling.
        /// </summary>
        public hVector4 zxyy => new hVector4(z, x, y, y);
        
        /// <summary>
        /// Returns hVector3.brgg swizzling (equivalent to hVector3.zxyy).
        /// </summary>
        public hVector4 brgg => new hVector4(z, x, y, y);
        
        /// <summary>
        /// Returns hVector3.zxyz swizzling.
        /// </summary>
        public hVector4 zxyz => new hVector4(z, x, y, z);
        
        /// <summary>
        /// Returns hVector3.brgb swizzling (equivalent to hVector3.zxyz).
        /// </summary>
        public hVector4 brgb => new hVector4(z, x, y, z);
        
        /// <summary>
        /// Returns hVector3.zxz swizzling.
        /// </summary>
        public hVector3 zxz => new hVector3(z, x, z);
        
        /// <summary>
        /// Returns hVector3.brb swizzling (equivalent to hVector3.zxz).
        /// </summary>
        public hVector3 brb => new hVector3(z, x, z);
        
        /// <summary>
        /// Returns hVector3.zxzx swizzling.
        /// </summary>
        public hVector4 zxzx => new hVector4(z, x, z, x);
        
        /// <summary>
        /// Returns hVector3.brbr swizzling (equivalent to hVector3.zxzx).
        /// </summary>
        public hVector4 brbr => new hVector4(z, x, z, x);
        
        /// <summary>
        /// Returns hVector3.zxzy swizzling.
        /// </summary>
        public hVector4 zxzy => new hVector4(z, x, z, y);
        
        /// <summary>
        /// Returns hVector3.brbg swizzling (equivalent to hVector3.zxzy).
        /// </summary>
        public hVector4 brbg => new hVector4(z, x, z, y);
        
        /// <summary>
        /// Returns hVector3.zxzz swizzling.
        /// </summary>
        public hVector4 zxzz => new hVector4(z, x, z, z);
        
        /// <summary>
        /// Returns hVector3.brbb swizzling (equivalent to hVector3.zxzz).
        /// </summary>
        public hVector4 brbb => new hVector4(z, x, z, z);
        
        /// <summary>
        /// Returns hVector3.zy swizzling.
        /// </summary>
        public hVector2 zy => new hVector2(z, y);
        
        /// <summary>
        /// Returns hVector3.bg swizzling (equivalent to hVector3.zy).
        /// </summary>
        public hVector2 bg => new hVector2(z, y);
        
        /// <summary>
        /// Returns hVector3.zyx swizzling.
        /// </summary>
        public hVector3 zyx => new hVector3(z, y, x);
        
        /// <summary>
        /// Returns hVector3.bgr swizzling (equivalent to hVector3.zyx).
        /// </summary>
        public hVector3 bgr => new hVector3(z, y, x);
        
        /// <summary>
        /// Returns hVector3.zyxx swizzling.
        /// </summary>
        public hVector4 zyxx => new hVector4(z, y, x, x);
        
        /// <summary>
        /// Returns hVector3.bgrr swizzling (equivalent to hVector3.zyxx).
        /// </summary>
        public hVector4 bgrr => new hVector4(z, y, x, x);
        
        /// <summary>
        /// Returns hVector3.zyxy swizzling.
        /// </summary>
        public hVector4 zyxy => new hVector4(z, y, x, y);
        
        /// <summary>
        /// Returns hVector3.bgrg swizzling (equivalent to hVector3.zyxy).
        /// </summary>
        public hVector4 bgrg => new hVector4(z, y, x, y);
        
        /// <summary>
        /// Returns hVector3.zyxz swizzling.
        /// </summary>
        public hVector4 zyxz => new hVector4(z, y, x, z);
        
        /// <summary>
        /// Returns hVector3.bgrb swizzling (equivalent to hVector3.zyxz).
        /// </summary>
        public hVector4 bgrb => new hVector4(z, y, x, z);
        
        /// <summary>
        /// Returns hVector3.zyy swizzling.
        /// </summary>
        public hVector3 zyy => new hVector3(z, y, y);
        
        /// <summary>
        /// Returns hVector3.bgg swizzling (equivalent to hVector3.zyy).
        /// </summary>
        public hVector3 bgg => new hVector3(z, y, y);
        
        /// <summary>
        /// Returns hVector3.zyyx swizzling.
        /// </summary>
        public hVector4 zyyx => new hVector4(z, y, y, x);
        
        /// <summary>
        /// Returns hVector3.bggr swizzling (equivalent to hVector3.zyyx).
        /// </summary>
        public hVector4 bggr => new hVector4(z, y, y, x);
        
        /// <summary>
        /// Returns hVector3.zyyy swizzling.
        /// </summary>
        public hVector4 zyyy => new hVector4(z, y, y, y);
        
        /// <summary>
        /// Returns hVector3.bggg swizzling (equivalent to hVector3.zyyy).
        /// </summary>
        public hVector4 bggg => new hVector4(z, y, y, y);
        
        /// <summary>
        /// Returns hVector3.zyyz swizzling.
        /// </summary>
        public hVector4 zyyz => new hVector4(z, y, y, z);
        
        /// <summary>
        /// Returns hVector3.bggb swizzling (equivalent to hVector3.zyyz).
        /// </summary>
        public hVector4 bggb => new hVector4(z, y, y, z);
        
        /// <summary>
        /// Returns hVector3.zyz swizzling.
        /// </summary>
        public hVector3 zyz => new hVector3(z, y, z);
        
        /// <summary>
        /// Returns hVector3.bgb swizzling (equivalent to hVector3.zyz).
        /// </summary>
        public hVector3 bgb => new hVector3(z, y, z);
        
        /// <summary>
        /// Returns hVector3.zyzx swizzling.
        /// </summary>
        public hVector4 zyzx => new hVector4(z, y, z, x);
        
        /// <summary>
        /// Returns hVector3.bgbr swizzling (equivalent to hVector3.zyzx).
        /// </summary>
        public hVector4 bgbr => new hVector4(z, y, z, x);
        
        /// <summary>
        /// Returns hVector3.zyzy swizzling.
        /// </summary>
        public hVector4 zyzy => new hVector4(z, y, z, y);
        
        /// <summary>
        /// Returns hVector3.bgbg swizzling (equivalent to hVector3.zyzy).
        /// </summary>
        public hVector4 bgbg => new hVector4(z, y, z, y);
        
        /// <summary>
        /// Returns hVector3.zyzz swizzling.
        /// </summary>
        public hVector4 zyzz => new hVector4(z, y, z, z);
        
        /// <summary>
        /// Returns hVector3.bgbb swizzling (equivalent to hVector3.zyzz).
        /// </summary>
        public hVector4 bgbb => new hVector4(z, y, z, z);
        
        /// <summary>
        /// Returns hVector3.zz swizzling.
        /// </summary>
        public hVector2 zz => new hVector2(z, z);
        
        /// <summary>
        /// Returns hVector3.bb swizzling (equivalent to hVector3.zz).
        /// </summary>
        public hVector2 bb => new hVector2(z, z);
        
        /// <summary>
        /// Returns hVector3.zzx swizzling.
        /// </summary>
        public hVector3 zzx => new hVector3(z, z, x);
        
        /// <summary>
        /// Returns hVector3.bbr swizzling (equivalent to hVector3.zzx).
        /// </summary>
        public hVector3 bbr => new hVector3(z, z, x);
        
        /// <summary>
        /// Returns hVector3.zzxx swizzling.
        /// </summary>
        public hVector4 zzxx => new hVector4(z, z, x, x);
        
        /// <summary>
        /// Returns hVector3.bbrr swizzling (equivalent to hVector3.zzxx).
        /// </summary>
        public hVector4 bbrr => new hVector4(z, z, x, x);
        
        /// <summary>
        /// Returns hVector3.zzxy swizzling.
        /// </summary>
        public hVector4 zzxy => new hVector4(z, z, x, y);
        
        /// <summary>
        /// Returns hVector3.bbrg swizzling (equivalent to hVector3.zzxy).
        /// </summary>
        public hVector4 bbrg => new hVector4(z, z, x, y);
        
        /// <summary>
        /// Returns hVector3.zzxz swizzling.
        /// </summary>
        public hVector4 zzxz => new hVector4(z, z, x, z);
        
        /// <summary>
        /// Returns hVector3.bbrb swizzling (equivalent to hVector3.zzxz).
        /// </summary>
        public hVector4 bbrb => new hVector4(z, z, x, z);
        
        /// <summary>
        /// Returns hVector3.zzy swizzling.
        /// </summary>
        public hVector3 zzy => new hVector3(z, z, y);
        
        /// <summary>
        /// Returns hVector3.bbg swizzling (equivalent to hVector3.zzy).
        /// </summary>
        public hVector3 bbg => new hVector3(z, z, y);
        
        /// <summary>
        /// Returns hVector3.zzyx swizzling.
        /// </summary>
        public hVector4 zzyx => new hVector4(z, z, y, x);
        
        /// <summary>
        /// Returns hVector3.bbgr swizzling (equivalent to hVector3.zzyx).
        /// </summary>
        public hVector4 bbgr => new hVector4(z, z, y, x);
        
        /// <summary>
        /// Returns hVector3.zzyy swizzling.
        /// </summary>
        public hVector4 zzyy => new hVector4(z, z, y, y);
        
        /// <summary>
        /// Returns hVector3.bbgg swizzling (equivalent to hVector3.zzyy).
        /// </summary>
        public hVector4 bbgg => new hVector4(z, z, y, y);
        
        /// <summary>
        /// Returns hVector3.zzyz swizzling.
        /// </summary>
        public hVector4 zzyz => new hVector4(z, z, y, z);
        
        /// <summary>
        /// Returns hVector3.bbgb swizzling (equivalent to hVector3.zzyz).
        /// </summary>
        public hVector4 bbgb => new hVector4(z, z, y, z);
        
        /// <summary>
        /// Returns hVector3.zzz swizzling.
        /// </summary>
        public hVector3 zzz => new hVector3(z, z, z);
        
        /// <summary>
        /// Returns hVector3.bbb swizzling (equivalent to hVector3.zzz).
        /// </summary>
        public hVector3 bbb => new hVector3(z, z, z);
        
        /// <summary>
        /// Returns hVector3.zzzx swizzling.
        /// </summary>
        public hVector4 zzzx => new hVector4(z, z, z, x);
        
        /// <summary>
        /// Returns hVector3.bbbr swizzling (equivalent to hVector3.zzzx).
        /// </summary>
        public hVector4 bbbr => new hVector4(z, z, z, x);
        
        /// <summary>
        /// Returns hVector3.zzzy swizzling.
        /// </summary>
        public hVector4 zzzy => new hVector4(z, z, z, y);
        
        /// <summary>
        /// Returns hVector3.bbbg swizzling (equivalent to hVector3.zzzy).
        /// </summary>
        public hVector4 bbbg => new hVector4(z, z, z, y);
        
        /// <summary>
        /// Returns hVector3.zzzz swizzling.
        /// </summary>
        public hVector4 zzzz => new hVector4(z, z, z, z);
        
        /// <summary>
        /// Returns hVector3.bbbb swizzling (equivalent to hVector3.zzzz).
        /// </summary>
        public hVector4 bbbb => new hVector4(z, z, z, z);

        #endregion

    }
}
