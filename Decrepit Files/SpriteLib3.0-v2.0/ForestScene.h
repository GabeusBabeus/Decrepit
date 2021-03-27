#pragma once
#include "Scene.h"
#include "PhysicsPlaygroundListener.h"
#include <string>
#include "Game.h"

class ForestScene : public Scene
{
public:


	ForestScene(std::string name);

	void InitScene(float windowWidth, float windowHeight) override;

	void Update() override;


	void spawnLeftWall();
	void spawnGenericGround(int xGround, int yGround, int xGrass, int yGrass, int xGroundBox, int yGroundBox);
	void spawnGround();
	void twoWideGround();
	void spawnConnectorGround();
	void floatingGround();


	//Input overrides
	void KeyboardHold() override;
	void KeyboardDown() override;
	void KeyboardUp() override;

protected:

	PhysicsPlaygroundListener listener;

	
	bool m_firstWindow = false;
	bool m_secondWindow = false;

	int player, bridge, switchOne, switchTwo = 0;
	

	std::string m_fileInput;
	
};

