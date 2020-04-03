#pragma once

#include <boost/asio.hpp>

#include <message.h>


class Connection : std::enable_shared_from_this<Connection>
{
  public:
	Connection( boost::asio::io_context& ctx,
		boost::asio::ip::tcp::resolver::results_type& endpoints );

	void handle_read( const boost::system::error_code& error );

	void read();

	void handle_write();

	void send( const Message& message );

	boost::asio::ip::tcp::socket& get_socket() { return socket; }

  private:
	boost::asio::ip::tcp::socket socket;
	Message message;
	Message response;
};
