#pragma once
#include <stdint.h>
#include <cstring>

typedef uint32_t bitsection;
// length in bits
#define BITSECTION_LENGTH 32

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
	bitsectionSet(bitsection sectionArray[], uint32_t arraySize, bool copyArray);
	bitsectionSet(const bitsectionSet& copy);

	~bitsectionSet() { delete[] sections; }

	// variables

	uint32_t bitCount;
	uint32_t count;
	// section[0] = least significant
	bitsection* sections;

	bool isZero();

	// operator overloads
	inline bitsectionSet& operator+=(bitsection obs);
	bitsectionSet& operator+=(const bitsectionSet& obset);
	bitsectionSet& operator-=(bitsection obs);
	bitsectionSet& operator*=(bitsection obs);
	bitsectionSet& operator*=(const bitsectionSet& obset);
	bitsectionSet& operator<<=(uint32_t shift);
	bitsectionSet& operator>>=(uint32_t shift);
	inline bitsection& operator[](const uint32_t index) const;
	inline bitsection& operator[](const int index) const;
	//operator int() const;
private:
	bitsection AddAtSection(bitsection addition, uint32_t index);
};
