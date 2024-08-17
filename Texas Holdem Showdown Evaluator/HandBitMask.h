#pragma once

#include <vector>
#include <bitset>
#include "CardBitMask.h"


class HandBitMask {

private:

	

	CardBitMask* card_bitsets[5];
	std::bitset<27> combined_or_bitset;
	std::bitset<27> combined_and_bitset;

	CardBitMask** GetCardBitsets();


public:

	static HandBitMask* CreateHandBitMask(std::vector<CardBitMask*>);

	bool AreCardsUnique();

	bool IsHandFlush();

	int GetHandPrimeValue();

};