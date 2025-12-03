#pragma once
#include "KamataEngine.h"

class AbilityItem {
	void Initialize(KamataEngine::Model* model, KamataEngine::Camera* camera, const KamataEngine::Vector3& position);

	void Update();

	void Draw();

};
