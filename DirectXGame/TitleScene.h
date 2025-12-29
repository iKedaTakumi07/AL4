#pragma once
#include "Fade.h"
#include "KamataEngine.h"
using namespace KamataEngine;

class TitleScene {
public:
	enum class Phase {
		kFadeIn, // フェードイン
		kMain,   // メイン部
		kFadeOut // フェードアウト
	};

	void Initialize();

	void Update();

	void Draw();

	~TitleScene();

	bool isFinished() const { return finished_; };

private:
	// 3Dモデル
	Model* modelTitle_ = nullptr;
	Model* modelPlayer_ = nullptr;
	Model* modelStart_ = nullptr;

	WorldTransform worldTransformTitle_;
	WorldTransform worldTransformplayer_;
	WorldTransform worldTransformStart_;

	// カメラ
	Camera camera_;

	float counter_ = 0.0f;

	static inline const float kTimeTitleMove = 2.0f;

	bool finished_ = false;

	Fade* fade_ = nullptr;

	Phase phase_ = Phase::kFadeIn;
};
