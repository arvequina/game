#define GLM_FORCE_RADIANS 

#include <iostream>
#include <list>
#include <algorithm>
#include <vector>

#include "king/Engine.h"
#include "king/Updater.h"

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
	void findStonesSameColorInColumn(std::vector<position> &combos, King::Engine::Texture color, const int column, const int row);
	void findStonesSameColorInRow(std::vector<position> &combos, King::Engine::Texture color, const int column, const int row);
	void swap(const int column, const int row);
	void destroyAndFillStones(const std::vector<position>& vect);
	void assignColorToDestroyedStones(const int column, const int row);
	void destroyStone(const position& pos);
	std::vector<position> moreStonesToDestroy();
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