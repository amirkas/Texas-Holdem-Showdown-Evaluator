


#define NOMINAX

#include "EvaluatorLibrary.h"
#include "EvaluatorClass.h"
#include "CardBitMask.h"
#include "HandBitMask.h"
#include "string.h"
#include "vector"
#include "algorithm"
#include "HandEvaluationConstants.h"



int Get_Winner(char* player_one, char* player_two, char* board) {

	
	FastHandEvaluator *eval = new FastHandEvaluator();
	
	/*Create Bitmask for each card for each player*/
	CardBitMask* player_one_cardmask_one = new CardBitMask(player_one[0], player_one[1]);
	CardBitMask* player_one_cardmask_two = new CardBitMask(player_one[2], player_one[3]);

	CardBitMask* player_two_cardmask_one = new CardBitMask(player_two[0], player_two[1]);
	CardBitMask* player_two_cardmask_two = new CardBitMask(player_two[2], player_two[3]);

	/*Separate board into Card Bitmasks*/
	std::vector<CardBitMask*> board_masks;
	for (int i = 0; i < strlen(board); i += 2) {
		CardBitMask* board_card = new CardBitMask(board[i], board[i + 1]);
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

	//Iterate through every permutation of each player's possible hands
	//while tracking best hand rank (lowest value)
	while (std::next_permutation(player_one_cards.begin(), player_one_cards.end())) {
		std::next_permutation(player_two_cards.begin(), player_two_cards.end());
		std::vector<CardBitMask*> p_1_hand_arr(player_one_cards.begin(), player_one_cards.begin() + 5);
		HandBitMask* p_1_hand = HandBitMask::CreateHandBitMask(p_1_hand_arr);
		std::vector<CardBitMask*> p_2_hand_arr(player_two_cards.begin(), player_two_cards.begin() + 5);
		HandBitMask* p_2_hand = HandBitMask::CreateHandBitMask(p_2_hand_arr);

		player_one_best_rank = std::min(player_one_best_rank, eval->GetHandRank(p_1_hand));
		player_two_best_rank = std::min(player_two_best_rank, eval->GetHandRank(p_2_hand));

	}
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