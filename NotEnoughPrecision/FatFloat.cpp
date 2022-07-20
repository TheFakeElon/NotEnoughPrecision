#include "FatFloat.h"

FatFloat::FatFloat(uint32_t expoLength, uint32_t fractLength, uint8_t _sign)
{
	sign = _sign;
	exponentBitLength = expoLength;
	exponentSectionCount = (exponentBitLength + BITSECTION_LENGTH - 1) / BITSECTION_LENGTH;
	exponentBitsectionArray = new bitsection(exponentSectionCount);

	bias = pow(2u, exponentBitLength - 1) - 1;

	mantissaBitLength = fractLength;
	mantissaSectionCount = (mantissaBitLength + BITSECTION_LENGTH - 1) / BITSECTION_LENGTH;
	mantissaBitsectionArray = new bitsection(mantissaSectionCount);

}

FatFloat::~FatFloat()
{
	delete[] exponentBitsectionArray;
	delete[] mantissaBitsectionArray;
}

FatFloat& FatFloat::operator+=(const FatFloat& FF)
{
	/*
	if (bitsectionArrayLength != FF.bitsectionArrayLength)
	{
		return; //TODO
	}
	bitsection a;
	bitsection b;
	bitsection carry = 0;
	for (auto i = 0; i < bitsectionArrayLength; i++)
	{
		a = bitsectionArray[i];
		b = FF.bitsectionArray[i];
		bitsectionArray[i] = (a ^ b) ^ carry;
		carry = (a & b) | (a & carry) | (b & carry);
	}
	*/

	return *this;
}

FatFloat& FatFloat::operator*=(const FatFloat& FF)
{
	sign = sign ^ FF.sign;
	for (auto i = 0; mantissaSectionCount > i; i++)
	{
		mantissaBitsectionArray[i] *= FF.mantissaBitsectionArray[i];
		exponentBitsectionArray[i] += FF.exponentBitsectionArray[i] - FF.bias;
	}
	
}
#if BITSECTION_LENGTH == 32u
#define BIT_SCAN(index, mask) _BitScanReverse(index, mask)
#elif BITSECTION_LENGTH == 64u
#define BIT_SCAN(index, mask) _BitScanReverse64(index, mask)
#endif

inline void FatFloat::normalize()
{
	// most significant bit
	uint32_t arrayIndex = 0;
	unsigned long MSB = 0;
	while (BIT_SCAN(&MSB, mantissaBitsectionArray[arrayIndex]) == 0)
	{
		arrayIndex++;
	}
	if (MSB == 0 || (arrayIndex == 0 && MSB == BITSECTION_LENGTH) )
		return;
	auto shift = BITSECTION_LENGTH - MSB;
	// isolate length of "shift" bits starting at "MSB"
	bitsection carryMask = ((1 << shift) - 1) << MSB;
	bitsection carry = 0;
	bitsection sectCache;
	for (int i = static_cast<int>(mantissaSectionCount) - 1; i >= 0; i--)
	{
		sectCache = mantissaBitsectionArray[i];
		mantissaBitsectionArray[i] = (sectCache << shift) | carry; // shift this bitsection and then apply the carry from the last section
		carry = (sectCache & carryMask) >> MSB;
	}
	arrayIndex = 0;
	bitsection* bsectionPtr = &exponentBitsectionArray[0];
	while (*bsectionPtr == 0)
	{
		arrayIndex++;
		bsectionPtr = &exponentBitsectionArray[arrayIndex];
	}
	exponentBitsectionArray[exponentSectionCount - 1u] -= shift;

}