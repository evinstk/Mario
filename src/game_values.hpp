#pragma once

namespace te {

constexpr int   SOUND_FREQUENCY       = 44100;
constexpr int   SOUND_CHANNEL_NUM     = 2;
constexpr int   SOUND_SAMPLE_SIZE     = 2048;

constexpr int   COIN_SCORE            = 200;
constexpr int   COIN_COUNT_LIVE_UP    = 100;
constexpr int   LIVE_COUNT_START      = 3;
constexpr float COIN_BOUNCE_DURATION  = 0.5f;
constexpr float COIN_BOUNCE_HEIGHT    = 16.0f * 3.0f;
constexpr float BLOCK_BOUNCE_DURATION = 0.2f;
constexpr float BLOCK_BOUNCE_HEIGHT   = 8.0f;

extern const char *COIN_SOUND;
extern const char *JUMP_SOUND;

} // namespace te
