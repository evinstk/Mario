#include "sprite_renderer.hpp"

namespace te {

std::unique_ptr<SpriteRenderer> SpriteRenderer::m_instance = nullptr;

SpriteRenderer *SpriteRenderer::get() {
	if (!m_instance) {
		m_instance = std::make_unique<SpriteRenderer>();
	}
	return m_instance.get();
}

} // namespace te
