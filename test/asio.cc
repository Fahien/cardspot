#include <cstdlib>
#include <thread>

#include <boost/asio.hpp>
#include <fmt/format.h>
#include <fmt/color.h>

void success( const std::string& msg )
{
	static std::string ok = fmt::format( fmt::emphasis::bold | fmt::fg( fmt::color::green ), "OK" );
	fmt::print( "[ {} ] {}\n", ok, msg );
}

namespace asio = boost::asio;
namespace ptime = boost::posix_time;

class Printer
{
  public:
	Printer( asio::io_context& io ) : strand( io ), timer1( io, ptime::seconds( 1 ) ), timer2( io, ptime::seconds( 1 ) )
	{
		timer1.async_wait(
			asio::bind_executor( strand, std::bind( &Printer::callback1 , this, std::placeholders::_1 ) )
		);
		timer2.async_wait(
			asio::bind_executor( strand, std::bind( &Printer::callback2 , this, std::placeholders::_1 ) )
		);
	}

  private:
	asio::io_service::strand strand;
	asio::deadline_timer timer1;
	asio::deadline_timer timer2;

	int count = 0;

	void callback1( const boost::system::error_code& )
	{
		if ( count < 5 )
		{
			success( fmt::format( "Timer1 {}", count ) );
			++count;
			timer1.expires_at( timer1.expires_at() + ptime::seconds( 1 ) );
			timer1.async_wait( std::bind( &Printer::callback1, this, std::placeholders::_1 ) );
		}
	}

	void callback2( const boost::system::error_code& )
	{
		if ( count < 5 )
		{
			success( fmt::format( "Timer2 {}", count ) );
			++count;
			timer2.expires_at( timer2.expires_at() + ptime::seconds( 1 ) );
			timer2.async_wait( std::bind( &Printer::callback2, this, std::placeholders::_1 ) );
		}
	}

};

int main()
{
	asio::io_context io;
	Printer printer( io );

	auto thr = std::thread( [&io](){ io.run(); } );
	io.run();
	thr.join();
	success( "Timer started" );
	
	return EXIT_SUCCESS;
}
