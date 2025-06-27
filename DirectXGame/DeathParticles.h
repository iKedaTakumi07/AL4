#pragma once

#include "Math.h"
#include "kamataEngine.h"
#include <array>

using namespace KamataEngine;

class DeathParticles {
public:
	void Initialize(Model* model, Camera* camera, const Vector3& position);

	void Update();

	void Draw();

private:
	Camera* camera_ = nullptr;
	Model* model_ = nullptr;

	// パーティクルの工数
	static inline const uint32_t kNumParticles = 8;

	std::array<WorldTransform, kNumParticles> worldTransform_;
};
