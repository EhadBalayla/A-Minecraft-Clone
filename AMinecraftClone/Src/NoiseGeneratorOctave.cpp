#include "NoiseGeneratorOctave.h"

NoiseGeneratorOctave::NoiseGeneratorOctave(int var1) : NoiseGeneratorOctave(Random(), var1) {}
NoiseGeneratorOctave::NoiseGeneratorOctave(Random var1, int var2) {
	octaves = var2;
	generatorCollection = new NoiseGeneratorPerlin[var2];

	for (int var3 = 0; var3 < var2; ++var3) {
		generatorCollection[var3] = NoiseGeneratorPerlin(var1);
	}

}
NoiseGeneratorOctave::~NoiseGeneratorOctave() {
	delete[] generatorCollection;
	generatorCollection = nullptr;
}


double NoiseGeneratorOctave::generateNoise(double var1, double var3) {
	double var5 = 0.0;
	double var7 = 1.0;

	for (int var9 = 0; var9 < octaves; ++var9) {
		var5 += generatorCollection[var9].generateNoise(var1 / var7, var3 / var7) * var7;
		var7 *= 2.0;
	}

	return var5;
}

double NoiseGeneratorOctave::generateNoise(double var1, double var3, double var5) {
	double var7 = 0.0;
	double var9 = 1.0;

	for (int var11 = 0; var11 < octaves; ++var11) {
		var7 += generatorCollection[var11].generateNoiseD(var1 / var9, 0.0 / var9, var5 / var9) * var9;
		var9 *= 2.0;
	}

	return var7;
}