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
	mEngine.initGame();
	mEngine.Start(*this);
}

void MinerSpeedGame::Update() {
	if (mEngine.isTimeOver()) {
		//end game
		mEngine.gameOverScene();
		return;
	}
	// fill scene with all elements
	mEngine.fillScene();
	maybeDispatchMouseEvents();
}

void MinerSpeedGame::maybeDispatchMouseEvents() {
	mouseDownEvent();
	mouseUpEvent();
}

void MinerSpeedGame::mouseDownEvent() {
	if (!mEngine.GetMouseButtonDown()) {
		return;
	}
	if (mFirst) { // first time serves to select stone, then user can drag or just click somewhere else
		// check mouse pos and intersection in the 8x8 grid and substract initial point
		mPosBeginX = mEngine.GetMouseX() - POS_BEGIN_X;
		mPosBeginY = mEngine.GetMouseY() - POS_BEGIN_Y;
		// calculate column and row (FIXME: add epsilon trait for comparison)
		mColumn = -1; mRow = -1;
		if (mPosBeginX > 0.0f && mPosBeginY > 0.0f && mPosBeginX < SCENE_SIZE_X && mPosBeginY < SCENE_SIZE_Y) {
			mColumn = static_cast<int>(mPosBeginX) / STONE_SIZE_X;
			mRow = static_cast<int>(mPosBeginY) / STONE_SIZE_Y;
		}
		mFirst = false;
	} else { 
		// DRAGGING scenario
		// check mouse pos and intersection in the 8x8 grid and substract initial point
		mPosEndX = mEngine.GetMouseX() - POS_BEGIN_X;
		mPosEndY = mEngine.GetMouseY() - POS_BEGIN_Y;
		// calculate column and row
		int column = -1, row = -1;
		if (mPosEndX > std::numeric_limits<float>::epsilon() && mPosEndY > std::numeric_limits<float>::epsilon() &&
		    mPosEndX < SCENE_SIZE_X && mPosEndY < SCENE_SIZE_Y) {
			column = static_cast<int>(mPosEndX) / STONE_SIZE_X;
			row = static_cast<int>(mPosEndY) / STONE_SIZE_Y;
		}
		// check if swap is possible
		if (row >-1 && column > -1 && mColumn > -1 && mRow > -1) {
			// FIXME: not working properly
			swap(column, row);
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

void MinerSpeedGame::swap(const int column, const int row) {
	// only allow row and column-wise moves
	int swaped = false;
	if (row >= 0 && abs(mRow - row) == 1) {
		swaped = true;
		mEngine.swapStoneColor(mColumn, mRow, 0, row - mRow);
	} else if (column >= 0 && abs(mColumn - column) == 1) {
		// if condition to do swap (3+ stones same color) then do swap
		swaped = true;
		mEngine.swapStoneColor(mColumn, mRow, column - mColumn, 0);
	}

	if (swaped) {
		// just allow one swap
		mEngine.SetMouseButtonDown(false);
		// not allowed to swap same color stones
		if (mEngine.getStoneColors()[mColumn][mRow] == mEngine.getStoneColors()[column][row]) {
			return;
		}
		std::vector<position> destroyStonesOrigin, destroyStonesEnd;
		destroyStonesOrigin = getStonesToDestroy(mColumn, mRow);
		destroyStonesEnd    = getStonesToDestroy(column, row);
		if (destroyStonesOrigin.size() != 0 || destroyStonesEnd.size() !=0) {
			destroyAndFillStones(destroyStonesOrigin);
			destroyAndFillStones(destroyStonesEnd);
		} else { // put stones to original location if swap not possible
			mEngine.swapStoneColor(mColumn, mRow, column - mColumn, row - mRow);
		}
		bool moreStones = true;
		while (moreStones) {
			std::vector<position> stonesToDestroy = moreStonesToDestroy();
			destroyAndFillStones(stonesToDestroy);
			if (stonesToDestroy.empty()) {
				moreStones = false;
			}
		}
	}
}

std::vector<position> MinerSpeedGame::getStonesToDestroy(const int column, const int row) {
	std::vector<position> stonesColumn, stonesRow;
	// max we can destroy is a entire row (8 elements) and half column (4 elements) or the opposite -> total of 12
	stonesColumn.reserve(12);
	stonesRow.reserve(12);

	King::Engine::Texture currentColor = mEngine.getStoneColors()[column][row];
	findStonesSameColorInColumn(stonesColumn, currentColor, column, row);
	findStonesSameColorInRow(stonesRow, currentColor, column, row);
	// nothing to do in these cases
	if (stonesRow.size() == 0 && stonesColumn.size() == 0) {
		return std::vector<position>();
	}
	// add initial point if other direction is empty
	if (stonesRow.size() == 0) {
		stonesColumn.push_back(position(column, row));
		return stonesColumn;
	}
	// add initial point if other direction is empty
	if (stonesColumn.size() == 0) {
		stonesRow.push_back(position(column, row));
		return stonesRow;
	}
	// we get both row and column combinations of stones
	std::vector<position> resultBoth;
	resultBoth.push_back(position(column, row));
	resultBoth.insert(resultBoth.end(), stonesRow.begin(), stonesRow.end());
	resultBoth.insert(resultBoth.end(), stonesColumn.begin(), stonesColumn.end());
	return resultBoth;
}

void MinerSpeedGame::findStonesSameColorInColumn(std::vector<position> &stones, King::Engine::Texture color, const int column, const int row) {
	// iterate left in the same column and keep the row
	for (int currentColumn = column - 1; currentColumn >= 0; --currentColumn) {
		if (color == mEngine.getStoneColors()[currentColumn][row]) {
			stones.push_back(position(currentColumn, row));
		} else {
			break;
		}
	}
	// iterate right in the same column and keep the row
	for (int currentColumn = column + 1; currentColumn < GAME_GRID_SIZE_X; ++currentColumn) {
		if (color == mEngine.getStoneColors()[currentColumn][row]) {
			stones.push_back(position(currentColumn, row));
		} else {
			break;
		}
	}
	// nothing to do if not found at least 2 stones of same color
	if (stones.size() < 2) {
		stones.clear();
	}
}

void MinerSpeedGame::findStonesSameColorInRow(std::vector<position> &stones, King::Engine::Texture color, const int column, const int row) {
	// iterate down in the same row and keep the column
	for (int currentRow = row + 1; currentRow < GAME_GRID_SIZE_Y; ++currentRow) {
		if (color == mEngine.getStoneColors()[column][currentRow]) {
			stones.push_back(position(column, currentRow));
		} else {
			break;
		}
	}
	// iterate up in the same row and keep the column
	for (int currentRow = row - 1; currentRow >= 0; --currentRow) {
		if (color == mEngine.getStoneColors()[column][currentRow]) {
			stones.push_back(position(column, currentRow));
		} else {
			break;
		}
	}
	// nothing to do if not found at least 2 stones of same color
	if (stones.size() < 2) {
		stones.clear();
	}
}

void MinerSpeedGame::destroyAndFillStones(const std::vector<position> &vect) {
	// first destroy all stones to be destroyed
	for (auto nextPos : vect) {
		// FIXME: try to add animation here somehow
		destroyStone(nextPos);	
	}
	for (auto nextPos : vect) {
		assignColorToDestroyedStones(nextPos.first, nextPos.second);
	}
}

void MinerSpeedGame::destroyStone(const position &pos) {
	mEngine.setStoneColor(pos.first, pos.second, King::Engine::Texture::TEXTURE_EMPTY);
}

void MinerSpeedGame::assignColorToDestroyedStones(const int column, const int row) {
	// last row so no more rows left -> generate a new one
	if (row == 0) {
		mEngine.setStoneColor(column, row, King::Engine::getRandomStoneColor());
		return;
	}
	// if cell already has a color -> check above
	if (mEngine.getStoneColors()[column][row] != King::Engine::Texture::TEXTURE_EMPTY) {
		assignColorToDestroyedStones(column, row - 1);
		return; 
	}
	// find stone above that is not empty
	bool found = false;
	for (int nextRow = row - 1; nextRow >= 0; --nextRow) {
		if (mEngine.getStoneColors()[column][nextRow] != King::Engine::Texture::TEXTURE_EMPTY) {
			// assign current color from the stone above
			mEngine.setStoneColor(column, row, mEngine.getStoneColors()[column][nextRow]);
			mEngine.setStoneColor(column, nextRow, King::Engine::Texture::TEXTURE_EMPTY);
			found = true;
			break;
		}
	}
	if (found == false) {
		mEngine.setStoneColor(column, row, King::Engine::getRandomStoneColor());
	}
	assignColorToDestroyedStones(column, row - 1);
	return;
}

std::vector<position> MinerSpeedGame::moreStonesToDestroy() {
	// we need to check again for possible combinations as the grid has been modified
	std::vector<position> stonesToDestroy = std::vector<position>();
	bool exitLoop = false;
	for (int y = 0; y < GAME_GRID_SIZE_Y; ++y) {
		for (int x = 0; x < GAME_GRID_SIZE_X; ++x) {
			stonesToDestroy = getStonesToDestroy(y, x);
			if (!stonesToDestroy.empty()) {
				exitLoop = true;
				break;
			}
		}
		if (exitLoop) {
			break;
		}
	}
	return stonesToDestroy;
}

// FIXME: rotation just in case
//const char text[] = "This rotates at 5/PI Hz";
//mRotation += mEngine.GetLastFrameSeconds();
//mEngine.Write(text, mEngine.GetWidth() / 2.0f, mEngine.GetHeight() / 2.0f, mRotation * 2.5f);