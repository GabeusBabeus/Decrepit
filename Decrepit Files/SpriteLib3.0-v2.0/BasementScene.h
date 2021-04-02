#pragma once
#include "Scene.h"
#include "BasementSceneListener.h"
#include <string>

class BasementScene : public Scene
{
public:

	BasementScene(std::string name);

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

	BasementSceneListener listener;
	bool doorOpen = false;



};