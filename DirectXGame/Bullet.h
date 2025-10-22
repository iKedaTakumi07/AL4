#pragma once
#include "KamataEngine.h"
#include "Math.h"

using namespace KamataEngine;

class Player;
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

	void Initialize(Model* model, Camera* camera, const Vector3& position);

	void Update();

	void Draw();

	AABB GetAABB();

	Vector3 GetWorldPosition() const;

	void Setdirection(Vector3 direction) { kBulletdirection = direction; };

	void SetMapChipField(MapChipField* mapChipField) { mapChipFeild_ = mapChipField; };

	// void OnCollision(const Player* player);

	 bool isDead() const { return isDead_; };
	// bool IsCollisionDisabled() const { return isCollisionDisabled_; };

private:
	// トランスフォーム
	WorldTransform worldTransform_;

	struct CollisionMapInfo {
		bool ceiling = false;
		bool landing = false;
		bool hitWall = false;
		Vector3 move;
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
	Vector3 velocity_ = {};

	// 弾の速さ
	static inline const float kbulletSpeed = 0.1f;
	static inline const float kLimitRunSpeed = 0.15f;
	// 向き
	Vector3 kBulletdirection = {};

	// モデル
	Model* model_ = nullptr;

	// カメラ
	Camera* camera_ = nullptr;

	// フラグ
	bool isDead_ = false;

	// キャラクターの当たり判定サイズ
	static inline const float kWidth = 0.6f;
	static inline const float kHeight = 0.6f;
	/*bool isCollisionDisabled_ = false;*/

	// マップチップによるフィールド
	MapChipField* mapChipFeild_ = nullptr;
	// 指定した角の計算
	Vector3 CornerPosition(const Vector3& center, Corner Corner);
	static inline const float kBlank = 0.04f;
};
