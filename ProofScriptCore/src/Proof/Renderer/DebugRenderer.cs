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


    public static class ImmediateRenderer
    {
        public static void DrawMesh(StaticMesh mesh, Material material,TransferMatrix4 transform, bool castShadow)
        {
            InternalCalls.ImmediateRenderer_SubmitMesh(mesh.ID.ToUInt64(), material.ID.ToUInt64(), ref transform, castShadow);
        }

        public static void DrawDynamicMesh(DynamicMesh mesh, Material material, uint subMeshIndex,TransferMatrix4 transform, bool castShadow)
        {
            InternalCalls.ImmediateRenderer_SubmitDynamicMesh(mesh.ID, material.ID, subMeshIndex,ref transform, castShadow);
        }

        public static void SubmitMeshComponent(MeshComponent meshComponent,Material material,TransferMatrix4 transform,bool castShadow)
        {
            if(meshComponent.Mesh != null)
                DrawMesh(meshComponent.Mesh, material,transform,false);
        }

        public static void SubmitMeshComponent(MeshComponent meshComponent,Material material)
        {
            SubmitMeshComponent(meshComponent, material,meshComponent.Entity.WorldTransformMatrix,true);
        }

        public static void SubmitDynamicMeshComponent(DynamicMeshComponent dynamicMeshComponent,Material material,TransferMatrix4 transform,bool castShadow)
        {
            if(dynamicMeshComponent.Mesh != null)
                DrawDynamicMesh(dynamicMeshComponent.Mesh,material,dynamicMeshComponent.SubMeshIndex,transform,castShadow);
        }

        public static void SubmitDynamicMeshComponent(DynamicMeshComponent dynamicMeshComponent,Material material)
        {
            DrawDynamicMesh(dynamicMeshComponent.Mesh,material,dynamicMeshComponent.SubMeshIndex,dynamicMeshComponent.Entity.WorldTransformMatrix,true);
        }
    }
}
