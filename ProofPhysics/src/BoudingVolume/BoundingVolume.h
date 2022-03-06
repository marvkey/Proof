#pragma once
#include "../Body/RigidBody.h"
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
		BVHNode Children[2];
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
			// A leaf basically has no children
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

		void Insert(RigidBody& body, const BoundingVolme& volume) {
			// if leaf we have to creawte 2 new bounding volume
			if (IsLeaf()) {
				Children[0] = BVHNode<BoundingVolme>(this,volume,body); 
				Children[1] = BVHNode<BoundingVolme>(this, volume, body);

				// we are no longer leaf so we remove th body
				this->body = nullptr;
				RecalculateBoundingVolume();
				return;
			}
			// ELSE WE WORK OUT WITWH CHILD TO KEEP
			if(Children[0]->volume.GetGrowth(volume) < Children[1]->volume.GetGrowth(volume))
				Children[0]->Insert(body, volume)
			else
				Children[1]->Insert(body, volume)

		}
		/**
		* Deletes this node, removing it first from the hierarchy, along
		* with its associated rigid body and child nodes. This method
		* deletes the node and all its children (but obviously not the
		* rigid bodies). This also has the effect of deleting the sibling
		* of this node, and changing the parent node so that it contains
		* the data currently in that sibling. Finally it forces the
		* hierarchy above the current node to reconsider its bounding
		* volume.
		*/
		~BVHNode() {
			// If we don’t have a parent, then we ignore the sibling processing.
			if (parent) {
				// Find our sibling.
				BVHNode<BoundingVolumeClass>* sibling;
				if (parent->Children[0] == this) sibling = parent->Children[1];
				else sibling = parent->children[0];
				// Write its data to our parent.
				parent->volume = sibling->volume;
				parent->body = sibling->body;
				parent->children[0] = sibling->children[0];
				parent->children[1] = sibling->children[1];
				// Delete the sibling (we blank its parent and
				// children to avoid processing/deleting them).
				sibling->parent = NULL;
				sibling->body = NULL;
				sibling->children[0] = NULL;
				sibling->children[1] = NULL;
				delete sibling;
				// Recalculate the parent’s bounding volume.
				parent->RecalculateBoundingVolume();
			}
			
			// Delete our children (again we remove their parent data so
			// we don’t try to process their siblings as they are deleted).
			if (Children[0]) {
				Children[0]->parent = NULL;
				//delete Children[0];
			}
			if (Children[1]) {
				Children[1]->parent = NULL;
				//delete Children	[0];
			}
		}
	private:
		void RecalculateBoundingVolume() {};
	};

}
