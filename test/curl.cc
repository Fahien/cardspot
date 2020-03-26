#include <cstdlib>

#include <spot/card/card.h>

#include <spot/gfx/graphics.h>
#include <spot/gfx/viewport.h>
#include <spot/gfx/png.h>
#include <spot/net/curl.h>
#include <fmt/format.h>

using namespace spot::gfx;
using namespace spot::math;
using namespace spot::card;

constexpr const float unit = 0.125f;


int32_t create_card( const std::string& icon, Graphics& gfx )
{
	spot::net::Curl curl;

	const std::string base_path = "https://game-icons.net/icons/ffffff/000000/1x1/";
	auto url = base_path + icon + ".png";

	auto [bytes, err] = curl.download( url );
	if ( err )
	{
		fmt::print( "{}\n", spot::net::Curl::strerror( err ) );
	}

	auto view = gfx.models.images.load( icon.c_str(), bytes );

	auto& node = gfx.models.create_node(
		Mesh::create_quad(
			gfx.models.create_material( view ).index
		)
	);

	return node.index;
}


int main( int argc, const char** argv )
{
	const auto viewport = Viewport( { -8, -8 }, { 16, 16 } );

	auto gfx = Graphics();

	auto player = Player( gfx );

	int32_t card1 = create_card( "lorc/plain-dagger", gfx );
	gfx.models.get_node( card1 )->translation.x -= 1.0f;
	int32_t card2 = create_card( "sbed/shield", gfx );
	gfx.models.get_node( card2 )->translation.x += 1.0f;

	player.hand.add_card( card1 );
	player.hand.add_card( card2 );

	gfx.view = look_at( Vec3::Z, Vec3::Zero, Vec3::Y );
	gfx.proj = ortho(
		viewport.offset.x , viewport.extent.x + viewport.offset.x,
		viewport.offset.y, viewport.extent.y + viewport.offset.y,
		0.125f, 2.0 );

	double tick = 1.0;
	double time = 0.0f;

	int32_t select = -1;

	while ( gfx.window.is_alive() )
	{
		gfx.glfw.poll();

		const auto dt = gfx.glfw.get_delta();
		gfx.window.update( dt );

		if ( gfx.window.pressed )
		{
			auto vcoords = gfx.window.cursor_to( viewport );
			if ( select < 0 )
			{
				// Select a node
				auto hand = gfx.models.get_node( player.hand.get_node() );
				for ( auto card_index : hand->children )
				{
					auto card = gfx.models.get_node( card_index );
					if ( card->contains( vcoords ) )
					{
						select = card_index;
					}
				}
			}
			else
			{
				// Move selected
				auto node = gfx.models.get_node( select );
				node->translation.x = vcoords.x;
				node->translation.y = vcoords.y;
			}
		}
		else
		{
			// Unselect
			select = -1;
		}
		

		if ( gfx.render_begin() )
		{
			gfx.draw( player.hand.get_node() );
			gfx.render_end();
		}
	}

	gfx.device.wait_idle();

	return EXIT_SUCCESS;
}
