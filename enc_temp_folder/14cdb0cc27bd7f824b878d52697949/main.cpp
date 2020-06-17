#define GLM_FORCE_RADIANS 

#include <king/Engine.h>
#include <king/Updater.h>

#include "iostream"

//**********************************************************************

class ExampleGame : public King::Updater {
public:

	ExampleGame()
		: mEngine("./assets"),
		  mRotation(0.0f),
		  mPosBeginX(0.0f),
		  mPosBeginY(0.0f),
		  mPosEndX(0.0f),
		  mPosEndY(0.0f),
		  mFirst(true)
	{}

	void Start() {
		mEngine.Start(*this);
	}

	void Update() {
		// create structure 8x8 with rendering info
		mEngine.fillScene();
		//const char text[] = "This rotates at 5/PI Hz";
		//mRotation += mEngine.GetLastFrameSeconds();
		//mEngine.Write(text, mEngine.GetWidth() / 2.0f, mEngine.GetHeight() / 2.0f, mRotation * 2.5f);

		if (mEngine.GetMouseButtonDown()) {
			int column = -1, row = -1;
			if (mFirst) {
				// check mouse pos and intersection in the 8x8 grid
				mPosBeginX = mEngine.GetMouseX();
				mPosBeginY = mEngine.GetMouseY();
				// check if any stone gets selected
				// ...
				// substract initial point
				//std::cout << "[DEBUG] BEGIN mPosBeginX : " << mPosBeginX << std::endl;
				//std::cout << "[DEBUG] BEGIN mPosBeginY : " << mPosBeginY << std::endl;
				mPosBeginX -= 330.0f;
				mPosBeginY -= 75.0f;
				//std::cout << "[DEBUG] AFTER mPosBeginX : " << mPosBeginX << std::endl;
				//std::cout << "[DEBUG] AFTER mPosBeginY : " << mPosBeginY << std::endl;

				// calculate column and row
				if (mPosBeginX > 0.001f && mPosBeginY > 0.001f && mPosBeginX < 8 * 43.0f && mPosBeginY < 8 * 43.0f) {
					column = int(mPosBeginX) / 43.0f;
					row = int(mPosBeginY) / 43.0f;
				}
				std::cout << "[DEBUG] column : " << column << std::endl;
				std::cout << "[DEBUG] row : " << row << std::endl;
			} else {
				// move selected stone
				// ...
			}
			
		}
		if (mEngine.GetMouseButtonUp()) {
			mFirst = true;
			mEngine.SetMouseButtonUp(false);
			mPosEndX = mEngine.GetMouseX();
			mPosEndY = mEngine.GetMouseY();
			// check swap of stones if possible
			// ...
		}
		//mEngine.Render(King::Engine::TEXTURE_YELLOW, mYellowDiamondX, mYellowDiamondY);
		//mEngine.Write("Click to", mYellowDiamondX, mYellowDiamondY + 40.0f);
		//mEngine.Write("move me!", mYellowDiamondX, mYellowDiamondY + 70.0f);
	}

private:
	King::Engine mEngine;
	float mRotation;
	float mPosBeginX;
	float mPosBeginY;
	float mPosEndX;
	float mPosEndY;
	bool mFirst;
};

//**********************************************************************

int main(int argc, char *argv[]) {
	ExampleGame game;
	game.Start();

	return 0;
}


