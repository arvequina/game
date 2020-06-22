#include "animation.h"

Animation::Animation(const position<int> stoneIndex, 
	                 const ActionFromGesture action, 
	                 const Texture stoneColor, 
	                 const unsigned int startTime)
	: mStoneIndex(stoneIndex)
	, mAction(action)
	, mStoneColor(stoneColor)
	, mStartTime(startTime)
{}
