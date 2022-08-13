using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Proof
{
    static class Random
    {
        public static float Float(float min, float max)
        {
         return InternalCalls.Random_Float(min, max);
        }
    }
}
