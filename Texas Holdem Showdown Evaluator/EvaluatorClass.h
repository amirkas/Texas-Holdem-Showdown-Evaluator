#pragma once


#include <bitset>
#include <string>
#include <vector>
#include <unordered_map>
#include <iostream>
#include <utility>
#include <fstream>
#include <math.h>
#include "HandEvaluationConstants.h"


//Forward Declaration for CardBitMask and HandBitMask classes.
class CardBitMask;
class HandBitMask;

class FastHandEvaluator {

private:

	//Unordered maps map:
	//		 key (hand prime value) : value (hand ranking)
	std::unordered_map<int, short>* flushes;
	std::unordered_map<int, short>* paired;
	std::unordered_map<int, short>* unique_ranks;


	/* Private Function Declarations */

	/*Functions that generate lookup table for hand ranks*/
	void EvaluateAllHandRanks();

	static void GenerateStraightFlushesAndStraights(std::unordered_map<int, short>*, std::unordered_map<int, short>*);
	static void GenerateQuads(std::unordered_map<int, short>*);
	static void GenerateFullHouses(std::unordered_map<int, short>*);
	static void GenerateFlushes(std::unordered_map<int, short>*);
	static void GenerateTrips(std::unordered_map<int, short>*);
	static void GenerateTwoPairs(std::unordered_map<int, short>*);
	static void GenerateOnePairs(std::unordered_map<int, short>*);
	static void GenerateAllHighCards(std::unordered_map<int, short>*, int);

	/*Helper functions for lookup table generation and retrieval*/
	static int GetCardPrime(int);
	static bool IsHandStraight(std::vector<int>);
	static bool AreRanksUnique(std::vector<int>);


public:
	/* Constructor*/
	FastHandEvaluator();

	/* Deconstructor*/
	~FastHandEvaluator();

	/*Hand Evaluators that format hands for table lookup*/
	short GetHandRank(HandBitMask*);

	int EvaluateWinner(HandBitMask* OOP_hand, HandBitMask* IP_hand);

};