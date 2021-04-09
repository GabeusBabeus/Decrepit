#pragma once

#include "Scene.h"
#include "Game.h"
#include <string>

class MainMenu : public Scene
{
public:
	MainMenu(std::string name);

	void InitScene(float windowWidth, float windowHeight) override;

	void Update() override;


	void setPointer(int ent);
	//Input overrides
	void KeyboardHold() override;
	void KeyboardDown() override;
	void KeyboardUp() override;

	int getSceneChange() override;
	void setSceneChange(int sc) override;


protected:
	int m_pointer;
	int pos = 0;

};

