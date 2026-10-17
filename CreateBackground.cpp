#include <SFML/Graphics.hpp>
#include "ZombieArena.h"
#include <iostream>

int createBackground(VertexArray& rVA, IntRect arena) {
	// Anything we do to rVA we are actually doing to background (in main)

	const int TILE_SIZE = 50;
	const int TILE_TYPES = 3;
	const int VERTS_PER_TILE = 6;   // two triangles per tile (was 4 for quads)

	int worldWidth = arena.size.x / TILE_SIZE;
	int worldHeight = arena.size.y / TILE_SIZE;

	rVA.setPrimitiveType(PrimitiveType::Triangles);
	rVA.resize(worldWidth * worldHeight * VERTS_PER_TILE);

	int currentVertex = 0;

	for (int w = 0; w < worldWidth; w++)
	{
		for (int h = 0; h < worldHeight; h++)
		{
			float x = static_cast<float>(arena.position.x + w * TILE_SIZE);
			float y = static_cast<float>(arena.position.y + h * TILE_SIZE);
			float s = static_cast<float>(TILE_SIZE);

			// Corner positions
			Vector2f tl(x, y);
			Vector2f tr(x + s, y);
			Vector2f br(x + s, y + s);
			Vector2f bl(x, y + s);

			// Pick the tile type: edges use the "wall" tile (index 3),
			// the inside is a random one of the other tiles
			int verticalOffset;
			if (h == 0 || h == worldHeight - 1 || w == 0 || w == worldWidth - 1)
				verticalOffset = TILE_TYPES * TILE_SIZE;
			else
				verticalOffset = (std::rand() % TILE_TYPES) * TILE_SIZE;

			float v = static_cast<float>(verticalOffset);

			// Texture coordinates for the same four corners
			Vector2f ttl(0.f, v);
			Vector2f ttr(s, v);
			Vector2f tbr(s, v + s);
			Vector2f tbl(0.f, v + s);

			// Triangle 1: top-left, top-right, bottom-right
			rVA[currentVertex + 0].position = tl;  rVA[currentVertex + 0].texCoords = ttl;
			rVA[currentVertex + 1].position = tr;  rVA[currentVertex + 1].texCoords = ttr;
			rVA[currentVertex + 2].position = br;  rVA[currentVertex + 2].texCoords = tbr;

			// Triangle 2: top-left, bottom-right, bottom-left
			rVA[currentVertex + 3].position = tl;  rVA[currentVertex + 3].texCoords = ttl;
			rVA[currentVertex + 4].position = br;  rVA[currentVertex + 4].texCoords = tbr;
			rVA[currentVertex + 5].position = bl;  rVA[currentVertex + 5].texCoords = tbl;

			currentVertex += VERTS_PER_TILE;
		}
	}

	return TILE_SIZE;
}