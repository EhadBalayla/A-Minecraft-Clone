#include "TerrainGen_3.h"
#include "WorldManager.h"

TerrainGen_3::TerrainGen_3(Random& random, WorldManager* world) : Rand(random), owningWorld(world),
	noiseGen1(random, 16), noiseGen2(random, 16), noiseGen3(random, 8), noiseGen4(random, 4), noiseGen5(random, 4), noiseGen6(random, 10), noiseGen7(random, 16), mobSpawnerNoise(random, 8) {

	sandNoise = new double[256];
	gravelNoise = new double[256];
	stoneNoise = new double[256];
}
TerrainGen_3::~TerrainGen_3() {

}

void TerrainGen_3::GenerateChunk(BlockType* voxelData, int ChunkX, int ChunkZ, uint8_t LOD) {
	Rand.setSeed((long)ChunkX * 341873128712L + (long)ChunkZ * 132897987541L);

	generateTerrain(ChunkX, ChunkZ, voxelData);
	replaceSurfaceBlocks(ChunkX, ChunkZ, voxelData);
}

void TerrainGen_3::generateTerrain(int var1, int var2, BlockType* voxelData) {
	int var4 = 4;
	int var5 = 64;
	int var6 = var4 + 1;
	int var7 = 17;
	int var8 = var4 + 1;
	noiseArray = initializeNoiseField(noiseArray, var1 * var4, 0, var2 * var4, var6, var7, var8);

	for (int var9 = 0; var9 < var4; ++var9) {
		for (int var10 = 0; var10 < var4; ++var10) {
			for (int var11 = 0; var11 < 16; ++var11) {
				double var12 = 0.125;
				double var14 = noiseArray[((var9 + 0) * var8 + var10 + 0) * var7 + var11 + 0];
				double var16 = noiseArray[((var9 + 0) * var8 + var10 + 1) * var7 + var11 + 0];
				double var18 = noiseArray[((var9 + 1) * var8 + var10 + 0) * var7 + var11 + 0];
				double var20 = noiseArray[((var9 + 1) * var8 + var10 + 1) * var7 + var11 + 0];
				double var22 = (noiseArray[((var9 + 0) * var8 + var10 + 0) * var7 + var11 + 1] - var14) * var12;
				double var24 = (noiseArray[((var9 + 0) * var8 + var10 + 1) * var7 + var11 + 1] - var16) * var12;
				double var26 = (noiseArray[((var9 + 1) * var8 + var10 + 0) * var7 + var11 + 1] - var18) * var12;
				double var28 = (noiseArray[((var9 + 1) * var8 + var10 + 1) * var7 + var11 + 1] - var20) * var12;

				for (int var30 = 0; var30 < 8; ++var30) {
					double var31 = 0.25;
					double var33 = var14;
					double var35 = var16;
					double var37 = (var18 - var14) * var31;
					double var39 = (var20 - var16) * var31;

					for (int var41 = 0; var41 < 4; ++var41) {
						double var44 = 0.25;
						double var46 = var33;
						double var48 = (var35 - var33) * var44;

						for (int var50 = 0; var50 < 4; ++var50) {
							BlockType var51 = BlockType::Air;
							if (var11 * 8 + var30 < var5) {
								if (owningWorld->IsSnowWorld && var11 * 8 + var30 >= var5 - 1) {
									var51 = BlockType::Obsidian;
								}
								else {
									var51 = BlockType::WaterStill;
								}
							}

							if (var46 > 0.0) {
								var51 = BlockType::Stone;
							}

							voxelData[IndexAt(var41 + var9 * 4, var11 * 8 + var30, var50 + var10 * 4)] = var51;
							var46 += var48;
						}

						var33 += var37;
						var35 += var39;
					}

					var14 += var22;
					var16 += var24;
					var18 += var26;
					var20 += var28;
				}
			}
		}
	}
}
void TerrainGen_3::replaceSurfaceBlocks(int var1, int var2, BlockType* voxelData) {
	int var4 = 64;
	double var5 = 1.0 / 32.0;
	sandNoise = noiseGen4.generateNoiseOctaves(sandNoise, (double)(var1 * 16), (double)(var2 * 16), 0.0, 16, 16, 1, var5, var5, 1.0);
	gravelNoise = noiseGen4.generateNoiseOctaves(gravelNoise, (double)(var2 * 16), 109.0134, (double)(var1 * 16), 16, 1, 16, var5, 1.0, var5);
	stoneNoise = noiseGen5.generateNoiseOctaves(stoneNoise, (double)(var1 * 16), (double)(var2 * 16), 0.0, 16, 16, 1, var5 * 2.0, var5 * 2.0, var5 * 2.0);

	for (int var7 = 0; var7 < 16; ++var7) {
		for (int var8 = 0; var8 < 16; ++var8) {
			bool var9 = sandNoise[var7 + var8 * 16] + Rand.nextDouble() * 0.2 > 0.0;
			bool var10 = gravelNoise[var7 + var8 * 16] + Rand.nextDouble() * 0.2 > 3.0;
			int var11 = (int)(stoneNoise[var7 + var8 * 16] / 3.0 + 3.0 + Rand.nextDouble() * 0.25);
			int var12 = -1;
			BlockType var13 = BlockType::Grass;
			BlockType var14 = BlockType::Dirt;

			for (int var15 = 127; var15 >= 0; --var15) {
				int var16 = IndexAt(var7, var15, var8);

				if (var15 <= 0 + Rand.nextInt(6) - 1) {
					voxelData[var16] = BlockType::Bedrock;
				}
				else {
					BlockType var17 = voxelData[var16];
					if (var17 == 0) {
						var12 = -1;
					}
					else if (var17 == BlockType::Stone) {
						if (var12 == -1) {
							if (var11 <= 0) {
								var13 = BlockType::Air;
								var14 = BlockType::Stone;
							}
							else if (var15 >= var4 - 4 && var15 <= var4 + 1) {
								var13 = BlockType::Grass;
								var14 = BlockType::Dirt;
								if (var10) {
									var13 = BlockType::Air;
								}

								if (var10) {
									var14 = BlockType::Gravel;
								}

								if (var9) {
									var13 = BlockType::Sand;
								}

								if (var9) {
									var14 = BlockType::Sand;
								}
							}

							if (var15 < var4 && var13 == 0) {
								var13 = BlockType::WaterStill;
							}

							var12 = var11;
							if (var15 >= var4 - 1) {
								voxelData[var16] = var13;
							}
							else {
								voxelData[var16] = var14;
							}
						}
						else if (var12 > 0) {
							--var12;
							voxelData[var16] = var14;
						}
					}
				}
			}
		}
	}
}




double* TerrainGen_3::initializeNoiseField(double* var1, int var2, int var3, int var4, int var5, int var6, int var7) {
	if (var1 == nullptr) {
		var1 = new double[var5 * var6 * var7];
	}

	double var8 = 684.412;
	double var10 = 684.412;
	noise6 = noiseGen6.generateNoiseOctaves(noise6, (double)var2, (double)var3, (double)var4, var5, 1, var7, 1.0, 0.0, 1.0);
	noise7 = noiseGen7.generateNoiseOctaves(noise7, (double)var2, (double)var3, (double)var4, var5, 1, var7, 100.0, 0.0, 100.0);
	noise3 = noiseGen3.generateNoiseOctaves(noise3, (double)var2, (double)var3, (double)var4, var5, var6, var7, var8 / 80.0, var10 / 160.0, var8 / 80.0);
	noise1 = noiseGen1.generateNoiseOctaves(noise1, (double)var2, (double)var3, (double)var4, var5, var6, var7, var8, var10, var8);
	noise2 = noiseGen2.generateNoiseOctaves(noise2, (double)var2, (double)var3, (double)var4, var5, var6, var7, var8, var10, var8);
	int var12 = 0;
	int var13 = 0;

	for (int var14 = 0; var14 < var5; ++var14) {
		for (int var15 = 0; var15 < var7; ++var15) {
			double var16 = (noise6[var13] + 256.0) / 512.0;
			if (var16 > 1.0) {
				var16 = 1.0;
			}

			double var18 = 0.0;
			double var20 = noise7[var13] / 8000.0;
			if (var20 < 0.0) {
				var20 = -var20;
			}

			var20 = var20 * 3.0 - 3.0;
			if (var20 < 0.0) {
				var20 /= 2.0;
				if (var20 < -1.0) {
					var20 = -1.0;
				}

				var20 /= 1.4;
				var20 /= 2.0;
				var16 = 0.0;
			}
			else {
				if (var20 > 1.0) {
					var20 = 1.0;
				}

				var20 /= 6.0;
			}

			var16 += 0.5;
			var20 = var20 * (double)var6 / 16.0;
			double var22 = (double)var6 / 2.0 + var20 * 4.0;
			++var13;

			for (int var24 = 0; var24 < var6; ++var24) {
				double var25 = 0.0;
				double var27 = ((double)var24 - var22) * 12.0 / var16;
				if (var27 < 0.0) {
					var27 *= 4.0;
				}

				double var29 = noise1[var12] / 512.0;
				double var31 = noise2[var12] / 512.0;
				double var33 = (noise3[var12] / 10.0 + 1.0) / 2.0;
				if (var33 < 0.0) {
					var25 = var29;
				}
				else if (var33 > 1.0) {
					var25 = var31;
				}
				else {
					var25 = var29 + (var31 - var29) * var33;
				}

				var25 -= var27;
				double var35;
				if (var24 > var6 - 4) {
					var35 = (double)((float)(var24 - (var6 - 4)) / 3.0F);
					var25 = var25 * (1.0 - var35) + -10.0 * var35;
				}

				if ((double)var24 < var18) {
					var35 = (var18 - (double)var24) / 4.0;
					if (var35 < 0.0) {
						var35 = 0.0;
					}

					if (var35 > 1.0) {
						var35 = 1.0;
					}

					var25 = var25 * (1.0 - var35) + -10.0 * var35;
				}

				var1[var12] = var25;
				++var12;
			}
		}
	}

	return var1;
}