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

	/*std::random_device seedGenerator;*/
	std::mt19937_64 randomEngine;
	std::uniform_real_distribution<float> rotationDistribution(-std::numbers::pi_v<float>, std::numbers::pi_v<float>);

	for (WorldTransform& worldTransform : ellipseWorldTransforms_) {
		worldTransform.rotation_ = {0.0f, 0.0f, rotationDistribution(randomEngine)};
		worldTransform.translation_ = position;
		worldTransform.translation_.z = -1.0f;
		worldTransform.Initialize();
	}
}

void HitEffect::Update() {
	if (IsDead()) {
		return; // 既に消滅している場合は更新しない
	}

	switch (state_) {
	case State::kSpread: {
		++counter_;
		float scale = 0.5f + static_cast<float>(counter_) / kSpreadTime * 0.5f;
		const float slashScale = 2.0f;
		for (auto& slashWorldTransform : ellipseWorldTransforms_) {
			slashWorldTransform.scale_ = {0.1f, scale * slashScale, 1.0f};
		}

		const float circleScale = 1.0;

		circleWorldTransform_.scale_ = {scale * circleScale, scale * circleScale, 1.0f};

		if (counter_ >= kSpreadTime) {
			state_ = State::kFade;
			counter_ = 0; // カウンターをリセット
		}
		break;
	}
	case State::kFade: {
		++counter_;
		objectColor_.SetColor(Vector4{1.0f, 1.0f, 1.0f, 1.0f - static_cast<float>(counter_) / kFadeTime});

		if (++counter_ >= kFadeTime) {
			state_ = State::kDead;
		}

		break;
	}
	default:
		break;
	}

	for (auto& slashWorldTransform : ellipseWorldTransforms_) {
		WorldtransformUpdate(slashWorldTransform);
	}

	WorldtransformUpdate(circleWorldTransform_);
}

void HitEffect::Draw() {
	assert(model_);
	assert(camera_);

	model_->Draw(circleWorldTransform_, *camera_, &objectColor_);

	for (auto& slashWorldTransform : ellipseWorldTransforms_) {
		model_->Draw(slashWorldTransform, *camera_, &objectColor_);
	}
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
