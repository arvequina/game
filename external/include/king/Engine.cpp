#include "Engine.h"

#define GLM_FORCE_RADIANS 
#include <glm/gtc/matrix_transform.hpp>
#include <glew/glew.h>
#include <glm/glm.hpp>
#include <sdl/Sdl.h>
#include <iostream>
#include <string>
#include <time.h>
#include <stdexcept>
#include <algorithm>
#include <vector>

#include "Font.h"
#include "GlContext.h"
#include "Sdl.h"
#include "SdlWindow.h"
#include "SdlSurface.h"
#include "Updater.h"

namespace King {
	static const int WindowWidth = WINDOW_WITDH;
	static const int WindowHeight = WINDOW_HEIGHT;
	static const float MaxFrameTicks = 300.0f;
	static const float TextScale = 0.5f;

	class Engine::GameGrid {
	public:
		GameGrid() {
			// initialize positions
			initialize();
		}
		void initialize();
		const position<float>(&getStonePositions() const)[GAME_GRID_SIZE_X][GAME_GRID_SIZE_Y];
		const King::Engine::Texture(&getStoneColors() const)[GAME_GRID_SIZE_X][GAME_GRID_SIZE_Y];
		void setStonePosition(const int column, const int row, const float mouseX, const float mouseY);
		void swapStoneColor(const int column, const int row, const int directionX, const int directionY);
		void setStoneColor(const int column, const int row, King::Engine::Texture color);
	private:
		position<float> mPositions[GAME_GRID_SIZE_X][GAME_GRID_SIZE_Y];
		King::Engine::Texture mColors[GAME_GRID_SIZE_X][GAME_GRID_SIZE_Y];
	};

	struct Engine::EngineImplementation {
		Sdl mSdl;
		SdlWindow mSdlWindow;
		GlContext mGlContext;
		std::unique_ptr<SdlSurface> mSdlSurfaceContainer[Engine::TEXTURE_MAX];
		std::unique_ptr<SdlSurface> mFontSdlSurface;
		std::unique_ptr<Engine::GameGrid> mGameGrid;
		float mElapsedTicks;
		float mLastFrameSeconds;
		Updater* mUpdater;
		bool mQuit;

		float mMouseX;
		float mMouseY;
		bool mMouseButtonDown;
		bool mMouseButtonUp;
		
		EngineImplementation()
			: mSdl(SDL_INIT_VIDEO | SDL_INIT_TIMER | SDL_INIT_NOPARACHUTE)
			, mSdlWindow(WindowWidth, WindowHeight)
			, mGlContext(mSdlWindow)
			, mLastFrameSeconds(1.0f / FPS)
			, mMouseX(WindowWidth * 0.5f)
			, mMouseY(WindowHeight * 0.5f)
			, mMouseButtonDown(false)
			, mMouseButtonUp(false)
			, mQuit(false)
			, mUpdater(nullptr)
			, mElapsedTicks(static_cast<float>(SDL_GetTicks()))
		{}

		void Start();
		void ParseEvents();
		float getCurrentTime() const;
	};

	Engine::Engine(const char* assetsDirectory)
		: mPimpl(new EngineImplementation) {
		SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
		SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
		SDL_GL_SetSwapInterval(1);

		glEnable(GL_TEXTURE_2D);
		glClearColor(0.0, 0.0, 0.0, 1.0);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		std::string assets(assetsDirectory);
		std::string background = assets; background += "/BackGround.jpg";
		std::string blue = assets; blue += "/Blue.png";
		std::string green = assets; green += "/Green.png";
		std::string purple = assets; purple += "/Purple.png";
		std::string red = assets; red += "/Red.png";
		std::string yellow = assets; yellow += "/Yellow.png";
		std::string transparent = assets; transparent += "/Transparent.png";
		std::string font = assets; font += "/berlin_sans_demi_72_0.png";

		mPimpl->mSdlSurfaceContainer[Engine::TEXTURE_BACKGROUND].reset(new SdlSurface(background.c_str()));
		mPimpl->mSdlSurfaceContainer[Engine::TEXTURE_BLUE].reset(new SdlSurface(blue.c_str()));
		mPimpl->mSdlSurfaceContainer[Engine::TEXTURE_GREEN].reset(new SdlSurface(green.c_str()));
		mPimpl->mSdlSurfaceContainer[Engine::TEXTURE_PURPLE].reset(new SdlSurface(purple.c_str()));
		mPimpl->mSdlSurfaceContainer[Engine::TEXTURE_RED].reset(new SdlSurface(red.c_str()));
		mPimpl->mSdlSurfaceContainer[Engine::TEXTURE_YELLOW].reset(new SdlSurface(yellow.c_str()));
		mPimpl->mSdlSurfaceContainer[Engine::TEXTURE_EMPTY].reset(new SdlSurface(transparent.c_str()));
		mPimpl->mFontSdlSurface.reset(new SdlSurface(font.c_str()));

		// ini gamegrid
		mPimpl->mGameGrid.reset(new Engine::GameGrid());

		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		glOrtho(0.0f, WindowWidth, WindowHeight, 0.0f, -1.0f, 1.0f);
		glMatrixMode(GL_MODELVIEW);
	}

	Engine::~Engine() {
	}

	float Engine::GetLastFrameSeconds() const {
		return mPimpl->mLastFrameSeconds;
	}

	float Engine::GetMouseX() const {
		return mPimpl->mMouseX;
	}

	float Engine::GetMouseY() const {
		return mPimpl->mMouseY;
	}

	bool Engine::GetMouseButtonDown() const {
		return mPimpl->mMouseButtonDown;
	}

	bool Engine::GetMouseButtonUp() const {
		return mPimpl->mMouseButtonUp;
	}

	void Engine::SetMouseButtonDown(const bool state) {
		mPimpl->mMouseButtonDown = state;
	}

	void Engine::SetMouseButtonUp(const bool state) {
		mPimpl->mMouseButtonUp = state;
	}

	void Engine::Quit() {
		mPimpl->mQuit = true;
	}

	void Engine::initGame() {
		initializeGameGrid();
		initializeTimer();
	}

	void Engine::Start(Updater& updater) {
		mPimpl->mUpdater = &updater;
		mPimpl->mSdlWindow.Show();
		mPimpl->Start();
	}

	int Engine::GetTextureHeight(Texture texture) const {
		return mPimpl->mSdlSurfaceContainer[texture]->Height();
	}

	int Engine::GetTextureWidth(Texture texture) const {
		return mPimpl->mSdlSurfaceContainer[texture]->Width();
	}

	void Engine::Render(Engine::Texture texture, const glm::mat4& transform) {
		glLoadMatrixf(reinterpret_cast<const float*>(&transform));
		SdlSurface& surface = *mPimpl->mSdlSurfaceContainer[texture];
		surface.Bind();
		glBegin(GL_QUADS);
		glTexCoord2i(0, 1); glVertex2i(0, surface.Height());
		glTexCoord2i(1, 1); glVertex2i(surface.Width(), surface.Height());
		glTexCoord2i(1, 0); glVertex2i(surface.Width(), 0);
		glTexCoord2i(0, 0); glVertex2i(0, 0);
		glEnd();
	}

	void Engine::Render(Texture texture, float x, float y, float rotation) {
		glm::mat4 transformation;
		transformation = glm::translate(transformation, glm::vec3(x, y, 0.0f));
		if (rotation) {
			transformation = glm::rotate(transformation, rotation, glm::vec3(0.0f, 0.0f, 1.0f));
		}
		Render(texture, transformation);
	}

	Glyph& FindGlyph(char c) {
		auto found = std::lower_bound(std::begin(Font), std::end(Font), c);
		if (found == std::end(Font) || c < *found) {
			found = std::lower_bound(std::begin(Font), std::end(Font), static_cast<int>('_'));
		}
		return *found;
	}

	float Engine::CalculateStringWidth(const char* text) const {
		int advance = 0;
		for (; *text; ++text) {
			Glyph& g = FindGlyph(*text);
			advance += g.advance;
		}
		return advance*TextScale;
	}

	void Engine::Write(const char* text, const glm::mat4& transform) {
		glLoadMatrixf(reinterpret_cast<const float*>(&transform));
		int advance = 0;
		for (; *text;++text) {
			Glyph& g = FindGlyph(*text);

			float fontTexWidth  = static_cast<float>(mPimpl->mFontSdlSurface->Width());
			float fontTexHeight = static_cast<float>(mPimpl->mFontSdlSurface->Height());

			float uvLeft = static_cast<float>(g.x) / fontTexWidth;
			float uvRight = static_cast<float>(g.x + g.width) / fontTexWidth;
			float uvBottom = static_cast<float>(g.y) / fontTexHeight;
			float uvTop = static_cast<float>(g.y + g.height) / fontTexHeight;

			float worldLeft = static_cast<float>(g.xoffset + advance);
			float worldRight = static_cast<float>(g.xoffset + g.width + advance);
			float worldBottom = static_cast<float>(g.yoffset);
			float worldTop = static_cast<float>(g.yoffset + g.height);

			mPimpl->mFontSdlSurface->Bind();

			glBegin(GL_QUADS);
			glTexCoord2f(uvLeft / 2.0f, uvTop / 2.0f); glVertex2f(worldLeft * TextScale, worldTop * TextScale);
			glTexCoord2f(uvRight / 2.0f, uvTop / 2.0f); glVertex2f(worldRight * TextScale, worldTop * TextScale);
			glTexCoord2f(uvRight / 2.0f, uvBottom / 2.0f); glVertex2f(worldRight * TextScale, worldBottom * TextScale);
			glTexCoord2f(uvLeft / 2.0f, uvBottom / 2.0f); glVertex2f(worldLeft * TextScale, worldBottom * TextScale);
			glEnd();
			advance += g.advance;
		}
	}

	void Engine::Write(const char* text, float x, float y, float rotation) {
		glm::mat4 transformation;
		transformation = glm::translate(transformation, glm::vec3(x, y, 0.0f));
		if (rotation) {
			transformation = glm::rotate(transformation, rotation, glm::vec3(0.0f, 0.0f, 1.0f));
			transformation = glm::translate(transformation, glm::vec3(-CalculateStringWidth(text)/2.0f, -20.0f, 0.0f));
		}
		Write(text, transformation);
	}

	int Engine::GetWidth() const {
		return WindowWidth;
	}

	int Engine::GetHeight() const {
		return WindowHeight;
	}

	void Engine::initializeGameGrid() const {
		mPimpl->mGameGrid->initialize();
	}

	float Engine::getCurrentTime() const {
		return static_cast<float>(SDL_GetTicks());
	}

	void Engine::fillScene() {
		Render(King::Engine::TEXTURE_BACKGROUND, 0.0f, 0.0f);
		const float pos_x_ini = GRID_POS_BEGIN_X;
		const float pos_y_ini = GRID_POS_BEGIN_Y + OFFSET_RENDER;
		// nested loop of 8 by 8 (low computing cost)
		for (int row = 0; row < GAME_GRID_SIZE_Y; ++row) {
			for (int column = 0; column < GAME_GRID_SIZE_X; ++column) {
				Render(mPimpl->mGameGrid->getStoneColors()[column][row], pos_x_ini + mPimpl->mGameGrid->getStonePositions()[column][row].column,
					                                pos_y_ini + mPimpl->mGameGrid->getStonePositions()[column][row].row);
			}
		}
		printTimeLeft();
	}

	void Engine::printTimeLeft() {
		if (mTimeLeft < 0.01f) {
			Write("Game over", POS_TIME_LEFT_TEXT_X, POS_TIME_LEFT_TEXT_Y);
		} else {
			Write("Time Left", POS_TIME_LEFT_TEXT_X, POS_TIME_LEFT_TEXT_Y);
			Write(std::to_string(static_cast<int>(mTimeLeft)).c_str(), POS_TIME_LEFT_NUM_X, POS_TIME_LEFT_NUM_Y);
		}
	}

	void Engine::initializeTimer() {
		mStartTime = getCurrentTime();
	}

	bool Engine::isTimeOver() {
		mTimeLeft = MAX_GAME_TIME - (getCurrentTime() - mStartTime)* 0.001f;
		if (mTimeLeft < 0.0f) {
			return true;
		}
		return false;
	}

	void Engine::gameOverScene() {
		waitFor(GAME_OVER_WAIT_TIME);
		// bye!
		Quit();
	}

	void Engine::waitFor(float waitTime) const {
		float initialTime = getCurrentTime();
		while ((getCurrentTime() - initialTime)*0.001f < GAME_OVER_WAIT_TIME) {}
	}

	void Engine::setStonePosition(const int column, const int row, const float mouseX, const float mouseY) {
		mPimpl->mGameGrid->setStonePosition(column, row, mouseX, mouseY);
	}

	const position<float>(&Engine::getStonePositions() const)[GAME_GRID_SIZE_X][GAME_GRID_SIZE_Y] {
		return mPimpl->mGameGrid->getStonePositions();
	}

	void Engine::swapStoneColor(const int column, const int row, const int directionX, const int directionY) {
		mPimpl->mGameGrid->swapStoneColor(column, row, directionX, directionY);
	}

	void Engine::setStoneColor(const int column, const int row, King::Engine::Texture color) {
		mPimpl->mGameGrid->setStoneColor(column, row, color);
	}

	const King::Engine::Texture(&Engine::getStoneColors() const)[GAME_GRID_SIZE_X][GAME_GRID_SIZE_Y] {
		return mPimpl->mGameGrid->getStoneColors();
	}

    King::Engine::Texture Engine::getRandomStoneColor() {
		return static_cast<King::Engine::Texture>(rand() % 5 + 1);
	}

	void Engine::EngineImplementation::Start() {
		while (!mQuit) {
			SDL_GL_SwapWindow(mSdlWindow);
			glClear(GL_COLOR_BUFFER_BIT);

			ParseEvents();
			
			float currentTicks = static_cast<float>(SDL_GetTicks());
			float lastFrameTicks = currentTicks - mElapsedTicks;
			mElapsedTicks = currentTicks;

			lastFrameTicks = std::min(lastFrameTicks, MaxFrameTicks);
			mLastFrameSeconds = lastFrameTicks * 0.001f;

			// not updating all the time
			if (mUpdater && mLastFrameSeconds > 0.01) {
				mUpdater->Update();
			}
		}
	}

	float Engine::EngineImplementation::getCurrentTime() const {
		return mElapsedTicks;
	}

	void Engine::EngineImplementation::ParseEvents() {
		SDL_Event event;
		while (SDL_PollEvent(&event)) {
			switch (event.type) {
			case SDL_QUIT:
			case SDL_KEYDOWN:
				mQuit = true;
				break;
			case SDL_MOUSEBUTTONDOWN:
				mMouseButtonDown = true;
				break;
			case SDL_MOUSEBUTTONUP:
				mMouseButtonDown = false;
				mMouseButtonUp = true;
				break;
			case SDL_MOUSEMOTION:
				mMouseX = static_cast<float>(event.motion.x);
				mMouseY = static_cast<float>(event.motion.y);
				break;
			default:
				break;
			}
		}
	}

	void Engine::GameGrid::initialize() {
		int column = 0, row = 0;
		float posX = 0.0f, posY = 0.0f;
		std::srand(time(0));
		// nested loop for small grid 8x8
		for (int row = 0; row < GAME_GRID_SIZE_Y; ++row) {
			for (int column = 0; column < GAME_GRID_SIZE_X; ++column) {
				mPositions[column][row].column = posX + column * STONE_SIZE_X;
				mPositions[column][row].row = posY + row * STONE_SIZE_Y;
				King::Engine::Texture color = getRandomStoneColor();
			    while ((row >= 2 && mColors[column][row - 1] == color && mColors[column][row - 2] == color) ||
					   (column >= 2 && mColors[column - 1][row] == color && mColors[column - 2][row] == color)) {
					color = getRandomStoneColor();
				}
				mColors[column][row] = color;
			}
		}
	}

	void Engine::GameGrid::setStonePosition(const int column, const int row, const float mouseX, const float mouseY) {
		mPositions[column][row] = position<float>(mouseX, mouseY);
	}

	const position<float>(&Engine::GameGrid::getStonePositions() const)[GAME_GRID_SIZE_X][GAME_GRID_SIZE_Y] {
		return mPositions;
	}

	void Engine::GameGrid::swapStoneColor(const int column, const int row, const int directionX, const int directionY) {
		// check for writting/reading out of bounds
		if ((row + directionY >= 0 && column + directionX >= 0) &&
			(row + directionY < GAME_GRID_SIZE_X && column + directionX < GAME_GRID_SIZE_Y)) {
			std::swap(mColors[column + directionX][row + directionY], mColors[column][row]);
		}
	}
	
	void Engine::GameGrid::setStoneColor(const int column, const int row, King::Engine::Texture color) {
		// check for writting out of bounds
		if (row >= 0 && row < GAME_GRID_SIZE_Y && column >= 0 && column < GAME_GRID_SIZE_X) {
			mColors[column][row] = color;
		}
	}

	const King::Engine::Texture(&Engine::GameGrid::getStoneColors() const)[GAME_GRID_SIZE_X][GAME_GRID_SIZE_Y] {
		return mColors;
	}
}
