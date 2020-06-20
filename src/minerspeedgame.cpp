#include "minerspeedgame.h"

MinerSpeedGame::MinerSpeedGame()
	: mEngine("./assets")
	, mRotation(0.0f)
	, mPosBeginX(0.0f)
	, mPosBeginY(0.0f)
	, mPosEndX(0.0f)
	, mPosEndY(0.0f)
	, mFirst(true)
	, mColumn(-1)
	, mRow(-1)
{}

void MinerSpeedGame::Start() {
	//mEngine->initContollers();
	mEngine.initGame();
	initializeTimer();
	mEngine.Start(*this);
}

void MinerSpeedGame::initializeTimer() {
	mStartTime = mEngine.getCurrentTime();
}

bool MinerSpeedGame::checkTimeOver() {
	mTimeLeft =  MAX_GAME_TIME - (mEngine.getCurrentTime() - mStartTime)* 0.001f;
	if (mTimeLeft < 0.0f) {
		return true;
	}
	return false;
}

void MinerSpeedGame::Update() {
	if (checkTimeOver()) {
		//end game
		mEngine.gameOverScene();
	}
	// fill scene with all elements
	mEngine.fillScene(mTimeLeft);
	// check mouse events
	eventsController();
}

void MinerSpeedGame::eventsController() {
	// mouse down
	mouseDownEvent();
	// mouse up
	mouseUpEvent();
}

void MinerSpeedGame::mouseDownEvent() {
	if (mEngine.GetMouseButtonDown()) {
		if (mFirst) { // first time serves to select stone, then user can drag or just click somewhere else
			// check mouse pos and intersection in the 8x8 grid
			mPosBeginX = mEngine.GetMouseX();
			mPosBeginY = mEngine.GetMouseY();
			// substract initial point
			mPosBeginX -= POS_BEGIN_X;
			mPosBeginY -= POS_BEGIN_Y;
			// FIXME: add variable SELECTED for each stone
			// calculate column and row (FIXME: add epsilon trait for comparison)
			mColumn = -1;
			mRow = -1;
			if (mPosBeginX > 0.0f && mPosBeginY > 0.0f && mPosBeginX < SCENE_SIZE && mPosBeginY < SCENE_SIZE) {
				mColumn = int(mPosBeginX) / STONE_SIZE;
				mRow = int(mPosBeginY) / STONE_SIZE;
			}
			std::cout << "[DEBUG] BEGIN row/column :" << mRow << " - " << mColumn << std::endl;
			mFirst = false;
		} else { 
			// DRAGGING scenario
			// check mouse pos and intersection in the 8x8 grid
			mPosEndX = mEngine.GetMouseX();
			mPosEndY = mEngine.GetMouseY();
			// substract initial point
			mPosEndX -= POS_BEGIN_X;
			mPosEndY -= POS_BEGIN_Y;
			// calculate column and row (FIXME: add epsilon trait for comparison)
			int column = -1;
			int row = -1;
			if (mPosEndX > 0.000f && mPosEndY > 0.000f && mPosEndX < SCENE_SIZE && mPosEndY < SCENE_SIZE) {
				column = int(mPosEndX) / STONE_SIZE;
				row = int(mPosEndY) / STONE_SIZE;
			}
			// check if swap is possible
			std::cout << "[DEBUG] END row/column :" << row << " - " << column << std::endl;
			if (row >-1 && column > -1 && mColumn > -1 && mRow > -1) {
				//verifySwap(row, column);
				swap(row, column);
			}
		}
	}
}

void MinerSpeedGame::mouseUpEvent() {
	if (mEngine.GetMouseButtonUp()) {
		mFirst = true;
		mEngine.SetMouseButtonUp(false);
		mPosEndX = mEngine.GetMouseX();
		mPosEndY = mEngine.GetMouseY();
		// check swap of stones if possible
		// ...
	}
}

void MinerSpeedGame::swap(const int row, const int column) {
	// only allow row and column-wise moves
	std::vector<std::vector<position>*> *combinations = new std::vector<std::vector<position>*>();
	int swaped = false;
	if (row >= 0 && abs(mRow - row) == 1) {
		swaped = true;
		mEngine.swapStoneColor(mRow, mColumn, 0, row - mRow);
	}
	else if (column >= 0 && abs(mColumn - column) == 1) {
		// if condition to do swap (3+ stones same color) then do swap
		swaped = true;
		mEngine.swapStoneColor(mRow, mColumn, column - mColumn, 0);
	}

	if (swaped) {
		// just allow one swap
		mEngine.SetMouseButtonDown(false);
		// first be sure that you cant swap same colors;
		if (mEngine.getStoneColors()[mRow][mColumn] == mEngine.getStoneColors()[row][column]) {
			return;
		}
		std::vector<position> destroyedStones1,destroyedStones2, destroyedNewStones;
		destroyedStones1 = destroyStones(mRow, mColumn);
		destroyedStones2 = destroyStones(row, column);
		destroyedNewStones = std::vector<position>();
		bool ready = false;
		while (!ready) {
			if (destroyedStones1.size() != 0 || destroyedStones2.size() !=0 || destroyedNewStones.size() != 0) {
				fillDestroyedStones(destroyedStones1);
				fillDestroyedStones(destroyedStones2);
				fillDestroyedStones(destroyedNewStones);
				destroyedStones1.clear();
				destroyedStones2.clear();
				destroyedNewStones.clear();
			} else { // put them back
				mEngine.swapStoneColor(mRow, mColumn, column - mColumn, row - mRow);
				ready = true;
			}
			bool destroy = false;
			for (int y = 0; y < GAME_GRID_SIZE; ++y) {
				for (int x = 0; x < GAME_GRID_SIZE; ++x) {
					destroyedNewStones = destroyStones(y, x);
					if (destroyedNewStones.size() != 0) {
						destroy = true;
						break;
					}
				}
				if (destroy) {
					break;
				}
			}
			if (!destroy) {
				ready = true;
			}
		}
	}
}

std::vector<position> MinerSpeedGame::destroyStones(const int row, const int column) {

	std::vector<position> resultColumn, resultRow;
	resultColumn.reserve(12);//max we destroy entire row(8) and half column(4) or the opposite, for a totale of 12
	resultRow.reserve(12);

	King::Engine::Texture currentColor = mEngine.getStoneColors()[row][column];
	// check left in the same column and keep the row
	for (int leftX = column - 1; leftX >= 0; leftX--) {
		if(currentColor == mEngine.getStoneColors()[row][leftX]){
			resultColumn.push_back(position(row, leftX));
		}
		else {
			break;
		}
	}

	// check right in the same column and keep the row
	for (int rightX = column + 1; rightX < GAME_GRID_SIZE; rightX++) {
		if (currentColor == mEngine.getStoneColors()[row][rightX]) {
			resultColumn.push_back(position(row, rightX));
		}
		else {
			break;
		}
	}

	if (resultColumn.size() < 2) { // nothing happens
		resultColumn.clear();
	}

	// check down in the same row and keep the column
	for (int rowY = row + 1; rowY < GAME_GRID_SIZE; rowY++) {
		if (currentColor == mEngine.getStoneColors()[rowY][column]) {
			resultRow.push_back(position(rowY, column));
		}
		else {
			break;
		}
	}

	// check up in the same row and keep the column
	for (int rowY = row - 1; rowY >= 0; rowY--) {
		if (currentColor == mEngine.getStoneColors()[rowY][column]) {
			resultRow.push_back(position(rowY, column));
		}
		else {
			break;
		}
	}

	if (resultRow.size() < 2) { // nothing happens
		resultRow.clear();
	}

	if (resultRow.size() == 0 && resultColumn.size() == 0) { // nothing happens in either cases
		return std::vector<position>();
	}

	if (resultRow.size() == 0) {
		resultColumn.push_back(position(row, column));
		return resultColumn;
	}

	if (resultColumn.size() == 0) {
		resultRow.push_back(position(row, column));
		return resultRow;
	}

	// we get both row and column match
	std::vector<position> resultBoth;
	resultBoth.push_back(position(row,column));
	resultBoth.insert(resultBoth.end(), resultRow.begin(), resultRow.end());
	resultBoth.insert(resultBoth.end(), resultColumn.begin(), resultColumn.end());
	return resultBoth;
}

void MinerSpeedGame::fillDestroyedStones(const std::vector<position> &vect) {
	// FIXME: not working properly, weird stone adding
	// first empty every all destroyed stones
	// FIXME: try to add animation somewhere
	//bool animation = false;
	for (auto nextPos : vect) {
		mEngine.setStoneColor(nextPos.first, nextPos.second, King::Engine::Texture::TEXTURE_EMPTY);
	}
	for (auto nextPos : vect) {
		// getRandomStone()
		fixEmptyStone(nextPos.first, nextPos.second);
	}
}

bool MinerSpeedGame::fixEmptyStone(const int row, const int column) {
	// FIXME: not working properly, weird stone adding
	// nothing above so generate a new one
	if (row == 0) {
		// FIXME: animation here???
		//mEngine.setStoneColor(row, column, King::Engine::Texture::TEXTURE_EMPTY/*getRandomStone();
		mEngine.setStoneColor(row, column, King::Engine::getRandomStone());
		return false;
	}

	// good cell, check above
	if (mEngine.getStoneColors()[row][column] != King::Engine::Texture::TEXTURE_EMPTY) {
		fixEmptyStone(row - 1, column);
		return false; 
	}

	// try to find the above stone that is not empty
	bool found = false;
	for (int rowY = row - 1; rowY >= 0; rowY--) {
		if (mEngine.getStoneColors()[rowY][column] != King::Engine::Texture::TEXTURE_EMPTY) {
			// assign current color from the above one
			mEngine.setStoneColor(row, column, mEngine.getStoneColors()[rowY][column]);
			mEngine.setStoneColor(rowY, column, King::Engine::Texture::TEXTURE_EMPTY);
			found = true;
			break;
		}
	}
	if (found == false) {
		// FIXME: animation here???
		//mEngine.setStoneColor(row, column, King::Engine::Texture::TEXTURE_EMPTY/*(King::Engine::Texture)(getRandomStone()*/);
		mEngine.setStoneColor(row, column, King::Engine::getRandomStone());
	}
	fixEmptyStone(row - 1, column);
	return false;
}

// FIXME: rotation just in case
//const char text[] = "This rotates at 5/PI Hz";
//mRotation += mEngine.GetLastFrameSeconds();
//mEngine.Write(text, mEngine.GetWidth() / 2.0f, mEngine.GetHeight() / 2.0f, mRotation * 2.5f);