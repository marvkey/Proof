#include "Proofprch.h"
#include "MeshImpoter.h"

#include "Proof/Scene/Mesh.h"
#include "Proof/Math/MathResource.h"
#include "Proof/Renderer/Renderer.h"
#include "Proof/Renderer/RenderMaterial.h"
#include "Proof/Asset/AssetManager.h"
#include "Proof/Utils/FileSystem.h"

#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <assimp/Importer.hpp>

//https://github.com/Ant-Play/Ant/blob/2dab7c0362f017911df9090b1608ec4b81ad1f2c/Ant/src/Ant/Asset/AssimpMeshImporter.cpp
namespace Proof
{
	//aiProcess_FlipUVs
	//aiProcess_GenSmoothNormals
	static const uint32_t s_MeshImportFlags =
		aiProcess_CalcTangentSpace |        // Create binormals/tangents just in case
		aiProcess_Triangulate |             // Make sure we're triangles
		aiProcess_SortByPType |             // Split meshes by primitive type
		aiProcess_GenNormals |              // Make sure we have legit normals
		aiProcess_GenUVCoords |             // Convert UVs if required 
		//		aiProcess_OptimizeGraph |
		aiProcess_OptimizeMeshes |          // Batch draws where possible
		aiProcess_JoinIdenticalVertices |
		aiProcess_LimitBoneWeights |        // If more than N (=4) bone weights, discard least influencing bones and renormalise sum to 1
		aiProcess_ValidateDataStructure |   // Validation
		aiProcess_GlobalScale;              // e.g. convert cm to m for fbx import (and other formats where cm is native)
	namespace Utils {

		glm::mat4 Mat4FromAIMatrix4x4(const aiMatrix4x4& matrix)
		{
			glm::mat4 result;
			//the a,b,c,d in assimp is the row ; the 1,2,3,4 is the column
			result[0][0] = matrix.a1; result[1][0] = matrix.a2; result[2][0] = matrix.a3; result[3][0] = matrix.a4;
			result[0][1] = matrix.b1; result[1][1] = matrix.b2; result[2][1] = matrix.b3; result[3][1] = matrix.b4;
			result[0][2] = matrix.c1; result[1][2] = matrix.c2; result[2][2] = matrix.c3; result[3][2] = matrix.c4;
			result[0][3] = matrix.d1; result[1][3] = matrix.d2; result[2][3] = matrix.d3; result[3][3] = matrix.d4;
			return result;
		}

		//#if MESH_DEBUG_LOG
		#if 1
		void PrintNode(aiNode* node, size_t depth)
		{
			PF_ENGINE_TRACE("{0:^{1}}{2} {{", "", depth * 3, node->mName.C_Str());
			++depth;
			glm::vec3 translation;
			glm::vec3 rotation;
			glm::vec3 scale;
			glm::mat4 transform = Mat4FromAIMatrix4x4(node->mTransformation);
			MathResource::DecomposeTransform(transform, translation, rotation, scale);
			rotation = glm::degrees(rotation);

			PF_ENGINE_TRACE("{0:^{1}}translation: ({2:6.2f}, {3:6.2f}, {4:6.2f})", "", depth * 3, translation.x, translation.y, translation.z);
			PF_ENGINE_TRACE("{0:^{1}}rotation:    ({2:6.2f}, {3:6.2f}, {4:6.2f})", "", depth * 3, rotation.x, rotation.y, rotation.z);
			PF_ENGINE_TRACE("{0:^{1}}scale:       ({2:6.2f}, {3:6.2f}, {4:6.2f})", "", depth * 3, scale.x, scale.y, scale.z);
			for (uint32_t i = 0; i < node->mNumChildren; ++i)
			{
				PrintNode(node->mChildren[i], depth);
			}
			--depth;
			PF_ENGINE_TRACE("{0:^{1}}}}", "", depth * 3);
		}
		#endif

	}
	inline void TraverseNodes(std::vector<MeshNode>& nodes, std::vector<SubMesh>& subMeshes, void* assimpNode, uint32_t nodeIndex, const glm::mat4& parentTransform = glm::mat4(1.0f), uint32_t level = 0)
	{
		aiNode* aNode = (aiNode*)assimpNode;

		MeshNode& node = nodes[nodeIndex];
		node.Name = aNode->mName.C_Str();
		node.LocalTransform = Utils::Mat4FromAIMatrix4x4(aNode->mTransformation);

		glm::mat4 transform = parentTransform * node.LocalTransform;
		for (uint32_t i = 0; i < aNode->mNumMeshes; i++)
		{
			uint32_t submeshIndex = aNode->mMeshes[i];
			auto& submesh = subMeshes[submeshIndex];
			submesh.NodeName = aNode->mName.C_Str();
			submesh.Transform = transform;
			submesh.LocalTransform = node.LocalTransform;

			node.Submeshes.push_back(submeshIndex);
		}

		// ANT_MESH_LOG("{0} {1}", LevelToSpaces(level), node->mName.C_Str());

		uint32_t parentNodeIndex = nodes.size() - 1;
		node.Children.resize(aNode->mNumChildren);
		for (uint32_t i = 0; i < aNode->mNumChildren; i++)
		{
			MeshNode& child = nodes.emplace_back();
			uint32_t childIndex = nodes.size() - 1;
			child.Parent = parentNodeIndex;
			nodes[nodeIndex].Children[i] = childIndex;
			TraverseNodes(nodes, subMeshes,aNode->mChildren[i], childIndex, transform, level + 1);
		}
	}
	MeshImporter::MeshImporter(const std::filesystem::path& path):
		m_Path(path)
	{
	}
	Count<class MeshSource> MeshImporter::ImportToMeshSource()
    {

		Assimp::Importer importer;
		importer.SetPropertyBool(AI_CONFIG_IMPORT_FBX_PRESERVE_PIVOTS, false);

		const aiScene* scene = importer.ReadFile(m_Path.string(), s_MeshImportFlags);
		if (!scene /* || !scene->HasMeshes()*/)  // note: scene can legit contain no meshes (e.g. it could contain an armature, an animation, and no skin (mesh)))
		{
			PF_ENGINE_ERROR("Mesh {} Failed to load mesh file: {}", m_Path.string());
			//meshSource->SetFlag(AssetFlag::Invalid);
			return nullptr;
		}
		/*
		meshSource->m_Skeleton = AnimationImporterAssimp::ImportSkeleton(scene);
		ANT_CORE_INFO_TAG("Animation", "Skeleton {0} found in mesh file '{1}'", meshSource->HasSkeleton() ? "" : "not", m_Path.string());
		if (meshSource->HasSkeleton())
		{
			const auto animationNames = AnimationImporterAssimp::GetAnimationNames(scene);
			meshSource->m_Animations.reserve(std::size(animationNames));
			for (const auto& animationName : animationNames)
			{
				meshSource->m_Animations.emplace_back(AnimationImporterAssimp::ImportAnimation(scene, animationName, *meshSource->m_Skeleton));
			}
		}
		*/
		AABB meshSourceBoundingBox;
		std::vector<SubMesh> subMeshes;
		std::vector<Vertex> vertices;
		std::vector<Index> indices;
		std::vector<MeshNode> nodes;
		Count<MaterialTable> materialTable = Count<MaterialTable>::Create();

		if (scene->HasMeshes())
		{
			uint32_t vertexCount = 0;
			uint32_t indexCount = 0;

			meshSourceBoundingBox.Min = { FLT_MAX, FLT_MAX, FLT_MAX };
			meshSourceBoundingBox.Max = { -FLT_MAX, -FLT_MAX, -FLT_MAX };

			subMeshes.reserve(scene->mNumMeshes);
			for (unsigned m = 0; m < scene->mNumMeshes; m++)
			{
				aiMesh* mesh = scene->mMeshes[m];
				if (mesh->mName.length == 0)
					mesh->mName.Set("UnnamedMesh");

				SubMesh& submesh = subMeshes.emplace_back();
				submesh.BaseVertex = vertexCount;
				submesh.BaseIndex = indexCount;
				submesh.MaterialIndex = mesh->mMaterialIndex;
				submesh.VertexCount = mesh->mNumVertices;
				submesh.IndexCount = mesh->mNumFaces * 3;
				submesh.Name = mesh->mName.C_Str();

				vertexCount += mesh->mNumVertices;
				indexCount += submesh.IndexCount;

				PF_CORE_ASSERT(mesh->HasPositions(), "Meshes require positions.");
				PF_CORE_ASSERT(mesh->HasNormals(), "Meshes require normals.");

				// Vertices
				auto& aabb = submesh.BoundingBox;
				aabb.Min = { FLT_MAX, FLT_MAX, FLT_MAX };
				aabb.Max = { -FLT_MAX, -FLT_MAX, -FLT_MAX };
				for (size_t i = 0; i < mesh->mNumVertices; i++)
				{
					Vertex vertex;
					vertex.Position = { mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z };
					vertex.Normal = { mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z };
					aabb.Min.x = glm::min(vertex.Position.x, aabb.Min.x);
					aabb.Min.y = glm::min(vertex.Position.y, aabb.Min.y);
					aabb.Min.z = glm::min(vertex.Position.z, aabb.Min.z);
					aabb.Max.x = glm::max(vertex.Position.x, aabb.Max.x);
					aabb.Max.y = glm::max(vertex.Position.y, aabb.Max.y);
					aabb.Max.z = glm::max(vertex.Position.z, aabb.Max.z);

					if (mesh->HasTangentsAndBitangents())
					{
						vertex.Tangent = { mesh->mTangents[i].x, mesh->mTangents[i].y, mesh->mTangents[i].z };
						vertex.Bitangent = { mesh->mBitangents[i].x, mesh->mBitangents[i].y, mesh->mBitangents[i].z };
					}

					if (mesh->HasTextureCoords(0))
						vertex.TexCoord = { mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y };

					vertices.push_back(vertex);
				}

				// Indices
				for (size_t i = 0; i < mesh->mNumFaces; i++)
				{
					PF_CORE_ASSERT(mesh->mFaces[i].mNumIndices == 3, "Must have 3 indices.");
					Index index = { mesh->mFaces[i].mIndices[0], mesh->mFaces[i].mIndices[1], mesh->mFaces[i].mIndices[2] };
					indices.push_back(index);

					//meshSource->m_TriangleCache[m].emplace_back(meshSource->m_Vertices[index.V1 + submesh.BaseVertex], meshSource->m_Vertices[index.V2 + submesh.BaseVertex], meshSource->m_Vertices[index.V3 + submesh.BaseVertex]);
				}
			}

			//#if MESH_DEBUG_LOG
			//ANT_CORE_INFO_TAG("Mesh", "Traversing nodes for scene '{0}'", filename);
			Utils::PrintNode(scene->mRootNode, 0);
			//#endif

			MeshNode& rootNode = nodes.emplace_back();
			TraverseNodes(nodes,subMeshes, scene->mRootNode, 0);

			for (const auto& submesh : subMeshes)
			{
				AABB transformedSubmeshAABB = submesh.BoundingBox;
				glm::vec3 min = glm::vec3(submesh.Transform * glm::vec4(transformedSubmeshAABB.Min, 1.0f));
				glm::vec3 max = glm::vec3(submesh.Transform * glm::vec4(transformedSubmeshAABB.Max, 1.0f));

				meshSourceBoundingBox.Min.x = glm::min(meshSourceBoundingBox.Min.x, min.x);
				meshSourceBoundingBox.Min.y = glm::min(meshSourceBoundingBox.Min.y, min.y);
				meshSourceBoundingBox.Min.z = glm::min(meshSourceBoundingBox.Min.z, min.z);
				meshSourceBoundingBox.Max.x = glm::max(meshSourceBoundingBox.Max.x, max.x);
				meshSourceBoundingBox.Max.y = glm::max(meshSourceBoundingBox.Max.y, max.y);
				meshSourceBoundingBox.Max.z = glm::max(meshSourceBoundingBox.Max.z, max.z);
			}

		}
		Count<Texture2D> whiteTexture = Renderer::GetWhiteTexture();

		if (scene->HasMaterials())
		{
			PF_ENGINE_INFO("---- Materials - {0} ----", m_Path.string().c_str());

			for (uint32_t i = 0; i < scene->mNumMaterials; i++)
			{
				auto aiMaterial = scene->mMaterials[i];
				auto aiMaterialName = aiMaterial->GetName();
				if (aiMaterialName.length == 0)
					aiMaterialName.Set("UnnamedMaterial");

				Count<Material> mi = Count<Material>::Create(aiMaterialName.C_Str());
				materialTable->SetMaterial(i, mi);

				PF_ENGINE_INFO("  {0} (Index = {1})", aiMaterialName.data, i);
				aiString aiTexPath;
				uint32_t textureCount = aiMaterial->GetTextureCount(aiTextureType_DIFFUSE);
				PF_ENGINE_INFO("    TextureCount = {0}", textureCount);
				glm::vec3 albedoColor(0.8f);
				float emission = 0.0f;
				aiColor3D aiColor, aiEmission;
				if (aiMaterial->Get(AI_MATKEY_COLOR_DIFFUSE, aiColor) == AI_SUCCESS)
					albedoColor = { aiColor.r, aiColor.g, aiColor.b };

				if (aiMaterial->Get(AI_MATKEY_COLOR_EMISSIVE, aiEmission) == AI_SUCCESS)
					emission = aiEmission.r;

				//mi->SetAlbedo(Vector{ aiColor.r, aiColor.g, aiColor.b });
				//mi->SetAlbedo(albedoColor);
				//mi->Set("u_MaterialUniforms.Emission", emission);

				float roughness, metalness;
				if (aiMaterial->Get(AI_MATKEY_ROUGHNESS_FACTOR, roughness) != aiReturn_SUCCESS)
					roughness = 0.5f; // Default value

				if (aiMaterial->Get(AI_MATKEY_REFLECTIVITY, metalness) != aiReturn_SUCCESS)
					metalness = 0.0f;



				PF_ENGINE_TRACE("    COLOR = {0}, {1}, {2}", aiColor.r, aiColor.g, aiColor.b);
				PF_ENGINE_TRACE("    ROUGHNESS = {0}", roughness);
				PF_ENGINE_TRACE("    METALNESS = {0}", metalness);
				PF_ENGINE_TRACE("    EMISSION = {0}", emission);
				mi->SetEmission(emission);
				bool hasAlbedoMap = aiMaterial->GetTexture(aiTextureType_DIFFUSE, 0, &aiTexPath) == AI_SUCCESS;
				bool fallback = !hasAlbedoMap;
				if (hasAlbedoMap)
				{
					AssetID textureHandle = 0;
					TextureConfiguration spec;
					spec.DebugName = aiTexPath.C_Str();
					if (auto aiTexEmbedded = scene->GetEmbeddedTexture(aiTexPath.C_Str()))
					{
						spec.Format = ImageFormat::RGBA;
						spec.Width = aiTexEmbedded->mWidth;
						spec.Height = aiTexEmbedded->mHeight;
						
						textureHandle = AssetManager::CreateRuntimeOnlyRendererAsset<Texture2D>(FileSystem::GetFileName(aiTexPath.C_Str()), spec,Buffer(aiTexEmbedded->pcData, 1))->GetID();
					}
					else
					{
						// TODO: Temp - this should be handled by filesystem
						auto parentPath = m_Path.parent_path();
						parentPath /= std::string(aiTexPath.data);
						std::string texturePath = parentPath.string();
						//texturePath = FileSystem::GetFileFullNameWithoutExtension(texturePath);
						//texturePath += ".Texture.ProofAsset";
						
						PF_ENGINE_TRACE("    Albedo map path = {0}", texturePath);
						//textureHandle = AssetManager::CreateMemoryOnlyRendererAsset<Texture2D>(FileSystem::GetFileName(texturePath), spec, std::filesystem::path(texturePath))->GetID();
						//textureHandle = AssetManager::GetAsset<Texture2D>(texturePath)->GetID();
						textureHandle = AssetManager::CreateRuntimeOnlyRendererAsset<Texture2D>(FileSystem::GetFileName(texturePath), spec, std::filesystem::path(texturePath))->GetID();
					}

					Count<Texture2D> texture = AssetManager::GetAsset<Texture2D>(textureHandle);
					if (texture /* && texture->Loaded()*/)
					{
						mi->SetAlbedoMap(texture);
						mi->SetAlbedo(glm::vec3(1.0f));
					}
					else
					{
						PF_ENGINE_ERROR("Mesh Could not load texture: {0}", aiTexPath.C_Str());
						fallback = true;
					}
				}

				if (fallback)
				{
					PF_ENGINE_INFO("    No albedo map");
					mi->SetAlbedoMap(whiteTexture);
				}

				// Normal maps
				bool hasNormalMap = aiMaterial->GetTexture(aiTextureType_NORMALS, 0, &aiTexPath) == AI_SUCCESS;
				fallback = !hasNormalMap;
				if (hasNormalMap)
				{
					AssetID textureHandle = 0;

					TextureConfiguration spec;
					spec.DebugName = aiTexPath.C_Str();
					if (auto aiTexEmbedded = scene->GetEmbeddedTexture(aiTexPath.C_Str()))
					{
						spec.Format = ImageFormat::RGB;
						spec.Width = aiTexEmbedded->mWidth;
						spec.Height = aiTexEmbedded->mHeight;
						textureHandle = AssetManager::CreateRuntimeOnlyRendererAsset<Texture2D>(FileSystem::GetFileName(aiTexPath.C_Str()), spec, Buffer(aiTexEmbedded->pcData, 1))->GetID();
					}
					else
					{

						// TODO: Temp - this should be handled by Proof filesystem
						auto parentPath = m_Path.parent_path();
						parentPath /= std::string(aiTexPath.data);
						std::string texturePath = parentPath.string();
						PF_ENGINE_TRACE("    Normal map path = {0}", texturePath);
						textureHandle = AssetManager::CreateRuntimeOnlyRendererAsset<Texture2D>(FileSystem::GetFileName(texturePath), spec, texturePath)->GetID();
					}

					Count<Texture2D> texture = AssetManager::GetAsset<Texture2D>(textureHandle);
					if (texture /* && texture->Loaded()*/)
					{
						mi->SetNormalMap(texture);
						mi->SetNormalTextureToggle(true);
					}
					else
					{
						PF_ENGINE_ERROR("    Could not load texture: {0}", aiTexPath.C_Str());
						fallback = true;
					}
				}

				if (fallback)
				{
					PF_ENGINE_INFO("    No normal map");
					mi->SetNormalMap(whiteTexture);
					mi->SetNormalTextureToggle(false);
				}

				// Roughness map
				bool hasRoughnessMap = aiMaterial->GetTexture(aiTextureType_SHININESS, 0, &aiTexPath) == AI_SUCCESS;
				fallback = !hasRoughnessMap;
				if (hasRoughnessMap)
				{
					AssetID textureHandle = 0;
					TextureConfiguration spec;
					spec.DebugName = aiTexPath.C_Str();
					if (auto aiTexEmbedded = scene->GetEmbeddedTexture(aiTexPath.C_Str()))
					{
						spec.Format = ImageFormat::RGB;
						spec.Width = aiTexEmbedded->mWidth;
						spec.Height = aiTexEmbedded->mHeight;
						textureHandle = AssetManager::CreateRuntimeOnlyRendererAsset<Texture2D>(FileSystem::GetFileName(aiTexPath.C_Str()), spec, Buffer(aiTexEmbedded->pcData, 1))->GetID();
					}
					else
					{
						// TODO: Temp - this should be handled by Proof filesystem
						auto parentPath = m_Path.parent_path();
						parentPath /= std::string(aiTexPath.data);
						std::string texturePath = parentPath.string();
						PF_ENGINE_TRACE("    Roughness map path = {0}", texturePath);
						textureHandle = AssetManager::CreateRuntimeOnlyRendererAsset<Texture2D>(FileSystem::GetFileName(texturePath), spec, texturePath)->GetID();
					}

					Count<Texture2D> texture = AssetManager::GetAsset<Texture2D>(textureHandle);
					if (texture /*&& texture->Loaded()*/)
					{
						mi->SetRoughnessMap(texture);
						mi->SetRoughness(1.0f);
					}
					else
					{
						PF_ENGINE_ERROR("    Could not load texture: {0}", aiTexPath.C_Str());
						fallback = true;
					}
				}

				if (fallback)
				{
					PF_ENGINE_TRACE("    No roughness map");
					mi->SetRoughnessMap(whiteTexture);
					mi->SetRoughness(roughness);
				}
				bool metalnessTextureFound = false;
				for (uint32_t p = 0; p < aiMaterial->mNumProperties; p++)
				{
					auto prop = aiMaterial->mProperties[p];
					if (prop->mType == aiPTI_String)
					{
						uint32_t strLength = *(uint32_t*)prop->mData;
						std::string str(prop->mData + 4, strLength);

						std::string key = prop->mKey.data;
						if (key == "$raw.ReflectionFactor|file")
						{
							AssetID textureHandle = 0;
							TextureConfiguration spec;
							spec.DebugName = str;
							if (auto aiTexEmbedded = scene->GetEmbeddedTexture(str.data()))
							{
								spec.Format = ImageFormat::RGB;
								spec.Width = aiTexEmbedded->mWidth;
								spec.Height = aiTexEmbedded->mHeight;
								textureHandle = AssetManager::CreateRuntimeOnlyRendererAsset<Texture2D>(FileSystem::GetFileName(aiTexPath.C_Str()), spec, Buffer(aiTexEmbedded->pcData, 1))->GetID();
							}
							else
							{
								// TODO: Temp - this should be handled by Proof filesystem
								auto parentPath = m_Path.parent_path();
								parentPath /= str;
								std::string texturePath = parentPath.string();
								PF_ENGINE_TRACE("    Metalness map path = {0}", texturePath);
								textureHandle = AssetManager::CreateRuntimeOnlyRendererAsset<Texture2D>(FileSystem::GetFileName(texturePath), spec, texturePath)->GetID();
							}

							Count<Texture2D> texture = AssetManager::GetAsset<Texture2D>(textureHandle);
							if (texture /*&& texture->Loaded()*/)
							{
								metalnessTextureFound = true;
								mi->SetMetalnessMap(texture);
								mi->SetMetalness(1.0f);
							}
							else
							{
								PF_ENGINE_WARN("Mesh", "    Could not load texture: {0}", str);
							}
							break;
						}
					}
				}

				fallback = !metalnessTextureFound;
				if (fallback)
				{
					PF_ENGINE_TRACE("    No metalness map");
					mi->SetMetalnessMap(whiteTexture);
					mi->SetMetalness(metalness);

				}
			}
		}
		else
		{
			
			auto mi = AssetManager::GetDefaultAsset(DefaultRuntimeAssets::Material).As<Material>();
			materialTable->SetMaterial(0, mi);
		}

        return Count<MeshSource>::Create(FileSystem::GetFileName(m_Path), vertices, indices, subMeshes, nodes, materialTable, meshSourceBoundingBox);
    }
    
}

