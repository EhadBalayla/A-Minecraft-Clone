#include "TerrainGen_3.h"
#include "WorldManager.h"
#include "MathHelper.h"
#include "Game.h"

#define PI 3.14159265358979323846

TerrainGen_3::TerrainGen_3(long seed, WorldManager* world) : Rand(Random(seed)), randomSeed(seed), owningWorld(world),
	noiseGen1(Rand, 16), noiseGen2(Rand, 16), noiseGen3(Rand, 8), noiseGen4(Rand, 4), noiseGen5(Rand, 4), noiseGen6(Rand, 10), noiseGen7(Rand, 16), mobSpawnerNoise(Rand, 8) {

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
	//GenerateCaves(ChunkX, ChunkZ, voxelData);
}
void TerrainGen_3::populate(int var2, int var3) {
	int var4 = var2 * 16;
	int var5 = var3 * 16;
	Rand.setSeed(randomSeed);
	long var6 = Rand.nextLong() / 2L * 2L + 1L;
	long var8 = Rand.nextLong() / 2L * 2L + 1L;
	Rand.setSeed((long)var2 * var6 + (long)var3 * var8 ^ randomSeed);
	double var10 = 0.25;

	int var12;
	int var13;
	int var14;
	int var15;
	for (var12 = 0; var12 < 8; ++var12) {
		var13 = var4 + Rand.nextInt(16) + 8;
		var14 = Rand.nextInt(128);
		var15 = var5 + Rand.nextInt(16) + 8;
		GenerateDungeon(Rand, var13, var14, var15);
	}

	for (var12 = 0; var12 < 10; ++var12) {
		var13 = var4 + Rand.nextInt(16);
		var14 = Rand.nextInt(128);
		var15 = var5 + Rand.nextInt(16);
		GenerateClay(32, Rand, var13, var14, var15);
	}

	for (var12 = 0; var12 < 20; ++var12) {
		var13 = var4 + Rand.nextInt(16);
		var14 = Rand.nextInt(128);
		var15 = var5 + Rand.nextInt(16);
		GenerateOre(32, BlockType::Dirt, Rand, var13, var14, var15);
	}

	for (var12 = 0; var12 < 10; ++var12) {
		var13 = var4 + Rand.nextInt(16);
		var14 = Rand.nextInt(128);
		var15 = var5 + Rand.nextInt(16);
		GenerateOre(32, BlockType::Gravel, Rand, var13, var14, var15);
	}

	for (var12 = 0; var12 < 20; ++var12) {
		var13 = var4 + Rand.nextInt(16);
		var14 = Rand.nextInt(128);
		var15 = var5 + Rand.nextInt(16);
		GenerateOre(16, BlockType::CoalOre, Rand, var13, var14, var15);
	}

	for (var12 = 0; var12 < 20; ++var12) {
		var13 = var4 + Rand.nextInt(16);
		var14 = Rand.nextInt(64);
		var15 = var5 + Rand.nextInt(16);
		GenerateOre(8, BlockType::IronOre, Rand, var13, var14, var15);
	}

	for (var12 = 0; var12 < 2; ++var12) {
		var13 = var4 + Rand.nextInt(16);
		var14 = Rand.nextInt(32);
		var15 = var5 + Rand.nextInt(16);
		GenerateOre(8, BlockType::GoldOre, Rand, var13, var14, var15);
	}

	for (var12 = 0; var12 < 8; ++var12) {
		var13 = var4 + Rand.nextInt(16);
		var14 = Rand.nextInt(16);
		var15 = var5 + Rand.nextInt(16);
		GenerateOre(7, BlockType::TNT, Rand, var13, var14, var15); //remember to change to redstone
	}

	for (var12 = 0; var12 < 1; ++var12) {
		var13 = var4 + Rand.nextInt(16);
		var14 = Rand.nextInt(16);
		var15 = var5 + Rand.nextInt(16);
		GenerateOre(7, BlockType::DiamondOre, Rand, var13, var14, var15);
	}

	var10 = 0.5;
	var12 = (int)((mobSpawnerNoise.generateNoiseOctaves((double)var4 * var10, (double)var5 * var10) / 8.0 + Rand.nextDouble() * 4.0 + 4.0) / 3.0);
	if (var12 < 0) {
		var12 = 0;
	}

	if (Rand.nextInt(10) == 0) {
		++var12;
	}

	bool BigTrees = false;
	if (Rand.nextInt(10) == 0) {
		BigTrees = true;
	}

	int var16;
	for (var14 = 0; var14 < var12; ++var14) {
		var15 = var4 + Rand.nextInt(16) + 8;
		var16 = var5 + Rand.nextInt(16) + 8;
		//((WorldGenerator)var18).setScale(1.0, 1.0, 1.0);
		//((WorldGenerator)var18).generate(this.worldObj, this.rand, var15, this.worldObj.getHeightValue(var15, var16), var16);
	}

	int var17;
	for (var14 = 0; var14 < 2; ++var14) {
		var15 = var4 + Rand.nextInt(16) + 8;
		var16 = Rand.nextInt(128);
		var17 = var5 + Rand.nextInt(16) + 8;
		GenerateFlowers(BlockType::YellowFlower, Rand, var15, var16, var17);
	}

	if (Rand.nextInt(2) == 0) {
		var14 = var4 + Rand.nextInt(16) + 8;
		var15 = Rand.nextInt(128);
		var16 = var5 + Rand.nextInt(16) + 8;
		GenerateFlowers(BlockType::RedFlower, Rand, var14, var15, var16);
	}

	if (Rand.nextInt(4) == 0) {
		var14 = var4 + Rand.nextInt(16) + 8;
		var15 = Rand.nextInt(128);
		var16 = var5 + Rand.nextInt(16) + 8;
		GenerateFlowers(BlockType::BrownMushroom, Rand, var14, var15, var16);
	}

	if (Rand.nextInt(8) == 0) {
		var14 = var4 + Rand.nextInt(16) + 8;
		var15 = Rand.nextInt(128);
		var16 = var5 + Rand.nextInt(16) + 8;
		GenerateFlowers(BlockType::RedMushroom, Rand, var14, var15, var16);
	}

	for (var14 = 0; var14 < 10; ++var14) {
		var15 = var4 + Rand.nextInt(16) + 8;
		var16 = Rand.nextInt(128);
		var17 = var5 + Rand.nextInt(16) + 8;
		GenerateReed(Rand, var15, var16, var17);
	}

	for (var14 = 0; var14 < 1; ++var14) {
		var15 = var4 + Rand.nextInt(16) + 8;
		var16 = Rand.nextInt(128);
		var17 = var5 + Rand.nextInt(16) + 8;
		GenerateCactus(Rand, var15, var16, var17);
	}

	for (var14 = 0; var14 < 50; ++var14) {
		var15 = var4 + Rand.nextInt(16) + 8;
		var16 = Rand.nextInt(Rand.nextInt(120) + 8);
		var17 = var5 + Rand.nextInt(16) + 8;
		GenerateLiquids(BlockType::WaterStill, Rand, var15, var16, var17);
	}

	for (var14 = 0; var14 < 20; ++var14) {
		var15 = var4 + Rand.nextInt(16) + 8;
		var16 = Rand.nextInt(Rand.nextInt(Rand.nextInt(112) + 8) + 8);
		var17 = var5 + Rand.nextInt(16) + 8;
		GenerateLiquids(BlockType::Obsidian, Rand, var15, var16, var17);
	}

	/*for (var14 = var4 + 8 + 0; var14 < var4 + 8 + 16; ++var14) {
		for (var15 = var5 + 8 + 0; var15 < var5 + 8 + 16; ++var15) {
			var16 = this.worldObj.getTopSolidOrLiquidBlock(var14, var15);
			if (this.worldObj.snowCovered && var16 > 0 && var16 < 128 && this.worldObj.getBlockId(var14, var16, var15) == 0 && this.worldObj.getBlockMaterial(var14, var16 - 1, var15).getIsSolid() && this.worldObj.getBlockMaterial(var14, var16 - 1, var15) != Material.ice) {
				this.worldObj.setBlockWithNotify(var14, var16, var15, Block.snow.blockID);
			}
		}
	}*/

	//BlockSand.fallInstantly = false;
}

void TerrainGen_3::generateTerrain(int var1, int var2, BlockType* voxelData) {
	char var4 = 4;
	char var5 = 64;
	int var6 = var4 + 1;
	char var7 = 17;
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
void TerrainGen_3::GenerateCaves(int var3, int var4, BlockType* var5) {
	int var6 = cavesRange;
	CavesRand.setSeed(randomSeed);
	long var7 = CavesRand.nextLong() / 2L * 2L + 1L;
	long var9 = CavesRand.nextLong() / 2L * 2L + 1L;

	for (int var11 = var3 - var6; var11 <= var3 + var6; ++var11) {
		for (int var12 = var4 - var6; var12 <= var4 + var6; ++var12) {
			CavesRand.setSeed((long)var11 * var7 + (long)var12 * var9 ^ randomSeed);
			recursiveGenerate(var11, var12, var3, var4, var5);
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

void TerrainGen_3::GenerateOre(int numberOfBlocks, BlockType type, Random& var2, int var3, int var4, int var5) {
	float var6 = var2.nextFloat() * (float)PI;
	double var7 = (double)((float)(var3 + 8) + MathHelper::sin(var6) * (float)numberOfBlocks / 8.0F);
	double var9 = (double)((float)(var3 + 8) - MathHelper::sin(var6) * (float)numberOfBlocks / 8.0F);
	double var11 = (double)((float)(var5 + 8) + MathHelper::cos(var6) * (float)numberOfBlocks / 8.0F);
	double var13 = (double)((float)(var5 + 8) - MathHelper::cos(var6) * (float)numberOfBlocks / 8.0F);
	double var15 = (double)(var4 + var2.nextInt(3) + 2);
	double var17 = (double)(var4 + var2.nextInt(3) + 2);

	for (int var19 = 0; var19 <= numberOfBlocks; ++var19) {
		double var20 = var7 + (var9 - var7) * (double)var19 / (double)numberOfBlocks;
		double var22 = var15 + (var17 - var15) * (double)var19 / (double)numberOfBlocks;
		double var24 = var11 + (var13 - var11) * (double)var19 / (double)numberOfBlocks;
		double var26 = var2.nextDouble() * (double)numberOfBlocks / 16.0;
		double var28 = (double)(MathHelper::sin((float)var19 * (float)PI / (float)numberOfBlocks) + 1.0F) * var26 + 1.0;
		double var30 = (double)(MathHelper::sin((float)var19 * (float)PI / (float)numberOfBlocks) + 1.0F) * var26 + 1.0;

		for (int var32 = (int)(var20 - var28 / 2.0); var32 <= (int)(var20 + var28 / 2.0); ++var32) {
			for (int var33 = (int)(var22 - var30 / 2.0); var33 <= (int)(var22 + var30 / 2.0); ++var33) {
				for (int var34 = (int)(var24 - var28 / 2.0); var34 <= (int)(var24 + var28 / 2.0); ++var34) {
					double var35 = ((double)var32 + 0.5 - var20) / (var28 / 2.0);
					double var37 = ((double)var33 + 0.5 - var22) / (var30 / 2.0);
					double var39 = ((double)var34 + 0.5 - var24) / (var28 / 2.0);
					if (var35 * var35 + var37 * var37 + var39 * var39 < 1.0 && owningWorld->QueriedGetBlockAt(var32, var33, var34) == BlockType::Stone) {
						owningWorld->QueriedSetBlockAt(var32, var33, var34, type);
					}
				}
			}
		}
	}
}
void TerrainGen_3::GenerateClay(int numberOfBlocks, Random& var2, int var3, int var4, int var5) {
	if (Game::e_BlockRegistery[owningWorld->QueriedGetBlockAt(var3, var4, var5)].visibility != BlockVisiblity::Liquid) {
		return;
	}
	else {
		float var6 = var2.nextFloat() * (float)PI;
		double var7 = (double)((float)(var3 + 8) + MathHelper::sin(var6) * (float)numberOfBlocks / 8.0F);
		double var9 = (double)((float)(var3 + 8) - MathHelper::sin(var6) * (float)numberOfBlocks / 8.0F);
		double var11 = (double)((float)(var5 + 8) + MathHelper::cos(var6) * (float)numberOfBlocks / 8.0F);
		double var13 = (double)((float)(var5 + 8) - MathHelper::cos(var6) * (float)numberOfBlocks / 8.0F);
		double var15 = (double)(var4 + var2.nextInt(3) + 2);
		double var17 = (double)(var4 + var2.nextInt(3) + 2);

		for (int var19 = 0; var19 <= numberOfBlocks; ++var19) {
			double var20 = var7 + (var9 - var7) * (double)var19 / (double)numberOfBlocks;
			double var22 = var15 + (var17 - var15) * (double)var19 / (double)numberOfBlocks;
			double var24 = var11 + (var13 - var11) * (double)var19 / (double)numberOfBlocks;
			double var26 = var2.nextDouble() * (double)numberOfBlocks / 16.0;
			double var28 = (double)(MathHelper::sin((float)var19 * (float)PI / (float)numberOfBlocks) + 1.0F) * var26 + 1.0;
			double var30 = (double)(MathHelper::sin((float)var19 * (float)PI / (float)numberOfBlocks) + 1.0F) * var26 + 1.0;

			for (int var32 = (int)(var20 - var28 / 2.0); var32 <= (int)(var20 + var28 / 2.0); ++var32) {
				for (int var33 = (int)(var22 - var30 / 2.0); var33 <= (int)(var22 + var30 / 2.0); ++var33) {
					for (int var34 = (int)(var24 - var28 / 2.0); var34 <= (int)(var24 + var28 / 2.0); ++var34) {
						double var35 = ((double)var32 + 0.5 - var20) / (var28 / 2.0);
						double var37 = ((double)var33 + 0.5 - var22) / (var30 / 2.0);
						double var39 = ((double)var34 + 0.5 - var24) / (var28 / 2.0);
						if (var35 * var35 + var37 * var37 + var39 * var39 < 1.0) {
							BlockType var41 = owningWorld->QueriedGetBlockAt(var32, var33, var34);
							if (var41 == BlockType::Sand) {
								owningWorld->QueriedSetBlockAt(var32, var33, var34, BlockType::Obsidian); //remember to switch to Clay later
							}
						}
					}
				}
			}
		}

		return;
	}
}
void TerrainGen_3::GenerateDungeon(Random& var2, int var3, int var4, int var5) {
	char var6 = 3;
	int var7 = var2.nextInt(2) + 2;
	int var8 = var2.nextInt(2) + 2;
	int var9 = 0;

	int var10;
	int var11;
	int var12;
	for (var10 = var3 - var7 - 1; var10 <= var3 + var7 + 1; ++var10) {
		for (var11 = var4 - 1; var11 <= var4 + var6 + 1; ++var11) {
			for (var12 = var5 - var8 - 1; var12 <= var5 + var8 + 1; ++var12) {
				BlockVisiblity var13 = Game::e_BlockRegistery[owningWorld->QueriedGetBlockAt(var10, var11, var12)].visibility;
				if (var11 == var4 - 1 && var13 != BlockVisiblity::Opaque) {
					return;
				}

				if (var11 == var4 + var6 + 1 && var13 != BlockVisiblity::Opaque) {
					return;
				}

				if ((var10 == var3 - var7 - 1 || var10 == var3 + var7 + 1 || var12 == var5 - var8 - 1 || var12 == var5 + var8 + 1) && var11 == var4 && owningWorld->QueriedGetBlockAt(var10, var11, var12) == BlockType::Air && owningWorld->QueriedGetBlockAt(var10, var11 + 1, var12) == BlockType::Air) {
					++var9;
				}
			}
		}
	}

	if (var9 >= 1 && var9 <= 5) {
		for (var10 = var3 - var7 - 1; var10 <= var3 + var7 + 1; ++var10) {
			for (var11 = var4 + var6; var11 >= var4 - 1; --var11) {
				for (var12 = var5 - var8 - 1; var12 <= var5 + var8 + 1; ++var12) {
					if (var10 != var3 - var7 - 1 && var11 != var4 - 1 && var12 != var5 - var8 - 1 && var10 != var3 + var7 + 1 && var11 != var4 + var6 + 1 && var12 != var5 + var8 + 1) {
						owningWorld->QueriedSetBlockAt(var10, var11, var12, BlockType::Air);
					}
					else if (var11 >= 0 && Game::e_BlockRegistery[owningWorld->QueriedGetBlockAt(var10, var11 - 1, var12)].visibility != BlockVisiblity::Opaque) {
						owningWorld->QueriedSetBlockAt(var10, var11, var12, BlockType::Air);
					}
					else if (Game::e_BlockRegistery[owningWorld->QueriedGetBlockAt(var10, var11, var12)].visibility == BlockVisiblity::Opaque) {
						if (var11 == var4 - 1 && var2.nextInt(4) != 0) {
							owningWorld->QueriedSetBlockAt(var10, var11, var12, BlockType::MossyCobblestone);
						}
						else {
							owningWorld->QueriedSetBlockAt(var10, var11, var12, BlockType::Cobblestone);
						}
					}
				}
			}
		}

	/*label110:
		for (var10 = 0; var10 < 2; ++var10) {
			for (var11 = 0; var11 < 3; ++var11) {
				var12 = var3 + var2.nextInt(var7 * 2 + 1) - var7;
				int var14 = var5 + var2.nextInt(var8 * 2 + 1) - var8;
				if (var1.getBlockId(var12, var4, var14) == 0) {
					int var15 = 0;
					if (var1.getBlockMaterial(var12 - 1, var4, var14).isSolid()) {
						++var15;
					}

					if (var1.getBlockMaterial(var12 + 1, var4, var14).isSolid()) {
						++var15;
					}

					if (var1.getBlockMaterial(var12, var4, var14 - 1).isSolid()) {
						++var15;
					}

					if (var1.getBlockMaterial(var12, var4, var14 + 1).isSolid()) {
						++var15;
					}

					if (var15 == 1) {
						var1.setBlockWithNotify(var12, var4, var14, Block.chest.blockID);
						TileEntityChest var16 = (TileEntityChest)var1.getBlockTileEntity(var12, var4, var14);
						int var17 = 0;

						while (true) {
							if (var17 >= 8) {
								continue label110;
							}

							ItemStack var18 = this.pickCheckLootItem(var2);
							if (var18 != null) {
								var16.setInventorySlotContents(var2.nextInt(var16.getSizeInventory()), var18);
							}

							++var17;
						}
					}
				}
			}
		}*/

		owningWorld->QueriedSetBlockAt(var3, var4, var5, BlockType::MobSpawner);
		//TileEntityMobSpawner var19 = (TileEntityMobSpawner)var1.getBlockTileEntity(var3, var4, var5);
		//var19.mobID = this.pickMobSpawner(var2);
		return;
	}
	else {
		return;
	}
}
void TerrainGen_3::GenerateFlowers(BlockType flowerType, Random& var2, int var3, int var4, int var5) {

}
void TerrainGen_3::GenerateReed(Random& var2, int var3, int var4, int var5) {
	
}
void TerrainGen_3::GenerateCactus(Random& var2, int var3, int var4, int var5) {

}
void TerrainGen_3::GenerateLiquids(BlockType type, Random& var2, int var3, int var4, int var5) {

}


void TerrainGen_3::recursiveGenerate(int var2, int var3, int var4, int var5, BlockType* var6) {
	int var7 = CavesRand.nextInt(CavesRand.nextInt(CavesRand.nextInt(40) + 1) + 1);
	if (CavesRand.nextInt(15) != 0) {
		var7 = 0;
	}

	for (int var8 = 0; var8 < var7; ++var8) {
		double var9 = (double)(var2 * 16 + CavesRand.nextInt(16));
		double var11 = (double)CavesRand.nextInt(CavesRand.nextInt(120) + 8);
		double var13 = (double)(var3 * 16 + CavesRand.nextInt(16));
		int var15 = 1;
		if (CavesRand.nextInt(4) == 0) {
			generateLargeCaveNode(var4, var5, var6, var9, var11, var13);
			var15 += CavesRand.nextInt(4);
		}

		for (int var16 = 0; var16 < var15; ++var16) {
			float var17 = CavesRand.nextFloat() * (float)PI * 2.0F;
			float var18 = (CavesRand.nextFloat() - 0.5F) * 2.0F / 8.0F;
			float var19 = CavesRand.nextFloat() * 2.0F + CavesRand.nextFloat();
			generateCaveNode(var4, var5, var6, var9, var11, var13, var19, var17, var18, 0, 0, 1.0);
		}
	}

}
void TerrainGen_3::generateLargeCaveNode(int var1, int var2, BlockType* var3, double var4, double var6, double var8) {
	generateCaveNode(var1, var2, var3, var4, var6, var8, 1.0F + CavesRand.nextFloat() * 6.0F, 0.0F, 0.0F, -1, -1, 0.5);
}
void TerrainGen_3::generateCaveNode(int var1, int var2, BlockType* var3, double var4, double var6, double var8, float var10, float var11, float var12, int var13, int var14, double var15) {
	double var17 = (double)(var1 * 16 + 8);
	double var19 = (double)(var2 * 16 + 8);
	float var21 = 0.0F;
	float var22 = 0.0F;
	Random var23(CavesRand.nextLong());
	if (var14 <= 0) {
		int var24 = cavesRange * 16 - 16;
		var14 = var24 - var23.nextInt(var24 / 4);
	}

	bool var52 = false;
	if (var13 == -1) {
		var13 = var14 / 2;
		var52 = true;
	}

	int var25 = var23.nextInt(var14 / 2) + var14 / 4;

	for (bool var26 = var23.nextInt(6) == 0; var13 < var14; ++var13) {
		double var27 = 1.5 + (double)(MathHelper::sin((float)var13 * (float)PI / (float)var14) * var10 * 1.0F);
		double var29 = var27 * var15;
		float var31 = MathHelper::cos(var12);
		float var32 = MathHelper::sin(var12);
		var4 += (double)(MathHelper::cos(var11) * var31);
		var6 += (double)var32;
		var8 += (double)(MathHelper::sin(var11) * var31);
		if (var26) {
			var12 *= 0.92F;
		}
		else {
			var12 *= 0.7F;
		}

		var12 += var22 * 0.1F;
		var11 += var21 * 0.1F;
		var22 *= 0.9F;
		var21 *= 12.0F / 16.0F;
		var22 += (var23.nextFloat() - var23.nextFloat()) * var23.nextFloat() * 2.0F;
		var21 += (var23.nextFloat() - var23.nextFloat()) * var23.nextFloat() * 4.0F;
		if (!var52 && var13 == var25 && var10 > 1.0F) {
			generateCaveNode(var1, var2, var3, var4, var6, var8, var23.nextFloat() * 0.5F + 0.5F, var11 - (float)PI * 0.5F, var12 / 3.0F, var13, var14, 1.0);
			generateCaveNode(var1, var2, var3, var4, var6, var8, var23.nextFloat() * 0.5F + 0.5F, var11 + (float)PI * 0.5F, var12 / 3.0F, var13, var14, 1.0);
			return;
		}

		if (var52 || var23.nextInt(4) != 0) {
			double var33 = var4 - var17;
			double var35 = var8 - var19;
			double var37 = (double)(var14 - var13);
			double var39 = (double)(var10 + 2.0F + 16.0F);
			if (var33 * var33 + var35 * var35 - var37 * var37 > var39 * var39) {
				return;
			}

			if (var4 >= var17 - 16.0 - var27 * 2.0 && var8 >= var19 - 16.0 - var27 * 2.0 && var4 <= var17 + 16.0 + var27 * 2.0 && var8 <= var19 + 16.0 + var27 * 2.0) {
				int var53 = MathHelper::floor_double(var4 - var27) - var1 * 16 - 1;
				int var34 = MathHelper::floor_double(var4 + var27) - var1 * 16 + 1;
				int var54 = MathHelper::floor_double(var6 - var29) - 1;
				int var36 = MathHelper::floor_double(var6 + var29) + 1;
				int var55 = MathHelper::floor_double(var8 - var27) - var2 * 16 - 1;
				int var38 = MathHelper::floor_double(var8 + var27) - var2 * 16 + 1;
				if (var53 < 0) {
					var53 = 0;
				}

				if (var34 > 16) {
					var34 = 16;
				}

				if (var54 < 1) {
					var54 = 1;
				}

				if (var36 > 120) {
					var36 = 120;
				}

				if (var55 < 0) {
					var55 = 0;
				}

				if (var38 > 16) {
					var38 = 16;
				}

				bool var56 = false;

				int var40;
				int var43;
				for (var40 = var53; !var56 && var40 < var34; ++var40) {
					for (int var41 = var55; !var56 && var41 < var38; ++var41) {
						for (int var42 = var36 + 1; !var56 && var42 >= var54 - 1; --var42) {
							//var43 = (var40 * 16 + var41) * 128 + var42;

							var43 = IndexAt(var40, var42, var41);

							if (var42 >= 0 && var42 < 128) {
								if (var3[var43] == BlockType::Obsidian || var3[var43] == BlockType::WaterStill) {
									var56 = true;
								}

								if (var42 != var54 - 1 && var40 != var53 && var40 != var34 - 1 && var41 != var55 && var41 != var38 - 1) {
									var42 = var54;
								}
							}
						}
					}
				}

				if (!var56) {
					for (var40 = var53; var40 < var34; ++var40) {
						double var57 = ((double)(var40 + var1 * 16) + 0.5 - var4) / var27;

						for (var43 = var55; var43 < var38; ++var43) {
							double var44 = ((double)(var43 + var2 * 16) + 0.5 - var8) / var27;
							//int var46 = (var40 * 16 + var43) * 128 + var36;
							int var46 = IndexAt(var40, var36, var43);
							bool var47 = false;

							for (int var48 = var36 - 1; var48 >= var54; --var48) {
								double var49 = ((double)var48 + 0.5 - var6) / var29;
								if (var49 > -0.7 && var57 * var57 + var49 * var49 + var44 * var44 < 1.0) {
									BlockType var51 = var3[var46];
									if (var51 == BlockType::Grass) {
										var47 = true;
									}

									if (var51 == BlockType::Stone || var51 == BlockType::Dirt || var51 == BlockType::Grass) {
										if (var48 < 10) {
											var3[var46] = BlockType::Obsidian; //remember to switch to Lava
										}
										else {
											var3[var46] = BlockType::Air;
											if (var47 && var3[var46 - 1] == BlockType::Dirt) {
												var3[var46 - 1] = BlockType::Grass;
											}
										}
									}
								}

								--var46;
							}
						}
					}

					if (var52) {
						break;
					}
				}
			}
		}
	}

}