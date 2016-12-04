#include "GravityController.h"
#include "Game.h"

using namespace BGE;
using namespace std;

bool GravityController::Initialise()
{
	transform->velocity = glm::vec3(0, 0, 0);
	return true;
}

void GravityController::Update(float deltaTime)
{
	if (transform->position.y <= 0)
	{
		transform->position.y = 0;
		transform->velocity = float(-0.8) * transform->velocity;
	}

	transform->velocity -= float(deltaTime / 1000.0) * g_value;
	transform->position += transform->velocity;
}