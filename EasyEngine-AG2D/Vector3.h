#include "EasyEngineAG2D.h"
#pragma once

namespace EE {
	class Vector3 {
	private:
		double x, y, z;
	public:
		Vector3();
		Vector3(const Vector3& v);
		Vector3(double x, double y, double z);
		Vector3(int x, int y, int z);
		void Set(double x1, double y1, double z1);
		void Set(const Vector3& v);
		double getX() { return x; }
		void setX(double value) { x = value; }
		double getY() { return y; }
		void setY(double value) { y = value; }
		double getZ() { return z; }
		void setZ(double value) { z = value; }
		void Move(double mx, double my, double mz);
		void operator +=(const Vector3& rhs);
		void operator -=(const Vector3& rhs);
		void operator *=(const Vector3& rhs);
		void operator /=(const Vector3& rhs);
		bool operator ==(const Vector3& rhs) const;
		bool operator !=(const Vector3& rhs) const;
		Vector3& operator =(const Vector3& rhs);
		double Distance(const Vector3& v);
		double Length();
		double DotProduct(const Vector3& v);
		Vector3 CrossProduct(const Vector3& v);
		Vector3 Normal();

	}; // Class
}; // Namespace
