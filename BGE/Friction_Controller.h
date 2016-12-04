#pragma once
#include "PhysicsController.h"
#include <vector>

namespace BGE
{
	class Friction_Controller :
		public GameComponent
	{
	private:
		glm::vec3 myScale; // scale will get updated from the parents scale, so I use this variable to remember my scale
	public:
		Friction_Controller();
		~Friction_Controller();
		Friction_Controller(btCollisionShape * shape, btRigidBody * rigidBody, btMotionState * motionState);
		void SetPhysicsStuff(btCollisionShape * shape, btRigidBody * rigidBody, btMotionState * motionState);
		void Cleanup();
		const glm::vec3 g_value = glm::vec3(0, float(9.8), 0);
		btVector3 force;
		bool Initialise();
		void Update(float);
		btCollisionShape * shape;
		btRigidBody * rigidBody;
		btMotionState * motionState;
		bool tilt = false;
	};
}
