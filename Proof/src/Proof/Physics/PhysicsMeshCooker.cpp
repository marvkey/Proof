#include "Proofprch.h"
#include "PhysicsEngine.h"

#include "PhysicsMeshCooker.h"
#include "Proof/Scene/Mesh.h"

#include "Proof/Renderer/MeshWorkShop.h"
#include "Proof/Renderer/Vertex.h"

namespace  Proof {
	std::unordered_map<AssetID, Count<class Mesh>> s_Meshes;
	std::unordered_map<AssetID, physx::PxTriangleMesh*> s_ConvexMeshes;
	Count<Mesh> PhysicsDebugCube;
	Count<Mesh> PhysicsDebugSphere;
	Count<Mesh> PhysicsDebugCapsule;
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
		if (mesh == nullptr)
			return;
		std::vector<physx::PxVec3> vertices;
		std::vector<uint32_t> indices;
		uint32_t index = 0;
		for (const SubMesh& subMesh : mesh->GetMeshSource()->GetSubMeshes())
		{
			//if (mesh->IsMeshExcluded(index))continue;
			//for (const auto& vertex : subMesh.Vertices)
			//	vertices.emplace_back(PhysxUtils::VectorToPhysxVector(vertex.Vertices));
			//for (const auto& val : subMesh.Indices)
			//	indices.emplace_back(val);
			//index++;
		}
		/*
		{
			physx::PxConvexMeshDesc convexDesc;
			convexDesc.points.count = vertices.size();
			convexDesc.points.stride = sizeof(physx::PxVec3);
			convexDesc.points.data = vertices.data();
			convexDesc.flags = physx::PxConvexFlag::eCOMPUTE_CONVEX;

			//convexDesc.indices.count = indices.size();
			//convexDesc.indices.stride = sizeof(uint32_t);
			//convexDesc.indices.data = indices.data();

			physx::PxDefaultMemoryOutputStream buf;
			physx::PxConvexMeshCookingResult::Enum result;
			if (!s_MeshCooker->cookConvexMesh(convexDesc, buf, &result))
			{
				switch (result)
				{
					case physx::PxConvexMeshCookingResult::eSUCCESS:
						break;
					case physx::PxConvexMeshCookingResult::ePOLYGONS_LIMIT_REACHED:
						PF_EC_ERROR("Convex Mesh plygons limit reached");
						break;
					case physx::PxConvexMeshCookingResult::eZERO_AREA_TEST_FAILED:
						PF_EC_ERROR("Zero area test failed");
						break;
					default:
						break;
				}
			}
			physx::PxDefaultMemoryInputData input(buf.getData(), buf.getSize());
			physx::PxConvexMesh* convexMesh = PhysicsEngine::GetPhysics()->createConvexMesh(input);
			s_ConvexMeshes[ID] = convexMesh;
			std::vector<Vertex> meshVertices;
			std::vector<uint32_t> indexVector;
			{

				auto vertices = convexMesh->getVertices();
				for (uint32_t vertexPosIndex = 0; vertexPosIndex < convexMesh->getNbVertices(); vertexPosIndex++)
				{
					auto pos = vertices[vertexPosIndex];
					Vertex vertex;
					vertex.Vertices = Vector{ pos.x,pos.y,pos.z };
					meshVertices.emplace_back(vertex);
				}
				uint32_t indexCount;
				indexVector.resize(convexMesh->getNbPolygons() * 3);
				for (uint32_t indexPos = 0; indexPos < convexMesh->getNbPolygons(); indexPos++)
				{
					physx::PxHullPolygon poly;
					convexMesh->getPolygonData(indexPos, poly);
					for (uint32_t j = 0; j < poly.mNbVerts - 2; j++)
					{
						indexVector[indexCount++] = poly.mIndexBase;
						indexVector[indexCount++] = poly.mIndexBase + j + 1;
						indexVector[indexCount++] = poly.mIndexBase + j + 2;
					}
				}
			}
			Count<Mesh> newMesh = Count<Mesh>::Create(AssetManager::GetAsset<Mesh>(ID)->GetName(),
				meshVertices, indexVector);

			s_Meshes[mesh->GetID()] = newMesh;
			
		}
		return;
		*/
		{
			physx::PxTriangleMeshDesc meshDesc;
			meshDesc.points.count = vertices.size();
			meshDesc.points.stride = sizeof(physx::PxVec3);
			meshDesc.points.data = vertices.data();

			meshDesc.triangles.count = indices.size();
			meshDesc.triangles.stride = 3 * sizeof(uint32_t);
			meshDesc.triangles.data = indices.data();

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
						break;
					case physx::PxConvexMeshCookingResult::eFAILURE:
						PF_EC_ERROR("Physx mesh cooking failed");
						break;
				}
			}
			physx::PxDefaultMemoryInputData input(buf.getData(), buf.getSize());
			physx::PxTriangleMesh* triangelMesh = PhysicsEngine::GetPhysics()->createTriangleMesh(input);
			s_ConvexMeshes[ID] = triangelMesh;
			std::vector<Vertex> meshVertices;
			std::vector<uint32_t> meshIndices;
			{
			
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
					meshIndices.emplace_back(indices[indexPos]);
				}
			}
			Count<Mesh> newMesh = Count<Mesh>::Create(AssetManager::GetAsset<Mesh>(ID)->GetName(),
				meshVertices, meshIndices);

			s_Meshes[mesh->GetID()] = newMesh;
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
		if(!PhysicsDebugCube)
			PhysicsDebugCube = MeshWorkShop::GenerateCube();
		return PhysicsDebugCube;
	}
	Count<class Mesh> PhysicsMeshCooker::GetCapsuleColliderMesh()
	{
		if(!PhysicsDebugCapsule)
			PhysicsDebugCapsule = MeshWorkShop::GenerateCapsule();
		return PhysicsDebugCapsule;

	}
	Count<class Mesh> PhysicsMeshCooker::GetSphereColliderMesh()
	{
		if(!PhysicsDebugSphere)
			PhysicsDebugSphere = MeshWorkShop::GenerateSphere();
		return PhysicsDebugSphere;
	}

	void PhysicsMeshCooker::Init()
	{
		physx::PxCookingParams params = physx::PxCookingParams(PhysicsEngine::GetPhysics()->getTolerancesScale());
		params.meshWeldTolerance = 0.1f;
		params.buildTriangleAdjacencies = true;
		// disable mesh cleaning - perform mesh validation on development configurations
		//params.meshPreprocessParams |= physx::PxMeshPreprocessingFlag::eDISABLE_CLEAN_MESH;
		// disable edge precompute, edges are set for each triangle, slows contact generation
		//params.meshPreprocessParams |= physx::PxMeshPreprocessingFlag::eDISABLE_ACTIVE_EDGES_PRECOMPUTE;


		s_MeshCooker = PxCreateCooking(PX_PHYSICS_VERSION, *PhysicsEngine::GetFoundation(), params);
		PF_CORE_ASSERT(s_MeshCooker, "Physics failed to create MeshCooker failed!");

	}
	void PhysicsMeshCooker::Release()
	{
		s_MeshCooker->release();
		PhysicsDebugSphere = nullptr;
		PhysicsDebugCapsule = nullptr;
		PhysicsDebugCube = nullptr;
	}
}