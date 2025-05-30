#include "CameraControlloer.h"
#include "Player.h"

using namespace KamataEngine;

void CameraControlloer::Initialize(Camera* camera) {

	// 初期化
	camera_ = camera;
}

void CameraControlloer::Update() {
	// 追跡対象の座尾を参照
	const WorldTransform& targetWorldTransform = target_->GetWorldTransform();

	// 対象のオフセットから計算
	camera_->translation_.x = targetWorldTransform.translation_.x + targetOffset_.x;
	camera_->translation_.y = targetWorldTransform.translation_.y + targetOffset_.y;
	camera_->translation_.z = targetWorldTransform.translation_.z + targetOffset_.z;

	// 行列更新
	camera_->UpdateMatrix();
}

void CameraControlloer::Reset() {
	// 追跡対象の座尾を参照
	const WorldTransform& targetWorldTransform = target_->GetWorldTransform();

	// 対象のオフセットから計算
	camera_->translation_.x = targetWorldTransform.translation_.x + targetOffset_.x;
	camera_->translation_.y = targetWorldTransform.translation_.y + targetOffset_.y;
	camera_->translation_.z = targetWorldTransform.translation_.z + targetOffset_.z;
}