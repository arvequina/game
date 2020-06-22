#ifndef ANIMATION_HEADER
#define ANIMATION_HEADER

#include "common.h"

class AnimationManager;
class Animation {
public:
	Animation(const position<int>& stoneIndex,
		      const ActionFromGesture& action,
		      const Texture& stoneColor,
		      const unsigned int startTime)
		: mStoneIndex(stoneIndex)
		, mAction(action)
		, mStoneColor(stoneColor)
		, mStartTime(startTime) {}
	position<int>& stoneIndex() { return mStoneIndex; }
	ActionFromGesture& action() { return mAction; }
	Texture& stoneColor() { return mStoneColor; }
	unsigned int startTime() { return mStartTime; }
private:
	position<int> mStoneIndex;
	ActionFromGesture mAction;
	Texture mStoneColor;
	unsigned int mStartTime;
};

#endif // ANIMATION_HEADER