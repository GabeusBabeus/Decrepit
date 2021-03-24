#include "LadderTeleport.h"
#include "ECS.h"

void LadderTeleport::OnTrigger()
{
	Trigger::OnTrigger();
}

void LadderTeleport::OnEnter()
{
	Trigger::OnEnter();
	auto& player = ECS::GetComponent<Transform>(MainEntities::MainPlayer());
	player.SetPosition(vec3(0.f,0.f,2.f));

}

void LadderTeleport::OnExit()
{
	Trigger::OnExit();
	
}