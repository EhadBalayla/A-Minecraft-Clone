#pragma once
#include "NoiseGeneratorOne.h"
#include "Random.h"

class NoiseGeneratorPerlin : public NoiseGeneratorOne
{
public:

	NoiseGeneratorPerlin() : NoiseGeneratorPerlin(Random()) {}
	NoiseGeneratorPerlin(Random var1);

	double generateNoise(double var1, double var3);
	double generateNoiseD(double var1, double var3, double var5);

private:
	int permutations[512];
	double xCoord;
	double yCoord;
	double zCoord;

	static double generateNoise(double var0);
	static double lerp(double var0, double var2, double var4);
	static double grad(int var0, double var1, double var3, double var5);
	double generateNoise(double var1, double var3, double var5);
};

