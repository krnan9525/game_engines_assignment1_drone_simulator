#pragma once
#pragma once
#include "GameComponent.h"
#include <vector>

namespace BGE
{
	class Steerable2DController :
		public GameComponent
	{
	public:
		Steerable2DController(float m);
		float mass = 0.1;
		glm::vec3 f_accumulator;
		const float damping = 0.00001f;
		const glm::vec3 g_value = glm::vec3(0, float(9.8), 0);
		void Update(float timeD);
	};
}
