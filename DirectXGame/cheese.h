#pragma once
#include "Math.h"
#include <KamataEngine.h>

using namespace KamataEngine;

class Player;
class GameScene;
class MapChipField;

class cheese {
public:
	// スターコイン的なもの

	enum class Behavior {
		kUnknown = -1,
		kExist,
		kDefeated,
	};

	void Initialize(Model* model, Camera* camera, const Vector3& position);

	void Update();

	void Draw();

	AABB GetAABB();

	Vector3 GetWorldPosition();

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
};
