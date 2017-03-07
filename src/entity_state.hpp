#pragma once
#include "types.hpp"
#include <EASTL/vector_map.h>
#include <glm/vec3.hpp>

namespace te {

struct gamestate_t;

struct entitystate_t {
	entitymap_t<colliderid_t> colliders;
	entitymap_t<float> wallOffsets;
	entitymap_t<float> groundOffsets;
	entitymap_t<float> ceilingOffsets;

	entityset_t               hitGround;
	entitymap_t<bounceanim_t> bounceAnimations;
	entitymap_t<glm::vec3>    spriteOffsets;

	entitymap_t<soundid_t> bounceSounds;

	entityset_t underGravity;
	entityset_t isGround;

	entitymap_t<prize_t> prizes;
	entitymap_t<int> prizeNum;

	entitymap_t<glm::vec3> velocities;
	entitymap_t<glm::vec3> translations;

	entitymap_t<animid_t> animationsLeft;
	entitymap_t<animid_t> animationsRight;
	entitymap_t<spriteanimator_t> spriteAnimators;
	entitymap_t<tileid_t> tilesetSprites;

	entityset_t canBounce;
	entitymap_t<int> bounceNum;

	entitymap_t<float> lifetimes;

	void run(levelid_t levelID);
	void input(const Uint8 *keyboardState);
	void step(float dt);
	void makeEntity();
	void destroyEntity();

	const gamestate_t *pGame;
	entitystate_t(const gamestate_t& g) : pGame(&g) {}

private:
	void stepWallOffsets(entitymap_t<float>& state, float dt) const;
	void stepColliders(entitymap_t<float>& groundOffsets, float dt) const;
	void stepCeilingOffsets(entitymap_t<float>& ceilingOffsets, entityset_t& hitGround, float dt) const;
	void stepBounceAnimations(entitymap_t<bounceanim_t>& state, float dt) const;
	void stepSpriteOffsets(entitymap_t<glm::vec3>& state) const;
	void stepVelocities(entitymap_t<glm::vec3>& velocities) const;
	void stepTranslations(entitymap_t<glm::vec3>& state, float dt) const;
	void stepSpriteAnimators(entitymap_t<spriteanimator_t>& state, float dt) const;
	void stepSprites(entitymap_t<tileid_t>& state) const;
	void stepBounceNum(entitymap_t<int>& state) const;
	void stepCanBounce(entityset_t& state) const;
	void stepPrizeNum(entitymap_t<int>& state) const;
	void stepPrizes(entitymap_t<prize_t>& state) const;

	void makeEntityTranslations(entitymap_t<glm::vec3>& state) const;
	void makeEntityBounceAnimations(entitymap_t<bounceanim_t>& state) const;
	void makeEntityLifetimes(entitymap_t<float>& state) const;
	void makeEntityAnimators(entitymap_t<spriteanimator_t>& state) const;

	void destroyEntityTranslations(entitymap_t<glm::vec3>& state) const;
	void destroyEntityLifetimes(entitymap_t<float>& state) const;
};

} // namespace te
