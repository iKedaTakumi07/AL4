#pragma once
#include <KamataEngine.h>

using namespace KamataEngine;

class CameraController {
public:
	void Initialize(Camera* camera);

	void Update();

private:
	Camera* camera_ = nullptr;
};
