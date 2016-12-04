#include "Lab6.h"
#include "Content.h"
#include "VectorDrawer.h"
#include "GravityController.h"
#include "Steerable2DController.h"
#include "LazerBeam.h"
#include "FountainEffect.h"
#include "Utils.h"

using namespace BGE;

Lab6::Lab6(void)
{
	elapsed = 10000;
	turnRate = glm::half_pi<float>(); // Turn half_pi radians per second
}

Lab6::~Lab6(void)
{
}

bool Lab6::Initialise()
{
	std::shared_ptr<GameComponent> ground = make_shared<Ground>();
	Attach(ground);

	ship1 = make_shared<GameComponent>(true);
	ship1->Attach(Content::LoadModel("cobramk3", glm::rotate(glm::mat4(1), 180.0f, glm::vec3(0, 1, 0))));
	ship1->transform->position = glm::vec3(-10, 50, -10);
	ship1->Attach(make_shared<VectorDrawer>());
	ship1->Attach(make_shared<GravityController>());
	ship1->Attach(make_shared<Steerable2DController>(5000));
	Attach(ship1);

	ship2 = make_shared<GameComponent>(true);
	ship2->Attach(Content::LoadModel("ferdelance", glm::rotate(glm::mat4(1), 180.0f, glm::vec3(0, 1, 0))));
	ship2->Attach(make_shared<VectorDrawer>());
	ship2->Attach(make_shared<GravityController>());
	ship2->Attach(make_shared<Steerable2DController>(50000));
	ship2->transform->diffuse = glm::vec3(1.0f, 0.0f, 0.0f);
	ship2->transform->specular = glm::vec3(1.2f, 1.2f, 1.2f);

	ship2->transform->position = glm::vec3(10, 30, -10);
	Attach(ship2);

	slerping = false;
	t = 0.0f;

	Game::Initialise();

	camera->transform->position = glm::vec3(0, 4, 20);
	return true;
}

void Lab6::Update(float deltaTime)
{
	// Movement of ship2
	if (keyState[SDL_SCANCODE_UP])
	{
		ship2->transform->Walk(speed * deltaTime);
	}
	if (keyState[SDL_SCANCODE_DOWN])
	{
		ship2->transform->Walk(-speed * deltaTime);
	}
	if (keyState[SDL_SCANCODE_LEFT])
	{
		ship2->transform->Yaw(deltaTime * speed * speed);
	}
	if (keyState[SDL_SCANCODE_RIGHT])
	{
		ship2->transform->Yaw(-deltaTime * speed * speed);
	}

	if (keyState[SDL_SCANCODE_O])
	{
		ship2->transform->Fly(deltaTime * speed);
	}

	if (keyState[SDL_SCANCODE_L])
	{
		ship2->transform->Fly(-deltaTime * speed);
	}

	// Your code goes here...

	if (keyState[SDL_SCANCODE_SPACE] && !is_turning)
	{
		glm::vec3 look1 = ship1->transform->look;
		glm::vec3 direction = ship2->transform->position - ship1->transform->position;

		start_quat = ship1->transform->orientation;

		float angle = re_theta(look1, direction);
		glm::vec3 axis = glm::cross(look1, direction);
		if (glm::length(axis) != 0.0 && abs(angle) > 0.05)
		{
			axis = axis / glm::length(axis);
			glm::quat turning_quat = glm::angleAxis(angle, axis);
			end_quat = turning_quat *start_quat;
			if (glm::length(end_quat) < 1.01 && glm::length(end_quat) > 0.99) {
				turnRate = 0.1 / angle;
				is_turning = true;
			}
		}
	}

	if (is_turning)
	{
		ship1->transform->orientation = glm::mix(start_quat, end_quat, turning_stage);
		if (turning_stage >= 1)
		{
			is_turning = false;
			turning_stage = 0;
		}
		turning_stage += turnRate;
	}

	Game::Update(deltaTime);
}

float Lab6::re_theta(glm::vec3 u, glm::vec3 v)
{
	float cos_theta = glm::dot(u, v) / glm::length(u) / glm::length(v);

	float angle = glm::acos(cos_theta);
	angle = abs(angle);
	return glm::degrees(angle);
}