#ifndef __MAINENTITIES_H__
#define __MAINENTITIES_H__

class MainEntities
{
public:
	//Getters
	static int MainCamera();
	static int MainPlayer();
	static int doorBasement();
	static int keyOne();
	//static int keyTwo();
	//Setters
	static void MainCamera(int main);
	static void MainPlayer(int main);
	static void doorBasement(int main);
	static void keyOne(int main);
	//static void keyTwo(int main);
private:
	static int m_mainCamera;
	static int m_mainPlayer;
	static int m_doorBasement;
	static int m_keyOne;
	//static int m_keyTwo;
};

#endif // !__MAINENTITIES_H__
