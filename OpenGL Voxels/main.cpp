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

#define WIDTH 1920
#define HEIGHT 1080

#define MAX_TRANSLATE 128.0f

#define CHUNK_DIMENSIONS 32

#define SENSITIVITY 0.1f;

struct Vertex {
	float x;
	float y;
	float z;
	float uvX;
	float uvY;
	float normalX;
	float normalY;
	float normalZ;
};

class Buffer {
public:
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;
	unsigned int index = 0;

	void createBackVerticalPlane(float x, float y, float z) {
		vertices.push_back({ x + 0.0f, y + 1.0f,z + 0.0f,1.0f,1.0f,0.0f, 0.0f, -1.0f });
		vertices.push_back({ x + 0.0f,y + 0.0f,z + 0.0f,1.0f,0.0f,0.0f, 0.0f, -1.0f });
		vertices.push_back({ x + 1.0f,y + 0.0f,z + 0.0f,0.0f,0.0f,0.0f, 0.0f, -1.0f });
		vertices.push_back({ x + 1.0f,y + 1.0f,z + 0.0f,0.0f,1.0f,0.0f, 0.0f, -1.0f });

		indices.push_back(index * 4 + 0);
		indices.push_back(index * 4 + 1);
		indices.push_back(index * 4 + 3);
		indices.push_back(index * 4 + 1);
		indices.push_back(index * 4 + 2);
		indices.push_back(index * 4 + 3);

		index++;
	}

	void createFrontVerticalPlane(float x, float y, float z) {
		vertices.push_back({ x + 1.0f, y + 1.0f,z + 1.0f,1.0f,1.0f,0.0f, 0.0f, 1.0f });
		vertices.push_back({ x + 1.0f,y + 0.0f,z + 1.0f,1.0f,0.0f,0.0f, 0.0f, 1.0f });
		vertices.push_back({ x + 0.0f,y + 0.0f,z + 1.0f,0.0f,0.0f,0.0f, 0.0f, 1.0f });
		vertices.push_back({ x + 0.0f,y + 1.0f,z + 1.0f,0.0f,1.0f,0.0f, 0.0f, 1.0f });

		indices.push_back(index * 4 + 0);
		indices.push_back(index * 4 + 1);
		indices.push_back(index * 4 + 3);
		indices.push_back(index * 4 + 1);
		indices.push_back(index * 4 + 2);
		indices.push_back(index * 4 + 3);

		index++;
	}

	void createLeftVerticalPlane(float x, float y, float z) {
		vertices.push_back({ x + 0.0f, y + 1.0f,z + 1.0f,1.0f,1.0f,-1.0f, 0.0f, 0.0f });
		vertices.push_back({ x + 0.0f,y + 0.0f,z + 1.0f,1.0f,0.0f,-1.0f, 0.0f, 0.0f });
		vertices.push_back({ x + 0.0f,y + 0.0f,z + 0.0f,0.0f,0.0f,-1.0f, 0.0f, 0.0f });
		vertices.push_back({ x + 0.0f,y + 1.0f,z + 0.0f,0.0f,1.0f,-1.0f, 0.0f, 0.0f });

		indices.push_back(index * 4 + 0);
		indices.push_back(index * 4 + 1);
		indices.push_back(index * 4 + 3);
		indices.push_back(index * 4 + 1);
		indices.push_back(index * 4 + 2);
		indices.push_back(index * 4 + 3);

		index++;
	}

	void createRightVerticalPlane(float x, float y, float z) {
		vertices.push_back({ x + 1.0f, y + 1.0f,z + 0.0f,1.0f,1.0f,1.0f, 0.0f, 0.0f });
		vertices.push_back({ x + 1.0f,y + 0.0f,z + 0.0f,1.0f,0.0f,1.0f, 0.0f, 0.0f });
		vertices.push_back({ x + 1.0f,y + 0.0f,z + 1.0f,0.0f,0.0f,1.0f, 0.0f, 0.0f });
		vertices.push_back({ x + 1.0f,y + 1.0f,z + 1.0f,0.0f,1.0f,1.0f, 0.0f, 0.0f });

		indices.push_back(index * 4 + 0);
		indices.push_back(index * 4 + 1);
		indices.push_back(index * 4 + 3);
		indices.push_back(index * 4 + 1);
		indices.push_back(index * 4 + 2);
		indices.push_back(index * 4 + 3);

		index++;
	}

	void createBottomHorizontalPlane(float x, float y, float z) {
		vertices.push_back({ x + 1.0f, y + 0.0f,z + 1.0f,1.0f,1.0f,0.0f,-1.0f,0.0f });
		vertices.push_back({ x + 1.0f,y + 0.0f,z + 0.0f,1.0f,0.0f,0.0f,-1.0f,0.0f });
		vertices.push_back({ x + 0.0f,y + 0.0f,z + 0.0f,0.0f,0.0f,0.0f,-1.0f,0.0f });
		vertices.push_back({ x + 0.0f,y + 0.0f,z + 1.0f,0.0f,1.0f,0.0f,-1.0f,0.0f });

		indices.push_back(index * 4 + 0);
		indices.push_back(index * 4 + 1);
		indices.push_back(index * 4 + 3);
		indices.push_back(index * 4 + 1);
		indices.push_back(index * 4 + 2);
		indices.push_back(index * 4 + 3);

		index++;
	}

	void createTopHorizontalPlane(float x, float y, float z) {
		vertices.push_back({ x + 1.0f, y + 1.0f,z + 0.0f,1.0f,1.0f,0.0f,1.0f,0.0f });
		vertices.push_back({ x + 1.0f,y + 1.0f,z + 1.0f,1.0f,0.0f,0.0f,1.0f,0.0f });
		vertices.push_back({ x + 0.0f,y + 1.0f,z + 1.0f,0.0f,0.0f,0.0f,1.0f,0.0f });
		vertices.push_back({ x + 0.0f,y + 1.0f,z + 0.0f,0.0f,1.0f,0.0f,1.0f,0.0f });

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
		glBufferSubData(GL_ARRAY_BUFFER, 0, getVertexBufferSize() * sizeof(Vertex), &vertices.front());
		glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, getIndexBufferSize() * sizeof(unsigned int), &indices.front());
	}

	~Buffer() {
		vertices.clear();
		indices.clear();
	}
};

class Chunk {
public:
	unsigned int tiles[CHUNK_DIMENSIONS][CHUNK_DIMENSIONS][CHUNK_DIMENSIONS];

	//Init chunk
	Chunk() {
		generateTerrain();
	}

	//Get tile at given position, if out of bounds return 0 (Air)
	unsigned int getTile(int x, int y, int z) {
		if (x < 0 || x >= CHUNK_DIMENSIONS || y < 0 || y >= CHUNK_DIMENSIONS || z < 0 || z >= CHUNK_DIMENSIONS) return 0;
		return tiles[x][y][z];
	}

	//Generate mountains inside the 3D world array using perlin noise
	void generateTerrain() {
		noise::module::Perlin perlinNoise;
		perlinNoise.SetSeed(rand() % 100000);

		for (int x = 0; x < CHUNK_DIMENSIONS; x++) {
			for (int y = 0; y < CHUNK_DIMENSIONS; y++) {
				for (int z = 0; z < CHUNK_DIMENSIONS; z++) {
					tiles[x][y][z] = 0;
				}
			}
		}

		for (int x = 0; x < CHUNK_DIMENSIONS; x++) {
			for (int z = 0; z < CHUNK_DIMENSIONS; z++) {
				int height = (int)(abs(perlinNoise.GetValue((double)x / 64, (double)z / 64, 0.0f) * 32));
				for (int y = 0; y < height; y++) {
					tiles[x][y][z] = 1;
				}
			}
		}
	}

	//Generate a large cube
	void generateCube() {
		for (int x = 0; x < CHUNK_DIMENSIONS; x++) {
			for (int y = 0; y < CHUNK_DIMENSIONS; y++) {
				for (int z = 0; z < CHUNK_DIMENSIONS; z++) {
					tiles[x][y][z] = 1;
				}
			}
		}
	}

	//Convert the 3D world array to a mesh
	void pushToBuffer(Buffer* buffer) {
		for (int x = 0; x < CHUNK_DIMENSIONS; x++) {
			for (int y = 0; y < CHUNK_DIMENSIONS; y++) {
				for (int z = 0; z < CHUNK_DIMENSIONS; z++) {
					unsigned int tile = tiles[x][y][z];
					if (tile == 0) continue;
					float xPos = (float)x;
					float yPos = (float)y;
					float zPos = (float)z;
					if (getTile(x + 1, y, z) == 0) {
						buffer->createRightVerticalPlane(xPos, yPos, zPos);
					}
					if (getTile(x - 1, y, z) == 0) {
						buffer->createLeftVerticalPlane(xPos, yPos, zPos);
					}
					if (getTile(x, y + 1, z) == 0) {
						buffer->createTopHorizontalPlane(xPos, yPos, zPos);
					}
					if (getTile(x, y - 1, z) == 0) {
						buffer->createBottomHorizontalPlane(xPos, yPos, zPos);
					}
					if (getTile(x, y, z + 1) == 0) {
						buffer->createFrontVerticalPlane(xPos, yPos, zPos);
					}
					if (getTile(x, y, z - 1) == 0) {
						buffer->createBackVerticalPlane(xPos, yPos, zPos);
					}
				}
			}
		}
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

bool debugEnabled = false;
bool firstClick = true;
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
	Chunk *chunk = new Chunk;

	chunk->pushToBuffer(buffer);

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

	//Create and bind to vertex array object (storing used VBO, EBO and Vertex Attributes) (VBO bindings, active shader program, texture bindings, texture sampling/wrapping settings, uniforms)
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
	glm::vec3 lightColor = glm::vec3(1.0f);
	glUniform3fv(lightColorUniform, 1, glm::value_ptr(lightColor));


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

	float translateX = -CHUNK_DIMENSIONS/2;
	float translateY = 0.0f;
	float translateZ = -CHUNK_DIMENSIONS/2;
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

		glUniformMatrix4fv(transformUniform, 1, GL_FALSE, glm::value_ptr(transformMatrix));
		
		glClearColor(0.2f, 0.3f, 0.4f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
		//Draw graphics
		glBindVertexArray(VAO);
		glUseProgram(shaderProgram);
		glDrawElements(GL_TRIANGLES, buffer->getIndexBufferSize(), GL_UNSIGNED_INT, 0);

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
			if (ImGui::Button("Regenerate Chunk")) {
				buffer->push();
				chunk->generateTerrain();
				chunk->pushToBuffer(buffer);
				buffer->pop();
			};
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
	delete chunk;
	delete camera;

	return 0;
}