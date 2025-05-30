#include "CameraController.h"
#include "player.h"

using namespace KamataEngine;

void CameraController::Initialize(Camera* camera) { camera_ = camera; }

void CameraController::Update() {}

void CameraController::Reset() {
	// 追跡委対象の座標を参照
	const WorldTransform& targetWorldTransForm = target_.
}