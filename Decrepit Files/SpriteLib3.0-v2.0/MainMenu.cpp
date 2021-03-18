#include "MainMenu.h"
#include "Utilities.h"

MainMenu::MainMenu(std::string name) : Scene(name)
{
	m_gravity = b2Vec2(0.f, 0.f);
	m_physicsWorld->SetGravity(m_gravity);
}

void MainMenu::InitScene(float windowWidth, float windowHeight)
{
	//Dynamically allocates the register 
	m_sceneReg = new entt::registry;

	ECS::AttachRegister(m_sceneReg);

	float aspectRatio = windowWidth / windowHeight;
	m_pointer = 0;
	//Setup main camera entity
	{
		//Creates camera
		auto entity = ECS::CreateEntity();
		ECS::SetIsMainCamera(entity, true);

		//Creates new orthographic camera
		ECS::AttachComponent<Camera>(entity);
		ECS::AttachComponent<HorizontalScroll>(entity);
		ECS::AttachComponent<VerticalScroll>(entity);

		vec4 temp = vec4(-75.f, 75.f, -75.f, 75.f);
		ECS::GetComponent<Camera>(entity).SetOrthoSize(temp);
		ECS::GetComponent<Camera>(entity).SetWindowSize(vec2(float(windowWidth), float(windowHeight)));
		ECS::GetComponent<Camera>(entity).Orthographic(aspectRatio, temp.x, temp.y, temp.z, temp.w, -100.f, 100.f);

		ECS::GetComponent<HorizontalScroll>(entity).SetCam(&ECS::GetComponent<Camera>(entity));
		ECS::GetComponent<VerticalScroll>(entity).SetCam(&ECS::GetComponent<Camera>(entity));
	}

	//Setup helloworld sign
	{
		auto entity = ECS::CreateEntity();

		ECS::AttachComponent<Sprite>(entity);
		ECS::AttachComponent<Transform>(entity);

		std::string fileName = "Tower Inside/Base/Menu/Title_Image.png";
		ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 290,185);
		ECS::GetComponent<Sprite>(entity).SetTransparency(1.f);
		ECS::GetComponent<Transform>(entity).SetPosition(vec3(0.f, 0.f, -1.f));
	}
	//Setup helloworld sign
	{
		auto entity = ECS::CreateEntity();

		ECS::AttachComponent<Sprite>(entity);
		ECS::AttachComponent<Transform>(entity);

		std::string fileName = "Tower Inside/Base/Menu/NewGameButtonEng.png";
		ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 48, 11);
		ECS::GetComponent<Sprite>(entity).SetTransparency(1.f);
		ECS::GetComponent<Transform>(entity).SetPosition(vec3(-40.f, -45.f, 0.f));
	}
	//Setup helloworld sign
	{
		auto entity = ECS::CreateEntity();

		ECS::AttachComponent<Sprite>(entity);
		ECS::AttachComponent<Transform>(entity);

		std::string fileName = "Tower Inside/Base/Menu/ExitButtonEng.png";
		ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 48, 11);
		ECS::GetComponent<Sprite>(entity).SetTransparency(1.f);
		ECS::GetComponent<Transform>(entity).SetPosition(vec3(60.f, -45.f, 0.f));
	}
	//Pointer
	{
		auto entity = ECS::CreateEntity();
		m_pointer = entity;
		ECS::AttachComponent<Sprite>(entity);
		ECS::AttachComponent<Transform>(entity);

		std::string fileName = "Tower Inside/Base/Menu/TitleArrow.png";
		ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 32, 22);
		ECS::GetComponent<Sprite>(entity).SetTransparency(1.f);
		ECS::GetComponent<Transform>(entity).SetPosition(vec3(-80.f, -45.f, 0.f)); 
	}
	//star animation
	{
		auto entity = ECS::CreateEntity();
		ECS::AttachComponent<Sprite>(entity);
		ECS::AttachComponent<Transform>(entity);

		std::string fileName = "Tower Inside/Base/Filling/Blinding_Light.png";
		ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 32, 32);
		ECS::GetComponent<Sprite>(entity).SetTransparency(1.f);
		ECS::GetComponent<Transform>(entity).SetPosition(vec3(50.f, 0.f, 0.f));
	}

	

	ECS::GetComponent<HorizontalScroll>(MainEntities::MainCamera()).SetFocus(&ECS::GetComponent<Camera>(MainEntities::MainCamera()));
	ECS::GetComponent<VerticalScroll>(MainEntities::MainCamera()).SetFocus(&ECS::GetComponent<Camera>(MainEntities::MainCamera()));
}


 
void MainMenu::Update()
{

	


}

void MainMenu::KeyboardHold()
{

}

void MainMenu::KeyboardDown()
{
	auto& pointer = ECS::GetComponent<Transform>(m_pointer);
	
	if (pos == 0) {
		if (Input::GetKeyDown(Key::D)) {
			pointer.SetPosition(vec3(pointer.GetPositionX() + 100.f, -40, 0.f));
			pos = 1;

		}
	}
	if (pos == 1) {
		if (Input::GetKeyDown(Key::A)) {
			pointer.SetPosition(vec3(pointer.GetPositionX() - 100.f, -40.f, 0.f));
			pos = 0;
		}
	}


}

void MainMenu::KeyboardUp()
{
}
