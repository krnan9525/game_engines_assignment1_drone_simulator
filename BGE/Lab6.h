#pragma once
#include "Game.h"
#include "GameComponent.h"
#include "FountainEffect.h"

using namespace std;

namespace BGE
{
	class Lab6 :
		public Game
	{
	public:
		Lab6(void);
		~Lab6(void);

		shared_ptr<GameComponent> ship1;
		shared_ptr<GameComponent> ship2;
		float turnRate;
		float toRotate;
		float elapsed;
		bool Initialise();
		void Update(float deltaTime);
		bool slerping;
		glm::quat fromQuaternion;
		glm::quat toQuaternion;
		float t;
		float re_theta(glm::vec3, glm::vec3);
		bool is_turning = false;
		float turning_stage = 0;
		glm::quat start_quat;
		glm::quat end_quat;
	};
}
#pragma once
