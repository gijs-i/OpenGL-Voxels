//(c) gijs.i voxel engine

#include <iostream>
#include <vector>
#include <math.h>
#include <string>

#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "noise/noise.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include "shader_handler.hpp"
#include "buffer.hpp"
#include "block.hpp"

#define WIDTH 1920
#define HEIGHT 1080

#define MAX_TRANSLATE 128.0f

#define CHUNK_DIMENSIONS 48

#define SENSITIVITY 0.1f;

/* 
	Chunk system certainly not final. World currently consists of only 1 chunk. An ideal system would be a std::vector containing multiple chunks and being able to load and unload certain chunks.
	Even better would being able to load and unload certain parts in the VBO to save on VRAM. A way to implement that would be to assign certain chunks of data in the VBO to certain chunks and when a chunk is unloaded deleting that data in the VBO.
	This is to avoid having to completely reupload the VBO from scratch and to only upload a certain part of it.
*/

class Chunk {
public:
	unsigned int tiles[CHUNK_DIMENSIONS][CHUNK_DIMENSIONS][CHUNK_DIMENSIONS];

	//Init chunk
	Chunk() {
		for (int x = 0; x < CHUNK_DIMENSIONS; x++) {
			for (int y = 0; y < CHUNK_DIMENSIONS; y++) {
				for (int z = 0; z < CHUNK_DIMENSIONS; z++) {
					tiles[x][y][z] = 0;
				}
			}
		}
		generateTerrain();
	}

	//Get tile at given position, if out of bounds return 0 (Air)
	unsigned int getTile(int x, int y, int z) {
		if (x < 0 || x >= CHUNK_DIMENSIONS || y < 0 || y >= CHUNK_DIMENSIONS || z < 0 || z >= CHUNK_DIMENSIONS) return 0;
		return tiles[x][y][z];
	}

	void setTile(unsigned int x, unsigned int y, unsigned int z, BlockEnum block) {
		tiles[x][y][z] = (unsigned int)block;
	}

	void regenerate() {
		for (int x = 0; x < CHUNK_DIMENSIONS; x++) {
			for (int y = 0; y < CHUNK_DIMENSIONS; y++) {
				for (int z = 0; z < CHUNK_DIMENSIONS; z++) {
					tiles[x][y][z] = 0;
				}
			}
		}
		generateTerrain();
	}

	//Generate mountains inside the 3D world array using perlin noise
	void generateTerrain() {
		noise::module::Perlin perlinNoise;
		srand(time(0));
		perlinNoise.SetSeed(rand() % 10000);

		std::cout << "[Chunk] Generating chunk..." << std::endl;

		for (int x = 0; x < CHUNK_DIMENSIONS; x++) {
			for (int z = 0; z < CHUNK_DIMENSIONS; z++) {
				float mainNoise = abs(perlinNoise.GetValue((float)x / 512.0f, 0.0f, (float)z / 512.0f)) * 16;
				float octaveNoise = abs(perlinNoise.GetValue((float)x / 128.0f, 0.0f, (float)z / 128.0f)) * 32;
				unsigned int height = 4 + (unsigned int)(mainNoise + octaveNoise);
				for (int y = 0; y < height; y++) {
					BlockEnum tile = abs(perlinNoise.GetValue((float)x / 256.0f, (float)y / 256.0f, (float)z / 256.0f)) > 0.1 ? BlockEnum::DIRT : BlockEnum::STONE;
					if (y == height - 1 && tile == BlockEnum::DIRT) tile = BlockEnum::GRASS;
					setTile(x, y, z, tile);
				}
			}
		}

		std::cout << "[Chunk] Chunk generated." << std::endl;
	}

	//Convert the 3D world array to a mesh
	void generateMesh(Buffer* buffer, BlockRegistry* blockRegistry) {
		std::cout << "[Chunk] Generating mesh..." << std::endl;
		for (int x = 0; x < CHUNK_DIMENSIONS; x++) {
			for (int y = 0; y < CHUNK_DIMENSIONS; y++) {
				for (int z = 0; z < CHUNK_DIMENSIONS; z++) {
					unsigned int tile = tiles[x][y][z];
					if (tile == 0) continue;

					BlockSpaceInfo blockSpaceInfo = {
						getTile(x + 1, y, z) == 0, //Right
						getTile(x - 1, y, z) == 0, //Left
						getTile(x, y + 1, z) == 0, //Top
						getTile(x, y - 1, z) == 0, //Bottom
						getTile(x, y, z + 1) == 0, //Front
						getTile(x, y, z - 1) == 0, //Back

						getTile(x + 1, y, z - 1) != 0, //Top-right
						getTile(x + 1, y, z + 1) != 0, //Bottom-right
						getTile(x - 1, y, z + 1) != 0, //Bottom-left
						getTile(x - 1, y, z - 1) != 0, //Top-left

						getTile(x, y + 1, z - 1) != 0, //Upper top
						getTile(x + 1, y + 1, z - 1) != 0, //Upper top-right
						getTile(x + 1, y + 1, z) != 0, //Upper right
						getTile(x + 1, y + 1, z + 1) != 0, //Upper down-right
						getTile(x, y + 1, z + 1) != 0, //Upper down
						getTile(x - 1, y + 1, z + 1) != 0, //Upper down-left
						getTile(x - 1, y + 1, z) != 0, //Upper left
						getTile(x - 1, y + 1, z - 1) != 0, //Upper Top-left

						getTile(x, y - 1, z - 1) != 0, //Lower top
						getTile(x + 1, y - 1, z - 1) != 0, //Lower top-right
						getTile(x + 1, y - 1, z) != 0, //Lower right
						getTile(x + 1, y - 1, z + 1) != 0, //Lower down-right
						getTile(x, y - 1, z + 1) != 0, //Lower down
						getTile(x - 1, y - 1, z + 1) != 0, //Lower down-left
						getTile(x - 1, y - 1, z) != 0, //Lower left
						getTile(x - 1, y - 1, z - 1) != 0, //Upper Top-left
					};

					Block* block = blockRegistry->getBlock(tile);
					block->generateMesh(buffer,(float)x,(float)y,(float)z,blockSpaceInfo);
				}
			}
		}
		std::cout << "[Chunk] Mesh generation done." << std::endl;
	}
};

class Camera {
public:
	glm::mat4 projectionMatrix;
	glm::mat4 viewMatrix;

	glm::vec3 cameraPos = glm::vec3(0.0f, 25.0f, 40.0f);
	glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
	glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

	float FOV = 45.0f;
	float cameraSpeed = 20.0f;
	float yaw = 0.0f;
	float pitch = 0.0f;

	unsigned int projectionUniform;
	unsigned int viewUniform;

	Camera(unsigned int shaderProgram) {
		projectionUniform = glGetUniformLocation(shaderProgram, "uProjection");
		projectionMatrix = glm::perspective(glm::radians(45.0f), (float)WIDTH / (float)HEIGHT, 0.1f, 1000.0f);
		glUniformMatrix4fv(projectionUniform, 1, GL_FALSE, glm::value_ptr(projectionMatrix));

		viewUniform = glGetUniformLocation(shaderProgram, "uView");
		viewMatrix = glm::lookAt(glm::vec3(0.0f, 0.0f, 3.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(viewUniform, 1, GL_FALSE, glm::value_ptr(viewMatrix));
	}

	void update() {
		viewMatrix = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
		projectionMatrix = glm::perspective(glm::radians(FOV), (float)WIDTH / (float)HEIGHT, 0.1f, 1000.0f);
		glUniformMatrix4fv(projectionUniform, 1, GL_FALSE, glm::value_ptr(projectionMatrix));
		glUniformMatrix4fv(viewUniform, 1, GL_FALSE, glm::value_ptr(viewMatrix));
	}

	void updateLookDirection(float xOffset, float yOffset) {
		yaw += xOffset;
		pitch += yOffset;
		if(pitch > 89.0f) pitch = 89.0f;
		if (pitch < -89.0f) pitch = -89.0f;
		glm::vec3 cameraDirection;
		cameraDirection.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
		cameraDirection.y = sin(glm::radians(pitch));
		cameraDirection.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
		cameraFront = glm::normalize(cameraDirection);
	}
};

Vector3 raycast(Chunk* chunk, glm::vec3 pos, glm::vec3 lookPos,float length, float stepSize) {
	unsigned int tile;
	int x;
	int y;
	int z;
	for (float i = 0.0f; i < length; i += stepSize) {
		glm::vec3 ray = pos + i * lookPos;
		x = (int)ray.x;
		y = (int)ray.y;
		z = (int)ray.z;
		tile = chunk->getTile(x,y,z);
		if (tile != 0) return { x, y, z };
	}
}

//Rays can hit a diagonal and place blocks on a diagonal. Maybe a grid snap of the lookPos vector should do the trick, allowing it to only place blocks on a certain face.
Vector3 raycastNormal(Chunk* chunk, glm::vec3 pos, glm::vec3 lookPos, float length, float stepSize) {
	unsigned int tile;
	int x;
	int y;
	int z;
	for (float i = 0.0f; i < length; i += stepSize) {
		glm::vec3 ray = pos + i * lookPos;
		int rayX = (int)ray.x;
		int rayY = (int)ray.y;
		int rayZ = (int)ray.z;
		tile = chunk->getTile(rayX, rayY, rayZ);
		if (tile == 0) {
			x = rayX;
			y = rayY;
			z = rayZ;
			std::cout << "[Raycast] Found air block before block at: " << x << ", " << y << ", " << z << std::endl;
		}
		else {
			std::cout << "[Raycast] Found block: " << tile << std::endl;
			break;
		}
	}
	return { x, y, z };
}

bool debugEnabled = false;
bool firstClick = true;
bool performRaycast = false;
float lastX = (float)(WIDTH / 2), lastY = (float)(HEIGHT / 2);
Camera* camera = nullptr;

void processInput(GLFWwindow* window, Camera* camera, float deltaTime) {
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) camera->cameraPos += camera->cameraSpeed * camera->cameraFront * deltaTime;
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) camera->cameraPos -= camera->cameraSpeed * camera->cameraFront * deltaTime;
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) camera->cameraPos -= camera->cameraSpeed * glm::normalize(glm::cross(camera->cameraFront, camera->cameraUp)) * deltaTime;
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) camera->cameraPos += camera->cameraSpeed * glm::normalize(glm::cross(camera->cameraFront, camera->cameraUp)) * deltaTime;
}

void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
	if (key == GLFW_KEY_F1 && action == GLFW_PRESS) {
		debugEnabled = !debugEnabled;
		if (debugEnabled) {
			glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		}
		else {
			glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
		}
	}
	if (key == GLFW_KEY_E && action == GLFW_PRESS) performRaycast = true;
}

void cursorPosCallback(GLFWwindow* window, double mouseXPos, double mouseYPos) {
	if (firstClick) {
		lastX = mouseXPos;
		lastY = mouseYPos;
		firstClick = false;
	}
	float xOffset = mouseXPos - lastX;
	float yOffset = lastY - mouseYPos;
	lastX = mouseXPos;
	lastY = mouseYPos;
	xOffset *= SENSITIVITY;
	yOffset *= SENSITIVITY;
	if (!debugEnabled) {
		camera->updateLookDirection(xOffset, yOffset);
	}
}

void scrollCallback(GLFWwindow* window, double xOffset, double yOffset) {
	camera->cameraSpeed += (float)yOffset;
	if (camera->cameraSpeed < 0.0f) camera->cameraSpeed = 0.0f;
	if (camera->cameraSpeed > 100.0f) camera->cameraSpeed = 100.0f;
}

int main() {

	Buffer *buffer = new Buffer;
	BlockRegistry* blockRegistry = new BlockRegistry;
	Chunk *chunk = new Chunk;

	chunk->generateMesh(buffer, blockRegistry);

	//Load GLFW
	if (!glfwInit()) {
		std::cout << "[GLFW] Couldn't initialize GLFW!" << std::endl;
	}

	//Specify OpenGL settings
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	//Create window
	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "OpenGL Voxels", NULL, NULL);

	if (!window) {
		std::cout << "[GLFW] Couldn't create window!" << std::endl;
		glfwTerminate();
	}

	glfwMakeContextCurrent(window);

	//Load GLAD
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cout << "[GLAD] Couldn't initialize GLAD!" << std::endl;
		return -1;
	}

	unsigned int shaderProgram = createShaderProgram("vertex_shader.shader", "fragment_shader.shader");

	//Create and bind to vertex array object (VBO and EBO bindings, active shader program, texture bindings, texture sampling/wrapping settings, attributes, uniforms)
	unsigned int VAO;
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	//Create and bind to vertex buffer object
	unsigned int VBO;
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, buffer->getVertexBufferSize() * sizeof(Vertex), &buffer->vertices.front(), GL_DYNAMIC_DRAW);


	//Create and bind to element buffer object
	unsigned int EBO;
	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, buffer->getIndexBufferSize() * sizeof(unsigned int), &buffer->indices.front(), GL_DYNAMIC_DRAW);

	//Create vertex attributes
	unsigned int positionAttribute = glGetAttribLocation(shaderProgram, "aPosition");
	glVertexAttribPointer(positionAttribute, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
	glEnableVertexAttribArray(positionAttribute);

	unsigned int uvAttribute = glGetAttribLocation(shaderProgram, "aTexCoord");
	glVertexAttribPointer(uvAttribute, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(uvAttribute);

	unsigned int normalAttribute = glGetAttribLocation(shaderProgram, "aNormal");
	glVertexAttribPointer(normalAttribute, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(5*sizeof(float)));
	glEnableVertexAttribArray(normalAttribute);

	unsigned int AOAttribute = glGetAttribLocation(shaderProgram, "aAO");
	glVertexAttribPointer(AOAttribute, 1, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(8 * sizeof(float)));
	glEnableVertexAttribArray(AOAttribute);

	//Create uniforms
	unsigned int timeUniform = glGetUniformLocation(shaderProgram, "uTime");
	glUniform1f(timeUniform, (float)glfwGetTime());

	unsigned int transformUniform = glGetUniformLocation(shaderProgram, "uTransform");
	glm::mat4 transformMatrix = glm::mat4(1.0f);
	glUniformMatrix4fv(transformUniform, 1, GL_FALSE,glm::value_ptr(transformMatrix));

	unsigned int lightPosUniform = glGetUniformLocation(shaderProgram, "uLightPos");
	glm::vec3 lightPos = glm::vec3(1.0f);
	glUniform3fv(lightPosUniform, 1, glm::value_ptr(lightPos));

	unsigned int ambientUniform = glGetUniformLocation(shaderProgram, "uAmbient");
	glm::vec3 ambientColor = glm::vec3(1.0f);
	glUniform3fv(ambientUniform, 1, glm::value_ptr(ambientColor));

	unsigned int lightColorUniform = glGetUniformLocation(shaderProgram, "uLightColor");
	glm::vec3 lightColor = glm::vec3(125.0f/255.0f, 125.0f/255.0f, 125.0f/255.0f);
	glUniform3fv(lightColorUniform, 1, glm::value_ptr(lightColor));

	unsigned int AOIntensityUniform = glGetUniformLocation(shaderProgram, "uAOIntensity");
	float AOIntensity = 0.15f;
	glUniform1f(AOIntensityUniform, AOIntensity);


	//unsigned int colorUniform = glGetUniformLocation(shaderProgram, "uColor");
	float colorValue[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
	//glUniform4fv(colorUniform, 1, &colorValue[0]);

	//Create texture
	//Create texture object
	unsigned int texture;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D,texture);
	
	//Set texture settings
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	//Load image data
	stbi_set_flip_vertically_on_load(true);
	int width, height, nrChannels;
	unsigned char* imageData = stbi_load("atlas.png", &width, &height, &nrChannels, STBI_rgb_alpha);

	if (imageData) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, imageData);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else {
		std::cout << "[STB] Failed to load image data!" << std::endl;
	}

	//Destroy unused image data
	stbi_image_free(imageData);

	//Generic GL settings
	glfwSwapInterval(0);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSetKeyCallback(window, keyCallback);
	glfwSetScrollCallback(window, scrollCallback);
	glfwSetCursorPosCallback(window, cursorPosCallback);
	glViewport(0, 0, WIDTH, HEIGHT);
	glEnable(GL_DEPTH_TEST);


	unsigned int glError;
	while (glError = glGetError()) {
		std::cout << "[OpenGL] Error: " << glError << std::endl;
		return -1;
	}

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init("#version 330 core");
	ImGui::StyleColorsDark();

	float deltaTime = 0.0;
	float lastFrame = 0.0f;

	float translateX = 0.0f;
	float translateY = 0.0f;
	float translateZ = 0.0f;
	float rotateX = 0.0f;
	float rotateY = 0.0f;
	float rotateZ = 0.0f;

	float lightX = 0.5f;
	float lightY = -1.0f;
	float lightZ = 0.25f;

	camera = new Camera(shaderProgram);

	while (!glfwWindowShouldClose(window)) {

		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;

		//Update matrices
		transformMatrix = glm::mat4(1.0f);
		transformMatrix = glm::rotate(transformMatrix, rotateX, glm::vec3(1.0f, 0.0f, 0.0f));
		transformMatrix = glm::rotate(transformMatrix, rotateY, glm::vec3(0.0f, 1.0, 0.0f));
		transformMatrix = glm::rotate(transformMatrix, rotateZ, glm::vec3(0.0f, 0.0f, 1.0f));
		transformMatrix = glm::translate(transformMatrix, glm::vec3(translateX,translateY,translateZ));

		//Update lighting
		lightPos = glm::vec3(lightX, lightY, lightZ);

		//Update and upload camera
		camera->update();

		//Upload data to the GPU
		glUniform1f(timeUniform, (float)glfwGetTime());
		
		glUniform3fv(lightPosUniform, 1, glm::value_ptr(lightPos));
		glUniform3fv(ambientUniform, 1, glm::value_ptr(ambientColor));
		glUniform3fv(lightColorUniform, 1, glm::value_ptr(lightColor));
		glUniform1f(AOIntensityUniform, AOIntensity);

		glUniformMatrix4fv(transformUniform, 1, GL_FALSE, glm::value_ptr(transformMatrix));
		
		glClearColor(0.2f, 0.3f, 0.4f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		if (performRaycast) {
			Vector3 blockPos = raycastNormal(chunk, camera->cameraPos, camera->cameraFront, 16.0f, 0.5f);
			chunk->setTile(blockPos.x, blockPos.y, blockPos.z, BlockEnum::STONE);
			buffer->push();
			//chunk->regenerate();
			chunk->generateMesh(buffer, blockRegistry);
			buffer->pop();
			performRaycast = false;
		}
		
		//Draw graphics
		buffer->render(VAO);

		//Begin ImGUI draw
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		if (debugEnabled) {
			ImGui::Begin("Chunk controls");
			ImGui::SliderFloat("Translate X", &translateX, -MAX_TRANSLATE, MAX_TRANSLATE);
			ImGui::SliderFloat("Translate Y", &translateY, -MAX_TRANSLATE, MAX_TRANSLATE);
			ImGui::SliderFloat("Translate Z", &translateZ, -MAX_TRANSLATE, MAX_TRANSLATE);
			ImGui::SliderAngle("Rotate X", &rotateX);
			ImGui::SliderAngle("Rotate Y", &rotateY);
			ImGui::SliderAngle("Rotate Z", &rotateZ);
			if (ImGui::Button("Render Chunk")) {
				buffer->push();
				chunk->generateMesh(buffer, blockRegistry);
				buffer->pop();
			};
			if (ImGui::Button("Raycast")) {
				raycast(chunk, camera->cameraPos, camera->cameraFront, 16.0f, 1.0f);
				buffer->push();
				chunk->generateMesh(buffer, blockRegistry);
				buffer->pop();
			}
			ImGui::End();

			ImGui::Begin("Camera controls");
			ImGui::SliderFloat("FOV", &camera->FOV, 0.0f, 180.0f);
			ImGui::SliderFloat("Camera Speed", &camera->cameraSpeed, 0.0f, 100.0f);
			ImGui::End();

			ImGui::Begin("Lighting controls");
			ImGui::SliderFloat("Light X", &lightX, -1.0f,1.0f);
			ImGui::SliderFloat("Light Y", &lightY, -1.0f, 1.0f);
			ImGui::SliderFloat("Light Z", &lightZ, -1.0f, 1.0f);
			ImGui::ColorPicker3("Ambient Color", glm::value_ptr(ambientColor));
			ImGui::ColorPicker3("Light Color", glm::value_ptr(lightColor));
			ImGui::SliderFloat("Ambient Occlusion Intensity", &AOIntensity, 0.0f, 1.0f);
			ImGui::End();
		}

		ImGui::Render();

		//Upload draw data to the GPU
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		glfwSwapBuffers(window);
		glfwPollEvents();
		processInput(window, camera, deltaTime);

		lastFrame = currentFrame;
	}

	glfwDestroyWindow(window);

	glfwTerminate();

	delete buffer;
	delete blockRegistry;
	delete chunk;
	delete camera;

	return 0;
}