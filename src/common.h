#pragma once

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

enum ActionFromGesture {
	STILL,
	FROM_DOWN,
	FROM_UP,
	FROM_LEFT,
	FROM_RIGHT,
	DESTROY
};

// help template class for positions (float) and index positions (int)
template <class T>
class position {
public:
	position() : column(static_cast<T>(-1)), row(static_cast<T>(-1)) {}
	position(T newCol, T newRow) { column = newCol; row = newRow; }
	bool isSameIndex(const position<int>& comparison) {
		return (this->column == comparison.column)
			&& (this->row == comparison.row);
	}
	bool isSameIndexF(const position<float>& comparison) {
		return (this->column - comparison.column < std::numeric_limits<float>::epsilon())
			&& (this->row - comparison.row < std::numeric_limits<float>::epsilon());
	}
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
constexpr auto MAX_GAME_TIME = 60.0f;
constexpr auto ANIMATION_DURATION = 1.0f;
constexpr auto GAME_OVER_WAIT_TIME = 3.0f;
constexpr auto FPS = 60.0f;
