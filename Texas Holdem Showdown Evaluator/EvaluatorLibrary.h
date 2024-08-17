#pragma once


#ifdef EVALUATOR_EXPORTS
#define HAND_EVALUATOR_API __declspec(dllexport)
#else
#define HAND_EVALUATOR_API __declspec(dllexport)
#endif

extern "C" HAND_EVALUATOR_API int Get_Winner(static char* player_one, static char* player_two, static char* board);
