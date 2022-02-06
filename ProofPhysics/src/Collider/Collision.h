#pragma once
#include "../Body/RigidBody.h"
#include "Collider.h"
namespace ProofPhysicsEngine
{
	struct PotentialContact {
		/**
		* Rigid bodies that may come in contact
		*/
		RigidBody* body[2];
	};

	/*
	* class for nodes in boudign volume hereichy
	* uses binary tree
	*/
	template<class BoundingVolme>
	class BVHNode {
	public:
		/*
		* child node of htis node
		*/
		BVHNode* Children[2];
		/**
		* Holds a single bounding volume encompassing all the
		* descendents of this node.
		*/
		BoundingVolme volume;
		/**
		* rigid body at this node of the hierarchy.
		* Only leaf nodes can have a rigid body defined (see isLeaf).
		* Note that it is possible to rewrite the algorithms in this
		* class to handle objects at all levels of the hierarchy,
		* but the code provided ignores this vector unless firstChild
		* is NULL.
		*/
		RigidBody* body =nullptr;
		/*
		* Returns if the body has no child nodes 
		*/
		bool IsLeaf() const {	
			return body != nullptr;
		}
		/**
		* Checks the potential contacts from this node downward in
		* the hierarchy, writing them to the given array (up to the
		* given limit). Returns the number of potential contacts it
		* found.
		*/
		uint32_t GetPotentialContacts(PotentialContact* contacts,uint32_t limit) const {
			if (IsLeaf() || limit == 0)return;
			// Get the potential contacts of one of our children with
			
			return Children[0]->GetPotentialContactsWith(Children[1], contacts, limit);
		}
		uint32_t GetPotentialContactsWith(const BVHNode* other, PotentialContact* contacts, uint32_t limit) const {
			// Early-out if we don’t overlap or if we have no room
			// to report contacts.
			if (!Overlaps(other) || limit == 0) return 0;
			// If we’re both at leaf nodes, then we have a potential contact.
			if (IsLeaf() && other->IsLeaf()) {
				contacts->body[0] = body;
				contacts->body[1] = other->body;
				return 1;
			}
			// Determine which node to descend into. If either is
			// a leaf, then we descend the other. If both are branches,
			// then we use the one with the largest size.
			if (other->IsLeaf() ||
				(!IsLeaf() && volume.getSize() >= other->volume.getSize())) {
				// Recurse into ourself.
				uint32_t count = Children[0]->GetPotentialContactsWith(
					other, contacts, limit
				);
				// Check whether we have enough slots to do the other side too.
				if (limit > count) {
					return count + Children[1]->GetPotentialContactsWith(other, contacts + count, limit - count);
				}
				else {
					return count;
				}
			}
			else {
				// Recurse into the other node.
				uint32_t count = GetPotentialContactsWith(
					other->Children[0], contacts, limit
				);
				// Check whether we have enough slots to do the other side too.
				if (limit > count) {
					return count + GetPotentialContactsWith(
						other->Children[1], contacts + count, limit - count
					);
				}
				else {
					return count;
				}
			}
		}
		bool Overlaps(const BVHNode* other)const {
			return volume.Intersect(other->volume);
		}
	};
	
}
