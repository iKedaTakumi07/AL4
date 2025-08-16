#include "CleraScene.h"
#include "Math.h"
#include <numbers>
using namespace KamataEngine;
void CleraScene::Initialize() {

	camera_.Initialize();

	fade_ = new Fade;
	fade_->Initialize();
	fade_->Start(Fade::Status::FadeIn, 1.0f);
}

void CleraScene::Update() {

	switch (phase_) {
	case CleraScene::Phase::kFadeIn:
		fade_->Update();

		if (fade_->IsFinished()) {
			phase_ = Phase::kMain;
		}
		break;
	case CleraScene::Phase::kMain:
		if (Input::GetInstance()->PushKey(DIK_SPACE)) {
			fade_->Start(Fade::Status::FadeOut, 1.0f);
			phase_ = Phase::kFadeOut;
		}
		break;
	case CleraScene::Phase::kFadeOut:
		fade_->Update();
		if (fade_->IsFinished()) {
			finished_ = true;
		}

		break;
	}

	camera_.TransferMatrix();
}

void CleraScene::Draw() {
	// directXCommonインスタンスの取得
	DirectXCommon* dxCommon = DirectXCommon::GetInstance();

	Model::PreDraw(dxCommon->GetCommandList());

	// 描画
	fade_->Draw();

	Model::PostDraw();
}

CleraScene::~CleraScene() {
	//
	delete fade_;
}
