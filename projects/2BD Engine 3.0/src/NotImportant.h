#pragma once
/*
		auto& ecs = GetRegistry("Test");
		
		entt::entity e1 = ecs.create();
		MeshRenderer& m1 = ecs.assign<MeshRenderer>(e1);
		ecs.assign<TempTransform>(e1).Scale = glm::vec3(1.0f);
		m1.Material = testMat;
		m1.Mesh = myMesh;
		
		//Spider
		entt::entity e2 = ecs.create();
		MeshRenderer& m2 = ecs.assign<MeshRenderer>(e2);
		ecs.assign<TempTransform>(e2).Scale = glm::vec3(1.0f);
		m2.Material = testMat;
		m2.Mesh = myMeshObj;
		
		//Level1
		entt::entity L1 = ecs.create();
		MeshRenderer& Lv1 = ecs.assign<MeshRenderer>(L1);
		ecs.assign<TempTransform>(L1).Scale = glm::vec3(1.0f);
		Lv1.Material = testMat;
		Lv1.Mesh = mylevel;
		
		//Bed
		entt::entity e3 = ecs.create();
		MeshRenderer& m3 = ecs.assign<MeshRenderer>(e3);
		ecs.assign<TempTransform>(e3).Scale = glm::vec3(1.0f);
		m3.Material = testMat;
		m3.Mesh = myMeshObjBed;
		
		myModelTransformObj = glm::mat4(1.0f);
		myModelTransform1 = glm::mat4(1.0f);
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
				CurrentRegistry().get<TempTransform>(e).Position = glm::vec3(-2.0, 24.5, 0);
			};
			auto& BedSpawn = ecs.get_or_assign<UpdateBehaviour>(e3);//e3 is the bed
			BedSpawn.Function = BedPos1;
		}
		else if (rng % 3 == 1) {
			auto BedPos2 = [](entt::entity e, float dt) {
				CurrentRegistry().get<TempTransform>(e).Position = glm::vec3(-28.0, 17.5, 0);
			};
			auto& BedSpawn = ecs.get_or_assign<UpdateBehaviour>(e3);//e3 is the bed
			BedSpawn.Function = BedPos2;
		}
		else if (rng % 3 == 2) {
			auto BedPos3 = [](entt::entity e, float dt) {
				CurrentRegistry().get<TempTransform>(e).Position = glm::vec3(-28.0, 28.5, 0);
			};
			auto& BedSpawn = ecs.get_or_assign<UpdateBehaviour>(e3);//e3 is the bed
			BedSpawn.Function = BedPos3;
		}
		
		//Movement
		//Forward
		auto moveF = [](entt::entity e, float dt) {
			CurrentRegistry().get<TempTransform>(e).Position += glm::vec3(0, 0.005, 0);
		};
		//Back
		auto moveB = [](entt::entity e, float dt) {
			CurrentRegistry().get<TempTransform>(e).Position += glm::vec3(0, -0.005, 0);
		};
		//Right
		auto moveR = [](entt::entity e, float dt) {
			CurrentRegistry().get<TempTransform>(e).Position += glm::vec3(0.005, 0, 0);
		};
		//Left
		auto moveL = [](entt::entity e, float dt) {
			CurrentRegistry().get<TempTransform>(e).Position += glm::vec3(-0.005, 0, 0);
		};
		//Rotate Right
		auto rotR = [](entt::entity e, float dt) {
			CurrentRegistry().get<TempTransform>(e).EulerRotation += glm::vec3(0, 0, 90 * dt);
		};
		//Rotate Left
		auto rotL = [](entt::entity e, float dt) {
			CurrentRegistry().get<TempTransform>(e).EulerRotation += glm::vec3(0, 0, -90 * dt);
		};
		//Gets and assigns the behaviour (think I'll only need this to work for all of them).
		auto& up = ecs.get_or_assign<UpdateBehaviour>(e1);//e1 is the tile square
		
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
		
		//Movement
		auto& moveSpider = ecs.get_or_assign <UpdateBehaviour>(e2);
		//End of old

*/