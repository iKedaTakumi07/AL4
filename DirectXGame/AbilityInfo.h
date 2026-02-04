#pragma once
#include "KamataEngine.h"

class AbilityInfo {
public:
	void Initialize(uint32_t textureHandle, const KamataEngine::Vector2& position);

	void Update();

	void Draw();

	// set
	void SetTextureHandle(uint32_t textureHandle) ;

private:
	KamataEngine::Sprite* sprite_ = nullptr;

	KamataEngine::Vector2 pos;
	uint32_t textureHandle_;
};
