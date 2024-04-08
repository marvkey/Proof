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
    /// Temporary Vector of type long with 4 components, used for implementing swizzling for lVector4.
    /// </summary>
    [RegisterCoreClassStruct]
   
    [StructLayout(LayoutKind.Sequential)]
    public struct swizzle_lVector4
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
        
        /// <summary>
        /// w-component
        /// </summary>
        
        internal readonly long w;

        #endregion


        #region Constructors
        
        /// <summary>
        /// Constructor for swizzle_lVector4.
        /// </summary>
        internal swizzle_lVector4(long x, long y, long z, long w)
        {
            this.x = x;
            this.y = y;
            this.z = z;
            this.w = w;
        }

        #endregion


        #region Properties
        
        /// <summary>
        /// Returns lVector4.xx swizzling.
        /// </summary>
        public lVector2 xx => new lVector2(x, x);
        
        /// <summary>
        /// Returns lVector4.rr swizzling (equivalent to lVector4.xx).
        /// </summary>
        public lVector2 rr => new lVector2(x, x);
        
        /// <summary>
        /// Returns lVector4.xxx swizzling.
        /// </summary>
        public lVector3 xxx => new lVector3(x, x, x);
        
        /// <summary>
        /// Returns lVector4.rrr swizzling (equivalent to lVector4.xxx).
        /// </summary>
        public lVector3 rrr => new lVector3(x, x, x);
        
        /// <summary>
        /// Returns lVector4.xxxx swizzling.
        /// </summary>
        public lVector4 xxxx => new lVector4(x, x, x, x);
        
        /// <summary>
        /// Returns lVector4.rrrr swizzling (equivalent to lVector4.xxxx).
        /// </summary>
        public lVector4 rrrr => new lVector4(x, x, x, x);
        
        /// <summary>
        /// Returns lVector4.xxxy swizzling.
        /// </summary>
        public lVector4 xxxy => new lVector4(x, x, x, y);
        
        /// <summary>
        /// Returns lVector4.rrrg swizzling (equivalent to lVector4.xxxy).
        /// </summary>
        public lVector4 rrrg => new lVector4(x, x, x, y);
        
        /// <summary>
        /// Returns lVector4.xxxz swizzling.
        /// </summary>
        public lVector4 xxxz => new lVector4(x, x, x, z);
        
        /// <summary>
        /// Returns lVector4.rrrb swizzling (equivalent to lVector4.xxxz).
        /// </summary>
        public lVector4 rrrb => new lVector4(x, x, x, z);
        
        /// <summary>
        /// Returns lVector4.xxxw swizzling.
        /// </summary>
        public lVector4 xxxw => new lVector4(x, x, x, w);
        
        /// <summary>
        /// Returns lVector4.rrra swizzling (equivalent to lVector4.xxxw).
        /// </summary>
        public lVector4 rrra => new lVector4(x, x, x, w);
        
        /// <summary>
        /// Returns lVector4.xxy swizzling.
        /// </summary>
        public lVector3 xxy => new lVector3(x, x, y);
        
        /// <summary>
        /// Returns lVector4.rrg swizzling (equivalent to lVector4.xxy).
        /// </summary>
        public lVector3 rrg => new lVector3(x, x, y);
        
        /// <summary>
        /// Returns lVector4.xxyx swizzling.
        /// </summary>
        public lVector4 xxyx => new lVector4(x, x, y, x);
        
        /// <summary>
        /// Returns lVector4.rrgr swizzling (equivalent to lVector4.xxyx).
        /// </summary>
        public lVector4 rrgr => new lVector4(x, x, y, x);
        
        /// <summary>
        /// Returns lVector4.xxyy swizzling.
        /// </summary>
        public lVector4 xxyy => new lVector4(x, x, y, y);
        
        /// <summary>
        /// Returns lVector4.rrgg swizzling (equivalent to lVector4.xxyy).
        /// </summary>
        public lVector4 rrgg => new lVector4(x, x, y, y);
        
        /// <summary>
        /// Returns lVector4.xxyz swizzling.
        /// </summary>
        public lVector4 xxyz => new lVector4(x, x, y, z);
        
        /// <summary>
        /// Returns lVector4.rrgb swizzling (equivalent to lVector4.xxyz).
        /// </summary>
        public lVector4 rrgb => new lVector4(x, x, y, z);
        
        /// <summary>
        /// Returns lVector4.xxyw swizzling.
        /// </summary>
        public lVector4 xxyw => new lVector4(x, x, y, w);
        
        /// <summary>
        /// Returns lVector4.rrga swizzling (equivalent to lVector4.xxyw).
        /// </summary>
        public lVector4 rrga => new lVector4(x, x, y, w);
        
        /// <summary>
        /// Returns lVector4.xxz swizzling.
        /// </summary>
        public lVector3 xxz => new lVector3(x, x, z);
        
        /// <summary>
        /// Returns lVector4.rrb swizzling (equivalent to lVector4.xxz).
        /// </summary>
        public lVector3 rrb => new lVector3(x, x, z);
        
        /// <summary>
        /// Returns lVector4.xxzx swizzling.
        /// </summary>
        public lVector4 xxzx => new lVector4(x, x, z, x);
        
        /// <summary>
        /// Returns lVector4.rrbr swizzling (equivalent to lVector4.xxzx).
        /// </summary>
        public lVector4 rrbr => new lVector4(x, x, z, x);
        
        /// <summary>
        /// Returns lVector4.xxzy swizzling.
        /// </summary>
        public lVector4 xxzy => new lVector4(x, x, z, y);
        
        /// <summary>
        /// Returns lVector4.rrbg swizzling (equivalent to lVector4.xxzy).
        /// </summary>
        public lVector4 rrbg => new lVector4(x, x, z, y);
        
        /// <summary>
        /// Returns lVector4.xxzz swizzling.
        /// </summary>
        public lVector4 xxzz => new lVector4(x, x, z, z);
        
        /// <summary>
        /// Returns lVector4.rrbb swizzling (equivalent to lVector4.xxzz).
        /// </summary>
        public lVector4 rrbb => new lVector4(x, x, z, z);
        
        /// <summary>
        /// Returns lVector4.xxzw swizzling.
        /// </summary>
        public lVector4 xxzw => new lVector4(x, x, z, w);
        
        /// <summary>
        /// Returns lVector4.rrba swizzling (equivalent to lVector4.xxzw).
        /// </summary>
        public lVector4 rrba => new lVector4(x, x, z, w);
        
        /// <summary>
        /// Returns lVector4.xxw swizzling.
        /// </summary>
        public lVector3 xxw => new lVector3(x, x, w);
        
        /// <summary>
        /// Returns lVector4.rra swizzling (equivalent to lVector4.xxw).
        /// </summary>
        public lVector3 rra => new lVector3(x, x, w);
        
        /// <summary>
        /// Returns lVector4.xxwx swizzling.
        /// </summary>
        public lVector4 xxwx => new lVector4(x, x, w, x);
        
        /// <summary>
        /// Returns lVector4.rrar swizzling (equivalent to lVector4.xxwx).
        /// </summary>
        public lVector4 rrar => new lVector4(x, x, w, x);
        
        /// <summary>
        /// Returns lVector4.xxwy swizzling.
        /// </summary>
        public lVector4 xxwy => new lVector4(x, x, w, y);
        
        /// <summary>
        /// Returns lVector4.rrag swizzling (equivalent to lVector4.xxwy).
        /// </summary>
        public lVector4 rrag => new lVector4(x, x, w, y);
        
        /// <summary>
        /// Returns lVector4.xxwz swizzling.
        /// </summary>
        public lVector4 xxwz => new lVector4(x, x, w, z);
        
        /// <summary>
        /// Returns lVector4.rrab swizzling (equivalent to lVector4.xxwz).
        /// </summary>
        public lVector4 rrab => new lVector4(x, x, w, z);
        
        /// <summary>
        /// Returns lVector4.xxww swizzling.
        /// </summary>
        public lVector4 xxww => new lVector4(x, x, w, w);
        
        /// <summary>
        /// Returns lVector4.rraa swizzling (equivalent to lVector4.xxww).
        /// </summary>
        public lVector4 rraa => new lVector4(x, x, w, w);
        
        /// <summary>
        /// Returns lVector4.xy swizzling.
        /// </summary>
        public lVector2 xy => new lVector2(x, y);
        
        /// <summary>
        /// Returns lVector4.rg swizzling (equivalent to lVector4.xy).
        /// </summary>
        public lVector2 rg => new lVector2(x, y);
        
        /// <summary>
        /// Returns lVector4.xyx swizzling.
        /// </summary>
        public lVector3 xyx => new lVector3(x, y, x);
        
        /// <summary>
        /// Returns lVector4.rgr swizzling (equivalent to lVector4.xyx).
        /// </summary>
        public lVector3 rgr => new lVector3(x, y, x);
        
        /// <summary>
        /// Returns lVector4.xyxx swizzling.
        /// </summary>
        public lVector4 xyxx => new lVector4(x, y, x, x);
        
        /// <summary>
        /// Returns lVector4.rgrr swizzling (equivalent to lVector4.xyxx).
        /// </summary>
        public lVector4 rgrr => new lVector4(x, y, x, x);
        
        /// <summary>
        /// Returns lVector4.xyxy swizzling.
        /// </summary>
        public lVector4 xyxy => new lVector4(x, y, x, y);
        
        /// <summary>
        /// Returns lVector4.rgrg swizzling (equivalent to lVector4.xyxy).
        /// </summary>
        public lVector4 rgrg => new lVector4(x, y, x, y);
        
        /// <summary>
        /// Returns lVector4.xyxz swizzling.
        /// </summary>
        public lVector4 xyxz => new lVector4(x, y, x, z);
        
        /// <summary>
        /// Returns lVector4.rgrb swizzling (equivalent to lVector4.xyxz).
        /// </summary>
        public lVector4 rgrb => new lVector4(x, y, x, z);
        
        /// <summary>
        /// Returns lVector4.xyxw swizzling.
        /// </summary>
        public lVector4 xyxw => new lVector4(x, y, x, w);
        
        /// <summary>
        /// Returns lVector4.rgra swizzling (equivalent to lVector4.xyxw).
        /// </summary>
        public lVector4 rgra => new lVector4(x, y, x, w);
        
        /// <summary>
        /// Returns lVector4.xyy swizzling.
        /// </summary>
        public lVector3 xyy => new lVector3(x, y, y);
        
        /// <summary>
        /// Returns lVector4.rgg swizzling (equivalent to lVector4.xyy).
        /// </summary>
        public lVector3 rgg => new lVector3(x, y, y);
        
        /// <summary>
        /// Returns lVector4.xyyx swizzling.
        /// </summary>
        public lVector4 xyyx => new lVector4(x, y, y, x);
        
        /// <summary>
        /// Returns lVector4.rggr swizzling (equivalent to lVector4.xyyx).
        /// </summary>
        public lVector4 rggr => new lVector4(x, y, y, x);
        
        /// <summary>
        /// Returns lVector4.xyyy swizzling.
        /// </summary>
        public lVector4 xyyy => new lVector4(x, y, y, y);
        
        /// <summary>
        /// Returns lVector4.rggg swizzling (equivalent to lVector4.xyyy).
        /// </summary>
        public lVector4 rggg => new lVector4(x, y, y, y);
        
        /// <summary>
        /// Returns lVector4.xyyz swizzling.
        /// </summary>
        public lVector4 xyyz => new lVector4(x, y, y, z);
        
        /// <summary>
        /// Returns lVector4.rggb swizzling (equivalent to lVector4.xyyz).
        /// </summary>
        public lVector4 rggb => new lVector4(x, y, y, z);
        
        /// <summary>
        /// Returns lVector4.xyyw swizzling.
        /// </summary>
        public lVector4 xyyw => new lVector4(x, y, y, w);
        
        /// <summary>
        /// Returns lVector4.rgga swizzling (equivalent to lVector4.xyyw).
        /// </summary>
        public lVector4 rgga => new lVector4(x, y, y, w);
        
        /// <summary>
        /// Returns lVector4.xyz swizzling.
        /// </summary>
        public lVector3 xyz => new lVector3(x, y, z);
        
        /// <summary>
        /// Returns lVector4.rgb swizzling (equivalent to lVector4.xyz).
        /// </summary>
        public lVector3 rgb => new lVector3(x, y, z);
        
        /// <summary>
        /// Returns lVector4.xyzx swizzling.
        /// </summary>
        public lVector4 xyzx => new lVector4(x, y, z, x);
        
        /// <summary>
        /// Returns lVector4.rgbr swizzling (equivalent to lVector4.xyzx).
        /// </summary>
        public lVector4 rgbr => new lVector4(x, y, z, x);
        
        /// <summary>
        /// Returns lVector4.xyzy swizzling.
        /// </summary>
        public lVector4 xyzy => new lVector4(x, y, z, y);
        
        /// <summary>
        /// Returns lVector4.rgbg swizzling (equivalent to lVector4.xyzy).
        /// </summary>
        public lVector4 rgbg => new lVector4(x, y, z, y);
        
        /// <summary>
        /// Returns lVector4.xyzz swizzling.
        /// </summary>
        public lVector4 xyzz => new lVector4(x, y, z, z);
        
        /// <summary>
        /// Returns lVector4.rgbb swizzling (equivalent to lVector4.xyzz).
        /// </summary>
        public lVector4 rgbb => new lVector4(x, y, z, z);
        
        /// <summary>
        /// Returns lVector4.xyzw swizzling.
        /// </summary>
        public lVector4 xyzw => new lVector4(x, y, z, w);
        
        /// <summary>
        /// Returns lVector4.rgba swizzling (equivalent to lVector4.xyzw).
        /// </summary>
        public lVector4 rgba => new lVector4(x, y, z, w);
        
        /// <summary>
        /// Returns lVector4.xyw swizzling.
        /// </summary>
        public lVector3 xyw => new lVector3(x, y, w);
        
        /// <summary>
        /// Returns lVector4.rga swizzling (equivalent to lVector4.xyw).
        /// </summary>
        public lVector3 rga => new lVector3(x, y, w);
        
        /// <summary>
        /// Returns lVector4.xywx swizzling.
        /// </summary>
        public lVector4 xywx => new lVector4(x, y, w, x);
        
        /// <summary>
        /// Returns lVector4.rgar swizzling (equivalent to lVector4.xywx).
        /// </summary>
        public lVector4 rgar => new lVector4(x, y, w, x);
        
        /// <summary>
        /// Returns lVector4.xywy swizzling.
        /// </summary>
        public lVector4 xywy => new lVector4(x, y, w, y);
        
        /// <summary>
        /// Returns lVector4.rgag swizzling (equivalent to lVector4.xywy).
        /// </summary>
        public lVector4 rgag => new lVector4(x, y, w, y);
        
        /// <summary>
        /// Returns lVector4.xywz swizzling.
        /// </summary>
        public lVector4 xywz => new lVector4(x, y, w, z);
        
        /// <summary>
        /// Returns lVector4.rgab swizzling (equivalent to lVector4.xywz).
        /// </summary>
        public lVector4 rgab => new lVector4(x, y, w, z);
        
        /// <summary>
        /// Returns lVector4.xyww swizzling.
        /// </summary>
        public lVector4 xyww => new lVector4(x, y, w, w);
        
        /// <summary>
        /// Returns lVector4.rgaa swizzling (equivalent to lVector4.xyww).
        /// </summary>
        public lVector4 rgaa => new lVector4(x, y, w, w);
        
        /// <summary>
        /// Returns lVector4.xz swizzling.
        /// </summary>
        public lVector2 xz => new lVector2(x, z);
        
        /// <summary>
        /// Returns lVector4.rb swizzling (equivalent to lVector4.xz).
        /// </summary>
        public lVector2 rb => new lVector2(x, z);
        
        /// <summary>
        /// Returns lVector4.xzx swizzling.
        /// </summary>
        public lVector3 xzx => new lVector3(x, z, x);
        
        /// <summary>
        /// Returns lVector4.rbr swizzling (equivalent to lVector4.xzx).
        /// </summary>
        public lVector3 rbr => new lVector3(x, z, x);
        
        /// <summary>
        /// Returns lVector4.xzxx swizzling.
        /// </summary>
        public lVector4 xzxx => new lVector4(x, z, x, x);
        
        /// <summary>
        /// Returns lVector4.rbrr swizzling (equivalent to lVector4.xzxx).
        /// </summary>
        public lVector4 rbrr => new lVector4(x, z, x, x);
        
        /// <summary>
        /// Returns lVector4.xzxy swizzling.
        /// </summary>
        public lVector4 xzxy => new lVector4(x, z, x, y);
        
        /// <summary>
        /// Returns lVector4.rbrg swizzling (equivalent to lVector4.xzxy).
        /// </summary>
        public lVector4 rbrg => new lVector4(x, z, x, y);
        
        /// <summary>
        /// Returns lVector4.xzxz swizzling.
        /// </summary>
        public lVector4 xzxz => new lVector4(x, z, x, z);
        
        /// <summary>
        /// Returns lVector4.rbrb swizzling (equivalent to lVector4.xzxz).
        /// </summary>
        public lVector4 rbrb => new lVector4(x, z, x, z);
        
        /// <summary>
        /// Returns lVector4.xzxw swizzling.
        /// </summary>
        public lVector4 xzxw => new lVector4(x, z, x, w);
        
        /// <summary>
        /// Returns lVector4.rbra swizzling (equivalent to lVector4.xzxw).
        /// </summary>
        public lVector4 rbra => new lVector4(x, z, x, w);
        
        /// <summary>
        /// Returns lVector4.xzy swizzling.
        /// </summary>
        public lVector3 xzy => new lVector3(x, z, y);
        
        /// <summary>
        /// Returns lVector4.rbg swizzling (equivalent to lVector4.xzy).
        /// </summary>
        public lVector3 rbg => new lVector3(x, z, y);
        
        /// <summary>
        /// Returns lVector4.xzyx swizzling.
        /// </summary>
        public lVector4 xzyx => new lVector4(x, z, y, x);
        
        /// <summary>
        /// Returns lVector4.rbgr swizzling (equivalent to lVector4.xzyx).
        /// </summary>
        public lVector4 rbgr => new lVector4(x, z, y, x);
        
        /// <summary>
        /// Returns lVector4.xzyy swizzling.
        /// </summary>
        public lVector4 xzyy => new lVector4(x, z, y, y);
        
        /// <summary>
        /// Returns lVector4.rbgg swizzling (equivalent to lVector4.xzyy).
        /// </summary>
        public lVector4 rbgg => new lVector4(x, z, y, y);
        
        /// <summary>
        /// Returns lVector4.xzyz swizzling.
        /// </summary>
        public lVector4 xzyz => new lVector4(x, z, y, z);
        
        /// <summary>
        /// Returns lVector4.rbgb swizzling (equivalent to lVector4.xzyz).
        /// </summary>
        public lVector4 rbgb => new lVector4(x, z, y, z);
        
        /// <summary>
        /// Returns lVector4.xzyw swizzling.
        /// </summary>
        public lVector4 xzyw => new lVector4(x, z, y, w);
        
        /// <summary>
        /// Returns lVector4.rbga swizzling (equivalent to lVector4.xzyw).
        /// </summary>
        public lVector4 rbga => new lVector4(x, z, y, w);
        
        /// <summary>
        /// Returns lVector4.xzz swizzling.
        /// </summary>
        public lVector3 xzz => new lVector3(x, z, z);
        
        /// <summary>
        /// Returns lVector4.rbb swizzling (equivalent to lVector4.xzz).
        /// </summary>
        public lVector3 rbb => new lVector3(x, z, z);
        
        /// <summary>
        /// Returns lVector4.xzzx swizzling.
        /// </summary>
        public lVector4 xzzx => new lVector4(x, z, z, x);
        
        /// <summary>
        /// Returns lVector4.rbbr swizzling (equivalent to lVector4.xzzx).
        /// </summary>
        public lVector4 rbbr => new lVector4(x, z, z, x);
        
        /// <summary>
        /// Returns lVector4.xzzy swizzling.
        /// </summary>
        public lVector4 xzzy => new lVector4(x, z, z, y);
        
        /// <summary>
        /// Returns lVector4.rbbg swizzling (equivalent to lVector4.xzzy).
        /// </summary>
        public lVector4 rbbg => new lVector4(x, z, z, y);
        
        /// <summary>
        /// Returns lVector4.xzzz swizzling.
        /// </summary>
        public lVector4 xzzz => new lVector4(x, z, z, z);
        
        /// <summary>
        /// Returns lVector4.rbbb swizzling (equivalent to lVector4.xzzz).
        /// </summary>
        public lVector4 rbbb => new lVector4(x, z, z, z);
        
        /// <summary>
        /// Returns lVector4.xzzw swizzling.
        /// </summary>
        public lVector4 xzzw => new lVector4(x, z, z, w);
        
        /// <summary>
        /// Returns lVector4.rbba swizzling (equivalent to lVector4.xzzw).
        /// </summary>
        public lVector4 rbba => new lVector4(x, z, z, w);
        
        /// <summary>
        /// Returns lVector4.xzw swizzling.
        /// </summary>
        public lVector3 xzw => new lVector3(x, z, w);
        
        /// <summary>
        /// Returns lVector4.rba swizzling (equivalent to lVector4.xzw).
        /// </summary>
        public lVector3 rba => new lVector3(x, z, w);
        
        /// <summary>
        /// Returns lVector4.xzwx swizzling.
        /// </summary>
        public lVector4 xzwx => new lVector4(x, z, w, x);
        
        /// <summary>
        /// Returns lVector4.rbar swizzling (equivalent to lVector4.xzwx).
        /// </summary>
        public lVector4 rbar => new lVector4(x, z, w, x);
        
        /// <summary>
        /// Returns lVector4.xzwy swizzling.
        /// </summary>
        public lVector4 xzwy => new lVector4(x, z, w, y);
        
        /// <summary>
        /// Returns lVector4.rbag swizzling (equivalent to lVector4.xzwy).
        /// </summary>
        public lVector4 rbag => new lVector4(x, z, w, y);
        
        /// <summary>
        /// Returns lVector4.xzwz swizzling.
        /// </summary>
        public lVector4 xzwz => new lVector4(x, z, w, z);
        
        /// <summary>
        /// Returns lVector4.rbab swizzling (equivalent to lVector4.xzwz).
        /// </summary>
        public lVector4 rbab => new lVector4(x, z, w, z);
        
        /// <summary>
        /// Returns lVector4.xzww swizzling.
        /// </summary>
        public lVector4 xzww => new lVector4(x, z, w, w);
        
        /// <summary>
        /// Returns lVector4.rbaa swizzling (equivalent to lVector4.xzww).
        /// </summary>
        public lVector4 rbaa => new lVector4(x, z, w, w);
        
        /// <summary>
        /// Returns lVector4.xw swizzling.
        /// </summary>
        public lVector2 xw => new lVector2(x, w);
        
        /// <summary>
        /// Returns lVector4.ra swizzling (equivalent to lVector4.xw).
        /// </summary>
        public lVector2 ra => new lVector2(x, w);
        
        /// <summary>
        /// Returns lVector4.xwx swizzling.
        /// </summary>
        public lVector3 xwx => new lVector3(x, w, x);
        
        /// <summary>
        /// Returns lVector4.rar swizzling (equivalent to lVector4.xwx).
        /// </summary>
        public lVector3 rar => new lVector3(x, w, x);
        
        /// <summary>
        /// Returns lVector4.xwxx swizzling.
        /// </summary>
        public lVector4 xwxx => new lVector4(x, w, x, x);
        
        /// <summary>
        /// Returns lVector4.rarr swizzling (equivalent to lVector4.xwxx).
        /// </summary>
        public lVector4 rarr => new lVector4(x, w, x, x);
        
        /// <summary>
        /// Returns lVector4.xwxy swizzling.
        /// </summary>
        public lVector4 xwxy => new lVector4(x, w, x, y);
        
        /// <summary>
        /// Returns lVector4.rarg swizzling (equivalent to lVector4.xwxy).
        /// </summary>
        public lVector4 rarg => new lVector4(x, w, x, y);
        
        /// <summary>
        /// Returns lVector4.xwxz swizzling.
        /// </summary>
        public lVector4 xwxz => new lVector4(x, w, x, z);
        
        /// <summary>
        /// Returns lVector4.rarb swizzling (equivalent to lVector4.xwxz).
        /// </summary>
        public lVector4 rarb => new lVector4(x, w, x, z);
        
        /// <summary>
        /// Returns lVector4.xwxw swizzling.
        /// </summary>
        public lVector4 xwxw => new lVector4(x, w, x, w);
        
        /// <summary>
        /// Returns lVector4.rara swizzling (equivalent to lVector4.xwxw).
        /// </summary>
        public lVector4 rara => new lVector4(x, w, x, w);
        
        /// <summary>
        /// Returns lVector4.xwy swizzling.
        /// </summary>
        public lVector3 xwy => new lVector3(x, w, y);
        
        /// <summary>
        /// Returns lVector4.rag swizzling (equivalent to lVector4.xwy).
        /// </summary>
        public lVector3 rag => new lVector3(x, w, y);
        
        /// <summary>
        /// Returns lVector4.xwyx swizzling.
        /// </summary>
        public lVector4 xwyx => new lVector4(x, w, y, x);
        
        /// <summary>
        /// Returns lVector4.ragr swizzling (equivalent to lVector4.xwyx).
        /// </summary>
        public lVector4 ragr => new lVector4(x, w, y, x);
        
        /// <summary>
        /// Returns lVector4.xwyy swizzling.
        /// </summary>
        public lVector4 xwyy => new lVector4(x, w, y, y);
        
        /// <summary>
        /// Returns lVector4.ragg swizzling (equivalent to lVector4.xwyy).
        /// </summary>
        public lVector4 ragg => new lVector4(x, w, y, y);
        
        /// <summary>
        /// Returns lVector4.xwyz swizzling.
        /// </summary>
        public lVector4 xwyz => new lVector4(x, w, y, z);
        
        /// <summary>
        /// Returns lVector4.ragb swizzling (equivalent to lVector4.xwyz).
        /// </summary>
        public lVector4 ragb => new lVector4(x, w, y, z);
        
        /// <summary>
        /// Returns lVector4.xwyw swizzling.
        /// </summary>
        public lVector4 xwyw => new lVector4(x, w, y, w);
        
        /// <summary>
        /// Returns lVector4.raga swizzling (equivalent to lVector4.xwyw).
        /// </summary>
        public lVector4 raga => new lVector4(x, w, y, w);
        
        /// <summary>
        /// Returns lVector4.xwz swizzling.
        /// </summary>
        public lVector3 xwz => new lVector3(x, w, z);
        
        /// <summary>
        /// Returns lVector4.rab swizzling (equivalent to lVector4.xwz).
        /// </summary>
        public lVector3 rab => new lVector3(x, w, z);
        
        /// <summary>
        /// Returns lVector4.xwzx swizzling.
        /// </summary>
        public lVector4 xwzx => new lVector4(x, w, z, x);
        
        /// <summary>
        /// Returns lVector4.rabr swizzling (equivalent to lVector4.xwzx).
        /// </summary>
        public lVector4 rabr => new lVector4(x, w, z, x);
        
        /// <summary>
        /// Returns lVector4.xwzy swizzling.
        /// </summary>
        public lVector4 xwzy => new lVector4(x, w, z, y);
        
        /// <summary>
        /// Returns lVector4.rabg swizzling (equivalent to lVector4.xwzy).
        /// </summary>
        public lVector4 rabg => new lVector4(x, w, z, y);
        
        /// <summary>
        /// Returns lVector4.xwzz swizzling.
        /// </summary>
        public lVector4 xwzz => new lVector4(x, w, z, z);
        
        /// <summary>
        /// Returns lVector4.rabb swizzling (equivalent to lVector4.xwzz).
        /// </summary>
        public lVector4 rabb => new lVector4(x, w, z, z);
        
        /// <summary>
        /// Returns lVector4.xwzw swizzling.
        /// </summary>
        public lVector4 xwzw => new lVector4(x, w, z, w);
        
        /// <summary>
        /// Returns lVector4.raba swizzling (equivalent to lVector4.xwzw).
        /// </summary>
        public lVector4 raba => new lVector4(x, w, z, w);
        
        /// <summary>
        /// Returns lVector4.xww swizzling.
        /// </summary>
        public lVector3 xww => new lVector3(x, w, w);
        
        /// <summary>
        /// Returns lVector4.raa swizzling (equivalent to lVector4.xww).
        /// </summary>
        public lVector3 raa => new lVector3(x, w, w);
        
        /// <summary>
        /// Returns lVector4.xwwx swizzling.
        /// </summary>
        public lVector4 xwwx => new lVector4(x, w, w, x);
        
        /// <summary>
        /// Returns lVector4.raar swizzling (equivalent to lVector4.xwwx).
        /// </summary>
        public lVector4 raar => new lVector4(x, w, w, x);
        
        /// <summary>
        /// Returns lVector4.xwwy swizzling.
        /// </summary>
        public lVector4 xwwy => new lVector4(x, w, w, y);
        
        /// <summary>
        /// Returns lVector4.raag swizzling (equivalent to lVector4.xwwy).
        /// </summary>
        public lVector4 raag => new lVector4(x, w, w, y);
        
        /// <summary>
        /// Returns lVector4.xwwz swizzling.
        /// </summary>
        public lVector4 xwwz => new lVector4(x, w, w, z);
        
        /// <summary>
        /// Returns lVector4.raab swizzling (equivalent to lVector4.xwwz).
        /// </summary>
        public lVector4 raab => new lVector4(x, w, w, z);
        
        /// <summary>
        /// Returns lVector4.xwww swizzling.
        /// </summary>
        public lVector4 xwww => new lVector4(x, w, w, w);
        
        /// <summary>
        /// Returns lVector4.raaa swizzling (equivalent to lVector4.xwww).
        /// </summary>
        public lVector4 raaa => new lVector4(x, w, w, w);
        
        /// <summary>
        /// Returns lVector4.yx swizzling.
        /// </summary>
        public lVector2 yx => new lVector2(y, x);
        
        /// <summary>
        /// Returns lVector4.gr swizzling (equivalent to lVector4.yx).
        /// </summary>
        public lVector2 gr => new lVector2(y, x);
        
        /// <summary>
        /// Returns lVector4.yxx swizzling.
        /// </summary>
        public lVector3 yxx => new lVector3(y, x, x);
        
        /// <summary>
        /// Returns lVector4.grr swizzling (equivalent to lVector4.yxx).
        /// </summary>
        public lVector3 grr => new lVector3(y, x, x);
        
        /// <summary>
        /// Returns lVector4.yxxx swizzling.
        /// </summary>
        public lVector4 yxxx => new lVector4(y, x, x, x);
        
        /// <summary>
        /// Returns lVector4.grrr swizzling (equivalent to lVector4.yxxx).
        /// </summary>
        public lVector4 grrr => new lVector4(y, x, x, x);
        
        /// <summary>
        /// Returns lVector4.yxxy swizzling.
        /// </summary>
        public lVector4 yxxy => new lVector4(y, x, x, y);
        
        /// <summary>
        /// Returns lVector4.grrg swizzling (equivalent to lVector4.yxxy).
        /// </summary>
        public lVector4 grrg => new lVector4(y, x, x, y);
        
        /// <summary>
        /// Returns lVector4.yxxz swizzling.
        /// </summary>
        public lVector4 yxxz => new lVector4(y, x, x, z);
        
        /// <summary>
        /// Returns lVector4.grrb swizzling (equivalent to lVector4.yxxz).
        /// </summary>
        public lVector4 grrb => new lVector4(y, x, x, z);
        
        /// <summary>
        /// Returns lVector4.yxxw swizzling.
        /// </summary>
        public lVector4 yxxw => new lVector4(y, x, x, w);
        
        /// <summary>
        /// Returns lVector4.grra swizzling (equivalent to lVector4.yxxw).
        /// </summary>
        public lVector4 grra => new lVector4(y, x, x, w);
        
        /// <summary>
        /// Returns lVector4.yxy swizzling.
        /// </summary>
        public lVector3 yxy => new lVector3(y, x, y);
        
        /// <summary>
        /// Returns lVector4.grg swizzling (equivalent to lVector4.yxy).
        /// </summary>
        public lVector3 grg => new lVector3(y, x, y);
        
        /// <summary>
        /// Returns lVector4.yxyx swizzling.
        /// </summary>
        public lVector4 yxyx => new lVector4(y, x, y, x);
        
        /// <summary>
        /// Returns lVector4.grgr swizzling (equivalent to lVector4.yxyx).
        /// </summary>
        public lVector4 grgr => new lVector4(y, x, y, x);
        
        /// <summary>
        /// Returns lVector4.yxyy swizzling.
        /// </summary>
        public lVector4 yxyy => new lVector4(y, x, y, y);
        
        /// <summary>
        /// Returns lVector4.grgg swizzling (equivalent to lVector4.yxyy).
        /// </summary>
        public lVector4 grgg => new lVector4(y, x, y, y);
        
        /// <summary>
        /// Returns lVector4.yxyz swizzling.
        /// </summary>
        public lVector4 yxyz => new lVector4(y, x, y, z);
        
        /// <summary>
        /// Returns lVector4.grgb swizzling (equivalent to lVector4.yxyz).
        /// </summary>
        public lVector4 grgb => new lVector4(y, x, y, z);
        
        /// <summary>
        /// Returns lVector4.yxyw swizzling.
        /// </summary>
        public lVector4 yxyw => new lVector4(y, x, y, w);
        
        /// <summary>
        /// Returns lVector4.grga swizzling (equivalent to lVector4.yxyw).
        /// </summary>
        public lVector4 grga => new lVector4(y, x, y, w);
        
        /// <summary>
        /// Returns lVector4.yxz swizzling.
        /// </summary>
        public lVector3 yxz => new lVector3(y, x, z);
        
        /// <summary>
        /// Returns lVector4.grb swizzling (equivalent to lVector4.yxz).
        /// </summary>
        public lVector3 grb => new lVector3(y, x, z);
        
        /// <summary>
        /// Returns lVector4.yxzx swizzling.
        /// </summary>
        public lVector4 yxzx => new lVector4(y, x, z, x);
        
        /// <summary>
        /// Returns lVector4.grbr swizzling (equivalent to lVector4.yxzx).
        /// </summary>
        public lVector4 grbr => new lVector4(y, x, z, x);
        
        /// <summary>
        /// Returns lVector4.yxzy swizzling.
        /// </summary>
        public lVector4 yxzy => new lVector4(y, x, z, y);
        
        /// <summary>
        /// Returns lVector4.grbg swizzling (equivalent to lVector4.yxzy).
        /// </summary>
        public lVector4 grbg => new lVector4(y, x, z, y);
        
        /// <summary>
        /// Returns lVector4.yxzz swizzling.
        /// </summary>
        public lVector4 yxzz => new lVector4(y, x, z, z);
        
        /// <summary>
        /// Returns lVector4.grbb swizzling (equivalent to lVector4.yxzz).
        /// </summary>
        public lVector4 grbb => new lVector4(y, x, z, z);
        
        /// <summary>
        /// Returns lVector4.yxzw swizzling.
        /// </summary>
        public lVector4 yxzw => new lVector4(y, x, z, w);
        
        /// <summary>
        /// Returns lVector4.grba swizzling (equivalent to lVector4.yxzw).
        /// </summary>
        public lVector4 grba => new lVector4(y, x, z, w);
        
        /// <summary>
        /// Returns lVector4.yxw swizzling.
        /// </summary>
        public lVector3 yxw => new lVector3(y, x, w);
        
        /// <summary>
        /// Returns lVector4.gra swizzling (equivalent to lVector4.yxw).
        /// </summary>
        public lVector3 gra => new lVector3(y, x, w);
        
        /// <summary>
        /// Returns lVector4.yxwx swizzling.
        /// </summary>
        public lVector4 yxwx => new lVector4(y, x, w, x);
        
        /// <summary>
        /// Returns lVector4.grar swizzling (equivalent to lVector4.yxwx).
        /// </summary>
        public lVector4 grar => new lVector4(y, x, w, x);
        
        /// <summary>
        /// Returns lVector4.yxwy swizzling.
        /// </summary>
        public lVector4 yxwy => new lVector4(y, x, w, y);
        
        /// <summary>
        /// Returns lVector4.grag swizzling (equivalent to lVector4.yxwy).
        /// </summary>
        public lVector4 grag => new lVector4(y, x, w, y);
        
        /// <summary>
        /// Returns lVector4.yxwz swizzling.
        /// </summary>
        public lVector4 yxwz => new lVector4(y, x, w, z);
        
        /// <summary>
        /// Returns lVector4.grab swizzling (equivalent to lVector4.yxwz).
        /// </summary>
        public lVector4 grab => new lVector4(y, x, w, z);
        
        /// <summary>
        /// Returns lVector4.yxww swizzling.
        /// </summary>
        public lVector4 yxww => new lVector4(y, x, w, w);
        
        /// <summary>
        /// Returns lVector4.graa swizzling (equivalent to lVector4.yxww).
        /// </summary>
        public lVector4 graa => new lVector4(y, x, w, w);
        
        /// <summary>
        /// Returns lVector4.yy swizzling.
        /// </summary>
        public lVector2 yy => new lVector2(y, y);
        
        /// <summary>
        /// Returns lVector4.gg swizzling (equivalent to lVector4.yy).
        /// </summary>
        public lVector2 gg => new lVector2(y, y);
        
        /// <summary>
        /// Returns lVector4.yyx swizzling.
        /// </summary>
        public lVector3 yyx => new lVector3(y, y, x);
        
        /// <summary>
        /// Returns lVector4.ggr swizzling (equivalent to lVector4.yyx).
        /// </summary>
        public lVector3 ggr => new lVector3(y, y, x);
        
        /// <summary>
        /// Returns lVector4.yyxx swizzling.
        /// </summary>
        public lVector4 yyxx => new lVector4(y, y, x, x);
        
        /// <summary>
        /// Returns lVector4.ggrr swizzling (equivalent to lVector4.yyxx).
        /// </summary>
        public lVector4 ggrr => new lVector4(y, y, x, x);
        
        /// <summary>
        /// Returns lVector4.yyxy swizzling.
        /// </summary>
        public lVector4 yyxy => new lVector4(y, y, x, y);
        
        /// <summary>
        /// Returns lVector4.ggrg swizzling (equivalent to lVector4.yyxy).
        /// </summary>
        public lVector4 ggrg => new lVector4(y, y, x, y);
        
        /// <summary>
        /// Returns lVector4.yyxz swizzling.
        /// </summary>
        public lVector4 yyxz => new lVector4(y, y, x, z);
        
        /// <summary>
        /// Returns lVector4.ggrb swizzling (equivalent to lVector4.yyxz).
        /// </summary>
        public lVector4 ggrb => new lVector4(y, y, x, z);
        
        /// <summary>
        /// Returns lVector4.yyxw swizzling.
        /// </summary>
        public lVector4 yyxw => new lVector4(y, y, x, w);
        
        /// <summary>
        /// Returns lVector4.ggra swizzling (equivalent to lVector4.yyxw).
        /// </summary>
        public lVector4 ggra => new lVector4(y, y, x, w);
        
        /// <summary>
        /// Returns lVector4.yyy swizzling.
        /// </summary>
        public lVector3 yyy => new lVector3(y, y, y);
        
        /// <summary>
        /// Returns lVector4.ggg swizzling (equivalent to lVector4.yyy).
        /// </summary>
        public lVector3 ggg => new lVector3(y, y, y);
        
        /// <summary>
        /// Returns lVector4.yyyx swizzling.
        /// </summary>
        public lVector4 yyyx => new lVector4(y, y, y, x);
        
        /// <summary>
        /// Returns lVector4.gggr swizzling (equivalent to lVector4.yyyx).
        /// </summary>
        public lVector4 gggr => new lVector4(y, y, y, x);
        
        /// <summary>
        /// Returns lVector4.yyyy swizzling.
        /// </summary>
        public lVector4 yyyy => new lVector4(y, y, y, y);
        
        /// <summary>
        /// Returns lVector4.gggg swizzling (equivalent to lVector4.yyyy).
        /// </summary>
        public lVector4 gggg => new lVector4(y, y, y, y);
        
        /// <summary>
        /// Returns lVector4.yyyz swizzling.
        /// </summary>
        public lVector4 yyyz => new lVector4(y, y, y, z);
        
        /// <summary>
        /// Returns lVector4.gggb swizzling (equivalent to lVector4.yyyz).
        /// </summary>
        public lVector4 gggb => new lVector4(y, y, y, z);
        
        /// <summary>
        /// Returns lVector4.yyyw swizzling.
        /// </summary>
        public lVector4 yyyw => new lVector4(y, y, y, w);
        
        /// <summary>
        /// Returns lVector4.ggga swizzling (equivalent to lVector4.yyyw).
        /// </summary>
        public lVector4 ggga => new lVector4(y, y, y, w);
        
        /// <summary>
        /// Returns lVector4.yyz swizzling.
        /// </summary>
        public lVector3 yyz => new lVector3(y, y, z);
        
        /// <summary>
        /// Returns lVector4.ggb swizzling (equivalent to lVector4.yyz).
        /// </summary>
        public lVector3 ggb => new lVector3(y, y, z);
        
        /// <summary>
        /// Returns lVector4.yyzx swizzling.
        /// </summary>
        public lVector4 yyzx => new lVector4(y, y, z, x);
        
        /// <summary>
        /// Returns lVector4.ggbr swizzling (equivalent to lVector4.yyzx).
        /// </summary>
        public lVector4 ggbr => new lVector4(y, y, z, x);
        
        /// <summary>
        /// Returns lVector4.yyzy swizzling.
        /// </summary>
        public lVector4 yyzy => new lVector4(y, y, z, y);
        
        /// <summary>
        /// Returns lVector4.ggbg swizzling (equivalent to lVector4.yyzy).
        /// </summary>
        public lVector4 ggbg => new lVector4(y, y, z, y);
        
        /// <summary>
        /// Returns lVector4.yyzz swizzling.
        /// </summary>
        public lVector4 yyzz => new lVector4(y, y, z, z);
        
        /// <summary>
        /// Returns lVector4.ggbb swizzling (equivalent to lVector4.yyzz).
        /// </summary>
        public lVector4 ggbb => new lVector4(y, y, z, z);
        
        /// <summary>
        /// Returns lVector4.yyzw swizzling.
        /// </summary>
        public lVector4 yyzw => new lVector4(y, y, z, w);
        
        /// <summary>
        /// Returns lVector4.ggba swizzling (equivalent to lVector4.yyzw).
        /// </summary>
        public lVector4 ggba => new lVector4(y, y, z, w);
        
        /// <summary>
        /// Returns lVector4.yyw swizzling.
        /// </summary>
        public lVector3 yyw => new lVector3(y, y, w);
        
        /// <summary>
        /// Returns lVector4.gga swizzling (equivalent to lVector4.yyw).
        /// </summary>
        public lVector3 gga => new lVector3(y, y, w);
        
        /// <summary>
        /// Returns lVector4.yywx swizzling.
        /// </summary>
        public lVector4 yywx => new lVector4(y, y, w, x);
        
        /// <summary>
        /// Returns lVector4.ggar swizzling (equivalent to lVector4.yywx).
        /// </summary>
        public lVector4 ggar => new lVector4(y, y, w, x);
        
        /// <summary>
        /// Returns lVector4.yywy swizzling.
        /// </summary>
        public lVector4 yywy => new lVector4(y, y, w, y);
        
        /// <summary>
        /// Returns lVector4.ggag swizzling (equivalent to lVector4.yywy).
        /// </summary>
        public lVector4 ggag => new lVector4(y, y, w, y);
        
        /// <summary>
        /// Returns lVector4.yywz swizzling.
        /// </summary>
        public lVector4 yywz => new lVector4(y, y, w, z);
        
        /// <summary>
        /// Returns lVector4.ggab swizzling (equivalent to lVector4.yywz).
        /// </summary>
        public lVector4 ggab => new lVector4(y, y, w, z);
        
        /// <summary>
        /// Returns lVector4.yyww swizzling.
        /// </summary>
        public lVector4 yyww => new lVector4(y, y, w, w);
        
        /// <summary>
        /// Returns lVector4.ggaa swizzling (equivalent to lVector4.yyww).
        /// </summary>
        public lVector4 ggaa => new lVector4(y, y, w, w);
        
        /// <summary>
        /// Returns lVector4.yz swizzling.
        /// </summary>
        public lVector2 yz => new lVector2(y, z);
        
        /// <summary>
        /// Returns lVector4.gb swizzling (equivalent to lVector4.yz).
        /// </summary>
        public lVector2 gb => new lVector2(y, z);
        
        /// <summary>
        /// Returns lVector4.yzx swizzling.
        /// </summary>
        public lVector3 yzx => new lVector3(y, z, x);
        
        /// <summary>
        /// Returns lVector4.gbr swizzling (equivalent to lVector4.yzx).
        /// </summary>
        public lVector3 gbr => new lVector3(y, z, x);
        
        /// <summary>
        /// Returns lVector4.yzxx swizzling.
        /// </summary>
        public lVector4 yzxx => new lVector4(y, z, x, x);
        
        /// <summary>
        /// Returns lVector4.gbrr swizzling (equivalent to lVector4.yzxx).
        /// </summary>
        public lVector4 gbrr => new lVector4(y, z, x, x);
        
        /// <summary>
        /// Returns lVector4.yzxy swizzling.
        /// </summary>
        public lVector4 yzxy => new lVector4(y, z, x, y);
        
        /// <summary>
        /// Returns lVector4.gbrg swizzling (equivalent to lVector4.yzxy).
        /// </summary>
        public lVector4 gbrg => new lVector4(y, z, x, y);
        
        /// <summary>
        /// Returns lVector4.yzxz swizzling.
        /// </summary>
        public lVector4 yzxz => new lVector4(y, z, x, z);
        
        /// <summary>
        /// Returns lVector4.gbrb swizzling (equivalent to lVector4.yzxz).
        /// </summary>
        public lVector4 gbrb => new lVector4(y, z, x, z);
        
        /// <summary>
        /// Returns lVector4.yzxw swizzling.
        /// </summary>
        public lVector4 yzxw => new lVector4(y, z, x, w);
        
        /// <summary>
        /// Returns lVector4.gbra swizzling (equivalent to lVector4.yzxw).
        /// </summary>
        public lVector4 gbra => new lVector4(y, z, x, w);
        
        /// <summary>
        /// Returns lVector4.yzy swizzling.
        /// </summary>
        public lVector3 yzy => new lVector3(y, z, y);
        
        /// <summary>
        /// Returns lVector4.gbg swizzling (equivalent to lVector4.yzy).
        /// </summary>
        public lVector3 gbg => new lVector3(y, z, y);
        
        /// <summary>
        /// Returns lVector4.yzyx swizzling.
        /// </summary>
        public lVector4 yzyx => new lVector4(y, z, y, x);
        
        /// <summary>
        /// Returns lVector4.gbgr swizzling (equivalent to lVector4.yzyx).
        /// </summary>
        public lVector4 gbgr => new lVector4(y, z, y, x);
        
        /// <summary>
        /// Returns lVector4.yzyy swizzling.
        /// </summary>
        public lVector4 yzyy => new lVector4(y, z, y, y);
        
        /// <summary>
        /// Returns lVector4.gbgg swizzling (equivalent to lVector4.yzyy).
        /// </summary>
        public lVector4 gbgg => new lVector4(y, z, y, y);
        
        /// <summary>
        /// Returns lVector4.yzyz swizzling.
        /// </summary>
        public lVector4 yzyz => new lVector4(y, z, y, z);
        
        /// <summary>
        /// Returns lVector4.gbgb swizzling (equivalent to lVector4.yzyz).
        /// </summary>
        public lVector4 gbgb => new lVector4(y, z, y, z);
        
        /// <summary>
        /// Returns lVector4.yzyw swizzling.
        /// </summary>
        public lVector4 yzyw => new lVector4(y, z, y, w);
        
        /// <summary>
        /// Returns lVector4.gbga swizzling (equivalent to lVector4.yzyw).
        /// </summary>
        public lVector4 gbga => new lVector4(y, z, y, w);
        
        /// <summary>
        /// Returns lVector4.yzz swizzling.
        /// </summary>
        public lVector3 yzz => new lVector3(y, z, z);
        
        /// <summary>
        /// Returns lVector4.gbb swizzling (equivalent to lVector4.yzz).
        /// </summary>
        public lVector3 gbb => new lVector3(y, z, z);
        
        /// <summary>
        /// Returns lVector4.yzzx swizzling.
        /// </summary>
        public lVector4 yzzx => new lVector4(y, z, z, x);
        
        /// <summary>
        /// Returns lVector4.gbbr swizzling (equivalent to lVector4.yzzx).
        /// </summary>
        public lVector4 gbbr => new lVector4(y, z, z, x);
        
        /// <summary>
        /// Returns lVector4.yzzy swizzling.
        /// </summary>
        public lVector4 yzzy => new lVector4(y, z, z, y);
        
        /// <summary>
        /// Returns lVector4.gbbg swizzling (equivalent to lVector4.yzzy).
        /// </summary>
        public lVector4 gbbg => new lVector4(y, z, z, y);
        
        /// <summary>
        /// Returns lVector4.yzzz swizzling.
        /// </summary>
        public lVector4 yzzz => new lVector4(y, z, z, z);
        
        /// <summary>
        /// Returns lVector4.gbbb swizzling (equivalent to lVector4.yzzz).
        /// </summary>
        public lVector4 gbbb => new lVector4(y, z, z, z);
        
        /// <summary>
        /// Returns lVector4.yzzw swizzling.
        /// </summary>
        public lVector4 yzzw => new lVector4(y, z, z, w);
        
        /// <summary>
        /// Returns lVector4.gbba swizzling (equivalent to lVector4.yzzw).
        /// </summary>
        public lVector4 gbba => new lVector4(y, z, z, w);
        
        /// <summary>
        /// Returns lVector4.yzw swizzling.
        /// </summary>
        public lVector3 yzw => new lVector3(y, z, w);
        
        /// <summary>
        /// Returns lVector4.gba swizzling (equivalent to lVector4.yzw).
        /// </summary>
        public lVector3 gba => new lVector3(y, z, w);
        
        /// <summary>
        /// Returns lVector4.yzwx swizzling.
        /// </summary>
        public lVector4 yzwx => new lVector4(y, z, w, x);
        
        /// <summary>
        /// Returns lVector4.gbar swizzling (equivalent to lVector4.yzwx).
        /// </summary>
        public lVector4 gbar => new lVector4(y, z, w, x);
        
        /// <summary>
        /// Returns lVector4.yzwy swizzling.
        /// </summary>
        public lVector4 yzwy => new lVector4(y, z, w, y);
        
        /// <summary>
        /// Returns lVector4.gbag swizzling (equivalent to lVector4.yzwy).
        /// </summary>
        public lVector4 gbag => new lVector4(y, z, w, y);
        
        /// <summary>
        /// Returns lVector4.yzwz swizzling.
        /// </summary>
        public lVector4 yzwz => new lVector4(y, z, w, z);
        
        /// <summary>
        /// Returns lVector4.gbab swizzling (equivalent to lVector4.yzwz).
        /// </summary>
        public lVector4 gbab => new lVector4(y, z, w, z);
        
        /// <summary>
        /// Returns lVector4.yzww swizzling.
        /// </summary>
        public lVector4 yzww => new lVector4(y, z, w, w);
        
        /// <summary>
        /// Returns lVector4.gbaa swizzling (equivalent to lVector4.yzww).
        /// </summary>
        public lVector4 gbaa => new lVector4(y, z, w, w);
        
        /// <summary>
        /// Returns lVector4.yw swizzling.
        /// </summary>
        public lVector2 yw => new lVector2(y, w);
        
        /// <summary>
        /// Returns lVector4.ga swizzling (equivalent to lVector4.yw).
        /// </summary>
        public lVector2 ga => new lVector2(y, w);
        
        /// <summary>
        /// Returns lVector4.ywx swizzling.
        /// </summary>
        public lVector3 ywx => new lVector3(y, w, x);
        
        /// <summary>
        /// Returns lVector4.gar swizzling (equivalent to lVector4.ywx).
        /// </summary>
        public lVector3 gar => new lVector3(y, w, x);
        
        /// <summary>
        /// Returns lVector4.ywxx swizzling.
        /// </summary>
        public lVector4 ywxx => new lVector4(y, w, x, x);
        
        /// <summary>
        /// Returns lVector4.garr swizzling (equivalent to lVector4.ywxx).
        /// </summary>
        public lVector4 garr => new lVector4(y, w, x, x);
        
        /// <summary>
        /// Returns lVector4.ywxy swizzling.
        /// </summary>
        public lVector4 ywxy => new lVector4(y, w, x, y);
        
        /// <summary>
        /// Returns lVector4.garg swizzling (equivalent to lVector4.ywxy).
        /// </summary>
        public lVector4 garg => new lVector4(y, w, x, y);
        
        /// <summary>
        /// Returns lVector4.ywxz swizzling.
        /// </summary>
        public lVector4 ywxz => new lVector4(y, w, x, z);
        
        /// <summary>
        /// Returns lVector4.garb swizzling (equivalent to lVector4.ywxz).
        /// </summary>
        public lVector4 garb => new lVector4(y, w, x, z);
        
        /// <summary>
        /// Returns lVector4.ywxw swizzling.
        /// </summary>
        public lVector4 ywxw => new lVector4(y, w, x, w);
        
        /// <summary>
        /// Returns lVector4.gara swizzling (equivalent to lVector4.ywxw).
        /// </summary>
        public lVector4 gara => new lVector4(y, w, x, w);
        
        /// <summary>
        /// Returns lVector4.ywy swizzling.
        /// </summary>
        public lVector3 ywy => new lVector3(y, w, y);
        
        /// <summary>
        /// Returns lVector4.gag swizzling (equivalent to lVector4.ywy).
        /// </summary>
        public lVector3 gag => new lVector3(y, w, y);
        
        /// <summary>
        /// Returns lVector4.ywyx swizzling.
        /// </summary>
        public lVector4 ywyx => new lVector4(y, w, y, x);
        
        /// <summary>
        /// Returns lVector4.gagr swizzling (equivalent to lVector4.ywyx).
        /// </summary>
        public lVector4 gagr => new lVector4(y, w, y, x);
        
        /// <summary>
        /// Returns lVector4.ywyy swizzling.
        /// </summary>
        public lVector4 ywyy => new lVector4(y, w, y, y);
        
        /// <summary>
        /// Returns lVector4.gagg swizzling (equivalent to lVector4.ywyy).
        /// </summary>
        public lVector4 gagg => new lVector4(y, w, y, y);
        
        /// <summary>
        /// Returns lVector4.ywyz swizzling.
        /// </summary>
        public lVector4 ywyz => new lVector4(y, w, y, z);
        
        /// <summary>
        /// Returns lVector4.gagb swizzling (equivalent to lVector4.ywyz).
        /// </summary>
        public lVector4 gagb => new lVector4(y, w, y, z);
        
        /// <summary>
        /// Returns lVector4.ywyw swizzling.
        /// </summary>
        public lVector4 ywyw => new lVector4(y, w, y, w);
        
        /// <summary>
        /// Returns lVector4.gaga swizzling (equivalent to lVector4.ywyw).
        /// </summary>
        public lVector4 gaga => new lVector4(y, w, y, w);
        
        /// <summary>
        /// Returns lVector4.ywz swizzling.
        /// </summary>
        public lVector3 ywz => new lVector3(y, w, z);
        
        /// <summary>
        /// Returns lVector4.gab swizzling (equivalent to lVector4.ywz).
        /// </summary>
        public lVector3 gab => new lVector3(y, w, z);
        
        /// <summary>
        /// Returns lVector4.ywzx swizzling.
        /// </summary>
        public lVector4 ywzx => new lVector4(y, w, z, x);
        
        /// <summary>
        /// Returns lVector4.gabr swizzling (equivalent to lVector4.ywzx).
        /// </summary>
        public lVector4 gabr => new lVector4(y, w, z, x);
        
        /// <summary>
        /// Returns lVector4.ywzy swizzling.
        /// </summary>
        public lVector4 ywzy => new lVector4(y, w, z, y);
        
        /// <summary>
        /// Returns lVector4.gabg swizzling (equivalent to lVector4.ywzy).
        /// </summary>
        public lVector4 gabg => new lVector4(y, w, z, y);
        
        /// <summary>
        /// Returns lVector4.ywzz swizzling.
        /// </summary>
        public lVector4 ywzz => new lVector4(y, w, z, z);
        
        /// <summary>
        /// Returns lVector4.gabb swizzling (equivalent to lVector4.ywzz).
        /// </summary>
        public lVector4 gabb => new lVector4(y, w, z, z);
        
        /// <summary>
        /// Returns lVector4.ywzw swizzling.
        /// </summary>
        public lVector4 ywzw => new lVector4(y, w, z, w);
        
        /// <summary>
        /// Returns lVector4.gaba swizzling (equivalent to lVector4.ywzw).
        /// </summary>
        public lVector4 gaba => new lVector4(y, w, z, w);
        
        /// <summary>
        /// Returns lVector4.yww swizzling.
        /// </summary>
        public lVector3 yww => new lVector3(y, w, w);
        
        /// <summary>
        /// Returns lVector4.gaa swizzling (equivalent to lVector4.yww).
        /// </summary>
        public lVector3 gaa => new lVector3(y, w, w);
        
        /// <summary>
        /// Returns lVector4.ywwx swizzling.
        /// </summary>
        public lVector4 ywwx => new lVector4(y, w, w, x);
        
        /// <summary>
        /// Returns lVector4.gaar swizzling (equivalent to lVector4.ywwx).
        /// </summary>
        public lVector4 gaar => new lVector4(y, w, w, x);
        
        /// <summary>
        /// Returns lVector4.ywwy swizzling.
        /// </summary>
        public lVector4 ywwy => new lVector4(y, w, w, y);
        
        /// <summary>
        /// Returns lVector4.gaag swizzling (equivalent to lVector4.ywwy).
        /// </summary>
        public lVector4 gaag => new lVector4(y, w, w, y);
        
        /// <summary>
        /// Returns lVector4.ywwz swizzling.
        /// </summary>
        public lVector4 ywwz => new lVector4(y, w, w, z);
        
        /// <summary>
        /// Returns lVector4.gaab swizzling (equivalent to lVector4.ywwz).
        /// </summary>
        public lVector4 gaab => new lVector4(y, w, w, z);
        
        /// <summary>
        /// Returns lVector4.ywww swizzling.
        /// </summary>
        public lVector4 ywww => new lVector4(y, w, w, w);
        
        /// <summary>
        /// Returns lVector4.gaaa swizzling (equivalent to lVector4.ywww).
        /// </summary>
        public lVector4 gaaa => new lVector4(y, w, w, w);
        
        /// <summary>
        /// Returns lVector4.zx swizzling.
        /// </summary>
        public lVector2 zx => new lVector2(z, x);
        
        /// <summary>
        /// Returns lVector4.br swizzling (equivalent to lVector4.zx).
        /// </summary>
        public lVector2 br => new lVector2(z, x);
        
        /// <summary>
        /// Returns lVector4.zxx swizzling.
        /// </summary>
        public lVector3 zxx => new lVector3(z, x, x);
        
        /// <summary>
        /// Returns lVector4.brr swizzling (equivalent to lVector4.zxx).
        /// </summary>
        public lVector3 brr => new lVector3(z, x, x);
        
        /// <summary>
        /// Returns lVector4.zxxx swizzling.
        /// </summary>
        public lVector4 zxxx => new lVector4(z, x, x, x);
        
        /// <summary>
        /// Returns lVector4.brrr swizzling (equivalent to lVector4.zxxx).
        /// </summary>
        public lVector4 brrr => new lVector4(z, x, x, x);
        
        /// <summary>
        /// Returns lVector4.zxxy swizzling.
        /// </summary>
        public lVector4 zxxy => new lVector4(z, x, x, y);
        
        /// <summary>
        /// Returns lVector4.brrg swizzling (equivalent to lVector4.zxxy).
        /// </summary>
        public lVector4 brrg => new lVector4(z, x, x, y);
        
        /// <summary>
        /// Returns lVector4.zxxz swizzling.
        /// </summary>
        public lVector4 zxxz => new lVector4(z, x, x, z);
        
        /// <summary>
        /// Returns lVector4.brrb swizzling (equivalent to lVector4.zxxz).
        /// </summary>
        public lVector4 brrb => new lVector4(z, x, x, z);
        
        /// <summary>
        /// Returns lVector4.zxxw swizzling.
        /// </summary>
        public lVector4 zxxw => new lVector4(z, x, x, w);
        
        /// <summary>
        /// Returns lVector4.brra swizzling (equivalent to lVector4.zxxw).
        /// </summary>
        public lVector4 brra => new lVector4(z, x, x, w);
        
        /// <summary>
        /// Returns lVector4.zxy swizzling.
        /// </summary>
        public lVector3 zxy => new lVector3(z, x, y);
        
        /// <summary>
        /// Returns lVector4.brg swizzling (equivalent to lVector4.zxy).
        /// </summary>
        public lVector3 brg => new lVector3(z, x, y);
        
        /// <summary>
        /// Returns lVector4.zxyx swizzling.
        /// </summary>
        public lVector4 zxyx => new lVector4(z, x, y, x);
        
        /// <summary>
        /// Returns lVector4.brgr swizzling (equivalent to lVector4.zxyx).
        /// </summary>
        public lVector4 brgr => new lVector4(z, x, y, x);
        
        /// <summary>
        /// Returns lVector4.zxyy swizzling.
        /// </summary>
        public lVector4 zxyy => new lVector4(z, x, y, y);
        
        /// <summary>
        /// Returns lVector4.brgg swizzling (equivalent to lVector4.zxyy).
        /// </summary>
        public lVector4 brgg => new lVector4(z, x, y, y);
        
        /// <summary>
        /// Returns lVector4.zxyz swizzling.
        /// </summary>
        public lVector4 zxyz => new lVector4(z, x, y, z);
        
        /// <summary>
        /// Returns lVector4.brgb swizzling (equivalent to lVector4.zxyz).
        /// </summary>
        public lVector4 brgb => new lVector4(z, x, y, z);
        
        /// <summary>
        /// Returns lVector4.zxyw swizzling.
        /// </summary>
        public lVector4 zxyw => new lVector4(z, x, y, w);
        
        /// <summary>
        /// Returns lVector4.brga swizzling (equivalent to lVector4.zxyw).
        /// </summary>
        public lVector4 brga => new lVector4(z, x, y, w);
        
        /// <summary>
        /// Returns lVector4.zxz swizzling.
        /// </summary>
        public lVector3 zxz => new lVector3(z, x, z);
        
        /// <summary>
        /// Returns lVector4.brb swizzling (equivalent to lVector4.zxz).
        /// </summary>
        public lVector3 brb => new lVector3(z, x, z);
        
        /// <summary>
        /// Returns lVector4.zxzx swizzling.
        /// </summary>
        public lVector4 zxzx => new lVector4(z, x, z, x);
        
        /// <summary>
        /// Returns lVector4.brbr swizzling (equivalent to lVector4.zxzx).
        /// </summary>
        public lVector4 brbr => new lVector4(z, x, z, x);
        
        /// <summary>
        /// Returns lVector4.zxzy swizzling.
        /// </summary>
        public lVector4 zxzy => new lVector4(z, x, z, y);
        
        /// <summary>
        /// Returns lVector4.brbg swizzling (equivalent to lVector4.zxzy).
        /// </summary>
        public lVector4 brbg => new lVector4(z, x, z, y);
        
        /// <summary>
        /// Returns lVector4.zxzz swizzling.
        /// </summary>
        public lVector4 zxzz => new lVector4(z, x, z, z);
        
        /// <summary>
        /// Returns lVector4.brbb swizzling (equivalent to lVector4.zxzz).
        /// </summary>
        public lVector4 brbb => new lVector4(z, x, z, z);
        
        /// <summary>
        /// Returns lVector4.zxzw swizzling.
        /// </summary>
        public lVector4 zxzw => new lVector4(z, x, z, w);
        
        /// <summary>
        /// Returns lVector4.brba swizzling (equivalent to lVector4.zxzw).
        /// </summary>
        public lVector4 brba => new lVector4(z, x, z, w);
        
        /// <summary>
        /// Returns lVector4.zxw swizzling.
        /// </summary>
        public lVector3 zxw => new lVector3(z, x, w);
        
        /// <summary>
        /// Returns lVector4.bra swizzling (equivalent to lVector4.zxw).
        /// </summary>
        public lVector3 bra => new lVector3(z, x, w);
        
        /// <summary>
        /// Returns lVector4.zxwx swizzling.
        /// </summary>
        public lVector4 zxwx => new lVector4(z, x, w, x);
        
        /// <summary>
        /// Returns lVector4.brar swizzling (equivalent to lVector4.zxwx).
        /// </summary>
        public lVector4 brar => new lVector4(z, x, w, x);
        
        /// <summary>
        /// Returns lVector4.zxwy swizzling.
        /// </summary>
        public lVector4 zxwy => new lVector4(z, x, w, y);
        
        /// <summary>
        /// Returns lVector4.brag swizzling (equivalent to lVector4.zxwy).
        /// </summary>
        public lVector4 brag => new lVector4(z, x, w, y);
        
        /// <summary>
        /// Returns lVector4.zxwz swizzling.
        /// </summary>
        public lVector4 zxwz => new lVector4(z, x, w, z);
        
        /// <summary>
        /// Returns lVector4.brab swizzling (equivalent to lVector4.zxwz).
        /// </summary>
        public lVector4 brab => new lVector4(z, x, w, z);
        
        /// <summary>
        /// Returns lVector4.zxww swizzling.
        /// </summary>
        public lVector4 zxww => new lVector4(z, x, w, w);
        
        /// <summary>
        /// Returns lVector4.braa swizzling (equivalent to lVector4.zxww).
        /// </summary>
        public lVector4 braa => new lVector4(z, x, w, w);
        
        /// <summary>
        /// Returns lVector4.zy swizzling.
        /// </summary>
        public lVector2 zy => new lVector2(z, y);
        
        /// <summary>
        /// Returns lVector4.bg swizzling (equivalent to lVector4.zy).
        /// </summary>
        public lVector2 bg => new lVector2(z, y);
        
        /// <summary>
        /// Returns lVector4.zyx swizzling.
        /// </summary>
        public lVector3 zyx => new lVector3(z, y, x);
        
        /// <summary>
        /// Returns lVector4.bgr swizzling (equivalent to lVector4.zyx).
        /// </summary>
        public lVector3 bgr => new lVector3(z, y, x);
        
        /// <summary>
        /// Returns lVector4.zyxx swizzling.
        /// </summary>
        public lVector4 zyxx => new lVector4(z, y, x, x);
        
        /// <summary>
        /// Returns lVector4.bgrr swizzling (equivalent to lVector4.zyxx).
        /// </summary>
        public lVector4 bgrr => new lVector4(z, y, x, x);
        
        /// <summary>
        /// Returns lVector4.zyxy swizzling.
        /// </summary>
        public lVector4 zyxy => new lVector4(z, y, x, y);
        
        /// <summary>
        /// Returns lVector4.bgrg swizzling (equivalent to lVector4.zyxy).
        /// </summary>
        public lVector4 bgrg => new lVector4(z, y, x, y);
        
        /// <summary>
        /// Returns lVector4.zyxz swizzling.
        /// </summary>
        public lVector4 zyxz => new lVector4(z, y, x, z);
        
        /// <summary>
        /// Returns lVector4.bgrb swizzling (equivalent to lVector4.zyxz).
        /// </summary>
        public lVector4 bgrb => new lVector4(z, y, x, z);
        
        /// <summary>
        /// Returns lVector4.zyxw swizzling.
        /// </summary>
        public lVector4 zyxw => new lVector4(z, y, x, w);
        
        /// <summary>
        /// Returns lVector4.bgra swizzling (equivalent to lVector4.zyxw).
        /// </summary>
        public lVector4 bgra => new lVector4(z, y, x, w);
        
        /// <summary>
        /// Returns lVector4.zyy swizzling.
        /// </summary>
        public lVector3 zyy => new lVector3(z, y, y);
        
        /// <summary>
        /// Returns lVector4.bgg swizzling (equivalent to lVector4.zyy).
        /// </summary>
        public lVector3 bgg => new lVector3(z, y, y);
        
        /// <summary>
        /// Returns lVector4.zyyx swizzling.
        /// </summary>
        public lVector4 zyyx => new lVector4(z, y, y, x);
        
        /// <summary>
        /// Returns lVector4.bggr swizzling (equivalent to lVector4.zyyx).
        /// </summary>
        public lVector4 bggr => new lVector4(z, y, y, x);
        
        /// <summary>
        /// Returns lVector4.zyyy swizzling.
        /// </summary>
        public lVector4 zyyy => new lVector4(z, y, y, y);
        
        /// <summary>
        /// Returns lVector4.bggg swizzling (equivalent to lVector4.zyyy).
        /// </summary>
        public lVector4 bggg => new lVector4(z, y, y, y);
        
        /// <summary>
        /// Returns lVector4.zyyz swizzling.
        /// </summary>
        public lVector4 zyyz => new lVector4(z, y, y, z);
        
        /// <summary>
        /// Returns lVector4.bggb swizzling (equivalent to lVector4.zyyz).
        /// </summary>
        public lVector4 bggb => new lVector4(z, y, y, z);
        
        /// <summary>
        /// Returns lVector4.zyyw swizzling.
        /// </summary>
        public lVector4 zyyw => new lVector4(z, y, y, w);
        
        /// <summary>
        /// Returns lVector4.bgga swizzling (equivalent to lVector4.zyyw).
        /// </summary>
        public lVector4 bgga => new lVector4(z, y, y, w);
        
        /// <summary>
        /// Returns lVector4.zyz swizzling.
        /// </summary>
        public lVector3 zyz => new lVector3(z, y, z);
        
        /// <summary>
        /// Returns lVector4.bgb swizzling (equivalent to lVector4.zyz).
        /// </summary>
        public lVector3 bgb => new lVector3(z, y, z);
        
        /// <summary>
        /// Returns lVector4.zyzx swizzling.
        /// </summary>
        public lVector4 zyzx => new lVector4(z, y, z, x);
        
        /// <summary>
        /// Returns lVector4.bgbr swizzling (equivalent to lVector4.zyzx).
        /// </summary>
        public lVector4 bgbr => new lVector4(z, y, z, x);
        
        /// <summary>
        /// Returns lVector4.zyzy swizzling.
        /// </summary>
        public lVector4 zyzy => new lVector4(z, y, z, y);
        
        /// <summary>
        /// Returns lVector4.bgbg swizzling (equivalent to lVector4.zyzy).
        /// </summary>
        public lVector4 bgbg => new lVector4(z, y, z, y);
        
        /// <summary>
        /// Returns lVector4.zyzz swizzling.
        /// </summary>
        public lVector4 zyzz => new lVector4(z, y, z, z);
        
        /// <summary>
        /// Returns lVector4.bgbb swizzling (equivalent to lVector4.zyzz).
        /// </summary>
        public lVector4 bgbb => new lVector4(z, y, z, z);
        
        /// <summary>
        /// Returns lVector4.zyzw swizzling.
        /// </summary>
        public lVector4 zyzw => new lVector4(z, y, z, w);
        
        /// <summary>
        /// Returns lVector4.bgba swizzling (equivalent to lVector4.zyzw).
        /// </summary>
        public lVector4 bgba => new lVector4(z, y, z, w);
        
        /// <summary>
        /// Returns lVector4.zyw swizzling.
        /// </summary>
        public lVector3 zyw => new lVector3(z, y, w);
        
        /// <summary>
        /// Returns lVector4.bga swizzling (equivalent to lVector4.zyw).
        /// </summary>
        public lVector3 bga => new lVector3(z, y, w);
        
        /// <summary>
        /// Returns lVector4.zywx swizzling.
        /// </summary>
        public lVector4 zywx => new lVector4(z, y, w, x);
        
        /// <summary>
        /// Returns lVector4.bgar swizzling (equivalent to lVector4.zywx).
        /// </summary>
        public lVector4 bgar => new lVector4(z, y, w, x);
        
        /// <summary>
        /// Returns lVector4.zywy swizzling.
        /// </summary>
        public lVector4 zywy => new lVector4(z, y, w, y);
        
        /// <summary>
        /// Returns lVector4.bgag swizzling (equivalent to lVector4.zywy).
        /// </summary>
        public lVector4 bgag => new lVector4(z, y, w, y);
        
        /// <summary>
        /// Returns lVector4.zywz swizzling.
        /// </summary>
        public lVector4 zywz => new lVector4(z, y, w, z);
        
        /// <summary>
        /// Returns lVector4.bgab swizzling (equivalent to lVector4.zywz).
        /// </summary>
        public lVector4 bgab => new lVector4(z, y, w, z);
        
        /// <summary>
        /// Returns lVector4.zyww swizzling.
        /// </summary>
        public lVector4 zyww => new lVector4(z, y, w, w);
        
        /// <summary>
        /// Returns lVector4.bgaa swizzling (equivalent to lVector4.zyww).
        /// </summary>
        public lVector4 bgaa => new lVector4(z, y, w, w);
        
        /// <summary>
        /// Returns lVector4.zz swizzling.
        /// </summary>
        public lVector2 zz => new lVector2(z, z);
        
        /// <summary>
        /// Returns lVector4.bb swizzling (equivalent to lVector4.zz).
        /// </summary>
        public lVector2 bb => new lVector2(z, z);
        
        /// <summary>
        /// Returns lVector4.zzx swizzling.
        /// </summary>
        public lVector3 zzx => new lVector3(z, z, x);
        
        /// <summary>
        /// Returns lVector4.bbr swizzling (equivalent to lVector4.zzx).
        /// </summary>
        public lVector3 bbr => new lVector3(z, z, x);
        
        /// <summary>
        /// Returns lVector4.zzxx swizzling.
        /// </summary>
        public lVector4 zzxx => new lVector4(z, z, x, x);
        
        /// <summary>
        /// Returns lVector4.bbrr swizzling (equivalent to lVector4.zzxx).
        /// </summary>
        public lVector4 bbrr => new lVector4(z, z, x, x);
        
        /// <summary>
        /// Returns lVector4.zzxy swizzling.
        /// </summary>
        public lVector4 zzxy => new lVector4(z, z, x, y);
        
        /// <summary>
        /// Returns lVector4.bbrg swizzling (equivalent to lVector4.zzxy).
        /// </summary>
        public lVector4 bbrg => new lVector4(z, z, x, y);
        
        /// <summary>
        /// Returns lVector4.zzxz swizzling.
        /// </summary>
        public lVector4 zzxz => new lVector4(z, z, x, z);
        
        /// <summary>
        /// Returns lVector4.bbrb swizzling (equivalent to lVector4.zzxz).
        /// </summary>
        public lVector4 bbrb => new lVector4(z, z, x, z);
        
        /// <summary>
        /// Returns lVector4.zzxw swizzling.
        /// </summary>
        public lVector4 zzxw => new lVector4(z, z, x, w);
        
        /// <summary>
        /// Returns lVector4.bbra swizzling (equivalent to lVector4.zzxw).
        /// </summary>
        public lVector4 bbra => new lVector4(z, z, x, w);
        
        /// <summary>
        /// Returns lVector4.zzy swizzling.
        /// </summary>
        public lVector3 zzy => new lVector3(z, z, y);
        
        /// <summary>
        /// Returns lVector4.bbg swizzling (equivalent to lVector4.zzy).
        /// </summary>
        public lVector3 bbg => new lVector3(z, z, y);
        
        /// <summary>
        /// Returns lVector4.zzyx swizzling.
        /// </summary>
        public lVector4 zzyx => new lVector4(z, z, y, x);
        
        /// <summary>
        /// Returns lVector4.bbgr swizzling (equivalent to lVector4.zzyx).
        /// </summary>
        public lVector4 bbgr => new lVector4(z, z, y, x);
        
        /// <summary>
        /// Returns lVector4.zzyy swizzling.
        /// </summary>
        public lVector4 zzyy => new lVector4(z, z, y, y);
        
        /// <summary>
        /// Returns lVector4.bbgg swizzling (equivalent to lVector4.zzyy).
        /// </summary>
        public lVector4 bbgg => new lVector4(z, z, y, y);
        
        /// <summary>
        /// Returns lVector4.zzyz swizzling.
        /// </summary>
        public lVector4 zzyz => new lVector4(z, z, y, z);
        
        /// <summary>
        /// Returns lVector4.bbgb swizzling (equivalent to lVector4.zzyz).
        /// </summary>
        public lVector4 bbgb => new lVector4(z, z, y, z);
        
        /// <summary>
        /// Returns lVector4.zzyw swizzling.
        /// </summary>
        public lVector4 zzyw => new lVector4(z, z, y, w);
        
        /// <summary>
        /// Returns lVector4.bbga swizzling (equivalent to lVector4.zzyw).
        /// </summary>
        public lVector4 bbga => new lVector4(z, z, y, w);
        
        /// <summary>
        /// Returns lVector4.zzz swizzling.
        /// </summary>
        public lVector3 zzz => new lVector3(z, z, z);
        
        /// <summary>
        /// Returns lVector4.bbb swizzling (equivalent to lVector4.zzz).
        /// </summary>
        public lVector3 bbb => new lVector3(z, z, z);
        
        /// <summary>
        /// Returns lVector4.zzzx swizzling.
        /// </summary>
        public lVector4 zzzx => new lVector4(z, z, z, x);
        
        /// <summary>
        /// Returns lVector4.bbbr swizzling (equivalent to lVector4.zzzx).
        /// </summary>
        public lVector4 bbbr => new lVector4(z, z, z, x);
        
        /// <summary>
        /// Returns lVector4.zzzy swizzling.
        /// </summary>
        public lVector4 zzzy => new lVector4(z, z, z, y);
        
        /// <summary>
        /// Returns lVector4.bbbg swizzling (equivalent to lVector4.zzzy).
        /// </summary>
        public lVector4 bbbg => new lVector4(z, z, z, y);
        
        /// <summary>
        /// Returns lVector4.zzzz swizzling.
        /// </summary>
        public lVector4 zzzz => new lVector4(z, z, z, z);
        
        /// <summary>
        /// Returns lVector4.bbbb swizzling (equivalent to lVector4.zzzz).
        /// </summary>
        public lVector4 bbbb => new lVector4(z, z, z, z);
        
        /// <summary>
        /// Returns lVector4.zzzw swizzling.
        /// </summary>
        public lVector4 zzzw => new lVector4(z, z, z, w);
        
        /// <summary>
        /// Returns lVector4.bbba swizzling (equivalent to lVector4.zzzw).
        /// </summary>
        public lVector4 bbba => new lVector4(z, z, z, w);
        
        /// <summary>
        /// Returns lVector4.zzw swizzling.
        /// </summary>
        public lVector3 zzw => new lVector3(z, z, w);
        
        /// <summary>
        /// Returns lVector4.bba swizzling (equivalent to lVector4.zzw).
        /// </summary>
        public lVector3 bba => new lVector3(z, z, w);
        
        /// <summary>
        /// Returns lVector4.zzwx swizzling.
        /// </summary>
        public lVector4 zzwx => new lVector4(z, z, w, x);
        
        /// <summary>
        /// Returns lVector4.bbar swizzling (equivalent to lVector4.zzwx).
        /// </summary>
        public lVector4 bbar => new lVector4(z, z, w, x);
        
        /// <summary>
        /// Returns lVector4.zzwy swizzling.
        /// </summary>
        public lVector4 zzwy => new lVector4(z, z, w, y);
        
        /// <summary>
        /// Returns lVector4.bbag swizzling (equivalent to lVector4.zzwy).
        /// </summary>
        public lVector4 bbag => new lVector4(z, z, w, y);
        
        /// <summary>
        /// Returns lVector4.zzwz swizzling.
        /// </summary>
        public lVector4 zzwz => new lVector4(z, z, w, z);
        
        /// <summary>
        /// Returns lVector4.bbab swizzling (equivalent to lVector4.zzwz).
        /// </summary>
        public lVector4 bbab => new lVector4(z, z, w, z);
        
        /// <summary>
        /// Returns lVector4.zzww swizzling.
        /// </summary>
        public lVector4 zzww => new lVector4(z, z, w, w);
        
        /// <summary>
        /// Returns lVector4.bbaa swizzling (equivalent to lVector4.zzww).
        /// </summary>
        public lVector4 bbaa => new lVector4(z, z, w, w);
        
        /// <summary>
        /// Returns lVector4.zw swizzling.
        /// </summary>
        public lVector2 zw => new lVector2(z, w);
        
        /// <summary>
        /// Returns lVector4.ba swizzling (equivalent to lVector4.zw).
        /// </summary>
        public lVector2 ba => new lVector2(z, w);
        
        /// <summary>
        /// Returns lVector4.zwx swizzling.
        /// </summary>
        public lVector3 zwx => new lVector3(z, w, x);
        
        /// <summary>
        /// Returns lVector4.bar swizzling (equivalent to lVector4.zwx).
        /// </summary>
        public lVector3 bar => new lVector3(z, w, x);
        
        /// <summary>
        /// Returns lVector4.zwxx swizzling.
        /// </summary>
        public lVector4 zwxx => new lVector4(z, w, x, x);
        
        /// <summary>
        /// Returns lVector4.barr swizzling (equivalent to lVector4.zwxx).
        /// </summary>
        public lVector4 barr => new lVector4(z, w, x, x);
        
        /// <summary>
        /// Returns lVector4.zwxy swizzling.
        /// </summary>
        public lVector4 zwxy => new lVector4(z, w, x, y);
        
        /// <summary>
        /// Returns lVector4.barg swizzling (equivalent to lVector4.zwxy).
        /// </summary>
        public lVector4 barg => new lVector4(z, w, x, y);
        
        /// <summary>
        /// Returns lVector4.zwxz swizzling.
        /// </summary>
        public lVector4 zwxz => new lVector4(z, w, x, z);
        
        /// <summary>
        /// Returns lVector4.barb swizzling (equivalent to lVector4.zwxz).
        /// </summary>
        public lVector4 barb => new lVector4(z, w, x, z);
        
        /// <summary>
        /// Returns lVector4.zwxw swizzling.
        /// </summary>
        public lVector4 zwxw => new lVector4(z, w, x, w);
        
        /// <summary>
        /// Returns lVector4.bara swizzling (equivalent to lVector4.zwxw).
        /// </summary>
        public lVector4 bara => new lVector4(z, w, x, w);
        
        /// <summary>
        /// Returns lVector4.zwy swizzling.
        /// </summary>
        public lVector3 zwy => new lVector3(z, w, y);
        
        /// <summary>
        /// Returns lVector4.bag swizzling (equivalent to lVector4.zwy).
        /// </summary>
        public lVector3 bag => new lVector3(z, w, y);
        
        /// <summary>
        /// Returns lVector4.zwyx swizzling.
        /// </summary>
        public lVector4 zwyx => new lVector4(z, w, y, x);
        
        /// <summary>
        /// Returns lVector4.bagr swizzling (equivalent to lVector4.zwyx).
        /// </summary>
        public lVector4 bagr => new lVector4(z, w, y, x);
        
        /// <summary>
        /// Returns lVector4.zwyy swizzling.
        /// </summary>
        public lVector4 zwyy => new lVector4(z, w, y, y);
        
        /// <summary>
        /// Returns lVector4.bagg swizzling (equivalent to lVector4.zwyy).
        /// </summary>
        public lVector4 bagg => new lVector4(z, w, y, y);
        
        /// <summary>
        /// Returns lVector4.zwyz swizzling.
        /// </summary>
        public lVector4 zwyz => new lVector4(z, w, y, z);
        
        /// <summary>
        /// Returns lVector4.bagb swizzling (equivalent to lVector4.zwyz).
        /// </summary>
        public lVector4 bagb => new lVector4(z, w, y, z);
        
        /// <summary>
        /// Returns lVector4.zwyw swizzling.
        /// </summary>
        public lVector4 zwyw => new lVector4(z, w, y, w);
        
        /// <summary>
        /// Returns lVector4.baga swizzling (equivalent to lVector4.zwyw).
        /// </summary>
        public lVector4 baga => new lVector4(z, w, y, w);
        
        /// <summary>
        /// Returns lVector4.zwz swizzling.
        /// </summary>
        public lVector3 zwz => new lVector3(z, w, z);
        
        /// <summary>
        /// Returns lVector4.bab swizzling (equivalent to lVector4.zwz).
        /// </summary>
        public lVector3 bab => new lVector3(z, w, z);
        
        /// <summary>
        /// Returns lVector4.zwzx swizzling.
        /// </summary>
        public lVector4 zwzx => new lVector4(z, w, z, x);
        
        /// <summary>
        /// Returns lVector4.babr swizzling (equivalent to lVector4.zwzx).
        /// </summary>
        public lVector4 babr => new lVector4(z, w, z, x);
        
        /// <summary>
        /// Returns lVector4.zwzy swizzling.
        /// </summary>
        public lVector4 zwzy => new lVector4(z, w, z, y);
        
        /// <summary>
        /// Returns lVector4.babg swizzling (equivalent to lVector4.zwzy).
        /// </summary>
        public lVector4 babg => new lVector4(z, w, z, y);
        
        /// <summary>
        /// Returns lVector4.zwzz swizzling.
        /// </summary>
        public lVector4 zwzz => new lVector4(z, w, z, z);
        
        /// <summary>
        /// Returns lVector4.babb swizzling (equivalent to lVector4.zwzz).
        /// </summary>
        public lVector4 babb => new lVector4(z, w, z, z);
        
        /// <summary>
        /// Returns lVector4.zwzw swizzling.
        /// </summary>
        public lVector4 zwzw => new lVector4(z, w, z, w);
        
        /// <summary>
        /// Returns lVector4.baba swizzling (equivalent to lVector4.zwzw).
        /// </summary>
        public lVector4 baba => new lVector4(z, w, z, w);
        
        /// <summary>
        /// Returns lVector4.zww swizzling.
        /// </summary>
        public lVector3 zww => new lVector3(z, w, w);
        
        /// <summary>
        /// Returns lVector4.baa swizzling (equivalent to lVector4.zww).
        /// </summary>
        public lVector3 baa => new lVector3(z, w, w);
        
        /// <summary>
        /// Returns lVector4.zwwx swizzling.
        /// </summary>
        public lVector4 zwwx => new lVector4(z, w, w, x);
        
        /// <summary>
        /// Returns lVector4.baar swizzling (equivalent to lVector4.zwwx).
        /// </summary>
        public lVector4 baar => new lVector4(z, w, w, x);
        
        /// <summary>
        /// Returns lVector4.zwwy swizzling.
        /// </summary>
        public lVector4 zwwy => new lVector4(z, w, w, y);
        
        /// <summary>
        /// Returns lVector4.baag swizzling (equivalent to lVector4.zwwy).
        /// </summary>
        public lVector4 baag => new lVector4(z, w, w, y);
        
        /// <summary>
        /// Returns lVector4.zwwz swizzling.
        /// </summary>
        public lVector4 zwwz => new lVector4(z, w, w, z);
        
        /// <summary>
        /// Returns lVector4.baab swizzling (equivalent to lVector4.zwwz).
        /// </summary>
        public lVector4 baab => new lVector4(z, w, w, z);
        
        /// <summary>
        /// Returns lVector4.zwww swizzling.
        /// </summary>
        public lVector4 zwww => new lVector4(z, w, w, w);
        
        /// <summary>
        /// Returns lVector4.baaa swizzling (equivalent to lVector4.zwww).
        /// </summary>
        public lVector4 baaa => new lVector4(z, w, w, w);
        
        /// <summary>
        /// Returns lVector4.wx swizzling.
        /// </summary>
        public lVector2 wx => new lVector2(w, x);
        
        /// <summary>
        /// Returns lVector4.ar swizzling (equivalent to lVector4.wx).
        /// </summary>
        public lVector2 ar => new lVector2(w, x);
        
        /// <summary>
        /// Returns lVector4.wxx swizzling.
        /// </summary>
        public lVector3 wxx => new lVector3(w, x, x);
        
        /// <summary>
        /// Returns lVector4.arr swizzling (equivalent to lVector4.wxx).
        /// </summary>
        public lVector3 arr => new lVector3(w, x, x);
        
        /// <summary>
        /// Returns lVector4.wxxx swizzling.
        /// </summary>
        public lVector4 wxxx => new lVector4(w, x, x, x);
        
        /// <summary>
        /// Returns lVector4.arrr swizzling (equivalent to lVector4.wxxx).
        /// </summary>
        public lVector4 arrr => new lVector4(w, x, x, x);
        
        /// <summary>
        /// Returns lVector4.wxxy swizzling.
        /// </summary>
        public lVector4 wxxy => new lVector4(w, x, x, y);
        
        /// <summary>
        /// Returns lVector4.arrg swizzling (equivalent to lVector4.wxxy).
        /// </summary>
        public lVector4 arrg => new lVector4(w, x, x, y);
        
        /// <summary>
        /// Returns lVector4.wxxz swizzling.
        /// </summary>
        public lVector4 wxxz => new lVector4(w, x, x, z);
        
        /// <summary>
        /// Returns lVector4.arrb swizzling (equivalent to lVector4.wxxz).
        /// </summary>
        public lVector4 arrb => new lVector4(w, x, x, z);
        
        /// <summary>
        /// Returns lVector4.wxxw swizzling.
        /// </summary>
        public lVector4 wxxw => new lVector4(w, x, x, w);
        
        /// <summary>
        /// Returns lVector4.arra swizzling (equivalent to lVector4.wxxw).
        /// </summary>
        public lVector4 arra => new lVector4(w, x, x, w);
        
        /// <summary>
        /// Returns lVector4.wxy swizzling.
        /// </summary>
        public lVector3 wxy => new lVector3(w, x, y);
        
        /// <summary>
        /// Returns lVector4.arg swizzling (equivalent to lVector4.wxy).
        /// </summary>
        public lVector3 arg => new lVector3(w, x, y);
        
        /// <summary>
        /// Returns lVector4.wxyx swizzling.
        /// </summary>
        public lVector4 wxyx => new lVector4(w, x, y, x);
        
        /// <summary>
        /// Returns lVector4.argr swizzling (equivalent to lVector4.wxyx).
        /// </summary>
        public lVector4 argr => new lVector4(w, x, y, x);
        
        /// <summary>
        /// Returns lVector4.wxyy swizzling.
        /// </summary>
        public lVector4 wxyy => new lVector4(w, x, y, y);
        
        /// <summary>
        /// Returns lVector4.argg swizzling (equivalent to lVector4.wxyy).
        /// </summary>
        public lVector4 argg => new lVector4(w, x, y, y);
        
        /// <summary>
        /// Returns lVector4.wxyz swizzling.
        /// </summary>
        public lVector4 wxyz => new lVector4(w, x, y, z);
        
        /// <summary>
        /// Returns lVector4.argb swizzling (equivalent to lVector4.wxyz).
        /// </summary>
        public lVector4 argb => new lVector4(w, x, y, z);
        
        /// <summary>
        /// Returns lVector4.wxyw swizzling.
        /// </summary>
        public lVector4 wxyw => new lVector4(w, x, y, w);
        
        /// <summary>
        /// Returns lVector4.arga swizzling (equivalent to lVector4.wxyw).
        /// </summary>
        public lVector4 arga => new lVector4(w, x, y, w);
        
        /// <summary>
        /// Returns lVector4.wxz swizzling.
        /// </summary>
        public lVector3 wxz => new lVector3(w, x, z);
        
        /// <summary>
        /// Returns lVector4.arb swizzling (equivalent to lVector4.wxz).
        /// </summary>
        public lVector3 arb => new lVector3(w, x, z);
        
        /// <summary>
        /// Returns lVector4.wxzx swizzling.
        /// </summary>
        public lVector4 wxzx => new lVector4(w, x, z, x);
        
        /// <summary>
        /// Returns lVector4.arbr swizzling (equivalent to lVector4.wxzx).
        /// </summary>
        public lVector4 arbr => new lVector4(w, x, z, x);
        
        /// <summary>
        /// Returns lVector4.wxzy swizzling.
        /// </summary>
        public lVector4 wxzy => new lVector4(w, x, z, y);
        
        /// <summary>
        /// Returns lVector4.arbg swizzling (equivalent to lVector4.wxzy).
        /// </summary>
        public lVector4 arbg => new lVector4(w, x, z, y);
        
        /// <summary>
        /// Returns lVector4.wxzz swizzling.
        /// </summary>
        public lVector4 wxzz => new lVector4(w, x, z, z);
        
        /// <summary>
        /// Returns lVector4.arbb swizzling (equivalent to lVector4.wxzz).
        /// </summary>
        public lVector4 arbb => new lVector4(w, x, z, z);
        
        /// <summary>
        /// Returns lVector4.wxzw swizzling.
        /// </summary>
        public lVector4 wxzw => new lVector4(w, x, z, w);
        
        /// <summary>
        /// Returns lVector4.arba swizzling (equivalent to lVector4.wxzw).
        /// </summary>
        public lVector4 arba => new lVector4(w, x, z, w);
        
        /// <summary>
        /// Returns lVector4.wxw swizzling.
        /// </summary>
        public lVector3 wxw => new lVector3(w, x, w);
        
        /// <summary>
        /// Returns lVector4.ara swizzling (equivalent to lVector4.wxw).
        /// </summary>
        public lVector3 ara => new lVector3(w, x, w);
        
        /// <summary>
        /// Returns lVector4.wxwx swizzling.
        /// </summary>
        public lVector4 wxwx => new lVector4(w, x, w, x);
        
        /// <summary>
        /// Returns lVector4.arar swizzling (equivalent to lVector4.wxwx).
        /// </summary>
        public lVector4 arar => new lVector4(w, x, w, x);
        
        /// <summary>
        /// Returns lVector4.wxwy swizzling.
        /// </summary>
        public lVector4 wxwy => new lVector4(w, x, w, y);
        
        /// <summary>
        /// Returns lVector4.arag swizzling (equivalent to lVector4.wxwy).
        /// </summary>
        public lVector4 arag => new lVector4(w, x, w, y);
        
        /// <summary>
        /// Returns lVector4.wxwz swizzling.
        /// </summary>
        public lVector4 wxwz => new lVector4(w, x, w, z);
        
        /// <summary>
        /// Returns lVector4.arab swizzling (equivalent to lVector4.wxwz).
        /// </summary>
        public lVector4 arab => new lVector4(w, x, w, z);
        
        /// <summary>
        /// Returns lVector4.wxww swizzling.
        /// </summary>
        public lVector4 wxww => new lVector4(w, x, w, w);
        
        /// <summary>
        /// Returns lVector4.araa swizzling (equivalent to lVector4.wxww).
        /// </summary>
        public lVector4 araa => new lVector4(w, x, w, w);
        
        /// <summary>
        /// Returns lVector4.wy swizzling.
        /// </summary>
        public lVector2 wy => new lVector2(w, y);
        
        /// <summary>
        /// Returns lVector4.ag swizzling (equivalent to lVector4.wy).
        /// </summary>
        public lVector2 ag => new lVector2(w, y);
        
        /// <summary>
        /// Returns lVector4.wyx swizzling.
        /// </summary>
        public lVector3 wyx => new lVector3(w, y, x);
        
        /// <summary>
        /// Returns lVector4.agr swizzling (equivalent to lVector4.wyx).
        /// </summary>
        public lVector3 agr => new lVector3(w, y, x);
        
        /// <summary>
        /// Returns lVector4.wyxx swizzling.
        /// </summary>
        public lVector4 wyxx => new lVector4(w, y, x, x);
        
        /// <summary>
        /// Returns lVector4.agrr swizzling (equivalent to lVector4.wyxx).
        /// </summary>
        public lVector4 agrr => new lVector4(w, y, x, x);
        
        /// <summary>
        /// Returns lVector4.wyxy swizzling.
        /// </summary>
        public lVector4 wyxy => new lVector4(w, y, x, y);
        
        /// <summary>
        /// Returns lVector4.agrg swizzling (equivalent to lVector4.wyxy).
        /// </summary>
        public lVector4 agrg => new lVector4(w, y, x, y);
        
        /// <summary>
        /// Returns lVector4.wyxz swizzling.
        /// </summary>
        public lVector4 wyxz => new lVector4(w, y, x, z);
        
        /// <summary>
        /// Returns lVector4.agrb swizzling (equivalent to lVector4.wyxz).
        /// </summary>
        public lVector4 agrb => new lVector4(w, y, x, z);
        
        /// <summary>
        /// Returns lVector4.wyxw swizzling.
        /// </summary>
        public lVector4 wyxw => new lVector4(w, y, x, w);
        
        /// <summary>
        /// Returns lVector4.agra swizzling (equivalent to lVector4.wyxw).
        /// </summary>
        public lVector4 agra => new lVector4(w, y, x, w);
        
        /// <summary>
        /// Returns lVector4.wyy swizzling.
        /// </summary>
        public lVector3 wyy => new lVector3(w, y, y);
        
        /// <summary>
        /// Returns lVector4.agg swizzling (equivalent to lVector4.wyy).
        /// </summary>
        public lVector3 agg => new lVector3(w, y, y);
        
        /// <summary>
        /// Returns lVector4.wyyx swizzling.
        /// </summary>
        public lVector4 wyyx => new lVector4(w, y, y, x);
        
        /// <summary>
        /// Returns lVector4.aggr swizzling (equivalent to lVector4.wyyx).
        /// </summary>
        public lVector4 aggr => new lVector4(w, y, y, x);
        
        /// <summary>
        /// Returns lVector4.wyyy swizzling.
        /// </summary>
        public lVector4 wyyy => new lVector4(w, y, y, y);
        
        /// <summary>
        /// Returns lVector4.aggg swizzling (equivalent to lVector4.wyyy).
        /// </summary>
        public lVector4 aggg => new lVector4(w, y, y, y);
        
        /// <summary>
        /// Returns lVector4.wyyz swizzling.
        /// </summary>
        public lVector4 wyyz => new lVector4(w, y, y, z);
        
        /// <summary>
        /// Returns lVector4.aggb swizzling (equivalent to lVector4.wyyz).
        /// </summary>
        public lVector4 aggb => new lVector4(w, y, y, z);
        
        /// <summary>
        /// Returns lVector4.wyyw swizzling.
        /// </summary>
        public lVector4 wyyw => new lVector4(w, y, y, w);
        
        /// <summary>
        /// Returns lVector4.agga swizzling (equivalent to lVector4.wyyw).
        /// </summary>
        public lVector4 agga => new lVector4(w, y, y, w);
        
        /// <summary>
        /// Returns lVector4.wyz swizzling.
        /// </summary>
        public lVector3 wyz => new lVector3(w, y, z);
        
        /// <summary>
        /// Returns lVector4.agb swizzling (equivalent to lVector4.wyz).
        /// </summary>
        public lVector3 agb => new lVector3(w, y, z);
        
        /// <summary>
        /// Returns lVector4.wyzx swizzling.
        /// </summary>
        public lVector4 wyzx => new lVector4(w, y, z, x);
        
        /// <summary>
        /// Returns lVector4.agbr swizzling (equivalent to lVector4.wyzx).
        /// </summary>
        public lVector4 agbr => new lVector4(w, y, z, x);
        
        /// <summary>
        /// Returns lVector4.wyzy swizzling.
        /// </summary>
        public lVector4 wyzy => new lVector4(w, y, z, y);
        
        /// <summary>
        /// Returns lVector4.agbg swizzling (equivalent to lVector4.wyzy).
        /// </summary>
        public lVector4 agbg => new lVector4(w, y, z, y);
        
        /// <summary>
        /// Returns lVector4.wyzz swizzling.
        /// </summary>
        public lVector4 wyzz => new lVector4(w, y, z, z);
        
        /// <summary>
        /// Returns lVector4.agbb swizzling (equivalent to lVector4.wyzz).
        /// </summary>
        public lVector4 agbb => new lVector4(w, y, z, z);
        
        /// <summary>
        /// Returns lVector4.wyzw swizzling.
        /// </summary>
        public lVector4 wyzw => new lVector4(w, y, z, w);
        
        /// <summary>
        /// Returns lVector4.agba swizzling (equivalent to lVector4.wyzw).
        /// </summary>
        public lVector4 agba => new lVector4(w, y, z, w);
        
        /// <summary>
        /// Returns lVector4.wyw swizzling.
        /// </summary>
        public lVector3 wyw => new lVector3(w, y, w);
        
        /// <summary>
        /// Returns lVector4.aga swizzling (equivalent to lVector4.wyw).
        /// </summary>
        public lVector3 aga => new lVector3(w, y, w);
        
        /// <summary>
        /// Returns lVector4.wywx swizzling.
        /// </summary>
        public lVector4 wywx => new lVector4(w, y, w, x);
        
        /// <summary>
        /// Returns lVector4.agar swizzling (equivalent to lVector4.wywx).
        /// </summary>
        public lVector4 agar => new lVector4(w, y, w, x);
        
        /// <summary>
        /// Returns lVector4.wywy swizzling.
        /// </summary>
        public lVector4 wywy => new lVector4(w, y, w, y);
        
        /// <summary>
        /// Returns lVector4.agag swizzling (equivalent to lVector4.wywy).
        /// </summary>
        public lVector4 agag => new lVector4(w, y, w, y);
        
        /// <summary>
        /// Returns lVector4.wywz swizzling.
        /// </summary>
        public lVector4 wywz => new lVector4(w, y, w, z);
        
        /// <summary>
        /// Returns lVector4.agab swizzling (equivalent to lVector4.wywz).
        /// </summary>
        public lVector4 agab => new lVector4(w, y, w, z);
        
        /// <summary>
        /// Returns lVector4.wyww swizzling.
        /// </summary>
        public lVector4 wyww => new lVector4(w, y, w, w);
        
        /// <summary>
        /// Returns lVector4.agaa swizzling (equivalent to lVector4.wyww).
        /// </summary>
        public lVector4 agaa => new lVector4(w, y, w, w);
        
        /// <summary>
        /// Returns lVector4.wz swizzling.
        /// </summary>
        public lVector2 wz => new lVector2(w, z);
        
        /// <summary>
        /// Returns lVector4.ab swizzling (equivalent to lVector4.wz).
        /// </summary>
        public lVector2 ab => new lVector2(w, z);
        
        /// <summary>
        /// Returns lVector4.wzx swizzling.
        /// </summary>
        public lVector3 wzx => new lVector3(w, z, x);
        
        /// <summary>
        /// Returns lVector4.abr swizzling (equivalent to lVector4.wzx).
        /// </summary>
        public lVector3 abr => new lVector3(w, z, x);
        
        /// <summary>
        /// Returns lVector4.wzxx swizzling.
        /// </summary>
        public lVector4 wzxx => new lVector4(w, z, x, x);
        
        /// <summary>
        /// Returns lVector4.abrr swizzling (equivalent to lVector4.wzxx).
        /// </summary>
        public lVector4 abrr => new lVector4(w, z, x, x);
        
        /// <summary>
        /// Returns lVector4.wzxy swizzling.
        /// </summary>
        public lVector4 wzxy => new lVector4(w, z, x, y);
        
        /// <summary>
        /// Returns lVector4.abrg swizzling (equivalent to lVector4.wzxy).
        /// </summary>
        public lVector4 abrg => new lVector4(w, z, x, y);
        
        /// <summary>
        /// Returns lVector4.wzxz swizzling.
        /// </summary>
        public lVector4 wzxz => new lVector4(w, z, x, z);
        
        /// <summary>
        /// Returns lVector4.abrb swizzling (equivalent to lVector4.wzxz).
        /// </summary>
        public lVector4 abrb => new lVector4(w, z, x, z);
        
        /// <summary>
        /// Returns lVector4.wzxw swizzling.
        /// </summary>
        public lVector4 wzxw => new lVector4(w, z, x, w);
        
        /// <summary>
        /// Returns lVector4.abra swizzling (equivalent to lVector4.wzxw).
        /// </summary>
        public lVector4 abra => new lVector4(w, z, x, w);
        
        /// <summary>
        /// Returns lVector4.wzy swizzling.
        /// </summary>
        public lVector3 wzy => new lVector3(w, z, y);
        
        /// <summary>
        /// Returns lVector4.abg swizzling (equivalent to lVector4.wzy).
        /// </summary>
        public lVector3 abg => new lVector3(w, z, y);
        
        /// <summary>
        /// Returns lVector4.wzyx swizzling.
        /// </summary>
        public lVector4 wzyx => new lVector4(w, z, y, x);
        
        /// <summary>
        /// Returns lVector4.abgr swizzling (equivalent to lVector4.wzyx).
        /// </summary>
        public lVector4 abgr => new lVector4(w, z, y, x);
        
        /// <summary>
        /// Returns lVector4.wzyy swizzling.
        /// </summary>
        public lVector4 wzyy => new lVector4(w, z, y, y);
        
        /// <summary>
        /// Returns lVector4.abgg swizzling (equivalent to lVector4.wzyy).
        /// </summary>
        public lVector4 abgg => new lVector4(w, z, y, y);
        
        /// <summary>
        /// Returns lVector4.wzyz swizzling.
        /// </summary>
        public lVector4 wzyz => new lVector4(w, z, y, z);
        
        /// <summary>
        /// Returns lVector4.abgb swizzling (equivalent to lVector4.wzyz).
        /// </summary>
        public lVector4 abgb => new lVector4(w, z, y, z);
        
        /// <summary>
        /// Returns lVector4.wzyw swizzling.
        /// </summary>
        public lVector4 wzyw => new lVector4(w, z, y, w);
        
        /// <summary>
        /// Returns lVector4.abga swizzling (equivalent to lVector4.wzyw).
        /// </summary>
        public lVector4 abga => new lVector4(w, z, y, w);
        
        /// <summary>
        /// Returns lVector4.wzz swizzling.
        /// </summary>
        public lVector3 wzz => new lVector3(w, z, z);
        
        /// <summary>
        /// Returns lVector4.abb swizzling (equivalent to lVector4.wzz).
        /// </summary>
        public lVector3 abb => new lVector3(w, z, z);
        
        /// <summary>
        /// Returns lVector4.wzzx swizzling.
        /// </summary>
        public lVector4 wzzx => new lVector4(w, z, z, x);
        
        /// <summary>
        /// Returns lVector4.abbr swizzling (equivalent to lVector4.wzzx).
        /// </summary>
        public lVector4 abbr => new lVector4(w, z, z, x);
        
        /// <summary>
        /// Returns lVector4.wzzy swizzling.
        /// </summary>
        public lVector4 wzzy => new lVector4(w, z, z, y);
        
        /// <summary>
        /// Returns lVector4.abbg swizzling (equivalent to lVector4.wzzy).
        /// </summary>
        public lVector4 abbg => new lVector4(w, z, z, y);
        
        /// <summary>
        /// Returns lVector4.wzzz swizzling.
        /// </summary>
        public lVector4 wzzz => new lVector4(w, z, z, z);
        
        /// <summary>
        /// Returns lVector4.abbb swizzling (equivalent to lVector4.wzzz).
        /// </summary>
        public lVector4 abbb => new lVector4(w, z, z, z);
        
        /// <summary>
        /// Returns lVector4.wzzw swizzling.
        /// </summary>
        public lVector4 wzzw => new lVector4(w, z, z, w);
        
        /// <summary>
        /// Returns lVector4.abba swizzling (equivalent to lVector4.wzzw).
        /// </summary>
        public lVector4 abba => new lVector4(w, z, z, w);
        
        /// <summary>
        /// Returns lVector4.wzw swizzling.
        /// </summary>
        public lVector3 wzw => new lVector3(w, z, w);
        
        /// <summary>
        /// Returns lVector4.aba swizzling (equivalent to lVector4.wzw).
        /// </summary>
        public lVector3 aba => new lVector3(w, z, w);
        
        /// <summary>
        /// Returns lVector4.wzwx swizzling.
        /// </summary>
        public lVector4 wzwx => new lVector4(w, z, w, x);
        
        /// <summary>
        /// Returns lVector4.abar swizzling (equivalent to lVector4.wzwx).
        /// </summary>
        public lVector4 abar => new lVector4(w, z, w, x);
        
        /// <summary>
        /// Returns lVector4.wzwy swizzling.
        /// </summary>
        public lVector4 wzwy => new lVector4(w, z, w, y);
        
        /// <summary>
        /// Returns lVector4.abag swizzling (equivalent to lVector4.wzwy).
        /// </summary>
        public lVector4 abag => new lVector4(w, z, w, y);
        
        /// <summary>
        /// Returns lVector4.wzwz swizzling.
        /// </summary>
        public lVector4 wzwz => new lVector4(w, z, w, z);
        
        /// <summary>
        /// Returns lVector4.abab swizzling (equivalent to lVector4.wzwz).
        /// </summary>
        public lVector4 abab => new lVector4(w, z, w, z);
        
        /// <summary>
        /// Returns lVector4.wzww swizzling.
        /// </summary>
        public lVector4 wzww => new lVector4(w, z, w, w);
        
        /// <summary>
        /// Returns lVector4.abaa swizzling (equivalent to lVector4.wzww).
        /// </summary>
        public lVector4 abaa => new lVector4(w, z, w, w);
        
        /// <summary>
        /// Returns lVector4.ww swizzling.
        /// </summary>
        public lVector2 ww => new lVector2(w, w);
        
        /// <summary>
        /// Returns lVector4.aa swizzling (equivalent to lVector4.ww).
        /// </summary>
        public lVector2 aa => new lVector2(w, w);
        
        /// <summary>
        /// Returns lVector4.wwx swizzling.
        /// </summary>
        public lVector3 wwx => new lVector3(w, w, x);
        
        /// <summary>
        /// Returns lVector4.aar swizzling (equivalent to lVector4.wwx).
        /// </summary>
        public lVector3 aar => new lVector3(w, w, x);
        
        /// <summary>
        /// Returns lVector4.wwxx swizzling.
        /// </summary>
        public lVector4 wwxx => new lVector4(w, w, x, x);
        
        /// <summary>
        /// Returns lVector4.aarr swizzling (equivalent to lVector4.wwxx).
        /// </summary>
        public lVector4 aarr => new lVector4(w, w, x, x);
        
        /// <summary>
        /// Returns lVector4.wwxy swizzling.
        /// </summary>
        public lVector4 wwxy => new lVector4(w, w, x, y);
        
        /// <summary>
        /// Returns lVector4.aarg swizzling (equivalent to lVector4.wwxy).
        /// </summary>
        public lVector4 aarg => new lVector4(w, w, x, y);
        
        /// <summary>
        /// Returns lVector4.wwxz swizzling.
        /// </summary>
        public lVector4 wwxz => new lVector4(w, w, x, z);
        
        /// <summary>
        /// Returns lVector4.aarb swizzling (equivalent to lVector4.wwxz).
        /// </summary>
        public lVector4 aarb => new lVector4(w, w, x, z);
        
        /// <summary>
        /// Returns lVector4.wwxw swizzling.
        /// </summary>
        public lVector4 wwxw => new lVector4(w, w, x, w);
        
        /// <summary>
        /// Returns lVector4.aara swizzling (equivalent to lVector4.wwxw).
        /// </summary>
        public lVector4 aara => new lVector4(w, w, x, w);
        
        /// <summary>
        /// Returns lVector4.wwy swizzling.
        /// </summary>
        public lVector3 wwy => new lVector3(w, w, y);
        
        /// <summary>
        /// Returns lVector4.aag swizzling (equivalent to lVector4.wwy).
        /// </summary>
        public lVector3 aag => new lVector3(w, w, y);
        
        /// <summary>
        /// Returns lVector4.wwyx swizzling.
        /// </summary>
        public lVector4 wwyx => new lVector4(w, w, y, x);
        
        /// <summary>
        /// Returns lVector4.aagr swizzling (equivalent to lVector4.wwyx).
        /// </summary>
        public lVector4 aagr => new lVector4(w, w, y, x);
        
        /// <summary>
        /// Returns lVector4.wwyy swizzling.
        /// </summary>
        public lVector4 wwyy => new lVector4(w, w, y, y);
        
        /// <summary>
        /// Returns lVector4.aagg swizzling (equivalent to lVector4.wwyy).
        /// </summary>
        public lVector4 aagg => new lVector4(w, w, y, y);
        
        /// <summary>
        /// Returns lVector4.wwyz swizzling.
        /// </summary>
        public lVector4 wwyz => new lVector4(w, w, y, z);
        
        /// <summary>
        /// Returns lVector4.aagb swizzling (equivalent to lVector4.wwyz).
        /// </summary>
        public lVector4 aagb => new lVector4(w, w, y, z);
        
        /// <summary>
        /// Returns lVector4.wwyw swizzling.
        /// </summary>
        public lVector4 wwyw => new lVector4(w, w, y, w);
        
        /// <summary>
        /// Returns lVector4.aaga swizzling (equivalent to lVector4.wwyw).
        /// </summary>
        public lVector4 aaga => new lVector4(w, w, y, w);
        
        /// <summary>
        /// Returns lVector4.wwz swizzling.
        /// </summary>
        public lVector3 wwz => new lVector3(w, w, z);
        
        /// <summary>
        /// Returns lVector4.aab swizzling (equivalent to lVector4.wwz).
        /// </summary>
        public lVector3 aab => new lVector3(w, w, z);
        
        /// <summary>
        /// Returns lVector4.wwzx swizzling.
        /// </summary>
        public lVector4 wwzx => new lVector4(w, w, z, x);
        
        /// <summary>
        /// Returns lVector4.aabr swizzling (equivalent to lVector4.wwzx).
        /// </summary>
        public lVector4 aabr => new lVector4(w, w, z, x);
        
        /// <summary>
        /// Returns lVector4.wwzy swizzling.
        /// </summary>
        public lVector4 wwzy => new lVector4(w, w, z, y);
        
        /// <summary>
        /// Returns lVector4.aabg swizzling (equivalent to lVector4.wwzy).
        /// </summary>
        public lVector4 aabg => new lVector4(w, w, z, y);
        
        /// <summary>
        /// Returns lVector4.wwzz swizzling.
        /// </summary>
        public lVector4 wwzz => new lVector4(w, w, z, z);
        
        /// <summary>
        /// Returns lVector4.aabb swizzling (equivalent to lVector4.wwzz).
        /// </summary>
        public lVector4 aabb => new lVector4(w, w, z, z);
        
        /// <summary>
        /// Returns lVector4.wwzw swizzling.
        /// </summary>
        public lVector4 wwzw => new lVector4(w, w, z, w);
        
        /// <summary>
        /// Returns lVector4.aaba swizzling (equivalent to lVector4.wwzw).
        /// </summary>
        public lVector4 aaba => new lVector4(w, w, z, w);
        
        /// <summary>
        /// Returns lVector4.www swizzling.
        /// </summary>
        public lVector3 www => new lVector3(w, w, w);
        
        /// <summary>
        /// Returns lVector4.aaa swizzling (equivalent to lVector4.www).
        /// </summary>
        public lVector3 aaa => new lVector3(w, w, w);
        
        /// <summary>
        /// Returns lVector4.wwwx swizzling.
        /// </summary>
        public lVector4 wwwx => new lVector4(w, w, w, x);
        
        /// <summary>
        /// Returns lVector4.aaar swizzling (equivalent to lVector4.wwwx).
        /// </summary>
        public lVector4 aaar => new lVector4(w, w, w, x);
        
        /// <summary>
        /// Returns lVector4.wwwy swizzling.
        /// </summary>
        public lVector4 wwwy => new lVector4(w, w, w, y);
        
        /// <summary>
        /// Returns lVector4.aaag swizzling (equivalent to lVector4.wwwy).
        /// </summary>
        public lVector4 aaag => new lVector4(w, w, w, y);
        
        /// <summary>
        /// Returns lVector4.wwwz swizzling.
        /// </summary>
        public lVector4 wwwz => new lVector4(w, w, w, z);
        
        /// <summary>
        /// Returns lVector4.aaab swizzling (equivalent to lVector4.wwwz).
        /// </summary>
        public lVector4 aaab => new lVector4(w, w, w, z);
        
        /// <summary>
        /// Returns lVector4.wwww swizzling.
        /// </summary>
        public lVector4 wwww => new lVector4(w, w, w, w);
        
        /// <summary>
        /// Returns lVector4.aaaa swizzling (equivalent to lVector4.wwww).
        /// </summary>
        public lVector4 aaaa => new lVector4(w, w, w, w);

        #endregion

    }
}
