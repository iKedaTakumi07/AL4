#include "Fade.h"
#include <algorithm>
void Fade::Initialize() {
	// スプライトを持たせる
	sprite_ = Sprite::Create(0, Vector2{});
	sprite_->SetSize(Vector2(WinApp::kWindowWidth, WinApp::kWindowHeight));
	sprite_->SetColor(Vector4(0, 0, 0, 1));
}

void Fade::Update() {
	switch (status_) {
	case Status::None:
		break;
	case Status::FadeIn:
		// 1フレーム分の秒数をカウントアップ
		counter_ += 1.0f / 60.0f;
		// フェード持続時間に達したら打ち止め
		if (counter_ >= duration_) {
			counter_ = duration_;
		}
		// 0.0fから1.0fのの間で、経過時間がフェード継続時間に近づくほどアルファ値を大きくする
		sprite_->SetColor(Vector4(0, 0, 0, std::clamp(1.0f - counter_ / duration_, 0.0f, 1.0f)));

		break;
	case Status::FadeOut:
		// 1フレーム分の秒数をカウントアップ
		counter_ += 1.0f / 60.0f;
		// フェード持続時間に達したら打ち止め
		if (counter_ >= duration_) {
			counter_ = duration_;
		}
		// 0.0fから1.0fのの間で、経過時間がフェード継続時間に近づくほどアルファ値を大きくする
		sprite_->SetColor(Vector4(0, 0, 0, std::clamp(counter_ / duration_, 0.0f, 1.0f)));
		break;
	}
}

void Fade::Draw() {

	if (status_ == Status::None) {
		return;
	}

	Sprite::PreDraw(DirectXCommon::GetInstance()->GetCommandList());
	sprite_->Draw();
	Sprite::PostDraw();
}

void Fade::Start(Status status, float duration) {
	status_ = status;
	duration_ = duration;
	counter_ = 0.0f;
}

void Fade::Stop() { status_ = Status::None; }

bool Fade::IsFinished() const {

	// フェード状態に夜分岐
	switch (status_) {
	case Fade::Status::FadeIn:
	case Fade::Status::FadeOut:

		if (counter_ >= duration_) {
			return true;
		} else {
			return false;
		}
	}
	return true;
}
