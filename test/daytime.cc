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

enum class Command : uint8_t
{
	NOP,
	LIST,
};

struct Message
{
	Command command;
};


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
		auto buf = boost::array<char, 128>();
		boost::system::error_code error;
		logi( "Reading" );
		auto len = socket.read_some( asio::buffer( buf ), error );
		if ( error == asio::error::eof )
		{
			break; // connection closed
		}
		else if ( error )
		{
			loge( fmt::format( "Read error: {}", error.message() ) );
			return EXIT_FAILURE;
		}

		fmt::print( "{}\n", std::string( buf.data(), len ) );

		logi( "Writing" );
		boost::system::error_code werror;
		auto message = Message{ Command::NOP };
		socket.write_some( asio::buffer( &message, sizeof( message ) ), werror );
		if ( werror )
		{
			loge( fmt::format( "Write error: {}", error.message() ) );
			return EXIT_FAILURE;
		}
	}

	return EXIT_SUCCESS;
}
