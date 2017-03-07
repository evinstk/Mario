#pragma once
#include "id.hpp"
#include "types.hpp"
#include "entity_state.hpp"
#include <tegl/types.hpp>
#include <EASTL/vector_map.h>
#include <EASTL/vector.h>
#include <EASTL/vector_set.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_integer.hpp>
#include <GL/glew.h>

namespace te {

struct gamestate_t;

struct worldstate_t {
	worldmode_t mode;
	float modeElapsed;

	glm::mat4 projection;
	glm::imat4 view;

	entitystate_t entity;
	int nextEntityID;
	entityset_t freeIDs;

	levelid_t level;
	vector_t<layer_t> layers;

	entityid_t playerEntity;
	int score;
	int coinCount;
	int lives;

	bool deathTrigger;

	vector_t<entityrequest_t> newEntityQueue;
	vector_t<entityid_t> newEntityIDs;
	entityset_t destroyQueue;

	void run(levelid_t levelID);
	void dispatch();
	void input(const Uint8 *keyboardState);
	void step(float dt);
	void makeEntity();
	void destroyEntity();

	worldstate_t(const gamestate_t& g);

private:
	const gamestate_t *pGame;

	void runNextEntityID(int& state, levelid_t levelID) const;
	void runPlayerEntity(entityid_t& playerEntity, levelid_t levelID) const;

	void stepMode(worldmode_t& modeState, float& elapsedState, float dt) const;
	void stepScore(int& score, int& coinCount, int& lives) const;
	void stepDeathTrigger(bool& state) const;
	void stepView(glm::imat4& state, float dt) const;
	void stepNewEntityQueue(vector_t<entityrequest_t>& state) const;
	void stepDestroyQueue(entityset_t& state) const;

	void destroyEntityFreeIDs(entityset_t& state) const;
};

} // namespace te
