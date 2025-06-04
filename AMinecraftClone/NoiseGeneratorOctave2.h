#pragma once
#include "NoiseGeneratorPerlin.h"
class NoiseGeneratorOctave2 : public NoiseGeneratorOne
{
public:
	NoiseGeneratorOctave2(Random& var1, int var2);
	~NoiseGeneratorOctave2();

	double noiseGenerator(double var1, double var3);
	double generateNoiseOctaves(double var1, double var3, double var5);
private:
	NoiseGeneratorPerlin* generatorCollection;
	int octaves;
};

