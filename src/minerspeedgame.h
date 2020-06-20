#define GLM_FORCE_RADIANS 

#include <king/Engine.h>
#include <king/Updater.h>


#include "iostream"
#include "list"
#include <algorithm>
#include <vector>

typedef std::pair<int, int> position;

//**********************************************************************

#define maxGameTime 600.0f
class MinerSpeedGame : public King::Updater {
public:

	MinerSpeedGame();
	// destructor
	void Start();
	void Update();

private:
	void eventsController();
	void mouseDownEvent();
	void mouseUpEvent();
	void verifySwap(const int row, const int column);
	std::vector<std::pair<int, int>> destroyStones(const int x, const int y);
	void swap(const int row, const int column);
	std::vector<position>* scanPosition(const int row, const int column);
	std::vector<std::vector<position>*> * scan();
	void fillDestroyedStones(const std::vector<std::pair<int, int>>& vect);
	bool fixEmptyStone(const int row, const int column);
	bool verifyStoneCombinations(const int row, const int column);
	void initializeTimer();
	bool checkTimeOver();
	King::Engine mEngine;
	float mRotation;
	float mPosBeginX;
	float mPosBeginY;
	float mPosEndX;
	float mPosEndY;
	float mStartTime;
	float mTimeLeft;
	bool mFirst;
	int mColumn;
	int mRow;
};

//**********************************************************************