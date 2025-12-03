#pragma once
#include "KamataEngine.h"
#include "Math.h"

class Enemy;
class MapChipField;

class Bullet {
public:
	// 左右
	enum Corner {
		kRightBottom,
		kLeftBottom,
		kRightTop,
		KLeftTop,

		KNumCorner, // 要素数
	};

	void Initialize(KamataEngine::Model* model, KamataEngine::Camera* camera, const KamataEngine::Vector3& position, float chage);

	void Update();

	void Draw();

	AABB GetAABB();
	
	KamataEngine::Vector3 GetWorldPosition() const;

	void Setdirection(KamataEngine::Vector3 direction) { kBulletdirection = direction; };

	void SetMapChipField(MapChipField* mapChipField) { mapChipFeild_ = mapChipField; };

	void OnCollision();

	bool IsDead() const { return isDead_; };
	// bool IsCollisionDisabled() const { return isCollisionDisabled_; };

	float isGetLevel() { return chargeLevel_; };

private:
	// トランスフォーム
	KamataEngine::WorldTransform worldTransform_;

	struct CollisionMapInfo {
		bool ceiling = false;
		bool landing = false;
		bool hitWall = false;
		KamataEngine::Vector3 move;
	};

	// 当たり判定
	void CheckMapCollision(CollisionMapInfo& info);

	// 全方向
	void CheckMapCollisionUP(CollisionMapInfo& info);
	void CheckMapCollisionDown(CollisionMapInfo& info);
	void CheckMapCollisionRight(CollisionMapInfo& info);
	void CheckMapCollisionLeft(CollisionMapInfo& info);

	void isHit(const CollisionMapInfo& info);

	// 速度
	KamataEngine::Vector3 velocity_ = {};

	// 弾の速さ
	static inline const float kbulletSpeed = 0.15f;
	static inline const float kLimitRunSpeed = 0.3f;
	// 向き
	KamataEngine::Vector3 kBulletdirection = {};

	// モデル
	KamataEngine::Model* model_ = nullptr;

	// カメラ
	KamataEngine::Camera* camera_ = nullptr;

	// フラグ
	bool isDead_ = false;

	// キャラクターの当たり判定サイズ
	static inline const float kWidth = 0.6f;
	static inline const float kHeight = 0.6f;
	/*bool isCollisionDisabled_ = false;*/

	// マップチップによるフィールド
	MapChipField* mapChipFeild_ = nullptr;
	// 指定した角の計算
	KamataEngine::Vector3 CornerPosition(const KamataEngine::Vector3& center, Corner Corner);
	static inline const float kBlank = 0.04f;

	// チャージショット系列
	float chargeLevel_;
};
