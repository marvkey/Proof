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
    /// Temporary Vector of type bool with 4 components, used for implementing swizzling for bVector4.
    /// </summary>
    
   
    [StructLayout(LayoutKind.Sequential)]
    public struct swizzle_bVector4
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
        
        /// <summary>
        /// w-component
        /// </summary>
        
        internal readonly bool w;

        #endregion


        #region Constructors
        
        /// <summary>
        /// Constructor for swizzle_bVector4.
        /// </summary>
        internal swizzle_bVector4(bool x, bool y, bool z, bool w)
        {
            this.x = x;
            this.y = y;
            this.z = z;
            this.w = w;
        }

        #endregion


        #region Properties
        
        /// <summary>
        /// Returns bVector4.xx swizzling.
        /// </summary>
        public bVector2 xx => new bVector2(x, x);
        
        /// <summary>
        /// Returns bVector4.rr swizzling (equivalent to bVector4.xx).
        /// </summary>
        public bVector2 rr => new bVector2(x, x);
        
        /// <summary>
        /// Returns bVector4.xxx swizzling.
        /// </summary>
        public bVector3 xxx => new bVector3(x, x, x);
        
        /// <summary>
        /// Returns bVector4.rrr swizzling (equivalent to bVector4.xxx).
        /// </summary>
        public bVector3 rrr => new bVector3(x, x, x);
        
        /// <summary>
        /// Returns bVector4.xxxx swizzling.
        /// </summary>
        public bVector4 xxxx => new bVector4(x, x, x, x);
        
        /// <summary>
        /// Returns bVector4.rrrr swizzling (equivalent to bVector4.xxxx).
        /// </summary>
        public bVector4 rrrr => new bVector4(x, x, x, x);
        
        /// <summary>
        /// Returns bVector4.xxxy swizzling.
        /// </summary>
        public bVector4 xxxy => new bVector4(x, x, x, y);
        
        /// <summary>
        /// Returns bVector4.rrrg swizzling (equivalent to bVector4.xxxy).
        /// </summary>
        public bVector4 rrrg => new bVector4(x, x, x, y);
        
        /// <summary>
        /// Returns bVector4.xxxz swizzling.
        /// </summary>
        public bVector4 xxxz => new bVector4(x, x, x, z);
        
        /// <summary>
        /// Returns bVector4.rrrb swizzling (equivalent to bVector4.xxxz).
        /// </summary>
        public bVector4 rrrb => new bVector4(x, x, x, z);
        
        /// <summary>
        /// Returns bVector4.xxxw swizzling.
        /// </summary>
        public bVector4 xxxw => new bVector4(x, x, x, w);
        
        /// <summary>
        /// Returns bVector4.rrra swizzling (equivalent to bVector4.xxxw).
        /// </summary>
        public bVector4 rrra => new bVector4(x, x, x, w);
        
        /// <summary>
        /// Returns bVector4.xxy swizzling.
        /// </summary>
        public bVector3 xxy => new bVector3(x, x, y);
        
        /// <summary>
        /// Returns bVector4.rrg swizzling (equivalent to bVector4.xxy).
        /// </summary>
        public bVector3 rrg => new bVector3(x, x, y);
        
        /// <summary>
        /// Returns bVector4.xxyx swizzling.
        /// </summary>
        public bVector4 xxyx => new bVector4(x, x, y, x);
        
        /// <summary>
        /// Returns bVector4.rrgr swizzling (equivalent to bVector4.xxyx).
        /// </summary>
        public bVector4 rrgr => new bVector4(x, x, y, x);
        
        /// <summary>
        /// Returns bVector4.xxyy swizzling.
        /// </summary>
        public bVector4 xxyy => new bVector4(x, x, y, y);
        
        /// <summary>
        /// Returns bVector4.rrgg swizzling (equivalent to bVector4.xxyy).
        /// </summary>
        public bVector4 rrgg => new bVector4(x, x, y, y);
        
        /// <summary>
        /// Returns bVector4.xxyz swizzling.
        /// </summary>
        public bVector4 xxyz => new bVector4(x, x, y, z);
        
        /// <summary>
        /// Returns bVector4.rrgb swizzling (equivalent to bVector4.xxyz).
        /// </summary>
        public bVector4 rrgb => new bVector4(x, x, y, z);
        
        /// <summary>
        /// Returns bVector4.xxyw swizzling.
        /// </summary>
        public bVector4 xxyw => new bVector4(x, x, y, w);
        
        /// <summary>
        /// Returns bVector4.rrga swizzling (equivalent to bVector4.xxyw).
        /// </summary>
        public bVector4 rrga => new bVector4(x, x, y, w);
        
        /// <summary>
        /// Returns bVector4.xxz swizzling.
        /// </summary>
        public bVector3 xxz => new bVector3(x, x, z);
        
        /// <summary>
        /// Returns bVector4.rrb swizzling (equivalent to bVector4.xxz).
        /// </summary>
        public bVector3 rrb => new bVector3(x, x, z);
        
        /// <summary>
        /// Returns bVector4.xxzx swizzling.
        /// </summary>
        public bVector4 xxzx => new bVector4(x, x, z, x);
        
        /// <summary>
        /// Returns bVector4.rrbr swizzling (equivalent to bVector4.xxzx).
        /// </summary>
        public bVector4 rrbr => new bVector4(x, x, z, x);
        
        /// <summary>
        /// Returns bVector4.xxzy swizzling.
        /// </summary>
        public bVector4 xxzy => new bVector4(x, x, z, y);
        
        /// <summary>
        /// Returns bVector4.rrbg swizzling (equivalent to bVector4.xxzy).
        /// </summary>
        public bVector4 rrbg => new bVector4(x, x, z, y);
        
        /// <summary>
        /// Returns bVector4.xxzz swizzling.
        /// </summary>
        public bVector4 xxzz => new bVector4(x, x, z, z);
        
        /// <summary>
        /// Returns bVector4.rrbb swizzling (equivalent to bVector4.xxzz).
        /// </summary>
        public bVector4 rrbb => new bVector4(x, x, z, z);
        
        /// <summary>
        /// Returns bVector4.xxzw swizzling.
        /// </summary>
        public bVector4 xxzw => new bVector4(x, x, z, w);
        
        /// <summary>
        /// Returns bVector4.rrba swizzling (equivalent to bVector4.xxzw).
        /// </summary>
        public bVector4 rrba => new bVector4(x, x, z, w);
        
        /// <summary>
        /// Returns bVector4.xxw swizzling.
        /// </summary>
        public bVector3 xxw => new bVector3(x, x, w);
        
        /// <summary>
        /// Returns bVector4.rra swizzling (equivalent to bVector4.xxw).
        /// </summary>
        public bVector3 rra => new bVector3(x, x, w);
        
        /// <summary>
        /// Returns bVector4.xxwx swizzling.
        /// </summary>
        public bVector4 xxwx => new bVector4(x, x, w, x);
        
        /// <summary>
        /// Returns bVector4.rrar swizzling (equivalent to bVector4.xxwx).
        /// </summary>
        public bVector4 rrar => new bVector4(x, x, w, x);
        
        /// <summary>
        /// Returns bVector4.xxwy swizzling.
        /// </summary>
        public bVector4 xxwy => new bVector4(x, x, w, y);
        
        /// <summary>
        /// Returns bVector4.rrag swizzling (equivalent to bVector4.xxwy).
        /// </summary>
        public bVector4 rrag => new bVector4(x, x, w, y);
        
        /// <summary>
        /// Returns bVector4.xxwz swizzling.
        /// </summary>
        public bVector4 xxwz => new bVector4(x, x, w, z);
        
        /// <summary>
        /// Returns bVector4.rrab swizzling (equivalent to bVector4.xxwz).
        /// </summary>
        public bVector4 rrab => new bVector4(x, x, w, z);
        
        /// <summary>
        /// Returns bVector4.xxww swizzling.
        /// </summary>
        public bVector4 xxww => new bVector4(x, x, w, w);
        
        /// <summary>
        /// Returns bVector4.rraa swizzling (equivalent to bVector4.xxww).
        /// </summary>
        public bVector4 rraa => new bVector4(x, x, w, w);
        
        /// <summary>
        /// Returns bVector4.xy swizzling.
        /// </summary>
        public bVector2 xy => new bVector2(x, y);
        
        /// <summary>
        /// Returns bVector4.rg swizzling (equivalent to bVector4.xy).
        /// </summary>
        public bVector2 rg => new bVector2(x, y);
        
        /// <summary>
        /// Returns bVector4.xyx swizzling.
        /// </summary>
        public bVector3 xyx => new bVector3(x, y, x);
        
        /// <summary>
        /// Returns bVector4.rgr swizzling (equivalent to bVector4.xyx).
        /// </summary>
        public bVector3 rgr => new bVector3(x, y, x);
        
        /// <summary>
        /// Returns bVector4.xyxx swizzling.
        /// </summary>
        public bVector4 xyxx => new bVector4(x, y, x, x);
        
        /// <summary>
        /// Returns bVector4.rgrr swizzling (equivalent to bVector4.xyxx).
        /// </summary>
        public bVector4 rgrr => new bVector4(x, y, x, x);
        
        /// <summary>
        /// Returns bVector4.xyxy swizzling.
        /// </summary>
        public bVector4 xyxy => new bVector4(x, y, x, y);
        
        /// <summary>
        /// Returns bVector4.rgrg swizzling (equivalent to bVector4.xyxy).
        /// </summary>
        public bVector4 rgrg => new bVector4(x, y, x, y);
        
        /// <summary>
        /// Returns bVector4.xyxz swizzling.
        /// </summary>
        public bVector4 xyxz => new bVector4(x, y, x, z);
        
        /// <summary>
        /// Returns bVector4.rgrb swizzling (equivalent to bVector4.xyxz).
        /// </summary>
        public bVector4 rgrb => new bVector4(x, y, x, z);
        
        /// <summary>
        /// Returns bVector4.xyxw swizzling.
        /// </summary>
        public bVector4 xyxw => new bVector4(x, y, x, w);
        
        /// <summary>
        /// Returns bVector4.rgra swizzling (equivalent to bVector4.xyxw).
        /// </summary>
        public bVector4 rgra => new bVector4(x, y, x, w);
        
        /// <summary>
        /// Returns bVector4.xyy swizzling.
        /// </summary>
        public bVector3 xyy => new bVector3(x, y, y);
        
        /// <summary>
        /// Returns bVector4.rgg swizzling (equivalent to bVector4.xyy).
        /// </summary>
        public bVector3 rgg => new bVector3(x, y, y);
        
        /// <summary>
        /// Returns bVector4.xyyx swizzling.
        /// </summary>
        public bVector4 xyyx => new bVector4(x, y, y, x);
        
        /// <summary>
        /// Returns bVector4.rggr swizzling (equivalent to bVector4.xyyx).
        /// </summary>
        public bVector4 rggr => new bVector4(x, y, y, x);
        
        /// <summary>
        /// Returns bVector4.xyyy swizzling.
        /// </summary>
        public bVector4 xyyy => new bVector4(x, y, y, y);
        
        /// <summary>
        /// Returns bVector4.rggg swizzling (equivalent to bVector4.xyyy).
        /// </summary>
        public bVector4 rggg => new bVector4(x, y, y, y);
        
        /// <summary>
        /// Returns bVector4.xyyz swizzling.
        /// </summary>
        public bVector4 xyyz => new bVector4(x, y, y, z);
        
        /// <summary>
        /// Returns bVector4.rggb swizzling (equivalent to bVector4.xyyz).
        /// </summary>
        public bVector4 rggb => new bVector4(x, y, y, z);
        
        /// <summary>
        /// Returns bVector4.xyyw swizzling.
        /// </summary>
        public bVector4 xyyw => new bVector4(x, y, y, w);
        
        /// <summary>
        /// Returns bVector4.rgga swizzling (equivalent to bVector4.xyyw).
        /// </summary>
        public bVector4 rgga => new bVector4(x, y, y, w);
        
        /// <summary>
        /// Returns bVector4.xyz swizzling.
        /// </summary>
        public bVector3 xyz => new bVector3(x, y, z);
        
        /// <summary>
        /// Returns bVector4.rgb swizzling (equivalent to bVector4.xyz).
        /// </summary>
        public bVector3 rgb => new bVector3(x, y, z);
        
        /// <summary>
        /// Returns bVector4.xyzx swizzling.
        /// </summary>
        public bVector4 xyzx => new bVector4(x, y, z, x);
        
        /// <summary>
        /// Returns bVector4.rgbr swizzling (equivalent to bVector4.xyzx).
        /// </summary>
        public bVector4 rgbr => new bVector4(x, y, z, x);
        
        /// <summary>
        /// Returns bVector4.xyzy swizzling.
        /// </summary>
        public bVector4 xyzy => new bVector4(x, y, z, y);
        
        /// <summary>
        /// Returns bVector4.rgbg swizzling (equivalent to bVector4.xyzy).
        /// </summary>
        public bVector4 rgbg => new bVector4(x, y, z, y);
        
        /// <summary>
        /// Returns bVector4.xyzz swizzling.
        /// </summary>
        public bVector4 xyzz => new bVector4(x, y, z, z);
        
        /// <summary>
        /// Returns bVector4.rgbb swizzling (equivalent to bVector4.xyzz).
        /// </summary>
        public bVector4 rgbb => new bVector4(x, y, z, z);
        
        /// <summary>
        /// Returns bVector4.xyzw swizzling.
        /// </summary>
        public bVector4 xyzw => new bVector4(x, y, z, w);
        
        /// <summary>
        /// Returns bVector4.rgba swizzling (equivalent to bVector4.xyzw).
        /// </summary>
        public bVector4 rgba => new bVector4(x, y, z, w);
        
        /// <summary>
        /// Returns bVector4.xyw swizzling.
        /// </summary>
        public bVector3 xyw => new bVector3(x, y, w);
        
        /// <summary>
        /// Returns bVector4.rga swizzling (equivalent to bVector4.xyw).
        /// </summary>
        public bVector3 rga => new bVector3(x, y, w);
        
        /// <summary>
        /// Returns bVector4.xywx swizzling.
        /// </summary>
        public bVector4 xywx => new bVector4(x, y, w, x);
        
        /// <summary>
        /// Returns bVector4.rgar swizzling (equivalent to bVector4.xywx).
        /// </summary>
        public bVector4 rgar => new bVector4(x, y, w, x);
        
        /// <summary>
        /// Returns bVector4.xywy swizzling.
        /// </summary>
        public bVector4 xywy => new bVector4(x, y, w, y);
        
        /// <summary>
        /// Returns bVector4.rgag swizzling (equivalent to bVector4.xywy).
        /// </summary>
        public bVector4 rgag => new bVector4(x, y, w, y);
        
        /// <summary>
        /// Returns bVector4.xywz swizzling.
        /// </summary>
        public bVector4 xywz => new bVector4(x, y, w, z);
        
        /// <summary>
        /// Returns bVector4.rgab swizzling (equivalent to bVector4.xywz).
        /// </summary>
        public bVector4 rgab => new bVector4(x, y, w, z);
        
        /// <summary>
        /// Returns bVector4.xyww swizzling.
        /// </summary>
        public bVector4 xyww => new bVector4(x, y, w, w);
        
        /// <summary>
        /// Returns bVector4.rgaa swizzling (equivalent to bVector4.xyww).
        /// </summary>
        public bVector4 rgaa => new bVector4(x, y, w, w);
        
        /// <summary>
        /// Returns bVector4.xz swizzling.
        /// </summary>
        public bVector2 xz => new bVector2(x, z);
        
        /// <summary>
        /// Returns bVector4.rb swizzling (equivalent to bVector4.xz).
        /// </summary>
        public bVector2 rb => new bVector2(x, z);
        
        /// <summary>
        /// Returns bVector4.xzx swizzling.
        /// </summary>
        public bVector3 xzx => new bVector3(x, z, x);
        
        /// <summary>
        /// Returns bVector4.rbr swizzling (equivalent to bVector4.xzx).
        /// </summary>
        public bVector3 rbr => new bVector3(x, z, x);
        
        /// <summary>
        /// Returns bVector4.xzxx swizzling.
        /// </summary>
        public bVector4 xzxx => new bVector4(x, z, x, x);
        
        /// <summary>
        /// Returns bVector4.rbrr swizzling (equivalent to bVector4.xzxx).
        /// </summary>
        public bVector4 rbrr => new bVector4(x, z, x, x);
        
        /// <summary>
        /// Returns bVector4.xzxy swizzling.
        /// </summary>
        public bVector4 xzxy => new bVector4(x, z, x, y);
        
        /// <summary>
        /// Returns bVector4.rbrg swizzling (equivalent to bVector4.xzxy).
        /// </summary>
        public bVector4 rbrg => new bVector4(x, z, x, y);
        
        /// <summary>
        /// Returns bVector4.xzxz swizzling.
        /// </summary>
        public bVector4 xzxz => new bVector4(x, z, x, z);
        
        /// <summary>
        /// Returns bVector4.rbrb swizzling (equivalent to bVector4.xzxz).
        /// </summary>
        public bVector4 rbrb => new bVector4(x, z, x, z);
        
        /// <summary>
        /// Returns bVector4.xzxw swizzling.
        /// </summary>
        public bVector4 xzxw => new bVector4(x, z, x, w);
        
        /// <summary>
        /// Returns bVector4.rbra swizzling (equivalent to bVector4.xzxw).
        /// </summary>
        public bVector4 rbra => new bVector4(x, z, x, w);
        
        /// <summary>
        /// Returns bVector4.xzy swizzling.
        /// </summary>
        public bVector3 xzy => new bVector3(x, z, y);
        
        /// <summary>
        /// Returns bVector4.rbg swizzling (equivalent to bVector4.xzy).
        /// </summary>
        public bVector3 rbg => new bVector3(x, z, y);
        
        /// <summary>
        /// Returns bVector4.xzyx swizzling.
        /// </summary>
        public bVector4 xzyx => new bVector4(x, z, y, x);
        
        /// <summary>
        /// Returns bVector4.rbgr swizzling (equivalent to bVector4.xzyx).
        /// </summary>
        public bVector4 rbgr => new bVector4(x, z, y, x);
        
        /// <summary>
        /// Returns bVector4.xzyy swizzling.
        /// </summary>
        public bVector4 xzyy => new bVector4(x, z, y, y);
        
        /// <summary>
        /// Returns bVector4.rbgg swizzling (equivalent to bVector4.xzyy).
        /// </summary>
        public bVector4 rbgg => new bVector4(x, z, y, y);
        
        /// <summary>
        /// Returns bVector4.xzyz swizzling.
        /// </summary>
        public bVector4 xzyz => new bVector4(x, z, y, z);
        
        /// <summary>
        /// Returns bVector4.rbgb swizzling (equivalent to bVector4.xzyz).
        /// </summary>
        public bVector4 rbgb => new bVector4(x, z, y, z);
        
        /// <summary>
        /// Returns bVector4.xzyw swizzling.
        /// </summary>
        public bVector4 xzyw => new bVector4(x, z, y, w);
        
        /// <summary>
        /// Returns bVector4.rbga swizzling (equivalent to bVector4.xzyw).
        /// </summary>
        public bVector4 rbga => new bVector4(x, z, y, w);
        
        /// <summary>
        /// Returns bVector4.xzz swizzling.
        /// </summary>
        public bVector3 xzz => new bVector3(x, z, z);
        
        /// <summary>
        /// Returns bVector4.rbb swizzling (equivalent to bVector4.xzz).
        /// </summary>
        public bVector3 rbb => new bVector3(x, z, z);
        
        /// <summary>
        /// Returns bVector4.xzzx swizzling.
        /// </summary>
        public bVector4 xzzx => new bVector4(x, z, z, x);
        
        /// <summary>
        /// Returns bVector4.rbbr swizzling (equivalent to bVector4.xzzx).
        /// </summary>
        public bVector4 rbbr => new bVector4(x, z, z, x);
        
        /// <summary>
        /// Returns bVector4.xzzy swizzling.
        /// </summary>
        public bVector4 xzzy => new bVector4(x, z, z, y);
        
        /// <summary>
        /// Returns bVector4.rbbg swizzling (equivalent to bVector4.xzzy).
        /// </summary>
        public bVector4 rbbg => new bVector4(x, z, z, y);
        
        /// <summary>
        /// Returns bVector4.xzzz swizzling.
        /// </summary>
        public bVector4 xzzz => new bVector4(x, z, z, z);
        
        /// <summary>
        /// Returns bVector4.rbbb swizzling (equivalent to bVector4.xzzz).
        /// </summary>
        public bVector4 rbbb => new bVector4(x, z, z, z);
        
        /// <summary>
        /// Returns bVector4.xzzw swizzling.
        /// </summary>
        public bVector4 xzzw => new bVector4(x, z, z, w);
        
        /// <summary>
        /// Returns bVector4.rbba swizzling (equivalent to bVector4.xzzw).
        /// </summary>
        public bVector4 rbba => new bVector4(x, z, z, w);
        
        /// <summary>
        /// Returns bVector4.xzw swizzling.
        /// </summary>
        public bVector3 xzw => new bVector3(x, z, w);
        
        /// <summary>
        /// Returns bVector4.rba swizzling (equivalent to bVector4.xzw).
        /// </summary>
        public bVector3 rba => new bVector3(x, z, w);
        
        /// <summary>
        /// Returns bVector4.xzwx swizzling.
        /// </summary>
        public bVector4 xzwx => new bVector4(x, z, w, x);
        
        /// <summary>
        /// Returns bVector4.rbar swizzling (equivalent to bVector4.xzwx).
        /// </summary>
        public bVector4 rbar => new bVector4(x, z, w, x);
        
        /// <summary>
        /// Returns bVector4.xzwy swizzling.
        /// </summary>
        public bVector4 xzwy => new bVector4(x, z, w, y);
        
        /// <summary>
        /// Returns bVector4.rbag swizzling (equivalent to bVector4.xzwy).
        /// </summary>
        public bVector4 rbag => new bVector4(x, z, w, y);
        
        /// <summary>
        /// Returns bVector4.xzwz swizzling.
        /// </summary>
        public bVector4 xzwz => new bVector4(x, z, w, z);
        
        /// <summary>
        /// Returns bVector4.rbab swizzling (equivalent to bVector4.xzwz).
        /// </summary>
        public bVector4 rbab => new bVector4(x, z, w, z);
        
        /// <summary>
        /// Returns bVector4.xzww swizzling.
        /// </summary>
        public bVector4 xzww => new bVector4(x, z, w, w);
        
        /// <summary>
        /// Returns bVector4.rbaa swizzling (equivalent to bVector4.xzww).
        /// </summary>
        public bVector4 rbaa => new bVector4(x, z, w, w);
        
        /// <summary>
        /// Returns bVector4.xw swizzling.
        /// </summary>
        public bVector2 xw => new bVector2(x, w);
        
        /// <summary>
        /// Returns bVector4.ra swizzling (equivalent to bVector4.xw).
        /// </summary>
        public bVector2 ra => new bVector2(x, w);
        
        /// <summary>
        /// Returns bVector4.xwx swizzling.
        /// </summary>
        public bVector3 xwx => new bVector3(x, w, x);
        
        /// <summary>
        /// Returns bVector4.rar swizzling (equivalent to bVector4.xwx).
        /// </summary>
        public bVector3 rar => new bVector3(x, w, x);
        
        /// <summary>
        /// Returns bVector4.xwxx swizzling.
        /// </summary>
        public bVector4 xwxx => new bVector4(x, w, x, x);
        
        /// <summary>
        /// Returns bVector4.rarr swizzling (equivalent to bVector4.xwxx).
        /// </summary>
        public bVector4 rarr => new bVector4(x, w, x, x);
        
        /// <summary>
        /// Returns bVector4.xwxy swizzling.
        /// </summary>
        public bVector4 xwxy => new bVector4(x, w, x, y);
        
        /// <summary>
        /// Returns bVector4.rarg swizzling (equivalent to bVector4.xwxy).
        /// </summary>
        public bVector4 rarg => new bVector4(x, w, x, y);
        
        /// <summary>
        /// Returns bVector4.xwxz swizzling.
        /// </summary>
        public bVector4 xwxz => new bVector4(x, w, x, z);
        
        /// <summary>
        /// Returns bVector4.rarb swizzling (equivalent to bVector4.xwxz).
        /// </summary>
        public bVector4 rarb => new bVector4(x, w, x, z);
        
        /// <summary>
        /// Returns bVector4.xwxw swizzling.
        /// </summary>
        public bVector4 xwxw => new bVector4(x, w, x, w);
        
        /// <summary>
        /// Returns bVector4.rara swizzling (equivalent to bVector4.xwxw).
        /// </summary>
        public bVector4 rara => new bVector4(x, w, x, w);
        
        /// <summary>
        /// Returns bVector4.xwy swizzling.
        /// </summary>
        public bVector3 xwy => new bVector3(x, w, y);
        
        /// <summary>
        /// Returns bVector4.rag swizzling (equivalent to bVector4.xwy).
        /// </summary>
        public bVector3 rag => new bVector3(x, w, y);
        
        /// <summary>
        /// Returns bVector4.xwyx swizzling.
        /// </summary>
        public bVector4 xwyx => new bVector4(x, w, y, x);
        
        /// <summary>
        /// Returns bVector4.ragr swizzling (equivalent to bVector4.xwyx).
        /// </summary>
        public bVector4 ragr => new bVector4(x, w, y, x);
        
        /// <summary>
        /// Returns bVector4.xwyy swizzling.
        /// </summary>
        public bVector4 xwyy => new bVector4(x, w, y, y);
        
        /// <summary>
        /// Returns bVector4.ragg swizzling (equivalent to bVector4.xwyy).
        /// </summary>
        public bVector4 ragg => new bVector4(x, w, y, y);
        
        /// <summary>
        /// Returns bVector4.xwyz swizzling.
        /// </summary>
        public bVector4 xwyz => new bVector4(x, w, y, z);
        
        /// <summary>
        /// Returns bVector4.ragb swizzling (equivalent to bVector4.xwyz).
        /// </summary>
        public bVector4 ragb => new bVector4(x, w, y, z);
        
        /// <summary>
        /// Returns bVector4.xwyw swizzling.
        /// </summary>
        public bVector4 xwyw => new bVector4(x, w, y, w);
        
        /// <summary>
        /// Returns bVector4.raga swizzling (equivalent to bVector4.xwyw).
        /// </summary>
        public bVector4 raga => new bVector4(x, w, y, w);
        
        /// <summary>
        /// Returns bVector4.xwz swizzling.
        /// </summary>
        public bVector3 xwz => new bVector3(x, w, z);
        
        /// <summary>
        /// Returns bVector4.rab swizzling (equivalent to bVector4.xwz).
        /// </summary>
        public bVector3 rab => new bVector3(x, w, z);
        
        /// <summary>
        /// Returns bVector4.xwzx swizzling.
        /// </summary>
        public bVector4 xwzx => new bVector4(x, w, z, x);
        
        /// <summary>
        /// Returns bVector4.rabr swizzling (equivalent to bVector4.xwzx).
        /// </summary>
        public bVector4 rabr => new bVector4(x, w, z, x);
        
        /// <summary>
        /// Returns bVector4.xwzy swizzling.
        /// </summary>
        public bVector4 xwzy => new bVector4(x, w, z, y);
        
        /// <summary>
        /// Returns bVector4.rabg swizzling (equivalent to bVector4.xwzy).
        /// </summary>
        public bVector4 rabg => new bVector4(x, w, z, y);
        
        /// <summary>
        /// Returns bVector4.xwzz swizzling.
        /// </summary>
        public bVector4 xwzz => new bVector4(x, w, z, z);
        
        /// <summary>
        /// Returns bVector4.rabb swizzling (equivalent to bVector4.xwzz).
        /// </summary>
        public bVector4 rabb => new bVector4(x, w, z, z);
        
        /// <summary>
        /// Returns bVector4.xwzw swizzling.
        /// </summary>
        public bVector4 xwzw => new bVector4(x, w, z, w);
        
        /// <summary>
        /// Returns bVector4.raba swizzling (equivalent to bVector4.xwzw).
        /// </summary>
        public bVector4 raba => new bVector4(x, w, z, w);
        
        /// <summary>
        /// Returns bVector4.xww swizzling.
        /// </summary>
        public bVector3 xww => new bVector3(x, w, w);
        
        /// <summary>
        /// Returns bVector4.raa swizzling (equivalent to bVector4.xww).
        /// </summary>
        public bVector3 raa => new bVector3(x, w, w);
        
        /// <summary>
        /// Returns bVector4.xwwx swizzling.
        /// </summary>
        public bVector4 xwwx => new bVector4(x, w, w, x);
        
        /// <summary>
        /// Returns bVector4.raar swizzling (equivalent to bVector4.xwwx).
        /// </summary>
        public bVector4 raar => new bVector4(x, w, w, x);
        
        /// <summary>
        /// Returns bVector4.xwwy swizzling.
        /// </summary>
        public bVector4 xwwy => new bVector4(x, w, w, y);
        
        /// <summary>
        /// Returns bVector4.raag swizzling (equivalent to bVector4.xwwy).
        /// </summary>
        public bVector4 raag => new bVector4(x, w, w, y);
        
        /// <summary>
        /// Returns bVector4.xwwz swizzling.
        /// </summary>
        public bVector4 xwwz => new bVector4(x, w, w, z);
        
        /// <summary>
        /// Returns bVector4.raab swizzling (equivalent to bVector4.xwwz).
        /// </summary>
        public bVector4 raab => new bVector4(x, w, w, z);
        
        /// <summary>
        /// Returns bVector4.xwww swizzling.
        /// </summary>
        public bVector4 xwww => new bVector4(x, w, w, w);
        
        /// <summary>
        /// Returns bVector4.raaa swizzling (equivalent to bVector4.xwww).
        /// </summary>
        public bVector4 raaa => new bVector4(x, w, w, w);
        
        /// <summary>
        /// Returns bVector4.yx swizzling.
        /// </summary>
        public bVector2 yx => new bVector2(y, x);
        
        /// <summary>
        /// Returns bVector4.gr swizzling (equivalent to bVector4.yx).
        /// </summary>
        public bVector2 gr => new bVector2(y, x);
        
        /// <summary>
        /// Returns bVector4.yxx swizzling.
        /// </summary>
        public bVector3 yxx => new bVector3(y, x, x);
        
        /// <summary>
        /// Returns bVector4.grr swizzling (equivalent to bVector4.yxx).
        /// </summary>
        public bVector3 grr => new bVector3(y, x, x);
        
        /// <summary>
        /// Returns bVector4.yxxx swizzling.
        /// </summary>
        public bVector4 yxxx => new bVector4(y, x, x, x);
        
        /// <summary>
        /// Returns bVector4.grrr swizzling (equivalent to bVector4.yxxx).
        /// </summary>
        public bVector4 grrr => new bVector4(y, x, x, x);
        
        /// <summary>
        /// Returns bVector4.yxxy swizzling.
        /// </summary>
        public bVector4 yxxy => new bVector4(y, x, x, y);
        
        /// <summary>
        /// Returns bVector4.grrg swizzling (equivalent to bVector4.yxxy).
        /// </summary>
        public bVector4 grrg => new bVector4(y, x, x, y);
        
        /// <summary>
        /// Returns bVector4.yxxz swizzling.
        /// </summary>
        public bVector4 yxxz => new bVector4(y, x, x, z);
        
        /// <summary>
        /// Returns bVector4.grrb swizzling (equivalent to bVector4.yxxz).
        /// </summary>
        public bVector4 grrb => new bVector4(y, x, x, z);
        
        /// <summary>
        /// Returns bVector4.yxxw swizzling.
        /// </summary>
        public bVector4 yxxw => new bVector4(y, x, x, w);
        
        /// <summary>
        /// Returns bVector4.grra swizzling (equivalent to bVector4.yxxw).
        /// </summary>
        public bVector4 grra => new bVector4(y, x, x, w);
        
        /// <summary>
        /// Returns bVector4.yxy swizzling.
        /// </summary>
        public bVector3 yxy => new bVector3(y, x, y);
        
        /// <summary>
        /// Returns bVector4.grg swizzling (equivalent to bVector4.yxy).
        /// </summary>
        public bVector3 grg => new bVector3(y, x, y);
        
        /// <summary>
        /// Returns bVector4.yxyx swizzling.
        /// </summary>
        public bVector4 yxyx => new bVector4(y, x, y, x);
        
        /// <summary>
        /// Returns bVector4.grgr swizzling (equivalent to bVector4.yxyx).
        /// </summary>
        public bVector4 grgr => new bVector4(y, x, y, x);
        
        /// <summary>
        /// Returns bVector4.yxyy swizzling.
        /// </summary>
        public bVector4 yxyy => new bVector4(y, x, y, y);
        
        /// <summary>
        /// Returns bVector4.grgg swizzling (equivalent to bVector4.yxyy).
        /// </summary>
        public bVector4 grgg => new bVector4(y, x, y, y);
        
        /// <summary>
        /// Returns bVector4.yxyz swizzling.
        /// </summary>
        public bVector4 yxyz => new bVector4(y, x, y, z);
        
        /// <summary>
        /// Returns bVector4.grgb swizzling (equivalent to bVector4.yxyz).
        /// </summary>
        public bVector4 grgb => new bVector4(y, x, y, z);
        
        /// <summary>
        /// Returns bVector4.yxyw swizzling.
        /// </summary>
        public bVector4 yxyw => new bVector4(y, x, y, w);
        
        /// <summary>
        /// Returns bVector4.grga swizzling (equivalent to bVector4.yxyw).
        /// </summary>
        public bVector4 grga => new bVector4(y, x, y, w);
        
        /// <summary>
        /// Returns bVector4.yxz swizzling.
        /// </summary>
        public bVector3 yxz => new bVector3(y, x, z);
        
        /// <summary>
        /// Returns bVector4.grb swizzling (equivalent to bVector4.yxz).
        /// </summary>
        public bVector3 grb => new bVector3(y, x, z);
        
        /// <summary>
        /// Returns bVector4.yxzx swizzling.
        /// </summary>
        public bVector4 yxzx => new bVector4(y, x, z, x);
        
        /// <summary>
        /// Returns bVector4.grbr swizzling (equivalent to bVector4.yxzx).
        /// </summary>
        public bVector4 grbr => new bVector4(y, x, z, x);
        
        /// <summary>
        /// Returns bVector4.yxzy swizzling.
        /// </summary>
        public bVector4 yxzy => new bVector4(y, x, z, y);
        
        /// <summary>
        /// Returns bVector4.grbg swizzling (equivalent to bVector4.yxzy).
        /// </summary>
        public bVector4 grbg => new bVector4(y, x, z, y);
        
        /// <summary>
        /// Returns bVector4.yxzz swizzling.
        /// </summary>
        public bVector4 yxzz => new bVector4(y, x, z, z);
        
        /// <summary>
        /// Returns bVector4.grbb swizzling (equivalent to bVector4.yxzz).
        /// </summary>
        public bVector4 grbb => new bVector4(y, x, z, z);
        
        /// <summary>
        /// Returns bVector4.yxzw swizzling.
        /// </summary>
        public bVector4 yxzw => new bVector4(y, x, z, w);
        
        /// <summary>
        /// Returns bVector4.grba swizzling (equivalent to bVector4.yxzw).
        /// </summary>
        public bVector4 grba => new bVector4(y, x, z, w);
        
        /// <summary>
        /// Returns bVector4.yxw swizzling.
        /// </summary>
        public bVector3 yxw => new bVector3(y, x, w);
        
        /// <summary>
        /// Returns bVector4.gra swizzling (equivalent to bVector4.yxw).
        /// </summary>
        public bVector3 gra => new bVector3(y, x, w);
        
        /// <summary>
        /// Returns bVector4.yxwx swizzling.
        /// </summary>
        public bVector4 yxwx => new bVector4(y, x, w, x);
        
        /// <summary>
        /// Returns bVector4.grar swizzling (equivalent to bVector4.yxwx).
        /// </summary>
        public bVector4 grar => new bVector4(y, x, w, x);
        
        /// <summary>
        /// Returns bVector4.yxwy swizzling.
        /// </summary>
        public bVector4 yxwy => new bVector4(y, x, w, y);
        
        /// <summary>
        /// Returns bVector4.grag swizzling (equivalent to bVector4.yxwy).
        /// </summary>
        public bVector4 grag => new bVector4(y, x, w, y);
        
        /// <summary>
        /// Returns bVector4.yxwz swizzling.
        /// </summary>
        public bVector4 yxwz => new bVector4(y, x, w, z);
        
        /// <summary>
        /// Returns bVector4.grab swizzling (equivalent to bVector4.yxwz).
        /// </summary>
        public bVector4 grab => new bVector4(y, x, w, z);
        
        /// <summary>
        /// Returns bVector4.yxww swizzling.
        /// </summary>
        public bVector4 yxww => new bVector4(y, x, w, w);
        
        /// <summary>
        /// Returns bVector4.graa swizzling (equivalent to bVector4.yxww).
        /// </summary>
        public bVector4 graa => new bVector4(y, x, w, w);
        
        /// <summary>
        /// Returns bVector4.yy swizzling.
        /// </summary>
        public bVector2 yy => new bVector2(y, y);
        
        /// <summary>
        /// Returns bVector4.gg swizzling (equivalent to bVector4.yy).
        /// </summary>
        public bVector2 gg => new bVector2(y, y);
        
        /// <summary>
        /// Returns bVector4.yyx swizzling.
        /// </summary>
        public bVector3 yyx => new bVector3(y, y, x);
        
        /// <summary>
        /// Returns bVector4.ggr swizzling (equivalent to bVector4.yyx).
        /// </summary>
        public bVector3 ggr => new bVector3(y, y, x);
        
        /// <summary>
        /// Returns bVector4.yyxx swizzling.
        /// </summary>
        public bVector4 yyxx => new bVector4(y, y, x, x);
        
        /// <summary>
        /// Returns bVector4.ggrr swizzling (equivalent to bVector4.yyxx).
        /// </summary>
        public bVector4 ggrr => new bVector4(y, y, x, x);
        
        /// <summary>
        /// Returns bVector4.yyxy swizzling.
        /// </summary>
        public bVector4 yyxy => new bVector4(y, y, x, y);
        
        /// <summary>
        /// Returns bVector4.ggrg swizzling (equivalent to bVector4.yyxy).
        /// </summary>
        public bVector4 ggrg => new bVector4(y, y, x, y);
        
        /// <summary>
        /// Returns bVector4.yyxz swizzling.
        /// </summary>
        public bVector4 yyxz => new bVector4(y, y, x, z);
        
        /// <summary>
        /// Returns bVector4.ggrb swizzling (equivalent to bVector4.yyxz).
        /// </summary>
        public bVector4 ggrb => new bVector4(y, y, x, z);
        
        /// <summary>
        /// Returns bVector4.yyxw swizzling.
        /// </summary>
        public bVector4 yyxw => new bVector4(y, y, x, w);
        
        /// <summary>
        /// Returns bVector4.ggra swizzling (equivalent to bVector4.yyxw).
        /// </summary>
        public bVector4 ggra => new bVector4(y, y, x, w);
        
        /// <summary>
        /// Returns bVector4.yyy swizzling.
        /// </summary>
        public bVector3 yyy => new bVector3(y, y, y);
        
        /// <summary>
        /// Returns bVector4.ggg swizzling (equivalent to bVector4.yyy).
        /// </summary>
        public bVector3 ggg => new bVector3(y, y, y);
        
        /// <summary>
        /// Returns bVector4.yyyx swizzling.
        /// </summary>
        public bVector4 yyyx => new bVector4(y, y, y, x);
        
        /// <summary>
        /// Returns bVector4.gggr swizzling (equivalent to bVector4.yyyx).
        /// </summary>
        public bVector4 gggr => new bVector4(y, y, y, x);
        
        /// <summary>
        /// Returns bVector4.yyyy swizzling.
        /// </summary>
        public bVector4 yyyy => new bVector4(y, y, y, y);
        
        /// <summary>
        /// Returns bVector4.gggg swizzling (equivalent to bVector4.yyyy).
        /// </summary>
        public bVector4 gggg => new bVector4(y, y, y, y);
        
        /// <summary>
        /// Returns bVector4.yyyz swizzling.
        /// </summary>
        public bVector4 yyyz => new bVector4(y, y, y, z);
        
        /// <summary>
        /// Returns bVector4.gggb swizzling (equivalent to bVector4.yyyz).
        /// </summary>
        public bVector4 gggb => new bVector4(y, y, y, z);
        
        /// <summary>
        /// Returns bVector4.yyyw swizzling.
        /// </summary>
        public bVector4 yyyw => new bVector4(y, y, y, w);
        
        /// <summary>
        /// Returns bVector4.ggga swizzling (equivalent to bVector4.yyyw).
        /// </summary>
        public bVector4 ggga => new bVector4(y, y, y, w);
        
        /// <summary>
        /// Returns bVector4.yyz swizzling.
        /// </summary>
        public bVector3 yyz => new bVector3(y, y, z);
        
        /// <summary>
        /// Returns bVector4.ggb swizzling (equivalent to bVector4.yyz).
        /// </summary>
        public bVector3 ggb => new bVector3(y, y, z);
        
        /// <summary>
        /// Returns bVector4.yyzx swizzling.
        /// </summary>
        public bVector4 yyzx => new bVector4(y, y, z, x);
        
        /// <summary>
        /// Returns bVector4.ggbr swizzling (equivalent to bVector4.yyzx).
        /// </summary>
        public bVector4 ggbr => new bVector4(y, y, z, x);
        
        /// <summary>
        /// Returns bVector4.yyzy swizzling.
        /// </summary>
        public bVector4 yyzy => new bVector4(y, y, z, y);
        
        /// <summary>
        /// Returns bVector4.ggbg swizzling (equivalent to bVector4.yyzy).
        /// </summary>
        public bVector4 ggbg => new bVector4(y, y, z, y);
        
        /// <summary>
        /// Returns bVector4.yyzz swizzling.
        /// </summary>
        public bVector4 yyzz => new bVector4(y, y, z, z);
        
        /// <summary>
        /// Returns bVector4.ggbb swizzling (equivalent to bVector4.yyzz).
        /// </summary>
        public bVector4 ggbb => new bVector4(y, y, z, z);
        
        /// <summary>
        /// Returns bVector4.yyzw swizzling.
        /// </summary>
        public bVector4 yyzw => new bVector4(y, y, z, w);
        
        /// <summary>
        /// Returns bVector4.ggba swizzling (equivalent to bVector4.yyzw).
        /// </summary>
        public bVector4 ggba => new bVector4(y, y, z, w);
        
        /// <summary>
        /// Returns bVector4.yyw swizzling.
        /// </summary>
        public bVector3 yyw => new bVector3(y, y, w);
        
        /// <summary>
        /// Returns bVector4.gga swizzling (equivalent to bVector4.yyw).
        /// </summary>
        public bVector3 gga => new bVector3(y, y, w);
        
        /// <summary>
        /// Returns bVector4.yywx swizzling.
        /// </summary>
        public bVector4 yywx => new bVector4(y, y, w, x);
        
        /// <summary>
        /// Returns bVector4.ggar swizzling (equivalent to bVector4.yywx).
        /// </summary>
        public bVector4 ggar => new bVector4(y, y, w, x);
        
        /// <summary>
        /// Returns bVector4.yywy swizzling.
        /// </summary>
        public bVector4 yywy => new bVector4(y, y, w, y);
        
        /// <summary>
        /// Returns bVector4.ggag swizzling (equivalent to bVector4.yywy).
        /// </summary>
        public bVector4 ggag => new bVector4(y, y, w, y);
        
        /// <summary>
        /// Returns bVector4.yywz swizzling.
        /// </summary>
        public bVector4 yywz => new bVector4(y, y, w, z);
        
        /// <summary>
        /// Returns bVector4.ggab swizzling (equivalent to bVector4.yywz).
        /// </summary>
        public bVector4 ggab => new bVector4(y, y, w, z);
        
        /// <summary>
        /// Returns bVector4.yyww swizzling.
        /// </summary>
        public bVector4 yyww => new bVector4(y, y, w, w);
        
        /// <summary>
        /// Returns bVector4.ggaa swizzling (equivalent to bVector4.yyww).
        /// </summary>
        public bVector4 ggaa => new bVector4(y, y, w, w);
        
        /// <summary>
        /// Returns bVector4.yz swizzling.
        /// </summary>
        public bVector2 yz => new bVector2(y, z);
        
        /// <summary>
        /// Returns bVector4.gb swizzling (equivalent to bVector4.yz).
        /// </summary>
        public bVector2 gb => new bVector2(y, z);
        
        /// <summary>
        /// Returns bVector4.yzx swizzling.
        /// </summary>
        public bVector3 yzx => new bVector3(y, z, x);
        
        /// <summary>
        /// Returns bVector4.gbr swizzling (equivalent to bVector4.yzx).
        /// </summary>
        public bVector3 gbr => new bVector3(y, z, x);
        
        /// <summary>
        /// Returns bVector4.yzxx swizzling.
        /// </summary>
        public bVector4 yzxx => new bVector4(y, z, x, x);
        
        /// <summary>
        /// Returns bVector4.gbrr swizzling (equivalent to bVector4.yzxx).
        /// </summary>
        public bVector4 gbrr => new bVector4(y, z, x, x);
        
        /// <summary>
        /// Returns bVector4.yzxy swizzling.
        /// </summary>
        public bVector4 yzxy => new bVector4(y, z, x, y);
        
        /// <summary>
        /// Returns bVector4.gbrg swizzling (equivalent to bVector4.yzxy).
        /// </summary>
        public bVector4 gbrg => new bVector4(y, z, x, y);
        
        /// <summary>
        /// Returns bVector4.yzxz swizzling.
        /// </summary>
        public bVector4 yzxz => new bVector4(y, z, x, z);
        
        /// <summary>
        /// Returns bVector4.gbrb swizzling (equivalent to bVector4.yzxz).
        /// </summary>
        public bVector4 gbrb => new bVector4(y, z, x, z);
        
        /// <summary>
        /// Returns bVector4.yzxw swizzling.
        /// </summary>
        public bVector4 yzxw => new bVector4(y, z, x, w);
        
        /// <summary>
        /// Returns bVector4.gbra swizzling (equivalent to bVector4.yzxw).
        /// </summary>
        public bVector4 gbra => new bVector4(y, z, x, w);
        
        /// <summary>
        /// Returns bVector4.yzy swizzling.
        /// </summary>
        public bVector3 yzy => new bVector3(y, z, y);
        
        /// <summary>
        /// Returns bVector4.gbg swizzling (equivalent to bVector4.yzy).
        /// </summary>
        public bVector3 gbg => new bVector3(y, z, y);
        
        /// <summary>
        /// Returns bVector4.yzyx swizzling.
        /// </summary>
        public bVector4 yzyx => new bVector4(y, z, y, x);
        
        /// <summary>
        /// Returns bVector4.gbgr swizzling (equivalent to bVector4.yzyx).
        /// </summary>
        public bVector4 gbgr => new bVector4(y, z, y, x);
        
        /// <summary>
        /// Returns bVector4.yzyy swizzling.
        /// </summary>
        public bVector4 yzyy => new bVector4(y, z, y, y);
        
        /// <summary>
        /// Returns bVector4.gbgg swizzling (equivalent to bVector4.yzyy).
        /// </summary>
        public bVector4 gbgg => new bVector4(y, z, y, y);
        
        /// <summary>
        /// Returns bVector4.yzyz swizzling.
        /// </summary>
        public bVector4 yzyz => new bVector4(y, z, y, z);
        
        /// <summary>
        /// Returns bVector4.gbgb swizzling (equivalent to bVector4.yzyz).
        /// </summary>
        public bVector4 gbgb => new bVector4(y, z, y, z);
        
        /// <summary>
        /// Returns bVector4.yzyw swizzling.
        /// </summary>
        public bVector4 yzyw => new bVector4(y, z, y, w);
        
        /// <summary>
        /// Returns bVector4.gbga swizzling (equivalent to bVector4.yzyw).
        /// </summary>
        public bVector4 gbga => new bVector4(y, z, y, w);
        
        /// <summary>
        /// Returns bVector4.yzz swizzling.
        /// </summary>
        public bVector3 yzz => new bVector3(y, z, z);
        
        /// <summary>
        /// Returns bVector4.gbb swizzling (equivalent to bVector4.yzz).
        /// </summary>
        public bVector3 gbb => new bVector3(y, z, z);
        
        /// <summary>
        /// Returns bVector4.yzzx swizzling.
        /// </summary>
        public bVector4 yzzx => new bVector4(y, z, z, x);
        
        /// <summary>
        /// Returns bVector4.gbbr swizzling (equivalent to bVector4.yzzx).
        /// </summary>
        public bVector4 gbbr => new bVector4(y, z, z, x);
        
        /// <summary>
        /// Returns bVector4.yzzy swizzling.
        /// </summary>
        public bVector4 yzzy => new bVector4(y, z, z, y);
        
        /// <summary>
        /// Returns bVector4.gbbg swizzling (equivalent to bVector4.yzzy).
        /// </summary>
        public bVector4 gbbg => new bVector4(y, z, z, y);
        
        /// <summary>
        /// Returns bVector4.yzzz swizzling.
        /// </summary>
        public bVector4 yzzz => new bVector4(y, z, z, z);
        
        /// <summary>
        /// Returns bVector4.gbbb swizzling (equivalent to bVector4.yzzz).
        /// </summary>
        public bVector4 gbbb => new bVector4(y, z, z, z);
        
        /// <summary>
        /// Returns bVector4.yzzw swizzling.
        /// </summary>
        public bVector4 yzzw => new bVector4(y, z, z, w);
        
        /// <summary>
        /// Returns bVector4.gbba swizzling (equivalent to bVector4.yzzw).
        /// </summary>
        public bVector4 gbba => new bVector4(y, z, z, w);
        
        /// <summary>
        /// Returns bVector4.yzw swizzling.
        /// </summary>
        public bVector3 yzw => new bVector3(y, z, w);
        
        /// <summary>
        /// Returns bVector4.gba swizzling (equivalent to bVector4.yzw).
        /// </summary>
        public bVector3 gba => new bVector3(y, z, w);
        
        /// <summary>
        /// Returns bVector4.yzwx swizzling.
        /// </summary>
        public bVector4 yzwx => new bVector4(y, z, w, x);
        
        /// <summary>
        /// Returns bVector4.gbar swizzling (equivalent to bVector4.yzwx).
        /// </summary>
        public bVector4 gbar => new bVector4(y, z, w, x);
        
        /// <summary>
        /// Returns bVector4.yzwy swizzling.
        /// </summary>
        public bVector4 yzwy => new bVector4(y, z, w, y);
        
        /// <summary>
        /// Returns bVector4.gbag swizzling (equivalent to bVector4.yzwy).
        /// </summary>
        public bVector4 gbag => new bVector4(y, z, w, y);
        
        /// <summary>
        /// Returns bVector4.yzwz swizzling.
        /// </summary>
        public bVector4 yzwz => new bVector4(y, z, w, z);
        
        /// <summary>
        /// Returns bVector4.gbab swizzling (equivalent to bVector4.yzwz).
        /// </summary>
        public bVector4 gbab => new bVector4(y, z, w, z);
        
        /// <summary>
        /// Returns bVector4.yzww swizzling.
        /// </summary>
        public bVector4 yzww => new bVector4(y, z, w, w);
        
        /// <summary>
        /// Returns bVector4.gbaa swizzling (equivalent to bVector4.yzww).
        /// </summary>
        public bVector4 gbaa => new bVector4(y, z, w, w);
        
        /// <summary>
        /// Returns bVector4.yw swizzling.
        /// </summary>
        public bVector2 yw => new bVector2(y, w);
        
        /// <summary>
        /// Returns bVector4.ga swizzling (equivalent to bVector4.yw).
        /// </summary>
        public bVector2 ga => new bVector2(y, w);
        
        /// <summary>
        /// Returns bVector4.ywx swizzling.
        /// </summary>
        public bVector3 ywx => new bVector3(y, w, x);
        
        /// <summary>
        /// Returns bVector4.gar swizzling (equivalent to bVector4.ywx).
        /// </summary>
        public bVector3 gar => new bVector3(y, w, x);
        
        /// <summary>
        /// Returns bVector4.ywxx swizzling.
        /// </summary>
        public bVector4 ywxx => new bVector4(y, w, x, x);
        
        /// <summary>
        /// Returns bVector4.garr swizzling (equivalent to bVector4.ywxx).
        /// </summary>
        public bVector4 garr => new bVector4(y, w, x, x);
        
        /// <summary>
        /// Returns bVector4.ywxy swizzling.
        /// </summary>
        public bVector4 ywxy => new bVector4(y, w, x, y);
        
        /// <summary>
        /// Returns bVector4.garg swizzling (equivalent to bVector4.ywxy).
        /// </summary>
        public bVector4 garg => new bVector4(y, w, x, y);
        
        /// <summary>
        /// Returns bVector4.ywxz swizzling.
        /// </summary>
        public bVector4 ywxz => new bVector4(y, w, x, z);
        
        /// <summary>
        /// Returns bVector4.garb swizzling (equivalent to bVector4.ywxz).
        /// </summary>
        public bVector4 garb => new bVector4(y, w, x, z);
        
        /// <summary>
        /// Returns bVector4.ywxw swizzling.
        /// </summary>
        public bVector4 ywxw => new bVector4(y, w, x, w);
        
        /// <summary>
        /// Returns bVector4.gara swizzling (equivalent to bVector4.ywxw).
        /// </summary>
        public bVector4 gara => new bVector4(y, w, x, w);
        
        /// <summary>
        /// Returns bVector4.ywy swizzling.
        /// </summary>
        public bVector3 ywy => new bVector3(y, w, y);
        
        /// <summary>
        /// Returns bVector4.gag swizzling (equivalent to bVector4.ywy).
        /// </summary>
        public bVector3 gag => new bVector3(y, w, y);
        
        /// <summary>
        /// Returns bVector4.ywyx swizzling.
        /// </summary>
        public bVector4 ywyx => new bVector4(y, w, y, x);
        
        /// <summary>
        /// Returns bVector4.gagr swizzling (equivalent to bVector4.ywyx).
        /// </summary>
        public bVector4 gagr => new bVector4(y, w, y, x);
        
        /// <summary>
        /// Returns bVector4.ywyy swizzling.
        /// </summary>
        public bVector4 ywyy => new bVector4(y, w, y, y);
        
        /// <summary>
        /// Returns bVector4.gagg swizzling (equivalent to bVector4.ywyy).
        /// </summary>
        public bVector4 gagg => new bVector4(y, w, y, y);
        
        /// <summary>
        /// Returns bVector4.ywyz swizzling.
        /// </summary>
        public bVector4 ywyz => new bVector4(y, w, y, z);
        
        /// <summary>
        /// Returns bVector4.gagb swizzling (equivalent to bVector4.ywyz).
        /// </summary>
        public bVector4 gagb => new bVector4(y, w, y, z);
        
        /// <summary>
        /// Returns bVector4.ywyw swizzling.
        /// </summary>
        public bVector4 ywyw => new bVector4(y, w, y, w);
        
        /// <summary>
        /// Returns bVector4.gaga swizzling (equivalent to bVector4.ywyw).
        /// </summary>
        public bVector4 gaga => new bVector4(y, w, y, w);
        
        /// <summary>
        /// Returns bVector4.ywz swizzling.
        /// </summary>
        public bVector3 ywz => new bVector3(y, w, z);
        
        /// <summary>
        /// Returns bVector4.gab swizzling (equivalent to bVector4.ywz).
        /// </summary>
        public bVector3 gab => new bVector3(y, w, z);
        
        /// <summary>
        /// Returns bVector4.ywzx swizzling.
        /// </summary>
        public bVector4 ywzx => new bVector4(y, w, z, x);
        
        /// <summary>
        /// Returns bVector4.gabr swizzling (equivalent to bVector4.ywzx).
        /// </summary>
        public bVector4 gabr => new bVector4(y, w, z, x);
        
        /// <summary>
        /// Returns bVector4.ywzy swizzling.
        /// </summary>
        public bVector4 ywzy => new bVector4(y, w, z, y);
        
        /// <summary>
        /// Returns bVector4.gabg swizzling (equivalent to bVector4.ywzy).
        /// </summary>
        public bVector4 gabg => new bVector4(y, w, z, y);
        
        /// <summary>
        /// Returns bVector4.ywzz swizzling.
        /// </summary>
        public bVector4 ywzz => new bVector4(y, w, z, z);
        
        /// <summary>
        /// Returns bVector4.gabb swizzling (equivalent to bVector4.ywzz).
        /// </summary>
        public bVector4 gabb => new bVector4(y, w, z, z);
        
        /// <summary>
        /// Returns bVector4.ywzw swizzling.
        /// </summary>
        public bVector4 ywzw => new bVector4(y, w, z, w);
        
        /// <summary>
        /// Returns bVector4.gaba swizzling (equivalent to bVector4.ywzw).
        /// </summary>
        public bVector4 gaba => new bVector4(y, w, z, w);
        
        /// <summary>
        /// Returns bVector4.yww swizzling.
        /// </summary>
        public bVector3 yww => new bVector3(y, w, w);
        
        /// <summary>
        /// Returns bVector4.gaa swizzling (equivalent to bVector4.yww).
        /// </summary>
        public bVector3 gaa => new bVector3(y, w, w);
        
        /// <summary>
        /// Returns bVector4.ywwx swizzling.
        /// </summary>
        public bVector4 ywwx => new bVector4(y, w, w, x);
        
        /// <summary>
        /// Returns bVector4.gaar swizzling (equivalent to bVector4.ywwx).
        /// </summary>
        public bVector4 gaar => new bVector4(y, w, w, x);
        
        /// <summary>
        /// Returns bVector4.ywwy swizzling.
        /// </summary>
        public bVector4 ywwy => new bVector4(y, w, w, y);
        
        /// <summary>
        /// Returns bVector4.gaag swizzling (equivalent to bVector4.ywwy).
        /// </summary>
        public bVector4 gaag => new bVector4(y, w, w, y);
        
        /// <summary>
        /// Returns bVector4.ywwz swizzling.
        /// </summary>
        public bVector4 ywwz => new bVector4(y, w, w, z);
        
        /// <summary>
        /// Returns bVector4.gaab swizzling (equivalent to bVector4.ywwz).
        /// </summary>
        public bVector4 gaab => new bVector4(y, w, w, z);
        
        /// <summary>
        /// Returns bVector4.ywww swizzling.
        /// </summary>
        public bVector4 ywww => new bVector4(y, w, w, w);
        
        /// <summary>
        /// Returns bVector4.gaaa swizzling (equivalent to bVector4.ywww).
        /// </summary>
        public bVector4 gaaa => new bVector4(y, w, w, w);
        
        /// <summary>
        /// Returns bVector4.zx swizzling.
        /// </summary>
        public bVector2 zx => new bVector2(z, x);
        
        /// <summary>
        /// Returns bVector4.br swizzling (equivalent to bVector4.zx).
        /// </summary>
        public bVector2 br => new bVector2(z, x);
        
        /// <summary>
        /// Returns bVector4.zxx swizzling.
        /// </summary>
        public bVector3 zxx => new bVector3(z, x, x);
        
        /// <summary>
        /// Returns bVector4.brr swizzling (equivalent to bVector4.zxx).
        /// </summary>
        public bVector3 brr => new bVector3(z, x, x);
        
        /// <summary>
        /// Returns bVector4.zxxx swizzling.
        /// </summary>
        public bVector4 zxxx => new bVector4(z, x, x, x);
        
        /// <summary>
        /// Returns bVector4.brrr swizzling (equivalent to bVector4.zxxx).
        /// </summary>
        public bVector4 brrr => new bVector4(z, x, x, x);
        
        /// <summary>
        /// Returns bVector4.zxxy swizzling.
        /// </summary>
        public bVector4 zxxy => new bVector4(z, x, x, y);
        
        /// <summary>
        /// Returns bVector4.brrg swizzling (equivalent to bVector4.zxxy).
        /// </summary>
        public bVector4 brrg => new bVector4(z, x, x, y);
        
        /// <summary>
        /// Returns bVector4.zxxz swizzling.
        /// </summary>
        public bVector4 zxxz => new bVector4(z, x, x, z);
        
        /// <summary>
        /// Returns bVector4.brrb swizzling (equivalent to bVector4.zxxz).
        /// </summary>
        public bVector4 brrb => new bVector4(z, x, x, z);
        
        /// <summary>
        /// Returns bVector4.zxxw swizzling.
        /// </summary>
        public bVector4 zxxw => new bVector4(z, x, x, w);
        
        /// <summary>
        /// Returns bVector4.brra swizzling (equivalent to bVector4.zxxw).
        /// </summary>
        public bVector4 brra => new bVector4(z, x, x, w);
        
        /// <summary>
        /// Returns bVector4.zxy swizzling.
        /// </summary>
        public bVector3 zxy => new bVector3(z, x, y);
        
        /// <summary>
        /// Returns bVector4.brg swizzling (equivalent to bVector4.zxy).
        /// </summary>
        public bVector3 brg => new bVector3(z, x, y);
        
        /// <summary>
        /// Returns bVector4.zxyx swizzling.
        /// </summary>
        public bVector4 zxyx => new bVector4(z, x, y, x);
        
        /// <summary>
        /// Returns bVector4.brgr swizzling (equivalent to bVector4.zxyx).
        /// </summary>
        public bVector4 brgr => new bVector4(z, x, y, x);
        
        /// <summary>
        /// Returns bVector4.zxyy swizzling.
        /// </summary>
        public bVector4 zxyy => new bVector4(z, x, y, y);
        
        /// <summary>
        /// Returns bVector4.brgg swizzling (equivalent to bVector4.zxyy).
        /// </summary>
        public bVector4 brgg => new bVector4(z, x, y, y);
        
        /// <summary>
        /// Returns bVector4.zxyz swizzling.
        /// </summary>
        public bVector4 zxyz => new bVector4(z, x, y, z);
        
        /// <summary>
        /// Returns bVector4.brgb swizzling (equivalent to bVector4.zxyz).
        /// </summary>
        public bVector4 brgb => new bVector4(z, x, y, z);
        
        /// <summary>
        /// Returns bVector4.zxyw swizzling.
        /// </summary>
        public bVector4 zxyw => new bVector4(z, x, y, w);
        
        /// <summary>
        /// Returns bVector4.brga swizzling (equivalent to bVector4.zxyw).
        /// </summary>
        public bVector4 brga => new bVector4(z, x, y, w);
        
        /// <summary>
        /// Returns bVector4.zxz swizzling.
        /// </summary>
        public bVector3 zxz => new bVector3(z, x, z);
        
        /// <summary>
        /// Returns bVector4.brb swizzling (equivalent to bVector4.zxz).
        /// </summary>
        public bVector3 brb => new bVector3(z, x, z);
        
        /// <summary>
        /// Returns bVector4.zxzx swizzling.
        /// </summary>
        public bVector4 zxzx => new bVector4(z, x, z, x);
        
        /// <summary>
        /// Returns bVector4.brbr swizzling (equivalent to bVector4.zxzx).
        /// </summary>
        public bVector4 brbr => new bVector4(z, x, z, x);
        
        /// <summary>
        /// Returns bVector4.zxzy swizzling.
        /// </summary>
        public bVector4 zxzy => new bVector4(z, x, z, y);
        
        /// <summary>
        /// Returns bVector4.brbg swizzling (equivalent to bVector4.zxzy).
        /// </summary>
        public bVector4 brbg => new bVector4(z, x, z, y);
        
        /// <summary>
        /// Returns bVector4.zxzz swizzling.
        /// </summary>
        public bVector4 zxzz => new bVector4(z, x, z, z);
        
        /// <summary>
        /// Returns bVector4.brbb swizzling (equivalent to bVector4.zxzz).
        /// </summary>
        public bVector4 brbb => new bVector4(z, x, z, z);
        
        /// <summary>
        /// Returns bVector4.zxzw swizzling.
        /// </summary>
        public bVector4 zxzw => new bVector4(z, x, z, w);
        
        /// <summary>
        /// Returns bVector4.brba swizzling (equivalent to bVector4.zxzw).
        /// </summary>
        public bVector4 brba => new bVector4(z, x, z, w);
        
        /// <summary>
        /// Returns bVector4.zxw swizzling.
        /// </summary>
        public bVector3 zxw => new bVector3(z, x, w);
        
        /// <summary>
        /// Returns bVector4.bra swizzling (equivalent to bVector4.zxw).
        /// </summary>
        public bVector3 bra => new bVector3(z, x, w);
        
        /// <summary>
        /// Returns bVector4.zxwx swizzling.
        /// </summary>
        public bVector4 zxwx => new bVector4(z, x, w, x);
        
        /// <summary>
        /// Returns bVector4.brar swizzling (equivalent to bVector4.zxwx).
        /// </summary>
        public bVector4 brar => new bVector4(z, x, w, x);
        
        /// <summary>
        /// Returns bVector4.zxwy swizzling.
        /// </summary>
        public bVector4 zxwy => new bVector4(z, x, w, y);
        
        /// <summary>
        /// Returns bVector4.brag swizzling (equivalent to bVector4.zxwy).
        /// </summary>
        public bVector4 brag => new bVector4(z, x, w, y);
        
        /// <summary>
        /// Returns bVector4.zxwz swizzling.
        /// </summary>
        public bVector4 zxwz => new bVector4(z, x, w, z);
        
        /// <summary>
        /// Returns bVector4.brab swizzling (equivalent to bVector4.zxwz).
        /// </summary>
        public bVector4 brab => new bVector4(z, x, w, z);
        
        /// <summary>
        /// Returns bVector4.zxww swizzling.
        /// </summary>
        public bVector4 zxww => new bVector4(z, x, w, w);
        
        /// <summary>
        /// Returns bVector4.braa swizzling (equivalent to bVector4.zxww).
        /// </summary>
        public bVector4 braa => new bVector4(z, x, w, w);
        
        /// <summary>
        /// Returns bVector4.zy swizzling.
        /// </summary>
        public bVector2 zy => new bVector2(z, y);
        
        /// <summary>
        /// Returns bVector4.bg swizzling (equivalent to bVector4.zy).
        /// </summary>
        public bVector2 bg => new bVector2(z, y);
        
        /// <summary>
        /// Returns bVector4.zyx swizzling.
        /// </summary>
        public bVector3 zyx => new bVector3(z, y, x);
        
        /// <summary>
        /// Returns bVector4.bgr swizzling (equivalent to bVector4.zyx).
        /// </summary>
        public bVector3 bgr => new bVector3(z, y, x);
        
        /// <summary>
        /// Returns bVector4.zyxx swizzling.
        /// </summary>
        public bVector4 zyxx => new bVector4(z, y, x, x);
        
        /// <summary>
        /// Returns bVector4.bgrr swizzling (equivalent to bVector4.zyxx).
        /// </summary>
        public bVector4 bgrr => new bVector4(z, y, x, x);
        
        /// <summary>
        /// Returns bVector4.zyxy swizzling.
        /// </summary>
        public bVector4 zyxy => new bVector4(z, y, x, y);
        
        /// <summary>
        /// Returns bVector4.bgrg swizzling (equivalent to bVector4.zyxy).
        /// </summary>
        public bVector4 bgrg => new bVector4(z, y, x, y);
        
        /// <summary>
        /// Returns bVector4.zyxz swizzling.
        /// </summary>
        public bVector4 zyxz => new bVector4(z, y, x, z);
        
        /// <summary>
        /// Returns bVector4.bgrb swizzling (equivalent to bVector4.zyxz).
        /// </summary>
        public bVector4 bgrb => new bVector4(z, y, x, z);
        
        /// <summary>
        /// Returns bVector4.zyxw swizzling.
        /// </summary>
        public bVector4 zyxw => new bVector4(z, y, x, w);
        
        /// <summary>
        /// Returns bVector4.bgra swizzling (equivalent to bVector4.zyxw).
        /// </summary>
        public bVector4 bgra => new bVector4(z, y, x, w);
        
        /// <summary>
        /// Returns bVector4.zyy swizzling.
        /// </summary>
        public bVector3 zyy => new bVector3(z, y, y);
        
        /// <summary>
        /// Returns bVector4.bgg swizzling (equivalent to bVector4.zyy).
        /// </summary>
        public bVector3 bgg => new bVector3(z, y, y);
        
        /// <summary>
        /// Returns bVector4.zyyx swizzling.
        /// </summary>
        public bVector4 zyyx => new bVector4(z, y, y, x);
        
        /// <summary>
        /// Returns bVector4.bggr swizzling (equivalent to bVector4.zyyx).
        /// </summary>
        public bVector4 bggr => new bVector4(z, y, y, x);
        
        /// <summary>
        /// Returns bVector4.zyyy swizzling.
        /// </summary>
        public bVector4 zyyy => new bVector4(z, y, y, y);
        
        /// <summary>
        /// Returns bVector4.bggg swizzling (equivalent to bVector4.zyyy).
        /// </summary>
        public bVector4 bggg => new bVector4(z, y, y, y);
        
        /// <summary>
        /// Returns bVector4.zyyz swizzling.
        /// </summary>
        public bVector4 zyyz => new bVector4(z, y, y, z);
        
        /// <summary>
        /// Returns bVector4.bggb swizzling (equivalent to bVector4.zyyz).
        /// </summary>
        public bVector4 bggb => new bVector4(z, y, y, z);
        
        /// <summary>
        /// Returns bVector4.zyyw swizzling.
        /// </summary>
        public bVector4 zyyw => new bVector4(z, y, y, w);
        
        /// <summary>
        /// Returns bVector4.bgga swizzling (equivalent to bVector4.zyyw).
        /// </summary>
        public bVector4 bgga => new bVector4(z, y, y, w);
        
        /// <summary>
        /// Returns bVector4.zyz swizzling.
        /// </summary>
        public bVector3 zyz => new bVector3(z, y, z);
        
        /// <summary>
        /// Returns bVector4.bgb swizzling (equivalent to bVector4.zyz).
        /// </summary>
        public bVector3 bgb => new bVector3(z, y, z);
        
        /// <summary>
        /// Returns bVector4.zyzx swizzling.
        /// </summary>
        public bVector4 zyzx => new bVector4(z, y, z, x);
        
        /// <summary>
        /// Returns bVector4.bgbr swizzling (equivalent to bVector4.zyzx).
        /// </summary>
        public bVector4 bgbr => new bVector4(z, y, z, x);
        
        /// <summary>
        /// Returns bVector4.zyzy swizzling.
        /// </summary>
        public bVector4 zyzy => new bVector4(z, y, z, y);
        
        /// <summary>
        /// Returns bVector4.bgbg swizzling (equivalent to bVector4.zyzy).
        /// </summary>
        public bVector4 bgbg => new bVector4(z, y, z, y);
        
        /// <summary>
        /// Returns bVector4.zyzz swizzling.
        /// </summary>
        public bVector4 zyzz => new bVector4(z, y, z, z);
        
        /// <summary>
        /// Returns bVector4.bgbb swizzling (equivalent to bVector4.zyzz).
        /// </summary>
        public bVector4 bgbb => new bVector4(z, y, z, z);
        
        /// <summary>
        /// Returns bVector4.zyzw swizzling.
        /// </summary>
        public bVector4 zyzw => new bVector4(z, y, z, w);
        
        /// <summary>
        /// Returns bVector4.bgba swizzling (equivalent to bVector4.zyzw).
        /// </summary>
        public bVector4 bgba => new bVector4(z, y, z, w);
        
        /// <summary>
        /// Returns bVector4.zyw swizzling.
        /// </summary>
        public bVector3 zyw => new bVector3(z, y, w);
        
        /// <summary>
        /// Returns bVector4.bga swizzling (equivalent to bVector4.zyw).
        /// </summary>
        public bVector3 bga => new bVector3(z, y, w);
        
        /// <summary>
        /// Returns bVector4.zywx swizzling.
        /// </summary>
        public bVector4 zywx => new bVector4(z, y, w, x);
        
        /// <summary>
        /// Returns bVector4.bgar swizzling (equivalent to bVector4.zywx).
        /// </summary>
        public bVector4 bgar => new bVector4(z, y, w, x);
        
        /// <summary>
        /// Returns bVector4.zywy swizzling.
        /// </summary>
        public bVector4 zywy => new bVector4(z, y, w, y);
        
        /// <summary>
        /// Returns bVector4.bgag swizzling (equivalent to bVector4.zywy).
        /// </summary>
        public bVector4 bgag => new bVector4(z, y, w, y);
        
        /// <summary>
        /// Returns bVector4.zywz swizzling.
        /// </summary>
        public bVector4 zywz => new bVector4(z, y, w, z);
        
        /// <summary>
        /// Returns bVector4.bgab swizzling (equivalent to bVector4.zywz).
        /// </summary>
        public bVector4 bgab => new bVector4(z, y, w, z);
        
        /// <summary>
        /// Returns bVector4.zyww swizzling.
        /// </summary>
        public bVector4 zyww => new bVector4(z, y, w, w);
        
        /// <summary>
        /// Returns bVector4.bgaa swizzling (equivalent to bVector4.zyww).
        /// </summary>
        public bVector4 bgaa => new bVector4(z, y, w, w);
        
        /// <summary>
        /// Returns bVector4.zz swizzling.
        /// </summary>
        public bVector2 zz => new bVector2(z, z);
        
        /// <summary>
        /// Returns bVector4.bb swizzling (equivalent to bVector4.zz).
        /// </summary>
        public bVector2 bb => new bVector2(z, z);
        
        /// <summary>
        /// Returns bVector4.zzx swizzling.
        /// </summary>
        public bVector3 zzx => new bVector3(z, z, x);
        
        /// <summary>
        /// Returns bVector4.bbr swizzling (equivalent to bVector4.zzx).
        /// </summary>
        public bVector3 bbr => new bVector3(z, z, x);
        
        /// <summary>
        /// Returns bVector4.zzxx swizzling.
        /// </summary>
        public bVector4 zzxx => new bVector4(z, z, x, x);
        
        /// <summary>
        /// Returns bVector4.bbrr swizzling (equivalent to bVector4.zzxx).
        /// </summary>
        public bVector4 bbrr => new bVector4(z, z, x, x);
        
        /// <summary>
        /// Returns bVector4.zzxy swizzling.
        /// </summary>
        public bVector4 zzxy => new bVector4(z, z, x, y);
        
        /// <summary>
        /// Returns bVector4.bbrg swizzling (equivalent to bVector4.zzxy).
        /// </summary>
        public bVector4 bbrg => new bVector4(z, z, x, y);
        
        /// <summary>
        /// Returns bVector4.zzxz swizzling.
        /// </summary>
        public bVector4 zzxz => new bVector4(z, z, x, z);
        
        /// <summary>
        /// Returns bVector4.bbrb swizzling (equivalent to bVector4.zzxz).
        /// </summary>
        public bVector4 bbrb => new bVector4(z, z, x, z);
        
        /// <summary>
        /// Returns bVector4.zzxw swizzling.
        /// </summary>
        public bVector4 zzxw => new bVector4(z, z, x, w);
        
        /// <summary>
        /// Returns bVector4.bbra swizzling (equivalent to bVector4.zzxw).
        /// </summary>
        public bVector4 bbra => new bVector4(z, z, x, w);
        
        /// <summary>
        /// Returns bVector4.zzy swizzling.
        /// </summary>
        public bVector3 zzy => new bVector3(z, z, y);
        
        /// <summary>
        /// Returns bVector4.bbg swizzling (equivalent to bVector4.zzy).
        /// </summary>
        public bVector3 bbg => new bVector3(z, z, y);
        
        /// <summary>
        /// Returns bVector4.zzyx swizzling.
        /// </summary>
        public bVector4 zzyx => new bVector4(z, z, y, x);
        
        /// <summary>
        /// Returns bVector4.bbgr swizzling (equivalent to bVector4.zzyx).
        /// </summary>
        public bVector4 bbgr => new bVector4(z, z, y, x);
        
        /// <summary>
        /// Returns bVector4.zzyy swizzling.
        /// </summary>
        public bVector4 zzyy => new bVector4(z, z, y, y);
        
        /// <summary>
        /// Returns bVector4.bbgg swizzling (equivalent to bVector4.zzyy).
        /// </summary>
        public bVector4 bbgg => new bVector4(z, z, y, y);
        
        /// <summary>
        /// Returns bVector4.zzyz swizzling.
        /// </summary>
        public bVector4 zzyz => new bVector4(z, z, y, z);
        
        /// <summary>
        /// Returns bVector4.bbgb swizzling (equivalent to bVector4.zzyz).
        /// </summary>
        public bVector4 bbgb => new bVector4(z, z, y, z);
        
        /// <summary>
        /// Returns bVector4.zzyw swizzling.
        /// </summary>
        public bVector4 zzyw => new bVector4(z, z, y, w);
        
        /// <summary>
        /// Returns bVector4.bbga swizzling (equivalent to bVector4.zzyw).
        /// </summary>
        public bVector4 bbga => new bVector4(z, z, y, w);
        
        /// <summary>
        /// Returns bVector4.zzz swizzling.
        /// </summary>
        public bVector3 zzz => new bVector3(z, z, z);
        
        /// <summary>
        /// Returns bVector4.bbb swizzling (equivalent to bVector4.zzz).
        /// </summary>
        public bVector3 bbb => new bVector3(z, z, z);
        
        /// <summary>
        /// Returns bVector4.zzzx swizzling.
        /// </summary>
        public bVector4 zzzx => new bVector4(z, z, z, x);
        
        /// <summary>
        /// Returns bVector4.bbbr swizzling (equivalent to bVector4.zzzx).
        /// </summary>
        public bVector4 bbbr => new bVector4(z, z, z, x);
        
        /// <summary>
        /// Returns bVector4.zzzy swizzling.
        /// </summary>
        public bVector4 zzzy => new bVector4(z, z, z, y);
        
        /// <summary>
        /// Returns bVector4.bbbg swizzling (equivalent to bVector4.zzzy).
        /// </summary>
        public bVector4 bbbg => new bVector4(z, z, z, y);
        
        /// <summary>
        /// Returns bVector4.zzzz swizzling.
        /// </summary>
        public bVector4 zzzz => new bVector4(z, z, z, z);
        
        /// <summary>
        /// Returns bVector4.bbbb swizzling (equivalent to bVector4.zzzz).
        /// </summary>
        public bVector4 bbbb => new bVector4(z, z, z, z);
        
        /// <summary>
        /// Returns bVector4.zzzw swizzling.
        /// </summary>
        public bVector4 zzzw => new bVector4(z, z, z, w);
        
        /// <summary>
        /// Returns bVector4.bbba swizzling (equivalent to bVector4.zzzw).
        /// </summary>
        public bVector4 bbba => new bVector4(z, z, z, w);
        
        /// <summary>
        /// Returns bVector4.zzw swizzling.
        /// </summary>
        public bVector3 zzw => new bVector3(z, z, w);
        
        /// <summary>
        /// Returns bVector4.bba swizzling (equivalent to bVector4.zzw).
        /// </summary>
        public bVector3 bba => new bVector3(z, z, w);
        
        /// <summary>
        /// Returns bVector4.zzwx swizzling.
        /// </summary>
        public bVector4 zzwx => new bVector4(z, z, w, x);
        
        /// <summary>
        /// Returns bVector4.bbar swizzling (equivalent to bVector4.zzwx).
        /// </summary>
        public bVector4 bbar => new bVector4(z, z, w, x);
        
        /// <summary>
        /// Returns bVector4.zzwy swizzling.
        /// </summary>
        public bVector4 zzwy => new bVector4(z, z, w, y);
        
        /// <summary>
        /// Returns bVector4.bbag swizzling (equivalent to bVector4.zzwy).
        /// </summary>
        public bVector4 bbag => new bVector4(z, z, w, y);
        
        /// <summary>
        /// Returns bVector4.zzwz swizzling.
        /// </summary>
        public bVector4 zzwz => new bVector4(z, z, w, z);
        
        /// <summary>
        /// Returns bVector4.bbab swizzling (equivalent to bVector4.zzwz).
        /// </summary>
        public bVector4 bbab => new bVector4(z, z, w, z);
        
        /// <summary>
        /// Returns bVector4.zzww swizzling.
        /// </summary>
        public bVector4 zzww => new bVector4(z, z, w, w);
        
        /// <summary>
        /// Returns bVector4.bbaa swizzling (equivalent to bVector4.zzww).
        /// </summary>
        public bVector4 bbaa => new bVector4(z, z, w, w);
        
        /// <summary>
        /// Returns bVector4.zw swizzling.
        /// </summary>
        public bVector2 zw => new bVector2(z, w);
        
        /// <summary>
        /// Returns bVector4.ba swizzling (equivalent to bVector4.zw).
        /// </summary>
        public bVector2 ba => new bVector2(z, w);
        
        /// <summary>
        /// Returns bVector4.zwx swizzling.
        /// </summary>
        public bVector3 zwx => new bVector3(z, w, x);
        
        /// <summary>
        /// Returns bVector4.bar swizzling (equivalent to bVector4.zwx).
        /// </summary>
        public bVector3 bar => new bVector3(z, w, x);
        
        /// <summary>
        /// Returns bVector4.zwxx swizzling.
        /// </summary>
        public bVector4 zwxx => new bVector4(z, w, x, x);
        
        /// <summary>
        /// Returns bVector4.barr swizzling (equivalent to bVector4.zwxx).
        /// </summary>
        public bVector4 barr => new bVector4(z, w, x, x);
        
        /// <summary>
        /// Returns bVector4.zwxy swizzling.
        /// </summary>
        public bVector4 zwxy => new bVector4(z, w, x, y);
        
        /// <summary>
        /// Returns bVector4.barg swizzling (equivalent to bVector4.zwxy).
        /// </summary>
        public bVector4 barg => new bVector4(z, w, x, y);
        
        /// <summary>
        /// Returns bVector4.zwxz swizzling.
        /// </summary>
        public bVector4 zwxz => new bVector4(z, w, x, z);
        
        /// <summary>
        /// Returns bVector4.barb swizzling (equivalent to bVector4.zwxz).
        /// </summary>
        public bVector4 barb => new bVector4(z, w, x, z);
        
        /// <summary>
        /// Returns bVector4.zwxw swizzling.
        /// </summary>
        public bVector4 zwxw => new bVector4(z, w, x, w);
        
        /// <summary>
        /// Returns bVector4.bara swizzling (equivalent to bVector4.zwxw).
        /// </summary>
        public bVector4 bara => new bVector4(z, w, x, w);
        
        /// <summary>
        /// Returns bVector4.zwy swizzling.
        /// </summary>
        public bVector3 zwy => new bVector3(z, w, y);
        
        /// <summary>
        /// Returns bVector4.bag swizzling (equivalent to bVector4.zwy).
        /// </summary>
        public bVector3 bag => new bVector3(z, w, y);
        
        /// <summary>
        /// Returns bVector4.zwyx swizzling.
        /// </summary>
        public bVector4 zwyx => new bVector4(z, w, y, x);
        
        /// <summary>
        /// Returns bVector4.bagr swizzling (equivalent to bVector4.zwyx).
        /// </summary>
        public bVector4 bagr => new bVector4(z, w, y, x);
        
        /// <summary>
        /// Returns bVector4.zwyy swizzling.
        /// </summary>
        public bVector4 zwyy => new bVector4(z, w, y, y);
        
        /// <summary>
        /// Returns bVector4.bagg swizzling (equivalent to bVector4.zwyy).
        /// </summary>
        public bVector4 bagg => new bVector4(z, w, y, y);
        
        /// <summary>
        /// Returns bVector4.zwyz swizzling.
        /// </summary>
        public bVector4 zwyz => new bVector4(z, w, y, z);
        
        /// <summary>
        /// Returns bVector4.bagb swizzling (equivalent to bVector4.zwyz).
        /// </summary>
        public bVector4 bagb => new bVector4(z, w, y, z);
        
        /// <summary>
        /// Returns bVector4.zwyw swizzling.
        /// </summary>
        public bVector4 zwyw => new bVector4(z, w, y, w);
        
        /// <summary>
        /// Returns bVector4.baga swizzling (equivalent to bVector4.zwyw).
        /// </summary>
        public bVector4 baga => new bVector4(z, w, y, w);
        
        /// <summary>
        /// Returns bVector4.zwz swizzling.
        /// </summary>
        public bVector3 zwz => new bVector3(z, w, z);
        
        /// <summary>
        /// Returns bVector4.bab swizzling (equivalent to bVector4.zwz).
        /// </summary>
        public bVector3 bab => new bVector3(z, w, z);
        
        /// <summary>
        /// Returns bVector4.zwzx swizzling.
        /// </summary>
        public bVector4 zwzx => new bVector4(z, w, z, x);
        
        /// <summary>
        /// Returns bVector4.babr swizzling (equivalent to bVector4.zwzx).
        /// </summary>
        public bVector4 babr => new bVector4(z, w, z, x);
        
        /// <summary>
        /// Returns bVector4.zwzy swizzling.
        /// </summary>
        public bVector4 zwzy => new bVector4(z, w, z, y);
        
        /// <summary>
        /// Returns bVector4.babg swizzling (equivalent to bVector4.zwzy).
        /// </summary>
        public bVector4 babg => new bVector4(z, w, z, y);
        
        /// <summary>
        /// Returns bVector4.zwzz swizzling.
        /// </summary>
        public bVector4 zwzz => new bVector4(z, w, z, z);
        
        /// <summary>
        /// Returns bVector4.babb swizzling (equivalent to bVector4.zwzz).
        /// </summary>
        public bVector4 babb => new bVector4(z, w, z, z);
        
        /// <summary>
        /// Returns bVector4.zwzw swizzling.
        /// </summary>
        public bVector4 zwzw => new bVector4(z, w, z, w);
        
        /// <summary>
        /// Returns bVector4.baba swizzling (equivalent to bVector4.zwzw).
        /// </summary>
        public bVector4 baba => new bVector4(z, w, z, w);
        
        /// <summary>
        /// Returns bVector4.zww swizzling.
        /// </summary>
        public bVector3 zww => new bVector3(z, w, w);
        
        /// <summary>
        /// Returns bVector4.baa swizzling (equivalent to bVector4.zww).
        /// </summary>
        public bVector3 baa => new bVector3(z, w, w);
        
        /// <summary>
        /// Returns bVector4.zwwx swizzling.
        /// </summary>
        public bVector4 zwwx => new bVector4(z, w, w, x);
        
        /// <summary>
        /// Returns bVector4.baar swizzling (equivalent to bVector4.zwwx).
        /// </summary>
        public bVector4 baar => new bVector4(z, w, w, x);
        
        /// <summary>
        /// Returns bVector4.zwwy swizzling.
        /// </summary>
        public bVector4 zwwy => new bVector4(z, w, w, y);
        
        /// <summary>
        /// Returns bVector4.baag swizzling (equivalent to bVector4.zwwy).
        /// </summary>
        public bVector4 baag => new bVector4(z, w, w, y);
        
        /// <summary>
        /// Returns bVector4.zwwz swizzling.
        /// </summary>
        public bVector4 zwwz => new bVector4(z, w, w, z);
        
        /// <summary>
        /// Returns bVector4.baab swizzling (equivalent to bVector4.zwwz).
        /// </summary>
        public bVector4 baab => new bVector4(z, w, w, z);
        
        /// <summary>
        /// Returns bVector4.zwww swizzling.
        /// </summary>
        public bVector4 zwww => new bVector4(z, w, w, w);
        
        /// <summary>
        /// Returns bVector4.baaa swizzling (equivalent to bVector4.zwww).
        /// </summary>
        public bVector4 baaa => new bVector4(z, w, w, w);
        
        /// <summary>
        /// Returns bVector4.wx swizzling.
        /// </summary>
        public bVector2 wx => new bVector2(w, x);
        
        /// <summary>
        /// Returns bVector4.ar swizzling (equivalent to bVector4.wx).
        /// </summary>
        public bVector2 ar => new bVector2(w, x);
        
        /// <summary>
        /// Returns bVector4.wxx swizzling.
        /// </summary>
        public bVector3 wxx => new bVector3(w, x, x);
        
        /// <summary>
        /// Returns bVector4.arr swizzling (equivalent to bVector4.wxx).
        /// </summary>
        public bVector3 arr => new bVector3(w, x, x);
        
        /// <summary>
        /// Returns bVector4.wxxx swizzling.
        /// </summary>
        public bVector4 wxxx => new bVector4(w, x, x, x);
        
        /// <summary>
        /// Returns bVector4.arrr swizzling (equivalent to bVector4.wxxx).
        /// </summary>
        public bVector4 arrr => new bVector4(w, x, x, x);
        
        /// <summary>
        /// Returns bVector4.wxxy swizzling.
        /// </summary>
        public bVector4 wxxy => new bVector4(w, x, x, y);
        
        /// <summary>
        /// Returns bVector4.arrg swizzling (equivalent to bVector4.wxxy).
        /// </summary>
        public bVector4 arrg => new bVector4(w, x, x, y);
        
        /// <summary>
        /// Returns bVector4.wxxz swizzling.
        /// </summary>
        public bVector4 wxxz => new bVector4(w, x, x, z);
        
        /// <summary>
        /// Returns bVector4.arrb swizzling (equivalent to bVector4.wxxz).
        /// </summary>
        public bVector4 arrb => new bVector4(w, x, x, z);
        
        /// <summary>
        /// Returns bVector4.wxxw swizzling.
        /// </summary>
        public bVector4 wxxw => new bVector4(w, x, x, w);
        
        /// <summary>
        /// Returns bVector4.arra swizzling (equivalent to bVector4.wxxw).
        /// </summary>
        public bVector4 arra => new bVector4(w, x, x, w);
        
        /// <summary>
        /// Returns bVector4.wxy swizzling.
        /// </summary>
        public bVector3 wxy => new bVector3(w, x, y);
        
        /// <summary>
        /// Returns bVector4.arg swizzling (equivalent to bVector4.wxy).
        /// </summary>
        public bVector3 arg => new bVector3(w, x, y);
        
        /// <summary>
        /// Returns bVector4.wxyx swizzling.
        /// </summary>
        public bVector4 wxyx => new bVector4(w, x, y, x);
        
        /// <summary>
        /// Returns bVector4.argr swizzling (equivalent to bVector4.wxyx).
        /// </summary>
        public bVector4 argr => new bVector4(w, x, y, x);
        
        /// <summary>
        /// Returns bVector4.wxyy swizzling.
        /// </summary>
        public bVector4 wxyy => new bVector4(w, x, y, y);
        
        /// <summary>
        /// Returns bVector4.argg swizzling (equivalent to bVector4.wxyy).
        /// </summary>
        public bVector4 argg => new bVector4(w, x, y, y);
        
        /// <summary>
        /// Returns bVector4.wxyz swizzling.
        /// </summary>
        public bVector4 wxyz => new bVector4(w, x, y, z);
        
        /// <summary>
        /// Returns bVector4.argb swizzling (equivalent to bVector4.wxyz).
        /// </summary>
        public bVector4 argb => new bVector4(w, x, y, z);
        
        /// <summary>
        /// Returns bVector4.wxyw swizzling.
        /// </summary>
        public bVector4 wxyw => new bVector4(w, x, y, w);
        
        /// <summary>
        /// Returns bVector4.arga swizzling (equivalent to bVector4.wxyw).
        /// </summary>
        public bVector4 arga => new bVector4(w, x, y, w);
        
        /// <summary>
        /// Returns bVector4.wxz swizzling.
        /// </summary>
        public bVector3 wxz => new bVector3(w, x, z);
        
        /// <summary>
        /// Returns bVector4.arb swizzling (equivalent to bVector4.wxz).
        /// </summary>
        public bVector3 arb => new bVector3(w, x, z);
        
        /// <summary>
        /// Returns bVector4.wxzx swizzling.
        /// </summary>
        public bVector4 wxzx => new bVector4(w, x, z, x);
        
        /// <summary>
        /// Returns bVector4.arbr swizzling (equivalent to bVector4.wxzx).
        /// </summary>
        public bVector4 arbr => new bVector4(w, x, z, x);
        
        /// <summary>
        /// Returns bVector4.wxzy swizzling.
        /// </summary>
        public bVector4 wxzy => new bVector4(w, x, z, y);
        
        /// <summary>
        /// Returns bVector4.arbg swizzling (equivalent to bVector4.wxzy).
        /// </summary>
        public bVector4 arbg => new bVector4(w, x, z, y);
        
        /// <summary>
        /// Returns bVector4.wxzz swizzling.
        /// </summary>
        public bVector4 wxzz => new bVector4(w, x, z, z);
        
        /// <summary>
        /// Returns bVector4.arbb swizzling (equivalent to bVector4.wxzz).
        /// </summary>
        public bVector4 arbb => new bVector4(w, x, z, z);
        
        /// <summary>
        /// Returns bVector4.wxzw swizzling.
        /// </summary>
        public bVector4 wxzw => new bVector4(w, x, z, w);
        
        /// <summary>
        /// Returns bVector4.arba swizzling (equivalent to bVector4.wxzw).
        /// </summary>
        public bVector4 arba => new bVector4(w, x, z, w);
        
        /// <summary>
        /// Returns bVector4.wxw swizzling.
        /// </summary>
        public bVector3 wxw => new bVector3(w, x, w);
        
        /// <summary>
        /// Returns bVector4.ara swizzling (equivalent to bVector4.wxw).
        /// </summary>
        public bVector3 ara => new bVector3(w, x, w);
        
        /// <summary>
        /// Returns bVector4.wxwx swizzling.
        /// </summary>
        public bVector4 wxwx => new bVector4(w, x, w, x);
        
        /// <summary>
        /// Returns bVector4.arar swizzling (equivalent to bVector4.wxwx).
        /// </summary>
        public bVector4 arar => new bVector4(w, x, w, x);
        
        /// <summary>
        /// Returns bVector4.wxwy swizzling.
        /// </summary>
        public bVector4 wxwy => new bVector4(w, x, w, y);
        
        /// <summary>
        /// Returns bVector4.arag swizzling (equivalent to bVector4.wxwy).
        /// </summary>
        public bVector4 arag => new bVector4(w, x, w, y);
        
        /// <summary>
        /// Returns bVector4.wxwz swizzling.
        /// </summary>
        public bVector4 wxwz => new bVector4(w, x, w, z);
        
        /// <summary>
        /// Returns bVector4.arab swizzling (equivalent to bVector4.wxwz).
        /// </summary>
        public bVector4 arab => new bVector4(w, x, w, z);
        
        /// <summary>
        /// Returns bVector4.wxww swizzling.
        /// </summary>
        public bVector4 wxww => new bVector4(w, x, w, w);
        
        /// <summary>
        /// Returns bVector4.araa swizzling (equivalent to bVector4.wxww).
        /// </summary>
        public bVector4 araa => new bVector4(w, x, w, w);
        
        /// <summary>
        /// Returns bVector4.wy swizzling.
        /// </summary>
        public bVector2 wy => new bVector2(w, y);
        
        /// <summary>
        /// Returns bVector4.ag swizzling (equivalent to bVector4.wy).
        /// </summary>
        public bVector2 ag => new bVector2(w, y);
        
        /// <summary>
        /// Returns bVector4.wyx swizzling.
        /// </summary>
        public bVector3 wyx => new bVector3(w, y, x);
        
        /// <summary>
        /// Returns bVector4.agr swizzling (equivalent to bVector4.wyx).
        /// </summary>
        public bVector3 agr => new bVector3(w, y, x);
        
        /// <summary>
        /// Returns bVector4.wyxx swizzling.
        /// </summary>
        public bVector4 wyxx => new bVector4(w, y, x, x);
        
        /// <summary>
        /// Returns bVector4.agrr swizzling (equivalent to bVector4.wyxx).
        /// </summary>
        public bVector4 agrr => new bVector4(w, y, x, x);
        
        /// <summary>
        /// Returns bVector4.wyxy swizzling.
        /// </summary>
        public bVector4 wyxy => new bVector4(w, y, x, y);
        
        /// <summary>
        /// Returns bVector4.agrg swizzling (equivalent to bVector4.wyxy).
        /// </summary>
        public bVector4 agrg => new bVector4(w, y, x, y);
        
        /// <summary>
        /// Returns bVector4.wyxz swizzling.
        /// </summary>
        public bVector4 wyxz => new bVector4(w, y, x, z);
        
        /// <summary>
        /// Returns bVector4.agrb swizzling (equivalent to bVector4.wyxz).
        /// </summary>
        public bVector4 agrb => new bVector4(w, y, x, z);
        
        /// <summary>
        /// Returns bVector4.wyxw swizzling.
        /// </summary>
        public bVector4 wyxw => new bVector4(w, y, x, w);
        
        /// <summary>
        /// Returns bVector4.agra swizzling (equivalent to bVector4.wyxw).
        /// </summary>
        public bVector4 agra => new bVector4(w, y, x, w);
        
        /// <summary>
        /// Returns bVector4.wyy swizzling.
        /// </summary>
        public bVector3 wyy => new bVector3(w, y, y);
        
        /// <summary>
        /// Returns bVector4.agg swizzling (equivalent to bVector4.wyy).
        /// </summary>
        public bVector3 agg => new bVector3(w, y, y);
        
        /// <summary>
        /// Returns bVector4.wyyx swizzling.
        /// </summary>
        public bVector4 wyyx => new bVector4(w, y, y, x);
        
        /// <summary>
        /// Returns bVector4.aggr swizzling (equivalent to bVector4.wyyx).
        /// </summary>
        public bVector4 aggr => new bVector4(w, y, y, x);
        
        /// <summary>
        /// Returns bVector4.wyyy swizzling.
        /// </summary>
        public bVector4 wyyy => new bVector4(w, y, y, y);
        
        /// <summary>
        /// Returns bVector4.aggg swizzling (equivalent to bVector4.wyyy).
        /// </summary>
        public bVector4 aggg => new bVector4(w, y, y, y);
        
        /// <summary>
        /// Returns bVector4.wyyz swizzling.
        /// </summary>
        public bVector4 wyyz => new bVector4(w, y, y, z);
        
        /// <summary>
        /// Returns bVector4.aggb swizzling (equivalent to bVector4.wyyz).
        /// </summary>
        public bVector4 aggb => new bVector4(w, y, y, z);
        
        /// <summary>
        /// Returns bVector4.wyyw swizzling.
        /// </summary>
        public bVector4 wyyw => new bVector4(w, y, y, w);
        
        /// <summary>
        /// Returns bVector4.agga swizzling (equivalent to bVector4.wyyw).
        /// </summary>
        public bVector4 agga => new bVector4(w, y, y, w);
        
        /// <summary>
        /// Returns bVector4.wyz swizzling.
        /// </summary>
        public bVector3 wyz => new bVector3(w, y, z);
        
        /// <summary>
        /// Returns bVector4.agb swizzling (equivalent to bVector4.wyz).
        /// </summary>
        public bVector3 agb => new bVector3(w, y, z);
        
        /// <summary>
        /// Returns bVector4.wyzx swizzling.
        /// </summary>
        public bVector4 wyzx => new bVector4(w, y, z, x);
        
        /// <summary>
        /// Returns bVector4.agbr swizzling (equivalent to bVector4.wyzx).
        /// </summary>
        public bVector4 agbr => new bVector4(w, y, z, x);
        
        /// <summary>
        /// Returns bVector4.wyzy swizzling.
        /// </summary>
        public bVector4 wyzy => new bVector4(w, y, z, y);
        
        /// <summary>
        /// Returns bVector4.agbg swizzling (equivalent to bVector4.wyzy).
        /// </summary>
        public bVector4 agbg => new bVector4(w, y, z, y);
        
        /// <summary>
        /// Returns bVector4.wyzz swizzling.
        /// </summary>
        public bVector4 wyzz => new bVector4(w, y, z, z);
        
        /// <summary>
        /// Returns bVector4.agbb swizzling (equivalent to bVector4.wyzz).
        /// </summary>
        public bVector4 agbb => new bVector4(w, y, z, z);
        
        /// <summary>
        /// Returns bVector4.wyzw swizzling.
        /// </summary>
        public bVector4 wyzw => new bVector4(w, y, z, w);
        
        /// <summary>
        /// Returns bVector4.agba swizzling (equivalent to bVector4.wyzw).
        /// </summary>
        public bVector4 agba => new bVector4(w, y, z, w);
        
        /// <summary>
        /// Returns bVector4.wyw swizzling.
        /// </summary>
        public bVector3 wyw => new bVector3(w, y, w);
        
        /// <summary>
        /// Returns bVector4.aga swizzling (equivalent to bVector4.wyw).
        /// </summary>
        public bVector3 aga => new bVector3(w, y, w);
        
        /// <summary>
        /// Returns bVector4.wywx swizzling.
        /// </summary>
        public bVector4 wywx => new bVector4(w, y, w, x);
        
        /// <summary>
        /// Returns bVector4.agar swizzling (equivalent to bVector4.wywx).
        /// </summary>
        public bVector4 agar => new bVector4(w, y, w, x);
        
        /// <summary>
        /// Returns bVector4.wywy swizzling.
        /// </summary>
        public bVector4 wywy => new bVector4(w, y, w, y);
        
        /// <summary>
        /// Returns bVector4.agag swizzling (equivalent to bVector4.wywy).
        /// </summary>
        public bVector4 agag => new bVector4(w, y, w, y);
        
        /// <summary>
        /// Returns bVector4.wywz swizzling.
        /// </summary>
        public bVector4 wywz => new bVector4(w, y, w, z);
        
        /// <summary>
        /// Returns bVector4.agab swizzling (equivalent to bVector4.wywz).
        /// </summary>
        public bVector4 agab => new bVector4(w, y, w, z);
        
        /// <summary>
        /// Returns bVector4.wyww swizzling.
        /// </summary>
        public bVector4 wyww => new bVector4(w, y, w, w);
        
        /// <summary>
        /// Returns bVector4.agaa swizzling (equivalent to bVector4.wyww).
        /// </summary>
        public bVector4 agaa => new bVector4(w, y, w, w);
        
        /// <summary>
        /// Returns bVector4.wz swizzling.
        /// </summary>
        public bVector2 wz => new bVector2(w, z);
        
        /// <summary>
        /// Returns bVector4.ab swizzling (equivalent to bVector4.wz).
        /// </summary>
        public bVector2 ab => new bVector2(w, z);
        
        /// <summary>
        /// Returns bVector4.wzx swizzling.
        /// </summary>
        public bVector3 wzx => new bVector3(w, z, x);
        
        /// <summary>
        /// Returns bVector4.abr swizzling (equivalent to bVector4.wzx).
        /// </summary>
        public bVector3 abr => new bVector3(w, z, x);
        
        /// <summary>
        /// Returns bVector4.wzxx swizzling.
        /// </summary>
        public bVector4 wzxx => new bVector4(w, z, x, x);
        
        /// <summary>
        /// Returns bVector4.abrr swizzling (equivalent to bVector4.wzxx).
        /// </summary>
        public bVector4 abrr => new bVector4(w, z, x, x);
        
        /// <summary>
        /// Returns bVector4.wzxy swizzling.
        /// </summary>
        public bVector4 wzxy => new bVector4(w, z, x, y);
        
        /// <summary>
        /// Returns bVector4.abrg swizzling (equivalent to bVector4.wzxy).
        /// </summary>
        public bVector4 abrg => new bVector4(w, z, x, y);
        
        /// <summary>
        /// Returns bVector4.wzxz swizzling.
        /// </summary>
        public bVector4 wzxz => new bVector4(w, z, x, z);
        
        /// <summary>
        /// Returns bVector4.abrb swizzling (equivalent to bVector4.wzxz).
        /// </summary>
        public bVector4 abrb => new bVector4(w, z, x, z);
        
        /// <summary>
        /// Returns bVector4.wzxw swizzling.
        /// </summary>
        public bVector4 wzxw => new bVector4(w, z, x, w);
        
        /// <summary>
        /// Returns bVector4.abra swizzling (equivalent to bVector4.wzxw).
        /// </summary>
        public bVector4 abra => new bVector4(w, z, x, w);
        
        /// <summary>
        /// Returns bVector4.wzy swizzling.
        /// </summary>
        public bVector3 wzy => new bVector3(w, z, y);
        
        /// <summary>
        /// Returns bVector4.abg swizzling (equivalent to bVector4.wzy).
        /// </summary>
        public bVector3 abg => new bVector3(w, z, y);
        
        /// <summary>
        /// Returns bVector4.wzyx swizzling.
        /// </summary>
        public bVector4 wzyx => new bVector4(w, z, y, x);
        
        /// <summary>
        /// Returns bVector4.abgr swizzling (equivalent to bVector4.wzyx).
        /// </summary>
        public bVector4 abgr => new bVector4(w, z, y, x);
        
        /// <summary>
        /// Returns bVector4.wzyy swizzling.
        /// </summary>
        public bVector4 wzyy => new bVector4(w, z, y, y);
        
        /// <summary>
        /// Returns bVector4.abgg swizzling (equivalent to bVector4.wzyy).
        /// </summary>
        public bVector4 abgg => new bVector4(w, z, y, y);
        
        /// <summary>
        /// Returns bVector4.wzyz swizzling.
        /// </summary>
        public bVector4 wzyz => new bVector4(w, z, y, z);
        
        /// <summary>
        /// Returns bVector4.abgb swizzling (equivalent to bVector4.wzyz).
        /// </summary>
        public bVector4 abgb => new bVector4(w, z, y, z);
        
        /// <summary>
        /// Returns bVector4.wzyw swizzling.
        /// </summary>
        public bVector4 wzyw => new bVector4(w, z, y, w);
        
        /// <summary>
        /// Returns bVector4.abga swizzling (equivalent to bVector4.wzyw).
        /// </summary>
        public bVector4 abga => new bVector4(w, z, y, w);
        
        /// <summary>
        /// Returns bVector4.wzz swizzling.
        /// </summary>
        public bVector3 wzz => new bVector3(w, z, z);
        
        /// <summary>
        /// Returns bVector4.abb swizzling (equivalent to bVector4.wzz).
        /// </summary>
        public bVector3 abb => new bVector3(w, z, z);
        
        /// <summary>
        /// Returns bVector4.wzzx swizzling.
        /// </summary>
        public bVector4 wzzx => new bVector4(w, z, z, x);
        
        /// <summary>
        /// Returns bVector4.abbr swizzling (equivalent to bVector4.wzzx).
        /// </summary>
        public bVector4 abbr => new bVector4(w, z, z, x);
        
        /// <summary>
        /// Returns bVector4.wzzy swizzling.
        /// </summary>
        public bVector4 wzzy => new bVector4(w, z, z, y);
        
        /// <summary>
        /// Returns bVector4.abbg swizzling (equivalent to bVector4.wzzy).
        /// </summary>
        public bVector4 abbg => new bVector4(w, z, z, y);
        
        /// <summary>
        /// Returns bVector4.wzzz swizzling.
        /// </summary>
        public bVector4 wzzz => new bVector4(w, z, z, z);
        
        /// <summary>
        /// Returns bVector4.abbb swizzling (equivalent to bVector4.wzzz).
        /// </summary>
        public bVector4 abbb => new bVector4(w, z, z, z);
        
        /// <summary>
        /// Returns bVector4.wzzw swizzling.
        /// </summary>
        public bVector4 wzzw => new bVector4(w, z, z, w);
        
        /// <summary>
        /// Returns bVector4.abba swizzling (equivalent to bVector4.wzzw).
        /// </summary>
        public bVector4 abba => new bVector4(w, z, z, w);
        
        /// <summary>
        /// Returns bVector4.wzw swizzling.
        /// </summary>
        public bVector3 wzw => new bVector3(w, z, w);
        
        /// <summary>
        /// Returns bVector4.aba swizzling (equivalent to bVector4.wzw).
        /// </summary>
        public bVector3 aba => new bVector3(w, z, w);
        
        /// <summary>
        /// Returns bVector4.wzwx swizzling.
        /// </summary>
        public bVector4 wzwx => new bVector4(w, z, w, x);
        
        /// <summary>
        /// Returns bVector4.abar swizzling (equivalent to bVector4.wzwx).
        /// </summary>
        public bVector4 abar => new bVector4(w, z, w, x);
        
        /// <summary>
        /// Returns bVector4.wzwy swizzling.
        /// </summary>
        public bVector4 wzwy => new bVector4(w, z, w, y);
        
        /// <summary>
        /// Returns bVector4.abag swizzling (equivalent to bVector4.wzwy).
        /// </summary>
        public bVector4 abag => new bVector4(w, z, w, y);
        
        /// <summary>
        /// Returns bVector4.wzwz swizzling.
        /// </summary>
        public bVector4 wzwz => new bVector4(w, z, w, z);
        
        /// <summary>
        /// Returns bVector4.abab swizzling (equivalent to bVector4.wzwz).
        /// </summary>
        public bVector4 abab => new bVector4(w, z, w, z);
        
        /// <summary>
        /// Returns bVector4.wzww swizzling.
        /// </summary>
        public bVector4 wzww => new bVector4(w, z, w, w);
        
        /// <summary>
        /// Returns bVector4.abaa swizzling (equivalent to bVector4.wzww).
        /// </summary>
        public bVector4 abaa => new bVector4(w, z, w, w);
        
        /// <summary>
        /// Returns bVector4.ww swizzling.
        /// </summary>
        public bVector2 ww => new bVector2(w, w);
        
        /// <summary>
        /// Returns bVector4.aa swizzling (equivalent to bVector4.ww).
        /// </summary>
        public bVector2 aa => new bVector2(w, w);
        
        /// <summary>
        /// Returns bVector4.wwx swizzling.
        /// </summary>
        public bVector3 wwx => new bVector3(w, w, x);
        
        /// <summary>
        /// Returns bVector4.aar swizzling (equivalent to bVector4.wwx).
        /// </summary>
        public bVector3 aar => new bVector3(w, w, x);
        
        /// <summary>
        /// Returns bVector4.wwxx swizzling.
        /// </summary>
        public bVector4 wwxx => new bVector4(w, w, x, x);
        
        /// <summary>
        /// Returns bVector4.aarr swizzling (equivalent to bVector4.wwxx).
        /// </summary>
        public bVector4 aarr => new bVector4(w, w, x, x);
        
        /// <summary>
        /// Returns bVector4.wwxy swizzling.
        /// </summary>
        public bVector4 wwxy => new bVector4(w, w, x, y);
        
        /// <summary>
        /// Returns bVector4.aarg swizzling (equivalent to bVector4.wwxy).
        /// </summary>
        public bVector4 aarg => new bVector4(w, w, x, y);
        
        /// <summary>
        /// Returns bVector4.wwxz swizzling.
        /// </summary>
        public bVector4 wwxz => new bVector4(w, w, x, z);
        
        /// <summary>
        /// Returns bVector4.aarb swizzling (equivalent to bVector4.wwxz).
        /// </summary>
        public bVector4 aarb => new bVector4(w, w, x, z);
        
        /// <summary>
        /// Returns bVector4.wwxw swizzling.
        /// </summary>
        public bVector4 wwxw => new bVector4(w, w, x, w);
        
        /// <summary>
        /// Returns bVector4.aara swizzling (equivalent to bVector4.wwxw).
        /// </summary>
        public bVector4 aara => new bVector4(w, w, x, w);
        
        /// <summary>
        /// Returns bVector4.wwy swizzling.
        /// </summary>
        public bVector3 wwy => new bVector3(w, w, y);
        
        /// <summary>
        /// Returns bVector4.aag swizzling (equivalent to bVector4.wwy).
        /// </summary>
        public bVector3 aag => new bVector3(w, w, y);
        
        /// <summary>
        /// Returns bVector4.wwyx swizzling.
        /// </summary>
        public bVector4 wwyx => new bVector4(w, w, y, x);
        
        /// <summary>
        /// Returns bVector4.aagr swizzling (equivalent to bVector4.wwyx).
        /// </summary>
        public bVector4 aagr => new bVector4(w, w, y, x);
        
        /// <summary>
        /// Returns bVector4.wwyy swizzling.
        /// </summary>
        public bVector4 wwyy => new bVector4(w, w, y, y);
        
        /// <summary>
        /// Returns bVector4.aagg swizzling (equivalent to bVector4.wwyy).
        /// </summary>
        public bVector4 aagg => new bVector4(w, w, y, y);
        
        /// <summary>
        /// Returns bVector4.wwyz swizzling.
        /// </summary>
        public bVector4 wwyz => new bVector4(w, w, y, z);
        
        /// <summary>
        /// Returns bVector4.aagb swizzling (equivalent to bVector4.wwyz).
        /// </summary>
        public bVector4 aagb => new bVector4(w, w, y, z);
        
        /// <summary>
        /// Returns bVector4.wwyw swizzling.
        /// </summary>
        public bVector4 wwyw => new bVector4(w, w, y, w);
        
        /// <summary>
        /// Returns bVector4.aaga swizzling (equivalent to bVector4.wwyw).
        /// </summary>
        public bVector4 aaga => new bVector4(w, w, y, w);
        
        /// <summary>
        /// Returns bVector4.wwz swizzling.
        /// </summary>
        public bVector3 wwz => new bVector3(w, w, z);
        
        /// <summary>
        /// Returns bVector4.aab swizzling (equivalent to bVector4.wwz).
        /// </summary>
        public bVector3 aab => new bVector3(w, w, z);
        
        /// <summary>
        /// Returns bVector4.wwzx swizzling.
        /// </summary>
        public bVector4 wwzx => new bVector4(w, w, z, x);
        
        /// <summary>
        /// Returns bVector4.aabr swizzling (equivalent to bVector4.wwzx).
        /// </summary>
        public bVector4 aabr => new bVector4(w, w, z, x);
        
        /// <summary>
        /// Returns bVector4.wwzy swizzling.
        /// </summary>
        public bVector4 wwzy => new bVector4(w, w, z, y);
        
        /// <summary>
        /// Returns bVector4.aabg swizzling (equivalent to bVector4.wwzy).
        /// </summary>
        public bVector4 aabg => new bVector4(w, w, z, y);
        
        /// <summary>
        /// Returns bVector4.wwzz swizzling.
        /// </summary>
        public bVector4 wwzz => new bVector4(w, w, z, z);
        
        /// <summary>
        /// Returns bVector4.aabb swizzling (equivalent to bVector4.wwzz).
        /// </summary>
        public bVector4 aabb => new bVector4(w, w, z, z);
        
        /// <summary>
        /// Returns bVector4.wwzw swizzling.
        /// </summary>
        public bVector4 wwzw => new bVector4(w, w, z, w);
        
        /// <summary>
        /// Returns bVector4.aaba swizzling (equivalent to bVector4.wwzw).
        /// </summary>
        public bVector4 aaba => new bVector4(w, w, z, w);
        
        /// <summary>
        /// Returns bVector4.www swizzling.
        /// </summary>
        public bVector3 www => new bVector3(w, w, w);
        
        /// <summary>
        /// Returns bVector4.aaa swizzling (equivalent to bVector4.www).
        /// </summary>
        public bVector3 aaa => new bVector3(w, w, w);
        
        /// <summary>
        /// Returns bVector4.wwwx swizzling.
        /// </summary>
        public bVector4 wwwx => new bVector4(w, w, w, x);
        
        /// <summary>
        /// Returns bVector4.aaar swizzling (equivalent to bVector4.wwwx).
        /// </summary>
        public bVector4 aaar => new bVector4(w, w, w, x);
        
        /// <summary>
        /// Returns bVector4.wwwy swizzling.
        /// </summary>
        public bVector4 wwwy => new bVector4(w, w, w, y);
        
        /// <summary>
        /// Returns bVector4.aaag swizzling (equivalent to bVector4.wwwy).
        /// </summary>
        public bVector4 aaag => new bVector4(w, w, w, y);
        
        /// <summary>
        /// Returns bVector4.wwwz swizzling.
        /// </summary>
        public bVector4 wwwz => new bVector4(w, w, w, z);
        
        /// <summary>
        /// Returns bVector4.aaab swizzling (equivalent to bVector4.wwwz).
        /// </summary>
        public bVector4 aaab => new bVector4(w, w, w, z);
        
        /// <summary>
        /// Returns bVector4.wwww swizzling.
        /// </summary>
        public bVector4 wwww => new bVector4(w, w, w, w);
        
        /// <summary>
        /// Returns bVector4.aaaa swizzling (equivalent to bVector4.wwww).
        /// </summary>
        public bVector4 aaaa => new bVector4(w, w, w, w);

        #endregion

    }
}
