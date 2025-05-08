#pragma once
#include "KamataEngine.h"
using namespace KamataEngine;

class MakeAffine {
public:
	Matrix4x4 MakeRotateXMatrix(float radian);
	Matrix4x4 MakeRotateYMatrix(float radian);
	Matrix4x4 MakeRotateZMatrix(float radian);
	Matrix4x4 Mulyiply(const Matrix4x4& m1, const Matrix4x4& m2);
	Matrix4x4 MakeAffineMatrix(const Vector3& scale, const Vector3& rotate, const Vector3& translate);

private:
};
