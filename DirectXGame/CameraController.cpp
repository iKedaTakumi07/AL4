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

	// 対象者のカメラ座標を計算
	camera_->translation_.x = targetWorldTransForm.translation_.x + targetOffset_.x;
	camera_->translation_.y = targetWorldTransForm.translation_.y + targetOffset_.y;
	camera_->translation_.z = targetWorldTransForm.translation_.z + targetOffset_.z;

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