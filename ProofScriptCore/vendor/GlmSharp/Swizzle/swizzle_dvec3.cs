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
    /// Temporary Vector of type double with 3 components, used for implementing swizzling for dVector3.
    /// </summary>
    [RegisterCoreClassStruct]
   
    [StructLayout(LayoutKind.Sequential)]
    public struct swizzle_dVector3
    {

        #region Fields
        
        /// <summary>
        /// x-component
        /// </summary>
        
        internal readonly double x;
        
        /// <summary>
        /// y-component
        /// </summary>
        
        internal readonly double y;
        
        /// <summary>
        /// z-component
        /// </summary>
        
        internal readonly double z;

        #endregion


        #region Constructors
        
        /// <summary>
        /// Constructor for swizzle_dVector3.
        /// </summary>
        internal swizzle_dVector3(double x, double y, double z)
        {
            this.x = x;
            this.y = y;
            this.z = z;
        }

        #endregion


        #region Properties
        
        /// <summary>
        /// Returns dVector3.xx swizzling.
        /// </summary>
        public dVector2 xx => new dVector2(x, x);
        
        /// <summary>
        /// Returns dVector3.rr swizzling (equivalent to dVector3.xx).
        /// </summary>
        public dVector2 rr => new dVector2(x, x);
        
        /// <summary>
        /// Returns dVector3.xxx swizzling.
        /// </summary>
        public dVector3 xxx => new dVector3(x, x, x);
        
        /// <summary>
        /// Returns dVector3.rrr swizzling (equivalent to dVector3.xxx).
        /// </summary>
        public dVector3 rrr => new dVector3(x, x, x);
        
        /// <summary>
        /// Returns dVector3.xxxx swizzling.
        /// </summary>
        public dVector4 xxxx => new dVector4(x, x, x, x);
        
        /// <summary>
        /// Returns dVector3.rrrr swizzling (equivalent to dVector3.xxxx).
        /// </summary>
        public dVector4 rrrr => new dVector4(x, x, x, x);
        
        /// <summary>
        /// Returns dVector3.xxxy swizzling.
        /// </summary>
        public dVector4 xxxy => new dVector4(x, x, x, y);
        
        /// <summary>
        /// Returns dVector3.rrrg swizzling (equivalent to dVector3.xxxy).
        /// </summary>
        public dVector4 rrrg => new dVector4(x, x, x, y);
        
        /// <summary>
        /// Returns dVector3.xxxz swizzling.
        /// </summary>
        public dVector4 xxxz => new dVector4(x, x, x, z);
        
        /// <summary>
        /// Returns dVector3.rrrb swizzling (equivalent to dVector3.xxxz).
        /// </summary>
        public dVector4 rrrb => new dVector4(x, x, x, z);
        
        /// <summary>
        /// Returns dVector3.xxy swizzling.
        /// </summary>
        public dVector3 xxy => new dVector3(x, x, y);
        
        /// <summary>
        /// Returns dVector3.rrg swizzling (equivalent to dVector3.xxy).
        /// </summary>
        public dVector3 rrg => new dVector3(x, x, y);
        
        /// <summary>
        /// Returns dVector3.xxyx swizzling.
        /// </summary>
        public dVector4 xxyx => new dVector4(x, x, y, x);
        
        /// <summary>
        /// Returns dVector3.rrgr swizzling (equivalent to dVector3.xxyx).
        /// </summary>
        public dVector4 rrgr => new dVector4(x, x, y, x);
        
        /// <summary>
        /// Returns dVector3.xxyy swizzling.
        /// </summary>
        public dVector4 xxyy => new dVector4(x, x, y, y);
        
        /// <summary>
        /// Returns dVector3.rrgg swizzling (equivalent to dVector3.xxyy).
        /// </summary>
        public dVector4 rrgg => new dVector4(x, x, y, y);
        
        /// <summary>
        /// Returns dVector3.xxyz swizzling.
        /// </summary>
        public dVector4 xxyz => new dVector4(x, x, y, z);
        
        /// <summary>
        /// Returns dVector3.rrgb swizzling (equivalent to dVector3.xxyz).
        /// </summary>
        public dVector4 rrgb => new dVector4(x, x, y, z);
        
        /// <summary>
        /// Returns dVector3.xxz swizzling.
        /// </summary>
        public dVector3 xxz => new dVector3(x, x, z);
        
        /// <summary>
        /// Returns dVector3.rrb swizzling (equivalent to dVector3.xxz).
        /// </summary>
        public dVector3 rrb => new dVector3(x, x, z);
        
        /// <summary>
        /// Returns dVector3.xxzx swizzling.
        /// </summary>
        public dVector4 xxzx => new dVector4(x, x, z, x);
        
        /// <summary>
        /// Returns dVector3.rrbr swizzling (equivalent to dVector3.xxzx).
        /// </summary>
        public dVector4 rrbr => new dVector4(x, x, z, x);
        
        /// <summary>
        /// Returns dVector3.xxzy swizzling.
        /// </summary>
        public dVector4 xxzy => new dVector4(x, x, z, y);
        
        /// <summary>
        /// Returns dVector3.rrbg swizzling (equivalent to dVector3.xxzy).
        /// </summary>
        public dVector4 rrbg => new dVector4(x, x, z, y);
        
        /// <summary>
        /// Returns dVector3.xxzz swizzling.
        /// </summary>
        public dVector4 xxzz => new dVector4(x, x, z, z);
        
        /// <summary>
        /// Returns dVector3.rrbb swizzling (equivalent to dVector3.xxzz).
        /// </summary>
        public dVector4 rrbb => new dVector4(x, x, z, z);
        
        /// <summary>
        /// Returns dVector3.xy swizzling.
        /// </summary>
        public dVector2 xy => new dVector2(x, y);
        
        /// <summary>
        /// Returns dVector3.rg swizzling (equivalent to dVector3.xy).
        /// </summary>
        public dVector2 rg => new dVector2(x, y);
        
        /// <summary>
        /// Returns dVector3.xyx swizzling.
        /// </summary>
        public dVector3 xyx => new dVector3(x, y, x);
        
        /// <summary>
        /// Returns dVector3.rgr swizzling (equivalent to dVector3.xyx).
        /// </summary>
        public dVector3 rgr => new dVector3(x, y, x);
        
        /// <summary>
        /// Returns dVector3.xyxx swizzling.
        /// </summary>
        public dVector4 xyxx => new dVector4(x, y, x, x);
        
        /// <summary>
        /// Returns dVector3.rgrr swizzling (equivalent to dVector3.xyxx).
        /// </summary>
        public dVector4 rgrr => new dVector4(x, y, x, x);
        
        /// <summary>
        /// Returns dVector3.xyxy swizzling.
        /// </summary>
        public dVector4 xyxy => new dVector4(x, y, x, y);
        
        /// <summary>
        /// Returns dVector3.rgrg swizzling (equivalent to dVector3.xyxy).
        /// </summary>
        public dVector4 rgrg => new dVector4(x, y, x, y);
        
        /// <summary>
        /// Returns dVector3.xyxz swizzling.
        /// </summary>
        public dVector4 xyxz => new dVector4(x, y, x, z);
        
        /// <summary>
        /// Returns dVector3.rgrb swizzling (equivalent to dVector3.xyxz).
        /// </summary>
        public dVector4 rgrb => new dVector4(x, y, x, z);
        
        /// <summary>
        /// Returns dVector3.xyy swizzling.
        /// </summary>
        public dVector3 xyy => new dVector3(x, y, y);
        
        /// <summary>
        /// Returns dVector3.rgg swizzling (equivalent to dVector3.xyy).
        /// </summary>
        public dVector3 rgg => new dVector3(x, y, y);
        
        /// <summary>
        /// Returns dVector3.xyyx swizzling.
        /// </summary>
        public dVector4 xyyx => new dVector4(x, y, y, x);
        
        /// <summary>
        /// Returns dVector3.rggr swizzling (equivalent to dVector3.xyyx).
        /// </summary>
        public dVector4 rggr => new dVector4(x, y, y, x);
        
        /// <summary>
        /// Returns dVector3.xyyy swizzling.
        /// </summary>
        public dVector4 xyyy => new dVector4(x, y, y, y);
        
        /// <summary>
        /// Returns dVector3.rggg swizzling (equivalent to dVector3.xyyy).
        /// </summary>
        public dVector4 rggg => new dVector4(x, y, y, y);
        
        /// <summary>
        /// Returns dVector3.xyyz swizzling.
        /// </summary>
        public dVector4 xyyz => new dVector4(x, y, y, z);
        
        /// <summary>
        /// Returns dVector3.rggb swizzling (equivalent to dVector3.xyyz).
        /// </summary>
        public dVector4 rggb => new dVector4(x, y, y, z);
        
        /// <summary>
        /// Returns dVector3.xyz swizzling.
        /// </summary>
        public dVector3 xyz => new dVector3(x, y, z);
        
        /// <summary>
        /// Returns dVector3.rgb swizzling (equivalent to dVector3.xyz).
        /// </summary>
        public dVector3 rgb => new dVector3(x, y, z);
        
        /// <summary>
        /// Returns dVector3.xyzx swizzling.
        /// </summary>
        public dVector4 xyzx => new dVector4(x, y, z, x);
        
        /// <summary>
        /// Returns dVector3.rgbr swizzling (equivalent to dVector3.xyzx).
        /// </summary>
        public dVector4 rgbr => new dVector4(x, y, z, x);
        
        /// <summary>
        /// Returns dVector3.xyzy swizzling.
        /// </summary>
        public dVector4 xyzy => new dVector4(x, y, z, y);
        
        /// <summary>
        /// Returns dVector3.rgbg swizzling (equivalent to dVector3.xyzy).
        /// </summary>
        public dVector4 rgbg => new dVector4(x, y, z, y);
        
        /// <summary>
        /// Returns dVector3.xyzz swizzling.
        /// </summary>
        public dVector4 xyzz => new dVector4(x, y, z, z);
        
        /// <summary>
        /// Returns dVector3.rgbb swizzling (equivalent to dVector3.xyzz).
        /// </summary>
        public dVector4 rgbb => new dVector4(x, y, z, z);
        
        /// <summary>
        /// Returns dVector3.xz swizzling.
        /// </summary>
        public dVector2 xz => new dVector2(x, z);
        
        /// <summary>
        /// Returns dVector3.rb swizzling (equivalent to dVector3.xz).
        /// </summary>
        public dVector2 rb => new dVector2(x, z);
        
        /// <summary>
        /// Returns dVector3.xzx swizzling.
        /// </summary>
        public dVector3 xzx => new dVector3(x, z, x);
        
        /// <summary>
        /// Returns dVector3.rbr swizzling (equivalent to dVector3.xzx).
        /// </summary>
        public dVector3 rbr => new dVector3(x, z, x);
        
        /// <summary>
        /// Returns dVector3.xzxx swizzling.
        /// </summary>
        public dVector4 xzxx => new dVector4(x, z, x, x);
        
        /// <summary>
        /// Returns dVector3.rbrr swizzling (equivalent to dVector3.xzxx).
        /// </summary>
        public dVector4 rbrr => new dVector4(x, z, x, x);
        
        /// <summary>
        /// Returns dVector3.xzxy swizzling.
        /// </summary>
        public dVector4 xzxy => new dVector4(x, z, x, y);
        
        /// <summary>
        /// Returns dVector3.rbrg swizzling (equivalent to dVector3.xzxy).
        /// </summary>
        public dVector4 rbrg => new dVector4(x, z, x, y);
        
        /// <summary>
        /// Returns dVector3.xzxz swizzling.
        /// </summary>
        public dVector4 xzxz => new dVector4(x, z, x, z);
        
        /// <summary>
        /// Returns dVector3.rbrb swizzling (equivalent to dVector3.xzxz).
        /// </summary>
        public dVector4 rbrb => new dVector4(x, z, x, z);
        
        /// <summary>
        /// Returns dVector3.xzy swizzling.
        /// </summary>
        public dVector3 xzy => new dVector3(x, z, y);
        
        /// <summary>
        /// Returns dVector3.rbg swizzling (equivalent to dVector3.xzy).
        /// </summary>
        public dVector3 rbg => new dVector3(x, z, y);
        
        /// <summary>
        /// Returns dVector3.xzyx swizzling.
        /// </summary>
        public dVector4 xzyx => new dVector4(x, z, y, x);
        
        /// <summary>
        /// Returns dVector3.rbgr swizzling (equivalent to dVector3.xzyx).
        /// </summary>
        public dVector4 rbgr => new dVector4(x, z, y, x);
        
        /// <summary>
        /// Returns dVector3.xzyy swizzling.
        /// </summary>
        public dVector4 xzyy => new dVector4(x, z, y, y);
        
        /// <summary>
        /// Returns dVector3.rbgg swizzling (equivalent to dVector3.xzyy).
        /// </summary>
        public dVector4 rbgg => new dVector4(x, z, y, y);
        
        /// <summary>
        /// Returns dVector3.xzyz swizzling.
        /// </summary>
        public dVector4 xzyz => new dVector4(x, z, y, z);
        
        /// <summary>
        /// Returns dVector3.rbgb swizzling (equivalent to dVector3.xzyz).
        /// </summary>
        public dVector4 rbgb => new dVector4(x, z, y, z);
        
        /// <summary>
        /// Returns dVector3.xzz swizzling.
        /// </summary>
        public dVector3 xzz => new dVector3(x, z, z);
        
        /// <summary>
        /// Returns dVector3.rbb swizzling (equivalent to dVector3.xzz).
        /// </summary>
        public dVector3 rbb => new dVector3(x, z, z);
        
        /// <summary>
        /// Returns dVector3.xzzx swizzling.
        /// </summary>
        public dVector4 xzzx => new dVector4(x, z, z, x);
        
        /// <summary>
        /// Returns dVector3.rbbr swizzling (equivalent to dVector3.xzzx).
        /// </summary>
        public dVector4 rbbr => new dVector4(x, z, z, x);
        
        /// <summary>
        /// Returns dVector3.xzzy swizzling.
        /// </summary>
        public dVector4 xzzy => new dVector4(x, z, z, y);
        
        /// <summary>
        /// Returns dVector3.rbbg swizzling (equivalent to dVector3.xzzy).
        /// </summary>
        public dVector4 rbbg => new dVector4(x, z, z, y);
        
        /// <summary>
        /// Returns dVector3.xzzz swizzling.
        /// </summary>
        public dVector4 xzzz => new dVector4(x, z, z, z);
        
        /// <summary>
        /// Returns dVector3.rbbb swizzling (equivalent to dVector3.xzzz).
        /// </summary>
        public dVector4 rbbb => new dVector4(x, z, z, z);
        
        /// <summary>
        /// Returns dVector3.yx swizzling.
        /// </summary>
        public dVector2 yx => new dVector2(y, x);
        
        /// <summary>
        /// Returns dVector3.gr swizzling (equivalent to dVector3.yx).
        /// </summary>
        public dVector2 gr => new dVector2(y, x);
        
        /// <summary>
        /// Returns dVector3.yxx swizzling.
        /// </summary>
        public dVector3 yxx => new dVector3(y, x, x);
        
        /// <summary>
        /// Returns dVector3.grr swizzling (equivalent to dVector3.yxx).
        /// </summary>
        public dVector3 grr => new dVector3(y, x, x);
        
        /// <summary>
        /// Returns dVector3.yxxx swizzling.
        /// </summary>
        public dVector4 yxxx => new dVector4(y, x, x, x);
        
        /// <summary>
        /// Returns dVector3.grrr swizzling (equivalent to dVector3.yxxx).
        /// </summary>
        public dVector4 grrr => new dVector4(y, x, x, x);
        
        /// <summary>
        /// Returns dVector3.yxxy swizzling.
        /// </summary>
        public dVector4 yxxy => new dVector4(y, x, x, y);
        
        /// <summary>
        /// Returns dVector3.grrg swizzling (equivalent to dVector3.yxxy).
        /// </summary>
        public dVector4 grrg => new dVector4(y, x, x, y);
        
        /// <summary>
        /// Returns dVector3.yxxz swizzling.
        /// </summary>
        public dVector4 yxxz => new dVector4(y, x, x, z);
        
        /// <summary>
        /// Returns dVector3.grrb swizzling (equivalent to dVector3.yxxz).
        /// </summary>
        public dVector4 grrb => new dVector4(y, x, x, z);
        
        /// <summary>
        /// Returns dVector3.yxy swizzling.
        /// </summary>
        public dVector3 yxy => new dVector3(y, x, y);
        
        /// <summary>
        /// Returns dVector3.grg swizzling (equivalent to dVector3.yxy).
        /// </summary>
        public dVector3 grg => new dVector3(y, x, y);
        
        /// <summary>
        /// Returns dVector3.yxyx swizzling.
        /// </summary>
        public dVector4 yxyx => new dVector4(y, x, y, x);
        
        /// <summary>
        /// Returns dVector3.grgr swizzling (equivalent to dVector3.yxyx).
        /// </summary>
        public dVector4 grgr => new dVector4(y, x, y, x);
        
        /// <summary>
        /// Returns dVector3.yxyy swizzling.
        /// </summary>
        public dVector4 yxyy => new dVector4(y, x, y, y);
        
        /// <summary>
        /// Returns dVector3.grgg swizzling (equivalent to dVector3.yxyy).
        /// </summary>
        public dVector4 grgg => new dVector4(y, x, y, y);
        
        /// <summary>
        /// Returns dVector3.yxyz swizzling.
        /// </summary>
        public dVector4 yxyz => new dVector4(y, x, y, z);
        
        /// <summary>
        /// Returns dVector3.grgb swizzling (equivalent to dVector3.yxyz).
        /// </summary>
        public dVector4 grgb => new dVector4(y, x, y, z);
        
        /// <summary>
        /// Returns dVector3.yxz swizzling.
        /// </summary>
        public dVector3 yxz => new dVector3(y, x, z);
        
        /// <summary>
        /// Returns dVector3.grb swizzling (equivalent to dVector3.yxz).
        /// </summary>
        public dVector3 grb => new dVector3(y, x, z);
        
        /// <summary>
        /// Returns dVector3.yxzx swizzling.
        /// </summary>
        public dVector4 yxzx => new dVector4(y, x, z, x);
        
        /// <summary>
        /// Returns dVector3.grbr swizzling (equivalent to dVector3.yxzx).
        /// </summary>
        public dVector4 grbr => new dVector4(y, x, z, x);
        
        /// <summary>
        /// Returns dVector3.yxzy swizzling.
        /// </summary>
        public dVector4 yxzy => new dVector4(y, x, z, y);
        
        /// <summary>
        /// Returns dVector3.grbg swizzling (equivalent to dVector3.yxzy).
        /// </summary>
        public dVector4 grbg => new dVector4(y, x, z, y);
        
        /// <summary>
        /// Returns dVector3.yxzz swizzling.
        /// </summary>
        public dVector4 yxzz => new dVector4(y, x, z, z);
        
        /// <summary>
        /// Returns dVector3.grbb swizzling (equivalent to dVector3.yxzz).
        /// </summary>
        public dVector4 grbb => new dVector4(y, x, z, z);
        
        /// <summary>
        /// Returns dVector3.yy swizzling.
        /// </summary>
        public dVector2 yy => new dVector2(y, y);
        
        /// <summary>
        /// Returns dVector3.gg swizzling (equivalent to dVector3.yy).
        /// </summary>
        public dVector2 gg => new dVector2(y, y);
        
        /// <summary>
        /// Returns dVector3.yyx swizzling.
        /// </summary>
        public dVector3 yyx => new dVector3(y, y, x);
        
        /// <summary>
        /// Returns dVector3.ggr swizzling (equivalent to dVector3.yyx).
        /// </summary>
        public dVector3 ggr => new dVector3(y, y, x);
        
        /// <summary>
        /// Returns dVector3.yyxx swizzling.
        /// </summary>
        public dVector4 yyxx => new dVector4(y, y, x, x);
        
        /// <summary>
        /// Returns dVector3.ggrr swizzling (equivalent to dVector3.yyxx).
        /// </summary>
        public dVector4 ggrr => new dVector4(y, y, x, x);
        
        /// <summary>
        /// Returns dVector3.yyxy swizzling.
        /// </summary>
        public dVector4 yyxy => new dVector4(y, y, x, y);
        
        /// <summary>
        /// Returns dVector3.ggrg swizzling (equivalent to dVector3.yyxy).
        /// </summary>
        public dVector4 ggrg => new dVector4(y, y, x, y);
        
        /// <summary>
        /// Returns dVector3.yyxz swizzling.
        /// </summary>
        public dVector4 yyxz => new dVector4(y, y, x, z);
        
        /// <summary>
        /// Returns dVector3.ggrb swizzling (equivalent to dVector3.yyxz).
        /// </summary>
        public dVector4 ggrb => new dVector4(y, y, x, z);
        
        /// <summary>
        /// Returns dVector3.yyy swizzling.
        /// </summary>
        public dVector3 yyy => new dVector3(y, y, y);
        
        /// <summary>
        /// Returns dVector3.ggg swizzling (equivalent to dVector3.yyy).
        /// </summary>
        public dVector3 ggg => new dVector3(y, y, y);
        
        /// <summary>
        /// Returns dVector3.yyyx swizzling.
        /// </summary>
        public dVector4 yyyx => new dVector4(y, y, y, x);
        
        /// <summary>
        /// Returns dVector3.gggr swizzling (equivalent to dVector3.yyyx).
        /// </summary>
        public dVector4 gggr => new dVector4(y, y, y, x);
        
        /// <summary>
        /// Returns dVector3.yyyy swizzling.
        /// </summary>
        public dVector4 yyyy => new dVector4(y, y, y, y);
        
        /// <summary>
        /// Returns dVector3.gggg swizzling (equivalent to dVector3.yyyy).
        /// </summary>
        public dVector4 gggg => new dVector4(y, y, y, y);
        
        /// <summary>
        /// Returns dVector3.yyyz swizzling.
        /// </summary>
        public dVector4 yyyz => new dVector4(y, y, y, z);
        
        /// <summary>
        /// Returns dVector3.gggb swizzling (equivalent to dVector3.yyyz).
        /// </summary>
        public dVector4 gggb => new dVector4(y, y, y, z);
        
        /// <summary>
        /// Returns dVector3.yyz swizzling.
        /// </summary>
        public dVector3 yyz => new dVector3(y, y, z);
        
        /// <summary>
        /// Returns dVector3.ggb swizzling (equivalent to dVector3.yyz).
        /// </summary>
        public dVector3 ggb => new dVector3(y, y, z);
        
        /// <summary>
        /// Returns dVector3.yyzx swizzling.
        /// </summary>
        public dVector4 yyzx => new dVector4(y, y, z, x);
        
        /// <summary>
        /// Returns dVector3.ggbr swizzling (equivalent to dVector3.yyzx).
        /// </summary>
        public dVector4 ggbr => new dVector4(y, y, z, x);
        
        /// <summary>
        /// Returns dVector3.yyzy swizzling.
        /// </summary>
        public dVector4 yyzy => new dVector4(y, y, z, y);
        
        /// <summary>
        /// Returns dVector3.ggbg swizzling (equivalent to dVector3.yyzy).
        /// </summary>
        public dVector4 ggbg => new dVector4(y, y, z, y);
        
        /// <summary>
        /// Returns dVector3.yyzz swizzling.
        /// </summary>
        public dVector4 yyzz => new dVector4(y, y, z, z);
        
        /// <summary>
        /// Returns dVector3.ggbb swizzling (equivalent to dVector3.yyzz).
        /// </summary>
        public dVector4 ggbb => new dVector4(y, y, z, z);
        
        /// <summary>
        /// Returns dVector3.yz swizzling.
        /// </summary>
        public dVector2 yz => new dVector2(y, z);
        
        /// <summary>
        /// Returns dVector3.gb swizzling (equivalent to dVector3.yz).
        /// </summary>
        public dVector2 gb => new dVector2(y, z);
        
        /// <summary>
        /// Returns dVector3.yzx swizzling.
        /// </summary>
        public dVector3 yzx => new dVector3(y, z, x);
        
        /// <summary>
        /// Returns dVector3.gbr swizzling (equivalent to dVector3.yzx).
        /// </summary>
        public dVector3 gbr => new dVector3(y, z, x);
        
        /// <summary>
        /// Returns dVector3.yzxx swizzling.
        /// </summary>
        public dVector4 yzxx => new dVector4(y, z, x, x);
        
        /// <summary>
        /// Returns dVector3.gbrr swizzling (equivalent to dVector3.yzxx).
        /// </summary>
        public dVector4 gbrr => new dVector4(y, z, x, x);
        
        /// <summary>
        /// Returns dVector3.yzxy swizzling.
        /// </summary>
        public dVector4 yzxy => new dVector4(y, z, x, y);
        
        /// <summary>
        /// Returns dVector3.gbrg swizzling (equivalent to dVector3.yzxy).
        /// </summary>
        public dVector4 gbrg => new dVector4(y, z, x, y);
        
        /// <summary>
        /// Returns dVector3.yzxz swizzling.
        /// </summary>
        public dVector4 yzxz => new dVector4(y, z, x, z);
        
        /// <summary>
        /// Returns dVector3.gbrb swizzling (equivalent to dVector3.yzxz).
        /// </summary>
        public dVector4 gbrb => new dVector4(y, z, x, z);
        
        /// <summary>
        /// Returns dVector3.yzy swizzling.
        /// </summary>
        public dVector3 yzy => new dVector3(y, z, y);
        
        /// <summary>
        /// Returns dVector3.gbg swizzling (equivalent to dVector3.yzy).
        /// </summary>
        public dVector3 gbg => new dVector3(y, z, y);
        
        /// <summary>
        /// Returns dVector3.yzyx swizzling.
        /// </summary>
        public dVector4 yzyx => new dVector4(y, z, y, x);
        
        /// <summary>
        /// Returns dVector3.gbgr swizzling (equivalent to dVector3.yzyx).
        /// </summary>
        public dVector4 gbgr => new dVector4(y, z, y, x);
        
        /// <summary>
        /// Returns dVector3.yzyy swizzling.
        /// </summary>
        public dVector4 yzyy => new dVector4(y, z, y, y);
        
        /// <summary>
        /// Returns dVector3.gbgg swizzling (equivalent to dVector3.yzyy).
        /// </summary>
        public dVector4 gbgg => new dVector4(y, z, y, y);
        
        /// <summary>
        /// Returns dVector3.yzyz swizzling.
        /// </summary>
        public dVector4 yzyz => new dVector4(y, z, y, z);
        
        /// <summary>
        /// Returns dVector3.gbgb swizzling (equivalent to dVector3.yzyz).
        /// </summary>
        public dVector4 gbgb => new dVector4(y, z, y, z);
        
        /// <summary>
        /// Returns dVector3.yzz swizzling.
        /// </summary>
        public dVector3 yzz => new dVector3(y, z, z);
        
        /// <summary>
        /// Returns dVector3.gbb swizzling (equivalent to dVector3.yzz).
        /// </summary>
        public dVector3 gbb => new dVector3(y, z, z);
        
        /// <summary>
        /// Returns dVector3.yzzx swizzling.
        /// </summary>
        public dVector4 yzzx => new dVector4(y, z, z, x);
        
        /// <summary>
        /// Returns dVector3.gbbr swizzling (equivalent to dVector3.yzzx).
        /// </summary>
        public dVector4 gbbr => new dVector4(y, z, z, x);
        
        /// <summary>
        /// Returns dVector3.yzzy swizzling.
        /// </summary>
        public dVector4 yzzy => new dVector4(y, z, z, y);
        
        /// <summary>
        /// Returns dVector3.gbbg swizzling (equivalent to dVector3.yzzy).
        /// </summary>
        public dVector4 gbbg => new dVector4(y, z, z, y);
        
        /// <summary>
        /// Returns dVector3.yzzz swizzling.
        /// </summary>
        public dVector4 yzzz => new dVector4(y, z, z, z);
        
        /// <summary>
        /// Returns dVector3.gbbb swizzling (equivalent to dVector3.yzzz).
        /// </summary>
        public dVector4 gbbb => new dVector4(y, z, z, z);
        
        /// <summary>
        /// Returns dVector3.zx swizzling.
        /// </summary>
        public dVector2 zx => new dVector2(z, x);
        
        /// <summary>
        /// Returns dVector3.br swizzling (equivalent to dVector3.zx).
        /// </summary>
        public dVector2 br => new dVector2(z, x);
        
        /// <summary>
        /// Returns dVector3.zxx swizzling.
        /// </summary>
        public dVector3 zxx => new dVector3(z, x, x);
        
        /// <summary>
        /// Returns dVector3.brr swizzling (equivalent to dVector3.zxx).
        /// </summary>
        public dVector3 brr => new dVector3(z, x, x);
        
        /// <summary>
        /// Returns dVector3.zxxx swizzling.
        /// </summary>
        public dVector4 zxxx => new dVector4(z, x, x, x);
        
        /// <summary>
        /// Returns dVector3.brrr swizzling (equivalent to dVector3.zxxx).
        /// </summary>
        public dVector4 brrr => new dVector4(z, x, x, x);
        
        /// <summary>
        /// Returns dVector3.zxxy swizzling.
        /// </summary>
        public dVector4 zxxy => new dVector4(z, x, x, y);
        
        /// <summary>
        /// Returns dVector3.brrg swizzling (equivalent to dVector3.zxxy).
        /// </summary>
        public dVector4 brrg => new dVector4(z, x, x, y);
        
        /// <summary>
        /// Returns dVector3.zxxz swizzling.
        /// </summary>
        public dVector4 zxxz => new dVector4(z, x, x, z);
        
        /// <summary>
        /// Returns dVector3.brrb swizzling (equivalent to dVector3.zxxz).
        /// </summary>
        public dVector4 brrb => new dVector4(z, x, x, z);
        
        /// <summary>
        /// Returns dVector3.zxy swizzling.
        /// </summary>
        public dVector3 zxy => new dVector3(z, x, y);
        
        /// <summary>
        /// Returns dVector3.brg swizzling (equivalent to dVector3.zxy).
        /// </summary>
        public dVector3 brg => new dVector3(z, x, y);
        
        /// <summary>
        /// Returns dVector3.zxyx swizzling.
        /// </summary>
        public dVector4 zxyx => new dVector4(z, x, y, x);
        
        /// <summary>
        /// Returns dVector3.brgr swizzling (equivalent to dVector3.zxyx).
        /// </summary>
        public dVector4 brgr => new dVector4(z, x, y, x);
        
        /// <summary>
        /// Returns dVector3.zxyy swizzling.
        /// </summary>
        public dVector4 zxyy => new dVector4(z, x, y, y);
        
        /// <summary>
        /// Returns dVector3.brgg swizzling (equivalent to dVector3.zxyy).
        /// </summary>
        public dVector4 brgg => new dVector4(z, x, y, y);
        
        /// <summary>
        /// Returns dVector3.zxyz swizzling.
        /// </summary>
        public dVector4 zxyz => new dVector4(z, x, y, z);
        
        /// <summary>
        /// Returns dVector3.brgb swizzling (equivalent to dVector3.zxyz).
        /// </summary>
        public dVector4 brgb => new dVector4(z, x, y, z);
        
        /// <summary>
        /// Returns dVector3.zxz swizzling.
        /// </summary>
        public dVector3 zxz => new dVector3(z, x, z);
        
        /// <summary>
        /// Returns dVector3.brb swizzling (equivalent to dVector3.zxz).
        /// </summary>
        public dVector3 brb => new dVector3(z, x, z);
        
        /// <summary>
        /// Returns dVector3.zxzx swizzling.
        /// </summary>
        public dVector4 zxzx => new dVector4(z, x, z, x);
        
        /// <summary>
        /// Returns dVector3.brbr swizzling (equivalent to dVector3.zxzx).
        /// </summary>
        public dVector4 brbr => new dVector4(z, x, z, x);
        
        /// <summary>
        /// Returns dVector3.zxzy swizzling.
        /// </summary>
        public dVector4 zxzy => new dVector4(z, x, z, y);
        
        /// <summary>
        /// Returns dVector3.brbg swizzling (equivalent to dVector3.zxzy).
        /// </summary>
        public dVector4 brbg => new dVector4(z, x, z, y);
        
        /// <summary>
        /// Returns dVector3.zxzz swizzling.
        /// </summary>
        public dVector4 zxzz => new dVector4(z, x, z, z);
        
        /// <summary>
        /// Returns dVector3.brbb swizzling (equivalent to dVector3.zxzz).
        /// </summary>
        public dVector4 brbb => new dVector4(z, x, z, z);
        
        /// <summary>
        /// Returns dVector3.zy swizzling.
        /// </summary>
        public dVector2 zy => new dVector2(z, y);
        
        /// <summary>
        /// Returns dVector3.bg swizzling (equivalent to dVector3.zy).
        /// </summary>
        public dVector2 bg => new dVector2(z, y);
        
        /// <summary>
        /// Returns dVector3.zyx swizzling.
        /// </summary>
        public dVector3 zyx => new dVector3(z, y, x);
        
        /// <summary>
        /// Returns dVector3.bgr swizzling (equivalent to dVector3.zyx).
        /// </summary>
        public dVector3 bgr => new dVector3(z, y, x);
        
        /// <summary>
        /// Returns dVector3.zyxx swizzling.
        /// </summary>
        public dVector4 zyxx => new dVector4(z, y, x, x);
        
        /// <summary>
        /// Returns dVector3.bgrr swizzling (equivalent to dVector3.zyxx).
        /// </summary>
        public dVector4 bgrr => new dVector4(z, y, x, x);
        
        /// <summary>
        /// Returns dVector3.zyxy swizzling.
        /// </summary>
        public dVector4 zyxy => new dVector4(z, y, x, y);
        
        /// <summary>
        /// Returns dVector3.bgrg swizzling (equivalent to dVector3.zyxy).
        /// </summary>
        public dVector4 bgrg => new dVector4(z, y, x, y);
        
        /// <summary>
        /// Returns dVector3.zyxz swizzling.
        /// </summary>
        public dVector4 zyxz => new dVector4(z, y, x, z);
        
        /// <summary>
        /// Returns dVector3.bgrb swizzling (equivalent to dVector3.zyxz).
        /// </summary>
        public dVector4 bgrb => new dVector4(z, y, x, z);
        
        /// <summary>
        /// Returns dVector3.zyy swizzling.
        /// </summary>
        public dVector3 zyy => new dVector3(z, y, y);
        
        /// <summary>
        /// Returns dVector3.bgg swizzling (equivalent to dVector3.zyy).
        /// </summary>
        public dVector3 bgg => new dVector3(z, y, y);
        
        /// <summary>
        /// Returns dVector3.zyyx swizzling.
        /// </summary>
        public dVector4 zyyx => new dVector4(z, y, y, x);
        
        /// <summary>
        /// Returns dVector3.bggr swizzling (equivalent to dVector3.zyyx).
        /// </summary>
        public dVector4 bggr => new dVector4(z, y, y, x);
        
        /// <summary>
        /// Returns dVector3.zyyy swizzling.
        /// </summary>
        public dVector4 zyyy => new dVector4(z, y, y, y);
        
        /// <summary>
        /// Returns dVector3.bggg swizzling (equivalent to dVector3.zyyy).
        /// </summary>
        public dVector4 bggg => new dVector4(z, y, y, y);
        
        /// <summary>
        /// Returns dVector3.zyyz swizzling.
        /// </summary>
        public dVector4 zyyz => new dVector4(z, y, y, z);
        
        /// <summary>
        /// Returns dVector3.bggb swizzling (equivalent to dVector3.zyyz).
        /// </summary>
        public dVector4 bggb => new dVector4(z, y, y, z);
        
        /// <summary>
        /// Returns dVector3.zyz swizzling.
        /// </summary>
        public dVector3 zyz => new dVector3(z, y, z);
        
        /// <summary>
        /// Returns dVector3.bgb swizzling (equivalent to dVector3.zyz).
        /// </summary>
        public dVector3 bgb => new dVector3(z, y, z);
        
        /// <summary>
        /// Returns dVector3.zyzx swizzling.
        /// </summary>
        public dVector4 zyzx => new dVector4(z, y, z, x);
        
        /// <summary>
        /// Returns dVector3.bgbr swizzling (equivalent to dVector3.zyzx).
        /// </summary>
        public dVector4 bgbr => new dVector4(z, y, z, x);
        
        /// <summary>
        /// Returns dVector3.zyzy swizzling.
        /// </summary>
        public dVector4 zyzy => new dVector4(z, y, z, y);
        
        /// <summary>
        /// Returns dVector3.bgbg swizzling (equivalent to dVector3.zyzy).
        /// </summary>
        public dVector4 bgbg => new dVector4(z, y, z, y);
        
        /// <summary>
        /// Returns dVector3.zyzz swizzling.
        /// </summary>
        public dVector4 zyzz => new dVector4(z, y, z, z);
        
        /// <summary>
        /// Returns dVector3.bgbb swizzling (equivalent to dVector3.zyzz).
        /// </summary>
        public dVector4 bgbb => new dVector4(z, y, z, z);
        
        /// <summary>
        /// Returns dVector3.zz swizzling.
        /// </summary>
        public dVector2 zz => new dVector2(z, z);
        
        /// <summary>
        /// Returns dVector3.bb swizzling (equivalent to dVector3.zz).
        /// </summary>
        public dVector2 bb => new dVector2(z, z);
        
        /// <summary>
        /// Returns dVector3.zzx swizzling.
        /// </summary>
        public dVector3 zzx => new dVector3(z, z, x);
        
        /// <summary>
        /// Returns dVector3.bbr swizzling (equivalent to dVector3.zzx).
        /// </summary>
        public dVector3 bbr => new dVector3(z, z, x);
        
        /// <summary>
        /// Returns dVector3.zzxx swizzling.
        /// </summary>
        public dVector4 zzxx => new dVector4(z, z, x, x);
        
        /// <summary>
        /// Returns dVector3.bbrr swizzling (equivalent to dVector3.zzxx).
        /// </summary>
        public dVector4 bbrr => new dVector4(z, z, x, x);
        
        /// <summary>
        /// Returns dVector3.zzxy swizzling.
        /// </summary>
        public dVector4 zzxy => new dVector4(z, z, x, y);
        
        /// <summary>
        /// Returns dVector3.bbrg swizzling (equivalent to dVector3.zzxy).
        /// </summary>
        public dVector4 bbrg => new dVector4(z, z, x, y);
        
        /// <summary>
        /// Returns dVector3.zzxz swizzling.
        /// </summary>
        public dVector4 zzxz => new dVector4(z, z, x, z);
        
        /// <summary>
        /// Returns dVector3.bbrb swizzling (equivalent to dVector3.zzxz).
        /// </summary>
        public dVector4 bbrb => new dVector4(z, z, x, z);
        
        /// <summary>
        /// Returns dVector3.zzy swizzling.
        /// </summary>
        public dVector3 zzy => new dVector3(z, z, y);
        
        /// <summary>
        /// Returns dVector3.bbg swizzling (equivalent to dVector3.zzy).
        /// </summary>
        public dVector3 bbg => new dVector3(z, z, y);
        
        /// <summary>
        /// Returns dVector3.zzyx swizzling.
        /// </summary>
        public dVector4 zzyx => new dVector4(z, z, y, x);
        
        /// <summary>
        /// Returns dVector3.bbgr swizzling (equivalent to dVector3.zzyx).
        /// </summary>
        public dVector4 bbgr => new dVector4(z, z, y, x);
        
        /// <summary>
        /// Returns dVector3.zzyy swizzling.
        /// </summary>
        public dVector4 zzyy => new dVector4(z, z, y, y);
        
        /// <summary>
        /// Returns dVector3.bbgg swizzling (equivalent to dVector3.zzyy).
        /// </summary>
        public dVector4 bbgg => new dVector4(z, z, y, y);
        
        /// <summary>
        /// Returns dVector3.zzyz swizzling.
        /// </summary>
        public dVector4 zzyz => new dVector4(z, z, y, z);
        
        /// <summary>
        /// Returns dVector3.bbgb swizzling (equivalent to dVector3.zzyz).
        /// </summary>
        public dVector4 bbgb => new dVector4(z, z, y, z);
        
        /// <summary>
        /// Returns dVector3.zzz swizzling.
        /// </summary>
        public dVector3 zzz => new dVector3(z, z, z);
        
        /// <summary>
        /// Returns dVector3.bbb swizzling (equivalent to dVector3.zzz).
        /// </summary>
        public dVector3 bbb => new dVector3(z, z, z);
        
        /// <summary>
        /// Returns dVector3.zzzx swizzling.
        /// </summary>
        public dVector4 zzzx => new dVector4(z, z, z, x);
        
        /// <summary>
        /// Returns dVector3.bbbr swizzling (equivalent to dVector3.zzzx).
        /// </summary>
        public dVector4 bbbr => new dVector4(z, z, z, x);
        
        /// <summary>
        /// Returns dVector3.zzzy swizzling.
        /// </summary>
        public dVector4 zzzy => new dVector4(z, z, z, y);
        
        /// <summary>
        /// Returns dVector3.bbbg swizzling (equivalent to dVector3.zzzy).
        /// </summary>
        public dVector4 bbbg => new dVector4(z, z, z, y);
        
        /// <summary>
        /// Returns dVector3.zzzz swizzling.
        /// </summary>
        public dVector4 zzzz => new dVector4(z, z, z, z);
        
        /// <summary>
        /// Returns dVector3.bbbb swizzling (equivalent to dVector3.zzzz).
        /// </summary>
        public dVector4 bbbb => new dVector4(z, z, z, z);

        #endregion

    }
}
