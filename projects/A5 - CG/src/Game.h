#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <unordered_map>
#include <vector>
#include <functional>

#include "GLM/glm.hpp"

#include "Mesh.h"
#include "Shader.h"
#include "Camera.h"

class Game {
public:
	Game();
	~Game();

	void Run();

	void Resize(int newWidth, int newHeight);

protected:
	void Initialize();
	void Shutdown();

	void LoadContent();
	void UnloadContent();

	void InitImGui();
	void ShutdownImGui();

	void ImGuiNewFrame();
	void ImGuiEndFrame();

	void Update(float deltaTime);
	void Draw(float deltaTime);
	void DrawGui(float deltaTime);

	glm::ivec2 myWindowSize;
	void __RenderScene(glm::ivec4 viewport, Camera::Sptr camera, bool wireFrame, bool color);

	//Probably use to select viewport (only 1 active at a time)
	bool Active1 = false; //numbers correspond to camera numbers
	bool Active2 = false;
	bool Active3 = false;
	bool Active = true; //set true to make it default

	//Mouse Input
	double xPosition = 2160;
	double yPosition = 2160;
	//Previous position, this will help update
	double prevX = 2160;
	double prevY = 2160;

	//Border
	int border = 4; // 4px border
	int selectborder = 4;

private:
	// Stores the main window that the game is running in
	GLFWwindow* myWindow;
	// Stores the clear color of the game's window
	glm::vec4   myClearColor;
	// Stores the title of the game's window
	char        myWindowTitle[32];

	Camera::Sptr myCamera;
	//Other 3 cameras
	Camera::Sptr myCamera1;
	Camera::Sptr myCamera2;
	Camera::Sptr myCamera3;

	// Our models transformation matrix
	glm::mat4   myModelTransform;

	//Different Camera Set Up
	struct Viewport
	{
		Camera::Sptr Camera;
		float MinX, MaxX;
		float MinY, MaxY;
		bool Enabled;
	};

	Viewport myViewports[4];

};