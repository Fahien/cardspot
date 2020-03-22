#include <cstdlib>

#include <boost/array.hpp>
#include <boost/asio.hpp>
#include <fmt/color.h>
#include <fmt/format.h>

void loge( const std::string& msg )
{
	static std::string fail = fmt::format( fmt::emphasis::bold | fmt::fg( fmt::color::red ), "FAIL" );
	fmt::print( "[{}] {}\n", fail, msg );
}

namespace asio = boost::asio;

int main( int argc, const char** argv )
{
	auto io = asio::io_service();

	auto acceptor = asio::ip::tcp::acceptor( io, asio::ip::tcp::endpoint( asio::ip::tcp::v4(), 13 ) );

	while ( true )
	{
		auto timer = asio::deadline_timer( io, boost::posix_time::seconds( 5 ) );

		auto socket = asio::ip::tcp::socket( io );
		acceptor.accept( socket );

		timer.wait();
		std::string message = "we";
		boost::system::error_code ignored_error;
		boost::asio::write( socket, asio::buffer( message ), ignored_error );
	}

	return EXIT_SUCCESS;
}

