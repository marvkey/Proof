#pragma once
#include "Proof/Core/Core.h"
#include "Proof/Math/Math.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Proof/Renderer/Texture.h"
#include <unordered_set>
#include "Material.h"
#include "Proof/Renderer/UIRenderer/UIPanel.h"
#include "Proof/Renderer//ParticleSystem.h"
#include "Proof/Math/MathResource.h"
#include "Proof/Math/Vector.h"
#include "Camera/SceneCamera.h"
#include "Proof/Math/MathConvert.h"
#include "Proof/Core/UUID.h"
#include "Proof/Input/KeyCodes.h"
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>
#include<vector>
#include <string>
#include <format>
#include <set>
#include <any>
/* REMEMBER TO IMPLEMENT SYSTEM OF NEW GET ASSET AS WE HAVE A POINTER BUT BEFORE ACCESS We have to check if ID still exist Asset*/
/* THE DESTRUCTOR OFEACH GETS CALLED WEHN THE POINTER GETS DEREFRENCED BE REMEMBER WHEN TESTING */
namespace Proof
{
	struct Proof_API IDComponent {
		const UUID& GetID() const {
			return m_ID;
		}
		IDComponent(const IDComponent&) = default;
		IDComponent() = default;

		IDComponent(UUID ID) :
			m_ID(ID)
		{

		}
	private:

		UUID m_ID;
		friend class Entity;
		friend class World;
		friend class SceneHierachyPanel;
		friend class SceneSerializer;
	};
	struct Proof_API TagComponent{
		TagComponent() = default;
		TagComponent(const TagComponent&) = default;

		/*TODO CHANGE TO SUBTAG*/
		void AddTag(const std::string& Tag) {
			m_Tags.emplace_back(Tag);
		}
		bool HasTag(const std::string& Tag)const {
			for (const std::string& TagName : m_Tags) {
				if (Tag == TagName) {
					return true;
				}
			}
			return false;
		}

		bool RemoveTag(uint32_t index){
			if(index <=m_Tags.size()){
				m_Tags.erase(m_Tags.begin()+ index);
				return true;
			}
			return false;
		};
		std::string Tag;
	private:
		std::vector<std::string> m_Tags;
		friend class Entity;
		friend class World;
		friend class SceneHierachyPanel;
		friend class SceneSerializer;
	};
	struct HierarchyComponent {
	public:
		UUID ParentHandle = 0;
		std::vector<UUID> Children;

		bool HasChild(UUID id) {
			return std::find(Children.begin(), Children.end(), id) != Children.end();
		}
		 // Return -1 if the child was not found
		int GetChildIndex(UUID id) {
			auto& children = Children;
			for (uint32_t index = 0; index < children.size(); ++index)
			{
				if (children[index] == id)
				{
					return index;
				}
			}
			return -1;
		}
		HierarchyComponent() = default;
		HierarchyComponent(const HierarchyComponent& other) = default;
		HierarchyComponent(UUID parent)
			: ParentHandle(parent) {}
	private:

	};
	struct TransformComponent {
		glm::vec3 Location = {0.0f,0.0f,0.0f};
		glm::vec3 Scale = {1.0f,1.0f,1.0f};
		TransformComponent() = default;
		TransformComponent(const TransformComponent& other) {
			Location = other.Location;
			Rotation = other.Rotation;
			Scale = other.Scale;
		}
		TransformComponent operator+ (const TransformComponent& other)const {
			TransformComponent temp;
			temp.Location = this->Location + other.Location;
			temp.Rotation = this->Rotation + other.Rotation;
			temp.Scale = this->Scale + other.Scale;
			return temp;
		}
		// returns as radians
		glm::vec3 GetRotationEuler() const
		{
			return RotationEuler;
		}
		// pass as radians
		void SetRotationEuler(const glm::vec3& euler)
		{
			RotationEuler = euler;
			Rotation = glm::quat(RotationEuler);
		}
		glm::mat4 GetRotationMatrix() const
		{
			return glm::toMat4(GetRotation());
		}
		// returns as radians
		glm::quat GetRotation() const
		{
			return Rotation;
		}
		// pass as radians
		void SetRotation(const glm::quat& quat)
		{
			Rotation = quat;
			RotationEuler = glm::eulerAngles(Rotation);
		}
		glm::mat4 GetTransform() const {
			return glm::translate(glm::mat4(1.0f), Location) 
				* glm::toMat4(Rotation)
				* glm::scale(glm::mat4(1.0f), Scale);
		}

		glm::vec3 GetFowardVector() const
		{
				// Extract the rotation matrix from the transform matrix

			// Get the forward vector from the rotation matrix
			glm::vec3 forward = glm::normalize(GetRotationMatrix() * glm::vec4(0.0f, 0.0f, 1.0f,1));

			// origininally how it was
			//glm::vec3 forward = glm::normalize(rotationMatrix * glm::vec3(0.0f, 0.0f, -1.0f));

			return forward;
		}
		glm::vec3 GetRightVector() const
		{

			// Get the right vector from the rotation matrix
			glm::vec3 right = glm::normalize(GetRotationMatrix() * glm::vec4(1.0f, 0.0f, 0.0f,1.0f));

			return right;
		}
		glm::vec3 GetUpVector() const
		{

			// Get the up vector from the rotation matrix
			glm::vec3 up = glm::normalize(GetRotationMatrix() * glm::vec4(0.0f, 1.0f, 0.0f,1.0f));

			return up;
		}

		void SetTransform(const glm::mat4& transform)
		{
			MathResource::DecomposeTransform(transform, Location, Rotation, Scale);
			RotationEuler = glm::eulerAngles(Rotation);
		}
	private:

		//from cherno
		// These are private so that you are forced to set them via
		// SetRotation() or SetRotationEuler()
		// This avoids situation where one of them gets set and the other is forgotten.
		//
		// Why do we need both a quat and Euler angle representation for rotation?
		// Because Euler suffers from gimbal lock -> rotations should be stored as quaternions.
		//
		// BUT: quaternions are confusing, and humans like to work with Euler angles.
		// We cannot store just the quaternions and translate to/from Euler because the conversion
		// Euler -> quat -> Euler is not invariant.
		//
		// It's also sometimes useful to be able to store rotations > 360 degrees which
		// quats do not support.
		//
		// Accordingly, we store Euler for "editor" stuff that humans work with, 
		// and quats for everything else.  The two are maintained in-sync via the SetRotation()
		// methods.
		glm::vec3 RotationEuler = { 0.0f, 0.0f, 0.0f };
		glm::quat Rotation = { 1.0f, 0.0f, 0.0f, 0.0f };
		//friend class World;
		//friend class SceneSerializer;
		//friend class SceneHierachyPanel;
		//friend class Entity;
	};
	struct PrefabComponent
	{
		AssetID PrefabID = 0;
		UUID PrefabEntityID = 0; // wich enitty this prefab matches to int eh prefab asset

		PrefabComponent() = default;
		PrefabComponent(const PrefabComponent& other) = default;
	};
	struct Proof_API NativeScriptComponent{
		NativeScriptComponent(const NativeScriptComponent& other) {
			this->Instance = other.Instance;
			this->InstantiateScript = other.InstantiateScript;
			this->DestroyScript = other.DestroyScript;
			this->m_ScriptPointerName = other.m_ScriptPointerName;
			this->m_HasScriptAttached = other.m_HasScriptAttached;
		}
		NativeScriptComponent() = default;
		class Script* Instance = nullptr;
		class Script* (*InstantiateScript)()=nullptr;
		void (*DestroyScript)(NativeScriptComponent*)=nullptr;

		template<class T,typename... Args>
		void Bind(Args... arg) {
			m_HasScriptAttached = true;
			InstantiateScript = []() { return static_cast<Script*>(new T(arg...)); };
			DestroyScript = [](NativeScriptComponent* NSC) {delete NSC->Instance; NSC->Instance = nullptr;};
		}

		const std::string& GetScriptName()const {
			return m_ScriptPointerName;
		}
	private:
		std::string m_ScriptPointerName = "null";
		friend class SceneHierachyPanel;
		friend class World;
		bool m_HasScriptAttached = false;
	};

	struct MeshComponent{
		MeshComponent() = default;
		MeshComponent(const MeshComponent&other):
		m_MeshID(other.m_MeshID)
			,MaterialTable(Count<class MaterialTable>::CreateFrom(other.MaterialTable))
		{
		}

		void SetMesh(UUID ID,bool takeMaterialTable = true);
		void RemoveMesh();

		// we could do  check and compare if these material table equls the meshes material table when serilizing
		// if they do match then we just store in serilize using hte meshes
		//if no mathc then we save the material table
		Count<MaterialTable> MaterialTable = Count<class MaterialTable>::Create();
		Count<class Mesh> GetMesh();
		bool Visible = true;
		bool CastShadow = true;
	private:
		friend class Entity;
		friend class World;
		friend class ECS;
		friend class SceneHierachyPanel;
		friend class SceneSerializer;
		friend class SceneRendererUI;
		friend class Editore3D;
		AssetID m_MeshID = 0;
	};

	struct DynamicMeshComponent {
	public:

	private:
		friend class Entity;
		friend class World;
		friend class ECS;
		friend class SceneHierachyPanel;
		friend class SceneSerializer;
		friend class SceneRendererUI;
		friend class Editore3D;
		UUID m_MeshID;
		uint32_t m_SubmeshIndex = 0;
	};
	struct Proof_API SpriteComponent{
		SpriteComponent(const SpriteComponent&) = default;
		SpriteComponent() = default;
		glm::vec4 Colour = {1.0f,1.0f,1.0f,1.0f};
		Count<Texture2D> Texture;
	private:
		friend class Entity;
		friend class World;
		friend class ECS;
		friend class SceneHierachyPanel;
		friend class SceneSerializer;
		uint32_t StartIndexSlot = 0;
	};

	struct SkyLightComponent{
	public:
		SkyLightComponent(const SkyLightComponent&) = default;
		SkyLightComponent() = default;
		//hdr maps
		void RemoveImage();
		void LoadMap(AssetID asset);
	public:
		// Hdr Maps
		float SkyBoxLoad = 0.0f;
		Count<Environment> Environment;

		//dynamic sky
		bool DynamicSky = true;
		float Turbidity = 2.0f;//min(1.8f)
		float Azimuth = 0;
		float Inclination = 0;
		
		//for all 
		Vector ColorTint{1};
		float MapRotation = 0.0f;
		float Intensity = 1.0f;
	private:
		AssetID Image;
		friend class Entity;
		friend class World;
		friend class ECS;
		friend class SceneHierachyPanel;
		friend class SceneSerializer;
	};

	struct DirectionalLightComponent {
		DirectionalLightComponent(const DirectionalLightComponent&) = default;
		DirectionalLightComponent() = default;

		Vector Color = { 1 };
		float Intensity = 1;

		bool CastShadow = true;
		float ShadowStrength =0.5f; // 0.0 to 1.0 how dark sahdow is 
		
		//soft shadows
		bool CastSoftShadow = true;
		float ShadowSoftness = 0.5f; //how soft the shadow is from 0.0 to 1.0f 
	};

	struct PointLightComponent {
		PointLightComponent(const PointLightComponent&) = default;
		PointLightComponent() = default;
		glm::vec3 Color { 1 };
		float Intensity =1; // min =0, max = 500f;
		float MinRadius =1; // min  = 0,maximum should be Radius
		float Radius =10;// min is 0, max max float
		float Falloff =1.0f; //min is 0, represent how far before the light becomes weaker
		glm::vec3 OffsetLocation { 0 };
		bool CastsShadows = false;
		bool SoftShadows = false;
		float ShadowStrength =0.5f;// 0.0 to 1.0 how dark sahdow is
		float ShadowSoftness = 0.5f;//how soft the shadow is from 0.0 to 1.0f 
	};

	struct SpotLightComponent {
		SpotLightComponent(const SpotLightComponent&) = default;
		SpotLightComponent() = default;
		glm::vec3 Color{ 1.0f };
		float Intensity = 1.0f; // Range: 0.0 to positive infinity.
		float Range = 10.0f;// Range: 0.0 to positive infinity.
		float Angle = 60.0f;// Range: 0.0 to 180.0 degrees. // Angle of the spot light cone (in degrees).
		// Attenuation factor for the spotlight cone's light intensity
		// as the angle between the light direction and surface normal increases.
		// A higher value results in a more rapid decrease in intensity
		// as the angle deviates from the spotlight's central direction.
		float AngleAttenuation = 5.0f; // Range: 0.0 to positive infinity.
		float Falloff = 1.0f;// Range: 0.0 to positive infinity.  factor that affects how the light intensity diminishes.
		bool CastsShadows = false;
		bool SoftShadows = false;
		float ShadowStrength = 0.5f;// 0.0 to 1.0 how dark sahdow is
		float ShadowSoftness = 0.5f;//how soft the shadow is from 0.0 to 1.0f 
	};



	struct Proof_API CameraComponent{
	public:
		ProjectionType Projection = ProjectionType::Perspective;
		CameraComponent(const CameraComponent&) = default;

		CameraComponent() = default;
		float NearPlane = 0.1f;
		float FarPlane = 2000.f;
		float FovDeg = 45;

		bool UseLocalRotation = false;
		friend class World;
		friend class SceneSerializer;
		friend class SceneHierachyPanel;
		friend class WorldRenderer;
	};

	struct Proof_API CubeColliderComponent {
		CubeColliderComponent(const CubeColliderComponent&) = default;
		CubeColliderComponent() = default;
		glm::vec3 OffsetLocation = { 0,0,0 };
		glm::vec3 OffsetScale= { 1,1,1 };
		bool IsTrigger = false;

		void RemovePhysicsMaterial() {
			m_PhysicsMaterialPointerID = 0;
		}

		bool HasPhysicsMaterial() {
			return GetPhysicsMaterial() == nullptr ? false : true;
		}
		Count<class PhysicsMaterial> GetPhysicsMaterial();

	private:
		mutable UUID m_PhysicsMaterialPointerID = 0;
		friend class World;
		friend class SceneSerializer;
		friend class SceneHierachyPanel;
		friend class WorldRenderer;
		friend class PhysicsEngine;
		friend class PhysicsActor;
	};
	
	struct Proof_API SphereColliderComponent {
		SphereColliderComponent(const SphereColliderComponent&) = default;
		SphereColliderComponent() = default;
		Vector OffsetLocation = { 0,0,0 };
		float Radius = 1.0f;
		bool IsTrigger = false;
		void RemovePhysicsMaterial() {
			m_PhysicsMaterialPointerID = 0;
		}
		bool HasPhysicsMaterial() {
			return GetPhysicsMaterial() == nullptr ? false : true;
		}
		Count<class PhysicsMaterial> GetPhysicsMaterial();
	private:
		UUID m_PhysicsMaterialPointerID = 0;
		friend class World;
		friend class SceneSerializer;
		friend class SceneHierachyPanel;
		friend class PhysicsActor;
		friend class WorldRenderer;
		friend class PhysicsEngine;
	};
	enum class CapsuleDirection {
		X=1,
		Y = 0,
		Z =2
	};
	struct Proof_API CapsuleColliderComponent {
		CapsuleColliderComponent(const CapsuleColliderComponent&) = default;
		CapsuleColliderComponent() = default;
		Vector OffsetLocation = { 0,0,0 };
		float Radius = 0.5f;
		float Height = 2.0f;
		CapsuleDirection Direction = CapsuleDirection::Y;
		bool IsTrigger = false;
		void RemovePhysicsMaterial() {
			m_PhysicsMaterialPointerID = 0;
		}
		bool HasPhysicsMaterial(){
			return GetPhysicsMaterial() == nullptr ? false : true;
		}
		Count<class PhysicsMaterial> GetPhysicsMaterial();
	private:
		friend class World;
		friend class SceneSerializer;
		friend class SceneHierachyPanel;
		friend class PhysicsActor;
		friend class WorldRenderer;
		friend class PhysicsEngine;
		mutable UUID m_PhysicsMaterialPointerID= 0;
	};

	struct Proof_API MeshColliderComponent {
		MeshColliderComponent(const MeshColliderComponent&) = default;
		MeshColliderComponent() = default;
		bool IsTrigger = false;
		void RemovePhysicsMaterial() {
			m_PhysicsMaterialPointerID = 0;
		}
		bool HasPhysicsMaterial() {
			return GetPhysicsMaterial() == nullptr ? false : true;
		}
		Count<class PhysicsMaterial> GetPhysicsMaterial();
		Count<class Mesh> GetMesh();

		UUID GetMeshSource() {
			return m_MeshAssetPointerID;
		}
		void RemoveMeshSource() {
			m_MeshAssetPointerID = 0;
		}
	private:
		mutable UUID m_MeshAssetPointerID = 0;// POINTS TO THE MESH ASSET
		friend class World;
		friend class SceneSerializer;
		friend class SceneHierachyPanel;
		friend class WorldRenderer;
		friend class PhysicsActor;
		friend class PhysicsEngine;
		mutable UUID m_PhysicsMaterialPointerID = 0;
	};	
	enum class RigidBodyType {
		Static,
		Dynamic
	};
	enum class ForceMode {
		Force,				
		Impule,			
		VelocityChange,	
		Acceleration
	};
	
	class Proof_API RigidBodyComponent {
	public:
		RigidBodyComponent(const RigidBodyComponent&) = default;
		RigidBodyComponent() = default;
		
		float Mass = 1.0f;
		float AngularDrag = 0.05f;
		float LinearDrag = 0.0f;
		bool Gravity = true;
		bool Kinimatic = false;
		
		VectorTemplate<bool>FreezeLocation = { false,false,false };
		VectorTemplate<bool>FreezeRotation = { false,false,false };

		RigidBodyType GetType() {
			return m_RigidBodyType;
		}
	private:
		RigidBodyType m_RigidBodyType = RigidBodyType::Static;
		void* m_RuntimeBody = nullptr;
		friend class World;
		friend class SceneSerializer;
		friend class SceneHierachyPanel;
		friend class WorldRenderer;
		friend class PhysicsActor;
	};

	struct ScriptComponent {
	public:
		ScriptComponent(const ScriptComponent& other) = default;
		ScriptComponent() = default;
		std::unordered_set<std::string> ScriptsNames;
	};

	struct TextComponent {
	public:
		TextComponent(const TextComponent& other) = default;
		TextComponent() = default;

		//chagne to u32 string in teh futre 
		std::string Text;
		glm::vec4 Colour{ 1 };

		// horizontal distnace between each character
		float Kerning = 0.0f;
		// spacing of new line 
		float LineSpacing = 1.0f;
		//Todo background color

		bool UseLocalRotation = false;
	};
	
	struct PlayerInputComponent {
	public:
		Players InputPlayer  = Players::None;
		Count<class Prefab> Player;
	};

	struct ParticleSystemComponent {
	public:
		ParticleSystemComponent(const ParticleSystemComponent& other) = default;
		ParticleSystemComponent() = default;
		Count< ParticleHandlerTable> ParticleHandlerTable = Count<class ParticleHandlerTable>::Create();
	};

	struct PlayerHUDComponent {
		Count< UITable> HudTable = Count<class UITable>::Create();
	};
	template<class ... Component>
	struct ComponentGroup {

	};
	using AllComponents =
		ComponentGroup<IDComponent, TagComponent, HierarchyComponent, TransformComponent, PrefabComponent,
		MeshComponent, SkyLightComponent, DirectionalLightComponent, PointLightComponent,SpotLightComponent, CameraComponent,
		CubeColliderComponent, SphereColliderComponent, CapsuleColliderComponent,MeshColliderComponent,RigidBodyComponent,
		ScriptComponent, TextComponent, PlayerInputComponent, PlayerHUDComponent, ParticleSystemComponent>;
	

	using LightComponnet =ComponentGroup<SkyLightComponent, DirectionalLightComponent, PointLightComponent, SpotLightComponent>;
}



