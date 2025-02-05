using Proof;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Proof
{
    //[RegisterCoreClassStruct]
    public static class DebugRenderer
    {

        public static void DrawLine(Vector3 p0, Vector3 p1, Vector4 color)
        {
            InternalCalls.DebugRenderer_DrawLine(ref p0, ref p1, ref color);
        }

        public static void DrawRay(Vector3 origin, Vector3 direction, float length,Vector4 color)
        {
            InternalCalls.DebugRenderer_DrawRayLength(ref origin, ref direction, ref length,ref color);
        }
        public static void DrawRay(Vector3 origin, Vector3 direction, Vector4 color)
        {
            InternalCalls.DebugRenderer_DrawRay(ref origin, ref direction, ref color);
        }

        public static void DrawQuadBillboard(Vector3 translation, Vector2 size, Vector4 color)
        {
            InternalCalls.DebugRenderer_DrawQuadBillboard(ref translation, ref size, ref color);
        }

        public static float LineWidth { set => InternalCalls.DebugRenderer_SetLineWidth(value); }

    }
}
