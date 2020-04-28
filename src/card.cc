#include <spot/card/card.h>

#include <spot/gfx/graphics.h>


namespace spot::card
{


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


Hand::Hand( const gfx::Handle<gfx::Gltf>& model, Deck& d )
: deck { d }
, node { model->nodes.push() }
{
}


void Hand::add_card( const gfx::Handle<Card>& card )
{
	node->add_child( card->node );
	cards.emplace_back( card );
}


} // namespace spot::card
