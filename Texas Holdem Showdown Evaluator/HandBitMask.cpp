
#include "HandBitMask.h" 
#include "CardBitMask.h"
#include "HandEvaluationConstants.h"
#include "bitset"

/*
CreateHandBitMask()	-	Factory Constructor for HandBitMask
	- Return Value: Pointer to newly created HandBitMask;
	- Errors:
		- If size of inputted array is not 5, returns nullptr.
		- If any CardBitMask pointers is null, func returns nullptr.
*/
HandBitMask* HandBitMask::CreateHandBitMask(std::vector<CardBitMask*> card_bitmasks) {


	if (card_bitmasks.size() != 5) {
		return nullptr;
	}

	HandBitMask* hand_bitmask = new HandBitMask();
	std::bitset<27> combined_or = std::bitset<27>(0);
	std::bitset<27> combined_and = std::bitset<27>().set();

	int i = 0;
	std::bitset<27> curr_card_bits;
	for (CardBitMask* card_bits : card_bitmasks) {
		if (card_bits == nullptr) {
			delete( hand_bitmask );
			return nullptr;
		}
		hand_bitmask->card_bitsets[i] = card_bitmasks[i];
		curr_card_bits = card_bits->GetCardBitset();
		combined_or |= curr_card_bits;
		combined_and &= curr_card_bits;

		i++;
	}
	hand_bitmask->combined_or_bitset = combined_or;
	hand_bitmask->combined_and_bitset = combined_and;
	return hand_bitmask;
}



/*
Public Methods
	* AreCardsUnique() - Checks if all 5 cards in hand are a unique rank.
	* IsHandFlush() - Checks if a hand is a flush.
	* GetHandPrimeValue() - Gets prime value for hand for table lookup.
*/


/*
AreCardsUnique()
	- Checks if all 5 cards in hand are unique rank.
	- Flushes, Straights, and High Card hands must have all cards be a unique rank.
	- Paired boards must have at least 1 duplicate rank.
*/
bool HandBitMask::AreCardsUnique() {

	std::bitset<27> temp = this->combined_or_bitset;
	temp >>= 14;
	return temp.count() == 5;
}

/*
IsHandFlush()
	- Checks if all 5 cards have the same suit.
	- Guaranteed that all 5 cards are unique if hand is a flush.
*/
bool HandBitMask::IsHandFlush() {

	std::bitset<27> temp = this->combined_and_bitset;
	temp &= HandEvalConstants::FLUSH_BIT_MASK;
	return temp.count() == 1;
}

/*
GetHandPrimeValue()
	- Gets prime value associated with hand.
	- Iterates through each card to find it's individual prime value.
	- Multiplies each cards prime value to create unique prime product for hand.
		- Order of cards does not matter.
*/
int HandBitMask::GetHandPrimeValue() {

	int product = 1;
	for (CardBitMask* card_bitmask : this->card_bitsets) {
		product *= card_bitmask->GetCardPrime();
	}
	return product;
}




/*
Private methods - Object Accessors.
	- GetCardBitsets(), returns pointer to CardBitMask* array.
	- GetCombinedOrBitset(), returns 27-sized bitset of each hand combined with Bitwise OR
	- GetCombinedAndBitset(), returns 27-sized bitset of each hand combined with Bitwise AND

	- SetCardBitsets(int index, CardBitMask* card)
		- Sets CardBitMask* at this->card_bitsets[index] to card.
	- SetCombinedOrBitset(std::bitset<27>) - Sets
*/

/*
GetCardBitsets()
Returns CardBitMask** - pointer to Object's card_bitsets array.
*/
CardBitMask** HandBitMask::GetCardBitsets() {
	return this->card_bitsets;
}