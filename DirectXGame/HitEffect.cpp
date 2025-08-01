#include "HitEffect.h"
#include <cassert>
#include <math.h>
#include <numbers>
#include <random>

using namespace KamataEngine;

Model* HitEffect::model_ = nullptr;
Camera* HitEffect::camera_ = nullptr;

void HitEffect::Initialize(const Vector3& position) {

	// 円形エフェクト
	circleWorldTransform_.translation_ = position;
	circleWorldTransform_.Initialize();
	circleWorldTransform_.translation_.z = -1.0f;
	objectColor_.Initialize();
}

void HitEffect::Update() { WorldtransformUpdate(circleWorldTransform_); }

void HitEffect::Draw() {
	assert(model_);
	assert(camera_);

	model_->Draw(circleWorldTransform_, *camera_, &objectColor_);
}

HitEffect* HitEffect::Create(const Vector3& position) {

	// インスタンス生成
	HitEffect* instance = new HitEffect();

	// newの失敗を検出
	assert(instance);

	// インスタンスの初期化
	instance->Initialize(position);

	// return
	return instance;
}
