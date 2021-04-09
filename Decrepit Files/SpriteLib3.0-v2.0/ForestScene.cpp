#include "ForestScene.h"
#include "Utilities.h"
#include <random>
#include "Platform.h"
#include "Door.h"



ForestScene::ForestScene(std::string name)
	: Scene(name)
{
	//No gravity this is a top down scene
	m_gravity = b2Vec2(0.f, -980.f);
	m_physicsWorld->SetGravity(m_gravity);

	m_physicsWorld->SetContactListener(&listener);
	
	
	
}

int ForestScene::getSceneChange() {
	return m_sceneChange;
}

void ForestScene::setSceneChange(int sc) {
	m_sceneChange = sc;
}


void ForestScene::spawnLeftWall() {
	{
		

		
			{
				//Creates entity
				auto entity = ECS::CreateEntity();

				//Add components
				ECS::AttachComponent<Sprite>(entity);
				ECS::AttachComponent<Transform>(entity);
				ECS::AttachComponent<PhysicsBody>(entity);

				//Sets up components
				std::string fileName = "ForestTiles/Dirt/Wall_L_R_Object.png";
				ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 256, 512);
				ECS::GetComponent<Transform>(entity).SetPosition(vec3(0.f, 0.f, 3.f));

				auto& tempSpr = ECS::GetComponent<Sprite>(entity);
				auto& tempPhsBody = ECS::GetComponent<PhysicsBody>(entity);
				
				
				float shrinkX = 0.f;
				float shrinkY = 0.f;
				b2Body* tempBody;
				b2BodyDef tempDef;
				tempDef.type = b2_staticBody;
				tempDef.position.Set(float32(-256.f), float32(256.f));

				tempBody = m_physicsWorld->CreateBody(&tempDef);

				tempPhsBody = PhysicsBody(entity, tempBody, float(tempSpr.GetWidth() - shrinkX),
					float(tempSpr.GetHeight() - shrinkY), vec2(0.f, 0.f), false, OBJECTS, PLAYER | ENEMY | OBJECTS);
				tempPhsBody.SetColor(vec4(0.f, 1.f, 0.f, 0.3f));


			}	
	}
}

void ForestScene::spawnGround() 
{
	int groundSpawn = 720;
	for (int i = 0; i <= 1; i++) {
		{
			//Creates entity
			auto entity = ECS::CreateEntity();

			//Add components
			ECS::AttachComponent<Sprite>(entity);
			ECS::AttachComponent<Transform>(entity);
			ECS::AttachComponent<PhysicsBody>(entity);

			//Sets up components
			std::string fileName = "ForestTiles/Dirt/BASE.png";
			ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 2880, 192);
			ECS::GetComponent<Transform>(entity).SetPosition(vec3(0.f, 0.f, 4.f));

			auto& tempSpr = ECS::GetComponent<Sprite>(entity);
			auto& tempPhsBody = ECS::GetComponent<PhysicsBody>(entity);

			float shrinkX = 0.f;
			float shrinkY = 0.f;
			b2Body* tempBody;
			b2BodyDef tempDef;
			tempDef.type = b2_staticBody;
			tempDef.position.Set(float32(groundSpawn), float32(-352.f));

			tempBody = m_physicsWorld->CreateBody(&tempDef);

			tempPhsBody = PhysicsBody(entity, tempBody, float(tempSpr.GetWidth() - shrinkX),
				float(tempSpr.GetHeight() - shrinkY), vec2(0.f, 0.f), false, ENEMY, PLAYER | ENEMY | OBJECTS);
			tempPhsBody.SetColor(vec4(0.f, 1.f, 0.f, 0.3f));

		}

		groundSpawn = groundSpawn + 2880;
	}
}


void ForestScene::spawnSprite(int xGround, int yGround, int xGrass, int yGrass, int grassAmount ,int xGroundBox, int yGroundBox, int boxX, int boxY, int xSprite, int ySprite, int depth ,std::string fileN) {

	//Spawning Sprite
	{
	
		int fillGrass = 0;


		for (int i = 0; i <= grassAmount; i++) {

			{
				//Setup new Entity

				/*Scene::CreateSprite(m_sceneReg, "HelloWorld.png", 100, 60, 0.5f, vec3(0.f, 0.f, 0.f));*/

				//Creates entity
				auto entity = ECS::CreateEntity();

				//Add components
				ECS::AttachComponent<Sprite>(entity);
				ECS::AttachComponent<Transform>(entity);

				//Set up the components
				std::string fileName = "ForestTiles/Grass/Background_Grass.png";
				ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 64, 32);
				ECS::GetComponent<Sprite>(entity).SetTransparency(.9f);
				ECS::GetComponent<Transform>(entity).SetPosition(vec3(xGrass + fillGrass, yGrass, 3.f));

			}


			{
				//Setup new Entity

				//Creates entity
				auto entity = ECS::CreateEntity();

				//Add components
				ECS::AttachComponent<Sprite>(entity);
				ECS::AttachComponent<Transform>(entity);

				//Set up the components
				std::string fileName = "ForestTiles/Grass/Foreground_Grass.png";
				ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 64, 32);
				ECS::GetComponent<Sprite>(entity).SetTransparency(.9f);
				ECS::GetComponent<Transform>(entity).SetPosition(vec3(xGrass + fillGrass, yGrass - 4.f, 5.f));

			}

			fillGrass = fillGrass + 64;
		}


	

		{
			//Creates entity
			auto entity = ECS::CreateEntity();

			//Add components
			ECS::AttachComponent<Sprite>(entity);
			ECS::AttachComponent<Transform>(entity);
			ECS::AttachComponent<PhysicsBody>(entity);

			//Sets up components
			std::string fileName = fileN;
			ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, xSprite, ySprite);
			ECS::GetComponent<Transform>(entity).SetPosition(vec3(0.f, 0.f, 4.f));

			auto& tempSpr = ECS::GetComponent<Sprite>(entity);
			auto& tempPhsBody = ECS::GetComponent<PhysicsBody>(entity);

			float shrinkX = 0.f;
			float shrinkY = 0.f;
			b2Body* tempBody;
			b2BodyDef tempDef;
			tempDef.type = b2_staticBody;
			tempDef.position.Set(float32(xGround), float32(yGround));

			tempBody = m_physicsWorld->CreateBody(&tempDef);

			tempPhsBody = PhysicsBody(entity, tempBody, float(tempSpr.GetWidth() - shrinkX),
				float(tempSpr.GetHeight() - shrinkY), vec2(0.f, 0.f), false, WALL, PLAYER | ENEMY | OBJECTS);
			tempPhsBody.SetColor(vec4(0.f, 1.f, 0.f, 0.3f));


		}


		float physBodyX = boxX;
		float physBodyY = boxY;

		{
			//Creates entity
			auto entity = ECS::CreateEntity();

			//Add components
			ECS::AttachComponent<Transform>(entity);
			ECS::AttachComponent<PhysicsBody>(entity);

			//Sets up components
			ECS::GetComponent<Transform>(entity).SetPosition(vec3(0.f, 0.f, 5.f));
			auto& tempPhsBody = ECS::GetComponent<PhysicsBody>(entity);

			float shrinkX = 0.f;
			float shrinkY = 0.f;
			b2Body* tempBody;
			b2BodyDef tempDef;
			tempDef.type = b2_staticBody;
			tempDef.position.Set(float32(xGroundBox), float32(yGroundBox));

			tempBody = m_physicsWorld->CreateBody(&tempDef);

			tempPhsBody = PhysicsBody(entity, tempBody, float(physBodyX),
				float(physBodyY / 2), vec2(0.f, 16.f), false, GROUND, PLAYER | ENEMY | OBJECTS);
			tempPhsBody.SetColor(vec4(1.f, 0.f, 0.f, 0.3f));

		}

	}

}









void ForestScene::InitScene(float windowWidth, float windowHeight)
{



	//Dynamically allocates the register
	m_sceneReg = new entt::registry;
	m_physicsWorld = new b2World(m_gravity);

	//No gravity this is a top down scene
	m_gravity = b2Vec2(0.f, -98.f);
	m_physicsWorld->SetGravity(m_gravity);

	m_physicsWorld->SetContactListener(&listener);

	//Attach the register
	ECS::AttachRegister(m_sceneReg);



	//Sets up aspect ratio for the camera
	float aspectRatio = windowWidth / windowHeight;

	
	//Spawning all of the forest Objects
	{
		//Left Wall
		ForestScene::spawnLeftWall();

		//Underground 
		ForestScene::spawnGround();

		//Platforms              GrX  /   GrY /   GrsX / GrsY/ GrsA/ GrbX /  GrbY /  Bx  /  By  / SprX / SprY / Dpth / FileName
		ForestScene::spawnSprite(-128.f, -128.f,  -96.f,  16.f, 2.f,  -32.f, -23.f, 192.f, 32.f, 384.f, 256.f, 2.f, "ForestTiles/Dirt/LargePlatform_1.png");
		ForestScene::spawnSprite( 384.f, -128.f,  288.f,  16.f, 3.f,  384.f, -23.f, 256.f, 32.f, 256.f, 256.f, 2.f, "ForestTiles/Dirt/Platform_2.png");
		ForestScene::spawnSprite( 896.f, -128.f,  802.f,  16.f, 3.f,  896.f, -23.f, 256.f, 32.f, 256.f, 256.f, 2.f, "ForestTiles/Dirt/Platform_3.png");
		ForestScene::spawnSprite(1056.f, -160.f, 1056.f, -48.f, 0.f, 1056.f, -87.f,  64.f, 32.f,  64.f, 192.f, 2.f, "ForestTiles/Dirt/Connector_Platform.png");
		ForestScene::spawnSprite(1152.f, -192.f, 1120.f,-112.f, 1.f, 1152.f,-151.f, 128.f, 32.f, 128.f, 128.f, 2.f, "ForestTiles/Dirt/Low_Ground_Right.png");
		ForestScene::spawnSprite(1344.f, -192.f, 1312.f,-112.f, 1.f, 1344.f,-151.f, 128.f, 32.f, 128.f, 128.f, 2.f, "ForestTiles/Dirt/Low_Ground_Middle.png");
		ForestScene::spawnSprite(1600.f, -192.f, 1568.f,-112.f, 1.f, 1600.f,-151.f, 128.f, 32.f, 128.f, 128.f, 2.f, "ForestTiles/Dirt/Low_Ground_Left.png");
		ForestScene::spawnSprite(1696.f, -160.f, 1696.f, -48.f, 0.f, 1696.f, -87.f,  64.f, 32.f,  64.f, 192.f, 2.f, "ForestTiles/Dirt/Connector_Platform_Left.png");
		ForestScene::spawnSprite(1856.f, -128.f, 1760.f,  16.f, 3.f, 1856.f, -23.f, 256.f, 32.f, 256.f, 256.f, 2.f, "ForestTiles/Dirt/Platform_4.png");
		ForestScene::spawnSprite(2144.f,    0.f, 2080.f, 144.f, 2.f, 2144.f, 105.f, 192.f, 32.f, 192.f, 256.f, 2.f, "ForestTiles/Dirt/Floating_Right.png");
		ForestScene::spawnSprite(2176.f, -224.f, 2080.f,-178.f, 3.f, 2176.f,-215.f, 256.f, 32.f, 256.f,  64.f, 2.f, "ForestTiles/Dirt/Long_Bottom.png");
		ForestScene::spawnSprite(2304.f,   64.f, 2272.f, 144.f, 1.f, 2304.f, 105.f, 128.f, 32.f, 128.f, 128.f, 2.f, "ForestTiles/Dirt/Floating_Small.png");
		ForestScene::spawnSprite(2368.f, -192.f, 2336.f,-112.f, 1.f, 2368.f,-151.f, 128.f, 32.f, 128.f, 128.f, 2.f, "ForestTiles/Dirt/Low_Ground_Left2.png");
		ForestScene::spawnSprite(2496.f, -160.f, 2464.f, -48.f, 1.f, 2496.f, -85.f, 128.f, 32.f, 128.f, 192.f, 2.f, "ForestTiles/Dirt/Lower_Long_Right.png");
		ForestScene::spawnSprite(2944.f, -160.f, 2722.f, -48.f, 7.f, 2944.f, -85.f, 512.f, 32.f, 512.f, 192.f, 2.f, "ForestTiles/Dirt/Long_Mid_End.png");
	}


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

		//-75.f, 175.f, -80.f, 175.f
		vec4 temp = vec4(-155.f, 300.f, -200.f, 150.f);
		ECS::GetComponent<Camera>(entity).SetOrthoSize(temp);
		ECS::GetComponent<Camera>(entity).SetWindowSize(vec2(float(windowWidth), float(windowHeight)));
		ECS::GetComponent<Camera>(entity).Orthographic(aspectRatio, temp.x, temp.y, temp.z, temp.w, -100.f, 100.f);

		//Attaches the camera to vert and horiz scrolls
		ECS::GetComponent<HorizontalScroll>(entity).SetCam(&ECS::GetComponent<Camera>(entity));
		ECS::GetComponent<VerticalScroll>(entity).SetCam(&ECS::GetComponent<Camera>(entity));
	}

	int skyPosition = 640;
	int forestPosition = 1500;
	int fogPosition = 832;

	for (int i = 0; i <= 1; i++) {
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
			ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 3400, 2000);
			ECS::GetComponent<Sprite>(entity).SetTransparency(1.f);
			ECS::GetComponent<Transform>(entity).SetPosition(vec3(forestPosition, 200.f, 1.f));
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
			ECS::GetComponent<Transform>(entity).SetPosition(vec3(skyPosition, 100.f, 0.f));
		}

		//Setup new Entity
		{


			//Creates entity
			auto entity = ECS::CreateEntity();

			//Add components
			ECS::AttachComponent<Sprite>(entity);
			ECS::AttachComponent<Transform>(entity);

			//Set up the components
			std::string fileName = "ForestTiles/Dirt/Fog_Black_Fullscreen.png";
			ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 2815, 384);
			ECS::GetComponent<Sprite>(entity).SetTransparency(0.7f);
			ECS::GetComponent<Transform>(entity).SetPosition(vec3(fogPosition, -256.f, 8.f));
		}
		fogPosition = fogPosition + 2815;
		skyPosition = skyPosition + 1280;
		forestPosition = forestPosition + 3400;



	}



	//All Dem Pits
	{
		{
			{

				//Creates entity
				auto entity = ECS::CreateEntity();

				//Add components
				ECS::AttachComponent<Sprite>(entity);
				ECS::AttachComponent<Transform>(entity);

				//Set up the components
				std::string fileName = "ForestTiles/Dirt/Pit_2.png";
				ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 256, 256);
				ECS::GetComponent<Sprite>(entity).SetTransparency(1.f);
				ECS::GetComponent<Transform>(entity).SetPosition(vec3(160.f, -128.f, 3.f));
			}
			{
				auto entity = ECS::CreateEntity();

				//Add components
				ECS::AttachComponent<Sprite>(entity);
				ECS::AttachComponent<Transform>(entity);

				//Set up the components
				std::string fileName = "ForestTiles/Spikes_3.png";
				ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 192, 64);
				ECS::GetComponent<Sprite>(entity).SetTransparency(1.f);
				ECS::GetComponent<Transform>(entity).SetPosition(vec3(160.f, -256.f, 5.f));
			}
		}

		{
			{

				//Creates entity
				auto entity = ECS::CreateEntity();

				//Add components
				ECS::AttachComponent<Sprite>(entity);
				ECS::AttachComponent<Transform>(entity);

				//Set up the components
				std::string fileName = "ForestTiles/Dirt/Pit_3.png";
				ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 384, 256);
				ECS::GetComponent<Sprite>(entity).SetTransparency(1.f);
				ECS::GetComponent<Transform>(entity).SetPosition(vec3(640.f, -128.f, 3.f));
			}

			{
				auto entity = ECS::CreateEntity();

				//Add components
				ECS::AttachComponent<Sprite>(entity);
				ECS::AttachComponent<Transform>(entity);

				//Set up the components
				std::string fileName = "ForestTiles/Spikes_4.png";
				ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 256, 64);
				ECS::GetComponent<Sprite>(entity).SetTransparency(1.f);
				ECS::GetComponent<Transform>(entity).SetPosition(vec3(640.f, -256.f, 5.f));
			}
		}

		{
			{

				//Creates entity
				auto entity = ECS::CreateEntity();

				//Add components
				ECS::AttachComponent<Sprite>(entity);
				ECS::AttachComponent<Transform>(entity);

				//Set up the components
				std::string fileName = "ForestTiles/Dirt/Pit_VeryLarge.png";
				ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 768, 256);
				ECS::GetComponent<Sprite>(entity).SetTransparency(1.f);
				ECS::GetComponent<Transform>(entity).SetPosition(vec3(1376.f, -192.f, 3.f));
			}

			{
				auto entity = ECS::CreateEntity();

				//Add components
				ECS::AttachComponent<Sprite>(entity);
				ECS::AttachComponent<Transform>(entity);

				//Set up the components
				std::string fileName = "ForestTiles/Spikes.png";
				ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 64, 64);
				ECS::GetComponent<Sprite>(entity).SetTransparency(1.f);
				ECS::GetComponent<Transform>(entity).SetPosition(vec3(1248.f, -256.f, 5.f));

			}
		}
		{
			{

				//Creates entity
				auto entity = ECS::CreateEntity();

				//Add components
				ECS::AttachComponent<Sprite>(entity);
				ECS::AttachComponent<Transform>(entity);

				//Set up the components
				std::string fileName = "ForestTiles/Dirt/Pit_VeryLarge.png";
				ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 768, 256);
				ECS::GetComponent<Sprite>(entity).SetTransparency(1.f);
				ECS::GetComponent<Transform>(entity).SetPosition(vec3(2330.f, -192.f, 3.f));
			}

			{
				auto entity = ECS::CreateEntity();

				//Add components
				ECS::AttachComponent<Sprite>(entity);
				ECS::AttachComponent<Transform>(entity);

				//Set up the components
				std::string fileName = "ForestTiles/Spikes_2.png";
				ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 128, 64);
				ECS::GetComponent<Sprite>(entity).SetTransparency(1.f);
				ECS::GetComponent<Transform>(entity).SetPosition(vec3(1472.f, -256.f, 5.f));

			}
		}

		{
			auto entity = ECS::CreateEntity();

			//Add components
			ECS::AttachComponent<Sprite>(entity);
			ECS::AttachComponent<Transform>(entity);

			//Set up the components
			std::string fileName = "ForestTiles/Spikes.png";
			ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 64, 64);
			ECS::GetComponent<Sprite>(entity).SetTransparency(1.f);
			ECS::GetComponent<Transform>(entity).SetPosition(vec3(2016.f, -256.f, 5.f));

		}

		{
			auto entity = ECS::CreateEntity();

			//Add components
			ECS::AttachComponent<Sprite>(entity);
			ECS::AttachComponent<Transform>(entity);

			//Set up the components
			std::string fileName = "ForestTiles/Spikes_2.png";
			ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 128, 64);
			ECS::GetComponent<Sprite>(entity).SetTransparency(1.f);
			ECS::GetComponent<Transform>(entity).SetPosition(vec3(2624.f, -256.f, 5.f));

		}

		//Tower Entrance
		{
			{
				auto entity = ECS::CreateEntity();

				//Add components
				ECS::AttachComponent<Sprite>(entity);
				ECS::AttachComponent<Transform>(entity);

				//Set up the components
				std::string fileName = "ForestTiles/Left_Entrance.png";
				ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 192, 256);
				ECS::GetComponent<Sprite>(entity).SetTransparency(1.f);
				ECS::GetComponent<Transform>(entity).SetPosition(vec3(2816.f, 64.f, 4.f));

			}

			{
				auto entity = ECS::CreateEntity();

				//Add components
				ECS::AttachComponent<Sprite>(entity);
				ECS::AttachComponent<Transform>(entity);

				//Set up the components
				std::string fileName = "ForestTiles/Right_Entrance.png";
				ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 192, 256);
				ECS::GetComponent<Sprite>(entity).SetTransparency(1.f);
				ECS::GetComponent<Transform>(entity).SetPosition(vec3(3008.f, 64.f, 4.f));

			}
		}

		{
			//Creates entity
			auto entity = ECS::CreateEntity();

			//Add components
			ECS::AttachComponent<Transform>(entity);
			ECS::AttachComponent<PhysicsBody>(entity);

			//Sets up components
			ECS::GetComponent<Transform>(entity).SetPosition(vec3(0.f, 0.f, 5.f));
			auto& tempPhsBody = ECS::GetComponent<PhysicsBody>(entity);

			float shrinkX = 0.f;
			float shrinkY = 0.f;
			b2Body* tempBody;
			b2BodyDef tempDef;
			tempDef.type = b2_staticBody;
			tempDef.position.Set(float32(2954.f), float32(-32.f));

			tempBody = m_physicsWorld->CreateBody(&tempDef);

			tempPhsBody = PhysicsBody(entity, tempBody, float(32),
				float(126 / 2), vec2(0.f, 16.f), false, TRIGGER, PLAYER | ENEMY | OBJECTS);
			tempPhsBody.SetColor(vec4(1.f, 0.f, 0.f, 0.3f));

		}

		{
			
			auto entity = ECS::CreateEntity();
			//Add components

			ECS::AttachComponent<Sprite>(entity);
			ECS::AttachComponent<Transform>(entity);

			//Set up the components
			std::string fileName = "ForestTiles/E_to_enter.png";
			ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 128, 128);
			ECS::GetComponent<Sprite>(entity).SetTransparency(1.f);
			ECS::GetComponent<Transform>(entity).SetPosition(vec3(3008.f, 64.f, 5.f));
			
		}
	}

	//Animated Objects
	{
		//Character entity
		{
			/*Scene::CreatePhysicsSprite(m_sceneReg, "LinkStandby", 80, 60, 1.f, vec3(0.f, 30.f, 2.f), b2_dynamicBody, 0.f, 0.f, true, true)*/



			auto entity = ECS::CreateEntity();
			ECS::SetIsMainPlayer(entity, true);
			std::cout << entity << std::endl;

			auto animations = File::LoadJSON("player_animations.json");

			//Add components
			ECS::AttachComponent<Player>(entity);
			ECS::AttachComponent<Sprite>(entity);
			ECS::AttachComponent<Transform>(entity);
			ECS::AttachComponent<PhysicsBody>(entity);
			ECS::AttachComponent<CanJump>(entity);
			ECS::AttachComponent<AnimationController>(entity);

			//Sets up the components
			std::string fileName = "spritesheets/PNG/player_spritesheet.png";
			auto& animController = ECS::GetComponent<AnimationController>(entity);




			animController.InitUVs(fileName);

			//std::string animations = "player_animations.json";

			//ECS::GetComponent<Player>(entity).InitPlayer(fileName, animations, 25, 25, &ECS::GetComponent<Sprite>(entity), &ECS::GetComponent<AnimationController>(entity), &ECS::GetComponent<Transform>(entity));


			//Idle Left
			animController.AddAnimation(animations["IdleLeft"]);	//0
			//Idle Right
			animController.AddAnimation(animations["IdleRight"]);	//1
			//WalkLeft
			animController.AddAnimation(animations["WalkLeft"]);	//2
			//WalkRight
			animController.AddAnimation(animations["WalkRight"]);	//3
			//Fall Left
			animController.AddAnimation(animations["FallLeft"]);	//4
			//Fall Right
			animController.AddAnimation(animations["FallRight"]);	//5
			//Jump Left
			animController.AddAnimation(animations["JumpLeft"]);	//6
			//Jump Right
			animController.AddAnimation(animations["JumpRight"]);	//7

			animController.SetActiveAnim(1);

			ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 25, 25, true, &animController);
			ECS::GetComponent<Sprite>(entity).SetTransparency(1.f);
			ECS::GetComponent<Transform>(entity).SetPosition(vec3(0.f, 30.f, 4.f));


			auto& tempSpr = ECS::GetComponent<Sprite>(entity);
			auto& tempPhsBody = ECS::GetComponent<PhysicsBody>(entity);

			float shrinkX = 0.f;
			float shrinkY = 0.f;

			b2Body* tempBody;
			b2BodyDef tempDef;
			tempDef.type = b2_dynamicBody;
			tempDef.position.Set(float32(20.f), float32(20.f));

			tempBody = m_physicsWorld->CreateBody(&tempDef);

			tempPhsBody = PhysicsBody(entity, tempBody, float((tempSpr.GetHeight() - shrinkY) / 2.f), vec2(0.f, 0.f), false, PLAYER, ENVIRONMENT | ENEMY | OBJECTS | TRIGGER | HEXAGON, 1.f, 4.5f);





			tempPhsBody.SetRotationAngleDeg(0.f);
			tempPhsBody.SetFixedRotation(true);
			tempPhsBody.SetColor(vec4(1.f, 0.f, 1.f, 0.3f));
			tempPhsBody.SetGravityScale(1.f);
		}



		//Setup new Entity
		{
			auto entity = ECS::CreateEntity();
			auto Abutton = File::LoadJSON("AButton.json");
			ECS::AttachComponent<Sprite>(entity);
			ECS::AttachComponent<Transform>(entity);
			ECS::AttachComponent<AnimationController>(entity);
			std::string fileName = "spritesheets/PNG/ASpriteSheet.png";

			auto& animController = ECS::GetComponent<AnimationController>(entity);
			animController.InitUVs(fileName);
			animController.AddAnimation(Abutton["AButton"]); // 8
			animController.SetActiveAnim(0);
			ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 32, 32, true, &animController);

			ECS::GetComponent<Sprite>(entity).SetTransparency(1.f);
			ECS::GetComponent<Transform>(entity).SetPosition(vec3(-64.f, 100.f, 2.f));
		}


		//Setup new Entity
		{
			auto entity = ECS::CreateEntity();
			auto Dbutton = File::LoadJSON("DButton.json");
			ECS::AttachComponent<Sprite>(entity);
			ECS::AttachComponent<Transform>(entity);
			ECS::AttachComponent<AnimationController>(entity);
			std::string fileName = "spritesheets/PNG/DSpriteSheet.png";

			auto& animController = ECS::GetComponent<AnimationController>(entity);
			animController.InitUVs(fileName);
			animController.AddAnimation(Dbutton["DButton"]); // 9
			animController.SetActiveAnim(0);
			ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 32, 32, true, &animController);

			ECS::GetComponent<Sprite>(entity).SetTransparency(1.f);
			ECS::GetComponent<Transform>(entity).SetPosition(vec3(0.f, 100.f, 2.f));
		}

		//Setup new Entity
		{
			auto entity = ECS::CreateEntity();
			auto Spacebutton = File::LoadJSON("Space.json");
			ECS::AttachComponent<Sprite>(entity);
			ECS::AttachComponent<Transform>(entity);
			ECS::AttachComponent<AnimationController>(entity);
			std::string fileName = "spritesheets/PNG/SpaceSpriteSheet.png";

			auto& animController = ECS::GetComponent<AnimationController>(entity);
			animController.InitUVs(fileName);
			animController.AddAnimation(Spacebutton["Space"]); // 10
			animController.SetActiveAnim(0);
			ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 64, 32, true, &animController);

			ECS::GetComponent<Sprite>(entity).SetTransparency(1.f);
			ECS::GetComponent<Transform>(entity).SetPosition(vec3(164.f, 0.f, 2.f));
		}

		{

			auto entity = ECS::CreateEntity();
			auto Jelly = File::LoadJSON("Jelly.json");
			ECS::AttachComponent<Sprite>(entity);
			ECS::AttachComponent<Transform>(entity);
			ECS::AttachComponent<AnimationController>(entity);
			std::string fileName = "spritesheets/PNG/Jellysheet.png";

			auto& animController = ECS::GetComponent<AnimationController>(entity);
			
			animController.InitUVs(fileName);
			animController.AddAnimation(Jelly["Jelly"]); // 11
			animController.SetActiveAnim(0);
			
			auto& anim = animController.GetAnimation(0);
			anim.SetSecPerFrame(.05f);

			ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 64, 64, true, &animController);

			ECS::GetComponent<Sprite>(entity).SetTransparency(1.f);
			ECS::GetComponent<Transform>(entity).SetPosition(vec3(205.f, -18.f, 3.f));
		}

		//Setup new Entity
		{
			auto entity = ECS::CreateEntity();
			auto PitAni = File::LoadJSON("Pit_Ani.json");
			ECS::AttachComponent<Sprite>(entity);
			ECS::AttachComponent<Transform>(entity);
			ECS::AttachComponent<AnimationController>(entity);
			std::string fileName = "spritesheets/PNG/Pitsheet.png";

			auto& animController = ECS::GetComponent<AnimationController>(entity);
			animController.InitUVs(fileName);
			animController.AddAnimation(PitAni["PitAni"]); // 8
			animController.SetActiveAnim(0);
			ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 128, 128, true, &animController);

			ECS::GetComponent<Sprite>(entity).SetTransparency(1.f);
			ECS::GetComponent<Transform>(entity).SetPosition(vec3(160.f, -194.f, 4.f));
		}





	}

	
	ECS::GetComponent<HorizontalScroll>(MainEntities::MainCamera()).SetFocus(&ECS::GetComponent<Transform>(MainEntities::MainPlayer()));
	ECS::GetComponent<VerticalScroll>(MainEntities::MainCamera()).SetFocus(&ECS::GetComponent<Transform>(MainEntities::MainPlayer()));
}

void ForestScene::Update()
{
	auto& animations = ECS::GetComponent<Player>(MainEntities::MainPlayer());
	
	animations.Update();

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
		player.GetBody()->ApplyForceToCenter(b2Vec2(-500000.f * speed, 0.f), true);
	}
	if (Input::GetKey(Key::D))
	{
		player.GetBody()->ApplyForceToCenter(b2Vec2(500000.f * speed, 0.f), true);
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
			
			player.GetBody()->ApplyLinearImpulseToCenter(b2Vec2(0.f, 250000.f), true);
			canJump.m_canJump = false;

		}
	}


	if (Input::GetKeyDown(Key::M)) {
		player.SetVelocity(vec3(0, 0, 0));
		player.SetPosition(b2Vec2(20, 20), true);
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

	if (Input::GetKeyDown(Key::E))
	{
		Scene::setSceneChange(1);

	}


}





void ForestScene::KeyboardUp()
{


}