#include <cstdlib>

#include <spot/gfx/graphics.h>
#include "spot/card/card.h"

namespace spot
{

card::Card create_card( const char* image, gfx::Graphics& gfx )
{
	card::Card ret;
	ret.mesh = gfx.models.meshes.size();

	auto view = gfx.images.load( image );
	auto& material = gfx.models.create_material( view );
	gfx.models.meshes.emplace_back(
		gfx::Mesh::create_quad(
			material.index,
			math::Vec3( -1.25f, -1.75f, 0.0f ),
			math::Vec3( 1.25f, 1.75f, 0.0f )
		)
	);

	return ret;
}

}  //namespace spot

int main()
{
	using namespace spot;

	auto gfx = gfx::Graphics();

	auto player = card::Player( gfx );
	player.deck.cards.emplace_back(
		create_card( "img/card.png", gfx )
	);
	player.hand.add_card( gfx.models.create_node().index );

	gfx.view = gfx::look_at( math::Vec3::Z * 8.0f, math::Vec3::Zero, math::Vec3::Y );

	while ( gfx.window.is_alive() )
	{
		gfx.glfw.poll();

		const auto dt = gfx.glfw.get_delta();
		gfx.window.update( dt );

		if ( gfx.window.click )
		{
			auto& node = gfx.models.create_node();
			node.mesh = player.deck.cards[0].mesh;

			auto hand = gfx.models.get_node( player.hand.get_node() );

			node.translation.x += hand->children.size() * 1.0f;
			node.translation.z += hand->children.size() * 0.005f;

			hand->translation.x -= hand->children.size() ? 0.5f : 0.0f;
			hand->children.emplace_back( node.index );
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
