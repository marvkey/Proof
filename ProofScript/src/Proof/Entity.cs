using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Proof
{
   public class Entity
    {
		protected Entity() { ID = 0; }

		internal Entity(ulong id)
		{
			ID = id;
			Console.WriteLine("Entity Created");
		}
		public readonly ulong ID;
	}
}
