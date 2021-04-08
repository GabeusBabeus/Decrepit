#include "BasementSceneListener.h"

#include "ECS.h"
#include "Utilities.h"
using namespace std;
BasementSceneListener::BasementSceneListener()
	: b2ContactListener()
{

}


void BasementSceneListener::BeginContact(b2Contact* contact)
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
	//auto& keyOne = ECS::GetComponent<PhysicsBody>(MainEntities::keyOne());
	//auto& door = ECS::GetComponent<Door>(MainEntities::doorBasement());

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
	if ((filterA.categoryBits == PLAYER && filterB.categoryBits == ENEMY) || (filterB.categoryBits == PLAYER && filterA.categoryBits == ENEMY))
	{
		if (filterA.categoryBits == PLAYER)
		{
			player.SetVelocity(vec3(0, 0, 0));
			player.SetPosition(b2Vec2(-100, 15), true);
			player.GetBody()->ApplyLinearImpulseToCenter(b2Vec2(0.f, -1.f), true);
		}
		else if (filterB.categoryBits == PLAYER)
		{
			player.SetVelocity(vec3(0, 0, 0));
			player.SetPosition(b2Vec2(-100, 15), true);
			player.GetBody()->ApplyLinearImpulseToCenter(b2Vec2(0.f, 1.f), true);
		}
	}

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
	/*if ((filterA.categoryBits == PLAYER && filterB.categoryBits == KEY) || (filterB.categoryBits == PLAYER && filterA.categoryBits == KEY))
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
	}*/
	
	


}

void BasementSceneListener::EndContact(b2Contact* contact)
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

void BasementSceneListener::TriggerEnter(b2Fixture* sensor)
{
	int entity = (int)sensor->GetBody()->GetUserData();

	ECS::GetComponent<Trigger*>(entity)->OnEnter();
}

void BasementSceneListener::TriggerExit(b2Fixture* sensor)
{
	int entity = (int)sensor->GetBody()->GetUserData();

	ECS::GetComponent<Trigger*>(entity)->OnExit();
}
