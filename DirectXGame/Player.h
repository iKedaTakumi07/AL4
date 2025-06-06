#pragma once
#include "KamataEngine.h"

using namespace KamataEngine;

class MapChipField;

class Player {
public:
	// 左右
	enum class LRDirection {
		kRight,
		kleft,
	};

	void Initialize(Model* model, Camera* camera, const Vector3& position);

	void Update();

	void Draw();

	WorldTransform& GetWorldTransform() { return worldTransform_; };

	const Vector3& GetVelocity() const { return velocity_; };

	void SetMapChipField(MapChipField* mapChipField) { mapChipFeild_ = mapChipField; };

private:
	// ワールド変換データ
	WorldTransform worldTransform_;

	// 座標
	Vector3 velocity_{};
	// 加速/減速/最大速度
	static inline const float kAcceleration = 0.01f;
	static inline const float kAtteleration = 0.05f;
	static inline const float kLimitRunSpeed = 0.3f;

	// 左右向き/角度/タイマー/旋回時間
	LRDirection lrDirection_ = LRDirection::kRight;
	float turnFirstRotationY_ = 0.0f;
	float turnTimer_ = 0.0f;
	// 旋回時間
	static inline const float kTimeTurn = 0.5f;

	// 着地状態フラグ
	bool onGround_ = true;

	// 重力加速度/最大落下速度/ジャンプ初速
	static inline const float kGravityAcceleration = 0.98f;
	static inline const float kLimitFallSpeed = 0.5f;
	static inline const float kJumpAcceleration = 20.0f;

	// モデル
	Model* model_ = nullptr;

	// カメラ
	Camera* camera_ = nullptr;

	// マップチップによるフィールド
	MapChipField* mapChipFeild_ = nullptr;

	// キャラクターの当たり判定サイズ
	static inline const float kWidth = 0.8f;
	static inline const float kHeight = 0.8f;

	struct CollisionMapInfo {
		bool ceiling = false;
		bool landing = false;
		bool hitWall = false;
		Vector3 move;
	};
};
