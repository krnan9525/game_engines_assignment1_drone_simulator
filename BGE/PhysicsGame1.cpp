#include "PhysicsGame1.h"
#include "PhysicsController.h"
#include "Sphere.h"
#include "PhysicsCamera.h"
#include "Box.h"
#include "Cylinder.h"
#include "Steerable3DController.h"
#include "Ground.h"
#include "Content.h"
#include <btBulletDynamicsCommon.h>
#include <gtc/quaternion.hpp>
#include <gtx/quaternion.hpp>
#include <gtx/euler_angles.hpp>
#include <gtx/norm.hpp>
#include "Friction_Controller.h"
#include "VectorDrawer.h"
#include "Utils.h"

using namespace BGE;

PhysicsGame1::PhysicsGame1(void)
{
}

PhysicsGame1::~PhysicsGame1(void)
{
}

std::shared_ptr<GameComponent> station;

bool PhysicsGame1::Initialise()
{
	//physicsFactory->CreateCameraPhysics();

	//shared_ptr<PhysicsController> box1 = physicsFactory->CreateBox(1, 1, 4, glm::vec3(5, 5, 0), glm::quat());
	//shared_ptr<PhysicsController> box2 = physicsFactory->CreateBox(1, 1, 4, glm::vec3(5, 5, 5), glm::quat());

	// A hinge
	//btHingeConstraint *hinge = new btHingeConstraint(*box1->rigidBody, *box2->rigidBody, btVector3(0, 0, 2.5f), btVector3(0, 0, -2.5f), btVector3(0, 1, 0), btVector3(0, 1, 0), true);
	//dynamicsWorld->addConstraint(hinge);

	/*
	2 -------- 1
	|          |
	3 -------- 4
	*/

	// Another hinge
	box1 = physicsFactory->CreateBox(8, 0.7, 6, glm::vec3(15, 10, 0), glm::quat(0, 0, 0, 1));
	cyl1 = physicsFactory->CreateCylinder2(1, 0.7, glm::vec3(15, 5, -5), glm::angleAxis(90.0f, glm::vec3(1, 0, 0)), "wheel1");
	btPoint2PointConstraint *hinge1 = new  btPoint2PointConstraint(*box1->rigidBody, *cyl1->rigidBody, btVector3(4.0, -1, -3.0), btVector3(0, 0, 0));
	//btPoint2PointConstraint *pointConstraint = new btPoint2PointConstraint(*box1->rigidBody, *cyl->rigidBody, btVector3(4.0, 0, 4.0), btVector3(0, 0, 0));
	//dynamicsWorld->addConstraint(pointConstraint);

	cyl2 = physicsFactory->CreateCylinder2(1, 0.7, glm::vec3(15, 5, -5), glm::angleAxis(90.0f, glm::vec3(1, 0, 0)), "wheel2");
	btPoint2PointConstraint *hinge2 = new btPoint2PointConstraint(*box1->rigidBody, *cyl2->rigidBody, btVector3(-4.0, -1, -3.0), btVector3(0, 0, 0));
	//pointConstraint = new btPoint2PointConstraint(*box1->rigidBody, *cyl->rigidBody, btVector3(4.0, 0, 4.0), btVector3(0, 0, 0));
	//dynamicsWorld->addConstraint(pointConstraint);

	cyl3 = physicsFactory->CreateCylinder2(1, 0.7, glm::vec3(15, 5, -5), glm::angleAxis(90.0f, glm::vec3(1, 0, 0)), "wheel3");
	btPoint2PointConstraint *hinge3 = new btPoint2PointConstraint(*box1->rigidBody, *cyl3->rigidBody, btVector3(-4.0, -1, 3.0), btVector3(0, 0, 0));
	//pointConstraint = new btPoint2PointConstraint(*box1->rigidBody, *cyl->rigidBody, btVector3(4.0, 0, 4.0), btVector3(0, 0, 0));
	//dynamicsWorld->addConstraint(pointConstraint);

	cyl4 = physicsFactory->CreateCylinder2(1, 0.7, glm::vec3(15, 5, -5), glm::angleAxis(90.0f, glm::vec3(1, 0, 0)), "wheel4");
	btPoint2PointConstraint *hinge4 = new btPoint2PointConstraint(*box1->rigidBody, *cyl4->rigidBody, btVector3(4.0, -1, 3.0), btVector3(0, 0, 0));
	//pointConstraint = new btPoint2PointConstraint(*box1->rigidBody, *cyl->rigidBody, btVector3(4.0, 0, 4.0), btVector3(0, 0, 0));
	//dynamicsWorld->addConstraint(pointConstraint);
	dynamicsWorld->addConstraint(hinge4);
	dynamicsWorld->addConstraint(hinge3);
	dynamicsWorld->addConstraint(hinge2);
	dynamicsWorld->addConstraint(hinge1);
	physicsFactory->CreateGroundPhysics();
	dynamicsWorld->setGravity(btVector3(0, -9.8, 0));
	centFountain1 = make_shared<FountainEffect>(50, true);
	Attach(centFountain1);
	centFountain2 = make_shared<FountainEffect>(50, true);
	Attach(centFountain2);
	centFountain3 = make_shared<FountainEffect>(50, true);
	Attach(centFountain3);
	centFountain4 = make_shared<FountainEffect>(50, true);
	Attach(centFountain4);

	if (!Game::Initialise()) {
		return false;
	}

	camera->transform->position = glm::vec3(-20, 10, 0);

	return true;
}

void BGE::PhysicsGame1::Update(float timeDelta)
{
	camera->transform->position = box1->transform->position + glm::vec3(-40, 5, 0);
	camera->transform->orientation = glm::quat(0.612, 0.354, -0.612, -0.354);

	centFountain1->transform->position = cyl1->transform->position;
	centFountain2->transform->position = cyl2->transform->position;
	centFountain3->transform->position = cyl3->transform->position;
	centFountain4->transform->position = cyl4->transform->position;
	Game::Update(timeDelta);
}

void BGE::PhysicsGame1::Cleanup()
{
	Game::Cleanup();
}