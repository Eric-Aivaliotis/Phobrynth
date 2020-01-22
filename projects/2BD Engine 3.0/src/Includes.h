#pragma once
//Just to keep all includes in one place,
//basically all .h and .cpp will only include this

//All .h programs we coded
#include "BoundingBox.h"
#include "Camera.h"
#include "Game.h"
#include "Mesh.h"
#include "Shader.h"

//For Game.h
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "GLM/glm.hpp"
#include "Mesh.h"
#include "Shader.h"
#include "Camera.h"
#include <vector>

#include <fstream>
#include <string>
#include <sstream>

//Better Model Loading 
//Some includes here are for other basic functions
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <map>
#include <vector>


//currently commented out as I'm not using it for current tests

//Assimp Includes 
//$(SolutionDir)/Additional   For some reason removes my relative linking every time
//#include <assimp/Importer.hpp>
//#include <assimp/scene.h>
//#include <assimp/postprocess.h>
//
////Soil include
//#include <SOIL2/SOIL2.h>