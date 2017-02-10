#include "world_state.hpp"
#include "action.hpp"
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

static void MarioMain() {
	Libs libs;
	std::unique_ptr<SDL_Window, decltype(&SDL_DestroyWindow)> upWindow(
		SDL_CreateWindow("Super Mario Bros.",
						 SDL_WINDOWPOS_CENTERED,
						 SDL_WINDOWPOS_CENTERED,
						 1024,
						 768,
						 SDL_WINDOW_SHOWN|SDL_WINDOW_OPENGL),
		SDL_DestroyWindow);

	SDL_GLContext context = SDL_GL_CreateContext(upWindow.get());
	if (!context) {
		std::cerr << "Could not initialize GL context: " << SDL_GetError() << std::endl;
	}

	glewExperimental = GL_TRUE;
	assert(glewInit() == GLEW_OK);

	worldstate_t worldState;

	tmx_t tmx("tiled/1-1.tmx");
	action_t loadAction = {
		.type = actiontype_t::LOAD_LEVEL,
		.payload = {
			.tmx = &tmx
		}
	};
	reduceWorld(worldState, loadAction, worldState);

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
			}
			action_t action = {
				.type = actiontype_t::STEP,
				.payload = {
					.dt = timePerFrame
				}
			};
			reduceWorld(worldState, action, worldState);
		}
		glClearColor(0, 0, 0, 1.f);
		glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
		spriteRenderer.draw(worldState);
		SDL_GL_SwapWindow(upWindow.get());
	}
}

} // namespace te

int main(int, char **) {
	te::MarioMain();
	return 0;
}
