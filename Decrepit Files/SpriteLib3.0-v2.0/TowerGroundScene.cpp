#pragma comment(lib, "winmm.lib")
#include "TowerGroundScene.h"
#include "Platform.h"
#include "Utilities.h"
#include <random>
#include "Door.h"
#include "Scroll.h"
#include "LadderTrigger.h"
#include "LadderTeleport.h"

TowerGroundScene::TowerGroundScene(std::string name)
	: Scene(name)
{
	m_gravity = b2Vec2(0.f, -98.f);
	m_physicsWorld->SetGravity(m_gravity);

	m_physicsWorld->SetContactListener(&listener);

}

int TowerGroundScene::getSceneChange() {
	return m_sceneChange;
}

void TowerGroundScene::setSceneChange(int sc) {
	m_sceneChange = sc;
}


void TowerGroundScene::InitScene(float windowWidth, float windowHeight)
{
	//Dynamically allocates the register
	m_sceneReg = new entt::registry;
	m_physicsWorld = new b2World(m_gravity);

	m_gravity = b2Vec2(0.f, -98.f);
	m_physicsWorld->SetGravity(m_gravity);

	m_physicsWorld->SetContactListener(&listener);

	//Attach the register
	ECS::AttachRegister(m_sceneReg);

	//Sets up aspect ratio for the camera
	float aspectRatio = windowWidth / windowHeight;
	
	EffectManager::CreateEffect(EffectType::Vignette, windowWidth, windowHeight);
	VignetteEffect* vig = (VignetteEffect*)EffectManager::GetEffect(EffectManager::GetVignetteHandle());
	vig->SetInnerRadius(0.f);
	
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
	//Link entity
	{
		/*Scene::CreatePhysicsSprite(m_sceneReg, "LinkStandby", 80, 60, 1.f, vec3(0.f, 30.f, 2.f), b2_dynamicBody, 0.f, 0.f, true, true)*/



		auto entity = ECS::CreateEntity();
		ECS::SetIsMainPlayer(entity, true);
		

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
		ECS::GetComponent<Transform>(entity).SetPosition(vec3(0.f, 30.f, 8.f));

		ECS::GetComponent<CanJump>(entity).inForest = false;

		auto& tempSpr = ECS::GetComponent<Sprite>(entity);
		auto& tempPhsBody = ECS::GetComponent<PhysicsBody>(entity);

		float shrinkX = 0.f;
		float shrinkY = 0.f;

		b2Body* tempBody;
		b2BodyDef tempDef;
		tempDef.type = b2_dynamicBody;
		tempDef.position.Set(float32(-20.f), float32(-50.f));

		tempBody = m_physicsWorld->CreateBody(&tempDef);

		tempPhsBody = PhysicsBody(entity, tempBody, float((tempSpr.GetHeight() - shrinkY) / 2.f), vec2(0.f, 0.f), false, PLAYER, ENVIRONMENT | ENEMY | OBJECTS | TRIGGER | HEXAGON, 1.f, 4.5f);





		tempPhsBody.SetRotationAngleDeg(0.f);
		tempPhsBody.SetFixedRotation(true);
		tempPhsBody.SetColor(vec4(1.f, 0.f, 1.f, 0.3f));
		tempPhsBody.SetGravityScale(1.f);
	}
	
	//START END SCREEN
	{
	float moveX = -3000.f;
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
			ECS::GetComponent<Transform>(entity).SetPosition(vec3(0.f + moveX, 0.f, 8.f));
		}
		{
			auto entity = ECS::CreateEntity();
			Scroll::setScrollEntity(6, entity);
			//ECS::AttachComponent<Sprite>(entity);
			ECS::AttachComponent<Transform>(entity);
			//std::string fileName = "Tower Inside/Base/Menu/Title_Image.png";
			//ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 290, 185);
			//ECS::GetComponent<Sprite>(entity).SetTransparency(1.f);
			ECS::GetComponent<Transform>(entity).SetPosition(vec3(0.f + moveX, 0.f, -1.f));
		}
		{
			/*Scene::CreateSprite(m_sceneReg, "HelloWorld.png", 100, 60, 0.5f, vec3(0.f, 0.f, 0.f));*/

			//Creates entity
			auto entity = ECS::CreateEntity();
			
			//Add components
			ECS::AttachComponent<Sprite>(entity);
			ECS::AttachComponent<Transform>(entity);

			//Set up the components
			std::string fileName = "Tower Inside V2/You_Win.png";
			ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 192, 64);
			ECS::GetComponent<Sprite>(entity).SetTransparency(1.f);
			ECS::GetComponent<Transform>(entity).SetPosition(vec3(0.f + moveX, 0.f, 9.f));
		}

	}
	//END END SCREEN
	//START TOP LEVEL
	{
		float moveX = 1000.f;
		float moveY = 1000.f;

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
			ECS::GetComponent<Transform>(entity).SetPosition(vec3(0.f + moveX, 100.f + moveY, 0.f));
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
			ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 2560, 720);
			ECS::GetComponent<Sprite>(entity).SetTransparency(1.f);
			ECS::GetComponent<Transform>(entity).SetPosition(vec3(0.f + moveX, 100.f + moveY, 0.f));
		}
		{
			/*Scene::CreateSprite(m_sceneReg, "HelloWorld.png", 100, 60, 0.5f, vec3(0.f, 0.f, 0.f));*/

			//Creates entity
			auto entity = ECS::CreateEntity();
			auto scroll = File::LoadJSON("Scroll.json");
			std::string fileName = "spritesheets/PNG/scroll.png";
			//Add components
			ECS::AttachComponent<Scroll>(entity);
			ECS::AttachComponent<Sprite>(entity);
			ECS::AttachComponent<Transform>(entity);
			ECS::AttachComponent<PhysicsBody>(entity);
			ECS::AttachComponent<AnimationController>(entity);
			ECS::GetComponent<Sprite>(entity).SetTransparency(1.f);
			ECS::GetComponent<Transform>(entity).SetPosition(vec3(-110, 105, 4.f));
			Scroll::setScrollEntity(4, entity);
			auto& animController = ECS::GetComponent<AnimationController>(entity);
			animController.InitUVs(fileName);
			animController.AddAnimation(scroll["ScrollAnimation"]);
			animController.SetActiveAnim(0);
			ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 32, 32, true, &animController);
			//Set up the components
			auto& tempSpr = ECS::GetComponent<Sprite>(entity);
			auto& tempPhsBody = ECS::GetComponent<PhysicsBody>(entity);

			float shrinkX = 20.f;
			float shrinkY = 20.f;
			b2Body* tempBody;
			b2BodyDef tempDef;
			tempDef.type = b2_staticBody;
			tempDef.position.Set(float32(100 + moveX), float32(-45 + moveY));

			tempBody = m_physicsWorld->CreateBody(&tempDef);

			tempPhsBody = PhysicsBody(entity, tempBody, float(tempSpr.GetWidth() - shrinkX),
				float(tempSpr.GetHeight() - shrinkY), vec2(0.f, 0.f), false, SCROLL4, PLAYER | ENEMY | OBJECTS | HEXAGON);
			tempPhsBody.SetColor(vec4(0.f, 0.f, 1.f, 0.3f));

		} 
		{
			/*Scene::CreateSprite(m_sceneReg, "HelloWorld.png", 100, 60, 0.5f, vec3(0.f, 0.f, 0.f));*/

			//Creates entity
			auto entity = ECS::CreateEntity();
			auto scroll = File::LoadJSON("Idol.json");
			std::string fileName = "spritesheets/PNG/alter.png";
			//Add components
			ECS::AttachComponent<Scroll>(entity);
			ECS::AttachComponent<Sprite>(entity);
			ECS::AttachComponent<Transform>(entity);
			ECS::AttachComponent<PhysicsBody>(entity);
			ECS::AttachComponent<AnimationController>(entity);
			ECS::GetComponent<Sprite>(entity).SetTransparency(1.f);
			ECS::GetComponent<Transform>(entity).SetPosition(vec3(-110, 105, 4.f));
			Scroll::setScrollEntity(5, entity);
			auto& animController = ECS::GetComponent<AnimationController>(entity);
			animController.InitUVs(fileName);
			animController.AddAnimation(scroll["Idol"]);
			animController.SetActiveAnim(0);
			ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 64, 64, true, &animController);
			//Set up the components
			auto& tempSpr = ECS::GetComponent<Sprite>(entity);
			auto& tempPhsBody = ECS::GetComponent<PhysicsBody>(entity);

			float shrinkX = 20.f;
			float shrinkY = 20.f;
			b2Body* tempBody;
			b2BodyDef tempDef;
			tempDef.type = b2_staticBody;
			tempDef.position.Set(float32(350 + moveX), float32(-50 + moveY));

			tempBody = m_physicsWorld->CreateBody(&tempDef);

			tempPhsBody = PhysicsBody(entity, tempBody, float(tempSpr.GetWidth() - shrinkX),
				float(tempSpr.GetHeight() - shrinkY), vec2(0.f, 20.f), false, SCROLL5, PLAYER | ENEMY | OBJECTS | HEXAGON);
			tempPhsBody.SetColor(vec4(0.f, 0.f, 1.f, 0.3f));

		}
		{
			/*Scene::CreateSprite(m_sceneReg, "HelloWorld.png", 100, 60, 0.5f, vec3(0.f, 0.f, 0.f));*/

			//Creates entity
			auto entity = ECS::CreateEntity();
			ECS::AttachComponent<Scroll>(entity);
			ECS::AttachComponent<Transform>(entity);
			ECS::AttachComponent<PhysicsBody>(entity);
			ECS::GetComponent<Transform>(entity).SetPosition(vec3(-110, 105, 4.f));
			Scroll::setScrollEntity(6, entity);
			auto& tempPhsBody = ECS::GetComponent<PhysicsBody>(entity);
			float shrinkX = 0.f;
			float shrinkY = 0.f;
			b2Body* tempBody;
			b2BodyDef tempDef;
			tempDef.type = b2_staticBody;
			tempDef.position.Set(float32(500 + moveX), float32(-40 + moveY));

			tempBody = m_physicsWorld->CreateBody(&tempDef);

			tempPhsBody = PhysicsBody(entity, tempBody, float(32 - shrinkX),
				float(100 - shrinkY), vec2(0.f, 0.f), false, DOORFIN, PLAYER | ENEMY | OBJECTS | HEXAGON);
			tempPhsBody.SetColor(vec4(0.f, 0.f, 1.f, 0.3f));

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
			tempDef.position.Set(float32(-164.f + moveX), float32(0.f + moveY));

			tempBody = m_physicsWorld->CreateBody(&tempDef);

			tempPhsBody = PhysicsBody(entity, tempBody, float(tempSpr.GetWidth() - shrinkX),
				float(tempSpr.GetHeight() - shrinkY), vec2(0.f, 0.f), false, WALL, PLAYER | ENEMY | OBJECTS | HEXAGON);
			tempPhsBody.SetColor(vec4(0.f, 1.f, 0.f, 0.3f));

			//Filling in the terrain without multiple scopes

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
			tempDef.position.Set(float32(-164.f + moveX), float32(-192.f + moveY));

			tempBody = m_physicsWorld->CreateBody(&tempDef);

			tempPhsBody = PhysicsBody(entity, tempBody, float(tempSpr.GetWidth() - shrinkX),
				float(tempSpr.GetHeight() - shrinkY), vec2(0.f, 0.f), false, WALL, PLAYER | ENEMY | OBJECTS | HEXAGON);
			tempPhsBody.SetColor(vec4(0.f, 1.f, 0.f, 0.3f));

			//Filling in the terrain without multiple scopes

		}
		
		//FLOOR1
		
			{
				//Creates entity
				auto entity = ECS::CreateEntity();

				//Add components
				ECS::AttachComponent<Sprite>(entity);
				ECS::AttachComponent<Transform>(entity);
				ECS::AttachComponent<PhysicsBody>(entity);

				//Sets up components
				std::string fileName = "WallTiles/floor.png";
				ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 708, 64);
				ECS::GetComponent<Transform>(entity).SetPosition(vec3(30.f, -20.f, 3.f));

				auto& tempSpr = ECS::GetComponent<Sprite>(entity);
				auto& tempPhsBody = ECS::GetComponent<PhysicsBody>(entity);

				float shrinkX = 0.f;
				float shrinkY = 0.f;
				b2Body* tempBody;
				b2BodyDef tempDef;
				tempDef.type = b2_staticBody;
				tempDef.position.Set(float32(300.f + moveX), float32(-120.f + moveY));

				tempBody = m_physicsWorld->CreateBody(&tempDef);

				tempPhsBody = PhysicsBody(entity, tempBody, float(tempSpr.GetWidth() - shrinkX),
					float(tempSpr.GetHeight() - shrinkY), vec2(0.f, 0.f), false, GROUND, PLAYER | ENEMY | OBJECTS | HEXAGON);
				tempPhsBody.SetColor(vec4(0.f, 1.f, 0.f, 0.3f));

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
				ECS::GetComponent<Transform>(entity).SetPosition(vec3(30.f, -20.f, 2.f));
				ECS::GetComponent<Trigger*>(entity) = new LadderTrigger();
				ECS::GetComponent<Trigger*>(entity)->SetTriggerEntity(entity);

				auto& tempSpr = ECS::GetComponent<Sprite>(entity);
				auto& tempPhsBody = ECS::GetComponent<PhysicsBody>(entity);

				float shrinkX = 40.f;
				float shrinkY = 20.f;
				b2Body* tempBody;
				b2BodyDef tempDef;
				tempDef.type = b2_staticBody;
				tempDef.position.Set(float32(-90.f + moveX), float32(-180.f + moveY));

				tempBody = m_physicsWorld->CreateBody(&tempDef);

				tempPhsBody = PhysicsBody(entity, tempBody, float(tempSpr.GetWidth() - shrinkX),
					float(tempSpr.GetHeight() - shrinkY), vec2(0.f, 0.f), true, TRIGGER, PLAYER | OBJECTS);
				tempPhsBody.SetColor(vec4(0.f, 1.f, 0.f, 0.3f));

			}
			{

				//Creates entity
				auto entity = ECS::CreateEntity();

				//Add components
				ECS::AttachComponent<Transform>(entity);
				ECS::AttachComponent<PhysicsBody>(entity);


				//Sets up components
				ECS::GetComponent<Transform>(entity).SetPosition(vec3(30.f, -20.f, 2.f));

				auto& tempPhsBody = ECS::GetComponent<PhysicsBody>(entity);

				b2Body* tempBody;
				b2BodyDef tempDef;
				tempDef.type = b2_staticBody;
				tempDef.position.Set(float32(-96.f + moveX), float32(-150.f + moveY));

				tempBody = m_physicsWorld->CreateBody(&tempDef);

				tempPhsBody = PhysicsBody(entity, tempBody, float(100), float(1), vec2(0.f, 0.f), false, LADDER4, PLAYER | OBJECTS);
				tempPhsBody.SetColor(vec4(1.f, 0.f, 1.f, 0.3f));

			}

		
	}
	//END TOP LEVEL

	//START BASEMENT
	{
	float moveX = 1500.f;
		//BG
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
			ECS::GetComponent<Transform>(entity).SetPosition(vec3(0.f + moveX, 50.f, 1.f));
		}

		{
			

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
						tempDef.position.Set(float32(-164.f + moveX), float32(-15.f + (increment * i)));

						tempBody = m_physicsWorld->CreateBody(&tempDef);

						tempPhsBody = PhysicsBody(entity, tempBody, float(tempSpr.GetWidth() - shrinkX),
							float(tempSpr.GetHeight() - shrinkY), vec2(0.f, 0.f), false, WALL, PLAYER | ENEMY | OBJECTS | HEXAGON);
						tempPhsBody.SetColor(vec4(0.f, 1.f, 0.f, 0.3f));

						//Filling in the terrain without multiple scopes

					}
				}
				{
					float physBodyX = 140.f;
					float physBodyY = 64.f;
					float posX = -45.f + moveX;
					float posY = -42.f;
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
							float(tempSpr.GetHeight() - shrinkY), vec2(0.f, 12.f), false, WALL, PLAYER | ENEMY | OBJECTS | HEXAGON);
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
							float(physBodyY / 2), vec2(0.f, 16.f), false, GROUND, PLAYER | ENEMY | OBJECTS | HEXAGON);
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
				//SPIKE ON PLAT 2
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
					tempDef.position.Set(float32(190 + moveX), float32(8.f));

					tempBody = m_physicsWorld->CreateBody(&tempDef);

					tempPhsBody = PhysicsBody(entity, tempBody, float(tempSpr.GetWidth() - shrinkX),
						float(tempSpr.GetHeight() - shrinkY), vec2(0.f, 0.f), false, ENEMY, PLAYER | ENEMY | OBJECTS | HEXAGON);
					tempPhsBody.SetColor(vec4(0.f, 1.f, 0.f, 0.3f));

				}
				//SMALL PLATFORM 2
				{
					float physBodyX = 140.f;
					float physBodyY = 64.f;
					float posX = 180.f + moveX;
					float posY = -20.f;
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
					//SCROLL
					{
						/*Scene::CreateSprite(m_sceneReg, "HelloWorld.png", 100, 60, 0.5f, vec3(0.f, 0.f, 0.f));*/

						//Creates entity
						auto entity = ECS::CreateEntity();
						auto scroll = File::LoadJSON("Scroll.json");
						std::string fileName = "spritesheets/PNG/scroll.png";
						//Add components
						ECS::AttachComponent<Scroll>(entity);
						ECS::AttachComponent<Sprite>(entity);
						ECS::AttachComponent<Transform>(entity);
						ECS::AttachComponent<PhysicsBody>(entity);
						ECS::AttachComponent<AnimationController>(entity);
						ECS::GetComponent<Sprite>(entity).SetTransparency(1.f);
						ECS::GetComponent<Transform>(entity).SetPosition(vec3(-110, 105, 4.f));
						Scroll::setScrollEntity(3, entity);
						auto& animController = ECS::GetComponent<AnimationController>(entity);
						animController.InitUVs(fileName);
						animController.AddAnimation(scroll["ScrollAnimation"]);
						animController.SetActiveAnim(0);
						ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 32, 32, true, &animController);
						//Set up the components
						auto& tempSpr = ECS::GetComponent<Sprite>(entity);
						auto& tempPhsBody = ECS::GetComponent<PhysicsBody>(entity);

						float shrinkX = 20.f;
						float shrinkY = 20.f;
						b2Body* tempBody;
						b2BodyDef tempDef;
						tempDef.type = b2_staticBody;
						tempDef.position.Set(float32(180 + moveX), float32(40));

						tempBody = m_physicsWorld->CreateBody(&tempDef);

						tempPhsBody = PhysicsBody(entity, tempBody, float(tempSpr.GetWidth() - shrinkX),
							float(tempSpr.GetHeight() - shrinkY), vec2(0.f, 0.f), false, SCROLL3, PLAYER | ENEMY | OBJECTS | HEXAGON);
						tempPhsBody.SetColor(vec4(0.f, 0.f, 1.f, 0.3f));

					}
					//SMALL PLATFORM 3
					{
						float physBodyX = 140.f;
						float physBodyY = 64.f;
						float posX = 420.f + moveX;
						float posY = -46.f;
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
							ECS::GetComponent<Transform>(entity).SetPosition(vec3(30.f, -20.f, 2.f));
							ECS::GetComponent<Trigger*>(entity) = new LadderTrigger();
							ECS::GetComponent<Trigger*>(entity)->SetTriggerEntity(entity);

							auto& tempSpr = ECS::GetComponent<Sprite>(entity);
							auto& tempPhsBody = ECS::GetComponent<PhysicsBody>(entity);

							float shrinkX = 40.f;
							float shrinkY = 20.f;
							b2Body* tempBody;
							b2BodyDef tempDef;
							tempDef.type = b2_staticBody;
							tempDef.position.Set(float32(-46.f + moveX), float32(70.f));

							tempBody = m_physicsWorld->CreateBody(&tempDef);

							tempPhsBody = PhysicsBody(entity, tempBody, float(tempSpr.GetWidth() - shrinkX),
								float(tempSpr.GetHeight() - shrinkY), vec2(0.f, 0.f), true, TRIGGER, PLAYER | OBJECTS);
							tempPhsBody.SetColor(vec4(0.f, 1.f, 0.f, 0.3f));

						}
						{

							//Creates entity
							auto entity = ECS::CreateEntity();

							//Add components
							ECS::AttachComponent<Transform>(entity);
							ECS::AttachComponent<PhysicsBody>(entity);


							//Sets up components
							ECS::GetComponent<Transform>(entity).SetPosition(vec3(30.f, -20.f, 2.f));

							auto& tempPhsBody = ECS::GetComponent<PhysicsBody>(entity);

							b2Body* tempBody;
							b2BodyDef tempDef;
							tempDef.type = b2_staticBody;
							tempDef.position.Set(float32(-46.f + moveX), float32(165.f));

							tempBody = m_physicsWorld->CreateBody(&tempDef);

							tempPhsBody = PhysicsBody(entity, tempBody, float(0), float(0), vec2(0.f, 0.f), false, LADDER5, PLAYER | OBJECTS);
							tempPhsBody.SetColor(vec4(1.f, 0.f, 1.f, 0.3f));

						}
						//RIGHT WALL
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
							tempDef.position.Set(float32(544.f + moveX), float32(-15.f + (increment * i)));

							tempBody = m_physicsWorld->CreateBody(&tempDef);

							tempPhsBody = PhysicsBody(entity, tempBody, float(tempSpr.GetWidth() - shrinkX),
								float(tempSpr.GetHeight() - shrinkY), vec2(0.f, 0.f), false, WALL, PLAYER | ENEMY | OBJECTS | HEXAGON);
							tempPhsBody.SetColor(vec4(0.f, 1.f, 0.f, 0.3f));

							//Filling in the terrain without multiple scopes

						}
					}

					//ROOF
					{

						//Creates entity
						auto entity = ECS::CreateEntity();

						//Add components
						ECS::AttachComponent<Sprite>(entity);
						ECS::AttachComponent<Transform>(entity);
						ECS::AttachComponent<PhysicsBody>(entity);

						//Sets up components
						std::string fileName = "Tower Inside/Base/Filling/roofLong.png";
						ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 768, 64);
						ECS::GetComponent<Transform>(entity).SetPosition(vec3(30.f, -20.f, 3.f));

						auto& tempSpr = ECS::GetComponent<Sprite>(entity);
						auto& tempPhsBody = ECS::GetComponent<PhysicsBody>(entity);

						float shrinkX = 0.f;
						float shrinkY = 0.f;
						b2Body* tempBody;
						b2BodyDef tempDef;
						tempDef.type = b2_staticBody;
						tempDef.position.Set(float32(244 + moveX), float32(200.f));

						tempBody = m_physicsWorld->CreateBody(&tempDef);

						tempPhsBody = PhysicsBody(entity, tempBody, float(tempSpr.GetWidth() - shrinkX),
							float(tempSpr.GetHeight() - shrinkY), vec2(0.f, 0.f), false, OBJECTS, PLAYER | ENEMY | OBJECTS | HEXAGON);
						tempPhsBody.SetColor(vec4(0.f, 1.f, 0.f, 0.3f));





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
					tempDef.position.Set(float32(-128.f + moveX), float32(-120.f));

					tempBody = m_physicsWorld->CreateBody(&tempDef);

					tempPhsBody = PhysicsBody(entity, tempBody, float(tempSpr.GetWidth() - shrinkX),
						float(tempSpr.GetHeight() - shrinkY), vec2(0.f, 0.f), false, GROUND, PLAYER | ENEMY | OBJECTS | HEXAGON);
					tempPhsBody.SetColor(vec4(0.f, 1.f, 0.f, 0.3f));

				}
				//FLOOR1
				
					{
						//Creates entity
						auto entity = ECS::CreateEntity();

						//Add components
						ECS::AttachComponent<Sprite>(entity);
						ECS::AttachComponent<Transform>(entity);
						ECS::AttachComponent<PhysicsBody>(entity);

						//Sets up components
						std::string fileName = "WallTiles/floor.png";
						ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 704, 64);
						ECS::GetComponent<Transform>(entity).SetPosition(vec3(30.f, -20.f, 3.f));

						auto& tempSpr = ECS::GetComponent<Sprite>(entity);
						auto& tempPhsBody = ECS::GetComponent<PhysicsBody>(entity);

						float shrinkX = 0.f;
						float shrinkY = 0.f;
						b2Body* tempBody;
						b2BodyDef tempDef;
						tempDef.type = b2_staticBody;
						tempDef.position.Set(float32(186 + moveX), float32(-120.f));

						tempBody = m_physicsWorld->CreateBody(&tempDef);

						tempPhsBody = PhysicsBody(entity, tempBody, float(tempSpr.GetWidth() - shrinkX),
							float(tempSpr.GetHeight() - shrinkY), vec2(0.f, 0.f), false, GROUND, PLAYER | ENEMY | OBJECTS | HEXAGON);
						tempPhsBody.SetColor(vec4(0.f, 1.f, 0.f, 0.3f));

					}
					
				
				//SPIKE ON Ground

				

					//Creates entity
					auto entity = ECS::CreateEntity();

					//Add components
					ECS::AttachComponent<Sprite>(entity);
					ECS::AttachComponent<Transform>(entity);
					ECS::AttachComponent<PhysicsBody>(entity);

					//Sets up components
					std::string fileName = "Tower Inside/spikeLine.png";
					ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 576, 32);
					ECS::GetComponent<Transform>(entity).SetPosition(vec3(30.f, -20.f, 4.f));

					auto& tempSpr = ECS::GetComponent<Sprite>(entity);
					auto& tempPhsBody = ECS::GetComponent<PhysicsBody>(entity);

					float shrinkX = 0.f;
					float shrinkY = 0.f;
					b2Body* tempBody;
					b2BodyDef tempDef;
					tempDef.type = b2_staticBody;
					tempDef.position.Set(float32(196 + moveX), float32(-95.f));

					tempBody = m_physicsWorld->CreateBody(&tempDef);

					tempPhsBody = PhysicsBody(entity, tempBody, float(tempSpr.GetWidth() - shrinkX),
					float(tempSpr.GetHeight() - shrinkY), vec2(0.f, 0.f), false, ENEMY, PLAYER | ENEMY | OBJECTS | HEXAGON);
					tempPhsBody.SetColor(vec4(0.f, 1.f, 0.f, 0.3f));

					

				

			}
			//KEY
			{

				//Creates entity
				auto entity = ECS::CreateEntity();
				ECS::SetIsKeyTwo(entity, true);

				//Add components
				ECS::AttachComponent<Sprite>(entity);
				ECS::AttachComponent<Transform>(entity);
				ECS::AttachComponent<PhysicsBody>(entity);

				//Sets up components
				std::string fileName = "Tower Inside V2/Key2.png";
				ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 32, 32);
				ECS::GetComponent<Transform>(entity).SetPosition(vec3(30.f, -20.f, 4.f));

				auto& tempSpr = ECS::GetComponent<Sprite>(entity);
				auto& tempPhsBody = ECS::GetComponent<PhysicsBody>(entity);

				float shrinkX = 20.f;
				float shrinkY = 20.f;
				b2Body* tempBody;
				b2BodyDef tempDef;
				tempDef.type = b2_staticBody;
				tempDef.position.Set(float32(410.f + moveX), float32(0.f));

				tempBody = m_physicsWorld->CreateBody(&tempDef);

				tempPhsBody = PhysicsBody(entity, tempBody, float(tempSpr.GetWidth() - shrinkX),
					float(tempSpr.GetHeight() - shrinkY), vec2(0.f, 0.f), false, KEY2, PLAYER | ENEMY | OBJECTS | HEXAGON);
				tempPhsBody.SetColor(vec4(0.f, 0.f, 1.f, 0.3f));

			}
		}
	}
		//END BASEMENT
	//START MID LEVEL
	{
	float moveX = 2500.f;
	//BG
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
		ECS::GetComponent<Transform>(entity).SetPosition(vec3(0.f + moveX, 50.f, 1.f));
	}

	{
		
		

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
					tempDef.position.Set(float32(-164.f + moveX), float32(-207.f + (increment * i)));

					tempBody = m_physicsWorld->CreateBody(&tempDef);

					tempPhsBody = PhysicsBody(entity, tempBody, float(tempSpr.GetWidth() - shrinkX),
						float(tempSpr.GetHeight() - shrinkY), vec2(0.f, 0.f), false, WALL, PLAYER | ENEMY | OBJECTS | HEXAGON);
					tempPhsBody.SetColor(vec4(0.f, 1.f, 0.f, 0.3f));


				}
			}
			{
				float physBodyX = 140.f;
				float physBodyY = 64.f;
				float posX = 350.f + moveX;
				float posY = 90.f;
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
						float(tempSpr.GetHeight() - shrinkY), vec2(0.f, 12.f), false, WALL, PLAYER | ENEMY | OBJECTS | HEXAGON);
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
						float(physBodyY / 2), vec2(0.f, 16.f), false, GROUND, PLAYER | ENEMY | OBJECTS | HEXAGON);
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
			{
				float physBodyX = 140.f;
				float physBodyY = 64.f;
				float posX = 444.f + moveX;
				float posY = -40.f;
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
						float(tempSpr.GetHeight() - shrinkY), vec2(0.f, 12.f), false, WALL, PLAYER | ENEMY | OBJECTS | HEXAGON);
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
						float(physBodyY / 2), vec2(0.f, 16.f), false, GROUND, PLAYER | ENEMY | OBJECTS | HEXAGON);
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
			{
				float physBodyX = 140.f;
				float physBodyY = 64.f;
				float posX = -55.f + moveX;
				float posY = 120.f;
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
						float(tempSpr.GetHeight() - shrinkY), vec2(0.f, 12.f), false, WALL, PLAYER | ENEMY | OBJECTS | HEXAGON);
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
						float(physBodyY / 2), vec2(0.f, 16.f), false, GROUND, PLAYER | ENEMY | OBJECTS | HEXAGON);
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
			//FLOATING BLOCKS
			{
				//Creates entity
				auto entity = ECS::CreateEntity();

				//Add components
				ECS::AttachComponent<Sprite>(entity);
				ECS::AttachComponent<Transform>(entity);
				ECS::AttachComponent<PhysicsBody>(entity);

				//Sets up components
				std::string fileName = "Tower Inside V2/Base/Filling/Central Filling/Central 3.png";
				ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 64, 64);
				ECS::GetComponent<Transform>(entity).SetPosition(vec3(30.f, 0.f, 3.f));

				auto& tempSpr = ECS::GetComponent<Sprite>(entity);
				auto& tempPhsBody = ECS::GetComponent<PhysicsBody>(entity);

				float shrinkX = 0.f;
				float shrinkY = 0.f;
				b2Body* tempBody;
				b2BodyDef tempDef;
				tempDef.type = b2_staticBody;
				tempDef.position.Set(float32(80 + moveX), float32(45));

				tempBody = m_physicsWorld->CreateBody(&tempDef);

				tempPhsBody = PhysicsBody(entity, tempBody, float(tempSpr.GetWidth() - shrinkX),
					float(tempSpr.GetHeight() - shrinkY), vec2(0.f, 0.f), false, WALL, PLAYER | ENEMY | OBJECTS | HEXAGON);
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
				std::string fileName = "Tower Inside V2/Base/Filling/Central Filling/Central 3.png";
				ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 64, 64);
				ECS::GetComponent<Transform>(entity).SetPosition(vec3(30.f, 0.f, 3.f));

				auto& tempSpr = ECS::GetComponent<Sprite>(entity);
				auto& tempPhsBody = ECS::GetComponent<PhysicsBody>(entity);

				float shrinkX = 0.f;
				float shrinkY = 0.f;
				b2Body* tempBody;
				b2BodyDef tempDef;
				tempDef.type = b2_staticBody;
				tempDef.position.Set(float32(80 + moveX), float32(-19));

				tempBody = m_physicsWorld->CreateBody(&tempDef);

				tempPhsBody = PhysicsBody(entity, tempBody, float(tempSpr.GetWidth() - shrinkX),
					float(tempSpr.GetHeight() - shrinkY), vec2(0.f, 0.f), false, WALL, PLAYER | ENEMY | OBJECTS | HEXAGON);
				tempPhsBody.SetColor(vec4(0.f, 1.f, 0.f, 0.3f));

			}
			//SPIKES
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
				ECS::GetComponent<Transform>(entity).SetPosition(vec3(30.f, -20.f, 2.f));

				auto& tempSpr = ECS::GetComponent<Sprite>(entity);
				auto& tempPhsBody = ECS::GetComponent<PhysicsBody>(entity);

				float shrinkX = 0.f;
				float shrinkY = 0.f;
				b2Body* tempBody;
				b2BodyDef tempDef;
				tempDef.type = b2_staticBody;
				tempDef.position.Set(float32(80.f + moveX), float32(76.f));

				tempBody = m_physicsWorld->CreateBody(&tempDef);

				tempPhsBody = PhysicsBody(entity, tempBody, float(tempSpr.GetWidth() - shrinkX),
					float(tempSpr.GetHeight() - shrinkY), vec2(0.f, 0.f), false, ENEMY, PLAYER | ENEMY | OBJECTS | HEXAGON);
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
				std::string fileName = "Tower Inside V2/Spikes R.png";
				ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 32, 32);
				ECS::GetComponent<Transform>(entity).SetPosition(vec3(30.f, -20.f, 2.f));

				auto& tempSpr = ECS::GetComponent<Sprite>(entity);
				auto& tempPhsBody = ECS::GetComponent<PhysicsBody>(entity);

				float shrinkX = 0.f;
				float shrinkY = 0.f;
				b2Body* tempBody;
				b2BodyDef tempDef;
				tempDef.type = b2_staticBody;
				tempDef.position.Set(float32(278.f + moveX), float32(100.f));

				tempBody = m_physicsWorld->CreateBody(&tempDef);

				tempPhsBody = PhysicsBody(entity, tempBody, float(tempSpr.GetWidth() - shrinkX),
					float(tempSpr.GetHeight() - shrinkY), vec2(0.f, 0.f), false, ENEMY, PLAYER | ENEMY | OBJECTS | HEXAGON);
				tempPhsBody.SetColor(vec4(0.f, 1.f, 0.f, 0.3f));

			}
			for(int i = 0; i <4; i++) {
				{

					//Creates entity
					auto entity = ECS::CreateEntity();

					//Add components
					ECS::AttachComponent<Sprite>(entity);
					ECS::AttachComponent<Transform>(entity);
					ECS::AttachComponent<PhysicsBody>(entity);

					//Sets up components
					std::string fileName = "Tower Inside V2/Spikes R.png";
					ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 32, 32);
					ECS::GetComponent<Transform>(entity).SetPosition(vec3(30.f, -20.f, 2.f));

					auto& tempSpr = ECS::GetComponent<Sprite>(entity);
					auto& tempPhsBody = ECS::GetComponent<PhysicsBody>(entity);

					float shrinkX = 0.f;
					float shrinkY = 3.f;
					b2Body* tempBody;
					b2BodyDef tempDef;
					tempDef.type = b2_staticBody;
					tempDef.position.Set(float32(52.f + moveX), float32(-36.f + 32*i));

					tempBody = m_physicsWorld->CreateBody(&tempDef);

					tempPhsBody = PhysicsBody(entity, tempBody, float(tempSpr.GetWidth() - shrinkX),
						float(tempSpr.GetHeight() - shrinkY), vec2(0.f, 0.f), false, ENEMY, PLAYER | ENEMY | OBJECTS | HEXAGON);
					tempPhsBody.SetColor(vec4(0.f, 0.f, 1.f, 0.3f));

				}
			}
			{
				float physBodyX = 140.f;
				float physBodyY = 64.f;
				float posX = 300.f + moveX;
				float posY = -80.f;
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
						float(tempSpr.GetHeight() - shrinkY), vec2(0.f, 12.f), false, WALL, PLAYER | ENEMY | OBJECTS | HEXAGON);
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
						float(physBodyY / 2), vec2(0.f, 16.f), false, GROUND, PLAYER | ENEMY | OBJECTS | HEXAGON);
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

					//LADDER TO NEXT LEVEL//LOCKED LADDER
			{

				//Creates entity
				auto entity = ECS::CreateEntity();
				lockedLadder = entity;

				//Add components
				ECS::AttachComponent<Sprite>(entity);
				ECS::AttachComponent<Transform>(entity);
				ECS::AttachComponent<PhysicsBody>(entity);
				ECS::AttachComponent<Trigger*>(entity);


				//Sets up components
				std::string fileName = "Tower Inside V2/Ladder_Locked.png";
				ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 64, 192);
				ECS::GetComponent<Transform>(entity).SetPosition(vec3(30.f, -20.f, 2.f));
				//ECS::GetComponent<Trigger*>(entity) = new LadderTrigger();
				//ECS::GetComponent<Trigger*>(entity)->SetTriggerEntity(entity);

				auto& tempSpr = ECS::GetComponent<Sprite>(entity);
				auto& tempPhsBody = ECS::GetComponent<PhysicsBody>(entity);

				float shrinkX = 40.f;
				float shrinkY = 20.f;
				b2Body* tempBody;
				b2BodyDef tempDef;
				tempDef.type = b2_staticBody;
				tempDef.position.Set(float32(350.f + moveX), float32(200.f));

				tempBody = m_physicsWorld->CreateBody(&tempDef);

				tempPhsBody = PhysicsBody(entity, tempBody, float(tempSpr.GetWidth() - shrinkX),
					float(tempSpr.GetHeight() - shrinkY), vec2(0.f, 0.f), false, DOOR2, PLAYER | OBJECTS);
				tempPhsBody.SetColor(vec4(0.f, 1.f, 0.f, 0.3f));

			}
			{

				//Creates entity
				auto entity = ECS::CreateEntity();

				//Add components
				ECS::AttachComponent<Transform>(entity);
				ECS::AttachComponent<PhysicsBody>(entity);


				//Sets up components
				ECS::GetComponent<Transform>(entity).SetPosition(vec3(30.f, -20.f, 2.f));

				auto& tempPhsBody = ECS::GetComponent<PhysicsBody>(entity);

				b2Body* tempBody;
				b2BodyDef tempDef;
				tempDef.type = b2_staticBody;
				tempDef.position.Set(float32(350.f + moveX), float32(265.f));

				tempBody = m_physicsWorld->CreateBody(&tempDef);

				tempPhsBody = PhysicsBody(entity, tempBody, float(0), float(0), vec2(0.f, 0.f), false, LADDER3, PLAYER | OBJECTS);
				tempPhsBody.SetColor(vec4(1.f, 0.f, 1.f, 0.3f));

			}
			//LADDER TO PREVIOUS LEVEL
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
				ECS::GetComponent<Transform>(entity).SetPosition(vec3(30.f, -20.f, 2.f));
				ECS::GetComponent<Trigger*>(entity) = new LadderTrigger();
				ECS::GetComponent<Trigger*>(entity)->SetTriggerEntity(entity);

				auto& tempSpr = ECS::GetComponent<Sprite>(entity);
				auto& tempPhsBody = ECS::GetComponent<PhysicsBody>(entity);

				float shrinkX = 40.f;
				float shrinkY = 20.f;
				b2Body* tempBody;
				b2BodyDef tempDef;
				tempDef.type = b2_staticBody;
				tempDef.position.Set(float32(-100.f + moveX), float32(-200.f));

				tempBody = m_physicsWorld->CreateBody(&tempDef);

				tempPhsBody = PhysicsBody(entity, tempBody, float(tempSpr.GetWidth() - shrinkX),
					float(tempSpr.GetHeight() - shrinkY), vec2(0.f, 0.f), true, TRIGGER, PLAYER | OBJECTS);
				tempPhsBody.SetColor(vec4(0.f, 1.f, 0.f, 0.3f));

			}
			{

				//Creates entity
				auto entity = ECS::CreateEntity();

				//Add components
				ECS::AttachComponent<Transform>(entity);
				ECS::AttachComponent<PhysicsBody>(entity);


				//Sets up components
				ECS::GetComponent<Transform>(entity).SetPosition(vec3(40.f, -20.f, 2.f));

				auto& tempPhsBody = ECS::GetComponent<PhysicsBody>(entity);

				b2Body* tempBody;
				b2BodyDef tempDef;
				tempDef.type = b2_staticBody;
				tempDef.position.Set(float32(-100.f + moveX), float32(-150.f));

				tempBody = m_physicsWorld->CreateBody(&tempDef);

				tempPhsBody = PhysicsBody(entity, tempBody, float(100), float(1), vec2(0.f, 0.f), false, LADDER2, PLAYER | OBJECTS);
				tempPhsBody.SetColor(vec4(1.f, 0.f, 1.f, 0.3f));

			}


			//RIGHT WALL
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
				tempDef.position.Set(float32(544.f + moveX), float32(-15.f + (increment * i)));

				tempBody = m_physicsWorld->CreateBody(&tempDef);

				tempPhsBody = PhysicsBody(entity, tempBody, float(tempSpr.GetWidth() - shrinkX),
					float(tempSpr.GetHeight() - shrinkY), vec2(0.f, 0.f), false, WALL, PLAYER | ENEMY | OBJECTS | HEXAGON);
				tempPhsBody.SetColor(vec4(0.f, 1.f, 0.f, 0.3f));

				//Filling in the terrain without multiple scopes

			}
		}

		//ROOF
		{

			//Creates entity
			auto entity = ECS::CreateEntity();

			//Add components
			ECS::AttachComponent<Sprite>(entity);
			ECS::AttachComponent<Transform>(entity);
			ECS::AttachComponent<PhysicsBody>(entity);

			//Sets up components
			std::string fileName = "Tower Inside/Base/Filling/roofLong.png";
			ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 768, 64);
			ECS::GetComponent<Transform>(entity).SetPosition(vec3(30.f, -20.f, 3.f));

			auto& tempSpr = ECS::GetComponent<Sprite>(entity);
			auto& tempPhsBody = ECS::GetComponent<PhysicsBody>(entity);

			float shrinkX = 0.f;
			float shrinkY = 0.f;
			b2Body* tempBody;
			b2BodyDef tempDef;
			tempDef.type = b2_staticBody;
			tempDef.position.Set(float32(244 + moveX), float32(320.f));

			tempBody = m_physicsWorld->CreateBody(&tempDef);

			tempPhsBody = PhysicsBody(entity, tempBody, float(tempSpr.GetWidth() - shrinkX),
				float(tempSpr.GetHeight() - shrinkY), vec2(0.f, 0.f), false, OBJECTS, PLAYER | ENEMY | OBJECTS | HEXAGON);
			tempPhsBody.SetColor(vec4(0.f, 1.f, 0.f, 0.3f));





		}

	}
	
	//FLOOR1

	{
		//Creates entity
		auto entity = ECS::CreateEntity();

		//Add components
		ECS::AttachComponent<Sprite>(entity);
		ECS::AttachComponent<Transform>(entity);
		ECS::AttachComponent<PhysicsBody>(entity);

		//Sets up components
		std::string fileName = "WallTiles/floor.png";
		ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 704, 64);
		ECS::GetComponent<Transform>(entity).SetPosition(vec3(30.f, -20.f, 3.f));

		auto& tempSpr = ECS::GetComponent<Sprite>(entity);
		auto& tempPhsBody = ECS::GetComponent<PhysicsBody>(entity);

		float shrinkX = 0.f;
		float shrinkY = 0.f;
		b2Body* tempBody;
		b2BodyDef tempDef;
		tempDef.type = b2_staticBody;
		tempDef.position.Set(float32(300 + moveX), float32(-120.f));

		tempBody = m_physicsWorld->CreateBody(&tempDef);

		tempPhsBody = PhysicsBody(entity, tempBody, float(tempSpr.GetWidth() - shrinkX),
			float(tempSpr.GetHeight() - shrinkY), vec2(0.f, 0.f), false, GROUND, PLAYER | ENEMY | OBJECTS | HEXAGON);
		tempPhsBody.SetColor(vec4(0.f, 1.f, 0.f, 0.3f));

	}




	{
		/*Scene::CreateSprite(m_sceneReg, "HelloWorld.png", 100, 60, 0.5f, vec3(0.f, 0.f, 0.f));*/

		//Creates entity
		auto entity = ECS::CreateEntity();
		auto scroll = File::LoadJSON("Scroll.json");
		std::string fileName = "spritesheets/PNG/scroll.png";
		//Add components
		ECS::AttachComponent<Scroll>(entity);
		ECS::AttachComponent<Sprite>(entity);
		ECS::AttachComponent<Transform>(entity);
		ECS::AttachComponent<PhysicsBody>(entity);
		ECS::AttachComponent<AnimationController>(entity);
		ECS::GetComponent<Sprite>(entity).SetTransparency(1.f);
		ECS::GetComponent<Transform>(entity).SetPosition(vec3(-110, 105, 4.f));
		Scroll::setScrollEntity(2, entity);
		auto& animController = ECS::GetComponent<AnimationController>(entity);
		animController.InitUVs(fileName);
		animController.AddAnimation(scroll["ScrollAnimation"]);
		animController.SetActiveAnim(0);
		ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 32, 32, true, &animController);
		//Set up the components
		auto& tempSpr = ECS::GetComponent<Sprite>(entity);
		auto& tempPhsBody = ECS::GetComponent<PhysicsBody>(entity);

		float shrinkX = 20.f;
		float shrinkY = 20.f;
		b2Body* tempBody;
		b2BodyDef tempDef;
		tempDef.type = b2_staticBody;
		tempDef.position.Set(float32(300 + moveX), float32(135));

		tempBody = m_physicsWorld->CreateBody(&tempDef);

		tempPhsBody = PhysicsBody(entity, tempBody, float(tempSpr.GetWidth() - shrinkX),
			float(tempSpr.GetHeight() - shrinkY), vec2(0.f, 0.f), false, SCROLL2, PLAYER | ENEMY | OBJECTS | HEXAGON);
		tempPhsBody.SetColor(vec4(0.f, 0.f, 1.f, 0.3f));

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
				std::string fileName = "Tower Inside V2/Key3.png";
				ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 32, 32);
				ECS::GetComponent<Transform>(entity).SetPosition(vec3(30.f, -20.f, 4.f));

				auto& tempSpr = ECS::GetComponent<Sprite>(entity);
				auto& tempPhsBody = ECS::GetComponent<PhysicsBody>(entity);

				float shrinkX = 20.f;
				float shrinkY = 20.f;
				b2Body* tempBody;
				b2BodyDef tempDef;
				tempDef.type = b2_staticBody;
				tempDef.position.Set(float32(-100.f + moveX), float32(165.f));

				tempBody = m_physicsWorld->CreateBody(&tempDef);

				tempPhsBody = PhysicsBody(entity, tempBody, float(tempSpr.GetWidth() - shrinkX),
					float(tempSpr.GetHeight() - shrinkY), vec2(0.f, 0.f), false, KEY, PLAYER | ENEMY | OBJECTS | HEXAGON);
				tempPhsBody.SetColor(vec4(0.f, 0.f, 1.f, 0.3f));

			}
		}
	
	
	//END MidLevel

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



	
	
	{
		
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
					tempDef.position.Set(float32(-164.f), float32(-15.f + (increment * i)));

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
						float(tempSpr.GetHeight() - shrinkY), vec2(0.f, 12.f), false, WALL, PLAYER | ENEMY | OBJECTS | HEXAGON);
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
						float(physBodyY / 2), vec2(0.f, 16.f), false, GROUND, PLAYER | ENEMY | OBJECTS | HEXAGON);
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
			//WALL OVER SPIKE
			{
				//Creates entity
				auto entity = ECS::CreateEntity();

				//Add components
				ECS::AttachComponent<Sprite>(entity);
				ECS::AttachComponent<Transform>(entity);
				ECS::AttachComponent<PhysicsBody>(entity);

				//Sets up components
				std::string fileName = "Tower Inside/Base/Filling/Central 1.png";
				ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 32, 128);
				ECS::GetComponent<Transform>(entity).SetPosition(vec3(30.f, 0.f, 3.f));

				auto& tempSpr = ECS::GetComponent<Sprite>(entity);
				auto& tempPhsBody = ECS::GetComponent<PhysicsBody>(entity);

				float shrinkX = 0.f;
				float shrinkY = 0.f;
				b2Body* tempBody;
				b2BodyDef tempDef;
				tempDef.type = b2_staticBody;
				tempDef.position.Set(float32(268), float32(0));

				tempBody = m_physicsWorld->CreateBody(&tempDef);

				tempPhsBody = PhysicsBody(entity, tempBody, float(tempSpr.GetWidth() - shrinkX),
					float(tempSpr.GetHeight() - shrinkY), vec2(0.f, 12.f), false, WALL, PLAYER | ENEMY | OBJECTS | HEXAGON);
				tempPhsBody.SetColor(vec4(0.f, 1.f, 0.f, 0.3f));

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
			//SCROLL
			{
				/*Scene::CreateSprite(m_sceneReg, "HelloWorld.png", 100, 60, 0.5f, vec3(0.f, 0.f, 0.f));*/

				//Creates entity
				auto entity = ECS::CreateEntity();
				auto scroll = File::LoadJSON("Scroll.json");
				std::string fileName = "spritesheets/PNG/scroll.png";
				//Add components
				ECS::AttachComponent<Scroll>(entity);
				ECS::AttachComponent<Sprite>(entity);
				ECS::AttachComponent<Transform>(entity);
				ECS::AttachComponent<PhysicsBody>(entity);
				ECS::AttachComponent<AnimationController>(entity);
				ECS::GetComponent<Sprite>(entity).SetTransparency(1.f);
				ECS::GetComponent<Transform>(entity).SetPosition(vec3(-110, 105, 4.f));
				Scroll::setScrollEntity(1, entity);
				auto& animController = ECS::GetComponent<AnimationController>(entity);
				animController.InitUVs(fileName);
				animController.AddAnimation(scroll["ScrollAnimation"]);
				animController.SetActiveAnim(0);
				ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 32, 32, true, &animController);
				//Set up the components
				auto& tempSpr = ECS::GetComponent<Sprite>(entity);
				auto& tempPhsBody = ECS::GetComponent<PhysicsBody>(entity);
				 
				float shrinkX = 20.f;
				float shrinkY = 20.f;
				b2Body* tempBody;
				b2BodyDef tempDef;
				tempDef.type = b2_staticBody;
				tempDef.position.Set(float32(-110), float32(105));

				tempBody = m_physicsWorld->CreateBody(&tempDef);

				tempPhsBody = PhysicsBody(entity, tempBody, float(tempSpr.GetWidth() - shrinkX),
					float(tempSpr.GetHeight() - shrinkY), vec2(0.f, 0.f), false, SCROLL1, PLAYER | ENEMY | OBJECTS | HEXAGON);
				tempPhsBody.SetColor(vec4(0.f, 0.f, 1.f, 0.3f));
				
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
				
				//Creates entity
				auto entity = ECS::CreateEntity();

				//Add components
				ECS::AttachComponent<Sprite>(entity);
				ECS::AttachComponent<Transform>(entity);
				ECS::AttachComponent<PhysicsBody>(entity);

				//Sets up components
				std::string fileName = "Tower Inside/Base/Filling/roofLong.png";
				ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 780, 64);
				ECS::GetComponent<Transform>(entity).SetPosition(vec3(30.f, -20.f, 3.f));

				auto& tempSpr = ECS::GetComponent<Sprite>(entity);
				auto& tempPhsBody = ECS::GetComponent<PhysicsBody>(entity);

				float shrinkX = 0.f;
				float shrinkY = 0.f;
				b2Body* tempBody;
				b2BodyDef tempDef;
				tempDef.type = b2_staticBody;
				tempDef.position.Set(float32(255.f), float32(300.f));

				tempBody = m_physicsWorld->CreateBody(&tempDef);

				tempPhsBody = PhysicsBody(entity, tempBody, float(tempSpr.GetWidth() - shrinkX),
					float(tempSpr.GetHeight() - shrinkY), vec2(0.f, 0.f), false, OBJECTS, PLAYER | ENEMY | OBJECTS | HEXAGON);
				tempPhsBody.SetColor(vec4(0.f, 1.f, 0.f, 0.3f));

					



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
				ECS::GetComponent<Transform>(entity).SetPosition(vec3(30.f, -20.f, 2.f));
				ECS::GetComponent<Trigger*>(entity) = new LadderTrigger();
				ECS::GetComponent<Trigger*>(entity)->SetTriggerEntity(entity);

				auto& tempSpr = ECS::GetComponent<Sprite>(entity);
				auto& tempPhsBody = ECS::GetComponent<PhysicsBody>(entity);

				float shrinkX = 40.f;
				float shrinkY = 20.f;
				b2Body* tempBody;
				b2BodyDef tempDef;
				tempDef.type = b2_staticBody;
				tempDef.position.Set(float32(565.f), float32(165.f));

				tempBody = m_physicsWorld->CreateBody(&tempDef);

				tempPhsBody = PhysicsBody(entity, tempBody, float(tempSpr.GetWidth() - shrinkX),
					float(tempSpr.GetHeight() - shrinkY), vec2(0.f, 0.f), true, TRIGGER, PLAYER | OBJECTS);
				tempPhsBody.SetColor(vec4(0.f, 1.f, 0.f, 0.3f));

			}
			{

				//Creates entity
				auto entity = ECS::CreateEntity();

				//Add components
				ECS::AttachComponent<Transform>(entity);
				ECS::AttachComponent<PhysicsBody>(entity);


				//Sets up components
				ECS::GetComponent<Transform>(entity).SetPosition(vec3(30.f, -20.f, 2.f));

				auto& tempPhsBody = ECS::GetComponent<PhysicsBody>(entity);

				b2Body* tempBody;
				b2BodyDef tempDef;
				tempDef.type = b2_staticBody;
				tempDef.position.Set(float32(565.f), float32(240.f));

				tempBody = m_physicsWorld->CreateBody(&tempDef);

				tempPhsBody = PhysicsBody(entity, tempBody, float(0),float(0), vec2(0.f, 0.f), false, LADDER, PLAYER | OBJECTS);
				tempPhsBody.SetColor(vec4(1.f, 0.f, 1.f, 0.3f));

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
					std::string fileName = "Tower Inside/Base/DoorClosed.png";
					ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 64, 64);
					ECS::GetComponent<Transform>(entity).SetPosition(vec3(30.f, -20.f, 6.f));

					auto& tempSpr = ECS::GetComponent<Sprite>(entity);
					auto& tempPhsBody = ECS::GetComponent<PhysicsBody>(entity);

					float shrinkX = 0.f;
					float shrinkY = 0.f;
					b2Body* tempBody;
					b2BodyDef tempDef;
					tempDef.type = b2_staticBody;
					tempDef.position.Set(float32(540.f), float32(-44.f));

					tempBody = m_physicsWorld->CreateBody(&tempDef);
					
					tempPhsBody = PhysicsBody(entity, tempBody, float(tempSpr.GetWidth() - shrinkX),
						float(tempSpr.GetHeight() - shrinkY), vec2(0.f, 0.f), false, DOOR, PLAYER | ENEMY | OBJECTS | HEXAGON);
					tempPhsBody.SetColor(vec4(0.f, 1.f, 0.f, 0.3f));

				}
				{

					//Creates entity
					auto entity = ECS::CreateEntity();
					//Add components
					ECS::AttachComponent<Sprite>(entity);
					ECS::AttachComponent<Transform>(entity);
					ECS::AttachComponent<PhysicsBody>(entity);
					ECS::AttachComponent<Door>(entity);

					//Sets up components
					std::string fileName = "Tower Inside/Base/Ground Floor Center TH.png";
					ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 64, 64);
					ECS::GetComponent<Transform>(entity).SetPosition(vec3(30.f, -20.f, 5.f));

					auto& tempSpr = ECS::GetComponent<Sprite>(entity);
					auto& tempPhsBody = ECS::GetComponent<PhysicsBody>(entity);

					float shrinkX = 0.f;
					float shrinkY = 0.f;
					b2Body* tempBody;
					b2BodyDef tempDef;
					tempDef.type = b2_staticBody;
					tempDef.position.Set(float32(576.f), float32(-56.f));

					tempBody = m_physicsWorld->CreateBody(&tempDef);

					tempPhsBody = PhysicsBody(entity, tempBody, float(tempSpr.GetWidth() - shrinkX),
						float(tempSpr.GetHeight() - shrinkY), vec2(0.f, 0.f), false, DOOR, PLAYER | ENEMY | OBJECTS | HEXAGON);
					tempPhsBody.SetColor(vec4(0.f, 1.f, 0.f, 0.3f));

				}

				{

					//Creates entity
					auto entity = ECS::CreateEntity();
					//Add components
					ECS::AttachComponent<Sprite>(entity);
					ECS::AttachComponent<Transform>(entity);

					//Sets up components
					std::string fileName = "Tower Inside/Base/Ground Floor TH R.png";
					ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 64, 64);
					ECS::GetComponent<Transform>(entity).SetPosition(vec3(640.f, -56.f, 3.f));

					

				}
				{

					//Creates entity
					auto entity = ECS::CreateEntity();
					//Add components
					ECS::AttachComponent<Sprite>(entity);
					ECS::AttachComponent<Transform>(entity);

					//Sets up components
					std::string fileName = "Tower Inside/Base/Ground Floor TH L.png";
					ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 64, 64);
					ECS::GetComponent<Transform>(entity).SetPosition(vec3(512.f, -56.f, 3.f));

				}
				{

					//Creates entity
					auto entity = ECS::CreateEntity();
					//Add components
					ECS::AttachComponent<Sprite>(entity);
					ECS::AttachComponent<Transform>(entity);
					ECS::AttachComponent<PhysicsBody>(entity);

					//Sets up components
					std::string fileName = "Tower Inside/Base/Ground Floor UL.png";
					ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 64, 64);
					ECS::GetComponent<Transform>(entity).SetPosition(vec3(30.f, -20.f, 2.f));

					auto& tempSpr = ECS::GetComponent<Sprite>(entity);
					auto& tempPhsBody = ECS::GetComponent<PhysicsBody>(entity);

					float shrinkX = 0.f;
					float shrinkY = 0.f;
					b2Body* tempBody;
					b2BodyDef tempDef;
					tempDef.type = b2_staticBody;
					tempDef.position.Set(float32(512.f), float32(8.f));

					tempBody = m_physicsWorld->CreateBody(&tempDef);

					tempPhsBody = PhysicsBody(entity, tempBody, float(tempSpr.GetWidth() - shrinkX),
						float(tempSpr.GetHeight() - shrinkY), vec2(0.f, 0.f), false, WALL, PLAYER | ENEMY | OBJECTS | HEXAGON);
					tempPhsBody.SetColor(vec4(0.f, 1.f, 0.f, 0.3f));

				}
				{

					//Creates entity
					auto entity = ECS::CreateEntity();
					//Add components
					ECS::AttachComponent<Sprite>(entity);
					ECS::AttachComponent<Transform>(entity);

					//Sets up components
					std::string fileName = "Tower Inside/Base/Ground Floor U.png";
					ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 64, 64);
					ECS::GetComponent<Transform>(entity).SetPosition(vec3(576.f, 8.f, 2.f));

				}
				{

					//Creates entity
					auto entity = ECS::CreateEntity();
					//Add components
					ECS::AttachComponent<Sprite>(entity);
					ECS::AttachComponent<Transform>(entity);

					//Sets up components
					std::string fileName = "Tower Inside/Base/Ground Floor UR.png";
					ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 64, 64);
					ECS::GetComponent<Transform>(entity).SetPosition(vec3(640.f,8.f, 2.f));

					
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
			
				{
					//Creates entity
					auto entity = ECS::CreateEntity();

					//Add components
					ECS::AttachComponent<Sprite>(entity);
					ECS::AttachComponent<Transform>(entity);
					ECS::AttachComponent<PhysicsBody>(entity);

					//Sets up components
					std::string fileName = "WallTiles/floor.png";
					ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 768, 64);
					ECS::GetComponent<Transform>(entity).SetPosition(vec3(30.f, -20.f, 3.f));

					auto& tempSpr = ECS::GetComponent<Sprite>(entity);
					auto& tempPhsBody = ECS::GetComponent<PhysicsBody>(entity);

					float shrinkX = 0.f;
					float shrinkY = 0.f;
					b2Body* tempBody;
					b2BodyDef tempDef;
					tempDef.type = b2_staticBody;
					tempDef.position.Set(float32(278), float32(-120.f));

					tempBody = m_physicsWorld->CreateBody(&tempDef);

					tempPhsBody = PhysicsBody(entity, tempBody, float(tempSpr.GetWidth() - shrinkX),
						float(tempSpr.GetHeight() - shrinkY), vec2(0.f, 0.f), false, GROUND, PLAYER | ENEMY | OBJECTS | HEXAGON);
					tempPhsBody.SetColor(vec4(0.f, 1.f, 0.f, 0.3f));

				}
				
			
			//SPIKE ON Ground

			for (int i = 0; i < 7; i++) {
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
					tempDef.position.Set(float32(168.f+(32*i)), float32(-90.f));

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
	VignetteEffect* vig = (VignetteEffect*)EffectManager::GetEffect(EffectManager::GetVignetteHandle());
	
	
	
	if (m_lerpEnabled) {
		m_tVal += Timer::deltaTime;
		m_lerpVal = Util::Lerp<float>(m_val1, m_val2, m_tVal);
		
	}

	auto& player = ECS::GetComponent<PhysicsBody>(MainEntities::MainPlayer());
	auto& level = ECS::GetComponent<CanJump>(MainEntities::MainPlayer());
	auto& cam = ECS::GetComponent<Camera>(MainEntities::MainCamera());
	auto& animations = ECS::GetComponent<Player>(MainEntities::MainPlayer());
	animations.Update();
	
	

		auto& door = ECS::GetComponent<Door>(MainEntities::doorBasement());
		auto& removeDoor = ECS::GetComponent<PhysicsBody>(MainEntities::doorBasement());
		auto& removeLadder = ECS::GetComponent<PhysicsBody>(lockedLadder);
		//auto& anim = ECS::GetComponent<AnimationController>(MainEntities::MainPlayer());

		//DOOR TO BASEMENT OPEN
		if (door.doorOpenOne) {

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
				std::string fileName = "Tower Inside/Base/DoorOpen.png";
				ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 64, 64);
				ECS::GetComponent<Transform>(entity).SetPosition(vec3(30.f, -20.f, 7.f));

				auto& tempSpr = ECS::GetComponent<Sprite>(entity);
				auto& tempPhsBody = ECS::GetComponent<PhysicsBody>(entity);

				float shrinkX = 40.f;
				float shrinkY = 40.f;
				b2Body* tempBody;
				b2BodyDef tempDef;
				tempDef.type = b2_staticBody;
				tempDef.position.Set(float32(removeDoor.GetPosition().x), float32(removeDoor.GetPosition().y));

				tempBody = m_physicsWorld->CreateBody(&tempDef);

				tempPhsBody = PhysicsBody(entity, tempBody, float(tempSpr.GetWidth() - shrinkX),
					float(tempSpr.GetHeight() - shrinkY), vec2(20.f, 0.f), false, DOOR, PLAYER | ENEMY | OBJECTS | HEXAGON);
				tempPhsBody.SetColor(vec4(1.f, 0.f, 0.f, 0.3f));

			}
			removeDoor.SetPosition(b2Vec2(-100, -400), true);
			auto& door = ECS::GetComponent<Door>(MainEntities::doorBasement());
			door.doorOpenOne = false;
			door.doorTransportOne = true;
		}
		if (door.doorOpenTwo) {

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
				ECS::GetComponent<Transform>(entity).SetPosition(vec3(30.f, -20.f, 2.f));
				ECS::GetComponent<Trigger*>(entity) = new LadderTrigger();
				ECS::GetComponent<Trigger*>(entity)->SetTriggerEntity(entity);

				auto& tempSpr = ECS::GetComponent<Sprite>(entity);
				auto& tempPhsBody = ECS::GetComponent<PhysicsBody>(entity);

				float shrinkX = 40.f;
				float shrinkY = 20.f;
				b2Body* tempBody;
				b2BodyDef tempDef;
				tempDef.type = b2_staticBody;
				tempDef.position.Set(float32(350.f + 2500.f), float32(200.f));

				tempBody = m_physicsWorld->CreateBody(&tempDef);

				tempPhsBody = PhysicsBody(entity, tempBody, float(tempSpr.GetWidth() - shrinkX),
					float(tempSpr.GetHeight() - shrinkY), vec2(0.f, 0.f), true, TRIGGER, PLAYER | OBJECTS);
				tempPhsBody.SetColor(vec4(0.f, 1.f, 0.f, 0.3f));

			}
			{

				//Creates entity
				auto entity = ECS::CreateEntity();

				//Add components
				ECS::AttachComponent<Transform>(entity);
				ECS::AttachComponent<PhysicsBody>(entity);


				//Sets up components
				ECS::GetComponent<Transform>(entity).SetPosition(vec3(30.f, -20.f, 2.f));

				auto& tempPhsBody = ECS::GetComponent<PhysicsBody>(entity);

				b2Body* tempBody;
				b2BodyDef tempDef;
				tempDef.type = b2_staticBody;
				tempDef.position.Set(float32(350.f + 1500.f), float32(265.f));

				tempBody = m_physicsWorld->CreateBody(&tempDef);

				tempPhsBody = PhysicsBody(entity, tempBody, float(0), float(0), vec2(0.f, 0.f), false, LADDER3, PLAYER | OBJECTS);
				tempPhsBody.SetColor(vec4(1.f, 0.f, 1.f, 0.3f));

			}
			removeLadder.SetPosition(b2Vec2(-100, -4000), true);
			auto& door = ECS::GetComponent<Door>(MainEntities::doorBasement());
			door.doorOpenTwo = false;
			door.doorTransportTwo = true;
		}
		
		
	
}

void TowerGroundScene::KeyboardHold()
{
	auto& player = ECS::GetComponent<PhysicsBody>(MainEntities::MainPlayer());
	auto& ladder = ECS::GetComponent<CanJump>(MainEntities::MainPlayer());
	/*VignetteEffect* vig = (VignetteEffect*)EffectManager::GetEffect(EffectManager::GetVignetteHandle());
	if (m_lerpEnabled) {
		vig->SetOuterRadius(m_lerpVal);
		if (m_tVal >= 1.f) {
			m_lerpEnabled = false;
			m_tVal = 0.f;

		}
		
	}*/
	


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

	

	if (Input::GetKeyDown(Key::O))
	{
		Scene::setSceneChange(0);
	}

}

void TowerGroundScene::KeyboardDown()
{
	auto& player = ECS::GetComponent<PhysicsBody>(MainEntities::MainPlayer());
	auto& canJump = ECS::GetComponent<CanJump>(MainEntities::MainPlayer());
	auto& playerTrans = ECS::GetComponent<Transform>(MainEntities::MainPlayer());
	

	//REMOVE
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
		//REMOVE
		if (Input::GetKeyDown(Key::M)) {
			player.SetVelocity(vec3(0, 0, 0));
			player.SetPosition(b2Vec2(-100 + 1000, 20 + 1000 ),true);
			//player.SetPosition(b2Vec2(565, 200), true);
			player.GetBody()->ApplyLinearImpulseToCenter(b2Vec2(0.f, -1.f), true);
			canJump.currLevel = "toplevel";
			canJump.enterLeft = true;
		}
		//REMOVE
		if (Input::GetKeyDown(Key::N)) { 
			//canJump.enterLeft = true;
			std::cout << player.GetPosition().x  << " ";
			std::cout << player.GetPosition().y << std::endl;
			auto& door = ECS::GetComponent<Door>(MainEntities::doorBasement());
			std::cout << door.doorOpenTwo;
			std::cout << door.doorTransportTwo;
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

	




}