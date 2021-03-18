#include "TowerGroundScene.h"
#include "Platform.h"
#include "Utilities.h"
#include <random>
#include "Platform.cpp"
#include "Door.h"
#include "LadderTrigger.h"

TowerGroundScene::TowerGroundScene(std::string name)
	: Scene(name)
{
	m_gravity = b2Vec2(0.f, -98.f);
	m_physicsWorld->SetGravity(m_gravity);

	m_physicsWorld->SetContactListener(&listener);

}

void TowerGroundScene::InitScene(float windowWidth, float windowHeight)
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
		std::string fileName = "WallTiles/Background.png";
		ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 1280, 1280);
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
		tempDef.position.Set(float32(-100.f), float32(-40.f));

		tempBody = m_physicsWorld->CreateBody(&tempDef);

		//tempPhsBody = PhysicsBody(entity, tempBody, float(tempSpr.GetWidth() - shrinkX), float(tempSpr.GetHeight() - shrinkY), vec2(0.f, 0.f), false, PLAYER, ENEMY | OBJECTS | PICKUP | TRIGGER, 0.5f, 3.f);
		tempPhsBody = PhysicsBody(entity, tempBody, float((tempSpr.GetHeight() - shrinkY) / 2.f), vec2(0.f, 0.f), false, PLAYER, ENVIRONMENT | ENEMY | OBJECTS | PICKUP | TRIGGER | HEXAGON, 1.f, 4.5f);
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
			ECS::GetComponent<Sprite>(entity).SetTransparency(.25f);
			ECS::GetComponent<Transform>(entity).SetPosition(vec3(-96.f, -100.f, 6.f));

		}

		{

			
			
			for (int i = 0; i < 4; i++) {

				int increment = 192;
				//left wall
				{
					//Creates entity
					auto entity = ECS::CreateEntity();

					//Add components
					ECS::AttachComponent<Sprite>(entity);
					ECS::AttachComponent<Transform>(entity);
					ECS::AttachComponent<PhysicsBody>(entity);

					//Sets up components
					std::string fileName = "Tower Inside/Base/Filling/FIXED_LEFT.png";
					ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 64, 192);
					ECS::GetComponent<Transform>(entity).SetPosition(vec3(30.f, -20.f, 5.f));

					auto& tempSpr = ECS::GetComponent<Sprite>(entity);
					auto& tempPhsBody = ECS::GetComponent<PhysicsBody>(entity);

					float shrinkX = 0.f;
					float shrinkY = 0.f;
					b2Body* tempBody;
					b2BodyDef tempDef;
					tempDef.type = b2_staticBody; 
					tempDef.position.Set(float32(-164.f), float32(-15.f + (increment*i)));

					tempBody = m_physicsWorld->CreateBody(&tempDef);

					tempPhsBody = PhysicsBody(entity, tempBody, float(tempSpr.GetWidth() - shrinkX),
						float(tempSpr.GetHeight() - shrinkY), vec2(0.f, 0.f), false, WALL, PLAYER | ENEMY | OBJECTS | HEXAGON);
					tempPhsBody.SetColor(vec4(0.f, 1.f, 0.f, 0.3f));

					//Filling in the terrain without multiple scopes

				}
			}
			for (int i = 0; i < 3; i++) {

				int increment = 192;
				//right wall
				{
					//Creates entity
					auto entity = ECS::CreateEntity();

					//Add components
					ECS::AttachComponent<Sprite>(entity);
					ECS::AttachComponent<Transform>(entity);
					ECS::AttachComponent<PhysicsBody>(entity);

					//Sets up components
					std::string fileName = "Tower Inside/Base/Filling/FIXED_RIGHT.png";
					ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 64, 192);
					ECS::GetComponent<Transform>(entity).SetPosition(vec3(30.f, -20.f, 3.f));

					auto& tempSpr = ECS::GetComponent<Sprite>(entity);
					auto& tempPhsBody = ECS::GetComponent<PhysicsBody>(entity);

					float shrinkX = 0.f;
					float shrinkY = 0.f;
					b2Body* tempBody;
					b2BodyDef tempDef;
					tempDef.type = b2_staticBody;
					tempDef.position.Set(float32(672.f), float32(-15.f + (increment * i)));

					tempBody = m_physicsWorld->CreateBody(&tempDef);

					tempPhsBody = PhysicsBody(entity, tempBody, float(tempSpr.GetWidth() - shrinkX),
						float(tempSpr.GetHeight() - shrinkY), vec2(0.f, 0.f), false, WALL, PLAYER | ENEMY | OBJECTS | HEXAGON);
					tempPhsBody.SetColor(vec4(0.f, 1.f, 0.f, 0.3f));

					//Filling in the terrain without multiple scopes

				}
			}
			//PLATFORM 1 GROUP
			{
				float physBodyX = 140.f;
				float physBodyY = 62.f;
				float posX = 0.f;
				float posY = -24.f;
				float topOffset = -18.f;
				//small platform 1(textured)
				{
					//Creates entity
					auto entity = ECS::CreateEntity();

					//Add components
					ECS::AttachComponent<Sprite>(entity);
					ECS::AttachComponent<Transform>(entity);
					ECS::AttachComponent<PhysicsBody>(entity);

					//Sets up components
					std::string fileName = "WallTiles/platformBH.png";
					ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 144, 64);
					ECS::GetComponent<Transform>(entity).SetPosition(vec3(30.f, 0.f, 3.f));

					auto& tempSpr = ECS::GetComponent<Sprite>(entity);
					auto& tempPhsBody = ECS::GetComponent<PhysicsBody>(entity);

					float shrinkX = 0.f;
					float shrinkY = 26.f;
					b2Body* tempBody;
					b2BodyDef tempDef;
					tempDef.type = b2_staticBody;
					tempDef.position.Set(float32(posX), float32(posY));

					tempBody = m_physicsWorld->CreateBody(&tempDef);

					tempPhsBody = PhysicsBody(entity, tempBody, float(tempSpr.GetWidth() - shrinkX),
						float(tempSpr.GetHeight() - shrinkY), vec2(0.f,12.f), false, WALL, PLAYER | ENEMY | OBJECTS | HEXAGON);
					tempPhsBody.SetColor(vec4(0.f, 1.f, 0.f, 0.3f));

				}
				//platform top half
				{
					//Creates entity
					auto entity = ECS::CreateEntity();

					//Add components
					ECS::AttachComponent<Sprite>(entity);
					ECS::AttachComponent<Transform>(entity);
					ECS::AttachComponent<PhysicsBody>(entity);

					//Sets up components
					std::string fileName = "WallTiles/platformtop.png";
					ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 144, 16);
					ECS::GetComponent<Transform>(entity).SetPosition(vec3(30.f, 0.f, 3.f));

					auto& tempSpr = ECS::GetComponent<Sprite>(entity);
					auto& tempPhsBody = ECS::GetComponent<PhysicsBody>(entity);
					b2Body* tempBody;
					b2BodyDef tempDef;
					tempDef.type = b2_staticBody;
					tempDef.position.Set(float32(posX), float32(posY+40.f));

					tempBody = m_physicsWorld->CreateBody(&tempDef);

					tempPhsBody = PhysicsBody(entity, tempBody, float(tempSpr.GetWidth()),
						float(tempSpr.GetHeight()), vec2(0.f, topOffset), false, WALL, PLAYER | ENEMY | OBJECTS | HEXAGON);
					tempPhsBody.SetColor(vec4(0.f, 1.f, 0.f, 0.3f));

				}
				//small platform 1(top)
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
					tempDef.position.Set(float32(posX), float32(posY));

					tempBody = m_physicsWorld->CreateBody(&tempDef);

					tempPhsBody = PhysicsBody(entity, tempBody, float(physBodyX),
						float(physBodyY/2), vec2(0.f,16.f), false, GROUND, PLAYER | ENEMY | OBJECTS | HEXAGON);
					tempPhsBody.SetColor(vec4(1.f, 0.f, 0.f, 0.3f));

				}

				//small platform 1(bottom)
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
					float shrinkY = (posY/2.f)+20;
					b2Body* tempBody;
					b2BodyDef tempDef;
					tempDef.type = b2_staticBody;
					tempDef.position.Set(float32(posX), float32(posY));

					tempBody = m_physicsWorld->CreateBody(&tempDef);

					tempPhsBody = PhysicsBody(entity, tempBody, float(physBodyX),
						float(physBodyY/2), vec2(0.f, 8.f), false, OBJECTS, PLAYER | ENEMY | OBJECTS | HEXAGON);
					tempPhsBody.SetColor(vec4(0.f, 0.f, 1.f, 0.3f));

				}
			}
			
			//PLATFORM 2 GROUP
			{
			float physBodyX = 140.f;
			float physBodyY = 62.f;
			float posX = -56.f;
			float posY = 60.f;
			float topOffset = -18.f;
				//small platform 2(textured)
				{
					//Creates entity
					auto entity = ECS::CreateEntity();

					//Add components
					ECS::AttachComponent<Sprite>(entity);
					ECS::AttachComponent<Transform>(entity);
					ECS::AttachComponent<PhysicsBody>(entity);

					//Sets up components
					std::string fileName = "WallTiles/platformBH.png";
					ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 144, 64);
					ECS::GetComponent<Transform>(entity).SetPosition(vec3(30.f, 0.f, 3.f));

					auto& tempSpr = ECS::GetComponent<Sprite>(entity);
					auto& tempPhsBody = ECS::GetComponent<PhysicsBody>(entity);

					float shrinkX = 0.f;
					float shrinkY = 26.f;
					b2Body* tempBody;
					b2BodyDef tempDef;
					tempDef.type = b2_staticBody;
					tempDef.position.Set(float32(posX), float32(posY));

					tempBody = m_physicsWorld->CreateBody(&tempDef);

					tempPhsBody = PhysicsBody(entity, tempBody, float(tempSpr.GetWidth() - shrinkX),
						float(tempSpr.GetHeight() - shrinkY), vec2(0.f, 12.f), false, WALL, PLAYER | ENEMY | OBJECTS | HEXAGON);
					tempPhsBody.SetColor(vec4(0.f, 1.f, 0.f, 0.3f));

				}
				//platform2 top half
				{
				//Creates entity
				auto entity = ECS::CreateEntity();

				//Add components
				ECS::AttachComponent<Sprite>(entity);
				ECS::AttachComponent<Transform>(entity);
				ECS::AttachComponent<PhysicsBody>(entity);

				//Sets up components
				std::string fileName = "WallTiles/platformtop.png";
				ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 144, 16);
				ECS::GetComponent<Transform>(entity).SetPosition(vec3(30.f, 0.f, 3.f));

				auto& tempSpr = ECS::GetComponent<Sprite>(entity);
				auto& tempPhsBody = ECS::GetComponent<PhysicsBody>(entity);
				b2Body* tempBody;
				b2BodyDef tempDef;
				tempDef.type = b2_staticBody;
				tempDef.position.Set(float32(posX), float32(posY + 40.f));

				tempBody = m_physicsWorld->CreateBody(&tempDef);

				tempPhsBody = PhysicsBody(entity, tempBody, float(tempSpr.GetWidth()),
					float(tempSpr.GetHeight()), vec2(0.f, topOffset), false, WALL, PLAYER | ENEMY | OBJECTS | HEXAGON);
				tempPhsBody.SetColor(vec4(0.f, 1.f, 0.f, 0.3f));

				}
				//small platform 2(top)
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
					tempDef.position.Set(float32(posX), float32(posY));

					tempBody = m_physicsWorld->CreateBody(&tempDef);

					tempPhsBody = PhysicsBody(entity, tempBody, float(physBodyX),
						float(physBodyY / 2), vec2(0.f, 16.f), false, GROUND, PLAYER | ENEMY | OBJECTS | HEXAGON);
					tempPhsBody.SetColor(vec4(1.f, 0.f, 0.f, 0.3f));

				}

				//small platform 2(bottom)
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
					float shrinkY = (posY / 2.f) + 20;
					b2Body* tempBody;
					b2BodyDef tempDef;
					tempDef.type = b2_staticBody;
					tempDef.position.Set(float32(posX), float32(posY));

					tempBody = m_physicsWorld->CreateBody(&tempDef);

					tempPhsBody = PhysicsBody(entity, tempBody, float(physBodyX),
						float(physBodyY / 2), vec2(0.f, 8.f), false, OBJECTS, PLAYER | ENEMY | OBJECTS | HEXAGON);
					tempPhsBody.SetColor(vec4(0.f, 0.f, 1.f, 0.3f));

				}
			}
			//SPIKE ON PLATFORM 3

			{

				//Creates entity
				auto entity = ECS::CreateEntity();

				//Add components
				ECS::AttachComponent<Sprite>(entity);
				ECS::AttachComponent<Transform>(entity);
				ECS::AttachComponent<PhysicsBody>(entity);

				//Sets up components
				std::string fileName = "Tower Inside/Spikes.png";
				ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 32, 32);
				ECS::GetComponent<Transform>(entity).SetPosition(vec3(30.f, -20.f, 5.f));

				auto& tempSpr = ECS::GetComponent<Sprite>(entity);
				auto& tempPhsBody = ECS::GetComponent<PhysicsBody>(entity);

				float shrinkX = 0.f;
				float shrinkY = 0.f;
				b2Body* tempBody;
				b2BodyDef tempDef;
				tempDef.type = b2_staticBody;
				tempDef.position.Set(float32(200.f), float32(110.f));

				tempBody = m_physicsWorld->CreateBody(&tempDef);

				tempPhsBody = PhysicsBody(entity, tempBody, float(tempSpr.GetWidth() - shrinkX),
					float(tempSpr.GetHeight() - shrinkY), vec2(0.f, 0.f), false, ENEMY, PLAYER | ENEMY | OBJECTS | HEXAGON);
				tempPhsBody.SetColor(vec4(0.f, 1.f, 0.f, 0.3f));

			}



			//PLATFORM 3 GROUP

			{
			float physBodyX = 140.f;
			float physBodyY = 62.f;
			float posX = 200.f;
			float posY = 80.f;
			float topOffset = -18.f;
			//small platform 3(textured)
			{
				//Creates entity
				auto entity = ECS::CreateEntity();

				//Add components
				ECS::AttachComponent<Sprite>(entity);
				ECS::AttachComponent<Transform>(entity);
				ECS::AttachComponent<PhysicsBody>(entity);

				//Sets up components
				std::string fileName = "WallTiles/platformBH.png";
				ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 144, 64);
				ECS::GetComponent<Transform>(entity).SetPosition(vec3(30.f, 0.f, 3.f));

				auto& tempSpr = ECS::GetComponent<Sprite>(entity);
				auto& tempPhsBody = ECS::GetComponent<PhysicsBody>(entity);

				float shrinkX = 0.f;
				float shrinkY = 26.f;
				b2Body* tempBody;
				b2BodyDef tempDef;
				tempDef.type = b2_staticBody;
				tempDef.position.Set(float32(posX), float32(posY));

				tempBody = m_physicsWorld->CreateBody(&tempDef);

				tempPhsBody = PhysicsBody(entity, tempBody, float(tempSpr.GetWidth() - shrinkX),
					float(tempSpr.GetHeight() - shrinkY), vec2(0.f, 12.f), false, WALL, PLAYER | ENEMY | OBJECTS | HEXAGON);
				tempPhsBody.SetColor(vec4(0.f, 1.f, 0.f, 0.3f));

			}
			//platform3 top half
			{
				//Creates entity
				auto entity = ECS::CreateEntity();

				//Add components
				ECS::AttachComponent<Sprite>(entity);
				ECS::AttachComponent<Transform>(entity);
				ECS::AttachComponent<PhysicsBody>(entity);

				//Sets up components
				std::string fileName = "WallTiles/platformtop.png";
				ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 144, 16);
				ECS::GetComponent<Transform>(entity).SetPosition(vec3(30.f, 0.f, 3.f));

				auto& tempSpr = ECS::GetComponent<Sprite>(entity);
				auto& tempPhsBody = ECS::GetComponent<PhysicsBody>(entity);
				b2Body* tempBody;
				b2BodyDef tempDef;
				tempDef.type = b2_staticBody;
				tempDef.position.Set(float32(posX), float32(posY + 40.f));

				tempBody = m_physicsWorld->CreateBody(&tempDef);

				tempPhsBody = PhysicsBody(entity, tempBody, float(tempSpr.GetWidth()),
					float(tempSpr.GetHeight()), vec2(0.f, topOffset), false, WALL, PLAYER | ENEMY | OBJECTS | HEXAGON);
				tempPhsBody.SetColor(vec4(0.f, 1.f, 0.f, 0.3f));

			}
			//small platform 3(top)
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
				tempDef.position.Set(float32(posX), float32(posY));

				tempBody = m_physicsWorld->CreateBody(&tempDef);

				tempPhsBody = PhysicsBody(entity, tempBody, float(physBodyX),
					float(physBodyY / 2), vec2(0.f, 16.f), false, GROUND, PLAYER | ENEMY | OBJECTS | HEXAGON);
				tempPhsBody.SetColor(vec4(1.f, 0.f, 0.f, 0.3f));

			}

			//small platform 3(bottom)
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
				float shrinkY = (posY / 2.f) + 20;
				b2Body* tempBody;
				b2BodyDef tempDef;
				tempDef.type = b2_staticBody;
				tempDef.position.Set(float32(posX), float32(posY));

				tempBody = m_physicsWorld->CreateBody(&tempDef);

				tempPhsBody = PhysicsBody(entity, tempBody, float(physBodyX),
					float(physBodyY / 2), vec2(0.f, 8.f), false, OBJECTS, PLAYER | ENEMY | OBJECTS | HEXAGON);
				tempPhsBody.SetColor(vec4(0.f, 0.f, 1.f, 0.3f));

			}
			}
			//PLATFORM 4 GROUP
			{
			float physBodyX = 140.f;
			float physBodyY = 62.f;
			float posX = 565.f;
			float posY = 60.f;
			float topOffset = -18.f;
			//small platform 3(textured)
			{
				//Creates entity
				auto entity = ECS::CreateEntity();

				//Add components
				ECS::AttachComponent<Sprite>(entity);
				ECS::AttachComponent<Transform>(entity);
				ECS::AttachComponent<PhysicsBody>(entity);

				//Sets up components
				std::string fileName = "WallTiles/platformBH.png";
				ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 144, 64);
				ECS::GetComponent<Transform>(entity).SetPosition(vec3(30.f, 0.f, 3.f));

				auto& tempSpr = ECS::GetComponent<Sprite>(entity);
				auto& tempPhsBody = ECS::GetComponent<PhysicsBody>(entity);

				float shrinkX = 0.f;
				float shrinkY = 26.f;
				b2Body* tempBody;
				b2BodyDef tempDef;
				tempDef.type = b2_staticBody;
				tempDef.position.Set(float32(posX), float32(posY));

				tempBody = m_physicsWorld->CreateBody(&tempDef);

				tempPhsBody = PhysicsBody(entity, tempBody, float(tempSpr.GetWidth() - shrinkX),
					float(tempSpr.GetHeight() - shrinkY), vec2(0.f, 12.f), false, WALL, PLAYER | ENEMY | OBJECTS | HEXAGON);
				tempPhsBody.SetColor(vec4(0.f, 1.f, 0.f, 0.3f));

			}
			//platform4 top half
			{
				//Creates entity
				auto entity = ECS::CreateEntity();

				//Add components
				ECS::AttachComponent<Sprite>(entity);
				ECS::AttachComponent<Transform>(entity);
				ECS::AttachComponent<PhysicsBody>(entity);

				//Sets up components
				std::string fileName = "WallTiles/platformtop.png";
				ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 144, 16);
				ECS::GetComponent<Transform>(entity).SetPosition(vec3(30.f, 0.f, 3.f));

				auto& tempSpr = ECS::GetComponent<Sprite>(entity);
				auto& tempPhsBody = ECS::GetComponent<PhysicsBody>(entity);
				b2Body* tempBody;
				b2BodyDef tempDef;
				tempDef.type = b2_staticBody;
				tempDef.position.Set(float32(posX), float32(posY + 40.f));

				tempBody = m_physicsWorld->CreateBody(&tempDef);

				tempPhsBody = PhysicsBody(entity, tempBody, float(tempSpr.GetWidth()),
					float(tempSpr.GetHeight()), vec2(0.f, topOffset), false, WALL, PLAYER | ENEMY | OBJECTS | HEXAGON);
				tempPhsBody.SetColor(vec4(0.f, 1.f, 0.f, 0.3f));

			}
			//small platform 4(top)
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
				tempDef.position.Set(float32(posX), float32(posY));

				tempBody = m_physicsWorld->CreateBody(&tempDef);

				tempPhsBody = PhysicsBody(entity, tempBody, float(physBodyX),
					float(physBodyY / 2), vec2(0.f, 16.f), false, GROUND, PLAYER | ENEMY | OBJECTS | HEXAGON);
				tempPhsBody.SetColor(vec4(1.f, 0.f, 0.f, 0.3f));

			}

			//small platform 4(bottom)
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
				float shrinkY = (posY / 2.f) + 20;
				b2Body* tempBody;
				b2BodyDef tempDef;
				tempDef.type = b2_staticBody;
				tempDef.position.Set(float32(posX), float32(posY));

				tempBody = m_physicsWorld->CreateBody(&tempDef);

				tempPhsBody = PhysicsBody(entity, tempBody, float(physBodyX),
					float(physBodyY / 2), vec2(0.f, 8.f), false, OBJECTS, PLAYER | ENEMY | OBJECTS | HEXAGON);
				tempPhsBody.SetColor(vec4(0.f, 0.f, 1.f, 0.3f));

			}
			//ROOF
			{
				for (int i = 0; i < 13; i++) 
					{
						//Creates entity
						auto entity = ECS::CreateEntity();

						//Add components
						ECS::AttachComponent<Sprite>(entity);
						ECS::AttachComponent<Transform>(entity);
						ECS::AttachComponent<PhysicsBody>(entity);

						//Sets up components
						std::string fileName = "Tower Inside/Base/Filling/ROOF.png";
						ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 64, 64);
						ECS::GetComponent<Transform>(entity).SetPosition(vec3(30.f, -20.f, 3.f));

						auto& tempSpr = ECS::GetComponent<Sprite>(entity);
						auto& tempPhsBody = ECS::GetComponent<PhysicsBody>(entity);

						float shrinkX = 0.f;
						float shrinkY = 0.f;
						b2Body* tempBody;
						b2BodyDef tempDef;
						tempDef.type = b2_staticBody;
						tempDef.position.Set(float32((i - 2) * 64.f), float32(300.f));

						tempBody = m_physicsWorld->CreateBody(&tempDef);

						tempPhsBody = PhysicsBody(entity, tempBody, float(tempSpr.GetWidth() - shrinkX),
							float(tempSpr.GetHeight() - shrinkY), vec2(0.f, 0.f), false, OBJECTS, PLAYER | ENEMY | OBJECTS | HEXAGON);
						tempPhsBody.SetColor(vec4(0.f, 1.f, 0.f, 0.3f));

					}



			}
			//LADDER TO NEXT LEVEL
			{

				//Creates entity
				auto entity = ECS::CreateEntity();

				//Add components
				ECS::AttachComponent<Sprite>(entity);
				ECS::AttachComponent<Transform>(entity);
				ECS::AttachComponent<PhysicsBody>(entity);
				ECS::AttachComponent<Trigger*>(entity);


				//Sets up components
				std::string fileName = "Tower Inside/Base/Filling/Ladder.png";
				ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 64, 192);
				ECS::GetComponent<Transform>(entity).SetPosition(vec3(30.f, -20.f, 4.f));
				ECS::GetComponent<Trigger*>(entity) = new LadderTrigger();
				ECS::GetComponent<Trigger*>(entity)->SetTriggerEntity(entity);

				auto& tempSpr = ECS::GetComponent<Sprite>(entity);
				auto& tempPhsBody = ECS::GetComponent<PhysicsBody>(entity);

				float shrinkX = 40.f;
				float shrinkY = 20.f;
				b2Body* tempBody;
				b2BodyDef tempDef;
				tempDef.type = b2_staticBody;
				tempDef.position.Set(float32(565.f), float32(185.f));

				tempBody = m_physicsWorld->CreateBody(&tempDef);

				tempPhsBody = PhysicsBody(entity, tempBody, float(tempSpr.GetWidth() - shrinkX),
					float(tempSpr.GetHeight() - shrinkY), vec2(0.f, 0.f), true, TRIGGER, PLAYER | OBJECTS);
				tempPhsBody.SetColor(vec4(0.f, 1.f, 0.f, 0.3f));

			}
			
			//DOOR TO BASEMENT CLOSED
			
				{

					//Creates entity
					auto entity = ECS::CreateEntity();
					ECS::SetIsDoorBasement(entity, true);
					//Add components
					ECS::AttachComponent<Sprite>(entity);
					ECS::AttachComponent<Transform>(entity);
					ECS::AttachComponent<PhysicsBody>(entity);
					ECS::AttachComponent<Door>(entity);

					//Sets up components
					std::string fileName = "Tower Inside/Base/Entrance TEMP.png";
					ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 64, 64);
					ECS::GetComponent<Transform>(entity).SetPosition(vec3(30.f, -20.f, 4.f));
					//ECS::GetComponent<Door>(entity).doorOpen = false;

					auto& tempSpr = ECS::GetComponent<Sprite>(entity);
					auto& tempPhsBody = ECS::GetComponent<PhysicsBody>(entity);

					float shrinkX = 0.f;
					float shrinkY = 0.f;
					b2Body* tempBody;
					b2BodyDef tempDef;
					tempDef.type = b2_staticBody;
					tempDef.position.Set(float32(250.f), float32(-60.f));

					tempBody = m_physicsWorld->CreateBody(&tempDef);
					
					tempPhsBody = PhysicsBody(entity, tempBody, float(tempSpr.GetWidth() - shrinkX),
						float(tempSpr.GetHeight() - shrinkY), vec2(0.f, 0.f), false, DOOR, PLAYER | ENEMY | OBJECTS | HEXAGON);
					tempPhsBody.SetColor(vec4(0.f, 1.f, 0.f, 0.3f));

				}
				
			//KEY
			{

				//Creates entity
				auto entity = ECS::CreateEntity();
				ECS::SetIsKeyOne(entity, true);

				//Add components
				ECS::AttachComponent<Sprite>(entity);
				ECS::AttachComponent<Transform>(entity);
				ECS::AttachComponent<PhysicsBody>(entity);

				//Sets up components
				std::string fileName = "Tower Inside/Key.png";
				ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 32, 32);
				ECS::GetComponent<Transform>(entity).SetPosition(vec3(30.f, -20.f, 4.f));

				auto& tempSpr = ECS::GetComponent<Sprite>(entity);
				auto& tempPhsBody = ECS::GetComponent<PhysicsBody>(entity);

				float shrinkX = 20.f;
				float shrinkY = 20.f;
				b2Body* tempBody;
				b2BodyDef tempDef;
				tempDef.type = b2_staticBody;
				tempDef.position.Set(float32(0.f), float32(-60.f));

				tempBody = m_physicsWorld->CreateBody(&tempDef);

				tempPhsBody = PhysicsBody(entity, tempBody, float(tempSpr.GetWidth() - shrinkX),
					float(tempSpr.GetHeight() - shrinkY), vec2(0.f, 0.f), false, KEY, PLAYER | ENEMY | OBJECTS | HEXAGON);
				tempPhsBody.SetColor(vec4(0.f, 0.f, 1.f, 0.3f));

			}

			}
			{
			//Creates entity
			auto entity = ECS::CreateEntity();

			//Add components
			ECS::AttachComponent<Sprite>(entity);
			ECS::AttachComponent<Transform>(entity);
			ECS::AttachComponent<PhysicsBody>(entity);

			//Sets up components
			std::string fileName = "WallTiles/Ground Floor Center BH.png";
			ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 64, 64);
			ECS::GetComponent<Transform>(entity).SetPosition(vec3(30.f, -20.f, 3.f));

			auto& tempSpr = ECS::GetComponent<Sprite>(entity);
			auto& tempPhsBody = ECS::GetComponent<PhysicsBody>(entity);

			float shrinkX = 0.f;
			float shrinkY = 0.f;
			b2Body* tempBody;
			b2BodyDef tempDef;
			tempDef.type = b2_staticBody;
			tempDef.position.Set(float32( -128.f), float32(-120.f));

			tempBody = m_physicsWorld->CreateBody(&tempDef);

			tempPhsBody = PhysicsBody(entity, tempBody, float(tempSpr.GetWidth() - shrinkX),
				float(tempSpr.GetHeight() - shrinkY), vec2(0.f, 0.f), false, GROUND, PLAYER | ENEMY | OBJECTS | HEXAGON);
			tempPhsBody.SetColor(vec4(0.f, 1.f, 0.f, 0.3f));

				}
			//FLOOR1
			for (int i = 2; i < 13; i++) {
				{
					//Creates entity
					auto entity = ECS::CreateEntity();

					//Add components
					ECS::AttachComponent<Sprite>(entity);
					ECS::AttachComponent<Transform>(entity);
					ECS::AttachComponent<PhysicsBody>(entity);

					//Sets up components
					std::string fileName = "WallTiles/Ground Floor Center BH.png";
					ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 64, 64);
					ECS::GetComponent<Transform>(entity).SetPosition(vec3(30.f, -20.f, 3.f));

					auto& tempSpr = ECS::GetComponent<Sprite>(entity);
					auto& tempPhsBody = ECS::GetComponent<PhysicsBody>(entity);

					float shrinkX = 0.f;
					float shrinkY = 0.f;
					b2Body* tempBody;
					b2BodyDef tempDef;
					tempDef.type = b2_staticBody;
					tempDef.position.Set(float32((i-2)*64.f), float32(-120.f));

					tempBody = m_physicsWorld->CreateBody(&tempDef);

					tempPhsBody = PhysicsBody(entity, tempBody, float(tempSpr.GetWidth() - shrinkX),
						float(tempSpr.GetHeight() - shrinkY), vec2(0.f, 0.f), false, GROUND, PLAYER | ENEMY | OBJECTS | HEXAGON);
					tempPhsBody.SetColor(vec4(0.f, 1.f, 0.f, 0.3f));

				}
				{
					//Creates entity
					auto entity = ECS::CreateEntity();

					//Add components
					ECS::AttachComponent<Sprite>(entity);
					ECS::AttachComponent<Transform>(entity);

					//Sets up components
					std::string fileName = "WallTiles/Ground Floor TH Alternate.png";
					ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 64, 64);
					ECS::GetComponent<Sprite>(entity).SetTransparency(1.f);
					ECS::GetComponent<Transform>(entity).SetPosition(vec3((i - 2) * 64.f, -56.f, 2.f));
					

				}
			}
			//SPIKE ON Ground

			for (int i = 0; i < 3; i++) {
				{

					//Creates entity
					auto entity = ECS::CreateEntity();

					//Add components
					ECS::AttachComponent<Sprite>(entity);
					ECS::AttachComponent<Transform>(entity);
					ECS::AttachComponent<PhysicsBody>(entity);

					//Sets up components
					std::string fileName = "Tower Inside/Spikes.png";
					ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 32, 32);
					ECS::GetComponent<Transform>(entity).SetPosition(vec3(30.f, -20.f, 4.f));

					auto& tempSpr = ECS::GetComponent<Sprite>(entity);
					auto& tempPhsBody = ECS::GetComponent<PhysicsBody>(entity);

					float shrinkX = 0.f;
					float shrinkY = 0.f;
					b2Body* tempBody;
					b2BodyDef tempDef;
					tempDef.type = b2_staticBody;
					tempDef.position.Set(float32(264.f+(32*i)), float32(-90.f));

					tempBody = m_physicsWorld->CreateBody(&tempDef);

					tempPhsBody = PhysicsBody(entity, tempBody, float(tempSpr.GetWidth() - shrinkX),
						float(tempSpr.GetHeight() - shrinkY), vec2(0.f, 0.f), false, ENEMY, PLAYER | ENEMY | OBJECTS | HEXAGON);
					tempPhsBody.SetColor(vec4(0.f, 1.f, 0.f, 0.3f));

				}

			}

		}

	}


	
	




	ECS::GetComponent<HorizontalScroll>(MainEntities::MainCamera()).SetFocus(&ECS::GetComponent<Transform>(MainEntities::MainPlayer()));
	ECS::GetComponent<VerticalScroll>(MainEntities::MainCamera()).SetFocus(&ECS::GetComponent<Transform>(MainEntities::MainPlayer()));
}

void TowerGroundScene::Update()
{
	auto& door = ECS::GetComponent<Door>(MainEntities::doorBasement());
	auto& removeDoor = ECS::GetComponent<PhysicsBody>(MainEntities::doorBasement());
	//DOOR TO BASEMENT OPEN
	if (door.doorOpen) {
		removeDoor.SetPosition(b2Vec2(-100, -400), true);
		{

			//Creates entity
			auto entity = ECS::CreateEntity();
			ECS::SetIsDoorBasement(entity, true);
			//Add components
			ECS::AttachComponent<Sprite>(entity);
			ECS::AttachComponent<Transform>(entity);
			ECS::AttachComponent<PhysicsBody>(entity);
			ECS::AttachComponent<Door>(entity);

			//Sets up components
			std::string fileName = "Tower Inside/Base/Entrance TEMP.png";
			ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 64, 64);
			ECS::GetComponent<Transform>(entity).SetPosition(vec3(30.f, -20.f, 4.f));

			auto& tempSpr = ECS::GetComponent<Sprite>(entity);
			auto& tempPhsBody = ECS::GetComponent<PhysicsBody>(entity);

			float shrinkX = 40.f;
			float shrinkY = 40.f;
			b2Body* tempBody;
			b2BodyDef tempDef;
			tempDef.type = b2_staticBody;
			tempDef.position.Set(float32(250.f), float32(-60.f));

			tempBody = m_physicsWorld->CreateBody(&tempDef);

			tempPhsBody = PhysicsBody(entity, tempBody, float(tempSpr.GetWidth() - shrinkX),
				float(tempSpr.GetHeight() - shrinkY), vec2(0.f, 0.f), false, DOOR, PLAYER | ENEMY | OBJECTS | HEXAGON);
			tempPhsBody.SetColor(vec4(1.f, 0.f, 0.f, 0.3f));

		}
		auto& door = ECS::GetComponent<Door>(MainEntities::doorBasement());
		door.doorOpen = false;
		door.doorTransport = true;
	}
	
}

void TowerGroundScene::KeyboardHold()
{
	auto& player = ECS::GetComponent<PhysicsBody>(MainEntities::MainPlayer());
	auto& ladder = ECS::GetComponent<CanJump>(MainEntities::MainPlayer());

	float speed = 3.f;
	b2Vec2 vel = b2Vec2(0.f, 0.f);

	
	if (Input::GetKey(Key::A))
	{
		player.GetBody()->ApplyForceToCenter(b2Vec2(-150000.f * speed, 0.f), true);
	}
	if (Input::GetKey(Key::D))
	{
		player.GetBody()->ApplyForceToCenter(b2Vec2(150000.f * speed, 0.f), true);
	}
	if (ladder.enableLadder) {
		if (Input::GetKey(Key::W)) {
			player.SetPosition(b2Vec2(player.GetPosition().x, player.GetPosition().y + 1));
		}
	}
	

}

void TowerGroundScene::KeyboardDown()
{
	auto& player = ECS::GetComponent<PhysicsBody>(MainEntities::MainPlayer());
	auto& canJump = ECS::GetComponent<CanJump>(MainEntities::MainPlayer());
	auto& playerTrans = ECS::GetComponent<Transform>(MainEntities::MainPlayer());
	


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
		//player.SetPosition(b2Vec2(565, 200), true);
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

void TowerGroundScene::KeyboardUp()
{
	auto& player = ECS::GetComponent<PhysicsBody>(MainEntities::MainPlayer());

	




}