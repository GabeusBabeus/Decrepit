#include "ForestSceneListener.h"
#include "ECS.h"


ForestSceneListener::ForestSceneListener()
	:b2ContactListener()
{

}

void ForestSceneListener::BeginContact(b2Contact* contact) {

	b2Fixture* fixtureA = contact->GetFixtureA();
	b2Fixture* fixtureB = contact->GetFixtureB();

	bool sensorA = fixtureA->IsSensor();
	bool sensorB = fixtureB->IsSensor();


	//If neither or both are sensors, will be false
	if (!(sensorA ^ sensorB)) {

		if (sensorA)
		{
			TriggerObject(fixtureA);
		}
		else if (sensorB)
		{
			TriggerObject(fixtureB);
		}

	}

}

void ForestSceneListener::EndContact(b2Contact* contact) {

}

void ForestSceneListener::TriggerObject(b2Fixture* sensor) {

	int entity = (int)sensor->GetUserData();

	ECS::GetComponent<Trigger>(entity).OnTrigger();
}