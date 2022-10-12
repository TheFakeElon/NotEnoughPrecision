#include "FatFloat.h"

FatFloat::FatFloat(uint32_t expoLength, uint32_t fractLength, bool _sign)
{
	sign = _sign;
	uint32_t sectionLength = (exponentSet.bitCount + BITSECTION_LENGTH - 1u) / BITSECTION_LENGTH;
	exponentSet = bitsectionSet(sectionLength);

	bias = (bitsection)pow(2u, exponentSet.bitCount - 1) - 1;

	sectionLength = (mantissaSet.bitCount + BITSECTION_LENGTH - 1u) / BITSECTION_LENGTH;
	mantissaSet = bitsectionSet(sectionLength);

}
//todo: include string
FatFloat::FatFloat(std::string digitStr)
{
	const uint32_t digitsPerSection = (uint32_t)log10(pow(2, BITSECTION_LENGTH)) + 1;
	uint32_t len = digitStr.length();
	
	for (int i = len - 1; i >= 0; i--)
	{
		
	}
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
	sign ^= FF.sign;

	mantissaSet *= FF.mantissaSet;
	exponentSet += FF.exponentSet;
	// just realized bias will probably need to be a bitsection too :(. Consider ignoring IEEE 754 and using a bool identifier instead
	exponentSet -= FF.bias;

	return *this;
	
}
#if BITSECTION_LENGTH == 32
#define BIT_SCAN(index, mask) _BitScanReverse(index, mask)
#elif BITSECTION_LENGTH == 64
#define BIT_SCAN(index, mask) _BitScanReverse64(index, mask)
#endif

inline void FatFloat::normalize()
{
	// first bitsection with non-zero bits
	uint32_t arrayIndex = 0;
	// most significant bit
	unsigned long MSB = 0;
	while (BIT_SCAN(&MSB, mantissaSet[arrayIndex]) == 0)
	{
		arrayIndex++;
	}
	if (MSB == 0 || (arrayIndex == 0 && MSB == BITSECTION_LENGTH) )
		return;
	auto shift = BITSECTION_LENGTH - MSB;
	// normalize bitsection positions if we have leading zero sections. We compensate/balance the exponent for this later
	if (arrayIndex != 0)
	{
		for (auto i = arrayIndex; i < mantissaSet.count; i++)
		{
			mantissaSet[i - arrayIndex] = mantissaSet[i];
			mantissaSet[i] = 0;
		}
	}
	mantissaSet <<= shift;
	exponentSet -= shift + (BITSECTION_LENGTH * arrayIndex);

}