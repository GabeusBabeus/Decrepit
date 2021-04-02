#pragma once
#include <Box2D/Box2D.h>


class BasementListener : public b2ContactListener
{
public:
	BasementListener();



	void BeginContact(b2Contact* contact) override;
	void EndContact(b2Contact* contact) override;
private:
	void TriggerEnter(b2Fixture* sensor);
	void TriggerExit(b2Fixture* sensor);


};