#include "minerspeedgame.h"
#include "iostream"

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
	// mouse click
	mouseClickEvent(mEngine.GetMouseButtonDown());
	// mouse release
	mouseReleaseEvent(mEngine.GetMouseButtonUp());
}

void MinerSpeedGame::mouseClickEvent(const bool mouseClick) {
	if (mouseClick) {
		if (mFirst) { // first time serves to select stone, then user can drag or just click somewhere else
			// check mouse pos and intersection in the 8x8 grid
			mPosBeginX = mEngine.GetMouseX();
			mPosBeginY = mEngine.GetMouseY();
			// substract initial point
			mPosBeginX -= 330.0f;
			mPosBeginY -= 75.0f;
			const float pos_increment = 43.0f;
			// FIXME: add variable SELECTED for each stone
			// calculate column and row (FIXME: add epsilon trait for comparison)
			if (mPosBeginX > 0.001f && mPosBeginY > 0.001f && mPosBeginX < 8 * 43.0f && mPosBeginY < 8 * pos_increment) {
				mColumn = int(mPosBeginX) / pos_increment;
				mRow = int(mPosBeginY) / pos_increment;
			}
			mFirst = false;
		} else { 
			// DRAGGING scenario
			// check mouse pos and intersection in the 8x8 grid
			mPosEndX = mEngine.GetMouseX();
			mPosEndY = mEngine.GetMouseY();
			// substract initial point
			mPosEndX -= 330.0f;
			mPosEndY -= 75.0f;
			const float pos_increment = 43.0f;
			// calculate column and row (FIXME: add epsilon trait for comparison)
			int column = mColumn;
			int row = mRow;
			if (mPosEndX > 0.001f && mPosEndY > 0.001f && mPosEndX < 8 * pos_increment && mPosEndY < 8 * pos_increment) {
				column = int(mPosEndX) / pos_increment;
				row = int(mPosEndY) / pos_increment;
			}
			// check if swap is possible
			verifySwap(row, column);
		}
	}
}

void MinerSpeedGame::mouseReleaseEvent(const bool mouseRelease) {
	if (mouseRelease) {
		mFirst = true;
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