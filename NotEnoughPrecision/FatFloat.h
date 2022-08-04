#include <stdint.h>
#include <cmath>
#include "bitsection.h"

class FatFloat
{
public:
	FatFloat(uint32_t expoLength, uint32_t fractLength, uint8_t _sign);
	FatFloat(char* numStr)
	~FatFloat();

	uint8_t sign;
	uint64_t bias;

	bitsectionSet exponentSet;

	bitsectionSet mantissaSet;

	FatFloat& operator = (const FatFloat& FF);
	bool operator == (const FatFloat& FF);

	FatFloat& operator+= (const FatFloat& FF);

	FatFloat& operator*= (const FatFloat& FF);

private:
	inline void normalize();
};
