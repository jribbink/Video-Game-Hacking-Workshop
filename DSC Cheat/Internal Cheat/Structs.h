#pragma once
struct PaddedVector3D {
	float x;              //0x00
	char _pad0x04[0xC];   //0x04
	float y;              //0x10
	char _pad0x14[0xC];  //0x14
	float z;              //0x20
};
struct Vector2D
{
	float x = 0, y = 0;
	Vector2D() {}
	Vector2D(float X, float Y)
	{
		x = X;
		y = Y;
	}
	float& operator [](int i) { if (i == 0) return x; else if (i == 1) return y; }
};
inline Vector2D operator-(const Vector2D& x, const Vector2D& y)
{
	Vector2D vector;
	vector.x = x.x - y.x;
	vector.y = x.y - y.y;
	return vector;
}
inline Vector2D operator+(const Vector2D& x, const Vector2D& y)
{
	Vector2D vector;
	vector.x = x.x + y.x;
	vector.y = x.y + y.y;
	return vector;
}
inline Vector2D operator*(const Vector2D& x, const float& y)
{
	Vector2D vector;
	vector.x = x.x * y;
	vector.y = x.y * y;
	return vector;
}

struct Vector3D
{
	float x = 0, y = 0, z = 0;
	Vector3D() {}
	Vector3D(float X, float Y, float Z)
	{
		x = X;
		y = Y;
		z = Z;
	}
	float& operator [](int i) { if (i == 0) return x; else if (i == 1) return y; else if (i == 2) return z; }

};
inline Vector3D operator-(const Vector3D& x, const Vector3D& y)
{
	Vector3D vector;
	vector.x = x.x - y.x;
	vector.y = x.y - y.y;
	vector.z = x.z - y.z;
	return vector;
}
inline Vector3D operator+(const Vector3D& x, const Vector3D& y)
{
	Vector3D vector;
	vector.x = x.x + y.x;
	vector.y = x.y + y.y;
	vector.z = x.z + y.z;
	return vector;
}
inline Vector3D operator-(const Vector3D& x, const PaddedVector3D& y)
{
	Vector3D vector;
	vector.x = x.x - y.x;
	vector.y = x.y - y.y;
	vector.z = x.z - y.z;
	return vector;
}
inline Vector3D operator+(const Vector3D& x, const PaddedVector3D& y)
{
	Vector3D vector;
	vector.x = x.x + y.x;
	vector.y = x.y + y.y;
	vector.z = x.z + y.z;
	return vector;
}
inline Vector3D operator-(const PaddedVector3D& x, const Vector3D& y)
{
	Vector3D vector;
	vector.x = x.x - y.x;
	vector.y = x.y - y.y;
	vector.z = x.z - y.z;
	return vector;
}
inline Vector3D operator+(const PaddedVector3D& x, const Vector3D& y)
{
	Vector3D vector;
	vector.x = x.x + y.x;
	vector.y = x.y + y.y;
	vector.z = x.z + y.z;
	return vector;
}

struct glow_t {
	int m_nNextFreeSlot;
	int pEntity;
	float r;
	float g;
	float b;
	float a;
	bool m_bGlowAlphaCappedByRenderAlpha;
	float m_flGlowAlphaFunctionOfMaxVelocity;
	float m_flGlowAlphaMax;
	float m_flGlowPulseOverdrive;
	bool m_bRenderWhenOccluded;
	bool m_bRenderWhenUnoccluded;
	bool m_bFullBloomRender;
	int m_nFullBloomStencilTestValue;
	int m_nRenderStyle;
	int m_nSplitScreenSlot;
};

//Conversions
const float DegToRad = 3.14159265f / 180.0f;
const float RadToDeg = 180.0f / 3.14159265f;