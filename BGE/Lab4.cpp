#include "Lab4.h"
#include "Content.h"
#include "VectorDrawer.h"
#include "LazerBeam.h"
#include "FountainEffect.h"

using namespace BGE;
using namespace glm;

Lab4::Lab4(void)
{
	elapsed = 10000;
}

bool Lab4::Initialise()
{
	std::shared_ptr<GameComponent> ground = make_shared<Ground>();
	Attach(ground);

	ship1 = make_shared<GameComponent>(true);
	ship1->Attach(Content::LoadModel("cobramk3", glm::rotate(glm::mat4(1), 180.0f, glm::vec3(0, 1, 0))));
	ship1->transform->position = glm::vec3(-10, 2, -10);
	ship1->Attach(make_shared<VectorDrawer>());
	Attach(ship1);

	ship2 = make_shared<GameComponent>(true);
	ship2->Attach(Content::LoadModel("ferdelance", glm::rotate(glm::mat4(1), 180.0f, glm::vec3(0, 1, 0))));
	ship2->Attach(make_shared<VectorDrawer>());
	ship2->transform->diffuse = glm::vec3(1.0f, 0.0f, 0.0f);
	ship2->transform->specular = glm::vec3(1.2f, 1.2f, 1.2f);

	ship2->transform->position = glm::vec3(9, 6, -9);
	Attach(ship2);

	// make a circle of fountains

	fountainTheta = 0.0f;

	for (int i = 0; i < NUM_FOUNTAINS; i++)
	{
		// 500 in the constructor indicates the number of particles in the effect.
		// You may need to compile in release mode or reduce the number of particles to get an acceptable framerate
		shared_ptr<FountainEffect> centFountain = make_shared<FountainEffect>(100, true);
		fountains.push_back(centFountain);
		fountainTheta += glm::pi<float>() * 2 / NUM_FOUNTAINS;
		centFountain->transform->position.x = FOUNTAIN_RADIUS * glm::cos(glm::pi<float>() * 2 / NUM_FOUNTAINS + fountainTheta);
		centFountain->transform->position.z = FOUNTAIN_RADIUS * glm::sin(glm::pi<float>() * 2 / NUM_FOUNTAINS + fountainTheta);
		centFountain->transform->position.y = FOUNTAIN_HEIGHT;
		centFountain->transform->diffuse = glm::vec3(i / 4.0, i / 2.0, i / 3.0); // Sets the colour of the fountain

		Attach(centFountain);
	}

	Game::Initialise();

	camera->transform->position = glm::vec3(0, 4, 20);

	ground_height = ship2->transform->position.y;

	return true;
}

void Lab4::Update(float timeDelta)
{
	camera->transform->position = ship2->transform->position - (ship2->transform->look * float(15.0)) + glm::vec3(-1, 5, 0);
	camera->transform->orientation = ship2->transform->orientation * glm::quat(0.854, 0, 0, -0.520);

	//quat tempp = quat(vec3(acos(before_vector1.x), acos(before_vector1.y), acos(before_vector1.z)));
	//ship1->transform->orientation = tempp * glm::inverse(ship1->transform->orientation);
	//ship1->transform->world =

	// Movement of ship2
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
		ship2->transform->Yaw(timeDelta * speed * speed);
	}
	if (keyState[SDL_SCANCODE_RIGHT])
	{
		ship2->transform->Yaw(-timeDelta * speed * speed);
	}
	if (keyState[SDL_SCANCODE_J] && !is_jumping)
	{
		ship2->transform->velocity.y = 22.0;
		is_jumping = true;
		ship2->transform->position.y += 0.01;
	}

	// Put code in here to control the height of the fountains....
	// Use fountainTheta
	for (int i = 0; i < fountains.size(); i++)
	{
		fountains[i]->transform->position.y = FOUNTAIN_HEIGHT * sin(fountainTheta) + FOUNTAIN_HEIGHT;
		fountainTheta += glm::pi<float>() * 0.001f;
	}
	fountainTheta += timeDelta;
	if (fountainTheta >= glm::pi<float>() * 2.0f)
	{
		fountainTheta = 0.0f;
	}

	Game::Update(timeDelta);

	// Put your code here to calculate the world transform matrix for ship1
	// You need to include the rotation bit
	//ship1->transform->world = glm::translate(glm::mat4(1), ship1->transform->position);

	glm::vec3 before_vector1 = normalize(ship2->transform->position - ship1->transform->position);
	vec3 look = ship1->transform->look;
	vec3 cros_pro = cross(look, before_vector1);
	quat just_try(sqrt((length(before_vector1) *length(before_vector1)) * (length(look)* length(look))) + dot(look, before_vector1), cros_pro.x, cros_pro.y, cros_pro.z);
	float th = re_theta(ship1->transform->look, before_vector1);
	//float th2 = re_theta(ship1->transform->look, before_vector1);
	vec3 axis = cross(before_vector1, ship1->transform->look);
	//mat4 rot = rotate(mat4(1.0), th, glm::abs(axis));
	//mat4 pos = translate(mat4(1.0), ship1->transform->position);
	//ship1->transform->Pitch(0);
	if (dot(normalize(look), normalize(before_vector1)) < 0.999 && dot(normalize(look), normalize(before_vector1)) > -0.999)
	{
		quat temp_quat = angleAxis(th, normalize(axis));
		//quat temp_quat2 = angleAxis(float(12), normalize(glm::vec3(0, 1, 0)));

		ship1->transform->orientation = ship1->transform->TransformOrientation(temp_quat); //normalize(just_try);//
		//ship1->transform->world = ship1->transform->world * rot; }
	}
	if (is_jumping)
	{
		ship2->transform->velocity.y += timeDelta * gravity;
		//ship2->transform->position.y = timeDelta *
		if (ship2->transform->position.y <= ground_height)
		{
			ship2->transform->position.y = ground_height;
			is_jumping = false;
			ship2->transform->velocity.y = 0;
		}
	}
	ship2->transform->position += timeDelta * ship2->transform->velocity;
}
float Lab4::re_theta(glm::vec3 u, glm::vec3 v)
{
	float cos_theta = glm::dot(glm::normalize(u), glm::normalize(v));
	float angle = glm::acos(cos_theta);
	if (glm::cross(u, v).y > 0)
		angle = -angle;
	return glm::degrees(angle);
}