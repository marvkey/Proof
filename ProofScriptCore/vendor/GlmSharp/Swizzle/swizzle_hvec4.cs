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
    /// Temporary Vectortor of type Half with 4 components, used for implementing swizzling for hVector4.
    /// </summary>
    [RegisterCoreClassStruct]
   
    [StructLayout(LayoutKind.Sequential)]
    public struct swizzle_hVector4
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
        
        /// <summary>
        /// w-component
        /// </summary>
        
        internal readonly Half w;

        #endregion


        #region Constructors
        
        /// <summary>
        /// Constructor for swizzle_hVector4.
        /// </summary>
        internal swizzle_hVector4(Half x, Half y, Half z, Half w)
        {
            this.x = x;
            this.y = y;
            this.z = z;
            this.w = w;
        }

        #endregion


        #region Properties
        
        /// <summary>
        /// Returns hVector4.xx swizzling.
        /// </summary>
        public hVector2 xx => new hVector2(x, x);
        
        /// <summary>
        /// Returns hVector4.rr swizzling (equivalent to hVector4.xx).
        /// </summary>
        public hVector2 rr => new hVector2(x, x);
        
        /// <summary>
        /// Returns hVector4.xxx swizzling.
        /// </summary>
        public hVector3 xxx => new hVector3(x, x, x);
        
        /// <summary>
        /// Returns hVector4.rrr swizzling (equivalent to hVector4.xxx).
        /// </summary>
        public hVector3 rrr => new hVector3(x, x, x);
        
        /// <summary>
        /// Returns hVector4.xxxx swizzling.
        /// </summary>
        public hVector4 xxxx => new hVector4(x, x, x, x);
        
        /// <summary>
        /// Returns hVector4.rrrr swizzling (equivalent to hVector4.xxxx).
        /// </summary>
        public hVector4 rrrr => new hVector4(x, x, x, x);
        
        /// <summary>
        /// Returns hVector4.xxxy swizzling.
        /// </summary>
        public hVector4 xxxy => new hVector4(x, x, x, y);
        
        /// <summary>
        /// Returns hVector4.rrrg swizzling (equivalent to hVector4.xxxy).
        /// </summary>
        public hVector4 rrrg => new hVector4(x, x, x, y);
        
        /// <summary>
        /// Returns hVector4.xxxz swizzling.
        /// </summary>
        public hVector4 xxxz => new hVector4(x, x, x, z);
        
        /// <summary>
        /// Returns hVector4.rrrb swizzling (equivalent to hVector4.xxxz).
        /// </summary>
        public hVector4 rrrb => new hVector4(x, x, x, z);
        
        /// <summary>
        /// Returns hVector4.xxxw swizzling.
        /// </summary>
        public hVector4 xxxw => new hVector4(x, x, x, w);
        
        /// <summary>
        /// Returns hVector4.rrra swizzling (equivalent to hVector4.xxxw).
        /// </summary>
        public hVector4 rrra => new hVector4(x, x, x, w);
        
        /// <summary>
        /// Returns hVector4.xxy swizzling.
        /// </summary>
        public hVector3 xxy => new hVector3(x, x, y);
        
        /// <summary>
        /// Returns hVector4.rrg swizzling (equivalent to hVector4.xxy).
        /// </summary>
        public hVector3 rrg => new hVector3(x, x, y);
        
        /// <summary>
        /// Returns hVector4.xxyx swizzling.
        /// </summary>
        public hVector4 xxyx => new hVector4(x, x, y, x);
        
        /// <summary>
        /// Returns hVector4.rrgr swizzling (equivalent to hVector4.xxyx).
        /// </summary>
        public hVector4 rrgr => new hVector4(x, x, y, x);
        
        /// <summary>
        /// Returns hVector4.xxyy swizzling.
        /// </summary>
        public hVector4 xxyy => new hVector4(x, x, y, y);
        
        /// <summary>
        /// Returns hVector4.rrgg swizzling (equivalent to hVector4.xxyy).
        /// </summary>
        public hVector4 rrgg => new hVector4(x, x, y, y);
        
        /// <summary>
        /// Returns hVector4.xxyz swizzling.
        /// </summary>
        public hVector4 xxyz => new hVector4(x, x, y, z);
        
        /// <summary>
        /// Returns hVector4.rrgb swizzling (equivalent to hVector4.xxyz).
        /// </summary>
        public hVector4 rrgb => new hVector4(x, x, y, z);
        
        /// <summary>
        /// Returns hVector4.xxyw swizzling.
        /// </summary>
        public hVector4 xxyw => new hVector4(x, x, y, w);
        
        /// <summary>
        /// Returns hVector4.rrga swizzling (equivalent to hVector4.xxyw).
        /// </summary>
        public hVector4 rrga => new hVector4(x, x, y, w);
        
        /// <summary>
        /// Returns hVector4.xxz swizzling.
        /// </summary>
        public hVector3 xxz => new hVector3(x, x, z);
        
        /// <summary>
        /// Returns hVector4.rrb swizzling (equivalent to hVector4.xxz).
        /// </summary>
        public hVector3 rrb => new hVector3(x, x, z);
        
        /// <summary>
        /// Returns hVector4.xxzx swizzling.
        /// </summary>
        public hVector4 xxzx => new hVector4(x, x, z, x);
        
        /// <summary>
        /// Returns hVector4.rrbr swizzling (equivalent to hVector4.xxzx).
        /// </summary>
        public hVector4 rrbr => new hVector4(x, x, z, x);
        
        /// <summary>
        /// Returns hVector4.xxzy swizzling.
        /// </summary>
        public hVector4 xxzy => new hVector4(x, x, z, y);
        
        /// <summary>
        /// Returns hVector4.rrbg swizzling (equivalent to hVector4.xxzy).
        /// </summary>
        public hVector4 rrbg => new hVector4(x, x, z, y);
        
        /// <summary>
        /// Returns hVector4.xxzz swizzling.
        /// </summary>
        public hVector4 xxzz => new hVector4(x, x, z, z);
        
        /// <summary>
        /// Returns hVector4.rrbb swizzling (equivalent to hVector4.xxzz).
        /// </summary>
        public hVector4 rrbb => new hVector4(x, x, z, z);
        
        /// <summary>
        /// Returns hVector4.xxzw swizzling.
        /// </summary>
        public hVector4 xxzw => new hVector4(x, x, z, w);
        
        /// <summary>
        /// Returns hVector4.rrba swizzling (equivalent to hVector4.xxzw).
        /// </summary>
        public hVector4 rrba => new hVector4(x, x, z, w);
        
        /// <summary>
        /// Returns hVector4.xxw swizzling.
        /// </summary>
        public hVector3 xxw => new hVector3(x, x, w);
        
        /// <summary>
        /// Returns hVector4.rra swizzling (equivalent to hVector4.xxw).
        /// </summary>
        public hVector3 rra => new hVector3(x, x, w);
        
        /// <summary>
        /// Returns hVector4.xxwx swizzling.
        /// </summary>
        public hVector4 xxwx => new hVector4(x, x, w, x);
        
        /// <summary>
        /// Returns hVector4.rrar swizzling (equivalent to hVector4.xxwx).
        /// </summary>
        public hVector4 rrar => new hVector4(x, x, w, x);
        
        /// <summary>
        /// Returns hVector4.xxwy swizzling.
        /// </summary>
        public hVector4 xxwy => new hVector4(x, x, w, y);
        
        /// <summary>
        /// Returns hVector4.rrag swizzling (equivalent to hVector4.xxwy).
        /// </summary>
        public hVector4 rrag => new hVector4(x, x, w, y);
        
        /// <summary>
        /// Returns hVector4.xxwz swizzling.
        /// </summary>
        public hVector4 xxwz => new hVector4(x, x, w, z);
        
        /// <summary>
        /// Returns hVector4.rrab swizzling (equivalent to hVector4.xxwz).
        /// </summary>
        public hVector4 rrab => new hVector4(x, x, w, z);
        
        /// <summary>
        /// Returns hVector4.xxww swizzling.
        /// </summary>
        public hVector4 xxww => new hVector4(x, x, w, w);
        
        /// <summary>
        /// Returns hVector4.rraa swizzling (equivalent to hVector4.xxww).
        /// </summary>
        public hVector4 rraa => new hVector4(x, x, w, w);
        
        /// <summary>
        /// Returns hVector4.xy swizzling.
        /// </summary>
        public hVector2 xy => new hVector2(x, y);
        
        /// <summary>
        /// Returns hVector4.rg swizzling (equivalent to hVector4.xy).
        /// </summary>
        public hVector2 rg => new hVector2(x, y);
        
        /// <summary>
        /// Returns hVector4.xyx swizzling.
        /// </summary>
        public hVector3 xyx => new hVector3(x, y, x);
        
        /// <summary>
        /// Returns hVector4.rgr swizzling (equivalent to hVector4.xyx).
        /// </summary>
        public hVector3 rgr => new hVector3(x, y, x);
        
        /// <summary>
        /// Returns hVector4.xyxx swizzling.
        /// </summary>
        public hVector4 xyxx => new hVector4(x, y, x, x);
        
        /// <summary>
        /// Returns hVector4.rgrr swizzling (equivalent to hVector4.xyxx).
        /// </summary>
        public hVector4 rgrr => new hVector4(x, y, x, x);
        
        /// <summary>
        /// Returns hVector4.xyxy swizzling.
        /// </summary>
        public hVector4 xyxy => new hVector4(x, y, x, y);
        
        /// <summary>
        /// Returns hVector4.rgrg swizzling (equivalent to hVector4.xyxy).
        /// </summary>
        public hVector4 rgrg => new hVector4(x, y, x, y);
        
        /// <summary>
        /// Returns hVector4.xyxz swizzling.
        /// </summary>
        public hVector4 xyxz => new hVector4(x, y, x, z);
        
        /// <summary>
        /// Returns hVector4.rgrb swizzling (equivalent to hVector4.xyxz).
        /// </summary>
        public hVector4 rgrb => new hVector4(x, y, x, z);
        
        /// <summary>
        /// Returns hVector4.xyxw swizzling.
        /// </summary>
        public hVector4 xyxw => new hVector4(x, y, x, w);
        
        /// <summary>
        /// Returns hVector4.rgra swizzling (equivalent to hVector4.xyxw).
        /// </summary>
        public hVector4 rgra => new hVector4(x, y, x, w);
        
        /// <summary>
        /// Returns hVector4.xyy swizzling.
        /// </summary>
        public hVector3 xyy => new hVector3(x, y, y);
        
        /// <summary>
        /// Returns hVector4.rgg swizzling (equivalent to hVector4.xyy).
        /// </summary>
        public hVector3 rgg => new hVector3(x, y, y);
        
        /// <summary>
        /// Returns hVector4.xyyx swizzling.
        /// </summary>
        public hVector4 xyyx => new hVector4(x, y, y, x);
        
        /// <summary>
        /// Returns hVector4.rggr swizzling (equivalent to hVector4.xyyx).
        /// </summary>
        public hVector4 rggr => new hVector4(x, y, y, x);
        
        /// <summary>
        /// Returns hVector4.xyyy swizzling.
        /// </summary>
        public hVector4 xyyy => new hVector4(x, y, y, y);
        
        /// <summary>
        /// Returns hVector4.rggg swizzling (equivalent to hVector4.xyyy).
        /// </summary>
        public hVector4 rggg => new hVector4(x, y, y, y);
        
        /// <summary>
        /// Returns hVector4.xyyz swizzling.
        /// </summary>
        public hVector4 xyyz => new hVector4(x, y, y, z);
        
        /// <summary>
        /// Returns hVector4.rggb swizzling (equivalent to hVector4.xyyz).
        /// </summary>
        public hVector4 rggb => new hVector4(x, y, y, z);
        
        /// <summary>
        /// Returns hVector4.xyyw swizzling.
        /// </summary>
        public hVector4 xyyw => new hVector4(x, y, y, w);
        
        /// <summary>
        /// Returns hVector4.rgga swizzling (equivalent to hVector4.xyyw).
        /// </summary>
        public hVector4 rgga => new hVector4(x, y, y, w);
        
        /// <summary>
        /// Returns hVector4.xyz swizzling.
        /// </summary>
        public hVector3 xyz => new hVector3(x, y, z);
        
        /// <summary>
        /// Returns hVector4.rgb swizzling (equivalent to hVector4.xyz).
        /// </summary>
        public hVector3 rgb => new hVector3(x, y, z);
        
        /// <summary>
        /// Returns hVector4.xyzx swizzling.
        /// </summary>
        public hVector4 xyzx => new hVector4(x, y, z, x);
        
        /// <summary>
        /// Returns hVector4.rgbr swizzling (equivalent to hVector4.xyzx).
        /// </summary>
        public hVector4 rgbr => new hVector4(x, y, z, x);
        
        /// <summary>
        /// Returns hVector4.xyzy swizzling.
        /// </summary>
        public hVector4 xyzy => new hVector4(x, y, z, y);
        
        /// <summary>
        /// Returns hVector4.rgbg swizzling (equivalent to hVector4.xyzy).
        /// </summary>
        public hVector4 rgbg => new hVector4(x, y, z, y);
        
        /// <summary>
        /// Returns hVector4.xyzz swizzling.
        /// </summary>
        public hVector4 xyzz => new hVector4(x, y, z, z);
        
        /// <summary>
        /// Returns hVector4.rgbb swizzling (equivalent to hVector4.xyzz).
        /// </summary>
        public hVector4 rgbb => new hVector4(x, y, z, z);
        
        /// <summary>
        /// Returns hVector4.xyzw swizzling.
        /// </summary>
        public hVector4 xyzw => new hVector4(x, y, z, w);
        
        /// <summary>
        /// Returns hVector4.rgba swizzling (equivalent to hVector4.xyzw).
        /// </summary>
        public hVector4 rgba => new hVector4(x, y, z, w);
        
        /// <summary>
        /// Returns hVector4.xyw swizzling.
        /// </summary>
        public hVector3 xyw => new hVector3(x, y, w);
        
        /// <summary>
        /// Returns hVector4.rga swizzling (equivalent to hVector4.xyw).
        /// </summary>
        public hVector3 rga => new hVector3(x, y, w);
        
        /// <summary>
        /// Returns hVector4.xywx swizzling.
        /// </summary>
        public hVector4 xywx => new hVector4(x, y, w, x);
        
        /// <summary>
        /// Returns hVector4.rgar swizzling (equivalent to hVector4.xywx).
        /// </summary>
        public hVector4 rgar => new hVector4(x, y, w, x);
        
        /// <summary>
        /// Returns hVector4.xywy swizzling.
        /// </summary>
        public hVector4 xywy => new hVector4(x, y, w, y);
        
        /// <summary>
        /// Returns hVector4.rgag swizzling (equivalent to hVector4.xywy).
        /// </summary>
        public hVector4 rgag => new hVector4(x, y, w, y);
        
        /// <summary>
        /// Returns hVector4.xywz swizzling.
        /// </summary>
        public hVector4 xywz => new hVector4(x, y, w, z);
        
        /// <summary>
        /// Returns hVector4.rgab swizzling (equivalent to hVector4.xywz).
        /// </summary>
        public hVector4 rgab => new hVector4(x, y, w, z);
        
        /// <summary>
        /// Returns hVector4.xyww swizzling.
        /// </summary>
        public hVector4 xyww => new hVector4(x, y, w, w);
        
        /// <summary>
        /// Returns hVector4.rgaa swizzling (equivalent to hVector4.xyww).
        /// </summary>
        public hVector4 rgaa => new hVector4(x, y, w, w);
        
        /// <summary>
        /// Returns hVector4.xz swizzling.
        /// </summary>
        public hVector2 xz => new hVector2(x, z);
        
        /// <summary>
        /// Returns hVector4.rb swizzling (equivalent to hVector4.xz).
        /// </summary>
        public hVector2 rb => new hVector2(x, z);
        
        /// <summary>
        /// Returns hVector4.xzx swizzling.
        /// </summary>
        public hVector3 xzx => new hVector3(x, z, x);
        
        /// <summary>
        /// Returns hVector4.rbr swizzling (equivalent to hVector4.xzx).
        /// </summary>
        public hVector3 rbr => new hVector3(x, z, x);
        
        /// <summary>
        /// Returns hVector4.xzxx swizzling.
        /// </summary>
        public hVector4 xzxx => new hVector4(x, z, x, x);
        
        /// <summary>
        /// Returns hVector4.rbrr swizzling (equivalent to hVector4.xzxx).
        /// </summary>
        public hVector4 rbrr => new hVector4(x, z, x, x);
        
        /// <summary>
        /// Returns hVector4.xzxy swizzling.
        /// </summary>
        public hVector4 xzxy => new hVector4(x, z, x, y);
        
        /// <summary>
        /// Returns hVector4.rbrg swizzling (equivalent to hVector4.xzxy).
        /// </summary>
        public hVector4 rbrg => new hVector4(x, z, x, y);
        
        /// <summary>
        /// Returns hVector4.xzxz swizzling.
        /// </summary>
        public hVector4 xzxz => new hVector4(x, z, x, z);
        
        /// <summary>
        /// Returns hVector4.rbrb swizzling (equivalent to hVector4.xzxz).
        /// </summary>
        public hVector4 rbrb => new hVector4(x, z, x, z);
        
        /// <summary>
        /// Returns hVector4.xzxw swizzling.
        /// </summary>
        public hVector4 xzxw => new hVector4(x, z, x, w);
        
        /// <summary>
        /// Returns hVector4.rbra swizzling (equivalent to hVector4.xzxw).
        /// </summary>
        public hVector4 rbra => new hVector4(x, z, x, w);
        
        /// <summary>
        /// Returns hVector4.xzy swizzling.
        /// </summary>
        public hVector3 xzy => new hVector3(x, z, y);
        
        /// <summary>
        /// Returns hVector4.rbg swizzling (equivalent to hVector4.xzy).
        /// </summary>
        public hVector3 rbg => new hVector3(x, z, y);
        
        /// <summary>
        /// Returns hVector4.xzyx swizzling.
        /// </summary>
        public hVector4 xzyx => new hVector4(x, z, y, x);
        
        /// <summary>
        /// Returns hVector4.rbgr swizzling (equivalent to hVector4.xzyx).
        /// </summary>
        public hVector4 rbgr => new hVector4(x, z, y, x);
        
        /// <summary>
        /// Returns hVector4.xzyy swizzling.
        /// </summary>
        public hVector4 xzyy => new hVector4(x, z, y, y);
        
        /// <summary>
        /// Returns hVector4.rbgg swizzling (equivalent to hVector4.xzyy).
        /// </summary>
        public hVector4 rbgg => new hVector4(x, z, y, y);
        
        /// <summary>
        /// Returns hVector4.xzyz swizzling.
        /// </summary>
        public hVector4 xzyz => new hVector4(x, z, y, z);
        
        /// <summary>
        /// Returns hVector4.rbgb swizzling (equivalent to hVector4.xzyz).
        /// </summary>
        public hVector4 rbgb => new hVector4(x, z, y, z);
        
        /// <summary>
        /// Returns hVector4.xzyw swizzling.
        /// </summary>
        public hVector4 xzyw => new hVector4(x, z, y, w);
        
        /// <summary>
        /// Returns hVector4.rbga swizzling (equivalent to hVector4.xzyw).
        /// </summary>
        public hVector4 rbga => new hVector4(x, z, y, w);
        
        /// <summary>
        /// Returns hVector4.xzz swizzling.
        /// </summary>
        public hVector3 xzz => new hVector3(x, z, z);
        
        /// <summary>
        /// Returns hVector4.rbb swizzling (equivalent to hVector4.xzz).
        /// </summary>
        public hVector3 rbb => new hVector3(x, z, z);
        
        /// <summary>
        /// Returns hVector4.xzzx swizzling.
        /// </summary>
        public hVector4 xzzx => new hVector4(x, z, z, x);
        
        /// <summary>
        /// Returns hVector4.rbbr swizzling (equivalent to hVector4.xzzx).
        /// </summary>
        public hVector4 rbbr => new hVector4(x, z, z, x);
        
        /// <summary>
        /// Returns hVector4.xzzy swizzling.
        /// </summary>
        public hVector4 xzzy => new hVector4(x, z, z, y);
        
        /// <summary>
        /// Returns hVector4.rbbg swizzling (equivalent to hVector4.xzzy).
        /// </summary>
        public hVector4 rbbg => new hVector4(x, z, z, y);
        
        /// <summary>
        /// Returns hVector4.xzzz swizzling.
        /// </summary>
        public hVector4 xzzz => new hVector4(x, z, z, z);
        
        /// <summary>
        /// Returns hVector4.rbbb swizzling (equivalent to hVector4.xzzz).
        /// </summary>
        public hVector4 rbbb => new hVector4(x, z, z, z);
        
        /// <summary>
        /// Returns hVector4.xzzw swizzling.
        /// </summary>
        public hVector4 xzzw => new hVector4(x, z, z, w);
        
        /// <summary>
        /// Returns hVector4.rbba swizzling (equivalent to hVector4.xzzw).
        /// </summary>
        public hVector4 rbba => new hVector4(x, z, z, w);
        
        /// <summary>
        /// Returns hVector4.xzw swizzling.
        /// </summary>
        public hVector3 xzw => new hVector3(x, z, w);
        
        /// <summary>
        /// Returns hVector4.rba swizzling (equivalent to hVector4.xzw).
        /// </summary>
        public hVector3 rba => new hVector3(x, z, w);
        
        /// <summary>
        /// Returns hVector4.xzwx swizzling.
        /// </summary>
        public hVector4 xzwx => new hVector4(x, z, w, x);
        
        /// <summary>
        /// Returns hVector4.rbar swizzling (equivalent to hVector4.xzwx).
        /// </summary>
        public hVector4 rbar => new hVector4(x, z, w, x);
        
        /// <summary>
        /// Returns hVector4.xzwy swizzling.
        /// </summary>
        public hVector4 xzwy => new hVector4(x, z, w, y);
        
        /// <summary>
        /// Returns hVector4.rbag swizzling (equivalent to hVector4.xzwy).
        /// </summary>
        public hVector4 rbag => new hVector4(x, z, w, y);
        
        /// <summary>
        /// Returns hVector4.xzwz swizzling.
        /// </summary>
        public hVector4 xzwz => new hVector4(x, z, w, z);
        
        /// <summary>
        /// Returns hVector4.rbab swizzling (equivalent to hVector4.xzwz).
        /// </summary>
        public hVector4 rbab => new hVector4(x, z, w, z);
        
        /// <summary>
        /// Returns hVector4.xzww swizzling.
        /// </summary>
        public hVector4 xzww => new hVector4(x, z, w, w);
        
        /// <summary>
        /// Returns hVector4.rbaa swizzling (equivalent to hVector4.xzww).
        /// </summary>
        public hVector4 rbaa => new hVector4(x, z, w, w);
        
        /// <summary>
        /// Returns hVector4.xw swizzling.
        /// </summary>
        public hVector2 xw => new hVector2(x, w);
        
        /// <summary>
        /// Returns hVector4.ra swizzling (equivalent to hVector4.xw).
        /// </summary>
        public hVector2 ra => new hVector2(x, w);
        
        /// <summary>
        /// Returns hVector4.xwx swizzling.
        /// </summary>
        public hVector3 xwx => new hVector3(x, w, x);
        
        /// <summary>
        /// Returns hVector4.rar swizzling (equivalent to hVector4.xwx).
        /// </summary>
        public hVector3 rar => new hVector3(x, w, x);
        
        /// <summary>
        /// Returns hVector4.xwxx swizzling.
        /// </summary>
        public hVector4 xwxx => new hVector4(x, w, x, x);
        
        /// <summary>
        /// Returns hVector4.rarr swizzling (equivalent to hVector4.xwxx).
        /// </summary>
        public hVector4 rarr => new hVector4(x, w, x, x);
        
        /// <summary>
        /// Returns hVector4.xwxy swizzling.
        /// </summary>
        public hVector4 xwxy => new hVector4(x, w, x, y);
        
        /// <summary>
        /// Returns hVector4.rarg swizzling (equivalent to hVector4.xwxy).
        /// </summary>
        public hVector4 rarg => new hVector4(x, w, x, y);
        
        /// <summary>
        /// Returns hVector4.xwxz swizzling.
        /// </summary>
        public hVector4 xwxz => new hVector4(x, w, x, z);
        
        /// <summary>
        /// Returns hVector4.rarb swizzling (equivalent to hVector4.xwxz).
        /// </summary>
        public hVector4 rarb => new hVector4(x, w, x, z);
        
        /// <summary>
        /// Returns hVector4.xwxw swizzling.
        /// </summary>
        public hVector4 xwxw => new hVector4(x, w, x, w);
        
        /// <summary>
        /// Returns hVector4.rara swizzling (equivalent to hVector4.xwxw).
        /// </summary>
        public hVector4 rara => new hVector4(x, w, x, w);
        
        /// <summary>
        /// Returns hVector4.xwy swizzling.
        /// </summary>
        public hVector3 xwy => new hVector3(x, w, y);
        
        /// <summary>
        /// Returns hVector4.rag swizzling (equivalent to hVector4.xwy).
        /// </summary>
        public hVector3 rag => new hVector3(x, w, y);
        
        /// <summary>
        /// Returns hVector4.xwyx swizzling.
        /// </summary>
        public hVector4 xwyx => new hVector4(x, w, y, x);
        
        /// <summary>
        /// Returns hVector4.ragr swizzling (equivalent to hVector4.xwyx).
        /// </summary>
        public hVector4 ragr => new hVector4(x, w, y, x);
        
        /// <summary>
        /// Returns hVector4.xwyy swizzling.
        /// </summary>
        public hVector4 xwyy => new hVector4(x, w, y, y);
        
        /// <summary>
        /// Returns hVector4.ragg swizzling (equivalent to hVector4.xwyy).
        /// </summary>
        public hVector4 ragg => new hVector4(x, w, y, y);
        
        /// <summary>
        /// Returns hVector4.xwyz swizzling.
        /// </summary>
        public hVector4 xwyz => new hVector4(x, w, y, z);
        
        /// <summary>
        /// Returns hVector4.ragb swizzling (equivalent to hVector4.xwyz).
        /// </summary>
        public hVector4 ragb => new hVector4(x, w, y, z);
        
        /// <summary>
        /// Returns hVector4.xwyw swizzling.
        /// </summary>
        public hVector4 xwyw => new hVector4(x, w, y, w);
        
        /// <summary>
        /// Returns hVector4.raga swizzling (equivalent to hVector4.xwyw).
        /// </summary>
        public hVector4 raga => new hVector4(x, w, y, w);
        
        /// <summary>
        /// Returns hVector4.xwz swizzling.
        /// </summary>
        public hVector3 xwz => new hVector3(x, w, z);
        
        /// <summary>
        /// Returns hVector4.rab swizzling (equivalent to hVector4.xwz).
        /// </summary>
        public hVector3 rab => new hVector3(x, w, z);
        
        /// <summary>
        /// Returns hVector4.xwzx swizzling.
        /// </summary>
        public hVector4 xwzx => new hVector4(x, w, z, x);
        
        /// <summary>
        /// Returns hVector4.rabr swizzling (equivalent to hVector4.xwzx).
        /// </summary>
        public hVector4 rabr => new hVector4(x, w, z, x);
        
        /// <summary>
        /// Returns hVector4.xwzy swizzling.
        /// </summary>
        public hVector4 xwzy => new hVector4(x, w, z, y);
        
        /// <summary>
        /// Returns hVector4.rabg swizzling (equivalent to hVector4.xwzy).
        /// </summary>
        public hVector4 rabg => new hVector4(x, w, z, y);
        
        /// <summary>
        /// Returns hVector4.xwzz swizzling.
        /// </summary>
        public hVector4 xwzz => new hVector4(x, w, z, z);
        
        /// <summary>
        /// Returns hVector4.rabb swizzling (equivalent to hVector4.xwzz).
        /// </summary>
        public hVector4 rabb => new hVector4(x, w, z, z);
        
        /// <summary>
        /// Returns hVector4.xwzw swizzling.
        /// </summary>
        public hVector4 xwzw => new hVector4(x, w, z, w);
        
        /// <summary>
        /// Returns hVector4.raba swizzling (equivalent to hVector4.xwzw).
        /// </summary>
        public hVector4 raba => new hVector4(x, w, z, w);
        
        /// <summary>
        /// Returns hVector4.xww swizzling.
        /// </summary>
        public hVector3 xww => new hVector3(x, w, w);
        
        /// <summary>
        /// Returns hVector4.raa swizzling (equivalent to hVector4.xww).
        /// </summary>
        public hVector3 raa => new hVector3(x, w, w);
        
        /// <summary>
        /// Returns hVector4.xwwx swizzling.
        /// </summary>
        public hVector4 xwwx => new hVector4(x, w, w, x);
        
        /// <summary>
        /// Returns hVector4.raar swizzling (equivalent to hVector4.xwwx).
        /// </summary>
        public hVector4 raar => new hVector4(x, w, w, x);
        
        /// <summary>
        /// Returns hVector4.xwwy swizzling.
        /// </summary>
        public hVector4 xwwy => new hVector4(x, w, w, y);
        
        /// <summary>
        /// Returns hVector4.raag swizzling (equivalent to hVector4.xwwy).
        /// </summary>
        public hVector4 raag => new hVector4(x, w, w, y);
        
        /// <summary>
        /// Returns hVector4.xwwz swizzling.
        /// </summary>
        public hVector4 xwwz => new hVector4(x, w, w, z);
        
        /// <summary>
        /// Returns hVector4.raab swizzling (equivalent to hVector4.xwwz).
        /// </summary>
        public hVector4 raab => new hVector4(x, w, w, z);
        
        /// <summary>
        /// Returns hVector4.xwww swizzling.
        /// </summary>
        public hVector4 xwww => new hVector4(x, w, w, w);
        
        /// <summary>
        /// Returns hVector4.raaa swizzling (equivalent to hVector4.xwww).
        /// </summary>
        public hVector4 raaa => new hVector4(x, w, w, w);
        
        /// <summary>
        /// Returns hVector4.yx swizzling.
        /// </summary>
        public hVector2 yx => new hVector2(y, x);
        
        /// <summary>
        /// Returns hVector4.gr swizzling (equivalent to hVector4.yx).
        /// </summary>
        public hVector2 gr => new hVector2(y, x);
        
        /// <summary>
        /// Returns hVector4.yxx swizzling.
        /// </summary>
        public hVector3 yxx => new hVector3(y, x, x);
        
        /// <summary>
        /// Returns hVector4.grr swizzling (equivalent to hVector4.yxx).
        /// </summary>
        public hVector3 grr => new hVector3(y, x, x);
        
        /// <summary>
        /// Returns hVector4.yxxx swizzling.
        /// </summary>
        public hVector4 yxxx => new hVector4(y, x, x, x);
        
        /// <summary>
        /// Returns hVector4.grrr swizzling (equivalent to hVector4.yxxx).
        /// </summary>
        public hVector4 grrr => new hVector4(y, x, x, x);
        
        /// <summary>
        /// Returns hVector4.yxxy swizzling.
        /// </summary>
        public hVector4 yxxy => new hVector4(y, x, x, y);
        
        /// <summary>
        /// Returns hVector4.grrg swizzling (equivalent to hVector4.yxxy).
        /// </summary>
        public hVector4 grrg => new hVector4(y, x, x, y);
        
        /// <summary>
        /// Returns hVector4.yxxz swizzling.
        /// </summary>
        public hVector4 yxxz => new hVector4(y, x, x, z);
        
        /// <summary>
        /// Returns hVector4.grrb swizzling (equivalent to hVector4.yxxz).
        /// </summary>
        public hVector4 grrb => new hVector4(y, x, x, z);
        
        /// <summary>
        /// Returns hVector4.yxxw swizzling.
        /// </summary>
        public hVector4 yxxw => new hVector4(y, x, x, w);
        
        /// <summary>
        /// Returns hVector4.grra swizzling (equivalent to hVector4.yxxw).
        /// </summary>
        public hVector4 grra => new hVector4(y, x, x, w);
        
        /// <summary>
        /// Returns hVector4.yxy swizzling.
        /// </summary>
        public hVector3 yxy => new hVector3(y, x, y);
        
        /// <summary>
        /// Returns hVector4.grg swizzling (equivalent to hVector4.yxy).
        /// </summary>
        public hVector3 grg => new hVector3(y, x, y);
        
        /// <summary>
        /// Returns hVector4.yxyx swizzling.
        /// </summary>
        public hVector4 yxyx => new hVector4(y, x, y, x);
        
        /// <summary>
        /// Returns hVector4.grgr swizzling (equivalent to hVector4.yxyx).
        /// </summary>
        public hVector4 grgr => new hVector4(y, x, y, x);
        
        /// <summary>
        /// Returns hVector4.yxyy swizzling.
        /// </summary>
        public hVector4 yxyy => new hVector4(y, x, y, y);
        
        /// <summary>
        /// Returns hVector4.grgg swizzling (equivalent to hVector4.yxyy).
        /// </summary>
        public hVector4 grgg => new hVector4(y, x, y, y);
        
        /// <summary>
        /// Returns hVector4.yxyz swizzling.
        /// </summary>
        public hVector4 yxyz => new hVector4(y, x, y, z);
        
        /// <summary>
        /// Returns hVector4.grgb swizzling (equivalent to hVector4.yxyz).
        /// </summary>
        public hVector4 grgb => new hVector4(y, x, y, z);
        
        /// <summary>
        /// Returns hVector4.yxyw swizzling.
        /// </summary>
        public hVector4 yxyw => new hVector4(y, x, y, w);
        
        /// <summary>
        /// Returns hVector4.grga swizzling (equivalent to hVector4.yxyw).
        /// </summary>
        public hVector4 grga => new hVector4(y, x, y, w);
        
        /// <summary>
        /// Returns hVector4.yxz swizzling.
        /// </summary>
        public hVector3 yxz => new hVector3(y, x, z);
        
        /// <summary>
        /// Returns hVector4.grb swizzling (equivalent to hVector4.yxz).
        /// </summary>
        public hVector3 grb => new hVector3(y, x, z);
        
        /// <summary>
        /// Returns hVector4.yxzx swizzling.
        /// </summary>
        public hVector4 yxzx => new hVector4(y, x, z, x);
        
        /// <summary>
        /// Returns hVector4.grbr swizzling (equivalent to hVector4.yxzx).
        /// </summary>
        public hVector4 grbr => new hVector4(y, x, z, x);
        
        /// <summary>
        /// Returns hVector4.yxzy swizzling.
        /// </summary>
        public hVector4 yxzy => new hVector4(y, x, z, y);
        
        /// <summary>
        /// Returns hVector4.grbg swizzling (equivalent to hVector4.yxzy).
        /// </summary>
        public hVector4 grbg => new hVector4(y, x, z, y);
        
        /// <summary>
        /// Returns hVector4.yxzz swizzling.
        /// </summary>
        public hVector4 yxzz => new hVector4(y, x, z, z);
        
        /// <summary>
        /// Returns hVector4.grbb swizzling (equivalent to hVector4.yxzz).
        /// </summary>
        public hVector4 grbb => new hVector4(y, x, z, z);
        
        /// <summary>
        /// Returns hVector4.yxzw swizzling.
        /// </summary>
        public hVector4 yxzw => new hVector4(y, x, z, w);
        
        /// <summary>
        /// Returns hVector4.grba swizzling (equivalent to hVector4.yxzw).
        /// </summary>
        public hVector4 grba => new hVector4(y, x, z, w);
        
        /// <summary>
        /// Returns hVector4.yxw swizzling.
        /// </summary>
        public hVector3 yxw => new hVector3(y, x, w);
        
        /// <summary>
        /// Returns hVector4.gra swizzling (equivalent to hVector4.yxw).
        /// </summary>
        public hVector3 gra => new hVector3(y, x, w);
        
        /// <summary>
        /// Returns hVector4.yxwx swizzling.
        /// </summary>
        public hVector4 yxwx => new hVector4(y, x, w, x);
        
        /// <summary>
        /// Returns hVector4.grar swizzling (equivalent to hVector4.yxwx).
        /// </summary>
        public hVector4 grar => new hVector4(y, x, w, x);
        
        /// <summary>
        /// Returns hVector4.yxwy swizzling.
        /// </summary>
        public hVector4 yxwy => new hVector4(y, x, w, y);
        
        /// <summary>
        /// Returns hVector4.grag swizzling (equivalent to hVector4.yxwy).
        /// </summary>
        public hVector4 grag => new hVector4(y, x, w, y);
        
        /// <summary>
        /// Returns hVector4.yxwz swizzling.
        /// </summary>
        public hVector4 yxwz => new hVector4(y, x, w, z);
        
        /// <summary>
        /// Returns hVector4.grab swizzling (equivalent to hVector4.yxwz).
        /// </summary>
        public hVector4 grab => new hVector4(y, x, w, z);
        
        /// <summary>
        /// Returns hVector4.yxww swizzling.
        /// </summary>
        public hVector4 yxww => new hVector4(y, x, w, w);
        
        /// <summary>
        /// Returns hVector4.graa swizzling (equivalent to hVector4.yxww).
        /// </summary>
        public hVector4 graa => new hVector4(y, x, w, w);
        
        /// <summary>
        /// Returns hVector4.yy swizzling.
        /// </summary>
        public hVector2 yy => new hVector2(y, y);
        
        /// <summary>
        /// Returns hVector4.gg swizzling (equivalent to hVector4.yy).
        /// </summary>
        public hVector2 gg => new hVector2(y, y);
        
        /// <summary>
        /// Returns hVector4.yyx swizzling.
        /// </summary>
        public hVector3 yyx => new hVector3(y, y, x);
        
        /// <summary>
        /// Returns hVector4.ggr swizzling (equivalent to hVector4.yyx).
        /// </summary>
        public hVector3 ggr => new hVector3(y, y, x);
        
        /// <summary>
        /// Returns hVector4.yyxx swizzling.
        /// </summary>
        public hVector4 yyxx => new hVector4(y, y, x, x);
        
        /// <summary>
        /// Returns hVector4.ggrr swizzling (equivalent to hVector4.yyxx).
        /// </summary>
        public hVector4 ggrr => new hVector4(y, y, x, x);
        
        /// <summary>
        /// Returns hVector4.yyxy swizzling.
        /// </summary>
        public hVector4 yyxy => new hVector4(y, y, x, y);
        
        /// <summary>
        /// Returns hVector4.ggrg swizzling (equivalent to hVector4.yyxy).
        /// </summary>
        public hVector4 ggrg => new hVector4(y, y, x, y);
        
        /// <summary>
        /// Returns hVector4.yyxz swizzling.
        /// </summary>
        public hVector4 yyxz => new hVector4(y, y, x, z);
        
        /// <summary>
        /// Returns hVector4.ggrb swizzling (equivalent to hVector4.yyxz).
        /// </summary>
        public hVector4 ggrb => new hVector4(y, y, x, z);
        
        /// <summary>
        /// Returns hVector4.yyxw swizzling.
        /// </summary>
        public hVector4 yyxw => new hVector4(y, y, x, w);
        
        /// <summary>
        /// Returns hVector4.ggra swizzling (equivalent to hVector4.yyxw).
        /// </summary>
        public hVector4 ggra => new hVector4(y, y, x, w);
        
        /// <summary>
        /// Returns hVector4.yyy swizzling.
        /// </summary>
        public hVector3 yyy => new hVector3(y, y, y);
        
        /// <summary>
        /// Returns hVector4.ggg swizzling (equivalent to hVector4.yyy).
        /// </summary>
        public hVector3 ggg => new hVector3(y, y, y);
        
        /// <summary>
        /// Returns hVector4.yyyx swizzling.
        /// </summary>
        public hVector4 yyyx => new hVector4(y, y, y, x);
        
        /// <summary>
        /// Returns hVector4.gggr swizzling (equivalent to hVector4.yyyx).
        /// </summary>
        public hVector4 gggr => new hVector4(y, y, y, x);
        
        /// <summary>
        /// Returns hVector4.yyyy swizzling.
        /// </summary>
        public hVector4 yyyy => new hVector4(y, y, y, y);
        
        /// <summary>
        /// Returns hVector4.gggg swizzling (equivalent to hVector4.yyyy).
        /// </summary>
        public hVector4 gggg => new hVector4(y, y, y, y);
        
        /// <summary>
        /// Returns hVector4.yyyz swizzling.
        /// </summary>
        public hVector4 yyyz => new hVector4(y, y, y, z);
        
        /// <summary>
        /// Returns hVector4.gggb swizzling (equivalent to hVector4.yyyz).
        /// </summary>
        public hVector4 gggb => new hVector4(y, y, y, z);
        
        /// <summary>
        /// Returns hVector4.yyyw swizzling.
        /// </summary>
        public hVector4 yyyw => new hVector4(y, y, y, w);
        
        /// <summary>
        /// Returns hVector4.ggga swizzling (equivalent to hVector4.yyyw).
        /// </summary>
        public hVector4 ggga => new hVector4(y, y, y, w);
        
        /// <summary>
        /// Returns hVector4.yyz swizzling.
        /// </summary>
        public hVector3 yyz => new hVector3(y, y, z);
        
        /// <summary>
        /// Returns hVector4.ggb swizzling (equivalent to hVector4.yyz).
        /// </summary>
        public hVector3 ggb => new hVector3(y, y, z);
        
        /// <summary>
        /// Returns hVector4.yyzx swizzling.
        /// </summary>
        public hVector4 yyzx => new hVector4(y, y, z, x);
        
        /// <summary>
        /// Returns hVector4.ggbr swizzling (equivalent to hVector4.yyzx).
        /// </summary>
        public hVector4 ggbr => new hVector4(y, y, z, x);
        
        /// <summary>
        /// Returns hVector4.yyzy swizzling.
        /// </summary>
        public hVector4 yyzy => new hVector4(y, y, z, y);
        
        /// <summary>
        /// Returns hVector4.ggbg swizzling (equivalent to hVector4.yyzy).
        /// </summary>
        public hVector4 ggbg => new hVector4(y, y, z, y);
        
        /// <summary>
        /// Returns hVector4.yyzz swizzling.
        /// </summary>
        public hVector4 yyzz => new hVector4(y, y, z, z);
        
        /// <summary>
        /// Returns hVector4.ggbb swizzling (equivalent to hVector4.yyzz).
        /// </summary>
        public hVector4 ggbb => new hVector4(y, y, z, z);
        
        /// <summary>
        /// Returns hVector4.yyzw swizzling.
        /// </summary>
        public hVector4 yyzw => new hVector4(y, y, z, w);
        
        /// <summary>
        /// Returns hVector4.ggba swizzling (equivalent to hVector4.yyzw).
        /// </summary>
        public hVector4 ggba => new hVector4(y, y, z, w);
        
        /// <summary>
        /// Returns hVector4.yyw swizzling.
        /// </summary>
        public hVector3 yyw => new hVector3(y, y, w);
        
        /// <summary>
        /// Returns hVector4.gga swizzling (equivalent to hVector4.yyw).
        /// </summary>
        public hVector3 gga => new hVector3(y, y, w);
        
        /// <summary>
        /// Returns hVector4.yywx swizzling.
        /// </summary>
        public hVector4 yywx => new hVector4(y, y, w, x);
        
        /// <summary>
        /// Returns hVector4.ggar swizzling (equivalent to hVector4.yywx).
        /// </summary>
        public hVector4 ggar => new hVector4(y, y, w, x);
        
        /// <summary>
        /// Returns hVector4.yywy swizzling.
        /// </summary>
        public hVector4 yywy => new hVector4(y, y, w, y);
        
        /// <summary>
        /// Returns hVector4.ggag swizzling (equivalent to hVector4.yywy).
        /// </summary>
        public hVector4 ggag => new hVector4(y, y, w, y);
        
        /// <summary>
        /// Returns hVector4.yywz swizzling.
        /// </summary>
        public hVector4 yywz => new hVector4(y, y, w, z);
        
        /// <summary>
        /// Returns hVector4.ggab swizzling (equivalent to hVector4.yywz).
        /// </summary>
        public hVector4 ggab => new hVector4(y, y, w, z);
        
        /// <summary>
        /// Returns hVector4.yyww swizzling.
        /// </summary>
        public hVector4 yyww => new hVector4(y, y, w, w);
        
        /// <summary>
        /// Returns hVector4.ggaa swizzling (equivalent to hVector4.yyww).
        /// </summary>
        public hVector4 ggaa => new hVector4(y, y, w, w);
        
        /// <summary>
        /// Returns hVector4.yz swizzling.
        /// </summary>
        public hVector2 yz => new hVector2(y, z);
        
        /// <summary>
        /// Returns hVector4.gb swizzling (equivalent to hVector4.yz).
        /// </summary>
        public hVector2 gb => new hVector2(y, z);
        
        /// <summary>
        /// Returns hVector4.yzx swizzling.
        /// </summary>
        public hVector3 yzx => new hVector3(y, z, x);
        
        /// <summary>
        /// Returns hVector4.gbr swizzling (equivalent to hVector4.yzx).
        /// </summary>
        public hVector3 gbr => new hVector3(y, z, x);
        
        /// <summary>
        /// Returns hVector4.yzxx swizzling.
        /// </summary>
        public hVector4 yzxx => new hVector4(y, z, x, x);
        
        /// <summary>
        /// Returns hVector4.gbrr swizzling (equivalent to hVector4.yzxx).
        /// </summary>
        public hVector4 gbrr => new hVector4(y, z, x, x);
        
        /// <summary>
        /// Returns hVector4.yzxy swizzling.
        /// </summary>
        public hVector4 yzxy => new hVector4(y, z, x, y);
        
        /// <summary>
        /// Returns hVector4.gbrg swizzling (equivalent to hVector4.yzxy).
        /// </summary>
        public hVector4 gbrg => new hVector4(y, z, x, y);
        
        /// <summary>
        /// Returns hVector4.yzxz swizzling.
        /// </summary>
        public hVector4 yzxz => new hVector4(y, z, x, z);
        
        /// <summary>
        /// Returns hVector4.gbrb swizzling (equivalent to hVector4.yzxz).
        /// </summary>
        public hVector4 gbrb => new hVector4(y, z, x, z);
        
        /// <summary>
        /// Returns hVector4.yzxw swizzling.
        /// </summary>
        public hVector4 yzxw => new hVector4(y, z, x, w);
        
        /// <summary>
        /// Returns hVector4.gbra swizzling (equivalent to hVector4.yzxw).
        /// </summary>
        public hVector4 gbra => new hVector4(y, z, x, w);
        
        /// <summary>
        /// Returns hVector4.yzy swizzling.
        /// </summary>
        public hVector3 yzy => new hVector3(y, z, y);
        
        /// <summary>
        /// Returns hVector4.gbg swizzling (equivalent to hVector4.yzy).
        /// </summary>
        public hVector3 gbg => new hVector3(y, z, y);
        
        /// <summary>
        /// Returns hVector4.yzyx swizzling.
        /// </summary>
        public hVector4 yzyx => new hVector4(y, z, y, x);
        
        /// <summary>
        /// Returns hVector4.gbgr swizzling (equivalent to hVector4.yzyx).
        /// </summary>
        public hVector4 gbgr => new hVector4(y, z, y, x);
        
        /// <summary>
        /// Returns hVector4.yzyy swizzling.
        /// </summary>
        public hVector4 yzyy => new hVector4(y, z, y, y);
        
        /// <summary>
        /// Returns hVector4.gbgg swizzling (equivalent to hVector4.yzyy).
        /// </summary>
        public hVector4 gbgg => new hVector4(y, z, y, y);
        
        /// <summary>
        /// Returns hVector4.yzyz swizzling.
        /// </summary>
        public hVector4 yzyz => new hVector4(y, z, y, z);
        
        /// <summary>
        /// Returns hVector4.gbgb swizzling (equivalent to hVector4.yzyz).
        /// </summary>
        public hVector4 gbgb => new hVector4(y, z, y, z);
        
        /// <summary>
        /// Returns hVector4.yzyw swizzling.
        /// </summary>
        public hVector4 yzyw => new hVector4(y, z, y, w);
        
        /// <summary>
        /// Returns hVector4.gbga swizzling (equivalent to hVector4.yzyw).
        /// </summary>
        public hVector4 gbga => new hVector4(y, z, y, w);
        
        /// <summary>
        /// Returns hVector4.yzz swizzling.
        /// </summary>
        public hVector3 yzz => new hVector3(y, z, z);
        
        /// <summary>
        /// Returns hVector4.gbb swizzling (equivalent to hVector4.yzz).
        /// </summary>
        public hVector3 gbb => new hVector3(y, z, z);
        
        /// <summary>
        /// Returns hVector4.yzzx swizzling.
        /// </summary>
        public hVector4 yzzx => new hVector4(y, z, z, x);
        
        /// <summary>
        /// Returns hVector4.gbbr swizzling (equivalent to hVector4.yzzx).
        /// </summary>
        public hVector4 gbbr => new hVector4(y, z, z, x);
        
        /// <summary>
        /// Returns hVector4.yzzy swizzling.
        /// </summary>
        public hVector4 yzzy => new hVector4(y, z, z, y);
        
        /// <summary>
        /// Returns hVector4.gbbg swizzling (equivalent to hVector4.yzzy).
        /// </summary>
        public hVector4 gbbg => new hVector4(y, z, z, y);
        
        /// <summary>
        /// Returns hVector4.yzzz swizzling.
        /// </summary>
        public hVector4 yzzz => new hVector4(y, z, z, z);
        
        /// <summary>
        /// Returns hVector4.gbbb swizzling (equivalent to hVector4.yzzz).
        /// </summary>
        public hVector4 gbbb => new hVector4(y, z, z, z);
        
        /// <summary>
        /// Returns hVector4.yzzw swizzling.
        /// </summary>
        public hVector4 yzzw => new hVector4(y, z, z, w);
        
        /// <summary>
        /// Returns hVector4.gbba swizzling (equivalent to hVector4.yzzw).
        /// </summary>
        public hVector4 gbba => new hVector4(y, z, z, w);
        
        /// <summary>
        /// Returns hVector4.yzw swizzling.
        /// </summary>
        public hVector3 yzw => new hVector3(y, z, w);
        
        /// <summary>
        /// Returns hVector4.gba swizzling (equivalent to hVector4.yzw).
        /// </summary>
        public hVector3 gba => new hVector3(y, z, w);
        
        /// <summary>
        /// Returns hVector4.yzwx swizzling.
        /// </summary>
        public hVector4 yzwx => new hVector4(y, z, w, x);
        
        /// <summary>
        /// Returns hVector4.gbar swizzling (equivalent to hVector4.yzwx).
        /// </summary>
        public hVector4 gbar => new hVector4(y, z, w, x);
        
        /// <summary>
        /// Returns hVector4.yzwy swizzling.
        /// </summary>
        public hVector4 yzwy => new hVector4(y, z, w, y);
        
        /// <summary>
        /// Returns hVector4.gbag swizzling (equivalent to hVector4.yzwy).
        /// </summary>
        public hVector4 gbag => new hVector4(y, z, w, y);
        
        /// <summary>
        /// Returns hVector4.yzwz swizzling.
        /// </summary>
        public hVector4 yzwz => new hVector4(y, z, w, z);
        
        /// <summary>
        /// Returns hVector4.gbab swizzling (equivalent to hVector4.yzwz).
        /// </summary>
        public hVector4 gbab => new hVector4(y, z, w, z);
        
        /// <summary>
        /// Returns hVector4.yzww swizzling.
        /// </summary>
        public hVector4 yzww => new hVector4(y, z, w, w);
        
        /// <summary>
        /// Returns hVector4.gbaa swizzling (equivalent to hVector4.yzww).
        /// </summary>
        public hVector4 gbaa => new hVector4(y, z, w, w);
        
        /// <summary>
        /// Returns hVector4.yw swizzling.
        /// </summary>
        public hVector2 yw => new hVector2(y, w);
        
        /// <summary>
        /// Returns hVector4.ga swizzling (equivalent to hVector4.yw).
        /// </summary>
        public hVector2 ga => new hVector2(y, w);
        
        /// <summary>
        /// Returns hVector4.ywx swizzling.
        /// </summary>
        public hVector3 ywx => new hVector3(y, w, x);
        
        /// <summary>
        /// Returns hVector4.gar swizzling (equivalent to hVector4.ywx).
        /// </summary>
        public hVector3 gar => new hVector3(y, w, x);
        
        /// <summary>
        /// Returns hVector4.ywxx swizzling.
        /// </summary>
        public hVector4 ywxx => new hVector4(y, w, x, x);
        
        /// <summary>
        /// Returns hVector4.garr swizzling (equivalent to hVector4.ywxx).
        /// </summary>
        public hVector4 garr => new hVector4(y, w, x, x);
        
        /// <summary>
        /// Returns hVector4.ywxy swizzling.
        /// </summary>
        public hVector4 ywxy => new hVector4(y, w, x, y);
        
        /// <summary>
        /// Returns hVector4.garg swizzling (equivalent to hVector4.ywxy).
        /// </summary>
        public hVector4 garg => new hVector4(y, w, x, y);
        
        /// <summary>
        /// Returns hVector4.ywxz swizzling.
        /// </summary>
        public hVector4 ywxz => new hVector4(y, w, x, z);
        
        /// <summary>
        /// Returns hVector4.garb swizzling (equivalent to hVector4.ywxz).
        /// </summary>
        public hVector4 garb => new hVector4(y, w, x, z);
        
        /// <summary>
        /// Returns hVector4.ywxw swizzling.
        /// </summary>
        public hVector4 ywxw => new hVector4(y, w, x, w);
        
        /// <summary>
        /// Returns hVector4.gara swizzling (equivalent to hVector4.ywxw).
        /// </summary>
        public hVector4 gara => new hVector4(y, w, x, w);
        
        /// <summary>
        /// Returns hVector4.ywy swizzling.
        /// </summary>
        public hVector3 ywy => new hVector3(y, w, y);
        
        /// <summary>
        /// Returns hVector4.gag swizzling (equivalent to hVector4.ywy).
        /// </summary>
        public hVector3 gag => new hVector3(y, w, y);
        
        /// <summary>
        /// Returns hVector4.ywyx swizzling.
        /// </summary>
        public hVector4 ywyx => new hVector4(y, w, y, x);
        
        /// <summary>
        /// Returns hVector4.gagr swizzling (equivalent to hVector4.ywyx).
        /// </summary>
        public hVector4 gagr => new hVector4(y, w, y, x);
        
        /// <summary>
        /// Returns hVector4.ywyy swizzling.
        /// </summary>
        public hVector4 ywyy => new hVector4(y, w, y, y);
        
        /// <summary>
        /// Returns hVector4.gagg swizzling (equivalent to hVector4.ywyy).
        /// </summary>
        public hVector4 gagg => new hVector4(y, w, y, y);
        
        /// <summary>
        /// Returns hVector4.ywyz swizzling.
        /// </summary>
        public hVector4 ywyz => new hVector4(y, w, y, z);
        
        /// <summary>
        /// Returns hVector4.gagb swizzling (equivalent to hVector4.ywyz).
        /// </summary>
        public hVector4 gagb => new hVector4(y, w, y, z);
        
        /// <summary>
        /// Returns hVector4.ywyw swizzling.
        /// </summary>
        public hVector4 ywyw => new hVector4(y, w, y, w);
        
        /// <summary>
        /// Returns hVector4.gaga swizzling (equivalent to hVector4.ywyw).
        /// </summary>
        public hVector4 gaga => new hVector4(y, w, y, w);
        
        /// <summary>
        /// Returns hVector4.ywz swizzling.
        /// </summary>
        public hVector3 ywz => new hVector3(y, w, z);
        
        /// <summary>
        /// Returns hVector4.gab swizzling (equivalent to hVector4.ywz).
        /// </summary>
        public hVector3 gab => new hVector3(y, w, z);
        
        /// <summary>
        /// Returns hVector4.ywzx swizzling.
        /// </summary>
        public hVector4 ywzx => new hVector4(y, w, z, x);
        
        /// <summary>
        /// Returns hVector4.gabr swizzling (equivalent to hVector4.ywzx).
        /// </summary>
        public hVector4 gabr => new hVector4(y, w, z, x);
        
        /// <summary>
        /// Returns hVector4.ywzy swizzling.
        /// </summary>
        public hVector4 ywzy => new hVector4(y, w, z, y);
        
        /// <summary>
        /// Returns hVector4.gabg swizzling (equivalent to hVector4.ywzy).
        /// </summary>
        public hVector4 gabg => new hVector4(y, w, z, y);
        
        /// <summary>
        /// Returns hVector4.ywzz swizzling.
        /// </summary>
        public hVector4 ywzz => new hVector4(y, w, z, z);
        
        /// <summary>
        /// Returns hVector4.gabb swizzling (equivalent to hVector4.ywzz).
        /// </summary>
        public hVector4 gabb => new hVector4(y, w, z, z);
        
        /// <summary>
        /// Returns hVector4.ywzw swizzling.
        /// </summary>
        public hVector4 ywzw => new hVector4(y, w, z, w);
        
        /// <summary>
        /// Returns hVector4.gaba swizzling (equivalent to hVector4.ywzw).
        /// </summary>
        public hVector4 gaba => new hVector4(y, w, z, w);
        
        /// <summary>
        /// Returns hVector4.yww swizzling.
        /// </summary>
        public hVector3 yww => new hVector3(y, w, w);
        
        /// <summary>
        /// Returns hVector4.gaa swizzling (equivalent to hVector4.yww).
        /// </summary>
        public hVector3 gaa => new hVector3(y, w, w);
        
        /// <summary>
        /// Returns hVector4.ywwx swizzling.
        /// </summary>
        public hVector4 ywwx => new hVector4(y, w, w, x);
        
        /// <summary>
        /// Returns hVector4.gaar swizzling (equivalent to hVector4.ywwx).
        /// </summary>
        public hVector4 gaar => new hVector4(y, w, w, x);
        
        /// <summary>
        /// Returns hVector4.ywwy swizzling.
        /// </summary>
        public hVector4 ywwy => new hVector4(y, w, w, y);
        
        /// <summary>
        /// Returns hVector4.gaag swizzling (equivalent to hVector4.ywwy).
        /// </summary>
        public hVector4 gaag => new hVector4(y, w, w, y);
        
        /// <summary>
        /// Returns hVector4.ywwz swizzling.
        /// </summary>
        public hVector4 ywwz => new hVector4(y, w, w, z);
        
        /// <summary>
        /// Returns hVector4.gaab swizzling (equivalent to hVector4.ywwz).
        /// </summary>
        public hVector4 gaab => new hVector4(y, w, w, z);
        
        /// <summary>
        /// Returns hVector4.ywww swizzling.
        /// </summary>
        public hVector4 ywww => new hVector4(y, w, w, w);
        
        /// <summary>
        /// Returns hVector4.gaaa swizzling (equivalent to hVector4.ywww).
        /// </summary>
        public hVector4 gaaa => new hVector4(y, w, w, w);
        
        /// <summary>
        /// Returns hVector4.zx swizzling.
        /// </summary>
        public hVector2 zx => new hVector2(z, x);
        
        /// <summary>
        /// Returns hVector4.br swizzling (equivalent to hVector4.zx).
        /// </summary>
        public hVector2 br => new hVector2(z, x);
        
        /// <summary>
        /// Returns hVector4.zxx swizzling.
        /// </summary>
        public hVector3 zxx => new hVector3(z, x, x);
        
        /// <summary>
        /// Returns hVector4.brr swizzling (equivalent to hVector4.zxx).
        /// </summary>
        public hVector3 brr => new hVector3(z, x, x);
        
        /// <summary>
        /// Returns hVector4.zxxx swizzling.
        /// </summary>
        public hVector4 zxxx => new hVector4(z, x, x, x);
        
        /// <summary>
        /// Returns hVector4.brrr swizzling (equivalent to hVector4.zxxx).
        /// </summary>
        public hVector4 brrr => new hVector4(z, x, x, x);
        
        /// <summary>
        /// Returns hVector4.zxxy swizzling.
        /// </summary>
        public hVector4 zxxy => new hVector4(z, x, x, y);
        
        /// <summary>
        /// Returns hVector4.brrg swizzling (equivalent to hVector4.zxxy).
        /// </summary>
        public hVector4 brrg => new hVector4(z, x, x, y);
        
        /// <summary>
        /// Returns hVector4.zxxz swizzling.
        /// </summary>
        public hVector4 zxxz => new hVector4(z, x, x, z);
        
        /// <summary>
        /// Returns hVector4.brrb swizzling (equivalent to hVector4.zxxz).
        /// </summary>
        public hVector4 brrb => new hVector4(z, x, x, z);
        
        /// <summary>
        /// Returns hVector4.zxxw swizzling.
        /// </summary>
        public hVector4 zxxw => new hVector4(z, x, x, w);
        
        /// <summary>
        /// Returns hVector4.brra swizzling (equivalent to hVector4.zxxw).
        /// </summary>
        public hVector4 brra => new hVector4(z, x, x, w);
        
        /// <summary>
        /// Returns hVector4.zxy swizzling.
        /// </summary>
        public hVector3 zxy => new hVector3(z, x, y);
        
        /// <summary>
        /// Returns hVector4.brg swizzling (equivalent to hVector4.zxy).
        /// </summary>
        public hVector3 brg => new hVector3(z, x, y);
        
        /// <summary>
        /// Returns hVector4.zxyx swizzling.
        /// </summary>
        public hVector4 zxyx => new hVector4(z, x, y, x);
        
        /// <summary>
        /// Returns hVector4.brgr swizzling (equivalent to hVector4.zxyx).
        /// </summary>
        public hVector4 brgr => new hVector4(z, x, y, x);
        
        /// <summary>
        /// Returns hVector4.zxyy swizzling.
        /// </summary>
        public hVector4 zxyy => new hVector4(z, x, y, y);
        
        /// <summary>
        /// Returns hVector4.brgg swizzling (equivalent to hVector4.zxyy).
        /// </summary>
        public hVector4 brgg => new hVector4(z, x, y, y);
        
        /// <summary>
        /// Returns hVector4.zxyz swizzling.
        /// </summary>
        public hVector4 zxyz => new hVector4(z, x, y, z);
        
        /// <summary>
        /// Returns hVector4.brgb swizzling (equivalent to hVector4.zxyz).
        /// </summary>
        public hVector4 brgb => new hVector4(z, x, y, z);
        
        /// <summary>
        /// Returns hVector4.zxyw swizzling.
        /// </summary>
        public hVector4 zxyw => new hVector4(z, x, y, w);
        
        /// <summary>
        /// Returns hVector4.brga swizzling (equivalent to hVector4.zxyw).
        /// </summary>
        public hVector4 brga => new hVector4(z, x, y, w);
        
        /// <summary>
        /// Returns hVector4.zxz swizzling.
        /// </summary>
        public hVector3 zxz => new hVector3(z, x, z);
        
        /// <summary>
        /// Returns hVector4.brb swizzling (equivalent to hVector4.zxz).
        /// </summary>
        public hVector3 brb => new hVector3(z, x, z);
        
        /// <summary>
        /// Returns hVector4.zxzx swizzling.
        /// </summary>
        public hVector4 zxzx => new hVector4(z, x, z, x);
        
        /// <summary>
        /// Returns hVector4.brbr swizzling (equivalent to hVector4.zxzx).
        /// </summary>
        public hVector4 brbr => new hVector4(z, x, z, x);
        
        /// <summary>
        /// Returns hVector4.zxzy swizzling.
        /// </summary>
        public hVector4 zxzy => new hVector4(z, x, z, y);
        
        /// <summary>
        /// Returns hVector4.brbg swizzling (equivalent to hVector4.zxzy).
        /// </summary>
        public hVector4 brbg => new hVector4(z, x, z, y);
        
        /// <summary>
        /// Returns hVector4.zxzz swizzling.
        /// </summary>
        public hVector4 zxzz => new hVector4(z, x, z, z);
        
        /// <summary>
        /// Returns hVector4.brbb swizzling (equivalent to hVector4.zxzz).
        /// </summary>
        public hVector4 brbb => new hVector4(z, x, z, z);
        
        /// <summary>
        /// Returns hVector4.zxzw swizzling.
        /// </summary>
        public hVector4 zxzw => new hVector4(z, x, z, w);
        
        /// <summary>
        /// Returns hVector4.brba swizzling (equivalent to hVector4.zxzw).
        /// </summary>
        public hVector4 brba => new hVector4(z, x, z, w);
        
        /// <summary>
        /// Returns hVector4.zxw swizzling.
        /// </summary>
        public hVector3 zxw => new hVector3(z, x, w);
        
        /// <summary>
        /// Returns hVector4.bra swizzling (equivalent to hVector4.zxw).
        /// </summary>
        public hVector3 bra => new hVector3(z, x, w);
        
        /// <summary>
        /// Returns hVector4.zxwx swizzling.
        /// </summary>
        public hVector4 zxwx => new hVector4(z, x, w, x);
        
        /// <summary>
        /// Returns hVector4.brar swizzling (equivalent to hVector4.zxwx).
        /// </summary>
        public hVector4 brar => new hVector4(z, x, w, x);
        
        /// <summary>
        /// Returns hVector4.zxwy swizzling.
        /// </summary>
        public hVector4 zxwy => new hVector4(z, x, w, y);
        
        /// <summary>
        /// Returns hVector4.brag swizzling (equivalent to hVector4.zxwy).
        /// </summary>
        public hVector4 brag => new hVector4(z, x, w, y);
        
        /// <summary>
        /// Returns hVector4.zxwz swizzling.
        /// </summary>
        public hVector4 zxwz => new hVector4(z, x, w, z);
        
        /// <summary>
        /// Returns hVector4.brab swizzling (equivalent to hVector4.zxwz).
        /// </summary>
        public hVector4 brab => new hVector4(z, x, w, z);
        
        /// <summary>
        /// Returns hVector4.zxww swizzling.
        /// </summary>
        public hVector4 zxww => new hVector4(z, x, w, w);
        
        /// <summary>
        /// Returns hVector4.braa swizzling (equivalent to hVector4.zxww).
        /// </summary>
        public hVector4 braa => new hVector4(z, x, w, w);
        
        /// <summary>
        /// Returns hVector4.zy swizzling.
        /// </summary>
        public hVector2 zy => new hVector2(z, y);
        
        /// <summary>
        /// Returns hVector4.bg swizzling (equivalent to hVector4.zy).
        /// </summary>
        public hVector2 bg => new hVector2(z, y);
        
        /// <summary>
        /// Returns hVector4.zyx swizzling.
        /// </summary>
        public hVector3 zyx => new hVector3(z, y, x);
        
        /// <summary>
        /// Returns hVector4.bgr swizzling (equivalent to hVector4.zyx).
        /// </summary>
        public hVector3 bgr => new hVector3(z, y, x);
        
        /// <summary>
        /// Returns hVector4.zyxx swizzling.
        /// </summary>
        public hVector4 zyxx => new hVector4(z, y, x, x);
        
        /// <summary>
        /// Returns hVector4.bgrr swizzling (equivalent to hVector4.zyxx).
        /// </summary>
        public hVector4 bgrr => new hVector4(z, y, x, x);
        
        /// <summary>
        /// Returns hVector4.zyxy swizzling.
        /// </summary>
        public hVector4 zyxy => new hVector4(z, y, x, y);
        
        /// <summary>
        /// Returns hVector4.bgrg swizzling (equivalent to hVector4.zyxy).
        /// </summary>
        public hVector4 bgrg => new hVector4(z, y, x, y);
        
        /// <summary>
        /// Returns hVector4.zyxz swizzling.
        /// </summary>
        public hVector4 zyxz => new hVector4(z, y, x, z);
        
        /// <summary>
        /// Returns hVector4.bgrb swizzling (equivalent to hVector4.zyxz).
        /// </summary>
        public hVector4 bgrb => new hVector4(z, y, x, z);
        
        /// <summary>
        /// Returns hVector4.zyxw swizzling.
        /// </summary>
        public hVector4 zyxw => new hVector4(z, y, x, w);
        
        /// <summary>
        /// Returns hVector4.bgra swizzling (equivalent to hVector4.zyxw).
        /// </summary>
        public hVector4 bgra => new hVector4(z, y, x, w);
        
        /// <summary>
        /// Returns hVector4.zyy swizzling.
        /// </summary>
        public hVector3 zyy => new hVector3(z, y, y);
        
        /// <summary>
        /// Returns hVector4.bgg swizzling (equivalent to hVector4.zyy).
        /// </summary>
        public hVector3 bgg => new hVector3(z, y, y);
        
        /// <summary>
        /// Returns hVector4.zyyx swizzling.
        /// </summary>
        public hVector4 zyyx => new hVector4(z, y, y, x);
        
        /// <summary>
        /// Returns hVector4.bggr swizzling (equivalent to hVector4.zyyx).
        /// </summary>
        public hVector4 bggr => new hVector4(z, y, y, x);
        
        /// <summary>
        /// Returns hVector4.zyyy swizzling.
        /// </summary>
        public hVector4 zyyy => new hVector4(z, y, y, y);
        
        /// <summary>
        /// Returns hVector4.bggg swizzling (equivalent to hVector4.zyyy).
        /// </summary>
        public hVector4 bggg => new hVector4(z, y, y, y);
        
        /// <summary>
        /// Returns hVector4.zyyz swizzling.
        /// </summary>
        public hVector4 zyyz => new hVector4(z, y, y, z);
        
        /// <summary>
        /// Returns hVector4.bggb swizzling (equivalent to hVector4.zyyz).
        /// </summary>
        public hVector4 bggb => new hVector4(z, y, y, z);
        
        /// <summary>
        /// Returns hVector4.zyyw swizzling.
        /// </summary>
        public hVector4 zyyw => new hVector4(z, y, y, w);
        
        /// <summary>
        /// Returns hVector4.bgga swizzling (equivalent to hVector4.zyyw).
        /// </summary>
        public hVector4 bgga => new hVector4(z, y, y, w);
        
        /// <summary>
        /// Returns hVector4.zyz swizzling.
        /// </summary>
        public hVector3 zyz => new hVector3(z, y, z);
        
        /// <summary>
        /// Returns hVector4.bgb swizzling (equivalent to hVector4.zyz).
        /// </summary>
        public hVector3 bgb => new hVector3(z, y, z);
        
        /// <summary>
        /// Returns hVector4.zyzx swizzling.
        /// </summary>
        public hVector4 zyzx => new hVector4(z, y, z, x);
        
        /// <summary>
        /// Returns hVector4.bgbr swizzling (equivalent to hVector4.zyzx).
        /// </summary>
        public hVector4 bgbr => new hVector4(z, y, z, x);
        
        /// <summary>
        /// Returns hVector4.zyzy swizzling.
        /// </summary>
        public hVector4 zyzy => new hVector4(z, y, z, y);
        
        /// <summary>
        /// Returns hVector4.bgbg swizzling (equivalent to hVector4.zyzy).
        /// </summary>
        public hVector4 bgbg => new hVector4(z, y, z, y);
        
        /// <summary>
        /// Returns hVector4.zyzz swizzling.
        /// </summary>
        public hVector4 zyzz => new hVector4(z, y, z, z);
        
        /// <summary>
        /// Returns hVector4.bgbb swizzling (equivalent to hVector4.zyzz).
        /// </summary>
        public hVector4 bgbb => new hVector4(z, y, z, z);
        
        /// <summary>
        /// Returns hVector4.zyzw swizzling.
        /// </summary>
        public hVector4 zyzw => new hVector4(z, y, z, w);
        
        /// <summary>
        /// Returns hVector4.bgba swizzling (equivalent to hVector4.zyzw).
        /// </summary>
        public hVector4 bgba => new hVector4(z, y, z, w);
        
        /// <summary>
        /// Returns hVector4.zyw swizzling.
        /// </summary>
        public hVector3 zyw => new hVector3(z, y, w);
        
        /// <summary>
        /// Returns hVector4.bga swizzling (equivalent to hVector4.zyw).
        /// </summary>
        public hVector3 bga => new hVector3(z, y, w);
        
        /// <summary>
        /// Returns hVector4.zywx swizzling.
        /// </summary>
        public hVector4 zywx => new hVector4(z, y, w, x);
        
        /// <summary>
        /// Returns hVector4.bgar swizzling (equivalent to hVector4.zywx).
        /// </summary>
        public hVector4 bgar => new hVector4(z, y, w, x);
        
        /// <summary>
        /// Returns hVector4.zywy swizzling.
        /// </summary>
        public hVector4 zywy => new hVector4(z, y, w, y);
        
        /// <summary>
        /// Returns hVector4.bgag swizzling (equivalent to hVector4.zywy).
        /// </summary>
        public hVector4 bgag => new hVector4(z, y, w, y);
        
        /// <summary>
        /// Returns hVector4.zywz swizzling.
        /// </summary>
        public hVector4 zywz => new hVector4(z, y, w, z);
        
        /// <summary>
        /// Returns hVector4.bgab swizzling (equivalent to hVector4.zywz).
        /// </summary>
        public hVector4 bgab => new hVector4(z, y, w, z);
        
        /// <summary>
        /// Returns hVector4.zyww swizzling.
        /// </summary>
        public hVector4 zyww => new hVector4(z, y, w, w);
        
        /// <summary>
        /// Returns hVector4.bgaa swizzling (equivalent to hVector4.zyww).
        /// </summary>
        public hVector4 bgaa => new hVector4(z, y, w, w);
        
        /// <summary>
        /// Returns hVector4.zz swizzling.
        /// </summary>
        public hVector2 zz => new hVector2(z, z);
        
        /// <summary>
        /// Returns hVector4.bb swizzling (equivalent to hVector4.zz).
        /// </summary>
        public hVector2 bb => new hVector2(z, z);
        
        /// <summary>
        /// Returns hVector4.zzx swizzling.
        /// </summary>
        public hVector3 zzx => new hVector3(z, z, x);
        
        /// <summary>
        /// Returns hVector4.bbr swizzling (equivalent to hVector4.zzx).
        /// </summary>
        public hVector3 bbr => new hVector3(z, z, x);
        
        /// <summary>
        /// Returns hVector4.zzxx swizzling.
        /// </summary>
        public hVector4 zzxx => new hVector4(z, z, x, x);
        
        /// <summary>
        /// Returns hVector4.bbrr swizzling (equivalent to hVector4.zzxx).
        /// </summary>
        public hVector4 bbrr => new hVector4(z, z, x, x);
        
        /// <summary>
        /// Returns hVector4.zzxy swizzling.
        /// </summary>
        public hVector4 zzxy => new hVector4(z, z, x, y);
        
        /// <summary>
        /// Returns hVector4.bbrg swizzling (equivalent to hVector4.zzxy).
        /// </summary>
        public hVector4 bbrg => new hVector4(z, z, x, y);
        
        /// <summary>
        /// Returns hVector4.zzxz swizzling.
        /// </summary>
        public hVector4 zzxz => new hVector4(z, z, x, z);
        
        /// <summary>
        /// Returns hVector4.bbrb swizzling (equivalent to hVector4.zzxz).
        /// </summary>
        public hVector4 bbrb => new hVector4(z, z, x, z);
        
        /// <summary>
        /// Returns hVector4.zzxw swizzling.
        /// </summary>
        public hVector4 zzxw => new hVector4(z, z, x, w);
        
        /// <summary>
        /// Returns hVector4.bbra swizzling (equivalent to hVector4.zzxw).
        /// </summary>
        public hVector4 bbra => new hVector4(z, z, x, w);
        
        /// <summary>
        /// Returns hVector4.zzy swizzling.
        /// </summary>
        public hVector3 zzy => new hVector3(z, z, y);
        
        /// <summary>
        /// Returns hVector4.bbg swizzling (equivalent to hVector4.zzy).
        /// </summary>
        public hVector3 bbg => new hVector3(z, z, y);
        
        /// <summary>
        /// Returns hVector4.zzyx swizzling.
        /// </summary>
        public hVector4 zzyx => new hVector4(z, z, y, x);
        
        /// <summary>
        /// Returns hVector4.bbgr swizzling (equivalent to hVector4.zzyx).
        /// </summary>
        public hVector4 bbgr => new hVector4(z, z, y, x);
        
        /// <summary>
        /// Returns hVector4.zzyy swizzling.
        /// </summary>
        public hVector4 zzyy => new hVector4(z, z, y, y);
        
        /// <summary>
        /// Returns hVector4.bbgg swizzling (equivalent to hVector4.zzyy).
        /// </summary>
        public hVector4 bbgg => new hVector4(z, z, y, y);
        
        /// <summary>
        /// Returns hVector4.zzyz swizzling.
        /// </summary>
        public hVector4 zzyz => new hVector4(z, z, y, z);
        
        /// <summary>
        /// Returns hVector4.bbgb swizzling (equivalent to hVector4.zzyz).
        /// </summary>
        public hVector4 bbgb => new hVector4(z, z, y, z);
        
        /// <summary>
        /// Returns hVector4.zzyw swizzling.
        /// </summary>
        public hVector4 zzyw => new hVector4(z, z, y, w);
        
        /// <summary>
        /// Returns hVector4.bbga swizzling (equivalent to hVector4.zzyw).
        /// </summary>
        public hVector4 bbga => new hVector4(z, z, y, w);
        
        /// <summary>
        /// Returns hVector4.zzz swizzling.
        /// </summary>
        public hVector3 zzz => new hVector3(z, z, z);
        
        /// <summary>
        /// Returns hVector4.bbb swizzling (equivalent to hVector4.zzz).
        /// </summary>
        public hVector3 bbb => new hVector3(z, z, z);
        
        /// <summary>
        /// Returns hVector4.zzzx swizzling.
        /// </summary>
        public hVector4 zzzx => new hVector4(z, z, z, x);
        
        /// <summary>
        /// Returns hVector4.bbbr swizzling (equivalent to hVector4.zzzx).
        /// </summary>
        public hVector4 bbbr => new hVector4(z, z, z, x);
        
        /// <summary>
        /// Returns hVector4.zzzy swizzling.
        /// </summary>
        public hVector4 zzzy => new hVector4(z, z, z, y);
        
        /// <summary>
        /// Returns hVector4.bbbg swizzling (equivalent to hVector4.zzzy).
        /// </summary>
        public hVector4 bbbg => new hVector4(z, z, z, y);
        
        /// <summary>
        /// Returns hVector4.zzzz swizzling.
        /// </summary>
        public hVector4 zzzz => new hVector4(z, z, z, z);
        
        /// <summary>
        /// Returns hVector4.bbbb swizzling (equivalent to hVector4.zzzz).
        /// </summary>
        public hVector4 bbbb => new hVector4(z, z, z, z);
        
        /// <summary>
        /// Returns hVector4.zzzw swizzling.
        /// </summary>
        public hVector4 zzzw => new hVector4(z, z, z, w);
        
        /// <summary>
        /// Returns hVector4.bbba swizzling (equivalent to hVector4.zzzw).
        /// </summary>
        public hVector4 bbba => new hVector4(z, z, z, w);
        
        /// <summary>
        /// Returns hVector4.zzw swizzling.
        /// </summary>
        public hVector3 zzw => new hVector3(z, z, w);
        
        /// <summary>
        /// Returns hVector4.bba swizzling (equivalent to hVector4.zzw).
        /// </summary>
        public hVector3 bba => new hVector3(z, z, w);
        
        /// <summary>
        /// Returns hVector4.zzwx swizzling.
        /// </summary>
        public hVector4 zzwx => new hVector4(z, z, w, x);
        
        /// <summary>
        /// Returns hVector4.bbar swizzling (equivalent to hVector4.zzwx).
        /// </summary>
        public hVector4 bbar => new hVector4(z, z, w, x);
        
        /// <summary>
        /// Returns hVector4.zzwy swizzling.
        /// </summary>
        public hVector4 zzwy => new hVector4(z, z, w, y);
        
        /// <summary>
        /// Returns hVector4.bbag swizzling (equivalent to hVector4.zzwy).
        /// </summary>
        public hVector4 bbag => new hVector4(z, z, w, y);
        
        /// <summary>
        /// Returns hVector4.zzwz swizzling.
        /// </summary>
        public hVector4 zzwz => new hVector4(z, z, w, z);
        
        /// <summary>
        /// Returns hVector4.bbab swizzling (equivalent to hVector4.zzwz).
        /// </summary>
        public hVector4 bbab => new hVector4(z, z, w, z);
        
        /// <summary>
        /// Returns hVector4.zzww swizzling.
        /// </summary>
        public hVector4 zzww => new hVector4(z, z, w, w);
        
        /// <summary>
        /// Returns hVector4.bbaa swizzling (equivalent to hVector4.zzww).
        /// </summary>
        public hVector4 bbaa => new hVector4(z, z, w, w);
        
        /// <summary>
        /// Returns hVector4.zw swizzling.
        /// </summary>
        public hVector2 zw => new hVector2(z, w);
        
        /// <summary>
        /// Returns hVector4.ba swizzling (equivalent to hVector4.zw).
        /// </summary>
        public hVector2 ba => new hVector2(z, w);
        
        /// <summary>
        /// Returns hVector4.zwx swizzling.
        /// </summary>
        public hVector3 zwx => new hVector3(z, w, x);
        
        /// <summary>
        /// Returns hVector4.bar swizzling (equivalent to hVector4.zwx).
        /// </summary>
        public hVector3 bar => new hVector3(z, w, x);
        
        /// <summary>
        /// Returns hVector4.zwxx swizzling.
        /// </summary>
        public hVector4 zwxx => new hVector4(z, w, x, x);
        
        /// <summary>
        /// Returns hVector4.barr swizzling (equivalent to hVector4.zwxx).
        /// </summary>
        public hVector4 barr => new hVector4(z, w, x, x);
        
        /// <summary>
        /// Returns hVector4.zwxy swizzling.
        /// </summary>
        public hVector4 zwxy => new hVector4(z, w, x, y);
        
        /// <summary>
        /// Returns hVector4.barg swizzling (equivalent to hVector4.zwxy).
        /// </summary>
        public hVector4 barg => new hVector4(z, w, x, y);
        
        /// <summary>
        /// Returns hVector4.zwxz swizzling.
        /// </summary>
        public hVector4 zwxz => new hVector4(z, w, x, z);
        
        /// <summary>
        /// Returns hVector4.barb swizzling (equivalent to hVector4.zwxz).
        /// </summary>
        public hVector4 barb => new hVector4(z, w, x, z);
        
        /// <summary>
        /// Returns hVector4.zwxw swizzling.
        /// </summary>
        public hVector4 zwxw => new hVector4(z, w, x, w);
        
        /// <summary>
        /// Returns hVector4.bara swizzling (equivalent to hVector4.zwxw).
        /// </summary>
        public hVector4 bara => new hVector4(z, w, x, w);
        
        /// <summary>
        /// Returns hVector4.zwy swizzling.
        /// </summary>
        public hVector3 zwy => new hVector3(z, w, y);
        
        /// <summary>
        /// Returns hVector4.bag swizzling (equivalent to hVector4.zwy).
        /// </summary>
        public hVector3 bag => new hVector3(z, w, y);
        
        /// <summary>
        /// Returns hVector4.zwyx swizzling.
        /// </summary>
        public hVector4 zwyx => new hVector4(z, w, y, x);
        
        /// <summary>
        /// Returns hVector4.bagr swizzling (equivalent to hVector4.zwyx).
        /// </summary>
        public hVector4 bagr => new hVector4(z, w, y, x);
        
        /// <summary>
        /// Returns hVector4.zwyy swizzling.
        /// </summary>
        public hVector4 zwyy => new hVector4(z, w, y, y);
        
        /// <summary>
        /// Returns hVector4.bagg swizzling (equivalent to hVector4.zwyy).
        /// </summary>
        public hVector4 bagg => new hVector4(z, w, y, y);
        
        /// <summary>
        /// Returns hVector4.zwyz swizzling.
        /// </summary>
        public hVector4 zwyz => new hVector4(z, w, y, z);
        
        /// <summary>
        /// Returns hVector4.bagb swizzling (equivalent to hVector4.zwyz).
        /// </summary>
        public hVector4 bagb => new hVector4(z, w, y, z);
        
        /// <summary>
        /// Returns hVector4.zwyw swizzling.
        /// </summary>
        public hVector4 zwyw => new hVector4(z, w, y, w);
        
        /// <summary>
        /// Returns hVector4.baga swizzling (equivalent to hVector4.zwyw).
        /// </summary>
        public hVector4 baga => new hVector4(z, w, y, w);
        
        /// <summary>
        /// Returns hVector4.zwz swizzling.
        /// </summary>
        public hVector3 zwz => new hVector3(z, w, z);
        
        /// <summary>
        /// Returns hVector4.bab swizzling (equivalent to hVector4.zwz).
        /// </summary>
        public hVector3 bab => new hVector3(z, w, z);
        
        /// <summary>
        /// Returns hVector4.zwzx swizzling.
        /// </summary>
        public hVector4 zwzx => new hVector4(z, w, z, x);
        
        /// <summary>
        /// Returns hVector4.babr swizzling (equivalent to hVector4.zwzx).
        /// </summary>
        public hVector4 babr => new hVector4(z, w, z, x);
        
        /// <summary>
        /// Returns hVector4.zwzy swizzling.
        /// </summary>
        public hVector4 zwzy => new hVector4(z, w, z, y);
        
        /// <summary>
        /// Returns hVector4.babg swizzling (equivalent to hVector4.zwzy).
        /// </summary>
        public hVector4 babg => new hVector4(z, w, z, y);
        
        /// <summary>
        /// Returns hVector4.zwzz swizzling.
        /// </summary>
        public hVector4 zwzz => new hVector4(z, w, z, z);
        
        /// <summary>
        /// Returns hVector4.babb swizzling (equivalent to hVector4.zwzz).
        /// </summary>
        public hVector4 babb => new hVector4(z, w, z, z);
        
        /// <summary>
        /// Returns hVector4.zwzw swizzling.
        /// </summary>
        public hVector4 zwzw => new hVector4(z, w, z, w);
        
        /// <summary>
        /// Returns hVector4.baba swizzling (equivalent to hVector4.zwzw).
        /// </summary>
        public hVector4 baba => new hVector4(z, w, z, w);
        
        /// <summary>
        /// Returns hVector4.zww swizzling.
        /// </summary>
        public hVector3 zww => new hVector3(z, w, w);
        
        /// <summary>
        /// Returns hVector4.baa swizzling (equivalent to hVector4.zww).
        /// </summary>
        public hVector3 baa => new hVector3(z, w, w);
        
        /// <summary>
        /// Returns hVector4.zwwx swizzling.
        /// </summary>
        public hVector4 zwwx => new hVector4(z, w, w, x);
        
        /// <summary>
        /// Returns hVector4.baar swizzling (equivalent to hVector4.zwwx).
        /// </summary>
        public hVector4 baar => new hVector4(z, w, w, x);
        
        /// <summary>
        /// Returns hVector4.zwwy swizzling.
        /// </summary>
        public hVector4 zwwy => new hVector4(z, w, w, y);
        
        /// <summary>
        /// Returns hVector4.baag swizzling (equivalent to hVector4.zwwy).
        /// </summary>
        public hVector4 baag => new hVector4(z, w, w, y);
        
        /// <summary>
        /// Returns hVector4.zwwz swizzling.
        /// </summary>
        public hVector4 zwwz => new hVector4(z, w, w, z);
        
        /// <summary>
        /// Returns hVector4.baab swizzling (equivalent to hVector4.zwwz).
        /// </summary>
        public hVector4 baab => new hVector4(z, w, w, z);
        
        /// <summary>
        /// Returns hVector4.zwww swizzling.
        /// </summary>
        public hVector4 zwww => new hVector4(z, w, w, w);
        
        /// <summary>
        /// Returns hVector4.baaa swizzling (equivalent to hVector4.zwww).
        /// </summary>
        public hVector4 baaa => new hVector4(z, w, w, w);
        
        /// <summary>
        /// Returns hVector4.wx swizzling.
        /// </summary>
        public hVector2 wx => new hVector2(w, x);
        
        /// <summary>
        /// Returns hVector4.ar swizzling (equivalent to hVector4.wx).
        /// </summary>
        public hVector2 ar => new hVector2(w, x);
        
        /// <summary>
        /// Returns hVector4.wxx swizzling.
        /// </summary>
        public hVector3 wxx => new hVector3(w, x, x);
        
        /// <summary>
        /// Returns hVector4.arr swizzling (equivalent to hVector4.wxx).
        /// </summary>
        public hVector3 arr => new hVector3(w, x, x);
        
        /// <summary>
        /// Returns hVector4.wxxx swizzling.
        /// </summary>
        public hVector4 wxxx => new hVector4(w, x, x, x);
        
        /// <summary>
        /// Returns hVector4.arrr swizzling (equivalent to hVector4.wxxx).
        /// </summary>
        public hVector4 arrr => new hVector4(w, x, x, x);
        
        /// <summary>
        /// Returns hVector4.wxxy swizzling.
        /// </summary>
        public hVector4 wxxy => new hVector4(w, x, x, y);
        
        /// <summary>
        /// Returns hVector4.arrg swizzling (equivalent to hVector4.wxxy).
        /// </summary>
        public hVector4 arrg => new hVector4(w, x, x, y);
        
        /// <summary>
        /// Returns hVector4.wxxz swizzling.
        /// </summary>
        public hVector4 wxxz => new hVector4(w, x, x, z);
        
        /// <summary>
        /// Returns hVector4.arrb swizzling (equivalent to hVector4.wxxz).
        /// </summary>
        public hVector4 arrb => new hVector4(w, x, x, z);
        
        /// <summary>
        /// Returns hVector4.wxxw swizzling.
        /// </summary>
        public hVector4 wxxw => new hVector4(w, x, x, w);
        
        /// <summary>
        /// Returns hVector4.arra swizzling (equivalent to hVector4.wxxw).
        /// </summary>
        public hVector4 arra => new hVector4(w, x, x, w);
        
        /// <summary>
        /// Returns hVector4.wxy swizzling.
        /// </summary>
        public hVector3 wxy => new hVector3(w, x, y);
        
        /// <summary>
        /// Returns hVector4.arg swizzling (equivalent to hVector4.wxy).
        /// </summary>
        public hVector3 arg => new hVector3(w, x, y);
        
        /// <summary>
        /// Returns hVector4.wxyx swizzling.
        /// </summary>
        public hVector4 wxyx => new hVector4(w, x, y, x);
        
        /// <summary>
        /// Returns hVector4.argr swizzling (equivalent to hVector4.wxyx).
        /// </summary>
        public hVector4 argr => new hVector4(w, x, y, x);
        
        /// <summary>
        /// Returns hVector4.wxyy swizzling.
        /// </summary>
        public hVector4 wxyy => new hVector4(w, x, y, y);
        
        /// <summary>
        /// Returns hVector4.argg swizzling (equivalent to hVector4.wxyy).
        /// </summary>
        public hVector4 argg => new hVector4(w, x, y, y);
        
        /// <summary>
        /// Returns hVector4.wxyz swizzling.
        /// </summary>
        public hVector4 wxyz => new hVector4(w, x, y, z);
        
        /// <summary>
        /// Returns hVector4.argb swizzling (equivalent to hVector4.wxyz).
        /// </summary>
        public hVector4 argb => new hVector4(w, x, y, z);
        
        /// <summary>
        /// Returns hVector4.wxyw swizzling.
        /// </summary>
        public hVector4 wxyw => new hVector4(w, x, y, w);
        
        /// <summary>
        /// Returns hVector4.arga swizzling (equivalent to hVector4.wxyw).
        /// </summary>
        public hVector4 arga => new hVector4(w, x, y, w);
        
        /// <summary>
        /// Returns hVector4.wxz swizzling.
        /// </summary>
        public hVector3 wxz => new hVector3(w, x, z);
        
        /// <summary>
        /// Returns hVector4.arb swizzling (equivalent to hVector4.wxz).
        /// </summary>
        public hVector3 arb => new hVector3(w, x, z);
        
        /// <summary>
        /// Returns hVector4.wxzx swizzling.
        /// </summary>
        public hVector4 wxzx => new hVector4(w, x, z, x);
        
        /// <summary>
        /// Returns hVector4.arbr swizzling (equivalent to hVector4.wxzx).
        /// </summary>
        public hVector4 arbr => new hVector4(w, x, z, x);
        
        /// <summary>
        /// Returns hVector4.wxzy swizzling.
        /// </summary>
        public hVector4 wxzy => new hVector4(w, x, z, y);
        
        /// <summary>
        /// Returns hVector4.arbg swizzling (equivalent to hVector4.wxzy).
        /// </summary>
        public hVector4 arbg => new hVector4(w, x, z, y);
        
        /// <summary>
        /// Returns hVector4.wxzz swizzling.
        /// </summary>
        public hVector4 wxzz => new hVector4(w, x, z, z);
        
        /// <summary>
        /// Returns hVector4.arbb swizzling (equivalent to hVector4.wxzz).
        /// </summary>
        public hVector4 arbb => new hVector4(w, x, z, z);
        
        /// <summary>
        /// Returns hVector4.wxzw swizzling.
        /// </summary>
        public hVector4 wxzw => new hVector4(w, x, z, w);
        
        /// <summary>
        /// Returns hVector4.arba swizzling (equivalent to hVector4.wxzw).
        /// </summary>
        public hVector4 arba => new hVector4(w, x, z, w);
        
        /// <summary>
        /// Returns hVector4.wxw swizzling.
        /// </summary>
        public hVector3 wxw => new hVector3(w, x, w);
        
        /// <summary>
        /// Returns hVector4.ara swizzling (equivalent to hVector4.wxw).
        /// </summary>
        public hVector3 ara => new hVector3(w, x, w);
        
        /// <summary>
        /// Returns hVector4.wxwx swizzling.
        /// </summary>
        public hVector4 wxwx => new hVector4(w, x, w, x);
        
        /// <summary>
        /// Returns hVector4.arar swizzling (equivalent to hVector4.wxwx).
        /// </summary>
        public hVector4 arar => new hVector4(w, x, w, x);
        
        /// <summary>
        /// Returns hVector4.wxwy swizzling.
        /// </summary>
        public hVector4 wxwy => new hVector4(w, x, w, y);
        
        /// <summary>
        /// Returns hVector4.arag swizzling (equivalent to hVector4.wxwy).
        /// </summary>
        public hVector4 arag => new hVector4(w, x, w, y);
        
        /// <summary>
        /// Returns hVector4.wxwz swizzling.
        /// </summary>
        public hVector4 wxwz => new hVector4(w, x, w, z);
        
        /// <summary>
        /// Returns hVector4.arab swizzling (equivalent to hVector4.wxwz).
        /// </summary>
        public hVector4 arab => new hVector4(w, x, w, z);
        
        /// <summary>
        /// Returns hVector4.wxww swizzling.
        /// </summary>
        public hVector4 wxww => new hVector4(w, x, w, w);
        
        /// <summary>
        /// Returns hVector4.araa swizzling (equivalent to hVector4.wxww).
        /// </summary>
        public hVector4 araa => new hVector4(w, x, w, w);
        
        /// <summary>
        /// Returns hVector4.wy swizzling.
        /// </summary>
        public hVector2 wy => new hVector2(w, y);
        
        /// <summary>
        /// Returns hVector4.ag swizzling (equivalent to hVector4.wy).
        /// </summary>
        public hVector2 ag => new hVector2(w, y);
        
        /// <summary>
        /// Returns hVector4.wyx swizzling.
        /// </summary>
        public hVector3 wyx => new hVector3(w, y, x);
        
        /// <summary>
        /// Returns hVector4.agr swizzling (equivalent to hVector4.wyx).
        /// </summary>
        public hVector3 agr => new hVector3(w, y, x);
        
        /// <summary>
        /// Returns hVector4.wyxx swizzling.
        /// </summary>
        public hVector4 wyxx => new hVector4(w, y, x, x);
        
        /// <summary>
        /// Returns hVector4.agrr swizzling (equivalent to hVector4.wyxx).
        /// </summary>
        public hVector4 agrr => new hVector4(w, y, x, x);
        
        /// <summary>
        /// Returns hVector4.wyxy swizzling.
        /// </summary>
        public hVector4 wyxy => new hVector4(w, y, x, y);
        
        /// <summary>
        /// Returns hVector4.agrg swizzling (equivalent to hVector4.wyxy).
        /// </summary>
        public hVector4 agrg => new hVector4(w, y, x, y);
        
        /// <summary>
        /// Returns hVector4.wyxz swizzling.
        /// </summary>
        public hVector4 wyxz => new hVector4(w, y, x, z);
        
        /// <summary>
        /// Returns hVector4.agrb swizzling (equivalent to hVector4.wyxz).
        /// </summary>
        public hVector4 agrb => new hVector4(w, y, x, z);
        
        /// <summary>
        /// Returns hVector4.wyxw swizzling.
        /// </summary>
        public hVector4 wyxw => new hVector4(w, y, x, w);
        
        /// <summary>
        /// Returns hVector4.agra swizzling (equivalent to hVector4.wyxw).
        /// </summary>
        public hVector4 agra => new hVector4(w, y, x, w);
        
        /// <summary>
        /// Returns hVector4.wyy swizzling.
        /// </summary>
        public hVector3 wyy => new hVector3(w, y, y);
        
        /// <summary>
        /// Returns hVector4.agg swizzling (equivalent to hVector4.wyy).
        /// </summary>
        public hVector3 agg => new hVector3(w, y, y);
        
        /// <summary>
        /// Returns hVector4.wyyx swizzling.
        /// </summary>
        public hVector4 wyyx => new hVector4(w, y, y, x);
        
        /// <summary>
        /// Returns hVector4.aggr swizzling (equivalent to hVector4.wyyx).
        /// </summary>
        public hVector4 aggr => new hVector4(w, y, y, x);
        
        /// <summary>
        /// Returns hVector4.wyyy swizzling.
        /// </summary>
        public hVector4 wyyy => new hVector4(w, y, y, y);
        
        /// <summary>
        /// Returns hVector4.aggg swizzling (equivalent to hVector4.wyyy).
        /// </summary>
        public hVector4 aggg => new hVector4(w, y, y, y);
        
        /// <summary>
        /// Returns hVector4.wyyz swizzling.
        /// </summary>
        public hVector4 wyyz => new hVector4(w, y, y, z);
        
        /// <summary>
        /// Returns hVector4.aggb swizzling (equivalent to hVector4.wyyz).
        /// </summary>
        public hVector4 aggb => new hVector4(w, y, y, z);
        
        /// <summary>
        /// Returns hVector4.wyyw swizzling.
        /// </summary>
        public hVector4 wyyw => new hVector4(w, y, y, w);
        
        /// <summary>
        /// Returns hVector4.agga swizzling (equivalent to hVector4.wyyw).
        /// </summary>
        public hVector4 agga => new hVector4(w, y, y, w);
        
        /// <summary>
        /// Returns hVector4.wyz swizzling.
        /// </summary>
        public hVector3 wyz => new hVector3(w, y, z);
        
        /// <summary>
        /// Returns hVector4.agb swizzling (equivalent to hVector4.wyz).
        /// </summary>
        public hVector3 agb => new hVector3(w, y, z);
        
        /// <summary>
        /// Returns hVector4.wyzx swizzling.
        /// </summary>
        public hVector4 wyzx => new hVector4(w, y, z, x);
        
        /// <summary>
        /// Returns hVector4.agbr swizzling (equivalent to hVector4.wyzx).
        /// </summary>
        public hVector4 agbr => new hVector4(w, y, z, x);
        
        /// <summary>
        /// Returns hVector4.wyzy swizzling.
        /// </summary>
        public hVector4 wyzy => new hVector4(w, y, z, y);
        
        /// <summary>
        /// Returns hVector4.agbg swizzling (equivalent to hVector4.wyzy).
        /// </summary>
        public hVector4 agbg => new hVector4(w, y, z, y);
        
        /// <summary>
        /// Returns hVector4.wyzz swizzling.
        /// </summary>
        public hVector4 wyzz => new hVector4(w, y, z, z);
        
        /// <summary>
        /// Returns hVector4.agbb swizzling (equivalent to hVector4.wyzz).
        /// </summary>
        public hVector4 agbb => new hVector4(w, y, z, z);
        
        /// <summary>
        /// Returns hVector4.wyzw swizzling.
        /// </summary>
        public hVector4 wyzw => new hVector4(w, y, z, w);
        
        /// <summary>
        /// Returns hVector4.agba swizzling (equivalent to hVector4.wyzw).
        /// </summary>
        public hVector4 agba => new hVector4(w, y, z, w);
        
        /// <summary>
        /// Returns hVector4.wyw swizzling.
        /// </summary>
        public hVector3 wyw => new hVector3(w, y, w);
        
        /// <summary>
        /// Returns hVector4.aga swizzling (equivalent to hVector4.wyw).
        /// </summary>
        public hVector3 aga => new hVector3(w, y, w);
        
        /// <summary>
        /// Returns hVector4.wywx swizzling.
        /// </summary>
        public hVector4 wywx => new hVector4(w, y, w, x);
        
        /// <summary>
        /// Returns hVector4.agar swizzling (equivalent to hVector4.wywx).
        /// </summary>
        public hVector4 agar => new hVector4(w, y, w, x);
        
        /// <summary>
        /// Returns hVector4.wywy swizzling.
        /// </summary>
        public hVector4 wywy => new hVector4(w, y, w, y);
        
        /// <summary>
        /// Returns hVector4.agag swizzling (equivalent to hVector4.wywy).
        /// </summary>
        public hVector4 agag => new hVector4(w, y, w, y);
        
        /// <summary>
        /// Returns hVector4.wywz swizzling.
        /// </summary>
        public hVector4 wywz => new hVector4(w, y, w, z);
        
        /// <summary>
        /// Returns hVector4.agab swizzling (equivalent to hVector4.wywz).
        /// </summary>
        public hVector4 agab => new hVector4(w, y, w, z);
        
        /// <summary>
        /// Returns hVector4.wyww swizzling.
        /// </summary>
        public hVector4 wyww => new hVector4(w, y, w, w);
        
        /// <summary>
        /// Returns hVector4.agaa swizzling (equivalent to hVector4.wyww).
        /// </summary>
        public hVector4 agaa => new hVector4(w, y, w, w);
        
        /// <summary>
        /// Returns hVector4.wz swizzling.
        /// </summary>
        public hVector2 wz => new hVector2(w, z);
        
        /// <summary>
        /// Returns hVector4.ab swizzling (equivalent to hVector4.wz).
        /// </summary>
        public hVector2 ab => new hVector2(w, z);
        
        /// <summary>
        /// Returns hVector4.wzx swizzling.
        /// </summary>
        public hVector3 wzx => new hVector3(w, z, x);
        
        /// <summary>
        /// Returns hVector4.abr swizzling (equivalent to hVector4.wzx).
        /// </summary>
        public hVector3 abr => new hVector3(w, z, x);
        
        /// <summary>
        /// Returns hVector4.wzxx swizzling.
        /// </summary>
        public hVector4 wzxx => new hVector4(w, z, x, x);
        
        /// <summary>
        /// Returns hVector4.abrr swizzling (equivalent to hVector4.wzxx).
        /// </summary>
        public hVector4 abrr => new hVector4(w, z, x, x);
        
        /// <summary>
        /// Returns hVector4.wzxy swizzling.
        /// </summary>
        public hVector4 wzxy => new hVector4(w, z, x, y);
        
        /// <summary>
        /// Returns hVector4.abrg swizzling (equivalent to hVector4.wzxy).
        /// </summary>
        public hVector4 abrg => new hVector4(w, z, x, y);
        
        /// <summary>
        /// Returns hVector4.wzxz swizzling.
        /// </summary>
        public hVector4 wzxz => new hVector4(w, z, x, z);
        
        /// <summary>
        /// Returns hVector4.abrb swizzling (equivalent to hVector4.wzxz).
        /// </summary>
        public hVector4 abrb => new hVector4(w, z, x, z);
        
        /// <summary>
        /// Returns hVector4.wzxw swizzling.
        /// </summary>
        public hVector4 wzxw => new hVector4(w, z, x, w);
        
        /// <summary>
        /// Returns hVector4.abra swizzling (equivalent to hVector4.wzxw).
        /// </summary>
        public hVector4 abra => new hVector4(w, z, x, w);
        
        /// <summary>
        /// Returns hVector4.wzy swizzling.
        /// </summary>
        public hVector3 wzy => new hVector3(w, z, y);
        
        /// <summary>
        /// Returns hVector4.abg swizzling (equivalent to hVector4.wzy).
        /// </summary>
        public hVector3 abg => new hVector3(w, z, y);
        
        /// <summary>
        /// Returns hVector4.wzyx swizzling.
        /// </summary>
        public hVector4 wzyx => new hVector4(w, z, y, x);
        
        /// <summary>
        /// Returns hVector4.abgr swizzling (equivalent to hVector4.wzyx).
        /// </summary>
        public hVector4 abgr => new hVector4(w, z, y, x);
        
        /// <summary>
        /// Returns hVector4.wzyy swizzling.
        /// </summary>
        public hVector4 wzyy => new hVector4(w, z, y, y);
        
        /// <summary>
        /// Returns hVector4.abgg swizzling (equivalent to hVector4.wzyy).
        /// </summary>
        public hVector4 abgg => new hVector4(w, z, y, y);
        
        /// <summary>
        /// Returns hVector4.wzyz swizzling.
        /// </summary>
        public hVector4 wzyz => new hVector4(w, z, y, z);
        
        /// <summary>
        /// Returns hVector4.abgb swizzling (equivalent to hVector4.wzyz).
        /// </summary>
        public hVector4 abgb => new hVector4(w, z, y, z);
        
        /// <summary>
        /// Returns hVector4.wzyw swizzling.
        /// </summary>
        public hVector4 wzyw => new hVector4(w, z, y, w);
        
        /// <summary>
        /// Returns hVector4.abga swizzling (equivalent to hVector4.wzyw).
        /// </summary>
        public hVector4 abga => new hVector4(w, z, y, w);
        
        /// <summary>
        /// Returns hVector4.wzz swizzling.
        /// </summary>
        public hVector3 wzz => new hVector3(w, z, z);
        
        /// <summary>
        /// Returns hVector4.abb swizzling (equivalent to hVector4.wzz).
        /// </summary>
        public hVector3 abb => new hVector3(w, z, z);
        
        /// <summary>
        /// Returns hVector4.wzzx swizzling.
        /// </summary>
        public hVector4 wzzx => new hVector4(w, z, z, x);
        
        /// <summary>
        /// Returns hVector4.abbr swizzling (equivalent to hVector4.wzzx).
        /// </summary>
        public hVector4 abbr => new hVector4(w, z, z, x);
        
        /// <summary>
        /// Returns hVector4.wzzy swizzling.
        /// </summary>
        public hVector4 wzzy => new hVector4(w, z, z, y);
        
        /// <summary>
        /// Returns hVector4.abbg swizzling (equivalent to hVector4.wzzy).
        /// </summary>
        public hVector4 abbg => new hVector4(w, z, z, y);
        
        /// <summary>
        /// Returns hVector4.wzzz swizzling.
        /// </summary>
        public hVector4 wzzz => new hVector4(w, z, z, z);
        
        /// <summary>
        /// Returns hVector4.abbb swizzling (equivalent to hVector4.wzzz).
        /// </summary>
        public hVector4 abbb => new hVector4(w, z, z, z);
        
        /// <summary>
        /// Returns hVector4.wzzw swizzling.
        /// </summary>
        public hVector4 wzzw => new hVector4(w, z, z, w);
        
        /// <summary>
        /// Returns hVector4.abba swizzling (equivalent to hVector4.wzzw).
        /// </summary>
        public hVector4 abba => new hVector4(w, z, z, w);
        
        /// <summary>
        /// Returns hVector4.wzw swizzling.
        /// </summary>
        public hVector3 wzw => new hVector3(w, z, w);
        
        /// <summary>
        /// Returns hVector4.aba swizzling (equivalent to hVector4.wzw).
        /// </summary>
        public hVector3 aba => new hVector3(w, z, w);
        
        /// <summary>
        /// Returns hVector4.wzwx swizzling.
        /// </summary>
        public hVector4 wzwx => new hVector4(w, z, w, x);
        
        /// <summary>
        /// Returns hVector4.abar swizzling (equivalent to hVector4.wzwx).
        /// </summary>
        public hVector4 abar => new hVector4(w, z, w, x);
        
        /// <summary>
        /// Returns hVector4.wzwy swizzling.
        /// </summary>
        public hVector4 wzwy => new hVector4(w, z, w, y);
        
        /// <summary>
        /// Returns hVector4.abag swizzling (equivalent to hVector4.wzwy).
        /// </summary>
        public hVector4 abag => new hVector4(w, z, w, y);
        
        /// <summary>
        /// Returns hVector4.wzwz swizzling.
        /// </summary>
        public hVector4 wzwz => new hVector4(w, z, w, z);
        
        /// <summary>
        /// Returns hVector4.abab swizzling (equivalent to hVector4.wzwz).
        /// </summary>
        public hVector4 abab => new hVector4(w, z, w, z);
        
        /// <summary>
        /// Returns hVector4.wzww swizzling.
        /// </summary>
        public hVector4 wzww => new hVector4(w, z, w, w);
        
        /// <summary>
        /// Returns hVector4.abaa swizzling (equivalent to hVector4.wzww).
        /// </summary>
        public hVector4 abaa => new hVector4(w, z, w, w);
        
        /// <summary>
        /// Returns hVector4.ww swizzling.
        /// </summary>
        public hVector2 ww => new hVector2(w, w);
        
        /// <summary>
        /// Returns hVector4.aa swizzling (equivalent to hVector4.ww).
        /// </summary>
        public hVector2 aa => new hVector2(w, w);
        
        /// <summary>
        /// Returns hVector4.wwx swizzling.
        /// </summary>
        public hVector3 wwx => new hVector3(w, w, x);
        
        /// <summary>
        /// Returns hVector4.aar swizzling (equivalent to hVector4.wwx).
        /// </summary>
        public hVector3 aar => new hVector3(w, w, x);
        
        /// <summary>
        /// Returns hVector4.wwxx swizzling.
        /// </summary>
        public hVector4 wwxx => new hVector4(w, w, x, x);
        
        /// <summary>
        /// Returns hVector4.aarr swizzling (equivalent to hVector4.wwxx).
        /// </summary>
        public hVector4 aarr => new hVector4(w, w, x, x);
        
        /// <summary>
        /// Returns hVector4.wwxy swizzling.
        /// </summary>
        public hVector4 wwxy => new hVector4(w, w, x, y);
        
        /// <summary>
        /// Returns hVector4.aarg swizzling (equivalent to hVector4.wwxy).
        /// </summary>
        public hVector4 aarg => new hVector4(w, w, x, y);
        
        /// <summary>
        /// Returns hVector4.wwxz swizzling.
        /// </summary>
        public hVector4 wwxz => new hVector4(w, w, x, z);
        
        /// <summary>
        /// Returns hVector4.aarb swizzling (equivalent to hVector4.wwxz).
        /// </summary>
        public hVector4 aarb => new hVector4(w, w, x, z);
        
        /// <summary>
        /// Returns hVector4.wwxw swizzling.
        /// </summary>
        public hVector4 wwxw => new hVector4(w, w, x, w);
        
        /// <summary>
        /// Returns hVector4.aara swizzling (equivalent to hVector4.wwxw).
        /// </summary>
        public hVector4 aara => new hVector4(w, w, x, w);
        
        /// <summary>
        /// Returns hVector4.wwy swizzling.
        /// </summary>
        public hVector3 wwy => new hVector3(w, w, y);
        
        /// <summary>
        /// Returns hVector4.aag swizzling (equivalent to hVector4.wwy).
        /// </summary>
        public hVector3 aag => new hVector3(w, w, y);
        
        /// <summary>
        /// Returns hVector4.wwyx swizzling.
        /// </summary>
        public hVector4 wwyx => new hVector4(w, w, y, x);
        
        /// <summary>
        /// Returns hVector4.aagr swizzling (equivalent to hVector4.wwyx).
        /// </summary>
        public hVector4 aagr => new hVector4(w, w, y, x);
        
        /// <summary>
        /// Returns hVector4.wwyy swizzling.
        /// </summary>
        public hVector4 wwyy => new hVector4(w, w, y, y);
        
        /// <summary>
        /// Returns hVector4.aagg swizzling (equivalent to hVector4.wwyy).
        /// </summary>
        public hVector4 aagg => new hVector4(w, w, y, y);
        
        /// <summary>
        /// Returns hVector4.wwyz swizzling.
        /// </summary>
        public hVector4 wwyz => new hVector4(w, w, y, z);
        
        /// <summary>
        /// Returns hVector4.aagb swizzling (equivalent to hVector4.wwyz).
        /// </summary>
        public hVector4 aagb => new hVector4(w, w, y, z);
        
        /// <summary>
        /// Returns hVector4.wwyw swizzling.
        /// </summary>
        public hVector4 wwyw => new hVector4(w, w, y, w);
        
        /// <summary>
        /// Returns hVector4.aaga swizzling (equivalent to hVector4.wwyw).
        /// </summary>
        public hVector4 aaga => new hVector4(w, w, y, w);
        
        /// <summary>
        /// Returns hVector4.wwz swizzling.
        /// </summary>
        public hVector3 wwz => new hVector3(w, w, z);
        
        /// <summary>
        /// Returns hVector4.aab swizzling (equivalent to hVector4.wwz).
        /// </summary>
        public hVector3 aab => new hVector3(w, w, z);
        
        /// <summary>
        /// Returns hVector4.wwzx swizzling.
        /// </summary>
        public hVector4 wwzx => new hVector4(w, w, z, x);
        
        /// <summary>
        /// Returns hVector4.aabr swizzling (equivalent to hVector4.wwzx).
        /// </summary>
        public hVector4 aabr => new hVector4(w, w, z, x);
        
        /// <summary>
        /// Returns hVector4.wwzy swizzling.
        /// </summary>
        public hVector4 wwzy => new hVector4(w, w, z, y);
        
        /// <summary>
        /// Returns hVector4.aabg swizzling (equivalent to hVector4.wwzy).
        /// </summary>
        public hVector4 aabg => new hVector4(w, w, z, y);
        
        /// <summary>
        /// Returns hVector4.wwzz swizzling.
        /// </summary>
        public hVector4 wwzz => new hVector4(w, w, z, z);
        
        /// <summary>
        /// Returns hVector4.aabb swizzling (equivalent to hVector4.wwzz).
        /// </summary>
        public hVector4 aabb => new hVector4(w, w, z, z);
        
        /// <summary>
        /// Returns hVector4.wwzw swizzling.
        /// </summary>
        public hVector4 wwzw => new hVector4(w, w, z, w);
        
        /// <summary>
        /// Returns hVector4.aaba swizzling (equivalent to hVector4.wwzw).
        /// </summary>
        public hVector4 aaba => new hVector4(w, w, z, w);
        
        /// <summary>
        /// Returns hVector4.www swizzling.
        /// </summary>
        public hVector3 www => new hVector3(w, w, w);
        
        /// <summary>
        /// Returns hVector4.aaa swizzling (equivalent to hVector4.www).
        /// </summary>
        public hVector3 aaa => new hVector3(w, w, w);
        
        /// <summary>
        /// Returns hVector4.wwwx swizzling.
        /// </summary>
        public hVector4 wwwx => new hVector4(w, w, w, x);
        
        /// <summary>
        /// Returns hVector4.aaar swizzling (equivalent to hVector4.wwwx).
        /// </summary>
        public hVector4 aaar => new hVector4(w, w, w, x);
        
        /// <summary>
        /// Returns hVector4.wwwy swizzling.
        /// </summary>
        public hVector4 wwwy => new hVector4(w, w, w, y);
        
        /// <summary>
        /// Returns hVector4.aaag swizzling (equivalent to hVector4.wwwy).
        /// </summary>
        public hVector4 aaag => new hVector4(w, w, w, y);
        
        /// <summary>
        /// Returns hVector4.wwwz swizzling.
        /// </summary>
        public hVector4 wwwz => new hVector4(w, w, w, z);
        
        /// <summary>
        /// Returns hVector4.aaab swizzling (equivalent to hVector4.wwwz).
        /// </summary>
        public hVector4 aaab => new hVector4(w, w, w, z);
        
        /// <summary>
        /// Returns hVector4.wwww swizzling.
        /// </summary>
        public hVector4 wwww => new hVector4(w, w, w, w);
        
        /// <summary>
        /// Returns hVector4.aaaa swizzling (equivalent to hVector4.wwww).
        /// </summary>
        public hVector4 aaaa => new hVector4(w, w, w, w);

        #endregion

    }
}
