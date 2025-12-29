#include "Goal.h"
#include "Player.h"
#include <cassert>

void Goal::Initialize(Model* model, Camera* camera, const Vector3& position) {
	// nullポインタチェック
	assert(model);

	// 初期化
	model_ = model;
	camera_ = camera;

	// ワールド変換の初期アk
	worldTransform_.Initialize();
	worldTransform_.translation_ = position;
}

AABB Goal::GetAABB() {

	Vector3 worldPos = GetWorldPosition();

	AABB aabb;
	aabb.min = {worldPos.x - kWidth / 2.0f, worldPos.y - kHeight / 2.0f, worldPos.z - kWidth / 2.0f};
	aabb.max = {worldPos.x + kWidth / 2.0f, worldPos.y + kHeight / 2.0f, worldPos.z + kWidth / 2.0f};

	return aabb;
}

Vector3 Goal::GetWorldPosition() const {
	// ワールド座標を取得
	Vector3 WorldPos;
	WorldPos.x = worldTransform_.matWorld_.m[3][0];
	WorldPos.y = worldTransform_.matWorld_.m[3][1];
	WorldPos.z = worldTransform_.matWorld_.m[3][2];

	return WorldPos;
}
void Goal::Update() {
	// 座標更新
	WorldtransformUpdate(worldTransform_);
}
void Goal::Draw() {

	// 描画
	model_->Draw(worldTransform_, *camera_);
}

void Goal::OnCollision(const Player* enemy) {
	isGoal_ = true;

	(void)enemy;
};