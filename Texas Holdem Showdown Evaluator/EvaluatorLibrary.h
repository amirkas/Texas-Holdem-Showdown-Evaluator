#pragma once
#include <string>

#ifdef EVALUATOR_EXPORTS
#define HAND_EVALUATOR_API __declspec(dllexport)
#else
#define HAND_EVALUATOR_API __declspec(dllexport)
#endif

extern "C" HAND_EVALUATOR_API void InitEvaluator();

extern "C" HAND_EVALUATOR_API int Get_Winner(std::string player_one, std::string player_two, std::string board);
