#pragma once
#include "Fade.h"
#include "KamataEngine.h"
using namespace KamataEngine;

class SelectScene {
public:
	enum class Phase {
		kFadeIn, // フェードイン
		kMain,   // メイン部
		kFadeOut // フェードアウト
	};

	enum Stage {
		ksousa,
		k1_1,
		k1_2,

	};

	void Initialize();

	void Update();

	void Draw();

	~SelectScene();

	bool isFinished() const { return finished_; };

	int GetStageNumber() { return stageNumber_; };

private:
	// カメラ
	Camera camera_;

	float counter_ = 0.0f;

	bool finished_ = false;

	Fade* fade_ = nullptr;

	Phase phase_ = Phase::kFadeIn;

	Stage Stage_ = Stage::k1_1;

	int stageNumber_;
};
