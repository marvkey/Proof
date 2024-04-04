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
    /// Temporary Vectortor of type T with 3 components, used for implementing swizzling for gVector3.
    /// </summary>
    [RegisterCoreClassStruct]
   
    [StructLayout(LayoutKind.Sequential)]
    public struct swizzle_gVector3<T>
    {

        #region Fields
        
        /// <summary>
        /// x-component
        /// </summary>
        
        internal readonly T x;
        
        /// <summary>
        /// y-component
        /// </summary>
        
        internal readonly T y;
        
        /// <summary>
        /// z-component
        /// </summary>
        
        internal readonly T z;

        #endregion


        #region Constructors
        
        /// <summary>
        /// Constructor for swizzle_gVector3.
        /// </summary>
        internal swizzle_gVector3(T x, T y, T z)
        {
            this.x = x;
            this.y = y;
            this.z = z;
        }

        #endregion


        #region Properties
        
        /// <summary>
        /// Returns gVector3.xx swizzling.
        /// </summary>
        public gVector2<T> xx => new gVector2<T>(x, x);
        
        /// <summary>
        /// Returns gVector3.rr swizzling (equivalent to gVector3.xx).
        /// </summary>
        public gVector2<T> rr => new gVector2<T>(x, x);
        
        /// <summary>
        /// Returns gVector3.xxx swizzling.
        /// </summary>
        public gVector3<T> xxx => new gVector3<T>(x, x, x);
        
        /// <summary>
        /// Returns gVector3.rrr swizzling (equivalent to gVector3.xxx).
        /// </summary>
        public gVector3<T> rrr => new gVector3<T>(x, x, x);
        
        /// <summary>
        /// Returns gVector3.xxxx swizzling.
        /// </summary>
        public gVector4<T> xxxx => new gVector4<T>(x, x, x, x);
        
        /// <summary>
        /// Returns gVector3.rrrr swizzling (equivalent to gVector3.xxxx).
        /// </summary>
        public gVector4<T> rrrr => new gVector4<T>(x, x, x, x);
        
        /// <summary>
        /// Returns gVector3.xxxy swizzling.
        /// </summary>
        public gVector4<T> xxxy => new gVector4<T>(x, x, x, y);
        
        /// <summary>
        /// Returns gVector3.rrrg swizzling (equivalent to gVector3.xxxy).
        /// </summary>
        public gVector4<T> rrrg => new gVector4<T>(x, x, x, y);
        
        /// <summary>
        /// Returns gVector3.xxxz swizzling.
        /// </summary>
        public gVector4<T> xxxz => new gVector4<T>(x, x, x, z);
        
        /// <summary>
        /// Returns gVector3.rrrb swizzling (equivalent to gVector3.xxxz).
        /// </summary>
        public gVector4<T> rrrb => new gVector4<T>(x, x, x, z);
        
        /// <summary>
        /// Returns gVector3.xxy swizzling.
        /// </summary>
        public gVector3<T> xxy => new gVector3<T>(x, x, y);
        
        /// <summary>
        /// Returns gVector3.rrg swizzling (equivalent to gVector3.xxy).
        /// </summary>
        public gVector3<T> rrg => new gVector3<T>(x, x, y);
        
        /// <summary>
        /// Returns gVector3.xxyx swizzling.
        /// </summary>
        public gVector4<T> xxyx => new gVector4<T>(x, x, y, x);
        
        /// <summary>
        /// Returns gVector3.rrgr swizzling (equivalent to gVector3.xxyx).
        /// </summary>
        public gVector4<T> rrgr => new gVector4<T>(x, x, y, x);
        
        /// <summary>
        /// Returns gVector3.xxyy swizzling.
        /// </summary>
        public gVector4<T> xxyy => new gVector4<T>(x, x, y, y);
        
        /// <summary>
        /// Returns gVector3.rrgg swizzling (equivalent to gVector3.xxyy).
        /// </summary>
        public gVector4<T> rrgg => new gVector4<T>(x, x, y, y);
        
        /// <summary>
        /// Returns gVector3.xxyz swizzling.
        /// </summary>
        public gVector4<T> xxyz => new gVector4<T>(x, x, y, z);
        
        /// <summary>
        /// Returns gVector3.rrgb swizzling (equivalent to gVector3.xxyz).
        /// </summary>
        public gVector4<T> rrgb => new gVector4<T>(x, x, y, z);
        
        /// <summary>
        /// Returns gVector3.xxz swizzling.
        /// </summary>
        public gVector3<T> xxz => new gVector3<T>(x, x, z);
        
        /// <summary>
        /// Returns gVector3.rrb swizzling (equivalent to gVector3.xxz).
        /// </summary>
        public gVector3<T> rrb => new gVector3<T>(x, x, z);
        
        /// <summary>
        /// Returns gVector3.xxzx swizzling.
        /// </summary>
        public gVector4<T> xxzx => new gVector4<T>(x, x, z, x);
        
        /// <summary>
        /// Returns gVector3.rrbr swizzling (equivalent to gVector3.xxzx).
        /// </summary>
        public gVector4<T> rrbr => new gVector4<T>(x, x, z, x);
        
        /// <summary>
        /// Returns gVector3.xxzy swizzling.
        /// </summary>
        public gVector4<T> xxzy => new gVector4<T>(x, x, z, y);
        
        /// <summary>
        /// Returns gVector3.rrbg swizzling (equivalent to gVector3.xxzy).
        /// </summary>
        public gVector4<T> rrbg => new gVector4<T>(x, x, z, y);
        
        /// <summary>
        /// Returns gVector3.xxzz swizzling.
        /// </summary>
        public gVector4<T> xxzz => new gVector4<T>(x, x, z, z);
        
        /// <summary>
        /// Returns gVector3.rrbb swizzling (equivalent to gVector3.xxzz).
        /// </summary>
        public gVector4<T> rrbb => new gVector4<T>(x, x, z, z);
        
        /// <summary>
        /// Returns gVector3.xy swizzling.
        /// </summary>
        public gVector2<T> xy => new gVector2<T>(x, y);
        
        /// <summary>
        /// Returns gVector3.rg swizzling (equivalent to gVector3.xy).
        /// </summary>
        public gVector2<T> rg => new gVector2<T>(x, y);
        
        /// <summary>
        /// Returns gVector3.xyx swizzling.
        /// </summary>
        public gVector3<T> xyx => new gVector3<T>(x, y, x);
        
        /// <summary>
        /// Returns gVector3.rgr swizzling (equivalent to gVector3.xyx).
        /// </summary>
        public gVector3<T> rgr => new gVector3<T>(x, y, x);
        
        /// <summary>
        /// Returns gVector3.xyxx swizzling.
        /// </summary>
        public gVector4<T> xyxx => new gVector4<T>(x, y, x, x);
        
        /// <summary>
        /// Returns gVector3.rgrr swizzling (equivalent to gVector3.xyxx).
        /// </summary>
        public gVector4<T> rgrr => new gVector4<T>(x, y, x, x);
        
        /// <summary>
        /// Returns gVector3.xyxy swizzling.
        /// </summary>
        public gVector4<T> xyxy => new gVector4<T>(x, y, x, y);
        
        /// <summary>
        /// Returns gVector3.rgrg swizzling (equivalent to gVector3.xyxy).
        /// </summary>
        public gVector4<T> rgrg => new gVector4<T>(x, y, x, y);
        
        /// <summary>
        /// Returns gVector3.xyxz swizzling.
        /// </summary>
        public gVector4<T> xyxz => new gVector4<T>(x, y, x, z);
        
        /// <summary>
        /// Returns gVector3.rgrb swizzling (equivalent to gVector3.xyxz).
        /// </summary>
        public gVector4<T> rgrb => new gVector4<T>(x, y, x, z);
        
        /// <summary>
        /// Returns gVector3.xyy swizzling.
        /// </summary>
        public gVector3<T> xyy => new gVector3<T>(x, y, y);
        
        /// <summary>
        /// Returns gVector3.rgg swizzling (equivalent to gVector3.xyy).
        /// </summary>
        public gVector3<T> rgg => new gVector3<T>(x, y, y);
        
        /// <summary>
        /// Returns gVector3.xyyx swizzling.
        /// </summary>
        public gVector4<T> xyyx => new gVector4<T>(x, y, y, x);
        
        /// <summary>
        /// Returns gVector3.rggr swizzling (equivalent to gVector3.xyyx).
        /// </summary>
        public gVector4<T> rggr => new gVector4<T>(x, y, y, x);
        
        /// <summary>
        /// Returns gVector3.xyyy swizzling.
        /// </summary>
        public gVector4<T> xyyy => new gVector4<T>(x, y, y, y);
        
        /// <summary>
        /// Returns gVector3.rggg swizzling (equivalent to gVector3.xyyy).
        /// </summary>
        public gVector4<T> rggg => new gVector4<T>(x, y, y, y);
        
        /// <summary>
        /// Returns gVector3.xyyz swizzling.
        /// </summary>
        public gVector4<T> xyyz => new gVector4<T>(x, y, y, z);
        
        /// <summary>
        /// Returns gVector3.rggb swizzling (equivalent to gVector3.xyyz).
        /// </summary>
        public gVector4<T> rggb => new gVector4<T>(x, y, y, z);
        
        /// <summary>
        /// Returns gVector3.xyz swizzling.
        /// </summary>
        public gVector3<T> xyz => new gVector3<T>(x, y, z);
        
        /// <summary>
        /// Returns gVector3.rgb swizzling (equivalent to gVector3.xyz).
        /// </summary>
        public gVector3<T> rgb => new gVector3<T>(x, y, z);
        
        /// <summary>
        /// Returns gVector3.xyzx swizzling.
        /// </summary>
        public gVector4<T> xyzx => new gVector4<T>(x, y, z, x);
        
        /// <summary>
        /// Returns gVector3.rgbr swizzling (equivalent to gVector3.xyzx).
        /// </summary>
        public gVector4<T> rgbr => new gVector4<T>(x, y, z, x);
        
        /// <summary>
        /// Returns gVector3.xyzy swizzling.
        /// </summary>
        public gVector4<T> xyzy => new gVector4<T>(x, y, z, y);
        
        /// <summary>
        /// Returns gVector3.rgbg swizzling (equivalent to gVector3.xyzy).
        /// </summary>
        public gVector4<T> rgbg => new gVector4<T>(x, y, z, y);
        
        /// <summary>
        /// Returns gVector3.xyzz swizzling.
        /// </summary>
        public gVector4<T> xyzz => new gVector4<T>(x, y, z, z);
        
        /// <summary>
        /// Returns gVector3.rgbb swizzling (equivalent to gVector3.xyzz).
        /// </summary>
        public gVector4<T> rgbb => new gVector4<T>(x, y, z, z);
        
        /// <summary>
        /// Returns gVector3.xz swizzling.
        /// </summary>
        public gVector2<T> xz => new gVector2<T>(x, z);
        
        /// <summary>
        /// Returns gVector3.rb swizzling (equivalent to gVector3.xz).
        /// </summary>
        public gVector2<T> rb => new gVector2<T>(x, z);
        
        /// <summary>
        /// Returns gVector3.xzx swizzling.
        /// </summary>
        public gVector3<T> xzx => new gVector3<T>(x, z, x);
        
        /// <summary>
        /// Returns gVector3.rbr swizzling (equivalent to gVector3.xzx).
        /// </summary>
        public gVector3<T> rbr => new gVector3<T>(x, z, x);
        
        /// <summary>
        /// Returns gVector3.xzxx swizzling.
        /// </summary>
        public gVector4<T> xzxx => new gVector4<T>(x, z, x, x);
        
        /// <summary>
        /// Returns gVector3.rbrr swizzling (equivalent to gVector3.xzxx).
        /// </summary>
        public gVector4<T> rbrr => new gVector4<T>(x, z, x, x);
        
        /// <summary>
        /// Returns gVector3.xzxy swizzling.
        /// </summary>
        public gVector4<T> xzxy => new gVector4<T>(x, z, x, y);
        
        /// <summary>
        /// Returns gVector3.rbrg swizzling (equivalent to gVector3.xzxy).
        /// </summary>
        public gVector4<T> rbrg => new gVector4<T>(x, z, x, y);
        
        /// <summary>
        /// Returns gVector3.xzxz swizzling.
        /// </summary>
        public gVector4<T> xzxz => new gVector4<T>(x, z, x, z);
        
        /// <summary>
        /// Returns gVector3.rbrb swizzling (equivalent to gVector3.xzxz).
        /// </summary>
        public gVector4<T> rbrb => new gVector4<T>(x, z, x, z);
        
        /// <summary>
        /// Returns gVector3.xzy swizzling.
        /// </summary>
        public gVector3<T> xzy => new gVector3<T>(x, z, y);
        
        /// <summary>
        /// Returns gVector3.rbg swizzling (equivalent to gVector3.xzy).
        /// </summary>
        public gVector3<T> rbg => new gVector3<T>(x, z, y);
        
        /// <summary>
        /// Returns gVector3.xzyx swizzling.
        /// </summary>
        public gVector4<T> xzyx => new gVector4<T>(x, z, y, x);
        
        /// <summary>
        /// Returns gVector3.rbgr swizzling (equivalent to gVector3.xzyx).
        /// </summary>
        public gVector4<T> rbgr => new gVector4<T>(x, z, y, x);
        
        /// <summary>
        /// Returns gVector3.xzyy swizzling.
        /// </summary>
        public gVector4<T> xzyy => new gVector4<T>(x, z, y, y);
        
        /// <summary>
        /// Returns gVector3.rbgg swizzling (equivalent to gVector3.xzyy).
        /// </summary>
        public gVector4<T> rbgg => new gVector4<T>(x, z, y, y);
        
        /// <summary>
        /// Returns gVector3.xzyz swizzling.
        /// </summary>
        public gVector4<T> xzyz => new gVector4<T>(x, z, y, z);
        
        /// <summary>
        /// Returns gVector3.rbgb swizzling (equivalent to gVector3.xzyz).
        /// </summary>
        public gVector4<T> rbgb => new gVector4<T>(x, z, y, z);
        
        /// <summary>
        /// Returns gVector3.xzz swizzling.
        /// </summary>
        public gVector3<T> xzz => new gVector3<T>(x, z, z);
        
        /// <summary>
        /// Returns gVector3.rbb swizzling (equivalent to gVector3.xzz).
        /// </summary>
        public gVector3<T> rbb => new gVector3<T>(x, z, z);
        
        /// <summary>
        /// Returns gVector3.xzzx swizzling.
        /// </summary>
        public gVector4<T> xzzx => new gVector4<T>(x, z, z, x);
        
        /// <summary>
        /// Returns gVector3.rbbr swizzling (equivalent to gVector3.xzzx).
        /// </summary>
        public gVector4<T> rbbr => new gVector4<T>(x, z, z, x);
        
        /// <summary>
        /// Returns gVector3.xzzy swizzling.
        /// </summary>
        public gVector4<T> xzzy => new gVector4<T>(x, z, z, y);
        
        /// <summary>
        /// Returns gVector3.rbbg swizzling (equivalent to gVector3.xzzy).
        /// </summary>
        public gVector4<T> rbbg => new gVector4<T>(x, z, z, y);
        
        /// <summary>
        /// Returns gVector3.xzzz swizzling.
        /// </summary>
        public gVector4<T> xzzz => new gVector4<T>(x, z, z, z);
        
        /// <summary>
        /// Returns gVector3.rbbb swizzling (equivalent to gVector3.xzzz).
        /// </summary>
        public gVector4<T> rbbb => new gVector4<T>(x, z, z, z);
        
        /// <summary>
        /// Returns gVector3.yx swizzling.
        /// </summary>
        public gVector2<T> yx => new gVector2<T>(y, x);
        
        /// <summary>
        /// Returns gVector3.gr swizzling (equivalent to gVector3.yx).
        /// </summary>
        public gVector2<T> gr => new gVector2<T>(y, x);
        
        /// <summary>
        /// Returns gVector3.yxx swizzling.
        /// </summary>
        public gVector3<T> yxx => new gVector3<T>(y, x, x);
        
        /// <summary>
        /// Returns gVector3.grr swizzling (equivalent to gVector3.yxx).
        /// </summary>
        public gVector3<T> grr => new gVector3<T>(y, x, x);
        
        /// <summary>
        /// Returns gVector3.yxxx swizzling.
        /// </summary>
        public gVector4<T> yxxx => new gVector4<T>(y, x, x, x);
        
        /// <summary>
        /// Returns gVector3.grrr swizzling (equivalent to gVector3.yxxx).
        /// </summary>
        public gVector4<T> grrr => new gVector4<T>(y, x, x, x);
        
        /// <summary>
        /// Returns gVector3.yxxy swizzling.
        /// </summary>
        public gVector4<T> yxxy => new gVector4<T>(y, x, x, y);
        
        /// <summary>
        /// Returns gVector3.grrg swizzling (equivalent to gVector3.yxxy).
        /// </summary>
        public gVector4<T> grrg => new gVector4<T>(y, x, x, y);
        
        /// <summary>
        /// Returns gVector3.yxxz swizzling.
        /// </summary>
        public gVector4<T> yxxz => new gVector4<T>(y, x, x, z);
        
        /// <summary>
        /// Returns gVector3.grrb swizzling (equivalent to gVector3.yxxz).
        /// </summary>
        public gVector4<T> grrb => new gVector4<T>(y, x, x, z);
        
        /// <summary>
        /// Returns gVector3.yxy swizzling.
        /// </summary>
        public gVector3<T> yxy => new gVector3<T>(y, x, y);
        
        /// <summary>
        /// Returns gVector3.grg swizzling (equivalent to gVector3.yxy).
        /// </summary>
        public gVector3<T> grg => new gVector3<T>(y, x, y);
        
        /// <summary>
        /// Returns gVector3.yxyx swizzling.
        /// </summary>
        public gVector4<T> yxyx => new gVector4<T>(y, x, y, x);
        
        /// <summary>
        /// Returns gVector3.grgr swizzling (equivalent to gVector3.yxyx).
        /// </summary>
        public gVector4<T> grgr => new gVector4<T>(y, x, y, x);
        
        /// <summary>
        /// Returns gVector3.yxyy swizzling.
        /// </summary>
        public gVector4<T> yxyy => new gVector4<T>(y, x, y, y);
        
        /// <summary>
        /// Returns gVector3.grgg swizzling (equivalent to gVector3.yxyy).
        /// </summary>
        public gVector4<T> grgg => new gVector4<T>(y, x, y, y);
        
        /// <summary>
        /// Returns gVector3.yxyz swizzling.
        /// </summary>
        public gVector4<T> yxyz => new gVector4<T>(y, x, y, z);
        
        /// <summary>
        /// Returns gVector3.grgb swizzling (equivalent to gVector3.yxyz).
        /// </summary>
        public gVector4<T> grgb => new gVector4<T>(y, x, y, z);
        
        /// <summary>
        /// Returns gVector3.yxz swizzling.
        /// </summary>
        public gVector3<T> yxz => new gVector3<T>(y, x, z);
        
        /// <summary>
        /// Returns gVector3.grb swizzling (equivalent to gVector3.yxz).
        /// </summary>
        public gVector3<T> grb => new gVector3<T>(y, x, z);
        
        /// <summary>
        /// Returns gVector3.yxzx swizzling.
        /// </summary>
        public gVector4<T> yxzx => new gVector4<T>(y, x, z, x);
        
        /// <summary>
        /// Returns gVector3.grbr swizzling (equivalent to gVector3.yxzx).
        /// </summary>
        public gVector4<T> grbr => new gVector4<T>(y, x, z, x);
        
        /// <summary>
        /// Returns gVector3.yxzy swizzling.
        /// </summary>
        public gVector4<T> yxzy => new gVector4<T>(y, x, z, y);
        
        /// <summary>
        /// Returns gVector3.grbg swizzling (equivalent to gVector3.yxzy).
        /// </summary>
        public gVector4<T> grbg => new gVector4<T>(y, x, z, y);
        
        /// <summary>
        /// Returns gVector3.yxzz swizzling.
        /// </summary>
        public gVector4<T> yxzz => new gVector4<T>(y, x, z, z);
        
        /// <summary>
        /// Returns gVector3.grbb swizzling (equivalent to gVector3.yxzz).
        /// </summary>
        public gVector4<T> grbb => new gVector4<T>(y, x, z, z);
        
        /// <summary>
        /// Returns gVector3.yy swizzling.
        /// </summary>
        public gVector2<T> yy => new gVector2<T>(y, y);
        
        /// <summary>
        /// Returns gVector3.gg swizzling (equivalent to gVector3.yy).
        /// </summary>
        public gVector2<T> gg => new gVector2<T>(y, y);
        
        /// <summary>
        /// Returns gVector3.yyx swizzling.
        /// </summary>
        public gVector3<T> yyx => new gVector3<T>(y, y, x);
        
        /// <summary>
        /// Returns gVector3.ggr swizzling (equivalent to gVector3.yyx).
        /// </summary>
        public gVector3<T> ggr => new gVector3<T>(y, y, x);
        
        /// <summary>
        /// Returns gVector3.yyxx swizzling.
        /// </summary>
        public gVector4<T> yyxx => new gVector4<T>(y, y, x, x);
        
        /// <summary>
        /// Returns gVector3.ggrr swizzling (equivalent to gVector3.yyxx).
        /// </summary>
        public gVector4<T> ggrr => new gVector4<T>(y, y, x, x);
        
        /// <summary>
        /// Returns gVector3.yyxy swizzling.
        /// </summary>
        public gVector4<T> yyxy => new gVector4<T>(y, y, x, y);
        
        /// <summary>
        /// Returns gVector3.ggrg swizzling (equivalent to gVector3.yyxy).
        /// </summary>
        public gVector4<T> ggrg => new gVector4<T>(y, y, x, y);
        
        /// <summary>
        /// Returns gVector3.yyxz swizzling.
        /// </summary>
        public gVector4<T> yyxz => new gVector4<T>(y, y, x, z);
        
        /// <summary>
        /// Returns gVector3.ggrb swizzling (equivalent to gVector3.yyxz).
        /// </summary>
        public gVector4<T> ggrb => new gVector4<T>(y, y, x, z);
        
        /// <summary>
        /// Returns gVector3.yyy swizzling.
        /// </summary>
        public gVector3<T> yyy => new gVector3<T>(y, y, y);
        
        /// <summary>
        /// Returns gVector3.ggg swizzling (equivalent to gVector3.yyy).
        /// </summary>
        public gVector3<T> ggg => new gVector3<T>(y, y, y);
        
        /// <summary>
        /// Returns gVector3.yyyx swizzling.
        /// </summary>
        public gVector4<T> yyyx => new gVector4<T>(y, y, y, x);
        
        /// <summary>
        /// Returns gVector3.gggr swizzling (equivalent to gVector3.yyyx).
        /// </summary>
        public gVector4<T> gggr => new gVector4<T>(y, y, y, x);
        
        /// <summary>
        /// Returns gVector3.yyyy swizzling.
        /// </summary>
        public gVector4<T> yyyy => new gVector4<T>(y, y, y, y);
        
        /// <summary>
        /// Returns gVector3.gggg swizzling (equivalent to gVector3.yyyy).
        /// </summary>
        public gVector4<T> gggg => new gVector4<T>(y, y, y, y);
        
        /// <summary>
        /// Returns gVector3.yyyz swizzling.
        /// </summary>
        public gVector4<T> yyyz => new gVector4<T>(y, y, y, z);
        
        /// <summary>
        /// Returns gVector3.gggb swizzling (equivalent to gVector3.yyyz).
        /// </summary>
        public gVector4<T> gggb => new gVector4<T>(y, y, y, z);
        
        /// <summary>
        /// Returns gVector3.yyz swizzling.
        /// </summary>
        public gVector3<T> yyz => new gVector3<T>(y, y, z);
        
        /// <summary>
        /// Returns gVector3.ggb swizzling (equivalent to gVector3.yyz).
        /// </summary>
        public gVector3<T> ggb => new gVector3<T>(y, y, z);
        
        /// <summary>
        /// Returns gVector3.yyzx swizzling.
        /// </summary>
        public gVector4<T> yyzx => new gVector4<T>(y, y, z, x);
        
        /// <summary>
        /// Returns gVector3.ggbr swizzling (equivalent to gVector3.yyzx).
        /// </summary>
        public gVector4<T> ggbr => new gVector4<T>(y, y, z, x);
        
        /// <summary>
        /// Returns gVector3.yyzy swizzling.
        /// </summary>
        public gVector4<T> yyzy => new gVector4<T>(y, y, z, y);
        
        /// <summary>
        /// Returns gVector3.ggbg swizzling (equivalent to gVector3.yyzy).
        /// </summary>
        public gVector4<T> ggbg => new gVector4<T>(y, y, z, y);
        
        /// <summary>
        /// Returns gVector3.yyzz swizzling.
        /// </summary>
        public gVector4<T> yyzz => new gVector4<T>(y, y, z, z);
        
        /// <summary>
        /// Returns gVector3.ggbb swizzling (equivalent to gVector3.yyzz).
        /// </summary>
        public gVector4<T> ggbb => new gVector4<T>(y, y, z, z);
        
        /// <summary>
        /// Returns gVector3.yz swizzling.
        /// </summary>
        public gVector2<T> yz => new gVector2<T>(y, z);
        
        /// <summary>
        /// Returns gVector3.gb swizzling (equivalent to gVector3.yz).
        /// </summary>
        public gVector2<T> gb => new gVector2<T>(y, z);
        
        /// <summary>
        /// Returns gVector3.yzx swizzling.
        /// </summary>
        public gVector3<T> yzx => new gVector3<T>(y, z, x);
        
        /// <summary>
        /// Returns gVector3.gbr swizzling (equivalent to gVector3.yzx).
        /// </summary>
        public gVector3<T> gbr => new gVector3<T>(y, z, x);
        
        /// <summary>
        /// Returns gVector3.yzxx swizzling.
        /// </summary>
        public gVector4<T> yzxx => new gVector4<T>(y, z, x, x);
        
        /// <summary>
        /// Returns gVector3.gbrr swizzling (equivalent to gVector3.yzxx).
        /// </summary>
        public gVector4<T> gbrr => new gVector4<T>(y, z, x, x);
        
        /// <summary>
        /// Returns gVector3.yzxy swizzling.
        /// </summary>
        public gVector4<T> yzxy => new gVector4<T>(y, z, x, y);
        
        /// <summary>
        /// Returns gVector3.gbrg swizzling (equivalent to gVector3.yzxy).
        /// </summary>
        public gVector4<T> gbrg => new gVector4<T>(y, z, x, y);
        
        /// <summary>
        /// Returns gVector3.yzxz swizzling.
        /// </summary>
        public gVector4<T> yzxz => new gVector4<T>(y, z, x, z);
        
        /// <summary>
        /// Returns gVector3.gbrb swizzling (equivalent to gVector3.yzxz).
        /// </summary>
        public gVector4<T> gbrb => new gVector4<T>(y, z, x, z);
        
        /// <summary>
        /// Returns gVector3.yzy swizzling.
        /// </summary>
        public gVector3<T> yzy => new gVector3<T>(y, z, y);
        
        /// <summary>
        /// Returns gVector3.gbg swizzling (equivalent to gVector3.yzy).
        /// </summary>
        public gVector3<T> gbg => new gVector3<T>(y, z, y);
        
        /// <summary>
        /// Returns gVector3.yzyx swizzling.
        /// </summary>
        public gVector4<T> yzyx => new gVector4<T>(y, z, y, x);
        
        /// <summary>
        /// Returns gVector3.gbgr swizzling (equivalent to gVector3.yzyx).
        /// </summary>
        public gVector4<T> gbgr => new gVector4<T>(y, z, y, x);
        
        /// <summary>
        /// Returns gVector3.yzyy swizzling.
        /// </summary>
        public gVector4<T> yzyy => new gVector4<T>(y, z, y, y);
        
        /// <summary>
        /// Returns gVector3.gbgg swizzling (equivalent to gVector3.yzyy).
        /// </summary>
        public gVector4<T> gbgg => new gVector4<T>(y, z, y, y);
        
        /// <summary>
        /// Returns gVector3.yzyz swizzling.
        /// </summary>
        public gVector4<T> yzyz => new gVector4<T>(y, z, y, z);
        
        /// <summary>
        /// Returns gVector3.gbgb swizzling (equivalent to gVector3.yzyz).
        /// </summary>
        public gVector4<T> gbgb => new gVector4<T>(y, z, y, z);
        
        /// <summary>
        /// Returns gVector3.yzz swizzling.
        /// </summary>
        public gVector3<T> yzz => new gVector3<T>(y, z, z);
        
        /// <summary>
        /// Returns gVector3.gbb swizzling (equivalent to gVector3.yzz).
        /// </summary>
        public gVector3<T> gbb => new gVector3<T>(y, z, z);
        
        /// <summary>
        /// Returns gVector3.yzzx swizzling.
        /// </summary>
        public gVector4<T> yzzx => new gVector4<T>(y, z, z, x);
        
        /// <summary>
        /// Returns gVector3.gbbr swizzling (equivalent to gVector3.yzzx).
        /// </summary>
        public gVector4<T> gbbr => new gVector4<T>(y, z, z, x);
        
        /// <summary>
        /// Returns gVector3.yzzy swizzling.
        /// </summary>
        public gVector4<T> yzzy => new gVector4<T>(y, z, z, y);
        
        /// <summary>
        /// Returns gVector3.gbbg swizzling (equivalent to gVector3.yzzy).
        /// </summary>
        public gVector4<T> gbbg => new gVector4<T>(y, z, z, y);
        
        /// <summary>
        /// Returns gVector3.yzzz swizzling.
        /// </summary>
        public gVector4<T> yzzz => new gVector4<T>(y, z, z, z);
        
        /// <summary>
        /// Returns gVector3.gbbb swizzling (equivalent to gVector3.yzzz).
        /// </summary>
        public gVector4<T> gbbb => new gVector4<T>(y, z, z, z);
        
        /// <summary>
        /// Returns gVector3.zx swizzling.
        /// </summary>
        public gVector2<T> zx => new gVector2<T>(z, x);
        
        /// <summary>
        /// Returns gVector3.br swizzling (equivalent to gVector3.zx).
        /// </summary>
        public gVector2<T> br => new gVector2<T>(z, x);
        
        /// <summary>
        /// Returns gVector3.zxx swizzling.
        /// </summary>
        public gVector3<T> zxx => new gVector3<T>(z, x, x);
        
        /// <summary>
        /// Returns gVector3.brr swizzling (equivalent to gVector3.zxx).
        /// </summary>
        public gVector3<T> brr => new gVector3<T>(z, x, x);
        
        /// <summary>
        /// Returns gVector3.zxxx swizzling.
        /// </summary>
        public gVector4<T> zxxx => new gVector4<T>(z, x, x, x);
        
        /// <summary>
        /// Returns gVector3.brrr swizzling (equivalent to gVector3.zxxx).
        /// </summary>
        public gVector4<T> brrr => new gVector4<T>(z, x, x, x);
        
        /// <summary>
        /// Returns gVector3.zxxy swizzling.
        /// </summary>
        public gVector4<T> zxxy => new gVector4<T>(z, x, x, y);
        
        /// <summary>
        /// Returns gVector3.brrg swizzling (equivalent to gVector3.zxxy).
        /// </summary>
        public gVector4<T> brrg => new gVector4<T>(z, x, x, y);
        
        /// <summary>
        /// Returns gVector3.zxxz swizzling.
        /// </summary>
        public gVector4<T> zxxz => new gVector4<T>(z, x, x, z);
        
        /// <summary>
        /// Returns gVector3.brrb swizzling (equivalent to gVector3.zxxz).
        /// </summary>
        public gVector4<T> brrb => new gVector4<T>(z, x, x, z);
        
        /// <summary>
        /// Returns gVector3.zxy swizzling.
        /// </summary>
        public gVector3<T> zxy => new gVector3<T>(z, x, y);
        
        /// <summary>
        /// Returns gVector3.brg swizzling (equivalent to gVector3.zxy).
        /// </summary>
        public gVector3<T> brg => new gVector3<T>(z, x, y);
        
        /// <summary>
        /// Returns gVector3.zxyx swizzling.
        /// </summary>
        public gVector4<T> zxyx => new gVector4<T>(z, x, y, x);
        
        /// <summary>
        /// Returns gVector3.brgr swizzling (equivalent to gVector3.zxyx).
        /// </summary>
        public gVector4<T> brgr => new gVector4<T>(z, x, y, x);
        
        /// <summary>
        /// Returns gVector3.zxyy swizzling.
        /// </summary>
        public gVector4<T> zxyy => new gVector4<T>(z, x, y, y);
        
        /// <summary>
        /// Returns gVector3.brgg swizzling (equivalent to gVector3.zxyy).
        /// </summary>
        public gVector4<T> brgg => new gVector4<T>(z, x, y, y);
        
        /// <summary>
        /// Returns gVector3.zxyz swizzling.
        /// </summary>
        public gVector4<T> zxyz => new gVector4<T>(z, x, y, z);
        
        /// <summary>
        /// Returns gVector3.brgb swizzling (equivalent to gVector3.zxyz).
        /// </summary>
        public gVector4<T> brgb => new gVector4<T>(z, x, y, z);
        
        /// <summary>
        /// Returns gVector3.zxz swizzling.
        /// </summary>
        public gVector3<T> zxz => new gVector3<T>(z, x, z);
        
        /// <summary>
        /// Returns gVector3.brb swizzling (equivalent to gVector3.zxz).
        /// </summary>
        public gVector3<T> brb => new gVector3<T>(z, x, z);
        
        /// <summary>
        /// Returns gVector3.zxzx swizzling.
        /// </summary>
        public gVector4<T> zxzx => new gVector4<T>(z, x, z, x);
        
        /// <summary>
        /// Returns gVector3.brbr swizzling (equivalent to gVector3.zxzx).
        /// </summary>
        public gVector4<T> brbr => new gVector4<T>(z, x, z, x);
        
        /// <summary>
        /// Returns gVector3.zxzy swizzling.
        /// </summary>
        public gVector4<T> zxzy => new gVector4<T>(z, x, z, y);
        
        /// <summary>
        /// Returns gVector3.brbg swizzling (equivalent to gVector3.zxzy).
        /// </summary>
        public gVector4<T> brbg => new gVector4<T>(z, x, z, y);
        
        /// <summary>
        /// Returns gVector3.zxzz swizzling.
        /// </summary>
        public gVector4<T> zxzz => new gVector4<T>(z, x, z, z);
        
        /// <summary>
        /// Returns gVector3.brbb swizzling (equivalent to gVector3.zxzz).
        /// </summary>
        public gVector4<T> brbb => new gVector4<T>(z, x, z, z);
        
        /// <summary>
        /// Returns gVector3.zy swizzling.
        /// </summary>
        public gVector2<T> zy => new gVector2<T>(z, y);
        
        /// <summary>
        /// Returns gVector3.bg swizzling (equivalent to gVector3.zy).
        /// </summary>
        public gVector2<T> bg => new gVector2<T>(z, y);
        
        /// <summary>
        /// Returns gVector3.zyx swizzling.
        /// </summary>
        public gVector3<T> zyx => new gVector3<T>(z, y, x);
        
        /// <summary>
        /// Returns gVector3.bgr swizzling (equivalent to gVector3.zyx).
        /// </summary>
        public gVector3<T> bgr => new gVector3<T>(z, y, x);
        
        /// <summary>
        /// Returns gVector3.zyxx swizzling.
        /// </summary>
        public gVector4<T> zyxx => new gVector4<T>(z, y, x, x);
        
        /// <summary>
        /// Returns gVector3.bgrr swizzling (equivalent to gVector3.zyxx).
        /// </summary>
        public gVector4<T> bgrr => new gVector4<T>(z, y, x, x);
        
        /// <summary>
        /// Returns gVector3.zyxy swizzling.
        /// </summary>
        public gVector4<T> zyxy => new gVector4<T>(z, y, x, y);
        
        /// <summary>
        /// Returns gVector3.bgrg swizzling (equivalent to gVector3.zyxy).
        /// </summary>
        public gVector4<T> bgrg => new gVector4<T>(z, y, x, y);
        
        /// <summary>
        /// Returns gVector3.zyxz swizzling.
        /// </summary>
        public gVector4<T> zyxz => new gVector4<T>(z, y, x, z);
        
        /// <summary>
        /// Returns gVector3.bgrb swizzling (equivalent to gVector3.zyxz).
        /// </summary>
        public gVector4<T> bgrb => new gVector4<T>(z, y, x, z);
        
        /// <summary>
        /// Returns gVector3.zyy swizzling.
        /// </summary>
        public gVector3<T> zyy => new gVector3<T>(z, y, y);
        
        /// <summary>
        /// Returns gVector3.bgg swizzling (equivalent to gVector3.zyy).
        /// </summary>
        public gVector3<T> bgg => new gVector3<T>(z, y, y);
        
        /// <summary>
        /// Returns gVector3.zyyx swizzling.
        /// </summary>
        public gVector4<T> zyyx => new gVector4<T>(z, y, y, x);
        
        /// <summary>
        /// Returns gVector3.bggr swizzling (equivalent to gVector3.zyyx).
        /// </summary>
        public gVector4<T> bggr => new gVector4<T>(z, y, y, x);
        
        /// <summary>
        /// Returns gVector3.zyyy swizzling.
        /// </summary>
        public gVector4<T> zyyy => new gVector4<T>(z, y, y, y);
        
        /// <summary>
        /// Returns gVector3.bggg swizzling (equivalent to gVector3.zyyy).
        /// </summary>
        public gVector4<T> bggg => new gVector4<T>(z, y, y, y);
        
        /// <summary>
        /// Returns gVector3.zyyz swizzling.
        /// </summary>
        public gVector4<T> zyyz => new gVector4<T>(z, y, y, z);
        
        /// <summary>
        /// Returns gVector3.bggb swizzling (equivalent to gVector3.zyyz).
        /// </summary>
        public gVector4<T> bggb => new gVector4<T>(z, y, y, z);
        
        /// <summary>
        /// Returns gVector3.zyz swizzling.
        /// </summary>
        public gVector3<T> zyz => new gVector3<T>(z, y, z);
        
        /// <summary>
        /// Returns gVector3.bgb swizzling (equivalent to gVector3.zyz).
        /// </summary>
        public gVector3<T> bgb => new gVector3<T>(z, y, z);
        
        /// <summary>
        /// Returns gVector3.zyzx swizzling.
        /// </summary>
        public gVector4<T> zyzx => new gVector4<T>(z, y, z, x);
        
        /// <summary>
        /// Returns gVector3.bgbr swizzling (equivalent to gVector3.zyzx).
        /// </summary>
        public gVector4<T> bgbr => new gVector4<T>(z, y, z, x);
        
        /// <summary>
        /// Returns gVector3.zyzy swizzling.
        /// </summary>
        public gVector4<T> zyzy => new gVector4<T>(z, y, z, y);
        
        /// <summary>
        /// Returns gVector3.bgbg swizzling (equivalent to gVector3.zyzy).
        /// </summary>
        public gVector4<T> bgbg => new gVector4<T>(z, y, z, y);
        
        /// <summary>
        /// Returns gVector3.zyzz swizzling.
        /// </summary>
        public gVector4<T> zyzz => new gVector4<T>(z, y, z, z);
        
        /// <summary>
        /// Returns gVector3.bgbb swizzling (equivalent to gVector3.zyzz).
        /// </summary>
        public gVector4<T> bgbb => new gVector4<T>(z, y, z, z);
        
        /// <summary>
        /// Returns gVector3.zz swizzling.
        /// </summary>
        public gVector2<T> zz => new gVector2<T>(z, z);
        
        /// <summary>
        /// Returns gVector3.bb swizzling (equivalent to gVector3.zz).
        /// </summary>
        public gVector2<T> bb => new gVector2<T>(z, z);
        
        /// <summary>
        /// Returns gVector3.zzx swizzling.
        /// </summary>
        public gVector3<T> zzx => new gVector3<T>(z, z, x);
        
        /// <summary>
        /// Returns gVector3.bbr swizzling (equivalent to gVector3.zzx).
        /// </summary>
        public gVector3<T> bbr => new gVector3<T>(z, z, x);
        
        /// <summary>
        /// Returns gVector3.zzxx swizzling.
        /// </summary>
        public gVector4<T> zzxx => new gVector4<T>(z, z, x, x);
        
        /// <summary>
        /// Returns gVector3.bbrr swizzling (equivalent to gVector3.zzxx).
        /// </summary>
        public gVector4<T> bbrr => new gVector4<T>(z, z, x, x);
        
        /// <summary>
        /// Returns gVector3.zzxy swizzling.
        /// </summary>
        public gVector4<T> zzxy => new gVector4<T>(z, z, x, y);
        
        /// <summary>
        /// Returns gVector3.bbrg swizzling (equivalent to gVector3.zzxy).
        /// </summary>
        public gVector4<T> bbrg => new gVector4<T>(z, z, x, y);
        
        /// <summary>
        /// Returns gVector3.zzxz swizzling.
        /// </summary>
        public gVector4<T> zzxz => new gVector4<T>(z, z, x, z);
        
        /// <summary>
        /// Returns gVector3.bbrb swizzling (equivalent to gVector3.zzxz).
        /// </summary>
        public gVector4<T> bbrb => new gVector4<T>(z, z, x, z);
        
        /// <summary>
        /// Returns gVector3.zzy swizzling.
        /// </summary>
        public gVector3<T> zzy => new gVector3<T>(z, z, y);
        
        /// <summary>
        /// Returns gVector3.bbg swizzling (equivalent to gVector3.zzy).
        /// </summary>
        public gVector3<T> bbg => new gVector3<T>(z, z, y);
        
        /// <summary>
        /// Returns gVector3.zzyx swizzling.
        /// </summary>
        public gVector4<T> zzyx => new gVector4<T>(z, z, y, x);
        
        /// <summary>
        /// Returns gVector3.bbgr swizzling (equivalent to gVector3.zzyx).
        /// </summary>
        public gVector4<T> bbgr => new gVector4<T>(z, z, y, x);
        
        /// <summary>
        /// Returns gVector3.zzyy swizzling.
        /// </summary>
        public gVector4<T> zzyy => new gVector4<T>(z, z, y, y);
        
        /// <summary>
        /// Returns gVector3.bbgg swizzling (equivalent to gVector3.zzyy).
        /// </summary>
        public gVector4<T> bbgg => new gVector4<T>(z, z, y, y);
        
        /// <summary>
        /// Returns gVector3.zzyz swizzling.
        /// </summary>
        public gVector4<T> zzyz => new gVector4<T>(z, z, y, z);
        
        /// <summary>
        /// Returns gVector3.bbgb swizzling (equivalent to gVector3.zzyz).
        /// </summary>
        public gVector4<T> bbgb => new gVector4<T>(z, z, y, z);
        
        /// <summary>
        /// Returns gVector3.zzz swizzling.
        /// </summary>
        public gVector3<T> zzz => new gVector3<T>(z, z, z);
        
        /// <summary>
        /// Returns gVector3.bbb swizzling (equivalent to gVector3.zzz).
        /// </summary>
        public gVector3<T> bbb => new gVector3<T>(z, z, z);
        
        /// <summary>
        /// Returns gVector3.zzzx swizzling.
        /// </summary>
        public gVector4<T> zzzx => new gVector4<T>(z, z, z, x);
        
        /// <summary>
        /// Returns gVector3.bbbr swizzling (equivalent to gVector3.zzzx).
        /// </summary>
        public gVector4<T> bbbr => new gVector4<T>(z, z, z, x);
        
        /// <summary>
        /// Returns gVector3.zzzy swizzling.
        /// </summary>
        public gVector4<T> zzzy => new gVector4<T>(z, z, z, y);
        
        /// <summary>
        /// Returns gVector3.bbbg swizzling (equivalent to gVector3.zzzy).
        /// </summary>
        public gVector4<T> bbbg => new gVector4<T>(z, z, z, y);
        
        /// <summary>
        /// Returns gVector3.zzzz swizzling.
        /// </summary>
        public gVector4<T> zzzz => new gVector4<T>(z, z, z, z);
        
        /// <summary>
        /// Returns gVector3.bbbb swizzling (equivalent to gVector3.zzzz).
        /// </summary>
        public gVector4<T> bbbb => new gVector4<T>(z, z, z, z);

        #endregion

    }
}
