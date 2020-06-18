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
			//std::cout << "[DEBUG] START column : " << mColumn << std::endl;
			//std::cout << "[DEBUG] START row : " << mRow << std::endl;
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
			//std::cout << "[DEBUG] END column : " << mColumn << std::endl;
			//std::cout << "[DEBUG] END row : " << mRow << std::endl;
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
		verifyStoneCombinations(row, column);
		mEngine.setStoneColor(mRow, mColumn, 0, row - mRow);
		// just allow one swap
		mEngine.SetMouseButtonDown(false);
	}
	else if (column >= 0 && abs(mColumn - column) == 1) {
		// if condition to do swap (3+ stones same color) then do swap
		verifyStoneCombinations(row, column);
		mEngine.setStoneColor(mRow, mColumn, column - mColumn, 0);
		// just allow one swap
		mEngine.SetMouseButtonDown(false);
	}
	//mEngine.setStonePosition(mRow, mColumn, mEngine.GetMouseX() - 330.0f, mEngine.GetMouseY() - 75.0f);
}

void MinerSpeedGame::verifyStoneCombinations(const int row, const int column) {
	auto stoneColors = mEngine.getStoneColors();
	King::Engine::Texture nextStoneColor = stoneColors[row][column];
	King::Engine::Texture currentStoneColor = stoneColors[mRow][mColumn];
	// CURRENT stone
	// row-wise verification (left/right)
	// left 
	int xLeft = column - 1, xRight = column + 1;
	bool leftEnd = false, rightEnd = false;
	std::cout << "[DEBUG] currentStoneColor : " << currentStoneColor;
	while ((xLeft >= 0 || xRight < 8) && !(leftEnd && rightEnd)) {
		std::cout << "[DEBUG] stoneColor row - xLeft : " << row << " - " << xLeft << " is -> " << stoneColors[row][xLeft] << std::endl;
		if (xLeft >= 0 && stoneColors[row][xLeft] == currentStoneColor) {
			--xLeft;
		} else {
			leftEnd = true;
		}
		std::cout << "[DEBUG] stoneColor row - xRight : " << row << " - " << xRight << " is -> " << stoneColors[row][xRight] << std::endl;
		if (xRight < 8 && stoneColors[row][xRight] == currentStoneColor) {
			++xRight;
		} else {
			rightEnd = true;
		}
	}
	// if 3 or more, remove stones
	int rowOfStones = xRight - (xLeft + 1);
	
	// **** COLUMN ****
	//std::cout << "[DEBUG] BEGIN rowOfStones : " << rowOfStones << std::endl;
	//std::cout << "[DEBUG] xLeft : " << xLeft << std::endl;
	//std::cout << "[DEBUG] END xRight : " << xRight << std::endl;
	// column-wise verification (up/down)
	int yDown = row + 1, yUp = row - 1;
	bool downEnd = false, upEnd = false;
	while ((yUp >= 0 || yDown < 8) && !(downEnd && upEnd)) {
		if (yUp >= 0 && stoneColors[yUp][column] == currentStoneColor) {
			--yUp;
		} else {
			upEnd = true;
		}
		if (yDown < 8 && stoneColors[yDown][column] == currentStoneColor) {
			++yDown;
		} else {
			downEnd = true;
		}
	}
	// if 3 or more, remove stones
	int columnOfStones = yDown - (yUp + 1);
	//std::cout << "[DEBUG] BEGIN columnOfStones : " << columnOfStones << std::endl;
	//std::cout << "[DEBUG] yDown : " << yDown << std::endl;
	//std::cout << "[DEBUG] END yUp : " << yUp << std::endl;

	// *****************************************************

	// NEXT stone
	// row-wise verification (left/right)
	//for () {}
	// column-wise verification (up/down)
	//for () {}
	//std::cout << "[DEBUG] stoneColor row - column : " << row << " - " << column << " is -> " << stoneColors[row][column] << std::endl;
	//std::cout << "[DEBUG] xLeft : " << xLeft << std::endl;
	//std::cout << "[DEBUG] xRight : " << xRight << std::endl;
	//std::cout << "[DEBUG] leftEnd : " << leftEnd << std::endl;
	//std::cout << "[DEBUG] rightEnd : " << rightEnd << std::endl;
}

// FIXME: rotation just in case
//const char text[] = "This rotates at 5/PI Hz";
//mRotation += mEngine.GetLastFrameSeconds();
//mEngine.Write(text, mEngine.GetWidth() / 2.0f, mEngine.GetHeight() / 2.0f, mRotation * 2.5f);