#include "Game.h"
#include "Logging.h"
#include "SceneManager.h"
#include "MeshRenderer.h"
#include "Texture2D.h"
#include <functional>

#include <stdexcept>

#define IMGUI_IMPL_OPENGL_LOADER_GLAD
#include "imgui.h"
#include "imgui_impl_opengl3.cpp"
#include "imgui_impl_glfw.cpp"

#include <GLM/glm.hpp>
#include <GLM/gtc/matrix_transform.hpp>

//Transformation Matrix
#include "Transform.h"
//New Object Loader
#include "ObjectLoader.h"

struct TempTransform {

	glm::vec3 SetPosition = glm::vec3(0.0f);
	glm::vec3 SetRotation = glm::vec3(0.0f);
	glm::vec3 SetScale = glm::vec3(1.0f);

	glm::mat4 GetWorldTransform() const {
		return
			glm::translate(glm::mat4(1.0f), SetPosition) *
			glm::mat4_cast(glm::quat(glm::radians(SetRotation))) *
			glm::scale(glm::mat4(1.0f), SetScale);
	}
};

struct UpdateBehaviour {
	std::function<void(entt::entity e, float dt)> Function;
};

/*
	Handles debug messages from OpenGL
	https://www.khronos.org/opengl/wiki/Debug_Output#Message_Components
	@param source    Which part of OpenGL dispatched the message
	@param type      The type of message (ex: error, performance issues, deprecated behavior)
	@param id        The ID of the error or message (to distinguish between different types of errors, like nullref or index out of range)
	@param severity  The severity of the message (from High to Notification)
	@param length    The length of the message
	@param message   The human readable message from OpenGL
	@param userParam The pointer we set with glDebugMessageCallback (should be the game pointer)
*/
void GlDebugMessage(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam) {
	switch (severity) {
	case GL_DEBUG_SEVERITY_LOW:          LOG_INFO(message); break;
	case GL_DEBUG_SEVERITY_MEDIUM:       LOG_WARN(message); break;
	case GL_DEBUG_SEVERITY_HIGH:         LOG_ERROR(message); break;
#ifdef LOG_GL_NOTIFICATIONS
	case GL_DEBUG_SEVERITY_NOTIFICATION: LOG_INFO(message); break;
#endif
	default: break;
	}
}

void GlfwWindowResizedCallback(GLFWwindow* window, int width, int height) {
	//glViewport(0, 0, width, height);
	Game* game = (Game*)glfwGetWindowUserPointer(window);
	if (game) {
		game->Resize(width, height);
	}
}

Game::Game() :
	myWindow(nullptr),
	myWindowTitle("Game"),
	myClearColor(glm::vec4(0, 0, 0, 1)),
	myModelTransform(glm::mat4(1))
{ }

Game::~Game() { }

void Game::Run()
{
	Initialize();
	InitImGui();
	LoadContent();

	static float prevFrame = glfwGetTime();

	// Run as long as the window is open
	while (!glfwWindowShouldClose(myWindow)) {
		// Poll for events from windows (clicks, keypressed, closing, all that)
		glfwPollEvents();

		float thisFrame = glfwGetTime();
		float deltaTime = thisFrame - prevFrame;

		Update(deltaTime);
		Draw(deltaTime);

		ImGuiNewFrame();
		DrawGui(deltaTime);
		ImGuiEndFrame();

		// Store this frames time for the next go around
		prevFrame = thisFrame;

		// Present our image to windows
		glfwSwapBuffers(myWindow);
	}

	LOG_INFO("Shutting down...");

	UnloadContent();

	ShutdownImGui();
	Shutdown();
}

void Game::Resize(int newWidth, int newHeight) {
	myCamera->Projection = glm::perspective(glm::radians(60.0f), newWidth / (float)newHeight, 0.01f, 1000.0f);
}

void Game::Initialize() {
	// Initialize GLFW
	if (glfwInit() == GLFW_FALSE) {
		std::cout << "Failed to initialize GLFW" << std::endl;
		throw std::runtime_error("Failed to initialize GLFW");
	}

	// Enable transparent backbuffers for our windows (note that Windows expects our colors to be pre-multiplied with alpha)
	glfwWindowHint(GLFW_TRANSPARENT_FRAMEBUFFER, true);

	// Create a new GLFW window
	myWindow = glfwCreateWindow(1080, 1080, myWindowTitle, nullptr, nullptr);

	// Tie our game to our window, so we can access it via callbacks
	glfwSetWindowUserPointer(myWindow, this);

	// Set our window resized callback
	glfwSetWindowSizeCallback(myWindow, GlfwWindowResizedCallback);

	// We want GL commands to be executed for our window, so we make our window's context the current one
	glfwMakeContextCurrent(myWindow);

	// Let glad know what function loader we are using (will call gl commands via glfw)
	if (gladLoadGLLoader((GLADloadproc)glfwGetProcAddress) == 0) {
		std::cout << "Failed to initialize Glad" << std::endl;
		throw std::runtime_error("Failed to initialize GLAD");
	}

	// Log our renderer and OpenGL version
	LOG_INFO(glGetString(GL_RENDERER));
	LOG_INFO(glGetString(GL_VERSION));

	// Enable debugging, and route messages to our callback
	glEnable(GL_DEBUG_OUTPUT);
	glDebugMessageCallback(GlDebugMessage, this);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
}

void Game::Shutdown() {
	glfwTerminate();
}

glm::vec4 testColor = glm::vec4(1.0f, 0.0f, 1.0f, 1.0f);

void Game::LoadContent() {
	myCamera = std::make_shared<Camera>();
	myCamera->SetPosition(glm::vec3(CameraPosX, CameraPosY, CameraPosZ)); //Camera Position
	myCamera->LookAt(glm::vec3(0), glm::vec3(0, 0, 1)); // Can replace with character position later
	myCamera->Projection = glm::perspective(glm::radians(60.0f), 1.0f, 0.01f, 1000.0f);

	// Create our 4 vertices
	Vertex vertices[4] = {
		// Position Color Normal Tex Coords
		// x y z r g b a x y z u v
		{{ -7.5f, -7.5f, 0.0f }, { 1.0f, 1.0f, 1.0f, 1.0f }, { 0.0f, 0.0f, 1.0f }, { 1.0f, 0.0f }},
		{{ 7.5f, -7.5f, 0.0f }, { 1.0f, 1.0f, 1.0f, 1.0f }, { 0.0f, 0.0f, 1.0f }, { 0.0f, 0.0f }},
		{{ -7.5f, 7.5f, 0.0f }, { 1.0f, 1.0f, 1.0f, 1.0f }, { 0.0f, 0.0f, 1.0f }, { 1.0f, 1.0f }},
		{{ 7.5f, 7.5f, 0.0f }, { 1.0f, 1.0f, 1.0f, 1.0f }, { 0.0f, 0.0f, 1.0f }, { 0.0f, 1.0f }}
	};
	// Create our 6 indices
	uint32_t indices[6] = {
		0, 1, 2,
		2, 1, 3
	};

	// Create a new mesh from the data, in this case the 4 floors
	myMesh = std::make_shared<Mesh>(vertices, 4, indices, 6);
	myMesh2 = std::make_shared<Mesh>(vertices, 4, indices, 6);
	myMesh3 = std::make_shared<Mesh>(vertices, 4, indices, 6);
	myMesh4 = std::make_shared<Mesh>(vertices, 4, indices, 6);

	// Create and compile shader
	myShader = std::make_shared<Shader>();
	myShader->Load("passthrough.vs", "passthrough.fs");

	myNormalShader = std::make_shared<Shader>();
	myNormalShader->Load("passthrough.vs", "normalView.fs");

	Shader::Sptr phong = std::make_shared<Shader>();
	phong->Load("lighting.vs.glsl", "blinn-phong.fs.glsl");

	glm::vec3 position = myCamera->GetPosition();

	Material::Sptr testMat = std::make_shared<Material>(phong);
	testMat->Set("a_LightPos", { -2, 1.5, 2 });
	//testMat->Set("a_LightPos", { position });
	testMat->Set("a_LightColor", { 1.0f, 1.0f, 1.0f }); //color of the light 
	testMat->Set("a_AmbientColor", { 1.0f, 1.0f, 0.9f }); //color of the scene
	testMat->Set("a_AmbientPower", 0.2f); //basically sets the color of scene
	testMat->Set("a_LightSpecPower", 0.5f);
	testMat->Set("a_LightShininess", 256);
	testMat->Set("a_LightAttenuation", 1.0f);
	//Texture2D::Sptr albedo = Texture2D::LoadFromFile("color-grid.png");
	Texture2D::Sptr albedo = Texture2D::LoadFromFile("Tile.png");
	testMat->Set("s_Albedo", albedo);

	//Second Light
	Shader::Sptr phong2 = std::make_shared<Shader>();
	phong2->Load("lighting.vs.glsl", "blinn-phong.fs.glsl");
	Material::Sptr testMat2 = std::make_shared<Material>(phong2);
	testMat2->SetTest("a_LightPos", { -28.5, 14, 1 }, "a_LightPos2", { -26, 2, 1 });
	testMat2->Set("a_LightColor", { 1.0f, 1.0f, 1.0f }); //color of the light 
	testMat2->Set("a_AmbientColor", { 1.0f, 1.0f, 0.9f });
	testMat2->Set("a_AmbientPower", 0.2f);
	testMat2->Set("a_LightSpecPower", 0.5f);
	testMat2->Set("a_LightShininess", 256);
	testMat2->Set("a_LightAttenuation", 1.0f);
	Texture2D::Sptr albedo2 = Texture2D::LoadFromFile("Tile.png");
	testMat2->Set("s_Albedo", albedo2);

	//New Object loader 
	glm::vec4 baseColor = glm::vec4(1, 1, 1, 1);
	//Objectloader::LoadObject("Level1_Floorless.obj", baseColor);

	//Objectloader::LoadObjectToMesh("Level1_Floorless.obj", baseColor);


	//Engine
	//Load UV spider
	OpenObj("SpiderModelUVF1.obj", ObjMeshData, uvs, normals);
	//The obj's mesh data
	myMeshObj = std::make_shared<Mesh>(ObjMeshData.data(), ObjMeshData.size(), nullptr, 0);

	//Load main character
	OpenObj("Johnny.obj", MainCharData, uvs, normals);
	MainCharacter = std::make_shared<Mesh>(MainCharData.data(), MainCharData.size(), nullptr, 0);

	//load bed
	OpenObj("Bed.obj", ObjMeshData, uvs, normals);
	myMeshObjBed = std::make_shared<Mesh>(ObjMeshData.data(), ObjMeshData.size(), nullptr, 0);

	//load Level
	OpenObj("Level1_Floorless.obj", ObjMeshData, uvs, normals);
	mylevel = std::make_shared<Mesh>(ObjMeshData.data(), ObjMeshData.size(), nullptr, 0);

	//square
	myModelTransform = glm::mat4(1.0f);
	//End Engine


	SceneManager::RegisterScene("Test");
	SceneManager::RegisterScene("Test2");
	SceneManager::SetCurrentScene("Test");

	{
		auto& ecs = GetRegistry("Test");

		//Older Transformations
		entt::entity e1 = ecs.create();
		MeshRenderer& m1 = ecs.assign<MeshRenderer>(e1);
		ecs.assign<TempTransform>(e1).SetScale = glm::vec3(1.0f);
		m1.Material = testMat;
		m1.Mesh = myMesh;

		//Square 2
		entt::entity e4 = ecs.create();
		MeshRenderer& m4 = ecs.assign<MeshRenderer>(e4);
		ecs.assign<TempTransform>(e4).SetScale = glm::vec3(1.0f);
		m4.Material = testMat2;
		m4.Mesh = myMesh2;

		//Square 3
		entt::entity e5 = ecs.create();
		MeshRenderer& m5 = ecs.assign<MeshRenderer>(e5);
		ecs.assign<TempTransform>(e5).SetScale = glm::vec3(1.0f);
		m5.Material = testMat2;
		m5.Mesh = myMesh3;

		//Square 4
		entt::entity e6 = ecs.create();
		MeshRenderer& m6 = ecs.assign<MeshRenderer>(e6);
		ecs.assign<TempTransform>(e6).SetScale = glm::vec3(1.0f);
		m6.Material = testMat2;
		m6.Mesh = myMesh4;

		//Johnny
		entt::entity MainChar = ecs.create();
		MeshRenderer& mc = ecs.assign<MeshRenderer>(MainChar);
		ecs.assign<TempTransform>(MainChar).SetScale = glm::vec3(1.0f);
		mc.Material = testMat;
		//mc.Mesh = MainCharacter; //Not currently drawing Johnny

		//Spider
		entt::entity e2 = ecs.create();
		MeshRenderer& m2 = ecs.assign<MeshRenderer>(e2);
		ecs.assign<TempTransform>(e2).SetScale = glm::vec3(1.2f);
		m2.Material = testMat;
		m2.Mesh = myMeshObj;

		//Level1
		entt::entity L1 = ecs.create();
		MeshRenderer& Lv1 = ecs.assign<MeshRenderer>(L1);
		ecs.assign<TempTransform>(L1).SetScale = glm::vec3(1.0f);
		Lv1.Material = testMat;
		Lv1.Mesh = mylevel;

		//Bed
		entt::entity e3 = ecs.create();
		MeshRenderer& m3 = ecs.assign<MeshRenderer>(e3);
		ecs.assign<TempTransform>(e3).SetScale = glm::vec3(1.0f);
		m3.Material = testMat;
		m3.Mesh = myMeshObjBed;

		//Required for our current movement system (for main character)
		myModelTransformObj = glm::mat3(1.0f);
		myModelTransform1 = glm::mat4(1.0f);

		//Setting up the floor textures
		auto FloorPos1 = [](entt::entity e, float dt) {
			CurrentRegistry().get<TempTransform>(e).SetPosition = glm::vec3(-7.5, 7.5, 0);
		};
		auto& Flooring1 = ecs.get_or_assign<UpdateBehaviour>(e1);//floor
		Flooring1.Function = FloorPos1;
		//Setting up the floor textures
		auto FloorPos2 = [](entt::entity e, float dt) {
			CurrentRegistry().get<TempTransform>(e).SetPosition = glm::vec3(-22.5, 7.5, 0);
		};
		auto& Flooring2 = ecs.get_or_assign<UpdateBehaviour>(e4);//floor
		Flooring2.Function = FloorPos2;
		//Floor 3
		auto FloorPos3 = [](entt::entity e, float dt) {
			CurrentRegistry().get<TempTransform>(e).SetPosition = glm::vec3(-7.5, 22.5, 0);
		};
		auto& Flooring3 = ecs.get_or_assign<UpdateBehaviour>(e5);//floor
		Flooring3.Function = FloorPos3;
		//Floor 4
		auto FloorPos4 = [](entt::entity e, float dt) {
			CurrentRegistry().get<TempTransform>(e).SetPosition = glm::vec3(-22.5, 22.5, 0);
		};
		auto& Flooring4 = ecs.get_or_assign<UpdateBehaviour>(e6);//floor
		Flooring4.Function = FloorPos4;

		//RNG Bed Spawning 
		int rng;
		rng = rand() % 3;
		auto start = std::chrono::system_clock::now();
		std::vector<int> v(100000, 42);
		auto end = std::chrono::system_clock::now();
		std::chrono::duration<double> diff = end - start;

		rng = diff.count() * 100000;
		if (rng % 3 == 0) {
			auto BedPos1 = [](entt::entity e, float dt) {
				CurrentRegistry().get<TempTransform>(e).SetPosition = glm::vec3(-2.0, 24.5, 0);
			};
			auto& BedSpawn = ecs.get_or_assign<UpdateBehaviour>(e3);//e3 is the bed
			BedSpawn.Function = BedPos1;
		}
		else if (rng % 3 == 1) {
			auto BedPos2 = [](entt::entity e, float dt) {
				CurrentRegistry().get<TempTransform>(e).SetPosition = glm::vec3(-28.0, 17.5, 0);
			};
			auto& BedSpawn = ecs.get_or_assign<UpdateBehaviour>(e3);//e3 is the bed
			BedSpawn.Function = BedPos2;
		}
		else if (rng % 3 == 2) {
			auto BedPos3 = [](entt::entity e, float dt) {
				CurrentRegistry().get<TempTransform>(e).SetPosition = glm::vec3(-28.0, 28.5, 0);
			};
			auto& BedSpawn = ecs.get_or_assign<UpdateBehaviour>(e3);//e3 is the bed
			BedSpawn.Function = BedPos3;
		}

		//Movement (New system)
		//Forward
		auto moveF = [](entt::entity e, float dt) {
			CurrentRegistry().get<TempTransform>(e).SetPosition += glm::vec3(0, 0.005, 0);
		};
		//Back
		auto moveB = [](entt::entity e, float dt) {
			CurrentRegistry().get<TempTransform>(e).SetPosition += glm::vec3(0, -0.005, 0);
		};
		//Right
		auto moveR = [](entt::entity e, float dt) {
			CurrentRegistry().get<TempTransform>(e).SetPosition += glm::vec3(0.005, 0, 0);
		};
		//Left
		auto moveL = [](entt::entity e, float dt) {
			CurrentRegistry().get<TempTransform>(e).SetPosition += glm::vec3(-0.005, 0, 0);
		};
		//Rotate Right
		auto rotR = [](entt::entity e, float dt) {
			CurrentRegistry().get<TempTransform>(e).SetRotation += glm::vec3(0, 0, 90 * dt);
		};
		//Rotate Left
		auto rotL = [](entt::entity e, float dt) {
			CurrentRegistry().get<TempTransform>(e).SetRotation += glm::vec3(0, 0, -90 * dt);
		};

		//This moves the spider you see at start 
		//Spider Movement
		auto CircleMoving = [](entt::entity e, float dt) {
			CurrentRegistry().get<TempTransform>(e).SetPosition += glm::vec3(0, 0.005, 0);
			CurrentRegistry().get<TempTransform>(e).SetRotation += glm::vec3(0, 0, -90 * dt); //not moving as I though
		}; 
		//Movement that actually causes it move forever
		auto& moveSpider = ecs.get_or_assign <UpdateBehaviour>(e2);//e2 is the spider 
		moveSpider.Function = CircleMoving;

		//This will aplly the movement functions above
		auto& up = ecs.get_or_assign<UpdateBehaviour>(e1);

		//Basically updates position forever
		if (moveForward == true) { //Forward Moving 
			up.Function = moveF;
		}
		if (moveBack == true) { //Back Moving 
			up.Function = moveB;
		}
		if (moveLeft == true) { //Left Moving 
			up.Function = moveL;
		}
		if (moveRight == true) { //Right Moving 
			up.Function = moveR;
		}
		if (rotateLeft == true) { //Rotation Left
			up.Function = rotL;
		}
		if (rotateRight == true) { //Rotation Right
			up.Function = rotR;
		}
	}
}

void Game::UnloadContent() {

}

void Game::InitImGui() {
	// Creates a new ImGUI context
	ImGui::CreateContext();
	// Gets our ImGUI input/output 
	ImGuiIO& io = ImGui::GetIO();
	// Enable keyboard navigation
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
	// Allow docking to our window
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
	// Allow multiple viewports (so we can drag ImGui off our window)
	io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;
	// Allow our viewports to use transparent backbuffers
	io.ConfigFlags |= ImGuiConfigFlags_TransparentBackbuffers;

	// Set up the ImGui implementation for OpenGL
	ImGui_ImplGlfw_InitForOpenGL(myWindow, true);
	ImGui_ImplOpenGL3_Init("#version 410");

	// Dark mode FTW
	ImGui::StyleColorsDark();

	// Get our imgui style
	ImGuiStyle& style = ImGui::GetStyle();
	//style.Alpha = 1.0f;
	if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
		style.WindowRounding = 0.0f;
		style.Colors[ImGuiCol_WindowBg].w = 0.8f;
	}
}

void Game::ShutdownImGui() {
	// Cleanup the ImGui implementation
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	// Destroy our ImGui context
	ImGui::DestroyContext();
}

void Game::ImGuiNewFrame() {
	// Implementation new frame
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	// ImGui context new frame
	ImGui::NewFrame();
}

void Game::ImGuiEndFrame() {
	// Make sure ImGui knows how big our window is
	ImGuiIO& io = ImGui::GetIO();
	int width{ 0 }, height{ 0 };
	glfwGetWindowSize(myWindow, &width, &height);
	io.DisplaySize = ImVec2(width, height);

	// Render all of our ImGui elements
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

	// If we have multiple viewports enabled (can drag into a new window)
	if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
		// Update the windows that ImGui is using
		ImGui::UpdatePlatformWindows();
		ImGui::RenderPlatformWindowsDefault();
		// Restore our gl context
		glfwMakeContextCurrent(myWindow);
	}
}

//Original movement (what main character uses)
void Game::Update(float deltaTime) {
	glm::vec3 movement = glm::vec3(0.0f);
	glm::vec3 rotation = glm::vec3(0.0f);

	float speed = 3.25f;
	float rotSpeed = 1.2f;

	// Get the camera's position so we can edit it
	glm::vec3 position = myCamera->GetPosition();
	glm::vec3 positionC = position - position;

	//Forward
	if (glfwGetKey(myWindow, GLFW_KEY_W) == GLFW_PRESS) {
		myModelTransformObj = glm::translate(myModelTransformObj, glm::vec3(0.0f, 0.013, 0.0));
		//myModelTransformObj += glm::translate(myModelTransformObj, glm::vec3(CameraPosX, CameraPosY, 0.0));
		movement.y += speed * deltaTime;
		moveForward = true;
	}
	//Left
	if (glfwGetKey(myWindow, GLFW_KEY_A) == GLFW_PRESS) {
		myModelTransformObj = glm::translate(myModelTransformObj, glm::vec3(-0.013, 0.0f, 0.0));
		movement.x -= speed * deltaTime;
		moveLeft = true;
	}
	//Back
	if (glfwGetKey(myWindow, GLFW_KEY_S) == GLFW_PRESS) {
		myModelTransformObj = glm::translate(myModelTransformObj, glm::vec3(0.0f, -0.013, 0.0));
		movement.y -= speed * deltaTime;
		moveBack = true;
	}
	//Right
	if (glfwGetKey(myWindow, GLFW_KEY_D) == GLFW_PRESS) {
		myModelTransformObj = glm::translate(myModelTransformObj, glm::vec3(0.013, 0.0f, 0.0));
		movement.x += speed * deltaTime;
		moveRight = true;
	}
	//Used these rotates before, they work fine are a little buggy with camera "follow" (IJKL, old movement, when I&K = forward/Back, J&L rotate, U&O Left/Right)
	//:Left rotate
	if (glfwGetKey(myWindow, GLFW_KEY_J) == GLFW_PRESS) {
		myModelTransformObj = glm::rotate(myModelTransformObj, 0.0055f, glm::vec3(0, 0, 1));
		rotateLeft = true;
	}
	//Right rotate
	if (glfwGetKey(myWindow, GLFW_KEY_L) == GLFW_PRESS) {
		myModelTransformObj = glm::rotate(myModelTransformObj, -0.0055f, glm::vec3(0, 0, 1));
		rotateRight = true;
	}
	myModelTransformObj = myModelTransformObj;

	if (glfwGetKey(myWindow, GLFW_KEY_J) == GLFW_PRESS)
		rotation.z -= rotSpeed * deltaTime;
	if (glfwGetKey(myWindow, GLFW_KEY_L) == GLFW_PRESS)
		rotation.z += rotSpeed * deltaTime;

	// Rotate and move our camera based on input
	myCamera->Rotate(rotation);
	myCamera->Move(movement);

	// Rotate our transformation matrix a little bit each frame
	myModelTransform = glm::rotate(myModelTransform, deltaTime, glm::vec3(0, 0, 1));


	static float angle = 90;

	auto view = CurrentRegistry().view<UpdateBehaviour>();
	for (const auto& e : view) {
		auto& func = CurrentRegistry().get<UpdateBehaviour>(e);
		if (func.Function) {
			func.Function(e, deltaTime);
		}
	}
}

void Game::Draw(float deltaTime) {
	// Clear our screen every frame
	glClearColor(myClearColor.x, myClearColor.y, myClearColor.z, myClearColor.w);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	myShader->Bind();
	//obj creation
	//glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), &vertices[0], GL_STATIC_DRAW);
	myShader->SetUniform("a_ModelViewProjection", myCamera->GetViewProjection() * myModelTransformObj);
	MainCharacter->Draw();

	// We'll grab a reference to the ecs to make things easier
	auto& ecs = CurrentRegistry();

	// We sort our mesh renderers based on material properties
	// This will group all of our meshes based on shader first, then material second
	ecs.sort<MeshRenderer>([](const MeshRenderer& lhs, const MeshRenderer& rhs) {
		if (rhs.Material == nullptr || rhs.Mesh == nullptr)
			return false;
		else if (lhs.Material == nullptr || lhs.Mesh == nullptr)
			return true;
		else if (lhs.Material->GetShader() != rhs.Material->GetShader())
			return lhs.Material->GetShader() < rhs.Material->GetShader();
		else
			return lhs.Material < rhs.Material;
		});

	// These will keep track of the current shader and material that we have bound
	Material::Sptr mat = nullptr;
	Shader::Sptr boundShader = nullptr;

	// A view will let us iterate over all of our entities that have the given component types
	auto view = ecs.view<MeshRenderer>();

	for (const auto& entity : view) {
		// Get our shader
		const MeshRenderer& renderer = ecs.get<MeshRenderer>(entity);

		// Early bail if mesh is invalid
		if (renderer.Mesh == nullptr || renderer.Material == nullptr)
			continue;
		// If our shader has changed, we need to bind it and update our frame-level uniforms
		if (renderer.Material->GetShader() != boundShader) {
			boundShader = renderer.Material->GetShader();
			boundShader->Bind();
			boundShader->SetUniform("a_CameraPos", myCamera->GetPosition());
		}
		// If our material has changed, we need to apply it to the shader
		if (renderer.Material != mat) {
			mat = renderer.Material;
			mat->Apply();
		}
		//Older Transforms
		// We'll need some info about the entities position in the world
		const TempTransform& transform = ecs.get_or_assign<TempTransform>(entity);
		// Get the object's transformation
		glm::mat4 worldTransform = transform.GetWorldTransform();
		// Our normal matrix is the inverse-transpose of our object's world rotation
		glm::mat3 normalMatrix = glm::mat3(glm::transpose(glm::inverse(transform.GetWorldTransform())));
		
		// Update the MVP using the item's transform
		mat->GetShader()->SetUniform(
			"a_ModelViewProjection",
			myCamera->GetViewProjection() *
			transform.GetWorldTransform());
		
		// Update the model matrix to the item's world transform
		mat->GetShader()->SetUniform("a_Model", transform.GetWorldTransform());
		//end of old

		//New Transformations
		//const Transform& transform = ecs.get_or_assign<Transform>(entity);
		//glm::mat4 worldTransform = transform.GetWorldTransform();
		//glm::mat3 normalMatrix = glm::mat3(glm::transpose(glm::inverse(worldTransform)));
		//
		//mat->GetShader()->SetUniform(
		//	"a_ModelViewProjection",
		//	myCamera->GetViewProjection() *
		//	worldTransform);
		//mat->GetShader()->SetUniform("a_Model", worldTransform);
		//end of new

		// Update the model matrix to the item's world transform
		mat->GetShader()->SetUniform("a_NormalMatrix", normalMatrix);
		// Draw the item
		renderer.Mesh->Draw();
	}
}

void Game::DrawGui(float deltaTime) {
	// Open a new ImGui window
	static bool testOpen = true;
	ImGui::Begin("Test", &testOpen, ImVec2(300, 200));
	// Draw a color editor
	ImGui::ColorEdit4("Clear Color", &myClearColor[0]);
	// Check if a textbox has changed, and update our window title if it has
	if (ImGui::InputText("Window Title", myWindowTitle, 32)) {
		glfwSetWindowTitle(myWindow, myWindowTitle);
	}
	// Our object's test color
	ImGui::ColorEdit4("Object Color", &testColor[0]);
	auto it = SceneManager::Each();
	for (auto& kvp : it) {
		if (ImGui::Button(kvp.first.c_str())) {
			SceneManager::SetCurrentScene(kvp.first);
		}
	}
	ImGui::End();

	// Open a second ImGui window
	ImGui::Begin("Debug");
	// Draw a formatted text line
	ImGui::Text("Time: %f", glfwGetTime());

	// Start a new ImGui header for our camera settings
	if (ImGui::CollapsingHeader("Camera Settings")) {
		// Draw our camera's normal
		glm::vec3 camNormal = myCamera->GetForward();
		ImGui::DragFloat3("Normal", &camNormal[0]);

		// Get the camera's position so we can edit it
		glm::vec3 position = myCamera->GetPosition();
		// Draw an editor control for the position, and update camera position
		if (ImGui::DragFloat3("Position", &position[0])) {
			myCamera->SetPosition(position);
		}
		if (ImGui::Button("Look at center")) {
			myCamera->LookAt(glm::vec3(0), glm::vec3(0, 0, 1));
		}
		// Get the camera pinning value
		static glm::vec3 camPin;

		// Get whether or not camera pinning is enabled
		bool camPlaneEnabled = myCamera->GetPinnedUp().has_value();
		// Draw a checkbox for camera pinning
		if (ImGui::Checkbox("Pinning Enabled", &camPlaneEnabled)) {
			// If we've disabled pinning, cache our pinning vector and remove it
			if (!camPlaneEnabled) {
				camPin = myCamera->GetPinnedUp().value();
				myCamera->SetPinnedUp(std::optional<glm::vec3>());
			}
			// Set our camera's pinning vector to our cached value
			else {
				myCamera->SetPinnedUp(camPin);
			}
		}
		// If we have enabled pinning
		if (camPlaneEnabled) {
			// Draw a slider for our camera pin direction
			if (ImGui::InputFloat3("Pin Direction", &camPin[0])) {
				myCamera->SetPinnedUp(camPin);
			}
		}
	}
	ImGui::End();
}


//Will replace with new OBj loader thats our goal before Review to just have new Obj loader
//Cleaner code, better lighting
//Also repo
//Taken from website and modified slightly
//http://www.opengl-tutorial.org/beginners-tutorials/tutorial-7-model-loading/
bool Game::OpenObj(const char* path, std::vector<Vertex>& out_vertices, std::vector<glm::vec2>& out_uvs, std::vector < glm::vec3 >& out_normals)
{
	std::vector< unsigned int > vertexIndices, uvIndices, normalIndices;
	std::vector< glm::vec3 > temp_vertices;
	std::vector< glm::vec2 > temp_uvs;
	std::vector< glm::vec3 > temp_normals;

	FILE* file = fopen(path, "r");
	if (file == NULL) {
		printf("Impossible to open the file !\n");
		return false;
	}

	while (1) {

		char lineHeader[128];
		// read the first word of the line
		int res = fscanf(file, "%s", lineHeader);
		if (res == EOF)
			break; // EOF = End Of File. Quit the loop.
		if (strcmp(lineHeader, "v") == 0) {
			glm::vec3 vertex;
			fscanf(file, "%f %f %f\n", &vertex.x, &vertex.y, &vertex.z);
			temp_vertices.push_back(vertex);
		}
		else if (strcmp(lineHeader, "vt") == 0) {
			glm::vec2 uv;
			fscanf(file, "%f %f\n", &uv.x, &uv.y);
			temp_uvs.push_back(uv);
		}
		else if (strcmp(lineHeader, "vn") == 0) {
			glm::vec3 normal;
			fscanf(file, "%f %f %f\n", &normal.x, &normal.y, &normal.z);
			temp_normals.push_back(normal);
		}
		else if (strcmp(lineHeader, "f") == 0) {
			std::string vertex1, vertex2, vertex3;
			unsigned int vertexIndex[3], uvIndex[3], normalIndex[3];
			int matches = fscanf(file, "%d/%*d/%*d %d/%*d/%*d %d/%*d/%*d\n", &vertexIndex[0], &vertexIndex[1], &vertexIndex[2]);

			if (matches != 3) {
				printf("File can't be read by our simple parser : ( Try exporting with other options\n");
				return false;
			}
			vertexIndices.push_back(vertexIndex[0]);
			vertexIndices.push_back(vertexIndex[1]);
			vertexIndices.push_back(vertexIndex[2]);
			uvIndices.push_back(uvIndex[0]);
			uvIndices.push_back(uvIndex[1]);
			uvIndices.push_back(uvIndex[2]);
			normalIndices.push_back(normalIndex[0]);
			normalIndices.push_back(normalIndex[1]);
			normalIndices.push_back(normalIndex[2]);
		}
	}
	out_vertices.resize(vertexIndices.size());
	out_uvs.resize(uvIndices.size()); //Might need
	out_normals.resize(normalIndices.size()); //Might need

	for (unsigned int i = 0; i < vertexIndices.size(); i++) {
		unsigned int vertexIndex = vertexIndices[i];
		glm::vec3 vertex = temp_vertices[vertexIndex - 1];
		out_vertices[i].Position = vertex;
		out_vertices[i].Color = glm::vec4(1.0f);
		//UVs
		unsigned int uvIndex = uvIndices[i];
		if (uvIndex == 0) {
			glm::vec2 uv = temp_uvs[uvIndex - 1];
			out_vertices[i].UV = uv;
		}

		//Normals
		unsigned int normalIndex = normalIndices[i];
		//glm::vec3 normal = temp_normals[normalIndex - 1];
		//out_normals[i].Position = normal;

		//out_uvs[i].x = glm::vec4(1.0f);
		//out_normals
		//out_vertices.push_back(vertex);
	}
}
