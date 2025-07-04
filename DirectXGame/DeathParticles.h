#pragma once

#include "Math.h"
#include "kamataEngine.h"
#include <algorithm>
#include <array>
#include <numbers>

using namespace KamataEngine;

class DeathParticles {
public:
	void Initialize(Model* model, Camera* camera, const Vector3& position);

	void Update();

	void Draw();

	bool IsFinished() { return isFinished_; }

private:
	Camera* camera_ = nullptr;
	Model* model_ = nullptr;

	// パーティクルの工数
	static inline const uint32_t kNumParticles = 8;

	std::array<WorldTransform, kNumParticles> worldTransform_;

	// 存在時間
	static inline const float kDuration = 2.5f;
	// 移動した速さ
	static inline const float kSpeed = 0.05f;
	// 分割した1つの角度
	static inline const float kAngleUnit = std::numbers::pi_v<float> / kNumParticles;

	// 終了フラグ
	bool isFinished_ = false;
	// 経過時間買ううんと
	float counter_ = 0.0f;
	// 色変更オブジェクト
	ObjectColor objectColor_;
	// 色の数値
	Vector4 color_;
};
