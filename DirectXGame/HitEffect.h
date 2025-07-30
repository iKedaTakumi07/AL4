#pragma once
#include "KamataEngine.h"
#include "Math.h"

using namespace KamataEngine;

class HitEffect {
public:
	void Initialize(const Vector3 &position);

	void Update();

	void Draw();

	static void SetModel(Model* model) { model_ = model; };

	static void SetCamera(Camera* camera) { camera_ = camera; };

	static HitEffect* Create(const Vector3& position);



private:
	static Model* model_;

	static Camera* camera_;

	WorldTransform circleWorldTransform_;

	ObjectColor objectColor_;
};
