#define GLM_FORCE_RADIANS 

#include <iostream>
#include <list>
#include <algorithm>
#include <vector>

#include "king/Engine.h"
#include "king/Updater.h"

typedef std::vector<position<int>> vectorOfPositions;
typedef std::pair<King::Engine::ActionsFromGestures, King::Engine::ActionsFromGestures> pairOfActions;

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

	vectorOfPositions getStonesToDestroy(const int x, const int y);
	void destroyStone(const position<int> &pos);
	vectorOfPositions moreStonesToDestroy();
	void destroyAndFillStones(const vectorOfPositions &vect);
	void assignColorToDestroyedStones(const int column, const int row);

	void findStonesSameColorInColumn(vectorOfPositions &combos, King::Engine::Texture color, const int column, const int row);
	void findStonesSameColorInRow(vectorOfPositions &combos, King::Engine::Texture color, const int column, const int row);
	
	void tryToSwapStones(const int originColumn, const int originRow, const int endColumn, const int endRow);
	void calculateStoneOriginPosition();
	void calculateStoneEndPosition();
	position<float> translateMouseToGridPosition() const;
	position<int> MinerSpeedGame::getAndConvertMousePositionToGridIndex();
	pairOfActions createVerticalStoneMoveAction(bool upDirection);
	pairOfActions createHorizontalStoneMoveAction(bool leftDirection);
	
	King::Engine mEngine;
	float mRotation;
	float mStartTime;
	float mTimeLeft;
	bool mFirst;
	position<int> mGridOriginIndex;
};

//**********************************************************************