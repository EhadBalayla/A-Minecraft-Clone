#pragma once
#include "NoiseGeneratorPerlin3.h"
#include "Random.h"

class NoiseGeneratorOctave3
{
public:
	NoiseGeneratorOctave3(Random& var1, int var2);
	~NoiseGeneratorOctave3();

	double noiseGenerator(double var1, double var3);
	double generateNoiseOctaves(double var1, double var3, double var5);
	double* generateNoiseOctaves(double** var1, int* var1_Len, int var2, int var3, int var4, int var5, int var6, int var7, double var8, double var10, double var12);
private:
	NoiseGeneratorPerlin3* generatorCollection;
	int octaves;
};

