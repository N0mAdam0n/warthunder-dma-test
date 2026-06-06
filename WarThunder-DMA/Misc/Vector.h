#pragma once
#include <Vector.h>
class Vector2
{
public:
	Vector2();
	Vector2(float _x, float _y);
	~Vector2();

	float x, y;

	Vector2 operator *(Vector2 a);
	Vector2 operator /(Vector2 a);
	Vector2 operator +(Vector2 a);
	Vector2 operator -(Vector2 a);
	bool operator ==(Vector2 a);
	bool operator !=(Vector2 a);

	bool IsZero() const;

	static Vector2 Zero();

	static float Distance(Vector2 a, Vector2 b);
};

struct matrix4x4
{
	float matrix[4][4];

	float* operator[](int i) { return matrix[i]; }
	const float* operator[](int i) const { return matrix[i]; }
};

struct Matrix3x3
{
	float data[3][3] = { };
	constexpr float* operator[](int index) noexcept
	{
		return data[index];
	}

	constexpr const float* operator[](int index) const noexcept
	{
		return data[index];
	}


};

class Vector3
{
public:
	Vector3();
	Vector3(float _x, float _y, float _z);
	~Vector3();

	float x, y, z;

	Vector3 operator *(Vector3 a);
	Vector3 operator *(float f);
	Vector3 operator /(Vector3 a);
	Vector3 operator /(float f);
	Vector3 operator +(Vector3 a);
	Vector3 operator -(Vector3 a);
	Vector3 operator -(const Vector3& other) const;
	bool operator ==(Vector3 a);
	bool operator !=(Vector3 a);

	bool IsZero() const;
	bool IsValid() const;

	static float Dot(Vector3 left, Vector3 right);
	inline float Dot(Vector3 v)
	{
		return x * v.x + y * v.y + z * v.z;
	}
	static float Distance(Vector3 a, Vector3 b);
	inline float Distance(Vector3 v)
	{
		return float(sqrtf(powf(v.x - x, 2.0) + powf(v.y - y, 2.0) + powf(v.z - z, 2.0)));
	}
	static int FormattedDistance(Vector3 a, Vector3 b);
	static Vector3 Zero();
	static Vector3 Lerp(Vector3 a, Vector3 b, float t);

	float Length() const;
	float LengthSqr() const;

	Vector3 Clamp() const;

	Vector3 operator* (const Matrix3x3& rotation_matrix) const noexcept
	{
		Vector3 result;
		result.x = x * rotation_matrix[0][0] + y * rotation_matrix[1][0] + z * rotation_matrix[2][0];
		result.y = x * rotation_matrix[0][1] + y * rotation_matrix[1][1] + z * rotation_matrix[2][1];
		result.z = x * rotation_matrix[0][2] + y * rotation_matrix[1][2] + z * rotation_matrix[2][2];
		return result;
	}
};

struct ViewMatrix
{
public:
	float matrix[4][4];

	Vector3 Transform(const Vector3 vector) const;
};


class Vector4
{
public:
	Vector4() : x(0.0f), y(0.0f), z(0.0f), w(0.0f) {}
	Vector4(float _x, float _y, float _z, float _w) : x(_x), y(_y), z(_z), w(_w) {}
	Vector4 operator*(const matrix4x4& mat) const noexcept
	{
		Vector4 result;

		result.x = x * mat.matrix[0][0] + y * mat.matrix[1][0] + z * mat.matrix[2][0] + w * mat.matrix[3][0];
		result.y = x * mat.matrix[0][1] + y * mat.matrix[1][1] + z * mat.matrix[2][1] + w * mat.matrix[3][1];
		result.z = x * mat.matrix[0][2] + y * mat.matrix[1][2] + z * mat.matrix[2][2] + w * mat.matrix[3][2];
		result.w = x * mat.matrix[0][3] + y * mat.matrix[1][3] + z * mat.matrix[2][3] + w * mat.matrix[3][3];

		return result;
	}
	float x, y, z, w;
};

Vector4 operator*(const ViewMatrix& m, const Vector4& v);