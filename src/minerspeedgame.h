#ifndef MINERSPEED_HEADER
#define MINERSPEED_HEADER

#include <iostream>
#include <list>
#include <algorithm>
#include <vector>

#include "king/Engine.h"
#include "king/Updater.h"
#include "common.h"
#include "animation.h"

//**********************************************************************
class MinerSpeedGame : public King::Updater {
public:
	MinerSpeedGame();
	~MinerSpeedGame();
	void Initialize();
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

	void findStonesSameColorInColumn(vectorOfPositions &combos, const Texture &color, const int column, const int row);
	void findStonesSameColorInRow(vectorOfPositions &combos, const Texture &color, const int column, const int row);
	
	void tryToSwapStones(const position<int> originIndex, const position<int> endIndex);
	void swap(const position<int> &originIndex, const position<int> &endIndex, pairOfActions &stoneMoveAction);
	void calculateStoneOriginPosition();
	void calculateStoneEndPosition();
	position<int> calculateMouseGridPosition();
	position<float> translateMouseToGridPosition() const;
	position<int> MinerSpeedGame::getAndConvertMousePositionToGridIndex();
	pairOfActions createVerticalStoneMoveAction(bool upDirection);
	pairOfActions createHorizontalStoneMoveAction(bool leftDirection);
	
	King::Engine mEngine;
	float mRotation;
	float mStartTime;
	float mTimeLeft;
	bool mFirst;
	bool mStoneSelected;
	position<int> mGridOriginIndex;
	AnimationManager *mAnimationManager;
};

#endif // MINERSPEED_HEADER