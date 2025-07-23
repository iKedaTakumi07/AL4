#pragma once
#include "KamataEngine.h"
#include "Math.h"

using namespace KamataEngine;

class HitEffect {
public:
	void Initialize();

	void Update();

	void Draw();

private:
	static Model* model_;

	static Camera* camera_;
};
