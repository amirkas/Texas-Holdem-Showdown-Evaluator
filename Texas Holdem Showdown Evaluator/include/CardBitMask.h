#pragma once

#include <bitset>

class CardBitMask {

private:

	std::bitset<27> card_bits;

	//Format 
	//13 Rank Bits			- Each rank sets 1 bit to 1, from left 'A' : 1|0000|0000|0000, to right '2' : 0|0000|0000|0001
	//4  Suit Bits			- Each suit sets 1 bit to 1, Spade : 1000, Club: 0100, Diamond: 0010, Heart : 0001
	//4  Rank Bits (Int)	- Integer representation of card rank. A (14) : 1110, K (13) : 1101, ...., 2 (2) : 0010
	//6  Rank Bits (Prime)	- Prime representation of card ranks:
			//Rank:		2		3		4		5		6		7		8		9		T		J		Q		K		A
			//Prime:	2		3		5		7		11		13		17		19		23		29		31		37		41
			//bit:		000001	000011	000101	000111	001011	001101	010001	010011	010111	011101	011111	100101	101001

public:

	CardBitMask(std::bitset<27>);

	CardBitMask(char, char);

	CardBitMask(std::string);

	int GetCardPrime();

	char GetCardSuit();

	int GetCardRankInt();

	char GetCardRankChar();

	std::bitset<27> GetCardBitset();

};
