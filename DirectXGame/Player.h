#pragma once
#include "KamataEngine.h"
#include "calculation.h"

class Player {
public:
	void Initialize(KamataEngine::Model* model, KamataEngine::Camera* camera);

	void Update();

	void Draw();

private:
	std::vector<std::vector<KamataEngine::WorldTransform*>> worldTransformBlocks_;

	// ワールド変換データ
	KamataEngine::WorldTransform worldTransform_;

	// モデル
	KamataEngine::Model* model_ = nullptr;

	// カメラ
	KamataEngine::Camera* camera_;
};
