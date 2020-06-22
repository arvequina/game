#include "minerspeedgame.h"

MinerSpeedGame::MinerSpeedGame()
	: mEngine("./assets")
	, mRotation(0.0f)
	, mFirst(true)
	, mStoneSelected(false)
	, mGridOriginIndex(-1,-1) {
}

MinerSpeedGame::~MinerSpeedGame() {
}

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
	maybeDispatchMouseDownEvent();
	maybeDispatchMouseUpEvent();
}

void MinerSpeedGame::maybeDispatchMouseDownEvent() {
	if (!mEngine.GetMouseButtonDown()) {
		return;
	}
	// first time serves to select stone, then user can drag or just click somewhere else
	if (!mStoneSelected) {
		if (mFirst) {
			calculateStoneOriginPosition();
		} else { // DRAGGING scenario
			calculateStoneEndPosition();
		}
	}
}

position<int> MinerSpeedGame::calculateMouseGridPosition() {
	return getAndConvertMousePositionToGridIndex();
}

void MinerSpeedGame::calculateStoneOriginPosition() {
	// check mouse pos and intersection in the 8x8 grid and substract initial point
	mGridOriginIndex = getAndConvertMousePositionToGridIndex();
	mFirst = false;
}

void MinerSpeedGame::calculateStoneEndPosition() {
	// check mouse pos and intersection in the 8x8 grid and substract initial point
	position<int> gridEndIndex = getAndConvertMousePositionToGridIndex();
	// check if swap is possible
	if (gridEndIndex.column > -1 && gridEndIndex.row > -1 && mGridOriginIndex.column > -1 && mGridOriginIndex.row > -1) {
		tryToSwapStones(mGridOriginIndex, gridEndIndex);
	}
}

position<int> MinerSpeedGame::getAndConvertMousePositionToGridIndex() {
	position<int> gridIndex;
	auto gridPos = translateMouseToGridPosition();
	if (gridPos.column > std::numeric_limits<float>::epsilon() &&
		gridPos.row > std::numeric_limits<float>::epsilon() &&
		gridPos.column < RENDER_GRID_SIZE_X && gridPos.row < RENDER_GRID_SIZE_Y) {
		gridIndex.column = static_cast<int>(gridPos.column) / STONE_SIZE_X;
		gridIndex.row = static_cast<int>(gridPos.row) / STONE_SIZE_Y;
	}
	return gridIndex;
}

inline position<float> MinerSpeedGame::translateMouseToGridPosition() const {
	return position<float>(mEngine.GetMouseX() - GRID_POS_BEGIN_X, mEngine.GetMouseY() - GRID_POS_BEGIN_Y);
}

void MinerSpeedGame::maybeDispatchMouseUpEvent() {
	if (mEngine.GetMouseButtonUp()) {
		mFirst = true;
		mEngine.SetMouseButtonUp(false);
		position<int> gridEndIndex = calculateMouseGridPosition();
		if (mStoneSelected) { 
			if (!((gridEndIndex.column == mGridOriginIndex.column) && (gridEndIndex.row == mGridOriginIndex.row))) {
				tryToSwapStones(mGridOriginIndex, gridEndIndex);
			}
			mStoneSelected = false;
		} else {
			if ((gridEndIndex.column == mGridOriginIndex.column) && (gridEndIndex.row == mGridOriginIndex.row)) {
				mStoneSelected = true;
			}
		}
	}
}

void MinerSpeedGame::tryToSwapStones(const position<int> originIndex, const position<int> endIndex) {
	pairOfActions stoneMoveAction;
	// allow only one unit difference row or column wise
	if (endIndex.row >= 0 && abs(originIndex.row - endIndex.row) == 1 && abs(originIndex.column - endIndex.column) == 0) {
		mEngine.swapStoneColor(originIndex.column, originIndex.row, 0, endIndex.row - originIndex.row);
		stoneMoveAction = createVerticalStoneMoveAction(originIndex.row - endIndex.row > 0);
		swap(originIndex, endIndex, stoneMoveAction);
	} else if (endIndex.column >= 0 && abs(originIndex.column - endIndex.column) == 1 && abs(originIndex.row - endIndex.row) == 0) {
		mEngine.swapStoneColor(originIndex.column, originIndex.row, endIndex.column - originIndex.column, 0);
		stoneMoveAction = createHorizontalStoneMoveAction(originIndex.column - endIndex.column > 0);
		swap(originIndex, endIndex, stoneMoveAction);
	}
}

void MinerSpeedGame::swap(const position<int> originIndex, const position<int> endIndex, pairOfActions stoneMoveAction) {
	// just allow one swap
	mEngine.SetMouseButtonDown(false);
	// add moving actions to swaped stones
	mEngine.addStoneAction(endIndex.column, endIndex.row, stoneMoveAction.first);
	mEngine.addStoneAction(originIndex.column, originIndex.row, stoneMoveAction.second);
	vectorOfPositions destroyOriginStones, destroyEndStones;
	destroyOriginStones = getStonesToDestroy(originIndex.column, originIndex.row);
	destroyEndStones = getStonesToDestroy(endIndex.column, endIndex.row);
	if (destroyOriginStones.size() != 0 || destroyEndStones.size() != 0) {
		destroyAndFillStones(destroyOriginStones);
		destroyAndFillStones(destroyEndStones);
	}
	else { // put stones to original location if swap not possible
		mEngine.swapStoneColor(originIndex.column, originIndex.row, endIndex.column - originIndex.column, endIndex.row - originIndex.row);
	}
	bool moreStones = true;
	while (moreStones) {
		vectorOfPositions stonesToDestroy = moreStonesToDestroy();
		destroyAndFillStones(stonesToDestroy);
		if (stonesToDestroy.empty()) {
			moreStones = false;
		}
	}
}

pairOfActions MinerSpeedGame::createVerticalStoneMoveAction(bool upDirection) {
	pairOfActions moveAction;
	if (upDirection) {
		moveAction.first = King::Engine::ActionsFromGestures::FROM_DOWN;
		moveAction.second = King::Engine::ActionsFromGestures::FROM_UP;
	} else {
		moveAction.first = King::Engine::ActionsFromGestures::FROM_UP;
		moveAction.second = King::Engine::ActionsFromGestures::FROM_DOWN;
	}
	return moveAction;
}

pairOfActions MinerSpeedGame::createHorizontalStoneMoveAction(bool leftDirection) {
	pairOfActions moveAction;
	if (leftDirection) {
		moveAction.first = King::Engine::ActionsFromGestures::FROM_LEFT;
		moveAction.second = King::Engine::ActionsFromGestures::FROM_RIGHT;
	} else {
		moveAction.first = King::Engine::ActionsFromGestures::FROM_RIGHT;
		moveAction.second = King::Engine::ActionsFromGestures::FROM_LEFT;
	}
	return moveAction;
}

vectorOfPositions MinerSpeedGame::getStonesToDestroy(const int column, const int row) {
	vectorOfPositions stonesColumn, stonesRow;
	// Max we can destroy is 2 stones left + 2 stones right + 2 stones up or down + the movement (and all possible combinations) -> total 7 
	stonesColumn.reserve(MIN_STONES_COMBO * 3 + 1);
	stonesRow.reserve(MIN_STONES_COMBO * 3 + 1);

	King::Engine::Texture currentColor = mEngine.getStoneColors()[column][row];
	findStonesSameColorInColumn(stonesColumn, currentColor, column, row);
	findStonesSameColorInRow(stonesRow, currentColor, column, row);
	// nothing to do in these cases
	if (stonesColumn.size() == 0 && stonesRow.size() == 0) {
		return vectorOfPositions();
	}
	// add initial point if other direction is empty
	if (stonesRow.size() == 0) {
		stonesColumn.push_back(position<int>(column, row));
		return stonesColumn;
	}
	// add initial point if other direction is empty
	if (stonesColumn.size() == 0) {
		stonesRow.push_back(position<int>(column, row));
		return stonesRow;
	}
	// we get both row and column combinations of stones
	vectorOfPositions resultBoth;
	resultBoth.push_back(position<int>(column, row));
	resultBoth.insert(resultBoth.end(), stonesRow.begin(), stonesRow.end());
	resultBoth.insert(resultBoth.end(), stonesColumn.begin(), stonesColumn.end());
	return resultBoth;
}

void MinerSpeedGame::findStonesSameColorInColumn(vectorOfPositions &stones, King::Engine::Texture color, const int column, const int row) {
	// iterate left in the same column and keep the row
	for (int currentColumn = column - 1; currentColumn >= 0; --currentColumn) {
		if (color == mEngine.getStoneColors()[currentColumn][row]) {
			stones.push_back(position<int>(currentColumn, row));
		} else {
			break;
		}
	}
	// iterate right in the same column and keep the row
	for (int currentColumn = column + 1; currentColumn < GAME_GRID_SIZE_X; ++currentColumn) {
		if (color == mEngine.getStoneColors()[currentColumn][row]) {
			stones.push_back(position<int>(currentColumn, row));
		} else {
			break;
		}
	}
	// nothing to do if not found at least 2 stones of same color
	if (stones.size() < MIN_STONES_COMBO - 1) {
		stones.clear();
	}
}

void MinerSpeedGame::findStonesSameColorInRow(vectorOfPositions &stones, King::Engine::Texture color, const int column, const int row) {
	// iterate down in the same row and keep the column
	for (int currentRow = row + 1; currentRow < GAME_GRID_SIZE_Y; ++currentRow) {
		if (color == mEngine.getStoneColors()[column][currentRow]) {
			stones.push_back(position<int>(column, currentRow));
		} else {
			break;
		}
	}
	// iterate up in the same row and keep the column
	for (int currentRow = row - 1; currentRow >= 0; --currentRow) {
		if (color == mEngine.getStoneColors()[column][currentRow]) {
			stones.push_back(position<int>(column, currentRow));
		} else {
			break;
		}
	}
	// nothing to do if not found at least 2 stones of same color
	if (stones.size() < MIN_STONES_COMBO - 1) {
		stones.clear();
	}
}

void MinerSpeedGame::destroyAndFillStones(const vectorOfPositions &vect) {
	if (vect.empty()) {
		return;
	}
	for (auto nextPos : vect) {
		mEngine.addStoneAction(nextPos.column, nextPos.row, King::Engine::ActionsFromGestures::DESTROY);
		destroyStone(nextPos);	
	}
	for (auto nextPos : vect) {
		assignColorToDestroyedStones(nextPos.column, nextPos.row);
	}
}

void MinerSpeedGame::destroyStone(const position<int> &pos) {
	mEngine.setStoneColor(pos.column, pos.row, King::Engine::Texture::TEXTURE_EMPTY);
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

vectorOfPositions MinerSpeedGame::moreStonesToDestroy() {
	// we need to check again for possible combinations as the grid has been modified
	vectorOfPositions stonesToDestroy = vectorOfPositions();
	bool exitLoop = false;
	for (int y = 0; y < GAME_GRID_SIZE_Y; ++y) {
		for (int x = 0; x < GAME_GRID_SIZE_X; ++x) {
			stonesToDestroy = getStonesToDestroy(x, y);
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