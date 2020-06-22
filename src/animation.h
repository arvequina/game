#ifndef ANIMATION_HEADER
#define ANIMATION_HEADER

#include "common.h"

class AnimationManager;
class Animation {
public:
	Animation(const position<int>& stoneIndex,
		      const ActionFromGesture& action,
		      const Texture& stoneColor,
		      const float startTime)
		: mStoneIndex(stoneIndex)
		, mAction(action)
		, mStoneColor(stoneColor)
		, mStartTime(startTime) {}
	position<int>& stoneIndex() { return mStoneIndex; }
	ActionFromGesture& action() { return mAction; }
	Texture& stoneColor() { return mStoneColor; }
	float startTime() { return mStartTime; }
private:
	position<int> mStoneIndex;
	ActionFromGesture mAction;
	Texture mStoneColor;
	float mStartTime;
};

#endif // ANIMATION_HEADER