#include "DeathParticles.h"

using namespace KamataEngine;

void DeathParticles::Initialize(Model* model, Camera* camera, const Vector3& position) {
	// 初期化
	model_ = model;
	camera_ = camera;

	for (auto& worldTransform : worldTransform_) {
		worldTransform.Initialize();
		worldTransform.translation_ = position;
	}
}

void DeathParticles::Update() {
	// ワールド座標の更新
	for (auto& worldTransform : worldTransform_) {
		WolrdtransformUpdate(worldTransform);
	}
}

void DeathParticles::Draw() {
	for (auto& worldTransform : worldTransform_) {
		model_->Draw(worldTransform, *camera_);
	}
}