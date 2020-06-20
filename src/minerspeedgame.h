#define GLM_FORCE_RADIANS 

#include <king/Engine.h>
#include <king/Updater.h>


#include "iostream"
#include "list"
#include <algorithm>
#include <vector>

typedef std::pair<int, int> position;

//**********************************************************************
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
	std::vector<std::pair<int, int>> destroyStones(const int x, const int y);
	void swap(const int row, const int column);
	void fillDestroyedStones(const std::vector<position>& vect);
	bool fixEmptyStone(const int row, const int column);
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