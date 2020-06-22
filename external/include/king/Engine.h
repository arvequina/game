// (C) king.com Ltd 2014
#include <glm/fwd.hpp>
#include <memory>
#include "../../../src/common.h"

class AnimationManager;
namespace King {
	class Updater;
	class Engine {
	public:
		Engine(const char* assetsDirectory);
		~Engine();

		float GetLastFrameSeconds() const;
		float GetMouseX() const;
		float GetMouseY() const;
		bool GetMouseButtonDown() const;
		void SetMouseButtonDown(const bool state);
		bool GetMouseButtonUp() const;
		void SetMouseButtonUp(const bool state);
		
		void initializeTimer();
		void setAnimationManager(AnimationManager *manager);
		void Start(Updater& updater);
		void Quit();

		int GetTextureHeight(Texture texture) const;
		int GetTextureWidth(Texture texture) const;
		void Render(Texture texture, const glm::mat4& transform, const float scaling = 1.0f);
		void Render(Texture texture, float x, float y, const float scaling = 1.0f, float rotation = 0.0f);
		void fillScene();
		void gameIsOver();

		float CalculateStringWidth(const char* text) const;
		void Write(const char* text, const glm::mat4& transform);
		void Write(const char* text, float x, float y, float rotation = 0.0f);

		bool isTimeOver();
		void waitFor(float waitTime) const;
		void checkAnimationsToRender();
		float getCurrentTime() const;
		void printTimeLeft();

		void setStonePosition(const int column, const int row, const float mouseX, const float mouseY);
		const position<float>(&getStonePositions() const)[GAME_GRID_SIZE_X][GAME_GRID_SIZE_Y];
		void swapStoneColor(const int column, const int row, const int directionX, const int directionY);
		void setStoneColor(const int column, const int row, Texture color);
		const Texture(&getStoneColors() const)[GAME_GRID_SIZE_X][GAME_GRID_SIZE_Y];
		static Texture getRandomStoneColor();
	
		int GetWidth() const;
		int GetHeight() const;

	private:
		struct EngineImplementation;
		class GameGrid;
		std::unique_ptr<EngineImplementation> mPimpl;
		std::unique_ptr<GameGrid> mGameGrid;
		float mStartTime;
		float mTimeLeft;
		std::unique_ptr<AnimationManager> mAnimationManager;
	};
}