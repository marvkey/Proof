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
    /// Temporary Vector of type decimal with 4 components, used for implementing swizzling for decVector4.
    /// </summary>
    [RegisterCoreClassStruct]
   
    [StructLayout(LayoutKind.Sequential)]
    public struct swizzle_decVector4
    {

        #region Fields
        
        /// <summary>
        /// x-component
        /// </summary>
        
        internal readonly decimal x;
        
        /// <summary>
        /// y-component
        /// </summary>
        
        internal readonly decimal y;
        
        /// <summary>
        /// z-component
        /// </summary>
        
        internal readonly decimal z;
        
        /// <summary>
        /// w-component
        /// </summary>
        
        internal readonly decimal w;

        #endregion


        #region Constructors
        
        /// <summary>
        /// Constructor for swizzle_decVector4.
        /// </summary>
        internal swizzle_decVector4(decimal x, decimal y, decimal z, decimal w)
        {
            this.x = x;
            this.y = y;
            this.z = z;
            this.w = w;
        }

        #endregion


        #region Properties
        
        /// <summary>
        /// Returns decVector4.xx swizzling.
        /// </summary>
        public decVector2 xx => new decVector2(x, x);
        
        /// <summary>
        /// Returns decVector4.rr swizzling (equivalent to decVector4.xx).
        /// </summary>
        public decVector2 rr => new decVector2(x, x);
        
        /// <summary>
        /// Returns decVector4.xxx swizzling.
        /// </summary>
        public decVector3 xxx => new decVector3(x, x, x);
        
        /// <summary>
        /// Returns decVector4.rrr swizzling (equivalent to decVector4.xxx).
        /// </summary>
        public decVector3 rrr => new decVector3(x, x, x);
        
        /// <summary>
        /// Returns decVector4.xxxx swizzling.
        /// </summary>
        public decVector4 xxxx => new decVector4(x, x, x, x);
        
        /// <summary>
        /// Returns decVector4.rrrr swizzling (equivalent to decVector4.xxxx).
        /// </summary>
        public decVector4 rrrr => new decVector4(x, x, x, x);
        
        /// <summary>
        /// Returns decVector4.xxxy swizzling.
        /// </summary>
        public decVector4 xxxy => new decVector4(x, x, x, y);
        
        /// <summary>
        /// Returns decVector4.rrrg swizzling (equivalent to decVector4.xxxy).
        /// </summary>
        public decVector4 rrrg => new decVector4(x, x, x, y);
        
        /// <summary>
        /// Returns decVector4.xxxz swizzling.
        /// </summary>
        public decVector4 xxxz => new decVector4(x, x, x, z);
        
        /// <summary>
        /// Returns decVector4.rrrb swizzling (equivalent to decVector4.xxxz).
        /// </summary>
        public decVector4 rrrb => new decVector4(x, x, x, z);
        
        /// <summary>
        /// Returns decVector4.xxxw swizzling.
        /// </summary>
        public decVector4 xxxw => new decVector4(x, x, x, w);
        
        /// <summary>
        /// Returns decVector4.rrra swizzling (equivalent to decVector4.xxxw).
        /// </summary>
        public decVector4 rrra => new decVector4(x, x, x, w);
        
        /// <summary>
        /// Returns decVector4.xxy swizzling.
        /// </summary>
        public decVector3 xxy => new decVector3(x, x, y);
        
        /// <summary>
        /// Returns decVector4.rrg swizzling (equivalent to decVector4.xxy).
        /// </summary>
        public decVector3 rrg => new decVector3(x, x, y);
        
        /// <summary>
        /// Returns decVector4.xxyx swizzling.
        /// </summary>
        public decVector4 xxyx => new decVector4(x, x, y, x);
        
        /// <summary>
        /// Returns decVector4.rrgr swizzling (equivalent to decVector4.xxyx).
        /// </summary>
        public decVector4 rrgr => new decVector4(x, x, y, x);
        
        /// <summary>
        /// Returns decVector4.xxyy swizzling.
        /// </summary>
        public decVector4 xxyy => new decVector4(x, x, y, y);
        
        /// <summary>
        /// Returns decVector4.rrgg swizzling (equivalent to decVector4.xxyy).
        /// </summary>
        public decVector4 rrgg => new decVector4(x, x, y, y);
        
        /// <summary>
        /// Returns decVector4.xxyz swizzling.
        /// </summary>
        public decVector4 xxyz => new decVector4(x, x, y, z);
        
        /// <summary>
        /// Returns decVector4.rrgb swizzling (equivalent to decVector4.xxyz).
        /// </summary>
        public decVector4 rrgb => new decVector4(x, x, y, z);
        
        /// <summary>
        /// Returns decVector4.xxyw swizzling.
        /// </summary>
        public decVector4 xxyw => new decVector4(x, x, y, w);
        
        /// <summary>
        /// Returns decVector4.rrga swizzling (equivalent to decVector4.xxyw).
        /// </summary>
        public decVector4 rrga => new decVector4(x, x, y, w);
        
        /// <summary>
        /// Returns decVector4.xxz swizzling.
        /// </summary>
        public decVector3 xxz => new decVector3(x, x, z);
        
        /// <summary>
        /// Returns decVector4.rrb swizzling (equivalent to decVector4.xxz).
        /// </summary>
        public decVector3 rrb => new decVector3(x, x, z);
        
        /// <summary>
        /// Returns decVector4.xxzx swizzling.
        /// </summary>
        public decVector4 xxzx => new decVector4(x, x, z, x);
        
        /// <summary>
        /// Returns decVector4.rrbr swizzling (equivalent to decVector4.xxzx).
        /// </summary>
        public decVector4 rrbr => new decVector4(x, x, z, x);
        
        /// <summary>
        /// Returns decVector4.xxzy swizzling.
        /// </summary>
        public decVector4 xxzy => new decVector4(x, x, z, y);
        
        /// <summary>
        /// Returns decVector4.rrbg swizzling (equivalent to decVector4.xxzy).
        /// </summary>
        public decVector4 rrbg => new decVector4(x, x, z, y);
        
        /// <summary>
        /// Returns decVector4.xxzz swizzling.
        /// </summary>
        public decVector4 xxzz => new decVector4(x, x, z, z);
        
        /// <summary>
        /// Returns decVector4.rrbb swizzling (equivalent to decVector4.xxzz).
        /// </summary>
        public decVector4 rrbb => new decVector4(x, x, z, z);
        
        /// <summary>
        /// Returns decVector4.xxzw swizzling.
        /// </summary>
        public decVector4 xxzw => new decVector4(x, x, z, w);
        
        /// <summary>
        /// Returns decVector4.rrba swizzling (equivalent to decVector4.xxzw).
        /// </summary>
        public decVector4 rrba => new decVector4(x, x, z, w);
        
        /// <summary>
        /// Returns decVector4.xxw swizzling.
        /// </summary>
        public decVector3 xxw => new decVector3(x, x, w);
        
        /// <summary>
        /// Returns decVector4.rra swizzling (equivalent to decVector4.xxw).
        /// </summary>
        public decVector3 rra => new decVector3(x, x, w);
        
        /// <summary>
        /// Returns decVector4.xxwx swizzling.
        /// </summary>
        public decVector4 xxwx => new decVector4(x, x, w, x);
        
        /// <summary>
        /// Returns decVector4.rrar swizzling (equivalent to decVector4.xxwx).
        /// </summary>
        public decVector4 rrar => new decVector4(x, x, w, x);
        
        /// <summary>
        /// Returns decVector4.xxwy swizzling.
        /// </summary>
        public decVector4 xxwy => new decVector4(x, x, w, y);
        
        /// <summary>
        /// Returns decVector4.rrag swizzling (equivalent to decVector4.xxwy).
        /// </summary>
        public decVector4 rrag => new decVector4(x, x, w, y);
        
        /// <summary>
        /// Returns decVector4.xxwz swizzling.
        /// </summary>
        public decVector4 xxwz => new decVector4(x, x, w, z);
        
        /// <summary>
        /// Returns decVector4.rrab swizzling (equivalent to decVector4.xxwz).
        /// </summary>
        public decVector4 rrab => new decVector4(x, x, w, z);
        
        /// <summary>
        /// Returns decVector4.xxww swizzling.
        /// </summary>
        public decVector4 xxww => new decVector4(x, x, w, w);
        
        /// <summary>
        /// Returns decVector4.rraa swizzling (equivalent to decVector4.xxww).
        /// </summary>
        public decVector4 rraa => new decVector4(x, x, w, w);
        
        /// <summary>
        /// Returns decVector4.xy swizzling.
        /// </summary>
        public decVector2 xy => new decVector2(x, y);
        
        /// <summary>
        /// Returns decVector4.rg swizzling (equivalent to decVector4.xy).
        /// </summary>
        public decVector2 rg => new decVector2(x, y);
        
        /// <summary>
        /// Returns decVector4.xyx swizzling.
        /// </summary>
        public decVector3 xyx => new decVector3(x, y, x);
        
        /// <summary>
        /// Returns decVector4.rgr swizzling (equivalent to decVector4.xyx).
        /// </summary>
        public decVector3 rgr => new decVector3(x, y, x);
        
        /// <summary>
        /// Returns decVector4.xyxx swizzling.
        /// </summary>
        public decVector4 xyxx => new decVector4(x, y, x, x);
        
        /// <summary>
        /// Returns decVector4.rgrr swizzling (equivalent to decVector4.xyxx).
        /// </summary>
        public decVector4 rgrr => new decVector4(x, y, x, x);
        
        /// <summary>
        /// Returns decVector4.xyxy swizzling.
        /// </summary>
        public decVector4 xyxy => new decVector4(x, y, x, y);
        
        /// <summary>
        /// Returns decVector4.rgrg swizzling (equivalent to decVector4.xyxy).
        /// </summary>
        public decVector4 rgrg => new decVector4(x, y, x, y);
        
        /// <summary>
        /// Returns decVector4.xyxz swizzling.
        /// </summary>
        public decVector4 xyxz => new decVector4(x, y, x, z);
        
        /// <summary>
        /// Returns decVector4.rgrb swizzling (equivalent to decVector4.xyxz).
        /// </summary>
        public decVector4 rgrb => new decVector4(x, y, x, z);
        
        /// <summary>
        /// Returns decVector4.xyxw swizzling.
        /// </summary>
        public decVector4 xyxw => new decVector4(x, y, x, w);
        
        /// <summary>
        /// Returns decVector4.rgra swizzling (equivalent to decVector4.xyxw).
        /// </summary>
        public decVector4 rgra => new decVector4(x, y, x, w);
        
        /// <summary>
        /// Returns decVector4.xyy swizzling.
        /// </summary>
        public decVector3 xyy => new decVector3(x, y, y);
        
        /// <summary>
        /// Returns decVector4.rgg swizzling (equivalent to decVector4.xyy).
        /// </summary>
        public decVector3 rgg => new decVector3(x, y, y);
        
        /// <summary>
        /// Returns decVector4.xyyx swizzling.
        /// </summary>
        public decVector4 xyyx => new decVector4(x, y, y, x);
        
        /// <summary>
        /// Returns decVector4.rggr swizzling (equivalent to decVector4.xyyx).
        /// </summary>
        public decVector4 rggr => new decVector4(x, y, y, x);
        
        /// <summary>
        /// Returns decVector4.xyyy swizzling.
        /// </summary>
        public decVector4 xyyy => new decVector4(x, y, y, y);
        
        /// <summary>
        /// Returns decVector4.rggg swizzling (equivalent to decVector4.xyyy).
        /// </summary>
        public decVector4 rggg => new decVector4(x, y, y, y);
        
        /// <summary>
        /// Returns decVector4.xyyz swizzling.
        /// </summary>
        public decVector4 xyyz => new decVector4(x, y, y, z);
        
        /// <summary>
        /// Returns decVector4.rggb swizzling (equivalent to decVector4.xyyz).
        /// </summary>
        public decVector4 rggb => new decVector4(x, y, y, z);
        
        /// <summary>
        /// Returns decVector4.xyyw swizzling.
        /// </summary>
        public decVector4 xyyw => new decVector4(x, y, y, w);
        
        /// <summary>
        /// Returns decVector4.rgga swizzling (equivalent to decVector4.xyyw).
        /// </summary>
        public decVector4 rgga => new decVector4(x, y, y, w);
        
        /// <summary>
        /// Returns decVector4.xyz swizzling.
        /// </summary>
        public decVector3 xyz => new decVector3(x, y, z);
        
        /// <summary>
        /// Returns decVector4.rgb swizzling (equivalent to decVector4.xyz).
        /// </summary>
        public decVector3 rgb => new decVector3(x, y, z);
        
        /// <summary>
        /// Returns decVector4.xyzx swizzling.
        /// </summary>
        public decVector4 xyzx => new decVector4(x, y, z, x);
        
        /// <summary>
        /// Returns decVector4.rgbr swizzling (equivalent to decVector4.xyzx).
        /// </summary>
        public decVector4 rgbr => new decVector4(x, y, z, x);
        
        /// <summary>
        /// Returns decVector4.xyzy swizzling.
        /// </summary>
        public decVector4 xyzy => new decVector4(x, y, z, y);
        
        /// <summary>
        /// Returns decVector4.rgbg swizzling (equivalent to decVector4.xyzy).
        /// </summary>
        public decVector4 rgbg => new decVector4(x, y, z, y);
        
        /// <summary>
        /// Returns decVector4.xyzz swizzling.
        /// </summary>
        public decVector4 xyzz => new decVector4(x, y, z, z);
        
        /// <summary>
        /// Returns decVector4.rgbb swizzling (equivalent to decVector4.xyzz).
        /// </summary>
        public decVector4 rgbb => new decVector4(x, y, z, z);
        
        /// <summary>
        /// Returns decVector4.xyzw swizzling.
        /// </summary>
        public decVector4 xyzw => new decVector4(x, y, z, w);
        
        /// <summary>
        /// Returns decVector4.rgba swizzling (equivalent to decVector4.xyzw).
        /// </summary>
        public decVector4 rgba => new decVector4(x, y, z, w);
        
        /// <summary>
        /// Returns decVector4.xyw swizzling.
        /// </summary>
        public decVector3 xyw => new decVector3(x, y, w);
        
        /// <summary>
        /// Returns decVector4.rga swizzling (equivalent to decVector4.xyw).
        /// </summary>
        public decVector3 rga => new decVector3(x, y, w);
        
        /// <summary>
        /// Returns decVector4.xywx swizzling.
        /// </summary>
        public decVector4 xywx => new decVector4(x, y, w, x);
        
        /// <summary>
        /// Returns decVector4.rgar swizzling (equivalent to decVector4.xywx).
        /// </summary>
        public decVector4 rgar => new decVector4(x, y, w, x);
        
        /// <summary>
        /// Returns decVector4.xywy swizzling.
        /// </summary>
        public decVector4 xywy => new decVector4(x, y, w, y);
        
        /// <summary>
        /// Returns decVector4.rgag swizzling (equivalent to decVector4.xywy).
        /// </summary>
        public decVector4 rgag => new decVector4(x, y, w, y);
        
        /// <summary>
        /// Returns decVector4.xywz swizzling.
        /// </summary>
        public decVector4 xywz => new decVector4(x, y, w, z);
        
        /// <summary>
        /// Returns decVector4.rgab swizzling (equivalent to decVector4.xywz).
        /// </summary>
        public decVector4 rgab => new decVector4(x, y, w, z);
        
        /// <summary>
        /// Returns decVector4.xyww swizzling.
        /// </summary>
        public decVector4 xyww => new decVector4(x, y, w, w);
        
        /// <summary>
        /// Returns decVector4.rgaa swizzling (equivalent to decVector4.xyww).
        /// </summary>
        public decVector4 rgaa => new decVector4(x, y, w, w);
        
        /// <summary>
        /// Returns decVector4.xz swizzling.
        /// </summary>
        public decVector2 xz => new decVector2(x, z);
        
        /// <summary>
        /// Returns decVector4.rb swizzling (equivalent to decVector4.xz).
        /// </summary>
        public decVector2 rb => new decVector2(x, z);
        
        /// <summary>
        /// Returns decVector4.xzx swizzling.
        /// </summary>
        public decVector3 xzx => new decVector3(x, z, x);
        
        /// <summary>
        /// Returns decVector4.rbr swizzling (equivalent to decVector4.xzx).
        /// </summary>
        public decVector3 rbr => new decVector3(x, z, x);
        
        /// <summary>
        /// Returns decVector4.xzxx swizzling.
        /// </summary>
        public decVector4 xzxx => new decVector4(x, z, x, x);
        
        /// <summary>
        /// Returns decVector4.rbrr swizzling (equivalent to decVector4.xzxx).
        /// </summary>
        public decVector4 rbrr => new decVector4(x, z, x, x);
        
        /// <summary>
        /// Returns decVector4.xzxy swizzling.
        /// </summary>
        public decVector4 xzxy => new decVector4(x, z, x, y);
        
        /// <summary>
        /// Returns decVector4.rbrg swizzling (equivalent to decVector4.xzxy).
        /// </summary>
        public decVector4 rbrg => new decVector4(x, z, x, y);
        
        /// <summary>
        /// Returns decVector4.xzxz swizzling.
        /// </summary>
        public decVector4 xzxz => new decVector4(x, z, x, z);
        
        /// <summary>
        /// Returns decVector4.rbrb swizzling (equivalent to decVector4.xzxz).
        /// </summary>
        public decVector4 rbrb => new decVector4(x, z, x, z);
        
        /// <summary>
        /// Returns decVector4.xzxw swizzling.
        /// </summary>
        public decVector4 xzxw => new decVector4(x, z, x, w);
        
        /// <summary>
        /// Returns decVector4.rbra swizzling (equivalent to decVector4.xzxw).
        /// </summary>
        public decVector4 rbra => new decVector4(x, z, x, w);
        
        /// <summary>
        /// Returns decVector4.xzy swizzling.
        /// </summary>
        public decVector3 xzy => new decVector3(x, z, y);
        
        /// <summary>
        /// Returns decVector4.rbg swizzling (equivalent to decVector4.xzy).
        /// </summary>
        public decVector3 rbg => new decVector3(x, z, y);
        
        /// <summary>
        /// Returns decVector4.xzyx swizzling.
        /// </summary>
        public decVector4 xzyx => new decVector4(x, z, y, x);
        
        /// <summary>
        /// Returns decVector4.rbgr swizzling (equivalent to decVector4.xzyx).
        /// </summary>
        public decVector4 rbgr => new decVector4(x, z, y, x);
        
        /// <summary>
        /// Returns decVector4.xzyy swizzling.
        /// </summary>
        public decVector4 xzyy => new decVector4(x, z, y, y);
        
        /// <summary>
        /// Returns decVector4.rbgg swizzling (equivalent to decVector4.xzyy).
        /// </summary>
        public decVector4 rbgg => new decVector4(x, z, y, y);
        
        /// <summary>
        /// Returns decVector4.xzyz swizzling.
        /// </summary>
        public decVector4 xzyz => new decVector4(x, z, y, z);
        
        /// <summary>
        /// Returns decVector4.rbgb swizzling (equivalent to decVector4.xzyz).
        /// </summary>
        public decVector4 rbgb => new decVector4(x, z, y, z);
        
        /// <summary>
        /// Returns decVector4.xzyw swizzling.
        /// </summary>
        public decVector4 xzyw => new decVector4(x, z, y, w);
        
        /// <summary>
        /// Returns decVector4.rbga swizzling (equivalent to decVector4.xzyw).
        /// </summary>
        public decVector4 rbga => new decVector4(x, z, y, w);
        
        /// <summary>
        /// Returns decVector4.xzz swizzling.
        /// </summary>
        public decVector3 xzz => new decVector3(x, z, z);
        
        /// <summary>
        /// Returns decVector4.rbb swizzling (equivalent to decVector4.xzz).
        /// </summary>
        public decVector3 rbb => new decVector3(x, z, z);
        
        /// <summary>
        /// Returns decVector4.xzzx swizzling.
        /// </summary>
        public decVector4 xzzx => new decVector4(x, z, z, x);
        
        /// <summary>
        /// Returns decVector4.rbbr swizzling (equivalent to decVector4.xzzx).
        /// </summary>
        public decVector4 rbbr => new decVector4(x, z, z, x);
        
        /// <summary>
        /// Returns decVector4.xzzy swizzling.
        /// </summary>
        public decVector4 xzzy => new decVector4(x, z, z, y);
        
        /// <summary>
        /// Returns decVector4.rbbg swizzling (equivalent to decVector4.xzzy).
        /// </summary>
        public decVector4 rbbg => new decVector4(x, z, z, y);
        
        /// <summary>
        /// Returns decVector4.xzzz swizzling.
        /// </summary>
        public decVector4 xzzz => new decVector4(x, z, z, z);
        
        /// <summary>
        /// Returns decVector4.rbbb swizzling (equivalent to decVector4.xzzz).
        /// </summary>
        public decVector4 rbbb => new decVector4(x, z, z, z);
        
        /// <summary>
        /// Returns decVector4.xzzw swizzling.
        /// </summary>
        public decVector4 xzzw => new decVector4(x, z, z, w);
        
        /// <summary>
        /// Returns decVector4.rbba swizzling (equivalent to decVector4.xzzw).
        /// </summary>
        public decVector4 rbba => new decVector4(x, z, z, w);
        
        /// <summary>
        /// Returns decVector4.xzw swizzling.
        /// </summary>
        public decVector3 xzw => new decVector3(x, z, w);
        
        /// <summary>
        /// Returns decVector4.rba swizzling (equivalent to decVector4.xzw).
        /// </summary>
        public decVector3 rba => new decVector3(x, z, w);
        
        /// <summary>
        /// Returns decVector4.xzwx swizzling.
        /// </summary>
        public decVector4 xzwx => new decVector4(x, z, w, x);
        
        /// <summary>
        /// Returns decVector4.rbar swizzling (equivalent to decVector4.xzwx).
        /// </summary>
        public decVector4 rbar => new decVector4(x, z, w, x);
        
        /// <summary>
        /// Returns decVector4.xzwy swizzling.
        /// </summary>
        public decVector4 xzwy => new decVector4(x, z, w, y);
        
        /// <summary>
        /// Returns decVector4.rbag swizzling (equivalent to decVector4.xzwy).
        /// </summary>
        public decVector4 rbag => new decVector4(x, z, w, y);
        
        /// <summary>
        /// Returns decVector4.xzwz swizzling.
        /// </summary>
        public decVector4 xzwz => new decVector4(x, z, w, z);
        
        /// <summary>
        /// Returns decVector4.rbab swizzling (equivalent to decVector4.xzwz).
        /// </summary>
        public decVector4 rbab => new decVector4(x, z, w, z);
        
        /// <summary>
        /// Returns decVector4.xzww swizzling.
        /// </summary>
        public decVector4 xzww => new decVector4(x, z, w, w);
        
        /// <summary>
        /// Returns decVector4.rbaa swizzling (equivalent to decVector4.xzww).
        /// </summary>
        public decVector4 rbaa => new decVector4(x, z, w, w);
        
        /// <summary>
        /// Returns decVector4.xw swizzling.
        /// </summary>
        public decVector2 xw => new decVector2(x, w);
        
        /// <summary>
        /// Returns decVector4.ra swizzling (equivalent to decVector4.xw).
        /// </summary>
        public decVector2 ra => new decVector2(x, w);
        
        /// <summary>
        /// Returns decVector4.xwx swizzling.
        /// </summary>
        public decVector3 xwx => new decVector3(x, w, x);
        
        /// <summary>
        /// Returns decVector4.rar swizzling (equivalent to decVector4.xwx).
        /// </summary>
        public decVector3 rar => new decVector3(x, w, x);
        
        /// <summary>
        /// Returns decVector4.xwxx swizzling.
        /// </summary>
        public decVector4 xwxx => new decVector4(x, w, x, x);
        
        /// <summary>
        /// Returns decVector4.rarr swizzling (equivalent to decVector4.xwxx).
        /// </summary>
        public decVector4 rarr => new decVector4(x, w, x, x);
        
        /// <summary>
        /// Returns decVector4.xwxy swizzling.
        /// </summary>
        public decVector4 xwxy => new decVector4(x, w, x, y);
        
        /// <summary>
        /// Returns decVector4.rarg swizzling (equivalent to decVector4.xwxy).
        /// </summary>
        public decVector4 rarg => new decVector4(x, w, x, y);
        
        /// <summary>
        /// Returns decVector4.xwxz swizzling.
        /// </summary>
        public decVector4 xwxz => new decVector4(x, w, x, z);
        
        /// <summary>
        /// Returns decVector4.rarb swizzling (equivalent to decVector4.xwxz).
        /// </summary>
        public decVector4 rarb => new decVector4(x, w, x, z);
        
        /// <summary>
        /// Returns decVector4.xwxw swizzling.
        /// </summary>
        public decVector4 xwxw => new decVector4(x, w, x, w);
        
        /// <summary>
        /// Returns decVector4.rara swizzling (equivalent to decVector4.xwxw).
        /// </summary>
        public decVector4 rara => new decVector4(x, w, x, w);
        
        /// <summary>
        /// Returns decVector4.xwy swizzling.
        /// </summary>
        public decVector3 xwy => new decVector3(x, w, y);
        
        /// <summary>
        /// Returns decVector4.rag swizzling (equivalent to decVector4.xwy).
        /// </summary>
        public decVector3 rag => new decVector3(x, w, y);
        
        /// <summary>
        /// Returns decVector4.xwyx swizzling.
        /// </summary>
        public decVector4 xwyx => new decVector4(x, w, y, x);
        
        /// <summary>
        /// Returns decVector4.ragr swizzling (equivalent to decVector4.xwyx).
        /// </summary>
        public decVector4 ragr => new decVector4(x, w, y, x);
        
        /// <summary>
        /// Returns decVector4.xwyy swizzling.
        /// </summary>
        public decVector4 xwyy => new decVector4(x, w, y, y);
        
        /// <summary>
        /// Returns decVector4.ragg swizzling (equivalent to decVector4.xwyy).
        /// </summary>
        public decVector4 ragg => new decVector4(x, w, y, y);
        
        /// <summary>
        /// Returns decVector4.xwyz swizzling.
        /// </summary>
        public decVector4 xwyz => new decVector4(x, w, y, z);
        
        /// <summary>
        /// Returns decVector4.ragb swizzling (equivalent to decVector4.xwyz).
        /// </summary>
        public decVector4 ragb => new decVector4(x, w, y, z);
        
        /// <summary>
        /// Returns decVector4.xwyw swizzling.
        /// </summary>
        public decVector4 xwyw => new decVector4(x, w, y, w);
        
        /// <summary>
        /// Returns decVector4.raga swizzling (equivalent to decVector4.xwyw).
        /// </summary>
        public decVector4 raga => new decVector4(x, w, y, w);
        
        /// <summary>
        /// Returns decVector4.xwz swizzling.
        /// </summary>
        public decVector3 xwz => new decVector3(x, w, z);
        
        /// <summary>
        /// Returns decVector4.rab swizzling (equivalent to decVector4.xwz).
        /// </summary>
        public decVector3 rab => new decVector3(x, w, z);
        
        /// <summary>
        /// Returns decVector4.xwzx swizzling.
        /// </summary>
        public decVector4 xwzx => new decVector4(x, w, z, x);
        
        /// <summary>
        /// Returns decVector4.rabr swizzling (equivalent to decVector4.xwzx).
        /// </summary>
        public decVector4 rabr => new decVector4(x, w, z, x);
        
        /// <summary>
        /// Returns decVector4.xwzy swizzling.
        /// </summary>
        public decVector4 xwzy => new decVector4(x, w, z, y);
        
        /// <summary>
        /// Returns decVector4.rabg swizzling (equivalent to decVector4.xwzy).
        /// </summary>
        public decVector4 rabg => new decVector4(x, w, z, y);
        
        /// <summary>
        /// Returns decVector4.xwzz swizzling.
        /// </summary>
        public decVector4 xwzz => new decVector4(x, w, z, z);
        
        /// <summary>
        /// Returns decVector4.rabb swizzling (equivalent to decVector4.xwzz).
        /// </summary>
        public decVector4 rabb => new decVector4(x, w, z, z);
        
        /// <summary>
        /// Returns decVector4.xwzw swizzling.
        /// </summary>
        public decVector4 xwzw => new decVector4(x, w, z, w);
        
        /// <summary>
        /// Returns decVector4.raba swizzling (equivalent to decVector4.xwzw).
        /// </summary>
        public decVector4 raba => new decVector4(x, w, z, w);
        
        /// <summary>
        /// Returns decVector4.xww swizzling.
        /// </summary>
        public decVector3 xww => new decVector3(x, w, w);
        
        /// <summary>
        /// Returns decVector4.raa swizzling (equivalent to decVector4.xww).
        /// </summary>
        public decVector3 raa => new decVector3(x, w, w);
        
        /// <summary>
        /// Returns decVector4.xwwx swizzling.
        /// </summary>
        public decVector4 xwwx => new decVector4(x, w, w, x);
        
        /// <summary>
        /// Returns decVector4.raar swizzling (equivalent to decVector4.xwwx).
        /// </summary>
        public decVector4 raar => new decVector4(x, w, w, x);
        
        /// <summary>
        /// Returns decVector4.xwwy swizzling.
        /// </summary>
        public decVector4 xwwy => new decVector4(x, w, w, y);
        
        /// <summary>
        /// Returns decVector4.raag swizzling (equivalent to decVector4.xwwy).
        /// </summary>
        public decVector4 raag => new decVector4(x, w, w, y);
        
        /// <summary>
        /// Returns decVector4.xwwz swizzling.
        /// </summary>
        public decVector4 xwwz => new decVector4(x, w, w, z);
        
        /// <summary>
        /// Returns decVector4.raab swizzling (equivalent to decVector4.xwwz).
        /// </summary>
        public decVector4 raab => new decVector4(x, w, w, z);
        
        /// <summary>
        /// Returns decVector4.xwww swizzling.
        /// </summary>
        public decVector4 xwww => new decVector4(x, w, w, w);
        
        /// <summary>
        /// Returns decVector4.raaa swizzling (equivalent to decVector4.xwww).
        /// </summary>
        public decVector4 raaa => new decVector4(x, w, w, w);
        
        /// <summary>
        /// Returns decVector4.yx swizzling.
        /// </summary>
        public decVector2 yx => new decVector2(y, x);
        
        /// <summary>
        /// Returns decVector4.gr swizzling (equivalent to decVector4.yx).
        /// </summary>
        public decVector2 gr => new decVector2(y, x);
        
        /// <summary>
        /// Returns decVector4.yxx swizzling.
        /// </summary>
        public decVector3 yxx => new decVector3(y, x, x);
        
        /// <summary>
        /// Returns decVector4.grr swizzling (equivalent to decVector4.yxx).
        /// </summary>
        public decVector3 grr => new decVector3(y, x, x);
        
        /// <summary>
        /// Returns decVector4.yxxx swizzling.
        /// </summary>
        public decVector4 yxxx => new decVector4(y, x, x, x);
        
        /// <summary>
        /// Returns decVector4.grrr swizzling (equivalent to decVector4.yxxx).
        /// </summary>
        public decVector4 grrr => new decVector4(y, x, x, x);
        
        /// <summary>
        /// Returns decVector4.yxxy swizzling.
        /// </summary>
        public decVector4 yxxy => new decVector4(y, x, x, y);
        
        /// <summary>
        /// Returns decVector4.grrg swizzling (equivalent to decVector4.yxxy).
        /// </summary>
        public decVector4 grrg => new decVector4(y, x, x, y);
        
        /// <summary>
        /// Returns decVector4.yxxz swizzling.
        /// </summary>
        public decVector4 yxxz => new decVector4(y, x, x, z);
        
        /// <summary>
        /// Returns decVector4.grrb swizzling (equivalent to decVector4.yxxz).
        /// </summary>
        public decVector4 grrb => new decVector4(y, x, x, z);
        
        /// <summary>
        /// Returns decVector4.yxxw swizzling.
        /// </summary>
        public decVector4 yxxw => new decVector4(y, x, x, w);
        
        /// <summary>
        /// Returns decVector4.grra swizzling (equivalent to decVector4.yxxw).
        /// </summary>
        public decVector4 grra => new decVector4(y, x, x, w);
        
        /// <summary>
        /// Returns decVector4.yxy swizzling.
        /// </summary>
        public decVector3 yxy => new decVector3(y, x, y);
        
        /// <summary>
        /// Returns decVector4.grg swizzling (equivalent to decVector4.yxy).
        /// </summary>
        public decVector3 grg => new decVector3(y, x, y);
        
        /// <summary>
        /// Returns decVector4.yxyx swizzling.
        /// </summary>
        public decVector4 yxyx => new decVector4(y, x, y, x);
        
        /// <summary>
        /// Returns decVector4.grgr swizzling (equivalent to decVector4.yxyx).
        /// </summary>
        public decVector4 grgr => new decVector4(y, x, y, x);
        
        /// <summary>
        /// Returns decVector4.yxyy swizzling.
        /// </summary>
        public decVector4 yxyy => new decVector4(y, x, y, y);
        
        /// <summary>
        /// Returns decVector4.grgg swizzling (equivalent to decVector4.yxyy).
        /// </summary>
        public decVector4 grgg => new decVector4(y, x, y, y);
        
        /// <summary>
        /// Returns decVector4.yxyz swizzling.
        /// </summary>
        public decVector4 yxyz => new decVector4(y, x, y, z);
        
        /// <summary>
        /// Returns decVector4.grgb swizzling (equivalent to decVector4.yxyz).
        /// </summary>
        public decVector4 grgb => new decVector4(y, x, y, z);
        
        /// <summary>
        /// Returns decVector4.yxyw swizzling.
        /// </summary>
        public decVector4 yxyw => new decVector4(y, x, y, w);
        
        /// <summary>
        /// Returns decVector4.grga swizzling (equivalent to decVector4.yxyw).
        /// </summary>
        public decVector4 grga => new decVector4(y, x, y, w);
        
        /// <summary>
        /// Returns decVector4.yxz swizzling.
        /// </summary>
        public decVector3 yxz => new decVector3(y, x, z);
        
        /// <summary>
        /// Returns decVector4.grb swizzling (equivalent to decVector4.yxz).
        /// </summary>
        public decVector3 grb => new decVector3(y, x, z);
        
        /// <summary>
        /// Returns decVector4.yxzx swizzling.
        /// </summary>
        public decVector4 yxzx => new decVector4(y, x, z, x);
        
        /// <summary>
        /// Returns decVector4.grbr swizzling (equivalent to decVector4.yxzx).
        /// </summary>
        public decVector4 grbr => new decVector4(y, x, z, x);
        
        /// <summary>
        /// Returns decVector4.yxzy swizzling.
        /// </summary>
        public decVector4 yxzy => new decVector4(y, x, z, y);
        
        /// <summary>
        /// Returns decVector4.grbg swizzling (equivalent to decVector4.yxzy).
        /// </summary>
        public decVector4 grbg => new decVector4(y, x, z, y);
        
        /// <summary>
        /// Returns decVector4.yxzz swizzling.
        /// </summary>
        public decVector4 yxzz => new decVector4(y, x, z, z);
        
        /// <summary>
        /// Returns decVector4.grbb swizzling (equivalent to decVector4.yxzz).
        /// </summary>
        public decVector4 grbb => new decVector4(y, x, z, z);
        
        /// <summary>
        /// Returns decVector4.yxzw swizzling.
        /// </summary>
        public decVector4 yxzw => new decVector4(y, x, z, w);
        
        /// <summary>
        /// Returns decVector4.grba swizzling (equivalent to decVector4.yxzw).
        /// </summary>
        public decVector4 grba => new decVector4(y, x, z, w);
        
        /// <summary>
        /// Returns decVector4.yxw swizzling.
        /// </summary>
        public decVector3 yxw => new decVector3(y, x, w);
        
        /// <summary>
        /// Returns decVector4.gra swizzling (equivalent to decVector4.yxw).
        /// </summary>
        public decVector3 gra => new decVector3(y, x, w);
        
        /// <summary>
        /// Returns decVector4.yxwx swizzling.
        /// </summary>
        public decVector4 yxwx => new decVector4(y, x, w, x);
        
        /// <summary>
        /// Returns decVector4.grar swizzling (equivalent to decVector4.yxwx).
        /// </summary>
        public decVector4 grar => new decVector4(y, x, w, x);
        
        /// <summary>
        /// Returns decVector4.yxwy swizzling.
        /// </summary>
        public decVector4 yxwy => new decVector4(y, x, w, y);
        
        /// <summary>
        /// Returns decVector4.grag swizzling (equivalent to decVector4.yxwy).
        /// </summary>
        public decVector4 grag => new decVector4(y, x, w, y);
        
        /// <summary>
        /// Returns decVector4.yxwz swizzling.
        /// </summary>
        public decVector4 yxwz => new decVector4(y, x, w, z);
        
        /// <summary>
        /// Returns decVector4.grab swizzling (equivalent to decVector4.yxwz).
        /// </summary>
        public decVector4 grab => new decVector4(y, x, w, z);
        
        /// <summary>
        /// Returns decVector4.yxww swizzling.
        /// </summary>
        public decVector4 yxww => new decVector4(y, x, w, w);
        
        /// <summary>
        /// Returns decVector4.graa swizzling (equivalent to decVector4.yxww).
        /// </summary>
        public decVector4 graa => new decVector4(y, x, w, w);
        
        /// <summary>
        /// Returns decVector4.yy swizzling.
        /// </summary>
        public decVector2 yy => new decVector2(y, y);
        
        /// <summary>
        /// Returns decVector4.gg swizzling (equivalent to decVector4.yy).
        /// </summary>
        public decVector2 gg => new decVector2(y, y);
        
        /// <summary>
        /// Returns decVector4.yyx swizzling.
        /// </summary>
        public decVector3 yyx => new decVector3(y, y, x);
        
        /// <summary>
        /// Returns decVector4.ggr swizzling (equivalent to decVector4.yyx).
        /// </summary>
        public decVector3 ggr => new decVector3(y, y, x);
        
        /// <summary>
        /// Returns decVector4.yyxx swizzling.
        /// </summary>
        public decVector4 yyxx => new decVector4(y, y, x, x);
        
        /// <summary>
        /// Returns decVector4.ggrr swizzling (equivalent to decVector4.yyxx).
        /// </summary>
        public decVector4 ggrr => new decVector4(y, y, x, x);
        
        /// <summary>
        /// Returns decVector4.yyxy swizzling.
        /// </summary>
        public decVector4 yyxy => new decVector4(y, y, x, y);
        
        /// <summary>
        /// Returns decVector4.ggrg swizzling (equivalent to decVector4.yyxy).
        /// </summary>
        public decVector4 ggrg => new decVector4(y, y, x, y);
        
        /// <summary>
        /// Returns decVector4.yyxz swizzling.
        /// </summary>
        public decVector4 yyxz => new decVector4(y, y, x, z);
        
        /// <summary>
        /// Returns decVector4.ggrb swizzling (equivalent to decVector4.yyxz).
        /// </summary>
        public decVector4 ggrb => new decVector4(y, y, x, z);
        
        /// <summary>
        /// Returns decVector4.yyxw swizzling.
        /// </summary>
        public decVector4 yyxw => new decVector4(y, y, x, w);
        
        /// <summary>
        /// Returns decVector4.ggra swizzling (equivalent to decVector4.yyxw).
        /// </summary>
        public decVector4 ggra => new decVector4(y, y, x, w);
        
        /// <summary>
        /// Returns decVector4.yyy swizzling.
        /// </summary>
        public decVector3 yyy => new decVector3(y, y, y);
        
        /// <summary>
        /// Returns decVector4.ggg swizzling (equivalent to decVector4.yyy).
        /// </summary>
        public decVector3 ggg => new decVector3(y, y, y);
        
        /// <summary>
        /// Returns decVector4.yyyx swizzling.
        /// </summary>
        public decVector4 yyyx => new decVector4(y, y, y, x);
        
        /// <summary>
        /// Returns decVector4.gggr swizzling (equivalent to decVector4.yyyx).
        /// </summary>
        public decVector4 gggr => new decVector4(y, y, y, x);
        
        /// <summary>
        /// Returns decVector4.yyyy swizzling.
        /// </summary>
        public decVector4 yyyy => new decVector4(y, y, y, y);
        
        /// <summary>
        /// Returns decVector4.gggg swizzling (equivalent to decVector4.yyyy).
        /// </summary>
        public decVector4 gggg => new decVector4(y, y, y, y);
        
        /// <summary>
        /// Returns decVector4.yyyz swizzling.
        /// </summary>
        public decVector4 yyyz => new decVector4(y, y, y, z);
        
        /// <summary>
        /// Returns decVector4.gggb swizzling (equivalent to decVector4.yyyz).
        /// </summary>
        public decVector4 gggb => new decVector4(y, y, y, z);
        
        /// <summary>
        /// Returns decVector4.yyyw swizzling.
        /// </summary>
        public decVector4 yyyw => new decVector4(y, y, y, w);
        
        /// <summary>
        /// Returns decVector4.ggga swizzling (equivalent to decVector4.yyyw).
        /// </summary>
        public decVector4 ggga => new decVector4(y, y, y, w);
        
        /// <summary>
        /// Returns decVector4.yyz swizzling.
        /// </summary>
        public decVector3 yyz => new decVector3(y, y, z);
        
        /// <summary>
        /// Returns decVector4.ggb swizzling (equivalent to decVector4.yyz).
        /// </summary>
        public decVector3 ggb => new decVector3(y, y, z);
        
        /// <summary>
        /// Returns decVector4.yyzx swizzling.
        /// </summary>
        public decVector4 yyzx => new decVector4(y, y, z, x);
        
        /// <summary>
        /// Returns decVector4.ggbr swizzling (equivalent to decVector4.yyzx).
        /// </summary>
        public decVector4 ggbr => new decVector4(y, y, z, x);
        
        /// <summary>
        /// Returns decVector4.yyzy swizzling.
        /// </summary>
        public decVector4 yyzy => new decVector4(y, y, z, y);
        
        /// <summary>
        /// Returns decVector4.ggbg swizzling (equivalent to decVector4.yyzy).
        /// </summary>
        public decVector4 ggbg => new decVector4(y, y, z, y);
        
        /// <summary>
        /// Returns decVector4.yyzz swizzling.
        /// </summary>
        public decVector4 yyzz => new decVector4(y, y, z, z);
        
        /// <summary>
        /// Returns decVector4.ggbb swizzling (equivalent to decVector4.yyzz).
        /// </summary>
        public decVector4 ggbb => new decVector4(y, y, z, z);
        
        /// <summary>
        /// Returns decVector4.yyzw swizzling.
        /// </summary>
        public decVector4 yyzw => new decVector4(y, y, z, w);
        
        /// <summary>
        /// Returns decVector4.ggba swizzling (equivalent to decVector4.yyzw).
        /// </summary>
        public decVector4 ggba => new decVector4(y, y, z, w);
        
        /// <summary>
        /// Returns decVector4.yyw swizzling.
        /// </summary>
        public decVector3 yyw => new decVector3(y, y, w);
        
        /// <summary>
        /// Returns decVector4.gga swizzling (equivalent to decVector4.yyw).
        /// </summary>
        public decVector3 gga => new decVector3(y, y, w);
        
        /// <summary>
        /// Returns decVector4.yywx swizzling.
        /// </summary>
        public decVector4 yywx => new decVector4(y, y, w, x);
        
        /// <summary>
        /// Returns decVector4.ggar swizzling (equivalent to decVector4.yywx).
        /// </summary>
        public decVector4 ggar => new decVector4(y, y, w, x);
        
        /// <summary>
        /// Returns decVector4.yywy swizzling.
        /// </summary>
        public decVector4 yywy => new decVector4(y, y, w, y);
        
        /// <summary>
        /// Returns decVector4.ggag swizzling (equivalent to decVector4.yywy).
        /// </summary>
        public decVector4 ggag => new decVector4(y, y, w, y);
        
        /// <summary>
        /// Returns decVector4.yywz swizzling.
        /// </summary>
        public decVector4 yywz => new decVector4(y, y, w, z);
        
        /// <summary>
        /// Returns decVector4.ggab swizzling (equivalent to decVector4.yywz).
        /// </summary>
        public decVector4 ggab => new decVector4(y, y, w, z);
        
        /// <summary>
        /// Returns decVector4.yyww swizzling.
        /// </summary>
        public decVector4 yyww => new decVector4(y, y, w, w);
        
        /// <summary>
        /// Returns decVector4.ggaa swizzling (equivalent to decVector4.yyww).
        /// </summary>
        public decVector4 ggaa => new decVector4(y, y, w, w);
        
        /// <summary>
        /// Returns decVector4.yz swizzling.
        /// </summary>
        public decVector2 yz => new decVector2(y, z);
        
        /// <summary>
        /// Returns decVector4.gb swizzling (equivalent to decVector4.yz).
        /// </summary>
        public decVector2 gb => new decVector2(y, z);
        
        /// <summary>
        /// Returns decVector4.yzx swizzling.
        /// </summary>
        public decVector3 yzx => new decVector3(y, z, x);
        
        /// <summary>
        /// Returns decVector4.gbr swizzling (equivalent to decVector4.yzx).
        /// </summary>
        public decVector3 gbr => new decVector3(y, z, x);
        
        /// <summary>
        /// Returns decVector4.yzxx swizzling.
        /// </summary>
        public decVector4 yzxx => new decVector4(y, z, x, x);
        
        /// <summary>
        /// Returns decVector4.gbrr swizzling (equivalent to decVector4.yzxx).
        /// </summary>
        public decVector4 gbrr => new decVector4(y, z, x, x);
        
        /// <summary>
        /// Returns decVector4.yzxy swizzling.
        /// </summary>
        public decVector4 yzxy => new decVector4(y, z, x, y);
        
        /// <summary>
        /// Returns decVector4.gbrg swizzling (equivalent to decVector4.yzxy).
        /// </summary>
        public decVector4 gbrg => new decVector4(y, z, x, y);
        
        /// <summary>
        /// Returns decVector4.yzxz swizzling.
        /// </summary>
        public decVector4 yzxz => new decVector4(y, z, x, z);
        
        /// <summary>
        /// Returns decVector4.gbrb swizzling (equivalent to decVector4.yzxz).
        /// </summary>
        public decVector4 gbrb => new decVector4(y, z, x, z);
        
        /// <summary>
        /// Returns decVector4.yzxw swizzling.
        /// </summary>
        public decVector4 yzxw => new decVector4(y, z, x, w);
        
        /// <summary>
        /// Returns decVector4.gbra swizzling (equivalent to decVector4.yzxw).
        /// </summary>
        public decVector4 gbra => new decVector4(y, z, x, w);
        
        /// <summary>
        /// Returns decVector4.yzy swizzling.
        /// </summary>
        public decVector3 yzy => new decVector3(y, z, y);
        
        /// <summary>
        /// Returns decVector4.gbg swizzling (equivalent to decVector4.yzy).
        /// </summary>
        public decVector3 gbg => new decVector3(y, z, y);
        
        /// <summary>
        /// Returns decVector4.yzyx swizzling.
        /// </summary>
        public decVector4 yzyx => new decVector4(y, z, y, x);
        
        /// <summary>
        /// Returns decVector4.gbgr swizzling (equivalent to decVector4.yzyx).
        /// </summary>
        public decVector4 gbgr => new decVector4(y, z, y, x);
        
        /// <summary>
        /// Returns decVector4.yzyy swizzling.
        /// </summary>
        public decVector4 yzyy => new decVector4(y, z, y, y);
        
        /// <summary>
        /// Returns decVector4.gbgg swizzling (equivalent to decVector4.yzyy).
        /// </summary>
        public decVector4 gbgg => new decVector4(y, z, y, y);
        
        /// <summary>
        /// Returns decVector4.yzyz swizzling.
        /// </summary>
        public decVector4 yzyz => new decVector4(y, z, y, z);
        
        /// <summary>
        /// Returns decVector4.gbgb swizzling (equivalent to decVector4.yzyz).
        /// </summary>
        public decVector4 gbgb => new decVector4(y, z, y, z);
        
        /// <summary>
        /// Returns decVector4.yzyw swizzling.
        /// </summary>
        public decVector4 yzyw => new decVector4(y, z, y, w);
        
        /// <summary>
        /// Returns decVector4.gbga swizzling (equivalent to decVector4.yzyw).
        /// </summary>
        public decVector4 gbga => new decVector4(y, z, y, w);
        
        /// <summary>
        /// Returns decVector4.yzz swizzling.
        /// </summary>
        public decVector3 yzz => new decVector3(y, z, z);
        
        /// <summary>
        /// Returns decVector4.gbb swizzling (equivalent to decVector4.yzz).
        /// </summary>
        public decVector3 gbb => new decVector3(y, z, z);
        
        /// <summary>
        /// Returns decVector4.yzzx swizzling.
        /// </summary>
        public decVector4 yzzx => new decVector4(y, z, z, x);
        
        /// <summary>
        /// Returns decVector4.gbbr swizzling (equivalent to decVector4.yzzx).
        /// </summary>
        public decVector4 gbbr => new decVector4(y, z, z, x);
        
        /// <summary>
        /// Returns decVector4.yzzy swizzling.
        /// </summary>
        public decVector4 yzzy => new decVector4(y, z, z, y);
        
        /// <summary>
        /// Returns decVector4.gbbg swizzling (equivalent to decVector4.yzzy).
        /// </summary>
        public decVector4 gbbg => new decVector4(y, z, z, y);
        
        /// <summary>
        /// Returns decVector4.yzzz swizzling.
        /// </summary>
        public decVector4 yzzz => new decVector4(y, z, z, z);
        
        /// <summary>
        /// Returns decVector4.gbbb swizzling (equivalent to decVector4.yzzz).
        /// </summary>
        public decVector4 gbbb => new decVector4(y, z, z, z);
        
        /// <summary>
        /// Returns decVector4.yzzw swizzling.
        /// </summary>
        public decVector4 yzzw => new decVector4(y, z, z, w);
        
        /// <summary>
        /// Returns decVector4.gbba swizzling (equivalent to decVector4.yzzw).
        /// </summary>
        public decVector4 gbba => new decVector4(y, z, z, w);
        
        /// <summary>
        /// Returns decVector4.yzw swizzling.
        /// </summary>
        public decVector3 yzw => new decVector3(y, z, w);
        
        /// <summary>
        /// Returns decVector4.gba swizzling (equivalent to decVector4.yzw).
        /// </summary>
        public decVector3 gba => new decVector3(y, z, w);
        
        /// <summary>
        /// Returns decVector4.yzwx swizzling.
        /// </summary>
        public decVector4 yzwx => new decVector4(y, z, w, x);
        
        /// <summary>
        /// Returns decVector4.gbar swizzling (equivalent to decVector4.yzwx).
        /// </summary>
        public decVector4 gbar => new decVector4(y, z, w, x);
        
        /// <summary>
        /// Returns decVector4.yzwy swizzling.
        /// </summary>
        public decVector4 yzwy => new decVector4(y, z, w, y);
        
        /// <summary>
        /// Returns decVector4.gbag swizzling (equivalent to decVector4.yzwy).
        /// </summary>
        public decVector4 gbag => new decVector4(y, z, w, y);
        
        /// <summary>
        /// Returns decVector4.yzwz swizzling.
        /// </summary>
        public decVector4 yzwz => new decVector4(y, z, w, z);
        
        /// <summary>
        /// Returns decVector4.gbab swizzling (equivalent to decVector4.yzwz).
        /// </summary>
        public decVector4 gbab => new decVector4(y, z, w, z);
        
        /// <summary>
        /// Returns decVector4.yzww swizzling.
        /// </summary>
        public decVector4 yzww => new decVector4(y, z, w, w);
        
        /// <summary>
        /// Returns decVector4.gbaa swizzling (equivalent to decVector4.yzww).
        /// </summary>
        public decVector4 gbaa => new decVector4(y, z, w, w);
        
        /// <summary>
        /// Returns decVector4.yw swizzling.
        /// </summary>
        public decVector2 yw => new decVector2(y, w);
        
        /// <summary>
        /// Returns decVector4.ga swizzling (equivalent to decVector4.yw).
        /// </summary>
        public decVector2 ga => new decVector2(y, w);
        
        /// <summary>
        /// Returns decVector4.ywx swizzling.
        /// </summary>
        public decVector3 ywx => new decVector3(y, w, x);
        
        /// <summary>
        /// Returns decVector4.gar swizzling (equivalent to decVector4.ywx).
        /// </summary>
        public decVector3 gar => new decVector3(y, w, x);
        
        /// <summary>
        /// Returns decVector4.ywxx swizzling.
        /// </summary>
        public decVector4 ywxx => new decVector4(y, w, x, x);
        
        /// <summary>
        /// Returns decVector4.garr swizzling (equivalent to decVector4.ywxx).
        /// </summary>
        public decVector4 garr => new decVector4(y, w, x, x);
        
        /// <summary>
        /// Returns decVector4.ywxy swizzling.
        /// </summary>
        public decVector4 ywxy => new decVector4(y, w, x, y);
        
        /// <summary>
        /// Returns decVector4.garg swizzling (equivalent to decVector4.ywxy).
        /// </summary>
        public decVector4 garg => new decVector4(y, w, x, y);
        
        /// <summary>
        /// Returns decVector4.ywxz swizzling.
        /// </summary>
        public decVector4 ywxz => new decVector4(y, w, x, z);
        
        /// <summary>
        /// Returns decVector4.garb swizzling (equivalent to decVector4.ywxz).
        /// </summary>
        public decVector4 garb => new decVector4(y, w, x, z);
        
        /// <summary>
        /// Returns decVector4.ywxw swizzling.
        /// </summary>
        public decVector4 ywxw => new decVector4(y, w, x, w);
        
        /// <summary>
        /// Returns decVector4.gara swizzling (equivalent to decVector4.ywxw).
        /// </summary>
        public decVector4 gara => new decVector4(y, w, x, w);
        
        /// <summary>
        /// Returns decVector4.ywy swizzling.
        /// </summary>
        public decVector3 ywy => new decVector3(y, w, y);
        
        /// <summary>
        /// Returns decVector4.gag swizzling (equivalent to decVector4.ywy).
        /// </summary>
        public decVector3 gag => new decVector3(y, w, y);
        
        /// <summary>
        /// Returns decVector4.ywyx swizzling.
        /// </summary>
        public decVector4 ywyx => new decVector4(y, w, y, x);
        
        /// <summary>
        /// Returns decVector4.gagr swizzling (equivalent to decVector4.ywyx).
        /// </summary>
        public decVector4 gagr => new decVector4(y, w, y, x);
        
        /// <summary>
        /// Returns decVector4.ywyy swizzling.
        /// </summary>
        public decVector4 ywyy => new decVector4(y, w, y, y);
        
        /// <summary>
        /// Returns decVector4.gagg swizzling (equivalent to decVector4.ywyy).
        /// </summary>
        public decVector4 gagg => new decVector4(y, w, y, y);
        
        /// <summary>
        /// Returns decVector4.ywyz swizzling.
        /// </summary>
        public decVector4 ywyz => new decVector4(y, w, y, z);
        
        /// <summary>
        /// Returns decVector4.gagb swizzling (equivalent to decVector4.ywyz).
        /// </summary>
        public decVector4 gagb => new decVector4(y, w, y, z);
        
        /// <summary>
        /// Returns decVector4.ywyw swizzling.
        /// </summary>
        public decVector4 ywyw => new decVector4(y, w, y, w);
        
        /// <summary>
        /// Returns decVector4.gaga swizzling (equivalent to decVector4.ywyw).
        /// </summary>
        public decVector4 gaga => new decVector4(y, w, y, w);
        
        /// <summary>
        /// Returns decVector4.ywz swizzling.
        /// </summary>
        public decVector3 ywz => new decVector3(y, w, z);
        
        /// <summary>
        /// Returns decVector4.gab swizzling (equivalent to decVector4.ywz).
        /// </summary>
        public decVector3 gab => new decVector3(y, w, z);
        
        /// <summary>
        /// Returns decVector4.ywzx swizzling.
        /// </summary>
        public decVector4 ywzx => new decVector4(y, w, z, x);
        
        /// <summary>
        /// Returns decVector4.gabr swizzling (equivalent to decVector4.ywzx).
        /// </summary>
        public decVector4 gabr => new decVector4(y, w, z, x);
        
        /// <summary>
        /// Returns decVector4.ywzy swizzling.
        /// </summary>
        public decVector4 ywzy => new decVector4(y, w, z, y);
        
        /// <summary>
        /// Returns decVector4.gabg swizzling (equivalent to decVector4.ywzy).
        /// </summary>
        public decVector4 gabg => new decVector4(y, w, z, y);
        
        /// <summary>
        /// Returns decVector4.ywzz swizzling.
        /// </summary>
        public decVector4 ywzz => new decVector4(y, w, z, z);
        
        /// <summary>
        /// Returns decVector4.gabb swizzling (equivalent to decVector4.ywzz).
        /// </summary>
        public decVector4 gabb => new decVector4(y, w, z, z);
        
        /// <summary>
        /// Returns decVector4.ywzw swizzling.
        /// </summary>
        public decVector4 ywzw => new decVector4(y, w, z, w);
        
        /// <summary>
        /// Returns decVector4.gaba swizzling (equivalent to decVector4.ywzw).
        /// </summary>
        public decVector4 gaba => new decVector4(y, w, z, w);
        
        /// <summary>
        /// Returns decVector4.yww swizzling.
        /// </summary>
        public decVector3 yww => new decVector3(y, w, w);
        
        /// <summary>
        /// Returns decVector4.gaa swizzling (equivalent to decVector4.yww).
        /// </summary>
        public decVector3 gaa => new decVector3(y, w, w);
        
        /// <summary>
        /// Returns decVector4.ywwx swizzling.
        /// </summary>
        public decVector4 ywwx => new decVector4(y, w, w, x);
        
        /// <summary>
        /// Returns decVector4.gaar swizzling (equivalent to decVector4.ywwx).
        /// </summary>
        public decVector4 gaar => new decVector4(y, w, w, x);
        
        /// <summary>
        /// Returns decVector4.ywwy swizzling.
        /// </summary>
        public decVector4 ywwy => new decVector4(y, w, w, y);
        
        /// <summary>
        /// Returns decVector4.gaag swizzling (equivalent to decVector4.ywwy).
        /// </summary>
        public decVector4 gaag => new decVector4(y, w, w, y);
        
        /// <summary>
        /// Returns decVector4.ywwz swizzling.
        /// </summary>
        public decVector4 ywwz => new decVector4(y, w, w, z);
        
        /// <summary>
        /// Returns decVector4.gaab swizzling (equivalent to decVector4.ywwz).
        /// </summary>
        public decVector4 gaab => new decVector4(y, w, w, z);
        
        /// <summary>
        /// Returns decVector4.ywww swizzling.
        /// </summary>
        public decVector4 ywww => new decVector4(y, w, w, w);
        
        /// <summary>
        /// Returns decVector4.gaaa swizzling (equivalent to decVector4.ywww).
        /// </summary>
        public decVector4 gaaa => new decVector4(y, w, w, w);
        
        /// <summary>
        /// Returns decVector4.zx swizzling.
        /// </summary>
        public decVector2 zx => new decVector2(z, x);
        
        /// <summary>
        /// Returns decVector4.br swizzling (equivalent to decVector4.zx).
        /// </summary>
        public decVector2 br => new decVector2(z, x);
        
        /// <summary>
        /// Returns decVector4.zxx swizzling.
        /// </summary>
        public decVector3 zxx => new decVector3(z, x, x);
        
        /// <summary>
        /// Returns decVector4.brr swizzling (equivalent to decVector4.zxx).
        /// </summary>
        public decVector3 brr => new decVector3(z, x, x);
        
        /// <summary>
        /// Returns decVector4.zxxx swizzling.
        /// </summary>
        public decVector4 zxxx => new decVector4(z, x, x, x);
        
        /// <summary>
        /// Returns decVector4.brrr swizzling (equivalent to decVector4.zxxx).
        /// </summary>
        public decVector4 brrr => new decVector4(z, x, x, x);
        
        /// <summary>
        /// Returns decVector4.zxxy swizzling.
        /// </summary>
        public decVector4 zxxy => new decVector4(z, x, x, y);
        
        /// <summary>
        /// Returns decVector4.brrg swizzling (equivalent to decVector4.zxxy).
        /// </summary>
        public decVector4 brrg => new decVector4(z, x, x, y);
        
        /// <summary>
        /// Returns decVector4.zxxz swizzling.
        /// </summary>
        public decVector4 zxxz => new decVector4(z, x, x, z);
        
        /// <summary>
        /// Returns decVector4.brrb swizzling (equivalent to decVector4.zxxz).
        /// </summary>
        public decVector4 brrb => new decVector4(z, x, x, z);
        
        /// <summary>
        /// Returns decVector4.zxxw swizzling.
        /// </summary>
        public decVector4 zxxw => new decVector4(z, x, x, w);
        
        /// <summary>
        /// Returns decVector4.brra swizzling (equivalent to decVector4.zxxw).
        /// </summary>
        public decVector4 brra => new decVector4(z, x, x, w);
        
        /// <summary>
        /// Returns decVector4.zxy swizzling.
        /// </summary>
        public decVector3 zxy => new decVector3(z, x, y);
        
        /// <summary>
        /// Returns decVector4.brg swizzling (equivalent to decVector4.zxy).
        /// </summary>
        public decVector3 brg => new decVector3(z, x, y);
        
        /// <summary>
        /// Returns decVector4.zxyx swizzling.
        /// </summary>
        public decVector4 zxyx => new decVector4(z, x, y, x);
        
        /// <summary>
        /// Returns decVector4.brgr swizzling (equivalent to decVector4.zxyx).
        /// </summary>
        public decVector4 brgr => new decVector4(z, x, y, x);
        
        /// <summary>
        /// Returns decVector4.zxyy swizzling.
        /// </summary>
        public decVector4 zxyy => new decVector4(z, x, y, y);
        
        /// <summary>
        /// Returns decVector4.brgg swizzling (equivalent to decVector4.zxyy).
        /// </summary>
        public decVector4 brgg => new decVector4(z, x, y, y);
        
        /// <summary>
        /// Returns decVector4.zxyz swizzling.
        /// </summary>
        public decVector4 zxyz => new decVector4(z, x, y, z);
        
        /// <summary>
        /// Returns decVector4.brgb swizzling (equivalent to decVector4.zxyz).
        /// </summary>
        public decVector4 brgb => new decVector4(z, x, y, z);
        
        /// <summary>
        /// Returns decVector4.zxyw swizzling.
        /// </summary>
        public decVector4 zxyw => new decVector4(z, x, y, w);
        
        /// <summary>
        /// Returns decVector4.brga swizzling (equivalent to decVector4.zxyw).
        /// </summary>
        public decVector4 brga => new decVector4(z, x, y, w);
        
        /// <summary>
        /// Returns decVector4.zxz swizzling.
        /// </summary>
        public decVector3 zxz => new decVector3(z, x, z);
        
        /// <summary>
        /// Returns decVector4.brb swizzling (equivalent to decVector4.zxz).
        /// </summary>
        public decVector3 brb => new decVector3(z, x, z);
        
        /// <summary>
        /// Returns decVector4.zxzx swizzling.
        /// </summary>
        public decVector4 zxzx => new decVector4(z, x, z, x);
        
        /// <summary>
        /// Returns decVector4.brbr swizzling (equivalent to decVector4.zxzx).
        /// </summary>
        public decVector4 brbr => new decVector4(z, x, z, x);
        
        /// <summary>
        /// Returns decVector4.zxzy swizzling.
        /// </summary>
        public decVector4 zxzy => new decVector4(z, x, z, y);
        
        /// <summary>
        /// Returns decVector4.brbg swizzling (equivalent to decVector4.zxzy).
        /// </summary>
        public decVector4 brbg => new decVector4(z, x, z, y);
        
        /// <summary>
        /// Returns decVector4.zxzz swizzling.
        /// </summary>
        public decVector4 zxzz => new decVector4(z, x, z, z);
        
        /// <summary>
        /// Returns decVector4.brbb swizzling (equivalent to decVector4.zxzz).
        /// </summary>
        public decVector4 brbb => new decVector4(z, x, z, z);
        
        /// <summary>
        /// Returns decVector4.zxzw swizzling.
        /// </summary>
        public decVector4 zxzw => new decVector4(z, x, z, w);
        
        /// <summary>
        /// Returns decVector4.brba swizzling (equivalent to decVector4.zxzw).
        /// </summary>
        public decVector4 brba => new decVector4(z, x, z, w);
        
        /// <summary>
        /// Returns decVector4.zxw swizzling.
        /// </summary>
        public decVector3 zxw => new decVector3(z, x, w);
        
        /// <summary>
        /// Returns decVector4.bra swizzling (equivalent to decVector4.zxw).
        /// </summary>
        public decVector3 bra => new decVector3(z, x, w);
        
        /// <summary>
        /// Returns decVector4.zxwx swizzling.
        /// </summary>
        public decVector4 zxwx => new decVector4(z, x, w, x);
        
        /// <summary>
        /// Returns decVector4.brar swizzling (equivalent to decVector4.zxwx).
        /// </summary>
        public decVector4 brar => new decVector4(z, x, w, x);
        
        /// <summary>
        /// Returns decVector4.zxwy swizzling.
        /// </summary>
        public decVector4 zxwy => new decVector4(z, x, w, y);
        
        /// <summary>
        /// Returns decVector4.brag swizzling (equivalent to decVector4.zxwy).
        /// </summary>
        public decVector4 brag => new decVector4(z, x, w, y);
        
        /// <summary>
        /// Returns decVector4.zxwz swizzling.
        /// </summary>
        public decVector4 zxwz => new decVector4(z, x, w, z);
        
        /// <summary>
        /// Returns decVector4.brab swizzling (equivalent to decVector4.zxwz).
        /// </summary>
        public decVector4 brab => new decVector4(z, x, w, z);
        
        /// <summary>
        /// Returns decVector4.zxww swizzling.
        /// </summary>
        public decVector4 zxww => new decVector4(z, x, w, w);
        
        /// <summary>
        /// Returns decVector4.braa swizzling (equivalent to decVector4.zxww).
        /// </summary>
        public decVector4 braa => new decVector4(z, x, w, w);
        
        /// <summary>
        /// Returns decVector4.zy swizzling.
        /// </summary>
        public decVector2 zy => new decVector2(z, y);
        
        /// <summary>
        /// Returns decVector4.bg swizzling (equivalent to decVector4.zy).
        /// </summary>
        public decVector2 bg => new decVector2(z, y);
        
        /// <summary>
        /// Returns decVector4.zyx swizzling.
        /// </summary>
        public decVector3 zyx => new decVector3(z, y, x);
        
        /// <summary>
        /// Returns decVector4.bgr swizzling (equivalent to decVector4.zyx).
        /// </summary>
        public decVector3 bgr => new decVector3(z, y, x);
        
        /// <summary>
        /// Returns decVector4.zyxx swizzling.
        /// </summary>
        public decVector4 zyxx => new decVector4(z, y, x, x);
        
        /// <summary>
        /// Returns decVector4.bgrr swizzling (equivalent to decVector4.zyxx).
        /// </summary>
        public decVector4 bgrr => new decVector4(z, y, x, x);
        
        /// <summary>
        /// Returns decVector4.zyxy swizzling.
        /// </summary>
        public decVector4 zyxy => new decVector4(z, y, x, y);
        
        /// <summary>
        /// Returns decVector4.bgrg swizzling (equivalent to decVector4.zyxy).
        /// </summary>
        public decVector4 bgrg => new decVector4(z, y, x, y);
        
        /// <summary>
        /// Returns decVector4.zyxz swizzling.
        /// </summary>
        public decVector4 zyxz => new decVector4(z, y, x, z);
        
        /// <summary>
        /// Returns decVector4.bgrb swizzling (equivalent to decVector4.zyxz).
        /// </summary>
        public decVector4 bgrb => new decVector4(z, y, x, z);
        
        /// <summary>
        /// Returns decVector4.zyxw swizzling.
        /// </summary>
        public decVector4 zyxw => new decVector4(z, y, x, w);
        
        /// <summary>
        /// Returns decVector4.bgra swizzling (equivalent to decVector4.zyxw).
        /// </summary>
        public decVector4 bgra => new decVector4(z, y, x, w);
        
        /// <summary>
        /// Returns decVector4.zyy swizzling.
        /// </summary>
        public decVector3 zyy => new decVector3(z, y, y);
        
        /// <summary>
        /// Returns decVector4.bgg swizzling (equivalent to decVector4.zyy).
        /// </summary>
        public decVector3 bgg => new decVector3(z, y, y);
        
        /// <summary>
        /// Returns decVector4.zyyx swizzling.
        /// </summary>
        public decVector4 zyyx => new decVector4(z, y, y, x);
        
        /// <summary>
        /// Returns decVector4.bggr swizzling (equivalent to decVector4.zyyx).
        /// </summary>
        public decVector4 bggr => new decVector4(z, y, y, x);
        
        /// <summary>
        /// Returns decVector4.zyyy swizzling.
        /// </summary>
        public decVector4 zyyy => new decVector4(z, y, y, y);
        
        /// <summary>
        /// Returns decVector4.bggg swizzling (equivalent to decVector4.zyyy).
        /// </summary>
        public decVector4 bggg => new decVector4(z, y, y, y);
        
        /// <summary>
        /// Returns decVector4.zyyz swizzling.
        /// </summary>
        public decVector4 zyyz => new decVector4(z, y, y, z);
        
        /// <summary>
        /// Returns decVector4.bggb swizzling (equivalent to decVector4.zyyz).
        /// </summary>
        public decVector4 bggb => new decVector4(z, y, y, z);
        
        /// <summary>
        /// Returns decVector4.zyyw swizzling.
        /// </summary>
        public decVector4 zyyw => new decVector4(z, y, y, w);
        
        /// <summary>
        /// Returns decVector4.bgga swizzling (equivalent to decVector4.zyyw).
        /// </summary>
        public decVector4 bgga => new decVector4(z, y, y, w);
        
        /// <summary>
        /// Returns decVector4.zyz swizzling.
        /// </summary>
        public decVector3 zyz => new decVector3(z, y, z);
        
        /// <summary>
        /// Returns decVector4.bgb swizzling (equivalent to decVector4.zyz).
        /// </summary>
        public decVector3 bgb => new decVector3(z, y, z);
        
        /// <summary>
        /// Returns decVector4.zyzx swizzling.
        /// </summary>
        public decVector4 zyzx => new decVector4(z, y, z, x);
        
        /// <summary>
        /// Returns decVector4.bgbr swizzling (equivalent to decVector4.zyzx).
        /// </summary>
        public decVector4 bgbr => new decVector4(z, y, z, x);
        
        /// <summary>
        /// Returns decVector4.zyzy swizzling.
        /// </summary>
        public decVector4 zyzy => new decVector4(z, y, z, y);
        
        /// <summary>
        /// Returns decVector4.bgbg swizzling (equivalent to decVector4.zyzy).
        /// </summary>
        public decVector4 bgbg => new decVector4(z, y, z, y);
        
        /// <summary>
        /// Returns decVector4.zyzz swizzling.
        /// </summary>
        public decVector4 zyzz => new decVector4(z, y, z, z);
        
        /// <summary>
        /// Returns decVector4.bgbb swizzling (equivalent to decVector4.zyzz).
        /// </summary>
        public decVector4 bgbb => new decVector4(z, y, z, z);
        
        /// <summary>
        /// Returns decVector4.zyzw swizzling.
        /// </summary>
        public decVector4 zyzw => new decVector4(z, y, z, w);
        
        /// <summary>
        /// Returns decVector4.bgba swizzling (equivalent to decVector4.zyzw).
        /// </summary>
        public decVector4 bgba => new decVector4(z, y, z, w);
        
        /// <summary>
        /// Returns decVector4.zyw swizzling.
        /// </summary>
        public decVector3 zyw => new decVector3(z, y, w);
        
        /// <summary>
        /// Returns decVector4.bga swizzling (equivalent to decVector4.zyw).
        /// </summary>
        public decVector3 bga => new decVector3(z, y, w);
        
        /// <summary>
        /// Returns decVector4.zywx swizzling.
        /// </summary>
        public decVector4 zywx => new decVector4(z, y, w, x);
        
        /// <summary>
        /// Returns decVector4.bgar swizzling (equivalent to decVector4.zywx).
        /// </summary>
        public decVector4 bgar => new decVector4(z, y, w, x);
        
        /// <summary>
        /// Returns decVector4.zywy swizzling.
        /// </summary>
        public decVector4 zywy => new decVector4(z, y, w, y);
        
        /// <summary>
        /// Returns decVector4.bgag swizzling (equivalent to decVector4.zywy).
        /// </summary>
        public decVector4 bgag => new decVector4(z, y, w, y);
        
        /// <summary>
        /// Returns decVector4.zywz swizzling.
        /// </summary>
        public decVector4 zywz => new decVector4(z, y, w, z);
        
        /// <summary>
        /// Returns decVector4.bgab swizzling (equivalent to decVector4.zywz).
        /// </summary>
        public decVector4 bgab => new decVector4(z, y, w, z);
        
        /// <summary>
        /// Returns decVector4.zyww swizzling.
        /// </summary>
        public decVector4 zyww => new decVector4(z, y, w, w);
        
        /// <summary>
        /// Returns decVector4.bgaa swizzling (equivalent to decVector4.zyww).
        /// </summary>
        public decVector4 bgaa => new decVector4(z, y, w, w);
        
        /// <summary>
        /// Returns decVector4.zz swizzling.
        /// </summary>
        public decVector2 zz => new decVector2(z, z);
        
        /// <summary>
        /// Returns decVector4.bb swizzling (equivalent to decVector4.zz).
        /// </summary>
        public decVector2 bb => new decVector2(z, z);
        
        /// <summary>
        /// Returns decVector4.zzx swizzling.
        /// </summary>
        public decVector3 zzx => new decVector3(z, z, x);
        
        /// <summary>
        /// Returns decVector4.bbr swizzling (equivalent to decVector4.zzx).
        /// </summary>
        public decVector3 bbr => new decVector3(z, z, x);
        
        /// <summary>
        /// Returns decVector4.zzxx swizzling.
        /// </summary>
        public decVector4 zzxx => new decVector4(z, z, x, x);
        
        /// <summary>
        /// Returns decVector4.bbrr swizzling (equivalent to decVector4.zzxx).
        /// </summary>
        public decVector4 bbrr => new decVector4(z, z, x, x);
        
        /// <summary>
        /// Returns decVector4.zzxy swizzling.
        /// </summary>
        public decVector4 zzxy => new decVector4(z, z, x, y);
        
        /// <summary>
        /// Returns decVector4.bbrg swizzling (equivalent to decVector4.zzxy).
        /// </summary>
        public decVector4 bbrg => new decVector4(z, z, x, y);
        
        /// <summary>
        /// Returns decVector4.zzxz swizzling.
        /// </summary>
        public decVector4 zzxz => new decVector4(z, z, x, z);
        
        /// <summary>
        /// Returns decVector4.bbrb swizzling (equivalent to decVector4.zzxz).
        /// </summary>
        public decVector4 bbrb => new decVector4(z, z, x, z);
        
        /// <summary>
        /// Returns decVector4.zzxw swizzling.
        /// </summary>
        public decVector4 zzxw => new decVector4(z, z, x, w);
        
        /// <summary>
        /// Returns decVector4.bbra swizzling (equivalent to decVector4.zzxw).
        /// </summary>
        public decVector4 bbra => new decVector4(z, z, x, w);
        
        /// <summary>
        /// Returns decVector4.zzy swizzling.
        /// </summary>
        public decVector3 zzy => new decVector3(z, z, y);
        
        /// <summary>
        /// Returns decVector4.bbg swizzling (equivalent to decVector4.zzy).
        /// </summary>
        public decVector3 bbg => new decVector3(z, z, y);
        
        /// <summary>
        /// Returns decVector4.zzyx swizzling.
        /// </summary>
        public decVector4 zzyx => new decVector4(z, z, y, x);
        
        /// <summary>
        /// Returns decVector4.bbgr swizzling (equivalent to decVector4.zzyx).
        /// </summary>
        public decVector4 bbgr => new decVector4(z, z, y, x);
        
        /// <summary>
        /// Returns decVector4.zzyy swizzling.
        /// </summary>
        public decVector4 zzyy => new decVector4(z, z, y, y);
        
        /// <summary>
        /// Returns decVector4.bbgg swizzling (equivalent to decVector4.zzyy).
        /// </summary>
        public decVector4 bbgg => new decVector4(z, z, y, y);
        
        /// <summary>
        /// Returns decVector4.zzyz swizzling.
        /// </summary>
        public decVector4 zzyz => new decVector4(z, z, y, z);
        
        /// <summary>
        /// Returns decVector4.bbgb swizzling (equivalent to decVector4.zzyz).
        /// </summary>
        public decVector4 bbgb => new decVector4(z, z, y, z);
        
        /// <summary>
        /// Returns decVector4.zzyw swizzling.
        /// </summary>
        public decVector4 zzyw => new decVector4(z, z, y, w);
        
        /// <summary>
        /// Returns decVector4.bbga swizzling (equivalent to decVector4.zzyw).
        /// </summary>
        public decVector4 bbga => new decVector4(z, z, y, w);
        
        /// <summary>
        /// Returns decVector4.zzz swizzling.
        /// </summary>
        public decVector3 zzz => new decVector3(z, z, z);
        
        /// <summary>
        /// Returns decVector4.bbb swizzling (equivalent to decVector4.zzz).
        /// </summary>
        public decVector3 bbb => new decVector3(z, z, z);
        
        /// <summary>
        /// Returns decVector4.zzzx swizzling.
        /// </summary>
        public decVector4 zzzx => new decVector4(z, z, z, x);
        
        /// <summary>
        /// Returns decVector4.bbbr swizzling (equivalent to decVector4.zzzx).
        /// </summary>
        public decVector4 bbbr => new decVector4(z, z, z, x);
        
        /// <summary>
        /// Returns decVector4.zzzy swizzling.
        /// </summary>
        public decVector4 zzzy => new decVector4(z, z, z, y);
        
        /// <summary>
        /// Returns decVector4.bbbg swizzling (equivalent to decVector4.zzzy).
        /// </summary>
        public decVector4 bbbg => new decVector4(z, z, z, y);
        
        /// <summary>
        /// Returns decVector4.zzzz swizzling.
        /// </summary>
        public decVector4 zzzz => new decVector4(z, z, z, z);
        
        /// <summary>
        /// Returns decVector4.bbbb swizzling (equivalent to decVector4.zzzz).
        /// </summary>
        public decVector4 bbbb => new decVector4(z, z, z, z);
        
        /// <summary>
        /// Returns decVector4.zzzw swizzling.
        /// </summary>
        public decVector4 zzzw => new decVector4(z, z, z, w);
        
        /// <summary>
        /// Returns decVector4.bbba swizzling (equivalent to decVector4.zzzw).
        /// </summary>
        public decVector4 bbba => new decVector4(z, z, z, w);
        
        /// <summary>
        /// Returns decVector4.zzw swizzling.
        /// </summary>
        public decVector3 zzw => new decVector3(z, z, w);
        
        /// <summary>
        /// Returns decVector4.bba swizzling (equivalent to decVector4.zzw).
        /// </summary>
        public decVector3 bba => new decVector3(z, z, w);
        
        /// <summary>
        /// Returns decVector4.zzwx swizzling.
        /// </summary>
        public decVector4 zzwx => new decVector4(z, z, w, x);
        
        /// <summary>
        /// Returns decVector4.bbar swizzling (equivalent to decVector4.zzwx).
        /// </summary>
        public decVector4 bbar => new decVector4(z, z, w, x);
        
        /// <summary>
        /// Returns decVector4.zzwy swizzling.
        /// </summary>
        public decVector4 zzwy => new decVector4(z, z, w, y);
        
        /// <summary>
        /// Returns decVector4.bbag swizzling (equivalent to decVector4.zzwy).
        /// </summary>
        public decVector4 bbag => new decVector4(z, z, w, y);
        
        /// <summary>
        /// Returns decVector4.zzwz swizzling.
        /// </summary>
        public decVector4 zzwz => new decVector4(z, z, w, z);
        
        /// <summary>
        /// Returns decVector4.bbab swizzling (equivalent to decVector4.zzwz).
        /// </summary>
        public decVector4 bbab => new decVector4(z, z, w, z);
        
        /// <summary>
        /// Returns decVector4.zzww swizzling.
        /// </summary>
        public decVector4 zzww => new decVector4(z, z, w, w);
        
        /// <summary>
        /// Returns decVector4.bbaa swizzling (equivalent to decVector4.zzww).
        /// </summary>
        public decVector4 bbaa => new decVector4(z, z, w, w);
        
        /// <summary>
        /// Returns decVector4.zw swizzling.
        /// </summary>
        public decVector2 zw => new decVector2(z, w);
        
        /// <summary>
        /// Returns decVector4.ba swizzling (equivalent to decVector4.zw).
        /// </summary>
        public decVector2 ba => new decVector2(z, w);
        
        /// <summary>
        /// Returns decVector4.zwx swizzling.
        /// </summary>
        public decVector3 zwx => new decVector3(z, w, x);
        
        /// <summary>
        /// Returns decVector4.bar swizzling (equivalent to decVector4.zwx).
        /// </summary>
        public decVector3 bar => new decVector3(z, w, x);
        
        /// <summary>
        /// Returns decVector4.zwxx swizzling.
        /// </summary>
        public decVector4 zwxx => new decVector4(z, w, x, x);
        
        /// <summary>
        /// Returns decVector4.barr swizzling (equivalent to decVector4.zwxx).
        /// </summary>
        public decVector4 barr => new decVector4(z, w, x, x);
        
        /// <summary>
        /// Returns decVector4.zwxy swizzling.
        /// </summary>
        public decVector4 zwxy => new decVector4(z, w, x, y);
        
        /// <summary>
        /// Returns decVector4.barg swizzling (equivalent to decVector4.zwxy).
        /// </summary>
        public decVector4 barg => new decVector4(z, w, x, y);
        
        /// <summary>
        /// Returns decVector4.zwxz swizzling.
        /// </summary>
        public decVector4 zwxz => new decVector4(z, w, x, z);
        
        /// <summary>
        /// Returns decVector4.barb swizzling (equivalent to decVector4.zwxz).
        /// </summary>
        public decVector4 barb => new decVector4(z, w, x, z);
        
        /// <summary>
        /// Returns decVector4.zwxw swizzling.
        /// </summary>
        public decVector4 zwxw => new decVector4(z, w, x, w);
        
        /// <summary>
        /// Returns decVector4.bara swizzling (equivalent to decVector4.zwxw).
        /// </summary>
        public decVector4 bara => new decVector4(z, w, x, w);
        
        /// <summary>
        /// Returns decVector4.zwy swizzling.
        /// </summary>
        public decVector3 zwy => new decVector3(z, w, y);
        
        /// <summary>
        /// Returns decVector4.bag swizzling (equivalent to decVector4.zwy).
        /// </summary>
        public decVector3 bag => new decVector3(z, w, y);
        
        /// <summary>
        /// Returns decVector4.zwyx swizzling.
        /// </summary>
        public decVector4 zwyx => new decVector4(z, w, y, x);
        
        /// <summary>
        /// Returns decVector4.bagr swizzling (equivalent to decVector4.zwyx).
        /// </summary>
        public decVector4 bagr => new decVector4(z, w, y, x);
        
        /// <summary>
        /// Returns decVector4.zwyy swizzling.
        /// </summary>
        public decVector4 zwyy => new decVector4(z, w, y, y);
        
        /// <summary>
        /// Returns decVector4.bagg swizzling (equivalent to decVector4.zwyy).
        /// </summary>
        public decVector4 bagg => new decVector4(z, w, y, y);
        
        /// <summary>
        /// Returns decVector4.zwyz swizzling.
        /// </summary>
        public decVector4 zwyz => new decVector4(z, w, y, z);
        
        /// <summary>
        /// Returns decVector4.bagb swizzling (equivalent to decVector4.zwyz).
        /// </summary>
        public decVector4 bagb => new decVector4(z, w, y, z);
        
        /// <summary>
        /// Returns decVector4.zwyw swizzling.
        /// </summary>
        public decVector4 zwyw => new decVector4(z, w, y, w);
        
        /// <summary>
        /// Returns decVector4.baga swizzling (equivalent to decVector4.zwyw).
        /// </summary>
        public decVector4 baga => new decVector4(z, w, y, w);
        
        /// <summary>
        /// Returns decVector4.zwz swizzling.
        /// </summary>
        public decVector3 zwz => new decVector3(z, w, z);
        
        /// <summary>
        /// Returns decVector4.bab swizzling (equivalent to decVector4.zwz).
        /// </summary>
        public decVector3 bab => new decVector3(z, w, z);
        
        /// <summary>
        /// Returns decVector4.zwzx swizzling.
        /// </summary>
        public decVector4 zwzx => new decVector4(z, w, z, x);
        
        /// <summary>
        /// Returns decVector4.babr swizzling (equivalent to decVector4.zwzx).
        /// </summary>
        public decVector4 babr => new decVector4(z, w, z, x);
        
        /// <summary>
        /// Returns decVector4.zwzy swizzling.
        /// </summary>
        public decVector4 zwzy => new decVector4(z, w, z, y);
        
        /// <summary>
        /// Returns decVector4.babg swizzling (equivalent to decVector4.zwzy).
        /// </summary>
        public decVector4 babg => new decVector4(z, w, z, y);
        
        /// <summary>
        /// Returns decVector4.zwzz swizzling.
        /// </summary>
        public decVector4 zwzz => new decVector4(z, w, z, z);
        
        /// <summary>
        /// Returns decVector4.babb swizzling (equivalent to decVector4.zwzz).
        /// </summary>
        public decVector4 babb => new decVector4(z, w, z, z);
        
        /// <summary>
        /// Returns decVector4.zwzw swizzling.
        /// </summary>
        public decVector4 zwzw => new decVector4(z, w, z, w);
        
        /// <summary>
        /// Returns decVector4.baba swizzling (equivalent to decVector4.zwzw).
        /// </summary>
        public decVector4 baba => new decVector4(z, w, z, w);
        
        /// <summary>
        /// Returns decVector4.zww swizzling.
        /// </summary>
        public decVector3 zww => new decVector3(z, w, w);
        
        /// <summary>
        /// Returns decVector4.baa swizzling (equivalent to decVector4.zww).
        /// </summary>
        public decVector3 baa => new decVector3(z, w, w);
        
        /// <summary>
        /// Returns decVector4.zwwx swizzling.
        /// </summary>
        public decVector4 zwwx => new decVector4(z, w, w, x);
        
        /// <summary>
        /// Returns decVector4.baar swizzling (equivalent to decVector4.zwwx).
        /// </summary>
        public decVector4 baar => new decVector4(z, w, w, x);
        
        /// <summary>
        /// Returns decVector4.zwwy swizzling.
        /// </summary>
        public decVector4 zwwy => new decVector4(z, w, w, y);
        
        /// <summary>
        /// Returns decVector4.baag swizzling (equivalent to decVector4.zwwy).
        /// </summary>
        public decVector4 baag => new decVector4(z, w, w, y);
        
        /// <summary>
        /// Returns decVector4.zwwz swizzling.
        /// </summary>
        public decVector4 zwwz => new decVector4(z, w, w, z);
        
        /// <summary>
        /// Returns decVector4.baab swizzling (equivalent to decVector4.zwwz).
        /// </summary>
        public decVector4 baab => new decVector4(z, w, w, z);
        
        /// <summary>
        /// Returns decVector4.zwww swizzling.
        /// </summary>
        public decVector4 zwww => new decVector4(z, w, w, w);
        
        /// <summary>
        /// Returns decVector4.baaa swizzling (equivalent to decVector4.zwww).
        /// </summary>
        public decVector4 baaa => new decVector4(z, w, w, w);
        
        /// <summary>
        /// Returns decVector4.wx swizzling.
        /// </summary>
        public decVector2 wx => new decVector2(w, x);
        
        /// <summary>
        /// Returns decVector4.ar swizzling (equivalent to decVector4.wx).
        /// </summary>
        public decVector2 ar => new decVector2(w, x);
        
        /// <summary>
        /// Returns decVector4.wxx swizzling.
        /// </summary>
        public decVector3 wxx => new decVector3(w, x, x);
        
        /// <summary>
        /// Returns decVector4.arr swizzling (equivalent to decVector4.wxx).
        /// </summary>
        public decVector3 arr => new decVector3(w, x, x);
        
        /// <summary>
        /// Returns decVector4.wxxx swizzling.
        /// </summary>
        public decVector4 wxxx => new decVector4(w, x, x, x);
        
        /// <summary>
        /// Returns decVector4.arrr swizzling (equivalent to decVector4.wxxx).
        /// </summary>
        public decVector4 arrr => new decVector4(w, x, x, x);
        
        /// <summary>
        /// Returns decVector4.wxxy swizzling.
        /// </summary>
        public decVector4 wxxy => new decVector4(w, x, x, y);
        
        /// <summary>
        /// Returns decVector4.arrg swizzling (equivalent to decVector4.wxxy).
        /// </summary>
        public decVector4 arrg => new decVector4(w, x, x, y);
        
        /// <summary>
        /// Returns decVector4.wxxz swizzling.
        /// </summary>
        public decVector4 wxxz => new decVector4(w, x, x, z);
        
        /// <summary>
        /// Returns decVector4.arrb swizzling (equivalent to decVector4.wxxz).
        /// </summary>
        public decVector4 arrb => new decVector4(w, x, x, z);
        
        /// <summary>
        /// Returns decVector4.wxxw swizzling.
        /// </summary>
        public decVector4 wxxw => new decVector4(w, x, x, w);
        
        /// <summary>
        /// Returns decVector4.arra swizzling (equivalent to decVector4.wxxw).
        /// </summary>
        public decVector4 arra => new decVector4(w, x, x, w);
        
        /// <summary>
        /// Returns decVector4.wxy swizzling.
        /// </summary>
        public decVector3 wxy => new decVector3(w, x, y);
        
        /// <summary>
        /// Returns decVector4.arg swizzling (equivalent to decVector4.wxy).
        /// </summary>
        public decVector3 arg => new decVector3(w, x, y);
        
        /// <summary>
        /// Returns decVector4.wxyx swizzling.
        /// </summary>
        public decVector4 wxyx => new decVector4(w, x, y, x);
        
        /// <summary>
        /// Returns decVector4.argr swizzling (equivalent to decVector4.wxyx).
        /// </summary>
        public decVector4 argr => new decVector4(w, x, y, x);
        
        /// <summary>
        /// Returns decVector4.wxyy swizzling.
        /// </summary>
        public decVector4 wxyy => new decVector4(w, x, y, y);
        
        /// <summary>
        /// Returns decVector4.argg swizzling (equivalent to decVector4.wxyy).
        /// </summary>
        public decVector4 argg => new decVector4(w, x, y, y);
        
        /// <summary>
        /// Returns decVector4.wxyz swizzling.
        /// </summary>
        public decVector4 wxyz => new decVector4(w, x, y, z);
        
        /// <summary>
        /// Returns decVector4.argb swizzling (equivalent to decVector4.wxyz).
        /// </summary>
        public decVector4 argb => new decVector4(w, x, y, z);
        
        /// <summary>
        /// Returns decVector4.wxyw swizzling.
        /// </summary>
        public decVector4 wxyw => new decVector4(w, x, y, w);
        
        /// <summary>
        /// Returns decVector4.arga swizzling (equivalent to decVector4.wxyw).
        /// </summary>
        public decVector4 arga => new decVector4(w, x, y, w);
        
        /// <summary>
        /// Returns decVector4.wxz swizzling.
        /// </summary>
        public decVector3 wxz => new decVector3(w, x, z);
        
        /// <summary>
        /// Returns decVector4.arb swizzling (equivalent to decVector4.wxz).
        /// </summary>
        public decVector3 arb => new decVector3(w, x, z);
        
        /// <summary>
        /// Returns decVector4.wxzx swizzling.
        /// </summary>
        public decVector4 wxzx => new decVector4(w, x, z, x);
        
        /// <summary>
        /// Returns decVector4.arbr swizzling (equivalent to decVector4.wxzx).
        /// </summary>
        public decVector4 arbr => new decVector4(w, x, z, x);
        
        /// <summary>
        /// Returns decVector4.wxzy swizzling.
        /// </summary>
        public decVector4 wxzy => new decVector4(w, x, z, y);
        
        /// <summary>
        /// Returns decVector4.arbg swizzling (equivalent to decVector4.wxzy).
        /// </summary>
        public decVector4 arbg => new decVector4(w, x, z, y);
        
        /// <summary>
        /// Returns decVector4.wxzz swizzling.
        /// </summary>
        public decVector4 wxzz => new decVector4(w, x, z, z);
        
        /// <summary>
        /// Returns decVector4.arbb swizzling (equivalent to decVector4.wxzz).
        /// </summary>
        public decVector4 arbb => new decVector4(w, x, z, z);
        
        /// <summary>
        /// Returns decVector4.wxzw swizzling.
        /// </summary>
        public decVector4 wxzw => new decVector4(w, x, z, w);
        
        /// <summary>
        /// Returns decVector4.arba swizzling (equivalent to decVector4.wxzw).
        /// </summary>
        public decVector4 arba => new decVector4(w, x, z, w);
        
        /// <summary>
        /// Returns decVector4.wxw swizzling.
        /// </summary>
        public decVector3 wxw => new decVector3(w, x, w);
        
        /// <summary>
        /// Returns decVector4.ara swizzling (equivalent to decVector4.wxw).
        /// </summary>
        public decVector3 ara => new decVector3(w, x, w);
        
        /// <summary>
        /// Returns decVector4.wxwx swizzling.
        /// </summary>
        public decVector4 wxwx => new decVector4(w, x, w, x);
        
        /// <summary>
        /// Returns decVector4.arar swizzling (equivalent to decVector4.wxwx).
        /// </summary>
        public decVector4 arar => new decVector4(w, x, w, x);
        
        /// <summary>
        /// Returns decVector4.wxwy swizzling.
        /// </summary>
        public decVector4 wxwy => new decVector4(w, x, w, y);
        
        /// <summary>
        /// Returns decVector4.arag swizzling (equivalent to decVector4.wxwy).
        /// </summary>
        public decVector4 arag => new decVector4(w, x, w, y);
        
        /// <summary>
        /// Returns decVector4.wxwz swizzling.
        /// </summary>
        public decVector4 wxwz => new decVector4(w, x, w, z);
        
        /// <summary>
        /// Returns decVector4.arab swizzling (equivalent to decVector4.wxwz).
        /// </summary>
        public decVector4 arab => new decVector4(w, x, w, z);
        
        /// <summary>
        /// Returns decVector4.wxww swizzling.
        /// </summary>
        public decVector4 wxww => new decVector4(w, x, w, w);
        
        /// <summary>
        /// Returns decVector4.araa swizzling (equivalent to decVector4.wxww).
        /// </summary>
        public decVector4 araa => new decVector4(w, x, w, w);
        
        /// <summary>
        /// Returns decVector4.wy swizzling.
        /// </summary>
        public decVector2 wy => new decVector2(w, y);
        
        /// <summary>
        /// Returns decVector4.ag swizzling (equivalent to decVector4.wy).
        /// </summary>
        public decVector2 ag => new decVector2(w, y);
        
        /// <summary>
        /// Returns decVector4.wyx swizzling.
        /// </summary>
        public decVector3 wyx => new decVector3(w, y, x);
        
        /// <summary>
        /// Returns decVector4.agr swizzling (equivalent to decVector4.wyx).
        /// </summary>
        public decVector3 agr => new decVector3(w, y, x);
        
        /// <summary>
        /// Returns decVector4.wyxx swizzling.
        /// </summary>
        public decVector4 wyxx => new decVector4(w, y, x, x);
        
        /// <summary>
        /// Returns decVector4.agrr swizzling (equivalent to decVector4.wyxx).
        /// </summary>
        public decVector4 agrr => new decVector4(w, y, x, x);
        
        /// <summary>
        /// Returns decVector4.wyxy swizzling.
        /// </summary>
        public decVector4 wyxy => new decVector4(w, y, x, y);
        
        /// <summary>
        /// Returns decVector4.agrg swizzling (equivalent to decVector4.wyxy).
        /// </summary>
        public decVector4 agrg => new decVector4(w, y, x, y);
        
        /// <summary>
        /// Returns decVector4.wyxz swizzling.
        /// </summary>
        public decVector4 wyxz => new decVector4(w, y, x, z);
        
        /// <summary>
        /// Returns decVector4.agrb swizzling (equivalent to decVector4.wyxz).
        /// </summary>
        public decVector4 agrb => new decVector4(w, y, x, z);
        
        /// <summary>
        /// Returns decVector4.wyxw swizzling.
        /// </summary>
        public decVector4 wyxw => new decVector4(w, y, x, w);
        
        /// <summary>
        /// Returns decVector4.agra swizzling (equivalent to decVector4.wyxw).
        /// </summary>
        public decVector4 agra => new decVector4(w, y, x, w);
        
        /// <summary>
        /// Returns decVector4.wyy swizzling.
        /// </summary>
        public decVector3 wyy => new decVector3(w, y, y);
        
        /// <summary>
        /// Returns decVector4.agg swizzling (equivalent to decVector4.wyy).
        /// </summary>
        public decVector3 agg => new decVector3(w, y, y);
        
        /// <summary>
        /// Returns decVector4.wyyx swizzling.
        /// </summary>
        public decVector4 wyyx => new decVector4(w, y, y, x);
        
        /// <summary>
        /// Returns decVector4.aggr swizzling (equivalent to decVector4.wyyx).
        /// </summary>
        public decVector4 aggr => new decVector4(w, y, y, x);
        
        /// <summary>
        /// Returns decVector4.wyyy swizzling.
        /// </summary>
        public decVector4 wyyy => new decVector4(w, y, y, y);
        
        /// <summary>
        /// Returns decVector4.aggg swizzling (equivalent to decVector4.wyyy).
        /// </summary>
        public decVector4 aggg => new decVector4(w, y, y, y);
        
        /// <summary>
        /// Returns decVector4.wyyz swizzling.
        /// </summary>
        public decVector4 wyyz => new decVector4(w, y, y, z);
        
        /// <summary>
        /// Returns decVector4.aggb swizzling (equivalent to decVector4.wyyz).
        /// </summary>
        public decVector4 aggb => new decVector4(w, y, y, z);
        
        /// <summary>
        /// Returns decVector4.wyyw swizzling.
        /// </summary>
        public decVector4 wyyw => new decVector4(w, y, y, w);
        
        /// <summary>
        /// Returns decVector4.agga swizzling (equivalent to decVector4.wyyw).
        /// </summary>
        public decVector4 agga => new decVector4(w, y, y, w);
        
        /// <summary>
        /// Returns decVector4.wyz swizzling.
        /// </summary>
        public decVector3 wyz => new decVector3(w, y, z);
        
        /// <summary>
        /// Returns decVector4.agb swizzling (equivalent to decVector4.wyz).
        /// </summary>
        public decVector3 agb => new decVector3(w, y, z);
        
        /// <summary>
        /// Returns decVector4.wyzx swizzling.
        /// </summary>
        public decVector4 wyzx => new decVector4(w, y, z, x);
        
        /// <summary>
        /// Returns decVector4.agbr swizzling (equivalent to decVector4.wyzx).
        /// </summary>
        public decVector4 agbr => new decVector4(w, y, z, x);
        
        /// <summary>
        /// Returns decVector4.wyzy swizzling.
        /// </summary>
        public decVector4 wyzy => new decVector4(w, y, z, y);
        
        /// <summary>
        /// Returns decVector4.agbg swizzling (equivalent to decVector4.wyzy).
        /// </summary>
        public decVector4 agbg => new decVector4(w, y, z, y);
        
        /// <summary>
        /// Returns decVector4.wyzz swizzling.
        /// </summary>
        public decVector4 wyzz => new decVector4(w, y, z, z);
        
        /// <summary>
        /// Returns decVector4.agbb swizzling (equivalent to decVector4.wyzz).
        /// </summary>
        public decVector4 agbb => new decVector4(w, y, z, z);
        
        /// <summary>
        /// Returns decVector4.wyzw swizzling.
        /// </summary>
        public decVector4 wyzw => new decVector4(w, y, z, w);
        
        /// <summary>
        /// Returns decVector4.agba swizzling (equivalent to decVector4.wyzw).
        /// </summary>
        public decVector4 agba => new decVector4(w, y, z, w);
        
        /// <summary>
        /// Returns decVector4.wyw swizzling.
        /// </summary>
        public decVector3 wyw => new decVector3(w, y, w);
        
        /// <summary>
        /// Returns decVector4.aga swizzling (equivalent to decVector4.wyw).
        /// </summary>
        public decVector3 aga => new decVector3(w, y, w);
        
        /// <summary>
        /// Returns decVector4.wywx swizzling.
        /// </summary>
        public decVector4 wywx => new decVector4(w, y, w, x);
        
        /// <summary>
        /// Returns decVector4.agar swizzling (equivalent to decVector4.wywx).
        /// </summary>
        public decVector4 agar => new decVector4(w, y, w, x);
        
        /// <summary>
        /// Returns decVector4.wywy swizzling.
        /// </summary>
        public decVector4 wywy => new decVector4(w, y, w, y);
        
        /// <summary>
        /// Returns decVector4.agag swizzling (equivalent to decVector4.wywy).
        /// </summary>
        public decVector4 agag => new decVector4(w, y, w, y);
        
        /// <summary>
        /// Returns decVector4.wywz swizzling.
        /// </summary>
        public decVector4 wywz => new decVector4(w, y, w, z);
        
        /// <summary>
        /// Returns decVector4.agab swizzling (equivalent to decVector4.wywz).
        /// </summary>
        public decVector4 agab => new decVector4(w, y, w, z);
        
        /// <summary>
        /// Returns decVector4.wyww swizzling.
        /// </summary>
        public decVector4 wyww => new decVector4(w, y, w, w);
        
        /// <summary>
        /// Returns decVector4.agaa swizzling (equivalent to decVector4.wyww).
        /// </summary>
        public decVector4 agaa => new decVector4(w, y, w, w);
        
        /// <summary>
        /// Returns decVector4.wz swizzling.
        /// </summary>
        public decVector2 wz => new decVector2(w, z);
        
        /// <summary>
        /// Returns decVector4.ab swizzling (equivalent to decVector4.wz).
        /// </summary>
        public decVector2 ab => new decVector2(w, z);
        
        /// <summary>
        /// Returns decVector4.wzx swizzling.
        /// </summary>
        public decVector3 wzx => new decVector3(w, z, x);
        
        /// <summary>
        /// Returns decVector4.abr swizzling (equivalent to decVector4.wzx).
        /// </summary>
        public decVector3 abr => new decVector3(w, z, x);
        
        /// <summary>
        /// Returns decVector4.wzxx swizzling.
        /// </summary>
        public decVector4 wzxx => new decVector4(w, z, x, x);
        
        /// <summary>
        /// Returns decVector4.abrr swizzling (equivalent to decVector4.wzxx).
        /// </summary>
        public decVector4 abrr => new decVector4(w, z, x, x);
        
        /// <summary>
        /// Returns decVector4.wzxy swizzling.
        /// </summary>
        public decVector4 wzxy => new decVector4(w, z, x, y);
        
        /// <summary>
        /// Returns decVector4.abrg swizzling (equivalent to decVector4.wzxy).
        /// </summary>
        public decVector4 abrg => new decVector4(w, z, x, y);
        
        /// <summary>
        /// Returns decVector4.wzxz swizzling.
        /// </summary>
        public decVector4 wzxz => new decVector4(w, z, x, z);
        
        /// <summary>
        /// Returns decVector4.abrb swizzling (equivalent to decVector4.wzxz).
        /// </summary>
        public decVector4 abrb => new decVector4(w, z, x, z);
        
        /// <summary>
        /// Returns decVector4.wzxw swizzling.
        /// </summary>
        public decVector4 wzxw => new decVector4(w, z, x, w);
        
        /// <summary>
        /// Returns decVector4.abra swizzling (equivalent to decVector4.wzxw).
        /// </summary>
        public decVector4 abra => new decVector4(w, z, x, w);
        
        /// <summary>
        /// Returns decVector4.wzy swizzling.
        /// </summary>
        public decVector3 wzy => new decVector3(w, z, y);
        
        /// <summary>
        /// Returns decVector4.abg swizzling (equivalent to decVector4.wzy).
        /// </summary>
        public decVector3 abg => new decVector3(w, z, y);
        
        /// <summary>
        /// Returns decVector4.wzyx swizzling.
        /// </summary>
        public decVector4 wzyx => new decVector4(w, z, y, x);
        
        /// <summary>
        /// Returns decVector4.abgr swizzling (equivalent to decVector4.wzyx).
        /// </summary>
        public decVector4 abgr => new decVector4(w, z, y, x);
        
        /// <summary>
        /// Returns decVector4.wzyy swizzling.
        /// </summary>
        public decVector4 wzyy => new decVector4(w, z, y, y);
        
        /// <summary>
        /// Returns decVector4.abgg swizzling (equivalent to decVector4.wzyy).
        /// </summary>
        public decVector4 abgg => new decVector4(w, z, y, y);
        
        /// <summary>
        /// Returns decVector4.wzyz swizzling.
        /// </summary>
        public decVector4 wzyz => new decVector4(w, z, y, z);
        
        /// <summary>
        /// Returns decVector4.abgb swizzling (equivalent to decVector4.wzyz).
        /// </summary>
        public decVector4 abgb => new decVector4(w, z, y, z);
        
        /// <summary>
        /// Returns decVector4.wzyw swizzling.
        /// </summary>
        public decVector4 wzyw => new decVector4(w, z, y, w);
        
        /// <summary>
        /// Returns decVector4.abga swizzling (equivalent to decVector4.wzyw).
        /// </summary>
        public decVector4 abga => new decVector4(w, z, y, w);
        
        /// <summary>
        /// Returns decVector4.wzz swizzling.
        /// </summary>
        public decVector3 wzz => new decVector3(w, z, z);
        
        /// <summary>
        /// Returns decVector4.abb swizzling (equivalent to decVector4.wzz).
        /// </summary>
        public decVector3 abb => new decVector3(w, z, z);
        
        /// <summary>
        /// Returns decVector4.wzzx swizzling.
        /// </summary>
        public decVector4 wzzx => new decVector4(w, z, z, x);
        
        /// <summary>
        /// Returns decVector4.abbr swizzling (equivalent to decVector4.wzzx).
        /// </summary>
        public decVector4 abbr => new decVector4(w, z, z, x);
        
        /// <summary>
        /// Returns decVector4.wzzy swizzling.
        /// </summary>
        public decVector4 wzzy => new decVector4(w, z, z, y);
        
        /// <summary>
        /// Returns decVector4.abbg swizzling (equivalent to decVector4.wzzy).
        /// </summary>
        public decVector4 abbg => new decVector4(w, z, z, y);
        
        /// <summary>
        /// Returns decVector4.wzzz swizzling.
        /// </summary>
        public decVector4 wzzz => new decVector4(w, z, z, z);
        
        /// <summary>
        /// Returns decVector4.abbb swizzling (equivalent to decVector4.wzzz).
        /// </summary>
        public decVector4 abbb => new decVector4(w, z, z, z);
        
        /// <summary>
        /// Returns decVector4.wzzw swizzling.
        /// </summary>
        public decVector4 wzzw => new decVector4(w, z, z, w);
        
        /// <summary>
        /// Returns decVector4.abba swizzling (equivalent to decVector4.wzzw).
        /// </summary>
        public decVector4 abba => new decVector4(w, z, z, w);
        
        /// <summary>
        /// Returns decVector4.wzw swizzling.
        /// </summary>
        public decVector3 wzw => new decVector3(w, z, w);
        
        /// <summary>
        /// Returns decVector4.aba swizzling (equivalent to decVector4.wzw).
        /// </summary>
        public decVector3 aba => new decVector3(w, z, w);
        
        /// <summary>
        /// Returns decVector4.wzwx swizzling.
        /// </summary>
        public decVector4 wzwx => new decVector4(w, z, w, x);
        
        /// <summary>
        /// Returns decVector4.abar swizzling (equivalent to decVector4.wzwx).
        /// </summary>
        public decVector4 abar => new decVector4(w, z, w, x);
        
        /// <summary>
        /// Returns decVector4.wzwy swizzling.
        /// </summary>
        public decVector4 wzwy => new decVector4(w, z, w, y);
        
        /// <summary>
        /// Returns decVector4.abag swizzling (equivalent to decVector4.wzwy).
        /// </summary>
        public decVector4 abag => new decVector4(w, z, w, y);
        
        /// <summary>
        /// Returns decVector4.wzwz swizzling.
        /// </summary>
        public decVector4 wzwz => new decVector4(w, z, w, z);
        
        /// <summary>
        /// Returns decVector4.abab swizzling (equivalent to decVector4.wzwz).
        /// </summary>
        public decVector4 abab => new decVector4(w, z, w, z);
        
        /// <summary>
        /// Returns decVector4.wzww swizzling.
        /// </summary>
        public decVector4 wzww => new decVector4(w, z, w, w);
        
        /// <summary>
        /// Returns decVector4.abaa swizzling (equivalent to decVector4.wzww).
        /// </summary>
        public decVector4 abaa => new decVector4(w, z, w, w);
        
        /// <summary>
        /// Returns decVector4.ww swizzling.
        /// </summary>
        public decVector2 ww => new decVector2(w, w);
        
        /// <summary>
        /// Returns decVector4.aa swizzling (equivalent to decVector4.ww).
        /// </summary>
        public decVector2 aa => new decVector2(w, w);
        
        /// <summary>
        /// Returns decVector4.wwx swizzling.
        /// </summary>
        public decVector3 wwx => new decVector3(w, w, x);
        
        /// <summary>
        /// Returns decVector4.aar swizzling (equivalent to decVector4.wwx).
        /// </summary>
        public decVector3 aar => new decVector3(w, w, x);
        
        /// <summary>
        /// Returns decVector4.wwxx swizzling.
        /// </summary>
        public decVector4 wwxx => new decVector4(w, w, x, x);
        
        /// <summary>
        /// Returns decVector4.aarr swizzling (equivalent to decVector4.wwxx).
        /// </summary>
        public decVector4 aarr => new decVector4(w, w, x, x);
        
        /// <summary>
        /// Returns decVector4.wwxy swizzling.
        /// </summary>
        public decVector4 wwxy => new decVector4(w, w, x, y);
        
        /// <summary>
        /// Returns decVector4.aarg swizzling (equivalent to decVector4.wwxy).
        /// </summary>
        public decVector4 aarg => new decVector4(w, w, x, y);
        
        /// <summary>
        /// Returns decVector4.wwxz swizzling.
        /// </summary>
        public decVector4 wwxz => new decVector4(w, w, x, z);
        
        /// <summary>
        /// Returns decVector4.aarb swizzling (equivalent to decVector4.wwxz).
        /// </summary>
        public decVector4 aarb => new decVector4(w, w, x, z);
        
        /// <summary>
        /// Returns decVector4.wwxw swizzling.
        /// </summary>
        public decVector4 wwxw => new decVector4(w, w, x, w);
        
        /// <summary>
        /// Returns decVector4.aara swizzling (equivalent to decVector4.wwxw).
        /// </summary>
        public decVector4 aara => new decVector4(w, w, x, w);
        
        /// <summary>
        /// Returns decVector4.wwy swizzling.
        /// </summary>
        public decVector3 wwy => new decVector3(w, w, y);
        
        /// <summary>
        /// Returns decVector4.aag swizzling (equivalent to decVector4.wwy).
        /// </summary>
        public decVector3 aag => new decVector3(w, w, y);
        
        /// <summary>
        /// Returns decVector4.wwyx swizzling.
        /// </summary>
        public decVector4 wwyx => new decVector4(w, w, y, x);
        
        /// <summary>
        /// Returns decVector4.aagr swizzling (equivalent to decVector4.wwyx).
        /// </summary>
        public decVector4 aagr => new decVector4(w, w, y, x);
        
        /// <summary>
        /// Returns decVector4.wwyy swizzling.
        /// </summary>
        public decVector4 wwyy => new decVector4(w, w, y, y);
        
        /// <summary>
        /// Returns decVector4.aagg swizzling (equivalent to decVector4.wwyy).
        /// </summary>
        public decVector4 aagg => new decVector4(w, w, y, y);
        
        /// <summary>
        /// Returns decVector4.wwyz swizzling.
        /// </summary>
        public decVector4 wwyz => new decVector4(w, w, y, z);
        
        /// <summary>
        /// Returns decVector4.aagb swizzling (equivalent to decVector4.wwyz).
        /// </summary>
        public decVector4 aagb => new decVector4(w, w, y, z);
        
        /// <summary>
        /// Returns decVector4.wwyw swizzling.
        /// </summary>
        public decVector4 wwyw => new decVector4(w, w, y, w);
        
        /// <summary>
        /// Returns decVector4.aaga swizzling (equivalent to decVector4.wwyw).
        /// </summary>
        public decVector4 aaga => new decVector4(w, w, y, w);
        
        /// <summary>
        /// Returns decVector4.wwz swizzling.
        /// </summary>
        public decVector3 wwz => new decVector3(w, w, z);
        
        /// <summary>
        /// Returns decVector4.aab swizzling (equivalent to decVector4.wwz).
        /// </summary>
        public decVector3 aab => new decVector3(w, w, z);
        
        /// <summary>
        /// Returns decVector4.wwzx swizzling.
        /// </summary>
        public decVector4 wwzx => new decVector4(w, w, z, x);
        
        /// <summary>
        /// Returns decVector4.aabr swizzling (equivalent to decVector4.wwzx).
        /// </summary>
        public decVector4 aabr => new decVector4(w, w, z, x);
        
        /// <summary>
        /// Returns decVector4.wwzy swizzling.
        /// </summary>
        public decVector4 wwzy => new decVector4(w, w, z, y);
        
        /// <summary>
        /// Returns decVector4.aabg swizzling (equivalent to decVector4.wwzy).
        /// </summary>
        public decVector4 aabg => new decVector4(w, w, z, y);
        
        /// <summary>
        /// Returns decVector4.wwzz swizzling.
        /// </summary>
        public decVector4 wwzz => new decVector4(w, w, z, z);
        
        /// <summary>
        /// Returns decVector4.aabb swizzling (equivalent to decVector4.wwzz).
        /// </summary>
        public decVector4 aabb => new decVector4(w, w, z, z);
        
        /// <summary>
        /// Returns decVector4.wwzw swizzling.
        /// </summary>
        public decVector4 wwzw => new decVector4(w, w, z, w);
        
        /// <summary>
        /// Returns decVector4.aaba swizzling (equivalent to decVector4.wwzw).
        /// </summary>
        public decVector4 aaba => new decVector4(w, w, z, w);
        
        /// <summary>
        /// Returns decVector4.www swizzling.
        /// </summary>
        public decVector3 www => new decVector3(w, w, w);
        
        /// <summary>
        /// Returns decVector4.aaa swizzling (equivalent to decVector4.www).
        /// </summary>
        public decVector3 aaa => new decVector3(w, w, w);
        
        /// <summary>
        /// Returns decVector4.wwwx swizzling.
        /// </summary>
        public decVector4 wwwx => new decVector4(w, w, w, x);
        
        /// <summary>
        /// Returns decVector4.aaar swizzling (equivalent to decVector4.wwwx).
        /// </summary>
        public decVector4 aaar => new decVector4(w, w, w, x);
        
        /// <summary>
        /// Returns decVector4.wwwy swizzling.
        /// </summary>
        public decVector4 wwwy => new decVector4(w, w, w, y);
        
        /// <summary>
        /// Returns decVector4.aaag swizzling (equivalent to decVector4.wwwy).
        /// </summary>
        public decVector4 aaag => new decVector4(w, w, w, y);
        
        /// <summary>
        /// Returns decVector4.wwwz swizzling.
        /// </summary>
        public decVector4 wwwz => new decVector4(w, w, w, z);
        
        /// <summary>
        /// Returns decVector4.aaab swizzling (equivalent to decVector4.wwwz).
        /// </summary>
        public decVector4 aaab => new decVector4(w, w, w, z);
        
        /// <summary>
        /// Returns decVector4.wwww swizzling.
        /// </summary>
        public decVector4 wwww => new decVector4(w, w, w, w);
        
        /// <summary>
        /// Returns decVector4.aaaa swizzling (equivalent to decVector4.wwww).
        /// </summary>
        public decVector4 aaaa => new decVector4(w, w, w, w);

        #endregion

    }
}
