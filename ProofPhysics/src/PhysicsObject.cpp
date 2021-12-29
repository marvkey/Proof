#include "PhysicsObject.h"
#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtc/type_ptr.hpp>
#include<glm/gtx/rotate_vector.hpp>
#include<glm/gtx/vector_angle.hpp>
#include<iostream>
namespace ProofPhysicsEngine {
	void PhysicsObject::Update(float delta){
		*Position += *Veclocity * delta;
	}
}