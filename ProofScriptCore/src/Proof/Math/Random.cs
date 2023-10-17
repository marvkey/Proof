using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

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

        public static Vector3 Vector3(float min, float max)
        {
            Vector3 vec ;
            vec.X=Float(min,max);
            vec.Y=Float(min,max);
            vec.Z = Float(min,max);

            return vec;
        }
        public static bool Bool(){
            return InternalCalls.Random_RandomBool();
        }
    }
}
