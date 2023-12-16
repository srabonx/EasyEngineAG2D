#include "EasyEngineAG2D.h"

namespace EE {
	Vector3::Vector3()
	{
		x = y = z = 0;
	}
	Vector3::Vector3(const Vector3& v)
	{
		*this = v;
	}
	Vector3::Vector3(double x, double y, double z)
	{
		Set(x, y, z);
	}
	Vector3::Vector3(int x, int y, int z)
	{
		Set((double)x, (double)y, (double)z);
	}
	void Vector3::Set(double x1, double y1, double z1)
	{
		x = x1; y = y1; z = z1;
	}
	void Vector3::Set(const Vector3& v)
	{
		x = v.x; y = v.y; z = v.z;
	}
	void Vector3::Move(double mx, double my, double mz)
	{
		x += mx; y += my; z += mz;
	}
	void Vector3::operator+=(const Vector3& rhs)
	{
		x += rhs.x; y += rhs.y; z += rhs.z;
	}
	void Vector3::operator-=(const Vector3& rhs)
	{
		x -= rhs.x; y -= rhs.y; z -= rhs.z;
	}
	
	void Vector3::operator*=(const Vector3& rhs)
	{
		x *= rhs.x; y *= rhs.y; z *= rhs.z;
	}
	void Vector3::operator/=(const Vector3& rhs)
	{
		x /= rhs.x; y /= rhs.y; z /= rhs.z;
	}
	// Equality comparison operator requires double rounding
	bool Vector3::operator==(const Vector3& rhs) const
	{
		return(
			(((rhs.x - 0.0001f) < x) && (x < (rhs.x + 0.0001f))) &&
			(((rhs.y - 0.0001f) < y) && (y < (rhs.y + 0.0001f))) &&
			(((rhs.z - 0.0001f) < z) && (z < (rhs.z + 0.0001f)))
			);
	}
	// inequality operator
	bool Vector3::operator!=(const Vector3& rhs) const
	{
		return (!(*this == rhs));
	}
	// Assign operator
	Vector3& Vector3::operator=(const Vector3& rhs)
	{
		Set(rhs);
		return *this;
	}
	// Distance only coded for 2D
	double Vector3::Distance(const Vector3& v)
	{
		return sqrt((v.x - x) * (v.x - x) + (v.y - y) * (v.y - y));
	}
	// Vector3 length is distance from the origin
	double Vector3::Length()
	{
		return sqrt(x * x + y * y + z * z);
	}

	// Dot/Scalar product: Difference between two directions
	double Vector3::DotProduct(const Vector3& v)
	{	
		return (x * v.x + y * v.y + z * v.y);
	}

	// Cross/vector product is used to calculate the normal
	Vector3 Vector3::CrossProduct(const Vector3& v)
	{
		double nx = (y * v.z) - (z * v.y);
		double ny = (z * v.x) - (x * v.z);
		double nz = (x * v.y) - (y * v.x);

		return Vector3(nx, ny, nz);
	}
	// Calculate normal angle of the Vector
	Vector3 Vector3::Normal()
	{
		double length;
		if (Length() == 0)
			length = 0;
		else
			length = 1 / Length();
		double nx = x * length;
		double ny = y * length;
		double nz = z * length;
		return Vector3(nx, ny, nz);
	}
}// namespace