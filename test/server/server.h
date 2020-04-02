#pragma once

#include <boost/asio.hpp>

class Connection;

class Server
{
  public:
	Server( boost::asio::io_context& c );

	void handle( Connection& conn, const boost::system::error_code& error );

	void accept();

	boost::asio::io_context& get_context() { return ctx; }
	const std::vector<std::unique_ptr<Connection>>& get_connections() const { return connections; }

  private:
	boost::asio::io_context& ctx;
	boost::asio::ip::tcp::acceptor acceptor;

	// Connections should not be moved from their position in memory
	// New connections may come in while other connections are still handled
	// When vector resizes, objects get moved. That is why we allocate
	// connections on the heap and use smart pointers as elements of the vector.
	std::vector<std::unique_ptr<Connection>> connections;
};
