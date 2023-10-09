#include "Proofprch.h"
#include "PhysicsEngine.h"

#include "PhysicsMeshCooker.h"
#include "Proof/Scene/Mesh.h"

#include "Proof/Renderer/MeshWorkShop.h"
#include "Proof/Renderer/Vertex.h"
#include "Proof/Core/Application.h"
#include "Proof/Project/Project.h"
#include "MeshCollider.h"
#include "Proof/Core/Buffer.h"
namespace  Proof 
{

	namespace Utils {

		static std::filesystem::path GetCacheDirectory()
		{
			return Application::Get()->GetProject()->GetCacheDirectory() / "PhysicsColliders";
		}

		static void CreateCacheDirectoryIfNeeded()
		{
			std::filesystem::path cacheDirectory = GetCacheDirectory();
			if (!std::filesystem::exists(cacheDirectory))
				std::filesystem::create_directories(cacheDirectory);
		}
	}
	struct CookingData
	{
		physx::PxCooking* CookingSDK;
		physx::PxCookingParams DefaultCookingParameters;

		CookingData(const physx::PxTolerancesScale& scale)
			: CookingSDK(nullptr), DefaultCookingParameters(scale)
		{
		}
	};
	struct AntPhysicsMesh
	{
		const char Header[11] = "ProofMCRaw";
		MeshColliderType Type;
		uint32_t SubmeshCount;
	};

	Count<Mesh> PhysicsDebugCube;
	Count<Mesh> PhysicsDebugSphere;
	Count<Mesh> PhysicsDebugCapsule;
	static CookingData* s_CookingData = nullptr;

	bool PhysicsMeshCooker::HasMesh(AssetID ID)
	{
		return false;
	}
	const Count<class Mesh> PhysicsMeshCooker::GetConvexMeshAsMesh(AssetID ID)
	{
		return nullptr;
	}

	physx::PxTriangleMesh* PhysicsMeshCooker::GetConvexMesh(AssetID ID)
	{
		return nullptr;
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

		/*
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
					vertex.Position = glm::vec3{ pos.x,pos.y,pos.z };
					meshVertices.emplace_back(vertex);
				}
				for (uint32_t indexPos = 0; indexPos < triangelMesh->getNbTriangles()*3; indexPos++)
				{
					meshIndices.emplace_back(indices[indexPos]);
				}
			}
			Count<Mesh> newMesh = Count<Mesh>::Create(AssetManager::GetAsset<Mesh>(ID)->GetName(),
				meshVertices, meshIndices);

			//s_Meshes[mesh->GetID()] = newMesh;
		}
		*/
	}

	void PhysicsMeshCooker::DeleteMesh(AssetID ID)
	{
		PF_CORE_ASSERT(HasMesh(ID), "Mesh does not exist");

		//auto cooker = s_ConvexMeshes.at(ID);
		//cooker->release();
		//s_ConvexMeshes.erase(ID);
		//s_Meshes.erase(ID);
	}
	Count<class Mesh> PhysicsMeshCooker::GetBoxColliderMesh()
	{
		if (!PhysicsDebugCube)
		{
			PhysicsDebugCube = MeshWorkShop::GenerateCube(glm::vec3{0.5});
			AssetManager::CreateRuntimeAsset(AssetManager::CreateID(), PhysicsDebugCube, "PhysicsDebugCube");
		}
		return PhysicsDebugCube;
	}
	Count<class Mesh> PhysicsMeshCooker::GetCapsuleColliderMesh()
	{
		if (!PhysicsDebugCapsule)
		{
			PhysicsDebugCapsule = MeshWorkShop::GenerateCapsule(0.5);
			AssetManager::CreateRuntimeAsset(AssetManager::CreateID(), PhysicsDebugCapsule, "PhysicsDebugCapsule");
		}
		return PhysicsDebugCapsule;

	}
	Count<class Mesh> PhysicsMeshCooker::GetSphereColliderMesh()
	{
		if (!PhysicsDebugSphere)
		{
			PhysicsDebugSphere = MeshWorkShop::GenerateSphere();
			AssetManager::CreateRuntimeAsset(AssetManager::CreateID(), PhysicsDebugSphere, "PhysicsDebugSphere");
		}
		return PhysicsDebugSphere;
	}

	void PhysicsMeshCooker::Init()
	{
		s_CookingData = new CookingData(PhysicsEngine::GetPhysics()->getTolerancesScale());
		s_CookingData->DefaultCookingParameters.midphaseDesc = physx::PxMeshMidPhase::eBVH34;

		s_CookingData->CookingSDK = PxCreateCooking(PX_PHYSICS_VERSION, *PhysicsEngine::GetFoundation(), s_CookingData->DefaultCookingParameters);
		PF_CORE_ASSERT(s_CookingData->CookingSDK, "Couldn't initialize PhysX Cooking SDK!");
	}
	void PhysicsMeshCooker::Release()
	{
		s_CookingData->CookingSDK->release();
		s_CookingData->CookingSDK = nullptr;
		delete s_CookingData;

		PhysicsDebugSphere = nullptr;
		PhysicsDebugCapsule = nullptr;
		PhysicsDebugCube = nullptr;
	}
	CookingResult PhysicsMeshCooker::CookConvexMesh(const Count<MeshCollider>& colliderAsset, const Count<class MeshSource>& meshSource, const std::vector<uint32_t>& submeshIndices, MeshColliderData& outData)
	{
		physx::PxConvexMeshDesc convexDesc;
		convexDesc.flags = physx::PxConvexFlag::eCOMPUTE_CONVEX;

		// Update cooking parameters
		physx::PxCookingParams cookingParams(s_CookingData->DefaultCookingParameters);
		if (colliderAsset->EnableVertexWelding)
		{
			cookingParams.meshPreprocessParams = physx::PxMeshPreprocessingFlag::eWELD_VERTICES;
			cookingParams.meshWeldTolerance = colliderAsset->VertexWeldTolerance;
		}
		if (colliderAsset->CheckZeroAreaTriangles)
		{
			convexDesc.flags |= physx::PxConvexFlag::eCHECK_ZERO_AREA_TRIANGLES;
			float zeroAreaTriangleThreshold = colliderAsset->AreaTestEpsilon;

			if (zeroAreaTriangleThreshold <= 0.0f)
			{
				PF_EC_WARN("Attempting to cook convex mesh collider with a zero-area threshold of {}! Zero-Area Threshold has to be greater than 0! Using 0.01 instead.", zeroAreaTriangleThreshold);
				zeroAreaTriangleThreshold = 0.01f;
			}

			cookingParams.areaTestEpsilon = zeroAreaTriangleThreshold;
		}

		if (colliderAsset->ShiftVerticesToOrigin)
			convexDesc.flags |= physx::PxConvexFlag::eSHIFT_VERTICES;

		s_CookingData->CookingSDK->setParams(cookingParams);

		const auto& vertices = meshSource->GetVertices();
		const auto& indices = meshSource->GetIndices();
		const auto& submeshes = meshSource->GetSubMeshes();

		CookingResult cookingResult = CookingResult::Failure;

		for (auto submeshIndex : submeshIndices)
		{
			const auto& submesh = submeshes[submeshIndex];

			convexDesc.points.count = submesh.VertexCount;
			convexDesc.points.stride = sizeof(Vertex);
			convexDesc.points.data = &vertices[submesh.BaseVertex];
			convexDesc.indices.count = submesh.IndexCount / 3;
			convexDesc.indices.data = &indices[submesh.BaseIndex / 3];
			//convexDesc.indices.stride = sizeof(Index);
			convexDesc.indices.stride = sizeof(uint32_t) * 3;

			if (vertices.size() >= convexDesc.vertexLimit)
			{
				PF_ENGINE_INFO("Attempting to cook a convex submesh that has more than {0} vertices! Switching to quantizing the input vertices.", convexDesc.vertexLimit);
				convexDesc.flags |= physx::PxConvexFlag::eQUANTIZE_INPUT | physx::PxConvexFlag::eSHIFT_VERTICES;
				convexDesc.quantizedCount = (physx::PxU16)vertices.size(); // TODO: This should be the vertex count for this submesh, not the entire mesh hierarchy
			}
			else
			{
				// Disable input quantization
				convexDesc.flags &= ~physx::PxConvexFlag::eQUANTIZE_INPUT;
			}

			physx::PxDefaultMemoryOutputStream buf;
			physx::PxConvexMeshCookingResult::Enum oldResult = physx::PxConvexMeshCookingResult::eSUCCESS;
			physx::PxConvexMeshCookingResult::Enum result;

			if (!s_CookingData->CookingSDK->cookConvexMesh(convexDesc, buf, &result))
			{

				bool fatalError = true;

				if (result == physx::PxConvexMeshCookingResult::eZERO_AREA_TEST_FAILED)
				{
					//if (!Application::IsRuntime())
					//{
					//	const auto& meshMetadata = Project::GetEditorAssetManager()->GetMetadata(colliderAsset->ColliderMesh);
					//	ANT_CONSOLE_LOG_WARN("Failed to cook submesh '{}' in mesh '{}'! Triangle with an area of 0 detected! Attempting to cook submesh without checking for zero-area triangles. If you experience issues with this mesh collider you should make sure your mesh doesn't have small triangles!", submesh.MeshName, meshMetadata.FilePath.string());
					//}

					float previousZeroAreaThreshold = cookingParams.areaTestEpsilon;
					convexDesc.flags &= ~physx::PxConvexFlag::eCHECK_ZERO_AREA_TRIANGLES;
					// Don't set to 0 here, it's invalid
					cookingParams.areaTestEpsilon = 0.00001f;
					s_CookingData->CookingSDK->setParams(cookingParams);

					// Attempt to cook without checking zero-area triangles
					oldResult = result;
					fatalError = !s_CookingData->CookingSDK->cookConvexMesh(convexDesc, buf, &result);

					cookingParams.areaTestEpsilon = previousZeroAreaThreshold;
					convexDesc.flags |= physx::PxConvexFlag::eCHECK_ZERO_AREA_TRIANGLES;
					s_CookingData->CookingSDK->setParams(cookingParams);
				}

				if (fatalError)
				{
					std::string errorMessage = "Unknown Error!";

					switch (result)
					{
						case physx::PxConvexMeshCookingResult::ePOLYGONS_LIMIT_REACHED:
							{
								errorMessage = fmt::format("Submesh has more than {} vertices! This shouldn't have been a problem because Ant should've detected this prior to cooking this submesh!", convexDesc.quantizedCount);
								break;
							}
					}

					//if (!Application::IsRuntime())
					//{
					//	const auto& meshMetadata = Project::GetEditorAssetManager()->GetMetadata(colliderAsset->ColliderMesh);
					//	ANT_CONSOLE_LOG_ERROR("Failed to cook submesh '{}' in mesh '{}'! Error: {}", submesh.MeshName, meshMetadata.FilePath.string(), errorMessage);
					//}

					for (auto& existingSubmesh : outData.Submeshes)
						existingSubmesh.ColliderData.Release();
					outData.Submeshes.clear();
					cookingResult = PhysXUtils::FromPhysXCookingResult(result);
					break;
				}
			}

			SubmeshColliderData& data = outData.Submeshes.emplace_back();
			data.ColliderData.Copy(buf.getData(), buf.getSize());
			data.Transform = submesh.Transform * glm::scale(glm::mat4(1.0f), colliderAsset->ColliderScale);
			cookingResult = CookingResult::Success;
		}

		s_CookingData->CookingSDK->setParams(s_CookingData->DefaultCookingParameters);
		outData.Type = MeshColliderType::Convex;
		return cookingResult;
	}
	CookingResult PhysicsMeshCooker::CookTriangleMesh(const Count<MeshCollider>& colliderAsset, const Count<class MeshSource>& meshSource, const std::vector<uint32_t>& submeshIndices, MeshColliderData& outData)
	{
		return CookingResult();
	}
}