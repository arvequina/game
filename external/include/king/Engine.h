// (C) king.com Ltd 2014

#pragma once

#include <glm/fwd.hpp>
#include <memory>

#define animationsMaxTime 250

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
			From_Down,
			From_Up,
			From_Left,
			From_Right
		};

		typedef struct{
			int row;
			int column;
			ActionsFromGestures action;
			Texture incomingColor;
			unsigned int startTime;
			int delay;
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
		void Start(Updater& updater);
		void Quit();

		int GetTextureHeight(Texture texture) const;
		int GetTextureWidth(Texture texture) const;
		void Render(Texture texture, const glm::mat4& transform);
		void Render(Texture texture, float x, float y, float rotation = 0.0f);


		float CalculateStringWidth(const char* text) const;
		void Write(const char* text, const glm::mat4& transform);
		void Write(const char* text, float x, float y, float rotation = 0.0f);

		void fillScene(float timeLeft);
		void doAnimations();
		void addAction(const int row, const int column, const ActionsFromGestures action, const King::Engine::Texture color, int delay = 0);
		void gameOverScene();
		void initializeGrid() const;
		float getCurrentTime() const;

		void setStonePosition(const int row, const int column, const float mouseX, const float mouseY);
		const std::pair<float, float>(&getStonePositions() const)[8][8];
		void swapStoneColor(const int row, const int column, const int directionX, const int directionY);
		void setStoneColor(const int row, const int column, King::Engine::Texture color);
		const King::Engine::Texture(&getStoneColors() const)[8][8];
		void resetStoneVisited();
		void setStoneVisited(const int row, const int column, bool visited);
		const bool(&getStoneVisited() const)[8][8];

		int GetWidth() const;
		int GetHeight() const;

	private:
		struct EngineImplementation;
		class GameGrid;
		std::unique_ptr<EngineImplementation> mPimpl;
	};
}