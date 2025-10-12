#pragma once
#include "Random.h"

class NoiseGeneratorPerlin3
{
public:
	NoiseGeneratorPerlin3() = default;
	NoiseGeneratorPerlin3(Random& Rand);

	double xCoord;
	double yCoord;
	double zCoord;

	double generateNoise(double var1, double var3, double var5);
	double lerp(double var1, double var3, double var5);
	double grad(int var1, double var2, double var4, double var6);
	double generateNoise(double var1, double var3);
	void populateNoiseArray(double* var1, double var2, double var4, double var6, int var8, int var9, int var10, double var11, double var13, double var15, double var17);
private:
	int permutations[512];
};

