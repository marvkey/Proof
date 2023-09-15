using Proof;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Demos
{
    public enum TestLevel: int
    {
        Low = 0,
        Medium = 1,
        High = 100,
    }

    
    class PhysicsCube : Entity
    {
      
        public TestLevel Level;
        public Prefab SpawnObject;
    }
}
