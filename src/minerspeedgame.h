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
	void maybeDispatchMouseDownEvent();
	void maybeDispatchMouseUpEvent();

	std::vector<position> getStonesToDestroy(const int x, const int y);
	void destroyStone(const position& pos);
	std::vector<position> moreStonesToDestroy();
	void destroyAndFillStones(const std::vector<position>& vect);
	void assignColorToDestroyedStones(const int column, const int row);

	void findStonesSameColorInColumn(std::vector<position> &combos, King::Engine::Texture color, const int column, const int row);
	void findStonesSameColorInRow(std::vector<position> &combos, King::Engine::Texture color, const int column, const int row);
	
	void tryToSwapStones(const int originColumn, const int originRow, const int endColumn, const int endRow);
	void calculateStoneOriginPosition();
	void calculateStoneEndPosition();
	positionF translateMouseToGridPosition() const;
	position MinerSpeedGame::getAndConvertMousePositionToGridIndex();
	
	King::Engine mEngine;
	float mRotation;
	float mStartTime;
	float mTimeLeft;
	bool mFirst;
	position mGridOriginIndex;
};

//**********************************************************************