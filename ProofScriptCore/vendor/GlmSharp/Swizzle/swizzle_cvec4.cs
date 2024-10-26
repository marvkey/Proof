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
    /// Temporary Vector of type Complex with 4 components, used for implementing swizzling for cVector4.
    /// </summary>
    /// 

    /*
    
   
    [StructLayout(LayoutKind.Sequential)]
    public struct swizzle_cVector4
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
        
        /// <summary>
        /// w-component
        /// </summary>
        
        internal readonly Complex w;

        #endregion


        #region Constructors
        
        /// <summary>
        /// Constructor for swizzle_cVector4.
        /// </summary>
        //internal swizzle_cVector4(Complex x, Complex y, Complex z, Complex w)
        //{
        //    this.x = x;
        //    this.y = y;
        //    this.z = z;
        //    this.w = w;
        //}

        #endregion


        #region Properties
        
        /// <summary>
        /// Returns cVector4.xx swizzling.
        /// </summary>
        public cVector2 xx => new cVector2(x, x);
        
        /// <summary>
        /// Returns cVector4.rr swizzling (equivalent to cVector4.xx).
        /// </summary>
        public cVector2 rr => new cVector2(x, x);
        
        /// <summary>
        /// Returns cVector4.xxx swizzling.
        /// </summary>
        public cVector3 xxx => new cVector3(x, x, x);
        
        /// <summary>
        /// Returns cVector4.rrr swizzling (equivalent to cVector4.xxx).
        /// </summary>
        public cVector3 rrr => new cVector3(x, x, x);
        
        /// <summary>
        /// Returns cVector4.xxxx swizzling.
        /// </summary>
        public cVector4 xxxx => new cVector4(x, x, x, x);
        
        /// <summary>
        /// Returns cVector4.rrrr swizzling (equivalent to cVector4.xxxx).
        /// </summary>
        public cVector4 rrrr => new cVector4(x, x, x, x);
        
        /// <summary>
        /// Returns cVector4.xxxy swizzling.
        /// </summary>
        public cVector4 xxxy => new cVector4(x, x, x, y);
        
        /// <summary>
        /// Returns cVector4.rrrg swizzling (equivalent to cVector4.xxxy).
        /// </summary>
        public cVector4 rrrg => new cVector4(x, x, x, y);
        
        /// <summary>
        /// Returns cVector4.xxxz swizzling.
        /// </summary>
        public cVector4 xxxz => new cVector4(x, x, x, z);
        
        /// <summary>
        /// Returns cVector4.rrrb swizzling (equivalent to cVector4.xxxz).
        /// </summary>
        public cVector4 rrrb => new cVector4(x, x, x, z);
        
        /// <summary>
        /// Returns cVector4.xxxw swizzling.
        /// </summary>
        public cVector4 xxxw => new cVector4(x, x, x, w);
        
        /// <summary>
        /// Returns cVector4.rrra swizzling (equivalent to cVector4.xxxw).
        /// </summary>
        public cVector4 rrra => new cVector4(x, x, x, w);
        
        /// <summary>
        /// Returns cVector4.xxy swizzling.
        /// </summary>
        public cVector3 xxy => new cVector3(x, x, y);
        
        /// <summary>
        /// Returns cVector4.rrg swizzling (equivalent to cVector4.xxy).
        /// </summary>
        public cVector3 rrg => new cVector3(x, x, y);
        
        /// <summary>
        /// Returns cVector4.xxyx swizzling.
        /// </summary>
        public cVector4 xxyx => new cVector4(x, x, y, x);
        
        /// <summary>
        /// Returns cVector4.rrgr swizzling (equivalent to cVector4.xxyx).
        /// </summary>
        public cVector4 rrgr => new cVector4(x, x, y, x);
        
        /// <summary>
        /// Returns cVector4.xxyy swizzling.
        /// </summary>
        public cVector4 xxyy => new cVector4(x, x, y, y);
        
        /// <summary>
        /// Returns cVector4.rrgg swizzling (equivalent to cVector4.xxyy).
        /// </summary>
        public cVector4 rrgg => new cVector4(x, x, y, y);
        
        /// <summary>
        /// Returns cVector4.xxyz swizzling.
        /// </summary>
        public cVector4 xxyz => new cVector4(x, x, y, z);
        
        /// <summary>
        /// Returns cVector4.rrgb swizzling (equivalent to cVector4.xxyz).
        /// </summary>
        public cVector4 rrgb => new cVector4(x, x, y, z);
        
        /// <summary>
        /// Returns cVector4.xxyw swizzling.
        /// </summary>
        public cVector4 xxyw => new cVector4(x, x, y, w);
        
        /// <summary>
        /// Returns cVector4.rrga swizzling (equivalent to cVector4.xxyw).
        /// </summary>
        public cVector4 rrga => new cVector4(x, x, y, w);
        
        /// <summary>
        /// Returns cVector4.xxz swizzling.
        /// </summary>
        public cVector3 xxz => new cVector3(x, x, z);
        
        /// <summary>
        /// Returns cVector4.rrb swizzling (equivalent to cVector4.xxz).
        /// </summary>
        public cVector3 rrb => new cVector3(x, x, z);
        
        /// <summary>
        /// Returns cVector4.xxzx swizzling.
        /// </summary>
        public cVector4 xxzx => new cVector4(x, x, z, x);
        
        /// <summary>
        /// Returns cVector4.rrbr swizzling (equivalent to cVector4.xxzx).
        /// </summary>
        public cVector4 rrbr => new cVector4(x, x, z, x);
        
        /// <summary>
        /// Returns cVector4.xxzy swizzling.
        /// </summary>
        public cVector4 xxzy => new cVector4(x, x, z, y);
        
        /// <summary>
        /// Returns cVector4.rrbg swizzling (equivalent to cVector4.xxzy).
        /// </summary>
        public cVector4 rrbg => new cVector4(x, x, z, y);
        
        /// <summary>
        /// Returns cVector4.xxzz swizzling.
        /// </summary>
        public cVector4 xxzz => new cVector4(x, x, z, z);
        
        /// <summary>
        /// Returns cVector4.rrbb swizzling (equivalent to cVector4.xxzz).
        /// </summary>
        public cVector4 rrbb => new cVector4(x, x, z, z);
        
        /// <summary>
        /// Returns cVector4.xxzw swizzling.
        /// </summary>
        public cVector4 xxzw => new cVector4(x, x, z, w);
        
        /// <summary>
        /// Returns cVector4.rrba swizzling (equivalent to cVector4.xxzw).
        /// </summary>
        public cVector4 rrba => new cVector4(x, x, z, w);
        
        /// <summary>
        /// Returns cVector4.xxw swizzling.
        /// </summary>
        public cVector3 xxw => new cVector3(x, x, w);
        
        /// <summary>
        /// Returns cVector4.rra swizzling (equivalent to cVector4.xxw).
        /// </summary>
        public cVector3 rra => new cVector3(x, x, w);
        
        /// <summary>
        /// Returns cVector4.xxwx swizzling.
        /// </summary>
        public cVector4 xxwx => new cVector4(x, x, w, x);
        
        /// <summary>
        /// Returns cVector4.rrar swizzling (equivalent to cVector4.xxwx).
        /// </summary>
        public cVector4 rrar => new cVector4(x, x, w, x);
        
        /// <summary>
        /// Returns cVector4.xxwy swizzling.
        /// </summary>
        public cVector4 xxwy => new cVector4(x, x, w, y);
        
        /// <summary>
        /// Returns cVector4.rrag swizzling (equivalent to cVector4.xxwy).
        /// </summary>
        public cVector4 rrag => new cVector4(x, x, w, y);
        
        /// <summary>
        /// Returns cVector4.xxwz swizzling.
        /// </summary>
        public cVector4 xxwz => new cVector4(x, x, w, z);
        
        /// <summary>
        /// Returns cVector4.rrab swizzling (equivalent to cVector4.xxwz).
        /// </summary>
        public cVector4 rrab => new cVector4(x, x, w, z);
        
        /// <summary>
        /// Returns cVector4.xxww swizzling.
        /// </summary>
        public cVector4 xxww => new cVector4(x, x, w, w);
        
        /// <summary>
        /// Returns cVector4.rraa swizzling (equivalent to cVector4.xxww).
        /// </summary>
        public cVector4 rraa => new cVector4(x, x, w, w);
        
        /// <summary>
        /// Returns cVector4.xy swizzling.
        /// </summary>
        public cVector2 xy => new cVector2(x, y);
        
        /// <summary>
        /// Returns cVector4.rg swizzling (equivalent to cVector4.xy).
        /// </summary>
        public cVector2 rg => new cVector2(x, y);
        
        /// <summary>
        /// Returns cVector4.xyx swizzling.
        /// </summary>
        public cVector3 xyx => new cVector3(x, y, x);
        
        /// <summary>
        /// Returns cVector4.rgr swizzling (equivalent to cVector4.xyx).
        /// </summary>
        public cVector3 rgr => new cVector3(x, y, x);
        
        /// <summary>
        /// Returns cVector4.xyxx swizzling.
        /// </summary>
        public cVector4 xyxx => new cVector4(x, y, x, x);
        
        /// <summary>
        /// Returns cVector4.rgrr swizzling (equivalent to cVector4.xyxx).
        /// </summary>
        public cVector4 rgrr => new cVector4(x, y, x, x);
        
        /// <summary>
        /// Returns cVector4.xyxy swizzling.
        /// </summary>
        public cVector4 xyxy => new cVector4(x, y, x, y);
        
        /// <summary>
        /// Returns cVector4.rgrg swizzling (equivalent to cVector4.xyxy).
        /// </summary>
        public cVector4 rgrg => new cVector4(x, y, x, y);
        
        /// <summary>
        /// Returns cVector4.xyxz swizzling.
        /// </summary>
        public cVector4 xyxz => new cVector4(x, y, x, z);
        
        /// <summary>
        /// Returns cVector4.rgrb swizzling (equivalent to cVector4.xyxz).
        /// </summary>
        public cVector4 rgrb => new cVector4(x, y, x, z);
        
        /// <summary>
        /// Returns cVector4.xyxw swizzling.
        /// </summary>
        public cVector4 xyxw => new cVector4(x, y, x, w);
        
        /// <summary>
        /// Returns cVector4.rgra swizzling (equivalent to cVector4.xyxw).
        /// </summary>
        public cVector4 rgra => new cVector4(x, y, x, w);
        
        /// <summary>
        /// Returns cVector4.xyy swizzling.
        /// </summary>
        public cVector3 xyy => new cVector3(x, y, y);
        
        /// <summary>
        /// Returns cVector4.rgg swizzling (equivalent to cVector4.xyy).
        /// </summary>
        public cVector3 rgg => new cVector3(x, y, y);
        
        /// <summary>
        /// Returns cVector4.xyyx swizzling.
        /// </summary>
        public cVector4 xyyx => new cVector4(x, y, y, x);
        
        /// <summary>
        /// Returns cVector4.rggr swizzling (equivalent to cVector4.xyyx).
        /// </summary>
        public cVector4 rggr => new cVector4(x, y, y, x);
        
        /// <summary>
        /// Returns cVector4.xyyy swizzling.
        /// </summary>
        public cVector4 xyyy => new cVector4(x, y, y, y);
        
        /// <summary>
        /// Returns cVector4.rggg swizzling (equivalent to cVector4.xyyy).
        /// </summary>
        public cVector4 rggg => new cVector4(x, y, y, y);
        
        /// <summary>
        /// Returns cVector4.xyyz swizzling.
        /// </summary>
        public cVector4 xyyz => new cVector4(x, y, y, z);
        
        /// <summary>
        /// Returns cVector4.rggb swizzling (equivalent to cVector4.xyyz).
        /// </summary>
        public cVector4 rggb => new cVector4(x, y, y, z);
        
        /// <summary>
        /// Returns cVector4.xyyw swizzling.
        /// </summary>
        public cVector4 xyyw => new cVector4(x, y, y, w);
        
        /// <summary>
        /// Returns cVector4.rgga swizzling (equivalent to cVector4.xyyw).
        /// </summary>
        public cVector4 rgga => new cVector4(x, y, y, w);
        
        /// <summary>
        /// Returns cVector4.xyz swizzling.
        /// </summary>
        public cVector3 xyz => new cVector3(x, y, z);
        
        /// <summary>
        /// Returns cVector4.rgb swizzling (equivalent to cVector4.xyz).
        /// </summary>
        public cVector3 rgb => new cVector3(x, y, z);
        
        /// <summary>
        /// Returns cVector4.xyzx swizzling.
        /// </summary>
        public cVector4 xyzx => new cVector4(x, y, z, x);
        
        /// <summary>
        /// Returns cVector4.rgbr swizzling (equivalent to cVector4.xyzx).
        /// </summary>
        public cVector4 rgbr => new cVector4(x, y, z, x);
        
        /// <summary>
        /// Returns cVector4.xyzy swizzling.
        /// </summary>
        public cVector4 xyzy => new cVector4(x, y, z, y);
        
        /// <summary>
        /// Returns cVector4.rgbg swizzling (equivalent to cVector4.xyzy).
        /// </summary>
        public cVector4 rgbg => new cVector4(x, y, z, y);
        
        /// <summary>
        /// Returns cVector4.xyzz swizzling.
        /// </summary>
        public cVector4 xyzz => new cVector4(x, y, z, z);
        
        /// <summary>
        /// Returns cVector4.rgbb swizzling (equivalent to cVector4.xyzz).
        /// </summary>
        public cVector4 rgbb => new cVector4(x, y, z, z);
        
        /// <summary>
        /// Returns cVector4.xyzw swizzling.
        /// </summary>
        public cVector4 xyzw => new cVector4(x, y, z, w);
        
        /// <summary>
        /// Returns cVector4.rgba swizzling (equivalent to cVector4.xyzw).
        /// </summary>
        public cVector4 rgba => new cVector4(x, y, z, w);
        
        /// <summary>
        /// Returns cVector4.xyw swizzling.
        /// </summary>
        public cVector3 xyw => new cVector3(x, y, w);
        
        /// <summary>
        /// Returns cVector4.rga swizzling (equivalent to cVector4.xyw).
        /// </summary>
        public cVector3 rga => new cVector3(x, y, w);
        
        /// <summary>
        /// Returns cVector4.xywx swizzling.
        /// </summary>
        public cVector4 xywx => new cVector4(x, y, w, x);
        
        /// <summary>
        /// Returns cVector4.rgar swizzling (equivalent to cVector4.xywx).
        /// </summary>
        public cVector4 rgar => new cVector4(x, y, w, x);
        
        /// <summary>
        /// Returns cVector4.xywy swizzling.
        /// </summary>
        public cVector4 xywy => new cVector4(x, y, w, y);
        
        /// <summary>
        /// Returns cVector4.rgag swizzling (equivalent to cVector4.xywy).
        /// </summary>
        public cVector4 rgag => new cVector4(x, y, w, y);
        
        /// <summary>
        /// Returns cVector4.xywz swizzling.
        /// </summary>
        public cVector4 xywz => new cVector4(x, y, w, z);
        
        /// <summary>
        /// Returns cVector4.rgab swizzling (equivalent to cVector4.xywz).
        /// </summary>
        public cVector4 rgab => new cVector4(x, y, w, z);
        
        /// <summary>
        /// Returns cVector4.xyww swizzling.
        /// </summary>
        public cVector4 xyww => new cVector4(x, y, w, w);
        
        /// <summary>
        /// Returns cVector4.rgaa swizzling (equivalent to cVector4.xyww).
        /// </summary>
        public cVector4 rgaa => new cVector4(x, y, w, w);
        
        /// <summary>
        /// Returns cVector4.xz swizzling.
        /// </summary>
        public cVector2 xz => new cVector2(x, z);
        
        /// <summary>
        /// Returns cVector4.rb swizzling (equivalent to cVector4.xz).
        /// </summary>
        public cVector2 rb => new cVector2(x, z);
        
        /// <summary>
        /// Returns cVector4.xzx swizzling.
        /// </summary>
        public cVector3 xzx => new cVector3(x, z, x);
        
        /// <summary>
        /// Returns cVector4.rbr swizzling (equivalent to cVector4.xzx).
        /// </summary>
        public cVector3 rbr => new cVector3(x, z, x);
        
        /// <summary>
        /// Returns cVector4.xzxx swizzling.
        /// </summary>
        public cVector4 xzxx => new cVector4(x, z, x, x);
        
        /// <summary>
        /// Returns cVector4.rbrr swizzling (equivalent to cVector4.xzxx).
        /// </summary>
        public cVector4 rbrr => new cVector4(x, z, x, x);
        
        /// <summary>
        /// Returns cVector4.xzxy swizzling.
        /// </summary>
        public cVector4 xzxy => new cVector4(x, z, x, y);
        
        /// <summary>
        /// Returns cVector4.rbrg swizzling (equivalent to cVector4.xzxy).
        /// </summary>
        public cVector4 rbrg => new cVector4(x, z, x, y);
        
        /// <summary>
        /// Returns cVector4.xzxz swizzling.
        /// </summary>
        public cVector4 xzxz => new cVector4(x, z, x, z);
        
        /// <summary>
        /// Returns cVector4.rbrb swizzling (equivalent to cVector4.xzxz).
        /// </summary>
        public cVector4 rbrb => new cVector4(x, z, x, z);
        
        /// <summary>
        /// Returns cVector4.xzxw swizzling.
        /// </summary>
        public cVector4 xzxw => new cVector4(x, z, x, w);
        
        /// <summary>
        /// Returns cVector4.rbra swizzling (equivalent to cVector4.xzxw).
        /// </summary>
        public cVector4 rbra => new cVector4(x, z, x, w);
        
        /// <summary>
        /// Returns cVector4.xzy swizzling.
        /// </summary>
        public cVector3 xzy => new cVector3(x, z, y);
        
        /// <summary>
        /// Returns cVector4.rbg swizzling (equivalent to cVector4.xzy).
        /// </summary>
        public cVector3 rbg => new cVector3(x, z, y);
        
        /// <summary>
        /// Returns cVector4.xzyx swizzling.
        /// </summary>
        public cVector4 xzyx => new cVector4(x, z, y, x);
        
        /// <summary>
        /// Returns cVector4.rbgr swizzling (equivalent to cVector4.xzyx).
        /// </summary>
        public cVector4 rbgr => new cVector4(x, z, y, x);
        
        /// <summary>
        /// Returns cVector4.xzyy swizzling.
        /// </summary>
        public cVector4 xzyy => new cVector4(x, z, y, y);
        
        /// <summary>
        /// Returns cVector4.rbgg swizzling (equivalent to cVector4.xzyy).
        /// </summary>
        public cVector4 rbgg => new cVector4(x, z, y, y);
        
        /// <summary>
        /// Returns cVector4.xzyz swizzling.
        /// </summary>
        public cVector4 xzyz => new cVector4(x, z, y, z);
        
        /// <summary>
        /// Returns cVector4.rbgb swizzling (equivalent to cVector4.xzyz).
        /// </summary>
        public cVector4 rbgb => new cVector4(x, z, y, z);
        
        /// <summary>
        /// Returns cVector4.xzyw swizzling.
        /// </summary>
        public cVector4 xzyw => new cVector4(x, z, y, w);
        
        /// <summary>
        /// Returns cVector4.rbga swizzling (equivalent to cVector4.xzyw).
        /// </summary>
        public cVector4 rbga => new cVector4(x, z, y, w);
        
        /// <summary>
        /// Returns cVector4.xzz swizzling.
        /// </summary>
        public cVector3 xzz => new cVector3(x, z, z);
        
        /// <summary>
        /// Returns cVector4.rbb swizzling (equivalent to cVector4.xzz).
        /// </summary>
        public cVector3 rbb => new cVector3(x, z, z);
        
        /// <summary>
        /// Returns cVector4.xzzx swizzling.
        /// </summary>
        public cVector4 xzzx => new cVector4(x, z, z, x);
        
        /// <summary>
        /// Returns cVector4.rbbr swizzling (equivalent to cVector4.xzzx).
        /// </summary>
        public cVector4 rbbr => new cVector4(x, z, z, x);
        
        /// <summary>
        /// Returns cVector4.xzzy swizzling.
        /// </summary>
        public cVector4 xzzy => new cVector4(x, z, z, y);
        
        /// <summary>
        /// Returns cVector4.rbbg swizzling (equivalent to cVector4.xzzy).
        /// </summary>
        public cVector4 rbbg => new cVector4(x, z, z, y);
        
        /// <summary>
        /// Returns cVector4.xzzz swizzling.
        /// </summary>
        public cVector4 xzzz => new cVector4(x, z, z, z);
        
        /// <summary>
        /// Returns cVector4.rbbb swizzling (equivalent to cVector4.xzzz).
        /// </summary>
        public cVector4 rbbb => new cVector4(x, z, z, z);
        
        /// <summary>
        /// Returns cVector4.xzzw swizzling.
        /// </summary>
        public cVector4 xzzw => new cVector4(x, z, z, w);
        
        /// <summary>
        /// Returns cVector4.rbba swizzling (equivalent to cVector4.xzzw).
        /// </summary>
        public cVector4 rbba => new cVector4(x, z, z, w);
        
        /// <summary>
        /// Returns cVector4.xzw swizzling.
        /// </summary>
        public cVector3 xzw => new cVector3(x, z, w);
        
        /// <summary>
        /// Returns cVector4.rba swizzling (equivalent to cVector4.xzw).
        /// </summary>
        public cVector3 rba => new cVector3(x, z, w);
        
        /// <summary>
        /// Returns cVector4.xzwx swizzling.
        /// </summary>
        public cVector4 xzwx => new cVector4(x, z, w, x);
        
        /// <summary>
        /// Returns cVector4.rbar swizzling (equivalent to cVector4.xzwx).
        /// </summary>
        public cVector4 rbar => new cVector4(x, z, w, x);
        
        /// <summary>
        /// Returns cVector4.xzwy swizzling.
        /// </summary>
        public cVector4 xzwy => new cVector4(x, z, w, y);
        
        /// <summary>
        /// Returns cVector4.rbag swizzling (equivalent to cVector4.xzwy).
        /// </summary>
        public cVector4 rbag => new cVector4(x, z, w, y);
        
        /// <summary>
        /// Returns cVector4.xzwz swizzling.
        /// </summary>
        public cVector4 xzwz => new cVector4(x, z, w, z);
        
        /// <summary>
        /// Returns cVector4.rbab swizzling (equivalent to cVector4.xzwz).
        /// </summary>
        public cVector4 rbab => new cVector4(x, z, w, z);
        
        /// <summary>
        /// Returns cVector4.xzww swizzling.
        /// </summary>
        public cVector4 xzww => new cVector4(x, z, w, w);
        
        /// <summary>
        /// Returns cVector4.rbaa swizzling (equivalent to cVector4.xzww).
        /// </summary>
        public cVector4 rbaa => new cVector4(x, z, w, w);
        
        /// <summary>
        /// Returns cVector4.xw swizzling.
        /// </summary>
        public cVector2 xw => new cVector2(x, w);
        
        /// <summary>
        /// Returns cVector4.ra swizzling (equivalent to cVector4.xw).
        /// </summary>
        public cVector2 ra => new cVector2(x, w);
        
        /// <summary>
        /// Returns cVector4.xwx swizzling.
        /// </summary>
        public cVector3 xwx => new cVector3(x, w, x);
        
        /// <summary>
        /// Returns cVector4.rar swizzling (equivalent to cVector4.xwx).
        /// </summary>
        public cVector3 rar => new cVector3(x, w, x);
        
        /// <summary>
        /// Returns cVector4.xwxx swizzling.
        /// </summary>
        public cVector4 xwxx => new cVector4(x, w, x, x);
        
        /// <summary>
        /// Returns cVector4.rarr swizzling (equivalent to cVector4.xwxx).
        /// </summary>
        public cVector4 rarr => new cVector4(x, w, x, x);
        
        /// <summary>
        /// Returns cVector4.xwxy swizzling.
        /// </summary>
        public cVector4 xwxy => new cVector4(x, w, x, y);
        
        /// <summary>
        /// Returns cVector4.rarg swizzling (equivalent to cVector4.xwxy).
        /// </summary>
        public cVector4 rarg => new cVector4(x, w, x, y);
        
        /// <summary>
        /// Returns cVector4.xwxz swizzling.
        /// </summary>
        public cVector4 xwxz => new cVector4(x, w, x, z);
        
        /// <summary>
        /// Returns cVector4.rarb swizzling (equivalent to cVector4.xwxz).
        /// </summary>
        public cVector4 rarb => new cVector4(x, w, x, z);
        
        /// <summary>
        /// Returns cVector4.xwxw swizzling.
        /// </summary>
        public cVector4 xwxw => new cVector4(x, w, x, w);
        
        /// <summary>
        /// Returns cVector4.rara swizzling (equivalent to cVector4.xwxw).
        /// </summary>
        public cVector4 rara => new cVector4(x, w, x, w);
        
        /// <summary>
        /// Returns cVector4.xwy swizzling.
        /// </summary>
        public cVector3 xwy => new cVector3(x, w, y);
        
        /// <summary>
        /// Returns cVector4.rag swizzling (equivalent to cVector4.xwy).
        /// </summary>
        public cVector3 rag => new cVector3(x, w, y);
        
        /// <summary>
        /// Returns cVector4.xwyx swizzling.
        /// </summary>
        public cVector4 xwyx => new cVector4(x, w, y, x);
        
        /// <summary>
        /// Returns cVector4.ragr swizzling (equivalent to cVector4.xwyx).
        /// </summary>
        public cVector4 ragr => new cVector4(x, w, y, x);
        
        /// <summary>
        /// Returns cVector4.xwyy swizzling.
        /// </summary>
        public cVector4 xwyy => new cVector4(x, w, y, y);
        
        /// <summary>
        /// Returns cVector4.ragg swizzling (equivalent to cVector4.xwyy).
        /// </summary>
        public cVector4 ragg => new cVector4(x, w, y, y);
        
        /// <summary>
        /// Returns cVector4.xwyz swizzling.
        /// </summary>
        public cVector4 xwyz => new cVector4(x, w, y, z);
        
        /// <summary>
        /// Returns cVector4.ragb swizzling (equivalent to cVector4.xwyz).
        /// </summary>
        public cVector4 ragb => new cVector4(x, w, y, z);
        
        /// <summary>
        /// Returns cVector4.xwyw swizzling.
        /// </summary>
        public cVector4 xwyw => new cVector4(x, w, y, w);
        
        /// <summary>
        /// Returns cVector4.raga swizzling (equivalent to cVector4.xwyw).
        /// </summary>
        public cVector4 raga => new cVector4(x, w, y, w);
        
        /// <summary>
        /// Returns cVector4.xwz swizzling.
        /// </summary>
        public cVector3 xwz => new cVector3(x, w, z);
        
        /// <summary>
        /// Returns cVector4.rab swizzling (equivalent to cVector4.xwz).
        /// </summary>
        public cVector3 rab => new cVector3(x, w, z);
        
        /// <summary>
        /// Returns cVector4.xwzx swizzling.
        /// </summary>
        public cVector4 xwzx => new cVector4(x, w, z, x);
        
        /// <summary>
        /// Returns cVector4.rabr swizzling (equivalent to cVector4.xwzx).
        /// </summary>
        public cVector4 rabr => new cVector4(x, w, z, x);
        
        /// <summary>
        /// Returns cVector4.xwzy swizzling.
        /// </summary>
        public cVector4 xwzy => new cVector4(x, w, z, y);
        
        /// <summary>
        /// Returns cVector4.rabg swizzling (equivalent to cVector4.xwzy).
        /// </summary>
        public cVector4 rabg => new cVector4(x, w, z, y);
        
        /// <summary>
        /// Returns cVector4.xwzz swizzling.
        /// </summary>
        public cVector4 xwzz => new cVector4(x, w, z, z);
        
        /// <summary>
        /// Returns cVector4.rabb swizzling (equivalent to cVector4.xwzz).
        /// </summary>
        public cVector4 rabb => new cVector4(x, w, z, z);
        
        /// <summary>
        /// Returns cVector4.xwzw swizzling.
        /// </summary>
        public cVector4 xwzw => new cVector4(x, w, z, w);
        
        /// <summary>
        /// Returns cVector4.raba swizzling (equivalent to cVector4.xwzw).
        /// </summary>
        public cVector4 raba => new cVector4(x, w, z, w);
        
        /// <summary>
        /// Returns cVector4.xww swizzling.
        /// </summary>
        public cVector3 xww => new cVector3(x, w, w);
        
        /// <summary>
        /// Returns cVector4.raa swizzling (equivalent to cVector4.xww).
        /// </summary>
        public cVector3 raa => new cVector3(x, w, w);
        
        /// <summary>
        /// Returns cVector4.xwwx swizzling.
        /// </summary>
        public cVector4 xwwx => new cVector4(x, w, w, x);
        
        /// <summary>
        /// Returns cVector4.raar swizzling (equivalent to cVector4.xwwx).
        /// </summary>
        public cVector4 raar => new cVector4(x, w, w, x);
        
        /// <summary>
        /// Returns cVector4.xwwy swizzling.
        /// </summary>
        public cVector4 xwwy => new cVector4(x, w, w, y);
        
        /// <summary>
        /// Returns cVector4.raag swizzling (equivalent to cVector4.xwwy).
        /// </summary>
        public cVector4 raag => new cVector4(x, w, w, y);
        
        /// <summary>
        /// Returns cVector4.xwwz swizzling.
        /// </summary>
        public cVector4 xwwz => new cVector4(x, w, w, z);
        
        /// <summary>
        /// Returns cVector4.raab swizzling (equivalent to cVector4.xwwz).
        /// </summary>
        public cVector4 raab => new cVector4(x, w, w, z);
        
        /// <summary>
        /// Returns cVector4.xwww swizzling.
        /// </summary>
        public cVector4 xwww => new cVector4(x, w, w, w);
        
        /// <summary>
        /// Returns cVector4.raaa swizzling (equivalent to cVector4.xwww).
        /// </summary>
        public cVector4 raaa => new cVector4(x, w, w, w);
        
        /// <summary>
        /// Returns cVector4.yx swizzling.
        /// </summary>
        public cVector2 yx => new cVector2(y, x);
        
        /// <summary>
        /// Returns cVector4.gr swizzling (equivalent to cVector4.yx).
        /// </summary>
        public cVector2 gr => new cVector2(y, x);
        
        /// <summary>
        /// Returns cVector4.yxx swizzling.
        /// </summary>
        public cVector3 yxx => new cVector3(y, x, x);
        
        /// <summary>
        /// Returns cVector4.grr swizzling (equivalent to cVector4.yxx).
        /// </summary>
        public cVector3 grr => new cVector3(y, x, x);
        
        /// <summary>
        /// Returns cVector4.yxxx swizzling.
        /// </summary>
        public cVector4 yxxx => new cVector4(y, x, x, x);
        
        /// <summary>
        /// Returns cVector4.grrr swizzling (equivalent to cVector4.yxxx).
        /// </summary>
        public cVector4 grrr => new cVector4(y, x, x, x);
        
        /// <summary>
        /// Returns cVector4.yxxy swizzling.
        /// </summary>
        public cVector4 yxxy => new cVector4(y, x, x, y);
        
        /// <summary>
        /// Returns cVector4.grrg swizzling (equivalent to cVector4.yxxy).
        /// </summary>
        public cVector4 grrg => new cVector4(y, x, x, y);
        
        /// <summary>
        /// Returns cVector4.yxxz swizzling.
        /// </summary>
        public cVector4 yxxz => new cVector4(y, x, x, z);
        
        /// <summary>
        /// Returns cVector4.grrb swizzling (equivalent to cVector4.yxxz).
        /// </summary>
        public cVector4 grrb => new cVector4(y, x, x, z);
        
        /// <summary>
        /// Returns cVector4.yxxw swizzling.
        /// </summary>
        public cVector4 yxxw => new cVector4(y, x, x, w);
        
        /// <summary>
        /// Returns cVector4.grra swizzling (equivalent to cVector4.yxxw).
        /// </summary>
        public cVector4 grra => new cVector4(y, x, x, w);
        
        /// <summary>
        /// Returns cVector4.yxy swizzling.
        /// </summary>
        public cVector3 yxy => new cVector3(y, x, y);
        
        /// <summary>
        /// Returns cVector4.grg swizzling (equivalent to cVector4.yxy).
        /// </summary>
        public cVector3 grg => new cVector3(y, x, y);
        
        /// <summary>
        /// Returns cVector4.yxyx swizzling.
        /// </summary>
        public cVector4 yxyx => new cVector4(y, x, y, x);
        
        /// <summary>
        /// Returns cVector4.grgr swizzling (equivalent to cVector4.yxyx).
        /// </summary>
        public cVector4 grgr => new cVector4(y, x, y, x);
        
        /// <summary>
        /// Returns cVector4.yxyy swizzling.
        /// </summary>
        public cVector4 yxyy => new cVector4(y, x, y, y);
        
        /// <summary>
        /// Returns cVector4.grgg swizzling (equivalent to cVector4.yxyy).
        /// </summary>
        public cVector4 grgg => new cVector4(y, x, y, y);
        
        /// <summary>
        /// Returns cVector4.yxyz swizzling.
        /// </summary>
        public cVector4 yxyz => new cVector4(y, x, y, z);
        
        /// <summary>
        /// Returns cVector4.grgb swizzling (equivalent to cVector4.yxyz).
        /// </summary>
        public cVector4 grgb => new cVector4(y, x, y, z);
        
        /// <summary>
        /// Returns cVector4.yxyw swizzling.
        /// </summary>
        public cVector4 yxyw => new cVector4(y, x, y, w);
        
        /// <summary>
        /// Returns cVector4.grga swizzling (equivalent to cVector4.yxyw).
        /// </summary>
        public cVector4 grga => new cVector4(y, x, y, w);
        
        /// <summary>
        /// Returns cVector4.yxz swizzling.
        /// </summary>
        public cVector3 yxz => new cVector3(y, x, z);
        
        /// <summary>
        /// Returns cVector4.grb swizzling (equivalent to cVector4.yxz).
        /// </summary>
        public cVector3 grb => new cVector3(y, x, z);
        
        /// <summary>
        /// Returns cVector4.yxzx swizzling.
        /// </summary>
        public cVector4 yxzx => new cVector4(y, x, z, x);
        
        /// <summary>
        /// Returns cVector4.grbr swizzling (equivalent to cVector4.yxzx).
        /// </summary>
        public cVector4 grbr => new cVector4(y, x, z, x);
        
        /// <summary>
        /// Returns cVector4.yxzy swizzling.
        /// </summary>
        public cVector4 yxzy => new cVector4(y, x, z, y);
        
        /// <summary>
        /// Returns cVector4.grbg swizzling (equivalent to cVector4.yxzy).
        /// </summary>
        public cVector4 grbg => new cVector4(y, x, z, y);
        
        /// <summary>
        /// Returns cVector4.yxzz swizzling.
        /// </summary>
        public cVector4 yxzz => new cVector4(y, x, z, z);
        
        /// <summary>
        /// Returns cVector4.grbb swizzling (equivalent to cVector4.yxzz).
        /// </summary>
        public cVector4 grbb => new cVector4(y, x, z, z);
        
        /// <summary>
        /// Returns cVector4.yxzw swizzling.
        /// </summary>
        public cVector4 yxzw => new cVector4(y, x, z, w);
        
        /// <summary>
        /// Returns cVector4.grba swizzling (equivalent to cVector4.yxzw).
        /// </summary>
        public cVector4 grba => new cVector4(y, x, z, w);
        
        /// <summary>
        /// Returns cVector4.yxw swizzling.
        /// </summary>
        public cVector3 yxw => new cVector3(y, x, w);
        
        /// <summary>
        /// Returns cVector4.gra swizzling (equivalent to cVector4.yxw).
        /// </summary>
        public cVector3 gra => new cVector3(y, x, w);
        
        /// <summary>
        /// Returns cVector4.yxwx swizzling.
        /// </summary>
        public cVector4 yxwx => new cVector4(y, x, w, x);
        
        /// <summary>
        /// Returns cVector4.grar swizzling (equivalent to cVector4.yxwx).
        /// </summary>
        public cVector4 grar => new cVector4(y, x, w, x);
        
        /// <summary>
        /// Returns cVector4.yxwy swizzling.
        /// </summary>
        public cVector4 yxwy => new cVector4(y, x, w, y);
        
        /// <summary>
        /// Returns cVector4.grag swizzling (equivalent to cVector4.yxwy).
        /// </summary>
        public cVector4 grag => new cVector4(y, x, w, y);
        
        /// <summary>
        /// Returns cVector4.yxwz swizzling.
        /// </summary>
        public cVector4 yxwz => new cVector4(y, x, w, z);
        
        /// <summary>
        /// Returns cVector4.grab swizzling (equivalent to cVector4.yxwz).
        /// </summary>
        public cVector4 grab => new cVector4(y, x, w, z);
        
        /// <summary>
        /// Returns cVector4.yxww swizzling.
        /// </summary>
        public cVector4 yxww => new cVector4(y, x, w, w);
        
        /// <summary>
        /// Returns cVector4.graa swizzling (equivalent to cVector4.yxww).
        /// </summary>
        public cVector4 graa => new cVector4(y, x, w, w);
        
        /// <summary>
        /// Returns cVector4.yy swizzling.
        /// </summary>
        public cVector2 yy => new cVector2(y, y);
        
        /// <summary>
        /// Returns cVector4.gg swizzling (equivalent to cVector4.yy).
        /// </summary>
        public cVector2 gg => new cVector2(y, y);
        
        /// <summary>
        /// Returns cVector4.yyx swizzling.
        /// </summary>
        public cVector3 yyx => new cVector3(y, y, x);
        
        /// <summary>
        /// Returns cVector4.ggr swizzling (equivalent to cVector4.yyx).
        /// </summary>
        public cVector3 ggr => new cVector3(y, y, x);
        
        /// <summary>
        /// Returns cVector4.yyxx swizzling.
        /// </summary>
        public cVector4 yyxx => new cVector4(y, y, x, x);
        
        /// <summary>
        /// Returns cVector4.ggrr swizzling (equivalent to cVector4.yyxx).
        /// </summary>
        public cVector4 ggrr => new cVector4(y, y, x, x);
        
        /// <summary>
        /// Returns cVector4.yyxy swizzling.
        /// </summary>
        public cVector4 yyxy => new cVector4(y, y, x, y);
        
        /// <summary>
        /// Returns cVector4.ggrg swizzling (equivalent to cVector4.yyxy).
        /// </summary>
        public cVector4 ggrg => new cVector4(y, y, x, y);
        
        /// <summary>
        /// Returns cVector4.yyxz swizzling.
        /// </summary>
        public cVector4 yyxz => new cVector4(y, y, x, z);
        
        /// <summary>
        /// Returns cVector4.ggrb swizzling (equivalent to cVector4.yyxz).
        /// </summary>
        public cVector4 ggrb => new cVector4(y, y, x, z);
        
        /// <summary>
        /// Returns cVector4.yyxw swizzling.
        /// </summary>
        public cVector4 yyxw => new cVector4(y, y, x, w);
        
        /// <summary>
        /// Returns cVector4.ggra swizzling (equivalent to cVector4.yyxw).
        /// </summary>
        public cVector4 ggra => new cVector4(y, y, x, w);
        
        /// <summary>
        /// Returns cVector4.yyy swizzling.
        /// </summary>
        public cVector3 yyy => new cVector3(y, y, y);
        
        /// <summary>
        /// Returns cVector4.ggg swizzling (equivalent to cVector4.yyy).
        /// </summary>
        public cVector3 ggg => new cVector3(y, y, y);
        
        /// <summary>
        /// Returns cVector4.yyyx swizzling.
        /// </summary>
        public cVector4 yyyx => new cVector4(y, y, y, x);
        
        /// <summary>
        /// Returns cVector4.gggr swizzling (equivalent to cVector4.yyyx).
        /// </summary>
        public cVector4 gggr => new cVector4(y, y, y, x);
        
        /// <summary>
        /// Returns cVector4.yyyy swizzling.
        /// </summary>
        public cVector4 yyyy => new cVector4(y, y, y, y);
        
        /// <summary>
        /// Returns cVector4.gggg swizzling (equivalent to cVector4.yyyy).
        /// </summary>
        public cVector4 gggg => new cVector4(y, y, y, y);
        
        /// <summary>
        /// Returns cVector4.yyyz swizzling.
        /// </summary>
        public cVector4 yyyz => new cVector4(y, y, y, z);
        
        /// <summary>
        /// Returns cVector4.gggb swizzling (equivalent to cVector4.yyyz).
        /// </summary>
        public cVector4 gggb => new cVector4(y, y, y, z);
        
        /// <summary>
        /// Returns cVector4.yyyw swizzling.
        /// </summary>
        public cVector4 yyyw => new cVector4(y, y, y, w);
        
        /// <summary>
        /// Returns cVector4.ggga swizzling (equivalent to cVector4.yyyw).
        /// </summary>
        public cVector4 ggga => new cVector4(y, y, y, w);
        
        /// <summary>
        /// Returns cVector4.yyz swizzling.
        /// </summary>
        public cVector3 yyz => new cVector3(y, y, z);
        
        /// <summary>
        /// Returns cVector4.ggb swizzling (equivalent to cVector4.yyz).
        /// </summary>
        public cVector3 ggb => new cVector3(y, y, z);
        
        /// <summary>
        /// Returns cVector4.yyzx swizzling.
        /// </summary>
        public cVector4 yyzx => new cVector4(y, y, z, x);
        
        /// <summary>
        /// Returns cVector4.ggbr swizzling (equivalent to cVector4.yyzx).
        /// </summary>
        public cVector4 ggbr => new cVector4(y, y, z, x);
        
        /// <summary>
        /// Returns cVector4.yyzy swizzling.
        /// </summary>
        public cVector4 yyzy => new cVector4(y, y, z, y);
        
        /// <summary>
        /// Returns cVector4.ggbg swizzling (equivalent to cVector4.yyzy).
        /// </summary>
        public cVector4 ggbg => new cVector4(y, y, z, y);
        
        /// <summary>
        /// Returns cVector4.yyzz swizzling.
        /// </summary>
        public cVector4 yyzz => new cVector4(y, y, z, z);
        
        /// <summary>
        /// Returns cVector4.ggbb swizzling (equivalent to cVector4.yyzz).
        /// </summary>
        public cVector4 ggbb => new cVector4(y, y, z, z);
        
        /// <summary>
        /// Returns cVector4.yyzw swizzling.
        /// </summary>
        public cVector4 yyzw => new cVector4(y, y, z, w);
        
        /// <summary>
        /// Returns cVector4.ggba swizzling (equivalent to cVector4.yyzw).
        /// </summary>
        public cVector4 ggba => new cVector4(y, y, z, w);
        
        /// <summary>
        /// Returns cVector4.yyw swizzling.
        /// </summary>
        public cVector3 yyw => new cVector3(y, y, w);
        
        /// <summary>
        /// Returns cVector4.gga swizzling (equivalent to cVector4.yyw).
        /// </summary>
        public cVector3 gga => new cVector3(y, y, w);
        
        /// <summary>
        /// Returns cVector4.yywx swizzling.
        /// </summary>
        public cVector4 yywx => new cVector4(y, y, w, x);
        
        /// <summary>
        /// Returns cVector4.ggar swizzling (equivalent to cVector4.yywx).
        /// </summary>
        public cVector4 ggar => new cVector4(y, y, w, x);
        
        /// <summary>
        /// Returns cVector4.yywy swizzling.
        /// </summary>
        public cVector4 yywy => new cVector4(y, y, w, y);
        
        /// <summary>
        /// Returns cVector4.ggag swizzling (equivalent to cVector4.yywy).
        /// </summary>
        public cVector4 ggag => new cVector4(y, y, w, y);
        
        /// <summary>
        /// Returns cVector4.yywz swizzling.
        /// </summary>
        public cVector4 yywz => new cVector4(y, y, w, z);
        
        /// <summary>
        /// Returns cVector4.ggab swizzling (equivalent to cVector4.yywz).
        /// </summary>
        public cVector4 ggab => new cVector4(y, y, w, z);
        
        /// <summary>
        /// Returns cVector4.yyww swizzling.
        /// </summary>
        public cVector4 yyww => new cVector4(y, y, w, w);
        
        /// <summary>
        /// Returns cVector4.ggaa swizzling (equivalent to cVector4.yyww).
        /// </summary>
        public cVector4 ggaa => new cVector4(y, y, w, w);
        
        /// <summary>
        /// Returns cVector4.yz swizzling.
        /// </summary>
        public cVector2 yz => new cVector2(y, z);
        
        /// <summary>
        /// Returns cVector4.gb swizzling (equivalent to cVector4.yz).
        /// </summary>
        public cVector2 gb => new cVector2(y, z);
        
        /// <summary>
        /// Returns cVector4.yzx swizzling.
        /// </summary>
        public cVector3 yzx => new cVector3(y, z, x);
        
        /// <summary>
        /// Returns cVector4.gbr swizzling (equivalent to cVector4.yzx).
        /// </summary>
        public cVector3 gbr => new cVector3(y, z, x);
        
        /// <summary>
        /// Returns cVector4.yzxx swizzling.
        /// </summary>
        public cVector4 yzxx => new cVector4(y, z, x, x);
        
        /// <summary>
        /// Returns cVector4.gbrr swizzling (equivalent to cVector4.yzxx).
        /// </summary>
        public cVector4 gbrr => new cVector4(y, z, x, x);
        
        /// <summary>
        /// Returns cVector4.yzxy swizzling.
        /// </summary>
        public cVector4 yzxy => new cVector4(y, z, x, y);
        
        /// <summary>
        /// Returns cVector4.gbrg swizzling (equivalent to cVector4.yzxy).
        /// </summary>
        public cVector4 gbrg => new cVector4(y, z, x, y);
        
        /// <summary>
        /// Returns cVector4.yzxz swizzling.
        /// </summary>
        public cVector4 yzxz => new cVector4(y, z, x, z);
        
        /// <summary>
        /// Returns cVector4.gbrb swizzling (equivalent to cVector4.yzxz).
        /// </summary>
        public cVector4 gbrb => new cVector4(y, z, x, z);
        
        /// <summary>
        /// Returns cVector4.yzxw swizzling.
        /// </summary>
        public cVector4 yzxw => new cVector4(y, z, x, w);
        
        /// <summary>
        /// Returns cVector4.gbra swizzling (equivalent to cVector4.yzxw).
        /// </summary>
        public cVector4 gbra => new cVector4(y, z, x, w);
        
        /// <summary>
        /// Returns cVector4.yzy swizzling.
        /// </summary>
        public cVector3 yzy => new cVector3(y, z, y);
        
        /// <summary>
        /// Returns cVector4.gbg swizzling (equivalent to cVector4.yzy).
        /// </summary>
        public cVector3 gbg => new cVector3(y, z, y);
        
        /// <summary>
        /// Returns cVector4.yzyx swizzling.
        /// </summary>
        public cVector4 yzyx => new cVector4(y, z, y, x);
        
        /// <summary>
        /// Returns cVector4.gbgr swizzling (equivalent to cVector4.yzyx).
        /// </summary>
        public cVector4 gbgr => new cVector4(y, z, y, x);
        
        /// <summary>
        /// Returns cVector4.yzyy swizzling.
        /// </summary>
        public cVector4 yzyy => new cVector4(y, z, y, y);
        
        /// <summary>
        /// Returns cVector4.gbgg swizzling (equivalent to cVector4.yzyy).
        /// </summary>
        public cVector4 gbgg => new cVector4(y, z, y, y);
        
        /// <summary>
        /// Returns cVector4.yzyz swizzling.
        /// </summary>
        public cVector4 yzyz => new cVector4(y, z, y, z);
        
        /// <summary>
        /// Returns cVector4.gbgb swizzling (equivalent to cVector4.yzyz).
        /// </summary>
        public cVector4 gbgb => new cVector4(y, z, y, z);
        
        /// <summary>
        /// Returns cVector4.yzyw swizzling.
        /// </summary>
        public cVector4 yzyw => new cVector4(y, z, y, w);
        
        /// <summary>
        /// Returns cVector4.gbga swizzling (equivalent to cVector4.yzyw).
        /// </summary>
        public cVector4 gbga => new cVector4(y, z, y, w);
        
        /// <summary>
        /// Returns cVector4.yzz swizzling.
        /// </summary>
        public cVector3 yzz => new cVector3(y, z, z);
        
        /// <summary>
        /// Returns cVector4.gbb swizzling (equivalent to cVector4.yzz).
        /// </summary>
        public cVector3 gbb => new cVector3(y, z, z);
        
        /// <summary>
        /// Returns cVector4.yzzx swizzling.
        /// </summary>
        public cVector4 yzzx => new cVector4(y, z, z, x);
        
        /// <summary>
        /// Returns cVector4.gbbr swizzling (equivalent to cVector4.yzzx).
        /// </summary>
        public cVector4 gbbr => new cVector4(y, z, z, x);
        
        /// <summary>
        /// Returns cVector4.yzzy swizzling.
        /// </summary>
        public cVector4 yzzy => new cVector4(y, z, z, y);
        
        /// <summary>
        /// Returns cVector4.gbbg swizzling (equivalent to cVector4.yzzy).
        /// </summary>
        public cVector4 gbbg => new cVector4(y, z, z, y);
        
        /// <summary>
        /// Returns cVector4.yzzz swizzling.
        /// </summary>
        public cVector4 yzzz => new cVector4(y, z, z, z);
        
        /// <summary>
        /// Returns cVector4.gbbb swizzling (equivalent to cVector4.yzzz).
        /// </summary>
        public cVector4 gbbb => new cVector4(y, z, z, z);
        
        /// <summary>
        /// Returns cVector4.yzzw swizzling.
        /// </summary>
        public cVector4 yzzw => new cVector4(y, z, z, w);
        
        /// <summary>
        /// Returns cVector4.gbba swizzling (equivalent to cVector4.yzzw).
        /// </summary>
        public cVector4 gbba => new cVector4(y, z, z, w);
        
        /// <summary>
        /// Returns cVector4.yzw swizzling.
        /// </summary>
        public cVector3 yzw => new cVector3(y, z, w);
        
        /// <summary>
        /// Returns cVector4.gba swizzling (equivalent to cVector4.yzw).
        /// </summary>
        public cVector3 gba => new cVector3(y, z, w);
        
        /// <summary>
        /// Returns cVector4.yzwx swizzling.
        /// </summary>
        public cVector4 yzwx => new cVector4(y, z, w, x);
        
        /// <summary>
        /// Returns cVector4.gbar swizzling (equivalent to cVector4.yzwx).
        /// </summary>
        public cVector4 gbar => new cVector4(y, z, w, x);
        
        /// <summary>
        /// Returns cVector4.yzwy swizzling.
        /// </summary>
        public cVector4 yzwy => new cVector4(y, z, w, y);
        
        /// <summary>
        /// Returns cVector4.gbag swizzling (equivalent to cVector4.yzwy).
        /// </summary>
        public cVector4 gbag => new cVector4(y, z, w, y);
        
        /// <summary>
        /// Returns cVector4.yzwz swizzling.
        /// </summary>
        public cVector4 yzwz => new cVector4(y, z, w, z);
        
        /// <summary>
        /// Returns cVector4.gbab swizzling (equivalent to cVector4.yzwz).
        /// </summary>
        public cVector4 gbab => new cVector4(y, z, w, z);
        
        /// <summary>
        /// Returns cVector4.yzww swizzling.
        /// </summary>
        public cVector4 yzww => new cVector4(y, z, w, w);
        
        /// <summary>
        /// Returns cVector4.gbaa swizzling (equivalent to cVector4.yzww).
        /// </summary>
        public cVector4 gbaa => new cVector4(y, z, w, w);
        
        /// <summary>
        /// Returns cVector4.yw swizzling.
        /// </summary>
        public cVector2 yw => new cVector2(y, w);
        
        /// <summary>
        /// Returns cVector4.ga swizzling (equivalent to cVector4.yw).
        /// </summary>
        public cVector2 ga => new cVector2(y, w);
        
        /// <summary>
        /// Returns cVector4.ywx swizzling.
        /// </summary>
        public cVector3 ywx => new cVector3(y, w, x);
        
        /// <summary>
        /// Returns cVector4.gar swizzling (equivalent to cVector4.ywx).
        /// </summary>
        public cVector3 gar => new cVector3(y, w, x);
        
        /// <summary>
        /// Returns cVector4.ywxx swizzling.
        /// </summary>
        public cVector4 ywxx => new cVector4(y, w, x, x);
        
        /// <summary>
        /// Returns cVector4.garr swizzling (equivalent to cVector4.ywxx).
        /// </summary>
        public cVector4 garr => new cVector4(y, w, x, x);
        
        /// <summary>
        /// Returns cVector4.ywxy swizzling.
        /// </summary>
        public cVector4 ywxy => new cVector4(y, w, x, y);
        
        /// <summary>
        /// Returns cVector4.garg swizzling (equivalent to cVector4.ywxy).
        /// </summary>
        public cVector4 garg => new cVector4(y, w, x, y);
        
        /// <summary>
        /// Returns cVector4.ywxz swizzling.
        /// </summary>
        public cVector4 ywxz => new cVector4(y, w, x, z);
        
        /// <summary>
        /// Returns cVector4.garb swizzling (equivalent to cVector4.ywxz).
        /// </summary>
        public cVector4 garb => new cVector4(y, w, x, z);
        
        /// <summary>
        /// Returns cVector4.ywxw swizzling.
        /// </summary>
        public cVector4 ywxw => new cVector4(y, w, x, w);
        
        /// <summary>
        /// Returns cVector4.gara swizzling (equivalent to cVector4.ywxw).
        /// </summary>
        public cVector4 gara => new cVector4(y, w, x, w);
        
        /// <summary>
        /// Returns cVector4.ywy swizzling.
        /// </summary>
        public cVector3 ywy => new cVector3(y, w, y);
        
        /// <summary>
        /// Returns cVector4.gag swizzling (equivalent to cVector4.ywy).
        /// </summary>
        public cVector3 gag => new cVector3(y, w, y);
        
        /// <summary>
        /// Returns cVector4.ywyx swizzling.
        /// </summary>
        public cVector4 ywyx => new cVector4(y, w, y, x);
        
        /// <summary>
        /// Returns cVector4.gagr swizzling (equivalent to cVector4.ywyx).
        /// </summary>
        public cVector4 gagr => new cVector4(y, w, y, x);
        
        /// <summary>
        /// Returns cVector4.ywyy swizzling.
        /// </summary>
        public cVector4 ywyy => new cVector4(y, w, y, y);
        
        /// <summary>
        /// Returns cVector4.gagg swizzling (equivalent to cVector4.ywyy).
        /// </summary>
        public cVector4 gagg => new cVector4(y, w, y, y);
        
        /// <summary>
        /// Returns cVector4.ywyz swizzling.
        /// </summary>
        public cVector4 ywyz => new cVector4(y, w, y, z);
        
        /// <summary>
        /// Returns cVector4.gagb swizzling (equivalent to cVector4.ywyz).
        /// </summary>
        public cVector4 gagb => new cVector4(y, w, y, z);
        
        /// <summary>
        /// Returns cVector4.ywyw swizzling.
        /// </summary>
        public cVector4 ywyw => new cVector4(y, w, y, w);
        
        /// <summary>
        /// Returns cVector4.gaga swizzling (equivalent to cVector4.ywyw).
        /// </summary>
        public cVector4 gaga => new cVector4(y, w, y, w);
        
        /// <summary>
        /// Returns cVector4.ywz swizzling.
        /// </summary>
        public cVector3 ywz => new cVector3(y, w, z);
        
        /// <summary>
        /// Returns cVector4.gab swizzling (equivalent to cVector4.ywz).
        /// </summary>
        public cVector3 gab => new cVector3(y, w, z);
        
        /// <summary>
        /// Returns cVector4.ywzx swizzling.
        /// </summary>
        public cVector4 ywzx => new cVector4(y, w, z, x);
        
        /// <summary>
        /// Returns cVector4.gabr swizzling (equivalent to cVector4.ywzx).
        /// </summary>
        public cVector4 gabr => new cVector4(y, w, z, x);
        
        /// <summary>
        /// Returns cVector4.ywzy swizzling.
        /// </summary>
        public cVector4 ywzy => new cVector4(y, w, z, y);
        
        /// <summary>
        /// Returns cVector4.gabg swizzling (equivalent to cVector4.ywzy).
        /// </summary>
        public cVector4 gabg => new cVector4(y, w, z, y);
        
        /// <summary>
        /// Returns cVector4.ywzz swizzling.
        /// </summary>
        public cVector4 ywzz => new cVector4(y, w, z, z);
        
        /// <summary>
        /// Returns cVector4.gabb swizzling (equivalent to cVector4.ywzz).
        /// </summary>
        public cVector4 gabb => new cVector4(y, w, z, z);
        
        /// <summary>
        /// Returns cVector4.ywzw swizzling.
        /// </summary>
        public cVector4 ywzw => new cVector4(y, w, z, w);
        
        /// <summary>
        /// Returns cVector4.gaba swizzling (equivalent to cVector4.ywzw).
        /// </summary>
        public cVector4 gaba => new cVector4(y, w, z, w);
        
        /// <summary>
        /// Returns cVector4.yww swizzling.
        /// </summary>
        public cVector3 yww => new cVector3(y, w, w);
        
        /// <summary>
        /// Returns cVector4.gaa swizzling (equivalent to cVector4.yww).
        /// </summary>
        public cVector3 gaa => new cVector3(y, w, w);
        
        /// <summary>
        /// Returns cVector4.ywwx swizzling.
        /// </summary>
        public cVector4 ywwx => new cVector4(y, w, w, x);
        
        /// <summary>
        /// Returns cVector4.gaar swizzling (equivalent to cVector4.ywwx).
        /// </summary>
        public cVector4 gaar => new cVector4(y, w, w, x);
        
        /// <summary>
        /// Returns cVector4.ywwy swizzling.
        /// </summary>
        public cVector4 ywwy => new cVector4(y, w, w, y);
        
        /// <summary>
        /// Returns cVector4.gaag swizzling (equivalent to cVector4.ywwy).
        /// </summary>
        public cVector4 gaag => new cVector4(y, w, w, y);
        
        /// <summary>
        /// Returns cVector4.ywwz swizzling.
        /// </summary>
        public cVector4 ywwz => new cVector4(y, w, w, z);
        
        /// <summary>
        /// Returns cVector4.gaab swizzling (equivalent to cVector4.ywwz).
        /// </summary>
        public cVector4 gaab => new cVector4(y, w, w, z);
        
        /// <summary>
        /// Returns cVector4.ywww swizzling.
        /// </summary>
        public cVector4 ywww => new cVector4(y, w, w, w);
        
        /// <summary>
        /// Returns cVector4.gaaa swizzling (equivalent to cVector4.ywww).
        /// </summary>
        public cVector4 gaaa => new cVector4(y, w, w, w);
        
        /// <summary>
        /// Returns cVector4.zx swizzling.
        /// </summary>
        public cVector2 zx => new cVector2(z, x);
        
        /// <summary>
        /// Returns cVector4.br swizzling (equivalent to cVector4.zx).
        /// </summary>
        public cVector2 br => new cVector2(z, x);
        
        /// <summary>
        /// Returns cVector4.zxx swizzling.
        /// </summary>
        public cVector3 zxx => new cVector3(z, x, x);
        
        /// <summary>
        /// Returns cVector4.brr swizzling (equivalent to cVector4.zxx).
        /// </summary>
        public cVector3 brr => new cVector3(z, x, x);
        
        /// <summary>
        /// Returns cVector4.zxxx swizzling.
        /// </summary>
        public cVector4 zxxx => new cVector4(z, x, x, x);
        
        /// <summary>
        /// Returns cVector4.brrr swizzling (equivalent to cVector4.zxxx).
        /// </summary>
        public cVector4 brrr => new cVector4(z, x, x, x);
        
        /// <summary>
        /// Returns cVector4.zxxy swizzling.
        /// </summary>
        public cVector4 zxxy => new cVector4(z, x, x, y);
        
        /// <summary>
        /// Returns cVector4.brrg swizzling (equivalent to cVector4.zxxy).
        /// </summary>
        public cVector4 brrg => new cVector4(z, x, x, y);
        
        /// <summary>
        /// Returns cVector4.zxxz swizzling.
        /// </summary>
        public cVector4 zxxz => new cVector4(z, x, x, z);
        
        /// <summary>
        /// Returns cVector4.brrb swizzling (equivalent to cVector4.zxxz).
        /// </summary>
        public cVector4 brrb => new cVector4(z, x, x, z);
        
        /// <summary>
        /// Returns cVector4.zxxw swizzling.
        /// </summary>
        public cVector4 zxxw => new cVector4(z, x, x, w);
        
        /// <summary>
        /// Returns cVector4.brra swizzling (equivalent to cVector4.zxxw).
        /// </summary>
        public cVector4 brra => new cVector4(z, x, x, w);
        
        /// <summary>
        /// Returns cVector4.zxy swizzling.
        /// </summary>
        public cVector3 zxy => new cVector3(z, x, y);
        
        /// <summary>
        /// Returns cVector4.brg swizzling (equivalent to cVector4.zxy).
        /// </summary>
        public cVector3 brg => new cVector3(z, x, y);
        
        /// <summary>
        /// Returns cVector4.zxyx swizzling.
        /// </summary>
        public cVector4 zxyx => new cVector4(z, x, y, x);
        
        /// <summary>
        /// Returns cVector4.brgr swizzling (equivalent to cVector4.zxyx).
        /// </summary>
        public cVector4 brgr => new cVector4(z, x, y, x);
        
        /// <summary>
        /// Returns cVector4.zxyy swizzling.
        /// </summary>
        public cVector4 zxyy => new cVector4(z, x, y, y);
        
        /// <summary>
        /// Returns cVector4.brgg swizzling (equivalent to cVector4.zxyy).
        /// </summary>
        public cVector4 brgg => new cVector4(z, x, y, y);
        
        /// <summary>
        /// Returns cVector4.zxyz swizzling.
        /// </summary>
        public cVector4 zxyz => new cVector4(z, x, y, z);
        
        /// <summary>
        /// Returns cVector4.brgb swizzling (equivalent to cVector4.zxyz).
        /// </summary>
        public cVector4 brgb => new cVector4(z, x, y, z);
        
        /// <summary>
        /// Returns cVector4.zxyw swizzling.
        /// </summary>
        public cVector4 zxyw => new cVector4(z, x, y, w);
        
        /// <summary>
        /// Returns cVector4.brga swizzling (equivalent to cVector4.zxyw).
        /// </summary>
        public cVector4 brga => new cVector4(z, x, y, w);
        
        /// <summary>
        /// Returns cVector4.zxz swizzling.
        /// </summary>
        public cVector3 zxz => new cVector3(z, x, z);
        
        /// <summary>
        /// Returns cVector4.brb swizzling (equivalent to cVector4.zxz).
        /// </summary>
        public cVector3 brb => new cVector3(z, x, z);
        
        /// <summary>
        /// Returns cVector4.zxzx swizzling.
        /// </summary>
        public cVector4 zxzx => new cVector4(z, x, z, x);
        
        /// <summary>
        /// Returns cVector4.brbr swizzling (equivalent to cVector4.zxzx).
        /// </summary>
        public cVector4 brbr => new cVector4(z, x, z, x);
        
        /// <summary>
        /// Returns cVector4.zxzy swizzling.
        /// </summary>
        public cVector4 zxzy => new cVector4(z, x, z, y);
        
        /// <summary>
        /// Returns cVector4.brbg swizzling (equivalent to cVector4.zxzy).
        /// </summary>
        public cVector4 brbg => new cVector4(z, x, z, y);
        
        /// <summary>
        /// Returns cVector4.zxzz swizzling.
        /// </summary>
        public cVector4 zxzz => new cVector4(z, x, z, z);
        
        /// <summary>
        /// Returns cVector4.brbb swizzling (equivalent to cVector4.zxzz).
        /// </summary>
        public cVector4 brbb => new cVector4(z, x, z, z);
        
        /// <summary>
        /// Returns cVector4.zxzw swizzling.
        /// </summary>
        public cVector4 zxzw => new cVector4(z, x, z, w);
        
        /// <summary>
        /// Returns cVector4.brba swizzling (equivalent to cVector4.zxzw).
        /// </summary>
        public cVector4 brba => new cVector4(z, x, z, w);
        
        /// <summary>
        /// Returns cVector4.zxw swizzling.
        /// </summary>
        public cVector3 zxw => new cVector3(z, x, w);
        
        /// <summary>
        /// Returns cVector4.bra swizzling (equivalent to cVector4.zxw).
        /// </summary>
        public cVector3 bra => new cVector3(z, x, w);
        
        /// <summary>
        /// Returns cVector4.zxwx swizzling.
        /// </summary>
        public cVector4 zxwx => new cVector4(z, x, w, x);
        
        /// <summary>
        /// Returns cVector4.brar swizzling (equivalent to cVector4.zxwx).
        /// </summary>
        public cVector4 brar => new cVector4(z, x, w, x);
        
        /// <summary>
        /// Returns cVector4.zxwy swizzling.
        /// </summary>
        public cVector4 zxwy => new cVector4(z, x, w, y);
        
        /// <summary>
        /// Returns cVector4.brag swizzling (equivalent to cVector4.zxwy).
        /// </summary>
        public cVector4 brag => new cVector4(z, x, w, y);
        
        /// <summary>
        /// Returns cVector4.zxwz swizzling.
        /// </summary>
        public cVector4 zxwz => new cVector4(z, x, w, z);
        
        /// <summary>
        /// Returns cVector4.brab swizzling (equivalent to cVector4.zxwz).
        /// </summary>
        public cVector4 brab => new cVector4(z, x, w, z);
        
        /// <summary>
        /// Returns cVector4.zxww swizzling.
        /// </summary>
        public cVector4 zxww => new cVector4(z, x, w, w);
        
        /// <summary>
        /// Returns cVector4.braa swizzling (equivalent to cVector4.zxww).
        /// </summary>
        public cVector4 braa => new cVector4(z, x, w, w);
        
        /// <summary>
        /// Returns cVector4.zy swizzling.
        /// </summary>
        public cVector2 zy => new cVector2(z, y);
        
        /// <summary>
        /// Returns cVector4.bg swizzling (equivalent to cVector4.zy).
        /// </summary>
        public cVector2 bg => new cVector2(z, y);
        
        /// <summary>
        /// Returns cVector4.zyx swizzling.
        /// </summary>
        public cVector3 zyx => new cVector3(z, y, x);
        
        /// <summary>
        /// Returns cVector4.bgr swizzling (equivalent to cVector4.zyx).
        /// </summary>
        public cVector3 bgr => new cVector3(z, y, x);
        
        /// <summary>
        /// Returns cVector4.zyxx swizzling.
        /// </summary>
        public cVector4 zyxx => new cVector4(z, y, x, x);
        
        /// <summary>
        /// Returns cVector4.bgrr swizzling (equivalent to cVector4.zyxx).
        /// </summary>
        public cVector4 bgrr => new cVector4(z, y, x, x);
        
        /// <summary>
        /// Returns cVector4.zyxy swizzling.
        /// </summary>
        public cVector4 zyxy => new cVector4(z, y, x, y);
        
        /// <summary>
        /// Returns cVector4.bgrg swizzling (equivalent to cVector4.zyxy).
        /// </summary>
        public cVector4 bgrg => new cVector4(z, y, x, y);
        
        /// <summary>
        /// Returns cVector4.zyxz swizzling.
        /// </summary>
        public cVector4 zyxz => new cVector4(z, y, x, z);
        
        /// <summary>
        /// Returns cVector4.bgrb swizzling (equivalent to cVector4.zyxz).
        /// </summary>
        public cVector4 bgrb => new cVector4(z, y, x, z);
        
        /// <summary>
        /// Returns cVector4.zyxw swizzling.
        /// </summary>
        public cVector4 zyxw => new cVector4(z, y, x, w);
        
        /// <summary>
        /// Returns cVector4.bgra swizzling (equivalent to cVector4.zyxw).
        /// </summary>
        public cVector4 bgra => new cVector4(z, y, x, w);
        
        /// <summary>
        /// Returns cVector4.zyy swizzling.
        /// </summary>
        public cVector3 zyy => new cVector3(z, y, y);
        
        /// <summary>
        /// Returns cVector4.bgg swizzling (equivalent to cVector4.zyy).
        /// </summary>
        public cVector3 bgg => new cVector3(z, y, y);
        
        /// <summary>
        /// Returns cVector4.zyyx swizzling.
        /// </summary>
        public cVector4 zyyx => new cVector4(z, y, y, x);
        
        /// <summary>
        /// Returns cVector4.bggr swizzling (equivalent to cVector4.zyyx).
        /// </summary>
        public cVector4 bggr => new cVector4(z, y, y, x);
        
        /// <summary>
        /// Returns cVector4.zyyy swizzling.
        /// </summary>
        public cVector4 zyyy => new cVector4(z, y, y, y);
        
        /// <summary>
        /// Returns cVector4.bggg swizzling (equivalent to cVector4.zyyy).
        /// </summary>
        public cVector4 bggg => new cVector4(z, y, y, y);
        
        /// <summary>
        /// Returns cVector4.zyyz swizzling.
        /// </summary>
        public cVector4 zyyz => new cVector4(z, y, y, z);
        
        /// <summary>
        /// Returns cVector4.bggb swizzling (equivalent to cVector4.zyyz).
        /// </summary>
        public cVector4 bggb => new cVector4(z, y, y, z);
        
        /// <summary>
        /// Returns cVector4.zyyw swizzling.
        /// </summary>
        public cVector4 zyyw => new cVector4(z, y, y, w);
        
        /// <summary>
        /// Returns cVector4.bgga swizzling (equivalent to cVector4.zyyw).
        /// </summary>
        public cVector4 bgga => new cVector4(z, y, y, w);
        
        /// <summary>
        /// Returns cVector4.zyz swizzling.
        /// </summary>
        public cVector3 zyz => new cVector3(z, y, z);
        
        /// <summary>
        /// Returns cVector4.bgb swizzling (equivalent to cVector4.zyz).
        /// </summary>
        public cVector3 bgb => new cVector3(z, y, z);
        
        /// <summary>
        /// Returns cVector4.zyzx swizzling.
        /// </summary>
        public cVector4 zyzx => new cVector4(z, y, z, x);
        
        /// <summary>
        /// Returns cVector4.bgbr swizzling (equivalent to cVector4.zyzx).
        /// </summary>
        public cVector4 bgbr => new cVector4(z, y, z, x);
        
        /// <summary>
        /// Returns cVector4.zyzy swizzling.
        /// </summary>
        public cVector4 zyzy => new cVector4(z, y, z, y);
        
        /// <summary>
        /// Returns cVector4.bgbg swizzling (equivalent to cVector4.zyzy).
        /// </summary>
        public cVector4 bgbg => new cVector4(z, y, z, y);
        
        /// <summary>
        /// Returns cVector4.zyzz swizzling.
        /// </summary>
        public cVector4 zyzz => new cVector4(z, y, z, z);
        
        /// <summary>
        /// Returns cVector4.bgbb swizzling (equivalent to cVector4.zyzz).
        /// </summary>
        public cVector4 bgbb => new cVector4(z, y, z, z);
        
        /// <summary>
        /// Returns cVector4.zyzw swizzling.
        /// </summary>
        public cVector4 zyzw => new cVector4(z, y, z, w);
        
        /// <summary>
        /// Returns cVector4.bgba swizzling (equivalent to cVector4.zyzw).
        /// </summary>
        public cVector4 bgba => new cVector4(z, y, z, w);
        
        /// <summary>
        /// Returns cVector4.zyw swizzling.
        /// </summary>
        public cVector3 zyw => new cVector3(z, y, w);
        
        /// <summary>
        /// Returns cVector4.bga swizzling (equivalent to cVector4.zyw).
        /// </summary>
        public cVector3 bga => new cVector3(z, y, w);
        
        /// <summary>
        /// Returns cVector4.zywx swizzling.
        /// </summary>
        public cVector4 zywx => new cVector4(z, y, w, x);
        
        /// <summary>
        /// Returns cVector4.bgar swizzling (equivalent to cVector4.zywx).
        /// </summary>
        public cVector4 bgar => new cVector4(z, y, w, x);
        
        /// <summary>
        /// Returns cVector4.zywy swizzling.
        /// </summary>
        public cVector4 zywy => new cVector4(z, y, w, y);
        
        /// <summary>
        /// Returns cVector4.bgag swizzling (equivalent to cVector4.zywy).
        /// </summary>
        public cVector4 bgag => new cVector4(z, y, w, y);
        
        /// <summary>
        /// Returns cVector4.zywz swizzling.
        /// </summary>
        public cVector4 zywz => new cVector4(z, y, w, z);
        
        /// <summary>
        /// Returns cVector4.bgab swizzling (equivalent to cVector4.zywz).
        /// </summary>
        public cVector4 bgab => new cVector4(z, y, w, z);
        
        /// <summary>
        /// Returns cVector4.zyww swizzling.
        /// </summary>
        public cVector4 zyww => new cVector4(z, y, w, w);
        
        /// <summary>
        /// Returns cVector4.bgaa swizzling (equivalent to cVector4.zyww).
        /// </summary>
        public cVector4 bgaa => new cVector4(z, y, w, w);
        
        /// <summary>
        /// Returns cVector4.zz swizzling.
        /// </summary>
        public cVector2 zz => new cVector2(z, z);
        
        /// <summary>
        /// Returns cVector4.bb swizzling (equivalent to cVector4.zz).
        /// </summary>
        public cVector2 bb => new cVector2(z, z);
        
        /// <summary>
        /// Returns cVector4.zzx swizzling.
        /// </summary>
        public cVector3 zzx => new cVector3(z, z, x);
        
        /// <summary>
        /// Returns cVector4.bbr swizzling (equivalent to cVector4.zzx).
        /// </summary>
        public cVector3 bbr => new cVector3(z, z, x);
        
        /// <summary>
        /// Returns cVector4.zzxx swizzling.
        /// </summary>
        public cVector4 zzxx => new cVector4(z, z, x, x);
        
        /// <summary>
        /// Returns cVector4.bbrr swizzling (equivalent to cVector4.zzxx).
        /// </summary>
        public cVector4 bbrr => new cVector4(z, z, x, x);
        
        /// <summary>
        /// Returns cVector4.zzxy swizzling.
        /// </summary>
        public cVector4 zzxy => new cVector4(z, z, x, y);
        
        /// <summary>
        /// Returns cVector4.bbrg swizzling (equivalent to cVector4.zzxy).
        /// </summary>
        public cVector4 bbrg => new cVector4(z, z, x, y);
        
        /// <summary>
        /// Returns cVector4.zzxz swizzling.
        /// </summary>
        public cVector4 zzxz => new cVector4(z, z, x, z);
        
        /// <summary>
        /// Returns cVector4.bbrb swizzling (equivalent to cVector4.zzxz).
        /// </summary>
        public cVector4 bbrb => new cVector4(z, z, x, z);
        
        /// <summary>
        /// Returns cVector4.zzxw swizzling.
        /// </summary>
        public cVector4 zzxw => new cVector4(z, z, x, w);
        
        /// <summary>
        /// Returns cVector4.bbra swizzling (equivalent to cVector4.zzxw).
        /// </summary>
        public cVector4 bbra => new cVector4(z, z, x, w);
        
        /// <summary>
        /// Returns cVector4.zzy swizzling.
        /// </summary>
        public cVector3 zzy => new cVector3(z, z, y);
        
        /// <summary>
        /// Returns cVector4.bbg swizzling (equivalent to cVector4.zzy).
        /// </summary>
        public cVector3 bbg => new cVector3(z, z, y);
        
        /// <summary>
        /// Returns cVector4.zzyx swizzling.
        /// </summary>
        public cVector4 zzyx => new cVector4(z, z, y, x);
        
        /// <summary>
        /// Returns cVector4.bbgr swizzling (equivalent to cVector4.zzyx).
        /// </summary>
        public cVector4 bbgr => new cVector4(z, z, y, x);
        
        /// <summary>
        /// Returns cVector4.zzyy swizzling.
        /// </summary>
        public cVector4 zzyy => new cVector4(z, z, y, y);
        
        /// <summary>
        /// Returns cVector4.bbgg swizzling (equivalent to cVector4.zzyy).
        /// </summary>
        public cVector4 bbgg => new cVector4(z, z, y, y);
        
        /// <summary>
        /// Returns cVector4.zzyz swizzling.
        /// </summary>
        public cVector4 zzyz => new cVector4(z, z, y, z);
        
        /// <summary>
        /// Returns cVector4.bbgb swizzling (equivalent to cVector4.zzyz).
        /// </summary>
        public cVector4 bbgb => new cVector4(z, z, y, z);
        
        /// <summary>
        /// Returns cVector4.zzyw swizzling.
        /// </summary>
        public cVector4 zzyw => new cVector4(z, z, y, w);
        
        /// <summary>
        /// Returns cVector4.bbga swizzling (equivalent to cVector4.zzyw).
        /// </summary>
        public cVector4 bbga => new cVector4(z, z, y, w);
        
        /// <summary>
        /// Returns cVector4.zzz swizzling.
        /// </summary>
        public cVector3 zzz => new cVector3(z, z, z);
        
        /// <summary>
        /// Returns cVector4.bbb swizzling (equivalent to cVector4.zzz).
        /// </summary>
        public cVector3 bbb => new cVector3(z, z, z);
        
        /// <summary>
        /// Returns cVector4.zzzx swizzling.
        /// </summary>
        public cVector4 zzzx => new cVector4(z, z, z, x);
        
        /// <summary>
        /// Returns cVector4.bbbr swizzling (equivalent to cVector4.zzzx).
        /// </summary>
        public cVector4 bbbr => new cVector4(z, z, z, x);
        
        /// <summary>
        /// Returns cVector4.zzzy swizzling.
        /// </summary>
        public cVector4 zzzy => new cVector4(z, z, z, y);
        
        /// <summary>
        /// Returns cVector4.bbbg swizzling (equivalent to cVector4.zzzy).
        /// </summary>
        public cVector4 bbbg => new cVector4(z, z, z, y);
        
        /// <summary>
        /// Returns cVector4.zzzz swizzling.
        /// </summary>
        public cVector4 zzzz => new cVector4(z, z, z, z);
        
        /// <summary>
        /// Returns cVector4.bbbb swizzling (equivalent to cVector4.zzzz).
        /// </summary>
        public cVector4 bbbb => new cVector4(z, z, z, z);
        
        /// <summary>
        /// Returns cVector4.zzzw swizzling.
        /// </summary>
        public cVector4 zzzw => new cVector4(z, z, z, w);
        
        /// <summary>
        /// Returns cVector4.bbba swizzling (equivalent to cVector4.zzzw).
        /// </summary>
        public cVector4 bbba => new cVector4(z, z, z, w);
        
        /// <summary>
        /// Returns cVector4.zzw swizzling.
        /// </summary>
        public cVector3 zzw => new cVector3(z, z, w);
        
        /// <summary>
        /// Returns cVector4.bba swizzling (equivalent to cVector4.zzw).
        /// </summary>
        public cVector3 bba => new cVector3(z, z, w);
        
        /// <summary>
        /// Returns cVector4.zzwx swizzling.
        /// </summary>
        public cVector4 zzwx => new cVector4(z, z, w, x);
        
        /// <summary>
        /// Returns cVector4.bbar swizzling (equivalent to cVector4.zzwx).
        /// </summary>
        public cVector4 bbar => new cVector4(z, z, w, x);
        
        /// <summary>
        /// Returns cVector4.zzwy swizzling.
        /// </summary>
        public cVector4 zzwy => new cVector4(z, z, w, y);
        
        /// <summary>
        /// Returns cVector4.bbag swizzling (equivalent to cVector4.zzwy).
        /// </summary>
        public cVector4 bbag => new cVector4(z, z, w, y);
        
        /// <summary>
        /// Returns cVector4.zzwz swizzling.
        /// </summary>
        public cVector4 zzwz => new cVector4(z, z, w, z);
        
        /// <summary>
        /// Returns cVector4.bbab swizzling (equivalent to cVector4.zzwz).
        /// </summary>
        public cVector4 bbab => new cVector4(z, z, w, z);
        
        /// <summary>
        /// Returns cVector4.zzww swizzling.
        /// </summary>
        public cVector4 zzww => new cVector4(z, z, w, w);
        
        /// <summary>
        /// Returns cVector4.bbaa swizzling (equivalent to cVector4.zzww).
        /// </summary>
        public cVector4 bbaa => new cVector4(z, z, w, w);
        
        /// <summary>
        /// Returns cVector4.zw swizzling.
        /// </summary>
        public cVector2 zw => new cVector2(z, w);
        
        /// <summary>
        /// Returns cVector4.ba swizzling (equivalent to cVector4.zw).
        /// </summary>
        public cVector2 ba => new cVector2(z, w);
        
        /// <summary>
        /// Returns cVector4.zwx swizzling.
        /// </summary>
        public cVector3 zwx => new cVector3(z, w, x);
        
        /// <summary>
        /// Returns cVector4.bar swizzling (equivalent to cVector4.zwx).
        /// </summary>
        public cVector3 bar => new cVector3(z, w, x);
        
        /// <summary>
        /// Returns cVector4.zwxx swizzling.
        /// </summary>
        public cVector4 zwxx => new cVector4(z, w, x, x);
        
        /// <summary>
        /// Returns cVector4.barr swizzling (equivalent to cVector4.zwxx).
        /// </summary>
        public cVector4 barr => new cVector4(z, w, x, x);
        
        /// <summary>
        /// Returns cVector4.zwxy swizzling.
        /// </summary>
        public cVector4 zwxy => new cVector4(z, w, x, y);
        
        /// <summary>
        /// Returns cVector4.barg swizzling (equivalent to cVector4.zwxy).
        /// </summary>
        public cVector4 barg => new cVector4(z, w, x, y);
        
        /// <summary>
        /// Returns cVector4.zwxz swizzling.
        /// </summary>
        public cVector4 zwxz => new cVector4(z, w, x, z);
        
        /// <summary>
        /// Returns cVector4.barb swizzling (equivalent to cVector4.zwxz).
        /// </summary>
        public cVector4 barb => new cVector4(z, w, x, z);
        
        /// <summary>
        /// Returns cVector4.zwxw swizzling.
        /// </summary>
        public cVector4 zwxw => new cVector4(z, w, x, w);
        
        /// <summary>
        /// Returns cVector4.bara swizzling (equivalent to cVector4.zwxw).
        /// </summary>
        public cVector4 bara => new cVector4(z, w, x, w);
        
        /// <summary>
        /// Returns cVector4.zwy swizzling.
        /// </summary>
        public cVector3 zwy => new cVector3(z, w, y);
        
        /// <summary>
        /// Returns cVector4.bag swizzling (equivalent to cVector4.zwy).
        /// </summary>
        public cVector3 bag => new cVector3(z, w, y);
        
        /// <summary>
        /// Returns cVector4.zwyx swizzling.
        /// </summary>
        public cVector4 zwyx => new cVector4(z, w, y, x);
        
        /// <summary>
        /// Returns cVector4.bagr swizzling (equivalent to cVector4.zwyx).
        /// </summary>
        public cVector4 bagr => new cVector4(z, w, y, x);
        
        /// <summary>
        /// Returns cVector4.zwyy swizzling.
        /// </summary>
        public cVector4 zwyy => new cVector4(z, w, y, y);
        
        /// <summary>
        /// Returns cVector4.bagg swizzling (equivalent to cVector4.zwyy).
        /// </summary>
        public cVector4 bagg => new cVector4(z, w, y, y);
        
        /// <summary>
        /// Returns cVector4.zwyz swizzling.
        /// </summary>
        public cVector4 zwyz => new cVector4(z, w, y, z);
        
        /// <summary>
        /// Returns cVector4.bagb swizzling (equivalent to cVector4.zwyz).
        /// </summary>
        public cVector4 bagb => new cVector4(z, w, y, z);
        
        /// <summary>
        /// Returns cVector4.zwyw swizzling.
        /// </summary>
        public cVector4 zwyw => new cVector4(z, w, y, w);
        
        /// <summary>
        /// Returns cVector4.baga swizzling (equivalent to cVector4.zwyw).
        /// </summary>
        public cVector4 baga => new cVector4(z, w, y, w);
        
        /// <summary>
        /// Returns cVector4.zwz swizzling.
        /// </summary>
        public cVector3 zwz => new cVector3(z, w, z);
        
        /// <summary>
        /// Returns cVector4.bab swizzling (equivalent to cVector4.zwz).
        /// </summary>
        public cVector3 bab => new cVector3(z, w, z);
        
        /// <summary>
        /// Returns cVector4.zwzx swizzling.
        /// </summary>
        public cVector4 zwzx => new cVector4(z, w, z, x);
        
        /// <summary>
        /// Returns cVector4.babr swizzling (equivalent to cVector4.zwzx).
        /// </summary>
        public cVector4 babr => new cVector4(z, w, z, x);
        
        /// <summary>
        /// Returns cVector4.zwzy swizzling.
        /// </summary>
        public cVector4 zwzy => new cVector4(z, w, z, y);
        
        /// <summary>
        /// Returns cVector4.babg swizzling (equivalent to cVector4.zwzy).
        /// </summary>
        public cVector4 babg => new cVector4(z, w, z, y);
        
        /// <summary>
        /// Returns cVector4.zwzz swizzling.
        /// </summary>
        public cVector4 zwzz => new cVector4(z, w, z, z);
        
        /// <summary>
        /// Returns cVector4.babb swizzling (equivalent to cVector4.zwzz).
        /// </summary>
        public cVector4 babb => new cVector4(z, w, z, z);
        
        /// <summary>
        /// Returns cVector4.zwzw swizzling.
        /// </summary>
        public cVector4 zwzw => new cVector4(z, w, z, w);
        
        /// <summary>
        /// Returns cVector4.baba swizzling (equivalent to cVector4.zwzw).
        /// </summary>
        public cVector4 baba => new cVector4(z, w, z, w);
        
        /// <summary>
        /// Returns cVector4.zww swizzling.
        /// </summary>
        public cVector3 zww => new cVector3(z, w, w);
        
        /// <summary>
        /// Returns cVector4.baa swizzling (equivalent to cVector4.zww).
        /// </summary>
        public cVector3 baa => new cVector3(z, w, w);
        
        /// <summary>
        /// Returns cVector4.zwwx swizzling.
        /// </summary>
        public cVector4 zwwx => new cVector4(z, w, w, x);
        
        /// <summary>
        /// Returns cVector4.baar swizzling (equivalent to cVector4.zwwx).
        /// </summary>
        public cVector4 baar => new cVector4(z, w, w, x);
        
        /// <summary>
        /// Returns cVector4.zwwy swizzling.
        /// </summary>
        public cVector4 zwwy => new cVector4(z, w, w, y);
        
        /// <summary>
        /// Returns cVector4.baag swizzling (equivalent to cVector4.zwwy).
        /// </summary>
        public cVector4 baag => new cVector4(z, w, w, y);
        
        /// <summary>
        /// Returns cVector4.zwwz swizzling.
        /// </summary>
        public cVector4 zwwz => new cVector4(z, w, w, z);
        
        /// <summary>
        /// Returns cVector4.baab swizzling (equivalent to cVector4.zwwz).
        /// </summary>
        public cVector4 baab => new cVector4(z, w, w, z);
        
        /// <summary>
        /// Returns cVector4.zwww swizzling.
        /// </summary>
        public cVector4 zwww => new cVector4(z, w, w, w);
        
        /// <summary>
        /// Returns cVector4.baaa swizzling (equivalent to cVector4.zwww).
        /// </summary>
        public cVector4 baaa => new cVector4(z, w, w, w);
        
        /// <summary>
        /// Returns cVector4.wx swizzling.
        /// </summary>
        public cVector2 wx => new cVector2(w, x);
        
        /// <summary>
        /// Returns cVector4.ar swizzling (equivalent to cVector4.wx).
        /// </summary>
        public cVector2 ar => new cVector2(w, x);
        
        /// <summary>
        /// Returns cVector4.wxx swizzling.
        /// </summary>
        public cVector3 wxx => new cVector3(w, x, x);
        
        /// <summary>
        /// Returns cVector4.arr swizzling (equivalent to cVector4.wxx).
        /// </summary>
        public cVector3 arr => new cVector3(w, x, x);
        
        /// <summary>
        /// Returns cVector4.wxxx swizzling.
        /// </summary>
        public cVector4 wxxx => new cVector4(w, x, x, x);
        
        /// <summary>
        /// Returns cVector4.arrr swizzling (equivalent to cVector4.wxxx).
        /// </summary>
        public cVector4 arrr => new cVector4(w, x, x, x);
        
        /// <summary>
        /// Returns cVector4.wxxy swizzling.
        /// </summary>
        public cVector4 wxxy => new cVector4(w, x, x, y);
        
        /// <summary>
        /// Returns cVector4.arrg swizzling (equivalent to cVector4.wxxy).
        /// </summary>
        public cVector4 arrg => new cVector4(w, x, x, y);
        
        /// <summary>
        /// Returns cVector4.wxxz swizzling.
        /// </summary>
        public cVector4 wxxz => new cVector4(w, x, x, z);
        
        /// <summary>
        /// Returns cVector4.arrb swizzling (equivalent to cVector4.wxxz).
        /// </summary>
        public cVector4 arrb => new cVector4(w, x, x, z);
        
        /// <summary>
        /// Returns cVector4.wxxw swizzling.
        /// </summary>
        public cVector4 wxxw => new cVector4(w, x, x, w);
        
        /// <summary>
        /// Returns cVector4.arra swizzling (equivalent to cVector4.wxxw).
        /// </summary>
        public cVector4 arra => new cVector4(w, x, x, w);
        
        /// <summary>
        /// Returns cVector4.wxy swizzling.
        /// </summary>
        public cVector3 wxy => new cVector3(w, x, y);
        
        /// <summary>
        /// Returns cVector4.arg swizzling (equivalent to cVector4.wxy).
        /// </summary>
        public cVector3 arg => new cVector3(w, x, y);
        
        /// <summary>
        /// Returns cVector4.wxyx swizzling.
        /// </summary>
        public cVector4 wxyx => new cVector4(w, x, y, x);
        
        /// <summary>
        /// Returns cVector4.argr swizzling (equivalent to cVector4.wxyx).
        /// </summary>
        public cVector4 argr => new cVector4(w, x, y, x);
        
        /// <summary>
        /// Returns cVector4.wxyy swizzling.
        /// </summary>
        public cVector4 wxyy => new cVector4(w, x, y, y);
        
        /// <summary>
        /// Returns cVector4.argg swizzling (equivalent to cVector4.wxyy).
        /// </summary>
        public cVector4 argg => new cVector4(w, x, y, y);
        
        /// <summary>
        /// Returns cVector4.wxyz swizzling.
        /// </summary>
        public cVector4 wxyz => new cVector4(w, x, y, z);
        
        /// <summary>
        /// Returns cVector4.argb swizzling (equivalent to cVector4.wxyz).
        /// </summary>
        public cVector4 argb => new cVector4(w, x, y, z);
        
        /// <summary>
        /// Returns cVector4.wxyw swizzling.
        /// </summary>
        public cVector4 wxyw => new cVector4(w, x, y, w);
        
        /// <summary>
        /// Returns cVector4.arga swizzling (equivalent to cVector4.wxyw).
        /// </summary>
        public cVector4 arga => new cVector4(w, x, y, w);
        
        /// <summary>
        /// Returns cVector4.wxz swizzling.
        /// </summary>
        public cVector3 wxz => new cVector3(w, x, z);
        
        /// <summary>
        /// Returns cVector4.arb swizzling (equivalent to cVector4.wxz).
        /// </summary>
        public cVector3 arb => new cVector3(w, x, z);
        
        /// <summary>
        /// Returns cVector4.wxzx swizzling.
        /// </summary>
        public cVector4 wxzx => new cVector4(w, x, z, x);
        
        /// <summary>
        /// Returns cVector4.arbr swizzling (equivalent to cVector4.wxzx).
        /// </summary>
        public cVector4 arbr => new cVector4(w, x, z, x);
        
        /// <summary>
        /// Returns cVector4.wxzy swizzling.
        /// </summary>
        public cVector4 wxzy => new cVector4(w, x, z, y);
        
        /// <summary>
        /// Returns cVector4.arbg swizzling (equivalent to cVector4.wxzy).
        /// </summary>
        public cVector4 arbg => new cVector4(w, x, z, y);
        
        /// <summary>
        /// Returns cVector4.wxzz swizzling.
        /// </summary>
        public cVector4 wxzz => new cVector4(w, x, z, z);
        
        /// <summary>
        /// Returns cVector4.arbb swizzling (equivalent to cVector4.wxzz).
        /// </summary>
        public cVector4 arbb => new cVector4(w, x, z, z);
        
        /// <summary>
        /// Returns cVector4.wxzw swizzling.
        /// </summary>
        public cVector4 wxzw => new cVector4(w, x, z, w);
        
        /// <summary>
        /// Returns cVector4.arba swizzling (equivalent to cVector4.wxzw).
        /// </summary>
        public cVector4 arba => new cVector4(w, x, z, w);
        
        /// <summary>
        /// Returns cVector4.wxw swizzling.
        /// </summary>
        public cVector3 wxw => new cVector3(w, x, w);
        
        /// <summary>
        /// Returns cVector4.ara swizzling (equivalent to cVector4.wxw).
        /// </summary>
        public cVector3 ara => new cVector3(w, x, w);
        
        /// <summary>
        /// Returns cVector4.wxwx swizzling.
        /// </summary>
        public cVector4 wxwx => new cVector4(w, x, w, x);
        
        /// <summary>
        /// Returns cVector4.arar swizzling (equivalent to cVector4.wxwx).
        /// </summary>
        public cVector4 arar => new cVector4(w, x, w, x);
        
        /// <summary>
        /// Returns cVector4.wxwy swizzling.
        /// </summary>
        public cVector4 wxwy => new cVector4(w, x, w, y);
        
        /// <summary>
        /// Returns cVector4.arag swizzling (equivalent to cVector4.wxwy).
        /// </summary>
        public cVector4 arag => new cVector4(w, x, w, y);
        
        /// <summary>
        /// Returns cVector4.wxwz swizzling.
        /// </summary>
        public cVector4 wxwz => new cVector4(w, x, w, z);
        
        /// <summary>
        /// Returns cVector4.arab swizzling (equivalent to cVector4.wxwz).
        /// </summary>
        public cVector4 arab => new cVector4(w, x, w, z);
        
        /// <summary>
        /// Returns cVector4.wxww swizzling.
        /// </summary>
        public cVector4 wxww => new cVector4(w, x, w, w);
        
        /// <summary>
        /// Returns cVector4.araa swizzling (equivalent to cVector4.wxww).
        /// </summary>
        public cVector4 araa => new cVector4(w, x, w, w);
        
        /// <summary>
        /// Returns cVector4.wy swizzling.
        /// </summary>
        public cVector2 wy => new cVector2(w, y);
        
        /// <summary>
        /// Returns cVector4.ag swizzling (equivalent to cVector4.wy).
        /// </summary>
        public cVector2 ag => new cVector2(w, y);
        
        /// <summary>
        /// Returns cVector4.wyx swizzling.
        /// </summary>
        public cVector3 wyx => new cVector3(w, y, x);
        
        /// <summary>
        /// Returns cVector4.agr swizzling (equivalent to cVector4.wyx).
        /// </summary>
        public cVector3 agr => new cVector3(w, y, x);
        
        /// <summary>
        /// Returns cVector4.wyxx swizzling.
        /// </summary>
        public cVector4 wyxx => new cVector4(w, y, x, x);
        
        /// <summary>
        /// Returns cVector4.agrr swizzling (equivalent to cVector4.wyxx).
        /// </summary>
        public cVector4 agrr => new cVector4(w, y, x, x);
        
        /// <summary>
        /// Returns cVector4.wyxy swizzling.
        /// </summary>
        public cVector4 wyxy => new cVector4(w, y, x, y);
        
        /// <summary>
        /// Returns cVector4.agrg swizzling (equivalent to cVector4.wyxy).
        /// </summary>
        public cVector4 agrg => new cVector4(w, y, x, y);
        
        /// <summary>
        /// Returns cVector4.wyxz swizzling.
        /// </summary>
        public cVector4 wyxz => new cVector4(w, y, x, z);
        
        /// <summary>
        /// Returns cVector4.agrb swizzling (equivalent to cVector4.wyxz).
        /// </summary>
        public cVector4 agrb => new cVector4(w, y, x, z);
        
        /// <summary>
        /// Returns cVector4.wyxw swizzling.
        /// </summary>
        public cVector4 wyxw => new cVector4(w, y, x, w);
        
        /// <summary>
        /// Returns cVector4.agra swizzling (equivalent to cVector4.wyxw).
        /// </summary>
        public cVector4 agra => new cVector4(w, y, x, w);
        
        /// <summary>
        /// Returns cVector4.wyy swizzling.
        /// </summary>
        public cVector3 wyy => new cVector3(w, y, y);
        
        /// <summary>
        /// Returns cVector4.agg swizzling (equivalent to cVector4.wyy).
        /// </summary>
        public cVector3 agg => new cVector3(w, y, y);
        
        /// <summary>
        /// Returns cVector4.wyyx swizzling.
        /// </summary>
        public cVector4 wyyx => new cVector4(w, y, y, x);
        
        /// <summary>
        /// Returns cVector4.aggr swizzling (equivalent to cVector4.wyyx).
        /// </summary>
        public cVector4 aggr => new cVector4(w, y, y, x);
        
        /// <summary>
        /// Returns cVector4.wyyy swizzling.
        /// </summary>
        public cVector4 wyyy => new cVector4(w, y, y, y);
        
        /// <summary>
        /// Returns cVector4.aggg swizzling (equivalent to cVector4.wyyy).
        /// </summary>
        public cVector4 aggg => new cVector4(w, y, y, y);
        
        /// <summary>
        /// Returns cVector4.wyyz swizzling.
        /// </summary>
        public cVector4 wyyz => new cVector4(w, y, y, z);
        
        /// <summary>
        /// Returns cVector4.aggb swizzling (equivalent to cVector4.wyyz).
        /// </summary>
        public cVector4 aggb => new cVector4(w, y, y, z);
        
        /// <summary>
        /// Returns cVector4.wyyw swizzling.
        /// </summary>
        public cVector4 wyyw => new cVector4(w, y, y, w);
        
        /// <summary>
        /// Returns cVector4.agga swizzling (equivalent to cVector4.wyyw).
        /// </summary>
        public cVector4 agga => new cVector4(w, y, y, w);
        
        /// <summary>
        /// Returns cVector4.wyz swizzling.
        /// </summary>
        public cVector3 wyz => new cVector3(w, y, z);
        
        /// <summary>
        /// Returns cVector4.agb swizzling (equivalent to cVector4.wyz).
        /// </summary>
        public cVector3 agb => new cVector3(w, y, z);
        
        /// <summary>
        /// Returns cVector4.wyzx swizzling.
        /// </summary>
        public cVector4 wyzx => new cVector4(w, y, z, x);
        
        /// <summary>
        /// Returns cVector4.agbr swizzling (equivalent to cVector4.wyzx).
        /// </summary>
        public cVector4 agbr => new cVector4(w, y, z, x);
        
        /// <summary>
        /// Returns cVector4.wyzy swizzling.
        /// </summary>
        public cVector4 wyzy => new cVector4(w, y, z, y);
        
        /// <summary>
        /// Returns cVector4.agbg swizzling (equivalent to cVector4.wyzy).
        /// </summary>
        public cVector4 agbg => new cVector4(w, y, z, y);
        
        /// <summary>
        /// Returns cVector4.wyzz swizzling.
        /// </summary>
        public cVector4 wyzz => new cVector4(w, y, z, z);
        
        /// <summary>
        /// Returns cVector4.agbb swizzling (equivalent to cVector4.wyzz).
        /// </summary>
        public cVector4 agbb => new cVector4(w, y, z, z);
        
        /// <summary>
        /// Returns cVector4.wyzw swizzling.
        /// </summary>
        public cVector4 wyzw => new cVector4(w, y, z, w);
        
        /// <summary>
        /// Returns cVector4.agba swizzling (equivalent to cVector4.wyzw).
        /// </summary>
        public cVector4 agba => new cVector4(w, y, z, w);
        
        /// <summary>
        /// Returns cVector4.wyw swizzling.
        /// </summary>
        public cVector3 wyw => new cVector3(w, y, w);
        
        /// <summary>
        /// Returns cVector4.aga swizzling (equivalent to cVector4.wyw).
        /// </summary>
        public cVector3 aga => new cVector3(w, y, w);
        
        /// <summary>
        /// Returns cVector4.wywx swizzling.
        /// </summary>
        public cVector4 wywx => new cVector4(w, y, w, x);
        
        /// <summary>
        /// Returns cVector4.agar swizzling (equivalent to cVector4.wywx).
        /// </summary>
        public cVector4 agar => new cVector4(w, y, w, x);
        
        /// <summary>
        /// Returns cVector4.wywy swizzling.
        /// </summary>
        public cVector4 wywy => new cVector4(w, y, w, y);
        
        /// <summary>
        /// Returns cVector4.agag swizzling (equivalent to cVector4.wywy).
        /// </summary>
        public cVector4 agag => new cVector4(w, y, w, y);
        
        /// <summary>
        /// Returns cVector4.wywz swizzling.
        /// </summary>
        public cVector4 wywz => new cVector4(w, y, w, z);
        
        /// <summary>
        /// Returns cVector4.agab swizzling (equivalent to cVector4.wywz).
        /// </summary>
        public cVector4 agab => new cVector4(w, y, w, z);
        
        /// <summary>
        /// Returns cVector4.wyww swizzling.
        /// </summary>
        public cVector4 wyww => new cVector4(w, y, w, w);
        
        /// <summary>
        /// Returns cVector4.agaa swizzling (equivalent to cVector4.wyww).
        /// </summary>
        public cVector4 agaa => new cVector4(w, y, w, w);
        
        /// <summary>
        /// Returns cVector4.wz swizzling.
        /// </summary>
        public cVector2 wz => new cVector2(w, z);
        
        /// <summary>
        /// Returns cVector4.ab swizzling (equivalent to cVector4.wz).
        /// </summary>
        public cVector2 ab => new cVector2(w, z);
        
        /// <summary>
        /// Returns cVector4.wzx swizzling.
        /// </summary>
        public cVector3 wzx => new cVector3(w, z, x);
        
        /// <summary>
        /// Returns cVector4.abr swizzling (equivalent to cVector4.wzx).
        /// </summary>
        public cVector3 abr => new cVector3(w, z, x);
        
        /// <summary>
        /// Returns cVector4.wzxx swizzling.
        /// </summary>
        public cVector4 wzxx => new cVector4(w, z, x, x);
        
        /// <summary>
        /// Returns cVector4.abrr swizzling (equivalent to cVector4.wzxx).
        /// </summary>
        public cVector4 abrr => new cVector4(w, z, x, x);
        
        /// <summary>
        /// Returns cVector4.wzxy swizzling.
        /// </summary>
        public cVector4 wzxy => new cVector4(w, z, x, y);
        
        /// <summary>
        /// Returns cVector4.abrg swizzling (equivalent to cVector4.wzxy).
        /// </summary>
        public cVector4 abrg => new cVector4(w, z, x, y);
        
        /// <summary>
        /// Returns cVector4.wzxz swizzling.
        /// </summary>
        public cVector4 wzxz => new cVector4(w, z, x, z);
        
        /// <summary>
        /// Returns cVector4.abrb swizzling (equivalent to cVector4.wzxz).
        /// </summary>
        public cVector4 abrb => new cVector4(w, z, x, z);
        
        /// <summary>
        /// Returns cVector4.wzxw swizzling.
        /// </summary>
        public cVector4 wzxw => new cVector4(w, z, x, w);
        
        /// <summary>
        /// Returns cVector4.abra swizzling (equivalent to cVector4.wzxw).
        /// </summary>
        public cVector4 abra => new cVector4(w, z, x, w);
        
        /// <summary>
        /// Returns cVector4.wzy swizzling.
        /// </summary>
        public cVector3 wzy => new cVector3(w, z, y);
        
        /// <summary>
        /// Returns cVector4.abg swizzling (equivalent to cVector4.wzy).
        /// </summary>
        public cVector3 abg => new cVector3(w, z, y);
        
        /// <summary>
        /// Returns cVector4.wzyx swizzling.
        /// </summary>
        public cVector4 wzyx => new cVector4(w, z, y, x);
        
        /// <summary>
        /// Returns cVector4.abgr swizzling (equivalent to cVector4.wzyx).
        /// </summary>
        public cVector4 abgr => new cVector4(w, z, y, x);
        
        /// <summary>
        /// Returns cVector4.wzyy swizzling.
        /// </summary>
        public cVector4 wzyy => new cVector4(w, z, y, y);
        
        /// <summary>
        /// Returns cVector4.abgg swizzling (equivalent to cVector4.wzyy).
        /// </summary>
        public cVector4 abgg => new cVector4(w, z, y, y);
        
        /// <summary>
        /// Returns cVector4.wzyz swizzling.
        /// </summary>
        public cVector4 wzyz => new cVector4(w, z, y, z);
        
        /// <summary>
        /// Returns cVector4.abgb swizzling (equivalent to cVector4.wzyz).
        /// </summary>
        public cVector4 abgb => new cVector4(w, z, y, z);
        
        /// <summary>
        /// Returns cVector4.wzyw swizzling.
        /// </summary>
        public cVector4 wzyw => new cVector4(w, z, y, w);
        
        /// <summary>
        /// Returns cVector4.abga swizzling (equivalent to cVector4.wzyw).
        /// </summary>
        public cVector4 abga => new cVector4(w, z, y, w);
        
        /// <summary>
        /// Returns cVector4.wzz swizzling.
        /// </summary>
        public cVector3 wzz => new cVector3(w, z, z);
        
        /// <summary>
        /// Returns cVector4.abb swizzling (equivalent to cVector4.wzz).
        /// </summary>
        public cVector3 abb => new cVector3(w, z, z);
        
        /// <summary>
        /// Returns cVector4.wzzx swizzling.
        /// </summary>
        public cVector4 wzzx => new cVector4(w, z, z, x);
        
        /// <summary>
        /// Returns cVector4.abbr swizzling (equivalent to cVector4.wzzx).
        /// </summary>
        public cVector4 abbr => new cVector4(w, z, z, x);
        
        /// <summary>
        /// Returns cVector4.wzzy swizzling.
        /// </summary>
        public cVector4 wzzy => new cVector4(w, z, z, y);
        
        /// <summary>
        /// Returns cVector4.abbg swizzling (equivalent to cVector4.wzzy).
        /// </summary>
        public cVector4 abbg => new cVector4(w, z, z, y);
        
        /// <summary>
        /// Returns cVector4.wzzz swizzling.
        /// </summary>
        public cVector4 wzzz => new cVector4(w, z, z, z);
        
        /// <summary>
        /// Returns cVector4.abbb swizzling (equivalent to cVector4.wzzz).
        /// </summary>
        public cVector4 abbb => new cVector4(w, z, z, z);
        
        /// <summary>
        /// Returns cVector4.wzzw swizzling.
        /// </summary>
        public cVector4 wzzw => new cVector4(w, z, z, w);
        
        /// <summary>
        /// Returns cVector4.abba swizzling (equivalent to cVector4.wzzw).
        /// </summary>
        public cVector4 abba => new cVector4(w, z, z, w);
        
        /// <summary>
        /// Returns cVector4.wzw swizzling.
        /// </summary>
        public cVector3 wzw => new cVector3(w, z, w);
        
        /// <summary>
        /// Returns cVector4.aba swizzling (equivalent to cVector4.wzw).
        /// </summary>
        public cVector3 aba => new cVector3(w, z, w);
        
        /// <summary>
        /// Returns cVector4.wzwx swizzling.
        /// </summary>
        public cVector4 wzwx => new cVector4(w, z, w, x);
        
        /// <summary>
        /// Returns cVector4.abar swizzling (equivalent to cVector4.wzwx).
        /// </summary>
        public cVector4 abar => new cVector4(w, z, w, x);
        
        /// <summary>
        /// Returns cVector4.wzwy swizzling.
        /// </summary>
        public cVector4 wzwy => new cVector4(w, z, w, y);
        
        /// <summary>
        /// Returns cVector4.abag swizzling (equivalent to cVector4.wzwy).
        /// </summary>
        public cVector4 abag => new cVector4(w, z, w, y);
        
        /// <summary>
        /// Returns cVector4.wzwz swizzling.
        /// </summary>
        public cVector4 wzwz => new cVector4(w, z, w, z);
        
        /// <summary>
        /// Returns cVector4.abab swizzling (equivalent to cVector4.wzwz).
        /// </summary>
        public cVector4 abab => new cVector4(w, z, w, z);
        
        /// <summary>
        /// Returns cVector4.wzww swizzling.
        /// </summary>
        public cVector4 wzww => new cVector4(w, z, w, w);
        
        /// <summary>
        /// Returns cVector4.abaa swizzling (equivalent to cVector4.wzww).
        /// </summary>
        public cVector4 abaa => new cVector4(w, z, w, w);
        
        /// <summary>
        /// Returns cVector4.ww swizzling.
        /// </summary>
        public cVector2 ww => new cVector2(w, w);
        
        /// <summary>
        /// Returns cVector4.aa swizzling (equivalent to cVector4.ww).
        /// </summary>
        public cVector2 aa => new cVector2(w, w);
        
        /// <summary>
        /// Returns cVector4.wwx swizzling.
        /// </summary>
        public cVector3 wwx => new cVector3(w, w, x);
        
        /// <summary>
        /// Returns cVector4.aar swizzling (equivalent to cVector4.wwx).
        /// </summary>
        public cVector3 aar => new cVector3(w, w, x);
        
        /// <summary>
        /// Returns cVector4.wwxx swizzling.
        /// </summary>
        public cVector4 wwxx => new cVector4(w, w, x, x);
        
        /// <summary>
        /// Returns cVector4.aarr swizzling (equivalent to cVector4.wwxx).
        /// </summary>
        public cVector4 aarr => new cVector4(w, w, x, x);
        
        /// <summary>
        /// Returns cVector4.wwxy swizzling.
        /// </summary>
        public cVector4 wwxy => new cVector4(w, w, x, y);
        
        /// <summary>
        /// Returns cVector4.aarg swizzling (equivalent to cVector4.wwxy).
        /// </summary>
        public cVector4 aarg => new cVector4(w, w, x, y);
        
        /// <summary>
        /// Returns cVector4.wwxz swizzling.
        /// </summary>
        public cVector4 wwxz => new cVector4(w, w, x, z);
        
        /// <summary>
        /// Returns cVector4.aarb swizzling (equivalent to cVector4.wwxz).
        /// </summary>
        public cVector4 aarb => new cVector4(w, w, x, z);
        
        /// <summary>
        /// Returns cVector4.wwxw swizzling.
        /// </summary>
        public cVector4 wwxw => new cVector4(w, w, x, w);
        
        /// <summary>
        /// Returns cVector4.aara swizzling (equivalent to cVector4.wwxw).
        /// </summary>
        public cVector4 aara => new cVector4(w, w, x, w);
        
        /// <summary>
        /// Returns cVector4.wwy swizzling.
        /// </summary>
        public cVector3 wwy => new cVector3(w, w, y);
        
        /// <summary>
        /// Returns cVector4.aag swizzling (equivalent to cVector4.wwy).
        /// </summary>
        public cVector3 aag => new cVector3(w, w, y);
        
        /// <summary>
        /// Returns cVector4.wwyx swizzling.
        /// </summary>
        public cVector4 wwyx => new cVector4(w, w, y, x);
        
        /// <summary>
        /// Returns cVector4.aagr swizzling (equivalent to cVector4.wwyx).
        /// </summary>
        public cVector4 aagr => new cVector4(w, w, y, x);
        
        /// <summary>
        /// Returns cVector4.wwyy swizzling.
        /// </summary>
        public cVector4 wwyy => new cVector4(w, w, y, y);
        
        /// <summary>
        /// Returns cVector4.aagg swizzling (equivalent to cVector4.wwyy).
        /// </summary>
        public cVector4 aagg => new cVector4(w, w, y, y);
        
        /// <summary>
        /// Returns cVector4.wwyz swizzling.
        /// </summary>
        public cVector4 wwyz => new cVector4(w, w, y, z);
        
        /// <summary>
        /// Returns cVector4.aagb swizzling (equivalent to cVector4.wwyz).
        /// </summary>
        public cVector4 aagb => new cVector4(w, w, y, z);
        
        /// <summary>
        /// Returns cVector4.wwyw swizzling.
        /// </summary>
        public cVector4 wwyw => new cVector4(w, w, y, w);
        
        /// <summary>
        /// Returns cVector4.aaga swizzling (equivalent to cVector4.wwyw).
        /// </summary>
        public cVector4 aaga => new cVector4(w, w, y, w);
        
        /// <summary>
        /// Returns cVector4.wwz swizzling.
        /// </summary>
        public cVector3 wwz => new cVector3(w, w, z);
        
        /// <summary>
        /// Returns cVector4.aab swizzling (equivalent to cVector4.wwz).
        /// </summary>
        public cVector3 aab => new cVector3(w, w, z);
        
        /// <summary>
        /// Returns cVector4.wwzx swizzling.
        /// </summary>
        public cVector4 wwzx => new cVector4(w, w, z, x);
        
        /// <summary>
        /// Returns cVector4.aabr swizzling (equivalent to cVector4.wwzx).
        /// </summary>
        public cVector4 aabr => new cVector4(w, w, z, x);
        
        /// <summary>
        /// Returns cVector4.wwzy swizzling.
        /// </summary>
        public cVector4 wwzy => new cVector4(w, w, z, y);
        
        /// <summary>
        /// Returns cVector4.aabg swizzling (equivalent to cVector4.wwzy).
        /// </summary>
        public cVector4 aabg => new cVector4(w, w, z, y);
        
        /// <summary>
        /// Returns cVector4.wwzz swizzling.
        /// </summary>
        public cVector4 wwzz => new cVector4(w, w, z, z);
        
        /// <summary>
        /// Returns cVector4.aabb swizzling (equivalent to cVector4.wwzz).
        /// </summary>
        public cVector4 aabb => new cVector4(w, w, z, z);
        
        /// <summary>
        /// Returns cVector4.wwzw swizzling.
        /// </summary>
        public cVector4 wwzw => new cVector4(w, w, z, w);
        
        /// <summary>
        /// Returns cVector4.aaba swizzling (equivalent to cVector4.wwzw).
        /// </summary>
        public cVector4 aaba => new cVector4(w, w, z, w);
        
        /// <summary>
        /// Returns cVector4.www swizzling.
        /// </summary>
        public cVector3 www => new cVector3(w, w, w);
        
        /// <summary>
        /// Returns cVector4.aaa swizzling (equivalent to cVector4.www).
        /// </summary>
        public cVector3 aaa => new cVector3(w, w, w);
        
        /// <summary>
        /// Returns cVector4.wwwx swizzling.
        /// </summary>
        public cVector4 wwwx => new cVector4(w, w, w, x);
        
        /// <summary>
        /// Returns cVector4.aaar swizzling (equivalent to cVector4.wwwx).
        /// </summary>
        public cVector4 aaar => new cVector4(w, w, w, x);
        
        /// <summary>
        /// Returns cVector4.wwwy swizzling.
        /// </summary>
        public cVector4 wwwy => new cVector4(w, w, w, y);
        
        /// <summary>
        /// Returns cVector4.aaag swizzling (equivalent to cVector4.wwwy).
        /// </summary>
        public cVector4 aaag => new cVector4(w, w, w, y);
        
        /// <summary>
        /// Returns cVector4.wwwz swizzling.
        /// </summary>
        public cVector4 wwwz => new cVector4(w, w, w, z);
        
        /// <summary>
        /// Returns cVector4.aaab swizzling (equivalent to cVector4.wwwz).
        /// </summary>
        public cVector4 aaab => new cVector4(w, w, w, z);
        
        /// <summary>
        /// Returns cVector4.wwww swizzling.
        /// </summary>
        public cVector4 wwww => new cVector4(w, w, w, w);
        
        /// <summary>
        /// Returns cVector4.aaaa swizzling (equivalent to cVector4.wwww).
        /// </summary>
        public cVector4 aaaa => new cVector4(w, w, w, w);

        #endregion

    }
    */
}
