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

		void Render(const Texture texture, const glm::mat4& transform, const float scaling = 1.0f);
		void Render(const Texture texture, const float x, const float y, const float scaling = 1.0f, float rotation = 0.0f);
		void checkAnimationsToRender();
		void fillScene();
		void gameIsOver();

		float CalculateStringWidth(const char* text) const;
		void Write(const char* text, const glm::mat4& transform);
		void Write(const char* text, float x, float y, float rotation = 0.0f);

		bool isTimeOver();
		void waitFor() const;
		float getCurrentTime() const;
		void printTimeLeft();

		void setStonePosition(const int column, const int row, const float mouseX, const float mouseY);
		const position<float>(&getStonePositions() const)[GAME_GRID_SIZE_X][GAME_GRID_SIZE_Y];
		void swapStoneColor(const int column, const int row, const int directionX, const int directionY);
		void setStoneColor(const int column, const int row, const Texture color);
		const Texture(&getStoneColors() const)[GAME_GRID_SIZE_X][GAME_GRID_SIZE_Y];
		
		int GetWidth() const;
		int GetHeight() const;
		int GetTextureHeight(const Texture texture) const;
		int GetTextureWidth(const Texture texture) const;

	private:
		struct EngineImplementation;
		class GameGrid;
		std::unique_ptr<EngineImplementation> mPimpl;
		std::unique_ptr<GameGrid> mGameGrid;
		std::unique_ptr<AnimationManager> mAnimationManager;
		float mStartTime;
		float mTimeLeft;
	};
}