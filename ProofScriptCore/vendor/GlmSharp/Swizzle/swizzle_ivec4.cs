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
    /// Temporary Vectortor of type int with 4 components, used for implementing swizzling for iVector4.
    /// </summary>
    [RegisterCoreClassStruct]
   
    [StructLayout(LayoutKind.Sequential)]
    public struct swizzle_iVector4
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
        
        /// <summary>
        /// w-component
        /// </summary>
        
        internal readonly int w;

        #endregion


        #region Constructors
        
        /// <summary>
        /// Constructor for swizzle_iVector4.
        /// </summary>
        internal swizzle_iVector4(int x, int y, int z, int w)
        {
            this.x = x;
            this.y = y;
            this.z = z;
            this.w = w;
        }

        #endregion


        #region Properties
        
        /// <summary>
        /// Returns iVector4.xx swizzling.
        /// </summary>
        public iVector2 xx => new iVector2(x, x);
        
        /// <summary>
        /// Returns iVector4.rr swizzling (equivalent to iVector4.xx).
        /// </summary>
        public iVector2 rr => new iVector2(x, x);
        
        /// <summary>
        /// Returns iVector4.xxx swizzling.
        /// </summary>
        public iVector3 xxx => new iVector3(x, x, x);
        
        /// <summary>
        /// Returns iVector4.rrr swizzling (equivalent to iVector4.xxx).
        /// </summary>
        public iVector3 rrr => new iVector3(x, x, x);
        
        /// <summary>
        /// Returns iVector4.xxxx swizzling.
        /// </summary>
        public iVector4 xxxx => new iVector4(x, x, x, x);
        
        /// <summary>
        /// Returns iVector4.rrrr swizzling (equivalent to iVector4.xxxx).
        /// </summary>
        public iVector4 rrrr => new iVector4(x, x, x, x);
        
        /// <summary>
        /// Returns iVector4.xxxy swizzling.
        /// </summary>
        public iVector4 xxxy => new iVector4(x, x, x, y);
        
        /// <summary>
        /// Returns iVector4.rrrg swizzling (equivalent to iVector4.xxxy).
        /// </summary>
        public iVector4 rrrg => new iVector4(x, x, x, y);
        
        /// <summary>
        /// Returns iVector4.xxxz swizzling.
        /// </summary>
        public iVector4 xxxz => new iVector4(x, x, x, z);
        
        /// <summary>
        /// Returns iVector4.rrrb swizzling (equivalent to iVector4.xxxz).
        /// </summary>
        public iVector4 rrrb => new iVector4(x, x, x, z);
        
        /// <summary>
        /// Returns iVector4.xxxw swizzling.
        /// </summary>
        public iVector4 xxxw => new iVector4(x, x, x, w);
        
        /// <summary>
        /// Returns iVector4.rrra swizzling (equivalent to iVector4.xxxw).
        /// </summary>
        public iVector4 rrra => new iVector4(x, x, x, w);
        
        /// <summary>
        /// Returns iVector4.xxy swizzling.
        /// </summary>
        public iVector3 xxy => new iVector3(x, x, y);
        
        /// <summary>
        /// Returns iVector4.rrg swizzling (equivalent to iVector4.xxy).
        /// </summary>
        public iVector3 rrg => new iVector3(x, x, y);
        
        /// <summary>
        /// Returns iVector4.xxyx swizzling.
        /// </summary>
        public iVector4 xxyx => new iVector4(x, x, y, x);
        
        /// <summary>
        /// Returns iVector4.rrgr swizzling (equivalent to iVector4.xxyx).
        /// </summary>
        public iVector4 rrgr => new iVector4(x, x, y, x);
        
        /// <summary>
        /// Returns iVector4.xxyy swizzling.
        /// </summary>
        public iVector4 xxyy => new iVector4(x, x, y, y);
        
        /// <summary>
        /// Returns iVector4.rrgg swizzling (equivalent to iVector4.xxyy).
        /// </summary>
        public iVector4 rrgg => new iVector4(x, x, y, y);
        
        /// <summary>
        /// Returns iVector4.xxyz swizzling.
        /// </summary>
        public iVector4 xxyz => new iVector4(x, x, y, z);
        
        /// <summary>
        /// Returns iVector4.rrgb swizzling (equivalent to iVector4.xxyz).
        /// </summary>
        public iVector4 rrgb => new iVector4(x, x, y, z);
        
        /// <summary>
        /// Returns iVector4.xxyw swizzling.
        /// </summary>
        public iVector4 xxyw => new iVector4(x, x, y, w);
        
        /// <summary>
        /// Returns iVector4.rrga swizzling (equivalent to iVector4.xxyw).
        /// </summary>
        public iVector4 rrga => new iVector4(x, x, y, w);
        
        /// <summary>
        /// Returns iVector4.xxz swizzling.
        /// </summary>
        public iVector3 xxz => new iVector3(x, x, z);
        
        /// <summary>
        /// Returns iVector4.rrb swizzling (equivalent to iVector4.xxz).
        /// </summary>
        public iVector3 rrb => new iVector3(x, x, z);
        
        /// <summary>
        /// Returns iVector4.xxzx swizzling.
        /// </summary>
        public iVector4 xxzx => new iVector4(x, x, z, x);
        
        /// <summary>
        /// Returns iVector4.rrbr swizzling (equivalent to iVector4.xxzx).
        /// </summary>
        public iVector4 rrbr => new iVector4(x, x, z, x);
        
        /// <summary>
        /// Returns iVector4.xxzy swizzling.
        /// </summary>
        public iVector4 xxzy => new iVector4(x, x, z, y);
        
        /// <summary>
        /// Returns iVector4.rrbg swizzling (equivalent to iVector4.xxzy).
        /// </summary>
        public iVector4 rrbg => new iVector4(x, x, z, y);
        
        /// <summary>
        /// Returns iVector4.xxzz swizzling.
        /// </summary>
        public iVector4 xxzz => new iVector4(x, x, z, z);
        
        /// <summary>
        /// Returns iVector4.rrbb swizzling (equivalent to iVector4.xxzz).
        /// </summary>
        public iVector4 rrbb => new iVector4(x, x, z, z);
        
        /// <summary>
        /// Returns iVector4.xxzw swizzling.
        /// </summary>
        public iVector4 xxzw => new iVector4(x, x, z, w);
        
        /// <summary>
        /// Returns iVector4.rrba swizzling (equivalent to iVector4.xxzw).
        /// </summary>
        public iVector4 rrba => new iVector4(x, x, z, w);
        
        /// <summary>
        /// Returns iVector4.xxw swizzling.
        /// </summary>
        public iVector3 xxw => new iVector3(x, x, w);
        
        /// <summary>
        /// Returns iVector4.rra swizzling (equivalent to iVector4.xxw).
        /// </summary>
        public iVector3 rra => new iVector3(x, x, w);
        
        /// <summary>
        /// Returns iVector4.xxwx swizzling.
        /// </summary>
        public iVector4 xxwx => new iVector4(x, x, w, x);
        
        /// <summary>
        /// Returns iVector4.rrar swizzling (equivalent to iVector4.xxwx).
        /// </summary>
        public iVector4 rrar => new iVector4(x, x, w, x);
        
        /// <summary>
        /// Returns iVector4.xxwy swizzling.
        /// </summary>
        public iVector4 xxwy => new iVector4(x, x, w, y);
        
        /// <summary>
        /// Returns iVector4.rrag swizzling (equivalent to iVector4.xxwy).
        /// </summary>
        public iVector4 rrag => new iVector4(x, x, w, y);
        
        /// <summary>
        /// Returns iVector4.xxwz swizzling.
        /// </summary>
        public iVector4 xxwz => new iVector4(x, x, w, z);
        
        /// <summary>
        /// Returns iVector4.rrab swizzling (equivalent to iVector4.xxwz).
        /// </summary>
        public iVector4 rrab => new iVector4(x, x, w, z);
        
        /// <summary>
        /// Returns iVector4.xxww swizzling.
        /// </summary>
        public iVector4 xxww => new iVector4(x, x, w, w);
        
        /// <summary>
        /// Returns iVector4.rraa swizzling (equivalent to iVector4.xxww).
        /// </summary>
        public iVector4 rraa => new iVector4(x, x, w, w);
        
        /// <summary>
        /// Returns iVector4.xy swizzling.
        /// </summary>
        public iVector2 xy => new iVector2(x, y);
        
        /// <summary>
        /// Returns iVector4.rg swizzling (equivalent to iVector4.xy).
        /// </summary>
        public iVector2 rg => new iVector2(x, y);
        
        /// <summary>
        /// Returns iVector4.xyx swizzling.
        /// </summary>
        public iVector3 xyx => new iVector3(x, y, x);
        
        /// <summary>
        /// Returns iVector4.rgr swizzling (equivalent to iVector4.xyx).
        /// </summary>
        public iVector3 rgr => new iVector3(x, y, x);
        
        /// <summary>
        /// Returns iVector4.xyxx swizzling.
        /// </summary>
        public iVector4 xyxx => new iVector4(x, y, x, x);
        
        /// <summary>
        /// Returns iVector4.rgrr swizzling (equivalent to iVector4.xyxx).
        /// </summary>
        public iVector4 rgrr => new iVector4(x, y, x, x);
        
        /// <summary>
        /// Returns iVector4.xyxy swizzling.
        /// </summary>
        public iVector4 xyxy => new iVector4(x, y, x, y);
        
        /// <summary>
        /// Returns iVector4.rgrg swizzling (equivalent to iVector4.xyxy).
        /// </summary>
        public iVector4 rgrg => new iVector4(x, y, x, y);
        
        /// <summary>
        /// Returns iVector4.xyxz swizzling.
        /// </summary>
        public iVector4 xyxz => new iVector4(x, y, x, z);
        
        /// <summary>
        /// Returns iVector4.rgrb swizzling (equivalent to iVector4.xyxz).
        /// </summary>
        public iVector4 rgrb => new iVector4(x, y, x, z);
        
        /// <summary>
        /// Returns iVector4.xyxw swizzling.
        /// </summary>
        public iVector4 xyxw => new iVector4(x, y, x, w);
        
        /// <summary>
        /// Returns iVector4.rgra swizzling (equivalent to iVector4.xyxw).
        /// </summary>
        public iVector4 rgra => new iVector4(x, y, x, w);
        
        /// <summary>
        /// Returns iVector4.xyy swizzling.
        /// </summary>
        public iVector3 xyy => new iVector3(x, y, y);
        
        /// <summary>
        /// Returns iVector4.rgg swizzling (equivalent to iVector4.xyy).
        /// </summary>
        public iVector3 rgg => new iVector3(x, y, y);
        
        /// <summary>
        /// Returns iVector4.xyyx swizzling.
        /// </summary>
        public iVector4 xyyx => new iVector4(x, y, y, x);
        
        /// <summary>
        /// Returns iVector4.rggr swizzling (equivalent to iVector4.xyyx).
        /// </summary>
        public iVector4 rggr => new iVector4(x, y, y, x);
        
        /// <summary>
        /// Returns iVector4.xyyy swizzling.
        /// </summary>
        public iVector4 xyyy => new iVector4(x, y, y, y);
        
        /// <summary>
        /// Returns iVector4.rggg swizzling (equivalent to iVector4.xyyy).
        /// </summary>
        public iVector4 rggg => new iVector4(x, y, y, y);
        
        /// <summary>
        /// Returns iVector4.xyyz swizzling.
        /// </summary>
        public iVector4 xyyz => new iVector4(x, y, y, z);
        
        /// <summary>
        /// Returns iVector4.rggb swizzling (equivalent to iVector4.xyyz).
        /// </summary>
        public iVector4 rggb => new iVector4(x, y, y, z);
        
        /// <summary>
        /// Returns iVector4.xyyw swizzling.
        /// </summary>
        public iVector4 xyyw => new iVector4(x, y, y, w);
        
        /// <summary>
        /// Returns iVector4.rgga swizzling (equivalent to iVector4.xyyw).
        /// </summary>
        public iVector4 rgga => new iVector4(x, y, y, w);
        
        /// <summary>
        /// Returns iVector4.xyz swizzling.
        /// </summary>
        public iVector3 xyz => new iVector3(x, y, z);
        
        /// <summary>
        /// Returns iVector4.rgb swizzling (equivalent to iVector4.xyz).
        /// </summary>
        public iVector3 rgb => new iVector3(x, y, z);
        
        /// <summary>
        /// Returns iVector4.xyzx swizzling.
        /// </summary>
        public iVector4 xyzx => new iVector4(x, y, z, x);
        
        /// <summary>
        /// Returns iVector4.rgbr swizzling (equivalent to iVector4.xyzx).
        /// </summary>
        public iVector4 rgbr => new iVector4(x, y, z, x);
        
        /// <summary>
        /// Returns iVector4.xyzy swizzling.
        /// </summary>
        public iVector4 xyzy => new iVector4(x, y, z, y);
        
        /// <summary>
        /// Returns iVector4.rgbg swizzling (equivalent to iVector4.xyzy).
        /// </summary>
        public iVector4 rgbg => new iVector4(x, y, z, y);
        
        /// <summary>
        /// Returns iVector4.xyzz swizzling.
        /// </summary>
        public iVector4 xyzz => new iVector4(x, y, z, z);
        
        /// <summary>
        /// Returns iVector4.rgbb swizzling (equivalent to iVector4.xyzz).
        /// </summary>
        public iVector4 rgbb => new iVector4(x, y, z, z);
        
        /// <summary>
        /// Returns iVector4.xyzw swizzling.
        /// </summary>
        public iVector4 xyzw => new iVector4(x, y, z, w);
        
        /// <summary>
        /// Returns iVector4.rgba swizzling (equivalent to iVector4.xyzw).
        /// </summary>
        public iVector4 rgba => new iVector4(x, y, z, w);
        
        /// <summary>
        /// Returns iVector4.xyw swizzling.
        /// </summary>
        public iVector3 xyw => new iVector3(x, y, w);
        
        /// <summary>
        /// Returns iVector4.rga swizzling (equivalent to iVector4.xyw).
        /// </summary>
        public iVector3 rga => new iVector3(x, y, w);
        
        /// <summary>
        /// Returns iVector4.xywx swizzling.
        /// </summary>
        public iVector4 xywx => new iVector4(x, y, w, x);
        
        /// <summary>
        /// Returns iVector4.rgar swizzling (equivalent to iVector4.xywx).
        /// </summary>
        public iVector4 rgar => new iVector4(x, y, w, x);
        
        /// <summary>
        /// Returns iVector4.xywy swizzling.
        /// </summary>
        public iVector4 xywy => new iVector4(x, y, w, y);
        
        /// <summary>
        /// Returns iVector4.rgag swizzling (equivalent to iVector4.xywy).
        /// </summary>
        public iVector4 rgag => new iVector4(x, y, w, y);
        
        /// <summary>
        /// Returns iVector4.xywz swizzling.
        /// </summary>
        public iVector4 xywz => new iVector4(x, y, w, z);
        
        /// <summary>
        /// Returns iVector4.rgab swizzling (equivalent to iVector4.xywz).
        /// </summary>
        public iVector4 rgab => new iVector4(x, y, w, z);
        
        /// <summary>
        /// Returns iVector4.xyww swizzling.
        /// </summary>
        public iVector4 xyww => new iVector4(x, y, w, w);
        
        /// <summary>
        /// Returns iVector4.rgaa swizzling (equivalent to iVector4.xyww).
        /// </summary>
        public iVector4 rgaa => new iVector4(x, y, w, w);
        
        /// <summary>
        /// Returns iVector4.xz swizzling.
        /// </summary>
        public iVector2 xz => new iVector2(x, z);
        
        /// <summary>
        /// Returns iVector4.rb swizzling (equivalent to iVector4.xz).
        /// </summary>
        public iVector2 rb => new iVector2(x, z);
        
        /// <summary>
        /// Returns iVector4.xzx swizzling.
        /// </summary>
        public iVector3 xzx => new iVector3(x, z, x);
        
        /// <summary>
        /// Returns iVector4.rbr swizzling (equivalent to iVector4.xzx).
        /// </summary>
        public iVector3 rbr => new iVector3(x, z, x);
        
        /// <summary>
        /// Returns iVector4.xzxx swizzling.
        /// </summary>
        public iVector4 xzxx => new iVector4(x, z, x, x);
        
        /// <summary>
        /// Returns iVector4.rbrr swizzling (equivalent to iVector4.xzxx).
        /// </summary>
        public iVector4 rbrr => new iVector4(x, z, x, x);
        
        /// <summary>
        /// Returns iVector4.xzxy swizzling.
        /// </summary>
        public iVector4 xzxy => new iVector4(x, z, x, y);
        
        /// <summary>
        /// Returns iVector4.rbrg swizzling (equivalent to iVector4.xzxy).
        /// </summary>
        public iVector4 rbrg => new iVector4(x, z, x, y);
        
        /// <summary>
        /// Returns iVector4.xzxz swizzling.
        /// </summary>
        public iVector4 xzxz => new iVector4(x, z, x, z);
        
        /// <summary>
        /// Returns iVector4.rbrb swizzling (equivalent to iVector4.xzxz).
        /// </summary>
        public iVector4 rbrb => new iVector4(x, z, x, z);
        
        /// <summary>
        /// Returns iVector4.xzxw swizzling.
        /// </summary>
        public iVector4 xzxw => new iVector4(x, z, x, w);
        
        /// <summary>
        /// Returns iVector4.rbra swizzling (equivalent to iVector4.xzxw).
        /// </summary>
        public iVector4 rbra => new iVector4(x, z, x, w);
        
        /// <summary>
        /// Returns iVector4.xzy swizzling.
        /// </summary>
        public iVector3 xzy => new iVector3(x, z, y);
        
        /// <summary>
        /// Returns iVector4.rbg swizzling (equivalent to iVector4.xzy).
        /// </summary>
        public iVector3 rbg => new iVector3(x, z, y);
        
        /// <summary>
        /// Returns iVector4.xzyx swizzling.
        /// </summary>
        public iVector4 xzyx => new iVector4(x, z, y, x);
        
        /// <summary>
        /// Returns iVector4.rbgr swizzling (equivalent to iVector4.xzyx).
        /// </summary>
        public iVector4 rbgr => new iVector4(x, z, y, x);
        
        /// <summary>
        /// Returns iVector4.xzyy swizzling.
        /// </summary>
        public iVector4 xzyy => new iVector4(x, z, y, y);
        
        /// <summary>
        /// Returns iVector4.rbgg swizzling (equivalent to iVector4.xzyy).
        /// </summary>
        public iVector4 rbgg => new iVector4(x, z, y, y);
        
        /// <summary>
        /// Returns iVector4.xzyz swizzling.
        /// </summary>
        public iVector4 xzyz => new iVector4(x, z, y, z);
        
        /// <summary>
        /// Returns iVector4.rbgb swizzling (equivalent to iVector4.xzyz).
        /// </summary>
        public iVector4 rbgb => new iVector4(x, z, y, z);
        
        /// <summary>
        /// Returns iVector4.xzyw swizzling.
        /// </summary>
        public iVector4 xzyw => new iVector4(x, z, y, w);
        
        /// <summary>
        /// Returns iVector4.rbga swizzling (equivalent to iVector4.xzyw).
        /// </summary>
        public iVector4 rbga => new iVector4(x, z, y, w);
        
        /// <summary>
        /// Returns iVector4.xzz swizzling.
        /// </summary>
        public iVector3 xzz => new iVector3(x, z, z);
        
        /// <summary>
        /// Returns iVector4.rbb swizzling (equivalent to iVector4.xzz).
        /// </summary>
        public iVector3 rbb => new iVector3(x, z, z);
        
        /// <summary>
        /// Returns iVector4.xzzx swizzling.
        /// </summary>
        public iVector4 xzzx => new iVector4(x, z, z, x);
        
        /// <summary>
        /// Returns iVector4.rbbr swizzling (equivalent to iVector4.xzzx).
        /// </summary>
        public iVector4 rbbr => new iVector4(x, z, z, x);
        
        /// <summary>
        /// Returns iVector4.xzzy swizzling.
        /// </summary>
        public iVector4 xzzy => new iVector4(x, z, z, y);
        
        /// <summary>
        /// Returns iVector4.rbbg swizzling (equivalent to iVector4.xzzy).
        /// </summary>
        public iVector4 rbbg => new iVector4(x, z, z, y);
        
        /// <summary>
        /// Returns iVector4.xzzz swizzling.
        /// </summary>
        public iVector4 xzzz => new iVector4(x, z, z, z);
        
        /// <summary>
        /// Returns iVector4.rbbb swizzling (equivalent to iVector4.xzzz).
        /// </summary>
        public iVector4 rbbb => new iVector4(x, z, z, z);
        
        /// <summary>
        /// Returns iVector4.xzzw swizzling.
        /// </summary>
        public iVector4 xzzw => new iVector4(x, z, z, w);
        
        /// <summary>
        /// Returns iVector4.rbba swizzling (equivalent to iVector4.xzzw).
        /// </summary>
        public iVector4 rbba => new iVector4(x, z, z, w);
        
        /// <summary>
        /// Returns iVector4.xzw swizzling.
        /// </summary>
        public iVector3 xzw => new iVector3(x, z, w);
        
        /// <summary>
        /// Returns iVector4.rba swizzling (equivalent to iVector4.xzw).
        /// </summary>
        public iVector3 rba => new iVector3(x, z, w);
        
        /// <summary>
        /// Returns iVector4.xzwx swizzling.
        /// </summary>
        public iVector4 xzwx => new iVector4(x, z, w, x);
        
        /// <summary>
        /// Returns iVector4.rbar swizzling (equivalent to iVector4.xzwx).
        /// </summary>
        public iVector4 rbar => new iVector4(x, z, w, x);
        
        /// <summary>
        /// Returns iVector4.xzwy swizzling.
        /// </summary>
        public iVector4 xzwy => new iVector4(x, z, w, y);
        
        /// <summary>
        /// Returns iVector4.rbag swizzling (equivalent to iVector4.xzwy).
        /// </summary>
        public iVector4 rbag => new iVector4(x, z, w, y);
        
        /// <summary>
        /// Returns iVector4.xzwz swizzling.
        /// </summary>
        public iVector4 xzwz => new iVector4(x, z, w, z);
        
        /// <summary>
        /// Returns iVector4.rbab swizzling (equivalent to iVector4.xzwz).
        /// </summary>
        public iVector4 rbab => new iVector4(x, z, w, z);
        
        /// <summary>
        /// Returns iVector4.xzww swizzling.
        /// </summary>
        public iVector4 xzww => new iVector4(x, z, w, w);
        
        /// <summary>
        /// Returns iVector4.rbaa swizzling (equivalent to iVector4.xzww).
        /// </summary>
        public iVector4 rbaa => new iVector4(x, z, w, w);
        
        /// <summary>
        /// Returns iVector4.xw swizzling.
        /// </summary>
        public iVector2 xw => new iVector2(x, w);
        
        /// <summary>
        /// Returns iVector4.ra swizzling (equivalent to iVector4.xw).
        /// </summary>
        public iVector2 ra => new iVector2(x, w);
        
        /// <summary>
        /// Returns iVector4.xwx swizzling.
        /// </summary>
        public iVector3 xwx => new iVector3(x, w, x);
        
        /// <summary>
        /// Returns iVector4.rar swizzling (equivalent to iVector4.xwx).
        /// </summary>
        public iVector3 rar => new iVector3(x, w, x);
        
        /// <summary>
        /// Returns iVector4.xwxx swizzling.
        /// </summary>
        public iVector4 xwxx => new iVector4(x, w, x, x);
        
        /// <summary>
        /// Returns iVector4.rarr swizzling (equivalent to iVector4.xwxx).
        /// </summary>
        public iVector4 rarr => new iVector4(x, w, x, x);
        
        /// <summary>
        /// Returns iVector4.xwxy swizzling.
        /// </summary>
        public iVector4 xwxy => new iVector4(x, w, x, y);
        
        /// <summary>
        /// Returns iVector4.rarg swizzling (equivalent to iVector4.xwxy).
        /// </summary>
        public iVector4 rarg => new iVector4(x, w, x, y);
        
        /// <summary>
        /// Returns iVector4.xwxz swizzling.
        /// </summary>
        public iVector4 xwxz => new iVector4(x, w, x, z);
        
        /// <summary>
        /// Returns iVector4.rarb swizzling (equivalent to iVector4.xwxz).
        /// </summary>
        public iVector4 rarb => new iVector4(x, w, x, z);
        
        /// <summary>
        /// Returns iVector4.xwxw swizzling.
        /// </summary>
        public iVector4 xwxw => new iVector4(x, w, x, w);
        
        /// <summary>
        /// Returns iVector4.rara swizzling (equivalent to iVector4.xwxw).
        /// </summary>
        public iVector4 rara => new iVector4(x, w, x, w);
        
        /// <summary>
        /// Returns iVector4.xwy swizzling.
        /// </summary>
        public iVector3 xwy => new iVector3(x, w, y);
        
        /// <summary>
        /// Returns iVector4.rag swizzling (equivalent to iVector4.xwy).
        /// </summary>
        public iVector3 rag => new iVector3(x, w, y);
        
        /// <summary>
        /// Returns iVector4.xwyx swizzling.
        /// </summary>
        public iVector4 xwyx => new iVector4(x, w, y, x);
        
        /// <summary>
        /// Returns iVector4.ragr swizzling (equivalent to iVector4.xwyx).
        /// </summary>
        public iVector4 ragr => new iVector4(x, w, y, x);
        
        /// <summary>
        /// Returns iVector4.xwyy swizzling.
        /// </summary>
        public iVector4 xwyy => new iVector4(x, w, y, y);
        
        /// <summary>
        /// Returns iVector4.ragg swizzling (equivalent to iVector4.xwyy).
        /// </summary>
        public iVector4 ragg => new iVector4(x, w, y, y);
        
        /// <summary>
        /// Returns iVector4.xwyz swizzling.
        /// </summary>
        public iVector4 xwyz => new iVector4(x, w, y, z);
        
        /// <summary>
        /// Returns iVector4.ragb swizzling (equivalent to iVector4.xwyz).
        /// </summary>
        public iVector4 ragb => new iVector4(x, w, y, z);
        
        /// <summary>
        /// Returns iVector4.xwyw swizzling.
        /// </summary>
        public iVector4 xwyw => new iVector4(x, w, y, w);
        
        /// <summary>
        /// Returns iVector4.raga swizzling (equivalent to iVector4.xwyw).
        /// </summary>
        public iVector4 raga => new iVector4(x, w, y, w);
        
        /// <summary>
        /// Returns iVector4.xwz swizzling.
        /// </summary>
        public iVector3 xwz => new iVector3(x, w, z);
        
        /// <summary>
        /// Returns iVector4.rab swizzling (equivalent to iVector4.xwz).
        /// </summary>
        public iVector3 rab => new iVector3(x, w, z);
        
        /// <summary>
        /// Returns iVector4.xwzx swizzling.
        /// </summary>
        public iVector4 xwzx => new iVector4(x, w, z, x);
        
        /// <summary>
        /// Returns iVector4.rabr swizzling (equivalent to iVector4.xwzx).
        /// </summary>
        public iVector4 rabr => new iVector4(x, w, z, x);
        
        /// <summary>
        /// Returns iVector4.xwzy swizzling.
        /// </summary>
        public iVector4 xwzy => new iVector4(x, w, z, y);
        
        /// <summary>
        /// Returns iVector4.rabg swizzling (equivalent to iVector4.xwzy).
        /// </summary>
        public iVector4 rabg => new iVector4(x, w, z, y);
        
        /// <summary>
        /// Returns iVector4.xwzz swizzling.
        /// </summary>
        public iVector4 xwzz => new iVector4(x, w, z, z);
        
        /// <summary>
        /// Returns iVector4.rabb swizzling (equivalent to iVector4.xwzz).
        /// </summary>
        public iVector4 rabb => new iVector4(x, w, z, z);
        
        /// <summary>
        /// Returns iVector4.xwzw swizzling.
        /// </summary>
        public iVector4 xwzw => new iVector4(x, w, z, w);
        
        /// <summary>
        /// Returns iVector4.raba swizzling (equivalent to iVector4.xwzw).
        /// </summary>
        public iVector4 raba => new iVector4(x, w, z, w);
        
        /// <summary>
        /// Returns iVector4.xww swizzling.
        /// </summary>
        public iVector3 xww => new iVector3(x, w, w);
        
        /// <summary>
        /// Returns iVector4.raa swizzling (equivalent to iVector4.xww).
        /// </summary>
        public iVector3 raa => new iVector3(x, w, w);
        
        /// <summary>
        /// Returns iVector4.xwwx swizzling.
        /// </summary>
        public iVector4 xwwx => new iVector4(x, w, w, x);
        
        /// <summary>
        /// Returns iVector4.raar swizzling (equivalent to iVector4.xwwx).
        /// </summary>
        public iVector4 raar => new iVector4(x, w, w, x);
        
        /// <summary>
        /// Returns iVector4.xwwy swizzling.
        /// </summary>
        public iVector4 xwwy => new iVector4(x, w, w, y);
        
        /// <summary>
        /// Returns iVector4.raag swizzling (equivalent to iVector4.xwwy).
        /// </summary>
        public iVector4 raag => new iVector4(x, w, w, y);
        
        /// <summary>
        /// Returns iVector4.xwwz swizzling.
        /// </summary>
        public iVector4 xwwz => new iVector4(x, w, w, z);
        
        /// <summary>
        /// Returns iVector4.raab swizzling (equivalent to iVector4.xwwz).
        /// </summary>
        public iVector4 raab => new iVector4(x, w, w, z);
        
        /// <summary>
        /// Returns iVector4.xwww swizzling.
        /// </summary>
        public iVector4 xwww => new iVector4(x, w, w, w);
        
        /// <summary>
        /// Returns iVector4.raaa swizzling (equivalent to iVector4.xwww).
        /// </summary>
        public iVector4 raaa => new iVector4(x, w, w, w);
        
        /// <summary>
        /// Returns iVector4.yx swizzling.
        /// </summary>
        public iVector2 yx => new iVector2(y, x);
        
        /// <summary>
        /// Returns iVector4.gr swizzling (equivalent to iVector4.yx).
        /// </summary>
        public iVector2 gr => new iVector2(y, x);
        
        /// <summary>
        /// Returns iVector4.yxx swizzling.
        /// </summary>
        public iVector3 yxx => new iVector3(y, x, x);
        
        /// <summary>
        /// Returns iVector4.grr swizzling (equivalent to iVector4.yxx).
        /// </summary>
        public iVector3 grr => new iVector3(y, x, x);
        
        /// <summary>
        /// Returns iVector4.yxxx swizzling.
        /// </summary>
        public iVector4 yxxx => new iVector4(y, x, x, x);
        
        /// <summary>
        /// Returns iVector4.grrr swizzling (equivalent to iVector4.yxxx).
        /// </summary>
        public iVector4 grrr => new iVector4(y, x, x, x);
        
        /// <summary>
        /// Returns iVector4.yxxy swizzling.
        /// </summary>
        public iVector4 yxxy => new iVector4(y, x, x, y);
        
        /// <summary>
        /// Returns iVector4.grrg swizzling (equivalent to iVector4.yxxy).
        /// </summary>
        public iVector4 grrg => new iVector4(y, x, x, y);
        
        /// <summary>
        /// Returns iVector4.yxxz swizzling.
        /// </summary>
        public iVector4 yxxz => new iVector4(y, x, x, z);
        
        /// <summary>
        /// Returns iVector4.grrb swizzling (equivalent to iVector4.yxxz).
        /// </summary>
        public iVector4 grrb => new iVector4(y, x, x, z);
        
        /// <summary>
        /// Returns iVector4.yxxw swizzling.
        /// </summary>
        public iVector4 yxxw => new iVector4(y, x, x, w);
        
        /// <summary>
        /// Returns iVector4.grra swizzling (equivalent to iVector4.yxxw).
        /// </summary>
        public iVector4 grra => new iVector4(y, x, x, w);
        
        /// <summary>
        /// Returns iVector4.yxy swizzling.
        /// </summary>
        public iVector3 yxy => new iVector3(y, x, y);
        
        /// <summary>
        /// Returns iVector4.grg swizzling (equivalent to iVector4.yxy).
        /// </summary>
        public iVector3 grg => new iVector3(y, x, y);
        
        /// <summary>
        /// Returns iVector4.yxyx swizzling.
        /// </summary>
        public iVector4 yxyx => new iVector4(y, x, y, x);
        
        /// <summary>
        /// Returns iVector4.grgr swizzling (equivalent to iVector4.yxyx).
        /// </summary>
        public iVector4 grgr => new iVector4(y, x, y, x);
        
        /// <summary>
        /// Returns iVector4.yxyy swizzling.
        /// </summary>
        public iVector4 yxyy => new iVector4(y, x, y, y);
        
        /// <summary>
        /// Returns iVector4.grgg swizzling (equivalent to iVector4.yxyy).
        /// </summary>
        public iVector4 grgg => new iVector4(y, x, y, y);
        
        /// <summary>
        /// Returns iVector4.yxyz swizzling.
        /// </summary>
        public iVector4 yxyz => new iVector4(y, x, y, z);
        
        /// <summary>
        /// Returns iVector4.grgb swizzling (equivalent to iVector4.yxyz).
        /// </summary>
        public iVector4 grgb => new iVector4(y, x, y, z);
        
        /// <summary>
        /// Returns iVector4.yxyw swizzling.
        /// </summary>
        public iVector4 yxyw => new iVector4(y, x, y, w);
        
        /// <summary>
        /// Returns iVector4.grga swizzling (equivalent to iVector4.yxyw).
        /// </summary>
        public iVector4 grga => new iVector4(y, x, y, w);
        
        /// <summary>
        /// Returns iVector4.yxz swizzling.
        /// </summary>
        public iVector3 yxz => new iVector3(y, x, z);
        
        /// <summary>
        /// Returns iVector4.grb swizzling (equivalent to iVector4.yxz).
        /// </summary>
        public iVector3 grb => new iVector3(y, x, z);
        
        /// <summary>
        /// Returns iVector4.yxzx swizzling.
        /// </summary>
        public iVector4 yxzx => new iVector4(y, x, z, x);
        
        /// <summary>
        /// Returns iVector4.grbr swizzling (equivalent to iVector4.yxzx).
        /// </summary>
        public iVector4 grbr => new iVector4(y, x, z, x);
        
        /// <summary>
        /// Returns iVector4.yxzy swizzling.
        /// </summary>
        public iVector4 yxzy => new iVector4(y, x, z, y);
        
        /// <summary>
        /// Returns iVector4.grbg swizzling (equivalent to iVector4.yxzy).
        /// </summary>
        public iVector4 grbg => new iVector4(y, x, z, y);
        
        /// <summary>
        /// Returns iVector4.yxzz swizzling.
        /// </summary>
        public iVector4 yxzz => new iVector4(y, x, z, z);
        
        /// <summary>
        /// Returns iVector4.grbb swizzling (equivalent to iVector4.yxzz).
        /// </summary>
        public iVector4 grbb => new iVector4(y, x, z, z);
        
        /// <summary>
        /// Returns iVector4.yxzw swizzling.
        /// </summary>
        public iVector4 yxzw => new iVector4(y, x, z, w);
        
        /// <summary>
        /// Returns iVector4.grba swizzling (equivalent to iVector4.yxzw).
        /// </summary>
        public iVector4 grba => new iVector4(y, x, z, w);
        
        /// <summary>
        /// Returns iVector4.yxw swizzling.
        /// </summary>
        public iVector3 yxw => new iVector3(y, x, w);
        
        /// <summary>
        /// Returns iVector4.gra swizzling (equivalent to iVector4.yxw).
        /// </summary>
        public iVector3 gra => new iVector3(y, x, w);
        
        /// <summary>
        /// Returns iVector4.yxwx swizzling.
        /// </summary>
        public iVector4 yxwx => new iVector4(y, x, w, x);
        
        /// <summary>
        /// Returns iVector4.grar swizzling (equivalent to iVector4.yxwx).
        /// </summary>
        public iVector4 grar => new iVector4(y, x, w, x);
        
        /// <summary>
        /// Returns iVector4.yxwy swizzling.
        /// </summary>
        public iVector4 yxwy => new iVector4(y, x, w, y);
        
        /// <summary>
        /// Returns iVector4.grag swizzling (equivalent to iVector4.yxwy).
        /// </summary>
        public iVector4 grag => new iVector4(y, x, w, y);
        
        /// <summary>
        /// Returns iVector4.yxwz swizzling.
        /// </summary>
        public iVector4 yxwz => new iVector4(y, x, w, z);
        
        /// <summary>
        /// Returns iVector4.grab swizzling (equivalent to iVector4.yxwz).
        /// </summary>
        public iVector4 grab => new iVector4(y, x, w, z);
        
        /// <summary>
        /// Returns iVector4.yxww swizzling.
        /// </summary>
        public iVector4 yxww => new iVector4(y, x, w, w);
        
        /// <summary>
        /// Returns iVector4.graa swizzling (equivalent to iVector4.yxww).
        /// </summary>
        public iVector4 graa => new iVector4(y, x, w, w);
        
        /// <summary>
        /// Returns iVector4.yy swizzling.
        /// </summary>
        public iVector2 yy => new iVector2(y, y);
        
        /// <summary>
        /// Returns iVector4.gg swizzling (equivalent to iVector4.yy).
        /// </summary>
        public iVector2 gg => new iVector2(y, y);
        
        /// <summary>
        /// Returns iVector4.yyx swizzling.
        /// </summary>
        public iVector3 yyx => new iVector3(y, y, x);
        
        /// <summary>
        /// Returns iVector4.ggr swizzling (equivalent to iVector4.yyx).
        /// </summary>
        public iVector3 ggr => new iVector3(y, y, x);
        
        /// <summary>
        /// Returns iVector4.yyxx swizzling.
        /// </summary>
        public iVector4 yyxx => new iVector4(y, y, x, x);
        
        /// <summary>
        /// Returns iVector4.ggrr swizzling (equivalent to iVector4.yyxx).
        /// </summary>
        public iVector4 ggrr => new iVector4(y, y, x, x);
        
        /// <summary>
        /// Returns iVector4.yyxy swizzling.
        /// </summary>
        public iVector4 yyxy => new iVector4(y, y, x, y);
        
        /// <summary>
        /// Returns iVector4.ggrg swizzling (equivalent to iVector4.yyxy).
        /// </summary>
        public iVector4 ggrg => new iVector4(y, y, x, y);
        
        /// <summary>
        /// Returns iVector4.yyxz swizzling.
        /// </summary>
        public iVector4 yyxz => new iVector4(y, y, x, z);
        
        /// <summary>
        /// Returns iVector4.ggrb swizzling (equivalent to iVector4.yyxz).
        /// </summary>
        public iVector4 ggrb => new iVector4(y, y, x, z);
        
        /// <summary>
        /// Returns iVector4.yyxw swizzling.
        /// </summary>
        public iVector4 yyxw => new iVector4(y, y, x, w);
        
        /// <summary>
        /// Returns iVector4.ggra swizzling (equivalent to iVector4.yyxw).
        /// </summary>
        public iVector4 ggra => new iVector4(y, y, x, w);
        
        /// <summary>
        /// Returns iVector4.yyy swizzling.
        /// </summary>
        public iVector3 yyy => new iVector3(y, y, y);
        
        /// <summary>
        /// Returns iVector4.ggg swizzling (equivalent to iVector4.yyy).
        /// </summary>
        public iVector3 ggg => new iVector3(y, y, y);
        
        /// <summary>
        /// Returns iVector4.yyyx swizzling.
        /// </summary>
        public iVector4 yyyx => new iVector4(y, y, y, x);
        
        /// <summary>
        /// Returns iVector4.gggr swizzling (equivalent to iVector4.yyyx).
        /// </summary>
        public iVector4 gggr => new iVector4(y, y, y, x);
        
        /// <summary>
        /// Returns iVector4.yyyy swizzling.
        /// </summary>
        public iVector4 yyyy => new iVector4(y, y, y, y);
        
        /// <summary>
        /// Returns iVector4.gggg swizzling (equivalent to iVector4.yyyy).
        /// </summary>
        public iVector4 gggg => new iVector4(y, y, y, y);
        
        /// <summary>
        /// Returns iVector4.yyyz swizzling.
        /// </summary>
        public iVector4 yyyz => new iVector4(y, y, y, z);
        
        /// <summary>
        /// Returns iVector4.gggb swizzling (equivalent to iVector4.yyyz).
        /// </summary>
        public iVector4 gggb => new iVector4(y, y, y, z);
        
        /// <summary>
        /// Returns iVector4.yyyw swizzling.
        /// </summary>
        public iVector4 yyyw => new iVector4(y, y, y, w);
        
        /// <summary>
        /// Returns iVector4.ggga swizzling (equivalent to iVector4.yyyw).
        /// </summary>
        public iVector4 ggga => new iVector4(y, y, y, w);
        
        /// <summary>
        /// Returns iVector4.yyz swizzling.
        /// </summary>
        public iVector3 yyz => new iVector3(y, y, z);
        
        /// <summary>
        /// Returns iVector4.ggb swizzling (equivalent to iVector4.yyz).
        /// </summary>
        public iVector3 ggb => new iVector3(y, y, z);
        
        /// <summary>
        /// Returns iVector4.yyzx swizzling.
        /// </summary>
        public iVector4 yyzx => new iVector4(y, y, z, x);
        
        /// <summary>
        /// Returns iVector4.ggbr swizzling (equivalent to iVector4.yyzx).
        /// </summary>
        public iVector4 ggbr => new iVector4(y, y, z, x);
        
        /// <summary>
        /// Returns iVector4.yyzy swizzling.
        /// </summary>
        public iVector4 yyzy => new iVector4(y, y, z, y);
        
        /// <summary>
        /// Returns iVector4.ggbg swizzling (equivalent to iVector4.yyzy).
        /// </summary>
        public iVector4 ggbg => new iVector4(y, y, z, y);
        
        /// <summary>
        /// Returns iVector4.yyzz swizzling.
        /// </summary>
        public iVector4 yyzz => new iVector4(y, y, z, z);
        
        /// <summary>
        /// Returns iVector4.ggbb swizzling (equivalent to iVector4.yyzz).
        /// </summary>
        public iVector4 ggbb => new iVector4(y, y, z, z);
        
        /// <summary>
        /// Returns iVector4.yyzw swizzling.
        /// </summary>
        public iVector4 yyzw => new iVector4(y, y, z, w);
        
        /// <summary>
        /// Returns iVector4.ggba swizzling (equivalent to iVector4.yyzw).
        /// </summary>
        public iVector4 ggba => new iVector4(y, y, z, w);
        
        /// <summary>
        /// Returns iVector4.yyw swizzling.
        /// </summary>
        public iVector3 yyw => new iVector3(y, y, w);
        
        /// <summary>
        /// Returns iVector4.gga swizzling (equivalent to iVector4.yyw).
        /// </summary>
        public iVector3 gga => new iVector3(y, y, w);
        
        /// <summary>
        /// Returns iVector4.yywx swizzling.
        /// </summary>
        public iVector4 yywx => new iVector4(y, y, w, x);
        
        /// <summary>
        /// Returns iVector4.ggar swizzling (equivalent to iVector4.yywx).
        /// </summary>
        public iVector4 ggar => new iVector4(y, y, w, x);
        
        /// <summary>
        /// Returns iVector4.yywy swizzling.
        /// </summary>
        public iVector4 yywy => new iVector4(y, y, w, y);
        
        /// <summary>
        /// Returns iVector4.ggag swizzling (equivalent to iVector4.yywy).
        /// </summary>
        public iVector4 ggag => new iVector4(y, y, w, y);
        
        /// <summary>
        /// Returns iVector4.yywz swizzling.
        /// </summary>
        public iVector4 yywz => new iVector4(y, y, w, z);
        
        /// <summary>
        /// Returns iVector4.ggab swizzling (equivalent to iVector4.yywz).
        /// </summary>
        public iVector4 ggab => new iVector4(y, y, w, z);
        
        /// <summary>
        /// Returns iVector4.yyww swizzling.
        /// </summary>
        public iVector4 yyww => new iVector4(y, y, w, w);
        
        /// <summary>
        /// Returns iVector4.ggaa swizzling (equivalent to iVector4.yyww).
        /// </summary>
        public iVector4 ggaa => new iVector4(y, y, w, w);
        
        /// <summary>
        /// Returns iVector4.yz swizzling.
        /// </summary>
        public iVector2 yz => new iVector2(y, z);
        
        /// <summary>
        /// Returns iVector4.gb swizzling (equivalent to iVector4.yz).
        /// </summary>
        public iVector2 gb => new iVector2(y, z);
        
        /// <summary>
        /// Returns iVector4.yzx swizzling.
        /// </summary>
        public iVector3 yzx => new iVector3(y, z, x);
        
        /// <summary>
        /// Returns iVector4.gbr swizzling (equivalent to iVector4.yzx).
        /// </summary>
        public iVector3 gbr => new iVector3(y, z, x);
        
        /// <summary>
        /// Returns iVector4.yzxx swizzling.
        /// </summary>
        public iVector4 yzxx => new iVector4(y, z, x, x);
        
        /// <summary>
        /// Returns iVector4.gbrr swizzling (equivalent to iVector4.yzxx).
        /// </summary>
        public iVector4 gbrr => new iVector4(y, z, x, x);
        
        /// <summary>
        /// Returns iVector4.yzxy swizzling.
        /// </summary>
        public iVector4 yzxy => new iVector4(y, z, x, y);
        
        /// <summary>
        /// Returns iVector4.gbrg swizzling (equivalent to iVector4.yzxy).
        /// </summary>
        public iVector4 gbrg => new iVector4(y, z, x, y);
        
        /// <summary>
        /// Returns iVector4.yzxz swizzling.
        /// </summary>
        public iVector4 yzxz => new iVector4(y, z, x, z);
        
        /// <summary>
        /// Returns iVector4.gbrb swizzling (equivalent to iVector4.yzxz).
        /// </summary>
        public iVector4 gbrb => new iVector4(y, z, x, z);
        
        /// <summary>
        /// Returns iVector4.yzxw swizzling.
        /// </summary>
        public iVector4 yzxw => new iVector4(y, z, x, w);
        
        /// <summary>
        /// Returns iVector4.gbra swizzling (equivalent to iVector4.yzxw).
        /// </summary>
        public iVector4 gbra => new iVector4(y, z, x, w);
        
        /// <summary>
        /// Returns iVector4.yzy swizzling.
        /// </summary>
        public iVector3 yzy => new iVector3(y, z, y);
        
        /// <summary>
        /// Returns iVector4.gbg swizzling (equivalent to iVector4.yzy).
        /// </summary>
        public iVector3 gbg => new iVector3(y, z, y);
        
        /// <summary>
        /// Returns iVector4.yzyx swizzling.
        /// </summary>
        public iVector4 yzyx => new iVector4(y, z, y, x);
        
        /// <summary>
        /// Returns iVector4.gbgr swizzling (equivalent to iVector4.yzyx).
        /// </summary>
        public iVector4 gbgr => new iVector4(y, z, y, x);
        
        /// <summary>
        /// Returns iVector4.yzyy swizzling.
        /// </summary>
        public iVector4 yzyy => new iVector4(y, z, y, y);
        
        /// <summary>
        /// Returns iVector4.gbgg swizzling (equivalent to iVector4.yzyy).
        /// </summary>
        public iVector4 gbgg => new iVector4(y, z, y, y);
        
        /// <summary>
        /// Returns iVector4.yzyz swizzling.
        /// </summary>
        public iVector4 yzyz => new iVector4(y, z, y, z);
        
        /// <summary>
        /// Returns iVector4.gbgb swizzling (equivalent to iVector4.yzyz).
        /// </summary>
        public iVector4 gbgb => new iVector4(y, z, y, z);
        
        /// <summary>
        /// Returns iVector4.yzyw swizzling.
        /// </summary>
        public iVector4 yzyw => new iVector4(y, z, y, w);
        
        /// <summary>
        /// Returns iVector4.gbga swizzling (equivalent to iVector4.yzyw).
        /// </summary>
        public iVector4 gbga => new iVector4(y, z, y, w);
        
        /// <summary>
        /// Returns iVector4.yzz swizzling.
        /// </summary>
        public iVector3 yzz => new iVector3(y, z, z);
        
        /// <summary>
        /// Returns iVector4.gbb swizzling (equivalent to iVector4.yzz).
        /// </summary>
        public iVector3 gbb => new iVector3(y, z, z);
        
        /// <summary>
        /// Returns iVector4.yzzx swizzling.
        /// </summary>
        public iVector4 yzzx => new iVector4(y, z, z, x);
        
        /// <summary>
        /// Returns iVector4.gbbr swizzling (equivalent to iVector4.yzzx).
        /// </summary>
        public iVector4 gbbr => new iVector4(y, z, z, x);
        
        /// <summary>
        /// Returns iVector4.yzzy swizzling.
        /// </summary>
        public iVector4 yzzy => new iVector4(y, z, z, y);
        
        /// <summary>
        /// Returns iVector4.gbbg swizzling (equivalent to iVector4.yzzy).
        /// </summary>
        public iVector4 gbbg => new iVector4(y, z, z, y);
        
        /// <summary>
        /// Returns iVector4.yzzz swizzling.
        /// </summary>
        public iVector4 yzzz => new iVector4(y, z, z, z);
        
        /// <summary>
        /// Returns iVector4.gbbb swizzling (equivalent to iVector4.yzzz).
        /// </summary>
        public iVector4 gbbb => new iVector4(y, z, z, z);
        
        /// <summary>
        /// Returns iVector4.yzzw swizzling.
        /// </summary>
        public iVector4 yzzw => new iVector4(y, z, z, w);
        
        /// <summary>
        /// Returns iVector4.gbba swizzling (equivalent to iVector4.yzzw).
        /// </summary>
        public iVector4 gbba => new iVector4(y, z, z, w);
        
        /// <summary>
        /// Returns iVector4.yzw swizzling.
        /// </summary>
        public iVector3 yzw => new iVector3(y, z, w);
        
        /// <summary>
        /// Returns iVector4.gba swizzling (equivalent to iVector4.yzw).
        /// </summary>
        public iVector3 gba => new iVector3(y, z, w);
        
        /// <summary>
        /// Returns iVector4.yzwx swizzling.
        /// </summary>
        public iVector4 yzwx => new iVector4(y, z, w, x);
        
        /// <summary>
        /// Returns iVector4.gbar swizzling (equivalent to iVector4.yzwx).
        /// </summary>
        public iVector4 gbar => new iVector4(y, z, w, x);
        
        /// <summary>
        /// Returns iVector4.yzwy swizzling.
        /// </summary>
        public iVector4 yzwy => new iVector4(y, z, w, y);
        
        /// <summary>
        /// Returns iVector4.gbag swizzling (equivalent to iVector4.yzwy).
        /// </summary>
        public iVector4 gbag => new iVector4(y, z, w, y);
        
        /// <summary>
        /// Returns iVector4.yzwz swizzling.
        /// </summary>
        public iVector4 yzwz => new iVector4(y, z, w, z);
        
        /// <summary>
        /// Returns iVector4.gbab swizzling (equivalent to iVector4.yzwz).
        /// </summary>
        public iVector4 gbab => new iVector4(y, z, w, z);
        
        /// <summary>
        /// Returns iVector4.yzww swizzling.
        /// </summary>
        public iVector4 yzww => new iVector4(y, z, w, w);
        
        /// <summary>
        /// Returns iVector4.gbaa swizzling (equivalent to iVector4.yzww).
        /// </summary>
        public iVector4 gbaa => new iVector4(y, z, w, w);
        
        /// <summary>
        /// Returns iVector4.yw swizzling.
        /// </summary>
        public iVector2 yw => new iVector2(y, w);
        
        /// <summary>
        /// Returns iVector4.ga swizzling (equivalent to iVector4.yw).
        /// </summary>
        public iVector2 ga => new iVector2(y, w);
        
        /// <summary>
        /// Returns iVector4.ywx swizzling.
        /// </summary>
        public iVector3 ywx => new iVector3(y, w, x);
        
        /// <summary>
        /// Returns iVector4.gar swizzling (equivalent to iVector4.ywx).
        /// </summary>
        public iVector3 gar => new iVector3(y, w, x);
        
        /// <summary>
        /// Returns iVector4.ywxx swizzling.
        /// </summary>
        public iVector4 ywxx => new iVector4(y, w, x, x);
        
        /// <summary>
        /// Returns iVector4.garr swizzling (equivalent to iVector4.ywxx).
        /// </summary>
        public iVector4 garr => new iVector4(y, w, x, x);
        
        /// <summary>
        /// Returns iVector4.ywxy swizzling.
        /// </summary>
        public iVector4 ywxy => new iVector4(y, w, x, y);
        
        /// <summary>
        /// Returns iVector4.garg swizzling (equivalent to iVector4.ywxy).
        /// </summary>
        public iVector4 garg => new iVector4(y, w, x, y);
        
        /// <summary>
        /// Returns iVector4.ywxz swizzling.
        /// </summary>
        public iVector4 ywxz => new iVector4(y, w, x, z);
        
        /// <summary>
        /// Returns iVector4.garb swizzling (equivalent to iVector4.ywxz).
        /// </summary>
        public iVector4 garb => new iVector4(y, w, x, z);
        
        /// <summary>
        /// Returns iVector4.ywxw swizzling.
        /// </summary>
        public iVector4 ywxw => new iVector4(y, w, x, w);
        
        /// <summary>
        /// Returns iVector4.gara swizzling (equivalent to iVector4.ywxw).
        /// </summary>
        public iVector4 gara => new iVector4(y, w, x, w);
        
        /// <summary>
        /// Returns iVector4.ywy swizzling.
        /// </summary>
        public iVector3 ywy => new iVector3(y, w, y);
        
        /// <summary>
        /// Returns iVector4.gag swizzling (equivalent to iVector4.ywy).
        /// </summary>
        public iVector3 gag => new iVector3(y, w, y);
        
        /// <summary>
        /// Returns iVector4.ywyx swizzling.
        /// </summary>
        public iVector4 ywyx => new iVector4(y, w, y, x);
        
        /// <summary>
        /// Returns iVector4.gagr swizzling (equivalent to iVector4.ywyx).
        /// </summary>
        public iVector4 gagr => new iVector4(y, w, y, x);
        
        /// <summary>
        /// Returns iVector4.ywyy swizzling.
        /// </summary>
        public iVector4 ywyy => new iVector4(y, w, y, y);
        
        /// <summary>
        /// Returns iVector4.gagg swizzling (equivalent to iVector4.ywyy).
        /// </summary>
        public iVector4 gagg => new iVector4(y, w, y, y);
        
        /// <summary>
        /// Returns iVector4.ywyz swizzling.
        /// </summary>
        public iVector4 ywyz => new iVector4(y, w, y, z);
        
        /// <summary>
        /// Returns iVector4.gagb swizzling (equivalent to iVector4.ywyz).
        /// </summary>
        public iVector4 gagb => new iVector4(y, w, y, z);
        
        /// <summary>
        /// Returns iVector4.ywyw swizzling.
        /// </summary>
        public iVector4 ywyw => new iVector4(y, w, y, w);
        
        /// <summary>
        /// Returns iVector4.gaga swizzling (equivalent to iVector4.ywyw).
        /// </summary>
        public iVector4 gaga => new iVector4(y, w, y, w);
        
        /// <summary>
        /// Returns iVector4.ywz swizzling.
        /// </summary>
        public iVector3 ywz => new iVector3(y, w, z);
        
        /// <summary>
        /// Returns iVector4.gab swizzling (equivalent to iVector4.ywz).
        /// </summary>
        public iVector3 gab => new iVector3(y, w, z);
        
        /// <summary>
        /// Returns iVector4.ywzx swizzling.
        /// </summary>
        public iVector4 ywzx => new iVector4(y, w, z, x);
        
        /// <summary>
        /// Returns iVector4.gabr swizzling (equivalent to iVector4.ywzx).
        /// </summary>
        public iVector4 gabr => new iVector4(y, w, z, x);
        
        /// <summary>
        /// Returns iVector4.ywzy swizzling.
        /// </summary>
        public iVector4 ywzy => new iVector4(y, w, z, y);
        
        /// <summary>
        /// Returns iVector4.gabg swizzling (equivalent to iVector4.ywzy).
        /// </summary>
        public iVector4 gabg => new iVector4(y, w, z, y);
        
        /// <summary>
        /// Returns iVector4.ywzz swizzling.
        /// </summary>
        public iVector4 ywzz => new iVector4(y, w, z, z);
        
        /// <summary>
        /// Returns iVector4.gabb swizzling (equivalent to iVector4.ywzz).
        /// </summary>
        public iVector4 gabb => new iVector4(y, w, z, z);
        
        /// <summary>
        /// Returns iVector4.ywzw swizzling.
        /// </summary>
        public iVector4 ywzw => new iVector4(y, w, z, w);
        
        /// <summary>
        /// Returns iVector4.gaba swizzling (equivalent to iVector4.ywzw).
        /// </summary>
        public iVector4 gaba => new iVector4(y, w, z, w);
        
        /// <summary>
        /// Returns iVector4.yww swizzling.
        /// </summary>
        public iVector3 yww => new iVector3(y, w, w);
        
        /// <summary>
        /// Returns iVector4.gaa swizzling (equivalent to iVector4.yww).
        /// </summary>
        public iVector3 gaa => new iVector3(y, w, w);
        
        /// <summary>
        /// Returns iVector4.ywwx swizzling.
        /// </summary>
        public iVector4 ywwx => new iVector4(y, w, w, x);
        
        /// <summary>
        /// Returns iVector4.gaar swizzling (equivalent to iVector4.ywwx).
        /// </summary>
        public iVector4 gaar => new iVector4(y, w, w, x);
        
        /// <summary>
        /// Returns iVector4.ywwy swizzling.
        /// </summary>
        public iVector4 ywwy => new iVector4(y, w, w, y);
        
        /// <summary>
        /// Returns iVector4.gaag swizzling (equivalent to iVector4.ywwy).
        /// </summary>
        public iVector4 gaag => new iVector4(y, w, w, y);
        
        /// <summary>
        /// Returns iVector4.ywwz swizzling.
        /// </summary>
        public iVector4 ywwz => new iVector4(y, w, w, z);
        
        /// <summary>
        /// Returns iVector4.gaab swizzling (equivalent to iVector4.ywwz).
        /// </summary>
        public iVector4 gaab => new iVector4(y, w, w, z);
        
        /// <summary>
        /// Returns iVector4.ywww swizzling.
        /// </summary>
        public iVector4 ywww => new iVector4(y, w, w, w);
        
        /// <summary>
        /// Returns iVector4.gaaa swizzling (equivalent to iVector4.ywww).
        /// </summary>
        public iVector4 gaaa => new iVector4(y, w, w, w);
        
        /// <summary>
        /// Returns iVector4.zx swizzling.
        /// </summary>
        public iVector2 zx => new iVector2(z, x);
        
        /// <summary>
        /// Returns iVector4.br swizzling (equivalent to iVector4.zx).
        /// </summary>
        public iVector2 br => new iVector2(z, x);
        
        /// <summary>
        /// Returns iVector4.zxx swizzling.
        /// </summary>
        public iVector3 zxx => new iVector3(z, x, x);
        
        /// <summary>
        /// Returns iVector4.brr swizzling (equivalent to iVector4.zxx).
        /// </summary>
        public iVector3 brr => new iVector3(z, x, x);
        
        /// <summary>
        /// Returns iVector4.zxxx swizzling.
        /// </summary>
        public iVector4 zxxx => new iVector4(z, x, x, x);
        
        /// <summary>
        /// Returns iVector4.brrr swizzling (equivalent to iVector4.zxxx).
        /// </summary>
        public iVector4 brrr => new iVector4(z, x, x, x);
        
        /// <summary>
        /// Returns iVector4.zxxy swizzling.
        /// </summary>
        public iVector4 zxxy => new iVector4(z, x, x, y);
        
        /// <summary>
        /// Returns iVector4.brrg swizzling (equivalent to iVector4.zxxy).
        /// </summary>
        public iVector4 brrg => new iVector4(z, x, x, y);
        
        /// <summary>
        /// Returns iVector4.zxxz swizzling.
        /// </summary>
        public iVector4 zxxz => new iVector4(z, x, x, z);
        
        /// <summary>
        /// Returns iVector4.brrb swizzling (equivalent to iVector4.zxxz).
        /// </summary>
        public iVector4 brrb => new iVector4(z, x, x, z);
        
        /// <summary>
        /// Returns iVector4.zxxw swizzling.
        /// </summary>
        public iVector4 zxxw => new iVector4(z, x, x, w);
        
        /// <summary>
        /// Returns iVector4.brra swizzling (equivalent to iVector4.zxxw).
        /// </summary>
        public iVector4 brra => new iVector4(z, x, x, w);
        
        /// <summary>
        /// Returns iVector4.zxy swizzling.
        /// </summary>
        public iVector3 zxy => new iVector3(z, x, y);
        
        /// <summary>
        /// Returns iVector4.brg swizzling (equivalent to iVector4.zxy).
        /// </summary>
        public iVector3 brg => new iVector3(z, x, y);
        
        /// <summary>
        /// Returns iVector4.zxyx swizzling.
        /// </summary>
        public iVector4 zxyx => new iVector4(z, x, y, x);
        
        /// <summary>
        /// Returns iVector4.brgr swizzling (equivalent to iVector4.zxyx).
        /// </summary>
        public iVector4 brgr => new iVector4(z, x, y, x);
        
        /// <summary>
        /// Returns iVector4.zxyy swizzling.
        /// </summary>
        public iVector4 zxyy => new iVector4(z, x, y, y);
        
        /// <summary>
        /// Returns iVector4.brgg swizzling (equivalent to iVector4.zxyy).
        /// </summary>
        public iVector4 brgg => new iVector4(z, x, y, y);
        
        /// <summary>
        /// Returns iVector4.zxyz swizzling.
        /// </summary>
        public iVector4 zxyz => new iVector4(z, x, y, z);
        
        /// <summary>
        /// Returns iVector4.brgb swizzling (equivalent to iVector4.zxyz).
        /// </summary>
        public iVector4 brgb => new iVector4(z, x, y, z);
        
        /// <summary>
        /// Returns iVector4.zxyw swizzling.
        /// </summary>
        public iVector4 zxyw => new iVector4(z, x, y, w);
        
        /// <summary>
        /// Returns iVector4.brga swizzling (equivalent to iVector4.zxyw).
        /// </summary>
        public iVector4 brga => new iVector4(z, x, y, w);
        
        /// <summary>
        /// Returns iVector4.zxz swizzling.
        /// </summary>
        public iVector3 zxz => new iVector3(z, x, z);
        
        /// <summary>
        /// Returns iVector4.brb swizzling (equivalent to iVector4.zxz).
        /// </summary>
        public iVector3 brb => new iVector3(z, x, z);
        
        /// <summary>
        /// Returns iVector4.zxzx swizzling.
        /// </summary>
        public iVector4 zxzx => new iVector4(z, x, z, x);
        
        /// <summary>
        /// Returns iVector4.brbr swizzling (equivalent to iVector4.zxzx).
        /// </summary>
        public iVector4 brbr => new iVector4(z, x, z, x);
        
        /// <summary>
        /// Returns iVector4.zxzy swizzling.
        /// </summary>
        public iVector4 zxzy => new iVector4(z, x, z, y);
        
        /// <summary>
        /// Returns iVector4.brbg swizzling (equivalent to iVector4.zxzy).
        /// </summary>
        public iVector4 brbg => new iVector4(z, x, z, y);
        
        /// <summary>
        /// Returns iVector4.zxzz swizzling.
        /// </summary>
        public iVector4 zxzz => new iVector4(z, x, z, z);
        
        /// <summary>
        /// Returns iVector4.brbb swizzling (equivalent to iVector4.zxzz).
        /// </summary>
        public iVector4 brbb => new iVector4(z, x, z, z);
        
        /// <summary>
        /// Returns iVector4.zxzw swizzling.
        /// </summary>
        public iVector4 zxzw => new iVector4(z, x, z, w);
        
        /// <summary>
        /// Returns iVector4.brba swizzling (equivalent to iVector4.zxzw).
        /// </summary>
        public iVector4 brba => new iVector4(z, x, z, w);
        
        /// <summary>
        /// Returns iVector4.zxw swizzling.
        /// </summary>
        public iVector3 zxw => new iVector3(z, x, w);
        
        /// <summary>
        /// Returns iVector4.bra swizzling (equivalent to iVector4.zxw).
        /// </summary>
        public iVector3 bra => new iVector3(z, x, w);
        
        /// <summary>
        /// Returns iVector4.zxwx swizzling.
        /// </summary>
        public iVector4 zxwx => new iVector4(z, x, w, x);
        
        /// <summary>
        /// Returns iVector4.brar swizzling (equivalent to iVector4.zxwx).
        /// </summary>
        public iVector4 brar => new iVector4(z, x, w, x);
        
        /// <summary>
        /// Returns iVector4.zxwy swizzling.
        /// </summary>
        public iVector4 zxwy => new iVector4(z, x, w, y);
        
        /// <summary>
        /// Returns iVector4.brag swizzling (equivalent to iVector4.zxwy).
        /// </summary>
        public iVector4 brag => new iVector4(z, x, w, y);
        
        /// <summary>
        /// Returns iVector4.zxwz swizzling.
        /// </summary>
        public iVector4 zxwz => new iVector4(z, x, w, z);
        
        /// <summary>
        /// Returns iVector4.brab swizzling (equivalent to iVector4.zxwz).
        /// </summary>
        public iVector4 brab => new iVector4(z, x, w, z);
        
        /// <summary>
        /// Returns iVector4.zxww swizzling.
        /// </summary>
        public iVector4 zxww => new iVector4(z, x, w, w);
        
        /// <summary>
        /// Returns iVector4.braa swizzling (equivalent to iVector4.zxww).
        /// </summary>
        public iVector4 braa => new iVector4(z, x, w, w);
        
        /// <summary>
        /// Returns iVector4.zy swizzling.
        /// </summary>
        public iVector2 zy => new iVector2(z, y);
        
        /// <summary>
        /// Returns iVector4.bg swizzling (equivalent to iVector4.zy).
        /// </summary>
        public iVector2 bg => new iVector2(z, y);
        
        /// <summary>
        /// Returns iVector4.zyx swizzling.
        /// </summary>
        public iVector3 zyx => new iVector3(z, y, x);
        
        /// <summary>
        /// Returns iVector4.bgr swizzling (equivalent to iVector4.zyx).
        /// </summary>
        public iVector3 bgr => new iVector3(z, y, x);
        
        /// <summary>
        /// Returns iVector4.zyxx swizzling.
        /// </summary>
        public iVector4 zyxx => new iVector4(z, y, x, x);
        
        /// <summary>
        /// Returns iVector4.bgrr swizzling (equivalent to iVector4.zyxx).
        /// </summary>
        public iVector4 bgrr => new iVector4(z, y, x, x);
        
        /// <summary>
        /// Returns iVector4.zyxy swizzling.
        /// </summary>
        public iVector4 zyxy => new iVector4(z, y, x, y);
        
        /// <summary>
        /// Returns iVector4.bgrg swizzling (equivalent to iVector4.zyxy).
        /// </summary>
        public iVector4 bgrg => new iVector4(z, y, x, y);
        
        /// <summary>
        /// Returns iVector4.zyxz swizzling.
        /// </summary>
        public iVector4 zyxz => new iVector4(z, y, x, z);
        
        /// <summary>
        /// Returns iVector4.bgrb swizzling (equivalent to iVector4.zyxz).
        /// </summary>
        public iVector4 bgrb => new iVector4(z, y, x, z);
        
        /// <summary>
        /// Returns iVector4.zyxw swizzling.
        /// </summary>
        public iVector4 zyxw => new iVector4(z, y, x, w);
        
        /// <summary>
        /// Returns iVector4.bgra swizzling (equivalent to iVector4.zyxw).
        /// </summary>
        public iVector4 bgra => new iVector4(z, y, x, w);
        
        /// <summary>
        /// Returns iVector4.zyy swizzling.
        /// </summary>
        public iVector3 zyy => new iVector3(z, y, y);
        
        /// <summary>
        /// Returns iVector4.bgg swizzling (equivalent to iVector4.zyy).
        /// </summary>
        public iVector3 bgg => new iVector3(z, y, y);
        
        /// <summary>
        /// Returns iVector4.zyyx swizzling.
        /// </summary>
        public iVector4 zyyx => new iVector4(z, y, y, x);
        
        /// <summary>
        /// Returns iVector4.bggr swizzling (equivalent to iVector4.zyyx).
        /// </summary>
        public iVector4 bggr => new iVector4(z, y, y, x);
        
        /// <summary>
        /// Returns iVector4.zyyy swizzling.
        /// </summary>
        public iVector4 zyyy => new iVector4(z, y, y, y);
        
        /// <summary>
        /// Returns iVector4.bggg swizzling (equivalent to iVector4.zyyy).
        /// </summary>
        public iVector4 bggg => new iVector4(z, y, y, y);
        
        /// <summary>
        /// Returns iVector4.zyyz swizzling.
        /// </summary>
        public iVector4 zyyz => new iVector4(z, y, y, z);
        
        /// <summary>
        /// Returns iVector4.bggb swizzling (equivalent to iVector4.zyyz).
        /// </summary>
        public iVector4 bggb => new iVector4(z, y, y, z);
        
        /// <summary>
        /// Returns iVector4.zyyw swizzling.
        /// </summary>
        public iVector4 zyyw => new iVector4(z, y, y, w);
        
        /// <summary>
        /// Returns iVector4.bgga swizzling (equivalent to iVector4.zyyw).
        /// </summary>
        public iVector4 bgga => new iVector4(z, y, y, w);
        
        /// <summary>
        /// Returns iVector4.zyz swizzling.
        /// </summary>
        public iVector3 zyz => new iVector3(z, y, z);
        
        /// <summary>
        /// Returns iVector4.bgb swizzling (equivalent to iVector4.zyz).
        /// </summary>
        public iVector3 bgb => new iVector3(z, y, z);
        
        /// <summary>
        /// Returns iVector4.zyzx swizzling.
        /// </summary>
        public iVector4 zyzx => new iVector4(z, y, z, x);
        
        /// <summary>
        /// Returns iVector4.bgbr swizzling (equivalent to iVector4.zyzx).
        /// </summary>
        public iVector4 bgbr => new iVector4(z, y, z, x);
        
        /// <summary>
        /// Returns iVector4.zyzy swizzling.
        /// </summary>
        public iVector4 zyzy => new iVector4(z, y, z, y);
        
        /// <summary>
        /// Returns iVector4.bgbg swizzling (equivalent to iVector4.zyzy).
        /// </summary>
        public iVector4 bgbg => new iVector4(z, y, z, y);
        
        /// <summary>
        /// Returns iVector4.zyzz swizzling.
        /// </summary>
        public iVector4 zyzz => new iVector4(z, y, z, z);
        
        /// <summary>
        /// Returns iVector4.bgbb swizzling (equivalent to iVector4.zyzz).
        /// </summary>
        public iVector4 bgbb => new iVector4(z, y, z, z);
        
        /// <summary>
        /// Returns iVector4.zyzw swizzling.
        /// </summary>
        public iVector4 zyzw => new iVector4(z, y, z, w);
        
        /// <summary>
        /// Returns iVector4.bgba swizzling (equivalent to iVector4.zyzw).
        /// </summary>
        public iVector4 bgba => new iVector4(z, y, z, w);
        
        /// <summary>
        /// Returns iVector4.zyw swizzling.
        /// </summary>
        public iVector3 zyw => new iVector3(z, y, w);
        
        /// <summary>
        /// Returns iVector4.bga swizzling (equivalent to iVector4.zyw).
        /// </summary>
        public iVector3 bga => new iVector3(z, y, w);
        
        /// <summary>
        /// Returns iVector4.zywx swizzling.
        /// </summary>
        public iVector4 zywx => new iVector4(z, y, w, x);
        
        /// <summary>
        /// Returns iVector4.bgar swizzling (equivalent to iVector4.zywx).
        /// </summary>
        public iVector4 bgar => new iVector4(z, y, w, x);
        
        /// <summary>
        /// Returns iVector4.zywy swizzling.
        /// </summary>
        public iVector4 zywy => new iVector4(z, y, w, y);
        
        /// <summary>
        /// Returns iVector4.bgag swizzling (equivalent to iVector4.zywy).
        /// </summary>
        public iVector4 bgag => new iVector4(z, y, w, y);
        
        /// <summary>
        /// Returns iVector4.zywz swizzling.
        /// </summary>
        public iVector4 zywz => new iVector4(z, y, w, z);
        
        /// <summary>
        /// Returns iVector4.bgab swizzling (equivalent to iVector4.zywz).
        /// </summary>
        public iVector4 bgab => new iVector4(z, y, w, z);
        
        /// <summary>
        /// Returns iVector4.zyww swizzling.
        /// </summary>
        public iVector4 zyww => new iVector4(z, y, w, w);
        
        /// <summary>
        /// Returns iVector4.bgaa swizzling (equivalent to iVector4.zyww).
        /// </summary>
        public iVector4 bgaa => new iVector4(z, y, w, w);
        
        /// <summary>
        /// Returns iVector4.zz swizzling.
        /// </summary>
        public iVector2 zz => new iVector2(z, z);
        
        /// <summary>
        /// Returns iVector4.bb swizzling (equivalent to iVector4.zz).
        /// </summary>
        public iVector2 bb => new iVector2(z, z);
        
        /// <summary>
        /// Returns iVector4.zzx swizzling.
        /// </summary>
        public iVector3 zzx => new iVector3(z, z, x);
        
        /// <summary>
        /// Returns iVector4.bbr swizzling (equivalent to iVector4.zzx).
        /// </summary>
        public iVector3 bbr => new iVector3(z, z, x);
        
        /// <summary>
        /// Returns iVector4.zzxx swizzling.
        /// </summary>
        public iVector4 zzxx => new iVector4(z, z, x, x);
        
        /// <summary>
        /// Returns iVector4.bbrr swizzling (equivalent to iVector4.zzxx).
        /// </summary>
        public iVector4 bbrr => new iVector4(z, z, x, x);
        
        /// <summary>
        /// Returns iVector4.zzxy swizzling.
        /// </summary>
        public iVector4 zzxy => new iVector4(z, z, x, y);
        
        /// <summary>
        /// Returns iVector4.bbrg swizzling (equivalent to iVector4.zzxy).
        /// </summary>
        public iVector4 bbrg => new iVector4(z, z, x, y);
        
        /// <summary>
        /// Returns iVector4.zzxz swizzling.
        /// </summary>
        public iVector4 zzxz => new iVector4(z, z, x, z);
        
        /// <summary>
        /// Returns iVector4.bbrb swizzling (equivalent to iVector4.zzxz).
        /// </summary>
        public iVector4 bbrb => new iVector4(z, z, x, z);
        
        /// <summary>
        /// Returns iVector4.zzxw swizzling.
        /// </summary>
        public iVector4 zzxw => new iVector4(z, z, x, w);
        
        /// <summary>
        /// Returns iVector4.bbra swizzling (equivalent to iVector4.zzxw).
        /// </summary>
        public iVector4 bbra => new iVector4(z, z, x, w);
        
        /// <summary>
        /// Returns iVector4.zzy swizzling.
        /// </summary>
        public iVector3 zzy => new iVector3(z, z, y);
        
        /// <summary>
        /// Returns iVector4.bbg swizzling (equivalent to iVector4.zzy).
        /// </summary>
        public iVector3 bbg => new iVector3(z, z, y);
        
        /// <summary>
        /// Returns iVector4.zzyx swizzling.
        /// </summary>
        public iVector4 zzyx => new iVector4(z, z, y, x);
        
        /// <summary>
        /// Returns iVector4.bbgr swizzling (equivalent to iVector4.zzyx).
        /// </summary>
        public iVector4 bbgr => new iVector4(z, z, y, x);
        
        /// <summary>
        /// Returns iVector4.zzyy swizzling.
        /// </summary>
        public iVector4 zzyy => new iVector4(z, z, y, y);
        
        /// <summary>
        /// Returns iVector4.bbgg swizzling (equivalent to iVector4.zzyy).
        /// </summary>
        public iVector4 bbgg => new iVector4(z, z, y, y);
        
        /// <summary>
        /// Returns iVector4.zzyz swizzling.
        /// </summary>
        public iVector4 zzyz => new iVector4(z, z, y, z);
        
        /// <summary>
        /// Returns iVector4.bbgb swizzling (equivalent to iVector4.zzyz).
        /// </summary>
        public iVector4 bbgb => new iVector4(z, z, y, z);
        
        /// <summary>
        /// Returns iVector4.zzyw swizzling.
        /// </summary>
        public iVector4 zzyw => new iVector4(z, z, y, w);
        
        /// <summary>
        /// Returns iVector4.bbga swizzling (equivalent to iVector4.zzyw).
        /// </summary>
        public iVector4 bbga => new iVector4(z, z, y, w);
        
        /// <summary>
        /// Returns iVector4.zzz swizzling.
        /// </summary>
        public iVector3 zzz => new iVector3(z, z, z);
        
        /// <summary>
        /// Returns iVector4.bbb swizzling (equivalent to iVector4.zzz).
        /// </summary>
        public iVector3 bbb => new iVector3(z, z, z);
        
        /// <summary>
        /// Returns iVector4.zzzx swizzling.
        /// </summary>
        public iVector4 zzzx => new iVector4(z, z, z, x);
        
        /// <summary>
        /// Returns iVector4.bbbr swizzling (equivalent to iVector4.zzzx).
        /// </summary>
        public iVector4 bbbr => new iVector4(z, z, z, x);
        
        /// <summary>
        /// Returns iVector4.zzzy swizzling.
        /// </summary>
        public iVector4 zzzy => new iVector4(z, z, z, y);
        
        /// <summary>
        /// Returns iVector4.bbbg swizzling (equivalent to iVector4.zzzy).
        /// </summary>
        public iVector4 bbbg => new iVector4(z, z, z, y);
        
        /// <summary>
        /// Returns iVector4.zzzz swizzling.
        /// </summary>
        public iVector4 zzzz => new iVector4(z, z, z, z);
        
        /// <summary>
        /// Returns iVector4.bbbb swizzling (equivalent to iVector4.zzzz).
        /// </summary>
        public iVector4 bbbb => new iVector4(z, z, z, z);
        
        /// <summary>
        /// Returns iVector4.zzzw swizzling.
        /// </summary>
        public iVector4 zzzw => new iVector4(z, z, z, w);
        
        /// <summary>
        /// Returns iVector4.bbba swizzling (equivalent to iVector4.zzzw).
        /// </summary>
        public iVector4 bbba => new iVector4(z, z, z, w);
        
        /// <summary>
        /// Returns iVector4.zzw swizzling.
        /// </summary>
        public iVector3 zzw => new iVector3(z, z, w);
        
        /// <summary>
        /// Returns iVector4.bba swizzling (equivalent to iVector4.zzw).
        /// </summary>
        public iVector3 bba => new iVector3(z, z, w);
        
        /// <summary>
        /// Returns iVector4.zzwx swizzling.
        /// </summary>
        public iVector4 zzwx => new iVector4(z, z, w, x);
        
        /// <summary>
        /// Returns iVector4.bbar swizzling (equivalent to iVector4.zzwx).
        /// </summary>
        public iVector4 bbar => new iVector4(z, z, w, x);
        
        /// <summary>
        /// Returns iVector4.zzwy swizzling.
        /// </summary>
        public iVector4 zzwy => new iVector4(z, z, w, y);
        
        /// <summary>
        /// Returns iVector4.bbag swizzling (equivalent to iVector4.zzwy).
        /// </summary>
        public iVector4 bbag => new iVector4(z, z, w, y);
        
        /// <summary>
        /// Returns iVector4.zzwz swizzling.
        /// </summary>
        public iVector4 zzwz => new iVector4(z, z, w, z);
        
        /// <summary>
        /// Returns iVector4.bbab swizzling (equivalent to iVector4.zzwz).
        /// </summary>
        public iVector4 bbab => new iVector4(z, z, w, z);
        
        /// <summary>
        /// Returns iVector4.zzww swizzling.
        /// </summary>
        public iVector4 zzww => new iVector4(z, z, w, w);
        
        /// <summary>
        /// Returns iVector4.bbaa swizzling (equivalent to iVector4.zzww).
        /// </summary>
        public iVector4 bbaa => new iVector4(z, z, w, w);
        
        /// <summary>
        /// Returns iVector4.zw swizzling.
        /// </summary>
        public iVector2 zw => new iVector2(z, w);
        
        /// <summary>
        /// Returns iVector4.ba swizzling (equivalent to iVector4.zw).
        /// </summary>
        public iVector2 ba => new iVector2(z, w);
        
        /// <summary>
        /// Returns iVector4.zwx swizzling.
        /// </summary>
        public iVector3 zwx => new iVector3(z, w, x);
        
        /// <summary>
        /// Returns iVector4.bar swizzling (equivalent to iVector4.zwx).
        /// </summary>
        public iVector3 bar => new iVector3(z, w, x);
        
        /// <summary>
        /// Returns iVector4.zwxx swizzling.
        /// </summary>
        public iVector4 zwxx => new iVector4(z, w, x, x);
        
        /// <summary>
        /// Returns iVector4.barr swizzling (equivalent to iVector4.zwxx).
        /// </summary>
        public iVector4 barr => new iVector4(z, w, x, x);
        
        /// <summary>
        /// Returns iVector4.zwxy swizzling.
        /// </summary>
        public iVector4 zwxy => new iVector4(z, w, x, y);
        
        /// <summary>
        /// Returns iVector4.barg swizzling (equivalent to iVector4.zwxy).
        /// </summary>
        public iVector4 barg => new iVector4(z, w, x, y);
        
        /// <summary>
        /// Returns iVector4.zwxz swizzling.
        /// </summary>
        public iVector4 zwxz => new iVector4(z, w, x, z);
        
        /// <summary>
        /// Returns iVector4.barb swizzling (equivalent to iVector4.zwxz).
        /// </summary>
        public iVector4 barb => new iVector4(z, w, x, z);
        
        /// <summary>
        /// Returns iVector4.zwxw swizzling.
        /// </summary>
        public iVector4 zwxw => new iVector4(z, w, x, w);
        
        /// <summary>
        /// Returns iVector4.bara swizzling (equivalent to iVector4.zwxw).
        /// </summary>
        public iVector4 bara => new iVector4(z, w, x, w);
        
        /// <summary>
        /// Returns iVector4.zwy swizzling.
        /// </summary>
        public iVector3 zwy => new iVector3(z, w, y);
        
        /// <summary>
        /// Returns iVector4.bag swizzling (equivalent to iVector4.zwy).
        /// </summary>
        public iVector3 bag => new iVector3(z, w, y);
        
        /// <summary>
        /// Returns iVector4.zwyx swizzling.
        /// </summary>
        public iVector4 zwyx => new iVector4(z, w, y, x);
        
        /// <summary>
        /// Returns iVector4.bagr swizzling (equivalent to iVector4.zwyx).
        /// </summary>
        public iVector4 bagr => new iVector4(z, w, y, x);
        
        /// <summary>
        /// Returns iVector4.zwyy swizzling.
        /// </summary>
        public iVector4 zwyy => new iVector4(z, w, y, y);
        
        /// <summary>
        /// Returns iVector4.bagg swizzling (equivalent to iVector4.zwyy).
        /// </summary>
        public iVector4 bagg => new iVector4(z, w, y, y);
        
        /// <summary>
        /// Returns iVector4.zwyz swizzling.
        /// </summary>
        public iVector4 zwyz => new iVector4(z, w, y, z);
        
        /// <summary>
        /// Returns iVector4.bagb swizzling (equivalent to iVector4.zwyz).
        /// </summary>
        public iVector4 bagb => new iVector4(z, w, y, z);
        
        /// <summary>
        /// Returns iVector4.zwyw swizzling.
        /// </summary>
        public iVector4 zwyw => new iVector4(z, w, y, w);
        
        /// <summary>
        /// Returns iVector4.baga swizzling (equivalent to iVector4.zwyw).
        /// </summary>
        public iVector4 baga => new iVector4(z, w, y, w);
        
        /// <summary>
        /// Returns iVector4.zwz swizzling.
        /// </summary>
        public iVector3 zwz => new iVector3(z, w, z);
        
        /// <summary>
        /// Returns iVector4.bab swizzling (equivalent to iVector4.zwz).
        /// </summary>
        public iVector3 bab => new iVector3(z, w, z);
        
        /// <summary>
        /// Returns iVector4.zwzx swizzling.
        /// </summary>
        public iVector4 zwzx => new iVector4(z, w, z, x);
        
        /// <summary>
        /// Returns iVector4.babr swizzling (equivalent to iVector4.zwzx).
        /// </summary>
        public iVector4 babr => new iVector4(z, w, z, x);
        
        /// <summary>
        /// Returns iVector4.zwzy swizzling.
        /// </summary>
        public iVector4 zwzy => new iVector4(z, w, z, y);
        
        /// <summary>
        /// Returns iVector4.babg swizzling (equivalent to iVector4.zwzy).
        /// </summary>
        public iVector4 babg => new iVector4(z, w, z, y);
        
        /// <summary>
        /// Returns iVector4.zwzz swizzling.
        /// </summary>
        public iVector4 zwzz => new iVector4(z, w, z, z);
        
        /// <summary>
        /// Returns iVector4.babb swizzling (equivalent to iVector4.zwzz).
        /// </summary>
        public iVector4 babb => new iVector4(z, w, z, z);
        
        /// <summary>
        /// Returns iVector4.zwzw swizzling.
        /// </summary>
        public iVector4 zwzw => new iVector4(z, w, z, w);
        
        /// <summary>
        /// Returns iVector4.baba swizzling (equivalent to iVector4.zwzw).
        /// </summary>
        public iVector4 baba => new iVector4(z, w, z, w);
        
        /// <summary>
        /// Returns iVector4.zww swizzling.
        /// </summary>
        public iVector3 zww => new iVector3(z, w, w);
        
        /// <summary>
        /// Returns iVector4.baa swizzling (equivalent to iVector4.zww).
        /// </summary>
        public iVector3 baa => new iVector3(z, w, w);
        
        /// <summary>
        /// Returns iVector4.zwwx swizzling.
        /// </summary>
        public iVector4 zwwx => new iVector4(z, w, w, x);
        
        /// <summary>
        /// Returns iVector4.baar swizzling (equivalent to iVector4.zwwx).
        /// </summary>
        public iVector4 baar => new iVector4(z, w, w, x);
        
        /// <summary>
        /// Returns iVector4.zwwy swizzling.
        /// </summary>
        public iVector4 zwwy => new iVector4(z, w, w, y);
        
        /// <summary>
        /// Returns iVector4.baag swizzling (equivalent to iVector4.zwwy).
        /// </summary>
        public iVector4 baag => new iVector4(z, w, w, y);
        
        /// <summary>
        /// Returns iVector4.zwwz swizzling.
        /// </summary>
        public iVector4 zwwz => new iVector4(z, w, w, z);
        
        /// <summary>
        /// Returns iVector4.baab swizzling (equivalent to iVector4.zwwz).
        /// </summary>
        public iVector4 baab => new iVector4(z, w, w, z);
        
        /// <summary>
        /// Returns iVector4.zwww swizzling.
        /// </summary>
        public iVector4 zwww => new iVector4(z, w, w, w);
        
        /// <summary>
        /// Returns iVector4.baaa swizzling (equivalent to iVector4.zwww).
        /// </summary>
        public iVector4 baaa => new iVector4(z, w, w, w);
        
        /// <summary>
        /// Returns iVector4.wx swizzling.
        /// </summary>
        public iVector2 wx => new iVector2(w, x);
        
        /// <summary>
        /// Returns iVector4.ar swizzling (equivalent to iVector4.wx).
        /// </summary>
        public iVector2 ar => new iVector2(w, x);
        
        /// <summary>
        /// Returns iVector4.wxx swizzling.
        /// </summary>
        public iVector3 wxx => new iVector3(w, x, x);
        
        /// <summary>
        /// Returns iVector4.arr swizzling (equivalent to iVector4.wxx).
        /// </summary>
        public iVector3 arr => new iVector3(w, x, x);
        
        /// <summary>
        /// Returns iVector4.wxxx swizzling.
        /// </summary>
        public iVector4 wxxx => new iVector4(w, x, x, x);
        
        /// <summary>
        /// Returns iVector4.arrr swizzling (equivalent to iVector4.wxxx).
        /// </summary>
        public iVector4 arrr => new iVector4(w, x, x, x);
        
        /// <summary>
        /// Returns iVector4.wxxy swizzling.
        /// </summary>
        public iVector4 wxxy => new iVector4(w, x, x, y);
        
        /// <summary>
        /// Returns iVector4.arrg swizzling (equivalent to iVector4.wxxy).
        /// </summary>
        public iVector4 arrg => new iVector4(w, x, x, y);
        
        /// <summary>
        /// Returns iVector4.wxxz swizzling.
        /// </summary>
        public iVector4 wxxz => new iVector4(w, x, x, z);
        
        /// <summary>
        /// Returns iVector4.arrb swizzling (equivalent to iVector4.wxxz).
        /// </summary>
        public iVector4 arrb => new iVector4(w, x, x, z);
        
        /// <summary>
        /// Returns iVector4.wxxw swizzling.
        /// </summary>
        public iVector4 wxxw => new iVector4(w, x, x, w);
        
        /// <summary>
        /// Returns iVector4.arra swizzling (equivalent to iVector4.wxxw).
        /// </summary>
        public iVector4 arra => new iVector4(w, x, x, w);
        
        /// <summary>
        /// Returns iVector4.wxy swizzling.
        /// </summary>
        public iVector3 wxy => new iVector3(w, x, y);
        
        /// <summary>
        /// Returns iVector4.arg swizzling (equivalent to iVector4.wxy).
        /// </summary>
        public iVector3 arg => new iVector3(w, x, y);
        
        /// <summary>
        /// Returns iVector4.wxyx swizzling.
        /// </summary>
        public iVector4 wxyx => new iVector4(w, x, y, x);
        
        /// <summary>
        /// Returns iVector4.argr swizzling (equivalent to iVector4.wxyx).
        /// </summary>
        public iVector4 argr => new iVector4(w, x, y, x);
        
        /// <summary>
        /// Returns iVector4.wxyy swizzling.
        /// </summary>
        public iVector4 wxyy => new iVector4(w, x, y, y);
        
        /// <summary>
        /// Returns iVector4.argg swizzling (equivalent to iVector4.wxyy).
        /// </summary>
        public iVector4 argg => new iVector4(w, x, y, y);
        
        /// <summary>
        /// Returns iVector4.wxyz swizzling.
        /// </summary>
        public iVector4 wxyz => new iVector4(w, x, y, z);
        
        /// <summary>
        /// Returns iVector4.argb swizzling (equivalent to iVector4.wxyz).
        /// </summary>
        public iVector4 argb => new iVector4(w, x, y, z);
        
        /// <summary>
        /// Returns iVector4.wxyw swizzling.
        /// </summary>
        public iVector4 wxyw => new iVector4(w, x, y, w);
        
        /// <summary>
        /// Returns iVector4.arga swizzling (equivalent to iVector4.wxyw).
        /// </summary>
        public iVector4 arga => new iVector4(w, x, y, w);
        
        /// <summary>
        /// Returns iVector4.wxz swizzling.
        /// </summary>
        public iVector3 wxz => new iVector3(w, x, z);
        
        /// <summary>
        /// Returns iVector4.arb swizzling (equivalent to iVector4.wxz).
        /// </summary>
        public iVector3 arb => new iVector3(w, x, z);
        
        /// <summary>
        /// Returns iVector4.wxzx swizzling.
        /// </summary>
        public iVector4 wxzx => new iVector4(w, x, z, x);
        
        /// <summary>
        /// Returns iVector4.arbr swizzling (equivalent to iVector4.wxzx).
        /// </summary>
        public iVector4 arbr => new iVector4(w, x, z, x);
        
        /// <summary>
        /// Returns iVector4.wxzy swizzling.
        /// </summary>
        public iVector4 wxzy => new iVector4(w, x, z, y);
        
        /// <summary>
        /// Returns iVector4.arbg swizzling (equivalent to iVector4.wxzy).
        /// </summary>
        public iVector4 arbg => new iVector4(w, x, z, y);
        
        /// <summary>
        /// Returns iVector4.wxzz swizzling.
        /// </summary>
        public iVector4 wxzz => new iVector4(w, x, z, z);
        
        /// <summary>
        /// Returns iVector4.arbb swizzling (equivalent to iVector4.wxzz).
        /// </summary>
        public iVector4 arbb => new iVector4(w, x, z, z);
        
        /// <summary>
        /// Returns iVector4.wxzw swizzling.
        /// </summary>
        public iVector4 wxzw => new iVector4(w, x, z, w);
        
        /// <summary>
        /// Returns iVector4.arba swizzling (equivalent to iVector4.wxzw).
        /// </summary>
        public iVector4 arba => new iVector4(w, x, z, w);
        
        /// <summary>
        /// Returns iVector4.wxw swizzling.
        /// </summary>
        public iVector3 wxw => new iVector3(w, x, w);
        
        /// <summary>
        /// Returns iVector4.ara swizzling (equivalent to iVector4.wxw).
        /// </summary>
        public iVector3 ara => new iVector3(w, x, w);
        
        /// <summary>
        /// Returns iVector4.wxwx swizzling.
        /// </summary>
        public iVector4 wxwx => new iVector4(w, x, w, x);
        
        /// <summary>
        /// Returns iVector4.arar swizzling (equivalent to iVector4.wxwx).
        /// </summary>
        public iVector4 arar => new iVector4(w, x, w, x);
        
        /// <summary>
        /// Returns iVector4.wxwy swizzling.
        /// </summary>
        public iVector4 wxwy => new iVector4(w, x, w, y);
        
        /// <summary>
        /// Returns iVector4.arag swizzling (equivalent to iVector4.wxwy).
        /// </summary>
        public iVector4 arag => new iVector4(w, x, w, y);
        
        /// <summary>
        /// Returns iVector4.wxwz swizzling.
        /// </summary>
        public iVector4 wxwz => new iVector4(w, x, w, z);
        
        /// <summary>
        /// Returns iVector4.arab swizzling (equivalent to iVector4.wxwz).
        /// </summary>
        public iVector4 arab => new iVector4(w, x, w, z);
        
        /// <summary>
        /// Returns iVector4.wxww swizzling.
        /// </summary>
        public iVector4 wxww => new iVector4(w, x, w, w);
        
        /// <summary>
        /// Returns iVector4.araa swizzling (equivalent to iVector4.wxww).
        /// </summary>
        public iVector4 araa => new iVector4(w, x, w, w);
        
        /// <summary>
        /// Returns iVector4.wy swizzling.
        /// </summary>
        public iVector2 wy => new iVector2(w, y);
        
        /// <summary>
        /// Returns iVector4.ag swizzling (equivalent to iVector4.wy).
        /// </summary>
        public iVector2 ag => new iVector2(w, y);
        
        /// <summary>
        /// Returns iVector4.wyx swizzling.
        /// </summary>
        public iVector3 wyx => new iVector3(w, y, x);
        
        /// <summary>
        /// Returns iVector4.agr swizzling (equivalent to iVector4.wyx).
        /// </summary>
        public iVector3 agr => new iVector3(w, y, x);
        
        /// <summary>
        /// Returns iVector4.wyxx swizzling.
        /// </summary>
        public iVector4 wyxx => new iVector4(w, y, x, x);
        
        /// <summary>
        /// Returns iVector4.agrr swizzling (equivalent to iVector4.wyxx).
        /// </summary>
        public iVector4 agrr => new iVector4(w, y, x, x);
        
        /// <summary>
        /// Returns iVector4.wyxy swizzling.
        /// </summary>
        public iVector4 wyxy => new iVector4(w, y, x, y);
        
        /// <summary>
        /// Returns iVector4.agrg swizzling (equivalent to iVector4.wyxy).
        /// </summary>
        public iVector4 agrg => new iVector4(w, y, x, y);
        
        /// <summary>
        /// Returns iVector4.wyxz swizzling.
        /// </summary>
        public iVector4 wyxz => new iVector4(w, y, x, z);
        
        /// <summary>
        /// Returns iVector4.agrb swizzling (equivalent to iVector4.wyxz).
        /// </summary>
        public iVector4 agrb => new iVector4(w, y, x, z);
        
        /// <summary>
        /// Returns iVector4.wyxw swizzling.
        /// </summary>
        public iVector4 wyxw => new iVector4(w, y, x, w);
        
        /// <summary>
        /// Returns iVector4.agra swizzling (equivalent to iVector4.wyxw).
        /// </summary>
        public iVector4 agra => new iVector4(w, y, x, w);
        
        /// <summary>
        /// Returns iVector4.wyy swizzling.
        /// </summary>
        public iVector3 wyy => new iVector3(w, y, y);
        
        /// <summary>
        /// Returns iVector4.agg swizzling (equivalent to iVector4.wyy).
        /// </summary>
        public iVector3 agg => new iVector3(w, y, y);
        
        /// <summary>
        /// Returns iVector4.wyyx swizzling.
        /// </summary>
        public iVector4 wyyx => new iVector4(w, y, y, x);
        
        /// <summary>
        /// Returns iVector4.aggr swizzling (equivalent to iVector4.wyyx).
        /// </summary>
        public iVector4 aggr => new iVector4(w, y, y, x);
        
        /// <summary>
        /// Returns iVector4.wyyy swizzling.
        /// </summary>
        public iVector4 wyyy => new iVector4(w, y, y, y);
        
        /// <summary>
        /// Returns iVector4.aggg swizzling (equivalent to iVector4.wyyy).
        /// </summary>
        public iVector4 aggg => new iVector4(w, y, y, y);
        
        /// <summary>
        /// Returns iVector4.wyyz swizzling.
        /// </summary>
        public iVector4 wyyz => new iVector4(w, y, y, z);
        
        /// <summary>
        /// Returns iVector4.aggb swizzling (equivalent to iVector4.wyyz).
        /// </summary>
        public iVector4 aggb => new iVector4(w, y, y, z);
        
        /// <summary>
        /// Returns iVector4.wyyw swizzling.
        /// </summary>
        public iVector4 wyyw => new iVector4(w, y, y, w);
        
        /// <summary>
        /// Returns iVector4.agga swizzling (equivalent to iVector4.wyyw).
        /// </summary>
        public iVector4 agga => new iVector4(w, y, y, w);
        
        /// <summary>
        /// Returns iVector4.wyz swizzling.
        /// </summary>
        public iVector3 wyz => new iVector3(w, y, z);
        
        /// <summary>
        /// Returns iVector4.agb swizzling (equivalent to iVector4.wyz).
        /// </summary>
        public iVector3 agb => new iVector3(w, y, z);
        
        /// <summary>
        /// Returns iVector4.wyzx swizzling.
        /// </summary>
        public iVector4 wyzx => new iVector4(w, y, z, x);
        
        /// <summary>
        /// Returns iVector4.agbr swizzling (equivalent to iVector4.wyzx).
        /// </summary>
        public iVector4 agbr => new iVector4(w, y, z, x);
        
        /// <summary>
        /// Returns iVector4.wyzy swizzling.
        /// </summary>
        public iVector4 wyzy => new iVector4(w, y, z, y);
        
        /// <summary>
        /// Returns iVector4.agbg swizzling (equivalent to iVector4.wyzy).
        /// </summary>
        public iVector4 agbg => new iVector4(w, y, z, y);
        
        /// <summary>
        /// Returns iVector4.wyzz swizzling.
        /// </summary>
        public iVector4 wyzz => new iVector4(w, y, z, z);
        
        /// <summary>
        /// Returns iVector4.agbb swizzling (equivalent to iVector4.wyzz).
        /// </summary>
        public iVector4 agbb => new iVector4(w, y, z, z);
        
        /// <summary>
        /// Returns iVector4.wyzw swizzling.
        /// </summary>
        public iVector4 wyzw => new iVector4(w, y, z, w);
        
        /// <summary>
        /// Returns iVector4.agba swizzling (equivalent to iVector4.wyzw).
        /// </summary>
        public iVector4 agba => new iVector4(w, y, z, w);
        
        /// <summary>
        /// Returns iVector4.wyw swizzling.
        /// </summary>
        public iVector3 wyw => new iVector3(w, y, w);
        
        /// <summary>
        /// Returns iVector4.aga swizzling (equivalent to iVector4.wyw).
        /// </summary>
        public iVector3 aga => new iVector3(w, y, w);
        
        /// <summary>
        /// Returns iVector4.wywx swizzling.
        /// </summary>
        public iVector4 wywx => new iVector4(w, y, w, x);
        
        /// <summary>
        /// Returns iVector4.agar swizzling (equivalent to iVector4.wywx).
        /// </summary>
        public iVector4 agar => new iVector4(w, y, w, x);
        
        /// <summary>
        /// Returns iVector4.wywy swizzling.
        /// </summary>
        public iVector4 wywy => new iVector4(w, y, w, y);
        
        /// <summary>
        /// Returns iVector4.agag swizzling (equivalent to iVector4.wywy).
        /// </summary>
        public iVector4 agag => new iVector4(w, y, w, y);
        
        /// <summary>
        /// Returns iVector4.wywz swizzling.
        /// </summary>
        public iVector4 wywz => new iVector4(w, y, w, z);
        
        /// <summary>
        /// Returns iVector4.agab swizzling (equivalent to iVector4.wywz).
        /// </summary>
        public iVector4 agab => new iVector4(w, y, w, z);
        
        /// <summary>
        /// Returns iVector4.wyww swizzling.
        /// </summary>
        public iVector4 wyww => new iVector4(w, y, w, w);
        
        /// <summary>
        /// Returns iVector4.agaa swizzling (equivalent to iVector4.wyww).
        /// </summary>
        public iVector4 agaa => new iVector4(w, y, w, w);
        
        /// <summary>
        /// Returns iVector4.wz swizzling.
        /// </summary>
        public iVector2 wz => new iVector2(w, z);
        
        /// <summary>
        /// Returns iVector4.ab swizzling (equivalent to iVector4.wz).
        /// </summary>
        public iVector2 ab => new iVector2(w, z);
        
        /// <summary>
        /// Returns iVector4.wzx swizzling.
        /// </summary>
        public iVector3 wzx => new iVector3(w, z, x);
        
        /// <summary>
        /// Returns iVector4.abr swizzling (equivalent to iVector4.wzx).
        /// </summary>
        public iVector3 abr => new iVector3(w, z, x);
        
        /// <summary>
        /// Returns iVector4.wzxx swizzling.
        /// </summary>
        public iVector4 wzxx => new iVector4(w, z, x, x);
        
        /// <summary>
        /// Returns iVector4.abrr swizzling (equivalent to iVector4.wzxx).
        /// </summary>
        public iVector4 abrr => new iVector4(w, z, x, x);
        
        /// <summary>
        /// Returns iVector4.wzxy swizzling.
        /// </summary>
        public iVector4 wzxy => new iVector4(w, z, x, y);
        
        /// <summary>
        /// Returns iVector4.abrg swizzling (equivalent to iVector4.wzxy).
        /// </summary>
        public iVector4 abrg => new iVector4(w, z, x, y);
        
        /// <summary>
        /// Returns iVector4.wzxz swizzling.
        /// </summary>
        public iVector4 wzxz => new iVector4(w, z, x, z);
        
        /// <summary>
        /// Returns iVector4.abrb swizzling (equivalent to iVector4.wzxz).
        /// </summary>
        public iVector4 abrb => new iVector4(w, z, x, z);
        
        /// <summary>
        /// Returns iVector4.wzxw swizzling.
        /// </summary>
        public iVector4 wzxw => new iVector4(w, z, x, w);
        
        /// <summary>
        /// Returns iVector4.abra swizzling (equivalent to iVector4.wzxw).
        /// </summary>
        public iVector4 abra => new iVector4(w, z, x, w);
        
        /// <summary>
        /// Returns iVector4.wzy swizzling.
        /// </summary>
        public iVector3 wzy => new iVector3(w, z, y);
        
        /// <summary>
        /// Returns iVector4.abg swizzling (equivalent to iVector4.wzy).
        /// </summary>
        public iVector3 abg => new iVector3(w, z, y);
        
        /// <summary>
        /// Returns iVector4.wzyx swizzling.
        /// </summary>
        public iVector4 wzyx => new iVector4(w, z, y, x);
        
        /// <summary>
        /// Returns iVector4.abgr swizzling (equivalent to iVector4.wzyx).
        /// </summary>
        public iVector4 abgr => new iVector4(w, z, y, x);
        
        /// <summary>
        /// Returns iVector4.wzyy swizzling.
        /// </summary>
        public iVector4 wzyy => new iVector4(w, z, y, y);
        
        /// <summary>
        /// Returns iVector4.abgg swizzling (equivalent to iVector4.wzyy).
        /// </summary>
        public iVector4 abgg => new iVector4(w, z, y, y);
        
        /// <summary>
        /// Returns iVector4.wzyz swizzling.
        /// </summary>
        public iVector4 wzyz => new iVector4(w, z, y, z);
        
        /// <summary>
        /// Returns iVector4.abgb swizzling (equivalent to iVector4.wzyz).
        /// </summary>
        public iVector4 abgb => new iVector4(w, z, y, z);
        
        /// <summary>
        /// Returns iVector4.wzyw swizzling.
        /// </summary>
        public iVector4 wzyw => new iVector4(w, z, y, w);
        
        /// <summary>
        /// Returns iVector4.abga swizzling (equivalent to iVector4.wzyw).
        /// </summary>
        public iVector4 abga => new iVector4(w, z, y, w);
        
        /// <summary>
        /// Returns iVector4.wzz swizzling.
        /// </summary>
        public iVector3 wzz => new iVector3(w, z, z);
        
        /// <summary>
        /// Returns iVector4.abb swizzling (equivalent to iVector4.wzz).
        /// </summary>
        public iVector3 abb => new iVector3(w, z, z);
        
        /// <summary>
        /// Returns iVector4.wzzx swizzling.
        /// </summary>
        public iVector4 wzzx => new iVector4(w, z, z, x);
        
        /// <summary>
        /// Returns iVector4.abbr swizzling (equivalent to iVector4.wzzx).
        /// </summary>
        public iVector4 abbr => new iVector4(w, z, z, x);
        
        /// <summary>
        /// Returns iVector4.wzzy swizzling.
        /// </summary>
        public iVector4 wzzy => new iVector4(w, z, z, y);
        
        /// <summary>
        /// Returns iVector4.abbg swizzling (equivalent to iVector4.wzzy).
        /// </summary>
        public iVector4 abbg => new iVector4(w, z, z, y);
        
        /// <summary>
        /// Returns iVector4.wzzz swizzling.
        /// </summary>
        public iVector4 wzzz => new iVector4(w, z, z, z);
        
        /// <summary>
        /// Returns iVector4.abbb swizzling (equivalent to iVector4.wzzz).
        /// </summary>
        public iVector4 abbb => new iVector4(w, z, z, z);
        
        /// <summary>
        /// Returns iVector4.wzzw swizzling.
        /// </summary>
        public iVector4 wzzw => new iVector4(w, z, z, w);
        
        /// <summary>
        /// Returns iVector4.abba swizzling (equivalent to iVector4.wzzw).
        /// </summary>
        public iVector4 abba => new iVector4(w, z, z, w);
        
        /// <summary>
        /// Returns iVector4.wzw swizzling.
        /// </summary>
        public iVector3 wzw => new iVector3(w, z, w);
        
        /// <summary>
        /// Returns iVector4.aba swizzling (equivalent to iVector4.wzw).
        /// </summary>
        public iVector3 aba => new iVector3(w, z, w);
        
        /// <summary>
        /// Returns iVector4.wzwx swizzling.
        /// </summary>
        public iVector4 wzwx => new iVector4(w, z, w, x);
        
        /// <summary>
        /// Returns iVector4.abar swizzling (equivalent to iVector4.wzwx).
        /// </summary>
        public iVector4 abar => new iVector4(w, z, w, x);
        
        /// <summary>
        /// Returns iVector4.wzwy swizzling.
        /// </summary>
        public iVector4 wzwy => new iVector4(w, z, w, y);
        
        /// <summary>
        /// Returns iVector4.abag swizzling (equivalent to iVector4.wzwy).
        /// </summary>
        public iVector4 abag => new iVector4(w, z, w, y);
        
        /// <summary>
        /// Returns iVector4.wzwz swizzling.
        /// </summary>
        public iVector4 wzwz => new iVector4(w, z, w, z);
        
        /// <summary>
        /// Returns iVector4.abab swizzling (equivalent to iVector4.wzwz).
        /// </summary>
        public iVector4 abab => new iVector4(w, z, w, z);
        
        /// <summary>
        /// Returns iVector4.wzww swizzling.
        /// </summary>
        public iVector4 wzww => new iVector4(w, z, w, w);
        
        /// <summary>
        /// Returns iVector4.abaa swizzling (equivalent to iVector4.wzww).
        /// </summary>
        public iVector4 abaa => new iVector4(w, z, w, w);
        
        /// <summary>
        /// Returns iVector4.ww swizzling.
        /// </summary>
        public iVector2 ww => new iVector2(w, w);
        
        /// <summary>
        /// Returns iVector4.aa swizzling (equivalent to iVector4.ww).
        /// </summary>
        public iVector2 aa => new iVector2(w, w);
        
        /// <summary>
        /// Returns iVector4.wwx swizzling.
        /// </summary>
        public iVector3 wwx => new iVector3(w, w, x);
        
        /// <summary>
        /// Returns iVector4.aar swizzling (equivalent to iVector4.wwx).
        /// </summary>
        public iVector3 aar => new iVector3(w, w, x);
        
        /// <summary>
        /// Returns iVector4.wwxx swizzling.
        /// </summary>
        public iVector4 wwxx => new iVector4(w, w, x, x);
        
        /// <summary>
        /// Returns iVector4.aarr swizzling (equivalent to iVector4.wwxx).
        /// </summary>
        public iVector4 aarr => new iVector4(w, w, x, x);
        
        /// <summary>
        /// Returns iVector4.wwxy swizzling.
        /// </summary>
        public iVector4 wwxy => new iVector4(w, w, x, y);
        
        /// <summary>
        /// Returns iVector4.aarg swizzling (equivalent to iVector4.wwxy).
        /// </summary>
        public iVector4 aarg => new iVector4(w, w, x, y);
        
        /// <summary>
        /// Returns iVector4.wwxz swizzling.
        /// </summary>
        public iVector4 wwxz => new iVector4(w, w, x, z);
        
        /// <summary>
        /// Returns iVector4.aarb swizzling (equivalent to iVector4.wwxz).
        /// </summary>
        public iVector4 aarb => new iVector4(w, w, x, z);
        
        /// <summary>
        /// Returns iVector4.wwxw swizzling.
        /// </summary>
        public iVector4 wwxw => new iVector4(w, w, x, w);
        
        /// <summary>
        /// Returns iVector4.aara swizzling (equivalent to iVector4.wwxw).
        /// </summary>
        public iVector4 aara => new iVector4(w, w, x, w);
        
        /// <summary>
        /// Returns iVector4.wwy swizzling.
        /// </summary>
        public iVector3 wwy => new iVector3(w, w, y);
        
        /// <summary>
        /// Returns iVector4.aag swizzling (equivalent to iVector4.wwy).
        /// </summary>
        public iVector3 aag => new iVector3(w, w, y);
        
        /// <summary>
        /// Returns iVector4.wwyx swizzling.
        /// </summary>
        public iVector4 wwyx => new iVector4(w, w, y, x);
        
        /// <summary>
        /// Returns iVector4.aagr swizzling (equivalent to iVector4.wwyx).
        /// </summary>
        public iVector4 aagr => new iVector4(w, w, y, x);
        
        /// <summary>
        /// Returns iVector4.wwyy swizzling.
        /// </summary>
        public iVector4 wwyy => new iVector4(w, w, y, y);
        
        /// <summary>
        /// Returns iVector4.aagg swizzling (equivalent to iVector4.wwyy).
        /// </summary>
        public iVector4 aagg => new iVector4(w, w, y, y);
        
        /// <summary>
        /// Returns iVector4.wwyz swizzling.
        /// </summary>
        public iVector4 wwyz => new iVector4(w, w, y, z);
        
        /// <summary>
        /// Returns iVector4.aagb swizzling (equivalent to iVector4.wwyz).
        /// </summary>
        public iVector4 aagb => new iVector4(w, w, y, z);
        
        /// <summary>
        /// Returns iVector4.wwyw swizzling.
        /// </summary>
        public iVector4 wwyw => new iVector4(w, w, y, w);
        
        /// <summary>
        /// Returns iVector4.aaga swizzling (equivalent to iVector4.wwyw).
        /// </summary>
        public iVector4 aaga => new iVector4(w, w, y, w);
        
        /// <summary>
        /// Returns iVector4.wwz swizzling.
        /// </summary>
        public iVector3 wwz => new iVector3(w, w, z);
        
        /// <summary>
        /// Returns iVector4.aab swizzling (equivalent to iVector4.wwz).
        /// </summary>
        public iVector3 aab => new iVector3(w, w, z);
        
        /// <summary>
        /// Returns iVector4.wwzx swizzling.
        /// </summary>
        public iVector4 wwzx => new iVector4(w, w, z, x);
        
        /// <summary>
        /// Returns iVector4.aabr swizzling (equivalent to iVector4.wwzx).
        /// </summary>
        public iVector4 aabr => new iVector4(w, w, z, x);
        
        /// <summary>
        /// Returns iVector4.wwzy swizzling.
        /// </summary>
        public iVector4 wwzy => new iVector4(w, w, z, y);
        
        /// <summary>
        /// Returns iVector4.aabg swizzling (equivalent to iVector4.wwzy).
        /// </summary>
        public iVector4 aabg => new iVector4(w, w, z, y);
        
        /// <summary>
        /// Returns iVector4.wwzz swizzling.
        /// </summary>
        public iVector4 wwzz => new iVector4(w, w, z, z);
        
        /// <summary>
        /// Returns iVector4.aabb swizzling (equivalent to iVector4.wwzz).
        /// </summary>
        public iVector4 aabb => new iVector4(w, w, z, z);
        
        /// <summary>
        /// Returns iVector4.wwzw swizzling.
        /// </summary>
        public iVector4 wwzw => new iVector4(w, w, z, w);
        
        /// <summary>
        /// Returns iVector4.aaba swizzling (equivalent to iVector4.wwzw).
        /// </summary>
        public iVector4 aaba => new iVector4(w, w, z, w);
        
        /// <summary>
        /// Returns iVector4.www swizzling.
        /// </summary>
        public iVector3 www => new iVector3(w, w, w);
        
        /// <summary>
        /// Returns iVector4.aaa swizzling (equivalent to iVector4.www).
        /// </summary>
        public iVector3 aaa => new iVector3(w, w, w);
        
        /// <summary>
        /// Returns iVector4.wwwx swizzling.
        /// </summary>
        public iVector4 wwwx => new iVector4(w, w, w, x);
        
        /// <summary>
        /// Returns iVector4.aaar swizzling (equivalent to iVector4.wwwx).
        /// </summary>
        public iVector4 aaar => new iVector4(w, w, w, x);
        
        /// <summary>
        /// Returns iVector4.wwwy swizzling.
        /// </summary>
        public iVector4 wwwy => new iVector4(w, w, w, y);
        
        /// <summary>
        /// Returns iVector4.aaag swizzling (equivalent to iVector4.wwwy).
        /// </summary>
        public iVector4 aaag => new iVector4(w, w, w, y);
        
        /// <summary>
        /// Returns iVector4.wwwz swizzling.
        /// </summary>
        public iVector4 wwwz => new iVector4(w, w, w, z);
        
        /// <summary>
        /// Returns iVector4.aaab swizzling (equivalent to iVector4.wwwz).
        /// </summary>
        public iVector4 aaab => new iVector4(w, w, w, z);
        
        /// <summary>
        /// Returns iVector4.wwww swizzling.
        /// </summary>
        public iVector4 wwww => new iVector4(w, w, w, w);
        
        /// <summary>
        /// Returns iVector4.aaaa swizzling (equivalent to iVector4.wwww).
        /// </summary>
        public iVector4 aaaa => new iVector4(w, w, w, w);

        #endregion

    }
}
