#pragma once
#include "KamataEngine.h"
#include "Math.h"

using namespace KamataEngine;

class Player;

class Bullet {
public:
	enum Corner {
		kRightBottom,
		kLeftBottom,
		kRightTop,
		KLeftTop,

		KNumCorner, // 要素数
	};

	// 左右
	enum class LRDirection {
		kRight,
		kleft,
	};

	void Initialize(Model* model, Camera* camera, const Vector3& position);

	void Update();

	void Draw();

	AABB GetAABB();

	Vector3 GetWorldPosition() const;

	/*Vector3 Setdirection(Player* direction) {};*/

	// void OnCollision(const Player* player);
	// bool isDead() const { return isDead_; };
	// bool IsCollisionDisabled() const { return isCollisionDisabled_; };

private:
	// トランスフォーム
	WorldTransform worldTransform_;

	// 速度
	Vector3 velocity_ = {};
	// 弾の速さ
	static inline const float kbulletSpeed = 0.3f;
	// 向き
	Vector3 kBulletdirection = {};

	// モデル
	Model* model_ = nullptr;

	// カメラ
	Camera* camera_ = nullptr;

	// フラグ
	bool isDead_ = false;

	// キャラクターの当たり判定サイズ
	static inline const float kWidth = 0.8f;
	static inline const float kHeight = 0.8f;
	/*bool isCollisionDisabled_ = false;*/
};
