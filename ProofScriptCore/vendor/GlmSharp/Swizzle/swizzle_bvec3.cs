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
    /// Temporary Vector of type bool with 3 components, used for implementing swizzling for bVector3.
    /// </summary>
    [RegisterCoreClassStruct]
   
    [StructLayout(LayoutKind.Sequential)]
    public struct swizzle_bVector3
    {

        #region Fields
        
        /// <summary>
        /// x-component
        /// </summary>
        
        internal readonly bool x;
        
        /// <summary>
        /// y-component
        /// </summary>
        
        internal readonly bool y;
        
        /// <summary>
        /// z-component
        /// </summary>
        
        internal readonly bool z;

        #endregion


        #region Constructors
        
        /// <summary>
        /// Constructor for swizzle_bVector3.
        /// </summary>
        internal swizzle_bVector3(bool x, bool y, bool z)
        {
            this.x = x;
            this.y = y;
            this.z = z;
        }

        #endregion


        #region Properties
        
        /// <summary>
        /// Returns bVector3.xx swizzling.
        /// </summary>
        public bVector2 xx => new bVector2(x, x);
        
        /// <summary>
        /// Returns bVector3.rr swizzling (equivalent to bVector3.xx).
        /// </summary>
        public bVector2 rr => new bVector2(x, x);
        
        /// <summary>
        /// Returns bVector3.xxx swizzling.
        /// </summary>
        public bVector3 xxx => new bVector3(x, x, x);
        
        /// <summary>
        /// Returns bVector3.rrr swizzling (equivalent to bVector3.xxx).
        /// </summary>
        public bVector3 rrr => new bVector3(x, x, x);
        
        /// <summary>
        /// Returns bVector3.xxxx swizzling.
        /// </summary>
        public bVector4 xxxx => new bVector4(x, x, x, x);
        
        /// <summary>
        /// Returns bVector3.rrrr swizzling (equivalent to bVector3.xxxx).
        /// </summary>
        public bVector4 rrrr => new bVector4(x, x, x, x);
        
        /// <summary>
        /// Returns bVector3.xxxy swizzling.
        /// </summary>
        public bVector4 xxxy => new bVector4(x, x, x, y);
        
        /// <summary>
        /// Returns bVector3.rrrg swizzling (equivalent to bVector3.xxxy).
        /// </summary>
        public bVector4 rrrg => new bVector4(x, x, x, y);
        
        /// <summary>
        /// Returns bVector3.xxxz swizzling.
        /// </summary>
        public bVector4 xxxz => new bVector4(x, x, x, z);
        
        /// <summary>
        /// Returns bVector3.rrrb swizzling (equivalent to bVector3.xxxz).
        /// </summary>
        public bVector4 rrrb => new bVector4(x, x, x, z);
        
        /// <summary>
        /// Returns bVector3.xxy swizzling.
        /// </summary>
        public bVector3 xxy => new bVector3(x, x, y);
        
        /// <summary>
        /// Returns bVector3.rrg swizzling (equivalent to bVector3.xxy).
        /// </summary>
        public bVector3 rrg => new bVector3(x, x, y);
        
        /// <summary>
        /// Returns bVector3.xxyx swizzling.
        /// </summary>
        public bVector4 xxyx => new bVector4(x, x, y, x);
        
        /// <summary>
        /// Returns bVector3.rrgr swizzling (equivalent to bVector3.xxyx).
        /// </summary>
        public bVector4 rrgr => new bVector4(x, x, y, x);
        
        /// <summary>
        /// Returns bVector3.xxyy swizzling.
        /// </summary>
        public bVector4 xxyy => new bVector4(x, x, y, y);
        
        /// <summary>
        /// Returns bVector3.rrgg swizzling (equivalent to bVector3.xxyy).
        /// </summary>
        public bVector4 rrgg => new bVector4(x, x, y, y);
        
        /// <summary>
        /// Returns bVector3.xxyz swizzling.
        /// </summary>
        public bVector4 xxyz => new bVector4(x, x, y, z);
        
        /// <summary>
        /// Returns bVector3.rrgb swizzling (equivalent to bVector3.xxyz).
        /// </summary>
        public bVector4 rrgb => new bVector4(x, x, y, z);
        
        /// <summary>
        /// Returns bVector3.xxz swizzling.
        /// </summary>
        public bVector3 xxz => new bVector3(x, x, z);
        
        /// <summary>
        /// Returns bVector3.rrb swizzling (equivalent to bVector3.xxz).
        /// </summary>
        public bVector3 rrb => new bVector3(x, x, z);
        
        /// <summary>
        /// Returns bVector3.xxzx swizzling.
        /// </summary>
        public bVector4 xxzx => new bVector4(x, x, z, x);
        
        /// <summary>
        /// Returns bVector3.rrbr swizzling (equivalent to bVector3.xxzx).
        /// </summary>
        public bVector4 rrbr => new bVector4(x, x, z, x);
        
        /// <summary>
        /// Returns bVector3.xxzy swizzling.
        /// </summary>
        public bVector4 xxzy => new bVector4(x, x, z, y);
        
        /// <summary>
        /// Returns bVector3.rrbg swizzling (equivalent to bVector3.xxzy).
        /// </summary>
        public bVector4 rrbg => new bVector4(x, x, z, y);
        
        /// <summary>
        /// Returns bVector3.xxzz swizzling.
        /// </summary>
        public bVector4 xxzz => new bVector4(x, x, z, z);
        
        /// <summary>
        /// Returns bVector3.rrbb swizzling (equivalent to bVector3.xxzz).
        /// </summary>
        public bVector4 rrbb => new bVector4(x, x, z, z);
        
        /// <summary>
        /// Returns bVector3.xy swizzling.
        /// </summary>
        public bVector2 xy => new bVector2(x, y);
        
        /// <summary>
        /// Returns bVector3.rg swizzling (equivalent to bVector3.xy).
        /// </summary>
        public bVector2 rg => new bVector2(x, y);
        
        /// <summary>
        /// Returns bVector3.xyx swizzling.
        /// </summary>
        public bVector3 xyx => new bVector3(x, y, x);
        
        /// <summary>
        /// Returns bVector3.rgr swizzling (equivalent to bVector3.xyx).
        /// </summary>
        public bVector3 rgr => new bVector3(x, y, x);
        
        /// <summary>
        /// Returns bVector3.xyxx swizzling.
        /// </summary>
        public bVector4 xyxx => new bVector4(x, y, x, x);
        
        /// <summary>
        /// Returns bVector3.rgrr swizzling (equivalent to bVector3.xyxx).
        /// </summary>
        public bVector4 rgrr => new bVector4(x, y, x, x);
        
        /// <summary>
        /// Returns bVector3.xyxy swizzling.
        /// </summary>
        public bVector4 xyxy => new bVector4(x, y, x, y);
        
        /// <summary>
        /// Returns bVector3.rgrg swizzling (equivalent to bVector3.xyxy).
        /// </summary>
        public bVector4 rgrg => new bVector4(x, y, x, y);
        
        /// <summary>
        /// Returns bVector3.xyxz swizzling.
        /// </summary>
        public bVector4 xyxz => new bVector4(x, y, x, z);
        
        /// <summary>
        /// Returns bVector3.rgrb swizzling (equivalent to bVector3.xyxz).
        /// </summary>
        public bVector4 rgrb => new bVector4(x, y, x, z);
        
        /// <summary>
        /// Returns bVector3.xyy swizzling.
        /// </summary>
        public bVector3 xyy => new bVector3(x, y, y);
        
        /// <summary>
        /// Returns bVector3.rgg swizzling (equivalent to bVector3.xyy).
        /// </summary>
        public bVector3 rgg => new bVector3(x, y, y);
        
        /// <summary>
        /// Returns bVector3.xyyx swizzling.
        /// </summary>
        public bVector4 xyyx => new bVector4(x, y, y, x);
        
        /// <summary>
        /// Returns bVector3.rggr swizzling (equivalent to bVector3.xyyx).
        /// </summary>
        public bVector4 rggr => new bVector4(x, y, y, x);
        
        /// <summary>
        /// Returns bVector3.xyyy swizzling.
        /// </summary>
        public bVector4 xyyy => new bVector4(x, y, y, y);
        
        /// <summary>
        /// Returns bVector3.rggg swizzling (equivalent to bVector3.xyyy).
        /// </summary>
        public bVector4 rggg => new bVector4(x, y, y, y);
        
        /// <summary>
        /// Returns bVector3.xyyz swizzling.
        /// </summary>
        public bVector4 xyyz => new bVector4(x, y, y, z);
        
        /// <summary>
        /// Returns bVector3.rggb swizzling (equivalent to bVector3.xyyz).
        /// </summary>
        public bVector4 rggb => new bVector4(x, y, y, z);
        
        /// <summary>
        /// Returns bVector3.xyz swizzling.
        /// </summary>
        public bVector3 xyz => new bVector3(x, y, z);
        
        /// <summary>
        /// Returns bVector3.rgb swizzling (equivalent to bVector3.xyz).
        /// </summary>
        public bVector3 rgb => new bVector3(x, y, z);
        
        /// <summary>
        /// Returns bVector3.xyzx swizzling.
        /// </summary>
        public bVector4 xyzx => new bVector4(x, y, z, x);
        
        /// <summary>
        /// Returns bVector3.rgbr swizzling (equivalent to bVector3.xyzx).
        /// </summary>
        public bVector4 rgbr => new bVector4(x, y, z, x);
        
        /// <summary>
        /// Returns bVector3.xyzy swizzling.
        /// </summary>
        public bVector4 xyzy => new bVector4(x, y, z, y);
        
        /// <summary>
        /// Returns bVector3.rgbg swizzling (equivalent to bVector3.xyzy).
        /// </summary>
        public bVector4 rgbg => new bVector4(x, y, z, y);
        
        /// <summary>
        /// Returns bVector3.xyzz swizzling.
        /// </summary>
        public bVector4 xyzz => new bVector4(x, y, z, z);
        
        /// <summary>
        /// Returns bVector3.rgbb swizzling (equivalent to bVector3.xyzz).
        /// </summary>
        public bVector4 rgbb => new bVector4(x, y, z, z);
        
        /// <summary>
        /// Returns bVector3.xz swizzling.
        /// </summary>
        public bVector2 xz => new bVector2(x, z);
        
        /// <summary>
        /// Returns bVector3.rb swizzling (equivalent to bVector3.xz).
        /// </summary>
        public bVector2 rb => new bVector2(x, z);
        
        /// <summary>
        /// Returns bVector3.xzx swizzling.
        /// </summary>
        public bVector3 xzx => new bVector3(x, z, x);
        
        /// <summary>
        /// Returns bVector3.rbr swizzling (equivalent to bVector3.xzx).
        /// </summary>
        public bVector3 rbr => new bVector3(x, z, x);
        
        /// <summary>
        /// Returns bVector3.xzxx swizzling.
        /// </summary>
        public bVector4 xzxx => new bVector4(x, z, x, x);
        
        /// <summary>
        /// Returns bVector3.rbrr swizzling (equivalent to bVector3.xzxx).
        /// </summary>
        public bVector4 rbrr => new bVector4(x, z, x, x);
        
        /// <summary>
        /// Returns bVector3.xzxy swizzling.
        /// </summary>
        public bVector4 xzxy => new bVector4(x, z, x, y);
        
        /// <summary>
        /// Returns bVector3.rbrg swizzling (equivalent to bVector3.xzxy).
        /// </summary>
        public bVector4 rbrg => new bVector4(x, z, x, y);
        
        /// <summary>
        /// Returns bVector3.xzxz swizzling.
        /// </summary>
        public bVector4 xzxz => new bVector4(x, z, x, z);
        
        /// <summary>
        /// Returns bVector3.rbrb swizzling (equivalent to bVector3.xzxz).
        /// </summary>
        public bVector4 rbrb => new bVector4(x, z, x, z);
        
        /// <summary>
        /// Returns bVector3.xzy swizzling.
        /// </summary>
        public bVector3 xzy => new bVector3(x, z, y);
        
        /// <summary>
        /// Returns bVector3.rbg swizzling (equivalent to bVector3.xzy).
        /// </summary>
        public bVector3 rbg => new bVector3(x, z, y);
        
        /// <summary>
        /// Returns bVector3.xzyx swizzling.
        /// </summary>
        public bVector4 xzyx => new bVector4(x, z, y, x);
        
        /// <summary>
        /// Returns bVector3.rbgr swizzling (equivalent to bVector3.xzyx).
        /// </summary>
        public bVector4 rbgr => new bVector4(x, z, y, x);
        
        /// <summary>
        /// Returns bVector3.xzyy swizzling.
        /// </summary>
        public bVector4 xzyy => new bVector4(x, z, y, y);
        
        /// <summary>
        /// Returns bVector3.rbgg swizzling (equivalent to bVector3.xzyy).
        /// </summary>
        public bVector4 rbgg => new bVector4(x, z, y, y);
        
        /// <summary>
        /// Returns bVector3.xzyz swizzling.
        /// </summary>
        public bVector4 xzyz => new bVector4(x, z, y, z);
        
        /// <summary>
        /// Returns bVector3.rbgb swizzling (equivalent to bVector3.xzyz).
        /// </summary>
        public bVector4 rbgb => new bVector4(x, z, y, z);
        
        /// <summary>
        /// Returns bVector3.xzz swizzling.
        /// </summary>
        public bVector3 xzz => new bVector3(x, z, z);
        
        /// <summary>
        /// Returns bVector3.rbb swizzling (equivalent to bVector3.xzz).
        /// </summary>
        public bVector3 rbb => new bVector3(x, z, z);
        
        /// <summary>
        /// Returns bVector3.xzzx swizzling.
        /// </summary>
        public bVector4 xzzx => new bVector4(x, z, z, x);
        
        /// <summary>
        /// Returns bVector3.rbbr swizzling (equivalent to bVector3.xzzx).
        /// </summary>
        public bVector4 rbbr => new bVector4(x, z, z, x);
        
        /// <summary>
        /// Returns bVector3.xzzy swizzling.
        /// </summary>
        public bVector4 xzzy => new bVector4(x, z, z, y);
        
        /// <summary>
        /// Returns bVector3.rbbg swizzling (equivalent to bVector3.xzzy).
        /// </summary>
        public bVector4 rbbg => new bVector4(x, z, z, y);
        
        /// <summary>
        /// Returns bVector3.xzzz swizzling.
        /// </summary>
        public bVector4 xzzz => new bVector4(x, z, z, z);
        
        /// <summary>
        /// Returns bVector3.rbbb swizzling (equivalent to bVector3.xzzz).
        /// </summary>
        public bVector4 rbbb => new bVector4(x, z, z, z);
        
        /// <summary>
        /// Returns bVector3.yx swizzling.
        /// </summary>
        public bVector2 yx => new bVector2(y, x);
        
        /// <summary>
        /// Returns bVector3.gr swizzling (equivalent to bVector3.yx).
        /// </summary>
        public bVector2 gr => new bVector2(y, x);
        
        /// <summary>
        /// Returns bVector3.yxx swizzling.
        /// </summary>
        public bVector3 yxx => new bVector3(y, x, x);
        
        /// <summary>
        /// Returns bVector3.grr swizzling (equivalent to bVector3.yxx).
        /// </summary>
        public bVector3 grr => new bVector3(y, x, x);
        
        /// <summary>
        /// Returns bVector3.yxxx swizzling.
        /// </summary>
        public bVector4 yxxx => new bVector4(y, x, x, x);
        
        /// <summary>
        /// Returns bVector3.grrr swizzling (equivalent to bVector3.yxxx).
        /// </summary>
        public bVector4 grrr => new bVector4(y, x, x, x);
        
        /// <summary>
        /// Returns bVector3.yxxy swizzling.
        /// </summary>
        public bVector4 yxxy => new bVector4(y, x, x, y);
        
        /// <summary>
        /// Returns bVector3.grrg swizzling (equivalent to bVector3.yxxy).
        /// </summary>
        public bVector4 grrg => new bVector4(y, x, x, y);
        
        /// <summary>
        /// Returns bVector3.yxxz swizzling.
        /// </summary>
        public bVector4 yxxz => new bVector4(y, x, x, z);
        
        /// <summary>
        /// Returns bVector3.grrb swizzling (equivalent to bVector3.yxxz).
        /// </summary>
        public bVector4 grrb => new bVector4(y, x, x, z);
        
        /// <summary>
        /// Returns bVector3.yxy swizzling.
        /// </summary>
        public bVector3 yxy => new bVector3(y, x, y);
        
        /// <summary>
        /// Returns bVector3.grg swizzling (equivalent to bVector3.yxy).
        /// </summary>
        public bVector3 grg => new bVector3(y, x, y);
        
        /// <summary>
        /// Returns bVector3.yxyx swizzling.
        /// </summary>
        public bVector4 yxyx => new bVector4(y, x, y, x);
        
        /// <summary>
        /// Returns bVector3.grgr swizzling (equivalent to bVector3.yxyx).
        /// </summary>
        public bVector4 grgr => new bVector4(y, x, y, x);
        
        /// <summary>
        /// Returns bVector3.yxyy swizzling.
        /// </summary>
        public bVector4 yxyy => new bVector4(y, x, y, y);
        
        /// <summary>
        /// Returns bVector3.grgg swizzling (equivalent to bVector3.yxyy).
        /// </summary>
        public bVector4 grgg => new bVector4(y, x, y, y);
        
        /// <summary>
        /// Returns bVector3.yxyz swizzling.
        /// </summary>
        public bVector4 yxyz => new bVector4(y, x, y, z);
        
        /// <summary>
        /// Returns bVector3.grgb swizzling (equivalent to bVector3.yxyz).
        /// </summary>
        public bVector4 grgb => new bVector4(y, x, y, z);
        
        /// <summary>
        /// Returns bVector3.yxz swizzling.
        /// </summary>
        public bVector3 yxz => new bVector3(y, x, z);
        
        /// <summary>
        /// Returns bVector3.grb swizzling (equivalent to bVector3.yxz).
        /// </summary>
        public bVector3 grb => new bVector3(y, x, z);
        
        /// <summary>
        /// Returns bVector3.yxzx swizzling.
        /// </summary>
        public bVector4 yxzx => new bVector4(y, x, z, x);
        
        /// <summary>
        /// Returns bVector3.grbr swizzling (equivalent to bVector3.yxzx).
        /// </summary>
        public bVector4 grbr => new bVector4(y, x, z, x);
        
        /// <summary>
        /// Returns bVector3.yxzy swizzling.
        /// </summary>
        public bVector4 yxzy => new bVector4(y, x, z, y);
        
        /// <summary>
        /// Returns bVector3.grbg swizzling (equivalent to bVector3.yxzy).
        /// </summary>
        public bVector4 grbg => new bVector4(y, x, z, y);
        
        /// <summary>
        /// Returns bVector3.yxzz swizzling.
        /// </summary>
        public bVector4 yxzz => new bVector4(y, x, z, z);
        
        /// <summary>
        /// Returns bVector3.grbb swizzling (equivalent to bVector3.yxzz).
        /// </summary>
        public bVector4 grbb => new bVector4(y, x, z, z);
        
        /// <summary>
        /// Returns bVector3.yy swizzling.
        /// </summary>
        public bVector2 yy => new bVector2(y, y);
        
        /// <summary>
        /// Returns bVector3.gg swizzling (equivalent to bVector3.yy).
        /// </summary>
        public bVector2 gg => new bVector2(y, y);
        
        /// <summary>
        /// Returns bVector3.yyx swizzling.
        /// </summary>
        public bVector3 yyx => new bVector3(y, y, x);
        
        /// <summary>
        /// Returns bVector3.ggr swizzling (equivalent to bVector3.yyx).
        /// </summary>
        public bVector3 ggr => new bVector3(y, y, x);
        
        /// <summary>
        /// Returns bVector3.yyxx swizzling.
        /// </summary>
        public bVector4 yyxx => new bVector4(y, y, x, x);
        
        /// <summary>
        /// Returns bVector3.ggrr swizzling (equivalent to bVector3.yyxx).
        /// </summary>
        public bVector4 ggrr => new bVector4(y, y, x, x);
        
        /// <summary>
        /// Returns bVector3.yyxy swizzling.
        /// </summary>
        public bVector4 yyxy => new bVector4(y, y, x, y);
        
        /// <summary>
        /// Returns bVector3.ggrg swizzling (equivalent to bVector3.yyxy).
        /// </summary>
        public bVector4 ggrg => new bVector4(y, y, x, y);
        
        /// <summary>
        /// Returns bVector3.yyxz swizzling.
        /// </summary>
        public bVector4 yyxz => new bVector4(y, y, x, z);
        
        /// <summary>
        /// Returns bVector3.ggrb swizzling (equivalent to bVector3.yyxz).
        /// </summary>
        public bVector4 ggrb => new bVector4(y, y, x, z);
        
        /// <summary>
        /// Returns bVector3.yyy swizzling.
        /// </summary>
        public bVector3 yyy => new bVector3(y, y, y);
        
        /// <summary>
        /// Returns bVector3.ggg swizzling (equivalent to bVector3.yyy).
        /// </summary>
        public bVector3 ggg => new bVector3(y, y, y);
        
        /// <summary>
        /// Returns bVector3.yyyx swizzling.
        /// </summary>
        public bVector4 yyyx => new bVector4(y, y, y, x);
        
        /// <summary>
        /// Returns bVector3.gggr swizzling (equivalent to bVector3.yyyx).
        /// </summary>
        public bVector4 gggr => new bVector4(y, y, y, x);
        
        /// <summary>
        /// Returns bVector3.yyyy swizzling.
        /// </summary>
        public bVector4 yyyy => new bVector4(y, y, y, y);
        
        /// <summary>
        /// Returns bVector3.gggg swizzling (equivalent to bVector3.yyyy).
        /// </summary>
        public bVector4 gggg => new bVector4(y, y, y, y);
        
        /// <summary>
        /// Returns bVector3.yyyz swizzling.
        /// </summary>
        public bVector4 yyyz => new bVector4(y, y, y, z);
        
        /// <summary>
        /// Returns bVector3.gggb swizzling (equivalent to bVector3.yyyz).
        /// </summary>
        public bVector4 gggb => new bVector4(y, y, y, z);
        
        /// <summary>
        /// Returns bVector3.yyz swizzling.
        /// </summary>
        public bVector3 yyz => new bVector3(y, y, z);
        
        /// <summary>
        /// Returns bVector3.ggb swizzling (equivalent to bVector3.yyz).
        /// </summary>
        public bVector3 ggb => new bVector3(y, y, z);
        
        /// <summary>
        /// Returns bVector3.yyzx swizzling.
        /// </summary>
        public bVector4 yyzx => new bVector4(y, y, z, x);
        
        /// <summary>
        /// Returns bVector3.ggbr swizzling (equivalent to bVector3.yyzx).
        /// </summary>
        public bVector4 ggbr => new bVector4(y, y, z, x);
        
        /// <summary>
        /// Returns bVector3.yyzy swizzling.
        /// </summary>
        public bVector4 yyzy => new bVector4(y, y, z, y);
        
        /// <summary>
        /// Returns bVector3.ggbg swizzling (equivalent to bVector3.yyzy).
        /// </summary>
        public bVector4 ggbg => new bVector4(y, y, z, y);
        
        /// <summary>
        /// Returns bVector3.yyzz swizzling.
        /// </summary>
        public bVector4 yyzz => new bVector4(y, y, z, z);
        
        /// <summary>
        /// Returns bVector3.ggbb swizzling (equivalent to bVector3.yyzz).
        /// </summary>
        public bVector4 ggbb => new bVector4(y, y, z, z);
        
        /// <summary>
        /// Returns bVector3.yz swizzling.
        /// </summary>
        public bVector2 yz => new bVector2(y, z);
        
        /// <summary>
        /// Returns bVector3.gb swizzling (equivalent to bVector3.yz).
        /// </summary>
        public bVector2 gb => new bVector2(y, z);
        
        /// <summary>
        /// Returns bVector3.yzx swizzling.
        /// </summary>
        public bVector3 yzx => new bVector3(y, z, x);
        
        /// <summary>
        /// Returns bVector3.gbr swizzling (equivalent to bVector3.yzx).
        /// </summary>
        public bVector3 gbr => new bVector3(y, z, x);
        
        /// <summary>
        /// Returns bVector3.yzxx swizzling.
        /// </summary>
        public bVector4 yzxx => new bVector4(y, z, x, x);
        
        /// <summary>
        /// Returns bVector3.gbrr swizzling (equivalent to bVector3.yzxx).
        /// </summary>
        public bVector4 gbrr => new bVector4(y, z, x, x);
        
        /// <summary>
        /// Returns bVector3.yzxy swizzling.
        /// </summary>
        public bVector4 yzxy => new bVector4(y, z, x, y);
        
        /// <summary>
        /// Returns bVector3.gbrg swizzling (equivalent to bVector3.yzxy).
        /// </summary>
        public bVector4 gbrg => new bVector4(y, z, x, y);
        
        /// <summary>
        /// Returns bVector3.yzxz swizzling.
        /// </summary>
        public bVector4 yzxz => new bVector4(y, z, x, z);
        
        /// <summary>
        /// Returns bVector3.gbrb swizzling (equivalent to bVector3.yzxz).
        /// </summary>
        public bVector4 gbrb => new bVector4(y, z, x, z);
        
        /// <summary>
        /// Returns bVector3.yzy swizzling.
        /// </summary>
        public bVector3 yzy => new bVector3(y, z, y);
        
        /// <summary>
        /// Returns bVector3.gbg swizzling (equivalent to bVector3.yzy).
        /// </summary>
        public bVector3 gbg => new bVector3(y, z, y);
        
        /// <summary>
        /// Returns bVector3.yzyx swizzling.
        /// </summary>
        public bVector4 yzyx => new bVector4(y, z, y, x);
        
        /// <summary>
        /// Returns bVector3.gbgr swizzling (equivalent to bVector3.yzyx).
        /// </summary>
        public bVector4 gbgr => new bVector4(y, z, y, x);
        
        /// <summary>
        /// Returns bVector3.yzyy swizzling.
        /// </summary>
        public bVector4 yzyy => new bVector4(y, z, y, y);
        
        /// <summary>
        /// Returns bVector3.gbgg swizzling (equivalent to bVector3.yzyy).
        /// </summary>
        public bVector4 gbgg => new bVector4(y, z, y, y);
        
        /// <summary>
        /// Returns bVector3.yzyz swizzling.
        /// </summary>
        public bVector4 yzyz => new bVector4(y, z, y, z);
        
        /// <summary>
        /// Returns bVector3.gbgb swizzling (equivalent to bVector3.yzyz).
        /// </summary>
        public bVector4 gbgb => new bVector4(y, z, y, z);
        
        /// <summary>
        /// Returns bVector3.yzz swizzling.
        /// </summary>
        public bVector3 yzz => new bVector3(y, z, z);
        
        /// <summary>
        /// Returns bVector3.gbb swizzling (equivalent to bVector3.yzz).
        /// </summary>
        public bVector3 gbb => new bVector3(y, z, z);
        
        /// <summary>
        /// Returns bVector3.yzzx swizzling.
        /// </summary>
        public bVector4 yzzx => new bVector4(y, z, z, x);
        
        /// <summary>
        /// Returns bVector3.gbbr swizzling (equivalent to bVector3.yzzx).
        /// </summary>
        public bVector4 gbbr => new bVector4(y, z, z, x);
        
        /// <summary>
        /// Returns bVector3.yzzy swizzling.
        /// </summary>
        public bVector4 yzzy => new bVector4(y, z, z, y);
        
        /// <summary>
        /// Returns bVector3.gbbg swizzling (equivalent to bVector3.yzzy).
        /// </summary>
        public bVector4 gbbg => new bVector4(y, z, z, y);
        
        /// <summary>
        /// Returns bVector3.yzzz swizzling.
        /// </summary>
        public bVector4 yzzz => new bVector4(y, z, z, z);
        
        /// <summary>
        /// Returns bVector3.gbbb swizzling (equivalent to bVector3.yzzz).
        /// </summary>
        public bVector4 gbbb => new bVector4(y, z, z, z);
        
        /// <summary>
        /// Returns bVector3.zx swizzling.
        /// </summary>
        public bVector2 zx => new bVector2(z, x);
        
        /// <summary>
        /// Returns bVector3.br swizzling (equivalent to bVector3.zx).
        /// </summary>
        public bVector2 br => new bVector2(z, x);
        
        /// <summary>
        /// Returns bVector3.zxx swizzling.
        /// </summary>
        public bVector3 zxx => new bVector3(z, x, x);
        
        /// <summary>
        /// Returns bVector3.brr swizzling (equivalent to bVector3.zxx).
        /// </summary>
        public bVector3 brr => new bVector3(z, x, x);
        
        /// <summary>
        /// Returns bVector3.zxxx swizzling.
        /// </summary>
        public bVector4 zxxx => new bVector4(z, x, x, x);
        
        /// <summary>
        /// Returns bVector3.brrr swizzling (equivalent to bVector3.zxxx).
        /// </summary>
        public bVector4 brrr => new bVector4(z, x, x, x);
        
        /// <summary>
        /// Returns bVector3.zxxy swizzling.
        /// </summary>
        public bVector4 zxxy => new bVector4(z, x, x, y);
        
        /// <summary>
        /// Returns bVector3.brrg swizzling (equivalent to bVector3.zxxy).
        /// </summary>
        public bVector4 brrg => new bVector4(z, x, x, y);
        
        /// <summary>
        /// Returns bVector3.zxxz swizzling.
        /// </summary>
        public bVector4 zxxz => new bVector4(z, x, x, z);
        
        /// <summary>
        /// Returns bVector3.brrb swizzling (equivalent to bVector3.zxxz).
        /// </summary>
        public bVector4 brrb => new bVector4(z, x, x, z);
        
        /// <summary>
        /// Returns bVector3.zxy swizzling.
        /// </summary>
        public bVector3 zxy => new bVector3(z, x, y);
        
        /// <summary>
        /// Returns bVector3.brg swizzling (equivalent to bVector3.zxy).
        /// </summary>
        public bVector3 brg => new bVector3(z, x, y);
        
        /// <summary>
        /// Returns bVector3.zxyx swizzling.
        /// </summary>
        public bVector4 zxyx => new bVector4(z, x, y, x);
        
        /// <summary>
        /// Returns bVector3.brgr swizzling (equivalent to bVector3.zxyx).
        /// </summary>
        public bVector4 brgr => new bVector4(z, x, y, x);
        
        /// <summary>
        /// Returns bVector3.zxyy swizzling.
        /// </summary>
        public bVector4 zxyy => new bVector4(z, x, y, y);
        
        /// <summary>
        /// Returns bVector3.brgg swizzling (equivalent to bVector3.zxyy).
        /// </summary>
        public bVector4 brgg => new bVector4(z, x, y, y);
        
        /// <summary>
        /// Returns bVector3.zxyz swizzling.
        /// </summary>
        public bVector4 zxyz => new bVector4(z, x, y, z);
        
        /// <summary>
        /// Returns bVector3.brgb swizzling (equivalent to bVector3.zxyz).
        /// </summary>
        public bVector4 brgb => new bVector4(z, x, y, z);
        
        /// <summary>
        /// Returns bVector3.zxz swizzling.
        /// </summary>
        public bVector3 zxz => new bVector3(z, x, z);
        
        /// <summary>
        /// Returns bVector3.brb swizzling (equivalent to bVector3.zxz).
        /// </summary>
        public bVector3 brb => new bVector3(z, x, z);
        
        /// <summary>
        /// Returns bVector3.zxzx swizzling.
        /// </summary>
        public bVector4 zxzx => new bVector4(z, x, z, x);
        
        /// <summary>
        /// Returns bVector3.brbr swizzling (equivalent to bVector3.zxzx).
        /// </summary>
        public bVector4 brbr => new bVector4(z, x, z, x);
        
        /// <summary>
        /// Returns bVector3.zxzy swizzling.
        /// </summary>
        public bVector4 zxzy => new bVector4(z, x, z, y);
        
        /// <summary>
        /// Returns bVector3.brbg swizzling (equivalent to bVector3.zxzy).
        /// </summary>
        public bVector4 brbg => new bVector4(z, x, z, y);
        
        /// <summary>
        /// Returns bVector3.zxzz swizzling.
        /// </summary>
        public bVector4 zxzz => new bVector4(z, x, z, z);
        
        /// <summary>
        /// Returns bVector3.brbb swizzling (equivalent to bVector3.zxzz).
        /// </summary>
        public bVector4 brbb => new bVector4(z, x, z, z);
        
        /// <summary>
        /// Returns bVector3.zy swizzling.
        /// </summary>
        public bVector2 zy => new bVector2(z, y);
        
        /// <summary>
        /// Returns bVector3.bg swizzling (equivalent to bVector3.zy).
        /// </summary>
        public bVector2 bg => new bVector2(z, y);
        
        /// <summary>
        /// Returns bVector3.zyx swizzling.
        /// </summary>
        public bVector3 zyx => new bVector3(z, y, x);
        
        /// <summary>
        /// Returns bVector3.bgr swizzling (equivalent to bVector3.zyx).
        /// </summary>
        public bVector3 bgr => new bVector3(z, y, x);
        
        /// <summary>
        /// Returns bVector3.zyxx swizzling.
        /// </summary>
        public bVector4 zyxx => new bVector4(z, y, x, x);
        
        /// <summary>
        /// Returns bVector3.bgrr swizzling (equivalent to bVector3.zyxx).
        /// </summary>
        public bVector4 bgrr => new bVector4(z, y, x, x);
        
        /// <summary>
        /// Returns bVector3.zyxy swizzling.
        /// </summary>
        public bVector4 zyxy => new bVector4(z, y, x, y);
        
        /// <summary>
        /// Returns bVector3.bgrg swizzling (equivalent to bVector3.zyxy).
        /// </summary>
        public bVector4 bgrg => new bVector4(z, y, x, y);
        
        /// <summary>
        /// Returns bVector3.zyxz swizzling.
        /// </summary>
        public bVector4 zyxz => new bVector4(z, y, x, z);
        
        /// <summary>
        /// Returns bVector3.bgrb swizzling (equivalent to bVector3.zyxz).
        /// </summary>
        public bVector4 bgrb => new bVector4(z, y, x, z);
        
        /// <summary>
        /// Returns bVector3.zyy swizzling.
        /// </summary>
        public bVector3 zyy => new bVector3(z, y, y);
        
        /// <summary>
        /// Returns bVector3.bgg swizzling (equivalent to bVector3.zyy).
        /// </summary>
        public bVector3 bgg => new bVector3(z, y, y);
        
        /// <summary>
        /// Returns bVector3.zyyx swizzling.
        /// </summary>
        public bVector4 zyyx => new bVector4(z, y, y, x);
        
        /// <summary>
        /// Returns bVector3.bggr swizzling (equivalent to bVector3.zyyx).
        /// </summary>
        public bVector4 bggr => new bVector4(z, y, y, x);
        
        /// <summary>
        /// Returns bVector3.zyyy swizzling.
        /// </summary>
        public bVector4 zyyy => new bVector4(z, y, y, y);
        
        /// <summary>
        /// Returns bVector3.bggg swizzling (equivalent to bVector3.zyyy).
        /// </summary>
        public bVector4 bggg => new bVector4(z, y, y, y);
        
        /// <summary>
        /// Returns bVector3.zyyz swizzling.
        /// </summary>
        public bVector4 zyyz => new bVector4(z, y, y, z);
        
        /// <summary>
        /// Returns bVector3.bggb swizzling (equivalent to bVector3.zyyz).
        /// </summary>
        public bVector4 bggb => new bVector4(z, y, y, z);
        
        /// <summary>
        /// Returns bVector3.zyz swizzling.
        /// </summary>
        public bVector3 zyz => new bVector3(z, y, z);
        
        /// <summary>
        /// Returns bVector3.bgb swizzling (equivalent to bVector3.zyz).
        /// </summary>
        public bVector3 bgb => new bVector3(z, y, z);
        
        /// <summary>
        /// Returns bVector3.zyzx swizzling.
        /// </summary>
        public bVector4 zyzx => new bVector4(z, y, z, x);
        
        /// <summary>
        /// Returns bVector3.bgbr swizzling (equivalent to bVector3.zyzx).
        /// </summary>
        public bVector4 bgbr => new bVector4(z, y, z, x);
        
        /// <summary>
        /// Returns bVector3.zyzy swizzling.
        /// </summary>
        public bVector4 zyzy => new bVector4(z, y, z, y);
        
        /// <summary>
        /// Returns bVector3.bgbg swizzling (equivalent to bVector3.zyzy).
        /// </summary>
        public bVector4 bgbg => new bVector4(z, y, z, y);
        
        /// <summary>
        /// Returns bVector3.zyzz swizzling.
        /// </summary>
        public bVector4 zyzz => new bVector4(z, y, z, z);
        
        /// <summary>
        /// Returns bVector3.bgbb swizzling (equivalent to bVector3.zyzz).
        /// </summary>
        public bVector4 bgbb => new bVector4(z, y, z, z);
        
        /// <summary>
        /// Returns bVector3.zz swizzling.
        /// </summary>
        public bVector2 zz => new bVector2(z, z);
        
        /// <summary>
        /// Returns bVector3.bb swizzling (equivalent to bVector3.zz).
        /// </summary>
        public bVector2 bb => new bVector2(z, z);
        
        /// <summary>
        /// Returns bVector3.zzx swizzling.
        /// </summary>
        public bVector3 zzx => new bVector3(z, z, x);
        
        /// <summary>
        /// Returns bVector3.bbr swizzling (equivalent to bVector3.zzx).
        /// </summary>
        public bVector3 bbr => new bVector3(z, z, x);
        
        /// <summary>
        /// Returns bVector3.zzxx swizzling.
        /// </summary>
        public bVector4 zzxx => new bVector4(z, z, x, x);
        
        /// <summary>
        /// Returns bVector3.bbrr swizzling (equivalent to bVector3.zzxx).
        /// </summary>
        public bVector4 bbrr => new bVector4(z, z, x, x);
        
        /// <summary>
        /// Returns bVector3.zzxy swizzling.
        /// </summary>
        public bVector4 zzxy => new bVector4(z, z, x, y);
        
        /// <summary>
        /// Returns bVector3.bbrg swizzling (equivalent to bVector3.zzxy).
        /// </summary>
        public bVector4 bbrg => new bVector4(z, z, x, y);
        
        /// <summary>
        /// Returns bVector3.zzxz swizzling.
        /// </summary>
        public bVector4 zzxz => new bVector4(z, z, x, z);
        
        /// <summary>
        /// Returns bVector3.bbrb swizzling (equivalent to bVector3.zzxz).
        /// </summary>
        public bVector4 bbrb => new bVector4(z, z, x, z);
        
        /// <summary>
        /// Returns bVector3.zzy swizzling.
        /// </summary>
        public bVector3 zzy => new bVector3(z, z, y);
        
        /// <summary>
        /// Returns bVector3.bbg swizzling (equivalent to bVector3.zzy).
        /// </summary>
        public bVector3 bbg => new bVector3(z, z, y);
        
        /// <summary>
        /// Returns bVector3.zzyx swizzling.
        /// </summary>
        public bVector4 zzyx => new bVector4(z, z, y, x);
        
        /// <summary>
        /// Returns bVector3.bbgr swizzling (equivalent to bVector3.zzyx).
        /// </summary>
        public bVector4 bbgr => new bVector4(z, z, y, x);
        
        /// <summary>
        /// Returns bVector3.zzyy swizzling.
        /// </summary>
        public bVector4 zzyy => new bVector4(z, z, y, y);
        
        /// <summary>
        /// Returns bVector3.bbgg swizzling (equivalent to bVector3.zzyy).
        /// </summary>
        public bVector4 bbgg => new bVector4(z, z, y, y);
        
        /// <summary>
        /// Returns bVector3.zzyz swizzling.
        /// </summary>
        public bVector4 zzyz => new bVector4(z, z, y, z);
        
        /// <summary>
        /// Returns bVector3.bbgb swizzling (equivalent to bVector3.zzyz).
        /// </summary>
        public bVector4 bbgb => new bVector4(z, z, y, z);
        
        /// <summary>
        /// Returns bVector3.zzz swizzling.
        /// </summary>
        public bVector3 zzz => new bVector3(z, z, z);
        
        /// <summary>
        /// Returns bVector3.bbb swizzling (equivalent to bVector3.zzz).
        /// </summary>
        public bVector3 bbb => new bVector3(z, z, z);
        
        /// <summary>
        /// Returns bVector3.zzzx swizzling.
        /// </summary>
        public bVector4 zzzx => new bVector4(z, z, z, x);
        
        /// <summary>
        /// Returns bVector3.bbbr swizzling (equivalent to bVector3.zzzx).
        /// </summary>
        public bVector4 bbbr => new bVector4(z, z, z, x);
        
        /// <summary>
        /// Returns bVector3.zzzy swizzling.
        /// </summary>
        public bVector4 zzzy => new bVector4(z, z, z, y);
        
        /// <summary>
        /// Returns bVector3.bbbg swizzling (equivalent to bVector3.zzzy).
        /// </summary>
        public bVector4 bbbg => new bVector4(z, z, z, y);
        
        /// <summary>
        /// Returns bVector3.zzzz swizzling.
        /// </summary>
        public bVector4 zzzz => new bVector4(z, z, z, z);
        
        /// <summary>
        /// Returns bVector3.bbbb swizzling (equivalent to bVector3.zzzz).
        /// </summary>
        public bVector4 bbbb => new bVector4(z, z, z, z);

        #endregion

    }
}
