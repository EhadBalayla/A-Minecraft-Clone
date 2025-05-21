#pragma once
#include "Noise.h"
class CombinedNoise : public Noise
{
public:
	CombinedNoise();
	CombinedNoise(Noise* var1, Noise* var2);
	~CombinedNoise();
	double compute(double var1, double var3) override;
private:
	Noise* noise1;
	Noise* noise2;
};

