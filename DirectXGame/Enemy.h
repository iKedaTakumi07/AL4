#pragma once

#include "Math.h"
#include <KamataEngine.h>

using namespace KamataEngine;

class Player;
class GameScene;
class MapChipField;

class Enemy {
public:
	enum class Behavior {
		kUnknown = -1,
		kWalk,
		kDefeated,
	};

	// 左右
	enum class LRDirection {
		kRight,
		kleft,
	};

	enum Corner {
		kRightBottom,
		kLeftBottom,
		kRightTop,
		KLeftTop,

		KNumCorner, // 要素数
	};

	void Initialize(Model* model, Camera* camera, const Vector3& position);

	void Update();

	void Draw();

	AABB GetAABB();

	Vector3 GetWorldPosition();

	void OnCollision(const Player* player);

	bool isDead() const { return isDead_; }

	bool IsCollisionDisabled() const { return isCollisionDisabled_; };

	void SetMapChipField(MapChipField* mapChipField) { mapChipFeild_ = mapChipField; };

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

	struct CollisionMapInfo {
		bool ceiling = false;
		bool landing = false;
		bool hitWall = false;
		Vector3 move;
	};

	void CheckMapCollision(CollisionMapInfo& info);

	// 指定した角の計算
	Vector3 CornerPosition(const Vector3& center, Corner Corner);

	// 全方向
	void CheckMapCollisionUP(CollisionMapInfo& info);
	void CheckMapCollisionDown(CollisionMapInfo& info);
	void CheckMapCollisionRight(CollisionMapInfo& info);
	void CheckMapCollisionLeft(CollisionMapInfo& info);

	// マップチップによるフィールド
	MapChipField* mapChipFeild_ = nullptr;

	static inline const float kBlank = 0.04f;

	// 着地時の速度軽減率
	static inline const float kAttenuationLanding = 0.1f;
	// 微小な数値
	static inline const float kGroundSearchHeight = 0.06f;
	// 速度減衰率
	static inline const float kAttenuationWall = 0.2f;

	// 着地状態フラグ
	bool onGround_ = true;

	// 重力加速度/最大落下速度/ジャンプ初速
	static inline const float kGravityAcceleration = 0.98f;
	static inline const float kLimitFallSpeed = 0.5f;
	static inline const float kJumpAcceleration = 20.0f;

	// 衝突判定
	void UpdateOnWall(const CollisionMapInfo& info);
	// 設置状態の切り替え
	void UpDateOnGround(const CollisionMapInfo& info);

	// 左右向き/角度/タイマー/旋回時間
	LRDirection lrDirection_ = LRDirection::kleft;
	float turnFirstRotationY_ = 0.0f;
	float turnTimer_ = 0.0f;
	// 旋回時間
	static inline const float kTimeTurn = 0.5f;
};
