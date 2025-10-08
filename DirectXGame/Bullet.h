#pragma once
#include "KamataEngine.h"
#include "Math.h"

using namespace KamataEngine;

class Bullet {
public:
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

	// void OnCollision(const Player* player);
	// bool isDead() const { return isDead_; };
	// bool IsCollisionDisabled() const { return isCollisionDisabled_; };

private:
	// トランスフォーム
	WorldTransform worldTransform_;

	// 弾の速さ
	static inline const float kbulletSpeed = 0.3f;
	// 速度
	Vector3 velocity_ = {};

	// モデル
	Model* model_ = nullptr;

	// カメラ
	Camera* camera_ = nullptr;

	// フラグ
	bool isDead_ = false;
	/*bool isCollisionDisabled_ = false;*/
};
