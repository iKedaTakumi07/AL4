#pragma once
#include "KamataEngine.h"

using namespace KamataEngine;

struct AABB {
	Vector3 min;
	Vector3 max;
};

Matrix4x4 MakeRotateZMatrix(float radian);

void WorldtransformUpdate(KamataEngine::WorldTransform& worldTransform);

KamataEngine::Vector3 Lerp(KamataEngine::Vector3 x1, KamataEngine::Vector3 x2, float t);

float easeInOutQuint(float x1, float x2, float t);

const Vector3 operator+(const Vector3& lhv, const Vector3& rhv);

Vector3& operator+=(Vector3& lhs, const Vector3& rhv);
Vector3 operator+(const Vector3& v);
Vector3 operator-(const Vector3& v);

bool IsCollision(const AABB& aabb1, const AABB& aabb2);

Vector3 Transform(const Vector3& vector, const Matrix4x4& matrix);

float Lerp(float x1, float x2, float t);

float EaseIn(float x1, float x2, float t);

float EaseOut(float x1, float x2, float t);

inline float ToRadians(float degrees) { return degrees * (3.1415f / 180.0f); }
inline float ToDegrees(float radians) { return radians * (180.0f / 3.1415f); }