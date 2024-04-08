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
    /// Temporary Vector of type float with 4 components, used for implementing swizzling for Vector4.
    /// </summary>
    [RegisterCoreClassStruct]
   
    [StructLayout(LayoutKind.Sequential)]
    public struct swizzle_Vector4
    {

        #region Fields
        
        /// <summary>
        /// x-component
        /// </summary>
        
        internal readonly float x;
        
        /// <summary>
        /// y-component
        /// </summary>
        
        internal readonly float y;
        
        /// <summary>
        /// z-component
        /// </summary>
        
        internal readonly float z;
        
        /// <summary>
        /// w-component
        /// </summary>
        
        internal readonly float w;

        #endregion


        #region Constructors
        
        /// <summary>
        /// Constructor for swizzle_Vector4.
        /// </summary>
        internal swizzle_Vector4(float x, float y, float z, float w)
        {
            this.x = x;
            this.y = y;
            this.z = z;
            this.w = w;
        }

        #endregion


        #region Properties
        
        /// <summary>
        /// Returns Vector4.xx swizzling.
        /// </summary>
        public Vector2 xx => new Vector2(x, x);
        
        /// <summary>
        /// Returns Vector4.rr swizzling (equivalent to Vector4.xx).
        /// </summary>
        public Vector2 rr => new Vector2(x, x);
        
        /// <summary>
        /// Returns Vector4.xxx swizzling.
        /// </summary>
        public Vector3 xxx => new Vector3(x, x, x);
        
        /// <summary>
        /// Returns Vector4.rrr swizzling (equivalent to Vector4.xxx).
        /// </summary>
        public Vector3 rrr => new Vector3(x, x, x);
        
        /// <summary>
        /// Returns Vector4.xxxx swizzling.
        /// </summary>
        public Vector4 xxxx => new Vector4(x, x, x, x);
        
        /// <summary>
        /// Returns Vector4.rrrr swizzling (equivalent to Vector4.xxxx).
        /// </summary>
        public Vector4 rrrr => new Vector4(x, x, x, x);
        
        /// <summary>
        /// Returns Vector4.xxxy swizzling.
        /// </summary>
        public Vector4 xxxy => new Vector4(x, x, x, y);
        
        /// <summary>
        /// Returns Vector4.rrrg swizzling (equivalent to Vector4.xxxy).
        /// </summary>
        public Vector4 rrrg => new Vector4(x, x, x, y);
        
        /// <summary>
        /// Returns Vector4.xxxz swizzling.
        /// </summary>
        public Vector4 xxxz => new Vector4(x, x, x, z);
        
        /// <summary>
        /// Returns Vector4.rrrb swizzling (equivalent to Vector4.xxxz).
        /// </summary>
        public Vector4 rrrb => new Vector4(x, x, x, z);
        
        /// <summary>
        /// Returns Vector4.xxxw swizzling.
        /// </summary>
        public Vector4 xxxw => new Vector4(x, x, x, w);
        
        /// <summary>
        /// Returns Vector4.rrra swizzling (equivalent to Vector4.xxxw).
        /// </summary>
        public Vector4 rrra => new Vector4(x, x, x, w);
        
        /// <summary>
        /// Returns Vector4.xxy swizzling.
        /// </summary>
        public Vector3 xxy => new Vector3(x, x, y);
        
        /// <summary>
        /// Returns Vector4.rrg swizzling (equivalent to Vector4.xxy).
        /// </summary>
        public Vector3 rrg => new Vector3(x, x, y);
        
        /// <summary>
        /// Returns Vector4.xxyx swizzling.
        /// </summary>
        public Vector4 xxyx => new Vector4(x, x, y, x);
        
        /// <summary>
        /// Returns Vector4.rrgr swizzling (equivalent to Vector4.xxyx).
        /// </summary>
        public Vector4 rrgr => new Vector4(x, x, y, x);
        
        /// <summary>
        /// Returns Vector4.xxyy swizzling.
        /// </summary>
        public Vector4 xxyy => new Vector4(x, x, y, y);
        
        /// <summary>
        /// Returns Vector4.rrgg swizzling (equivalent to Vector4.xxyy).
        /// </summary>
        public Vector4 rrgg => new Vector4(x, x, y, y);
        
        /// <summary>
        /// Returns Vector4.xxyz swizzling.
        /// </summary>
        public Vector4 xxyz => new Vector4(x, x, y, z);
        
        /// <summary>
        /// Returns Vector4.rrgb swizzling (equivalent to Vector4.xxyz).
        /// </summary>
        public Vector4 rrgb => new Vector4(x, x, y, z);
        
        /// <summary>
        /// Returns Vector4.xxyw swizzling.
        /// </summary>
        public Vector4 xxyw => new Vector4(x, x, y, w);
        
        /// <summary>
        /// Returns Vector4.rrga swizzling (equivalent to Vector4.xxyw).
        /// </summary>
        public Vector4 rrga => new Vector4(x, x, y, w);
        
        /// <summary>
        /// Returns Vector4.xxz swizzling.
        /// </summary>
        public Vector3 xxz => new Vector3(x, x, z);
        
        /// <summary>
        /// Returns Vector4.rrb swizzling (equivalent to Vector4.xxz).
        /// </summary>
        public Vector3 rrb => new Vector3(x, x, z);
        
        /// <summary>
        /// Returns Vector4.xxzx swizzling.
        /// </summary>
        public Vector4 xxzx => new Vector4(x, x, z, x);
        
        /// <summary>
        /// Returns Vector4.rrbr swizzling (equivalent to Vector4.xxzx).
        /// </summary>
        public Vector4 rrbr => new Vector4(x, x, z, x);
        
        /// <summary>
        /// Returns Vector4.xxzy swizzling.
        /// </summary>
        public Vector4 xxzy => new Vector4(x, x, z, y);
        
        /// <summary>
        /// Returns Vector4.rrbg swizzling (equivalent to Vector4.xxzy).
        /// </summary>
        public Vector4 rrbg => new Vector4(x, x, z, y);
        
        /// <summary>
        /// Returns Vector4.xxzz swizzling.
        /// </summary>
        public Vector4 xxzz => new Vector4(x, x, z, z);
        
        /// <summary>
        /// Returns Vector4.rrbb swizzling (equivalent to Vector4.xxzz).
        /// </summary>
        public Vector4 rrbb => new Vector4(x, x, z, z);
        
        /// <summary>
        /// Returns Vector4.xxzw swizzling.
        /// </summary>
        public Vector4 xxzw => new Vector4(x, x, z, w);
        
        /// <summary>
        /// Returns Vector4.rrba swizzling (equivalent to Vector4.xxzw).
        /// </summary>
        public Vector4 rrba => new Vector4(x, x, z, w);
        
        /// <summary>
        /// Returns Vector4.xxw swizzling.
        /// </summary>
        public Vector3 xxw => new Vector3(x, x, w);
        
        /// <summary>
        /// Returns Vector4.rra swizzling (equivalent to Vector4.xxw).
        /// </summary>
        public Vector3 rra => new Vector3(x, x, w);
        
        /// <summary>
        /// Returns Vector4.xxwx swizzling.
        /// </summary>
        public Vector4 xxwx => new Vector4(x, x, w, x);
        
        /// <summary>
        /// Returns Vector4.rrar swizzling (equivalent to Vector4.xxwx).
        /// </summary>
        public Vector4 rrar => new Vector4(x, x, w, x);
        
        /// <summary>
        /// Returns Vector4.xxwy swizzling.
        /// </summary>
        public Vector4 xxwy => new Vector4(x, x, w, y);
        
        /// <summary>
        /// Returns Vector4.rrag swizzling (equivalent to Vector4.xxwy).
        /// </summary>
        public Vector4 rrag => new Vector4(x, x, w, y);
        
        /// <summary>
        /// Returns Vector4.xxwz swizzling.
        /// </summary>
        public Vector4 xxwz => new Vector4(x, x, w, z);
        
        /// <summary>
        /// Returns Vector4.rrab swizzling (equivalent to Vector4.xxwz).
        /// </summary>
        public Vector4 rrab => new Vector4(x, x, w, z);
        
        /// <summary>
        /// Returns Vector4.xxww swizzling.
        /// </summary>
        public Vector4 xxww => new Vector4(x, x, w, w);
        
        /// <summary>
        /// Returns Vector4.rraa swizzling (equivalent to Vector4.xxww).
        /// </summary>
        public Vector4 rraa => new Vector4(x, x, w, w);
        
        /// <summary>
        /// Returns Vector4.xy swizzling.
        /// </summary>
        public Vector2 xy => new Vector2(x, y);
        
        /// <summary>
        /// Returns Vector4.rg swizzling (equivalent to Vector4.xy).
        /// </summary>
        public Vector2 rg => new Vector2(x, y);
        
        /// <summary>
        /// Returns Vector4.xyx swizzling.
        /// </summary>
        public Vector3 xyx => new Vector3(x, y, x);
        
        /// <summary>
        /// Returns Vector4.rgr swizzling (equivalent to Vector4.xyx).
        /// </summary>
        public Vector3 rgr => new Vector3(x, y, x);
        
        /// <summary>
        /// Returns Vector4.xyxx swizzling.
        /// </summary>
        public Vector4 xyxx => new Vector4(x, y, x, x);
        
        /// <summary>
        /// Returns Vector4.rgrr swizzling (equivalent to Vector4.xyxx).
        /// </summary>
        public Vector4 rgrr => new Vector4(x, y, x, x);
        
        /// <summary>
        /// Returns Vector4.xyxy swizzling.
        /// </summary>
        public Vector4 xyxy => new Vector4(x, y, x, y);
        
        /// <summary>
        /// Returns Vector4.rgrg swizzling (equivalent to Vector4.xyxy).
        /// </summary>
        public Vector4 rgrg => new Vector4(x, y, x, y);
        
        /// <summary>
        /// Returns Vector4.xyxz swizzling.
        /// </summary>
        public Vector4 xyxz => new Vector4(x, y, x, z);
        
        /// <summary>
        /// Returns Vector4.rgrb swizzling (equivalent to Vector4.xyxz).
        /// </summary>
        public Vector4 rgrb => new Vector4(x, y, x, z);
        
        /// <summary>
        /// Returns Vector4.xyxw swizzling.
        /// </summary>
        public Vector4 xyxw => new Vector4(x, y, x, w);
        
        /// <summary>
        /// Returns Vector4.rgra swizzling (equivalent to Vector4.xyxw).
        /// </summary>
        public Vector4 rgra => new Vector4(x, y, x, w);
        
        /// <summary>
        /// Returns Vector4.xyy swizzling.
        /// </summary>
        public Vector3 xyy => new Vector3(x, y, y);
        
        /// <summary>
        /// Returns Vector4.rgg swizzling (equivalent to Vector4.xyy).
        /// </summary>
        public Vector3 rgg => new Vector3(x, y, y);
        
        /// <summary>
        /// Returns Vector4.xyyx swizzling.
        /// </summary>
        public Vector4 xyyx => new Vector4(x, y, y, x);
        
        /// <summary>
        /// Returns Vector4.rggr swizzling (equivalent to Vector4.xyyx).
        /// </summary>
        public Vector4 rggr => new Vector4(x, y, y, x);
        
        /// <summary>
        /// Returns Vector4.xyyy swizzling.
        /// </summary>
        public Vector4 xyyy => new Vector4(x, y, y, y);
        
        /// <summary>
        /// Returns Vector4.rggg swizzling (equivalent to Vector4.xyyy).
        /// </summary>
        public Vector4 rggg => new Vector4(x, y, y, y);
        
        /// <summary>
        /// Returns Vector4.xyyz swizzling.
        /// </summary>
        public Vector4 xyyz => new Vector4(x, y, y, z);
        
        /// <summary>
        /// Returns Vector4.rggb swizzling (equivalent to Vector4.xyyz).
        /// </summary>
        public Vector4 rggb => new Vector4(x, y, y, z);
        
        /// <summary>
        /// Returns Vector4.xyyw swizzling.
        /// </summary>
        public Vector4 xyyw => new Vector4(x, y, y, w);
        
        /// <summary>
        /// Returns Vector4.rgga swizzling (equivalent to Vector4.xyyw).
        /// </summary>
        public Vector4 rgga => new Vector4(x, y, y, w);
        
        /// <summary>
        /// Returns Vector4.xyz swizzling.
        /// </summary>
        public Vector3 xyz => new Vector3(x, y, z);
        
        /// <summary>
        /// Returns Vector4.rgb swizzling (equivalent to Vector4.xyz).
        /// </summary>
        public Vector3 rgb => new Vector3(x, y, z);
        
        /// <summary>
        /// Returns Vector4.xyzx swizzling.
        /// </summary>
        public Vector4 xyzx => new Vector4(x, y, z, x);
        
        /// <summary>
        /// Returns Vector4.rgbr swizzling (equivalent to Vector4.xyzx).
        /// </summary>
        public Vector4 rgbr => new Vector4(x, y, z, x);
        
        /// <summary>
        /// Returns Vector4.xyzy swizzling.
        /// </summary>
        public Vector4 xyzy => new Vector4(x, y, z, y);
        
        /// <summary>
        /// Returns Vector4.rgbg swizzling (equivalent to Vector4.xyzy).
        /// </summary>
        public Vector4 rgbg => new Vector4(x, y, z, y);
        
        /// <summary>
        /// Returns Vector4.xyzz swizzling.
        /// </summary>
        public Vector4 xyzz => new Vector4(x, y, z, z);
        
        /// <summary>
        /// Returns Vector4.rgbb swizzling (equivalent to Vector4.xyzz).
        /// </summary>
        public Vector4 rgbb => new Vector4(x, y, z, z);
        
        /// <summary>
        /// Returns Vector4.xyzw swizzling.
        /// </summary>
        public Vector4 xyzw => new Vector4(x, y, z, w);
        
        /// <summary>
        /// Returns Vector4.rgba swizzling (equivalent to Vector4.xyzw).
        /// </summary>
        public Vector4 rgba => new Vector4(x, y, z, w);
        
        /// <summary>
        /// Returns Vector4.xyw swizzling.
        /// </summary>
        public Vector3 xyw => new Vector3(x, y, w);
        
        /// <summary>
        /// Returns Vector4.rga swizzling (equivalent to Vector4.xyw).
        /// </summary>
        public Vector3 rga => new Vector3(x, y, w);
        
        /// <summary>
        /// Returns Vector4.xywx swizzling.
        /// </summary>
        public Vector4 xywx => new Vector4(x, y, w, x);
        
        /// <summary>
        /// Returns Vector4.rgar swizzling (equivalent to Vector4.xywx).
        /// </summary>
        public Vector4 rgar => new Vector4(x, y, w, x);
        
        /// <summary>
        /// Returns Vector4.xywy swizzling.
        /// </summary>
        public Vector4 xywy => new Vector4(x, y, w, y);
        
        /// <summary>
        /// Returns Vector4.rgag swizzling (equivalent to Vector4.xywy).
        /// </summary>
        public Vector4 rgag => new Vector4(x, y, w, y);
        
        /// <summary>
        /// Returns Vector4.xywz swizzling.
        /// </summary>
        public Vector4 xywz => new Vector4(x, y, w, z);
        
        /// <summary>
        /// Returns Vector4.rgab swizzling (equivalent to Vector4.xywz).
        /// </summary>
        public Vector4 rgab => new Vector4(x, y, w, z);
        
        /// <summary>
        /// Returns Vector4.xyww swizzling.
        /// </summary>
        public Vector4 xyww => new Vector4(x, y, w, w);
        
        /// <summary>
        /// Returns Vector4.rgaa swizzling (equivalent to Vector4.xyww).
        /// </summary>
        public Vector4 rgaa => new Vector4(x, y, w, w);
        
        /// <summary>
        /// Returns Vector4.xz swizzling.
        /// </summary>
        public Vector2 xz => new Vector2(x, z);
        
        /// <summary>
        /// Returns Vector4.rb swizzling (equivalent to Vector4.xz).
        /// </summary>
        public Vector2 rb => new Vector2(x, z);
        
        /// <summary>
        /// Returns Vector4.xzx swizzling.
        /// </summary>
        public Vector3 xzx => new Vector3(x, z, x);
        
        /// <summary>
        /// Returns Vector4.rbr swizzling (equivalent to Vector4.xzx).
        /// </summary>
        public Vector3 rbr => new Vector3(x, z, x);
        
        /// <summary>
        /// Returns Vector4.xzxx swizzling.
        /// </summary>
        public Vector4 xzxx => new Vector4(x, z, x, x);
        
        /// <summary>
        /// Returns Vector4.rbrr swizzling (equivalent to Vector4.xzxx).
        /// </summary>
        public Vector4 rbrr => new Vector4(x, z, x, x);
        
        /// <summary>
        /// Returns Vector4.xzxy swizzling.
        /// </summary>
        public Vector4 xzxy => new Vector4(x, z, x, y);
        
        /// <summary>
        /// Returns Vector4.rbrg swizzling (equivalent to Vector4.xzxy).
        /// </summary>
        public Vector4 rbrg => new Vector4(x, z, x, y);
        
        /// <summary>
        /// Returns Vector4.xzxz swizzling.
        /// </summary>
        public Vector4 xzxz => new Vector4(x, z, x, z);
        
        /// <summary>
        /// Returns Vector4.rbrb swizzling (equivalent to Vector4.xzxz).
        /// </summary>
        public Vector4 rbrb => new Vector4(x, z, x, z);
        
        /// <summary>
        /// Returns Vector4.xzxw swizzling.
        /// </summary>
        public Vector4 xzxw => new Vector4(x, z, x, w);
        
        /// <summary>
        /// Returns Vector4.rbra swizzling (equivalent to Vector4.xzxw).
        /// </summary>
        public Vector4 rbra => new Vector4(x, z, x, w);
        
        /// <summary>
        /// Returns Vector4.xzy swizzling.
        /// </summary>
        public Vector3 xzy => new Vector3(x, z, y);
        
        /// <summary>
        /// Returns Vector4.rbg swizzling (equivalent to Vector4.xzy).
        /// </summary>
        public Vector3 rbg => new Vector3(x, z, y);
        
        /// <summary>
        /// Returns Vector4.xzyx swizzling.
        /// </summary>
        public Vector4 xzyx => new Vector4(x, z, y, x);
        
        /// <summary>
        /// Returns Vector4.rbgr swizzling (equivalent to Vector4.xzyx).
        /// </summary>
        public Vector4 rbgr => new Vector4(x, z, y, x);
        
        /// <summary>
        /// Returns Vector4.xzyy swizzling.
        /// </summary>
        public Vector4 xzyy => new Vector4(x, z, y, y);
        
        /// <summary>
        /// Returns Vector4.rbgg swizzling (equivalent to Vector4.xzyy).
        /// </summary>
        public Vector4 rbgg => new Vector4(x, z, y, y);
        
        /// <summary>
        /// Returns Vector4.xzyz swizzling.
        /// </summary>
        public Vector4 xzyz => new Vector4(x, z, y, z);
        
        /// <summary>
        /// Returns Vector4.rbgb swizzling (equivalent to Vector4.xzyz).
        /// </summary>
        public Vector4 rbgb => new Vector4(x, z, y, z);
        
        /// <summary>
        /// Returns Vector4.xzyw swizzling.
        /// </summary>
        public Vector4 xzyw => new Vector4(x, z, y, w);
        
        /// <summary>
        /// Returns Vector4.rbga swizzling (equivalent to Vector4.xzyw).
        /// </summary>
        public Vector4 rbga => new Vector4(x, z, y, w);
        
        /// <summary>
        /// Returns Vector4.xzz swizzling.
        /// </summary>
        public Vector3 xzz => new Vector3(x, z, z);
        
        /// <summary>
        /// Returns Vector4.rbb swizzling (equivalent to Vector4.xzz).
        /// </summary>
        public Vector3 rbb => new Vector3(x, z, z);
        
        /// <summary>
        /// Returns Vector4.xzzx swizzling.
        /// </summary>
        public Vector4 xzzx => new Vector4(x, z, z, x);
        
        /// <summary>
        /// Returns Vector4.rbbr swizzling (equivalent to Vector4.xzzx).
        /// </summary>
        public Vector4 rbbr => new Vector4(x, z, z, x);
        
        /// <summary>
        /// Returns Vector4.xzzy swizzling.
        /// </summary>
        public Vector4 xzzy => new Vector4(x, z, z, y);
        
        /// <summary>
        /// Returns Vector4.rbbg swizzling (equivalent to Vector4.xzzy).
        /// </summary>
        public Vector4 rbbg => new Vector4(x, z, z, y);
        
        /// <summary>
        /// Returns Vector4.xzzz swizzling.
        /// </summary>
        public Vector4 xzzz => new Vector4(x, z, z, z);
        
        /// <summary>
        /// Returns Vector4.rbbb swizzling (equivalent to Vector4.xzzz).
        /// </summary>
        public Vector4 rbbb => new Vector4(x, z, z, z);
        
        /// <summary>
        /// Returns Vector4.xzzw swizzling.
        /// </summary>
        public Vector4 xzzw => new Vector4(x, z, z, w);
        
        /// <summary>
        /// Returns Vector4.rbba swizzling (equivalent to Vector4.xzzw).
        /// </summary>
        public Vector4 rbba => new Vector4(x, z, z, w);
        
        /// <summary>
        /// Returns Vector4.xzw swizzling.
        /// </summary>
        public Vector3 xzw => new Vector3(x, z, w);
        
        /// <summary>
        /// Returns Vector4.rba swizzling (equivalent to Vector4.xzw).
        /// </summary>
        public Vector3 rba => new Vector3(x, z, w);
        
        /// <summary>
        /// Returns Vector4.xzwx swizzling.
        /// </summary>
        public Vector4 xzwx => new Vector4(x, z, w, x);
        
        /// <summary>
        /// Returns Vector4.rbar swizzling (equivalent to Vector4.xzwx).
        /// </summary>
        public Vector4 rbar => new Vector4(x, z, w, x);
        
        /// <summary>
        /// Returns Vector4.xzwy swizzling.
        /// </summary>
        public Vector4 xzwy => new Vector4(x, z, w, y);
        
        /// <summary>
        /// Returns Vector4.rbag swizzling (equivalent to Vector4.xzwy).
        /// </summary>
        public Vector4 rbag => new Vector4(x, z, w, y);
        
        /// <summary>
        /// Returns Vector4.xzwz swizzling.
        /// </summary>
        public Vector4 xzwz => new Vector4(x, z, w, z);
        
        /// <summary>
        /// Returns Vector4.rbab swizzling (equivalent to Vector4.xzwz).
        /// </summary>
        public Vector4 rbab => new Vector4(x, z, w, z);
        
        /// <summary>
        /// Returns Vector4.xzww swizzling.
        /// </summary>
        public Vector4 xzww => new Vector4(x, z, w, w);
        
        /// <summary>
        /// Returns Vector4.rbaa swizzling (equivalent to Vector4.xzww).
        /// </summary>
        public Vector4 rbaa => new Vector4(x, z, w, w);
        
        /// <summary>
        /// Returns Vector4.xw swizzling.
        /// </summary>
        public Vector2 xw => new Vector2(x, w);
        
        /// <summary>
        /// Returns Vector4.ra swizzling (equivalent to Vector4.xw).
        /// </summary>
        public Vector2 ra => new Vector2(x, w);
        
        /// <summary>
        /// Returns Vector4.xwx swizzling.
        /// </summary>
        public Vector3 xwx => new Vector3(x, w, x);
        
        /// <summary>
        /// Returns Vector4.rar swizzling (equivalent to Vector4.xwx).
        /// </summary>
        public Vector3 rar => new Vector3(x, w, x);
        
        /// <summary>
        /// Returns Vector4.xwxx swizzling.
        /// </summary>
        public Vector4 xwxx => new Vector4(x, w, x, x);
        
        /// <summary>
        /// Returns Vector4.rarr swizzling (equivalent to Vector4.xwxx).
        /// </summary>
        public Vector4 rarr => new Vector4(x, w, x, x);
        
        /// <summary>
        /// Returns Vector4.xwxy swizzling.
        /// </summary>
        public Vector4 xwxy => new Vector4(x, w, x, y);
        
        /// <summary>
        /// Returns Vector4.rarg swizzling (equivalent to Vector4.xwxy).
        /// </summary>
        public Vector4 rarg => new Vector4(x, w, x, y);
        
        /// <summary>
        /// Returns Vector4.xwxz swizzling.
        /// </summary>
        public Vector4 xwxz => new Vector4(x, w, x, z);
        
        /// <summary>
        /// Returns Vector4.rarb swizzling (equivalent to Vector4.xwxz).
        /// </summary>
        public Vector4 rarb => new Vector4(x, w, x, z);
        
        /// <summary>
        /// Returns Vector4.xwxw swizzling.
        /// </summary>
        public Vector4 xwxw => new Vector4(x, w, x, w);
        
        /// <summary>
        /// Returns Vector4.rara swizzling (equivalent to Vector4.xwxw).
        /// </summary>
        public Vector4 rara => new Vector4(x, w, x, w);
        
        /// <summary>
        /// Returns Vector4.xwy swizzling.
        /// </summary>
        public Vector3 xwy => new Vector3(x, w, y);
        
        /// <summary>
        /// Returns Vector4.rag swizzling (equivalent to Vector4.xwy).
        /// </summary>
        public Vector3 rag => new Vector3(x, w, y);
        
        /// <summary>
        /// Returns Vector4.xwyx swizzling.
        /// </summary>
        public Vector4 xwyx => new Vector4(x, w, y, x);
        
        /// <summary>
        /// Returns Vector4.ragr swizzling (equivalent to Vector4.xwyx).
        /// </summary>
        public Vector4 ragr => new Vector4(x, w, y, x);
        
        /// <summary>
        /// Returns Vector4.xwyy swizzling.
        /// </summary>
        public Vector4 xwyy => new Vector4(x, w, y, y);
        
        /// <summary>
        /// Returns Vector4.ragg swizzling (equivalent to Vector4.xwyy).
        /// </summary>
        public Vector4 ragg => new Vector4(x, w, y, y);
        
        /// <summary>
        /// Returns Vector4.xwyz swizzling.
        /// </summary>
        public Vector4 xwyz => new Vector4(x, w, y, z);
        
        /// <summary>
        /// Returns Vector4.ragb swizzling (equivalent to Vector4.xwyz).
        /// </summary>
        public Vector4 ragb => new Vector4(x, w, y, z);
        
        /// <summary>
        /// Returns Vector4.xwyw swizzling.
        /// </summary>
        public Vector4 xwyw => new Vector4(x, w, y, w);
        
        /// <summary>
        /// Returns Vector4.raga swizzling (equivalent to Vector4.xwyw).
        /// </summary>
        public Vector4 raga => new Vector4(x, w, y, w);
        
        /// <summary>
        /// Returns Vector4.xwz swizzling.
        /// </summary>
        public Vector3 xwz => new Vector3(x, w, z);
        
        /// <summary>
        /// Returns Vector4.rab swizzling (equivalent to Vector4.xwz).
        /// </summary>
        public Vector3 rab => new Vector3(x, w, z);
        
        /// <summary>
        /// Returns Vector4.xwzx swizzling.
        /// </summary>
        public Vector4 xwzx => new Vector4(x, w, z, x);
        
        /// <summary>
        /// Returns Vector4.rabr swizzling (equivalent to Vector4.xwzx).
        /// </summary>
        public Vector4 rabr => new Vector4(x, w, z, x);
        
        /// <summary>
        /// Returns Vector4.xwzy swizzling.
        /// </summary>
        public Vector4 xwzy => new Vector4(x, w, z, y);
        
        /// <summary>
        /// Returns Vector4.rabg swizzling (equivalent to Vector4.xwzy).
        /// </summary>
        public Vector4 rabg => new Vector4(x, w, z, y);
        
        /// <summary>
        /// Returns Vector4.xwzz swizzling.
        /// </summary>
        public Vector4 xwzz => new Vector4(x, w, z, z);
        
        /// <summary>
        /// Returns Vector4.rabb swizzling (equivalent to Vector4.xwzz).
        /// </summary>
        public Vector4 rabb => new Vector4(x, w, z, z);
        
        /// <summary>
        /// Returns Vector4.xwzw swizzling.
        /// </summary>
        public Vector4 xwzw => new Vector4(x, w, z, w);
        
        /// <summary>
        /// Returns Vector4.raba swizzling (equivalent to Vector4.xwzw).
        /// </summary>
        public Vector4 raba => new Vector4(x, w, z, w);
        
        /// <summary>
        /// Returns Vector4.xww swizzling.
        /// </summary>
        public Vector3 xww => new Vector3(x, w, w);
        
        /// <summary>
        /// Returns Vector4.raa swizzling (equivalent to Vector4.xww).
        /// </summary>
        public Vector3 raa => new Vector3(x, w, w);
        
        /// <summary>
        /// Returns Vector4.xwwx swizzling.
        /// </summary>
        public Vector4 xwwx => new Vector4(x, w, w, x);
        
        /// <summary>
        /// Returns Vector4.raar swizzling (equivalent to Vector4.xwwx).
        /// </summary>
        public Vector4 raar => new Vector4(x, w, w, x);
        
        /// <summary>
        /// Returns Vector4.xwwy swizzling.
        /// </summary>
        public Vector4 xwwy => new Vector4(x, w, w, y);
        
        /// <summary>
        /// Returns Vector4.raag swizzling (equivalent to Vector4.xwwy).
        /// </summary>
        public Vector4 raag => new Vector4(x, w, w, y);
        
        /// <summary>
        /// Returns Vector4.xwwz swizzling.
        /// </summary>
        public Vector4 xwwz => new Vector4(x, w, w, z);
        
        /// <summary>
        /// Returns Vector4.raab swizzling (equivalent to Vector4.xwwz).
        /// </summary>
        public Vector4 raab => new Vector4(x, w, w, z);
        
        /// <summary>
        /// Returns Vector4.xwww swizzling.
        /// </summary>
        public Vector4 xwww => new Vector4(x, w, w, w);
        
        /// <summary>
        /// Returns Vector4.raaa swizzling (equivalent to Vector4.xwww).
        /// </summary>
        public Vector4 raaa => new Vector4(x, w, w, w);
        
        /// <summary>
        /// Returns Vector4.yx swizzling.
        /// </summary>
        public Vector2 yx => new Vector2(y, x);
        
        /// <summary>
        /// Returns Vector4.gr swizzling (equivalent to Vector4.yx).
        /// </summary>
        public Vector2 gr => new Vector2(y, x);
        
        /// <summary>
        /// Returns Vector4.yxx swizzling.
        /// </summary>
        public Vector3 yxx => new Vector3(y, x, x);
        
        /// <summary>
        /// Returns Vector4.grr swizzling (equivalent to Vector4.yxx).
        /// </summary>
        public Vector3 grr => new Vector3(y, x, x);
        
        /// <summary>
        /// Returns Vector4.yxxx swizzling.
        /// </summary>
        public Vector4 yxxx => new Vector4(y, x, x, x);
        
        /// <summary>
        /// Returns Vector4.grrr swizzling (equivalent to Vector4.yxxx).
        /// </summary>
        public Vector4 grrr => new Vector4(y, x, x, x);
        
        /// <summary>
        /// Returns Vector4.yxxy swizzling.
        /// </summary>
        public Vector4 yxxy => new Vector4(y, x, x, y);
        
        /// <summary>
        /// Returns Vector4.grrg swizzling (equivalent to Vector4.yxxy).
        /// </summary>
        public Vector4 grrg => new Vector4(y, x, x, y);
        
        /// <summary>
        /// Returns Vector4.yxxz swizzling.
        /// </summary>
        public Vector4 yxxz => new Vector4(y, x, x, z);
        
        /// <summary>
        /// Returns Vector4.grrb swizzling (equivalent to Vector4.yxxz).
        /// </summary>
        public Vector4 grrb => new Vector4(y, x, x, z);
        
        /// <summary>
        /// Returns Vector4.yxxw swizzling.
        /// </summary>
        public Vector4 yxxw => new Vector4(y, x, x, w);
        
        /// <summary>
        /// Returns Vector4.grra swizzling (equivalent to Vector4.yxxw).
        /// </summary>
        public Vector4 grra => new Vector4(y, x, x, w);
        
        /// <summary>
        /// Returns Vector4.yxy swizzling.
        /// </summary>
        public Vector3 yxy => new Vector3(y, x, y);
        
        /// <summary>
        /// Returns Vector4.grg swizzling (equivalent to Vector4.yxy).
        /// </summary>
        public Vector3 grg => new Vector3(y, x, y);
        
        /// <summary>
        /// Returns Vector4.yxyx swizzling.
        /// </summary>
        public Vector4 yxyx => new Vector4(y, x, y, x);
        
        /// <summary>
        /// Returns Vector4.grgr swizzling (equivalent to Vector4.yxyx).
        /// </summary>
        public Vector4 grgr => new Vector4(y, x, y, x);
        
        /// <summary>
        /// Returns Vector4.yxyy swizzling.
        /// </summary>
        public Vector4 yxyy => new Vector4(y, x, y, y);
        
        /// <summary>
        /// Returns Vector4.grgg swizzling (equivalent to Vector4.yxyy).
        /// </summary>
        public Vector4 grgg => new Vector4(y, x, y, y);
        
        /// <summary>
        /// Returns Vector4.yxyz swizzling.
        /// </summary>
        public Vector4 yxyz => new Vector4(y, x, y, z);
        
        /// <summary>
        /// Returns Vector4.grgb swizzling (equivalent to Vector4.yxyz).
        /// </summary>
        public Vector4 grgb => new Vector4(y, x, y, z);
        
        /// <summary>
        /// Returns Vector4.yxyw swizzling.
        /// </summary>
        public Vector4 yxyw => new Vector4(y, x, y, w);
        
        /// <summary>
        /// Returns Vector4.grga swizzling (equivalent to Vector4.yxyw).
        /// </summary>
        public Vector4 grga => new Vector4(y, x, y, w);
        
        /// <summary>
        /// Returns Vector4.yxz swizzling.
        /// </summary>
        public Vector3 yxz => new Vector3(y, x, z);
        
        /// <summary>
        /// Returns Vector4.grb swizzling (equivalent to Vector4.yxz).
        /// </summary>
        public Vector3 grb => new Vector3(y, x, z);
        
        /// <summary>
        /// Returns Vector4.yxzx swizzling.
        /// </summary>
        public Vector4 yxzx => new Vector4(y, x, z, x);
        
        /// <summary>
        /// Returns Vector4.grbr swizzling (equivalent to Vector4.yxzx).
        /// </summary>
        public Vector4 grbr => new Vector4(y, x, z, x);
        
        /// <summary>
        /// Returns Vector4.yxzy swizzling.
        /// </summary>
        public Vector4 yxzy => new Vector4(y, x, z, y);
        
        /// <summary>
        /// Returns Vector4.grbg swizzling (equivalent to Vector4.yxzy).
        /// </summary>
        public Vector4 grbg => new Vector4(y, x, z, y);
        
        /// <summary>
        /// Returns Vector4.yxzz swizzling.
        /// </summary>
        public Vector4 yxzz => new Vector4(y, x, z, z);
        
        /// <summary>
        /// Returns Vector4.grbb swizzling (equivalent to Vector4.yxzz).
        /// </summary>
        public Vector4 grbb => new Vector4(y, x, z, z);
        
        /// <summary>
        /// Returns Vector4.yxzw swizzling.
        /// </summary>
        public Vector4 yxzw => new Vector4(y, x, z, w);
        
        /// <summary>
        /// Returns Vector4.grba swizzling (equivalent to Vector4.yxzw).
        /// </summary>
        public Vector4 grba => new Vector4(y, x, z, w);
        
        /// <summary>
        /// Returns Vector4.yxw swizzling.
        /// </summary>
        public Vector3 yxw => new Vector3(y, x, w);
        
        /// <summary>
        /// Returns Vector4.gra swizzling (equivalent to Vector4.yxw).
        /// </summary>
        public Vector3 gra => new Vector3(y, x, w);
        
        /// <summary>
        /// Returns Vector4.yxwx swizzling.
        /// </summary>
        public Vector4 yxwx => new Vector4(y, x, w, x);
        
        /// <summary>
        /// Returns Vector4.grar swizzling (equivalent to Vector4.yxwx).
        /// </summary>
        public Vector4 grar => new Vector4(y, x, w, x);
        
        /// <summary>
        /// Returns Vector4.yxwy swizzling.
        /// </summary>
        public Vector4 yxwy => new Vector4(y, x, w, y);
        
        /// <summary>
        /// Returns Vector4.grag swizzling (equivalent to Vector4.yxwy).
        /// </summary>
        public Vector4 grag => new Vector4(y, x, w, y);
        
        /// <summary>
        /// Returns Vector4.yxwz swizzling.
        /// </summary>
        public Vector4 yxwz => new Vector4(y, x, w, z);
        
        /// <summary>
        /// Returns Vector4.grab swizzling (equivalent to Vector4.yxwz).
        /// </summary>
        public Vector4 grab => new Vector4(y, x, w, z);
        
        /// <summary>
        /// Returns Vector4.yxww swizzling.
        /// </summary>
        public Vector4 yxww => new Vector4(y, x, w, w);
        
        /// <summary>
        /// Returns Vector4.graa swizzling (equivalent to Vector4.yxww).
        /// </summary>
        public Vector4 graa => new Vector4(y, x, w, w);
        
        /// <summary>
        /// Returns Vector4.yy swizzling.
        /// </summary>
        public Vector2 yy => new Vector2(y, y);
        
        /// <summary>
        /// Returns Vector4.gg swizzling (equivalent to Vector4.yy).
        /// </summary>
        public Vector2 gg => new Vector2(y, y);
        
        /// <summary>
        /// Returns Vector4.yyx swizzling.
        /// </summary>
        public Vector3 yyx => new Vector3(y, y, x);
        
        /// <summary>
        /// Returns Vector4.ggr swizzling (equivalent to Vector4.yyx).
        /// </summary>
        public Vector3 ggr => new Vector3(y, y, x);
        
        /// <summary>
        /// Returns Vector4.yyxx swizzling.
        /// </summary>
        public Vector4 yyxx => new Vector4(y, y, x, x);
        
        /// <summary>
        /// Returns Vector4.ggrr swizzling (equivalent to Vector4.yyxx).
        /// </summary>
        public Vector4 ggrr => new Vector4(y, y, x, x);
        
        /// <summary>
        /// Returns Vector4.yyxy swizzling.
        /// </summary>
        public Vector4 yyxy => new Vector4(y, y, x, y);
        
        /// <summary>
        /// Returns Vector4.ggrg swizzling (equivalent to Vector4.yyxy).
        /// </summary>
        public Vector4 ggrg => new Vector4(y, y, x, y);
        
        /// <summary>
        /// Returns Vector4.yyxz swizzling.
        /// </summary>
        public Vector4 yyxz => new Vector4(y, y, x, z);
        
        /// <summary>
        /// Returns Vector4.ggrb swizzling (equivalent to Vector4.yyxz).
        /// </summary>
        public Vector4 ggrb => new Vector4(y, y, x, z);
        
        /// <summary>
        /// Returns Vector4.yyxw swizzling.
        /// </summary>
        public Vector4 yyxw => new Vector4(y, y, x, w);
        
        /// <summary>
        /// Returns Vector4.ggra swizzling (equivalent to Vector4.yyxw).
        /// </summary>
        public Vector4 ggra => new Vector4(y, y, x, w);
        
        /// <summary>
        /// Returns Vector4.yyy swizzling.
        /// </summary>
        public Vector3 yyy => new Vector3(y, y, y);
        
        /// <summary>
        /// Returns Vector4.ggg swizzling (equivalent to Vector4.yyy).
        /// </summary>
        public Vector3 ggg => new Vector3(y, y, y);
        
        /// <summary>
        /// Returns Vector4.yyyx swizzling.
        /// </summary>
        public Vector4 yyyx => new Vector4(y, y, y, x);
        
        /// <summary>
        /// Returns Vector4.gggr swizzling (equivalent to Vector4.yyyx).
        /// </summary>
        public Vector4 gggr => new Vector4(y, y, y, x);
        
        /// <summary>
        /// Returns Vector4.yyyy swizzling.
        /// </summary>
        public Vector4 yyyy => new Vector4(y, y, y, y);
        
        /// <summary>
        /// Returns Vector4.gggg swizzling (equivalent to Vector4.yyyy).
        /// </summary>
        public Vector4 gggg => new Vector4(y, y, y, y);
        
        /// <summary>
        /// Returns Vector4.yyyz swizzling.
        /// </summary>
        public Vector4 yyyz => new Vector4(y, y, y, z);
        
        /// <summary>
        /// Returns Vector4.gggb swizzling (equivalent to Vector4.yyyz).
        /// </summary>
        public Vector4 gggb => new Vector4(y, y, y, z);
        
        /// <summary>
        /// Returns Vector4.yyyw swizzling.
        /// </summary>
        public Vector4 yyyw => new Vector4(y, y, y, w);
        
        /// <summary>
        /// Returns Vector4.ggga swizzling (equivalent to Vector4.yyyw).
        /// </summary>
        public Vector4 ggga => new Vector4(y, y, y, w);
        
        /// <summary>
        /// Returns Vector4.yyz swizzling.
        /// </summary>
        public Vector3 yyz => new Vector3(y, y, z);
        
        /// <summary>
        /// Returns Vector4.ggb swizzling (equivalent to Vector4.yyz).
        /// </summary>
        public Vector3 ggb => new Vector3(y, y, z);
        
        /// <summary>
        /// Returns Vector4.yyzx swizzling.
        /// </summary>
        public Vector4 yyzx => new Vector4(y, y, z, x);
        
        /// <summary>
        /// Returns Vector4.ggbr swizzling (equivalent to Vector4.yyzx).
        /// </summary>
        public Vector4 ggbr => new Vector4(y, y, z, x);
        
        /// <summary>
        /// Returns Vector4.yyzy swizzling.
        /// </summary>
        public Vector4 yyzy => new Vector4(y, y, z, y);
        
        /// <summary>
        /// Returns Vector4.ggbg swizzling (equivalent to Vector4.yyzy).
        /// </summary>
        public Vector4 ggbg => new Vector4(y, y, z, y);
        
        /// <summary>
        /// Returns Vector4.yyzz swizzling.
        /// </summary>
        public Vector4 yyzz => new Vector4(y, y, z, z);
        
        /// <summary>
        /// Returns Vector4.ggbb swizzling (equivalent to Vector4.yyzz).
        /// </summary>
        public Vector4 ggbb => new Vector4(y, y, z, z);
        
        /// <summary>
        /// Returns Vector4.yyzw swizzling.
        /// </summary>
        public Vector4 yyzw => new Vector4(y, y, z, w);
        
        /// <summary>
        /// Returns Vector4.ggba swizzling (equivalent to Vector4.yyzw).
        /// </summary>
        public Vector4 ggba => new Vector4(y, y, z, w);
        
        /// <summary>
        /// Returns Vector4.yyw swizzling.
        /// </summary>
        public Vector3 yyw => new Vector3(y, y, w);
        
        /// <summary>
        /// Returns Vector4.gga swizzling (equivalent to Vector4.yyw).
        /// </summary>
        public Vector3 gga => new Vector3(y, y, w);
        
        /// <summary>
        /// Returns Vector4.yywx swizzling.
        /// </summary>
        public Vector4 yywx => new Vector4(y, y, w, x);
        
        /// <summary>
        /// Returns Vector4.ggar swizzling (equivalent to Vector4.yywx).
        /// </summary>
        public Vector4 ggar => new Vector4(y, y, w, x);
        
        /// <summary>
        /// Returns Vector4.yywy swizzling.
        /// </summary>
        public Vector4 yywy => new Vector4(y, y, w, y);
        
        /// <summary>
        /// Returns Vector4.ggag swizzling (equivalent to Vector4.yywy).
        /// </summary>
        public Vector4 ggag => new Vector4(y, y, w, y);
        
        /// <summary>
        /// Returns Vector4.yywz swizzling.
        /// </summary>
        public Vector4 yywz => new Vector4(y, y, w, z);
        
        /// <summary>
        /// Returns Vector4.ggab swizzling (equivalent to Vector4.yywz).
        /// </summary>
        public Vector4 ggab => new Vector4(y, y, w, z);
        
        /// <summary>
        /// Returns Vector4.yyww swizzling.
        /// </summary>
        public Vector4 yyww => new Vector4(y, y, w, w);
        
        /// <summary>
        /// Returns Vector4.ggaa swizzling (equivalent to Vector4.yyww).
        /// </summary>
        public Vector4 ggaa => new Vector4(y, y, w, w);
        
        /// <summary>
        /// Returns Vector4.yz swizzling.
        /// </summary>
        public Vector2 yz => new Vector2(y, z);
        
        /// <summary>
        /// Returns Vector4.gb swizzling (equivalent to Vector4.yz).
        /// </summary>
        public Vector2 gb => new Vector2(y, z);
        
        /// <summary>
        /// Returns Vector4.yzx swizzling.
        /// </summary>
        public Vector3 yzx => new Vector3(y, z, x);
        
        /// <summary>
        /// Returns Vector4.gbr swizzling (equivalent to Vector4.yzx).
        /// </summary>
        public Vector3 gbr => new Vector3(y, z, x);
        
        /// <summary>
        /// Returns Vector4.yzxx swizzling.
        /// </summary>
        public Vector4 yzxx => new Vector4(y, z, x, x);
        
        /// <summary>
        /// Returns Vector4.gbrr swizzling (equivalent to Vector4.yzxx).
        /// </summary>
        public Vector4 gbrr => new Vector4(y, z, x, x);
        
        /// <summary>
        /// Returns Vector4.yzxy swizzling.
        /// </summary>
        public Vector4 yzxy => new Vector4(y, z, x, y);
        
        /// <summary>
        /// Returns Vector4.gbrg swizzling (equivalent to Vector4.yzxy).
        /// </summary>
        public Vector4 gbrg => new Vector4(y, z, x, y);
        
        /// <summary>
        /// Returns Vector4.yzxz swizzling.
        /// </summary>
        public Vector4 yzxz => new Vector4(y, z, x, z);
        
        /// <summary>
        /// Returns Vector4.gbrb swizzling (equivalent to Vector4.yzxz).
        /// </summary>
        public Vector4 gbrb => new Vector4(y, z, x, z);
        
        /// <summary>
        /// Returns Vector4.yzxw swizzling.
        /// </summary>
        public Vector4 yzxw => new Vector4(y, z, x, w);
        
        /// <summary>
        /// Returns Vector4.gbra swizzling (equivalent to Vector4.yzxw).
        /// </summary>
        public Vector4 gbra => new Vector4(y, z, x, w);
        
        /// <summary>
        /// Returns Vector4.yzy swizzling.
        /// </summary>
        public Vector3 yzy => new Vector3(y, z, y);
        
        /// <summary>
        /// Returns Vector4.gbg swizzling (equivalent to Vector4.yzy).
        /// </summary>
        public Vector3 gbg => new Vector3(y, z, y);
        
        /// <summary>
        /// Returns Vector4.yzyx swizzling.
        /// </summary>
        public Vector4 yzyx => new Vector4(y, z, y, x);
        
        /// <summary>
        /// Returns Vector4.gbgr swizzling (equivalent to Vector4.yzyx).
        /// </summary>
        public Vector4 gbgr => new Vector4(y, z, y, x);
        
        /// <summary>
        /// Returns Vector4.yzyy swizzling.
        /// </summary>
        public Vector4 yzyy => new Vector4(y, z, y, y);
        
        /// <summary>
        /// Returns Vector4.gbgg swizzling (equivalent to Vector4.yzyy).
        /// </summary>
        public Vector4 gbgg => new Vector4(y, z, y, y);
        
        /// <summary>
        /// Returns Vector4.yzyz swizzling.
        /// </summary>
        public Vector4 yzyz => new Vector4(y, z, y, z);
        
        /// <summary>
        /// Returns Vector4.gbgb swizzling (equivalent to Vector4.yzyz).
        /// </summary>
        public Vector4 gbgb => new Vector4(y, z, y, z);
        
        /// <summary>
        /// Returns Vector4.yzyw swizzling.
        /// </summary>
        public Vector4 yzyw => new Vector4(y, z, y, w);
        
        /// <summary>
        /// Returns Vector4.gbga swizzling (equivalent to Vector4.yzyw).
        /// </summary>
        public Vector4 gbga => new Vector4(y, z, y, w);
        
        /// <summary>
        /// Returns Vector4.yzz swizzling.
        /// </summary>
        public Vector3 yzz => new Vector3(y, z, z);
        
        /// <summary>
        /// Returns Vector4.gbb swizzling (equivalent to Vector4.yzz).
        /// </summary>
        public Vector3 gbb => new Vector3(y, z, z);
        
        /// <summary>
        /// Returns Vector4.yzzx swizzling.
        /// </summary>
        public Vector4 yzzx => new Vector4(y, z, z, x);
        
        /// <summary>
        /// Returns Vector4.gbbr swizzling (equivalent to Vector4.yzzx).
        /// </summary>
        public Vector4 gbbr => new Vector4(y, z, z, x);
        
        /// <summary>
        /// Returns Vector4.yzzy swizzling.
        /// </summary>
        public Vector4 yzzy => new Vector4(y, z, z, y);
        
        /// <summary>
        /// Returns Vector4.gbbg swizzling (equivalent to Vector4.yzzy).
        /// </summary>
        public Vector4 gbbg => new Vector4(y, z, z, y);
        
        /// <summary>
        /// Returns Vector4.yzzz swizzling.
        /// </summary>
        public Vector4 yzzz => new Vector4(y, z, z, z);
        
        /// <summary>
        /// Returns Vector4.gbbb swizzling (equivalent to Vector4.yzzz).
        /// </summary>
        public Vector4 gbbb => new Vector4(y, z, z, z);
        
        /// <summary>
        /// Returns Vector4.yzzw swizzling.
        /// </summary>
        public Vector4 yzzw => new Vector4(y, z, z, w);
        
        /// <summary>
        /// Returns Vector4.gbba swizzling (equivalent to Vector4.yzzw).
        /// </summary>
        public Vector4 gbba => new Vector4(y, z, z, w);
        
        /// <summary>
        /// Returns Vector4.yzw swizzling.
        /// </summary>
        public Vector3 yzw => new Vector3(y, z, w);
        
        /// <summary>
        /// Returns Vector4.gba swizzling (equivalent to Vector4.yzw).
        /// </summary>
        public Vector3 gba => new Vector3(y, z, w);
        
        /// <summary>
        /// Returns Vector4.yzwx swizzling.
        /// </summary>
        public Vector4 yzwx => new Vector4(y, z, w, x);
        
        /// <summary>
        /// Returns Vector4.gbar swizzling (equivalent to Vector4.yzwx).
        /// </summary>
        public Vector4 gbar => new Vector4(y, z, w, x);
        
        /// <summary>
        /// Returns Vector4.yzwy swizzling.
        /// </summary>
        public Vector4 yzwy => new Vector4(y, z, w, y);
        
        /// <summary>
        /// Returns Vector4.gbag swizzling (equivalent to Vector4.yzwy).
        /// </summary>
        public Vector4 gbag => new Vector4(y, z, w, y);
        
        /// <summary>
        /// Returns Vector4.yzwz swizzling.
        /// </summary>
        public Vector4 yzwz => new Vector4(y, z, w, z);
        
        /// <summary>
        /// Returns Vector4.gbab swizzling (equivalent to Vector4.yzwz).
        /// </summary>
        public Vector4 gbab => new Vector4(y, z, w, z);
        
        /// <summary>
        /// Returns Vector4.yzww swizzling.
        /// </summary>
        public Vector4 yzww => new Vector4(y, z, w, w);
        
        /// <summary>
        /// Returns Vector4.gbaa swizzling (equivalent to Vector4.yzww).
        /// </summary>
        public Vector4 gbaa => new Vector4(y, z, w, w);
        
        /// <summary>
        /// Returns Vector4.yw swizzling.
        /// </summary>
        public Vector2 yw => new Vector2(y, w);
        
        /// <summary>
        /// Returns Vector4.ga swizzling (equivalent to Vector4.yw).
        /// </summary>
        public Vector2 ga => new Vector2(y, w);
        
        /// <summary>
        /// Returns Vector4.ywx swizzling.
        /// </summary>
        public Vector3 ywx => new Vector3(y, w, x);
        
        /// <summary>
        /// Returns Vector4.gar swizzling (equivalent to Vector4.ywx).
        /// </summary>
        public Vector3 gar => new Vector3(y, w, x);
        
        /// <summary>
        /// Returns Vector4.ywxx swizzling.
        /// </summary>
        public Vector4 ywxx => new Vector4(y, w, x, x);
        
        /// <summary>
        /// Returns Vector4.garr swizzling (equivalent to Vector4.ywxx).
        /// </summary>
        public Vector4 garr => new Vector4(y, w, x, x);
        
        /// <summary>
        /// Returns Vector4.ywxy swizzling.
        /// </summary>
        public Vector4 ywxy => new Vector4(y, w, x, y);
        
        /// <summary>
        /// Returns Vector4.garg swizzling (equivalent to Vector4.ywxy).
        /// </summary>
        public Vector4 garg => new Vector4(y, w, x, y);
        
        /// <summary>
        /// Returns Vector4.ywxz swizzling.
        /// </summary>
        public Vector4 ywxz => new Vector4(y, w, x, z);
        
        /// <summary>
        /// Returns Vector4.garb swizzling (equivalent to Vector4.ywxz).
        /// </summary>
        public Vector4 garb => new Vector4(y, w, x, z);
        
        /// <summary>
        /// Returns Vector4.ywxw swizzling.
        /// </summary>
        public Vector4 ywxw => new Vector4(y, w, x, w);
        
        /// <summary>
        /// Returns Vector4.gara swizzling (equivalent to Vector4.ywxw).
        /// </summary>
        public Vector4 gara => new Vector4(y, w, x, w);
        
        /// <summary>
        /// Returns Vector4.ywy swizzling.
        /// </summary>
        public Vector3 ywy => new Vector3(y, w, y);
        
        /// <summary>
        /// Returns Vector4.gag swizzling (equivalent to Vector4.ywy).
        /// </summary>
        public Vector3 gag => new Vector3(y, w, y);
        
        /// <summary>
        /// Returns Vector4.ywyx swizzling.
        /// </summary>
        public Vector4 ywyx => new Vector4(y, w, y, x);
        
        /// <summary>
        /// Returns Vector4.gagr swizzling (equivalent to Vector4.ywyx).
        /// </summary>
        public Vector4 gagr => new Vector4(y, w, y, x);
        
        /// <summary>
        /// Returns Vector4.ywyy swizzling.
        /// </summary>
        public Vector4 ywyy => new Vector4(y, w, y, y);
        
        /// <summary>
        /// Returns Vector4.gagg swizzling (equivalent to Vector4.ywyy).
        /// </summary>
        public Vector4 gagg => new Vector4(y, w, y, y);
        
        /// <summary>
        /// Returns Vector4.ywyz swizzling.
        /// </summary>
        public Vector4 ywyz => new Vector4(y, w, y, z);
        
        /// <summary>
        /// Returns Vector4.gagb swizzling (equivalent to Vector4.ywyz).
        /// </summary>
        public Vector4 gagb => new Vector4(y, w, y, z);
        
        /// <summary>
        /// Returns Vector4.ywyw swizzling.
        /// </summary>
        public Vector4 ywyw => new Vector4(y, w, y, w);
        
        /// <summary>
        /// Returns Vector4.gaga swizzling (equivalent to Vector4.ywyw).
        /// </summary>
        public Vector4 gaga => new Vector4(y, w, y, w);
        
        /// <summary>
        /// Returns Vector4.ywz swizzling.
        /// </summary>
        public Vector3 ywz => new Vector3(y, w, z);
        
        /// <summary>
        /// Returns Vector4.gab swizzling (equivalent to Vector4.ywz).
        /// </summary>
        public Vector3 gab => new Vector3(y, w, z);
        
        /// <summary>
        /// Returns Vector4.ywzx swizzling.
        /// </summary>
        public Vector4 ywzx => new Vector4(y, w, z, x);
        
        /// <summary>
        /// Returns Vector4.gabr swizzling (equivalent to Vector4.ywzx).
        /// </summary>
        public Vector4 gabr => new Vector4(y, w, z, x);
        
        /// <summary>
        /// Returns Vector4.ywzy swizzling.
        /// </summary>
        public Vector4 ywzy => new Vector4(y, w, z, y);
        
        /// <summary>
        /// Returns Vector4.gabg swizzling (equivalent to Vector4.ywzy).
        /// </summary>
        public Vector4 gabg => new Vector4(y, w, z, y);
        
        /// <summary>
        /// Returns Vector4.ywzz swizzling.
        /// </summary>
        public Vector4 ywzz => new Vector4(y, w, z, z);
        
        /// <summary>
        /// Returns Vector4.gabb swizzling (equivalent to Vector4.ywzz).
        /// </summary>
        public Vector4 gabb => new Vector4(y, w, z, z);
        
        /// <summary>
        /// Returns Vector4.ywzw swizzling.
        /// </summary>
        public Vector4 ywzw => new Vector4(y, w, z, w);
        
        /// <summary>
        /// Returns Vector4.gaba swizzling (equivalent to Vector4.ywzw).
        /// </summary>
        public Vector4 gaba => new Vector4(y, w, z, w);
        
        /// <summary>
        /// Returns Vector4.yww swizzling.
        /// </summary>
        public Vector3 yww => new Vector3(y, w, w);
        
        /// <summary>
        /// Returns Vector4.gaa swizzling (equivalent to Vector4.yww).
        /// </summary>
        public Vector3 gaa => new Vector3(y, w, w);
        
        /// <summary>
        /// Returns Vector4.ywwx swizzling.
        /// </summary>
        public Vector4 ywwx => new Vector4(y, w, w, x);
        
        /// <summary>
        /// Returns Vector4.gaar swizzling (equivalent to Vector4.ywwx).
        /// </summary>
        public Vector4 gaar => new Vector4(y, w, w, x);
        
        /// <summary>
        /// Returns Vector4.ywwy swizzling.
        /// </summary>
        public Vector4 ywwy => new Vector4(y, w, w, y);
        
        /// <summary>
        /// Returns Vector4.gaag swizzling (equivalent to Vector4.ywwy).
        /// </summary>
        public Vector4 gaag => new Vector4(y, w, w, y);
        
        /// <summary>
        /// Returns Vector4.ywwz swizzling.
        /// </summary>
        public Vector4 ywwz => new Vector4(y, w, w, z);
        
        /// <summary>
        /// Returns Vector4.gaab swizzling (equivalent to Vector4.ywwz).
        /// </summary>
        public Vector4 gaab => new Vector4(y, w, w, z);
        
        /// <summary>
        /// Returns Vector4.ywww swizzling.
        /// </summary>
        public Vector4 ywww => new Vector4(y, w, w, w);
        
        /// <summary>
        /// Returns Vector4.gaaa swizzling (equivalent to Vector4.ywww).
        /// </summary>
        public Vector4 gaaa => new Vector4(y, w, w, w);
        
        /// <summary>
        /// Returns Vector4.zx swizzling.
        /// </summary>
        public Vector2 zx => new Vector2(z, x);
        
        /// <summary>
        /// Returns Vector4.br swizzling (equivalent to Vector4.zx).
        /// </summary>
        public Vector2 br => new Vector2(z, x);
        
        /// <summary>
        /// Returns Vector4.zxx swizzling.
        /// </summary>
        public Vector3 zxx => new Vector3(z, x, x);
        
        /// <summary>
        /// Returns Vector4.brr swizzling (equivalent to Vector4.zxx).
        /// </summary>
        public Vector3 brr => new Vector3(z, x, x);
        
        /// <summary>
        /// Returns Vector4.zxxx swizzling.
        /// </summary>
        public Vector4 zxxx => new Vector4(z, x, x, x);
        
        /// <summary>
        /// Returns Vector4.brrr swizzling (equivalent to Vector4.zxxx).
        /// </summary>
        public Vector4 brrr => new Vector4(z, x, x, x);
        
        /// <summary>
        /// Returns Vector4.zxxy swizzling.
        /// </summary>
        public Vector4 zxxy => new Vector4(z, x, x, y);
        
        /// <summary>
        /// Returns Vector4.brrg swizzling (equivalent to Vector4.zxxy).
        /// </summary>
        public Vector4 brrg => new Vector4(z, x, x, y);
        
        /// <summary>
        /// Returns Vector4.zxxz swizzling.
        /// </summary>
        public Vector4 zxxz => new Vector4(z, x, x, z);
        
        /// <summary>
        /// Returns Vector4.brrb swizzling (equivalent to Vector4.zxxz).
        /// </summary>
        public Vector4 brrb => new Vector4(z, x, x, z);
        
        /// <summary>
        /// Returns Vector4.zxxw swizzling.
        /// </summary>
        public Vector4 zxxw => new Vector4(z, x, x, w);
        
        /// <summary>
        /// Returns Vector4.brra swizzling (equivalent to Vector4.zxxw).
        /// </summary>
        public Vector4 brra => new Vector4(z, x, x, w);
        
        /// <summary>
        /// Returns Vector4.zxy swizzling.
        /// </summary>
        public Vector3 zxy => new Vector3(z, x, y);
        
        /// <summary>
        /// Returns Vector4.brg swizzling (equivalent to Vector4.zxy).
        /// </summary>
        public Vector3 brg => new Vector3(z, x, y);
        
        /// <summary>
        /// Returns Vector4.zxyx swizzling.
        /// </summary>
        public Vector4 zxyx => new Vector4(z, x, y, x);
        
        /// <summary>
        /// Returns Vector4.brgr swizzling (equivalent to Vector4.zxyx).
        /// </summary>
        public Vector4 brgr => new Vector4(z, x, y, x);
        
        /// <summary>
        /// Returns Vector4.zxyy swizzling.
        /// </summary>
        public Vector4 zxyy => new Vector4(z, x, y, y);
        
        /// <summary>
        /// Returns Vector4.brgg swizzling (equivalent to Vector4.zxyy).
        /// </summary>
        public Vector4 brgg => new Vector4(z, x, y, y);
        
        /// <summary>
        /// Returns Vector4.zxyz swizzling.
        /// </summary>
        public Vector4 zxyz => new Vector4(z, x, y, z);
        
        /// <summary>
        /// Returns Vector4.brgb swizzling (equivalent to Vector4.zxyz).
        /// </summary>
        public Vector4 brgb => new Vector4(z, x, y, z);
        
        /// <summary>
        /// Returns Vector4.zxyw swizzling.
        /// </summary>
        public Vector4 zxyw => new Vector4(z, x, y, w);
        
        /// <summary>
        /// Returns Vector4.brga swizzling (equivalent to Vector4.zxyw).
        /// </summary>
        public Vector4 brga => new Vector4(z, x, y, w);
        
        /// <summary>
        /// Returns Vector4.zxz swizzling.
        /// </summary>
        public Vector3 zxz => new Vector3(z, x, z);
        
        /// <summary>
        /// Returns Vector4.brb swizzling (equivalent to Vector4.zxz).
        /// </summary>
        public Vector3 brb => new Vector3(z, x, z);
        
        /// <summary>
        /// Returns Vector4.zxzx swizzling.
        /// </summary>
        public Vector4 zxzx => new Vector4(z, x, z, x);
        
        /// <summary>
        /// Returns Vector4.brbr swizzling (equivalent to Vector4.zxzx).
        /// </summary>
        public Vector4 brbr => new Vector4(z, x, z, x);
        
        /// <summary>
        /// Returns Vector4.zxzy swizzling.
        /// </summary>
        public Vector4 zxzy => new Vector4(z, x, z, y);
        
        /// <summary>
        /// Returns Vector4.brbg swizzling (equivalent to Vector4.zxzy).
        /// </summary>
        public Vector4 brbg => new Vector4(z, x, z, y);
        
        /// <summary>
        /// Returns Vector4.zxzz swizzling.
        /// </summary>
        public Vector4 zxzz => new Vector4(z, x, z, z);
        
        /// <summary>
        /// Returns Vector4.brbb swizzling (equivalent to Vector4.zxzz).
        /// </summary>
        public Vector4 brbb => new Vector4(z, x, z, z);
        
        /// <summary>
        /// Returns Vector4.zxzw swizzling.
        /// </summary>
        public Vector4 zxzw => new Vector4(z, x, z, w);
        
        /// <summary>
        /// Returns Vector4.brba swizzling (equivalent to Vector4.zxzw).
        /// </summary>
        public Vector4 brba => new Vector4(z, x, z, w);
        
        /// <summary>
        /// Returns Vector4.zxw swizzling.
        /// </summary>
        public Vector3 zxw => new Vector3(z, x, w);
        
        /// <summary>
        /// Returns Vector4.bra swizzling (equivalent to Vector4.zxw).
        /// </summary>
        public Vector3 bra => new Vector3(z, x, w);
        
        /// <summary>
        /// Returns Vector4.zxwx swizzling.
        /// </summary>
        public Vector4 zxwx => new Vector4(z, x, w, x);
        
        /// <summary>
        /// Returns Vector4.brar swizzling (equivalent to Vector4.zxwx).
        /// </summary>
        public Vector4 brar => new Vector4(z, x, w, x);
        
        /// <summary>
        /// Returns Vector4.zxwy swizzling.
        /// </summary>
        public Vector4 zxwy => new Vector4(z, x, w, y);
        
        /// <summary>
        /// Returns Vector4.brag swizzling (equivalent to Vector4.zxwy).
        /// </summary>
        public Vector4 brag => new Vector4(z, x, w, y);
        
        /// <summary>
        /// Returns Vector4.zxwz swizzling.
        /// </summary>
        public Vector4 zxwz => new Vector4(z, x, w, z);
        
        /// <summary>
        /// Returns Vector4.brab swizzling (equivalent to Vector4.zxwz).
        /// </summary>
        public Vector4 brab => new Vector4(z, x, w, z);
        
        /// <summary>
        /// Returns Vector4.zxww swizzling.
        /// </summary>
        public Vector4 zxww => new Vector4(z, x, w, w);
        
        /// <summary>
        /// Returns Vector4.braa swizzling (equivalent to Vector4.zxww).
        /// </summary>
        public Vector4 braa => new Vector4(z, x, w, w);
        
        /// <summary>
        /// Returns Vector4.zy swizzling.
        /// </summary>
        public Vector2 zy => new Vector2(z, y);
        
        /// <summary>
        /// Returns Vector4.bg swizzling (equivalent to Vector4.zy).
        /// </summary>
        public Vector2 bg => new Vector2(z, y);
        
        /// <summary>
        /// Returns Vector4.zyx swizzling.
        /// </summary>
        public Vector3 zyx => new Vector3(z, y, x);
        
        /// <summary>
        /// Returns Vector4.bgr swizzling (equivalent to Vector4.zyx).
        /// </summary>
        public Vector3 bgr => new Vector3(z, y, x);
        
        /// <summary>
        /// Returns Vector4.zyxx swizzling.
        /// </summary>
        public Vector4 zyxx => new Vector4(z, y, x, x);
        
        /// <summary>
        /// Returns Vector4.bgrr swizzling (equivalent to Vector4.zyxx).
        /// </summary>
        public Vector4 bgrr => new Vector4(z, y, x, x);
        
        /// <summary>
        /// Returns Vector4.zyxy swizzling.
        /// </summary>
        public Vector4 zyxy => new Vector4(z, y, x, y);
        
        /// <summary>
        /// Returns Vector4.bgrg swizzling (equivalent to Vector4.zyxy).
        /// </summary>
        public Vector4 bgrg => new Vector4(z, y, x, y);
        
        /// <summary>
        /// Returns Vector4.zyxz swizzling.
        /// </summary>
        public Vector4 zyxz => new Vector4(z, y, x, z);
        
        /// <summary>
        /// Returns Vector4.bgrb swizzling (equivalent to Vector4.zyxz).
        /// </summary>
        public Vector4 bgrb => new Vector4(z, y, x, z);
        
        /// <summary>
        /// Returns Vector4.zyxw swizzling.
        /// </summary>
        public Vector4 zyxw => new Vector4(z, y, x, w);
        
        /// <summary>
        /// Returns Vector4.bgra swizzling (equivalent to Vector4.zyxw).
        /// </summary>
        public Vector4 bgra => new Vector4(z, y, x, w);
        
        /// <summary>
        /// Returns Vector4.zyy swizzling.
        /// </summary>
        public Vector3 zyy => new Vector3(z, y, y);
        
        /// <summary>
        /// Returns Vector4.bgg swizzling (equivalent to Vector4.zyy).
        /// </summary>
        public Vector3 bgg => new Vector3(z, y, y);
        
        /// <summary>
        /// Returns Vector4.zyyx swizzling.
        /// </summary>
        public Vector4 zyyx => new Vector4(z, y, y, x);
        
        /// <summary>
        /// Returns Vector4.bggr swizzling (equivalent to Vector4.zyyx).
        /// </summary>
        public Vector4 bggr => new Vector4(z, y, y, x);
        
        /// <summary>
        /// Returns Vector4.zyyy swizzling.
        /// </summary>
        public Vector4 zyyy => new Vector4(z, y, y, y);
        
        /// <summary>
        /// Returns Vector4.bggg swizzling (equivalent to Vector4.zyyy).
        /// </summary>
        public Vector4 bggg => new Vector4(z, y, y, y);
        
        /// <summary>
        /// Returns Vector4.zyyz swizzling.
        /// </summary>
        public Vector4 zyyz => new Vector4(z, y, y, z);
        
        /// <summary>
        /// Returns Vector4.bggb swizzling (equivalent to Vector4.zyyz).
        /// </summary>
        public Vector4 bggb => new Vector4(z, y, y, z);
        
        /// <summary>
        /// Returns Vector4.zyyw swizzling.
        /// </summary>
        public Vector4 zyyw => new Vector4(z, y, y, w);
        
        /// <summary>
        /// Returns Vector4.bgga swizzling (equivalent to Vector4.zyyw).
        /// </summary>
        public Vector4 bgga => new Vector4(z, y, y, w);
        
        /// <summary>
        /// Returns Vector4.zyz swizzling.
        /// </summary>
        public Vector3 zyz => new Vector3(z, y, z);
        
        /// <summary>
        /// Returns Vector4.bgb swizzling (equivalent to Vector4.zyz).
        /// </summary>
        public Vector3 bgb => new Vector3(z, y, z);
        
        /// <summary>
        /// Returns Vector4.zyzx swizzling.
        /// </summary>
        public Vector4 zyzx => new Vector4(z, y, z, x);
        
        /// <summary>
        /// Returns Vector4.bgbr swizzling (equivalent to Vector4.zyzx).
        /// </summary>
        public Vector4 bgbr => new Vector4(z, y, z, x);
        
        /// <summary>
        /// Returns Vector4.zyzy swizzling.
        /// </summary>
        public Vector4 zyzy => new Vector4(z, y, z, y);
        
        /// <summary>
        /// Returns Vector4.bgbg swizzling (equivalent to Vector4.zyzy).
        /// </summary>
        public Vector4 bgbg => new Vector4(z, y, z, y);
        
        /// <summary>
        /// Returns Vector4.zyzz swizzling.
        /// </summary>
        public Vector4 zyzz => new Vector4(z, y, z, z);
        
        /// <summary>
        /// Returns Vector4.bgbb swizzling (equivalent to Vector4.zyzz).
        /// </summary>
        public Vector4 bgbb => new Vector4(z, y, z, z);
        
        /// <summary>
        /// Returns Vector4.zyzw swizzling.
        /// </summary>
        public Vector4 zyzw => new Vector4(z, y, z, w);
        
        /// <summary>
        /// Returns Vector4.bgba swizzling (equivalent to Vector4.zyzw).
        /// </summary>
        public Vector4 bgba => new Vector4(z, y, z, w);
        
        /// <summary>
        /// Returns Vector4.zyw swizzling.
        /// </summary>
        public Vector3 zyw => new Vector3(z, y, w);
        
        /// <summary>
        /// Returns Vector4.bga swizzling (equivalent to Vector4.zyw).
        /// </summary>
        public Vector3 bga => new Vector3(z, y, w);
        
        /// <summary>
        /// Returns Vector4.zywx swizzling.
        /// </summary>
        public Vector4 zywx => new Vector4(z, y, w, x);
        
        /// <summary>
        /// Returns Vector4.bgar swizzling (equivalent to Vector4.zywx).
        /// </summary>
        public Vector4 bgar => new Vector4(z, y, w, x);
        
        /// <summary>
        /// Returns Vector4.zywy swizzling.
        /// </summary>
        public Vector4 zywy => new Vector4(z, y, w, y);
        
        /// <summary>
        /// Returns Vector4.bgag swizzling (equivalent to Vector4.zywy).
        /// </summary>
        public Vector4 bgag => new Vector4(z, y, w, y);
        
        /// <summary>
        /// Returns Vector4.zywz swizzling.
        /// </summary>
        public Vector4 zywz => new Vector4(z, y, w, z);
        
        /// <summary>
        /// Returns Vector4.bgab swizzling (equivalent to Vector4.zywz).
        /// </summary>
        public Vector4 bgab => new Vector4(z, y, w, z);
        
        /// <summary>
        /// Returns Vector4.zyww swizzling.
        /// </summary>
        public Vector4 zyww => new Vector4(z, y, w, w);
        
        /// <summary>
        /// Returns Vector4.bgaa swizzling (equivalent to Vector4.zyww).
        /// </summary>
        public Vector4 bgaa => new Vector4(z, y, w, w);
        
        /// <summary>
        /// Returns Vector4.zz swizzling.
        /// </summary>
        public Vector2 zz => new Vector2(z, z);
        
        /// <summary>
        /// Returns Vector4.bb swizzling (equivalent to Vector4.zz).
        /// </summary>
        public Vector2 bb => new Vector2(z, z);
        
        /// <summary>
        /// Returns Vector4.zzx swizzling.
        /// </summary>
        public Vector3 zzx => new Vector3(z, z, x);
        
        /// <summary>
        /// Returns Vector4.bbr swizzling (equivalent to Vector4.zzx).
        /// </summary>
        public Vector3 bbr => new Vector3(z, z, x);
        
        /// <summary>
        /// Returns Vector4.zzxx swizzling.
        /// </summary>
        public Vector4 zzxx => new Vector4(z, z, x, x);
        
        /// <summary>
        /// Returns Vector4.bbrr swizzling (equivalent to Vector4.zzxx).
        /// </summary>
        public Vector4 bbrr => new Vector4(z, z, x, x);
        
        /// <summary>
        /// Returns Vector4.zzxy swizzling.
        /// </summary>
        public Vector4 zzxy => new Vector4(z, z, x, y);
        
        /// <summary>
        /// Returns Vector4.bbrg swizzling (equivalent to Vector4.zzxy).
        /// </summary>
        public Vector4 bbrg => new Vector4(z, z, x, y);
        
        /// <summary>
        /// Returns Vector4.zzxz swizzling.
        /// </summary>
        public Vector4 zzxz => new Vector4(z, z, x, z);
        
        /// <summary>
        /// Returns Vector4.bbrb swizzling (equivalent to Vector4.zzxz).
        /// </summary>
        public Vector4 bbrb => new Vector4(z, z, x, z);
        
        /// <summary>
        /// Returns Vector4.zzxw swizzling.
        /// </summary>
        public Vector4 zzxw => new Vector4(z, z, x, w);
        
        /// <summary>
        /// Returns Vector4.bbra swizzling (equivalent to Vector4.zzxw).
        /// </summary>
        public Vector4 bbra => new Vector4(z, z, x, w);
        
        /// <summary>
        /// Returns Vector4.zzy swizzling.
        /// </summary>
        public Vector3 zzy => new Vector3(z, z, y);
        
        /// <summary>
        /// Returns Vector4.bbg swizzling (equivalent to Vector4.zzy).
        /// </summary>
        public Vector3 bbg => new Vector3(z, z, y);
        
        /// <summary>
        /// Returns Vector4.zzyx swizzling.
        /// </summary>
        public Vector4 zzyx => new Vector4(z, z, y, x);
        
        /// <summary>
        /// Returns Vector4.bbgr swizzling (equivalent to Vector4.zzyx).
        /// </summary>
        public Vector4 bbgr => new Vector4(z, z, y, x);
        
        /// <summary>
        /// Returns Vector4.zzyy swizzling.
        /// </summary>
        public Vector4 zzyy => new Vector4(z, z, y, y);
        
        /// <summary>
        /// Returns Vector4.bbgg swizzling (equivalent to Vector4.zzyy).
        /// </summary>
        public Vector4 bbgg => new Vector4(z, z, y, y);
        
        /// <summary>
        /// Returns Vector4.zzyz swizzling.
        /// </summary>
        public Vector4 zzyz => new Vector4(z, z, y, z);
        
        /// <summary>
        /// Returns Vector4.bbgb swizzling (equivalent to Vector4.zzyz).
        /// </summary>
        public Vector4 bbgb => new Vector4(z, z, y, z);
        
        /// <summary>
        /// Returns Vector4.zzyw swizzling.
        /// </summary>
        public Vector4 zzyw => new Vector4(z, z, y, w);
        
        /// <summary>
        /// Returns Vector4.bbga swizzling (equivalent to Vector4.zzyw).
        /// </summary>
        public Vector4 bbga => new Vector4(z, z, y, w);
        
        /// <summary>
        /// Returns Vector4.zzz swizzling.
        /// </summary>
        public Vector3 zzz => new Vector3(z, z, z);
        
        /// <summary>
        /// Returns Vector4.bbb swizzling (equivalent to Vector4.zzz).
        /// </summary>
        public Vector3 bbb => new Vector3(z, z, z);
        
        /// <summary>
        /// Returns Vector4.zzzx swizzling.
        /// </summary>
        public Vector4 zzzx => new Vector4(z, z, z, x);
        
        /// <summary>
        /// Returns Vector4.bbbr swizzling (equivalent to Vector4.zzzx).
        /// </summary>
        public Vector4 bbbr => new Vector4(z, z, z, x);
        
        /// <summary>
        /// Returns Vector4.zzzy swizzling.
        /// </summary>
        public Vector4 zzzy => new Vector4(z, z, z, y);
        
        /// <summary>
        /// Returns Vector4.bbbg swizzling (equivalent to Vector4.zzzy).
        /// </summary>
        public Vector4 bbbg => new Vector4(z, z, z, y);
        
        /// <summary>
        /// Returns Vector4.zzzz swizzling.
        /// </summary>
        public Vector4 zzzz => new Vector4(z, z, z, z);
        
        /// <summary>
        /// Returns Vector4.bbbb swizzling (equivalent to Vector4.zzzz).
        /// </summary>
        public Vector4 bbbb => new Vector4(z, z, z, z);
        
        /// <summary>
        /// Returns Vector4.zzzw swizzling.
        /// </summary>
        public Vector4 zzzw => new Vector4(z, z, z, w);
        
        /// <summary>
        /// Returns Vector4.bbba swizzling (equivalent to Vector4.zzzw).
        /// </summary>
        public Vector4 bbba => new Vector4(z, z, z, w);
        
        /// <summary>
        /// Returns Vector4.zzw swizzling.
        /// </summary>
        public Vector3 zzw => new Vector3(z, z, w);
        
        /// <summary>
        /// Returns Vector4.bba swizzling (equivalent to Vector4.zzw).
        /// </summary>
        public Vector3 bba => new Vector3(z, z, w);
        
        /// <summary>
        /// Returns Vector4.zzwx swizzling.
        /// </summary>
        public Vector4 zzwx => new Vector4(z, z, w, x);
        
        /// <summary>
        /// Returns Vector4.bbar swizzling (equivalent to Vector4.zzwx).
        /// </summary>
        public Vector4 bbar => new Vector4(z, z, w, x);
        
        /// <summary>
        /// Returns Vector4.zzwy swizzling.
        /// </summary>
        public Vector4 zzwy => new Vector4(z, z, w, y);
        
        /// <summary>
        /// Returns Vector4.bbag swizzling (equivalent to Vector4.zzwy).
        /// </summary>
        public Vector4 bbag => new Vector4(z, z, w, y);
        
        /// <summary>
        /// Returns Vector4.zzwz swizzling.
        /// </summary>
        public Vector4 zzwz => new Vector4(z, z, w, z);
        
        /// <summary>
        /// Returns Vector4.bbab swizzling (equivalent to Vector4.zzwz).
        /// </summary>
        public Vector4 bbab => new Vector4(z, z, w, z);
        
        /// <summary>
        /// Returns Vector4.zzww swizzling.
        /// </summary>
        public Vector4 zzww => new Vector4(z, z, w, w);
        
        /// <summary>
        /// Returns Vector4.bbaa swizzling (equivalent to Vector4.zzww).
        /// </summary>
        public Vector4 bbaa => new Vector4(z, z, w, w);
        
        /// <summary>
        /// Returns Vector4.zw swizzling.
        /// </summary>
        public Vector2 zw => new Vector2(z, w);
        
        /// <summary>
        /// Returns Vector4.ba swizzling (equivalent to Vector4.zw).
        /// </summary>
        public Vector2 ba => new Vector2(z, w);
        
        /// <summary>
        /// Returns Vector4.zwx swizzling.
        /// </summary>
        public Vector3 zwx => new Vector3(z, w, x);
        
        /// <summary>
        /// Returns Vector4.bar swizzling (equivalent to Vector4.zwx).
        /// </summary>
        public Vector3 bar => new Vector3(z, w, x);
        
        /// <summary>
        /// Returns Vector4.zwxx swizzling.
        /// </summary>
        public Vector4 zwxx => new Vector4(z, w, x, x);
        
        /// <summary>
        /// Returns Vector4.barr swizzling (equivalent to Vector4.zwxx).
        /// </summary>
        public Vector4 barr => new Vector4(z, w, x, x);
        
        /// <summary>
        /// Returns Vector4.zwxy swizzling.
        /// </summary>
        public Vector4 zwxy => new Vector4(z, w, x, y);
        
        /// <summary>
        /// Returns Vector4.barg swizzling (equivalent to Vector4.zwxy).
        /// </summary>
        public Vector4 barg => new Vector4(z, w, x, y);
        
        /// <summary>
        /// Returns Vector4.zwxz swizzling.
        /// </summary>
        public Vector4 zwxz => new Vector4(z, w, x, z);
        
        /// <summary>
        /// Returns Vector4.barb swizzling (equivalent to Vector4.zwxz).
        /// </summary>
        public Vector4 barb => new Vector4(z, w, x, z);
        
        /// <summary>
        /// Returns Vector4.zwxw swizzling.
        /// </summary>
        public Vector4 zwxw => new Vector4(z, w, x, w);
        
        /// <summary>
        /// Returns Vector4.bara swizzling (equivalent to Vector4.zwxw).
        /// </summary>
        public Vector4 bara => new Vector4(z, w, x, w);
        
        /// <summary>
        /// Returns Vector4.zwy swizzling.
        /// </summary>
        public Vector3 zwy => new Vector3(z, w, y);
        
        /// <summary>
        /// Returns Vector4.bag swizzling (equivalent to Vector4.zwy).
        /// </summary>
        public Vector3 bag => new Vector3(z, w, y);
        
        /// <summary>
        /// Returns Vector4.zwyx swizzling.
        /// </summary>
        public Vector4 zwyx => new Vector4(z, w, y, x);
        
        /// <summary>
        /// Returns Vector4.bagr swizzling (equivalent to Vector4.zwyx).
        /// </summary>
        public Vector4 bagr => new Vector4(z, w, y, x);
        
        /// <summary>
        /// Returns Vector4.zwyy swizzling.
        /// </summary>
        public Vector4 zwyy => new Vector4(z, w, y, y);
        
        /// <summary>
        /// Returns Vector4.bagg swizzling (equivalent to Vector4.zwyy).
        /// </summary>
        public Vector4 bagg => new Vector4(z, w, y, y);
        
        /// <summary>
        /// Returns Vector4.zwyz swizzling.
        /// </summary>
        public Vector4 zwyz => new Vector4(z, w, y, z);
        
        /// <summary>
        /// Returns Vector4.bagb swizzling (equivalent to Vector4.zwyz).
        /// </summary>
        public Vector4 bagb => new Vector4(z, w, y, z);
        
        /// <summary>
        /// Returns Vector4.zwyw swizzling.
        /// </summary>
        public Vector4 zwyw => new Vector4(z, w, y, w);
        
        /// <summary>
        /// Returns Vector4.baga swizzling (equivalent to Vector4.zwyw).
        /// </summary>
        public Vector4 baga => new Vector4(z, w, y, w);
        
        /// <summary>
        /// Returns Vector4.zwz swizzling.
        /// </summary>
        public Vector3 zwz => new Vector3(z, w, z);
        
        /// <summary>
        /// Returns Vector4.bab swizzling (equivalent to Vector4.zwz).
        /// </summary>
        public Vector3 bab => new Vector3(z, w, z);
        
        /// <summary>
        /// Returns Vector4.zwzx swizzling.
        /// </summary>
        public Vector4 zwzx => new Vector4(z, w, z, x);
        
        /// <summary>
        /// Returns Vector4.babr swizzling (equivalent to Vector4.zwzx).
        /// </summary>
        public Vector4 babr => new Vector4(z, w, z, x);
        
        /// <summary>
        /// Returns Vector4.zwzy swizzling.
        /// </summary>
        public Vector4 zwzy => new Vector4(z, w, z, y);
        
        /// <summary>
        /// Returns Vector4.babg swizzling (equivalent to Vector4.zwzy).
        /// </summary>
        public Vector4 babg => new Vector4(z, w, z, y);
        
        /// <summary>
        /// Returns Vector4.zwzz swizzling.
        /// </summary>
        public Vector4 zwzz => new Vector4(z, w, z, z);
        
        /// <summary>
        /// Returns Vector4.babb swizzling (equivalent to Vector4.zwzz).
        /// </summary>
        public Vector4 babb => new Vector4(z, w, z, z);
        
        /// <summary>
        /// Returns Vector4.zwzw swizzling.
        /// </summary>
        public Vector4 zwzw => new Vector4(z, w, z, w);
        
        /// <summary>
        /// Returns Vector4.baba swizzling (equivalent to Vector4.zwzw).
        /// </summary>
        public Vector4 baba => new Vector4(z, w, z, w);
        
        /// <summary>
        /// Returns Vector4.zww swizzling.
        /// </summary>
        public Vector3 zww => new Vector3(z, w, w);
        
        /// <summary>
        /// Returns Vector4.baa swizzling (equivalent to Vector4.zww).
        /// </summary>
        public Vector3 baa => new Vector3(z, w, w);
        
        /// <summary>
        /// Returns Vector4.zwwx swizzling.
        /// </summary>
        public Vector4 zwwx => new Vector4(z, w, w, x);
        
        /// <summary>
        /// Returns Vector4.baar swizzling (equivalent to Vector4.zwwx).
        /// </summary>
        public Vector4 baar => new Vector4(z, w, w, x);
        
        /// <summary>
        /// Returns Vector4.zwwy swizzling.
        /// </summary>
        public Vector4 zwwy => new Vector4(z, w, w, y);
        
        /// <summary>
        /// Returns Vector4.baag swizzling (equivalent to Vector4.zwwy).
        /// </summary>
        public Vector4 baag => new Vector4(z, w, w, y);
        
        /// <summary>
        /// Returns Vector4.zwwz swizzling.
        /// </summary>
        public Vector4 zwwz => new Vector4(z, w, w, z);
        
        /// <summary>
        /// Returns Vector4.baab swizzling (equivalent to Vector4.zwwz).
        /// </summary>
        public Vector4 baab => new Vector4(z, w, w, z);
        
        /// <summary>
        /// Returns Vector4.zwww swizzling.
        /// </summary>
        public Vector4 zwww => new Vector4(z, w, w, w);
        
        /// <summary>
        /// Returns Vector4.baaa swizzling (equivalent to Vector4.zwww).
        /// </summary>
        public Vector4 baaa => new Vector4(z, w, w, w);
        
        /// <summary>
        /// Returns Vector4.wx swizzling.
        /// </summary>
        public Vector2 wx => new Vector2(w, x);
        
        /// <summary>
        /// Returns Vector4.ar swizzling (equivalent to Vector4.wx).
        /// </summary>
        public Vector2 ar => new Vector2(w, x);
        
        /// <summary>
        /// Returns Vector4.wxx swizzling.
        /// </summary>
        public Vector3 wxx => new Vector3(w, x, x);
        
        /// <summary>
        /// Returns Vector4.arr swizzling (equivalent to Vector4.wxx).
        /// </summary>
        public Vector3 arr => new Vector3(w, x, x);
        
        /// <summary>
        /// Returns Vector4.wxxx swizzling.
        /// </summary>
        public Vector4 wxxx => new Vector4(w, x, x, x);
        
        /// <summary>
        /// Returns Vector4.arrr swizzling (equivalent to Vector4.wxxx).
        /// </summary>
        public Vector4 arrr => new Vector4(w, x, x, x);
        
        /// <summary>
        /// Returns Vector4.wxxy swizzling.
        /// </summary>
        public Vector4 wxxy => new Vector4(w, x, x, y);
        
        /// <summary>
        /// Returns Vector4.arrg swizzling (equivalent to Vector4.wxxy).
        /// </summary>
        public Vector4 arrg => new Vector4(w, x, x, y);
        
        /// <summary>
        /// Returns Vector4.wxxz swizzling.
        /// </summary>
        public Vector4 wxxz => new Vector4(w, x, x, z);
        
        /// <summary>
        /// Returns Vector4.arrb swizzling (equivalent to Vector4.wxxz).
        /// </summary>
        public Vector4 arrb => new Vector4(w, x, x, z);
        
        /// <summary>
        /// Returns Vector4.wxxw swizzling.
        /// </summary>
        public Vector4 wxxw => new Vector4(w, x, x, w);
        
        /// <summary>
        /// Returns Vector4.arra swizzling (equivalent to Vector4.wxxw).
        /// </summary>
        public Vector4 arra => new Vector4(w, x, x, w);
        
        /// <summary>
        /// Returns Vector4.wxy swizzling.
        /// </summary>
        public Vector3 wxy => new Vector3(w, x, y);
        
        /// <summary>
        /// Returns Vector4.arg swizzling (equivalent to Vector4.wxy).
        /// </summary>
        public Vector3 arg => new Vector3(w, x, y);
        
        /// <summary>
        /// Returns Vector4.wxyx swizzling.
        /// </summary>
        public Vector4 wxyx => new Vector4(w, x, y, x);
        
        /// <summary>
        /// Returns Vector4.argr swizzling (equivalent to Vector4.wxyx).
        /// </summary>
        public Vector4 argr => new Vector4(w, x, y, x);
        
        /// <summary>
        /// Returns Vector4.wxyy swizzling.
        /// </summary>
        public Vector4 wxyy => new Vector4(w, x, y, y);
        
        /// <summary>
        /// Returns Vector4.argg swizzling (equivalent to Vector4.wxyy).
        /// </summary>
        public Vector4 argg => new Vector4(w, x, y, y);
        
        /// <summary>
        /// Returns Vector4.wxyz swizzling.
        /// </summary>
        public Vector4 wxyz => new Vector4(w, x, y, z);
        
        /// <summary>
        /// Returns Vector4.argb swizzling (equivalent to Vector4.wxyz).
        /// </summary>
        public Vector4 argb => new Vector4(w, x, y, z);
        
        /// <summary>
        /// Returns Vector4.wxyw swizzling.
        /// </summary>
        public Vector4 wxyw => new Vector4(w, x, y, w);
        
        /// <summary>
        /// Returns Vector4.arga swizzling (equivalent to Vector4.wxyw).
        /// </summary>
        public Vector4 arga => new Vector4(w, x, y, w);
        
        /// <summary>
        /// Returns Vector4.wxz swizzling.
        /// </summary>
        public Vector3 wxz => new Vector3(w, x, z);
        
        /// <summary>
        /// Returns Vector4.arb swizzling (equivalent to Vector4.wxz).
        /// </summary>
        public Vector3 arb => new Vector3(w, x, z);
        
        /// <summary>
        /// Returns Vector4.wxzx swizzling.
        /// </summary>
        public Vector4 wxzx => new Vector4(w, x, z, x);
        
        /// <summary>
        /// Returns Vector4.arbr swizzling (equivalent to Vector4.wxzx).
        /// </summary>
        public Vector4 arbr => new Vector4(w, x, z, x);
        
        /// <summary>
        /// Returns Vector4.wxzy swizzling.
        /// </summary>
        public Vector4 wxzy => new Vector4(w, x, z, y);
        
        /// <summary>
        /// Returns Vector4.arbg swizzling (equivalent to Vector4.wxzy).
        /// </summary>
        public Vector4 arbg => new Vector4(w, x, z, y);
        
        /// <summary>
        /// Returns Vector4.wxzz swizzling.
        /// </summary>
        public Vector4 wxzz => new Vector4(w, x, z, z);
        
        /// <summary>
        /// Returns Vector4.arbb swizzling (equivalent to Vector4.wxzz).
        /// </summary>
        public Vector4 arbb => new Vector4(w, x, z, z);
        
        /// <summary>
        /// Returns Vector4.wxzw swizzling.
        /// </summary>
        public Vector4 wxzw => new Vector4(w, x, z, w);
        
        /// <summary>
        /// Returns Vector4.arba swizzling (equivalent to Vector4.wxzw).
        /// </summary>
        public Vector4 arba => new Vector4(w, x, z, w);
        
        /// <summary>
        /// Returns Vector4.wxw swizzling.
        /// </summary>
        public Vector3 wxw => new Vector3(w, x, w);
        
        /// <summary>
        /// Returns Vector4.ara swizzling (equivalent to Vector4.wxw).
        /// </summary>
        public Vector3 ara => new Vector3(w, x, w);
        
        /// <summary>
        /// Returns Vector4.wxwx swizzling.
        /// </summary>
        public Vector4 wxwx => new Vector4(w, x, w, x);
        
        /// <summary>
        /// Returns Vector4.arar swizzling (equivalent to Vector4.wxwx).
        /// </summary>
        public Vector4 arar => new Vector4(w, x, w, x);
        
        /// <summary>
        /// Returns Vector4.wxwy swizzling.
        /// </summary>
        public Vector4 wxwy => new Vector4(w, x, w, y);
        
        /// <summary>
        /// Returns Vector4.arag swizzling (equivalent to Vector4.wxwy).
        /// </summary>
        public Vector4 arag => new Vector4(w, x, w, y);
        
        /// <summary>
        /// Returns Vector4.wxwz swizzling.
        /// </summary>
        public Vector4 wxwz => new Vector4(w, x, w, z);
        
        /// <summary>
        /// Returns Vector4.arab swizzling (equivalent to Vector4.wxwz).
        /// </summary>
        public Vector4 arab => new Vector4(w, x, w, z);
        
        /// <summary>
        /// Returns Vector4.wxww swizzling.
        /// </summary>
        public Vector4 wxww => new Vector4(w, x, w, w);
        
        /// <summary>
        /// Returns Vector4.araa swizzling (equivalent to Vector4.wxww).
        /// </summary>
        public Vector4 araa => new Vector4(w, x, w, w);
        
        /// <summary>
        /// Returns Vector4.wy swizzling.
        /// </summary>
        public Vector2 wy => new Vector2(w, y);
        
        /// <summary>
        /// Returns Vector4.ag swizzling (equivalent to Vector4.wy).
        /// </summary>
        public Vector2 ag => new Vector2(w, y);
        
        /// <summary>
        /// Returns Vector4.wyx swizzling.
        /// </summary>
        public Vector3 wyx => new Vector3(w, y, x);
        
        /// <summary>
        /// Returns Vector4.agr swizzling (equivalent to Vector4.wyx).
        /// </summary>
        public Vector3 agr => new Vector3(w, y, x);
        
        /// <summary>
        /// Returns Vector4.wyxx swizzling.
        /// </summary>
        public Vector4 wyxx => new Vector4(w, y, x, x);
        
        /// <summary>
        /// Returns Vector4.agrr swizzling (equivalent to Vector4.wyxx).
        /// </summary>
        public Vector4 agrr => new Vector4(w, y, x, x);
        
        /// <summary>
        /// Returns Vector4.wyxy swizzling.
        /// </summary>
        public Vector4 wyxy => new Vector4(w, y, x, y);
        
        /// <summary>
        /// Returns Vector4.agrg swizzling (equivalent to Vector4.wyxy).
        /// </summary>
        public Vector4 agrg => new Vector4(w, y, x, y);
        
        /// <summary>
        /// Returns Vector4.wyxz swizzling.
        /// </summary>
        public Vector4 wyxz => new Vector4(w, y, x, z);
        
        /// <summary>
        /// Returns Vector4.agrb swizzling (equivalent to Vector4.wyxz).
        /// </summary>
        public Vector4 agrb => new Vector4(w, y, x, z);
        
        /// <summary>
        /// Returns Vector4.wyxw swizzling.
        /// </summary>
        public Vector4 wyxw => new Vector4(w, y, x, w);
        
        /// <summary>
        /// Returns Vector4.agra swizzling (equivalent to Vector4.wyxw).
        /// </summary>
        public Vector4 agra => new Vector4(w, y, x, w);
        
        /// <summary>
        /// Returns Vector4.wyy swizzling.
        /// </summary>
        public Vector3 wyy => new Vector3(w, y, y);
        
        /// <summary>
        /// Returns Vector4.agg swizzling (equivalent to Vector4.wyy).
        /// </summary>
        public Vector3 agg => new Vector3(w, y, y);
        
        /// <summary>
        /// Returns Vector4.wyyx swizzling.
        /// </summary>
        public Vector4 wyyx => new Vector4(w, y, y, x);
        
        /// <summary>
        /// Returns Vector4.aggr swizzling (equivalent to Vector4.wyyx).
        /// </summary>
        public Vector4 aggr => new Vector4(w, y, y, x);
        
        /// <summary>
        /// Returns Vector4.wyyy swizzling.
        /// </summary>
        public Vector4 wyyy => new Vector4(w, y, y, y);
        
        /// <summary>
        /// Returns Vector4.aggg swizzling (equivalent to Vector4.wyyy).
        /// </summary>
        public Vector4 aggg => new Vector4(w, y, y, y);
        
        /// <summary>
        /// Returns Vector4.wyyz swizzling.
        /// </summary>
        public Vector4 wyyz => new Vector4(w, y, y, z);
        
        /// <summary>
        /// Returns Vector4.aggb swizzling (equivalent to Vector4.wyyz).
        /// </summary>
        public Vector4 aggb => new Vector4(w, y, y, z);
        
        /// <summary>
        /// Returns Vector4.wyyw swizzling.
        /// </summary>
        public Vector4 wyyw => new Vector4(w, y, y, w);
        
        /// <summary>
        /// Returns Vector4.agga swizzling (equivalent to Vector4.wyyw).
        /// </summary>
        public Vector4 agga => new Vector4(w, y, y, w);
        
        /// <summary>
        /// Returns Vector4.wyz swizzling.
        /// </summary>
        public Vector3 wyz => new Vector3(w, y, z);
        
        /// <summary>
        /// Returns Vector4.agb swizzling (equivalent to Vector4.wyz).
        /// </summary>
        public Vector3 agb => new Vector3(w, y, z);
        
        /// <summary>
        /// Returns Vector4.wyzx swizzling.
        /// </summary>
        public Vector4 wyzx => new Vector4(w, y, z, x);
        
        /// <summary>
        /// Returns Vector4.agbr swizzling (equivalent to Vector4.wyzx).
        /// </summary>
        public Vector4 agbr => new Vector4(w, y, z, x);
        
        /// <summary>
        /// Returns Vector4.wyzy swizzling.
        /// </summary>
        public Vector4 wyzy => new Vector4(w, y, z, y);
        
        /// <summary>
        /// Returns Vector4.agbg swizzling (equivalent to Vector4.wyzy).
        /// </summary>
        public Vector4 agbg => new Vector4(w, y, z, y);
        
        /// <summary>
        /// Returns Vector4.wyzz swizzling.
        /// </summary>
        public Vector4 wyzz => new Vector4(w, y, z, z);
        
        /// <summary>
        /// Returns Vector4.agbb swizzling (equivalent to Vector4.wyzz).
        /// </summary>
        public Vector4 agbb => new Vector4(w, y, z, z);
        
        /// <summary>
        /// Returns Vector4.wyzw swizzling.
        /// </summary>
        public Vector4 wyzw => new Vector4(w, y, z, w);
        
        /// <summary>
        /// Returns Vector4.agba swizzling (equivalent to Vector4.wyzw).
        /// </summary>
        public Vector4 agba => new Vector4(w, y, z, w);
        
        /// <summary>
        /// Returns Vector4.wyw swizzling.
        /// </summary>
        public Vector3 wyw => new Vector3(w, y, w);
        
        /// <summary>
        /// Returns Vector4.aga swizzling (equivalent to Vector4.wyw).
        /// </summary>
        public Vector3 aga => new Vector3(w, y, w);
        
        /// <summary>
        /// Returns Vector4.wywx swizzling.
        /// </summary>
        public Vector4 wywx => new Vector4(w, y, w, x);
        
        /// <summary>
        /// Returns Vector4.agar swizzling (equivalent to Vector4.wywx).
        /// </summary>
        public Vector4 agar => new Vector4(w, y, w, x);
        
        /// <summary>
        /// Returns Vector4.wywy swizzling.
        /// </summary>
        public Vector4 wywy => new Vector4(w, y, w, y);
        
        /// <summary>
        /// Returns Vector4.agag swizzling (equivalent to Vector4.wywy).
        /// </summary>
        public Vector4 agag => new Vector4(w, y, w, y);
        
        /// <summary>
        /// Returns Vector4.wywz swizzling.
        /// </summary>
        public Vector4 wywz => new Vector4(w, y, w, z);
        
        /// <summary>
        /// Returns Vector4.agab swizzling (equivalent to Vector4.wywz).
        /// </summary>
        public Vector4 agab => new Vector4(w, y, w, z);
        
        /// <summary>
        /// Returns Vector4.wyww swizzling.
        /// </summary>
        public Vector4 wyww => new Vector4(w, y, w, w);
        
        /// <summary>
        /// Returns Vector4.agaa swizzling (equivalent to Vector4.wyww).
        /// </summary>
        public Vector4 agaa => new Vector4(w, y, w, w);
        
        /// <summary>
        /// Returns Vector4.wz swizzling.
        /// </summary>
        public Vector2 wz => new Vector2(w, z);
        
        /// <summary>
        /// Returns Vector4.ab swizzling (equivalent to Vector4.wz).
        /// </summary>
        public Vector2 ab => new Vector2(w, z);
        
        /// <summary>
        /// Returns Vector4.wzx swizzling.
        /// </summary>
        public Vector3 wzx => new Vector3(w, z, x);
        
        /// <summary>
        /// Returns Vector4.abr swizzling (equivalent to Vector4.wzx).
        /// </summary>
        public Vector3 abr => new Vector3(w, z, x);
        
        /// <summary>
        /// Returns Vector4.wzxx swizzling.
        /// </summary>
        public Vector4 wzxx => new Vector4(w, z, x, x);
        
        /// <summary>
        /// Returns Vector4.abrr swizzling (equivalent to Vector4.wzxx).
        /// </summary>
        public Vector4 abrr => new Vector4(w, z, x, x);
        
        /// <summary>
        /// Returns Vector4.wzxy swizzling.
        /// </summary>
        public Vector4 wzxy => new Vector4(w, z, x, y);
        
        /// <summary>
        /// Returns Vector4.abrg swizzling (equivalent to Vector4.wzxy).
        /// </summary>
        public Vector4 abrg => new Vector4(w, z, x, y);
        
        /// <summary>
        /// Returns Vector4.wzxz swizzling.
        /// </summary>
        public Vector4 wzxz => new Vector4(w, z, x, z);
        
        /// <summary>
        /// Returns Vector4.abrb swizzling (equivalent to Vector4.wzxz).
        /// </summary>
        public Vector4 abrb => new Vector4(w, z, x, z);
        
        /// <summary>
        /// Returns Vector4.wzxw swizzling.
        /// </summary>
        public Vector4 wzxw => new Vector4(w, z, x, w);
        
        /// <summary>
        /// Returns Vector4.abra swizzling (equivalent to Vector4.wzxw).
        /// </summary>
        public Vector4 abra => new Vector4(w, z, x, w);
        
        /// <summary>
        /// Returns Vector4.wzy swizzling.
        /// </summary>
        public Vector3 wzy => new Vector3(w, z, y);
        
        /// <summary>
        /// Returns Vector4.abg swizzling (equivalent to Vector4.wzy).
        /// </summary>
        public Vector3 abg => new Vector3(w, z, y);
        
        /// <summary>
        /// Returns Vector4.wzyx swizzling.
        /// </summary>
        public Vector4 wzyx => new Vector4(w, z, y, x);
        
        /// <summary>
        /// Returns Vector4.abgr swizzling (equivalent to Vector4.wzyx).
        /// </summary>
        public Vector4 abgr => new Vector4(w, z, y, x);
        
        /// <summary>
        /// Returns Vector4.wzyy swizzling.
        /// </summary>
        public Vector4 wzyy => new Vector4(w, z, y, y);
        
        /// <summary>
        /// Returns Vector4.abgg swizzling (equivalent to Vector4.wzyy).
        /// </summary>
        public Vector4 abgg => new Vector4(w, z, y, y);
        
        /// <summary>
        /// Returns Vector4.wzyz swizzling.
        /// </summary>
        public Vector4 wzyz => new Vector4(w, z, y, z);
        
        /// <summary>
        /// Returns Vector4.abgb swizzling (equivalent to Vector4.wzyz).
        /// </summary>
        public Vector4 abgb => new Vector4(w, z, y, z);
        
        /// <summary>
        /// Returns Vector4.wzyw swizzling.
        /// </summary>
        public Vector4 wzyw => new Vector4(w, z, y, w);
        
        /// <summary>
        /// Returns Vector4.abga swizzling (equivalent to Vector4.wzyw).
        /// </summary>
        public Vector4 abga => new Vector4(w, z, y, w);
        
        /// <summary>
        /// Returns Vector4.wzz swizzling.
        /// </summary>
        public Vector3 wzz => new Vector3(w, z, z);
        
        /// <summary>
        /// Returns Vector4.abb swizzling (equivalent to Vector4.wzz).
        /// </summary>
        public Vector3 abb => new Vector3(w, z, z);
        
        /// <summary>
        /// Returns Vector4.wzzx swizzling.
        /// </summary>
        public Vector4 wzzx => new Vector4(w, z, z, x);
        
        /// <summary>
        /// Returns Vector4.abbr swizzling (equivalent to Vector4.wzzx).
        /// </summary>
        public Vector4 abbr => new Vector4(w, z, z, x);
        
        /// <summary>
        /// Returns Vector4.wzzy swizzling.
        /// </summary>
        public Vector4 wzzy => new Vector4(w, z, z, y);
        
        /// <summary>
        /// Returns Vector4.abbg swizzling (equivalent to Vector4.wzzy).
        /// </summary>
        public Vector4 abbg => new Vector4(w, z, z, y);
        
        /// <summary>
        /// Returns Vector4.wzzz swizzling.
        /// </summary>
        public Vector4 wzzz => new Vector4(w, z, z, z);
        
        /// <summary>
        /// Returns Vector4.abbb swizzling (equivalent to Vector4.wzzz).
        /// </summary>
        public Vector4 abbb => new Vector4(w, z, z, z);
        
        /// <summary>
        /// Returns Vector4.wzzw swizzling.
        /// </summary>
        public Vector4 wzzw => new Vector4(w, z, z, w);
        
        /// <summary>
        /// Returns Vector4.abba swizzling (equivalent to Vector4.wzzw).
        /// </summary>
        public Vector4 abba => new Vector4(w, z, z, w);
        
        /// <summary>
        /// Returns Vector4.wzw swizzling.
        /// </summary>
        public Vector3 wzw => new Vector3(w, z, w);
        
        /// <summary>
        /// Returns Vector4.aba swizzling (equivalent to Vector4.wzw).
        /// </summary>
        public Vector3 aba => new Vector3(w, z, w);
        
        /// <summary>
        /// Returns Vector4.wzwx swizzling.
        /// </summary>
        public Vector4 wzwx => new Vector4(w, z, w, x);
        
        /// <summary>
        /// Returns Vector4.abar swizzling (equivalent to Vector4.wzwx).
        /// </summary>
        public Vector4 abar => new Vector4(w, z, w, x);
        
        /// <summary>
        /// Returns Vector4.wzwy swizzling.
        /// </summary>
        public Vector4 wzwy => new Vector4(w, z, w, y);
        
        /// <summary>
        /// Returns Vector4.abag swizzling (equivalent to Vector4.wzwy).
        /// </summary>
        public Vector4 abag => new Vector4(w, z, w, y);
        
        /// <summary>
        /// Returns Vector4.wzwz swizzling.
        /// </summary>
        public Vector4 wzwz => new Vector4(w, z, w, z);
        
        /// <summary>
        /// Returns Vector4.abab swizzling (equivalent to Vector4.wzwz).
        /// </summary>
        public Vector4 abab => new Vector4(w, z, w, z);
        
        /// <summary>
        /// Returns Vector4.wzww swizzling.
        /// </summary>
        public Vector4 wzww => new Vector4(w, z, w, w);
        
        /// <summary>
        /// Returns Vector4.abaa swizzling (equivalent to Vector4.wzww).
        /// </summary>
        public Vector4 abaa => new Vector4(w, z, w, w);
        
        /// <summary>
        /// Returns Vector4.ww swizzling.
        /// </summary>
        public Vector2 ww => new Vector2(w, w);
        
        /// <summary>
        /// Returns Vector4.aa swizzling (equivalent to Vector4.ww).
        /// </summary>
        public Vector2 aa => new Vector2(w, w);
        
        /// <summary>
        /// Returns Vector4.wwx swizzling.
        /// </summary>
        public Vector3 wwx => new Vector3(w, w, x);
        
        /// <summary>
        /// Returns Vector4.aar swizzling (equivalent to Vector4.wwx).
        /// </summary>
        public Vector3 aar => new Vector3(w, w, x);
        
        /// <summary>
        /// Returns Vector4.wwxx swizzling.
        /// </summary>
        public Vector4 wwxx => new Vector4(w, w, x, x);
        
        /// <summary>
        /// Returns Vector4.aarr swizzling (equivalent to Vector4.wwxx).
        /// </summary>
        public Vector4 aarr => new Vector4(w, w, x, x);
        
        /// <summary>
        /// Returns Vector4.wwxy swizzling.
        /// </summary>
        public Vector4 wwxy => new Vector4(w, w, x, y);
        
        /// <summary>
        /// Returns Vector4.aarg swizzling (equivalent to Vector4.wwxy).
        /// </summary>
        public Vector4 aarg => new Vector4(w, w, x, y);
        
        /// <summary>
        /// Returns Vector4.wwxz swizzling.
        /// </summary>
        public Vector4 wwxz => new Vector4(w, w, x, z);
        
        /// <summary>
        /// Returns Vector4.aarb swizzling (equivalent to Vector4.wwxz).
        /// </summary>
        public Vector4 aarb => new Vector4(w, w, x, z);
        
        /// <summary>
        /// Returns Vector4.wwxw swizzling.
        /// </summary>
        public Vector4 wwxw => new Vector4(w, w, x, w);
        
        /// <summary>
        /// Returns Vector4.aara swizzling (equivalent to Vector4.wwxw).
        /// </summary>
        public Vector4 aara => new Vector4(w, w, x, w);
        
        /// <summary>
        /// Returns Vector4.wwy swizzling.
        /// </summary>
        public Vector3 wwy => new Vector3(w, w, y);
        
        /// <summary>
        /// Returns Vector4.aag swizzling (equivalent to Vector4.wwy).
        /// </summary>
        public Vector3 aag => new Vector3(w, w, y);
        
        /// <summary>
        /// Returns Vector4.wwyx swizzling.
        /// </summary>
        public Vector4 wwyx => new Vector4(w, w, y, x);
        
        /// <summary>
        /// Returns Vector4.aagr swizzling (equivalent to Vector4.wwyx).
        /// </summary>
        public Vector4 aagr => new Vector4(w, w, y, x);
        
        /// <summary>
        /// Returns Vector4.wwyy swizzling.
        /// </summary>
        public Vector4 wwyy => new Vector4(w, w, y, y);
        
        /// <summary>
        /// Returns Vector4.aagg swizzling (equivalent to Vector4.wwyy).
        /// </summary>
        public Vector4 aagg => new Vector4(w, w, y, y);
        
        /// <summary>
        /// Returns Vector4.wwyz swizzling.
        /// </summary>
        public Vector4 wwyz => new Vector4(w, w, y, z);
        
        /// <summary>
        /// Returns Vector4.aagb swizzling (equivalent to Vector4.wwyz).
        /// </summary>
        public Vector4 aagb => new Vector4(w, w, y, z);
        
        /// <summary>
        /// Returns Vector4.wwyw swizzling.
        /// </summary>
        public Vector4 wwyw => new Vector4(w, w, y, w);
        
        /// <summary>
        /// Returns Vector4.aaga swizzling (equivalent to Vector4.wwyw).
        /// </summary>
        public Vector4 aaga => new Vector4(w, w, y, w);
        
        /// <summary>
        /// Returns Vector4.wwz swizzling.
        /// </summary>
        public Vector3 wwz => new Vector3(w, w, z);
        
        /// <summary>
        /// Returns Vector4.aab swizzling (equivalent to Vector4.wwz).
        /// </summary>
        public Vector3 aab => new Vector3(w, w, z);
        
        /// <summary>
        /// Returns Vector4.wwzx swizzling.
        /// </summary>
        public Vector4 wwzx => new Vector4(w, w, z, x);
        
        /// <summary>
        /// Returns Vector4.aabr swizzling (equivalent to Vector4.wwzx).
        /// </summary>
        public Vector4 aabr => new Vector4(w, w, z, x);
        
        /// <summary>
        /// Returns Vector4.wwzy swizzling.
        /// </summary>
        public Vector4 wwzy => new Vector4(w, w, z, y);
        
        /// <summary>
        /// Returns Vector4.aabg swizzling (equivalent to Vector4.wwzy).
        /// </summary>
        public Vector4 aabg => new Vector4(w, w, z, y);
        
        /// <summary>
        /// Returns Vector4.wwzz swizzling.
        /// </summary>
        public Vector4 wwzz => new Vector4(w, w, z, z);
        
        /// <summary>
        /// Returns Vector4.aabb swizzling (equivalent to Vector4.wwzz).
        /// </summary>
        public Vector4 aabb => new Vector4(w, w, z, z);
        
        /// <summary>
        /// Returns Vector4.wwzw swizzling.
        /// </summary>
        public Vector4 wwzw => new Vector4(w, w, z, w);
        
        /// <summary>
        /// Returns Vector4.aaba swizzling (equivalent to Vector4.wwzw).
        /// </summary>
        public Vector4 aaba => new Vector4(w, w, z, w);
        
        /// <summary>
        /// Returns Vector4.www swizzling.
        /// </summary>
        public Vector3 www => new Vector3(w, w, w);
        
        /// <summary>
        /// Returns Vector4.aaa swizzling (equivalent to Vector4.www).
        /// </summary>
        public Vector3 aaa => new Vector3(w, w, w);
        
        /// <summary>
        /// Returns Vector4.wwwx swizzling.
        /// </summary>
        public Vector4 wwwx => new Vector4(w, w, w, x);
        
        /// <summary>
        /// Returns Vector4.aaar swizzling (equivalent to Vector4.wwwx).
        /// </summary>
        public Vector4 aaar => new Vector4(w, w, w, x);
        
        /// <summary>
        /// Returns Vector4.wwwy swizzling.
        /// </summary>
        public Vector4 wwwy => new Vector4(w, w, w, y);
        
        /// <summary>
        /// Returns Vector4.aaag swizzling (equivalent to Vector4.wwwy).
        /// </summary>
        public Vector4 aaag => new Vector4(w, w, w, y);
        
        /// <summary>
        /// Returns Vector4.wwwz swizzling.
        /// </summary>
        public Vector4 wwwz => new Vector4(w, w, w, z);
        
        /// <summary>
        /// Returns Vector4.aaab swizzling (equivalent to Vector4.wwwz).
        /// </summary>
        public Vector4 aaab => new Vector4(w, w, w, z);
        
        /// <summary>
        /// Returns Vector4.wwww swizzling.
        /// </summary>
        public Vector4 wwww => new Vector4(w, w, w, w);
        
        /// <summary>
        /// Returns Vector4.aaaa swizzling (equivalent to Vector4.wwww).
        /// </summary>
        public Vector4 aaaa => new Vector4(w, w, w, w);

        #endregion

    }
}
