#pragma once
#include <GLM/glm.hpp>
#include <unordered_map>
#include <memory>
#include "Shader.h"
#include "Texture2D.h"
#include "TextureCube.h"

/*
Represents settings for a shader
*/
class Material {
public:
	typedef std::shared_ptr<Material> Sptr;

	bool HasTransparency;

	
	// Modify the existing constructor! Don’t add a new one!
	Material(const Shader::Sptr& shader) : HasTransparency(false) { myShader = shader; }
	virtual ~Material() = default;
	
	const Shader::Sptr& GetShader() const { return myShader; }
	virtual void Apply();
	
	void Set(const std::string& name, const glm::mat4& value) { myMat4s[name] = value; }
	void Set(const std::string& name, const glm::vec4& value) { myVec4s[name] = value; }
	void Set(const std::string& name, const glm::vec3& value) { myVec3s[name] = value; }
	void Set(const std::string& name, const float& value) { myFloats[name] = value; }

	// New in tutorial 06
	void Set(const std::string& name, const Texture2D::Sptr& value, const TextureSampler::Sptr& sampler = nullptr) {
		myTextures[name] = { value, sampler };
	 }
	void Set(const std::string& name, const TextureCube::Sptr& value, const TextureSampler::Sptr& sampler = nullptr) {
		myCubeMaps[name] = { value, sampler };
	}

	void Set(const std::string& name, const int& value) { myInts[name] = value; }
	
protected:
	struct Sampler2DInfo {
		Texture2D::Sptr Texture;
		TextureSampler::Sptr Sampler;
	};
	
	Shader::Sptr myShader;
	std::unordered_map<std::string, glm::mat4> myMat4s;
	std::unordered_map<std::string, glm::vec4> myVec4s;
	std::unordered_map<std::string, glm::vec3> myVec3s;
	std::unordered_map<std::string, glm::vec2> myVec2s;
	std::unordered_map<std::string, float> myFloats;
	std::unordered_map<std::string, int> myInts;

	// New in tutorial 06
	std::unordered_map<std::string, Sampler2DInfo> myTextures;

	struct SamplerCubeInfo {
		TextureCube::Sptr Texture;
		TextureSampler::Sptr Sampler;
	};
	std::unordered_map<std::string, SamplerCubeInfo> myCubeMaps;
};