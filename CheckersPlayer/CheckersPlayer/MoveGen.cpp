#include "MoveGen.h"
#include "Bitboards.h"

#pragma region Definitions

template <Color Us>
void generateManQuiets(const Position&, std::vector<Move>&);

template<Color Us>
void generateKingQuiets(const Position&, std::vector<Move>&);

template<Color Us>
void generateQuiets(const Position&, std::vector<Move>&);

template<Color Us, bool isKing>
void generateMultis(std::vector<Move>&, Bitboard, Bitboard, Bitboard, Move);

template<Color Us>
void generateCaptures(const Position&, std::vector<Move>&);

template<Color Us>
void generateSingles(const Position&, std::vector<Move>&);

template<Color Us>
void generateSingle(const Position&, Bitboard, std::vector<Move>&, Move);

#pragma endregion

MoveGen::MoveGen(const Position& pos)
	:position(pos)
{
	//WHITE
	if (pos.toMove == WHITE) {
		generateCaptures<WHITE>(pos, moves);
		//Captures are mandatory
		if (moves.size())
			return;

		generateQuiets<WHITE>(pos, moves);
		return;
	}
	//BLACK
	generateCaptures<BLACK>(pos, moves);
	//Captures are mandatory
	if (moves.size())
		return;

	generateQuiets<BLACK>(pos, moves);
}

MoveGen::MoveGen(const Position& pos, Square lastMoved)
	:position(pos)
{
	if (lastMoved != NOSQUARE) {

		Bitboard piece = 1u << lastMoved;

		Move move = makeCapture();
		move = addSquare(move, lastMoved);

		if (pos.toMove == WHITE)
			generateSingle<WHITE>(pos, piece, moves, move);
		else
			generateSingle<BLACK>(pos, piece, moves, move);
		
		return;
	}

	if (pos.toMove == WHITE) {

		generateSingles<WHITE>(pos, moves);
		if (moves.size())
			return;

		generateQuiets<WHITE>(pos, moves);
		return;
	}
	else {
		generateSingles<BLACK>(pos, moves);
		if (moves.size())
			return;

		generateQuiets<BLACK>(pos, moves);
		return;
	}
}

template<Color Us>
void generateManQuiets(const Position& pos, std::vector<Move>& moveList) {

	constexpr Direction UpLeft = Us == WHITE ? UPLEFT : DOWNRIGHT;
	constexpr Direction UpRight = Us == WHITE ? UPRIGHT : DOWNLEFT;

	Bitboard upleft, upright;
	upleft = upright = pos.mans(Us);

	while (upleft) {

		unsigned int from = pop_lsb(upleft);

		Bitboard newPos = shift<UpLeft>(1u << from);
		newPos &= ~pos.pieces();

		if (newPos) 
			moveList.push_back(makeMove(Square(from), Square(lsb(newPos))));
		
	}

	while (upright) {

		unsigned int from = pop_lsb(upright);

		Bitboard newPos = shift<UpRight>(1u << from);
		newPos &= ~pos.pieces();

		if (newPos) 
			moveList.push_back(makeMove(Square(from), Square(lsb(newPos))));
		
	}

}

template<Color Us>
void generateKingQuiets(const Position& pos, std::vector<Move>& moveList) {

	Bitboard kings;

#pragma region Direction
	kings = pos.kings(Us);
	while (kings) {

		unsigned int from = pop_lsb(kings);

		Bitboard newPos = shift<UPLEFT>(1u << from);
		newPos &= ~pos.pieces();

		if (newPos)
			moveList.push_back(makeMove(Square(from), Square(lsb(newPos))));

	}
#pragma endregion
#pragma region Direction
	kings = pos.kings(Us);
	while (kings) {

		unsigned int from = pop_lsb(kings);

		Bitboard newPos = shift<UPRIGHT>(1u << from);
		newPos &= ~pos.pieces();

		if (newPos)
			moveList.push_back(makeMove(Square(from), Square(lsb(newPos))));

	}
#pragma endregion
#pragma region Direction
	kings = pos.kings(Us);
	while (kings) {

		unsigned int from = pop_lsb(kings);

		Bitboard newPos = shift<DOWNRIGHT>(1u << from);
		newPos &= ~pos.pieces();

		if (newPos)
			moveList.push_back(makeMove(Square(from), Square(lsb(newPos))));

	}
#pragma endregion
#pragma region Direction
	kings = pos.kings(Us);
	while (kings) {

		unsigned int from = pop_lsb(kings);

		Bitboard newPos = shift<DOWNLEFT>(1u << from);
		newPos &= ~pos.pieces();

		if (newPos)
			moveList.push_back(makeMove(Square(from), Square(lsb(newPos))));

	}
#pragma endregion
}

template<Color Us>
inline void generateQuiets(const Position& pos, std::vector<Move>& moveList) {
	generateManQuiets<Us>(pos, moveList);
	generateKingQuiets<Us>(pos, moveList);
}

template<Color Us, bool isKing>
void generateMultis(std::vector<Move>& moveList, Bitboard piece, Bitboard all, Bitboard enemy, Move move) {

	constexpr Direction UpLeft = Us == WHITE ? UPLEFT : DOWNRIGHT;
	constexpr Direction UpRight = Us == WHITE ? UPRIGHT : DOWNLEFT;
	constexpr Direction DownRight = Us == WHITE ? DOWNRIGHT : UPLEFT;
	constexpr Direction DownLeft = Us == WHITE ? DOWNLEFT : UPRIGHT;

	constexpr Bitboard KingsRow = Us == WHITE ? 0xF0000000u : 0xFu;

	move = addSquare(move, (Square)lsb(piece));
	Bitboard newPiece, newEnemy, newAll;
	bool canJump = false;

	//Move terminates when reaching kingsrow
	if(!isKing && (piece & KingsRow)) {
		moveList.push_back(move);
		return;
	}


#pragma region Direction
	newPiece = shift<UpLeft>(piece) & enemy;

	newAll = all ^ newPiece;
	newEnemy = enemy ^ newPiece;
	newPiece = shift<UpLeft>(newPiece) & (~newAll);

	if (newPiece) {
		generateMultis<Us, isKing>(moveList, newPiece, newAll, newEnemy, move);
		canJump = true;
	}
#pragma endregion
#pragma region Direction
	newPiece = shift<UpRight>(piece) & enemy;

	newAll = all ^ newPiece;
	newEnemy = enemy ^ newPiece;
	newPiece = shift<UpRight>(newPiece) & (~newAll);

	if (newPiece) {
		generateMultis<Us, isKing>(moveList, newPiece, newAll, newEnemy, move);
		canJump = true;
	}
#pragma endregion
	if (isKing) {
#pragma region Direction
		newPiece = shift<DownRight>(piece) & enemy;

		newAll = all ^ newPiece;
		newEnemy = enemy ^ newPiece;
		newPiece = shift<DownRight>(newPiece) & (~newAll);

		if (newPiece) {
			generateMultis<Us, isKing>(moveList, newPiece, newAll, newEnemy, move);
			canJump = true;
		}
#pragma endregion
#pragma region Direction
		newPiece = shift<DownLeft>(piece) & enemy;

		newAll = all ^ newPiece;
		newEnemy = enemy ^ newPiece;
		newPiece = shift<DownLeft>(newPiece) & (~newAll);

		if (newPiece) {
			generateMultis<Us, isKing>(moveList, newPiece, newAll, newEnemy, move);
			canJump = true;
		}
#pragma endregion
	}

	if (!canJump && squareCount(move) > 1) {
		moveList.push_back(move);
	}

}

template<Color Us>
void generateCaptures(const Position& pos, std::vector<Move>& moveList) {

	Bitboard pieces = pos.mans(Us);
	Bitboard kings = pos.kings(Us);
	Bitboard enemy = pos.pieces(~Us);
	Bitboard all;

	while (pieces) {

		Move move = makeCapture();
		Bitboard piece = 1ull << ((Square)pop_lsb(pieces));
		all = pos.pieces() ^ piece;

		generateMultis<Us, false>(moveList, piece, all, enemy, move);
	}

	while (kings) {

		Move move = makeCapture();
		Bitboard king = 1ull << ((Square)pop_lsb(kings));
		all = pos.pieces() ^ king;

		generateMultis<Us, true>(moveList, king, all, enemy, move);
	}
}

template<Color Us>
void generateSingles(const Position& pos, std::vector<Move>& moveList) {

	Bitboard pieces = pos.mans(Us);
	Bitboard kings = pos.kings(Us);
	Bitboard enemy = pos.pieces(~Us);

	while (pieces) {
		Move move = makeCapture();
		Square s = (Square)pop_lsb(pieces);

		move = addSquare(move, s);

		Bitboard piece = 1ull << s;

		generateSingle<Us>(pos, piece, moveList, move);
	}
	while (kings) {
		Move move = makeCapture();
		Square s = (Square)pop_lsb(kings);

		move = addSquare(move, s);

		Bitboard piece = 1ull << s;

		generateSingle<Us>(pos, piece, moveList, move);
	}

}

template<Color Us>
void generateSingle(const Position&pos , Bitboard piece, std::vector<Move>& moveList, Move move) {

	constexpr Direction UpLeft = Us == WHITE ? UPLEFT : DOWNRIGHT;
	constexpr Direction UpRight = Us == WHITE ? UPRIGHT : DOWNLEFT;
	constexpr Direction DownRight = Us == WHITE ? DOWNRIGHT : UPLEFT;
	constexpr Direction DownLeft = Us == WHITE ? DOWNLEFT : UPRIGHT;

	bool isKing = pos.kings() & piece;

	Bitboard newPos = shift<UpLeft>(shift<UpLeft>(piece) & pos.pieces(~Us));
	newPos &= ~pos.pieces();
	if (newPos) {
		moveList.push_back(addSquare(move, (Square)lsb(newPos)));
	}

	newPos = shift<UpRight>(shift<UpRight>(piece) & pos.pieces(~Us));
	newPos &= ~pos.pieces();
	if (newPos) {
		moveList.push_back(addSquare(move, (Square)lsb(newPos)));
	}

	if (!isKing)
		return;

	newPos = shift<DownLeft>(shift<DownLeft>(piece) & pos.pieces(~Us));
	newPos &= ~pos.pieces();
	if (newPos) {
		moveList.push_back(addSquare(move, (Square)lsb(newPos)));
	}

	newPos = shift<DownRight>(shift<DownRight>(piece) & pos.pieces(~Us));
	newPos &= ~pos.pieces();
	if (newPos) {
		moveList.push_back(addSquare(move, (Square)lsb(newPos)));
	}

}