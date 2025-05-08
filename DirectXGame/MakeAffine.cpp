#include "MakeAffine.h"

using namespace KamataEngine;

Matrix4x4 MakeAffine::MakeRotateXMatrix(float radian) {
	Matrix4x4 num;
	num = {1, 0, 0, 0, 0, std::cos(radian), std::sin(radian), 0, 0, std::sin(-radian), std::cos(radian), 0, 0, 0, 0, 1};
	return num;
}

Matrix4x4 MakeAffine::MakeRotateYMatrix(float radian) {
	Matrix4x4 num;
	num = {std::cos(radian), 0, std::sin(-radian), 0, 0, 1, 0, 0, std::sin(radian), 0, std::cos(radian), 0, 0, 0, 0, 1};
	return num;
}

Matrix4x4 MakeAffine::MakeRotateZMatrix(float radian) {
	Matrix4x4 num;
	num = {std::cos(radian), std::sin(radian), 0, 0, std::sin(-radian), std::cos(radian), 0, 0, 0, 0, 1, 0, 0, 0, 0, 1};
	return num;
}

Matrix4x4 MakeAffine::Mulyiply(const Matrix4x4& m1, const Matrix4x4& m2) {
	Matrix4x4 num;
	num.m[0][0] = m1.m[0][0] * m2.m[0][0] + m1.m[0][1] * m2.m[1][0] + m1.m[0][2] * m2.m[2][0] + m1.m[0][3] * m2.m[3][0];
	num.m[0][1] = m1.m[0][0] * m2.m[0][1] + m1.m[0][1] * m2.m[1][1] + m1.m[0][2] * m2.m[2][1] + m1.m[0][3] * m2.m[3][1];
	num.m[0][2] = m1.m[0][0] * m2.m[0][2] + m1.m[0][1] * m2.m[1][2] + m1.m[0][2] * m2.m[2][2] + m1.m[0][3] * m2.m[3][2];
	num.m[0][3] = m1.m[0][0] * m2.m[0][3] + m1.m[0][1] * m2.m[1][3] + m1.m[0][2] * m2.m[2][3] + m1.m[0][3] * m2.m[3][3];

	num.m[1][0] = m1.m[1][0] * m2.m[0][0] + m1.m[1][1] * m2.m[1][0] + m1.m[1][2] * m2.m[2][0] + m1.m[1][3] * m2.m[3][0];
	num.m[1][1] = m1.m[1][0] * m2.m[0][1] + m1.m[1][1] * m2.m[1][1] + m1.m[1][2] * m2.m[2][1] + m1.m[1][3] * m2.m[3][1];
	num.m[1][2] = m1.m[1][0] * m2.m[0][2] + m1.m[1][1] * m2.m[1][2] + m1.m[1][2] * m2.m[2][2] + m1.m[1][3] * m2.m[3][2];
	num.m[1][3] = m1.m[1][0] * m2.m[0][3] + m1.m[1][1] * m2.m[1][3] + m1.m[1][2] * m2.m[2][3] + m1.m[1][3] * m2.m[3][3];

	num.m[2][0] = m1.m[2][0] * m2.m[0][0] + m1.m[2][1] * m2.m[1][0] + m1.m[2][2] * m2.m[2][0] + m1.m[2][3] * m2.m[3][0];
	num.m[2][1] = m1.m[2][0] * m2.m[0][1] + m1.m[2][1] * m2.m[1][1] + m1.m[2][2] * m2.m[2][1] + m1.m[2][3] * m2.m[3][1];
	num.m[2][2] = m1.m[2][0] * m2.m[0][2] + m1.m[2][1] * m2.m[1][2] + m1.m[2][2] * m2.m[2][2] + m1.m[2][3] * m2.m[3][2];
	num.m[2][3] = m1.m[2][0] * m2.m[0][3] + m1.m[2][1] * m2.m[1][3] + m1.m[2][2] * m2.m[2][3] + m1.m[2][3] * m2.m[3][3];

	num.m[3][0] = m1.m[3][0] * m2.m[0][0] + m1.m[3][1] * m2.m[1][0] + m1.m[3][2] * m2.m[2][0] + m1.m[3][3] * m2.m[3][0];
	num.m[3][1] = m1.m[3][0] * m2.m[0][1] + m1.m[3][1] * m2.m[1][1] + m1.m[3][2] * m2.m[2][1] + m1.m[3][3] * m2.m[3][1];
	num.m[3][2] = m1.m[3][0] * m2.m[0][2] + m1.m[3][1] * m2.m[1][2] + m1.m[3][2] * m2.m[2][2] + m1.m[3][3] * m2.m[3][2];
	num.m[3][3] = m1.m[3][0] * m2.m[0][3] + m1.m[3][1] * m2.m[1][3] + m1.m[3][2] * m2.m[2][3] + m1.m[3][3] * m2.m[3][3];

	return num;
}

Matrix4x4 MakeAffine::MakeAffineMatrix(const Vector3& scale, const Vector3& rotate, const Vector3& translate) {
	Matrix4x4 rotateX = MakeRotateXMatrix(rotate.x);
	Matrix4x4 rotateY = MakeRotateYMatrix(rotate.y);
	Matrix4x4 rotateZ = MakeRotateZMatrix(rotate.z);
	Matrix4x4 rotateXYZ = Mulyiply(rotateX, Mulyiply(rotateY, rotateZ));

	Matrix4x4 num;
	num.m[0][0] = scale.x * rotateXYZ.m[0][0];
	num.m[0][1] = scale.x * rotateXYZ.m[0][1];
	num.m[0][2] = scale.x * rotateXYZ.m[0][2];
	num.m[0][3] = 0.0f * 0.0f * 0.0f * 0.0f;
	num.m[1][0] = scale.y * rotateXYZ.m[1][0];
	num.m[1][1] = scale.y * rotateXYZ.m[1][1];
	num.m[1][2] = scale.y * rotateXYZ.m[1][2];
	num.m[1][3] = 0.0f * 0.0f * 0.0f * 0.0f;
	num.m[2][0] = scale.z * rotateXYZ.m[2][0];
	num.m[2][1] = scale.z * rotateXYZ.m[2][1];
	num.m[2][2] = scale.z * rotateXYZ.m[2][2];
	num.m[2][3] = 0.0f * 0.0f * 0.0f * 0.0f;
	num.m[3][0] = translate.x;
	num.m[3][1] = translate.y;
	num.m[3][2] = translate.z;
	num.m[3][3] = 1.0f;
	return num;
}
