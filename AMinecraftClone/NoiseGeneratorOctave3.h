#pragma once
#include "NoiseGeneratorPerlin3.h"
#include "Random.h"

class NoiseGeneratorOctave3
{
public:
	NoiseGeneratorOctave3(Random& var1, int var2);

	double generateNoiseOctaves(double var1, double var3);
	double* generateNoiseOctaves(double* var1, double var2, double var4, double var6, int var8, int var9, int var10, double var11, double var13, double var15);
private:
	NoiseGeneratorPerlin3* generatorCollection;
	int octaves;
};

