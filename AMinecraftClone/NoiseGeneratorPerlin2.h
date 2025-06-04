#pragma once
#include "NoiseGeneratorPerlin.h"
#include "Random.h"
class NoiseGeneratorPerlin2 : public NoiseGeneratorOne
{
public:
	NoiseGeneratorPerlin2();
	NoiseGeneratorPerlin2(Random var1);

	double generateNoise(double var1, double var3);
	double generateNoiseD(double var1, double var3, double var5);
private:
	int permutations[512];
	double xCoord;
	double yCoord;
	double zCoord;

	double generateNoise(double var1, double var3, double var5);
	static double generateNoise(double var0);
	static double lerp(double var0, double var2, double var4);
	static double grad(int var0, double var1, double var3, double var5);
};

