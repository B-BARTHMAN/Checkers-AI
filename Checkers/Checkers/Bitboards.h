#pragma once
#include "types.h"
#include <intrin.h>
#include <nmmintrin.h>

constexpr Bitboard lefties = 0x0F0F0F0Fu;
constexpr Bitboard FILEABB = 0x01010101u;
constexpr Bitboard FILEHBB = 0x80808080u;

template<Direction D>
constexpr Bitboard shift(Bitboard b) {
	return D == UPLEFT ? (b << (3 + ((b & lefties) == 0))) * ((b & FILEABB) == 0)
		: D == UPRIGHT ? (b << (4 + ((b & lefties) == 0))) * ((b & FILEHBB) == 0)
		: D == DOWNLEFT ? (b >> (4 + ((b & lefties) != 0))) * ((b & FILEABB) == 0)
		: D == DOWNRIGHT ? (b >> (3 + ((b & lefties) != 0))) * ((b & FILEHBB) == 0)
		: 0;
}

inline Bitboard shift(Direction D, Bitboard b) {
	return D == UPLEFT ? (b << (3 + ((b & lefties) == 0))) * ((b & FILEABB) == 0)
		: D == UPRIGHT ? (b << (4 + ((b & lefties) == 0))) * ((b & FILEHBB) == 0)
		: D == DOWNLEFT ? (b >> (4 + ((b & lefties) != 0))) * ((b & FILEABB) == 0)
		: D == DOWNRIGHT ? (b >> (3 + ((b & lefties) != 0))) * ((b & FILEHBB) == 0)
		: 0;
}

inline unsigned int lsb(Bitboard b) {
	unsigned long bit;
	_BitScanForward(&bit, b);
	return bit;
}

inline unsigned int pop_lsb(Bitboard& b) {
	unsigned long bit;
	_BitScanForward(&bit, b);
	b &= ~(1u << bit);
	return bit;
}

template<Direction D>
inline Bitboard shiftBitboard(Bitboard b) {
	Bitboard empty = 0u;
	while (b) {
		empty |= shift<D>(1u << pop_lsb(b));
	}
	return empty;
}