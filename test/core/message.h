#pragma once

#include <string>
#include <array>


enum class Command : uint16_t
{
	NOP,
	HI,
	STR,
	LIST,
	NODE,
};

struct Message
{
	static Message hi( const std::string& name );
	static Message str( const std::string& str );
	static Message node( int32_t node );

	Command command = Command::NOP;
	uint16_t version = 0;
	
	union Data
	{
		uint8_t data[28] = {};

		std::array<char, 28> str;

		struct Hi
		{
			std::array<char, 28> name;
		} hi;

		int32_t node;

	} u = {};
};

static_assert( sizeof( Message ) == 32 );
