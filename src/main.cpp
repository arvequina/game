#define GLM_FORCE_RADIANS 

#include <king/Engine.h>
#include <king/Updater.h>

//**********************************************************************

class ExampleGame : public King::Updater {
public:

	ExampleGame()
		: mEngine("./assets"),
		  mRotation(0.0f),
		  mYellowDiamondX(100.0f),
		  mYellowDiamondY(100.0f) 
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
			mYellowDiamondX = mEngine.GetMouseX();
			mYellowDiamondY = mEngine.GetMouseY();
		}
		//mEngine.Render(King::Engine::TEXTURE_YELLOW, mYellowDiamondX, mYellowDiamondY);
		//mEngine.Write("Click to", mYellowDiamondX, mYellowDiamondY + 40.0f);
		//mEngine.Write("move me!", mYellowDiamondX, mYellowDiamondY + 70.0f);
	}

private:
	King::Engine mEngine;
	float mRotation;
	float mYellowDiamondX;
	float mYellowDiamondY;
};

//**********************************************************************

int main(int argc, char *argv[]) {
	ExampleGame game;
	game.Start();

	return 0;
}


