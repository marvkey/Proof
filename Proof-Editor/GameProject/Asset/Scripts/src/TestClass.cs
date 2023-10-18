using Proof;
using System;
using System.Collections.Generic;
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
        public int age = 10;
        public bool Available = true;
        public TestState statejadshlajdla = (TestState)12;
    }
}
