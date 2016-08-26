#include <cmath>
#include "Vector2.h"

Vector2::Vector2( float a, float b ) : x(a), y(b)
{
}

Vector2::Vector2(const Vector2 &rhs) : x(rhs.x), y(rhs.y)
{
}

void Vector2::Set( float a, float b )
{
	x = a;
	y = b;
}

bool Vector2::IsZero() const
{
	return x == 0 && y == 0;
}

Vector2 Vector2::operator+( const Vector2& rhs ) const
{
	return Vector2(x + rhs.x, y + rhs.y);
}

Vector2 Vector2::operator-( const Vector2& rhs ) const
{
	return Vector2(x - rhs.x, y - rhs.y);
}

Vector2 Vector2::operator-( void ) const
{
	return Vector2(-x, -y);
}

Vector2 Vector2::operator*( float scalar ) const
{
	return Vector2(scalar * x, scalar * y);
}

Vector2 Vector2::operator/(float divisor) const
{
	return Vector2(x / divisor, y / divisor);
}

Vector2& Vector2::operator+= (const Vector2 &rhs)
{
	this->x += rhs.x;
	this->y += rhs.y;

	return *this;
}

Vector2& Vector2::operator-= (const Vector2 &rhs)
{
	this->x -= rhs.x;
	this->y -= rhs.y;

	return *this;
}

Vector2& Vector2::operator*= (const float &factor)
{
	this->x *= factor;
	this->y *= factor;

	return *this;
}

Vector2& Vector2::operator/= (const float &factor)
{
	this->x /= factor;
	this->y /= factor;

	return *this;
}

float Vector2::Length( void ) const
{
	return sqrtf(x * x + y * y);
}

float Vector2::LengthSquared() const
{
	return x*x + y*y;
}

float Vector2::Dot( const Vector2& rhs ) const
{
	return x * rhs.x + y * rhs.y;
}

Vector2& Vector2::Normalize()
{
	float l = Length();

	x /= l;
	y /= l;

	return *this;
}

Vector2 Vector2::Normalized( void ) const
{
	return Vector2(x, y) / Length();
}
