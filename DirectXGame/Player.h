#pragma once
#include "KamataEngine.h"
#include "Math.h"

using namespace KamataEngine;

class MapChipField;
class Enemy;

class Player {
public:
	// ふるまい
	enum class Behavior {
		kUnknown = -1,
		kRoot,
		kAttack,
	};

	// 攻撃モーション
	enum class AttackPhase {
		kUnknown = -1,

		kAnticipation,
		kAction,
		kRecovery,
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

	void Initialize(Model* model, Model* modelAttack, Camera* camera, const Vector3& position);

	void Update();

	void Draw();

	WorldTransform& GetWorldTransform() { return worldTransform_; };

	const Vector3& GetVelocity() const { return velocity_; };

	void SetMapChipField(MapChipField* mapChipField) { mapChipFeild_ = mapChipField; };

	Vector3 GetWorldPosition() const;

	AABB GetAABB();

	void OnCollision(const Enemy* enemy);

	// ですフラグのトリがー
	bool IsDead() const { return isDead_; };

	void BehaviorRootInitialize();

	void BehaviorRootUpdata();

	void BehaviorAttackInitialize();

	void BehaviorAttackUpdata();

	bool IsAttack() const { return behavior_ == Behavior::kAttack && attackPhase_ == AttackPhase::kAction; }

private:
	// ワールド変換データ
	WorldTransform worldTransform_;

	// 座標
	Vector3 velocity_{};
	// 加速/減速/最大速度
	static inline const float kAcceleration = 0.01f;
	static inline const float kAtteleration = 0.1f;
	static inline const float kLimitRunSpeed = 0.15f;

	// 左右向き/角度/タイマー/旋回時間
	LRDirection lrDirection_ = LRDirection::kRight;
	float turnFirstRotationY_ = 0.0f;
	float turnTimer_ = 0.0f;
	// 旋回時間
	static inline const float kTimeTurn = 0.5f;

	// 着地状態フラグ
	bool onGround_ = true;
	// 2段ジャンプ
	bool isSpaceJump = false;

	// 重力加速度/最大落下速度/ジャンプ初速/2段ジャンプ初速
	static inline const float kGravityAcceleration = 0.98f;
	static inline const float kLimitFallSpeed = 0.5f;
	static inline const float kJumpAcceleration = 20.0f;
	static inline const float kSpaceJumpAcceleration = 15.0f;

	// モデル
	Model* model_ = nullptr;
	Model* modelAttack_ = nullptr;

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

	void InputMove();

	// 当たり判定
	void CheckMapCollision(CollisionMapInfo& info);

	// 全方向
	void CheckMapCollisionUP(CollisionMapInfo& info);
	void CheckMapCollisionDown(CollisionMapInfo& info);
	void CheckMapCollisionRight(CollisionMapInfo& info);
	void CheckMapCollisionLeft(CollisionMapInfo& info);

	// 衝突判定
	void UpdateOnWall(const CollisionMapInfo& info);

	// 指定した角の計算
	Vector3 CornerPosition(const Vector3& center, Corner Corner);

	static inline const float kBlank = 0.04f;
	// 設置状態の切り替え
	void UpDateOnGround(const CollisionMapInfo& info);
	// 着地時の速度軽減率/微小な数値/速度減衰率
	static inline const float kAttenuationLanding = 0.9f;
	static inline const float kGroundSearchHeight = 0.06f;
	static inline const float kAttenuationWall = 0.2f;

	// ですフラグ
	bool isDead_ = false;

	// ふるまい
	Behavior behavior_ = Behavior::kRoot;

	// 次のふるまい
	Behavior behaviorRequest_ = Behavior::kUnknown;

	// 攻撃ギミックの経過時間カウンター
	uint32_t attackParameter_ = 0;

	// 攻撃モーション
	AttackPhase attackPhase_ = AttackPhase::kUnknown;

	// 予備動作の時間
	static inline const uint32_t KanticipationTime = 8;
	// 前進動作の時間
	static inline const uint32_t kActionTime = 5;
	// 余韻動作の時間
	static inline const uint32_t kRecoveryTime = 12;
	WorldTransform worldTransformAttack_;
};
