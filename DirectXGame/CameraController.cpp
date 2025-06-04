#include "CameraController.h"
#include "Math.h"
#include "player.h"
#include <imgui.h>

using namespace KamataEngine;

void CameraController::Initialize(Camera* camera) { camera_ = camera; }

void CameraController::Update() {

	// 追跡委対象の座標を参照
	const WorldTransform& targetWorldTransForm = target_->GetWorldTransform();

	// 対象者のカメラ座標を計算
	camera_->translation_.x = targetWorldTransForm.translation_.x + targetOffset_.x;
	camera_->translation_.y = targetWorldTransForm.translation_.y + targetOffset_.y;
	camera_->translation_.z = targetWorldTransForm.translation_.z + targetOffset_.z;

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