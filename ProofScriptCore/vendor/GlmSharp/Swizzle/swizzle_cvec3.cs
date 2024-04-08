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
    /// Temporary Vector of type Complex with 3 components, used for implementing swizzling for cVector3.
    /// </summary>
    /// 
    /*
    [RegisterCoreClassStruct]
   
    [StructLayout(LayoutKind.Sequential)]
    public struct swizzle_cVector3
    {

        #region Fields
        
        /// <summary>
        /// x-component
        /// </summary>
        
        internal readonly Complex x;
        
        /// <summary>
        /// y-component
        /// </summary>
        
        internal readonly Complex y;
        
        /// <summary>
        /// z-component
        /// </summary>
        
        internal readonly Complex z;

        #endregion


        #region Constructors
        
        /// <summary>
        /// Constructor for swizzle_cVector3.
        /// </summary>
        internal swizzle_cVector3(Complex x, Complex y, Complex z)
        {
            this.x = x;
            this.y = y;
            this.z = z;
        }

        #endregion


        #region Properties
        
        /// <summary>
        /// Returns cVector3.xx swizzling.
        /// </summary>
        public cVector2 xx => new cVector2(x, x);
        
        /// <summary>
        /// Returns cVector3.rr swizzling (equivalent to cVector3.xx).
        /// </summary>
        public cVector2 rr => new cVector2(x, x);
        
        /// <summary>
        /// Returns cVector3.xxx swizzling.
        /// </summary>
        public cVector3 xxx => new cVector3(x, x, x);
        
        /// <summary>
        /// Returns cVector3.rrr swizzling (equivalent to cVector3.xxx).
        /// </summary>
        public cVector3 rrr => new cVector3(x, x, x);
        
        /// <summary>
        /// Returns cVector3.xxxx swizzling.
        /// </summary>
        public cVector4 xxxx => new cVector4(x, x, x, x);
        
        /// <summary>
        /// Returns cVector3.rrrr swizzling (equivalent to cVector3.xxxx).
        /// </summary>
        public cVector4 rrrr => new cVector4(x, x, x, x);
        
        /// <summary>
        /// Returns cVector3.xxxy swizzling.
        /// </summary>
        public cVector4 xxxy => new cVector4(x, x, x, y);
        
        /// <summary>
        /// Returns cVector3.rrrg swizzling (equivalent to cVector3.xxxy).
        /// </summary>
        public cVector4 rrrg => new cVector4(x, x, x, y);
        
        /// <summary>
        /// Returns cVector3.xxxz swizzling.
        /// </summary>
        public cVector4 xxxz => new cVector4(x, x, x, z);
        
        /// <summary>
        /// Returns cVector3.rrrb swizzling (equivalent to cVector3.xxxz).
        /// </summary>
        public cVector4 rrrb => new cVector4(x, x, x, z);
        
        /// <summary>
        /// Returns cVector3.xxy swizzling.
        /// </summary>
        public cVector3 xxy => new cVector3(x, x, y);
        
        /// <summary>
        /// Returns cVector3.rrg swizzling (equivalent to cVector3.xxy).
        /// </summary>
        public cVector3 rrg => new cVector3(x, x, y);
        
        /// <summary>
        /// Returns cVector3.xxyx swizzling.
        /// </summary>
        public cVector4 xxyx => new cVector4(x, x, y, x);
        
        /// <summary>
        /// Returns cVector3.rrgr swizzling (equivalent to cVector3.xxyx).
        /// </summary>
        public cVector4 rrgr => new cVector4(x, x, y, x);
        
        /// <summary>
        /// Returns cVector3.xxyy swizzling.
        /// </summary>
        public cVector4 xxyy => new cVector4(x, x, y, y);
        
        /// <summary>
        /// Returns cVector3.rrgg swizzling (equivalent to cVector3.xxyy).
        /// </summary>
        public cVector4 rrgg => new cVector4(x, x, y, y);
        
        /// <summary>
        /// Returns cVector3.xxyz swizzling.
        /// </summary>
        public cVector4 xxyz => new cVector4(x, x, y, z);
        
        /// <summary>
        /// Returns cVector3.rrgb swizzling (equivalent to cVector3.xxyz).
        /// </summary>
        public cVector4 rrgb => new cVector4(x, x, y, z);
        
        /// <summary>
        /// Returns cVector3.xxz swizzling.
        /// </summary>
        public cVector3 xxz => new cVector3(x, x, z);
        
        /// <summary>
        /// Returns cVector3.rrb swizzling (equivalent to cVector3.xxz).
        /// </summary>
        public cVector3 rrb => new cVector3(x, x, z);
        
        /// <summary>
        /// Returns cVector3.xxzx swizzling.
        /// </summary>
        public cVector4 xxzx => new cVector4(x, x, z, x);
        
        /// <summary>
        /// Returns cVector3.rrbr swizzling (equivalent to cVector3.xxzx).
        /// </summary>
        public cVector4 rrbr => new cVector4(x, x, z, x);
        
        /// <summary>
        /// Returns cVector3.xxzy swizzling.
        /// </summary>
        public cVector4 xxzy => new cVector4(x, x, z, y);
        
        /// <summary>
        /// Returns cVector3.rrbg swizzling (equivalent to cVector3.xxzy).
        /// </summary>
        public cVector4 rrbg => new cVector4(x, x, z, y);
        
        /// <summary>
        /// Returns cVector3.xxzz swizzling.
        /// </summary>
        public cVector4 xxzz => new cVector4(x, x, z, z);
        
        /// <summary>
        /// Returns cVector3.rrbb swizzling (equivalent to cVector3.xxzz).
        /// </summary>
        public cVector4 rrbb => new cVector4(x, x, z, z);
        
        /// <summary>
        /// Returns cVector3.xy swizzling.
        /// </summary>
        public cVector2 xy => new cVector2(x, y);
        
        /// <summary>
        /// Returns cVector3.rg swizzling (equivalent to cVector3.xy).
        /// </summary>
        public cVector2 rg => new cVector2(x, y);
        
        /// <summary>
        /// Returns cVector3.xyx swizzling.
        /// </summary>
        public cVector3 xyx => new cVector3(x, y, x);
        
        /// <summary>
        /// Returns cVector3.rgr swizzling (equivalent to cVector3.xyx).
        /// </summary>
        public cVector3 rgr => new cVector3(x, y, x);
        
        /// <summary>
        /// Returns cVector3.xyxx swizzling.
        /// </summary>
        public cVector4 xyxx => new cVector4(x, y, x, x);
        
        /// <summary>
        /// Returns cVector3.rgrr swizzling (equivalent to cVector3.xyxx).
        /// </summary>
        public cVector4 rgrr => new cVector4(x, y, x, x);
        
        /// <summary>
        /// Returns cVector3.xyxy swizzling.
        /// </summary>
        public cVector4 xyxy => new cVector4(x, y, x, y);
        
        /// <summary>
        /// Returns cVector3.rgrg swizzling (equivalent to cVector3.xyxy).
        /// </summary>
        public cVector4 rgrg => new cVector4(x, y, x, y);
        
        /// <summary>
        /// Returns cVector3.xyxz swizzling.
        /// </summary>
        public cVector4 xyxz => new cVector4(x, y, x, z);
        
        /// <summary>
        /// Returns cVector3.rgrb swizzling (equivalent to cVector3.xyxz).
        /// </summary>
        public cVector4 rgrb => new cVector4(x, y, x, z);
        
        /// <summary>
        /// Returns cVector3.xyy swizzling.
        /// </summary>
        public cVector3 xyy => new cVector3(x, y, y);
        
        /// <summary>
        /// Returns cVector3.rgg swizzling (equivalent to cVector3.xyy).
        /// </summary>
        public cVector3 rgg => new cVector3(x, y, y);
        
        /// <summary>
        /// Returns cVector3.xyyx swizzling.
        /// </summary>
        public cVector4 xyyx => new cVector4(x, y, y, x);
        
        /// <summary>
        /// Returns cVector3.rggr swizzling (equivalent to cVector3.xyyx).
        /// </summary>
        public cVector4 rggr => new cVector4(x, y, y, x);
        
        /// <summary>
        /// Returns cVector3.xyyy swizzling.
        /// </summary>
        public cVector4 xyyy => new cVector4(x, y, y, y);
        
        /// <summary>
        /// Returns cVector3.rggg swizzling (equivalent to cVector3.xyyy).
        /// </summary>
        public cVector4 rggg => new cVector4(x, y, y, y);
        
        /// <summary>
        /// Returns cVector3.xyyz swizzling.
        /// </summary>
        public cVector4 xyyz => new cVector4(x, y, y, z);
        
        /// <summary>
        /// Returns cVector3.rggb swizzling (equivalent to cVector3.xyyz).
        /// </summary>
        public cVector4 rggb => new cVector4(x, y, y, z);
        
        /// <summary>
        /// Returns cVector3.xyz swizzling.
        /// </summary>
        public cVector3 xyz => new cVector3(x, y, z);
        
        /// <summary>
        /// Returns cVector3.rgb swizzling (equivalent to cVector3.xyz).
        /// </summary>
        public cVector3 rgb => new cVector3(x, y, z);
        
        /// <summary>
        /// Returns cVector3.xyzx swizzling.
        /// </summary>
        public cVector4 xyzx => new cVector4(x, y, z, x);
        
        /// <summary>
        /// Returns cVector3.rgbr swizzling (equivalent to cVector3.xyzx).
        /// </summary>
        public cVector4 rgbr => new cVector4(x, y, z, x);
        
        /// <summary>
        /// Returns cVector3.xyzy swizzling.
        /// </summary>
        public cVector4 xyzy => new cVector4(x, y, z, y);
        
        /// <summary>
        /// Returns cVector3.rgbg swizzling (equivalent to cVector3.xyzy).
        /// </summary>
        public cVector4 rgbg => new cVector4(x, y, z, y);
        
        /// <summary>
        /// Returns cVector3.xyzz swizzling.
        /// </summary>
        public cVector4 xyzz => new cVector4(x, y, z, z);
        
        /// <summary>
        /// Returns cVector3.rgbb swizzling (equivalent to cVector3.xyzz).
        /// </summary>
        public cVector4 rgbb => new cVector4(x, y, z, z);
        
        /// <summary>
        /// Returns cVector3.xz swizzling.
        /// </summary>
        public cVector2 xz => new cVector2(x, z);
        
        /// <summary>
        /// Returns cVector3.rb swizzling (equivalent to cVector3.xz).
        /// </summary>
        public cVector2 rb => new cVector2(x, z);
        
        /// <summary>
        /// Returns cVector3.xzx swizzling.
        /// </summary>
        public cVector3 xzx => new cVector3(x, z, x);
        
        /// <summary>
        /// Returns cVector3.rbr swizzling (equivalent to cVector3.xzx).
        /// </summary>
        public cVector3 rbr => new cVector3(x, z, x);
        
        /// <summary>
        /// Returns cVector3.xzxx swizzling.
        /// </summary>
        public cVector4 xzxx => new cVector4(x, z, x, x);
        
        /// <summary>
        /// Returns cVector3.rbrr swizzling (equivalent to cVector3.xzxx).
        /// </summary>
        public cVector4 rbrr => new cVector4(x, z, x, x);
        
        /// <summary>
        /// Returns cVector3.xzxy swizzling.
        /// </summary>
        public cVector4 xzxy => new cVector4(x, z, x, y);
        
        /// <summary>
        /// Returns cVector3.rbrg swizzling (equivalent to cVector3.xzxy).
        /// </summary>
        public cVector4 rbrg => new cVector4(x, z, x, y);
        
        /// <summary>
        /// Returns cVector3.xzxz swizzling.
        /// </summary>
        public cVector4 xzxz => new cVector4(x, z, x, z);
        
        /// <summary>
        /// Returns cVector3.rbrb swizzling (equivalent to cVector3.xzxz).
        /// </summary>
        public cVector4 rbrb => new cVector4(x, z, x, z);
        
        /// <summary>
        /// Returns cVector3.xzy swizzling.
        /// </summary>
        public cVector3 xzy => new cVector3(x, z, y);
        
        /// <summary>
        /// Returns cVector3.rbg swizzling (equivalent to cVector3.xzy).
        /// </summary>
        public cVector3 rbg => new cVector3(x, z, y);
        
        /// <summary>
        /// Returns cVector3.xzyx swizzling.
        /// </summary>
        public cVector4 xzyx => new cVector4(x, z, y, x);
        
        /// <summary>
        /// Returns cVector3.rbgr swizzling (equivalent to cVector3.xzyx).
        /// </summary>
        public cVector4 rbgr => new cVector4(x, z, y, x);
        
        /// <summary>
        /// Returns cVector3.xzyy swizzling.
        /// </summary>
        public cVector4 xzyy => new cVector4(x, z, y, y);
        
        /// <summary>
        /// Returns cVector3.rbgg swizzling (equivalent to cVector3.xzyy).
        /// </summary>
        public cVector4 rbgg => new cVector4(x, z, y, y);
        
        /// <summary>
        /// Returns cVector3.xzyz swizzling.
        /// </summary>
        public cVector4 xzyz => new cVector4(x, z, y, z);
        
        /// <summary>
        /// Returns cVector3.rbgb swizzling (equivalent to cVector3.xzyz).
        /// </summary>
        public cVector4 rbgb => new cVector4(x, z, y, z);
        
        /// <summary>
        /// Returns cVector3.xzz swizzling.
        /// </summary>
        public cVector3 xzz => new cVector3(x, z, z);
        
        /// <summary>
        /// Returns cVector3.rbb swizzling (equivalent to cVector3.xzz).
        /// </summary>
        public cVector3 rbb => new cVector3(x, z, z);
        
        /// <summary>
        /// Returns cVector3.xzzx swizzling.
        /// </summary>
        public cVector4 xzzx => new cVector4(x, z, z, x);
        
        /// <summary>
        /// Returns cVector3.rbbr swizzling (equivalent to cVector3.xzzx).
        /// </summary>
        public cVector4 rbbr => new cVector4(x, z, z, x);
        
        /// <summary>
        /// Returns cVector3.xzzy swizzling.
        /// </summary>
        public cVector4 xzzy => new cVector4(x, z, z, y);
        
        /// <summary>
        /// Returns cVector3.rbbg swizzling (equivalent to cVector3.xzzy).
        /// </summary>
        public cVector4 rbbg => new cVector4(x, z, z, y);
        
        /// <summary>
        /// Returns cVector3.xzzz swizzling.
        /// </summary>
        public cVector4 xzzz => new cVector4(x, z, z, z);
        
        /// <summary>
        /// Returns cVector3.rbbb swizzling (equivalent to cVector3.xzzz).
        /// </summary>
        public cVector4 rbbb => new cVector4(x, z, z, z);
        
        /// <summary>
        /// Returns cVector3.yx swizzling.
        /// </summary>
        public cVector2 yx => new cVector2(y, x);
        
        /// <summary>
        /// Returns cVector3.gr swizzling (equivalent to cVector3.yx).
        /// </summary>
        public cVector2 gr => new cVector2(y, x);
        
        /// <summary>
        /// Returns cVector3.yxx swizzling.
        /// </summary>
        public cVector3 yxx => new cVector3(y, x, x);
        
        /// <summary>
        /// Returns cVector3.grr swizzling (equivalent to cVector3.yxx).
        /// </summary>
        public cVector3 grr => new cVector3(y, x, x);
        
        /// <summary>
        /// Returns cVector3.yxxx swizzling.
        /// </summary>
        public cVector4 yxxx => new cVector4(y, x, x, x);
        
        /// <summary>
        /// Returns cVector3.grrr swizzling (equivalent to cVector3.yxxx).
        /// </summary>
        public cVector4 grrr => new cVector4(y, x, x, x);
        
        /// <summary>
        /// Returns cVector3.yxxy swizzling.
        /// </summary>
        public cVector4 yxxy => new cVector4(y, x, x, y);
        
        /// <summary>
        /// Returns cVector3.grrg swizzling (equivalent to cVector3.yxxy).
        /// </summary>
        public cVector4 grrg => new cVector4(y, x, x, y);
        
        /// <summary>
        /// Returns cVector3.yxxz swizzling.
        /// </summary>
        public cVector4 yxxz => new cVector4(y, x, x, z);
        
        /// <summary>
        /// Returns cVector3.grrb swizzling (equivalent to cVector3.yxxz).
        /// </summary>
        public cVector4 grrb => new cVector4(y, x, x, z);
        
        /// <summary>
        /// Returns cVector3.yxy swizzling.
        /// </summary>
        public cVector3 yxy => new cVector3(y, x, y);
        
        /// <summary>
        /// Returns cVector3.grg swizzling (equivalent to cVector3.yxy).
        /// </summary>
        public cVector3 grg => new cVector3(y, x, y);
        
        /// <summary>
        /// Returns cVector3.yxyx swizzling.
        /// </summary>
        public cVector4 yxyx => new cVector4(y, x, y, x);
        
        /// <summary>
        /// Returns cVector3.grgr swizzling (equivalent to cVector3.yxyx).
        /// </summary>
        public cVector4 grgr => new cVector4(y, x, y, x);
        
        /// <summary>
        /// Returns cVector3.yxyy swizzling.
        /// </summary>
        public cVector4 yxyy => new cVector4(y, x, y, y);
        
        /// <summary>
        /// Returns cVector3.grgg swizzling (equivalent to cVector3.yxyy).
        /// </summary>
        public cVector4 grgg => new cVector4(y, x, y, y);
        
        /// <summary>
        /// Returns cVector3.yxyz swizzling.
        /// </summary>
        public cVector4 yxyz => new cVector4(y, x, y, z);
        
        /// <summary>
        /// Returns cVector3.grgb swizzling (equivalent to cVector3.yxyz).
        /// </summary>
        public cVector4 grgb => new cVector4(y, x, y, z);
        
        /// <summary>
        /// Returns cVector3.yxz swizzling.
        /// </summary>
        public cVector3 yxz => new cVector3(y, x, z);
        
        /// <summary>
        /// Returns cVector3.grb swizzling (equivalent to cVector3.yxz).
        /// </summary>
        public cVector3 grb => new cVector3(y, x, z);
        
        /// <summary>
        /// Returns cVector3.yxzx swizzling.
        /// </summary>
        public cVector4 yxzx => new cVector4(y, x, z, x);
        
        /// <summary>
        /// Returns cVector3.grbr swizzling (equivalent to cVector3.yxzx).
        /// </summary>
        public cVector4 grbr => new cVector4(y, x, z, x);
        
        /// <summary>
        /// Returns cVector3.yxzy swizzling.
        /// </summary>
        public cVector4 yxzy => new cVector4(y, x, z, y);
        
        /// <summary>
        /// Returns cVector3.grbg swizzling (equivalent to cVector3.yxzy).
        /// </summary>
        public cVector4 grbg => new cVector4(y, x, z, y);
        
        /// <summary>
        /// Returns cVector3.yxzz swizzling.
        /// </summary>
        public cVector4 yxzz => new cVector4(y, x, z, z);
        
        /// <summary>
        /// Returns cVector3.grbb swizzling (equivalent to cVector3.yxzz).
        /// </summary>
        public cVector4 grbb => new cVector4(y, x, z, z);
        
        /// <summary>
        /// Returns cVector3.yy swizzling.
        /// </summary>
        public cVector2 yy => new cVector2(y, y);
        
        /// <summary>
        /// Returns cVector3.gg swizzling (equivalent to cVector3.yy).
        /// </summary>
        public cVector2 gg => new cVector2(y, y);
        
        /// <summary>
        /// Returns cVector3.yyx swizzling.
        /// </summary>
        public cVector3 yyx => new cVector3(y, y, x);
        
        /// <summary>
        /// Returns cVector3.ggr swizzling (equivalent to cVector3.yyx).
        /// </summary>
        public cVector3 ggr => new cVector3(y, y, x);
        
        /// <summary>
        /// Returns cVector3.yyxx swizzling.
        /// </summary>
        public cVector4 yyxx => new cVector4(y, y, x, x);
        
        /// <summary>
        /// Returns cVector3.ggrr swizzling (equivalent to cVector3.yyxx).
        /// </summary>
        public cVector4 ggrr => new cVector4(y, y, x, x);
        
        /// <summary>
        /// Returns cVector3.yyxy swizzling.
        /// </summary>
        public cVector4 yyxy => new cVector4(y, y, x, y);
        
        /// <summary>
        /// Returns cVector3.ggrg swizzling (equivalent to cVector3.yyxy).
        /// </summary>
        public cVector4 ggrg => new cVector4(y, y, x, y);
        
        /// <summary>
        /// Returns cVector3.yyxz swizzling.
        /// </summary>
        public cVector4 yyxz => new cVector4(y, y, x, z);
        
        /// <summary>
        /// Returns cVector3.ggrb swizzling (equivalent to cVector3.yyxz).
        /// </summary>
        public cVector4 ggrb => new cVector4(y, y, x, z);
        
        /// <summary>
        /// Returns cVector3.yyy swizzling.
        /// </summary>
        public cVector3 yyy => new cVector3(y, y, y);
        
        /// <summary>
        /// Returns cVector3.ggg swizzling (equivalent to cVector3.yyy).
        /// </summary>
        public cVector3 ggg => new cVector3(y, y, y);
        
        /// <summary>
        /// Returns cVector3.yyyx swizzling.
        /// </summary>
        public cVector4 yyyx => new cVector4(y, y, y, x);
        
        /// <summary>
        /// Returns cVector3.gggr swizzling (equivalent to cVector3.yyyx).
        /// </summary>
        public cVector4 gggr => new cVector4(y, y, y, x);
        
        /// <summary>
        /// Returns cVector3.yyyy swizzling.
        /// </summary>
        public cVector4 yyyy => new cVector4(y, y, y, y);
        
        /// <summary>
        /// Returns cVector3.gggg swizzling (equivalent to cVector3.yyyy).
        /// </summary>
        public cVector4 gggg => new cVector4(y, y, y, y);
        
        /// <summary>
        /// Returns cVector3.yyyz swizzling.
        /// </summary>
        public cVector4 yyyz => new cVector4(y, y, y, z);
        
        /// <summary>
        /// Returns cVector3.gggb swizzling (equivalent to cVector3.yyyz).
        /// </summary>
        public cVector4 gggb => new cVector4(y, y, y, z);
        
        /// <summary>
        /// Returns cVector3.yyz swizzling.
        /// </summary>
        public cVector3 yyz => new cVector3(y, y, z);
        
        /// <summary>
        /// Returns cVector3.ggb swizzling (equivalent to cVector3.yyz).
        /// </summary>
        public cVector3 ggb => new cVector3(y, y, z);
        
        /// <summary>
        /// Returns cVector3.yyzx swizzling.
        /// </summary>
        public cVector4 yyzx => new cVector4(y, y, z, x);
        
        /// <summary>
        /// Returns cVector3.ggbr swizzling (equivalent to cVector3.yyzx).
        /// </summary>
        public cVector4 ggbr => new cVector4(y, y, z, x);
        
        /// <summary>
        /// Returns cVector3.yyzy swizzling.
        /// </summary>
        public cVector4 yyzy => new cVector4(y, y, z, y);
        
        /// <summary>
        /// Returns cVector3.ggbg swizzling (equivalent to cVector3.yyzy).
        /// </summary>
        public cVector4 ggbg => new cVector4(y, y, z, y);
        
        /// <summary>
        /// Returns cVector3.yyzz swizzling.
        /// </summary>
        public cVector4 yyzz => new cVector4(y, y, z, z);
        
        /// <summary>
        /// Returns cVector3.ggbb swizzling (equivalent to cVector3.yyzz).
        /// </summary>
        public cVector4 ggbb => new cVector4(y, y, z, z);
        
        /// <summary>
        /// Returns cVector3.yz swizzling.
        /// </summary>
        public cVector2 yz => new cVector2(y, z);
        
        /// <summary>
        /// Returns cVector3.gb swizzling (equivalent to cVector3.yz).
        /// </summary>
        public cVector2 gb => new cVector2(y, z);
        
        /// <summary>
        /// Returns cVector3.yzx swizzling.
        /// </summary>
        public cVector3 yzx => new cVector3(y, z, x);
        
        /// <summary>
        /// Returns cVector3.gbr swizzling (equivalent to cVector3.yzx).
        /// </summary>
        public cVector3 gbr => new cVector3(y, z, x);
        
        /// <summary>
        /// Returns cVector3.yzxx swizzling.
        /// </summary>
        public cVector4 yzxx => new cVector4(y, z, x, x);
        
        /// <summary>
        /// Returns cVector3.gbrr swizzling (equivalent to cVector3.yzxx).
        /// </summary>
        public cVector4 gbrr => new cVector4(y, z, x, x);
        
        /// <summary>
        /// Returns cVector3.yzxy swizzling.
        /// </summary>
        public cVector4 yzxy => new cVector4(y, z, x, y);
        
        /// <summary>
        /// Returns cVector3.gbrg swizzling (equivalent to cVector3.yzxy).
        /// </summary>
        public cVector4 gbrg => new cVector4(y, z, x, y);
        
        /// <summary>
        /// Returns cVector3.yzxz swizzling.
        /// </summary>
        public cVector4 yzxz => new cVector4(y, z, x, z);
        
        /// <summary>
        /// Returns cVector3.gbrb swizzling (equivalent to cVector3.yzxz).
        /// </summary>
        public cVector4 gbrb => new cVector4(y, z, x, z);
        
        /// <summary>
        /// Returns cVector3.yzy swizzling.
        /// </summary>
        public cVector3 yzy => new cVector3(y, z, y);
        
        /// <summary>
        /// Returns cVector3.gbg swizzling (equivalent to cVector3.yzy).
        /// </summary>
        public cVector3 gbg => new cVector3(y, z, y);
        
        /// <summary>
        /// Returns cVector3.yzyx swizzling.
        /// </summary>
        public cVector4 yzyx => new cVector4(y, z, y, x);
        
        /// <summary>
        /// Returns cVector3.gbgr swizzling (equivalent to cVector3.yzyx).
        /// </summary>
        public cVector4 gbgr => new cVector4(y, z, y, x);
        
        /// <summary>
        /// Returns cVector3.yzyy swizzling.
        /// </summary>
        public cVector4 yzyy => new cVector4(y, z, y, y);
        
        /// <summary>
        /// Returns cVector3.gbgg swizzling (equivalent to cVector3.yzyy).
        /// </summary>
        public cVector4 gbgg => new cVector4(y, z, y, y);
        
        /// <summary>
        /// Returns cVector3.yzyz swizzling.
        /// </summary>
        public cVector4 yzyz => new cVector4(y, z, y, z);
        
        /// <summary>
        /// Returns cVector3.gbgb swizzling (equivalent to cVector3.yzyz).
        /// </summary>
        public cVector4 gbgb => new cVector4(y, z, y, z);
        
        /// <summary>
        /// Returns cVector3.yzz swizzling.
        /// </summary>
        public cVector3 yzz => new cVector3(y, z, z);
        
        /// <summary>
        /// Returns cVector3.gbb swizzling (equivalent to cVector3.yzz).
        /// </summary>
        public cVector3 gbb => new cVector3(y, z, z);
        
        /// <summary>
        /// Returns cVector3.yzzx swizzling.
        /// </summary>
        public cVector4 yzzx => new cVector4(y, z, z, x);
        
        /// <summary>
        /// Returns cVector3.gbbr swizzling (equivalent to cVector3.yzzx).
        /// </summary>
        public cVector4 gbbr => new cVector4(y, z, z, x);
        
        /// <summary>
        /// Returns cVector3.yzzy swizzling.
        /// </summary>
        public cVector4 yzzy => new cVector4(y, z, z, y);
        
        /// <summary>
        /// Returns cVector3.gbbg swizzling (equivalent to cVector3.yzzy).
        /// </summary>
        public cVector4 gbbg => new cVector4(y, z, z, y);
        
        /// <summary>
        /// Returns cVector3.yzzz swizzling.
        /// </summary>
        public cVector4 yzzz => new cVector4(y, z, z, z);
        
        /// <summary>
        /// Returns cVector3.gbbb swizzling (equivalent to cVector3.yzzz).
        /// </summary>
        public cVector4 gbbb => new cVector4(y, z, z, z);
        
        /// <summary>
        /// Returns cVector3.zx swizzling.
        /// </summary>
        public cVector2 zx => new cVector2(z, x);
        
        /// <summary>
        /// Returns cVector3.br swizzling (equivalent to cVector3.zx).
        /// </summary>
        public cVector2 br => new cVector2(z, x);
        
        /// <summary>
        /// Returns cVector3.zxx swizzling.
        /// </summary>
        public cVector3 zxx => new cVector3(z, x, x);
        
        /// <summary>
        /// Returns cVector3.brr swizzling (equivalent to cVector3.zxx).
        /// </summary>
        public cVector3 brr => new cVector3(z, x, x);
        
        /// <summary>
        /// Returns cVector3.zxxx swizzling.
        /// </summary>
        public cVector4 zxxx => new cVector4(z, x, x, x);
        
        /// <summary>
        /// Returns cVector3.brrr swizzling (equivalent to cVector3.zxxx).
        /// </summary>
        public cVector4 brrr => new cVector4(z, x, x, x);
        
        /// <summary>
        /// Returns cVector3.zxxy swizzling.
        /// </summary>
        public cVector4 zxxy => new cVector4(z, x, x, y);
        
        /// <summary>
        /// Returns cVector3.brrg swizzling (equivalent to cVector3.zxxy).
        /// </summary>
        public cVector4 brrg => new cVector4(z, x, x, y);
        
        /// <summary>
        /// Returns cVector3.zxxz swizzling.
        /// </summary>
        public cVector4 zxxz => new cVector4(z, x, x, z);
        
        /// <summary>
        /// Returns cVector3.brrb swizzling (equivalent to cVector3.zxxz).
        /// </summary>
        public cVector4 brrb => new cVector4(z, x, x, z);
        
        /// <summary>
        /// Returns cVector3.zxy swizzling.
        /// </summary>
        public cVector3 zxy => new cVector3(z, x, y);
        
        /// <summary>
        /// Returns cVector3.brg swizzling (equivalent to cVector3.zxy).
        /// </summary>
        public cVector3 brg => new cVector3(z, x, y);
        
        /// <summary>
        /// Returns cVector3.zxyx swizzling.
        /// </summary>
        public cVector4 zxyx => new cVector4(z, x, y, x);
        
        /// <summary>
        /// Returns cVector3.brgr swizzling (equivalent to cVector3.zxyx).
        /// </summary>
        public cVector4 brgr => new cVector4(z, x, y, x);
        
        /// <summary>
        /// Returns cVector3.zxyy swizzling.
        /// </summary>
        public cVector4 zxyy => new cVector4(z, x, y, y);
        
        /// <summary>
        /// Returns cVector3.brgg swizzling (equivalent to cVector3.zxyy).
        /// </summary>
        public cVector4 brgg => new cVector4(z, x, y, y);
        
        /// <summary>
        /// Returns cVector3.zxyz swizzling.
        /// </summary>
        public cVector4 zxyz => new cVector4(z, x, y, z);
        
        /// <summary>
        /// Returns cVector3.brgb swizzling (equivalent to cVector3.zxyz).
        /// </summary>
        public cVector4 brgb => new cVector4(z, x, y, z);
        
        /// <summary>
        /// Returns cVector3.zxz swizzling.
        /// </summary>
        public cVector3 zxz => new cVector3(z, x, z);
        
        /// <summary>
        /// Returns cVector3.brb swizzling (equivalent to cVector3.zxz).
        /// </summary>
        public cVector3 brb => new cVector3(z, x, z);
        
        /// <summary>
        /// Returns cVector3.zxzx swizzling.
        /// </summary>
        public cVector4 zxzx => new cVector4(z, x, z, x);
        
        /// <summary>
        /// Returns cVector3.brbr swizzling (equivalent to cVector3.zxzx).
        /// </summary>
        public cVector4 brbr => new cVector4(z, x, z, x);
        
        /// <summary>
        /// Returns cVector3.zxzy swizzling.
        /// </summary>
        public cVector4 zxzy => new cVector4(z, x, z, y);
        
        /// <summary>
        /// Returns cVector3.brbg swizzling (equivalent to cVector3.zxzy).
        /// </summary>
        public cVector4 brbg => new cVector4(z, x, z, y);
        
        /// <summary>
        /// Returns cVector3.zxzz swizzling.
        /// </summary>
        public cVector4 zxzz => new cVector4(z, x, z, z);
        
        /// <summary>
        /// Returns cVector3.brbb swizzling (equivalent to cVector3.zxzz).
        /// </summary>
        public cVector4 brbb => new cVector4(z, x, z, z);
        
        /// <summary>
        /// Returns cVector3.zy swizzling.
        /// </summary>
        public cVector2 zy => new cVector2(z, y);
        
        /// <summary>
        /// Returns cVector3.bg swizzling (equivalent to cVector3.zy).
        /// </summary>
        public cVector2 bg => new cVector2(z, y);
        
        /// <summary>
        /// Returns cVector3.zyx swizzling.
        /// </summary>
        public cVector3 zyx => new cVector3(z, y, x);
        
        /// <summary>
        /// Returns cVector3.bgr swizzling (equivalent to cVector3.zyx).
        /// </summary>
        public cVector3 bgr => new cVector3(z, y, x);
        
        /// <summary>
        /// Returns cVector3.zyxx swizzling.
        /// </summary>
        public cVector4 zyxx => new cVector4(z, y, x, x);
        
        /// <summary>
        /// Returns cVector3.bgrr swizzling (equivalent to cVector3.zyxx).
        /// </summary>
        public cVector4 bgrr => new cVector4(z, y, x, x);
        
        /// <summary>
        /// Returns cVector3.zyxy swizzling.
        /// </summary>
        public cVector4 zyxy => new cVector4(z, y, x, y);
        
        /// <summary>
        /// Returns cVector3.bgrg swizzling (equivalent to cVector3.zyxy).
        /// </summary>
        public cVector4 bgrg => new cVector4(z, y, x, y);
        
        /// <summary>
        /// Returns cVector3.zyxz swizzling.
        /// </summary>
        public cVector4 zyxz => new cVector4(z, y, x, z);
        
        /// <summary>
        /// Returns cVector3.bgrb swizzling (equivalent to cVector3.zyxz).
        /// </summary>
        public cVector4 bgrb => new cVector4(z, y, x, z);
        
        /// <summary>
        /// Returns cVector3.zyy swizzling.
        /// </summary>
        public cVector3 zyy => new cVector3(z, y, y);
        
        /// <summary>
        /// Returns cVector3.bgg swizzling (equivalent to cVector3.zyy).
        /// </summary>
        public cVector3 bgg => new cVector3(z, y, y);
        
        /// <summary>
        /// Returns cVector3.zyyx swizzling.
        /// </summary>
        public cVector4 zyyx => new cVector4(z, y, y, x);
        
        /// <summary>
        /// Returns cVector3.bggr swizzling (equivalent to cVector3.zyyx).
        /// </summary>
        public cVector4 bggr => new cVector4(z, y, y, x);
        
        /// <summary>
        /// Returns cVector3.zyyy swizzling.
        /// </summary>
        public cVector4 zyyy => new cVector4(z, y, y, y);
        
        /// <summary>
        /// Returns cVector3.bggg swizzling (equivalent to cVector3.zyyy).
        /// </summary>
        public cVector4 bggg => new cVector4(z, y, y, y);
        
        /// <summary>
        /// Returns cVector3.zyyz swizzling.
        /// </summary>
        public cVector4 zyyz => new cVector4(z, y, y, z);
        
        /// <summary>
        /// Returns cVector3.bggb swizzling (equivalent to cVector3.zyyz).
        /// </summary>
        public cVector4 bggb => new cVector4(z, y, y, z);
        
        /// <summary>
        /// Returns cVector3.zyz swizzling.
        /// </summary>
        public cVector3 zyz => new cVector3(z, y, z);
        
        /// <summary>
        /// Returns cVector3.bgb swizzling (equivalent to cVector3.zyz).
        /// </summary>
        public cVector3 bgb => new cVector3(z, y, z);
        
        /// <summary>
        /// Returns cVector3.zyzx swizzling.
        /// </summary>
        public cVector4 zyzx => new cVector4(z, y, z, x);
        
        /// <summary>
        /// Returns cVector3.bgbr swizzling (equivalent to cVector3.zyzx).
        /// </summary>
        public cVector4 bgbr => new cVector4(z, y, z, x);
        
        /// <summary>
        /// Returns cVector3.zyzy swizzling.
        /// </summary>
        public cVector4 zyzy => new cVector4(z, y, z, y);
        
        /// <summary>
        /// Returns cVector3.bgbg swizzling (equivalent to cVector3.zyzy).
        /// </summary>
        public cVector4 bgbg => new cVector4(z, y, z, y);
        
        /// <summary>
        /// Returns cVector3.zyzz swizzling.
        /// </summary>
        public cVector4 zyzz => new cVector4(z, y, z, z);
        
        /// <summary>
        /// Returns cVector3.bgbb swizzling (equivalent to cVector3.zyzz).
        /// </summary>
        public cVector4 bgbb => new cVector4(z, y, z, z);
        
        /// <summary>
        /// Returns cVector3.zz swizzling.
        /// </summary>
        public cVector2 zz => new cVector2(z, z);
        
        /// <summary>
        /// Returns cVector3.bb swizzling (equivalent to cVector3.zz).
        /// </summary>
        public cVector2 bb => new cVector2(z, z);
        
        /// <summary>
        /// Returns cVector3.zzx swizzling.
        /// </summary>
        public cVector3 zzx => new cVector3(z, z, x);
        
        /// <summary>
        /// Returns cVector3.bbr swizzling (equivalent to cVector3.zzx).
        /// </summary>
        public cVector3 bbr => new cVector3(z, z, x);
        
        /// <summary>
        /// Returns cVector3.zzxx swizzling.
        /// </summary>
        public cVector4 zzxx => new cVector4(z, z, x, x);
        
        /// <summary>
        /// Returns cVector3.bbrr swizzling (equivalent to cVector3.zzxx).
        /// </summary>
        public cVector4 bbrr => new cVector4(z, z, x, x);
        
        /// <summary>
        /// Returns cVector3.zzxy swizzling.
        /// </summary>
        public cVector4 zzxy => new cVector4(z, z, x, y);
        
        /// <summary>
        /// Returns cVector3.bbrg swizzling (equivalent to cVector3.zzxy).
        /// </summary>
        public cVector4 bbrg => new cVector4(z, z, x, y);
        
        /// <summary>
        /// Returns cVector3.zzxz swizzling.
        /// </summary>
        public cVector4 zzxz => new cVector4(z, z, x, z);
        
        /// <summary>
        /// Returns cVector3.bbrb swizzling (equivalent to cVector3.zzxz).
        /// </summary>
        public cVector4 bbrb => new cVector4(z, z, x, z);
        
        /// <summary>
        /// Returns cVector3.zzy swizzling.
        /// </summary>
        public cVector3 zzy => new cVector3(z, z, y);
        
        /// <summary>
        /// Returns cVector3.bbg swizzling (equivalent to cVector3.zzy).
        /// </summary>
        public cVector3 bbg => new cVector3(z, z, y);
        
        /// <summary>
        /// Returns cVector3.zzyx swizzling.
        /// </summary>
        public cVector4 zzyx => new cVector4(z, z, y, x);
        
        /// <summary>
        /// Returns cVector3.bbgr swizzling (equivalent to cVector3.zzyx).
        /// </summary>
        public cVector4 bbgr => new cVector4(z, z, y, x);
        
        /// <summary>
        /// Returns cVector3.zzyy swizzling.
        /// </summary>
        public cVector4 zzyy => new cVector4(z, z, y, y);
        
        /// <summary>
        /// Returns cVector3.bbgg swizzling (equivalent to cVector3.zzyy).
        /// </summary>
        public cVector4 bbgg => new cVector4(z, z, y, y);
        
        /// <summary>
        /// Returns cVector3.zzyz swizzling.
        /// </summary>
        public cVector4 zzyz => new cVector4(z, z, y, z);
        
        /// <summary>
        /// Returns cVector3.bbgb swizzling (equivalent to cVector3.zzyz).
        /// </summary>
        public cVector4 bbgb => new cVector4(z, z, y, z);
        
        /// <summary>
        /// Returns cVector3.zzz swizzling.
        /// </summary>
        public cVector3 zzz => new cVector3(z, z, z);
        
        /// <summary>
        /// Returns cVector3.bbb swizzling (equivalent to cVector3.zzz).
        /// </summary>
        public cVector3 bbb => new cVector3(z, z, z);
        
        /// <summary>
        /// Returns cVector3.zzzx swizzling.
        /// </summary>
        public cVector4 zzzx => new cVector4(z, z, z, x);
        
        /// <summary>
        /// Returns cVector3.bbbr swizzling (equivalent to cVector3.zzzx).
        /// </summary>
        public cVector4 bbbr => new cVector4(z, z, z, x);
        
        /// <summary>
        /// Returns cVector3.zzzy swizzling.
        /// </summary>
        public cVector4 zzzy => new cVector4(z, z, z, y);
        
        /// <summary>
        /// Returns cVector3.bbbg swizzling (equivalent to cVector3.zzzy).
        /// </summary>
        public cVector4 bbbg => new cVector4(z, z, z, y);
        
        /// <summary>
        /// Returns cVector3.zzzz swizzling.
        /// </summary>
        public cVector4 zzzz => new cVector4(z, z, z, z);
        
        /// <summary>
        /// Returns cVector3.bbbb swizzling (equivalent to cVector3.zzzz).
        /// </summary>
        public cVector4 bbbb => new cVector4(z, z, z, z);

        #endregion

    }
    */
}
