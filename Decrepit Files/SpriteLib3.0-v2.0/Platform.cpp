#include "Platform.h"
#include "Utilities.h"
#include "Scene.h"
#include "TowerGroundScene.h"
#include <random>
Platform::Platform()
{
}
auto Platform::createPlatform(b2World* physWorld) {
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
				float shrinkY = 0.f;
				b2Body* tempBody;
				b2BodyDef tempDef;
				tempDef.type = b2_staticBody;
				tempDef.position.Set(float32(0.f), float32(16.f));

				tempBody = physWorld->CreateBody(&tempDef);

				tempPhsBody = PhysicsBody(entity, tempBody, float(tempSpr.GetWidth() - shrinkX),
					float(tempSpr.GetHeight() - shrinkY), vec2(0.f, 0.f), false, WALL, PLAYER | ENEMY | OBJECTS | HEXAGON);
				tempPhsBody.SetColor(vec4(0.f, 1.f, 0.f, 0.3f));

	}
}