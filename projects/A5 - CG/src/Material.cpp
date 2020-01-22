#include "Material.h"

void Material::Apply() {
	for (auto& kvp : myMat4s)
		myShader->SetUniform(kvp.first.c_str(), kvp.second);
	for (auto& kvp : myVec4s)
		myShader->SetUniform(kvp.first.c_str(), kvp.second);
	for (auto& kvp : myVec3s)
		myShader->SetUniform(kvp.first.c_str(), kvp.second);
	for (auto& kvp : myFloats)
		myShader->SetUniform(kvp.first.c_str(), kvp.second);
	for (auto& kvp : myInts)
		myShader->SetUniform(kvp.first.c_str(), kvp.second);

	// New in tutorial 06
	// updated in tutorial 09
	int slot = 0;
	for (auto& kvp : myTextures) {
		if (kvp.second.Sampler != nullptr)
			kvp.second.Sampler->Bind(slot);
		else
			TextureSampler::Unbind(slot);
		kvp.second.Texture->Bind(slot);
		myShader->SetUniform(kvp.first.c_str(), slot);
		slot++;
	}
	for (auto& kvp : myCubeMaps) {
		if (kvp.second.Sampler != nullptr)
			kvp.second.Sampler->Bind(slot);
		else
			TextureSampler::Unbind(slot);
		kvp.second.Texture->Bind(slot);
		myShader->SetUniform(kvp.first.c_str(), slot);
		slot++;
	}

	if (HasTransparency) {
		glEnable(GL_BLEND);
		glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE, GL_ONE);
	}
	else {
		glDisable(GL_BLEND);
	}

}