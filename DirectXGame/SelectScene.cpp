#include "SelectScene.h"
#include "Math.h"
#include <numbers>

using namespace KamataEngine;
void SelectScene::Initialize() {

	models_.push_back(Model::CreateFromOBJ("number1")); // 操作説明
	models_.push_back(Model::CreateFromOBJ("number1")); // ステージ順に↓
	models_.push_back(Model::CreateFromOBJ("number2"));
	models_.push_back(Model::CreateFromOBJ("number3"));

	const float kScale = 10.0f;

	for (int i = 0; i < models_.size(); i++) {
		WorldTransform* wt = new WorldTransform();
		wt->Initialize();
		wt->scale_ = {kScale, kScale, kScale};
		wt->translation_.x = i * slideStepX_;
		wt->translation_.y = -2.0f;
		worldTransforms_.push_back(wt);
	}

	camera_.Initialize();

	fade_ = new Fade;
	fade_->Initialize();
	fade_->Start(Fade::Status::FadeIn, 1.0f);
}

void SelectScene::Update() {

	switch (phase_) {
	case SelectScene::Phase::kFadeIn:
		fade_->Update();

		if (fade_->IsFinished()) {
			phase_ = Phase::kMain;
		}
		break;
	case SelectScene::Phase::kMain:
		switch (Stage_) {
		case ksousa:
			if (!slideAnim_.isPlaying && Input::GetInstance()->PushKey(DIK_RIGHT)) {
				slideAnim_.isPlaying = true;
				slideAnim_.timer = 0.0f;
				slideAnim_.startOffsetX = slideOffsetX_;
				slideAnim_.endOffsetX = slideOffsetX_ - slideStepX_;
				Stage_ = k1_1;
			}
			if (Input::GetInstance()->PushKey(DIK_SPACE)) {
				stageNumber_ = 0;
				fade_->Start(Fade::Status::FadeOut, 1.0f);
				phase_ = Phase::kFadeOut;
			}
			break;
		case k1_1:
			if (!slideAnim_.isPlaying && Input::GetInstance()->PushKey(DIK_LEFT)) {
				slideAnim_.isPlaying = true;
				slideAnim_.timer = 0.0f;
				slideAnim_.startOffsetX = slideOffsetX_;
				slideAnim_.endOffsetX = slideOffsetX_ + slideStepX_;
				Stage_ = ksousa;
			}
			if (Input::GetInstance()->PushKey(DIK_SPACE)) {
				stageNumber_ = 1;
				fade_->Start(Fade::Status::FadeOut, 1.0f);
				phase_ = Phase::kFadeOut;
			}
			if (!slideAnim_.isPlaying && Input::GetInstance()->PushKey(DIK_RIGHT)) {
				slideAnim_.isPlaying = true;
				slideAnim_.timer = 0.0f;
				slideAnim_.startOffsetX = slideOffsetX_;
				slideAnim_.endOffsetX = slideOffsetX_ - slideStepX_;
				Stage_ = k1_2;
			}
			break;
		case k1_2:
			if (!slideAnim_.isPlaying && Input::GetInstance()->PushKey(DIK_LEFT)) {
				slideAnim_.isPlaying = true;
				slideAnim_.timer = 0.0f;
				slideAnim_.startOffsetX = slideOffsetX_;
				slideAnim_.endOffsetX = slideOffsetX_ + slideStepX_;
				Stage_ = k1_1;
			}
			if (!slideAnim_.isPlaying && Input::GetInstance()->PushKey(DIK_RIGHT)) {
				slideAnim_.isPlaying = true;
				slideAnim_.timer = 0.0f;
				slideAnim_.startOffsetX = slideOffsetX_;
				slideAnim_.endOffsetX = slideOffsetX_ - slideStepX_;
				Stage_ = k1_3;
			}
			if (Input::GetInstance()->PushKey(DIK_SPACE)) {
				stageNumber_ = 2;
				fade_->Start(Fade::Status::FadeOut, 1.0f);
				phase_ = Phase::kFadeOut;
			}
			break;
		case k1_3:
			if (!slideAnim_.isPlaying && Input::GetInstance()->PushKey(DIK_LEFT)) {
				slideAnim_.isPlaying = true;
				slideAnim_.timer = 0.0f;
				slideAnim_.startOffsetX = slideOffsetX_;
				slideAnim_.endOffsetX = slideOffsetX_ + slideStepX_;
				Stage_ = k1_2;
			}
			if (Input::GetInstance()->PushKey(DIK_SPACE)) {
				stageNumber_ = 3;
				fade_->Start(Fade::Status::FadeOut, 1.0f);
				phase_ = Phase::kFadeOut;
			}
			break;
		}

		// スライドアニメーション更新
		if (slideAnim_.isPlaying) {
			slideAnim_.timer += 1.0f / 60.0f;
			float t = slideAnim_.timer / slideAnim_.duration;
			if (t >= 1.0f) {
				t = 1.0f;
				slideAnim_.isPlaying = false;
			}
			float eased = EaseInQuint(t);
			slideOffsetX_ = slideAnim_.startOffsetX + (slideAnim_.endOffsetX - slideAnim_.startOffsetX) * eased;
		}
		break;
	case SelectScene::Phase::kFadeOut:
		fade_->Update();
		if (fade_->IsFinished()) {
			finished_ = true;
		}

		break;
	}

	// モデル位置更新
	for (int i = 0; i < models_.size(); i++) {
		worldTransforms_[i]->translation_.x = (i * slideStepX_) + slideOffsetX_;
		WorldtransformUpdate(*worldTransforms_[i]);
	}

	camera_.TransferMatrix();
}

void SelectScene::Draw() {
	// directXCommonインスタンスの取得
	DirectXCommon* dxCommon = DirectXCommon::GetInstance();

	Model::PreDraw(dxCommon->GetCommandList());

	// 描画
	for (int i = 0; i < models_.size(); i++) {

		models_[i]->Draw(*worldTransforms_[i], camera_);
	}
	fade_->Draw();

	Model::PostDraw();
}

SelectScene::~SelectScene() {
	//
	for (auto* m : models_)
		delete m;
	for (auto* w : worldTransforms_)
		delete w;

	delete fade_;
}
