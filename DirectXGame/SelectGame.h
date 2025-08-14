#pragma once
#include "Fade.h"
#include "KamataEngine.h"
using namespace KamataEngine;

class SelectGame {
public:
	enum class Phase {
		kFadeIn, // フェードイン
		kMain,   // メイン部
		kFadeOut // フェードアウト
	};

	void Initialize();

	void Update();

	void Draw();

	~SelectGame();

	bool isFinished() const { return finished_; };

private:
	// カメラ
	Camera camera_;

	float counter_ = 0.0f;

	bool finished_ = false;

	Fade* fade_ = nullptr;

	Phase phase_ = Phase::kFadeIn;
};
