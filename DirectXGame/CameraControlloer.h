#pragma once
#include <KamataEngine.h>

using namespace KamataEngine;

class CameraControlloer {
public:
	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

private:
	Camera* camera_ = nullptr;
};
