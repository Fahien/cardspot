#include <cstdlib>
#include <thread>

#include <boost/array.hpp>
#include <boost/asio.hpp>
#include <fmt/color.h>
#include <fmt/format.h>

#include "log.h"
#include "message.h"
#include "connection.h"

#include "spot/gfx/graphics.h"
#include "board.h"


#define BUF( buf ) ( asio::buffer( &buf, sizeof( buf ) ) )
#define STR( arr ) ( std::string( std::begin( arr ), std::end( arr ) ) )

namespace asio = boost::asio;


int main( int argc, const char** argv )
{
	if ( argc != 2 )
	{
		loge( fmt::format( "Usage: {} <host>", argv[0] ) );
		return EXIT_FAILURE;
	}

	auto io = asio::io_context();
	auto resolver = asio::ip::tcp::resolver( io );
	auto endpoints = resolver.resolve( argv[1], "daytime" );

	for ( auto& endpoint : endpoints )
	{
		logi( fmt::format( "{} {} {}::{}", endpoint.host_name(), endpoint.service_name(), endpoint.endpoint().address().to_string(), endpoint.endpoint().port() ) );
	}
	auto gfx = spot::gfx::Graphics();

	auto con = Connection( gfx, io, endpoints );
	auto io_thread = std::thread( [&io](){ io.run(); } );

	auto meshes = spot::gfx::SolidMeshes( gfx );
	auto board = spot::gfx::create_board( gfx );

	gfx.view = spot::gfx::look_at( spot::math::Vec3::Z, spot::math::Vec3::Zero, spot::math::Vec3::Y );

	gfx.proj = spot::gfx::ortho(
		spot::gfx::viewport.offset.x, spot::gfx::viewport.extent.x,
		spot::gfx::viewport.offset.y, spot::gfx::viewport.extent.y,
		0.125f, 2.0 );

	while ( gfx.window.is_alive() )
	{
		gfx.glfw.poll();

		const auto dt = gfx.glfw.get_delta();
		gfx.window.update( dt );

		if ( gfx.window.click )
		{
			auto coords = gfx.window.cursor_to( spot::gfx::viewport );

			for ( auto child : NODE( board )->get_children() )
			{
				if ( child->contains( coords ) )
				{
					// Tell the server to interact with this node
					con.send( Message::node( child->index ) );
				}
			}
		}
	
		if ( gfx.render_begin() )
		{
			gfx.draw( board );
			gfx.render_end();
		}
	}

	/// @todo Is detaching the right approach?
	io_thread.detach();
	gfx.device.wait_idle();
	return EXIT_SUCCESS;
}
