#include "TopLevelScene.h"
#include "Utilities.h"
#include <random>
#include "Door.h"
#include "LadderTrigger.h"
#include "LadderTeleport.h"

TopLevelScene::TopLevelScene(std::string name)
	: Scene(name)
{
	m_gravity = b2Vec2(0.f, -98.f);
	m_physicsWorld->SetGravity(m_gravity);

	m_physicsWorld->SetContactListener(&listener);

}

void TopLevelScene::InitScene(float windowWidth, float windowHeight)
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
	

	



	//Link entity
	{
		/*Scene::CreatePhysicsSprite(m_sceneReg, "LinkStandby", 80, 60, 1.f, vec3(0.f, 30.f, 2.f), b2_dynamicBody, 0.f, 0.f, true, true)*/
		auto idle = File::LoadJSON("idle.json");

		auto entity = ECS::CreateEntity();
		ECS::SetIsMainPlayer(entity, true);

		//Add components
		ECS::AttachComponent<Sprite>(entity);
		ECS::AttachComponent<Transform>(entity);
		ECS::AttachComponent<PhysicsBody>(entity);
		ECS::AttachComponent<CanJump>(entity);
		ECS::AttachComponent<AnimationController>(entity);

		//Sets up the components
		std::string fileName = "spritesheets/PNG/PlayerIdle.png";

		ECS::GetComponent<Sprite>(entity).SetTransparency(1.f);
		ECS::GetComponent<Transform>(entity).SetPosition(vec3(0.f, 30.f, 8.f));

		auto& animController = ECS::GetComponent<AnimationController>(entity);
		animController.InitUVs(fileName);
		animController.AddAnimation(idle["idle"]);
		animController.SetActiveAnim(0);
		ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 20, 25, true, &animController);

		auto& tempSpr = ECS::GetComponent<Sprite>(entity);
		auto& tempPhsBody = ECS::GetComponent<PhysicsBody>(entity);

		float shrinkX = 0.f;
		float shrinkY = 0.f;

		b2Body* tempBody;
		b2BodyDef tempDef;
		tempDef.type = b2_dynamicBody;
		tempDef.position.Set(float32(-100.f), float32(15.f));

		tempBody = m_physicsWorld->CreateBody(&tempDef);

		tempPhsBody = PhysicsBody(entity, tempBody, float((tempSpr.GetHeight() - shrinkY) / 2.f), vec2(0.f, 0.f), false, PLAYER, ENVIRONMENT | ENEMY | OBJECTS | TRIGGER | HEXAGON, 1.f, 4.5f);

		tempPhsBody.SetRotationAngleDeg(0.f);
		tempPhsBody.SetFixedRotation(true);
		tempPhsBody.SetColor(vec4(1.f, 0.f, 1.f, 0.3f));
		tempPhsBody.SetGravityScale(1.f);
	}

	
		

		



			
	//SKY
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
				tempDef.position.Set(float32(-164.f), float32(-79.f));

				tempBody = m_physicsWorld->CreateBody(&tempDef);

				tempPhsBody = PhysicsBody(entity, tempBody, float(tempSpr.GetWidth() - shrinkX),
					float(tempSpr.GetHeight() - shrinkY), vec2(0.f, 0.f), false, WALL, PLAYER | ENEMY | OBJECTS | HEXAGON);
				tempPhsBody.SetColor(vec4(0.f, 1.f, 0.f, 0.3f));

				//Filling in the terrain without multiple scopes

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
				tempDef.position.Set(float32(-128.f), float32(-120.f));

				tempBody = m_physicsWorld->CreateBody(&tempDef);

				tempPhsBody = PhysicsBody(entity, tempBody, float(tempSpr.GetWidth() - shrinkX),
					float(tempSpr.GetHeight() - shrinkY), vec2(0.f, 0.f), false, GROUND, PLAYER | ENEMY | OBJECTS | HEXAGON);
				tempPhsBody.SetColor(vec4(0.f, 1.f, 0.f, 0.3f));

			}
			//FLOOR1
			for (int i = 0; i < 10; i++) {
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
					tempDef.position.Set(float32((i - 2) * 64.f), float32(-120.f));

					tempBody = m_physicsWorld->CreateBody(&tempDef);

					tempPhsBody = PhysicsBody(entity, tempBody, float(tempSpr.GetWidth() - shrinkX),
						float(tempSpr.GetHeight() - shrinkY), vec2(0.f, 0.f), false, GROUND, PLAYER | ENEMY | OBJECTS | HEXAGON);
					tempPhsBody.SetColor(vec4(0.f, 1.f, 0.f, 0.3f));

				}
				
			}
			

		
	









	ECS::GetComponent<HorizontalScroll>(MainEntities::MainCamera()).SetFocus(&ECS::GetComponent<Transform>(MainEntities::MainPlayer()));
	ECS::GetComponent<VerticalScroll>(MainEntities::MainCamera()).SetFocus(&ECS::GetComponent<Transform>(MainEntities::MainPlayer()));
}

void TopLevelScene::Update()
{








}

void TopLevelScene::KeyboardHold()
{
	auto& player = ECS::GetComponent<PhysicsBody>(MainEntities::MainPlayer());
	auto& ladder = ECS::GetComponent<CanJump>(MainEntities::MainPlayer());

	float speed = 3.f;
	b2Vec2 vel = b2Vec2(0.f, 0.f);


	if (Input::GetKey(Key::A))
	{
		player.GetBody()->ApplyForceToCenter(b2Vec2(-200000.f * speed, 0.f), true);
	}
	if (Input::GetKey(Key::D))
	{
		player.GetBody()->ApplyForceToCenter(b2Vec2(200000.f * speed, 0.f), true);
	}


	if (ladder.enableLadder) {
		if (Input::GetKey(Key::W)) {
			player.SetPosition(b2Vec2(player.GetPosition().x, player.GetPosition().y + 1));
		}
	}


}

void TopLevelScene::KeyboardDown()
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

void TopLevelScene::KeyboardUp()
{






}