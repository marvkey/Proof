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
            return (float)rand.NextDouble() * (max - min) + min;// between min and max
        }

        public static double Double(double min, double max)
        {
            return rand.NextDouble() * (max - min) + min;
        }

        public static int Int(int min, int max)
        {
            return rand.Next(min, max);
        }

        public static bool Bool(){
            return Convert.ToBoolean(rand.Next(0,1));
        }
    }
}
