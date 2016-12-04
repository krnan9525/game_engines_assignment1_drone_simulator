#pragma once
#include "Game.h"
#include "PhysicsController.h"
#include "PhysicsFactory.h"
#include "FountainEffect.h"
#include <btBulletDynamicsCommon.h>

namespace BGE
{
	class PhysicsGame1 :
		public Game
	{
	private:

	public:
		PhysicsGame1(void);
		~PhysicsGame1(void);
		bool Initialise();
		void Update(float timeDelta);
		void Cleanup();
		void CreateWall();
		shared_ptr<PhysicsController> box1;
		shared_ptr<FountainEffect> centFountain1;
		shared_ptr<FountainEffect> centFountain2;
		shared_ptr<FountainEffect> centFountain3;
		shared_ptr<FountainEffect> centFountain4;
		shared_ptr<Friction_Controller> cyl1;
		shared_ptr<Friction_Controller> cyl2;
		shared_ptr<Friction_Controller> cyl3;
		shared_ptr<Friction_Controller> cyl4;
	};
}
