#pragma once
#include "Noise.h"
#include "PerlinNoise.h"
#include <vector>
#include <array>
class OctaveNoise : public Noise
{
public:
	OctaveNoise();
	OctaveNoise(unsigned int seed, int var2);
	~OctaveNoise();
	double compute(double var1, double var3) override;
private:
	std::vector<PerlinNoise*> perlin;
	int octaves;
};

