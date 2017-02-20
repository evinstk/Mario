#pragma once
#include <EASTL/tuple.h>

namespace te {

enum class id_type_t {
	ENTITY,
	ANIMATION,
	ANIMATION_CONTROLLER,
	COLLIDER,
	TILESET,
	SOUND,
	MUSIC,
	LEVEL,
	LEVEL_OBJECT,
	TILE
};

template <id_type_t I, typename T = int>
struct id_t {
	T id;
	id_t() = default;
	explicit id_t( T _id ) : id( _id ) {}
	bool operator<( const id_t& rhs ) const noexcept {
		return id < rhs.id;
	}
	bool operator==( const id_t& rhs ) const noexcept {
		return id == rhs.id;
	}
	bool operator!=( const id_t& rhs ) const noexcept {
		return !( id == rhs.id );
	}
	id_t next() const noexcept {
		return id_t(id + 1);
	}
	//template <typename F>
	//bool operator<( const F& rhs ) const noexcept {
	//	return id.first < rhs;
	//}
};

//template <id_type_t I, typename F, typename S>
//bool operator<( const F& lhs, id_t<I, eastl::pair<F,S>> id ) noexcept {
//	return lhs < id.first;
//}

using entityid_t      = id_t<id_type_t::ENTITY>;
using colliderid_t    = id_t<id_type_t::COLLIDER>;
using tilesetid_t     = id_t<id_type_t::TILESET>;
using soundid_t       = id_t<id_type_t::SOUND>;
using musicid_t       = id_t<id_type_t::MUSIC>;
using levelid_t       = id_t<id_type_t::LEVEL>;
using levelobjectid_t = id_t<id_type_t::LEVEL_OBJECT, eastl::pair<levelid_t, int>>;
using animid_t        = id_t<id_type_t::ANIMATION, eastl::pair<tilesetid_t, int>>;
using animctrlid_t    = id_t<id_type_t::ANIMATION_CONTROLLER>;
using tileid_t        = id_t<id_type_t::TILE, eastl::pair<tilesetid_t, int>>;

} // namespace te
