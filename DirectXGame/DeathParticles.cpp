#include "DeathParticles.h"
#include "Math.h"

using namespace KamataEngine;

void DeathParticles::Initialize(Model* model, Camera* camera, const Vector3& position) {
	// 初期化
	model_ = model;
	camera_ = camera;

	for (WorldTransform& worldTransform : worldTransform_) {
		worldTransform.Initialize();
		worldTransform.translation_ = position;
	}
}

void DeathParticles::Update() {
	// ワールド座標の更新
	for (WorldTransform& worldTransform : worldTransform_) {
		WolrdtransformUpdate(worldTransform);
	}
}

void DeathParticles::Draw() {
	for (WorldTransform& worldTransform : worldTransform_) {
		model_->Draw(worldTransform, *camera_);
	}
}