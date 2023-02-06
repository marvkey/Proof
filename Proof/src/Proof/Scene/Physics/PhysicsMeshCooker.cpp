#include "Proofprch.h"
#include "PhysicsEngine.h"

#include "PhysicsMeshCooker.h"
#include "Proof/Scene/Mesh.h"
#include"Proof/Asset/MeshAsset.h"

namespace  Proof {
	std::unordered_map<AssetID, Count<class Mesh>> s_Meshes;
	std::unordered_map<AssetID, physx::PxConvexMesh*> s_ConvexMeshes;
	static class physx::PxCooking* s_MeshCooker = nullptr;
	bool PhysicsMeshCooker::HasMesh(AssetID ID)
	{
		return s_Meshes.contains(ID);
	}
	const Count<class Mesh> PhysicsMeshCooker::GetConvexMeshAsMesh(AssetID ID)
	{
		return s_Meshes.at(ID);
	}

	physx::PxConvexMesh* PhysicsMeshCooker::GetConvexMesh(AssetID ID)
	{
		return s_ConvexMeshes.at(ID);
	}
	void PhysicsMeshCooker::CookMesh(AssetID ID)
	{
		PF_CORE_ASSERT(AssetManager::HasID(ID), "Asset Manager does not have asset");

		Mesh* mesh = AssetManager::GetAsset<MeshAsset>(ID)->GetMesh();

		if (mesh)
		{
			std::vector<physx::PxVec3> vertices;
			std::vector<uint32_t> indices;

			for (const SubMesh& subMesh : mesh->GetSubMeshes())
			{
				for (const auto& vertex : subMesh.m_Vertices)
					vertices.emplace_back(physx::PxVec3{ vertex.Vertices.X,vertex.Vertices.Y,vertex.Vertices.Z });
				for (const auto& val : subMesh.m_Indices)
					indices.emplace_back(val);
			}

			physx::PxConvexMeshDesc convexDesc;
			convexDesc.points.count = vertices.size();
			convexDesc.points.stride = sizeof(physx::PxVec3);
			convexDesc.points.data = vertices.data();

			convexDesc.indices.count = indices.size();
			convexDesc.indices.stride = sizeof(uint32_t);
			convexDesc.indices.data = indices.data();
			convexDesc.flags = physx::PxConvexFlag::eCOMPUTE_CONVEX;

			convexDesc.flags |= physx::PxConvexFlag::ePLANE_SHIFTING;

			physx::PxDefaultMemoryOutputStream buf;
			physx::PxConvexMeshCookingResult::Enum error;
			if (!s_MeshCooker->cookConvexMesh(convexDesc, buf, &error))
			{
				switch (error)
				{
					case physx::PxConvexMeshCookingResult::eSUCCESS:
						break;
					case physx::PxConvexMeshCookingResult::eZERO_AREA_TEST_FAILED:
						PF_EC_ERROR("Physx mesh cooking failed,couldn't find 4 initial vertices without a small triangle");
						break;
					case physx::PxConvexMeshCookingResult::ePOLYGONS_LIMIT_REACHED:
						PF_EC_ERROR("Physx mesh cooking failed,has reached the 255 polygons limit,Try to simplify the input vertices or try to use the eINFLATE_CONVEX or the eQUANTIZE_INPUT flags");
						break;
					case physx::PxConvexMeshCookingResult::eFAILURE:
						PF_EC_ERROR("Physx mesh cooking failed");
						break;
				}
			}
			physx::PxDefaultMemoryInputData input(buf.getData(), buf.getSize());
			physx::PxConvexMesh* convexMesh = PhysicsEngine::GetPhysics()->createConvexMesh(input);
			s_ConvexMeshes[ID] = convexMesh;

			{
				std::vector<Vertex> meshVertices;
				std::vector<uint32_t> meshIndices;
				auto vertices = convexMesh->getVertices();
				auto indices = convexMesh->getIndexBuffer();
				for (uint32_t vertexPosIndex = 0; vertexPosIndex < convexMesh->getNbVertices(); vertexPosIndex++)
				{
					auto pos = vertices[vertexPosIndex];
					Vertex vertex;
					vertex.Vertices = Vector{ pos.x,pos.y,pos.z };
					meshVertices.emplace_back(vertex);
				}
				for (uint32_t indexPos = 0; indexPos < PF_ARRAYSIZE(indices); indexPos++)
				{
					meshVertices.emplace_back(indices[indexPos]);
				}
				Count<Mesh> mesh = CreateCount<Mesh>(AssetManager::GetAsset<MeshAsset>(ID)->GetName(),
					meshVertices, meshIndices);
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