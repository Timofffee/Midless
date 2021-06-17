#ifndef G_WORLD_H
#define G_WORLD_H

#define WORLD_SIZE_X 16
#define WORLD_SIZE_Y 8
#define WORLD_SIZE_Z 16
#define WORLD_SIZE (WORLD_SIZE_X * WORLD_SIZE_Y * WORLD_SIZE_Z)

#define WORLD_BLOCK_SIZE_X (WORLD_SIZE_X * CHUNK_SIZE_X)
#define WORLD_BLOCK_SIZE_Y (WORLD_SIZE_Y * CHUNK_SIZE_Y)
#define WORLD_BLOCK_SIZE_Z (WORLD_SIZE_Z * CHUNK_SIZE_Z)

#include "chunk.h"
#include "raylib.h"

typedef struct World{
    Chunk chunks[WORLD_SIZE];
    Material mat;
} World;

//Initialize the world.
void World_Init(void);
//Unload the world.
void World_Unload(void);
//Draw the world.
void World_Draw(Vector3 camPosition);
//Apply terrain texture to the world.
void World_ApplyTexture(Texture2D texture);
void World_ApplyShader(Shader shader);
//Set block at a given position and reload affected meshes.
void World_SetBlock(Vector3 blockPos, int blockID);

//Get block ID at a given position.
int World_GetBlock(Vector3 blockPos);

int World_IsValidChunkPos(Vector3 chunkPos);
int World_IsValidBlockPos(Vector3 blockPos);
int World_ChunkPosToIndex(Vector3 chunkPos);
int World_BlockPosToIndex(Vector3 blockPos);

Vector3 World_ChunkIndexToPos(int chunkIndex);
Vector3 World_BlockIndexToPos(int blockIndex);

Chunk* World_GetChunkAt(Vector3 pos);

#endif