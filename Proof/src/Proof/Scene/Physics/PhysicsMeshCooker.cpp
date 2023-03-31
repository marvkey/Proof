#include "Proofprch.h"
#include "PhysicsEngine.h"

#include "PhysicsMeshCooker.h"
#include "Proof/Scene/Mesh.h"

#include "Proof/Renderer/MeshWorkShop.h"

namespace  Proof {
	std::unordered_map<AssetID, Count<class Mesh>> s_Meshes;
	std::unordered_map<AssetID, physx::PxTriangleMesh*> s_ConvexMeshes;
	static class physx::PxCooking* s_MeshCooker = nullptr;
	bool PhysicsMeshCooker::HasMesh(AssetID ID)
	{
		return s_Meshes.contains(ID);
	}
	const Count<class Mesh> PhysicsMeshCooker::GetConvexMeshAsMesh(AssetID ID)
	{
		return s_Meshes.at(ID);
	}

	physx::PxTriangleMesh* PhysicsMeshCooker::GetConvexMesh(AssetID ID)
	{
		return s_ConvexMeshes.at(ID);
	}
	void PhysicsMeshCooker::CookMesh(AssetID ID)
	{
		PF_CORE_ASSERT(AssetManager::HasAsset(ID), "Asset Manager does not have asset");

		Count<Mesh> mesh = AssetManager::GetAsset<Mesh>(ID);
		if (mesh)
		{
			std::vector<physx::PxVec3> vertices;
			std::vector<uint32_t> indices;
			uint32_t index = 0;
			for (const SubMesh& subMesh : mesh->GetMeshSource()->GetSubMeshes())
			{
				if (mesh->IsMeshExcluded(index))continue;
				for (const auto& vertex : subMesh.Vertices)
					vertices.emplace_back(PhysxUtils::VectorToPhysxVector(vertex.Vertices));
				for (const auto& val : subMesh.Indices)
					indices.emplace_back(val);
				index++;
			}
		
			physx::PxTriangleMeshDesc meshDesc;
			meshDesc.points.count = vertices.size();
			meshDesc.points.stride = sizeof(physx::PxVec3);
			meshDesc.points.data = vertices.data();

			meshDesc.triangles.count = indices.size();
			meshDesc.triangles.stride = 3 * sizeof(uint32_t);
			meshDesc.triangles.data = indices.data();

			//convexDesc.flags |= physx::PxConvexFlag::ePLANE_SHIFTING;

			physx::PxDefaultMemoryOutputStream buf;
			physx::PxTriangleMeshCookingResult::Enum error;
			if (!s_MeshCooker->cookTriangleMesh(meshDesc, buf, &error))
			{
				switch (error)
				{
					case physx::PxTriangleMeshCookingResult::eSUCCESS:
						break;
					case physx::PxTriangleMeshCookingResult::eLARGE_TRIANGLE:
						PF_EC_ERROR("Large traingle convex triangel mesh cooking");
						//PF_EC_ERROR("Physx mesh cooking failed,couldn't find 4 initial vertices without a small triangle");
						break;
					//case physx::PxTriangleMeshCookingResult::e:
					//	PF_EC_ERROR("Physx mesh cooking failed,has reached the 255 polygons limit,Try to simplify the input vertices or try to use the eINFLATE_CONVEX or the eQUANTIZE_INPUT flags");
					//	break;
					case physx::PxConvexMeshCookingResult::eFAILURE:
						PF_EC_ERROR("Physx mesh cooking failed");
						break;
				}
			}
			physx::PxDefaultMemoryInputData input(buf.getData(), buf.getSize());
			physx::PxTriangleMesh* triangelMesh = PhysicsEngine::GetPhysics()->createTriangleMesh(input);
			s_ConvexMeshes[ID] = triangelMesh;

			{
				std::vector<Vertex> meshVertices;
				std::vector<uint32_t> meshIndices;
				auto vertices = triangelMesh->getVertices();
				uint32_t* indices =(uint32_t*) triangelMesh->getTriangles();
				for (uint32_t vertexPosIndex = 0; vertexPosIndex < triangelMesh->getNbVertices(); vertexPosIndex++)
				{
					auto pos = vertices[vertexPosIndex];
					Vertex vertex;
					vertex.Vertices = Vector{ pos.x,pos.y,pos.z };
					meshVertices.emplace_back(vertex);
				}
				for (uint32_t indexPos = 0; indexPos < triangelMesh->getNbTriangles()*3; indexPos++)
				{
					meshVertices.emplace_back(indices[indexPos]);
				}
				//Count<Mesh> mesh = CreateCount<Mesh>(AssetManager::GetAsset<MeshAsset>(ID)->GetName(),
				//	meshVertices, meshIndices);
			}
		}
	}

	void PhysicsMeshCooker::DeleteMesh(AssetID ID)
	{
		PF_CORE_ASSERT(HasMesh(ID), "Mesh does not exist");

		auto cooker = s_ConvexMeshes.at(ID);
		cooker->release();
		s_ConvexMeshes.erase(ID);
		s_Meshes.erase(ID);
	}
	Count<class Mesh> PhysicsMeshCooker::GetCubeColliderMesh()
	{
		static auto mesh = MeshWorkShop::GenerateCube();
		return mesh;
	}
	Count<class Mesh> PhysicsMeshCooker::GetCapsuleColliderMesh()
	{
		static auto capsule = MeshWorkShop::GenerateCapsule();
		return capsule;

	}
	Count<class Mesh> PhysicsMeshCooker::GetSphereColliderMesh()
	{
		static auto sphere = MeshWorkShop::GenerateUVSphere();
		return sphere;
	}

	void PhysicsMeshCooker::Init()
	{
		s_MeshCooker = PxCreateCooking(PX_PHYSICS_VERSION, *PhysicsEngine::GetFoundation(), physx::PxCookingParams(PhysicsEngine::GetPhysics()->getTolerancesScale()));
		PF_CORE_ASSERT(s_MeshCooker, "Physics failed to create MeshCooker failed!");

	}
	void PhysicsMeshCooker::Release()
	{
		s_MeshCooker->release();
	}
}