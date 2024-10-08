#pragma once
#include <string>
#include <utility>


#ifdef EVALUATOR_EXPORTS
#define HAND_EVALUATOR_API __declspec(dllexport)
#else
#define HAND_EVALUATOR_API __declspec(dllexport)
#endif

struct EquityData {
	float playerOneEquity;
	float playerTwoEquity;
	float tieFrequency;
};

/**
 * @brief Initializes evaluator object by creating the rank search table.
 */
extern "C" HAND_EVALUATOR_API void InitEvaluator();

/**
 * @brief Closes evaluator object, removing it from the heap.
 */
extern "C" HAND_EVALUATOR_API void CloseEvaluator();

/**
 * @brief Finds the winner at showdown between 2 players.
 * @param player_one 
 * @param player_two 
 * @param board 
 * @return 1 if player 1 wins, -1 if player 2 wins, 0 if it is a draw.
 */
extern "C" HAND_EVALUATOR_API int Get_Winner_Old_One(std::string player_one, std::string player_two, std::string board);
extern "C" HAND_EVALUATOR_API int Get_Winner_Old_Two(std::string player_one, std::string player_two, std::string board);
extern "C" HAND_EVALUATOR_API int Get_Winner(std::string player_one, std::string player_two, std::string board);

extern "C" HAND_EVALUATOR_API EquityData EquityFromFlop(std::string playerOne, std::string playerTwo, std::string board);

extern "C" HAND_EVALUATOR_API EquityData EquityFromTurn(std::string playerOne, std::string playerTwo, std::string board);

/**
 * @brief Calculates equity before river card is dealt using a monte carlo sim.
 * @param player_one 
 * @param player_two 
 * @param board 
 * @return equity of player 1 ; (1 - player 1 equity) = player 2 equity.
 */
extern "C" HAND_EVALUATOR_API EquityData MonteCarloEquity(std::string player_one,
																std::string player_two,
																std::string board,
																int iterations);


