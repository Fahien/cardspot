#include "connection.h"

#include <message.h>
#include <fmt/color.h>
#include <fmt/format.h>

#include <log.h>

#define BUF( buf ) ( asio::buffer( &buf, sizeof( buf ) ) )
#define STR( arr ) ( std::string( std::begin( arr ), std::end( arr ) ) )


namespace asio = boost::asio;
namespace ptime = boost::posix_time;


Connection::Connection( asio::io_context& ctx, asio::ip::tcp::resolver::results_type& endpoints )
: socket { ctx }
{
	logi( "Connecting" );
	asio::connect( socket, endpoints );
	read();
}


void Connection::handle_read( const boost::system::error_code& error )
{
	logi( "Handling read" );
	if ( error == asio::error::eof )
	{
		logi( "Server disconnected" );
		socket.close();
		return;
	}
	else if ( error )
	{
		loge( fmt::format( "Read error: {}", error.message() ) );
		socket.close();
		return;
	}

	switch ( response.command )
	{
	case Command::HI:
		log_down( fmt::format( "{} v{}", STR( response.u.hi.name ), response.version ) );
		break;
	case Command::STR:
		log_down( STR( response.u.str ) );
		break;
	default:
		loge( "Command unknown" );
	}

	// Keep reading messages
	read();
}


void Connection::read()
{
	logi( "Reading" );
	asio::async_read( socket, BUF( response ),
		std::bind( &Connection::handle_read, this, std::placeholders::_1 ) );
}


void Connection::handle_write()
{
	logi( "Handle write" );
	// Size is important for this function
	// Callback will be called only when that amount of bytes arrives
}


void Connection::send( const Message& msg )
{
	message = msg;
	asio::async_write( socket, BUF( message ),
		std::bind( &Connection::handle_write, this ) );
}

