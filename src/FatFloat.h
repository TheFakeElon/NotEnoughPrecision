#include <stdint.h>
#include <cmath>
#include <string>
#include "bitsection.h"

class FatFloat
{
public:
	FatFloat(uint32_t expoLength, uint32_t fractLength, bool _sign);
	FatFloat(std::string digitStr);
	~FatFloat()
	{
		delete &exponentSet;
		delete &mantissaSet;
	}

	const char* linkTest = "working!";

	bool sign;
	bitsection bias;

	bitsectionSet exponentSet;

	bitsectionSet mantissaSet;

	FatFloat& operator = (const FatFloat& FF);
	bool operator == (const FatFloat& FF);

	FatFloat& operator+= (const FatFloat& FF);

	FatFloat& operator*= (const FatFloat& FF);

private:
	inline void normalize();
};
