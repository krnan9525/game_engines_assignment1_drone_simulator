#include "Lab3.h"
#include "Content.h"
#include "VectorDrawer.h"
#include "LazerBeam.h"

using namespace BGE;
using namespace glm;

Lab3::Lab3(void)
{
	elapsed = 10000;
}

bool Lab3::Initialise()
{
	std::shared_ptr<GameComponent> ground = make_shared<Ground>();
	Attach(ground);

	ship1 = make_shared<GameComponent>(true);
	ship1->Attach(Content::LoadModel("cobramk3", glm::rotate(glm::mat4(1), 180.0f, glm::vec3(0, 1, 0))));
	ship1->Attach(make_shared<VectorDrawer>(glm::vec3(5, 5, 5)));
	ship1->transform->position = glm::vec3(-10, 2, -10);
	Attach(ship1);

	ship2 = make_shared<GameComponent>(true);
	ship2->Attach(Content::LoadModel("python", glm::rotate(glm::mat4(1), 180.0f, glm::vec3(0, 1, 0))));
	ship2->Attach(make_shared<VectorDrawer>(glm::vec3(5, 5, 5)));
	ship2->transform->position = glm::vec3(9, 3, -9);
	Attach(ship2);
	Game::Initialise();

	camera->transform->position = glm::vec3(0, 4, 20);
	return true;
}

quat fromaxisangle(float angle, vec3 axis)
{
	float half_sin = sin(0.5f * angle);
	float half_cos = cos(0.5f * angle);
	return quat(half_cos,
		half_sin * axis.x,
		half_sin * axis.y,
		half_sin * axis.z);
}

quat fromtwovectors(vec3 u, vec3 v)
{
	float cos_theta = dot(normalize(u), normalize(v));
	float angle = acos(cos_theta);
	vec3 w = normalize(cross(u, v));
	return fromaxisangle(angle, w);
}

float re_theta(vec3 u, vec3 v)
{
	float cos_theta = dot(normalize(u), normalize(v));
	float angle = acos(cos_theta);
	if (dot(normalize(u), normalize(v)) < 0)
		angle = -angle;
	return degrees(angle);
}

void Lab3::Update(float timeDelta)
{
	static float timeToFire = 0.2f / 2.0f;

	static float time_past = 0;

	time_past += timeDelta;

	camera->transform->position = ship2->transform->position - (ship2->transform->look * float(15.0)) + glm::vec3(-1, 5, 0);
	camera->transform->orientation = ship2->transform->orientation * glm::quat(0.854, 0, 0, -0.520);

	glm::vec3 before_vector1 = normalize(ship2->transform->position - ship1->transform->position);

	//quat tempp = quat(vec3(acos(before_vector1.x), acos(before_vector1.y), acos(before_vector1.z)));
	//ship1->transform->orientation = tempp * glm::inverse(ship1->transform->orientation);
	//ship1->transform->world =
	float th = re_theta(ship1->transform->look, before_vector1);
	vec3 axis = normalize(cross(ship1->transform->look, before_vector1));
	mat4 rot = glm::rotate(mat4(1), th, axis);
	mat4 pos = translate(mat4(1), ship1->transform->position);
	ship1->transform->world = ship1->transform->world * pos * rot;
	//ship1->transform->orientation = quat(glm::rotate(mat4(1), degrees(re_theta(ship1->transform->look, before_vector1)), cross(ship1->transform->look, before_vector1)));

	if (keyState[SDL_SCANCODE_UP])
	{
		ship2->transform->position += ship2->transform->look * speed * timeDelta;
	}
	if (keyState[SDL_SCANCODE_DOWN])
	{
		ship2->transform->position -= ship2->transform->look * speed * timeDelta;
	}
	if (keyState[SDL_SCANCODE_LEFT])
	{
		ship2->transform->Yaw(1);
	}
	if (keyState[SDL_SCANCODE_RIGHT])
	{
		ship2->transform->Yaw(-1);
	}
	if (keyState[SDL_SCANCODE_U])
	{
		ship1->transform->position += ship1->transform->look * speed * timeDelta;
	}
	if (keyState[SDL_SCANCODE_J])
	{
		ship1->transform->position -= ship1->transform->look * speed * timeDelta;
	}
	if (keyState[SDL_SCANCODE_H])
	{
		ship1->transform->Yaw(1);
	}
	if (keyState[SDL_SCANCODE_K])
	{
		ship1->transform->Yaw(-1);
	}

	if (glm::length(ship1->transform->position - ship2->transform->position) < 5)
	{
		Game::PrintText("HELLO", glm::vec2(0, 0));
	}

	if (glm::dot(ship2->transform->look, ship1->transform->position - ship2->transform->position) > 0)
	{
		Game::PrintText("FRONT", glm::vec2(0, 0));
	}
	else
	{
		Game::PrintText("BACK", glm::vec2(0, 0));
	}

	if ((glm::dot(glm::normalize(ship2->transform->look), glm::normalize(ship1->transform->position - ship2->transform->position))) > 0.707 &&
		timeToFire <= time_past)
	{
		time_past = 0;
		//Game::PrintText("IN", glm::vec2(300, 10));
		shared_ptr<LazerBeam> lazer = make_shared<LazerBeam>();
		lazer->transform->position = ship2->transform->position;
		lazer->transform->look = ship2->transform->look;
		Attach(lazer);
	}

	elapsed += timeDelta;

	Game::Update(timeDelta);
}