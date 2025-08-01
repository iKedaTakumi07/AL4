#pragma once
#include "Math.h"
#include <KamataEngine.h>

using namespace KamataEngine;

class Player;
class GameScene;

class Enemy {
public:
	enum class Behavior {
		kUnknown = -1,
		kWalk,
		kDefeated,
	};

	void Initialize(Model* model, Camera* camera, const Vector3& position);

	void Update();

	void Draw();

	AABB GetAABB();

	Vector3 GetWorldPosition();

	void OnCollision(const Player* player);

	bool isDead() const { return isDead_; }

	bool IsCollisionDisabled() const { return isCollisionDisabled_; };

	void SetGameScene(GameScene* gameScene) { gameScene_ = gameScene; }

private:
	// トランスフォーム
	WorldTransform worldTransform_;

	// モデル
	Model* model_ = nullptr;

	// カメラ
	Camera* camera_ = nullptr;

	// 歩きの速さ
	static inline const float kwalkSpeed = 0.02f;
	// 速度
	Vector3 velocity_ = {};

	// 最初の角度
	static inline const float kWalkMotionAngleStart = 0.0f;
	// 最後の角度
	static inline const float kWalkMotionAngleEnd = 30.0f;
	// アニメーション周期
	static inline const float kWalkMotionTime = 1.0f;
	// 経過時間
	float walkTimer_ = 0.0f;

	static inline const float kWidth = 0.8f;
	static inline const float kHeight = 0.8f;

	// フラグ
	bool isDead_ = false;

	Behavior behavior_ = Behavior::kWalk;
	Behavior behaviorRequest_ = Behavior::kUnknown;

	static inline const float kDefeatedTime = 0.6f;
	static inline const float kDefeatedMotionAngleStaart = 0.0f;
	static inline const float kDefeatedMotionAngleEnd = -60.0f;
	float counter_ = 0.0f;

	bool isCollisionDisabled_ = false;

	GameScene* gameScene_ = nullptr;
};
