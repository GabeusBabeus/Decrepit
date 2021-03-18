#pragma once
#include "Scene.h"
#include "PhysicsPlaygroundListener.h"
#include <string>

class TowerGroundScene : public Scene
{
public:

	TowerGroundScene(std::string name);

	void InitScene(float windowWidth, float windowHeight) override;

	void Update() override;

	//Input overrides
	void KeyboardHold() override;
	void KeyboardDown() override;
	void KeyboardUp() override;

protected:

	bool m_firstWindow = false;
	bool m_secondWindow = false;

	std::string m_fileInput;

	PhysicsPlaygroundListener listener;

	int doubleJump = 0;
	int puzzleWall1;
	int puzzleWall2;

	

};