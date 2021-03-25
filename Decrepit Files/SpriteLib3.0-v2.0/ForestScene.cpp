#include "ForestScene.h"
#include "Utilities.h"
#include <random>
#include "Game.h"
#include "Platform.h"
#include "Door.h"



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

		vec4 temp = vec4(-175.f, 275.f, -180.f, 175.f);
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

	

	//Character entity
	{
		/*Scene::CreatePhysicsSprite(m_sceneReg, "LinkStandby", 80, 60, 1.f, vec3(0.f, 30.f, 2.f), b2_dynamicBody, 0.f, 0.f, true, true)*/

		auto entity = ECS::CreateEntity();
		ECS::SetIsMainPlayer(entity, true);

		player = entity;

		//Add components
		ECS::AttachComponent<Sprite>(entity);
		ECS::AttachComponent<Transform>(entity);
		ECS::AttachComponent<PhysicsBody>(entity);
		ECS::AttachComponent<CanJump>(entity);

		//Sets up the components
		std::string fileName = "Character.png";
		ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 27, 27);
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
		tempPhsBody = PhysicsBody(entity, tempBody, float((tempSpr.GetHeight() - shrinkY) / 2.f), vec2(0.f, 0.f), false, PLAYER, GROUND | ENVIRONMENT | ENEMY | OBJECTS | TRIGGER, 0.5f, 3.f);
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
				ECS::GetComponent<Transform>(entity).SetPosition(vec3(-96.f + fillGrass, -70.f, 2.f));

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
				ECS::GetComponent<Transform>(entity).SetPosition(vec3(-96.f + fillGrass, -60.f, 5.f));

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
				tempDef.position.Set(float32(0.f), float32(-70.f - fillDirt));

				tempBody = m_physicsWorld->CreateBody(&tempDef);

				tempPhsBody = PhysicsBody(entity, tempBody, float(tempSpr.GetWidth() - shrinkX),
					float(tempSpr.GetHeight() - shrinkY), vec2(0.f, 0.f), false, WALL, PLAYER | ENEMY | OBJECTS);
				tempPhsBody.SetColor(vec4(0.f, 1.f, 0.f, 0.3f));
				
				//Filling in the terrain without multiple scopes
				
			}

			fillDirt = fillDirt + 32;
			fillGrass = fillGrass + 64;
		}

		float physBodyX = 254.f;
		float physBodyY = 25.f;

		{
			//Creates entity
			auto entity = ECS::CreateEntity();

			//Add components
			ECS::AttachComponent<Transform>(entity);
			ECS::AttachComponent<PhysicsBody>(entity);

			//Sets up components
			ECS::GetComponent<Transform>(entity).SetPosition(vec3(30.f, 0.f, 4.f));
			auto& tempPhsBody = ECS::GetComponent<PhysicsBody>(entity);

			float shrinkX = 0.f;
			float shrinkY = 0.f;
			b2Body* tempBody;
			b2BodyDef tempDef;
			tempDef.type = b2_staticBody;
			tempDef.position.Set(float32(0.f), float32(-76.f));

			tempBody = m_physicsWorld->CreateBody(&tempDef);

			tempPhsBody = PhysicsBody(entity, tempBody, float(physBodyX),
				float(physBodyY / 2), vec2(0.f, 16.f), false, GROUND, PLAYER | ENEMY | OBJECTS);
			tempPhsBody.SetColor(vec4(1.f, 0.f, 0.f, 0.3f));

		}

	}



	//Setup First Hole
	{

		int fillDirt = 0;

		for (int i = 0; i < 2; i++) 
		{

			{
				//Creates entity
				auto entity = ECS::CreateEntity();

				//Add components
				ECS::AttachComponent<Sprite>(entity);
				ECS::AttachComponent<Transform>(entity);
				ECS::AttachComponent<PhysicsBody>(entity);

				//Sets up components
				std::string fileName = "ForestTiles/Dirt_Tile1.png";
				ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 52, 40);
				ECS::GetComponent<Transform>(entity).SetPosition(vec3(30.f, -20.f, 3.f));

				auto& tempSpr = ECS::GetComponent<Sprite>(entity);
				auto& tempPhsBody = ECS::GetComponent<PhysicsBody>(entity);

				float shrinkX = 0.f;
				float shrinkY = 0.f;
				b2Body* tempBody;
				b2BodyDef tempDef;
				tempDef.type = b2_staticBody;
				tempDef.position.Set(float32(152.f), float32(-122.f - fillDirt));

				tempBody = m_physicsWorld->CreateBody(&tempDef);

				tempPhsBody = PhysicsBody(entity, tempBody, float(tempSpr.GetWidth() - shrinkX),
					float(tempSpr.GetHeight() - shrinkY), vec2(0.f, 0.f), false, GROUND, PLAYER | ENEMY | OBJECTS);
				tempPhsBody.SetColor(vec4(0.f, 1.f, 0.f, 0.3f));
			}

			fillDirt = fillDirt + 40;
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
			ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 64, 40);
			ECS::GetComponent<Sprite>(entity).SetTransparency(.9f);
			ECS::GetComponent<Transform>(entity).SetPosition(vec3(152.f, -100.f, 2.f));

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
					float(tempSpr.GetHeight() - shrinkY), vec2(0.f, 0.f), false, WALL , PLAYER | ENEMY | OBJECTS);
				tempPhsBody.SetColor(vec4(0.f, 1.f, 0.f, 0.3f));

				//Filling in the terrain without multiple scopes

			}

			fillDirt = fillDirt + 32;
			fillGrass = fillGrass + 64;
		}

		float physBodyX = 254.f;
		float physBodyY = 25.f;

		{
			//Creates entity
			auto entity = ECS::CreateEntity();

			//Add components
			ECS::AttachComponent<Transform>(entity);
			ECS::AttachComponent<PhysicsBody>(entity);

			//Sets up components
			ECS::GetComponent<Transform>(entity).SetPosition(vec3(30.f, 0.f, 4.f));
			auto& tempPhsBody = ECS::GetComponent<PhysicsBody>(entity);

			float shrinkX = 0.f;
			float shrinkY = 0.f;
			b2Body* tempBody;
			b2BodyDef tempDef;
			tempDef.type = b2_staticBody;
			tempDef.position.Set(float32(306.f), float32(-76.f));

			tempBody = m_physicsWorld->CreateBody(&tempDef);

			tempPhsBody = PhysicsBody(entity, tempBody, float(physBodyX),
				float(physBodyY / 2), vec2(0.f, 16.f), false, GROUND, PLAYER | ENEMY | OBJECTS);
			tempPhsBody.SetColor(vec4(1.f, 0.f, 0.f, 0.3f));

		}

	}

	//Switch One
	{
		{
			//Creates entity
			auto entity = ECS::CreateEntity();
			ECS::SetIsSwitchOne(entity, true);

			

			//Add components
			ECS::AttachComponent<Sprite>(entity);
			ECS::AttachComponent<Transform>(entity);
			ECS::AttachComponent<PhysicsBody>(entity);
			

			//Sets up components
			std::string fileName = "ForestTiles/Switch_Off.png";
			ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 64, 64);
			ECS::GetComponent<Transform>(entity).SetPosition(vec3(30.f, -20.f, 3.f));
			

			auto& tempSpr = ECS::GetComponent<Sprite>(entity);
			auto& tempPhsBody = ECS::GetComponent<PhysicsBody>(entity);


			float shrinkX = 32.f;
			float shrinkY = 0.f;
			b2Body* tempBody;
			b2BodyDef tempDef;
			tempDef.type = b2_staticBody;
			tempDef.position.Set(float32(400.f), float32(-20.f));

			tempBody = m_physicsWorld->CreateBody(&tempDef);

			tempPhsBody = PhysicsBody(entity, tempBody, float(tempSpr.GetWidth() - shrinkX), float(tempSpr.GetHeight() - shrinkY), vec2(0.f, 0.f), false, SWITCH, PLAYER);
			tempPhsBody.SetColor(vec4(0.f, 1.f, 0.f, 0.3f));

			

		}

		{
			//Creates entity
			auto entity = ECS::CreateEntity();
			ECS::SetIsBridgeOne(entity, true);

			//Add components
			ECS::AttachComponent<Sprite>(entity);
			ECS::AttachComponent<Transform>(entity);
			ECS::AttachComponent<PhysicsBody>(entity);
			ECS::AttachComponent<Bridge>(entity);

			//Sets up components
			std::string fileName = "ForestTiles/Bridge_Up.png";
			ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 64, 140);
			ECS::GetComponent<Transform>(entity).SetPosition(vec3(30.f, -20.f, 2.f));


			auto& tempSpr = ECS::GetComponent<Sprite>(entity);
			auto& tempPhsBody = ECS::GetComponent<PhysicsBody>(entity);


			float shrinkX = 0.f;
			float shrinkY = 0.f;
			b2Body* tempBody;
			b2BodyDef tempDef;
			tempDef.type = b2_staticBody;
			tempDef.position.Set(float32(460.f), float32(15.f));

			tempBody = m_physicsWorld->CreateBody(&tempDef);

			tempPhsBody = PhysicsBody(entity, tempBody, float(tempSpr.GetWidth() - shrinkX), float(tempSpr.GetHeight() - shrinkY), vec2(0.f, 0.f), false, OBJECTS, PLAYER);
			tempPhsBody.SetColor(vec4(1.f, 2.f, 4.f, 0.3f));


		}

	}

	//Setup Second Hole

	{

	int fillDirt = 0;

	for (int i = 0; i < 2; i++)
	{

		{
			//Creates entity
			auto entity = ECS::CreateEntity();

			//Add components
			ECS::AttachComponent<Sprite>(entity);
			ECS::AttachComponent<Transform>(entity);
			ECS::AttachComponent<PhysicsBody>(entity);

			//Sets up components
			std::string fileName = "ForestTiles/Dirt_Tile1.png";
			ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 208, 40);
			ECS::GetComponent<Transform>(entity).SetPosition(vec3(30.f, -20.f, 3.f));

			auto& tempSpr = ECS::GetComponent<Sprite>(entity);
			auto& tempPhsBody = ECS::GetComponent<PhysicsBody>(entity);

			float shrinkX = 0.f;
			float shrinkY = 0.f;
			b2Body* tempBody;
			b2BodyDef tempDef;
			tempDef.type = b2_staticBody;
			tempDef.position.Set(float32(538.f), float32(-122.f - fillDirt));

			tempBody = m_physicsWorld->CreateBody(&tempDef);

			tempPhsBody = PhysicsBody(entity, tempBody, float(tempSpr.GetWidth() - shrinkX),
				float(tempSpr.GetHeight() - shrinkY), vec2(0.f, 0.f), false, GROUND, PLAYER | ENEMY | OBJECTS);
			tempPhsBody.SetColor(vec4(0.f, 1.f, 0.f, 0.3f));
		}

		fillDirt = fillDirt + 40;
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
		ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 64, 40);
		ECS::GetComponent<Sprite>(entity).SetTransparency(.9f);
		ECS::GetComponent<Transform>(entity).SetPosition(vec3(434.f, -100.f, 2.f));

	}

	}

	
	////PLATFORM 1 GROUP
	//{
	//	float physBodyX = 140.f;
	//	float physBodyY = 62.f;
	//	float posX = 0.f;
	//	float posY = -24.f;
	//	float topOffset = -18.f;
	//	//small platform 1(textured)
	//	{
	//		//Creates entity
	//		auto entity = ECS::CreateEntity();

	//		//Add components
	//		ECS::AttachComponent<Sprite>(entity);
	//		ECS::AttachComponent<Transform>(entity);
	//		ECS::AttachComponent<PhysicsBody>(entity);

	//		//Sets up components
	//		std::string fileName = "WallTiles/platformBH.png";
	//		ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 144, 64);
	//		ECS::GetComponent<Transform>(entity).SetPosition(vec3(30.f, 0.f, 3.f));

	//		auto& tempSpr = ECS::GetComponent<Sprite>(entity);
	//		auto& tempPhsBody = ECS::GetComponent<PhysicsBody>(entity);

	//		float shrinkX = 0.f;
	//		float shrinkY = 26.f;
	//		b2Body* tempBody;
	//		b2BodyDef tempDef;
	//		tempDef.type = b2_staticBody;
	//		tempDef.position.Set(float32(posX), float32(posY));

	//		tempBody = m_physicsWorld->CreateBody(&tempDef);

	//		tempPhsBody = PhysicsBody(entity, tempBody, float(tempSpr.GetWidth() - shrinkX),
	//			float(tempSpr.GetHeight() - shrinkY), vec2(0.f, 12.f), false, WALL, PLAYER | ENEMY | OBJECTS | HEXAGON);
	//		tempPhsBody.SetColor(vec4(0.f, 1.f, 0.f, 0.3f));

	//	}
	//	////platform top half
	//	//{
	//	//	//Creates entity
	//	//	auto entity = ECS::CreateEntity();

	//	//	//Add components
	//	//	ECS::AttachComponent<Sprite>(entity);
	//	//	ECS::AttachComponent<Transform>(entity);
	//	//	ECS::AttachComponent<PhysicsBody>(entity);

	//	//	//Sets up components
	//	//	std::string fileName = "WallTiles/platformtop.png";
	//	//	ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 144, 16);
	//	//	ECS::GetComponent<Transform>(entity).SetPosition(vec3(30.f, 0.f, 3.f));

	//	//	auto& tempSpr = ECS::GetComponent<Sprite>(entity);
	//	//	auto& tempPhsBody = ECS::GetComponent<PhysicsBody>(entity);
	//	//	b2Body* tempBody;
	//	//	b2BodyDef tempDef;
	//	//	tempDef.type = b2_staticBody;
	//	//	tempDef.position.Set(float32(posX), float32(posY+40.f));

	//	//	tempBody = m_physicsWorld->CreateBody(&tempDef);

	//	//	tempPhsBody = PhysicsBody(entity, tempBody, float(tempSpr.GetWidth()),
	//	//		float(tempSpr.GetHeight()), vec2(0.f, topOffset), false, WALL, PLAYER | ENEMY | OBJECTS | HEXAGON);
	//	//	tempPhsBody.SetColor(vec4(0.f, 1.f, 0.f, 0.3f));

	//	//}
	//	//small platform 1(top)
	//	{
	//		//Creates entity
	//		auto entity = ECS::CreateEntity();

	//		//Add components
	//		ECS::AttachComponent<Transform>(entity);
	//		ECS::AttachComponent<PhysicsBody>(entity);

	//		//Sets up components
	//		ECS::GetComponent<Transform>(entity).SetPosition(vec3(30.f, 0.f, 4.f));
	//		auto& tempPhsBody = ECS::GetComponent<PhysicsBody>(entity);

	//		float shrinkX = 0.f;
	//		float shrinkY = 0.f;
	//		b2Body* tempBody;
	//		b2BodyDef tempDef;
	//		tempDef.type = b2_staticBody;
	//		tempDef.position.Set(float32(posX), float32(posY));

	//		tempBody = m_physicsWorld->CreateBody(&tempDef);

	//		tempPhsBody = PhysicsBody(entity, tempBody, float(physBodyX),
	//			float(physBodyY / 2), vec2(0.f, 16.f), false, GROUND, PLAYER | ENEMY | OBJECTS | HEXAGON);
	//		tempPhsBody.SetColor(vec4(1.f, 0.f, 0.f, 0.3f));

	//	}

	//	//small platform 1(bottom)
	//	{
	//		//Creates entity
	//		auto entity = ECS::CreateEntity();

	//		//Add components
	//		ECS::AttachComponent<Transform>(entity);
	//		ECS::AttachComponent<PhysicsBody>(entity);

	//		//Sets up components
	//		ECS::GetComponent<Transform>(entity).SetPosition(vec3(30.f, 0.f, 4.f));
	//		auto& tempPhsBody = ECS::GetComponent<PhysicsBody>(entity);

	//		float shrinkX = 0.f;
	//		float shrinkY = (posY / 2.f) + 20;
	//		b2Body* tempBody;
	//		b2BodyDef tempDef;
	//		tempDef.type = b2_staticBody;
	//		tempDef.position.Set(float32(posX), float32(posY));

	//		tempBody = m_physicsWorld->CreateBody(&tempDef);

	//		tempPhsBody = PhysicsBody(entity, tempBody, float(physBodyX),
	//			float(physBodyY / 2), vec2(0.f, 8.f), false, OBJECTS, PLAYER | ENEMY | OBJECTS | HEXAGON);
	//		tempPhsBody.SetColor(vec4(0.f, 0.f, 1.f, 0.3f));

	//	}
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
			
			player.GetBody()->ApplyLinearImpulseToCenter(b2Vec2(0.f, 200000.f), true);
			canJump.m_canJump = false;

			

		}
	}


	if (Input::GetKeyDown(Key::M)) {
		ECS::GetComponent<HorizontalScroll>(MainEntities::MainCamera()).setSpawnCam(true);
		player.SetVelocity(vec3(0, 0, 0));
		player.SetPosition(b2Vec2(-100, -40), true);
		player.GetBody()->ApplyLinearImpulseToCenter(b2Vec2(0.f, -1.f), true);
		
	}
	if (canJump.m_wallJump) {
		if (Input::GetKeyDown(Key::Space))
		{
			if (canJump.m_facingRight) {
				player.GetBody()->ApplyLinearImpulseToCenter(b2Vec2(150000.f, 250000.f), true);
				canJump.m_wallJump = false;
			}
			else {
				player.GetBody()->ApplyLinearImpulseToCenter(b2Vec2(-150000.f, 250000.f), true);
				canJump.m_wallJump = false;
			}




		}
	}

}

void ForestScene::KeyboardUp()
{


}