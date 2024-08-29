
#include "pch.h"
#define NOMINAX
#include "CardBitMask.h"
#include "HandEvaluationConstants.h"
#include "bitset"
#include "vector"
using namespace HandEvalConstants;

/*
Constructors
 - Generates bitset that uniquely represents each card in 52 card deck
*/
CardBitMask::CardBitMask(std::bitset<27> card_bitset) {

	this->card_bits = card_bitset;
}

//Convert String representation to Bit representation
CardBitMask::CardBitMask(std::string card) : CardBitMask(card[0], card[1]) {}

//Convert Char representation to Bit Representation
CardBitMask::CardBitMask(char card_rank, char card_suit) {

	int rank = CARD_RANKS.at(card_rank);
	int suit_val = SUIT_VALUE.at(card_suit);
	int rank_prime = CARD_PRIME[rank - 2];

	std::bitset<27> rank_bits_prime = std::bitset<27>(rank_prime);

	std::bitset<27> rank_bits_int = std::bitset<27>(rank);
	rank_bits_int <<= 6;

	std::bitset<27> suit_bits = std::bitset<27>(suit_val);
	suit_bits <<= 10;



	//Start of with bitset that has 1 set at 1st bit.
	this->card_bits = std::bitset<27>().set(0);

	//To Get Rank Bits, left shift the bit set by 14 + (rank - 2), alternatively 12 + rank
	this->card_bits <<= ( 12 + ( rank ) );

	//Bitwise and with created bitsets for rank int, rank prime, and suit val.
	this->card_bits |= rank_bits_prime;
	this->card_bits |= rank_bits_int;
	this->card_bits |= suit_bits;
}


/*
Accessors
Retrives information about card from bitset.
*/

/*
GetCardPrime()
	- Gets prime number associated with card rank.
	- Used to generate unique prime multiplication product for table lookup.
*/
int CardBitMask::GetCardPrime() {

	std::bitset<27> temp = this->card_bits;
	std::bitset<6> rank_prime_bits(temp.to_ullong());
	return (int) rank_prime_bits.to_ullong();
}

/*
GetCardSuit()
	- Gets suit of card as char
*/
char CardBitMask::GetCardSuit() {

	std::bitset<27> temp = this->card_bits;
	temp &= FLUSH_BIT_MASK;
	temp >>= 10;
	int suit_val = (int) temp.to_ullong();
	switch (suit_val) {
		case 8:
			return 's';
		case 4:
			return 'c';
		case 2:
			return 'd';
		case 1:
			return 'h';
		default:
			return '\0';
	}
}

/*
GetCardRankInt()
	- Gets rank of card as int
*/
int CardBitMask::GetCardRankInt() {

	std::bitset<27> temp = this->card_bits;
	temp &= RANK_BIT_MASK;
	temp >>= 6;
	return (int) temp.to_ullong();
}

/*
GetCardRankChar()
	- Gets Rank of card as char
*/
char CardBitMask::GetCardRankChar() {

	int rank_val = GetCardRankInt();
	return RANK_TO_CHAR[rank_val];
}

/*
GetCardBitset()
	- Returns bitset representation of card
*/
std::bitset<27> CardBitMask::GetCardBitset() {
	return this->card_bits;
}