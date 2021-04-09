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

	int getSceneChange() override;
	void setSceneChange(int sc) override;


protected:

	bool m_firstWindow = false;
	bool m_secondWindow = false;

	std::string m_fileInput;

	PhysicsPlaygroundListener listener;
	bool doorOpen = false;
	int doubleJump = 0;
	int puzzleWall1;
	int puzzleWall2;
	int m_pointer = 0;
	int pos = 0;
	int cameraHolder = 0;
	int lockedLadder = 0;
	b2Vec2 leveloneSpawn = b2Vec2(-100, -60);
	b2Vec2 leveltwoSpawn = b2Vec2(1400, 20);

	bool m_lerpEnabled = false;
	float m_val1 = 0.f;
	float m_val2 = 0.f;
	float m_tVal = 0.f;
	float m_lerpVal = 0.f;




	

};