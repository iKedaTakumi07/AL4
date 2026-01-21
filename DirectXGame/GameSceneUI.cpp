#include "GameSceneUI.h"
#include <numbers>
#include <cassert>

using namespace KamataEngine;

void GameSceneUI::Initialize(KamataEngine::Model* model, KamataEngine::Camera* camera) {
	// nullポインタチェック
	assert(model);

	modelHp_ = model;
	camera_ = camera;

	const float kScale = 0.5f;

	for (int i = 0; i < MaxPlayerHp_; i++) {
		WorldTransform* wt = new WorldTransform();
		wt->Initialize();
		wt->scale_ = {kScale, kScale, kScale};
		wt->rotation_.y = std::numbers::pi_v<float> / 2.0f;
		wt->translation_.x = i * 10.0f;
		wt->translation_.y = 20.0f;
		wt->translation_.z = -0.6f;
		worldTransforms_.push_back(wt);
	}
}

void GameSceneUI::Update() {

	for (int i = 0; i < PlayerHp_; i++) {
		worldTransforms_[i]->translation_.x = camera_->translation_.x + i * 1.0f - 10.0f;
		worldTransforms_[i]->translation_.y = camera_->translation_.y + 5.0f;

		WorldtransformUpdate(*worldTransforms_[i]);
	}
}

void GameSceneUI::Draw() {
	// 描画
	for (int i = 0; i < PlayerHp_; i++) {
		modelHp_->Draw(*worldTransforms_[i], *camera_);
	}
}