#pragma once

union vec2{
	struct{
		int x;
		int y;
	};
	struct{
		int width;
		int height;
	};
};

inline int getDistanceSquared(const vec2 vector)
{
	return (vector.x * vector.x) + (vector.y * vector.y);
}

inline int getArea(const vec2 dimension)
{
	return dimension.x * dimension.y;
}

inline vec2 operator+(const vec2 &left, const vec2 &right)
{
	return vec2{left.x + right.x, left.y + right.y};
}

inline vec2 operator-(const vec2 &left, const vec2 &right)
{
	return vec2{left.x - right.x, left.y - right.y};
}

inline vec2 operator*(const vec2 &left, const vec2 &right)
{
	return vec2{left.x * right.x, left.y * right.y};
}

inline vec2 operator/(const vec2 &left, const vec2 &right)
{
	return vec2{left.x / right.x, left.y / right.y};
}

inline vec2 operator+(const vec2 &left, const int &right)
{
	return vec2{left.x + right, left.y + right};
}

inline vec2 operator-(const vec2 &left, const int &right)
{
	return vec2{left.x - right, left.y - right};
}

inline vec2 operator*(const vec2 &left, const int &right)
{
	return vec2{left.x * right, left.y * right};
}

inline vec2 operator/(const vec2 &left, int &right)
{
	return vec2{left.x / right, left.y / right};
}