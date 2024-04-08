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
    /// Temporary Vector of type double with 4 components, used for implementing swizzling for dVector4.
    /// </summary>
    [RegisterCoreClassStruct]
   
    [StructLayout(LayoutKind.Sequential)]
    public struct swizzle_dVector4
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
        
        /// <summary>
        /// w-component
        /// </summary>
        
        internal readonly double w;

        #endregion


        #region Constructors
        
        /// <summary>
        /// Constructor for swizzle_dVector4.
        /// </summary>
        internal swizzle_dVector4(double x, double y, double z, double w)
        {
            this.x = x;
            this.y = y;
            this.z = z;
            this.w = w;
        }

        #endregion


        #region Properties
        
        /// <summary>
        /// Returns dVector4.xx swizzling.
        /// </summary>
        public dVector2 xx => new dVector2(x, x);
        
        /// <summary>
        /// Returns dVector4.rr swizzling (equivalent to dVector4.xx).
        /// </summary>
        public dVector2 rr => new dVector2(x, x);
        
        /// <summary>
        /// Returns dVector4.xxx swizzling.
        /// </summary>
        public dVector3 xxx => new dVector3(x, x, x);
        
        /// <summary>
        /// Returns dVector4.rrr swizzling (equivalent to dVector4.xxx).
        /// </summary>
        public dVector3 rrr => new dVector3(x, x, x);
        
        /// <summary>
        /// Returns dVector4.xxxx swizzling.
        /// </summary>
        public dVector4 xxxx => new dVector4(x, x, x, x);
        
        /// <summary>
        /// Returns dVector4.rrrr swizzling (equivalent to dVector4.xxxx).
        /// </summary>
        public dVector4 rrrr => new dVector4(x, x, x, x);
        
        /// <summary>
        /// Returns dVector4.xxxy swizzling.
        /// </summary>
        public dVector4 xxxy => new dVector4(x, x, x, y);
        
        /// <summary>
        /// Returns dVector4.rrrg swizzling (equivalent to dVector4.xxxy).
        /// </summary>
        public dVector4 rrrg => new dVector4(x, x, x, y);
        
        /// <summary>
        /// Returns dVector4.xxxz swizzling.
        /// </summary>
        public dVector4 xxxz => new dVector4(x, x, x, z);
        
        /// <summary>
        /// Returns dVector4.rrrb swizzling (equivalent to dVector4.xxxz).
        /// </summary>
        public dVector4 rrrb => new dVector4(x, x, x, z);
        
        /// <summary>
        /// Returns dVector4.xxxw swizzling.
        /// </summary>
        public dVector4 xxxw => new dVector4(x, x, x, w);
        
        /// <summary>
        /// Returns dVector4.rrra swizzling (equivalent to dVector4.xxxw).
        /// </summary>
        public dVector4 rrra => new dVector4(x, x, x, w);
        
        /// <summary>
        /// Returns dVector4.xxy swizzling.
        /// </summary>
        public dVector3 xxy => new dVector3(x, x, y);
        
        /// <summary>
        /// Returns dVector4.rrg swizzling (equivalent to dVector4.xxy).
        /// </summary>
        public dVector3 rrg => new dVector3(x, x, y);
        
        /// <summary>
        /// Returns dVector4.xxyx swizzling.
        /// </summary>
        public dVector4 xxyx => new dVector4(x, x, y, x);
        
        /// <summary>
        /// Returns dVector4.rrgr swizzling (equivalent to dVector4.xxyx).
        /// </summary>
        public dVector4 rrgr => new dVector4(x, x, y, x);
        
        /// <summary>
        /// Returns dVector4.xxyy swizzling.
        /// </summary>
        public dVector4 xxyy => new dVector4(x, x, y, y);
        
        /// <summary>
        /// Returns dVector4.rrgg swizzling (equivalent to dVector4.xxyy).
        /// </summary>
        public dVector4 rrgg => new dVector4(x, x, y, y);
        
        /// <summary>
        /// Returns dVector4.xxyz swizzling.
        /// </summary>
        public dVector4 xxyz => new dVector4(x, x, y, z);
        
        /// <summary>
        /// Returns dVector4.rrgb swizzling (equivalent to dVector4.xxyz).
        /// </summary>
        public dVector4 rrgb => new dVector4(x, x, y, z);
        
        /// <summary>
        /// Returns dVector4.xxyw swizzling.
        /// </summary>
        public dVector4 xxyw => new dVector4(x, x, y, w);
        
        /// <summary>
        /// Returns dVector4.rrga swizzling (equivalent to dVector4.xxyw).
        /// </summary>
        public dVector4 rrga => new dVector4(x, x, y, w);
        
        /// <summary>
        /// Returns dVector4.xxz swizzling.
        /// </summary>
        public dVector3 xxz => new dVector3(x, x, z);
        
        /// <summary>
        /// Returns dVector4.rrb swizzling (equivalent to dVector4.xxz).
        /// </summary>
        public dVector3 rrb => new dVector3(x, x, z);
        
        /// <summary>
        /// Returns dVector4.xxzx swizzling.
        /// </summary>
        public dVector4 xxzx => new dVector4(x, x, z, x);
        
        /// <summary>
        /// Returns dVector4.rrbr swizzling (equivalent to dVector4.xxzx).
        /// </summary>
        public dVector4 rrbr => new dVector4(x, x, z, x);
        
        /// <summary>
        /// Returns dVector4.xxzy swizzling.
        /// </summary>
        public dVector4 xxzy => new dVector4(x, x, z, y);
        
        /// <summary>
        /// Returns dVector4.rrbg swizzling (equivalent to dVector4.xxzy).
        /// </summary>
        public dVector4 rrbg => new dVector4(x, x, z, y);
        
        /// <summary>
        /// Returns dVector4.xxzz swizzling.
        /// </summary>
        public dVector4 xxzz => new dVector4(x, x, z, z);
        
        /// <summary>
        /// Returns dVector4.rrbb swizzling (equivalent to dVector4.xxzz).
        /// </summary>
        public dVector4 rrbb => new dVector4(x, x, z, z);
        
        /// <summary>
        /// Returns dVector4.xxzw swizzling.
        /// </summary>
        public dVector4 xxzw => new dVector4(x, x, z, w);
        
        /// <summary>
        /// Returns dVector4.rrba swizzling (equivalent to dVector4.xxzw).
        /// </summary>
        public dVector4 rrba => new dVector4(x, x, z, w);
        
        /// <summary>
        /// Returns dVector4.xxw swizzling.
        /// </summary>
        public dVector3 xxw => new dVector3(x, x, w);
        
        /// <summary>
        /// Returns dVector4.rra swizzling (equivalent to dVector4.xxw).
        /// </summary>
        public dVector3 rra => new dVector3(x, x, w);
        
        /// <summary>
        /// Returns dVector4.xxwx swizzling.
        /// </summary>
        public dVector4 xxwx => new dVector4(x, x, w, x);
        
        /// <summary>
        /// Returns dVector4.rrar swizzling (equivalent to dVector4.xxwx).
        /// </summary>
        public dVector4 rrar => new dVector4(x, x, w, x);
        
        /// <summary>
        /// Returns dVector4.xxwy swizzling.
        /// </summary>
        public dVector4 xxwy => new dVector4(x, x, w, y);
        
        /// <summary>
        /// Returns dVector4.rrag swizzling (equivalent to dVector4.xxwy).
        /// </summary>
        public dVector4 rrag => new dVector4(x, x, w, y);
        
        /// <summary>
        /// Returns dVector4.xxwz swizzling.
        /// </summary>
        public dVector4 xxwz => new dVector4(x, x, w, z);
        
        /// <summary>
        /// Returns dVector4.rrab swizzling (equivalent to dVector4.xxwz).
        /// </summary>
        public dVector4 rrab => new dVector4(x, x, w, z);
        
        /// <summary>
        /// Returns dVector4.xxww swizzling.
        /// </summary>
        public dVector4 xxww => new dVector4(x, x, w, w);
        
        /// <summary>
        /// Returns dVector4.rraa swizzling (equivalent to dVector4.xxww).
        /// </summary>
        public dVector4 rraa => new dVector4(x, x, w, w);
        
        /// <summary>
        /// Returns dVector4.xy swizzling.
        /// </summary>
        public dVector2 xy => new dVector2(x, y);
        
        /// <summary>
        /// Returns dVector4.rg swizzling (equivalent to dVector4.xy).
        /// </summary>
        public dVector2 rg => new dVector2(x, y);
        
        /// <summary>
        /// Returns dVector4.xyx swizzling.
        /// </summary>
        public dVector3 xyx => new dVector3(x, y, x);
        
        /// <summary>
        /// Returns dVector4.rgr swizzling (equivalent to dVector4.xyx).
        /// </summary>
        public dVector3 rgr => new dVector3(x, y, x);
        
        /// <summary>
        /// Returns dVector4.xyxx swizzling.
        /// </summary>
        public dVector4 xyxx => new dVector4(x, y, x, x);
        
        /// <summary>
        /// Returns dVector4.rgrr swizzling (equivalent to dVector4.xyxx).
        /// </summary>
        public dVector4 rgrr => new dVector4(x, y, x, x);
        
        /// <summary>
        /// Returns dVector4.xyxy swizzling.
        /// </summary>
        public dVector4 xyxy => new dVector4(x, y, x, y);
        
        /// <summary>
        /// Returns dVector4.rgrg swizzling (equivalent to dVector4.xyxy).
        /// </summary>
        public dVector4 rgrg => new dVector4(x, y, x, y);
        
        /// <summary>
        /// Returns dVector4.xyxz swizzling.
        /// </summary>
        public dVector4 xyxz => new dVector4(x, y, x, z);
        
        /// <summary>
        /// Returns dVector4.rgrb swizzling (equivalent to dVector4.xyxz).
        /// </summary>
        public dVector4 rgrb => new dVector4(x, y, x, z);
        
        /// <summary>
        /// Returns dVector4.xyxw swizzling.
        /// </summary>
        public dVector4 xyxw => new dVector4(x, y, x, w);
        
        /// <summary>
        /// Returns dVector4.rgra swizzling (equivalent to dVector4.xyxw).
        /// </summary>
        public dVector4 rgra => new dVector4(x, y, x, w);
        
        /// <summary>
        /// Returns dVector4.xyy swizzling.
        /// </summary>
        public dVector3 xyy => new dVector3(x, y, y);
        
        /// <summary>
        /// Returns dVector4.rgg swizzling (equivalent to dVector4.xyy).
        /// </summary>
        public dVector3 rgg => new dVector3(x, y, y);
        
        /// <summary>
        /// Returns dVector4.xyyx swizzling.
        /// </summary>
        public dVector4 xyyx => new dVector4(x, y, y, x);
        
        /// <summary>
        /// Returns dVector4.rggr swizzling (equivalent to dVector4.xyyx).
        /// </summary>
        public dVector4 rggr => new dVector4(x, y, y, x);
        
        /// <summary>
        /// Returns dVector4.xyyy swizzling.
        /// </summary>
        public dVector4 xyyy => new dVector4(x, y, y, y);
        
        /// <summary>
        /// Returns dVector4.rggg swizzling (equivalent to dVector4.xyyy).
        /// </summary>
        public dVector4 rggg => new dVector4(x, y, y, y);
        
        /// <summary>
        /// Returns dVector4.xyyz swizzling.
        /// </summary>
        public dVector4 xyyz => new dVector4(x, y, y, z);
        
        /// <summary>
        /// Returns dVector4.rggb swizzling (equivalent to dVector4.xyyz).
        /// </summary>
        public dVector4 rggb => new dVector4(x, y, y, z);
        
        /// <summary>
        /// Returns dVector4.xyyw swizzling.
        /// </summary>
        public dVector4 xyyw => new dVector4(x, y, y, w);
        
        /// <summary>
        /// Returns dVector4.rgga swizzling (equivalent to dVector4.xyyw).
        /// </summary>
        public dVector4 rgga => new dVector4(x, y, y, w);
        
        /// <summary>
        /// Returns dVector4.xyz swizzling.
        /// </summary>
        public dVector3 xyz => new dVector3(x, y, z);
        
        /// <summary>
        /// Returns dVector4.rgb swizzling (equivalent to dVector4.xyz).
        /// </summary>
        public dVector3 rgb => new dVector3(x, y, z);
        
        /// <summary>
        /// Returns dVector4.xyzx swizzling.
        /// </summary>
        public dVector4 xyzx => new dVector4(x, y, z, x);
        
        /// <summary>
        /// Returns dVector4.rgbr swizzling (equivalent to dVector4.xyzx).
        /// </summary>
        public dVector4 rgbr => new dVector4(x, y, z, x);
        
        /// <summary>
        /// Returns dVector4.xyzy swizzling.
        /// </summary>
        public dVector4 xyzy => new dVector4(x, y, z, y);
        
        /// <summary>
        /// Returns dVector4.rgbg swizzling (equivalent to dVector4.xyzy).
        /// </summary>
        public dVector4 rgbg => new dVector4(x, y, z, y);
        
        /// <summary>
        /// Returns dVector4.xyzz swizzling.
        /// </summary>
        public dVector4 xyzz => new dVector4(x, y, z, z);
        
        /// <summary>
        /// Returns dVector4.rgbb swizzling (equivalent to dVector4.xyzz).
        /// </summary>
        public dVector4 rgbb => new dVector4(x, y, z, z);
        
        /// <summary>
        /// Returns dVector4.xyzw swizzling.
        /// </summary>
        public dVector4 xyzw => new dVector4(x, y, z, w);
        
        /// <summary>
        /// Returns dVector4.rgba swizzling (equivalent to dVector4.xyzw).
        /// </summary>
        public dVector4 rgba => new dVector4(x, y, z, w);
        
        /// <summary>
        /// Returns dVector4.xyw swizzling.
        /// </summary>
        public dVector3 xyw => new dVector3(x, y, w);
        
        /// <summary>
        /// Returns dVector4.rga swizzling (equivalent to dVector4.xyw).
        /// </summary>
        public dVector3 rga => new dVector3(x, y, w);
        
        /// <summary>
        /// Returns dVector4.xywx swizzling.
        /// </summary>
        public dVector4 xywx => new dVector4(x, y, w, x);
        
        /// <summary>
        /// Returns dVector4.rgar swizzling (equivalent to dVector4.xywx).
        /// </summary>
        public dVector4 rgar => new dVector4(x, y, w, x);
        
        /// <summary>
        /// Returns dVector4.xywy swizzling.
        /// </summary>
        public dVector4 xywy => new dVector4(x, y, w, y);
        
        /// <summary>
        /// Returns dVector4.rgag swizzling (equivalent to dVector4.xywy).
        /// </summary>
        public dVector4 rgag => new dVector4(x, y, w, y);
        
        /// <summary>
        /// Returns dVector4.xywz swizzling.
        /// </summary>
        public dVector4 xywz => new dVector4(x, y, w, z);
        
        /// <summary>
        /// Returns dVector4.rgab swizzling (equivalent to dVector4.xywz).
        /// </summary>
        public dVector4 rgab => new dVector4(x, y, w, z);
        
        /// <summary>
        /// Returns dVector4.xyww swizzling.
        /// </summary>
        public dVector4 xyww => new dVector4(x, y, w, w);
        
        /// <summary>
        /// Returns dVector4.rgaa swizzling (equivalent to dVector4.xyww).
        /// </summary>
        public dVector4 rgaa => new dVector4(x, y, w, w);
        
        /// <summary>
        /// Returns dVector4.xz swizzling.
        /// </summary>
        public dVector2 xz => new dVector2(x, z);
        
        /// <summary>
        /// Returns dVector4.rb swizzling (equivalent to dVector4.xz).
        /// </summary>
        public dVector2 rb => new dVector2(x, z);
        
        /// <summary>
        /// Returns dVector4.xzx swizzling.
        /// </summary>
        public dVector3 xzx => new dVector3(x, z, x);
        
        /// <summary>
        /// Returns dVector4.rbr swizzling (equivalent to dVector4.xzx).
        /// </summary>
        public dVector3 rbr => new dVector3(x, z, x);
        
        /// <summary>
        /// Returns dVector4.xzxx swizzling.
        /// </summary>
        public dVector4 xzxx => new dVector4(x, z, x, x);
        
        /// <summary>
        /// Returns dVector4.rbrr swizzling (equivalent to dVector4.xzxx).
        /// </summary>
        public dVector4 rbrr => new dVector4(x, z, x, x);
        
        /// <summary>
        /// Returns dVector4.xzxy swizzling.
        /// </summary>
        public dVector4 xzxy => new dVector4(x, z, x, y);
        
        /// <summary>
        /// Returns dVector4.rbrg swizzling (equivalent to dVector4.xzxy).
        /// </summary>
        public dVector4 rbrg => new dVector4(x, z, x, y);
        
        /// <summary>
        /// Returns dVector4.xzxz swizzling.
        /// </summary>
        public dVector4 xzxz => new dVector4(x, z, x, z);
        
        /// <summary>
        /// Returns dVector4.rbrb swizzling (equivalent to dVector4.xzxz).
        /// </summary>
        public dVector4 rbrb => new dVector4(x, z, x, z);
        
        /// <summary>
        /// Returns dVector4.xzxw swizzling.
        /// </summary>
        public dVector4 xzxw => new dVector4(x, z, x, w);
        
        /// <summary>
        /// Returns dVector4.rbra swizzling (equivalent to dVector4.xzxw).
        /// </summary>
        public dVector4 rbra => new dVector4(x, z, x, w);
        
        /// <summary>
        /// Returns dVector4.xzy swizzling.
        /// </summary>
        public dVector3 xzy => new dVector3(x, z, y);
        
        /// <summary>
        /// Returns dVector4.rbg swizzling (equivalent to dVector4.xzy).
        /// </summary>
        public dVector3 rbg => new dVector3(x, z, y);
        
        /// <summary>
        /// Returns dVector4.xzyx swizzling.
        /// </summary>
        public dVector4 xzyx => new dVector4(x, z, y, x);
        
        /// <summary>
        /// Returns dVector4.rbgr swizzling (equivalent to dVector4.xzyx).
        /// </summary>
        public dVector4 rbgr => new dVector4(x, z, y, x);
        
        /// <summary>
        /// Returns dVector4.xzyy swizzling.
        /// </summary>
        public dVector4 xzyy => new dVector4(x, z, y, y);
        
        /// <summary>
        /// Returns dVector4.rbgg swizzling (equivalent to dVector4.xzyy).
        /// </summary>
        public dVector4 rbgg => new dVector4(x, z, y, y);
        
        /// <summary>
        /// Returns dVector4.xzyz swizzling.
        /// </summary>
        public dVector4 xzyz => new dVector4(x, z, y, z);
        
        /// <summary>
        /// Returns dVector4.rbgb swizzling (equivalent to dVector4.xzyz).
        /// </summary>
        public dVector4 rbgb => new dVector4(x, z, y, z);
        
        /// <summary>
        /// Returns dVector4.xzyw swizzling.
        /// </summary>
        public dVector4 xzyw => new dVector4(x, z, y, w);
        
        /// <summary>
        /// Returns dVector4.rbga swizzling (equivalent to dVector4.xzyw).
        /// </summary>
        public dVector4 rbga => new dVector4(x, z, y, w);
        
        /// <summary>
        /// Returns dVector4.xzz swizzling.
        /// </summary>
        public dVector3 xzz => new dVector3(x, z, z);
        
        /// <summary>
        /// Returns dVector4.rbb swizzling (equivalent to dVector4.xzz).
        /// </summary>
        public dVector3 rbb => new dVector3(x, z, z);
        
        /// <summary>
        /// Returns dVector4.xzzx swizzling.
        /// </summary>
        public dVector4 xzzx => new dVector4(x, z, z, x);
        
        /// <summary>
        /// Returns dVector4.rbbr swizzling (equivalent to dVector4.xzzx).
        /// </summary>
        public dVector4 rbbr => new dVector4(x, z, z, x);
        
        /// <summary>
        /// Returns dVector4.xzzy swizzling.
        /// </summary>
        public dVector4 xzzy => new dVector4(x, z, z, y);
        
        /// <summary>
        /// Returns dVector4.rbbg swizzling (equivalent to dVector4.xzzy).
        /// </summary>
        public dVector4 rbbg => new dVector4(x, z, z, y);
        
        /// <summary>
        /// Returns dVector4.xzzz swizzling.
        /// </summary>
        public dVector4 xzzz => new dVector4(x, z, z, z);
        
        /// <summary>
        /// Returns dVector4.rbbb swizzling (equivalent to dVector4.xzzz).
        /// </summary>
        public dVector4 rbbb => new dVector4(x, z, z, z);
        
        /// <summary>
        /// Returns dVector4.xzzw swizzling.
        /// </summary>
        public dVector4 xzzw => new dVector4(x, z, z, w);
        
        /// <summary>
        /// Returns dVector4.rbba swizzling (equivalent to dVector4.xzzw).
        /// </summary>
        public dVector4 rbba => new dVector4(x, z, z, w);
        
        /// <summary>
        /// Returns dVector4.xzw swizzling.
        /// </summary>
        public dVector3 xzw => new dVector3(x, z, w);
        
        /// <summary>
        /// Returns dVector4.rba swizzling (equivalent to dVector4.xzw).
        /// </summary>
        public dVector3 rba => new dVector3(x, z, w);
        
        /// <summary>
        /// Returns dVector4.xzwx swizzling.
        /// </summary>
        public dVector4 xzwx => new dVector4(x, z, w, x);
        
        /// <summary>
        /// Returns dVector4.rbar swizzling (equivalent to dVector4.xzwx).
        /// </summary>
        public dVector4 rbar => new dVector4(x, z, w, x);
        
        /// <summary>
        /// Returns dVector4.xzwy swizzling.
        /// </summary>
        public dVector4 xzwy => new dVector4(x, z, w, y);
        
        /// <summary>
        /// Returns dVector4.rbag swizzling (equivalent to dVector4.xzwy).
        /// </summary>
        public dVector4 rbag => new dVector4(x, z, w, y);
        
        /// <summary>
        /// Returns dVector4.xzwz swizzling.
        /// </summary>
        public dVector4 xzwz => new dVector4(x, z, w, z);
        
        /// <summary>
        /// Returns dVector4.rbab swizzling (equivalent to dVector4.xzwz).
        /// </summary>
        public dVector4 rbab => new dVector4(x, z, w, z);
        
        /// <summary>
        /// Returns dVector4.xzww swizzling.
        /// </summary>
        public dVector4 xzww => new dVector4(x, z, w, w);
        
        /// <summary>
        /// Returns dVector4.rbaa swizzling (equivalent to dVector4.xzww).
        /// </summary>
        public dVector4 rbaa => new dVector4(x, z, w, w);
        
        /// <summary>
        /// Returns dVector4.xw swizzling.
        /// </summary>
        public dVector2 xw => new dVector2(x, w);
        
        /// <summary>
        /// Returns dVector4.ra swizzling (equivalent to dVector4.xw).
        /// </summary>
        public dVector2 ra => new dVector2(x, w);
        
        /// <summary>
        /// Returns dVector4.xwx swizzling.
        /// </summary>
        public dVector3 xwx => new dVector3(x, w, x);
        
        /// <summary>
        /// Returns dVector4.rar swizzling (equivalent to dVector4.xwx).
        /// </summary>
        public dVector3 rar => new dVector3(x, w, x);
        
        /// <summary>
        /// Returns dVector4.xwxx swizzling.
        /// </summary>
        public dVector4 xwxx => new dVector4(x, w, x, x);
        
        /// <summary>
        /// Returns dVector4.rarr swizzling (equivalent to dVector4.xwxx).
        /// </summary>
        public dVector4 rarr => new dVector4(x, w, x, x);
        
        /// <summary>
        /// Returns dVector4.xwxy swizzling.
        /// </summary>
        public dVector4 xwxy => new dVector4(x, w, x, y);
        
        /// <summary>
        /// Returns dVector4.rarg swizzling (equivalent to dVector4.xwxy).
        /// </summary>
        public dVector4 rarg => new dVector4(x, w, x, y);
        
        /// <summary>
        /// Returns dVector4.xwxz swizzling.
        /// </summary>
        public dVector4 xwxz => new dVector4(x, w, x, z);
        
        /// <summary>
        /// Returns dVector4.rarb swizzling (equivalent to dVector4.xwxz).
        /// </summary>
        public dVector4 rarb => new dVector4(x, w, x, z);
        
        /// <summary>
        /// Returns dVector4.xwxw swizzling.
        /// </summary>
        public dVector4 xwxw => new dVector4(x, w, x, w);
        
        /// <summary>
        /// Returns dVector4.rara swizzling (equivalent to dVector4.xwxw).
        /// </summary>
        public dVector4 rara => new dVector4(x, w, x, w);
        
        /// <summary>
        /// Returns dVector4.xwy swizzling.
        /// </summary>
        public dVector3 xwy => new dVector3(x, w, y);
        
        /// <summary>
        /// Returns dVector4.rag swizzling (equivalent to dVector4.xwy).
        /// </summary>
        public dVector3 rag => new dVector3(x, w, y);
        
        /// <summary>
        /// Returns dVector4.xwyx swizzling.
        /// </summary>
        public dVector4 xwyx => new dVector4(x, w, y, x);
        
        /// <summary>
        /// Returns dVector4.ragr swizzling (equivalent to dVector4.xwyx).
        /// </summary>
        public dVector4 ragr => new dVector4(x, w, y, x);
        
        /// <summary>
        /// Returns dVector4.xwyy swizzling.
        /// </summary>
        public dVector4 xwyy => new dVector4(x, w, y, y);
        
        /// <summary>
        /// Returns dVector4.ragg swizzling (equivalent to dVector4.xwyy).
        /// </summary>
        public dVector4 ragg => new dVector4(x, w, y, y);
        
        /// <summary>
        /// Returns dVector4.xwyz swizzling.
        /// </summary>
        public dVector4 xwyz => new dVector4(x, w, y, z);
        
        /// <summary>
        /// Returns dVector4.ragb swizzling (equivalent to dVector4.xwyz).
        /// </summary>
        public dVector4 ragb => new dVector4(x, w, y, z);
        
        /// <summary>
        /// Returns dVector4.xwyw swizzling.
        /// </summary>
        public dVector4 xwyw => new dVector4(x, w, y, w);
        
        /// <summary>
        /// Returns dVector4.raga swizzling (equivalent to dVector4.xwyw).
        /// </summary>
        public dVector4 raga => new dVector4(x, w, y, w);
        
        /// <summary>
        /// Returns dVector4.xwz swizzling.
        /// </summary>
        public dVector3 xwz => new dVector3(x, w, z);
        
        /// <summary>
        /// Returns dVector4.rab swizzling (equivalent to dVector4.xwz).
        /// </summary>
        public dVector3 rab => new dVector3(x, w, z);
        
        /// <summary>
        /// Returns dVector4.xwzx swizzling.
        /// </summary>
        public dVector4 xwzx => new dVector4(x, w, z, x);
        
        /// <summary>
        /// Returns dVector4.rabr swizzling (equivalent to dVector4.xwzx).
        /// </summary>
        public dVector4 rabr => new dVector4(x, w, z, x);
        
        /// <summary>
        /// Returns dVector4.xwzy swizzling.
        /// </summary>
        public dVector4 xwzy => new dVector4(x, w, z, y);
        
        /// <summary>
        /// Returns dVector4.rabg swizzling (equivalent to dVector4.xwzy).
        /// </summary>
        public dVector4 rabg => new dVector4(x, w, z, y);
        
        /// <summary>
        /// Returns dVector4.xwzz swizzling.
        /// </summary>
        public dVector4 xwzz => new dVector4(x, w, z, z);
        
        /// <summary>
        /// Returns dVector4.rabb swizzling (equivalent to dVector4.xwzz).
        /// </summary>
        public dVector4 rabb => new dVector4(x, w, z, z);
        
        /// <summary>
        /// Returns dVector4.xwzw swizzling.
        /// </summary>
        public dVector4 xwzw => new dVector4(x, w, z, w);
        
        /// <summary>
        /// Returns dVector4.raba swizzling (equivalent to dVector4.xwzw).
        /// </summary>
        public dVector4 raba => new dVector4(x, w, z, w);
        
        /// <summary>
        /// Returns dVector4.xww swizzling.
        /// </summary>
        public dVector3 xww => new dVector3(x, w, w);
        
        /// <summary>
        /// Returns dVector4.raa swizzling (equivalent to dVector4.xww).
        /// </summary>
        public dVector3 raa => new dVector3(x, w, w);
        
        /// <summary>
        /// Returns dVector4.xwwx swizzling.
        /// </summary>
        public dVector4 xwwx => new dVector4(x, w, w, x);
        
        /// <summary>
        /// Returns dVector4.raar swizzling (equivalent to dVector4.xwwx).
        /// </summary>
        public dVector4 raar => new dVector4(x, w, w, x);
        
        /// <summary>
        /// Returns dVector4.xwwy swizzling.
        /// </summary>
        public dVector4 xwwy => new dVector4(x, w, w, y);
        
        /// <summary>
        /// Returns dVector4.raag swizzling (equivalent to dVector4.xwwy).
        /// </summary>
        public dVector4 raag => new dVector4(x, w, w, y);
        
        /// <summary>
        /// Returns dVector4.xwwz swizzling.
        /// </summary>
        public dVector4 xwwz => new dVector4(x, w, w, z);
        
        /// <summary>
        /// Returns dVector4.raab swizzling (equivalent to dVector4.xwwz).
        /// </summary>
        public dVector4 raab => new dVector4(x, w, w, z);
        
        /// <summary>
        /// Returns dVector4.xwww swizzling.
        /// </summary>
        public dVector4 xwww => new dVector4(x, w, w, w);
        
        /// <summary>
        /// Returns dVector4.raaa swizzling (equivalent to dVector4.xwww).
        /// </summary>
        public dVector4 raaa => new dVector4(x, w, w, w);
        
        /// <summary>
        /// Returns dVector4.yx swizzling.
        /// </summary>
        public dVector2 yx => new dVector2(y, x);
        
        /// <summary>
        /// Returns dVector4.gr swizzling (equivalent to dVector4.yx).
        /// </summary>
        public dVector2 gr => new dVector2(y, x);
        
        /// <summary>
        /// Returns dVector4.yxx swizzling.
        /// </summary>
        public dVector3 yxx => new dVector3(y, x, x);
        
        /// <summary>
        /// Returns dVector4.grr swizzling (equivalent to dVector4.yxx).
        /// </summary>
        public dVector3 grr => new dVector3(y, x, x);
        
        /// <summary>
        /// Returns dVector4.yxxx swizzling.
        /// </summary>
        public dVector4 yxxx => new dVector4(y, x, x, x);
        
        /// <summary>
        /// Returns dVector4.grrr swizzling (equivalent to dVector4.yxxx).
        /// </summary>
        public dVector4 grrr => new dVector4(y, x, x, x);
        
        /// <summary>
        /// Returns dVector4.yxxy swizzling.
        /// </summary>
        public dVector4 yxxy => new dVector4(y, x, x, y);
        
        /// <summary>
        /// Returns dVector4.grrg swizzling (equivalent to dVector4.yxxy).
        /// </summary>
        public dVector4 grrg => new dVector4(y, x, x, y);
        
        /// <summary>
        /// Returns dVector4.yxxz swizzling.
        /// </summary>
        public dVector4 yxxz => new dVector4(y, x, x, z);
        
        /// <summary>
        /// Returns dVector4.grrb swizzling (equivalent to dVector4.yxxz).
        /// </summary>
        public dVector4 grrb => new dVector4(y, x, x, z);
        
        /// <summary>
        /// Returns dVector4.yxxw swizzling.
        /// </summary>
        public dVector4 yxxw => new dVector4(y, x, x, w);
        
        /// <summary>
        /// Returns dVector4.grra swizzling (equivalent to dVector4.yxxw).
        /// </summary>
        public dVector4 grra => new dVector4(y, x, x, w);
        
        /// <summary>
        /// Returns dVector4.yxy swizzling.
        /// </summary>
        public dVector3 yxy => new dVector3(y, x, y);
        
        /// <summary>
        /// Returns dVector4.grg swizzling (equivalent to dVector4.yxy).
        /// </summary>
        public dVector3 grg => new dVector3(y, x, y);
        
        /// <summary>
        /// Returns dVector4.yxyx swizzling.
        /// </summary>
        public dVector4 yxyx => new dVector4(y, x, y, x);
        
        /// <summary>
        /// Returns dVector4.grgr swizzling (equivalent to dVector4.yxyx).
        /// </summary>
        public dVector4 grgr => new dVector4(y, x, y, x);
        
        /// <summary>
        /// Returns dVector4.yxyy swizzling.
        /// </summary>
        public dVector4 yxyy => new dVector4(y, x, y, y);
        
        /// <summary>
        /// Returns dVector4.grgg swizzling (equivalent to dVector4.yxyy).
        /// </summary>
        public dVector4 grgg => new dVector4(y, x, y, y);
        
        /// <summary>
        /// Returns dVector4.yxyz swizzling.
        /// </summary>
        public dVector4 yxyz => new dVector4(y, x, y, z);
        
        /// <summary>
        /// Returns dVector4.grgb swizzling (equivalent to dVector4.yxyz).
        /// </summary>
        public dVector4 grgb => new dVector4(y, x, y, z);
        
        /// <summary>
        /// Returns dVector4.yxyw swizzling.
        /// </summary>
        public dVector4 yxyw => new dVector4(y, x, y, w);
        
        /// <summary>
        /// Returns dVector4.grga swizzling (equivalent to dVector4.yxyw).
        /// </summary>
        public dVector4 grga => new dVector4(y, x, y, w);
        
        /// <summary>
        /// Returns dVector4.yxz swizzling.
        /// </summary>
        public dVector3 yxz => new dVector3(y, x, z);
        
        /// <summary>
        /// Returns dVector4.grb swizzling (equivalent to dVector4.yxz).
        /// </summary>
        public dVector3 grb => new dVector3(y, x, z);
        
        /// <summary>
        /// Returns dVector4.yxzx swizzling.
        /// </summary>
        public dVector4 yxzx => new dVector4(y, x, z, x);
        
        /// <summary>
        /// Returns dVector4.grbr swizzling (equivalent to dVector4.yxzx).
        /// </summary>
        public dVector4 grbr => new dVector4(y, x, z, x);
        
        /// <summary>
        /// Returns dVector4.yxzy swizzling.
        /// </summary>
        public dVector4 yxzy => new dVector4(y, x, z, y);
        
        /// <summary>
        /// Returns dVector4.grbg swizzling (equivalent to dVector4.yxzy).
        /// </summary>
        public dVector4 grbg => new dVector4(y, x, z, y);
        
        /// <summary>
        /// Returns dVector4.yxzz swizzling.
        /// </summary>
        public dVector4 yxzz => new dVector4(y, x, z, z);
        
        /// <summary>
        /// Returns dVector4.grbb swizzling (equivalent to dVector4.yxzz).
        /// </summary>
        public dVector4 grbb => new dVector4(y, x, z, z);
        
        /// <summary>
        /// Returns dVector4.yxzw swizzling.
        /// </summary>
        public dVector4 yxzw => new dVector4(y, x, z, w);
        
        /// <summary>
        /// Returns dVector4.grba swizzling (equivalent to dVector4.yxzw).
        /// </summary>
        public dVector4 grba => new dVector4(y, x, z, w);
        
        /// <summary>
        /// Returns dVector4.yxw swizzling.
        /// </summary>
        public dVector3 yxw => new dVector3(y, x, w);
        
        /// <summary>
        /// Returns dVector4.gra swizzling (equivalent to dVector4.yxw).
        /// </summary>
        public dVector3 gra => new dVector3(y, x, w);
        
        /// <summary>
        /// Returns dVector4.yxwx swizzling.
        /// </summary>
        public dVector4 yxwx => new dVector4(y, x, w, x);
        
        /// <summary>
        /// Returns dVector4.grar swizzling (equivalent to dVector4.yxwx).
        /// </summary>
        public dVector4 grar => new dVector4(y, x, w, x);
        
        /// <summary>
        /// Returns dVector4.yxwy swizzling.
        /// </summary>
        public dVector4 yxwy => new dVector4(y, x, w, y);
        
        /// <summary>
        /// Returns dVector4.grag swizzling (equivalent to dVector4.yxwy).
        /// </summary>
        public dVector4 grag => new dVector4(y, x, w, y);
        
        /// <summary>
        /// Returns dVector4.yxwz swizzling.
        /// </summary>
        public dVector4 yxwz => new dVector4(y, x, w, z);
        
        /// <summary>
        /// Returns dVector4.grab swizzling (equivalent to dVector4.yxwz).
        /// </summary>
        public dVector4 grab => new dVector4(y, x, w, z);
        
        /// <summary>
        /// Returns dVector4.yxww swizzling.
        /// </summary>
        public dVector4 yxww => new dVector4(y, x, w, w);
        
        /// <summary>
        /// Returns dVector4.graa swizzling (equivalent to dVector4.yxww).
        /// </summary>
        public dVector4 graa => new dVector4(y, x, w, w);
        
        /// <summary>
        /// Returns dVector4.yy swizzling.
        /// </summary>
        public dVector2 yy => new dVector2(y, y);
        
        /// <summary>
        /// Returns dVector4.gg swizzling (equivalent to dVector4.yy).
        /// </summary>
        public dVector2 gg => new dVector2(y, y);
        
        /// <summary>
        /// Returns dVector4.yyx swizzling.
        /// </summary>
        public dVector3 yyx => new dVector3(y, y, x);
        
        /// <summary>
        /// Returns dVector4.ggr swizzling (equivalent to dVector4.yyx).
        /// </summary>
        public dVector3 ggr => new dVector3(y, y, x);
        
        /// <summary>
        /// Returns dVector4.yyxx swizzling.
        /// </summary>
        public dVector4 yyxx => new dVector4(y, y, x, x);
        
        /// <summary>
        /// Returns dVector4.ggrr swizzling (equivalent to dVector4.yyxx).
        /// </summary>
        public dVector4 ggrr => new dVector4(y, y, x, x);
        
        /// <summary>
        /// Returns dVector4.yyxy swizzling.
        /// </summary>
        public dVector4 yyxy => new dVector4(y, y, x, y);
        
        /// <summary>
        /// Returns dVector4.ggrg swizzling (equivalent to dVector4.yyxy).
        /// </summary>
        public dVector4 ggrg => new dVector4(y, y, x, y);
        
        /// <summary>
        /// Returns dVector4.yyxz swizzling.
        /// </summary>
        public dVector4 yyxz => new dVector4(y, y, x, z);
        
        /// <summary>
        /// Returns dVector4.ggrb swizzling (equivalent to dVector4.yyxz).
        /// </summary>
        public dVector4 ggrb => new dVector4(y, y, x, z);
        
        /// <summary>
        /// Returns dVector4.yyxw swizzling.
        /// </summary>
        public dVector4 yyxw => new dVector4(y, y, x, w);
        
        /// <summary>
        /// Returns dVector4.ggra swizzling (equivalent to dVector4.yyxw).
        /// </summary>
        public dVector4 ggra => new dVector4(y, y, x, w);
        
        /// <summary>
        /// Returns dVector4.yyy swizzling.
        /// </summary>
        public dVector3 yyy => new dVector3(y, y, y);
        
        /// <summary>
        /// Returns dVector4.ggg swizzling (equivalent to dVector4.yyy).
        /// </summary>
        public dVector3 ggg => new dVector3(y, y, y);
        
        /// <summary>
        /// Returns dVector4.yyyx swizzling.
        /// </summary>
        public dVector4 yyyx => new dVector4(y, y, y, x);
        
        /// <summary>
        /// Returns dVector4.gggr swizzling (equivalent to dVector4.yyyx).
        /// </summary>
        public dVector4 gggr => new dVector4(y, y, y, x);
        
        /// <summary>
        /// Returns dVector4.yyyy swizzling.
        /// </summary>
        public dVector4 yyyy => new dVector4(y, y, y, y);
        
        /// <summary>
        /// Returns dVector4.gggg swizzling (equivalent to dVector4.yyyy).
        /// </summary>
        public dVector4 gggg => new dVector4(y, y, y, y);
        
        /// <summary>
        /// Returns dVector4.yyyz swizzling.
        /// </summary>
        public dVector4 yyyz => new dVector4(y, y, y, z);
        
        /// <summary>
        /// Returns dVector4.gggb swizzling (equivalent to dVector4.yyyz).
        /// </summary>
        public dVector4 gggb => new dVector4(y, y, y, z);
        
        /// <summary>
        /// Returns dVector4.yyyw swizzling.
        /// </summary>
        public dVector4 yyyw => new dVector4(y, y, y, w);
        
        /// <summary>
        /// Returns dVector4.ggga swizzling (equivalent to dVector4.yyyw).
        /// </summary>
        public dVector4 ggga => new dVector4(y, y, y, w);
        
        /// <summary>
        /// Returns dVector4.yyz swizzling.
        /// </summary>
        public dVector3 yyz => new dVector3(y, y, z);
        
        /// <summary>
        /// Returns dVector4.ggb swizzling (equivalent to dVector4.yyz).
        /// </summary>
        public dVector3 ggb => new dVector3(y, y, z);
        
        /// <summary>
        /// Returns dVector4.yyzx swizzling.
        /// </summary>
        public dVector4 yyzx => new dVector4(y, y, z, x);
        
        /// <summary>
        /// Returns dVector4.ggbr swizzling (equivalent to dVector4.yyzx).
        /// </summary>
        public dVector4 ggbr => new dVector4(y, y, z, x);
        
        /// <summary>
        /// Returns dVector4.yyzy swizzling.
        /// </summary>
        public dVector4 yyzy => new dVector4(y, y, z, y);
        
        /// <summary>
        /// Returns dVector4.ggbg swizzling (equivalent to dVector4.yyzy).
        /// </summary>
        public dVector4 ggbg => new dVector4(y, y, z, y);
        
        /// <summary>
        /// Returns dVector4.yyzz swizzling.
        /// </summary>
        public dVector4 yyzz => new dVector4(y, y, z, z);
        
        /// <summary>
        /// Returns dVector4.ggbb swizzling (equivalent to dVector4.yyzz).
        /// </summary>
        public dVector4 ggbb => new dVector4(y, y, z, z);
        
        /// <summary>
        /// Returns dVector4.yyzw swizzling.
        /// </summary>
        public dVector4 yyzw => new dVector4(y, y, z, w);
        
        /// <summary>
        /// Returns dVector4.ggba swizzling (equivalent to dVector4.yyzw).
        /// </summary>
        public dVector4 ggba => new dVector4(y, y, z, w);
        
        /// <summary>
        /// Returns dVector4.yyw swizzling.
        /// </summary>
        public dVector3 yyw => new dVector3(y, y, w);
        
        /// <summary>
        /// Returns dVector4.gga swizzling (equivalent to dVector4.yyw).
        /// </summary>
        public dVector3 gga => new dVector3(y, y, w);
        
        /// <summary>
        /// Returns dVector4.yywx swizzling.
        /// </summary>
        public dVector4 yywx => new dVector4(y, y, w, x);
        
        /// <summary>
        /// Returns dVector4.ggar swizzling (equivalent to dVector4.yywx).
        /// </summary>
        public dVector4 ggar => new dVector4(y, y, w, x);
        
        /// <summary>
        /// Returns dVector4.yywy swizzling.
        /// </summary>
        public dVector4 yywy => new dVector4(y, y, w, y);
        
        /// <summary>
        /// Returns dVector4.ggag swizzling (equivalent to dVector4.yywy).
        /// </summary>
        public dVector4 ggag => new dVector4(y, y, w, y);
        
        /// <summary>
        /// Returns dVector4.yywz swizzling.
        /// </summary>
        public dVector4 yywz => new dVector4(y, y, w, z);
        
        /// <summary>
        /// Returns dVector4.ggab swizzling (equivalent to dVector4.yywz).
        /// </summary>
        public dVector4 ggab => new dVector4(y, y, w, z);
        
        /// <summary>
        /// Returns dVector4.yyww swizzling.
        /// </summary>
        public dVector4 yyww => new dVector4(y, y, w, w);
        
        /// <summary>
        /// Returns dVector4.ggaa swizzling (equivalent to dVector4.yyww).
        /// </summary>
        public dVector4 ggaa => new dVector4(y, y, w, w);
        
        /// <summary>
        /// Returns dVector4.yz swizzling.
        /// </summary>
        public dVector2 yz => new dVector2(y, z);
        
        /// <summary>
        /// Returns dVector4.gb swizzling (equivalent to dVector4.yz).
        /// </summary>
        public dVector2 gb => new dVector2(y, z);
        
        /// <summary>
        /// Returns dVector4.yzx swizzling.
        /// </summary>
        public dVector3 yzx => new dVector3(y, z, x);
        
        /// <summary>
        /// Returns dVector4.gbr swizzling (equivalent to dVector4.yzx).
        /// </summary>
        public dVector3 gbr => new dVector3(y, z, x);
        
        /// <summary>
        /// Returns dVector4.yzxx swizzling.
        /// </summary>
        public dVector4 yzxx => new dVector4(y, z, x, x);
        
        /// <summary>
        /// Returns dVector4.gbrr swizzling (equivalent to dVector4.yzxx).
        /// </summary>
        public dVector4 gbrr => new dVector4(y, z, x, x);
        
        /// <summary>
        /// Returns dVector4.yzxy swizzling.
        /// </summary>
        public dVector4 yzxy => new dVector4(y, z, x, y);
        
        /// <summary>
        /// Returns dVector4.gbrg swizzling (equivalent to dVector4.yzxy).
        /// </summary>
        public dVector4 gbrg => new dVector4(y, z, x, y);
        
        /// <summary>
        /// Returns dVector4.yzxz swizzling.
        /// </summary>
        public dVector4 yzxz => new dVector4(y, z, x, z);
        
        /// <summary>
        /// Returns dVector4.gbrb swizzling (equivalent to dVector4.yzxz).
        /// </summary>
        public dVector4 gbrb => new dVector4(y, z, x, z);
        
        /// <summary>
        /// Returns dVector4.yzxw swizzling.
        /// </summary>
        public dVector4 yzxw => new dVector4(y, z, x, w);
        
        /// <summary>
        /// Returns dVector4.gbra swizzling (equivalent to dVector4.yzxw).
        /// </summary>
        public dVector4 gbra => new dVector4(y, z, x, w);
        
        /// <summary>
        /// Returns dVector4.yzy swizzling.
        /// </summary>
        public dVector3 yzy => new dVector3(y, z, y);
        
        /// <summary>
        /// Returns dVector4.gbg swizzling (equivalent to dVector4.yzy).
        /// </summary>
        public dVector3 gbg => new dVector3(y, z, y);
        
        /// <summary>
        /// Returns dVector4.yzyx swizzling.
        /// </summary>
        public dVector4 yzyx => new dVector4(y, z, y, x);
        
        /// <summary>
        /// Returns dVector4.gbgr swizzling (equivalent to dVector4.yzyx).
        /// </summary>
        public dVector4 gbgr => new dVector4(y, z, y, x);
        
        /// <summary>
        /// Returns dVector4.yzyy swizzling.
        /// </summary>
        public dVector4 yzyy => new dVector4(y, z, y, y);
        
        /// <summary>
        /// Returns dVector4.gbgg swizzling (equivalent to dVector4.yzyy).
        /// </summary>
        public dVector4 gbgg => new dVector4(y, z, y, y);
        
        /// <summary>
        /// Returns dVector4.yzyz swizzling.
        /// </summary>
        public dVector4 yzyz => new dVector4(y, z, y, z);
        
        /// <summary>
        /// Returns dVector4.gbgb swizzling (equivalent to dVector4.yzyz).
        /// </summary>
        public dVector4 gbgb => new dVector4(y, z, y, z);
        
        /// <summary>
        /// Returns dVector4.yzyw swizzling.
        /// </summary>
        public dVector4 yzyw => new dVector4(y, z, y, w);
        
        /// <summary>
        /// Returns dVector4.gbga swizzling (equivalent to dVector4.yzyw).
        /// </summary>
        public dVector4 gbga => new dVector4(y, z, y, w);
        
        /// <summary>
        /// Returns dVector4.yzz swizzling.
        /// </summary>
        public dVector3 yzz => new dVector3(y, z, z);
        
        /// <summary>
        /// Returns dVector4.gbb swizzling (equivalent to dVector4.yzz).
        /// </summary>
        public dVector3 gbb => new dVector3(y, z, z);
        
        /// <summary>
        /// Returns dVector4.yzzx swizzling.
        /// </summary>
        public dVector4 yzzx => new dVector4(y, z, z, x);
        
        /// <summary>
        /// Returns dVector4.gbbr swizzling (equivalent to dVector4.yzzx).
        /// </summary>
        public dVector4 gbbr => new dVector4(y, z, z, x);
        
        /// <summary>
        /// Returns dVector4.yzzy swizzling.
        /// </summary>
        public dVector4 yzzy => new dVector4(y, z, z, y);
        
        /// <summary>
        /// Returns dVector4.gbbg swizzling (equivalent to dVector4.yzzy).
        /// </summary>
        public dVector4 gbbg => new dVector4(y, z, z, y);
        
        /// <summary>
        /// Returns dVector4.yzzz swizzling.
        /// </summary>
        public dVector4 yzzz => new dVector4(y, z, z, z);
        
        /// <summary>
        /// Returns dVector4.gbbb swizzling (equivalent to dVector4.yzzz).
        /// </summary>
        public dVector4 gbbb => new dVector4(y, z, z, z);
        
        /// <summary>
        /// Returns dVector4.yzzw swizzling.
        /// </summary>
        public dVector4 yzzw => new dVector4(y, z, z, w);
        
        /// <summary>
        /// Returns dVector4.gbba swizzling (equivalent to dVector4.yzzw).
        /// </summary>
        public dVector4 gbba => new dVector4(y, z, z, w);
        
        /// <summary>
        /// Returns dVector4.yzw swizzling.
        /// </summary>
        public dVector3 yzw => new dVector3(y, z, w);
        
        /// <summary>
        /// Returns dVector4.gba swizzling (equivalent to dVector4.yzw).
        /// </summary>
        public dVector3 gba => new dVector3(y, z, w);
        
        /// <summary>
        /// Returns dVector4.yzwx swizzling.
        /// </summary>
        public dVector4 yzwx => new dVector4(y, z, w, x);
        
        /// <summary>
        /// Returns dVector4.gbar swizzling (equivalent to dVector4.yzwx).
        /// </summary>
        public dVector4 gbar => new dVector4(y, z, w, x);
        
        /// <summary>
        /// Returns dVector4.yzwy swizzling.
        /// </summary>
        public dVector4 yzwy => new dVector4(y, z, w, y);
        
        /// <summary>
        /// Returns dVector4.gbag swizzling (equivalent to dVector4.yzwy).
        /// </summary>
        public dVector4 gbag => new dVector4(y, z, w, y);
        
        /// <summary>
        /// Returns dVector4.yzwz swizzling.
        /// </summary>
        public dVector4 yzwz => new dVector4(y, z, w, z);
        
        /// <summary>
        /// Returns dVector4.gbab swizzling (equivalent to dVector4.yzwz).
        /// </summary>
        public dVector4 gbab => new dVector4(y, z, w, z);
        
        /// <summary>
        /// Returns dVector4.yzww swizzling.
        /// </summary>
        public dVector4 yzww => new dVector4(y, z, w, w);
        
        /// <summary>
        /// Returns dVector4.gbaa swizzling (equivalent to dVector4.yzww).
        /// </summary>
        public dVector4 gbaa => new dVector4(y, z, w, w);
        
        /// <summary>
        /// Returns dVector4.yw swizzling.
        /// </summary>
        public dVector2 yw => new dVector2(y, w);
        
        /// <summary>
        /// Returns dVector4.ga swizzling (equivalent to dVector4.yw).
        /// </summary>
        public dVector2 ga => new dVector2(y, w);
        
        /// <summary>
        /// Returns dVector4.ywx swizzling.
        /// </summary>
        public dVector3 ywx => new dVector3(y, w, x);
        
        /// <summary>
        /// Returns dVector4.gar swizzling (equivalent to dVector4.ywx).
        /// </summary>
        public dVector3 gar => new dVector3(y, w, x);
        
        /// <summary>
        /// Returns dVector4.ywxx swizzling.
        /// </summary>
        public dVector4 ywxx => new dVector4(y, w, x, x);
        
        /// <summary>
        /// Returns dVector4.garr swizzling (equivalent to dVector4.ywxx).
        /// </summary>
        public dVector4 garr => new dVector4(y, w, x, x);
        
        /// <summary>
        /// Returns dVector4.ywxy swizzling.
        /// </summary>
        public dVector4 ywxy => new dVector4(y, w, x, y);
        
        /// <summary>
        /// Returns dVector4.garg swizzling (equivalent to dVector4.ywxy).
        /// </summary>
        public dVector4 garg => new dVector4(y, w, x, y);
        
        /// <summary>
        /// Returns dVector4.ywxz swizzling.
        /// </summary>
        public dVector4 ywxz => new dVector4(y, w, x, z);
        
        /// <summary>
        /// Returns dVector4.garb swizzling (equivalent to dVector4.ywxz).
        /// </summary>
        public dVector4 garb => new dVector4(y, w, x, z);
        
        /// <summary>
        /// Returns dVector4.ywxw swizzling.
        /// </summary>
        public dVector4 ywxw => new dVector4(y, w, x, w);
        
        /// <summary>
        /// Returns dVector4.gara swizzling (equivalent to dVector4.ywxw).
        /// </summary>
        public dVector4 gara => new dVector4(y, w, x, w);
        
        /// <summary>
        /// Returns dVector4.ywy swizzling.
        /// </summary>
        public dVector3 ywy => new dVector3(y, w, y);
        
        /// <summary>
        /// Returns dVector4.gag swizzling (equivalent to dVector4.ywy).
        /// </summary>
        public dVector3 gag => new dVector3(y, w, y);
        
        /// <summary>
        /// Returns dVector4.ywyx swizzling.
        /// </summary>
        public dVector4 ywyx => new dVector4(y, w, y, x);
        
        /// <summary>
        /// Returns dVector4.gagr swizzling (equivalent to dVector4.ywyx).
        /// </summary>
        public dVector4 gagr => new dVector4(y, w, y, x);
        
        /// <summary>
        /// Returns dVector4.ywyy swizzling.
        /// </summary>
        public dVector4 ywyy => new dVector4(y, w, y, y);
        
        /// <summary>
        /// Returns dVector4.gagg swizzling (equivalent to dVector4.ywyy).
        /// </summary>
        public dVector4 gagg => new dVector4(y, w, y, y);
        
        /// <summary>
        /// Returns dVector4.ywyz swizzling.
        /// </summary>
        public dVector4 ywyz => new dVector4(y, w, y, z);
        
        /// <summary>
        /// Returns dVector4.gagb swizzling (equivalent to dVector4.ywyz).
        /// </summary>
        public dVector4 gagb => new dVector4(y, w, y, z);
        
        /// <summary>
        /// Returns dVector4.ywyw swizzling.
        /// </summary>
        public dVector4 ywyw => new dVector4(y, w, y, w);
        
        /// <summary>
        /// Returns dVector4.gaga swizzling (equivalent to dVector4.ywyw).
        /// </summary>
        public dVector4 gaga => new dVector4(y, w, y, w);
        
        /// <summary>
        /// Returns dVector4.ywz swizzling.
        /// </summary>
        public dVector3 ywz => new dVector3(y, w, z);
        
        /// <summary>
        /// Returns dVector4.gab swizzling (equivalent to dVector4.ywz).
        /// </summary>
        public dVector3 gab => new dVector3(y, w, z);
        
        /// <summary>
        /// Returns dVector4.ywzx swizzling.
        /// </summary>
        public dVector4 ywzx => new dVector4(y, w, z, x);
        
        /// <summary>
        /// Returns dVector4.gabr swizzling (equivalent to dVector4.ywzx).
        /// </summary>
        public dVector4 gabr => new dVector4(y, w, z, x);
        
        /// <summary>
        /// Returns dVector4.ywzy swizzling.
        /// </summary>
        public dVector4 ywzy => new dVector4(y, w, z, y);
        
        /// <summary>
        /// Returns dVector4.gabg swizzling (equivalent to dVector4.ywzy).
        /// </summary>
        public dVector4 gabg => new dVector4(y, w, z, y);
        
        /// <summary>
        /// Returns dVector4.ywzz swizzling.
        /// </summary>
        public dVector4 ywzz => new dVector4(y, w, z, z);
        
        /// <summary>
        /// Returns dVector4.gabb swizzling (equivalent to dVector4.ywzz).
        /// </summary>
        public dVector4 gabb => new dVector4(y, w, z, z);
        
        /// <summary>
        /// Returns dVector4.ywzw swizzling.
        /// </summary>
        public dVector4 ywzw => new dVector4(y, w, z, w);
        
        /// <summary>
        /// Returns dVector4.gaba swizzling (equivalent to dVector4.ywzw).
        /// </summary>
        public dVector4 gaba => new dVector4(y, w, z, w);
        
        /// <summary>
        /// Returns dVector4.yww swizzling.
        /// </summary>
        public dVector3 yww => new dVector3(y, w, w);
        
        /// <summary>
        /// Returns dVector4.gaa swizzling (equivalent to dVector4.yww).
        /// </summary>
        public dVector3 gaa => new dVector3(y, w, w);
        
        /// <summary>
        /// Returns dVector4.ywwx swizzling.
        /// </summary>
        public dVector4 ywwx => new dVector4(y, w, w, x);
        
        /// <summary>
        /// Returns dVector4.gaar swizzling (equivalent to dVector4.ywwx).
        /// </summary>
        public dVector4 gaar => new dVector4(y, w, w, x);
        
        /// <summary>
        /// Returns dVector4.ywwy swizzling.
        /// </summary>
        public dVector4 ywwy => new dVector4(y, w, w, y);
        
        /// <summary>
        /// Returns dVector4.gaag swizzling (equivalent to dVector4.ywwy).
        /// </summary>
        public dVector4 gaag => new dVector4(y, w, w, y);
        
        /// <summary>
        /// Returns dVector4.ywwz swizzling.
        /// </summary>
        public dVector4 ywwz => new dVector4(y, w, w, z);
        
        /// <summary>
        /// Returns dVector4.gaab swizzling (equivalent to dVector4.ywwz).
        /// </summary>
        public dVector4 gaab => new dVector4(y, w, w, z);
        
        /// <summary>
        /// Returns dVector4.ywww swizzling.
        /// </summary>
        public dVector4 ywww => new dVector4(y, w, w, w);
        
        /// <summary>
        /// Returns dVector4.gaaa swizzling (equivalent to dVector4.ywww).
        /// </summary>
        public dVector4 gaaa => new dVector4(y, w, w, w);
        
        /// <summary>
        /// Returns dVector4.zx swizzling.
        /// </summary>
        public dVector2 zx => new dVector2(z, x);
        
        /// <summary>
        /// Returns dVector4.br swizzling (equivalent to dVector4.zx).
        /// </summary>
        public dVector2 br => new dVector2(z, x);
        
        /// <summary>
        /// Returns dVector4.zxx swizzling.
        /// </summary>
        public dVector3 zxx => new dVector3(z, x, x);
        
        /// <summary>
        /// Returns dVector4.brr swizzling (equivalent to dVector4.zxx).
        /// </summary>
        public dVector3 brr => new dVector3(z, x, x);
        
        /// <summary>
        /// Returns dVector4.zxxx swizzling.
        /// </summary>
        public dVector4 zxxx => new dVector4(z, x, x, x);
        
        /// <summary>
        /// Returns dVector4.brrr swizzling (equivalent to dVector4.zxxx).
        /// </summary>
        public dVector4 brrr => new dVector4(z, x, x, x);
        
        /// <summary>
        /// Returns dVector4.zxxy swizzling.
        /// </summary>
        public dVector4 zxxy => new dVector4(z, x, x, y);
        
        /// <summary>
        /// Returns dVector4.brrg swizzling (equivalent to dVector4.zxxy).
        /// </summary>
        public dVector4 brrg => new dVector4(z, x, x, y);
        
        /// <summary>
        /// Returns dVector4.zxxz swizzling.
        /// </summary>
        public dVector4 zxxz => new dVector4(z, x, x, z);
        
        /// <summary>
        /// Returns dVector4.brrb swizzling (equivalent to dVector4.zxxz).
        /// </summary>
        public dVector4 brrb => new dVector4(z, x, x, z);
        
        /// <summary>
        /// Returns dVector4.zxxw swizzling.
        /// </summary>
        public dVector4 zxxw => new dVector4(z, x, x, w);
        
        /// <summary>
        /// Returns dVector4.brra swizzling (equivalent to dVector4.zxxw).
        /// </summary>
        public dVector4 brra => new dVector4(z, x, x, w);
        
        /// <summary>
        /// Returns dVector4.zxy swizzling.
        /// </summary>
        public dVector3 zxy => new dVector3(z, x, y);
        
        /// <summary>
        /// Returns dVector4.brg swizzling (equivalent to dVector4.zxy).
        /// </summary>
        public dVector3 brg => new dVector3(z, x, y);
        
        /// <summary>
        /// Returns dVector4.zxyx swizzling.
        /// </summary>
        public dVector4 zxyx => new dVector4(z, x, y, x);
        
        /// <summary>
        /// Returns dVector4.brgr swizzling (equivalent to dVector4.zxyx).
        /// </summary>
        public dVector4 brgr => new dVector4(z, x, y, x);
        
        /// <summary>
        /// Returns dVector4.zxyy swizzling.
        /// </summary>
        public dVector4 zxyy => new dVector4(z, x, y, y);
        
        /// <summary>
        /// Returns dVector4.brgg swizzling (equivalent to dVector4.zxyy).
        /// </summary>
        public dVector4 brgg => new dVector4(z, x, y, y);
        
        /// <summary>
        /// Returns dVector4.zxyz swizzling.
        /// </summary>
        public dVector4 zxyz => new dVector4(z, x, y, z);
        
        /// <summary>
        /// Returns dVector4.brgb swizzling (equivalent to dVector4.zxyz).
        /// </summary>
        public dVector4 brgb => new dVector4(z, x, y, z);
        
        /// <summary>
        /// Returns dVector4.zxyw swizzling.
        /// </summary>
        public dVector4 zxyw => new dVector4(z, x, y, w);
        
        /// <summary>
        /// Returns dVector4.brga swizzling (equivalent to dVector4.zxyw).
        /// </summary>
        public dVector4 brga => new dVector4(z, x, y, w);
        
        /// <summary>
        /// Returns dVector4.zxz swizzling.
        /// </summary>
        public dVector3 zxz => new dVector3(z, x, z);
        
        /// <summary>
        /// Returns dVector4.brb swizzling (equivalent to dVector4.zxz).
        /// </summary>
        public dVector3 brb => new dVector3(z, x, z);
        
        /// <summary>
        /// Returns dVector4.zxzx swizzling.
        /// </summary>
        public dVector4 zxzx => new dVector4(z, x, z, x);
        
        /// <summary>
        /// Returns dVector4.brbr swizzling (equivalent to dVector4.zxzx).
        /// </summary>
        public dVector4 brbr => new dVector4(z, x, z, x);
        
        /// <summary>
        /// Returns dVector4.zxzy swizzling.
        /// </summary>
        public dVector4 zxzy => new dVector4(z, x, z, y);
        
        /// <summary>
        /// Returns dVector4.brbg swizzling (equivalent to dVector4.zxzy).
        /// </summary>
        public dVector4 brbg => new dVector4(z, x, z, y);
        
        /// <summary>
        /// Returns dVector4.zxzz swizzling.
        /// </summary>
        public dVector4 zxzz => new dVector4(z, x, z, z);
        
        /// <summary>
        /// Returns dVector4.brbb swizzling (equivalent to dVector4.zxzz).
        /// </summary>
        public dVector4 brbb => new dVector4(z, x, z, z);
        
        /// <summary>
        /// Returns dVector4.zxzw swizzling.
        /// </summary>
        public dVector4 zxzw => new dVector4(z, x, z, w);
        
        /// <summary>
        /// Returns dVector4.brba swizzling (equivalent to dVector4.zxzw).
        /// </summary>
        public dVector4 brba => new dVector4(z, x, z, w);
        
        /// <summary>
        /// Returns dVector4.zxw swizzling.
        /// </summary>
        public dVector3 zxw => new dVector3(z, x, w);
        
        /// <summary>
        /// Returns dVector4.bra swizzling (equivalent to dVector4.zxw).
        /// </summary>
        public dVector3 bra => new dVector3(z, x, w);
        
        /// <summary>
        /// Returns dVector4.zxwx swizzling.
        /// </summary>
        public dVector4 zxwx => new dVector4(z, x, w, x);
        
        /// <summary>
        /// Returns dVector4.brar swizzling (equivalent to dVector4.zxwx).
        /// </summary>
        public dVector4 brar => new dVector4(z, x, w, x);
        
        /// <summary>
        /// Returns dVector4.zxwy swizzling.
        /// </summary>
        public dVector4 zxwy => new dVector4(z, x, w, y);
        
        /// <summary>
        /// Returns dVector4.brag swizzling (equivalent to dVector4.zxwy).
        /// </summary>
        public dVector4 brag => new dVector4(z, x, w, y);
        
        /// <summary>
        /// Returns dVector4.zxwz swizzling.
        /// </summary>
        public dVector4 zxwz => new dVector4(z, x, w, z);
        
        /// <summary>
        /// Returns dVector4.brab swizzling (equivalent to dVector4.zxwz).
        /// </summary>
        public dVector4 brab => new dVector4(z, x, w, z);
        
        /// <summary>
        /// Returns dVector4.zxww swizzling.
        /// </summary>
        public dVector4 zxww => new dVector4(z, x, w, w);
        
        /// <summary>
        /// Returns dVector4.braa swizzling (equivalent to dVector4.zxww).
        /// </summary>
        public dVector4 braa => new dVector4(z, x, w, w);
        
        /// <summary>
        /// Returns dVector4.zy swizzling.
        /// </summary>
        public dVector2 zy => new dVector2(z, y);
        
        /// <summary>
        /// Returns dVector4.bg swizzling (equivalent to dVector4.zy).
        /// </summary>
        public dVector2 bg => new dVector2(z, y);
        
        /// <summary>
        /// Returns dVector4.zyx swizzling.
        /// </summary>
        public dVector3 zyx => new dVector3(z, y, x);
        
        /// <summary>
        /// Returns dVector4.bgr swizzling (equivalent to dVector4.zyx).
        /// </summary>
        public dVector3 bgr => new dVector3(z, y, x);
        
        /// <summary>
        /// Returns dVector4.zyxx swizzling.
        /// </summary>
        public dVector4 zyxx => new dVector4(z, y, x, x);
        
        /// <summary>
        /// Returns dVector4.bgrr swizzling (equivalent to dVector4.zyxx).
        /// </summary>
        public dVector4 bgrr => new dVector4(z, y, x, x);
        
        /// <summary>
        /// Returns dVector4.zyxy swizzling.
        /// </summary>
        public dVector4 zyxy => new dVector4(z, y, x, y);
        
        /// <summary>
        /// Returns dVector4.bgrg swizzling (equivalent to dVector4.zyxy).
        /// </summary>
        public dVector4 bgrg => new dVector4(z, y, x, y);
        
        /// <summary>
        /// Returns dVector4.zyxz swizzling.
        /// </summary>
        public dVector4 zyxz => new dVector4(z, y, x, z);
        
        /// <summary>
        /// Returns dVector4.bgrb swizzling (equivalent to dVector4.zyxz).
        /// </summary>
        public dVector4 bgrb => new dVector4(z, y, x, z);
        
        /// <summary>
        /// Returns dVector4.zyxw swizzling.
        /// </summary>
        public dVector4 zyxw => new dVector4(z, y, x, w);
        
        /// <summary>
        /// Returns dVector4.bgra swizzling (equivalent to dVector4.zyxw).
        /// </summary>
        public dVector4 bgra => new dVector4(z, y, x, w);
        
        /// <summary>
        /// Returns dVector4.zyy swizzling.
        /// </summary>
        public dVector3 zyy => new dVector3(z, y, y);
        
        /// <summary>
        /// Returns dVector4.bgg swizzling (equivalent to dVector4.zyy).
        /// </summary>
        public dVector3 bgg => new dVector3(z, y, y);
        
        /// <summary>
        /// Returns dVector4.zyyx swizzling.
        /// </summary>
        public dVector4 zyyx => new dVector4(z, y, y, x);
        
        /// <summary>
        /// Returns dVector4.bggr swizzling (equivalent to dVector4.zyyx).
        /// </summary>
        public dVector4 bggr => new dVector4(z, y, y, x);
        
        /// <summary>
        /// Returns dVector4.zyyy swizzling.
        /// </summary>
        public dVector4 zyyy => new dVector4(z, y, y, y);
        
        /// <summary>
        /// Returns dVector4.bggg swizzling (equivalent to dVector4.zyyy).
        /// </summary>
        public dVector4 bggg => new dVector4(z, y, y, y);
        
        /// <summary>
        /// Returns dVector4.zyyz swizzling.
        /// </summary>
        public dVector4 zyyz => new dVector4(z, y, y, z);
        
        /// <summary>
        /// Returns dVector4.bggb swizzling (equivalent to dVector4.zyyz).
        /// </summary>
        public dVector4 bggb => new dVector4(z, y, y, z);
        
        /// <summary>
        /// Returns dVector4.zyyw swizzling.
        /// </summary>
        public dVector4 zyyw => new dVector4(z, y, y, w);
        
        /// <summary>
        /// Returns dVector4.bgga swizzling (equivalent to dVector4.zyyw).
        /// </summary>
        public dVector4 bgga => new dVector4(z, y, y, w);
        
        /// <summary>
        /// Returns dVector4.zyz swizzling.
        /// </summary>
        public dVector3 zyz => new dVector3(z, y, z);
        
        /// <summary>
        /// Returns dVector4.bgb swizzling (equivalent to dVector4.zyz).
        /// </summary>
        public dVector3 bgb => new dVector3(z, y, z);
        
        /// <summary>
        /// Returns dVector4.zyzx swizzling.
        /// </summary>
        public dVector4 zyzx => new dVector4(z, y, z, x);
        
        /// <summary>
        /// Returns dVector4.bgbr swizzling (equivalent to dVector4.zyzx).
        /// </summary>
        public dVector4 bgbr => new dVector4(z, y, z, x);
        
        /// <summary>
        /// Returns dVector4.zyzy swizzling.
        /// </summary>
        public dVector4 zyzy => new dVector4(z, y, z, y);
        
        /// <summary>
        /// Returns dVector4.bgbg swizzling (equivalent to dVector4.zyzy).
        /// </summary>
        public dVector4 bgbg => new dVector4(z, y, z, y);
        
        /// <summary>
        /// Returns dVector4.zyzz swizzling.
        /// </summary>
        public dVector4 zyzz => new dVector4(z, y, z, z);
        
        /// <summary>
        /// Returns dVector4.bgbb swizzling (equivalent to dVector4.zyzz).
        /// </summary>
        public dVector4 bgbb => new dVector4(z, y, z, z);
        
        /// <summary>
        /// Returns dVector4.zyzw swizzling.
        /// </summary>
        public dVector4 zyzw => new dVector4(z, y, z, w);
        
        /// <summary>
        /// Returns dVector4.bgba swizzling (equivalent to dVector4.zyzw).
        /// </summary>
        public dVector4 bgba => new dVector4(z, y, z, w);
        
        /// <summary>
        /// Returns dVector4.zyw swizzling.
        /// </summary>
        public dVector3 zyw => new dVector3(z, y, w);
        
        /// <summary>
        /// Returns dVector4.bga swizzling (equivalent to dVector4.zyw).
        /// </summary>
        public dVector3 bga => new dVector3(z, y, w);
        
        /// <summary>
        /// Returns dVector4.zywx swizzling.
        /// </summary>
        public dVector4 zywx => new dVector4(z, y, w, x);
        
        /// <summary>
        /// Returns dVector4.bgar swizzling (equivalent to dVector4.zywx).
        /// </summary>
        public dVector4 bgar => new dVector4(z, y, w, x);
        
        /// <summary>
        /// Returns dVector4.zywy swizzling.
        /// </summary>
        public dVector4 zywy => new dVector4(z, y, w, y);
        
        /// <summary>
        /// Returns dVector4.bgag swizzling (equivalent to dVector4.zywy).
        /// </summary>
        public dVector4 bgag => new dVector4(z, y, w, y);
        
        /// <summary>
        /// Returns dVector4.zywz swizzling.
        /// </summary>
        public dVector4 zywz => new dVector4(z, y, w, z);
        
        /// <summary>
        /// Returns dVector4.bgab swizzling (equivalent to dVector4.zywz).
        /// </summary>
        public dVector4 bgab => new dVector4(z, y, w, z);
        
        /// <summary>
        /// Returns dVector4.zyww swizzling.
        /// </summary>
        public dVector4 zyww => new dVector4(z, y, w, w);
        
        /// <summary>
        /// Returns dVector4.bgaa swizzling (equivalent to dVector4.zyww).
        /// </summary>
        public dVector4 bgaa => new dVector4(z, y, w, w);
        
        /// <summary>
        /// Returns dVector4.zz swizzling.
        /// </summary>
        public dVector2 zz => new dVector2(z, z);
        
        /// <summary>
        /// Returns dVector4.bb swizzling (equivalent to dVector4.zz).
        /// </summary>
        public dVector2 bb => new dVector2(z, z);
        
        /// <summary>
        /// Returns dVector4.zzx swizzling.
        /// </summary>
        public dVector3 zzx => new dVector3(z, z, x);
        
        /// <summary>
        /// Returns dVector4.bbr swizzling (equivalent to dVector4.zzx).
        /// </summary>
        public dVector3 bbr => new dVector3(z, z, x);
        
        /// <summary>
        /// Returns dVector4.zzxx swizzling.
        /// </summary>
        public dVector4 zzxx => new dVector4(z, z, x, x);
        
        /// <summary>
        /// Returns dVector4.bbrr swizzling (equivalent to dVector4.zzxx).
        /// </summary>
        public dVector4 bbrr => new dVector4(z, z, x, x);
        
        /// <summary>
        /// Returns dVector4.zzxy swizzling.
        /// </summary>
        public dVector4 zzxy => new dVector4(z, z, x, y);
        
        /// <summary>
        /// Returns dVector4.bbrg swizzling (equivalent to dVector4.zzxy).
        /// </summary>
        public dVector4 bbrg => new dVector4(z, z, x, y);
        
        /// <summary>
        /// Returns dVector4.zzxz swizzling.
        /// </summary>
        public dVector4 zzxz => new dVector4(z, z, x, z);
        
        /// <summary>
        /// Returns dVector4.bbrb swizzling (equivalent to dVector4.zzxz).
        /// </summary>
        public dVector4 bbrb => new dVector4(z, z, x, z);
        
        /// <summary>
        /// Returns dVector4.zzxw swizzling.
        /// </summary>
        public dVector4 zzxw => new dVector4(z, z, x, w);
        
        /// <summary>
        /// Returns dVector4.bbra swizzling (equivalent to dVector4.zzxw).
        /// </summary>
        public dVector4 bbra => new dVector4(z, z, x, w);
        
        /// <summary>
        /// Returns dVector4.zzy swizzling.
        /// </summary>
        public dVector3 zzy => new dVector3(z, z, y);
        
        /// <summary>
        /// Returns dVector4.bbg swizzling (equivalent to dVector4.zzy).
        /// </summary>
        public dVector3 bbg => new dVector3(z, z, y);
        
        /// <summary>
        /// Returns dVector4.zzyx swizzling.
        /// </summary>
        public dVector4 zzyx => new dVector4(z, z, y, x);
        
        /// <summary>
        /// Returns dVector4.bbgr swizzling (equivalent to dVector4.zzyx).
        /// </summary>
        public dVector4 bbgr => new dVector4(z, z, y, x);
        
        /// <summary>
        /// Returns dVector4.zzyy swizzling.
        /// </summary>
        public dVector4 zzyy => new dVector4(z, z, y, y);
        
        /// <summary>
        /// Returns dVector4.bbgg swizzling (equivalent to dVector4.zzyy).
        /// </summary>
        public dVector4 bbgg => new dVector4(z, z, y, y);
        
        /// <summary>
        /// Returns dVector4.zzyz swizzling.
        /// </summary>
        public dVector4 zzyz => new dVector4(z, z, y, z);
        
        /// <summary>
        /// Returns dVector4.bbgb swizzling (equivalent to dVector4.zzyz).
        /// </summary>
        public dVector4 bbgb => new dVector4(z, z, y, z);
        
        /// <summary>
        /// Returns dVector4.zzyw swizzling.
        /// </summary>
        public dVector4 zzyw => new dVector4(z, z, y, w);
        
        /// <summary>
        /// Returns dVector4.bbga swizzling (equivalent to dVector4.zzyw).
        /// </summary>
        public dVector4 bbga => new dVector4(z, z, y, w);
        
        /// <summary>
        /// Returns dVector4.zzz swizzling.
        /// </summary>
        public dVector3 zzz => new dVector3(z, z, z);
        
        /// <summary>
        /// Returns dVector4.bbb swizzling (equivalent to dVector4.zzz).
        /// </summary>
        public dVector3 bbb => new dVector3(z, z, z);
        
        /// <summary>
        /// Returns dVector4.zzzx swizzling.
        /// </summary>
        public dVector4 zzzx => new dVector4(z, z, z, x);
        
        /// <summary>
        /// Returns dVector4.bbbr swizzling (equivalent to dVector4.zzzx).
        /// </summary>
        public dVector4 bbbr => new dVector4(z, z, z, x);
        
        /// <summary>
        /// Returns dVector4.zzzy swizzling.
        /// </summary>
        public dVector4 zzzy => new dVector4(z, z, z, y);
        
        /// <summary>
        /// Returns dVector4.bbbg swizzling (equivalent to dVector4.zzzy).
        /// </summary>
        public dVector4 bbbg => new dVector4(z, z, z, y);
        
        /// <summary>
        /// Returns dVector4.zzzz swizzling.
        /// </summary>
        public dVector4 zzzz => new dVector4(z, z, z, z);
        
        /// <summary>
        /// Returns dVector4.bbbb swizzling (equivalent to dVector4.zzzz).
        /// </summary>
        public dVector4 bbbb => new dVector4(z, z, z, z);
        
        /// <summary>
        /// Returns dVector4.zzzw swizzling.
        /// </summary>
        public dVector4 zzzw => new dVector4(z, z, z, w);
        
        /// <summary>
        /// Returns dVector4.bbba swizzling (equivalent to dVector4.zzzw).
        /// </summary>
        public dVector4 bbba => new dVector4(z, z, z, w);
        
        /// <summary>
        /// Returns dVector4.zzw swizzling.
        /// </summary>
        public dVector3 zzw => new dVector3(z, z, w);
        
        /// <summary>
        /// Returns dVector4.bba swizzling (equivalent to dVector4.zzw).
        /// </summary>
        public dVector3 bba => new dVector3(z, z, w);
        
        /// <summary>
        /// Returns dVector4.zzwx swizzling.
        /// </summary>
        public dVector4 zzwx => new dVector4(z, z, w, x);
        
        /// <summary>
        /// Returns dVector4.bbar swizzling (equivalent to dVector4.zzwx).
        /// </summary>
        public dVector4 bbar => new dVector4(z, z, w, x);
        
        /// <summary>
        /// Returns dVector4.zzwy swizzling.
        /// </summary>
        public dVector4 zzwy => new dVector4(z, z, w, y);
        
        /// <summary>
        /// Returns dVector4.bbag swizzling (equivalent to dVector4.zzwy).
        /// </summary>
        public dVector4 bbag => new dVector4(z, z, w, y);
        
        /// <summary>
        /// Returns dVector4.zzwz swizzling.
        /// </summary>
        public dVector4 zzwz => new dVector4(z, z, w, z);
        
        /// <summary>
        /// Returns dVector4.bbab swizzling (equivalent to dVector4.zzwz).
        /// </summary>
        public dVector4 bbab => new dVector4(z, z, w, z);
        
        /// <summary>
        /// Returns dVector4.zzww swizzling.
        /// </summary>
        public dVector4 zzww => new dVector4(z, z, w, w);
        
        /// <summary>
        /// Returns dVector4.bbaa swizzling (equivalent to dVector4.zzww).
        /// </summary>
        public dVector4 bbaa => new dVector4(z, z, w, w);
        
        /// <summary>
        /// Returns dVector4.zw swizzling.
        /// </summary>
        public dVector2 zw => new dVector2(z, w);
        
        /// <summary>
        /// Returns dVector4.ba swizzling (equivalent to dVector4.zw).
        /// </summary>
        public dVector2 ba => new dVector2(z, w);
        
        /// <summary>
        /// Returns dVector4.zwx swizzling.
        /// </summary>
        public dVector3 zwx => new dVector3(z, w, x);
        
        /// <summary>
        /// Returns dVector4.bar swizzling (equivalent to dVector4.zwx).
        /// </summary>
        public dVector3 bar => new dVector3(z, w, x);
        
        /// <summary>
        /// Returns dVector4.zwxx swizzling.
        /// </summary>
        public dVector4 zwxx => new dVector4(z, w, x, x);
        
        /// <summary>
        /// Returns dVector4.barr swizzling (equivalent to dVector4.zwxx).
        /// </summary>
        public dVector4 barr => new dVector4(z, w, x, x);
        
        /// <summary>
        /// Returns dVector4.zwxy swizzling.
        /// </summary>
        public dVector4 zwxy => new dVector4(z, w, x, y);
        
        /// <summary>
        /// Returns dVector4.barg swizzling (equivalent to dVector4.zwxy).
        /// </summary>
        public dVector4 barg => new dVector4(z, w, x, y);
        
        /// <summary>
        /// Returns dVector4.zwxz swizzling.
        /// </summary>
        public dVector4 zwxz => new dVector4(z, w, x, z);
        
        /// <summary>
        /// Returns dVector4.barb swizzling (equivalent to dVector4.zwxz).
        /// </summary>
        public dVector4 barb => new dVector4(z, w, x, z);
        
        /// <summary>
        /// Returns dVector4.zwxw swizzling.
        /// </summary>
        public dVector4 zwxw => new dVector4(z, w, x, w);
        
        /// <summary>
        /// Returns dVector4.bara swizzling (equivalent to dVector4.zwxw).
        /// </summary>
        public dVector4 bara => new dVector4(z, w, x, w);
        
        /// <summary>
        /// Returns dVector4.zwy swizzling.
        /// </summary>
        public dVector3 zwy => new dVector3(z, w, y);
        
        /// <summary>
        /// Returns dVector4.bag swizzling (equivalent to dVector4.zwy).
        /// </summary>
        public dVector3 bag => new dVector3(z, w, y);
        
        /// <summary>
        /// Returns dVector4.zwyx swizzling.
        /// </summary>
        public dVector4 zwyx => new dVector4(z, w, y, x);
        
        /// <summary>
        /// Returns dVector4.bagr swizzling (equivalent to dVector4.zwyx).
        /// </summary>
        public dVector4 bagr => new dVector4(z, w, y, x);
        
        /// <summary>
        /// Returns dVector4.zwyy swizzling.
        /// </summary>
        public dVector4 zwyy => new dVector4(z, w, y, y);
        
        /// <summary>
        /// Returns dVector4.bagg swizzling (equivalent to dVector4.zwyy).
        /// </summary>
        public dVector4 bagg => new dVector4(z, w, y, y);
        
        /// <summary>
        /// Returns dVector4.zwyz swizzling.
        /// </summary>
        public dVector4 zwyz => new dVector4(z, w, y, z);
        
        /// <summary>
        /// Returns dVector4.bagb swizzling (equivalent to dVector4.zwyz).
        /// </summary>
        public dVector4 bagb => new dVector4(z, w, y, z);
        
        /// <summary>
        /// Returns dVector4.zwyw swizzling.
        /// </summary>
        public dVector4 zwyw => new dVector4(z, w, y, w);
        
        /// <summary>
        /// Returns dVector4.baga swizzling (equivalent to dVector4.zwyw).
        /// </summary>
        public dVector4 baga => new dVector4(z, w, y, w);
        
        /// <summary>
        /// Returns dVector4.zwz swizzling.
        /// </summary>
        public dVector3 zwz => new dVector3(z, w, z);
        
        /// <summary>
        /// Returns dVector4.bab swizzling (equivalent to dVector4.zwz).
        /// </summary>
        public dVector3 bab => new dVector3(z, w, z);
        
        /// <summary>
        /// Returns dVector4.zwzx swizzling.
        /// </summary>
        public dVector4 zwzx => new dVector4(z, w, z, x);
        
        /// <summary>
        /// Returns dVector4.babr swizzling (equivalent to dVector4.zwzx).
        /// </summary>
        public dVector4 babr => new dVector4(z, w, z, x);
        
        /// <summary>
        /// Returns dVector4.zwzy swizzling.
        /// </summary>
        public dVector4 zwzy => new dVector4(z, w, z, y);
        
        /// <summary>
        /// Returns dVector4.babg swizzling (equivalent to dVector4.zwzy).
        /// </summary>
        public dVector4 babg => new dVector4(z, w, z, y);
        
        /// <summary>
        /// Returns dVector4.zwzz swizzling.
        /// </summary>
        public dVector4 zwzz => new dVector4(z, w, z, z);
        
        /// <summary>
        /// Returns dVector4.babb swizzling (equivalent to dVector4.zwzz).
        /// </summary>
        public dVector4 babb => new dVector4(z, w, z, z);
        
        /// <summary>
        /// Returns dVector4.zwzw swizzling.
        /// </summary>
        public dVector4 zwzw => new dVector4(z, w, z, w);
        
        /// <summary>
        /// Returns dVector4.baba swizzling (equivalent to dVector4.zwzw).
        /// </summary>
        public dVector4 baba => new dVector4(z, w, z, w);
        
        /// <summary>
        /// Returns dVector4.zww swizzling.
        /// </summary>
        public dVector3 zww => new dVector3(z, w, w);
        
        /// <summary>
        /// Returns dVector4.baa swizzling (equivalent to dVector4.zww).
        /// </summary>
        public dVector3 baa => new dVector3(z, w, w);
        
        /// <summary>
        /// Returns dVector4.zwwx swizzling.
        /// </summary>
        public dVector4 zwwx => new dVector4(z, w, w, x);
        
        /// <summary>
        /// Returns dVector4.baar swizzling (equivalent to dVector4.zwwx).
        /// </summary>
        public dVector4 baar => new dVector4(z, w, w, x);
        
        /// <summary>
        /// Returns dVector4.zwwy swizzling.
        /// </summary>
        public dVector4 zwwy => new dVector4(z, w, w, y);
        
        /// <summary>
        /// Returns dVector4.baag swizzling (equivalent to dVector4.zwwy).
        /// </summary>
        public dVector4 baag => new dVector4(z, w, w, y);
        
        /// <summary>
        /// Returns dVector4.zwwz swizzling.
        /// </summary>
        public dVector4 zwwz => new dVector4(z, w, w, z);
        
        /// <summary>
        /// Returns dVector4.baab swizzling (equivalent to dVector4.zwwz).
        /// </summary>
        public dVector4 baab => new dVector4(z, w, w, z);
        
        /// <summary>
        /// Returns dVector4.zwww swizzling.
        /// </summary>
        public dVector4 zwww => new dVector4(z, w, w, w);
        
        /// <summary>
        /// Returns dVector4.baaa swizzling (equivalent to dVector4.zwww).
        /// </summary>
        public dVector4 baaa => new dVector4(z, w, w, w);
        
        /// <summary>
        /// Returns dVector4.wx swizzling.
        /// </summary>
        public dVector2 wx => new dVector2(w, x);
        
        /// <summary>
        /// Returns dVector4.ar swizzling (equivalent to dVector4.wx).
        /// </summary>
        public dVector2 ar => new dVector2(w, x);
        
        /// <summary>
        /// Returns dVector4.wxx swizzling.
        /// </summary>
        public dVector3 wxx => new dVector3(w, x, x);
        
        /// <summary>
        /// Returns dVector4.arr swizzling (equivalent to dVector4.wxx).
        /// </summary>
        public dVector3 arr => new dVector3(w, x, x);
        
        /// <summary>
        /// Returns dVector4.wxxx swizzling.
        /// </summary>
        public dVector4 wxxx => new dVector4(w, x, x, x);
        
        /// <summary>
        /// Returns dVector4.arrr swizzling (equivalent to dVector4.wxxx).
        /// </summary>
        public dVector4 arrr => new dVector4(w, x, x, x);
        
        /// <summary>
        /// Returns dVector4.wxxy swizzling.
        /// </summary>
        public dVector4 wxxy => new dVector4(w, x, x, y);
        
        /// <summary>
        /// Returns dVector4.arrg swizzling (equivalent to dVector4.wxxy).
        /// </summary>
        public dVector4 arrg => new dVector4(w, x, x, y);
        
        /// <summary>
        /// Returns dVector4.wxxz swizzling.
        /// </summary>
        public dVector4 wxxz => new dVector4(w, x, x, z);
        
        /// <summary>
        /// Returns dVector4.arrb swizzling (equivalent to dVector4.wxxz).
        /// </summary>
        public dVector4 arrb => new dVector4(w, x, x, z);
        
        /// <summary>
        /// Returns dVector4.wxxw swizzling.
        /// </summary>
        public dVector4 wxxw => new dVector4(w, x, x, w);
        
        /// <summary>
        /// Returns dVector4.arra swizzling (equivalent to dVector4.wxxw).
        /// </summary>
        public dVector4 arra => new dVector4(w, x, x, w);
        
        /// <summary>
        /// Returns dVector4.wxy swizzling.
        /// </summary>
        public dVector3 wxy => new dVector3(w, x, y);
        
        /// <summary>
        /// Returns dVector4.arg swizzling (equivalent to dVector4.wxy).
        /// </summary>
        public dVector3 arg => new dVector3(w, x, y);
        
        /// <summary>
        /// Returns dVector4.wxyx swizzling.
        /// </summary>
        public dVector4 wxyx => new dVector4(w, x, y, x);
        
        /// <summary>
        /// Returns dVector4.argr swizzling (equivalent to dVector4.wxyx).
        /// </summary>
        public dVector4 argr => new dVector4(w, x, y, x);
        
        /// <summary>
        /// Returns dVector4.wxyy swizzling.
        /// </summary>
        public dVector4 wxyy => new dVector4(w, x, y, y);
        
        /// <summary>
        /// Returns dVector4.argg swizzling (equivalent to dVector4.wxyy).
        /// </summary>
        public dVector4 argg => new dVector4(w, x, y, y);
        
        /// <summary>
        /// Returns dVector4.wxyz swizzling.
        /// </summary>
        public dVector4 wxyz => new dVector4(w, x, y, z);
        
        /// <summary>
        /// Returns dVector4.argb swizzling (equivalent to dVector4.wxyz).
        /// </summary>
        public dVector4 argb => new dVector4(w, x, y, z);
        
        /// <summary>
        /// Returns dVector4.wxyw swizzling.
        /// </summary>
        public dVector4 wxyw => new dVector4(w, x, y, w);
        
        /// <summary>
        /// Returns dVector4.arga swizzling (equivalent to dVector4.wxyw).
        /// </summary>
        public dVector4 arga => new dVector4(w, x, y, w);
        
        /// <summary>
        /// Returns dVector4.wxz swizzling.
        /// </summary>
        public dVector3 wxz => new dVector3(w, x, z);
        
        /// <summary>
        /// Returns dVector4.arb swizzling (equivalent to dVector4.wxz).
        /// </summary>
        public dVector3 arb => new dVector3(w, x, z);
        
        /// <summary>
        /// Returns dVector4.wxzx swizzling.
        /// </summary>
        public dVector4 wxzx => new dVector4(w, x, z, x);
        
        /// <summary>
        /// Returns dVector4.arbr swizzling (equivalent to dVector4.wxzx).
        /// </summary>
        public dVector4 arbr => new dVector4(w, x, z, x);
        
        /// <summary>
        /// Returns dVector4.wxzy swizzling.
        /// </summary>
        public dVector4 wxzy => new dVector4(w, x, z, y);
        
        /// <summary>
        /// Returns dVector4.arbg swizzling (equivalent to dVector4.wxzy).
        /// </summary>
        public dVector4 arbg => new dVector4(w, x, z, y);
        
        /// <summary>
        /// Returns dVector4.wxzz swizzling.
        /// </summary>
        public dVector4 wxzz => new dVector4(w, x, z, z);
        
        /// <summary>
        /// Returns dVector4.arbb swizzling (equivalent to dVector4.wxzz).
        /// </summary>
        public dVector4 arbb => new dVector4(w, x, z, z);
        
        /// <summary>
        /// Returns dVector4.wxzw swizzling.
        /// </summary>
        public dVector4 wxzw => new dVector4(w, x, z, w);
        
        /// <summary>
        /// Returns dVector4.arba swizzling (equivalent to dVector4.wxzw).
        /// </summary>
        public dVector4 arba => new dVector4(w, x, z, w);
        
        /// <summary>
        /// Returns dVector4.wxw swizzling.
        /// </summary>
        public dVector3 wxw => new dVector3(w, x, w);
        
        /// <summary>
        /// Returns dVector4.ara swizzling (equivalent to dVector4.wxw).
        /// </summary>
        public dVector3 ara => new dVector3(w, x, w);
        
        /// <summary>
        /// Returns dVector4.wxwx swizzling.
        /// </summary>
        public dVector4 wxwx => new dVector4(w, x, w, x);
        
        /// <summary>
        /// Returns dVector4.arar swizzling (equivalent to dVector4.wxwx).
        /// </summary>
        public dVector4 arar => new dVector4(w, x, w, x);
        
        /// <summary>
        /// Returns dVector4.wxwy swizzling.
        /// </summary>
        public dVector4 wxwy => new dVector4(w, x, w, y);
        
        /// <summary>
        /// Returns dVector4.arag swizzling (equivalent to dVector4.wxwy).
        /// </summary>
        public dVector4 arag => new dVector4(w, x, w, y);
        
        /// <summary>
        /// Returns dVector4.wxwz swizzling.
        /// </summary>
        public dVector4 wxwz => new dVector4(w, x, w, z);
        
        /// <summary>
        /// Returns dVector4.arab swizzling (equivalent to dVector4.wxwz).
        /// </summary>
        public dVector4 arab => new dVector4(w, x, w, z);
        
        /// <summary>
        /// Returns dVector4.wxww swizzling.
        /// </summary>
        public dVector4 wxww => new dVector4(w, x, w, w);
        
        /// <summary>
        /// Returns dVector4.araa swizzling (equivalent to dVector4.wxww).
        /// </summary>
        public dVector4 araa => new dVector4(w, x, w, w);
        
        /// <summary>
        /// Returns dVector4.wy swizzling.
        /// </summary>
        public dVector2 wy => new dVector2(w, y);
        
        /// <summary>
        /// Returns dVector4.ag swizzling (equivalent to dVector4.wy).
        /// </summary>
        public dVector2 ag => new dVector2(w, y);
        
        /// <summary>
        /// Returns dVector4.wyx swizzling.
        /// </summary>
        public dVector3 wyx => new dVector3(w, y, x);
        
        /// <summary>
        /// Returns dVector4.agr swizzling (equivalent to dVector4.wyx).
        /// </summary>
        public dVector3 agr => new dVector3(w, y, x);
        
        /// <summary>
        /// Returns dVector4.wyxx swizzling.
        /// </summary>
        public dVector4 wyxx => new dVector4(w, y, x, x);
        
        /// <summary>
        /// Returns dVector4.agrr swizzling (equivalent to dVector4.wyxx).
        /// </summary>
        public dVector4 agrr => new dVector4(w, y, x, x);
        
        /// <summary>
        /// Returns dVector4.wyxy swizzling.
        /// </summary>
        public dVector4 wyxy => new dVector4(w, y, x, y);
        
        /// <summary>
        /// Returns dVector4.agrg swizzling (equivalent to dVector4.wyxy).
        /// </summary>
        public dVector4 agrg => new dVector4(w, y, x, y);
        
        /// <summary>
        /// Returns dVector4.wyxz swizzling.
        /// </summary>
        public dVector4 wyxz => new dVector4(w, y, x, z);
        
        /// <summary>
        /// Returns dVector4.agrb swizzling (equivalent to dVector4.wyxz).
        /// </summary>
        public dVector4 agrb => new dVector4(w, y, x, z);
        
        /// <summary>
        /// Returns dVector4.wyxw swizzling.
        /// </summary>
        public dVector4 wyxw => new dVector4(w, y, x, w);
        
        /// <summary>
        /// Returns dVector4.agra swizzling (equivalent to dVector4.wyxw).
        /// </summary>
        public dVector4 agra => new dVector4(w, y, x, w);
        
        /// <summary>
        /// Returns dVector4.wyy swizzling.
        /// </summary>
        public dVector3 wyy => new dVector3(w, y, y);
        
        /// <summary>
        /// Returns dVector4.agg swizzling (equivalent to dVector4.wyy).
        /// </summary>
        public dVector3 agg => new dVector3(w, y, y);
        
        /// <summary>
        /// Returns dVector4.wyyx swizzling.
        /// </summary>
        public dVector4 wyyx => new dVector4(w, y, y, x);
        
        /// <summary>
        /// Returns dVector4.aggr swizzling (equivalent to dVector4.wyyx).
        /// </summary>
        public dVector4 aggr => new dVector4(w, y, y, x);
        
        /// <summary>
        /// Returns dVector4.wyyy swizzling.
        /// </summary>
        public dVector4 wyyy => new dVector4(w, y, y, y);
        
        /// <summary>
        /// Returns dVector4.aggg swizzling (equivalent to dVector4.wyyy).
        /// </summary>
        public dVector4 aggg => new dVector4(w, y, y, y);
        
        /// <summary>
        /// Returns dVector4.wyyz swizzling.
        /// </summary>
        public dVector4 wyyz => new dVector4(w, y, y, z);
        
        /// <summary>
        /// Returns dVector4.aggb swizzling (equivalent to dVector4.wyyz).
        /// </summary>
        public dVector4 aggb => new dVector4(w, y, y, z);
        
        /// <summary>
        /// Returns dVector4.wyyw swizzling.
        /// </summary>
        public dVector4 wyyw => new dVector4(w, y, y, w);
        
        /// <summary>
        /// Returns dVector4.agga swizzling (equivalent to dVector4.wyyw).
        /// </summary>
        public dVector4 agga => new dVector4(w, y, y, w);
        
        /// <summary>
        /// Returns dVector4.wyz swizzling.
        /// </summary>
        public dVector3 wyz => new dVector3(w, y, z);
        
        /// <summary>
        /// Returns dVector4.agb swizzling (equivalent to dVector4.wyz).
        /// </summary>
        public dVector3 agb => new dVector3(w, y, z);
        
        /// <summary>
        /// Returns dVector4.wyzx swizzling.
        /// </summary>
        public dVector4 wyzx => new dVector4(w, y, z, x);
        
        /// <summary>
        /// Returns dVector4.agbr swizzling (equivalent to dVector4.wyzx).
        /// </summary>
        public dVector4 agbr => new dVector4(w, y, z, x);
        
        /// <summary>
        /// Returns dVector4.wyzy swizzling.
        /// </summary>
        public dVector4 wyzy => new dVector4(w, y, z, y);
        
        /// <summary>
        /// Returns dVector4.agbg swizzling (equivalent to dVector4.wyzy).
        /// </summary>
        public dVector4 agbg => new dVector4(w, y, z, y);
        
        /// <summary>
        /// Returns dVector4.wyzz swizzling.
        /// </summary>
        public dVector4 wyzz => new dVector4(w, y, z, z);
        
        /// <summary>
        /// Returns dVector4.agbb swizzling (equivalent to dVector4.wyzz).
        /// </summary>
        public dVector4 agbb => new dVector4(w, y, z, z);
        
        /// <summary>
        /// Returns dVector4.wyzw swizzling.
        /// </summary>
        public dVector4 wyzw => new dVector4(w, y, z, w);
        
        /// <summary>
        /// Returns dVector4.agba swizzling (equivalent to dVector4.wyzw).
        /// </summary>
        public dVector4 agba => new dVector4(w, y, z, w);
        
        /// <summary>
        /// Returns dVector4.wyw swizzling.
        /// </summary>
        public dVector3 wyw => new dVector3(w, y, w);
        
        /// <summary>
        /// Returns dVector4.aga swizzling (equivalent to dVector4.wyw).
        /// </summary>
        public dVector3 aga => new dVector3(w, y, w);
        
        /// <summary>
        /// Returns dVector4.wywx swizzling.
        /// </summary>
        public dVector4 wywx => new dVector4(w, y, w, x);
        
        /// <summary>
        /// Returns dVector4.agar swizzling (equivalent to dVector4.wywx).
        /// </summary>
        public dVector4 agar => new dVector4(w, y, w, x);
        
        /// <summary>
        /// Returns dVector4.wywy swizzling.
        /// </summary>
        public dVector4 wywy => new dVector4(w, y, w, y);
        
        /// <summary>
        /// Returns dVector4.agag swizzling (equivalent to dVector4.wywy).
        /// </summary>
        public dVector4 agag => new dVector4(w, y, w, y);
        
        /// <summary>
        /// Returns dVector4.wywz swizzling.
        /// </summary>
        public dVector4 wywz => new dVector4(w, y, w, z);
        
        /// <summary>
        /// Returns dVector4.agab swizzling (equivalent to dVector4.wywz).
        /// </summary>
        public dVector4 agab => new dVector4(w, y, w, z);
        
        /// <summary>
        /// Returns dVector4.wyww swizzling.
        /// </summary>
        public dVector4 wyww => new dVector4(w, y, w, w);
        
        /// <summary>
        /// Returns dVector4.agaa swizzling (equivalent to dVector4.wyww).
        /// </summary>
        public dVector4 agaa => new dVector4(w, y, w, w);
        
        /// <summary>
        /// Returns dVector4.wz swizzling.
        /// </summary>
        public dVector2 wz => new dVector2(w, z);
        
        /// <summary>
        /// Returns dVector4.ab swizzling (equivalent to dVector4.wz).
        /// </summary>
        public dVector2 ab => new dVector2(w, z);
        
        /// <summary>
        /// Returns dVector4.wzx swizzling.
        /// </summary>
        public dVector3 wzx => new dVector3(w, z, x);
        
        /// <summary>
        /// Returns dVector4.abr swizzling (equivalent to dVector4.wzx).
        /// </summary>
        public dVector3 abr => new dVector3(w, z, x);
        
        /// <summary>
        /// Returns dVector4.wzxx swizzling.
        /// </summary>
        public dVector4 wzxx => new dVector4(w, z, x, x);
        
        /// <summary>
        /// Returns dVector4.abrr swizzling (equivalent to dVector4.wzxx).
        /// </summary>
        public dVector4 abrr => new dVector4(w, z, x, x);
        
        /// <summary>
        /// Returns dVector4.wzxy swizzling.
        /// </summary>
        public dVector4 wzxy => new dVector4(w, z, x, y);
        
        /// <summary>
        /// Returns dVector4.abrg swizzling (equivalent to dVector4.wzxy).
        /// </summary>
        public dVector4 abrg => new dVector4(w, z, x, y);
        
        /// <summary>
        /// Returns dVector4.wzxz swizzling.
        /// </summary>
        public dVector4 wzxz => new dVector4(w, z, x, z);
        
        /// <summary>
        /// Returns dVector4.abrb swizzling (equivalent to dVector4.wzxz).
        /// </summary>
        public dVector4 abrb => new dVector4(w, z, x, z);
        
        /// <summary>
        /// Returns dVector4.wzxw swizzling.
        /// </summary>
        public dVector4 wzxw => new dVector4(w, z, x, w);
        
        /// <summary>
        /// Returns dVector4.abra swizzling (equivalent to dVector4.wzxw).
        /// </summary>
        public dVector4 abra => new dVector4(w, z, x, w);
        
        /// <summary>
        /// Returns dVector4.wzy swizzling.
        /// </summary>
        public dVector3 wzy => new dVector3(w, z, y);
        
        /// <summary>
        /// Returns dVector4.abg swizzling (equivalent to dVector4.wzy).
        /// </summary>
        public dVector3 abg => new dVector3(w, z, y);
        
        /// <summary>
        /// Returns dVector4.wzyx swizzling.
        /// </summary>
        public dVector4 wzyx => new dVector4(w, z, y, x);
        
        /// <summary>
        /// Returns dVector4.abgr swizzling (equivalent to dVector4.wzyx).
        /// </summary>
        public dVector4 abgr => new dVector4(w, z, y, x);
        
        /// <summary>
        /// Returns dVector4.wzyy swizzling.
        /// </summary>
        public dVector4 wzyy => new dVector4(w, z, y, y);
        
        /// <summary>
        /// Returns dVector4.abgg swizzling (equivalent to dVector4.wzyy).
        /// </summary>
        public dVector4 abgg => new dVector4(w, z, y, y);
        
        /// <summary>
        /// Returns dVector4.wzyz swizzling.
        /// </summary>
        public dVector4 wzyz => new dVector4(w, z, y, z);
        
        /// <summary>
        /// Returns dVector4.abgb swizzling (equivalent to dVector4.wzyz).
        /// </summary>
        public dVector4 abgb => new dVector4(w, z, y, z);
        
        /// <summary>
        /// Returns dVector4.wzyw swizzling.
        /// </summary>
        public dVector4 wzyw => new dVector4(w, z, y, w);
        
        /// <summary>
        /// Returns dVector4.abga swizzling (equivalent to dVector4.wzyw).
        /// </summary>
        public dVector4 abga => new dVector4(w, z, y, w);
        
        /// <summary>
        /// Returns dVector4.wzz swizzling.
        /// </summary>
        public dVector3 wzz => new dVector3(w, z, z);
        
        /// <summary>
        /// Returns dVector4.abb swizzling (equivalent to dVector4.wzz).
        /// </summary>
        public dVector3 abb => new dVector3(w, z, z);
        
        /// <summary>
        /// Returns dVector4.wzzx swizzling.
        /// </summary>
        public dVector4 wzzx => new dVector4(w, z, z, x);
        
        /// <summary>
        /// Returns dVector4.abbr swizzling (equivalent to dVector4.wzzx).
        /// </summary>
        public dVector4 abbr => new dVector4(w, z, z, x);
        
        /// <summary>
        /// Returns dVector4.wzzy swizzling.
        /// </summary>
        public dVector4 wzzy => new dVector4(w, z, z, y);
        
        /// <summary>
        /// Returns dVector4.abbg swizzling (equivalent to dVector4.wzzy).
        /// </summary>
        public dVector4 abbg => new dVector4(w, z, z, y);
        
        /// <summary>
        /// Returns dVector4.wzzz swizzling.
        /// </summary>
        public dVector4 wzzz => new dVector4(w, z, z, z);
        
        /// <summary>
        /// Returns dVector4.abbb swizzling (equivalent to dVector4.wzzz).
        /// </summary>
        public dVector4 abbb => new dVector4(w, z, z, z);
        
        /// <summary>
        /// Returns dVector4.wzzw swizzling.
        /// </summary>
        public dVector4 wzzw => new dVector4(w, z, z, w);
        
        /// <summary>
        /// Returns dVector4.abba swizzling (equivalent to dVector4.wzzw).
        /// </summary>
        public dVector4 abba => new dVector4(w, z, z, w);
        
        /// <summary>
        /// Returns dVector4.wzw swizzling.
        /// </summary>
        public dVector3 wzw => new dVector3(w, z, w);
        
        /// <summary>
        /// Returns dVector4.aba swizzling (equivalent to dVector4.wzw).
        /// </summary>
        public dVector3 aba => new dVector3(w, z, w);
        
        /// <summary>
        /// Returns dVector4.wzwx swizzling.
        /// </summary>
        public dVector4 wzwx => new dVector4(w, z, w, x);
        
        /// <summary>
        /// Returns dVector4.abar swizzling (equivalent to dVector4.wzwx).
        /// </summary>
        public dVector4 abar => new dVector4(w, z, w, x);
        
        /// <summary>
        /// Returns dVector4.wzwy swizzling.
        /// </summary>
        public dVector4 wzwy => new dVector4(w, z, w, y);
        
        /// <summary>
        /// Returns dVector4.abag swizzling (equivalent to dVector4.wzwy).
        /// </summary>
        public dVector4 abag => new dVector4(w, z, w, y);
        
        /// <summary>
        /// Returns dVector4.wzwz swizzling.
        /// </summary>
        public dVector4 wzwz => new dVector4(w, z, w, z);
        
        /// <summary>
        /// Returns dVector4.abab swizzling (equivalent to dVector4.wzwz).
        /// </summary>
        public dVector4 abab => new dVector4(w, z, w, z);
        
        /// <summary>
        /// Returns dVector4.wzww swizzling.
        /// </summary>
        public dVector4 wzww => new dVector4(w, z, w, w);
        
        /// <summary>
        /// Returns dVector4.abaa swizzling (equivalent to dVector4.wzww).
        /// </summary>
        public dVector4 abaa => new dVector4(w, z, w, w);
        
        /// <summary>
        /// Returns dVector4.ww swizzling.
        /// </summary>
        public dVector2 ww => new dVector2(w, w);
        
        /// <summary>
        /// Returns dVector4.aa swizzling (equivalent to dVector4.ww).
        /// </summary>
        public dVector2 aa => new dVector2(w, w);
        
        /// <summary>
        /// Returns dVector4.wwx swizzling.
        /// </summary>
        public dVector3 wwx => new dVector3(w, w, x);
        
        /// <summary>
        /// Returns dVector4.aar swizzling (equivalent to dVector4.wwx).
        /// </summary>
        public dVector3 aar => new dVector3(w, w, x);
        
        /// <summary>
        /// Returns dVector4.wwxx swizzling.
        /// </summary>
        public dVector4 wwxx => new dVector4(w, w, x, x);
        
        /// <summary>
        /// Returns dVector4.aarr swizzling (equivalent to dVector4.wwxx).
        /// </summary>
        public dVector4 aarr => new dVector4(w, w, x, x);
        
        /// <summary>
        /// Returns dVector4.wwxy swizzling.
        /// </summary>
        public dVector4 wwxy => new dVector4(w, w, x, y);
        
        /// <summary>
        /// Returns dVector4.aarg swizzling (equivalent to dVector4.wwxy).
        /// </summary>
        public dVector4 aarg => new dVector4(w, w, x, y);
        
        /// <summary>
        /// Returns dVector4.wwxz swizzling.
        /// </summary>
        public dVector4 wwxz => new dVector4(w, w, x, z);
        
        /// <summary>
        /// Returns dVector4.aarb swizzling (equivalent to dVector4.wwxz).
        /// </summary>
        public dVector4 aarb => new dVector4(w, w, x, z);
        
        /// <summary>
        /// Returns dVector4.wwxw swizzling.
        /// </summary>
        public dVector4 wwxw => new dVector4(w, w, x, w);
        
        /// <summary>
        /// Returns dVector4.aara swizzling (equivalent to dVector4.wwxw).
        /// </summary>
        public dVector4 aara => new dVector4(w, w, x, w);
        
        /// <summary>
        /// Returns dVector4.wwy swizzling.
        /// </summary>
        public dVector3 wwy => new dVector3(w, w, y);
        
        /// <summary>
        /// Returns dVector4.aag swizzling (equivalent to dVector4.wwy).
        /// </summary>
        public dVector3 aag => new dVector3(w, w, y);
        
        /// <summary>
        /// Returns dVector4.wwyx swizzling.
        /// </summary>
        public dVector4 wwyx => new dVector4(w, w, y, x);
        
        /// <summary>
        /// Returns dVector4.aagr swizzling (equivalent to dVector4.wwyx).
        /// </summary>
        public dVector4 aagr => new dVector4(w, w, y, x);
        
        /// <summary>
        /// Returns dVector4.wwyy swizzling.
        /// </summary>
        public dVector4 wwyy => new dVector4(w, w, y, y);
        
        /// <summary>
        /// Returns dVector4.aagg swizzling (equivalent to dVector4.wwyy).
        /// </summary>
        public dVector4 aagg => new dVector4(w, w, y, y);
        
        /// <summary>
        /// Returns dVector4.wwyz swizzling.
        /// </summary>
        public dVector4 wwyz => new dVector4(w, w, y, z);
        
        /// <summary>
        /// Returns dVector4.aagb swizzling (equivalent to dVector4.wwyz).
        /// </summary>
        public dVector4 aagb => new dVector4(w, w, y, z);
        
        /// <summary>
        /// Returns dVector4.wwyw swizzling.
        /// </summary>
        public dVector4 wwyw => new dVector4(w, w, y, w);
        
        /// <summary>
        /// Returns dVector4.aaga swizzling (equivalent to dVector4.wwyw).
        /// </summary>
        public dVector4 aaga => new dVector4(w, w, y, w);
        
        /// <summary>
        /// Returns dVector4.wwz swizzling.
        /// </summary>
        public dVector3 wwz => new dVector3(w, w, z);
        
        /// <summary>
        /// Returns dVector4.aab swizzling (equivalent to dVector4.wwz).
        /// </summary>
        public dVector3 aab => new dVector3(w, w, z);
        
        /// <summary>
        /// Returns dVector4.wwzx swizzling.
        /// </summary>
        public dVector4 wwzx => new dVector4(w, w, z, x);
        
        /// <summary>
        /// Returns dVector4.aabr swizzling (equivalent to dVector4.wwzx).
        /// </summary>
        public dVector4 aabr => new dVector4(w, w, z, x);
        
        /// <summary>
        /// Returns dVector4.wwzy swizzling.
        /// </summary>
        public dVector4 wwzy => new dVector4(w, w, z, y);
        
        /// <summary>
        /// Returns dVector4.aabg swizzling (equivalent to dVector4.wwzy).
        /// </summary>
        public dVector4 aabg => new dVector4(w, w, z, y);
        
        /// <summary>
        /// Returns dVector4.wwzz swizzling.
        /// </summary>
        public dVector4 wwzz => new dVector4(w, w, z, z);
        
        /// <summary>
        /// Returns dVector4.aabb swizzling (equivalent to dVector4.wwzz).
        /// </summary>
        public dVector4 aabb => new dVector4(w, w, z, z);
        
        /// <summary>
        /// Returns dVector4.wwzw swizzling.
        /// </summary>
        public dVector4 wwzw => new dVector4(w, w, z, w);
        
        /// <summary>
        /// Returns dVector4.aaba swizzling (equivalent to dVector4.wwzw).
        /// </summary>
        public dVector4 aaba => new dVector4(w, w, z, w);
        
        /// <summary>
        /// Returns dVector4.www swizzling.
        /// </summary>
        public dVector3 www => new dVector3(w, w, w);
        
        /// <summary>
        /// Returns dVector4.aaa swizzling (equivalent to dVector4.www).
        /// </summary>
        public dVector3 aaa => new dVector3(w, w, w);
        
        /// <summary>
        /// Returns dVector4.wwwx swizzling.
        /// </summary>
        public dVector4 wwwx => new dVector4(w, w, w, x);
        
        /// <summary>
        /// Returns dVector4.aaar swizzling (equivalent to dVector4.wwwx).
        /// </summary>
        public dVector4 aaar => new dVector4(w, w, w, x);
        
        /// <summary>
        /// Returns dVector4.wwwy swizzling.
        /// </summary>
        public dVector4 wwwy => new dVector4(w, w, w, y);
        
        /// <summary>
        /// Returns dVector4.aaag swizzling (equivalent to dVector4.wwwy).
        /// </summary>
        public dVector4 aaag => new dVector4(w, w, w, y);
        
        /// <summary>
        /// Returns dVector4.wwwz swizzling.
        /// </summary>
        public dVector4 wwwz => new dVector4(w, w, w, z);
        
        /// <summary>
        /// Returns dVector4.aaab swizzling (equivalent to dVector4.wwwz).
        /// </summary>
        public dVector4 aaab => new dVector4(w, w, w, z);
        
        /// <summary>
        /// Returns dVector4.wwww swizzling.
        /// </summary>
        public dVector4 wwww => new dVector4(w, w, w, w);
        
        /// <summary>
        /// Returns dVector4.aaaa swizzling (equivalent to dVector4.wwww).
        /// </summary>
        public dVector4 aaaa => new dVector4(w, w, w, w);

        #endregion

    }
}
