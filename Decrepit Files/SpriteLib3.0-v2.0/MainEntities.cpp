#include "MainEntities.h"

int MainEntities::m_mainCamera = 0;
int MainEntities::m_mainPlayer = 0;
int MainEntities::m_keyOne = 0;
int MainEntities::m_doorBasement = 0;
int MainEntities::m_switchOne = 0;
int MainEntities::m_switchTwo = 0;
int MainEntities::m_bridgeOne = 0;
int MainEntities::m_bridgeTwo = 0;
//int MainEntities::m_keyTwo = 0;

int MainEntities::MainCamera()
{
	return m_mainCamera;
}

int MainEntities::MainPlayer()
{
	return m_mainPlayer;
}
int MainEntities::doorBasement()
{
	return m_doorBasement;
}

int MainEntities::bridgeOne()
{
	return m_bridgeOne;
}

int MainEntities::bridgeTwo()
{
	return m_bridgeTwo;
}

int MainEntities::keyOne()
{
	return m_keyOne;
}

int MainEntities::switchOne()
{
	return m_switchOne;
}

int MainEntities::switchTwo()
{
	return m_switchTwo;
}

//int MainEntities::keyTwo()
//{
//	return m_keyTwo;
//}

void MainEntities::MainCamera(int main)
{
	m_mainCamera = main;
}

void MainEntities::MainPlayer(int main)
{
	m_mainPlayer = main;
}
void MainEntities::doorBasement(int main)
{
	m_doorBasement = main;
}

void MainEntities::bridgeOne(int main)
{
	m_bridgeOne = main;
}

void MainEntities::bridgeTwo(int main)
{
	m_bridgeTwo = main;
}

void MainEntities::keyOne(int main)
{
	m_keyOne = main;
}

void MainEntities::switchOne(int main) 
{
	m_switchOne = main;
}

void MainEntities::switchTwo(int main)
{
	m_switchTwo = main;
}

//void MainEntities::keyTwo(int main)
//{
//	m_keyTwo = main;
//}
