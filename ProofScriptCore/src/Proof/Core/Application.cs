using Proof;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Proof
{

    public static class Application
    {
        public static void Shutdown()
        {
            InternalCalls.Application_Shutdown();
        }
        public static float GetFPS()
        {
            return InternalCalls.Application_GetFPS();
        }
    }
}
