#include "Bullet.h"
#include <numbers>

using namespace KamataEngine;

void Bullet::Initialize(Model* model, Camera* camera, const Vector3& position){
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

void Bullet::Update(){
    
	velocity_.x += kbulletSpeed;
	
	// 行列を定数バッファに転送
	WorldtransformUpdate(worldTransform_);

}

void Bullet::Draw(){
    model_->Draw(worldTransform_, *camera_);
}

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
