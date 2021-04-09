#pragma once
#include <string>
#include <Box2D/Common/b2Math.h>
class CanJump
{
public:
	bool m_canJump = true;
	bool m_wallJump = false;
	int m_wallJumpNum = 1;
	bool m_facingRight = true;
	bool enableLadder = false;
	std::string currLevel = "levelone";
	bool enterLeft = true;
	bool enterRight = false;
	b2Vec2 leveloneSpawn = b2Vec2(-100, -60);
	b2Vec2 leveltwoSpawn = b2Vec2(1400, 20);

	bool inForest = true;

	void setEnterLeft(bool t) {
		enterLeft = t;
	}
private:
};

