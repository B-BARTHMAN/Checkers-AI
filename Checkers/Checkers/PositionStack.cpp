#include "PositionStack.h"
#include<fstream>
#include<cstdlib>

void PositionStack::save(const char* file) const
{
	std::ofstream output(file, std::ios::binary | std::ios::out | std::ios::trunc);

	unsigned int c = current;
	output.write(reinterpret_cast<const char*>(&c), sizeof(unsigned int));

	for (unsigned int i = 0; i < c; ++i) {

		//Write position
		output.write(reinterpret_cast<const char*>(&positions[i].pos.byColorBB[WHITE]), sizeof(Bitboard));
		output.write(reinterpret_cast<const char*>(&positions[i].pos.byColorBB[BLACK]), sizeof(Bitboard));
		output.write(reinterpret_cast<const char*>(&positions[i].pos.kingsBB), sizeof(Bitboard));
		output.write(reinterpret_cast<const char*>(&positions[i].pos.toMove), sizeof(Color));

		//Write outcomes
		output.write(reinterpret_cast<const char*>(&positions[i].expectedOutcome), sizeof(float));
		output.write(reinterpret_cast<const char*>(&positions[i].trueOutcome), sizeof(float));

	}

	output.close();
}

void PositionStack::load(const char* file)
{
	std::ifstream input(file, std::ios::binary | std::ios::in);
	
	unsigned int c;

	input.read((char*)&c, sizeof(unsigned int));
	current = c;

	for (unsigned int i = 0; i < c; ++i) {

		//read position
		input.read((char*)&positions[i].pos.byColorBB[WHITE], sizeof(Bitboard));
		input.read((char*)&positions[i].pos.byColorBB[BLACK], sizeof(Bitboard));
		input.read((char*)&positions[i].pos.kingsBB, sizeof(Bitboard));
		input.read((char*)&positions[i].pos.toMove, sizeof(Bitboard));

		//Write outcomes
		input.read((char*)&positions[i].expectedOutcome, sizeof(float));
		input.read((char*)&positions[i].trueOutcome, sizeof(float));

	}

	input.close();
}
