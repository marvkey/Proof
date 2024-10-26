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
    /// Temporary Vector of type T with 4 components, used for implementing swizzling for gVector4.
    /// </summary>
    
   
    [StructLayout(LayoutKind.Sequential)]
    public struct swizzle_gVector4<T>
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
        
        /// <summary>
        /// w-component
        /// </summary>
        
        internal readonly T w;

        #endregion


        #region Constructors
        
        /// <summary>
        /// Constructor for swizzle_gVector4.
        /// </summary>
        internal swizzle_gVector4(T x, T y, T z, T w)
        {
            this.x = x;
            this.y = y;
            this.z = z;
            this.w = w;
        }

        #endregion


        #region Properties
        
        /// <summary>
        /// Returns gVector4.xx swizzling.
        /// </summary>
        public gVector2<T> xx => new gVector2<T>(x, x);
        
        /// <summary>
        /// Returns gVector4.rr swizzling (equivalent to gVector4.xx).
        /// </summary>
        public gVector2<T> rr => new gVector2<T>(x, x);
        
        /// <summary>
        /// Returns gVector4.xxx swizzling.
        /// </summary>
        public gVector3<T> xxx => new gVector3<T>(x, x, x);
        
        /// <summary>
        /// Returns gVector4.rrr swizzling (equivalent to gVector4.xxx).
        /// </summary>
        public gVector3<T> rrr => new gVector3<T>(x, x, x);
        
        /// <summary>
        /// Returns gVector4.xxxx swizzling.
        /// </summary>
        public gVector4<T> xxxx => new gVector4<T>(x, x, x, x);
        
        /// <summary>
        /// Returns gVector4.rrrr swizzling (equivalent to gVector4.xxxx).
        /// </summary>
        public gVector4<T> rrrr => new gVector4<T>(x, x, x, x);
        
        /// <summary>
        /// Returns gVector4.xxxy swizzling.
        /// </summary>
        public gVector4<T> xxxy => new gVector4<T>(x, x, x, y);
        
        /// <summary>
        /// Returns gVector4.rrrg swizzling (equivalent to gVector4.xxxy).
        /// </summary>
        public gVector4<T> rrrg => new gVector4<T>(x, x, x, y);
        
        /// <summary>
        /// Returns gVector4.xxxz swizzling.
        /// </summary>
        public gVector4<T> xxxz => new gVector4<T>(x, x, x, z);
        
        /// <summary>
        /// Returns gVector4.rrrb swizzling (equivalent to gVector4.xxxz).
        /// </summary>
        public gVector4<T> rrrb => new gVector4<T>(x, x, x, z);
        
        /// <summary>
        /// Returns gVector4.xxxw swizzling.
        /// </summary>
        public gVector4<T> xxxw => new gVector4<T>(x, x, x, w);
        
        /// <summary>
        /// Returns gVector4.rrra swizzling (equivalent to gVector4.xxxw).
        /// </summary>
        public gVector4<T> rrra => new gVector4<T>(x, x, x, w);
        
        /// <summary>
        /// Returns gVector4.xxy swizzling.
        /// </summary>
        public gVector3<T> xxy => new gVector3<T>(x, x, y);
        
        /// <summary>
        /// Returns gVector4.rrg swizzling (equivalent to gVector4.xxy).
        /// </summary>
        public gVector3<T> rrg => new gVector3<T>(x, x, y);
        
        /// <summary>
        /// Returns gVector4.xxyx swizzling.
        /// </summary>
        public gVector4<T> xxyx => new gVector4<T>(x, x, y, x);
        
        /// <summary>
        /// Returns gVector4.rrgr swizzling (equivalent to gVector4.xxyx).
        /// </summary>
        public gVector4<T> rrgr => new gVector4<T>(x, x, y, x);
        
        /// <summary>
        /// Returns gVector4.xxyy swizzling.
        /// </summary>
        public gVector4<T> xxyy => new gVector4<T>(x, x, y, y);
        
        /// <summary>
        /// Returns gVector4.rrgg swizzling (equivalent to gVector4.xxyy).
        /// </summary>
        public gVector4<T> rrgg => new gVector4<T>(x, x, y, y);
        
        /// <summary>
        /// Returns gVector4.xxyz swizzling.
        /// </summary>
        public gVector4<T> xxyz => new gVector4<T>(x, x, y, z);
        
        /// <summary>
        /// Returns gVector4.rrgb swizzling (equivalent to gVector4.xxyz).
        /// </summary>
        public gVector4<T> rrgb => new gVector4<T>(x, x, y, z);
        
        /// <summary>
        /// Returns gVector4.xxyw swizzling.
        /// </summary>
        public gVector4<T> xxyw => new gVector4<T>(x, x, y, w);
        
        /// <summary>
        /// Returns gVector4.rrga swizzling (equivalent to gVector4.xxyw).
        /// </summary>
        public gVector4<T> rrga => new gVector4<T>(x, x, y, w);
        
        /// <summary>
        /// Returns gVector4.xxz swizzling.
        /// </summary>
        public gVector3<T> xxz => new gVector3<T>(x, x, z);
        
        /// <summary>
        /// Returns gVector4.rrb swizzling (equivalent to gVector4.xxz).
        /// </summary>
        public gVector3<T> rrb => new gVector3<T>(x, x, z);
        
        /// <summary>
        /// Returns gVector4.xxzx swizzling.
        /// </summary>
        public gVector4<T> xxzx => new gVector4<T>(x, x, z, x);
        
        /// <summary>
        /// Returns gVector4.rrbr swizzling (equivalent to gVector4.xxzx).
        /// </summary>
        public gVector4<T> rrbr => new gVector4<T>(x, x, z, x);
        
        /// <summary>
        /// Returns gVector4.xxzy swizzling.
        /// </summary>
        public gVector4<T> xxzy => new gVector4<T>(x, x, z, y);
        
        /// <summary>
        /// Returns gVector4.rrbg swizzling (equivalent to gVector4.xxzy).
        /// </summary>
        public gVector4<T> rrbg => new gVector4<T>(x, x, z, y);
        
        /// <summary>
        /// Returns gVector4.xxzz swizzling.
        /// </summary>
        public gVector4<T> xxzz => new gVector4<T>(x, x, z, z);
        
        /// <summary>
        /// Returns gVector4.rrbb swizzling (equivalent to gVector4.xxzz).
        /// </summary>
        public gVector4<T> rrbb => new gVector4<T>(x, x, z, z);
        
        /// <summary>
        /// Returns gVector4.xxzw swizzling.
        /// </summary>
        public gVector4<T> xxzw => new gVector4<T>(x, x, z, w);
        
        /// <summary>
        /// Returns gVector4.rrba swizzling (equivalent to gVector4.xxzw).
        /// </summary>
        public gVector4<T> rrba => new gVector4<T>(x, x, z, w);
        
        /// <summary>
        /// Returns gVector4.xxw swizzling.
        /// </summary>
        public gVector3<T> xxw => new gVector3<T>(x, x, w);
        
        /// <summary>
        /// Returns gVector4.rra swizzling (equivalent to gVector4.xxw).
        /// </summary>
        public gVector3<T> rra => new gVector3<T>(x, x, w);
        
        /// <summary>
        /// Returns gVector4.xxwx swizzling.
        /// </summary>
        public gVector4<T> xxwx => new gVector4<T>(x, x, w, x);
        
        /// <summary>
        /// Returns gVector4.rrar swizzling (equivalent to gVector4.xxwx).
        /// </summary>
        public gVector4<T> rrar => new gVector4<T>(x, x, w, x);
        
        /// <summary>
        /// Returns gVector4.xxwy swizzling.
        /// </summary>
        public gVector4<T> xxwy => new gVector4<T>(x, x, w, y);
        
        /// <summary>
        /// Returns gVector4.rrag swizzling (equivalent to gVector4.xxwy).
        /// </summary>
        public gVector4<T> rrag => new gVector4<T>(x, x, w, y);
        
        /// <summary>
        /// Returns gVector4.xxwz swizzling.
        /// </summary>
        public gVector4<T> xxwz => new gVector4<T>(x, x, w, z);
        
        /// <summary>
        /// Returns gVector4.rrab swizzling (equivalent to gVector4.xxwz).
        /// </summary>
        public gVector4<T> rrab => new gVector4<T>(x, x, w, z);
        
        /// <summary>
        /// Returns gVector4.xxww swizzling.
        /// </summary>
        public gVector4<T> xxww => new gVector4<T>(x, x, w, w);
        
        /// <summary>
        /// Returns gVector4.rraa swizzling (equivalent to gVector4.xxww).
        /// </summary>
        public gVector4<T> rraa => new gVector4<T>(x, x, w, w);
        
        /// <summary>
        /// Returns gVector4.xy swizzling.
        /// </summary>
        public gVector2<T> xy => new gVector2<T>(x, y);
        
        /// <summary>
        /// Returns gVector4.rg swizzling (equivalent to gVector4.xy).
        /// </summary>
        public gVector2<T> rg => new gVector2<T>(x, y);
        
        /// <summary>
        /// Returns gVector4.xyx swizzling.
        /// </summary>
        public gVector3<T> xyx => new gVector3<T>(x, y, x);
        
        /// <summary>
        /// Returns gVector4.rgr swizzling (equivalent to gVector4.xyx).
        /// </summary>
        public gVector3<T> rgr => new gVector3<T>(x, y, x);
        
        /// <summary>
        /// Returns gVector4.xyxx swizzling.
        /// </summary>
        public gVector4<T> xyxx => new gVector4<T>(x, y, x, x);
        
        /// <summary>
        /// Returns gVector4.rgrr swizzling (equivalent to gVector4.xyxx).
        /// </summary>
        public gVector4<T> rgrr => new gVector4<T>(x, y, x, x);
        
        /// <summary>
        /// Returns gVector4.xyxy swizzling.
        /// </summary>
        public gVector4<T> xyxy => new gVector4<T>(x, y, x, y);
        
        /// <summary>
        /// Returns gVector4.rgrg swizzling (equivalent to gVector4.xyxy).
        /// </summary>
        public gVector4<T> rgrg => new gVector4<T>(x, y, x, y);
        
        /// <summary>
        /// Returns gVector4.xyxz swizzling.
        /// </summary>
        public gVector4<T> xyxz => new gVector4<T>(x, y, x, z);
        
        /// <summary>
        /// Returns gVector4.rgrb swizzling (equivalent to gVector4.xyxz).
        /// </summary>
        public gVector4<T> rgrb => new gVector4<T>(x, y, x, z);
        
        /// <summary>
        /// Returns gVector4.xyxw swizzling.
        /// </summary>
        public gVector4<T> xyxw => new gVector4<T>(x, y, x, w);
        
        /// <summary>
        /// Returns gVector4.rgra swizzling (equivalent to gVector4.xyxw).
        /// </summary>
        public gVector4<T> rgra => new gVector4<T>(x, y, x, w);
        
        /// <summary>
        /// Returns gVector4.xyy swizzling.
        /// </summary>
        public gVector3<T> xyy => new gVector3<T>(x, y, y);
        
        /// <summary>
        /// Returns gVector4.rgg swizzling (equivalent to gVector4.xyy).
        /// </summary>
        public gVector3<T> rgg => new gVector3<T>(x, y, y);
        
        /// <summary>
        /// Returns gVector4.xyyx swizzling.
        /// </summary>
        public gVector4<T> xyyx => new gVector4<T>(x, y, y, x);
        
        /// <summary>
        /// Returns gVector4.rggr swizzling (equivalent to gVector4.xyyx).
        /// </summary>
        public gVector4<T> rggr => new gVector4<T>(x, y, y, x);
        
        /// <summary>
        /// Returns gVector4.xyyy swizzling.
        /// </summary>
        public gVector4<T> xyyy => new gVector4<T>(x, y, y, y);
        
        /// <summary>
        /// Returns gVector4.rggg swizzling (equivalent to gVector4.xyyy).
        /// </summary>
        public gVector4<T> rggg => new gVector4<T>(x, y, y, y);
        
        /// <summary>
        /// Returns gVector4.xyyz swizzling.
        /// </summary>
        public gVector4<T> xyyz => new gVector4<T>(x, y, y, z);
        
        /// <summary>
        /// Returns gVector4.rggb swizzling (equivalent to gVector4.xyyz).
        /// </summary>
        public gVector4<T> rggb => new gVector4<T>(x, y, y, z);
        
        /// <summary>
        /// Returns gVector4.xyyw swizzling.
        /// </summary>
        public gVector4<T> xyyw => new gVector4<T>(x, y, y, w);
        
        /// <summary>
        /// Returns gVector4.rgga swizzling (equivalent to gVector4.xyyw).
        /// </summary>
        public gVector4<T> rgga => new gVector4<T>(x, y, y, w);
        
        /// <summary>
        /// Returns gVector4.xyz swizzling.
        /// </summary>
        public gVector3<T> xyz => new gVector3<T>(x, y, z);
        
        /// <summary>
        /// Returns gVector4.rgb swizzling (equivalent to gVector4.xyz).
        /// </summary>
        public gVector3<T> rgb => new gVector3<T>(x, y, z);
        
        /// <summary>
        /// Returns gVector4.xyzx swizzling.
        /// </summary>
        public gVector4<T> xyzx => new gVector4<T>(x, y, z, x);
        
        /// <summary>
        /// Returns gVector4.rgbr swizzling (equivalent to gVector4.xyzx).
        /// </summary>
        public gVector4<T> rgbr => new gVector4<T>(x, y, z, x);
        
        /// <summary>
        /// Returns gVector4.xyzy swizzling.
        /// </summary>
        public gVector4<T> xyzy => new gVector4<T>(x, y, z, y);
        
        /// <summary>
        /// Returns gVector4.rgbg swizzling (equivalent to gVector4.xyzy).
        /// </summary>
        public gVector4<T> rgbg => new gVector4<T>(x, y, z, y);
        
        /// <summary>
        /// Returns gVector4.xyzz swizzling.
        /// </summary>
        public gVector4<T> xyzz => new gVector4<T>(x, y, z, z);
        
        /// <summary>
        /// Returns gVector4.rgbb swizzling (equivalent to gVector4.xyzz).
        /// </summary>
        public gVector4<T> rgbb => new gVector4<T>(x, y, z, z);
        
        /// <summary>
        /// Returns gVector4.xyzw swizzling.
        /// </summary>
        public gVector4<T> xyzw => new gVector4<T>(x, y, z, w);
        
        /// <summary>
        /// Returns gVector4.rgba swizzling (equivalent to gVector4.xyzw).
        /// </summary>
        public gVector4<T> rgba => new gVector4<T>(x, y, z, w);
        
        /// <summary>
        /// Returns gVector4.xyw swizzling.
        /// </summary>
        public gVector3<T> xyw => new gVector3<T>(x, y, w);
        
        /// <summary>
        /// Returns gVector4.rga swizzling (equivalent to gVector4.xyw).
        /// </summary>
        public gVector3<T> rga => new gVector3<T>(x, y, w);
        
        /// <summary>
        /// Returns gVector4.xywx swizzling.
        /// </summary>
        public gVector4<T> xywx => new gVector4<T>(x, y, w, x);
        
        /// <summary>
        /// Returns gVector4.rgar swizzling (equivalent to gVector4.xywx).
        /// </summary>
        public gVector4<T> rgar => new gVector4<T>(x, y, w, x);
        
        /// <summary>
        /// Returns gVector4.xywy swizzling.
        /// </summary>
        public gVector4<T> xywy => new gVector4<T>(x, y, w, y);
        
        /// <summary>
        /// Returns gVector4.rgag swizzling (equivalent to gVector4.xywy).
        /// </summary>
        public gVector4<T> rgag => new gVector4<T>(x, y, w, y);
        
        /// <summary>
        /// Returns gVector4.xywz swizzling.
        /// </summary>
        public gVector4<T> xywz => new gVector4<T>(x, y, w, z);
        
        /// <summary>
        /// Returns gVector4.rgab swizzling (equivalent to gVector4.xywz).
        /// </summary>
        public gVector4<T> rgab => new gVector4<T>(x, y, w, z);
        
        /// <summary>
        /// Returns gVector4.xyww swizzling.
        /// </summary>
        public gVector4<T> xyww => new gVector4<T>(x, y, w, w);
        
        /// <summary>
        /// Returns gVector4.rgaa swizzling (equivalent to gVector4.xyww).
        /// </summary>
        public gVector4<T> rgaa => new gVector4<T>(x, y, w, w);
        
        /// <summary>
        /// Returns gVector4.xz swizzling.
        /// </summary>
        public gVector2<T> xz => new gVector2<T>(x, z);
        
        /// <summary>
        /// Returns gVector4.rb swizzling (equivalent to gVector4.xz).
        /// </summary>
        public gVector2<T> rb => new gVector2<T>(x, z);
        
        /// <summary>
        /// Returns gVector4.xzx swizzling.
        /// </summary>
        public gVector3<T> xzx => new gVector3<T>(x, z, x);
        
        /// <summary>
        /// Returns gVector4.rbr swizzling (equivalent to gVector4.xzx).
        /// </summary>
        public gVector3<T> rbr => new gVector3<T>(x, z, x);
        
        /// <summary>
        /// Returns gVector4.xzxx swizzling.
        /// </summary>
        public gVector4<T> xzxx => new gVector4<T>(x, z, x, x);
        
        /// <summary>
        /// Returns gVector4.rbrr swizzling (equivalent to gVector4.xzxx).
        /// </summary>
        public gVector4<T> rbrr => new gVector4<T>(x, z, x, x);
        
        /// <summary>
        /// Returns gVector4.xzxy swizzling.
        /// </summary>
        public gVector4<T> xzxy => new gVector4<T>(x, z, x, y);
        
        /// <summary>
        /// Returns gVector4.rbrg swizzling (equivalent to gVector4.xzxy).
        /// </summary>
        public gVector4<T> rbrg => new gVector4<T>(x, z, x, y);
        
        /// <summary>
        /// Returns gVector4.xzxz swizzling.
        /// </summary>
        public gVector4<T> xzxz => new gVector4<T>(x, z, x, z);
        
        /// <summary>
        /// Returns gVector4.rbrb swizzling (equivalent to gVector4.xzxz).
        /// </summary>
        public gVector4<T> rbrb => new gVector4<T>(x, z, x, z);
        
        /// <summary>
        /// Returns gVector4.xzxw swizzling.
        /// </summary>
        public gVector4<T> xzxw => new gVector4<T>(x, z, x, w);
        
        /// <summary>
        /// Returns gVector4.rbra swizzling (equivalent to gVector4.xzxw).
        /// </summary>
        public gVector4<T> rbra => new gVector4<T>(x, z, x, w);
        
        /// <summary>
        /// Returns gVector4.xzy swizzling.
        /// </summary>
        public gVector3<T> xzy => new gVector3<T>(x, z, y);
        
        /// <summary>
        /// Returns gVector4.rbg swizzling (equivalent to gVector4.xzy).
        /// </summary>
        public gVector3<T> rbg => new gVector3<T>(x, z, y);
        
        /// <summary>
        /// Returns gVector4.xzyx swizzling.
        /// </summary>
        public gVector4<T> xzyx => new gVector4<T>(x, z, y, x);
        
        /// <summary>
        /// Returns gVector4.rbgr swizzling (equivalent to gVector4.xzyx).
        /// </summary>
        public gVector4<T> rbgr => new gVector4<T>(x, z, y, x);
        
        /// <summary>
        /// Returns gVector4.xzyy swizzling.
        /// </summary>
        public gVector4<T> xzyy => new gVector4<T>(x, z, y, y);
        
        /// <summary>
        /// Returns gVector4.rbgg swizzling (equivalent to gVector4.xzyy).
        /// </summary>
        public gVector4<T> rbgg => new gVector4<T>(x, z, y, y);
        
        /// <summary>
        /// Returns gVector4.xzyz swizzling.
        /// </summary>
        public gVector4<T> xzyz => new gVector4<T>(x, z, y, z);
        
        /// <summary>
        /// Returns gVector4.rbgb swizzling (equivalent to gVector4.xzyz).
        /// </summary>
        public gVector4<T> rbgb => new gVector4<T>(x, z, y, z);
        
        /// <summary>
        /// Returns gVector4.xzyw swizzling.
        /// </summary>
        public gVector4<T> xzyw => new gVector4<T>(x, z, y, w);
        
        /// <summary>
        /// Returns gVector4.rbga swizzling (equivalent to gVector4.xzyw).
        /// </summary>
        public gVector4<T> rbga => new gVector4<T>(x, z, y, w);
        
        /// <summary>
        /// Returns gVector4.xzz swizzling.
        /// </summary>
        public gVector3<T> xzz => new gVector3<T>(x, z, z);
        
        /// <summary>
        /// Returns gVector4.rbb swizzling (equivalent to gVector4.xzz).
        /// </summary>
        public gVector3<T> rbb => new gVector3<T>(x, z, z);
        
        /// <summary>
        /// Returns gVector4.xzzx swizzling.
        /// </summary>
        public gVector4<T> xzzx => new gVector4<T>(x, z, z, x);
        
        /// <summary>
        /// Returns gVector4.rbbr swizzling (equivalent to gVector4.xzzx).
        /// </summary>
        public gVector4<T> rbbr => new gVector4<T>(x, z, z, x);
        
        /// <summary>
        /// Returns gVector4.xzzy swizzling.
        /// </summary>
        public gVector4<T> xzzy => new gVector4<T>(x, z, z, y);
        
        /// <summary>
        /// Returns gVector4.rbbg swizzling (equivalent to gVector4.xzzy).
        /// </summary>
        public gVector4<T> rbbg => new gVector4<T>(x, z, z, y);
        
        /// <summary>
        /// Returns gVector4.xzzz swizzling.
        /// </summary>
        public gVector4<T> xzzz => new gVector4<T>(x, z, z, z);
        
        /// <summary>
        /// Returns gVector4.rbbb swizzling (equivalent to gVector4.xzzz).
        /// </summary>
        public gVector4<T> rbbb => new gVector4<T>(x, z, z, z);
        
        /// <summary>
        /// Returns gVector4.xzzw swizzling.
        /// </summary>
        public gVector4<T> xzzw => new gVector4<T>(x, z, z, w);
        
        /// <summary>
        /// Returns gVector4.rbba swizzling (equivalent to gVector4.xzzw).
        /// </summary>
        public gVector4<T> rbba => new gVector4<T>(x, z, z, w);
        
        /// <summary>
        /// Returns gVector4.xzw swizzling.
        /// </summary>
        public gVector3<T> xzw => new gVector3<T>(x, z, w);
        
        /// <summary>
        /// Returns gVector4.rba swizzling (equivalent to gVector4.xzw).
        /// </summary>
        public gVector3<T> rba => new gVector3<T>(x, z, w);
        
        /// <summary>
        /// Returns gVector4.xzwx swizzling.
        /// </summary>
        public gVector4<T> xzwx => new gVector4<T>(x, z, w, x);
        
        /// <summary>
        /// Returns gVector4.rbar swizzling (equivalent to gVector4.xzwx).
        /// </summary>
        public gVector4<T> rbar => new gVector4<T>(x, z, w, x);
        
        /// <summary>
        /// Returns gVector4.xzwy swizzling.
        /// </summary>
        public gVector4<T> xzwy => new gVector4<T>(x, z, w, y);
        
        /// <summary>
        /// Returns gVector4.rbag swizzling (equivalent to gVector4.xzwy).
        /// </summary>
        public gVector4<T> rbag => new gVector4<T>(x, z, w, y);
        
        /// <summary>
        /// Returns gVector4.xzwz swizzling.
        /// </summary>
        public gVector4<T> xzwz => new gVector4<T>(x, z, w, z);
        
        /// <summary>
        /// Returns gVector4.rbab swizzling (equivalent to gVector4.xzwz).
        /// </summary>
        public gVector4<T> rbab => new gVector4<T>(x, z, w, z);
        
        /// <summary>
        /// Returns gVector4.xzww swizzling.
        /// </summary>
        public gVector4<T> xzww => new gVector4<T>(x, z, w, w);
        
        /// <summary>
        /// Returns gVector4.rbaa swizzling (equivalent to gVector4.xzww).
        /// </summary>
        public gVector4<T> rbaa => new gVector4<T>(x, z, w, w);
        
        /// <summary>
        /// Returns gVector4.xw swizzling.
        /// </summary>
        public gVector2<T> xw => new gVector2<T>(x, w);
        
        /// <summary>
        /// Returns gVector4.ra swizzling (equivalent to gVector4.xw).
        /// </summary>
        public gVector2<T> ra => new gVector2<T>(x, w);
        
        /// <summary>
        /// Returns gVector4.xwx swizzling.
        /// </summary>
        public gVector3<T> xwx => new gVector3<T>(x, w, x);
        
        /// <summary>
        /// Returns gVector4.rar swizzling (equivalent to gVector4.xwx).
        /// </summary>
        public gVector3<T> rar => new gVector3<T>(x, w, x);
        
        /// <summary>
        /// Returns gVector4.xwxx swizzling.
        /// </summary>
        public gVector4<T> xwxx => new gVector4<T>(x, w, x, x);
        
        /// <summary>
        /// Returns gVector4.rarr swizzling (equivalent to gVector4.xwxx).
        /// </summary>
        public gVector4<T> rarr => new gVector4<T>(x, w, x, x);
        
        /// <summary>
        /// Returns gVector4.xwxy swizzling.
        /// </summary>
        public gVector4<T> xwxy => new gVector4<T>(x, w, x, y);
        
        /// <summary>
        /// Returns gVector4.rarg swizzling (equivalent to gVector4.xwxy).
        /// </summary>
        public gVector4<T> rarg => new gVector4<T>(x, w, x, y);
        
        /// <summary>
        /// Returns gVector4.xwxz swizzling.
        /// </summary>
        public gVector4<T> xwxz => new gVector4<T>(x, w, x, z);
        
        /// <summary>
        /// Returns gVector4.rarb swizzling (equivalent to gVector4.xwxz).
        /// </summary>
        public gVector4<T> rarb => new gVector4<T>(x, w, x, z);
        
        /// <summary>
        /// Returns gVector4.xwxw swizzling.
        /// </summary>
        public gVector4<T> xwxw => new gVector4<T>(x, w, x, w);
        
        /// <summary>
        /// Returns gVector4.rara swizzling (equivalent to gVector4.xwxw).
        /// </summary>
        public gVector4<T> rara => new gVector4<T>(x, w, x, w);
        
        /// <summary>
        /// Returns gVector4.xwy swizzling.
        /// </summary>
        public gVector3<T> xwy => new gVector3<T>(x, w, y);
        
        /// <summary>
        /// Returns gVector4.rag swizzling (equivalent to gVector4.xwy).
        /// </summary>
        public gVector3<T> rag => new gVector3<T>(x, w, y);
        
        /// <summary>
        /// Returns gVector4.xwyx swizzling.
        /// </summary>
        public gVector4<T> xwyx => new gVector4<T>(x, w, y, x);
        
        /// <summary>
        /// Returns gVector4.ragr swizzling (equivalent to gVector4.xwyx).
        /// </summary>
        public gVector4<T> ragr => new gVector4<T>(x, w, y, x);
        
        /// <summary>
        /// Returns gVector4.xwyy swizzling.
        /// </summary>
        public gVector4<T> xwyy => new gVector4<T>(x, w, y, y);
        
        /// <summary>
        /// Returns gVector4.ragg swizzling (equivalent to gVector4.xwyy).
        /// </summary>
        public gVector4<T> ragg => new gVector4<T>(x, w, y, y);
        
        /// <summary>
        /// Returns gVector4.xwyz swizzling.
        /// </summary>
        public gVector4<T> xwyz => new gVector4<T>(x, w, y, z);
        
        /// <summary>
        /// Returns gVector4.ragb swizzling (equivalent to gVector4.xwyz).
        /// </summary>
        public gVector4<T> ragb => new gVector4<T>(x, w, y, z);
        
        /// <summary>
        /// Returns gVector4.xwyw swizzling.
        /// </summary>
        public gVector4<T> xwyw => new gVector4<T>(x, w, y, w);
        
        /// <summary>
        /// Returns gVector4.raga swizzling (equivalent to gVector4.xwyw).
        /// </summary>
        public gVector4<T> raga => new gVector4<T>(x, w, y, w);
        
        /// <summary>
        /// Returns gVector4.xwz swizzling.
        /// </summary>
        public gVector3<T> xwz => new gVector3<T>(x, w, z);
        
        /// <summary>
        /// Returns gVector4.rab swizzling (equivalent to gVector4.xwz).
        /// </summary>
        public gVector3<T> rab => new gVector3<T>(x, w, z);
        
        /// <summary>
        /// Returns gVector4.xwzx swizzling.
        /// </summary>
        public gVector4<T> xwzx => new gVector4<T>(x, w, z, x);
        
        /// <summary>
        /// Returns gVector4.rabr swizzling (equivalent to gVector4.xwzx).
        /// </summary>
        public gVector4<T> rabr => new gVector4<T>(x, w, z, x);
        
        /// <summary>
        /// Returns gVector4.xwzy swizzling.
        /// </summary>
        public gVector4<T> xwzy => new gVector4<T>(x, w, z, y);
        
        /// <summary>
        /// Returns gVector4.rabg swizzling (equivalent to gVector4.xwzy).
        /// </summary>
        public gVector4<T> rabg => new gVector4<T>(x, w, z, y);
        
        /// <summary>
        /// Returns gVector4.xwzz swizzling.
        /// </summary>
        public gVector4<T> xwzz => new gVector4<T>(x, w, z, z);
        
        /// <summary>
        /// Returns gVector4.rabb swizzling (equivalent to gVector4.xwzz).
        /// </summary>
        public gVector4<T> rabb => new gVector4<T>(x, w, z, z);
        
        /// <summary>
        /// Returns gVector4.xwzw swizzling.
        /// </summary>
        public gVector4<T> xwzw => new gVector4<T>(x, w, z, w);
        
        /// <summary>
        /// Returns gVector4.raba swizzling (equivalent to gVector4.xwzw).
        /// </summary>
        public gVector4<T> raba => new gVector4<T>(x, w, z, w);
        
        /// <summary>
        /// Returns gVector4.xww swizzling.
        /// </summary>
        public gVector3<T> xww => new gVector3<T>(x, w, w);
        
        /// <summary>
        /// Returns gVector4.raa swizzling (equivalent to gVector4.xww).
        /// </summary>
        public gVector3<T> raa => new gVector3<T>(x, w, w);
        
        /// <summary>
        /// Returns gVector4.xwwx swizzling.
        /// </summary>
        public gVector4<T> xwwx => new gVector4<T>(x, w, w, x);
        
        /// <summary>
        /// Returns gVector4.raar swizzling (equivalent to gVector4.xwwx).
        /// </summary>
        public gVector4<T> raar => new gVector4<T>(x, w, w, x);
        
        /// <summary>
        /// Returns gVector4.xwwy swizzling.
        /// </summary>
        public gVector4<T> xwwy => new gVector4<T>(x, w, w, y);
        
        /// <summary>
        /// Returns gVector4.raag swizzling (equivalent to gVector4.xwwy).
        /// </summary>
        public gVector4<T> raag => new gVector4<T>(x, w, w, y);
        
        /// <summary>
        /// Returns gVector4.xwwz swizzling.
        /// </summary>
        public gVector4<T> xwwz => new gVector4<T>(x, w, w, z);
        
        /// <summary>
        /// Returns gVector4.raab swizzling (equivalent to gVector4.xwwz).
        /// </summary>
        public gVector4<T> raab => new gVector4<T>(x, w, w, z);
        
        /// <summary>
        /// Returns gVector4.xwww swizzling.
        /// </summary>
        public gVector4<T> xwww => new gVector4<T>(x, w, w, w);
        
        /// <summary>
        /// Returns gVector4.raaa swizzling (equivalent to gVector4.xwww).
        /// </summary>
        public gVector4<T> raaa => new gVector4<T>(x, w, w, w);
        
        /// <summary>
        /// Returns gVector4.yx swizzling.
        /// </summary>
        public gVector2<T> yx => new gVector2<T>(y, x);
        
        /// <summary>
        /// Returns gVector4.gr swizzling (equivalent to gVector4.yx).
        /// </summary>
        public gVector2<T> gr => new gVector2<T>(y, x);
        
        /// <summary>
        /// Returns gVector4.yxx swizzling.
        /// </summary>
        public gVector3<T> yxx => new gVector3<T>(y, x, x);
        
        /// <summary>
        /// Returns gVector4.grr swizzling (equivalent to gVector4.yxx).
        /// </summary>
        public gVector3<T> grr => new gVector3<T>(y, x, x);
        
        /// <summary>
        /// Returns gVector4.yxxx swizzling.
        /// </summary>
        public gVector4<T> yxxx => new gVector4<T>(y, x, x, x);
        
        /// <summary>
        /// Returns gVector4.grrr swizzling (equivalent to gVector4.yxxx).
        /// </summary>
        public gVector4<T> grrr => new gVector4<T>(y, x, x, x);
        
        /// <summary>
        /// Returns gVector4.yxxy swizzling.
        /// </summary>
        public gVector4<T> yxxy => new gVector4<T>(y, x, x, y);
        
        /// <summary>
        /// Returns gVector4.grrg swizzling (equivalent to gVector4.yxxy).
        /// </summary>
        public gVector4<T> grrg => new gVector4<T>(y, x, x, y);
        
        /// <summary>
        /// Returns gVector4.yxxz swizzling.
        /// </summary>
        public gVector4<T> yxxz => new gVector4<T>(y, x, x, z);
        
        /// <summary>
        /// Returns gVector4.grrb swizzling (equivalent to gVector4.yxxz).
        /// </summary>
        public gVector4<T> grrb => new gVector4<T>(y, x, x, z);
        
        /// <summary>
        /// Returns gVector4.yxxw swizzling.
        /// </summary>
        public gVector4<T> yxxw => new gVector4<T>(y, x, x, w);
        
        /// <summary>
        /// Returns gVector4.grra swizzling (equivalent to gVector4.yxxw).
        /// </summary>
        public gVector4<T> grra => new gVector4<T>(y, x, x, w);
        
        /// <summary>
        /// Returns gVector4.yxy swizzling.
        /// </summary>
        public gVector3<T> yxy => new gVector3<T>(y, x, y);
        
        /// <summary>
        /// Returns gVector4.grg swizzling (equivalent to gVector4.yxy).
        /// </summary>
        public gVector3<T> grg => new gVector3<T>(y, x, y);
        
        /// <summary>
        /// Returns gVector4.yxyx swizzling.
        /// </summary>
        public gVector4<T> yxyx => new gVector4<T>(y, x, y, x);
        
        /// <summary>
        /// Returns gVector4.grgr swizzling (equivalent to gVector4.yxyx).
        /// </summary>
        public gVector4<T> grgr => new gVector4<T>(y, x, y, x);
        
        /// <summary>
        /// Returns gVector4.yxyy swizzling.
        /// </summary>
        public gVector4<T> yxyy => new gVector4<T>(y, x, y, y);
        
        /// <summary>
        /// Returns gVector4.grgg swizzling (equivalent to gVector4.yxyy).
        /// </summary>
        public gVector4<T> grgg => new gVector4<T>(y, x, y, y);
        
        /// <summary>
        /// Returns gVector4.yxyz swizzling.
        /// </summary>
        public gVector4<T> yxyz => new gVector4<T>(y, x, y, z);
        
        /// <summary>
        /// Returns gVector4.grgb swizzling (equivalent to gVector4.yxyz).
        /// </summary>
        public gVector4<T> grgb => new gVector4<T>(y, x, y, z);
        
        /// <summary>
        /// Returns gVector4.yxyw swizzling.
        /// </summary>
        public gVector4<T> yxyw => new gVector4<T>(y, x, y, w);
        
        /// <summary>
        /// Returns gVector4.grga swizzling (equivalent to gVector4.yxyw).
        /// </summary>
        public gVector4<T> grga => new gVector4<T>(y, x, y, w);
        
        /// <summary>
        /// Returns gVector4.yxz swizzling.
        /// </summary>
        public gVector3<T> yxz => new gVector3<T>(y, x, z);
        
        /// <summary>
        /// Returns gVector4.grb swizzling (equivalent to gVector4.yxz).
        /// </summary>
        public gVector3<T> grb => new gVector3<T>(y, x, z);
        
        /// <summary>
        /// Returns gVector4.yxzx swizzling.
        /// </summary>
        public gVector4<T> yxzx => new gVector4<T>(y, x, z, x);
        
        /// <summary>
        /// Returns gVector4.grbr swizzling (equivalent to gVector4.yxzx).
        /// </summary>
        public gVector4<T> grbr => new gVector4<T>(y, x, z, x);
        
        /// <summary>
        /// Returns gVector4.yxzy swizzling.
        /// </summary>
        public gVector4<T> yxzy => new gVector4<T>(y, x, z, y);
        
        /// <summary>
        /// Returns gVector4.grbg swizzling (equivalent to gVector4.yxzy).
        /// </summary>
        public gVector4<T> grbg => new gVector4<T>(y, x, z, y);
        
        /// <summary>
        /// Returns gVector4.yxzz swizzling.
        /// </summary>
        public gVector4<T> yxzz => new gVector4<T>(y, x, z, z);
        
        /// <summary>
        /// Returns gVector4.grbb swizzling (equivalent to gVector4.yxzz).
        /// </summary>
        public gVector4<T> grbb => new gVector4<T>(y, x, z, z);
        
        /// <summary>
        /// Returns gVector4.yxzw swizzling.
        /// </summary>
        public gVector4<T> yxzw => new gVector4<T>(y, x, z, w);
        
        /// <summary>
        /// Returns gVector4.grba swizzling (equivalent to gVector4.yxzw).
        /// </summary>
        public gVector4<T> grba => new gVector4<T>(y, x, z, w);
        
        /// <summary>
        /// Returns gVector4.yxw swizzling.
        /// </summary>
        public gVector3<T> yxw => new gVector3<T>(y, x, w);
        
        /// <summary>
        /// Returns gVector4.gra swizzling (equivalent to gVector4.yxw).
        /// </summary>
        public gVector3<T> gra => new gVector3<T>(y, x, w);
        
        /// <summary>
        /// Returns gVector4.yxwx swizzling.
        /// </summary>
        public gVector4<T> yxwx => new gVector4<T>(y, x, w, x);
        
        /// <summary>
        /// Returns gVector4.grar swizzling (equivalent to gVector4.yxwx).
        /// </summary>
        public gVector4<T> grar => new gVector4<T>(y, x, w, x);
        
        /// <summary>
        /// Returns gVector4.yxwy swizzling.
        /// </summary>
        public gVector4<T> yxwy => new gVector4<T>(y, x, w, y);
        
        /// <summary>
        /// Returns gVector4.grag swizzling (equivalent to gVector4.yxwy).
        /// </summary>
        public gVector4<T> grag => new gVector4<T>(y, x, w, y);
        
        /// <summary>
        /// Returns gVector4.yxwz swizzling.
        /// </summary>
        public gVector4<T> yxwz => new gVector4<T>(y, x, w, z);
        
        /// <summary>
        /// Returns gVector4.grab swizzling (equivalent to gVector4.yxwz).
        /// </summary>
        public gVector4<T> grab => new gVector4<T>(y, x, w, z);
        
        /// <summary>
        /// Returns gVector4.yxww swizzling.
        /// </summary>
        public gVector4<T> yxww => new gVector4<T>(y, x, w, w);
        
        /// <summary>
        /// Returns gVector4.graa swizzling (equivalent to gVector4.yxww).
        /// </summary>
        public gVector4<T> graa => new gVector4<T>(y, x, w, w);
        
        /// <summary>
        /// Returns gVector4.yy swizzling.
        /// </summary>
        public gVector2<T> yy => new gVector2<T>(y, y);
        
        /// <summary>
        /// Returns gVector4.gg swizzling (equivalent to gVector4.yy).
        /// </summary>
        public gVector2<T> gg => new gVector2<T>(y, y);
        
        /// <summary>
        /// Returns gVector4.yyx swizzling.
        /// </summary>
        public gVector3<T> yyx => new gVector3<T>(y, y, x);
        
        /// <summary>
        /// Returns gVector4.ggr swizzling (equivalent to gVector4.yyx).
        /// </summary>
        public gVector3<T> ggr => new gVector3<T>(y, y, x);
        
        /// <summary>
        /// Returns gVector4.yyxx swizzling.
        /// </summary>
        public gVector4<T> yyxx => new gVector4<T>(y, y, x, x);
        
        /// <summary>
        /// Returns gVector4.ggrr swizzling (equivalent to gVector4.yyxx).
        /// </summary>
        public gVector4<T> ggrr => new gVector4<T>(y, y, x, x);
        
        /// <summary>
        /// Returns gVector4.yyxy swizzling.
        /// </summary>
        public gVector4<T> yyxy => new gVector4<T>(y, y, x, y);
        
        /// <summary>
        /// Returns gVector4.ggrg swizzling (equivalent to gVector4.yyxy).
        /// </summary>
        public gVector4<T> ggrg => new gVector4<T>(y, y, x, y);
        
        /// <summary>
        /// Returns gVector4.yyxz swizzling.
        /// </summary>
        public gVector4<T> yyxz => new gVector4<T>(y, y, x, z);
        
        /// <summary>
        /// Returns gVector4.ggrb swizzling (equivalent to gVector4.yyxz).
        /// </summary>
        public gVector4<T> ggrb => new gVector4<T>(y, y, x, z);
        
        /// <summary>
        /// Returns gVector4.yyxw swizzling.
        /// </summary>
        public gVector4<T> yyxw => new gVector4<T>(y, y, x, w);
        
        /// <summary>
        /// Returns gVector4.ggra swizzling (equivalent to gVector4.yyxw).
        /// </summary>
        public gVector4<T> ggra => new gVector4<T>(y, y, x, w);
        
        /// <summary>
        /// Returns gVector4.yyy swizzling.
        /// </summary>
        public gVector3<T> yyy => new gVector3<T>(y, y, y);
        
        /// <summary>
        /// Returns gVector4.ggg swizzling (equivalent to gVector4.yyy).
        /// </summary>
        public gVector3<T> ggg => new gVector3<T>(y, y, y);
        
        /// <summary>
        /// Returns gVector4.yyyx swizzling.
        /// </summary>
        public gVector4<T> yyyx => new gVector4<T>(y, y, y, x);
        
        /// <summary>
        /// Returns gVector4.gggr swizzling (equivalent to gVector4.yyyx).
        /// </summary>
        public gVector4<T> gggr => new gVector4<T>(y, y, y, x);
        
        /// <summary>
        /// Returns gVector4.yyyy swizzling.
        /// </summary>
        public gVector4<T> yyyy => new gVector4<T>(y, y, y, y);
        
        /// <summary>
        /// Returns gVector4.gggg swizzling (equivalent to gVector4.yyyy).
        /// </summary>
        public gVector4<T> gggg => new gVector4<T>(y, y, y, y);
        
        /// <summary>
        /// Returns gVector4.yyyz swizzling.
        /// </summary>
        public gVector4<T> yyyz => new gVector4<T>(y, y, y, z);
        
        /// <summary>
        /// Returns gVector4.gggb swizzling (equivalent to gVector4.yyyz).
        /// </summary>
        public gVector4<T> gggb => new gVector4<T>(y, y, y, z);
        
        /// <summary>
        /// Returns gVector4.yyyw swizzling.
        /// </summary>
        public gVector4<T> yyyw => new gVector4<T>(y, y, y, w);
        
        /// <summary>
        /// Returns gVector4.ggga swizzling (equivalent to gVector4.yyyw).
        /// </summary>
        public gVector4<T> ggga => new gVector4<T>(y, y, y, w);
        
        /// <summary>
        /// Returns gVector4.yyz swizzling.
        /// </summary>
        public gVector3<T> yyz => new gVector3<T>(y, y, z);
        
        /// <summary>
        /// Returns gVector4.ggb swizzling (equivalent to gVector4.yyz).
        /// </summary>
        public gVector3<T> ggb => new gVector3<T>(y, y, z);
        
        /// <summary>
        /// Returns gVector4.yyzx swizzling.
        /// </summary>
        public gVector4<T> yyzx => new gVector4<T>(y, y, z, x);
        
        /// <summary>
        /// Returns gVector4.ggbr swizzling (equivalent to gVector4.yyzx).
        /// </summary>
        public gVector4<T> ggbr => new gVector4<T>(y, y, z, x);
        
        /// <summary>
        /// Returns gVector4.yyzy swizzling.
        /// </summary>
        public gVector4<T> yyzy => new gVector4<T>(y, y, z, y);
        
        /// <summary>
        /// Returns gVector4.ggbg swizzling (equivalent to gVector4.yyzy).
        /// </summary>
        public gVector4<T> ggbg => new gVector4<T>(y, y, z, y);
        
        /// <summary>
        /// Returns gVector4.yyzz swizzling.
        /// </summary>
        public gVector4<T> yyzz => new gVector4<T>(y, y, z, z);
        
        /// <summary>
        /// Returns gVector4.ggbb swizzling (equivalent to gVector4.yyzz).
        /// </summary>
        public gVector4<T> ggbb => new gVector4<T>(y, y, z, z);
        
        /// <summary>
        /// Returns gVector4.yyzw swizzling.
        /// </summary>
        public gVector4<T> yyzw => new gVector4<T>(y, y, z, w);
        
        /// <summary>
        /// Returns gVector4.ggba swizzling (equivalent to gVector4.yyzw).
        /// </summary>
        public gVector4<T> ggba => new gVector4<T>(y, y, z, w);
        
        /// <summary>
        /// Returns gVector4.yyw swizzling.
        /// </summary>
        public gVector3<T> yyw => new gVector3<T>(y, y, w);
        
        /// <summary>
        /// Returns gVector4.gga swizzling (equivalent to gVector4.yyw).
        /// </summary>
        public gVector3<T> gga => new gVector3<T>(y, y, w);
        
        /// <summary>
        /// Returns gVector4.yywx swizzling.
        /// </summary>
        public gVector4<T> yywx => new gVector4<T>(y, y, w, x);
        
        /// <summary>
        /// Returns gVector4.ggar swizzling (equivalent to gVector4.yywx).
        /// </summary>
        public gVector4<T> ggar => new gVector4<T>(y, y, w, x);
        
        /// <summary>
        /// Returns gVector4.yywy swizzling.
        /// </summary>
        public gVector4<T> yywy => new gVector4<T>(y, y, w, y);
        
        /// <summary>
        /// Returns gVector4.ggag swizzling (equivalent to gVector4.yywy).
        /// </summary>
        public gVector4<T> ggag => new gVector4<T>(y, y, w, y);
        
        /// <summary>
        /// Returns gVector4.yywz swizzling.
        /// </summary>
        public gVector4<T> yywz => new gVector4<T>(y, y, w, z);
        
        /// <summary>
        /// Returns gVector4.ggab swizzling (equivalent to gVector4.yywz).
        /// </summary>
        public gVector4<T> ggab => new gVector4<T>(y, y, w, z);
        
        /// <summary>
        /// Returns gVector4.yyww swizzling.
        /// </summary>
        public gVector4<T> yyww => new gVector4<T>(y, y, w, w);
        
        /// <summary>
        /// Returns gVector4.ggaa swizzling (equivalent to gVector4.yyww).
        /// </summary>
        public gVector4<T> ggaa => new gVector4<T>(y, y, w, w);
        
        /// <summary>
        /// Returns gVector4.yz swizzling.
        /// </summary>
        public gVector2<T> yz => new gVector2<T>(y, z);
        
        /// <summary>
        /// Returns gVector4.gb swizzling (equivalent to gVector4.yz).
        /// </summary>
        public gVector2<T> gb => new gVector2<T>(y, z);
        
        /// <summary>
        /// Returns gVector4.yzx swizzling.
        /// </summary>
        public gVector3<T> yzx => new gVector3<T>(y, z, x);
        
        /// <summary>
        /// Returns gVector4.gbr swizzling (equivalent to gVector4.yzx).
        /// </summary>
        public gVector3<T> gbr => new gVector3<T>(y, z, x);
        
        /// <summary>
        /// Returns gVector4.yzxx swizzling.
        /// </summary>
        public gVector4<T> yzxx => new gVector4<T>(y, z, x, x);
        
        /// <summary>
        /// Returns gVector4.gbrr swizzling (equivalent to gVector4.yzxx).
        /// </summary>
        public gVector4<T> gbrr => new gVector4<T>(y, z, x, x);
        
        /// <summary>
        /// Returns gVector4.yzxy swizzling.
        /// </summary>
        public gVector4<T> yzxy => new gVector4<T>(y, z, x, y);
        
        /// <summary>
        /// Returns gVector4.gbrg swizzling (equivalent to gVector4.yzxy).
        /// </summary>
        public gVector4<T> gbrg => new gVector4<T>(y, z, x, y);
        
        /// <summary>
        /// Returns gVector4.yzxz swizzling.
        /// </summary>
        public gVector4<T> yzxz => new gVector4<T>(y, z, x, z);
        
        /// <summary>
        /// Returns gVector4.gbrb swizzling (equivalent to gVector4.yzxz).
        /// </summary>
        public gVector4<T> gbrb => new gVector4<T>(y, z, x, z);
        
        /// <summary>
        /// Returns gVector4.yzxw swizzling.
        /// </summary>
        public gVector4<T> yzxw => new gVector4<T>(y, z, x, w);
        
        /// <summary>
        /// Returns gVector4.gbra swizzling (equivalent to gVector4.yzxw).
        /// </summary>
        public gVector4<T> gbra => new gVector4<T>(y, z, x, w);
        
        /// <summary>
        /// Returns gVector4.yzy swizzling.
        /// </summary>
        public gVector3<T> yzy => new gVector3<T>(y, z, y);
        
        /// <summary>
        /// Returns gVector4.gbg swizzling (equivalent to gVector4.yzy).
        /// </summary>
        public gVector3<T> gbg => new gVector3<T>(y, z, y);
        
        /// <summary>
        /// Returns gVector4.yzyx swizzling.
        /// </summary>
        public gVector4<T> yzyx => new gVector4<T>(y, z, y, x);
        
        /// <summary>
        /// Returns gVector4.gbgr swizzling (equivalent to gVector4.yzyx).
        /// </summary>
        public gVector4<T> gbgr => new gVector4<T>(y, z, y, x);
        
        /// <summary>
        /// Returns gVector4.yzyy swizzling.
        /// </summary>
        public gVector4<T> yzyy => new gVector4<T>(y, z, y, y);
        
        /// <summary>
        /// Returns gVector4.gbgg swizzling (equivalent to gVector4.yzyy).
        /// </summary>
        public gVector4<T> gbgg => new gVector4<T>(y, z, y, y);
        
        /// <summary>
        /// Returns gVector4.yzyz swizzling.
        /// </summary>
        public gVector4<T> yzyz => new gVector4<T>(y, z, y, z);
        
        /// <summary>
        /// Returns gVector4.gbgb swizzling (equivalent to gVector4.yzyz).
        /// </summary>
        public gVector4<T> gbgb => new gVector4<T>(y, z, y, z);
        
        /// <summary>
        /// Returns gVector4.yzyw swizzling.
        /// </summary>
        public gVector4<T> yzyw => new gVector4<T>(y, z, y, w);
        
        /// <summary>
        /// Returns gVector4.gbga swizzling (equivalent to gVector4.yzyw).
        /// </summary>
        public gVector4<T> gbga => new gVector4<T>(y, z, y, w);
        
        /// <summary>
        /// Returns gVector4.yzz swizzling.
        /// </summary>
        public gVector3<T> yzz => new gVector3<T>(y, z, z);
        
        /// <summary>
        /// Returns gVector4.gbb swizzling (equivalent to gVector4.yzz).
        /// </summary>
        public gVector3<T> gbb => new gVector3<T>(y, z, z);
        
        /// <summary>
        /// Returns gVector4.yzzx swizzling.
        /// </summary>
        public gVector4<T> yzzx => new gVector4<T>(y, z, z, x);
        
        /// <summary>
        /// Returns gVector4.gbbr swizzling (equivalent to gVector4.yzzx).
        /// </summary>
        public gVector4<T> gbbr => new gVector4<T>(y, z, z, x);
        
        /// <summary>
        /// Returns gVector4.yzzy swizzling.
        /// </summary>
        public gVector4<T> yzzy => new gVector4<T>(y, z, z, y);
        
        /// <summary>
        /// Returns gVector4.gbbg swizzling (equivalent to gVector4.yzzy).
        /// </summary>
        public gVector4<T> gbbg => new gVector4<T>(y, z, z, y);
        
        /// <summary>
        /// Returns gVector4.yzzz swizzling.
        /// </summary>
        public gVector4<T> yzzz => new gVector4<T>(y, z, z, z);
        
        /// <summary>
        /// Returns gVector4.gbbb swizzling (equivalent to gVector4.yzzz).
        /// </summary>
        public gVector4<T> gbbb => new gVector4<T>(y, z, z, z);
        
        /// <summary>
        /// Returns gVector4.yzzw swizzling.
        /// </summary>
        public gVector4<T> yzzw => new gVector4<T>(y, z, z, w);
        
        /// <summary>
        /// Returns gVector4.gbba swizzling (equivalent to gVector4.yzzw).
        /// </summary>
        public gVector4<T> gbba => new gVector4<T>(y, z, z, w);
        
        /// <summary>
        /// Returns gVector4.yzw swizzling.
        /// </summary>
        public gVector3<T> yzw => new gVector3<T>(y, z, w);
        
        /// <summary>
        /// Returns gVector4.gba swizzling (equivalent to gVector4.yzw).
        /// </summary>
        public gVector3<T> gba => new gVector3<T>(y, z, w);
        
        /// <summary>
        /// Returns gVector4.yzwx swizzling.
        /// </summary>
        public gVector4<T> yzwx => new gVector4<T>(y, z, w, x);
        
        /// <summary>
        /// Returns gVector4.gbar swizzling (equivalent to gVector4.yzwx).
        /// </summary>
        public gVector4<T> gbar => new gVector4<T>(y, z, w, x);
        
        /// <summary>
        /// Returns gVector4.yzwy swizzling.
        /// </summary>
        public gVector4<T> yzwy => new gVector4<T>(y, z, w, y);
        
        /// <summary>
        /// Returns gVector4.gbag swizzling (equivalent to gVector4.yzwy).
        /// </summary>
        public gVector4<T> gbag => new gVector4<T>(y, z, w, y);
        
        /// <summary>
        /// Returns gVector4.yzwz swizzling.
        /// </summary>
        public gVector4<T> yzwz => new gVector4<T>(y, z, w, z);
        
        /// <summary>
        /// Returns gVector4.gbab swizzling (equivalent to gVector4.yzwz).
        /// </summary>
        public gVector4<T> gbab => new gVector4<T>(y, z, w, z);
        
        /// <summary>
        /// Returns gVector4.yzww swizzling.
        /// </summary>
        public gVector4<T> yzww => new gVector4<T>(y, z, w, w);
        
        /// <summary>
        /// Returns gVector4.gbaa swizzling (equivalent to gVector4.yzww).
        /// </summary>
        public gVector4<T> gbaa => new gVector4<T>(y, z, w, w);
        
        /// <summary>
        /// Returns gVector4.yw swizzling.
        /// </summary>
        public gVector2<T> yw => new gVector2<T>(y, w);
        
        /// <summary>
        /// Returns gVector4.ga swizzling (equivalent to gVector4.yw).
        /// </summary>
        public gVector2<T> ga => new gVector2<T>(y, w);
        
        /// <summary>
        /// Returns gVector4.ywx swizzling.
        /// </summary>
        public gVector3<T> ywx => new gVector3<T>(y, w, x);
        
        /// <summary>
        /// Returns gVector4.gar swizzling (equivalent to gVector4.ywx).
        /// </summary>
        public gVector3<T> gar => new gVector3<T>(y, w, x);
        
        /// <summary>
        /// Returns gVector4.ywxx swizzling.
        /// </summary>
        public gVector4<T> ywxx => new gVector4<T>(y, w, x, x);
        
        /// <summary>
        /// Returns gVector4.garr swizzling (equivalent to gVector4.ywxx).
        /// </summary>
        public gVector4<T> garr => new gVector4<T>(y, w, x, x);
        
        /// <summary>
        /// Returns gVector4.ywxy swizzling.
        /// </summary>
        public gVector4<T> ywxy => new gVector4<T>(y, w, x, y);
        
        /// <summary>
        /// Returns gVector4.garg swizzling (equivalent to gVector4.ywxy).
        /// </summary>
        public gVector4<T> garg => new gVector4<T>(y, w, x, y);
        
        /// <summary>
        /// Returns gVector4.ywxz swizzling.
        /// </summary>
        public gVector4<T> ywxz => new gVector4<T>(y, w, x, z);
        
        /// <summary>
        /// Returns gVector4.garb swizzling (equivalent to gVector4.ywxz).
        /// </summary>
        public gVector4<T> garb => new gVector4<T>(y, w, x, z);
        
        /// <summary>
        /// Returns gVector4.ywxw swizzling.
        /// </summary>
        public gVector4<T> ywxw => new gVector4<T>(y, w, x, w);
        
        /// <summary>
        /// Returns gVector4.gara swizzling (equivalent to gVector4.ywxw).
        /// </summary>
        public gVector4<T> gara => new gVector4<T>(y, w, x, w);
        
        /// <summary>
        /// Returns gVector4.ywy swizzling.
        /// </summary>
        public gVector3<T> ywy => new gVector3<T>(y, w, y);
        
        /// <summary>
        /// Returns gVector4.gag swizzling (equivalent to gVector4.ywy).
        /// </summary>
        public gVector3<T> gag => new gVector3<T>(y, w, y);
        
        /// <summary>
        /// Returns gVector4.ywyx swizzling.
        /// </summary>
        public gVector4<T> ywyx => new gVector4<T>(y, w, y, x);
        
        /// <summary>
        /// Returns gVector4.gagr swizzling (equivalent to gVector4.ywyx).
        /// </summary>
        public gVector4<T> gagr => new gVector4<T>(y, w, y, x);
        
        /// <summary>
        /// Returns gVector4.ywyy swizzling.
        /// </summary>
        public gVector4<T> ywyy => new gVector4<T>(y, w, y, y);
        
        /// <summary>
        /// Returns gVector4.gagg swizzling (equivalent to gVector4.ywyy).
        /// </summary>
        public gVector4<T> gagg => new gVector4<T>(y, w, y, y);
        
        /// <summary>
        /// Returns gVector4.ywyz swizzling.
        /// </summary>
        public gVector4<T> ywyz => new gVector4<T>(y, w, y, z);
        
        /// <summary>
        /// Returns gVector4.gagb swizzling (equivalent to gVector4.ywyz).
        /// </summary>
        public gVector4<T> gagb => new gVector4<T>(y, w, y, z);
        
        /// <summary>
        /// Returns gVector4.ywyw swizzling.
        /// </summary>
        public gVector4<T> ywyw => new gVector4<T>(y, w, y, w);
        
        /// <summary>
        /// Returns gVector4.gaga swizzling (equivalent to gVector4.ywyw).
        /// </summary>
        public gVector4<T> gaga => new gVector4<T>(y, w, y, w);
        
        /// <summary>
        /// Returns gVector4.ywz swizzling.
        /// </summary>
        public gVector3<T> ywz => new gVector3<T>(y, w, z);
        
        /// <summary>
        /// Returns gVector4.gab swizzling (equivalent to gVector4.ywz).
        /// </summary>
        public gVector3<T> gab => new gVector3<T>(y, w, z);
        
        /// <summary>
        /// Returns gVector4.ywzx swizzling.
        /// </summary>
        public gVector4<T> ywzx => new gVector4<T>(y, w, z, x);
        
        /// <summary>
        /// Returns gVector4.gabr swizzling (equivalent to gVector4.ywzx).
        /// </summary>
        public gVector4<T> gabr => new gVector4<T>(y, w, z, x);
        
        /// <summary>
        /// Returns gVector4.ywzy swizzling.
        /// </summary>
        public gVector4<T> ywzy => new gVector4<T>(y, w, z, y);
        
        /// <summary>
        /// Returns gVector4.gabg swizzling (equivalent to gVector4.ywzy).
        /// </summary>
        public gVector4<T> gabg => new gVector4<T>(y, w, z, y);
        
        /// <summary>
        /// Returns gVector4.ywzz swizzling.
        /// </summary>
        public gVector4<T> ywzz => new gVector4<T>(y, w, z, z);
        
        /// <summary>
        /// Returns gVector4.gabb swizzling (equivalent to gVector4.ywzz).
        /// </summary>
        public gVector4<T> gabb => new gVector4<T>(y, w, z, z);
        
        /// <summary>
        /// Returns gVector4.ywzw swizzling.
        /// </summary>
        public gVector4<T> ywzw => new gVector4<T>(y, w, z, w);
        
        /// <summary>
        /// Returns gVector4.gaba swizzling (equivalent to gVector4.ywzw).
        /// </summary>
        public gVector4<T> gaba => new gVector4<T>(y, w, z, w);
        
        /// <summary>
        /// Returns gVector4.yww swizzling.
        /// </summary>
        public gVector3<T> yww => new gVector3<T>(y, w, w);
        
        /// <summary>
        /// Returns gVector4.gaa swizzling (equivalent to gVector4.yww).
        /// </summary>
        public gVector3<T> gaa => new gVector3<T>(y, w, w);
        
        /// <summary>
        /// Returns gVector4.ywwx swizzling.
        /// </summary>
        public gVector4<T> ywwx => new gVector4<T>(y, w, w, x);
        
        /// <summary>
        /// Returns gVector4.gaar swizzling (equivalent to gVector4.ywwx).
        /// </summary>
        public gVector4<T> gaar => new gVector4<T>(y, w, w, x);
        
        /// <summary>
        /// Returns gVector4.ywwy swizzling.
        /// </summary>
        public gVector4<T> ywwy => new gVector4<T>(y, w, w, y);
        
        /// <summary>
        /// Returns gVector4.gaag swizzling (equivalent to gVector4.ywwy).
        /// </summary>
        public gVector4<T> gaag => new gVector4<T>(y, w, w, y);
        
        /// <summary>
        /// Returns gVector4.ywwz swizzling.
        /// </summary>
        public gVector4<T> ywwz => new gVector4<T>(y, w, w, z);
        
        /// <summary>
        /// Returns gVector4.gaab swizzling (equivalent to gVector4.ywwz).
        /// </summary>
        public gVector4<T> gaab => new gVector4<T>(y, w, w, z);
        
        /// <summary>
        /// Returns gVector4.ywww swizzling.
        /// </summary>
        public gVector4<T> ywww => new gVector4<T>(y, w, w, w);
        
        /// <summary>
        /// Returns gVector4.gaaa swizzling (equivalent to gVector4.ywww).
        /// </summary>
        public gVector4<T> gaaa => new gVector4<T>(y, w, w, w);
        
        /// <summary>
        /// Returns gVector4.zx swizzling.
        /// </summary>
        public gVector2<T> zx => new gVector2<T>(z, x);
        
        /// <summary>
        /// Returns gVector4.br swizzling (equivalent to gVector4.zx).
        /// </summary>
        public gVector2<T> br => new gVector2<T>(z, x);
        
        /// <summary>
        /// Returns gVector4.zxx swizzling.
        /// </summary>
        public gVector3<T> zxx => new gVector3<T>(z, x, x);
        
        /// <summary>
        /// Returns gVector4.brr swizzling (equivalent to gVector4.zxx).
        /// </summary>
        public gVector3<T> brr => new gVector3<T>(z, x, x);
        
        /// <summary>
        /// Returns gVector4.zxxx swizzling.
        /// </summary>
        public gVector4<T> zxxx => new gVector4<T>(z, x, x, x);
        
        /// <summary>
        /// Returns gVector4.brrr swizzling (equivalent to gVector4.zxxx).
        /// </summary>
        public gVector4<T> brrr => new gVector4<T>(z, x, x, x);
        
        /// <summary>
        /// Returns gVector4.zxxy swizzling.
        /// </summary>
        public gVector4<T> zxxy => new gVector4<T>(z, x, x, y);
        
        /// <summary>
        /// Returns gVector4.brrg swizzling (equivalent to gVector4.zxxy).
        /// </summary>
        public gVector4<T> brrg => new gVector4<T>(z, x, x, y);
        
        /// <summary>
        /// Returns gVector4.zxxz swizzling.
        /// </summary>
        public gVector4<T> zxxz => new gVector4<T>(z, x, x, z);
        
        /// <summary>
        /// Returns gVector4.brrb swizzling (equivalent to gVector4.zxxz).
        /// </summary>
        public gVector4<T> brrb => new gVector4<T>(z, x, x, z);
        
        /// <summary>
        /// Returns gVector4.zxxw swizzling.
        /// </summary>
        public gVector4<T> zxxw => new gVector4<T>(z, x, x, w);
        
        /// <summary>
        /// Returns gVector4.brra swizzling (equivalent to gVector4.zxxw).
        /// </summary>
        public gVector4<T> brra => new gVector4<T>(z, x, x, w);
        
        /// <summary>
        /// Returns gVector4.zxy swizzling.
        /// </summary>
        public gVector3<T> zxy => new gVector3<T>(z, x, y);
        
        /// <summary>
        /// Returns gVector4.brg swizzling (equivalent to gVector4.zxy).
        /// </summary>
        public gVector3<T> brg => new gVector3<T>(z, x, y);
        
        /// <summary>
        /// Returns gVector4.zxyx swizzling.
        /// </summary>
        public gVector4<T> zxyx => new gVector4<T>(z, x, y, x);
        
        /// <summary>
        /// Returns gVector4.brgr swizzling (equivalent to gVector4.zxyx).
        /// </summary>
        public gVector4<T> brgr => new gVector4<T>(z, x, y, x);
        
        /// <summary>
        /// Returns gVector4.zxyy swizzling.
        /// </summary>
        public gVector4<T> zxyy => new gVector4<T>(z, x, y, y);
        
        /// <summary>
        /// Returns gVector4.brgg swizzling (equivalent to gVector4.zxyy).
        /// </summary>
        public gVector4<T> brgg => new gVector4<T>(z, x, y, y);
        
        /// <summary>
        /// Returns gVector4.zxyz swizzling.
        /// </summary>
        public gVector4<T> zxyz => new gVector4<T>(z, x, y, z);
        
        /// <summary>
        /// Returns gVector4.brgb swizzling (equivalent to gVector4.zxyz).
        /// </summary>
        public gVector4<T> brgb => new gVector4<T>(z, x, y, z);
        
        /// <summary>
        /// Returns gVector4.zxyw swizzling.
        /// </summary>
        public gVector4<T> zxyw => new gVector4<T>(z, x, y, w);
        
        /// <summary>
        /// Returns gVector4.brga swizzling (equivalent to gVector4.zxyw).
        /// </summary>
        public gVector4<T> brga => new gVector4<T>(z, x, y, w);
        
        /// <summary>
        /// Returns gVector4.zxz swizzling.
        /// </summary>
        public gVector3<T> zxz => new gVector3<T>(z, x, z);
        
        /// <summary>
        /// Returns gVector4.brb swizzling (equivalent to gVector4.zxz).
        /// </summary>
        public gVector3<T> brb => new gVector3<T>(z, x, z);
        
        /// <summary>
        /// Returns gVector4.zxzx swizzling.
        /// </summary>
        public gVector4<T> zxzx => new gVector4<T>(z, x, z, x);
        
        /// <summary>
        /// Returns gVector4.brbr swizzling (equivalent to gVector4.zxzx).
        /// </summary>
        public gVector4<T> brbr => new gVector4<T>(z, x, z, x);
        
        /// <summary>
        /// Returns gVector4.zxzy swizzling.
        /// </summary>
        public gVector4<T> zxzy => new gVector4<T>(z, x, z, y);
        
        /// <summary>
        /// Returns gVector4.brbg swizzling (equivalent to gVector4.zxzy).
        /// </summary>
        public gVector4<T> brbg => new gVector4<T>(z, x, z, y);
        
        /// <summary>
        /// Returns gVector4.zxzz swizzling.
        /// </summary>
        public gVector4<T> zxzz => new gVector4<T>(z, x, z, z);
        
        /// <summary>
        /// Returns gVector4.brbb swizzling (equivalent to gVector4.zxzz).
        /// </summary>
        public gVector4<T> brbb => new gVector4<T>(z, x, z, z);
        
        /// <summary>
        /// Returns gVector4.zxzw swizzling.
        /// </summary>
        public gVector4<T> zxzw => new gVector4<T>(z, x, z, w);
        
        /// <summary>
        /// Returns gVector4.brba swizzling (equivalent to gVector4.zxzw).
        /// </summary>
        public gVector4<T> brba => new gVector4<T>(z, x, z, w);
        
        /// <summary>
        /// Returns gVector4.zxw swizzling.
        /// </summary>
        public gVector3<T> zxw => new gVector3<T>(z, x, w);
        
        /// <summary>
        /// Returns gVector4.bra swizzling (equivalent to gVector4.zxw).
        /// </summary>
        public gVector3<T> bra => new gVector3<T>(z, x, w);
        
        /// <summary>
        /// Returns gVector4.zxwx swizzling.
        /// </summary>
        public gVector4<T> zxwx => new gVector4<T>(z, x, w, x);
        
        /// <summary>
        /// Returns gVector4.brar swizzling (equivalent to gVector4.zxwx).
        /// </summary>
        public gVector4<T> brar => new gVector4<T>(z, x, w, x);
        
        /// <summary>
        /// Returns gVector4.zxwy swizzling.
        /// </summary>
        public gVector4<T> zxwy => new gVector4<T>(z, x, w, y);
        
        /// <summary>
        /// Returns gVector4.brag swizzling (equivalent to gVector4.zxwy).
        /// </summary>
        public gVector4<T> brag => new gVector4<T>(z, x, w, y);
        
        /// <summary>
        /// Returns gVector4.zxwz swizzling.
        /// </summary>
        public gVector4<T> zxwz => new gVector4<T>(z, x, w, z);
        
        /// <summary>
        /// Returns gVector4.brab swizzling (equivalent to gVector4.zxwz).
        /// </summary>
        public gVector4<T> brab => new gVector4<T>(z, x, w, z);
        
        /// <summary>
        /// Returns gVector4.zxww swizzling.
        /// </summary>
        public gVector4<T> zxww => new gVector4<T>(z, x, w, w);
        
        /// <summary>
        /// Returns gVector4.braa swizzling (equivalent to gVector4.zxww).
        /// </summary>
        public gVector4<T> braa => new gVector4<T>(z, x, w, w);
        
        /// <summary>
        /// Returns gVector4.zy swizzling.
        /// </summary>
        public gVector2<T> zy => new gVector2<T>(z, y);
        
        /// <summary>
        /// Returns gVector4.bg swizzling (equivalent to gVector4.zy).
        /// </summary>
        public gVector2<T> bg => new gVector2<T>(z, y);
        
        /// <summary>
        /// Returns gVector4.zyx swizzling.
        /// </summary>
        public gVector3<T> zyx => new gVector3<T>(z, y, x);
        
        /// <summary>
        /// Returns gVector4.bgr swizzling (equivalent to gVector4.zyx).
        /// </summary>
        public gVector3<T> bgr => new gVector3<T>(z, y, x);
        
        /// <summary>
        /// Returns gVector4.zyxx swizzling.
        /// </summary>
        public gVector4<T> zyxx => new gVector4<T>(z, y, x, x);
        
        /// <summary>
        /// Returns gVector4.bgrr swizzling (equivalent to gVector4.zyxx).
        /// </summary>
        public gVector4<T> bgrr => new gVector4<T>(z, y, x, x);
        
        /// <summary>
        /// Returns gVector4.zyxy swizzling.
        /// </summary>
        public gVector4<T> zyxy => new gVector4<T>(z, y, x, y);
        
        /// <summary>
        /// Returns gVector4.bgrg swizzling (equivalent to gVector4.zyxy).
        /// </summary>
        public gVector4<T> bgrg => new gVector4<T>(z, y, x, y);
        
        /// <summary>
        /// Returns gVector4.zyxz swizzling.
        /// </summary>
        public gVector4<T> zyxz => new gVector4<T>(z, y, x, z);
        
        /// <summary>
        /// Returns gVector4.bgrb swizzling (equivalent to gVector4.zyxz).
        /// </summary>
        public gVector4<T> bgrb => new gVector4<T>(z, y, x, z);
        
        /// <summary>
        /// Returns gVector4.zyxw swizzling.
        /// </summary>
        public gVector4<T> zyxw => new gVector4<T>(z, y, x, w);
        
        /// <summary>
        /// Returns gVector4.bgra swizzling (equivalent to gVector4.zyxw).
        /// </summary>
        public gVector4<T> bgra => new gVector4<T>(z, y, x, w);
        
        /// <summary>
        /// Returns gVector4.zyy swizzling.
        /// </summary>
        public gVector3<T> zyy => new gVector3<T>(z, y, y);
        
        /// <summary>
        /// Returns gVector4.bgg swizzling (equivalent to gVector4.zyy).
        /// </summary>
        public gVector3<T> bgg => new gVector3<T>(z, y, y);
        
        /// <summary>
        /// Returns gVector4.zyyx swizzling.
        /// </summary>
        public gVector4<T> zyyx => new gVector4<T>(z, y, y, x);
        
        /// <summary>
        /// Returns gVector4.bggr swizzling (equivalent to gVector4.zyyx).
        /// </summary>
        public gVector4<T> bggr => new gVector4<T>(z, y, y, x);
        
        /// <summary>
        /// Returns gVector4.zyyy swizzling.
        /// </summary>
        public gVector4<T> zyyy => new gVector4<T>(z, y, y, y);
        
        /// <summary>
        /// Returns gVector4.bggg swizzling (equivalent to gVector4.zyyy).
        /// </summary>
        public gVector4<T> bggg => new gVector4<T>(z, y, y, y);
        
        /// <summary>
        /// Returns gVector4.zyyz swizzling.
        /// </summary>
        public gVector4<T> zyyz => new gVector4<T>(z, y, y, z);
        
        /// <summary>
        /// Returns gVector4.bggb swizzling (equivalent to gVector4.zyyz).
        /// </summary>
        public gVector4<T> bggb => new gVector4<T>(z, y, y, z);
        
        /// <summary>
        /// Returns gVector4.zyyw swizzling.
        /// </summary>
        public gVector4<T> zyyw => new gVector4<T>(z, y, y, w);
        
        /// <summary>
        /// Returns gVector4.bgga swizzling (equivalent to gVector4.zyyw).
        /// </summary>
        public gVector4<T> bgga => new gVector4<T>(z, y, y, w);
        
        /// <summary>
        /// Returns gVector4.zyz swizzling.
        /// </summary>
        public gVector3<T> zyz => new gVector3<T>(z, y, z);
        
        /// <summary>
        /// Returns gVector4.bgb swizzling (equivalent to gVector4.zyz).
        /// </summary>
        public gVector3<T> bgb => new gVector3<T>(z, y, z);
        
        /// <summary>
        /// Returns gVector4.zyzx swizzling.
        /// </summary>
        public gVector4<T> zyzx => new gVector4<T>(z, y, z, x);
        
        /// <summary>
        /// Returns gVector4.bgbr swizzling (equivalent to gVector4.zyzx).
        /// </summary>
        public gVector4<T> bgbr => new gVector4<T>(z, y, z, x);
        
        /// <summary>
        /// Returns gVector4.zyzy swizzling.
        /// </summary>
        public gVector4<T> zyzy => new gVector4<T>(z, y, z, y);
        
        /// <summary>
        /// Returns gVector4.bgbg swizzling (equivalent to gVector4.zyzy).
        /// </summary>
        public gVector4<T> bgbg => new gVector4<T>(z, y, z, y);
        
        /// <summary>
        /// Returns gVector4.zyzz swizzling.
        /// </summary>
        public gVector4<T> zyzz => new gVector4<T>(z, y, z, z);
        
        /// <summary>
        /// Returns gVector4.bgbb swizzling (equivalent to gVector4.zyzz).
        /// </summary>
        public gVector4<T> bgbb => new gVector4<T>(z, y, z, z);
        
        /// <summary>
        /// Returns gVector4.zyzw swizzling.
        /// </summary>
        public gVector4<T> zyzw => new gVector4<T>(z, y, z, w);
        
        /// <summary>
        /// Returns gVector4.bgba swizzling (equivalent to gVector4.zyzw).
        /// </summary>
        public gVector4<T> bgba => new gVector4<T>(z, y, z, w);
        
        /// <summary>
        /// Returns gVector4.zyw swizzling.
        /// </summary>
        public gVector3<T> zyw => new gVector3<T>(z, y, w);
        
        /// <summary>
        /// Returns gVector4.bga swizzling (equivalent to gVector4.zyw).
        /// </summary>
        public gVector3<T> bga => new gVector3<T>(z, y, w);
        
        /// <summary>
        /// Returns gVector4.zywx swizzling.
        /// </summary>
        public gVector4<T> zywx => new gVector4<T>(z, y, w, x);
        
        /// <summary>
        /// Returns gVector4.bgar swizzling (equivalent to gVector4.zywx).
        /// </summary>
        public gVector4<T> bgar => new gVector4<T>(z, y, w, x);
        
        /// <summary>
        /// Returns gVector4.zywy swizzling.
        /// </summary>
        public gVector4<T> zywy => new gVector4<T>(z, y, w, y);
        
        /// <summary>
        /// Returns gVector4.bgag swizzling (equivalent to gVector4.zywy).
        /// </summary>
        public gVector4<T> bgag => new gVector4<T>(z, y, w, y);
        
        /// <summary>
        /// Returns gVector4.zywz swizzling.
        /// </summary>
        public gVector4<T> zywz => new gVector4<T>(z, y, w, z);
        
        /// <summary>
        /// Returns gVector4.bgab swizzling (equivalent to gVector4.zywz).
        /// </summary>
        public gVector4<T> bgab => new gVector4<T>(z, y, w, z);
        
        /// <summary>
        /// Returns gVector4.zyww swizzling.
        /// </summary>
        public gVector4<T> zyww => new gVector4<T>(z, y, w, w);
        
        /// <summary>
        /// Returns gVector4.bgaa swizzling (equivalent to gVector4.zyww).
        /// </summary>
        public gVector4<T> bgaa => new gVector4<T>(z, y, w, w);
        
        /// <summary>
        /// Returns gVector4.zz swizzling.
        /// </summary>
        public gVector2<T> zz => new gVector2<T>(z, z);
        
        /// <summary>
        /// Returns gVector4.bb swizzling (equivalent to gVector4.zz).
        /// </summary>
        public gVector2<T> bb => new gVector2<T>(z, z);
        
        /// <summary>
        /// Returns gVector4.zzx swizzling.
        /// </summary>
        public gVector3<T> zzx => new gVector3<T>(z, z, x);
        
        /// <summary>
        /// Returns gVector4.bbr swizzling (equivalent to gVector4.zzx).
        /// </summary>
        public gVector3<T> bbr => new gVector3<T>(z, z, x);
        
        /// <summary>
        /// Returns gVector4.zzxx swizzling.
        /// </summary>
        public gVector4<T> zzxx => new gVector4<T>(z, z, x, x);
        
        /// <summary>
        /// Returns gVector4.bbrr swizzling (equivalent to gVector4.zzxx).
        /// </summary>
        public gVector4<T> bbrr => new gVector4<T>(z, z, x, x);
        
        /// <summary>
        /// Returns gVector4.zzxy swizzling.
        /// </summary>
        public gVector4<T> zzxy => new gVector4<T>(z, z, x, y);
        
        /// <summary>
        /// Returns gVector4.bbrg swizzling (equivalent to gVector4.zzxy).
        /// </summary>
        public gVector4<T> bbrg => new gVector4<T>(z, z, x, y);
        
        /// <summary>
        /// Returns gVector4.zzxz swizzling.
        /// </summary>
        public gVector4<T> zzxz => new gVector4<T>(z, z, x, z);
        
        /// <summary>
        /// Returns gVector4.bbrb swizzling (equivalent to gVector4.zzxz).
        /// </summary>
        public gVector4<T> bbrb => new gVector4<T>(z, z, x, z);
        
        /// <summary>
        /// Returns gVector4.zzxw swizzling.
        /// </summary>
        public gVector4<T> zzxw => new gVector4<T>(z, z, x, w);
        
        /// <summary>
        /// Returns gVector4.bbra swizzling (equivalent to gVector4.zzxw).
        /// </summary>
        public gVector4<T> bbra => new gVector4<T>(z, z, x, w);
        
        /// <summary>
        /// Returns gVector4.zzy swizzling.
        /// </summary>
        public gVector3<T> zzy => new gVector3<T>(z, z, y);
        
        /// <summary>
        /// Returns gVector4.bbg swizzling (equivalent to gVector4.zzy).
        /// </summary>
        public gVector3<T> bbg => new gVector3<T>(z, z, y);
        
        /// <summary>
        /// Returns gVector4.zzyx swizzling.
        /// </summary>
        public gVector4<T> zzyx => new gVector4<T>(z, z, y, x);
        
        /// <summary>
        /// Returns gVector4.bbgr swizzling (equivalent to gVector4.zzyx).
        /// </summary>
        public gVector4<T> bbgr => new gVector4<T>(z, z, y, x);
        
        /// <summary>
        /// Returns gVector4.zzyy swizzling.
        /// </summary>
        public gVector4<T> zzyy => new gVector4<T>(z, z, y, y);
        
        /// <summary>
        /// Returns gVector4.bbgg swizzling (equivalent to gVector4.zzyy).
        /// </summary>
        public gVector4<T> bbgg => new gVector4<T>(z, z, y, y);
        
        /// <summary>
        /// Returns gVector4.zzyz swizzling.
        /// </summary>
        public gVector4<T> zzyz => new gVector4<T>(z, z, y, z);
        
        /// <summary>
        /// Returns gVector4.bbgb swizzling (equivalent to gVector4.zzyz).
        /// </summary>
        public gVector4<T> bbgb => new gVector4<T>(z, z, y, z);
        
        /// <summary>
        /// Returns gVector4.zzyw swizzling.
        /// </summary>
        public gVector4<T> zzyw => new gVector4<T>(z, z, y, w);
        
        /// <summary>
        /// Returns gVector4.bbga swizzling (equivalent to gVector4.zzyw).
        /// </summary>
        public gVector4<T> bbga => new gVector4<T>(z, z, y, w);
        
        /// <summary>
        /// Returns gVector4.zzz swizzling.
        /// </summary>
        public gVector3<T> zzz => new gVector3<T>(z, z, z);
        
        /// <summary>
        /// Returns gVector4.bbb swizzling (equivalent to gVector4.zzz).
        /// </summary>
        public gVector3<T> bbb => new gVector3<T>(z, z, z);
        
        /// <summary>
        /// Returns gVector4.zzzx swizzling.
        /// </summary>
        public gVector4<T> zzzx => new gVector4<T>(z, z, z, x);
        
        /// <summary>
        /// Returns gVector4.bbbr swizzling (equivalent to gVector4.zzzx).
        /// </summary>
        public gVector4<T> bbbr => new gVector4<T>(z, z, z, x);
        
        /// <summary>
        /// Returns gVector4.zzzy swizzling.
        /// </summary>
        public gVector4<T> zzzy => new gVector4<T>(z, z, z, y);
        
        /// <summary>
        /// Returns gVector4.bbbg swizzling (equivalent to gVector4.zzzy).
        /// </summary>
        public gVector4<T> bbbg => new gVector4<T>(z, z, z, y);
        
        /// <summary>
        /// Returns gVector4.zzzz swizzling.
        /// </summary>
        public gVector4<T> zzzz => new gVector4<T>(z, z, z, z);
        
        /// <summary>
        /// Returns gVector4.bbbb swizzling (equivalent to gVector4.zzzz).
        /// </summary>
        public gVector4<T> bbbb => new gVector4<T>(z, z, z, z);
        
        /// <summary>
        /// Returns gVector4.zzzw swizzling.
        /// </summary>
        public gVector4<T> zzzw => new gVector4<T>(z, z, z, w);
        
        /// <summary>
        /// Returns gVector4.bbba swizzling (equivalent to gVector4.zzzw).
        /// </summary>
        public gVector4<T> bbba => new gVector4<T>(z, z, z, w);
        
        /// <summary>
        /// Returns gVector4.zzw swizzling.
        /// </summary>
        public gVector3<T> zzw => new gVector3<T>(z, z, w);
        
        /// <summary>
        /// Returns gVector4.bba swizzling (equivalent to gVector4.zzw).
        /// </summary>
        public gVector3<T> bba => new gVector3<T>(z, z, w);
        
        /// <summary>
        /// Returns gVector4.zzwx swizzling.
        /// </summary>
        public gVector4<T> zzwx => new gVector4<T>(z, z, w, x);
        
        /// <summary>
        /// Returns gVector4.bbar swizzling (equivalent to gVector4.zzwx).
        /// </summary>
        public gVector4<T> bbar => new gVector4<T>(z, z, w, x);
        
        /// <summary>
        /// Returns gVector4.zzwy swizzling.
        /// </summary>
        public gVector4<T> zzwy => new gVector4<T>(z, z, w, y);
        
        /// <summary>
        /// Returns gVector4.bbag swizzling (equivalent to gVector4.zzwy).
        /// </summary>
        public gVector4<T> bbag => new gVector4<T>(z, z, w, y);
        
        /// <summary>
        /// Returns gVector4.zzwz swizzling.
        /// </summary>
        public gVector4<T> zzwz => new gVector4<T>(z, z, w, z);
        
        /// <summary>
        /// Returns gVector4.bbab swizzling (equivalent to gVector4.zzwz).
        /// </summary>
        public gVector4<T> bbab => new gVector4<T>(z, z, w, z);
        
        /// <summary>
        /// Returns gVector4.zzww swizzling.
        /// </summary>
        public gVector4<T> zzww => new gVector4<T>(z, z, w, w);
        
        /// <summary>
        /// Returns gVector4.bbaa swizzling (equivalent to gVector4.zzww).
        /// </summary>
        public gVector4<T> bbaa => new gVector4<T>(z, z, w, w);
        
        /// <summary>
        /// Returns gVector4.zw swizzling.
        /// </summary>
        public gVector2<T> zw => new gVector2<T>(z, w);
        
        /// <summary>
        /// Returns gVector4.ba swizzling (equivalent to gVector4.zw).
        /// </summary>
        public gVector2<T> ba => new gVector2<T>(z, w);
        
        /// <summary>
        /// Returns gVector4.zwx swizzling.
        /// </summary>
        public gVector3<T> zwx => new gVector3<T>(z, w, x);
        
        /// <summary>
        /// Returns gVector4.bar swizzling (equivalent to gVector4.zwx).
        /// </summary>
        public gVector3<T> bar => new gVector3<T>(z, w, x);
        
        /// <summary>
        /// Returns gVector4.zwxx swizzling.
        /// </summary>
        public gVector4<T> zwxx => new gVector4<T>(z, w, x, x);
        
        /// <summary>
        /// Returns gVector4.barr swizzling (equivalent to gVector4.zwxx).
        /// </summary>
        public gVector4<T> barr => new gVector4<T>(z, w, x, x);
        
        /// <summary>
        /// Returns gVector4.zwxy swizzling.
        /// </summary>
        public gVector4<T> zwxy => new gVector4<T>(z, w, x, y);
        
        /// <summary>
        /// Returns gVector4.barg swizzling (equivalent to gVector4.zwxy).
        /// </summary>
        public gVector4<T> barg => new gVector4<T>(z, w, x, y);
        
        /// <summary>
        /// Returns gVector4.zwxz swizzling.
        /// </summary>
        public gVector4<T> zwxz => new gVector4<T>(z, w, x, z);
        
        /// <summary>
        /// Returns gVector4.barb swizzling (equivalent to gVector4.zwxz).
        /// </summary>
        public gVector4<T> barb => new gVector4<T>(z, w, x, z);
        
        /// <summary>
        /// Returns gVector4.zwxw swizzling.
        /// </summary>
        public gVector4<T> zwxw => new gVector4<T>(z, w, x, w);
        
        /// <summary>
        /// Returns gVector4.bara swizzling (equivalent to gVector4.zwxw).
        /// </summary>
        public gVector4<T> bara => new gVector4<T>(z, w, x, w);
        
        /// <summary>
        /// Returns gVector4.zwy swizzling.
        /// </summary>
        public gVector3<T> zwy => new gVector3<T>(z, w, y);
        
        /// <summary>
        /// Returns gVector4.bag swizzling (equivalent to gVector4.zwy).
        /// </summary>
        public gVector3<T> bag => new gVector3<T>(z, w, y);
        
        /// <summary>
        /// Returns gVector4.zwyx swizzling.
        /// </summary>
        public gVector4<T> zwyx => new gVector4<T>(z, w, y, x);
        
        /// <summary>
        /// Returns gVector4.bagr swizzling (equivalent to gVector4.zwyx).
        /// </summary>
        public gVector4<T> bagr => new gVector4<T>(z, w, y, x);
        
        /// <summary>
        /// Returns gVector4.zwyy swizzling.
        /// </summary>
        public gVector4<T> zwyy => new gVector4<T>(z, w, y, y);
        
        /// <summary>
        /// Returns gVector4.bagg swizzling (equivalent to gVector4.zwyy).
        /// </summary>
        public gVector4<T> bagg => new gVector4<T>(z, w, y, y);
        
        /// <summary>
        /// Returns gVector4.zwyz swizzling.
        /// </summary>
        public gVector4<T> zwyz => new gVector4<T>(z, w, y, z);
        
        /// <summary>
        /// Returns gVector4.bagb swizzling (equivalent to gVector4.zwyz).
        /// </summary>
        public gVector4<T> bagb => new gVector4<T>(z, w, y, z);
        
        /// <summary>
        /// Returns gVector4.zwyw swizzling.
        /// </summary>
        public gVector4<T> zwyw => new gVector4<T>(z, w, y, w);
        
        /// <summary>
        /// Returns gVector4.baga swizzling (equivalent to gVector4.zwyw).
        /// </summary>
        public gVector4<T> baga => new gVector4<T>(z, w, y, w);
        
        /// <summary>
        /// Returns gVector4.zwz swizzling.
        /// </summary>
        public gVector3<T> zwz => new gVector3<T>(z, w, z);
        
        /// <summary>
        /// Returns gVector4.bab swizzling (equivalent to gVector4.zwz).
        /// </summary>
        public gVector3<T> bab => new gVector3<T>(z, w, z);
        
        /// <summary>
        /// Returns gVector4.zwzx swizzling.
        /// </summary>
        public gVector4<T> zwzx => new gVector4<T>(z, w, z, x);
        
        /// <summary>
        /// Returns gVector4.babr swizzling (equivalent to gVector4.zwzx).
        /// </summary>
        public gVector4<T> babr => new gVector4<T>(z, w, z, x);
        
        /// <summary>
        /// Returns gVector4.zwzy swizzling.
        /// </summary>
        public gVector4<T> zwzy => new gVector4<T>(z, w, z, y);
        
        /// <summary>
        /// Returns gVector4.babg swizzling (equivalent to gVector4.zwzy).
        /// </summary>
        public gVector4<T> babg => new gVector4<T>(z, w, z, y);
        
        /// <summary>
        /// Returns gVector4.zwzz swizzling.
        /// </summary>
        public gVector4<T> zwzz => new gVector4<T>(z, w, z, z);
        
        /// <summary>
        /// Returns gVector4.babb swizzling (equivalent to gVector4.zwzz).
        /// </summary>
        public gVector4<T> babb => new gVector4<T>(z, w, z, z);
        
        /// <summary>
        /// Returns gVector4.zwzw swizzling.
        /// </summary>
        public gVector4<T> zwzw => new gVector4<T>(z, w, z, w);
        
        /// <summary>
        /// Returns gVector4.baba swizzling (equivalent to gVector4.zwzw).
        /// </summary>
        public gVector4<T> baba => new gVector4<T>(z, w, z, w);
        
        /// <summary>
        /// Returns gVector4.zww swizzling.
        /// </summary>
        public gVector3<T> zww => new gVector3<T>(z, w, w);
        
        /// <summary>
        /// Returns gVector4.baa swizzling (equivalent to gVector4.zww).
        /// </summary>
        public gVector3<T> baa => new gVector3<T>(z, w, w);
        
        /// <summary>
        /// Returns gVector4.zwwx swizzling.
        /// </summary>
        public gVector4<T> zwwx => new gVector4<T>(z, w, w, x);
        
        /// <summary>
        /// Returns gVector4.baar swizzling (equivalent to gVector4.zwwx).
        /// </summary>
        public gVector4<T> baar => new gVector4<T>(z, w, w, x);
        
        /// <summary>
        /// Returns gVector4.zwwy swizzling.
        /// </summary>
        public gVector4<T> zwwy => new gVector4<T>(z, w, w, y);
        
        /// <summary>
        /// Returns gVector4.baag swizzling (equivalent to gVector4.zwwy).
        /// </summary>
        public gVector4<T> baag => new gVector4<T>(z, w, w, y);
        
        /// <summary>
        /// Returns gVector4.zwwz swizzling.
        /// </summary>
        public gVector4<T> zwwz => new gVector4<T>(z, w, w, z);
        
        /// <summary>
        /// Returns gVector4.baab swizzling (equivalent to gVector4.zwwz).
        /// </summary>
        public gVector4<T> baab => new gVector4<T>(z, w, w, z);
        
        /// <summary>
        /// Returns gVector4.zwww swizzling.
        /// </summary>
        public gVector4<T> zwww => new gVector4<T>(z, w, w, w);
        
        /// <summary>
        /// Returns gVector4.baaa swizzling (equivalent to gVector4.zwww).
        /// </summary>
        public gVector4<T> baaa => new gVector4<T>(z, w, w, w);
        
        /// <summary>
        /// Returns gVector4.wx swizzling.
        /// </summary>
        public gVector2<T> wx => new gVector2<T>(w, x);
        
        /// <summary>
        /// Returns gVector4.ar swizzling (equivalent to gVector4.wx).
        /// </summary>
        public gVector2<T> ar => new gVector2<T>(w, x);
        
        /// <summary>
        /// Returns gVector4.wxx swizzling.
        /// </summary>
        public gVector3<T> wxx => new gVector3<T>(w, x, x);
        
        /// <summary>
        /// Returns gVector4.arr swizzling (equivalent to gVector4.wxx).
        /// </summary>
        public gVector3<T> arr => new gVector3<T>(w, x, x);
        
        /// <summary>
        /// Returns gVector4.wxxx swizzling.
        /// </summary>
        public gVector4<T> wxxx => new gVector4<T>(w, x, x, x);
        
        /// <summary>
        /// Returns gVector4.arrr swizzling (equivalent to gVector4.wxxx).
        /// </summary>
        public gVector4<T> arrr => new gVector4<T>(w, x, x, x);
        
        /// <summary>
        /// Returns gVector4.wxxy swizzling.
        /// </summary>
        public gVector4<T> wxxy => new gVector4<T>(w, x, x, y);
        
        /// <summary>
        /// Returns gVector4.arrg swizzling (equivalent to gVector4.wxxy).
        /// </summary>
        public gVector4<T> arrg => new gVector4<T>(w, x, x, y);
        
        /// <summary>
        /// Returns gVector4.wxxz swizzling.
        /// </summary>
        public gVector4<T> wxxz => new gVector4<T>(w, x, x, z);
        
        /// <summary>
        /// Returns gVector4.arrb swizzling (equivalent to gVector4.wxxz).
        /// </summary>
        public gVector4<T> arrb => new gVector4<T>(w, x, x, z);
        
        /// <summary>
        /// Returns gVector4.wxxw swizzling.
        /// </summary>
        public gVector4<T> wxxw => new gVector4<T>(w, x, x, w);
        
        /// <summary>
        /// Returns gVector4.arra swizzling (equivalent to gVector4.wxxw).
        /// </summary>
        public gVector4<T> arra => new gVector4<T>(w, x, x, w);
        
        /// <summary>
        /// Returns gVector4.wxy swizzling.
        /// </summary>
        public gVector3<T> wxy => new gVector3<T>(w, x, y);
        
        /// <summary>
        /// Returns gVector4.arg swizzling (equivalent to gVector4.wxy).
        /// </summary>
        public gVector3<T> arg => new gVector3<T>(w, x, y);
        
        /// <summary>
        /// Returns gVector4.wxyx swizzling.
        /// </summary>
        public gVector4<T> wxyx => new gVector4<T>(w, x, y, x);
        
        /// <summary>
        /// Returns gVector4.argr swizzling (equivalent to gVector4.wxyx).
        /// </summary>
        public gVector4<T> argr => new gVector4<T>(w, x, y, x);
        
        /// <summary>
        /// Returns gVector4.wxyy swizzling.
        /// </summary>
        public gVector4<T> wxyy => new gVector4<T>(w, x, y, y);
        
        /// <summary>
        /// Returns gVector4.argg swizzling (equivalent to gVector4.wxyy).
        /// </summary>
        public gVector4<T> argg => new gVector4<T>(w, x, y, y);
        
        /// <summary>
        /// Returns gVector4.wxyz swizzling.
        /// </summary>
        public gVector4<T> wxyz => new gVector4<T>(w, x, y, z);
        
        /// <summary>
        /// Returns gVector4.argb swizzling (equivalent to gVector4.wxyz).
        /// </summary>
        public gVector4<T> argb => new gVector4<T>(w, x, y, z);
        
        /// <summary>
        /// Returns gVector4.wxyw swizzling.
        /// </summary>
        public gVector4<T> wxyw => new gVector4<T>(w, x, y, w);
        
        /// <summary>
        /// Returns gVector4.arga swizzling (equivalent to gVector4.wxyw).
        /// </summary>
        public gVector4<T> arga => new gVector4<T>(w, x, y, w);
        
        /// <summary>
        /// Returns gVector4.wxz swizzling.
        /// </summary>
        public gVector3<T> wxz => new gVector3<T>(w, x, z);
        
        /// <summary>
        /// Returns gVector4.arb swizzling (equivalent to gVector4.wxz).
        /// </summary>
        public gVector3<T> arb => new gVector3<T>(w, x, z);
        
        /// <summary>
        /// Returns gVector4.wxzx swizzling.
        /// </summary>
        public gVector4<T> wxzx => new gVector4<T>(w, x, z, x);
        
        /// <summary>
        /// Returns gVector4.arbr swizzling (equivalent to gVector4.wxzx).
        /// </summary>
        public gVector4<T> arbr => new gVector4<T>(w, x, z, x);
        
        /// <summary>
        /// Returns gVector4.wxzy swizzling.
        /// </summary>
        public gVector4<T> wxzy => new gVector4<T>(w, x, z, y);
        
        /// <summary>
        /// Returns gVector4.arbg swizzling (equivalent to gVector4.wxzy).
        /// </summary>
        public gVector4<T> arbg => new gVector4<T>(w, x, z, y);
        
        /// <summary>
        /// Returns gVector4.wxzz swizzling.
        /// </summary>
        public gVector4<T> wxzz => new gVector4<T>(w, x, z, z);
        
        /// <summary>
        /// Returns gVector4.arbb swizzling (equivalent to gVector4.wxzz).
        /// </summary>
        public gVector4<T> arbb => new gVector4<T>(w, x, z, z);
        
        /// <summary>
        /// Returns gVector4.wxzw swizzling.
        /// </summary>
        public gVector4<T> wxzw => new gVector4<T>(w, x, z, w);
        
        /// <summary>
        /// Returns gVector4.arba swizzling (equivalent to gVector4.wxzw).
        /// </summary>
        public gVector4<T> arba => new gVector4<T>(w, x, z, w);
        
        /// <summary>
        /// Returns gVector4.wxw swizzling.
        /// </summary>
        public gVector3<T> wxw => new gVector3<T>(w, x, w);
        
        /// <summary>
        /// Returns gVector4.ara swizzling (equivalent to gVector4.wxw).
        /// </summary>
        public gVector3<T> ara => new gVector3<T>(w, x, w);
        
        /// <summary>
        /// Returns gVector4.wxwx swizzling.
        /// </summary>
        public gVector4<T> wxwx => new gVector4<T>(w, x, w, x);
        
        /// <summary>
        /// Returns gVector4.arar swizzling (equivalent to gVector4.wxwx).
        /// </summary>
        public gVector4<T> arar => new gVector4<T>(w, x, w, x);
        
        /// <summary>
        /// Returns gVector4.wxwy swizzling.
        /// </summary>
        public gVector4<T> wxwy => new gVector4<T>(w, x, w, y);
        
        /// <summary>
        /// Returns gVector4.arag swizzling (equivalent to gVector4.wxwy).
        /// </summary>
        public gVector4<T> arag => new gVector4<T>(w, x, w, y);
        
        /// <summary>
        /// Returns gVector4.wxwz swizzling.
        /// </summary>
        public gVector4<T> wxwz => new gVector4<T>(w, x, w, z);
        
        /// <summary>
        /// Returns gVector4.arab swizzling (equivalent to gVector4.wxwz).
        /// </summary>
        public gVector4<T> arab => new gVector4<T>(w, x, w, z);
        
        /// <summary>
        /// Returns gVector4.wxww swizzling.
        /// </summary>
        public gVector4<T> wxww => new gVector4<T>(w, x, w, w);
        
        /// <summary>
        /// Returns gVector4.araa swizzling (equivalent to gVector4.wxww).
        /// </summary>
        public gVector4<T> araa => new gVector4<T>(w, x, w, w);
        
        /// <summary>
        /// Returns gVector4.wy swizzling.
        /// </summary>
        public gVector2<T> wy => new gVector2<T>(w, y);
        
        /// <summary>
        /// Returns gVector4.ag swizzling (equivalent to gVector4.wy).
        /// </summary>
        public gVector2<T> ag => new gVector2<T>(w, y);
        
        /// <summary>
        /// Returns gVector4.wyx swizzling.
        /// </summary>
        public gVector3<T> wyx => new gVector3<T>(w, y, x);
        
        /// <summary>
        /// Returns gVector4.agr swizzling (equivalent to gVector4.wyx).
        /// </summary>
        public gVector3<T> agr => new gVector3<T>(w, y, x);
        
        /// <summary>
        /// Returns gVector4.wyxx swizzling.
        /// </summary>
        public gVector4<T> wyxx => new gVector4<T>(w, y, x, x);
        
        /// <summary>
        /// Returns gVector4.agrr swizzling (equivalent to gVector4.wyxx).
        /// </summary>
        public gVector4<T> agrr => new gVector4<T>(w, y, x, x);
        
        /// <summary>
        /// Returns gVector4.wyxy swizzling.
        /// </summary>
        public gVector4<T> wyxy => new gVector4<T>(w, y, x, y);
        
        /// <summary>
        /// Returns gVector4.agrg swizzling (equivalent to gVector4.wyxy).
        /// </summary>
        public gVector4<T> agrg => new gVector4<T>(w, y, x, y);
        
        /// <summary>
        /// Returns gVector4.wyxz swizzling.
        /// </summary>
        public gVector4<T> wyxz => new gVector4<T>(w, y, x, z);
        
        /// <summary>
        /// Returns gVector4.agrb swizzling (equivalent to gVector4.wyxz).
        /// </summary>
        public gVector4<T> agrb => new gVector4<T>(w, y, x, z);
        
        /// <summary>
        /// Returns gVector4.wyxw swizzling.
        /// </summary>
        public gVector4<T> wyxw => new gVector4<T>(w, y, x, w);
        
        /// <summary>
        /// Returns gVector4.agra swizzling (equivalent to gVector4.wyxw).
        /// </summary>
        public gVector4<T> agra => new gVector4<T>(w, y, x, w);
        
        /// <summary>
        /// Returns gVector4.wyy swizzling.
        /// </summary>
        public gVector3<T> wyy => new gVector3<T>(w, y, y);
        
        /// <summary>
        /// Returns gVector4.agg swizzling (equivalent to gVector4.wyy).
        /// </summary>
        public gVector3<T> agg => new gVector3<T>(w, y, y);
        
        /// <summary>
        /// Returns gVector4.wyyx swizzling.
        /// </summary>
        public gVector4<T> wyyx => new gVector4<T>(w, y, y, x);
        
        /// <summary>
        /// Returns gVector4.aggr swizzling (equivalent to gVector4.wyyx).
        /// </summary>
        public gVector4<T> aggr => new gVector4<T>(w, y, y, x);
        
        /// <summary>
        /// Returns gVector4.wyyy swizzling.
        /// </summary>
        public gVector4<T> wyyy => new gVector4<T>(w, y, y, y);
        
        /// <summary>
        /// Returns gVector4.aggg swizzling (equivalent to gVector4.wyyy).
        /// </summary>
        public gVector4<T> aggg => new gVector4<T>(w, y, y, y);
        
        /// <summary>
        /// Returns gVector4.wyyz swizzling.
        /// </summary>
        public gVector4<T> wyyz => new gVector4<T>(w, y, y, z);
        
        /// <summary>
        /// Returns gVector4.aggb swizzling (equivalent to gVector4.wyyz).
        /// </summary>
        public gVector4<T> aggb => new gVector4<T>(w, y, y, z);
        
        /// <summary>
        /// Returns gVector4.wyyw swizzling.
        /// </summary>
        public gVector4<T> wyyw => new gVector4<T>(w, y, y, w);
        
        /// <summary>
        /// Returns gVector4.agga swizzling (equivalent to gVector4.wyyw).
        /// </summary>
        public gVector4<T> agga => new gVector4<T>(w, y, y, w);
        
        /// <summary>
        /// Returns gVector4.wyz swizzling.
        /// </summary>
        public gVector3<T> wyz => new gVector3<T>(w, y, z);
        
        /// <summary>
        /// Returns gVector4.agb swizzling (equivalent to gVector4.wyz).
        /// </summary>
        public gVector3<T> agb => new gVector3<T>(w, y, z);
        
        /// <summary>
        /// Returns gVector4.wyzx swizzling.
        /// </summary>
        public gVector4<T> wyzx => new gVector4<T>(w, y, z, x);
        
        /// <summary>
        /// Returns gVector4.agbr swizzling (equivalent to gVector4.wyzx).
        /// </summary>
        public gVector4<T> agbr => new gVector4<T>(w, y, z, x);
        
        /// <summary>
        /// Returns gVector4.wyzy swizzling.
        /// </summary>
        public gVector4<T> wyzy => new gVector4<T>(w, y, z, y);
        
        /// <summary>
        /// Returns gVector4.agbg swizzling (equivalent to gVector4.wyzy).
        /// </summary>
        public gVector4<T> agbg => new gVector4<T>(w, y, z, y);
        
        /// <summary>
        /// Returns gVector4.wyzz swizzling.
        /// </summary>
        public gVector4<T> wyzz => new gVector4<T>(w, y, z, z);
        
        /// <summary>
        /// Returns gVector4.agbb swizzling (equivalent to gVector4.wyzz).
        /// </summary>
        public gVector4<T> agbb => new gVector4<T>(w, y, z, z);
        
        /// <summary>
        /// Returns gVector4.wyzw swizzling.
        /// </summary>
        public gVector4<T> wyzw => new gVector4<T>(w, y, z, w);
        
        /// <summary>
        /// Returns gVector4.agba swizzling (equivalent to gVector4.wyzw).
        /// </summary>
        public gVector4<T> agba => new gVector4<T>(w, y, z, w);
        
        /// <summary>
        /// Returns gVector4.wyw swizzling.
        /// </summary>
        public gVector3<T> wyw => new gVector3<T>(w, y, w);
        
        /// <summary>
        /// Returns gVector4.aga swizzling (equivalent to gVector4.wyw).
        /// </summary>
        public gVector3<T> aga => new gVector3<T>(w, y, w);
        
        /// <summary>
        /// Returns gVector4.wywx swizzling.
        /// </summary>
        public gVector4<T> wywx => new gVector4<T>(w, y, w, x);
        
        /// <summary>
        /// Returns gVector4.agar swizzling (equivalent to gVector4.wywx).
        /// </summary>
        public gVector4<T> agar => new gVector4<T>(w, y, w, x);
        
        /// <summary>
        /// Returns gVector4.wywy swizzling.
        /// </summary>
        public gVector4<T> wywy => new gVector4<T>(w, y, w, y);
        
        /// <summary>
        /// Returns gVector4.agag swizzling (equivalent to gVector4.wywy).
        /// </summary>
        public gVector4<T> agag => new gVector4<T>(w, y, w, y);
        
        /// <summary>
        /// Returns gVector4.wywz swizzling.
        /// </summary>
        public gVector4<T> wywz => new gVector4<T>(w, y, w, z);
        
        /// <summary>
        /// Returns gVector4.agab swizzling (equivalent to gVector4.wywz).
        /// </summary>
        public gVector4<T> agab => new gVector4<T>(w, y, w, z);
        
        /// <summary>
        /// Returns gVector4.wyww swizzling.
        /// </summary>
        public gVector4<T> wyww => new gVector4<T>(w, y, w, w);
        
        /// <summary>
        /// Returns gVector4.agaa swizzling (equivalent to gVector4.wyww).
        /// </summary>
        public gVector4<T> agaa => new gVector4<T>(w, y, w, w);
        
        /// <summary>
        /// Returns gVector4.wz swizzling.
        /// </summary>
        public gVector2<T> wz => new gVector2<T>(w, z);
        
        /// <summary>
        /// Returns gVector4.ab swizzling (equivalent to gVector4.wz).
        /// </summary>
        public gVector2<T> ab => new gVector2<T>(w, z);
        
        /// <summary>
        /// Returns gVector4.wzx swizzling.
        /// </summary>
        public gVector3<T> wzx => new gVector3<T>(w, z, x);
        
        /// <summary>
        /// Returns gVector4.abr swizzling (equivalent to gVector4.wzx).
        /// </summary>
        public gVector3<T> abr => new gVector3<T>(w, z, x);
        
        /// <summary>
        /// Returns gVector4.wzxx swizzling.
        /// </summary>
        public gVector4<T> wzxx => new gVector4<T>(w, z, x, x);
        
        /// <summary>
        /// Returns gVector4.abrr swizzling (equivalent to gVector4.wzxx).
        /// </summary>
        public gVector4<T> abrr => new gVector4<T>(w, z, x, x);
        
        /// <summary>
        /// Returns gVector4.wzxy swizzling.
        /// </summary>
        public gVector4<T> wzxy => new gVector4<T>(w, z, x, y);
        
        /// <summary>
        /// Returns gVector4.abrg swizzling (equivalent to gVector4.wzxy).
        /// </summary>
        public gVector4<T> abrg => new gVector4<T>(w, z, x, y);
        
        /// <summary>
        /// Returns gVector4.wzxz swizzling.
        /// </summary>
        public gVector4<T> wzxz => new gVector4<T>(w, z, x, z);
        
        /// <summary>
        /// Returns gVector4.abrb swizzling (equivalent to gVector4.wzxz).
        /// </summary>
        public gVector4<T> abrb => new gVector4<T>(w, z, x, z);
        
        /// <summary>
        /// Returns gVector4.wzxw swizzling.
        /// </summary>
        public gVector4<T> wzxw => new gVector4<T>(w, z, x, w);
        
        /// <summary>
        /// Returns gVector4.abra swizzling (equivalent to gVector4.wzxw).
        /// </summary>
        public gVector4<T> abra => new gVector4<T>(w, z, x, w);
        
        /// <summary>
        /// Returns gVector4.wzy swizzling.
        /// </summary>
        public gVector3<T> wzy => new gVector3<T>(w, z, y);
        
        /// <summary>
        /// Returns gVector4.abg swizzling (equivalent to gVector4.wzy).
        /// </summary>
        public gVector3<T> abg => new gVector3<T>(w, z, y);
        
        /// <summary>
        /// Returns gVector4.wzyx swizzling.
        /// </summary>
        public gVector4<T> wzyx => new gVector4<T>(w, z, y, x);
        
        /// <summary>
        /// Returns gVector4.abgr swizzling (equivalent to gVector4.wzyx).
        /// </summary>
        public gVector4<T> abgr => new gVector4<T>(w, z, y, x);
        
        /// <summary>
        /// Returns gVector4.wzyy swizzling.
        /// </summary>
        public gVector4<T> wzyy => new gVector4<T>(w, z, y, y);
        
        /// <summary>
        /// Returns gVector4.abgg swizzling (equivalent to gVector4.wzyy).
        /// </summary>
        public gVector4<T> abgg => new gVector4<T>(w, z, y, y);
        
        /// <summary>
        /// Returns gVector4.wzyz swizzling.
        /// </summary>
        public gVector4<T> wzyz => new gVector4<T>(w, z, y, z);
        
        /// <summary>
        /// Returns gVector4.abgb swizzling (equivalent to gVector4.wzyz).
        /// </summary>
        public gVector4<T> abgb => new gVector4<T>(w, z, y, z);
        
        /// <summary>
        /// Returns gVector4.wzyw swizzling.
        /// </summary>
        public gVector4<T> wzyw => new gVector4<T>(w, z, y, w);
        
        /// <summary>
        /// Returns gVector4.abga swizzling (equivalent to gVector4.wzyw).
        /// </summary>
        public gVector4<T> abga => new gVector4<T>(w, z, y, w);
        
        /// <summary>
        /// Returns gVector4.wzz swizzling.
        /// </summary>
        public gVector3<T> wzz => new gVector3<T>(w, z, z);
        
        /// <summary>
        /// Returns gVector4.abb swizzling (equivalent to gVector4.wzz).
        /// </summary>
        public gVector3<T> abb => new gVector3<T>(w, z, z);
        
        /// <summary>
        /// Returns gVector4.wzzx swizzling.
        /// </summary>
        public gVector4<T> wzzx => new gVector4<T>(w, z, z, x);
        
        /// <summary>
        /// Returns gVector4.abbr swizzling (equivalent to gVector4.wzzx).
        /// </summary>
        public gVector4<T> abbr => new gVector4<T>(w, z, z, x);
        
        /// <summary>
        /// Returns gVector4.wzzy swizzling.
        /// </summary>
        public gVector4<T> wzzy => new gVector4<T>(w, z, z, y);
        
        /// <summary>
        /// Returns gVector4.abbg swizzling (equivalent to gVector4.wzzy).
        /// </summary>
        public gVector4<T> abbg => new gVector4<T>(w, z, z, y);
        
        /// <summary>
        /// Returns gVector4.wzzz swizzling.
        /// </summary>
        public gVector4<T> wzzz => new gVector4<T>(w, z, z, z);
        
        /// <summary>
        /// Returns gVector4.abbb swizzling (equivalent to gVector4.wzzz).
        /// </summary>
        public gVector4<T> abbb => new gVector4<T>(w, z, z, z);
        
        /// <summary>
        /// Returns gVector4.wzzw swizzling.
        /// </summary>
        public gVector4<T> wzzw => new gVector4<T>(w, z, z, w);
        
        /// <summary>
        /// Returns gVector4.abba swizzling (equivalent to gVector4.wzzw).
        /// </summary>
        public gVector4<T> abba => new gVector4<T>(w, z, z, w);
        
        /// <summary>
        /// Returns gVector4.wzw swizzling.
        /// </summary>
        public gVector3<T> wzw => new gVector3<T>(w, z, w);
        
        /// <summary>
        /// Returns gVector4.aba swizzling (equivalent to gVector4.wzw).
        /// </summary>
        public gVector3<T> aba => new gVector3<T>(w, z, w);
        
        /// <summary>
        /// Returns gVector4.wzwx swizzling.
        /// </summary>
        public gVector4<T> wzwx => new gVector4<T>(w, z, w, x);
        
        /// <summary>
        /// Returns gVector4.abar swizzling (equivalent to gVector4.wzwx).
        /// </summary>
        public gVector4<T> abar => new gVector4<T>(w, z, w, x);
        
        /// <summary>
        /// Returns gVector4.wzwy swizzling.
        /// </summary>
        public gVector4<T> wzwy => new gVector4<T>(w, z, w, y);
        
        /// <summary>
        /// Returns gVector4.abag swizzling (equivalent to gVector4.wzwy).
        /// </summary>
        public gVector4<T> abag => new gVector4<T>(w, z, w, y);
        
        /// <summary>
        /// Returns gVector4.wzwz swizzling.
        /// </summary>
        public gVector4<T> wzwz => new gVector4<T>(w, z, w, z);
        
        /// <summary>
        /// Returns gVector4.abab swizzling (equivalent to gVector4.wzwz).
        /// </summary>
        public gVector4<T> abab => new gVector4<T>(w, z, w, z);
        
        /// <summary>
        /// Returns gVector4.wzww swizzling.
        /// </summary>
        public gVector4<T> wzww => new gVector4<T>(w, z, w, w);
        
        /// <summary>
        /// Returns gVector4.abaa swizzling (equivalent to gVector4.wzww).
        /// </summary>
        public gVector4<T> abaa => new gVector4<T>(w, z, w, w);
        
        /// <summary>
        /// Returns gVector4.ww swizzling.
        /// </summary>
        public gVector2<T> ww => new gVector2<T>(w, w);
        
        /// <summary>
        /// Returns gVector4.aa swizzling (equivalent to gVector4.ww).
        /// </summary>
        public gVector2<T> aa => new gVector2<T>(w, w);
        
        /// <summary>
        /// Returns gVector4.wwx swizzling.
        /// </summary>
        public gVector3<T> wwx => new gVector3<T>(w, w, x);
        
        /// <summary>
        /// Returns gVector4.aar swizzling (equivalent to gVector4.wwx).
        /// </summary>
        public gVector3<T> aar => new gVector3<T>(w, w, x);
        
        /// <summary>
        /// Returns gVector4.wwxx swizzling.
        /// </summary>
        public gVector4<T> wwxx => new gVector4<T>(w, w, x, x);
        
        /// <summary>
        /// Returns gVector4.aarr swizzling (equivalent to gVector4.wwxx).
        /// </summary>
        public gVector4<T> aarr => new gVector4<T>(w, w, x, x);
        
        /// <summary>
        /// Returns gVector4.wwxy swizzling.
        /// </summary>
        public gVector4<T> wwxy => new gVector4<T>(w, w, x, y);
        
        /// <summary>
        /// Returns gVector4.aarg swizzling (equivalent to gVector4.wwxy).
        /// </summary>
        public gVector4<T> aarg => new gVector4<T>(w, w, x, y);
        
        /// <summary>
        /// Returns gVector4.wwxz swizzling.
        /// </summary>
        public gVector4<T> wwxz => new gVector4<T>(w, w, x, z);
        
        /// <summary>
        /// Returns gVector4.aarb swizzling (equivalent to gVector4.wwxz).
        /// </summary>
        public gVector4<T> aarb => new gVector4<T>(w, w, x, z);
        
        /// <summary>
        /// Returns gVector4.wwxw swizzling.
        /// </summary>
        public gVector4<T> wwxw => new gVector4<T>(w, w, x, w);
        
        /// <summary>
        /// Returns gVector4.aara swizzling (equivalent to gVector4.wwxw).
        /// </summary>
        public gVector4<T> aara => new gVector4<T>(w, w, x, w);
        
        /// <summary>
        /// Returns gVector4.wwy swizzling.
        /// </summary>
        public gVector3<T> wwy => new gVector3<T>(w, w, y);
        
        /// <summary>
        /// Returns gVector4.aag swizzling (equivalent to gVector4.wwy).
        /// </summary>
        public gVector3<T> aag => new gVector3<T>(w, w, y);
        
        /// <summary>
        /// Returns gVector4.wwyx swizzling.
        /// </summary>
        public gVector4<T> wwyx => new gVector4<T>(w, w, y, x);
        
        /// <summary>
        /// Returns gVector4.aagr swizzling (equivalent to gVector4.wwyx).
        /// </summary>
        public gVector4<T> aagr => new gVector4<T>(w, w, y, x);
        
        /// <summary>
        /// Returns gVector4.wwyy swizzling.
        /// </summary>
        public gVector4<T> wwyy => new gVector4<T>(w, w, y, y);
        
        /// <summary>
        /// Returns gVector4.aagg swizzling (equivalent to gVector4.wwyy).
        /// </summary>
        public gVector4<T> aagg => new gVector4<T>(w, w, y, y);
        
        /// <summary>
        /// Returns gVector4.wwyz swizzling.
        /// </summary>
        public gVector4<T> wwyz => new gVector4<T>(w, w, y, z);
        
        /// <summary>
        /// Returns gVector4.aagb swizzling (equivalent to gVector4.wwyz).
        /// </summary>
        public gVector4<T> aagb => new gVector4<T>(w, w, y, z);
        
        /// <summary>
        /// Returns gVector4.wwyw swizzling.
        /// </summary>
        public gVector4<T> wwyw => new gVector4<T>(w, w, y, w);
        
        /// <summary>
        /// Returns gVector4.aaga swizzling (equivalent to gVector4.wwyw).
        /// </summary>
        public gVector4<T> aaga => new gVector4<T>(w, w, y, w);
        
        /// <summary>
        /// Returns gVector4.wwz swizzling.
        /// </summary>
        public gVector3<T> wwz => new gVector3<T>(w, w, z);
        
        /// <summary>
        /// Returns gVector4.aab swizzling (equivalent to gVector4.wwz).
        /// </summary>
        public gVector3<T> aab => new gVector3<T>(w, w, z);
        
        /// <summary>
        /// Returns gVector4.wwzx swizzling.
        /// </summary>
        public gVector4<T> wwzx => new gVector4<T>(w, w, z, x);
        
        /// <summary>
        /// Returns gVector4.aabr swizzling (equivalent to gVector4.wwzx).
        /// </summary>
        public gVector4<T> aabr => new gVector4<T>(w, w, z, x);
        
        /// <summary>
        /// Returns gVector4.wwzy swizzling.
        /// </summary>
        public gVector4<T> wwzy => new gVector4<T>(w, w, z, y);
        
        /// <summary>
        /// Returns gVector4.aabg swizzling (equivalent to gVector4.wwzy).
        /// </summary>
        public gVector4<T> aabg => new gVector4<T>(w, w, z, y);
        
        /// <summary>
        /// Returns gVector4.wwzz swizzling.
        /// </summary>
        public gVector4<T> wwzz => new gVector4<T>(w, w, z, z);
        
        /// <summary>
        /// Returns gVector4.aabb swizzling (equivalent to gVector4.wwzz).
        /// </summary>
        public gVector4<T> aabb => new gVector4<T>(w, w, z, z);
        
        /// <summary>
        /// Returns gVector4.wwzw swizzling.
        /// </summary>
        public gVector4<T> wwzw => new gVector4<T>(w, w, z, w);
        
        /// <summary>
        /// Returns gVector4.aaba swizzling (equivalent to gVector4.wwzw).
        /// </summary>
        public gVector4<T> aaba => new gVector4<T>(w, w, z, w);
        
        /// <summary>
        /// Returns gVector4.www swizzling.
        /// </summary>
        public gVector3<T> www => new gVector3<T>(w, w, w);
        
        /// <summary>
        /// Returns gVector4.aaa swizzling (equivalent to gVector4.www).
        /// </summary>
        public gVector3<T> aaa => new gVector3<T>(w, w, w);
        
        /// <summary>
        /// Returns gVector4.wwwx swizzling.
        /// </summary>
        public gVector4<T> wwwx => new gVector4<T>(w, w, w, x);
        
        /// <summary>
        /// Returns gVector4.aaar swizzling (equivalent to gVector4.wwwx).
        /// </summary>
        public gVector4<T> aaar => new gVector4<T>(w, w, w, x);
        
        /// <summary>
        /// Returns gVector4.wwwy swizzling.
        /// </summary>
        public gVector4<T> wwwy => new gVector4<T>(w, w, w, y);
        
        /// <summary>
        /// Returns gVector4.aaag swizzling (equivalent to gVector4.wwwy).
        /// </summary>
        public gVector4<T> aaag => new gVector4<T>(w, w, w, y);
        
        /// <summary>
        /// Returns gVector4.wwwz swizzling.
        /// </summary>
        public gVector4<T> wwwz => new gVector4<T>(w, w, w, z);
        
        /// <summary>
        /// Returns gVector4.aaab swizzling (equivalent to gVector4.wwwz).
        /// </summary>
        public gVector4<T> aaab => new gVector4<T>(w, w, w, z);
        
        /// <summary>
        /// Returns gVector4.wwww swizzling.
        /// </summary>
        public gVector4<T> wwww => new gVector4<T>(w, w, w, w);
        
        /// <summary>
        /// Returns gVector4.aaaa swizzling (equivalent to gVector4.wwww).
        /// </summary>
        public gVector4<T> aaaa => new gVector4<T>(w, w, w, w);

        #endregion

    }
}
