

#include "EvaluatorClass.h"
#include "HandEvaluationConstants.h"
#include "HandBitMask.h"

using namespace HandEvalConstants;

/*
Public Function Definitions
*/

/* Constructor and Deconstructor */

//Creates FastHandEvaluator Object, which takes in filename that will/already stores the lookup table
//Bool new_file creates new lookup table file at file_name if true, loads file at filename if false.
FastHandEvaluator::FastHandEvaluator() {



	this->flushes = new std::unordered_map<int, short>;
	this->paired = new std::unordered_map<int, short>;
	this->unique_ranks = new std::unordered_map<int, short>;
	EvaluateAllHandRanks();

}

//Deconstructor
FastHandEvaluator::~FastHandEvaluator() {

	delete( this->flushes );
	delete( this->paired );
	delete( this->unique_ranks );
}


/* Hand rank evaluators*/

//Returns -1 if hand cannot be evaluated
short FastHandEvaluator::GetHandRank(HandBitMask* hand_bits) {

	//HandBitMask must not be nullptr
	if (hand_bits == nullptr) {
		return -1;
	}
	//Check if hand is flush
	int HandPrimeVal = hand_bits->GetHandPrimeValue();
	if (hand_bits->IsHandFlush()) {
		return this->flushes->at(HandPrimeVal);
	}
	//Check if cards in hand are all unique rank
	if (hand_bits->AreCardsUnique()) {
		return this->unique_ranks->at(HandPrimeVal);
	}
	//Otherwise, and is paired
	else {
		return this->paired->at(HandPrimeVal);
	}
}

//Returns -1 if Out of position player wins hand
//Returns 1 if In Position player wins hand
//Returns 0 if both players tie (known as chopping colloquially)
int FastHandEvaluator::EvaluateWinner(HandBitMask* OOP_hand, HandBitMask* IP_hand) {
	short OOP_rank = GetHandRank(OOP_hand);
	short IP_rank = GetHandRank(IP_hand);
	if (OOP_rank < IP_rank) {
		return -1;
	}
	if (OOP_rank == IP_rank) {
		return 0;
	}
	else {
		return 1;
	}
}



/*
Private Function Definitions
*/

/*
Helper functions that generate lookup table for hand ranking retrieval
*/
void FastHandEvaluator::EvaluateAllHandRanks() {

	

	GenerateQuads(this->paired);

	GenerateFullHouses(this->paired);

	GenerateFlushes(this->flushes);

	GenerateTrips(this->paired);

	GenerateTwoPairs(this->paired);

	GenerateOnePairs(this->paired);

	GenerateAllHighCards(this->unique_ranks, HandEvalConstants::START_RANK_HIGH_CARDS);

	GenerateStraightFlushesAndStraights(this->flushes, this->unique_ranks);

}

void FastHandEvaluator::GenerateStraightFlushesAndStraights(std::unordered_map<int, short>* flushes, std::unordered_map<int, short>* straights) {

	short hand_rank = START_RANK_STRAIGHT_FLUSH;
	short straight_rank = START_RANK_STRAIGHTS;
	int i = 0;

	for (i; i < DISTINCT_STRAIGHT_FLUSHES; i++) {
		int straight_prime = STRAIGHT_PRIME_VALS[i];
		std::pair<int, short> straight_flush_entry = std::make_pair(straight_prime, hand_rank);
		std::pair<int, short> straight_entry = std::make_pair(straight_prime, straight_rank);
		flushes->insert_or_assign(straight_prime, hand_rank);
		
		straights->insert(straight_entry);

	
		straight_rank++;
		hand_rank++;
		
	}
}

void FastHandEvaluator::GenerateQuads(std::unordered_map<int, short>* paired) {

	short hand_rank = START_RANK_QUADS;
	int quad_rank = CARD_RANKS.at('A');

	for (quad_rank; quad_rank > 1; quad_rank--) {

		int kicker_rank = CARD_RANKS.at('A');
		for (kicker_rank; kicker_rank > 1; kicker_rank--) {

			if (kicker_rank == quad_rank) {
				continue;
			}
			int quad_prime = GetCardPrime(quad_rank);
			int kicker_prime = GetCardPrime(kicker_rank);
			int total_prime = pow(quad_prime, 4) * kicker_prime;
			std::pair<int, short> quad_entry = std::make_pair(total_prime, hand_rank);
			paired->insert(quad_entry);
			hand_rank++;
		}
	}
}

void FastHandEvaluator::GenerateFullHouses(std::unordered_map<int, short>* paired) {

	short hand_rank = START_RANK_FULL_HOUSE;

	int trips_rank = CARD_RANKS.at('A');

	for (trips_rank; trips_rank > 1; trips_rank--) {


		int pair_rank = CARD_RANKS.at('A');
		for (pair_rank; pair_rank > 1; pair_rank--) {

			if (trips_rank == pair_rank) {
				continue;
			}
			int trips_prime = GetCardPrime(trips_rank);
			int pair_prime = GetCardPrime(pair_rank);
			int total_prime = pow(trips_prime, 3) * pow(pair_prime, 2);
			std::pair<int, short> full_house_entry = std::make_pair(total_prime, hand_rank);
			paired->insert(full_house_entry);
			hand_rank++;
		}
	}
}

void FastHandEvaluator::GenerateAllHighCards(std::unordered_map<int, short>* high_cards, int start_rank) {

	short hand_rank = start_rank;

	int first_rank = CARD_RANKS.at('A');
	for (first_rank; first_rank > 5; first_rank--) {

		int second_rank = first_rank - 1;
		for (second_rank; second_rank > 4; second_rank--) {

			int third_rank = second_rank - 1;
			for (third_rank; third_rank > 3; third_rank--) {

				int fourth_rank = third_rank - 1;
				for (fourth_rank; fourth_rank > 2; fourth_rank--) {

					int fifth_rank = fourth_rank - 1;
					for (fifth_rank; fifth_rank > 1; fifth_rank--) {

						std::vector<int> temp_rank_list = { first_rank, second_rank, third_rank, fourth_rank, fifth_rank };
						//Check if cards don't make a straight.
						if (!IsHandStraight(temp_rank_list)) {

							int first_prime = GetCardPrime(first_rank);
							int second_prime = GetCardPrime(second_rank);
							int third_prime = GetCardPrime(third_rank);
							int fourth_prime = GetCardPrime(fourth_rank);
							int fifth_prime = GetCardPrime(fifth_rank);

							int prime_product = first_prime * second_prime * third_prime * fourth_prime * fifth_prime;
							std::pair<int, short> high_card_entry = std::make_pair(prime_product, hand_rank);
							high_cards->insert(high_card_entry);

							hand_rank++;

						}
					}
				}
			}
		}
	}
}

void FastHandEvaluator::GenerateFlushes(std::unordered_map<int, short>* flushes) {
	GenerateAllHighCards(flushes, START_RANK_FLUSHES);
}

void FastHandEvaluator::GenerateTrips(std::unordered_map<int, short>* paired) {

	short hand_rank = START_RANK_TRIPS;
	int trips_rank = CARD_RANKS.at('A');

	for (trips_rank; trips_rank > 1; trips_rank--) {

		int kicker_high_rank = CARD_RANKS.at('A');
		for (kicker_high_rank; kicker_high_rank > 2; kicker_high_rank--) {

			int kicker_low_rank = kicker_high_rank - 1;
			for (kicker_low_rank; kicker_low_rank > 1; kicker_low_rank--) {

				std::vector<int> temp_rank_list = { trips_rank, kicker_high_rank, kicker_low_rank };
				//Make sure card ranks are unique
				if (kicker_high_rank != trips_rank && kicker_low_rank != trips_rank) {

					int trips_prime = GetCardPrime(trips_rank);
					int kicker_high_prime = GetCardPrime(kicker_high_rank);
					int kicker_low_prime = GetCardPrime(kicker_low_rank);
					int prime_product = pow(trips_prime, 3) * kicker_high_prime * kicker_low_prime;
					std::pair<int, short> trips_entry = std::make_pair(prime_product, hand_rank);
					paired->insert(trips_entry);

					hand_rank++;
				}
			}
		}
	}
}

void FastHandEvaluator::GenerateTwoPairs(std::unordered_map<int, short>* paired) {

	int hand_rank = START_RANK_TWO_PAIRS;

	int high_pair = CARD_RANKS.at('A');

	for (high_pair; high_pair > 2; high_pair--) {

		int low_pair = high_pair - 1;
		for (low_pair; low_pair > 1; low_pair--) {

			int kicker = CARD_RANKS.at('A');
			for (kicker; kicker > 1; kicker--) {

				if (kicker != high_pair && kicker != low_pair) {

					int high_pair_prime = GetCardPrime(high_pair);
					int low_pair_prime = GetCardPrime(low_pair);
					int kicker_prime = GetCardPrime(kicker);
					int prime_product = pow(high_pair_prime, 2) * pow(low_pair_prime, 2) * kicker_prime;
					std::pair<int, short> two_pair_entry = std::make_pair(prime_product, hand_rank);
					paired->insert(two_pair_entry);

					hand_rank++;
				}
			}
		}
	}
}

void FastHandEvaluator::GenerateOnePairs(std::unordered_map<int, short>* paired) {

	int hand_rank = START_RANK_ONE_PAIRS;

	int pair_rank = CARD_RANKS.at('A');

	for (pair_rank; pair_rank > 1; pair_rank--) {

		int high_kicker = CARD_RANKS.at('A');
		for (high_kicker; high_kicker > 3; high_kicker--) {

			int medium_kicker = high_kicker - 1;
			for (medium_kicker; medium_kicker > 2; medium_kicker--) {

				int small_kicker = medium_kicker - 1;
				for (small_kicker; small_kicker > 1; small_kicker--) {

					if (pair_rank != high_kicker && pair_rank != medium_kicker && pair_rank != small_kicker) {

						int pair_prime = GetCardPrime(pair_rank);
						int high_kicker_prime = GetCardPrime(high_kicker);
						int medium_kicker_prime = GetCardPrime(medium_kicker);
						int low_kicker_prime = GetCardPrime(small_kicker);

						int prime_product = pow(pair_prime, 2) * high_kicker_prime * medium_kicker_prime * low_kicker_prime;
						std::pair<int, short> pair_entry = std::make_pair(prime_product, hand_rank);
						paired->insert(pair_entry);
						hand_rank++;
					}
				}
			}
		}
	}
}


//Helper Functions for lookup table generation
bool FastHandEvaluator::AreRanksUnique(std::vector<int> ranks_list) {

	std::bitset<13> curr = std::bitset<13>().set();
	for (int rank : ranks_list) {
		std::bitset<13> int_bit = std::bitset<13>().set(1);
		int_bit <<= ( rank - 2 );
		curr |= int_bit;

	}
	return curr.count() == ranks_list.size();
}

bool FastHandEvaluator::IsHandStraight(std::vector<int> ranks_list) {

	int min_rank = 15;
	int max_rank = 0;
	std::bitset<27> curr = std::bitset<27>(0);
	for (int rank : ranks_list) {
		min_rank = std::min(rank, min_rank);
		max_rank = std::max(rank, max_rank);
		std::bitset<27> int_bit = std::bitset<27>().set(1);
		int_bit <<= ( rank - 2 );
		curr |= int_bit;
	}

	curr >>= ( min_rank - 2 );

	//Edge case for Wheel Straight
	if (max_rank == 14 && min_rank == 2) {
		//Shift bitset to left by 1 and OR with {00001} to check for wheel
		curr <<= 1;
		std::bitset<27> one_bit = std::bitset<27>().set(1);
		curr |= one_bit;
	}

	//Check if straight by bitwise and with Straight Bit Mask
	curr &= STRAIGHT_BIT_MASK;
	return curr.count() == 5;
}

int FastHandEvaluator::GetCardPrime(int card_rank) {

	return CARD_PRIME[card_rank - 2];
}