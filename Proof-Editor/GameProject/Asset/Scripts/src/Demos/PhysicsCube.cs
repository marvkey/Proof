using Proof;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Demos
{
    public enum ErrorCode : int
    {
        None = 0,
        Unknown = 2,
        ConnectionLost = 100,
        OutlierReading = 200
    }

    public enum ErrorCodeafds : ulong
    {
        None = 0,
        Unknown = 2,
        ConnectionLost = 100,
        OutlierReading = 200
    }
    class PhysicsCube : Entity
    {
      
        public ErrorCode code;
    }
}
