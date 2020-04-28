#pragma once

#include <vector>
#include <spot/gltf/handle.h>

namespace spot
{

namespace gfx
{
class Gltf;
class Node;
class Animation;
}

namespace card
{

class Card : public gfx::Handled<Card>
{
  public:

	/// Graphics node of this card
	gfx::Handle<gfx::Node> node = {};

	/// Rotate animations
	gfx::Handle<gfx::Animation> hide_anim = {};
	gfx::Handle<gfx::Animation> show_anim = {};

	void flip();

	bool shown = true;
};


class Deck
{
  public:
	gfx::Uvec<Card> cards;
};


class Hand
{
  public:
	Hand( const gfx::Handle<gfx::Gltf>& model, Deck& d );

	const gfx::Handle<gfx::Node>& get_node() const noexcept { return node; }

	void add_card( const gfx::Handle<Card>& card );

	const std::vector<gfx::Handle<Card>>& get_cards() const { return cards; };

  private:
	Deck& deck;

	// Node of the hand
	gfx::Handle<gfx::Node> node = {};

	// Vector of indices to @ref Cards
	std::vector<gfx::Handle<Card>> cards;
};


class Player
{
  public:
	Player( const gfx::Handle<gfx::Gltf>& model ) : hand { model, deck } {}

	Deck deck;
	Hand hand;
	
};


} // namespace card
} // namespace spot