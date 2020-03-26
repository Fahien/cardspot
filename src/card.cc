#include <spot/card/card.h>

#include <spot/gfx/graphics.h>


namespace spot::card
{


Hand::Hand( gfx::Graphics& g )
: gfx { g }
, node { gfx.models.create_node().index }
{
}


void Hand::add_card( const int32_t card )
{
	auto hand = gfx.models.get_node( node );
	hand->children.emplace_back( card );
}


}
