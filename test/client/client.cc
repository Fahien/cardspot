#include <cstdlib>

#include <boost/array.hpp>
#include <boost/asio.hpp>
#include <fmt/color.h>
#include <fmt/format.h>

#include "log.h"
#include "message.h"

#include <thread>

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

	auto io = asio::io_service();
	auto resolver = asio::ip::tcp::resolver( io );
	auto endpoints = resolver.resolve( argv[1], "daytime" );

	for ( auto& endpoint : endpoints )
	{
		fmt::print( "{} {} {}::{}\n", endpoint.host_name(), endpoint.service_name(), endpoint.endpoint().address().to_string(), endpoint.endpoint().port() );
	}

	auto socket = asio::ip::tcp::socket( io );
	try
	{
		asio::connect( socket, endpoints );
	}
	catch ( const std::exception& e )
	{
		loge( e.what() );
		return EXIT_FAILURE;
	}

	while ( true )
	{
		auto message = Message();
		boost::system::error_code error;
		logi( "Reading" );
		auto len = socket.read_some( BUF( message ), error );
		if ( error == asio::error::eof )
		{
			break; // connection closed
		}
		else if ( error )
		{
			loge( fmt::format( "Read error: {}", error.message() ) );
			return EXIT_FAILURE;
		}

		switch ( message.command )
		{
		case Command::HI:
			log_down( fmt::format( "{} v{}", STR( message.u.hi.name ), message.version ) );
			break;
		case Command::STR:
			log_down( STR( message.u.str ) );
			return EXIT_SUCCESS;
		default:
			loge( "Command unknown" );
			return EXIT_FAILURE;
		}

		std::this_thread::sleep_for( std::chrono::seconds( 1 ) );

		log_up( "List" );
		boost::system::error_code werror;
		message = Message{ Command::LIST };
		socket.write_some( BUF( message ), werror );
		if ( werror )
		{
			loge( fmt::format( "Write error: {}", error.message() ) );
			return EXIT_FAILURE;
		}
	}

	return EXIT_SUCCESS;
}
