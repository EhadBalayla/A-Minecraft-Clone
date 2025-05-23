#include "NoiseGeneratorPerlin.h"
#include <cmath>


NoiseGeneratorPerlin::NoiseGeneratorPerlin(Random var1) {
	xCoord = var1.nextDouble() * 256.0;
	yCoord = var1.nextDouble() * 256.0;
	zCoord = var1.nextDouble() * 256.0;

	int var2;
	for (var2 = 0; var2 < 256; permutations[var2] = var2++) {
	}

	for (var2 = 0; var2 < 256; ++var2) {
		int var3 = var1.nextInt(256 - var2) + var2;
		int var4 = permutations[var2];
		permutations[var2] = permutations[var3];
		permutations[var3] = var4;
		permutations[var2 + 256] = permutations[var2];
	}

}

double NoiseGeneratorPerlin::generateNoise(double var1, double var3) {
	return this->generateNoise(var1, var3, 0.0);
}

double NoiseGeneratorPerlin::generateNoiseD(double var1, double var3, double var5) {
	return this->generateNoise(var1, var3, var5);
}

double NoiseGeneratorPerlin::generateNoise(double var1, double var3, double var5) {
	double var7 = var1 + this->xCoord;
	double var9 = var3 + this->yCoord;
	double var11 = var5 + this->zCoord;
	int var22 = static_cast<int>(std::floor(var7)) & 255;
	int var2 = static_cast<int>(std::floor(var9)) & 255;
	int var23 = static_cast<int>(std::floor(var11)) & 255;
	var7 -= std::floor(var7);
	var9 -= std::floor(var9);
	var11 -= std::floor(var11);
	double var16 = generateNoise(var7);
	double var18 = generateNoise(var9);
	double var20 = generateNoise(var11);
	int var4 = permutations[var22] + var2;
	int var24 = permutations[var4] + var23;
	var4 = permutations[var4 + 1] + var23;
	var22 = permutations[var22 + 1] + var2;
	var2 = permutations[var22] + var23;
	var22 = permutations[var22 + 1] + var23;
	return lerp(var20, lerp(var18, lerp(var16, grad(permutations[var24], var7, var9, var11), grad(permutations[var2], var7 - 1.0, var9, var11)), lerp(var16, grad(permutations[var4], var7, var9 - 1.0, var11), grad(permutations[var22], var7 - 1.0, var9 - 1.0, var11))), lerp(var18, lerp(var16, grad(permutations[var24 + 1], var7, var9, var11 - 1.0), grad(permutations[var2 + 1], var7 - 1.0, var9, var11 - 1.0)), lerp(var16, grad(permutations[var4 + 1], var7, var9 - 1.0, var11 - 1.0), grad(permutations[var22 + 1], var7 - 1.0, var9 - 1.0, var11 - 1.0))));
}

double NoiseGeneratorPerlin::grad(int var0, double var1, double var3, double var5) {
	var0 &= 15;
	double var8 = var0 < 8 ? var1 : var3;
	double var10 = var0 < 4 ? var3 : (var0 != 12 && var0 != 14 ? var5 : var1);
	return ((var0 & 1) == 0 ? var8 : -var8) + ((var0 & 2) == 0 ? var10 : -var10);
}

double NoiseGeneratorPerlin::lerp(double var0, double var2, double var4) {
	return var2 + var0 * (var4 - var2);
}

double NoiseGeneratorPerlin::generateNoise(double var0) {
	return var0 * var0 * var0 * (var0 * (var0 * 6.0 - 15.0) + 10.0);
}