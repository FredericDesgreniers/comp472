#pragma once

struct vec2{
	union
	{
		struct
		{
			int x;
			int y;
		};
		struct
		{
			int width;
			int height;
		};
	};
	vec2(int value){
		x = value;
		y = value;
	}
	vec2(int a, int b)
	{
		x = a;
		y = b;
	}
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