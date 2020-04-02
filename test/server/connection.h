#pragma once

#include <boost/asio.hpp>

#include <message.h>


class Server;

class Connection : std::enable_shared_from_this<Connection>
{
  public:
	Connection( Server& srv );

	void handle_read( const boost::system::error_code& error );

	void handle_write();

	void handle();

	boost::asio::ip::tcp::socket& get_socket() { return socket; }

  private:
	Server& server;
	boost::asio::ip::tcp::socket socket;
	Message message;
	Message response;
};
