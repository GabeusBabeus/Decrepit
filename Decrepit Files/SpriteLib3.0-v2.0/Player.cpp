#include "Player.h"

Player::Player()
{
}

Player::Player(std::string& fileName, std::string& animationJSON, int width, int height, Sprite* sprite, 
					AnimationController* controller, Transform* transform, bool hasPhys, PhysicsBody* body)
{
	InitPlayer(fileName, animationJSON, width, height, sprite, controller, transform, hasPhys, body);
}

void Player::InitPlayer(std::string& fileName, std::string& animationJSON, int width, int height, Sprite* sprite, 
							AnimationController* controller, Transform* transform, bool hasPhys, PhysicsBody* body)
{
	//Store references to the components
	m_sprite = sprite;
	m_animController = controller;
	m_transform = transform;
	m_hasPhysics = hasPhys;

	if (hasPhys)
	{
		m_physBody = body;
	}

	//Initialize UVs
	m_animController->InitUVs(fileName);

	//Loads the texture and sets width and height
	m_sprite->LoadSprite(fileName, width, height, true, m_animController);
	m_animController->SetVAO(m_sprite->GetVAO());
	m_animController->SetTextureSize(m_sprite->GetTextureSize());

	//Loads in the animations json file
	nlohmann::json animations = File::LoadJSON(animationJSON);

#ifdef TOPDOWN
	//Idle Up
	m_animController->AddAnimation(animations["IdleUp"].get<Animation>());
	//Idle Down
	m_animController->AddAnimation(animations["IdleDown"].get<Animation>());
#endif
	
#ifdef TOPDOWN
	//WalkUP
	m_animController->AddAnimation(animations["WalkUp"].get<Animation>());
	//WalkDown
	m_animController->AddAnimation(animations["WalkDown"].get<Animation>());
#endif
	
#ifdef TOPDOWN
	//AttackUp
	m_animController->AddAnimation(animations["AttackUp"].get<Animation>());
	//AttackDown
	m_animController->AddAnimation(animations["AttackDown"].get<Animation>());
#endif

	//Jump Animations\\

	


	//Set Default Animation
	m_animController->SetActiveAnim(IDLERIGHT);


}

void Player::Update()
{
	auto& canJump = ECS::GetComponent<CanJump>(MainEntities::MainPlayer());
	auto& animController = ECS::GetComponent<AnimationController>(90);

	m_moving = false;
	
	

	if (m_jumping && animController.GetAnimation(6).GetAnimationDone()) 
	{
		animController.GetAnimation(6).Reset();
		m_jumping = false;
		m_locked = false;
	}

	if (m_jumping && animController.GetAnimation(7).GetAnimationDone())
	{
		animController.GetAnimation(7).Reset();
		m_jumping = false;
		m_locked = false;
	}

	if (canJump.m_canJump)
	{
		m_jumping = false;
	}

	

	if (!m_locked) {
		MovementUpdate();
	}

}

void Player::MovementUpdate()
{
	auto& animController = ECS::GetComponent<AnimationController>(90);
	auto& canJump = ECS::GetComponent<CanJump>(MainEntities::MainPlayer());

	m_moving = false;
	m_hasPhysics = false;

	if (m_hasPhysics)
	{
		float speed = 10.f;
		vec3 vel = vec3(0.f, 0.f, 0.f);

		if (Input::GetKey(Key::Shift))
		{
			speed *= 7.f;
		}

#ifdef TOPDOWN
		if (Input::GetKey(Key::W))
		{
			vel = vel + vec3(0.f, 1.f, 0.f);
			m_facing = UP;
			m_moving = true;
		}
		if (Input::GetKey(Key::S))
		{
			vel = vel + vec3(0.f, -1.f, 0.f);
			m_facing = DOWN;
			m_moving = true;
		}
#endif

		if (Input::GetKey(Key::A))
		{

			m_facing = LEFT;
			m_moving = true;
		}
		if (Input::GetKey(Key::D))
		{

			m_facing = RIGHT;
			m_moving = true;
		}

		m_physBody->SetVelocity(vel * speed);
	}
	else
	{
		//Regular Movement
		float speed = 15.f;

		
		if (!m_jumping) {
			
			if (!m_locked) {
				if (Input::GetKey(Key::A))
				{
					m_facing = LEFT;
					m_moving = true;
					animController.SetActiveAnim(2);
				}
				else if (Input::GetKey(Key::D))
				{
					m_facing = RIGHT;
					m_moving = true;
					animController.SetActiveAnim(3);

				}
				else if (Input::GetKeyDown(Key::Space) && canJump.m_canJump)
				{
					
					if (m_facing == RIGHT)
					{
						animController.SetActiveAnim(7);
						m_jumping = true;
					}
					else if (m_facing == LEFT)
					{
						animController.SetActiveAnim(6);
						m_jumping = true;
					}

					m_locked = true;
				}
				else if (m_moving == false)
				{
					if (m_facing == RIGHT) {
						animController.SetActiveAnim(1);
					}
					else if (m_facing == LEFT)
					{
						animController.SetActiveAnim(0);
					}
				}
			}
		}



	}

	


}



void Player::SetActiveAnimation(int anim)
{
	m_animController->SetActiveAnim(anim);
}
