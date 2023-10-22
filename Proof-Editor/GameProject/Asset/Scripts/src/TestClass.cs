using Proof;
using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Game
{
    public enum TestState
    {
        None = 0,
        Practice,
        Guess,
    }
    class TestClass : Entity
    {
        void OnCreate()
        {
            Log.Trace("we are running ");
        }
        private int age = 10;
        public bool Available = true;
        public TestState statejadshlajdla = (TestState)12;
        public Material ChangeMaterial;

        [ShowInEditorAttribute("ue;;;pp",true)]
        //[ToolTipAttribute("Just a test")]
        public SByte testint8;
        public Int16 testint16;
        public Int32 testint32;
        public Int64 testint64;

        public Byte  testUint8;
        public UInt16 testUint16;
        public UInt32 testUint32;
        public UInt64 testUint64;

        [ClampValueAttribute(-10,10)]
        public float testFloat;
        public double testDouble;

        public Vector2 COlorChange2;
        public Vector3 COlorChange3;
        public Vector4 COlorChange;
    }
}
