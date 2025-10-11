#pragma once
#include "Random.h"

class NoiseGeneratorPerlin3
{
public:
	NoiseGeneratorPerlin3();
	NoiseGeneratorPerlin3(Random var1);

	void populateNoiseArray(double* var1, int var2, int var3, int var4, int var5, int var6, int var7, double var8, double var10, double var12, double var14);
	double generateNoiseD(double var1, double var3, double var5);
	double generateNoise(double var1, double var3);
private:
	double generateNoise(double var1, double var3, double var5);
	static double lerp(double var0, double var2, double var4);
	static double grad(int var0, double var1, double var3, double var5);

	int permutations[512];
	double xCoord;
	double yCoord;
	double zCoord;
};

