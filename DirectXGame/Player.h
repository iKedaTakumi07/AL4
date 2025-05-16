#pragma once
#include "KamataEngine.h"

// 左右
enum class LRDirection {
	kRight,
	kleft,
};

class Player {
public:
	void Initialize(KamataEngine::Model* model, KamataEngine::Camera* camera, const KamataEngine::Vector3& position);

	void Update();

	void Draw();

private:
	// ワールド変換データ
	KamataEngine::WorldTransform worldTransform_;

	// 座標
	KamataEngine::Vector3 velocity_{};
	// 加速/減速/最大速度
	static inline const float kAcceleration = 0.05f;
	static inline const float kAtteleration = 0.025f;
	static inline const float kLimitRunSpeed = 0.08f;

	// 左右向き/角度/タイマー/旋回時間
	LRDirection lrDirection_ = LRDirection::kRight;
	float turnFirstRotationY_ = 0.0f;
	float turnTimer_ = 0.0f;
	// 旋回時間
	static inline const float kTimeTurn = 1.0f;

	// 着地状態フラグ
	bool onGround_ = true;

	// 重力加速度/最大落下速度/ジャンプ初速
	static inline const float kGravityAcceleration = 0.025f;
	static inline const float kLimitFallSpeed = 0.075f;
	static inline const float kJumpAcceleration = 0.5f;

	// モデル
	KamataEngine::Model* model_ = nullptr;

	// カメラ
	KamataEngine::Camera* camera_;
};
