#pragma once

#include <vector>

namespace spot::card
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

	// Node of the hand
	uint32_t node;

	// Handles to cards
	std::vector<uint32_t> cards;
};


class Player
{
  public:
	
	Deck deck;
	Hand hand;
};


} // namespace spot::card
