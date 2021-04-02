#include "BasementListener.h"

#include "ECS.h"
#include "Utilities.h"
using namespace std;
BasementListener::BasementListener()
	: b2ContactListener()
{

}


void BasementListener::BeginContact(b2Contact* contact)
{
	b2Fixture* fixtureA = contact->GetFixtureA();
	b2Fixture* fixtureB = contact->GetFixtureB();

	bool sensorA = fixtureA->IsSensor();
	bool sensorB = fixtureB->IsSensor();

	//if neither or both are sensors, will be false
	if ((sensorA ^ sensorB))
	{
		if (sensorA)
		{
			TriggerEnter(fixtureA);
		}
		else if (sensorB)
		{
			TriggerEnter(fixtureB);
		}
	}

	b2Filter filterA = fixtureA->GetFilterData();
	b2Filter filterB = fixtureB->GetFilterData();
	auto& playerJump = ECS::GetComponent<CanJump>((int)fixtureB->GetBody()->GetUserData());
	auto& player = ECS::GetComponent<PhysicsBody>(MainEntities::MainPlayer());
	auto& keyOne = ECS::GetComponent<PhysicsBody>(MainEntities::keyOne());
	auto& door = ECS::GetComponent<Door>(MainEntities::doorBasement());
	//auto& switchOneTransform = ECS::GetComponent<Sprite>(MainEntities::switchOne());
	//auto& switchOnePhysics = ECS::GetComponent<PhysicsBody>(MainEntities::switchOne());
	//auto& switchTwo = ECS::GetComponent<PhysicsBody>(MainEntities::switchTwo());
	//auto& bridgeOneTransform = ECS::GetComponent<Sprite>(MainEntities::bridgeOne());
	//auto& bridgeOnePhysics = ECS::GetComponent<PhysicsBody>(MainEntities::bridgeOne());
	//auto& bridgeTwo = ECS::GetComponent<Bridge>(MainEntities::bridgeTwo());

	if ((filterA.categoryBits == PLAYER && filterB.categoryBits == GROUND) || (filterB.categoryBits == PLAYER && filterA.categoryBits == GROUND))
	{
		if (filterA.categoryBits == PLAYER)
		{
			playerJump.m_canJump = true;
			playerJump.m_wallJumpNum = 1;
		}
		else if (filterB.categoryBits == PLAYER)
		{
			playerJump.m_canJump = true;
			playerJump.m_wallJumpNum = 1;
		}
	}
	/*if ((filterA.categoryBits == PLAYER && filterB.categoryBits == ENEMY) || (filterB.categoryBits == PLAYER && filterA.categoryBits == ENEMY))
	{
		if (filterA.categoryBits == PLAYER)
		{
			ECS::GetComponent<HorizontalScroll>(MainEntities::MainCamera()).setSpawnCam(true);
			player.SetVelocity(vec3(0, 0, 0));
			player.SetPosition(b2Vec2(-100, -40), true);
			player.GetBody()->ApplyLinearImpulseToCenter(b2Vec2(0.f, -1.f), true);
		}
		else if (filterB.categoryBits == PLAYER)
		{
			ECS::GetComponent<HorizontalScroll>(MainEntities::MainCamera()).setSpawnCam(true);
			player.SetVelocity(vec3(0, 0, 0));
			player.SetPosition(b2Vec2(-100, -40), true);
			player.GetBody()->ApplyLinearImpulseToCenter(b2Vec2(0.f, -1.f), true);
		}
	}*/

	if ((filterA.categoryBits == PLAYER && filterB.categoryBits == OBJECTS) || (filterB.categoryBits == PLAYER && filterA.categoryBits == OBJECTS))
	{
		if (filterA.categoryBits == PLAYER)
		{
			playerJump.m_canJump = false;
			playerJump.m_wallJumpNum = 1;
		}
		else if (filterB.categoryBits == PLAYER)
		{
			playerJump.m_canJump = false;
			playerJump.m_wallJumpNum = 1;
		}
	}


	if ((filterA.categoryBits == PLAYER && filterB.categoryBits == WALL) || (filterB.categoryBits == PLAYER && filterA.categoryBits == WALL))
	{
		if (filterA.categoryBits == PLAYER)
		{
			cout << "TEST";
			playerJump.m_wallJump = true;
			if (player.GetPosition().x > fixtureB->GetBody()->GetPosition().x) {
				playerJump.m_facingRight = true;
			}
			else {
				playerJump.m_facingRight = false;
			}

		}
		else if (filterB.categoryBits == PLAYER)
		{
			cout << "TEST";
			playerJump.m_wallJump = true;
			if (player.GetPosition().x > fixtureA->GetBody()->GetPosition().x) {
				playerJump.m_facingRight = true;
			}
			else {
				playerJump.m_facingRight = false;
			}

		}
	}

	//if ((filterA.categoryBits == PLAYER && filterB.categoryBits == TRIGGER) || (filterB.categoryBits == PLAYER && filterA.categoryBits == TRIGGER))
	//{

	//	std::string switchOn = "ForestTiles/Interactables/Switch_On.png";
	//	//std::string bridgeDown = "ForestTiles/Interactables/Bridge_Down.png";

	//	if (filterA.categoryBits == TRIGGER)
	//	{
	//		
	//		switchOneTransform.LoadSprite(switchOn, 64, 64);
	//		//bridgeOneTransform.LoadSprite(bridgeDown, 360, 64);
	//		
	//	}
	//	else if (filterB.categoryBits == TRIGGER)
	//	{
	//	
	//		switchOneTransform.LoadSprite(switchOn, 64, 64);
	//		//bridgeOneTransform.LoadSprite(bridgeDown, 360, 64);
	//		
	//	}
	//}


	if ((filterA.categoryBits == PLAYER && filterB.categoryBits == LADDER) || (filterB.categoryBits == PLAYER && filterA.categoryBits == LADDER))
	{
		if (filterA.categoryBits == PLAYER)
		{
			player.SetPosition(b2Vec2(0, 400), true);


		}
		else if (filterB.categoryBits == PLAYER)
		{
			player.SetPosition(b2Vec2(0, 400), true);
		}

	}
	if ((filterA.categoryBits == PLAYER && filterB.categoryBits == KEY) || (filterB.categoryBits == PLAYER && filterA.categoryBits == KEY))
	{
		if (filterA.categoryBits == PLAYER)
		{
			keyOne.SetPosition(b2Vec2(-100, -400), true);
			door.haveKey = true;

		}
		else if (filterB.categoryBits == PLAYER)
		{
			keyOne.SetPosition(b2Vec2(-100, -400), true);
			door.haveKey = true;

		}
	}
	
	


}

void BasementListener::EndContact(b2Contact* contact)
{
	b2Fixture* fixtureA = contact->GetFixtureA();
	b2Fixture* fixtureB = contact->GetFixtureB();

	bool sensorA = fixtureA->IsSensor();
	bool sensorB = fixtureB->IsSensor();

	//if neither or both are sensors, will be false
	if ((sensorA ^ sensorB))
	{
		if (sensorA)
		{
			TriggerExit(fixtureA);
		}
		else if (sensorB)
		{
			TriggerExit(fixtureB);
		}
	}
	ECS::GetComponent<CanJump>((int)fixtureB->GetBody()->GetUserData()).m_wallJump = false;
}

void BasementListener::TriggerEnter(b2Fixture* sensor)
{
	int entity = (int)sensor->GetBody()->GetUserData();

	ECS::GetComponent<Trigger*>(entity)->OnEnter();
}

void BasementListener::TriggerExit(b2Fixture* sensor)
{
	int entity = (int)sensor->GetBody()->GetUserData();

	ECS::GetComponent<Trigger*>(entity)->OnExit();
}
