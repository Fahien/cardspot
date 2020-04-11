#include <spot/card/card.h>

#include <spot/gfx/graphics.h>


namespace spot::card
{


Card& Deck::add_card( Card&& c )
{
	c.index = cards.size();
	return cards.emplace_back( std::move( c ) );
}


Hand::Hand( gfx::Graphics& g, Deck& d )
: gfx { g }
, deck { d }
, node { gfx.models.create_node().index }
{
}


void Hand::add_card( const Card& card )
{
	auto hand = gfx.models.get_node( node );
	hand->add_child( *gfx.models.get_node( card.node ) );
	cards.emplace_back( card.index );
}


std::vector<const Card*> Hand::get_cards() const
{
	std::vector<const Card*> ret( cards.size() );

	for ( size_t i = 0; i < cards.size(); ++i )
	{
		auto card_index = cards[i];
		auto& card = deck.get_cards()[card_index];
		ret[i] = &card;
	}

	return ret;
}


} // namespace spot::card
