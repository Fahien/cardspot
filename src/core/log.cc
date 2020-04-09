#include "spot/core/log.h"

#include <fmt/color.h>
#include <fmt/format.h>


void loge( const std::string& msg )
{
	static std::string fail = fmt::format( fmt::emphasis::bold | fmt::fg( fmt::color::red ), "FAIL" );
	fmt::print( "[{}] {}\n", fail, msg );
}


void logi( const std::string& msg )
{
	static std::string ok = fmt::format( fmt::emphasis::bold | fmt::fg( fmt::color::green ), " OK " );
	fmt::print( "[{}] {}\n", ok, msg );
}


void log_up( const std::string& msg )
{
	static std::string up = fmt::format( fmt::emphasis::bold | fmt::fg( fmt::color::white ), "↑" );
	static std::string down = fmt::format( fmt::emphasis::bold | fmt::fg( fmt::color::dark_slate_gray ), "↓" );
	fmt::print( "[ {}{} ] {}\n", up, down, msg );
}

void log_down( const std::string& msg )
{
	static std::string up = fmt::format( fmt::emphasis::bold | fmt::fg( fmt::color::dark_slate_gray ), "↑" );
	static std::string down = fmt::format( fmt::emphasis::bold | fmt::fg( fmt::color::white ), "↓" );
	fmt::print( "[ {}{} ] {}\n", up, down, msg );
}


