#include <spot/card/card.h>

#include <fmt/format.h>
#include "spot/core/log.h"
#include <spot/gfx/graphics.h>


namespace spot::card
{

const math::Vec2 card_size = { 2.5f, 3.5f };
const math::Vec2 card_offset = { -card_size.x / 2.0f, -card_size.y / 2.0f };
const math::Vec2 card_b = { card_size.x / 2.0f, card_size.y / 2.0f };
const math::Rect card_rect = { card_offset, card_b };


Card::Card(
	const gfx::Handle<gfx::Gltf>& model,
	const std::string& image_path,
	const gfx::Handle<gfx::Mesh>& back_mesh )
: node { model->nodes.push() }
, hide_anim { model->animations.push( gfx::Animation( model ) ) }
, show_anim { model->animations.push( gfx::Animation( model ) ) }
{
	show_anim->state = gfx::Animation::State::Stop;
	show_anim->add_rotation( node, 0.5f,
		math::Quat( math::Vec3::Y, math::radians( 180.0f ) ),
		math::Quat( math::Vec3::Y, math::radians( 360.0f ) ) );
	hide_anim->state = gfx::Animation::State::Stop;
	hide_anim->add_rotation( node, 0.5f, math::Quat( math::Vec3::Y, math::radians( 180.0f ) ) );

	auto view = model->images->load( image_path.c_str() );
	auto material = model->materials.push( gfx::Material( view ) );
	node->mesh = model->meshes.push(
		gfx::Mesh::create_quad(
			material,
			math::Vec3( card_offset.x, card_offset.y, 0.0f ),
			math::Vec3( card_b.x, card_b.y, 0.0f )
		)
	);

	// Add node bounds
	auto rect = model->rects.push( gfx::Rect( card_offset, card_b ) );
	rect->node = node;
	node->bounds = model->bounds.push();
	node->bounds->shape = rect;

	// Add backface as a flipped child
	auto back_node = model->nodes.push( gfx::Node( back_mesh ) );
	back_node->rotation = math::Quat( math::Vec3::Y, math::radians( 180.0f ) );
	node->add_child( back_node );
}


void Card::flip()
{
	if ( shown )
	{
		hide_anim->state = gfx::Animation::State::Play;
	}
	else
	{
		show_anim->state = gfx::Animation::State::Play;
	}
	shown = !shown;
}


Hand::Hand( const gfx::Handle<gfx::Gltf>& m, Deck& d )
: model { m }
, deck { d }
, node { model->nodes.push() }
{
}


void Hand::organize_positions()
{
	auto size = cards.size();

	for ( size_t i = 0; i < size; ++i )
	{
		// When there is one card left and right are the same node
		auto& pos = node->get_children()[i];
		pos->get_children()[0]->translation = math::Vec3::Zero;
		pos->translation.x = i * card_size.x / 4.0f;
		pos->translation.z = i * card_size.y / 256.0f;
	}

	node->translation.x = -( (size - 1) * card_size.x / 8.0f );
}


void Hand::add_card( const gfx::Handle<Card>& card )
{
	auto pos = model->nodes.push();
	node->add_child( pos );
	pos->add_child( card->node );
	cards.emplace_back( card );
	organize_positions();
}


gfx::Handle<gfx::Mesh> create_back( const gfx::Handle<gfx::Gltf>& model )
{
	auto view = model->images->load( "img/back.png" );
	auto material = model->materials.push( gfx::Material( view ) );
	auto mesh = gfx::Mesh::create_quad(
			material,
			math::Vec3( card_offset.x, card_offset.y, 0.0f ),
			math::Vec3( card_b.x, card_b.y, 0.0f )
	);

	return model->meshes.push( std::move( mesh ) );
}


Player::Player( const gfx::Handle<gfx::Gltf>& model )
: back { create_back( model ) }
, hand { model, deck }
{}


} // namespace spot::card
