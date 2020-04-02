#include "connection.h"

#include <message.h>

#include "server.h"
#include "log.h"

#define BUF( buf ) ( asio::buffer( &buf, sizeof( buf ) ) )

namespace asio = boost::asio;
namespace ptime = boost::posix_time;


Connection::Connection( Server& srv )
: server { srv }
, socket { srv.get_context() }
{
	logi( "New connection" );
}


void Connection::handle_read( const boost::system::error_code& error )
{
	logi( "Handling read" );
	if ( error == asio::error::eof )
	{
		logi( "Client disconnected" );
		socket.close();
		return;
	}

	if ( response.command == Command::LIST )
	{
		logi( "Command LS received" );
		std::string list;
		for ( auto& cons : server.get_connections() )
		{
			if ( cons->socket.is_open() )
			{
				list += cons->socket.local_endpoint().address().to_string() + '\n';
			}
		}
		message = Message::str( list );
		asio::async_write( socket, BUF( message ),
			std::bind( &Connection::handle_write, this ) );
	}
	else if ( response.command == Command::NOP )
	{
		logi( "Nothing to do" );
	}
	else
	{
		loge( "Command unknown" );
		socket.close();
	}
}

void Connection::handle_write()
{
	logi( "Handle write" );
	// Size is important for this function
	// Callback will be called only when that amount of bytes arrives
	asio::async_read( socket, BUF( response ),
		std::bind( &Connection::handle_read, this, std::placeholders::_1 ) );
}

void Connection::handle()
{
	message = Message::hi( "cardspot-server" );
	asio::async_write( socket, BUF( message ),
		std::bind( &Connection::handle_write, this ) );
}

