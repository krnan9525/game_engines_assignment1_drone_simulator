#include "Friction_Controller.h"
#include "Game.h"
#include "Utils.h"
#include "FountainEffect.h"

using namespace BGE;
using namespace std;

bool Friction_Controller::Initialise()
{
	force = btVector3(0, 0, 0);
	transform->orientation = glm::angleAxis(90.0f, glm::vec3(0, 1, 0));
	return true;
}

void Friction_Controller::Update(float deltaTime)
{
	const Uint8 * keyState = Game::Instance()->GetKeyState();
	btTransform trans;
	btMotionState * ms = rigidBody->getMotionState();
	ms->getWorldTransform(trans);
	transform->position = BtToGLVector(trans.getOrigin());

	shared_ptr<GameComponent> obj1 = parent->parent->FindComponentByTag("BodyBox")->FindComponentByTag("box_controller");
	shared_ptr<PhysicsController> temp_obj = (dynamic_pointer_cast<PhysicsController> (obj1));

	if (tilt)
	{
		trans.setRotation(GLToBtQuat(temp_obj->transform->orientation));
		ms->setWorldTransform(trans);
		rigidBody->setMotionState(ms);
		transform->orientation = BtToGLQuat(trans.getRotation()); //glm::angleAxis(90.0f, glm::vec3(0, 1, 0)) *
		tilt = false;
	}
	else {
		transform->orientation = temp_obj->transform->orientation;
	}

	if (keyState[SDL_SCANCODE_UP])
	{
		force = btVector3(0, 0, 0);
		temp_obj->rigidBody->setLinearVelocity(GLToBtVector(temp_obj->transform->up) * 3);

		temp_obj->Update(deltaTime);
		//temp_obj->rigidBody->applyForce((deltaTime * 25000) * GLToBtVector(transform->up), temp_obj->rigidBody->getCenterOfMassPosition());
		//temp_obj->Update(deltaTime);
		//transform->orientation = glm::quat(0.7071067811865476f, 0.0f, -0.7071067811865476f, 0.0f);
		btTransform trans2;
		ms->getWorldTransform(trans2);
		trans2.setRotation(btQuaternion(0, 0, 0, 1));
		ms->setWorldTransform(trans2);
		//rigidBody->setMotionState(ms);
		vector< shared_ptr<GameComponent>>  obj2s = parent->parent->FindComponentsByTag("fountain");
		while (!obj2s.empty())
		{
			(dynamic_pointer_cast<FountainEffect> (obj2s.back()))->have_Effect = true;
			obj2s.pop_back();
		}
	}
	else
	{
		vector< shared_ptr<GameComponent>>  obj2s = parent->parent->FindComponentsByTag("fountain");
		while (!obj2s.empty())
		{
			(dynamic_pointer_cast<FountainEffect> (obj2s.back()))->have_Effect = false;
			obj2s.pop_back();
		}
	}
	if (keyState[SDL_SCANCODE_LEFT])
	{
		tilt = true;

		rigidBody->applyTorque(btVector3(0, 100, 0));
		temp_obj->rigidBody->applyTorque(btVector3(0, 100, 0));
	}
	else if (keyState[SDL_SCANCODE_RIGHT])
	{
		tilt = true;

		rigidBody->applyTorque(btVector3(0, -100, 0));
		temp_obj->rigidBody->applyTorque(btVector3(0, -100, 0));
	}
	if (keyState[SDL_SCANCODE_W])
	{
		//tilt = true;
		if (tag == "wheel2" || tag == "wheel3")
		{
			rigidBody->setLinearVelocity(rigidBody->getLinearVelocity() + 0.010f * GLToBtVector(temp_obj->transform->up));
		}
		if (tag == "wheel1" || tag == "wheel4")
		{
			rigidBody->setLinearVelocity(rigidBody->getLinearVelocity() + 0.000f * GLToBtVector(temp_obj->transform->up));
		}
	}
	else if (keyState[SDL_SCANCODE_S])
	{
		//tilt = true;
		if (tag == "wheel2" || tag == "wheel3")
		{
			rigidBody->setLinearVelocity(rigidBody->getLinearVelocity() + 0.000f * GLToBtVector(temp_obj->transform->up));
		}
		if (tag == "wheel1" || tag == "wheel4")
		{
			rigidBody->setLinearVelocity(rigidBody->getLinearVelocity() + 0.010f * GLToBtVector(temp_obj->transform->up));
		}
	}
	else if (keyState[SDL_SCANCODE_D])
	{
		//tilt = true;
		if (tag == "wheel4" || tag == "wheel3")
		{
			rigidBody->setLinearVelocity(rigidBody->getLinearVelocity() + 0.000f * GLToBtVector(temp_obj->transform->up));
		}
		if (tag == "wheel1" || tag == "wheel2")
		{
			rigidBody->setLinearVelocity(rigidBody->getLinearVelocity() + 0.010f * GLToBtVector(temp_obj->transform->up));
		}
	}
	else if (keyState[SDL_SCANCODE_A])
	{
		//tilt = true;
		if (tag == "wheel2" || tag == "wheel1")
		{
			rigidBody->setLinearVelocity(rigidBody->getLinearVelocity() + 0.000f * GLToBtVector(temp_obj->transform->up));
		}
		if (tag == "wheel3" || tag == "wheel4")
		{
			rigidBody->setLinearVelocity(rigidBody->getLinearVelocity() + 0.010f * GLToBtVector(temp_obj->transform->up));
		}
	}
	else
	{
		//rigidBody->setLinearVelocity(temp_obj->rigidBody->getLinearVelocity());
	}

	if (keyState[SDL_SCANCODE_R])
	{
		btTransform trans2;
		ms = temp_obj->rigidBody->getMotionState();
		ms->getWorldTransform(trans2);
		trans2.setRotation(btQuaternion(0, 0, 0, 1));
		ms->setWorldTransform(trans2);
		temp_obj->rigidBody->setMotionState(ms);
	}
	//transform->orientation = glm::angleAxis(90.0f, glm::vec3(0, 1, 0));
	//transform->position += transform->velocity * deltaTime;
	//this->rigidBody->applyForce(force, rigidBody->getCenterOfMassPosition());

	GameComponent::Update(deltaTime);
}

Friction_Controller::Friction_Controller()
{
	shape = nullptr;
	rigidBody = nullptr;
	motionState = nullptr;
}

Friction_Controller::Friction_Controller(btCollisionShape * shape, btRigidBody * rigidBody, btMotionState * motionState)
{
	this->shape = shape;
	this->rigidBody = rigidBody;
	if (this->rigidBody)
	{
		this->rigidBody->setUserPointer(this);
	}
	this->tag = "PhysicsController";
	this->motionState = motionState;
	//rigidBody->setMassProps(mass, btVector3(0.5, 0.5, 0.5));
}

void Friction_Controller::SetPhysicsStuff(btCollisionShape * shape, btRigidBody * rigidBody, btMotionState * motionState)
{
	this->shape = shape;
	this->rigidBody = rigidBody;
	this->rigidBody->setUserPointer(this);
	this->motionState = motionState;
}
Friction_Controller::~Friction_Controller(void)
{
	Game::Instance()->dynamicsWorld->removeRigidBody(rigidBody);
	SAFE_DELETE(rigidBody);
	SAFE_DELETE(motionState);
}

void Friction_Controller::Cleanup()
{
	GameComponent::Cleanup();
}