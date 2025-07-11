#include "Fade.h"
void Fade::Initialize() {
	// スプライトを持たせる
	sprite_ = Sprite::Create(0, Vector2{});
	sprite_->SetSize(Vector2(WinApp::kWindowWidth, WinApp::kWindowHeight));
	sprite_->SetColor(Vector4(0, 0, 0, 1));
}

void Fade::Update() {}

void Fade::Draw() {

	Sprite::PreDraw(DirectXCommon::GetInstance()->GetCommandList());
	sprite_->Draw();
	Sprite::PostDraw();
}