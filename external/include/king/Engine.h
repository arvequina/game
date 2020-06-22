// (C) king.com Ltd 2014
#include <glm/fwd.hpp>
#include <memory>

// help template class for positions (float) and index positions (int)
template <class T> 
class position {
public:
	position() : column(static_cast<T>(-1)), row(static_cast<T>(-1)) {}
	position(T newCol, T newRow) { column = newCol; row = newRow; }
	T column;
	T row;
};

constexpr auto WINDOW_WITDH = 755;
constexpr auto WINDOW_HEIGHT = 600;
constexpr auto POS_TIME_LEFT_TEXT_X = 50.0f;
constexpr auto POS_TIME_LEFT_TEXT_Y = 390.0f;
constexpr auto POS_TIME_LEFT_NUM_X = 110.0f;
constexpr auto POS_TIME_LEFT_NUM_Y = 420.0f;
constexpr auto STONE_SIZE_X = 43;
constexpr auto STONE_SIZE_Y = 43;
constexpr auto MIN_STONES_COMBO = 3;
constexpr auto GAME_GRID_SIZE_X = 8;
constexpr auto GAME_GRID_SIZE_Y = 8;
constexpr auto RENDER_GRID_SIZE_X = STONE_SIZE_X * GAME_GRID_SIZE_X;
constexpr auto RENDER_GRID_SIZE_Y = STONE_SIZE_Y * GAME_GRID_SIZE_Y;
constexpr auto OFFSET_RENDER_Y = 25.0f;
constexpr auto GRID_POS_BEGIN_X = 330.0f;
constexpr auto GRID_POS_BEGIN_Y = 75.0f;
constexpr auto MAX_GAME_TIME = 600.0f;
constexpr auto ANIMATION_DURATION = 1.0f;
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

		enum ActionsFromGestures {
			FROM_DOWN,
			FROM_UP,
			FROM_LEFT,
			FROM_RIGHT,
			DESTROY
		};

		typedef struct{
			int column;
			int row;
			ActionsFromGestures action;
			Texture incomingColor;
			unsigned int startTime;
		} actionsAnimation;

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
		void Render(Texture texture, const glm::mat4& transform, const float scaling = 1.0f);
		void Render(Texture texture, float x, float y, const float scaling = 1.0f, float rotation = 0.0f);
		void fillScene();
		void gameOverScene();

		float CalculateStringWidth(const char* text) const;
		void Write(const char* text, const glm::mat4& transform);
		void Write(const char* text, float x, float y, float rotation = 0.0f);

		bool isTimeOver();
		void waitFor(float waitTime) const;
		void renderAnimations();
		void addStoneAction(const int column, const int row, const ActionsFromGestures action);
		float getCurrentTime() const;
		void printTimeLeft();

		void setStonePosition(const int column, const int row, const float mouseX, const float mouseY);
		const position<float>(&getStonePositions() const)[GAME_GRID_SIZE_X][GAME_GRID_SIZE_Y];
		void swapStoneColor(const int column, const int row, const int directionX, const int directionY);
		void setStoneColor(const int column, const int row, King::Engine::Texture color);
		const King::Engine::Texture(&getStoneColors() const)[GAME_GRID_SIZE_X][GAME_GRID_SIZE_Y];
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