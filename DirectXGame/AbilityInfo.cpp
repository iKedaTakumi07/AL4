#include "AbilityInfo.h"

void AbilityInfo::Initialize(uint32_t textureHandle, const KamataEngine::Vector2& position) {

	this->textureHandle_ = textureHandle;
	pos = position;

	sprite_ = KamataEngine::Sprite::Create(this->textureHandle_, position);
}

void AbilityInfo::Update() {}

void AbilityInfo::Draw() {
	// スプライトを描画
	sprite_->Draw();
}

void AbilityInfo::SetTextureHandle(uint32_t textureHandle) {
	// セット
	sprite_->SetTextureHandle(textureHandle);
	textureHandle_ = textureHandle;
}
