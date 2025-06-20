#include "Enemy.h"
#include "Math.h"
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
}

void Enemy::Update() {
	// 更新
	WolrdtransformUpdate(worldTransform_);
}

void Enemy::Draw() {
	// 描画
	model_->Draw(worldTransform_, *camera_);
}