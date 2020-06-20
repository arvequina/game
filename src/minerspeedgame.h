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
	void maybeDispatchMouseEvents();
	void mouseDownEvent();
	void mouseUpEvent();
	std::vector<position> getStonesToDestroy(const int x, const int y);
	void findStonesSameColorInColumn(std::vector<position> &combos, King::Engine::Texture color, const int row, const int column);
	void findStonesSameColorInRow(std::vector<position> &combos, King::Engine::Texture color, const int row, const int column);
	void swap(const int row, const int column);
	void destroyAndFillStones(const std::vector<position>& vect);
	void assignColorToDestroyedStones(const int row, const int column);
	void destroyStone(const position& pos);
	std::vector<position> moreStonesToDestroy();
	void initializeTimer();
	bool isTimeOver();
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