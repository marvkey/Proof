#include "Proofprch.h"
#include "PhysicsPanelStats.h"
#include "Proof/Core/Profile.h"
#include "Proof/ImGui/UI.h"
#include "Proof/Scene/World.h"
#include "Proof/Physics/PhysicsWorld.h"
#include "Proof/Physics/PhysicsActor.h"
#include "Proof/Physics/PhysicsShapes.h"
#include "Proof/Physics/PhysicsMaterial.h"
#include "Proof/Utils/StringUtils.h"
#include <fmt/format.h>

//https://github.com/Ant-Play/Ant/blob/2dab7c0362f017911df9090b1608ec4b81ad1f2c/AntPlay/src/Panels/PhysicsStatsPanel.cpp
namespace Proof
{
	PhysicsStatsPanel::PhysicsStatsPanel() {}
	PhysicsStatsPanel::~PhysicsStatsPanel()
	{
		m_PhysicsWorld = nullptr;
	}

	void PhysicsStatsPanel::SetWorldContext(const Count<World>& context)
	{
		if (context == nullptr)
			m_PhysicsWorld = nullptr;
		else
			m_PhysicsWorld = context->GetPhysicsWorld();
	}

	void PhysicsStatsPanel::OnImGuiRender(const char* dsiplayName, bool& isOpen)
	{
		PF_PROFILE_FUNC();

		if (ImGui::Begin(dsiplayName, &isOpen) && m_PhysicsWorld != nullptr)
		{
			const auto& stats = m_PhysicsWorld->GetSimulationStats();

			if (UI::AttributeTreeNode("General Statistics"))
			{
				ImGui::Text("Active Constraints: %d", stats.nbActiveConstraints);
				ImGui::Text("Active Dynamic Actors: %d", stats.nbActiveDynamicBodies);
				ImGui::Text("Active Kinematic Actors: %d", stats.nbActiveKinematicBodies);
				ImGui::Text("Static Actors: %d", stats.nbStaticBodies);
				ImGui::Text("Dynamic Actors: %d", stats.nbDynamicBodies);
				ImGui::Text("Kinematic Actors: %d", stats.nbKinematicBodies);

				for (size_t i = 0; i < physx::PxGeometryType::eGEOMETRY_COUNT; i++)
					ImGui::Text("%s Shapes: %d", EnumReflection::EnumString((physx::PxGeometryType::Enum)i), stats.nbShapes[i]);

				ImGui::Text("Aggregates: %d", stats.nbAggregates);
				ImGui::Text("Articulations: %d", stats.nbArticulations);
				ImGui::Text("Axis Solver Constraints: %d", stats.nbAxisSolverConstraints);
				ImGui::Text("Compressed Contact Size: %s",Utils::String::BytesToString( stats.compressedContactSize));
				ImGui::Text("Required Contact Constraint Size: %s", Utils::String::BytesToString( stats.requiredContactConstraintMemory));
				ImGui::Text("Peak Constraint Size: %s", Utils::String::BytesToString(stats.peakConstraintMemory));

				ImGui::TreePop();
			}

			ImGui::Spacing();
			ImGui::Separator();
			ImGui::Spacing();

			static char s_SearchBuffer[256];
			ImGui::SetNextItemWidth(-1);
			ImGui::InputTextWithHint("##actorsearch", "Search Actors, Shapes, etc...", s_SearchBuffer, 255);

			const auto& actors = m_PhysicsWorld->GetActors();

			size_t searchLength = strlen(s_SearchBuffer);

			if (UI::AttributeTreeNode(fmt::format("Static Actors ({0})", stats.nbStaticBodies), searchLength > 0))
			{
				for (const auto& [entityID, actor] : actors)
				{
					if (actor->IsDynamic())
						continue;

					const auto& tag = actor->GetEntity().GetComponent<TagComponent>().Tag;

					if (searchLength > 0)
					{
						std::string searchBuffer = s_SearchBuffer;
						const auto lowerCopy = Utils::String::ToLower(searchBuffer);
						if (lowerCopy.find(searchBuffer) == std::string::npos)
							continue;
					}

					std::string label = fmt::format("{0}##{1}", tag, entityID);

					if (UI::AttributeTreeNode(label, false))
					{
						UI::BeginPropertyGrid();
						UI::PushItemDisabled();
						glm::vec3 translation = actor->GetLocation();
						glm::quat rotation = actor->GetRotation();
						glm::vec3 rotationEuler = glm::eulerAngles(rotation);
						UI::AttributeDrag("Translation", translation);
						UI::AttributeDrag("Rotation", rotationEuler);
						UI::PopItemDisabled();
						UI::EndPropertyGrid();

						const auto& collisionShapes = actor->GetCollisionShapes();

						ImGui::Text("Shapes: %d", collisionShapes.size());
						for (const auto& shape : collisionShapes)
						{
							std::string shapeLabel = fmt::format("{0}##{1}", shape->GetShapeName(), entityID);
							bool shapeOpen = UI::AttributeTreeNode(shapeLabel, false);
							if (shapeOpen)
							{
								UI::BeginPropertyGrid();
								UI::PushItemDisabled();

								glm::vec3 offset = shape->GetCenter();
								bool isTrigger = shape->IsTrigger();

								UI::AttributeDrag("Offset", offset);
								UI::AttributeBool("Is Trigger", isTrigger);

								const auto material = shape->GetMaterial();
								float staticFriction = material->GetStaticFriction();
								float dynamicFriction = material->GetDynamicFriction();
								float restitution = material->GetBounciness();

								UI::AttributeDrag("Static Friction", staticFriction);
								UI::AttributeDrag("Dynamic Friction", staticFriction);
								UI::AttributeDrag("Bounciness", restitution);

								UI::PopItemDisabled();
								UI::EndPropertyGrid();
								ImGui::TreePop();
							}
						}

						ImGui::TreePop();
					}
				}

				ImGui::TreePop();
			}

			if (UI::AttributeTreeNode(fmt::format("Dynamic Actors ({0})", stats.nbDynamicBodies), searchLength > 0))
			{
				for (const auto& [entityID, actor] : actors)
				{
					if (!actor->IsDynamic())
						continue;

					const auto& tag = actor->GetEntity().GetComponent<TagComponent>().Tag;

					if (searchLength > 0)
					{
						std::string searchBuffer = s_SearchBuffer;
						const auto lowerCopy = Utils::String::ToLower(searchBuffer);
						if (lowerCopy.find(searchBuffer) == std::string::npos)
							continue;
					}

					std::string label = fmt::format("{0}##{1}", tag, entityID);
					if (UI::AttributeTreeNode(label, false))
					{
						UI::BeginPropertyGrid();
						UI::PushItemDisabled();

						glm::vec3 translation = actor->GetLocation();
						glm::quat rotation = actor->GetRotation();
						glm::vec3 rotationEuler = glm::eulerAngles(rotation);
						UI::AttributeDrag("Translation", translation);
						UI::AttributeDrag("Rotation", rotationEuler);

						bool isKinematic = actor->IsKinematic();
						UI::AttributeBool("Is Kinematic", isKinematic);

						float mass = actor->GetMass();
						UI::AttributeDrag("Mass", mass);
						float inverseMass = actor->GetInverseMass();
						UI::AttributeDrag("Inverse Mass", inverseMass);

						bool hasGravity = actor->IsGravityEnabled();
						UI::AttributeBool("Has Gravity", hasGravity);

						bool isSleeping = actor->IsSleeping();
						UI::AttributeBool("Is Sleeping", isSleeping);

						glm::vec3 linearVelocity = actor->GetLinearVelocity();
						float maxLinearVelocity = actor->GetMaxLinearVelocity();
						glm::vec3 angularVelocity = actor->GetAngularVelocity();
						float maxAngularVelocity = actor->GetMaxAngularVelocity();
						UI::AttributeDrag("Linear Velocity", linearVelocity);
						UI::AttributeDrag("Max Linear Velocity", maxLinearVelocity);
						UI::AttributeDrag("Angular Velocity", angularVelocity);
						UI::AttributeDrag("Max Angular Velocity", maxAngularVelocity);

						float linearDrag = actor->GetLinearDrag();
						float angularDrag = actor->GetAngularDrag();
						UI::AttributeDrag("Linear Drag", linearDrag);
						UI::AttributeDrag("Angular Drag", angularDrag);

						UI::PopItemDisabled();
						UI::EndPropertyGrid();

						const auto& collisionShapes = actor->GetCollisionShapes();

						ImGui::Text("Shapes: %d", collisionShapes.size());

						for (const auto& shape : collisionShapes)
						{
							std::string shapeLabel = fmt::format("{0}##{1}", shape->GetShapeName(), entityID);
							if (UI::AttributeTreeNode(shapeLabel, false))
							{
								UI::BeginPropertyGrid();
								UI::PushItemDisabled();

								glm::vec3 offset = shape->GetCenter();
								bool isTrigger = shape->IsTrigger();

								UI::AttributeDrag("Offset", offset);
								UI::AttributeBool("Is Trigger", isTrigger);

								const auto& material = shape->GetMaterial();
								float staticFriction = material->GetStaticFriction();
								float dynamicFriction = material->GetDynamicFriction();
								float restitution = material->GetBounciness();

								UI::AttributeDrag("Static Friction", staticFriction);
								UI::AttributeDrag("Dynamic Friction", staticFriction);
								UI::AttributeDrag("Bounciness", restitution);

								UI::PopItemDisabled();
								UI::EndPropertyGrid();
								ImGui::TreePop();
							}
						}

						ImGui::TreePop();
					}
				}

				ImGui::TreePop();
			}
#if 0
			const auto& joints = m_PhysicsScene->GetJoints();

			if (UI::AttributeTreeNode(fmt::format("Joints ({0})", joints.size()), searchLength > 0))
			{
				for (const auto& [entityID, joint] : joints)
				{
					const auto& tag = joint->GetEntity().GetComponent<TagComponent>().Tag;

					if (searchLength > 0)
					{
						std::string searchBuffer = s_SearchBuffer;
						Utils::String::ToLower(searchBuffer);
						if (Utils::String::ToLowerCopy(tag).find(searchBuffer) == std::string::npos
							&& Utils::String::ToLowerCopy(joint->GetDebugName()).find(searchBuffer) == std::string::npos)
						{
							continue;
						}
					}

					std::string label = fmt::format("{0} ({1})##{1}", joint->GetDebugName(), tag, entityID);
					if (UI::PropertyGridHeader(label, false))
					{
						UI::BeginPropertyGrid();
						UI::BeginDisabled();

						bool isBreakable = joint->IsBreakable();
						UI::Property("Is Breakable", isBreakable);

						if (isBreakable)
						{
							bool isBroken = joint->IsBroken();
							UI::Property("Is Broken", isBroken);

							float breakForce, breakTorque;
							joint->GetBreakForceAndTorque(breakForce, breakTorque);
							UI::Property("Break Force", breakForce);
							UI::Property("Break Torque", breakTorque);
						}

						bool isCollisionEnabled = joint->IsCollisionEnabled();
						UI::Property("Is Collision Enabled", isCollisionEnabled);

						bool isPreProcessingEnabled = joint->IsPreProcessingEnabled();
						UI::Property("Is Preprocessing Enabled", isPreProcessingEnabled);

						UI::EndDisabled();
						UI::EndPropertyGrid();

						ImGui::TreePop();
					}
				}

				ImGui::TreePop();
			}
#endif
		}

		ImGui::End();
	}
}
