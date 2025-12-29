#pragma once
#include "Fade.h"
#include "KamataEngine.h"
#include "Skydome.h"
#include <vector>
using namespace KamataEngine;

class SelectScene {
public:
	enum class Phase {
		kFadeIn, // フェードイン
		kMain,   // メイン部
		kFadeOut // フェードアウト
	};

	enum Stage { ksousa, k1_1, k1_2, k1_3 };

	void Initialize();

	void Update();

	void Draw();

	~SelectScene();

	bool isFinished() const { return finished_; };

	int GetStageNumber() { return stageNumber_; };

private:
	// 数字モデル
	std::vector<Model*> models_;
	std::vector<WorldTransform*> worldTransforms_;

	Model* SelectModel_;
	Model* modelSkydome_ = nullptr;

	WorldTransform SelectTransform_;

	Model* SelectkeyModel_;
	// 背景
	Skydome* skydome_ = nullptr;

	WorldTransform SelectkeyTransform_;

	// カメラ
	Camera camera_;

	float counter_ = 0.0f;

	bool finished_ = false;

	Fade* fade_ = nullptr;

	Phase phase_ = Phase::kFadeIn;

	Stage Stage_ = Stage::ksousa;

	int stageNumber_;

	// スライドアニメーション管理
	struct Animation {
		bool isPlaying = false;
		float timer = 0.0f;
		float duration = 0.5f;
		float startOffsetX = 0.0f;
		float endOffsetX = 0.0f;
	};
	Animation slideAnim_;

	// 全体オフセット
	float slideOffsetX_ = 0.0f;

	// 移動量（1回の移動でずれる距離）
	float slideStepX_ = 30.0f;
};
