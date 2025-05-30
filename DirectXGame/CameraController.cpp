#define NOMINMAX

#include "CameraController.h"
#include "Math.h"
#include "Player.h"

using namespace KamataEngine;

void CameraController::Initialize(KamataEngine::Camera* camera) {

	// 初期化
	camera_ = camera;
}

void CameraController::Update() {

	// 追従対象のワールドトランスフォームの参照
	const WorldTransform& targetWorldTransform = target_->GetWorldTransform();

	const Vector3& targetVelocity = target_->GetVelocity();

	// 追従対象とオフセットからカメラの座標を計算
	controllePos.x = targetWorldTransform.translation_.x + targetOffset_.x + targetVelocity.x * kVelocityBias;
	controllePos.y = targetWorldTransform.translation_.y + targetOffset_.y + targetVelocity.y * kVelocityBias;
	controllePos.z = targetWorldTransform.translation_.z + targetOffset_.z + targetVelocity.z * kVelocityBias;

	camera_->translation_ = Lerp(camera_->translation_, controllePos, kInterpolationRate);

	// 移動範囲制限(補正)
	camera_->translation_.x = std::max(camera_->translation_.x, controllePos.x + targetMargi.left);
	camera_->translation_.x = std::min(camera_->translation_.x, controllePos.x + targetMargi.right);
	camera_->translation_.y = std::min(camera_->translation_.y, controllePos.y + targetMargi.bottom);
	camera_->translation_.y = std::max(camera_->translation_.y, controllePos.y + targetMargi.top);

	// 移動範囲制限
	camera_->translation_.x = std::max(camera_->translation_.x, movableArea_.left);
	camera_->translation_.x = std::min(camera_->translation_.x, movableArea_.right);
	camera_->translation_.y = std::min(camera_->translation_.y, movableArea_.bottom);
	camera_->translation_.y = std::max(camera_->translation_.y, movableArea_.top);

	// 行列を更新
	camera_->UpdateMatrix();
}

void CameraController::Reset() {
	// 追従対象のワールドトランスフォームの参照
	const WorldTransform& targetWorldTransform = target_->GetWorldTransform();
	// 追従対象とオフセットからカメラの座標を計算
	camera_->translation_.x = targetWorldTransform.translation_.x + targetOffset_.x;
	camera_->translation_.y = targetWorldTransform.translation_.y + targetOffset_.y;
	camera_->translation_.z = targetWorldTransform.translation_.z + targetOffset_.z;
}
