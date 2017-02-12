#include "reducer_world.hpp"
#include "entity_state.hpp"

namespace te {

static void stepVelocities(entitymap_t<glm::vec3>& velocities, entity_t playerEntity, const Uint8 *keyboard) {
	static constexpr float SPEED = 32.0f;
	velocities[playerEntity].x = (keyboard[SDL_SCANCODE_D] - keyboard[SDL_SCANCODE_A]) * SPEED;
}

static void stepTranslations(const entitymap_t<glm::vec3>& velocities,
					  float dt,
					  entitymap_t<glm::vec3>& translations) {
	for (const auto& velocityRow : velocities) {
		translations[velocityRow.first] += velocityRow.second * dt;
	}
}

static void stepAnimators(const animctrlmap_t<animctrl_t>& controllers,
						  const entitymap_t<glm::vec3>& velocities,
						  float dt,
						  entitymap_t<animator_t>& animators) {

	for (auto& animatorRow : animators) {
		animator_t& animator = animatorRow.second;
		animator.elapsed += dt;
		entity_t entityID = animatorRow.first;
		glm::vec3 velocity = velocities.find(entityID)->second;
		const animctrl_t& controller = controllers.find(animator.controller)->second;
		animid_t newAnim;
		if (velocity.x > 0) {
			newAnim = controller.walkRight;
		} else if (velocity.x < 0) {
			newAnim = controller.walkLeft;
		}
		if (newAnim != animator.animation && newAnim.id > 0) {
			animator.animation = newAnim;
			animator.elapsed = 0;
		}
	}
}

static void stepSprites(const entitymap_t<animator_t>& animators,
						const animmap_t<animation_t>& animations,
						entitymap_t<int>& sprites) {
	for (const auto& animatorRow : animators) {
		const auto& animator = animatorRow.second;
		if (animator.animation.id == 0) {
			continue;
		}
		const auto& animation = animations.find(animator.animation)->second;
		int elapsedMS = animator.elapsed * 1000;
		int clampedElapsed = elapsedMS % animation.totalDuration;
		int acc = 0;
		for (const auto& frame : animation.frames) {
			acc += frame.duration;
			if (clampedElapsed < acc) {
				entity_t entityID = animatorRow.first;
				sprites[entityID] = frame.gid;
				break;
			}
		}
	}
}

void stepEntity(entitystate_t& state, float dt, const Uint8 *keyboardState, entity_t playerEntity, const animctrlmap_t<animctrl_t>& animationControllers, const animmap_t<animation_t>& animations) {
	stepVelocities(state.velocities, playerEntity, keyboardState);
	stepTranslations(state.velocities, dt, state.translations);
	stepAnimators(animationControllers, state.velocities, dt, state.animators);
	stepSprites(state.animators, animations, state.tilesetSprites);
}

} // namespace te
