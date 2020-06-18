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