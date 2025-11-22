#pragma once
#include "NoiseGeneratorPerlin2.h"

class NoiseGeneratorOctave2
{
public:
	NoiseGeneratorOctave2(Random& var1, int var2);
	~NoiseGeneratorOctave2();

	double noiseGenerator(double var1, double var3);
	double generateNoiseOctaves(double var1, double var3, double var5);
private:
	NoiseGeneratorPerlin2* generatorCollection;
	int octaves;
};

