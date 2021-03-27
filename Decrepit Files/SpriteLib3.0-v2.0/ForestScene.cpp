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

void ForestScene::spawnLeftWall() {
	{
		int wallMove = 0;

		for (int i = 0; i <= 2; i++)
		{
			{
				//Creates entity
				auto entity = ECS::CreateEntity();

				//Add components
				ECS::AttachComponent<Sprite>(entity);
				ECS::AttachComponent<Transform>(entity);
				ECS::AttachComponent<PhysicsBody>(entity);

				//Sets up components
				std::string fileName = "ForestTiles/Dirt/Wall_L.png";
				ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 128, 256);
				ECS::GetComponent<Transform>(entity).SetPosition(vec3(0.f, 0.f, 8.f));

				auto& tempSpr = ECS::GetComponent<Sprite>(entity);
				auto& tempPhsBody = ECS::GetComponent<PhysicsBody>(entity);

				float shrinkX = 0.f;
				float shrinkY = 0.f;
				b2Body* tempBody;
				b2BodyDef tempDef;
				tempDef.type = b2_staticBody;
				tempDef.position.Set(float32(-252.f), float32(256.f - wallMove));

				tempBody = m_physicsWorld->CreateBody(&tempDef);

				tempPhsBody = PhysicsBody(entity, tempBody, float(tempSpr.GetWidth() - shrinkX),
					float(tempSpr.GetHeight() - shrinkY), vec2(0.f, 0.f), false, OBJECTS, PLAYER | ENEMY | OBJECTS);
				tempPhsBody.SetColor(vec4(0.f, 1.f, 0.f, 0.3f));


			}



			{
				//Creates entity
				auto entity = ECS::CreateEntity();

				//Add components
				ECS::AttachComponent<Sprite>(entity);
				ECS::AttachComponent<Transform>(entity);
				ECS::AttachComponent<PhysicsBody>(entity);

				//Sets up components
				std::string fileName = "ForestTiles/Dirt/Wall_R.png";
				ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 128, 256);
				ECS::GetComponent<Transform>(entity).SetPosition(vec3(0.f, 0.f, 8.f));

				auto& tempSpr = ECS::GetComponent<Sprite>(entity);
				auto& tempPhsBody = ECS::GetComponent<PhysicsBody>(entity);

				float shrinkX = 0.f;
				float shrinkY = 0.f;
				b2Body* tempBody;
				b2BodyDef tempDef;
				tempDef.type = b2_staticBody;
				tempDef.position.Set(float32(-126.f), float32(256.f - wallMove));

				tempBody = m_physicsWorld->CreateBody(&tempDef);

				tempPhsBody = PhysicsBody(entity, tempBody, float(tempSpr.GetWidth() - shrinkX),
					float(tempSpr.GetHeight() - shrinkY), vec2(0.f, 0.f), false, OBJECTS, PLAYER | ENEMY | OBJECTS);
				tempPhsBody.SetColor(vec4(0.f, 1.f, 0.f, 0.3f));



			}

			wallMove = wallMove + 256;
		}
	}
}

void ForestScene::spawnGround() 
{
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
		ECS::GetComponent<Transform>(entity).SetPosition(vec3(0.f, 0.f, 3.f));

		auto& tempSpr = ECS::GetComponent<Sprite>(entity);
		auto& tempPhsBody = ECS::GetComponent<PhysicsBody>(entity);

		float shrinkX = 0.f;
		float shrinkY = 0.f;
		b2Body* tempBody;
		b2BodyDef tempDef;
		tempDef.type = b2_staticBody;
		tempDef.position.Set(float32(720.f), float32(-360.f));

		tempBody = m_physicsWorld->CreateBody(&tempDef);

		tempPhsBody = PhysicsBody(entity, tempBody, float(tempSpr.GetWidth() - shrinkX),
			float(tempSpr.GetHeight() - shrinkY), vec2(0.f, 0.f), false, OBJECTS, PLAYER | ENEMY | OBJECTS);
		tempPhsBody.SetColor(vec4(0.f, 1.f, 0.f, 0.3f));

	}
}


void ForestScene::spawnGenericGround(int xGround, int yGround, int xGrass, int yGrass, int xGroundBox, int yGroundBox) {
	int xposGround = xGround;
	int yposGround = yGround;
	int xposGrass = xGrass;
	int yposGrass = yGrass;
	int xposGroundBox = xGroundBox;
	int yposGroundBox = yGroundBox;


	//Setup First Platform
	{
		int fillDirt = 0;
		int fillGrass = 0;


		for (int i = 0; i <= 2; i++) {

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
				ECS::GetComponent<Transform>(entity).SetPosition(vec3(xposGrass + fillGrass, yposGrass, 2.f));

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
				ECS::GetComponent<Transform>(entity).SetPosition(vec3(xposGrass + fillGrass, yposGrass - 4.f, 5.f));

			}

			fillGrass = fillGrass + 64;
		}


		for (int i = 0; i <= 3; i++) {

			


			{
				//Creates entity
				auto entity = ECS::CreateEntity();

				//Add components
				ECS::AttachComponent<Sprite>(entity);
				ECS::AttachComponent<Transform>(entity);
				ECS::AttachComponent<PhysicsBody>(entity);

				//Sets up components
				std::string fileName = "ForestTiles/Dirt/TopL&R&C.png";
				ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 192, 64);
				ECS::GetComponent<Transform>(entity).SetPosition(vec3(0.f, 0.f, 3.f));

				auto& tempSpr = ECS::GetComponent<Sprite>(entity);
				auto& tempPhsBody = ECS::GetComponent<PhysicsBody>(entity);

				float shrinkX = 0.f;
				float shrinkY = 0.f;
				b2Body* tempBody;
				b2BodyDef tempDef;
				tempDef.type = b2_staticBody;
				tempDef.position.Set(float32(xposGround), float32(yposGround - fillDirt));

				tempBody = m_physicsWorld->CreateBody(&tempDef);

				tempPhsBody = PhysicsBody(entity, tempBody, float(tempSpr.GetWidth() - shrinkX),
					float(tempSpr.GetHeight() - shrinkY), vec2(0.f, 0.f), false, WALL, PLAYER | ENEMY | OBJECTS);
				tempPhsBody.SetColor(vec4(0.f, 1.f, 0.f, 0.3f));

				//Filling in the terrain without multiple scopes

			}

			fillDirt = fillDirt + 64;
			
		}

		float physBodyX = 192.f;
		float physBodyY = 24.f;

		{
			//Creates entity
			auto entity = ECS::CreateEntity();

			//Add components
			ECS::AttachComponent<Transform>(entity);
			ECS::AttachComponent<PhysicsBody>(entity);

			//Sets up components
			ECS::GetComponent<Transform>(entity).SetPosition(vec3(0.f, 0.f, 4.f));
			auto& tempPhsBody = ECS::GetComponent<PhysicsBody>(entity);

			float shrinkX = 0.f;
			float shrinkY = 0.f;
			b2Body* tempBody;
			b2BodyDef tempDef;
			tempDef.type = b2_staticBody;
			tempDef.position.Set(float32(xposGroundBox), float32(yposGroundBox));

			tempBody = m_physicsWorld->CreateBody(&tempDef);

			tempPhsBody = PhysicsBody(entity, tempBody, float(physBodyX),
				float(physBodyY / 2), vec2(0.f, 16.f), false, GROUND, PLAYER | ENEMY | OBJECTS);
			tempPhsBody.SetColor(vec4(1.f, 0.f, 0.f, 0.3f));

		}

	}

}

void ForestScene::twoWideGround() {

}

void ForestScene::spawnConnectorGround() {

}

void ForestScene::floatingGround() {

}



void ForestScene::InitScene(float windowWidth, float windowHeight)
{
	//Dynamically allocates the register
	m_sceneReg = new entt::registry;

	//Attach the register
	ECS::AttachRegister(m_sceneReg);

	//Sets up aspect ratio for the camera
	float aspectRatio = windowWidth / windowHeight;

	{
		ForestScene::spawnLeftWall();

		//Underground 
		ForestScene::spawnGround();

		//Platform One
		ForestScene::spawnGenericGround(0.f, -64.f, -64.f, -20.f, 0.f, -52.f);

		//Platform Two
		ForestScene::spawnGenericGround(320.f, -64.f, 256.f, -20.f, 320.f, -52.f);

		//Platform after bridge
		ForestScene::spawnGenericGround(768.f, -64.f, 704.f, -20.f, 768.f, -52.f);
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
		ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 3400,2000);
		ECS::GetComponent<Sprite>(entity).SetTransparency(1.f);
		ECS::GetComponent<Transform>(entity).SetPosition(vec3(0.f, 100.f, 1.f));
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
			ECS::GetComponent<Transform>(entity).SetPosition(vec3(-96.f, -100.f, 20.f));

		}

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

	


	


	
	
	//Switch One
	{
		{
			//Creates entity
			auto entity = ECS::CreateEntity();
			ECS::SetIsSwitchOne(entity, true);
			switchOne = entity;
			

			//Add components
			ECS::AttachComponent<Sprite>(entity);
			ECS::AttachComponent<Transform>(entity);
			ECS::AttachComponent<PhysicsBody>(entity);
			ECS::AttachComponent<Trigger*>(entity);

			//Sets up components
			std::string fileName = "ForestTiles/Interactables/Switch_Off.png";
			ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 64, 64);
			ECS::GetComponent<Transform>(entity).SetPosition(vec3(0.f, 0.f, 3.f));
			ECS::GetComponent<Trigger*>(entity) = new DestroyTrigger();
			ECS::GetComponent<Trigger*>(entity)->SetTriggerEntity(switchOne);
			ECS::GetComponent<Trigger*>(entity)->AddTargetEntity(bridge);
			

			auto& tempSpr = ECS::GetComponent<Sprite>(entity);
			auto& tempPhsBody = ECS::GetComponent<PhysicsBody>(entity);


			float shrinkX = 32.f;
			float shrinkY = 0.f;
			b2Body* tempBody;
			b2BodyDef tempDef;
			tempDef.type = b2_staticBody;
			tempDef.position.Set(float32(400.f), float32(0.f));

			tempBody = m_physicsWorld->CreateBody(&tempDef);

			tempPhsBody = PhysicsBody(entity, tempBody, float(tempSpr.GetWidth() - shrinkX), float(tempSpr.GetHeight() - shrinkY), vec2(0.f, 0.f), true, TRIGGER, PLAYER | OBJECTS);
			tempPhsBody.SetColor(vec4(0.f, 1.f, 0.f, 0.3f));

			

		}

		//{
		//	//Creates entity
		//	auto entity = ECS::CreateEntity();
		//	ECS::SetIsBridgeOne(entity, true);
		//	bridge = entity;


		//	//Add components
		//	ECS::AttachComponent<Sprite>(entity);
		//	ECS::AttachComponent<Transform>(entity);
		//	ECS::AttachComponent<PhysicsBody>(entity);
		//	ECS::AttachComponent<Bridge>(entity);

		//	//Sets up components
		//	std::string fileName = "ForestTiles/Interactables/Bridge_Up.png";
		//	ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 64, 360);
		//	ECS::GetComponent<Transform>(entity).SetPosition(vec3(30.f, -20.f, 2.f));


		//	auto& tempSpr = ECS::GetComponent<Sprite>(entity);
		//	auto& tempPhsBody = ECS::GetComponent<PhysicsBody>(entity);


		//	float shrinkX = 0.f;
		//	float shrinkY = 0.f;
		//	b2Body* tempBody;
		//	b2BodyDef tempDef;
		//	tempDef.type = b2_staticBody;
		//	tempDef.position.Set(float32(490.f), float32(80.f));

		//	tempBody = m_physicsWorld->CreateBody(&tempDef);

		//	tempPhsBody = PhysicsBody(entity, tempBody, float(tempSpr.GetWidth() - shrinkX), float(tempSpr.GetHeight() - shrinkY), vec2(0.f, 0.f), false, OBJECTS, PLAYER);
		//	tempPhsBody.SetColor(vec4(1.f, 2.f, 4.f, 0.3f));


		//}

	}


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