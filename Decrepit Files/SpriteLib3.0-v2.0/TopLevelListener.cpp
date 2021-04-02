#include "TopLevelListener.h"

#include "ECS.h"
#include "Utilities.h"
using namespace std;
TopLevelListener::TopLevelListener()
	: b2ContactListener()
{

}


void TopLevelListener::BeginContact(b2Contact* contact)
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



	

}

void TopLevelListener::EndContact(b2Contact* contact)
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

void TopLevelListener::TriggerEnter(b2Fixture* sensor)
{
	int entity = (int)sensor->GetBody()->GetUserData();

	ECS::GetComponent<Trigger*>(entity)->OnEnter();
}

void TopLevelListener::TriggerExit(b2Fixture* sensor)
{
	int entity = (int)sensor->GetBody()->GetUserData();

	ECS::GetComponent<Trigger*>(entity)->OnExit();
}
