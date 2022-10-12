#include "bitsection.h"

const bitsection topBit = 1 << (BITSECTION_LENGTH - 1);


// ----------- CONSTRUCTORS -------------

bitsectionSet::bitsectionSet(bitsection sectionArray[], uint32_t arraySize, bool copyArray)
{
	count = arraySize / sizeof(bitsection);
	bitCount = count * BITSECTION_LENGTH;
	if (copyArray)
	{
		sections = new bitsection[count]();
		memcpy(sectionArray, sections, arraySize);
	}
	else
	{
		sections = sectionArray;
	}
}
// Duplicator
bitsectionSet::bitsectionSet(const bitsectionSet& copy)
{
	count = copy.count;
	bitCount = copy.bitCount;
	sections = new bitsection[count]();
	memcpy(copy.sections, sections, count * sizeof(bitsection));
}



/// ----------- PRIVATE FUNCTIONS -------------

// Add bitsection at specified index and increment other sections if required.
bitsection bitsectionSet::AddAtSection(bitsection addition, uint32_t index)
{
	bitsection presum = sections[index];
	bitsection sum = presum + addition;
	bitsection retSum = sum; // the value we return.
	sections[index] = sum;
	// if our least significant section overflows, increment next significant section and see if that overflows
	while (sum < presum && index != 0)
	{
		index--;
		presum = sections[index];
		sum = presum + 1; // if this overflows again, the while loop will continue
		sections[index] = sum;
	}
	return retSum;

}

/// ----------- PUBLIC FUNCTIONS -------------

inline bitsection& bitsectionSet::operator[](uint32_t index) const
{
	return sections[index];
}
inline bitsection& bitsectionSet::operator[](int index) const
{
	return sections[index];
}


inline bitsectionSet& bitsectionSet::operator+=(bitsection obs)
{
	AddAtSection(obs, 0);
	return *this;
}

bitsectionSet& bitsectionSet::operator+=(const bitsectionSet& obset)
{
	uint32_t index = 0;
	bitsection presum;
	bitsection sum;
	bitsection carry = 0; // this is either a zero or a one. 
	while (index < count)
	{
		index++;
		presum = sections[index];
		sum = presum + obset.sections[index];
		sections[index] = sum + carry;
		carry = presum > sum ? 1 : 0; // set carry to 1 if we overflow into the next bitsection.
	}

	return *this;
}

bitsectionSet& bitsectionSet::operator-=(bitsection obs)
{
	uint32_t index = 0;
	bitsection presub = sections[index];
	bitsection subbedValue = presub - obs;
	sections[index] = subbedValue;
	// if our least significant section underflows, decrement next significant section and so on.
	while (subbedValue > presub && index < count)
	{
		index++;
		presub = sections[index];
		subbedValue = presub - 1;
		sections[index] = subbedValue;
	}

	return *this;
}
// Not tested.
bitsectionSet& bitsectionSet::operator*=(bitsection obs)
{
	for (uint32_t i = 0; i < count; i++)
	{
		bitsection sectionCache = sections[i];
		if (sectionCache == 0)
			continue;
		bitsection res = 0;
		bitsection multiplierMut = obs;
		while (multiplierMut != 0)
		{
			if (multiplierMut & 1)
			{
				res += sectionCache;
			}
			sectionCache <<= 1;
			multiplierMut >>= 1;
			AddAtSection(res, i);

		}
	}
	return *this;
}
// VERY slow. optimize later
bitsectionSet& bitsectionSet::operator*=(const bitsectionSet& obs)
{
	bitsectionSet mutableObs = bitsectionSet(obs);
	bitsectionSet selfCopy = bitsectionSet(*this);
	bitsection *LSptr = &mutableObs.sections[0]; // pointer to least significant section
	bitsection res = 0;
	while (!mutableObs.isZero())
	{
		if (*LSptr & 1)
		{
			*this += selfCopy;
		}
		selfCopy <<= 1;
		mutableObs >>= 1;

	}
	return *this;
}

bitsectionSet& bitsectionSet::operator<<=(uint32_t shift)
{
	bitsection carryStartBit = BITSECTION_LENGTH - shift;
	// isolate length of "shift" bits starting at "carryStartBit"
	bitsection carryMask = ((1u << shift) - 1u) << carryStartBit;
	bitsection carry = 0;
	bitsection sectCache;
	for (uint32_t i = 0; i < count; i++)
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
	for (int i = count - 1; i >= 0; i--)
	{
		sectCache = sections[i];
		sections[i] = (sectCache >> shift) | carry; // shift this bitsection and then apply the carry from the last section
		// grab the lower x bits from our section cache (pre-shift)
		//and move them so they'll be the highest bits in the next section
		carry = (sectCache & carryMask);
	}
	return *this;
}

bool bitsectionSet::isZero()
{
	for (uint32_t i = 0; count > i; i++)
	{
		if (sections[i] != 0)
			return false;
	}
	return true;
}