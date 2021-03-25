#pragma once
#include "Scene.h"
#include "PhysicsPlaygroundListener.h"
#include <string>
#include "Game.h"

class ForestScene : public Scene, public Game
{
public:


	ForestScene(std::string name);

	void InitScene(float windowWidth, float windowHeight) override;

	void Update() override;

	


	//Input overrides
	void KeyboardHold() override;
	void KeyboardDown() override;
	void KeyboardUp() override;

protected:

	PhysicsPlaygroundListener listener;

	
	bool m_firstWindow = false;
	bool m_secondWindow = false;

	int player, bridge, lever_off, lever_on = 0;
	

	std::string m_fileInput;
	
};

