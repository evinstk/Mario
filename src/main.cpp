#include "game_state.hpp"
#include "sprite_renderer.hpp"
#include "game_values.hpp"
#include "game_action.hpp"
#include "util.hpp"
#include <tegl/readtmx.hpp>
#include <GL/glew.h>
#include <SDL.h>
#include <SDL_opengl.h>
#include <SDL_mixer.h>
#include <IL/il.h>
#include <IL/ilu.h>
#include <memory>
#include <iostream>
#include <cassert>

namespace te {

struct Libs {
	Libs() {
		SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

		Mix_OpenAudio(SOUND_FREQUENCY, MIX_DEFAULT_FORMAT, SOUND_CHANNEL_NUM, SOUND_SAMPLE_SIZE);

		ilInit();
		iluInit();
		ilClearColour(255, 255, 255, 0);
		assert(ilGetError() == IL_NO_ERROR);
	}
	~Libs() {
		Mix_Quit();
		SDL_Quit();
	}
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

	{
		std::unique_ptr<Mix_Chunk, decltype(&Mix_FreeChunk)> pCoinChunk(Mix_LoadWAV(COIN_SOUND),
																		&Mix_FreeChunk);
		loadSound(gameState, std::move(pCoinChunk), COIN_SOUND);
	}

	const char *tmxPathname = "tiled/1-1.tmx";
	tmx_t tmx(tmxPathname);
	assert(tmx.tilesets.size() == 0);

	auto musicIt = tmx.properties.find("music");
	if (musicIt != tmx.properties.end()) {
		std::string musicStr = "tiled/" + musicIt->second;
		musicptr_t pMusic(Mix_LoadMUS(musicStr.c_str()), Mix_FreeMusic);
		loadMusic(gameState, std::move(pMusic), musicStr.c_str());
	}

	for (const auto& externalTileset : tmx.externalTilesets) {
		std::string tilesetPathname = "tiled/" + externalTileset.source;
		tsxtileset_t tileset(tilesetPathname.c_str());
		loadGame(gameState, tileset, tilesetPathname.c_str());
	}
	for (const auto& group : tmx.objectgroups) {
		for (const auto& object : group.objects) {
			auto soundProp = object.properties.find("bounce-sound");
			if (soundProp != object.properties.end()) {
				std::string wavPathname = "tiled/" + soundProp->second;
				if (gameState.sound.soundID.find_as(wavPathname.c_str()) == gameState.sound.soundID.end()) {
					std::unique_ptr<Mix_Chunk, decltype(&Mix_FreeChunk)> pChunk(Mix_LoadWAV(wavPathname.c_str()),
																				&Mix_FreeChunk);
					loadSound(gameState, std::move(pChunk), wavPathname.c_str());
				}
			}
		}
	}
	loadGame(gameState, tmx, tmxPathname);

	levelid_t levelID = gameState.level.source.find(tmxPathname)->second;
	runGame(gameState, levelID);
	{
		auto musicIt = gameState.level.music.find(levelID);
		if (musicIt != gameState.level.music.end()) {
			Mix_Music *pMusic = getMusic(musicIt->second, gameState);
			Mix_PlayMusic(pMusic, -1);
		}
	}

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
				processGame(gameState, evt);
			}
			inputGame(gameState, SDL_GetKeyboardState(NULL));
			stepGame(gameState, timePerFrame);
			destroyEntity(gameState);
			makeEntity(gameState);
		}

		for (soundid_t soundID : gameState.world.soundQueue) {
			Mix_Chunk *pChunk = getChunk(soundID, gameState);
			Mix_PlayChannel(-1, pChunk, 0);
		}
		flushSoundQueue(gameState);

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
