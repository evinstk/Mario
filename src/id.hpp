#pragma once

namespace te {

enum class id_type_t {
	ENTITY,
	ANIMATION,
	ANIMATION_CONTROLLER,
	COLLIDER
};

template <id_type_t I, typename T = int>
struct id_t {
	T id;
	id_t() : id( 0 ) {}
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
};

using entity_t     = id_t<id_type_t::ENTITY>;
using animid_t     = id_t<id_type_t::ANIMATION>;
using animctrlid_t = id_t<id_type_t::ANIMATION_CONTROLLER>;
using colliderid_t = id_t<id_type_t::COLLIDER>;

} // namespace te
