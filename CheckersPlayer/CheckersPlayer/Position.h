#pragma once
#include "types.h"
#include "Bitboards.h"
#include "Matrix.h"

class Position
{
public:
	Bitboard byColorBB[COLOR_NB];
	Bitboard kingsBB;
	Color toMove = WHITE;

public:

	Position();
	
	//pieces
	Bitboard pieces() const;
	Bitboard pieces(Color c) const;
	Bitboard kings() const;
	Bitboard kings(Color c) const;
	Bitboard mans() const;
	Bitboard mans(Color c) const;

	//capturable
	template<Color Us> Bitboard canCapture() const;

	//Moves
	void makeMove(Move m);
	Position doMove(Move m) const;

	//Debug
	void printBoard() const;

	//Convert to Matrix for NN
	Matrix<128, 1> toVector(Color col) const;
};

//pieces
inline Bitboard Position::pieces() const {
	return byColorBB[WHITE] | byColorBB[BLACK];
}
inline Bitboard Position::pieces(Color c) const {
	return byColorBB[c];
}
inline Bitboard Position::kings() const {
	return kingsBB;
}
inline Bitboard Position::kings(Color c) const {
	return byColorBB[c] & kingsBB;
}
inline Bitboard Position::mans() const {
	return pieces() & (~kings());
}
inline Bitboard Position::mans(Color c) const {
	return byColorBB[c] & (~kingsBB);
}
//capturable
template<Color Us> inline Bitboard Position::canCapture() const {

	constexpr Direction UpLeft = Us == WHITE ? UPLEFT : DOWNRIGHT;
	constexpr Direction UpRight = Us == WHITE ? UPRIGHT : DOWNLEFT;
	constexpr Direction DownRight = Us == WHITE ? DOWNRIGHT : UPLEFT;
	constexpr Direction DownLeft = Us == WHITE ? DOWNLEFT : UPRIGHT;

	Bitboard upleft, upright;
	upleft = upright = pieces(Us);

	upleft = shift<UpLeft>(upleft) & pieces(~Us);
	upleft = shift<UpLeft>(upleft) & ~pieces();

	upright = shift<UpRight>(upright) & pieces(~Us);
	upright = shift<UpRight>(upright) & ~pieces();

	return shift<DownRight>(shift<DownRight>(upleft))
		| shift<DownLeft>(shift<DownLeft>(upright));
}
//Moves
inline Position Position::doMove(Move m) const {
	Position p = *this;
	p.makeMove(m);
	return p;
}