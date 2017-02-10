#pragma once

#include "world_state.hpp"
#include "reducer_world.hpp"

namespace te {

class BufferedWorldState {
public:
	BufferedWorldState();

	template <typename... Args>
	void step(Args&&... args) {
		reduceWorld(*m_readState, std::forward<Args>(args)..., *m_writeState);
		std::swap(m_readState, m_writeState);
	}

	const worldstate_t& state() const noexcept {
		return *m_readState;
	}
private:
	worldstate_t m_state1;
	worldstate_t m_state2;

	worldstate_t *m_readState;
	worldstate_t *m_writeState;
};

}
