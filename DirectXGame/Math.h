#pragma once
#include "KamataEngine.h"

using namespace KamataEngine;

void WolrdtransformUpdate(KamataEngine::WorldTransform& worldTransform);

KamataEngine::Vector3 Lerp(KamataEngine::Vector3 x1, KamataEngine::Vector3 x2, float t);

float easeInOutQuint(float x1, float x2, float t);

const Vector3 operator+(const Vector3& lhv, const Vector3& rhv);

Vector3& operator+=(Vector3& lhs, const Vector3& rhv);