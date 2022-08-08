#pragma once
#include <stdint.h>
#include <cstring>

typedef uint32_t bitsection;
// length in bits
const uint32_t BITSECTION_LENGTH = sizeof(bitsection) * 8;

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
	// section[0] = most significant
	bitsection* sections;

	bool isZero();

	// operator overloads
	bitsectionSet& operator+=(bitsection obs);
	bitsectionSet& operator+=(bitsectionSet& obset);
	bitsectionSet& operator-=(bitsection obs);
	bitsectionSet& operator*=(bitsection obs);
	bitsectionSet& operator*=(const bitsectionSet& obset);
	bitsectionSet& operator<<=(uint32_t shift);
	bitsectionSet& operator>>=(uint32_t shift);
	inline bitsection& operator[](uint32_t index);
	//operator int() const;
private:
	bitsection AddAtSection(bitsection addition, uint32_t index);
};
