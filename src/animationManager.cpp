#include "animationManager.h"

#include "../external/include/king/Engine.h"
#include <sdl/Sdl.h>

AnimationManager::AnimationManager() 
	: mAnimations(new std::vector<Animation*>())
{
	resetStonesActionGrid();
}

AnimationManager::~AnimationManager() {
	delete mAnimations;
}

void AnimationManager::addAnimation(Animation *animation) {
	mAnimations->push_back(animation);
	// increment stone action grid
	getStoneActionGrid()[animation->stoneIndex().column][animation->stoneIndex().row]++;
}

std::vector<Animation*> *AnimationManager::getAnimations() const {
	return mAnimations;
}

void AnimationManager::renderAnimations(King::Engine *engine) {
	auto animations = getAnimations();
	unsigned int currentTick = SDL_GetTicks();
	if (!animations->empty()) {
		bool done = false;
		for (auto animation : *animations) {
			if (currentTick - animation->startTime() > ANIMATION_DURATION * 1000.0f) { // animation done
				getStoneActionGrid()[animation->stoneIndex().column][animation->stoneIndex().row]--;
				done = true;
			} else { // do animation
				float scaling = 1.0f;
				float pos_x_ini = GRID_POS_BEGIN_X;
				float pos_y_ini = GRID_POS_BEGIN_Y + OFFSET_RENDER_Y;
				float timeRemaining = ANIMATION_DURATION * 1000.0f - (currentTick - animation->startTime());
				float offsetAnimation = timeRemaining * static_cast<float>(STONE_SIZE_X) / (ANIMATION_DURATION * 1000.0f);
				switch (animation->action()) {
				case ActionFromGesture::FROM_DOWN:
					pos_y_ini += offsetAnimation;
					break;
				case ActionFromGesture::FROM_UP:
					pos_y_ini -= offsetAnimation;
					break;
				case ActionFromGesture::FROM_LEFT:
					pos_x_ini += offsetAnimation;
					break;
				case ActionFromGesture::FROM_RIGHT:
					pos_x_ini -= offsetAnimation;
					break;
				case ActionFromGesture::DESTROY:
					scaling = 0.0f;
					break;
				}
				engine->Render(animation->stoneColor(), pos_x_ini + engine->getStonePositions()[animation->stoneIndex().column][animation->stoneIndex().row].column,
					pos_y_ini + engine->getStonePositions()[animation->stoneIndex().column][animation->stoneIndex().row].row, scaling);
			}
		}
		if (done) {
			animations->clear();
		}
	} else {
		// reset action grid
		resetStonesActionGrid();
	}
}

const unsigned int AnimationManager::getStoneActions(const int column, const int row) {
	return mStonesActions[column][row];
}

unsigned int(&AnimationManager::getStoneActionGrid())[GAME_GRID_SIZE_X][GAME_GRID_SIZE_Y] {
	return mStonesActions;
}

void AnimationManager::resetStonesActionGrid()
{
	for (int row = 0; row < GAME_GRID_SIZE_Y; ++row) {
		for (int column = 0; column < GAME_GRID_SIZE_X; ++column) {
			mStonesActions[column][row] = 0;
		}
	}
}