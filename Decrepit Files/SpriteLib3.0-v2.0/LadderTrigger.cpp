#include "LadderTrigger.h"
#include "ECS.h"

void LadderTrigger::OnTrigger()
{
	Trigger::OnTrigger();
}

void LadderTrigger::OnEnter()
{
	Trigger::OnEnter();
	auto& playerJump = ECS::GetComponent<CanJump>(MainEntities::MainPlayer());
	playerJump.enableLadder = true;
	
}

void LadderTrigger::OnExit()
{
	Trigger::OnExit();
	auto& playerJump = ECS::GetComponent<CanJump>(MainEntities::MainPlayer());
	playerJump.enableLadder = false;
}