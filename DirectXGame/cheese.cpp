#include "cheese.h"
#include "GameScene.h"
#include "MapChipField.h"
#include "Math.h"
#include "Player.h"
#include <cassert>
#include <numbers>

void cheese::Initialize(KamataEngine::Model* model, KamataEngine::Camera* camera, const Vector3& position) {

	// nullポインタチェック
	assert(model);

	// 初期化
	model_ = model;
	camera_ = camera;

	// ワールド変換の初期アk
	worldTransform_.Initialize();
	worldTransform_.translation_ = position;
	worldTransform_.rotation_.y = std::numbers::pi_v<float> * 3.0f / 2.0f;
}

Vector3 cheese::GetWorldPosition() {
	// ワールド座標を取得
	Vector3 WorldPos;
	WorldPos.x = worldTransform_.matWorld_.m[3][0];
	WorldPos.y = worldTransform_.matWorld_.m[3][1];
	WorldPos.z = worldTransform_.matWorld_.m[3][2];

	return WorldPos;
}

void cheese::Update() {

	if (behaviorRequest_ != Behavior::kUnknown) {
		behavior_ = behaviorRequest_;

		switch (behavior_) {
		case cheese::Behavior::kDefeated:
		default:
			counter_ = 0;
			break;
		}
		behaviorRequest_ = Behavior::kUnknown;
	}

	switch (behavior_) {
	case cheese::Behavior::kExist:

		// 回転
		worldTransform_.rotation_.y += 0.15f;

		// 更新
		WorldtransformUpdate(worldTransform_);
		break;
	case cheese::Behavior::kDefeated:

		counter_ += 1.0f / 60.0f;

		worldTransform_.rotation_.y += 0.5f;
		worldTransform_.translation_.y += 0.03f;

		WorldtransformUpdate(worldTransform_);
		if (counter_ >= kDefeatedTime) {
			isDead_ = true;
		}

		break;
	}
}

void cheese::Draw() {
	// 描画
	if (!isDead_) {
		model_->Draw(worldTransform_, *camera_);
	}
}

AABB cheese::GetAABB() {

	Vector3 worldPos = GetWorldPosition();

	AABB aabb;
	aabb.min = {worldPos.x - kWidth / 2.0f, worldPos.y - kHeight / 2.0f, worldPos.z - kWidth / 2.0f};
	aabb.max = {worldPos.x + kWidth / 2.0f, worldPos.y + kHeight / 2.0f, worldPos.z + kWidth / 2.0f};

	return aabb;
}
void cheese::OnCollision() {

	// ないなら処理しない
	if (behavior_ == Behavior::kDefeated) {
		return;
	}

	// 触った判定に
	behaviorRequest_ = Behavior::kDefeated;
	isCollisionDisabled_ = true;
}
