#include <cstdlib>
#include <thread>

#include <boost/array.hpp>
#include <boost/asio.hpp>
#include <fmt/color.h>
#include <fmt/format.h>

#include "log.h"
#include "message.h"
#include "connection.h"


#define BUF( buf ) ( asio::buffer( &buf, sizeof( buf ) ) )
#define STR( arr ) ( std::string( std::begin( arr ), std::end( arr ) ) )

namespace asio = boost::asio;


int main( int argc, const char** argv )
{
	if ( argc != 2 )
	{
		loge( fmt::format( "Usage: {} <host>", argv[0] ) );
		return EXIT_FAILURE;
	}

	auto io = asio::io_context();
	auto resolver = asio::ip::tcp::resolver( io );
	auto endpoints = resolver.resolve( argv[1], "daytime" );

	for ( auto& endpoint : endpoints )
	{
		logi( fmt::format( "{} {} {}::{}", endpoint.host_name(), endpoint.service_name(), endpoint.endpoint().address().to_string(), endpoint.endpoint().port() ) );
	}

	auto con = Connection( io, endpoints );
	auto io_thread = std::thread( [&io] () { io.run(); } );


	while ( true )
	{
		std::this_thread::sleep_for( std::chrono::seconds( 1 ) );
		log_up( "List" );
		boost::system::error_code error;
		auto message = Message{ Command::LIST };
		con.send( message );
	}

	return EXIT_SUCCESS;
}
