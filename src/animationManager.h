#ifndef ANIMATION_MANAGER_HEADER
#define ANIMATION_MANAGER_HEADER

#include <vector>
#include <memory>
#include "animation.h"
#include "common.h"


namespace King {
	class Engine;
};

class AnimationManager {
public:
	AnimationManager();
	~AnimationManager();

	void addAnimation(std::shared_ptr<Animation> animation);
	std::vector<std::shared_ptr<Animation>> *getAnimations() const;
	void checkAnimationsToRender(King::Engine *engine);
	void renderAnimations(King::Engine *engine);
	const unsigned int getStoneActions(const int column, const int row);
	unsigned int(&getStoneActionGrid())[GAME_GRID_SIZE_X][GAME_GRID_SIZE_Y];
	void resetStonesActionGrid();
	pairOfActions createVerticalAnimationAction(bool upDirection);
	pairOfActions createHorizontalAnimationAction(bool leftDirection);

private:
	std::vector<std::shared_ptr<Animation>> *mAnimations;
	unsigned int mStonesActions[GAME_GRID_SIZE_X][GAME_GRID_SIZE_Y];

};

#endif // ANIMATION_MANAGER_HEADER