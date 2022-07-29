#pragma once
#include <stdint.h>

typedef uint32_t bitsection;
// length in bits
#define BITSECTION_LENGTH 32u

struct bitsectionSet
{
	// constructors/destructors
	bitsectionSet() = default;
	bitsectionSet(uint32_t sectionAmt)
	{
		count = sectionAmt;
		bitCount = count * BITSECTION_LENGTH;
		sections = new bitsection[count];
	}
	~bitsectionSet() { delete[] sections; }

	// variables

	uint32_t bitCount;
	uint32_t count;
	// section[0] = most significant
	bitsection* sections;

	// operator overloads
	bitsectionSet& operator+=(bitsection obs);
	bitsectionSet& operator+=(bitsectionSet& obset);
	bitsectionSet& operator-=(bitsection obs);
	bitsectionSet& operator*=(bitsection obs);
	bitsectionSet& operator*=(bitsectionSet& obset);
	bitsectionSet& operator<<=(uint32_t shift);
	bitsectionSet& operator>>=(uint32_t shift);
	inline bitsection& operator[](uint32_t index);
};
