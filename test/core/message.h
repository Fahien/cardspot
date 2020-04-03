#pragma once

#include <string>
#include <array>


enum class Command : uint16_t
{
	NOP,
	HI,
	STR,
	LIST,
};

struct Message
{
	static Message hi( const std::string& name );
	static Message str( const std::string& str );

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
	} u = {};
};

static_assert( sizeof( Message ) == 32 );
