#pragma once
#include "KamataEngine.h"
#include "Math.h"

using namespace KamataEngine;

class HitEffect {
public:
	void Initialize();

	void Update();

	void Draw();

	static void SetModel(Model* model) { model_ = model; };

	static void SetCamera(Camera* camera) { camera_ = camera; };

private:
	static Model* model_;

	static Camera* camera_;
};
