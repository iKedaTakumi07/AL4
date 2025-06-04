#define NOMINMAX

#include "CameraController.h"
#include "Math.h"
#include "player.h"
#include <algorithm>

using namespace KamataEngine;

void CameraController::Initialize(Camera* camera) { camera_ = camera; }

void CameraController::Update() {

	// 追跡委対象の座標を参照
	const WorldTransform& targetWorldTransForm = target_->GetWorldTransform();

	const Vector3& targetVelocity = target_->GetVelocity();

	// 目標座標を計算
	destination_.x = targetWorldTransForm.translation_.x + targetOffset_.x + targetVelocity.x * KVelocityBias;
	destination_.y = targetWorldTransForm.translation_.y + targetOffset_.y + targetVelocity.y * KVelocityBias;
	destination_.z = targetWorldTransForm.translation_.z + targetOffset_.z + targetVelocity.z * KVelocityBias;

	// 対象者のカメラ座標を計算
	camera_->translation_ = Lerp(camera_->translation_, destination_, KinterpolationRate);

	// 移動範囲制限
	camera_->translation_.x = std::max(camera_->translation_.x, movableArea_.left);
	camera_->translation_.x = std::min(camera_->translation_.x, movableArea_.right);
	camera_->translation_.y = std::min(camera_->translation_.y, movableArea_.bottom);
	camera_->translation_.y = std::max(camera_->translation_.y, movableArea_.top);

	// 行列を変更する
	camera_->UpdateMatrix();
}

void CameraController::Reset() {
	// 追跡委対象の座標を参照
	const WorldTransform& targetWorldTransForm = target_->GetWorldTransform();

	// 対象者のカメラ座標を計算
	camera_->translation_.x = targetWorldTransForm.translation_.x + targetOffset_.x;
	camera_->translation_.y = targetWorldTransForm.translation_.y + targetOffset_.y;
	camera_->translation_.z = targetWorldTransForm.translation_.z + targetOffset_.z;
}