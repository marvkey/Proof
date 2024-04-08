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
    /// Temporary Vector of type int with 3 components, used for implementing swizzling for iVector3.
    /// </summary>
    [RegisterCoreClassStruct]
   
    [StructLayout(LayoutKind.Sequential)]
    public struct swizzle_iVector3
    {

        #region Fields
        
        /// <summary>
        /// x-component
        /// </summary>
        
        internal readonly int x;
        
        /// <summary>
        /// y-component
        /// </summary>
        
        internal readonly int y;
        
        /// <summary>
        /// z-component
        /// </summary>
        
        internal readonly int z;

        #endregion


        #region Constructors
        
        /// <summary>
        /// Constructor for swizzle_iVector3.
        /// </summary>
        internal swizzle_iVector3(int x, int y, int z)
        {
            this.x = x;
            this.y = y;
            this.z = z;
        }

        #endregion


        #region Properties
        
        /// <summary>
        /// Returns iVector3.xx swizzling.
        /// </summary>
        public iVector2 xx => new iVector2(x, x);
        
        /// <summary>
        /// Returns iVector3.rr swizzling (equivalent to iVector3.xx).
        /// </summary>
        public iVector2 rr => new iVector2(x, x);
        
        /// <summary>
        /// Returns iVector3.xxx swizzling.
        /// </summary>
        public iVector3 xxx => new iVector3(x, x, x);
        
        /// <summary>
        /// Returns iVector3.rrr swizzling (equivalent to iVector3.xxx).
        /// </summary>
        public iVector3 rrr => new iVector3(x, x, x);
        
        /// <summary>
        /// Returns iVector3.xxxx swizzling.
        /// </summary>
        public iVector4 xxxx => new iVector4(x, x, x, x);
        
        /// <summary>
        /// Returns iVector3.rrrr swizzling (equivalent to iVector3.xxxx).
        /// </summary>
        public iVector4 rrrr => new iVector4(x, x, x, x);
        
        /// <summary>
        /// Returns iVector3.xxxy swizzling.
        /// </summary>
        public iVector4 xxxy => new iVector4(x, x, x, y);
        
        /// <summary>
        /// Returns iVector3.rrrg swizzling (equivalent to iVector3.xxxy).
        /// </summary>
        public iVector4 rrrg => new iVector4(x, x, x, y);
        
        /// <summary>
        /// Returns iVector3.xxxz swizzling.
        /// </summary>
        public iVector4 xxxz => new iVector4(x, x, x, z);
        
        /// <summary>
        /// Returns iVector3.rrrb swizzling (equivalent to iVector3.xxxz).
        /// </summary>
        public iVector4 rrrb => new iVector4(x, x, x, z);
        
        /// <summary>
        /// Returns iVector3.xxy swizzling.
        /// </summary>
        public iVector3 xxy => new iVector3(x, x, y);
        
        /// <summary>
        /// Returns iVector3.rrg swizzling (equivalent to iVector3.xxy).
        /// </summary>
        public iVector3 rrg => new iVector3(x, x, y);
        
        /// <summary>
        /// Returns iVector3.xxyx swizzling.
        /// </summary>
        public iVector4 xxyx => new iVector4(x, x, y, x);
        
        /// <summary>
        /// Returns iVector3.rrgr swizzling (equivalent to iVector3.xxyx).
        /// </summary>
        public iVector4 rrgr => new iVector4(x, x, y, x);
        
        /// <summary>
        /// Returns iVector3.xxyy swizzling.
        /// </summary>
        public iVector4 xxyy => new iVector4(x, x, y, y);
        
        /// <summary>
        /// Returns iVector3.rrgg swizzling (equivalent to iVector3.xxyy).
        /// </summary>
        public iVector4 rrgg => new iVector4(x, x, y, y);
        
        /// <summary>
        /// Returns iVector3.xxyz swizzling.
        /// </summary>
        public iVector4 xxyz => new iVector4(x, x, y, z);
        
        /// <summary>
        /// Returns iVector3.rrgb swizzling (equivalent to iVector3.xxyz).
        /// </summary>
        public iVector4 rrgb => new iVector4(x, x, y, z);
        
        /// <summary>
        /// Returns iVector3.xxz swizzling.
        /// </summary>
        public iVector3 xxz => new iVector3(x, x, z);
        
        /// <summary>
        /// Returns iVector3.rrb swizzling (equivalent to iVector3.xxz).
        /// </summary>
        public iVector3 rrb => new iVector3(x, x, z);
        
        /// <summary>
        /// Returns iVector3.xxzx swizzling.
        /// </summary>
        public iVector4 xxzx => new iVector4(x, x, z, x);
        
        /// <summary>
        /// Returns iVector3.rrbr swizzling (equivalent to iVector3.xxzx).
        /// </summary>
        public iVector4 rrbr => new iVector4(x, x, z, x);
        
        /// <summary>
        /// Returns iVector3.xxzy swizzling.
        /// </summary>
        public iVector4 xxzy => new iVector4(x, x, z, y);
        
        /// <summary>
        /// Returns iVector3.rrbg swizzling (equivalent to iVector3.xxzy).
        /// </summary>
        public iVector4 rrbg => new iVector4(x, x, z, y);
        
        /// <summary>
        /// Returns iVector3.xxzz swizzling.
        /// </summary>
        public iVector4 xxzz => new iVector4(x, x, z, z);
        
        /// <summary>
        /// Returns iVector3.rrbb swizzling (equivalent to iVector3.xxzz).
        /// </summary>
        public iVector4 rrbb => new iVector4(x, x, z, z);
        
        /// <summary>
        /// Returns iVector3.xy swizzling.
        /// </summary>
        public iVector2 xy => new iVector2(x, y);
        
        /// <summary>
        /// Returns iVector3.rg swizzling (equivalent to iVector3.xy).
        /// </summary>
        public iVector2 rg => new iVector2(x, y);
        
        /// <summary>
        /// Returns iVector3.xyx swizzling.
        /// </summary>
        public iVector3 xyx => new iVector3(x, y, x);
        
        /// <summary>
        /// Returns iVector3.rgr swizzling (equivalent to iVector3.xyx).
        /// </summary>
        public iVector3 rgr => new iVector3(x, y, x);
        
        /// <summary>
        /// Returns iVector3.xyxx swizzling.
        /// </summary>
        public iVector4 xyxx => new iVector4(x, y, x, x);
        
        /// <summary>
        /// Returns iVector3.rgrr swizzling (equivalent to iVector3.xyxx).
        /// </summary>
        public iVector4 rgrr => new iVector4(x, y, x, x);
        
        /// <summary>
        /// Returns iVector3.xyxy swizzling.
        /// </summary>
        public iVector4 xyxy => new iVector4(x, y, x, y);
        
        /// <summary>
        /// Returns iVector3.rgrg swizzling (equivalent to iVector3.xyxy).
        /// </summary>
        public iVector4 rgrg => new iVector4(x, y, x, y);
        
        /// <summary>
        /// Returns iVector3.xyxz swizzling.
        /// </summary>
        public iVector4 xyxz => new iVector4(x, y, x, z);
        
        /// <summary>
        /// Returns iVector3.rgrb swizzling (equivalent to iVector3.xyxz).
        /// </summary>
        public iVector4 rgrb => new iVector4(x, y, x, z);
        
        /// <summary>
        /// Returns iVector3.xyy swizzling.
        /// </summary>
        public iVector3 xyy => new iVector3(x, y, y);
        
        /// <summary>
        /// Returns iVector3.rgg swizzling (equivalent to iVector3.xyy).
        /// </summary>
        public iVector3 rgg => new iVector3(x, y, y);
        
        /// <summary>
        /// Returns iVector3.xyyx swizzling.
        /// </summary>
        public iVector4 xyyx => new iVector4(x, y, y, x);
        
        /// <summary>
        /// Returns iVector3.rggr swizzling (equivalent to iVector3.xyyx).
        /// </summary>
        public iVector4 rggr => new iVector4(x, y, y, x);
        
        /// <summary>
        /// Returns iVector3.xyyy swizzling.
        /// </summary>
        public iVector4 xyyy => new iVector4(x, y, y, y);
        
        /// <summary>
        /// Returns iVector3.rggg swizzling (equivalent to iVector3.xyyy).
        /// </summary>
        public iVector4 rggg => new iVector4(x, y, y, y);
        
        /// <summary>
        /// Returns iVector3.xyyz swizzling.
        /// </summary>
        public iVector4 xyyz => new iVector4(x, y, y, z);
        
        /// <summary>
        /// Returns iVector3.rggb swizzling (equivalent to iVector3.xyyz).
        /// </summary>
        public iVector4 rggb => new iVector4(x, y, y, z);
        
        /// <summary>
        /// Returns iVector3.xyz swizzling.
        /// </summary>
        public iVector3 xyz => new iVector3(x, y, z);
        
        /// <summary>
        /// Returns iVector3.rgb swizzling (equivalent to iVector3.xyz).
        /// </summary>
        public iVector3 rgb => new iVector3(x, y, z);
        
        /// <summary>
        /// Returns iVector3.xyzx swizzling.
        /// </summary>
        public iVector4 xyzx => new iVector4(x, y, z, x);
        
        /// <summary>
        /// Returns iVector3.rgbr swizzling (equivalent to iVector3.xyzx).
        /// </summary>
        public iVector4 rgbr => new iVector4(x, y, z, x);
        
        /// <summary>
        /// Returns iVector3.xyzy swizzling.
        /// </summary>
        public iVector4 xyzy => new iVector4(x, y, z, y);
        
        /// <summary>
        /// Returns iVector3.rgbg swizzling (equivalent to iVector3.xyzy).
        /// </summary>
        public iVector4 rgbg => new iVector4(x, y, z, y);
        
        /// <summary>
        /// Returns iVector3.xyzz swizzling.
        /// </summary>
        public iVector4 xyzz => new iVector4(x, y, z, z);
        
        /// <summary>
        /// Returns iVector3.rgbb swizzling (equivalent to iVector3.xyzz).
        /// </summary>
        public iVector4 rgbb => new iVector4(x, y, z, z);
        
        /// <summary>
        /// Returns iVector3.xz swizzling.
        /// </summary>
        public iVector2 xz => new iVector2(x, z);
        
        /// <summary>
        /// Returns iVector3.rb swizzling (equivalent to iVector3.xz).
        /// </summary>
        public iVector2 rb => new iVector2(x, z);
        
        /// <summary>
        /// Returns iVector3.xzx swizzling.
        /// </summary>
        public iVector3 xzx => new iVector3(x, z, x);
        
        /// <summary>
        /// Returns iVector3.rbr swizzling (equivalent to iVector3.xzx).
        /// </summary>
        public iVector3 rbr => new iVector3(x, z, x);
        
        /// <summary>
        /// Returns iVector3.xzxx swizzling.
        /// </summary>
        public iVector4 xzxx => new iVector4(x, z, x, x);
        
        /// <summary>
        /// Returns iVector3.rbrr swizzling (equivalent to iVector3.xzxx).
        /// </summary>
        public iVector4 rbrr => new iVector4(x, z, x, x);
        
        /// <summary>
        /// Returns iVector3.xzxy swizzling.
        /// </summary>
        public iVector4 xzxy => new iVector4(x, z, x, y);
        
        /// <summary>
        /// Returns iVector3.rbrg swizzling (equivalent to iVector3.xzxy).
        /// </summary>
        public iVector4 rbrg => new iVector4(x, z, x, y);
        
        /// <summary>
        /// Returns iVector3.xzxz swizzling.
        /// </summary>
        public iVector4 xzxz => new iVector4(x, z, x, z);
        
        /// <summary>
        /// Returns iVector3.rbrb swizzling (equivalent to iVector3.xzxz).
        /// </summary>
        public iVector4 rbrb => new iVector4(x, z, x, z);
        
        /// <summary>
        /// Returns iVector3.xzy swizzling.
        /// </summary>
        public iVector3 xzy => new iVector3(x, z, y);
        
        /// <summary>
        /// Returns iVector3.rbg swizzling (equivalent to iVector3.xzy).
        /// </summary>
        public iVector3 rbg => new iVector3(x, z, y);
        
        /// <summary>
        /// Returns iVector3.xzyx swizzling.
        /// </summary>
        public iVector4 xzyx => new iVector4(x, z, y, x);
        
        /// <summary>
        /// Returns iVector3.rbgr swizzling (equivalent to iVector3.xzyx).
        /// </summary>
        public iVector4 rbgr => new iVector4(x, z, y, x);
        
        /// <summary>
        /// Returns iVector3.xzyy swizzling.
        /// </summary>
        public iVector4 xzyy => new iVector4(x, z, y, y);
        
        /// <summary>
        /// Returns iVector3.rbgg swizzling (equivalent to iVector3.xzyy).
        /// </summary>
        public iVector4 rbgg => new iVector4(x, z, y, y);
        
        /// <summary>
        /// Returns iVector3.xzyz swizzling.
        /// </summary>
        public iVector4 xzyz => new iVector4(x, z, y, z);
        
        /// <summary>
        /// Returns iVector3.rbgb swizzling (equivalent to iVector3.xzyz).
        /// </summary>
        public iVector4 rbgb => new iVector4(x, z, y, z);
        
        /// <summary>
        /// Returns iVector3.xzz swizzling.
        /// </summary>
        public iVector3 xzz => new iVector3(x, z, z);
        
        /// <summary>
        /// Returns iVector3.rbb swizzling (equivalent to iVector3.xzz).
        /// </summary>
        public iVector3 rbb => new iVector3(x, z, z);
        
        /// <summary>
        /// Returns iVector3.xzzx swizzling.
        /// </summary>
        public iVector4 xzzx => new iVector4(x, z, z, x);
        
        /// <summary>
        /// Returns iVector3.rbbr swizzling (equivalent to iVector3.xzzx).
        /// </summary>
        public iVector4 rbbr => new iVector4(x, z, z, x);
        
        /// <summary>
        /// Returns iVector3.xzzy swizzling.
        /// </summary>
        public iVector4 xzzy => new iVector4(x, z, z, y);
        
        /// <summary>
        /// Returns iVector3.rbbg swizzling (equivalent to iVector3.xzzy).
        /// </summary>
        public iVector4 rbbg => new iVector4(x, z, z, y);
        
        /// <summary>
        /// Returns iVector3.xzzz swizzling.
        /// </summary>
        public iVector4 xzzz => new iVector4(x, z, z, z);
        
        /// <summary>
        /// Returns iVector3.rbbb swizzling (equivalent to iVector3.xzzz).
        /// </summary>
        public iVector4 rbbb => new iVector4(x, z, z, z);
        
        /// <summary>
        /// Returns iVector3.yx swizzling.
        /// </summary>
        public iVector2 yx => new iVector2(y, x);
        
        /// <summary>
        /// Returns iVector3.gr swizzling (equivalent to iVector3.yx).
        /// </summary>
        public iVector2 gr => new iVector2(y, x);
        
        /// <summary>
        /// Returns iVector3.yxx swizzling.
        /// </summary>
        public iVector3 yxx => new iVector3(y, x, x);
        
        /// <summary>
        /// Returns iVector3.grr swizzling (equivalent to iVector3.yxx).
        /// </summary>
        public iVector3 grr => new iVector3(y, x, x);
        
        /// <summary>
        /// Returns iVector3.yxxx swizzling.
        /// </summary>
        public iVector4 yxxx => new iVector4(y, x, x, x);
        
        /// <summary>
        /// Returns iVector3.grrr swizzling (equivalent to iVector3.yxxx).
        /// </summary>
        public iVector4 grrr => new iVector4(y, x, x, x);
        
        /// <summary>
        /// Returns iVector3.yxxy swizzling.
        /// </summary>
        public iVector4 yxxy => new iVector4(y, x, x, y);
        
        /// <summary>
        /// Returns iVector3.grrg swizzling (equivalent to iVector3.yxxy).
        /// </summary>
        public iVector4 grrg => new iVector4(y, x, x, y);
        
        /// <summary>
        /// Returns iVector3.yxxz swizzling.
        /// </summary>
        public iVector4 yxxz => new iVector4(y, x, x, z);
        
        /// <summary>
        /// Returns iVector3.grrb swizzling (equivalent to iVector3.yxxz).
        /// </summary>
        public iVector4 grrb => new iVector4(y, x, x, z);
        
        /// <summary>
        /// Returns iVector3.yxy swizzling.
        /// </summary>
        public iVector3 yxy => new iVector3(y, x, y);
        
        /// <summary>
        /// Returns iVector3.grg swizzling (equivalent to iVector3.yxy).
        /// </summary>
        public iVector3 grg => new iVector3(y, x, y);
        
        /// <summary>
        /// Returns iVector3.yxyx swizzling.
        /// </summary>
        public iVector4 yxyx => new iVector4(y, x, y, x);
        
        /// <summary>
        /// Returns iVector3.grgr swizzling (equivalent to iVector3.yxyx).
        /// </summary>
        public iVector4 grgr => new iVector4(y, x, y, x);
        
        /// <summary>
        /// Returns iVector3.yxyy swizzling.
        /// </summary>
        public iVector4 yxyy => new iVector4(y, x, y, y);
        
        /// <summary>
        /// Returns iVector3.grgg swizzling (equivalent to iVector3.yxyy).
        /// </summary>
        public iVector4 grgg => new iVector4(y, x, y, y);
        
        /// <summary>
        /// Returns iVector3.yxyz swizzling.
        /// </summary>
        public iVector4 yxyz => new iVector4(y, x, y, z);
        
        /// <summary>
        /// Returns iVector3.grgb swizzling (equivalent to iVector3.yxyz).
        /// </summary>
        public iVector4 grgb => new iVector4(y, x, y, z);
        
        /// <summary>
        /// Returns iVector3.yxz swizzling.
        /// </summary>
        public iVector3 yxz => new iVector3(y, x, z);
        
        /// <summary>
        /// Returns iVector3.grb swizzling (equivalent to iVector3.yxz).
        /// </summary>
        public iVector3 grb => new iVector3(y, x, z);
        
        /// <summary>
        /// Returns iVector3.yxzx swizzling.
        /// </summary>
        public iVector4 yxzx => new iVector4(y, x, z, x);
        
        /// <summary>
        /// Returns iVector3.grbr swizzling (equivalent to iVector3.yxzx).
        /// </summary>
        public iVector4 grbr => new iVector4(y, x, z, x);
        
        /// <summary>
        /// Returns iVector3.yxzy swizzling.
        /// </summary>
        public iVector4 yxzy => new iVector4(y, x, z, y);
        
        /// <summary>
        /// Returns iVector3.grbg swizzling (equivalent to iVector3.yxzy).
        /// </summary>
        public iVector4 grbg => new iVector4(y, x, z, y);
        
        /// <summary>
        /// Returns iVector3.yxzz swizzling.
        /// </summary>
        public iVector4 yxzz => new iVector4(y, x, z, z);
        
        /// <summary>
        /// Returns iVector3.grbb swizzling (equivalent to iVector3.yxzz).
        /// </summary>
        public iVector4 grbb => new iVector4(y, x, z, z);
        
        /// <summary>
        /// Returns iVector3.yy swizzling.
        /// </summary>
        public iVector2 yy => new iVector2(y, y);
        
        /// <summary>
        /// Returns iVector3.gg swizzling (equivalent to iVector3.yy).
        /// </summary>
        public iVector2 gg => new iVector2(y, y);
        
        /// <summary>
        /// Returns iVector3.yyx swizzling.
        /// </summary>
        public iVector3 yyx => new iVector3(y, y, x);
        
        /// <summary>
        /// Returns iVector3.ggr swizzling (equivalent to iVector3.yyx).
        /// </summary>
        public iVector3 ggr => new iVector3(y, y, x);
        
        /// <summary>
        /// Returns iVector3.yyxx swizzling.
        /// </summary>
        public iVector4 yyxx => new iVector4(y, y, x, x);
        
        /// <summary>
        /// Returns iVector3.ggrr swizzling (equivalent to iVector3.yyxx).
        /// </summary>
        public iVector4 ggrr => new iVector4(y, y, x, x);
        
        /// <summary>
        /// Returns iVector3.yyxy swizzling.
        /// </summary>
        public iVector4 yyxy => new iVector4(y, y, x, y);
        
        /// <summary>
        /// Returns iVector3.ggrg swizzling (equivalent to iVector3.yyxy).
        /// </summary>
        public iVector4 ggrg => new iVector4(y, y, x, y);
        
        /// <summary>
        /// Returns iVector3.yyxz swizzling.
        /// </summary>
        public iVector4 yyxz => new iVector4(y, y, x, z);
        
        /// <summary>
        /// Returns iVector3.ggrb swizzling (equivalent to iVector3.yyxz).
        /// </summary>
        public iVector4 ggrb => new iVector4(y, y, x, z);
        
        /// <summary>
        /// Returns iVector3.yyy swizzling.
        /// </summary>
        public iVector3 yyy => new iVector3(y, y, y);
        
        /// <summary>
        /// Returns iVector3.ggg swizzling (equivalent to iVector3.yyy).
        /// </summary>
        public iVector3 ggg => new iVector3(y, y, y);
        
        /// <summary>
        /// Returns iVector3.yyyx swizzling.
        /// </summary>
        public iVector4 yyyx => new iVector4(y, y, y, x);
        
        /// <summary>
        /// Returns iVector3.gggr swizzling (equivalent to iVector3.yyyx).
        /// </summary>
        public iVector4 gggr => new iVector4(y, y, y, x);
        
        /// <summary>
        /// Returns iVector3.yyyy swizzling.
        /// </summary>
        public iVector4 yyyy => new iVector4(y, y, y, y);
        
        /// <summary>
        /// Returns iVector3.gggg swizzling (equivalent to iVector3.yyyy).
        /// </summary>
        public iVector4 gggg => new iVector4(y, y, y, y);
        
        /// <summary>
        /// Returns iVector3.yyyz swizzling.
        /// </summary>
        public iVector4 yyyz => new iVector4(y, y, y, z);
        
        /// <summary>
        /// Returns iVector3.gggb swizzling (equivalent to iVector3.yyyz).
        /// </summary>
        public iVector4 gggb => new iVector4(y, y, y, z);
        
        /// <summary>
        /// Returns iVector3.yyz swizzling.
        /// </summary>
        public iVector3 yyz => new iVector3(y, y, z);
        
        /// <summary>
        /// Returns iVector3.ggb swizzling (equivalent to iVector3.yyz).
        /// </summary>
        public iVector3 ggb => new iVector3(y, y, z);
        
        /// <summary>
        /// Returns iVector3.yyzx swizzling.
        /// </summary>
        public iVector4 yyzx => new iVector4(y, y, z, x);
        
        /// <summary>
        /// Returns iVector3.ggbr swizzling (equivalent to iVector3.yyzx).
        /// </summary>
        public iVector4 ggbr => new iVector4(y, y, z, x);
        
        /// <summary>
        /// Returns iVector3.yyzy swizzling.
        /// </summary>
        public iVector4 yyzy => new iVector4(y, y, z, y);
        
        /// <summary>
        /// Returns iVector3.ggbg swizzling (equivalent to iVector3.yyzy).
        /// </summary>
        public iVector4 ggbg => new iVector4(y, y, z, y);
        
        /// <summary>
        /// Returns iVector3.yyzz swizzling.
        /// </summary>
        public iVector4 yyzz => new iVector4(y, y, z, z);
        
        /// <summary>
        /// Returns iVector3.ggbb swizzling (equivalent to iVector3.yyzz).
        /// </summary>
        public iVector4 ggbb => new iVector4(y, y, z, z);
        
        /// <summary>
        /// Returns iVector3.yz swizzling.
        /// </summary>
        public iVector2 yz => new iVector2(y, z);
        
        /// <summary>
        /// Returns iVector3.gb swizzling (equivalent to iVector3.yz).
        /// </summary>
        public iVector2 gb => new iVector2(y, z);
        
        /// <summary>
        /// Returns iVector3.yzx swizzling.
        /// </summary>
        public iVector3 yzx => new iVector3(y, z, x);
        
        /// <summary>
        /// Returns iVector3.gbr swizzling (equivalent to iVector3.yzx).
        /// </summary>
        public iVector3 gbr => new iVector3(y, z, x);
        
        /// <summary>
        /// Returns iVector3.yzxx swizzling.
        /// </summary>
        public iVector4 yzxx => new iVector4(y, z, x, x);
        
        /// <summary>
        /// Returns iVector3.gbrr swizzling (equivalent to iVector3.yzxx).
        /// </summary>
        public iVector4 gbrr => new iVector4(y, z, x, x);
        
        /// <summary>
        /// Returns iVector3.yzxy swizzling.
        /// </summary>
        public iVector4 yzxy => new iVector4(y, z, x, y);
        
        /// <summary>
        /// Returns iVector3.gbrg swizzling (equivalent to iVector3.yzxy).
        /// </summary>
        public iVector4 gbrg => new iVector4(y, z, x, y);
        
        /// <summary>
        /// Returns iVector3.yzxz swizzling.
        /// </summary>
        public iVector4 yzxz => new iVector4(y, z, x, z);
        
        /// <summary>
        /// Returns iVector3.gbrb swizzling (equivalent to iVector3.yzxz).
        /// </summary>
        public iVector4 gbrb => new iVector4(y, z, x, z);
        
        /// <summary>
        /// Returns iVector3.yzy swizzling.
        /// </summary>
        public iVector3 yzy => new iVector3(y, z, y);
        
        /// <summary>
        /// Returns iVector3.gbg swizzling (equivalent to iVector3.yzy).
        /// </summary>
        public iVector3 gbg => new iVector3(y, z, y);
        
        /// <summary>
        /// Returns iVector3.yzyx swizzling.
        /// </summary>
        public iVector4 yzyx => new iVector4(y, z, y, x);
        
        /// <summary>
        /// Returns iVector3.gbgr swizzling (equivalent to iVector3.yzyx).
        /// </summary>
        public iVector4 gbgr => new iVector4(y, z, y, x);
        
        /// <summary>
        /// Returns iVector3.yzyy swizzling.
        /// </summary>
        public iVector4 yzyy => new iVector4(y, z, y, y);
        
        /// <summary>
        /// Returns iVector3.gbgg swizzling (equivalent to iVector3.yzyy).
        /// </summary>
        public iVector4 gbgg => new iVector4(y, z, y, y);
        
        /// <summary>
        /// Returns iVector3.yzyz swizzling.
        /// </summary>
        public iVector4 yzyz => new iVector4(y, z, y, z);
        
        /// <summary>
        /// Returns iVector3.gbgb swizzling (equivalent to iVector3.yzyz).
        /// </summary>
        public iVector4 gbgb => new iVector4(y, z, y, z);
        
        /// <summary>
        /// Returns iVector3.yzz swizzling.
        /// </summary>
        public iVector3 yzz => new iVector3(y, z, z);
        
        /// <summary>
        /// Returns iVector3.gbb swizzling (equivalent to iVector3.yzz).
        /// </summary>
        public iVector3 gbb => new iVector3(y, z, z);
        
        /// <summary>
        /// Returns iVector3.yzzx swizzling.
        /// </summary>
        public iVector4 yzzx => new iVector4(y, z, z, x);
        
        /// <summary>
        /// Returns iVector3.gbbr swizzling (equivalent to iVector3.yzzx).
        /// </summary>
        public iVector4 gbbr => new iVector4(y, z, z, x);
        
        /// <summary>
        /// Returns iVector3.yzzy swizzling.
        /// </summary>
        public iVector4 yzzy => new iVector4(y, z, z, y);
        
        /// <summary>
        /// Returns iVector3.gbbg swizzling (equivalent to iVector3.yzzy).
        /// </summary>
        public iVector4 gbbg => new iVector4(y, z, z, y);
        
        /// <summary>
        /// Returns iVector3.yzzz swizzling.
        /// </summary>
        public iVector4 yzzz => new iVector4(y, z, z, z);
        
        /// <summary>
        /// Returns iVector3.gbbb swizzling (equivalent to iVector3.yzzz).
        /// </summary>
        public iVector4 gbbb => new iVector4(y, z, z, z);
        
        /// <summary>
        /// Returns iVector3.zx swizzling.
        /// </summary>
        public iVector2 zx => new iVector2(z, x);
        
        /// <summary>
        /// Returns iVector3.br swizzling (equivalent to iVector3.zx).
        /// </summary>
        public iVector2 br => new iVector2(z, x);
        
        /// <summary>
        /// Returns iVector3.zxx swizzling.
        /// </summary>
        public iVector3 zxx => new iVector3(z, x, x);
        
        /// <summary>
        /// Returns iVector3.brr swizzling (equivalent to iVector3.zxx).
        /// </summary>
        public iVector3 brr => new iVector3(z, x, x);
        
        /// <summary>
        /// Returns iVector3.zxxx swizzling.
        /// </summary>
        public iVector4 zxxx => new iVector4(z, x, x, x);
        
        /// <summary>
        /// Returns iVector3.brrr swizzling (equivalent to iVector3.zxxx).
        /// </summary>
        public iVector4 brrr => new iVector4(z, x, x, x);
        
        /// <summary>
        /// Returns iVector3.zxxy swizzling.
        /// </summary>
        public iVector4 zxxy => new iVector4(z, x, x, y);
        
        /// <summary>
        /// Returns iVector3.brrg swizzling (equivalent to iVector3.zxxy).
        /// </summary>
        public iVector4 brrg => new iVector4(z, x, x, y);
        
        /// <summary>
        /// Returns iVector3.zxxz swizzling.
        /// </summary>
        public iVector4 zxxz => new iVector4(z, x, x, z);
        
        /// <summary>
        /// Returns iVector3.brrb swizzling (equivalent to iVector3.zxxz).
        /// </summary>
        public iVector4 brrb => new iVector4(z, x, x, z);
        
        /// <summary>
        /// Returns iVector3.zxy swizzling.
        /// </summary>
        public iVector3 zxy => new iVector3(z, x, y);
        
        /// <summary>
        /// Returns iVector3.brg swizzling (equivalent to iVector3.zxy).
        /// </summary>
        public iVector3 brg => new iVector3(z, x, y);
        
        /// <summary>
        /// Returns iVector3.zxyx swizzling.
        /// </summary>
        public iVector4 zxyx => new iVector4(z, x, y, x);
        
        /// <summary>
        /// Returns iVector3.brgr swizzling (equivalent to iVector3.zxyx).
        /// </summary>
        public iVector4 brgr => new iVector4(z, x, y, x);
        
        /// <summary>
        /// Returns iVector3.zxyy swizzling.
        /// </summary>
        public iVector4 zxyy => new iVector4(z, x, y, y);
        
        /// <summary>
        /// Returns iVector3.brgg swizzling (equivalent to iVector3.zxyy).
        /// </summary>
        public iVector4 brgg => new iVector4(z, x, y, y);
        
        /// <summary>
        /// Returns iVector3.zxyz swizzling.
        /// </summary>
        public iVector4 zxyz => new iVector4(z, x, y, z);
        
        /// <summary>
        /// Returns iVector3.brgb swizzling (equivalent to iVector3.zxyz).
        /// </summary>
        public iVector4 brgb => new iVector4(z, x, y, z);
        
        /// <summary>
        /// Returns iVector3.zxz swizzling.
        /// </summary>
        public iVector3 zxz => new iVector3(z, x, z);
        
        /// <summary>
        /// Returns iVector3.brb swizzling (equivalent to iVector3.zxz).
        /// </summary>
        public iVector3 brb => new iVector3(z, x, z);
        
        /// <summary>
        /// Returns iVector3.zxzx swizzling.
        /// </summary>
        public iVector4 zxzx => new iVector4(z, x, z, x);
        
        /// <summary>
        /// Returns iVector3.brbr swizzling (equivalent to iVector3.zxzx).
        /// </summary>
        public iVector4 brbr => new iVector4(z, x, z, x);
        
        /// <summary>
        /// Returns iVector3.zxzy swizzling.
        /// </summary>
        public iVector4 zxzy => new iVector4(z, x, z, y);
        
        /// <summary>
        /// Returns iVector3.brbg swizzling (equivalent to iVector3.zxzy).
        /// </summary>
        public iVector4 brbg => new iVector4(z, x, z, y);
        
        /// <summary>
        /// Returns iVector3.zxzz swizzling.
        /// </summary>
        public iVector4 zxzz => new iVector4(z, x, z, z);
        
        /// <summary>
        /// Returns iVector3.brbb swizzling (equivalent to iVector3.zxzz).
        /// </summary>
        public iVector4 brbb => new iVector4(z, x, z, z);
        
        /// <summary>
        /// Returns iVector3.zy swizzling.
        /// </summary>
        public iVector2 zy => new iVector2(z, y);
        
        /// <summary>
        /// Returns iVector3.bg swizzling (equivalent to iVector3.zy).
        /// </summary>
        public iVector2 bg => new iVector2(z, y);
        
        /// <summary>
        /// Returns iVector3.zyx swizzling.
        /// </summary>
        public iVector3 zyx => new iVector3(z, y, x);
        
        /// <summary>
        /// Returns iVector3.bgr swizzling (equivalent to iVector3.zyx).
        /// </summary>
        public iVector3 bgr => new iVector3(z, y, x);
        
        /// <summary>
        /// Returns iVector3.zyxx swizzling.
        /// </summary>
        public iVector4 zyxx => new iVector4(z, y, x, x);
        
        /// <summary>
        /// Returns iVector3.bgrr swizzling (equivalent to iVector3.zyxx).
        /// </summary>
        public iVector4 bgrr => new iVector4(z, y, x, x);
        
        /// <summary>
        /// Returns iVector3.zyxy swizzling.
        /// </summary>
        public iVector4 zyxy => new iVector4(z, y, x, y);
        
        /// <summary>
        /// Returns iVector3.bgrg swizzling (equivalent to iVector3.zyxy).
        /// </summary>
        public iVector4 bgrg => new iVector4(z, y, x, y);
        
        /// <summary>
        /// Returns iVector3.zyxz swizzling.
        /// </summary>
        public iVector4 zyxz => new iVector4(z, y, x, z);
        
        /// <summary>
        /// Returns iVector3.bgrb swizzling (equivalent to iVector3.zyxz).
        /// </summary>
        public iVector4 bgrb => new iVector4(z, y, x, z);
        
        /// <summary>
        /// Returns iVector3.zyy swizzling.
        /// </summary>
        public iVector3 zyy => new iVector3(z, y, y);
        
        /// <summary>
        /// Returns iVector3.bgg swizzling (equivalent to iVector3.zyy).
        /// </summary>
        public iVector3 bgg => new iVector3(z, y, y);
        
        /// <summary>
        /// Returns iVector3.zyyx swizzling.
        /// </summary>
        public iVector4 zyyx => new iVector4(z, y, y, x);
        
        /// <summary>
        /// Returns iVector3.bggr swizzling (equivalent to iVector3.zyyx).
        /// </summary>
        public iVector4 bggr => new iVector4(z, y, y, x);
        
        /// <summary>
        /// Returns iVector3.zyyy swizzling.
        /// </summary>
        public iVector4 zyyy => new iVector4(z, y, y, y);
        
        /// <summary>
        /// Returns iVector3.bggg swizzling (equivalent to iVector3.zyyy).
        /// </summary>
        public iVector4 bggg => new iVector4(z, y, y, y);
        
        /// <summary>
        /// Returns iVector3.zyyz swizzling.
        /// </summary>
        public iVector4 zyyz => new iVector4(z, y, y, z);
        
        /// <summary>
        /// Returns iVector3.bggb swizzling (equivalent to iVector3.zyyz).
        /// </summary>
        public iVector4 bggb => new iVector4(z, y, y, z);
        
        /// <summary>
        /// Returns iVector3.zyz swizzling.
        /// </summary>
        public iVector3 zyz => new iVector3(z, y, z);
        
        /// <summary>
        /// Returns iVector3.bgb swizzling (equivalent to iVector3.zyz).
        /// </summary>
        public iVector3 bgb => new iVector3(z, y, z);
        
        /// <summary>
        /// Returns iVector3.zyzx swizzling.
        /// </summary>
        public iVector4 zyzx => new iVector4(z, y, z, x);
        
        /// <summary>
        /// Returns iVector3.bgbr swizzling (equivalent to iVector3.zyzx).
        /// </summary>
        public iVector4 bgbr => new iVector4(z, y, z, x);
        
        /// <summary>
        /// Returns iVector3.zyzy swizzling.
        /// </summary>
        public iVector4 zyzy => new iVector4(z, y, z, y);
        
        /// <summary>
        /// Returns iVector3.bgbg swizzling (equivalent to iVector3.zyzy).
        /// </summary>
        public iVector4 bgbg => new iVector4(z, y, z, y);
        
        /// <summary>
        /// Returns iVector3.zyzz swizzling.
        /// </summary>
        public iVector4 zyzz => new iVector4(z, y, z, z);
        
        /// <summary>
        /// Returns iVector3.bgbb swizzling (equivalent to iVector3.zyzz).
        /// </summary>
        public iVector4 bgbb => new iVector4(z, y, z, z);
        
        /// <summary>
        /// Returns iVector3.zz swizzling.
        /// </summary>
        public iVector2 zz => new iVector2(z, z);
        
        /// <summary>
        /// Returns iVector3.bb swizzling (equivalent to iVector3.zz).
        /// </summary>
        public iVector2 bb => new iVector2(z, z);
        
        /// <summary>
        /// Returns iVector3.zzx swizzling.
        /// </summary>
        public iVector3 zzx => new iVector3(z, z, x);
        
        /// <summary>
        /// Returns iVector3.bbr swizzling (equivalent to iVector3.zzx).
        /// </summary>
        public iVector3 bbr => new iVector3(z, z, x);
        
        /// <summary>
        /// Returns iVector3.zzxx swizzling.
        /// </summary>
        public iVector4 zzxx => new iVector4(z, z, x, x);
        
        /// <summary>
        /// Returns iVector3.bbrr swizzling (equivalent to iVector3.zzxx).
        /// </summary>
        public iVector4 bbrr => new iVector4(z, z, x, x);
        
        /// <summary>
        /// Returns iVector3.zzxy swizzling.
        /// </summary>
        public iVector4 zzxy => new iVector4(z, z, x, y);
        
        /// <summary>
        /// Returns iVector3.bbrg swizzling (equivalent to iVector3.zzxy).
        /// </summary>
        public iVector4 bbrg => new iVector4(z, z, x, y);
        
        /// <summary>
        /// Returns iVector3.zzxz swizzling.
        /// </summary>
        public iVector4 zzxz => new iVector4(z, z, x, z);
        
        /// <summary>
        /// Returns iVector3.bbrb swizzling (equivalent to iVector3.zzxz).
        /// </summary>
        public iVector4 bbrb => new iVector4(z, z, x, z);
        
        /// <summary>
        /// Returns iVector3.zzy swizzling.
        /// </summary>
        public iVector3 zzy => new iVector3(z, z, y);
        
        /// <summary>
        /// Returns iVector3.bbg swizzling (equivalent to iVector3.zzy).
        /// </summary>
        public iVector3 bbg => new iVector3(z, z, y);
        
        /// <summary>
        /// Returns iVector3.zzyx swizzling.
        /// </summary>
        public iVector4 zzyx => new iVector4(z, z, y, x);
        
        /// <summary>
        /// Returns iVector3.bbgr swizzling (equivalent to iVector3.zzyx).
        /// </summary>
        public iVector4 bbgr => new iVector4(z, z, y, x);
        
        /// <summary>
        /// Returns iVector3.zzyy swizzling.
        /// </summary>
        public iVector4 zzyy => new iVector4(z, z, y, y);
        
        /// <summary>
        /// Returns iVector3.bbgg swizzling (equivalent to iVector3.zzyy).
        /// </summary>
        public iVector4 bbgg => new iVector4(z, z, y, y);
        
        /// <summary>
        /// Returns iVector3.zzyz swizzling.
        /// </summary>
        public iVector4 zzyz => new iVector4(z, z, y, z);
        
        /// <summary>
        /// Returns iVector3.bbgb swizzling (equivalent to iVector3.zzyz).
        /// </summary>
        public iVector4 bbgb => new iVector4(z, z, y, z);
        
        /// <summary>
        /// Returns iVector3.zzz swizzling.
        /// </summary>
        public iVector3 zzz => new iVector3(z, z, z);
        
        /// <summary>
        /// Returns iVector3.bbb swizzling (equivalent to iVector3.zzz).
        /// </summary>
        public iVector3 bbb => new iVector3(z, z, z);
        
        /// <summary>
        /// Returns iVector3.zzzx swizzling.
        /// </summary>
        public iVector4 zzzx => new iVector4(z, z, z, x);
        
        /// <summary>
        /// Returns iVector3.bbbr swizzling (equivalent to iVector3.zzzx).
        /// </summary>
        public iVector4 bbbr => new iVector4(z, z, z, x);
        
        /// <summary>
        /// Returns iVector3.zzzy swizzling.
        /// </summary>
        public iVector4 zzzy => new iVector4(z, z, z, y);
        
        /// <summary>
        /// Returns iVector3.bbbg swizzling (equivalent to iVector3.zzzy).
        /// </summary>
        public iVector4 bbbg => new iVector4(z, z, z, y);
        
        /// <summary>
        /// Returns iVector3.zzzz swizzling.
        /// </summary>
        public iVector4 zzzz => new iVector4(z, z, z, z);
        
        /// <summary>
        /// Returns iVector3.bbbb swizzling (equivalent to iVector3.zzzz).
        /// </summary>
        public iVector4 bbbb => new iVector4(z, z, z, z);

        #endregion

    }
}
