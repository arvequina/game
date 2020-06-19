#define GLM_FORCE_RADIANS 

#include <king/Engine.h>
#include <king/Updater.h>


#include "iostream"
#include "list"
#include <algorithm>
#include <vector>

typedef std::pair<int, int> position;

//**********************************************************************

class MinerSpeedGame : public King::Updater {
public:

	MinerSpeedGame();
	// destructor
	void Start();
	void Update();

private:
	void eventsController();
	void mouseDownEvent();
	void mouseUpEvent();
	void verifySwap(const int row, const int column);
	void swap(const int row, const int column);
	std::vector<position>* scanPosition(const int row, const int column);
	std::vector<std::vector<position>*> * scan();
	bool verifyStoneCombinations(const int row, const int column);
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