#include <cstdlib>

#include <fmt/format.h>
#include <spot/gfx/graphics.h>
#include "spot/card/card.h"
#include "spot/core/log.h"

namespace spot
{

const math::Vec2 card_size = { 2.5f, 3.5f };
const math::Vec2 card_offset = { -card_size.x / 2.0f, -card_size.y / 2.0f };
const math::Vec2 card_b = { card_size.x / 2.0f, card_size.y / 2.0f };
const math::Rect card_rect = { card_offset, card_b };


}  //namespace spot

int main()
{
	using namespace spot;

	auto gfx = gfx::Graphics();
	auto model = gfx.create_model();

	auto player = card::Player( model );
	auto first_card = player.deck.cards.push(
		card::Card( model, "img/card.png", player.back )
	);

	player.hand.add_card( first_card );

	gfx.camera.look_at( math::Vec3::Z, math::Vec3::Zero, math::Vec3::Y );
	gfx.viewport.set_offset( -4.0f, -4.0f );
	gfx.viewport.set_extent( 8.0f, 8.0f );

	gfx::Handle<gfx::Node> selected = model->nodes.push();
	selected->name = "Selection";
	auto rect_mesh = model->meshes.push(
		gfx::Mesh::create_rect( card_rect, gfx::Color::red )
	);
	selected->mesh = rect_mesh;

	while ( gfx.window.is_alive() )
	{
		gfx.glfw.poll();

		const auto dt = gfx.glfw.get_delta();
		gfx.window.update( dt );
		gfx.animations.update( dt, model );

		if ( gfx.window.click.left || gfx.window.click.right )
		{
			if ( gfx.window.click.left && selected )
			{
				selected->remove_from_parent();
			}

			// Select a card
			/// @todo Fix this call, as it does not work for perspective camera, I guess
			auto coords = gfx.window.cursor_to( gfx.viewport.get_abstract() );
			logi( "Click [" + std::to_string( coords.x ) + "," + std::to_string( coords.y ) + "]" );

			for ( auto card : player.hand.get_cards() )
			{
				if ( card->node->bounds )
				{
					auto& shape = card->node->bounds->get_shape();
					if ( shape.contains( coords ) )
					{
						if ( gfx.window.click.left )
						{
							logi( "Selected card" );
							card->node->add_child( selected );
						}
						else if ( gfx.window.click.right )
						{
							logi( "Rotating card" );
							card->flip();
						}
					}
				}
			}
		}

		if ( selected )
		{
			auto ratio = gfx.viewport.win_ratio();
			auto trans = gfx.window.swipe * ratio;
			if ( auto parent = selected->get_parent() )
			{
				parent->translation += trans;
			}
		}

		if ( gfx.window.scroll.y )
		{
			gfx.camera.node.translation.y += dt * gfx.window.scroll.y * 4.0f;
		}
		if ( gfx.window.scroll.x )
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
