using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Proof
{
    [RegisterCoreClassStruct]
    [AttributeUsage(AttributeTargets.Field)]
    public class ClampValueAttribute : Attribute
    {
        public readonly float Min;
        public readonly float Max;

        public ClampValueAttribute(float min, float max)
        {
            Min = min;
            Max = max;
        }
    }
    [RegisterCoreClassStruct]
    [AttributeUsage(AttributeTargets.Field)]
    public class MaxAttribute : Attribute
    {
        public readonly float Max;

        public MaxAttribute(float max)
        {
            Max = max;
        }
    }
    [RegisterCoreClassStruct]
    [AttributeUsage(AttributeTargets.Field)]
    public class MinAttribute : Attribute
    {
        public readonly float Min;

        public MinAttribute(float min)
        {
            Min = min;
        }
    }
}
