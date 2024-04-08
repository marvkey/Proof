using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using Proof;

namespace Proof
{
    public static class Random
    {
        private static System.Random rand = new System.Random();
        public static float Float(float min, float max)
        {
            return InternalCalls.Random_RandomFloat(min, max);  
        }

        public static double Double(double min, double max)
        {
            return InternalCalls.Random_RandomDouble(min, max); 
        }

        public static int Int(int min, int max)
        {
            return InternalCalls.Random_RandomInt32(min, max);
        }

        public static Proof.Vector3 Vector(float min, float max)
        {
            Proof.Vector3 Vector ;
            Vector.x=Float(min,max);
            Vector.y=Float(min,max);
            Vector.z = Float(min,max);

            return Vector;
        }
        public static bool Bool(){
            return InternalCalls.Random_RandomBool();
        }
    }
}
