
//(c) gijs.i voxel engine

#ifndef BUFFER_H
#define BUFFER_H

#include <vector>

#include "vertex.hpp"
#include "texture.hpp"

class Buffer {
public:
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;
	unsigned int index = 0;

	void createBackVerticalPlane(float x, float y, float z, TextureUV* textureUV, AOMap aoMap) {
		vertices.push_back({ x + 0.0f, y + 1.0f,z + 0.0f,textureUV->uvX2,textureUV->uvY2,0.0f, 0.0f, -1.0f, aoMap.topRight });
		vertices.push_back({ x + 0.0f,y + 0.0f,z + 0.0f,textureUV->uvX2,textureUV->uvY1,0.0f, 0.0f, -1.0f, aoMap.bottomRight });
		vertices.push_back({ x + 1.0f,y + 0.0f,z + 0.0f,textureUV->uvX1,textureUV->uvY1,0.0f, 0.0f, -1.0f, aoMap.bottomLeft });
		vertices.push_back({ x + 1.0f,y + 1.0f,z + 0.0f,textureUV->uvX1,textureUV->uvY2,0.0f, 0.0f, -1.0f, aoMap.topLeft });

		indices.push_back(index * 4 + 0);
		indices.push_back(index * 4 + 1);
		indices.push_back(index * 4 + 3);
		indices.push_back(index * 4 + 1);
		indices.push_back(index * 4 + 2);
		indices.push_back(index * 4 + 3);

		index++;
	}

	void createFrontVerticalPlane(float x, float y, float z, TextureUV* textureUV, AOMap aoMap) {
		vertices.push_back({ x + 1.0f, y + 1.0f,z + 1.0f,textureUV->uvX2,textureUV->uvY2,0.0f, 0.0f, 1.0f, aoMap.topRight });
		vertices.push_back({ x + 1.0f,y + 0.0f,z + 1.0f,textureUV->uvX2,textureUV->uvY1,0.0f, 0.0f, 1.0f, aoMap.bottomRight });
		vertices.push_back({ x + 0.0f,y + 0.0f,z + 1.0f,textureUV->uvX1,textureUV->uvY1,0.0f, 0.0f, 1.0f, aoMap.bottomLeft });
		vertices.push_back({ x + 0.0f,y + 1.0f,z + 1.0f,textureUV->uvX1,textureUV->uvY2,0.0f, 0.0f, 1.0f, aoMap.topLeft });

		indices.push_back(index * 4 + 0);
		indices.push_back(index * 4 + 1);
		indices.push_back(index * 4 + 3);
		indices.push_back(index * 4 + 1);
		indices.push_back(index * 4 + 2);
		indices.push_back(index * 4 + 3);

		index++;
	}

	void createLeftVerticalPlane(float x, float y, float z, TextureUV* textureUV, AOMap aoMap) {
		vertices.push_back({ x + 0.0f, y + 1.0f,z + 1.0f,textureUV->uvX2,textureUV->uvY2,-1.0f, 0.0f, 0.0f, aoMap.topRight });
		vertices.push_back({ x + 0.0f,y + 0.0f,z + 1.0f,textureUV->uvX2,textureUV->uvY1,-1.0f, 0.0f, 0.0f, aoMap.bottomRight });
		vertices.push_back({ x + 0.0f,y + 0.0f,z + 0.0f,textureUV->uvX1,textureUV->uvY1,-1.0f, 0.0f, 0.0f, aoMap.bottomLeft });
		vertices.push_back({ x + 0.0f,y + 1.0f,z + 0.0f,textureUV->uvX1,textureUV->uvY2,-1.0f, 0.0f, 0.0f, aoMap.topLeft });

		indices.push_back(index * 4 + 0);
		indices.push_back(index * 4 + 1);
		indices.push_back(index * 4 + 3);
		indices.push_back(index * 4 + 1);
		indices.push_back(index * 4 + 2);
		indices.push_back(index * 4 + 3);

		index++;
	}

	void createRightVerticalPlane(float x, float y, float z, TextureUV* textureUV, AOMap aoMap) {
		vertices.push_back({ x + 1.0f, y + 1.0f,z + 0.0f,textureUV->uvX2,textureUV->uvY2,1.0f, 0.0f, 0.0f, aoMap.topRight });
		vertices.push_back({ x + 1.0f,y + 0.0f,z + 0.0f,textureUV->uvX2,textureUV->uvY1,1.0f, 0.0f, 0.0f, aoMap.bottomRight });
		vertices.push_back({ x + 1.0f,y + 0.0f,z + 1.0f,textureUV->uvX1,textureUV->uvY1,1.0f, 0.0f, 0.0f, aoMap.bottomLeft });
		vertices.push_back({ x + 1.0f,y + 1.0f,z + 1.0f,textureUV->uvX1,textureUV->uvY2,1.0f, 0.0f, 0.0f, aoMap.topLeft });

		indices.push_back(index * 4 + 0);
		indices.push_back(index * 4 + 1);
		indices.push_back(index * 4 + 3);
		indices.push_back(index * 4 + 1);
		indices.push_back(index * 4 + 2);
		indices.push_back(index * 4 + 3);

		index++;
	}

	void createBottomHorizontalPlane(float x, float y, float z, TextureUV* textureUV, AOMap aoMap) {
		vertices.push_back({ x + 1.0f, y + 0.0f,z + 1.0f,textureUV->uvX2,textureUV->uvY2,0.0f,-1.0f,0.0f, aoMap.topRight });
		vertices.push_back({ x + 1.0f,y + 0.0f,z + 0.0f,textureUV->uvX2,textureUV->uvY1,0.0f,-1.0f,0.0f, aoMap.bottomRight });
		vertices.push_back({ x + 0.0f,y + 0.0f,z + 0.0f,textureUV->uvX1,textureUV->uvY1,0.0f,-1.0f,0.0f, aoMap.bottomLeft });
		vertices.push_back({ x + 0.0f,y + 0.0f,z + 1.0f,textureUV->uvX1,textureUV->uvY2,0.0f,-1.0f,0.0f, aoMap.topLeft });

		indices.push_back(index * 4 + 0);
		indices.push_back(index * 4 + 1);
		indices.push_back(index * 4 + 3);
		indices.push_back(index * 4 + 1);
		indices.push_back(index * 4 + 2);
		indices.push_back(index * 4 + 3);

		index++;
	}

	void createTopHorizontalPlane(float x, float y, float z, TextureUV* textureUV, AOMap aoMap) {
		vertices.push_back({ x + 1.0f, y + 1.0f,z + 0.0f,textureUV->uvX2,textureUV->uvY2,0.0f,1.0f,0.0f, aoMap.topRight });
		vertices.push_back({ x + 1.0f,y + 1.0f,z + 1.0f,textureUV->uvX2,textureUV->uvY1,0.0f,1.0f,0.0f, aoMap.bottomRight });
		vertices.push_back({ x + 0.0f,y + 1.0f,z + 1.0f,textureUV->uvX1,textureUV->uvY1,0.0f,1.0f,0.0f, aoMap.bottomLeft });
		vertices.push_back({ x + 0.0f,y + 1.0f,z + 0.0f,textureUV->uvX1,textureUV->uvY2,0.0f,1.0f,0.0f, aoMap.topLeft });

		indices.push_back(index * 4 + 0);
		indices.push_back(index * 4 + 1);
		indices.push_back(index * 4 + 3);
		indices.push_back(index * 4 + 1);
		indices.push_back(index * 4 + 2);
		indices.push_back(index * 4 + 3);

		index++;
	}

	unsigned int getVertexBufferSize() {
		return (unsigned int)vertices.size();
	}

	unsigned int getIndexBufferSize() {
		return (unsigned int)indices.size();
	}

	void printIndexBuffer() {
		for (int i = 0; i < indices.size(); i++) {
			std::cout << "Index " << i << ": " << indices[i] << std::endl;
		}
	}

	void printVertexBuffer() {
		for (int i = 0; i < vertices.size(); i++) {
			Vertex vertex = vertices[i];
			std::cout << "Vertex " << i << ": x: " << vertex.x << ", y: " << vertex.y << ", z: " << vertex.z << std::endl;
		}
	}

	void push() {
		vertices.clear();
		indices.clear();
		index = 0;
	}

	void pop() {
		//glBufferSubData(GL_ARRAY_BUFFER, 0, getVertexBufferSize() * sizeof(Vertex), &vertices.front());
		//glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, getIndexBufferSize() * sizeof(unsigned int), &indices.front());
		glBufferData(GL_ARRAY_BUFFER, getVertexBufferSize() * sizeof(Vertex), &vertices.front(), GL_DYNAMIC_DRAW);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, getIndexBufferSize() * sizeof(unsigned int), &indices.front(), GL_DYNAMIC_DRAW);
	}

	void render(unsigned int VAO) {
		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, getIndexBufferSize(), GL_UNSIGNED_INT, 0);
	}

	~Buffer() {
		vertices.clear();
		indices.clear();
	}
};

#endif