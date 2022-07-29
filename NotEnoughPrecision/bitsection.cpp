#include "bitsection.h"

const bitsection topBit = 1 << (BITSECTION_LENGTH - 1);

inline bitsection& bitsectionSet::operator[](uint32_t index)
{
	return sections[index];
}

bitsectionSet& bitsectionSet::operator+=(bitsection obs)
{
	uint32_t index = count - 1;
	bitsection presum = sections[index];
	bitsection sum = presum + obs;
	sections[index] = sum;
	// if our least significant section overflows, increment next significant section and see if that overflows
	while(sum < presum && index != 0)
	{
		index--;
		presum = sections[index];
		sum = presum + 1; // if this overflows again, the while loop will continue
		sections[index] = sum;
	}

	return *this;
}

bitsectionSet& bitsectionSet::operator+=(bitsectionSet& obset)
{
	uint32_t index = count - 1;
	bitsection presum;
	bitsection sum;
	bitsection carry = 0; // this is either a zero or a one. 
	while (index != 0)
	{
		index--;
		presum = sections[index];
		sum = presum + obset.sections[index];
		sections[index] = sum + carry;
		carry = presum > sum ? 1 : 0; // set carry to 1 if we overflow into the next bitsection.
	}

	return *this;
}

bitsectionSet& bitsectionSet::operator-=(bitsection obs)
{
	uint32_t index = count - 1;
	bitsection presub = sections[index];
	bitsection subbedValue = presub - obs;
	sections[index] = subbedValue;
	// if our least significant section underflows, decrement next significant section and so on.
	while (subbedValue > presub && index != 0)
	{
		index--;
		presub = sections[index];
		subbedValue = presub - 1;
		sections[index] = subbedValue;
	}

	return *this;
}
// NOT CURRENTLY WORKING. ONLY HANDLES LEAST SIG SECTION
bitsectionSet& bitsectionSet::operator*=(bitsection obs)
{
	bitsection sectionCache = sections[count - 1];
	bitsection res = 0;
	while (obs != 0)
	{
		if (obs & 1)
		{
			res += sectionCache;
			sectionCache <<= 1;
			obs >>= 1;
		}
		return *this += res;

	}
}
// NOT CURRENTLY WORKING
bitsectionSet& bitsectionSet::operator*=(bitsectionSet& obs)
{
	uint32_t index = count - 1;
	bitsection carry = 0;
	bitsection cachedSection;
	bitsection product;
	while (index != 0)
	{
		cachedSection = sections[index];
		product = cachedSection * obs.sections[index];
		sections[index] = product + carry;
		carry = cachedSection - (product >> );

	}
	for (int i = count - 1; i >= 0; i--)
	{
		cachedSection = sections[i];
		product = cachedSection * obs.sections[i];
		while()
		if (product < cachedSection)
		{
			
		}
	}
}

bitsectionSet& bitsectionSet::operator<<=(uint32_t shift)
{
	bitsection carryStartBit = BITSECTION_LENGTH - shift;
	// isolate length of "shift" bits starting at "carryStartBit"
	bitsection carryMask = ((1u << shift) - 1u) << carryStartBit;
	bitsection carry = 0;
	bitsection sectCache;
	for (int i = count - 1; i >= 0; i--)
	{
		sectCache = sections[i];
		sections[i] = (sectCache << shift) | carry; // shift this bitsection and then apply the carry from the last section
		// grab the top x bits from our section cache (pre-shift)
		//and move them so they'll be the lowest bits in the next section
		carry = (sectCache & carryMask) >> carryStartBit;
	}
	return *this;
}
bitsectionSet& bitsectionSet::operator>>=(uint32_t shift)
{
	// isolate length of "shift" bits
	bitsection carryMask = ((1u << shift) - 1u);
	bitsection carry = 0;
	bitsection sectCache;
	for (int i = 0; i < count; i++)
	{
		sectCache = sections[i];
		sections[i] = (sectCache >> shift) | carry; // shift this bitsection and then apply the carry from the last section
		// grab the lower x bits from our section cache (pre-shift)
		//and move them so they'll be the highest bits in the next section
		carry = (sectCache & carryMask);
	}
	return *this;
}