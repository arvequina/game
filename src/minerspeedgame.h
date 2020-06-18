#define GLM_FORCE_RADIANS 

#include <king/Engine.h>
#include <king/Updater.h>

//**********************************************************************

class MinerSpeedGame : public King::Updater {
public:

	MinerSpeedGame();
	// destructor
	void Start();
	void Update();

private:
	void VerifyMouseEvents();
	void mouseClickEvent(const bool mouseClick);
	void mouseReleaseEvent(const bool mouseRelease);
	void verifySwap(const int row, const int column);
	void verifyStoneCombinations(const int row, const int column);
	King::Engine mEngine;
	float mRotation;
	float mPosBeginX;
	float mPosBeginY;
	float mPosEndX;
	float mPosEndY;
	bool mFirst;
	int mColumn;
	int mRow;
};

//**********************************************************************