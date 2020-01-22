#pragma once
#include "Includes.h"
#include <iostream>
#include <unordered_map>
#include <vector>
#include <functional>

//#include "Material.h"

class Game {
public:
	Game();
	~Game();

	void Run();

	void Resize(int newWidth, int newHeight);

	bool orthoON = false;
	
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

	//From online 
	//http://www.opengl-tutorial.org/beginners-tutorials/tutorial-7-model-loading/ 
	bool OpenObj(const char* path, std::vector<Vertex>& out_vertices, std::vector <glm::vec2>& out_uvs, std::vector < glm::vec3 >& out_normals);//for Obj loading with UVs & normals

	//Movement Test
	bool rotateLeft = false;
	bool rotateRight = false;
	bool moveForward = false;
	bool moveBack = false;
	bool moveLeft = false;
	bool moveRight = false;

	float CameraPosX = 0;
	float CameraPosY = -2;
	float CameraPosZ = 10;

	private:
	// Stores the main window that the game is running in
	GLFWwindow* myWindow;
	// Stores the clear color of the game's window
	glm::vec4   myClearColor;
	// Stores the title of the game's window
	char        myWindowTitle[32];

	Camera::Sptr myCamera;
	// A shared pointer to our mesh
	Mesh::Sptr   myMesh;
	// A shared pointer to our shader
	Shader::Sptr myShader;
	// Shader for viewing normal maps
	Shader::Sptr myNormalShader;
	//Material::Sptr testMat;
	// Our models transformation matrix
	glm::mat4   myModelTransform;

	//Engine 
	//OBJ stuff
	Mesh::Sptr myMesh2;
	Mesh::Sptr myMesh3;
	Mesh::Sptr myMesh4;
	Mesh::Sptr myMeshObj;
	Shader::Sptr myShaderObj;
	glm::mat4 myModelTransformObj;
	glm::mat4 projection;
	std::vector<Vertex> ObjMeshData;//for the vertices
	//Might use for Obj loader
	std::vector< glm::vec3 > vertices;
	std::vector< glm::vec2 > uvs;
	std::vector< glm::vec3 > normals;

	//Bed
	Mesh::Sptr myMeshObjBed;
	std::vector<Vertex> ObjBedbMeshData;
	Shader::Sptr myShaderObjBed;
	//movement for smaller
	glm::mat4 myModelTransform1;


	//Level 1
	Mesh::Sptr mylevel;
	std::vector<Vertex> levelData;
	Shader::Sptr myShaderLevel;
	glm::mat4 myModelTransformLevel;

	//Main Character
	Mesh::Sptr MainCharacter;
	std::vector<Vertex> MainCharData;
	Shader::Sptr myShaderMainChar;


	//May delete code below later to make it better and more practical 
	//Catmull Rom
	std::vector<Vertex> v01;
	std::vector<Vertex> v02;
	std::vector<Vertex> v03;
	std::vector<Vertex> v04;

	Mesh::Sptr m01;
	Mesh::Sptr m02;
	Mesh::Sptr m03;
	Mesh::Sptr m04;

	Shader::Sptr s01;
	Shader::Sptr s02;
	Shader::Sptr s03;
	Shader::Sptr s04;

	// Tranformation Matrixes
	glm::mat4 t00;
	glm::mat4 t01;
	glm::mat4 t02;
	glm::mat4 t03;
	glm::mat4 t04;

	glm::vec3 tv01;
	glm::vec3 tv02;
	glm::vec3 tv03;
	glm::vec3 tv04;

	// Temporary 
	glm::vec3 tmp01;
	glm::vec3 tmp02;
	float index = 0.0f;
};