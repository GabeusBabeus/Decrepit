#include "MainEntities.h"

int MainEntities::m_mainCamera = 0;
int MainEntities::m_mainPlayer = 0;
int MainEntities::m_keyOne = 0;
int MainEntities::m_doorBasement = 0;
int MainEntities::m_pointer = 0;
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

int MainEntities::keyOne()
{
	return m_keyOne;
}

int MainEntities::pointer()
{
	return m_pointer;
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

void MainEntities::keyOne(int main)
{
	m_keyOne = main;
}

void MainEntities::pointer(int main)
{
	m_pointer = main;
}


//void MainEntities::keyTwo(int main)
//{
//	m_keyTwo = main;
//}
