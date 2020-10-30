#include "Position.h"

Position::Position() {
	kingsBB = 0u;
	byColorBB[WHITE] = 0b111111111111u;
	byColorBB[BLACK] = 0b11111111111100000000000000000000u;
	toMove = WHITE;
}

void Position::makeMove(Move m) {

	int squares = squareCount(m);
	bool capture = isCapture(m);

	Square sq1 = firstSquare(m);
	m = popSquare(m);

	bool kingMoved = (1u << sq1) & kingsBB;

	Color enemyCol = ~toMove;
	const Bitboard kingsRow = toMove == WHITE ? 0xF0000000u : 0x0000000Fu;

	// Single Capture or Quiet
	if (squares == 2) {

		//2nd square
		Square sq2 = firstSquare(m);

		if (capture) {

			/////////////Remove Enemy
			Direction dir = (sq2 - sq1) == 9 ? UPRIGHT : (sq2 - sq1) == 7 ? UPLEFT :
				(sq2 - sq1) == -9 ? DOWNLEFT : DOWNRIGHT;

			Bitboard enemy = ~shift(dir, 1u << sq1);

			byColorBB[enemyCol] &= enemy;
			kingsBB &= enemy;
		}

		byColorBB[toMove] ^= (1u << sq1) | (1u << sq2);

		if (kingMoved)
			kingsBB ^= (1u << sq1) | (1u << sq2);
		//PROMOTION
		else if ((1u << sq2) & kingsRow)
			kingsBB ^= (1u << sq2);

		toMove = ~toMove; // Switch sides
		return;
	}

	//MultiCapture
	Square sq = sq1;
	for (int i = 1; i < squares; ++i) {

		Square sq2 = firstSquare(m);
		m = popSquare(m);

		/////////////Remove Enemies
		Direction dir = (sq2 - sq) == 9 ? UPRIGHT : (sq2 - sq) == 7 ? UPLEFT :
			(sq2 - sq) == -9 ? DOWNLEFT : DOWNRIGHT;

		Bitboard enemy = ~shift(dir, 1u << sq);

		byColorBB[enemyCol] &= enemy;
		kingsBB &= enemy;

		sq = sq2;
	}

	byColorBB[toMove] ^= (1u << sq1) | (1u << sq);

	if (kingMoved)
		kingsBB ^= (1u << sq1) | (1u << sq);
	//PROMOTION
	else if ((1u << sq) & kingsRow)
		kingsBB ^= (1u << sq);

	toMove = ~toMove; // Switch sides
}

void Position::printBoard() const
{
	std::cout << "+---+---+---+---+---+---+---+---+\n";

	for (int rank = 7; rank >= 0; --rank) {
		for (int file = 0; file < 8; ++file) {

			if ((rank + file) & 1) {
				std::cout << "|   ";
				continue;
			}

			Bitboard sq = 1u << ((8 * rank + file) / 2);

			if (sq & mans(WHITE)) {
				std::cout << "| M ";
				continue;
			}
			else if (sq & mans(BLACK)) {
				std::cout << "| m ";
				continue;
			}
			else if (sq & kings(WHITE)) {
				std::cout << "| K ";
				continue;
			}
			else if(sq & kings(BLACK)) {
				std::cout << "| k ";
				continue;
			}

			std::cout << "|   ";
		}
		std::cout << "| " << (rank + 1) << "\n+---+---+---+---+---+---+---+---+\n";
	}

	std::wcout << "  a   b   c   d   e   f   g   h\n";
}

Matrix<128, 1> Position::toVector(Color col) const
{
	Matrix<128, 1> ret = Matrix<128, 1>();

	Bitboard ourMans = mans(col);
	Bitboard ourKings = kings(col);
	Bitboard theirMans = mans(~col);
	Bitboard theirKings = kings(~col);

	for (int sq = 0; sq < 32; ++sq) {

		Bitboard p = 1u << sq;
		int BaseIndex = col == WHITE ? 4 * sq : 4 * (31 - sq);
		
		if (p & ourMans)
			ret.val[BaseIndex + 0][0] = 1;
		else if (p & ourKings)
			ret.val[BaseIndex + 1][0] = 1;
		else if (p & theirMans)
			ret.val[BaseIndex + 2][0] = 1;
		else if (p & theirKings)
			ret.val[BaseIndex + 3][0] = 1;
	}

	return ret;
}

