#include "NoiseGeneratorPerlin3.h"

NoiseGeneratorPerlin3::NoiseGeneratorPerlin3() : NoiseGeneratorPerlin3(Random()) {

}
NoiseGeneratorPerlin3::NoiseGeneratorPerlin3(Random var1) {
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

double NoiseGeneratorPerlin3::generateNoise(double var1, double var3, double var5) {
	double var7 = var1 + xCoord;
	double var9 = var3 + yCoord;
	double var11 = var5 + zCoord;
	int var25 = (int)var7;
	int var2 = (int)var9;
	int var26 = (int)var11;
	if (var7 < (double)var25) {
		--var25;
	}

	if (var9 < (double)var2) {
		--var2;
	}

	if (var11 < (double)var26) {
		--var26;
	}

	int var4 = var25 & 255;
	int var27 = var2 & 255;
	int var6 = var26 & 255;
	var7 -= (double)var25;
	var9 -= (double)var2;
	var11 -= (double)var26;
	double var19 = var7 * var7 * var7 * (var7 * (var7 * 6.0 - 15.0) + 10.0);
	double var21 = var9 * var9 * var9 * (var9 * (var9 * 6.0 - 15.0) + 10.0);
	double var23 = var11 * var11 * var11 * (var11 * (var11 * 6.0 - 15.0) + 10.0);
	var25 = permutations[var4] + var27;
	var2 = permutations[var25] + var6;
	var25 = permutations[var25 + 1] + var6;
	var26 = permutations[var4 + 1] + var27;
	var4 = permutations[var26] + var6;
	var26 = permutations[var26 + 1] + var6;
	return lerp(var23, lerp(var21, lerp(var19, grad(permutations[var2], var7, var9, var11), grad(permutations[var4], var7 - 1.0, var9, var11)), lerp(var19, grad(permutations[var25], var7, var9 - 1.0, var11), grad(permutations[var26], var7 - 1.0, var9 - 1.0, var11))), lerp(var21, lerp(var19, grad(permutations[var2 + 1], var7, var9, var11 - 1.0), grad(permutations[var4 + 1], var7 - 1.0, var9, var11 - 1.0)), lerp(var19, grad(permutations[var25 + 1], var7, var9 - 1.0, var11 - 1.0), grad(permutations[var26 + 1], var7 - 1.0, var9 - 1.0, var11 - 1.0))));
}
double NoiseGeneratorPerlin3::lerp(double var0, double var2, double var4) {
	return var2 + var0 * (var4 - var2);
}
double NoiseGeneratorPerlin3::grad(int var0, double var1, double var3, double var5) {
	var0 &= 15;
	double var8 = var0 < 8 ? var1 : var3;
	double var10 = var0 < 4 ? var3 : (var0 != 12 && var0 != 14 ? var5 : var1);
	return ((var0 & 1) == 0 ? var8 : -var8) + ((var0 & 2) == 0 ? var10 : -var10);
}
double NoiseGeneratorPerlin3::generateNoise(double var1, double var3) {
	return generateNoise(var1, var3, 0.0);
}
double NoiseGeneratorPerlin3::generateNoiseD(double var1, double var3, double var5) {
	return generateNoise(var1, var3, var5);
}
void NoiseGeneratorPerlin3::populateNoiseArray(double* var1, int var2, int var3, int var4, int var5, int var6, int var7, double var8, double var10, double var12, double var14) {
	int var16 = 0;
	double var17 = 1.0 / var14;
	int var61 = -1;
	double var26 = 0.0;
	double var28 = 0.0;
	double var30 = 0.0;
	double var32 = 0.0;

	for (int var22 = 0; var22 < var5; ++var22) {
		double var35 = (double)(var2 + var22) * var8 + xCoord;
		int var15 = (int)var35;
		if (var35 < (double)var15) {
			--var15;
		}

		int var23 = var15 & 255;
		var35 -= (double)var15;
		double var39 = var35 * var35 * var35 * (var35 * (var35 * 6.0 - 15.0) + 10.0);

		for (int var24 = 0; var24 < var7; ++var24) {
			double var42 = (double)(var4 + var24) * var12 + zCoord;
			var15 = (int)var42;
			if (var42 < (double)var15) {
				--var15;
			}

			int var25 = var15 & 255;
			var42 -= (double)var15;
			double var46 = var42 * var42 * var42 * (var42 * (var42 * 6.0 - 15.0) + 10.0);

			for (int var34 = 0; var34 < var6; ++var34) {
				double var49 = (double)(var3 + var34) * var10 + yCoord;
				var15 = (int)var49;
				if (var49 < (double)var15) {
					--var15;
				}

				int var20 = var15 & 255;
				var49 -= (double)var15;
				double var53 = var49 * var49 * var49 * (var49 * (var49 * 6.0 - 15.0) + 10.0);
				if (var34 == 0 || var20 != var61) {
					var61 = var20;
					var15 = permutations[var23] + var20;
					int var19 = permutations[var15] + var25;
					var15 = permutations[var15 + 1] + var25;
					var20 += permutations[var23 + 1];
					int var21 = permutations[var20] + var25;
					var20 = permutations[var20 + 1] + var25;
					var26 = lerp(var39, grad(permutations[var19], var35, var49, var42), grad(permutations[var21], var35 - 1.0, var49, var42));
					var28 = lerp(var39, grad(permutations[var15], var35, var49 - 1.0, var42), grad(permutations[var20], var35 - 1.0, var49 - 1.0, var42));
					var30 = lerp(var39, grad(permutations[var19 + 1], var35, var49, var42 - 1.0), grad(permutations[var21 + 1], var35 - 1.0, var49, var42 - 1.0));
					var32 = lerp(var39, grad(permutations[var15 + 1], var35, var49 - 1.0, var42 - 1.0), grad(permutations[var20 + 1], var35 - 1.0, var49 - 1.0, var42 - 1.0));
				}

				double var55 = lerp(var53, var26, var28);
				double var57 = lerp(var53, var30, var32);
				double var59 = lerp(var46, var55, var57);
				int var10001 = var16++;
				var1[var10001] += var59 * var17;
			}
		}
	}

}