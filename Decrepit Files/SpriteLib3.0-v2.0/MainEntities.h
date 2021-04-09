#ifndef __MAINENTITIES_H__
#define __MAINENTITIES_H__

class MainEntities
{
public:
	//Getters
	static int MainCamera();
	static int MainPlayer();
	static int doorBasement();
	static int bridgeOne();
	static int bridgeTwo();
	static int keyOne();
	static int switchOne();
	static int switchTwo();
	static int MainMenu();

	static int keyTwo();
	//static int scrollOne();
	//static int scrollTwo();
	//static int scrollThree();

	//Setters
	static void MainCamera(int main);
	static void MainPlayer(int main);
	static void doorBasement(int main);
	static void bridgeOne(int main);
	static void bridgeTwo(int main);
	static void keyOne(int main);
	static void keyTwo(int main);
	static void switchOne(int main);
	static void switchTwo(int main);
	static void MainMenu(int main);
private:
	static int m_mainCamera;
	static int m_mainPlayer;
	static int m_doorBasement;
	static int m_keyOne;
	static int m_keyTwo;
	static int m_switchOne;
	static int m_switchTwo;
	static int m_bridgeOne;
	static int m_bridgeTwo;
	static int m_MainMenu;
};

#endif // !__MAINENTITIES_H__
