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

	// Handle to node mesh into the model
	uint32_t mesh;
};


class Deck
{
  public:

	std::vector<Card> cards;
};


class Hand
{
  public:
	Hand( gfx::Graphics& g );

	int32_t get_node() const noexcept { return node; }

	void add_card( int32_t card );

  private:
	gfx::Graphics& gfx;

	// Node of the hand
	int32_t node = -1;

	// Handles to cards
	std::vector<uint32_t> cards;
};


class Player
{
  public:
	Player( gfx::Graphics& g ) : gfx { g }, hand { g } {}

	gfx::Graphics& gfx;

	Deck deck;
	Hand hand;
	
};


} // namespace card
} // namespace spot