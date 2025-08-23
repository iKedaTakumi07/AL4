#include "Math.h"
#include <cassert>

using namespace KamataEngine;

Vector3 operator+(const Vector3& v) { return v; }
Vector3 operator-(const Vector3& v) { return Vector3(-v.x, -v.y, -v.z); }

Matrix4x4 MakeRotateXMatrix(float radian);
Matrix4x4 MakeRotateYMatrix(float radian);
Matrix4x4 MakeRotateZMatrix(float radian);
Matrix4x4 Mulyiply(const Matrix4x4& m1, const Matrix4x4& m2);
Matrix4x4 MakeAffineMatrix(const Vector3& scale, const Vector3& rotate, const Vector3& translate);

void WorldtransformUpdate(KamataEngine::WorldTransform& worldTransform) {
	// 行列計算
	worldTransform.matWorld_ = MakeAffineMatrix(worldTransform.scale_, worldTransform.rotation_, worldTransform.translation_);
	// 書き込み
	worldTransform.TransferMatrix();
}

Matrix4x4 MakeRotateXMatrix(float radian) {
	Matrix4x4 num;
	num = {1, 0, 0, 0, 0, std::cos(radian), std::sin(radian), 0, 0, std::sin(-radian), std::cos(radian), 0, 0, 0, 0, 1};
	return num;
}

Matrix4x4 MakeRotateYMatrix(float radian) {
	Matrix4x4 num;
	num = {std::cos(radian), 0, std::sin(-radian), 0, 0, 1, 0, 0, std::sin(radian), 0, std::cos(radian), 0, 0, 0, 0, 1};
	return num;
}

Matrix4x4 MakeRotateZMatrix(float radian) {
	Matrix4x4 num;
	num = {std::cos(radian), std::sin(radian), 0, 0, std::sin(-radian), std::cos(radian), 0, 0, 0, 0, 1, 0, 0, 0, 0, 1};
	return num;
}

Matrix4x4 Mulyiply(const Matrix4x4& m1, const Matrix4x4& m2) {
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

Matrix4x4 MakeAffineMatrix(const Vector3& scale, const Vector3& rotate, const Vector3& translate) {
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

Vector3& operator+=(Vector3& lhv, const Vector3& rhv) {
	lhv.x += rhv.x;
	lhv.y += rhv.y;
	lhv.z += rhv.z;
	return lhv;
}
const Vector3 operator-=(Vector3& v1, const Vector3& v2) {
	v1.x -= v2.x;
	v1.y -= v2.y;
	v1.z -= v2.z;

	return v1;
}

Vector3& operator*=(Vector3& v, float s) {
	v.x *= s;
	v.y *= s;
	v.z *= s;
	return v;
}

Vector3& operator/=(Vector3& v, float s) {
	v.x /= s;
	v.y /= s;
	v.z /= s;
	return v;
}

Matrix4x4& operator*=(Matrix4x4& lhm, const Matrix4x4& rhm) {
	Matrix4x4 result{};

	for (size_t i = 0; i < 4; i++) {
		for (size_t j = 0; j < 4; j++) {
			for (size_t k = 0; k < 4; k++) {
				result.m[i][j] += lhm.m[i][k] * rhm.m[k][j];
			}
		}
	}
	lhm = result;
	return lhm;
}

Matrix4x4 operator*(const Matrix4x4& m1, const Matrix4x4& m2) {
	Matrix4x4 result = m1;

	return result *= m2;
}

const Vector3 operator+(const Vector3& v1, const Vector3& v2) {
	Vector3 temp(v1);
	return temp += v2;
}

// 線形保管の関数
Vector3 Lerp(Vector3 x1, Vector3 x2, float t) { return {(1.0f - t) * x1.x + t * x2.x, (1.0f - t) * x1.y + t * x2.y, (1.0f - t) * x1.z + t * x2.z}; }
float Lerp(float x1, float x2, float t) { return (1.0f - t) * x1 + t * x2; }

float EaseIn(float x1, float x2, float t) {
	float easedT = t * t;

	return Lerp(x1, x2, easedT);
}

float EaseOut(float x1, float x2, float t) {
	float easedT = 1.0f - std::powf(1.0f - t, 3.0f);

	return Lerp(x1, x2, easedT);
}

float easeInOutQuint(float s, float e, float t) {
	float easedT;
	float rotationY;

	easedT = (t < 0.5 ? 8 * t * t * t * t : 1 - std::powf(-2 * t + 2, 4) / 2);
	if (easedT >= 1.0f) {
		easedT = 1.0f;
	}

	rotationY = (1.0f - easedT) * s + easedT * e;

	return rotationY;
}

bool IsCollision(const AABB& aabb1, const AABB& aabb2) {

	return (aabb1.min.x <= aabb2.max.x && aabb1.max.x >= aabb2.min.x) && (aabb1.min.y <= aabb2.max.y && aabb1.max.y >= aabb2.min.y) && (aabb1.min.z <= aabb2.max.z && aabb1.max.z >= aabb2.min.z);
}

Vector3 Transform(const Vector3& vector, const Matrix4x4& matrix) {
	Vector3 result;
	result.x = vector.x * matrix.m[0][0] + vector.y * matrix.m[1][0] + vector.z * matrix.m[2][0] + 1.0f * matrix.m[3][0];
	result.y = vector.x * matrix.m[0][1] + vector.y * matrix.m[1][1] + vector.z * matrix.m[2][1] + 1.0f * matrix.m[3][1];
	result.z = vector.x * matrix.m[0][2] + vector.y * matrix.m[1][2] + vector.z * matrix.m[2][2] + 1.0f * matrix.m[3][2];
	float w = vector.x * matrix.m[0][3] + vector.y * matrix.m[1][3] + vector.z * matrix.m[2][3] + 1.0f * matrix.m[3][3];
	assert(w != 0.0f);

	result.x /= w;
	result.y /= w;
	result.z /= w;
	return result;
}

float EaseInQuint(float t) { return t * t * t * t * t; }