#pragma once

#include <vector>
#include <bitset>
#include "CardBitMask.h"


class HandBitMask {

private:

	
	int primeRankProduct;
	std::bitset<27> combined_or_bitset;
	std::bitset<27> combined_and_bitset;

public:

	HandBitMask(); 

	HandBitMask(std::string);

	HandBitMask(std::vector<std::string>::iterator iHand,
				std::vector<std::string>::iterator handEnd);

	~HandBitMask() {}

	static HandBitMask* CreateHandBitMask(std::vector<CardBitMask*>);

	bool AreCardsUnique();

	bool IsHandFlush();

	int GetHandPrimeValue();

};