#pragma once
#include <iostream>
// five bits define a square, there can be at most 10 squares

typedef unsigned int Bitboard;

constexpr int MAX_MOVES = 256;

//Bit 0 - 49: Squares, Bit 50 - 53 : SquareCount, Bit 54 : Capture
enum Move : unsigned long long {

};

enum Square : int {
	SQUARE_0, SQUARE_1, SQUARE_2, SQUARE_3,
	SQUARE_4, SQUARE_5, SQUARE_6, SQUARE_7,
	SQUARE_8, SQUARE_9, SQUARE_10, SQUARE_11,
	SQUARE_12, SQUARE_13, SQUARE_14, SQUARE_15,
	SQUARE_16, SQUARE_17, SQUARE_18, SQUARE_19,
	SQUARE_20, SQUARE_21, SQUARE_22, SQUARE_23,
	SQUARE_24, SQUARE_25, SQUARE_26, SQUARE_27,
	SQUARE_28, SQUARE_29, SQUARE_30, SQUARE_31,
	NOSQUARE

};

enum Direction {
	UPLEFT,
	UPRIGHT,
	DOWNLEFT,
	DOWNRIGHT
};

//for perft: http://laatste.info/bb3/viewtopic.php?t=2822
enum MoveSquare : unsigned long long int {
	MVSQ_0 = 31ull << 0 * 5,
	MVSQ_1 = 31ull << 1 * 5,
	MVSQ_2 = 31ull << 2 * 5,
	MVSQ_3 = 31ull << 3 * 5,
	MVSQ_4 = 31ull << 4 * 5,
	MVSQ_5 = 31ull << 5 * 5,
	MVSQ_6 = 31ull << 6 * 5,
	MVSQ_7 = 31ull << 7 * 5,
	MVSQ_8 = 31ull << 8 * 5,
	MVSQ_9 = 31ull << 9 * 5,
};

enum Color {
	WHITE, BLACK, COLOR_NB = 2
};

constexpr Color operator~(Color c) {
	return Color(c ^ BLACK); // Toggle color
}

// make a move with two squares
constexpr Move makeMove(Square a, Square b) {
	return Move(((size_t)b << 5) + (size_t)a +
	(2ull << 50));
}

// make a move with one square
constexpr Move makeMove(Square s) {
	return Move(((size_t)s) + (1ull << 50));
}

// make an empty move
constexpr Move makeMove() {
	return Move(0ull);
}

// return the number of squares a move covers
constexpr int squareCount(Move m) {
	return (((size_t)m) >> 50) & 15;
}

// add a square to a move
constexpr Move addSquare(Move m, Square s) {
	return Move((size_t)m //BaseMove
		+ ((size_t)s << (squareCount(m) * 5)) //New Square
		+ (1ull << 50)); //New MoveCount
}

// remove the first square of a move
constexpr Move popSquare(Move m) {
	return Move((((size_t)m & 0x3FFFFFFFFFFFFull) >> 5)
		+ ((size_t)m & 0x3C000000000000ull) 
		- (1ull << 50));
}

constexpr Square firstSquare(Move m) {
	return Square(((size_t)m) & 31ull);
}

// makes a move a capture
constexpr Move makeCapture(Move m) {
	return Move(((size_t)m) & (1ull << 54));
}

// creates an empty captureMove
constexpr Move makeCapture() {
	return Move(1ull << 54);
}

// test if a move is a capture
constexpr bool isCapture(Move m) {
	return ((size_t)m) & (1ull << 54);
}

// print the squares in a move for debugging
inline void enumerateMove(Move m) {

	/*
	std::cout << "The move: " << ((size_t)m) << " covers " << squareCount(m) << " Squares:\n";
	*/
	int count = squareCount(m);
	for (int i = 0; i < count; ++i) {
		std::cout << (((size_t)m) & 0b11111ull);
		m = popSquare(m);
		if (i != (count - 1))
			std::cout << " x ";
	}
	std::cout << "\n";

}