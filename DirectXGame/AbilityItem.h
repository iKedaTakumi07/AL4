#pragma once
#include "KamataEngine.h"
#include "Math.h"

class AbilityItem {
public:
	enum class Behavior {
		kUnknown = -1,
		kExist,
		kDefeated,
	};

	void Initialize(KamataEngine::Model* model, KamataEngine::Camera* camera, const KamataEngine::Vector3& position);

	void Update();

	void Draw();

	AABB GetAABB();

	Vector3 GetWorldPosition();

	void SetNunber(uint32_t number);
	uint32_t GetNumber() { return AbilityNumber; }

	void OnCollision();

	bool isDead() const { return isDead_; }

	bool IsCollisionDisabled() const { return isCollisionDisabled_; };

private:
	// トランスフォーム
	WorldTransform worldTransform_;

	// モデル
	Model* model_ = nullptr;

	// カメラ
	Camera* camera_ = nullptr;

	// フラグ
	bool isDead_ = false;

	Behavior behavior_ = Behavior::kExist;
	Behavior behaviorRequest_ = Behavior::kUnknown;

	bool isCollisionDisabled_ = false;

	static inline const float kWidth = 0.8f;
	static inline const float kHeight = 0.8f;

	static inline const float kDefeatedTime = 0.6f;
	static inline const float kDefeatedMotionAngleStaart = 0.0f;
	static inline const float kDefeatedMotionAngleEnd = -60.0f;
	float counter_ = 0.0f;
	uint32_t AbilityNumber = 0;
};
