#include <stdint.h>
#include <cmath>

typedef uint32_t bitsection;
// length in bits
#define BITSECTION_LENGTH 32u

class FatFloat
{
public:
	FatFloat(uint32_t expoLength, uint32_t fractLength, uint8_t _sign);
	~FatFloat();

	uint8_t sign;
	uint64_t bias;

	uint32_t exponentBitLength;
	uint32_t exponentSectionCount;
	bitsection* exponentBitsectionArray;

	uint32_t mantissaBitLength;
	uint32_t mantissaSectionCount;
	bitsection* mantissaBitsectionArray;

	FatFloat& operator = (const FatFloat& FF);
	bool operator == (const FatFloat& FF);

	FatFloat& operator+= (const FatFloat& FF);

	FatFloat& operator*= (const FatFloat& FF);

private:
	inline void normalize();
	inline void sectionSubtract(bitsection* B, bitsection subtract)
};
