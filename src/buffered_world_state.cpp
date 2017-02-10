#include "buffered_world_state.hpp"
#include <utility>

namespace te {

BufferedWorldState::BufferedWorldState()
	: m_state1()
	, m_state2()
	, m_readState(&m_state1)
	, m_writeState(&m_state2)
{}

} // namespace te
