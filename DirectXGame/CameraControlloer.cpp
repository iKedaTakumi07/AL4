#include "CameraControlloer.h"
#include "Player.h"

using namespace KamataEngine;

void CameraControlloer::Initialize() {

	// 初期化
	camera_->Initialize();
}

void CameraControlloer::Update() {}

void CameraControlloer::Reset() {
	// 追跡対象の座尾を参照
	const WorldTransform& targetWorldTransform = target_->GetWorldTransform();

	// 対象のオフセットから計算
	camera_->translation_.x = targetWorldTransform.translation_.x + targetOffset_.x;
	camera_->translation_.y = targetWorldTransform.translation_.y + targetOffset_.y;
	camera_->translation_.z = targetWorldTransform.translation_.z + targetOffset_.z;

}