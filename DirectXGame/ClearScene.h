#pragma once
#include "Fade.h"
#include "KamataEngine.h"

using namespace KamataEngine;

class ClearScene {
public:
	enum class Phase {
		kFadeIn, // フェードイン
		kMain,   // メイン部
		kFadeOut // フェードアウト
	};

	void Initialize();

	void Update();

	void Draw();

	~ClearScene();

	bool isFinished() const { return finished_; };

private:
	// 3Dモデル
	Model* modelClearFont_ = nullptr;

	WorldTransform worldTransformClearFont_;

	// カメラ
	Camera camera_;

	float counter_ = 0.0f;

	static inline const float kTimeClearFontMove = 2.0f;

	bool finished_ = false;

	Fade* fade_ = nullptr;

	Phase phase_ = Phase::kFadeIn;
};
