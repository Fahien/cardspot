#include <cstdlib>

#include <boost/array.hpp>
#include <boost/asio.hpp>
#include <fmt/color.h>
#include <fmt/format.h>

#include <thread>

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

namespace asio = boost::asio;
namespace ptime = boost::posix_time;


enum class Command : uint8_t
{
	NOP,
	LIST,
};

struct Message
{
	Command command;
};

class Connection : std::enable_shared_from_this<Connection>
{
  public:
	Connection( asio::io_context& context )
	: socket { context }
	{
		logi( "New connection" );
	}

	~Connection()
	{
		logi( "Closing" ) ;
	}

	void handle_read( const boost::system::error_code& error )
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
		}
		else if ( response.command == Command::NOP )
		{
			logi( "Nothing to do" );
		}
		else
		{
			loge( "Command unknown" );
		}
		socket.close();
	}

	void handle_write()
	{
		logi( "Handle write" );
		// Size is important for this function
		// Callback will be called only when that amount of bytes arrives
		asio::async_read( socket, asio::buffer( &response, sizeof( response ) ),
			std::bind( &Connection::handle_read, this, std::placeholders::_1 ) );
	}

	void handle()
	{
		message = "Server says hello";
		asio::async_write( socket, asio::buffer( message ),
			std::bind( &Connection::handle_write, this ) );
	}

	asio::ip::tcp::socket& get_socket() { return socket; }

  private:
	asio::ip::tcp::socket socket;
	std::string message;
	Message response;
};

class Server
{
  public:
	Server( asio::io_context& context )
	: ctx { context }
	, acceptor { context, asio::ip::tcp::endpoint( asio::ip::tcp::v4(), 13 ) }
	{
		accept();
	}

	void handle( Connection& conn, const boost::system::error_code& error )
	{
		if ( !error )
		{
			logi( "Handling" );
			conn.handle();
		}

		accept();
	}

	void accept()
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
			conn = &connections.emplace_back( std::make_unique<Connection>( ctx ) );
		}
		acceptor.async_accept( (*conn)->get_socket(),
			std::bind( &Server::handle, this, std::ref( *conn->get() ), std::placeholders::_1 ) );
	}

  private:
	asio::io_context& ctx;
	asio::ip::tcp::acceptor acceptor;

	// Connections should not be moved from their position in memory
	// New connections may come in while other connections are still handled
	// When vector resizes, objects get moved. That is why we allocate
	// connections on the heap and use smart pointers as elements of the vector.
	std::vector<std::unique_ptr<Connection>> connections;
};


int main( int argc, const char** argv )
{
	auto ctx = asio::io_context();
	auto server = Server( ctx );
	ctx.run();

	return EXIT_SUCCESS;
}

