//(c) gijs.i voxel engine

#ifndef VERTEX_H
#define VERTEX_H

struct Vertex {
	float x, y, z, uvX, uvY, normalX, normalY, normalZ, AO;
};

struct Vector3 {
	int x, y, z;
};

struct AOMap {
	float topLeft, topRight, bottomLeft, bottomRight;
};

struct BlockSpaceInfo {
	bool rightEmpty, leftEmpty, topEmpty, bottomEmpty, frontEmpty, backEmpty;
	bool topRightOccupied, bottomRightOccupied, bottomLeftOccupied, topLeftOccupied;
	bool upperTopOccupied, upperTopRightOccupied, upperRightOccupied, upperDownRightOccupied, upperDownOccupied, upperDownLeftOccupied, upperLeftOccupied, upperTopLeftOccupied;
	bool lowerTopOccupied, lowerTopRightOccupied, lowerRightOccupied, lowerDownRightOccupied, lowerDownOccupied, lowerDownLeftOccupied, lowerLeftOccupied, lowerTopLeftOccupied;
};

#endif