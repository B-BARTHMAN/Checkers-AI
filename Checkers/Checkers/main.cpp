#include "Environment.h"
int main() {

	Environment env;
	env.load();
	env.start();

	return 0;
}

//#include "TestEnvironment.h"
//int main() {
//	TestEnvironment env;
//	env.load(30, 20);
//	env.play();
//	env.wait();
//
//	std::cin.get();
//}

//#include "MCSearch.h"
//#include "NeuralNetInf.h"
//NeuralNetInf network0;
//NeuralNetInf network1;
//float evaluate0(const Position& pos) {
//	return network0.forward(pos.toVector(pos.toMove));
//}
//float evaluate1(const Position& pos) {
//	return network1.forward(pos.toVector(pos.toMove));
//}
//int main() {
//	network0.load("Model/Generation20");
//	network1.load("Model/Generation30");
//	Position pos;
//	//PLY Loop
//	for (int ply = 0; ply <= 128; ++ply) {
//
//		//pos.printBoard();
//
//		if (MoveGen(pos).moves.size() == 0)
//			break;
//
//		if (ply & 0b1) {
//			MCSearchResult result = MCSearch(pos, evaluate1, 0);
//			std::cout << "BLACK: " << result.outcome << "\n";
//			pos.makeMove(result.mostPlayed());
//		}
//		else {
//			MCSearchResult result = MCSearch(pos, evaluate1, 0);
//			std::cout << "WHITE: " << result.outcome << "\n";
//			pos.makeMove(result.mostPlayed());
//		}
//	}
//	std::cin.get();
//}

//#include "TestRun.h"
//int main() {
//
//	TestRun();
//
//	std::cin.get();
//	return 0;
//}
