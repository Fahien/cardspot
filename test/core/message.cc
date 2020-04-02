#include "message.h"

#include <cassert>


Message Message::hi( const std::string& name )
{
	assert( name.size() <= 28 && "Name should not be longer than 28" );
	const uint16_t version = 1;
	Message m { Command::HI, version };
	std::memcpy( m.u.hi.name.data(), name.data(), name.size() );
	return m;
}


Message Message::str( const std::string& str )
{
	assert( str.size() <= 28 && "String should not be longer than 28" );
	const uint16_t version = 1;
	Message m { Command::STR, version };
	std::memcpy( m.u.str.data(), str.data(), str.size() );
	return m;
}
