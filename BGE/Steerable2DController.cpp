#include "Steerable2DController.h"
#include "Game.h"
#include "Utils.h"

using namespace BGE;
using namespace std;

Steerable2DController::Steerable2DController(float m)
{
	mass = m;
}

void Steerable2DController::Update(float timeD)
{
	const Uint8 * keyState = Game::Instance()->GetKeyState();
	if (keyState[SDL_SCANCODE_8])
	{
		//ship2->transform->Walk(speed * deltaTime);
		f_accumulator += Game::Instance()->camera.get()->transform->look * damping;
	}
	if (keyState[SDL_SCANCODE_2])
	{
		//ship2->transform->Walk(-speed * deltaTime);
		f_accumulator -= Game::Instance()->camera.get()->transform->look* damping;
	}
	if (keyState[SDL_SCANCODE_4])
	{
		//ship2->transform->Yaw(deltaTime * speed * speed);
		f_accumulator -= Game::Instance()->camera.get()->transform->right* damping;
	}
	if (keyState[SDL_SCANCODE_6])
	{
		//ship2->transform->Yaw(-deltaTime * speed * speed);
		f_accumulator += Game::Instance()->camera.get()->transform->right* damping;
	}
	transform->velocity += f_accumulator / mass;
}