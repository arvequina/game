#ifndef ANIMATION_MANAGER_HEADER
#define ANIMATION_MANAGER_HEADER

#include <vector>
#include "animation.h"
#include "common.h"

namespace King {
	class Engine;
};

class AnimationManager {
public:
	AnimationManager();
	~AnimationManager();
	void addAnimation(Animation *animation);
	std::vector<Animation*> *getAnimations() const;
	void renderAnimations(King::Engine *engine);
	const unsigned int getStoneActions(const int column, const int row);
	unsigned int(&getStoneActionGrid())[GAME_GRID_SIZE_X][GAME_GRID_SIZE_Y];
	void resetStonesActionGrid();
	pairOfActions createVerticalAnimationAction(bool upDirection);
	pairOfActions createHorizontalAnimationAction(bool leftDirection);
private:
	std::vector<Animation*> *mAnimations;
	unsigned int mStonesActions[GAME_GRID_SIZE_X][GAME_GRID_SIZE_Y];
};

#endif // ANIMATION_MANAGER_HEADER