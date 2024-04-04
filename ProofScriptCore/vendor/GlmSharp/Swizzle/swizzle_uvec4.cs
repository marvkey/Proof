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
    /// Temporary Vectortor of type uint with 4 components, used for implementing swizzling for uVector4.
    /// </summary>
    [RegisterCoreClassStruct]
   
    [StructLayout(LayoutKind.Sequential)]
    public struct swizzle_uVector4
    {

        #region Fields
        
        /// <summary>
        /// x-component
        /// </summary>
        
        internal readonly uint x;
        
        /// <summary>
        /// y-component
        /// </summary>
        
        internal readonly uint y;
        
        /// <summary>
        /// z-component
        /// </summary>
        
        internal readonly uint z;
        
        /// <summary>
        /// w-component
        /// </summary>
        
        internal readonly uint w;

        #endregion


        #region Constructors
        
        /// <summary>
        /// Constructor for swizzle_uVector4.
        /// </summary>
        internal swizzle_uVector4(uint x, uint y, uint z, uint w)
        {
            this.x = x;
            this.y = y;
            this.z = z;
            this.w = w;
        }

        #endregion


        #region Properties
        
        /// <summary>
        /// Returns uVector4.xx swizzling.
        /// </summary>
        public uVector2 xx => new uVector2(x, x);
        
        /// <summary>
        /// Returns uVector4.rr swizzling (equivalent to uVector4.xx).
        /// </summary>
        public uVector2 rr => new uVector2(x, x);
        
        /// <summary>
        /// Returns uVector4.xxx swizzling.
        /// </summary>
        public uVector3 xxx => new uVector3(x, x, x);
        
        /// <summary>
        /// Returns uVector4.rrr swizzling (equivalent to uVector4.xxx).
        /// </summary>
        public uVector3 rrr => new uVector3(x, x, x);
        
        /// <summary>
        /// Returns uVector4.xxxx swizzling.
        /// </summary>
        public uVector4 xxxx => new uVector4(x, x, x, x);
        
        /// <summary>
        /// Returns uVector4.rrrr swizzling (equivalent to uVector4.xxxx).
        /// </summary>
        public uVector4 rrrr => new uVector4(x, x, x, x);
        
        /// <summary>
        /// Returns uVector4.xxxy swizzling.
        /// </summary>
        public uVector4 xxxy => new uVector4(x, x, x, y);
        
        /// <summary>
        /// Returns uVector4.rrrg swizzling (equivalent to uVector4.xxxy).
        /// </summary>
        public uVector4 rrrg => new uVector4(x, x, x, y);
        
        /// <summary>
        /// Returns uVector4.xxxz swizzling.
        /// </summary>
        public uVector4 xxxz => new uVector4(x, x, x, z);
        
        /// <summary>
        /// Returns uVector4.rrrb swizzling (equivalent to uVector4.xxxz).
        /// </summary>
        public uVector4 rrrb => new uVector4(x, x, x, z);
        
        /// <summary>
        /// Returns uVector4.xxxw swizzling.
        /// </summary>
        public uVector4 xxxw => new uVector4(x, x, x, w);
        
        /// <summary>
        /// Returns uVector4.rrra swizzling (equivalent to uVector4.xxxw).
        /// </summary>
        public uVector4 rrra => new uVector4(x, x, x, w);
        
        /// <summary>
        /// Returns uVector4.xxy swizzling.
        /// </summary>
        public uVector3 xxy => new uVector3(x, x, y);
        
        /// <summary>
        /// Returns uVector4.rrg swizzling (equivalent to uVector4.xxy).
        /// </summary>
        public uVector3 rrg => new uVector3(x, x, y);
        
        /// <summary>
        /// Returns uVector4.xxyx swizzling.
        /// </summary>
        public uVector4 xxyx => new uVector4(x, x, y, x);
        
        /// <summary>
        /// Returns uVector4.rrgr swizzling (equivalent to uVector4.xxyx).
        /// </summary>
        public uVector4 rrgr => new uVector4(x, x, y, x);
        
        /// <summary>
        /// Returns uVector4.xxyy swizzling.
        /// </summary>
        public uVector4 xxyy => new uVector4(x, x, y, y);
        
        /// <summary>
        /// Returns uVector4.rrgg swizzling (equivalent to uVector4.xxyy).
        /// </summary>
        public uVector4 rrgg => new uVector4(x, x, y, y);
        
        /// <summary>
        /// Returns uVector4.xxyz swizzling.
        /// </summary>
        public uVector4 xxyz => new uVector4(x, x, y, z);
        
        /// <summary>
        /// Returns uVector4.rrgb swizzling (equivalent to uVector4.xxyz).
        /// </summary>
        public uVector4 rrgb => new uVector4(x, x, y, z);
        
        /// <summary>
        /// Returns uVector4.xxyw swizzling.
        /// </summary>
        public uVector4 xxyw => new uVector4(x, x, y, w);
        
        /// <summary>
        /// Returns uVector4.rrga swizzling (equivalent to uVector4.xxyw).
        /// </summary>
        public uVector4 rrga => new uVector4(x, x, y, w);
        
        /// <summary>
        /// Returns uVector4.xxz swizzling.
        /// </summary>
        public uVector3 xxz => new uVector3(x, x, z);
        
        /// <summary>
        /// Returns uVector4.rrb swizzling (equivalent to uVector4.xxz).
        /// </summary>
        public uVector3 rrb => new uVector3(x, x, z);
        
        /// <summary>
        /// Returns uVector4.xxzx swizzling.
        /// </summary>
        public uVector4 xxzx => new uVector4(x, x, z, x);
        
        /// <summary>
        /// Returns uVector4.rrbr swizzling (equivalent to uVector4.xxzx).
        /// </summary>
        public uVector4 rrbr => new uVector4(x, x, z, x);
        
        /// <summary>
        /// Returns uVector4.xxzy swizzling.
        /// </summary>
        public uVector4 xxzy => new uVector4(x, x, z, y);
        
        /// <summary>
        /// Returns uVector4.rrbg swizzling (equivalent to uVector4.xxzy).
        /// </summary>
        public uVector4 rrbg => new uVector4(x, x, z, y);
        
        /// <summary>
        /// Returns uVector4.xxzz swizzling.
        /// </summary>
        public uVector4 xxzz => new uVector4(x, x, z, z);
        
        /// <summary>
        /// Returns uVector4.rrbb swizzling (equivalent to uVector4.xxzz).
        /// </summary>
        public uVector4 rrbb => new uVector4(x, x, z, z);
        
        /// <summary>
        /// Returns uVector4.xxzw swizzling.
        /// </summary>
        public uVector4 xxzw => new uVector4(x, x, z, w);
        
        /// <summary>
        /// Returns uVector4.rrba swizzling (equivalent to uVector4.xxzw).
        /// </summary>
        public uVector4 rrba => new uVector4(x, x, z, w);
        
        /// <summary>
        /// Returns uVector4.xxw swizzling.
        /// </summary>
        public uVector3 xxw => new uVector3(x, x, w);
        
        /// <summary>
        /// Returns uVector4.rra swizzling (equivalent to uVector4.xxw).
        /// </summary>
        public uVector3 rra => new uVector3(x, x, w);
        
        /// <summary>
        /// Returns uVector4.xxwx swizzling.
        /// </summary>
        public uVector4 xxwx => new uVector4(x, x, w, x);
        
        /// <summary>
        /// Returns uVector4.rrar swizzling (equivalent to uVector4.xxwx).
        /// </summary>
        public uVector4 rrar => new uVector4(x, x, w, x);
        
        /// <summary>
        /// Returns uVector4.xxwy swizzling.
        /// </summary>
        public uVector4 xxwy => new uVector4(x, x, w, y);
        
        /// <summary>
        /// Returns uVector4.rrag swizzling (equivalent to uVector4.xxwy).
        /// </summary>
        public uVector4 rrag => new uVector4(x, x, w, y);
        
        /// <summary>
        /// Returns uVector4.xxwz swizzling.
        /// </summary>
        public uVector4 xxwz => new uVector4(x, x, w, z);
        
        /// <summary>
        /// Returns uVector4.rrab swizzling (equivalent to uVector4.xxwz).
        /// </summary>
        public uVector4 rrab => new uVector4(x, x, w, z);
        
        /// <summary>
        /// Returns uVector4.xxww swizzling.
        /// </summary>
        public uVector4 xxww => new uVector4(x, x, w, w);
        
        /// <summary>
        /// Returns uVector4.rraa swizzling (equivalent to uVector4.xxww).
        /// </summary>
        public uVector4 rraa => new uVector4(x, x, w, w);
        
        /// <summary>
        /// Returns uVector4.xy swizzling.
        /// </summary>
        public uVector2 xy => new uVector2(x, y);
        
        /// <summary>
        /// Returns uVector4.rg swizzling (equivalent to uVector4.xy).
        /// </summary>
        public uVector2 rg => new uVector2(x, y);
        
        /// <summary>
        /// Returns uVector4.xyx swizzling.
        /// </summary>
        public uVector3 xyx => new uVector3(x, y, x);
        
        /// <summary>
        /// Returns uVector4.rgr swizzling (equivalent to uVector4.xyx).
        /// </summary>
        public uVector3 rgr => new uVector3(x, y, x);
        
        /// <summary>
        /// Returns uVector4.xyxx swizzling.
        /// </summary>
        public uVector4 xyxx => new uVector4(x, y, x, x);
        
        /// <summary>
        /// Returns uVector4.rgrr swizzling (equivalent to uVector4.xyxx).
        /// </summary>
        public uVector4 rgrr => new uVector4(x, y, x, x);
        
        /// <summary>
        /// Returns uVector4.xyxy swizzling.
        /// </summary>
        public uVector4 xyxy => new uVector4(x, y, x, y);
        
        /// <summary>
        /// Returns uVector4.rgrg swizzling (equivalent to uVector4.xyxy).
        /// </summary>
        public uVector4 rgrg => new uVector4(x, y, x, y);
        
        /// <summary>
        /// Returns uVector4.xyxz swizzling.
        /// </summary>
        public uVector4 xyxz => new uVector4(x, y, x, z);
        
        /// <summary>
        /// Returns uVector4.rgrb swizzling (equivalent to uVector4.xyxz).
        /// </summary>
        public uVector4 rgrb => new uVector4(x, y, x, z);
        
        /// <summary>
        /// Returns uVector4.xyxw swizzling.
        /// </summary>
        public uVector4 xyxw => new uVector4(x, y, x, w);
        
        /// <summary>
        /// Returns uVector4.rgra swizzling (equivalent to uVector4.xyxw).
        /// </summary>
        public uVector4 rgra => new uVector4(x, y, x, w);
        
        /// <summary>
        /// Returns uVector4.xyy swizzling.
        /// </summary>
        public uVector3 xyy => new uVector3(x, y, y);
        
        /// <summary>
        /// Returns uVector4.rgg swizzling (equivalent to uVector4.xyy).
        /// </summary>
        public uVector3 rgg => new uVector3(x, y, y);
        
        /// <summary>
        /// Returns uVector4.xyyx swizzling.
        /// </summary>
        public uVector4 xyyx => new uVector4(x, y, y, x);
        
        /// <summary>
        /// Returns uVector4.rggr swizzling (equivalent to uVector4.xyyx).
        /// </summary>
        public uVector4 rggr => new uVector4(x, y, y, x);
        
        /// <summary>
        /// Returns uVector4.xyyy swizzling.
        /// </summary>
        public uVector4 xyyy => new uVector4(x, y, y, y);
        
        /// <summary>
        /// Returns uVector4.rggg swizzling (equivalent to uVector4.xyyy).
        /// </summary>
        public uVector4 rggg => new uVector4(x, y, y, y);
        
        /// <summary>
        /// Returns uVector4.xyyz swizzling.
        /// </summary>
        public uVector4 xyyz => new uVector4(x, y, y, z);
        
        /// <summary>
        /// Returns uVector4.rggb swizzling (equivalent to uVector4.xyyz).
        /// </summary>
        public uVector4 rggb => new uVector4(x, y, y, z);
        
        /// <summary>
        /// Returns uVector4.xyyw swizzling.
        /// </summary>
        public uVector4 xyyw => new uVector4(x, y, y, w);
        
        /// <summary>
        /// Returns uVector4.rgga swizzling (equivalent to uVector4.xyyw).
        /// </summary>
        public uVector4 rgga => new uVector4(x, y, y, w);
        
        /// <summary>
        /// Returns uVector4.xyz swizzling.
        /// </summary>
        public uVector3 xyz => new uVector3(x, y, z);
        
        /// <summary>
        /// Returns uVector4.rgb swizzling (equivalent to uVector4.xyz).
        /// </summary>
        public uVector3 rgb => new uVector3(x, y, z);
        
        /// <summary>
        /// Returns uVector4.xyzx swizzling.
        /// </summary>
        public uVector4 xyzx => new uVector4(x, y, z, x);
        
        /// <summary>
        /// Returns uVector4.rgbr swizzling (equivalent to uVector4.xyzx).
        /// </summary>
        public uVector4 rgbr => new uVector4(x, y, z, x);
        
        /// <summary>
        /// Returns uVector4.xyzy swizzling.
        /// </summary>
        public uVector4 xyzy => new uVector4(x, y, z, y);
        
        /// <summary>
        /// Returns uVector4.rgbg swizzling (equivalent to uVector4.xyzy).
        /// </summary>
        public uVector4 rgbg => new uVector4(x, y, z, y);
        
        /// <summary>
        /// Returns uVector4.xyzz swizzling.
        /// </summary>
        public uVector4 xyzz => new uVector4(x, y, z, z);
        
        /// <summary>
        /// Returns uVector4.rgbb swizzling (equivalent to uVector4.xyzz).
        /// </summary>
        public uVector4 rgbb => new uVector4(x, y, z, z);
        
        /// <summary>
        /// Returns uVector4.xyzw swizzling.
        /// </summary>
        public uVector4 xyzw => new uVector4(x, y, z, w);
        
        /// <summary>
        /// Returns uVector4.rgba swizzling (equivalent to uVector4.xyzw).
        /// </summary>
        public uVector4 rgba => new uVector4(x, y, z, w);
        
        /// <summary>
        /// Returns uVector4.xyw swizzling.
        /// </summary>
        public uVector3 xyw => new uVector3(x, y, w);
        
        /// <summary>
        /// Returns uVector4.rga swizzling (equivalent to uVector4.xyw).
        /// </summary>
        public uVector3 rga => new uVector3(x, y, w);
        
        /// <summary>
        /// Returns uVector4.xywx swizzling.
        /// </summary>
        public uVector4 xywx => new uVector4(x, y, w, x);
        
        /// <summary>
        /// Returns uVector4.rgar swizzling (equivalent to uVector4.xywx).
        /// </summary>
        public uVector4 rgar => new uVector4(x, y, w, x);
        
        /// <summary>
        /// Returns uVector4.xywy swizzling.
        /// </summary>
        public uVector4 xywy => new uVector4(x, y, w, y);
        
        /// <summary>
        /// Returns uVector4.rgag swizzling (equivalent to uVector4.xywy).
        /// </summary>
        public uVector4 rgag => new uVector4(x, y, w, y);
        
        /// <summary>
        /// Returns uVector4.xywz swizzling.
        /// </summary>
        public uVector4 xywz => new uVector4(x, y, w, z);
        
        /// <summary>
        /// Returns uVector4.rgab swizzling (equivalent to uVector4.xywz).
        /// </summary>
        public uVector4 rgab => new uVector4(x, y, w, z);
        
        /// <summary>
        /// Returns uVector4.xyww swizzling.
        /// </summary>
        public uVector4 xyww => new uVector4(x, y, w, w);
        
        /// <summary>
        /// Returns uVector4.rgaa swizzling (equivalent to uVector4.xyww).
        /// </summary>
        public uVector4 rgaa => new uVector4(x, y, w, w);
        
        /// <summary>
        /// Returns uVector4.xz swizzling.
        /// </summary>
        public uVector2 xz => new uVector2(x, z);
        
        /// <summary>
        /// Returns uVector4.rb swizzling (equivalent to uVector4.xz).
        /// </summary>
        public uVector2 rb => new uVector2(x, z);
        
        /// <summary>
        /// Returns uVector4.xzx swizzling.
        /// </summary>
        public uVector3 xzx => new uVector3(x, z, x);
        
        /// <summary>
        /// Returns uVector4.rbr swizzling (equivalent to uVector4.xzx).
        /// </summary>
        public uVector3 rbr => new uVector3(x, z, x);
        
        /// <summary>
        /// Returns uVector4.xzxx swizzling.
        /// </summary>
        public uVector4 xzxx => new uVector4(x, z, x, x);
        
        /// <summary>
        /// Returns uVector4.rbrr swizzling (equivalent to uVector4.xzxx).
        /// </summary>
        public uVector4 rbrr => new uVector4(x, z, x, x);
        
        /// <summary>
        /// Returns uVector4.xzxy swizzling.
        /// </summary>
        public uVector4 xzxy => new uVector4(x, z, x, y);
        
        /// <summary>
        /// Returns uVector4.rbrg swizzling (equivalent to uVector4.xzxy).
        /// </summary>
        public uVector4 rbrg => new uVector4(x, z, x, y);
        
        /// <summary>
        /// Returns uVector4.xzxz swizzling.
        /// </summary>
        public uVector4 xzxz => new uVector4(x, z, x, z);
        
        /// <summary>
        /// Returns uVector4.rbrb swizzling (equivalent to uVector4.xzxz).
        /// </summary>
        public uVector4 rbrb => new uVector4(x, z, x, z);
        
        /// <summary>
        /// Returns uVector4.xzxw swizzling.
        /// </summary>
        public uVector4 xzxw => new uVector4(x, z, x, w);
        
        /// <summary>
        /// Returns uVector4.rbra swizzling (equivalent to uVector4.xzxw).
        /// </summary>
        public uVector4 rbra => new uVector4(x, z, x, w);
        
        /// <summary>
        /// Returns uVector4.xzy swizzling.
        /// </summary>
        public uVector3 xzy => new uVector3(x, z, y);
        
        /// <summary>
        /// Returns uVector4.rbg swizzling (equivalent to uVector4.xzy).
        /// </summary>
        public uVector3 rbg => new uVector3(x, z, y);
        
        /// <summary>
        /// Returns uVector4.xzyx swizzling.
        /// </summary>
        public uVector4 xzyx => new uVector4(x, z, y, x);
        
        /// <summary>
        /// Returns uVector4.rbgr swizzling (equivalent to uVector4.xzyx).
        /// </summary>
        public uVector4 rbgr => new uVector4(x, z, y, x);
        
        /// <summary>
        /// Returns uVector4.xzyy swizzling.
        /// </summary>
        public uVector4 xzyy => new uVector4(x, z, y, y);
        
        /// <summary>
        /// Returns uVector4.rbgg swizzling (equivalent to uVector4.xzyy).
        /// </summary>
        public uVector4 rbgg => new uVector4(x, z, y, y);
        
        /// <summary>
        /// Returns uVector4.xzyz swizzling.
        /// </summary>
        public uVector4 xzyz => new uVector4(x, z, y, z);
        
        /// <summary>
        /// Returns uVector4.rbgb swizzling (equivalent to uVector4.xzyz).
        /// </summary>
        public uVector4 rbgb => new uVector4(x, z, y, z);
        
        /// <summary>
        /// Returns uVector4.xzyw swizzling.
        /// </summary>
        public uVector4 xzyw => new uVector4(x, z, y, w);
        
        /// <summary>
        /// Returns uVector4.rbga swizzling (equivalent to uVector4.xzyw).
        /// </summary>
        public uVector4 rbga => new uVector4(x, z, y, w);
        
        /// <summary>
        /// Returns uVector4.xzz swizzling.
        /// </summary>
        public uVector3 xzz => new uVector3(x, z, z);
        
        /// <summary>
        /// Returns uVector4.rbb swizzling (equivalent to uVector4.xzz).
        /// </summary>
        public uVector3 rbb => new uVector3(x, z, z);
        
        /// <summary>
        /// Returns uVector4.xzzx swizzling.
        /// </summary>
        public uVector4 xzzx => new uVector4(x, z, z, x);
        
        /// <summary>
        /// Returns uVector4.rbbr swizzling (equivalent to uVector4.xzzx).
        /// </summary>
        public uVector4 rbbr => new uVector4(x, z, z, x);
        
        /// <summary>
        /// Returns uVector4.xzzy swizzling.
        /// </summary>
        public uVector4 xzzy => new uVector4(x, z, z, y);
        
        /// <summary>
        /// Returns uVector4.rbbg swizzling (equivalent to uVector4.xzzy).
        /// </summary>
        public uVector4 rbbg => new uVector4(x, z, z, y);
        
        /// <summary>
        /// Returns uVector4.xzzz swizzling.
        /// </summary>
        public uVector4 xzzz => new uVector4(x, z, z, z);
        
        /// <summary>
        /// Returns uVector4.rbbb swizzling (equivalent to uVector4.xzzz).
        /// </summary>
        public uVector4 rbbb => new uVector4(x, z, z, z);
        
        /// <summary>
        /// Returns uVector4.xzzw swizzling.
        /// </summary>
        public uVector4 xzzw => new uVector4(x, z, z, w);
        
        /// <summary>
        /// Returns uVector4.rbba swizzling (equivalent to uVector4.xzzw).
        /// </summary>
        public uVector4 rbba => new uVector4(x, z, z, w);
        
        /// <summary>
        /// Returns uVector4.xzw swizzling.
        /// </summary>
        public uVector3 xzw => new uVector3(x, z, w);
        
        /// <summary>
        /// Returns uVector4.rba swizzling (equivalent to uVector4.xzw).
        /// </summary>
        public uVector3 rba => new uVector3(x, z, w);
        
        /// <summary>
        /// Returns uVector4.xzwx swizzling.
        /// </summary>
        public uVector4 xzwx => new uVector4(x, z, w, x);
        
        /// <summary>
        /// Returns uVector4.rbar swizzling (equivalent to uVector4.xzwx).
        /// </summary>
        public uVector4 rbar => new uVector4(x, z, w, x);
        
        /// <summary>
        /// Returns uVector4.xzwy swizzling.
        /// </summary>
        public uVector4 xzwy => new uVector4(x, z, w, y);
        
        /// <summary>
        /// Returns uVector4.rbag swizzling (equivalent to uVector4.xzwy).
        /// </summary>
        public uVector4 rbag => new uVector4(x, z, w, y);
        
        /// <summary>
        /// Returns uVector4.xzwz swizzling.
        /// </summary>
        public uVector4 xzwz => new uVector4(x, z, w, z);
        
        /// <summary>
        /// Returns uVector4.rbab swizzling (equivalent to uVector4.xzwz).
        /// </summary>
        public uVector4 rbab => new uVector4(x, z, w, z);
        
        /// <summary>
        /// Returns uVector4.xzww swizzling.
        /// </summary>
        public uVector4 xzww => new uVector4(x, z, w, w);
        
        /// <summary>
        /// Returns uVector4.rbaa swizzling (equivalent to uVector4.xzww).
        /// </summary>
        public uVector4 rbaa => new uVector4(x, z, w, w);
        
        /// <summary>
        /// Returns uVector4.xw swizzling.
        /// </summary>
        public uVector2 xw => new uVector2(x, w);
        
        /// <summary>
        /// Returns uVector4.ra swizzling (equivalent to uVector4.xw).
        /// </summary>
        public uVector2 ra => new uVector2(x, w);
        
        /// <summary>
        /// Returns uVector4.xwx swizzling.
        /// </summary>
        public uVector3 xwx => new uVector3(x, w, x);
        
        /// <summary>
        /// Returns uVector4.rar swizzling (equivalent to uVector4.xwx).
        /// </summary>
        public uVector3 rar => new uVector3(x, w, x);
        
        /// <summary>
        /// Returns uVector4.xwxx swizzling.
        /// </summary>
        public uVector4 xwxx => new uVector4(x, w, x, x);
        
        /// <summary>
        /// Returns uVector4.rarr swizzling (equivalent to uVector4.xwxx).
        /// </summary>
        public uVector4 rarr => new uVector4(x, w, x, x);
        
        /// <summary>
        /// Returns uVector4.xwxy swizzling.
        /// </summary>
        public uVector4 xwxy => new uVector4(x, w, x, y);
        
        /// <summary>
        /// Returns uVector4.rarg swizzling (equivalent to uVector4.xwxy).
        /// </summary>
        public uVector4 rarg => new uVector4(x, w, x, y);
        
        /// <summary>
        /// Returns uVector4.xwxz swizzling.
        /// </summary>
        public uVector4 xwxz => new uVector4(x, w, x, z);
        
        /// <summary>
        /// Returns uVector4.rarb swizzling (equivalent to uVector4.xwxz).
        /// </summary>
        public uVector4 rarb => new uVector4(x, w, x, z);
        
        /// <summary>
        /// Returns uVector4.xwxw swizzling.
        /// </summary>
        public uVector4 xwxw => new uVector4(x, w, x, w);
        
        /// <summary>
        /// Returns uVector4.rara swizzling (equivalent to uVector4.xwxw).
        /// </summary>
        public uVector4 rara => new uVector4(x, w, x, w);
        
        /// <summary>
        /// Returns uVector4.xwy swizzling.
        /// </summary>
        public uVector3 xwy => new uVector3(x, w, y);
        
        /// <summary>
        /// Returns uVector4.rag swizzling (equivalent to uVector4.xwy).
        /// </summary>
        public uVector3 rag => new uVector3(x, w, y);
        
        /// <summary>
        /// Returns uVector4.xwyx swizzling.
        /// </summary>
        public uVector4 xwyx => new uVector4(x, w, y, x);
        
        /// <summary>
        /// Returns uVector4.ragr swizzling (equivalent to uVector4.xwyx).
        /// </summary>
        public uVector4 ragr => new uVector4(x, w, y, x);
        
        /// <summary>
        /// Returns uVector4.xwyy swizzling.
        /// </summary>
        public uVector4 xwyy => new uVector4(x, w, y, y);
        
        /// <summary>
        /// Returns uVector4.ragg swizzling (equivalent to uVector4.xwyy).
        /// </summary>
        public uVector4 ragg => new uVector4(x, w, y, y);
        
        /// <summary>
        /// Returns uVector4.xwyz swizzling.
        /// </summary>
        public uVector4 xwyz => new uVector4(x, w, y, z);
        
        /// <summary>
        /// Returns uVector4.ragb swizzling (equivalent to uVector4.xwyz).
        /// </summary>
        public uVector4 ragb => new uVector4(x, w, y, z);
        
        /// <summary>
        /// Returns uVector4.xwyw swizzling.
        /// </summary>
        public uVector4 xwyw => new uVector4(x, w, y, w);
        
        /// <summary>
        /// Returns uVector4.raga swizzling (equivalent to uVector4.xwyw).
        /// </summary>
        public uVector4 raga => new uVector4(x, w, y, w);
        
        /// <summary>
        /// Returns uVector4.xwz swizzling.
        /// </summary>
        public uVector3 xwz => new uVector3(x, w, z);
        
        /// <summary>
        /// Returns uVector4.rab swizzling (equivalent to uVector4.xwz).
        /// </summary>
        public uVector3 rab => new uVector3(x, w, z);
        
        /// <summary>
        /// Returns uVector4.xwzx swizzling.
        /// </summary>
        public uVector4 xwzx => new uVector4(x, w, z, x);
        
        /// <summary>
        /// Returns uVector4.rabr swizzling (equivalent to uVector4.xwzx).
        /// </summary>
        public uVector4 rabr => new uVector4(x, w, z, x);
        
        /// <summary>
        /// Returns uVector4.xwzy swizzling.
        /// </summary>
        public uVector4 xwzy => new uVector4(x, w, z, y);
        
        /// <summary>
        /// Returns uVector4.rabg swizzling (equivalent to uVector4.xwzy).
        /// </summary>
        public uVector4 rabg => new uVector4(x, w, z, y);
        
        /// <summary>
        /// Returns uVector4.xwzz swizzling.
        /// </summary>
        public uVector4 xwzz => new uVector4(x, w, z, z);
        
        /// <summary>
        /// Returns uVector4.rabb swizzling (equivalent to uVector4.xwzz).
        /// </summary>
        public uVector4 rabb => new uVector4(x, w, z, z);
        
        /// <summary>
        /// Returns uVector4.xwzw swizzling.
        /// </summary>
        public uVector4 xwzw => new uVector4(x, w, z, w);
        
        /// <summary>
        /// Returns uVector4.raba swizzling (equivalent to uVector4.xwzw).
        /// </summary>
        public uVector4 raba => new uVector4(x, w, z, w);
        
        /// <summary>
        /// Returns uVector4.xww swizzling.
        /// </summary>
        public uVector3 xww => new uVector3(x, w, w);
        
        /// <summary>
        /// Returns uVector4.raa swizzling (equivalent to uVector4.xww).
        /// </summary>
        public uVector3 raa => new uVector3(x, w, w);
        
        /// <summary>
        /// Returns uVector4.xwwx swizzling.
        /// </summary>
        public uVector4 xwwx => new uVector4(x, w, w, x);
        
        /// <summary>
        /// Returns uVector4.raar swizzling (equivalent to uVector4.xwwx).
        /// </summary>
        public uVector4 raar => new uVector4(x, w, w, x);
        
        /// <summary>
        /// Returns uVector4.xwwy swizzling.
        /// </summary>
        public uVector4 xwwy => new uVector4(x, w, w, y);
        
        /// <summary>
        /// Returns uVector4.raag swizzling (equivalent to uVector4.xwwy).
        /// </summary>
        public uVector4 raag => new uVector4(x, w, w, y);
        
        /// <summary>
        /// Returns uVector4.xwwz swizzling.
        /// </summary>
        public uVector4 xwwz => new uVector4(x, w, w, z);
        
        /// <summary>
        /// Returns uVector4.raab swizzling (equivalent to uVector4.xwwz).
        /// </summary>
        public uVector4 raab => new uVector4(x, w, w, z);
        
        /// <summary>
        /// Returns uVector4.xwww swizzling.
        /// </summary>
        public uVector4 xwww => new uVector4(x, w, w, w);
        
        /// <summary>
        /// Returns uVector4.raaa swizzling (equivalent to uVector4.xwww).
        /// </summary>
        public uVector4 raaa => new uVector4(x, w, w, w);
        
        /// <summary>
        /// Returns uVector4.yx swizzling.
        /// </summary>
        public uVector2 yx => new uVector2(y, x);
        
        /// <summary>
        /// Returns uVector4.gr swizzling (equivalent to uVector4.yx).
        /// </summary>
        public uVector2 gr => new uVector2(y, x);
        
        /// <summary>
        /// Returns uVector4.yxx swizzling.
        /// </summary>
        public uVector3 yxx => new uVector3(y, x, x);
        
        /// <summary>
        /// Returns uVector4.grr swizzling (equivalent to uVector4.yxx).
        /// </summary>
        public uVector3 grr => new uVector3(y, x, x);
        
        /// <summary>
        /// Returns uVector4.yxxx swizzling.
        /// </summary>
        public uVector4 yxxx => new uVector4(y, x, x, x);
        
        /// <summary>
        /// Returns uVector4.grrr swizzling (equivalent to uVector4.yxxx).
        /// </summary>
        public uVector4 grrr => new uVector4(y, x, x, x);
        
        /// <summary>
        /// Returns uVector4.yxxy swizzling.
        /// </summary>
        public uVector4 yxxy => new uVector4(y, x, x, y);
        
        /// <summary>
        /// Returns uVector4.grrg swizzling (equivalent to uVector4.yxxy).
        /// </summary>
        public uVector4 grrg => new uVector4(y, x, x, y);
        
        /// <summary>
        /// Returns uVector4.yxxz swizzling.
        /// </summary>
        public uVector4 yxxz => new uVector4(y, x, x, z);
        
        /// <summary>
        /// Returns uVector4.grrb swizzling (equivalent to uVector4.yxxz).
        /// </summary>
        public uVector4 grrb => new uVector4(y, x, x, z);
        
        /// <summary>
        /// Returns uVector4.yxxw swizzling.
        /// </summary>
        public uVector4 yxxw => new uVector4(y, x, x, w);
        
        /// <summary>
        /// Returns uVector4.grra swizzling (equivalent to uVector4.yxxw).
        /// </summary>
        public uVector4 grra => new uVector4(y, x, x, w);
        
        /// <summary>
        /// Returns uVector4.yxy swizzling.
        /// </summary>
        public uVector3 yxy => new uVector3(y, x, y);
        
        /// <summary>
        /// Returns uVector4.grg swizzling (equivalent to uVector4.yxy).
        /// </summary>
        public uVector3 grg => new uVector3(y, x, y);
        
        /// <summary>
        /// Returns uVector4.yxyx swizzling.
        /// </summary>
        public uVector4 yxyx => new uVector4(y, x, y, x);
        
        /// <summary>
        /// Returns uVector4.grgr swizzling (equivalent to uVector4.yxyx).
        /// </summary>
        public uVector4 grgr => new uVector4(y, x, y, x);
        
        /// <summary>
        /// Returns uVector4.yxyy swizzling.
        /// </summary>
        public uVector4 yxyy => new uVector4(y, x, y, y);
        
        /// <summary>
        /// Returns uVector4.grgg swizzling (equivalent to uVector4.yxyy).
        /// </summary>
        public uVector4 grgg => new uVector4(y, x, y, y);
        
        /// <summary>
        /// Returns uVector4.yxyz swizzling.
        /// </summary>
        public uVector4 yxyz => new uVector4(y, x, y, z);
        
        /// <summary>
        /// Returns uVector4.grgb swizzling (equivalent to uVector4.yxyz).
        /// </summary>
        public uVector4 grgb => new uVector4(y, x, y, z);
        
        /// <summary>
        /// Returns uVector4.yxyw swizzling.
        /// </summary>
        public uVector4 yxyw => new uVector4(y, x, y, w);
        
        /// <summary>
        /// Returns uVector4.grga swizzling (equivalent to uVector4.yxyw).
        /// </summary>
        public uVector4 grga => new uVector4(y, x, y, w);
        
        /// <summary>
        /// Returns uVector4.yxz swizzling.
        /// </summary>
        public uVector3 yxz => new uVector3(y, x, z);
        
        /// <summary>
        /// Returns uVector4.grb swizzling (equivalent to uVector4.yxz).
        /// </summary>
        public uVector3 grb => new uVector3(y, x, z);
        
        /// <summary>
        /// Returns uVector4.yxzx swizzling.
        /// </summary>
        public uVector4 yxzx => new uVector4(y, x, z, x);
        
        /// <summary>
        /// Returns uVector4.grbr swizzling (equivalent to uVector4.yxzx).
        /// </summary>
        public uVector4 grbr => new uVector4(y, x, z, x);
        
        /// <summary>
        /// Returns uVector4.yxzy swizzling.
        /// </summary>
        public uVector4 yxzy => new uVector4(y, x, z, y);
        
        /// <summary>
        /// Returns uVector4.grbg swizzling (equivalent to uVector4.yxzy).
        /// </summary>
        public uVector4 grbg => new uVector4(y, x, z, y);
        
        /// <summary>
        /// Returns uVector4.yxzz swizzling.
        /// </summary>
        public uVector4 yxzz => new uVector4(y, x, z, z);
        
        /// <summary>
        /// Returns uVector4.grbb swizzling (equivalent to uVector4.yxzz).
        /// </summary>
        public uVector4 grbb => new uVector4(y, x, z, z);
        
        /// <summary>
        /// Returns uVector4.yxzw swizzling.
        /// </summary>
        public uVector4 yxzw => new uVector4(y, x, z, w);
        
        /// <summary>
        /// Returns uVector4.grba swizzling (equivalent to uVector4.yxzw).
        /// </summary>
        public uVector4 grba => new uVector4(y, x, z, w);
        
        /// <summary>
        /// Returns uVector4.yxw swizzling.
        /// </summary>
        public uVector3 yxw => new uVector3(y, x, w);
        
        /// <summary>
        /// Returns uVector4.gra swizzling (equivalent to uVector4.yxw).
        /// </summary>
        public uVector3 gra => new uVector3(y, x, w);
        
        /// <summary>
        /// Returns uVector4.yxwx swizzling.
        /// </summary>
        public uVector4 yxwx => new uVector4(y, x, w, x);
        
        /// <summary>
        /// Returns uVector4.grar swizzling (equivalent to uVector4.yxwx).
        /// </summary>
        public uVector4 grar => new uVector4(y, x, w, x);
        
        /// <summary>
        /// Returns uVector4.yxwy swizzling.
        /// </summary>
        public uVector4 yxwy => new uVector4(y, x, w, y);
        
        /// <summary>
        /// Returns uVector4.grag swizzling (equivalent to uVector4.yxwy).
        /// </summary>
        public uVector4 grag => new uVector4(y, x, w, y);
        
        /// <summary>
        /// Returns uVector4.yxwz swizzling.
        /// </summary>
        public uVector4 yxwz => new uVector4(y, x, w, z);
        
        /// <summary>
        /// Returns uVector4.grab swizzling (equivalent to uVector4.yxwz).
        /// </summary>
        public uVector4 grab => new uVector4(y, x, w, z);
        
        /// <summary>
        /// Returns uVector4.yxww swizzling.
        /// </summary>
        public uVector4 yxww => new uVector4(y, x, w, w);
        
        /// <summary>
        /// Returns uVector4.graa swizzling (equivalent to uVector4.yxww).
        /// </summary>
        public uVector4 graa => new uVector4(y, x, w, w);
        
        /// <summary>
        /// Returns uVector4.yy swizzling.
        /// </summary>
        public uVector2 yy => new uVector2(y, y);
        
        /// <summary>
        /// Returns uVector4.gg swizzling (equivalent to uVector4.yy).
        /// </summary>
        public uVector2 gg => new uVector2(y, y);
        
        /// <summary>
        /// Returns uVector4.yyx swizzling.
        /// </summary>
        public uVector3 yyx => new uVector3(y, y, x);
        
        /// <summary>
        /// Returns uVector4.ggr swizzling (equivalent to uVector4.yyx).
        /// </summary>
        public uVector3 ggr => new uVector3(y, y, x);
        
        /// <summary>
        /// Returns uVector4.yyxx swizzling.
        /// </summary>
        public uVector4 yyxx => new uVector4(y, y, x, x);
        
        /// <summary>
        /// Returns uVector4.ggrr swizzling (equivalent to uVector4.yyxx).
        /// </summary>
        public uVector4 ggrr => new uVector4(y, y, x, x);
        
        /// <summary>
        /// Returns uVector4.yyxy swizzling.
        /// </summary>
        public uVector4 yyxy => new uVector4(y, y, x, y);
        
        /// <summary>
        /// Returns uVector4.ggrg swizzling (equivalent to uVector4.yyxy).
        /// </summary>
        public uVector4 ggrg => new uVector4(y, y, x, y);
        
        /// <summary>
        /// Returns uVector4.yyxz swizzling.
        /// </summary>
        public uVector4 yyxz => new uVector4(y, y, x, z);
        
        /// <summary>
        /// Returns uVector4.ggrb swizzling (equivalent to uVector4.yyxz).
        /// </summary>
        public uVector4 ggrb => new uVector4(y, y, x, z);
        
        /// <summary>
        /// Returns uVector4.yyxw swizzling.
        /// </summary>
        public uVector4 yyxw => new uVector4(y, y, x, w);
        
        /// <summary>
        /// Returns uVector4.ggra swizzling (equivalent to uVector4.yyxw).
        /// </summary>
        public uVector4 ggra => new uVector4(y, y, x, w);
        
        /// <summary>
        /// Returns uVector4.yyy swizzling.
        /// </summary>
        public uVector3 yyy => new uVector3(y, y, y);
        
        /// <summary>
        /// Returns uVector4.ggg swizzling (equivalent to uVector4.yyy).
        /// </summary>
        public uVector3 ggg => new uVector3(y, y, y);
        
        /// <summary>
        /// Returns uVector4.yyyx swizzling.
        /// </summary>
        public uVector4 yyyx => new uVector4(y, y, y, x);
        
        /// <summary>
        /// Returns uVector4.gggr swizzling (equivalent to uVector4.yyyx).
        /// </summary>
        public uVector4 gggr => new uVector4(y, y, y, x);
        
        /// <summary>
        /// Returns uVector4.yyyy swizzling.
        /// </summary>
        public uVector4 yyyy => new uVector4(y, y, y, y);
        
        /// <summary>
        /// Returns uVector4.gggg swizzling (equivalent to uVector4.yyyy).
        /// </summary>
        public uVector4 gggg => new uVector4(y, y, y, y);
        
        /// <summary>
        /// Returns uVector4.yyyz swizzling.
        /// </summary>
        public uVector4 yyyz => new uVector4(y, y, y, z);
        
        /// <summary>
        /// Returns uVector4.gggb swizzling (equivalent to uVector4.yyyz).
        /// </summary>
        public uVector4 gggb => new uVector4(y, y, y, z);
        
        /// <summary>
        /// Returns uVector4.yyyw swizzling.
        /// </summary>
        public uVector4 yyyw => new uVector4(y, y, y, w);
        
        /// <summary>
        /// Returns uVector4.ggga swizzling (equivalent to uVector4.yyyw).
        /// </summary>
        public uVector4 ggga => new uVector4(y, y, y, w);
        
        /// <summary>
        /// Returns uVector4.yyz swizzling.
        /// </summary>
        public uVector3 yyz => new uVector3(y, y, z);
        
        /// <summary>
        /// Returns uVector4.ggb swizzling (equivalent to uVector4.yyz).
        /// </summary>
        public uVector3 ggb => new uVector3(y, y, z);
        
        /// <summary>
        /// Returns uVector4.yyzx swizzling.
        /// </summary>
        public uVector4 yyzx => new uVector4(y, y, z, x);
        
        /// <summary>
        /// Returns uVector4.ggbr swizzling (equivalent to uVector4.yyzx).
        /// </summary>
        public uVector4 ggbr => new uVector4(y, y, z, x);
        
        /// <summary>
        /// Returns uVector4.yyzy swizzling.
        /// </summary>
        public uVector4 yyzy => new uVector4(y, y, z, y);
        
        /// <summary>
        /// Returns uVector4.ggbg swizzling (equivalent to uVector4.yyzy).
        /// </summary>
        public uVector4 ggbg => new uVector4(y, y, z, y);
        
        /// <summary>
        /// Returns uVector4.yyzz swizzling.
        /// </summary>
        public uVector4 yyzz => new uVector4(y, y, z, z);
        
        /// <summary>
        /// Returns uVector4.ggbb swizzling (equivalent to uVector4.yyzz).
        /// </summary>
        public uVector4 ggbb => new uVector4(y, y, z, z);
        
        /// <summary>
        /// Returns uVector4.yyzw swizzling.
        /// </summary>
        public uVector4 yyzw => new uVector4(y, y, z, w);
        
        /// <summary>
        /// Returns uVector4.ggba swizzling (equivalent to uVector4.yyzw).
        /// </summary>
        public uVector4 ggba => new uVector4(y, y, z, w);
        
        /// <summary>
        /// Returns uVector4.yyw swizzling.
        /// </summary>
        public uVector3 yyw => new uVector3(y, y, w);
        
        /// <summary>
        /// Returns uVector4.gga swizzling (equivalent to uVector4.yyw).
        /// </summary>
        public uVector3 gga => new uVector3(y, y, w);
        
        /// <summary>
        /// Returns uVector4.yywx swizzling.
        /// </summary>
        public uVector4 yywx => new uVector4(y, y, w, x);
        
        /// <summary>
        /// Returns uVector4.ggar swizzling (equivalent to uVector4.yywx).
        /// </summary>
        public uVector4 ggar => new uVector4(y, y, w, x);
        
        /// <summary>
        /// Returns uVector4.yywy swizzling.
        /// </summary>
        public uVector4 yywy => new uVector4(y, y, w, y);
        
        /// <summary>
        /// Returns uVector4.ggag swizzling (equivalent to uVector4.yywy).
        /// </summary>
        public uVector4 ggag => new uVector4(y, y, w, y);
        
        /// <summary>
        /// Returns uVector4.yywz swizzling.
        /// </summary>
        public uVector4 yywz => new uVector4(y, y, w, z);
        
        /// <summary>
        /// Returns uVector4.ggab swizzling (equivalent to uVector4.yywz).
        /// </summary>
        public uVector4 ggab => new uVector4(y, y, w, z);
        
        /// <summary>
        /// Returns uVector4.yyww swizzling.
        /// </summary>
        public uVector4 yyww => new uVector4(y, y, w, w);
        
        /// <summary>
        /// Returns uVector4.ggaa swizzling (equivalent to uVector4.yyww).
        /// </summary>
        public uVector4 ggaa => new uVector4(y, y, w, w);
        
        /// <summary>
        /// Returns uVector4.yz swizzling.
        /// </summary>
        public uVector2 yz => new uVector2(y, z);
        
        /// <summary>
        /// Returns uVector4.gb swizzling (equivalent to uVector4.yz).
        /// </summary>
        public uVector2 gb => new uVector2(y, z);
        
        /// <summary>
        /// Returns uVector4.yzx swizzling.
        /// </summary>
        public uVector3 yzx => new uVector3(y, z, x);
        
        /// <summary>
        /// Returns uVector4.gbr swizzling (equivalent to uVector4.yzx).
        /// </summary>
        public uVector3 gbr => new uVector3(y, z, x);
        
        /// <summary>
        /// Returns uVector4.yzxx swizzling.
        /// </summary>
        public uVector4 yzxx => new uVector4(y, z, x, x);
        
        /// <summary>
        /// Returns uVector4.gbrr swizzling (equivalent to uVector4.yzxx).
        /// </summary>
        public uVector4 gbrr => new uVector4(y, z, x, x);
        
        /// <summary>
        /// Returns uVector4.yzxy swizzling.
        /// </summary>
        public uVector4 yzxy => new uVector4(y, z, x, y);
        
        /// <summary>
        /// Returns uVector4.gbrg swizzling (equivalent to uVector4.yzxy).
        /// </summary>
        public uVector4 gbrg => new uVector4(y, z, x, y);
        
        /// <summary>
        /// Returns uVector4.yzxz swizzling.
        /// </summary>
        public uVector4 yzxz => new uVector4(y, z, x, z);
        
        /// <summary>
        /// Returns uVector4.gbrb swizzling (equivalent to uVector4.yzxz).
        /// </summary>
        public uVector4 gbrb => new uVector4(y, z, x, z);
        
        /// <summary>
        /// Returns uVector4.yzxw swizzling.
        /// </summary>
        public uVector4 yzxw => new uVector4(y, z, x, w);
        
        /// <summary>
        /// Returns uVector4.gbra swizzling (equivalent to uVector4.yzxw).
        /// </summary>
        public uVector4 gbra => new uVector4(y, z, x, w);
        
        /// <summary>
        /// Returns uVector4.yzy swizzling.
        /// </summary>
        public uVector3 yzy => new uVector3(y, z, y);
        
        /// <summary>
        /// Returns uVector4.gbg swizzling (equivalent to uVector4.yzy).
        /// </summary>
        public uVector3 gbg => new uVector3(y, z, y);
        
        /// <summary>
        /// Returns uVector4.yzyx swizzling.
        /// </summary>
        public uVector4 yzyx => new uVector4(y, z, y, x);
        
        /// <summary>
        /// Returns uVector4.gbgr swizzling (equivalent to uVector4.yzyx).
        /// </summary>
        public uVector4 gbgr => new uVector4(y, z, y, x);
        
        /// <summary>
        /// Returns uVector4.yzyy swizzling.
        /// </summary>
        public uVector4 yzyy => new uVector4(y, z, y, y);
        
        /// <summary>
        /// Returns uVector4.gbgg swizzling (equivalent to uVector4.yzyy).
        /// </summary>
        public uVector4 gbgg => new uVector4(y, z, y, y);
        
        /// <summary>
        /// Returns uVector4.yzyz swizzling.
        /// </summary>
        public uVector4 yzyz => new uVector4(y, z, y, z);
        
        /// <summary>
        /// Returns uVector4.gbgb swizzling (equivalent to uVector4.yzyz).
        /// </summary>
        public uVector4 gbgb => new uVector4(y, z, y, z);
        
        /// <summary>
        /// Returns uVector4.yzyw swizzling.
        /// </summary>
        public uVector4 yzyw => new uVector4(y, z, y, w);
        
        /// <summary>
        /// Returns uVector4.gbga swizzling (equivalent to uVector4.yzyw).
        /// </summary>
        public uVector4 gbga => new uVector4(y, z, y, w);
        
        /// <summary>
        /// Returns uVector4.yzz swizzling.
        /// </summary>
        public uVector3 yzz => new uVector3(y, z, z);
        
        /// <summary>
        /// Returns uVector4.gbb swizzling (equivalent to uVector4.yzz).
        /// </summary>
        public uVector3 gbb => new uVector3(y, z, z);
        
        /// <summary>
        /// Returns uVector4.yzzx swizzling.
        /// </summary>
        public uVector4 yzzx => new uVector4(y, z, z, x);
        
        /// <summary>
        /// Returns uVector4.gbbr swizzling (equivalent to uVector4.yzzx).
        /// </summary>
        public uVector4 gbbr => new uVector4(y, z, z, x);
        
        /// <summary>
        /// Returns uVector4.yzzy swizzling.
        /// </summary>
        public uVector4 yzzy => new uVector4(y, z, z, y);
        
        /// <summary>
        /// Returns uVector4.gbbg swizzling (equivalent to uVector4.yzzy).
        /// </summary>
        public uVector4 gbbg => new uVector4(y, z, z, y);
        
        /// <summary>
        /// Returns uVector4.yzzz swizzling.
        /// </summary>
        public uVector4 yzzz => new uVector4(y, z, z, z);
        
        /// <summary>
        /// Returns uVector4.gbbb swizzling (equivalent to uVector4.yzzz).
        /// </summary>
        public uVector4 gbbb => new uVector4(y, z, z, z);
        
        /// <summary>
        /// Returns uVector4.yzzw swizzling.
        /// </summary>
        public uVector4 yzzw => new uVector4(y, z, z, w);
        
        /// <summary>
        /// Returns uVector4.gbba swizzling (equivalent to uVector4.yzzw).
        /// </summary>
        public uVector4 gbba => new uVector4(y, z, z, w);
        
        /// <summary>
        /// Returns uVector4.yzw swizzling.
        /// </summary>
        public uVector3 yzw => new uVector3(y, z, w);
        
        /// <summary>
        /// Returns uVector4.gba swizzling (equivalent to uVector4.yzw).
        /// </summary>
        public uVector3 gba => new uVector3(y, z, w);
        
        /// <summary>
        /// Returns uVector4.yzwx swizzling.
        /// </summary>
        public uVector4 yzwx => new uVector4(y, z, w, x);
        
        /// <summary>
        /// Returns uVector4.gbar swizzling (equivalent to uVector4.yzwx).
        /// </summary>
        public uVector4 gbar => new uVector4(y, z, w, x);
        
        /// <summary>
        /// Returns uVector4.yzwy swizzling.
        /// </summary>
        public uVector4 yzwy => new uVector4(y, z, w, y);
        
        /// <summary>
        /// Returns uVector4.gbag swizzling (equivalent to uVector4.yzwy).
        /// </summary>
        public uVector4 gbag => new uVector4(y, z, w, y);
        
        /// <summary>
        /// Returns uVector4.yzwz swizzling.
        /// </summary>
        public uVector4 yzwz => new uVector4(y, z, w, z);
        
        /// <summary>
        /// Returns uVector4.gbab swizzling (equivalent to uVector4.yzwz).
        /// </summary>
        public uVector4 gbab => new uVector4(y, z, w, z);
        
        /// <summary>
        /// Returns uVector4.yzww swizzling.
        /// </summary>
        public uVector4 yzww => new uVector4(y, z, w, w);
        
        /// <summary>
        /// Returns uVector4.gbaa swizzling (equivalent to uVector4.yzww).
        /// </summary>
        public uVector4 gbaa => new uVector4(y, z, w, w);
        
        /// <summary>
        /// Returns uVector4.yw swizzling.
        /// </summary>
        public uVector2 yw => new uVector2(y, w);
        
        /// <summary>
        /// Returns uVector4.ga swizzling (equivalent to uVector4.yw).
        /// </summary>
        public uVector2 ga => new uVector2(y, w);
        
        /// <summary>
        /// Returns uVector4.ywx swizzling.
        /// </summary>
        public uVector3 ywx => new uVector3(y, w, x);
        
        /// <summary>
        /// Returns uVector4.gar swizzling (equivalent to uVector4.ywx).
        /// </summary>
        public uVector3 gar => new uVector3(y, w, x);
        
        /// <summary>
        /// Returns uVector4.ywxx swizzling.
        /// </summary>
        public uVector4 ywxx => new uVector4(y, w, x, x);
        
        /// <summary>
        /// Returns uVector4.garr swizzling (equivalent to uVector4.ywxx).
        /// </summary>
        public uVector4 garr => new uVector4(y, w, x, x);
        
        /// <summary>
        /// Returns uVector4.ywxy swizzling.
        /// </summary>
        public uVector4 ywxy => new uVector4(y, w, x, y);
        
        /// <summary>
        /// Returns uVector4.garg swizzling (equivalent to uVector4.ywxy).
        /// </summary>
        public uVector4 garg => new uVector4(y, w, x, y);
        
        /// <summary>
        /// Returns uVector4.ywxz swizzling.
        /// </summary>
        public uVector4 ywxz => new uVector4(y, w, x, z);
        
        /// <summary>
        /// Returns uVector4.garb swizzling (equivalent to uVector4.ywxz).
        /// </summary>
        public uVector4 garb => new uVector4(y, w, x, z);
        
        /// <summary>
        /// Returns uVector4.ywxw swizzling.
        /// </summary>
        public uVector4 ywxw => new uVector4(y, w, x, w);
        
        /// <summary>
        /// Returns uVector4.gara swizzling (equivalent to uVector4.ywxw).
        /// </summary>
        public uVector4 gara => new uVector4(y, w, x, w);
        
        /// <summary>
        /// Returns uVector4.ywy swizzling.
        /// </summary>
        public uVector3 ywy => new uVector3(y, w, y);
        
        /// <summary>
        /// Returns uVector4.gag swizzling (equivalent to uVector4.ywy).
        /// </summary>
        public uVector3 gag => new uVector3(y, w, y);
        
        /// <summary>
        /// Returns uVector4.ywyx swizzling.
        /// </summary>
        public uVector4 ywyx => new uVector4(y, w, y, x);
        
        /// <summary>
        /// Returns uVector4.gagr swizzling (equivalent to uVector4.ywyx).
        /// </summary>
        public uVector4 gagr => new uVector4(y, w, y, x);
        
        /// <summary>
        /// Returns uVector4.ywyy swizzling.
        /// </summary>
        public uVector4 ywyy => new uVector4(y, w, y, y);
        
        /// <summary>
        /// Returns uVector4.gagg swizzling (equivalent to uVector4.ywyy).
        /// </summary>
        public uVector4 gagg => new uVector4(y, w, y, y);
        
        /// <summary>
        /// Returns uVector4.ywyz swizzling.
        /// </summary>
        public uVector4 ywyz => new uVector4(y, w, y, z);
        
        /// <summary>
        /// Returns uVector4.gagb swizzling (equivalent to uVector4.ywyz).
        /// </summary>
        public uVector4 gagb => new uVector4(y, w, y, z);
        
        /// <summary>
        /// Returns uVector4.ywyw swizzling.
        /// </summary>
        public uVector4 ywyw => new uVector4(y, w, y, w);
        
        /// <summary>
        /// Returns uVector4.gaga swizzling (equivalent to uVector4.ywyw).
        /// </summary>
        public uVector4 gaga => new uVector4(y, w, y, w);
        
        /// <summary>
        /// Returns uVector4.ywz swizzling.
        /// </summary>
        public uVector3 ywz => new uVector3(y, w, z);
        
        /// <summary>
        /// Returns uVector4.gab swizzling (equivalent to uVector4.ywz).
        /// </summary>
        public uVector3 gab => new uVector3(y, w, z);
        
        /// <summary>
        /// Returns uVector4.ywzx swizzling.
        /// </summary>
        public uVector4 ywzx => new uVector4(y, w, z, x);
        
        /// <summary>
        /// Returns uVector4.gabr swizzling (equivalent to uVector4.ywzx).
        /// </summary>
        public uVector4 gabr => new uVector4(y, w, z, x);
        
        /// <summary>
        /// Returns uVector4.ywzy swizzling.
        /// </summary>
        public uVector4 ywzy => new uVector4(y, w, z, y);
        
        /// <summary>
        /// Returns uVector4.gabg swizzling (equivalent to uVector4.ywzy).
        /// </summary>
        public uVector4 gabg => new uVector4(y, w, z, y);
        
        /// <summary>
        /// Returns uVector4.ywzz swizzling.
        /// </summary>
        public uVector4 ywzz => new uVector4(y, w, z, z);
        
        /// <summary>
        /// Returns uVector4.gabb swizzling (equivalent to uVector4.ywzz).
        /// </summary>
        public uVector4 gabb => new uVector4(y, w, z, z);
        
        /// <summary>
        /// Returns uVector4.ywzw swizzling.
        /// </summary>
        public uVector4 ywzw => new uVector4(y, w, z, w);
        
        /// <summary>
        /// Returns uVector4.gaba swizzling (equivalent to uVector4.ywzw).
        /// </summary>
        public uVector4 gaba => new uVector4(y, w, z, w);
        
        /// <summary>
        /// Returns uVector4.yww swizzling.
        /// </summary>
        public uVector3 yww => new uVector3(y, w, w);
        
        /// <summary>
        /// Returns uVector4.gaa swizzling (equivalent to uVector4.yww).
        /// </summary>
        public uVector3 gaa => new uVector3(y, w, w);
        
        /// <summary>
        /// Returns uVector4.ywwx swizzling.
        /// </summary>
        public uVector4 ywwx => new uVector4(y, w, w, x);
        
        /// <summary>
        /// Returns uVector4.gaar swizzling (equivalent to uVector4.ywwx).
        /// </summary>
        public uVector4 gaar => new uVector4(y, w, w, x);
        
        /// <summary>
        /// Returns uVector4.ywwy swizzling.
        /// </summary>
        public uVector4 ywwy => new uVector4(y, w, w, y);
        
        /// <summary>
        /// Returns uVector4.gaag swizzling (equivalent to uVector4.ywwy).
        /// </summary>
        public uVector4 gaag => new uVector4(y, w, w, y);
        
        /// <summary>
        /// Returns uVector4.ywwz swizzling.
        /// </summary>
        public uVector4 ywwz => new uVector4(y, w, w, z);
        
        /// <summary>
        /// Returns uVector4.gaab swizzling (equivalent to uVector4.ywwz).
        /// </summary>
        public uVector4 gaab => new uVector4(y, w, w, z);
        
        /// <summary>
        /// Returns uVector4.ywww swizzling.
        /// </summary>
        public uVector4 ywww => new uVector4(y, w, w, w);
        
        /// <summary>
        /// Returns uVector4.gaaa swizzling (equivalent to uVector4.ywww).
        /// </summary>
        public uVector4 gaaa => new uVector4(y, w, w, w);
        
        /// <summary>
        /// Returns uVector4.zx swizzling.
        /// </summary>
        public uVector2 zx => new uVector2(z, x);
        
        /// <summary>
        /// Returns uVector4.br swizzling (equivalent to uVector4.zx).
        /// </summary>
        public uVector2 br => new uVector2(z, x);
        
        /// <summary>
        /// Returns uVector4.zxx swizzling.
        /// </summary>
        public uVector3 zxx => new uVector3(z, x, x);
        
        /// <summary>
        /// Returns uVector4.brr swizzling (equivalent to uVector4.zxx).
        /// </summary>
        public uVector3 brr => new uVector3(z, x, x);
        
        /// <summary>
        /// Returns uVector4.zxxx swizzling.
        /// </summary>
        public uVector4 zxxx => new uVector4(z, x, x, x);
        
        /// <summary>
        /// Returns uVector4.brrr swizzling (equivalent to uVector4.zxxx).
        /// </summary>
        public uVector4 brrr => new uVector4(z, x, x, x);
        
        /// <summary>
        /// Returns uVector4.zxxy swizzling.
        /// </summary>
        public uVector4 zxxy => new uVector4(z, x, x, y);
        
        /// <summary>
        /// Returns uVector4.brrg swizzling (equivalent to uVector4.zxxy).
        /// </summary>
        public uVector4 brrg => new uVector4(z, x, x, y);
        
        /// <summary>
        /// Returns uVector4.zxxz swizzling.
        /// </summary>
        public uVector4 zxxz => new uVector4(z, x, x, z);
        
        /// <summary>
        /// Returns uVector4.brrb swizzling (equivalent to uVector4.zxxz).
        /// </summary>
        public uVector4 brrb => new uVector4(z, x, x, z);
        
        /// <summary>
        /// Returns uVector4.zxxw swizzling.
        /// </summary>
        public uVector4 zxxw => new uVector4(z, x, x, w);
        
        /// <summary>
        /// Returns uVector4.brra swizzling (equivalent to uVector4.zxxw).
        /// </summary>
        public uVector4 brra => new uVector4(z, x, x, w);
        
        /// <summary>
        /// Returns uVector4.zxy swizzling.
        /// </summary>
        public uVector3 zxy => new uVector3(z, x, y);
        
        /// <summary>
        /// Returns uVector4.brg swizzling (equivalent to uVector4.zxy).
        /// </summary>
        public uVector3 brg => new uVector3(z, x, y);
        
        /// <summary>
        /// Returns uVector4.zxyx swizzling.
        /// </summary>
        public uVector4 zxyx => new uVector4(z, x, y, x);
        
        /// <summary>
        /// Returns uVector4.brgr swizzling (equivalent to uVector4.zxyx).
        /// </summary>
        public uVector4 brgr => new uVector4(z, x, y, x);
        
        /// <summary>
        /// Returns uVector4.zxyy swizzling.
        /// </summary>
        public uVector4 zxyy => new uVector4(z, x, y, y);
        
        /// <summary>
        /// Returns uVector4.brgg swizzling (equivalent to uVector4.zxyy).
        /// </summary>
        public uVector4 brgg => new uVector4(z, x, y, y);
        
        /// <summary>
        /// Returns uVector4.zxyz swizzling.
        /// </summary>
        public uVector4 zxyz => new uVector4(z, x, y, z);
        
        /// <summary>
        /// Returns uVector4.brgb swizzling (equivalent to uVector4.zxyz).
        /// </summary>
        public uVector4 brgb => new uVector4(z, x, y, z);
        
        /// <summary>
        /// Returns uVector4.zxyw swizzling.
        /// </summary>
        public uVector4 zxyw => new uVector4(z, x, y, w);
        
        /// <summary>
        /// Returns uVector4.brga swizzling (equivalent to uVector4.zxyw).
        /// </summary>
        public uVector4 brga => new uVector4(z, x, y, w);
        
        /// <summary>
        /// Returns uVector4.zxz swizzling.
        /// </summary>
        public uVector3 zxz => new uVector3(z, x, z);
        
        /// <summary>
        /// Returns uVector4.brb swizzling (equivalent to uVector4.zxz).
        /// </summary>
        public uVector3 brb => new uVector3(z, x, z);
        
        /// <summary>
        /// Returns uVector4.zxzx swizzling.
        /// </summary>
        public uVector4 zxzx => new uVector4(z, x, z, x);
        
        /// <summary>
        /// Returns uVector4.brbr swizzling (equivalent to uVector4.zxzx).
        /// </summary>
        public uVector4 brbr => new uVector4(z, x, z, x);
        
        /// <summary>
        /// Returns uVector4.zxzy swizzling.
        /// </summary>
        public uVector4 zxzy => new uVector4(z, x, z, y);
        
        /// <summary>
        /// Returns uVector4.brbg swizzling (equivalent to uVector4.zxzy).
        /// </summary>
        public uVector4 brbg => new uVector4(z, x, z, y);
        
        /// <summary>
        /// Returns uVector4.zxzz swizzling.
        /// </summary>
        public uVector4 zxzz => new uVector4(z, x, z, z);
        
        /// <summary>
        /// Returns uVector4.brbb swizzling (equivalent to uVector4.zxzz).
        /// </summary>
        public uVector4 brbb => new uVector4(z, x, z, z);
        
        /// <summary>
        /// Returns uVector4.zxzw swizzling.
        /// </summary>
        public uVector4 zxzw => new uVector4(z, x, z, w);
        
        /// <summary>
        /// Returns uVector4.brba swizzling (equivalent to uVector4.zxzw).
        /// </summary>
        public uVector4 brba => new uVector4(z, x, z, w);
        
        /// <summary>
        /// Returns uVector4.zxw swizzling.
        /// </summary>
        public uVector3 zxw => new uVector3(z, x, w);
        
        /// <summary>
        /// Returns uVector4.bra swizzling (equivalent to uVector4.zxw).
        /// </summary>
        public uVector3 bra => new uVector3(z, x, w);
        
        /// <summary>
        /// Returns uVector4.zxwx swizzling.
        /// </summary>
        public uVector4 zxwx => new uVector4(z, x, w, x);
        
        /// <summary>
        /// Returns uVector4.brar swizzling (equivalent to uVector4.zxwx).
        /// </summary>
        public uVector4 brar => new uVector4(z, x, w, x);
        
        /// <summary>
        /// Returns uVector4.zxwy swizzling.
        /// </summary>
        public uVector4 zxwy => new uVector4(z, x, w, y);
        
        /// <summary>
        /// Returns uVector4.brag swizzling (equivalent to uVector4.zxwy).
        /// </summary>
        public uVector4 brag => new uVector4(z, x, w, y);
        
        /// <summary>
        /// Returns uVector4.zxwz swizzling.
        /// </summary>
        public uVector4 zxwz => new uVector4(z, x, w, z);
        
        /// <summary>
        /// Returns uVector4.brab swizzling (equivalent to uVector4.zxwz).
        /// </summary>
        public uVector4 brab => new uVector4(z, x, w, z);
        
        /// <summary>
        /// Returns uVector4.zxww swizzling.
        /// </summary>
        public uVector4 zxww => new uVector4(z, x, w, w);
        
        /// <summary>
        /// Returns uVector4.braa swizzling (equivalent to uVector4.zxww).
        /// </summary>
        public uVector4 braa => new uVector4(z, x, w, w);
        
        /// <summary>
        /// Returns uVector4.zy swizzling.
        /// </summary>
        public uVector2 zy => new uVector2(z, y);
        
        /// <summary>
        /// Returns uVector4.bg swizzling (equivalent to uVector4.zy).
        /// </summary>
        public uVector2 bg => new uVector2(z, y);
        
        /// <summary>
        /// Returns uVector4.zyx swizzling.
        /// </summary>
        public uVector3 zyx => new uVector3(z, y, x);
        
        /// <summary>
        /// Returns uVector4.bgr swizzling (equivalent to uVector4.zyx).
        /// </summary>
        public uVector3 bgr => new uVector3(z, y, x);
        
        /// <summary>
        /// Returns uVector4.zyxx swizzling.
        /// </summary>
        public uVector4 zyxx => new uVector4(z, y, x, x);
        
        /// <summary>
        /// Returns uVector4.bgrr swizzling (equivalent to uVector4.zyxx).
        /// </summary>
        public uVector4 bgrr => new uVector4(z, y, x, x);
        
        /// <summary>
        /// Returns uVector4.zyxy swizzling.
        /// </summary>
        public uVector4 zyxy => new uVector4(z, y, x, y);
        
        /// <summary>
        /// Returns uVector4.bgrg swizzling (equivalent to uVector4.zyxy).
        /// </summary>
        public uVector4 bgrg => new uVector4(z, y, x, y);
        
        /// <summary>
        /// Returns uVector4.zyxz swizzling.
        /// </summary>
        public uVector4 zyxz => new uVector4(z, y, x, z);
        
        /// <summary>
        /// Returns uVector4.bgrb swizzling (equivalent to uVector4.zyxz).
        /// </summary>
        public uVector4 bgrb => new uVector4(z, y, x, z);
        
        /// <summary>
        /// Returns uVector4.zyxw swizzling.
        /// </summary>
        public uVector4 zyxw => new uVector4(z, y, x, w);
        
        /// <summary>
        /// Returns uVector4.bgra swizzling (equivalent to uVector4.zyxw).
        /// </summary>
        public uVector4 bgra => new uVector4(z, y, x, w);
        
        /// <summary>
        /// Returns uVector4.zyy swizzling.
        /// </summary>
        public uVector3 zyy => new uVector3(z, y, y);
        
        /// <summary>
        /// Returns uVector4.bgg swizzling (equivalent to uVector4.zyy).
        /// </summary>
        public uVector3 bgg => new uVector3(z, y, y);
        
        /// <summary>
        /// Returns uVector4.zyyx swizzling.
        /// </summary>
        public uVector4 zyyx => new uVector4(z, y, y, x);
        
        /// <summary>
        /// Returns uVector4.bggr swizzling (equivalent to uVector4.zyyx).
        /// </summary>
        public uVector4 bggr => new uVector4(z, y, y, x);
        
        /// <summary>
        /// Returns uVector4.zyyy swizzling.
        /// </summary>
        public uVector4 zyyy => new uVector4(z, y, y, y);
        
        /// <summary>
        /// Returns uVector4.bggg swizzling (equivalent to uVector4.zyyy).
        /// </summary>
        public uVector4 bggg => new uVector4(z, y, y, y);
        
        /// <summary>
        /// Returns uVector4.zyyz swizzling.
        /// </summary>
        public uVector4 zyyz => new uVector4(z, y, y, z);
        
        /// <summary>
        /// Returns uVector4.bggb swizzling (equivalent to uVector4.zyyz).
        /// </summary>
        public uVector4 bggb => new uVector4(z, y, y, z);
        
        /// <summary>
        /// Returns uVector4.zyyw swizzling.
        /// </summary>
        public uVector4 zyyw => new uVector4(z, y, y, w);
        
        /// <summary>
        /// Returns uVector4.bgga swizzling (equivalent to uVector4.zyyw).
        /// </summary>
        public uVector4 bgga => new uVector4(z, y, y, w);
        
        /// <summary>
        /// Returns uVector4.zyz swizzling.
        /// </summary>
        public uVector3 zyz => new uVector3(z, y, z);
        
        /// <summary>
        /// Returns uVector4.bgb swizzling (equivalent to uVector4.zyz).
        /// </summary>
        public uVector3 bgb => new uVector3(z, y, z);
        
        /// <summary>
        /// Returns uVector4.zyzx swizzling.
        /// </summary>
        public uVector4 zyzx => new uVector4(z, y, z, x);
        
        /// <summary>
        /// Returns uVector4.bgbr swizzling (equivalent to uVector4.zyzx).
        /// </summary>
        public uVector4 bgbr => new uVector4(z, y, z, x);
        
        /// <summary>
        /// Returns uVector4.zyzy swizzling.
        /// </summary>
        public uVector4 zyzy => new uVector4(z, y, z, y);
        
        /// <summary>
        /// Returns uVector4.bgbg swizzling (equivalent to uVector4.zyzy).
        /// </summary>
        public uVector4 bgbg => new uVector4(z, y, z, y);
        
        /// <summary>
        /// Returns uVector4.zyzz swizzling.
        /// </summary>
        public uVector4 zyzz => new uVector4(z, y, z, z);
        
        /// <summary>
        /// Returns uVector4.bgbb swizzling (equivalent to uVector4.zyzz).
        /// </summary>
        public uVector4 bgbb => new uVector4(z, y, z, z);
        
        /// <summary>
        /// Returns uVector4.zyzw swizzling.
        /// </summary>
        public uVector4 zyzw => new uVector4(z, y, z, w);
        
        /// <summary>
        /// Returns uVector4.bgba swizzling (equivalent to uVector4.zyzw).
        /// </summary>
        public uVector4 bgba => new uVector4(z, y, z, w);
        
        /// <summary>
        /// Returns uVector4.zyw swizzling.
        /// </summary>
        public uVector3 zyw => new uVector3(z, y, w);
        
        /// <summary>
        /// Returns uVector4.bga swizzling (equivalent to uVector4.zyw).
        /// </summary>
        public uVector3 bga => new uVector3(z, y, w);
        
        /// <summary>
        /// Returns uVector4.zywx swizzling.
        /// </summary>
        public uVector4 zywx => new uVector4(z, y, w, x);
        
        /// <summary>
        /// Returns uVector4.bgar swizzling (equivalent to uVector4.zywx).
        /// </summary>
        public uVector4 bgar => new uVector4(z, y, w, x);
        
        /// <summary>
        /// Returns uVector4.zywy swizzling.
        /// </summary>
        public uVector4 zywy => new uVector4(z, y, w, y);
        
        /// <summary>
        /// Returns uVector4.bgag swizzling (equivalent to uVector4.zywy).
        /// </summary>
        public uVector4 bgag => new uVector4(z, y, w, y);
        
        /// <summary>
        /// Returns uVector4.zywz swizzling.
        /// </summary>
        public uVector4 zywz => new uVector4(z, y, w, z);
        
        /// <summary>
        /// Returns uVector4.bgab swizzling (equivalent to uVector4.zywz).
        /// </summary>
        public uVector4 bgab => new uVector4(z, y, w, z);
        
        /// <summary>
        /// Returns uVector4.zyww swizzling.
        /// </summary>
        public uVector4 zyww => new uVector4(z, y, w, w);
        
        /// <summary>
        /// Returns uVector4.bgaa swizzling (equivalent to uVector4.zyww).
        /// </summary>
        public uVector4 bgaa => new uVector4(z, y, w, w);
        
        /// <summary>
        /// Returns uVector4.zz swizzling.
        /// </summary>
        public uVector2 zz => new uVector2(z, z);
        
        /// <summary>
        /// Returns uVector4.bb swizzling (equivalent to uVector4.zz).
        /// </summary>
        public uVector2 bb => new uVector2(z, z);
        
        /// <summary>
        /// Returns uVector4.zzx swizzling.
        /// </summary>
        public uVector3 zzx => new uVector3(z, z, x);
        
        /// <summary>
        /// Returns uVector4.bbr swizzling (equivalent to uVector4.zzx).
        /// </summary>
        public uVector3 bbr => new uVector3(z, z, x);
        
        /// <summary>
        /// Returns uVector4.zzxx swizzling.
        /// </summary>
        public uVector4 zzxx => new uVector4(z, z, x, x);
        
        /// <summary>
        /// Returns uVector4.bbrr swizzling (equivalent to uVector4.zzxx).
        /// </summary>
        public uVector4 bbrr => new uVector4(z, z, x, x);
        
        /// <summary>
        /// Returns uVector4.zzxy swizzling.
        /// </summary>
        public uVector4 zzxy => new uVector4(z, z, x, y);
        
        /// <summary>
        /// Returns uVector4.bbrg swizzling (equivalent to uVector4.zzxy).
        /// </summary>
        public uVector4 bbrg => new uVector4(z, z, x, y);
        
        /// <summary>
        /// Returns uVector4.zzxz swizzling.
        /// </summary>
        public uVector4 zzxz => new uVector4(z, z, x, z);
        
        /// <summary>
        /// Returns uVector4.bbrb swizzling (equivalent to uVector4.zzxz).
        /// </summary>
        public uVector4 bbrb => new uVector4(z, z, x, z);
        
        /// <summary>
        /// Returns uVector4.zzxw swizzling.
        /// </summary>
        public uVector4 zzxw => new uVector4(z, z, x, w);
        
        /// <summary>
        /// Returns uVector4.bbra swizzling (equivalent to uVector4.zzxw).
        /// </summary>
        public uVector4 bbra => new uVector4(z, z, x, w);
        
        /// <summary>
        /// Returns uVector4.zzy swizzling.
        /// </summary>
        public uVector3 zzy => new uVector3(z, z, y);
        
        /// <summary>
        /// Returns uVector4.bbg swizzling (equivalent to uVector4.zzy).
        /// </summary>
        public uVector3 bbg => new uVector3(z, z, y);
        
        /// <summary>
        /// Returns uVector4.zzyx swizzling.
        /// </summary>
        public uVector4 zzyx => new uVector4(z, z, y, x);
        
        /// <summary>
        /// Returns uVector4.bbgr swizzling (equivalent to uVector4.zzyx).
        /// </summary>
        public uVector4 bbgr => new uVector4(z, z, y, x);
        
        /// <summary>
        /// Returns uVector4.zzyy swizzling.
        /// </summary>
        public uVector4 zzyy => new uVector4(z, z, y, y);
        
        /// <summary>
        /// Returns uVector4.bbgg swizzling (equivalent to uVector4.zzyy).
        /// </summary>
        public uVector4 bbgg => new uVector4(z, z, y, y);
        
        /// <summary>
        /// Returns uVector4.zzyz swizzling.
        /// </summary>
        public uVector4 zzyz => new uVector4(z, z, y, z);
        
        /// <summary>
        /// Returns uVector4.bbgb swizzling (equivalent to uVector4.zzyz).
        /// </summary>
        public uVector4 bbgb => new uVector4(z, z, y, z);
        
        /// <summary>
        /// Returns uVector4.zzyw swizzling.
        /// </summary>
        public uVector4 zzyw => new uVector4(z, z, y, w);
        
        /// <summary>
        /// Returns uVector4.bbga swizzling (equivalent to uVector4.zzyw).
        /// </summary>
        public uVector4 bbga => new uVector4(z, z, y, w);
        
        /// <summary>
        /// Returns uVector4.zzz swizzling.
        /// </summary>
        public uVector3 zzz => new uVector3(z, z, z);
        
        /// <summary>
        /// Returns uVector4.bbb swizzling (equivalent to uVector4.zzz).
        /// </summary>
        public uVector3 bbb => new uVector3(z, z, z);
        
        /// <summary>
        /// Returns uVector4.zzzx swizzling.
        /// </summary>
        public uVector4 zzzx => new uVector4(z, z, z, x);
        
        /// <summary>
        /// Returns uVector4.bbbr swizzling (equivalent to uVector4.zzzx).
        /// </summary>
        public uVector4 bbbr => new uVector4(z, z, z, x);
        
        /// <summary>
        /// Returns uVector4.zzzy swizzling.
        /// </summary>
        public uVector4 zzzy => new uVector4(z, z, z, y);
        
        /// <summary>
        /// Returns uVector4.bbbg swizzling (equivalent to uVector4.zzzy).
        /// </summary>
        public uVector4 bbbg => new uVector4(z, z, z, y);
        
        /// <summary>
        /// Returns uVector4.zzzz swizzling.
        /// </summary>
        public uVector4 zzzz => new uVector4(z, z, z, z);
        
        /// <summary>
        /// Returns uVector4.bbbb swizzling (equivalent to uVector4.zzzz).
        /// </summary>
        public uVector4 bbbb => new uVector4(z, z, z, z);
        
        /// <summary>
        /// Returns uVector4.zzzw swizzling.
        /// </summary>
        public uVector4 zzzw => new uVector4(z, z, z, w);
        
        /// <summary>
        /// Returns uVector4.bbba swizzling (equivalent to uVector4.zzzw).
        /// </summary>
        public uVector4 bbba => new uVector4(z, z, z, w);
        
        /// <summary>
        /// Returns uVector4.zzw swizzling.
        /// </summary>
        public uVector3 zzw => new uVector3(z, z, w);
        
        /// <summary>
        /// Returns uVector4.bba swizzling (equivalent to uVector4.zzw).
        /// </summary>
        public uVector3 bba => new uVector3(z, z, w);
        
        /// <summary>
        /// Returns uVector4.zzwx swizzling.
        /// </summary>
        public uVector4 zzwx => new uVector4(z, z, w, x);
        
        /// <summary>
        /// Returns uVector4.bbar swizzling (equivalent to uVector4.zzwx).
        /// </summary>
        public uVector4 bbar => new uVector4(z, z, w, x);
        
        /// <summary>
        /// Returns uVector4.zzwy swizzling.
        /// </summary>
        public uVector4 zzwy => new uVector4(z, z, w, y);
        
        /// <summary>
        /// Returns uVector4.bbag swizzling (equivalent to uVector4.zzwy).
        /// </summary>
        public uVector4 bbag => new uVector4(z, z, w, y);
        
        /// <summary>
        /// Returns uVector4.zzwz swizzling.
        /// </summary>
        public uVector4 zzwz => new uVector4(z, z, w, z);
        
        /// <summary>
        /// Returns uVector4.bbab swizzling (equivalent to uVector4.zzwz).
        /// </summary>
        public uVector4 bbab => new uVector4(z, z, w, z);
        
        /// <summary>
        /// Returns uVector4.zzww swizzling.
        /// </summary>
        public uVector4 zzww => new uVector4(z, z, w, w);
        
        /// <summary>
        /// Returns uVector4.bbaa swizzling (equivalent to uVector4.zzww).
        /// </summary>
        public uVector4 bbaa => new uVector4(z, z, w, w);
        
        /// <summary>
        /// Returns uVector4.zw swizzling.
        /// </summary>
        public uVector2 zw => new uVector2(z, w);
        
        /// <summary>
        /// Returns uVector4.ba swizzling (equivalent to uVector4.zw).
        /// </summary>
        public uVector2 ba => new uVector2(z, w);
        
        /// <summary>
        /// Returns uVector4.zwx swizzling.
        /// </summary>
        public uVector3 zwx => new uVector3(z, w, x);
        
        /// <summary>
        /// Returns uVector4.bar swizzling (equivalent to uVector4.zwx).
        /// </summary>
        public uVector3 bar => new uVector3(z, w, x);
        
        /// <summary>
        /// Returns uVector4.zwxx swizzling.
        /// </summary>
        public uVector4 zwxx => new uVector4(z, w, x, x);
        
        /// <summary>
        /// Returns uVector4.barr swizzling (equivalent to uVector4.zwxx).
        /// </summary>
        public uVector4 barr => new uVector4(z, w, x, x);
        
        /// <summary>
        /// Returns uVector4.zwxy swizzling.
        /// </summary>
        public uVector4 zwxy => new uVector4(z, w, x, y);
        
        /// <summary>
        /// Returns uVector4.barg swizzling (equivalent to uVector4.zwxy).
        /// </summary>
        public uVector4 barg => new uVector4(z, w, x, y);
        
        /// <summary>
        /// Returns uVector4.zwxz swizzling.
        /// </summary>
        public uVector4 zwxz => new uVector4(z, w, x, z);
        
        /// <summary>
        /// Returns uVector4.barb swizzling (equivalent to uVector4.zwxz).
        /// </summary>
        public uVector4 barb => new uVector4(z, w, x, z);
        
        /// <summary>
        /// Returns uVector4.zwxw swizzling.
        /// </summary>
        public uVector4 zwxw => new uVector4(z, w, x, w);
        
        /// <summary>
        /// Returns uVector4.bara swizzling (equivalent to uVector4.zwxw).
        /// </summary>
        public uVector4 bara => new uVector4(z, w, x, w);
        
        /// <summary>
        /// Returns uVector4.zwy swizzling.
        /// </summary>
        public uVector3 zwy => new uVector3(z, w, y);
        
        /// <summary>
        /// Returns uVector4.bag swizzling (equivalent to uVector4.zwy).
        /// </summary>
        public uVector3 bag => new uVector3(z, w, y);
        
        /// <summary>
        /// Returns uVector4.zwyx swizzling.
        /// </summary>
        public uVector4 zwyx => new uVector4(z, w, y, x);
        
        /// <summary>
        /// Returns uVector4.bagr swizzling (equivalent to uVector4.zwyx).
        /// </summary>
        public uVector4 bagr => new uVector4(z, w, y, x);
        
        /// <summary>
        /// Returns uVector4.zwyy swizzling.
        /// </summary>
        public uVector4 zwyy => new uVector4(z, w, y, y);
        
        /// <summary>
        /// Returns uVector4.bagg swizzling (equivalent to uVector4.zwyy).
        /// </summary>
        public uVector4 bagg => new uVector4(z, w, y, y);
        
        /// <summary>
        /// Returns uVector4.zwyz swizzling.
        /// </summary>
        public uVector4 zwyz => new uVector4(z, w, y, z);
        
        /// <summary>
        /// Returns uVector4.bagb swizzling (equivalent to uVector4.zwyz).
        /// </summary>
        public uVector4 bagb => new uVector4(z, w, y, z);
        
        /// <summary>
        /// Returns uVector4.zwyw swizzling.
        /// </summary>
        public uVector4 zwyw => new uVector4(z, w, y, w);
        
        /// <summary>
        /// Returns uVector4.baga swizzling (equivalent to uVector4.zwyw).
        /// </summary>
        public uVector4 baga => new uVector4(z, w, y, w);
        
        /// <summary>
        /// Returns uVector4.zwz swizzling.
        /// </summary>
        public uVector3 zwz => new uVector3(z, w, z);
        
        /// <summary>
        /// Returns uVector4.bab swizzling (equivalent to uVector4.zwz).
        /// </summary>
        public uVector3 bab => new uVector3(z, w, z);
        
        /// <summary>
        /// Returns uVector4.zwzx swizzling.
        /// </summary>
        public uVector4 zwzx => new uVector4(z, w, z, x);
        
        /// <summary>
        /// Returns uVector4.babr swizzling (equivalent to uVector4.zwzx).
        /// </summary>
        public uVector4 babr => new uVector4(z, w, z, x);
        
        /// <summary>
        /// Returns uVector4.zwzy swizzling.
        /// </summary>
        public uVector4 zwzy => new uVector4(z, w, z, y);
        
        /// <summary>
        /// Returns uVector4.babg swizzling (equivalent to uVector4.zwzy).
        /// </summary>
        public uVector4 babg => new uVector4(z, w, z, y);
        
        /// <summary>
        /// Returns uVector4.zwzz swizzling.
        /// </summary>
        public uVector4 zwzz => new uVector4(z, w, z, z);
        
        /// <summary>
        /// Returns uVector4.babb swizzling (equivalent to uVector4.zwzz).
        /// </summary>
        public uVector4 babb => new uVector4(z, w, z, z);
        
        /// <summary>
        /// Returns uVector4.zwzw swizzling.
        /// </summary>
        public uVector4 zwzw => new uVector4(z, w, z, w);
        
        /// <summary>
        /// Returns uVector4.baba swizzling (equivalent to uVector4.zwzw).
        /// </summary>
        public uVector4 baba => new uVector4(z, w, z, w);
        
        /// <summary>
        /// Returns uVector4.zww swizzling.
        /// </summary>
        public uVector3 zww => new uVector3(z, w, w);
        
        /// <summary>
        /// Returns uVector4.baa swizzling (equivalent to uVector4.zww).
        /// </summary>
        public uVector3 baa => new uVector3(z, w, w);
        
        /// <summary>
        /// Returns uVector4.zwwx swizzling.
        /// </summary>
        public uVector4 zwwx => new uVector4(z, w, w, x);
        
        /// <summary>
        /// Returns uVector4.baar swizzling (equivalent to uVector4.zwwx).
        /// </summary>
        public uVector4 baar => new uVector4(z, w, w, x);
        
        /// <summary>
        /// Returns uVector4.zwwy swizzling.
        /// </summary>
        public uVector4 zwwy => new uVector4(z, w, w, y);
        
        /// <summary>
        /// Returns uVector4.baag swizzling (equivalent to uVector4.zwwy).
        /// </summary>
        public uVector4 baag => new uVector4(z, w, w, y);
        
        /// <summary>
        /// Returns uVector4.zwwz swizzling.
        /// </summary>
        public uVector4 zwwz => new uVector4(z, w, w, z);
        
        /// <summary>
        /// Returns uVector4.baab swizzling (equivalent to uVector4.zwwz).
        /// </summary>
        public uVector4 baab => new uVector4(z, w, w, z);
        
        /// <summary>
        /// Returns uVector4.zwww swizzling.
        /// </summary>
        public uVector4 zwww => new uVector4(z, w, w, w);
        
        /// <summary>
        /// Returns uVector4.baaa swizzling (equivalent to uVector4.zwww).
        /// </summary>
        public uVector4 baaa => new uVector4(z, w, w, w);
        
        /// <summary>
        /// Returns uVector4.wx swizzling.
        /// </summary>
        public uVector2 wx => new uVector2(w, x);
        
        /// <summary>
        /// Returns uVector4.ar swizzling (equivalent to uVector4.wx).
        /// </summary>
        public uVector2 ar => new uVector2(w, x);
        
        /// <summary>
        /// Returns uVector4.wxx swizzling.
        /// </summary>
        public uVector3 wxx => new uVector3(w, x, x);
        
        /// <summary>
        /// Returns uVector4.arr swizzling (equivalent to uVector4.wxx).
        /// </summary>
        public uVector3 arr => new uVector3(w, x, x);
        
        /// <summary>
        /// Returns uVector4.wxxx swizzling.
        /// </summary>
        public uVector4 wxxx => new uVector4(w, x, x, x);
        
        /// <summary>
        /// Returns uVector4.arrr swizzling (equivalent to uVector4.wxxx).
        /// </summary>
        public uVector4 arrr => new uVector4(w, x, x, x);
        
        /// <summary>
        /// Returns uVector4.wxxy swizzling.
        /// </summary>
        public uVector4 wxxy => new uVector4(w, x, x, y);
        
        /// <summary>
        /// Returns uVector4.arrg swizzling (equivalent to uVector4.wxxy).
        /// </summary>
        public uVector4 arrg => new uVector4(w, x, x, y);
        
        /// <summary>
        /// Returns uVector4.wxxz swizzling.
        /// </summary>
        public uVector4 wxxz => new uVector4(w, x, x, z);
        
        /// <summary>
        /// Returns uVector4.arrb swizzling (equivalent to uVector4.wxxz).
        /// </summary>
        public uVector4 arrb => new uVector4(w, x, x, z);
        
        /// <summary>
        /// Returns uVector4.wxxw swizzling.
        /// </summary>
        public uVector4 wxxw => new uVector4(w, x, x, w);
        
        /// <summary>
        /// Returns uVector4.arra swizzling (equivalent to uVector4.wxxw).
        /// </summary>
        public uVector4 arra => new uVector4(w, x, x, w);
        
        /// <summary>
        /// Returns uVector4.wxy swizzling.
        /// </summary>
        public uVector3 wxy => new uVector3(w, x, y);
        
        /// <summary>
        /// Returns uVector4.arg swizzling (equivalent to uVector4.wxy).
        /// </summary>
        public uVector3 arg => new uVector3(w, x, y);
        
        /// <summary>
        /// Returns uVector4.wxyx swizzling.
        /// </summary>
        public uVector4 wxyx => new uVector4(w, x, y, x);
        
        /// <summary>
        /// Returns uVector4.argr swizzling (equivalent to uVector4.wxyx).
        /// </summary>
        public uVector4 argr => new uVector4(w, x, y, x);
        
        /// <summary>
        /// Returns uVector4.wxyy swizzling.
        /// </summary>
        public uVector4 wxyy => new uVector4(w, x, y, y);
        
        /// <summary>
        /// Returns uVector4.argg swizzling (equivalent to uVector4.wxyy).
        /// </summary>
        public uVector4 argg => new uVector4(w, x, y, y);
        
        /// <summary>
        /// Returns uVector4.wxyz swizzling.
        /// </summary>
        public uVector4 wxyz => new uVector4(w, x, y, z);
        
        /// <summary>
        /// Returns uVector4.argb swizzling (equivalent to uVector4.wxyz).
        /// </summary>
        public uVector4 argb => new uVector4(w, x, y, z);
        
        /// <summary>
        /// Returns uVector4.wxyw swizzling.
        /// </summary>
        public uVector4 wxyw => new uVector4(w, x, y, w);
        
        /// <summary>
        /// Returns uVector4.arga swizzling (equivalent to uVector4.wxyw).
        /// </summary>
        public uVector4 arga => new uVector4(w, x, y, w);
        
        /// <summary>
        /// Returns uVector4.wxz swizzling.
        /// </summary>
        public uVector3 wxz => new uVector3(w, x, z);
        
        /// <summary>
        /// Returns uVector4.arb swizzling (equivalent to uVector4.wxz).
        /// </summary>
        public uVector3 arb => new uVector3(w, x, z);
        
        /// <summary>
        /// Returns uVector4.wxzx swizzling.
        /// </summary>
        public uVector4 wxzx => new uVector4(w, x, z, x);
        
        /// <summary>
        /// Returns uVector4.arbr swizzling (equivalent to uVector4.wxzx).
        /// </summary>
        public uVector4 arbr => new uVector4(w, x, z, x);
        
        /// <summary>
        /// Returns uVector4.wxzy swizzling.
        /// </summary>
        public uVector4 wxzy => new uVector4(w, x, z, y);
        
        /// <summary>
        /// Returns uVector4.arbg swizzling (equivalent to uVector4.wxzy).
        /// </summary>
        public uVector4 arbg => new uVector4(w, x, z, y);
        
        /// <summary>
        /// Returns uVector4.wxzz swizzling.
        /// </summary>
        public uVector4 wxzz => new uVector4(w, x, z, z);
        
        /// <summary>
        /// Returns uVector4.arbb swizzling (equivalent to uVector4.wxzz).
        /// </summary>
        public uVector4 arbb => new uVector4(w, x, z, z);
        
        /// <summary>
        /// Returns uVector4.wxzw swizzling.
        /// </summary>
        public uVector4 wxzw => new uVector4(w, x, z, w);
        
        /// <summary>
        /// Returns uVector4.arba swizzling (equivalent to uVector4.wxzw).
        /// </summary>
        public uVector4 arba => new uVector4(w, x, z, w);
        
        /// <summary>
        /// Returns uVector4.wxw swizzling.
        /// </summary>
        public uVector3 wxw => new uVector3(w, x, w);
        
        /// <summary>
        /// Returns uVector4.ara swizzling (equivalent to uVector4.wxw).
        /// </summary>
        public uVector3 ara => new uVector3(w, x, w);
        
        /// <summary>
        /// Returns uVector4.wxwx swizzling.
        /// </summary>
        public uVector4 wxwx => new uVector4(w, x, w, x);
        
        /// <summary>
        /// Returns uVector4.arar swizzling (equivalent to uVector4.wxwx).
        /// </summary>
        public uVector4 arar => new uVector4(w, x, w, x);
        
        /// <summary>
        /// Returns uVector4.wxwy swizzling.
        /// </summary>
        public uVector4 wxwy => new uVector4(w, x, w, y);
        
        /// <summary>
        /// Returns uVector4.arag swizzling (equivalent to uVector4.wxwy).
        /// </summary>
        public uVector4 arag => new uVector4(w, x, w, y);
        
        /// <summary>
        /// Returns uVector4.wxwz swizzling.
        /// </summary>
        public uVector4 wxwz => new uVector4(w, x, w, z);
        
        /// <summary>
        /// Returns uVector4.arab swizzling (equivalent to uVector4.wxwz).
        /// </summary>
        public uVector4 arab => new uVector4(w, x, w, z);
        
        /// <summary>
        /// Returns uVector4.wxww swizzling.
        /// </summary>
        public uVector4 wxww => new uVector4(w, x, w, w);
        
        /// <summary>
        /// Returns uVector4.araa swizzling (equivalent to uVector4.wxww).
        /// </summary>
        public uVector4 araa => new uVector4(w, x, w, w);
        
        /// <summary>
        /// Returns uVector4.wy swizzling.
        /// </summary>
        public uVector2 wy => new uVector2(w, y);
        
        /// <summary>
        /// Returns uVector4.ag swizzling (equivalent to uVector4.wy).
        /// </summary>
        public uVector2 ag => new uVector2(w, y);
        
        /// <summary>
        /// Returns uVector4.wyx swizzling.
        /// </summary>
        public uVector3 wyx => new uVector3(w, y, x);
        
        /// <summary>
        /// Returns uVector4.agr swizzling (equivalent to uVector4.wyx).
        /// </summary>
        public uVector3 agr => new uVector3(w, y, x);
        
        /// <summary>
        /// Returns uVector4.wyxx swizzling.
        /// </summary>
        public uVector4 wyxx => new uVector4(w, y, x, x);
        
        /// <summary>
        /// Returns uVector4.agrr swizzling (equivalent to uVector4.wyxx).
        /// </summary>
        public uVector4 agrr => new uVector4(w, y, x, x);
        
        /// <summary>
        /// Returns uVector4.wyxy swizzling.
        /// </summary>
        public uVector4 wyxy => new uVector4(w, y, x, y);
        
        /// <summary>
        /// Returns uVector4.agrg swizzling (equivalent to uVector4.wyxy).
        /// </summary>
        public uVector4 agrg => new uVector4(w, y, x, y);
        
        /// <summary>
        /// Returns uVector4.wyxz swizzling.
        /// </summary>
        public uVector4 wyxz => new uVector4(w, y, x, z);
        
        /// <summary>
        /// Returns uVector4.agrb swizzling (equivalent to uVector4.wyxz).
        /// </summary>
        public uVector4 agrb => new uVector4(w, y, x, z);
        
        /// <summary>
        /// Returns uVector4.wyxw swizzling.
        /// </summary>
        public uVector4 wyxw => new uVector4(w, y, x, w);
        
        /// <summary>
        /// Returns uVector4.agra swizzling (equivalent to uVector4.wyxw).
        /// </summary>
        public uVector4 agra => new uVector4(w, y, x, w);
        
        /// <summary>
        /// Returns uVector4.wyy swizzling.
        /// </summary>
        public uVector3 wyy => new uVector3(w, y, y);
        
        /// <summary>
        /// Returns uVector4.agg swizzling (equivalent to uVector4.wyy).
        /// </summary>
        public uVector3 agg => new uVector3(w, y, y);
        
        /// <summary>
        /// Returns uVector4.wyyx swizzling.
        /// </summary>
        public uVector4 wyyx => new uVector4(w, y, y, x);
        
        /// <summary>
        /// Returns uVector4.aggr swizzling (equivalent to uVector4.wyyx).
        /// </summary>
        public uVector4 aggr => new uVector4(w, y, y, x);
        
        /// <summary>
        /// Returns uVector4.wyyy swizzling.
        /// </summary>
        public uVector4 wyyy => new uVector4(w, y, y, y);
        
        /// <summary>
        /// Returns uVector4.aggg swizzling (equivalent to uVector4.wyyy).
        /// </summary>
        public uVector4 aggg => new uVector4(w, y, y, y);
        
        /// <summary>
        /// Returns uVector4.wyyz swizzling.
        /// </summary>
        public uVector4 wyyz => new uVector4(w, y, y, z);
        
        /// <summary>
        /// Returns uVector4.aggb swizzling (equivalent to uVector4.wyyz).
        /// </summary>
        public uVector4 aggb => new uVector4(w, y, y, z);
        
        /// <summary>
        /// Returns uVector4.wyyw swizzling.
        /// </summary>
        public uVector4 wyyw => new uVector4(w, y, y, w);
        
        /// <summary>
        /// Returns uVector4.agga swizzling (equivalent to uVector4.wyyw).
        /// </summary>
        public uVector4 agga => new uVector4(w, y, y, w);
        
        /// <summary>
        /// Returns uVector4.wyz swizzling.
        /// </summary>
        public uVector3 wyz => new uVector3(w, y, z);
        
        /// <summary>
        /// Returns uVector4.agb swizzling (equivalent to uVector4.wyz).
        /// </summary>
        public uVector3 agb => new uVector3(w, y, z);
        
        /// <summary>
        /// Returns uVector4.wyzx swizzling.
        /// </summary>
        public uVector4 wyzx => new uVector4(w, y, z, x);
        
        /// <summary>
        /// Returns uVector4.agbr swizzling (equivalent to uVector4.wyzx).
        /// </summary>
        public uVector4 agbr => new uVector4(w, y, z, x);
        
        /// <summary>
        /// Returns uVector4.wyzy swizzling.
        /// </summary>
        public uVector4 wyzy => new uVector4(w, y, z, y);
        
        /// <summary>
        /// Returns uVector4.agbg swizzling (equivalent to uVector4.wyzy).
        /// </summary>
        public uVector4 agbg => new uVector4(w, y, z, y);
        
        /// <summary>
        /// Returns uVector4.wyzz swizzling.
        /// </summary>
        public uVector4 wyzz => new uVector4(w, y, z, z);
        
        /// <summary>
        /// Returns uVector4.agbb swizzling (equivalent to uVector4.wyzz).
        /// </summary>
        public uVector4 agbb => new uVector4(w, y, z, z);
        
        /// <summary>
        /// Returns uVector4.wyzw swizzling.
        /// </summary>
        public uVector4 wyzw => new uVector4(w, y, z, w);
        
        /// <summary>
        /// Returns uVector4.agba swizzling (equivalent to uVector4.wyzw).
        /// </summary>
        public uVector4 agba => new uVector4(w, y, z, w);
        
        /// <summary>
        /// Returns uVector4.wyw swizzling.
        /// </summary>
        public uVector3 wyw => new uVector3(w, y, w);
        
        /// <summary>
        /// Returns uVector4.aga swizzling (equivalent to uVector4.wyw).
        /// </summary>
        public uVector3 aga => new uVector3(w, y, w);
        
        /// <summary>
        /// Returns uVector4.wywx swizzling.
        /// </summary>
        public uVector4 wywx => new uVector4(w, y, w, x);
        
        /// <summary>
        /// Returns uVector4.agar swizzling (equivalent to uVector4.wywx).
        /// </summary>
        public uVector4 agar => new uVector4(w, y, w, x);
        
        /// <summary>
        /// Returns uVector4.wywy swizzling.
        /// </summary>
        public uVector4 wywy => new uVector4(w, y, w, y);
        
        /// <summary>
        /// Returns uVector4.agag swizzling (equivalent to uVector4.wywy).
        /// </summary>
        public uVector4 agag => new uVector4(w, y, w, y);
        
        /// <summary>
        /// Returns uVector4.wywz swizzling.
        /// </summary>
        public uVector4 wywz => new uVector4(w, y, w, z);
        
        /// <summary>
        /// Returns uVector4.agab swizzling (equivalent to uVector4.wywz).
        /// </summary>
        public uVector4 agab => new uVector4(w, y, w, z);
        
        /// <summary>
        /// Returns uVector4.wyww swizzling.
        /// </summary>
        public uVector4 wyww => new uVector4(w, y, w, w);
        
        /// <summary>
        /// Returns uVector4.agaa swizzling (equivalent to uVector4.wyww).
        /// </summary>
        public uVector4 agaa => new uVector4(w, y, w, w);
        
        /// <summary>
        /// Returns uVector4.wz swizzling.
        /// </summary>
        public uVector2 wz => new uVector2(w, z);
        
        /// <summary>
        /// Returns uVector4.ab swizzling (equivalent to uVector4.wz).
        /// </summary>
        public uVector2 ab => new uVector2(w, z);
        
        /// <summary>
        /// Returns uVector4.wzx swizzling.
        /// </summary>
        public uVector3 wzx => new uVector3(w, z, x);
        
        /// <summary>
        /// Returns uVector4.abr swizzling (equivalent to uVector4.wzx).
        /// </summary>
        public uVector3 abr => new uVector3(w, z, x);
        
        /// <summary>
        /// Returns uVector4.wzxx swizzling.
        /// </summary>
        public uVector4 wzxx => new uVector4(w, z, x, x);
        
        /// <summary>
        /// Returns uVector4.abrr swizzling (equivalent to uVector4.wzxx).
        /// </summary>
        public uVector4 abrr => new uVector4(w, z, x, x);
        
        /// <summary>
        /// Returns uVector4.wzxy swizzling.
        /// </summary>
        public uVector4 wzxy => new uVector4(w, z, x, y);
        
        /// <summary>
        /// Returns uVector4.abrg swizzling (equivalent to uVector4.wzxy).
        /// </summary>
        public uVector4 abrg => new uVector4(w, z, x, y);
        
        /// <summary>
        /// Returns uVector4.wzxz swizzling.
        /// </summary>
        public uVector4 wzxz => new uVector4(w, z, x, z);
        
        /// <summary>
        /// Returns uVector4.abrb swizzling (equivalent to uVector4.wzxz).
        /// </summary>
        public uVector4 abrb => new uVector4(w, z, x, z);
        
        /// <summary>
        /// Returns uVector4.wzxw swizzling.
        /// </summary>
        public uVector4 wzxw => new uVector4(w, z, x, w);
        
        /// <summary>
        /// Returns uVector4.abra swizzling (equivalent to uVector4.wzxw).
        /// </summary>
        public uVector4 abra => new uVector4(w, z, x, w);
        
        /// <summary>
        /// Returns uVector4.wzy swizzling.
        /// </summary>
        public uVector3 wzy => new uVector3(w, z, y);
        
        /// <summary>
        /// Returns uVector4.abg swizzling (equivalent to uVector4.wzy).
        /// </summary>
        public uVector3 abg => new uVector3(w, z, y);
        
        /// <summary>
        /// Returns uVector4.wzyx swizzling.
        /// </summary>
        public uVector4 wzyx => new uVector4(w, z, y, x);
        
        /// <summary>
        /// Returns uVector4.abgr swizzling (equivalent to uVector4.wzyx).
        /// </summary>
        public uVector4 abgr => new uVector4(w, z, y, x);
        
        /// <summary>
        /// Returns uVector4.wzyy swizzling.
        /// </summary>
        public uVector4 wzyy => new uVector4(w, z, y, y);
        
        /// <summary>
        /// Returns uVector4.abgg swizzling (equivalent to uVector4.wzyy).
        /// </summary>
        public uVector4 abgg => new uVector4(w, z, y, y);
        
        /// <summary>
        /// Returns uVector4.wzyz swizzling.
        /// </summary>
        public uVector4 wzyz => new uVector4(w, z, y, z);
        
        /// <summary>
        /// Returns uVector4.abgb swizzling (equivalent to uVector4.wzyz).
        /// </summary>
        public uVector4 abgb => new uVector4(w, z, y, z);
        
        /// <summary>
        /// Returns uVector4.wzyw swizzling.
        /// </summary>
        public uVector4 wzyw => new uVector4(w, z, y, w);
        
        /// <summary>
        /// Returns uVector4.abga swizzling (equivalent to uVector4.wzyw).
        /// </summary>
        public uVector4 abga => new uVector4(w, z, y, w);
        
        /// <summary>
        /// Returns uVector4.wzz swizzling.
        /// </summary>
        public uVector3 wzz => new uVector3(w, z, z);
        
        /// <summary>
        /// Returns uVector4.abb swizzling (equivalent to uVector4.wzz).
        /// </summary>
        public uVector3 abb => new uVector3(w, z, z);
        
        /// <summary>
        /// Returns uVector4.wzzx swizzling.
        /// </summary>
        public uVector4 wzzx => new uVector4(w, z, z, x);
        
        /// <summary>
        /// Returns uVector4.abbr swizzling (equivalent to uVector4.wzzx).
        /// </summary>
        public uVector4 abbr => new uVector4(w, z, z, x);
        
        /// <summary>
        /// Returns uVector4.wzzy swizzling.
        /// </summary>
        public uVector4 wzzy => new uVector4(w, z, z, y);
        
        /// <summary>
        /// Returns uVector4.abbg swizzling (equivalent to uVector4.wzzy).
        /// </summary>
        public uVector4 abbg => new uVector4(w, z, z, y);
        
        /// <summary>
        /// Returns uVector4.wzzz swizzling.
        /// </summary>
        public uVector4 wzzz => new uVector4(w, z, z, z);
        
        /// <summary>
        /// Returns uVector4.abbb swizzling (equivalent to uVector4.wzzz).
        /// </summary>
        public uVector4 abbb => new uVector4(w, z, z, z);
        
        /// <summary>
        /// Returns uVector4.wzzw swizzling.
        /// </summary>
        public uVector4 wzzw => new uVector4(w, z, z, w);
        
        /// <summary>
        /// Returns uVector4.abba swizzling (equivalent to uVector4.wzzw).
        /// </summary>
        public uVector4 abba => new uVector4(w, z, z, w);
        
        /// <summary>
        /// Returns uVector4.wzw swizzling.
        /// </summary>
        public uVector3 wzw => new uVector3(w, z, w);
        
        /// <summary>
        /// Returns uVector4.aba swizzling (equivalent to uVector4.wzw).
        /// </summary>
        public uVector3 aba => new uVector3(w, z, w);
        
        /// <summary>
        /// Returns uVector4.wzwx swizzling.
        /// </summary>
        public uVector4 wzwx => new uVector4(w, z, w, x);
        
        /// <summary>
        /// Returns uVector4.abar swizzling (equivalent to uVector4.wzwx).
        /// </summary>
        public uVector4 abar => new uVector4(w, z, w, x);
        
        /// <summary>
        /// Returns uVector4.wzwy swizzling.
        /// </summary>
        public uVector4 wzwy => new uVector4(w, z, w, y);
        
        /// <summary>
        /// Returns uVector4.abag swizzling (equivalent to uVector4.wzwy).
        /// </summary>
        public uVector4 abag => new uVector4(w, z, w, y);
        
        /// <summary>
        /// Returns uVector4.wzwz swizzling.
        /// </summary>
        public uVector4 wzwz => new uVector4(w, z, w, z);
        
        /// <summary>
        /// Returns uVector4.abab swizzling (equivalent to uVector4.wzwz).
        /// </summary>
        public uVector4 abab => new uVector4(w, z, w, z);
        
        /// <summary>
        /// Returns uVector4.wzww swizzling.
        /// </summary>
        public uVector4 wzww => new uVector4(w, z, w, w);
        
        /// <summary>
        /// Returns uVector4.abaa swizzling (equivalent to uVector4.wzww).
        /// </summary>
        public uVector4 abaa => new uVector4(w, z, w, w);
        
        /// <summary>
        /// Returns uVector4.ww swizzling.
        /// </summary>
        public uVector2 ww => new uVector2(w, w);
        
        /// <summary>
        /// Returns uVector4.aa swizzling (equivalent to uVector4.ww).
        /// </summary>
        public uVector2 aa => new uVector2(w, w);
        
        /// <summary>
        /// Returns uVector4.wwx swizzling.
        /// </summary>
        public uVector3 wwx => new uVector3(w, w, x);
        
        /// <summary>
        /// Returns uVector4.aar swizzling (equivalent to uVector4.wwx).
        /// </summary>
        public uVector3 aar => new uVector3(w, w, x);
        
        /// <summary>
        /// Returns uVector4.wwxx swizzling.
        /// </summary>
        public uVector4 wwxx => new uVector4(w, w, x, x);
        
        /// <summary>
        /// Returns uVector4.aarr swizzling (equivalent to uVector4.wwxx).
        /// </summary>
        public uVector4 aarr => new uVector4(w, w, x, x);
        
        /// <summary>
        /// Returns uVector4.wwxy swizzling.
        /// </summary>
        public uVector4 wwxy => new uVector4(w, w, x, y);
        
        /// <summary>
        /// Returns uVector4.aarg swizzling (equivalent to uVector4.wwxy).
        /// </summary>
        public uVector4 aarg => new uVector4(w, w, x, y);
        
        /// <summary>
        /// Returns uVector4.wwxz swizzling.
        /// </summary>
        public uVector4 wwxz => new uVector4(w, w, x, z);
        
        /// <summary>
        /// Returns uVector4.aarb swizzling (equivalent to uVector4.wwxz).
        /// </summary>
        public uVector4 aarb => new uVector4(w, w, x, z);
        
        /// <summary>
        /// Returns uVector4.wwxw swizzling.
        /// </summary>
        public uVector4 wwxw => new uVector4(w, w, x, w);
        
        /// <summary>
        /// Returns uVector4.aara swizzling (equivalent to uVector4.wwxw).
        /// </summary>
        public uVector4 aara => new uVector4(w, w, x, w);
        
        /// <summary>
        /// Returns uVector4.wwy swizzling.
        /// </summary>
        public uVector3 wwy => new uVector3(w, w, y);
        
        /// <summary>
        /// Returns uVector4.aag swizzling (equivalent to uVector4.wwy).
        /// </summary>
        public uVector3 aag => new uVector3(w, w, y);
        
        /// <summary>
        /// Returns uVector4.wwyx swizzling.
        /// </summary>
        public uVector4 wwyx => new uVector4(w, w, y, x);
        
        /// <summary>
        /// Returns uVector4.aagr swizzling (equivalent to uVector4.wwyx).
        /// </summary>
        public uVector4 aagr => new uVector4(w, w, y, x);
        
        /// <summary>
        /// Returns uVector4.wwyy swizzling.
        /// </summary>
        public uVector4 wwyy => new uVector4(w, w, y, y);
        
        /// <summary>
        /// Returns uVector4.aagg swizzling (equivalent to uVector4.wwyy).
        /// </summary>
        public uVector4 aagg => new uVector4(w, w, y, y);
        
        /// <summary>
        /// Returns uVector4.wwyz swizzling.
        /// </summary>
        public uVector4 wwyz => new uVector4(w, w, y, z);
        
        /// <summary>
        /// Returns uVector4.aagb swizzling (equivalent to uVector4.wwyz).
        /// </summary>
        public uVector4 aagb => new uVector4(w, w, y, z);
        
        /// <summary>
        /// Returns uVector4.wwyw swizzling.
        /// </summary>
        public uVector4 wwyw => new uVector4(w, w, y, w);
        
        /// <summary>
        /// Returns uVector4.aaga swizzling (equivalent to uVector4.wwyw).
        /// </summary>
        public uVector4 aaga => new uVector4(w, w, y, w);
        
        /// <summary>
        /// Returns uVector4.wwz swizzling.
        /// </summary>
        public uVector3 wwz => new uVector3(w, w, z);
        
        /// <summary>
        /// Returns uVector4.aab swizzling (equivalent to uVector4.wwz).
        /// </summary>
        public uVector3 aab => new uVector3(w, w, z);
        
        /// <summary>
        /// Returns uVector4.wwzx swizzling.
        /// </summary>
        public uVector4 wwzx => new uVector4(w, w, z, x);
        
        /// <summary>
        /// Returns uVector4.aabr swizzling (equivalent to uVector4.wwzx).
        /// </summary>
        public uVector4 aabr => new uVector4(w, w, z, x);
        
        /// <summary>
        /// Returns uVector4.wwzy swizzling.
        /// </summary>
        public uVector4 wwzy => new uVector4(w, w, z, y);
        
        /// <summary>
        /// Returns uVector4.aabg swizzling (equivalent to uVector4.wwzy).
        /// </summary>
        public uVector4 aabg => new uVector4(w, w, z, y);
        
        /// <summary>
        /// Returns uVector4.wwzz swizzling.
        /// </summary>
        public uVector4 wwzz => new uVector4(w, w, z, z);
        
        /// <summary>
        /// Returns uVector4.aabb swizzling (equivalent to uVector4.wwzz).
        /// </summary>
        public uVector4 aabb => new uVector4(w, w, z, z);
        
        /// <summary>
        /// Returns uVector4.wwzw swizzling.
        /// </summary>
        public uVector4 wwzw => new uVector4(w, w, z, w);
        
        /// <summary>
        /// Returns uVector4.aaba swizzling (equivalent to uVector4.wwzw).
        /// </summary>
        public uVector4 aaba => new uVector4(w, w, z, w);
        
        /// <summary>
        /// Returns uVector4.www swizzling.
        /// </summary>
        public uVector3 www => new uVector3(w, w, w);
        
        /// <summary>
        /// Returns uVector4.aaa swizzling (equivalent to uVector4.www).
        /// </summary>
        public uVector3 aaa => new uVector3(w, w, w);
        
        /// <summary>
        /// Returns uVector4.wwwx swizzling.
        /// </summary>
        public uVector4 wwwx => new uVector4(w, w, w, x);
        
        /// <summary>
        /// Returns uVector4.aaar swizzling (equivalent to uVector4.wwwx).
        /// </summary>
        public uVector4 aaar => new uVector4(w, w, w, x);
        
        /// <summary>
        /// Returns uVector4.wwwy swizzling.
        /// </summary>
        public uVector4 wwwy => new uVector4(w, w, w, y);
        
        /// <summary>
        /// Returns uVector4.aaag swizzling (equivalent to uVector4.wwwy).
        /// </summary>
        public uVector4 aaag => new uVector4(w, w, w, y);
        
        /// <summary>
        /// Returns uVector4.wwwz swizzling.
        /// </summary>
        public uVector4 wwwz => new uVector4(w, w, w, z);
        
        /// <summary>
        /// Returns uVector4.aaab swizzling (equivalent to uVector4.wwwz).
        /// </summary>
        public uVector4 aaab => new uVector4(w, w, w, z);
        
        /// <summary>
        /// Returns uVector4.wwww swizzling.
        /// </summary>
        public uVector4 wwww => new uVector4(w, w, w, w);
        
        /// <summary>
        /// Returns uVector4.aaaa swizzling (equivalent to uVector4.wwww).
        /// </summary>
        public uVector4 aaaa => new uVector4(w, w, w, w);

        #endregion

    }
}
