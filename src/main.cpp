#include "game_state.hpp"
#include "sprite_renderer.hpp"
#include "text_renderer.hpp"
#include "game_values.hpp"
#include "game_action.hpp"
#include "util.hpp"
#include "world_state.hpp"
#include "sound_effect_state.hpp"
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
		chunkptr_t pCoinChunk(Mix_LoadWAV(COIN_SOUND), chunkdeleter_t());
		loadSound(gameState, eastl::move(pCoinChunk), COIN_SOUND);

		chunkptr_t pJumpChunk(Mix_LoadWAV(JUMP_SOUND), chunkdeleter_t());
		loadSound(gameState, eastl::move(pJumpChunk), JUMP_SOUND);
	}

	const char *tmxPathname = "tiled/1-1.tmx";
	tmx_t tmx(tmxPathname);
	assert(tmx.tilesets.size() == 0);

	auto musicIt = tmx.properties.find("music");
	if (musicIt != tmx.properties.end()) {
		std::string musicStr = "tiled/" + musicIt->second;
		musicptr_t pMusic(Mix_LoadMUS(musicStr.c_str()), musicdeleter_t());
		loadMusic(gameState, eastl::move(pMusic), musicStr.c_str());
	}

	auto musicDieIt = tmx.properties.find("music-die");
	if (musicDieIt != tmx.properties.end()) {
		std::string musicStr = "tiled/" + musicDieIt->second;
		if (gSound.soundID.find_as(musicStr.c_str()) == gSound.soundID.end()) {
			chunkptr_t pChunk(Mix_LoadWAV(musicStr.c_str()), chunkdeleter_t());
			loadSound(gameState, eastl::move(pChunk), musicStr.c_str());
		}
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
				if (gSound.soundID.find_as(wavPathname.c_str()) == gSound.soundID.end()) {
					chunkptr_t pChunk(Mix_LoadWAV(wavPathname.c_str()), chunkdeleter_t());
					loadSound(gameState, eastl::move(pChunk), wavPathname.c_str());
				}
			}
		}
	}
	loadGame(gameState, tmx, tmxPathname);

	levelid_t levelID = gameState.level.source.find(tmxPathname)->second;
	runGame(gameState, levelID);

	SpriteRenderer spriteRenderer;
	TextRenderer textRenderer;
	
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

		for (soundid_t soundID : gWorld.soundQueue) {
			Mix_Chunk *pChunk = getChunk(soundID, gameState);
			Mix_PlayChannel(-1, pChunk, 0);
		}
		flushSoundQueue();

		for (auto musicCmd : gSoundEffect.musicCommandQueue) {
			switch (musicCmd.first) {
			case musiccmd_t::PAUSE:
				Mix_PauseMusic();
				break;
			case musiccmd_t::PLAY:
				Mix_Music *pMusic = getMusic(musicCmd.second, gameState);
				Mix_PlayMusic(pMusic, -1);
			}
		}
		flushMusicCommandQueue();

		glClearColor(0, 0, 0, 1.f);
		glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
		spriteRenderer.draw(gameState);
		std::string scoreStr = std::to_string(gWorld.score);
		textRenderer.draw(scoreStr.c_str(), {0, FONT_PIXEL_SIZE}, gWorld.projection);
		SDL_GL_SwapWindow(upWindow.get());
	}

	return 0;
}

} // namespace te

int main(int, char **) {
	return te::MarioMain();
}
