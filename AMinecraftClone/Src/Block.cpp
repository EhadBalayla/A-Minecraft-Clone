#include "Block.h"
#include "Game.h"

AABB Block::blockHitbox = { glm::vec3(0.0), glm::vec3(1.0) };

Block::Block() {

}

Block::Block(BlockType newType) {
	setType(newType);
}

void Block::setType(BlockType newType) {
	m_Type = newType;
	data = Game::e_BlockRegistery[m_Type];
}

void Block::BreakBlock() {
	setType(BlockType::Air);
	owningChunk->IsModified = true;
	if (BlockX == 0) //if at the X edge of the chunk
	{
		int ChunksX = owningChunk->ChunkX;
		int ChunksZ = owningChunk->ChunkZ;

		//owningChunk->owningWorld->getChunkAt(ChunksX - 1, ChunksZ)->GenerateMesh();
	}
	else if (BlockX == 15) { //if at the other X edge of the chunk
		int ChunksX = owningChunk->ChunkX;
		int ChunksZ = owningChunk->ChunkZ;

		//owningChunk->owningWorld->getChunkAt(ChunksX + 1, ChunksZ)->GenerateMesh();
	}
	if (BlockZ == 0) //if at the Z edge of the chunk
	{
		int ChunksX = owningChunk->ChunkX;
		int ChunksZ = owningChunk->ChunkZ;

		//owningChunk->owningWorld->getChunkAt(ChunksX, ChunksZ - 1)->GenerateMesh();
	}
	else if (BlockZ == 15) { //if at the other Z edge of the chunk
		int ChunksX = owningChunk->ChunkX;
		int ChunksZ = owningChunk->ChunkZ;

		//owningChunk->owningWorld->getChunkAt(ChunksX, ChunksZ + 1)->GenerateMesh();
	}
}

glm::ivec3 Block::getWorldPosition() {
	return glm::ivec3(BlockX + owningChunk->ChunkX * 16, BlockY, BlockZ + owningChunk->ChunkZ * 16);
}