// (C) king.com Ltd 2014
#include <glm/fwd.hpp>
#include <memory>

typedef std::pair<int, int> position;
typedef std::pair<float, float> positionF;

constexpr auto WINDOW_WITDH = 755;
constexpr auto WINDOW_HEIGHT = 600;
constexpr auto POS_BEGIN_X = 330;
constexpr auto POS_BEGIN_Y = 100;
constexpr auto POS_TIME_LEFT_TEXT_X = 50.0f;
constexpr auto POS_TIME_LEFT_TEXT_Y = 390.0f;
constexpr auto POS_TIME_LEFT_NUM_X = 110.0f;
constexpr auto POS_TIME_LEFT_NUM_Y = 420.0f;
constexpr auto SCENE_SIZE = 344;
constexpr auto STONE_SIZE = 43;
constexpr auto GAME_GRID_SIZE = 8;
constexpr auto MAX_GAME_TIME = 60.0f;
constexpr auto GAME_OVER_WAIT_TIME = 3.0f;
constexpr auto FPS = 60.0f;

namespace King {
	class Updater;
	class Engine {
	public:
		enum Texture {
			TEXTURE_BACKGROUND,
			TEXTURE_BLUE,
			TEXTURE_GREEN,
			TEXTURE_PURPLE,
			TEXTURE_RED,
			TEXTURE_YELLOW,
			TEXTURE_EMPTY,
			TEXTURE_MAX
		};

		Engine(const char* assetsDirectory);
		~Engine();

		float GetLastFrameSeconds() const;
		float GetMouseX() const;
		float GetMouseY() const;
		bool GetMouseButtonDown() const;
		void SetMouseButtonDown(const bool state);
		bool GetMouseButtonUp() const;
		void SetMouseButtonUp(const bool state);
		
		void initGame();
		void initializeTimer();
		void initializeGameGrid() const;
		void Start(Updater& updater);
		void Quit();

		int GetTextureHeight(Texture texture) const;
		int GetTextureWidth(Texture texture) const;
		void Render(Texture texture, const glm::mat4& transform);
		void Render(Texture texture, float x, float y, float rotation = 0.0f);
		void fillScene();
		void gameOverScene();

		float CalculateStringWidth(const char* text) const;
		void Write(const char* text, const glm::mat4& transform);
		void Write(const char* text, float x, float y, float rotation = 0.0f);

		
		bool isTimeOver();
		void waitFor(float waitTime) const;
		float getCurrentTime() const;
		void printTimeLeft();

		void setStonePosition(const int row, const int column, const float mouseX, const float mouseY);
		const positionF(&getStonePositions() const)[GAME_GRID_SIZE][GAME_GRID_SIZE];
		void swapStoneColor(const int row, const int column, const int directionX, const int directionY);
		void setStoneColor(const int row, const int column, King::Engine::Texture color);
		const King::Engine::Texture(&getStoneColors() const)[GAME_GRID_SIZE][GAME_GRID_SIZE];
		static King::Engine::Texture getRandomStoneColor();
	
		int GetWidth() const;
		int GetHeight() const;

	private:
		struct EngineImplementation;
		class GameGrid;
		std::unique_ptr<EngineImplementation> mPimpl;
		float mStartTime;
		float mTimeLeft;
	};
}