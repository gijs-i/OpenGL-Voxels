//(c) gijs.i voxel engine

#ifndef TEXTURE_H
#define TEXTURE_H

#define ATLAS_SIZE 128.0f

class TextureUV {
public:
	float uvX1, uvY1, uvX2, uvY2;
	TextureUV(float x, float y, float width, float height) {
		uvX1 = x / ATLAS_SIZE;
		uvY1 = y / ATLAS_SIZE;
		uvX2 = (x + width) / ATLAS_SIZE;
		uvY2 = (y + height) / ATLAS_SIZE;
	}
};

#endif