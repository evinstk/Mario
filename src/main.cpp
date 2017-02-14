#include "game_state.hpp"
#include "sprite_renderer.hpp"
#include "reducer_world.hpp"
#include <tegl/readtmx.hpp>
#include <GL/glew.h>
#include <SDL.h>
#include <SDL_opengl.h>
#include <IL/il.h>
#include <IL/ilu.h>
#include <memory>
#include <iostream>
#include <cassert>

namespace te {

struct Libs {
	Libs() {
		SDL_Init(SDL_INIT_VIDEO);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

		ilInit();
		iluInit();
		ilClearColour(255, 255, 255, 0);
		assert(ilGetError() == IL_NO_ERROR);
	}
	~Libs() { SDL_Quit(); }
};

static int MarioMain() {
	Libs libs;
	std::unique_ptr<SDL_Window, decltype(&SDL_DestroyWindow)> upWindow(
		SDL_CreateWindow("Super Mario Bros.",
						 SDL_WINDOWPOS_CENTERED,
						 SDL_WINDOWPOS_CENTERED,
						 1280,
						 720,
						 SDL_WINDOW_SHOWN|SDL_WINDOW_OPENGL),
		SDL_DestroyWindow);

	SDL_GLContext context = SDL_GL_CreateContext(upWindow.get());
	if (!context) {
		std::cerr << "Could not initialize GL context: " << SDL_GetError() << std::endl;
		return -1;
	}

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_DEPTH_TEST);

	glewExperimental = GL_TRUE;
	assert(glewInit() == GLEW_OK);

	gamestate_t gameState;

	const char *tmxPathname = "tiled/1-1.tmx";
	tmx_t tmx(tmxPathname);
	assert(tmx.tilesets.size() == 0);
	for (const auto& externalTileset : tmx.externalTilesets) {
		std::string tilesetPathname = "tiled/" + externalTileset.source;
		tsxtileset_t tileset(tilesetPathname.c_str());
		loadGame(gameState, tileset, tilesetPathname.c_str());
	}
	loadGame(gameState, tmx, tmxPathname);

	levelid_t levelID = gameState.level.source.find(tmxPathname)->second;
	runGame(gameState, levelID);

	SpriteRenderer spriteRenderer;
	
	constexpr float timePerFrame = 1.0f / 60.0f;
	float timeSinceLastUpdate = 0.0f;
	Uint64 t0 = SDL_GetPerformanceCounter();
	bool running = true;
	while (running) {
		Uint64 now = SDL_GetPerformanceCounter();
		float dt = (now - t0) / (float)SDL_GetPerformanceFrequency();
		t0 = now;
		timeSinceLastUpdate += dt;
		while (timeSinceLastUpdate > timePerFrame) {
			timeSinceLastUpdate -= timePerFrame;
			SDL_Event evt;
			while (SDL_PollEvent(&evt)) {
				if (evt.type == SDL_QUIT) {
					running = false;
				}
				inputGame(gameState, evt);
			}
			stepGame(gameState, timePerFrame, SDL_GetKeyboardState(NULL));
		}
		glClearColor(0, 0, 0, 1.f);
		glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
		spriteRenderer.draw(gameState);
		SDL_GL_SwapWindow(upWindow.get());
	}

	return 0;
}

} // namespace te

int main(int, char **) {
	return te::MarioMain();
}
