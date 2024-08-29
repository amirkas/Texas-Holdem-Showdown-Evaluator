
#include "pch.h"
#include "EvaluatorLibrary.h"
#include "EvaluatorClass.h"
#include "CardBitMask.h"
#include "HandBitMask.h"
#include "vector"
#include <algorithm>
#include "HandEvaluationConstants.h"
#include "utility"
#include "combinations.h"

FastHandEvaluator* eval;
bool InitCalled = false;
std::string InitNotCalledErr = "InitEvaluator must be called before executing any other functions";

void InitEvaluator() {
	eval = new FastHandEvaluator();
	InitCalled = true;
}

void CloseEvaluator() {
	delete eval;
	InitCalled = false;
}

class f {
public:
	short* maxRank;
	FastHandEvaluator* evaluator;

	explicit f(short* pos, FastHandEvaluator* e) : maxRank(pos), evaluator(e) {}

	template<class It>
	bool operator()(It first, It last) {
		std::vector<CardBitMask*> combo;
		combo.insert(combo.begin(), first, last);
		HandBitMask* handMask = HandBitMask::CreateHandBitMask(combo);
		*maxRank = (std::min)(*maxRank, evaluator->GetHandRank(handMask));
		/*Clear memory on heap*/
		delete handMask;
		return false;
	}
};

int Get_Winner_Old_One(std::string player_one, std::string player_two, std::string board) {
	
	if (!InitCalled) {
		throw std::runtime_error(InitNotCalledErr);
	}

	/*Create Bitmask for each card for each player*/
	CardBitMask* player_one_cardmask_one = new CardBitMask(player_one.substr(0, 2));
	CardBitMask* player_one_cardmask_two = new CardBitMask(player_one.substr(2, 2));

	CardBitMask* player_two_cardmask_one = new CardBitMask(player_two.substr(0, 2));
	CardBitMask* player_two_cardmask_two = new CardBitMask(player_two.substr(2, 2));

	/*Separate board into Card Bitmasks*/
	std::vector<CardBitMask*> board_masks;
	for (int i = 0; i < board.size(); i += 2) {
		CardBitMask* board_card = new CardBitMask(board.substr(i, 2));
		board_masks.push_back(board_card);
	}

	/*Create card bitmask array for each player's cards combined with board cards*/
	std::vector<CardBitMask*> player_one_cards;
	player_one_cards.insert(player_one_cards.end(), board_masks.begin(), board_masks.end());
	player_one_cards.push_back(player_one_cardmask_one);
	player_one_cards.push_back(player_one_cardmask_two);

	std::vector<CardBitMask*> player_two_cards;
	player_two_cards.insert(player_two_cards.end(), board_masks.begin(), board_masks.end());
	player_two_cards.push_back(player_two_cardmask_one);
	player_two_cards.push_back(player_two_cardmask_two);


	/*Evaluate best possible hand rank for each player given community cards*/
	short player_one_best_rank = HandEvalConstants::START_RANK_HIGH_CARDS + 1;
	short player_two_best_rank = HandEvalConstants::START_RANK_HIGH_CARDS + 1;

	//Iterate through every combination of each player's possible hands
	//while tracking best hand rank (lowest value)

	for_each_combination(player_one_cards.begin(),
						 player_one_cards.begin() + 5,
						 player_one_cards.end(),
						 f(&player_one_best_rank, eval));
	for_each_combination(player_two_cards.begin(),
						 player_two_cards.begin() + 5,
						 player_two_cards.end(),
						 f(&player_two_best_rank, eval));

	/*while (std::next_permutation(player_one_cards.begin(), player_one_cards.end())) {
		std::next_permutation(player_two_cards.begin(), player_two_cards.end());
		std::vector<CardBitMask*> p_1_hand_arr(player_one_cards.begin(), player_one_cards.begin() + 5);
		HandBitMask* p_1_hand = HandBitMask::CreateHandBitMask(p_1_hand_arr);
		std::vector<CardBitMask*> p_2_hand_arr(player_two_cards.begin(), player_two_cards.begin() + 5);
		HandBitMask* p_2_hand = HandBitMask::CreateHandBitMask(p_2_hand_arr);

		player_one_best_rank = std::min(player_one_best_rank, eval->GetHandRank(p_1_hand));
		player_two_best_rank = std::min(player_two_best_rank, eval->GetHandRank(p_2_hand));

	}*/

	/*Free memory on heap*/
	player_one_cards.clear();
	player_two_cards.clear();
	board_masks.clear();


	if (player_one_best_rank < player_two_best_rank) {
		return 1;
	}
	else if (player_one_best_rank == player_two_best_rank) {
		return 0;
	}
	else {
		return -1;
	}
}

class g {
public:
	short* maxRankOne;
	short* maxRankTwo;
	FastHandEvaluator* evaluator;
	std::string playerOneStr;
	std::string playerTwoStr;

	explicit g(
		short* pPosOne, short* pPosTwo,  FastHandEvaluator* e,
		std::string one, std::string two
	)		: maxRankOne(pPosOne), maxRankTwo(pPosTwo), evaluator(e),
			playerOneStr(one), playerTwoStr(two) {}

	template<class It>
	bool operator()(It first, It last) {
		
		std::string totalHandOne = "";
		std::string totalHandTwo = "";
		for (first; first < last; first++) {
			totalHandOne.append(playerOneStr.substr(*first, 2));
			totalHandTwo.append(playerTwoStr.substr(*first, 2));
		}
		*maxRankOne = (std::min)(*maxRankOne, evaluator->GetHandRank(totalHandOne));
		*maxRankTwo = (std::min)(*maxRankTwo, evaluator->GetHandRank(totalHandTwo));
		return false;
	}
};

int Get_Winner_Old_Two(std::string player_one, std::string player_two, std::string board) {
	
	std::string totalPlayerOne = player_one + board;
	std::string totalPlayerTwo = player_two + board;
	if (totalPlayerOne.size() != 14 || totalPlayerTwo.size() != 14) {
		return -2;
	}
	std::vector<int> *pIndexArray = new std::vector<int>(7, 0);
	for (int iStr = 0; iStr < totalPlayerOne.size() / 2; iStr++) {
		pIndexArray->at(iStr) = iStr * 2;
	}
	/*Evaluate best possible hand rank for each player given community cards*/
	short player_one_best_rank = HandEvalConstants::START_RANK_HIGH_CARDS + 1;
	short player_two_best_rank = HandEvalConstants::START_RANK_HIGH_CARDS + 1;

	for_each_combination(
						pIndexArray->begin(),
						pIndexArray->begin() + 5,
						pIndexArray->end(),
						g(	&player_one_best_rank, &player_two_best_rank,
							eval, totalPlayerOne, totalPlayerTwo)
						);
	
	delete pIndexArray;
	if (player_one_best_rank < player_two_best_rank) {
		return 1;
	}
	else if (player_one_best_rank == player_two_best_rank) {
		return 0;
	}
	else {
		return -1;
	}
}


class h {
public:
	short* maxRank;
	FastHandEvaluator* evaluator;

	explicit h(short* pPosOne, FastHandEvaluator* e) : maxRank(pPosOne), evaluator(e) {}

	template<class It>
	bool operator()(It first, It last) {
		*maxRank = (std::min)(*maxRank, evaluator->GetHandRank(first, last));
		return false;
	}
};

int Get_Winner(std::string player_one, std::string player_two, std::string board) {

	std::vector<std::string>* playerOneStrArr = new std::vector<std::string>(1, player_one.substr(0, 2));
	std::vector<std::string>* playerTwoStrArr = new std::vector<std::string>(1, player_two.substr(0, 2));
	playerOneStrArr->push_back(player_one.substr(2, 2));
	playerTwoStrArr->push_back(player_two.substr(2, 2));

	for (int iStr = 0; iStr < board.size(); iStr += 2) {
		playerOneStrArr->push_back(board.substr(iStr, 2));
		playerTwoStrArr->push_back(board.substr(iStr, 2));
	}

	/*Evaluate best possible hand rank for each player given community cards*/
	short player_one_best_rank = HandEvalConstants::START_RANK_HIGH_CARDS + 1;
	short player_two_best_rank = HandEvalConstants::START_RANK_HIGH_CARDS + 1;

	for_each_combination(playerOneStrArr->begin(),
						 playerOneStrArr->begin() + 5,
						 playerOneStrArr->end(),
						 h(&player_one_best_rank, eval)
						);
	for_each_combination(playerTwoStrArr->begin(),
						 playerTwoStrArr->begin() + 5,
						 playerTwoStrArr->end(),
						 h(&player_two_best_rank, eval)
	);

	delete playerOneStrArr;
	delete playerTwoStrArr;
	if (player_one_best_rank < player_two_best_rank) {
		return 1;
	}
	else if (player_one_best_rank == player_two_best_rank) {
		return 0;
	}
	else {
		return -1;
	}
}


