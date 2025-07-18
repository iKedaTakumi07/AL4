#define NOMINMAX

#include "Enemy.h"
#include "Math.h"
#include "Player.h"
#include <cassert>
#include <numbers>

void Enemy::Initialize(KamataEngine::Model* model, KamataEngine::Camera* camera, const Vector3& position) {

	// nullポインタチェック
	assert(model);

	// 初期化
	model_ = model;
	camera_ = camera;

	// ワールド変換の初期アk
	worldTransform_.Initialize();
	worldTransform_.translation_ = position;
	worldTransform_.rotation_.y = std::numbers::pi_v<float> * 3.0f / 2.0f;

	velocity_ = {-kwalkSpeed, 0, 0};

	walkTimer_ = 0.0f;
}

Vector3 Enemy::GetWorldPosition() {
	// ワールド座標を取得
	Vector3 WorldPos;
	WorldPos.x = worldTransform_.matWorld_.m[3][0];
	WorldPos.y = worldTransform_.matWorld_.m[3][1];
	WorldPos.z = worldTransform_.matWorld_.m[3][2];

	return WorldPos;
}

void Enemy::Update() {

	if (behaviorRequest_ != Behavior::kUnknown) {
		behavior_ = behaviorRequest_;

		switch (behavior_) {
		case Enemy::Behavior::kDefeated:
		default:
			counter_ = 0;
			break;
		}

		behaviorRequest_ = Behavior::kUnknown;
	}

	switch (behavior_) {
	case Enemy::Behavior::kWalk:

		// 移動
		worldTransform_.translation_ += velocity_;

		// タイマーの更新
		walkTimer_ += 1.0f / 60.0f;

		// 回転
		// float param = std::sin()
		worldTransform_.rotation_.x = std::sin(std::numbers::pi_v<float> * 2.0f * walkTimer_ / kWalkMotionTime);

		// 更新
		WorldtransformUpdate(worldTransform_);
		break;
	case Enemy::Behavior::kDefeated:
		counter_ += 1.0f / 60.0f;

		worldTransform_.rotation_.y += 0.3f;
		worldTransform_.rotation_.x = EaseOut(ToRadians(kDefeatedMotionAngleStaart), ToRadians(kDefeatedMotionAngleEnd), counter_ / kDefeatedTime);

		WorldtransformUpdate(worldTransform_);
		if (counter_ >= kDefeatedTime) {
			isDead_ = true;
		}

		break;
	}
}

void Enemy::Draw() {
	// 描画
	model_->Draw(worldTransform_, *camera_);
}

AABB Enemy::GetAABB() {

	Vector3 worldPos = GetWorldPosition();

	AABB aabb;
	aabb.min = {worldPos.x - kWidth / 2.0f, worldPos.y - kHeight / 2.0f, worldPos.z - kWidth / 2.0f};
	aabb.max = {worldPos.x + kWidth / 2.0f, worldPos.y + kHeight / 2.0f, worldPos.z + kWidth / 2.0f};

	return aabb;
}
void Enemy::OnCollision(const Player* player) {
	(void)player;
	//
	isDead_ = true;

	// ふふふ
	if (behavior_ == Behavior::kDefeated) {
		return;
	}

	if (player->IsAttack()) {
		behaviorRequest_ = Behavior::kDefeated;
	}
}
