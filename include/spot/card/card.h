#pragma once

#include <vector>

namespace spot
{

namespace gfx
{
class Graphics;
}

namespace card
{

class Card
{
  public:

	/// Index of this card in the deck
	int32_t index = -1;

	/// Graphics node of this card
	int32_t node = -1;
};


class Deck
{
  public:
	Card& add_card( Card&& c );

	const std::vector<Card>& get_cards() const { return cards; }

  private:
	std::vector<Card> cards;
};


class Hand
{
  public:
	Hand( gfx::Graphics& g, Deck& d );

	int32_t get_node() const noexcept { return node; }

	void add_card( const Card& card );

	std::vector<const Card*> get_cards() const;

  private:
	gfx::Graphics& gfx;
	Deck& deck;

	// Node of the hand
	int32_t node = -1;

	// Vector of indices to @ref Cards
	std::vector<int32_t> cards;
};


class Player
{
  public:
	Player( gfx::Graphics& g ) : gfx { g }, hand { g, deck } {}

	gfx::Graphics& gfx;

	Deck deck;
	Hand hand;
	
};


} // namespace card
} // namespace spot