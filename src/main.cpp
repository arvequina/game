#define GLM_FORCE_RADIANS 

#include <king/Engine.h>
#include <king/Updater.h>

//**********************************************************************

class ExampleGame : public King::Updater {
public:

	ExampleGame()
		: mEngine("./assets"),
		  mRotation(0.0f),
		  mPosX(0.0f),
		  mPosY(0.0f) 
	{}

	void Start() {
		mEngine.Start(*this);
	}

	void Update() {
		mEngine.fillScene();
		//const char text[] = "This rotates at 5/PI Hz";
		//mRotation += mEngine.GetLastFrameSeconds();
		//mEngine.Write(text, mEngine.GetWidth() / 2.0f, mEngine.GetHeight() / 2.0f, mRotation * 2.5f);

		if (mEngine.GetMouseButtonDown()) {
			// check mouse pos and intersection in the 8x8 grid
			mPosX = mEngine.GetMouseX();
			mPosY = mEngine.GetMouseY();
			// check if any stone gets selected
			// ...
		}
		if (mEngine.GetMouseButtonUp()) {
			mEngine.SetMouseButtonUp(false);
			mPosX = mEngine.GetMouseX();
			mPosY = mEngine.GetMouseY();
			// check swap of stones
			// ...
		}
		//mEngine.Render(King::Engine::TEXTURE_YELLOW, mYellowDiamondX, mYellowDiamondY);
		//mEngine.Write("Click to", mYellowDiamondX, mYellowDiamondY + 40.0f);
		//mEngine.Write("move me!", mYellowDiamondX, mYellowDiamondY + 70.0f);
	}

private:
	King::Engine mEngine;
	float mRotation;
	float mPosX;
	float mPosY;
};

//**********************************************************************

int main(int argc, char *argv[]) {
	ExampleGame game;
	game.Start();

	return 0;
}


