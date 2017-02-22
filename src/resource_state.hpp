#pragma once
#include "types.hpp"
#include <EASTL/vector_map.h>

namespace te {

template <typename ID, typename R>
struct resourcestate_t {
	int nextID;
	stringmap_t<ID> id;
	eastl::vector_map<ID, R> resource;

	resourcestate_t() : nextID(1) {}
};

template <typename ID, typename R>
void insert(resourcestate_t<ID, R> state, const char *strID, R&& resource) {
	if (state.find_as(strID) != state.end()) {
		return; // already registered
	}

	ID id(state.nextID++);
	state.id.insert({ eastl::string(strID), id });
	state.resource.insert({ id, std::forward<R>(resource) });
}

} // namespace te
