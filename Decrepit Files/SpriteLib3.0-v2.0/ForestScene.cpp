#include "ForestScene.h"

#include "Utilities.h"
#include <random>

ForestScene::ForestScene(std::string name)
	: Scene(name)
{
	//No gravity this is a top down scene
	m_gravity = b2Vec2(0.f, -98.f);
	m_physicsWorld->SetGravity(m_gravity);

	m_physicsWorld->SetContactListener(&listener);

}

void ForestScene::InitScene(float windowWidth, float windowHeight)
{
	//Dynamically allocates the register
	m_sceneReg = new entt::registry;

	//Attach the register
	ECS::AttachRegister(m_sceneReg);

	//Sets up aspect ratio for the camera
	float aspectRatio = windowWidth / windowHeight;



	//Setup MainCamera Entity
	{
		/*Scene::CreateCamera(m_sceneReg, vec4(-75.f, 75.f, -75.f, 75.f), -100.f, 100.f, windowWidth, windowHeight, true, true);*/

		//Creates Camera entity
		auto entity = ECS::CreateEntity();
		ECS::SetIsMainCamera(entity, true);

		//Creates new orthographic camera
		ECS::AttachComponent<Camera>(entity);
		ECS::AttachComponent<HorizontalScroll>(entity);
		ECS::AttachComponent<VerticalScroll>(entity);

		vec4 temp = vec4(-75.f, 175.f, -80.f, 175.f);
		ECS::GetComponent<Camera>(entity).SetOrthoSize(temp);
		ECS::GetComponent<Camera>(entity).SetWindowSize(vec2(float(windowWidth), float(windowHeight)));
		ECS::GetComponent<Camera>(entity).Orthographic(aspectRatio, temp.x, temp.y, temp.z, temp.w, -100.f, 100.f);

		//Attaches the camera to vert and horiz scrolls
		ECS::GetComponent<HorizontalScroll>(entity).SetCam(&ECS::GetComponent<Camera>(entity));
		ECS::GetComponent<VerticalScroll>(entity).SetCam(&ECS::GetComponent<Camera>(entity));
	}

	//Setup new Entity
	{
		/*Scene::CreateSprite(m_sceneReg, "HelloWorld.png", 100, 60, 0.5f, vec3(0.f, 0.f, 0.f));*/

		//Creates entity
		auto entity = ECS::CreateEntity();

		//Add components
		ECS::AttachComponent<Sprite>(entity);
		ECS::AttachComponent<Transform>(entity);

		//Set up the components
		std::string fileName = "ForestTiles/Background.png";
		ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 1754,1204);
		ECS::GetComponent<Sprite>(entity).SetTransparency(1.f);
		ECS::GetComponent<Transform>(entity).SetPosition(vec3(0.f, 50.f, 1.f));
	}

	//Setup new Entity
	{
		/*Scene::CreateSprite(m_sceneReg, "HelloWorld.png", 100, 60, 0.5f, vec3(0.f, 0.f, 0.f));*/

		//Creates entity
		auto entity = ECS::CreateEntity();

		//Add components
		ECS::AttachComponent<Sprite>(entity);
		ECS::AttachComponent<Transform>(entity);

		//Set up the components
		std::string fileName = "ForestTiles/Sky.png";
		ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 1280, 720);
		ECS::GetComponent<Sprite>(entity).SetTransparency(1.f);
		ECS::GetComponent<Transform>(entity).SetPosition(vec3(0.f, 100.f, 0.f));
	}

	

	//Link entity
	{
		/*Scene::CreatePhysicsSprite(m_sceneReg, "LinkStandby", 80, 60, 1.f, vec3(0.f, 30.f, 2.f), b2_dynamicBody, 0.f, 0.f, true, true)*/

		auto entity = ECS::CreateEntity();
		ECS::SetIsMainPlayer(entity, true);

		//Add components
		ECS::AttachComponent<Sprite>(entity);
		ECS::AttachComponent<Transform>(entity);
		ECS::AttachComponent<PhysicsBody>(entity);
		ECS::AttachComponent<CanJump>(entity);

		//Sets up the components
		std::string fileName = "Character.png";
		ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 20, 25);
		ECS::GetComponent<Sprite>(entity).SetTransparency(1.f);
		ECS::GetComponent<Transform>(entity).SetPosition(vec3(0.f, 30.f, 4.f));

		auto& tempSpr = ECS::GetComponent<Sprite>(entity);
		auto& tempPhsBody = ECS::GetComponent<PhysicsBody>(entity);

		float shrinkX = 0.f;
		float shrinkY = 0.f;

		b2Body* tempBody;
		b2BodyDef tempDef;
		tempDef.type = b2_dynamicBody;
		tempDef.position.Set(float32(0.f), float32(30.f));

		tempBody = m_physicsWorld->CreateBody(&tempDef);

		//tempPhsBody = PhysicsBody(entity, tempBody, float(tempSpr.GetWidth() - shrinkX), float(tempSpr.GetHeight() - shrinkY), vec2(0.f, 0.f), false, PLAYER, ENEMY | OBJECTS | PICKUP | TRIGGER, 0.5f, 3.f);
		tempPhsBody = PhysicsBody(entity, tempBody, float((tempSpr.GetHeight() - shrinkY) / 2.f), vec2(0.f, 0.f), false, PLAYER, ENVIRONMENT | ENEMY | OBJECTS | PICKUP | TRIGGER | HEXAGON, 0.5f, 3.f);
		//std::vector<b2Vec2> points = {b2Vec2(-tempSpr.GetWidth()/2.f, -tempSpr.GetHeight()/2.f), b2Vec2(tempSpr.GetWidth()/2.f, -tempSpr.GetHeight()/2.f), b2Vec2(0.f, tempSpr.GetHeight()/2.f)};
		//tempPhsBody = PhysicsBody(entity, BodyType::TRIANGLE, tempBody, points, vec2(0.f, 0.f), false, PLAYER, ENEMY | OBJECTS | PICKUP | TRIGGER, 0.5f, 3.f);

		tempPhsBody.SetRotationAngleDeg(0.f);
		tempPhsBody.SetFixedRotation(true);
		tempPhsBody.SetColor(vec4(1.f, 0.f, 1.f, 0.3f));
		tempPhsBody.SetGravityScale(1.f);
	}

	{
		//PreSpawn Fog
		{
			//Setup new Entity

			/*Scene::CreateSprite(m_sceneReg, "HelloWorld.png", 100, 60, 0.5f, vec3(0.f, 0.f, 0.f));*/

			//Creates entity
			auto entity = ECS::CreateEntity();

			//Add components
			ECS::AttachComponent<Sprite>(entity);
			ECS::AttachComponent<Transform>(entity);

			//Set up the components
			std::string fileName = "ForestTiles/Fog_Tile.png";
			ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 64000, 64000);
			ECS::GetComponent<Sprite>(entity).SetTransparency(.1f);
			ECS::GetComponent<Transform>(entity).SetPosition(vec3(-96.f, -100.f, 6.f));

		}

	}


	//Setup First Platform
	{
		int fillDirt = 0;
		int fillGrass = 0;

		for (int i = 0; i < 4; i++) {

			{
				//Setup new Entity

				/*Scene::CreateSprite(m_sceneReg, "HelloWorld.png", 100, 60, 0.5f, vec3(0.f, 0.f, 0.f));*/

				//Creates entity
				auto entity = ECS::CreateEntity();

				//Add components
				ECS::AttachComponent<Sprite>(entity);
				ECS::AttachComponent<Transform>(entity);

				//Set up the components
				std::string fileName = "ForestTiles/Grass_Tile_Back.png";
				ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 64, 64);
				ECS::GetComponent<Sprite>(entity).SetTransparency(.9f);
				ECS::GetComponent<Transform>(entity).SetPosition(vec3(-96.f + fillGrass, -100.f, 2.f));

			}

			{
				//Setup new Entity

				/*Scene::CreateSprite(m_sceneReg, "HelloWorld.png", 100, 60, 0.5f, vec3(0.f, 0.f, 0.f));*/

				//Creates entity
				auto entity = ECS::CreateEntity();

				//Add components
				ECS::AttachComponent<Sprite>(entity);
				ECS::AttachComponent<Transform>(entity);

				//Set up the components
				std::string fileName = "ForestTiles/Grass_Tile1_.png";
				ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 64, 64);
				ECS::GetComponent<Sprite>(entity).SetTransparency(.9f);
				ECS::GetComponent<Transform>(entity).SetPosition(vec3(-96.f + fillGrass, -90.f, 5.f));

			}


			{
				//Creates entity
				auto entity = ECS::CreateEntity();

				//Add components
				ECS::AttachComponent<Sprite>(entity);
				ECS::AttachComponent<Transform>(entity);
				ECS::AttachComponent<PhysicsBody>(entity);

				//Sets up components
				std::string fileName = "ForestTiles/Dirt_Tile1.png";
				ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 256, 32);
				ECS::GetComponent<Transform>(entity).SetPosition(vec3(30.f, -20.f, 3.f));

				auto& tempSpr = ECS::GetComponent<Sprite>(entity);
				auto& tempPhsBody = ECS::GetComponent<PhysicsBody>(entity);

				float shrinkX = 0.f;
				float shrinkY = 0.f;
				b2Body* tempBody;
				b2BodyDef tempDef;
				tempDef.type = b2_staticBody;
				tempDef.position.Set(float32(0.f), float32(-100.f - fillDirt));

				tempBody = m_physicsWorld->CreateBody(&tempDef);

				tempPhsBody = PhysicsBody(entity, tempBody, float(tempSpr.GetWidth() - shrinkX),
					float(tempSpr.GetHeight() - shrinkY), vec2(0.f, 0.f), false, GROUND, PLAYER | ENEMY | OBJECTS | HEXAGON);
				tempPhsBody.SetColor(vec4(0.f, 1.f, 0.f, 0.3f));
				
				//Filling in the terrain without multiple scopes
				
			}

			fillDirt = fillDirt + 32;
			fillGrass = fillGrass + 64;
		}
	}

	//Setup Second Platform
	{
		int fillDirt = 0;
		int fillGrass = 0;

		for (int i = 0; i < 4; i++) {

			{
				//Setup new Entity

				/*Scene::CreateSprite(m_sceneReg, "HelloWorld.png", 100, 60, 0.5f, vec3(0.f, 0.f, 0.f));*/

				//Creates entity
				auto entity = ECS::CreateEntity();

				//Add components
				ECS::AttachComponent<Sprite>(entity);
				ECS::AttachComponent<Transform>(entity);

				//Set up the components
				std::string fileName = "ForestTiles/Grass_Tile_Back.png";
				ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 64, 64);
				ECS::GetComponent<Sprite>(entity).SetTransparency(.9f);
				ECS::GetComponent<Transform>(entity).SetPosition(vec3(210.f + fillGrass, -70.f, 2.f));

			}

			{
				//Setup new Entity

				/*Scene::CreateSprite(m_sceneReg, "HelloWorld.png", 100, 60, 0.5f, vec3(0.f, 0.f, 0.f));*/

				//Creates entity
				auto entity = ECS::CreateEntity();

				//Add components
				ECS::AttachComponent<Sprite>(entity);
				ECS::AttachComponent<Transform>(entity);

				//Set up the components
				std::string fileName = "ForestTiles/Grass_Tile1_.png";
				ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 64, 64);
				ECS::GetComponent<Sprite>(entity).SetTransparency(.9f);
				ECS::GetComponent<Transform>(entity).SetPosition(vec3(210.f + fillGrass, -60.f, 5.f));

			}

			{
				//Setup new Entity

				/*Scene::CreateSprite(m_sceneReg, "HelloWorld.png", 100, 60, 0.5f, vec3(0.f, 0.f, 0.f));*/

				//Creates entity
				auto entity = ECS::CreateEntity();

				//Add components
				ECS::AttachComponent<Sprite>(entity);
				ECS::AttachComponent<Transform>(entity);

				//Set up the components
				std::string fileName = "ForestTiles/Rubble_Right1.png";
				ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 128, 64);
				ECS::GetComponent<Sprite>(entity).SetTransparency(.9f);
				ECS::GetComponent<Transform>(entity).SetPosition(vec3(370.f, -22.f, 4.f));

			}


			{
				//Creates entity
				auto entity = ECS::CreateEntity();

				//Add components
				ECS::AttachComponent<Sprite>(entity);
				ECS::AttachComponent<Transform>(entity);
				ECS::AttachComponent<PhysicsBody>(entity);

				//Sets up components
				std::string fileName = "ForestTiles/Dirt_Tile1.png";
				ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 256, 32);
				ECS::GetComponent<Transform>(entity).SetPosition(vec3(30.f, -20.f, 3.f));

				auto& tempSpr = ECS::GetComponent<Sprite>(entity);
				auto& tempPhsBody = ECS::GetComponent<PhysicsBody>(entity);

				float shrinkX = 0.f;
				float shrinkY = 0.f;
				b2Body* tempBody;
				b2BodyDef tempDef;
				tempDef.type = b2_staticBody;
				tempDef.position.Set(float32(306.f), float32(-70.f - fillDirt));

				tempBody = m_physicsWorld->CreateBody(&tempDef);

				tempPhsBody = PhysicsBody(entity, tempBody, float(tempSpr.GetWidth() - shrinkX),
					float(tempSpr.GetHeight() - shrinkY), vec2(0.f, 0.f), false, GROUND, PLAYER | ENEMY | OBJECTS | HEXAGON);
				tempPhsBody.SetColor(vec4(0.f, 1.f, 0.f, 0.3f));

				//Filling in the terrain without multiple scopes

			}

			fillDirt = fillDirt + 32;
			fillGrass = fillGrass + 64;
		}
	}


	//Setup Third Platform
	{
		int fillDirt = 0;
		int fillGrass = 0;

		for (int i = 0; i < 4; i++) {

			{
				//Setup new Entity

				/*Scene::CreateSprite(m_sceneReg, "HelloWorld.png", 100, 60, 0.5f, vec3(0.f, 0.f, 0.f));*/

				//Creates entity
				auto entity = ECS::CreateEntity();

				//Add components
				ECS::AttachComponent<Sprite>(entity);
				ECS::AttachComponent<Transform>(entity);

				//Set up the components
				std::string fileName = "ForestTiles/Grass_Tile_Back.png";
				ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 64, 64);
				ECS::GetComponent<Sprite>(entity).SetTransparency(.9f);
				ECS::GetComponent<Transform>(entity).SetPosition(vec3(466.f + fillGrass, -30.f, 2.f));

			}

			{
				//Setup new Entity

				/*Scene::CreateSprite(m_sceneReg, "HelloWorld.png", 100, 60, 0.5f, vec3(0.f, 0.f, 0.f));*/

				//Creates entity
				auto entity = ECS::CreateEntity();

				//Add components
				ECS::AttachComponent<Sprite>(entity);
				ECS::AttachComponent<Transform>(entity);

				//Set up the components
				std::string fileName = "ForestTiles/Grass_Tile1_.png";
				ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 64, 64);
				ECS::GetComponent<Sprite>(entity).SetTransparency(.9f);
				ECS::GetComponent<Transform>(entity).SetPosition(vec3(466.f + fillGrass, -20.f, 5.f));

			}

			{
			//Creates entity
			auto entity = ECS::CreateEntity();

			//Add components
			ECS::AttachComponent<Sprite>(entity);
			ECS::AttachComponent<Transform>(entity);
			ECS::AttachComponent<PhysicsBody>(entity);

			//Sets up components
			std::string fileName = "ForestTiles/Dirt_Tile1.png";
			ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 256, 32);
			ECS::GetComponent<Transform>(entity).SetPosition(vec3(30.f, -20.f, 3.f));

			auto& tempSpr = ECS::GetComponent<Sprite>(entity);
			auto& tempPhsBody = ECS::GetComponent<PhysicsBody>(entity);

			float shrinkX = 0.f;
			float shrinkY = 0.f;
			b2Body* tempBody;
			b2BodyDef tempDef;
			tempDef.type = b2_staticBody;
			tempDef.position.Set(float32(562.f), float32(-30.f - fillDirt));

			tempBody = m_physicsWorld->CreateBody(&tempDef);

			tempPhsBody = PhysicsBody(entity, tempBody, float(tempSpr.GetWidth() - shrinkX),
				float(tempSpr.GetHeight() - shrinkY), vec2(0.f, 0.f), false, GROUND, PLAYER | ENEMY | OBJECTS | HEXAGON);
			tempPhsBody.SetColor(vec4(0.f, 1.f, 0.f, 0.3f));

			//Filling in the terrain without multiple scopes

			}

			fillDirt = fillDirt + 32;
			fillGrass = fillGrass + 64;
		}
	}


	//Setup static RAMP
	//{
	//	//Creates entity
	//	auto entity = ECS::CreateEntity();

	//	//Add components
	//	ECS::AttachComponent<Sprite>(entity);
	//	ECS::AttachComponent<Transform>(entity);
	//	ECS::AttachComponent<PhysicsBody>(entity);

	//	//Sets up components
	//	std::string fileName = "ForestTiles/Grass_Tile_Back.png";
	//	ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 64, 64);
	//	ECS::GetComponent<Transform>(entity).SetPosition(vec3(30.f, -20.f, 2.f));

	//	auto& tempSpr = ECS::GetComponent<Sprite>(entity);
	//	auto& tempPhsBody = ECS::GetComponent<PhysicsBody>(entity);

	//	float shrinkX = 0.f;
	//	float shrinkY = 0.f;
	//	b2Body* tempBody;
	//	b2BodyDef tempDef;
	//	tempDef.type = b2_staticBody;
	//	tempDef.position.Set(float32(137.1f), float32(-29.3f));

	//	tempBody = m_physicsWorld->CreateBody(&tempDef);

	//	tempPhsBody = PhysicsBody(entity, tempBody, float(tempSpr.GetWidth() - shrinkX),
	//		float(tempSpr.GetHeight() - shrinkY), vec2(0.f, 0.f), false, GROUND, PLAYER | OBJECTS | ENEMY | HEXAGON, 0.8f);
	//	tempPhsBody.SetColor(vec4(0.f, 1.f, 0.f, 0.3f));
	//	tempPhsBody.SetRotationAngleDeg(-30.f);
	//}

	//Setup static Platform
	//{
	//	//Creates entity
	//	auto entity = ECS::CreateEntity();

	//	//Add components
	//	ECS::AttachComponent<Sprite>(entity);
	//	ECS::AttachComponent<Transform>(entity);
	//	ECS::AttachComponent<PhysicsBody>(entity);

	//	//Sets up components
	//	std::string fileName = "boxSprite.jpg";
	//	ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 150, 10);
	//	ECS::GetComponent<Transform>(entity).SetPosition(vec3(30.f, -20.f, 2.f));

	//	auto& tempSpr = ECS::GetComponent<Sprite>(entity);
	//	auto& tempPhsBody = ECS::GetComponent<PhysicsBody>(entity);

	//	float shrinkX = 0.f;
	//	float shrinkY = 0.f;
	//	b2Body* tempBody;
	//	b2BodyDef tempDef;
	//	tempDef.type = b2_staticBody;
	//	tempDef.position.Set(float32(240.f), float32(-50.f));

	//	tempBody = m_physicsWorld->CreateBody(&tempDef);

	//	tempPhsBody = PhysicsBody(entity, tempBody, float(tempSpr.GetWidth() - shrinkX),
	//		float(tempSpr.GetHeight() - shrinkY), vec2(0.f, 0.f), false, GROUND, PLAYER | OBJECTS | ENEMY | HEXAGON);
	//	tempPhsBody.SetColor(vec4(0.f, 1.f, 0.f, 0.3f));
	//}

	//Setup static Wall
	//{
	//	//Creates entity
	//	auto entity = ECS::CreateEntity();
	//	puzzleWall1 = entity;

	//	//Add components
	//	ECS::AttachComponent<Sprite>(entity);
	//	ECS::AttachComponent<Transform>(entity);
	//	ECS::AttachComponent<PhysicsBody>(entity);

	//	//Sets up components
	//	std::string fileName = "boxSprite.jpg";
	//	ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 50, 15);
	//	ECS::GetComponent<Transform>(entity).SetPosition(vec3(45.f, -20.f, 2.f));

	//	auto& tempSpr = ECS::GetComponent<Sprite>(entity);
	//	auto& tempPhsBody = ECS::GetComponent<PhysicsBody>(entity);

	//	float shrinkX = 0.f;
	//	float shrinkY = 0.f;
	//	b2Body* tempBody;
	//	b2BodyDef tempDef;
	//	tempDef.type = b2_kinematicBody;
	//	tempDef.position.Set(float32(267.f), float32(5.f));

	//	tempBody = m_physicsWorld->CreateBody(&tempDef);

	//	tempPhsBody = PhysicsBody(entity, tempBody, float(tempSpr.GetWidth() - shrinkX), float(tempSpr.GetHeight() - shrinkY), vec2(0.f, 0.f), false, ENVIRONMENT, PLAYER | OBJECTS | ENEMY | HEXAGON);
	//	tempPhsBody.SetColor(vec4(0.f, 1.f, 0.f, 0.3f));
	//	tempPhsBody.SetRotationAngleDeg(90.f);
	//	tempPhsBody.SetPosition(b2Vec2(267.f, 5.f));
	//}

	////Setup static Wall
	//{
	//	//Creates entity
	//	auto entity = ECS::CreateEntity();
	//	puzzleWall2 = entity;

	//	//Add components
	//	ECS::AttachComponent<Sprite>(entity);
	//	ECS::AttachComponent<Transform>(entity);
	//	ECS::AttachComponent<PhysicsBody>(entity);

	//	//Sets up components
	//	std::string fileName = "boxSprite.jpg";
	//	ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 80, 15);
	//	ECS::GetComponent<Transform>(entity).SetPosition(vec3(30.f, -20.f, 2.f));

	//	auto& tempSpr = ECS::GetComponent<Sprite>(entity);
	//	auto& tempPhsBody = ECS::GetComponent<PhysicsBody>(entity);

	//	float shrinkX = 0.f;
	//	float shrinkY = 0.f;
	//	b2Body* tempBody;
	//	b2BodyDef tempDef;
	//	tempDef.type = b2_staticBody;
	//	tempDef.position.Set(float32(322.f), float32(-15.f));

	//	tempBody = m_physicsWorld->CreateBody(&tempDef);

	//	tempPhsBody = PhysicsBody(entity, tempBody, float(tempSpr.GetWidth() - shrinkX), float(tempSpr.GetHeight() - shrinkY), vec2(0.f, 0.f), false, ENVIRONMENT, PLAYER | OBJECTS | ENEMY | HEXAGON);
	//	tempPhsBody.SetColor(vec4(0.f, 1.f, 0.f, 0.3f));
	//	tempPhsBody.SetRotationAngleDeg(90.f);
	//}

	////Ball
	//{
	//	auto entity = ECS::CreateEntity();
	//	//Add components
	//	ECS::AttachComponent<Sprite>(entity);
	//	ECS::AttachComponent<Transform>(entity);
	//	ECS::AttachComponent<PhysicsBody>(entity);

	//	//Sets up the components
	//	std::string fileName = "BeachBall.png";
	//	ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 20, 20);
	//	ECS::GetComponent<Sprite>(entity).SetTransparency(1.f);
	//	ECS::GetComponent<Transform>(entity).SetPosition(vec3(45.f, -8.f, 3.f));

	//	auto& tempSpr = ECS::GetComponent<Sprite>(entity);
	//	auto& tempPhsBody = ECS::GetComponent<PhysicsBody>(entity);

	//	float shrinkX = 0.f;
	//	float shrinkY = 0.f;

	//	b2Body* tempBody;
	//	b2BodyDef tempDef;
	//	tempDef.type = b2_dynamicBody;
	//	tempDef.position.Set(float32(45.f), float32(-8.f));

	//	tempBody = m_physicsWorld->CreateBody(&tempDef);

	//	//tempPhsBody = PhysicsBody(tempBody, float(tempSpr.GetWidth() - shrinkX), float(tempSpr.GetHeight() - shrinkY), vec2(0.f, 0.f), false);
	//	tempPhsBody = PhysicsBody(entity, tempBody, float((tempSpr.GetWidth() - shrinkY) / 2.f), vec2(0.f, 0.f), false, OBJECTS, GROUND | HEXAGON | ENVIRONMENT | PLAYER | TRIGGER | HEXAGON, 0.3f);

	//	tempPhsBody.SetColor(vec4(1.f, 0.f, 1.f, 0.3f));
	//}

	////Setup trigger
	//{
	//	//Creates entity
	//	auto entity = ECS::CreateEntity();

	//	//Add components
	//	ECS::AttachComponent<Transform>(entity);
	//	ECS::AttachComponent<PhysicsBody>(entity);
	//	ECS::AttachComponent<Trigger*>(entity);

	//	//Sets up components
	//	std::string fileName = "boxSprite.jpg";
	//	ECS::GetComponent<Transform>(entity).SetPosition(vec3(30.f, -20.f, 80.f));
	//	ECS::GetComponent<Trigger*>(entity) = new TranslateTrigger();
	//	ECS::GetComponent<Trigger*>(entity)->SetTriggerEntity(entity);
	//	ECS::GetComponent<Trigger*>(entity)->AddTargetEntity(puzzleWall1);
	//	TranslateTrigger* temp = (TranslateTrigger*)ECS::GetComponent<Trigger*>(entity);
	//	temp->movement = b2Vec2(0.f, 15.f);

	//	//ECS::GetComponent<Trigger*>(entity)->AddTargetEntity(puzzleWall2);

	//	auto& tempPhsBody = ECS::GetComponent<PhysicsBody>(entity);

	//	float shrinkX = 0.f;
	//	float shrinkY = 0.f;
	//	b2Body* tempBody;
	//	b2BodyDef tempDef;
	//	tempDef.type = b2_staticBody;
	//	tempDef.position.Set(float32(295.f), float32(-30.f));

	//	tempBody = m_physicsWorld->CreateBody(&tempDef);

	//	tempPhsBody = PhysicsBody(entity, tempBody, float(40.f - shrinkX), float(40.f - shrinkY), vec2(0.f, 0.f), true, TRIGGER, PLAYER | OBJECTS);
	//	tempPhsBody.SetColor(vec4(1.f, 0.f, 0.f, 0.3f));
	//}

	//{
	//	auto entity = ECS::CreateEntity();

	//	//Add components
	//	ECS::AttachComponent<Sprite>(entity);
	//	ECS::AttachComponent<Transform>(entity);
	//	ECS::AttachComponent<PhysicsBody>(entity);

	//	//Sets up the components
	//	std::string fileName = "hexagon.png";
	//	ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 20, 20);
	//	ECS::GetComponent<Sprite>(entity).SetTransparency(1.f);
	//	ECS::GetComponent<Transform>(entity).SetPosition(vec3(45.f, -8.f, 3.f));

	//	auto& tempSpr = ECS::GetComponent<Sprite>(entity);
	//	auto& tempPhsBody = ECS::GetComponent<PhysicsBody>(entity);

	//	float shrinkX = 0.f;
	//	float shrinkY = 0.f;

	//	b2Body* tempBody;
	//	b2BodyDef tempDef;
	//	tempDef.type = b2_dynamicBody;
	//	tempDef.position.Set(float32(0.f), float32(260.f));

	//	tempBody = m_physicsWorld->CreateBody(&tempDef);

	//	std::vector<b2Vec2> points = { b2Vec2(-tempSpr.GetWidth() / 2.f, 0), b2Vec2(-tempSpr.GetWidth() / 4.f, -tempSpr.GetHeight() / 2.f),
	//									b2Vec2(tempSpr.GetWidth() / 4.f, -tempSpr.GetHeight() / 2.f), b2Vec2(tempSpr.GetWidth() / 2.f, 0),
	//									b2Vec2(tempSpr.GetWidth() / 4.f, tempSpr.GetHeight() / 2.f),  b2Vec2(-tempSpr.GetWidth() / 4.f, tempSpr.GetHeight() / 2.f) };

	//	tempPhsBody = PhysicsBody(entity, BodyType::HEXAGON, tempBody, points, vec2(0.f, 0.f), false, HEXAGON, GROUND | OBJECTS | ENVIRONMENT | PLAYER | TRIGGER, 0.5f, 3.5);
	//	tempPhsBody.SetColor(vec4(1.f, 0.f, 1.f, 0.3f));
	//}


	ECS::GetComponent<HorizontalScroll>(MainEntities::MainCamera()).SetFocus(&ECS::GetComponent<Transform>(MainEntities::MainPlayer()));
	ECS::GetComponent<VerticalScroll>(MainEntities::MainCamera()).SetFocus(&ECS::GetComponent<Transform>(MainEntities::MainPlayer()));
}

void ForestScene::Update()
{

}

void ForestScene::KeyboardHold()
{
	auto& player = ECS::GetComponent<PhysicsBody>(MainEntities::MainPlayer());

	float speed = 1.f;
	b2Vec2 vel = b2Vec2(0.f, 0.f);

	if (Input::GetKey(Key::Shift))
	{
		speed *= 5.f;
	}

	if (Input::GetKey(Key::A))
	{
		player.GetBody()->ApplyForceToCenter(b2Vec2(-400000.f * speed, 0.f), true);
	}
	if (Input::GetKey(Key::D))
	{
		player.GetBody()->ApplyForceToCenter(b2Vec2(400000.f * speed, 0.f), true);
	}

	//Change physics body size for circle
	if (Input::GetKey(Key::O))
	{
		player.ScaleBody(1.3 * Timer::deltaTime, 0);
	}
	else if (Input::GetKey(Key::I))
	{
		player.ScaleBody(-1.3 * Timer::deltaTime, 0);
	}
	
}

void ForestScene::KeyboardDown()
{
	auto& player = ECS::GetComponent<PhysicsBody>(MainEntities::MainPlayer());
	auto& canJump = ECS::GetComponent<CanJump>(MainEntities::MainPlayer());

	

	if (Input::GetKeyDown(Key::T))
	{
		PhysicsBody::SetDraw(!PhysicsBody::GetDraw());
	}
	if (canJump.m_canJump)
	{	
		if (Input::GetKeyDown(Key::Space))
		{
			doubleJump++;
			player.GetBody()->ApplyLinearImpulseToCenter(b2Vec2(0.f, 150000.f), true);
			
			std::cout << doubleJump;

			if (doubleJump == 2) {
				canJump.m_canJump = false;
				doubleJump = 0;
			}

		}
	}
}

void ForestScene::KeyboardUp()
{


}