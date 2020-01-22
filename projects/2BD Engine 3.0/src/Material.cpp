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

	// New in tutorial 08
	int slot = 0;
	for (auto& kvp : myTextures) {
		kvp.second->Bind(slot);
		myShader->SetUniform(kvp.first.c_str(), slot);
		slot++;
	}
}