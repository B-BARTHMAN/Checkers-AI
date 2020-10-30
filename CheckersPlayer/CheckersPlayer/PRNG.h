#pragma once
#include <chrono>
#include <atomic>

class PRNG
{
	std::atomic_size_t s;

public:

	PRNG(size_t seed)
		:s(seed)
	{}
	PRNG() {
		s = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
	}
	inline size_t rand64() {
		s ^= s >> 12; s ^= s << 25; s ^= s >> 27;
		return s * 2685821657736338717LL;
	}
};

extern PRNG s_PRNG;

