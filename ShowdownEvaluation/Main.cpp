#include <iostream>
#include "..\Texas Holdem Showdown Evaluator\EvaluatorLibrary.h"

int main(int argc, char* argv[]) {
	std::cout << "Hello World\n\n";
	/*char p1[5] = "JsTs";
	char p2[5] = "4c5s";
	char b[11] = "AsQsKs4d3c";
	int winner = Get_Winner(p1, p2, b);
	std::cout << winner << "\n";*/

	char p1_1[5] = "4s5s";
	char p2_1[5] = "AcAd";
	char b_1[11] = "AsAh2s3s8h";

	int winner_1 = Get_Winner(p1_1, p2_1, b_1);
	std::cout << winner_1 << "\n";




}