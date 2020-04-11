#include <cstdlib>

#include <fmt/format.h>
#include <spot/gfx/graphics.h>
#include "spot/card/card.h"
#include "spot/core/log.h"

#define NODE( index ) ( gfx.models.get_node( index ) )
#define BOUNDS( bounds_index ) ( gfx.models.gltf.get_bounds( bounds_index ) )
#define SHAPE( bounds_index ) ( gfx.models.gltf.get_shape( bounds_index ) )

namespace spot
{

const math::Vec2 card_size = { 2.5f, 3.5f };
const math::Vec2 card_offset = { -card_size.x / 2.0f, -card_size.y / 2.0f };
const math::Vec2 card_b = { card_size.x / 2.0f, card_size.y / 2.0f };
const math::Rect card_rect = { card_offset, card_b };


card::Card create_card( const char* image, gfx::Graphics& gfx )
{
	card::Card ret;
	auto& node = gfx.models.create_node();
	ret.node = node.index;

	node.mesh = gfx.models.meshes.size();

	auto view = gfx.images.load( image );
	auto& material = gfx.models.create_material( view );
	gfx.models.meshes.emplace_back(
		gfx::Mesh::create_quad(
			material.index,
			math::Vec3( card_offset.x, card_offset.y, 0.0f ),
			math::Vec3( card_b.x, card_b.y, 0.0f )
		)
	);

	// Add node bounds
	auto& rect = gfx.models.gltf.rects.emplace_back( gltf::Rect( card_offset, card_b ) );
	rect.model = &gfx.models.gltf;
	rect.node = ret.node;
	rect.index = gfx.models.gltf.rects.size() - 1;

	auto& bounds = gfx.models.gltf.bounds.emplace_back();
	bounds.index = gfx.models.gltf.bounds.size() - 1;
	bounds.type = gltf::Bounds::Type::Rect;
	bounds.shape = rect.index;

	node.bounds = bounds.index;

	return ret;
}

}  //namespace spot

int main()
{
	using namespace spot;

	auto gfx = gfx::Graphics();

	auto player = card::Player( gfx );
	auto& first_card = player.deck.add_card(
		create_card( "img/card.png", gfx )
	);

	player.hand.add_card( first_card );

	gfx.camera.look_at( math::Vec3::Z, math::Vec3::Zero, math::Vec3::Y );
	gfx.viewport.set_offset( -8.0f, -8.0f );
	gfx.viewport.set_extent( 16.0f, 16.0f );

	int32_t selected_index = -1;
	{
		auto& selected_node = gfx.models.create_node();
		selected_node.name = "Selection";
		auto& rect_mesh = gfx.models.create_mesh(
			gfx::Mesh::create_rect( card_rect, gfx::Color::red )
		);
		selected_node.mesh = rect_mesh.index;
		selected_index = selected_node.index;
	}

	while ( gfx.window.is_alive() )
	{
		gfx.glfw.poll();

		const auto dt = gfx.glfw.get_delta();
		gfx.window.update( dt );

		if ( gfx.window.click.right )
		{
			// Add a new card to the hand
			auto& new_card = player.deck.add_card(
				create_card( "img/card.png", gfx )
			);

			player.hand.add_card( new_card );

			auto hand = NODE( player.hand.get_node() );
			auto node = NODE( new_card.node );
		}

		if ( gfx.window.click.left )
		{
			if ( auto selected_node = NODE( selected_index ) )
			{
				selected_node->remove_from_parent();
			}

			// Select a card
			/// @todo Fix this call, as it does not work for perspective camera, I guess
			auto coords = gfx.window.cursor_to( gfx.viewport.get_abstract() );
			logi( "Click [" + std::to_string( coords.x ) + "," + std::to_string( coords.y ) + "]" );

			for ( auto card : player.hand.get_cards() )
			{
				auto node = NODE( card->node );
				if ( auto shape = SHAPE( node->bounds ) )
				{
					if ( shape->contains( coords ) )
					{
						logi( "Clicked on card" );
						auto selected_node = NODE( selected_index );
						selected_node->remove_from_parent();
						node->add_child( *selected_node );
					}
				}
			}
		}

		if ( auto selected = NODE( selected_index ) )
		{
			auto ratio = gfx.viewport.win_ratio();
			auto trans = gfx.window.swipe * ratio;
			if ( auto parent = selected->get_parent() )
			{
				parent->translation += trans;
			}
		}

		if ( gfx.window.scroll.y != 0 )
		{
			gfx.camera.node.translation.y += dt * gfx.window.scroll.y * 4.0f;
		}
		if ( gfx.window.scroll.x != 0 )
		{
			gfx.camera.node.translation.x += dt * gfx.window.scroll.x * 4.0f;
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
