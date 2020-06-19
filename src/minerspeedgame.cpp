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
	mEngine.Start(*this);
}

void MinerSpeedGame::Update() {
	// create structure 8x8 with rendering info
	mEngine.fillScene();
	// check mouse events
	VerifyMouseEvents();
}

void MinerSpeedGame::VerifyMouseEvents() {
	// mouse down
	mouseDownEvent(mEngine.GetMouseButtonDown());
	// mouse up
	mouseUpEvent(mEngine.GetMouseButtonUp());
}

void MinerSpeedGame::mouseDownEvent(const bool mouseDown) {
	if (mouseDown) {
		if (mFirst) { // first time serves to select stone, then user can drag or just click somewhere else
			// check mouse pos and intersection in the 8x8 grid
			mPosBeginX = mEngine.GetMouseX();
			mPosBeginY = mEngine.GetMouseY();
			// substract initial point
			mPosBeginX -= 330.0f;
			mPosBeginY -= 100.0f;
			const float pos_increment = 43.0f;
			// FIXME: add variable SELECTED for each stone
			// calculate column and row (FIXME: add epsilon trait for comparison)
			mColumn = -1;
			mRow = -1;
			if (mPosBeginX > 0.0f && mPosBeginY > 0.0f && mPosBeginX < 8 * pos_increment && mPosBeginY < 8 * pos_increment) {
				mColumn = int(mPosBeginX) / pos_increment;
				mRow = int(mPosBeginY) / pos_increment;
			}
			std::cout << "first row/colm" << mRow << mColumn;
			mFirst = false;
		} else { 
			// DRAGGING scenario
			// check mouse pos and intersection in the 8x8 grid
			mPosEndX = mEngine.GetMouseX();
			mPosEndY = mEngine.GetMouseY();
			// substract initial point
			mPosEndX -= 330.0f;
			mPosEndY -= 100.0f;
			const float pos_increment = 43.0f;
			// calculate column and row (FIXME: add epsilon trait for comparison)
			int column = -1;
			int row = -1;
			if (mPosEndX > 0.000f && mPosEndY > 0.000f && mPosEndX < 8 * pos_increment && mPosEndY < 8 * pos_increment) {
				column = int(mPosEndX) / pos_increment;
				row = int(mPosEndY) / pos_increment;
			}
			// check if swap is possible
			std::cout << "second row/colm" << row << column;
			if (row >-1 && column>-1 && mColumn > -1 && mRow > -1) {
				//verifySwap(row, column);
				swap(row, column);
			}
			mFirst = true;
		}
	}
}

void MinerSpeedGame::mouseUpEvent(const bool mouseUp) {
	if (mouseUp) {
		//mFirst = true;
		mEngine.SetMouseButtonUp(false);
		mPosEndX = mEngine.GetMouseX();
		mPosEndY = mEngine.GetMouseY();
		// check swap of stones if possible
		// ...
	}
}

void MinerSpeedGame::verifySwap(const int row, const int column) {
	// only allow row and column-wise moves
	if (row >= 0 && abs(mRow - row) == 1) {
		// if condition to do swap (3+ stones same color) then do swap
		mEngine.swapStoneColor(mRow, mColumn, 0, row - mRow);
		bool allowed = verifyStoneCombinations(row, column);
		if (!allowed) {
		// revert swap
			mEngine.swapStoneColor(mRow, mColumn, 0, row - mRow);
		}
		// just allow one swap
		mEngine.SetMouseButtonDown(false);
	}
	else if (column >= 0 && abs(mColumn - column) == 1) {
		// if condition to do swap (3+ stones same color) then do swap
		mEngine.swapStoneColor(mRow, mColumn, column - mColumn, 0);
		bool allowed = verifyStoneCombinations(row, column);
		if (!allowed) {
			// revert swap
			mEngine.swapStoneColor(mRow, mColumn, column - mColumn, 0);
		}
		// just allow one swap
		mEngine.SetMouseButtonDown(false);
	}
}

std::vector<std::vector<position>*> *MinerSpeedGame::scan() {
	std::vector<position> *stones = nullptr;
	std::vector<std::vector<position>*> *combinations = new std::vector<std::vector<position>*>();
	for (int y = 0; y < 8; ++y) {
		for (int x = 0; x < 8; ++x) {
			stones = scanPosition(y, x);
			if (stones->size() >= 3) {
				// add to groups
				combinations->push_back(stones);
			}
		}
	}
	// reset visited map
	mEngine.resetStoneVisited();
	return combinations;
}

std::vector<position>* MinerSpeedGame::scanPosition(const int row, const int column) {
	if (mEngine.getStoneVisited()[row][column] == true) {
		return new std::vector<position>();
	}
	mEngine.setStoneVisited(row, column, true);
	std::vector<position> *results = new std::vector<position>();
	results->push_back(position(row, column));
	std::list<position> nextPositions = { position(row, column + 1), position(row + 1, column) , position(row, column - 1) , position(row - 1, column) };
	for (auto nextPos : nextPositions) {
		int x = std::max(0, std::min(nextPos.second, 7));
		int y = std::max(0, std::min(nextPos.first, 7));
		if (mEngine.getStoneColors()[y][x] == mEngine.getStoneColors()[row][column]) {
			//results->push_back(scan(y, x));
			std::vector<position> * res = scanPosition(x, y);
			if (!res->empty()) {
				for (std::vector<position>::iterator it = res->begin(); it != res->end(); ++it) {
					results->push_back((*it));
				}
			}
			return results;
		}
	}
}

void MinerSpeedGame::swap(const int row, const int column) {
	// only allow row and column-wise moves
	if (row >= 0 && abs(mRow - row) == 1) {
		// if condition to do swap (3+ stones same color) then do swap
		mEngine.swapStoneColor(mRow, mColumn, 0, row - mRow);
		//bool allowed = verifyStoneCombinations(row, column);
		std::vector<std::vector<position>*> *combinations = scan();
		if (combinations->empty()) {
			// revert swap
			mEngine.swapStoneColor(mRow, mColumn, 0, row - mRow);
		}
		//if (!allowed) {
		//	// revert swap
		//	mEngine.swapStoneColor(mRow, mColumn, 0, row - mRow);
		//}
		// just allow one swap
		mEngine.SetMouseButtonDown(false);
	}
	else if (column >= 0 && abs(mColumn - column) == 1) {
		// if condition to do swap (3+ stones same color) then do swap
		mEngine.swapStoneColor(mRow, mColumn, column - mColumn, 0);
		std::vector<std::vector<position>*> *combinations = scan();
		if (combinations->empty()) {
			// revert swap
			mEngine.swapStoneColor(mRow, mColumn, column - mColumn, 0);
		}
		//bool allowed = verifyStoneCombinations(row, column);
		//if (!allowed) {
		//	// revert swap
		//	mEngine.swapStoneColor(mRow, mColumn, column - mColumn, 0);
		//}
		// just allow one swap
		mEngine.SetMouseButtonDown(false);
	}
}

bool MinerSpeedGame::verifyStoneCombinations(const int row, const int column) {
	bool combinationAllowed = false;
	auto stoneColors = mEngine.getStoneColors();
	King::Engine::Texture currentStoneColor = stoneColors[row][column];
	King::Engine::Texture nextStoneColor = stoneColors[mRow][mColumn];
	// CURRENT stone
	
	// ROW
	int xPosLeft = column, xPosRight = column; 
	//bool leftEnd = false, rightEnd = false;
	while (stoneColors[row][--xPosLeft] == currentStoneColor) {}
	while (stoneColors[row][++xPosRight] == currentStoneColor) {}
	int rowOfStones = abs(xPosRight - xPosLeft) - 1;
	
	// COLUMN
	int yPosUp = row, yPosDown = row;
	while (stoneColors[--yPosUp][column] == currentStoneColor) {}
	while (stoneColors[++yPosDown][column] == currentStoneColor) {}
	int columnOfStones = abs(yPosDown - yPosUp) - 1;

	if (rowOfStones >= 3) {
		combinationAllowed = true;
		int currentRow = row, currentColumn = xPosLeft + 1;
		while (currentRow > 0) {
			for (currentColumn = xPosLeft + 1; currentColumn < xPosRight; ++currentColumn) {
				mEngine.swapStoneColor(currentRow, currentColumn, 0, -1);
			}
			--currentRow;
		}
		// create new stones
		for (currentColumn = xPosLeft + 1; currentColumn < xPosRight; ++currentColumn) {
			mEngine.setStoneColor(currentRow, currentColumn, (King::Engine::Texture)(rand() % 5 + 1));
		}
	}
	else if (columnOfStones >= 3) {
		combinationAllowed = true;
		int currentRow = yPosDown - 1, currentColumn = column;
		while (currentRow > (yPosDown - 1 - columnOfStones)) {
			mEngine.swapStoneColor(currentRow, currentColumn, 0, -columnOfStones);
			--currentRow;
		}
		// create new stones
		for (; currentRow >= 0; --currentRow) {
			mEngine.setStoneColor(currentRow, currentColumn, (King::Engine::Texture)(rand() % 5 + 1));
		}
	}
	return combinationAllowed;
}

// FIXME: rotation just in case
//const char text[] = "This rotates at 5/PI Hz";
//mRotation += mEngine.GetLastFrameSeconds();
//mEngine.Write(text, mEngine.GetWidth() / 2.0f, mEngine.GetHeight() / 2.0f, mRotation * 2.5f);