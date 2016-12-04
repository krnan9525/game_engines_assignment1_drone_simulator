#pragma once
#include "GameComponent.h"
#include <vector>

namespace BGE
{
	class GravityController :
		public GameComponent
	{
	private:
		glm::vec3 myScale; // scale will get updated from the parents scale, so I use this variable to remember my scale
	public:

		const glm::vec3 g_value = glm::vec3(0, float(9.8), 0);
		bool Initialise();
		void Update(float timeD);
	};
}
