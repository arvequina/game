#include "animationManager.h"

#include "../external/include/king/Engine.h"
#include <sdl/Sdl.h>
#include <vector>

AnimationManager::AnimationManager() 
	: mAnimations(new std::vector<std::shared_ptr<Animation>>()) {
	resetStonesActionGrid();
}

AnimationManager::~AnimationManager() {
	// no need to delete internal pointers in vector as they are shared pointers
	delete mAnimations;
}

void AnimationManager::addAnimation(std::shared_ptr<Animation> animation) {
	mAnimations->push_back(animation);
	// increment stone action grid
	getStoneActionGrid()[animation->stoneIndex().column][animation->stoneIndex().row]++;
}

std::vector<std::shared_ptr<Animation>> *AnimationManager::getAnimations() const {
	return mAnimations;
}

void AnimationManager::checkAnimationsToRender(King::Engine *engine) {
	if (!getAnimations()->empty()) {
		renderAnimations(engine);
	} else {
		// reset action grid
		resetStonesActionGrid();
	}
}

void AnimationManager::renderAnimations(King::Engine *engine) {
	unsigned int currentTick = SDL_GetTicks();
	unsigned int animationsFinished = 0;
	auto animations = getAnimations();
	for (auto animation : *animations) {
		if ((currentTick - animation->startTime() > ANIMATION_DURATION * 1000.0f) || animation->isFinished()) { // animation finishing or already finished
			getStoneActionGrid()[animation->stoneIndex().column][animation->stoneIndex().row]--;
			if (!animation->isFinished()) {
				animation->setFinished(true);
			}
			++animationsFinished;
		} else {
			// calculate animation parameters
			float scaling = 1.0f;
			float offsetPosX = GRID_POS_BEGIN_X;
			float offsetPosY = GRID_POS_BEGIN_Y + OFFSET_RENDER_Y;
			float animationRemainingTime = ANIMATION_DURATION * 1000.0f - (currentTick - animation->startTime());
			float offsetAnimation = animationRemainingTime * static_cast<float>(STONE_SIZE_X) / (ANIMATION_DURATION * 1000.0f);

			switch (animation->action()) {
			case ActionFromGesture::FROM_DOWN:
				offsetPosY += offsetAnimation;
				break;
			case ActionFromGesture::FROM_UP:
				offsetPosY -= offsetAnimation;
				break;
			case ActionFromGesture::FROM_LEFT:
				offsetPosX += offsetAnimation;
				break;
			case ActionFromGesture::FROM_RIGHT:
				offsetPosX -= offsetAnimation;
				break;
			case ActionFromGesture::DESTROY:
				scaling = ANIMATION_SCALING;
				offsetPosX += STONE_SIZE_X * (1.0f - scaling) * 0.5f;
				offsetPosY += STONE_SIZE_Y * (1.0f - scaling) * 0.5f;
				break;
			default:
				break;
			}
			engine->Render(animation->stoneColor(), offsetPosX + engine->getStonePositions()[animation->stoneIndex().column][animation->stoneIndex().row].column,
				offsetPosY + engine->getStonePositions()[animation->stoneIndex().column][animation->stoneIndex().row].row, scaling);
		}
	}
	// clear animations container if all animations are finished
	if (animationsFinished == animations->size()) {
		animations->clear();
	}
}

const unsigned int AnimationManager::getStoneActions(const int column, const int row) {
	return mStonesActions[column][row];
}

unsigned int(&AnimationManager::getStoneActionGrid())[GAME_GRID_SIZE_X][GAME_GRID_SIZE_Y] {
	return mStonesActions;
}

void AnimationManager::resetStonesActionGrid() {
	for (int row = 0; row < GAME_GRID_SIZE_Y; ++row) {
		for (int column = 0; column < GAME_GRID_SIZE_X; ++column) {
			mStonesActions[column][row] = 0;
		}
	}
}

pairOfActions AnimationManager::createVerticalAnimationAction(bool upDirection) {
	pairOfActions animationAction;
	if (upDirection) {
		animationAction.first = ActionFromGesture::FROM_DOWN;
		animationAction.second = ActionFromGesture::FROM_UP;
	} else {
		animationAction.first = ActionFromGesture::FROM_UP;
		animationAction.second = ActionFromGesture::FROM_DOWN;
	}
	return animationAction;
}

pairOfActions AnimationManager::createHorizontalAnimationAction(bool leftDirection) {
	pairOfActions animationAction;
	if (leftDirection) {
		animationAction.first = ActionFromGesture::FROM_LEFT;
		animationAction.second = ActionFromGesture::FROM_RIGHT;
	} else {
		animationAction.first = ActionFromGesture::FROM_RIGHT;
		animationAction.second = ActionFromGesture::FROM_LEFT;
	}
	return animationAction;
}