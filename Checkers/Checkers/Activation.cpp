#include "Activation.h"
#include <cmath>

void ReLU(float& val, bool derivative) {
	if (derivative) {
		if (val < 0)
			val = 0;
		else
			val = 1;
	}
	else {
		if (val < 0)
			val = 0;
	}
}

void TanHAlt(float& val, bool derivative)
{
	float z = 1. / (1. + abs(val));

	if (derivative)
		val = z * z;
	else
		val *= z;
}
