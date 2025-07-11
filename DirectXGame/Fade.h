#pragma once
#include "KamataEngine.h"

using namespace KamataEngine;

class Fade {
public:
public:
	void Initialize();

	void Update();

	void Draw();

private:
	Sprite* sprite_ = nullptr;
};
