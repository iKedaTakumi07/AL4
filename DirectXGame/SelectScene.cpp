#include "SelectScene.h"
#include "Math.h"
#include <numbers>
using namespace KamataEngine;
void SelectScene::Initialize() {

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
		case SelectScene::ksousa:
			if (Input::GetInstance()->PushKey(DIK_D)) {
				Stage_ = SelectScene::k1_1;
			}
			break;
		case SelectScene::k1_1:
			if (Input::GetInstance()->PushKey(DIK_A)) {
				Stage_ = SelectScene::ksousa;
			}

			if (Input::GetInstance()->PushKey(DIK_SPACE)) {
				stageNumber_ = 0;
				fade_->Start(Fade::Status::FadeOut, 1.0f);
				phase_ = Phase::kFadeOut;
			}
			if (Input::GetInstance()->PushKey(DIK_D)) {
				Stage_ = SelectScene::k1_2;
			}

			break;
		case SelectScene::k1_2:

			if (Input::GetInstance()->PushKey(DIK_A)) {
				Stage_ = SelectScene::k1_1;
			}
			if (Input::GetInstance()->PushKey(DIK_SPACE)) {
				stageNumber_ = 1;
				fade_->Start(Fade::Status::FadeOut, 1.0f);
				phase_ = Phase::kFadeOut;
			}
			break;
		}

		break;
	case SelectScene::Phase::kFadeOut:
		fade_->Update();
		if (fade_->IsFinished()) {
			finished_ = true;
		}

		break;
	}

	camera_.TransferMatrix();
}

void SelectScene::Draw() {
	// directXCommonインスタンスの取得
	DirectXCommon* dxCommon = DirectXCommon::GetInstance();

	Model::PreDraw(dxCommon->GetCommandList());

	// 描画
	fade_->Draw();

	Model::PostDraw();
}

SelectScene::~SelectScene() {
	//
	delete fade_;
}
