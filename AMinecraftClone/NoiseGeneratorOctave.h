#pragma once
#include "NoiseGeneratorPerlin.h"

class NoiseGeneratorOctave
{
public:
	NoiseGeneratorOctave(int var1);
	~NoiseGeneratorOctave();

	double generateNoise(double var1, double var3);
	double generateNoise(double var1, double var3, double var5);
private:
	NoiseGeneratorPerlin* generatorCollection;
	int octaves;

	NoiseGeneratorOctave(Random var1, int var2);
};

