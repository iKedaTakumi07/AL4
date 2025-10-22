#include "Bullet.h"
#include <algorithm>
#include <numbers>

using namespace KamataEngine;

void Bullet::Initialize(Model* model, Camera* camera, const Vector3& position) {
	// nullポインタチェック
	assert(model);

	// 初期化
	model_ = model;
	camera_ = camera;

	// ワールド変換の初期アk
	worldTransform_.Initialize();
	worldTransform_.translation_ = position;
	worldTransform_.rotation_.y = std::numbers::pi_v<float> / 2.0f;
}

void Bullet::Update() {

	// 左右加速
	Vector3 acceleration = {};
	if (kBulletdirection.y <= 2.5f) {
		acceleration.x += kbulletSpeed;
	} else {
		acceleration.x += kbulletSpeed * -1.0f;
	}

	// 加速/減速
	velocity_.x += acceleration.x;

	// 最大速度制限
	velocity_.x = std::clamp(velocity_.x, -kLimitRunSpeed, kLimitRunSpeed);

	// 衝突情報を初期化
	CollisionMapInfo collisionMapInfo;
	// 移動量に速度の値をコピー
	collisionMapInfo.move = velocity_;
	collisionMapInfo.landing = false;
	collisionMapInfo.hitWall = false;

	// 衝突チェック
	/*CheckMapCollision(collisionMapInfo);*/

	worldTransform_.translation_ += collisionMapInfo.move;

	// 行列を定数バッファに転送
	WorldtransformUpdate(worldTransform_);
}

void Bullet::Draw() { model_->Draw(worldTransform_, *camera_); }

AABB Bullet::GetAABB() {

	Vector3 worldPos = GetWorldPosition();

	AABB aabb;
	aabb.min = {worldPos.x - kWidth / 2.0f, worldPos.y - kHeight / 2.0f, worldPos.z - kWidth / 2.0f};
	aabb.max = {worldPos.x + kWidth / 2.0f, worldPos.y + kHeight / 2.0f, worldPos.z + kWidth / 2.0f};

	return aabb;
}

Vector3 Bullet::GetWorldPosition() const {
	// ワールド座標を取得
	Vector3 WorldPos;
	WorldPos.x = worldTransform_.translation_.x;
	WorldPos.y = worldTransform_.translation_.y;
	WorldPos.z = worldTransform_.translation_.z;

	return WorldPos;
}
