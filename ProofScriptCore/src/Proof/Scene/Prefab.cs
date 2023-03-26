using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;

namespace Proof
{
    public class Prefab
    {
        protected Prefab() { ID = 0; }

        internal Prefab(ulong id)
        {
            ID = id;
        }
        //Asset ID
        public readonly ulong ID;
    }
}
