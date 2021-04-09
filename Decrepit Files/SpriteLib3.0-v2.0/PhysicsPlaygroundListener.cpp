#include "PhysicsPlaygroundListener.h"

#include "ECS.h"
#include "Utilities.h"
#include "Scene.h"
#include "Scroll.h"
using namespace std;
PhysicsPlaygroundListener::PhysicsPlaygroundListener()
	: b2ContactListener()
{

}


void PhysicsPlaygroundListener::BeginContact(b2Contact* contact)
{
	b2Fixture* fixtureA = contact->GetFixtureA();
	b2Fixture* fixtureB = contact->GetFixtureB();

	bool sensorA = fixtureA->IsSensor();
	bool sensorB = fixtureB->IsSensor();

	//if neither or both are sensors, will be false
	if ((sensorA ^ sensorB))
	{
		if (sensorA)
		{
			TriggerEnter(fixtureA);
		}
		else if (sensorB)
		{
			TriggerEnter(fixtureB);
		}
	}

	b2Filter filterA = fixtureA->GetFilterData();
	b2Filter filterB = fixtureB->GetFilterData();
	VignetteEffect* vig = (VignetteEffect*)EffectManager::GetEffect(EffectManager::GetVignetteHandle());
	auto& playerJump = ECS::GetComponent<CanJump>((int)fixtureB->GetBody()->GetUserData());
	auto& level = ECS::GetComponent<CanJump>(MainEntities::MainPlayer());
	auto& player = ECS::GetComponent<PhysicsBody>(MainEntities::MainPlayer());
	auto& keyOne = ECS::GetComponent<PhysicsBody>(MainEntities::keyOne());
	auto& keyTwo = ECS::GetComponent<PhysicsBody>(MainEntities::keyTwo());
	auto& door = ECS::GetComponent<Door>(MainEntities::doorBasement());
	auto& cam = ECS::GetComponent<Camera>(MainEntities::MainCamera());
	auto& scroll1 = ECS::GetComponent<PhysicsBody>(Scroll::getScrollEntity(1));
	auto& scroll2 = ECS::GetComponent<PhysicsBody>(Scroll::getScrollEntity(2));
	auto& scroll3 = ECS::GetComponent<PhysicsBody>(Scroll::getScrollEntity(3));
	auto& scroll4 = ECS::GetComponent<PhysicsBody>(Scroll::getScrollEntity(4));
	auto& scroll5 = ECS::GetComponent<PhysicsBody>(Scroll::getScrollEntity(5));
	auto endCard = ECS::GetComponent<Transform>(Scroll::getScrollEntity(6));
	/*bool lerpEnabled = false;
	float tVal = 0.f;
	float lerpVal = 0.f;
	float val1 = 0.f, val2 = 0.f;
	if (lerpEnabled) {
		tVal += Timer::deltaTime;
		lerpVal = Util::Lerp<float>(val1, val2, tVal);

	}
	if (lerpEnabled) {
		vig->SetOuterRadius(lerpVal);
		if (tVal >= 1.f) {
			lerpEnabled = false;
			tVal = 0.f;
			lerpVal = val2;

		}

	}*/

	if ((filterA.categoryBits == PLAYER && filterB.categoryBits == GROUND) || (filterB.categoryBits == PLAYER && filterA.categoryBits == GROUND))
	{
		if (filterA.categoryBits == PLAYER)
		{
			playerJump.m_canJump = true;
			playerJump.m_wallJumpNum = 1;
		}
		else if (filterB.categoryBits == PLAYER)
		{
			playerJump.m_canJump = true;
			playerJump.m_wallJumpNum = 1;
		}
	}
	if ((filterA.categoryBits == PLAYER && filterB.categoryBits == ENEMY) || (filterB.categoryBits == PLAYER && filterA.categoryBits == ENEMY))
	{
		
		
			if (filterA.categoryBits == PLAYER)
			{
				if (playerJump.currLevel == "levelone") {
					player.SetVelocity(vec3(0, 0, 0));
					player.SetPosition(b2Vec2(-20, -50), true);
					player.GetBody()->ApplyLinearImpulseToCenter(b2Vec2(0.f, -1.f), true);
					playerJump.enterLeft = true;
				}
				
			}
			else if (filterB.categoryBits == PLAYER)
			{
				if (playerJump.currLevel == "levelone") {
					player.SetVelocity(vec3(0, 0, 0));
					player.SetPosition(b2Vec2(-20, -50), true);
					player.GetBody()->ApplyLinearImpulseToCenter(b2Vec2(0.f, -1.f), true);
					playerJump.enterLeft = true;
				}
			}
			
		if (playerJump.currLevel == "basement") {
			if (filterA.categoryBits == PLAYER)
			{
				player.SetVelocity(vec3(0, 0, 0));
				player.SetPosition(b2Vec2(1400, 20), true);
				player.GetBody()->ApplyLinearImpulseToCenter(b2Vec2(0.f, -1.f), true);
			}
			else if (filterB.categoryBits == PLAYER)
			{
				
				player.SetVelocity(vec3(0, 0, 0));
				player.SetPosition(b2Vec2(1400, 20), true);
				player.GetBody()->ApplyLinearImpulseToCenter(b2Vec2(0.f, -1.f), true);
			}


		}
		if (playerJump.currLevel == "toplevel") {

		}
		if (playerJump.currLevel == "leveltwo") {
			if (filterA.categoryBits == PLAYER)
			{
				player.SetVelocity(vec3(0, 0, 0));
				player.SetPosition(b2Vec2(2475, -40), true);
				player.GetBody()->ApplyLinearImpulseToCenter(b2Vec2(0.f, -1.f), true);
			}
			else if (filterB.categoryBits == PLAYER)
			{
				player.SetVelocity(vec3(0, 0, 0));
				player.SetPosition(b2Vec2(2475, -40), true);
				player.GetBody()->ApplyLinearImpulseToCenter(b2Vec2(0.f, -1.f), true);
			}
		}
		
		

		
	}

	if ((filterA.categoryBits == PLAYER && filterB.categoryBits == OBJECTS) || (filterB.categoryBits == PLAYER && filterA.categoryBits == OBJECTS))
	{
		if (filterA.categoryBits == PLAYER)
		{
			playerJump.m_canJump = false;
			playerJump.m_wallJumpNum = 1;
		}
		else if (filterB.categoryBits == PLAYER)
		{
			playerJump.m_canJump = false;
			playerJump.m_wallJumpNum = 1;
		}
	}


	if ((filterA.categoryBits == PLAYER && filterB.categoryBits == WALL) || (filterB.categoryBits == PLAYER && filterA.categoryBits == WALL))
	{
		if (filterA.categoryBits == PLAYER)
		{
			cout << "TEST";
			playerJump.m_wallJump = true;
			if (player.GetPosition().x > fixtureB->GetBody()->GetPosition().x) {
				playerJump.m_facingRight = true;
			}
			else {
				playerJump.m_facingRight = false;
			}

		}
		else if (filterB.categoryBits == PLAYER)
		{
			cout << "TEST";
			playerJump.m_wallJump = true;
			if (player.GetPosition().x > fixtureA->GetBody()->GetPosition().x) {
				playerJump.m_facingRight = true;
			}
			else {
				playerJump.m_facingRight = false;
			}

		}
	}



	if ((filterA.categoryBits == PLAYER && filterB.categoryBits == SCROLL1) || (filterB.categoryBits == PLAYER && filterA.categoryBits == SCROLL1))
	{
		if (filterA.categoryBits == PLAYER)
		{
			scroll1.SetPosition(b2Vec2(-100, -4000), true);
			//mciSendString("close mp3 ", NULL, 0, NULL);
			mciSendString("open \"1FloorScroll.mp3\" type mpegvideo alias mp3a", NULL, 0, NULL);
			mciSendString("play mp3a Notify", NULL, 0, NULL);
			

		}
		else if (filterB.categoryBits == PLAYER)
		{
			scroll1.SetPosition(b2Vec2(-100, -4000), true);
			//mciSendString("close mp3 ", NULL, 0, NULL);
			mciSendString("open \"1FloorScroll.mp3\" type mpegvideo alias mp3a", NULL, 0, NULL);
			mciSendString("play mp3a Notify", NULL, 0, NULL);

		}
	}
	if ((filterA.categoryBits == PLAYER && filterB.categoryBits == SCROLL2) || (filterB.categoryBits == PLAYER && filterA.categoryBits == SCROLL2))
	{
		if (filterA.categoryBits == PLAYER)
		{
			scroll2.SetPosition(b2Vec2(-100, -4000), true);
			//mciSendString("close mp3 ", NULL, 0, NULL);
			mciSendString("open \"3FloorScroll.mp3\" type mpegvideo alias mp3b", NULL, 0, NULL);
			mciSendString("play mp3b Notify", NULL, 0, NULL);

		}
		else if (filterB.categoryBits == PLAYER)
		{
			scroll2.SetPosition(b2Vec2(-100, -4000), true);
			//mciSendString("close mp3 ", NULL, 0, NULL);
			mciSendString("open \"3FloorScroll.mp3\" type mpegvideo alias mp3b", NULL, 0, NULL);
			mciSendString("play mp3b Notify", NULL, 0, NULL);

		}
	}
	if ((filterA.categoryBits == PLAYER && filterB.categoryBits == SCROLL3) || (filterB.categoryBits == PLAYER && filterA.categoryBits == SCROLL3))
	{
		if (filterA.categoryBits == PLAYER)
		{
			scroll3.SetPosition(b2Vec2(-100, -4000), true);
			mciSendString("open \"2FloorScroll.mp3\" type mpegvideo alias mp3c", NULL, 0, NULL);
			mciSendString("play mp3c", NULL, 0, NULL);

		}
		else if (filterB.categoryBits == PLAYER)
		{
			scroll3.SetPosition(b2Vec2(-100, -4000), true);
			mciSendString("open \"2FloorScroll.mp3\" type mpegvideo alias mp3c", NULL, 0, NULL);
			mciSendString("play mp3c", NULL, 0, NULL);

		}
	}
	if ((filterA.categoryBits == PLAYER && filterB.categoryBits == SCROLL4) || (filterB.categoryBits == PLAYER && filterA.categoryBits == SCROLL4))
	{
		if (filterA.categoryBits == PLAYER)
		{
			scroll4.SetPosition(b2Vec2(-100, -4000), true);
			mciSendString("open \"GoodEndP1.mp3\" type mpegvideo alias mp3d", NULL, 0, NULL);
			mciSendString("play mp3d", NULL, 0, NULL);

		}
		else if (filterB.categoryBits == PLAYER)
		{
			scroll4.SetPosition(b2Vec2(-100, -4000), true);
			mciSendString("open \"GoodEndP1.mp3\" type mpegvideo alias mp3d", NULL, 0, NULL);
			mciSendString("play mp3d", NULL, 0, NULL);

		}
	}
	if ((filterA.categoryBits == PLAYER && filterB.categoryBits == SCROLL5) || (filterB.categoryBits == PLAYER && filterA.categoryBits == SCROLL5))
	{
		if (filterA.categoryBits == PLAYER)
		{
			//scroll5.SetPosition(b2Vec2(-100, -4000), true);
			mciSendString("open \"GoodEndP2.mp3\" type mpegvideo alias mp3e", NULL, 0, NULL);
			mciSendString("play mp3e", NULL, 0, NULL);

		}
		else if (filterB.categoryBits == PLAYER)
		{
			//scroll5.SetPosition(b2Vec2(-100, -4000), true);
			mciSendString("open \"GoodEndP2.mp3\" type mpegvideo alias mp3e", NULL, 0, NULL);
			mciSendString("play mp3e", NULL, 0, NULL);

		}
	}
	if ((filterA.categoryBits == PLAYER && filterB.categoryBits == KEY) || (filterB.categoryBits == PLAYER && filterA.categoryBits == KEY))
		{
			if (filterA.categoryBits == PLAYER)
			{
				keyOne.SetPosition(b2Vec2(-100, -400), true);
				door.haveKeyOne = true;

			}
			else if (filterB.categoryBits == PLAYER)
			{
				keyOne.SetPosition(b2Vec2(-100, -400), true);
				door.haveKeyOne = true;

			}
	}
	if ((filterA.categoryBits == PLAYER && filterB.categoryBits == KEY2) || (filterB.categoryBits == PLAYER && filterA.categoryBits == KEY2))
	{
		if (filterA.categoryBits == PLAYER)
		{
			keyTwo.SetPosition(b2Vec2(-100, -400), true);
			door.haveKeyTwo = true;

		}
		else if (filterB.categoryBits == PLAYER)
		{
			keyTwo.SetPosition(b2Vec2(-100, -400), true);
			door.haveKeyTwo = true;

		}
	}
	
		if ((filterA.categoryBits == PLAYER && filterB.categoryBits == DOOR) || (filterB.categoryBits == PLAYER && filterA.categoryBits == DOOR))
		{
			if (filterA.categoryBits == PLAYER)
			{
				//cout << door.doorOpen;
				if (door.haveKeyOne) {
					door.doorOpenOne = true;
				}
				if (door.doorTransportOne) {
					
					player.SetPosition(b2Vec2(1400, 20), true);
					playerJump.currLevel = "basement";
				}

			}
			else if (filterB.categoryBits == PLAYER)
			{
				//cout << door.doorOpen;

				if (door.haveKeyOne) {
					door.doorOpenOne = true;
				}
				if (door.doorTransportOne) {
					
					player.SetPosition(b2Vec2(1400, 20), true);
					playerJump.currLevel = "basement";
				}

			}
		}
		if ((filterA.categoryBits == PLAYER && filterB.categoryBits == DOOR2) || (filterB.categoryBits == PLAYER && filterA.categoryBits == DOOR2))
		{
			if (filterA.categoryBits == PLAYER)
			{
				//cout << door.doorOpen;
				if (door.haveKeyTwo) {
					door.doorOpenTwo = true;
				}
				

			}
			else if (filterB.categoryBits == PLAYER)
			{
				//cout << door.doorOpen;

				if (door.haveKeyTwo) {
					door.doorOpenTwo = true;
				}
				

			}
		}
		if ((filterA.categoryBits == PLAYER && filterB.categoryBits == DOORFIN) || (filterB.categoryBits == PLAYER && filterA.categoryBits == DOORFIN))
		{
			if (filterA.categoryBits == PLAYER)
			{
				playerJump.currLevel = "endcard";


			}
			else if (filterB.categoryBits == PLAYER)
			{
				playerJump.currLevel = "endcard";


			}
		}
		if ((filterA.categoryBits == PLAYER && filterB.categoryBits == LADDER) || (filterB.categoryBits == PLAYER && filterA.categoryBits == LADDER))
		{
			if (filterA.categoryBits == PLAYER)
			{
			
				
				player.SetPosition(b2Vec2(2475, -60), true);
				playerJump.currLevel = "leveltwo";
				
				
			}
			else if (filterB.categoryBits == PLAYER)
			{
				
				
				player.SetPosition(b2Vec2(2475, -60), true);
				playerJump.currLevel = "leveltwo"; 
				
				
				
			}
		}
		if ((filterA.categoryBits == PLAYER && filterB.categoryBits == LADDER2) || (filterB.categoryBits == PLAYER && filterA.categoryBits == LADDER2))
		{
			if (filterA.categoryBits == PLAYER)
			{
				
				
				player.SetPosition(b2Vec2(565.f, 100.f), true);
				playerJump.currLevel = "levelone";
				playerJump.enterRight = true;
				

			}
			else if (filterB.categoryBits == PLAYER)
			{
				
				
				player.SetPosition(b2Vec2(565.f, 100.f), true);
				playerJump.currLevel = "levelone";
				playerJump.enterRight = true;
				

			}
		}
		if ((filterA.categoryBits == PLAYER && filterB.categoryBits == LADDER3) || (filterB.categoryBits == PLAYER && filterA.categoryBits == LADDER3))
		{

			if (filterA.categoryBits == PLAYER)
			{
				
				
				player.SetPosition(b2Vec2(960.f, 925.f), true);
				playerJump.currLevel = "toplevel";
				playerJump.enterLeft = true;
				

			}
			else if (filterB.categoryBits == PLAYER)
			{
				
				
				player.SetPosition(b2Vec2(960.f, 925.f), true);
				playerJump.currLevel = "toplevel";
				

			}
		}
		if ((filterA.categoryBits == PLAYER && filterB.categoryBits == LADDER4) || (filterB.categoryBits == PLAYER && filterA.categoryBits == LADDER4))
		{
			if (filterA.categoryBits == PLAYER)
			{

				
				player.SetPosition(b2Vec2(2875.f, 135.f), true);
				playerJump.currLevel = "leveltwo";
				playerJump.enterRight = true;


			}
			else if (filterB.categoryBits == PLAYER)
			{

				
				player.SetPosition(b2Vec2(2875.f, 135.f), true);
				playerJump.currLevel = "leveltwo";
				playerJump.enterRight = true;


			}
		}
		if ((filterA.categoryBits == PLAYER && filterB.categoryBits == LADDER5) || (filterB.categoryBits == PLAYER && filterA.categoryBits == LADDER5))
		{
			if (filterA.categoryBits == PLAYER)
			{

				
				player.SetPosition(b2Vec2(505.f, -40.f), true);
				playerJump.currLevel = "levelone";
				playerJump.enterRight = true;


			}
			else if (filterB.categoryBits == PLAYER)
			{

				
				player.SetPosition(b2Vec2(505.f, -40.f), true);
				playerJump.currLevel = "levelone";
				playerJump.enterRight = true;


			}
		}
		
		

}

void PhysicsPlaygroundListener::EndContact(b2Contact* contact)
{
	b2Fixture* fixtureA = contact->GetFixtureA();
	b2Fixture* fixtureB = contact->GetFixtureB();

	bool sensorA = fixtureA->IsSensor();
	bool sensorB = fixtureB->IsSensor();

	//if neither or both are sensors, will be false
	if ((sensorA ^ sensorB))
	{
		if (sensorA)
		{
			TriggerExit(fixtureA);
		}
		else if (sensorB)
		{
			TriggerExit(fixtureB);
		}
	}
	ECS::GetComponent<CanJump>((int)fixtureB->GetBody()->GetUserData()).m_wallJump = false;
}

void PhysicsPlaygroundListener::TriggerEnter(b2Fixture* sensor)
{
	int entity = (int)sensor->GetBody()->GetUserData();

	ECS::GetComponent<Trigger*>(entity)->OnEnter();
}

void PhysicsPlaygroundListener::TriggerExit(b2Fixture* sensor)
{
	int entity = (int)sensor->GetBody()->GetUserData();

	ECS::GetComponent<Trigger*>(entity)->OnExit();
}
