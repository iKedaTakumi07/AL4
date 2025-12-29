#pragma once
#include "KamataEngine.h"
#include "Math.h"
#include <cstdint>

using namespace KamataEngine;

class HitEffect {
public:
	enum class State {
		kSpread, // 拡大中
		kFade,   // フェードアウト中
		kDead    // 死亡
	};

	void Initialize(const Vector3& position);

	void Update();

	void Draw();

	static void SetModel(Model* model) { model_ = model; };

	static void SetCamera(Camera* camera) { camera_ = camera; };

	bool IsDead() const { return state_ == State::kDead; }

	static HitEffect* Create(const Vector3& position);

	static inline const uint32_t kSpreadTime = 10;

	static inline const uint32_t kFadeTime = 20;

	static inline const uint32_t kLifetime = kSpreadTime + kFadeTime;

private:
	static const inline uint32_t kellipseEffectNum = 2;

	std::array<WorldTransform, kellipseEffectNum> ellipseWorldTransforms_;

	static Model* model_;

	static Camera* camera_;

	WorldTransform circleWorldTransform_;

	ObjectColor objectColor_;

	State state_ = State::kSpread;

	uint32_t counter_ = 0;
};
