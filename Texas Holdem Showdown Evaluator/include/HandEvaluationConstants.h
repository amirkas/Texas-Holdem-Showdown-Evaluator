#pragma once

#include <map>
#include <bitset>
#include <string>

namespace HandEvalConstants {

	/*
	Constants for card Char <=> Int conversion.
	*/
	const std::map<char, int> CARD_RANKS = {
		{'A', 14}, {'K', 13}, {'Q', 12}, {'J', 11}, {'T', 10},
		{'9', 9}, {'8', 8}, {'7', 7}, {'6', 6},
		{'5', 5}, {'4', 4}, {'3', 3}, {'2', 2}
	};

	//Values for index 0 & 1 are null as rank starts from 2.
	const char RANK_TO_CHAR[16] = "nn23456789TJQKA";
	const int CARD_PRIME[13] = { 2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37, 41 };
	const std::map<char, int> SUIT_VALUE = { {'s', 8}, {'c', 4}, {'d', 2}, {'h', 1} };

	/*
	Constants that state:
	 - Number of distinct hands for each class (suit ignored)
	 - Starting rank of each class of hand
	*/
	const short DISTINCT_STRAIGHT_FLUSHES = 10;
	const short DISTINCT_QUADS = 156;
	const short DISTINCT_FULL_HOUSES = 156;
	const short DISTINCT_FLUSHES = 1277;
	const short DISTINCT_STRAIGHTS = 10;
	const short DISTINCT_TRIPS = 858;
	const short DISTINCT_TWO_PAIRS = 858;
	const short DISTINCT_ONE_PAIRS = 2860;
	const short DISTINCT_HIGH_CARDS = 1277;

	const short START_RANK_STRAIGHT_FLUSH = 1;
	const short START_RANK_QUADS = 11;
	const short START_RANK_FULL_HOUSE = 167;
	const short START_RANK_FLUSHES = 323;
	const short START_RANK_STRAIGHTS = 1600;
	const short START_RANK_TRIPS = 1610;
	const short START_RANK_TWO_PAIRS = 2468;
	const short START_RANK_ONE_PAIRS = 3326;
	const short START_RANK_HIGH_CARDS = 6186;


	/*
	Constants to support Bitmask operations
	*/
	const std::bitset<27> STRAIGHT_BIT_MASK = std::bitset<27>(std::string("11111")) <<= 14;
	const std::bitset<27> FLUSH_BIT_MASK = std::bitset<27>(std::string("1111")) <<= 10;
	const std::bitset<27> RANK_BIT_MASK = std::bitset<27>(std::string("1111")) <<= 6;

	//Straight prime values from straights (A to T) -> (5 to A)
	const int STRAIGHT_PRIME_VALS[10] = {
		31367009, 14535931, 6678671, 2800733,
		1062347, 323323, 85085, 15015, 2310, 8610
	};

	const int TABLE_SIZE = 7642;

}