#pragma once
#include "Scene.h"
#include "TopLevelListener.h"
#include <string>

class TopLevelScene : public Scene
{
public:

	TopLevelScene(std::string name);

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

	TopLevelListener listener;
	bool doorOpen = false;



};