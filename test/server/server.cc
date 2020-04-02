#include <cstdlib>
#include <thread>

#include <boost/array.hpp>
#include <boost/asio.hpp>

#include "server.h"
#include "connection.h"
#include "log.h"

namespace asio = boost::asio;
namespace ptime = boost::posix_time;


Server::Server( asio::io_context& c )
: ctx { c }
, acceptor { c, asio::ip::tcp::endpoint( asio::ip::tcp::v4(), 13 ) }
{
	accept();
}


void Server::handle( Connection& conn, const boost::system::error_code& error )
{
	if ( !error )
	{
		logi( "Handling" );
		conn.handle();
	}

	accept();
}


void Server::accept()
{
	logi( "Accepting" );
	std::unique_ptr<Connection>* conn = nullptr;

	for ( auto& c : connections )
	{
		if ( !c->get_socket().is_open() )
		{
			conn = &c;
			break;
		}
	}

	if ( !conn )
	{
		conn = &connections.emplace_back( std::make_unique<Connection>( *this ) );
	}
	acceptor.async_accept( (*conn)->get_socket(),
		std::bind( &Server::handle, this, std::ref( *conn->get() ), std::placeholders::_1 ) );
}


int main( int argc, const char** argv )
{
	auto ctx = asio::io_context();
	auto server = Server( ctx );
	ctx.run();

	return EXIT_SUCCESS;
}

