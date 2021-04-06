#pragma once
#include "Scene.h"
#include "ForestSceneListener.h"
#include <string>
#include "Game.h"

class ForestScene : public Scene
{
public:


	ForestScene(std::string name);

	void InitScene(float windowWidth, float windowHeight) override;

	void Update() override;


	void spawnLeftWall();
	void spawnSprite(int xGround, int yGround, int xGrass, int yGrass, int grassAmount, int xGroundBox, int yGroundBox, int boxX, int boxY, int xSprite, int ySprite, int depth, std::string fileName);
	void spawnGround();
	
	



	//Input overrides
	void KeyboardHold() override;
	void KeyboardDown() override;
	void KeyboardUp() override;

protected:

	ForestSceneListener listener;
	
	bool m_firstWindow = false;
	bool m_secondWindow = false;

	int player, bridge, switchOne, switchTwo = 0;
	

	std::string m_fileInput;
	
};

