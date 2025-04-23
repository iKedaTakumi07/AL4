#pragma once
#include "KamataEngine.h"

class Player {
public:
	//
	void Initialize(KamataEngine::Model* model, uint32_t textureHandle, KamataEngine::Camera* camera);

	//
	void Update();

	//
	void Draw();

private:
	// 3Dモデル
	KamataEngine::Model* model_ = nullptr;

	// ワールドトランスフォームも初期化
	KamataEngine::WorldTransform worldTransform_;

	// カメラ
	KamataEngine::Camera* camea_ = nullptr;

	// テクスチャハンドル
	uint32_t textureHandle_ = 0;
};
