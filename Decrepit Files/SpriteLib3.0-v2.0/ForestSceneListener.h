#pragma once
#include <Box2D/Box2D.h>
#include "Scene.h"

class ForestSceneListener : public b2ContactListener
{
public:
	ForestSceneListener();

	void BeginContact(b2Contact* contact) override;
	void EndContact(b2Contact* contact) override;
private:
	void TriggerEnter(b2Fixture* sensor);
	void TriggerExit(b2Fixture* sensor);

	

};

