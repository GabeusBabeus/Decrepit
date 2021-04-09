#include "MainMenu.h"
#include "Utilities.h"

MainMenu::MainMenu(std::string name) : Scene(name)
{
	m_gravity = b2Vec2(0.f, 0.f);
	m_physicsWorld->SetGravity(m_gravity);
}

int MainMenu::getSceneChange() {
	return m_sceneChange;
}

void MainMenu::setSceneChange(int sc) {
	m_sceneChange = sc;
}


void MainMenu::InitScene(float windowWidth, float windowHeight)
{
	//Dynamically allocates the register 
	m_sceneReg = new entt::registry;
	m_physicsWorld = new b2World(m_gravity);

	m_gravity = b2Vec2(0.f, 0.f);
	m_physicsWorld->SetGravity(m_gravity);


	ECS::AttachRegister(m_sceneReg);

	float aspectRatio = windowWidth / windowHeight;
	m_pointer = 0;
	pos = 0;
	//Setup main camera entity
	{
		//Creates camera
		auto entity = ECS::CreateEntity();
		ECS::SetIsMainCamera(entity, true);

		//Creates new orthographic camera
		ECS::AttachComponent<Camera>(entity);
		//ECS::AttachComponent<HorizontalScroll>(entity);
		//ECS::AttachComponent<VerticalScroll>(entity);

		vec4 temp = vec4(-75.f, 175.f, -80.f, 175.f);
		ECS::GetComponent<Camera>(entity).SetOrthoSize(temp);
		ECS::GetComponent<Camera>(entity).SetWindowSize(vec2(float(windowWidth), float(windowHeight)));
		ECS::GetComponent<Camera>(entity).Orthographic(aspectRatio, temp.x, temp.y, temp.z, temp.w, -100.f, 100.f);

		//ECS::GetComponent<HorizontalScroll>(entity).SetCam(&ECS::GetComponent<Camera>(entity));
		//ECS::GetComponent<VerticalScroll>(entity).SetCam(&ECS::GetComponent<Camera>(entity));
	}

	//START MAINMENU
	{
		float moveX = 60;
		float moveY = 25;
		//main menu title image
		{
			auto entity = ECS::CreateEntity();

			ECS::AttachComponent<Sprite>(entity);
			ECS::AttachComponent<Transform>(entity);

			std::string fileName = "Tower Inside/Base/Menu/unknown.png";
			ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 300, 200);
			ECS::GetComponent<Sprite>(entity).SetTransparency(1.f);
			ECS::GetComponent<Transform>(entity).SetPosition(vec3(20.f + moveX, -35.f + moveY, 1.f));
		}
		{
			auto entity = ECS::CreateEntity();

			ECS::AttachComponent<Sprite>(entity);
			ECS::AttachComponent<Transform>(entity);

			std::string fileName = "WallTiles/Background.png";
			ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 600, 600);
			ECS::GetComponent<Sprite>(entity).SetTransparency(1.f);
			ECS::GetComponent<Transform>(entity).SetPosition(vec3(30.f + moveX, -35.f + moveY, 0.f));
		}
		
		{
			auto entity = ECS::CreateEntity();
			auto star = File::LoadJSON("Starlight.json");
			ECS::AttachComponent<Sprite>(entity);
			ECS::AttachComponent<Transform>(entity);
			ECS::AttachComponent<AnimationController>(entity);
			std::string fileName = "spritesheets/PNG/Stars_V3.png";

			auto& animController = ECS::GetComponent<AnimationController>(entity);
			animController.InitUVs(fileName);
			animController.AddAnimation(star["Starlight"]);
			animController.SetActiveAnim(0);
			ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 60, 82, true, &animController);

			ECS::GetComponent<Sprite>(entity).SetTransparency(1.f);
			ECS::GetComponent<Transform>(entity).SetPosition(vec3(125.f + moveX, 20.f + moveY, 2.f));
		}
		//NAME
		{
			auto entity = ECS::CreateEntity();
			auto name = File::LoadJSON("NAMESAKE.json");
			ECS::AttachComponent<Sprite>(entity);
			ECS::AttachComponent<Transform>(entity);
			ECS::AttachComponent<AnimationController>(entity);
			std::string fileName = "spritesheets/PNG/NAME.png";

			auto& animController = ECS::GetComponent<AnimationController>(entity);
			animController.InitUVs(fileName);
			animController.AddAnimation(name["NAME"]);
			animController.SetActiveAnim(0);
			ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 350, 180, true, &animController);

			ECS::GetComponent<Sprite>(entity).SetTransparency(1.f);
			ECS::GetComponent<Transform>(entity).SetPosition(vec3(0.f + moveX, 40.f + moveY, 5.f));
		}
		{
			auto idle = File::LoadJSON("IDR.json");
			auto entity = ECS::CreateEntity();

			//Add components
			ECS::AttachComponent<Sprite>(entity);
			ECS::AttachComponent<Transform>(entity);
			ECS::AttachComponent<AnimationController>(entity);

			//Sets up the components
			std::string fileName = "spritesheets/PNG/IdleR.png";

			ECS::GetComponent<Sprite>(entity).SetTransparency(1.f);
			ECS::GetComponent<Transform>(entity).SetPosition(vec3(-70.f + moveX, -10.f + moveY, 8.f));

			auto& animController = ECS::GetComponent<AnimationController>(entity);
			animController.InitUVs(fileName);
			animController.AddAnimation(idle["IDR"]);
			animController.SetActiveAnim(0);
			ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 30, 40, true, &animController);


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
			ECS::GetComponent<Transform>(entity).SetPosition(vec3(-95.f + moveX, -75.f + moveY, 2.f));
		}
	}
	

	

	//ECS::GetComponent<HorizontalScroll>(MainEntities::MainCamera()).SetFocus(&ECS::GetComponent<Camera>(MainEntities::MainCamera()));
	//ECS::GetComponent<VerticalScroll>(MainEntities::MainCamera()).SetFocus(&ECS::GetComponent<Camera>(MainEntities::MainCamera()));
}


 
void MainMenu::Update()
{



}

void MainMenu::KeyboardHold()
{

	auto& pointer = ECS::GetComponent<Transform>(m_pointer);

	
	if (Input::GetKeyDown(Key::D) && pos == 0) {
		pointer.SetPosition(vec3(pointer.GetPositionX() + 140.f, pointer.GetPositionY(), 2.f));
		pos = 1;
		std::cout << pos;
	}
	if (Input::GetKeyDown(Key::A) && pos == 1) {
		pointer.SetPosition(vec3(pointer.GetPositionX() - 140.f, pointer.GetPositionY(), 2.f));
		pos = 0;
		std::cout << pos;
		
	}
	if (Input::GetKeyDown(Key::Enter) && pos == 0) {
		Scene::setSceneChange(0);
	}
	else if (Input::GetKeyDown(Key::Enter) && pos == 1) {
		exit(1);
	}

	
}

void MainMenu::KeyboardDown()
{
	

	




}

void MainMenu::KeyboardUp()
{
}
